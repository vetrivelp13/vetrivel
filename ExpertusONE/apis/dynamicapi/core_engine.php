<?php
/*
 * ExpertusOne API
 * Core Engine v1.0
 * 
 * @author: Rajkumar U
 * @date    :   02-Jan-2012
 * 
 * All global variables, functions related to the Dynamic REST API resides here.
 * 
 * DONOT MODIFY THIS FILE UNTIL IS ABSOLUTELY NECESSARY
 * 
 */

class CoreEngine{
    
    /*
     * global variables
     */

    var $ORMPath = '';
    var $ORMMethod = '';
    var $ORMParams = array();
    var $fullDetails = array();
    var $ini_array = array();
    var $dispCols = array();
    var $testEngineParams=array();
    var $throttle = 0;
    var $htmltags = 0;
    var $privilege_parameter = '';
    var $entity_type_parameter = '';
    
    //constructor
     public function prepareDetals($APIName){
        $this->ini_array = parse_ini_file("rest_api.config", 1);
        if (!array_key_exists($APIName, $this->ini_array)) { // check API exist in config list
            throw new Exception("Apiname is invalid");
        }   
        $this->fix_ini_multi($this->ini_array);
        $this->ORMPath = $this->ini_array[$APIName]['ORM_file'];
        $this->ORMMethod = $this->ini_array[$APIName]['method_name'];
        $this->ORMParams = $this->ini_array[$APIName]['params'];
        $this->fullDetails = $this->ini_array[$APIName];
        $this->dispCols = $this->ini_array[$APIName]['response'];
        $this->testEngineParams=$this->ini_array[$APIName]['testengine'];
        $this->apiCategory=$this->ini_array[$APIName]['category'];
        $this->throttle=isset($this->ini_array[$APIName]['throttle']) ? $this->ini_array[$APIName]['throttle'] : 0;
        $this->htmltags=isset($this->ini_array[$APIName]['htmltags']) ? $this->ini_array[$APIName]['htmltags'] : 0;
        $this->privilege_parameter=isset($this->ini_array[$APIName]['privilege_parameter']) ? $this->ini_array[$APIName]['privilege_parameter'] : '';
        $this->entity_type_parameter=isset($this->ini_array[$APIName]['entity_type_parameter']) ? $this->ini_array[$APIName]['entity_type_parameter'] : '';
    }
    
    private static function fix_ini_multi(&$ini_arr) {
        foreach ($ini_arr AS $key => &$value) {
            if (is_array($value)) {
                self::fix_ini_multi($value);
            }
            if (strpos($key, '.') !== FALSE) {
                $key_arr = explode('.', $key);
                $last_key = array_pop($key_arr);
                $cur_elem = &$ini_arr;
                foreach ($key_arr AS $key_step) {
                    if (!isset($cur_elem[$key_step])) {
                        $cur_elem[$key_step] = array();
                    }
                    $cur_elem = &$cur_elem[$key_step];
                }
                //error_reporting(0);
                //@ini_set('display_errors', 0);
                
                @$cur_elem[$last_key] = $value;
                unset($ini_arr[$key]);
            }
        }
   }
    
    /*
     * getters for variables
     */
   public function getPrivilegeParams(){
    return $this->privilege_parameter;
   }
   
   public function getEntityTypeParams(){
    return $this->entity_type_parameter;
   }
   
    public function getPath(){
        return $this->ORMPath;
    }
    
    public function getMethod(){
        return $this->ORMMethod;
    }
    
    public function getParams(){
        return $this->ORMParams;
    }
    
    public function getFullDetails(){
        return $this->fullDetails;
    }
    
    public function getRespCols(){
        return $this->dispCols;
    }
    public function getTestEngineParams()
    {
            return $this->testEngineParams; 
    }
    public function getFullPath($path){
        return $_SERVER['DOCUMENT_ROOT'].$path;
    }
    
    public function getCatagory(){
        return $this->apiCategory;
    }
    
    public function getThrottle(){
        return $this->throttle;
    }
    
    public function getHtmltags() {
        return $this->htmltags;
    }
    
    public function getDocumentation(){
        $this->ini_array = parse_ini_file("rest_api.config", 1);
        $this->fix_ini_multi($this->ini_array);
        return $this->ini_array;
    }
    
    public function getParameters(){
        $getparams = array();
        $this->constructParams($getparams,$this->ORMParams);
        return $getparams;
    }
    
    function getRequiredParam(){
        $valarry = $this->ORMParams;
        $paramary = array();
        $this->formatRequiredVal($paramary,$valarry);
        return $paramary;
    }
    
