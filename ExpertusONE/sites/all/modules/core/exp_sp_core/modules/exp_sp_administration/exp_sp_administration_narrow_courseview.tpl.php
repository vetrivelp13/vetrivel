
<?php 
global $theme_key;
expDebug::dPrint('enter the function sasasasAAAA'.print_r($theme_key,true),4);
?>
<div class="top-record-div-left">
 <div class="para">
  <div class="code-container">
       <span class="detail-code"><?php print t("LBL083"); ?>:</span>
       <span class="detail-desc limit-title-row"><span class="vtip limit-title" title="<?php print sanitize_data($results['catalog']->title); ?>"><?php print $results['catalog']->title; ?>  </span></span>
    </div>
 </div>
 <div class="left-side-container">
     <div class="para">
         <div class="code-container">
           <span class="detail-code"><?php print t("LBL228"); ?>:</span>
           <span class="detail-desc detail-single-line"><span class="vtip single-line-lbl-val" title="<?php print sanitize_data($results['catalog']->code); ?>"><?php  print titleController('ADMIN-NARROW-SEARCH-COURSEVIEW-CATALOG-CODE', $results['catalog']->code,65); ?>  </span></span>
         </div>
        </div>
    </div>
 <div class="right-side-container">
        <div class="para">
         <div class="code-container">
           <span class="detail-code"><?php print t("LBL038"); ?>:</span>
           <span  class="detail-desc"><?php print t($results['catalog']->lang); ?></span>
        </div>
    </div>
 </div>
 <div class="para">
     <div class="code-container">
       <span class="detail-code"><?php print t("LBL229"); ?>:</span>
       <span id="course-detail-desc-view" class="detail-desc"><?php print $results['catalog']->description; ?></span>
    </div>
</div>
 <div class="left-side-container">
     <div class="para">
            <div class="code-container">
                <span class="detail-code"><?php print t("LBL040"); ?>:</span>
                 <span class="detail-desc"><span class="vtip" title="<?php print $results['catalog']->currency_symbol; ?>"><?php print $results['catalog']->currency_symbol; ?>  </span>
                 <span class="vtip" title="<?php print $results['catalog']->price; ?>"><?php print $results['catalog']->price; ?>  </span></span>                                                                           
            </div>
    </div>
<?php /*if ($results['catalog']->is_compliance == 1) { */?> 
    <div class="para">
        <div class="code-container">
            <span class="detail-code"><?php print t("LBL234"); ?>:</span>
             <span class="detail-desc">
             <?php  $compliance_type = '';
              if($results['catalog']->compliance_completed == "days"){
                $compliance_type = t('LBL3083');
              }elseif($results['catalog']->compliance_completed == "hire_days"){
                $compliance_type = t('LBL3084');
              }
              if(!empty($results['catalog']->complete_days)){ print ($results['catalog']->complete_days.' '.t('LBL605').' '.$compliance_type);
                            }elseif(!empty($results['catalog']->complete_date)){ print date_format(date_create($results['catalog']->complete_date),'M d, Y') ;
                    } ?>                                            
             </span>                                    
        </div>
  </div>
 <?php /* } */ ?>
 </div>
 <div class="right-side-container">
  <div class="para">
        <div class="code-container">
            <span class="detail-code"><?php print t("Compliance"); ?>:</span>
             <span class="detail-desc">
             <?php print ($results['catalog']->is_compliance == 1) ? t("Yes") : t("No"); ?>                                             
             </span>                                    
        </div>
  </div>
  <?php if ($results['catalog']->is_compliance == 1) { ?>
   <div class="para">
        <div class="code-container">
            <span class="detail-code"><?php print t("LBL604"); ?>:</span>
             <span class="detail-desc">
            <?php if(!empty($results['catalog']->validity_days)){ print ($results['catalog']->validity_days.' '.t('LBL605'));
                            }elseif(!empty($results['catalog']->validity_date)){ print date_format(date_create($results['catalog']->validity_date),'M d, Y') ;
                    } ?>     
             </span>                                    
        </div>
  </div>
  <?php } ?>
 </div>
     
<?php $tags  = getCatalogTags($results['catalog']->id,'Course');
    $tagString  = '';
    $vtip_class_tags = '';
    if(!empty($tags)){
      $tagString  = implode(", ",$tags);
      $vtip_class_tags = ($tagString != '') ? 'vtip':'';
    }
?>

