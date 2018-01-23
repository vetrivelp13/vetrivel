<?php
/**
 * Display a list of discounts.
 */
function uc_discounts_admin_list() {
  $search_params = array(
    'name' => arg(5),
  );

  $header = array(
  array('data' => t('Discount Name'), 'field' => 'name'),
  //array('data' => t('Short Description'), 'field' => 'short_description'),
  array('data' => t('Qualifying Type'), 'field' => 'qualifying_type'),
  array('data' => t('Type'), 'field' => 'discount_type'),
  // array('data' => t('Amount'), 'field' => 'discount_amount'),
  // array('data' => t('Weight'), 'field' => 'weight'),
  //array('data' => t('Created At'), 'field' => 'insert_timestamp', 'sort' => 'desc'),
  array('data' => t('Expires At'), 'field' => 'expiration'),
  array('data' => t('Active'), 'field' => 'is_active'),
  array('data' => t('Actions')),
  );
  
  $sortIndex = '';
  if(isset($_GET["order"])) {
    if($_GET["order"] == 'Discount Name') {
      $sortIndex = 'name';
      $sord      = $_GET["sort"];
    } else if($_GET["order"] == 'Qualifying Type') {
      $sortIndex = 'qualifying_type';
      $sord      = $_GET["sort"];
    } else if($_GET["order"] == 'Type') {
      $sortIndex = 'discount_type';
      $sord      = $_GET["sort"];
    } else if($_GET["order"] == 'Expires At') {
      $sortIndex = 'expiration';
      $sord      = $_GET["sort"];
    } else if($_GET["order"] == 'Active') {
      $sortIndex = 'is_active';
      $sord      = $_GET["sort"];
    }
  }
  $searchParam = arg(5);

  $query = db_select("uc_discounts",'d')
  ->fields('d', array('discount_id','name','qualifying_type','discount_type','has_expiration','expiration','is_active'));
  
  if(!empty($searchParam)) {
    $query->condition(db_or()->
      condition('d.name','%'.db_like($searchParam).'%','LIKE')->
      condition('d.qualifying_type','%'.db_like($searchParam).'%','LIKE')->
      condition('d.discount_type','%'.db_like($searchParam).'%','LIKE')->
      condition('d.is_active','%'.db_like($searchParam).'%','LIKE')->
      condition('d.expiration','%'.db_like($searchParam).'%','LIKE')
    );
  }
 
 if(!empty($sortIndex)) {
    $query->orderBy($sortIndex,$sord);
 }
  $gridRes = $query->execute();

  $rows = array();
  //while ($discount = db_fetch_object($result)) {
  foreach($gridRes as $discount) {
    //$total_use_count = (is_numeric($discount->total_use_count)) ? $discount->total_use_count : 0;
    //$total_times_applied = (is_numeric($discount->total_times_applied)) ? $discount->total_times_applied : 0;
    $operations = array(
    // l(t("usage"), "admin/reports/uc_discounts/discount/". $discount->discount_id),
    l(t("edit"), "admin/store-admin/uc_discounts/edit/". $discount->discount_id),
    // l(t("copy"), "admin/store-admin/uc_discounts/copy/". $discount->discount_id),
    // l(t("delete"), "admin/store-admin/uc_discounts/delete/". $discount->discount_id),
    );
    //echo "<br>======discount_type_name======== 1"; print_r($discount);
    $rows[] = array('data' =>
    array(
    $discount->name,
    //$discount->short_description,
    qualifying_type_name($discount->qualifying_type),
    discount_type_name($discount->discount_type),
    // array('data' => discount_amount_formatted($discount), 'class' => 'numeric'),
    // array('data' => $discount->weight, 'class' => 'numeric'),
    // array('data' => format_date($discount->insert_timestamp, 'small'), 'class' => 'date'),
    array('data' => ($discount->has_expiration ? format_date($discount->expiration, 'small') : 'N/A'), 'class' => 'date'),
    array('data' => ($discount->is_active ? '&#10003;' : 'X'), 'class' => ($discount->is_active ? 'is_active' : 'is_inactive')),
    //array('data' => implode(' ', $operations), 'class' => 'operations'),
    array('data' => uc_discount_actions($discount, TRUE),'class' => 'operations', 'nowrap' => 'nowrap'),
    ),
    );
  }

  /**/

  if (empty($rows)) {
    $rows[] = array(array('data' => t('No discounts.'), 'colspan' => count($header)));
  }

  /*$output = '<div class="uc-discount-search">
    			<div class="uc-store-add-button">
    				<form action="'.url('admin/store-admin/uc_discounts/add').'" method="post"><input type="submit" value="'.t("Add Discount").'" /></form>
    			</div>
               </div>';

  $output .= '<div class="content-wid-resize ">';
  //$output .= drupal_get_form('uc_discounts_search_form');
  $output .= '</div>';
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('uc-discounts'))));
  //$output .= theme('table', $header, $rows, array('class' => 'uc-discounts'));
  $output .= theme('pager', NULL, 50, 0);
  return $output;*/
  
  $build = array();

  //if ($search === FALSE) {
    $build['discount_overview_search_form'] = drupal_get_form('uc_discounts_search_form', $search_params);
    /* + array(
      '#prefix' => '<div class="order-overview-form">',
      '#suffix' => '</div>',
    );*/    
  //}

  $build['discounts'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('class' => array('uc-discounts')),
    '#weight' => 1,
  );
  $build['pager'] = array(
    '#theme' => 'pager',
    '#element' => 0,
    '#weight' => 5,
  );
  
  return $build;
}

/**
 * Create a form for searching the discounts
 */
function uc_discounts_search_form($form, &$form_state) {
  $form = array();

 $form['search'] = array(
   '#type' => 'fieldset',
   '#title' => '',
   '#collapsible' => FALSE,
   '#collapsed' => empty($values['name']),
   );

  $form['search']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => !empty($form_state['build_info']['args'][0]['name']) ? $form_state['build_info']['args'][0]['name'] : '',
    '#autocomplete_path' => 'portalpages/sp_administrator/discount/autocomplete',
  );

  $form['search']['submit'] = array(
    '#type' => 'submit',
    '#value' => strtoupper(t('LBL304')),
    '#prefix' => '<div class="search-submit">',
    '#suffix' => '</div>',
  );
  $form['search']['add'] = array(
  	'#markup' => '<div class="uc-discount-search">
    			<div class="uc-store-add-button">
    				<input onclick="location.href=\''.url('portalpages/sp_administrator/store-admin/uc_discounts/add').'\'" type="button" value="'.t("Add Discount").'" />
    			</div>
               </div>');

  return $form;
}

function uc_discounts_search_form_submit($form, &$form_state) {
  $args = array(
  $form_state['values']['name'],
  );
  $form_state['redirect'] = array('portalpages/sp_administrator/store-admin/uc_discounts/list/'. implode('/', $args));
}

/**
 * Create or edit a discount.
 *
 * @ingroup forms
 * @see uc_discounts_form_submit()
 */
