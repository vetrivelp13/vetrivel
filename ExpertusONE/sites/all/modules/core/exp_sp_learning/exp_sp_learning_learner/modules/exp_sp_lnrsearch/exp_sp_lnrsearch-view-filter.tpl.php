<?php 
  // Retaining filter values has been taken from the cookie 'catalog_searchStr'
  global $theme_key;
  global $catalog_reg;
  $searchStr_read = isset($_COOKIE['searchStr_read'])?$_COOKIE['searchStr_read']:'';
  if($searchStr_read==1){
    $catalog_searchStr = $_COOKIE['catalog_searchStr'];
    $searchStrArr = explode('&',$catalog_searchStr);
    foreach($searchStrArr as $searchStrValue){
      list($searchStrName, $searchStrValue) = explode('=', $searchStrValue);
        $$searchStrName = explode('|',$searchStrValue);
    } 
  }
  $catSrcArr = array();
  $isCached = TRUE;
  if ($cache = cache_get('catlog_src_filters')){
  	$catSrcArr = $cache->data;
  	unset($catSrcArr['exp_catalog_tagcloud']);
  	expDebug::dPrint("Cached catalog search data ".print_r($catSrcArr,true),3);
  }else{
  	$isCached = FALSE;
  }
?>
<?php 
// this is not need now bcz we are getting all search related data from cookie the search sting val assing like that.
/*if($searchStr_read==1 && count($title) && $title[0]!=''){ ?>
<script language="text/javascript">$('#search_searchtext').val('<?php echo urldecode($title[0]); ?>');</script>
<?php } */?>
<table class="content region-sidebar-widget-bg">
	<tbody>
		<tr>
			<td>
				<div id="LnrSearchService_searchOptDiv">
					<div class="search-list" id="LnrSearchService_searchoptions">
						<div class="search-opts" id="searchopts-content">   
						<?php if($theme_key != 'expertusoneV2') {?>
						<div>
							<span class='srch-filter-heading'><?php print t("Narrow Results"); ?></span>
						</div>		
						<?php }?>							

						<div id='search_delivery_type' class='search-opt-box'>
							<ul class='find-list-items'>
								<li>
									<span id='deliveryTypeHideShow' class='cls-show ' onclick='$("#lnr-catalog-search").data("lnrcatalogsearch").showHide("deliveryTypeHideShow","paintDeliveryType");'>
										<a class='search-heading'><?php print t("LBL036"); ?></a>
									</span>
									<span id="refine-filter-pin-icon" class="pin-icon-black" onclick="$('#lnr-catalog-search').data('lnrcatalogsearch').pinUnpinFilterCriteria();"></span>
								</li>
							</ul>
							<div id='paintDeliveryType' class="catalog-criteria-filter-set">
							
								<?php 
								
								  if (isset($catSrcArr['exp_delivery_type'])){
                    $delivery_types = $catSrcArr['exp_delivery_type'];
                  }else{
                  	$delivery_types = getDeliverTypes(); 
                    $catSrcArr['exp_delivery_type'] = $delivery_types;
                  }
      	
								//$delivery_types = getDeliverTypes(); 
								  foreach ($delivery_types as $id => $row) {
								?>
									<div id='delivery_hideshow_<?php print $row->c_code; ?>' class='srch-checkbox-container-cls' style='display:block'>
											<label for='lrn_srch_delivery_<?php print $row->c_code; ?>' class='checkbox-unselected' ><input id='lrn_srch_delivery_<?php print $row->c_code; ?>' class='dt-others srch-checkbox-cls' type='checkbox' <?php echo in_array($row->c_code, $dl_type)?'checked':''; ?>  value='<?php print $row->c_code; ?>' onClick='$("#lnr-catalog-search").data("lnrcatalogsearch").searchAction(this);' data-filter-label='<?php print t($row->c_name);?>' data-filter-name='delivery-type' data-filter-id='lrn_srch_delivery_<?php print $row->c_code; ?>'/></label>
											<label title="<?php print t($row->c_name); ?>" for='lrn_srch_delivery_<?php print $row->c_code; ?>' class='srch-label-cls highlight-light-blue vtip'><?php $narrTitle = t($row->c_name); print titleController('LNRSEARCH-VIEW-FILTER-TITLE', $narrTitle,15); ?></label>
									</div>
								<?php } ?>
								
								<?php if(!empty($_SESSION['availableFunctionalities']->exp_sp_learning_plan_detail)) { ?>
								
									<?php 
    								  //if ($cache = cache_get('exp_learning_object_type')){
											if (isset($catSrcArr['exp_learning_object_type'])){
                        $object_types = $catSrcArr['exp_learning_object_type'];
                      }else{
                       	$object_types = getLearningObjectType(); 
                       	$catSrcArr['exp_learning_object_type'] = $object_types;
                      }
									//$object_types = getLearningObjectType(); 
									  foreach ($object_types as $id => $row) {
									?>
										<div id='delivery_hideshow_<?php print $row->code; ?>' class='srch-checkbox-container-cls' style='display:block'>
										<?php $narrTitle=''; if(($row->name)=='Curricula') { $narrTitle=t(Curriculum); } else { $narrTitle=t($row->name); } ?>
												<label for='lrn_srch_object_<?php print $row->code; ?>' class='checkbox-unselected' ><input id='lrn_srch_object_<?php print $row->code; ?>' class='ot-others srch-checkbox-cls' type='checkbox'  <?php echo in_array($row->code, $ob_type)?'checked':''; ?>  value='<?php print $row->code; ?>' onClick='$("#lnr-catalog-search").data("lnrcatalogsearch").searchAction(this);' data-filter-label='<?php print $narrTitle; ?>' data-filter-name='training-plan' data-filter-id='lrn_srch_object_<?php print $row->code; ?>'/></label>
												<label title="<?php print $narrTitle; ?>" for='lrn_srch_object_<?php print $row->code; ?>' class='srch-label-cls highlight-light-blue vtip'><?php print titleController('LNRSEARCH-VIEW-FILTER-TITLE-SRCH', $narrTitle,15); ?></label>
										</div>
									<?php } ?>
								
								<?php } ?>
								
							</div>
						</div>
						
						<!--  MRO FILTER  -->
						<?php 
						if (isset($catSrcArr['exp_MRO_type'])){
							$trainings = $catSrcArr['exp_MRO_type'];
						}else{
							$comp = getTrainingCompliance();
							$mand = getMandOrRecTrainningCount('cre_sys_inv_man');
							$recm = getMandOrRecTrainningCount('cre_sys_inv_rec');
							$trainings = array($comp,$mand,$recm);
							$catSrcArr['exp_MRO_type'] = $trainings;
						}
						$items = array_values(array_filter($trainings));
						if(count($items)){
							//if($catalog_reg != 'Course') { // removed this condition for 40364
							global $user;
								if ($user->uid) {							  
									if (isset($catSrcArr['exp_static_training_type'])){
		                $training_types = $catSrcArr['exp_static_training_type'];
		              }else{
		               	$training_types = getStaticTrainingTypes($items);
		               	$catSrcArr['exp_static_training_type'] = $training_types;
                  }
		                                      
									 //$training_types = getStaticTrainingTypes();
									 if(!empty($training_types)){
									?>
								<div id='search_MRO_type' class='search-opt-box'>
									<ul class='find-list-items'>
										<li>
											<span id='mroTypeHideShow' class='cls-show ' onclick='$("#lnr-catalog-search").data("lnrcatalogsearch").showHide("mroTypeHideShow","paintMROType");'>
												<a class='search-heading'><?php print t("LBL037"); ?></a>
											</span>
										</li>
									</ul>
									<div id='paintMROType' class="catalog-criteria-filter-set">
									
										<?php 
										  foreach ($training_types as $key => $val) {
										?>
											<div id='mro_hideshow_<?php print $val->code; ?>' class='srch-checkbox-container-cls' style='display:block'>
													<label for='lrn_srch_mro_<?php print $val->code; ?>' class='checkbox-unselected' ><input id='lrn_srch_mro_<?php print $val->code; ?>' class='mro-others srch-checkbox-cls' type='checkbox'  <?php echo in_array($val->code, $mro_type)?'checked':''; ?> value='<?php print $val->code; ?>' onClick='$("#lnr-catalog-search").data("lnrcatalogsearch").searchAction(this);' data-filter-label='<?php print t($val->name); ?>' data-filter-name='training-mro' data-filter-id='lrn_srch_mro_<?php print $val->code; ?>'/></label>
													<label title="<?php print t($val->name); ?>" for='lrn_srch_mro_<?php print $val->code; ?>' class='srch-label-cls highlight-light-blue vtip'><?php print titleController('LNRSEARCH-VIEW-FILTER-MRO-OTHERS', t($val->name), 20); //print t($val->name); ?></label>
											</div>
										<?php } ?>
																		
									</div>
								</div>
								<?php } 
								} 
							//}

						}?>
						<!--  MRO FILTER END -->
					<?php

					    // Above code commented for getting static language data
							if (isset($catSrcArr['exp_static_language_type'])){
                $all_language_types = $catSrcArr['exp_static_language_type'];
              }else{
                $all_language_types = getAllStaticLanguageTypes();
                $catSrcArr['exp_static_language_type'] = $all_language_types;
              }
                        
              $userPreferedLanguage = getUserPreferredLanguage();

              $prefLang = array();                        
              if(!empty($userPreferedLanguage)) {
                foreach($all_language_types as $key => $value){
                  if(strstr($userPreferedLanguage,$value->language_code)){
                     $prefLang = $value;
                     unset($all_language_types[$key]);                        			
                  }
                }
                if(count($prefLang)){   
                   array_unshift($all_language_types,$prefLang);
                }                
              }
					    //$all_language_types = getAllStaticLanguageTypes();
					    if(!empty($all_language_types) && count($all_language_types)>1):
					    ?>
						<div id='search_language' class='search-opt-box'>
							<ul class='find-list-items'>
								<li>
									<span id='languageHideShow' class='cls-show ' onclick='$("#lnr-catalog-search").data("lnrcatalogsearch").showHide("languageHideShow","paintLanguage");'>
										<a class='search-heading'><?php print t("LBL038"); ?></a>
									</span>
								</li>								
								
								</ul>
							<div id='paintLanguage' class='languagefilter catalog-criteria-filter-set'>
							<span class='searchtext' style="position: relative; clear:both; display:none;">						    
							<input id="language_searchtext" class="ac_input searchtext language" type="text" ondrop="return false;" autocomplete="off" alt="search..." maxlength="70" size="25" name="searchword" value="<?php print t("LBL304");?>" style="color:#999; display:none; border-radius:12px; border:1px solid #e5e5e5; padding-left:5px; font-size:12px;" onkeyup = "searchKeyPress(event,this);">
								    <a class="location-search filter-search-start-date-search-bg" id='language' title="<?php print t("LBL304"); ?>"  style="top:-1px; left:103px;" onclick="filtersearch(this);"> </a>
								</span>
							<div id="paintLanguagescroll">
							<?php
								$langCount = 0;
								$chkStatus = (stristr($_SERVER['HTTP_REFERER'],'search|')) ? '':'checked';
								 if(isset($_GET['callfrom']) && isset($_GET['language'])){
								 	$lg_type = getWidgetSelectedLanguage();
								 	$userPreferedLanguage = '';
								 }
								$language_js = array();
								foreach ($all_language_types as $id => $value) {
									$langCount = $langCount+1; 
									$language_js[] = t($value->language);

									?>
									<div id='srchfilter<?php print str_replace(' ','',t($value->language)); ?>' class = 'lang_filter search_filter_language'>
											<div id='lang_hideshow_<?php print $langCount; ?>' class='srch-checkbox-container-cls' <?php  print 'style="display:block"'; ?>>
  												<label for='lrn_srch_lang_<?php print $value->language_code; ?>' class='checkbox-unselected'><input id='lrn_srch_lang_<?php print $value->language_code; ?>' class='lang-others srch-checkbox-cls' type='checkbox' <?php if($lg_type[0]) { echo in_array($value->language_code, $lg_type)? $chkStatus:''; } else { echo ($value->language_code == $userPreferedLanguage)?$chkStatus:''; }?>  value='<?php print $value->language_code; ?>' onClick='$("#lnr-catalog-search").data("lnrcatalogsearch").searchAction(this);' data-filter-label='<?php print t($value->language);?>' data-filter-name='language' data-filter-id='lrn_srch_lang_<?php print $value->language_code; ?>'/></label>
  												<label title="<?php print t($value->language); ?>" for='lrn_srch_lang_<?php print $value->language_code; ?>' class='srch-label-cls highlight-light-blue vtip'><?php print titleController('LNRSEARCH-VIEW-FILTER-LANG', t($value->language),18); ?></label>
											</div>
										</div>
								<?php } 
								$languages_enabled = json_encode($language_js);
								//expDebug::dPrint('Language variable = '.print_r($languages_enabled, 1),1);
								//expDebug::dPrint('Language count'.$langCount,1); ?>
								<?php //  if ($langCount>4) {?>
							<!--<div id='lang_hideshow_more' class='display-more' onClick='$("#lnr-catalog-search").data("lnrcatalogsearch").moreListDisplay(<?php print $langCount; ?>,"lang_hideshow");'><?php print t("LBL543"); ?></div>
								<div id='lang_hideshow_short' style='display:none;' class='display-short' onClick='$("#lnr-catalog-search").data("lnrcatalogsearch").shortListDisplay(<?php print $langCount; ?>,"lang_hideshow");'><?php print t("LBL544"); ?></div>-->
								<?php // }  ?>
								<input id= 'filterlist_language' type ="hidden" value = "<?php print $langCount ?>"/>
								</div>
							</div>
						</div>
						<?php endif; ?>
					<?php 
					// Above code commented for getting static country data
					
					/*if ($cache = cache_get('exp_static_country_type')){
                		$all_country_types = $cache->data;
                    }else{
                    	$all_country_types = getAllStaticCountry();
                    	cache_set('exp_static_country_type', $all_country_types);
                    }*/
					// 0048813: Display Job role as a narrow filter in the catalog page
					$u_id = getIdOfLoggedInUser();
					if($u_id) {
						$user_jobrole_arr = getUserJobrole($u_id);
						if(count($user_jobrole_arr)>1){
							?>
							<div id='search_jobrole' class='search-opt-box'>
							<ul class='find-list-items'>
								<li>
									<span id='jobroleHideShow' class='cls-show ' onclick='$("#lnr-catalog-search").data("lnrcatalogsearch").showHide("jobroleHideShow","paintJobrole");'>
										<a class='search-heading'><?php print t("LBL133"); ?></a>
									</span>
								</li>
							</ul>
							<div id='paintJobrole' class="catalog-criteria-filter-set">
							<div id='job-role-wrapper'>
								<?php
								 $count = 0;
								 foreach ($user_jobrole_arr as $id => $value) {								   
								 if(!empty($value)) {
								   ?>
									<?php $count = $count+1; ?>
									<div id='jobrole_hideshow_<?php print $count; ?>' class='srch-checkbox-container-cls' <?php  /*if ($count>=5) { print 'style="display:none"'; } else { print 'style="display:block"'; } */ ?>>
											<label for='lrn_srch_jobrole_<?php print $value->job_role; ?>' class='checkbox-unselected'><input id='lrn_srch_jobrole_<?php print $value->job_role; ?>'  class='jobrole-others srch-checkbox-cls' type='checkbox'   <?php echo in_array($value->job_role, $jr_type)?'checked':''; ?> value='<?php print $value->job_role; ?>' onClick='$("#lnr-catalog-search").data("lnrcatalogsearch").searchAction(this);' data-filter-label='<?php print t($value->name); ?>' data-filter-name='jobrole' data-filter-id='lrn_srch_jobrole_<?php print $value->job_role; ?>'/></label>
											<label for='lrn_srch_jobrole_<?php print $value->job_role; ?>' class='srch-label-cls highlight-light-blue vtip' title="<?php print t($value->name); ?>"><?php print titleController('LNRSEARCH-VIEW-FILTER-JOBROLE-NAME', t($value->name),14); //print t($value->name); ?></label>
									</div>
								<?php  }
								  } ?>
								  </div>
								  <input id= 'jobrole_list' type ="hidden" value = "<?php print $count ?>"/>
							</div>
						</div>
						<?php 
						}
					}


					if (isset($catSrcArr['exp_catalog_country'])){
						$all_country_types = $catSrcArr['exp_catalog_country'];
					}else{
						$all_country_types = getAllStaticCountry();
						$catSrcArr['exp_catalog_country'] = $all_country_types;
					}
					if(!empty($all_country_types) && count($all_country_types)>1):
					?>
					<div id='search_country' class='search-opt-box'>
							<ul class='find-list-items'>
								<li>
									<span id='countryHideShow' class='cls-show ' onclick='$("#lnr-catalog-search").data("lnrcatalogsearch").showHide("countryHideShow","paintCountry");'>
										<a class='search-heading'><?php print t("LBL039"); ?></a>
									</span>
									<!-- <span class="reset-img" onclick='$("#lnr-catalog-search").data("lnrcatalogsearch").resetCheckbox("Country");'>Reset</span> -->
								</li>
							</ul>
							<div id='paintCountry' class='countryfilter catalog-criteria-filter-set'>
							<span class='searchtext' style="position: relative; clear:both; display:none;">
								<input id="country_searchtext" class="ac_input searchtext country" type="text" ondrop="return false;" autocomplete="off" alt="search..." maxlength="70" size="25" name="searchword" 
								 value="<?php print t("LBL304");?>" style="color:#999; display:none; border-radius:12px; border:1px solid #e5e5e5; padding-left:5px; font-size:12px;" onkeyup = "searchKeyPress(event,this);">
								    <a class="location-search filter-search-start-date-search-bg" id='country' title="<?php print t("LBL304"); ?>"  style="top:-1px; left:103px;" onclick="filtersearch(this);"> </a>
								</span>
								<div id="paintCountryscroll">
								<?php
								 $count = 0;
								 $country_js = array();
								 foreach ($all_country_types as $id => $value) {								   
								 $country_js[] = t($value->country_name);
								 if(!empty($value)) {
								   ?>
									<?php $count = $count+1; ?>
									<div id='srchfilter<?php print str_replace(' ','',(t($value->country_name))); ?>' class='search_filter_country'>
									<div id='country_hideshow_<?php print $count; ?>' class='srch-checkbox-container-cls' <?php print 'style="display:block"'; ?>>
											<label for='lrn_srch_country_<?php print $value->country_code; ?>' class='checkbox-unselected'><input id='lrn_srch_country_<?php print $value->country_code; ?>'  class='country-others srch-checkbox-cls' type='checkbox'   <?php echo in_array($value->country_code, $cy_type)?'checked':''; ?> value='<?php print $value->country_code; ?>' onClick='$("#lnr-catalog-search").data("lnrcatalogsearch").searchAction(this);' data-filter-label='<?php print t($value->country_name); ?>' data-filter-name='country' data-filter-id='lrn_srch_country_<?php print $value->country_code; ?>'/></label>
											<label for='lrn_srch_country_<?php print $value->country_code; ?>' class='srch-label-cls highlight-light-blue vtip' title='<?php print t($value->country_name); ?>'><?php ($theme_key == 'expertusoneV2')? print titleController('LNRSEARCH-COUNTRY-NAME',t($value->country_name),15): print titleController('FADEOUT',t($value->country_name),19); ?></label> <!-- Changed by ganeshbabuv for #0078236 -->
									</div>
									</div>
								<?php  }
								  } ?>
								  <?php 
								//  expDebug::dPrint('Country count--->joola'.$count,1);
								  $countries_enabled = json_encode($country_js);
								//  expDebug::dPrint('Country variable = '.print_r($countries_enabled, 1),1);?>
								<?php // if ($count>4) {?>
							<!-- <div id='country_hideshow_more' class='display-more' onClick='$("#lnr-catalog-search").data("lnrcatalogsearch").moreListDisplay(<?php print $count; ?>,"country_hideshow");'><?php print t("LBL543"); ?></div>
								<div id='country_hideshow_short' style='display:none;' class='display-short' onClick='$("#lnr-catalog-search").data("lnrcatalogsearch").shortListDisplay(<?php print $count; ?>,"country_hideshow");'><?php print t("LBL544"); ?></div> -->
								<?php // }  ?>
							</div>
						</div>
							<input id= 'filterlist_country'  type ="hidden" value = "<?php print $count ?>"/>
						</div>
						<?php  endif;
						?>
						
				    <?php if(!empty($_SESSION['availableFunctionalities']->exp_sp_commerce)) { ?>
					<div id='search_price' class='search-opt-box' style='display:block'>
						<ul class='find-list-items'>
							<li>
								<span id='priceHideShow' class='cls-show ' onclick='$("#lnr-catalog-search").data("lnrcatalogsearch").showHide("priceHideShow","paintPrice");'>
								<a class='search-heading'><?php print t("LBL040"); ?></a></span>
							</li>
						</ul>
						<div id="paintPrice" class="catalog-criteria-filter-set">
							<div id='price_hideshow' class='srch-checkbox-container-cls' style='display:block' unselectable="on">
								<?php if($theme_key == 'expertusoneV2') { ?>
									<div id="price-slider-range" style="width: 109px;" data-filter-label='<?php print t("LBL040"); ?>' data-filter-name='price-slider' data-filter-id="price-slider-range"></div>
								<?php } else { ?>							
					        		<div id="price-slider-range" style="margin-left:8px;width: 141px;" data-filter-label='<?php print t("LBL040"); ?>' data-filter-name='price-slider' data-filter-id="price-slider-range"></div>
					        	<?php } ?>
					        	<input  readonly type="text" id="price-slide-left" onKeyDown="preventBackspace();" />
					        	<input  readonly type="text" id="price-slide-right" onKeyDown="preventBackspace();" />
				
					        	<!-- <input type="text" id="price-slide" style="padding-top:10px;font-size:12px;border:0;height:20px;text-align:center;color:#333333;font-weight:bold;" /> -->
					        </div>
				        </div>
					</div>    							
           			<?php } ?>
           					
           			  <div id='search_startdate' class='search-opt-box' style='display:block'>
							<ul class='find-list-items'>
								<li>
									<span id='startdateHideShow' class='cls-show ' onclick='$("#lnr-catalog-search").data("lnrcatalogsearch").showHide("startdateHideShow","paintStartDate");'>
										<a class='search-heading'><?php print t("LBL042"); ?> <span class="date-range-heading"> (<?php print t("LBL043"); ?>)</span> </a>
									</span>
									<span id='date-clr' class='clr-txt' style='display:none' onclick='$("#lnr-catalog-search").data("lnrcatalogsearch").clearField("Date");'><?php print t("LBL307"); ?></span>
								</li>
							</ul>
							<div id='paintStartDate' class="catalog-criteria-filter-set">
								  <div style="display: block;  font-size: 12px;" id="showDateRange">
    								  <div class="catalog-start-date">
    								  	<div class="filter-search-start-date-left-bg"></div>
    									  <input type="text" value="<?php echo $startdate[0]==''? t('LBL251').':'.t('LBL112'):$startdate[0];?>" class="catalog-start-date-field filter-search-start-date-middle-bg"  onblur="$('#lnr-catalog-search').data('lnrcatalogsearch').hightlightedText(event,'ad_startdate1', '<?php print addslashes(t('LBL251').':'.t("LBL112")); ?>')" onfocus="$('#lnr-catalog-search').data('lnrcatalogsearch').hightlightedText(event,'ad_startdate1','<?php print addslashes(t('LBL251').':'.t("LBL112")); ?>')"onchange="$('#lnr-catalog-search').data('lnrcatalogsearch').hightlightedText(event,'ad_startdate1','<?php print addslashes(t('LBL251').':'.t("LBL112")); ?>')" name="ad_startdate1" id="ad_startdate1">
    									  <div class="filter-search-start-date-right-bg"></div>
    									  <?php if($enddate[0]!=''){?>
    									  	<script type="text/javascript">$('#ad_startdate1').css('color','#222222').css('fontSize','13px').css('fontStyle','normal');</script>
    									  <?php } ?>
    								  </div>    								 
    								  <div class="catalog-start-date">
    								  	<div class="filter-search-start-date-left-bg"></div>
    									   <input type="text" value="<?php echo $enddate[0]==''? t('LBL113'):$enddate[0];?>" class="catalog-start-date-field filter-search-start-date-middle-bg" onblur="$('#lnr-catalog-search').data('lnrcatalogsearch').hightlightedText(event,'ad_startdate2','<?php print addslashes(t('LBL113')); ?>')"onfocus="$('#lnr-catalog-search').data('lnrcatalogsearch').hightlightedText(event,'ad_startdate2','<?php print addslashes(t('LBL113')); ?>')"onchange="$('#lnr-catalog-search').data('lnrcatalogsearch').hightlightedText(event,'ad_startdate2','<?php print addslashes(t('LBL113')); ?>')" name="ad_startdate2" id="ad_startdate2">
    									 <div class="filter-search-start-date-right-bg"></div>
    									  <?php if($enddate[0]!=''){?>
    									  	<script type="text/javascript">$('#ad_startdate2').css('color','#222222').css('fontSize','13px').css('fontStyle','normal');$('#date-clr').css('display','block');</script>
    									  <?php } ?>
    									  
    								  </div>
    								  <div class="catalog-date-format">
    								  	  <div class="curved-blue-button-left"></div> 
    									  <span class="date-refersh-img" onClick='$("#lnr-catalog-search").data("lnrcatalogsearch").dateValidationCheck();' data-filter-label='<?php print t("LBL042"); ?>' data-filter-name='date-range' data-filter-id='date-range'><?php print t("LBL669"); ?></span>
    									  <div class="curved-blue-button-right"></div>
    								  </div> 
								  </div>
							</div>
						</div>
						
						<?php
							if (isset($catSrcArr['exp_catalog_tagcloud'])){
								$tags_for_cloud = $catSrcArr['exp_catalog_tagcloud'];
							}else{
								$tags_for_cloud = getWeightedTags(array('Course','Class','Certification','Curricula','Learning Plan'));
								$catSrcArr['exp_catalog_tagcloud'] = $tags_for_cloud;
							}
						  if (!empty($tags_for_cloud)):
						?>
						<div id='search_tag' class='search-opt-box' style='display:block'>
							<ul class='find-list-items'>
								<li>
									<span id='tagHideShow' class='cls-show ' onclick='$("#lnr-catalog-search").data("lnrcatalogsearch").showHide("tagHideShow","paintTag");'>
										<a class='search-heading'><?php print t("LBL191"); ?></a>
									</span>
									<span id='tag-clr' class='clr-txt' style='display:none' onclick='$("#lnr-catalog-search").data("lnrcatalogsearch").clearField("Tag");'><?php print t("LBL307"); ?></span>
								</li>
							</ul>
							<div id='paintTag' class="catalog-criteria-filter-set">
							  <div class='lnr-tags-cloud-container-cls'>
							    <input type='text' id='srch_criteria_tag' name='srch_criteria_tag' value='' style='display:none;' data-filter-id='cloud-tag'  data-filter-name='cloud-tag' data-filter-label='<?php print t("LBL191"); ?>'/>
							    <div id='tags_cloud' class='tagscloud-scrollable taglink'>
							      <p id="tagcloudcontainerid" class="cloudtagsp">
                    <?php  $i =1; $cnt =0; $existingTagName=array();
                    $char = array('1'=>17, '2'=> 15,'3'=>12, '4'=> 11,'5'=>8,'6'=>7,'7'=>6,'8'=>5);
                    foreach ($tags_for_cloud as $tagName => $weight): 
                    if($i <= 7){
                    	$existingTagName[] = $tagName;
                    ?>
                    <span class='tagscloud-term'>
                      <a class="tagscloud-tag level<?php print $weight; ?>" href="javascript:void(0);"
                                     onclick="$('#lnr-catalog-search').data('lnrcatalogsearch').underlineTagAndTriggerSearch('<?php print rawurlencode($tagName); ?>', this);"><?php (count($tags_for_cloud) > 7) ? print titleController('FADEOUT', $tagName,$char[$weight]) : print $tagName; ?></a>
                    </span>
                    <?php 
                    }
                    $i++; $cnt++;
                     endforeach; ?>
                    <?php if($tag[0]!=''){?>
  									  	<script type="text/javascript">
  									  		$('#srch_criteria_tag').val('<?php echo $tag[0];?>');
  									  	  $('#tag-clr').css('display', 'block');
  									  		$("#tags_cloud a").filter(function() {
  									      	return $(this).text() === "<?php echo $tag[0];?>";
  									  		}).css('text-decoration', 'underline');  									  	
  									  	</script>
  									  <?php } ?>
                    </p>
                    <div id='tagtip'>
                    <div id= 'inner-tag-tip'>
                    
                    </div>
                    <div id= 'inner-tag-tip-dup' style='display: none;'>
                    <?php  $j=1;
                    foreach ($tags_for_cloud as $tagName => $weight): ?>
                    <span class='tagscloud-term'>
                      <a id="tag<?php print $j;?>" class="tagscloud-tag level<?php print $weight; ?>" href="javascript:void(0);"
                                     onclick="$('#lnr-catalog-search').data('lnrcatalogsearch').underlineTagCloudAndTriggerSearch('<?php print rawurlencode($tagName); ?>', this, <?php print $weight; ?>);"><?php print $tagName; ?></a>
                    </span>
                    <?php $j++; endforeach; ?>
                    </div>
                    </div>
                  </div>
		            </div>
							</div>
						</div>
						<?php endif;?>
						<div id='search_location' class='search-opt-box search-opt-box-lastitem' style='display:block'>
							<ul class='find-list-items'>
								<li>
									<span id='locationHideShow' class='cls-show ' onclick='$("#lnr-catalog-search").data("lnrcatalogsearch").showHide("locationHideShow","paintLocation");'>
										<a class='search-heading'><?php print t("Location"); ?></a>
									</span>
									<span id='location-clr' class='clr-txt' style='display:none' onclick='$("#lnr-catalog-search").data("lnrcatalogsearch").clearField("Location");'><?php print t("LBL307"); ?></span>
								</li>
							</ul>
							<div id='paintLocation' class="catalog-criteria-filter-set">
								<div id='location_hideshow' class='srch-checkbox-container-cls' style='display:block'>
									<div class="filter-search-start-date-left-bg"></div>
									<!-- Added/changed by ganeshbabuv on Jan 11th 2016 for  Location Filter #66404 -->
									 <input type='text' name='srch_criteria_location' class='ac_input searchlocation filter-search-start-date-middle-bg loc_catalogpage_ac' size='15' id='srch_criteria_location' autocomplete='on' onkeypress='$("#lnr-catalog-search").data("lnrcatalogsearch").locationEnterKey();this.style.fontSize="13px";this.style.fontStyle="normal";' value="<?php echo $location[0]==''? t('LBL1321') : trim($location[0]);?>" onblur="$('#lnr-catalog-search').data('lnrcatalogsearch').hightlightedText(event, 'srch_criteria_location','<?php print addslashes(t("LBL1321")); ?>')" onfocus="$('#lnr-catalog-search').data('lnrcatalogsearch').hightlightedText(event, 'srch_criteria_location','<?php print addslashes(t("LBL1321")); ?>')" onchange="$('#lnr-catalog-search').data('lnrcatalogsearch').hightlightedText(event, 'srch_criteria_location','<?php print addslashes(t("LBL1321")); ?>')" data-filter-label='<?php print t("Location"); ?>' data-filter-name='location' data-filter-id='srch_criteria_location'>
									<a class='location-search filter-search-start-date-search-bg' title='<?php print t("LBL304"); ?>' onClick='$("#lnr-catalog-search").data("lnrcatalogsearch").searchActionLocation();'>&nbsp;</a>
  									  <?php if($location[0]!=''){?>
  									  	<script type="text/javascript">$('#srch_criteria_location').css('color','#222222').css('fontSize','13px').css('fontStyle','normal');$('#location-clr').css('display','block');</script>
  									  <?php } ?>
  									  <div class="filter-search-start-date-right-bg"></div>
								</div>
							</div>
						</div>
						
				
  			    <?php if(!empty($_SESSION['availableFunctionalities']->exp_sp_fivestar)) {?>	  
						
						<div id='search_rating' class='search-opt-box search-opt-box-rating-item' style='display:block'>
									<ul class='find-list-items'>
										<li>
											<span id='ratingHideShow' class='cls-show ' onclick='$("#lnr-catalog-search").data("lnrcatalogsearch").showHide("ratingHideShow","paintRating");'>
												<a class='search-heading'><?php print strtoupper(t("Rating")); ?></a>
											</span>
										</li>
									</ul>
									<div id='paintRating' class="catalog-criteria-filter-set">
											<div id='rating_hideshow' class='srch-checkbox-container-cls' style='display:block'>
												<label for='lrn_srch_rating' class='checkbox-unselected' >
												<?php $checkedSts = (in_array(20, $rating_type)) ? 'checked=""' : '';  ?>
													<input id='lrn_srch_one_rating' class='rating-others srch-checkbox-cls' type='checkbox' <?php echo $checkedSts;?> value='20' onClick='$("#lnr-catalog-search").data("lnrcatalogsearch").searchAction(this);' data-filter-label='1 <?php print t("Rating"); ?>' data-filter-name='rating' data-filter-id='lrn_srch_one_rating'/>
												</label>
												<span class='lrn_srch_rating'><?php print theme('fivestar_static', array('rating' => 20,'stars' => 5,'startype' => 'star_common'));?></span>
											</div>	
											<div id='rating_hideshow' class='srch-checkbox-container-cls' style='display:block'>
												<label for='lrn_srch_rating' class='checkbox-unselected' >
												<?php $checkedSts = (in_array(40, $rating_type)) ? 'checked=""' : '';  ?>
													<input id='lrn_srch_two_rating' class='rating-others srch-checkbox-cls' type='checkbox' <?php echo $checkedSts;?> value='40' onClick='$("#lnr-catalog-search").data("lnrcatalogsearch").searchAction(this);' data-filter-label='2 <?php print t("Rating"); ?>' data-filter-name='rating' data-filter-id='lrn_srch_two_rating'/>
												</label>
												<span class='lrn_srch_rating'><?php print theme('fivestar_static', array('rating' => 40,'stars' => 5,'startype' => 'star_common'));?></span>
											</div>	
											<div id='rating_hideshow' class='srch-checkbox-container-cls' style='display:block'>
												<label for='lrn_srch_rating' class='checkbox-unselected' >
												<?php $checkedSts = (in_array(60, $rating_type)) ? 'checked=""' : '';  ?>
													<input id='lrn_srch_three_rating' class='rating-others srch-checkbox-cls' type='checkbox' <?php echo $checkedSts;?> value='60' onClick='$("#lnr-catalog-search").data("lnrcatalogsearch").searchAction(this);' data-filter-label='3 <?php print t("Rating"); ?>' data-filter-name='rating' data-filter-id='lrn_srch_three_rating'/>
												</label>
												<span class='lrn_srch_rating'><?php print theme('fivestar_static', array('rating' => 60,'stars' => 5,'startype' => 'star_common'));?></span>
											</div>	
											<div id='rating_hideshow' class='srch-checkbox-container-cls' style='display:block'>
												<label for='lrn_srch_rating' class='checkbox-unselected' >
												<?php $checkedSts = (in_array(80, $rating_type)) ? 'checked=""' : '';  ?>
													<input id='lrn_srch_four_rating' class='rating-others srch-checkbox-cls' type='checkbox' <?php echo $checkedSts;?> value='80' onClick='$("#lnr-catalog-search").data("lnrcatalogsearch").searchAction(this);' data-filter-label='4 <?php print t("Rating"); ?>' data-filter-name='rating' data-filter-id='lrn_srch_four_rating'/>
												</label>
												<span class='lrn_srch_rating'><?php print theme('fivestar_static', array('rating' => 80,'stars' => 5,'startype' => 'star_common'));?></span>
											</div>	
											<div id='rating_hideshow' class='srch-checkbox-container-cls' style='display:block'>
												<label for='lrn_srch_rating' class='checkbox-unselected' >
												<?php $checkedSts = (in_array(100, $rating_type)) ? 'checked=""' : '';  ?>
													<input id='lrn_srch_five_rating' class='rating-others srch-checkbox-cls' type='checkbox' <?php echo $checkedSts;?> value='100' onClick='$("#lnr-catalog-search").data("lnrcatalogsearch").searchAction(this);' data-filter-label='5 <?php print t("Rating"); ?>' data-filter-name='rating' data-filter-id='lrn_srch_five_rating'/>
												</label>
												<span class='lrn_srch_rating'><?php print theme('fivestar_static', array('rating' => 100,'stars' => 5,'startype' => 'star_common'));?></span>
											</div>
										
									</div>
						</div>
					<?php } ?>										
						
						</div>
					</div>
				</div>
			</td>
		</tr>
	</tbody>
