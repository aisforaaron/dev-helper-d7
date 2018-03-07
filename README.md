# Dev Helper Module

ONLY USE THIS ON LOCAL DEVELOPMENT MACHINES!!!

__You have been warned.__


## Goals

To have a reference place for storing, testing of common Drupal 7 functionality used while developing sites. Custom Drush commands are also defined for debug and reporting.


## Current Contents

* Admin pages can be found here:
  * Configuration > Development > Dev Helper
    * Settings tab, placeholder for future work.
    * Paragraphs Report, will load all nodes that have paragraph fields and create an array of nid => counts, and store this in the variables table.

* Drush command for searching rendered html output:
  ```
  $ drush string-search --help
  ```

* Drush command for importing a backup sql.gz:
  ```
  $ drush reset-db --help
  ```

## Reset DB 

Requirements
- Drush has to be installed first. Script tested with v8.1.9
- MySQL with user/pass root/root accessible from where you run this script

Installation
1. You will need to add a database export to this resetdb dir
  ** the .sql.gz file should be named the same as your Drupal site (See docroot/sites/*site-name*)
1. You need to add #!!dbname after the database name in your settings.local.php like so:
  "database"=>"drupal_dev", #!!dbname\n'
1. You may want to extend the app with more cmds specific to each site,
  **  Create a *site-name*.sh in resetdb dir 
1. Run this from drush  
  ```$ drush reset-db```

Files/Folders
* resetdb/backups will store db dumps of your site before the import runs
* resetdb/sites will store custom sh scripts and sql.gz file that reset-db cmd will import

General reset-db command workflow
1. drush cmd runs resetdb.sh, which calls your custom *site-name*.sh file
1. create new mysql db for a Drupal 7 site using root u/p
1. use SED cmd to update settings.local.php with new db name
1. import sql file from sites dir 
1. prompt user in terminal to run all cmds (updb, rr, fra, en/dis modules...)
1. run drush commands for local dev setup (this does not delete any code or current db)
