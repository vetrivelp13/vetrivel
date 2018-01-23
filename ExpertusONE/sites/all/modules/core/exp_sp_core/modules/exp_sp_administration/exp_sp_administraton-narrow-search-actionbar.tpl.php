<?php expDebug::dPrint('narrow search actionbar tpl file : $add_page_path = ' . print_r($add_page_path, true),4); ?> 
<?php expDebug::dPrint('narrow search actionbar tpl file : $ctools_style = ' . print_r($ctools_style, true),4);
expDebug::dPrint('narrow search actionbar tpl file : $ctools_style = ' . print_r($page_code, true),4);
//Added by vetrivel.P for #0070900
$previlage = adminVisibilityForGlobalAdd($page_code);
//#custom_attribute_0078975
//$privelegecusatt = adminVisibilityForcustomattribute('cre_sys_obt_cattr');
$privelegecusatt = adminVisibilityForGlobalAdd('cre_sys_obt_cattr','','','1');
expDebug::dPrint('narrow search actionbar tpl file : $privelegecusatt = ' . print_r($privelegecusatt[0]->addvisible, true),4);
//Need to remove this config value once enable and disable is included in module configuration
$enable = getConfigValue('bulk_user_upload');
$permission =  $previlage[0]->addvisible;
expDebug::dPrint('narrow search $previlage tpl file :  = ' . print_r($previlage, true),4);
$dynamic_translation = getConfigValue('dynamic_translation');