<div class="left-side-container">
     <div class="para">
            <div class="code-container">
                <span class="detail-code auth-vend"><?php print t("LBL269"); ?>:</span>
                 <span class="detail-desc detail-single-line"><span class="vtip single-line-lbl-val" title="<?php print sanitize_data($results['catalog']->author); ?>"><?php print titleController('ADMIN-NARROW-SEARCH-COURSEVIEW-CATALOG-AUTHOR-TITLE', $results['catalog']->author,55); ?>  </span></span>                                                                          
            </div>
    </div> 
 </div>
 <?php 
 if(module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status/Get Custom Attributes for Course View
    include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'/exp_sp_administration_customattribute_renderinforms.inc');
    print getCustomAttributesForView($results['catalog']->id, 'cre_sys_obt_crs');
  } //#custom_attribute_0078975 - End Check module status
?>
 <div class="para">
     <div class="code-container">
       <span class="detail-code"><?php print t("LBL191"); ?>:</span>
       <span id="course-detail-desc-view" class="detail-desc detail-single-line">
        <span class="single-line-lbl-val <?php print $vtip_class_tags; ?>" title="<?php print sanitize_data($tagString); ?>"> 
        <?php  print titleController('ADMIN-NARROW-SEARCH-COURSEVIEW-TAG-STRING', $tagString,100) ?>  </span>  
       </span>
    </div>
 </div>
 <?php $access  = getGroupNamesForView($results['catalog']->id, 'cre_sys_obt_crs');
  $accessName  = '';
    if(!empty($access)){
      $accessName  = implode(", ",$access);
      $vtip_class_access = ($accessName != '') ? 'vtip':'';
    }
    ?>
 <div class="para">
    <div class="code-container">
        <span class="detail-code"><?php print t("LBL642"); ?>:</span>
        <span id= "access-detail-desc" class="detail-desc detail-single-line"><span class="single-line-lbl-val <?php print $vtip_class_access; ?>" title="<?php print sanitize_data($accessName); ?>"> <?php ($theme_key == 'expertusoneV2') ? print titleController('ADMIN-NARROW-SEARCH-COURSEVIEW-ACCESS-NAME', $accessName,107) : print titleController('ADMIN-NARROW-SEARCH-COURSEVIEW-ACCESS-NAME', $accessName,85) ; ?>  </span>&nbsp;&nbsp;</span>                                  
    </div>
 </div>
 <?php 
 $entityTypeForCourse = 'cre_sys_obt_crs';
 $attachedRecord = getAttachmentsInfoView($results['catalog']->id, $entityTypeForCourse);
 $numAttachments = count($attachedRecord);
?>
 <div class="para">
    <div class="code-container">
        <span class="detail-code"><?php print t("LBL231"); ?>:</span>
             <span class="detail-desc detail-single-line">
             <?php for ($i = 0; $i < $numAttachments; $i++) { ?>
           <?php /*print ($i == 0)?'':'<span class="divider-pipeline">|</span>';?> <?php echo $attachedRecord[$i]->title;*/?>
           <?php $attachment_array[] = sanitize_data($attachedRecord[$i]->title);
            }
            $attachments_implode = implode('<span class="divider-pipeline">|</span>',$attachment_array);
            $attachments_implode_vtip = implode(', ',$attachment_array);
            $vtip_class_att = ($attachments_implode_vtip != '') ? 'vtip':'';
           ?>
          <span class="single-line-lbl-val <?php print $vtip_class_att; ?>" title="<?php print $attachments_implode_vtip; ?>"> <?php  print titleController('ADMIN-NARROW-SEARCH-COURSEVIEW-ATTACHMENTS-NAME', $attachments_implode,100); ?></span></span>
    </div>
 </div>
 <?php $certificateName = getCertificateforCourseView($results['catalog']->id);
       $vtip_class_cer = ($certificateName != '') ? 'vtip':''; ?>
 <div class="para">
    <div class="code-container">
        <span class="detail-code"><?php print t("LBL205"); ?>:</span>
         <span class="detail-desc detail-single-line"><span class="single-line-lbl-val <?php print $vtip_class_cer; ?>" title="<?php print sanitize_data($certificateName); ?>"> <?php print titleController('ADMIN-NARROW-SEARCH-COURSEVIEW-CERTIFICATE-NAME', $certificateName,100); ?></span>                                
    </div>
 </div>
