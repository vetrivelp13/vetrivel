<?php 
$callFrom = $context['callfrom'];
expDebug::dPrint('$callFrom value : misc ' .print_r($result,1));
switch ($callFrom) {
	case 'class_details':
	case 'course_details':
		$wrapperClass = 'border-box';
		$showPreRequsite = true;
		$showEquivalence = true;
		break;
	case 'lrnplan_details':
		$wrapperClass = 'border-box';
		$showPreRequsite = true;
		$showEquivalence = false;
		break;
	case 'course_class_list':
		$wrapperClass = 'border-box-cnt-crs';
		$showEquivalence = false;
		$showPreRequsite = false;
		break;
	case 'lrnplan_course_class_list':
		$wrapperClass = 'border-box-cnt-tp';
		$showPreRequsite = false;
		$showEquivalence = false;
		break;
	default:
		$wrapperClass = 'border-box';
		break;
}
?>
<div
	class="class-misc-wrapper <?php print $wrapperClass; ?> <?php print $record->delivery_type_code; ?>">
	<?php if(($callFrom == 'course_class_list' || $callFrom == 'lrnplan_course_class_list') && !empty($result->duration)){?>
	<div class="content-duration-title-container">
		<div class="sub-section-title padbt5"><?php print t("LBL248"); ?></div>
		<div class="duration val padbt5"> <?php 
		$pluralMinutes = false;
		if($result->duration != 1) {
			$pluralMinutes = true;
		}
		print $result->duration .' '.($pluralMinutes ? t('LBL424') : t('LBL423'));?>  </div>
	</div>
	<?php }?>
	<!--  Other Course/Class related attributes starts -->
	<?php if(!empty($result->registration_end_on) || !empty($result->complete_by) || !empty($result->validity) ){?>
	<div class="ontent-prereq-container padbt5">
		<!-- <div class="content-prereq-inner-container"> -->
		<div class="content-prereq-title-container">
			<div class="sub-section-title padbt5"><?php print t("Other").' '.t("LBL272"); ?></div>
		</div>
		<div class="course-block-container">
			<div class="course-block">
	<?php if(!empty($result->registration_end_on)){?>
		<div class="course-code-row">
			<div class="course-code label"><?php print t("LBL565"); ?>&nbsp;:&nbsp;<!-- Registration End: --></div>
					<div class="course-code val"> <?php print $result->registration_end_on; ?>  </div>
				</div>
	<?php }?>
	<?php if(!empty($result->complete_by) && empty($result->validity)){?>
		<div class="course-code-row">
			<div class="course-code label"><?php print t("LBL234"); ?>&nbsp;:&nbsp;<!-- complete by: --></div>
					<div class="course-code val"> <?php print $result->complete_by; ?>  </div>
				</div>
	<?php }?>
	<?php if(!empty($result->validity)){?>
		<div class="course-code-row">
			<div class="course-code label"><?php print t("LBL604"); ?>&nbsp;:&nbsp;<!-- Validity: --></div>
					<div class="course-code val"> <?php print $result->validity; ?>  </div>
				</div>
	<?php }?>
	</div>
		</div>
	</div>
	<?php }?>
	<!--  Other Course/Class related attributes end -->

	<div class="pre-requisite-wrapper">
	
	<?php if($showPreRequsite){ // show a pre-requisite block only to class details  ?>	
		<div class="content-prereq-container padbt5">
			<div class="content-prereq-inner-container">
				<div class="content-prereq-title-container">
					<div class="sub-section-title padbt5"><?php print t("LBL230"); ?></div>
				</div>
				<?php if ($result->has_preRequiste=='yes'): ?>	
				<div class="prerequisite-block-container">
					<div class="prerequisite-block">
			
							<?php foreach ($result->preRequiste as $row):?> 
								<div class="prerequisite-row">
									<div class="prerequisite-title vtip " title="<?php print sanitize_data($row->name); ?>"> <?php print titleController('BLOCK-EXP-SP-COURSEDETAIL-PREREQUISTE-NAMES', $row->name,30); ?>  </div>
						</div>
						<div class="prerequisite-code-row ">
									<div class="prerequisite-code label"><?php print t("LBL096"); ?>&nbsp;:&nbsp;<!-- Code: --></div>
									<div class="prerequisite-code val vtip " title="<?php print sanitize_data($row->code); ?>"> <?php print titleController('BLOCK-EXP-SP-COURSEDETAIL-EQUIVALENCE-CODE', $row->code,15); ?>  </div>
						</div>
							<?php endforeach; ?>
						
					</div>

				</div>
			</div>
			<?php else: ?>
			</div>
		<div class="prerequisite-row">
			<div class="prerequisite-msg no-records"> <?php if($callFrom=='lrnplan_details') { print t('MSG857'); } else {  print t('MSG342'); } ?>  </div>
		</div>
			<?php endif; ?>  
		 	<?php /*if ($result->has_preRequiste=='yes'): ?>
			<div class="content-prerequisite-action" onclick="$('body').data('prerequisite').getTpPrerequisites(<?php print $result->prereq_id; ?>,'','course','l1');">
				<div class="content-search-register-btn prerequisite-register-btn action-btn"><?php print $result->preRequiste_label;?></div>
			</div>	
			<?php endif; */ ?>
	
		</div>
	<?php } ?>
		
	<?php if($showEquivalence){  // show a equivelance block only to class details ?>	
		<div class="content-equiv-container padbt5">
		<div class="sub-section-title padbt5"><?php print t("LBL279"); ?></div>
		<div class="equivalence-block">
			<?php if ($result->has_Equivalence=='yes'): ?>	
				<?php foreach ($result->Equivalence as $row): ?> 
					<div class="equivalence-row">
						<div class="equivalence-title vtip" title="<?php print sanitize_data($row->name); ?>"> <?php print titleController('BLOCK-EXP-SP-COURSEDETAIL-EQUIVALENCE-NAMES', $row->name,30); ?>  </div>
				<div class="prepreuisute-action"></div>
			</div>
			<div class="equivalence-code-row ">
						<div class="equivalence-code label"><?php print t("LBL096"); ?>&nbsp;:&nbsp;<!-- Code: --></div>
						<div class="equivalence-code val vtip " title="<?php print sanitize_data($row->code); ?>"> <?php print titleController('BLOCK-EXP-SP-COURSEDETAIL-EQUIVALENCE-CODE', $row->code,15); ?>  </div>
			</div>
				<?php endforeach; ?>
			<?php else: ?>
						<div class="equivalence-row">
				<div class="equivalence-msg no-records"> <?php print t('MSG343'); ?>  </div>
			</div>
			<?php endif; ?>  
			</div>
	</div>
	<?php } ?>	
	
	<?php if($result->has_instructor == 'yes'){?>
		<div class="content-instructor-container padbt10">
		<div class="sub-section-title padbt5"><?php print t("ACLBL0008"); ?></div>
		<div class="content-instructor-block">
				<?php $insc = 0; 
					  $cntins = count($result->instructors); 
					  $inslim = 3; 
					  if($callFrom=='course_class_list' || $callFrom=='lrnplan_course_class_list') 
					  	  $inslim = 2;
					  $last_flag = false;
					  if($cntins%$inslim==0)
					  	$last_flag = true;
					  foreach ($result->instructors as $row): $insc++; ?>
					<div class="content-instructor-row padtp5px">
				<div class="avatar-image user-list-border-img">
							<img class="user-picture <?php if($row['ins_pic_default'] == 1) print 'default-pic';?>" src="<?php print $row['ins_uri']; ?>" height="<?php print $row['ins_pic_height'];?>px" width="<?php print $row['ins_pic_width'];?>px" />
				</div>
				<div class="content-instructor-detail vtip">
							<?php if (!empty($row['ins_name'])): ?><div class="instrcutor-name vtip" title="<?php print sanitize_data($row['ins_name']); ?>"><?php  print titleController('CLASS-DETAIL-INS-DETAILS-NAME',$row['ins_name'],20); ?></div><?php endif; ?>
							<?php if (!empty($row['ins_job_title'])): ?><div class="instrcutor-dest vtip" title="<?php print sanitize_data($row['ins_job_title']); ?>"><?php  print titleController('CLASS-DETAIL-INS-DETAILS-JOB',$row['ins_job_title'],20); ?></div><?php  endif; ?>
						</div>
			</div>
						<?php  if(($insc % $inslim) === 0) {?>
						<?php if($insc == $cntins && $last_flag == true){ ?> 
						<?php }else{?>
						<div class="space-height-separater5px">&nbsp;&nbsp;</div>	
						<?php }}?>
	    	    <?php endforeach; ?>
			</div>
	</div>
	
	<?php }?> 
	
		<div class="content-attachment-container padbt10">
		<div class="sub-section-title padbt5"><?php print t("LBL231"); ?>&nbsp;</div>
			<div id='class_details_page_attachment_more' class='content-details-attachment-more '>
				<?php if ($result->numAttachments > 0): ?>
				<div class='class-details-attachment-list'>
		            <?php for ($i = 0; $i < $result->numAttachments; $i++): ?>
							<span class='attachment-link'>
								<?php print ($i == 0) ? '': '<span class="divider-pipeline">|</span>'; ?> 
		                    	<a class="attach vtip" title="<?php print sanitize_data( $result->attachments[$i]->title) ?>" onclick="openAttachmentCommon('<?php print $result->attachments[$i]->content;?>');"><?php print titleController(' ', sanitize_data( $result->attachments[$i]->title),100);?></a>
				</span>
					<?php endfor; ?>
				</div>				
				<?php else: ?>
				<div class='class-details-attachment-list no-records'>
				<span class='attachment-msg'><?php print t('MSG370');?></span>
			</div>
				<?php endif;?>
	            
			</div>
	</div>


	<div class="content-tags-container">
		<div class="sub-section-title padbt5"><?php print t("LBL191"); ?></div>
			<?php if( $result->tagString!=''):?> 
				<div class="tags-val"><?php print $result->tagString;?></div>
			<?php else:?>
			<div class="tags-msg no-records"><?php print t('MSG349');?></div>
			<?php endif;?>
		</div>
		
	<?php if($result->additional_info!=''){?>
		<div class="limit-desc-row <?php print $result->delivery_type_code; ?> content-additional-info-container padtp10">
		<div class="sub-section-title padbt5"><?php print t("LBL3068"); ?></div>
			<div class="limit-desc lmt-cls-desc-add additional-info-desc vtip"><?php print $result->additional_info;?></div>
	</div>
	<?php }?>
		
	</div>
</div>