<?php 
expDebug::dPrint('enter the function sasasas1111'.print_r($record,true),4);
expDebug::dPrint('enter the function sasasasAAAA'.$record->classid,4);
global $theme_key;
?>
<div class="top-record-div-left">
 <div class="para">
  <div class="code-container">
        <span class="detail-code"><?php print t("LBL083"); ?>:</span>
         <span class="detail-desc limit-title-row"><span class="vtip limit-title" title="<?php print sanitize_data($record[0]->title); ?>"> <?php print $record[0]->title; ?>  </span></span>
    </div>
 </div>
 <div class="left-side-container">
     <div class="para">
        <div class="code-container">
            <span class="detail-code"><?php print t("LBL096"); ?>:</span>
             <span class="detail-desc detail-single-line"><span class="vtip single-line-lbl-val" title="<?php print sanitize_data($record[0]->code); ?>"> <?php print titleController('ADMIN-NARROW-TPVIEW-CODE', $record[0]->code,65); ?>  </span></span>
        </div>
     </div>
 </div>
 <div class="right-side-container">
     <div class="para">
        <div class="code-container">
            <span id="tp-right-view" class="detail-code"><?php print t("LBL038"); ?>:</span>
             <span class="detail-desc">
             <?php print t($record[0]->lang); ?>                                            
             </span>                                    
        </div>
     </div>
 </div>
 <div class="para">
    <div class="code-container">
        <span class="detail-code"><?php print t("LBL229"); ?>:</span>
         <span id="tp-desc-view" class="detail-desc">
         <?php print $record[0]->description; ?>                                                
         </span>                                    
    </div>
 </div>
 
                                    
