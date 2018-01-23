<?php
// $Id: comment-forum.tpl.php,v 1.15 2011-03-18 07:07:08 muthusamys Exp $

/**
 * @file comment.tpl.php
 * Default theme implementation for comments.
 *
 * Available variables:
 * - $author: Comment author. Can be link or plain text.
 * - $classes: A set of CSS classes for the DIV wrapping the comment.
     Possible values are: comment, comment-new, comment-preview,
     comment-unpublished, comment-published, odd, even, first, last,
     comment-by-anon, comment-by-author, or comment-mine.
 * - $content: Body of the post.
 * - $date: Date and time of posting.
 * - $links: Various operational links.
 * - $new: New comment marker.
 * - $picture: Authors picture.
 * - $signature: Authors signature.
 * - $status: Comment status. Possible values are:
 *   comment-unpublished, comment-published or comment-preview.
 * - $submitted: By line with date and time.
 * - $title: Linked title.
 * - $unpublished: Is the comment unpublished?
 *
 * These two variables are provided for context.
 * - $comment: Full comment object.
 * - $node: Node object the comments are attached to.
 *
 * @see template_preprocess_comment()
 * @see theme_comment()
 */
global $theme;
$path = arg();
if((arg(0) == "comment") && (arg(1) == "reply") )
{	
	print t("<h4 class='pgTitle' id='replyTopic'>"."Reply to a Post"."</h4>");
}
// Refresh event leader board to update ratings.
if($new)
{  
  print "<script>var parentWindow=window.opener; if(!parentWindow.closed && parentWindow!=null){parentWindow.EXPERTUS.SMARTPORTAL.ForumWidget.getForumsSingleTopic();}</script>";
}
?>
<script>
$(document).ready(function(){
  if($("#forumCommentCount").size()>0)
      $("#forumCommentCount").html(<?php print variable_get('exp_sp_forum_comment_count',0); ?>);
});
</script>
<div class="comment_cont">
	<div class="commentposted">
		<div class="commentposted">
			<div class="commentposted">
				<table border="0" cellpadding="0" width="100%" cellspacing="0">
					<tr>
						<td valign="top" align="left">
							<div class="comment_title">
								<b><span class="comment-title-link commnet-topic"><?php print $title ?><span></b> <?php print t(" - <span>").$author.'</span>, <span>'.ago($comment->timestamp).'</span>'; ?>
							</div>
						</td>
						<td valign="middle" align="right" style="width: 42%">
							<div class="comment_edit">
						  		<?php if (!empty($links)): ?>
						      		<div class="forum-post-links">
						        		<?php print $links ?>
						      		</div>
						    	<?php endif; ?>
						  	</div>						
						</td>
					</tr>
				</table>
			</div>
			<table border="0" cellpadding="0" cellspacing="0">
				<tr>
					<td width="6%" valign="top" align="center">
						<?php if(empty($comment->pid) && !( $path[0] == 'comment' && $path[1] == 'reply' )): ?>
							<div style="float:left;width:100%">
								  <div style='float:left;'>
										<div style='float:left; font-weight:bold;padding-left:18px;'><?php print t("Avg Rating:");?></div>
											<div style='float:left;' id="commentAvgRating<?php print $comment->cid ?>">
											  <?php print "<script>renderStars(".$comment->avg_rating.",'commentAvgRating".$comment->cid."')</script>"; ?>
											</div>
								  </div>
							</div>
						<?php endif; ?>
					</td>
				</tr>
			</table>
			<table border="0" cellpadding="0" cellspacing="0" style="width: 100%">
				<tr>
					<td valign="top" align="center" style="width: 6%;">
						<div>
							<?php print $picture;?>
						</div>		
					</td>
					<td valign="top" align="left">
							<div class="content">
			    				<?php print $content ?>
			    				<?php
			    					if (isset($_SESSION['availableFunctionalities']->flag_abuse)) :
			    		 				$flag = flag_get_flag('abuse_comment2');
									if ($flag && $flag->is_flagged($comment->cid)) {
							  			print '<span style="color:#ff0000">'.t("This comment is reported as abused.").'</span>';
										} 
									endif;
			    		 		?>
			    		 		<div class="splitter">&nbsp;</div>
			  				</div>
					</td>
				</tr>
			</table>
	  	</div>
	  	
  	</div>
</div>

