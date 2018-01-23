<?php 
/**
 * Function used to show catalog results based on url_string 
 */

function getWidgetCatalogSearch() {
  global $theme_key,$base_url;;
	drupal_add_js(drupal_get_path('module', 'fivestar').'/js/fivestar.js', $js_module_optional);
	drupal_add_js(drupal_get_path('module', 'exp_sp_learning').'/exp_sp_learning.js', $js_module_optional);
	drupal_add_js(drupal_get_path('module', 'fivestar').'/js/fivestar.ajax.js', $js_module_optional);
	if($theme_key == 'expertusoneV2')
		drupal_add_css(drupal_get_path('module', 'fivestar').'/css/fivestar_v2.css',$css_theme_optional);
	else
		drupal_add_css(drupal_get_path('module', 'fivestar').'/css/fivestar.css',$css_theme_optional);
	includeJqGridJsCss();
  require_once drupal_get_path('module', 'exp_sp_lnrsearch').'/exp_sp_lnrsearch.inc';
  $short_parameter = urldecode(arg(2));
  expDebug::dPrint('$short_parameter id = ' . print_r($short_parameter,true), 4);
  $short_url_arr = explode('&',$short_parameter); 
  $selectStmt = db_select('slt_widget','widget');
  $selectStmt->addField('widget','type','widget_type');
  $selectStmt->addField('widget','mode','search_mode');
  $selectStmt->addField('widget','display_size','display_size');
  $selectStmt->addField('widget','options','catalog_display_parameters');
  $selectStmt->addField('widget','parameters','widget_parameters');
  $selectStmt->addField('widget','theme','theme');
  $selectStmt->condition('widget.url',$short_url_arr[0],'=');
  $result = $selectStmt->execute()->fetchAssoc();
  //expDebug::dPrintDBAPI(' $selectStmt SQL = ' , $selectStmt);
  //expDebug::dPrint('slt_widget id = ' . print_r($result,true), 4);
  //expDebug::dPrint('slt_widget server ' . print_r($_SERVER,true), 4);
  // echo '<pre>'; print_r($result); echo '</pre>'; die;
  if (!empty($result)){
  	$widget_parameters_arr = unserialize($result['widget_parameters']);
  	$widget_parameters_str = '';
  	$userId = getSltpersonUserId();
  	foreach ($widget_parameters_arr as $key=>$val)
  	{
  		if(!empty($userId) && $userId > 0){
  			if($key!='language')
  				$widget_parameters_str .= '&'.$key.'='.$val;
  		}else{  			
	  		$widget_parameters_str .= '&'.$key.'='.$val;
  		}
  	}
  	$result['widget_parameters'] = $widget_parameters_str;
  	$result['catalog_display_parameters'] = unserialize($result['catalog_display_parameters']);
  	$_SESSION['widget']['catalog_display_parameters'] = $result['catalog_display_parameters'];
  	$_SESSION['widget']['widget_catalog_url'] = $base_url.$_SERVER['REQUEST_URI'];
  	//42055: Provide an option in the admin share widget screen to pass site url
  	$_SESSION['widget']['pass_url'] = $result['catalog_display_parameters']['pass_url'];
  	$display_size_arr = explode('x', $result['display_size']);
  	$_SESSION['widget']['display_width'] = ($display_size_arr[0] == '') ? '640' : $display_size_arr[0]; // default width 640
  	$_SESSION['widget']['display_height'] = ($display_size_arr[1] == '') ? '480' : $display_size_arr[1]; // default width 480
  	expDebug::dPrint('slt_widget Session ' . print_r($_SESSION['widget'],true), 4);
  	$blckPrm = block_role_permission_check('exp_sp_lnrsearch','tab_find_training');
    expDebug::dPrint("Block permission result for widget".$blckPrm,4);
    if($blckPrm === false){
     	$dest = explode('q=',$_SESSION['widget']['widget_catalog_url']);
     	$repRelayUrl = str_replace('%2F', '/', $dest[1]);
    	$query = array('destination' => $repRelayUrl);
     	drupal_goto('share/unauthorised',array('query' => $query)); 
    }else{
    	$outputString = '<div id="lnr-catalog-search"><div id="block-take-survey"></div><div id="searchRecordsPaint" class="search-record-paint"><table id=paintContentResults></table></div><div id="pager" style="display:none;"></div></div>
  		<div style="display: block;" id="no-records" style="display:none;"></div>
    		<div id="paintCatalog-show_more"  onclick="$(\'#searchRecordsPaint\').data(\'showmore\').showMoreData({\'gridId\': \'paintContentResults\', \'gridWrapperId\': \'searchRecordsPaint\', \'showMoreId\': this.id}, event);" class="show-more" style="text-align: center; display: none;">
					<span class="show-more-wrapper">'. t("SHOW MORE").'</span>
				</div>
  		<a onclick="getClickSignIn()" class="use-ajax ctools-modal-ctools-login-style" id="signin" html="1" href="/?q=ctools_ajax_sample/ajax/user_login/widget"></a>'; /*-- #41760 - widget parameter added to find a callfrom widget --*/
    }
  	
  } else {
   $outputString = '<div class="widget_info_msg">' .t('Sorry! Requested page not found. Please contact support team.') . '</div>';
  }
  drupal_add_js(array('widget' => array('widget_details' =>  $result)), 'setting');
  //echo '<pre>'; print_r($result); echo '</pre>';
  return $outputString;
}
function setWidgetJsVAriable(){
	$display_size = $_SESSION['widget']['display_width'].'x'.$_SESSION['widget']['display_height'];
	$result = array('display_size'=>$display_size);
	drupal_add_js(array('widget' => array('widget_details' =>  $result)), 'setting');
}
/**
 * Function used to retirve the course details
 */
