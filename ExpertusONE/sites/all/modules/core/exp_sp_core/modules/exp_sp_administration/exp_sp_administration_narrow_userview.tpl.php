<?php
expDebug::dPrint('enter the function sasasasAAAA    '.print_r($results,true),4);
expDebug::dPrint('enter the function sasasas'.print_r($type,true),4);
 global $theme_key;
 $sumedit = $results[0]->sumedit;
 if($results[0]->preferred_currency!=''){
    $getCurrencyDetails = getSiteCurrencyCode($results[0]->preferred_currency);
    $my_currency=$getCurrencyDetails[0]->currency_code." ".$getCurrencyDetails[0]->currency_symbol." ".$getCurrencyDetails[0]->currency_name;
 }else{
    $my_currency ='';
 }
?>
<div class="top-record-div-left">
 <div class="left-side-container">
  <div class="para">
   <div class="user-container">
       <span class="userview-label"><?php print t("LBL056"); ?>:</span>
        <span class="user-desc"><span class="vtip" title="<?php print sanitize_data($results[0]->first_name); ?>">
             <?php ($theme_key == 'expertusoneV2') ? print titleController('ADMIN-NARROW-USERVIEW-FNAME', $results[0]->first_name,24) : print titleController('ADMIN-NARROW-USERVIEW-FNAME', $results[0]->first_name,27); ?>
        </span></span>
     </div>
 </div>
  <div class="para">
   <div class="user-container">
       <span class="userview-label"><?php print t("LBL054"); ?>:</span>
        <span class="user-desc"><span class="vtip" title="<?php print sanitize_data($results[0]->user_name); ?>">
             <?php ($theme_key == 'expertusoneV2') ? print titleController('ADMIN-NARROW-USERVIEW-UNAME', $results[0]->user_name,24) : print titleController('ADMIN-NARROW-USERVIEW-UNAME', $results[0]->user_name,27); ?>
        </span></span>
     </div>
  </div>
  <div class="para">
   <div class="user-container">
       <span class="userview-label"><?php print t("LBL061"); ?>:</span>
        <span class="user-desc"><span class="vtip" title="<?php print sanitize_data($results[0]->email); ?>">
             <?php ($theme_key == 'expertusoneV2') ? print titleController('ADMIN-NARROW-USERVIEW-EMAIL', $results[0]->email,24) : print titleController('ADMIN-NARROW-USERVIEW-EMAIL', $results[0]->email,27); ?>
        </span></span>
     </div>
 </div>
 </div>
 <div class="right-side-container">
  <div class="para">
   <div class="user-container">
       <span class="userview-label"><?php print t("LBL058"); ?>:</span>
        <span class="user-desc"><span class="vtip" title="<?php print sanitize_data($results[0]->last_name); ?>">
             <?php ($theme_key == 'expertusoneV2') ? print titleController('ADMIN-NARROW-USERVIEW-LNAME', $results[0]->last_name,24) : print titleController('ADMIN-NARROW-USERVIEW-LNAME', $results[0]->last_name,27); ?>
        </span></span>
    </div>
  </div>
  <div class="para">
   <div class="user-container">
       <span class="userview-label"><?php print t("LBL060"); ?>:</span>
        <span class="user-desc"></span>
    </div>
  </div>
  <div class="para">
   <div class="user-container">
       <span class="userview-label"><?php print t("LBL172"); ?>:</span>
        <span class="user-desc"><span class="vtip" title="<?php print sanitize_data($results[0]->contact); ?>">
             <?php ($theme_key == 'expertusoneV2') ? print titleController('ADMIN-NARROW-USERVIEW-CONTACT', sanitize_data($results[0]->contact),24) : print titleController('ADMIN-NARROW-USERVIEW-CONTACT', sanitize_data($results[0]->contact),27); ?>
        </span></span>
    </div>
  </div>
 </div>
 <div class="para">
   <div class="user-container">
       <span class="userview-label"><?php print t("LBL064"); ?>:</span>
        <span class="user-desc" id="user-addr-desc"><span class="vtip" title="<?php print sanitize_data($results[0]->addr1); ?>">
             <?php ($theme_key == 'expertusoneV2') ? print titleController('ADMIN-NARROW-USERVIEW-ADDR1', sanitize_data($results[0]->addr1),24) : print titleController('ADMIN-NARROW-USERVIEW-ADDR1', sanitize_data($results[0]->addr1),27); ?>
        </span></span>
    </div>
 </div>
 <div class="para">
   <div class="user-container">
       <span class="userview-label"><?php print t("LBL065"); ?>:</span>
        <span class="user-desc" id="user-addr-desc"><span class="vtip" title="<?php print sanitize_data($results[0]->addr2); ?>">
             <?php ($theme_key == 'expertusoneV2') ? print titleController('ADMIN-NARROW-USERVIEW-ADDR2', sanitize_data($results[0]->addr2),24) : print titleController('ADMIN-NARROW-USERVIEW-ADDR2', sanitize_data($results[0]->addr2),27); ?>
        </span></span>
    </div>
 </div>
 <div class="left-side-container">
  <div class="para">
   <div class="user-container">
       <span class="userview-label"><?php print t("LBL066"); ?>:</span>
        <span class="user-desc"><span class="vtip" title="<?php print sanitize_data($results[0]->city); ?>">
             <?php ($theme_key == 'expertusoneV2') ? print titleController('ADMIN-NARROW-USERVIEW-CITY', sanitize_data($results[0]->city),24) : print titleController('ADMIN-NARROW-USERVIEW-CITY', sanitize_data($results[0]->city),27); ?>
        </span></span>
     </div>
  </div>
 <div class="para">
   <div class="user-container">
       <span class="userview-label"><?php print t("LBL039"); ?>:</span>
        <span class="user-desc"><span class="vtip" title="<?php print $results[0]->country; ?>">
             <?php ($theme_key == 'expertusoneV2') ? print titleController('ADMIN-NARROW-USERVIEW-COUNTRY', $results[0]->country,24) : print titleController('ADMIN-NARROW-USERVIEW-COUNTRY', $results[0]->country,27); ?>
        </span></span>
     </div>
  </div>   
  <div class="para">
   <div class="user-container">
       <span class="userview-label"><?php print t("LBL297"); ?>:</span>
        <span class="user-desc"><span class="vtip" title="<?php print sanitize_data($results[0]->timezone); ?>"> <?php print titleController('ADMIN-NARROW-USERVIEW-TIMEZONE', $results[0]->timezone,24); ?>  </span></span>
     </div>
  </div>
  <div class="para">
   <div class="user-container">
       <span class="userview-label"><?php print t("Mobile"); ?>:</span>
        <span class="user-desc"><span class="vtip" title="<?php print sanitize_data($results[0]->mobile); ?>">
             <?php ($theme_key == 'expertusoneV2') ? print titleController('ADMIN-NARROW-USERVIEW-MOBILE', sanitize_data($results[0]->mobile),24) : print titleController('ADMIN-NARROW-USERVIEW-MOBILE', sanitize_data($results[0]->mobile),27); ?>
        </span></span>
     </div>
  </div>
 </div>
  <div class="right-side-container">
    <div class="para">
    <div class="user-container">
         <span class="userview-label"><?php print t("LBL152"); ?>:</span>
          <span class="user-desc"><span class="vtip" title="<?php print $results[0]->state; ?>">
                       <?php ($theme_key == 'expertusoneV2') ? print titleController('ADMIN-NARROW-USERVIEW-STATE', $results[0]->state,24) : print titleController('ADMIN-NARROW-USERVIEW-STATE', $results[0]->state,27); ?>
          </span></span>
       </div>
    </div>
  <div class="para">
   <div class="user-container">
       <span class="userview-label"><?php print t("LBL562"); ?>:</span>
        <span class="user-desc"><span class="vtip" title="<?php print sanitize_data($results[0]->zip); ?>">
             <?php ($theme_key == 'expertusoneV2') ? print titleController('ADMIN-NARROW-USERVIEW-ZIP', sanitize_data($results[0]->zip),24) : print titleController('ADMIN-NARROW-USERVIEW-ZIP', sanitize_data($results[0]->zip),27); ?>
        </span></span>
     </div>
  </div>
  <div class="para">
   <div class="user-container">
       <span class="userview-label"><?php print t("LBL038"); ?>:</span>
        <span class="user-desc"><span class="vtip" title="<?php print $results[0]->language; ?>">
             <?php ($theme_key == 'expertusoneV2') ? print titleController('ADMIN-NARROW-USERVIEW-LANGUAGE', $results[0]->language,24) : print titleController('ADMIN-NARROW-USERVIEW-LANGUAGE', $results[0]->language,27); ?>
        </span></span>
     </div>
  </div>
   <div class="para">
   <div class="user-container">
       <span class="userview-label"><?php print t("LBL101"); ?>:</span>
        <span class="user-desc"><span class="vtip" title="<?php print $results[0]->preferred_currency; ?>">
             <?php ($theme_key == 'expertusoneV2') ? print titleController('ADMIN-NARROW-USERVIEW-CURRENCY', $my_currency,24) : print titleController('ADMIN-NARROW-USERVIEW-CURRENCY', $results[0]->preferred_currency,27); ?>
        </span></span>
     </div>
  </div>
 </div>
  <div class="para">
   <div class="user-container">
            <div class="user-separator"></div>
  </div>
 </div>
  <div class="left-side-container">
   <div class="para">
   <div class="user-container">
       <span class="userview-label"><?php print t("Organization"); ?>:</span>
       <span class="user-desc"><span class="vtip" title="<?php print sanitize_data($results[0]->organization); ?>"> <?php print titleController('ADMIN-NARROW-USERVIEW-ORG', sanitize_data($results[0]->organization),24); ?>  </span></span>
     </div>
  </div>
  <div class="para">
   <div class="user-container">
       <span class="userview-label"><?php print t("LBL174"); ?>:</span>
        <span class="user-desc"><span class="vtip" title="<?php print sanitize_data($results[0]->employment); ?>">
             <?php ($theme_key == 'expertusoneV2') ? print titleController('ADMIN-NARROW-USERVIEW-EMPLOYMENT', sanitize_data($results[0]->employment),24) : print titleController('ADMIN-NARROW-USERVIEW-EMPLOYMENT', sanitize_data($results[0]->employment),27); ?>
        </span></span>
     </div>
  </div>
  <div class="para">
   <div class="user-container">
       <span class="userview-label"><?php print t("LBL179"); ?>:</span>
        <span class="user-desc"><span class="vtip" title="<?php print sanitize_data($results[0]->department); ?>">
             <?php ($theme_key == 'expertusoneV2') ? print titleController('ADMIN-NARROW-USERVIEW-EMPLOYMENT', sanitize_data($results[0]->department),24) : print titleController('ADMIN-NARROW-USERVIEW-EMPLOYMENT', sanitize_data($results[0]->department),27); ?>
        </span></span>
     </div>
  </div>
  <div class="para">
   <div class="user-container job-role">
       <span class="userview-label"><?php print t("LBL133"); ?>:</span>
        <span class="user-desc" title="<?php print sanitize_data($results[0]->jobrole); ?>"><span class="vtip" title="<?php print sanitize_data($results[0]->jobrole); ?>">
             <?php ($theme_key == 'expertusoneV2') ? print titleController('ADMIN-NARROW-USERVIEW-JOBROLES', sanitize_data($results[0]->jobrole),24) : print titleController('ADMIN-NARROW-USERVIEW-JOBROLES', sanitize_data($results[0]->jobrole),27); ?>
        </span></span>
     </div>
  </div>
  <div class="para">
   <div class="user-container">
       <span class="userview-label"><?php print t("LBL579"); ?>:</span>
       <?php $role1 = ($results[0]->is_instructor == 'Y') ? t('Instructor') : '';
             $role2 = ($results[0]->is_manager == 'Y') ? t('Manager') : '';
             if((!empty($role1) && !empty($role2))){
                    $roles =  $role1. ', ' .$role2;
             }else if(empty($role1) && !empty($role2)){
                    $roles = $role2;
             }else if(!empty($role1) && empty($role2)){
                    $roles = $role1;
             }
       ?>
        <span class="user-desc"><span class="vtip" title="<?php print $roles; ?>">
             <?php ($theme_key == 'expertusoneV2') ? print titleController('ADMIN-NARROW-USERVIEW-ROLES', $roles,24) : print titleController('ADMIN-NARROW-USERVIEW-ROLES', $roles,27); ?>
        </span></span>
     </div>
  </div>
  <?php $webex_module=getProfileListItemByCode('lrn_cls_vct_web');
    if($results[0]->is_instructor == 'Y' && variable_get('webex_User') == 1  && $webex_module[0]->status == 'Y'){ ?>
  <div class="para">
   <div class="user-container">
       <span class="userview-label"><?php print t("WebEx").t(" ").t("LBL054"); ?>:</span>
                 
        <span class="user-desc"><span class="vtip" title="<?php print $results[0]->webex_name; ?>">
             <?php ($theme_key == 'expertusoneV2') ? print titleController('ADMIN-NARROW-USERVIEW-WEBEXUSERNAME',sanitize_data($results[0]->webex_name),24) : print titleController('ADMIN-NARROW-USERVIEW-WEBEXUSERNAME', $results[0]->webex_name,27); ?>
        </span></span>
     </div>
  </div>
  <?php } ?>
 </div>
  <div class="right-side-container">
  <div class="para">
   <div class="user-container">
       <span class="userview-label"><?php print t("Manager"); ?>:</span>
        <span class="user-desc"><span class="vtip" title="<?php print sanitize_data($results[0]->manager); ?>">
             <?php ($theme_key == 'expertusoneV2') ? print titleController('ADMIN-NARROW-USERVIEW-MANAGER', sanitize_data($results[0]->manager),24) : print titleController('ADMIN-NARROW-USERVIEW-MANAGER', sanitize_data($results[0]->manager),27); ?>
        </span></span>
     </div>
  </div>
  <div class="para">
   <div class="user-container">
       <span class="userview-label"><?php print t("LBL294"); ?>:</span>
        <span class="user-desc"><span class="vtip" title="<?php print sanitize_data($results[0]->employee_no); ?>">
             <?php ($theme_key == 'expertusoneV2') ? print titleController('ADMIN-NARROW-USERVIEW-EMPLOYEE-NO', sanitize_data($results[0]->employee_no),24) : print titleController('ADMIN-NARROW-USERVIEW-EMPLOYEE-NO', sanitize_data($results[0]->employee_no),27); ?>
        </span></span>
     </div>
  </div>
  <div class="para">
   <div class="user-container">
       <span class="userview-label"><?php print t("LBL073"); ?>:</span>
        <span class="user-desc"><span class="vtip" title="<?php print sanitize_data($results[0]->jobtitle); ?>">
             <?php ($theme_key == 'expertusoneV2') ? print titleController('ADMIN-NARROW-USERVIEW-JOBTITLE', sanitize_data($results[0]->jobtitle),24) : print titleController('ADMIN-NARROW-USERVIEW-JOBTITLE', sanitize_data($results[0]->jobtitle),27); ?>
        </span></span>
     </div>
  </div>
  <div class="para">
   <div class="user-container">
       <span class="userview-label"><?php print t("LBL173"); ?>:</span>
        <span class="user-desc"><span class="vtip" title="<?php print sanitize_data($results[0]->usertype); ?>">
             <?php ($theme_key == 'expertusoneV2') ? print titleController('ADMIN-NARROW-USERVIEW-USERTYPE', sanitize_data($results[0]->usertype),24) : print titleController('ADMIN-NARROW-USERVIEW-USERTYPE', sanitize_data($results[0]->usertype),27); ?>
        </span></span>
     </div>
  </div>
  <div class="para">
   <div class="user-container">
       <span class="userview-label"><?php print t("LBL175"); ?>:</span>
        <span class="user-desc"><span class="vtip" title="<?php print sanitize_data($results[0]->hire_date); ?>">
             <?php ($theme_key == 'expertusoneV2') ? print titleController('ADMIN-NARROW-USERVIEW-HIREDATE', sanitize_data($results[0]->hire_date),24) : print titleController('ADMIN-NARROW-USERVIEW-HIREDATE', sanitize_data($results[0]->hire_date),27); ?>
        </span></span>
     </div>
  </div>
 <!--  <div class="para">
   <div class="user-container">
       <span class="userview-label"></span>&nbsp;
        <span class="user-desc"><span class="vtip" title="">
             
        </span>&nbsp;&nbsp;</span>
     </div>
  </div>-->
 <?php $webex_module=getProfileListItemByCode('lrn_cls_vct_web');
    if($results[0]->is_instructor == 'Y' && variable_get('webex_User') == 1  && $webex_module[0]->status == 'Y'){ ?>
  <div class="para">
   <div class="user-container">
       <span class="userview-label"><?php print t("WebEx").t(" ").t("LBL060"); ?>:</span>
     </div>
  </div>
  <?php } ?>
 </div>
 <div class="para">
   <div class="user-container">
            <div class="user-separator"></div>
  </div>
 </div>
 <div class="left-side-container">
   <div class="para">
   <div class="user-container">
       <span class="userview-label"><?php print t("LBL136"); ?>:</span>
       <span class="user-desc"><span class="vtip" title="<?php print sanitize_data(str_replace(',', ', ', $results[0]->dottedorg)); ?>"> <?php print titleController('ADMIN-NARROW-USERVIEW-OTHER-ORG', str_replace(',', ', ', $results[0]->dottedorg),24); ?>  </span></span>
     </div>
  </div>
 </div>
 <div class="right-side-container">
  <div class="para">
   <div class="user-container">
       <span class="userview-label"><?php print t("LBL135"); ?>:</span>
        <span class="user-desc"><span class="vtip" title="<?php print sanitize_data(str_replace(',', ', ', $results[0]->dottedmanager)); ?>"> <?php print titleController('ADMIN-NARROW-USERVIEW-OTHER-MANAGER', sanitize_data(str_replace(',', ', ', $results[0]->dottedmanager)),20); ?>  </span></span>
     </div>
  </div>
 </div>
 <?php $userDetails = array();
         $userDetails['drupal_picture'] = getDrupalUserAvator($results[0]->uid);
         $headerProfileImage = file_create_url($userDetails['drupal_picture']);?>
 <div class="para">
    <div class="user-container picture">
        <span class="userview-label"><?php print t("LBL062"); ?>:</span>
         <span class="user-desc">
         <?php if(!empty($userDetails['drupal_picture'])){?>
          <table border="0" cellpadding="0" cellspacing="0"><tr><td style="padding-top:7px;" valign="middle"><img class="admin-user-load-picture" src= "<?php print $headerProfileImage;?>" width="60" height="35" /></td></tr></table>
         <?php }?>
         </span>
    </div>
