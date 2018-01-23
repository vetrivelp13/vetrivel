<?php

// $Id: uc_order-customer.tpl.php,v 1.1 2011-03-31 11:42:51 muthusamys Exp $

/**
 * @file
 * This file is the default customer invoice template for Ubercart.
 */
  global $theme_key;
  if($theme_key == 'expertusoneV2') {
?>
<style type="text/css">
/*New-theme-style*/
@media print {
input#btnPrint,input#btnClose {
display: none;
}
body {
 font-family: ProximaNovaBold;
 font-size: 12px;
}
}
.admin-action-button-middle-bg, .white-btn-bg-middle {
cursor: pointer;
}
.newInvoice{
	font-family: verdana, arial, helvetica;
	font-size:12px
}
#Invoice-container{
	padding-left: 1em
}
.Invoice-td{
	color: #000000;
	bgcolor:#cccccc;
}
.Invoice-view{
	text-align:right;
	font-weight:bold;
}
.Invoice-view-td{
	text-align:right;
}
.Invoice-inner-td{
	width:100%;
}
.Invoice-inner-order{
	padding-top:10px
}
#Invoice-order{
	border-top:1px solid #000;
}

.new-theme-container .newInvoice{
	font-family: openSansRegular, Arial;
	font-size:11px;
}
.new-theme-container #Invoice-container{
	padding-left: 1em;
}
.new-theme-container .newInvoice .Invoice-td{
	color: #FFFFFF;
	text-transform:uppercase;
	font-family: ProximaNovaBold;
	background: none repeat scroll 0 0 #000066;
}
.Invoice-view{
	text-align: right;
	font-weight: bold;
	text-transform: uppercase;
}
.Invoice-view-td{
	text-align:right;
}
.Invoice-inner-td{
	width:100%;
}
.Invoice-inner-order{
	padding-top: 10px
}
#Invoice-order{
	border-top: 1px solid #000;
}
.Invoice-tr,
.Invoice-inner-td{
  text-transform: uppercase;
 }
 .uc-invoice-currency-code{
 	width: 50px;
 	margin-left: 5px;

 }
/*safari hack Start*/
@media screen and (min-color-index:0)and(-webkit-min-device-pixel-ratio:0), not all and (min-resolution:.001dpcm) { @media { _:-webkit-full-screen,
     .new-theme-container .newInvoice .invoice-view-status{
        padding:0;
        line-height:13px;
        white-space:nowrap
      }
}}      
/*safari hack End*/
</style>
<?php }else{?>
<style type="text/css">
/*New-theme-style*/
@media print {
input#btnPrint,input#btnClose {
display: none;
}
body {
font-family: verdana, arial, helvetica;
 font-size: 12px;
}
}
#Invoice-container{
	padding-left: 1em
}
.Invoice-td{
	color: #000000;
	bgcolor:#cccccc;
}
.Invoice-view{
	text-align:right;
	font-weight:bold;
}
.Invoice-view-td{
	text-align:right;
}
.Invoice-inner-td{
	width:100%;
}
.Invoice-inner-order{
	padding-top:10px
}
#Invoice-order{
	border-top:1px solid #000;
}

.new-theme-container .newInvoice{
	font-family: verdana, arial, helvetica;
	font-size:11px;
}
.new-theme-container #Invoice-container{
	padding-left: 1em;
}
.new-theme-container .newInvoice .Invoice-td{
	color: #000000;
	text-transform:uppercase;
	font-family: ProximaNovaBold;
	background: none repeat scroll 0 0 #cccccc;
}
.Invoice-view{
	text-align: right;
	font-weight: bold;
	text-transform: uppercase;
}
.Invoice-view-td{
	text-align:right;
}
.Invoice-inner-td{
	width:100%;
}
.Invoice-inner-order{
	padding-top: 10px
}
#Invoice-order{
	border-top: 1px solid #000;
}
.Invoice-tr,
.Invoice-inner-td{
  text-transform: uppercase;
 }
 
</style>

