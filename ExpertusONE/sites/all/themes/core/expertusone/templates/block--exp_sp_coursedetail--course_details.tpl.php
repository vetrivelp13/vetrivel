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
// echo '<pre>';
// print_r($block);
// echo '</pre>';
if($block->results['catalog']->title){
	$attachedRecord = $block->results['attachments'];
	$numAttachments = count($attachedRecord);
  $mroImageCrsArr['cre_sys_inv_com'] =  '<div class="catalog-course-compliance-role-bg"><span class="catalog-course-compliance-bg-left"></span><span class="catalog-course-compliance-bg-middle">'.t('Compliance').'</span><span class="catalog-course-compliance-bg-right"></span></div>';	
  $mroImageCrsArr['cre_sys_inv_man'] =  '<div class="catalog-course-role-access-bg"><span class="catalog-course-mandatory-bg-left"></span><span class="catalog-course-mandatory-bg-middle">'.t('Mandatory').'</span><span class="catalog-course-mandatory-bg-right"></span></div>';
  $mroImageCrsArr['cre_sys_inv_opt'] =  '<div class="catalog-course-role-access-bg"><span class="catalog-course-optional-bg-left"></span><span class="catalog-course-optional-bg-middle">'.strtolower(t('Optional')).'</span><span class="catalog-course-optional-bg-right"></span></div>';
  $mroImageCrsArr['cre_sys_inv_rec'] =  '<div class="catalog-course-role-access-bg"><span class="catalog-course-recommended-bg-left"></span><span class="catalog-course-recommended-bg-middle">'.t('Recommended').'</span><span class="catalog-course-recommended-bg-right"></span></div>';
  $mroImageCrs = empty($block->results['catalog']->mro_id) ? '' : $mroImageCrsArr[$block->results['catalog']->mro_id];
  $mroImageCrs .= '<div class="clearBoth"></div>';
  if($block->results['catalog']->is_compliance==1){
          $mroImageCrs = $mroImageCrsArr['cre_sys_inv_com'];
  }  
	
  if(isset($_SESSION['widgetCallback']) && $_SESSION['widgetCallback']==TRUE){
  	$showTitle = widgetTitleController($title, 'view_detail_page',$block->results['catalog']->is_compliance);
  }
  else{
  if (empty($block->results['catalog']->mro_id)) {
  if($block->results['catalog']->is_compliance) {
  	  $showTitle = titleController('BLOCK-EXP-SP-COURSEDETAIL-COMPLIANCE-TITLE', $title,100);
  	}
  	else {
      $showTitle = titleController('BLOCK-EXP-SP-COURSEDETAIL-TITLE', $title,100);
  	}
  }
  else {
  $recomm_course = 0;
  	$mand_course = 0;
  	$mrovalue= explode(',',$block->results['catalog']->mro_id);
  	if (in_array('cre_sys_inv_rec', $mrovalue)){
  		$recomm_course = 1;
  	}
  	if (in_array('cre_sys_inv_man', $mrovalue)){
  		$mand_course = 1;
  	}
  	if($recomm_course == 1) {
  		$showTitle = titleController('BLOCK-EXP-SP-COURSEDETAILS-RECOMMENDED-TITLE', $title,100);
  	}
  	else if($mand_course == 1) {
  		$showTitle = titleController('BLOCK-EXP-SP-COURSEDETAILS-MANDATORY-TITLE', $title,100);
  	}
  	else {
  	$showTitle = titleController('BLOCK-EXP-SP-COURSEDETAIL-RECORD-TITLE', $title,70);
  	}
  }
  }
  $hideDesc= 'show';
  if( !empty($_SESSION['widget']['catalog_display_parameters']) &&  $_SESSION['widget']['catalog_display_parameters']['show_description']== false){
  	$hideDesc = 'hide';
  } expDebug::dPrint('widget Course desc: '.print_r($_SESSION['widget'], true).'$hideDesc : '.$hideDesc, 4);
	$tags  = getCatalogTags($block->results['catalog']->id,'Course');
	$tagString  = '';
	if(!empty($tags)){
	  $tagString  = implode(", ",$tags);
	}
	// Change the back link path
	$linkPath = checkWidgetUrl();
	//$linkPath = "javascript:location.href='/?q=" . $primaryPath . "/catalog-search'";
	?>
	<div id="<?php print $block_html_id; ?>" class="<?php print $classes; ?>">
	 	<div class='block-title-left'>
	 		<div class='block-title-right'>
	 			<div class='block-title-middle'>
	 				<?php if($title): ?>
	 					<div class="vtip" title = "<?php print sanitize_data($title);?>" style="float:left;"><h2 class='block-title course-delivery-title'><?php print $showTitle;?></h2></div>
	 					<div class="back-btn-container">
							<input type="button" class="detail-back-button" value="[ <?php print t("LBL212"); ?> ]" <?php if(arg(0) != '') {?> onclick="javascript:location.href='<?php print $linkPath; ?>';" <?php } else {?> onclick="javascript:location.href='/';" <?php } ?> />
						</div>
	 					<?php print $mroImageCrs;?>
	 				<?php endif; ?>
	 			</div>
	 		</div>
	 	</div>
		  <div class="content">
		   <div class="region-sidebar-widget-bg">
	    	<div class="course-details-content" id="course-details-display-content">
	    		<div id="course-details-data">
	    			<div class="detail-item-row">
	    			<div class="course-detail-info">
						<div class="para" style="display: none;">
							<div class="course-title-container"><?php print $title; ?></div>
						</div>
	    				<div class="para">
	    					<div class="code-container ccode">
	    						<span class="detail-code"><?php print t("LBL228"); ?>:</span>&nbsp;
	    						<span class="detail-desc"><?php print $block->results['catalog']->code ; ?></span>
	    					</div>
	    					
	    				</div>
	    				<?php if($hideDesc != 'hide'){ // Hide the Description?>
	    				<div class="para">
	    					<div class="code-container cdescription">
	        					<span class="detail-code"><?php print t("LBL229"); ?>:</span>&nbsp;
	        					<span id="course-detail-desc" class="detail-desc"><?php print $block->results['catalog']->description; ?></span>
	        				</div>
	    				</div>
	    				<?php } ?>
	    				<div class="para">
	    					<div class="code-container tags">
	        					<span class="detail-code"><?php print t("LBL191"); ?>:</span>&nbsp;
	        					<span id="course-detail-desc" class="detail-desc"><?php print $tagString; ?></span>
	        				</div>
	    				</div>
	    				
	                <?php if (!empty($block->results['preRequiste'])): ?>
	                    <div class="para">
	                      <div class="code-container prereq">
	                        <span class="detail-code"><?php print t("LBL230"); ?>:</span>
							<span class="detail-desc">
	                        <?php $preCount = count($block->results['preRequiste']);?>
	                        <?php if($preCount > 1):?>
	                          <ul>
	                        <?php foreach ($block->results['preRequiste'] as $row): ?> 
	                              <li>
	                              	<span id="course-detail-desc" class="head vtip" title="<?php print $row->name;?>"><?php print titleController('BLOCK-EXP-SP-COURSEDETAIL-PREREQUISTE-NAMES', $row->name,30); ?></span>
	                              	<span id="course-detail-desc" class="desc vtip" title="<?php print $row->code; ?>"> - &nbsp;(<?php print titleController('BLOCK-EXP-SP-COURSEDETAIL-PREREQUISTE-CODES', $row->code,10); ?>)</span>
	                              </li>
	                        <?php endforeach; ?>
	                          </ul>
	                        <?php else :?>
	                          <?php foreach ($block->results['preRequiste'] as $row): ?> 
	                             <span id="course-detail-desc" class="head vtip" title="<?php print $row->name;?>"><?php print titleController('BLOCK-EXP-SP-COURSEDETAIL-PREREQUISTE-NAME', $row->name,30); ?></span>
	                             <span id="course-detail-desc" class="desc vtip" title="<?php print $row->code; ?>"> - &nbsp;(<?php print titleController('BLOCK-EXP-SP-COURSEDETAIL-PREREQUISTE-CODE', $row->code,10); ?>)</span>
	                          <?php endforeach; ?>
	                        <?php endif;?> 
	                        </span>                       
	                    </div>
	                  </div>
	                 <?php endif; ?>  
	                 <?php if (!empty($block->results['Equivalence'])): ?>
	                    <div class="para">
	                      <div class="code-container equiv">
	                        <span class="detail-code"><?php print t("LBL279"); ?>:</span>
	                   		<span class="detail-desc">
	                        <?php $preCount = count($block->results['Equivalence']);?>
	                        <?php if($preCount > 1):?>
	                          <ul>
	                        <?php foreach ($block->results['Equivalence'] as $row): ?> 
	                              <li><span id="course-detail-desc" class="vtip" title="<?php print $row->name; ?>"><?php print titleController('BLOCK-EXP-SP-COURSEDETAIL-EQUIVALENCE-NAMES', $row->name,30); ?></span>
	                              <span id="course-detail-desc" class="vtip" title="<?php print $row->code; ?>">- &nbsp;(<?php print titleController('BLOCK-EXP-SP-COURSEDETAIL-EQUIVALENCE-CODES', $row->code,10); ?>)</span></li>
	                        <?php endforeach; ?>
	                        </ul>
	                        <?php else :?>
	                          <?php foreach ($block->results['Equivalence'] as $row): ?> 
	                             <span id="course-detail-desc" class="course-detail-desc vtip" title="<?php print $row->name; ?>"><?php print titleController('BLOCK-EXP-SP-COURSEDETAIL-EQUIVALENCE-NAME', $row->name,30); ?></span>
	                             <span id="course-detail-desc" class="vtip" title="<?php print $row->code; ?>">- &nbsp;(<?php print titleController('BLOCK-EXP-SP-COURSEDETAIL-EQUIVALENCE-CODE', $row->code,10); ?>)</span>
	                          <?php endforeach; ?>
	                        <?php endif;?>  
	                        </span>                      
	                    </div>
	                  </div>
	                 <?php endif; ?>  
	                 <!-- attachments --> 
	             		 <?php if (!empty($block->results['attachments'])): ?>
	                    <div class="para">
	                      <div class="code-container attachments">
	                        <span class="detail-code">
	                        	<span class=""><?php print t("LBL231"); ?>:</span>
	                        	<span class="course-attachment" id="course_details_page_attachments"><?php print t("LBL232"); ?></span>
	                        </span>
							<span class="detail-desc">
	                      		<ul class='course-details-attachmentdetails'>
	        						<?php for ($i = 0; $i < $numAttachments; $i++) { ?>
	        							<li class='attachment-link'><?php print ($i == 0)?'':'<span class="divider-pipeline">|</span>';?> <a class="vtip" title="<?php echo $attachedRecord[$i]->title ?>" onclick="openAttachment('<?php echo addslashes($attachedRecord[$i]->content); ?>')"><?php echo titleController(' ', $attachedRecord[$i]->title,100); ?></a></li> 
	        						<?php  } ?>
	      						</ul>
	      					</span>			
		                  </div>
	                  </div>
	                  <?php endif; ?>
	                  <!-- attachments end -->
	                </div>
	                 
	    				<div class="para">
	        				<div>
	            			  <input id='courseId' type=hidden value="<?php print $block->results['catalog']->id ;?>" />
	        				  <input id='courseNodeId' type=hidden value="<?php print arg(2); ?>" />
	            			  <input id='userId' type=hidden value="<?php global $user; print $user->uid; ?>" />
	        				</div>
	    				  <!--  Place holder table and div for jqgrid of classes list and pager -->
	    				  <div id="class-list-loader"></div>
	        			  <table id ="paint-classes-list" class="classes-list-table">    	
	        			     <div id="class-details-displayloader"></div>
	        			  </table>
	            	  	</div>
	            	  	<?php if(!empty($_SESSION['availableFunctionalities']->exp_sp_forum)){	 ?>
	            	  	 <?php if (arg(0) != 'widget' && arg(1) != '') {  // hide forum for widget detail page?>
	            	  		<!-- html for rendering forum topics -->
		            	  	<input id='tdataCrsId' type="hidden" value="<?php print $block->results['discussions'][0]->tid ; ?>" />
		    			 	<div class="para">
		    					<div class="code-container discussions">
		    					<span class="detail-code"><?php print t("DISCUSSIONS"); ?>:</span>	    													
		    					</div>
		    			 	</div>
		    			 	<div id="no-records" style="display: none"></div>					    			 	
		                 	<div id = "forum-topic-list-display" class = "forum-topic-list-class" style="clear:both;" >
		                    	<table id = "forumTopicListContentResults"></table>
		                    </div>
		                    <div id="pager" style="display:none;"></div> <!-- Datatable Pagination -->
       	  				<?php }?>
       	  				<?php }?>
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