?> 
            
            
<div id="<?php print $html_id;?>" class="narrow-search-actionbar <?php if(module_exists('exp_sp_administration_customattribute')){ print $GLOBALS["user"]->uid == 1 ? 'admin_user_custom_page' : 
 ($privelegecusatt[0]->addvisible !=-1) ? 'user_custom_page' : ''; }?>"  data='<?php print "".$actionbar_list."";?>'>

 <?php if ($show_export_button || $show_print_button): ?> 
  <ul class="narrow-search-actionbar-list  <?php print ($grey_out_add_button == 'group') ? 'grp-dropdown':($grey_out_add_button == 'customattribute' && module_exists('exp_sp_administration_customattribute')) ? 'grp-dropdown': '';?>" id="narrow-search-actionbar-list">

     <div class="grey-btn-bg-left"></div>
      <div class="grey-btn-bg-middle manage-dd"><span><?php print t('Manage');?></span></div>
                 <div class="manage-btn">
                     <a id="manage-dd-arrow" class="grey-btn-bg-right" "href="javascript:void(0);" onclick="manageDropdown();">&nbsp;&nbsp;</a>
                     <span class="dd-arrow-bg">&nbsp;</span>
                 </div>
                    <ul id="manage-dd-list">
                 <div class="manage-dd-list-arrow"></div>
                 <?php //Added by vetrivel.P for #0070900 
     if($enable == 1 && $upload_button_title && ($permission > 0 || $GLOBALS["user"]->uid == 1)){?>
        <li>
            <span class="user-upload-icon"></span><span class="dropdown-title"><?php print t('LBL610').' '.t('Users'); ?></span>
            <div id="help_text_upload" class="help-text-upload">
            <img src="/sites/all/themes/core/expertusoneV2/expertusone-internals/images/help.png" class="info-user-upload vtip" onclick="downloadLink('user_upload_done_btn');" title="<?php print t('MSG870');?>"/>
            </div>
            
             <div id="download_template">
               <div id='download_arrow' class="download-arrow"></div>
               <p id='template'><?php 
               //print t('MSG852'); //#custom_attribute_0078975 
               $customattr_mandatory_msg='';
               if(module_exists("exp_sp_administration_customattribute")){
                    $customattr_mandatory_msg=t('MSG1112');
               }
                $user_bulk_upload_msg=t('MSG852',array('@mand_custom_attr_msg'=>$customattr_mandatory_msg));
                print $user_bulk_upload_msg; 
               ?>
               </br>
               <?php print t('MSG853');?> <a id='download-link' class="download_link" "href="javascript:void(0);" onclick="downloadSample('user_upload_done_btn');"><?php print t('MSG854');?></a>.</p>
             </div>
             
            <form class='user_upload_form' method="post" id="user-upload-form" enctype="multipart/form-data" accept-charset="UTF-8">
            <input id='file_upload_id' type='file' name='files' class='file_upload_css' onchange='entityvalidation("user_upload_done_btn");' title="" data-wrapperid="paint-narrow-search-results" />
            <input type='submit' value='user_upload_done_btn' name='user_upload_done_btn' id='user_upload_done_btn' style='display:none;' />
            </form>
            </li>
        <div class="clearBoth"></div>
   <?php } //End for #0070900 by vetrivel.P ?>
                <?php //Added by Subin Rajan for ticket #76370
                //Need to remove this config value once enable and disable is included in module configuration
            $bulkProcEnable = getConfigValue('bulk_enroll_process');
     if($bulkProcEnable == 1 && $enroll_upload_button && ($permission > 0 || $GLOBALS["user"]->uid == 1)){?>
            <li>
            <span class="roster-upload-icon"></span><span class="dropdown-title"><?php print t('LBL610').' '.t('LBL275'); ?></span>
            <div id="help_text_upload" class="help-text-upload">
            <img src="/sites/all/themes/core/expertusoneV2/expertusone-internals/images/help.png" class="info-roster-upload vtip" onclick="downloadLink('enrollment_upload_file');" title="<?php print t('MSG870');?>"/>
            </div>
            
             <div id="download_template_enroll">
               <div id='download_roster_arrow' class="download_roster_arrow"></div>
               <p id='template'><?php print t('MSG741');?></br></br><?php print t('MSG742');?></br></br><?php print t('MSG743');?></br></br><?php print t('MSG865');?> <a id='download-link' class="download_link" href="javascript:void(0);" onclick="downloadSample('enrollment_upload_file');"><?php print t('MSG854');?></a>.</p>
             </div>
             
            <form class='roster_upload_form' method="post" id="roster-upload-form" enctype="multipart/form-data" accept-charset="UTF-8">
            <input id='file_upload_id' type='file' name='files' class='roster_file_upload_css' onchange='entityvalidation("enrollment_upload_file");' title="" data-wrapperid="paint-narrow-search-results" />
            <input type='submit' value='enrollment_upload_file' name='enrollment_upload_file' id='enrollment_upload_file' style='display:none;' />
            </form>
            </li>
        <div class="clearBoth"></div>
   <?php } //End Coding by Subin Rajan ?>
   
            <?php if (module_exists("exp_sp_administration_customattribute") && $dynamic_translation == 1 && $add_page_path =="/administration/manage/customattribute/nojs/addedit"):
            $languageList = locale_language_list('native');
            expDebug::dPrint("lang::".print_r($languageList,true));
            $langCodes = "";
            if(count($languageList) == 2 && $languageList["en-us"]=="English" &&  $languageList["en"]=="English Drupal")
            {
                //do not display translation popup menu
            }else{
                foreach ($languageList as $key => $value)
            
            {
                if($key == "en")
                    continue;
                if($langCodes == "")
                    $langCodes = $key;
                else
                    $langCodes = $langCodes.",".$key;
            }
            expDebug::dPrint("langcode::".print_r($langCodes,true));
            $accessmode = ($privelegecusatt[0]->addvisible > 0 || $GLOBALS["user"]->uid == 1)?"e":"v";
            ?>
                    <li>
                        <a onclick="launchTranslationsPopup('<?php print $langCodes;?>','<?php print $accessmode; ?>');"><span class="narrow-search-actionbar-translations" > </span> <span class="dropdown-title"><?php print t('Translations'); ?></span></a>
                    </li>
            <div class="clearBoth"></div>
            <?php } //if only english language 
                 ?>
            <?php endif; ?>
            <?php if ($show_export_button): ?>
             <li>
               <?php if ($showSeparator): ?>
                <span class="narrow-search-separater"></span>
                <?php endif; ?>
               <a onclick='$("<?php print '#' . $js_object_info['init_id']; ?>").data("<?php print $js_object_info['name']; ?>").exportSearchResults();'> <span class="narrow-search-actionbar-export"> </span> <span class="dropdown-title"><?php print t('Export to CSV'); ?></span></a>
             </li>
            <div class="clearBoth"></div>
            <?php $showSeparator = true; ?>
          <?php endif; ?>
         <?php $showSeparator = false; ?>
          <?php if ($show_print_button): ?> 
            <li>
              <a onclick='$("<?php print '#' . $js_object_info['init_id']; ?>").data("<?php print $js_object_info['name']; ?>").printSearchResults();'><span class="narrow-search-actionbar-print" > </span> <span class="dropdown-title"><?php print t('Print as PDF'); ?></span></a>
            </li>
            <div class="clearBoth"></div>
            <?php $showSeparator = true; ?>
          <?php endif; ?>
          <?php endif; ?>
   </ul>
   
  <?php 
  expDebug::dPrint('$page_code : '.print_r($page_code , true), 4);
  if($grey_out_add_button == 'group'){ // Only for Group screen
    $result1 = adminVisibilityForGlobalAdd('cre_sec');// Admin Group
    $result2 = adminVisibilityForGlobalAdd('cre_sec_learner');// Learner Group
    $privelage = privilegeChecking($result1[0]->addvisible,$result2[0]->addvisible);
  }else{
    $result = adminVisibilityForGlobalAdd($page_code);
  }
  $addvisible =  $result[0]->addvisible;
  if ($show_add_button && !empty($add_button_title)): ?>
    <li>
     <?php  if($addvisible > 0 || $GLOBALS["user"]->uid == 1 || !empty($privelage)) { ?>
       <?php if ($showSeparator): ?>
         <span class="narrow-search-separater"></span>
       <?php endif; ?>
      
      <?php $module_from = isset($module_from)?$module_from:'';
      if($module_from == 'report') { ?>
      <a class="narrow-search-actionbar-textbtn <?php if (!empty($add_page_path)) print $ctools_style.' use-ajax'; ?>" onclick='$("<?php print '#' . $js_object_info['init_id']; ?>").data("reportsearch").reportAddWizard();'>
      <?php } 
      else if($add_page_path == 'administration/order/create'){
      ?>
      <a class="narrow-search-actionbar-textbtn"  href='/?q=<?php print $add_page_path; ?>' data-wrapperid="paint-narrow-search-results">
      <?php 
      // #custom_attribute_0078975
      }else if($add_page_path != '/administration/people/group/type/nojs/addedit' && $add_page_path != 'administration/contentauthor/video/nojs/addedit' && $add_page_path != '/administration/manage/customattribute/nojs/addedit'){ ?>
      <a class="narrow-search-actionbar-textbtn addedit-form-expertusone-throbber <?php if (!empty($add_page_path)) print $ctools_style.' use-ajax'; ?>" <?php if (!empty($add_page_path)): ?> onclick='$(".qtip-popup-visible").html("").hide();$(".active-qtip-div").remove();$("#root-admin").data("narrowsearch").setPositionToCtoolPop("<?php print $add_page_path; ?>")'; href='/?q=<?php print $add_page_path; ?>' data-wrapperid="paint-narrow-search-results" <?php endif; ?> <?php if(empty($add_page_path)){ ?>onclick='$("<?php print '#' . $js_object_info['init_id']; ?>").data("<?php print $js_object_info['name']; ?>").addItem();' <?php } ?>>     
      <?php }
        else
        {
          
          if( $add_page_path == 'administration/contentauthor/video/nojs/addedit')
          {
          
       ?>
        <!--    <a class="narrow-search-actionbar-textbtn" onclick='showcreatemenu_contentauthor()'> --->
       <a title="<?php  print t('LBL817').' '.t('Video'); ?>" class="narrow-search-actionbar-orange-btnBG addedit-form-expertusone-throbber <?php if (!empty($add_page_path)) print $ctools_style.' use-ajax'; ?>" <?php if (!empty($add_page_path)): ?> onclick='$("#root-admin").data("narrowsearch").setPositionToCtoolPop("<?php print $add_page_path."/upload"; ?>")'; href='/?q=<?php print $add_page_path."/upload"; ?>' data-wrapperid="paint-narrow-search-results" <?php endif; ?> <?php if(empty($add_page_path)){ ?>onclick=' $("<?php print '#' . $js_object_info['init_id']; ?>").data("<?php print $js_object_info['name']; ?>").addItem();' <?php } ?>><?php print t('LBL817').' '.t('Video'); ?></a>      <?php
         }
        }
        ?>
      <?php global $user;
        $loggedUserName = $user->name;
        $account      = user_load($user->uid,TRUE);
        $adminPerm    = user_access('Certificate Admin Perm',$account); 
        if(($grey_out_add_button == 'notification_template' && ($loggedUserName != 'admin')) || ($grey_out_add_button == 'certificate' && ($loggedUserName != 'admin') && (empty($adminPerm)))) {?>
          <!--<span class="narrow-search-actionbar-greyout-btnLeft"></span>
          <span class="narrow-search-actionbar-greyout-btnBG" title="<?php //print $add_button_title; ?>">
          <?php // print $add_button_title; ?>
          </span>
          <span class="narrow-search-actionbar-greyout-btnRight"></span>
      --><?php }//#custom_attribute_0078975
       else if($grey_out_add_button == 'customattribute' && module_exists('exp_sp_administration_customattribute')) {
            expDebug::dPrint('$add_page_path : '.print_r($add_page_path , true), 4);
          ?> 
     <span class="narrow-search-actionbar-orange-btnLeft"></span>
      <span class="narrow-search-actionbar-orange-btnBG" title="<?php print $add_button_title; ?>"><?php print $add_button_title; ?>
        <span id="create-dd-arrow" class="pink-btn-bg-right" href="javascript:void(0);" onclick="createDropdown();"></span>
      </span>
      <span class="narrow-search-actionbar-orange-btnRight"></span>
      <ul id="create-dd-list">
                 <div class="create-dd-list-arrow"></div>
             <li>
             <a class="narrow-search-actionbar-textbtn addedit-form-expertusone-throbber <?php if (!empty($add_page_path)) print $ctools_style.' use-ajax'; ?>" <?php if (!empty($add_page_path)): ?> onclick='$("#root-admin").data("narrowsearch").setPositionToCtoolPop("administration/manage/customattribute/nojs/addedit/checkbox")'; href='/?q=administration/manage/customattribute/nojs/addedit/checkbox' data-wrapperid="paint-narrow-search-results" <?php endif; ?> <?php if(empty($add_page_path)){ ?>onclick='$("<?php print '#' . $js_object_info['init_id']; ?>").data("<?php print $js_object_info['name']; ?>").addItem();' <?php } ?>><span class="dropdown-title"><?php print t('LBL2006'); ?></span></a>
             </li>
            <div class="clearBoth"></div>
            <li>
              <a class="narrow-search-actionbar-textbtn addedit-form-expertusone-throbber <?php if (!empty($add_page_path)) print $ctools_style.' use-ajax'; ?>" <?php if (!empty($add_page_path)): ?> onclick='$("#root-admin").data("narrowsearch").setPositionToCtoolPop("administration/manage/customattribute/nojs/addedit/dropdown")'; href='/?q=administration/manage/customattribute/nojs/addedit/dropdown' data-wrapperid="paint-narrow-search-results" <?php endif; ?> <?php if(empty($add_page_path)){ ?>onclick='$("<?php print '#' . $js_object_info['init_id']; ?>").data("<?php print $js_object_info['name']; ?>").addItem();' <?php } ?>><span class="dropdown-title"><?php print t('LBL2007'); ?></span></a>
            </li>
            <div class="clearBoth"></div>
            <li>
              <a class="narrow-search-actionbar-textbtn addedit-form-expertusone-throbber <?php if (!empty($add_page_path)) print $ctools_style.' use-ajax'; ?>" <?php if (!empty($add_page_path)): ?> onclick='$("#root-admin").data("narrowsearch").setPositionToCtoolPop("administration/manage/customattribute/nojs/addedit/radio")'; href='/?q=administration/manage/customattribute/nojs/addedit/radio' data-wrapperid="paint-narrow-search-results" <?php endif; ?> <?php if(empty($add_page_path)){ ?>onclick='$("<?php print '#' . $js_object_info['init_id']; ?>").data("<?php print $js_object_info['name']; ?>").addItem();' <?php } ?>><span class="dropdown-title"><?php print t('LBL2008'); ?></span></a>
            </li>
            <div class="clearBoth"></div>
            <li>
              <a class="narrow-search-actionbar-textbtn addedit-form-expertusone-throbber <?php if (!empty($add_page_path)) print $ctools_style.' use-ajax'; ?>" <?php if (!empty($add_page_path)): ?> onclick='$("#root-admin").data("narrowsearch").setPositionToCtoolPop("administration/manage/customattribute/nojs/addedit/textarea")'; href='/?q=administration/manage/customattribute/nojs/addedit/textarea' data-wrapperid="paint-narrow-search-results" <?php endif; ?> <?php if(empty($add_page_path)){ ?>onclick='$("<?php print '#' . $js_object_info['init_id']; ?>").data("<?php print $js_object_info['name']; ?>").addItem();' <?php } ?>><span class="dropdown-title"><?php print t('LBL2010'); ?></span></a>
            </li>
            <div class="clearBoth"></div>
            <li>
              <a class="narrow-search-actionbar-textbtn addedit-form-expertusone-throbber <?php if (!empty($add_page_path)) print $ctools_style.' use-ajax'; ?>" <?php if (!empty($add_page_path)): ?> onclick='$("#root-admin").data("narrowsearch").setPositionToCtoolPop("administration/manage/customattribute/nojs/addedit/textbox")'; href='/?q=administration/manage/customattribute/nojs/addedit/textbox' data-wrapperid="paint-narrow-search-results" <?php endif; ?> <?php if(empty($add_page_path)){ ?>onclick='$("<?php print '#' . $js_object_info['init_id']; ?>").data("<?php print $js_object_info['name']; ?>").addItem();' <?php } ?>><span class="dropdown-title"><?php print t('LBL2009'); ?></span></a>
            </li>
            <div class="clearBoth"></div>
   </ul> <?php }
      else if($grey_out_add_button == 'group'){ ?>
     <?php if($privelage == 'both' ||  $GLOBALS["user"]->uid == 1){ ?>
     <span class="narrow-search-actionbar-orange-btnLeft"></span>
      <span class="narrow-search-actionbar-orange-btnBG" title="<?php print $add_button_title; ?>"><?php print $add_button_title; ?>
        <span id="create-dd-arrow" class="pink-btn-bg-right" href="javascript:void(0);" onclick="createDropdown();"></span>
      </span>
      <span class="narrow-search-actionbar-orange-btnRight"></span>
      <?php }else if($privelage == 'admin'){?>
      <a class="narrow-search-actionbar-textbtn adminGroup addedit-form-expertusone-throbber <?php if (!empty($add_page_path)) print $ctools_style.' use-ajax'; ?>" 
      <?php if (!empty($add_page_path)): ?> onclick='$("#root-admin").data("narrowsearch").setPositionToCtoolPop("administration/people/group/admin/nojs/addedit")'; 
      href='/?q=administration/people/group/admin/nojs/addedit' data-wrapperid="paint-narrow-search-results" <?php endif; ?> <?php if(empty($add_page_path)){ ?> 
      onclick='$("<?php print '#' . $js_object_info['init_id']; ?>").data("<?php print $js_object_info['name']; ?>").addItem();' <?php } ?>>
        <span class="narrow-search-actionbar-orange-btnLeft"></span>
        <span class="narrow-search-actionbar-orange-btnBG creategrouplabel" title="<?php print t('LBL817').' '.t('Administrator').' '.t('Group'); ?>">
            <?php print  t('LBL817').' '.t('Administrator').' '.t('Group'); ?></span>
        <span class="narrow-search-actionbar-orange-btnRight"></span>
      </a>
      <?php }else if($privelage == 'learner'){?>
      <a class="narrow-search-actionbar-textbtn learnerGroup addedit-form-expertusone-throbber <?php if (!empty($add_page_path)) print $ctools_style.' use-ajax'; ?>" 
      <?php if (!empty($add_page_path)): ?> onclick='$("#root-admin").data("narrowsearch").setPositionToCtoolPop("administration/people/group/learner/nojs/addedit")'; 
      href='/?q=administration/people/group/learner/nojs/addedit' data-wrapperid="paint-narrow-search-results" <?php endif; ?> 
      <?php if(empty($add_page_path)){ ?>onclick='$("<?php print '#' . $js_object_info['init_id']; ?>").data("<?php print $js_object_info['name']; ?>").addItem();'<?php }?>>
        <span class="narrow-search-actionbar-orange-btnLeft"></span>
        <span class="narrow-search-actionbar-orange-btnBG creategrouplabel" title="<?php print t('LBL817').' '.t('Learner').' '.t('Group'); ?>">
                <?php  print t('LBL817').' '.t('Learner').' '.t('Group');; ?></span>
        <span class="narrow-search-actionbar-orange-btnRight"></span> 
       </a><?php }?>
                 <!--<a id="create-dd-arrow" class="pink-btn-bg-right" "href="javascript:void(0);" onclick="createDropdown();">&nbsp;&nbsp;</a>-->
                    <ul id="create-dd-list">
                 <div class="create-dd-list-arrow"></div>
            <?php if ($show_export_button): ?>
             <li>
               <?php if ($showSeparator): ?>
                <!--<span class="narrow-search-separater"></span>-->
                <?php endif; ?>
               <a class="narrow-search-actionbar-textbtn addedit-form-expertusone-throbber <?php if (!empty($add_page_path)) print $ctools_style.' use-ajax'; ?>" <?php if (!empty($add_page_path)): ?> onclick='$("#root-admin").data("narrowsearch").setPositionToCtoolPop("administration/people/group/admin/nojs/addedit")'; href='/?q=administration/people/group/admin/nojs/addedit' data-wrapperid="paint-narrow-search-results" <?php endif; ?> <?php if(empty($add_page_path)){ ?>onclick='$("<?php print '#' . $js_object_info['init_id']; ?>").data("<?php print $js_object_info['name']; ?>").addItem();' <?php } ?>><span class="dropdown-title"><?php print t('Administrator'); ?></span></a>
             </li>
            <div class="clearBoth"></div>
            <?php $showSeparator = true; ?>
          <?php endif; ?>
         <?php $showSeparator = false; ?>
          <?php if ($show_print_button): ?> 
            <li>
              <a class="narrow-search-actionbar-textbtn addedit-form-expertusone-throbber <?php if (!empty($add_page_path)) print $ctools_style.' use-ajax'; ?>" <?php if (!empty($add_page_path)): ?> onclick='$("#root-admin").data("narrowsearch").setPositionToCtoolPop("administration/people/group/learner/nojs/addedit")'; href='/?q=administration/people/group/learner/nojs/addedit' data-wrapperid="paint-narrow-search-results" <?php endif; ?> <?php if(empty($add_page_path)){ ?>onclick='$("<?php print '#' . $js_object_info['init_id']; ?>").data("<?php print $js_object_info['name']; ?>").addItem();' <?php } ?>><span class="dropdown-title"><?php print t('Learner'); ?></span></a>
            </li>
            <div class="clearBoth"></div>
            <?php $showSeparator = true; ?>
          <?php endif; ?>
   </ul>
      <?php 
      }
      else {
      ?>
      <span class="narrow-search-actionbar-orange-btnLeft"></span>
      <?php /* ?>
      <a class="narrow-search-actionbar-textbtn" <?php if (!empty($add_page_path)): ?>href='/?q=<?php print $add_page_path; ?>'<?php endif; ?> onclick='$("<?php print '#' . $js_object_info['init_id']; ?>").data("<?php print $js_object_info['name']; ?>").addItem();'>
      <?php */ ?>
       <span class="narrow-search-actionbar-orange-btnBG" title="<?php print $add_button_title; ?>"  <?php if($add_page_path == 'administration/contentauthor/video/nojs/addedit') { ?> style="display:none"  <?php } ?> ><?php print $add_button_title; ?></span>
       <span class="narrow-search-actionbar-orange-btnRight"></span>
       <?php }?>
       
      </a>
       <?php } else{
        ?>
             <span class="narrow-search-separater"></span>
             <a>
      <span class="narrow-search-actionbar-greyout-btnLeft"></span>
          <span class="narrow-search-actionbar-greyout-btnBG" title="<?php print $add_button_title; ?>">
          <?php  print $add_button_title; ?>
          </span>
          <span class="narrow-search-actionbar-greyout-btnRight"></span></a>
        <?php 
       }?>
    </li>
    <?php $showSeparator = true; ?>
  <?php endif; ?>
   </ul><!-- End .narrow-search-sortbar-options -->
</div><!-- End .narrow-search-sortbar -->
<div class="clearBoth"></div>

<script>
var isTabScroll = document.getElementById('tab-scroller');
if(isTabScroll!=null){
    var actWidth= $('#narrow-search-actionbar').css('width');
  actWidth = actWidth.substring(0,actWidth.length - 6);
    //$('.page-menu-tab-navigator-prev').css('right',(parseInt(actWidth) + 50)+'px');
    $('.page-menu-tab-navigator-next').css('right',(parseInt(actWidth) + 20)+'px');
}
</script>