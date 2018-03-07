<?php

/**
 * Implements hook_drush_command().
 *
 * {@inheritdoc}
 */
function dev_helper_drush_command() {
  $items  = array();
  $items['string-search'] = array(
    'callback'    => 'dev_helper_drush_search_string',
    'description' => t('Search node output for a string.'),
    'arguments'   => [
      'str' => 'String to search in content.'
    ],
    'options' => [
      'verbose'    => 'Drush setting to show watchdog/error messages in command line',
    ],
    'examples' => [
      'Search output for a url www.google.com' => 'drush string-search "www.google.com"'
    ]
  );
  $items['reset-db'] = array(
    'callback'    => 'dev_helper_drush_reset_db',
    'description' => t('Load in a new sql.gz file and prompt user for common local dev commands.'),
//    'arguments'   => [
//      'skip' => 'Skip all prompts and just load db gzip.'
//    ],
//    'options' => [
//      'verbose'    => 'Drush setting to show watchdog/error messages in command line',
//    ],
    'examples' => [
      'drush reset-db' => 'Run drush command and related bash scripts.'
    ]
  );
  return $items;
}

function dev_helper_drush_help($section) {
  switch ($section) {
    case 'drush:string-search':
      return t("Run String-Search.");
    case 'drush:reset-db':
      return t("Load in a new sql file and prompt user for common local dev commands.");
  }
}

/**
 * Callback function for hook_drush_command().
 *
 * @param $str
 */
function dev_helper_drush_search_string ($str = '') {
  ini_set('max_execution_time', 0); // infinite
  if (!empty($str)) {
    $counter = 0;
    $results = db_query('SELECT * FROM node');
    $total = $results->rowCount();
    $quarters = round($total/4);
    $percent = 1;
    $nids = [];
    drush_print('Starting search string.');
    foreach ($results as $index => $result) {
      // print message when each 1/4th is scanned
      if ($index == ($quarters*$percent)) {
        $percent++;
        $currentPercent = round(($index/$total)*100);
        drush_print('Percent done ' . $currentPercent);
      }
      $node = node_load($result->nid);
      $content = drupal_render(node_view($node));
      preg_match('/(' . $str . ')/', $content, $matches, PREG_OFFSET_CAPTURE);
      if (!empty($matches)) {
        $nids[] = $result->nid;
      }
      $counter = $index;
    }
    drush_print('Search String Results: ' . print_r($nids, TRUE));
    watchdog('Dev Helper - Search String',
      'After searching :counter nodes, ":str" was found in these node ids<br> :nids',
      [':str' => $str, ':nids' => print_r($nids, TRUE), ':counter' => $counter],
      WATCHDOG_INFO);
  }
  else {
    drush_print('Empty string-search.');
    watchdog('Dev Helper - Search String', 'Empty search string passed.', [],WATCHDOG_INFO);
  }
}

/**
 * Callback function for hook_drush_command().
 *
 * @param $str
 */
function dev_helper_drush_reset_db($skip = FALSE) {
  // Prompt user to run script or not
  $msg = '- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -' . "\n";
  $msg.= '   R E S E T - D B - S C R I P T ' . "\n";
  $msg.= '- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -' . "\n";
  $msg.= ' - Script to create new db, import and adjust settings for local use.' . "\n";
  $msg.= ' - Make sure you have followed install steps in the Read Me file.' . "\n";
  $msg.= ' - You will be asked before running most cmds like backup/import db.' . "\n\n";
  $msg.= 'Are you sure you want to run reset-db?';
  if (drush_confirm($msg) == 'y') {
    // Get paths
    $sitePath = getcwd();
    $modulePath = drupal_get_path('module', 'dev_helper');
    // Change to resetdb dir
    chdir($sitePath . '/' . $modulePath  . '/' . 'resetdb');
    // Run script and send prompt output to terminal using passthru()
    passthru('./resetdb.sh');
  }
  else {
    // Exit back to terminal prompt
    drush_user_abort();
  }
}
