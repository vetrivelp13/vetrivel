<?php
// $Id: node-forum.tpl.php,v 1.18 2011-03-18 09:42:02 muthusamys Exp $

/**
 * @file node-og-group-post.tpl.php
 * 
 * Og has added a brief section at bottom for printing links to affiliated groups.
 * This template is used by default for non group nodes.
 *
 * Theme implementation to display a node.
 *
 * Available variables:
 * - $title: the (sanitized) title of the node.
 * - $content: Node body or teaser depending on $teaser flag.
 * - $picture: The authors picture of the node output from
 *   theme_user_picture().
 * - $date: Formatted creation date (use $created to reformat with
 *   format_date()).
 * - $links: Themed links like "Read more", "Add new comment", etc. output
 *   from theme_links().
 * - $name: Themed username of node author output from theme_user().
 * - $node_url: Direct url of the current node.
 * - $terms: the themed list of taxonomy term links output from theme_links().
 * - $submitted: themed submission information output from
 *   theme_node_submitted().
 *
 * Other variables:
 * - $node: Full node object. Contains data that may not be safe.
 * - $type: Node type, i.e. story, page, blog, etc.
 * - $comment_count: Number of comments attached to the node.
 * - $uid: User ID of the node author.
 * - $created: Time the node was published formatted in Unix timestamp.
 * - $zebra: Outputs either "even" or "odd". Useful for zebra striping in
 *   teaser listings.
 * - $id: Position of the node. Increments each time it's output.
 *
 * Node status variables:
 * - $teaser: Flag for the teaser state.
 * - $page: Flag for the full page state.
 * - $promote: Flag for front page promotion state.
 * - $sticky: Flags for sticky post setting.
 * - $status: Flag for published status.
 * - $comment: State of comment settings for the node.
 * - $readmore: Flags true if the teaser content of the node cannot hold the
 *   main body content.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 *
 * @see template_preprocess()
 * @see template_preprocess_node()
 */

//variable_set('admin_theme', 'garland');
global $base_url;
?>
<?php  if ($title): ?>
<table width="100%" border="0" cellpadding="0" cellspacing="0">
	<tbody> 	
	 	<tr>
	 		<td class="roundbox-TL"></td>
	 		<td class="roundbox-TM"></td>
	 		<td class="roundbox-TR"></td>
	 	</tr>
	 	<tr>
			<td class="roundbox-CL">&nbsp;</td>
 			<td class="roundbox-CM">
				<div class="front-block-header block-header forum_content_header">
					<div class="form_head_title"> <?php  print $title; ?> </div>
					<div class='forum-details'>
						<table border="0" width="100%" cellpadding="0" cellspacing="0">
							<tr>
								<td valign="top" align="center" width="15%">
									<div class="forum_pic">
										<div><?php print $picture ?></div>					
										<div class="forum_submit_text" class="created">
						  					<?php if ($submitted): ?>
						    				<?php print $submitted ?>
						  					<?php endif; ?>
				  						</div>
									</div>
								</td>
								<td valign="top" width="70%">
									<div class="forum_cont">
										<div class="node-description"><?php print $content ?></div>
											<?php if ($links && arg(0) == "node"): ?>
			      								<div class="links"><?php print $links; ?></div>
			    							<?php endif; ?>    			
									</div>
								</td>
								<td valign="top" width="15%">
									<div class="forum_rating">
										<?php if (isset($_SESSION['availableFunctionalities']->fivestar)) :?>
											<div class="forum_rating_title"><?php print t('Rating'); ?></div>
											<div>
												<?php
												if ( (arg(0) == 'node' || arg(0) == 'comment')) {
										        	if(user_access('rate content'))
										        	{          
											           if (_fivestar_validate_target('node', $node->nid)) {            
											            $star_rating = fivestar_widget_form($node);            
											           }
										        	}
										        }
												print $star_rating; ?>
											</div>
										<?php endif; ?>
										<div class="forum_posts"><?php print t('Number of Posts:'); ?></div>
										<div class="forumpostslist" id="forumCommentCount">
											<?php print variable_get('exp_sp_forum_comment_count',0); ?>
										</div>
									</div>							
								</td>
							</tr>
						</table>
											
						</div>		
					</div><!--  front-block-header closing here-->
					<div class="splitter">&nbsp;</div>
