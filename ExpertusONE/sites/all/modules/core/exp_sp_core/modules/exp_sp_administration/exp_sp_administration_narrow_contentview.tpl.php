<?php 
expDebug::dPrint('enter the function sasasas'.print_r($record,true),4);
expDebug::dPrint('enter the function sasasasAAAA'.$record->id,4);
global $theme_key;
?>
<?php 
if($record[0]->subtype == 'h5p-video-mp4' || $record[0]->subtype == 'h5p-video-webm' || $record[0]->subtype == 'h5p-Youtube' || $record[0]->subtype == 'h5p-Vimeo' || $record[0]->subtype == 'h5p-presentatn'){
 
   
   $interactions = $record[0]->interactions;
   $interactions1 = json_decode($interactions,1);
   
   $interactions2 = $record[1]->interactions;
   $interactions3 = json_decode($interactions2,1);
   
   expDebug::dPrint('intrsDetails-->'.print_r($interactions1,true),4);
   expDebug::dPrint('intrsDetailsAbhishek-->'.print_r($interactions3,true),4);
   expDebug::dPrint('myposter>'.print_r($interactions1[poster_image],true),4);
}
    ?>

<?php 
  if($record[0]->type == 'Video on Demand')
  {
      if($record[0]->subtype == 'h5p-video-mp4')
          $record[0]->type = 'Mp4 video' ;
      else if($record[0]->subtype == 'h5p-video-webm')
          $record[0]->type = 'Webm video' ;
      else if($record[0]->subtype == 'h5p-Youtube')
          $record[0]->type = 'Youtube video' ;
          else if($record[0]->subtype == 'h5p-Vimeo')
              $record[0]->type = 'Vimeo video' ;
  }  
  else if(($record[0]->subtype == 'h5p-presentatn') && ($record[0]->type == 'Knowledge Content'))
  {
      $record[0]->type = 'Presentation' ;
  }      
 ?>
<div class="top-record-div-left">
 <div class="left-side-container">
     <div class="para">
      <div class="code-container">
            <?php $type = t("LBL608").''.":"; ?>
            
             <?php if(($record[0]->subtype == 'h5p-video-mp4' || $record[0]->subtype == 'h5p-video-webm' || $record[0]->subtype == 'h5p-Youtube'  || $record[0]->subtype == 'h5p-Vimeo' || $record[0]->subtype == 'h5p-Vimeo' ||  $record[0]->subtype == 'h5p-presentatn')) { ?> 
                <span class="detail-code vtip contentauthor_view_label detail-single-line-cont-view-lbl" title= "<?php print $type?>"><?php print titlecontroller('CONTENT-GROUP-LABEL-VIEW-SCREEN',$type); ?></span>
            <?php
              }
              else 
              {
            ?>
                <span class="detail-code detail-single-line-cont-view-lbl vtip" title= "<?php print $type?>"><?php print titlecontroller('CONTENT-GROUP-LABEL-VIEW-SCREEN',$type); ?></span>
            <?php } ?>  
             <span class="detail-desc detail-single-line vtip" title="<?php print t($record[0]->type);?>"><?php print titlecontroller('CONTENT-GROUP-VALUE-VIEW-SCREEN',t($record[0]->type)); ?></span>                                     
        </div>
     </div>
 </div> 
 
 <?php if($record[0]->subtype == 'h5p-video-mp4' || $record[0]->subtype == 'h5p-video-webm' || $record[0]->subtype == 'h5p-Youtube' || $record[0]->subtype == 'h5p-Vimeo'  || $record[0]->subtype == 'h5p-presentatn') { 
?>
 <div class="para">
  <div class="code-container">
            <span class="detail-code contentauthor_view_label"><?php print t("LBL083"); ?>:</span>
         <span class="detail-desc detail-single-line vtip" style="max-width:500px; min-width:500px;"  title="<?php print htmlentities($record[0]->code); ?>">
         <?php ($theme_key == 'expertusoneV2') ? print titleController('ADMIN-NARROW-CONTENT-AUTHOR-VIEW-CODE', $record[0]->code,200) : print titleController('ADMIN-NARROW-CONTENT-AUTHOR-VIEW-CODE', $record[0]->code,200); ?>                                            
         </span> 
                                            
    </div>
 </div>
 <?php } ?>
 
 
 <?php if($record[0]->subtype != 'h5p-video-mp4' && $record[0]->subtype != 'h5p-video-webm' && $record[0]->subtype != 'h5p-Youtube' && $record[0]->subtype != 'h5p-Vimeo' && $record[0]->subtype != 'h5p-presentatn' ) { ?>
 <div class="right-side-container">
     <div class="para">
      <div class="code-container">
            <span class="detail-code"><?php print t("LBL038"); ?>:</span>
             <span class="detail-desc">
             <?php print t($record[0]->lang); ?>                                                
             </span>                                    
        </div>
     </div>
</div> 
<?php } ?>
 <?php if($record[0]->subtype == 'h5p-video-mp4' || $record[0]->subtype == 'h5p-video-webm' || $record[0]->subtype == 'h5p-Youtube' || $record[0]->subtype == 'h5p-Vimeo' || $record[0]->subtype == 'h5p-presentatn') { 
        
    ?> 
<div class="para">
  <div class="code-container">
        <span class="detail-code contentauthor_view_label"><?php print t("LBL229"); ?>:</span>
         <span id="detail-desc-view" class="detail-desc" style="margin-left: 0px;">
         <?php print $record[0]->description; ?>                                                
         </span>                                    
    </div>
 </div>
<?php  }else{?>
<div class="para">
  <div class="code-container">
        
        <span class="detail-code"><?php print t("LBL229"); ?>:</span>
         <span id="detail-desc-view" class="detail-desc">
         <?php print $record[0]->description; ?>                                                
         </span>                                    
    </div>
 </div>

<?php }?> 


