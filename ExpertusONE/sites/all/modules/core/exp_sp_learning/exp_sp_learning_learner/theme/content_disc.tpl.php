 <?php
$callFrom = $context['callfrom'];
switch($callFrom) {
	case 'class_details':
		$inputId = 'tdataClsParentId';
		break;
	case 'course_details':
		$inputId = 'tdataCrsId';
		break;
	case 'lrnplan_details':
		$inputId = 'tdataTpId';
		break;	
	default:
		$inputId = 'tdataClsParentId';
		break;				
}
// expDebug::dPrint('$result in discussions' . print_r($result, 1)); 
// expDebug::dPrint('callfrom in discussion: ' . $callFrom);
if (arg ( 0 ) != 'widget' && arg ( 1 ) != '' && ($result[0]->tid) != '') { // hide forum for widget detail page ?>
<!-- html for rendering forum topics -->
<input id='<?php print $inputId; ?>' type="hidden" value="<?php print $result[0]->tid ; ?>" />
<div class="para">
	<div class="code-container discussions">
		<span class="sub-section-title"><?php print t("DISCUSSIONS"); ?></span>
		<div class="clearBoth"></div>
		<span class="disc-list-container">
			<div id="no-records" style="display: none"></div>
			<div id="forum-topic-list-display" class="forum-topic-list-class">
				<table id="forumTopicListContentResults"></table>
			</div>
		</span>
	</div>
</div>
<div id="pager" style="display: none;"></div>
<!-- Datatable Pagination -->

<?php } ?>		  