<?php include 'header.php'; ?> 
<?php
header("content-type: text/html; charset=UTF-8");	//to print UTF8 characters properly
/*
 * ExpertusOne API
 * Documentation Engine.php v1.0
 *
 * @author: Ganesan
 * @date	:	04-Aug-2015
 *
 * All documentation related to the Dynamic REST API is generated here.
 *
 */
include_once($_SERVER["DOCUMENT_ROOT"].'/apis/dynamicapi/includes.php');

class DocuEngine{
	var $doc;
	
	function getAllEntity()
	{
		/*$select = db_select('slt_profile_list_items', 'splt');
		$select->addField('splt', 'code');
		$select->addField('splt', 'name');
		$select->condition('splt.parent_id','','IS NULL');
		$select->condition('splt.is_active','Y','=');
		$select->orderBy('name');
		//expDebug::dPrintDBAPI('$select  = ' , $select);
		
		$entityNames = $select->execute()->fetchAll();
		expDebug::dPrint('$entityNames = ' . print_r($entityNames, true) , 3);*/
		
		$entityNames = array("cbn_anm" => "Banner",
							"lrn_cls" => "Class",
							//"cme_set" => "Commerce Setting",
							"lrn_crs" => "Course",
							"lrn_cur" => "Training Plan",
							"lrn_ins" => "Instructor",
							"cre_org" => "Organization",
							//"cme_pmt" => "Payment",
							"lrn_res" => "Resources",
							//"cre_sec" => "Security",
							"sry"     => "Survey & Assessment",
							"cre_sys" => "System",
							//"lrn_tpm" => "Training Plan",
							"cre_usr" => "User",
							"usr_country" => "Country",
							"usr_state" => "State",
							"cre_sys_obt_cattr" => "Custom Attribute",); //#custom_attribute_0078975
		return $entityNames;
	}
	