</div>

 <?php $access  = getGroupNamesForView($results[0]->id, 'cre_usr');
             $accessName  = '';
            if(!empty($access)){
              $accessName  = implode(", ",$access);
            }
            $accessName = htmlentities($accessName, ENT_QUOTES, "UTF-8");
 ?>

  <div class="para">
    <div class="user-container">
        <span class="userview-label"><?php print t("LBL642"); ?>:</span>
         <span id="user-access-desc" class="user-desc"><span class="vtip" title="<?php print sanitize_data($accessName); ?>"> <?php ($theme_key == 'expertusoneV2') ? print titleController('ADMIN-NARROW-USERVIEW-ACCESSNAME', $accessName,85) : print titleController('ADMIN-NARROW-USERVIEW-ACCESSNAME', $accessName,80); ?>  </span></span>
    </div>
 </div>
<?php
/*  change by ayyappan for 40138: In User page view mode "Access pop up" incorrectly showing */
    $grpDetails = getGroupsDetails($results[0]->id, '', '', 'view_users_group',0);
    $grpNames   = '';
    foreach ($grpDetails as $key=>$value) {
        if(empty($grpNames)) {
            $grpNames = $value->name;
        }
        else {
            $grpNames .= ', ' . $value->name;
        }
    }
    $grpNames = htmlentities($grpNames, ENT_QUOTES, "UTF-8");
    expDebug::dPrint('ayyappan bm'.print_r($grpNames, 1), 4);
