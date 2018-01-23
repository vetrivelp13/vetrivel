<?php 
expDebug::dPrint('enter the function sasasas'.print_r($record,true),4);
expDebug::dPrint('enter the function sasasasAAAA'.$record->classid,4);
global $theme_key;
?>
<div class="top-record-div-left">
 <div class="para">
  <div class="code-container">
        <span class="detail-code"><?php print t("LBL083"); ?>:</span>
         <span class="detail-desc limit-title-row"><span class="vtip limit-title" title="<?php print sanitize_data($record->title); ?>"> <?php print $record->title; ?>  </span></span>                                                 
         </span>                                    
    </div>
 </div>
  <div class="left-side-container">
     <div class="para">
        <div class="code-container">
            <span class="detail-code"><?php print t("LBL096"); ?>:</span>
             <span class="detail-desc detail-single-line"><span class="vtip single-line-lbl-val" title="<?php print sanitize_data($record->code); ?>"> <?php ($theme_key == 'expertusoneV2') ? print titleController('NARROW-CLASSVIEW-DETAILDESC-CODE', $record->code,50) :  print titleController('NARROW-CLASSVIEW-DETAILDESC-CODE', $record->code,45); ?>  </span></span>                                   
        </div>
     </div>
 </div>
  <div class="right-side-container">
     <div class="para">
        <div class="code-container">
            <span class="detail-code"><?php print t("LBL038"); ?>:</span>
             <span class="detail-desc">
             <?php print t(getProfileListItemName($record->clslang)); ?>                                            
             </span>                                    
        </div>
     </div> 
    </div> 
 <div class="para">
    <div class="code-container">
        <span class="detail-code"><?php print t("LBL229"); ?>:</span>
         <span id="detail-desc-view" class="detail-desc">
         <?php print $record->description; ?>                                               
         </span>                                    
    </div>
 </div> 
 <div class="para">
       <div class="code-container additionalnotes">
                <span class="detail-code-addinformation"><?php print t("LBL3068"); ?>:</span>
                <span class="detail-desc-addnot"><?php print $record->additional_info;?></span>
                
       </div>
      <div  class="para">
            <?php if($record->addn_catalog_show != 1){
            $add_margin = 'set-addinformation';
        }

            ?>
            <?php if($record->addn_catalog_show == 1){
            ?>
            <div class="detail-desc-cata-notes">

                <?php print t("Shown in Catalog");
                ?>
            </div>
            <?php }
            ?>
            <?php if($record->addn_catalog_show == 1 && $record->addn_notification_show){
            ?>
            <div class="add-info-detail-pipe-line <?php echo $add_margin; ?>">
                |
            </div>
            <?php }
            ?>
            <?php if($record->addn_notification_show == 1){
            ?>
            <div class="detail-desc-noti-notes <?php echo $add_margin; ?>">

                <?php print t("LBL3071");
                ?>
            </div>
            <?php }
            ?>
        </div>
