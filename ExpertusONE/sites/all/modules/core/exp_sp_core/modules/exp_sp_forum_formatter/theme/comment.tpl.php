<?php
// $Id: comment.tpl.php,v 1.3 2011-03-03 11:39:45 sulthanabdulkadarn Exp $

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
$path_theme = drupal_get_path('theme', $theme);
?>
<div style="width:86%; margin-left: 14%; overflow: hidden; border-bottom:1px solid #aaaaaa; ">
	<div class="comment_title">
		<b><span class="comment-title-link commnet-topic"><?php print $title ?><span></b> <?php print t(" - ").$author.', '.ago($comment->timestamp); ?>
	</div>

	<div style="padding: 5px 5px 0px 0px; width: 7%; float: left;">
		<div style='text-align:center;'><img src="<?php print $path_theme?>/images/default_user.png" /></div>
		<div style="padding:5px 0px; color:#F8981D; text-align:center;"><?php print ucfirst($type); ?></div>
		<div class="forum-post-panel-sub">
	      <?php print $author_pane; ?>
	    </div>
	</div>
	
	<div style="float:left; width:92%; overflow:hidden;">
			<div class="content">
	    		<?php print $content ?>
	  		</div>
				
	  		<div style="padding-top:10px;" class="created">
	  		<?php if ($submitted): ?>
	    		<?php print $author.$submitted; ?>
	  		<?php endif; ?>
	  		</div>
	  		<?php if (!empty($links)): ?>
      			<div class="forum-post-links">
        			<?php print $links ?>
      			</div>
    		<?php endif; ?>
	  		
  		</div>
  		
</div>