<?php }
/*******************start**************/
$cert_css_path = $base_url. '/'. drupal_get_path('module', 'theme_override');
$filename = $_SERVER['DOCUMENT_ROOT'].'/'. drupal_get_path('module', 'theme_override').'/css/expertusone_en_override.css';

if ((function_exists('theme_override_init')) && (file_exists($filename))) { ?>
	<link href="<?php echo $cert_css_path; ?>/css/expertusone_en_override.css" rel="stylesheet">
<?php }
/********** End ************/
  $sltPersonId = db_query("SELECT id FROM slt_person WHERE user_name = (SELECT `name` FROM users WHERE uid=$order->uid) LIMIT 0,1")->fetchField();
  $sltPerson = getPersonDetails($sltPersonId, array('id', 'first_name', 'last_name','full_name', 'email', 'country',
                                                    'addr1', 'addr2', 'state', 'city', 'zip', 'phone_no'));
  /* $sltOrderId = db_query("SELECT id FROM slt_order WHERE uc_order_id=$order->order_id")->fetchField();  */

  $sltOrderDtl = db_query("SELECT orders.id, spli.attr2, orders.currency_type  FROM slt_order orders
				  Left Join slt_profile_list_items spli ON spli.attr1=orders.currency_type AND spli.code like 'cre_sys_crn_%'
				  WHERE orders.uc_order_id=$order->order_id")->fetchAssoc();

  $sltOrderId = $sltOrderDtl['id'];
  $curr_code = $sltOrderDtl['currency_type'];
  $curr_symbol = $sltOrderDtl['attr2'];
  expDebug::dPrint('$clsDetails_query $symbol_only = ' . $curr_symbol, 2);
  if(empty($curr_code))
  	$curr_code = 'USD';
  if(empty($curr_symbol))
  	$curr_symbol = ''; // 0056924 - default $ removal
  
  if (empty($sltPerson['id'])) {
    $sltPerson['first_name'] = '';
    $sltPerson['last_name']  = '';
    $sltPerson['full_name']  = '';
    $sltPerson['email']      = '';
    $sltPerson['addr1']      = '';
    $sltPerson['addr2']      = '';
    $sltPerson['country']    = '';
    $sltPerson['state']      = '';
    $sltPerson['city']       = '';
    $sltPerson['zip']        = '';
    $sltPerson['phone_no']   = '';
  }
  else {
    $sltPerson['state']   = getStateName($sltPerson['state'], $sltPerson['country']);
    $sltPerson['country'] = getCountryName($sltPerson['country']);
  }
?>
<table width="95%" border="0" cellspacing="0" cellpadding="1" align="center" bgcolor="#006699" class="PrintInnovoiceTxt new-theme-container">
  <tr>
    <td>
      <table width="100%" border="0" cellspacing="0" cellpadding="5" align="center" bgcolor="#FFFFFF" >
        <?php if ($business_header) { ?>
        <tr valign="top">
          <td>
            <table width="100%" class="newInvoice" border="1" >
              <tr>
                <td>
                 <img src="<?php /*echo $site_logo; echo $base_url."/sites/default/files/notification_logo.png*/ echo get_logo_path();?>" ></img>
                </td>
                <td width="98%">
                  <div id="Invoice-container">
                  <span><?php //echo $store_name; ?></span><br />
                  <?php //echo $site_slogan; ?>
                  </div>
                </td><!--
                <td nowrap="nowrap">
                  <?php echo $store_address; ?><br /><?php echo $store_phone; ?>
                </td>
              --></tr>
            </table>
          </td>
        </tr>
        <?php } ?>

        <tr valign="top">
          <td>
            <?php if ($thank_you_message)
            { ?>
            <p><b><?php echo t('Thanks for your order, !first_name !last_name!',
                                    array('!first_name' => $sltPerson['first_name'], '!last_name' => $sltPerson['last_name'])); ?></b>
            	<br></br><?php print t('This is a receipt of your recent order. Below is a summary of your order.');?> <?php //echo $order->order_id; ?>
            </p>

            <?php if (isset($_SESSION['new_user'])) { ?>
            <p><b><?php echo t('An account has been created for you with the following details:'); ?></b></p>
            <p><b><?php echo t('LBL054').':'; ?></b> <?php echo $new_username; ?><br />
            <b><?php echo t('LBL060').':'; ?></b> <?php echo $new_password; ?></p>
            <?php } ?>
            <!--
            <p><b><?php echo t('Want to manage your order online?'); ?></b><br />
            <?php echo t('If you need to check the status of your order, please visit our home page at !store_link and click on "My account" in the menu or login with the following link:', array('!store_link' => $store_link)); ?>
            <br /><br /><?php echo $site_login; ?></p>
            --><?php } ?>
			<?php //echo "<pre>"; print_r($order);?>
            <table cellpadding="4" cellspacing="0" border="0" width="100%" class="newInvoice">
               <tr>
                <td bgcolor="#cccccc" class="Invoice-td">
                  <b><?php echo t('LBL883'); ?></b>
                </td>
                <td bgcolor="#cccccc" class="Invoice-td"></td>
              </tr>
               <tr>
                <td colspan="2">

                  <table border="0" cellpadding="1" cellspacing="0" width="100%" class="newInvoice">
                    <tr>
                      <td nowrap="nowrap">
                        <b><?php echo t('LBL884'); ?></b>
                      </td>
                      <td><b>:</b></td>
                      <td width="98%">
                        <?php echo $order->order_id; ?>
                      </td>
                    </tr>

                    <tr>
                      <td nowrap="nowrap">
                        <b><?php echo t('LBL822').' '; ?></b>
                      </td>
                      <td><b>:</b></td>
                      <td width="98%">
                        <?php //echo Date('M,d Y',format_date($order->created, 'uc_store')); ?>
                        <?php echo format_date($order->created,'custom','M,d Y'); ?>
                      </td>
                    </tr>
                    <?php if($order->payment_method =='po'){?>
										<tr>
											<td nowrap="nowrap"><b><?php echo t('LBL778').' '; ?> </b>
											</td>
											<td><b>:</b></td>
											<td width="98%"><?php echo $order->payment_details['po_number']; ?>
											</td>
										</tr>
										<?php }?>
                    <tr>
                      <td nowrap="nowrap">
                        <b><?php echo t('LBL885').' '; ?></b>
                      </td>
                      <td><b>:</b></td>
                      <td width="98%">
                        <?php
                        	$orderStatus = db_query("SELECT slf_get_paymentstatus_name('$order->order_status','name')")->fetchField();
                            if($orderStatus)
                            	echo t($orderStatus);
                            else
                            	 echo "-";
                        ?>
                      </td>
                    </tr>
                    <tr>
                      <td nowrap="nowrap">
                        <b><?php echo t('LBL548').' '; ?></b>
                      </td>
                      <td><b>:</b></td>
                      <td width="98%">
                      <?php
                        	$paymentMethod = db_query("SELECT slf_get_paymentmethod_name('$order->payment_method','name')")->fetchField();
                        	//echo $paymentMethod;
                        	if($paymentMethod == 'Contract or Agreement number/Invoice') {
                        	  echo t('Contract or Agreement Number/Invoice');
                        	}elseif($paymentMethod == 'Zero Cost'){
                            	echo t('N/A');
                        	}elseif($paymentMethod){
                        		echo t($paymentMethod);
                        	}
                            else {
                            	 echo "-";
                            }

                        ?>
                      </td>
                    </tr>
                    <?php
                    if(($paymentMethod == 'Credit card') && ($orderStatus == 'Payment Confirmed')) {?>
                     <tr>
                      <td nowrap="nowrap">
                        <b><?php echo t('LBL886').' '; ?></b>
                      </td>
                      <td><b>:</b></td>
                      <td width="98%">
                      <?php
                    		 	$authCode = db_query("SELECT slf_get_cc_authcode('$sltOrderId')")->fetchField();
                        	if($authCode)
                            	echo $authCode;
                            else
                            	 echo "-";

                        ?>
                      </td>
                    </tr>
                    <?php }?>
                    <?php if ($shipping_method && uc_order_is_shippable($order)) { ?>
                    <?php } ?><!--

                    <tr>
                      <td nowrap="nowrap" width="140px">
                        <b><?php echo t('Products Subtotal'); ?></b>
                      </td>
                      <td><b>:</b></td>
                      <td width="98%">
                        <?php echo $order_subtotal; ?>
                      </td>
                    </tr>
                    -->

                   <?php
                    foreach ($line_items as $item) {
                    if ($item['line_item_id'] == 'subtotal' || $item['line_item_id'] == 'total') {
                      continue;
                    }?><!--

                    <tr>
                      <td nowrap="nowrap">
                        <b><?php echo $item['title']; ?>:</b>
                      </td>
                      <td>
                        <?php
                          //echo uc_price($item['amount'], $context);
                          echo uc_currency_format_order($item['amount'], $curr_code, $curr_symbol);
                        ?>
                      </td>
                    </tr>

                    -->
                    <?php } ?>
					<!--
                   <tr>
                      <td nowrap="nowrap" width="140px">
                        <b><?php echo t('Total for this Order'); ?>&nbsp;</b>
                      </td>
                      <td><b>:</b></td>
                      <td>
                        <?php echo $order_total; ?>
                      </td>
                    </tr>
				   --></table>

                </td>
              </tr>
              <tr>
                <td colspan="2" bgcolor="#cccccc" class="Invoice-td">
                  <b><?php echo t('LBL887'); ?></b>
                </td>
              </tr>
              <tr>
              	<td colspan="2">
              	<?php
				          echo $sltPerson['full_name'] . "<br>";
				          $userInfoItemsList = array('addr1', 'addr2', 'city', 'state', 'country', 'zip', 'email', 'phone_no');
				          foreach ($userInfoItemsList as $userInfoItem) {
					          if(!empty($sltPerson[$userInfoItem])) {
					            echo $sltPerson[$userInfoItem] . '<br>';
					          }
				          } //end foreach
                ?>
              	</td>
              </tr>
                <!--<tr>
                <td nowrap="nowrap">
                  <b><?php echo t('E-mail Address:'); ?></b>
                </td>
                <td width="98%" colspan="2">
                  <?php echo $order_email; ?>
                </td>
              </tr>-->
              <tr>
                <td colspan="2" bgcolor="#cccccc"  class="Invoice-td">
                  <b><?php echo t('LBL555'); ?></b>
                </td>
              </tr>

              <tr>
                <td colspan="2">

                  <table width="100%" cellspacing="0" cellpadding="0" class="newInvoice">
                    <tr>
                      <td valign="top" width="50%">
                       <!--   <b><?php  //echo t('Billing Address:'); ?></b><br /> -->
                        <?php
                        if(is_array($order_billing_address)){
                          foreach($order_billing_address as $billing) {
                          	if(!empty($billing)){
                           		echo (strpos($billing,'Unknown') === false) ? $billing . '<br>' : 'N/A';
                            }
                          }
                        }
                        else {
                        	echo $order_billing_address;
                        }
                        ?><br />
                        <br />

                      </td>
                    </tr>
                  </table>

                </td>
              </tr>


              <?php if (uc_order_is_shippable($order)) { ?>
              <tr>
                <td bgcolor="#cccccc"  class="Invoice-td">
                 <b><?php echo t('Shipping Details').' : '.t('LBL888'); ?></b>
                </td>
                  <td bgcolor="#cccccc" class="Invoice-td"></td>
              </tr>
              <?php } ?>
              <tr>
              </tr>
             <tr>
                      <td colspan="3" >
                        <table width="100%" class="newInvoice">
                        <tr class="Invoice-tr"><td><b><?php print t("Course").' '.t("LBL107");?></b></td><td><b><?php print t("LBL042");?></b></td><td><b><?php print t("Location");?></b></td><td class="Invoice-view-td"><b><?php print t("LBL040");?></b></td><td class="Invoice-view-td"><b><?php print t("LBL1165");?></b></td><td>&nbsp;</td></tr>
                        <?php //echo "<pre>"; print_r($order);?>
                         <?php if (is_array($order->products)) {
                         	$subRefundAmt = 0;
                         	$subTaxAmt    = 0;
                         	$subDiscountAmt = 0;
                            foreach ($order->products as $product) {
                              $price_info = array(
                                'price' => $product->price,
                                'qty' => $product->qty,
                              );

						  $slt_CourseQry = db_query("SELECT sl.entity_type, sl.entity_id FROM slt_node_learning_activity sl WHERE sl.node_id=:nid",
						                              array(':nid' =>$product->nid));
						  expDebug::dPrintDBAPI('$slt_CourseQry = ' , $slt_CourseQry, array(':nid' =>$product->nid));

						   foreach ($slt_CourseQry as $slt_Course) {
						   	 unset($classLocationName);
						   	 unset($sessionDate);
						     // Getting the Class session details for display the date
						     if($slt_Course->entity_type == 'cre_sys_obt_cls') {
						       $clsDetails_query = "SELECT cls.course_id, cls.delivery_type, slo.name FROM slt_course_class cls
                                                 LEFT JOIN slt_location slo ON cls.location_id = slo.id
                                                 WHERE cls.id=$slt_Course->entity_id";
						       expDebug::dPrint('$clsDetails_query = ' . $clsDetails_query, 2);
						       $clsDetails = db_query($clsDetails_query)->fetchAll();
						       expDebug::dPrint('$clsDetails = ' . print_r($clsDetails, true));

						       if ($clsDetails[0]->delivery_type == 'lrn_cls_dty_ilt') {
						       	 $classLocationName = $clsDetails[0]->name;
						       }

						       if ($clsDetails[0]->delivery_type == 'lrn_cls_dty_vcl' || $clsDetails[0]->delivery_type == 'lrn_cls_dty_ilt') {
						         $getSession_details = getSessionDetails($clsDetails[0]->course_id, $slt_Course->entity_id, $clsDetails[0]->delivery_type, "enroll");
      						   if(count($getSession_details) > 0) {
                       $sDay = $getSession_details[0]['session_start_date_format'];
                       $sTime = $getSession_details[0]['session_start_time_format'];
                       $sTimeForm = $getSession_details[0]['session_start_time_form'];
                       if (count($getSession_details) > 1) {
                         $sessLenEnd = count($getSession_details)-1;
                         $eDay = $getSession_details[$sessLenEnd]['session_start_date_format'];
                         $eTime = $getSession_details[$sessLenEnd]['session_start_end_format'];
                         $eTimeForm = $getSession_details[$sessLenEnd]['session_end_time_form'];
                         $sessionDate = $sDay .' '. $sTime .' '. '<span class="time-zone-text">'.$sTimeForm.'</span>'.' to '.$eDay. ' ' .$eTime.' <span class="time-zone-text">'.$eTimeForm.'</span>';
                       }
                       else {
                         $eTime = $getSession_details[0]['session_start_end_format'];
                         $eTimeForm = $getSession_details[0]['session_end_time_form'];
                         $sessionDate = $sDay .' '. $sTime.' '.'<span class="time-zone-text">'.$sTimeForm.'</span>'.' to '.$eTime.' <span class="time-zone-text">'.$eTimeForm.'</span>';
                       }
                     }
						       }
						     }

						     $orderStatus = getSltOrderItemDetails($sltOrderId, $slt_Course->entity_id, $slt_Course->entity_type);
						     $refund_amount = 0;
						     $taxAmt = 0;
						     $discountAmt  = 0;
						     if($orderStatus[0]->refund_amount > 0){
						     	$refundAmt = ($orderStatus[0]->refund_amount) ? ($orderStatus[0]->refund_amount) : 0;
						     	$taxAmt    = ($orderStatus[0]->tax_amount) ? ($orderStatus[0]->tax_amount) : 0;
						     	$discountAmt  = getDropListDiscountAmount($product->nid,$order->order_id);
						     	if($refundAmt){
						     		$refund_amount = $refundAmt - $taxAmt;
						     		$refund_amount = $refund_amount + $discountAmt;
						     	}
						     }
						     $subRefundAmt   += $refund_amount;
						     $subTaxAmt      += $taxAmt;
						     $subDiscountAmt += $discountAmt;
						     if(!$subRefundAmt){
						     	$subTaxAmt 			= 0;
						     	$subDiscountAmt = 0;
						     }
						     $cancelStatus = ($orderStatus[0]->order_status == 'cme_pmt_sts_rjt') ? t('Canceled') : '';
							 ?>
	                          <tr>
	                          	<td width="30%"><?php echo titleController('UC-ORDER-CUSTOMER-TITLE',$product->title,65);?></td>
	                          	<td width="21%">
	                          	  <?php
	                          	    if (!empty($sessionDate)) {
                                    echo $sessionDate;
	                          	    }
	                          	    else {
	                                  echo '&nbsp;&nbsp;&nbsp;-';
	                          	    }
	                          	  ?>
	                          	</td>
	                          	<td width="20%">
	                          	  <?php
	                          	    if (!empty($classLocationName)) {
	                          	      echo titleController('UC-ORDER-CUSTOMER-CLASS-LOCATIONNAME',$classLocationName,35);
                                  }
                                  else {
	                          		    echo '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-';
	                          	    }
	                          	  ?>
	                          	</td>
								
								<td class="Invoice-view-td" width="12%"><?php echo uc_currency_format_order($product->price * $product->qty, $curr_code, $curr_symbol);?></td>
								<td class="Invoice-view-td" width="12%"><?php echo uc_currency_format_order($refund_amount, $curr_code, $curr_symbol) ;?></td>
								<td  width="5%" class="invoice-view-status"><?php echo $cancelStatus;?></td>
	                          </tr>
                          <?php
							}

                              }?>
                          <?php
                          $discount_label_flag = 1;
                          $tax_spacing_top = '';
                          $uc_disc_cnt = 0;
                          foreach($line_items as $key => $line_item){
                            //print_r($line_item);
                           /*  if(($line_item['type'] == 'uc_discounts') && $discount_label_flag) {
                              print '<tr><td colspan="3" class="Invoice-view">&nbsp;</td><td class="Invoice-inner-td-">&nbsp;</td><td class="Invoice-inner-td-">&nbsp;</td></tr>';
                              $discount_label_flag = 0;
                            } */
                          if($uc_disc_cnt > 0 && $line_item['type'] == 'uc_discounts')
                          	continue;
                          if(($line_item['type'] == 'cybersource_tax') && empty($tax_spacing_top)) {
                              $tax_spacing_top = 'class="Invoice-inner-order"';
                            }
                          ?>
                          <tr>
                          <?php
                             if(($line_item['line_item_id'] == 'total') || ($line_item['line_item_id'] == 'subtotal')){
                             	$line_item['amount'] = ($line_item['amount'] < 0) ? 0 : $line_item['amount'];
                               print '<td colspan="3" class="Invoice-view">'.$line_item['title'].'</td><td class="Invoice-view-td"><div id="Invoice-order">'.uc_currency_format_order($line_item['amount'], $curr_code, $curr_symbol) .'</div></td>';
                               if($line_item['line_item_id'] == 'subtotal'){
                               	 print '<td class="Invoice-view-td"><div id="Invoice-order">'.uc_currency_format_order($subRefundAmt, $curr_code, $curr_symbol) .'</div></td><td></td>';
                               }
                               else if($line_item['line_item_id'] == 'total'){
                               	 $totalRefAmt = ($subRefundAmt - $subDiscountAmt) + $subTaxAmt;
                               	 print '<td class="Invoice-view-td"><div id="Invoice-order">'.uc_currency_format_order($totalRefAmt, $curr_code, $curr_symbol) .'</div></td>';
                               }
                             }else if($line_item['type'] == 'cybersource_tax'){
                               print '<td colspan="3" class="Invoice-view">'.t($line_item['title']).' : </td><td class="Invoice-view-td">'.uc_currency_format_order($line_item['amount'], $curr_code,$curr_symbol) .'</td>';
                               print '<td class="Invoice-view-td">'.uc_currency_format_order($subTaxAmt, $curr_code, $curr_symbol) .'</td><td></td>';
                             }else {
                               //print '<td colspan="3" class="Invoice-view-td"><div '.$tax_spacing_top.'>'.t($line_item['title']).' : </div></td><td class="Invoice-view-td"><div '.$tax_spacing_top.'>'.uc_currency_format_order($line_item['amount']) .'</div></td>';
                             	$uc_disc_cnt++;
                             	print '<td colspan="3" class="Invoice-view"><div '.$tax_spacing_top.'>'.t('LBL904').' : </div></td><td class="Invoice-view-td"><div '.$tax_spacing_top.'>'.uc_currency_format_order($order->ord_total_discounts, $curr_code, $curr_symbol) .'</div></td>';
                                print '<td class="Invoice-view-td"><div '.$tax_spacing_top.'>'.uc_currency_format_order(($subDiscountAmt*(-1)), $curr_code,$curr_symbol) .'</div></td><td></td>';
                             }
                           ?>
                          </tr>
                          <?php }
                           }
                           $config = getConfig('exp_sp');
                           $message = getConfigValue('order_message');
                           if ($message == 1){
                             $ques   = t('LBL240');
                           	 $msgstr = t('MSG547').' '.$config["reply_to"];
                             $ordmsg = t('MSG549');
                           }
                              ?>
                            <tr><td colspan="6">&nbsp;</td></tr>
                        </table>

                      </td>
                    </tr>

			 <!-- <tr>
			 	<td colspan="3"><?php print t('MSG539');?></td>
			 </tr>
			 <tr>
			 	<td colspan="3"><b><?php  print t('LBL895');?></b><br>
			 	<ul><li><?php print t('MSG540');?></li>
			 	<li><?php print t('MSG541');?></li>
			 	<li><?php print t('MSG542');?></li>
			 	</ul>
			 	</td>
			 </tr>
			  <tr>
			 	<td colspan="3"><b><?php print t('LBL896');?></b><br>
			 	<ul><li><?php print t('MSG543');?></li>
			 	<li><?php print t('MSG544');?></li>
			 	<li><?php print t('MSG545');?></li>
			 	</ul>
			 	</td>
			 </tr>
			 <tr><td colspan="3"><b><?php print t('LBL897');?></b></td></tr>
			 <tr><td colspan="3"><?php print t('MSG546');?></td></tr> -->
			 <tr class="Invoice-inner-td"><td colspan="3"><b><?php print $ques;?></b></td></tr>
			 <tr><td colspan="3"><?php print $msgstr;?></td></tr>
             <tr><td colspan="3"><?php print $ordmsg;?></td></tr>
              <?php if ($help_text || $email_text || $store_footer) { ?>
              <tr>
                <td colspan="2">
                  <hr noshade="noshade" size="1" /><br />

                  <?php if ($help_text) { ?>
                  <p><b><?php echo t('Where can I get help with reviewing my order?'); ?></b><br />
                  <?php echo t('To learn more about managing your orders on !store_link, please visit our <a href="!store_help_url">help page</a>.', array('!store_link' => $store_link, '!store_help_url' => $store_help_url)); ?>
                  <br /></p>
                  <?php } ?>

                  <?php if ($email_text) { ?>
                  <p><?php echo t('Please note: This e-mail message is an automated notification. Please do not reply to this message.'); ?></p>

                  <p><?php echo t('Thanks again for shopping with us.'); ?></p>
                  <?php } ?>

                  <?php if ($store_footer) { ?>
                  <p><b><?php echo $store_link; ?></b><br /><b><?php echo $site_slogan; ?></b></p>
                  <?php } ?>
                </td>
              </tr>
              <?php } ?>

            </table>
          </td>
        </tr>
      </table>
    </td>
  </tr>
</table>
