<?php
include_once ($_SERVER["DOCUMENT_ROOT"] . '/translatetool/MicrosoftTranslateArrayAutoDetection.php');


function getHtmlEventPatternTranslation($op=0){
    $html_events = 'onafterprint|onbeforeprint|onbeforeunload|onerror|onhashchange|onload|onmessage|onoffline|ononline|onpagehide|onpageshow|onpopstate|onresize|onstorage|onunload|ondblclick|ondrag|ondragend|ondragenter|ondragleave|ondragover|ondragstart|ondrop|onmousedown|onmousemove|onmouseout|onmouseover|onmouseup|onmousewheel|onscroll|onwheel|onblur|onchange|oncontextmenu|onfocus|oninput|oninvalid|onreset|onsearch|onselect|onsubmit|onkeydown|onkeypress|onkeyup|onclick|oncopy|oncut|onpaste|onabort|oncanplay|oncanplaythrough|oncuechange|ondurationchange|onemptied|onended|onerror|onloadeddata|onloadedmetadata|onloadstart|onpause|onplay|onplaying|onprogress|onratechange|onseeked|onseeking|onstalled|onsuspend|ontimeupdate|onvolumechange|onwaiting|onerror|onshow|ontoggle|onerror';
    
    $pattern = "/(<[A-Z][A-Z0-9]*[^>]*)($html_events)([\s]*=[\s]*)('[^>]*'|\"[^>]*\"|[^>]*)([^>]*>)/i";
    
    return ($op==1) ? $pattern : '/'.$html_events.'/i';
}

/**
 * Form validation for expertus_xss_validate().
 *
 */
function expertus_xss_validate_translation($fieldname,$fieldvalue) { //it should be removed later and use common method
    try{
        $retValue = new stdClass();
        $retValue->iserror = false;
        // 			expDebug::dPrint("xss_validate $fieldname ".$fieldname,4);
        // 			expDebug::dPrint("xss_validate $fieldvalue ".$fieldvalue,4);
        $pattern = getHtmlEventPatternTranslation();
        $replacement = '$1$5';
        {
            //	$terms =array('(?i)<\s*script','%2fscript','(?i)<\s*img','(?i)<\s*b','(?i)<\s*input','(?i)<\s*a','(?i)<\s*body','(?i)<\s*meta','(?i)<\s*\?');
            $terms =array('(?i)<\s*applet','(?i)<\s*base','(?i)<\s*bgsound','(?i)<\s*big','(?i)<\s*blink','(?i)<\s*blockquote','(?i)<\s*br','(?i)<\s*caption','(?i)<\s*center','(?i)<
							\s*cite','(?i)<\s*code','(?i)<\s*del','(?i)<\s*dir','(?i)<\s*div','(?i)<\s*embed','(?i)<\s*font',
                '(?i)<\s*form','(?i)<\s*frame','(?i)<\s*frameset','(?i)<\s*h','(?i)<\s*head','(?i)<\s*hr','(?i)<\s*html','(?i)<\s*i','(?i)<\s*link','(?i)<\s*marquee','(?i)<
							\s*menu','(?i)<\s*note','(?i)<\s*ol','(?i)<\s*p','(?i)<\s*param','(?i)<\s*pre','(?i)<\s*q','(?i)<\s*select','(?i)<\s*small','(?i)<\s*strike','(?i)<\s*stron
							g','(?i)<\s*sub','(?i)<\s*table','(?i)<\s*tbody','(?i)<\s*td','(?i)<\s*textarea','(?i)<\s*th','(?i)<\s*thead','(?i)<\s*title','(?i)<\s*tr','(?i)<\s*ul','(?i)<\s*var',
                '(?i)<\s*script','(?i)<\s*img','(?i)<\s*b','(?i)<\s*input','(?i)<\s*a','(?i)<\s*body','(?i)<\s*meta','(?i)<\s*iframe','(?i)<\s*\?');
            $result = array();
            
            foreach($terms as $term){
                if(!count($result)){
                    $result = preg_filter('~' . $term . '~','$0', $fieldvalue);
                }else{
                    break;
                }
            }
            while( preg_match($pattern, $fieldvalue) ){
                $fieldvalue = preg_replace($pattern, $replacement, $fieldvalue);
            }
            if(count($result))
            {
                //$this->sendError($fieldname." contains invalid text","Given data is having possibility of cross site scripting or sql injection.");
                $retValue->iserror = true;
                $retValue->error = $fieldname." contains invalid text";
            }
            else
            {
                $fieldvalue = xss_clean_translation($fieldvalue);
                $retValue->data = $fieldvalue;
            }
        }
        
        return $retValue;
    }catch (Exception $ex) {
        watchdog_exception('expertus_xss_validate', $ex);
        expertusErrorThrow($ex);
    }
    
}

