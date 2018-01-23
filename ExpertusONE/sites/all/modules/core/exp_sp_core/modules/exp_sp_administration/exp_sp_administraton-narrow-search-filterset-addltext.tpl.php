<?php global $theme_key; ?>
<?php expDebug::dPrint('narrow search filterset a tpl file : $default_text = ' . print_r($default_text, true),4);?>
<?php 
$disabled = ($code =='grploc') ? 'disabled':'';
$disclsleft = ($code =='grploc') ? 'group-state-disabled-left-bg':'';
$disclsright = ($code =='grploc') ? 'group-state-disabled-right-bg':'';
$disclsmiddle = ($code =='grploc') ? 'group-state-disabled-middle-bg':'';
?>
<div id='<?php print $html_id; ?>' class='narrow-search-filterset'>
<ul>
	<li>
	    <span id='<?php print $code; ?>_filterset_title' class='cls-show' onclick='$("<?php print '#' . $js_object_info['init_id']; ?>").data("<?php print $js_object_info['name']; ?>").showHide("<?php print $code; ?>_filterset_title", "<?php print $code; ?>_filterset");'>
	    <a class="narrow-search-filterset-heading"><?php print $title;?></a>
		</span>
		<span id='<?php print $code; ?>-addltext-clr' class='clr-txt' style='display: none' onclick='$("<?php print '#' . $js_object_info['init_id']; ?>").data("<?php print $js_object_info['name']; ?>").clearTextBoxFields("<?php print $code; ?>", "<?php print $code;?>-addltext-filter-go" );'><?php print t("LBL307"); ?></span>
	</li>
</ul>
<div id='<?php print $code; ?>_filterset'>
		<div id="narrow_search_hideshow" class="narrow-text-container-cls" style="display: block;">
		<?php if($theme_key == 'expertusoneV2') { /* Added/changed by ganeshbabuv on Jan 11th 2016 for  Location Filter #66404 */
			$max_txt_field_length='70';
			if($code=="classLocation" || $from=='AdminManageLocationPage'){ 
				$max_txt_field_length='500';  
			}
						
			$attr_name_str='';
			if($from=='AdminManageLocationPage'){
				$attr_name_str="name='".$name."'"; 
			} ?>
			<div class="filter-search-start-date-left-bg <?php print $disclsleft;?>"></div>
				<div class="filter-search-box-container">						
						<input type='text' <?php echo $attr_name_str;?> id='<?php print $code;?>-addltext-filter' ondrop="return false" class='ac_input narrow-search-ac-text-overlap filter-search-start-date-middle-bg <?php print $disclsmiddle;?>' value="<?php print $default_text;?>" size='25' maxlength="<?php print $max_txt_field_length;?>" alt='search...' size='20' autocomplete='on' <?php print $disabled;?>/>				
		    		<a class="narrow-text-search" title = <?php print t('LBL304');?> id="<?php print $code;?>-addltext-filter-go" 
      					data-default-text="<?php print $default_text;?>" 
      					onClick='$("<?php print '#' . $js_object_info['init_id']; ?>").data("<?php print $js_object_info['name']; ?>").validatetextField("<?php print $code; ?>","<?php print $code;?>-addltext-filter-go");'>&nbsp;</a>
    		</div>
    		<div class="filter-search-start-date-right-bg <?php print $disclsright;?>"></div>
		<?php } else { ?>
			<input type='text' id='<?php print $code;?>-addltext-filter' ondrop="return false" class='ac_input narrow-search-ac-text-overlap' value="<?php print $default_text;?>" size='25' maxlength='70' alt='search...' size='20' autocomplete='on'/>		
    		<a class="narrow-text-search" title = <?php print t('LBL304');?> id="<?php print $code;?>-addltext-filter-go" 
      			data-default-text="<?php print $default_text;?>" 
      			onClick='$("<?php print '#' . $js_object_info['init_id']; ?>").data("<?php print $js_object_info['name']; ?>").validatetextField("<?php print $code; ?>","<?php print $code;?>-addltext-filter-go");'>&nbsp;</a>
      	<?php } ?>			
		</div>
		</div>
</div>