<?php  endif; 
$display_mode = $_GET['display_mode'];
	
	switch($display_mode)
	{
		case COMMENT_MODE_THREADED_EXPANDED:			
			variable_set('comment_default_mode_' . $node->type, COMMENT_MODE_THREADED_EXPANDED);
			break;
		case COMMENT_MODE_THREADED_COLLAPSED:			
			variable_set('comment_default_mode_' . $node->type, COMMENT_MODE_THREADED_COLLAPSED);
			break;
		case COMMENT_MODE_FLAT_EXPANDED:			
			variable_set('comment_default_mode_' . $node->type, COMMENT_MODE_FLAT_EXPANDED);
			break;
		case COMMENT_MODE_FLAT_COLLAPSED:			
			variable_set('comment_default_mode_' . $node->type, COMMENT_MODE_FLAT_COLLAPSED);
			break;
		default:			
			variable_set('comment_default_mode_' . $node->type, COMMENT_MODE_THREADED_COLLAPSED);	
			$display_mode = COMMENT_MODE_THREADED_COLLAPSED;		
			break;
	}

?>
<script>
function submitSearchComments()
{
  var searchComments = $("#searchComments").val();    
  var displayMode = $("#displayMode").val()?$("#displayMode").val():'';
  window.location="<?php echo $base_url.$node_url;?>&search_comments="+searchComments+"&display_mode="+displayMode;
  return false;  
}
$(document).ready(function(){
  $("#displayMode").val(<?php echo $display_mode;?>);
  if($("#comments").size()==0)
  {   
    $("#noPostFoundLabel").show();
  }
});
</script>

 		</td>
 		<td class="roundbox-CR">&nbsp;</td>
 	</tr>
 	<tr>
 		<td class="roundbox-BL"></td>
 		<td class="roundbox-BM"></td>
 		<td class="roundbox-BR"></td>
 	</tr>
 	</tbody>
 	</table>
<?php if(arg(0) == "node"):?>
<div class="forum_threaded">
<form name="searchCommentsForm" method="GET" onsubmit="submitSearchComments(); return false;">
<table width="100%" border="0" cellpadding="0" cellspacing="0">
	<tr>
	<td style='width:50%;'><?php
	 if(!isset($_REQUEST['comments_uid'])):
	 print t("Views"); ?>:
		<select onchange="submitSearchComments()" id="displayMode">			
			<option value="<?php echo COMMENT_MODE_THREADED_COLLAPSED;?>"><?php print t("Collapsed"); ?></option>
			<option value="<?php echo COMMENT_MODE_THREADED_EXPANDED;?>"><?php print t("Expanded"); ?></option>	
		</select>
		<?php endif; ?>
	</td>
	<td width="50%">	 
  	<table width="58%" align="right" border="0" cellpadding="0" cellspacing="0">
  	<tr>
  	  <td>
  	     <?php print t("Search this Forum:")?>
  	  </td>
    	<td>
    	 <input type="text" id='searchComments' value="<?php print $_REQUEST['search_comments']?>" />
    	</td>
  	<td>
  	<input type="submit" value="<?php print strtoupper(t("LBL304"));?>" />
  	</td></tr>
  	</table>  	
	</td>
	
	</tr>
	<?php if( !(isForumUserRegisteredForTP() || isForumUserRegisteredForCourse()) ):?>
  <tr>
  		<?php if ( isTPForum(arg(1)) ):?>
      <td colspan="2" style="text-align:center;" ><?php print t("You can comment on this forum or rate it only after you have registered for this training program and your registration has been confirmed."); ?></td>
    	<?php elseif ( isCourseForum(arg(1)) ):?>  
      <td colspan="2" style="text-align:center;" ><?php print t("You can comment on this forum or rate it only after you have registered for this course and your registration has been confirmed."); ?></td>
		<?php else:?>
      <td colspan="2" style="text-align:center;" ><?php print t("You can comment on this forum or rate it only after your registration has been confirmed."); ?></td>	  
    	<?php endif; ?>  
  </tr>
  <?php endif; ?>
	<tr>
    <td class='forum_comment_topics' colspan="2"><?php print t("Topics"); ?>:</td>
  </tr>
  <tr id="noPostFoundLabel" style="display:none;">
      <td colspan="2" ><?php print t("No posts found."); ?></td>
  </tr>
   
</table>
</form>
</div>
<?php endif; ?>