function xss_clean_translation($data){
    // Fix &entity\n;
    $data = html_entity_decode($data, ENT_COMPAT, 'UTF-8');
    $data = rawurldecode($data);
    $data = str_replace(array('&amp;','&lt;','&gt;'), array('&amp;amp;','&amp;lt;','&amp;gt;'), $data);
    $data = preg_replace('/(&#*\w+)[\x00-\x20]+;/u', '$1;', $data);
    $data = preg_replace('/(&#x*[0-9A-F]+);*/iu', '$1;', $data);
    
    
    // Remove any attribute starting with "on" or xmlns
    $data = preg_replace('#(<[^>]+?[\x00-\x20"\'])(?:on|xmlns)[^>]*+>#iu', '$1>', $data);
    
    // Remove javascript: and vbscript: protocols
    $data = preg_replace('#([a-z]*)[\x00-\x20]*=[\x00-\x20]*([`\'"]*)[\x00-\x20]*j[\x00-\x20]*a[\x00-\x20]*v[\x00-\x20]*a[\x00-\x20]*s[\x00-\x20]*c[\x00-\x20]*r[\x00-\x20]*i[\x00-\x20]*p[\x00-\x20]*t[\x00-\x20]*:#iu', '$1=$2nojavascript...', $data);
    $data = preg_replace('#([a-z]*)[\x00-\x20]*=([\'"]*)[\x00-\x20]*v[\x00-\x20]*b[\x00-\x20]*s[\x00-\x20]*c[\x00-\x20]*r[\x00-\x20]*i[\x00-\x20]*p[\x00-\x20]*t[\x00-\x20]*:#iu', '$1=$2novbscript...', $data);
    $data = preg_replace('#([a-z]*)[\x00-\x20]*=([\'"]*)[\x00-\x20]*-moz-binding[\x00-\x20]*:#u', '$1=$2nomozbinding...', $data);
    
    // Only works in IE: <span style="width: expression(alert('Ping!'));"></span>
    $data = preg_replace('#(<[^>]+?)style[\x00-\x20]*=[\x00-\x20]*[`\'"]*.*?expression[\x00-\x20]*\([^>]*+>#i', '$1>', $data);
    $data = preg_replace('#(<[^>]+?)style[\x00-\x20]*=[\x00-\x20]*[`\'"]*.*?behaviour[\x00-\x20]*\([^>]*+>#i', '$1>', $data);
    $data = preg_replace('#(<[^>]+?)style[\x00-\x20]*=[\x00-\x20]*[`\'"]*.*?s[\x00-\x20]*c[\x00-\x20]*r[\x00-\x20]*i[\x00-\x20]*p[\x00-\x20]*t[\x00-\x20]*:*[^>]*+>#iu', '$1>', $data);
    
    // Remove namespaced elements (we do not need them)
    $data = preg_replace('#</*\w+:\w[^>]*+>#i', '', $data);
    // 		expDebug::dPrint("xss clean 2".$data,4);
    do{
        // Remove really unwanted tags
        $old_data = $data;
        $data = preg_replace('#</*(?:applet|b(?:ase|gsound|link)|embed|frame(?:set)?|i(?:frame|layer)|l(?:ayer|ink)|meta|object|s(?:cript|tyle)|title|xml)[^>]*+>#i', '', $data);
    }while ($old_data !== $data);
    // 		expDebug::dPrint("xss clean 3".$data,4);
    // we are done...
    return $data;
}