<?php if($record[0]->subtype == 'h5p-video-mp4' || $record[0]->subtype == 'h5p-video-webm' || $record[0]->subtype == 'h5p-Youtube' || $record[0]->subtype == 'h5p-Vimeo') { ?> 
   <div class="para">
   <div class="code-container">
   <span class="detail-code contentauthor_view_label" ><?php print t('Video'); ?>:</span>
         <span title=" <?php
         if($record[0]->subtype == 'h5p-video-mp4' || $record[0]->subtype == 'h5p-video-webm')
         {
          $video_name =    basename($interactions1[interactiveVideo][video][files][0][path]);
          print $video_name;
         }
         else
             print $interactions1[interactiveVideo][video][files][0][path];
         
         ?>" id="detail-desc-view" class="vtip detail-desc content_video_label">
         <?php
         if($record[0]->subtype == 'h5p-video-mp4' || $record[0]->subtype == 'h5p-video-webm')
         {
          $video_name =    basename($interactions1[interactiveVideo][video][files][0][path]);
          print  titleController('NARROW-CONTENTAUTHOR-VIEW-DETAIL-ATTACHMENTS-NAME',$video_name,70);
         }
         else
             print titleController('NARROW-CONTENTAUTHOR-VIEW-DETAIL-ATTACHMENTS-NAME',$interactions1[interactiveVideo][video][files][0][path],70);
         
         ?>                                             
         </span>                                    
    </div>
    </div>
<?php } ?> 

 <?php if($record[0]->subtype != 'h5p-video-mp4' && $record[0]->subtype != 'h5p-video-webm' && $record[0]->subtype != 'h5p-Youtube' && $record[0]->subtype != 'h5p-Vimeo' && $record[0]->subtype != 'h5p-presentatn') { ?>  

 <div class="para">
  <div class="code-container">
            <span class="detail-code"><?php print t("LBL107"); ?>:</span>
         <span class="detail-desc detail-single-line vtip" title="<?php print htmlentities($record[0]->code); ?>">
         <?php ($theme_key == 'expertusoneV2') ? print titleController('ADMIN-NARROW-CONTENT-VIEW-CODE', $record[0]->code,88) : print titleController('ADMIN-NARROW-CONTENT-VIEW-CODE', $record[0]->code,27); ?>                                            
         </span> 
                                            
    </div>
 </div>
 <?php 
    if(module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status
        //Get Custom Attributes for Content View #custom_attribute_0078975
        include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'/exp_sp_administration_customattribute_renderinforms.inc');
        print getCustomAttributesForView($record[0]->id, 'cre_sys_obt_cnt');
    } //#custom_attribute_0078975 - End Check module status
} ?> 
 

   <?php if(($record[0]->subtype == 'h5p-video-mp4' || $record[0]->subtype == 'h5p-video-webm' || $record[0]->subtype == 'h5p-Youtube' || $record[0]->subtype == 'h5p-Vimeo')) { ?> 
   <div class="para">
   <div class="code-container">
   <span class="detail-code contentauthor_view_label"><?php print t('LBL3045'); ?>:</span>
         <span id="detail-desc-view" class="detail-desc">
         <?php 
         print $interactions1[interactiveVideo][video][passingscore];
          ?>                                                
         </span>                                    
    </div>
    </div>
<?php } ?>

   <?php  if (($record[0]->subtype == 'h5p-presentatn') )  { ?> 
   <div class="para">
   <div class="code-container">
   <span class="detail-code contentauthor_view_label"><?php print t('LBL3045'); ?>:</span>
         <span id="detail-desc-view" class="detail-desc">
         <?php 
         print $interactions1[presentation][passingscore]; ?>                                               
         </span>                                    
    </div>
    </div>
<?php } ?>
  
  
  
  <?php if(($record[0]->subtype == 'h5p-video-mp4' || $record[0]->subtype == 'h5p-video-webm' || $record[0]->subtype == 'h5p-Youtube'  || $record[0]->subtype == 'h5p-Vimeo' || $record[0]->subtype == 'h5p-presentatn')) { ?> 
   <div class="para">
   <div class="code-container">
   <span class="detail-code contentauthor_view_label"><?php print t('LBL3025'); ?>:</span>
         <span id="detail-desc-view" class="detail-desc">
         <?php 
         if($interactions1[override][overrideRetry] == 1)
              $ShowRetry = 'Enabled';
         else
            $ShowRetry = 'Disabled';
         print t($ShowRetry); ?>                                                
         </span>                                    
    </div>
    </div>
<?php } ?>


  <?php if(($record[0]->subtype == 'h5p-video-mp4' || $record[0]->subtype == 'h5p-video-webm' || $record[0]->subtype == 'h5p-Youtube'  || $record[0]->subtype == 'h5p-Vimeo' || $record[0]->subtype == 'h5p-presentatn')) { ?> 
   <div class="para">
   <div class="code-container">
   <span class="detail-code contentauthor_view_label" ><?php print t('LBL3024'); ?>:</span>
         <span id="detail-desc-view" class="detail-desc">
         <?php 
         if($interactions1[override][overrideShowSolutionButton] == 1)
              $ShowSol = 'Enabled';
         else
            $ShowSol = 'Disabled';
         print t($ShowSol); ?>                                              
         </span>                                    
    </div>
    </div>
<?php } ?> 
 
