<?php
/*
 * ExpertusOne API
* Simulation Engine v1.0
*
* @author: Rajkumar U
* @date	:	02-Jan-2012
*
* All global variables, functions related to the Dynamic REST API resides here.
*
* DONOT MODIFY THIS FILE UNTIL IS ABSOLUTELY NECESSARY
*
*/

define('DRUPAL_ROOT', $_SERVER["DOCUMENT_ROOT"]);
include_once($_SERVER["DOCUMENT_ROOT"].'/apis/dynamicapi/includes.php');
include_once($_SERVER["DOCUMENT_ROOT"].'/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_sitesetup/exp_sp_administration_module_info/exp_sp_administration_module_info.inc');
require_once($_SERVER["DOCUMENT_ROOT"].'/includes/bootstrap.inc');
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

$custom_attr_status = getcustomattributemodulestatus();
expDebug::dPrint('$ret_tmp '.print_r($custom_attr_status,true), 5);

if($custom_attr_status == true){   //#custom_attribute_0078975 - Check module status
 include_once($_SERVER["DOCUMENT_ROOT"].'/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_manage/exp_sp_administration_customattribute/exp_sp_administration_customattribute.inc'); //#custom_attribute_0078975
} //#custom_attribute_0078975 - End Check module status

class SimulationEngine{
	var $CE = null;
	function __construct(){
		$this->CE = new CoreEngine();
	}
	function SimAPIList(){
		$apiList = $this->CE->getDocumentation();
		$string = '<option value="">Select</option>';
		$adminapi = '';
		$instructorapi = '';
		$learnerapi = '';
		$managerapi = '';		
		include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_sitesetup/exp_sp_administration_module_info/exp_sp_administration_module_info_share.inc";
        $custom_attr_status = getcustomattributemodulestatus();
		foreach($apiList as $key => $val){
			//STARTED 
			if($val['method_name'] == 'shareClassAPI'){				
				$val['customer_display'] = getShareModuleStatus('api');				
			}
            if($val['method_name'] == 'addCustomAttributeAPI' || $val['method_name'] == 'updateCustomAttributeAPI' || $val['method_name'] == 'listCustomAttributeAPI'){
                $val['customer_display'] = $custom_attr_status;             
            }
            if($val['method_name'] == 'createCustomFieldByRestAPI' || $val['method_name'] == 'updateCustomFieldByRestAPI' || $val['method_name'] == 'listCustomFieldApi' || $val['method_name'] == 'deleteCustomFieldByRestAPI'){
                
                if($custom_attr_status == true)
                $val['customer_display'] = false;
                else
                $val['customer_display'] = true;
            }
			//ENDED*/
			if($key != 'documentation'  && trim($val['customer_display']) == true){				
				// categories the option into seperate variable
				if(strtolower(trim($val['category'])) == 'adminapi'){
					$adminapi .= '<option value="'.$key.'">'.rawurldecode($val['display_name']).'</option>';
				}else if(strtolower(trim($val['category'])) == 'instructorapi'){
					$instructorapi .= '<option value="'.$key.'">'.rawurldecode($val['display_name']).'</option>';
				}else if(strtolower(trim($val['category'])) == 'learnerapi'){
					$learnerapi .= '<option value="'.$key.'">'.rawurldecode($val['display_name']).'</option>';
				}else if(strtolower(trim($val['category'])) == 'managerapi'){
					$managerapi .= '<option value="'.$key.'">'.rawurldecode($val['display_name']).'</option>';
				}
			}
		}
		// Splitted categories the option as Moved inside the Option Group
		// Check For Not Empty
		$adminapiGroup =''; $instructorapiGroup =''; $learnerapiGroup =''; $managerapiGroup ='';
		if(!empty($adminapi)){
			$adminapiGroup = '<optgroup label="AdminAPI">'.$adminapi.'</optgroup>';
		}
		if(!empty($instructorapi)){
			$instructorapiGroup = '<optgroup label="InstructorAPI">'.$instructorapi.'</optgroup>';
		}
		if(!empty($learnerapi)){
			$learnerapiGroup = '<optgroup label="LearnerAPI">'.$learnerapi.'</optgroup>';
		}
		if(!empty($managerapi)){
			$managerapiGroup = '<optgroup label="ManagerAPI">'.$managerapi.'</optgroup>';
		}
		// Merging the Option Group
		$string .= $adminapiGroup.''.$instructorapiGroup.''.$learnerapiGroup.''.$managerapiGroup;
		return $string;
	}
	function testAPIList(){
		$apiList = $this->CE->getDocumentation();
		$string = array();
		foreach($apiList as $key => $val){
			if($key != 'documentation'){
				if($val['test_category'] == $_GET['testfor']){
					$string[] = $key;
				}
			}
		}
		return $string;
	}