<div class="para">
       <div class="code-container additionalnotes">
               <span class="detail-code-addinformation"><?php print t("LBL3068"); ?>:</span>
               <span class="detail-desc-addinformation"><?php print $record[0]->additional_info;?></span>
                
       </div>
        <div  class="para">
            <?php if($record[0]->addn_catalog_show != 1){
            $add_margin = 'set-addinformationtp';
        }

            ?>
            <?php if($record[0]->addn_catalog_show == 1){
            ?>
            <div class="detail-desc-cata-notes-tp">

                <?php print t("Shown in Catalog");
                ?>
            </div>
            <?php }
            ?>
            <?php if($record[0]->addn_catalog_show == 1 && $record[0]->addn_notification_show == 1){
            ?>
            <div class="add-info-detail-pipe-line <?php echo $add_margin; ?>">
                |
            </div>
            <?php }
            ?>
            <?php if($record[0]->addn_notification_show == 1){
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
            <span class="detail-code"><?php print t("LBL037").' '.t("LBL036"); ?>:</span>
             <span class="detail-desc">
             <?php print t($record[0]->pro_type); ?>                                                
             </span>                                    
        </div>
     </div>
     <div class="para">
        <div class="code-container">
            <span class="detail-code"><?php print t("Enforce Sequencing"); ?>:</span>
             <span  class="detail-desc">
             <?php ($record[0]->sequence == 'Y') ? print t('Yes'):print t('No'); ?>                                             
             </span>                                    
     </div>
  </div>
   <?php if($record[0]->pro_code == 'cre_sys_obt_crt'): ?>
   <?php if(!empty($record[0]->expires_in_value)): ?>
      <div class="para">
       <div class="code-container expires">
          <span class="detail-code"><?php print t("LBL233"); ?>:</span>
            <span id="course-detail-desc" class="detail-desc">
       <?php print $record[0]->expires_in_value_str . '&nbsp;' . t('MSG568'); ?>
        </span>
      </div>
     </div>
 <?php endif; ?>
 <?php elseif($record[0]->pro_code == 'cre_sys_obt_trn'): ?>
      <div class="para">
       <div class="code-container complete">
        <span class="detail-code"><?php print t("LBL234"); ?>:</span>
         <span id="course-detail-desc" class="detail-desc"><?php print date_format(date_create($record[0]->end_date),'M d, Y'); ?></span>
       </div>
     </div>
    <?php endif; ?>
 </div> 
 <div class="right-side-container">
  <div class="para">
        <div class="code-container">
            <span id="tp-right-view" class="detail-code"><?php print t("LBL040"); ?>:</span>
             <span  class="detail-desc">
             <?php print $record[0]->currency_symbol.' '.$record[0]->price; ?>                                              
             </span>                                    
        </div>
   </div>
   <div class="para">
        <div class="code-container">
            <span id="tp-right-view" class="detail-code auth-vend"><?php print t("LBL269"); ?>:</span>
             <span class="detail-desc detail-single-line"><span class="vtip single-line-lbl-val" title="<?php print sanitize_data($record[0]->author); ?>"><?php ($theme_key == 'expertusoneV2')? print titleController('ADMIN-NARROW-TPVIEW-AUTHOR', $record[0]->author,13) : print titleController('ADMIN-NARROW-TPVIEW-AUTHOR', $record[0]->author,11); ?>  </span></span>                                               
        </div>
  </div>
 </div>
 <?php 
    if(module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status/Get Custom Attributes for Class View
        include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'/exp_sp_administration_customattribute_renderinforms.inc');
        print getCustomAttributesForView($record[0]->id, 'cre_sys_obt_trp');
    } //#custom_attribute_0078975 - End Check module status
    
  $tags  = getCatalogTags($record[0]->id,$record[0]->pro_type);
  expDebug::dPrint('$tags--->'.print_r($tags,true),5);
    $tagString  = '';
    if(!empty($tags)){
      $tagString  = implode(", ",$tags);
    } ?>
 <div class="para">
    <div class="code-container">
        <span class="detail-code"><?php print t("LBL191"); ?>:</span>
         <span class="detail-desc detail-single-line"><span class="vtip single-line-lbl-val" title="<?php print sanitize_data($tagString); ?>"> <?php print titleController('ADMIN-NARROW-TPVIEW-TAGS', $tagString,90); ?>  </span></span>                                      
    </div>
 </div>
  <?php 
  $entityTypeForTraningPlan = 'cre_sys_obt_trp';
  $attachedRecord = getAttachmentsInfoView($record[0]->id,$entityTypeForTraningPlan);
  expDebug::dPrint('$attachedRecord--->'.print_r($attachedRecord,true),5);
    $numAttachments = count($attachedRecord);
    ?>
<div class="para">
    <div class="code-container">
        <span class="detail-code"><?php print t("LBL231"); ?>:</span>
             <span id="tp-desc-view" class="detail-desc detail-single-line">
             <?php for ($i = 0; $i < $numAttachments; $i++) { ?>
                 <?php $attachment_array[] = $attachedRecord[$i]->title;
            }
            $attachments_implode = implode('<span class="divider-pipeline">|</span>',$attachment_array);
            $attachments_implode_vtip = implode(', ',$attachment_array);
           ?>
          <span class="vtip single-line-lbl-val" title="<?php print sanitize_data($attachments_implode_vtip); ?>"> <?php  print titleController('ADMIN-NARROW-TPVIEW-ATTACHMENTS-NAME', $attachments_implode,100); ?></span></span>
    </div>
 </div>
