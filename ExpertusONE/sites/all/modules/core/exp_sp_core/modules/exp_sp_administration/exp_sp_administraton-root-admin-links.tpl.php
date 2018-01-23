<?php global $theme_key; 
$admin_links_list = getParentAdminLinks();
expDebug::dPrint('narrow search admin links tpl file : rendered_admin_links_list = ' . print_r($admin_links_list, true),4);
expDebug::dPrint('narrow search admin links tpl file : rendered_admin_links_list = ' . print_r($title, true),4);?> 
<table id = '<?php print $html_id; ?>' class="root-admin-links-holder">
  <tbody>
    <tr>
      <td>
            <div id="root-admin-links-holder" class="root-admin-links-holder-wrap">    
              <?php if($theme_key == 'expertusoneV2') {?>
              	<div class='root-admin-links-heading'><?php print t($title); ?></div>
              	<ul class="list-item-administrator addedsubmenulink">
							<?php $admin_main_menu_count = sizeof($admin_links_list); ?>
	                       <?php foreach ($admin_links_list as $admin_menu_index => $admin_menu_res){ ?>
	                            <?php if($admin_menu_res["code"] == "contentauthor"){ ?>
	                         		<li class="admin-left-panel-module-list <?php print ($admin_menu_index == $admin_main_menu_count-1) ? 'admin-main_menu-last':'';?>"><a name='admin_link_<?php print $admin_menu_res["code"]; ?>' class="<?php print ($admin_menu_index == 0) ? 'admin-main_menu-first ':' ';?>" id='<?php print $admin_menu_res["code"];?>' href='/?q=<?php print $admin_menu_res["path"];?>' data="{'link_path':'<?php print $admin_menu_res["path"];?>'}"><?php print $admin_menu_res["title"]; ?></a><span class='titleSubscript'>(<?php print t('BETA'); ?>) </span>
	                            <?php } else { ?>
							        <li class="admin-left-panel-module-list <?php print ($admin_menu_index == $admin_main_menu_count-1) ? 'admin-main_menu-last':'';?>"><a name='admin_link_<?php print $admin_menu_res["code"]; ?>' class="<?php print ($admin_menu_index == 0) ? 'admin-main_menu-first ':' ';?>" id='<?php print $admin_menu_res["code"];?>' href='/?q=<?php print $admin_menu_res["path"];?>' data="{'link_path':'<?php print $admin_menu_res["path"];?>'}"><?php print $admin_menu_res["title"]; ?></a>
							    <?php } ?>     
							        <ul class="admin-module-submenu-list-item">
											<?php 
											$sub_menu_count = sizeof($admin_menu_res["admin_sub_menu_arr"]);
											for($i=0;$i<sizeof($admin_menu_res["admin_sub_menu_arr"]);$i++){ ?>
												<?php $sub_menu_list = ($admin_menu_res["admin_sub_menu_arr"][$i]["title"]=='Courses and Classes')? 'Course/Class' : $admin_menu_res["admin_sub_menu_arr"][$i]["title"];?>
												<li class="<?php if($_SERVER['REQUEST_URI']=='/?q='.$admin_menu_res["admin_sub_menu_arr"][$i]['path']){  if($i==0){ echo "firstliclass"; }else{ echo "submenuselected"; }  } ?>" ><a class="asublink" href='/?q=<?php print $admin_menu_res["admin_sub_menu_arr"][$i]['path'];?>' data="{'link_path':'<?php print $admin_menu_res["admin_sub_menu_arr"][$i]['path'];?>'}"><span><?php print $sub_menu_list;?></span></a><?php if ($sub_menu_count>1): ?>, <?php endif; ?></li> <?php //if(($i+1)%3==0){ print "</br>";} ?>
												<?php $sub_menu_count--; ?>
											<?php } ?>
						    		</ul></li>
						  <?php } ?>  
              	
              <?php } else { ?>
					<div class='root-admin-links-heading'><?php print t($title); ?></div>
						<ul class="list-item-administrator">
						<?php $admin_main_menu_count = sizeof($admin_links_list); ?>
                       <?php foreach ($admin_links_list as $admin_menu_index => $admin_menu_res){ ?>
						        <li class="<?php print ($admin_menu_index == $admin_main_menu_count-1) ? 'admin-main_menu-last':'';?>"><a name='admin_link_<?php print $admin_menu_res["code"]; ?>' class="<?php print ($admin_menu_index == 0) ? 'admin-main_menu-first':'';?>" id='<?php print $admin_menu_res["code"];?>' href='/?q=<?php print $admin_menu_res["path"];?>' data="{'link_path':'<?php print $admin_menu_res["path"];?>'}"><?php print $admin_menu_res["title"]; ?></a>
						        <ul class="admin-module-submenu-list-item">
											<?php 
											$sub_menu_count = sizeof($admin_menu_res["admin_sub_menu_arr"]);
											for($i=0;$i<sizeof($admin_menu_res["admin_sub_menu_arr"]);$i++){?>
												<?php $sub_menu_list = ($admin_menu_res["admin_sub_menu_arr"][$i]["title"]=='Courses and Classes')? 'Course/Class' : $admin_menu_res["admin_sub_menu_arr"][$i]["title"];?>
												<li><span><?php print $sub_menu_list;?></span><?php if ($sub_menu_count>1): ?>, <?php endif; ?></li> <?php // if(($i+1)%3==0){ print "</br>";} ?>
												<?php $sub_menu_count--; ?>
											<?php } ?>
					    		</ul></li>
					  <?php } ?>
				<?php } ?>  
              </ul>
            </div>           
      </td>
    </tr>
  </tbody>
</table>
