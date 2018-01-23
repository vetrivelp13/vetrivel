<?php 
expDebug::dPrint('result class child list received: ' . print_r($result, 1));
expDebug::dPrint('context class child list received: ' . print_r($context, 1));

$callFrom = $context['callfrom'];
// switch ($callFrom) {
// 	case 'class_details':
// 		$wrapperClass = 'border-box';
// 		break;
// 	case 'course_details_content':
// 		$wrapperClass = 'border-box-cnt-crs';
// 		break;
// 	case 'learning_plan_details':
// 		$wrapperClass = 'border-box-cnt-tp';
// 		break;
// 	default:
// 		$wrapperClass = 'border-box';
// 		break;
// }
?>

<div class="learningplan_group_container">
	<div id="course-list-<?php print $result->course_id; ?>" class="course-list padtp5">
		<div id="class-list-loaderimg-<?php print $result->course_id; ?>" class="class-list-loaderimg"></div>
		<?php if (!empty($result->group_title) && $result->show_title): ?>
			<div id="<?php print $result->group_id; ?>" class="lp_group_title <?php print $result->group_id; ?>">
				<fieldset ><legend align="left" title="<?php print $result->group_title; ?>" class="vtip"><?php print titleController('LEARNINGPLAN-DETAIL-GROUP-NAME', $result->group_title, 30); ?></legend></fieldset>
			</div>
		<?php endif; ?>
		<div class="lp_module_list lp_course_list-<?php print $result->group_id; ?>">
			<?php print theme ("content_header", array ( "result" => $result, 'context' => ['callfrom' => $callFrom])); ?>
			<div class="course_list_content">
				<div class="course-content-content-wrapper">
					<div class="course-content-more padbt5" id="course_content_more_<?php print $result->course_id; ?>"><div class="show_more_less" onclick="$('#learning-plan-details-display-content').data('lnrplandetails').renderClassesList(<?php print $result->course_id; ?>,<?php print $result->program_id; ?>)"><?php echo t('LBL713');?></div></div>
						<div  id="course_content_moredetail_<?php print $result->course_id; ?>" class="crs-more-less-container" >
							<div id="learning-course-subrow-<?php print $result->course_id; ?>" class="course_class_lists learning-module-crs-subrow-<?php print $result->module_id; ?>">
								<div class="learning-classes-details-wrapper">
                                   <div id="searchLrnplanClassListPaint-<?php print $result->course_id; ?>" class="search-lrnplan-class-list-paint"><table class="learning-classes-details" id="paint-classes-list-<?php print $result->course_id; ?>"></table></div>
                                   <div class="grid-show-more show-more-wrapper" style="text-align: center; border: none;">
										<span id="paintlrpclslist-show_more-<?php print $result->course_id; ?>" class="grid-show-more-wrapper show-more-handler"><?php print t("SHOW MORE"); ?></span>
						  		   </div>
                                </div>
                                <div class="course-content-less padbt5" id = "course_content_less_<?php print $result->course_id; ?>" style="display:none"><div class="show_more_less"  onclick="$('#learning-plan-details-display-content').data('lnrplandetails').hideClassesList(<?php print $result->course_id ?>)"><?php echo t('LBL3042');?></div></div>
                              		</div>
							</div>
						</div>
					</div>
		</div>
	</div>	
</div>