function getDrupalUserDataForTranslation($sltPersonId)
{
    try{
        if (!empty($sltPersonId)) {
            // Select table
            $selectStmt = db_select('slt_person', 'p');
            $selectStmt->leftJoin('users', 'u', 'p.user_name = u.name');
            // Set conditions
            $selectStmt->condition('p.id', $sltPersonId, '=');
            // Select fields and/or add expressions
            $selectStmt->addField('u', 'uid', 'uid');
            $selectStmt->addField('u', 'language', 'language');
            expDebug::dPrintDBAPI(' $selectStmt obj = ' , $selectStmt);
            // Execute query
            $result = $selectStmt->execute()->fetchAll();
            $user = new stdClass();
            $user->uid = $result[0]->uid;
            $user->language = $result[0]->language;
        }
        return $user;
    }catch (Exception $ex) {
        watchdog_exception('getDrupalUserDataForTranslation', $ex);
        expertusErrorThrow($ex);
    }
}
function getTranslationsForArrayStrings($arr, $userLang, $translationentityids, $entity_type)
{
    try {
        expDebug::dPrint("translationentityids::" . print_r($translationentityids, true), 5);
        expDebug::dPrint("entity_type::" . print_r($entity_type, true), 5);
        
        $time_start = microtime(true);
        $array_string = array_values($arr);
        expDebug::dPrint('Labels from source ' . print_r($array_string, true));
        if (count($arr) == 0)
            return array();
            
            $loggedInUserId = getIdOfLoggedInUser();
            $newarr = array();
            
            if ($userLang == "zh-hans")
                $userLang = "zh";
                else if ($userLang == "en-us")
                    $userLang = "en";
                    else if ($userLang == "pt-br")
                        $userLang = "pt";
                        else if ($userLang == "pt-pt")
                            $userLang = "pt";
                            
                            
                            $fieldName = "en_label";
                            /* if ($userLang == "zh-hans")
                             $fieldName = "zh_label";
                             else if ($userLang == "en-us")
                             $fieldName = "en_label";
                             else if ($userLang == "pt-br")
                             $fieldName = "pt_label";
                             else if ($userLang == "pt-pt")
                             $fieldName = "pt_label";
                             else*/
                             if($userLang == ""){
                                 $userLang = "en";
                             }
                             $fieldName = $userLang . "_label";
                             
                             $newNottranslatedLabels = array();
                             
                             $select = db_select('slt_labels_store', 'lbl');
                             $select->addField('lbl', 'entity_id', 'entity_id');
                             $select->addField('lbl', 'src_label', 'src_label');
                             $select->addField('lbl', 'src_lang', 'src_lang');
                             $select->addField('lbl', $fieldName, $fieldName);
                             // $select->condition('lbl.src_label', $array_string, 'in');
                             if ($entity_type == "custom_attribute_options")
                                 $select->condition('lbl.src_label', $array_string, 'in');
                                 else {
                                     $select->condition('lbl.entity_id', $translationentityids, 'in');
                                     $select->condition('lbl.entity_type', $entity_type, '=');
                                 }
                                 $result = $select->execute()->fetchAll();
                                 expDebug::dPrintDBAPI('Labels from labels store select obj = ', $select);
                                 expDebug::dPrint('Labels from labels store result ' . print_r($result, true));
                                 
                                 $j = 0;
                                 for ($i = 0; $i < count($array_string); $i ++) {
                                     
                                     for ($j = 0; $j < count($result); $j ++) {
                                         if ($array_string[$i] == $result[$j]->src_label && $result[$j]->src_lang != $userLang) {
                                             if (is_null($result[$j]->$fieldName) || strlen($result[$j]->$fieldName) == 0) {
                                                 $newarr[$array_string[$i]] = $array_string[$i]; // it will be updated after translation
                                                 $newNottranslatedLabels[$array_string[$i]] = $array_string[$i];
                                             } else
                                                 $newarr[$array_string[$i]] = $result[$j]->$fieldName;
                                         }
                                         else
                                         {
                                             if ($array_string[$i] == $result[$j]->src_label )
                                             {
                                             $updatingActualLangLabel = array();
                                             $updatingActualLangLabel['created_on'] = now();
                                             $updatingActualLangLabel['created_by'] = $loggedInUserId;
                                             $updatingActualLangLabel['updated_on'] = now();
                                             $updatingActualLangLabel['updated_by'] = $loggedInUserId;
                                             $updatingActualLangLabel[$fieldName] = $result[$j]->src_label;
                                             db_update('slt_labels_store')->fields($updatingActualLangLabel)
                                             ->condition('src_label', $result[$j]->src_label)
                                             ->execute();
                                             $newarr[$result[$j]->src_label] = $result[$j]->src_label;
                                             }
                                             
                                         }
                                     }
                                 }
                                 
                                 expDebug::dPrint("Need to translate :::" . print_r($newNottranslatedLabels, true), 3);
                                 expDebug::dPrint("Already translated :::" . print_r($newarr, true), 3);
                                 
                                 $new_array_string = array_values($newNottranslatedLabels);
                                 if (count($new_array_string) > 0) {
                                     
                                     $convertedStrArr = invokeTranslation($new_array_string, $userLang);
                                     expDebug::dPrint("Translated labels from microsoft api :::" . print_r($convertedStrArr, true), 3);
                                     
                                     $translatedArr = array();
                                     
                                     for ($i = 0; $i < count($new_array_string); $i ++) {
                                         $newarr[$new_array_string[$i]] = strval($convertedStrArr[$i]);
                                         
                                         $translatedArr['created_on'] = now();
                                         $translatedArr['created_by'] = $loggedInUserId;
                                         $translatedArr['updated_on'] = now();
                                         $translatedArr['updated_by'] = $loggedInUserId;
                                         $translatedArr[$fieldName] = strval($convertedStrArr[$i]);
                                         db_update('slt_labels_store')->fields($translatedArr)
                                         ->condition('src_label', $new_array_string[$i])
                                         ->execute();
                                     }
                                 }
                                 $time_end = microtime(true);
                                 expDebug::dPrint("Time diff  :::" . ($time_end - $time_start), 3);
    } catch (Exception $ex) {
        watchdog_exception('exp_sp_administration_announcement_permission', $ex);
        expertusErrorThrow($ex);
    }
    
    return $newarr;
}