//function uc_discounts_form($form_state, $discount_id = 0) {
function uc_discounts_form($form, &$form_state) {

  $discount_id    = arg(5);
  $form           = array();
  $form_submitted = $form_state["submitted"];
  $is_edit        = !empty($discount_id) ? true : false;// != 0;
  $seed           = rand(1, 1000000);

  if (!$form_submitted && $is_edit) { 
    $form_state["values"] = db_query("SELECT * FROM {uc_discounts} WHERE discount_id = :discount_id",array(':discount_id' => $discount_id))->fetchAll();
    $form_state["values"] = !empty($form_state) ? (array) $form_state["values"][0] : '';
  }

  $form["#attributes"] = array('class' => 'uc-discounts-form');
  $form["discount_id"] = array("#type" => "hidden", "#value" => !empty($form_state["values"]["discount_id"]) ? $form_state["values"]["discount_id"] : '');

  $form["name"] = array(
    '#prefix' =>'<div class="discount-container"><div class="uc-store-backlinks-div"><fieldset><legend>Related Links</legend><div class="uc-store-backlinks-innerdiv">'.l(t('Back to Discounts'),'portalpages/sp_administrator/store-admin/uc_discounts').'</div></fieldset></div><div class="discount_desc">',
    '#type' => "textfield",
    '#title' => t("Name"),
    '#default_value' => (!empty($form_state["values"]["name"]) ? $form_state["values"]["name"] : ''),
    '#size' => 30,
    '#required' => TRUE,
  );

  $form["description"] = array(
    '#type' => "textarea",
    '#title' => t("Description"),
    '#default_value' => !empty($form_state["values"]["description"]) ? $form_state["values"]["description"] : '',
    '#size' => 50,
  );
  
  //Determine codes default value
  $codes_string = NULL;
  if (isset($form_state["values"]["codes"])) {
    $codes_string = $form_state["values"]["codes"];
  }
  else {
    $codes = array();
    if ($is_edit) {
      $codes = get_codes_for_discount($discount_id);
    }
    $codes_string = create_codes_string($codes);
  }
  
  $form["discount_code"] = array(
    "#title" => t("Code"),
    "#type" => "textarea",
    "#default_value" => $codes_string,
    "#rows" => 5,
    "#required" => TRUE,    
  );

  $is_active = isset($form_state["values"]["is_active"]) ? $form_state["values"]["is_active"] : FALSE;
  $form["is_active"] = array(
    "#type" => "checkbox",
    "#title" => t("Is Active"),
    "#default_value" => $is_active,
  );

  $form["discount_type"] = array(
    "#type" => "select",
    "#title" => t("Discount Type"),
    "#options" => discount_type_options(),
    "#default_value" => !empty($form_state["values"]["discount_type"]) ? $form_state["values"]["discount_type"] : '',
  );

  $form["discount_amount"] = array(
    "#type" => "textfield",
    "#title" => t("Discount Amount"),
    "#description" => t("The amount of discount.  E.g. 50 (for $50) or 0.05 (for 5%)"),
    "#default_value" => !empty($form_state["values"]["discount_amount"]) ? $form_state["values"]["discount_amount"] : '',
    "#size" => 15,
    "#suffix" => "</div>",
    "#required" => TRUE,
  );

  $options = array();
  for ($i = -50; $i <= 50; $i++) $options[$i] = $i;
  $form["weight"] = array(
    "#type" => "select",
    "#title" => t("Priority"),
    "#options" => $options,
    "#default_value" => isset($form_state["values"]["weight"]) ? $form_state["values"]["weight"] : 0,
    "#prefix"  => '<div id="discount_priority">',
    "#suffix" => '</div>',
  );

  $has_activation = isset($form_state["values"]["has_activation"]) ? $form_state["values"]["has_activation"] : FALSE;
  /*$form['bussinessRule'] = array(
    '#type' => 'fieldset',
    '#title' => t('LBL719'),
    '#collapsible' => TRUE,
  );*/
  
  $form['bussinessRule']["has_activation"] = array(
    "#prefix" =>'<div class="has-activation">',
    "#type" => "checkbox",
    "#title" => t("Discount Activation Date"),
    "#default_value" => $has_activation,
    '#attributes' => array('onchange' => "$('#discount-activates-on-wrapper').toggle();"),
  );
  $activates_on = isset($form_state["values"]["activates_on"]) ? $form_state["values"]["activates_on"] : time();
  $form['bussinessRule']["activates_on"] = array(
    "#type" => "date_popup",
    "#date_format" => variable_get('date_format_short', 'm-d-Y H:i'),
    "#date_timezone" => variable_get('date_default_timezone', 0),
    /*"#date_increment" => 1,
    "#date_flexible" => 0,*/
    "#default_value" => date("Y-m-d H:i", $activates_on),
    '#suffix' => "</div>",
  );

  $has_expiration = isset($form_state["values"]["has_expiration"]) ? $form_state["values"]["has_expiration"] : FALSE;
  $form['bussinessRule']["has_expiration"] = array(
    "#prefix" =>'<div class="has-activation">',
    "#type" => "checkbox",
    "#title" => t("Discount Expires"),
    "#default_value" => $has_expiration,
    '#attributes' => array('onchange' => "$('#discount-expiration-wrapper').toggle();"),
  );

  $expiration = isset($form_state["values"]["expiration"]) ? $form_state["values"]["expiration"] : date();
  $form['bussinessRule']['bussinessRule']["expiration"] = array(
    "#type" => "date_popup",
    "#date_format" => variable_get('date_format_short', 'm-d-Y H:i'),
   // "#date_increment" => 1,
    //"#date_flexible" => 0,
    "#date_timezone" => variable_get('date_default_timezone', 0),
    "#title" => t("Discount Expiration") . sprintf("<span title='". t("This field is required.") ."' class='form-required'>*</span>"),
    "#default_value" => date("Y-m-d H:i", $expiration),
    '#suffix' => "</div>",   
  );
  
  /*$requires_code = isset($form_state["values"]["requires_code"]) ? $form_state["values"]["requires_code"] : FALSE;
  $form['bussinessRule']["qualifications"]["requires_code"] = array(
    "#prefix" =>'<div class="has-activation">',
  	"#type" => "checkbox",
    "#title" => t("Require code to activate discount."),
    "#default_value" => $requires_code,
  );*/  
  $form['bussinessRule'] = array(
    '#type' => 'fieldset',
    '#title' => t('LBL719'),
    '#collapsible' => TRUE,
  );
  $form['bussinessRule']["can_be_combined_with_other_discounts"] = array(
    "#prefix" =>'<div class="has-activation">',
    "#type" => "checkbox",
    "#title" => t("MSG618"),
    "#default_value" => isset($form_state["values"]["can_be_combined_with_other_discounts"]) ? $form_state["values"]["can_be_combined_with_other_discounts"] : FALSE,
    "#suffix" => "</div>",
  );
  // Hidden date fields for temporary
  //$form['bussinessRule'] = array();

  $form['qualification'] = array(
    '#type' => 'fieldset',
    '#title' => t('Qualification'),
    '#collapsible' => TRUE,
  );

  $form['qualification']["qualifications_qualifying_type"] = array(
    "#type" => "select",
    "#title" => t("Qualification Type"),
    "#options" => qualifying_type_options(),
    "#default_value" => isset($form_state["values"]["qualifying_type"]) ? $form_state["values"]["qualifying_type"] : '',
  );

  
  $form['qualification']["qualifications_qualifying_amount"] = array(
    "#type" => "textfield",
    "#title" => t("Minimum Qualification Total"),
    "#default_value" => isset($form_state["values"]["qualifying_amount"]) ? $form_state["values"]["qualifying_amount"] : '',
    "#size" => 15,
    "#required" => TRUE,
  );

  $form['qualification']["qualifications_qualifying_amount_max"] = array(
    "#type" => "textfield",
    "#title" => t("Maximum Qualifying Amount"),
    "#default_value" => isset($form_state["values"]["qualifying_amount_max"]) ? $form_state["values"]["qualifying_amount_max"] : '',
    "#size" => 15,
    "#required" => TRUE,
  );
  
  $codes_container_id = "codes-container-". $seed;
  $codes_prefix = sprintf("<div id='%s' onclick='if ( jQuery(\"#%s input[type=\\\"checkbox\\\"]\").attr(\"checked\") )" . "{ jQuery(\"#%s .codes-container\").slideDown(); }" . "else { jQuery(\"#%s .codes-container\").slideUp();} '>",
  $codes_container_id, $codes_container_id, $codes_container_id, $codes_container_id, $codes_container_id, $codes_container_id
  );

  //Create SKUs form element
  $options          = array();
  $result           = db_query("SELECT p.model, n.title FROM {uc_products} p, {node} n WHERE p.nid=n.nid ORDER BY n.title");
  $options['']      = t("No specific product required.");
  foreach ($result as $res) { $options[$res->model] = "#$res->model: $res->title"; }

  $form['maxuses'] = array(
    '#type' => 'fieldset',
    '#title' => t('Max Uses'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['maxuses']["discount_set_max_uses"] = array(
    "#type" => "textfield",
    "#title" => t("Max Uses"),
    "#default_value" => isset($form_state["values"]["max_uses"]) ? $form_state["values"]["max_uses"] : 0,
    "#size" => 7,
    "#required" => TRUE,
  );

  $form['maxuses']["discount_set_max_uses_per_user"] = array(
    "#type" => "textfield",
    "#title" => t("Max Uses Per User"),
    "#default_value" => isset($form_state["values"]["max_uses_per_user"]) ? $form_state["values"]["max_uses_per_user"] : 0,
    "#size" => 7,
    "#required" => TRUE,
  );

  $form['maxuses']["discount_set_max_uses_per_code"] = array(
    "#type" => "textfield",
    "#title" => t("Max Uses Per Code"),
    "#default_value" => isset($form_state["values"]["max_uses_per_code"]) ? $form_state["values"]["max_uses_per_code"] : 0,
    "#size" => 7,
    "#required" => TRUE,
  );

  $has_role_filter = isset($form_state["values"]["has_role_filter"]) ? $form_state["values"]["has_role_filter"] : FALSE;
  $userRollId = db_query("SELECT id FROM slt_profile_list_items WHERE CODE=:usertype",array(":usertype" => "cre_usr_ptp"))->fetchField();
  $rolesQry = db_query("select * from slt_profile_list_items where parent_id=:parent_id",array(":parent_id" => $userRollId));
  $rolesArr = Array(100 => t('Any'));  
  foreach($rolesQry as $rolesRs) {
    $rolesArr[$rolesRs->id] = $rolesRs->name;
  }

  $rcodes = array();
  if(!empty($discount_id)) {
    $query = db_select("uc_discounts_roles",'d')
    ->fields('d', array('role_id'))
    ->condition('d.discount_id',$discount_id,'=');
    $result = $query->execute();

    foreach ($result as $res) {
      $rcodes[] = $res->role_id;
    }
  }

  $form['assign'] = array(
    '#type' => 'fieldset',
    '#title' => t('Assign'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['assign']["qualifications_assign_roles"] = array(
  	"#prefix" =>'<div class="has-assign-role">',	
    "#type" => "checkboxes",
    "#title" => t("LBL173").':',
    "#default_value" => $rcodes,
    '#options' => $rolesArr,
  	"#required" => TRUE,
  );

  //Get current discount roles
  $role_ids = null;
  if (isset($form_state["values"]["role_ids"])) {
    $role_ids = $form_state["values"]["role_ids"];
  }
  else if ($is_edit) {
    $role_ids = get_role_ids_for_discount($discount_id, FALSE);
  }

  $rterms = array();
  if(!empty($discount_id)) {
    $query = db_select("uc_discounts_terms",'d')
    ->fields('d', array('term_id'))
    ->condition('d.discount_id',$discount_id,'=');
    $result = $query->execute();

    foreach ($result as $res) {
      $rterms[] = $res->term_id;
    }
  }

  //Get current discount terms
  $term_ids = null;
  if (isset($form_state["values"]["term_ids"])) {
    $term_ids = $form_state["values"]["term_ids"];
  }
  elseif ($is_edit) {  $term_ids = get_term_ids_for_discount($discount_id, FALSE);}
      
  $orgQry = db_select('taxonomy_term_data', 't');
  $orgQry->Join('taxonomy_vocabulary', 'v', 'v.vid = t.vid');
  $orgQry->Join('slt_organization', 'o', 'o.id = t.description');
  $orgQry->fields('t', array('tid', 'name'))
  ->condition('v.machine_name', 'organization_nav_vocabulary', '=')
  ->condition('o.status', 'cre_org_sts_act', '=');
  
  $result = $orgQry->execute();

  $orgArr = array();
  foreach($result as $orgInfo) {
    $orgArr[$orgInfo->tid] = $orgInfo->name;
  }

  /*$courseQry = db_select('taxonomy_term_data', 't');
  $courseQry->Join('taxonomy_vocabulary', 'v', 'v.vid = t.vid');
  $courseQry->fields('t', array('tid', 'name'))
  ->fields('t', array('tid','name'))
  ->condition('v.name', 'Course Catalog', '=');

   
  $result   = $courseQry->execute();
  $courseArr = array();
  foreach($result as $oClassInfo) {
    $courseArr[$oClassInfo->tid] = $oClassInfo->name;
  }*/

  $form['new_user']['#prefix'] = '<div class="req-pwd-form-wrapper">';
   
  $org_set_arr = array_intersect(array_keys($orgArr),$rterms);
  if(isset($_POST["organization_set"]) && count($_POST["organization_set"])>0) {
    $org_set_arr = $_POST["organization_set"];    
  }
     
  $form['assign']['organization_set'] = array(
    '#type' => 'multiselect',
    '#title' => t('Organization(s):'),
    '#options' => $orgArr, 
    '#default_value' => ((count($org_set_arr) > 0) ? $org_set_arr : ''),//_multiselect_fapi_example_options(),
    '#multiple' => TRUE,
    '#size' => 15,
    "#required" => FALSE,
  );

  /*$form['course'] = array(
    '#type' => 'fieldset',
    '#title' => t('Course'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  
  $course_set_arr = array_intersect(array_keys($courseArr),$rterms);
  if(isset($_POST["course_set"]) && count($_POST["course_set"])>0) {
    $course_set_arr = $_POST["course_set"];    
  }
  
  $form['course']['course_set'] = array(
    '#type' => 'multiselect',
    '#title' => t('Training Entity:'),
    '#options' => $courseArr,
    '#default_value' => ((count($course_set_arr) > 0) ? $course_set_arr : ''),//_multiselect_fapi_example_options(),
    '#multiple' => TRUE,
    '#size' => 15,   
  );*/

  $form['class'] = array(
    '#type' => 'fieldset',
    '#title' => t('Class'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $delivery_types = array();
  $vid = db_query("select vid FROM taxonomy_vocabulary WHERE machine_name = 'catalog'")->fetchField();
  $delivery_types = db_query("select description, name FROM taxonomy_term_data 
  WHERE description like 'lrn_cls_dty_%' and vid = ".$vid)->fetchAllKeyed();
  foreach($delivery_types as $key => $val) {
    $default_delivery = $key;
    break;
  } 
//print_r($delivery_types);
  $form['class']["training_entities_class"] = array(
    '#prefix' =>'<div class="class-training-entities">',
    '#type' => 'radios',
    //'#title' => 'Class',  	  
    '#options' => $delivery_types,
    '#default_value' => $default_delivery,
    '#attributes' => array('onchange' => 'var currVal = $(this).val();$(".class-training-entities").parent().find(".form-type-multiselect").hide();$(".class-training-entities").parent().find(".form-item-"+currVal.replace(/_/gi,\'-\')).show();'),
    '#suffix' =>'</div>',
  );
foreach($delivery_types as $key => $val) {
  $terms = uc_get_terms($key);
  //print_r($terms);
  $set_arr = array();
  $set_arr = array_intersect(array_keys($terms),$rterms);
  if(isset($_POST[$key]) && count($_POST[$key])>0) {
      expDebug::dPrint('post key ' .print_r($_POST[$key],true), 4);
    $set_arr = $_POST[$key];    
  }
  $form['class'][$key] = array(
    '#type' => 'multiselect',
    //'#title' => t('Training Entity'),
    '#options' => $terms,
    '#default_value' => ((count($set_arr) > 0) ? $set_arr : ''),
    '#multiple' => TRUE,
    '#size' => 15,
  );
  
}

$form['tpplan'] = array(
    '#type' => 'fieldset',
    '#title' => t('Training Plan'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

 $string = 'lrn_lpn_sts_atv';
  $tpQry = db_select("slt_program",'d')
    ->fields('d', array('id','title'))
    ->condition('d.status','lrn_lpn_sts_atv','=');
    //->condition('d.status','%'.db_like('lrn_lpn_sts_atv').'%','LIKE');
    //->condition('d.status','lrn_lpn_sts_atv','=');
    $result = $tpQry->execute();
    

  $tp_key = 'cre_sys_obt_crt';
  $tpArr    = array();
  foreach($result as $tpInfo) {
    //echo "<pre>"; print_r($tpInfo); echo "</pre>";
    $tpArr[$tpInfo->id] = $tpInfo->title;
  }
  
  $tpRadioOpt = array('cre_sys_obt_crt' => 'Certification','cre_sys_obt_trn' => 'Training Program','cre_sys_obt_cur' => 'Curricula');
    
  $delivery_types = array();
  $tvid = 5;
  $vid = db_query("select vid FROM taxonomy_vocabulary WHERE machine_name = 'tp_catalog_nav_vocabulary'")->fetchField();
  $delivery_types = db_query("select description, name FROM taxonomy_term_data 
  WHERE description like 'cre_sys_obt_%' and vid = ".$tvid)->fetchAllKeyed();
  foreach($delivery_types as $key => $val) {
    $default_delivery = $key;
    break;
  } 
  //print_r($tpRadioOpt);
  
  
  
  
  $form['tpplan']['tpopt'] = array(  
  //$form['tpplan']["training_entities_tp"] = array(
    '#prefix' =>'<div class="tp-training-entities">',
    '#type' => 'radios',
    //'#title' => 'Class',  	  
    '#options' => $delivery_types,
    '#default_value' => $default_delivery,
    '#attributes' => array('onchange' => 'var currVal = $(this).val();$(".tp-training-entities").parent().find(".form-type-multiselect").hide();$(".tp-training-entities").parent().find(".form-item-"+currVal.replace(/_/gi,\'-\')).show();'),
    '#suffix' =>'</div>',
    /*'#ajax' => array(
  			'event' => 'click',
            'callback' => 'discount_ajax_callback',
            'wrapper' => 'tp_dropdown_replace',
            ),*/
  );
  
  
  foreach($delivery_types as $key => $val) {
  $terms = uc_get_tp_terms($key);
  //print_r($terms);
  expDebug::dPrint(' TP Terms = ' . print_r($terms, true),4);
  $set_arr = array();
  $set_arr = array_intersect(array_keys($terms),$rterms);
  if(isset($_POST[$key]) && count($_POST[$key])>0) {
    $set_arr = $_POST[$key];    
  }
  
  $form['tpplan'][$key] = array(
    '#type' => 'multiselect',
    '#prefix' =>'<div style="clear:both;">',
    '#suffix' =>'</div>',
    //'#title' => t('Training Entity'),
    '#options' => $terms,
    '#default_value' => ((count($set_arr) > 0) ? $set_arr : ''),
    '#multiple' => TRUE,
    '#size' => 15,
  );
 

}
  /*$form['class']['discount_wbt_set'] = array(
    '#type' => 'multiselect',
    '#title' => t('Training Entity'),
    '#options' => $wbtTerms,
    '#default_value' => ((count($wbt_set_arr) > 0) ? $wbt_set_arr : ''),
    '#multiple' => TRUE,
    '#size' => 15,
  );*/

  /*$form['class']['discount_ilt_set'] = array(
    '#type' => 'multiselect',
    '#title' => t('Training Entity'),
    '#options' => $iltTerms,
    '#default_value' => ((count($ilt_set_arr) > 0) ? $ilt_set_arr : ''),
    '#multiple' => TRUE,
    '#size' => 15,
  );

  $form['class']['discount_vc_set'] = array(
    '#type' => 'multiselect',
    '#title' => t('Training Entity'),
    '#options' => $vclTerms,
    '#default_value' => ((count($vc_set_arr) > 0) ? $vc_set_arr : ''),
    '#multiple' => TRUE,
    '#size' => 15,
  );*/
  	
  $form["submit"] = array(
    "#type" => "submit", 
    '#prefix' => '<div class="form-actions">',
    '#suffix' => '</div></div>',
    "#value" => t("Submit")
  );

  drupal_add_js (
          '$(document).ready(function(){$(".class-training-entities").parent().find(".form-type-multiselect").hide();$(".class-training-entities").parent().find(".form-type-multiselect").each(function(index){if(index==0){$(this).show();}})
           $(".tp-training-entities").parent().find(".form-type-multiselect").hide();$(".tp-training-entities").parent().find(".form-type-multiselect").each(function(index){if(index==0){$(this).show();}
			});$(".form-item-organization-set > label").html("Organization(s): <span title=\"This field is required.\" class=\"form-required\">*</span>");
           			
      });','inline');
  
  return $form;
}

function discount_ajax_callback($form, $form_state) {
  return $form['tpplan']['tp_key'];
}


/**
 * Select terms
 */
function uc_get_terms($dtType) {
  $parentDeliveryId = db_query("SELECT tid FROM taxonomy_term_data WHERE NAME = '--All Delivery types--'")->fetchField();
  $iltQry = db_select('taxonomy_term_data', 'td');
  $iltQry->join('taxonomy_term_hierarchy','th','td.tid = th.parent OR(td.tid = th.tid AND th.parent ='.$parentDeliveryId.')');
  $iltQry->join('taxonomy_term_data','td1','th.tid = td1.tid');
  $iltQry->join('slt_course_class','cl','cl.id = td1.description OR td1.description = \''.$dtType.'\'');
  $iltQry->join('slt_course_template','cr','cr.id = cl.course_id AND cr.status = \'lrn_crs_sts_atv\'');
  $iltQry->fields('td1',array('tid','name'));
  $iltQry->condition('td.description',$dtType,'=');
  $iltQry->condition('cl.status','lrn_cls_sts_atv','=');
  //Added for #0021922: Discount creation trainings display 
  $iltQry->condition('cl.price',0,'>');  
  $iltQry->groupBy('td1.tid');
  expDebug::dPrintDBAPI(' uc_get_terms = ' ,$iltQry);
  $iltRs   = $iltQry->execute();
  $dtArr   = Array();
  $deliveryTypeName = db_query('SELECT name FROM slt_profile_list_items WHERE code = \''.$dtType.'\'')->fetchField();
  foreach($iltRs as $iltRec) {
    if($iltRec->name == $deliveryTypeName){
      $iltRec->name = 'Any '.$iltRec->name;
    }
    $dtArr[$iltRec->tid] = $iltRec->name;
  }
  return $dtArr;
}


/**
 * Select terms
 */
function uc_get_tp_terms($dtType) {
  $parentDeliveryId = db_query("SELECT tid FROM taxonomy_term_data WHERE NAME = '--All Tp--'")->fetchField();
  $iltQry = db_select('taxonomy_term_data', 'td');
  $iltQry->join('taxonomy_term_hierarchy','th','td.tid = th.parent OR(td.tid = th.tid AND th.parent ='.$parentDeliveryId.')');
  $iltQry->join('taxonomy_term_data','td1','th.tid = td1.tid');
  $iltQry->join('slt_program','prg','prg.id = td1.description OR td1.description = \''.$dtType.'\'');
  $iltQry->fields('td1',array('tid','name'));
  $iltQry->condition('td.description',$dtType,'=');
  $iltQry->condition('prg.status','lrn_lpn_sts_atv','=');
  //Added for #0021922: Discount creation trainings display 
  $iltQry->condition('prg.price',0,'>');  
  $iltQry->groupBy('td1.tid');
  expDebug::dPrintDBAPI(' uc_get_tp_terms = ' ,$iltQry);
  $iltRs   = $iltQry->execute();
  $dtArr   = Array();
  $deliveryTypeName = db_query('SELECT name FROM slt_profile_list_items WHERE code = \''.$dtType.'\'')->fetchField();
  foreach($iltRs as $iltRec) {
    if($iltRec->name == $deliveryTypeName){
      $iltRec->name = 'Any '.$iltRec->name;
    }
    $dtArr[$iltRec->tid] = $iltRec->name;
  }
  return $dtArr;
}
/**
 * Validate handler for uc_discounts_form().
 */
function uc_discounts_form_validate($form, &$form_state) {
  $values = $form_state["values"];

  $has_errors = FALSE;
  /*if ($values["requires_code"] && empty($values["codes"])) {
    form_set_error("codes", t("Discounts codes are required because 'Require(s) code' is checked"));
    $has_errors = TRUE;
  }*/

  //Check qualifying_amount_max (if necessary)
  //if ($values["has_qualifying_amount_max"]) {
    if (($index = strpos($values["qualifications_qualifying_amount_max"], "%")) !== FALSE) {
      $value = substr($values["qualifications_qualifying_amount_max"], 0, $index);
    }
    else $value = $values["qualifications_qualifying_amount_max"];

    if (!is_numeric($value)) {
      form_set_error("qualifications_qualifying_amount_max", t("Max qualifying amount is required."));
      $has_errors = TRUE;
    }
  //}

  //Check discount_amount
  if (($index = strpos($values["discount_amount"], "%")) !== FALSE) {
    $value = substr($values["discount_amount"], 0, $index);
  }
  else $value = $values["discount_amount"];

  if (!is_numeric($value) || strstr($value,'-') ) {
    form_set_error("discount_amount", t("Discount amount must be integer, decimal or percentage."));
    $has_errors = TRUE;
  }
  if (empty($_POST["organization_set"])) {
    form_set_error("term_ids", t("Organizations are required"));
    $has_errors = TRUE;
  }
  if($values["has_activation"] && empty($values["activates_on"])){
    form_set_error("activation", t("Activation date are required because 'Discount activation date' is checked"));
    $has_errors = TRUE;
  }
  if ($values["has_expiration"] && empty($values["expiration"])) {
    form_set_error("expiration", t("Expiration date are required because 'Discount expires' is checked"));
    $has_errors = TRUE;
  }
  if($values["has_activation"] && $values["has_expiration"]){
    if(strtotime($values["activates_on"]) > strtotime($values["expiration"])){
         form_set_error("date_expire", t("Discount expires date cannot be less than the Discount activation date."));
    }
  }
    $vid = db_query("select vid FROM taxonomy_vocabulary WHERE machine_name = 'catalog'")->fetchField();
    $delivery_types = db_query("select description, name FROM taxonomy_term_data 
    WHERE description like 'lrn_cls_dty_%' and vid = ".$vid)->fetchAllKeyed();
    $class_list = array();
    foreach($delivery_types as $key => $val) {
      expDebug::dPrint('Class List deltype:' . print_r($form_state["values"]['lrn_cls_dty_ilt'],true) , 4);
      if(isset($_POST[$key])){
        if(is_array($form_state["values"][$key])) {
          foreach($form_state["values"][$key] as $tid)
            $class_list[] = $tid;
        }
      }
    } 
   expDebug::dPrint('Class List :' . print_r($class_list,true) , 4);
   $tpVid = db_query("select vid FROM taxonomy_vocabulary WHERE machine_name = 'tp_catalog_nav_vocabulary'")->fetchField();
   $tp_delivery_types = db_query("select description, name FROM taxonomy_term_data WHERE description like 'cre_sys_obt_%' and vid = ".$tpVid)->fetchAllKeyed();
    $tp_class_list = array();
    foreach($tp_delivery_types as $key => $val) {
      if(isset($_POST[$key])){
        if(is_array($form_state["values"][$key])) {
          foreach($form_state["values"][$key] as $tpid)
            $tp_class_list[] = $tpid;
        }
      }
    }
  if (empty($class_list) &&  empty($tp_class_list)) {
    form_set_error("term_ids", t("Class or Training Plan needs to be selected"));
    $has_errors = TRUE;
  }
  /*$vid = db_query("select vid FROM taxonomy_vocabulary WHERE machine_name = 'catalog'")->fetchField();
  $delivery_types = db_query("select description, name FROM taxonomy_term_data 
  WHERE description like 'lrn_cls_dty_%' and vid = ".$vid)->fetchAllKeyed();
  $class_list = array();
  foreach($delivery_types as $key => $val) {
    if(is_array($values[$key])) {
      foreach($values[$key] as $tid)
        $class_list[] = $tid;
    }
  } 
  if (empty($class_list)) {
    form_set_error("term_ids", t("Training entites are required"));
    $has_errors = TRUE;
  }*/
  /*if ($values["filter_type"] == FILTER_TYPE_PRODUCTS) {
   if (empty($values["product_ids"])) {
   form_set_error("product_ids", t("Products are required because of 'Filter Type' value"));
   $has_errors = TRUE;
   }
   }
   elseif ($values["filter_type"] == FILTER_TYPE_TERMS) {
   if (empty($values["term_ids"])) {
   form_set_error("term_ids", t("Terms are required because of 'Filter Type' value"));
   $has_errors = TRUE;
   }*/
  /*if (empty($values["tree"]['selected_terms'])) {
   form_set_error("term_ids", t("Terms are required because of 'Filter Type' value"));
   $has_errors = TRUE;
   }*/
  /*}
   elseif ($values["filter_type"] == FILTER_TYPE_SKUS) {
   if (empty($values["skus"])) {
   form_set_error("skus", t("SKUs are required because of 'Filter Type' value"));
   $has_errors = TRUE;
   }
   }
   elseif ($values["filter_type"] == FILTER_TYPE_CLASS) {
   if (empty($values["classes"])) {
   form_set_error("classes", t("Product classes are required because of 'Filter Type' value"));
   $has_errors = TRUE;
   }
   }
   else {
   form_set_error("filter_type", t("Invalid filter type value"));
   $has_errors = TRUE;
   }*/

  /*if ($values["has_role_filter"] && empty($values["role_ids"])) {
    form_set_error("roles", t("Roles are required because 'Filter based on roles' is checked"));
    $has_errors = TRUE;
  }*/

  /*if ($values["has_expiration"] && !date_is_valid($values["expiration"])) {
    form_set_error("expiration", t("Expiration are required because 'Discount expires' is checked"));
    $has_errors = TRUE;
  }*/

  //Form requires rebuilding form so codes and expiration blocks will display correctly
  if ($has_errors) {
    $form_state["rebuild"] = TRUE;
  }
}

/**
 * Submit handler for uc_discounts_form().
 */
function uc_discounts_form_submit($form, &$form_state) {
  $values = $form_state["values"];
  $form_state["redirect"] = "portalpages/sp_administrator/store-admin/uc_discounts";
  $op = isset($form_state["values"]["op"]) ? $form_state["values"]["op"] : "";
  
  if ($op == t("Submit")) {
    $codes = explode("\n", $form_state["values"]["discount_code"]);    
    //$tr_set = $form_state["values"]["training_entities_class"];
    $discount_set       = isset($_POST["organization_set"]) ? $_POST["organization_set"] : array();
    //$discount_set2      = isset($_POST["course_set"]) ? $_POST["course_set"] : array();    
    $vid = db_query("select vid FROM taxonomy_vocabulary WHERE machine_name = 'catalog'")->fetchField();
    $delivery_types = db_query("select description, name FROM taxonomy_term_data 
    WHERE description like 'lrn_cls_dty_%' and vid = ".$vid)->fetchAllKeyed();
    $class_list = array();
    foreach($delivery_types as $key => $val) {
      if(isset($_POST[$key])){
        if(is_array($form_state["values"][$key])) {
          foreach($form_state["values"][$key] as $tid)
            $class_list[] = $tid;
        }
      }
    } 
    
   $tpVid = db_query("select vid FROM taxonomy_vocabulary WHERE machine_name = 'tp_catalog_nav_vocabulary'")->fetchField();
   $tp_delivery_types = db_query("select description, name FROM taxonomy_term_data WHERE description like 'cre_sys_obt_%' and vid = ".$tpVid)->fetchAllKeyed();
    $tp_class_list = array();
    foreach($tp_delivery_types as $key => $val) {
      if(isset($_POST[$key])){
        if(is_array($form_state["values"][$key])) {
          foreach($form_state["values"][$key] as $tpid)
            $tp_class_list[] = $tpid;
        }
      }
    }
   
    
    //$term_ids = array_merge($discount_set,$discount_set2,$discount_wbt_set,$discount_ilt_set,$discount_vc_set);
    //$term_ids = array_merge($discount_set,$discount_set2,$class_list);
    $term_ids = array_merge($discount_set,$class_list,$tp_class_list);
    $term_ids = array_unique($term_ids);
    
    $qualifications_has_roles = $form_state["values"]["qualifications_assign_roles"];
    $roleIds = Array();
    foreach($qualifications_has_roles as $key=>$value) {
      if(strlen($value) > 1) {
        $roleIds[] = $key;
      }
    }

    $role_ids = $roleIds;//($form_state["values"]["has_role_filter"]) ? $form_state["values"]["role_ids"] : array();

    $expirationDateSplit  = explode(" ",$form_state["values"]["expiration"]);
    $expiration_day       = explode("-",$expirationDateSplit[0]);
    $expiration_time      = explode(":",$expirationDateSplit[1]);
    $eHour   = $expiration_time[0];
    $eMin    = !empty($expiration_time[1]) ? $expiration_time[1] : 0;
    $eDay    = $expiration_day[2];
    $eMonth  = $expiration_day[1];
    $eYear   = $expiration_day[0];

    $activationDateSplit  = explode(" ",$form_state["values"]["activates_on"]);
    $activation_day       = explode("-",$activationDateSplit[0]);
    $activation_time      = explode(":",$activationDateSplit[1]);
    $aHour   = $activation_time[0];
    $aMin    = !empty($activation_time[1]) ? $activation_time[1] : 0;
    $aDay    = $activation_day[2];
    $aMonth  = $activation_day[1];
    $aYear   = $activation_day[0];

    $expiration    = mktime($eHour,$eMin,0,$eMonth,$eDay,$eYear);
    $activates_on  = mktime($aHour,$aMin,0,$aMonth,$aDay,$aYear);

    //Set discount_amount
    if (($index = strpos($form_state["values"]["discount_amount"], "%")) !== FALSE) {
      $discount_amount = floatval(substr($form_state["values"]["discount_amount"], 0, $index)) / 100;
    }
    else {
      $discount_amount = floatval($form_state["values"]["discount_amount"]);
    }

    $discount = new stdClass();
    $discount->name = $form_state['values']['name'];
    $discount->short_description = 'Discount';//$form_state['values']['short_description'];
    $discount->description = $form_state['values']['description'];
    $discount->qualifying_type = $form_state['values']['qualifications_qualifying_type'];
    $discount->qualifying_amount = $form_state['values']['qualifications_qualifying_amount'];
    if(empty($form_state['values']['qualifications_qualifying_amount_max']))
      $discount->has_qualifying_amount_max = 0;
    else
      $discount->has_qualifying_amount_max = 1;//$form_state['values']['has_qualifying_amount_max'];
    $discount->qualifying_amount_max = $form_state['values']['qualifications_qualifying_amount_max'];// $form_state['values']['qualifying_amount_max'];
    $discount->discount_type = $form_state['values']['discount_type'];
    $discount->discount_amount = $discount_amount;
    //$discount->requires_code = $form_state['values']['requires_code'];
    $discount->requires_code = 1;
    //$discount->filter_type = $form_state['values']['filter_type'];
    $discount->filter_type = FILTER_TYPE_TERMS; // filter type is removed and terms are used.
    $discount->has_role_filter = 1;//$form_state['values']['has_role_filter'];
    $discount->use_only_discounted_products_to_qualify = 0;//$form_state['values']['use_only_discounted_products_to_qualify'];
    $discount->requires_single_product_to_qualify = 0;//$form_state['values']['requires_single_product_to_qualify'];
    $discount->required_product = '';//$form_state['values']['required_product'];
    $discount->max_times_applied = 0;//$form_state['values']['max_times_applied'];
    $discount->limit_max_times_applied = 0;//$form_state['values']['limit_max_times_applied'];
    $discount->can_be_combined_with_other_discounts = $form_state['values']['can_be_combined_with_other_discounts'];
    $discount->max_uses = $form_state['values']['discount_set_max_uses'];//$form_state['values']['max_uses'];
    $discount->max_uses_per_user = $form_state['values']['discount_set_max_uses_per_user'];//$form_state['values']['max_uses_per_user'];
    $discount->max_uses_per_code = $form_state['values']['discount_set_max_uses_per_code'];//$form_state['values']['max_uses_per_code'];
    $discount->has_expiration = $form_state['values']['has_expiration'];
    $discount->expiration = $expiration;
    $discount->has_activation = $form_state['values']['has_activation'];
    $discount->activates_on = $activates_on;
    $discount->is_active = $form_state['values']['is_active'];
    $discount->weight = $form_state['values']['weight'];
    if (empty($form_state['values']['discount_id'])) {
      $discount->insert_timestamp = time();
      drupal_write_record('uc_discounts', $discount);      
    }
    else {
      $discount->discount_id = $form_state["values"]["discount_id"];
      drupal_write_record('uc_discounts', $discount, 'discount_id');

      //Delete existing related fields
      uc_discounts_codes_delete($discount->discount_id);
      //uc_discounts_products_delete($discount->discount_id);
      uc_discounts_terms_delete($discount->discount_id);
      //uc_discounts_skus_delete($discount->discount_id);
      //uc_discounts_classes_delete($discount->discount_id);
      uc_discounts_roles_delete($discount->discount_id);
    }

    //Insert codes (if necessary)
    if (!empty($codes)) {
      foreach ($codes as $code) {
        $code = trim($code);
        if (empty($code)) {
          continue;
        }
        $new_code = array('discount_id' => $discount->discount_id, 'code' => $code);
        $qryCodes = db_insert('uc_discounts_codes')
        ->fields($new_code);
        $qryCodes->execute();
        //drupal_write_record('uc_discounts_codes', $new_code);
      }
    }

    foreach ($term_ids as $term_id) {
      if (empty($term_id)) {
        continue;
      }
      $new_term = array('discount_id' => $discount->discount_id, 'term_id' => $term_id);
      $qryTerms = db_insert('uc_discounts_terms')
        ->fields($new_term);
        $qryTerms->execute();
      //print_r($new_term);
      //drupal_write_record('uc_discounts_terms', $new_term);
   }  

    //Insert roles
    if (count($role_ids)>0) {
      foreach ($role_ids as $role_id) {
        $new_role = array('discount_id' => $discount->discount_id, 'role_id' => $role_id);
        //drupal_write_record('uc_discounts_roles', $new_role);
        $qryRoles = db_insert('uc_discounts_roles')
        ->fields($new_role);
        $qryRoles->execute();
      }
    }
    drupal_set_message(t("%name settings have been saved.", array("%name" => $form_state["values"]["name"])));
  }
}

/**
 * Delete a discount.
 *
 * @ingroup forms
 * @see uc_discounts_delete_submit()
 */
function uc_discounts_delete($form_state, $discount_id) {
  $discount = db_fetch_object(db_query("SELECT * FROM {uc_discounts} WHERE discount_id=%d", $discount_id));
  $form     = array("discount_id" => array("#type" => "value", "#value" => $discount_id));
  $output   = confirm_form($form, t("Delete @row?", array("@row" => $discount->name)), 'portalpages/sp_administrator/store-admin/uc_discounts');
  return $output;
}

/**
 * Submit handler for uc_discounts_delete().
 */
function uc_discounts_delete_submit($form, &$form_state) {
  if ($form_state["values"]["confirm"]) {
    $discount_id = $form_state["values"]["discount_id"];
    uc_discounts_delete_all($discount_id);
  }

  drupal_set_message(t("Discount deleted."));
  $form_state["redirect"] = "portalpages/sp_administrator/store-admin/uc_discounts";
}

/**
 * Generate report for all discounts.
 */
function uc_discounts_report() {
  $op = arg(4) == 'download' ? 'download' : 'show';
  $header = array(
  array('data' => t('Active'), 'field' => 'd.is_active'),
  array('data' => t('Name'), 'field' => 'd.name'),
  array('data' => t('Short Description'), 'field' => 'd.short_description'),
  array('data' => t('Qualifying Type'), 'field' => 'd.qualifying_type'),
  array('data' => t('Type'), 'field' => 'd.discount_type'),
  array('data' => t('Uses'), 'field' => 'use_count'),
  array('data' => t('Times Applied'), 'field' => 'total_times_applied'),
  array('data' => t('Discounted Amount'), 'field' => 'total_amount'),
  array('data' => t('Revenue Amount'), 'field' => 'total_revenue'),
  array('data' => t('Weight'), 'field' => 'd.weight'),
  array('data' => t('Created At'), 'field' => 'insert_timestamp', 'sort' => 'desc'),
  array('data' => t('Operations')),
  );

  $query     = "SELECT d.*,
                       COUNT(du.discount_use_id) total_use_count,
                       SUM(du.times_applied) total_times_applied,
                       SUM(du.amount) total_amount,
                       SUM(o.order_total) total_revenue
                FROM {uc_discounts} d
                LEFT JOIN {uc_discounts_uses} du ON d.discount_id=du.discount_id
                LEFT JOIN {uc_orders} o ON du.order_id=o.order_id AND o.order_status = 'completed' AND o.order_total > 0
                GROUP BY d.discount_id";
  $query .= tablesort_sql($header);
  $count_sql = 'SELECT COUNT(*) FROM {uc_discounts}';
  $result = $op == 'download' ? db_query($query) : pager_query($query, 50, 0, $count_sql);

  $rows = array();
  while ($discount = db_fetch_object($result)) {
    $total_use_count = (is_numeric($discount->total_use_count)) ? $discount->total_use_count : 0;
    $total_times_applied = (is_numeric($discount->total_times_applied)) ? $discount->total_times_applied : 0;
    $operations = array(
    l(t('usage'), 'portalpages/sp_administrator/reports/uc_discounts/discount/'. $discount->discount_id),
    l(t('edit'), 'portalpages/sp_administrator/store-admin/uc_discounts/edit/'. $discount->discount_id),
    l(t('delete'), 'portalpages/sp_administrator/store-admin/uc_discounts/delete/'. $discount->discount_id),
    );
   
    $rows[] = array(
    array('data' => ($discount->is_active ? '&#10003;' : 'X'), 'class' => ($discount->is_active ? 'is_active' : 'is_inactive')),
    $discount->name,
    $discount->short_description,
    qualifying_type_name($discount->qualifying_type),
    discount_type_name($discount->discount_type),
    array('data' => $total_use_count, 'class' => 'numeric'),
    array('data' => $total_times_applied, 'class' => 'numeric'),
    uc_currency_format($discount->total_amount),
    uc_currency_format($discount->total_revenue),
    array('data' => $discount->weight, 'class' => 'weight'),
    array('data' => format_date($discount->insert_timestamp, 'small'), 'class' => 'date'),
    array('data' => implode(' ', $operations), 'class' => 'operations'),
    );
  }

  if ($op == 'download') {
    $http_headers = array(
      'Pragma: no-cache',
      'Expires: 0',
      'Cache-Control: no-cache, must-revalidate',
      'Cache-Control: private',
      'Content-Transfer-Encoding: binary',
      'Content-Disposition: attachment; filename="discount_usage_report.csv"',
      'Content-Type: text/csv'
      );
      foreach ($http_headers as $http_header) {
        $http_header = preg_replace('/\r?\n(?!\t| )/', '', $http_header);
        drupal_set_header($http_header);
      }

      $fp = fopen('php://output', 'w');
      array_unshift($rows, $header);
      foreach ($rows as $key => $row) {
        $csv_row = array();
        // remove Operations cell
        array_pop($row);
        foreach ($row as $cell) {
          $csv_row[] = is_array($cell) ? $cell['data'] : $cell;
        }
        if ($key > 0) {
          // make is_active CSV friendly
          $csv_row[0] = $csv_row[0] == 'X' ? 0 : 1;
        }
        fputcsv($fp, $csv_row);
      }
      fclose($fp);
      exit;
  }
  else {
    if (empty($rows)) {
      $rows[] = array(array('data' => t('No discounts.'), 'colspan' => count($header)));
    }

    $output .= theme('table', $header, $rows, array('class' => 'uc-discounts'));
    $output .= theme('pager', NULL, 50, 0);
    $output .= l(t('Export to CSV'), 'admin/reports/uc_discounts/all/download');
    return $output;
  }
}

/**
 * Generate report for discount.
 */
function uc_discounts_report_for_discount($discount_id) {
  //Add link to all discount data
  $output = l(t("Report for all discounts."), "admin/reports/uc_discounts/all");

  $output .= "<br/><br/>";


  //Add table for discount's data

  $header = array(
  t("Name"),
  t("Uses"),
  t("Times Applied"),
  t("Discounted Amount"),
  t("Revenue Amount"),
  array("data" => t("Operations"), "colspan" => 3),
  );

  $query               = "SELECT d.*, COUNT(du.discount_use_id) total_use_count,
                            SUM(du.times_applied) total_times_applied, SUM(du.amount) total_amount, SUM(o.order_total) total_revenue
                          FROM {uc_discounts} d
                          LEFT JOIN {uc_discounts_uses} du ON d.discount_id=du.discount_id
                          LEFT JOIN {uc_orders} o ON du.order_id=o.order_id AND o.order_status = 'completed' AND o.order_total > 0
                          WHERE d.discount_id=%d
                          GROUP BY d.discount_id";
  $discount            = db_fetch_object(db_query($query, $discount_id));
  $total_use_count     = (is_numeric($discount->total_use_count)) ? $discount->total_use_count : 0;
  $total_times_applied = (is_numeric($discount->total_times_applied)) ? $discount->total_times_applied : 0;
  $discounts[]         = array(
  $discount->name,
  $total_use_count,
  $total_times_applied,
  uc_currency_format($discount->total_amount),
  uc_currency_format($discount->total_revenue),
  l(t("edit"), "portalpages/sp_administrator/store-admin/uc_discounts/edit/". $discount->discount_id),
  l(t("copy"), "portalpages/sp_administrator/store-admin/uc_discounts/copy/". $discount->discount_id),
  l(t("delete"), "portalpages/sp_administrator/store-admin/uc_discounts/delete/". $discount->discount_id),
  );
  $output .= theme("table", $header, $discounts);

  $output .= "<br/><br/>";


  //Add table of discount's usage data

  $header = array(
  array("data" => t("User"), "field" => "user_id"),
  array("data" => t("Order"), "field" => "order_id"),
  array("data" => t("Code"), "field" => "code"),
  array("data" => t("Times Applied"), "field" => "times_applied"),
  array("data" => t("Amount"), "field" => "amount"),
  array("data" => t("Date"), "field" => "insert_timestamp", "sort" => "asc"),
  );

  $query = sprintf("SELECT du.*, u.uid user_id, u.name username, u.mail email FROM {uc_discounts_uses} du" . " LEFT JOIN {users} u ON du.user_id=u.uid" . " WHERE discount_id=%d", $discount_id
  );
  $tablesort = tablesort_sql($header);
  $result = pager_query($query . $tablesort, 50);

  $rows = array();
  while ($use = db_fetch_object($result)) {
    $user_description = ($use->user_id != 0) ? $use->username ." (". $use->email .")" : t("Anonymous");

    $rows[] = array("data" =>
    //Cell data
    array(
    $user_description,
    $use->order_id,
    (!empty($use->code)) ? $use->code : check_plain("<". t("no code") .">"),
    $use->times_applied,
    uc_currency_format($use->amount),
    date("Y-m-d H:i:s", $use->insert_timestamp),
    ),
    );
  }

  if (empty($rows)) {

    $rows[] = array(array("data" => t("No discount data available."), "colspan" => 11));

  }

  $output .= theme("table", $header, $rows, array("id" => "uc_discounts_report"));
  $output .= theme("pager", NULL, 50, 0);


  return $output;
}

/**
 * Allows admin to generate a list of unique codes that can be used for things like mail blasts
 */
function uc_discounts_generate_codes(&$form_state, $discount_id) {
}

/**
 * @see uc_discounts_generate_codes()
 */
function uc_discounts_generate_codes_form(&$form_state, $discount_id) {
  $form['about'] = array(
    '#value' => '<p>' . t('Generate unqiue codes for this discount.  After generating them simply copy and paste them into the Codes textarea for your discount.') . '</p>',
  );
  $form['num'] = array(
    '#title' => t('Number of codes'),
    '#type' => 'textfield',
    '#default_value' => $form_state['values']['num'],
    '#required' => TRUE,
    '#size' => 5,
  );
  $form['pattern'] = array(
    '#title' => t('Code pattern'),
    '#description' => t('The pattern for generating the codes.  %d will be replaced with a random digit, %s will be replaced with a random letter.'),
    '#default_value' => empty($form_state['values']['pattern']) ? 'code%d%s%d' : $form_state['values']['pattern'],
    '#type' => 'textfield',
    '#required' => TRUE,
    '#size' => 15,
  );
  $form['discount_id'] = array('#type' => 'value', '#value' => $discount_id);
  $form['submit'] = array(
    '#value' => t('Generate codes'),
    '#type' => 'submit',
  );

  if (!empty($form_state['storage']['codes'])) {
    $form['codes'] = array(
      '#value' => "<p><em>" . t('Copy these codes into the Codes textarea for your discount. Note that this list also contains any existing codes for the discount.') . "</em></p>
                   <code class=\"uc-discounts-generated-codes\">{$form_state['storage']['codes']}</code>",
    );
  }

  return $form;
}

function uc_discounts_generate_codes_form_validate($form, &$form_state) {
  if (!is_numeric($form_state['values']['num']) || $form_state['values']['num'] <= 0) {
    form_set_error('num', t('Number of discounts must be a number greater than 0.'));
  }

  if (substr_count($form_state['values']['pattern'], '%') < 2) {
    form_set_error('pattern', t('Code pattern must have at least two random digits or letters to ensure uniqueness.'));
  }
}

function uc_discounts_generate_codes_form_submit($form, &$form_state) {
  $codes = get_codes_for_discount($form_state['values']['discount_id']);
  foreach(range(1, $form_state['values']['num']) as $num) {
    while (true) {
      $code = preg_replace_callback('/%(s|d)/', '_uc_discounts_get_random_string', $form_state['values']['pattern']);
      if (!in_array($code, $codes)) {
        $codes[] = $code;
        break;
      }
    }
  }

  $form_state['storage']['codes'] = implode("\n", $codes);
}

/**
 * Generates a randome digit or letter, used as a callback for preg_replace
 */
function _uc_discounts_get_random_string($matches) {
  // @see user_password
  $str = $matches[1] == 's' ? 'abcdefghijkmnopqrstuvwxyz' : '23456789';
  $str_len = strlen($str) - 1;
  return $str[mt_rand(0, $str_len)];
}

function discount_search_autocomplete($string){
  $string = trim($string);  
  $query = db_select("uc_discounts",'d')
  ->fields('d', array('name'));
  
  if(!empty($string)) {
    $query->condition('d.name','%'.db_like($string).'%','LIKE');
  }
  $result = $query->execute()->fetchAll();
  $suggestions = array();
  foreach ($result as $match) {
    $suggestions[$match->name] = $match->name;
  }
  drupal_json_output((object)$suggestions);
}