<?php $access  = getGroupNamesForView($record[0]->id, $record[0]->object_type);
expDebug::dPrint('$access'.print_r($access,true),4);
  $accessName  = '';
    if(!empty($access)){
      $accessName  = implode(", ",$access);
    }
    ?>
 <div class="para">
    <div class="code-container">
        <span class="detail-code"><?php print t("LBL642"); ?>:</span>
        <span id="tp-desc-view" class="detail-desc detail-single-line"><span class="vtip single-line-lbl-val" title="<?php print sanitize_data($accessName); ?>"> <?php ($theme_key == 'expertusoneV2')? print titleController('ADMIN-NARROW-TPVIEW-ACCESS', $accessName,90) : print titleController('ADMIN-NARROW-TPVIEW-ACCESS', $accessName,80) ; ?>  </span></span>                                     
    </div>
 </div>
 <?php if (!empty($record[0]->prerequisites)): ?>
    <div class="para">
        <div class="code-container prereq">
            <span class="detail-code"><?php print t("LBL230"); ?>:</span>

            <span class="detail-desc">
            <?php //$preCount = count($block->results['learning'][0]->prerequisites);?>
            <ul>
                <?php foreach ($record[0]->prerequisites as $row): ?>
                <li> 
                    <span id="lrnplan-detail-preqcrs-desc" class="head"> <span class="vtip" title="<?php print sanitize_data($row->pe_title); ?>"> <?php print titleController('ADMIN-NARROW-TPVIEW-PREREQUISITE-TITLE',  $row->pe_title,50); ?> </span></span> 
                    <span id="lrnplan-detail-preqcode-desc" class="desc"> <span class="vtip" title="<?php print sanitize_data($row->pe_code); ?>"> -  <?php print titleController('ADMIN-NARROW-TPVIEW-PREREQUISITE-CODE',  $row->pe_code,20); ?> </span>  </span> 
                    <span id="lrnplan-detail-preqstyle-desc" class="type"> - <?php print titleController('ADMIN-NARROW-TPVIEW-PREREQUISITE-OBJECTTYPE',$row->pe_object_type);?></span> 
                </li>
                <?php endforeach; ?>
            </ul>
            </span>
        </div>
    </div>