	function getAllAttr()
	{
		/*$result = db_query("SELECT n.code, n.name 
				FROM {slt_profile_list_items} n WHERE LENGTH( code) - LENGTH(REPLACE(code, '_', '')) = 2 AND code like :code", array(':code' => '%'.db_like($code).'%'));
		
		expDebug::dPrint('$result  = '.$code, 5);
		expDebug::dPrintDBAPI('$result  = ' , $result);
		$entityNames = $result->fetchAll();
		expDebug::dPrint('$entityNames = ' . print_r($entityNames, true) , 3);*/
		
		/* Banner Attributes */
		$attrNames['cbn_anm'] = array("cbn_anm_sts" => "Banner Status", "cbn_anm_typ" => "Type");
		
		/* Class Attributes */
		$attrNames['lrn_cls'] = array("lrn_cls_sts" => "Class Status", "lrn_cls_dty" => "Delivery Type", "lrn_cls_vct" => "Virtual Classroom Type");
		
		/* Commerce Setting Attributes */
		//$attrNames['cme_set'] = array("cme_set_cnt" => "Country Setting", "cme_set_pmt" => "Payment Method", "cme_set_can" => "Cancellation Policy");
		
		/* Course Attributes */
		$attrNames['lrn_crs'] = array("lrn_crs_sts" => "Course Status", "lrn_crs_cmp" => "Completion Status", "lrn_crs_reg" => "Registration Status");
		
		/* Curricula Attributes */
		$attrNames['lrn_cur'] = array("lrn_cur_typ" => "Training Plan Type", "lrn_cur_sts" => "Training Plan Status", "lrn_tpm_ovr" => "Training Plan Enrollment Status");
		
		/* Instructor Attributes */
		$attrNames['lrn_ins'] = array("lrn_ins_sts" => "Instructor Status");
		
		/* Organization Attributes */
		$attrNames['cre_org'] = array("cre_org_sts" => "Org Status", "cre_org_typ" => "Org Type");
		
		/* Payment Attributes */
		$attrNames['cme_pmt'] = array("cme_pmt_sts" => "Payment Status", "cme_pmt_typ" => "Payment Type");
		
		/* Resources Attributes */
		$attrNames['lrn_res'] = array("lrn_res_eqm" => "Equipment", "lrn_res_rms" => "Classroom Status", "lrn_res_loc" => "Location Status");
		
		/* Security Attributes */
		$attrNames['cre_sec'] = array("cre_sec_sts" => "Group Status");
		
		/* Survey & Assessment Attributes */
		$attrNames['sry'] = array("sry_det_typ" => "Survey & Assessment Type", "sry_det_oth" => "Survey & Assessment Others", "sry_det_sry" => "Survey & Assessment Status", "sry_det_grp" => "Survey & Assessment Group Status", "sry_qtn_sts" => "Question Status", "sry_qtn_typ" => "Question Type","sry_ans_typ" => "Assessment Question Answer Logic");
		
		/* System Attributes */
		$attrNames['cre_sys'] = array("cre_sys_crn" => "Currency Type", "cre_sys_lng" => "Languages", "cre_sys_tmz" => "Time Zones");
		
		/* Training Plan Attributes */
		//$attrNames['lrn_tpm'] = array("lrn_tpm_ovr" => "Training Plan Status");
		
		/* User Attributes */
		$attrNames['cre_usr'] = array("cre_usr_dpt" => "Department", "cre_usr_etp" => "Employment Type", "cre_usr_gtp" => "Gender Type", "cre_usr_jrl" => "Job Role", 
										"cre_usr_jtl" => "Job Title", "cre_usr_ptp" => "User Type", "cre_usr_sts" => "User Status", "cre_usr_grp" => "User Groups");
		
		/* Country Attributes */
		$attrNames['usr_country'] = array("country_code" => "Country");
		
		/* State Attributes */
		$attrNames['usr_state'] = array("state_code" => "State");
		
		/* Custom Attributes - #custom_attribute_0078975*/
        $attrNames['cre_sys_obt_cattr'] = array("cre_cattr_sts" => "Custom Attribute Status"); 
		
		return $attrNames;
	}
	
	function getAllStatus($code)
	{
		if($code != "")
		{
			if($code == "country_code")
				$result = db_query("SELECT country_code as code, country_name as name FROM {slt_country} order by country_name");
			elseif($code == "state_code")
				$result = db_query("SELECT state_code as code, state_name as name FROM {slt_state} order by state_name");
			elseif($code == "cre_usr_grp")
				$result = db_query("SELECT code, name FROM {slt_groups} where status='cre_sec_sts_atv' order by name");
			elseif($code == "lrn_cur_typ")
				$result = db_query("SELECT n.code, n.name FROM {slt_profile_list_items} n WHERE is_active='Y' AND (code='cre_sys_obt_trn' OR code='cre_sys_obt_crt' OR code='cre_sys_obt_cur')");
			elseif($code == "cre_sys_tmz")
			$result = db_query("SELECT n.code, n.name FROM {slt_profile_list_items} n WHERE is_active='Y' AND SUBSTRING(n.code, -4) != '_del' AND code like :code order by CAST(REPLACE(REPLACE(REPLACE(REPLACE(n.attr1,'GMT',''),'(',''), ')', ''),':','')AS SIGNED)", array(':code' => '%'.db_like($code).'_%'));
			else
				$result = db_query("SELECT n.code, n.name FROM {slt_profile_list_items} n WHERE is_active='Y' AND SUBSTRING(n.code, -4) != '_del' AND code like :code", array(':code' => '%'.db_like($code).'_%'));
			
			expDebug::dPrint('$result  = '.$code, 5);
			expDebug::dPrintDBAPI('$result  = ' , $result);
			$entityNames = $result->fetchAll();
			//expDebug::dPrint('$entityNames = ' . print_r($entityNames, true) , 3);
			return $entityNames;
		}	
		else
			return "";
	}
}
$DE = new DocuEngine();
$allentitydata = $DE->getAllEntity();
$allAttrdata = $DE->getAllAttr();
?>

