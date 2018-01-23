<?php 
$callFrom = $context['callfrom'];
expDebug::dPrint( 'attach catalog register template : here 11111 '.$callFrom , 5);
$content_title = $result->data_title;
$content_code = $result->data_code;

if($callFrom == 'class_details' || $callFrom == 'lrnplan_course_class_list' || $callFrom == 'course_class_list'){
	$content_title =sanitize_data($result->data_title);
}
if($callFrom != 'catalog'){
	$content_code =sanitize_data($result->data_code);
}
switch ($callFrom) {
	case 'course_details':
	case 'class_details':
	case 'lrnplan_details':	
		$showComManIcon = true;
		$descClass = 'padbt10';
		$trunk8_class = $result->data_trunk8;
		break;
	case 'lrnplan_course_list':
	case 'lrnplan_course_class_list':
		$showComManIcon = false;
		$descClass = 'padbt5';
		$result->content_detail_container_class = '';
		$trunk8_class = $result->data_trunk8;
		break;
	case 'course_class_list':	
		$showComManIcon = true;
		$descClass = 'padbt5';
		$result->content_detail_container_class = '';
		$trunk8_class = $result->data_trunk8;
		break;
	case 'catalog':
		$showComManIcon = false;
		$descClass = 'padbt10';
		$trunk8_class = 'lmt-cat';
		break;
	case 'tp_prereq':
		$showComManIcon = false;
		$descClass = 'padbt10';
		$trunk8_class = 'lmt-des-prereq';
		break;
	default:
		$showComManIcon = true;
		$descClass = 'padbt10';
		$trunk8_class = 'lmt-def';
}
?>
<div
	class="content-row-container<?php print (!empty($result->content_row_class) ? ' '.$result->content_row_class : '');?>  <?php print $result->delivery_type_code; ?>">
	<?php if((!isset($_SESSION['widget']['catalog_display_parameters']) || $callFrom != 'catalog') || ($_SESSION['widget']['catalog_display_parameters']['show_icon']==true && $callFrom == 'catalog')){ ?>
	<div
		class="content-icon-container<?php print (!empty($result->content_icon_container_class) ? ' '.$result->content_icon_container_class : '');?>">
		<div class="content-icon <?php print $result->icon_class;?> vtip"
			title="<?php print t($result->icon_title);?>"></div>
    			<?php if (!empty($result->mroImageClass) && $showComManIcon):?>
    			<div class="content-compliance-role-bg">
					<?php print $result->mroImageClass; ?>
				</div>
				<?php endif; ?>
    		</div>
    <?php }?>
	<div
		class="content-detail-container<?php print (!empty($result->content_detail_container_class) ? ' ' . $result->content_detail_container_class : ''); print ($_SESSION['widget']['catalog_display_parameters']['show_button']==0 && $callFrom == 'catalog') ? ' clsEmbedButtonFalse':'';?>">
		<div
			class="content-title vtip limit-title-row <?php print $result->data_title_classes;?>">
			<span
				class="<?php print($callFrom != 'catalog' ? 'limit-title' : '');?> <?php print $trunk8_class . '-title';?> vtip"
				title="<?php  print str_replace('"','&quot;',(isset($result->data_title_tooltip) ? $result->data_title_tooltip : $result->data_title)); ?>"><?php print $content_title; ?></span>
					<?php 
					expDebug::dPrint('$result->mroImageClass '.$result->mroImageClass, 5);
					print (!empty($result->mroImageClass) && $callFrom == 'catalog' ? $result->mroImageClass : '');?>
				</div>
		<div class="<?php print ((!empty($result->content_detail_class) ? $result->content_detail_class : 'content-detail-code padbt5'));?>">
			<?php if($callFrom != 'catalog' && $callFrom != 'tp_prereq') { ?>
		    		<?php if (!empty($result->data_code) && $callFrom != 'course_class_list' && $callFrom != 'lrnplan_course_class_list') {?>
		    		<div class="line-item-container float-left">	
		    		<div class="detail-comp line-item cls-course-code-lbl"><?php print t("LBL096"); ?>&nbsp;:&nbsp;<!-- Code: --></div>
		    		</div>
			    		<div class="line-item-container float-left">				    		
								<div class="vtip line-item cls-course-code-val" title="<?php print str_replace('"','&quot;',$result->data_code); ?>"><?php print titleController($result->cls_class_code,$content_code, 20); ?></div>
							</div>
					<?php } ?>
					<?php if (!empty($result->statistics->language) && ($callFrom == 'course_class_list' || $callFrom == 'lrnplan_course_class_list')) {?>
			    		<div class="line-item-container float-left">
			    		<div class="detail-comp line-item"></div>
							<div class="vtip line-item" title="<?php print $result->statistics->language; ?>"><?php print titleController($result->cls_class_code,$result->statistics->language, 20); ?>
						</div></div>
					<?php } ?>
					<?php if (!empty($result->registration_end_on) && $callFrom == 'class_details') { ?>
			    		<div class="line-item-container float-left">
				    		<div class="divider-pipeline line-item">|</div><div class="detail-comp line-item"><?php print t("LBL565"); ?>&nbsp;:&nbsp;<!-- Registration Ends: --></div>
								<div class="vtip line-item" title="<?php print $result->registration_end_on; ?>"><?php print titleController($result->cls_class_code,$result->registration_end_on, 20); ?></div>
							</div>
					<?php } ?>
					<?php if($result->sessionDetails && ($callFrom != 'class_details' && $callFrom != 'course_class_list' && $callFrom != 'lrnplan_course_class_list')){?>
							<div class="line-item-container float-left">
								<div class="divider-pipeline line-item">|</div>
								<div class="detail-comp line-item"><!-- Starts --></div>
								<div class="vtip line-item"><?php print $result->sessionDetails; ?><!-- 30-01-2017 --></div>
							</div>
			    	<?php }?>
			    	<?php if($result->sessionDetailsHeader && ($callFrom == 'class_details' || $callFrom == 'course_class_list' || $callFrom == 'lrnplan_course_class_list')){?>
							<div class="line-item-container float-left">
								<div class="divider-pipeline line-item">|</div>
								<div class="detail-comp line-item"><!-- Starts --></div>
                       <div class="vtip line-item" title="<?php print strip_tags($result->sessionDetailsHeader); ?>"><?php print titleController('CLASS-DETAIL-DATE-TIME',$result->sessionDetailsHeader, 20); ?>	</div>
			    	  </div>
			    	   <div class="line-item-container float-left">
								<div class="vtip line-item" ><?php print $result->LocsessionDetailsHeader; ?></div>
							</div>
			    	
			    	<?php }?>
			    	<?php if (!empty($result->data_comp_by) && ($callFrom == 'course_details' || $callFrom == 'lrnplan_details')): ?>
			    		<div class="line-item-container float-left">
				    		<div title="course-info-seperator" class="divider-pipeline line-item">|</div>
					    	<div class="detail-comp line-item"><?php print t("LBL234"); ?>&nbsp;:&nbsp;<!-- Complete By: --></div>
								<div class="vtip line-item" title="<?php print $result->data_comp_by; ?>"><?php print $result->data_comp_by; ?><!-- 30-01-2017 --></div>
							</div>
			    	<?php endif; ?>
			    	<?php if (!empty($result->data_expire_on) && $callFrom == 'lrnplan_details'): ?>
			    		<div class="line-item-container float-left">
				    		<div title="course-info-seperator" class="divider-pipeline line-item">|</div>
					    	<div class="detail-comp line-item"><?php print t("LBL233"); ?>&nbsp;:&nbsp;<!-- Expires In: --></div>
					    	<div class="vtip line-item" title="<?php print $result->data_expire_on; ?>"><?php print $result->data_expire_on; ?><!-- 150 days after completion --></div>
					    </div>
			    	<?php endif; ?>
			    	<?php if (!empty($result->training_type) && $callFrom == 'lrnplan_course_list'): ?>
			    		<div class="line-item-container float-left">
			    			<div title="course-info-seperator" class="divider-pipeline line-item">|</div>
				    		<div class="detail-comp line-item"><?php print $result->training_type; ?><!-- Mandatory/Optional: --></div>
				    	</div>
				  	<?php endif; ?>
			    
			<?php } else if($callFrom == 'catalog' || $callFrom == 'tp_prereq') {?>
				<?php	//Catalog page will send bunch of elements in an array and we have to loop through
					$pipe_line = '';
					foreach($result->attributes as $items=>$item) {
						if($items!='locationtz'){/*Viswanathan added for #76619,#76437,#76441 */
						$span = $item->hidden ? '<div>' :'<div class="line-item-container float-left">';
						//$item->tooltip = htmlentities($item->tooltip,ENT_QUOTES,'UTF-8');
						//print ($item->hidden ? '' : $pipe_line);
						$span .= ($item->hidden ? '' : $pipe_line);
						$span .= '<span tst="sdsdf" ';
						$span .= (!empty($item->class) ? 'class="vtip '.$item->class.'"' : 'class="vtip"');
						$span .= (!empty($item->id) ? 'id="'.$item->id.'"' : '');
						$span .= (!empty($item->tooltip) ? 'title="'.htmlentities($item->tooltip,ENT_QUOTES,'UTF-8').'"' : ''); /*Viswanathan changed for #73045*/
						$span .= ($item->hidden ? 'style="display: none;"' : '');
						$span .= '>';
						$span .= $item->data;
						$span .= '</span>';
						$span .= '</div>';
						print $span;
						$pipe_line = '<span class="pipe-line">|</span>';}
					} ?>
			<?php } print $result->attributes['locationtz']->locationtztext;?>
		    </div>
			<?php if((!isset($_SESSION['widget']['catalog_display_parameters']) || $callFrom != 'catalog') || ( $callFrom == 'catalog' && isset($_SESSION['widget']['catalog_display_parameters']) &&  $_SESSION['widget']['catalog_display_parameters']['show_description']==TRUE)){?>
				<?php if($hideDesc != 'hide'){ // Hide the Description?>
					<div class="limit-desc-row content-description <?php print $descClass; ?> <?php print $result->delivery_type_code; ?> <?php print (!empty($result->content_description_class) ? $result->content_description_class : ''); ?>">
						<div class="limit-desc <?php print $trunk8_class . '-desc';?> vtip"><span class="cls-learner-descriptions"><?php print $result->data_desc; ?></span></div>
					</div>
    			<?php } ?>
			
			
			<div class="content-rateshare-container">
				<?php $pipe_line = ''; ?>
				<?php if(!empty($_SESSION['availableFunctionalities']->exp_sp_fivestar)) { ?>
					<div id="ratingBlock" class="rating-block">
						<?php $rating_node_id = (strpos($result->node_id, ',') !== false ? str_replace(',', '-', $result->node_id) : $result->node_id); ?>
						<?php  print "<div id='cls-node-".$rating_node_id."' class='float-left'>".drupal_render(drupal_get_form('fivestar_rating_widget', '', $result->start_rate_setting))."</div>";?>
					</div>
					<?php $pipe_line = '<span class="pipe-line line-item">|</span>'; ?>
				<?php } ?>
				<?php if($callFrom == 'catalog' && isset($result->tag_info_div_id)) { ?>
					<div id="<?php print $result->tag_info_div_id;?>" class="<?php print $result->tag_info_div_class;?>">
						<?php print $pipe_line; ?>
  				  		<span title="<?php print $result->tag_info_label_title;?>" class="<?php print $result->tag_info_label_class;?>"><?php print $result->tag_info_label_data;?></span> 
  				  		<span class="<?php print $result->tag_info_content_class;?>" title="<?php print $result->tag_info_content_title;?>"><?php print $result->tag_info_content_data;?></span>
					</div>
					<?php $pipe_line = '<span class="pipe-line line-item">|</span>'; ?>
  				<?php } ?>
				<?php 
				$sharemodule=getShareModuleStatus($callFrom);							
				if($sharemodule && $result->share_icon==true && ($callFrom=='catalog' || $callFrom=='class_details' || $callFrom=='course_details' || $callFrom=='lrnplan_details')){?> 
				<div id="shareoption">
				<?php print $pipe_line; ?>
				<div
					id="widget_share_qtip_visible_disp_<?php  print $result->qtipIdInit ?>"
					class="clsShareOption">
					<span title="<?php print t('Share');?>" class="share-tab-icon vtip"></span>
					<span title="<?php print t('Share');?>" class="vtip"><?php print '<a id="widget-share-'.$result->entity_id.'" class="single-share-link" onclick = "$(\'body\').data(\'refercourse\').getEmbedReferDetails(\'lnr-catalog-search\', \''.$result->delType.'\', '.$result->qtipOptAccessObj.',\'Popup\');">'. t('Share') .'</a>'; ?></span><span
						style="position: absolute; left: 0px; top: 0px;"
						class="qtip-popup-visible"
						id="visible-popup-<?php print $result->entity_id;?>"></span>
				</div>
				<span id="visible-popup-<?php print $result->class_id;?>"
					class='qtip-popup-visible'
					style='display: none; position: absolute; left: 0px; top: 0px;'></span>
			</div>
					<?php $pipe_line = '<span class="pipe-line line-item">|</span>'; ?>
				<?php }?>
				<?php if(!empty($result->request_cls_option_data)) { ?>
					<div id="request-class-option" class="request-class">
			        	<?php print $pipe_line; ?>
			        	<span title="<?php print $header_details->request_cls_option_title; ?>" class="<?php print $result->request_cls_option_class; ?>"></span>
			        	<span>
			        		<?php print $result->request_cls_option_data;?>
			        	</span>
		    		</div>
					<?php $pipe_line = '<span class="pipe-line line-item">|</span>'; ?>
				<?php }?>
			</div>
			
			<?php }if($callFrom == 'catalog' && !empty($result->scripts)){
				print $result->scripts;
			}?>
		<?php
		if($callFrom=='lrnplan_course_class_list'){?>
			<?php if($result->program_register_flag==true){
			?>
			<div class="class-content-more padbt5" id="class_content_more_<?php print $result->class_id ; ?>"><div class="show_more_less" onclick="showClassDetailMore(<?php print $result->class_id; ?>,'<?php print $result->delivery_type_code;?>','<?php print $result->comp_status;?>')"><?php echo t('LBL713');?></div></div>
			<?php }else{?>
			<div class="class-content-more padbt5" id="class_content_more_<?php print $result->class_id ; ?>"><div class="show_more_less" onclick="showClassDetailMore(<?php print $result->class_id; ?>,'<?php print $result->delivery_type_code;?>','no_show')"><?php echo t('LBL713');?></div></div>
			<?php }?>
							<?php }else if($callFrom == 'tp_prereq') {

								print $result->pre_req_show_more;
							} else{
			if($callFrom=='course_class_list') {
			?>
			<div class="class-content-more padbt5" id="class_content_more_<?php print $result->class_id ; ?>"><div class="show_more_less" onclick="showClassDetailMore(<?php print $result->class_id; ?>,'<?php print $result->delivery_type_code;?>','<?php print $result->comp_status;?>')"><?php echo t('LBL713');?></div></div>
			<?php } } 
	?>	
	</div>
