<?php include 'header.php'; ?>

<?php
/*
 * ExpertusOne API
 * Documentation Engine.php v1.0
 *
 * @author: Rajkumar U
 * @date    :   02-Jan-2012
 *
 * All documentation related to the Dynamic REST API is generated here.
 *
 * DONOT MODIFY THIS FILE UNTIL IS ABSOLUTELY NECESSARY
 */
include_once($_SERVER["DOCUMENT_ROOT"].'/apis/dynamicapi/includes.php');

$ret_tmp = getcustomattributemodulestatus();
if($ret_tmp == true){   //#custom_attribute_0078975 - Check module status
    include_once($_SERVER["DOCUMENT_ROOT"].'/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_manage/exp_sp_administration_customattribute/exp_sp_administration_customattribute.inc'); //#custom_attribute_0078975
} //#custom_attribute_0078975 - End Check module status


class DocuEngine{
    var $CE = null;
    var $RE = null;
    var $EE = null;
    var $doc;
    function __construct(){
        $this->CE = new CoreEngine();
        $this->RE = new RewriteEngine();
        $this->EE = new ExecutionEngine();
        $this->doc = $this->CE->getDocumentation();
    }

    function getResultSet($APIName){
        $this->CE->prepareDetals($APIName);
        $fulldet = $this->CE->getFullDetails();
        return $this->constructResultArray($fulldet['response']);
    }

    function getTestVal(){
        $valarry = $this->CE->getParams();
        $paramary = array();
        $this->formatTestVal($paramary,$valarry);
        return $paramary;
    }
    
    function constructResultArray($fulldet){
        $paramdet = array();
        $this->getParams($paramdet,$fulldet);
        $paramarray = array();
        foreach($paramdet as $key => $val){
            $paramarray[$key] = isset($val[1]) ? $val[1] : '';
        }
        return $paramarray;
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

    function generateXMLData($resultSet){
        print $this->RE->constructFullXMLDoc($resultSet);
    }

    function constructJSON($jsonstring){
        print $this->RE->getJSONData($jsonstring);
    }
    
    function getParams(&$params,$val){
        foreach($val as $par => $pval){
            if(is_array($pval)){
                $this->getParams($params,$pval);
            } else {
                $params[$par] = explode('>',$pval);
            }
        }
    }
}
function parseResponse($result) {
    $return = '<ul>';
    foreach ($result as $key=>$value) {
        if(is_array($value)) {
            $return .= "<li>".$key.parseResponse($value)."</li>";
        }
        else {
            $description = explode('>', $value);
            $return .= '<li>'.$key.' - '.urldecode($description[0]).'</li>';
        }
    }
    return $return.'</ul>';
}
function array2xml ($array_item) {
    $xml = '';
    foreach($array_item as $element => $value)
    {
        if (is_array($value))
        {
            $xml .= "\n\t<$element>".array2xml($value)."</$element>";
        }
        elseif($value == '')
        {
            $xml .= "\n\t<$element />";
        }
        else
        {
            $description = explode('>', $value);
            $xml .= "\n\t<$element>".isset($description[1]) ? htmlentities(trim($description[1])) : ''."</$element>";
        }
    }

    return $xml;
}
function constructJSONResponseResultSet (&$resultSet) {
    $xml = '';
    foreach ( $resultSet as $element => &$value ) {
        if (is_array ( $value )) {
            constructJSONResponseResultSet ( $value );
        } else {
            $description = explode ( '>', $value );
            $value = isset($description [1]) ? urldecode($description[1]) : '';
        }
    }

    return $xml;
}

function generateXMLResponseData($resultSet){
    $xmlResponseString = '<?xml version="1.0" encoding="UTF-8" ?><results>'.array2xml($resultSet).'</results>';
    $dom = new DOMDocument;
    $dom->preserveWhiteSpace = FALSE;
    $dom->loadXML($xmlResponseString);
    $dom->formatOutput = TRUE;
    return htmlspecialchars($dom->saveXml());
}
function generateJSONResponseData($resultSet){
    $DE = new DocuEngine();
    constructJSONResponseResultSet($resultSet);
    return $DE->RE->getJSONData(json_encode($resultSet));
}
$DE = new DocuEngine();
?>

<div id="content-outer">
    <div id="breadcrumbs">
        <div class="breadcrumb">
            <a href="/">Home</a> &rarr; <a href="#">Documentation</a>
            <div>
                <a href="#" data-count="none" data-related="" class="share-button">Tweet</a>
            </div>
        </div>
    </div>
    <h1 id="title">The REST API</h1>
    <div id="content-inner">

        <div id="content-main">
            <!-- <div class="doc-updated">Updated on <?php // print $DE->doc['documentation']['last_updated'];?></div> -->
        </div>
        <div>