</table>
<script language="Javascript" type="text/javascript"> 
  if(document.getElementById('lnr-catalog-search')) {
      $("#lnr-catalog-search").data("lnrcatalogsearch").paintAfterReady();
      $("#lnr-catalog-search").data("lnrcatalogsearch").paintLocationAutocomplete();
      var languagesEnabled = <?php print empty($languages_enabled) ? '[]' : $languages_enabled; ?>;
      var countriesEnabled = <?php print empty($countries_enabled)? '[]' : $countries_enabled; ?>;
      $("#lnr-catalog-search").data("lnrcatalogsearch").paintAutocomplete('language_searchtext', languagesEnabled);
	  $("#lnr-catalog-search").data("lnrcatalogsearch").paintAutocomplete('country_searchtext', countriesEnabled);
  }
</script>

<?php 
if($isCached == FALSE){
	$cacheTime = time() + (3600 * 5); // cache expire time to 5 hours.
	cache_set('catlog_src_filters',$catSrcArr,'cache',$cacheTime);
}
if(!empty($_SESSION['availableFunctionalities']->exp_sp_commerce)) {

	$priceRng = getSliderPriceRange();
	$startprice = $priceRng['startprice'];
	$endprice_set = $priceRng['endprice'];
	$UserDefacultCurrencyArr = getUserDefaultCurrency();
	$sitePreferredcurrency = getCurrencyDefDetails();

	$UserPrefCurrCode = $UserDefacultCurrencyArr['currency_code'];
	$endprice = db_query('SELECT slf_convert_price(:price,:type,:code) AS price',
			array(':price'=>$endprice_set,':type'=>$sitePreferredcurrency,':code'=>$UserPrefCurrCode))->fetchField();
	$endprice = floatval(round($endprice));
	$count_digit = strlen((string) $endprice);
	$increment = pow(10, ($count_digit-2));
	$endprice = (int) ($increment * ceil($endprice / $increment));
}