?>
 <div class="para">
    <div class="user-container">
        <span class="userview-label"><?php print t("Groups"); ?>:</span>
         <span id="user-group-desc" class="user-desc"><span class="vtip" title="<?php print $grpNames; ?>"> <?php ($theme_key == 'expertusoneV2') ? print titleController('ADMIN-NARROW-USERVIEW-GROUPS', $grpNames,85) : print titleController('ADMIN-NARROW-USERVIEW-GROUPS', $grpNames,80); ?>  </span></span>
    </div>
 </div>
 
  
  
 <?php 
    if(module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status/render Custom Attribute values
        include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'/exp_sp_administration_customattribute_renderinforms.inc');
        print getCustomAttributesForView($results[0]->id, 'cre_usr'); 
   }else{ 
       $custom = getCustomFieldsForView($results[0]->id, $type);
     expDebug::dPrint('$$custom'.print_r( $custom,true),5);
     foreach($custom as $customInfo) :?>
 <div class="para">
    <div class="user-container custom-field-container">
        <span class="userview-label"><span class="vtip" title="<?php print sanitize_data($customInfo->label); ?>"> <?php print $customInfo->label; ?>:</span></span>
         <span class="user-desc"><?php print $customInfo->value ?></span>
    </div>
 </div>
 <?php endforeach; 
   }
 ?>

 <?php
 /* change by ayyappan for 40138: In User page view mode "Access pop up" incorrectly showing */
