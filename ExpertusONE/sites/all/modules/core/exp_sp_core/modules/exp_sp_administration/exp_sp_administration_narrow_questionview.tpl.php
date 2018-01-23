<?php 
expDebug::dPrint('enter the function sasasasAAAA'.print_r($results,true),4);
global $theme_key;
?>
<div class="top-record-div-left">
 <div class="para">
  <div class="code-container">
	   <span class="detail-code"><?php print t("LBL240"); ?>:</span>
	    <span class="detail-desc limit-title-row"> <span class="vtip limit-title" title="<?php print sanitize_data($results[0]->title); ?>"><?php print $results[0]->title; ?></span> </span>
	</div>
 </div>
 <div class="para">
  <div class="code-container">
	   <span class="detail-code"><?php print t("LBL038"); ?>:</span>
	    <span class="detail-desc"><?php print t($results[0]->lang); ?></span>
	</div>
 </div>
 <div class="para">
  <div class="code-container">
	   <span class="detail-code"><?php print t("LBL242"); ?>:</span>
	    <span class="detail-desc"><?php print t($results[0]->type); ?></span>
	</div>
 </div>
 <?php $access  = getGroupNamesForView($results[0]->id, $questiontype);
expDebug::dPrint('$access-->'.print_r($access,true),4);
  $accessName  = '';
	if(!empty($access)){
	  $accessName  = implode(", ",$access);
	}
	$accessName=sanitize_data($accessName); // Added by ganeshbabuv on June 29th 2015 5:10 PM for security fixing 
	?>
 <div class="para">
	<div class="code-container">
		<span class="detail-code"><?php print t("LBL642"); ?>:</span>
		<span id="access-deatil-desc" class="detail-desc detail-single-line"><span class="single-line-lbl-val vtip" title="<?php print $accessName; ?>"> <?php ($theme_key == 'expertusoneV2')? print titleController('ADMIN-NARROW-QUESTION-VIEW-ACCESSNAME', $accessName,75) : print titleController('ADMIN-NARROW-QUESTION-VIEW-ACCESSNAME', $accessName,65); ?>  </span></span>                       				
	</div>
 </div>
 <?php  $tags  = getCatalogTags($results[0]->id, $questiontype);
	$tagString = '';
	$vtip_class_tags = '';
	if(!empty($tags)){
	  $tagString  = implode(", ",$tags);
	  $vtip_class_tags = ($tagString != '') ? 'vtip':'';
	}
