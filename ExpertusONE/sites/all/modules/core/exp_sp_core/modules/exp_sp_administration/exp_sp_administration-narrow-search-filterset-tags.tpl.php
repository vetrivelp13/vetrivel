<?php global $theme_key; ?>
<?php expDebug::dPrint('narrow search filterset a tpl file : $default_text = ' . print_r($default_text, true),4);?>
<?php expDebug::dPrint('narrow search filterset a tpl file : $tags_for_cloud = ' . print_r($tags_for_cloud, true), 4);?>
<div id='<?php print $html_id; ?>' class='narrow-search-filterset '>
  <ul>
	  <li>
	    <span id='<?php print $code; ?>_filterset_title' class='cls-show ' onclick='$("<?php print '#' . $js_object_info['init_id']; ?>").data("<?php print $js_object_info['name']; ?>").showHide("<?php print $code; ?>_filterset_title", "<?php print $code; ?>_filterset");'>
	      <a class="narrow-search-filterset-heading"><?php print $title;?></a>
		  </span>
		  <span id='<?php print $code; ?>-tagscloud-clr' class='clr-txt' style='display: none'
		           onclick='$("<?php print '#' . $js_object_info['init_id']; ?>").data("<?php print $js_object_info['name']; ?>").clearTagsCloudNarrowFilter("<?php print $code; ?>");'><?php print t("LBL307"); ?></span>
  	</li>
  </ul>
  <div id='<?php print $code; ?>_filterset'>
    <div class='tags-cloud-container-cls'>
    <input type='text' id='<?php print $code;?>-addltext-filter' value='' style='display:none;'/>
    <input type='text' id='tagentityType' value='tagtip' style='display:none;'/>
    <div id='<?php print $code; ?>_tagscloud' class='taglink tags_cloud'>
      <p id="tagcloudcontainerid" class="<?php print $code; ?>_cloudtagsp">
      <?php $i = 1; 
            $char = array('1'=>18, '2'=> 16,'3'=>13, '4'=> 12,'5'=>9,'6'=>8,'7'=>7,'8'=>6);
            foreach ($tags_for_cloud as $tagName => $weight): 
                    
                    if($i <= 7){
                    	if($code != 'catalogclasstag')
                    		$existingTagName[] = $tagName;
                    	else
                    		$existingClsTagName[] = $tagName;
                    ?>
        <span class='tagscloud-term'>
          <a class="tagscloud-tag level<?php print $weight;?>" href="javascript:void(0);"
               onclick="$('#root-admin').data('narrowsearch').underlineTagAndTriggerSearch('<?php print rawurlencode($tagName); ?>', '<?php print $code; ?>', this);"><?php (count($tags_for_cloud) > 7) ? print titleController('FADEOUT', $tagName,$char[$weight]) : print $tagName; ?></a>
        </span>
      <?php } $i++; endforeach;?>
      </p>
      <?php if($code != 'catalogclasstag'){?>
      <div id='tagtip'>
      <div id= 'inner-tag-tip'>
                    
			</div>
                   <div id= 'inner-tag-tip-dup' style='display: none;'> 
                    <?php  $cnt =0;
                    foreach ($tags_for_cloud as $tagName => $weight): 
                    $cnt++;?>
                    	<span class='tagscloud-term'>
          							<a class="tagscloud-tag level<?php print $weight;?>" href="javascript:void(0);"
               					onclick="$('#root-admin').data('narrowsearch').underlineTagCloudAndTriggerSearch('<?php print rawurlencode($tagName); ?>', '<?php print $code; ?>', this, <?php print $weight; ?>);"><?php print $tagName; ?></a>
        							</span>
                    <?php endforeach; ?>
                    <input type='text' id='tagCommonEnt' value='<?php echo empty($cnt)? 0 : $cnt;?>' style='display:none;'/>
                    </div>
                    </div>
<?php }else if($code == 'catalogclasstag'){ ?>
			<div id='tagtipclass'>
			<div id= 'inner-tag-tip-cls'>
                    
      </div>
      <div id= 'inner-tag-tip-dup-cls' style='display: none;'>
                    <?php  $cntCls =0;
                    foreach ($tags_for_cloud as $tagName => $weight): 
                    $cntCls++;?>
                    	<span class='tagscloud-term'>
          							<a class="tagscloud-tag level<?php print $weight;?>" href="javascript:void(0);"
               					onclick="$('#root-admin').data('narrowsearch').underlineTagCloudAndTriggerSearch('<?php print rawurlencode($tagName); ?>', '<?php print $code; ?>', this, <?php print $weight; ?>);"><?php print $tagName; ?></a>
        							</span>
                    <?php endforeach; ?>
                    <input type='text' id='tagClsEnt' value='<?php echo empty($cntCls) ? 0 : $cntCls;?>' style='display:none;'/>
                    </div>
                    </div>
                    <?php } ?>
		</div>
		</div>
  </div>
</div>


<script language="Javascript" type="text/javascript">

var entityType = <?php echo '"' . $code . '"'?>;
var existtagArrAdmin  = (entityType == 'catalogclasstag') ? existtagArrAdmin : <?php echo '["' . implode('", "', array_map('escape_string', $existingTagName)) . '"]' ?>;
var existtagClsArrAdmin  = <?php echo '["' . implode('", "', $existingClsTagName) . '"]' ?>;
var weightArrAdmin  = <?php echo '["' . implode('", "', $char) . '"]' ?>;

$(function(){
	
   $('.taglink').hover(
   		function(){
   			try{
   			var tagval = $('#tagentityType').val();
   			if(tagval == 'tagtipclass'){
   	   			var tagCount = $('#tagClsEnt').val();
   			}else{
   				var tagCount = $('#tagCommonEnt').val();
   			}
           	if(tagCount > 7){
           	if(tagCount < 15 && tagCount > 7){
           		$('#'+tagval).width(250).height(250).css('left', '-39px'); 
            }else if(tagCount < 25){
            	$('#'+tagval).width(360).height(300).css('left', '-90px');
            }else {
            	$('#'+tagval).width(480).height(390).css('left', '-60px');
                }
           	$('#'+tagval).fadeIn();
           	if(tagval == 'tagtipclass'){
           		$('#inner-tag-tip-cls').html('');
           	$('#inner-tag-tip-dup-cls .tagscloud-term').each(function() {
            	$(this).clone().appendTo('#inner-tag-tip-cls');
                var innerTagCls = parseInt($('#inner-tag-tip-cls').attr('scrollHeight'));
                var tagTipDivCls = parseInt($('#tagtipclass').attr('clientHeight')) - 10;
                if(tagTipDivCls <= innerTagCls) {
                	$('#inner-tag-tip-cls .tagscloud-term').last().remove();
                	return false;
                }
            });
           	}else{
           		$('#inner-tag-tip').html('');
           	$('#inner-tag-tip-dup .tagscloud-term').each(function() {
            	$(this).clone().appendTo('#inner-tag-tip');
                var innerTag = parseInt($('#inner-tag-tip').attr('scrollHeight'));
                var tagTipDiv = parseInt($('#'+tagval).attr('clientHeight')) - 10;
                if(tagTipDiv <= innerTag) {
                	$('#inner-tag-tip .tagscloud-term').last().remove();
                	return false;
                }
            });
           	}
           	$('#'+tagval).height('auto');
           	}
   			}catch(e){
        		//console.log(e);
        		}
   		},
   		function(){
   			var tagval = $('#tagentityType').val();
   			$('#'+tagval).hide();
   		}
   		)
})


</script>
