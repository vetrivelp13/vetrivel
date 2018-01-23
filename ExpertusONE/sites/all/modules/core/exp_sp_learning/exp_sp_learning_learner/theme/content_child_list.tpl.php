<?php 
expDebug::dPrint('result received: ' . print_r($result, 1));
$launch_result = $result->launch_variables;
expDebug::dPrint('launch result received: ' . print_r($launch_result, 1));

$callFrom = $context['callfrom'];
switch ($callFrom) {
	case 'class_details':
		$wrapperClass = 'border-box';
		break;
	case 'course_class_list':
		$wrapperClass = 'border-box-cnt-crs';
		break;
	case 'lrnplan_course_class_list':
		$wrapperClass = 'border-box-cnt-tp';
		break;
	default:
		$wrapperClass = 'border-box';
		break;
}
?>
<?php //if($launch_result->isLaunch===true || !empty($result->sessionDetailInfo)): ?>
<div class="class-content-wrapper <?php print $wrapperClass; ?> <?php print $result->delivery_type_code; ?>">
	<div class="content-list padtp5">
	<?php  if($launch_result->showmenu===true): ?>
		<div class="content-details-warpper padbt5  <?php if($launch_result->isLaunch===true){ echo "clsdisplayblock";?>  <?php }else{echo "clsdisplaynone";?> <?php }?>">
		<div class="sub-section-title padbt10"><?php print t('Content'); ?></div>
		<div class="paindContentResults clsSeeMorePlaceholderdiv padbt5 paindContentclsid_<?php echo $launch_result->class_id; ?>" id="paindContentResults_<?php echo $launch_result->enrolled_id; ?>" >
		</div>
		</div>
	<?php  endif; ?>

	<?php if(!empty($result->sessionDetailInfo)): ?>
		<?php if($result->delivery_type_code == 'lrn_cls_dty_ilt'):
			$session = $result->sessionDetailInfo[0];
			$locationDetails = formatLocationAddress($session); 
		?>
		<div class="session-location-warpper">
			<div class="sub-section-title padbt5"><?php print t('Location'); ?></div>
			<div class="session-location-block ">
				<div class="location-address padbt10">
					<?php print $locationDetails; ?>
				</div>
			</div>
		</div>
		<?php endif; ?>
		<div class="session-details-warpper">
		<div class="sub-section-title padbt5"><?php print t('LBL277'); ?></div>
			<?php 
			foreach($result->sessionDetailInfo as $session): 
			$str = '';
			$sesInsArr = explode(",",$session['session_instructor_name']);
			$total = count($sesInsArr);
			$i=0;
			for($k=0;$k<$total;$k++) {
				$i++;
				$instName = $sesInsArr[$k];
				$str .= titleController('CLASS-DETAIL-SESSION-INSTRUCTOR',$instName,200);
				if($i != $total) {
					$str .= "<span class='ses-seperator'> ,</span>";
				}
			}
			?>
			
					<div class="session-detail-block padbt5">
						<div class="session-name session-row">
							<span class="sess-attr-name"><?php print t('LBL107'); ?>: </span>
							<span class="sess-attr-val vtip" title="<?php print sanitize_data($session['title']); ?>"><?php  print titleController('CLASS-DETAIL-SESSION-TITLE',$session['title'],20); ?></span>
						</div>
						<div class="session-date session-row">
							<span class="sess-attr-name"><?php print t('LBL042')?>: </span>
							<span class="sess-attr-val" title=""><?php print $session['session_display_time']; ?> <?php print $session['tz_location_info']; ?> </span>
						</div>
						<div class="session-instructor session-row">
							<span class="sess-attr-name"><?php print t('Instructor'); ?>: </span>
							<span class="sess-attr-val vtip" title="<?php print sanitize_data($session['session_instructor_name']); ?>"><?php print $str; ?></span>
						</div>
					</div>		
			<?php endforeach; ?>
		</div>
	<?php endif; ?>
	</div>
	<?php if($callFrom=='class_details'):?>
		<div class="content-prereq-container <?php if($launch_result->isLaunch===true){?> padtp5 <?php }elseif(!empty($result->sessionDetailInfo)){?> padtp5 <?php  }?>">
			<div class="content-prereq-inner-container">
				<div class="content-prereq-title-container">
					<div class="sub-section-title padbt5"><?php print t("Course"); ?></div>
				</div>
				<div class="course-block-container">	
					<div class="course-block">
						<div class="course-row">	
							<div class="course-title vtip " title="<?php print sanitize_data($result->course_details->title); ?>"> <?php print titleController('BLOCK-EXP-SP-COURSEDETAIL-PREREQUISTE-NAMES', $result->course_details->title,30); ?>  </div>
						</div>
						<div class="course-code-row">
							<div class="course-code label"><?php print t("LBL096"); ?>&nbsp;:&nbsp;<!-- Code: --></div>
							<div class="course-code val vtip " title="<?php print sanitize_data($result->course_details->code); ?>"> <?php print titleController('BLOCK-EXP-SP-COURSEDETAIL-EQUIVALENCE-CODE', $result->course_details->code,15); ?>  </div>
						</div>
						<?php if(!empty($result->complete_by)){ ?>
							<div class="course-code-row">
								<div class="course-code label"><?php print t("LBL234"); ?>&nbsp;:&nbsp;<!-- Code: --></div>
								<div class="course-code val vtip " title="<?php print sanitize_data($result->complete_by); ?>"> <?php print titleController('BLOCK-EXP-SP-COURSEDETAIL-EQUIVALENCE-CODE', $result->complete_by,15); ?>  </div>
							</div>
						<?php } 
						 if(!empty($result->validity)){ ?>
							<div class="course-code-row">
								<div class="course-code label"><?php print t("LBL604"); ?>&nbsp;:&nbsp;<!-- Code: --></div>
								<div class="course-code val vtip " title="<?php print sanitize_data($result->validity); ?>"> <?php print titleController('BLOCK-EXP-SP-COURSEDETAIL-EQUIVALENCE-CODE', $result->validity,15); ?>  </div>
							</div>
						<?php } ?>
					</div>
					
				</div>
			</div>
		</div> 
	<?php endif; ?>
</div>	
<?php //endif; ?>
<?php if($launch_result->isLaunch===true){ drupal_add_js("jQuery(document).ready(function () { $('#lnr-catalog-search').data('contentPlayer').createLoader('paindContentResults_".$launch_result->enrolled_id."'); $('#lnr-catalog-search').data('contentPlayer').playContentMylearning({'masterEnrollId':'".$launch_result->master_enrollment_id."','enrollId':'".$launch_result->enrolled_id."','programId':'0','courseId':'".$launch_result->course_id."','classId':'".$launch_result->class_id."','userId':'".$launch_result->user_id."','LaunchFrom':'CL','defaultContent':'0','classTitle':'check the multiple content','pagefrom':'class_details',});});", 'inline');}?>

