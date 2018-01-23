<?php
//This file is for paymetric integration in mobile

global $base_url;
chdir("../../../../../../../");
define('DRUPAL_ROOT', getcwd());
require_once DRUPAL_ROOT."/includes/bootstrap.inc";
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
if(!isset($_GET['order_id'])){
  //drupal_set_message(t("We're sorry.  An error occurred while processing your order that prevents us from completing it at this time. Please contact us and we will resolve the issue as soon as possible."), 'error');
  //drupal_goto('cart/checkout/review');
  //return false;
}

include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/services/Encryption.php";
				$enc = new Encrypt();
				

expDebug::dPrint("access_from".$_SESSION['access_from']);

if(arg(0) == 'administration' || substr($_SERVER['HTTP_REFERER'], -strlen('administration/order/pay')) == 'administration/order/pay'){
	$orderId = $_SESSION['cart_admin_order'];
}
else{
	$orderId = $_GET['order_id'];
}
$order = uc_order_load($orderId);
//echo '<pre>';
//print_r($order);
$cc_number = $order->payment_details['cc_number'];
$currency = $order->currency_type;
$cc_type = _uc_paymetric_card_type($cc_number);
$cc_exp_month = $order->payment_details['cc_exp_month'];
$cc_exp_year = $order->payment_details['cc_exp_year'];
$cc_exp_year = substr($cc_exp_year,-2,2);
$cc_cvv = $order->payment_details['cc_cvv'];
$order_total = $order->order_total;
 $Payload='';
  if(variable_get('uc_payment_credit_gateway', '')=='exp_paymetric' && variable_get('paymetric_type', 'dicomp')=='dicomp'){ // Paymetric DI Implementation
  	$secureType = variable_get('paymetric_secure_method', 'tokenization');
  	if($secureType == 'tokenization'){
  		$GUID = variable_get('paymetric_di_guid', '');
			$PresharedKey = variable_get('paymetric_di_sharedkey', '');
			$BaseURL = variable_get('paymetric_di_url', '');
			if($GUID !='' && $PresharedKey!='' && $BaseURL!=''){
				$Payload = '<Request><MerchantReference></MerchantReference><TotalAmount>';
				$Payload .= '<Tax></Tax><GrandTotal></GrandTotal><CurrencyCode>'.$currency.'</CurrencyCode>';
				$Payload .= '</TotalAmount>';
				$Payload .= '<TokenizedCard><CCToken></CCToken><CCExpirationDate><Month>'.$cc_exp_month.'</Month><Year>'.$cc_exp_year.'</Year></CCExpirationDate>' ;
				$Payload .= '<CCType>4000</CCType></TokenizedCard><RedirectURL>'.$base_url.'/sites/all/modules/core/exp_sp_commerce/modules/exp_sp_payment/exp_sp_paymetric_resp_mobile.php</RedirectURL></Request>';
				$Signature = base64_encode(hash_hmac('sha256', $Payload, $PresharedKey, true));				
								
				$Payload = '<MerchantRequest><MerchantGUID>'.$GUID.'</MerchantGUID><Signature>'.$Signature.'</Signature>'.$Payload.'</MerchantRequest>';
			}
  	}
  }

$depth = array();
$ReturnPayload = $_GET['r'];
$ReturnSignature = $_GET['s'];

// correct the Modified Base 
$ReturnPayload = str_replace('-','+',$ReturnPayload);
$ReturnPayload = str_replace('_','/',$ReturnPayload);
$ReturnSignature = str_replace('-','+',$ReturnSignature);
$ReturnSignature = str_replace('_','/',$ReturnSignature);

$ReturnPayload = base64_decode($ReturnPayload);

function startElement($parser, $name, $attrs) 
{
    global $depth;
    for ($i = 0; $i < $depth[$parser]; $i++) {
        echo "&nbsp;&nbsp;&nbsp;&nbsp;";
    }
    echo "<b>$name</b><br>\n";
    $depth[$parser]++;
}

function endElement($parser, $name) 
{
    global $depth;
    $depth[$parser]--;
}

function contents($parser, $data) {

    global $closeTag;
	global $depth;
    for ($i = 0; $i < ($depth[$parser]+1); $i++) {
        echo "&nbsp;&nbsp;&nbsp;&nbsp;";
    }
    $data = preg_replace("/^\s+/", "", $data);
    $data = preg_replace("/\s+$/", "", $data);
    echo "$data<br>\n";
} 

?>


