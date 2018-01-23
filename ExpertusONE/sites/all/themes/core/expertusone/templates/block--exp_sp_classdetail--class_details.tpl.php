<?php
/**
 * @file
 * Theme implementation to display a block.
 *
 * Available variables:
 * - $title: Block title.
 * - $content: Block content.
 * - $block->module: Module that generated the block.
 * - $block->delta: An ID for the block, unique within each module.
 * - $block->region: The block region embedding the current block.
 * - $edit_links: A list of contextual links for the block. It can be
 *   manipulated through the variable $edit_links_array from preprocess
 *   functions.
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. The default values can be one or more of the
 *   following:
 *   - block: The current template type, i.e., "theming hook".
 *   - block-[module]: The module generating the block. For example, the user
 *     module is responsible for handling the default user navigation block. In
 *     that case the class would be "block-user".
 *   - first: The first block in the region.
 *   - last: The last block in the region.
 *   - region-count-[x]: The position of the block in the list of blocks in the
 *     current region.
 *   - region-odd: An odd-numbered block of the list of blocks in the current
 *     region.
 *   - region-even: An even-numbered block of the list of blocks in the current
 *     region.
 *   - count-[x]: The position of the block in the list of blocks on the current
 *     page.
 *   - odd: An odd-numbered block of the list of blocks on the current page.
 *   - even: An even-numbered block of the list of blocks on the current page.
 *
 * Helper variables:
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 * - $edit_links_array: An array of contextual links for the block.
 * - $block_zebra: Outputs 'odd' and 'even' dependent on each block region.
 * - $zebra: Same output as $block_zebra but independent of any block region.
 * - $block_id: Counter dependent on each block region.
 * - $id: Same output as $block_id but independent of any block region.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 * - $block_html_id: A valid HTML ID and guaranteed unique.
 *
 * @see template_preprocess()
 * @see zen_preprocess()
 * @see template_preprocess_block()
 * @see zen_preprocess_block()
 * @see zen_process()
 */