function getWidgetClassDetails() {
	drupal_add_js(drupal_get_path('module','exp_sp_prerequisite').'/exp_sp_prerequisite.js',$js_module_optional);
 $block = block_load('exp_sp_classdetail', 'class_details');
 $render_array = _block_get_renderable_array(_block_render_blocks(array($block)));
 $output = render($render_array);
 setWidgetJsVAriable();
 return $output.'<a onclick="getClickSignIn()" class="use-ajax ctools-modal-ctools-login-style" id="signin" html="1" href="/?q=ctools_ajax_sample/ajax/user_login"></a>';
}

/**
 * Function used to retirve the course details
 */
function getWidgetCourseDetails() {
	drupal_add_js(drupal_get_path('module','exp_sp_prerequisite').'/exp_sp_prerequisite.js',$js_module_optional);
 $block = block_load('exp_sp_coursedetail', 'course_details');
 $render_array = _block_get_renderable_array(_block_render_blocks(array($block)));
 $output = render($render_array);
 setWidgetJsVAriable();
 return $output.'<a onclick="getClickSignIn()" class="use-ajax ctools-modal-ctools-login-style" id="signin" html="1" href="/?q=ctools_ajax_sample/ajax/user_login"></a>';
}

/**
 * Function used to retirve the course details
 */
function getWidgetLearningPlanDetails() {
	drupal_add_js(drupal_get_path('module','exp_sp_prerequisite').'/exp_sp_prerequisite.js',$js_module_optional);
 require_once drupal_get_path('module', 'exp_sp_learning_plan_detail').'/exp_sp_learning_plan_detail.inc';
 require_once drupal_get_path('module', 'exp_sp_coursedetail').'/exp_sp_coursedetail.inc';
 $block = block_load('exp_sp_learning_plan_detail', 'learning_details');
 $render_array = _block_get_renderable_array(_block_render_blocks(array($block)));
 $output = render($render_array);
 setWidgetJsVAriable();
 return $output.'<a onclick="getClickSignIn()" class="use-ajax ctools-modal-ctools-login-style" id="signin" html="1" href="/?q=ctools_ajax_sample/ajax/user_login"></a>';
}
/**
 * Function used to display my learning page
 */