<?php $tags  = getCatalogTags($record[0]->id,'Content');
    $tagString  = '';
    if(!empty($tags)){
      $tagString  = implode(", ",$tags);
    }
?>
 <?php if($record[0]->subtype != 'h5p-video-mp4' && $record[0]->subtype != 'h5p-video-webm' && $record[0]->subtype != 'h5p-Youtube' && $record[0]->subtype != 'h5p-Vimeo' && $record[0]->subtype != 'h5p-presentatn') { ?>   
 <div class="para"   >
     <div class="code-container">
       <span class="detail-code"><?php print t("LBL191"); ?>:</span>
       <span id="detail-desc-view" class="detail-desc detail-single-line"><span class="single-line-lbl-val vtip" title="<?php print $tagString; ?>"><?php ($theme_key == 'expertusoneV2')? print titleController('ADMIN-NARROW-CONTENT-VIEW-TAGS',  $tagString,80) : print titleController('ADMIN-NARROW-CONTENT-VIEW-TAGS',  $tagString,70); ?></span>
       </span>
    </div>
 </div>
 <?php  } ?>
  <?php if($record[0]->subtype == 'h5p-video-mp4' || $record[0]->subtype == 'h5p-video-webm' || $record[0]->subtype == 'h5p-Youtube'  || $record[0]->subtype == 'h5p-Vimeo'){
      $entity_type = 'cre_sys_obt_cnt_aut';
      $title_length = 'ADMIN-NARROW-CONTENT-AUTHOR-VIEW-CODE';
      $authorCls = "contentauthor_view_label";
  }
  else if($record[0]->subtype == 'h5p-presentatn'){
      $entity_type = 'cre_sys_obt_cnt_aut_pre';
      $title_length = 'ADMIN-NARROW-CONTENT-AUTHOR-VIEW-CODE';
      $authorCls = "contentauthor_view_label";
  }
  else{
      $entity_type = 'cre_sys_obt_cnt';
      $title_length = 'ADMIN-NARROW-CONTENT-VIEW-ACCESSNAME';
      $authorCls = "";
  }
      
      
  $access  = getGroupNamesForView($record[0]->id, $entity_type);
