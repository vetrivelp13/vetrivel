<?php expDebug::dPrint('narrow search tab content tpl file',4);?>
<div id="<?php print $html_id;?>" class="narrow-search-results"
	data='<?php print "".$narrowsearch_tab_content_list."";?>'>
	<div id="narrow-search-results-topbar"
		class="narrow-search-results-topbar" style="display: none;">
		<!-- Start-narrow search results topbar -->
	</div>
	<!-- End-narrow search results topbar -->
	<div class="clearBoth"></div>
	<div id="paint-narrow-search-results"
		class="paint-narrow-search-results">
		<div id="narrow-search-no-records" class="narrow-search-no-records"
			style="display: none;"><?php print t('MSG381'); ?></div>
		<table id="narrow-search-results-holder"></table>
		<!-- jqGrid displays the results here -->
	</div>
	<div class="show-more-wrapper">
		<span id="admin-narrow-search-show-more" class="show-more-handler"><?php print t('SHOW MORE'); ?></span>
	</div>
	<!--<div id="narrow-search-results-pager" style="display: block;"></div>
	<!-- jqGrid Pagination -->
</div>