<?php
// Checks that the arguments are supplied, if not display help information.
if (in_array($argv, array('--help')) || $argc != 3) {
  ?>
  This is a script for reparsing all saved report queries
  in database table slt_report_query_bulder, column query_builder_org_sql.
 
  It takes 1 argument:
  --document-root is the path to your drupal root directory for
  your website.
 
  Usage:
  php reparse_reports_sql.php --document-root '/path/to/drupal/root'
 
<?php
} else {
	// Code taken from clicron.php and modified
  // Loop through arguments and extract the drupal root path.
  for ($i = 1; $i < $argc; $i++) {
    switch ($argv[$i]) {
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
  
  define('DRUPAL_ROOT', $path);
 
  // Code below is almost verbatim from cron.php, just the messages for
  // watchdog have been changed to indicate that the problem originated from
  // this script.
  include_once DRUPAL_ROOT . '/includes/bootstrap.inc';
  drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
  
  require_once DRUPAL_ROOT . '/sites/all/services/GlobalUtil.php';
  
  drupal_set_time_limit(0); // let it finish

  // Force the current user to anonymous as is done for cron
  $original_user = $GLOBALS['user'];
  expDebug::dPrint('$original_user = ' . print_r($original_user, true), 4);
  $GLOBALS['user'] = drupal_anonymous_user();
    
  require_once(drupal_get_path('module', 'exp_sp_lnrreports') . '/exp_sp_lnrreports.inc');
  
  $selStmt = db_select('slt_report_details', 'details');
  $selStmt->leftJoin('slt_report_query_builder', 'builder', 'details.id = builder.report_details_id AND (builder.union_query = \'0\' OR builder.union_query = \'1\')');
  $selStmt->addField('details', 'id', 'id');
  $selStmt->addField('details', 'title', 'title');
  $selStmt->addField('builder', 'query_builder_org_sql', 'org_sql');
  $selStmt->condition('details.report_builder_type', 'query_builder', '=');
  $selStmt->condition('details.status', 'cre_rpt_rps_del', '!=');
  expDebug::dPrintDBAPI('$selStmt', $selStmt);
  
  $result = $selStmt->execute();
  
  foreach ($result as $dataRow) {
  	echo $dataRow->id . '. ' . $dataRow->title . "\n";
  	$_REQUEST['qryval'] = $dataRow->org_sql;
  	setQueryBuilder($dataRow->id);
  	echo "\n";
  }
}
?>