</div>
 <div class="left-side-container">
     <div class="para">
        <div class="code-container">
            <span class="detail-code deli-type"><?php print t("LBL084"); ?>:</span>
             <span class="detail-desc"><?php 
                    $deliverytypeVal=getProfileListItemAttr($record->clsdeliverytype);
                    $row_del_type='';
                    if ($deliverytypeVal == '' || $deliverytypeVal == '-') {
                    return '<span>&nbsp;</span>';
                    }
                    else if($deliverytypeVal == 'WBT'){
                    $row_del_type = t("Web-based");
                    }
                    else if($deliverytypeVal == 'VC'){
                    $row_del_type = t("Virtual Class");
                    }
                    else if($deliverytypeVal == 'ILT'){
                    $row_del_type = t("Classroom");
                    }
                    else if($deliverytypeVal == 'VOD'){
                    $row_del_type = t("Video");
                    }
                    print $row_del_type;?>
        </span>                                 
        </div>
     </div>
      <?php 
      $tags  = getCatalogTags($record->classid,'Class');
        $tagString  = '';
        if(!empty($tags)){
          $tagString  = implode(", ",$tags);
        } ?>
     <div class="para">
        <div class="code-container">
            <span class="detail-code"><?php print t("LBL191"); ?>:</span>
             <span class="detail-desc detail-single-line">
             <span class="vtip single-line-lbl-val" title="<?php print sanitize_data($tagString); ?>"> 
             <?php  print titleController('NARROW-CLASSVIEW-DETAILDESC-TAGSTRING', $tagString,50)  ?>  </span>                                          
             </span>                                    
        </div>
     </div>
     <div class="para">
       <div class="code-container">
          <span class="detail-code reg-ends"><?php print t("LBL565"); ?>:</span>
           <span class="detail-desc">
                <?php if($record->registration_end_on){print date_format(date_create($record->registration_end_on),'m-d-Y');} ?>                                                
         </span>                                    
       </div>
   </div>
     <?php if($record->clsdeliverytype == 'lrn_cls_dty_ilt' || $record->clsdeliverytype == 'lrn_cls_dty_vcl') {?>
     <div class="para">
        <div class="code-container">
            <span class="detail-code"><?php print t("LBL248"); ?>:</span>
             <span class="detail-desc">
             <?php print $record->duration; ?>                                              
             </span>                                    
        </div>
     </div>
    <?php } ?>
 </div>
 <div class="right-side-container">
      <div class="para">
         <div class="code-container">
            <span class="detail-code"><?php print t("LBL040"); ?>:</span>
             <span class="detail-desc">
             <?php print $record->currency_type.' '.$record->price; ?>                                              
             </span>                                    
        </div>
     </div> 
    <?php $businessStr = t(getBusinessInfoView($record->classid));?>
     <div class="para">
        <div class="code-container">
            <span class="detail-code busi-rule"><?php print t("LBL719"); ?>:</span>
            <span class="detail-desc detail-single-line"><span class="vtip single-line-lbl-val" title="<?php print sanitize_data($businessStr); ?>"> <?php ($theme_key == 'expertusoneV2') ? print titleController('NARROW-CLASSVIEW-DETAILDESC-BUSINESSSTR', $businessStr,22) :print titleController('NARROW-CLASSVIEW-DETAILDESC-BUSINESSSTR', $businessStr,16); ?></span></span>                                     
        </div>
     </div>
     <div class="para">
        <div class="code-container">
            <span class="detail-code auth-vend"><?php print t("LBL269"); ?>:</span>
          <span class="detail-desc detail-single-line"><span class="vtip single-line-lbl-val" title="<?php print sanitize_data($record->author); ?>"> <?php ($theme_key == 'expertusoneV2') ? print titleController('NARROW-CLASSVIEW-DETAILDESC-AUTH-VEND', $record->author,22) : print titleController('NARROW-CLASSVIEW-DETAILDESC-AUTH-VEND', $record->author,16); ?>  </span></span>                                                                               
        </div>
     </div>
     <?php if($record->clsdeliverytype == 'lrn_cls_dty_ilt' || $record->clsdeliverytype == 'lrn_cls_dty_vcl') {?>
         <div class="para">
            <div class="code-container">
                <span class="detail-code"><?php print t("LBL267"); ?>:</span>
                 <span class="detail-desc">
                 <?php print $record->maxcapacity; ?>                                               
                 </span>                                    
            </div>
         </div>
        <?php } ?>