function updateContentLabels($oldval_arr, $newval_arr, $src_lang, $entity_type, $idarr, $fieldNamesArr = '')
{
    try {
        expDebug::dPrint("idarr::" . print_r($idarr, true), 5);
        expDebug::dPrint("entity_type::" . print_r($entity_type, true), 5);
        
        $fieldName = "";
        
        if ($src_lang == "zh-hans")
            $src_lang = "zh";
            else if ($src_lang == "en-us")
                $src_lang = "en";
                else if ($src_lang == "pt-br")
                    $src_lang = "pt";
                    else if ($src_lang == "pt-pt")
                        $src_lang = "pt";
                        
                        $recordCreatedLang = false;
                        $langlist = array("en","zh","ja","ko","fr","de","pt","it","es","ru");
                        for ($i = 0; $i < count($oldval_arr); $i ++) {
                            
                            if($oldval_arr[$i] == $newval_arr[$i])
                                continue; // update not required
                            
                            $translatedArr = array();
                            $loggedInUserId = getIdOfLoggedInUser();
                            
                            $translatedArr['created_on'] = now();
                            $translatedArr['created_by'] = $loggedInUserId;
                            $translatedArr['updated_on'] = now();
                            $translatedArr['updated_by'] = $loggedInUserId;
                        //    $translatedArr["sync_required"] ="1";
                            
                            $select = db_select('slt_labels_store', 'lbl');
                            $select->addField('lbl', "src_lang", "src_lang");
                            $select->addField('lbl', "src_label", "src_label");                            
                            $select->condition('entity_type', $entity_type);
                            $select->condition('entity_field_name', $fieldNamesArr[$i]);
                            $select->condition('entity_id', $idarr[0])->execute();
                            $result = $select->execute()->fetchAll();
                            expDebug::dPrintDBAPI("labels store select::: ", $select);
                            $translatedArr["src_label"] = $newval_arr[$i];
                            $translatedArr[$result[0]->src_lang."_label"]=$newval_arr[$i];
                            
                            for($langItr = 0; $langItr < count($langlist);$langItr++)
                                if($result[0]->src_lang != $langlist[$lantItr])
                                    $translatedArr[$langlist[$langItr]."_label"] = "";
                                    
                           // $translatedArr[$src_lang . "_label"] = "";
                            //$translatedArr[$src_lang . "_label"] = $newval_arr[$i];
                            
                            if ($result[0]->src_lang == $src_lang) {
                                //$translatedArr["src_label"] = $newval_arr[$i];
                                $recordCreatedLang = true;
                            }
                            expDebug::dPrint("updated values :::" . print_r($translatedArr, true), 3);
                            // db_update('slt_labels_store') -> fields($translatedArr)
                            // -> condition($src_lang.'_label', $oldval_arr[$i]) -> execute();
                            
                            if(count($result[0]) > 0)
                                db_update('slt_labels_store')->fields($translatedArr)
                                ->condition('entity_type', $entity_type)
                                  ->condition('entity_field_name', $fieldNamesArr[$i])
                            ->condition('entity_id', $idarr[0])
                            ->execute();
                        }
                        return $recordCreatedLang;
    } catch (Exception $ex) {
        watchdog_exception('ExpertusONETranslation', $ex);
        expertusErrorThrow($ex);
    }
}

