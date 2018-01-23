<?php
// $Id: box.tpl.php,v 1.5 2011-03-08 11:09:33 sulthanabdulkadarn Exp $

/**
 * @file box.tpl.php
 *
 * Theme implementation to display a box.
 *
 * Available variables:
 * - $title: Box title.
 * - $content: Box content.
 *
 * @see template_preprocess()
 */
?>
<div class="box"><div class="box-inner">
  <script type='text/javascript'>
  	if(!document.getElementById('replyTopic'))
  	{
  	   	document.write("<h4 class='pgTitle'>Start a New Topic</h4>");
  	}
	</script>	  
  <?php print $content; ?> 
</div></div> <!-- /box-inner, /box -->
