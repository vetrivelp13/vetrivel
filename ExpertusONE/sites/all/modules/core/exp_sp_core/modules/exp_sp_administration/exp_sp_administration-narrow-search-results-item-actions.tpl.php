<?php 
expDebug::dPrint('exp_sp_administration-narrow-search-results-item-actions.tpl.php In narrow search result item actions tpl file : $edit_button_action_params = ' . print_r($edit_button_action_params, true),4);
expDebug::dPrint('exp_sp_administration-narrow-search-results-item-actions.tpl.php In narrow search result item actions tpl file : $edit_button_action_params = ' . print_r($catalog_details, true),4);
 ?>
<div id='<?php print $html_id; ?>' class="narrow-search-results-item-action">
  
  <!-- </a> 
  <a class="crs-publish-btn">
  Publish
  </a>-->
  <!--   <a class="crs-unpublish-btn">
  Unpublish
  </a>
   </a>
   <a class="crs-pub-enrolled-btn">
  Enrolled
  </a>  -->

<?php
//print $catalog_details->object_type;
//print $catalog_details->type;
if(!empty($catalog_details->iscompliance)){
	$iscompliance = $catalog_details->iscompliance;
}else{
	$iscompliance = '0';
}
$catalogId  = ($catalog_details->object_type=='Class')? $catalog_details->class_id : $catalog_details->id;
if($catalog_details->object_type == 'Class'){
  //$unpublishStatus = getEnrolledClassDeleteAccess($catalog_details->class_id);
  $classValid = checkClassValidity($catalogId, $catalog_details->delivery_type_code);
  
  $enrStatus = ifClassCanBeCanceled($catalogId);
  
  if (($catalog_details->delivery_type_code == 'lrn_cls_dty_ilt' || $catalog_details->delivery_type_code == 'lrn_cls_dty_vcl')
                      && $catalog_details->status_code != 'lrn_cls_sts_atv' && $catalog_details->status_code != 'lrn_cls_sts_itv' ) {
  ?>
  <?php $statusType = t('Completed'); $actionInfo = ($catalog_details->status == $statusType) ? '' : 'action-text-wrapper-controller';?>
  	<a id="publish-unpublish-<?php print $catalogId;?>" class="crs-pub-enrolled-btn <?php print $actionInfo; ?>">
  	   <?php print t($catalog_details->status);?>
  	</a>
  <?php 
  } else if (($catalog_details->delivery_type_code == 'lrn_cls_dty_ilt' || $catalog_details->delivery_type_code == 'lrn_cls_dty_vcl')
                      && $classValid->flag == 0 && $classValid->totalSessions > 0 &&
                               $classValid->inProgressSessions <= 0 && $classValid->futureSessions <= 0 && $enrStatus == 0) {?>
                      
  	<a id="publish-unpublish-<?php print $catalogId;?>" class="crs-publish-btn action-crs-publish-btn"
  	     onclick='$("#root-admin").data("narrowsearch").publishAndUnpublishCatalog("<?php print $catalogId; ?>","<?php print $catalog_details->object_type; ?>","lrn_cls_sts_dld",this);'>
  	   <?php print t('LBL639');?>
  	</a>
  <?php } else if ($catalog_details->status_code == 'lrn_cls_sts_atv') {
  	$directEnrollCount = numClassDirectRegistrationEnrollments($catalogId);
  	$viaTPEnrollCount = numClassTPRegistrationEnrollments($catalogId);  	
  	
      if($classValid->flag == 0){
    		if ($catalog_details->delivery_type_code=='lrn_cls_dty_vcl' || $catalog_details->delivery_type_code=='lrn_cls_dty_ilt') {
    			if (($classValid->inProgressSessions + $classValid->futureSessions) > 0) {
    			  $labelType = t('Class') . ' ' . t('In progress');
    			}
    			else {
    			  $labelType = t('LBL634');
    			}
    		}
    		else {
    			$labelType = t('LBL635');
    		}
   ?>
        <a id="publish-unpublish-<?php print $catalogId;?>" class="crs-pub-enrolled-btn action-text-wrapper-controller"><?php print $labelType; ?></a>
            		
   <?php } else	if($directEnrollCount <= 0 && $viaTPEnrollCount <= 0 ){?>
   <?php if($catalog_details->sumedit	> 0  || $GLOBALS["user"]->uid == 1){?>
		<a id="publish-unpublish-<?php print core_encrypt($catalogId);?>"
		     class="crs-unpublish-btn action-crs-unpublish-btn action-text-wrapper-controller hide-show-txt-ie7-fix"
		       onclick='$("#root-admin").data("narrowsearch").publishAndUnpublishCatalog("<?php print core_encrypt($catalogId); ?>","<?php print $catalog_details->object_type; ?>","lrn_cls_sts_itv",this);'>
		     <?php print t('LBL575');?>
		</a>
		<?php } else{
			?>
			<a id="publish-unpublish-<?php print core_encrypt($catalogId);?>"
		     class="crs-pub-enrolled-btn action-crs-unpublish-btn action-text-wrapper-controller hide-show-txt-ie7-fix">
		     <?php print t('LBL575');?>
		</a>
			<?php 
		}?>
	<?php } else if ($directEnrollCount <= 0) {?>
	  <a id="publish-unpublish-<?php print $catalogId;?>" class="crs-pub-enrolled-btn action-text-wrapper-controller hide-show-txt-ie7-fix">
	    <?php print t('Active') . ' ' . t('TP') . ' ' . t('LBL275'); ?>
	  </a>
	<?php } else { ?>
		<a id="publish-unpublish-<?php print $catalogId;?>" class="crs-pub-enrolled-btn action-text-wrapper-controller hide-show-txt-ie7-fix">
		  <?php print t('LBL633'); ?>
		</a>  
	<?php }  
  } else if ($catalog_details->status_code=='lrn_cls_sts_itv') {
  	$completed_flag = '';
    	if($classValid->flag == 0 || $classValid->coursestatus != "lrn_crs_sts_atv") {
    		if($classValid->flag == 0){
    			if ($catalog_details->delivery_type_code=='lrn_cls_dty_vcl' || $catalog_details->delivery_type_code=='lrn_cls_dty_ilt') {
    				if (($classValid->inProgressSessions + $classValid->futureSessions) > 0) {
    				  $labelType = t('Class') . ' ' . t('In progress');
    				}
    				else {
    					$sessionCount = checkSessionContent($catalogId);
    					$directEnrollCount = numClassDirectRegistrationEnrollments($catalogId);
    					$viaTPEnrollCount = numClassTPRegistrationEnrollments($catalogId);
    					if($directEnrollCount > 0 || $sessionCount < 1) {
    						$labelType = t('LBL634');
    					}else {
    						$labelType = t('LBL639');
    						$completed_flag = 1;
    					}
    				}
    			}
    			else {
    				$labelType = t('LBL635');
    			}
            if($completed_flag == 1) {  ?> 
   					<a id="publish-unpublish-<?php print core_encrypt($catalogId);?>" class="crs-publish-btn action-crs-publish-btn action-text-wrapper-controller hide-show-txt-ie7-fix" 
  					onclick='$("#root-admin").data("narrowsearch").publishAndUnpublishCatalog("<?php print core_encrypt($catalogId); ?>","<?php print $catalog_details->object_type; ?>","lrn_cls_sts_dld",this);'>
  					<?php print $labelType;?>
  					</a> <?php 
   				} else {?>
   				<a id="publish-unpublish-<?php print $catalogId;?>" class="crs-pub-enrolled-btn action-text-wrapper-controller"><?php print $labelType; ?></a>   
   		<?php } ?>					 			
    	<?php }else if ($classValid->coursestatus != "lrn_crs_sts_atv"){
    			$labelType = t('LBL638');?>
    			<a id="publish-unpublish-<?php print $catalogId;?>" class="crs-pub-enrolled-btn action-text-wrapper-controller hide-show-txt-ie7-fix"><?php print $labelType; ?></a>
    	<?php }
  		}else if($catalog_details->sumedit	> 0  || $GLOBALS["user"]->uid == 1){?>
  			<a id="publish-unpublish-<?php print core_encrypt($catalogId);?>" class="crs-publish-btn action-crs-publish-btn action-text-wrapper-controller hide-show-txt-ie7-fix" 
  			onclick='$("#root-admin").data("narrowsearch").publishAndUnpublishCatalog("<?php print core_encrypt($catalogId); ?>","<?php print $catalog_details->object_type; ?>","lrn_cls_sts_atv",this,<?php print $iscompliance;?>);'>
  			<?php print t('LBL576');?>
  			</a>
  	
  <?php }else{
  	?>
  	<a id="publish-unpublish-<?php print core_encrypt($catalogId);?>" class="crs-pub-enrolled-btn action-crs-publish-btn action-text-wrapper-controller hide-show-txt-ie7-fix">
  	  			<?php print t('LBL576');?>
  	  			</a>
  	  			<?php
  }
  }else if ($catalog_details->status_code=='lrn_cls_sts_dld'){?>
    <a id="publish-unpublish-<?php print core_encrypt($catalogId);?>" class="crs-pub-enrolled-btn"><?php print t('Completed'); ?></a>
  <?php }else if ($catalog_details->status_code=='lrn_cls_sts_can'){ ?>
            <a id="publish-unpublish-<?php print core_encrypt($catalogId);?>" class="crs-pub-enrolled-btn"><?php print t('Canceled'); ?></a>
 <?php    }
       }else if($catalog_details->object_type=='Course'){
        if($catalog_details->status_code=='lrn_crs_sts_atv'){
       $ClassDeleted = getIfClassBeDelete($catalogId);
       if($ClassDeleted ==0){?>
        	<a id="publish-unpublish-<?php print core_encrypt($catalogId);?>" class="crs-pub-enrolled-btn"><?php print t('LBL636')?></a> 
      <?php }else{
                $unpublishStatus = getIfCourseBeUnpublished($catalogId);
                if($unpublishStatus == 0){
     ?><?php if($catalog_details->sumedit > 0  || $GLOBALS["user"]->uid == 1){?>
  					<a id="publish-unpublish-<?php print core_encrypt($catalogId);?>" class="crs-unpublish-btn" onclick='$("#root-admin").data("narrowsearch").publishAndUnpublishCatalog("<?php print core_encrypt($catalogId); ?>","<?php print $catalog_details->object_type; ?>","lrn_crs_sts_itv",this);'><?php print t('LBL571');?></a>
      <?php 
     }else{
     	?>
     	<a id="publish-unpublish-<?php print core_encrypt($catalogId);?>" class="crs-pub-enrolled-btn"><?php print t('LBL571');?></a>
     	<?php 
     }  }else{?>
  					<a id="publish-unpublish-<?php print core_encrypt($catalogId);?>" class="crs-pub-enrolled-btn"><?php print t('Published');?></a>
      <?php      }
    }
  }
  else{
  	$sessionValid = getIfCourseValid($catalogId);    	
    if($sessionValid[0]->sesavailable == 0){ ?>
  		<a id="publish-unpublish-<?php print core_encrypt($catalogId);?>" class="crs-pub-enrolled-btn"><?php print t('LBL636')?></a>
  	<?php }else if($catalog_details->sumedit > 0  || $GLOBALS["user"]->uid == 1){?>
  		<a id="publish-unpublish-<?php print core_encrypt($catalogId);?>" class="crs-publish-btn" onclick='$("#root-admin").data("narrowsearch").publishAndUnpublishCatalog("<?php print core_encrypt($catalogId); ?>","<?php print $catalog_details->object_type; ?>","lrn_crs_sts_atv",this);'><?php print t('LBL570');?></a>
    <?php 	}else{
    	?>
<a id="publish-unpublish-<?php print core_encrypt($catalogId);?>" class="crs-pub-enrolled-btn"><?php print t('LBL570');?></a>    	
    	<?php 
    }
        
  }
}else if($catalog_details->object_type=='Certification' || $catalog_details->object_type=='Curricula' || $catalog_details->object_type=='Learning Plan'){
  if($catalog_details->status_code=='lrn_lpn_sts_atv'){
    $unpublishStatus = getEnrolledTPDeleteAccess($catalog_details->id);
    if(!$unpublishStatus){
    	if($catalog_details->sumedit	> 0 || $GLOBALS["user"]->uid == 1){
    ?>        
		<a id="publish-unpublish-<?php print core_encrypt($catalogId);?>" class="crs-unpublish-btn" onclick='$("#root-admin").data("narrowsearch").publishAndUnpublishProgram("<?php print core_encrypt($catalogId); ?>",this);'>
		<?php print t('LBL571');?>
		</a>
    <?php
    	}else{
    		?>
    			<a id="publish-unpublish-<?php print core_encrypt($catalogId);?>" class="crs-pub-enrolled-btn">
		<?php print t('LBL571');?>
		</a>
    		<?php 
    	} 
    }else{?>
		<a id="publish-unpublish-<?php print core_encrypt($catalogId);?>" class="crs-pub-enrolled-btn"><?php print t('Published');?></a>
    <?php }
  }else{
   $programValid = getIfProgramValid($catalogId);            
   if($programValid[0]->sesavailable == 0){ ?>
                 <a id="publish-unpublish-<?php print core_encrypt($catalogId);?>" class="crs-pub-enrolled-btn"><?php print t('LBL637')?></a>
         <?php }else if($catalog_details->sumedit > 0  || $GLOBALS["user"]->uid == 1){
                 ?>
                 <a id="publish-unpublish-<?php print core_encrypt($catalogId);?>" class="crs-publish-btn" onclick='$("#root-admin").data("narrowsearch").publishAndUnpublishProgram("<?php print core_encrypt($catalogId); ?>",this);'><?php print t('LBL570');?></a>
                 <?php
         }else{?>
                 <a id="publish-unpublish-<?php print core_encrypt($catalogId);?>" class="crs-pub-enrolled-btn"><?php print t('LBL570');?></a>
         <?php }
  }
}else if(($catalog_details->type=='Assessment')|| ($catalog_details->type=='Survey')){
   if($catalog_details->status_code=='sry_det_sry_atv'){
   
    $unpublishStatus = getSurveyAssessmentDeleteAccess($catalog_details->id);
      if(!$unpublishStatus){
      	if($catalog_details->sumedit	> 0  || $GLOBALS["user"]->uid == 1){
      	?>
		<a id="publish-unpublish-<?php print $catalogId;?>" class="crs-unpublish-btn glossy-btn" onclick='$("#root-admin").data("narrowsearch").publishAndUnpublishSurveyAssessment("<?php print $catalogId; ?>","<?php print $catalog_details->type; ?>",this);;'><?php print t('LBL571');?></a>
   <?php
      	}else{
      		?>
      		<a id="publish-unpublish-<?php print $catalogId;?>" class="crs-pub-enrolled-btn  plainy-btn"><?php print t('LBL571');?></a>
      		<?php 
      	} 
    }else{?>
		<a id="publish-unpublish-<?php print $catalogId;?>" class="crs-pub-enrolled-btn plainy-btn"><?php print t('Published');?></a>
  <?php }
   }else{
   	$surAssessValid = getIfSurveyAssessmentValid($catalog_details->id);    	
    if($surAssessValid == 0){?>
  		<a id="publish-unpublish-<?php print $catalogId;?>" class="crs-pub-enrolled-btn plainy-btn"><?php print t('LBL650')?></a>
  <?php }else{?>

        <?php 
       	 if($catalog_details->type=='Assessment') {
        	  $maxAssessmentScore = maxScoreValidate($catalog_details->id);
              if($maxAssessmentScore != 0) {
        ?>   
        		<a id="publish-unpublish-<?php print $catalogId;?>" class="crs-pub-enrolled-btn plainy-btn"><?php print t('LBL570');?></a>   
           <?php }else{
           	if($catalog_details->sumedit	> 0  || $GLOBALS["user"]->uid == 1){
           	?>
        		<a id="publish-unpublish-<?php print $catalogId;?>" class="crs-publish-btn glossy-btn" onclick='$("#root-admin").data("narrowsearch").publishAndUnpublishSurveyAssessment("<?php print $catalogId; ?>","<?php print $catalog_details->type; ?>",this);'><?php print t('LBL570');?></a>
        		
        		
           <?php } else{
           	?>
						<a id="publish-unpublish-<?php print $catalogId;?>" class="crs-pub-enrolled-btn plainy-btn"><?php print t('LBL570');?></a>           	
           	<?php 
           }
           
           }?>
       
        <?php } else{ 
        		if($catalog_details->sumedit	> 0  || $GLOBALS["user"]->uid == 1){
        	?>
    		<a id="publish-unpublish-<?php print $catalogId;?>" class="crs-publish-btn glossy-btn" onclick='$("#root-admin").data("narrowsearch").publishAndUnpublishSurveyAssessment("<?php print $catalogId; ?>","<?php print $catalog_details->type; ?>",this);'><?php print t('LBL570');?></a>
      	<?php }
        else{
        	?>
        <a id="publish-unpublish-<?php print $catalogId;?>" class="crs-pub-enrolled-btn plainy-btn" ><?php print t('LBL570');?></a>	
        	<?php 
        } 
        }
        ?>
  
  <?php 	}        
  }  
  
}else if(($catalog_details->object_type=='bannertype')){
    if($catalog_details->status_code=='cbn_anm_sts_atv'){
    	if($catalog_details->sumedit	> 0  || $GLOBALS["user"]->uid == 1){       
    ?>
	<a id="publish-unpublish-<?php print $catalogId;?>" class="crs-unpublish-btn glossy-btn" onclick='$("#root-admin").data("narrowsearch").publishAndUnpublishBanner("<?php print $catalogId; ?>","<?php print $catalog_details->object_type; ?>",this);'><?php print t('LBL571');?></a>
    <?php 
    	}else{
    		?>
    		<a id="publish-unpublish-<?php print $catalogId;?>" class="crs-pub-enrolled-btn glossy-btn"><?php print t('LBL571');?></a>
    		<?php     		
    	}
  }else{
   /*-- #38539: UI issue home page - fix. --*/
  	if(($catalog_details->sumedit	> 0  || $GLOBALS["user"]->uid == 1) && $catalog_details->image_avilable == true ){
  	?>
    <a id="publish-unpublish-<?php print $catalogId;?>" class="crs-publish-btn glossy-btn" onclick='$("#root-admin").data("narrowsearch").publishAndUnpublishBanner("<?php print $catalogId; ?>","<?php print $catalog_details->object_type; ?>",this);'><?php print t('LBL570');?></a>
  <?php         
  	}else{
  		?>
  		<a id="publish-unpublish-<?php print $catalogId;?>" class="crs-pub-enrolled-btn glossy-btn"><?php print t('LBL623') . ' ' .t('MSG438'); ?></a>
  		<?php 
  	}
  }  
}else if(($catalog_details->object_type=='announcement')){
	
    if($catalog_details->status_code=='cre_sys_obt_not_atv'){     
    	if($catalog_details->sumedit	> 0  || $GLOBALS["user"]->uid == 1){  
    ?>
	<a id="publish-unpublish-<?php print $catalogId;?>" class="crs-unpublish-btn glossy-btn" onclick='$("#root-admin").data("narrowsearch").publishAndUnpublishAnnouncement("<?php print $catalogId; ?>","<?php print $catalog_details->object_type; ?>",this);'><?php print t('LBL571');?></a>
	    <?php
    	}else{
    		?>
    		<a id="publish-unpublish-<?php print $catalogId;?>" class="crs-pub-enrolled-btn glossy-btn");?><?php print t('LBL571');?></a>		
    		<?php 
    	} 
  }else{
  	if($catalog_details->sumedit	> 0  || $GLOBALS["user"]->uid == 1){
  	?>
  	<a id="publish-unpublish-<?php print $catalogId;?>" class="crs-publish-btn glossy-btn" onclick='$("#root-admin").data("narrowsearch").publishAndUnpublishAnnouncement("<?php print $catalogId; ?>","<?php print $catalog_details->object_type; ?>",this);'><?php print t('LBL570');?></a>
  <?php
  	}else{
  		?>
  		    <a id="publish-unpublish-<?php print $catalogId;?>" class="crs-pub-enrolled-btn plainy-btn"><?php print t('LBL570');?></a>
  		<?php 
  	}         
  }  
}else if($catalog_details->object_type=='SurveyAssessmentQuestions'){
  if($catalog_details->status_code=='sry_qtn_sts_atv'){
   $unpublishStatus = getQuestionEnrolledDeleteAccess($catalog_details->id);
      if(!$unpublishStatus){ 
      	if($catalog_details->sumedit	> 0  || $GLOBALS["user"]->uid == 1){
      	?>
		<a id="publish-unpublish-<?php print $catalogId;?>" class="crs-unpublish-btn glossy-btn" onclick='$("#root-admin").data("narrowsearch").publishAndUnpublishSurveyAssessmentQuestions("<?php print $catalogId; ?>",this);'><?php print t('LBL571');?></a>
   <?php 
      	}else{
      		?>
      <a id="publish-unpublish-<?php print $catalogId;?>" class="crs-pub-enrolled-btn plainy-btn"><?php print t('LBL571');?></a>		
      		<?php 
      	}
    }else{?>
		<a id="publish-unpublish-<?php print $catalogId;?>" class="crs-pub-enrolled-btn plainy-btn"><?php print t('Published');?></a>
  <?php }
    }else{
    	if($catalog_details->sumedit	> 0  || $GLOBALS["user"]->uid == 1){
    	?>
  	<a id="publish-unpublish-<?php print $catalogId;?>" class="crs-publish-btn glossy-btn" onclick='$("#root-admin").data("narrowsearch").publishAndUnpublishSurveyAssessmentQuestions("<?php print $catalogId; ?>",this);'><?php print t('LBL570');?></a>
  <?php 
    	}else{
    		?>
    <a id="publish-unpublish-<?php print $catalogId;?>" class="crs-pub-enrolled-btn plainy-btn"><?php print t('LBL570');?></a>		
    		<?php 
    	}        
  }
}else if(($catalog_details->object_type=='Content') && ($record[0]->subtype != 'h5p-video-mp4' && $record[0]->subtype != 'h5p-video-webm' && $record[0]->subtype != 'h5p-Youtube'  && $record[0]->subtype != 'h5p-Vimeo'  && $record[0]->subtype != 'h5p-presentatn')){
  if($catalog_details->status_code=='lrn_cnt_sts_atv'){
    $unpublishStatus = getContentDeleteAccess($catalog_details->id);
      if(!$unpublishStatus){
      	if($catalog_details->sumedit	> 0  || $GLOBALS["user"]->uid == 1){
      	?>
		<a id="publish-unpublish-<?php print $catalogId;?>" class="crs-unpublish-btn glossy-btn" onclick='$("#root-admin").data("narrowsearch").publishAndUnpublishContent("<?php print $catalogId; ?>",this);'><?php print t('LBL571');?></a>
   <?php 
      	}else{
      		?>
<a id="publish-unpublish-<?php print $catalogId;?>" class="crs-pub-enrolled-btn plainy-btn"><?php print t('LBL571');?></a>      		
      		<?php 
      	}
    }else{?>
		<a id="publish-unpublish-<?php print $catalogId;?>" class="crs-pub-enrolled-btn plainy-btn"><?php print t('Published');?></a>
  <?php }
     
  }else{
  	if($catalog_details->sumedit	> 0  || $GLOBALS["user"]->uid == 1){
  	?>
    <a id="publish-unpublish-<?php print $catalogId;?>" class="crs-publish-btn glossy-btn" onclick='$("#root-admin").data("narrowsearch").publishAndUnpublishContent("<?php print $catalogId; ?>",this);'><?php print t('LBL570');?></a>
  <?php         
  	}else{
  		?>
   <a id="publish-unpublish-<?php print $catalogId;?>" class="crs-pub-enrolled-btn plainy-btn"><?php print t('LBL570');?></a>  		
  		<?php 
  	}
  }
}else if(($catalog_details->object_type=='Content Author') && ($catalog_details->type =='video') ){
  if($catalog_details->status_code=='lrn_cnt_sts_atv'){
    $unpublishStatus = getContentDeleteAccess($catalog_details->id);
      if(!$unpublishStatus){
      	if($catalog_details->sumedit	> 0  || $GLOBALS["user"]->uid == 1){
      	?>
		<a id="publish-unpublish-<?php print $catalogId;?>" class="crs-unpublish-btn glossy-btn" onclick='$("#root-admin").data("narrowsearch").publishAndUnpublishVideo("<?php print $catalogId; ?>",this);'><?php print t('LBL571');?></a>
   <?php 
      	}else{
      		?>
<a id="publish-unpublish-<?php print $catalogId;?>" class="crs-pub-enrolled-btn plainy-btn"><?php print t('LBL571');?></a>      		
      		<?php 
      	}
    }else{?>
		<a id="publish-unpublish-<?php print $catalogId;?>" class="crs-pub-enrolled-btn plainy-btn"><?php print t('Published');?></a>
  <?php }
     
  }else{
  	if($catalog_details->sumedit	> 0  || $GLOBALS["user"]->uid == 1){
  	?>
    <a id="publish-unpublish-<?php print $catalogId;?>" class="crs-publish-btn glossy-btn" onclick='$("#root-admin").data("narrowsearch").publishAndUnpublishVideo("<?php print $catalogId; ?>",this);'><?php print t('LBL570');?></a>
  <?php         
  	}else{
  		?>
   <a id="publish-unpublish-<?php print $catalogId;?>" class="crs-pub-enrolled-btn plainy-btn"><?php print t('LBL570');?></a>  		
  		<?php 
  	}
  }
}else if(($catalog_details->object_type=='Content Author') && ($catalog_details->type=='presentation') ){
  if($catalog_details->status_code=='lrn_cnt_sts_atv'){
    $unpublishStatus = getContentDeleteAccess($catalog_details->id);
      if(!$unpublishStatus){
      	if($catalog_details->sumedit	> 0  || $GLOBALS["user"]->uid == 1){
      	?>
		<a id="publish-unpublish-<?php print $catalogId;?>" class="crs-unpublish-btn glossy-btn" onclick='$("#root-admin").data("narrowsearch").publishAndUnpublishPresentation("<?php print $catalogId; ?>",this);'><?php print t('LBL571');?></a>
   <?php 
      	}else{
      		?>
<a id="publish-unpublish-<?php print $catalogId;?>" class="crs-pub-enrolled-btn plainy-btn"><?php print t('LBL571');?></a>      		
      		<?php 
      	}
    }else{?>
		<a id="publish-unpublish-<?php print $catalogId;?>" class="crs-pub-enrolled-btn plainy-btn"><?php print t('Published');?></a>
  <?php }
     
  }else{
  	if($catalog_details->sumedit	> 0  || $GLOBALS["user"]->uid == 1){
  	?>
    <a id="publish-unpublish-<?php print $catalogId;?>" class="crs-publish-btn glossy-btn" onclick='$("#root-admin").data("narrowsearch").publishAndUnpublishPresentation("<?php print $catalogId; ?>",this);'><?php print t('LBL570');?></a>
  <?php         
  	}else{
  		?>
   <a id="publish-unpublish-<?php print $catalogId;?>" class="crs-pub-enrolled-btn plainy-btn"><?php print t('LBL570');?></a>  		
  		<?php 
  	}
  }
}else if($catalog_details->object_type=='location'){
  if($catalog_details->location_status=='lrn_res_loc_atv'){
    $unpublishStatus = getLocationSuspendAccess($catalog_details->id);
      if(!$unpublishStatus && ($catalog_details->sumedit > 0  || $GLOBALS["user"]->uid == 1 )){ ?>
		<a id="publish-unpublish-<?php print $catalogId;?>" class="crs-unpublish-btn glossy-btn" onclick='$("#root-admin").data("narrowsearch").publishAndUnpublishLocation("<?php print $catalogId; ?>",this);'><?php print t('LBL572');?></a>
   <?php 
    }else{?>
		  <a id="publish-unpublish-<?php print $catalogId;?>" class="crs-pub-enrolled-btn plainy-btn"><?php print t('LBL572');?></a> 
  <?php  }
     
  }else{
  		if($catalog_details->sumedit > 0  || $GLOBALS["user"]->uid == 1){ ?>
    		<a id="publish-unpublish-<?php print $catalogId;?>" class="crs-publish-btn glossy-btn" onclick='$("#root-admin").data("narrowsearch").publishAndUnpublishLocation("<?php print $catalogId; ?>",this);'><?php print t('LBL573');?></a>
  		<?php 	
  		}else{ ?>
  			<a id="publish-unpublish-<?php print $catalogId;?>" class="crs-pub-enrolled-btn plainy-btn"><?php print t('LBL573');?></a>	
  		<?php }       
  }
}//#custom_attribute_0078975
else if($catalog_details->object_type=='customattribute' && module_exists('exp_sp_administration_customattribute')){

    if($catalog_details->status=='cre_cattr_sts_atv'){

        //$unpublishStatus = getCustomAttributeSuspendAccess($catalog_details->id);
        //expDebug::dPrint('$fdvfd= ' . print_r($GLOBALS["user"]->uid, true),4);
        if($catalog_details->sumedit > 0  || $GLOBALS["user"]->uid == 1 ){ ?>

     <!-- if(!$unpublishStatus && ($catalog_details->sumedit > 0  || $GLOBALS["user"]->uid == 1 )){ ?>-->
      <a id="publish-unpublish-<?php print $catalogId;?>" class="crs-unpublish-btn glossy-btn" onclick='$("#root-admin").data("narrowsearch").publishAndUnpublishCustom("<?php print $catalogId; ?>",this);'><?php print t('LBL920');?></a>
  <?php 
  }else{?>
          <a id="publish-unpublish-<?php print $catalogId;?>" class="crs-pub-enrolled-btn plainy-btn"><?php print t('LBL920');?></a> 
    <?php 
    } 
     }else{
        if($catalog_details->sumedit > 0  || $GLOBALS["user"]->uid == 1){ ?>
            <a id="publish-unpublish-<?php print $catalogId;?>" class="crs-publish-btn glossy-btn" onclick='$("#root-admin").data("narrowsearch").publishAndUnpublishCustom("<?php print $catalogId; ?>",this);'><?php print t('LBL919');?></a>
        
        <?php   
        }else{ ?>
            <a id="publish-unpublish-<?php print $catalogId;?>" class="crs-pub-enrolled-btn plainy-btn"><?php print t('LBL919');?></a>  
        <?php }       
  }
}else if($catalog_details->object_type=='notification_template' || $catalog_details->object_type=='certificate'){
    $languageList = getAvailableTemplateNotification($catalog_details->id);
    if($catalog_details->status_code=='cre_ntn_sts_atv'){
    	if($catalog_details->sumedit	> 0  || $GLOBALS["user"]->uid == 1){
			if($catalog_details->code=='share_notification_class_tp' || $catalog_details->code=='share_notification_catalog'){
    ?>
	<a id="publish-unpublish-<?php print $catalogId;?>" class="crs-pub-enrolled-btn plainy-btn"><?php print t('LBL920');?></a>
			<?php } else { ?>
	<a id="publish-unpublish-<?php print $catalogId;?>" class="crs-unpublish-btn glossy-btn" onclick='$("#root-admin").data("narrowsearch").publishAndUnpublishNotification("<?php print $catalogId; ?>","<?php print $catalog_details->object_type; ?>",this);'><?php print t('LBL920');?></a>
   <?php 
    	}
		}else{
    		?>
  <a id="publish-unpublish-<?php print $catalogId;?>" class="crs-pub-enrolled-btn plainy-btn"><?php print t('LBL920');?></a>  		
    		<?php 
    	}
    	}else{
    if(count($languageList) == 0){ ?>
  		<a id="publish-unpublish-<?php print $catalogId;?>" class="crs-unpublish-btn plainy-btn"><?php print t('LBL976')?></a>
  	<?php }else{
  	
  		if($catalog_details->sumedit	> 0  || $GLOBALS["user"]->uid == 1 || $catalog_details->object_type=='certificate'){
			if($catalog_details->code=='share_notification_class_tp' || $catalog_details->code=='share_notification_catalog'){
  		?>
		<a id="publish-unpublish-<?php print $catalogId;?>" class="crs-pub-enrolled-btn plainy-btn"><?php print t('LBL919');?></a>
			<?php } else { ?>
  		<a id="publish-unpublish-<?php print $catalogId;?>" class="crs-publish-btn glossy-btn" onclick='$("#root-admin").data("narrowsearch").publishAndUnpublishNotification("<?php print $catalogId; ?>","<?php print $catalog_details->object_type; ?>",this);'><?php print t('LBL919');?></a>
    <?php }
  		}else{
  			?>
  		<a id="publish-unpublish-<?php print $catalogId;?>" class="crs-pub-enrolled-btn plainy-btn"><?php print t('LBL919');?></a>	
  			<?php 
  		}
  		}
  }
}else if($catalog_details->object_type == 'Tax'){ 
  if($catalog_details->status_code == 'cme_tax_sts_atv'){
    ?>
		<a id="publish-unpublish-<?php print $catalogId;?>" class="crs-unpublish-btn glossy-btn" onclick='$("#root-admin").data("narrowsearch").publishAndUnpublishTax("<?php print $catalogId; ?>",this);'><?php print t('LBL572');?></a>
  <?php    
  }else{?>
    <a id="publish-unpublish-<?php print $catalogId;?>" class="crs-publish-btn glossy-btn" onclick='$("#root-admin").data("narrowsearch").publishAndUnpublishTax("<?php print $catalogId; ?>",this);'><?php print t('LBL573');?></a>
  <?php         
  }
}else if($catalog_details->object_type == 'Discount'){
	if($catalog_details->dis_status){
		if($catalog_details->sumedit > 0  || $GLOBALS["user"]->uid == 1) {
			?>
			<a id="publish-unpublish-<?php print $catalogId;?>" class="crs-unpublish-btn glossy-btn" onclick='$("#root-admin").data("narrowsearch").publishAndUnpublishDiscount("<?php print $catalogId; ?>",this);'><?php print t('LBL572');?></a>
			<?php } else { ?>
			<a id="publish-unpublish-<?php print $catalogId;?>" class="crs-pub-enrolled-btn plainy-btn"><?php print t('LBL572');?></a> 
			<?php } ?>
	  <?php    
	  }else{ ?>
	  <?php $isAppAllTrng = getDiscountApplicableAllTrainings($catalogId);
	    	$isAppAllUsers     = getDiscountApplicableAllUsers($catalogId);
	    	$isAppTrng = getDiscountApplicabletotrainings($catalogId);
	    	$isApptoUsers = getDiscountApplicablegroups($catalogId);
	    	$allowPublishorNot = (($isAppAllTrng==0 && !$isAppTrng) || ($isAppAllUsers==0 && !$isApptoUsers)) ? 1 : 0;
	     	if($catalog_details->sumedit > 0  || $GLOBALS["user"]->uid == 1) {
	    	?>
	    <a id="publish-unpublish-<?php print $catalogId;?>" class="crs-publish-btn glossy-btn" onclick='$("#root-admin").data("narrowsearch").publishAndUnpublishDiscount("<?php print $catalogId; ?>",this,"<?php print $allowPublishorNot?>");'><?php print t('LBL573');?></a>
	    <?php } else { ?>
	    <a id="publish-unpublish-<?php print $catalogId;?>" class="crs-pub-enrolled-btn plainy-btn"><?php print t('LBL573');?></a> 
	    <?php } ?>
	  <?php         
	  }		
}else if($catalog_details->object_type == 'moduleinfo'){
  if((!empty($catalog_details->enable_option)) && ($catalog_details->enable_option =='No')){
    if($catalog_details->status == 1){
      ?>
  		<a id="publish-unpublish-<?php print $catalog_details->expertus_module_name;?>" class="crs-pub-enrolled-btn plainy-btn" onclick='return false;'><?php print t('LBL921');?></a>
    <?php    
    }else{?>
      <a id="publish-unpublish-<?php print $catalog_details->expertus_module_name;?>" class="crs-pub-enrolled-btn plainy-btn" onclick='return false;'><?php print t('LBL922');?></a>
    <?php         
    }    
  }
  else{
    if($catalog_details->status == 1){
    	if(($catalog_details->expertus_module_name == "exp_sp_administration_contentauthor") && ($catalog_details->published_content == 1))
    	 {
    ?>
           <a id="publish-unpublish-<?php print $catalog_details->expertus_module_name;?>" class="crs-unpublish-btn glossy-btn" onclick='$("#root-admin").data("narrowsearch").displayDeleteWizard("<?php print t('MSG866'); ?>","cre_ste_mod_aut", "exp_sp_administration_contentauthor",300);'><?php print t('LBL920');?></a>
			<?php  	  
       }
        else {
     ?>
		<a id="publish-unpublish-<?php print $catalog_details->expertus_module_name;?>" class="crs-unpublish-btn glossy-btn" onclick='$("#root-admin").data("narrowsearch").publishAndUnpublishModule("<?php print $catalog_details->expertus_module_name; ?>",this);'><?php print t('LBL920');?></a>
  <?php 
        }   
  }else{
    if($catalog_details->expertus_module_name == "exp_sp_administration_customattribute")
        {
        ?>
                <a id="publish-unpublish-<?php print $catalog_details->expertus_module_name;?>" class="crs-unpublish-btn glossy-btn" onclick='$("#root-admin").data("narrowsearch").displayDeleteWizard("<?php print t('MSG1109'); ?>","cre_ste_mod_cattr", "exp_sp_administration_customattribute",300);'><?php print t('LBL919');?></a>
                <?php     
        }
    else if($catalog_details->greyout == false){
       ?>
    <a id="publish-unpublish-<?php print $catalog_details->expertus_module_name;?>" class="crs-publish-btn glossy-btn" onclick='$("#root-admin").data("narrowsearch").publishAndUnpublishModule("<?php print $catalog_details->expertus_module_name; ?>",this);'><?php print t('LBL919');?></a>
  <?php         
  }else{?>
  <a id="publish-unpublish-<?php print $catalog_details->expertus_module_name;?>" class="crs-pub-enrolled-btn plainy-btn" onclick='return false;'><?php print t('LBL919');?></a>
  <?php 
  }	
 }
}
}else if($catalog_details->object_type == 'paymentmethod'){ 
  if($catalog_details->status_code == '1'){
    ?>
		<a id="publish-unpublish-<?php print $catalog_details->id;?>" class="crs-unpublish-btn glossy-btn" onclick='$("#root-admin").data("narrowsearch").publishAndUnpublishPayment("<?php print  $catalog_details->id; ?>",this);'><?php print t('LBL920');?></a>
  <?php    
  }else{?>
    <a id="publish-unpublish-<?php print $catalog_details->id;?>" class="crs-publish-btn glossy-btn" onclick='$("#root-admin").data("narrowsearch").publishAndUnpublishPayment("<?php print  $catalog_details->id; ?>",this);'><?php print t('LBL919');?></a>
  <?php         
  }
}else if($catalog_details->object_type == 'Order'){ 
  if($catalog_details->order_status != 'in_checkout' && $catalog_details->order_status != 'processing' && $catalog_details->order_status != 'canceled'){
  	$orderId = $catalog_details->uc_order_id;
  	if($catalog_details->order_status == 'pending' ){
  		$paymentReceived = uc_order_status_data('payment_received', 'title');
  	?>
  	  <a id="publish-unpublish-<?php print $catalog_details->uc_order_id;?>" class="crs-unpublish-btn glossy-btn" onclick='$("#root-admin").data("narrowsearch").publishAndUnpublishOrder("<?php print  $catalog_details->uc_order_id; ?>","<?php print  'payment_received'; ?>",this);'><?php print t($paymentReceived);?></a>
  	  <span class="dd-arrow"><span id="pub-unpub-action-btn" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn " onclick='displayNarrowSearchAction("<?php print  $catalog_details->uc_order_id; ?>",this);'> </span></span>
      <ul class="narrowsearch-pub-add-list pub-action-list-<?php print $catalog_details->uc_order_id;?>"><li class="save-pub-unpub-sub-menu"><input type="submit" class="form-submit" value="<?php print t('LBL109'); ?>" id="" onclick='$("#root-admin").data("narrowsearch").publishAndUnpublishOrder("<?php print  $catalog_details->uc_order_id; ?>","<?php print  'canceled'; ?>",this);'></li></ul>        
  	<?php 
  	}
  	else{
  		$paymentCanceled = t('LBL109');
  		$isNotCancellable = getCancelStatusFromSltOrderId($catalog_details->slt_order_id);
  		if($isNotCancellable){
  			?>
      <a id="publish-unpublish-<?php print $catalog_details->uc_order_id;?>" class="crs-pub-enrolled-btn plainy-btn" onclick='return false;'><?php print $paymentCanceled;?></a>
  		<?php 
  		}
  		else{  		
  	?>
  	  <a id="publish-unpublish-<?php print $catalog_details->uc_order_id;?>" class="crs-unpublish-btn glossy-btn cancel" onclick='$("#root-admin").data("narrowsearch").publishAndUnpublishOrder("<?php print  $catalog_details->uc_order_id; ?>","<?php print  'canceled'; ?>",this);'><?php print $paymentCanceled;?></a>
  <?php 
  		}
  	}  
  }else{ ?>
      <a id="publish-unpublish-<?php print $catalog_details->uc_order_id;?>" class="crs-pub-enrolled-btn plainy-btn" onclick='return false;'><?php print t('Canceled');?></a>
  <?php         
  }
  
}
?>
</div>