function storeContentLabels($arr, $src_lang, $entity_type = '', $entity_id = '', $field_names_arr,$oldOptions = array())
{
    
    try {
        $loggedInUserId = getIdOfLoggedInUser();
        $newLabels = array();
        if ($src_lang == "en-us") {
            $fieldName = "en_label";
            $src_lang = "en";
        } else if ($src_lang == "zh-hans") {
            $fieldName = "zh_label";
            $src_lang = "zh";
        } else if ($src_lang == "pt-br") {
            $fieldName = "pt_label";
            $src_lang = "pt";
        }
        else if ($src_lang == "pt-pt") {
            $fieldName = "pt_label";
            $src_lang = "pt";
        }
        else
            $fieldName = $src_lang . "_label";
            
          /*  $select = db_select('slt_labels_store', 'lbl');
            $select->addField('lbl', $fieldName, $fieldName);
            $select->addField('lbl', 'src_lang', 'src_lang');
            // $select -> addExpression('count(1)', 'cnt');
            $select->condition('lbl.' . $fieldName, $arr, 'in');
            $result = $select->execute()->fetchAll();
            expDebug::dPrintDBAPI("store content::: ", $select);*/
            expDebug::dPrint("new labels::" . print_r($arr, true), 3);
            
            expDebug::dPrint("storecontent oldoptions::" . print_r($oldOptions, true), 3);
            expDebug::dPrint("new ids:" . print_r($entity_id, true), 3);
            
            $k = 0;
            $optionsDBUpdateIds = array();
            for ($i = 0; $i < count($arr); $i ++) {
                $labelName = $arr[$i];
                $recExist  = false;
                for($j = 0; $j < count($oldOptions); $j++)
                {
                    expDebug::dPrint($oldOptions[$j]->opt_name."==chk==".$labelName);
                    if($oldOptions[$j]->opt_name == $labelName)
                    {
                        expDebug::dPrint($entity_type."====Inside if...".$entity_id[$i]."====".$field_names_arr."===old:".$oldOptions[$j]->id);
                        //expdebug::dPrint("array search:".array_search($oldOptions[$j]->id,$optionsDBUpdateIds),5);
                        //if(array_search($oldOptions[$j]->id,$optionsDBUpdateIds) == false)
                        {
                        $recExist = true;
                        $translatedArr = array();
                        
                        $translatedArr["entity_id"]= $entity_id[$i];
                        
                        if ($entity_type == "custom_attribute_options") {
                        db_update('slt_labels_store')->fields($translatedArr)
                        ->condition('entity_type', $entity_type)
                        ->condition('entity_field_name', $field_names_arr)
                        ->condition('entity_id', $oldOptions[$j]->id)
                        ->execute();
                        }
                        else
                        {
                            db_update('slt_labels_store')->fields($translatedArr)
                            ->condition('entity_type', $entity_type)
                            ->condition('entity_field_name', $field_names_arr[$i])
                            ->condition('entity_id', $oldOptions[$j]->id)
                            ->execute();
                        }
                        
                        //$optionsDBUpdateIds[] = $oldOptions[$j]->id;
                       // break;
                        }
                    }
                }
                if($recExist == false){
                    $newLabels[$k++] = $labelName;
                }
                
            }
            expDebug::dPrint("new labels for conversion::" . print_r($newLabels, true), 3);
            $langCode = "";
            $translatedArr = array();
            $translatedArrForInsert = array();
            for ($l = 0; $l < count($newLabels); $l ++) {
                
                
                if ($entity_type == "custom_attribute_options") {
                    
                    $actualIndex = "";
                    for ($q = 0; $q < count($arr); $q++) {
                        if($newLabels[$l]== $arr[$q]){
                            $actualIndex = $q;
                            break;
                        }
                            
                    }
                    
                    $select1 = db_select('slt_custom_attr_options', 'opt');
                    $select1->addField('opt', "cattr_id", "cattr_id");
                    // $select -> addExpression('count(1)', 'cnt');
                    $select1->condition('opt.id',$entity_id[$actualIndex],'=');
                    $result1 = $select1->execute()->fetchAll();
                    
                    
                    $select2 = db_select('slt_labels_store', 'sto');
                    $select2->addField('sto', "src_lang", "src_lang");
                    // $select -> addExpression('count(1)', 'cnt');
                    $select2->condition('sto.entity_id',$result1[0]->cattr_id,'=');
                    $select2->condition('sto.entity_type','cre_sys_obt_cattr','=');
                    $result2 = $select2->execute()->fetchAll();
                    $translatedArrForInsert[$result2[0]->src_lang. "_label"] = $newLabels[$l];
                    $translatedArrForInsert['src_lang'] = $result2[0]->src_lang;
                    
                }
                $translatedArrForInsert["src_label"] = $newLabels[$l];
                if ($entity_type != "custom_attribute_options") 
                    $translatedArrForInsert['src_lang'] = $src_lang;
                $translatedArrForInsert['created_on'] = now();
                $translatedArrForInsert['created_by'] = $loggedInUserId;
                $translatedArrForInsert['updated_on'] = now();
                $translatedArrForInsert['updated_by'] = $loggedInUserId;
                
                $translatedArrForInsert['entity_type'] = $entity_type;
                if ($entity_type == "custom_attribute_options") {
                    $translatedArrForInsert['entity_id'] = $entity_id[$actualIndex];
                    $translatedArrForInsert['entity_field_name'] = $field_names_arr;
                } else if ($entity_type == "cre_sys_obt_cattr") {
                    $translatedArrForInsert['entity_id'] = $entity_id;
                    $translatedArrForInsert['entity_field_name'] = $field_names_arr[$l];
                }
                if(trim($translatedArrForInsert["src_label"]) != "")
                    db_insert('slt_labels_store')->fields($translatedArrForInsert)->execute();
            }
    } catch (Exception $ex) {
        watchdog_exception('ExpertusONETranslation', $ex);
        expertusErrorThrow($ex);
    }
}

?>
