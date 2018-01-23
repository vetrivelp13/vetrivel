<?php
define('DRUPAL_ROOT', getcwd());
include_once "./includes/common.inc";
include_once "./includes/database/database.inc";
include_once './includes/bootstrap.inc';
require_once DRUPAL_ROOT."/sites/all/services/Trace.php";
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

class DrupalcreateOrderService {
  public function __construct(){
  	try{
	    $this->truncateOrders();
	    $this->truncateOrdersLineItems();
 	    $this->truncateOrdersProducts();
  	}catch(Exception $e){
  		expDebug::dPrint($e);
  	}
  }
  public function process(){
  	try{    
	    $this->createUcOrder();
	    $this->updateSltOrderId();
	    $this->insertUcOrderLineItemTax();
	    $this->insertUcOrderProducts();
  	}catch(Exception $e){
  		expDebug::dPrint($e);
  	}
  }

  private function truncateOrders()
  {
  	try{
    	db_query("TRUNCATE TABLE uc_orders");
  	}catch(Exception $e){
  		expDebug::dPrint($e);
  	}
  }
  private function truncateOrdersLineItems()
  {
  	try{
    	db_query("TRUNCATE TABLE uc_order_line_items");
  	}catch(Exception $e){
  		expDebug::dPrint($e);
  	}
  }
  
  private function truncateOrdersProducts()
  {
  	try{
    	db_query("TRUNCATE TABLE uc_order_products");
  	}catch(Exception $e){
  		expDebug::dPrint($e);
  	}
  }
  

  function createUcOrder()
  {
  	try{
  		$qry = "INSERT INTO uc_orders (
					order_id
					,uid
					,order_status
					,order_total
					,primary_email
					,product_count
					,billing_first_name
					,billing_last_name
					,billing_phone
					,billing_street1
					,billing_street2
					,billing_city
					,billing_postal_code
					,billing_country
					,billing_zone
					,payment_method
					,created
					,modified
					,currency_type) 
				SELECT 
					o.id  
					,IFNULL(u.uid,0)
					,CASE WHEN o.order_status = 'cme_pmt_sts_rjt' THEN 'canceled' 
			            WHEN o.order_status = 'cme_pmt_sts_cnm'  THEN 'completed'
			            WHEN o.order_status = 'cme_pmt_sts_pnd' OR o.order_status = 'lrn_crs_reg_ppm'  THEN 'pending'
			            WHEN o.order_status = 'cme_pmt_sts_rsv' OR o.order_status = 'lrn_crs_reg_rsv'  THEN 'in_checkout'
			            WHEN o.order_status = 'lrn_crs_reg_rsc'  THEN 'canceled'
			            ELSE 'completed'
			          END  order_status
					,o.order_total_amt
					,ifNULL(u.mail,'')
					,COUNT(oi.order_id) cnt_order
					,IFNULL(o.bill_firstname,'')
					,IFNULL(o.bill_lastname,'')
					,IFNULL(o.bill_phone,'')
					,IFNULL(o.bill_add1,'')
					,IFNULL(o.bill_add2,'')
					,IFNULL(o.bill_city,'')
					,IFNULL(o.bill_postalcode,'')
					,IFNULL((SELECT country_id FROM uc_countries WHERE country_name = o.bill_country),0) bill_country
					,IFNULL((SELECT zo.zone_id FROM uc_zones zo,uc_countries cn 
						WHERE (zo.zone_code = o.bill_state OR zone_name = o.bill_state) 
						AND zo.zone_country_id = cn.country_id AND cn.country_name = o.bill_country),0) zone_id 
					,IF(o.order_total_amt = 0.00000, 'Zero Cost', IFNULL(o.order_type,'other')) order_type
					,UNIX_TIMESTAMP(o.created_on) create_date
					,UNIX_TIMESTAMP(o.updated_on) update_date
					,IFNULL(o.currency_type,'USD') currency_type
				FROM slt_order o
				LEFT JOIN slt_order_items oi ON o.id = oi.order_id
				LEFT JOIN slt_enrollment enr ON enr.order_id = o.id
				LEFT JOIN slt_person sp ON sp.id = enr.user_id
				LEFT JOIN users u ON u.name = sp.user_name
				WHERE o.order_total_amt > 0
				GROUP BY o.id 
				ORDER BY o.id;";
  		db_query($qry);
	  }catch(Exception $e){
  		expDebug::dPrint($e);
  	}
  }
  
  private function updateSltOrderId()
  {
//     db_query("CALL slp_slt_order_uc_order_upd()");	//Commenting this procedure call as the follwoing update query is enough to Migrate orders from Non commerce to Commerce
  	db_query("UPDATE slt_order SET uc_order_id = id where order_total_amt > 0");
  }
  
  private function insertUcOrderLineItemTax()
  {
  	try{
  		db_query("INSERT INTO uc_order_line_items(order_id,`type`,title,amount) 
					SELECT o.order_id,'cybersource_tax','Tax','0.00000'
					FROM uc_orders o
					ORDER by o.order_id;");
  	}catch(Exception $e){
  		expDebug::dPrint($e);
  	}
  }
  function insertUcOrderProducts()
  {
  	try{
  		$qry = "INSERT INTO uc_order_products(
					order_id
					,nid
					,title
					,model
					,qty
					,cost
					,price
					,weight
				) 
				SELECT 
					uo.order_id
					,la.node_id
					,nde.title
					,'Class'
					,'1'
					,ifnull(oi.item_price,0.00000)
					,ifnull(oi.item_price,0.00000)
					,0 
				FROM slt_order so
				LEFT JOIN slt_order_items oi ON so.id = oi.order_id
				LEFT JOIN uc_orders uo ON so.uc_order_id=uo.order_id
				LEFT JOIN slt_node_learning_activity la ON oi.class_id= la.entity_id and if (oi.program_type='' or oi.program_type is null,la.entity_type='cre_sys_obt_cls',la.entity_type in ('cre_sys_obt_cur','cre_sys_obt_crt','cre_sys_obt_trn'))
				LEFT JOIN node nde ON la.node_id = nde.nid
				LEFT JOIN slt_enrollment enr ON enr.order_id = oi.order_id AND enr.class_id = oi.class_id AND enr.course_id = oi.course_id
				WHERE so.order_total_amt>0 AND IF(so.order_status='cme_pmt_sts_rjt',enr.reg_status IN('lrn_crs_reg_rsc','lrn_crs_reg_can'),enr.reg_status NOT IN('lrn_crs_reg_rsc'))
				ORDER BY so.id;";
  		db_query($qry);
  	}catch(Exception $e){
  		expDebug::dPrint($e);
  	}
  }
  
}

if($user->uid==1)
{
  $drupalServ=new DrupalcreateOrderService();
  $rtn =  $drupalServ->process();
  print "Order migration is completed. The data is replicated from slt_order to uc_order table.";
}
else
{
  print "You are not Authorized to access this Service.";         
}
?>