<?php if($callFrom!='course_details' && $callFrom!='lrnplan_course_list'):
	 if((!isset($_SESSION['widget']['catalog_display_parameters']) || $callFrom != 'catalog') || ($_SESSION['widget']['catalog_display_parameters']['show_button']==true && $callFrom == 'catalog')){ ?>
	<div
		class='content-action-container<?php print (!empty($result->content_action_container_class) ? $result->content_action_container_class : ' float-left');?>'>
	<?php expDebug::dPrint('$result->action_variables ' . print_r($result->action_variables, 1), 5);?>
		<?php if($callFrom == 'tp_prereq') {
			echo $result->action_variables;
		} else {
			echo theme("content_action", array("result" => $result->action_variables, "callFrom" => $callFrom));
		}?>
	</div>
	
<?php } endif; 
$callFrompage = $context['callfrom'];
if(contentPlayerIsActive() && ($result->delivery_type_code=='lrn_cls_dty_wbt' || $result->delivery_type_code=='lrn_cls_dty_vod')): ?>	
	<?php if($callFrompage == 'lrnplan_course_class_list'){
		if($result->master_enrollment_id>0){
	?>	
	<span id="paindContentclsid_<?php echo $result->class_id; ?>" class="classMenucontainerfunctionelement"
		style="display: none;" alt="<?php print $result->enrolled_id;?>"
		onclick="<?php print "$('#lnr-catalog-search').data('contentPlayer').playContentMylearning({'masterEnrollId':'".$result->master_enrollment_id."','enrollId':'".$result->enrolled_id."','programId':'0','courseId':'".$result->course_id."','classId':'".$result->class_id."','userId':'".$result->user_id."','LaunchFrom':'CL','defaultContent':'0','classTitle':'check the multiple content','pagefrom':'".$callFrompage."',});" ?>"></span>
	<?php } }else{
		?>
		<span id="paindContentclsid_<?php echo $result->class_id; ?>" class="classMenucontainerfunctionelement"
			style="display: none;" alt="<?php print $result->enrolled_id;?>"
			onclick="<?php print "$('#lnr-catalog-search').data('contentPlayer').playContentMylearning({'masterEnrollId':'".$result->master_enrollment_id."','enrollId':'".$result->enrolled_id."','programId':'0','courseId':'".$result->course_id."','classId':'".$result->class_id."','userId':'".$result->user_id."','LaunchFrom':'CL','defaultContent':'0','classTitle':'check the multiple content','pagefrom':'".$callFrompage."',});" ?>"></span>
		<?php 		
	} endif; ?>	
	<span id="reccuring_<?php echo $result->class_id; ?>" 	style="display: none;" value=<?php echo $result->recuring_class_flag; ?>></span>
</div>