?>

<?php if(!empty($_SESSION['availableFunctionalities']->exp_sp_commerce)) { ?>
<script language="Javascript" type="text/javascript">
	var price_max_value = '<?php print $endprice;?>';
  	$("#lnr-catalog-search").data("lnrcatalogsearch").priceSlider(<?php print $startprice;?>,<?php print $endprice;?>);
</script>
<?php } ?>


<script language="Javascript" type="text/javascript">
var counttag = <?php echo empty($cnt) ? 0 : $cnt;?>;
var existtagArr  = <?php echo '["' . implode('", "', array_map('escape_string', $existingTagName)) . '"]' ?>;
var weightArr  = <?php echo '["' . implode('", "', ($char)) . '"]' ?>;
$(function(){
    $('.taglink').hover(
    		function(){
        		try{
    			var findSFpage=$('body').hasClass("salesforce-widget");
            	if(counttag > 7){
            		$('#inner-tag-tip').html('');
            	if(counttag < 15 && counttag > 7){
            		if(findSFpage=true){
                    $('#tagtip').width(200).height(200).css('left', ' ').css('min-height', '150px');
            		}else{
            		$('#tagtip').width(200).height(200).css('left', '-39px').css('min-height', '150px');	
            		}
                 }else if(counttag < 25){
                	 if(findSFpage=true){
                        $('#tagtip').width(320).height(300).css('left', ' ');
                	 }else{
                		 $('#tagtip').width(320).height(300).css('left', '-50px');
                	 }
                    }else{
                    	if(findSFpage=true){
                        $('#tagtip').width(480).height(390).css('left', ' ').css('max-height', '390px');
                    	}else{
                    	$('#tagtip').width(480).height(390).css('left', '-120px').css('max-height', '390px');	
                    	}
                    }
            	$('#tagtip').fadeIn();
                $('#inner-tag-tip-dup .tagscloud-term').each(function() {
                	$(this).clone().appendTo('#inner-tag-tip');
                    var innerTag = parseInt($('#inner-tag-tip').attr('scrollHeight'));
                    var tagTipDiv = parseInt($('#tagtip').attr('clientHeight')) - 10;
                    if(tagTipDiv <= innerTag) {
                    	$('#inner-tag-tip .tagscloud-term').last().remove();
                    	return false;
                    }
                });
                $('#tagtip').height('auto');
            	}
    		}catch(e){
            		//console.log(e);
            		}
    		},
    		function(){
    		  $('#tagtip').hide();

    		}
    		);
});
</script>