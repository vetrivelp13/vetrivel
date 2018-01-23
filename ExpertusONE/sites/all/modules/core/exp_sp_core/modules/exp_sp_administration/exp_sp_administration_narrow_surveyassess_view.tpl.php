<?php 
expDebug::dPrint('enter the function sasasasAAAA'.print_r($results,true),4);
global $theme_key;
expDebug::dPrint('enter the function sasasas'.print_r($theme_key,true),4);
?>
<div class="top-record-div-left">
 <div class="para">
  <div class="code-container">
	   <span class="detail-code"><?php print t("LBL083"); ?>:</span>
	     <span class="detail-desc limit-title-row"><span class="vtip limit-title" title="<?php print sanitize_data($results[0]->title); ?>"> <?php print $results[0]->title; ?>  </span></span>
	</div>
 </div>
 <div class="left-side-container">
  <div class="para">
   <div class="code-container">
	   <span class="detail-code"><?php print t("LBL096"); ?>:</span>
	    <span class="detail-desc detail-single-line"><span class="single-line-lbl-val vtip" title="<?php print sanitize_data($results[0]->code); ?>"> <?php print titleController('ADMIN-NARROW-SURVEYASSESS-LEFT-CODE', $results[0]->code,45); ?>  </span></span>
	</div>
 </div>
 <div class="para">
  <div class="code-container">
	   <span class="detail-code"><?php print t("LBL038"); ?>:</span>
	    <span class="detail-desc"><?php print t($results[0]->lang); ?></span>
	</div>
  </div>
 <?php if($type == 'sry_det_typ_ass') {?>
  <div class="para">
   <div class="code-container">
	   <span class="detail-code"><?php print t("LBL955"); ?>:</span>
	    <span class="detail-desc"><?php !empty($results[0]->random) ? print t($results[0]->random) : print '-'; ?></span>
	</div>
  </div>
 <?php }?> 
 </div>
 <div class="right-side-container">
 <div class="para">
   <div class="code-container">
	   <span class="detail-code"><?php print t("LBL332"); ?>:</span>
	    <span class="detail-desc"><?php print $results[0]->perpage; ?></span>
	</div>
 </div>
 <?php if($type == 'sry_det_typ_ass') {?>
  <div class="para">
   <div class="code-container">
	    <span class="detail-code"><?php print t("LBL329"); ?>:</span>
	    <span class="detail-desc"><?php print $results[0]->min; ?></span>
	 </div>
 </div>
 <div class="para">
   <div class="code-container">
	    <span class="detail-code"><?php print t("LBL330"); ?>:</span>
	    <span class="detail-desc"><?php print $results[0]->max; ?></span>
	 </div>
 </div>
 <?php }?>
 </div>
 <div class="para">
  <div class="code-container">
	   <span  class="detail-code"><?php print t("LBL229"); ?>:</span>
	    <span id="survey-detail-desc" class="detail-desc"><?php print $results[0]->descrip; ?></span>
	</div>
 </div>
  <?php $access  = getGroupNamesForView($results[0]->id, $type);
  			$accessName  = '';
	if(!empty($access)){
	  $accessName  = implode(", ",$access);
	}
	?>
 <div class="para">
	<div class="code-container">
		<span  class="detail-code"><?php print t("LBL642"); ?>:</span>
		 <span id="access-detail-desc" class="detail-desc detail-single-line"><span class="single-line-lbl-val vtip" title="<?php print sanitize_data($accessName); ?>"> <?php ($theme_key == 'expertusoneV2')? print titleController('ADMIN-NARROW-SURVEYASSESS-ACCESSNAME', $accessName,75) :  print titleController('ADMIN-NARROW-SURVEYASSESS-ACCESSNAME', $accessName,65) ; ?>  </span></span>                        				
	</div>
 </div>
  <?php $custom = getCustomFieldsForView($results[0]->id, $type);
 		expDebug::dPrint(' $$custom'.print_r( $custom,true),5);?>
 <?php foreach($custom as $customInfo) :?>
	 <div class="para">
		<div class="code-container custom-field-container">
			<span class="detail-code"><span class="vtip" title="<?php print sanitize_data($customInfo->label); ?>"><?php print $customInfo->label; ?></span>:</span>
			<span  id="custom-detail-desc" class="detail-desc detail-single-line vtip" title="<?php print sanitize_data($customInfo->value); ?>"><?php print titleController('ADMIN-NARROW-SURVEYASSESS-CUSTOMINFO-VALUE',$customInfo->value,20); ?></span>
		</div>   
	 </div>
 <?php endforeach;?>
 	<div class="para">
		<?php $record= getQuestionsDetails($results[0]->id); 
					expDebug::dPrint('$record[$i]$record[$i]'.print_r($record,true),4);
			if(count($record) > 0) { ?>
					<div class="code-container">
						<div class="detail-code-container surveyassqa-title"><?php print t("LBL240"); ?>:</div>
							<span class="detail-desc">
								<table cellpadding="0" class="assess-question-detais" cellspacing="0" width="500" border="0" style="margin-bottom: 5px;">
										<tr>
											<th><?php print t("LBL325"); ?></th>
											<th><?php print t("LBL379"); ?></th>
											<?php if($type == 'sry_det_typ_ass') {?>
											 <th><?php print t("LBL668"); ?></th>
											<?php }else {?>
											 <th></th>
											<?php }?>
										</tr>
											<?php 
												$i=0;
							        foreach ($record as $key=>$value): 
								        if(($i%2) == 0){
								        	$class='table-detail-tr-even';
								        }else{
								        	$class='table-detail-tr-odd';
								        }
							         expDebug::dPrint('$record[$i]$record[$i]'.print_r($value,true),4);
							         expDebug::dPrint('$key$record[$i]'.print_r($key,true),4);?>
							        <tr class="<?php print $class;?>">
												<td valign="top"  width="65%"  class="class-detail-session-name"><span class="vtip" title="<?php print sanitize_data($value->question_txt); ?>"><?php ($theme_key == 'expertusoneV2')? print titleController('ADMIN-NARROW-SURVEYASSESS-QUESTION-TEXT',($key+1).'. '.$value->question_txt,57) :  print titleController('ADMIN-NARROW-SURVEYASSESS-QUESTION-TEXT',($key+1).'. '.$value->question_txt,52); ?></span></td>
												<td valign="top" width="25%" class="group-name"><span class="vtip" title="<?php print sanitize_data($value->survey_group_title); ?>"><?php print titleController('SURVEY-GROUP-TITLE',$value->survey_group_title,20); ?></span></td>
												<?php if($type == 'sry_det_typ_ass') {?>
												 <td valign="top" class="score-column"><?php print $value->survey_score; ?></td>
												<?php } else {?>
												 <td valign="top" class="mro-column"><?php ($value->mandatory_option == 'N')? print t('Optional'):print t('Mandatory') ?> </td>
												<?php }?>
											</tr>
											<?php $i++;
											 endforeach; ?> 
								</table>
						</span>
					</div>
				</div>
  <?php } else {?>
		<div class ="nonlist"><?php print t('MSG368')?></div>
	<?php } ?>
</div> 