/* if(!empty($results[0]->id)){
    $entityId = $results[0]->id;
} else{
    $entityId = 0;
}
$entityType          = 'cre_usr';
$qtipIdInit          = $entityId.'_'.$entityType;
$qtipOptAccessObj      = "{'entityId':".$entityId.",'entityType':'$entityType',
'url':'administration/catalogaccess/".$entityId."/".$entityType."/".$entityId."',
'popupDispId':'qtip_visible_disp_".$qtipIdInit."','catalogVisibleId':'qtipAccessqtip_visible_disp_".$qtipIdInit."',
'wid':650,'heg':'270','postype':'topleft','poslwid':'40','qdis':'ctool','linkid':'visible-user-".$entityId."'}";
$qtipOptgrpObj      = "{'entityId':".$entityId.",'entityType':'$entityType','url':'administration/view/groups/".$entityId."/".$entityType."','popupDispId':'qtip_visible_disp_grp_".$qtipIdInit."','catalogVisibleId':'qtipAccessqtip_visible_disp_grp_".$qtipIdInit."','wid':300,'heg':'200','postype':'topleft','poslwid':'','qdis':'ctool','linkid':'visible-user-grp".$entityId."','scrollid':'usr-group-list'}"; */
?>
<!-- change by ayyappan for 40138: In User page view mode "Access pop up" incorrectly showing -->
<!--
<div class="crs-tab-titles-container">

