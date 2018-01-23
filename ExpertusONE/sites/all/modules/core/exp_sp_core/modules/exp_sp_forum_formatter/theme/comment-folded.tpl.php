<?php
// $Id: comment-folded.tpl.php,v 1.4 2011-03-18 09:41:48 muthusamys Exp $

/**
 * @file comment-folded.tpl.php
 * Default theme implementation for folded comments.
 *
 * Available variables:
 * - $title: Linked title to full comment.
 * - $new: New comment marker.
 * - $author: Comment author. Can be link or plain text.
 * - $date: Date and time of posting.
 * - $comment: Full comment object.
 *
 * @see template_preprocess_comment_folded()
 * @see theme_comment_folded()
 */
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
<div class="comment-folded" style='width:100%;float:left;'>
  <div style="float:left;" class="subject"><?php print $title; ?></div>
  <div style="float:left;" class="credit"><?php print ' - '. '<b>'.$author.',</b>&nbsp;'.$date; ?></div>
  <?php if(empty($comment->pid) && !( $path[0] == 'comment' && $path[1] == 'reply' )): ?>
  <div style='float:left;'>, <?php print t("Avg Rating:");?></div>
  <div style="float:left;" id="commentAvgRating<?php print $comment->cid ?>"><?php print "<script>renderStars(".$comment->avg_rating.",'commentAvgRating".$comment->cid."')</script>"; ?></div>
  <?php endif; ?>
</div>
