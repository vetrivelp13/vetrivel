<div class="content_stats_wrapper">
	<ul class ="content_stats">
		<?php  if (!empty($result->class_count)): ?>
			<li class="stats-item stats-cls-cnt">
				<div class="stats-name"><?php print t('No of Classes'); ?>:</div>
				<div class="stats-val"><?php print $result->class_count; ?></div>
			</li>
		<?php endif; ?>
		
		<?php if (!empty($result->course_count)): ?>
			<li class="stats-item stats-crs-cnt">
				<div class="stats-name"><?php print t('No of Courses'); ?>:</div>
				<div class="stats-val">
					<?php print $result->course_count; ?>
					<span class="man_opt_courses">(<?php print $result->courses->mandatory . ' ' . t('Mandatory'); ?><?php if (!empty($result->courses->optional)): ?><?php print ', ' . $result->courses->optional . ' ' . t('Optional'); ?><?php endif; ?>)</span>
			</li>
		<?php endif; ?>
		
		<?php if (!empty($result->delivery_type)): ?>
			<li class="stats-item stats-del-type">
				<div class="stats-name"><?php print t('LBL084'); ?>:</div>
				<div class="stats-val"><?php print $result->delivery_type; ?></div>
			</li>
		<?php endif; ?>
		
		<?php if (!empty($result->language)): ?>
			<li class="stats-item stats-lang">
				<div class="stats-name"><?php print t('LBL038'); ?>:</div>
				<div class="stats-val"><?php print $result->language; ?></div>
			</li>
		<?php endif; ?>
		
		<?php if (!empty($result->session_count)): ?>
			<li class="stats-item stats-sess-cnt">
				<div class="stats-name"><?php print t('LBL277'); ?>:</div>
				<div class="stats-val"><?php print $result->session_count; ?></div>
			</li>
		<?php endif; ?>
		
		<?php if (!empty($result->duration)): ?>
			<li class="stats-item stats-duration">
				<div class="stats-name"><?php print t('LBL248'); ?>:</div>
				<div class="stats-val"><?php print $result->duration; ?></div>
			</li>
		<?php endif; ?>
		
		<?php if (!empty($result->content_count)): ?>
			<li class="stats-item stats-content-cnt">
				<div class="stats-name"><?php print t('Content'); ?>:</div>
				<div class="stats-val"><?php print $result->content_count; ?></div>
			</li>
		<?php endif; ?>
		
		<?php if (!empty($result->assessment)): ?>
			<li class="stats-item stats-assess">
				<div class="stats-name"><?php print t('Assessment'); ?>:</div>
				<div class="stats-val"><?php print $result->assessment; ?></div>
			</li>
		<?php endif; ?>
		
		<?php if (!empty($result->survey)): ?>
			<li class="stats-item stats-survey">
				<div class="stats-name"><?php print t('Survey'); ?>:</div>
				<div class="stats-val"><?php print $result->survey; ?></div>
			</li>
		<?php endif; ?>
		
		<?php if (!empty($result->prerequisite)): ?>
			<li class="stats-item stats-prereq">
				<div class="stats-name"><?php print t('LBL230'); ?>:</div>
				<div class="stats-val"><?php print $result->prerequisite; ?></div>
			</li>
		<?php endif; ?>
		
		<?php if (!empty($result->equivalence)): ?>
			<li class="stats-item stats-equilval">
				<div class="stats-name"><?php print t('LBL279'); ?>:</div>
				<div class="stats-val"><?php print $result->equivalence; ?></div>
			</li>
		<?php endif; ?>
		
		<?php if (!empty($result->discussion_topics)): ?>
			<li class="stats-item stats-dis-topics">
				<div class="stats-name"><?php print t('Discussion Topics'); ?>:</div>
				<div class="stats-val"><?php print $result->discussion_topics; ?></div>
			</li>
		<?php endif; ?>
	</ul>
</div>  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
