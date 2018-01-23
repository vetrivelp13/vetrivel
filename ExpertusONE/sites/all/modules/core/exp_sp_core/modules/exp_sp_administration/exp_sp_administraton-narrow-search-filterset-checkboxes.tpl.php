<?php expDebug::dPrint('narrow search filterset checkbox tpl file : $checkboxes_list = ' . print_r($checkboxes_list, true),4);?>
<div id='<?php print $html_id; ?>' class='narrow-search-filterset'  <?php if ($html_id == 'class-admin-page-opened') print 'style="display:none;"'; ?>>
  <ul>
    <li>
      <span id='<?php print $code; ?>_filterset_title' class='cls-show ' onclick='$("<?php print '#' . $js_object_info['init_id']; ?>").data("<?php print $js_object_info['name']; ?>").showHide("<?php print $code; ?>_filterset_title", "<?php print $code; ?>_filterset");'>
        <a class='narrow-search-filterset-heading'><?php print $title; ?></a>
      </span>
    </li>
  </ul>
  <div id='<?php print $code; ?>_filterset'>
  <div class='<?php print $code; ?>-scroll-list-container'>
  <?php $langCount = 0; 
  if($html_id == 'classlangtype_container' || $html_id == 'catalogclassmanageby_container'){
    $title = $title.'_class';
  }
  $dispId = str_replace(" ","",$title);
  $dispId1 = isset($divid)&&!empty($divid)?$divid:$dispId;
  $dispId1 = str_replace('\'','-',$dispId1);
  $checked_list = isset($checked_list)?$checked_list:array();
  
  foreach ($checkboxes_list as $val => $name):
          if(!empty($name)) {
            $langCount = $langCount+1;
               ?>
               <?php expDebug::dPrint('raman'.$name);?>
              <?php $narrTitle=''; if($name=='Curricula') { $narrTitle=t(Curriculum); } else { $narrTitle=$name; } ?> 
              <?php $titleLength = $code == 'group' ? 14 : 17;
              $title_translated = t($narrTitle);
              //Multilang Support
              if($title_translated == 'Administer'){
              	$title_translated = ucfirst(strtolower(t('ADMINISTER')));
              }
              $nameTitle = titleController('NARROW-SEARCH-FILTERSET-CHECKBOX', $title_translated, $titleLength);
              ?> 
              <?php $checkboxSelectedClass =  (isset($checked_list[$val]))? 'narrow-search-filterset-checkbox-selected': 'narrow-search-filterset-checkbox-unselected'; ?>
              <?php $labelSelectedClass =  (isset($checked_list[$val]))? 'narrow-search-filterset-item-label-selected': 'narrow-search-filterset-item-label-unselected'; ?>
              <?php $checkedStatus =  (isset($checked_list[$val]))? 'checked': ''; ?>
              <?php //38455: Managed by filters me is not checked when login as multilanguage ?>
              <?php //change by ayyappan for 40311 and 40223?>
              <div class='narrow-search-filterset-checkbox-container' id='<?php print $dispId1; ?>_<?php print $langCount; ?>'>
              <?php $check_box_id = $code == 'classmanage' ? 'classmanage_checkbox_'.$val : 'checkbox_'.$val; ?>
                  <?php 
                  if($_GET['q'] == 'administration/search-filter/order') {
                   if($title_translated != t('Completed')) {?>
	                <label for='<?php print $check_box_id; ?>' class='<?php print $checkboxSelectedClass; ?>' >
	                  
	                  <?php if ($code == 'clspgopened') { expDebug::dPrint('$code = ' . $code, 4); ?>
	                  <input id='checkbox_<?php print $code; ?>' class='narrow-search-filterset-checkbox'
	                      type='checkbox' <?php print $checkedStatus; ?> value="<?php print $val; ?>"/>
	                  <?php }else {?>
	                  <input id='<?php print $check_box_id; ?>' class='narrow-search-filterset-checkbox' type='checkbox' <?php print $checkedStatus; ?> value="<?php print $val; ?>" onClick='$("<?php print '#' . $js_object_info['init_id']; ?>").data("<?php print $js_object_info['name']; ?>").narrowSearch();'/>
	                  <?php }?>
	                </label>               
	                <label for='<?php print $check_box_id; ?>' class='narrow-search-filterset-item-label <?php print $labelSelectedClass;?> vtip' title='<?php print htmlspecialchars(t($narrTitle), ENT_QUOTES); ?>'>
	                  <?php  print $nameTitle; ?>
	                </label>
                 <?php  } } else { ?>
                	<label for='<?php print $check_box_id; ?>' class='<?php print $checkboxSelectedClass; ?>' >
                	<?php if ($code == 'clspgopened') {
                	expDebug::dPrint('$code = ' . $code, 4); ?>
                	<input id='checkbox_<?php print $code; ?>' class='narrow-search-filterset-checkbox'
                	     type='checkbox' <?php print $checkedStatus; ?> value="<?php print $val; ?>"/>
                	<?php }else {?>
                	<input id='<?php print $check_box_id; ?>' class='narrow-search-filterset-checkbox' type='checkbox' <?php print $checkedStatus; ?> value="<?php print $val; ?>" onClick='$("<?php print '#' . $js_object_info['init_id']; ?>").data("<?php print $js_object_info['name']; ?>").narrowSearch();'/>
                	<?php }?>
                	</label>               
                	<label for='<?php print $check_box_id; ?>' class='narrow-search-filterset-item-label <?php print $labelSelectedClass;?> vtip' title='<?php print htmlspecialchars(t($narrTitle), ENT_QUOTES); ?>'>
                	<?php  print $nameTitle; ?>
                	 </label>
             	<?php    } ?>
              </div>
              <?php
          } 
  endforeach; ?>
								<?php if($code == 'group') { ?>
								<input id= 'group_list_count' type ="hidden" value = "<?php print $langCount ?>"/>
								<?php } ?>
   </div>							
  </div> 
</div>