    function formatRequiredVal(&$paramArray,$valArray){
        foreach($valArray as $par => $pval){
            if(is_array($pval)){
                $this->formatRequiredVal($paramArray,$pval);
            } else {
                $temparr = explode('>',$pval);
                if(trim($temparr[0]) == 'Y'){
                  $paramArray[$par] = $temparr[0];
                }
            }
        }
        
        $custom_attr_status=getcustomattributemodulestatus(); //#custom_attribute_0078975 
        if($custom_attr_status == true){   //#custom_attribute_0078975 - Check module status
                //Merge Custom Attributes mandatory fields - #custom_attribute_0078975
                if(strtolower($this->ORMMethod) == 'addnewuserbyrestapi' || strtolower($this->ORMMethod) == 'updateuserbyrestapi' 
                        || strtolower($this->ORMMethod) == 'addnewcoursebyrestapi' || strtolower($this->ORMMethod) == 'updatecoursebyrestapi' 
                        || strtolower($this->ORMMethod) == 'addnewclassbyrestapi' || strtolower($this->ORMMethod) == 'updateclassbyrestapi' 
                        || strtolower($this->ORMMethod) == 'addnewtpbyrestapi' || strtolower($this->ORMMethod) == 'updatetpbyrestapi' 
                        || strtolower($this->ORMMethod) == 'addneworganizationbyrest' || strtolower($this->ORMMethod) == 'updateorganizationdetailsbyrest' 
                        || strtolower($this->ORMMethod) == 'addnewlocationbyrestapi' || strtolower($this->ORMMethod) == 'updatelocationbyrestapi' ) {
                            
                    include_once($_SERVER["DOCUMENT_ROOT"].'/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_manage/exp_sp_administration_customattribute/exp_sp_administration_customattribute.inc');
                    
                    if(strtolower($this->ORMMethod) == 'addnewuserbyrestapi' || strtolower($this->ORMMethod) == 'updateuserbyrestapi')
                        $customValues = getCustomAttributeValues('User','mandatory');
                        
                    if(strtolower($this->ORMMethod) == 'addnewcoursebyrestapi' || strtolower($this->ORMMethod) == 'updatecoursebyrestapi')
                        $customValues = getCustomAttributeValues('Course','mandatory');
                    
                    if(strtolower($this->ORMMethod) == 'addnewclassbyrestapi' || strtolower($this->ORMMethod) == 'updateclassbyrestapi')
                        $customValues = getCustomAttributeValues('Class','mandatory');
                    
                    if(strtolower($this->ORMMethod) == 'addnewtpbyrestapi' || strtolower($this->ORMMethod) == 'updatetpbyrestapi')
                        $customValues = getCustomAttributeValues('Training Plan','mandatory');
                    
                    if(strtolower($this->ORMMethod) == 'addneworganizationbyrest' || strtolower($this->ORMMethod) == 'updateorganizationdetailsbyrest')
                        $customValues = getCustomAttributeValues('Organization','mandatory');
                    
                    if(strtolower($this->ORMMethod) == 'addnewlocationbyrestapi' || strtolower($this->ORMMethod) == 'updatelocationbyrestapi')
                        $customValues = getCustomAttributeValues('Location','mandatory');
                    
                    if(count($customValues)>0){
                        foreach($customValues as $cuskey=>$cusvalue) {
                            if($cusvalue->attributmandatory == 1)
                             $paramArray[$cusvalue->attributecode]= 'Y';
                         }
                             expDebug::dPrint('api error handling result- '.print_r($paramArray,true), 5);
                    }
                }
        } //#custom_attribute_0078975 - End Check module status
    }
    
    function checkFillRequiredParam($requiredParams,$paramarraytemp){
      $neededParam = array();    
       $custom_attr_module_exists = getcustomattributemodulestatus(); //#custom_attribute_0078975 
      foreach ($requiredParams AS $key => $value) {  
        if(trim($paramarraytemp[$key]) == ''){
            //Replace Custom Attributes Name instead of Code to mandatory fields error display - #custom_attribute_0078975 
            if(strpos($key, 'c_')===0 && $custom_attr_module_exists) 
            {
                $neededParam1 = db_select('slt_custom_attr', 'cattr');
                $neededParam1->addField('cattr','cattr_name','attributename');              
                $neededParam1->condition('cattr_code', $key,'=');
                expDebug::dPrintDBAPI('Custom Attribute Query -->',$neededParam1);
                $result_customattr = $neededParam1->execute()->fetchField();
                expDebug::dPrint('$result_customattr ' . print_r($result_customattr, true) , 4);
                $neededParam[] = $result_customattr;                 
            }
            else
                $neededParam[] = $key;
        }
      }
      return $neededParam;
    }
    