</div>
 <?php 
        if(module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status
            //Get Custom Attributes for Class View #custom_attribute_0078975
            include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'/exp_sp_administration_customattribute_renderinforms.inc');
            print getCustomAttributesForView($record->classid, 'cre_sys_obt_cls');
        } //#custom_attribute_0078975 - End Check module status
        
  $entityTypeForClass = 'cre_sys_obt_cls';
  $attachedRecord = getAttachmentsInfoView($record->classid, $entityTypeForClass);
    $numAttachments = count($attachedRecord);
    ?>
  <div class="para">
    <div class="code-container">
        <span class="detail-code"><?php print t("LBL231"); ?>:</span>
             <span class="detail-desc detail-single-line">
             <?php for ($i = 0; $i < $numAttachments; $i++) { ?>
            <?php $attachment_array[] = sanitize_data($attachedRecord[$i]->title);
            }
            $attachments_implode = implode('<span class="divider-pipeline">|</span>',$attachment_array);
            $attachments_implode_vtip = implode(', ',$attachment_array);
           ?>
          <span class="vtip single-line-lbl-val" title="<?php print $attachments_implode_vtip; ?>"> <?php  print titleController('NARROW-CLASSVIEW-DETAILDESC-ATTACHMENTS-NAME', $attachments_implode,100); ?></span></span>
    </div>
 </div>
 <?php $access  = getGroupNamesForView($record->classid, 'cre_sys_obt_cls');
  $accessName  = '';
    if(!empty($access)){
      $accessName  = implode(", ",$access);
    }
    ?>
 <div class="para">
    <div class="code-container">
        <span class="detail-code"><?php print t("LBL642"); ?>:</span>
        <span id= "access-detail-desc" class="detail-desc detail-single-line"><span class="vtip single-line-lbl-val" title="<?php print sanitize_data($accessName); ?>"> <?php ($theme_key == 'expertusoneV2') ? print titleController('NARROW-CLASSVIEW-DETAILDESC-ACCESS', $accessName,90) : print titleController('NARROW-CLASSVIEW-DETAILDESC-ACCESS', sanitize_data($accessName),75); ?>  </span></span>                                           
    </div>
 </div>
 <?php $surveyName = getSurveyName($record->classid,'cre_sys_obt_cls');
             $surname = array();
             expDebug::dPrint('$$surname = '.print_r($surveyName,true),5);
             if(!empty($surveyName)){
                foreach($surveyName as $name){
                    $surname[]  = $name->title;
                }
                $survey_name  = implode(", ",$surname) ;
             }else{
                $survey_name = '';
             }
            
              expDebug::dPrint('$$surname name = '.print_r($surname,true),5);?>
 <div class="para">
    <div class="code-container">
        <span class="detail-code"><?php print t("Survey"); ?>:</span>
         <span class="detail-desc detail-single-line"><span class="vtip single-line-lbl-val" title="<?php print sanitize_data($survey_name); ?>"> <?php print titleController('NARROW-CLASSVIEW-DETAILDESC-SURVEY-TITLE', $survey_name,90); ?>  </span></span>  
    </div>
 </div>
 <?php $assessmentName = getAssessmentName($record->classid,'cre_sys_obt_cls');?>
 <div class="para">
   <?php if(empty($assessmentName)) { ?>
        <span class="detail-title"><?php print t("Assessment");?>:</span>
            <div class ="nonlist"><?php print t('MSG403')?></div>
    <?php } else {?>
        <div class="code-container">
            <span class="detail-code"><?php print t('Assessment'); ?>:</span>
                <span class="detail-desc">
       <table cellpadding="0" class="class-session-details" cellspacing="0" width="100%" border="0" style="margin-bottom: 5px;">
        <tr>
                <th width="75%"><?php print t("LBL083"); ?></th>
                <th width="25%"><?php print t("LBL202"); ?></th>
            </tr>
         <?php  $i=0;
             foreach($assessmentName as $assessInfo) {
                  if(($i%2) == 0){
                $class='table-detail-tr-even';
            }else{
                $class='table-detail-tr-odd';
            }?>
        <tr class="<?php print $class;?>">
            <td valign="top" class="class-detail-session-name"><span class="vtip" title="<?php print sanitize_data($assessInfo->title); ?>"> <?php print titleController('NARROW-CLASSVIEW-DETAILDESC-ASSESSINFO-TITLE', $assessInfo->title,90); ?>  </span></span></td>
            <td valign="top" width="20%"><?php print $assessInfo->attempts; ?></td>
     </tr>
    <?php $i++;
     } ?>
    </table>
   </span>
    </div>
    <?php }?>
 </div> 
  <div class="para">
 <?php if($record->clsdeliverytype == 'lrn_cls_dty_wbt' || $record->clsdeliverytype == 'lrn_cls_dty_vod') {?>
  <?php $contentName= getContentTitleForView($record->classid);
    expDebug::dPrint('$contentName-->'.print_r($contentName,true),4);
  if(empty($contentName)) { ?>
        <span class="detail-title"><?php print t("Content");?>:</span>
                        <div class ="nonlist"><?php print t('MSG403')?></div>
    <?php } else {?>
        <div class="code-container">
            <span class="detail-code cont-deta"><?php print t("Content").' '.t('LBL272'); ?>:</span>
            <span class="detail-desc">
   <table cellpadding="0" class="class-session-details" cellspacing="0" width="100%" border="0" style="margin-bottom: 5px;">
    <tr>
            <th><?php print t("Content"); ?></th>
            <th><?php print t("LBL854"); ?></th>
            <th><?php print t("LBL857"); ?></th>
            <th><?php print t("LBL604"); ?></th>
        </tr>
         <?php  $i=0;
                foreach($contentName as $contInfo) {
                expDebug::dPrint('$contentName--->'.print_r($contInfo,true),5);
                //Commented for #0064996
                //$lessonCnt = getCountOfLessonView($record->classid);
                // expDebug::dPrint('$$lessonCnt--->'.print_r($lessonCnt,true),5);
              if(($i%2) == 0){
            $class='table-detail-tr-even';
        }else{
            $class='table-detail-tr-odd';
    }?>
        <tr class="<?php //print $class;?>">
            <td valign="top" class="class-detail-session-name"><span class="vtip" title="<?php print sanitize_data($contInfo->title); ?>"> <?php print titleController('NARROW-CLASSVIEW-DETAILDESC-CONTINFO-TITLE', $contInfo->title,50); ?>  </span></span></td>
            <td valign="top" width="15%"><?php print $contInfo->lesson;?></td>
            <td valign="top" width="15%"><?php print $contInfo->attempts; ?></td>
            <td valign="top" width="15%"><?php print $contInfo->validity; ?></td>
     </tr>
    <?php $i++;
     } ?>
    </table>
   </span>
    </div>
        <?php } ?>
 <?php }?>
