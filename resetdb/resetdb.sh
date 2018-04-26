#!/bin/bash

# get project root from drush status, split string by :, trim whitespaces
siteRoot="$(drush status | grep 'Drupal root' | cut -d':' -f2 | tr -d '[:space:]')"
# get path of current script
scriptDir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
# get unix style timestamp
nowTime=$(date +%s)

# Disable Module function
# use for all modules that can easily be disabled from drush
# see if modules exist and enabled first before asking
# @params module, text
function drushDis {
	drush pm-info $1 | grep -o 'enabled' &> /dev/null
	if [ $? == 0 ]; then
		echo "- $2 (y/n): ";
		read moduleAction
		if [ "$moduleAction" == 'y' ]; then
			drush dis $1
		fi
	fi
}

# Run drush command
# @params drush-command, text
function drushCmd {
	echo "- $2 (y/n): ";
	read runCmd
	if [ "$runCmd" == 'y' ]; then
		drush $1
	fi
}

echo 'Enter Drupal site name folder where Drush can run, or "default": ';
read siteName

# move into site dir for drush cmds to run
cd $siteRoot/sites/$siteName

# Backup current db
echo 'Backup current db? (y/n): ';
read backupDb
if [ "$backupDb" == 'y' ]; then
  echo 'Add short text to sql.gz filename? (could be git branch related like version-##): ';
  read dbNote
  echo 'Backing up current db to resetdb/backups.'
  drush sql-dump --gzip --result-file=$scriptDir/backups/$siteName-$dbNote-$nowTime.sql
fi

# Import a previous backup? (or use the default one in 'else' step)
echo 'Switch to previous backup db over current database? OVERWRITES CURRENT DATA. (y/n): ';
read switchDb
if [ "$switchDb" == 'y' ]; then
  # Show list of backup dbs to user
  ls $scriptDir/backups
  echo ' ';
  echo 'Copy/paste the db.gz from file list above that you want to import.';
  read dbToImport
  # Check that the file is there and drop/import
	if [ ! -f $scriptDir/backups/$dbToImport ]; then
		echo 'Database file in backups dir does not exist. Maybe you copy/pasted wrong?';
		exit;
	fi
	echo 'Clearing out current db and importing selected backup.';
	# Clear out current db tables, suppress output
	drush sql-drop --yes &> /dev/null
	# Import sql.gz
	gunzip < $scriptDir/backups/$dbToImport | drush sqlc
	echo 'DB was imported';
else
  # Import the
  echo 'Import db gzip over current database? OVERWRITES CURRENT DATA. (y/n): ';
  read importDb
  if [ "$importDb" == 'y' ]; then
    if [ ! -f $scriptDir/sites/$siteName.sql.gz ]; then
      echo 'Database file does not exist. Please add file to resetdb/sites/*your-site*.sql.gz and run this again.';
      exit;
    fi
    # Clear out current db tables, suppress output
    drush sql-drop --yes &> /dev/null
    # Import sql.gz
    gunzip < $scriptDir/sites/$siteName.sql.gz | drush sqlc
    echo 'DB was imported';
  fi
fi

# Run custom site cmds if exist
if [ ! -f $scriptDir/sites/$siteName.sh ]; then
	echo 'No custom sitename script. Please review README.md for additional setup steps.';
	exit;
else
	echo 'Run custom commands, one at at time.';
	source $scriptDir/sites/$siteName.sh
fi

echo 'All done!';

cd $siteRoot/sites/$siteName