<?php if (!empty($results['preRequiste'])): ?>
 <div class="para">
     <div class="code-container prereq">
        <span class="detail-code"><?php print t("LBL230"); ?>:</span>
     <span class="detail-desc">
      <?php $preCount = count($results['preRequiste']);?>
      <?php if($preCount > 1):?>
     <ul>
      <?php foreach ($results['preRequiste'] as $row): ?> 
     <li>
    <span class="head"><span class="vtip" title="<?php print sanitize_data($row->name); ?>"> <?php print titleController('ADMIN-NARROW-SEARCH-COURSEVIEW-PREREQUISTE-MORETHANONE-NAME', $row->name,70); ?>  </span> </span> <span class = "separator"> - </span> 
      <span class="desc"><span class="vtip" title="<?php print sanitize_data($row->code); ?>"><?php print titleController('ADMIN-NARROW-SEARCH-COURSEVIEW-PREREQUISTE-MORETHANONE-CODE', $row->code,20); ?>  </span></span>
     </li>
      <?php endforeach; ?>
     </ul>
      <?php else :?>
      <?php foreach ($results['preRequiste'] as $row): ?> 
       <span class="head"><span class="vtip" title="<?php print sanitize_data($row->name); ?>"> <?php print titleController('ADMIN-NARROW-SEARCH-COURSEVIEW-PREREQUISTE-NAME', $row->name,70); ?>  </span> </span> <span class = "separator"> - </span> 
       <span class="desc"><span class="vtip" title="<?php print sanitize_data($row->code); ?>"><?php print titleController('ADMIN-NARROW-SEARCH-COURSEVIEW-PREREQUISTE-CODE', $row->code,20); ?>  </span></span>
      <?php endforeach; ?>
      <?php endif;?>  
     </span>                      
    </div>
 </div>
<?php endif; ?>
<?php if (!empty($results['Equivalence'])): ?>
 <div class="para">
     <div class="code-container equvi">
        <span class="detail-code"><?php print t("LBL279"); ?>:</span>
     <span class="detail-desc">
      <?php $preCount = count($results['Equivalence']);?>
      <?php if($preCount > 1):?>
     <ul>
      <?php foreach ($results['Equivalence'] as $row): ?> 
     <li>
    <span class="head"><span class="vtip" title="<?php print sanitize_data($row->name); ?>"> <?php print titleController('ADMIN-NARROW-SEARCH-COURSEVIEW-EQUIVALENCE-MORETHANONE-NAME', $row->name,70); ?>  </span> </span> <span class = "separator"> - </span> 
      <span class="desc"><span class="vtip" title="<?php print sanitize_data($row->code); ?>"><?php print titleController('ADMIN-NARROW-SEARCH-COURSEVIEW-EQUIVALENCE-MORETHANONE-CODE', $row->code,20); ?>  </span></span>
     </li>
      <?php endforeach; ?>
     </ul>
      <?php else :?>
      <?php foreach ($results['Equivalence'] as $row): ?> 
       <span class="head"><span class="vtip" title="<?php print sanitize_data($row->name); ?>"> <?php print titleController('ADMIN-NARROW-SEARCH-COURSEVIEW-EQUIVALENCE-NAME', $row->name,70); ?>  </span>  </span>  <span class = "separator" > - </span>
       <span class="desc"><span class="vtip" title="<?php print sanitize_data($row->code); ?>"><?php print titleController('ADMIN-NARROW-SEARCH-COURSEVIEW-EQUIVALENCE-CODE', $row->code,20); ?>  </span></span>
      <?php endforeach; ?>
      <?php endif;?>  
     </span>                      
    </div>
 </div>
<?php endif; ?>

 <?php  
 if(!module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status
         
         $custom = getCustomFieldsForView($results['catalog']->id, $type);
        expDebug::dPrint(' $$custom'.print_r( $custom,true),5);?>
    <?php foreach($custom as $customInfo) :?>
     <div class="para">
            <div class="code-container custom-field-container">
                 <span class="detail-code"><span class="vtip" title="<?php print sanitize_data($customInfo->label); ?>"><?php print $customInfo->label; ?>  </span></span>
                 <span  id="custom-detail-desc" class="detail-desc"><?php print sanitize_data($customInfo->value); ?></span>
            </div>   
     </div>
         <?php endforeach;
   } //#custom_attribute_0078975 - End Check module status ?>  
 
 <div id="course-details-display-content">
 <div id ="course-details-data">
  <div class="para">
    <div>
      <input id='courseId' type=hidden value="<?php print core_encrypt($results['catalog']->id) ;?>" />
      <input id='userId' type=hidden value="<?php global $user; print core_encrypt($user->uid); ?>" />
    </div>
    <!--  Place holder table and div for jqgrid of classes list and pager -->
     <div id="class-list-loader"></div>
       <table id ="paint-classes-list">    
        <div id="class-details-displayloader"></div>
       </table>
     </div>
 </div>
 </div>
</div>
<?php  drupal_add_js('jQuery(document).ready(function () {console.log("load tpl"); $(".limit-title").trunk8(trunk8.profile_title);$(".limit-desc").trunk8(trunk8.profile_desc); });', 'inline'); ?>