<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>DI EComm Sample</title>
        <script language="javascript" type="text/javascript">
            var Language = {"CCType":"Credit Card Type", "CCNumber":"Credit 1 Card 2 Number", "ExpDate":"Expiration Date", "StartDate":"Start Date", "IssuerNumber":"Issuer Number", "PayNow":"Continue", "CVV":"My Custom CVV", "":""}
            function MerchantValidationCheck(BinRange, ExpirationMonth, ExpirationYear, CVV, StartMonth, StartYear, Issuernumber) {
            	  //document.getElementById("ProcessMsg").style.display="none";
                return true;
            }
            window.parent.document.getElementById("payment-details").setAttribute("style","height: 150px;");
        </script>
        <style type="text/css">
            .DataIntercept input
            {
                border:1px solid #e5e5e5;
                border-radius: 10px;
                text-align: left;
                font-family:helvetica neue,helvetica,arial; 
				font-size:14px; 
				color:#666666;
            }
            .DataInterceptCreditCardType
            {
                visibility: hidden;
                display: none;
            }
            .clearfix payment-details-credit{
            	height:150px;
            }
        </style>
    </head>
    <body>
        <form action="" method="post">
			<?php
			if(arg(0) == 'administration' || (substr($_SERVER['HTTP_REFERER'], -strlen('administration/order/pay')) == 'administration/order/pay')){
				$drupalOID = $_SESSION['cart_admin_order'];
				$sltOID    = $_SESSION['slt_admin_orderid'];
			}
			else{
				$drupalOID = $_SESSION['cart_order'];
				$sltOID    = $_SESSION['slt_order_id'];
			}
			if (strlen($ReturnPayload)>0 && (base64_encode  (hash_hmac('sha256', $ReturnPayload, $PresharedKey, true)) == $ReturnSignature))
			{
					
					expDebug::dPrint("PAYMETRIC CC TOKEN RESPONSE");
					expDebug::dPrint($ReturnPayload , 4);
					$xml_parser = xml_parser_create();
				  xml_parse_into_struct($xml_parser, trim($ReturnPayload), $xml_values);
				  xml_parser_free($xml_parser);
				  expDebug::dPrint($xml_values , 4);
				  $status = $xml_values[1]['value'];
				  $cctoken = $xml_values[6]['value'];
				  $_SESSION['cc_date_exp'] = $xml_values[8]['value'];
				  if(strtolower($status)!='failed'){
				  	expDebug::dPrint("PASSED");
				  	
				  	expDebug::dPrint("PASSED".$status);
				  	expDebug::dPrint("PASSED".$cctoken );
				  	expDebug::dPrint("PASSED". $_SESSION['cc_date_exp']);
                     // header("Location:x-expertusone-oauth://callback?paymetric=true&status=".$xml_values[1]['value']."&cctoken=".$xml_values[6]['value']."&cc_date_exp=".$xml_values[8]['value']);

                    // header('Location :'.$_SERVER['DOCUMENT_ROOT'].'/test.php');
//                      . '/mobile/apns-dev.pem'
//                     window.location.href = "";
//                     var appUrl="x-expertusone-oauth://callback?"+passUrl;
                    
//                     window.location.href=appUrl;
				  	echo t('<span style="font-size: 16px; font-family: proximanovabold,arial;"> Click Continue to proceed.<br/> </span>');
					?>
					   <script> 
					     window.location.href= "x-expertusone-oauth://callback?paymetric=true&status=<?php echo($xml_values[1]['value']); ?>&cctoken=<?php echo($xml_values[6]['value']); ?>&cc_date_exp=<?php echo($xml_values[8]['value']); ?>";// "x-expertusone-oauth://callback?paymetric=true";
					        // window.parent.document.getElementById("edit-panes-payment-details-cc-token").value="<?php echo $cctoken; ?>";
 // 					   		var checkoutbtns1 = window.parent.document.getElementById("learner-admin-review-order");
 // 			                checkoutbtns1.style.display="block";
					   </script>
					<?php 
				  }else{
					  ?>
				   <script> 
				     window.location.href= "x-expertusone-oauth://callback?paymetric=true&status=failure";
				   </script>
					 <?php
                    // header("Location:paymetricredirect://expertus.com?status=failure");
 				  	echo t('Card verification is failure. Please try again...');
				  	?>
				  	<script> 
				  	  var framwin = window.parent.document.getElementById("PaymetricFrame");
				  	  framwin.src="/sites/all/modules/core/exp_sp_commerce/modules/exp_sp_payment/exp_sp_paymetric_resp_mobile.php?order_id='<?php echo $drupalOID ?>'&slt_order_id='<?php echo $sltOID?>'";
				  	</script> 
				  	<?php 
				  }
			}
			else
			{
				if (strlen($ReturnPayload)>0 && (base64_encode  (hash_hmac('sha256', $ReturnPayload, $PresharedKey, true)) != $ReturnSignature)){
					echo t('Card verification is failure. Please try again...');
					?>
				  	<script> 
				  	  var framwin1 = window.parent.document.getElementById("PaymetricFrame");
				  	  framwin1.src="/sites/all/modules/core/exp_sp_commerce/modules/exp_sp_payment/exp_sp_paymetric_resp_mobile.php?order_id='<?php echo $drupalOID ?>'&slt_order_id='<?php echo $sltOID?>'";
				  	</script> 
				  	<?php 
				}
			?>
						 <script> 
					   		 function checkOrderStatus(){
					   			  window.document.getElementById("Paymetric_ErrorLogging").innerHTML='';
										var ccn = document.getElementById('Paymetric_CreditCardNumber').value;
										var exm = document.getElementById('Paymetric_Exp_Month').value;
										var exy = "20"+document.getElementById('Paymetric_Exp_Year').value;
										var cvv = document.getElementById('Paymetric_CVV').value;

								   		  var paynowbtn = window.document.getElementById("PayNowButton");
								          paynowbtn.click();
								    if(window.document.getElementById("Paymetric_ErrorLogging").innerHTML.length<=0){
								    	 document.getElementById("ProcessMsg").style.display="block";
								    }
					   		 }
					   </script>
            <div style="width: 300px; border: solid 0px; margin-left:112px;display:none;">
                <script src="<?php echo $BaseURL; ?>/diecomm/Preloader/EN.ashx?GUID=<?php echo $GUID; ?>" type="text/javascript" language="javascript"></script>
            </div>
            <div id="ProcessMsg" style="margin: auto;width: 100%;position:absolute;top:40px;left:20px;font-family:helvetica neue,helvetica,arial;font-size: 30px;color: #E36500; height: 32px;">
               <p>Validating card detail...</br> Please do not click the Refresh or Back button.</br> Your transaction may be interrupted.</p>
            </div>
            <div  style="display:none;">
            	 <input type="button" class="PaymentPage_OrderCheckBtn" value="Validate" onclick="checkOrderStatus();" >
            </div>

            <script type="text/javascript" language="javascript">
				
// 			if(callFrom == 'mobile')
// 			{
				window.setTimeout(function()
				{					
					
					var dataString = "<?php echo($enc->getAESDecryptedValue($_REQUEST['cc_data'])); ?>";
					
					var dataArray = dataString.split('~~');
					       					
			    var img=document.querySelector('#Paymetric_CreditCardNumber');
			              img.value= dataArray[0];//'4111111111111111';
			              var exm=document.querySelector('#Paymetric_Exp_Month');
			              exm.value= dataArray[1];//"01";
			              var exy=document.querySelector('#Paymetric_Exp_Year');
			              exy.value=dataArray[2];//"18";
			              var cvv=document.querySelector('#Paymetric_CVV');
			              cvv.value=dataArray[3];//"111";
			              var paynowbtn = document.querySelector('#PayNowButton');
			               paynowbtn.click();
			             var error = document.querySelector('#Paymetric_ErrorLogging');
			             if(error.innerHTML){
			                  window.location.href= "x-expertusone-oauth://callback?paymetric=true&status=failure";
			             };
						 },3000);
			// }
		    
			    
                ////////////////////////////////////////////////////////////////////////
                //
                // The call below is required to populate the form that will be posted
                // back to the Paymetric Webservers. If you do not make this call the
                // form will not populate and an error will be generated when the client
                // tries to hit the Pay Now button
                //
                ////////////////////////////////////////////////////////////////////////
                UpdatePaymentPageContent('<?php echo $Payload; ?>');
            </script>
		<script language="javascript" type="text/javascript">
            var paybtn = document.getElementById("PayNowButton");
            paybtn.style.visibility="hidden";
            var checkoutbtns = window.parent.document.getElementById("learner-admin-review-order");
            //checkoutbtns.style.display="none";
        </script>
        <style type="text/css">
            .PaymentPage_OrderCheckBtn{
    						/*background-color : #FF8629;*/
    						background : url("/sites/all/themes/core/expertusoneV2/expertusone-internals/images/ExpertusIcons_v2.png") no-repeat scroll -12px -947px;
    						border:1px solid #ffffff;
    						border-radius:8px;
							color: #ffffff;
							cursor: pointer;
							font-family:openSansRegular,Arial; 
							font-size: 12px;
							margin-right: -10px;
							padding: 0 14px;
							text-align: center;
							position: absolute;
    						right: 10px;
    						top: 100px;
    						width: 90px;
						}
						.DataInterceptExpirationDate,
						.DataInterceptCVV,
						.DataInterceptCreditCardNumber{
						    border:0px solid;
							font-family:openSansRegular,Arial; 
							font-size:13px; 
							font-weight: normal;
							color:#2d2d2d;
							height:24px;
							width:120px;
						}
        </style>
			<?php
			}
			?>
        </form>
    </body>
</html>
  	