    <?php
    $calculateApi = 0;
//  print_r($DE->doc);exit; 
    $custom_attr_status=getcustomattributemodulestatus(); //#custom_attribute_0078975
    
    foreach($DE->doc as $key => $val){

        //#custom_attribute_0078975 
        if($val['method_name'] == 'addCustomAttributeAPI' || $val['method_name'] == 'updateCustomAttributeAPI' || $val['method_name'] == 'listCustomAttributeAPI'){ 
           $val['customer_display'] = $custom_attr_status; 
        }
        if($val['method_name'] == 'createCustomFieldByRestAPI' || $val['method_name'] == 'updateCustomFieldByRestAPI' || $val['method_name'] == 'listCustomFieldApi' || $val['method_name'] == 'deleteCustomFieldByRestAPI'){
            if($custom_attr_status == true)
                $val['customer_display'] = false;
            else
                $val['customer_display'] = true;
        }
        
        if($key != 'documentation'){
                if( strtolower(trim($val['category']))=="adminapi" && trim($val['customer_display']) == true) {
            ?>
            <?php if($calculateApi == 0){ ?>
            <div class="toc" id="toc">
                <div class="toc-title"></div>
                <div class="toc-list">
                    <ol>
                        <li class="toc-level-1"><a href="#otherres">List of APIs for Admin
                                functionality</a>
                            <ol>
                                <?php } ?>
                                <li class="toc-level-2"><a
                                    href="#<?php print strtolower($key); ?>"><?php print urldecode(trim($val['display_name']));?>
                                </a></li>
                                <?php
                                ++$calculateApi; // Pre increment
                }
        }
    }?>
            <?php if($calculateApi != 0) { ?>
                            </ol></li>

                    </ol>
                </div>
            </div>
            <h2 id="otherres">List Of APIs</h2>
            <?php }else{
            echo "<h2 id='otherres'>There are no API</h2>";
    }?>
<div class="section"><!-- block start --> <?php foreach($DE->doc as $key => $val){
    //#custom_attribute_0078975 
    if($val['method_name'] == 'addCustomAttributeAPI' || $val['method_name'] == 'updateCustomAttributeAPI' || $val['method_name'] == 'listCustomAttributeAPI'){
        $val['customer_display'] = $custom_attr_status;
    }
    if($val['method_name'] == 'createCustomFieldByRestAPI' || $val['method_name'] == 'updateCustomFieldByRestAPI' || $val['method_name'] == 'listCustomFieldApi' || $val['method_name'] == 'deleteCustomFieldByRestAPI'){
        if($custom_attr_status == true)
            $val['customer_display'] = false;
        else
            $val['customer_display'] = true;
    }
    
    if(isset($val['customer_display']) && trim($val['customer_display']) == true){
    $paramdet = array();
    $responsedet = array();
        if($key == 'documentation'){
            continue;
        }
        else
        {
            if( strtolower(trim($val['category']))!="adminapi") {
                continue;
            }
        }
        $DE->getParams($paramdet,$val['params']);
        /*  foreach($val['params'] as $par => $pval){
         $paramdet[$par] = explode('>',$pval);
        }
        foreach($val['response'] as $par => $pval){
        $responsedet[$par] = explode('>',$pval);
        }*/
//      $DE->getParams($responsedet,$val['response']);
        $responsedet = $val['response'];
               
        if($custom_attr_status == true){ //#custom_attribute_0078975 
           if(strtolower($key) == 'listuserapi' || strtolower($key) == 'listcoursesapi' || strtolower($key) == 'listclassesapi' 
                || strtolower($key) == 'listtpapi' || strtolower($key) == 'listorganizationapi' || strtolower($key) == 'coursesdetailsapi') {
            if(strtolower($key) == 'listuserapi')
                $customValues = getCustomAttributeValues('User','mandatory');
            
            if(strtolower($key) == 'listcoursesapi')
                $customValues = getCustomAttributeValues('Course','mandatory');
            
            if(strtolower($key) == 'listclassesapi')
                $customValues = getCustomAttributeValues('Class','mandatory');
            
            if(strtolower($key) == 'listtpapi')
                $customValues = getCustomAttributeValues('Training Plan','mandatory');
            
            if(strtolower($key) == 'listorganizationapi')
                $customValues = getCustomAttributeValues('Organization','mandatory');
            if(strtolower($key) == 'coursesdetailsapi')
                $customValues = getCustomAttributeValues('Class','mandatory');
            
              if(count($customValues)>0){    
                    foreach($customValues as $cuskey=>$cusvalue) {
                        $array1[strtolower(str_replace(' ','_',$cusvalue->attributename))] = (($cusvalue->attributehelptxt != '') ? $cusvalue->attributehelptxt : 'Custom Attribute');
                    }
                    $responsedet = array_merge_recursive($responsedet, $array1);
              }
            
            /*for($i=0; $i<count($array1); $i++) {
                $responsedet = moveArrayElement($responsedet, count($responsedet)-count($array1)+$i, (count($responsedet)-count($array1)+$i)-3);
            }*/
          }
         }
        ?>
<h3 id="<?php print strtolower($key); ?>"><?php print urldecode($val['display_name'])?>
                </h3>
                <p>
                    <strong>Description:</strong> <?php print urldecode($val['description'])?><br>
                    <strong>End Point URL:</strong> <a
                        href="<?php echo getDomainDoc();?>/apis/ext/ExpertusOneAPI"><?php echo getDomainDoc();?>/apis/ext/ExpertusOneAPI</a>
                    <br> <strong>Pre requisites:</strong> A valid access token is required for accessing this API. <br>
                    <strong>Input Parameters:</strong>
                    The following Parameters will be used to make this API call. These parameters are case sensitive.
                </p>
                <?php //create an array for optional and required params?>
                <h3>Optional Params</h3>
                <ul>
                    <?php
                    $optional = 1;
                    foreach($paramdet as $paramK => $valArr){
                        ?>
                    <?php
    if(trim($valArr[0]) == 'N'){?>
                    <li><?php
                    print $paramK;?> - <?php
                    print urldecode($valArr[1]);
                    $optional = 0;
    }
    ?></li>
                    <?php }
                    if($custom_attr_status == true){ //#custom_attribute_0078975 
                    if(strtolower($key) == 'usercreationapi' || strtolower($key) == 'userupdationapi' || strtolower($key) == 'listuserapi' 
                            || strtolower($key) == 'createcourseapi' || strtolower($key) == 'updatecourseapi' || strtolower($key) == 'listcoursesapi'
                            || strtolower($key) == 'createclassapi' || strtolower($key) == 'updateclassapi' || strtolower($key) == 'listclassesapi' 
                            || strtolower($key) == 'createtpapi' || strtolower($key) == 'updatetpapi' || strtolower($key) == 'listtpapi'
                            || strtolower($key) == 'addorganizationapi' || strtolower($key) == 'updateorganizationapi' 
                            || strtolower($key) == 'listorganizationapi' || strtolower($key) == 'createlocationapi' 
                            || strtolower($key) == 'updatelocationapi' || strtolower($key) == 'coursesdetailsapi') {
                        if(strtolower($key) == 'usercreationapi' || strtolower($key) == 'userupdationapi' || strtolower($key) == 'listuserapi')
                            $customValues = getCustomAttributeValues('User');
                        
                        if(strtolower($key) == 'createcourseapi' || strtolower($key) == 'updatecourseapi' || strtolower($key) == 'listcoursesapi')
                            $customValues = getCustomAttributeValues('Course');
                        
                        if(strtolower($key) == 'createclassapi' || strtolower($key) == 'updateclassapi' || strtolower($key) == 'listclassesapi')
                            $customValues = getCustomAttributeValues('Class');
                        
                        if(strtolower($key) == 'createtpapi' || strtolower($key) == 'updatetpapi' || strtolower($key) == 'listtpapi')
                            $customValues = getCustomAttributeValues('Training Plan');
                        
                        if(strtolower($key) == 'addorganizationapi' || strtolower($key) == 'updateorganizationapi' || strtolower($key) == 'listorganizationapi')
                            $customValues = getCustomAttributeValues('Organization');
                        
                        if(strtolower($key) == 'createlocationapi' || strtolower($key) == 'updatelocationapi')
                            $customValues = getCustomAttributeValues('Location');
                        
                        if(strtolower($key) == 'coursesdetailsapi')
                            $customValues = getCustomAttributeValues('Class');
                        
                          if(count($customValues)>0){                
                                foreach($customValues as $cuskey=>$cusvalue) {
                                    if(!empty($cusvalue) && $cusvalue->attributmandatory == 0) {
                                        print '<li>'.$cusvalue->attributename.' - '.(($cusvalue->attributehelptxt != '') ? $cusvalue->attributehelptxt : 'Custom Attribute').'</li>';
                                    }
                                }
                          }
                    }
                    } //#custom_attribute_0078975 
                    if($optional){
                        ?>
                    <li>None</li>
                    <?php
                    }
                    ?>
                </ul>

                <h3>Required params</h3>

                <ul>
                    <!--   <li>userid - &laquo; userid &raquo;</li> -->
                    <?php foreach($paramdet as $paramK => $valArr){
        if(trim($valArr[0]) == 'Y'){?>
                    <li><?php
                    print $paramK;?> - <?php
                    print urldecode($valArr[1]);
        }
        ?></li>
                    <?php }
                    if($custom_attr_status == true){ //#custom_attribute_0078975 
                    if(strtolower($key) == 'usercreationapi' || strtolower($key) == 'userupdationapi' || strtolower($key) == 'createcourseapi' 
                            || strtolower($key) == 'updatecourseapi' || strtolower($key) == 'createclassapi' || strtolower($key) == 'updateclassapi' 
                            || strtolower($key) == 'createtpapi' || strtolower($key) == 'updatetpapi' || strtolower($key) == 'addorganizationapi' 
                            || strtolower($key) == 'updateorganizationapi' || strtolower($key) == 'createlocationapi' || strtolower($key) == 'updatelocationapi') {
                            
                          if(count($customValues)>0){   
                                foreach($customValues as $ckey=>$cvalue) {
                                    if(!empty($cvalue) && $cvalue->attributmandatory == 1) {
                                        print '<li>'.$cvalue->attributename.' - '.(($cvalue->attributehelptxt != '') ? $cvalue->attributehelptxt : 'Custom Attribute').'</li>';
                                    }
                                }
                          }
                        
                    }
                    } //#custom_attribute_0078975 
                    ?>
    <li>returntype - Format in which the response is generated. Possible values: "xml"/"json"</li>
    <li>display_cols - Enter the required columns which are to be displayed as response. Multiple values can be entered by adding commas.</li>
    <li>apiname - The name used to call this API: "<?php print $key;?>"</li>
                    <li>access_token - oAuth Authentication token</li>
                </ul>
                <p>
                    <strong>API Response Schema</strong> The following columns will be generated as response when this API is called,
                </p>
                
                <?php 
    
    echo parseResponse($responsedet);

                ?>

                <p>Example code snippet to make this api call.</p>
                <div class="toc" id="toc">
                    <?php 
                    $disp_cols="'";
                    foreach($responsedet as $paramK => $valArr)
                    {
                        if($disp_cols=="'")
                            $disp_cols="'".$paramK;
                        else
                            $disp_cols= $disp_cols.",".$paramK;
                    }
                    $disp_cols.="'";
                    ?>
                    <div class="toc-title"></div>
                    <div class="toc-list">
                        <pre>

    /* Request for access token */
    
    $base_url = "<?php echo $base_url; ?>";
    $client_id="ef7c21f8966db0044802c1a7f19b8da2";
    $client_secret="4500e1dc6ded85fecb61cacb403b162d";
    
    $client_details = base64_encode("$client_id:$client_secret");
    
    $context = stream_context_create(array(
        'http' => array(
        'method' => 'POST',
        'header' => "Content-Type: application/x-www-form-urlencoded\r\n" .
                    "Authorization : Basic $client_details\r\n",
        'content' => "grant_type=client_credentials"
        )
    ));
     
    $response = file_get_contents($base_url.'/apis/oauth2/Token.php', false, $context);
    $res =  json_decode($response);
    $token = $res->access_token;

    // API params
    $data = array();              
    $data["display_cols"]=<?php print $disp_cols.";\n";?>
    $data["returntype"]="xml"; 
    $data["apiname"] = "<?php print $key;?>";<?php print "\n\t"; ?>

    // Required params
<?php
echo "\t"; 
foreach($paramdet as $paramK => $valArr){
    
        if(trim($valArr[0]) == 'Y'){?><?php
        echo '$data["'.$paramK.'"] = "'.(isset($valArr[2]) ? $valArr[2] : '').'";'."\n\t";
?><?php }}?>
    
    $fields_string = http_build_query($data); 
    
    $context = stream_context_create(array(
        'http' => array(
        'method' => 'POST',
        'header' => "Content-Type: application/x-www-form-urlencoded\r\n" .
                    "Authorization : Bearer $token\r\n",
        'content' => $fields_string
        )
    )); 
    
    $response = file_get_contents($base_url.'/apis/ext/ExpertusOneAPI.php', false, $context);
    $res =  json_decode($response);
    echo $res;

                    </div>
                </div>
                <!--   </div> -->

                <p>Sample output for XML returntype</p>
                <div class="toc" id="toc">
                    <div class="toc-title"></div>
                    <div class="toc-list">
                        <pre>
<?php
        print "\n";
        //print $DE->generateXMLData($DE->getResultSet($key));
        print generateXMLResponseData($val['response']);
        ?> 
</pre>
                    </div>
                </div>
                <p>Sample output for JSON returntype.</p>
                <div class="toc" id="toc">
                    <div class="toc-title"></div>
                    <div class="toc-list">
                        <pre>
        <?php
//      print "\n";
//      print $DE->constructJSON(json_encode($DE->getResultSet($key)));
        print "\n";
        print generateJSONResponseData($val['response']);
        ?>
</pre>
                    </div>
                </div>

        <?php } }?> <!-- very end -->
            </div>
        </div>
    </div>

        <?php include 'footer.php'; ?>