	function testAPIListFull(){
		$apiList = $this->CE->getDocumentation();
		$string = array();
		$add = array();
		$list = array();
		$update = array();
		foreach($apiList as $key => $val){
			if($key != 'documentation'){
				if($val['test_category'] == 'add'){
					$add[] = $key;
				} else if($val['test_category'] == 'list'){
					$list[] = $key;
				} else if($val['test_category'] == 'update'){
					$update[] = $key;
				}
			}
		}
		foreach($add as $key => $val){
			$string[] = $val;
		}
		foreach($list as $key => $val){
			$string[] = $val;
		}
		foreach($update as $key => $val){
			$string[] = $val;
		}
		return $string;
	}

	function getDomainDoc()
	{
		/*** get the url parts ***/
		
		$url = (!empty($_SERVER['HTTPS'])) ? "https://".$_SERVER['SERVER_NAME'].$_SERVER['REQUEST_URI'] : "http://".$_SERVER['SERVER_NAME'].$_SERVER['REQUEST_URI'];
		$parts = parse_url($url);
		/*** return the host domain ***/
		return $parts['scheme'].'://'.$parts['host'];
	}

	function getTestVal(){
		$valarry = $this->CE->getParams();
		$paramary = array();
		$this->formatTestVal($paramary,$valarry);
		return $paramary;
	}

	function formatTestVal(&$paramArray,$valArray){
		foreach($valArray as $par => $pval){
			if(is_array($pval)){
				$this->formatTestVal($paramArray,$pval);
			} else {
				$temparr = explode('>',$pval);
				$paramArray[$par] = $temparr[2];
			}
		}
	}
	function stringAccumulator(&$string,$array,$parm){
		foreach($array as $key => $val){
			if(is_array($val)){
				$this->stringAccumulator($string,$val,$parm);
			} else {
				$string .= '<tr><td><label>'.$key.':</label></td><td>';
				$string .= '<input type="text" name="'.$key.'" value="'.$parm[$key].'"></td></tr>';
			}
		}
	}

	function rewriteParams(&$paramArray,$srcArray,$buildinfo){
		foreach($srcArray as $key => $val){
			if(is_array($val)){
				$this->rewriteParams($paramArray,$val,$buildinfo);
			} else {
				$paramArray[$key] = str_replace("VERSION",$buildinfo,$val);
			}
		}
	}

	function getVersion(){
		$version = '';
		$file = fopen($_SERVER["DOCUMENT_ROOT"].'/VERSION.txt', "r") or exit("Unable to open file!");
		while(!feof($file))
		{
			$version .= fgets($file);
		}
		$versionArray = explode(" ",$version);
		return $versionArray[0];
	}

