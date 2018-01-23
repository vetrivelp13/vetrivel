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
if($block->results['learning'][0]->code){
	$mroImageClassArr['cre_sys_inv_man'] =  '<div class="catalog-course-role-access-bg"><span class="catalog-course-mandatory-bg-left"></span><span class="catalog-course-mandatory-bg-middle">'.t('Mandatory').'</span><span class="catalog-course-mandatory-bg-right"></span></div>';
	$mroImageClassArr['cre_sys_inv_opt'] =  '<div class="catalog-course-role-access-bg"><span class="catalog-course-optional-bg-left"></span><span class="catalog-course-optional-bg-middle">'.t('Optional').'</span><span class="catalog-course-optional-bg-right"></span></div>';
	$mroImageClassArr['cre_sys_inv_rec'] =  '<div class="catalog-course-role-access-bg"><span class="catalog-course-recommended-bg-left"></span><span class="catalog-course-recommended-bg-middle">'.t('Recommended').'</span><span class="catalog-course-recommended-bg-right"></span></div>';
	$mroImageClass = empty($block->results['learning'][0]->mro_id) ? '' : $mroImageClassArr[$block->results['learning'][0]->mro_id];
	$mroImageClass .= '<div class="clearBoth"></div>';
	
  
  if(isset($_SESSION['widgetCallback']) && $_SESSION['widgetCallback']==TRUE){
  	$showTitle = widgetTitleController($title, 'view_detail_page',$block->results['learning'][0]->mro_id);
  }
  else{
  if (empty($block->results['learning'][0]->mro_id)) {
    $showTitle = titleController('BLOCK-EXP-SP-LEARNING-PLAN-DETAIL-EMPTY-TITLE', $title,80);
  }
  else {
  $recomm_course = 0;
  	$mand_course = 0;
  	$mrovalue= explode(',',$block->results['learning'][0]->mro_id);
  	if (in_array('cre_sys_inv_rec', $mrovalue)){
  		$recomm_course = 1;
  	}
  	if (in_array('cre_sys_inv_man', $mrovalue)){
  		$mand_course = 1;
  	}
    if($mand_course == 1) {
  	  $showTitle = titleController('LEARNINGPLAN-MANDATORY-COURSE-TITLE',$title,72);
  	}
  	else if($recomm_course == 1) {
  	  $showTitle = titleController('LEARNINGPLAN-RECOMMENDED-COURSE-TITLE',$title,72);
  	}
  	else {
      $showTitle = titleController('LEARNING-PLAN-COURSE-TITLE', $title,70);
  	}
  }
  }
  $hideDesc= 'show';
  if( !empty($_SESSION['widget']['catalog_display_parameters']) &&  $_SESSION['widget']['catalog_display_parameters']['show_description']== false){
  	$hideDesc = 'hide';
  } expDebug::dPrint('widget Certification desc: '.print_r($_SESSION['widget'], true).'$hideButton : '.$hideDesc, 4);
  	$tags  = getCatalogTags($block->results['learning'][0]->id,$block->results['learning'][0]->pro_type);
	$tagString ='';
	if(!empty($tags)){
	  $tagString  = implode(", ",$tags);
	}
	// change back link path    
	$linkPath = checkWidgetUrl();
	?>
	<div id="<?php print $block_html_id; ?>" class="<?php print $classes; ?>">
	 	<div class='block-title-left'>
	 		<div class='block-title-right'>
	 			<div class='block-title-middle'>
	 				<?php if($title): ?>
	 				<div class="vtip" title = "<?php print $title;?>" style="float:left;"><h2 class='block-title course-delivery-title'><?php print $showTitle;?></h2></div>
	 			 	<?php print $mroImageClass;?>
	 				<?php endif; ?> 
	 				<div class="back-btn-container">
	 				<?php 
						$fromPage = arg(3);
						if($fromPage=='catalog'){
						print l('[ '.t("LBL212").' ]', $linkPath, 
						array('attributes' => array('class' => 'detail-back-button','title' => t("LBL212"))));
						}else{?>                   					
						<input type="button" title="<?php print t("LBL212"); ?>" class="detail-back-button vtip" value="[ <?php print t("LBL212"); ?> ]" onclick="javascript:location.href='/';" />
					<?php }?>
					</div> 
	 			</div>
	 		</div>
	 	</div>
		<div class="content">
		   <div class="region-sidebar-widget-bg">
	    	 <div class="lnp-details-content" id="learning-plan-details-display-content">
	    		<div id="learning-plan-details-data">
	    			<div class="detail-item-row">
	                <div class="learning-detail-info">
	                  <div class="learning-detail-desc">
						<div class="para" style="display: none;">
							<div class="lnp-title-container"><?php print $title; ?></div>
						</div>
						<div class="para">
	    					<div class="code-container protype">
	    						<span class="detail-code"><?php print t("LBL036"); ?>:</span>&nbsp;
	    						<span class="detail-desc"><?php print t($block->results['learning'][0]->pro_type) ; ?></span>
	    					</div>
	    				</div>
	    				<div class="para">
	    					<div class="code-container code">
	    						<span class="detail-code"><?php print t("LBL096"); ?>:</span>&nbsp;
	    						<span class="detail-desc vtip" title="<?php print $block->results['learning'][0]->code; ?>"><?php print titleController('LEARNINGPLAN-DETAIL-CODE',$block->results['learning'][0]->code,20); ?></span>
	    					</div>
	    				</div>
	    				<?php if($hideDesc != 'hide'){ // Hide the Description?>
	    				<div class="para">
	    					<div class="code-container cdescription">
	        					<span class="detail-code"><?php print t("LBL229"); ?>:</span>&nbsp;
	        					<span id="course-detail-desc" class="detail-desc"><?php print str_replace(array("\n","\r"), array("<br>","&nbsp;"),$block->results['learning'][0]->description); ?></span>
	        				</div>
	    				</div>
	    				<?php } ?>
	    				<div class="para">
	    					<div class="code-container tags">
	    						<span class="detail-code"><?php print t("LBL191"); ?>:</span>&nbsp;
	    						<span class="detail-desc"><?php print $tagString ; ?></span>
	    					</div>
	    				</div>
	    				<div class="para">
	    					<div class="code-container rating">
	    						<span class="detail-code"><?php print t("Rating"); ?>:</span>&nbsp;
	    						<span class="detail-desc" style="display:none;">
			    				
	    						</span>
	    					</div>
	    					
	                      		<div id="ratingBlock" class="rating-block">
	                      			<?php  
	                     				$getRegister = getObjectRegisteredOrNot($block->results['learning'][0]->id);
	                     				if($getRegister->master_enrolled_status == "lrn_tpm_ovr_cmp"){
	                     				$enableRate = "enable";
	                    				 }else{
	                     				$enableRate = "disable";
	                     				}
	                    						$settings = array(                    					
														  'content_type' => $block->results['learning'][0]->object_type,
														  'content_id' => $block->results['learning'][0]->node_id,
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
	
														print "<div class='prgdet-node'>".drupal_render(drupal_get_form('fivestar_rating_widget', '', $settings))."</div>";
														?>
	                 		</div>
	    				</div>
						<?php if (!empty($block->results['learning'][0]->prerequisites)): ?>
						<div class="para">
							<div class="code-container prereq">
								<span class="detail-code"><?php print t("LBL230"); ?>:</span>
								<span class="detail-desc">
								<?php //$preCount = count($block->results['learning'][0]->prerequisites);?>
									<ul>
									<?php foreach ($block->results['learning'][0]->prerequisites as $row): ?>
									<li>
										<span class="head vtip" title="<?php print $row->pe_title; ?>"><?php print titleController('LEARNINGPLAN-PREREQUISITIE-TITLE',$row->pe_title,20); ?></span>
										<span class="desc vtip" title="<?php print $row->pe_code; ?>">-&nbsp;&nbsp;(<?php print titleController('LEARNINGPLAN-PREREQUISITIE-CODE',$row->pe_code,20); ?>)</span> 
										<span class="type">-&nbsp;&nbsp;<?php print $row->pe_object_type;?></span>
									</li>
									<?php endforeach; ?>
									</ul>
								</span>
							</div> 
							<div class="clearBoth"></div>
						</div>
						<?php endif; ?>
	                    <?php if($block->results['learning'][0]->pro_code == 'cre_sys_obt_crt'): ?>
	                      <?php if(!empty($block->results['learning'][0]->expires_in_value)): ?>
						<div class="para">
                          <div class="code-container expires">
								<span class="detail-code"><?php print t("LBL233"); ?>:</span>&nbsp;
								<span id="course-detail-desc" class="detail-desc" >
									<?php print $block->results['learning'][0]->expires_in_value_str . '&nbsp;' . t('MSG568'); ?>
								</span>
							</div>
						</div>
						<?php endif; ?>
						<?php elseif($block->results['learning'][0]->pro_code == 'cre_sys_obt_trn'): ?>
						<div class="para">
							<div class="code-container complete">
								<span class="detail-code"><?php print t("LBL234"); ?>:</span>&nbsp;
								<span id="course-detail-desc" class="detail-desc"><?php print date_format(date_create($block->results['learning'][0]->end_date),'M d, Y'); ?></span>
							</div>
						</div>
						<?php endif; ?>
					</div>
					
	                  <div class="learning-detail-action">
	                      <div id="learning-plan-action"></div>
	                  </div>
	                  <?php 
	  						//Added by BalaG(19/12/2011) -- For Training Plan Attachments Display
	  						$attachedRecord = $block->results['attachments'];
							$numAttachments = count($attachedRecord);
	  					   if ($numAttachments > 0) { ?>
	                      	<div class="para">
	                        	<div class="code-container attachments">
	                      			<span class="detail-code">
	                      				<span class="lpn-head"><?php print t("LBL231"); ?>:</span>&nbsp;
	                      				<span class="lpn-click-info" id="lpn_details_page_attachments"><?php print t("LBL232"); ?></span>
	                      			</span>
	
	                            <span id='lpn_details_page_attachment_more' class='detail-desc'>
	  								<ul class='lpn-details-attachment-list'>
	                                     <?php  for ($i = 0; $i < $numAttachments; $i++) { ?>
	                                          <li class='attachment-link'><?php print ($i == 0)?'':'<span class="divider-pipeline">|</span>';?> <a class="attach vtip" title="<?php echo $attachedRecord[$i]->title ?>" onclick="openAttachment('<?php echo addslashes($attachedRecord[$i]->content) ?>')"><?php echo titleController(' ', $attachedRecord[$i]->title,100); ?></a></li>
	                                     <?php } ?>
	                                </ul>
	                            </span>
	                      		</div>	                            
	                        </div>
	                       <?php }?> 
	              	</div>
    				<div class="lnp-course-detail-info">
        				<div>
					<!--  <input id='courseId' type=hidden value="<?php //print $block->results['catalog']->id ; ?>" />
        				  <input id='courseNodeId' type=hidden value="<?php //print arg(2); ?>" />
            			  <input id='userId' type=hidden value="<?php global $user; //print $user->uid; ?>" />     -->

                          <input id='progrmId' type="hidden" value="<?php print $block->results['learning'][0]->id ; ?>" />
                          <input id='userId' type="hidden" value="<?php global $user; print $user->uid; ?>" />
                          
                          
        				</div>
    				  <!--  Place holder table and div for jqgrid of classes list and pager -->
	    				  <div id="course-list-loader" class="course-list-container">
	              
	                        <table border="0" cellpadding="0" class="learning-course-container course-list-table" width="100%" cellspacing="0">
	                         <?php 
	                         
	                         $modExists = array();
	                         if (count($block->results['learning'][0]->courselist) > 0) { ?>
	                          <?php foreach ($block->results['learning'][0]->courselist as $row): ?> 
	                          <?php if(!in_array($row->module_id, $modExists)){
	                          	array_push($modExists,$row->module_id);
	                          ?>
	                          <tr class="learning-module-row">
	                              <td class="learning-module-cell">
	                                <span id="learning-module-accordion-<?php print $row->module_id; ?>" class="title_close" onclick="$('#learning-plan-details-display-content').data('lnrplandetails').renderCourseList(<?php print $row->module_id; ?>);" class="learning-course-show-hide">&nbsp;</span>
	                               <span class="learning-module-title" href="javascript:void(0);" title="<?php print $row->module_title;?>"><?php print titleController('LEARNINGPLAN-DETAIL-GROUP-NAME',$row->module_title,20); ?></span>
	                              </td>
	                            </tr>
	                          <?php } ?>
	                          <tr class="learning-module-listcourse learning-module-subrow-<?php print $row->module_id; ?>">
	                            <td class="learning-module-course-level">
	                           	  <table  width="100%" cellspacing="0"  border="0" cellpadding="0" >
		                            <tr class="learning-course-row">
		                              <td class="learning-course-cell">
		                              <input id='courseId' type=hidden value="<?php print $row->course_id; ?>" />
	                              <div id="class-list-loaderimg-<?php print $row->course_id; ?>" class="class-list-loaderimg"></div>
	                              <span id="learning-course-accordion-<?php print $row->course_id; ?>" class="title_close" onclick="$('#learning-plan-details-display-content').data('lnrplandetails').renderClassesList(<?php print $row->course_id; ?>);" class="learning-course-show-hide">&nbsp;</span>
	                              <span class="learning-course-title vtip" href="javascript:void(0);" title="<?php print $row->crs_title;?>"><?php print titleController('LEARNINGPLAN-DETAIL-COURSE-TITLE',$row->crs_title,20); ?> 
	                              <!-- Commented For this Ticket #0037080 -->
	                              <?php // print ($row->is_required == 'Y')?' - <span class="mnd-opt-txt">'.t('Mandatory').'</span>':' - <span class="mnd-opt-txt">'.t('Optional').'</span>';?> </span></td>
	                            </tr>
	                            <tr id="learning-course-subrow-<?php print $row->course_id; ?>" class="learning-course-subrow learning-module-crs-subrow-<?php print $row->module_id; ?>">
	                              <td id="course-subrow-id-<?php print $row->course_id; ?>" class="paint-course-list-subrow">
	                                <div class="para">
	                                  <div class="code-container ccode">
	                                    <span class="detail-code"><?php print t("LBL228"); ?>:</span>&nbsp;
	                                    <span class="detail-desc vtip" title="<?php print $row->crs_code;?>"><?php print titleController('LEARNINGPLAN-DETAIL-COURSE-CODE',$row->crs_code,20); ?></span>
	                                   </div>
	                                </div>
	                                <?php if($hideDesc != 'hide'){ // Hide the Description?>
	                                <div class="para">
	                                  <div class="code-container cdesc">
	                                      <span class="detail-code"><?php print t("LBL229"); ?>:</span>&nbsp;
	                                      <span id="course-detail-desc" class="detail-desc"><?php print $row->crs_description; ?></span>
	                                  </div>
	                                </div>
									<?php } ?>
			                        <?php  //Added by BalaG(19/12/2011) -- For Course Attachments Display
	                          /* $attachedRecordCrs = $row->attachments;
							   $numAttachmentsCrs = count($attachedRecordCrs);
	
	                        if ($numAttachmentsCrs > 0 ) {*/ ?>
	                      	<!--<div class="para">
	                        	<div class="lpn-item-container">
	                      			<b><?php //print t("Attachments"); ?>:</b>&nbsp;
	                      			<span class="lpn-attachment" id="lpn_details_page_attachments"><?php // print t("(click to download)"); ?></span>
	                      		</div>
	                            <div id='lpn_details_page_attachment_more' class='lpn-details-attachment-more'>
	  								<ul class='lpn-details-attachment-list'>
	                                     <?php // for ($i=0; $i < $numAttachmentsCrs; $i++) { ?>
	                                          <li class='attachment-link <?php // print (($i+1) == 1)?'first':'';?> <?php //print (($i+1) == $numAttachmentsCrs)?'last':'';?>' onclick="openAttachment('<?php //echo $attachedRecordCrs[$i]->content ?>')"><?php //echo $attachedRecordCrs[$i]->title;?></li>
	                                     <?php //}?>
	                                </ul>
	                            </div>
	                        </div>
	                       --><?php //}?>
	                                
	                                <div class="learning-classes-details-wrapper">
	                                
	                                <div class="clearBoth"></div>
	                                  <div class="learning-course-displayloader" id="class-details-displayloader-<?php print $row->course_id; ?>"></div>
	                                  <table class="learning-classes-details" id="paint-classes-list-<?php print $row->course_id; ?>"></table>
		                                 </div>
		                                 
		                       
		                                </div>
		                              </td>
		                            </tr>
	                            </td></tr></table>
	                          <?php endforeach; ?>
	                          <?php } else { ?>
	                              <tr>
	                                <td class="noItemfound"><?php print t("MSG268"); ?></td>
	                              </tr>                          
	                          <?php } ?>
	                        </table>
	                        <?php if(!empty($_SESSION['availableFunctionalities']->exp_sp_forum)){	 ?>
	                        <?php if (arg(0) != 'widget' && arg(1) != '') {  // hide forum for widget detail page?>
	                        <!-- html for rendering forum topics -->
	                        <input id='tdataTpId' type="hidden" value="<?php print $block->results['discussions'][0]->tid ?>" /> 
	                        <div class="para" style="padding-top:0;">
	    						<div class="code-container discussions">
	    							<span class="detail-code"><?php print t("DISCUSSIONS"); ?></span>	    													
	
	    						<div id="no-records" style="display: none"></div>		
	                        	<div id = "forum-topic-list-display" class = "forum-topic-list-class" >
	                      			<table id = "forumTopicListContentResults"></table></div>
	                      		</div>
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



