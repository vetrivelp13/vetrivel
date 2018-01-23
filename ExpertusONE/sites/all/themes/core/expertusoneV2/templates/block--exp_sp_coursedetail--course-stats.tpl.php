<?php 
$result = new stdClass();
$result->catalog_item_count=2;
$result->catalog_item_type="Courses";
$result->catalog_item_delivery="Web-based, Classroom";
$result->catalog_item_lang="English , Chinese , Korean";
$result->catalog_item_equivalence="None";
$result->catalog_item_pre_requisite="Yes";
$result->catalog_item_topic_count=5;
?><div class="stats-container">
	<div class="cat-det-stat-container">
	
<?php if(isset($result->catalog_item_type) && $result->catalog_item_type=='Courses'){?>
<?php if(isset($result->catalog_item_count) && $result->catalog_item_count!=''){?>
		<div class="cat-det-stat-content">
	<!--		No of Courses  4 (3 Mandatory, 1 Optional) -->
			<div class="crs_num stats_title float-left"><?php print t("No of Courses"); ?>&nbsp;:&nbsp;</div>
			<div class="crs_num stats_info float-left"><?php print $result->catalog_item_count; ?></div>
			<div class="cat-det-stat-content-line clearBoth"></div>
		</div>
<?php }?>
<?php if(isset($result->catalog_item_delivery) && $result->catalog_item_delivery!=''){?>		
		<div class="cat-det-stat-content">
	<!--		Delivery type: WBT,ILT,VC-->
			<div class="crs_lang stats_title float-left"><?php print t("Languages"); ?>&nbsp;:&nbsp;</div>
			<div class="crs_lang stats_info float-left"><?php print $result->catalog_item_delivery; ?></div>
			<div class="cat-det-stat-content-line clearBoth"></div>
		</div>
<?php }?>		
<?php if(isset($result->catalog_item_lang) && $result->catalog_item_lang!=''){?>		
		<div class="cat-det-stat-content">
	<!--		Language: Engllish, Chinese, Spanish-->
			<div class="crs_lang stats_title float-left"><?php print t("Languages"); ?>&nbsp;:&nbsp;</div>
			<div class="crs_lang stats_info float-left"><?php print $result->catalog_item_lang; ?></div>
			<div class="cat-det-stat-content-line clearBoth"></div>
		</div>
<?php }?>
<?php if(isset($result->catalog_item_equivalence) && $result->catalog_item_equivalence!=''){?>		
		<div class="cat-det-stat-content">
	<!--		Equivalence: Yes-->
			<div class="crs_equ stats_title float-left"><?php print t("Equivalence"); ?>&nbsp;:&nbsp;</div>
			<div class="crs_equ stats_info float-left"><?php print $result->catalog_item_equivalence; ?></div>
			<div class="cat-det-stat-content-line clearBoth"></div>
		</div>
<?php }?>
<?php if(isset($result->catalog_item_pre_requisite) && $result->catalog_item_pre_requisite!=''){?>		
		<div class="cat-det-stat-content">
	<!--		Prerequisites: Yes -->
			<div class="crs_pre stats_title float-left"><?php print t("Pre-requisites"); ?>&nbsp;:&nbsp;</div>
			<div class="crs_pre stats_info float-left"><?php print $result->catalog_item_pre_requisite; ?></div>
			<div class="cat-det-stat-content-line clearBoth"></div>
		</div>
<?php }?>		
<?php if(isset($result->catalog_item_topic_count) && $result->catalog_item_topic_count!=''){?>		
		<div class="cat-det-stat-content">
	<!--		Discussion topics: 5 -->
			<div class="crs_dis_top stats_title float-left"><?php print t("Discussion topics"); ?>&nbsp;:&nbsp;</div>
			<div class="crs_dis_top stats_info float-left"><?php print $result->catalog_item_topic_count; ?></div>
			<div class="cat-det-stat-content-line clearBoth"></div>
		</div>
	</div>
<?php }?>	
<?php }?>
</div>
 
 
 
 	    			</div>
	    		</div>
 
 	    	</div>
 </div>
		</div>
		<div class='block-footer-left'>
			<div class='block-footer-right'>
				<div class='block-footer-middle'>&nbsp;</div>
			</div>
		</div>
	</div>
	</div>
	
	
	</div>
	</div>