function getWidgetEnrollmentSearch() {
   global $theme_key;
   includeJqGridJsCss();
   require_once drupal_get_path('module', 'exp_sp_lnrenrollment').'/exp_sp_lnrenrollment.inc';
   drupal_add_js(drupal_get_path('module', 'exp_sp_learning_learner').'/js/exp_sp_lnrenrollment/exp_sp_lnrenrollment.js', $js_module_optional);
  
   drupal_add_js(drupal_get_path('module', 'fivestar').'/js/fivestar.js', $js_module_optional);
   drupal_add_js(drupal_get_path('module', 'fivestar').'/js/fivestar.ajax.js', $js_module_optional);
   drupal_add_js(drupal_get_path('module', 'exp_sp_learning_learner').'/js/exp_sp_registrationdetail/exp_sp_launch.js', $js_module_optional);
   if(getConfigValue('wbt_content_launch_style') == 1) {
  	 drupal_add_js(drupal_get_path('module', 'exp_sp_learning_learner').'/js/exp_sp_registrationdetail/SCORM-API-WRAPPER-MODAL.js', $js_module_optional);
   } else {
  	 drupal_add_js(drupal_get_path('module', 'exp_sp_learning_learner').'/js/exp_sp_registrationdetail/SCORM-API-WRAPPER.js', $js_module_optional);
   }
   drupal_add_js(drupal_get_path('module', 'exp_sp_learning_learner').'/js/exp_sp_registrationdetail/SCORM12-LMS-API.js', $js_module_optional);
   drupal_add_js(drupal_get_path('module', 'exp_sp_learning_learner').'/js/exp_sp_registrationdetail/SCORM2004-LMS-API.js', $js_module_optional);
   drupal_add_js(drupal_get_path('module', 'exp_sp_survey').'/exp_sp_survey_learner/js/exp_sp_surveylearner/exp_sp_surveylearner.js',$js_module_optional);
   if($theme_key == 'expertusoneV2') {
    //NEWUI THEME STYLE SHEET
    drupal_add_css(drupal_get_path('module', 'exp_sp_learning_learner') .'/css/exp_sp_lnrsearch_v2.css', $css_theme_optional);
    drupal_add_css(drupal_get_path('module', 'exp_sp_learning_learner') .'/css/exp_sp_lnrenrollment_v2.css', $css_module_optional);
 
    drupal_add_css(drupal_get_path('module', 'exp_sp_survey').'/exp_sp_survey_learner/css/Survey_formatter_v2.css',$js_module_optional);
    drupal_add_css(drupal_get_path('module', 'fivestar').'/css/fivestar_v2.css',$css_theme_optional);
   }else{
    drupal_add_css(drupal_get_path('module', 'exp_sp_learning_learner') .'/css/exp_sp_lnrsearch.css', $css_theme_optional);
    drupal_add_css(drupal_get_path('module', 'exp_sp_learning_learner') .'/css/exp_sp_lnrenrollment.css', $css_module_optional);
  
    drupal_add_css(drupal_get_path('module', 'exp_sp_survey').'/exp_sp_survey_learner/css/Survey_formatter.css',$js_module_optional);
    drupal_add_css(drupal_get_path('module', 'fivestar').'/css/fivestar.css',$css_theme_optional);
    drupal_add_css(drupal_get_path('module', 'fivestar').'/css/fivestar-admin.css',$css_theme_optional);
    drupal_add_css(drupal_get_path('module', 'fivestar').'/css/fivestar-admin-rtl.css',$css_theme_optional);
    drupal_add_css(drupal_get_path('module', 'fivestar').'/css/fivestar-rtl.css',$css_theme_optional);
    drupal_add_css(drupal_get_path('module', 'exp_sp_fivestar').'/exp_sp_fivestar.css',$css_theme_optional);
}
   $navigationDiv1 = '<div id="learner-maincontent_tab3">
   <ul class="EnrolltabNavigation">'.
   '<li class="selected"><a id="Enrollmentpart" href="javascript:void(0);" onclick=\'$(".EnrolltabNavigation li").removeClass("selected");$(this).parents("li").addClass("selected");$("#learner-enrollment-tab-inner").data("enrollment").callEnrollment("lrn_crs_cmp_enr|lrn_crs_cmp_inp|lrn_crs_cmp_att","Enrollmentpart")\'><span><span>' .  t("Enrolled") .  '</span></span></a></li>'.
   '<li><a id="EnrollCompleted" href="javascript:void(0);" onclick=\'$(".EnrolltabNavigation li").removeClass("selected");$(this).parents("li").addClass("selected");$("#learner-enrollment-tab-inner").data("enrollment").callEnrollment("lrn_crs_cmp_cmp","EnrollCompleted");\'><span><span>' . t("Completed") .'</span></span></a></li>'.
   '<li><a id="EnrollInCompleted" href="javascript:void(0);" onclick=\'$(".EnrolltabNavigation li").removeClass("selected");$(this).parents("li").addClass("selected");$("#learner-enrollment-tab-inner").data("enrollment").callEnrollment("lrn_crs_cmp_inc|lrn_crs_cmp_nsw","EnrollInCompleted")\'><span><span>' . t("Incomplete") .'</span></span></a></li>'.
   '<li><a id="EnrollExpired" href="javascript:void(0);" onclick=\'$(".EnrolltabNavigation li").removeClass("selected");$(this).parents("li").addClass("selected");$("#learner-enrollment-tab-inner").data("enrollment").callEnrollment("lrn_crs_cmp_exp","EnrollExpired");\'><span><span>' . t("Expired") . '</span></span></a></li>'.
   '<li><a id="EnrollCanceled" href="javascript:void(0);" onclick=\'$(".EnrolltabNavigation li").removeClass("selected");$(this).parents("li").addClass("selected");$("#learner-enrollment-tab-inner").data("enrollment").callEnrollment("lrn_crs_reg_can","EnrollCanceled")\'><span><span>' .  t("Canceled") . '</span></span></a></li>'.
   '<li class="removeMrg"><a id="EnrollPayments" href="javascript:void(0);" onclick=\'$(".EnrolltabNavigation li").removeClass("selected");$(this).parents("li").addClass("selected");$("#learner-enrollment-tab-inner").data("enrollment").callEnrollment("lrn_crs_reg_ppm|lrn_crs_reg_wtl","EnrollPayments")\'><span><span>'. t("Pending") .'</span></span></a></li>'.
   '</ul>
   </div>';
   $outhtml ='
   
   <div class="content">
   <div id="learner-enrollment-tab-inner"></div>    <div id="block-take-survey"></div>
   </div>
   <div class="clearBoth"></div>
   <div id="enroll-result-container">'. $navigationDiv .'   
   <table id="paintEnrollmentResults"></table>
   <div id="pager"></div>
   <div id="enroll-noresult-msg">'. t('MSG261') .'<span id="enrollment-state">' . strtolower(t('Enrolled')). '</span>'.strtolower(t('LBL152')) . '</div>
   </div>
   
   ';
   return $outhtml;
  
}
//&title=&dl_type=lrn_cls_dty_ilt&lg_type=cre_sys_lng_eng&all_lg_type=cre_sys_lng_eng|cre_sys_lng_gzh&ob_type=&location=&tag=&price=$0-$10000&startdate=&enddate=&cy_type=&all_cy_type=IN|US&sortby=undefined&mro_type=&all_mro_type=&rating_type=&_search=false&nd=1412061972917&rows=10&page=1&sidx=&sord=desc
/*$searchParam = array(
			'title' => '', // search filter used
			'dl_type' => 'lrn_cls_dty_vod|lrn_cls_dty_wbt', // delivery type
			'lg_type' => 'cre_sys_lng_eng', // language type - multiple values
			'all_lg_type' => 'cre_sys_lng_eng|cre_sys_lng_gzh', // language type if all emoty
			'ob_type' => 'cre_sys_obt_crt|cre_sys_obt_cur|cre_sys_obt_trn', // tp type
			'location' => '', // localtion -single value
			'tag' => '', // tag name - single value
			'price' => '$65-$10000', // price value - range value
			'startdate' => '', // start date - date value, if null default date is current date
			'enddate' => '', // end date - date value
			'cy_type' => '', // country type - multi value
			'all_cy_type' => 'IN|US', // country type - if cy_type empty, this field used
			'sortby' => '', // sory by value - single value
			'mro_type' => '',  
			'all_mro_type' => '',
			'rating_type' => '',// raring - multiple values
			'rows' => 10, // pagination count
			'sord'=>'desc'
		);
echo serialize($searchParam); */

/*$searchParam = array(
		'ent_type'=>'cre_sys_obt_crs',
		'ent_id'=>56,
		'rows' => 10 // pagination count
);
echo serialize($searchParam);*/
?>