<?php  if($sumedit > 0 || $GLOBALS["user"]->uid == 1 || !isset($sumedit)){?>
<div class="access-tab-icon"></div>
        <div id='qtip_visible_disp_<?php print $qtipIdInit;?>'>
        <a id='visible-user-<?php print $entityId;?>' onclick = "callVisibility(<?php print $qtipOptAccessObj;?>);" class="tab-title"><?php print t("LBL642");?></a>
        <span id='visible-popup-<?php print $entityId;?>' class='qtip-popup-visible' style='display:none; position:absolute; left:0px; top:0px;'></span>
  </div>
  <?php }else{?>
  <div class="active-deactive-disable-access-icon"></div>
  <div id='qtip_visible_disp_<?php print $qtipIdInit;?>'>
        <a id='visible-user-<?php print $entityId;?>' class="tab-title tab-disable "><?php print t("LBL642");?></a>
        <span id='visible-popup-<?php print $entityId;?>' class='qtip-popup-visible' style='display:none; position:absolute; left:0px; top:0px;'></span>
  </div>
  <?php } ?>


  <div class="tab-seperator"></div>
  <div class="crs-tab-titles-container-grp"><div class="grp-tab-icon"></div>
        <div id='qtip_visible_disp_grp_<?php print $qtipIdInit;?>'><a id='visible-user-grp<?php print $entityId;?>' onclick = "callVisibility(<?php print $qtipOptgrpObj;?>);" class="tab-title">
        <?php print t('LBL816').t('Groups');?></a>
        <span id='visible-popup-<?php print $entityId;?>' class='qtip-popup-visible' style='display:none; position:absolute; left:0px; top:0px;'></span>
        </div>
        </div>
   </div>
    -->
<div class="clearBoth"></div>    
</div>





