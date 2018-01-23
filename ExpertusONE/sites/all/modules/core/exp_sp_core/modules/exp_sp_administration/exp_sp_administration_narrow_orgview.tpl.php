 <?php 
expDebug::dPrint('enter the function sasasasAAAA'.print_r($results,true),4);
global $theme_key;
?>
<div class="top-record-div-left">
 <div class="para">
  <div class="code-container">
       <span class="detail-code"><?php print t("LBL107"); ?>:</span>
        <span class="detail-desc limit-title-row"><span class="vtip limit-title" title="<?php print sanitize_data($results[0]->name); ?>"> <?php print $results[0]->name; ?>  </span></span>
    </div>
 </div>
 <div class="left-side-container">
  <div class="para">
   <div class="code-container">
       <span class="detail-code"><?php print t("LBL036"); ?>:</span>
        <span class="detail-desc detail-single-line"><span class="single-line-lbl-val vtip" title="<?php print $results[0]->type; ?>"> <?php ($theme_key == 'expertusoneV2')? print titleController('ADMIN-NARROW-ORGVIEW-TYPE', $results[0]->type,74) : print titleController('ADMIN-NARROW-ORGVIEW-NAME', $results[0]->type,68); ?>  </span></span>
     </div>
  </div>
 </div>
 <div class="right-side-container">
  <div class="para">
      <div class="code-container">
           <span id="org-right-detail-code" class="detail-code"><?php print t("LBL155"); ?>:</span>
            <span class="detail-desc detail-single-line"><span class="single-line-lbl-val vtip" title="<?php print sanitize_data($results[0]->cost_center); ?>"> <?php ($theme_key == 'expertusoneV2')? print titleController('ADMIN-NARROW-ORGVIEW-COSTCENTER', $results[0]->cost_center,28) : print titleController('ADMIN-NARROW-ORGVIEW-COSTCENTER', $results[0]->cost_center,25); ?>  </span></span>
        </div>
  </div>
 </div> 
 <div class="para">
  <div class="code-container">
       <span class="detail-code"><?php print t("LBL229"); ?>:</span>
        <span id="org-desc-view" class="detail-desc"><?php print $results[0]->description; ?>
    </div>
 </div>
 <div class="left-side-container">
  <div class="para">
   <div class="code-container">
       <span class="detail-code"><?php print t("LBL151"); ?>:</span>
       <span class="detail-desc detail-single-line"><span class="single-line-lbl-val vtip" title="<?php print sanitize_data($results[0]->parent); ?>"> <?php ($theme_key == 'expertusoneV2')? print titleController('ADMIN-NARROW-ORGVIEW-PARENT', $results[0]->parent,74) : print titleController('ADMIN-NARROW-ORGVIEW-PARENT', $results[0]->parent,68); ?>  </span></span>
    </div>
 </div>
</div> 
 <div class="right-side-container">
  <div class="para">
      <div class="code-container">
           <span id="org-right-detail-code" class="detail-code"><?php print t("LBL153"); ?>:</span>
            <span class="detail-desc detail-single-line"><span class="single-line-lbl-val vtip" title="<?php print sanitize_data($results[0]->contact); ?>"><?php ($theme_key == 'expertusoneV2')? print titleController('ADMIN-NARROW-ORGVIEW-CONTACT', sanitize_data($results[0]->contact),74) : print titleController('ADMIN-NARROW-ORGVIEW-CONTACT', sanitize_data($results[0]->contact),68); ?>  </span></span>
        </div>
     </div>
 </div>
 
  <?php
  
    if(module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status/Get Custom Attributes for Course View
        include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'/exp_sp_administration_customattribute_renderinforms.inc');
        print getCustomAttributesForView($results[0]->id, 'cre_org');
    } //#custom_attribute_0078975 - End Check module status
  
    $access  = getGroupNamesForView($results[0]->id, 'cre_org');
  $accessName  = '';
    if(!empty($access)){
      $accessName  = implode(", ",$access);
    }
    ?>
 <div class="para">
    <div class="code-container">
        <span class="detail-code"><?php print t("LBL642"); ?>:</span>
        <span id= "access-detail-desc" class="detail-desc detail-single-line"><span class="single-line-lbl-val vtip" title="<?php print sanitize_data($accessName); ?>"> <?php ($theme_key == 'expertusoneV2')? print titleController('ADMIN-NARROW-ORGVIEW-ACCESSNAME', $accessName,70) : print titleController('ADMIN-NARROW-ORGVIEW-ACCESSNAME', $accessName,62); ?>  </span></span>                                     
    </div>
 </div>
 
 <?php
 if(!module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status/Get Custom Attributes for Course View
        $custom = getCustomFieldsForView($results[0]->id, $type);
        expDebug::dPrint(' $$custom'.print_r( $custom,true),5);
        foreach($custom as $customInfo) :?>
     <div class="para">
        <div class="code-container custom-field-container">
            <span class="detail-code"><span class="vtip" title="<?php print sanitize_data($customInfo->label); ?>"><?php print $customInfo->label; ?></span>:</span>
            <span  id="custom-detail-desc" class="detail-desc"><span class="single-line-lbl-val vtip" title="<?php print sanitize_data($customInfo->value); ?>"><?php ($theme_key == 'expertusoneV2')? print titleController('ADMIN-NARROW-ORGVIEW-CUSTOMINFO-VALUE',  $customInfo->value,20):print titleController('ADMIN-NARROW-ORGVIEW-CUSTOMINFO-VALUE',  $customInfo->value,15); ?></span></span>
        </div>   
     </div>
     <?php endforeach;
 } //#custom_attribute_0078975 - End Check module status ?>
 
</div>