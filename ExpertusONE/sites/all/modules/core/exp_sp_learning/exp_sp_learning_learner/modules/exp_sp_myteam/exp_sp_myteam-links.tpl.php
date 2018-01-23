<?php 	expDebug::dPrint(' getRe sandhyaa '.$Type,4);
expDebug::dPrint(' getRe abhiiiii '.$MyApproval_visibility,4);
$styleAttribute ='style ="text-decoration:none" ';
$styleAttributeTeam='style ="text-decoration:none" ';
if($Type == "MyApproval"){
	$styleAttribute = 'style ="text-decoration:underline" ';
}else if($Type == "MyTeam"){
	$styleAttributeTeam = 'style ="text-decoration:underline" ';
}
$addCls = "";
$addParent = "";
if($Type == "MyTeam" || $Type == "MyApproval" ){
    $addCls = "root-admin-links-selected";
    $addParent = "root-admin-module-selected";
}
     


?>
<ul class="list-item-administrator addedsubmenulink">
<li class="admin-left-panel-module-list <?php print $addParent ?> ">
<a id="learning" class="admin-main_menu-first <?php print $addCls ?>" name="admin_link_learning" href="/?q=learning/myteam-search" data="{'link_path':'learning/myteam-search'}"><?php print t("Manage"); ?></a>
<ul class="admin-module-submenu-list-item">
<li class="myteamliclass">
<?php if($Type == "MyTeam"){?><a class="asublink" id="myteamlink" style="text-decoration:underline" href="/?q=learning/myteam-search" data="{'link_path':'learning/myteam-search'}">
<?php }else{?>
<a class="asublink" id="myteamlink"  href="/?q=learning/myteam-search" data="{'link_path':'learning/myteam-search'}"><?php }?><span><?php print t("MyTeam");?></span></a><?php if($MyApproval_visibility == 1) {?>,&nbsp</li>
<li class="approvalliclass">
<?php if($Type == "MyApproval"){?>
<a class="asublink" id = "myapprovallink"   style ="text-decoration:underline"  href="/?q=myapproval/myteam/Certificate" data="{'link_path':'myapproval/myteam/Certificate'}">
<?php }else{?>
<a class="asublink" id = "myapprovallink"  href="/?q=myapproval/myteam/Certificate" data="{'link_path':'myapproval/myteam/Certificate'}">
<?php }?>
<span><?php print t("MyApproval"); ?></span>
</a>
</li>
<?php } else{ ?>
</li>
<?php } ?>
</li>
</ul>