<div id="content-outer">
    <div id="breadcrumbs">
		<div class="breadcrumb">
			<a href="index.php">Home</a> &rarr; <a href="#">Documentation</a><div><a href="#" data-count="none" data-related="" class="share-button">Tweet</a></div>
		</div>      
	</div>
	
	<h1 id="title">API Codes</h1>
	<div class="toc" id="toc">
		<div class="toc-title"></div>
		<div class="toc-list">
			<a href="#">Entities</a>
			<ol>
				<?php foreach ($allentitydata as $ent_code => $ent_name){  
							if(count($allAttrdata[$ent_code]) > 0 ) {
				?>
					<li class="toc-level-2"><a href="#<?php echo $ent_code ?>"><?php echo $ent_name ?></a>
						<ul>
							<?php foreach ($allAttrdata[$ent_code] as $attr_code => $attr_name){  ?>
							<li class="toc-level-2"><a href="#<?php echo $attr_code ?>"><?php echo $attr_name ?></a>
							<?php } }  ?>
						</ul>
					</li>
				<?php }  ?>
			</ol>
			
			</li>
			
		
		</div>
	</div>
	
    <div id="content-inner">
       <div id="content-main">
			<?php 
			if(count($allentitydata) > 0 ) {
			
				foreach ($allentitydata as $ent_code => $ent_name){ 
					
					if(count($allAttrdata[$ent_code]) > 0 ) {
			?>
					<div class="section" id="<?php echo $ent_code ?>">
  						<h3 id="getstarted"><?php echo $ent_name ?></h3>
						<?php 
						foreach ($allAttrdata[$ent_code] as $attr_code => $attr_name){
							if($attr_code == "sry_det_oth")
								$allStatusdata = $DE->getAllStatus("sry_det_typ");
							else
								$allStatusdata = $DE->getAllStatus($attr_code);
						?>
							<div class="table sectionedit3" id="<?php echo $attr_code ?>">
								<table class="inline" border="0" style="border: 1px solid #EEEEEE;padding-right:5px;">
									<tbody>
										<tr class="row0">
											<th colspan="2" class="col0 leftalign"><?php echo $attr_name ?></th>
										</tr>
										<?php 
										
										if(count($allStatusdata) > 0 ) {
											$attr_name = explode(" ", $attr_name);
											$attr_name = end($attr_name);
											if($attr_name != "Type" && $attr_name != 'Status' && $attr_name != "Title")
												$attr_name = "Name";
										?>
										<tr class="row0">
											<th class="col0 leftalign" width="50%"><?php echo $attr_name; ?></th><th class="col1 leftalign" width="50%">Code </th>
										</tr>
										<?php
										foreach ($allStatusdata as $status_record){ 
											if($status_record->name != '' && $status_record->code != ''){
												
												if($attr_code == "sry_det_typ")
												{
													if($status_record->code != "sry_det_typ_sry" && $status_record->code != "sry_det_typ_ass")
														continue;
												}
												if($attr_code == "sry_det_oth")
												{
													if($status_record->code == "sry_det_typ_sry" || $status_record->code == "sry_det_typ_ass")
														continue;
												}
										?>
										<tr class="row0">
											<td class="row0" width="50%"><?php echo str_replace("Survey Assessment Deleted", "Deleted", $status_record->name) ?></td><td class="row0" width="50%"><?php echo $status_record->code ?></td>
										</tr>
										<?php } } } ?>
									</tbody>
								</table>
							</div>
						<?php } ?>
					</div>
			<?php  }  } } else { ?>
					<div class="section">
					    <h3 id="getstarted">No Data</h3>
					</div>
			<?php } ?> 
 		</div>       
 	</div>
</div>    
 
<?php include 'footer.php'; ?> 