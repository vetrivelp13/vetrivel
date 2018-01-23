<?php  //echo '<pre>'; print_r(unserialize($_POST['order_info'])); 
$ordVal = unserialize($_POST['order_info']);
?>
<html>
<head>
<style type="text/css">
body {
	background-color: #f5f5f5;
	font-size: 12px;
}
#payment_call {
	
	margin: 0px auto;
	text-align: center;
	width: 600px;
}
#payment_call tr, td {
	border: 1px solid grey;
	text-align: left;
}
.table_head {
	font-weight: bold;
	height: 40px;
}
.submit_btn {
	border: none;
	text-align: center;
}
</style>
</head>
<body>

	<form name="payment_call" id="payment_call" action="web-sample.php" method="post" >
		<table cellpadding="3" cellspacing="0" id="payment_call" >
			<tr>
				<td colspan="2" class="table_head">BILLING INFORMATION:</td>
			</tr>
			<tr>
				<td><label>First Name: </label></td>
				<td><input type="text" name="billing_first_name" value="<?php print $ordVal->billTo->firstName; ?>" readonly /></td>
			</tr>
			<tr>
				<td><label>Last Name: </label></td>
				<td><input type="text" name="billing_last_name" value="<?php print $ordVal->billTo->lastName; ?>" readonly /></td>
			</tr>
			<tr>
				<td><label>Address 1: </label></td>
				<td><input type="text" name="billing_street1" value="<?php print $ordVal->billTo->street1; ?>" readonly /></td>
			</tr>
			<tr>
				<td><label>Address 2: </label></td>
				<td><input type="text" name="billing_street2" value="<?php if(!empty($ordVal->billTo->street2)) print $ordVal->billTo->street2; else print null; ?>" readonly /></td>
			</tr>
			<tr>
				<td><label>City: </label></td>
				<td><input type="text" name="billing_city" value="<?php print $ordVal->billTo->city; ?>" readonly /></td>
			</tr>
			<tr>
				<td><label>State/Province: </label></td>
				<td><input type="text" name="billing_state" value="<?php print $ordVal->billTo->state; ?>" readonly /></td>
			</tr>
			<tr>
				<td><label>Zip Code: </label></td>
				<td><input type="text" name=billing_postal_code" value="<?php print $ordVal->billTo->postalCode; ?>" readonly /></td>
			</tr>
			<tr>
				<td><label>Country: </label></td>
				<td><input type="text" name="billing_country" value="<?php print $ordVal->billTo->country; ?>" readonly /></td>
			</tr>
			<tr>
				<td><label>Phone: </label></td>
				<td><input type="text" name="billing_phone" value="<?php if(!empty($ordVal->billTo->phone)) print $ordVal->billTo->phone; else print null; ?>" readonly /></td>
			</tr>
			<tr>
				<td colspan="2" class="table_head"></td>
			</tr>
			<tr>
				<td colspan="2" class="table_head">CREDIT CARD INFORMATION:</td>
			</tr>
			<tr>
				<td><label>Card Number: </label></td>
				<td><input type="text" name="card_number"  maxlength="16" value="4111111111111111" /></td>
			</tr>
			<tr>
				<td><label>Expiration Month: </label></td>
				<td><input type="text" name="card_expire_month" value="12" /></td>
			</tr>
			<tr>
				<td><label>Expiration Year: </label></td>
				<td><input type="text" name="card_expire_year" value="2016" /></td>
			</tr>
			<tr>
				<td><label>CVV: </label></td>
				<td><input type="text" name="card_cvv" value="1111" /></td>
			</tr>
			<tr>
				<td colspan="2" class="table_head">	</td>
			</tr>
			<tr>
				<td colspan="2" class="submit_btn">
					<input type="submit" name="submit" value="Proceed Payment" />
					<input type="hidden" name="order" value='<?php print serialize($ordVal); ?>' />
					<input type="hidden" name="response_url" value='<?php print $_POST['response_url']; ?>' />
				</td>
			</tr>
		</table>
	</form>
</body>
</html>
<?php