	function SimParamForm($apiname){
		$this->CE->prepareDetals($apiname);
		$dispparams = $this->CE->getRespCols();
		$disp = '';
		$i = 1;
		foreach($dispparams as $key => $val){
			$disp .= $key;
			if($i < count($dispparams)){
				$disp .= ",";
				$i++;
			}
		}
		$parm = array();
		$parm1 = $this->getTestVal();
		$this->rewriteParams($parm,$parm1,$this->getVersion());
		$apiparams = $this->CE->getParams();
		$string = '<form name="'.$apiname.'form" id="simulator_form">';
		$string .= '<table>';
		$this->stringAccumulator($string,$apiparams,$parm);
        $custom_attr_status = getcustomattributemodulestatus();
        
        expDebug::dPrint('$ret_tmp 1111'.print_r($custom_attr_status,true), 5);
        
        //Start #custom_attribute_0078975
        if($custom_attr_status == true){   //#custom_attribute_0078975 - Check module status
                if(strtolower($apiname) == 'usercreationapi' || strtolower($apiname) == 'userupdationapi' || strtolower($apiname) == 'listuserapi' 
                        || strtolower($apiname) == 'createcourseapi' || strtolower($apiname) == 'updatecourseapi' 
                        || strtolower($apiname) == 'listcoursesapi' || strtolower($apiname) == 'createclassapi' || strtolower($apiname) == 'updateclassapi' 
                        || strtolower($apiname) == 'listclassesapi' || strtolower($apiname) == 'createtpapi' || strtolower($apiname) == 'updatetpapi' 
                        || strtolower($apiname) == 'listtpapi' || strtolower($apiname) == 'addorganizationapi' || strtolower($apiname) == 'updateorganizationapi' 
                        || strtolower($apiname) == 'listorganizationapi' || strtolower($apiname) == 'createlocationapi' || strtolower($apiname) == 'updatelocationapi' 
                        || strtolower($apiname) == 'coursesdetailsapi') {
                    
                    if(strtolower($apiname) == 'usercreationapi' || strtolower($apiname) == 'userupdationapi' || strtolower($apiname) == 'listuserapi')
                        $customValues = getCustomAttributeValues('User','mandatory');
                    
                    if(strtolower($apiname) == 'createcourseapi' || strtolower($apiname) == 'updatecourseapi' || strtolower($apiname) == 'listcoursesapi')
                        $customValues = getCustomAttributeValues('Course','mandatory');
                    
                    if(strtolower($apiname) == 'createclassapi' || strtolower($apiname) == 'updateclassapi' || strtolower($apiname) == 'listclassesapi')
                        $customValues = getCustomAttributeValues('Class','mandatory');
                    
                    if(strtolower($apiname) == 'createtpapi' || strtolower($apiname) == 'updatetpapi' || strtolower($apiname) == 'listtpapi')
                        $customValues = getCustomAttributeValues('Training Plan','mandatory');
                    
                    if(strtolower($apiname) == 'addorganizationapi' || strtolower($apiname) == 'updateorganizationapi' || strtolower($apiname) == 'listorganizationapi')
                        $customValues = getCustomAttributeValues('Organization','mandatory');
                    
                    if(strtolower($apiname) == 'createlocationapi' || strtolower($apiname) == 'updatelocationapi')
                        $customValues = getCustomAttributeValues('Location','mandatory');
                    
                    if(strtolower($apiname) == 'coursesdetailsapi')
                        $customValues = getCustomAttributeValues('Class','mandatory');
                        
                        if(count($customValues)>0){
                            //if(strtolower($apiname) == 'usercreationapi' || strtolower($apiname) == 'userupdationapi'){
                                foreach($customValues as $cuskey=>$cusvalue) {
                                    if(!empty($cusvalue))
                                        $string .= '<tr><td><label>'.$cusvalue->attributename.':</label></td><td><input type="text" name="'.$cusvalue->attributecode.'" value=""></td></tr>';
                                }
                            //}
                            if(strtolower($apiname) == 'listuserapi' || strtolower($apiname) == 'listcoursesapi' || strtolower($apiname) == 'listclassesapi' 
                                    || strtolower($apiname) == 'listtpapi' || strtolower($apiname) == 'listorganizationapi' || strtolower($apiname) == 'coursesdetailsapi') {
                                foreach($customValues as $cuskey=>$cusvalue) {
                                    $attribute_values[] = strtolower(str_replace(' ','_',$cusvalue->attributename));
                                }
                                    expDebug::dPrint('api custom display columns - '.print_r($attribute_values,true), 5);
                                $attribute_values = implode(',',$attribute_values);
                                    expDebug::dPrint('api custom display columns after convert - '.print_r($attribute_values,true), 5);
                                $disp = $disp.','.$attribute_values;
                            }
                    }     
                            
                } //End #custom_attribute_0078975
        } //#custom_attribute_0078975 - End Check module status
        
		$string .='<tr><td><label>Display Columns : </label></td><td><input type="text" name="display_cols" value="'.$disp.'"></td></tr><tr><td><label>Return Type : </label></td><td><select name="returntype"><option value="xml">XML</option><option value="json">JSON</option></select></td></tr><tr><td></td></tr><tr><td><input type="hidden" name="apiurl" value="'.$this->getDomainDoc().'/apis/ext/ExpertusOneAPI.php"></td></tr>';
		$string .='<tr><td><input type="hidden" name="apiname" value="'.$apiname.'"></td></tr><tr><td colspan=2 style="text-align:right;"><input id ="execute" type="button" value="Execute" onclick="getData(\''.$apiname.'\')"></td></tr>';
		$string .='</table></form>';
		return $string;
	}
}
$SE = new SimulationEngine();
$actionkey = $_GET['actionkey'];
$apin = '';

if($actionkey == 'getParamForm'){
	$apin = $_GET['apiname'];
	if($apin == ''){
		echo "Please Select an API.";
	}else{
		echo $SE->SimParamForm($apin);
	}
} else if($actionkey == 'testapiname'){
	echo json_encode($SE->testAPIList());
}else if($actionkey == 'testapifull'){
	echo json_encode($SE->testAPIListFull());
}else {
	echo $SE->SimAPIList();
}
?>