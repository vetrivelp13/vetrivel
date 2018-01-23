<?php global $theme_key; $calendar_view_arr = array('catalog','user','location','announcement','order');?>
<div style="display: block;" id="search-list-calendar-title-keyword">
	<span id="search-dropdwn-list-class">
		<input type="hidden" value="Type a Class Title" id="calendar-autocomplete_hidden">
				<?php 
					//$admincalendar_pref =json_decode($_COOKIE['admincalendar_pref']);
					$admincalendar_pref = getAdminCalendarPreference();
				
					  expDebug::dPrint('suresh $admincalendar_pref ---->'.print_r($admincalendar_pref,1),4);
					  $filter_ful_arr = array('class'=>t('LBL353'),'announcements'=>t('LBL196'),'order'=>t('LBL1025'),'report'=>t('Report'));
					  $filter_arr =array();
					  $filter_arr['All'] =t('LBL1039');

					  if($admincalendar_pref->view_classroom == 1 || $admincalendar_pref->view_virtualclassroom == 1 || $admincalendar_pref->view_classroom == 'true' || $admincalendar_pref->view_virtualclassroom == 'true')
					  	$filter_arr['view_classroom'] =t('LBL353');
					  if($admincalendar_pref->view_announcements == 1 || $admincalendar_pref->view_announcements == 'true')
					  	$filter_arr['view_announcements'] =t('LBL196');
					  if(module_exists('exp_sp_commerce') && ($admincalendar_pref->view_orders == 1 || $admincalendar_pref->view_orders == 'true'))
					  	$filter_arr['view_orders'] =t('LBL1025');
					  if($admincalendar_pref->view_scheduled_report == 1 || $admincalendar_pref->view_scheduled_report == 'true')
					  	$filter_arr['view_scheduled_report'] =t('Report');
					expDebug::dPrint('Admin Pref------>'.print_r($admincalendar_pref,1),5);
					$i= 0;
					$disable_search = 0;
					if($admincalendar_pref->view_classroom != 1 && $admincalendar_pref->view_virtualclassroom !=1 && $admincalendar_pref->view_classroom != 'true' && $admincalendar_pref->view_virtualclassroom != 'true'
						&& (!module_exists('exp_sp_commerce') || ($admincalendar_pref->view_orders != 1 && $admincalendar_pref->view_orders != 'true')) &&
						$admincalendar_pref->view_scheduled_report != 1 && $admincalendar_pref->view_scheduled_report != 'true'){
							$disable_search = 1;
						}
					//$filter_arr = count($filter_arr) > 1 ? 	$filter_arr : array();
					 
					foreach ($filter_arr as $key=>$val){
						if($i==0){
							if(strlen($val) >= 13) 
							  $fadeout= '<span class="fade-out"></span>';   /* Added/Changed by ganesh on May 9th 2017 for #73716 */
							
					?>
					<input type="hidden" value="<?php echo $key;?>" id="search_all_calendar_type-hidden">
					<?php if($disable_search == 1) {?>
						<span class="select-list-dropdown-calendar vtip" id="select-list-calendar-dropdown" style = "opacity:0.3;" title="<?php echo $val; ?>"><?php echo $val;echo $fadeout;?></span>
						<a onclick="moreClassSearchHideShow();" class="select-list-calendar-dropdown-link dropdown-disable-search" style = "opacity:0.3;" id="admin-dropdown-arrow"> </a>
					<?php }else{?>
					<span class="select-list-dropdown-calendar vtip" id="select-list-calendar-dropdown" title="<?php echo $val; ?>"><?php echo $val;echo $fadeout;?></span> 
				
					<a onclick="moreClassSearchHideShow();" class="select-list-calendar-dropdown-link" id="admin-dropdown-arrow"> </a>
					<?php }?>
					<ul id="select-list-calendar-dropdown-list" style="display: none;">
				<?php }else{?>
					<li onclick="moreClassSearchTypeText('<?php echo $val; ?>','<?php echo $key;?>');" class="<?php echo $key;?>"><?php echo $val; ?></li>
				<?php }$i++;}/*else{ ?>
					<li onclick="moreClassSearchTypeText('<?php echo $val; ?>','<?php echo $key;?>');" style="display:none;"><?php echo $val; ?></li>
				<?php }}*/?>
			</ul>
	</span>
</div>
<?php if($disable_search == 1) {?>
<div class='narrow-search-calendar-filter-container' id='narrow-search-calendar-filter-container'>
  <ul class="eol-search disable-search">
  	<li class='eol-search-go'><a id='narrow-search-calendar-filter-go' href='javascript:void(0);' >&nbsp;</a></li>
    <li class='eol-search-input'>
      <span>
        <input type='text' id='narrow-search-calendar-filter' disabled="disabled" onfocus="showHideLabelFocus();" onblur ="showHideLabelBlur();" ondrop="return false" class='ac_input ui-autocomplete-input' value='<?php print t('LBL304'); ?>' name='searchcalendarword' size='25' maxlength='70' alt='search...' autocomplete='on'/>
      </span>
    </li>
  </ul>
</div> 
<?php }else{?>
<div class='narrow-search-calendar-filter-container' id='narrow-search-calendar-filter-container'>
  <ul class="eol-search enable-search">
  	<li class='eol-search-go'><a id='narrow-search-calendar-filter-go' href='javascript:void(0);' >&nbsp;</a></li>
    <li class='eol-search-input'>
      <span>
        <input type='text' id='narrow-search-calendar-filter' onfocus="showHideLabelFocus();" onblur ="showHideLabelBlur();" ondrop="return false" class='ac_input ui-autocomplete-input' value='<?php print t('LBL304'); ?>' name='searchcalendarword' size='25' maxlength='70' alt='search...' autocomplete='on'/>
      </span>
<!--      <div class="eol-search-clearance"><span id="searchclear" ></span></div>-->
    </li>
  </ul>
</div> 
<?php }?>