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
$class_details = $result;
?>

<div class="course-class-content-wrapper <?php //print $wrapperClass; ?> <?php //print $result->delivery_type_code; ?>">
	<div id="class-container-loader-<?php print $class_details->class_id ; ?>" class="class-list padtp5">
				<?php //foreach ( $result->class_details_arr as $class_details ) {  ?>
						<div class="class_list_container">
						<?php print theme ("content_header", array (
								"result" => $class_details, 
								"context" => $context,
						)); ?>
						<div class="class_list_content">
							<div class="class-content-content-wrapper">
						
									<div  id="class_content_moredetail_<?php print $class_details->class_id ; ?>" class="cls-more-less-container"  style="display:none;">
										<?php 
										// Class content informations disaplyed here
										print theme ("content_child_list", array (
												"result" => $class_details,
												"context" =>$context,
										));
										// Class misc informations disaplyed here
										print theme ("content_misc", array (
												"result" => $class_details,
												"context" => $context,
										));
										?>
										<div class="class-content-less padbt5" id = "class_content_less_<?php print $class_details->class_id ; ?>"><div class="show_more_less" onclick="showClassDetailLess(<?php print $class_details->class_id; ?>)"><?php echo t('LBL3042');?></div></div>
								</div>
							</div>
						</div>
						
					</div>
					
				<?php // } ?>

	</div> 
</div>	