?>
 <div class="para">
	 <div class="code-container">
	   <span class="detail-code"><?php print t("LBL191"); ?>:</span>
	   <span id="tag-detail-desc-view" class="detail-desc detail-single-line">
	    <span class="single-line-lbl-val <?php print $vtip_class_tags; ?>" title="<?php print sanitize_data($tagString); ?>"> 
	    	<?php print titleController('ADMIN-NARROW-SEARCH-QUESTION-VIEW-TAG-STRING', sanitize_data($tagString),100); ?>
	    </span>  
	   </span>
	</div>
 </div>
 <div class="para">
  <div class="code-container">
	   <span class="detail-code"><?php print t("Answers"); ?>:</span>
 <?php  $answer = explode('##',$results[0]->answer);
  expDebug::dPrint('$answer'.print_r($answer,true),4);?>
 <?php if($type == 'sry_det_typ_sry') {?>
 <?php if($results[0]->type == 'Multiple Choice' || $results[0]->type == 'Dropdown' || $results[0]->type == 'Rating') {
 	?>
  <span class="question-opt-container">
	   <?php $i=0; 
        foreach ($answer as $key=>$item): 
        expDebug::dPrint('$record[$i]$record[$i]'.print_r($item,true),4);
         expDebug::dPrint('$key$record[$i]'.print_r($key,true),4);?>
        <span class="opt-detail-code"><?php print t("LBL387").($key+1); ?>:</span>
		    <span class="sur-opt-detail-desc"><?php print $item."\n"; ?></span>
		   <?php $i++;
       endforeach; ?> 
    </span>
	</div>
 </div>
 <?php } else if($results[0]->type == 'Yes/No') {?>
	  <span class="question-opt-container">
	        <span class="sub-opt-detail-code"><?php print t("Yes"); ?></span>
			<span class="sub-opt-detail-code"><?php print t("No"); ?></span>
	    </span>
		</div>
	 </div>
	 <?php } else if($results[0]->type == 'True/False') {?>
	  <span class="question-opt-container">
	        <span class="sub-opt-detail-code"><?php print t("LBL384"); ?></span>
			<span class="sub-opt-detail-code"><?php print t("LBL385"); ?></span>
	    </span>
		</div>
	 </div>
	 <?php } ?> 
 <?php } else {
	 if($results[0]->type == 'Dropdown') {?>
	  <span class="question-opt-container">
		   <?php $i=0; 
	        foreach ($answer as $key=>$item): 
	        expDebug::dPrint('$record[$i]$record[$i]'.print_r($item,true),4);
	         expDebug::dPrint('$key$record[$i]'.print_r($key,true),4);?>
	        <span class="opt-detail-code"><?php print t("LBL387").($key+1); ?>:</span>
			    <span class="opt-detail-desc"><?php print $item."\n"; ?></span>
			    <input id="option-list" class="multiselect readonly" type="radio" value="<?php $results[0]->correct?>" <?php if($results[0]->correct == $item): ?> checked="checked" <?php endif;?> disabled name="option-list">
			    <span class="correct-detail-desc" ><?php print t('LBL714');?></span>
			   <?php $i++;
	       endforeach; ?> 
	    </span>
		</div>
	 </div>
	  <?php } else if($results[0]->type == 'Multiple Choice') {?>
	  <span class="question-opt-container">
		   <?php $i=0; 
	        foreach ($answer as $key=>$item): 
	        expDebug::dPrint('$record[$i]$record[$i]'.print_r($item,true),4);
	         expDebug::dPrint('$key$record[$i]'.print_r($key,true),4);?>
	        <span class="opt-detail-code"><?php print t("LBL387").($key+1); ?>:</span>
			    <span class="opt-detail-desc"><?php print $item."\n"; ?></span>
			     <?php $correct = explode('##',$results[0]->correct);?>
			    <input id="option-list" class="multiselect" type="checkbox" value="<?php $results[0]->correct?>" <?php if(in_array($item,$correct)): ?> checked="checked"  <?php endif;?> disabled name="option-list">
			    <span class="correct-detail-desc" ><?php print t('LBL714');?></span>
			   <?php $i++;
	       endforeach; ?> 
	    </span>
		</div>
	 </div>
	 <?php } else if($results[0]->type == 'Yes/No') {?>
	  <span class="question-opt-container">
	        <span class="sub-opt-detail-code"><?php print t("Yes"); ?>:</span>
	        <input id="option-list" class="multiselect readonly" type="radio" value="yes" <?php if($results[0]->correct == "Yes"): ?> checked="checked" <?php endif;?> disabled name="option-list">
	        <span class="correct-detail-desc"><?php print t('LBL714');?></span>
			    <span class="sub-opt-detail-code"><?php print t("No"); ?>:</span>
			    <input id="option-list" class="multiselect readonly" type="radio" value="no" <?php if($results[0]->correct == "No"): ?> checked="checked" <?php endif;?> disabled name="option-list">
			    <span class="correct-detail-desc" ><?php print t('LBL714');?></span>
	    </span>
		</div>
	 </div>
	 <?php } else if($results[0]->type == 'True/False') {?>
	  <span class="question-opt-container">
		   <span class="sub-opt-detail-code"><?php print t("LBL384"); ?>:</span>
	     <input id="option-list" class="multiselect readonly" type="radio" value="true" <?php if($results[0]->correct == "True"): ?> checked="checked" <?php endif;?> disabled name="option-list">
	     <span class="correct-detail-desc" ><?php print t('LBL714');?></span>
			 <span class="sub-opt-detail-code"><?php print t("LBL385"); ?>:</span>
			  <input id="option-list" class="multiselect readonly" type="radio" value="false" <?php if($results[0]->correct == "False"): ?> checked="checked" <?php endif;?> disabled name="option-list">
			  <span class="correct-detail-desc" ><?php print t('LBL714');?></span>
	    </span>
		</div>
	 </div>
	 <?php }?>
 <?php }?>
</div> 