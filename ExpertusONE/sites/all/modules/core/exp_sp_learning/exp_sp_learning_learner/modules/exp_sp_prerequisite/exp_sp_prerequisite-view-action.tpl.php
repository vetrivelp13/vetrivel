<?php 
  
  $results->prereq_enrolledid = getRegisteredCourseCheck($results->crs_id);  
  //print_r($results);
  if($results->prereq_enrolledid == 0) {
    print commonRegisterHtml($results,false);
    ?>    
  <?php } else { ?>
  
  		<div class="top-record-div">
            <table class="search-register-btn" border="0">
            	<tr>
            		<td class="action-btn-disable"
            			id="registerClsPreReq_<?php print $results->cls_id; ?>"><?php print t('Completed'); ?>
            		</td>
            	</tr>
            </table>
        </div>
                  
  <?php } ?>
  

  <!--  print getActionButton($results);  -->