<?php endif; ?>
 <?php $surveyName = getSurveyName($record[0]->id,$record[0]->object_type);?>
  <div class="para">
  <?php 
            /* $surname = '';
             expDebug::dPrint('$$surname = '.print_r($surveyName,true),5);
             if(!empty($surveyName)){
                $surname  = implode(", ",$surveyName);
            <?php } else {?>*/
      if(empty($surveyName)) { ?>
            <span class="detail-title"><?php print t("Survey");?>:</span>
                <div class ="nonlist"><?php print t('MSG403')?></div>
        <?php } else {?>

    <div class="code-container">
        <span class="detail-code"><?php print t("Survey"); ?>:</span>
        <!-- <span class="detail-desc"><span class="vtip" title="<?php //print sanitize_data($surname); ?>"> <?php //print titleController('ADMIN-NARROW-TPVIEW-SURVEYNAME', $surname,90); ?>  </span>&nbsp;&nbsp;</span> -->                                   
    <span class ="detail-desc" style="clear: both;">
    <table cellpadding="0" class="class-session-details" cellspacing="0" width="100%" border="0" style="margin-bottom: 5px;">
        <tr>
                <th><?php print t("LBL083"); ?></th>
                <th><?php print t("MODULE PATH"); ?></th>
            </tr>
            <?php  $i=0;
             foreach($surveyName as $surname) {
                expDebug::dPrint('MY SURVEYYYY'.print_r($surname,true),5);
                  if(($i%2) == 0){
                $class='table-detail-tr-even';
            }else{
                $class='table-detail-tr-odd';
            }?>
            <tr class="<?php //print $class;?>">
                <td valign="top" class="class-detail-session-name" width = "25%"><span class="vtip" title="<?php print sanitize_data($surname->title); ?>"> <?php print titleController('ADMIN-NARROW-TPVIEW-SURVEYNAME', $surname->title,15); ?>  </span></td>
                <td valign="top" width="25%"><span class ="vtip" title ="<?php print sanitize_data($surname->path); ?>"> <?php print titleController('ADMIN-NARROW-TPVIEW-DETAIL-MODULE-TITLE',$surname->path,10);?> </span></td>
            </tr>
        <?php $i++;
         } ?>
    </table>
    </span>
    </div>
    <?php }?>
 </div>
  <?php $assessmentName = getAssessmentName($record[0]->id,$record[0]->object_type);?>
 <div class="para">
      <?php if(empty($assessmentName)) { ?>
            <span class="detail-title"><?php print t("Assessment");?>:</span>
                <div class ="nonlist"><?php print t('MSG403')?></div>
        <?php } else {?>
            <div class="code-container">
                <span class="detail-code"><?php print t('Assessment'); ?>:</span>
                    <span class="detail-desc" style="clear: both;">
       <table cellpadding="0" class="class-session-details" cellspacing="0" width="100%" border="0" style="margin-bottom: 5px;">
        <tr>
                <th><?php print t("LBL083"); ?></th>
                <th><?php print t("LBL202"); ?></th>
                <th><?php print t("MODULE PATH"); ?></th>
            </tr>
         <?php  $i=0;
             foreach($assessmentName as $assessInfo) {
                expDebug::dPrint('$contentName--->'.print_r($contInfo,true),5);
                  if(($i%2) == 0){
                $class='table-detail-tr-even';
            }else{
                $class='table-detail-tr-odd';
            }?>
            <tr class="<?php //print $class;?>">
                <td valign="top" class="class-detail-session-name"><span class="vtip" title="<?php print sanitize_data($assessInfo->title); ?>"> <?php print titleController('ADMIN-NARROW-TPVIEW-ASSESSINFO', $assessInfo->title,80); ?>  </span></span></td>
                <td valign="top" class = "ass-attempts-col" width ="25%"><?php print $assessInfo->attempts; ?></td>
                <td valign="top" class = "ass-module-path-col" ><span class ="vtip" title ="<?php print sanitize_data($assessInfo->path); ?>"> <?php print titleController('ADMIN-NARROW-TPVIEW-DETAIL-MODULE-TITLE',$assessInfo->path,10);?></span></span></td>
         </tr>
        <?php $i++;
         } ?>
        </table>
       </span>
        </div>
    <?php }?>
 </div> 
  <div class="para">
    <div class="code-container">
            <!-- <span class="detail-code cour-deta"><?php print t('LBL246'); ?>:</span> -->
                 <?php 
                  $j=0;
                  expDebug::dPrint("record set ". print_r($record,true),5);
                if (count($record['module']) > 0) {
                 foreach($record['module'] as $rec): 
                    $module_title = $rec->module_title;
                    $module_id = $rec->module_id;
                 ?>
                 <div class="module-list-expand-collapse">
                 <div id="detail-code-expand-collapse-<?php print $module_id; ?>" class="title_open" onclick="renderCourseList(<?php print $module_id;?>)"></div>
                <span class="detail-code cour-deta"><span class ="vtip" title ="<?php print sanitize_data($module_title); ?>"> <?php print titleController('ADMIN-NARROW-TPVIEW-DETAIL-MODULE-TITLE',$module_title,10);?>:</span> 
                 <?php if (count($rec->courselist) > 0) {?>
                <span class="detail-desc">
                    <table id ="course-table-<?php print $module_id;?>" border="0" cellpadding="0" class="class-session-details" width="100%" cellspacing="0">
                  <tr>
                                            <th><?php print t("LBL228"); ?></th>
                                            <th><?php print t("LBL083"); ?></th>
                                            <th><?php print t("Group"); ?></th>
                                                <th></th>
                                    </tr>
                                    <?php 
            $modExists = array();
             ?>
             <?php $i=0; 
                  foreach ($rec->courselist as $row): 
                        if(($i%2) == 0){
                                $class='table-detail-tr-even';
                            }else{
                                $class='table-detail-tr-odd';
                            }?> 
                    
                  <tr class="<?php print $class;?>">
                            <td valign="top"class="class-detail-session-name"><span class="vtip" title="<?php print sanitize_data($row->crs_code); ?>"> <?php print titleController('ADMIN-NARROW-TPVIEW-DETAIL-COURSE-CODE', $row->crs_code,50); ?>  </span></td>
                          <td valign="top" class="title-col"><span class="vtip" title="<?php print sanitize_data($row->crs_title); ?>"> <?php print titleController('ADMIN-NARROW-TPVIEW-DETAIL-COURSE-TITLE', $row->crs_title,15); ?> </span></td>
                          <td valign="top" class="group-col"><span class="vtip" title="<?php print sanitize_data($row->group_title); ?>"> <?php print titleController('ADMIN-NARROW-TPVIEW-DETAIL-MODULE-TITLE', $row->group_title,10); ?></span></td>
                          <td valign="top" class="mand-col"><?php ($row->is_required == 'Y')? print t('Mandatory') : print t('Optional'); ?></td>
                  </tr>
          
              <?php $i++;
              endforeach; ?>
                  <?php //} else { ?>
                  <!--  <tr>
                      <td class="no-course-items"><?php //print t("MSG268"); ?></td>
                   </tr>  -->
             
             </table>
          </span>
              <?php } else { ?>
           <span class="detail-desc">
                    <table id ="course-table-<?php print $module_id;?>" border="0" cellpadding="0"  width="100%" cellspacing="0">
               <tr>
                  <td class="no-course-items"><?php print t("MSG268"); ?></td>
               </tr> 
                   </table>
          </span>
          <?php } ?>
          </div>
        <?php $j++; endforeach;?>
         <?php } else { ?>
                     <span class="detail-desc">
                    <table border="0" cellpadding="0" class="class-session-details" width="100%" cellspacing="0">
                   <tr>
                      <td class="no-course-items"><?php print t("MSG268"); ?></td>
                   </tr> 
             </table>   
          </span>
              <?php } ?>
          
       </div>
 <div class="para">
    <div class="code-container">
     <span class="detail-code detail-single-line-enrollment">
        <div class='enrollment-tab-icon vtip' title=<?php print t('LBL275')?>> </div>
       <div class="label-separator">
            <?php $entityId = core_encrypt($record[0]->id); 
                 $qtipOptEnrollmentObj  = "{'entityId':'".$entityId."','type':'".$type."','url':'administration/learning/program/view-screen/".$entityId."','popupDispId':'qtip_enrollment_disp_".$entityId."','wid':500,'heg':'300','postype':'middle','posrwid':'150','qdis':'ctool','linkid':'view-enrollments-".$entityId."'}";
            ?>
                <div id="enrollment-class-list-view" class=""><div class="enroll-tab-icon"></div>
                        <div id="qtip_enrollment_disp_<?php print $entityId;?>" class="view-enrollment-popup">
                            <a id="view-enrollments-<?php print $entityId;?>" class="tab-title enroll-label" onclick="$('body').data('mulitselectdatagrid').loadViewEnrollmentGrid(<?php print $qtipOptEnrollmentObj;?>);">
                            <?php print t('LBL275');?>
                            </a>
                            <span id="visible-popup-<?php print $entityId;?>" style='display:none; position:absolute; left:0px; top:0px;'></span>
                        </div>
                </div>
          </div>
      </span>
     </div>
  </div>
  
  <?php 
    if(!module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status/Get Custom Attributes for Class View
         $custom = getCustomFieldsForView($record[0]->id, $type);
         expDebug::dPrint(' $$custom'.print_r( $custom,true),5); 
         foreach($custom as $customInfo) :?>
     <div class="para">
        <div class="code-container custom-field-container">
                        <span class="detail-code"><span class="vtip"
                            title="<?php print sanitize_data($customInfo->label); ?>"><?php print $customInfo->label; ?></span>:</span>
            <span  id="custom-detail-desc" class="detail-desc"><?php print $customInfo->value; ?></span>
        </div>   
     </div>
         <?php endforeach;
         
    } //#custom_attribute_0078975 - End Check module status ?>   
 
 
</div>
</div>