    function constructParams(&$paramAry, $sourceAry){
        foreach($sourceAry as $key => $val){
            if(is_array($val)){
                $this->constructParams($paramAry,$val);
            } else {
                if(isset($_GET[$key])){
                    $paramAry[$key] = $_GET[$key];
                } else if(isset($_POST[$key])) {
                    $paramAry[$key] = $_POST[$key];
                }else {
                    $paramAry[$key] = '';
                }
            }
        }
        
        $custom_attr_status=getcustomattributemodulestatus(); //#custom_attribute_0078975  
        if($custom_attr_status == true){   //#custom_attribute_0078975 - Check module status
                //Passing the Custom Attributes Parameter Values #custom_attribute_0078975
                if(strtolower($this->ORMMethod) == 'addnewuserbyrestapi' || strtolower($this->ORMMethod) == 'updateuserbyrestapi' 
                        || strtolower($this->ORMMethod) == 'listuserbyrestapi' || strtolower($this->ORMMethod) == 'addnewcoursebyrestapi' 
                        || strtolower($this->ORMMethod) == 'updatecoursebyrestapi' || strtolower($this->ORMMethod) == 'listcatalogbyrestapi' 
                        || strtolower($this->ORMMethod) == 'addnewclassbyrestapi' || strtolower($this->ORMMethod) == 'updateclassbyrestapi' 
                        || strtolower($this->ORMMethod) == 'listclassbyrestapi' || strtolower($this->ORMMethod) == 'addnewtpbyrestapi' 
                        || strtolower($this->ORMMethod) == 'updatetpbyrestapi' || strtolower($this->ORMMethod) == 'listtpbyrestapi' 
                        || strtolower($this->ORMMethod) == 'addneworganizationbyrest' || strtolower($this->ORMMethod) == 'updateorganizationdetailsbyrest' 
                        || strtolower($this->ORMMethod) == 'listorganizationbyrestapi' || strtolower($this->ORMMethod) == 'addnewlocationbyrestapi' 
                        || strtolower($this->ORMMethod) == 'updatelocationbyrestapi') {
                    include_once($_SERVER["DOCUMENT_ROOT"].'/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_manage/exp_sp_administration_customattribute/exp_sp_administration_customattribute.inc');
                    
                    if(strtolower($this->ORMMethod) == 'addnewuserbyrestapi' || strtolower($this->ORMMethod) == 'updateuserbyrestapi' || strtolower($this->ORMMethod) == 'listuserbyrestapi')
                        $customValues = getCustomAttributeValues('User','mandatory');
                    
                    if(strtolower($this->ORMMethod) == 'addnewcoursebyrestapi' || strtolower($this->ORMMethod) == 'updatecoursebyrestapi' || strtolower($this->ORMMethod) == 'listcatalogbyrestapi')
                        $customValues = getCustomAttributeValues('Course','mandatory');
                    
                    if(strtolower($this->ORMMethod) == 'addnewclassbyrestapi' || strtolower($this->ORMMethod) == 'updateclassbyrestapi' || strtolower($this->ORMMethod) == 'listclassbyrestapi')
                        $customValues = getCustomAttributeValues('Class','mandatory');
                    
                    if(strtolower($this->ORMMethod) == 'addnewtpbyrestapi' || strtolower($this->ORMMethod) == 'updatetpbyrestapi' || strtolower($this->ORMMethod) == 'listtpbyrestapi')
                        $customValues = getCustomAttributeValues('Training Plan','mandatory');
                    
                    if(strtolower($this->ORMMethod) == 'addneworganizationbyrest' || strtolower($this->ORMMethod) == 'updateorganizationdetailsbyrest' || strtolower($this->ORMMethod) == 'listorganizationbyrestapi')
                        $customValues = getCustomAttributeValues('Organization','mandatory');
                    
                    if(strtolower($this->ORMMethod) == 'addnewlocationbyrestapi' || strtolower($this->ORMMethod) == 'updatelocationbyrestapi')
                        $customValues = getCustomAttributeValues('Location','mandatory');
                    
                      if(count($customValues)>0){
                        foreach($customValues as $cuskey=>$cusvalue) {
                            $paramAry[$cusvalue->attributecode]= $_POST[$cusvalue->attributecode];
                         }
                      }
                }
        } //#custom_attribute_0078975 - End Check module status
    }
    
}
?>