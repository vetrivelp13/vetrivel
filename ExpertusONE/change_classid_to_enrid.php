 <?php 
define('DRUPAL_ROOT', getcwd()); 
include DRUPAL_ROOT ."/sites/all/services/Trace.php";
require_once DRUPAL_ROOT .'/includes/bootstrap.inc';
include_once DRUPAL_ROOT . "/includes/common.inc";
include_once DRUPAL_ROOT . "/includes/database/database.inc";
include_once DRUPAL_ROOT . "/includes/bootstrap.inc";
include_once DRUPAL_ROOT . "/sites/all/services/GlobalUtil.php";
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
// drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
expDebug::dPrint('sample db return values');

$spquery = db_query("call sp_user_points_dup_delete()");

$result = db_query('select id,user_id,
         entity_id,
         count(*)
from     slt_user_points 
where entity_id!=0 and entity_type = "class"
group by user_id,
         entity_id,action_code
having   count(*) > 1;')->fetchAll();

expDebug::dPrint('sample db return values'.print_r($result,true),4);

foreach($result as $value){
	expDebug::dPrint('sample db return values'.print_r($value->user_id,true),4);
	expDebug::dPrint('sample db return values ent 123'.print_r($value->entity_id,true),4);
	//$enrollmentdet = db_query("select id,class_id,user_id from slt_enrollment where user_id = '.$value->user_id.' and class_id = '.$value->entity_id.'")->fetchAll();
	//expDebug::dPrint('sdet---->>>'.print_r($enrollmentdet,true),4);
	$select = db_select('slt_enrollment','enr');
	$select->addField('enr', 'id', 'id');
	$select->addField('enr', 'class_id', 'class_id');
	$select->addField('enr', 'user_id', 'user_id');
	$select->condition('enr.user_id', $value->user_id);
	$select->condition('enr.class_id', $value->entity_id);
	$enrdet = $select->execute()->fetchAll();
	expDebug::dPrintDBAPI(' SurAssAttachQuestion Qry1 Velu = ', $select);
	expDebug::dPrint('sdet---->>>'.print_r($enrdet,true),4);
	
	foreach($enrdet as $det){
		expDebug::dPrint('sdetnewssss---->>>'.print_r($det,true),4);
		expDebug::dPrint('sdetnewssss-ssss--->>>'.print_r($det->class_id,true),4);
		expDebug::dPrint('sdetnewssss-sseee--->>>'.print_r($det->user_id,true),4);
		//expDebug::dPrint('sdetnewssss111---->>>'.print_r($det['user_id'],true),4);
		
		$select = db_select('slt_user_points','usp');		
		$select->addField('usp', 'id', 'id');
		$select->addField('usp', 'action_code', 'action_code');
		$select->condition('usp.user_id', $det->user_id);
		$select->condition('usp.entity_id', $det->class_id);
		$select->where('usp.custom4 IS  NULL');
		$select->range(0,1);
		$uspid = $select->execute()->fetchAll();
		$sel_id = $uspid[0]->id;
		$action_code = $uspid[0]->action_code;
		expDebug::dPrintDBAPI(' asasasasas ', $select);
		//$sel = db_query("select id from slt_user_points_asd where user_id = $det->user_id and entity_id = $det->class_id where custom1 IS NULL limit 1")->fetchField();
		expDebug::dPrint('sdetsss---->>>'.print_r($uspid,true),4);
		//echo "update slt_user_points set entity_id = $det->id, custom4 = '70712' where user_id=$det->user_id  AND id = $sel_id and action_code =$action_code";
		db_query("update slt_user_points set entity_id = $det->id, custom4 = '70712' where user_id=$det->user_id  AND id = $sel_id and action_code ="."'".$action_code."'");
		
	}
}
$spquery = db_query("call sp_user_points_update()");
expDebug::dPrint('End of the cleanup script',4);
sleep(10);
$spDropquery = db_query("DROP PROCEDURE sp_user_points_dup_delete");
$spDropquery = db_query("DROP PROCEDURE sp_user_points_update");
?> 