</div> 
 <?php  if(isset($record->sessionDetailInfo)) { 
        expDebug::dPrint('$$record->sessionDetailInfo--->'.print_r($record->sessionDetailInfo,true),5); ?>
    <div class="para">
    <?php if(empty($record->sessionDetailInfo)) { ?>
        <span class="detail-title"><?php print t("LBL249");?>:</span>
                        <div class ="nonlist"><?php print t('MSG499')?></div>
                    <?php } else {?>
        <div class="code-container">
            <span class="detail-code"><?php print t("LBL249"); ?>:</span>
                <span class="detail-desc">
                    <table cellpadding="0" class="class-session-details" cellspacing="0" width="100%" border="0" style="margin-bottom: 5px;">
                    <tr>
                    <th><?php print t("LBL250"); ?></th>
                    <th><?php print t("LBL042"); ?></th>
                    <th><?php print t("LBL251"); ?></th>
                    <th><?php print t("LBL252"); ?></th>
                    </tr>
                    <?php  $i=0;
                     foreach($record->sessionDetailInfo as $sessionInfo) { 
                         if(($i%2) == 0){
                        $class='table-detail-tr-even';
                    }else{
                        $class='table-detail-tr-odd';
                    }?>
                    <tr class="<?php print $class;?>">
                    <?php 
                    if($record->clsdeliverytype == 'lrn_cls_dty_ilt'){
                        $sess_start_time = (strlen($sessionInfo["ilt_start_time"]) == 4) ? str_pad($sessionInfo["ilt_start_time"],5,0,STR_PAD_LEFT) : $sessionInfo["ilt_start_time"];
                        $sess_end_time = (strlen($sessionInfo["ilt_end_time"]) == 4) ? str_pad($sessionInfo["ilt_end_time"],5,0,STR_PAD_LEFT) : $sessionInfo["ilt_end_time"];
                        $ses_st_time_format =  $sessionInfo["ilt_start_time_format"];
                        $ses_end_time_format = $sessionInfo["ilt_end_time_format"];
                        $startdate = $sessionInfo["ilt_start_date"];
                    }else{
                        $sess_start_time = (strlen($sessionInfo["start_time"]) == 4) ? str_pad($sessionInfo["start_time"],5,0,STR_PAD_LEFT) : $sessionInfo["start_time"];
                        $sess_end_time = (strlen($sessionInfo["end_time"]) == 4) ? str_pad($sessionInfo["end_time"],5,0,STR_PAD_LEFT) : $sessionInfo["end_time"];
                        $ses_st_time_format =  $sessionInfo["start_time_format"];
                        $ses_end_time_format = $sessionInfo["end_time_format"];
                        $startdate = $sessionInfo["start_date"];
                    }
                    ?>
                    <td valign="top" class="class-detail-session-name vtip" title="<?php print sanitize_data($sessionInfo["title"]); ?>"><?php print titleController('ADMIN-VIEW-SESSION-TITLE', $sessionInfo["title"]); ?></td>
                    <td valign="top" width="15%"><?php print $startdate; ?></td>
                    <td valign="top" width="15%"><?php print $sess_start_time; ?><span class="time-zone-format"><?php print $ses_st_time_format; ?></span></td>
                    <td valign="top" width="15%"><?php print $sess_end_time; ?><span class="time-zone-format"><?php print $ses_end_time_format; ?></span></td>
                    </tr>
                    <?php $i++;
                     } ?>
                    </table>
            </span>
        </div>
        <?php }?>
    </div>
<?php if($record->clsdeliverytype == 'lrn_cls_dty_ilt') {?>
    <div class="para">
        <div class="code-container">
         <?php  
                $sessinc = 1;
                foreach($record->sessionDetailInfo as $sessionInfo) {
                if($sessinc==1){ 
                $sessinc++;
                if(!empty($record->locationname))
                    $sessAddDet = $record->locationname; 
                if(!empty($sessionInfo["session_address1"])){
                    $sessAddDet .= ",<br>".$sessionInfo["session_address1"];
                }if(!empty($sessionInfo["session_address2"])){
                    $sessAddDet .= ",<br>".$sessionInfo["session_address2"];
                }if(!empty($sessionInfo["session_city"])){
                    $sessAddDet .= ",<br>".$sessionInfo["session_city"];
                }if(!empty($sessionInfo["session_state"])){
                    $sessAddDet .= ", ".$sessionInfo["session_state"];
                }if(!empty($sessionInfo["session_country"])){
                    $sessAddDet .= ",<br>".$sessionInfo["session_country"];
                }if(!empty($sessionInfo["session_zipcode"])){
                    $sessAddDet .= "&nbsp;".$sessionInfo["session_zipcode"];
                }
                print "<span class='detail-code'>".t("Location").":</span><span class='detail-desc'>".$sessAddDet;
                }
                } 
                ?>  
            </span>
        </div>
    </div>
  <?php }  ?>
<?php } ?>
 <div class="para">
    <div class="code-container">
     <span class="detail-code detail-single-line-enrollment">
     <div class='enrollment-tab-icon vtip' title=<?php print t('LBL275')?>> </div>
      <div class="label-separator">
      <?php $entityId = $record->classid; 
      $qtipOptEnrollmentObj  = "{'entityId':".$entityId.",'url':'administration/learning/catalog/view-screen/".$entityId."','popupDispId':'qtip_enrollment_disp_".$entityId."','wid':480,'heg':'280','postype':'middle','posrwid':'115','qdis':'ctool','linkid':'view-enrollments-".$entityId."'}";
      ?>
      <div id="enrollment-class-list-view" class=""><div class="enroll-tab-icon"></div>
        <div id="qtip_enrollment_disp_<?php print $entityId;?>" class="view-enrollment-popup">
                <a id="view-enrollments-<?php print $entityId;?>" class="tab-title enroll-label" onclick="$('body').data('mulitselectdatagrid').loadViewEnrollmentGrid(<?php print $qtipOptEnrollmentObj?>);">
                    <?php print t('LBL275');?>
                </a>
            <span id="visible-popup-<?php print $entityId;?>" style='display:none; position:absolute; left:0px; top:0;'></span>
            </div>
        </div>
         </div>
     </span>
    </div>
 </div>
  <?php 
  if(!module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status
        $custom = getCustomFieldsForView($record->classid, $type);
        expDebug::dPrint(' $$custom'.print_r( $custom,true),5); 
        foreach($custom as $customInfo) :?>
     <div class="para">
        <div class="code-container custom-field-container">
            <span class="detail-code"><span class="vtip" title="<?php print sanitize_data($customInfo->label); ?>"><?php print $customInfo->label; ?></span>:</span>
            <span  id="custom-detail-desc" class="detail-desc"><?php print $customInfo->value; ?></span>
        </div>   
     </div>
     <?php endforeach;
 } //#custom_attribute_0078975 - End Check module status ?> 
 
 </div>
 <?php  drupal_add_js('jQuery(document).ready(function () {console.log("test"); $(".limit-title").trunk8(trunk8.profile_title);$(".limit-desc").trunk8(trunk8.profile_desc); });', 'inline'); ?>
<script>
 vtip();
</script>