global $user;
$record = $block->results['details'];
if($record->code){
	$attachedRecord = $block->results['attachments'];
	$courseInfo  = $block->results['course_details'];
	$numAttachments = count($attachedRecord);
	$availableSeats = $record->availableSeats;
	$classId = arg(2);
	$userId = getSltpersonUserId();
	
	$passData = "data={'NodeId':'".$record->node_id."','ClassId':'".$classId."','CourseId':'".$record->crs_id."'}";
	$hideDesc= 'show';
	if( !empty($_SESSION['widget']['catalog_display_parameters']) &&  $_SESSION['widget']['catalog_display_parameters']['show_description']== false){
		$hideDesc = 'hide';
	}expDebug::dPrint('widget Class desc: '.print_r($_SESSION['widget'], true).'$hideDesc : '.$hideDesc, 4);
	//$record->enrolled_id = getRegisteredOrNot($record->crs_id,$classId);
	
	$registerEndDateCheck = registerEndDateCheck($record->crs_id,$classId);
	
	$mroImageClassArr['cre_sys_inv_com'] =  '<div class="catalog-course-compliance-role-bg"><span class="catalog-course-compliance-bg-left"></span><span class="catalog-course-compliance-bg-middle">'.t('Compliance').'</span><span class="catalog-course-compliance-bg-right"></span></div>';	
	$mroImageClassArr['cre_sys_inv_man'] =  '<div class="catalog-course-role-access-bg"><span class="catalog-course-mandatory-bg-left"></span><span class="catalog-course-mandatory-bg-middle">'.t('Mandatory').'</span><span class="catalog-course-mandatory-bg-right"></span></div>';
	$mroImageClassArr['cre_sys_inv_opt'] =  '<div class="catalog-course-role-access-bg"><span class="catalog-course-optional-bg-left"></span><span class="catalog-course-optional-bg-middle">'.strtolower(t('Optional')).'</span><span class="catalog-course-optional-bg-right"></span></div>';
	$mroImageClassArr['cre_sys_inv_rec'] =  '<div class="catalog-course-role-access-bg"><span class="catalog-course-recommended-bg-left"></span><span class="catalog-course-recommended-bg-middle">'.t('Recommended').'</span><span class="catalog-course-recommended-bg-right"></span></div>';
	$mroImageClass = empty($record->mro_id) ? '' : $mroImageClassArr[$record->mro_id];
	$mroImageClass .= '<div class="clearBoth"></div>';
    if($block->results['details']->is_compliance==1){
          $mroImageClass = $mroImageClassArr['cre_sys_inv_com'];
    }    
    if(isset($_SESSION['widgetCallback']) && $_SESSION['widgetCallback']==TRUE){
    	$showTitle = widgetTitleController($title, 'view_detail_page',$block->results['details']->is_compliance);
    }
    else{
		  if (empty($record->mro_id)) {
		  	if($block->results['details']->is_compliance) {
		  		$showTitle = titleController('BLOCK-EXP-SP-CLASSDETAILS-COMPLIANCE-TITLE',$title,72);
		  	}
		  }
		  else {
		  $recomm_course = 0;
		  	$mand_course = 0;
		  	$mrovalue= explode(',',$record->mro_id);
		  	if (in_array('cre_sys_inv_rec', $mrovalue)){
		  		$recomm_course = 1;
		  	}
		  	if (in_array('cre_sys_inv_man', $mrovalue)){
		  		$mand_course = 1;
		  	}
		  	if($recomm_course == 1) {
		  		$showTitle = titleController('BLOCK-EXP-SP-CLASSDETAILS-RECOMMENDED-TITLE', $title,100);
		  	}
		  	else if($mand_course == 1) {
		  		$showTitle = titleController('BLOCK-EXP-SP-CLASSDETAILS-MANDATORY-TITLE', $title,100);
		  	}
		  	else {
		  	$showTitle = titleController('BLOCK-EXP-SP-CLASSDETAILS-TITLE', $title,70);
		  	}
		  }
   }
  
	$tags  = getCatalogTags($classId,'Class');
	$tagString  = '';
	if(!empty($tags)){
	  $tagString  = implode(", ",$tags);
	}
	// Change the back link path
	$linkPath = checkWidgetUrl();
	?>
	
	<div id="<?php print $block_html_id; ?>" class="<?php print $classes; ?>">
		<div class='block-title-left'>
	 		<div class='block-title-right'>
	 			<div class='block-title-middle'>
	 				<?php if ($title): ?>
	 					<div class="vtip" title = "<?php print sanitize_data($title);?>" style="float:left;"><h2 class='block-title class-delivery-title'><?php print $showTitle;?></h2></div>
	 					<?php print $mroImageClass;?>
	 				 <?php endif; ?>
	 				 <div class="back-btn-container">
	                                        <?php 
	                                    	  print l('['.t("LBL212").']', $linkPath, 
	                                    	  array('attributes' => array('class' => 'detail-back-button','title' => t("LBL212"))));
	                                        ?>                    					
	                    				</div>
	 			</div>
	 		</div>
	 	</div>
	    <div class="content">
	     <div class="region-sidebar-widget-bg">
	    	<div class="class-details-content" id="class_detail_content">
	    		<div id="class-details-data">
	                  <div id="class_details_display_area" class="class-detail-display-area">
	                    	<div class="detail-item-row">
	                    		<div class="class-detail-item-row-left">
	                        		<div class="para">
	                        			<div class="code-container code">
	                        				<span class="detail-code"><?php print t("LBL096"); ?>:</span>
	                        				<span class="detail-desc vtip" title="<?php print $record->code; ?>">
	                        					<?php print titleController('CLASS-DETAIL-CLASS-CODE',$record->code,20); ?>                        					
	                        				</span>                        				
	                      				</div>
	                      			</div>
	                        		<div class="para">
	                        			<div class="code-container info">
	                        				<span class="detail-code"><?php print t("LBL246"); ?>:</span>
	                        				<span class="detail-desc">
	                        					<?php print $courseInfo->title; ?>&nbsp;|&nbsp;<?php print $courseInfo->code; ?>                        					
	                        				</span>                      				
	                      				</div>
	                      			</div>
	                      			<?php if($hideDesc != 'hide'){ // Hide the Description?>
	                        		<div class="para">
	                        			<div class="code-container cdescription">
	                        				<span class="detail-code"><?php print t("LBL229"); ?>:</span>
	                        				<span class="detail-desc"><?php print str_replace(array("\n","\r"), array("<br>","&nbsp;"), $record->description);?></span>
	                        			</div>
	                        		</div>
	                        		<?php } ?>
	                        		<div class="para">
	                        			<div class="code-container tags">
	                        				<span class="detail-code"><?php print t("LBL191"); ?>:</span>
	                        				<span class="detail-desc"><?php print $tagString;?></span>
	                        			</div>
	                        		</div>
	                        		<div class="para">
	                        			<div class="code-container rating">
	                        				<span class="detail-code"><?php print t("Rating"); ?>:</span>
	                        				<span class="detail-desc" style="display:none;">&nbsp;</span>
	                        			</div>
	                        				<div id="ratingBlock" class="rating-block">
	                        					<?php 
			                        				if($record->comp_status == "lrn_crs_cmp_cmp"){
			                        					$enableRate = 'enable';
			                        				}else{
			                        					$enableRate = 'disable';
			                        				}
													$settings = array(                    					
															  'content_type' => 'Class',
															  'content_id' => $record->node_id,
															  'stars' => 5,
															  'autosubmit' => TRUE,
															  'allow_clear' => FALSE,
															  'required' => FALSE,
															  'tag' => 'vote',
		                    								  'style' => 'average',
		      												  'text' => 'average',
		                    								  'enable_rate' => $enableRate
		                    								  //'feedback_enable' => TRUE
															);
		                        					 print "<div class='clsdet-node'>".drupal_render(drupal_get_form('fivestar_rating_widget', '', $settings))."</div>"; 
		                        			   	?>
	                        			   </div>
	                        		</div>
	                        		
                                      <?php if (!empty($record->prerequisites)): ?>
	                                        <div class="para">
	                                          <div class="code-container prereq">
	                                              <span class="detail-code"><?php print t("LBL230"); ?>:</span>
	                                              <span class="detail-desc">
	                                                <?php $preCount = count($record->prerequisites);?>
	                                                <?php if($preCount > 1):?>
	                                                  <ul>
	                                                    <?php foreach ($record->prerequisites as $row): ?>
	                                                      <li>
	                                                     	<span id="course-detail-desc" class="head vtip" title="<?php print $row->crs_title;?>"><?php print titleController('CLASS-DETAIL-PREREQUISITIES-TITLE',  $row->crs_title,30); ?></span>
	                                                      	<span id="course-detail-desc" class="crscode vtip" title="<?php print $row->crs_code;?>">- (<?php print titleController('CLASS-DETAIL-PREREQUISITIES-CODE',  $row->crs_code,15); ?>)</span>
	                                                      </li>
	                                                    <?php endforeach; ?>
	                                                  </ul>
	                                                </span>  
	                                                <?php else :?> 
	                                                <span class="detail-desc">                  
	                                              <?php foreach ($record->prerequisites as $row): ?> 
	                                                  <span id="course-detail-desc" class="head vtip" title="<?php print $row->crs_title;?>"><?php print titleController('CLASS-DETAIL-PREREQUISITIE-TITLE',  $row->crs_title,30); ?></span>
	                                                  <span id="course-detail-desc" class="crscode vtip" title="<?php print $row->crs_code;?>">- (<?php print titleController('CLASS-DETAIL-PREREQUISITIE-CODE',  $row->crs_code,15); ?>)</span>
	                                              <?php endforeach; ?>
	                                              </span> 
	                                              <?php endif;?>                                     
	                                            <div class="clearBoth"></div>
	                                          </div>
	                                        </div>
	                                     <?php endif; ?>
	                                     <?php if (!empty($record->equivalence)): ?>
	                                        <div class="para">
	                                          <div class="code-container equiv">
	                                              <span class="detail-code"><?php print t("LBL247"); ?>:</span>
												  <span class="detail-desc">
	                                                <?php $preCount = count($record->equivalence);?>
	                                                <?php if($preCount > 1):?>
	                                                  <ul>
	                                                    <?php foreach ($record->equivalence as $row): ?>
	                                                      <li>
	                                                      <span id="course-detail-desc" class="head vtip" title="<?php print $row->crs_title; ?>"><?php print titleController('CLASS-DETAIL-EQUIVALENCES-TITLE',$row->crs_title,20); ?></span>
	                                                      <span id="course-detail-desc" class="crscode vtip" title="<?php print $row->crs_code; ?>">- (<?php print titleController('CLASS-DETAIL-EQUIVALENCES-CODE',$row->crs_code,20); ?>)</span>
	                                                      </li>
	                                                    <?php endforeach; ?>
	                                                  </ul>
	                                                <?php else :?>                   
	                                              <?php foreach ($record->equivalence as $row): ?> 
	                                                  <span id="course-detail-desc" class="head vtip" title="<?php print $row->crs_title; ?>"><?php print titleController('CLASS-DETAIL-EQUIVALENCE-TITLE',$row->crs_title,20); ?></span>
	                                                  <span id="course-detail-desc" class="crscode vtip" title="<?php print $row->crs_code; ?>">- (<?php print titleController('CLASS-DETAIL-EQUIVALENCE-CODE',$row->crs_code,20); ?>)</span>
	                                              <?php endforeach; ?>
	                                              <?php endif;?>  
	                                              </span>                                   
	                                          </div>
	                                        </div>
	                                     <?php endif; ?>
	                                    <?php if(!empty($record->duration) && $record->duration>0):?>
	                                		<div class="para">
	                                			<div class="code-container duration">
	                                				<span class="detail-code"><?php print t("LBL248"); ?>:</span>
	                                				<span class="detail-desc"><?php print formatDuration($record->duration);?></span>
		                                		</div>
	    	                          		</div>
	    	                          		<?php endif; ?>
	                                		<div class="para">
	                                			<div class="code-container language">
	                                				<span class="detail-code"><?php print t("LBL038"); ?>:</span>
	                                				<span class="detail-desc"><?php print t(getProfileListItemName($record->clslang));?></span>
	                                			</div>
	                                		</div>
	                                		<div class="para">
	                                			<div class="code-container delivery">
	                                				<span class="detail-code"><?php print t("LBL084"); ?>:</span>
	                                				<span class="detail-desc"><?php 
	                                				$deliverytypeVal=getProfileListItemAttr($record->clsdeliverytype);
	                                				$row_del_type='';
	                                				if ($deliverytypeVal == '' || $deliverytypeVal == '-') {
	                                					return '<span>&nbsp;</span>';
	                                				}
	                                				else if($deliverytypeVal == 'WBT'){
	                                					$row_del_type = t("Web-based");
	                                				}
	                                				else if($deliverytypeVal == 'VC'){
	                                					$row_del_type = t("Virtual Class");
	                                				}
	                                				else if($deliverytypeVal == 'ILT'){
	                                					$row_del_type = t("Classroom");
	                                				}
	                                				else if($deliverytypeVal == 'VOD'){
	                                					$row_del_type = t("Video");
	                                				}
	                                				print $row_del_type;?></span>
	                                			</div>
	                                		</div>
	                                		<?php if($record->clsdeliverytype == 'lrn_cls_dty_wbt') {?>
	                                		<div class="para">
	                                		  <div class="code-container lesson">
	                                		    <?php $lessonCnt = getCountOfLesson($record->classid);
	                                			    if(!empty($lessonCnt) && $lessonCnt!==0){?>
	                                		      <span class="detail-code"><?php print t("LBL854"); ?>:</span>
	                                		      <span class="detail-desc"><?php print $lessonCnt;} ?></span>
	                                		  </div>
	                                		</div>
	                                		<?php } //echo drupal_json_encode($record->clsdeliverytype);?>
	                                		   <?php  
     	                                	 /*-- #41376 - Print session information only for ILT and Virtual class--*/
     	                                	 if(isset($record->sessionDetailInfo) && ($record->clsdeliverytype == 'lrn_cls_dty_ilt' || $record->clsdeliverytype == 'lrn_cls_dty_vcl')) {  ?>
	                                		  <div class="para">
	                                  			<div class="code-container session">
	                                				<span class="detail-code"><?php print t("LBL249"); ?>:</span>
	        										<span class="detail-desc">
	                                    				<table cellpadding="0" class="class-session-details" cellspacing="0" width="100%" border="0" style="margin-bottom: 5px;">
	                                    					<tr>
	                                    						<th><?php print t("LBL250"); ?></th>
	                                    						<th><?php print t("LBL042"); ?></th>
	                                    						<th><?php print t("LBL251"); ?></th>
	                                    						<th><?php print t("LBL252"); ?></th>
	                                    					</tr>
	                                    					<?php  foreach($record->sessionDetailInfo as $sessionInfo) { ?>
	                                    					<tr>
		                                    					<?php 
		                                    					//Added by Vincent on Oct 28, 2013 for #0028593                
	                                    						if($record->clsdeliverytype == 'lrn_cls_dty_ilt'){
																											$sess_start_time = (strlen($sessionInfo["ilt_start_time"]) == 4) ? str_pad($sessionInfo["ilt_start_time"],5,0,STR_PAD_LEFT) : $sessionInfo["ilt_start_time"];
				                                    					$sess_end_time = (strlen($sessionInfo["ilt_end_time"]) == 4) ? str_pad($sessionInfo["ilt_end_time"],5,0,STR_PAD_LEFT) : $sessionInfo["ilt_end_time"];
				                                    					$ses_st_time_format =  $sessionInfo["ilt_start_time_format"];
				                                    					$ses_end_time_format = $sessionInfo["ilt_end_time_format"];
				                                    					$startdate = $sessionInfo["ilt_start_date"];
				                                    					
	                                    					    }else{
				                                    					$sess_start_time = (strlen($sessionInfo["start_time"]) == 4) ? str_pad($sessionInfo["start_time"],5,0,STR_PAD_LEFT) : $sessionInfo["start_time"];
				                                    					$sess_end_time = (strlen($sessionInfo["end_time"]) == 4) ? str_pad($sessionInfo["end_time"],5,0,STR_PAD_LEFT) : $sessionInfo["end_time"];
				                                    					$ses_st_time_format =  $sessionInfo["start_time_format"];
				                                    					$ses_end_time_format = $sessionInfo["end_time_format"];
				                                    					$startdate = $sessionInfo["start_date"];
	                                    					    }
		                                    					?>
	                                    						<td valign="top" class="class-detail-session-name vtip" title="<?php print $sessionInfo["title"]; ?>" width="50%"><?php print titleController('CLASS-DETAIL-SESSION-TITLE',$sessionInfo["title"],20); ?></td>
	                                    						<td valign="top" width="15%"><?php print $startdate; ?></td>
	                                    						<td valign="top" width="15%"><?php print $sess_start_time; ?><span class="time-zone-format"><?php print $ses_st_time_format; ?></span></td>
	                                    						<td valign="top" width="15%"><?php print $sess_end_time; ?><span class="time-zone-format"><?php print $ses_end_time_format; ?></span></td>
	                                    					</tr>
	                                    					<?php } ?>
	                                    				 </table>
	                                    			 </span>
	                                    			</div>
	                                    		</div>
	                                    		<div class="para">
	                                  				<div class="code-container location">	
	                                    				 <?php  
	                                    				 $sessinc = 1;
	                                    				 foreach($record->sessionDetailInfo as $sessionInfo) {
	                                    				 	if($sessinc==1){ 
																$sessinc++;
	                                    				 		if(!empty($record->locationname))
	                                    							$sessAddDet = $record->locationname; 
	                                    					    //if(!empty($sessionInfo["session_name"])){
				                                                  // $sessAddDet .= ",<br>".$sessionInfo["session_name"];
				                                                //} 
				                                                if(!empty($sessionInfo["session_address1"])){
				                                                   $sessAddDet .= ",<br>".$sessionInfo["session_address1"];
				                                                }if(!empty($sessionInfo["session_address2"])){
				                                                   $sessAddDet .= ",&nbsp;".$sessionInfo["session_address2"];
				                                                }if(!empty($sessionInfo["session_city"])){
				                                                  $sessAddDet .= ",<br>".$sessionInfo["session_city"];
				                                                }if(!empty($sessionInfo["session_state"])){
				                                                  $sessAddDet .= ",&nbsp;".$sessionInfo["session_state"];
				                                                }if(!empty($sessionInfo["session_country"])){
				                                                  $sessAddDet .= ",<br>".$sessionInfo["session_country"];
				                                                }if(!empty($sessionInfo["session_zipcode"])){
				                                                  $sessAddDet .= "&nbsp;-&nbsp;".$sessionInfo["session_zipcode"];
				                                                }
	                                    				 	
				                                                print "<span class='detail-code'>".t("Location").":</span><span class='detail-desc'>".$sessAddDet;
	                                    				 	}
				                                                } 
				                                                ?>	
	                                    					<?php
	                                    						/*if(!empty($record->locationname))
	                                    							$locDet = $record->locationname.",<br>"; 
	                                    					    if(!empty($record->locationaddr1)){
				                                                   $locDet .= $record->locationaddr1.",<br>";
				                                                }if(!empty($record->locationaddr2)){
				                                                   $locDet .= $record->locationaddr2.",<br>";
				                                                }if(!empty($record->locationcity)){
				                                                   $locDet .= $record->locationcity.",<br>";
				                                                }if(!empty($record->locationstate)){
				                                                  $locDet .= $record->locationstate.", ";
				                                                }if(!empty($record->locationcountryname)){
				                                                  $locDet .= $record->locationcountryname."&nbsp";
				                                                }if(!empty($record->locationzip)){
				                                                  $locDet .= $record->locationzip;
				                                                }
				                                                print "<b>Location: </b><br>".$locDet; */
				                                                ?>
				                                                 
	                                    				 </span>
	                                    				 </div>
	                                    			  </div>
	                                    			  <?php }  ?>

	                                  </div>
	                          		 <!-- //left column end -->
	                          		<div class="class-detail-item-row-right">
                    				<?php 
                    					$record->cls_id = $classId;                    				                   				 
                                    	$record->widgetId = 'class_details_display_area';                                    
                                    	print commonRegisterHtml($record);                       			
                    				?>	
	                          		</div> 
	                      		    <?php   if ($numAttachments > 0) { ?>
	                      				<div class="para">
	                        				<div class="code-container attachments">
	                      						<span class="detail-code">
	                      							<span class="lpn-head"><?php print t("LBL231"); ?>:</span>
	                      							<span class="lpn-click-info" id="class_details_page_attachments"><?php print t("LBL232"); ?></span>
	                      						</span>
	
		                                  		<span id='class_details_page_attachment_more' class='detail-desc'>
		  											<ul class='class-details-attachment-list'>
		                                              <?php for ($i = 0; $i < $numAttachments; $i++) { ?>
		                                            		<li class='attachment-link'><?php print ($i == 0)?'':'<span class="divider-pipeline">|</span>';?> <a class="attach vtip" title="<?php echo $attachedRecord[$i]->title ?>" onclick="$('#class_detail_content').data('lnrclassdetails').openAttachment('<?php echo addslashes($attachedRecord[$i]->content);?>');"><?php echo titleController(' ', $attachedRecord[$i]->title,100); ?></a></li>
		                                              <?php } ?>
		                                           	</ul>
		                            			</span>
											</div>
	                                     </div>
	                                  <?php }
	                                  if(!empty($_SESSION['availableFunctionalities']->exp_sp_forum)){	                                  
	                                  ?>
	                                  <?php if (arg(0) != 'widget' && arg(1) != '') {  // hide forum for widget detail page?>
		                                    <!-- html for rendering forum topics -->
		                                    <input id='tdataClsParentId' type="hidden" value="<?php print $block->results['discussions'][0]->tid ; ?>" />
			    			 				<div class="para" style="padding-top:10px;">
					    						<div class="code-container discussions">
					    							<span class="detail-code"><?php print t("DISCUSSIONS"); ?>:</span>	
					    							<div class="clearBoth"></div>    													
													<span class="class-list-container">	
						    			 				<div id="no-records" style="display: none"></div>
						                 				<div id = "forum-topic-list-display" class = "forum-topic-list-class" >
						                    				<table id = "forumTopicListContentResults"></table>
						                    			</div>
						                    		</span>	
						                   		</div>
						    				</div>	
						                    <div id="pager" style="display:none;"></div> <!-- Datatable Pagination -->
						                    <?php }?>			
					                  <?php }?>		             
							                          	
	                    	</div>
	                    </div>
	                </div>
	            </div>
	          </div>
	       </div>
		<div class='block-footer-left'>
			<div class='block-footer-right'>
				<div class='block-footer-middle'>&nbsp;</div>
			</div>
		</div>
	</div> 
<?php }else{
print catalogNoAccessHtml($block_html_id,$classes);

}?>