expDebug::dPrint('$access'.print_r($access,true),4);
  $accessName  = '';
    if(!empty($access)){
      $accessName  = implode(", ",$access);
    }
    ?>

 <div class="para">
    <div class="code-container">
        <span class="detail-code <?php print $authorCls ?> "><?php print t("LBL642"); ?>:</span>
         <span id= "detail-desc-view" class="detail-desc detail-single-line"><span class="single-line-lbl-val vtip" title="<?php print sanitize_data($accessName); ?>"><?php ($theme_key == 'expertusoneV2')? print titleController($title_length,  $accessName,80) :  print titleController($title_length,  $accessName,70); ?></span>
         </span>                                    
    </div>
 </div>
  
  
 <div class="para">
  <?php $getVersions= getVersionDetailsInfoView($record[0]->id);
    expDebug::dPrint('$getVersions-->'.print_r($getVersions,true),4);?>
  <div class="code-container">
     <span class="detail-code"></span>
         <span class="detail-desc">
      <table cellpadding="0" class="class-session-details" cellspacing="0" width="640" border="0" style="margin-bottom: 5px;">
      <?php if($record[0]->subtype != 'h5p-video-mp4' && $record[0]->subtype != 'h5p-video-webm' && $record[0]->subtype != 'h5p-Youtube'  && $record[0]->subtype != 'h5p-Vimeo'  && $record[0]->subtype != 'h5p-presentatn') { ?>        
         <tr>
            <th> <?php print t('LBL952');?></th>
         </tr>
                 <?php  $i=0;
                    foreach($getVersions as $versionInfo) {
                    expDebug::dPrint('$contentName--->'.print_r($versionInfo,true),5);
                  if(($i%2) == 0){
                $class='template-separation';
                }?>
         <tr>
                <td valign="top"><span title="<?php print sanitize_data($versionInfo->version_title); ?>" class="template-names vtip"><?php print titleController(' ', $versionInfo->version_title,100); ?></span></td>
             </tr>
         
             <tr>
                <td class="template-separation" valign="top" ><span class="edit-class-list-left"><?php print t('LBL944').' : '.$versionInfo->created_by.' ';?></span><span class="edit-class-list-left"><?php print ucfirst(t('LBL945')).' '.date_format(date_create($versionInfo->version_updated),'M d, Y')?></span></td>
           </tr>
                 <?php $i++;
                } ?>
            <?php } ?>  
    <!--  <?php if($record[0]->subtype == 'h5p-video-mp4' || $record[0]->subtype == 'h5p-video-webm' || $record[0]->subtype == 'h5p-Youtube'   || $record[0]->subtype == 'h5p-Vimeo' || $record[0]->subtype == 'h5p-presentatn') { ?>
        <td class="template-separation" valign="top" ><span class="edit-class-list-left"><?php print t('LBL944').' : '.$versionInfo->created_by.' ';?></span><span class="edit-class-list-left"><?php print ucfirst(t('LBL945')).' '.date_format(date_create($versionInfo->version_updated),'M d, Y')?></span></td>
        <?php } ?>
    --> 
                
    </table>
   </span>
    </div>
 </div>
</div> 
