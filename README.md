## Dev Helper Module

ONLY USE THIS ON LOCAL DEVELOPMENT MACHINES!!!

__Backup your files/db first. You have been warned.__


### Goals

To have a reference place for storing, testing of common Drupal 7 functionality used while developing sites. Custom Drush commands are also defined for debug and reporting.


### Current Contents

* Admin pages can be found here:
  * Configuration > Development > Dev Helper
    * Settings tab, placeholder for future work.
    * Paragraphs Report, will load all nodes that have paragraph fields and create an array of nid => counts, and store this in the variables table.

* Drush command for searching rendered html output:
  ```
  $ drush string-search --help
  ```

* Drush command for importing a backup sql.gz (This overwrites your current db!):
  ```
  $ drush reset-db --help
  ```

### Bugs

* Yeah, I'm sure they are in there. Coder beware, proceed with caution, backup your db, wash your hands.
* The module likes to set max_execution_time to forever, so you know how that goes.

