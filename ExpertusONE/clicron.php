<?php
// Checks that the arguments are supplied, if not display help information.
if (in_array($argv, array('--help')) || $argc != 5) {
  ?>
  This is a script to run cron from the command line.
 
  It takes 2 arguments, which must both be specified:
  --key is the cron key for your website, found on your
  status report page, the part after ?cron_key=?
  --document-root is the path to your drupal root directory for
  your website.
 
  Usage:
  php clicron.php --key YOUR_CRON_KEY --document-root '/path/to/drupal/root'
 
<?php
} else {
  // Loop through arguments and extract the cron key and drupal root path.
  for ($i = 1; $i < $argc; $i++) {
    switch ($argv[$i]) {
      case '--key':
        $i++;
        $key = $argv[$i];
        break;
      case '--document-root':
        $i++;
        $path = $argv[$i];
        break;
    }
  }
 
  chdir($path);
  // Sets script name
  $_SERVER['SCRIPT_NAME'] = $argv[0];
 
  // Values as copied from drupal.sh
  $_SERVER['HTTP_HOST'] = 'default';
  $_SERVER['REMOTE_ADDR'] = '127.0.0.1';
  $_SERVER['SERVER_SOFTWARE'] = NULL;
  $_SERVER['REQUEST_METHOD'] = 'GET';
  $_SERVER['QUERY_STRING'] = '';
  $_SERVER['HTTP_USER_AGENT'] = 'console';
  $_SERVER['DOCUMENT_ROOT'] = $path;
 
  // Set cron key get variable to use below code with as
  // little modification as possible.
  $_GET['cron_key'] = $key;
  define('DRUPAL_ROOT', $path);
 
  // Code below is almost verbatim from cron.php, just the messages for
  // watchdog have been changed to indicate that the problem originated from
  // this script.
  include_once DRUPAL_ROOT . '/includes/bootstrap.inc';
  drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
  
  require_once DRUPAL_ROOT . '/sites/all/services/GlobalUtil.php';
  
  if (!isset($_GET['cron_key']) || variable_get('cron_key', 'drupal') != $_GET['cron_key']) {
    watchdog('cron', "Cron could not run via $argv[0] because an invalid key was used.", array(), WATCHDOG_NOTICE);
    drupal_access_denied();
  }
  elseif (variable_get('maintenance_mode', 0)) {
    watchdog('cron', "Cron could not run via $argv[0] because the site is in maintenance mode.", array(), WATCHDOG_NOTICE);
    drupal_access_denied();
  }
  else {
    drupal_cron_run();
  }
  // Test line
}