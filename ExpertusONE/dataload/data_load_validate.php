<?php 


abstract class dataloadValidate
{
 abstract public function extenderValidate($jobDetail=array(),$batchrecords=array());
 abstract public function validate($jobDetail=array(),$batchrecords=array());
 abstract public function bulkValidate($jobDetail=array(),$batchrecords=array());
 abstract public function execute($updVal);
 abstract public function bulkExecute($tblDet=array());
 
}

?>