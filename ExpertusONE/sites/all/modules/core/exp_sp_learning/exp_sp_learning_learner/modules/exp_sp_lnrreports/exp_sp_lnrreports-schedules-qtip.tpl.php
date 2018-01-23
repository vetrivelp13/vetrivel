<?php
  expDebug::dPrint('exp_sp_lnrreports-schedules-qtip.tpl.php: $popup_id 1234555---> = ' . $popup_id, 4);
  
  expDebug::dPrint('$commands $popup_id= ' . print_r($popup_id, true), 4);
  expDebug::dPrint('$commands $popup_width = ' . print_r($popup_width, true), 4);
  expDebug::dPrint('$commands $display_html= ' . print_r($display_html, true), 4);
  
?>

<div id="schedule-qtip-popup">
	<div id="<?php print $popup_id;?>" class="qtip-box active-qtip-div <?php print $class;?>" style="display:none">
	  <a class="qtip-close-button" onclick="$('.active-qtip-div').remove();calRefreshAftClose();"></a>
	  <span id="bubble-wrapper-<?php print $popup_id;?>">
	    <table cellspacing="0" cellpadding="0" width="<?php print $popup_width;?>" height="auto" id="bubble-face-table">
	      <tbody>
	        <tr>
	          <td class="bubble-tl"></td>
	          <td class="bubble-t"></td>
	          <td class="bubble-tr"></td>
	        </tr>
	        <tr>
	          <td class="bubble-cl"></td>
	          <td valign="top" class="bubble-c" style="<?php print (!empty($popup_height)?  'height:'.$popup_height.'px;' :  ''); ?>">   
	          <div id="bubble-content-<?php print $popup_id;?>">
	            <div id="show_expertus_message"></div>
	             <?php print $display_html;?>
	          </div>
	          </td>
	          <td class="bubble-cr"></td>
	        </tr>
	        <tr>
	          <td class="bubble-bl"></td>
	          <td class="bubble-blt"></td>
	          <td class="bubble-br"></td>
	        </tr>
	      </tbody>
	    </table>
	  </span>
	</div>
</div>