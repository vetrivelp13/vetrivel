<?php

/**
 * Map node id to entity id using data stored in slt_node_learning_activity table.
 */
function getEntityIdFromNodeId($nodeId) {
	try{
		// Select the DB table
		$select = db_select('slt_node_learning_activity', 'nla');
		// Select fields to fetch
		$select->addField('nla',	'entity_id', 'entity_id');
		// Set the condition(s)
		$select->condition('nla.node_id', $nodeId);
		// Execute query and fetch the single values
		$entityId  = $select->execute()->fetchField();
		expDebug::dPrint(" Course id for node_id: " . $nodeId . " is: " . $entityId,4);
		return $entityId;
	}catch (Exception $ex) {
		watchdog_exception('getEntityIdFromNodeId', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Formats duration in minutes to hours and minutes.
 */
function formatDuration($durationInMin) {
	try{
		expDebug::dPrint(' formatDuration() called with durationInMin = ' . $durationInMin,4);
		if (empty($durationInMin) || $durationInMin <= 0) {
			return '-';
		}
		$hours = floor($durationInMin / 60);
		$minutes = $durationInMin % 60;
		//Prepare the hour text
		$hoursStr = '';
		if ($hours == 0) {
			$hoursStr = '';
		}
		else if ($hours == 1) {
			$hoursStr = $hours . ' ' . t('LBL421');
		}
		else {
			$hoursStr = $hours . ' ' . t('LBL422');
		}
		//Prepare the minute text
		$minutesStr = '';
		if ($minutes == 0) {
			$minutesStr = '';
		}
		else if ($minutes == 1) {
			$minutesStr = $minutes . ' ' . t('LBL423');
		}
		else {
			$minutesStr = $minutes . ' ' . t('LBL424');
		}
		if ($hoursStr == '') {
			return $minutesStr;
		}
		if ($minutesStr == '') {
			return $hoursStr;
		}
		return $hoursStr . ' ' . $minutesStr;
	}catch (Exception $ex) {
		watchdog_exception('formatDuration', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * Returns current date and time in the same format as was returned by MySQL now()
 */
function now() {
	try{
		return date_format(date_now(date_default_timezone(FALSE)),'Y-m-d H:i:s');
	}catch (Exception $ex) {
		watchdog_exception('formatnow', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getProfileItemIdFromCode() - Map profile item's code to its id in slt_profile_list_items table.
 *                            - Returns id.
 */
function getProfileItemIdFromCode($itemCode,$language='cre_sys_lng_eng') {
	try{
		// Select table
		$selectStmt = db_select('slt_profile_list_items', 'plist');
		// Set conditions
		$selectStmt->condition('plist.code', $itemCode);
		$selectStmt->condition('plist.lang_code', $language);
		// Select fields
		$selectStmt->addField('plist', 'id', 'id');
		expDebug::dPrintDBAPI(' $selectStmt = ' ,$selectStmt);
		// Execute query
		$result = $selectStmt->execute();
		expDebug::dPrint(' $result = ' . print_r($result, true),3);

		if ($result->rowCount() <= 0) {
			return null;
		}

		return $result->fetchField();
	}catch (Exception $ex) {
		watchdog_exception('getProfileItemIdFromCode', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getProfileItemCodeFromId() - Map profile item's id to its code in slt_profile_list_items table.
 *                            - Returns code.
 */
function getProfileItemCodeFromId($itemId) {
	try{
		// Select table
		$selectStmt = db_select('slt_profile_list_items', 'plist');

		// Set conditions
		$selectStmt->condition('plist.id', $itemId);

		// Select fields
		$selectStmt->addField('plist', 'code', 'code');

		expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);

		// Execute query
		$result = $selectStmt->execute()->fetchField();
		expDebug::dPrint(' $result = ' . print_r($result, true),3);

		return $result;
	}catch (Exception $ex) {
		watchdog_exception('getProfileItemCodeFromId', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getProfileSettingsColumn() - Fetch profile settings in a column $colName in slt_entity_profile_mapping for an entity (entityType, entityId)
 */
function getProfileSettingsColumn($entityType, $entityId, $colName) {
	try{
		// Select table
		$selectStmt = db_select('slt_entity_profile_mapping', 'pmap');

		// Set conditions
		$selectStmt->condition('pmap.entity_type', $entityType);
		$selectStmt->condition('pmap.entity_id', $entityId);

		// Select fields
		$selectStmt->addField('pmap', $colName, $colName);

		expDebug::dPrintDBAPI(' $selectStmt obj = ' , $selectStmt);

		// Execute query
		$result = $selectStmt->execute();

		if ($result->rowCount() <= 0) {
			return null;
		}
		return $result->fetchField();
	}catch (Exception $ex) {
		watchdog_exception('getProfileSettingsColumn', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * existsEntityProfileMapingRow() - Checks whether a row in table slt_entity_profile_mapping exists for (entityType, entityId)
 */
function existsEntityProfileMapingRow($entityType, $entityId) {
	try{
		// Select table
		$selectStmt = db_select('slt_entity_profile_mapping', 'pmap');

		// Set conditions
		$selectStmt->condition('pmap.entity_type', $entityType);
		$selectStmt->condition('pmap.entity_id', $entityId);

		// Select fields
		$selectStmt->addExpression('COUNT(pmap.id)', 'count');
		expDebug::dPrintDBAPI(' $selectStmt obj = ' , $selectStmt);
		// Execute query
		$rowCount = $selectStmt->execute()->fetchField();
		expDebug::dPrint(' $rowCount = ' . print_r($rowCount, true),3);
		return ($rowCount > 0)? true : false;
	}catch (Exception $ex) {
		watchdog_exception('existsEntityProfileMapingRow', $ex);
		expertusErrorThrow($ex);
	}
}

function getCountriesAPI(){
	try{
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		return getCountries('api');
	}catch (Exception $ex) {
		watchdog_exception('addClassToCartAPI', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getCountries() - Fetch country list.
 */
function getCountries($isAPI = ''){
	try{

		if($isAPI == 'api'){
			$select = db_select('uc_countries', 'country');
			$select->addField('country','country_id','country_code');
			$select->addField('country','country_name');
			$select->orderBy('country.country_name');
			expDebug::dPrintDBAPI('select countries from uc_countries' ,$select);
			$totalRecords = $select->countQuery()->execute()->fetchField();
			$result = $select->execute()->fetchAll();
			$result['totalrow'] = $totalRecords;
			return $result ;
		}
		else
		{
			$select = db_select('slt_country', 'country');
			$select->addField('country','country_code');
			$select->addField('country','country_name');
			$select->orderBy('country.country_name');
			expDebug::dPrintDBAPI('select countries from slt_country' ,$select);
			return $select->execute()->fetchAllKeyed();
		}
	}catch (Exception $ex) {
		watchdog_exception('getCountries', $ex);
		expertusErrorThrow($ex);
	}
}


function getStatesForCountryAPI($countryCode){
	try{
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		return getStatesForCountry($countryCode,'api');

	}catch (Exception $ex) {
		watchdog_exception('addClassToCartAPI', $ex);
		expertusErrorThrow($ex);
	}
}
/*
 * getStatesForCountry($countryCode) - Fetch states list of a country
 */
function getStatesForCountry($countryCode,$isAPI = ''){
	try{
		expDebug::dPrint(' $countryCode = ' . print_r($countryCode, true),4);
		if (empty($countryCode)) {
			return array();
		}

		if($isAPI == 'api'){
			$select = db_select('uc_zones', 'state');
			$select->addField('state', 'zone_id', 'state_code');
			$select->addField('state','zone_name', 'state_name');
			$select->condition('state.zone_country_id', $countryCode, '=');
			$select->orderBy('state.zone_name');
			$select->groupBy('state.zone_name');
			expDebug::dPrintDBAPI(' $select OBJ = ' ,$select);
			return $select->execute()->fetchAll();

		}
		else
		{
			$select = db_select('slt_state', 'state');
			$select->addField('state', 'state_code');
			$select->addField('state','state_name');
			$select->condition('state.country_code', $countryCode, '=');
			$select->orderBy('state.state_name');
			$select->groupBy('state.state_name');
			expDebug::dPrintDBAPI(' $select OBJ = ' ,$select);
			return $select->execute()->fetchAllKeyed();
		}

	}catch (Exception $ex) {
		watchdog_exception('getStatesForCountry', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getDeliveryTypeList() - Fetch active delivery types.
 */
function getDeliveryTypeList(){
	try{
		$selDType = db_select('slt_profile_list_items','profList');
		$selDType->addField('profList','code');
		$selDType->addField('profList','name');
		$selDType->Condition('profList.code','%lrn_cls_dty%','like');
		$selDType->Condition('profList.code','lrn_cls_dty','<>');
		$selDType->Condition('profList.is_active','Y');
		return $selDType->execute()->fetchAllKeyed();
	}catch (Exception $ex) {
		watchdog_exception('getDeliveryTypeList', $ex);
		expertusErrorThrow($ex);
	}
}
/*
 * getProfileListItemName() - Returns the profile list item name for the profile list item code.
 */
function getProfileListItemName($itemCode, $languageCode = 'cre_sys_lng_eng',$recertify_path='') {
	try{
		if (is_null($languageCode)) {
			$languageCode = 'cre_sys_lng_eng';
		}
		
		if(!empty($recertify_path) && $recertify_path > 1) {			
			$name = "Recertification";			
		} else {		
		// Select table
		$selectStmt = db_select('slt_profile_list_items', 'pli');
		// Set conditions
		$selectStmt->condition('pli.code', $itemCode);
		$selectStmt->condition('pli.lang_code', $languageCode);
		// Select fields and/or add expressions
		$selectStmt->addField('pli', 'name', 'name');
		expDebug::dPrintDBAPI('$selectStmt = ', $selectStmt);
		// Execute query
		$result = $selectStmt->execute();
		if ($result->rowCount() <= 0) {
			expDebug::dPrint('unknown item code',3);
			return null;
		}
		$name = $result->fetchField();
		}
		expDebug::dPrint('$name = ' . $name,3);
		return $name;
	}catch (Exception $ex) {
		watchdog_exception('getProfileListItemName', $ex);
		expertusErrorThrow($ex);
	}
}

function getProfileListItemAttr($itemCode, $languageCode = 'cre_sys_lng_eng') {
	try{
		if (is_null($languageCode)) {
			$languageCode = 'cre_sys_lng_eng';
		}
		// Select table
		$selectStmt = db_select('slt_profile_list_items', 'pli');
		// Set conditions
		$selectStmt->condition('pli.code', $itemCode);
		$selectStmt->condition('pli.lang_code', $languageCode);

		// Select fields and/or add expressions
		$selectStmt->addField('pli', 'attr1', 'attr1');
		expDebug::dPrintDBAPI('$selectStmtattr = ', $selectStmt);
		// Execute query
		$result = $selectStmt->execute();
		if ($result->rowCount() <= 0) {
			expDebug::dPrint('unknown item code');
			return null;
		}
		$attr1 = $result->fetchField();
		expDebug::dPrint('$attr1 = ' . $attr1,3);
		return $attr1;
	}catch (Exception $ex) {
		watchdog_exception('getProfileListItemAttr', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * subStringBetweenTwoStrings() - Returns the first substring from a $string which
 *                                is preceded by $precedeString and succeeded by $succeedString in the string.
 *                                Returns false if either $precedeString or $succeedString is not present in the $string.
 *                                If $precedeString is null or '', assumes start of string.
 *                                If $succeedString is null or '', assumes end of string.
 */
function subStringBtwTwoStrings($string, $precedeString = null, $succeedString = null) {
	expDebug::dPrint(' subStringBtwTwoStrings() called with $string = ' . $string .
                                                                               ' $precedeString = ' . $precedeString .
                                                                               ' $succeedString = ' . $succeedString,4);
	try{
		if ($string == null) {
			return false; // There should be a string to search from
		}
		if (!empty($precedeString)) {
			$findStringStartIndex = strpos($string, $precedeString);
			if (is_bool($findStringStartIndex)) { // strpos() returns FALSE (boolean) when searched string not found in string,
				// integer otherwise which can be 0.
				return false; // $precedeString must be present in $string
			}

			$findStringStartIndex += strlen($precedeString); // Skip the $precedeString
		}
		else {
			$findStringStartIndex = 0;
		}
		expDebug::dPrint(' $findStringStartIndex = ' . print_r($findStringStartIndex, true),4);
		if (!empty($succeedString)) {
			$findStringEndIndex = strpos($string, $succeedString, $findStringStartIndex);
			if (is_bool($findStringEndIndex)) { // strpos() returns FALSE (boolean) when searched string not found in string
				// integer otherwise which can be 0.
				return false; // $succeedString must be present in $string
			}

			$findStringEndIndex--; //strpos had returned the start index of $successString and not end index of findString.
		}
		else {
			$findStringEndIndex = strlen($string) - 1 ;
		}
		expDebug::dPrint(' $findStringEndIndex = ' . print_r($findStringEndIndex, true),4);
		$lengthFindString = $findStringEndIndex - $findStringStartIndex + 1;
		return substr($string, $findStringStartIndex, $lengthFindString);
	}catch (Exception $ex) {
		watchdog_exception('subStringBtwTwoStrings', $ex);
		expertusErrorThrow($ex);
	}
}
/*
 * getCCAuthCode() - Returns a credit card purchase AuthCode for a confirmed order.
 */
function getCCAuthCode($orderId) {
	try{
		// Select table
		$selectStmt = db_select('slt_payment_log', 'plog');
		// Join other tables.
		$selectStmt->leftJoin('slt_order', 'ord', 'plog.order_id = ord.id');
		// Set conditions
		$selectStmt->condition('plog.order_id', $orderId);
		$selectStmt->condition('ord.order_status', 'cme_pmt_sts_cnm');
		$selectStmt->condition('plog.type', 'CC');
		// Select fields and/or add expressions
		$selectStmt->addField('plog', 'responsexml', 'responsexml');
		$selectStmt->addField('ord', 'order_total_amt', 'order_total_amt');
		// Limit to 1 record (the first record)
		$selectStmt->range(0, 1);
		expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);
		// Execute query
		$result = $selectStmt->execute();
		expDebug::dPrint(' $result = ' . print_r($result, true),3);
		if ($result->rowCount() <= 0) {
			return '';
		}
		$record = $result->fetchAssoc(); //fetch first row in result set as an assosiative array
		expDebug::dPrint(' $record = ' . print_r($record, true),4);

		$reply = $record['responsexml'];
		$orderTotalAmount = $record['order_total_amt'];

		if ($reply == null || $reply == '' || $orderTotalAmount == 0 ) {
			return '';
		}

		$reply = str_replace(' ', '', $reply);
		$authCode = subStringBtwTwoStrings($reply, 'authorizationCode%3E', '%3C');
		expDebug::dPrint(' $authCode = ' . print_r($authCode, true),4);
		if (!is_string($authCode)) { // Must be boolean with value FALSE.
			return '';
		}
		return $authCode;
	}catch (Exception $ex) {
		watchdog_exception('getCCAuthCode', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getPaymentMethodName() - Returns profile name or profile code of learner order type
 */
function getPaymentMethodName($lnrOrderType, $returnType) {
	try{
		$orderTypeCode = $lnrOrderType;
		if ($lnrOrderType == 'check') {
			$orderTypeCode = 'cme_pmt_typ_chk';
		}
		elseif ($lnrOrderType == 'po') {
			$orderTypeCode = 'cme_pmt_typ_por';
		}
		elseif ($lnrOrderType == 'credit') {
			$orderTypeCode = 'cme_pmt_typ_crd';
		}
		elseif ($lnrOrderType == 'bank_transfer') {
			$orderTypeCode = 'cme_pmt_typ_bnt';
		}
		elseif ($lnrOrderType == 'other') {
			$orderTypeCode = 'cme_pmt_typ_acn';
		}
		elseif ($lnrOrderType == 'Zero Cost') {
			$orderTypeCode = 'Zero Cost';
		}

		if ($returnType == 'name' && $orderTypeCode != 'Zero Cost') {
			$orderTypeName = getProfileListItemName($orderTypeCode); // Language defaults to 'cre_sys_lng_eng'
			return $orderTypeName;
		}
		// Else return $orderTypeCode
		return $orderTypeCode;
	}catch (Exception $ex) {
		watchdog_exception('getPaymentMethodName', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getPaymentStatusName() - Returns profile name or profile code for payment status
 */
function getPaymentStatusName($paymentStatus, $returnType) {
	try{
		$paymentStatusCode = $paymentStatus;
		if ($paymentStatus == 'completed') {
			$paymentStatusCode = 'cme_pmt_sts_cnm';
		}
		elseif ($paymentStatus == 'pending') {
			$paymentStatusCode = 'cme_pmt_sts_pnd';
		}
		elseif ($paymentStatus == 'payment_received') {
			$paymentStatusCode = 'cme_pmt_sts_cnm';
		}
		elseif ($paymentStatus == 'canceled') {
			$paymentStatusCode = 'cme_pmt_sts_rjt';
		}
		if ($returnType == 'name') {
			$paymentStatusName = getProfileListItemName($paymentStatusCode); // Language defaults to 'cre_sys_lng_eng'
			if ($paymentStatusName != null) {
				return $paymentStatusName;
			}
		}
		// Else return $paymentStatusCode
		return $paymentStatusCode;
	}catch (Exception $ex) {
		watchdog_exception('getPaymentStatusName', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * dateTimeStrToUSShort() -  Converts date/datetime to US short date/datetime notation
 * e.g. Converts 2011-09-19 13:30:00 to 09/19/2011 13:30 ($year4 = true, $showTime = true, $time24= true, $showSeconds == false)
 * Note: This function also accepts UNIX timestamps as $dateTimeString. Prefix UNIX timestamp with @.
 *       e.g. '@1309943773' -> 07/06/2011
 */
function dateTimeStrToUSShort($dateTimeString, $year4, $showTime, $time24, $showSeconds) {
	try{
		if (empty($dateTimeString)) {
			return '';
		}
		$dateTime = new DateTime($dateTimeString);
		$yearCode = $year4 ? 'Y' : 'y';
		if ($showTime) {
			if ($showSeconds) {
				if ($time24) {
					return $dateTime->format('m/d/' . $yearCode . ' H:i:s');
				}
				// Show time in 12 hour format
				return $dateTime->format('m/d/' . $yearCode . ' h:i:s a');
			}
			// Don't show seconds
			if ($time24) {
				return $dateTime->format('m/d/' . $yearCode . ' H:i');
			}
			// Show time in 12 hour format
			return $dateTime->format('m/d/' . $yearCode . ' h:i a');
		}
		// Don't show time
		return $dateTime->format('m/d/'. $yearCode);
	}catch (Exception $ex) {
		watchdog_exception('dateTimeStrToUSShort', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * dateTimeStrToUnivShort() -  Converts date/datetime to Universal short date/datetime notation
 * e.g. Converts 2011-09-19 13:30:00 to Sep 19, 2011 13:30 ($year4 = true, $showTime = true, $time24= true, $showSeconds == false)
 * Note: This function also accepts UNIX timestamps as $dateTimeString. Prefix UNIX timestamp with @.
 *       e.g. '@1309943773' -> Dec 01, 2011
 */
function dateTimeStrToUnivShort($dateTimeString, $year4, $showTime, $time24, $showSeconds){
	try{
		if (empty($dateTimeString)) {
			return '';
		}
		$dateTime = new DateTime($dateTimeString);
		$yearCode = $year4 ? 'Y' : 'y';
		if ($showTime) {
			if ($showSeconds) {
				if ($time24) {
					return $dateTime->format('M d, ' . $yearCode . ' H:i:s');
				}
				// Show time in 12 hour format
				return $dateTime->format('M d, ' . $yearCode . ' h:i:s A');
			}
			// Don't show seconds
			if ($time24) {
				return $dateTime->format('M d, ' . $yearCode . ' H:i');
			}
			// Show time in 12 hour format
			return $dateTime->format('M d, ' . $yearCode . ' h:i A');
		}
		// Don't show time
		return $dateTime->format('M d, '. $yearCode);
	}catch (Exception $ex) {
		watchdog_exception('dateTimeStrToUnivShort', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Function to convert DB date (date only) to DB datetime
 *
 * @param string $date_field,$date_field_end
 * $date_field     - Date
 *
 * @return
 *   Returns Formatted Date
 */
function convertDateToDBDateFormat($date_field, $date_field_end=false) {
	try{
		if ($date_field_end){
			$hr=23;$min=59;$sec=59;
		} else {
			$hr=0;$min=0;$sec=0;
		}
		$sDate = explode("-",$date_field);
		$month = $sDate[0];
		$day   = $sDate[1];
		$year  = $sDate[2];
		$date_value = Date('Y-m-d H:i:s',mktime($hr, $min, $sec, $month, $day, $year));
		return $date_value;
	}catch (Exception $ex) {
		watchdog_exception('convertDateToDBDateFormat', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * convertNullValToNullStr - If $string is null, returns null string. Otherwise, returns string.
 */
function convertNullValToNullStr ($string) {
	try{
		if (is_null($string)) {
			return '';
		}
		return $string;
	}catch (Exception $ex) {
		watchdog_exception('convertNullValToNullStr', $ex);
		expertusErrorThrow($ex);
	}
}
/*
 * convertNullValToCurrency - If value is null, returns 0.00. Otherwise, returns value.
 */
function convertNullValToCurrency($value) {
	try{
		if (is_null($value)) {
			return 0.00;
		}

		return $value;
	}catch (Exception $ex) {
		watchdog_exception('convertNullValToNullStr', $ex);
		expertusErrorThrow($ex);
	}
}
/*
 * getCurrencyDetails - Gets currency_prefix, currency_postfix, name and id for a currency code from slt_profile_list_items
 */
function getCurrencyDetails($currencyCode, $languageCode = 'cre_sys_lng_eng') {
	try{
		if (is_null($languageCode)) {
			$languageCode = 'cre_sys_lng_eng';
		}
		// Select table
		$selectStmt = db_select('slt_profile_list_items', 'pli');
		// Set conditions
		$selectStmt->condition('pli.attr1', $currencyCode);
		$selectStmt->condition('pli.lang_code', $languageCode);
		$selectStmt->condition('pli.is_active', 'Y');
		// Select fields and/or add expressions
		$selectStmt->addField('pli', 'id', 'id');
		$selectStmt->addField('pli', 'name', 'name');
		$selectStmt->addField('pli', 'attr2', 'prefix_symbol');
		$selectStmt->addField('pli', 'attr1', 'postfix_symbol');
		// Execute query
		$result = $selectStmt->execute();
		if ($result->rowCount() <= 0) {
			return null;
		}
		return $result->fetchAssoc();
	}catch (Exception $ex) {
		watchdog_exception('convertNullValToNullStr', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * formatAmount() - Formats an amount string to specified format prefixing and optionally postfixing the currency prefix and postfix symbols respectively.
 */
function formatAmount($amount,
$includePostFixSymbol = false,
$currencyCode = 'usd', // Passed onto getCurrencyDetails()
$languageCode = 'cre_sys_lng_eng', // Passed onto getCurrencyDetails()
$decimals = 2, // Passed onto PHP number_format()
$dec_point = '.', // Passed onto PHP number_format()
$thousands_sep = ',' // Passed onto PHP number_format()
) {
	try{
		//Commented for issue when the amount is zero.
		/*if(empty($amount)) {
			return '';
		}*/
		$currencyDetails = getCurrencyDetails($currencyCode, $languageCode);
		expDebug::dPrint(' $currencyDetails = ' . print_r($currencyDetails, true),4);

		$prefixSymbol = '';
		$postfixSymbol = '';

		if ($currencyDetails != null) {
			$prefixSymbol = convertNullValToNullStr($currencyDetails['prefix_symbol']);
			$postfixSymbol = convertNullValToNullStr($currencyDetails['postfix_symbol']);
		}
		if(!empty($amount)) {
			$formattedAmount = $currencyCode .' '.number_format($amount, $decimals, $dec_point, $thousands_sep);

		if ($includePostFixSymbol) {
			$formattedAmount = $formattedAmount . ' ' . $postfixSymbol;
		}
		}else{
			$formattedAmount = $currencyCode.' '.'0.00';
		}
		return $formattedAmount;
	}catch (Exception $ex) {
		watchdog_exception('formatAmount', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getColumnNameForProfileField() - Replaces MySQL function slf_profileconfig_col_sel(). Returns column name for the profile field.
 */

function getColumnNameForProfileField($categoryName, $profileFieldName) {
	try{
		// Select table
		$selectStmt = db_select('slt_profile_list_items', 'pli');
		// Set conditions
		$selectStmt->condition('pli.name', $categoryName);
		// Select fields and/or add expressions
		$selectStmt->addField('pli', 'id', 'id');
		expDebug::dPrintDBAPI(' $selectStmt = ' ,$selectStmt);
		// Execute query
		$result = $selectStmt->execute();
		$categoryId = $result->fetchField();

		if ($categoryId == false) {
			return null;
		}
		// Select table
		$selectStmt = db_select('slt_profile_list_items', 'pli');
		// Set conditions
		$selectStmt->condition('pli.parent_id', $categoryId);
		$selectStmt->condition('pli.name', '%' . $profileFieldName . '%', 'LIKE');

		// Select fields and/or add expressions
		$selectStmt->addField('pli', 'id', 'id');

		// Limit to 1 record (the first record)
		$selectStmt->range(0, 1);

		expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);

		// Execute query
		$result = $selectStmt->execute();

		$profileFieldId = $result->fetchField();

		if ($profileFieldId == false) {
			return null;
		}

		// Select table
		$selectStmt = db_select('slt_profile_config', 'spc');

		// Set conditions
		$selectStmt->condition('spc.category_id', $categoryId);
		$selectStmt->condition('spc.profile_field_id', $profileFieldId);

		// Select fields and/or add expressions
		$selectStmt->addField('spc', 'cols', 'cols');

		expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);

		// Execute query
		$result = $selectStmt->execute();
		$columnNumber = $result->fetchField();

		if ($columnNumber == false) {
			return null;
		}
		return 'col' . $columnNumber;
	}catch (Exception $ex) {
		watchdog_exception('getColumnNameForProfileField', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getLocationDetails() - Fetches specified column values ($fields) of a location from slt_location
 */
function getLocationDetails($locationId, $fields) {
	try{
		if (empty($fields)) {
			// Get all fields
			$fields = array('id', 'name', 'contact_fname', 'contact_lname', 'addr1', 'addr2', 'city', 'state', 'country',
    					'zipcode', 'phone', 'direction', 'latitude', 'longitude', 'timezone', 'gmtoffset', 'dstoffset',
    						'is_active', 'status', 'custom0', 'custom1', 'custom2', 'custom3', 'custom4');
		}
		// Select table
		$selectStmt = db_select('slt_location', 'loc');
		// Set conditions
		$selectStmt->condition('loc.id', $locationId);
		// Select fields and/or add expressions
		foreach($fields as $fieldName) {
			$selectStmt->addField('loc', $fieldName, $fieldName);
		}
		expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);
		// Execute query
		$result = $selectStmt->execute();
		return $result->fetchAssoc();
	}catch (Exception $ex) {
		watchdog_exception('getLocationDetails', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getUpdatedByDetails() - Fetches whether the given userid is Instructor / Manager / Self
 */
function getUpdatedByDetails($userId, $updatedUserId){
	try{
		if($userId == $updatedUserId){
			$updatedby = t('Me');//self
		}else if($updatedUserId == 'Feed'){
			$updatedby = t('Feed');//Feed
		} else if(is_instructor($updatedUserId)){
			$updatedby  =t('Instructor'); //instructor
		} else if(is_manager($updatedUserId)){
			$updatedby = t('Manager'); //Manager
		} else {
			$updatedby = t('Admin');//admin
		}
		expDebug::dPrint('Updated by details : '.$userId. ' '. $updatedUserId. ' ' .$updatedby,4);
		return $updatedby;
	}catch (Exception $ex) {
		watchdog_exception('getUpdatedByDetails', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getPersonDetails() - Fetches specified details ($fields) of a person from slt_person
 */
function getPersonDetails($personId, $fields,$page='') {
	try{
		if(!$personId && empty($page)){
			return array();
		}
		$fields1 = array('id', 'first_name', 'last_name', 'middle_name', 'full_name', 'user_name', 'phone_no', 'mobile_no', 'sms_alert',
    				'status', 'user_type', 'employment_type', 'job_title', 'hire_date', 'terminated_date',
    				'is_rehire', 'is_instructor','is_manager','email', 'manager_id', 'org_id','dept_code','dotted_mngr_id','dotted_org_id','additional_info','is_instructor',
    				'addr1', 'addr2', 'city', 'state', 'country', 'zip', 'time_zone', 'preferred_loc_id', 'preferred_language', 'location_name','employee_no',
                    'stats_value1', 'stats_value2', 'stats_value3', 'stats_value4', 'stats_value5', 'stats_value6',
                    'stats_value7', 'stats_value8', 'stats_value9', 'stats_value10',
                    'custom0', 'custom1', 'custom2', 'custom3', 'custom4','is_mobileadmin','preferred_currency','attr2','attr1','attr4','webex_name','webex_pass');
		// Added by Vincent for performance imporvement on Sep 3, 2015
		$uid = getIdOfLoggedInUser();
		if($personId == $uid && isset($_SESSION['person_detail']) && count($_SESSION['person_detail'])){
			if (empty($fields)){
				return $_SESSION['person_detail'];
			}else{
				foreach($fields as $fieldName) {
					$detail[$fieldName] = $_SESSION['person_detail'][$fieldName];
				}
				return $detail;
			}
		}

		// Select table
		$selectStmt = db_select('slt_person', 'per');
		// Fetch time zone
		$selectStmt->leftJoin('slt_profile_list_items', 'pitem','per.time_zone = pitem.code');
		// Set conditions
		$selectStmt->condition('per.id', $personId);
		// Select fields and/or add expressions
		foreach($fields1 as $fieldName) {
			if($fieldName=='dotted_mngr_id')
				$selectStmt->addExpression('(select group_CONCAT(per2.user_name  separator \', \') from slt_person as per2 left join slt_person_other_manager as spom on per2.id in(spom.manager_id) where spom.user_id = '.$personId.' and spom.is_direct=\'N\')', $fieldName);
			if($fieldName=='dotted_org_id')
				$selectStmt->addExpression('(select group_CONCAT(org2.name  separator \', \') from slt_organization as org2 left join slt_person_other_organization as spoo on org2.id in(spoo.organization_id) where spoo.user_id = '.$personId.' and spoo.is_direct=\'N\')', $fieldName);
			if($fieldName =='attr2' || $fieldName =='attr1' || $fieldName =='attr4' ){
				$selectStmt->addField('pitem', $fieldName, $fieldName);
			}else{
				$selectStmt->addField('per', $fieldName, $fieldName);
			}
		}
		expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);
		// Execute query
		$result = $selectStmt->execute();
		$result = $result->fetchAssoc();
		expDebug::dPrint('$result = ' . print_r($result, true), 4);
		if($personId == $uid) $_SESSION['person_detail'] = $result;
		return $result;
	}
	catch (Exception $ex) {
		watchdog_exception('getPersonDetails', $ex);
		if (isset($_GET['cron_key'])) {
			expertusLogError($ex);
		}
		else {
			expertusErrorThrow($ex);
		}
	}
}
/**
 * If the currency is configured for default_profile_currency in exp_sp.ini & is_valid currency then return default_profile_currency
 * else return the system default currency
 * @return A
 */
function getCurrencyDefDetails(){
	try{
		$config = getConfig("exp_sp");
		$default_profile_currency = isset($config["default_profile_currency"]) ? trim($config["default_profile_currency"]) : '';
		$currencyCode = strtolower('cre_sys_crn_'.$default_profile_currency);

		if(empty($default_profile_currency) || empty(currency_isValidOrNot($currencyCode))){
			$selectQuery = db_select('slt_profile_list_items', 'spli');
			$selectQuery->addField('spli','attr1');
			$selectQuery->condition('spli.is_active', 'Y', '=');
			$selectQuery->condition('spli.attr4', 'Y', '=');
			$default_profile_currency = $selectQuery->execute()->fetchField();
			expDebug::dPrint("Default Prof currency-->>> " .print_r($default_user_currency,true),5);
		}

		return $default_profile_currency;
	} catch (Exception $ex) {
	watchdog_exception('getCurrencyDefDetails', $ex);
	expertusErrorThrow($ex);
	}
}

/**
 * If the $allow_currency_change is not configured in the exp_sp.ini then return false
 * else true
 * @return boolean
 */

function restrict_user_currency_select(){
	try{
		$config = getConfig("exp_sp");
		if(isset($config["allow_currency_change"])){
			$allow_currency_change = ($config["allow_currency_change"] == 1) ? FALSE : TRUE ;
		}else{
			$allow_currency_change = TRUE;
		}
		expDebug::dPrint("get restrict_user_currency_select --> " .var_export($allow_currency_change, 1),4);

		return $allow_currency_change;
	}catch (Exception $ex) {
		watchdog_exception('restrict_user_currency_select', $ex);
		expertusErrorThrow($ex);
	}
}

function getSecurityDefDetails($pid,$fields)
{
	try{
		if (!empty($pid)) {
			// Select table
			$selectStmt = db_select('slt_groups', 'grp');
			$selectStmt->join('slt_person','per');
			$selectStmt->leftjoin('slt_person_jobrole_mapping','lpjm','lpjm.user_id = per.id');
			$selectStmt->leftjoin('slt_group_attributes','grpatt','grpatt.group_id = grp.id');
			// Select fields and/or add expressions
			$selectStmt->addField('grp', 'name', 'name');
			$selectStmt->addField('grp', 'id', 'id');
			$selectStmt->where("(((((if(grp.org_id='All',per.org_id is not null,grp.org_id is null OR FIND_IN_SET(per.org_id,grp.org_id)>0))
      		AND (if(grp.user_type='All',per.user_type is not null,grp.user_type is null OR FIND_IN_SET(ifnull(per.user_type,''),grp.user_type)>0 ))
      		AND (if(grp.employment_type='All',per.employment_type is not null,grp.employment_type is null OR FIND_IN_SET(ifnull(per.employment_type,''),grp.employment_type)>0))
      		AND (if(grp.country='All',per.country is not null,grp.country is null OR FIND_IN_SET(ifnull(per.country,''),grp.country)>0))
      		AND (if(grp.state='All',per.state is not null,grp.state is null OR FIND_IN_SET(ifnull(concat(per.country,'-',per.state),''),grp.state)>0))
      		AND (if(grp.department='All',per.dept_code is not null,grp.department is null OR FIND_IN_SET(ifnull(per.dept_code,''),grp.department)>0))
      		AND (if(grp.job_role='All',(select count(1) from slt_person_jobrole_mapping as jobmap where jobmap.user_id = per.id)>0,grp.job_role is null OR FIND_IN_SET(ifnull(lpjm.job_role,''),ifnull(grp.job_role,''))>0))
      		AND (if(grp.language='All',per.preferred_language is not null,grp.language is null OR FIND_IN_SET(ifnull(per.preferred_language,''),grp.language)>0))
      		AND (CASE WHEN (grp.is_manager='Y' AND grp.is_instructor='Y')
                THEN
                (ifnull(per.is_manager,'N') = grp.is_manager or ifnull(per.is_instructor,'N') = grp.is_instructor)
                WHEN (grp.is_manager='Y' AND grp.is_instructor='N')
                THEN
                (ifnull(per.is_manager,'N') = grp.is_manager)
                WHEN (grp.is_manager='N' AND grp.is_instructor='Y')
                THEN
                (ifnull(per.is_instructor,'N') = grp.is_instructor)
                ELSE
                1=1 END)
			AND (
					if(grpatt.id is not null,if(grpatt.on_or_after_start_date is not null ,DATE_FORMAT(grpatt.on_or_after_start_date,'%Y-%m-%d') <= DATE_FORMAT(per.hire_date,'%Y-%m-%d'),1=0) OR 
            		if(grpatt.on_or_before_start_date is not null, DATE_FORMAT(grpatt.on_or_before_start_date,'%Y-%m-%d') >= DATE_FORMAT(per.hire_date,'%Y-%m-%d'), 1=0) OR
					if(grpatt.between_start_date is not null ,DATE_FORMAT(per.hire_date,'%Y-%m-%d') between DATE_FORMAT(grpatt.between_start_date,'%Y-%m-%d') AND DATE_FORMAT(grpatt.between_end_date,'%Y-%m-%d'), 1=0),1=1)  
				)		
      		AND (grp.removed_users is null OR FIND_IN_SET(per.id,grp.removed_users)<= 0)) OR (FIND_IN_SET(per.id,grp.added_users)>0))))");
			// Set conditions
			$selectStmt->condition('grp.status', 'cre_sec_sts_atv', '=');
			$selectStmt->condition('per.id', $pid, '=');
			$selectStmt->condition('grp.code', 'grp_adm', '!=');

			expDebug::dPrintDBAPI(' $selectStmt obj =' , $selectStmt);

			$union = db_select('slt_groups', 'grp');
			$union->join('slt_person','per');
			// Select fields and/or add expressions
			$union->addField('grp', 'name', 'name');
			$union->addField('grp', 'id', 'id');
			$union->where("(FIND_IN_SET(per.id,grp.added_users)>0 and (grp.removed_users is null OR FIND_IN_SET(per.id,grp.removed_users)<= 0))");
			// Set conditions
			$union->condition('grp.status', 'cre_sec_sts_atv', '=');
			$union->condition('per.id', $pid, '=');

			//expDebug::dPrintDBAPI(' $$union obj =' , $union);
			// Execute query
			$resultQuery = $selectStmt->union($union,'UNION ALL');
			$result = $resultQuery->execute();
			$result1 = $result->fetchAll();
			return $result1;
			expDebug::dPrintDBAPI(' get security defined details ' , $resultQuery);

		}
	}catch (Exception $ex) {
		watchdog_exception('getSecurityDefDetails', $ex);
		expertusErrorThrow($ex);
	}
}

function getSecurityDetails($pid,$fields)
{
	try{
		if (empty($fields)) {
			$fields = array('id','name');
		}
		$selectStmt = db_select('slt_groups', 'grp');
		$selectStmt->condition('grp.status',array('cre_sec_sts_itv','cre_sec_sts_del'),'NOT IN');
		foreach($fields as $fieldName) {
			$selectStmt->addField('grp', $fieldName, $fieldName);
		}
		expDebug::dPrintDBAPI(' $selectStmt ', $selectStmt);
		$result = $selectStmt->execute()->fetchAll();
		expDebug::dPrint(' $result '.print_r($result,true),4);
		return $result;
	}catch (Exception $ex) {
		watchdog_exception('getSecurityDetails', $ex);
		expertusErrorThrow($ex);
	}
}


/*
 * getOrganizationDetails() - Fetches specified details ($fields) of an organization from slt_organization
 */
function getOrganizationDetails($organizationId, $fields) {
	try{
		if (empty($fields)) {
			// Get all fields
			$fields = array('id', 'number', 'name', 'parent_org_id', 'parent_org_name', 'type', 'status',
					'add1', 'add2', 'city', 'state', 'country', 'zip', 'cost_center', 'contact_id',
					'contact_name', 'sales_rep_id', 'sales_rep_name', 'phone_no', 'fax_no', 'email',
					'website', 'stats1', 'stats1_desc', 'stats2', 'stats2_desc', 'stats3', 'stats3_desc',
					'stats4', 'stats4_desc', 'stats5', 'stats5_desc', 'stats6', 'stats6_desc', 'stats7', 'stats7_desc', 'stats8',
					'stats8_desc', 'stats9', 'stats9_desc', 'stats10', 'stats10_desc',
					'custom0', 'custom1', 'custom2', 'custom3', 'custom4');
		}

		// Select table
		$selectStmt = db_select('slt_organization', 'org');

		// Set conditions
		$selectStmt->condition('org.id', $organizationId);

		// Select fields and/or add expressions
		foreach($fields as $fieldName) {
			$selectStmt->addField('org', $fieldName, $fieldName);
		}
		expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);
		// Execute query
		$result = $selectStmt->execute();

		return $result->fetchAssoc();
	}catch (Exception $ex) {
		watchdog_exception('getOrganizationDetails', $ex);
		expertusErrorThrow($ex);
	}
}
/*
 * getDropPolicyDetails() - Fetches specified details ($fields) from slt_drop_policy
 */
function getDropPolicyDetails($entityId, $fields) {
	try{
		if (empty($fields)) {
			// Get all fields
			$fields = array('id', 'entity_id', 'entity_type', 'delivery_type', 'days', 'refund_percentage');
		}
		// Select table
		$selectStmt = db_select('slt_drop_policy', 'sdp');

		// Select fields and/or add expressions
		foreach($fields as $fieldName) {
			$selectStmt->addField('sdp', $fieldName, $fieldName);
		}
		// Execute query
		$result = $selectStmt->execute();
		return $result->fetchAll();
	}catch (Exception $ex) {
		watchdog_exception('getDropPolicyDetails', $ex);
		expertusErrorThrow($ex);
	}
}
/*
 * getIdOfLoggedInUser - Returns id of the logged in user (id is fetched from slt_person table).
 */
function getIdOfLoggedInUser() {
	try{
		global $user;
		// Added by Vincent for performance imporvement on Sep 2, 2015
		if($user->uid == 0 && !$_REQUEST['apiname']) return 0;
		
		if($user->uid == 0 && ($_REQUEST['apiname'] || $_REQUEST['mobile_request'])) return $_REQUEST['userid'];
		
		
		if(isset($_SESSION['logged_user_id']) && !empty($_SESSION['logged_user_id']))
		{
			drupal_add_js(array('user_prefference' => array('currency_code' =>  $_SESSION['user_preferred_currency_code'], 'currency_sym' =>  $_SESSION['user_preferred_currency_sym'])), 'setting');
			return $_SESSION['logged_user_id'];
		}

		expDebug::dPrint(' $user->name = ' . print_r($user->name, true),4);
		$personId = 0;
		if (!empty($user->name)) {
			// Select table
			unset($_SESSION['user_preferred_currency_code']);
			unset($_SESSION['user_preferred_currency_sym']);

			$selectStmt = db_select('slt_person', 'per');
			$selectStmt->leftjoin('slt_profile_list_items', 'spli', 'spli.attr1 = per.preferred_currency AND spli.code LIKE \'cre_sys_crn_%\' AND spli.is_active=\'Y\' AND spli.attr3=\'Y\'');
			// Set conditions
			$selectStmt->condition('per.user_name', $user->name);

			//Amaran: Added status is active condition only condtion:0041257
			$selectStmt->condition('per.status', 'cre_usr_sts_atv');

			// Select fields and/or add expressions
			$selectStmt->addField('per', 'id');
			$selectStmt->addField('per','preferred_currency','preferred_currency');
			$selectStmt->addField('spli','attr2','attr2');

			expDebug::dPrintDBAPI(' $selectStmt SQL = ' , $selectStmt);

			// Execute query
			$result = $selectStmt->execute()->fetchAssoc();

			$personId = empty($result['id'])? 0 : $result['id'];
			$_SESSION['logged_user_id'] = $personId;

			$getDefaultCurrency = getDefaultCurrency();
			$UserPrefCurrCode = $getDefaultCurrency['preferred_currency'];
			$UserPrefCurrSym  = $getDefaultCurrency['attr2'];

			$_SESSION['user_preferred_currency_code'] = empty($result['preferred_currency'])? $UserPrefCurrCode : $result['preferred_currency'];
			$_SESSION['user_preferred_currency_sym'] = empty($result['attr2'])? $UserPrefCurrSym : $result['attr2'];

			if(empty($_SESSION['user_preferred_currency_sym']))
			{
				$_SESSION['user_preferred_currency_sym'] = $_SESSION['user_preferred_currency_code'];
			}
			drupal_add_js(array('user_prefference' => array('currency_code' =>  $_SESSION['user_preferred_currency_code'], 'currency_sym' =>  $_SESSION['user_preferred_currency_sym'])), 'setting');
		}
		expDebug::dPrint(' returning $personId = ' . print_r($personId, true),3);
		return $personId;
	}catch (Exception $ex) {
		watchdog_exception('getIdOfLoggedInUser', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getIdForUserName - Get slt_person table id for $userName
 */
function getIdForUserName($userName) {
	try{
		expDebug::dPrint(' $userName = ' . print_r($userName, true),4);
		// Select table
		$selectStmt = db_select('slt_person', 'per');
		// Set conditions
		$selectStmt->condition('per.user_name', $userName);
		// Select fields and/or add expressions
		$selectStmt->addField('per', 'id');
		expDebug::dPrintDBAPI(' $selectStmt SQL = ' , $selectStmt);
		// Execute query
		$result = $selectStmt->execute()->fetchField();
		$personId = empty($result)? 0 : $result;
		expDebug::dPrint(' returning $personId = ' . $personId,3);
		return $personId;
	}catch (Exception $ex) {
		watchdog_exception('getIdForUserName', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getUserIdFromEmail - Get slt_person table id for $email
 */
function getUserIdFromEmail($email) {
	try{
		expDebug::dPrint(' $email = ' . print_r($email, true),4);
		// Select table
		$selectStmt = db_select('slt_person', 'per');
		// Set conditions
		$selectStmt->condition('per.email', $email);
		// Select fields and/or add expressions
		$selectStmt->addField('per', 'id');
		expDebug::dPrintDBAPI(' $selectStmt SQL = ' , $selectStmt);
		// Execute query
		$result = $selectStmt->execute()->fetchField();
		$personId = empty($result)? 0 : $result;
		expDebug::dPrint(' returning $personId = ' . $personId,3);
		return $personId;
	}catch (Exception $ex) {
		watchdog_exception('getUserIdFromEmail', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getUserIdFromDrupalUid - Returns slt_person id for uid in users table.
 */
function getUserIdFromDrupalUid($drupalUid) {
	try{
		// Select table
		$selectStmt = db_select('users', 'u');
		$selectStmt->leftJoin('slt_person', 'p', 'u.name = p.user_name');
		// Set conditions
		$selectStmt->condition('u.uid', $drupalUid, '=');
		// Select fields and/or add expressions
		$selectStmt->addField('p', 'id', 'id');
		expDebug::dPrintDBAPI(' $selectStmt obj = ' , $selectStmt);
		// Execute query
		$id = $selectStmt->execute()->fetchField();
		expDebug::dPrint(' returning $id = ' . $id . 'for $drupalUid = ' . $drupalUid,4);
		return $id;
	}catch (Exception $ex) {
		watchdog_exception('getUserIdFromDrupalUid', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getDrupalIdOfUser - Returns Drupal uid for slt_person table id.
 */
function getDrupalIdOfUser($personId) {
	try{
		// Added by Vincent for performance imporvement on Sep 3, 2015
		$user = getIdOfLoggedInUser();
		if($personId == $user && isset($_SESSION['drpal_userid']) && !empty($_SESSION['drpal_userid']))
			return $_SESSION['drpal_userid'];
		$uid = 0;
		if (!empty($personId)) {
			// Select table
			$selectStmt = db_select('slt_person', 'p');
			$selectStmt->leftJoin('users', 'u', 'p.user_name = u.name');
			// Set conditions
			$selectStmt->condition('p.id', $personId, '=');
			// Select fields and/or add expressions
			$selectStmt->addField('u', 'uid', 'uid');
			expDebug::dPrintDBAPI(' $selectStmt obj = ' , $selectStmt);
			// Execute query
			$result = $selectStmt->execute()->fetchField();
			$uid = empty($result)? 0 : $result;
		}
		expDebug::dPrint(' returning $uid = ' . $uid . 'for $personId = ' . $personId,3);
		if($personId == $user) $_SESSION['drpal_userid'] = $uid;
		return $uid;
	}catch (Exception $ex) {
		watchdog_exception('getDrupalIdOfUser', $ex);
		expertusErrorThrow($ex);
	}
}
/*
 *
- Fetch a user's attribte values from slt_person_jobrole_mapping.
*/
function getUserAttributeValues($userId) {
	try{
		expDebug::dPrint(' $userId = ' . print_r($userId, true),4);
		// Added by Vincent for performance imporvement on Sep 3, 2015
		$uid = getIdOfLoggedInUser();
		if($userId == $uid && isset($_SESSION['job_role']))
			return $_SESSION['job_role'];
		$userAccess = null;
		if ($userId > 0) {
			$selectjob = db_select('slt_person_jobrole_mapping','spam');
			$selectjob->addField('spam','job_role', 'job_role');
			$selectjob->condition('spam.user_id',$userId,'=');
			expDebug::dPrintDBAPI('Job ROLE sql query-->>',$selectjob);
			$userAccess = $selectjob->execute()->fetchCol();
			expDebug::dPrint(' $result = ' . print_r($userAccess, true),4);
		}
		if($userId == $uid) $_SESSION['job_role'] = $userAccess;

		return $userAccess;
	}catch (Exception $ex) {
		watchdog_exception('getUserAttributeValues', $ex);
		expertusErrorThrow($ex);
	}
}
function getUserJobrole($userId){
	$userJobrole = null;
	// Added by Vincent for performance imporvement on Sep 3, 2015
	$uid = getIdOfLoggedInUser();
	//	55932: Multiple Job Role filter is broken in Catalog page
	//	Session variable name was renamed to aviod getting reset in another function
	if($userId == $uid && isset($_SESSION['user_job_role']) && !empty($_SESSION['user_job_role']))
		return $_SESSION['user_job_role'];
	if ($userId > 0) {
		$selectjob = db_select('slt_person_jobrole_mapping','spam');
		$selectjob->leftJoin('slt_profile_list_items','spli','spam.job_role=spli.code');
		$selectjob->addField('spli','name', 'name');
		$selectjob->addField('spam','job_role', 'job_role');
		$selectjob->condition('spam.user_id',$userId,'=');
		expDebug::dPrintDBAPI('Job ROLE sql query-->>',$selectjob);
		$userJobrole = $selectjob->execute()->fetchAll();
		expDebug::dPrint(' $result = ' . print_r($userJobrole, true),4);
	}
	if($userId == $uid) $_SESSION['user_job_role'] = $userJobrole;

	return $userJobrole;
}


/*
 *
- Fetch a user's attribte values from slt_person_other_manager.
*/
function getUserOthManagerIds($userId) {
	try{
		expDebug::dPrint(' $userId = ' . print_r($userId, true),4);
		$userAccess = null;
		if ($userId > 0) {
			$selectjob = db_select('slt_person_other_manager','spom');
			$selectjob->addField('spom','manager_id', 'manager_id');
			$selectjob->condition('spom.user_id',$userId,'=');
			$selectjob->condition('spom.is_direct','N','=');
			expDebug::dPrintDBAPI('Other Manager sql query-->>',$selectjob);
			$userAccess = $selectjob->execute()->fetchCol();
			expDebug::dPrint(' $result = ' . print_r($userAccess, true),4);
		}
		return $userAccess;
	}catch (Exception $ex) {
		watchdog_exception('getUserAttributeValues', $ex);
		expertusErrorThrow($ex);
	}
}
/*
 *
- Fetch a user's attribte values from slt_person_other_manager.
*/
function getUserOtherOrganizationIds($userId) {
	try{
		expDebug::dPrint(' $userId = ' . print_r($userId, true),4);
		$userAccess = null;
		if ($userId > 0) {
			$selectjob = db_select('slt_person_other_organization','spoo');
			$selectjob->addField('spoo','organization_id', 'organization_id');
			$selectjob->condition('spoo.user_id',$userId,'=');
			$selectjob->condition('spoo.is_direct','N','=');
			expDebug::dPrintDBAPI('Other organization sql query-->>',$selectjob);
			$userAccess = $selectjob->execute()->fetchCol();
			expDebug::dPrint(' $result = ' . print_r($userAccess, true),4);
		}
		return $userAccess;
	}catch (Exception $ex) {
		watchdog_exception('getUserAttributeValues', $ex);
		expertusErrorThrow($ex);
	}
}
/*
 *
 - Fetch a user's access privileges from slt_person.
 */
function getUserAccessPriviledges($userId) {
	try{
		expDebug::dPrint(' $userId = ' . print_r($userId, true),4);
		$userAccess = null;
		// Added by Vincent for performance imporvement on Sep 3, 2015
		$uid = getIdOfLoggedInUser();
		if($userId == $uid && isset($_SESSION['access_prev']) && !empty($_SESSION['access_prev']))
			return $_SESSION['access_prev'];
		if ($userId > 0) {
			// Select table
			$selectStmt = db_select('slt_person', 'per');
			// Set conditions
			$selectStmt->condition('per.id', $userId);

			// Select fields and/or add expressions
			$selectStmt->addField('per', 'user_type', 'user_type');
			$selectStmt->addField('per', 'is_manager', 'is_manager');
			$selectStmt->addField('per', 'is_instructor', 'is_instructor');
			$selectStmt->addField('per', 'org_id', 'org_id');
			$selectStmt->addExpression('(select group_concat(job_role) from slt_person_jobrole_mapping where user_id ='.$userId.')',  'job_role');
			$selectStmt->addField('per', 'employment_type');
			$selectStmt->addField('per', 'dept_code','department');
			$selectStmt->addField('per', 'country');
			$selectStmt->addField('per', 'state');
			$selectStmt->addField('per', 'preferred_language', 'language');
			$selectStmt->addField('per', 'hire_date', 'hire_date');
			expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);

			// Execute query
			$userAccess = $selectStmt->execute()->fetch();
			expDebug::dPrint(' $result = ' . print_r($userAccess, true),4);
		}
		if($userId == $uid) $_SESSION['access_prev'] = $userAccess;

		return $userAccess;
	}catch (Exception $ex) {
		watchdog_exception('getUserAccessPriviledges', $ex);
		expertusErrorThrow($ex);
	}
}



/*
 * filterByUserAccessPrivileges - Adds the condition to filter records by the user's access priviledges.
 * If userId is given, it will use access priviledges of the given user id to add the conditions, otherwise
 * it will retrieve access priviledges of the logged in user.
 */
function filterByUserAccessPrivileges(&$selectStmt, $alisename='', $userId = '',$fromAdmin='',$entity_type='',$page='',$createByAdmin='',$join = '',$accessType ='',$createByAdminCondition = '',$enrollmentalias='') {	try{
		$userId = (isset($_REQUEST['apiname'])&& empty($userId)) ? $_REQUEST['userid'] :  $userId;
		$userId = empty($userId)? getIdOfLoggedInUser() : $userId;
		// check if user is part of Super Admin Group(grp_sup)
		if ($fromAdmin != '') {
    		if(is_data_admin($userId)) {
    		    if($page != 'Courseedit' && $page != 'Search' && $page != 'people_setting') {
                    $selectStmt->addExpression('1', 'sumEdit');
                    $selectStmt->addExpression('1', 'sumDelete');
                }
    		    expDebug::dPrint("User $userId is a member of Data Administer group and filterByUserAccessPrivileges validation is skipped", 5);
    		    return true;
    		}
		}
		if(($userId != '1') || ($userId == '1' && arg(0) != 'administration' && $page != 'user_attribute') || (($userId == '1' && arg(1) == 'order'))){ // Commited for issue with super admin
			//if(($userId != '1') || ($userId == '1' && arg(2) == 'user_login') || ($userId == '1' && arg(1) == 'learningcore') || ($userId == '1' && arg(1) == 'order')  || ($userId == '1' && arg(1) == 'lp-change-class') || arg(0) == 'learning'){
			global $catalog_reg;
			$aliasCatalogAccess = 'catacs';
			$aliasMapAccess = ($page != 'discount') ? 'group_map' : '';
// 			$checkSuperAdmin = '';
			$accessck = '1=1';
			$userAccessCond = 'FIND_IN_SET('.$userId.','.$aliasCatalogAccess.'.added_users)>0';
			if(empty($join)){
				$check_owner = '';
				if ($fromAdmin != '') {
					adminGrpMap ( $selectStmt, $alisename, $entity_type, $page, $createByAdmin, $userId, $accessType );
// 					$selectStmt->where('if(catacs.code = \'grp_sup\' AND catacs.userslist is not null AND catacs.userslist != \'\', FIND_IN_SET(' . $userId . ' ,catacs.userslist) > 0, if(catacs.code = \'grp_sup\' AND catacs.added_users is not null AND catacs.added_users != \'\', FIND_IN_SET(' . $userId . ' ,catacs.added_users) > 0, 1=1))');
// 					$checkSuperAdmin = 'if(catacs.code = \'grp_sup\' AND catacs.userslist is not null AND catacs.userslist != \'\', FIND_IN_SET(' . $userId . ' ,catacs.userslist) > 0, if(catacs.code = \'grp_sup\' AND catacs.added_users is not null AND catacs.added_users != \'\', FIND_IN_SET(' . $userId . ' ,catacs.added_users) > 0, 1=1)) OR ';
// 					$check_owner = ' OR detail.owner_id = ' . $userId;
						$userAccessCond = 'FIND_IN_SET('.$userId.','.$aliasCatalogAccess.'.added_users)>0';
				} else {
					$uacond = '';//return condition from learnerGrpMap for user access enhancement
					learnerGrpMap ( $selectStmt, $alisename, $entity_type, $page,'','',$userId,$uacond);
					//$userAccessPages = array('Class', 'Program','Course','lp','Spotlight','Myteam','Forum');
					if($page != 'discount'){
						$userAccessCond = 'FIND_IN_SET('.$userId.','.$aliasCatalogAccess.'.added_users)>0 OR usrmap.user_id = '.$userId;
						$accessck = '((SELECT count(1)FROM slt_user_access_mapping usrmap WHERE '.$uacond.' )=0)';
					}else{
						$userAccessCond = 'FIND_IN_SET('.$userId.','.$aliasCatalogAccess.'.added_users)>0';
					}
				}
			}

			expDebug::dPrint(' $userId = ' . print_r($userId, true),4);

			//Changed for #0068033 by vetrivelp
			$userAccess= new stdClass();
			$grpType = ($fromAdmin != '') ? 1 : 0;
			$userAccess  = getUserAccessPriviledges($userId);
			expDebug::dPrint(' $$userAccess = ' . print_r($userAccess, true),4);
			$enabledValues = getEnabledAttrForVisibility();
			$closeBracket='';
			$conbracket='';
			// $defaultArr = array('ste_con_cnt'=>'country','ste_con_dep'=>'department','ste_con_ety'=>'employment_type','ste_con_mgr'=>'is_manager','ste_con_ins'=>'is_instructor','ste_con_jbr'=>'job_role','ste_con_lng'=>'language','ste_con_org'=>'org_id','ste_con_ste'=>'state','ste_con_usr'=>'user_type');
			$lastid = 1;
			// $enabledValues = array_intersect_key($defaultArr,$result);
			$enabledCount = count($enabledValues);
			if($enabledCount > 0){
				if($createByAdmin != ''){
					//Code reverted for #0044057
					//$selectStmt->where('if('.$createByAdmin.'.created_by ='.$userId.' OR '.$createByAdmin.'.updated_by = '.$userId.' OR FIND_IN_SET('.$userId.','.$aliasCatalogAccess.'.added_users)>0
		    	//			OR ((select ifnull(org_id,\'T\') from slt_person where id = '.$userId.') = (select ifnull(org_id,\'F\') from slt_person where id = '.$createByAdmin.'.created_by))
		    	//			,1=1,('.$aliasCatalogAccess.'.removed_users IS NULL OR FIND_IN_SET('.$userId.','.$aliasCatalogAccess.'.removed_users) <= 0');
					if($createByAdminCondition != ''){  //68252 : Admin cannot see inaccessible groups attached in access popup
						$selectStmt->where($createByAdminCondition. ' if('.$createByAdmin.'.created_by ='.$userId.' OR '.$createByAdmin.'.updated_by = '.$userId . $check_owner . ' OR FIND_IN_SET('.$userId.','.$aliasCatalogAccess.'.added_users)>0,1=1,('.$aliasCatalogAccess.'.removed_users IS NULL OR FIND_IN_SET('.$userId.','.$aliasCatalogAccess.'.removed_users) <= 0');
					}
					else {
					$selectStmt->where('if('.$createByAdmin.'.created_by ='.$userId.' OR '.$createByAdmin.'.updated_by = '.$userId . $check_owner . ' OR FIND_IN_SET('.$userId.','.$aliasCatalogAccess.'.added_users)>0,1=1,('.$aliasCatalogAccess.'.removed_users IS NULL OR FIND_IN_SET('.$userId.','.$aliasCatalogAccess.'.removed_users) <= 0');
					}
					$selectStmt->where('if('.$aliasCatalogAccess.'.code = \'grp_adm\',FIND_IN_SET('.$userId.','.$aliasCatalogAccess.'.added_users)>0,1=1)');
				}else{
/* if(empty($join))
						$selectStmt->where('if(FIND_IN_SET('.$userId.','.$aliasCatalogAccess.'.added_users)>0,1=1,('.$aliasCatalogAccess.'.removed_users IS NULL OR FIND_IN_SET('.$userId.','.$aliasCatalogAccess.'.removed_users) <= 0');
					else
						$selectStmt->where('if((FIND_IN_SET('.$userId.','.$aliasCatalogAccess.'.added_users)>0 '.$check_owner.'),1=1,('.$aliasCatalogAccess.'.removed_users IS NULL OR FIND_IN_SET('.$userId.','.$aliasCatalogAccess.'.removed_users) <= 0'); */
						if($enrollmentalias!='' && $enrollmentalias!= 'login') { // Added By Rajeshwar for #0071732
							$conbracket = '))';
							$con = 'if('.$enrollmentalias.'.course_id = '.$alisename.'.course_id,'.$enrollmentalias.'.class_id = '.$alisename.'.id,';
							$selectStmt->where($con.'(if('.$userAccessCond.',1=1,('.$aliasCatalogAccess.'.removed_users IS NULL OR FIND_IN_SET('.$userId.','.$aliasCatalogAccess.'.removed_users) <= 0');
						}
						else{
							$selectStmt->where('if('.$userAccessCond.',1=1,('.$aliasCatalogAccess.'.removed_users IS NULL OR FIND_IN_SET('.$userId.','.$aliasCatalogAccess.'.removed_users) <= 0');
						}
					if(!empty($join))
						$selectStmt->where('if('.$aliasCatalogAccess.'.code = \'grp_adm\',FIND_IN_SET('.$userId.','.$aliasCatalogAccess.'.added_users)>0,1=1)');
				}

				foreach($enabledValues as $key => $value){
					$closeBracket = ($aliasMapAccess == '' && ($lastid == $enabledCount)) ? '))' :'';
					expDebug::dPrint(' $$value = ' . print_r($value, true),4);
					//7306: Support of users to have more than 1 job role association
					if($value == 'job_role'){
						$resultjobrole= array();
						$resultjobrole = getUserAttributeValues($userId);
						if(count($resultjobrole)>0){
						$userAccess->job_role = implode(',',$resultjobrole);
						expDebug::dPrint('Job ROLE sql RESULT-->>'.print_r($resultjobrole,1),4);
					}
					}
					if($value != 'role' && $value != 'hire_date'){
						$queryCondition = db_or();
						$queryCondition->where('if('.$aliasCatalogAccess . '.'.$value.'=\'All\',\''.$userAccess->$value .'\'!=\'\',('.$aliasCatalogAccess . '.'.$value.' IS NULL)');
						//$queryCondition->isNull($aliasCatalogAccess . '.'.$value);
						if (empty($userAccess->$value)) {
							$queryCondition->where($aliasCatalogAccess . '.'.$value.'=\'\')');
						}else{
							$queryCondition->condition($aliasCatalogAccess . '.'.$value, '');
							if($value == 'state'){
								$queryCondition->where('FIND_IN_SET(:'.$value.', '. $aliasCatalogAccess . '.'.$value.'))',
								array(':'.$value => $userAccess->country.'-'.$userAccess->$value));
							}else{

								//7306: Support of users to have more than 1 job role association
								if($value == 'job_role'){
									$cnt = count($resultjobrole);
									if($cnt>0){
										$i =1;
										$braket = ')';
										foreach ($resultjobrole as $job_role){
											expDebug::dPrint('$JOB_ROLE------>'.print_r($job_role,1),4);
											if($i==$cnt)
												$braket = '))';
											$job_role_var = $value.$i;
											$queryCondition->where('FIND_IN_SET(:'.$job_role_var.', '. $aliasCatalogAccess . '.'.$value.$braket,array(':'.$job_role_var => $job_role));
											$i++;
										}
									}
								}else{
								$queryCondition->where('FIND_IN_SET(:'.$value.', '. $aliasCatalogAccess . '.'.$value.'))',
								array(':'.$value => $userAccess->$value));
							}
						}
						}
						$selectStmt->condition($queryCondition);
					}else if($value == 'role'){

						$instructor = empty($userAccess->is_instructor)? 'N' : $userAccess->is_instructor;
						$manager = empty($userAccess->is_manager)?'N':$userAccess->is_manager;
						/* $insqueryCondition = db_or();
						$insqueryCondition->where('if('.$aliasCatalogAccess.'.is_instructor !=\'N\',\''.$instructor.'\'=\'Y\',1=1)');

						$selectStmt->condition($insqueryCondition);
						$mgrqueryCondition = db_or();
						$mgrqueryCondition->where('if('.$aliasCatalogAccess.'.is_manager !=\'N\',\''.$manager.'\'=\'Y\',1=1)'); */
						//$mgrqueryCondition->where('if('.$aliasCatalogAccess.'.is_manager is not null and '.$aliasCatalogAccess.'.is_manager!=0,'.$aliasCatalogAccess.'.is_manager='.$manager.',1=1)'.$closeBracket);
						$selectStmt->where('CASE WHEN ('.$aliasCatalogAccess.'.is_manager=\'Y\' AND '.$aliasCatalogAccess.'.is_instructor=\'Y\')
                                                    THEN
                                                    (:'.$manager.' = '.$aliasCatalogAccess.'.is_manager or :'.$instructor.' = '.$aliasCatalogAccess.'.is_instructor)
                                                    WHEN ('.$aliasCatalogAccess.'.is_manager=\'Y\' AND '.$aliasCatalogAccess.'.is_instructor=\'N\')
                                                    THEN
                                                    (:'.$manager.' = '.$aliasCatalogAccess.'.is_manager)
                                                    WHEN ('.$aliasCatalogAccess.'.is_manager=\'N\' AND '.$aliasCatalogAccess.'.is_instructor=\'Y\')
                                                    THEN
                                                    (:'.$instructor.' = '.$aliasCatalogAccess.'.is_instructor)
                                                    ELSE
                                                    1=1
                                                    END',array(':'.$instructor => $instructor,':'.$manager => $manager));

						//$selectStmt->condition($mgrqueryCondition);


					}else if($value == 'hire_date'){

						 $perHireDate = $userAccess->hire_date;

												$selectStmt->where('if(grpatt.id is not null,if(grpatt.on_or_after_start_date is not null ,DATE_FORMAT(grpatt.on_or_after_start_date,\'%Y-%m-%d\') <= DATE_FORMAT(:hire_date3,\'%Y-%m-%d\'),1=0) OR 
            		if(grpatt.on_or_before_start_date is not null, DATE_FORMAT(grpatt.on_or_before_start_date,\'%Y-%m-%d\') >= DATE_FORMAT(:hire_date2,\'%Y-%m-%d\'), 1=0) OR
					if(grpatt.between_start_date is not null ,DATE_FORMAT(:hire_date1,\'%Y-%m-%d\') between DATE_FORMAT(grpatt.between_start_date,\'%Y-%m-%d\') AND DATE_FORMAT(grpatt.between_end_date,\'%Y-%m-%d\'), 1=0),1=1)',
														array(':hire_date1' => $perHireDate, ':hire_date2' => $perHireDate, ':hire_date3' => $perHireDate)); 
					}

					$lastid++;
				}
				if($aliasMapAccess != ''){
					$closeBracket='))';
					$queryCondition1 = db_or();
					$queryCondition1->condition($aliasMapAccess.'.group_type',$grpType,'=');
					if($grpType == 0)
						$queryCondition1->where($aliasMapAccess.'.group_id IS NULL AND '.$accessck.' ');
					else
						$queryCondition1->where($aliasMapAccess.'.group_id IS NULL');
					$selectStmt->condition($queryCondition1);
				}
				$selectStmt->where('if(('.$aliasCatalogAccess.'.is_admin = 1),FIND_IN_SET('.$userId.','.$aliasCatalogAccess.'.userslist)>0,1=1)'.$closeBracket.$conbracket);				//$selectStmt->where('if(('.$aliasCatalogAccess.'.is_admin = 1 AND '.$aliasCatalogAccess.'.userslist is not null AND '.$aliasCatalogAccess.'.userslist != \'\'),FIND_IN_SET('.$userId.','.$aliasCatalogAccess.'.userslist)>0,1=1)'.$closeBracket);
				if($enrollmentalias == 'login'){
					$selectStmt->where('if((templ.id IS NOT NULL AND :hire_date1 != \'\'  AND templ.compliance_completed = \'hire_days\'),(DATE_ADD(:hire_date2, INTERVAL templ.complete_days DAY) >= NOW()),1=1)',array(':hire_date1' => $userAccess->hire_date,':hire_date2' => $userAccess->hire_date));
					$selectStmt->where('if(templ.id IS NOT NULL  AND templ.is_compliance=1 AND templ.validity_date IS NOT NULL,templ.validity_date >= curdate(),1=1 )');
					$selectStmt->where('if(templ.id IS NOT NULL  AND templ.complete_date IS NOT NULL,templ.complete_date >= curdate(),1=1 )');
						
				}
				
			}
		}

	}catch (Exception $ex) {
		watchdog_exception('getUserAccessPriviledges', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Function added by Vincent for #0071187
 * Simular to filterByUserAccessPrivileges function instead of filtering the 
 * users using group table the data will be fetched from pre-populated flat table
 * @param $selectStmt
 * @param $alisename
 * @param $userId
 * @param $fromAdmin
 * @param $entity_type
 * @param $page
 * @param $createByAdmin
 * @param $join
 * @param $accessType
 * @param $createByAdminCondition
 * @return unknown_type
 */
function filterByUserAccessPrivilegesUsingFlatTable(&$selectStmt, $alisename='', $userId = '',$fromAdmin='',$entity_type='',$page='',$createByAdmin='',$join = '',$accessType ='',$createByAdminCondition = '') {
	try{
		$userId = (isset($_REQUEST['apiname'])&& empty($userId)) ? $_REQUEST['userid'] :  $userId;
		$userId = empty($userId)? getIdOfLoggedInUser() : $userId;
		
		// check if user is part of Super Admin Group(grp_sup)
		if ($fromAdmin != '') {
    		if(is_data_admin($userId)) {
    		    if($page != 'Courseedit' && $page != 'Search' && $page != 'people_setting') {
                    $selectStmt->addExpression('1', 'sumEdit');
                    $selectStmt->addExpression('1', 'sumDelete');
                }
    		    expDebug::dPrint("User $userId is a member of Data Administer group and filterByUserAccessPrivileges validation is skipped", 5);
    		    return true;
    		}
		}
		
		$alisename = ($alisename == '') ?  'per': $alisename ;
		
		if(($userId != '1') || ($userId == '1' && arg(0) != 'administration' && $page != 'user_attribute') || (($userId == '1' && arg(1) == 'order'))){
			$selectStmt->leftJoin('slt_group_mapping','group_map','group_map.entity_id='.$alisename.'.id AND group_map.entity_type = \''.$entity_type.'\' and group_map.group_type = 1');
			$selectStmt->leftJoin('slt_group_user_mapping','usrgrp', 'usrgrp.group_id=group_map.group_id AND usrgrp.group_status = \'cre_sec_sts_atv\' and usrgrp.user_type != \'R\'');
			$selectStmt->where('IF('.$alisename.'.created_by = '.$userId.' OR '.$alisename.'.updated_by = '.$userId.', 1=1, IF((group_map.group_id is not null and group_map.group_type=1),usrgrp.user_id= '.$userId.',1=1))');
		}
		
	}catch (Exception $ex) {
		watchdog_exception('getUserAccessPriviledges', $ex);
		expertusErrorThrow($ex);
	}
}
/*
 * filterByUserAccessPrivileges - Adds the condition to filter records by the user's access priviledges.
 * If userId is given, it will use access priviledges of the given user id to add the conditions, otherwise
 * it will retrieve access priviledges of the logged in user.
 */
function filterByUserAccessPrivilegesCron(&$selectStmt, $alisename='', $userId = '',$fromAdmin='',$entity_type='',$page='',$createByAdmin='',$join = '',$cron='',$update='') {
	try{
		$userId = (isset($_REQUEST['apiname'])) ? $_REQUEST['userid'] :  $userId;
		$userId = empty($userId)? getIdOfLoggedInUser() : $userId;
		if(($userId != '1') || ($userId == '1' && arg(0) != 'administration') || (($userId == '1' && arg(1) == 'order'))){ // Commited for issue with super admin
			//if(($userId != '1') || ($userId == '1' && arg(2) == 'user_login') || ($userId == '1' && arg(1) == 'learningcore') || ($userId == '1' && arg(1) == 'order')  || ($userId == '1' && arg(1) == 'lp-change-class') || arg(0) == 'learning'){
			global $catalog_reg;
			$aliasCatalogAccess = 'catacs';
			$aliasMapAccess = ($page != 'discount') ? 'group_map' : '';

			if(empty($join)){
	  	if($fromAdmin != ''){
	  		adminGrpMap($selectStmt,$alisename,$entity_type,$page,$createByAdmin,$userId);
	  	}else{
	  		learnerGrpMap($selectStmt,$alisename,$entity_type,$page,$cron,$update,$userId);
	  	}
			}




		}

	}catch (Exception $ex) {
		watchdog_exception('getUserAccessPriviledges', $ex);
		expertusErrorThrow($ex);
	}
}
/*
 * descController() - To Return Short and Long Description according to the number of characters required.
 * This Function has Two Parameters
 * No.1 : $keyword - a unique keyword to be checked inside to change the function level variable
 * No.2 : $cString - the source string which would be worked upon.
 *
 * RETURN VALUES
 *     1. The Short Description String along with its tag element.
 *     2. The ellipsis along with its tag element
 *     3. The Long Description String along with its tag elements.
 *     4. The Anchor link showing up the arrow, which carries its show/hide functionality.
 */
function descController($keyword, $cString, $moreType='') {
	try{
		global $theme_key, $front_sidebar, $hightlight_blocks;
		$cString = /* strip_tags( */$cString/* ) */;
		$string_len = strlen($cString);
		$keyword = strtoupper($keyword);
		$maxLength = 0;
		switch($keyword) {
			case 'MOST VIEWED':
				$maxLength = 110;
				break;
			case 'MOST POPULAR TRAINING':
				if($theme_key == 'expertusoneV2'){
					if ($hightlight_blocks == 2 && !$front_sidebar) {
						$maxLength = 140;
					} else if($hightlight_blocks == 2 && $front_sidebar) {
						$maxLength = 80;
					} else {
						$maxLength = 45;
					}
				}else{
					$maxLength = 110;
				}
				break;
			case 'HIGHLY RATED TRAINING':
				if($theme_key == 'expertusoneV2'){
					if ($hightlight_blocks == 2 && !$front_sidebar) {
						$maxLength = 140;
					} else if($hightlight_blocks == 2 && $front_sidebar) {
						$maxLength = 80;
					} else {
						$maxLength = 45;
					}
				}else{
					$maxLength = 110;
				}
				break;
			case 'FEATURED TRAINING':
				$maxLength = 110;
				break;
			case 'MY LEARNING ANNOUNCEMENTS':
				$maxLength = 98;
				break;
			case 'NEW COURSES':
				$maxLength = 98;
				break;
			case 'NEW TRAINING':
				if($theme_key == 'expertusoneV2'){
					if ($hightlight_blocks == 2 && !$front_sidebar) {
						$maxLength = 140;
					} else if($hightlight_blocks == 2 && $front_sidebar) {
						$maxLength = 80;
					} else {
						$maxLength = 45;
					}
				}else{
					$maxLength = 98;
				}
				break;
			case 'CATALOG SEARCH':
				if($theme_key == 'expertusoneV2'){
					$maxLength = 150;
				}else{
					$maxLength = 245;
				}
				break;
			case 'COURSE DETAILS':
				$maxLength = 200;
				break;
			case 'NARROW_SEARCH':
				if($theme_key == 'expertusoneV2'){
					$maxLength = 140;
				}else{
					$maxLength = 150;
				}
				break;
			case 'ADMIN_CATALOG_DESC':
				$maxLength = 650;
				break;
			case 'ABOUT ME':
				$maxLength = 450;
				break;
			case 'GROUP VISIBILITY':
				$maxLength = 90;
				break;
			case 'FORUM ADD TOPIC':
				$maxLength = 250;
				break;
			CASE 'EXEMPTED_REASON':
				$maxLength = 100;
				break;
			case 'ANNOUNCEMENT_LIST':
				$maxLength = 100;
				break;
			case 'ADMIN_CATALOG_ADDINFO':
				$maxLength = 150;
				break;
		}
		$moreClass = '';
		if($theme_key == "expertusoneV2"){
			$moreClass = 'more-icon-'.array_pop(explode('_',$moreType));
		}

		$isutfstr = mb_detect_encoding($cString,"ASCII", true);
		if (is_string($isutfstr)){
	  if($string_len > $maxLength) {
	  	$res =  '<span class="item-short-desc">'.substr(strip_tags($cString), 0, $maxLength-1).'</span>';	//add strip tag for short desc ticket 0034069
	  	$res = $res.'<span class="item-elipsis">...</span>';
	  	$res = $res.'<span class="item-long-desc">'.$cString.'</span>';
	  	$res = $res.'<span class="more-text">';
	  	$res = $res.'<a href="javascript:void(0)" onclick="descriptionShowHide(this);" class="show-short-text '.$moreClass.'">&nbsp;</a></span>';
	  } else {
	  	$res = '<span class="item-short-desc">'.strip_tags($cString).'</span>';
	  }
		}else{
			$string_len = mb_strlen($cString,'UTF-8');
			if($string_len > $maxLength) {
				// Reset max length for Japanese/Korean/Chinese
				$charWidht = mb_strwidth(mb_substr($cString, 0, 1,'UTF-8'),'UTF-8');
				if($charWidht > 1){
				 $maxLength = round(($maxLength /4));
				 // $strlength = $strlength*2;
				}
				$res =  '<span class="item-short-desc">'.mb_substr(strip_tags($cString), 0, $maxLength,'UTF-8').'</span>';//add strip tag for short desc ticket 0034069
				$res = $res.'<span class="item-elipsis">...</span>';
				$res = $res.'<span class="item-long-desc">'.$cString.'</span>';
				$res = $res.'<span class="more-text">';
				$res = $res.'<a href="javascript:void(0)" onclick="descriptionShowHide(this);" class="show-short-text '.$moreClass.'">&nbsp;</a></span>';
			}else{
				$res = '<span class="item-short-desc">'.strip_tags($cString).'</span>';
			}
		}
		return $res;
	}catch (Exception $ex) {
		watchdog_exception('descController', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * titleController() - To return Title according to the number of characters required.
 * This Function has Two Parameters
 * No.1 : $keyword - a unique keyword to be checked inside to change the function level variable
 * No.2 : $cString - the source string which would be worked upon.
 * if the keyword is empty, it will go to default.
 * if maxlengthDefault parameter is not passed it will set to empty.
 *
 */
function titleController($keyword, $cString1,$maxLengthDefault= '') {
	try{
		$maxLengthDefaultOrg = $maxLengthDefault;
		if(trim($keyword) == '') {
			$keyword = 'keyword 404';
		}
		if(stripos($cString1,'fade-out-image') !== false){
			return $cString1;
	  		break;
		}
		$cString = strip_tags($cString1);
		$string_len = strlen($cString);
		$keyword = strtoupper($keyword);
		$maxLength = 0;
		$checkLength=true;
		$class_fade_out = strtolower($keyword).'-fadeout-container';
		switch($keyword) {
	  case 'ANNOUNCEMENT_SHORTTITLE':
	  	$maxLength = empty($maxLengthDefault)?250:$maxLengthDefault;
	  	$newline=explode("<li>", $cString1);
	  	if(sizeOf($newline)>2){
	  		$len = strlen(strip_tags($newline[0]))+strlen(strip_tags($newline[1]));
	  		if($len < $maxLength)
	  		$cString1=$newline[0]."<li>".$newline[1]."</ul>";
	  		else
	  		$cString1=$newline[0]."</ul>...";
	  		$checkLength=false;
	  	}else{

	  		$tmp = nl2br($cString1);
	  		$newline=explode("<p>", $tmp);
	  		if(sizeOf($newline)>2){
	  			$len = strlen($newline[0])+strlen($newline[1]);
	  			if($len < $maxLength){
	  				$len1 = strlen($newline[0])+strlen($newline[1])+strlen($newline[2]);
	  				if($len1 < $maxLength)
	  				$cString1='<p>'.$newline[0].'<p>'.$newline[1].'<p>'.$newline[2].'...';
	  				else
	  				$cString1='<p>'.$newline[0].'<p>'.$newline[1].'...';
	  				$string_len = $len;
	  				$checkLength=false;
	  			}else
	  			if($newline[0]==null){
	  				$cString1='<p>'.$newline[1].'...';
	  			}else{
	  				$cString1='<p>'.$newline[0].'...';
	  			}

	  		}
	  	}
	  	$brTag = array("<br>", "<br/>", "<br />","<p>", "</p>","<div>","</div>");
	  	$cString1=str_replace($brTag,'',$cString1);
	  	break;
	  	case 'ADMIN-NARROW-SEARCH':
	  	case 'NARROW-SEARCH-FILTERSET-RADIO-LABEL':
	  	case 'NARROW-SEARCH-FILTERSET-CHECKBOX':
	  	case 'ADMIN-NARROW-SEARCH-CODE':
	  	case 'MULTI-GRID-PERREQUISITE-TITLE':
	  	case 'MULTI-GRID-PERREQUISITE-CODE':
	  	case 'MULTI-GRID-EQUIVALENCE-TITLE':
	  	case 'MULTI-GRID-EQUIVALENCE-CODE':
	  	case 'MULTI-GRID-CONTENT-CODE':
	  	case 'MULTI-GRID-CONTENT-TYPE':
	  	case 'MULTI-GRID-ASSESSMENT-EDIT-TITLE':
	  	case 'MULTI-GRID-ASSESSMENT-EDIT-CODE':
	  	case 'MULTI-GRID-ASSESSMENT-VIEW-TITLE':
	  	case 'MULTI-GRID-ASSESSMENT-VIEW-CODE':
	  	case 'MULTI-GRID-SURVEY-VIEW-TITLE':
	  	case 'MULTI-GRID-SURVEY-VIEW-CODE':
	  	case 'MULTI-GRID-SURVEY-EDIT-TITLE':
	  	case 'MULTI-GRID-SURVEY-EDIT-CODE':
	  	case 'MULTI-GRID-SURASS-ATTACH-QUESTION-TITLE':
	  	case 'MULTI-GRID-SURASS-ATTACH-QUESTION-CODE':
	  	case 'NARROW-SEARCH-RESULTS-ITEM-CLASS-TITLE':
	  	case 'NARROW-SEARCH-RESULTS-ITEM-DETAIL-CLASS-CODE':
	  	case 'NARROW-SEARCH-RESULTS-ITEM-DETAIL-CLASS-LANGUAGE':
	  	case 'ADDEDIT-COURSE-FIELD-CODE':
	  	case 'ADDEDIT-COURSE-FIELD-TITLE':
        case 'ADDEDIT-COURSE-CUSTOMATTRIBUTE-LEFT-COL': //For Custom Attribute #custom_attribute_0078975
        case 'ADDEDIT-COURSE-CUSTOMATTRIBUTE-RIGHT-COL'://For Custom Attribute #custom_attribute_0078975
	  	case 'ADDEDIT-CLASS-BUSINESS-RULES-NAME':
	  	case 'ADDEDIT-CATAOG-ACCESS-GROUP-NAME':
	  	case 'ADDEDIT-CATAOG-ACCESS-GROUP-NAME-MRO':
	  	case 'ADDEDIT-CATAOG-ATTACHMENT-NAME':
 	  	case 'ADDEDIT-CATAOG-CERTIFICATE-NAME':
	  	case 'ADDEIDT-CATALOG-CLASS-CANCEL-SAVE-CLOSE':
	  	case 'ADDEDIT-ADMINISTRATION-COURSECLASS-FORM':
	  	case 'MULTI-GRID-TPATTACHCOURSE-TITLE':
	  	case 'MULTI-GRID-TPATTACHCOURSE-CODE':
	  	case 'MULTI-GRID-COUNTRYSETTING-TITLE':
	  	case 'MULTI-GRID-COUNTRYSETTING-CODE':
	  	case 'MULTI-GRID-ADDKEYWORDS-TITLE':
	  	case 'MULTI-GRID-SURASS-ATTACH-QUESTION-TITLE':
	  	case 'MULTI-GRID-SURASS-ATTACH-QUESTION-CODE':
	  	case 'NARROW-CLASSVIEW-DETAILDESC-TITLE':
	  	case 'NARROW-CLASSVIEW-DETAILDESC-CODE':
	  	case 'NARROW-CLASSVIEW-DETAILDESC-TAGSTRING':
	  	case 'NARROW-CLASSVIEW-DETAILDESC-BUSINESSSTR':
	  	case 'NARROW-CLASSVIEW-DETAILDESC-AUTH-VEND':
	  	case 'NARROW-CLASSVIEW-DETAILDESC-ACCESS':
	  	case 'NARROW-CLASSVIEW-DETAILDESC-SURVEY-TITLE':
	  	case 'NARROW-CLASSVIEW-DETAILDESC-ASSESSINFO-TITLE':
	  	case 'NARROW-CLASSVIEW-DETAILDESC-CONTINFO-TITLE':
	  	case 'NARROW-CLASSVIEW-DETAILDESC-CUSTOMINFO-LABEL':
	  	case 'ADMIN-NARROW-SEARCH-DATEWITHZONE':
	  	case 'ADMIN-NARROW-SEARCH-DATEWITHZONE-OLD':
	  	case 'ADMIN-NARROW-SEARCH-COURSEVIEW-CATALOG-TITLE':
	  	case 'ADMIN-NARROW-SEARCH-COURSEVIEW-CATALOG-CODE':
	  	case 'ADMIN-NARROW-SEARCH-COURSEVIEW-CATALOG-AUTHOR-TITLE':
	  	case 'ADMIN-NARROW-SEARCH-COURSEVIEW-TAG-STRING':
	  	case 'ADMIN-NARROW-SEARCH-COURSEVIEW-ACCESS-NAME':
	  	case 'ADMIN-NARROW-SEARCH-COURSEVIEW-ATTACHMENTS-TITLE':
	  	case 'ADMIN-NARROW-SEARCH-COURSEVIEW-CERTIFICATE-NAME':
	  	case 'ADMIN-NARROW-SEARCH-COURSEVIEW-PREREQUISTE-MORETHANONE-NAME':
	  	case 'ADMIN-NARROW-SEARCH-COURSEVIEW-PREREQUISTE-MORETHANONE-CODE':
	  	case 'ADMIN-NARROW-SEARCH-COURSEVIEW-PREREQUISTE-NAME':
	  	case 'ADMIN-NARROW-SEARCH-COURSEVIEW-PREREQUISTE-CODE':
	  	case 'ADMIN-NARROW-SEARCH-COURSEVIEW-EQUIVALENCE-MORETHANONE-NAME':
	  	case 'ADMIN-NARROW-SEARCH-COURSEVIEW-EQUIVALENCE-MORETHANONE-CODE':
	  	case 'ADMIN-NARROW-SEARCH-COURSEVIEW-EQUIVALENCE-NAME':
	  	case 'ADMIN-NARROW-SEARCH-COURSEVIEW-EQUIVALENCE-CODE':
	  	case 'ADMIN-NARROW-SEARCH-COURSEVIEW-CUSTOMINFO-LABEL':
	  	case 'ADMIN-NARROW-USERVIEW-FNAME':
	  	case 'ADMIN-NARROW-USERVIEW-UNAME':
	  	case 'ADMIN-NARROW-USERVIEW-EMAIL':
	  	case 'ADMIN-NARROW-USERVIEW-LNAME':
	  	case 'ADMIN-NARROW-USERVIEW-TIMEZONE':
	  	case 'ADMIN-NARROW-USERVIEW-ORG':
	  	case 'ADMIN-NARROW-USERVIEW-OTHER-ORG':
	  	case 'ADMIN-NARROW-USERVIEW-OTHER-MANAGER':
	  	case 'ADMIN-NARROW-USERVIEW-ACCESSNAME':
	  	case 'ADMIN-NARROW-USERVIEW-GROUPS':
	  	case 'ADMIN-NARROW-USERVIEW-CUSTOM-LABEL':
	  	case 'ADMIN-NARROW-USERVIEW-CUSTOM-VALUE':
        case 'ADMIN-NARROW-USERVIEW-CUSTOMATTRIBUTE-LEFT-COL': //For Custom Attribute #custom_attribute_0078975
        case 'ADMIN-NARROW-USERVIEW-CUSTOMATTRIBUTE-RIGHT-COL': //For Custom Attribute #custom_attribute_0078975
	  	case 'ADMIN-NARROW-ORGVIEW-NAME':
	  	case 'ADMIN-NARROW-ORGVIEW-COSTCENTER':
	  	case 'ADMIN-NARROW-ORGVIEW-ACCESSNAME':
	  	case 'ADMIN-NARROW-ORGVIEW-LABEL':
	  	case 'ADMIN-NARROW-ORGVIEW-PARENT':
	  	case 'ADMIN-GROUPS-PNAME-PARENT':
	  	case 'ADMIN-GROUPS-PNAME-CHILD':
	  	case 'ADMIN-GROUPS-UNAME':
	  	case 'ADMIN-GROUPS-FULLNAME':
	  	case 'ADMIN-GROUPS-ROLE-NAME':
	  	case 'ADMIN-ORG-NAME':
	  	case 'ADMIN-ORG-FNAME':
	  	case 'ADMIN-ORG-UNAME':
	  	case 'ADMIN-ORG-JOBTITLE':
	  	case 'ADMIN-ORG-USERTYPE':
	  	case 'ADMIN-ORG-MANAGER-NAME':
	  	case 'ADMIN-ROLE-UNAME':
	  	case 'ADMIN-ROLE-FNAME':
	  	case 'ADMIN-ROLE-ROLENAME':
	  	case 'ADMIN-ROLE-PNAME':
	  	case 'ADMIN-SETUP-NAME':
		case 'ADMIN-NARROW-QUESTION-VIEW-TITLE':
	  	case 'ADMIN-NARROW-QUESTION-VIEW-ACCESSNAME':
	  	case 'ADMIN-NARROW-TPVIEW-TITLE':
	  	case 'ADMIN-NARROW-TPVIEW-CODE':
	  	case 'ADMIN-NARROW-TPVIEW-AUTHOR':
	  	case 'ADMIN-NARROW-TPVIEW-TAGS':
	  	case 'ADMIN-NARROW-TPVIEW-PREREQUISITE-TITLE':
	  	case 'ADMIN-NARROW-TPVIEW-PREREQUISITE-CODE':
	  	case 'ADMIN-NARROW-TPVIEW-SURVEYNAME':
	  	case 'ADMIN-NARROW-TPVIEW-ASSESSINFO':
	  	case 'ADMIN-NARROW-TPVIEW-DETAIL-COURSE-TITLE':
	  	case 'ADMIN-NARROW-TPVIEW-DETAIL-COURSE-CODE':
	  	case 'ADMIN-NARROW-TPVIEW-DETAIL-MODULE-TITLE':
	  	case 'ADMIN-NARROW-TPVIEW-CUSTOMINFO-LABEL':
        case 'ADMIN-NARROW-TPVIEW-CUSTOMATTRIBUTE-LEFT-COL': //For Custom Attribute #custom_attribute_0078975
        case 'ADMIN-NARROW-TPVIEW-CUSTOMATTRIBUTE-RIGHT-COL'://For Custom Attribute #custom_attribute_0078975
	  	case 'ADMIN-CATALOG-COURSE-ADDEDIT-CUSTOMDETAILS-LABEL':
	  	case 'ADMIN-CATALOG-COURSE-ADDEDIT-COURSEDETAILS-TITLE':
	  	case 'ADMIN-NARROW-SURVEYASSESS-LEFT-TITLE':
	  	case 'ADMIN-NARROW-SURVEYASSESS-LEFT-CODE':
	  	case 'ADMIN-NARROW-SURVEYASSESS-ACCESSNAME':
	  	case 'ADMIN-NARROW-SURVEYASSESS-CUSTOMINFO-LABEL':
	  	case 'ADMIN-NARROW-SURVEYASSESS-QUESTION-TEXT':
	  	case 'ADMIN-NARROW-CONTENT-VIEW-CODE':
	  	case 'ADMIN-NARROW-CONTENT-VIEW-TAGS':
	  	case 'ADMIN-NARROW-CONTENT-VIEW-ACCESSNAME':
	  	case 'ADMIN-NARROW-BANNER-VIEW-TITLE':
	  	case 'ADMIN-NARROW-BANNER-VIEW-ACCESSNAME':
	  	case 'ADMIN-NARROW-NOTIFY-VIEW-TITLE':
	  	case 'ADMIN-NARROW-NOTIFY-VIEW-NOTIFYCC':
	  	case 'ADMIN-NARROW-NOTIFY-VIEW-ACCESSNAME':
	  	case 'LNR-SEARCH-VIEW-DETAIL-CODE':
	  	case 'LNR-SEARCH-VIEW-DETAIL-CODE-TP':
	  	case 'LNR-SEARCH-VIEW-DETAIL-LANG':
	  	case 'LNR-SEARCH-VIEW-DETAIL-TAG':
	  	case 'LNR-SEARCH-VIEW-DETAIL-TITLE':
	  	case 'LNR-SEARCH-VIEW-DETAIL-SESSION-NAME':
		case 'LNRSEARCH-COUNTRY-NAME': //Added by ganeshbabuv for #0078236 
	  	case 'EXP-SP-SKILL-LIST-VIEW-SKILLNAME':
	  	case 'EXP-SP-SKILL-LIST-VIEW-TITLE':
	  	case 'EXP-SP-FORUM-VIEW-LIST-NAME':
	  	case 'EXP-SP-FORUM-VIEW-LIST-CODE':
	  	case 'BLOCK-EXP-SP-CLASSDETAILS-TITLE':
	  	case 'BLOCK-EXP-SP-CLASSDETAIL-TITLE':
	  	case 'BLOCK-EXP-SP-COURSEDETAIL-TITLE':
	  	case 'BLOCK-EXP-SP-COURSEDETAIL-RECORD-TITLE':
	  	case 'EXP-SP-LNRREPORTS-VIEW-DETAIL-TITLE':
	  	case 'BLOCK-SP-LEARNING-SPOTLIGHT-TITLE':
	  	case 'EXP-SP-MYTEAM-VIEW-NAME':
	  	case 'EXP-SP-MYTEAM-VIEW-JOBTITLE':
	  	case 'EXP-SP-MYTEAM-VIEW-LOCATION':
		case 'EXP-SP-MYTEAM-VIEW-USERNAME':
	  	case 'EXP-SP-MYTEAM-MYCLASS-VIEW-CLASSTITLE':
	  	case 'EXP-SP-MYTEAM-MYCLASS-VIEW-CLASSTITLES':
	  	case 'BLOCK-EXP-SP-LEARNING-PLAN-DETAIL-EMPTY-TITLE':
	  	case 'BLOCK-EXP-SP-LEARNING-PLAN-DETAIL-TITLE':
	  	case 'EXP-SP-MYTEAM-CATALOG-SEARCH-CLASS-TITLE':
	  	case 'EXP-SP-MYTEAM-CATALOG-SEARCH-CLASS-CODE':
	  	case 'EXP-SP-MYTEAM-CATALOG-SEARCH-CLASS-LANGUAGE':
	  	case 'EXP-SP-MYTEAM-CATALOG-SEARCH-CLASS-LOCATION':
	  	case 'EXP-SP-ANNOUNCEMENT-TITLE':
	  	case 'EXP-SP-ADMIN-COMMERCE-DISCOUNTS-TYPE':
	  	case 'EXP-SP-ADMIN-COMMERCE-DISCOUNTS-ORGNAME':
	  	case 'EXP-SP-ADMIN-COMMERCE-DISCOUNTS-ROLENAME':
	  	case 'EXP-SP-ADMIN-COMMERCE-DISCOUNTS-USERTYPENAME':
	  	case 'EXP-SP-ADMIN-PROGRAM-LANG':
	  	case 'EXP-SP-ADMIN-RESOURCE-FACILITY-TITLE':
		case 'EXP-SP-ADMIN-CUSTOM-FIELDS-NEWVALUE':
	  	case 'EXP-SP-ADMIN-NOTIFICATION-TITLE':
	  	case 'EXP-SP-ADMIN-NOTIFICATION-ENTITYTITLE':
	  	case 'EXP-SP-ADMIN-SESSION-DETAILS-ATTENDEEURL':
	  	case 'EXP-SP-ADMIN-SESSION-DETAILS-PRESENTERURL':
	  	case 'ADMIN-COMMERCE-SETTING-CANCELLATIONPOL-DISPLAY-TITLE':
	  	case 'ADMIN-COMMERCE-TAX-STATENAME':
	  	case 'ADMIN-RESOURCE-LOCATION-DELETE-ACCESS':
	  	case 'ADMIN-RESOURCE-LOCATION-LOCATION-DETAILS':
	  	case 'EXP-SP-ADMIN-USER-ORGNAME':
	  	case 'EXP-SP-ADMIN-USER-MANAGRNAME':
	  	case 'EXP-SP-ADMIN-USER-GRPNAME':
	  	case 'EXP-SP-ADMIN-SURVEY-ASSESSMENT-ATTACH-QSTN-GROUPNAME':
	  	case 'ADMIN-CONFIG-GROUP-CHECK-ATTRIBUTES-TITLE':
	  	case 'EXP-SP-ADMIN-SURVEY-ASSESSMENT-QUESTION-CODE':
	  	case 'EXP-SP-ADMIN-SURVEY-ASSESSMENT-QUESTION-TITLE':
	  	case 'EXP-SP-ADMIN-SURVEY-ASSESSMENT-QUESTION-FULLMODTITLE':
	  	case 'EXP-SP-ADMIN-SURVEY-ASSESSMENT-SURVEYTITLE':
	  	case 'EXP-SP-ADMIN-SURVEY-ASSESSMENT-SURVEYCODE':
	  	case 'EXP-SP-ADMIN-SURVEY-ASSESSMENT-LABEL-TEXT':
	  	case 'EXP-SP-ADMIN-SURVEY-ASSESSMENT-STITLE':
	  	case 'ADMIN-QUESTIONS-DETAILS-TEXT':
	  	case 'EXP-SP-LNRREPORTS-REPORT-HEADER-TEXT':
	  	case 'EXP-SP-LNRREPORTS-REPORT-STATUSCOMMENT':
	  	case 'EXP-SP-LNRREPORTS-REPORT-FIELDTITLE':
	  	case 'EXP-SP-LNRREPORTS-REPORT-NEW-FIELD-TITLE':
	  	case 'EXP-SP-MYTRANSCRIPT-TITLE':
	  	case 'ADMIN-CATALOG-ACCESS-ORGNAME':
	  	case 'ADMIN-CATALOG-ACCESS-ROLENAME':
	  	case 'ADMIN-CATALOG-ACCESS-USER-TYPE-NAME':
	  	case 'ADMIN-COMMERCE-DISCOUNT-NAME':
	  	case 'ADMIN-COMMERCE-TAX-COUNTRYNAME':
	  	case 'EXP-SP-ADMIN-PGM-PGMID':
	  	case 'EXP-SP-ADMIN-PGM-TITLE':
	  	case 'EXP-SP-ADMIN-PGM-CODE':
	  	case 'EXP-SP-ADMIN-PGM-FULLMODTITLE':
	  	case 'EXP-SP-ADMIN-PGM-DEFAULT-TITLE':
	  	case 'EXP-SP-ADMIN-PGM-DEFAULT-CODE':
	  	case 'EXP-SP-ADMIN-PGM-DEFAULT-AUTHOR-VENDOR':
	  	case 'EXP-SP-ADMIN-PGM-DEFAULT-CUSTOM-LABELTXT':
	  	case 'EXP-SP-ADMIN-PGM-TP-TITLE':
	  	case 'EXP-SP-ADMIN-RESOURCE-LOCATIONID':
	  	case 'EXP-SP-ADMIN-CONTENT-MODULE-TITLE':
	  	case 'EXP-SP-ADMIN-LOCATION-MODULE-LOCATIONID':
	  	case 'EXP-SP-ADMIN-NOTOFICATION-TEMPLATE-MODULE-TITLE':
	  	case 'EXP-SP-ADMIN-SURVEYDETAILS-MODULE-SURVEY-TITLE':
	  	case 'EXP-SP-FORMATTER-PASSWORD-NAME':
	  	case 'EXP-SP-MYPROFILE-FULL-NAME':
	  	case 'EXP-SP-MYPROFILE-MANAGER-NAME':
	  	case 'EXP-SP-NOTIFY-MODULE-FINALMESSAGE':
	  	case 'EXP-SP-OAUTH-MODULE-CLIENTNAME':
	  	case 'EXP-SP-OAUTH-MODULE-CLIENTID':
	  	case 'EXP-SP-OAUTH-MODULE-CLIENTSECRET':
	  	case 'EXP-SP-OAUTH-MODULE-REDIRECTURL':
	  	case 'UC-ORDER-CUSTOMER-TITLE':
	  	case 'UC-ORDER-CUSTOMER-CLASS-LOCATIONNAME':
	  	case 'EXP-SP-CART-MODULE-TITLE':
	  	case 'EXP-SP-CART-MODULE-LOCATION':
	  	case 'EXP-SP-CART-MODULE-TITLE':
	  	case 'EXP-SP-CART-MODULE-CLASSCODE':
	  	case 'EXP-SP-CART-MODULE-LOCATION':
	  	case 'EXP-SP-CART-MODULE-TITLE-DESC':
	  	case 'EXP-SP-CART-MODULE-CART-MSG':
	  	case 'EXP-SP-CART-MODULE-TITLE-MARKUP':
	  	case 'EXP-SP-ADMIN-CONTENT-VERSION-VERSIONTITLE':
	  	case 'NARROW-ANNOUNCE-VIEW-ACCESSNAME':
	  	case 'MY-ACTIVITY-LIST-ENTITY-TITLE':
	  	case 'MY-ACTIVITY-LIST-ENTITY-TITLE-EMPTY':
	  	case 'LNRLEARNING-PLAN-CHANGE-CLASS-VIEW-DETAIL-CLASS-TITLE':
	  	case 'LNRLEARNING-PLAN-CHANGE-CLASS-VIEW-DETAIL-CLASS-CODE':
	  	case 'LNRSEARCH-VIEW-FILTER-TITLE':
	  	case 'LNRSEARCH-VIEW-FILTER-TITLE-SRCH':
	  	case 'LNRSEARCH-VIEW-FILTER-NAME':
	  	case 'LNRSEARCH-VIEW-FILTER-LANG':
	  	case 'LNRSEARCH-VIEW-FILTER-JOBROLE-NAME':
	  	case 'LNRSEARCH-VIEW-FILTER-MRO-OTHERS':
	  	case 'ANNOUNCEMENT-LEARNER-TITLE':
	  	case 'BLOCK-EXP-SP-COURSEDETAIL-PREREQUISTE-NAMES':
	  	case 'BLOCK-EXP-SP-COURSEDETAIL-PREREQUISTE-CODES':
	  	case 'BLOCK-EXP-SP-COURSEDETAIL-PREREQUISTE-NAME':
	  	case 'BLOCK-EXP-SP-COURSEDETAIL-PREREQUISTE-CODE':
	  	case 'BLOCK-EXP-SP-COURSEDETAIL-EQUIVALENCE-NAMES':
	  	case 'BLOCK-EXP-SP-COURSEDETAIL-EQUIVALENCE-CODES':
	  	case 'BLOCK-EXP-SP-COURSEDETAIL-EQUIVALENCE-NAME':
	  	case 'BLOCK-EXP-SP-COURSEDETAIL-EQUIVALENCE-CODE':
	  	case 'ADMIN-LEARNING-ENROLLMENTS-RESULT-CODE':
	  	case 'ADMIN-LEARNING-ENROLLMENTS-RESULT-TITLE':
	  	case 'ADMIN-LEARNING-ENROLLMENTS-RESULT-FOR-VIEW-FULLNAME':
	  	case 'ADMIN-LEARNING-ENROLLMENTS-RESULT-FOR-VIEW-USERNAME':
	  	case 'ADMIN-LEARNING-ENROLLMENTS-RESULT-FOR-VIEW-STATUS':
	  	case 'ADMIN-LEARNING-ENROLLMENTS-RESULT-FOR-VIEW-SCORE':
	  	case 'ADMIN-LEARNING-MASTERENROLLMENTS-RESULT-FOR-VIEW-FULLNAME':
	  	case 'ADMIN-LEARNING-MASTERENROLLMENTS-RESULT-FOR-VIEW-USERNAME':
	  	case 'ADMIN-LEARNING-MASTERENROLLMENTS-RESULT-FOR-VIEW-STATUS':
	  	case 'ADMIN-LEARNING-MASTERENROLLMENTS-RESULT-FOR-VIEW-SCORE':
	  	case 'ADMIN-LEARNING-SURVEYTITLE':
	  	case 'ADMIN-LEARNING-SURVEYCODE':
	  	case 'ADMIN-LEARNING-ELSEPART-SURVEYTITLE':
	  	case 'ADMIN-LEARNING-ELSEPART-SURVEYCODE':
	  	case 'ADMIN-LEARNING-METTING-TITLE':
	  	case 'ADMIN-LEARNING-METTING-FULLNAME':
	  	case 'ADMIN-LEARNING-TRANINGTITLE':
	  	case 'ADMIN-LEARNING-TRANINGCODE':
	  	case 'ADMIN-NARROW-SEARCH-EDIT-TOOLTIP-CODE':
	  	case 'ADMIN-ADD-CUSTOM-LABEL':
	  	case 'ADMIN-NARROW-USERVIEW-CONTACT':
	  	case 'ADMIN-NARROW-USERVIEW-ADDR1':
	  	case 'ADMIN-NARROW-USERVIEW-ADDR2':
	  	case 'ADMIN-NARROW-USERVIEW-CITY':
	  	case 'ADMIN-NARROW-USERVIEW-STATE':
	  	case 'ADMIN-NARROW-USERVIEW-MOBILE':
	  	case 'ADMIN-NARROW-USERVIEW-COUNTRY':
	  	case 'ADMIN-NARROW-USERVIEW-ZIP':
	  	case 'ADMIN-NARROW-USERVIEW-LANGUAGE':
	  	case 'ADMIN-NARROW-USERVIEW-EMPLOYMENT':
	  	case 'ADMIN-NARROW-USERVIEW-JOBROLES':
	  	case 'ADMIN-NARROW-USERVIEW-MANAGER':
	  	case 'ADMIN-NARROW-USERVIEW-EMPLOYEE-NO':
	  	case 'ADMIN-NARROW-USERVIEW-JOBTITLE':
	  	case 'ADMIN-NARROW-USERVIEW-USERTYPE':
	  	case 'ADMIN-NARROW-USERVIEW-HIREDATE':
	  	case 'ADMIN-NARROW-USERVIEW-ROLES':
	  	case 'ADMIN-NARROW-ORGVIEW-TYPE':
	  	case 'ADMIN-NARROW-ORGVIEW-DESCRIPTION':
	  	case 'ADMIN-NARROW-ORGVIEW-CONTACT':
	  	case 'ADMIN-NARROW-SEARCH-COURSEVIEW-ATTACHED-RECORD':
	  	case 'ADMIN-INC-ADDTEXT-AREAFIELD-TO-ADDEDIT-FORM':
	  	case 'ADMIN-ADD-CUSTOM-FIELD-LABEL':
	  	case 'ADMIN-NARROW-ORGVIEW-CUSTOMINFO-VALUE':
	  	case 'CART-ORDER-BILLING-NAME':
	  	case 'CART-ORDER-USER-NAME':
	  	case 'LNR-SEARCH-VIEW-DETAIL-COURSE-TITLE':
	  	case 'LNR-SEARCH-VIEW-DETAIL-LOCATION-NAME':
	  	case 'COURSE-DETAIL-COURSE-CODE':
	  	case 'COURSE-VIEW-CODE':
	  	case 'SESSION-TITLE':
	  	case 'CLASS-DETAIL-CLASS-CODE':
	  	case 'CLASS-DETAIL-DATE-TIME':
	  	case 'CLASS-DETAIL-PREREQUISITIES-TITLE':
	  	case 'CLASS-DETAIL-PREREQUISITIE-TITLE':
	  	case 'CLASS-DETAIL-PREREQUISITIES-CODE':
	  	case 'CLASS-DETAIL-PREREQUISITIE-CODE':
	  	case 'CLASS-DETAIL-EQUIVALENCES-TITLE':
	  	case 'CLASS-DETAIL-EQUIVALENCE-TITLE':
	  	case 'CLASS-DETAIL-EQUIVALENCES-CODE':
	  	case 'CLASS-DETAIL-EQUIVALENCE-CODE':
	  	case 'LEARNINGPLAN-DETAIL-CODE':
	  	case 'LEARNINGPLAN-DETAIL-GROUP-NAME':
	  	case 'LEARNINGPLAN-DETAIL-COURSE-TITLE':
	  	case 'LEARNINGPLAN-DETAIL-COURSE-CODE':
	  	case 'LEARNINGPLAN-PREREQUISITIE-TITLE':
	  	case 'LEARNINGPLAN-COURSE-TITLE':
	  	case 'LEARNING-PLAN-COURSE-TITLE':
	  	case 'CLASS-DETAIL-SESSION-TITLE':
	  	case 'FORUM-DETAIL-FORUM-TOPIC-NAME':
	  	case 'LNRSEARCH-DATE-TIME':
	  	case 'LNR-SEARCH-VIEW-DETAIL-COMPLIANCE-COURSE-TITLE':
	  	case 'LNR-SEARCH-VIEW-DETAIL-MANDATORY-COURSE-TITLE':
	  	case 'LNR-SEARCH-VIEW-DETAIL-RECOMMENDED-COURSE-TITLE':
	  	case 'BLOCK-EXP-SP-CLASSDETAILS-COMPLIANCE-TITLE':
	  	case 'BLOCK-EXP-SP-COURSEDETAIL-COMPLIANCE-TITLE':
	  	case 'LEARNINGPLAN-MANDATORY-COURSE-TITLE':
	  	case 'LEARNINGPLAN-RECOMMENDED-COURSE-TITLE':
	  	case 'SKILL-NAME':
	  	case 'BLOCK-EXP-SP-CLASSDETAILS-RECOMMENDED-TITLE':
	  	case 'BLOCK-EXP-SP-CLASSDETAILS-MANDATORY-TITLE':
	  	case 'BLOCK-EXP-SP-COURSEDETAILS-RECOMMENDED-TITLE':
	  	case 'BLOCK-EXP-SP-COURSEDETAILS-MANDATORY-TITLE':
	  	case 'NARROW-SEARCH-LANG':
	  	case 'NARROW-SEARCH-TYPE':
	  	case 'NARROW-SEARCH-LESSON':
	  	case 'ANNOUNCEMENT-USER-TWO-FULLNAME':
	  	case 'ANNOUNCEMENT-USER-ONE-FULLNAME':
	  	case 'EXP-USER-LEADERBOARD-REC-USERNAME':
	  	case 'NARROW-SEARCH-ORGANIZATION-TYPE':
	  	case 'NARROW-SEARCH-COMMON-CODE':
	  	case 'ADMIN-COMMERCE-VAT-NUMBERS':
	  	case 'TEAM-COUNTRY-NAME':
	  	case 'NARROW-SEARCH-COUNTRY':
	  	case 'NARROW-SEARCH-RECIPIENT-NAME':
	  	case 'NARROW-SEARCH-EQUIPMENT':
	  	case 'ADDEDIT-COURSE-FIELD-AUTHOR-VENDOR':
	  	case 'ADMIN-NARROW-TPVIEW-ACCESS':
	  	case 'ADMIN-NARROW-SEARCH-COURSEVIEW-ATTACHMENTS-NAME':
	  	case 'ADMIN-NARROW-TPVIEW-ATTACHMENTS-NAME':
	  	case 'LEARNINGPLAN-PREREQUISITIE-CODE':
	  	case 'NARROW-CLASSVIEW-DETAILDESC-ATTACHMENTS-NAME':
	  	case 'ADMIN-NARROW-TPVIEW-PREREQUISITE-OBJECTTYPE':
	  	case 'EXP-SP-ADMIN-PGM-FULLMODTITLE':
	  	case 'SURVEY-GROUP-TITLE':
	  	case 'EXP-SP-ADMIN-SURVEY-ASSESSMENT-LABEL-VALUE':
	  	case 'ADMIN-NARROW-SURVEYASSESS-CUSTOMINFO-VALUE':
	  	case 'MYTEAM-COMPLIANCE-COURSE-TITLE':
	  	case 'MYTEAM-MANDATORY-COURSE-TITLE':
	  	case 'MYTEAM-RECOMMENDED-COURSE-TITLE':
	  	case 'INSTRUCTOR-FULLNAME':
	  	case 'ADDEDIT-COURSE-CUSTOMLABEL-VIEW':
	  	case 'ADMIN-MULTISELECT-DROPDOWN':
	  	case 'ADDEDIT-COURSE-CUSTOMLABEL-VIEW':
	    case 'EXP-SP-LNRREPORTS-VIEW-DETAIL-TITLE-POPUP':
	    case 'WIDGET-VIEW-DETAIL-PAGE':
	    case 'WIDGET-VIEW-DETAIL-PAGE-COMPLIANCE':
        case 'WIDGET-CATALOG-PAGE-IMAGE':
        case 'WIDGET-CATALOG-PAGE':
        case 'DELETED-PANELS':
        case 'EXP-SP-PREREQUISITE-COURSE-TITLE':
        case 'EXP-SP-PREREQUISITE-COURSE-CODE':
        case 'EXP-SP-PREREQUISITE-CLASS-TITLE':
        case 'EXP-SP-PREREQUISITE-CLASS-TITLE-NEW':
        case 'EXP-SP-PREREQUISITE-CLASS-CODE':
        case 'EXP-SP-PREREQUISITE-CLASS-LOCATION';
        case 'EXP-SP-PREREQUISITE-TP-TITLE';
        case 'EXP-SP-PREREQUISITE-TP-CODE';
        case 'CONTENT-GROUP-LABEL-VIEW-SCREEN';
        case 'CONTENT-GROUP-VALUE-VIEW-SCREEN';
        case 'ORDER-PAYMENT-STATUS';
        case 'FADEOUT-MY-TRANSCRIPT';
        case 'EXP-SP-LNRMYPROGRAM-PRG-CODE-RIGHT-FALSE';
        case 'EXP-SP-LNRMYPROGRAM-ENROLL-CLASS-TITLE';
        case 'EXP-SP-MYTEAM-COMPLETED-STATUS';
        case 'EXP-SP-MYPROFILE-JOB-TITLE':
        case 'EXP-SP-MYPROFILE-ORG-NAME':
        case 'EXP-SP-MYPROFILE-JOB-TITLE-FULL-LENGTH':
        case 'EXP-SP-MYPROFILE-ORG-NAME-FULL-LENGTH':
        case 'EXP-TEAM-PAGE-NARROW-SEARCH-LOCATION':
        case 'EXP-SP-FOOTER-TITLE':
        case 'MY-ACCOUNT-FIRST-NAME':
        case 'MY-ACCOUNT-LAST-NAME':
        case 'PRESENTER-LABEL':
        case 'EXEMPTED_REASON':
        case 'CATALOG-CLASS-DETAIL-TITLE':
        case 'CATALOG-CLASS-DETAIL-CODE':
        case 'MULTI-GRID-CONTENTMOVEUSERS-TITLE':
        case 'EXP-SP-MYTEAM-VIEW-USERNAME':
        case 'EXP-SP-MYTEAM-CATALOG-SEARCH-SESSION-DATE':
        case 'LNRSEARCH-CRT-EXPIRE':
        case 'LNRSEARCH-REQUEST-CLS':
        case 'MULTI-GRID-CONTENTMOVEUSERS-CLASSTITLE':
        case 'MULTI-GRID-CONTENTMOVEUSERS-ENROLLMENTSTATUS':
        case 'ADMIN-VIEW-SESSION-TITLE':
        case 'ASSIGNLEARN-TP-CLS-TITLE':
       	case 'ASSIGNLEARN-TP-CLS-SESSION-TITLE':
       	case 'ADMIN-NARROW-SEARCH-QUESTION-VIEW-TAG-STRING':
        case 'ADMIN-PGM-MODULE':
        case 'ADMIN-PGM-MODULE-GRP':
        case 'MULTI-GRID-SURVEY-VIEW-PATH':
        case 'MULTI-GRID-ASSESSMENT-VIEW-PATH':
        case 'MULTI-GRID-ENRTP-VIEW-PATH':
        case 'ADMIN-PGM-ADD-MODULE':
        case 'EXP-SP-LNRMYPROGRAM-ATTACHMENT-NAME':
        case 'EXP-SP-LNRMYPROGRAM-COURSECLASS-ATTACHMENT-NAME':
        case 'EXP-SP-LNRENROLLMENT-ATTACHMENT-NAME':
        case 'EXP-SP-MYLEARNING-MENULIST':
	    case 'VIDEO-FILE-TITLE':
  	    case 'POSTER-IMAGE-TITLE':
        case 'EXP-SP-LNRENROLLMENT-INSTRUCTOR-NAMES':
        case 'LEARNINGPLAN-DETAIL-CLASS-CODE':
        case 'CLASS-DETAIL-CLASS-CODE-MULTI':
        case 'CLASS-DETAIL-CLASS-CODE-MULTI':
        case 'CONTENT-PLAYER-TITLE':
        case 'CLASS-DETAIL-INS-DETAILS-NAME':
        case 'CLASS-DETAIL-INS-DETAILS-JOB':
        case 'LNRSEARCH-DATE-TIME-MULTISESSION':
        case 'CLASS-DETAIL-SESSION-INSTRUCTOR':
        case 'COMPLETED-BY':
        case 'MY-ACCOUNT-ADDRESS1':
        case 'MY-ACCOUNT-ADDRESS2':
        case 'MY-ACCOUNT-CITY':
        case 'MY-ACCOUNT-COUNTRY':
		case 'ADMIN-NARROW-CONTENT-AUTHOR-VIEW-CODE':
        case 'TEAM-ASSIGN-LEARNING':
        case 'TEAM-ASSIGN-LEARNING-MORE':
        case 'EXP-SP-PREREQUISITE-CLASS-SESSION-DATE':
        case 'EXP-SP-PREREQUISITE-LOCATION':
        case 'ADMIN-LEARNING-MASTERENROLLMENTS-RESULT-FOR-VIEW-PATH':
        case 'CHANGE-CLASS-SESSIONDATETIME':
        case 'CHANGE-CLASS-LANGUAGE':
        case 'CHANGE-CLASS-LOCATION':
        case 'ADMIN-NARROW-CUSTOMATTRIBUTE-LEFT-COL': //For Custom Attribute #custom_attribute_0078975
        case 'ADMIN-NARROW-CUSTOMATTRIBUTE-RIGHT-COL': //For Custom Attribute #custom_attribute_0078975
        case 'ADMIN-CAL-TITLE':
        case 'ADMIN-CAL-LEFT-TITLE':
        case 'NARROW-CONTENTAUTHOR-VIEW-DETAIL-ATTACHMENTS-NAME':
        case 'PROFILE-SKILL-ADDED-ON-DATE':
        case 'PROFILE-SKILL-EXPIREDATE':
        case 'MY-APPROVAL-EXPIREDATE':
        case 'MY-APPROVAL-ADDED-ON-DATE':
        case 'MY-APPROVAL-UNAME':
			$res = '<div class="fade-out-title-container-unprocessed fade-out-title-container  '.$class_fade_out.'"><span class="title-lengthy-text">'.$cString1.'</span><span class="fade-out-image"></span></div>';
	  		return $res;
	  		break;
	  default:
	  	$maxLength = !empty($maxLengthDefault) ? $maxLengthDefault : 30;
	  	break;
	  	//ADMIN_TP_ATTACH_QUESTION_CODE
		}
		$maxLength = !empty($maxLengthDefault) ? $maxLengthDefault : $maxLength;
		$isutfstr = mb_detect_encoding($cString1,"ASCII",true);
		if (is_string($isutfstr)){
			if($string_len > $maxLength) {
				if($checkLength==false){
					$res = $cString1;
				}else{
					$res = utf8_decode(substr(utf8_encode($cString1), 0, $maxLength-1));
				}
				$openTags=broken_tags($res);
				$apendTags='';
				for($i=count($openTags);$i>0;$i--){
					$tags = $openTags[$i-1];
					if(strpos($tags,'br')===false){
						if(strpos($tags,'<p')<0 || strpos($tags,'<span')<0){
							$apendTags = str_replace("<","</",$tags).">".$apendTags;
						}else{
							$res .= $apendTags;
							$srcTag = str_replace("<","</",$tags).">";
							$tmpStr =  substr($cString1,strlen($res));
							$res .= substr($cString1,strlen($res),strpos($tmpStr,$srcTag)+strlen($srcTag));
							$apendTags='';
						}
					}
				}
				$res .= $apendTags;
				if(((strlen($res)) != strlen($cString1)) && $keyword != 'FADEOUT'){
					$res = trim($res).'...';
				}else if (((strlen($res)) != strlen($cString1)) && $keyword == 'FADEOUT' && ((strlen($res)+4) < strlen($cString1))) {
					$res = $res.'<span class ="textfade">'.substr($cString1,strlen($res),2).'</span>'.'<span class ="textfade1">'.substr($cString1,(strlen($res)+2),1).'</span>';
				}else{
					$res = $cString1;
				}
			} else {
				$res = $cString1;
			}
			return $res;
		}else{
			$rtnString = '';
			$truncateFlag = 0;
			$strlength = mb_strlen($cString1,'UTF-8');
			$cString1 = strip_tags($cString1);
			$rtnString	= mb_strimwidth($cString1, 0, $maxLength, "...");
			if($rtnString != $cString1){
				$truncateFlag = 1;
			}
			if($truncateFlag==1){
				$rtnString = mb_substr($rtnString,0,mb_strlen($rtnString,'UTF-8')-3,'UTF-8');
				$openTags=broken_tags($rtnString);
				$apendTags='';
				for($i=count($openTags);$i>0;$i--){
					$tags = $openTags[$i-1];
					if(mb_strpos($tags,'br',0,'UTF-8')===false){
						if(mb_strpos($tags,'<p',0,'UTF-8')<0 || mb_strpos($tags,'<span',0,'UTF-8')<0){
							$apendTags = str_replace("<","</",$tags).">".$apendTags;
						}else{
							$rtnString .= $apendTags;
							$srcTag = str_replace("<","</",$tags).">";
							$tmpStr =  mb_substr($cString1,mb_strlen($rtnString,'UTF-8'),'URF-8');
							$rtnString .= mb_substr($cString1,mb_strlen($rtnString,'UTF-8'),mb_strpos($tmpStr,$srcTag,0,'UTF-8')+mb_strlen($srcTag,'UTF-8'),'UTF-8');
							$apendTags='';
						}
					}
				}
				$rtnString .= $apendTags;
				if(mb_strlen($rtnString)!=mb_strlen($cString1) && $keyword != 'FADEOUT'){
					$rtnString = trim($rtnString).'...';
				}else if (((mb_strlen($rtnString)) != mb_strlen($cString1)) && $keyword == 'FADEOUT' && ((mb_strlen($rtnString)+4) < mb_strlen($cString1))) {
					$rtnString = $rtnString.'<span class ="textfade">'.mb_substr($cString1,mb_strlen($rtnString),2).'</span>'.'<span class ="textfade1">'.mb_substr($cString1,(mb_strlen($rtnString)+2),1).'</span>';
				} else {
					$rtnString = $cString1;
				}
			}
			return $rtnString;
		}
	}catch (Exception $ex) {
		watchdog_exception('titleController', $ex);
		expertusErrorThrow($ex);
	}
}

function broken_tags($str)
{
	try{
		$str=strrpos($str,'<')>=0?stripos(substr($str,strrpos($str,'<')),'>')>0?$str:$str.'>':$str;
		preg_match_all("/(<\w+)(?:.){0,}?>/", $str, $v1);
		preg_match_all("/<\/\w+>/", $str, $v2);
		$open = array_map('strtolower', $v1[1]);
		$closed = array_map('strtolower', $v2[0]);
		$err=0;
		foreach ($open as $tag)
		{
			$end_tag = preg_replace("/<(.*)/", "</$1>", $tag);
			if (!in_array($end_tag, $closed)){
				$err=1;
			}else{
				unset($closed[array_search($end_tag, $closed)]);
				unset($open[array_search($tag, $open)]);
			}
		}
		return $open;
	}catch (Exception $ex) {
		watchdog_exception('broken_tags', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * subStringController() - To return String according to the number of characters required.
 * This Function has Two Parameters
 * No.1 : $cString - the source string which would be worked upon.
 * No.2 : $length - length of word to display
 *
 */
function subStringController($cString,$length) {
	try{
		if(strlen($cString) > $length){
			//$stringResult = substr($cString, 0,$length);
			$stringResult = mb_substr($cString, 0,$length);
		}else{
			$stringResult = $cString;
		}
		return $stringResult;
	}catch (Exception $ex) {
		watchdog_exception('subStringController', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Funtion to return the Multiple Registration Status.
 *
 * @param
 *	   Class ID
 * @return
 *     Status Code
 */
function is_multiregistration_allowed($entityId) {
	try{
		$select = db_select('slt_business_rule_mapping', 'rules');
		$select->leftJoin('slt_profile_list_items','prof','prof.code = rules.business_rule_code');
		$select->addField('rules', 'id');
		$select->condition('rules.entity_id', $entityId);
		$select->condition('rules.entity_type', 'cre_sys_obt_cls');
		$select->condition('rules.business_rule_code', 'cre_sys_brl_rra');
		$select->condition('prof.is_active', 'Y','=');
		$result = $select->execute()->fetchAll();
		if(count($result) > 0){

			$deliveryTypeAllowed[] = 'lrn_cls_dty_vod';
			$deliveryTypeAllowed[] = 'lrn_cls_dty_wbt';

			$select = db_select('slt_course_class', 'cls');
			$select->addField('cls','delivery_type');
			$select->condition('cls.id', $entityId);
			$select->condition('cls.delivery_type', $deliveryTypeAllowed, 'IN');
			$deliveryTypeResult = $select->execute()->fetchAll();
			return count($deliveryTypeResult) > 0 ? true : false;
		}
		return false;
	}catch (Exception $ex) {
		watchdog_exception('is_multiregistration_allowed', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Funtion to check whether the current user is manager or not.
 *
 * @param
 *	   userId
 * @return
 *     true/false
 */
function is_manager($userId) {
	try{
		// Added by Vincent for performance imporvement on Sep 3, 2015
		$uid = getIdOfLoggedInUser();
		expDebug::dPrint('User id '.$uid);
		if($userId == $uid && isset($_SESSION['is_manager']))
			return ($_SESSION['is_manager'] == 0) ? false : true;
		$select = db_select('slt_person', 'per');
		//$select->leftJoin('slt_person_other_manager', 'per_oth_mng', 'per_oth_mng.user_id=per.id and per_oth_mng.is_direct=\'N\'');
		$select->addExpression('count(per.id)','cnt');
		//$andcondition = db_and()->condition('per.is_manager','Y')->condition('per.id', $userId);
		//$select->condition(db_or()->condition('per.manager_id', $userId)->condition($andcondition)->condition('per_oth_mng.manager_id',$userId));
		$select->condition('per.is_manager','Y','=');
		$select->condition('per.id',$userId,'=');
		expDebug::dPrintDBAPI("IS MANAGER ",$select);
		$result = $select->execute()->fetchField();
		if($userId == $uid)
			$_SESSION['is_manager'] = ($result > 0) ? 1 : 0;
		return ($result > 0) ? true : false;
	}catch (Exception $ex) {
		watchdog_exception('is_manager', $ex);
		expertusErrorThrow($ex);
	}
}
/**
 * Funtion to check whether the current user is instructor.
 *
 * @param
 *	   userId
 * @return
 *     true/false
 */
function is_instructor($userId) {
	try{
		// Added by Vincent for performance imporvement on Sep 3, 2015
		$uid = getIdOfLoggedInUser();
		if($userId == $uid && isset($_SESSION['is_instructor']))
			return ($_SESSION['is_instructor']==0) ? false : true;
		$select = db_select('slt_person', 'per');
		$select->addExpression('per.id','id');
		$select->condition('per.is_instructor','Y');
		$select->condition('per.id',$userId);
		expDebug::dPrintDBAPI('$select', $select);
		$result = $select->execute()->fetchField();
		expDebug::dPrint('Instructor result: '.print_r($result, true),3);
		if($userId == $uid)
			$_SESSION['is_instructor'] = ($result > 0) ? 1 : 0;
		return ($result > 0) ? true : false;
	}catch (Exception $ex) {
		watchdog_exception('is_instructor', $ex);
		expertusErrorThrow($ex);
	}
}
/*
 * Derive timezone of learner from slt_person table
 */
function getUserPHPTimezoneName($userId = ''){
	try{
		if(empty($userId)){
			$userId = getSltpersonUserId();
		}
		$uid = getSltpersonUserId();
		// Added by Vincent for performance imporvement on Sep 3, 2015
		$uid = getIdOfLoggedInUser();
		if($userId == $uid && isset($_SESSION['user_timezone']) && !empty($_SESSION['user_timezone'])){
			return $_SESSION['user_timezone'];
		}

		$select = db_select('slt_person', 'per');
		$select->join('slt_profile_list_items','spf','per.time_zone = spf.code');
		$select->addField('spf', 'attr2','zone_name');
		$select->addField('spf', 'name','timezone_name');
		$select->condition('per.id',$userId,'=');
		expDebug::dPrintDBAPI("getUserPHPTimezoneName ",$select);
		$result = $select->execute()->fetchAll();

		if($userId == $uid) $_SESSION['user_timezone'] = $result;

		return $result;
	}catch (Exception $ex) {
		watchdog_exception('getUserPHPTimezoneName', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * Convert session date to learner timezone
 */
function timeZoneConvert($InputDateTime, $FromTimezone, $ToTimezone ){
	try{
		$tzFrom		= new DateTimeZone($FromTimezone);
		$tzTo	= new DateTimeZone($ToTimezone);
		$myDateJune = date_create($InputDateTime,$tzFrom);
		date_timezone_set($myDateJune,$tzTo);
		return  $myDateJune;

	}catch (Exception $ex) {
		watchdog_exception('timeZoneConvert', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getTaxonomyTermCount()
 * SELECT COUNT(*) INTO term_org_count FROM taxonomy_term_data WHERE vid=orgvocabularyno AND description=xid;
 */
function getTaxonomyTermCount($vid, $description) {
	try{
		// Select table
		$selectStmt = db_select('taxonomy_term_data', 'ttd');
		// Set conditions
		$selectStmt->condition('ttd.vid', $vid);
		$selectStmt->condition('ttd.description', $description);
		// Select fields
		$selectStmt->addExpression('COUNT(ttd.tid)', 'count');
		expDebug::dPrintDBAPI(' $selectStmt obj = ' , $selectStmt);
		// Execute query
		$numTerms = $selectStmt->execute()->fetchField();
		expDebug::dPrint(' $numTerms = ' . print_r($numTerms, true),3);
		return $numTerms;
	}catch (Exception $ex) {
		watchdog_exception('getTaxonomyTermCount', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * Validate an email address.
 * Provide email address (raw input)
 * Returns true if the email address has the email address format and the domain exists.
 * Code obtained from http://www.linuxjournal.com/article/9585?page=0,3
 */
function validateEmail($email) {
	try{
		$isValid = true;

		$util=new GlobalUtil();
		$config=$util->getConfig();

		$atIndex = strrpos($email, "@");
		if (is_bool($atIndex) && !$atIndex)
		{
			$isValid = false;
		}
		else
		{
			$domain = substr($email, $atIndex+1);
			$local = substr($email, 0, $atIndex);
			$localLen = strlen($local);
			$domainLen = strlen($domain);
			if ($localLen < 1 || $localLen > 64)
			{
				// local part length exceeded
				$isValid = false;
			}
			else if ($domainLen < 1 || $domainLen > 255)
			{
				// domain part length exceeded
				$isValid = false;
			}
			else if ($local[0] == '.' || $local[$localLen-1] == '.')
			{
				// local part starts or ends with '.'
				$isValid = false;
			}
			else if (preg_match('/\\.\\./', $local))
			{
				// local part has two consecutive dots
				$isValid = false;
			}
			else if (!preg_match('/^[A-Za-z0-9\\-\\.]+$/', $domain))
			{
				// character not valid in domain part
				$isValid = false;
			}
			else if (preg_match('/\\.\\./', $domain))
			{
				// domain part has two consecutive dots
				$isValid = false;
			}
			else if (!preg_match('/^(\\\\.|[A-Za-z0-9!#%&`_=\\/$\'*+?^{}|~.-])+$/', str_replace("\\\\","",$local)))
			{
				// character not valid in local part unless local part is quoted
				if (!preg_match('/^"(\\\\"|[^"])+"$/', str_replace("\\\\","",$local)))
				{
					$isValid = false;
				}
			}
			/* Mandatory email domain validation if check_email_domain_name = 1 in exp_sp.ini */

			$checkemail = $config['check_email_domain_name'];
			expDebug::dPrint('Email Validations'.print_r($checkemail, true), 4);
			if($checkemail == 1) {
				if ($isValid && !(checkdnsrr($domain,"MX") || checkdnsrr($domain,"A")))
				{
					// domain not found in DNS
					$isValid = false;
					expDebug::dPrint('Email Validation'.print_r($isValid, true), 4);
				}
			}
		}

		return $isValid;
	}catch (Exception $ex) {
		watchdog_exception('validateEmail', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * validatePassword() - Function to validate password
 */
function validatePassword($passwordFieldId, $password, $confirmPasswordFieldId, $confirmPassword, $personId = null) {
	try{
		if (!empty($password) || !empty($confirmPassword)) {
	  if ($password != $confirmPassword){
	  	form_set_error($passwordFieldId, t('ERR012'));
	  }
	  else {
	  	$uid = 0;
	  	if (!empty($personId)) {
	  		$uid = getDrupalIdOfUser($personId);
	  	}

	  	if (empty($uid)) {
	  		$account = (object)array('uid' => 0);
	  	}
	  	else {
	  		$account = user_load($uid);
	  	}
	  	$error = _password_policy_constraint_validate($password, $account);
	  	expDebug::dPrint(' $error = ' . print_r($error, true),4);
	  	if ($error) {
	  		//form_set_error($passwordFieldId,'<ul><li>'. implode('</li><li>', $error) .'</li></ul>'); //
	  		$errorCnt = 0;
	  		foreach($error as $errorstr){
	  			$passwordField = ($errorCnt) ? $errorCnt.'pass' : $passwordFieldId;
	  			form_set_error($passwordField, $errorstr);
	  			$errorCnt++;
	  		}
	  	}
	  }
		}
	}catch (Exception $ex) {
		watchdog_exception('validatePassword', $ex);
		expertusErrorThrow($ex);
	}
}


/**
 * getProfileItemNamesKeyed() - Function to fetch all profile list item names keyed by code whose code is prefixed as $codePrefix.
 *                              The result is ordered by name. This function is used to build selection controls (e.g. dropdown and checkboxes)
 *                              for the new admin screen but can be used in other places as well.
 */
function getProfileItemNamesKeyed($codePrefix, $langCode = 'cre_sys_lng_eng', $alsoFetchInactiveItems = false, $notInList = '', $name='' ,$limit='', $orderby='name') {
	try{
		// Select the table
		$select = db_select('slt_profile_list_items','spli');

		// Changed for Currency Multicurrency.
		//Note : The code values are not used for currency code. Also the currency items will display only if the value of attr3 is 'Y'
		if($codePrefix == 'cre_sys_crn_'){
			$select->addField('spli','attr1', 'code');
			$select->addExpression('concat(spli.attr1,\' \',spli.attr2,\' \',spli.name)', 'name');
			$select->condition('spli.attr3', 'Y', '=');
		}else{
		$select->addField('spli','code', 'code');
		$select->addField('spli','name', 'name');
		}
		if($codePrefix == 'sry_ans_typ_') {
			$orderby = 'spli.id';
		}
		// Select the fields to fetch
		$select->orderBy($orderby);
		// Set the conditions
		$select->condition('spli.code', db_like($codePrefix) . '%', 'LIKE');
		$select->condition(db_and()->condition('spli.lang_code', $langCode, '='));
		if (!$alsoFetchInactiveItems) {
			$select->condition(db_and()->condition('spli.is_active', 'Y', '='));
		}
		if(!empty($notInList)){
			$select->condition(db_and()->condition('spli.code', $notInList, '!='));
		}
		if(!empty($name)){
			$select->condition('spli.name', '%'. db_like($name) . '%', 'LIKE');
		}
		filterByUserAccessPrivileges($select, 'spli','',1,rtrim($codePrefix, "_"),'','spli');
		if(!empty($limit))
		$select->range(0,$limit);
		expDebug::dPrintDBAPI('$select for getProfileItemNamesKeyed', $select);
		$result = $select->execute()->fetchAllKeyed();
		expDebug::dPrint('$result = ' . print_r($result, true));
		//drop down translation for randomize
		foreach($result as $key => $value){
		 	if($codePrefix == 'lrn_cls_vct_' && $value == 'Other') {
				$result[$key] = t('LBL1149');
			}else {
				$result[$key] = t($value);
			}
		}
		// #43800 - Multilanguage fix for Questions
		if ($codePrefix == 'sry_det_typ_ass_ran_') {
			foreach($result as $key => $value){
				if ($value == 'Questions')
				$result[$key] = t('LBL240');
				break;
			}
		}
		// Return results
		return $result;
	}catch (Exception $ex) {
		watchdog_exception('getProfileItemNamesKeyed', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * getMatchingProfileItemNamesKeyed() - Function to fetch all profile list item names keyed by code whose code is prefixed as $codePrefix and that
 *                              match the given $pattern. The result is ordered by name.
 */
function getMatchingProfileItemNamesKeyed($pattern, $codePrefix, $langCode = 'cre_sys_lng_eng', $alsoFetchInactiveItems = false, $notInList = '', $forApi = false, $fields = array()) {
	try{
		// Select the table
		$select = db_select('slt_profile_list_items','spli');

		// Select the fields to fetch
		if(empty($fields)) {
			$select->addField('spli','code', 'code');
			$select->addField('spli','name', 'name');
		}
		else {
			foreach ($fields as $field=>$alias) {
				$select->addField('spli', $field, $alias);
			}
		}
		// Order results by name
		$select->orderBy('spli.name');

		// Set the conditions
		$select->condition('spli.code', db_like($codePrefix) . '%', 'LIKE');

		$select->condition(db_and()->condition('spli.lang_code', $langCode, '='));
		if (!$alsoFetchInactiveItems) {
			$select->condition(db_and()->condition('spli.is_active', 'Y', '='));
		}
		if(!empty($notInList)){
			$select->condition(db_and()->condition('spli.code', $notInList, '!='));
		}
		if(isset($_GET['z'])){
			$skip_param_validation = getConfigValue('skip_param_validation');
			if($skip_param_validation)
			$select->condition(db_and()->condition('spli.name', '%' . db_like($pattern) . '%', 'LIKE'));
			//else
				$select->condition(db_and()->condition('spli.name', '%' . db_like($pattern) . '%', 'LIKE'));
		}else{
			$select->condition(db_and()->condition('spli.name', '%' . db_like($pattern) . '%', 'LIKE'));
			$select->range(0,1);
		}
		filterByUserAccessPrivileges($select, 'spli','',1,rtrim($codePrefix, "_"),'','spli');
		expDebug::dPrintDBAPI('$select', $select);
		if($forApi) {
			$result = $select->execute()->fetchAll();
		}
		else {
			$result = $select->execute()->fetchAllKeyed();
		}
		expDebug::dPrint('$result = ' . print_r($result, true));
		// Return results
		return $result;
	}catch (Exception $ex) {
		watchdog_exception('getMatchingProfileItemNamesKeyed', $ex);
		expertusErrorThrow($ex);
	}
}

function getMatchingProfileItemNamesKeyedautocomplete($pattern, $codePrefix, $langCode = 'cre_sys_lng_eng', $alsoFetchInactiveItems = false, $notInList = '', $forApi = false, $fields = array()) {
	try{
		// Select the table
		$select = db_select('slt_profile_list_items','spli');

		// Select the fields to fetch
		if(empty($fields)) {
			$select->addField('spli','code', 'code');
			$select->addField('spli','name', 'name');
		}
		else {
			foreach ($fields as $field=>$alias) {
				$select->addField('spli', $field, $alias);
			}
		}
		// Order results by name
		$select->orderBy('name');

		// Set the conditions
		$select->condition('spli.code', db_like($codePrefix) . '%', 'LIKE');

		$select->condition(db_and()->condition('spli.lang_code', $langCode, '='));
		if (!$alsoFetchInactiveItems) {
			$select->condition(db_and()->condition('spli.is_active', 'Y', '='));
		}
		if(!empty($notInList)){
			$select->condition(db_and()->condition('spli.code', $notInList, '!='));
		}
		if(isset($_GET['z'])){
			$skip_param_validation = getConfigValue('skip_param_validation');
			if($skip_param_validation)
				$select->condition(db_and()->condition('spli.name', '%' . db_like($pattern) . '%', 'LIKE'));
			else
				$select->condition(db_and()->condition('spli.name', '%' . $pattern . '%', 'LIKE'));
		}else{
			$select->condition(db_and()->condition('spli.name', $pattern, 'LIKE'));
			$select->range(0,1);
		}
		filterByUserAccessPrivileges($select, 'spli','',1,rtrim($codePrefix, "_"),'','spli');
		expDebug::dPrintDBAPI('$select', $select);
// 		if($forApi) {
// 			$result = $select->execute()->fetchAll();
// 		}
// 		else {
// 			$result = $select->execute()->fetchAllKeyed();
// 		}
		expDebug::dPrint('$result = ' . print_r($result, true));
		// Return results
		return $select;
	}catch (Exception $ex) {
		watchdog_exception('getMatchingProfileItemNamesKeyed', $ex);
		expertusErrorThrow($ex);
	}
}
/*
 * getCountryName() - Fetches country name for country code from slt_country table
 */
function getCountryName($countryCode) {
	try{
		if (empty($countryCode)) {
			return '';
		}
		// Select table
		$selectStmt = db_select('slt_country', 'ct');
		// Set conditions
		$selectStmt->condition('ct.country_code', $countryCode);
		// Select state name field for returning
		$selectStmt->addField('ct', 'country_name', 'name');
		expDebug::dPrintDBAPI(' $selectStmt = ' ,$selectStmt);
		// Execute query
		$result = $selectStmt->execute()->fetchField();
		expDebug::dPrint(' $result = ' . print_r($result, true),3);
		return (empty($result)? '' : $result);
	}catch (Exception $ex) {
		watchdog_exception('getCountryName', $ex);
		expertusErrorThrow($ex);
	}
}
/*
 * getStateName() - Fetches state name for (state code, country code) from slt_state table
 */
function getStateName($stateCode, $countryCode) {
	try{
		if (empty($stateCode) || empty($countryCode)) {
			return '';
		}
		// Select table
		$selectStmt = db_select('slt_state', 'st');
		// Set conditions
		$selectStmt->condition('st.country_code', $countryCode);
		$selectStmt->condition('st.state_code', $stateCode);
		// Select state name field for returning
		$selectStmt->addField('st', 'state_name', 'name');
		expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt );
		// Execute query
		$result = $selectStmt->execute()->fetchField();
		expDebug::dPrint(' $result = ' . print_r($result, true),3);
		return (empty($result)? '' : $result);
	}catch (Exception $ex) {
		watchdog_exception('getStateName', $ex);
		expertusErrorThrow($ex);
	}
}
/*
 * getImmediateChildrenOrgIds() - Returns in an array ids of the first level children of an organization ($orgId)
 */
function getImmediateChildrenOrgIds($orgId) {
	try{
		expDebug::dPrint(' getImmediateChildrenOrgIds() called for $orgId = ' . print_r($orgId, true),4);

		$childOrgsIds = array();
		// Select the table
		$select = db_select('slt_organization', 'org');

		// Add fields to fetch
		$select->addField('org', 'id', 'id');

		// Add condition
		$select->condition('org.parent_org_id', $orgId);

		expDebug::dPrintDBAPI(' $select SQL = ' , $select);

		$childOrgsList = $select->execute()->fetchAll();
		expDebug::dPrint(' $childOrgsList = '. print_r($childOrgsList, true),3);

		foreach ($childOrgsList as $childOrg) {
			$childOrgsIds[] = $childOrg->id;
		}
		expDebug::dPrint(' $childOrgsIds = '. print_r($childOrgsIds, true),4);
		return $childOrgsIds;
	}catch (Exception $ex) {
		watchdog_exception('getImmediateChildrenOrgIds', $ex);
		expertusErrorThrow($ex);
	}
}
/*
 * getDescendantOrgIds() - Returns in an array ids of all the descendants of an organization ($orgId)
 */
function getDescendantOrgIds($orgId) {
	try{
		expDebug::dPrint(' getDescendantOrgIds() called for $orgId = '. print_r($orgId, true),4);
		$immediateChildrenOrgIds = getImmediateChildrenOrgIds($orgId); // Get the immediate children of this organization
		expDebug::dPrint(' getImmediateChildrenOrgIds() returned $immediateChildrenOrgIds = '. print_r($immediateChildrenOrgIds, true),4);
		$descendantOrgIds = $immediateChildrenOrgIds; // Immediate children are descendants, so initialize descendants list with this list
		// For each immediate child fetch the descendants, and add to the $descendants list.
		foreach ($immediateChildrenOrgIds as $immediateChildOrgId) {
			$descendantOrgIds = array_merge($descendantOrgIds, getDescendantOrgIds($immediateChildOrgId));
		}
		// Return the descendants list
		expDebug::dPrint(' getDescendantOrgIds() returned $descendantOrgIds = '. print_r($descendantOrgIds, true),4);
		return $descendantOrgIds;
	}catch (Exception $ex) {
		watchdog_exception('getDescendantOrgIds', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getOrganizationAutoComplete() - Returns list of organizations.
 *                           The below search options are available which are to be sent in the extraParams option of the autocomplete jQuery plugin.
 *
 *                           search_by_orgnumber - When set to 1, also search the search text in number field apart from name field
 *                           id - When set to a non-null string, return id of each organization in the pipe separate results as the first field
 *                           fetch_orgnumber - When set to 1, return org number as a pipe separated field in result after the name.
 *                           excluded_org_id - When set to 1, exclude this org from the results, and all its descendant orgs.
 *
 *                           autocomplete jquery plugin sends the search string in 'z' and max number of records to fetch in 'limit'
 */
function getOrganizationAutoComplete() {
	try{
		$searchByOrgNumber = (isset($_GET['search_by_orgnumber']) && $_GET['search_by_orgnumber'] == 1)? true : false;
		expDebug::dPrint(' $searchByOrgNumber = '. print_r($searchByOrgNumber, true),4);

		$idRequired = (empty($_GET['id']))? false : true;
		expDebug::dPrint(' $idRequired = '. print_r($idRequired, true),4);

		$fetchOrgNumber = (isset($_GET['fetch_orgnumber'])) && ($_GET['fetch_orgnumber'] == 1)? true : false;
		expDebug::dPrint(' $fetchOrgNumber = '. print_r($fetchOrgNumber, true),4);

		$excludedOrgId = (isset($_GET['excluded_org_id']))? $_GET['excluded_org_id'] : 0;
		expDebug::dPrint(' $excludedOrgId = '. print_r($excludedOrgId, true),4);

		$orgSubstr = $_GET['z'];
		expDebug::dPrint(' $orgSubstr = '. print_r($orgSubstr, true),4);

		$limit = (empty($_GET['limit']))? 0 : $_GET['limit'];
		expDebug::dPrint(' $limit = '. print_r($limit, true),4);

		// Select the table
		$select = db_select('slt_organization', 'org');

		// Add fields to fetch
		if ($idRequired || $fetchOrgNumber) {
			$select->addExpression('org.name', 'name');
			if ($idRequired) {
				$select->addExpression('org.id', 'org_id');
			}
			if ($fetchOrgNumber) {
				$select->addExpression('org.number', 'number');
			}
		}
		else {  // When neither id nor username has been fetched, return distinct org names.
			$select->addExpression('DISTINCT(org.name)', 'name');
		}

		// Add conditions
		$select->condition('org.status', 'cre_org_sts_act');

		if ($searchByOrgNumber) {
			$select->condition(db_or()
			->condition('org.name', '%' . db_like($orgSubstr) . '%', 'LIKE')
			->condition('org.number', '%' . db_like($orgSubstr) . '%', 'LIKE'));
		}
		else {
			$select->condition('org.name', '%' . db_like($orgSubstr) . '%', 'LIKE');
		}

		if ((!empty($excludedOrgId)) && $excludedOrgId > 0) {
			$excludeList = getDescendantOrgIds($excludedOrgId);
			$excludeList[] = $excludedOrgId; // Append array
			$select->condition('org.id', $excludeList, 'NOT IN');
		}
		filterByUserAccessPrivileges($select,'org','',1,'cre_org','attach','org'); // #47296 - group access visiblity check
		$select->orderBy('name');
		// limit the number of records to be fetched and returned
		if ($limit > 0) {
			$select->range(0, $limit);
		}


		expDebug::dPrintDBAPI(' $select SQL = ' , $select);
		$matchingOrgs = $select->execute()->fetchAll();
		if(arg(2) == 'group'){
			array_unshift($matchingOrgs,(object) array('name' => 'All'));
		}
		expDebug::dPrint(' $matchingOrgs = '. print_r($matchingOrgs, true),3);
		foreach ($matchingOrgs as $org) {
			expDebug::dPrint(' $org = ' . print_r($org, true),3);
			$choiceStr = '';
			if ($idRequired) {
				$choiceStr .= $org->org_id . EXP_AC_SEPARATOR;
			}
			$choiceStr .= $org->name;

			if ($fetchOrgNumber) {
				$choiceStr .= EXP_AC_SEPARATOR . $org->number;
			}
			expDebug::dPrint(' $choiceStr = ' . print_r($choiceStr, true),4);
			print $choiceStr . "\n";
		}
	}catch (Exception $ex) {
		watchdog_exception('getOrganizationAutoComplete', $ex);
		expertusErrorThrow($ex);
	}
}
/*
 * getPersonAutoComplete() - Returns the matched person names.
 *                           The below search options are available which are to be sent in the extraParams option of the autocomplete jQuery plugin.
 *
 *                           search_by_username - When set to 1, also search the search text in user_name field apart from full_name field
 *                           id - When set to a non-null string, return id of each user in the pipe separate results as the first field
 *                           fetch_username - When set to 1, return it as a pipe separated field in result after the fullname.
 *                                            When set to 2, append username in brackets to each user's full name.
 *                           excluded_user_id - When set to 1, exclude this user from the results
 *                           exclude_reportees_of_excluded_user - When set to 1, exclude all direct/indirect reportees of excluded_user_id from the results
 *                           list_only_managers - When set to 1, list only the users for whom is_manager is set to 1 in slt_person table.
 *                           list_only_instructors - When set to 1, list only the users for whom is_instructor is set to 1 in slt_person table.
 *
 *                           autocomplete jquery plugin sends the search string in 'z' and max number of records to fetch in 'limit'
 */
function getPersonAutoComplete() {
	try{
		$searchByUsername = (isset($_GET['search_by_username']) && $_GET['search_by_username'] == 1)? true : false;
		expDebug::dPrint(' $searchByUsername = '. print_r($searchByUsername, true),4);

		$idRequired = (!empty($_GET['id']))? true : false;
		expDebug::dPrint(' $idRequired = '. print_r($idRequired, true),4);

		$userNameAsField = (isset($_GET['fetch_username'])) && ($_GET['fetch_username'] == 1)? true : false;
		expDebug::dPrint(' $userNameAsField = '. print_r($userNameAsField, true),4);

		$userNameAppended = (isset($_GET['fetch_username'])) && ($_GET['fetch_username'] == 2)? true : false;
		expDebug::dPrint(' $userNameAppended = '. print_r($userNameAppended, true),4);

		$excludedUserId = (isset($_GET['excluded_user_id']))? $_GET['excluded_user_id'] : 0;
		expDebug::dPrint(' $excludedUserId = '. print_r($excludedUserId, true),4);

		$excludeReportees = (isset($_GET['exclude_reportees_of_excluded_user']) && $_GET['exclude_reportees_of_excluded_user'] == 1)? true : false;
		expDebug::dPrint(' $excludeReportees = '. print_r($excludeReportees, true),4);

		$listOnlyManagers = (isset($_GET['list_only_managers']) && $_GET['list_only_managers'] == 1)? true : false;
		expDebug::dPrint(' $listOnlyManagers = '. print_r($listOnlyManagers, true),4);

		$listOnlyInstructors = (isset($_GET['list_only_instructors']) && $_GET['list_only_instructors'] == 1)? true : false;
		expDebug::dPrint(' $listOnlyInstructors = '. print_r($listOnlyInstructors, true),4);

		$exclude_logged_user = (isset($_GET['exclude_logged_user']) && $_GET['exclude_logged_user'] == 1)? true : false;
		expDebug::dPrint(' $exclude_logged_user = '. print_r($exclude_logged_user,true),4);

		$nameSubstr = $_GET['z'];
		expDebug::dPrint(' $nameSubstr = '. print_r($nameSubstr, true),4);

		$limit = (empty($_GET['limit']))? 0 : $_GET['limit'];
		expDebug::dPrint(' $limit = '. print_r($limit, true),4);

		$loggedUserId = getSltpersonUserId();
		// Select the table
		$select = db_select('slt_person', 'per');

		// Add fields to fetch
		if ($idRequired || $userNameAsField || $userNameAppended) {
			$select->addField('per', 'full_name', 'name');
			if ($idRequired) {
				$select->addField('per', 'id', 'id');
			}
			if ($userNameAsField || $userNameAppended) {
				$select->addField('per', 'user_name', 'user_name');
			}
		}
		else if(!empty($nameSubstr)) {
			$select->addField('per', 'full_name', 'name');
		}
		else { // When neither id is required nor username has been fetched, return distinct user full names.
			$select->addExpression('DISTINCT(per.full_name)', 'name');
		}

		// Add conditions
		// Return only the active records.
		$select->condition('per.status', 'cre_usr_sts_atv');

		// Always exclude 'admin' and 'guest' users
		$select->condition('per.user_name', array('admin', 'guest'), 'NOT IN');
		if($exclude_logged_user)	#fix for issues 0023841 & 0025347
		$select->condition('per.id', $loggedUserId, '<>');

		if ($searchByUsername) {
			$select->condition(db_or()
			->condition('per.full_name', '%' . db_like($nameSubstr) . '%', 'LIKE')
			->condition('per.user_name', '%' . db_like($nameSubstr) . '%', 'LIKE'));
		}
		else {
			$select->condition('per.full_name', '%' . db_like($nameSubstr) . '%', 'LIKE');
		}

		if (!empty($excludedUserId) && $excludedUserId > 0) {
			$excludedUserIdList = array($excludedUserId);
			if ($excludeReportees) {
				$excludedUserIdList = array_merge($excludedUserIdList, getAllDirectIndirectReporteesIds($excludedUserId));
			}
			$select->condition('per.id', $excludedUserIdList, 'NOT IN');
		}

		if ($listOnlyManagers) {
			$select->condition('per.is_manager', 'Y', '='); // @TODO Manager logic also includes users who have reportees
			// even if is_manager is not set to 1 for them
		}
		if ($listOnlyInstructors) {
			$select->condition('per.is_instructor', 'Y', '=');
		}
		$select->orderBy('name'); // order by full name
		filterByUserAccessPrivileges($select,'per','',1,'cre_usr','attach','per');

		// limit number of records to be fetched and returned
		if ($limit > 0) {
			$select->range(0, $limit);
		}
		expDebug::dPrintDBAPI(' $select SQL = ' ,$select);
		$matchingPersons = $select->execute()->fetchAll();
		expDebug::dPrint(' $matchingPersons = ' . print_r($matchingPersons, true),3);
		foreach ($matchingPersons as $person) {
			expDebug::dPrint(' $person = ' . print_r($person, true),3);
			$choiceStr = '';
			if ($idRequired) {
				$choiceStr .= $person->id . EXP_AC_SEPARATOR;
			}

			$choiceStr .= $person->name;

			if ($userNameAsField) {
				$choiceStr .= EXP_AC_SEPARATOR . $person->user_name;
			}
			else if ($userNameAppended) {
				$choiceStr .= ' (' . $person->user_name . ')';
			}
			expDebug::dPrint(' $choiceStr = ' . print_r($choiceStr, true),4);
			print $choiceStr . "\n";
		} // end foreach
	}
	catch (Exception $ex) {
		watchdog_exception('getPersonAutoComplete', $ex);
		expertusErrorThrow($ex);
	}
}

/**  Added/changed by ganeshbabuv on Jan 11th 2016 for  Location Filter #66404
 * Set the Location Query format according to partial and full location search
 * if $need_country_search=1 , then add the query for country search 
 */ 
 function getLocationFilterQuery($tmp_location_sets,$need_country_search='',$loc_aliase='loc',$loc_state_aliase='loc_stats',$loc_country_aliase='loc_ctry'){
  	try{	
		
			if(trim($loc_aliase)=='' || empty($loc_aliase)) $loc_aliase='loc';
			if(trim($loc_state_aliase)=='' || empty($loc_state_aliase)) $loc_state_aliase='loc_stats';
			if(trim($loc_country_aliase)=='' || empty($loc_country_aliase)) $loc_country_aliase='loc_ctry';		
			
			expDebug::dPrint('$tmp_location_sets ='.$tmp_location_sets.' --> $need_country_search ='.$need_country_search.' --> $loc_aliase ='.$loc_aliase.' --> $loc_state_aliase ='.$loc_state_aliase.' --> $loc_country_aliase ='.$loc_country_aliase.' --> $_REQUEST[apiname] ='.$_REQUEST['apiname'],5);
			
			if($tmp_location_sets!=''){
				
				$tmp_location_arr = explode("|",$tmp_location_sets); //Spilit it if multiple location , which gives with | symbol whether partial or full locatio  address search
	    		
	    		expDebug::dPrint('$tmp_location_arr ='.print_r($tmp_location_arr,true),5);
				
	    		$locationArray = db_or();
				
				foreach($tmp_location_arr as $tmp_location_arr_key => $tmp_location){
					
					$loc_split_arr=array(); 
			
					if($_REQUEST['apiname'] == 'ListEnrollmentsAPI' && $_REQUEST['mobile_version']){
						if(strpos($tmp_location,'~')!== false){
							$loc_split_arr=explode('~',$tmp_location);
							$loc_split_arr=array_map('trim',$loc_split_arr);
						}
						$ui_segregator='';
						$loc_col_db_str='concat_ws(\''.$ui_segregator.'\','.$loc_aliase.'.LocationName,'.$loc_aliase.'.LocationAddr1,'.$loc_aliase.'.LocationAddr2,'.$loc_aliase.'.LocationCity,'.$loc_aliase.'.LocationZip,'.$loc_state_aliase.'.stateName,'.$loc_country_aliase.'.LocationCountry)';
					}
					else if(isset($_REQUEST['apiname']) && trim($_REQUEST['apiname'])!=''){
					 	  if(strpos($tmp_location,'~')!== false){
					 	  	 $loc_split_arr=explode('~',$tmp_location);	
							 $loc_split_arr=array_map('trim',$loc_split_arr); 
					 	  }
						 $ui_segregator='';
						 $loc_col_db_str='concat_ws(\''.$ui_segregator.'\','.$loc_aliase.'.name,'.$loc_aliase.'.addr1,'.$loc_aliase.'.addr2,'.$loc_aliase.'.city,'.$loc_aliase.'.zipcode,'.$loc_state_aliase.'.state_name,'.$loc_country_aliase.'.country_name)';
					 }else{
					 	   if(strpos($tmp_location,',')!== false){
					 	   	  $loc_split_arr=explode(',',$tmp_location);
					 	  } 
						  $ui_segregator=' ';	 //if come from UI, comma with space will be there like location name, addr1, addr2...
						  $loc_col_db_str='replace(concat_ws(\''.$ui_segregator.'\','.$loc_aliase.'.name,'.$loc_aliase.'.addr1,'.$loc_aliase.'.addr2,'.$loc_aliase.'.city,'.$loc_aliase.'.zipcode,'.$loc_state_aliase.'.state_name,'.$loc_country_aliase.'.country_name),\',\',\'\')';
					 }
		    		
					$loc_or_condition='';
					
		    		if(isset($loc_split_arr) && !empty($loc_split_arr) && count($loc_split_arr)>0){ //Full Location Search 
		    		
		    		     $remove_comma_from_loc_str=addslashes(implode('',$loc_split_arr)); 
						 $tmp_full_loc_cond=$loc_col_db_str.' like \'%'.$remove_comma_from_loc_str.'%\'';
						 $loc_or_condition = db_and()->where($tmp_full_loc_cond);   
		    			
		    		}else{ //Partial Search  for API and wherever not applicable for country filter in UI

		    			$tmp_location=stripslashes(db_like($tmp_location));
						
					    if((isset($_REQUEST['apiname']) && ($_REQUEST['apiname'] == 'CatalogSearchAPI')) || $need_country_search=='1'){ //For API 
					    
							 $loc_or_condition = db_or()->condition($loc_aliase.'.name', '%' . $tmp_location. '%', 'LIKE')
				    								   ->condition($loc_aliase.'.addr1', '%' . $tmp_location . '%', 'LIKE')
													   ->condition($loc_aliase.'.addr2', '%' . $tmp_location . '%', 'LIKE')
													   ->condition($loc_aliase.'.city', '%' . $tmp_location . '%', 'LIKE')
													   ->condition($loc_state_aliase.'.state_name', '%' . $tmp_location . '%', 'LIKE')
													   ->condition($loc_aliase.'.zipcode', '%' . $tmp_location . '%', 'LIKE')
						  							   ->condition($loc_country_aliase.'.country_name', '%' . $tmp_location . '%', 'LIKE');
						}else{ //For UI	 
							  
							   $loc_or_condition = db_or()->condition($loc_aliase.'.name', '%' . $tmp_location. '%', 'LIKE')
				    								   ->condition($loc_aliase.'.addr1', '%' . $tmp_location . '%', 'LIKE')
													   ->condition($loc_aliase.'.addr2', '%' . $tmp_location . '%', 'LIKE')
													   ->condition($loc_aliase.'.city', '%' . $tmp_location . '%', 'LIKE')
													   ->condition($loc_state_aliase.'.state_name', '%' . $tmp_location . '%', 'LIKE')
													   ->condition($loc_aliase.'.zipcode', '%' . $tmp_location . '%', 'LIKE');  
						}	
				  }
               	  $locationArray->condition($loc_or_condition); 
				}                
			} 
			 

			if($_REQUEST['apiname'] == 'ListEnrollmentsAPI' && $_REQUEST['mobile_version'])
			{
				return $tmp_full_loc_cond;
			}
			 return $locationArray; 
			 		
	}catch (Exception $ex) {
  		watchdog_exception('getLocationFilterQuery', $ex);
  		expertusErrorThrow($ex);
  	}
 }

/**
 * Get results for Location autocomplete.
 *
 * @param string ''
 *
 * @return
 *   Auto Complete Location Results 
 */
 
 /* Added/changed by ganeshbabuv on Jan 11th 2016 for  Location Filter #66404 */ 
 function getLocationAutoComplete(){
	try{
	    	 
		$location = $_GET['z'];
		$select = db_select('slt_location', 'sl');
		$select->leftJoin('slt_country','loc_ctry','loc_ctry.country_code=sl.country');
		$select->leftJoin('slt_state','loc_stats','loc_stats.state_code = sl.state and loc_stats.country_code=sl.country');  
		
		$select->addExpression('DISTINCT sl.name', 'name');
		$select->addExpression('sl.id', 'loc_id');
		$select->addExpression('sl.addr1', 'addr1');
		$select->addExpression('sl.addr2', 'addr2');
		$select->addExpression('sl.city', 'city');
		$select->addExpression('sl.zipcode', 'zipcode');
		$select->addExpression('loc_stats.state_name', 'state_name');
		$select->addExpression('loc_ctry.country_name', 'country_name');
		
		$loc_or_condition = db_or()->condition('sl.name', '%' . db_like($location) . '%', 'LIKE')
    								   ->condition('sl.addr1', '%' . db_like($location) . '%', 'LIKE')
									   ->condition('sl.addr2', '%' . db_like($location) . '%', 'LIKE')
									   ->condition('loc_ctry.country_name', '%' . db_like($location) . '%', 'LIKE')/*Viswanathan added for #0074311*/
									   ->condition('sl.city', '%' . db_like($location) . '%', 'LIKE')
									   ->condition('loc_stats.state_name', '%' . db_like($location) . '%', 'LIKE')
									   ->condition('sl.zipcode', '%' . db_like($location) . '%', 'LIKE');
			
			
		$select->condition($loc_or_condition);  	
		 
		$select->condition('sl.status', 'lrn_res_loc_atv', '=');//added by joolavasavi #51529: Location shows in auto complete though location is not active state.
		
		 //filterByUserAccessPrivileges($select, 'sl','',1,'cre_sys_obt_loc','','sl'); 
		
		$qry_loc_autocomplete = $select->execute()->fetchAll();
		
		foreach ($qry_loc_autocomplete as $rec_loc_autocomplete) {
			//	print htmlentities($rec_loc_autocomplete->name,ENT_QUOTES)."\n";	//change by ayyappan for 42444: In catalog location filter if the location contains special character
			$loc_str=trim($rec_loc_autocomplete->name);
			
			$rec_loc_autocomplete->addr1=trim($rec_loc_autocomplete->addr1);
			if($rec_loc_autocomplete->addr1!=''){
				$loc_str.=", ".$rec_loc_autocomplete->addr1;
			}
			
			$rec_loc_autocomplete->addr2=trim($rec_loc_autocomplete->addr2);
			if($rec_loc_autocomplete->addr2!=''){
				$loc_str.=", ".$rec_loc_autocomplete->addr2;
			}
			
			$rec_loc_autocomplete->city=trim($rec_loc_autocomplete->city);
			if($rec_loc_autocomplete->city!=''){
				$loc_str.=", ".$rec_loc_autocomplete->city;
			}
			
			$rec_loc_autocomplete->zipcode=trim($rec_loc_autocomplete->zipcode);
			if($rec_loc_autocomplete->zipcode!=''){
				$loc_str.=", ".$rec_loc_autocomplete->zipcode;
			}
			
			$rec_loc_autocomplete->state_name=trim($rec_loc_autocomplete->state_name);			
			if($rec_loc_autocomplete->state_name!=''){
				$loc_str.=", ".$rec_loc_autocomplete->state_name;
			}
			
			$rec_loc_autocomplete->country_name=trim($rec_loc_autocomplete->country_name);			
			if($rec_loc_autocomplete->country_name!=''){
				$loc_str.=", ".$rec_loc_autocomplete->country_name;
			}  
			print $loc_str."\n"; 			
		}
	}catch (Exception $ex) {
		watchdog_exception('getLocationAutoComplete', $ex);
		expertusErrorThrow($ex);
	}
} 
 /*
 * getCodeForCustomProfile() - Get the code for a custom profile
 */
function getCodeForCustomProfile($profileTag) {
	// This should be obtained from slt_profile_tagging_defn
	// select code from slt_profile_tagging_defn where profile_cat = 'Custom Profile' and profile_tag = $profileTag
	// However, this is master data which is unlikely to change.
	try{
		switch ($profileTag) {
			case 'Organization':
				return 12;
				break;
		}
	}catch (Exception $ex) {
		watchdog_exception('getCodeForCustomProfile', $ex);
		expertusErrorThrow($ex);
	}
}


/*
 * getCourseAutoComplete() - Returns the matched course's title
 */
function getCourseAutoComplete() {
	try{
		$searchText =  $_GET['z'];
		// Select the table
		$select = db_select('slt_course_template', 'course');
		// Add fields to fetch
		$select->addField('course', 'title', 'title');
		// Add condition
		$select->condition('course.title', '%' . db_like($searchText) . '%', 'LIKE');
		$select->condition('course.status', 'lrn_crs_sts_del', '!=');
		filterByUserAccessPrivileges($select, 'course','',1,'cre_sys_obt_crs','','course');
		// Order in dictionary sort order
		$select->orderBy('title');
		expDebug::dPrintDBAPI(' $select obj = ' , $select );
		$matchingCourseNames = $select->execute()->fetchAll();
		expDebug::dPrint(' $matchingCourseNames = '. print_r($matchingCourseNames, true),3);
		foreach ($matchingCourseNames as $courseName) {
			print $courseName->title . "\n";
		}
	}catch (Exception $ex) {
		watchdog_exception('getCourseAutoComplete', $ex);
		expertusErrorThrow($ex);
	}
}
/**
 * Get results for Tag autocomplete.
 *
 * @param string ''
 *
 * @return
 *   Auto Complete Tag Results
 */
function getTagnameAutoComplete(){
	try{
		$tag = $_GET['z'];
		$select = db_select('slt_tagdefn', 'sl');
		$select->addField('sl', 'tagname');
		$select->condition('sl.tagname', '%' . db_like($tag) . '%', 'LIKE');
		expDebug::dPrintDBAPI('$select', $select);
		$qry_tag_autocomplete = $select->execute()->fetchAll();
		expDebug::dPrint('$qry_tag_autocomplete = ' . print_r($qry_tag_autocomplete, true));
		foreach ($qry_tag_autocomplete as $rec_tag_autocomplete) {
			print $rec_tag_autocomplete->tagname."\n";
		}
	}catch (Exception $ex) {
		watchdog_exception('getTagnameAutoComplete', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * getCourseTagnameAutoComplete()
 */
function getCourseTagnameAutoComplete(){
	try{
		$tag = $_GET['z'];
		$select = db_select('slt_tagdefn', 'sl');
		$select->leftJoin('slt_tag_entity', 'en', 'en.tagid = sl.id');
		$select->addField('sl', 'tagname');
		$select->condition('sl.tagname', '%' . db_like($tag) . '%', 'LIKE');
		$select->condition('en.entity_type', 'Course');
		expDebug::dPrintDBAPI('$select', $select);
		$qry_tag_autocomplete = $select->distinct()->execute()->fetchAll();
		expDebug::dPrint('$qry_tag_autocomplete = ' . print_r($qry_tag_autocomplete, true));
		foreach ($qry_tag_autocomplete as $rec_tag_autocomplete) {
			print $rec_tag_autocomplete->tagname."\n";
		}
	}catch (Exception $ex) {
		watchdog_exception('getCourseTagnameAutoComplete', $ex);
		expertusErrorThrow($ex);
	}
}
/**
 * getTrainingplanTagnameAutoComplete()
 */
function getTrainingplanTagnameAutoComplete(){
	try{
		$tag = $_GET['z'];
		$select = db_select('slt_tagdefn', 'sl');
		$select->leftJoin('slt_tag_entity', 'en', 'en.tagid = sl.id');
		$select->distinct();
		$select->addField('sl', 'tagname');
		$select->condition('sl.tagname', '%' . db_like($tag) . '%', 'LIKE');
		$trainingplanTagname = db_or();
		$trainingplanTagname->condition('en.entity_type', 'Course','=');
		$trainingplanTagname->condition('en.entity_type', 'Certification','=');
		$trainingplanTagname->condition('en.entity_type', 'Learning Plan','=');
		$trainingplanTagname->condition('en.entity_type', 'Curricula','=');
		$select->condition($trainingplanTagname);
		expDebug::dPrintDBAPI('getTrainingplanTagnameAutoComplete :', $select);
		$qry_tag_autocomplete = $select->execute()->fetchAll();
		expDebug::dPrint('getTrainingplanTagnameAutoComplete $qry_tag_autocomplete:'. print_r($qry_tag_autocomplete,true));
		foreach ($qry_tag_autocomplete as $rec_tag_autocomplete) {
			print $rec_tag_autocomplete->tagname."\n";
		}
	}catch (Exception $ex) {
		watchdog_exception('getTrainingplanTagnameAutoComplete', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * getContentTagnameAutoComplete()
 */
function getContentTagnameAutoComplete(){
	try{
		$tag = $_GET['z'];
		$select = db_select('slt_tagdefn', 'sl');
		$select->leftJoin('slt_tag_entity', 'en', 'en.tagid = sl.id');
		$select->distinct();
		$select->addField('sl', 'tagname');
		$select->condition('sl.tagname', '%' . db_like($tag) . '%', 'LIKE');
		$select->condition('en.entity_type', 'Content');
		expDebug::dPrintDBAPI('getContentTagnameAutoComplete :'.$select);
		$qry_tag_autocomplete = $select->execute()->fetchAll();
		expDebug::dPrint('getTrainingplanTagnameAutoComplete $qry_tag_autocomplete:'. print_r($qry_tag_autocomplete,true));
		foreach ($qry_tag_autocomplete as $rec_tag_autocomplete) {
			print $rec_tag_autocomplete->tagname."\n";
		}
	}catch (Exception $ex) {
		watchdog_exception('getCourseTagnameAutoComplete', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * getClassTagnameAutoComplete()
 */
function getClassTagnameAutoComplete(){
	try{
		$tag = $_GET['z'];
		// Select tables to fetch from
		$select = db_select('slt_tag_entity', 'tagentity');
		$select->leftJoin('slt_tagdefn', 'tagdefn', 'tagentity.tagid = tagdefn.id');
		// Select fields to fetch
		$select->distinct()->addField('tagdefn', 'tagname', 'tagname');
		// Add conditions
		$select->condition(db_or()->condition('tagentity.entity_type', 'Class')
		->condition('tagentity.entity_type', 'Course'));
		$select->condition('tagdefn.tagname', '%' . db_like($tag) . '%', 'LIKE');
		expDebug::dPrintDBAPI(' $select SQL = ' , $select);
		$tagsRecList = $select->execute()->fetchAll();
		expDebug::dPrint(' $tagsRecList = ' . print_r($tagsRecList, true),3);
		foreach ($tagsRecList as $tagRec) {
			print $tagRec->tagname . "\n";
		}
	}catch (Exception $ex) {
		watchdog_exception('getClassTagnameAutoComplete', $ex);
		expertusErrorThrow($ex);
	}
}
/*
 * getUsernameAutoComplete() - Returns the matched username from slt_person table.
 */
function getUsernameAutoComplete() { // Needed for search by manager username.
	try{
		$searchText =  $_GET['z'];
		$limit = isset($_GET['limit'])?$_GET['limit']:100;
		// Select the table
		$select = db_select('slt_person', 'per');

		// Add fields to fetch
		$select->addField('per', 'user_name', 'username');

		// Add condition
		$select->condition('per.user_name', '%' . db_like($searchText) . '%', 'LIKE');
		$select->condition('per.user_name', array('admin','guest'),'NOT IN');
		$select->condition('per.status', 'cre_usr_sts_atv','=');
		// Order in dictionary sort order first by lastname and then by first name.
		$select->orderBy('username', 'ASC');
		$select->range(0,$limit); // added by Vincent on Feb 26, 2015 for #0046021
		expDebug::dPrintDBAPI(' $select obj = ' , $select);

		$matchingUserNames = $select->execute()->fetchAll();
		expDebug::dPrint(' $matchingUserNames = '. print_r($matchingUserNames, true),3);

		foreach ($matchingUserNames as $userName) {
			print $userName->username . "\n";
		}
	}catch (Exception $ex) {
		watchdog_exception('getUsernameAutoComplete', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getFullnameAutoComplete() - Returns the matched username from slt_person table.
 */
function getFullnameAutoComplete() { // Needed for search by manager username.
	try{
		$searchText =  $_GET['z'];

		// Select the table
		$select = db_select('slt_person', 'per');

		// Add fields to fetch
		$select->addExpression('DISTINCT(per.full_name)', 'username');
		$select->condition('per.user_name',array('admin','guest'),'NOT IN');
		$select->condition('per.status',array('cre_usr_sts_atv'),'IN');

		// Add condition
		selectAddNarrowSearchByUserNameCondition($select, 'per', $searchText); // Same logic as used to fetch records

		// Order in dictionary sort order first by lastname and then by first name.
		$select->orderBy('username', 'ASC');

		expDebug::dPrintDBAPI(' $select obj = ' , $select);

		$matchingUserNames = $select->execute()->fetchAll();
		expDebug::dPrint(' $matchingUserNames = '. print_r($matchingUserNames, true),3);

		foreach ($matchingUserNames as $userName) {
			print rawurlencode($userName->username) . "\n";
		}
	}catch (Exception $ex) {
		watchdog_exception('getUsernameAutoComplete', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getSurveyAssesmentAutoComplete() - Get matched Survey/Assessment title
 */

function getSurveyAssessmentAutoComplete() {
	try{
		$searchText =  $_GET['z'];

		$searchType = arg(3);

		$searchType = (($searchType == 'assessment') ? 'sry_det_typ_ass' : 'sry_det_typ_sry');

		// Select the table
		$select = db_select('slt_survey', 'survey');

		// Add fields to fetch
		$select->addField('survey', 'title', 'title');

		// Add type condition
		$select->condition('survey.type', '%' . db_like($searchType) . '%', 'LIKE');

		$survAssessCondition = db_or();
		$survAssessCondition->condition('survey.title', '%' . db_like($searchText) . '%', 'LIKE');
		$survAssessCondition->condition('survey.code', '%' . db_like($searchText) . '%', 'LIKE');
		$select->condition($survAssessCondition);
		filterByUserAccessPrivileges($select, 'survey','',1,$searchType,'','survey');
		$select->condition('survey.status', 'sry_det_sry_del', '!=');

		// Order in dictionary sort order
		$select->orderBy('title');

		expDebug::dPrintDBAPI(' $select obj = ' ,$select);

		$matchingSurveyAssessmentNames = $select->execute()->fetchAll();
		expDebug::dPrint(' $matchingSurveyAssessmentNames = '. print_r($matchingSurveyAssessmentNames, true),3);
		foreach ($matchingSurveyAssessmentNames as $surveyAssessmentName) {
			print $surveyAssessmentName->title . "\n";
		}
	} catch (Exception $ex) {
		watchdog_exception('getSurveyAssesmentAutoComplete', $ex);
		expertusErrorThrow($ex);
	}

}

/**
 * Funtion to return the Cart Status.
 *
 * @param
 *	   Class ID/TP ID
 * @return
 *     Status Code
 */
function isCartAdded($nodeId = array(),$user_id = '') {
	try{
		if(empty($user_id)){
			global $user;
			$drupalUserId = $user->uid;
		}else{
			$drupalUserId = $user_id;
		}
		$result = 0;
		if($drupalUserId){
			$select = db_select('uc_cart_products', 'uccart');
			$select->addExpression('count(1)','cnt');
			$select->condition('uccart.cart_id',$drupalUserId,'=');
			$select->condition('uccart.nid',$nodeId,is_array($nodeId) ? 'IN':'=');
			$result = $select->execute()->fetchField();

		}
		return ($result > 0) ? 1 : 0;
	}catch (Exception $ex) {
		watchdog_exception('isCartAdded', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * record_sort - Sorts an array of records (each record is an array with same fields(keywords)) by a specific field value.
 * Code obtained from the comments in http://www.php.net/manual/en/function.asort.php
 * Slightly modified to remove the field from the sorted array.
 */
function record_sort($recordsList, $field, $removeField = false, $reverse = false)
{
	try{
		$hash = array();
		foreach($recordsList as $key => $record) {
			$hash[$record[$field] . $key] = $record;
		}
		expDebug::dPrint(' $hash = ' . print_r($hash, true),4);
		($reverse)? krsort($hash) : ksort($hash);
		expDebug::dPrint(' sorted $hash = ' . print_r($hash, true),4);
		$recordsList = array();

		foreach($hash as $record) {
			if ($removeField) {
				unset($record[$field]);
			}
			$recordsList[] = $record;
		}
		return $recordsList;
	}catch (Exception $ex) {
		watchdog_exception('record_sort', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * function to fetch the attachments from class or course And Training Plan level.
 * by Rajkumar U , on 02-12-2011.
 * Moved this function from Class Detail page to this page (BalaG)
 */
function getAttachmentsInfo($entityId,$type=''){
	try{
		$AttachmentSel =db_select('slt_entity_reading_mapper','serm');
		$AttachmentSel->addField('serm','id');
		$AttachmentSel->addField('serm','reading_id','sequence');
		$AttachmentSel->addField('serm','reading_type','title');
		$AttachmentSel->addField('serm','reading_content','content');
		$AttachmentSel->condition('serm.entity_id',$entityId,'=');
		if ($type=='trainingplan'){ // Added by BalaG on 19-12-2011
			$AttachmentSel->condition('serm.entity_type',array('cre_sys_obt_crt','cre_sys_obt_cur','cre_sys_obt_trn','cre_sys_obt_trp'),'IN');
		} else if ($type=='Course'){
			$AttachmentSel->condition('serm.entity_type',"cre_sys_obt_crs",'=');
		} else {
			$AttachmentSel->condition('serm.entity_type',"cre_sys_obt_cls",'=');
		}
		$AttachmentSel->condition('serm.showlnr',"Y",'=');
		expDebug::dPrintDBAPI(' $$AttachmentSel = ', $AttachmentSel);
		return $AttachmentSel->execute()->fetchAll();
	}catch (Exception $ex) {
		watchdog_exception('getAttachmentsInfo', $ex);
		expertusErrorThrow($ex);
	}

}
/*
 * existsUserEmail() - Check whether a user email already exists in Drupal users table
 *                     Returns false if does not exist, true if exists.
 *                     If $uid is set, excludes uid = $uid record in the search
 */
function existsUserEmail($email, $uid = null) {
	try{
		$select = db_select('users', 'users');
		$select->addExpression('COUNT(*)', 'count');
		$select->condition('users.mail', $email, '=');
		if (!empty($uid) && $uid > 0) {
			$select->condition('users.uid', $uid, '<>');
		}
		$result = $select->execute()->fetchField();
		return (($result > 0)? true : false);
	}catch (Exception $ex) {
		watchdog_exception('existsUserEmail', $ex);
		expertusErrorThrow($ex);
	}
}
/*
 * existsUserName() - Check whether a username already exists
 *                    Returns false if does not exist, true if exists
 *                    If $uid is set, excludes uid = $uid record in the search
 */
function existsUserName($uname, $uid = null) {
	try{
		$select = db_select('users', 'users');
		$select->addExpression('COUNT(*)', 'count');
		$select->condition('users.name', $uname, '=');
		if (!empty($uid) && $uid > 0) {
			$select->condition('users.uid', $uid, '<>');
		}
		$result =  $select->execute()->fetchField();

		return (($result > 0)? true : false);
	}catch (Exception $ex) {
		watchdog_exception('existsUserName', $ex);
		expertusErrorThrow($ex);
	}
}
/*
 * getProgramDetails() - Fetches specified details ($fields) of an program from slt_program
 */
function getTrainingProgramDetails($programId, $fields) {
	try{
		if (empty($fields)) {
			// Get all fields
			$fields = array('id', 'type', 'version', 'lang_code', 'locale_code', 'profile_code', 'title',
					'code', 'description', 'short_desc', 'start_date', 'end_date', 'registration_open_date', 'registration_close_date', 'max_capacity',
					'expiry_date', 'price', 'is_sequenced', 'credit_valid_upto', 'expires_in_value', 'expires_in_unit', 'object_type',
					'currency_type', 'status', 'created_by', 'created_on', 'updated_by', 'updated_on','additional_info','author_vendor',
					'custom0', 'custom1', 'custom2', 'custom3', 'custom4');
		}
		// Select table
		$selectStmt = db_select('slt_program', 'prg');
		$selectStmt->innerJoin('slt_profile_list_items', 'prof1', 'prof1.code = prg.lang_code');
		$selectStmt->innerJoin('slt_profile_list_items', 'prof2', 'prof2.code = prg.status');
		$selectStmt->leftJoin('slt_profile_list_items', 'prof3', 'prof3.attr1 = prg.currency_type');
		$selectStmt->innerJoin('slt_profile_list_items', 'prof4', 'prof4.code = prg.object_type');
		// Set conditions
		$selectStmt->condition('prg.id', $programId);
		// Select fields and/or add expressions
		foreach($fields as $fieldName) {
			$selectStmt->addField('prg', $fieldName, $fieldName);
		}
		$selectStmt->addField('prof1','name','language_name');
		$selectStmt->addField('prof2','name','status_name');
		$selectStmt->addField('prof3','attr1','currency_name');
		$selectStmt->addField('prof4','name','object_type_name');
		expDebug::dPrintDBAPI(' $selectStmt = ' ,$selectStmt);
		// Execute query
		$result = $selectStmt->execute();
		return $result->fetchAssoc();
	}catch (Exception $ex) {
		watchdog_exception('getTrainingProgramDetails', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getSltpersonUserId() - Get logged in user's person id from SPLearnerInfo cookie
 */
function getSltpersonUserId(){
	try{
		global $user;
		// Added by Vincent for performance imporvement on Sep 2, 2015
		if($user->uid == 0 && !$_REQUEST['apiname']) return 0;
		if(isset($_SESSION['logged_user_id']) && !empty($_SESSION['logged_user_id']))
			return $_SESSION['logged_user_id'];
		require_once $_SERVER['DOCUMENT_ROOT']."/getLearnerInfo.php";
		$obj = new GetLearnerInfo();
		$UserId = orgValidate($obj->getValue('userid'));
		$_SESSION['logged_user_id'] = $UserId;
		return $UserId;
	}catch (Exception $ex) {
		watchdog_exception('getSltpersonUserId', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getLoggedInUserDetails() - Get logged in user's details from SPLearnerInfo cookie
 */
function getLoggedInUserDetails(){
	try{
		$userDetails = array('userid' => '',
	                       'username' => '',
	                       'usermail' => '',
	                       'userfirstname' => '',
	                       'userlastname' => '',
	                       'usersavedfullname' => '');

		require_once "./getLearnerInfo.php";
		$obj = new GetLearnerInfo();
		foreach ($userDetails as $detail => $value) {
			$userDetails[$detail] = $obj->getValue($detail);
		}
		expDebug::dPrint('$userDetails = ' . print_r($userDetails, true), 4);
		return $userDetails;
	}
	catch (Exception $ex) {
		watchdog_exception('getLoggedInUserDetails', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * object2array() - Converts PHP object to an array
 */
function object2array($object) {
	try{
		$arr=array();
		if (is_object($object)) {
	  foreach ($object as $key => $value) {
	  	expDebug::dPrint("object2array key===". $key."==". $value,4);
	  	$arr[$key] = $value;
	  }
		}
		else {
	  $arr = $object;
		}
		return $arr;
	}catch (Exception $ex) {
		watchdog_exception('object2array', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * array_to_object() - Converts an array to PHP object
 */
function array_to_object($array = array()) {
	try{
		if (!empty($array)) {
			$data = false;
			$data=new stdClass();
			foreach ($array as $akey => $aval) {
				$data -> {$akey} = $aval;
			}
			return $data;
		}
		return false;
	}catch (Exception $ex) {
		watchdog_exception('array_to_object', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * uploadUserPicture() - Function to upload a user's picture from the path in Form API 'file' field $sourceField.
 *                       To be called from a form submit function.
 */
function uploadUserPicture($sourceField, $drupalUid, $updateDBSeparate = 0) {
	$txn = db_transaction();
	try{
		include_once $_SERVER["DOCUMENT_ROOT"]."/includes/file.inc";

		$validators  = array(
    'file_validate_extensions' => array('png gif jpg jpeg'), // Validate extensions.
    'file_validate_is_image' => array(), // Validates file is really an image.
    'file_validate_image_resolution' => array('85x85'),
		);

		$picture = file_save_upload($sourceField, $validators);
		expDebug::dPrint(' $picture returned by file_save_upload() = ' . print_r($picture, true),4);

		if(!empty($picture)) {
			if (!$picture->status) { // i.e. if picture is a temporary file
				// Prepare the pictures directory.
				$picture_directory = file_default_scheme() . '://' . variable_get('user_picture_path', 'pictures');
				expDebug::dPrint(' $picture_directory = ' . print_r($picture_directory, true),4);
				file_prepare_directory($picture_directory, FILE_CREATE_DIRECTORY);

				// Prepare the unique file name to place the picture under the $picture_directory.
				$info = image_get_info($picture->uri);
				expDebug::dPrint(' $info returned by image_get_info = ' . print_r($info, true),4);
				$destination = file_stream_wrapper_uri_normalize($picture_directory . '/picture-' . $drupalUid . '-' . REQUEST_TIME . '.' . $info['extension']);
				expDebug::dPrint(' $destination = ' . print_r($destination, true),4);

				// Move the temporary file into the final location.
				if ($picture = file_move($picture, $destination, FILE_EXISTS_RENAME)) {
					expDebug::dPrint(' $picture returned by file_move() = ' . print_r($picture, true),4);
					// If present, delete the previous picture
					$account = user_load($drupalUid);
					if (!empty($account->picture->fid)) {
						// Decriment file usage count in file_usage table, and delete the file if file_usage count becomes 0
						// We are registering against the user module and user type
						file_usage_delete($account->picture, 'user', 'user', $drupalUid);
						// If audit trail is active, we cannot delete the picture
						if (!module_exists(exp_sp_audit_trail)) {
							file_delete($account->picture);
						}
					}

					// Update file status from temporary to permanent so that cron does not clean it out
					$picture->status = FILE_STATUS_PERMANENT;
					$picture = file_save($picture);
					expDebug::dPrint(' $picture returned by file_save() = ' . print_r($picture, true),4);

					// Increment file usage count in file_usage table
					file_usage_add($picture, 'user', 'user', $drupalUid);

					if($updateDBSeparate == 0){
						// Save the picture reference (file id) in users table against the user's record.
						$numUpdated = db_update('users')
						->fields(array('picture' => $picture->fid))
						->condition('uid', $drupalUid)
						->execute();
						expDebug::dPrint('$numUpdated = ', $numUpdated);
					}
					return ($updateDBSeparate == 1) ? $picture->fid : true;
				}
			}
		}

		return false;
	}catch (Exception $ex) {
		$txn->rollback();
		unset($txn);
		watchdog_exception('uploadUserPicture', $ex);
		expertusErrorThrow($ex);
	}
	// Commit the transaction
	unset($txn);
}
/*
 * listProfileValues() - Fetch active profile values.
 */
function listProfileValues($code){
	try{
		$sel = db_select('slt_profile_list_items','profList');
		$sel->addField('profList','code');
		$sel->addField('profList','name');
		$sel->addField('profList','attr1');
		$sel->addField('profList','attr2');
		$sel->Condition('profList.code','%'.$code.'%','like');
		$sel->Condition('profList.code',$code,'<>');
		$sel->Condition('profList.is_active','Y');
		expDebug::dPrintDBAPI('select sp data', $sel, 4);
		$totalRecords = $sel->countQuery()->execute()->fetchField();
		$result =  $sel->execute()->fetchAll();
		expDebug::dPrint('list of sp '.print_r($result, 1));
		 if(count($result) >0){
		 	$result['totalrow'] = $totalRecords;
			return $result;
		}else{
			include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration.inc";
			expDebug::dPrint('GET ARRAY'.print_r($_POST, 1));
			$errobj = new stdClass();
			if(getURLParam('apiname') == "ListDeliveryTypes") {
				$errobj->errormsg = "deliveryType Code is invalid ";
				$errobj->errorfield = "deliveryTypeCode";
			}
			else if(getURLParam('apiname') == "ListTimezonesAPI"){
				$errobj->errormsg = "timezonecode is invalid ";
				$errobj->errorfield = "timezonecode";
			}
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			expDebug::dPrint('$errobj'.print_r($errobj, 1));

			return $errobj;
		}//changed by joolavasavi for 0050451: List Of Delivery Types
		//return $sel->execute()->fetchAll();
	}catch (Exception $ex) {
		watchdog_exception('listProfileValues', $ex);
		expertusErrorThrow($ex);
	}
}
/*
 * sendNewUserNotification() - Shared function between exp_sp_register module and new user admin module
 *                             for sending a new_user notification on new user creation.
 */
function sendNewUserNotification($newUserId, $createUserId) {
	try{
		// Note : The $createUserId is ignored by slp_lnr_notification_ins for new_user notification. $newUserId is used instead.
		//        See ticket #8412
		$dao = new AbstractDAO();
		$dao->connect();
		$callStoredProcStmt = "CALL slp_lnr_notification_ins('new_user', $newUserId , NULL , NULL , NULL , $createUserId)";
		expDebug::dPrint(' $callStoredProcStmt = '. $callStoredProcStmt,4);
		$dao->query($callStoredProcStmt);
		$dao->closeconnect();
	}catch (Exception $ex) {
		watchdog_exception('sendNewUserNotification', $ex);
		expertusErrorThrow($ex);
	}
}
/*
 * function updatePasswordPolicyHistory() - Function to update password policy history table when user password is changed
 */
function updatePasswordPolicyHistory($uid, $password) {
	$txn = db_transaction();
	try{
		if (module_exists('password_policy')) {
			$usrPassPolicy  = db_insert('password_policy_history');
			$usrPassPolicy->fields(array(
                    'uid' => $uid,
                    'pass' =>$password,
                    'created' => time(),
			));
			expDebug::dPrintDBAPI('$usrPassPolicy' , $usrPassPolicy);

			$usrPassPolicy->execute();
		}
	}catch (Exception $ex) {
		$txn->rollback();
		unset($txn);
		watchdog_exception('updatePasswordPolicyHistory', $ex);
		expertusErrorThrow($ex);
	}
	// Commit the transaction
	unset($txn);
}

/*
 * add_audit_trail_entry - If the module exp_sp_audit_trail is installed and enabled, adds audit trail entry in slt_audit_trail table
 */
function add_audit_trail_entry($loggedUserId, $entityId, $entityType, $moduleName, $functionalityName, $loggedUserAction,
$modifiedUserId = null, $oldValue = null, $newValue = null,
$custom0 = null, $custom1 = null, $custom2 = null, $custom3 = null, $custom4 = null) {
	try{
		if (module_exists('exp_sp_audit_trail')) {
			module_load_include('inc', 'exp_sp_audit_trail');
			$insertId = exp_sp_audit_trail_add_entry($loggedUserId, $entityId, $entityType, $moduleName, $functionalityName, $loggedUserAction,
			$modifiedUserId, $oldValue, $newValue, $custom0, $custom1, $custom2, $custom3, $custom4);
			return $insertId;
		}
	}catch (Exception $ex) {
		watchdog_exception('add_audit_trail_entry', $ex);
		expertusErrorThrow($ex);
	}
}
function add_person_preference($loggedUserId){
	try{
	//expDebug::dPrint('chekedt hte looged in user' , $loggedUserId);
	if(!empty($loggedUserId)){
	$exists = db_query("SELECT user_id FROM {slt_person_preference} WHERE user_id = :user_id", array(':user_id' => $loggedUserId))->fetchField();
	//expDebug::dPrintDBAPI('$$exists' , print_r($exists,true),5);
if(empty($exists)) {
		db_insert('slt_person_preference')
	->fields(array(
			'user_id' => $loggedUserId,	
			'created_by' => $sltPersonId,
			'created_on' => now(),
			'catalog_visited'=>null,
			'mylearning_visited'=>null,
			'updated_by' => $loggedUserId,
			'updated_on' => now()
	))
	->execute(); 
	}}
	}catch (Exception $ex) {
		watchdog_exception('add_person_preference', $ex);
		expertusErrorThrow($ex);
	}
}
function update_person_preference($loggedUserId,$param){
	try{
		$param_updated = db_update('slt_person_preference')
		->fields(array(
				"$param" => 1,
		))
		->condition('user_id', $loggedUserId, '=')
		->execute();
	}catch (Exception $ex) {
		watchdog_exception('update_person_preference', $ex);
		expertusErrorThrow($ex);
	}
}
function checkIfEnrolled($userId){
	try{
		$selectStmt = db_select('slt_enrollment', 'enr');
		$selectStmt->condition('enr.user_id', $userId);
		$selectStmt->condition('enr.reg_status', array('lrn_crs_reg_rsv','lrn_crs_reg_rsc'),'NOT IN');
		$selectStmt->addExpression('count(enr.id)');
		expDebug::dPrintDBAPI('ENROLLMENT/CATALOG LOGIN BASED ON CLASSES ENROLLED  ' , $selectStmt);
		// Execute query
		$result = $selectStmt->execute()->fetchField();
		expDebug::dPrint($result,3);
		return $result;
	}catch (Exception $ex) {
		watchdog_exception('checkIfEnrolled', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * Audit Trail Multiple entries
 */
function make_audit_trail_entries($resultRecord, $userId, $defaultValues, $formid, $action, $entityType , $moduleName, $functionalityName, $formFieldsList = array(),$modifiedUserId= null) {
	try{
		expDebug::dPrint(' result$formField = ' . print_r($resultRecord, true),4);
		expDebug::dPrint(' default$formField = ' . print_r($defaultValues, true),4);
		expDebug::dPrint(' modifiedUserId= ' .$modifiedUserId,4);
		$activity_id=null;
		foreach ($formFieldsList as $formField) {
			if (TRIM($resultRecord[$formField]) != trim($defaultValues[0]->$formField)) {
				expDebug::dPrint(' newVal = ' . print_r(TRIM($resultRecord[$formField]), true),4);
				expDebug::dPrint(' oldVal = ' . print_r($defaultValues[0]->$formField, true),4);
				$insertId = add_audit_trail_entry($userId, $formid, $entityType, $moduleName, $functionalityName, $action  .' '. $formField,
				$modifiedUserId, trim($defaultValues[0]->$formField),($resultRecord[$formField]),null,null,null,null,$activity_id);
				if($activity_id==null){
					$activity_id = $insertId;
				}
	  }
		}
	}catch (Exception $ex) {
		watchdog_exception('make_audit_trail_entries', $ex);
		expertusErrorThrow($ex);
	}
}

function getProfileListItemByCode($code,$languageCode = 'cre_sys_lng_eng'){
	try{
		$select = db_select('slt_profile_list_items', 'plist');
		$select->addField('plist','id', 'id');
		$select->addField('plist','name', 'name');
		$select->addField('plist','is_active', 'status');
		$select->addField('plist','attr1', 'attr1');
		$select->addField('plist','attr2', 'attr2');
		$select->addField('plist','attr3', 'attr3');
		$select->addField('plist','attr4', 'attr4');
		$select->condition('plist.code', $code,'=');
		$select->condition('plist.lang_code', $languageCode,'=');
		$result = $select->execute()->fetchAll();
		return $result;
	}catch (Exception $ex) {
		watchdog_exception('getProfileListItemByCode', $ex);
		expertusErrorThrow($ex);
	}
}
function getUserInformations($userId){
	try {
		$select = db_select('slt_person', 'per');
		$select->leftjoin('slt_profile_list_items', 'spli', 'spli.attr1 = per.preferred_currency AND spli.code LIKE \'cre_sys_crn_%\' AND spli.is_active=\'Y\' AND spli.attr3=\'Y\'');
		$select->addField('per','first_name','first_name');
		$select->addField('per','last_name','last_name');
		$select->addField('per','email','email');
		$select->addField('per','preferred_language','preferred_language');
		$select->addField('per','preferred_currency','preferred_currency');
		$select->addField('spli','code','code');
		$select->addField('spli','attr2','attr2');
		$select->condition('per.id',$userId);
		expDebug::dPrintDBAPI("get Person details Query", $select);
		// Execute the query
		$result = $select->execute()->fetchAssoc();
		return $result;
	} catch (Exception $ex) {
		watchdog_exception('getUserInformations', $ex);
		expertusErrorThrow($ex);
	}
}


function LdapFetchUSerDetails($userid = null){
	try{
		if($userid == null){
			$userid = getSltpersonUserId();
		}
		$custom   = db_select('slt_person', 'pers');
		$custom->addField('pers','first_name','first_name');
		$custom->addField('pers','last_name','last_name');
		$custom->addField('pers','time_zone','tzone');
		$custom->addField('pers','custom0','custom0');
		$custom->condition('id', $userid);
		$custom0 = $custom->execute()->fetchAll();
		return $custom0;
	}catch (Exception $ex) {
		watchdog_exception('LdapFetchUSerDetails', $ex);
		expertusErrorThrow($ex);
	}

}

/*
 * insMobileLogData() - Insert log data into slt_mobile_log.
 */
function insMobileLogData($userId,$logData){
	try{
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		$sel = db_select('slt_person','person');
		$sel->addField('person','email');
		$sel->Condition('person.id',$userId);
		$email =  $sel->execute()->fetchField();

		$txn = db_transaction();
		try {
			$insLog  = db_insert('slt_mobile_log');
			$insLog->fields(array(
                      'user_id' => $userId,
                      'email' => $email,
                      'logdata' => $logData,
                      'created_by' => $userId,
                      'created_on' => now(),
    									'updated_by' => $userId,
    									'updated_on' => now()
			));
			$logId = $insLog->execute();
			return array((object)array('id'=>$logId));
		}
		catch (Exception $ex) {
			$txn->rollback();
			unset($txn);
			watchdog_exception('orgadmin', $ex);
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Error in inserting log data";
			return $errobj;
			//return array((object)array('id'=>"failure"));
		}
		unset($txn);
	}catch (Exception $ex) {
		watchdog_exception('insMobileLogData', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * getProfileItemNamesKeyedArrayCode() - Function to fetch all profile list item names keyed by code whose code in the $arrayCode array
 *                              The result is ordered by name. This function is used to build selection controls (e.g. dropdown and checkboxes)
 *                              for the new admin screen but can be used in other places as well.
 */
function getProfileItemNamesKeyedArrayCode($arrayCode, $langCode = 'cre_sys_lng_eng', $alsoFetchInactiveItems = false) {
	try{
		//$codeList  = explode(',',$arrayCode);
		// Select the table
		$select = db_select('slt_profile_list_items','spli');

		// Select the fields to fetch
		$select->addField('spli','code', 'code');
		$select->addField('spli','name', 'name');

		// Order results by name
		$select->orderBy('name');

		// Set the conditions
		$select->condition('spli.code', $arrayCode , 'IN');
		$select->condition(db_and()->condition('spli.lang_code', $langCode, '='));
		if (!$alsoFetchInactiveItems) {
			$select->condition(db_and()->condition('spli.is_active', 'Y', '='));
		}
		expDebug::dPrintDBAPI(' $select object = ' , $select);
		$result = $select->execute()->fetchAllKeyed();
		expDebug::dPrint(' $result = ' . print_r($result, true),3);
		// Return results
		return $result;
	}catch (Exception $ex) {
		watchdog_exception('getProfileItemNamesKeyedArrayCode', $ex);
		expertusErrorThrow($ex);
	}
}//End getProfileItemNamesKeyedArrayCode()

/*
 * Returns the resultset for the file information.
 * @param params filePath
 * @return fileinfo
 */

function getUploadFileInfo($filePath,$dt="",$basetype ="")
{
	expDebug::dPrint("dt.".$dt." filePath::".$filePath,4);
	if(strtolower($dt)=="scorm" || strtolower($dt)=="scorm 1.2" || strtolower($dt)=="scorm 2004")
	{
		if(strpos($filePath,".zip")<=0)
		{
			$filePathStr=explode("/",$filePath);
			expDebug::dPrint("filepath str::".$filePathStr[3],4);
			///sites/default/files/contentupload
			$filePath=$filePathStr[0]."/".$filePathStr[1]."/".$filePathStr[2]."/".$filePathStr[3]."/".$filePathStr[4]."/".$filePathStr[5].".zip";
			expDebug::dPrint("filepath strr::".$filePath,4);
		}
	}
	try
	{
		$fileDt = new stdClass();
		$fileinfo =array();
	if($basetype == 'VOD'){
			if($_REQUEST['mobile_request']){
				$filePath = $filePath;
			}else{
				$filePath = $filePath . '.' . 'zip';
			}
			
		}
		if(strpos($filePath,"sites/default")>0)
		{
			$exploadStr = 'sites/default/';
			$path = explode($exploadStr, $filePath);
			$fileloc = $exploadStr.$path[1];

			$isFile = file_exists($fileloc);
			$fileType = filetype($fileloc);
			if($isFile){
				$accessTime = date("F d Y H:i:s T", fileatime($fileloc));
				$modifyTime = date("F d Y H:i:s T", filemtime($fileloc));
				$fileSize = filesize($fileloc);
				$fileDt->filesize= $fileSize;
				$fileDt->lastaccess= $accessTime;
				$fileDt->lastmodified= $modifyTime;
			}
			else{
				$fileDt->filesize = '';
				$fileDt->lastaccess = '';
				$fileDt->lastmodified = '';
			}
		}
		else
		{
			$fileDt->filesize = '';
			$fileDt->lastaccess = '';
			$fileDt->lastmodified = '';
		}
		$fileinfo = $fileDt;
		return $fileinfo;
	}catch(Exception $ex)
	{
		watchdog_exception('getUploadFileInfo', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getDirectReporteeIds() - Returns in an array slt_person ids of the direct reportees of a manager ($managerId)
 */
function getDirectReporteeIds($managerId) {
	try{
		expDebug::dPrint(' getDirectReporteeIds() called for $managerId = ' . print_r($managerId, true),4);
		$reporteesIds = array();
		// Select the table
		$select = db_select('slt_person', 'per');
		$select->leftJoin('slt_person_other_manager', 'per_oth_mng', 'per_oth_mng.user_id=per.id');
		// Add fields to fetch
		$select->addField('per', 'id', 'id');
		// Add condition
		$select->condition(db_or()->condition('per.manager_id', $managerId)
		->condition('per_oth_mng.manager_id',$managerId));

		expDebug::dPrintDBAPI(' $select SQL = ' , $select);
		$reporteesList = $select->execute()->fetchAll();
		expDebug::dPrint(' $reporteesList = '. print_r($reporteesList, true),3);
		foreach ($reporteesList as $reportee) {
			$reporteesIds[] = $reportee->id;
		}
		expDebug::dPrint(' $reporteesIds = '. print_r($reporteesIds, true),3);
		return $reporteesIds;
	}catch (Exception $ex) {
		watchdog_exception('getDirectReporteeIds', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getAllDirectIndirectReporteesIds() - Returns in an array slt_person ids of all the direct and indirect reportees of a manager ($managerId)
 */
function getAllDirectIndirectReporteesIds($managerId) {
	try{
		expDebug::dPrint(' getAllDirectIndirectReporteesIds() called for $managerId = '. print_r($managerId, true),4);

		$directReporteesIds = getDirectReporteeIds($managerId); // Get direct reportees of this manager
		expDebug::dPrint(' getDirectReporteeIds() returned $directReporteesIds = '. print_r($directReporteesIds, true),4);

		$allReporteesIds = $directReporteesIds; // Append direct reportees to $allReporteesIds

//	59995: System hangs when trying to save an edit to a user account (Administer tab > User)
// commented becase of we only check direct and indirect reportee details in one level only not recursive.
// 			// For each immediate reportee fetch all direct and indirect reportees, and add to the $descendants list.
// 			foreach ($directReporteesIds as $reporteeId) {
// 				$allReporteesIds = array_merge($allReporteesIds, getAllDirectIndirectReporteesIds($reporteeId));
// 			}

		// Return the descendants list
		expDebug::dPrint(' getAllDirectIndirectReporteesIds() returned $allReporteesIds = '. print_r($allReporteesIds, true),4);
		return $allReporteesIds;
	}
	catch (Exception $ex) {
		watchdog_exception('getAllDirectIndirectReporteesIds', $ex);
		expertusErrorThrow($ex);
	}
}

function insILTAttendance($userId,$sessionid,$instructor_feedback,$status,$score,$reached,$regid,$instructor_id,$latitude,$longitude,$altitude,$learnerid){
	try{
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		expDebug::dPrint("insturctor_id:".$instructor_id,4);
		if($instructor_id != null && $instructor_id != "null" && $instructor_id != "")
		{
			expDebug::dPrint("insturctor_id:".$instructor_id,4);
			$txn = db_transaction();
			try {
				$insLog  = db_insert('slt_mobile_ilt_attendance');
				$insLog->fields(array(
                      'learnerid' => 0,
                      'learnername' => "",
                      'instructor_id' => $instructor_id,
                      'sessionid' => $sessionid,
                      'instructor_feedback' => $instructor_feedback,
                      'reached' => $reached,
                      'latitude' => $latitude,
                      'longitude' => $longitude,
                      'altitude' => $altitude,
                      'status' => 'Started'
                      ));
                      $logId = $insLog->execute();
                      return array((object)array('id'=>$logId,'instructor_id'=>$instructor_id));
			}
			catch (Exception $ex) {
				expDebug::dPrint("insILT error:".$ex,4);
				$txn->rollback();
				unset($txn);
				watchdog_exception('orgadmin', $ex);
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "Error in inserting ILT Attendance";
				return $errobj;
				//return array((object)array('id'=>"failure"));
			}
			unset($txn);
		}
		else //for participants
		{
			$sel = db_select('slt_person','person');
			$sel->addField('person','full_name');
			$sel->condition('person.id',$learnerid);
			$full_name =  $sel->execute()->fetchField();
			$txn = db_transaction();

			$drupalUid = getDrupalIdOfUser($learnerid);
			$account = user_load($drupalUid);
			$filepath = $account->picture->filename;
			$path= '';
			if($filepath){
				$config           = getConfig("exp_sp");
				$configSiteUrl    = $config["admin_site_url"];
				$path = $configSiteUrl . '/sites/default/files/pictures/'. $filepath;
			}
			try {
				$inscheck = db_select('slt_mobile_ilt_attendance','milt');
				$inscheck->addField('milt','id','id');
				$inscheck->condition('milt.learnerid',$learnerid,'=');
				$inscheck->condition('milt.sessionid',$sessionid,'=');
				$logId=$inscheck->execute()->fetchField();
				expDebug::dPrintDBAPI("Mobile attendance qry :",$inscheck);
				expDebug::dPrint("Mobile attendance qry :".$logId,3);
				if(empty($logId) && $logId==''){
					$insLog  = db_insert('slt_mobile_ilt_attendance');
					$insLog->fields(array(
	                      'learnerid' => $learnerid,
	                      'learnername' => $full_name,
	                      'sessionid' => $sessionid,
	                      'instructor_feedback' => $instructor_feedback,
	                      'reached' => $reached,
	                      'status' => $status,
	                      'score' => $score,
	                      'regid' => $regid,
	         			  			'latitude' => $latitude,
	                      'longitude' => $longitude,
	                      'altitude' => $altitude,
	                      'photo_path' => $path

					));
					$logId = $insLog->execute();
				}
				return array((object)array('id'=>$logId,'learnerid'=>$learnerid,'learnername'=>$full_name, 'sessionid' => $sessionid,'instructor_feedback'=>$instructor_feedback,'reached'=>$reached,'status'=>$status,'score'=>$score, 'regid' => $regid));
			}
			catch (Exception $ex) {
				expDebug::dPrint("insILT error:".$ex,4);
				$txn->rollback();
				unset($txn);
				watchdog_exception('orgadmin', $ex);
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "Error in inserting ILT Attendance";
				return $errobj;
				//return array((object)array('id'=>"failure"));
			}
			unset($txn);
		}
	}catch (Exception $ex) {
		watchdog_exception('insILTAttendance', $ex);
		expertusErrorThrow($ex);
	}
}

function updILTAttendance($action,$id,$userId,$full_name,$sessionid,$instructor_feedback,$status,$score,$reached){
	$txn = db_transaction();
	try {
		$updLog  = db_update('slt_mobile_ilt_attendance');
		if($action != 'Update Status'){
			$updLog->fields(array(
                        'learnerid' => $userId,
                        'learnername' => $full_name,
                        'sessionid' => $sessionid,
                        'instructor_feedback' => $instructor_feedback,
                        'reached' => $reached,
                        'status' => $status,
                        'score' => $score

			));
			$updLog->condition('id', $id);
		}
		else{
			if($status == "Completed"){
				$updLog->fields(array(
                            'status' => $status,
                      'session_completed' => 1
				));
			}else{
				$updLog->fields(array(
                            'status' => $status
				));
			}
			$updLog->condition('sessionid', $sessionid);
		}
		$logId = $updLog->execute();
		if($status == "Completed")
		{
			$select = db_select('slt_mobile_ilt_attendance', 'att');
			$select->addField('att', 'regid', 'regid');
			$select->addField('att', 'score', 'score');
			$select->condition('sessionid', $sessionid);
			$result = $select->execute()->fetchAll();
			foreach($result as $resultEnr){
				$updScore  = db_update('slt_enrollment');
				$updScore->fields(array(
                        'comp_status' => 'lrn_crs_cmp_cmp',
                        'comp_date'   => now(),
                        'score' => $resultEnr->score

				));
				$updScore->condition('id', $resultEnr->regid);
				$logId = $updScore->execute();
			}
		}
		return array((object)array('id'=>$logId));
	}
	catch (Exception $ex) {
		$txn->rollback();
		unset($txn);
		watchdog_exception('orgadmin', $ex);
		$errobj=new stdClass();
		$errobj->isValidateError = 1;
		$errobj->errcode = 'L_012';
		$errobj->errormsg = "Error in updating ILT Attendance";
		return $errobj;
		//return array((object)array('id'=>"failure"));
	}
	unset($txn);
}

function listILTAttendance($userid, $sessionId,$id,$learnerid,$action,$instructor_id)
{
	try{
		expDebug::dPrint('$action :'.$action,4);
		$select = db_select('slt_mobile_ilt_attendance', 'att');
		if($action!=null && $action == "GetInstructorData")
		{
			$select->addField('att', 'id', 'id');
			$select->addField('att', 'sessionid', 'sessionid');
			$select->addField('att', 'reached', 'reached');
			$select->addField('att', 'latitude', 'latitude');
			$select->addField('att', 'longitude', 'longitude');
			$select->addField('att', 'altitude', 'altitude');
			if($sessionId != ""){
				$select->condition('att.sessionid', $sessionId);
			}
			if($instructor_id != ""){
				$select->condition('att.instructor_id', $instructor_id);
			}
			expDebug::dPrintDBAPI('$select', $select);
			$result = $select->execute()->fetchAll();
		}
		else if($action == "ListParticipants")
		{
			$qry =  " select job_title.name 'job_title',matt.id 'id',matt.learnerid 'learnerid' ,matt.learnername 'learnername',
  	  		   matt.sessionid 'sessionid' ,matt.instructor_feedback 'instructor_feedback',matt.status 'status',matt.score 'score',matt.reached 'reached',matt.regid 'regid',
  	  		matt.photo_path 'photo_path' from
  	  slt_mobile_ilt_attendance matt
  	  LEFT JOIN slt_person per ON per.id=matt.learnerid
  	  LEFT JOIN slt_profile_list_items job_title ON per.job_title = job_title.code where 1=1 ";
			if($sessionId != ""){
				$qry.= " and matt.sessionid = ".$sessionId;
			}
			if($learnerid && trim($learnerid) != ""){
				$qry.= " and matt.learnerid = ".$learnerid;
			}
			elseif($id  && $id != ""){
				$qry.= " and matt.id = ".$id;
			}

			$qry.= " order by matt.sessionid desc";
			$qry_res  = db_query($qry);
			expDebug::dPrintDBAPI('$qry_res', $qry_res);
			$result  = $qry_res->fetchAll();
		}else if($action == "ListParticipantsforCE"){
			
			
// 			$qry =  "select mobexc.id request_from,mob1exc.id request_to,job_title.name 'job_title',matt.id 'id',matt.learnerid 'learnerid' ,matt.learnername 'learnername',
// 			matt.sessionid 'sessionid' ,matt.instructor_feedback 'instructor_feedback',matt.status 'status',matt.score 'score',matt.reached 'reached',matt.regid 'regid',
// 			matt.photo_path 'photo_path' from
// 			slt_mobile_ilt_attendance matt
// 			LEFT JOIN slt_person per ON per.id=matt.learnerid
// 			LEFT JOIN slt_mobile_exchange_contacts  mobexc ON mobexc.session_attended_id = matt.sessionid and mobexc.contact_needed_of = ".$userid." and mobexc.participant_id = matt.learnerid and mobexc.status = 'CONTACT_REQUESTED'
// 			LEFT JOIN slt_mobile_exchange_contacts  mob1exc ON mob1exc.session_attended_id = matt.sessionid and mob1exc.participant_id = ".$userid." and mob1exc.contact_needed_of = matt.learnerid and mob1exc.status = 'CONTACT_REQUESTED'
// 			LEFT JOIN slt_profile_list_items job_title ON per.job_title = job_title.code where 1=1 ";
					
			
			$qry =  "select mob2exc.id 'approved',mobexc.id 'exchange_id',per.phone_no 'phone_no',per.email 'email',per.mobile_no 'mobile_no',mobexc.id request_from,mob1exc.id request_to,job_title.name 'job_title',matt.id 'id',matt.learnerid 'learnerid' ,matt.learnername 'learnername',
			matt.sessionid 'sessionid' ,matt.instructor_feedback 'instructor_feedback',matt.status 'status',matt.score 'score',matt.reached 'reached',matt.regid 'regid',
			matt.photo_path 'photo_path' from
			slt_mobile_ilt_attendance matt
			LEFT JOIN slt_person per ON per.id=matt.learnerid
			LEFT JOIN slt_mobile_exchange_contacts  mobexc ON mobexc.session_attended_id = matt.sessionid and mobexc.contact_needed_of = ".$userid." and mobexc.participant_id = matt.learnerid and mobexc.status = 'CONTACT_REQUESTED'
			LEFT JOIN slt_mobile_exchange_contacts  mob1exc ON mob1exc.session_attended_id = matt.sessionid and mob1exc.participant_id = ".$userid." and mob1exc.contact_needed_of = matt.learnerid and mob1exc.status = 'CONTACT_REQUESTED'
  	  		LEFT JOIN slt_mobile_exchange_contacts  mob2exc ON mob2exc.session_attended_id = matt.sessionid and ((mob2exc.participant_id = matt.learnerid and mob2exc.contact_needed_of = ".$userid.")  or (mob2exc.participant_id = ".$userid." and mob2exc.contact_needed_of = matt.learnerid) ) and (mob2exc.status = 'CONTACT_APPROVED' || mob2exc.status = 'CONTACT_STORED' )
			LEFT JOIN slt_profile_list_items job_title ON per.job_title = job_title.code where 1=1 ";
				
			
			if($sessionId != ""){
				$qry.= " and matt.sessionid = ".$sessionId;
			}
			if($learnerid && trim($learnerid) != ""){
				$qry.= " and matt.learnerid = ".$learnerid;
			}
			elseif($id  && $id != ""){
				$qry.= " and matt.id = ".$id;
			}
			
			$qry.= " group by matt.learnerid order by matt.sessionid desc";
			
			expDebug::dPrint('qryyyyyy '.$qry);
			$qry_res  = db_query($qry);
			expDebug::dPrintDBAPI('$qry_res', $qry_res);
			$result  = $qry_res->fetchAll();
				
			
			
		}

		return $result;
	}catch (Exception $ex) {
		watchdog_exception('listILTAttendance', $ex);
		expertusErrorThrow($ex);
	}
}
/**
 * Package and send the result of a page callback to the browser as HTML.
 *
 * @param $page_callback_result
 *   The result of a page callback. Can be one of:
 *   - NULL: to indicate no content.
 *   - An integer menu status constant: to indicate an error condition.
 *   - A string of HTML content.
 *   - A renderable array of content.
 *
 * @see drupal_deliver_page()
 */
function expertus_deliver_html_page($page_callback_result,$message = '') {
	// Emit the correct charset HTTP header, but not if the page callback
	// result is NULL, since that likely indicates that it printed something
	// in which case, no further headers may be sent, and not if code running
	// for this page request has already set the content type header.
	try{
		if (isset($page_callback_result) && is_null(drupal_get_http_header('Content-Type'))) {
			drupal_add_http_header('Content-Type', 'text/html; charset=utf-8');
		}
		if (isset($page_callback_result)) {
			print drupal_render_page($message);
		}
		// Perform end-of-request tasks.
		drupal_page_footer();
	}catch (Exception $ex) {
		watchdog_exception('expertus_deliver_html_page', $ex);
		expertusErrorThrow($ex);
	}
}
/*
 * Get site default Languages
 */
function getSiteDefaultLanguageCode(){
	try{
	 $drupalDefaultLanguage = $GLOBALS['language']->language;
	 if($drupalDefaultLanguage){
		 $siteLang = db_select('slt_profile_list_items','prof');
		 $siteLang->addField('prof','code');
		 $siteLang->condition('attr1', $drupalDefaultLanguage);
		 $siteLang->condition('code', db_like('cre_sys_lng') . '%', 'LIKE');
		 $siteLangName = $siteLang->execute()->fetchField();
		 return $siteLangName;
	 }
	 return '';
	}catch (Exception $ex) {
		watchdog_exception('getSiteDefaultLanguageCode', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * Get list of Currency in slt_profile_list_items
*Using this same function for exp_sp_administration_user.inc
* * Added by simbu Sep 25, 2015
*/
function getSiteCurrencyCode($code=NULL){
	try{
		 $siteCurrency = db_select('slt_profile_list_items','prof');
		 $siteCurrency->addField('prof','name','currency_name');
		 $siteCurrency->addField('prof','attr1','currency_code');
		 $siteCurrency->addField('prof','attr2','currency_symbol');
		 $siteCurrency->condition('prof.code','cre_sys_crn_%','LIKE');
		 $siteCurrency->condition('prof.attr3', 'Y');
		 $siteCurrency->condition('prof.is_active', 'Y');
		 if($code!=''){
		 	$siteCurrency->condition('attr1', $code);
		 }
		 $siteCurrency->orderBy('attr1', 'ASC');
		 $siteCurrencyName = $siteCurrency->execute()->fetchAll();
		 return $siteCurrencyName;
	}catch (Exception $ex) {
		watchdog_exception('getSiteCurrencyCode', $ex);
		expertusErrorThrow($ex);
	}
}

function getDefaultSiteCurrencyCode($userid){
	try{
		$siteCurrency = db_select('slt_person','slp');
		$siteCurrency->addField('slp','preferred_currency');
		$siteCurrency->condition('id', $userid);
		expDebug::dPrintDBAPI('getManagedByFilters query',$siteCurrency);
		$siteCurrencyName = $siteCurrency->execute()->fetchField();
		return $siteCurrencyName;
	}catch (Exception $ex) {
		watchdog_exception('getDefaultSiteCurrencyCode', $ex);
		expertusErrorThrow($ex);
	}
}

function trainingProgramMultiClassConflictCheck($listIds) {
	try{
		expDebug::dPrint(' enrClassSession date is different - skip'.count($listIds),3);
		$countClass = count($listIds);
		$storeClass = array();
		$conflictStatus = 0;
		for($i=0;$i<$countClass;$i++){
			$deliveryType = getClassDeliveryType($listIds[$i], 'Short');
			expDebug::dPrint(' trainingProgramMultiClassConflictCheck $deliveryType = ' . print_r($deliveryType, true) , 5);
			if ($deliveryType != 'wbt' && $deliveryType != 'vod') {
				$classSessionsDateTimeList = getClassSessionsDateTime($listIds[$i]);
				for($j=$i+1;$j<$countClass;$j++){
					$enrClassSessionsDateTimeList = getClassSessionsDateTime($listIds[$j]);
					foreach ($classSessionsDateTimeList as $classSessionsDateTime) {
						foreach ($enrClassSessionsDateTimeList as $enrClassSessionsDateTime) {
							// If the classSession is on a different date than enrClassSession, there is no conflict
							if ($classSessionsDateTime['date'] != $enrClassSessionsDateTime['date']) {
								expDebug::dPrint(' trainingProgramMultiClassConflictCheck date is different - skip');
								continue; // There is no conflict with this enrolled class session. Skip to the next enrolled class session.
							}

							// classSession is on the same date as enrClassSession
							// If the classSession's start_time - end_time is outside enrClassSession's start_time - end_time, there is no
							// conflict
							// If classSession start_time >= enrClassSession end_time, there is no conflict.
							// If classSession end_time <= enrClassSession start_time, there is no conflict
							if ($classSessionsDateTime['start_time'] >= $enrClassSessionsDateTime['end_time'] ||
							$classSessionsDateTime['end_time'] <= $enrClassSessionsDateTime['start_time']) {
								expDebug::dPrint(' trainingProgramMultiClassConflictCheckclassSession\'s start_time - end_time is outside enrClassSession\'s start_time - end_time - skip');
								continue; // There is no conflict with this enrolled class session. Skip to the next enrolled class session.
							}

							// A conflict is found. Save conflicting classId in $conflictStatus exit all loops
							$conflictStatus = $listIds[$i];
							throw new Exception('Conflict');
						} // end foreach ($enrClassSessionsDateTimeList as $enrClassSessionsDateTime)
					}
					//throw new Exception('Conflict');
				}
			}
		}
	}catch (Exception $ex) {
		// Only handle Conflict exceptions
		if ($ex->getMessage() == null || $ex->getMessage() != 'Conflict') {
			throw $ex; // Rethrow all other exceptions.
		}
		expDebug::dPrint(' In exception handler with $conflictStatus = ' . print_r($conflictStatus, true) , 4);
		expDebug::dPrint(' In exception handler with $conflictStatus = ' . print_r($ex->getMessage(), true) , 4);
	}
	return $conflictStatus;
}

function insertEnrollmentContentMapping($enrollContentObj,$from,&$statements=array()){
	// Modified by Vincent on June 24, 2015 for #0048488
	// If the call from auto register then the transacton should not be set
	// because already set in previous method otherwise transaction is required
	if(empty($statements)){
		$is_executable = true;
		$statements = array('statement' => true);
	}else if(!empty($statements)) {
		//$txn1 = ($from == 'AutoRegister')? new stdClass() : db_transaction();
	}
	try{
		$contents = getContentAndVersionForClass($enrollContentObj->class_id);
		$loggedUser = (from == 'lp_register') ? $enrollContentObj->ctuser : getSltpersonUserId();
		foreach ($contents as $content){
			expDebug::dPrint('content version id for cart  '.$content->content_id.' --- '.$content->version_id);
			$enrollContentStmt = db_insert('slt_enrollment_content_mapping');
			$enrollContentStmt->fields(array(
		  	'enroll_id' => $enrollContentObj->enroll_id,
		    'user_id' => $enrollContentObj->user_id,
		    'course_id' => $enrollContentObj->course_id,
		    'class_id' => $enrollContentObj->class_id,
		    'content_id' => $content->content_id,
		  	'version_id' => $content->version_id,
		    'created_by' => $loggedUser,
		    'created_on' => now(),
		  	'updated_by' => $loggedUser,
		  	'updated_on' => now(),
			));
			if(!empty($statements)) {
				$statements[] = $enrollContentStmt;
			} else {
				$enrollContentStmt->execute();
			}
			//$enrollContentStmt->execute();
			if($is_executable==true){
				expDebug::dPrint('insertEnrollmentContentMapping-->statements');
				dbStatementsExecution($statements);
			}
		}
	}catch (Exception $ex) {
		//if($from != 'AutoRegister')
	    //$txn1->rollback();
		watchdog_exception('insertEnrollmentContentMapping', $ex);
		if (isset($_GET['cron_key'])) {
			throw new Exception($ex);
		}
		throw $ex;
	}
  //	unset($txn1); // Commit txn
}

function removeEnrollmentContentMapping($enrollId){
	try{
	   
	    if($form_state['values']['create_user_id'])
	        $loggedInUserId = $form_state['values']['create_user_id'];
	    else
	        $loggedInUserId  = getIdOfLoggedInUser();
	    
	    $delete_id = db_select('slt_enrollment_content_mapping', 'ecm');
        $delete_id->addField('ecm', 'id', 'id');
        $delete_id->addField('ecm', 'content_id', 'content_id');
        $delete_id->addField('ecm', 'user_id', 'user_id');
        $delete_id->addField('ecm', 'course_id', 'course_id');
        $delete_id->addField('ecm', 'class_id', 'class_id');
        $delete_id->addField('ecm', 'version_id', 'version_id');
        
       $delete_id->condition('enroll_id', $enrollId,'=') ;
       
       $del=$delete_id->execute()->fetchAll();
	    expDebug::dPrintDBAPI('$delete_id', $del);
	    	    
		$deleteStmt = db_delete('slt_enrollment_content_mapping');
		$deleteStmt->condition('enroll_id', $enrollId);
		$deleteStmt->execute();
		expDebug::dPrintDBAPI('$deleteStmt', $deleteStmt);
		
		//Make delete log entries
		foreach ($del as $val){
		    $fields = array(
		            'table_name' => 'slt_enrollment_content_mapping',
		            'entity_id' => $val->id,
		            'entity_type' => 'content',
		            'module_name' => 'exp_sp_core',
		            'functionality_name' => 'removeEnrollmentContentMapping',
		            'api_name' => null,
		            'deleted_on' => now(),
		            'deleted_by' => $loggedInUserId,
		            'parent1_entity_id' => $enrollId,
		            'parent1_entity_type' => 'enroll_id',
		            'parent2_entity_id' => $val->content_id,
		            'parent2_entity_type' => 'content_id',
		            'parent3_entity_id' => $val->user_id,
		            'parent3_entity_type' => 'user_id',
		            'parent4_entity_id' => $val->course_id,
		            'parent4_entity_type' => 'course_id',
		            'parent5_entity_id' => $val->class_id,
		            'parent5_entity_type' => 'class_id',
		            'parent6_entity_id' => $val->version_id,
		            'parent6_entity_type' => 'version_id'
		    );
		    deleted_log_entry($fields);
		}expDebug::dPrint('slt_enrollment_content_mapping table log');
		
	}catch (Exception $ex) {
		watchdog_exception('removeEnrollmentContentMapping', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Set a message which reflects the status of the performed operation.
 *
 * If the function is called with no arguments, this function returns all set
 * messages without clearing them.
 *
 * @param $message
 *   The message to be displayed to the user. For consistency with other
 *   messages, it should begin with a capital letter and end with a period.
 * @param $type
 *   The type of the message. One of the following values are possible:
 *   - 'status'
 *   - 'warning'
 *   - 'error'
 * @param $repeat
 *   If this is FALSE and the message is already set, then the message won't
 *   be repeated.
 */
function expertus_set_message($message = NULL, $type = 'status', $repeat = TRUE){
	try{
		$config = getConfigValue('show_messages');
		if(!empty($config)){
			drupal_set_message($message,$type,$repeat);
		}
	}catch (Exception $ex) {
		watchdog_exception('expertus_set_message', $ex);
		expertusErrorThrow($ex);
	}
}

/* Function to remove a folder and its contents
 * php supports rmdir only for empty folders
 * For folders with contents, the files need to be unlinked first and then the directory deleted.
 * The process is recursive when the folder to be removed is not empty
 * @param $filePath - filename with path
 */
function removedir($filePath){
	try {
		if (is_dir($filePath))
		$dir_handle = opendir($filePath);
		if (!$dir_handle)
		return false;
		while($file = readdir($dir_handle)) {
			if ($file != "." && $file != "..") {
				if (!is_dir($filePath."/".$file))
				unlink($filePath."/".$file);
				else
				{
					$a=$filePath.'/'.$file;
					removedir($a);
				}
			}
		}
		closedir($dir_handle);
		rmdir($filePath);
	}catch (Exception $ex) {
		watchdog_exception('removedir', $ex);
		expertusErrorThrow($ex);
	}
}

/* Function to get the size of the uploaded zip file
 * @param - file size in bytes
 */
function getUploadedFileSize($bytes)
{
	try{
		if ($bytes >= 1073741824){
			$bytes = round(number_format($bytes / 1073741824, 2)) . ' GB';
		}elseif ($bytes >= 1048576){
			$bytes = round(number_format($bytes / 1048576, 2)) . ' MB';
		}elseif ($bytes > 1024 ){
			$bytes = round(number_format($bytes / 1024, 2)) . ' KB';
		}else{
			$bytes = '1 KB';
		}
		return $bytes;
	}catch (Exception $ex) {
		watchdog_exception('getUploadedFileSize', $ex);
		expertusErrorThrow($ex);
	}
}
/**
 *  Function to get the managed by options from profile list
 *
 */
function getManagedByFilters(){
	try{
		$filter = cache_get('managed_filter')->data;
		if($filter)
			return $filter;
		$select = db_select('slt_profile_list_items','spli');
		$select->addField('spli','code','code');
		$select->addField('spli','name','name');
		$select->condition('spli.code','cre_sys_fop_%','LIKE');
		expDebug::dPrintDBAPI('getManagedByFilters query',$select);
		$result = $select->execute()->fetchAllKeyed();
		cache_set('managed_filter',$result);
		return $result;
	}catch (Exception $ex) {
		watchdog_exception('insertEnrollmentContentMapping', $ex);
		throw $ex;
	}
}


/**
 *  Function to get the managed by options from profile list
 *
			 */
function getCurrencyFilters($type=""){
	try{
		$select = db_select('slt_profile_list_items','spli');
		// Commented for #0056451
		/*if($type == "TP")
		{
			$select->join('slt_program', 'slp', 'slp.currency_type = spli.attr1');
			$select->condition('slp.status',array('lrn_lpn_sts_del','lrn_lpn_sts_dft'),'NOT IN');
		}
		elseif($type == "Order")
		{
			$select->join('slt_order_items', 'sloi', 'sloi.currency_type = spli.attr1');
		}
		else
		{
			$select->join('slt_course_class', 'cls', 'cls.currency_type = spli.attr1');
			$select->condition('cls.status',array('lrn_cls_sts_del','lrn_cls_sts_dft'),'NOT IN');
		}*/
		$select->addField('spli','attr1','code');
		$select->addField('spli','name','name');
		$select->condition('spli.code','cre_sys_crn_%','LIKE');
		$select->condition('spli.attr3','Y','=');
		$select->condition('spli.is_active','Y','=');

		$select->orderBy('spli.name');
		expDebug::dPrintDBAPI('getCurrencyFilters query',$select);
		$result = $select->execute()->fetchAllKeyed();
		//cache_set('currency_filter',$result);
		return $result;
	}catch (Exception $ex) {
		watchdog_exception('insertEnrollmentContentMapping', $ex);
		throw $ex;
	}
}

/* common function to insert records into slt_notification */

function callCommonNotificationInsert($notificationType, $notificationInfo, $notificationTemplateInfo, $userId, $overrideContent, $iCal = null,$notificationVol='regular') {
	try {
		expDebug::dPrint('$notificationType = ' . print_r($notificationType, true), 4);
		expDebug::dPrint('$notificationInfo = ' . print_r($notificationInfo, true), 4);
		expDebug::dPrint('$notificationTemplateInfo = ' . print_r($notificationTemplateInfo, true), 4);
		expDebug::dPrint('$userId = ' . print_r($userId, true), 4);
		expDebug::dPrint('$overrideContent = ' . print_r($overrideContent, true), 4);

		$notifyFlag = null;
		$emailSendTo = null;
		$ccEmail = null;
		if ($notificationTemplateInfo != null) {
			$notifyFlag = $notificationTemplateInfo['status'];
			$emailSendTo = $notificationTemplateInfo['notification_sendto'];
			$ccEmail = $notificationTemplateInfo['notification_cc'];
			$adminEmail = $notificationTemplateInfo['notification_admin'];


			/*0040211: Need support to have CC in notifications to managers/admin
			 * Start
			 */
			module_load_include('inc', 'exp_sp_learning', 'exp_sp_learning');
			//       if ($notificationInfo['ccopy']=='true' && $notificationInfo['sendival'] == 0) {
			//         $sendStatus = 1;
			//       }
			if(!empty($ccEmail))
			$mailIdWithCc=  $emailSendTo.",".$ccEmail;
			else
			$mailIdWithCc=  $emailSendTo;
			expDebug::dPrint(' $mailIdWithCc = ' . print_r($mailIdWithCc, true) , 4);
			$drupalUser = getDrupalUserForNotificationInsert($userId,'','');
			$explodeMailIdWithCc = explode(",", $mailIdWithCc );
			expDebug::dPrint('$explodeMailIdWithCc =' . print_r($explodeMailIdWithCc, true) , 4);
			if((count($explodeMailIdWithCc) > 1) &&(in_array("cre_ntn_rpt_usr", $explodeMailIdWithCc))) {
				$ccValue = getSendToMailCc($mailIdWithCc, $drupalUser);
				expDebug::dPrint('$ccValue for learner = ' . print_r($ccValue, true) , 4);
			}
			elseif ((count($explodeMailIdWithCc) > 1) &&(!in_array("cre_ntn_rpt_usr", $explodeMailIdWithCc))) {
				$removedFirstOne = $explodeMailIdWithCc;
				expDebug::dPrint('$ccValue for other than learner= ' . print_r($removedFirstOne, true) , 4);
				$removedFirstValue = array_shift($removedFirstOne);

				expDebug::dPrint('$ccValue for other than learner= ' . print_r($removedFirstOne, true) , 4);
				$ccValue = getSendToMailCc(implode(',',$removedFirstOne), $drupalUser);
				expDebug::dPrint('$ccValue for other than learner= ' . print_r($ccValue, true) , 4);
			}


		}
		$setLanguage = 'cre_sys_lng_eng';
		$currentUserId = getIdOfLoggedInUser();
		if($notificationType == 'sendMobileAppLink'){
			$sendToEmail = $objectId2;
			$remark = '';
			$insertStmt1 = db_insert('slt_notification');
			$insertStmt1->fields(array(
          'msg_id' => $notificationInfo['message_id'],
          'lang_code' => $setLanguage,
          'msg_type' => $notificationInfo['message_type'],
          'token_str' => $notificationInfo['tokens_string'],
          'user_id' => NULL,
          'send_to_id' => NULL,
          'send_to_name' => '',
          'send_to_email' => $sendToEmail,
		  'send_to_email_cc' => (empty($ccValue)) ? NULL : $ccValue,
          'send_type' => $notificationInfo['send_type'],
          'send_status' => 'N',
          'attach_content' => $overrideContent,
          'remarks' => $remark,
          'created_by' => $currentUserId,
          'created_on' => now(),
          'updated_by' => $currentUserId,
          'updated_on' => now()
			));
			$userNotificationId = $insertStmt1->execute();
			expDebug::dPrint(' $userNotificationId = ' . print_r($userNotificationId, true) , 3);
			return $userNotificationId;
		}
		$drupalUser = getDrupalUserForNotificationInsert($userId,'','');
		expDebug::dPrint(' $drupalUser = ' . print_r($drupalUser, true) , 4);
		$user_preffered_language = $drupalUser['preferred_language'];
		expDebug::dPrint(' $user_preffered_language = ' .$user_preffered_language .  '--- message id' .$notificationInfo['message_id'] , 4);
		$isPreferredNotification = 0 ;
		if(!empty($user_preffered_language) && $user_preffered_language != $setLanguage){
			$isPreferredNotification  = getPreferredNotification($notificationInfo['message_id'],$user_preffered_language);
			expDebug::dPrint(' $$isPreferredNotification = ' . $isPreferredNotification , 4);
			if($isPreferredNotification){
				$setLanguage = $user_preffered_language;
			}
		}
		if($isPreferredNotification == 0 ){
			expDebug::dPrint(' notification language = ' . $notificationInfo['lang_code'] , 4);
			if(!empty($notificationInfo['lang_code']) && $notificationInfo['lang_code'] != $setLanguage){
				$isNotificationLanguage  = getPreferredNotification($notificationInfo['message_id'],$notificationInfo['lang_code']);
				expDebug::dPrint(' $$$isNotificationLanguage = ' . $isNotificationLanguage , 4);
				if($isNotificationLanguage){
					$setLanguage = $notificationInfo['lang_code'];
				}
			}
		}
		$nullValue = NULL;
		$emailSendTo = explode(',', $emailSendTo);
		if ((in_array("cre_ntn_rpt_adm", $emailSendTo)) && (count($explodeMailIdWithCc) == 1)) {
			$customAdmin = 1;
			$insertStmt1 = db_insert('slt_notification');
			$insertStmt1->fields(array(
          'msg_id' => $notificationInfo['message_id'],
          'lang_code' => 'cre_sys_lng_eng',
          'msg_type' => $notificationInfo['message_type'],
          'token_str' => $notificationInfo['tokens_string'],
          'user_id' => $userId,
          'send_to_id' => $nullValue,
          'send_to_name' => $nullValue,
          'send_to_email' => $nullValue,
		  'send_to_email_cc' => (empty($ccValue)) ? NULL : $ccValue,
          'send_type' => $notificationInfo['send_type'],
          'send_status' => 'N',
          'attach_content' => $overrideContent,
          'remarks' => $nullValue,
          'created_by' => $updateUserId,
          'created_on' => now(),
          'updated_by' => $updateUserId,
          'updated_on' => now(),
          'custom0'    => $customAdmin
			));
			$userNotificationId = $insertStmt1->execute();
			expDebug::dPrint(' $userNotificationId = ' . print_r($userNotificationId, true) , 3);
		}
		$sendStatus = 0;
		if($drupalUser['user_status'] == 'cre_usr_sts_atv'){
			$sendStatus = 1;
		}
		if($notificationType == 'SuspendUser'){
			$sendStatus =1;
		}
		if($notificationType == 'share_notification_class_tp' || $notificationType == 'request_class' || $notificationType == 'share_notification_catalog' || $notificationType == 'share_notification_h5p') {
			$sendStatus = 0;
			$emailSendTo = array('cre_ntn_rpt_usr');
			//       if ($notificationInfo['ccopy']=='true' && $notificationInfo['sendival'] == 0) {
			//         $sendStatus = 1;
			//       }
		}
		if ((in_array('cre_ntn_rpt_usr', $emailSendTo) && $sendStatus) && (count($explodeMailIdWithCc) == 1)) {
			if($notificationType == 'share_notification_class_tp' || $notificationType == 'share_notification_catalog' || $notificationType == 'share_notification_h5p')
			$sendToEmail = $notificationInfo['referfrom'];
			else
			$sendToEmail = $drupalUser['email'];
			$remark = '';
			$tableName = ($notificationVol=='bulk') ? 'slt_bulk_notification' : 'slt_notification';
		    $insertStmt1 = db_insert($tableName);
			$insertStmt1->fields(array(
          'msg_id' => $notificationInfo['message_id'],
          'lang_code' => $setLanguage,
          'msg_type' => $notificationInfo['message_type'],
          'token_str' => $notificationInfo['tokens_string'],
          'user_id' => $userId,
          'send_to_id' => $drupalUser['id'],
          'send_to_name' => $drupalUser['full_name'],
          'send_to_email' => $sendToEmail,
		  'send_to_email_cc' => (empty($ccValue)) ? NULL : $ccValue,
          'send_type' => $notificationInfo['send_type'],
          'send_status' => 'N',
          'attach_content' => $overrideContent,
          'remarks' => $remark,
          'ical' => $iCal,
          'created_by' => $updateUserId,
          'created_on' => now(),
          'updated_by' => $updateUserId,
          'updated_on' => now()
			));
			$userNotificationId = $insertStmt1->execute();
			expDebug::dPrint(' $userNotificationId = ' . print_r($userNotificationId, true) , 3);
		}
		if ((in_array("cre_ntn_rpt_mgr", $emailSendTo)) && (count($explodeMailIdWithCc) == 1)) {
			$manager_id = $drupalUser['manager_id'];
			if(!empty($manager_id)) {
				$getManagerRes = getDrupalUserForNotificationInsert($manager_id);
				$sendToEmail = $getManagerRes['email'];
				$setLanguage = 'cre_sys_lng_eng';
				$manager_preffered_language = $getManagerRes['preferred_language'];
				expDebug::dPrint(' $$manager_preffered_language = ' .$manager_preffered_language .  '--- message id' .$notificationInfo['message_id'] , 4);
				$isPreferredNotification = 0 ;
				if(!empty($manager_preffered_language) && $manager_preffered_language != $setLanguage){
					$isPreferredNotification  = getPreferredNotification($notificationInfo['message_id'],$manager_preffered_language);
					expDebug::dPrint(' $$isPreferredNotification = ' . $isPreferredNotification , 4);
					if($isPreferredNotification){
						$setLanguage = $manager_preffered_language;
					}
				}
				if($isPreferredNotification == 0 ){
					expDebug::dPrint(' manager notification language = ' . $notificationInfo['lang_code'] , 4);
					if(!empty($notificationInfo['lang_code']) && $notificationInfo['lang_code'] != $setLanguage){
						$isNotificationLanguage  = getPreferredNotification($notificationInfo['message_id'],$notificationInfo['lang_code']);
						expDebug::dPrint(' manager $$$isNotificationLanguage = ' . $isNotificationLanguage , 4);
						if($isNotificationLanguage){
							$setLanguage = $notificationInfo['lang_code'];
						}
					}
				}
				$remark = '';
				$insertStmt1 = db_insert('slt_notification');
				$insertStmt1->fields(array(
            'msg_id' => $notificationInfo['message_id'],
            'lang_code' => $setLanguage,
            'msg_type' => $notificationInfo['message_type'],
            'token_str' => $notificationInfo['tokens_string'],
            'user_id' => $manager_id,
            'send_to_id' => $getManagerRes['id'],
            'send_to_name' => $getManagerRes['full_name'],
            'send_to_email' => $sendToEmail,
			'send_to_email_cc' => (empty($ccValue)) ? NULL : $ccValue,
            'send_type' => $notificationInfo['send_type'],
            'send_status' => 'N',
            'attach_content' => $overrideContent,
            'remarks' => $remark,
            'created_by' => $updateUserId,
            'created_on' => now(),
            'updated_by' => $updateUserId,
            'updated_on' => now()
				));
				$userNotificationId = $insertStmt1->execute();
				expDebug::dPrint(' $userNotificationId = ' . print_r($userNotificationId, true),3);
			}
		}

		if ($notificationType == 'WaitlistLocationAdmin' || $notificationType == 'CancelLocationAdmin') {
			$sendToEmail = getFacilityContactEmail($classLocationId);
			expDebug::dPrint(' LocationAdmin $sendToEmail = ' . print_r($sendToEmail, true) , 4);
			if(!empty($sendToEmail)) {
				$remark = '';
				$insertStmt1 = db_insert('slt_notification');
				$insertStmt1->fields(array(
            'msg_id' => $notificationInfo['message_id'],
            'lang_code' => 'cre_sys_lng_eng',
            'msg_type' => $notificationInfo['message_type'],
            'token_str' => $notificationInfo['tokens_string'],
            'user_id' => $userId,
            'send_to_id' => $drupalUser['id'],
            'send_to_name' => $drupalUser['full_name'],
            'send_to_email' => $sendToEmail,
			'send_to_email_cc' => (empty($ccValue)) ? NULL : $ccValue,
            'send_type' => $notificationInfo['send_type'],
            'send_status' => 'N',
            'attach_content' => $overrideContent,
            'remarks' => $remark,
            'created_by' => $updateUserId,
            'created_on' => now(),
            'updated_by' => $updateUserId,
            'updated_on' => now()
				));
				$userNotificationId = $insertStmt1->execute();
				expDebug::dPrint(' $userNotificationId = ' . print_r($userNotificationId, true) , 3);
			}
		}

		if ($notificationType == 'share_notification_class_tp' || $notificationType == 'request_class' || $notificationType == 'share_notification_catalog' || $notificationType == 'share_notification_h5p') { // multiple catalog share notification added.
			$sendToEmail = $notificationInfo['sendToEmail'];
			if ($notificationInfo['ccopy']=='true' && $notificationInfo['sendival'] == 0){
				if($notificationType == 'request_class'){
					$ccValue = $drupalUser['email'];
				}else{
					$ccValue = $notificationInfo['referfrom'];
				}
			}
			else{
			$ccValue ='';
			}
			if(!empty($ccEmail)){
				if(!empty($ccValue)){
					$ccValue = $ccValue.",".$ccEmail;
				}else{
					$ccValue = $ccEmail;
				}
			}
			expDebug::dPrint('notification $sendToEmail = ' . print_r($sendToEmail, true) , 4);
			$setLanguage = 'cre_sys_lng_eng';

			if(!empty($notificationInfo['userExistinLMS'])){
				$drupalUser = getDrupalUserForNotificationInsert($notificationInfo['userExistinLMS']);
				expDebug::dPrint(' $drupalUser = ' . print_r($drupalUser, true) , 4);
				$user_preffered_language = $drupalUser['preferred_language'];
				expDebug::dPrint(' $user_preffered_language = ' .$user_preffered_language .  '--- message id' .$notificationInfo['message_id'] , 4);
				$isPreferredNotification = 0 ;
				if(!empty($user_preffered_language) && $user_preffered_language != $setLanguage){
					$isPreferredNotification  = getPreferredNotification($notificationInfo['message_id'],$user_preffered_language);
					expDebug::dPrint(' $$isPreferredNotification = ' . $isPreferredNotification , 4);
					if($isPreferredNotification){
						$setLanguage = $user_preffered_language;
					}
				}
				if($isPreferredNotification == 0 ){
					expDebug::dPrint(' notification language = ' . $notificationInfo['lang_code'] , 4);
					if(!empty($notificationInfo['lang_code']) && $notificationInfo['lang_code'] != $setLanguage){
						$isNotificationLanguage  = getPreferredNotification($notificationInfo['message_id'],$notificationInfo['lang_code']);
						expDebug::dPrint(' $$$isNotificationLanguage = ' . $isNotificationLanguage , 4);
						if($isNotificationLanguage){
							$setLanguage = $notificationInfo['lang_code'];
						}
					}
				}
			}
			
			if($notificationType == 'share_notification_h5p') // adding for h5p
			{
				$receiver_id = $notificationInfo['send_to_id'];
				$receivername = $notificationInfo['send_to_name'] ;
				$user_id = $userId;
			}
			else{ 
				$receiver_id = NULL;
				$receivername = NULL;
				$user_id = NULL;
			}
			
			

			if(!empty($sendToEmail)) {
				$remark = '';
				$insertStmt1 = db_insert('slt_notification');
				$insertStmt1->fields(array(
            'msg_id' => $notificationInfo['message_id'],
	      	'lang_code' => $setLanguage,
            'msg_type' => $notificationInfo['message_type'],
            'token_str' => $notificationInfo['tokens_string'],
            'user_id' => $user_id,
            'send_to_id' => $receiver_id,
            'send_to_name' => $receivername,
            'send_to_email' => $sendToEmail,
        		'send_to_email_cc' => $ccValue,
            'send_type' => $notificationInfo['send_type'],
            'send_status' => 'N',
            'attach_content' => $overrideContent,
            'remarks' => $remark,
            'created_by' => $updateUserId,
            'created_on' => now(),
            'updated_by' => $updateUserId,
            'updated_on' => now()
				));
				$userNotificationId = $insertStmt1->execute();
				expDebug::dPrint(' $userNotificationId = ' . print_r($userNotificationId, true) , 3);
			}
		}
		// Notify cc if required
		if ((!empty($ccEmail)) && (count($explodeMailIdWithCc) == 1) && $notificationType != 'request_class') {
			$insertStmt2 = db_insert('slt_notification');
			$insertStmt2->fields(array(
          'msg_id' => $notificationInfo['message_id'],
          'lang_code' => 'cre_sys_lng_eng',
          'msg_type' => $notificationInfo['message_type'],
          'token_str' => $notificationInfo['tokens_string'],
          'user_id' => $userId,
          'send_to_id' => $drupalUser['id'],
          'send_to_name' => $drupalUser['full_name'],
          'send_to_email' => $ccEmail,
		  'send_to_email_cc' => (empty($ccValue)) ? NULL : $ccValue,
          'send_type' => $notificationInfo['send_type'],
          'send_status' => 'N',
          'attach_content' => $overrideContent,
          'remarks' => $remark,
          'created_by' => $updateUserId,
          'created_on' => now(),
          'updated_by' => $updateUserId,
          'updated_on' => now()
			));
			expDebug::dPrintDBAPI('$insertStmt2  =>', $insertStmt1);
			$userNotificationId = $insertStmt2->execute();
		}
		if((count($explodeMailIdWithCc) > 1) &&(in_array("cre_ntn_rpt_usr", $explodeMailIdWithCc)) && $notificationType != 'request_class') {
			// $sendToEmail = $drupalUser['email'];
			$sendToEmail = getEmailId("cre_ntn_rpt_usr", $drupalUser);
			$ccValueExplode = explode(',', $ccValue);
			if(($key = array_search($sendToEmail,$ccValueExplode )) !== false) {
				unset( $ccValueExplode[$key]);
			}
			$ccValue = implode(',', $ccValueExplode);
			$remark = '';
			$insertStmt1 = db_insert('slt_notification');
			$insertStmt1->fields(array(
            'msg_id' => $notificationInfo['message_id'],
            'lang_code' => $setLanguage,
            'msg_type' => $notificationInfo['message_type'],
            'token_str' => $notificationInfo['tokens_string'],
            'user_id' => $userId,
            'send_to_id' => $drupalUser['id'],
            'send_to_name' => $drupalUser['full_name'],
            'send_to_email' => $sendToEmail,
            'send_to_email_cc' => $ccValue,
            'send_type' => $notificationInfo['send_type'],
            'send_status' => 'N',
            'attach_content' => $overrideContent,
            'ical' => $iCal,
            'remarks' => $remark,
            'created_by' => $updateUserId,
            'created_on' => now(),
            'updated_by' => $updateUserId,
            'updated_on' => now()
			));
			expDebug::dPrintDBAPI('$insertStmt1  =>', $insertStmt1);
			$userNotificationId = $insertStmt1->execute();
		}
		elseif ((count($explodeMailIdWithCc) > 1) &&(!in_array("cre_ntn_rpt_usr", $explodeMailIdWithCc)) && $notificationType != 'request_class') {
			$sendToEmail = getEmailId(array_shift(array_values($explodeMailIdWithCc)), $drupalUser);
			$ccValueExplode = explode(',', $ccValue);
			if(($key = array_search($sendToEmail,$ccValueExplode )) !== false) {
				unset( $ccValueExplode[$key]);
			}
			$ccValue = implode(',', $ccValueExplode);
			$remark = '';
			$insertStmt1 = db_insert('slt_notification');
			$insertStmt1->fields(array(
            'msg_id' => $notificationInfo['message_id'],
            'lang_code' => $setLanguage,
            'msg_type' => $notificationInfo['message_type'],
            'token_str' => $notificationInfo['tokens_string'],
            'user_id' => $userId,
            'send_to_id' => $drupalUser['id'],
            'send_to_name' => $drupalUser['full_name'],
            'send_to_email' => $sendToEmail,
            'send_to_email_cc' => $ccValue,
            'send_type' => $notificationInfo['send_type'],
            'send_status' => 'N',
            'attach_content' => $overrideContent,
            'ical' => $iCal,
            'remarks' => $remark,
            'created_by' => $updateUserId,
            'created_on' => now(),
            'updated_by' => $updateUserId,
            'updated_on' => now()
			));


			$adminNotificationId = $insertStmt1->execute();
			expDebug::dPrint('$adminNotificationId = ' . print_r($adminNotificationId, true) , 3);
		}
	}
	catch (Exception $ex) {
		watchdog_exception('callCommonNotificationInsert', $ex);
		if (isset($_GET['cron_key'])) {
			expertusLogError($ex);
		}
		else {
			expertusErrorThrow($ex);
		}
	}
}

function GetEnrollmentSummaryAPI()
{
	try {
		$qry = "SELECT
			    count(enr.comp_status) rec_count,
				count(enr.reg_status) reg_status_count,
			    enr.comp_status statuscode,
			    cls.title title,
		        if(enr.reg_status = 'lrn_crs_reg_cnf', if(enr.comp_status IS NOT NULL,compst.name,'Enrolled') ,regst.name) statusname,
			    enr.reg_status reg_status_code
			FROM slt_enrollment enr
				LEFT JOIN slt_course_class cls ON cls.id=enr.class_id
				AND cls.course_id=enr.course_id
				LEFT JOIN slt_person per ON per.id=enr.user_id
				LEFT JOIN slt_profile_list_items regst ON regst.code = enr.reg_status
				AND regst.lang_code='cre_sys_lng_eng'
				LEFT JOIN slt_profile_list_items compst ON compst.code = enr.comp_status
				AND compst.lang_code='cre_sys_lng_eng'
				WHERE cls.code = :code
				  AND enr.reg_status NOT IN ('lrn_crs_reg_rsc',
				                             'lrn_crs_reg_rsv') group by enr.comp_status, enr.reg_status ";
		$qry_res  = db_query($qry, array(':code' => $_REQUEST["classcode"]));
		$result = $qry_res->fetchAll();
		$resultCount = count($result);

		$pendingCount = 0;
		$pendingStatus = "";
		$title = "";
		for($i = 0; $i < $resultCount; $i++){
			if($result[$i]->reg_status_code == "lrn_crs_reg_can"){
				$result[$i]->rec_count = $result[$i]->reg_status_count;
				$result[$i]->statuscode = $result[$i]->reg_status_code;
			}
			else if($result[$i]->reg_status_code == "lrn_crs_reg_ppm" || $result[$i]->reg_status_code == "lrn_crs_reg_wtl"){
				$pendingCount += intval($result[$i]->reg_status_count,10);
				$title = $result[$i]->title;
				unset($result[$i]);
			}
		}
		if($pendingCount > 0 )
		{
			$pendingStsObj = new stdClass();
			$pendingStsObj->rec_count = $pendingCount;
			$pendingStsObj->statusname = "Pending";
			$pendingStsObj->statuscode = "lrn_crs_reg_ppm,lrn_crs_reg_wtl";
			$pendingStsObj->title = $title;
			array_push($result,$pendingStsObj);
		}
		return $result;
	}catch (Exception $ex) {
		watchdog_exception('GetEnrollmentSummaryAPI', $ex);
		expertusErrorThrow($ex);
	}
}

function listEnrollmentsByClass()
{
	try{
		$qry = "select
       cls.code 'code',
       per.id learnerid,
       per.phone_no,
       per.full_name full_name,
       per.user_name 'username',
       per.email Email,
   	   job_title.name 'job_title',
       DATE_FORMAT(enr.reg_date,'%b %d, %Y') 'reg_date',
       DATE_FORMAT(enr.comp_date,'%b %d, %Y') 'comp_date',
       if(enr.reg_status = 'lrn_crs_reg_cnf', if(enr.comp_status IS NOT NULL,compst.name,'Enrolled') ,regst.name) Status,
       enr.score Score,
       org.name Organization
		FROM slt_enrollment enr
		LEFT JOIN slt_course_class cls ON cls.id=enr.class_id
		AND cls.course_id=enr.course_id
		LEFT JOIN slt_person per ON per.id=enr.user_id
		LEFT JOIN slt_organization org ON org.id=per.org_id
   		LEFT JOIN slt_profile_list_items job_title ON per.job_title = job_title.code
		LEFT JOIN slt_profile_list_items regst ON regst.code = enr.reg_status
		AND regst.lang_code='cre_sys_lng_eng'
		LEFT JOIN slt_profile_list_items compst ON compst.code = enr.comp_status
		AND compst.lang_code='cre_sys_lng_eng'
		WHERE cls.code = :code
		  AND enr.reg_status NOT IN ('lrn_crs_reg_rsc',
		                             'lrn_crs_reg_rsv')";
		if(isset($_REQUEST["status"]) && ( $_REQUEST["status"] == "lrn_crs_reg_ppm,lrn_crs_reg_wtl"  || $_REQUEST["status"] == "lrn_crs_reg_wtl,lrn_crs_reg_ppm" ))
		{
			$qry.=" and (enr.reg_status = 'lrn_crs_reg_ppm' or enr.reg_status = 'lrn_crs_reg_wtl' )";
		}
		else if(isset($_REQUEST["status"]) && strlen($_REQUEST["status"]) >0 )
		$qry.=" and (enr.comp_status = '".$_REQUEST["status"]."' or enr.reg_status = '".$_REQUEST["status"]."')";

		$qry.=" order  by enr.score desc";

		$qry_res  = db_query($qry, array(':code' => $_REQUEST["classcode"]));
		$result  = $qry_res->fetchAll();
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		$resultCount = count($result);
		expDebug::dPrint("count:".$resultCount, 3);
		$config           = getConfig("exp_sp");
		$configSiteUrl    = $config["admin_site_url"];
		for($i = 0; $i < $resultCount; $i++){
			expDebug::dPrint($result[$i]->id, 3);
			$drupalUid = getDrupalIdOfUser($result[$i]->learnerid);
			$account = user_load($drupalUid);
			$filepath = $account->picture->filename;
			$path= '';
			if($filepath){
				$path = $configSiteUrl . '/sites/default/files/pictures/'. $filepath;
			}
			$result[$i]->profileimage_path = $path;
		}
		return $result;
	}catch (Exception $ex) {
		watchdog_exception('listEnrollmentsByClass', $ex);
		expertusErrorThrow($ex);
	}
}

function teamCompletionSummaryReportAPI()
{
	try{
		$sql = "SELECT
			   count(enr.comp_status) rec_count,
			   count(enr.reg_status) reg_status_count,
				cls.title title,
			   if(enr.reg_status = 'lrn_crs_reg_cnf', if(enr.comp_status IS NOT NULL,compst.name,'Enrolled') ,regst.name) statusname,
			   enr.comp_status statuscode,
			   enr.reg_status reg_status_code
			FROM slt_enrollment enr
			   LEFT JOIN slt_person per ON per.id = enr.user_id
			   LEFT JOIN slt_person mgr ON mgr.id = per.manager_id
			   LEFT JOIN slt_course_class cls ON cls.id = enr.class_id
			   LEFT JOIN slt_profile_list_items regst ON regst.code = enr.reg_status
			   AND regst.lang_code='cre_sys_lng_eng'
			   LEFT JOIN slt_profile_list_items compst ON compst.code = enr.comp_status
			   AND compst.lang_code='cre_sys_lng_eng'
			   LEFT JOIN slt_profile_list_items enrdeltype ON enrdeltype.code = cls.delivery_type
			   AND enrdeltype.lang_code='cre_sys_lng_eng'
			   WHERE enr.reg_status NOT IN ('lrn_crs_reg_rsc',
			                             'lrn_crs_reg_rsv')
			   AND (enr.created_by = mgr.id
			       OR enr.user_id = per.id)
			   AND cls.code=:code
			   AND mgr.id =:manager_id
			   AND enr.master_enrollment_id IS NULL group by enr.comp_status, enr.reg_status";
		$qry_res  = db_query($sql, array(':code' => $_REQUEST["classcode"],':manager_id'=>$_REQUEST["userid"]));
		$result  = $qry_res->fetchAll();
		$resultCount = count($result);
		$pendingCount = 0;
		$pendingStatus = "";
		$title = "";
		for($i = 0; $i < $resultCount; $i++){
			if($result[$i]->reg_status_code == "lrn_crs_reg_can"){
				$result[$i]->rec_count = $result[$i]->reg_status_count;
				$result[$i]->statuscode = $result[$i]->reg_status_code;
			}
			else if($result[$i]->reg_status_code == "lrn_crs_reg_ppm" || $result[$i]->reg_status_code == "lrn_crs_reg_wtl"){
				$pendingCount += intval($result[$i]->reg_status_count,10);
				$title = $result[$i]->title;
				unset($result[$i]);
			}
		}
		if($pendingCount > 0 )
		{
			$pendingStsObj = new stdClass();
			$pendingStsObj->rec_count = $pendingCount;
			$pendingStsObj->statusname = "Pending";
			$pendingStsObj->statuscode = "lrn_crs_reg_ppm,lrn_crs_reg_wtl";
			$pendingStsObj->title = $title;
			array_push($result,$pendingStsObj);
		}
		return $result;
	}catch (Exception $ex) {
		watchdog_exception('teamCompletionSummaryReportAPI', $ex);
		expertusErrorThrow($ex);
	}
}


function teamCompletionDetailedReportAPI()
{
	try{
		$sql = "SELECT per.id learnerid,per.full_name full_name,
		       per.user_name 'username',
			   per.email email,
       		   per.phone_no,
   	           job_title.name 'job_title',
        	   cls.title 'title',
		       cls.code 'code',
			   job_title.name 'job_title',
		       enrdeltype.name 'delivery_type',
		       DATE_FORMAT(enr.reg_date,'%b %d, %Y') registration_date,
		       DATE_FORMAT(enr.comp_date,'%b %d, %Y') completion_date,
		       if(enr.reg_status = 'lrn_crs_reg_cnf', if(enr.comp_status IS NOT NULL,compst.name,'Enrolled') ,regst.name) Status,
		       NULL 'percentagecompleted',
		       enr.score 'score',
			   enr.comp_status statuscode
			FROM slt_enrollment enr
			   LEFT JOIN slt_person per ON per.id = enr.user_id
			   LEFT JOIN slt_person mgr ON mgr.id = per.manager_id
			   LEFT JOIN slt_course_class cls ON cls.id = enr.class_id
			   LEFT JOIN slt_profile_list_items job_title ON per.job_title = job_title.code
			   LEFT JOIN slt_profile_list_items regst ON regst.code = enr.reg_status
			   AND regst.lang_code='cre_sys_lng_eng'
			   LEFT JOIN slt_profile_list_items compst ON compst.code = enr.comp_status
			   AND compst.lang_code='cre_sys_lng_eng'
			   LEFT JOIN slt_profile_list_items enrdeltype ON enrdeltype.code = cls.delivery_type
			   AND enrdeltype.lang_code='cre_sys_lng_eng'
			   WHERE enr.reg_status NOT IN ('lrn_crs_reg_rsc',
			                             'lrn_crs_reg_rsv')
			   AND (enr.created_by = mgr.id
			       OR enr.user_id = per.id)
			   AND cls.code=:code
			   AND mgr.id =:manager_id
			   AND enr.master_enrollment_id IS NULL";

		if(isset($_REQUEST["status"]) && ( $_REQUEST["status"] == "lrn_crs_reg_ppm,lrn_crs_reg_wtl"  || $_REQUEST["status"] == "lrn_crs_reg_wtl,lrn_crs_reg_ppm" ))
		{
			$sql.=" and (enr.reg_status = 'lrn_crs_reg_ppm' or enr.reg_status = 'lrn_crs_reg_wtl' )";
		}
		else if(isset($_REQUEST["status"]) && strlen($_REQUEST["status"]) >0 )
		$sql.=" and (enr.comp_status = '".$_REQUEST["status"]."' or enr.reg_status = '".$_REQUEST["status"]."')";

		$sql.=" order  by enr.score desc";

		$qry_res  = db_query($sql, array(':code' => $_REQUEST["classcode"],':manager_id'=>$_REQUEST["userid"]));
		$result  = $qry_res->fetchAll();
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		$resultCount = count($result);
		expDebug::dPrint("count:".$resultCount, 3);
		$config           = getConfig("exp_sp");
		$configSiteUrl    = $config["admin_site_url"];
		for($i = 0; $i < $resultCount; $i++){
			expDebug::dPrint($result[$i]->learnerid, 3);
			$drupalUid = getDrupalIdOfUser($result[$i]->learnerid);
			$account = user_load($drupalUid);
			$filepath = $account->picture->filename;
			$path= '';
			if($filepath){
				$path = $configSiteUrl . '/sites/default/files/pictures/'. $filepath;
			}
			$result[$i]->profileimage_path = $path;
		}
		return $result;
	}catch (Exception $ex) {
		watchdog_exception('teamCompletionDetailedReportAPI', $ex);
		expertusErrorThrow($ex);
	}
}
/*
 * common function to insert the user points for each action performed by the user
 */
function insertUserPointsForEachActionsPerformed ($userId,$code,$entityId = 0,$operationCode = '',$entityType = 'others',&$statements=array()) {
	include_once(drupal_get_path('module', 'exp_sp_admin_userpoints') .'/exp_sp_admin_userpoints.inc');
	expDebug::dPrint('User Id for inserting points'.$userId.'code for the action performed'.$code,4);

	if(empty($statements)){
		$is_executable = true;
		$statements = array('statement' => true);
	}else if(!empty($statements)) {
		//$txn = db_transaction();
	}
	try {
		if(module_exists('exp_sp_admin_userpoints')) {
			if($operationCode == '' || $operationCode == 'insert') {
				$userPoints = getUserPointsByCode($code);
				expDebug::dPrint('Points value for each code'.$userPoints,4);
				$insertPoints = db_insert('slt_user_points');
				$insertPoints->fields(array(
          'user_id' => $userId,
          'entity_id' => $entityId,
          'entity_type' => $entityType,
  	      'action_code' => $code,
          'operation_flag' => 'insert',
	      	'earned_points' => $userPoints,
          'total_points' => $userPoints,
          'created_by' => $userId,
	      	'created_on' => now(),
        	'updated_by' => $userId,
        	'updated_on' => now()
				));
				//$insertPoints->execute();
				if(!empty($statements)) {
					$statements[] = $insertPoints;
				} else {
					$insertPoints->execute();
				}
			}
			else if($operationCode == 'delete'){
				$userPoints = getDetailsToDeleteUserPoints($code,$entityId,$userId,$entityType);
				$updatePoints = db_update('slt_user_points');
				$updatePoints->fields(array(
          'operation_flag' => 'delete',
          'total_points' => 0,
          'updated_by' => $userId,
	      'updated_on' => now()
				));
				$updatePoints->condition('action_code',$code,'=');
				$updatePoints->condition('entity_id',$entityId,'=');
				$updatePoints->condition('user_id',$userId,'=');
				$updatePoints->condition('operation_flag','insert','=');
				if($entityType != 'others') {
					$updatePoints->condition('entity_type',$entityType,'=');
				}
				//$updatePoints->execute();
				if(!empty($statements)) {
					$statements[] = $updatePoints;
				} else {
					$updatePoints->execute();
				}
			}
		}
		//unset($txn);
			if($is_executable==true){
				dbStatementsExecution($statements);
			}
	}
	catch (Exception $ex) {
			//$txn->rollback();
			//unset($txn);
		watchdog_exception('insertUserPointsForEachActionsPerformed',$ex);
		if (isset($_GET['cron_key'])) {
			throw new Exception($ex);
		}
		expertusErrorThrow($ex);
	}
}

/*
 * function for getting the badge code for the specific user with their total points
 */
function getBadgesCodeForLeaderBoard($totalPoints) {
	try{
		$badgeArr = getBadgecodePointsKeyed($totalPoints);
		expDebug::dPrint('sample db return values'.print_r($badgeArr,true),4);
		$arr = array_values($badgeArr);
		$pointArr = array_flip($badgeArr);
		$cnt = count($arr);
		$findNum = $totalPoints;
		$result = '';
		for( $i = 0; $i < $cnt; $i++){
	  if($i != ($cnt-1)){
	  	if(($arr[$i] <= $findNum) && ($findNum < $arr[$i+1]))
	  	{
	  		$result = $arr[$i];
	  		break;
	  	}
	  }
	  else if($arr[$i] <= $findNum){
	  	$result = $arr[$i];
	  }
		}
		if($result)
		return $pointArr[$result];
	}catch (Exception $ex) {
		watchdog_exception('getBadgesCodeForLeaderBoard', $ex);
		expertusErrorThrow($ex);
	}
}

function getBadgecodePointsKeyed($totalPoints) {
	try{
		$sel = db_select('slt_badge_points','badges');
		$sel->addField('badges','badge_code');
		$sel->addField('badges','badge_points');
		$sel->condition('badges.badge_points',$totalPoints,'<=');
		expDebug::dPrintDBAPI('total code points keyed', $sel);
		$result = $sel->execute()->fetchAllKeyed();
		return $result;
	}catch (Exception $ex) {
		watchdog_exception('getBadgecodePointsKeyed', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * Fetches expertus defined language code for drupal language code
 */
function getExpLangCodeForDrupalLangCode($lang) {
	try{
		$select = db_select('slt_profile_list_items', 'splis');
		$select->addField('splis', 'code', 'drupallangcode');
		$select->condition('splis.code', db_like('cre_sys_lng_') . '%', 'LIKE');
		$select->Condition('splis.attr1', $lang);
		expDebug::dPrintDBAPI('Exp lang code for drupal lang code', $select);
		$language = $select->execute()->fetchField();
		return $language;
	}
	catch (Exception $ex) {
		watchdog_exception('getExpLangCodeForDrupalLangCode', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * Fetches users preferred language from slt_person table
 */
function userLanguageSelect($id) {
	try{
		$select = db_select('slt_person', 'spers');
		$select->addField('spers', 'preferred_language');
		$select->condition('spers.id', $id);
		expDebug::dPrintDBAPI('user preferred language', $select);
		$pref_languages = $select->execute()->fetchField();
		return $pref_languages;
	}
	catch (Exception $ex) {
		watchdog_exception('userLanguageSelect', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * Fetches the tax applied to an order item
 */
function getChargedTaxForOrderItem($sltOrderId, $entityId, $baseType) {
	try {
		// Refund the tax charged for the order item
		$select = db_select('slt_order_items', 'soi');
		$select->addField('soi', 'tax_amount', 'tax_amount');
		$select->condition('soi.order_id', $sltOrderId, '=');
		if (strtolower($baseType) == 'tp') {
			$select->condition('soi.program_id', $entityId, '=');
		}
		else {
			$select->condition('soi.class_id', $entityId, '=');
		}
		expDebug::dPrintDBAPI('$select', $select);
		$taxForOrderItem = $select->execute()->fetchField();
		expDebug::dPrint('$taxForOrderItem = ' . $taxForOrderItem,3);
		return $taxForOrderItem;
	}
	catch (Exception $ex) {
		watchdog_exception('getChargedTaxForOrderItem', $ex);
		expertusErrorThrow($ex);
	}
}


/*
 * Allow Re-Certification Before Certification Expired #0012673
*/
function getRecertifyBeforeCertifyExpired($completedDate, $expiredValue ,$expiredUnit){

	expDebug::dPrint(' getRecertifyBeforeCertifyExpired $completedDate = '.$completedDate,4);
	expDebug::dPrint(' getRecertifyBeforeCertifyExpired $expiredValue = '.$expiredValue,4);
	expDebug::dPrint(' getRecertifyBeforeCertifyExpired $expiredUnit = '.$expiredUnit,4);
	
	//Disable recertify when the certificate has no expiry
	if(empty($expiredValue))
		return 0;
	
	// Convert Expired Unit in to Mysql Unit format
	if($expiredUnit == 'days'){
		$expiredUnit = 'DAY';
	}else if($expiredUnit == 'months'){
		$expiredUnit = 'MONTH';
	}else if($expiredUnit == 'years'){
		$expiredUnit = 'YEAR';
	}// end id

	expDebug::dPrint(' getRecertifyBeforeCertifyExpired $expiredUnit for Mysql = '.$expiredUnit,4);

	// Expired date calulation
	// Completed Date + (Expired Values & Expired Unit)
	$expiredDateQry = db_query("SELECT DATE_ADD('$completedDate',INTERVAL $expiredValue $expiredUnit) AS expiredDate");
	$expiredDate = $expiredDateQry->fetchField();
	expDebug::dPrintDBAPI(' getRecertifyBeforeCertifyExpired $expiredDateQry = ',$expiredDateQry);
	expDebug::dPrint(' getRecertifyBeforeCertifyExpired $expiredDate = '.$expiredDate,4);

	// Get certification expired remainder days from exp_sp.ini
	$config           = getConfig("exp_sp");
	$cert_days_expire = isset($config["cert_expire_reminder_days"]) ? $config["cert_expire_reminder_days"] : 10;
	expDebug::dPrint(' getRecertifyBeforeCertifyExpired $cert_days_expire = '.$cert_days_expire,4);

	// Expired date calulation for before cerification expired
	// Expired Date - (Expired Values & Expired Unit)
	$beforeExpiredDateQry = db_query("SELECT DATE_SUB('$expiredDate',INTERVAL $cert_days_expire DAY) AS beforeExpiredDate");
	$beforeExpiredDate = $beforeExpiredDateQry->fetchField();
	expDebug::dPrintDBAPI(' getRecertifyBeforeCertifyExpired $beforeExpiredDateQry = ',$beforeExpiredDateQry);
	expDebug::dPrint(' getRecertifyBeforeCertifyExpired $beforeExpiredDate = '.$beforeExpiredDate,4);

	// Re - Certification start date check
	$recertifyStartCheckQry = db_query(" SELECT (curdate() >= date_format('$beforeExpiredDate','%Y-%m-%d')) AS recertifyStartCheck");
	$recertifyStartCheck = $recertifyStartCheckQry->fetchField();
	expDebug::dPrintDBAPI(' getRecertifyBeforeCertifyExpired $recertifyStartCheckQry = ',$recertifyStartCheckQry);
	expDebug::dPrint(' getRecertifyBeforeCertifyExpired $recertifyStartCheck = '.$recertifyStartCheck,4);

	// 1 Allow Re-Certify, 0 Do not allow Re-Certify
	return $recertifyStartCheck;

}// end getRecertifyBeforeCertifyExpired

function getRefundAmount($classId, $baseType, $unitPrice, $waitlistItem, $taxForOrderItem) {
	try{
		$refundPercentage = 0.0;
		$secsInADay = 24 * 60 * 60;

		if(strtolower($baseType) == 'ilt' || strtolower($baseType) == 'vc') {
			// Get date, start_time, end_time for all the sessions of this class. For VC also get PHP timezone.
			$select = db_select('slt_course_class_session', 'ses');
			$select->condition('ses.class_id', $classId, '=');

			$select->leftjoin('slt_profile_list_items', 'splitz',
					'ses.timezone = splitz.code  AND ses.timezone IS NOT NULL AND splitz.lang_code = \'cre_sys_lng_eng\'');

			$select->addExpression('CONCAT(date(ses.start_date), \' \', ses.start_time, \':00\')', 'start_date_time');
			$select->addField('splitz', 'attr2', 'phptimezone');
			$select->orderBy('ses.start_date');
			$select->orderBy('ses.start_time');
			$select->range(0, 1);
			expDebug::dPrintDBAPI('$select', $select);
			$firstSession = $select->execute()->fetch();
			expDebug::dPrint('$result = ' . print_r($firstSession, true));

			if (empty($firstSession->phptimezone)) {
				$curDateTimeUX = time();
				$sesStartDateTimeUX = strtotime($firstSession->start_date_time);
			}
			else {
				$sessionTZ = new DateTimeZone($firstSession->phptimezone);
				$curDateTimeInSessionTZ = new DateTime(null, $sessionTZ);
				$sesStartDateTimeInSessionTZ = new DateTime($firstSession->start_date_time, $sessionTZ);

				$curDateTimeUX = $curDateTimeInSessionTZ->getTimestamp();
				$sesStartDateTimeUX = $sesStartDateTimeInSessionTZ->getTimestamp();
			}

			$diffTimeInSec = $sesStartDateTimeUX - $curDateTimeUX;
			expDebug::dPrint('$diffTimeInSec = ' . $diffTimeInSec . ', diff time in days = ' . $diffTimeInSec/$secsInADay, 4);

			$select = db_select('slt_drop_policy', 'pol');
			$select->addField('pol', 'refund_percentage', 'refund_percentage');
			$select->addField('pol', 'days', 'days');
			if (strtolower($baseType) == 'ilt') {
				$select->condition('pol.delivery_type', 'lrn_cls_dty_ilt', '=');
			}
			else {
				$select->condition('pol.delivery_type', 'lrn_cls_dty_vcl', '=');
			}
			$select->orderBy('pol.days', 'DESC');
			expDebug::dPrintDBAPI('$select', $select);
			$dropPolicyList = $select->execute()->fetchAll();
			expDebug::dPrint('$dropPolicyList = ' . print_r($dropPolicyList, true));

			foreach ($dropPolicyList as $dropPolicy) {
				//Added by Shobanan for #0048250.
				if($dropPolicy->days == 0)
					$dropPolicySecs = ($secsInADay > $diffTimeInSec) ? $diffTimeInSec : $secsInADay;
				else
					$dropPolicySecs = $dropPolicy->days * $secsInADay;
					expDebug::dPrint('$dropPolicyList dd= ' . print_r($diffTimeInSec, true).'sdfsgddg'.$dropPolicySecs);
				if ($diffTimeInSec <= $dropPolicySecs) {
					$refundPercentage = $dropPolicy->refund_percentage;
				}
				else {
					break; // found $refundPercentage
				}
			}
		}
		else if(strtolower($baseType) == 'wbt') {
			$select = db_select('slt_drop_policy', 'pol');
			$select->addField('pol', 'refund_percentage', 'refund_percentage');
			$select->condition('pol.delivery_type', 'lrn_cls_dty_wbt', '=');
			expDebug::dPrintDBAPI('$select', $select);
			$dropPolicy = $select->execute()->fetch();
			expDebug::dPrint('$dropPolicy = ' . print_r($dropPolicy, true));
			$refundPercentage = $dropPolicy->refund_percentage;
		}
		else if(strtolower($baseType) == 'vod') {
			$select = db_select('slt_drop_policy', 'pol');
			$select->addField('pol', 'refund_percentage', 'refund_percentage');
			$select->condition('pol.delivery_type', 'lrn_cls_dty_vod', '=');
			expDebug::dPrintDBAPI('$select', $select);
			$dropPolicy = $select->execute()->fetch();
			expDebug::dPrint('$dropPolicy = ' . print_r($dropPolicy, true));
			$refundPercentage = $dropPolicy->refund_percentage;
		}
		else if(strtolower($baseType) == 'tp') {
			$select = db_select('slt_drop_policy', 'pol');
			$select->addField('pol','refund_percentage', 'refund_percentage');
			$select->condition('pol.delivery_type', 'cre_sys_obt_trp', '=');
			expDebug::dPrintDBAPI('$select', $select);
			$dropPolicy = $select->execute()->fetch();
			expDebug::dPrint('$dropPolicy = ' . print_r($dropPolicy, true));
			$refundPercentage = $dropPolicy->refund_percentage;
		}
		expDebug::dPrint('$refundPercentage = ' . $refundPercentage, 4);

		$refundAmount = 0.0;
		if ($waitlistItem == 'lrn_crs_reg_wtl' || $waitlistItem == 'lrn_tpm_ovr_wtl') {
			$refundAmount = $unitPrice+$taxForOrderItem;
		}
		else if (trim($refundPercentage) != 0.0) {
			$refundAmount = ($unitPrice+$taxForOrderItem) - (($refundPercentage / 100) * ($unitPrice+$taxForOrderItem));
		}
		else {
			$refundAmount = $unitPrice+$taxForOrderItem;
		}
		$deductedAmount = ($unitPrice+$taxForOrderItem) - $refundAmount;
		expDebug::dPrint('$refundAmount = ' . $refundAmount . ', $$taxForOrderItem ' . $taxForOrderItem .',$unitPrice = ' . $unitPrice . ', $deductedAmount = ' . $deductedAmount, 4);

		//$refundAmount = $refundAmount + $taxForOrderItem;

		$returnArr = array('deducted_amount' => $deductedAmount,
				'refund_value' => $refundAmount);
		expDebug::dPrint('$returnArr = ' . print_r($returnArr, true), 5);
		expDebug::dPrint('$returnArr = ' . print_r($returnArr, true), 5);
		return $returnArr;
	}
	catch (Exception $ex) {
		watchdog_exception('getRefundAmount', $ex);
		expertusErrorThrow($ex);
	}
}

function dropClassLineItem($param){
	try{
		expDebug::dPrint("dropClassLineItem param ". print_r($param, true), 5);
		$enrollId 			= $param['enroll_id'];
		$baseType       = $param['base_type'];
		$isRefunded     = $param['is_refunded'];
		$refund_amt     = $param['refund_amount'];
		$userId         = $param['user_id'];
		$updateUserId   = $param['update_user_id'];
		$cancelFrom     = $param['cancel_from'];
		$ucOrderId      = $param['uc_order_id'];
		$fromInstructor = $param['from_instructor'];
		$fromManager    = $param['from_manager'];

		$selectEnr = db_select('slt_enrollment', 'enr');
		$selectEnr->addField('enr','order_id', 'order_id');
		$selectEnr->addField('enr','course_id', 'course_id');
		$selectEnr->addField('enr','class_id', 'class_id');
		$selectEnr->addField('enr','user_id', 'user_id');
		$selectEnr->addField('enr','reg_status', 'reg_status');
		$selectEnr->addField('enr','waitlist_flag', 'waitlist_flag');
		$selectEnr->addField('enr','comp_status', 'comp_status');
		$selectEnr->addField('enr','comp_date', 'comp_date');
		$selectEnr->addField('enr','reg_status_date', 'reg_status_date');
		$selectEnr->addField('enr','waitlist_priority', 'waitlist_priority');
		$selectEnr->condition('enr.id', $enrollId,'=');
		$selectEnr->range(0, 1);
		$resultEnr = $selectEnr->execute()->fetchAll();
		expDebug::dPrint("Entering to drop enrollment method");
		//call to delete enroll
		$updatedRows = enrollmentDelete($enrollId,$updateUserId);
		expDebug::dPrint('enrollmentDelete updated rows '.$updatedRows, 5);
		if($updatedRows > 0) {
		if($isRefunded){
			$select = db_select('slt_enrollment', 'enr');
			$select->addField('enr','order_id', 'order_id');
			$select->addField('enr','course_id', 'course_id');
			$select->addField('enr','class_id', 'class_id');
			$select->addField('enr','user_id', 'user_id');
			$select->condition('enr.id', $enrollId,'=');
			$select->range(0, 1);
			$result =$select->execute()->fetchAll();
			$sltOrderId = $result[0]->order_id;

			$updateStmt = db_update('slt_order_items');
			// Set conditions
			$updateStmt->condition('order_id', $sltOrderId);
			$updateStmt->condition('user_id', $result[0]->user_id);
			$updateStmt->condition('course_id', $result[0]->course_id);
			$updateStmt->condition('class_id', $result[0]->class_id);
			$updateStmt->condition('program_id', '','IS NULL');
			$updateStmt->fields(array(
				'refund_amount'      => $refund_amt,
				'order_status'			 => 'cme_pmt_sts_rjt'
				));
				$updateStmt->execute();

				if($ucOrderId){
					//Call upercart order cancllation
					cancelUpertCartOrder($ucOrderId,$sltOrderId,$updateUserId);
				}
				if(!$param['no_notify']){
					$iCal = null;
					if ($baseType == 'ILT' || $baseType == 'VC') {
						expDebug::dPrint('Generating DropRefund notification with ical', 4);
						$iCal = getCancelICal($enrollId, 'Class');
					}
					insertNotification('DropRefund', $enrollId, $resultEnr[0]->class_id, $resultEnr[0]->user_id, $updateUserId, $iCal);
				}
		}
		else if(!$fromInstructor && !$fromManager){
			if($baseType=="ILT" && $resultEnr[0]->waitlist_flag != 'lrn_crs_reg_wtl'){
				$iCal = getCancelICal($enrollId, 'Class');
		  expDebug::dPrint('Generating Drop notification with ical', 4);
		  insertNotification('Drop', $enrollId, $resultEnr[0]->class_id, $resultEnr[0]->user_id, $updateUserId, $iCal);
			}
			else if($baseType=="WBT" || $baseType=="VOD"){
				insertNotification('WBTDropped', $enrollId, $resultEnr[0]->class_id, $resultEnr[0]->user_id, $updateUserId);
			}
			else if($baseType=="VC" && $resultEnr[0]->waitlist_flag != 'lrn_crs_reg_wtl'){
				$iCal = getCancelICal($enrollId, 'Class');
				expDebug::dPrint('Generating VCDropped notification with ical', 4);
				insertNotification('VCDropped', $enrollId, $resultEnr[0]->class_id, $resultEnr[0]->user_id, $updateUserId, $iCal);
			}
		}
		else if($resultEnr[0]->waitlist_flag != 'lrn_crs_reg_wtl'){
			if($fromInstructor == 1){
				$iCal = null;
				if ($baseType == 'ILT' || $baseType == 'VC') {
					$iCal = getCancelICal($enrollId, 'Class');
					expDebug::dPrint('Generating ClsRejectByInstructor notification with ical', 4);
				}
				insertNotification('ClsRejectByInstructor', $enrollId, $resultEnr[0]->class_id, $resultEnr[0]->user_id, $updateUserId, $iCal);
			}
			else if($fromManager == 1) {
				$iCal = null;
				if ($baseType == 'ILT' || $baseType == 'VC') {
					$iCal = getCancelICal($enrollId, 'Class');
					expDebug::dPrint('Generating ClsRejectByManager notification with ical', 4);
				}
				insertNotification('ClsRejectByManager', $enrollId, $resultEnr[0]->class_id, $resultEnr[0]->user_id, $updateUserId, $iCal);
			}
		}

		/* Waitlist Promotion Call */
		waitlistPromotionUpdate($resultEnr[0]->class_id,$resultEnr[0]->reg_status);
		if($resultEnr[0]->waitlist_flag == 'lrn_crs_reg_wtl') {
			insertNotification('WaitlistDrop', $resultEnr[0]->course_id, $resultEnr[0]->class_id, $resultEnr[0]->user_id, $updateUserId);
		}
		$insertaudit= array(
			'reg_status'      => 'lrn_crs_reg_can',
			'comp_status'     => NULL,
			'comp_date'		    => NULL,
			'reg_status_date' =>  now(),
			'waitlist_flag'		=> NULL,
			'waitlist_priority'		=> NULL,
			'updated_on'      => now(),
			'updated_by'      => $updateUserId
		);

		insertUserPointsForEachActionsPerformed($userId,'register_class',$enrollId,'delete','class');
		make_audit_trail_entries($insertaudit, $userId , $resultEnr, $resultEnr[0]->class_id,'updated','cre_sys_obt_cls','exp_sp_lnrenrollment','classcancelled',
		array('reg_status','comp_status','comp_date','reg_status_date','waitlist_flag','waitlist_priority'),$updateUserId);
		}
		
		syncSolrData('Enrollment');
		syncSolrData('User');
	}catch (Exception $ex) {
		watchdog_exception('dropClassLineItem', $ex);
		expertusErrorThrow($ex);
	}
}

function dropTPLineItem($param,$comp = ''){
	try{
		expDebug::dPrint("dropTPLineItem param ". print_r($param, true), 5);
		$masterEnrollId = $param['master_enroll_id'];
		$prgId          = $param['prg_id'];
		$isRefunded     = $param['is_refunded'];
		$refund_amount  = $param['refund_amount'];
		$user_id        = $param['user_id'];
		$updateUserId   = $param['update_user_id'];
		$cancelFrom     = $param['cancel_from'];
		$ucOrderId      = $param['uc_order_id'];
		$fromManager    = $param['from_manager'];
		$date = now();

		/* Waitlist Promotion Classes */
		$listClasses = db_select('slt_enrollment', 'enr');
		$listClasses->addField('enr','class_id');
		$listClasses->addField('enr','course_id');
		$listClasses->addField('enr','reg_status');
		$listClasses->condition('enr.master_enrollment_id', $masterEnrollId,'=');
		$resultEnrClasses =$listClasses->execute()->fetchAll();
		expDebug::dPrintDBAPI('oldvalue=', $listClasses);

		$updateStmt = db_update('slt_enrollment');
		$updateStmt->condition('user_id', $user_id);
		$updateStmt->condition('master_enrollment_id', $masterEnrollId);
		if($comp == 1)
			$updateStmt->condition('comp_status',array('lrn_crs_cmp_cmp'),'NOT IN'); //Ignore completed classes in the update #83881
		$updateStmt->fields(array(
			'reg_status'      	=> 'lrn_crs_reg_can',
			'reg_status_date'   => $date,
			'updated_on'      	=> $date,
			'updated_by'      	=> $updateUserId,
			'comp_status'     	=> NULL,
			'comp_date'       	=> NULL,
			'waitlist_flag'     => NULL,
			'waitlist_priority' => NULL
		));
		$updateStmt->execute();

		//Get Previous master_enroll_status
		$selectOverallStatus = db_select('slt_master_enrollment','mas');
		$selectOverallStatus->addField('mas','overall_status');
		$selectOverallStatus->addField('mas','recertify_path');
		$selectOverallStatus->condition('mas.id', $masterEnrollId);
		$prevOverallStatus   = $selectOverallStatus->execute()->fetchAll();
		expDebug::dPrintDBAPI('prevvalue=',$selectOverallStatus );

		$defaultvalue[0] =new stdClass();
		$defaultvalue[0]->id = $masterEnrollId;
		$defaultvalue[0]->overall_status = $prevOverallStatus[0]->overall_status;
		$defaultvalue[0]->updated_on = $date;
		$defaultvalue[0]->cancel_date = $date;
		$defaultvalue[0]->updated_by = $updateUserId;
		$certify_path = $prevOverallStatus[0]->recertify_path;

		expDebug::dPrint('defaultvalue='.print_r($defaultvalue[0],TRUE),4);
		//Update master enroll
		$updateMasterEnr = db_update('slt_master_enrollment');
		$updateMasterEnr->condition('id', $masterEnrollId);
		$updateMasterEnr->condition('user_id', $user_id);
		$updateMasterEnr->where('overall_status NOT IN (\'lrn_tpm_ovr_cmp\',\'lrn_tpm_ovr_cln\')');
		$updateMasterEnr->fields(array(
			'overall_status' => 'lrn_tpm_ovr_cln',
			//'is_current' => ($certify_path > 1) ? 'N' : 'Y',
			'updated_on'     => $date,
			'cancel_date'    => $date,
			'custom4'				 => 'lrn_tpm_ovr_cln',
			'updated_by'     => $updateUserId
		));
		$updateMasterEnr->execute();
		expDebug::dPrintDBAPI('update=', $updateMasterEnr);
		
		/*if($certify_path > 1){
			$selectStmt = db_select('slt_master_enrollment', 'ms');
			$selectStmt->addField('ms','id','id');
			$selectStmt->addField('ms','custom4','custom4');
			$selectStmt->addField('ms','comp_date','comp_date');
			$selectStmt->condition('ms.program_id',$prgId,'=');
			$selectStmt->condition('ms.user_id',$user_id,'=');
			$selectStmt->condition('ms.recertify_path',($certify_path-1),'=');
			$selectStmt->orderBy('ms.id', 'desc');
			$selectStmt->range(0, 1);
			expDebug::dPrintDBAPI('previous enrollment status update query :',$selectStmt);
			$prev_menr = $selectStmt->execute()->fetchAll();
			expDebug::dPrint("prev enrollment  >> ".print_r($prev_menr,true),5);
			$prev_menrid = $prev_menr[0]->id;
			$prev_custom4 = $prev_menr[0]->custom4;
			$prev_comp = $prev_menr[0]->comp_date;
	       	expDebug::dPrint("master enrollmetn id prev ". $prev_menrid ,5);
		       
	       	$cert_days_expire = isset($config["cert_expire_reminder_days"]) ? $config["cert_expire_reminder_days"] : 10;
	       	//get certificate expired or not
	       	$select = db_select('slt_program', 'p');
	       	$select->addExpression('(CASE WHEN(DATEDIFF(
               IF (p.expires_in_unit = \'days\', DATE_ADD(\''.$prev_comp.'\', INTERVAL p.expires_in_value DAY),
                     IF (p.expires_in_unit = \'months\', DATE_ADD(\''.$prev_comp.'\', INTERVAL p.expires_in_value MONTH),
                           DATE_ADD(\''.$prev_comp.'\', INTERVAL p.expires_in_value YEAR))),
               CURDATE()) < 0)THEN \'lrn_tpm_ovr_exp\'
               ELSE \'lrn_tpm_ovr_cmp\' END )','prev_status');
			
	 	 	$select->condition('p.id',$prgId);
	 	 	expDebug::dPrintDBAPI('certificate expiry or not :',$select);
	       	$prev_sts = $select->execute()->fetchField();
	 	 	
	       $updateMasterFields1['overall_status'] = $prev_sts;
	       $updateMasterFields1['is_current'] = 'Y';
	       $updateMasterEnroll1 = db_update('slt_master_enrollment');
	       $updateMasterEnroll1->fields($updateMasterFields1);
	       $updateMasterEnroll1->condition('id', $prev_menrid);
	       $updateMasterEnroll1->condition('program_id', $prgId);
	       expDebug::dPrintDBAPI('master enrollment update query 222 :',$updateMasterEnroll1);
	       $updateMasterEnroll1->execute();  
		}*/
		
		$insertaudit=array(
			'id'             =>$masterEnrollId,
			'overall_status' => 'lrn_tpm_ovr_cln',
			'updated_on'     => $date,
			'cancel_date'    => $date,
			'updated_by'     => $updateUserId

		);

		/*query for fetching object type for audit trail
		 *  ticket-0021440 on 7/2/13*/
		$select = db_select('slt_program', 'prg');
		$select->addField('prg','object_type','object_type');
		$select->condition('prg.id', $prgId);
		$objresult =$select->execute()->fetchField();
		expDebug::dPrintDBAPI('objecttype query:', $select);
		expDebug::dPrint('objresult'.$objresult,4);

		if($isRefunded)	{
			$select = db_select('slt_enrollment', 'enr');
			$select->addField('enr','order_id', 'order_id');
			$select->condition('enr.master_enrollment_id', $masterEnrollId,'=');
			$select->condition('enr.user_id', $user_id);
			$select->range(0, 1);
			$result =$select->execute()->fetchAll();

			$sltOrderId = $result[0]->order_id;

			$updateStmt = db_update('slt_order_items');
			// Set conditions
			$updateStmt->condition('order_id', $sltOrderId);
			$updateStmt->condition('user_id', $user_id);
			$updateStmt->condition('program_id', $prgId);
			$updateStmt->fields(array(
				'refund_amount'      => $refund_amount,
				'order_status'			 => 'cme_pmt_sts_rjt'
				));
				$updateStmt->execute();
				if($ucOrderId){
					//Call upercart order cancllation
					cancelUpertCartOrder($ucOrderId,$sltOrderId,$updateUserId);
				}
		}
		// Certification/Curriculum/Learning Plan cancel implemented
		if($isRefunded)	{
			$notificationType = "certCurrLPCancelRefund";
			$objectId1 = $prgId;
			$objectId2 = $masterEnrollId;
		}
		else if($prevOverallStatus[0]->overall_status == 'lrn_tpm_ovr_wtl'){
			$objectId1 = $masterEnrollId;
			$objectId2 = $prgId;
			$notificationType = "certCurrLPWaitlistCancel";
		}
		else if($fromManager){
			$notificationType = "certCurrLPRejectionByManager";
			$objectId1 = $masterEnrollId;
			$objectId2 = $prgId;
		}
		else{
			$objectId1 = $prgId;
			$objectId2 = $masterEnrollId;
			$notificationType = "certCurrLPCancel";
		}
		expDebug::dPrint('$notificationType = ' . $notificationType, 4);
		if (!$param['no_notify']) {
	  $iCal = null;
	  if ($notificationType == 'certCurrLPCancel' || $notificationType == 'certCurrLPCancelRefund' || $notificationType == 'certCurrLPRejectionByManager') {
	  	$iCal = getCancelICal($masterEnrollId, 'TP');
	  	expDebug::dPrint('Generating certCurrLPCancel or certCurrLPCancelRefund or certCurrLPRejectionByManager notification with ical', 4);
	  }
	  insertNotification($notificationType, $objectId1, $objectId2, $user_id, $updateUserId, $iCal);
		}

		/* Waitlist Promotion Call */
		foreach ($resultEnrClasses as $record) {
			$classId = $record->class_id;
			$prevRegStatus = $record->reg_status;
			waitlistPromotionUpdate($classId,$prevRegStatus);
		}
		insertUserPointsForEachActionsPerformed($user_id,'register_class',$masterEnrollId,'delete','tp');
		syncSolrData('MasterEnrollment');
		//Audit Entry
		make_audit_trail_entries( $insertaudit, $user_id ,$defaultvalue, $prgId ,'updated',$objresult,'exp_sp_core','tpcancelled',
		array('id','overall_status','updated_by'),$updateUserId);
	}catch (Exception $ex) {
		watchdog_exception('dropTPLineItem', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * upercart order cancllation.
 */
function cancelUpertCartOrder($ucOrderId, $sltOrderId,$updateUserId){
	try{
		$select = db_select('uc_order_products', 'uop');
		$select->addField('uop','nid');
		$select->condition('uop.order_id', $ucOrderId);
		$orderCount = $select->execute()->rowCount();

		$selectCancelOrder = db_select('slt_order_items', 'soi');
		$selectCancelOrder->addField('soi','id');
		$selectCancelOrder->condition('soi.order_id', $sltOrderId);
		$selectCancelOrder->condition(db_or()
		->isNull('soi.program_id')
		->condition('soi.program_id' , 0));
		$selectCancelOrder->condition('soi.order_status', 'cme_pmt_sts_rjt');
		expDebug::dPrintDBAPI('class query:', $selectCancelOrder);
		$cancelClsCount = $selectCancelOrder->execute()->rowCount();

		$selectCancelTPOrder = db_select('slt_order_items', 'soi');
		$selectCancelTPOrder->addField('soi','id');
		$selectCancelTPOrder->condition('soi.order_id', $sltOrderId);
		$selectCancelTPOrder->condition('soi.program_id' , 0 ,'>');
		$selectCancelTPOrder->condition('soi.order_status', 'cme_pmt_sts_rjt');
		$selectCancelTPOrder->groupBy('soi.program_id');
		expDebug::dPrintDBAPI('tp query:', $selectCancelTPOrder);
		$cancelTpCount = $selectCancelTPOrder->execute()->rowCount();

		$cancelCount = $cancelClsCount + $cancelTpCount;

		expDebug::dPrint("orderCount -> " . $orderCount . " cancelCount ->" .$cancelCount  , 4);
		if($cancelCount >= $orderCount){
			expDebug::dPrint("cancelUpertCartOrder ucOrderId" .$ucOrderId  , 4);

			$select = db_select('uc_orders', 'uo');
			$select->addField('uo','order_status');
			$select->condition('uo.order_id', $ucOrderId);
			$order_current_status = $select->execute()->fetchField();

			uc_order_update_status($ucOrderId, 'canceled');
			expDebug::dPrint(' cancelUpertCartOrder parent order cancel = ' . print_r($ucOrderId, true) , 4);

			$updateOrder = db_update('slt_order');
			$updateOrder->condition('id', $sltOrderId);
			$updateOrder->fields(array(
				'order_status' => 'cme_pmt_sts_rjt',
				'updated_on'    => now(),
				'updated_by'    => $updateUserId
			));
			$updateOrder->execute();

			//Insert Noitification
			$selectEnr = db_select('slt_enrollment', 'enr');
			$selectEnr->addField('enr','id');
			$selectEnr->addField('enr','user_id');
			$selectEnr->condition('enr.order_id', $sltOrderId);
			$selectEnr->range(0, 1);
			$resEnr = $selectEnr->execute()->fetchAll();
			$notiUserId = $resEnr[0]->user_id;
			$notiEnrId  = $resEnr[0]->id;
			if($notiUserId && $notiEnrId){
				insertNotification('pay_checkout_rejected', $notiEnrId, $notiEnrId, $notiUserId, $updateUserId);
			}

			//Audit trail for order status change
			$changefields = array(
				'order_id'  => $ucOrderId,
				'status'    => 'canceled',
			);
			$default[0] = new stdClass();
			$default[0]->order_id = $ucOrderId;
			$default[0]->status   = $order_current_status;
			make_audit_trail_entries($changefields, $updateUserId , $default ,$ucOrderId, 'Updated','Commerce','exp_sp_administration_order','updateUserOrder',
			array('order_id', 'status'));

		}
	}catch (Exception $ex) {
		watchdog_exception('cancelUpertCartOrder', $ex);
		expertusErrorThrow($ex);
	}
}

function getDropListDiscountAmount($nodeId,$ucOrderId){
	try{
		$selectDiscount = db_select('slt_class_discount','dis');
		$selectDiscount->addExpression('SUM(dis.discount)','discount');
		$selectDiscount->condition('dis.nid', $nodeId);
		$selectDiscount->condition('dis.order_id', $ucOrderId);
		$discountAmt = $selectDiscount->execute()->fetchField();
		//$discountAmt = $discountAmt ? number_format($discountAmt, 2) : 0;
		$discountAmt = $discountAmt ? $discountAmt : 0;
		expDebug::dPrint('getDropListDiscountAmount = ' . print_r($discountAmt, true) , 4);
		return $discountAmt;
	}catch (Exception $ex) {
		watchdog_exception('getDropListDiscountAmount', $ex);
		expertusErrorThrow($ex);
	}
}

function getDropListUnitPrice($nodeId,$ucOrderId){
	try{
		$selectPrice = db_select('uc_order_products','uop');
		$selectPrice->addField('uop','price');
		$selectPrice->condition('uop.nid', $nodeId);
		$selectPrice->condition('uop.order_id', $ucOrderId);
		$priceAmt = $selectPrice->execute()->fetchField();
		//$priceAmt = $priceAmt ? number_format($priceAmt, 2) : 0;
		$priceAmt = $priceAmt ? $priceAmt : 0;
		expDebug::dPrint(' getDropListUnitPrice = ' . print_r($priceAmt, true) , 4);
		return $priceAmt;
	}catch (Exception $ex) {
		watchdog_exception('getDropListUnitPrice', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * Returns user preferred language slt code.
 */
function getUserPreferredLanguage($getAnonLang = false) {
	try {
		$userPreferedLanguage = '';
		$u_id = getIdOfLoggedInUser();
		// Added by Vincent for performance imporvement on Sep 2, 2015
		if(isset($_SESSION['Pref_Language']) && !empty($_SESSION['Pref_Language']) && $u_id)
			return $_SESSION['Pref_Language'];

		if($u_id == 0){
			$userPreferedLanguage = getExpLangCodeForDrupalLangCode($GLOBALS['language']->language);
		}
		if ($u_id) {
			$userPreferedLanguage = userLanguageSelect($u_id);
			if (empty($userPreferedLanguage)) {
				$sitelang =  language_default('language');
				$userPreferedLanguage = getExpLangCodeForDrupalLangCode($sitelang);
			}
		}
		else if ($getAnonLang) { // if set to true, return anonymous user's preferred language for the session
			if (empty($_SESSION[language])) {
				$userPreferedLanguage = getExpLangCodeForDrupalLangCode('en-us');
			}
			else {
				$userPreferedLanguage = getExpLangCodeForDrupalLangCode($_SESSION[language]);
			}
		}
		$_SESSION['Pref_Language'] = $userPreferedLanguage;
		return $userPreferedLanguage;
	}
	catch (Exception $ex) {
		watchdog_exception('getUserPreferredLanguage', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * Fetches all the timezones from slt_profile_list_items
 */
function getTimeZonesList(){
	try{
		$select = db_select('slt_profile_list_items', 'spfa');
		$select->join('slt_profile_list_items','spfb','spfa.parent_id = %alias.id');
		$select->addField('spfa', 'code','zonecode');
		$select->addField('spfa', 'name','zonename');
		$select->condition('spfb.code', 'cre_sys_tmz');
		$select->orderBy('CAST(REPLACE(REPLACE(REPLACE(REPLACE(spfa.attr1,"GMT",""),"(",""), ")", ""),":","")AS SIGNED)');
		$timezone = $select->execute()->fetchAll();
		return $timezone;
	}catch (Exception $ex) {
		watchdog_exception('getTimeZone', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Get results for Rating.
 *
 * @param1 entity id
 * @param2 entity type
 * @return
 *   Rating Results
 */
function getEntityRatingInfo($node_id,$entity_type){
	try{
		$select = db_select('votingapi_cache','vote');
		$select->addField('vote', 'value', 'rating');
		$select->condition('vote.tag', 'vote');
		$select->condition('vote.function', 'average');
		$select->condition('vote.entity_id', $node_id);
		$select->condition('vote.entity_type', $entity_type);
		$rating = $select->execute()->fetchField();
		return $rating;
	}catch (Exception $ex) {
		watchdog_exception('getEntityRatingInfo', $ex);
		expertusErrorThrow($ex);
	}
}

/***
 * Get vote value for a class voted by particular user
 *
 */
function getUserRatingvalueforClass($userid,$nodeid){

	$selectStmt = db_select('votingapi_vote', 'vote');
	$selectStmt->addField('vote', 'value', 'vote_value');
	$selectStmt->condition('vote.entity_id', $nodeid);
	$selectStmt->condition('vote.uid', $userid);
	$vote_value_result  = $selectStmt->execute()->fetchField();
	if($vote_value_result == ""){
		return "0";
	}
	return $vote_value_result;
}

/*
 * Get list of Languages in slt_profile_list_items
 *Using this same function for exp_sp_administration_user.inc
 */
function getMyAccountAllLanguage(){
	try{
		$select = db_select('slt_profile_list_items', 'spfa');
		$select->join('slt_profile_list_items','spfb','spfa.parent_id = %alias.id');
		$select->addField('spfa', 'code','langcode');
		$select->addField('spfa', 'name','langname');
		$select->addField('spfa', 'attr1','drupallangcode');
		$select->condition('spfb.code', 'cre_sys_lng');
		$select->Condition('spfa.is_active','Y');
		$select->orderBy('langname', 'ASC');
		$languages = $select->execute()->fetchAll();
		return $languages;
	}catch (Exception $ex) {
		watchdog_exception('getAllLanguage', $ex);
		expertusErrorThrow($ex);
	}
}

function isClassSwitchAvail($deliveryType,$classId,$courseId,$userId,$enrollId,$type){
	try{
		expDebug::dPrint('Dedicated class count checkingtp'.print_r($type,true),5);
		$classSwitch = 0;
		if($type == 'program'){
			$alterClassCnt = getAlternativeEventStatus($classId,$courseId);
			expDebug::dPrint('other classes for the same course under tp'.print_r($alterClassCnt,true),5);
			if($alterClassCnt)
				$classSwitch = 1;
		
		}else{
			    $dedicated_class = checkDedicatedClassMyEnroll($classId,$courseId);
				expDebug::dPrint('non dedicated classes for the same course'.print_r($dedicated_class,true),5);
				if($dedicated_class != 0){
				//	$alterClassCnt = getAlternativeEventStatus($classId,$courseId);
				//	expDebug::dPrint('other classes for the same course'.print_r($alterClassCnt,true),5);
				//	if($alterClassCnt)
						$classSwitch = 1;
				}
					
			
		}
		expDebug::dPrint('Class switch'.print_r($classSwitch,true),5);
		return $classSwitch;
	}catch (Exception $ex) {
		watchdog_exception('isClassSwitchAvail', $ex);
		expertusErrorThrow($ex);
	}
}

function courseConflictCheckInClsSwitch($userId, $courseId, $classId, $enrollId){
	try{
		$selectStmt = db_select('slt_enrollment', 'enr');
		$selectStmt->condition('enr.user_id', $userId);
		$selectStmt->condition('enr.course_id', $courseId);
		$selectStmt->condition('enr.id', $enrollId, '!=');
		$selectStmt->condition('enr.reg_status', array('lrn_crs_reg_cnf','lrn_crs_reg_rsv','lrn_crs_reg_wtl','lrn_crs_reg_ppm') ,'IN');
		$selectStmt->addExpression('COUNT(1)', 'Count');
		expDebug::dPrintDBAPI(' $selectStmtcourseConflictCheckInClsSwitch = ' , $selectStmt);
		// Execute query
		$result = $selectStmt->execute()->fetchField();
		expDebug::dPrint(' $result = ' . print_r($result, true) , 3);
		return $result;
	}catch (Exception $ex) {
		watchdog_exception('courseConflictCheckInClsSwitch', $ex);
		expertusErrorThrow($ex);
	}
}
function getAlternativeEventStatus($classId,$courseId){
	try{

		$timeZone = date_default_timezone(false);
		$defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;

		$select = db_select('slt_course_template', 'course');
		$select->leftJoin('slt_course_class', 'class','course.id = class.course_id AND class.id != :enroll_classid',array('enroll_classid' =>$classId));//AND class.id = :enroll_classid',array('enroll_classid' =>$classId)
		$select->leftJoin('slt_course_class_session', 'ses', 'ses.class_id=class.id and ses.id = (select id from slt_course_class_session where class_id=class.id order by start_date,start_time limit 1)');
		$select->addField('ses', 'timezone','timezone');
		$select->addExpression('COUNT(1)', 'count');
		$select->addExpression("CONCAT( DATE_FORMAT(ses.start_date, _utf8'%Y-%m-%d'),' ',ses.start_time,':00')", 'session_end_datetime');
		$select->condition('class.id', $classId,'!=');
		$select->condition('class.course_id', $courseId,'=');
		$select->condition('course.status','lrn_crs_sts_atv','=');
		$select->groupBy("ses.course_id , class.id having session_end_datetime > IFNULL(CONVERT_TZ(now(), '".$defaultTimezone."' ,(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone)),NOW()) or session_end_datetime is null");// Added by Shobana for #0022196
		expDebug::dPrintDBAPI(' selectgetAlternativeEventStatus = ' , $select);
		$selectcount = $select->execute()->fetchAll();
		return $selectcount;
	}catch (Exception $ex) {
		watchdog_exception('getAlternativeEventStatus', $ex);
		expertusErrorThrow($ex);
	}
}
/*
 * function for checking dedicated class
 */
function checkDedicatedClassMyEnroll($classId,$courseId) {
	try{
		$selectStmt = db_select('slt_course_class','cls');
		$selectStmt->addExpression('COUNT(1)','id');
		$selectStmt->condition('cls.id',$classId,'<>');
		$selectStmt->condition('cls.course_id',$courseId,'=');
		$selectStmt->condition('cls.status','lrn_cls_sts_atv','=');
		$selectStmt->where('cls.dedicated_class_flag = \'N\' or cls.dedicated_class_flag is null');
		expDebug::dPrintDBAPI('checking dedicated classes',$selectStmt);
		$resultCnt = $selectStmt->execute()->fetchField();
		return $resultCnt;
	}catch (Exception $ex) {
		watchdog_exception('checkDedicatedClassMyEnroll', $ex);
		expertusErrorThrow($ex);
	}
}
/*
 * function for checking whether the class is priced class or not
 */
function checkPriceForSwitchClass($classId,$courseId) {
	try{
		$selectStmt = db_select('slt_course_class','cls');
		$selectStmt->addField('cls','price','price');
		$selectStmt->condition('cls.id',$classId,'=');
		$selectStmt->condition('cls.course_id',$courseId,'=');
		$selectStmt->where('cls.dedicated_class_flag = \'N\' or cls.dedicated_class_flag is null');
		expDebug::dPrintDBAPI('checkPriceForSwitchClass',$selectStmt);
		$resultCnt = $selectStmt->execute()->fetchField();
		return $resultCnt;
	}catch (Exception $ex) {
		watchdog_exception('checkPriceForSwitchClass', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * Returns the site timezone
 */
function defaultSiteTimeZone($callFromAPI=0){
	try{

		$dfTz = cache_get('sys_default_tz')->data;
		if($dfTz && !$callFromAPI)
			return $dfTz;
		$sysTimezone = date_default_timezone(FALSE);
		$selTimeZone = db_select('slt_profile_list_items', 'prf');
		$selTimeZone->addField('prf','code','timezone_code');
		$selTimeZone->condition('prf.attr2', $sysTimezone);
		expDebug::dPrintDBAPI(' $selTimeZone obj = ' , $selTimeZone);
		$defaultTimezone['time_zone'] = $selTimeZone->execute()->fetchField();
		$defaultTimezone['sysTimezone'] = $sysTimezone;
		expDebug::dPrint('param change==>>'.print_r($defaultTimezone,true),4);
		cache_set('sys_default_tz',$defaultTimezone);
		return $defaultTimezone;
	}catch(Exception $ex) {
		watchdog_exception('defaultSiteTimeZone', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * Checks whether a string ends with a substring
 */
function endswith($string, $substr) {
	try{
		$strlen = strlen($string);
		$substrlen = strlen($substr);
		if ($substrlen == 0) return true;
		if ($substrlen > $strlen) return false;
		return substr_compare($string, $substr, -$substrlen) === 0;
		// return (substr($string, -$substrlen) === $substr); //alternative method
	}catch (Exception $ex) {
		watchdog_exception('endswith', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * Function to handle SQL injection
 * Returns 0 if the value is not numeric
 */
function stripQuotes($val){
	try{
		if(!is_numeric($val)){
			return 0;
		}
		return $val;
	}catch (Exception $ex) {
		watchdog_exception('stripQuotes', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * Function to handle SQL injection
 * Returns value without single and double quotes
 */
function stripApostrophe($val){
	try{
		$val = preg_replace('/\'/', '', $val);
		$val = preg_replace('/\"/', '', $val);
		return $val;
	}catch (Exception $ex) {
		watchdog_exception('stripApostrophe', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * Function to handle SQL injection
 * Strips the value other than Alpha,_,|
 */
function stripRegStatus($val,$from = ''){
	try{
		//Modified by Priya for #0033626
		// Hyphen char added for #0036617
		if (strlen($val) == strlen(utf8_decode($val))) { // Fixed for the multi language search Fix #0044189
			if($from == 'Assesment'){ // For Only Assesment To Eliminate | Pipeline Symbol also #0038953.
				return 	preg_replace('/[^a-zA-Z0-9,.@\'\_\-()&\":;%!^`*{}[]$ \/]/si' , '' , $val);
			}else{
				return 	preg_replace('/[^a-zA-Z0-9,.@\'\_\-()&\":;%|!^`*{}[]$ |\/~]/si' , '' , $val);
				//return 	preg_replace('/[^a-zA-Z0-9,.@\'\_\-()&\":;%$ |\/]/si' , '' , $val);
			}
		}else{
			return $val;
		}
	}catch (Exception $ex) {
		watchdog_exception('stripRegStatus', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * Function to handle SQL injection
 * Strips the value other than Alpha,_,|
 */
/* function secFixSpecialCharValidation($val,$from = ''){
	try{
		//Modified by Priya for #0033626
		// Hyphen char added for #0036617
		if (strlen($val) == strlen(utf8_decode($val))) { // Fixed for the multi language search Fix #0044189
			if($from == 'Assesment'){ // For Only Assesment To Eliminate | Pipeline Symbol also #0038953.
				return 	preg_replace('/[^a-zA-Z0-9,.@\'\_\-()&\":\[\[\]\]!*;%$ \/]/si' , '' , $val); // Modified for #0053661
			}else{
				return 	preg_replace('/[^a-zA-Z0-9,.@\'\_\-()&\":\[\[\]\]!*;%|$ |\/~]/si' , '' , $val); // Modified for #0053661
				//return 	preg_replace('/[^a-zA-Z0-9,.@\'\_\-()&\":;%$ |\/]/si' , '' , $val);
			}
		}else{
			return $val;
		}
	}catch (Exception $ex) {
		watchdog_exception('stripRegStatus', $ex);
		expertusErrorThrow($ex);
	}
} */

/*
 * Function to handle SQL injection
 *  If argument is passed as a string
 */
function orgValidate($val){
	try{
		if(empty($val))
			return $val;

		if(!is_numeric($val)){
			throw new Exception('Invalid argument');
		}
		return $val;
	}catch (Exception $ex) {
		watchdog_exception('orgValidate', $ex);
		expertusErrorThrow($ex);
	}
}

function getNodeIdResults($entityId,$entityType){
	try{
		// Select the DB table
		$select = db_select('slt_node_learning_activity', 'nla');

		// Select fields to fetch
		$select->addField('nla',	'node_id', 'node_id');

		// Set the condition(s)
		$select->condition('nla.entity_id', $entityId);

		$select->condition('nla.entity_type', $entityType);

		// Execute query and fetch the single values
		$nodeId  = $select->execute()->fetchField();

		return $nodeId;
	}catch (Exception $ex) {
		watchdog_exception('getNodeIdResults', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * function to check whether the class is dedicated or not
 */
function is_dedicated($classId){
	try{
		$select = db_select('slt_course_class', 'cls');
		$select->addField('cls','id','id');
		$select->condition('cls.dedicated_class_flag','Y');
		$select->condition('cls.id',$classId);
		expDebug::dPrintDBAPI('$select', $select);
		$result = $select->execute()->fetchField();
		expDebug::dPrint('Dedicated result: '.print_r($result, true),3);
		return $result;
	}catch (Exception $ex) {
		watchdog_exception('is_dedicated', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * Get list of TimeZone in slt_profile_list_items
 */
function _getTimeZone(){
	try {
		$select = db_select('slt_profile_list_items', 'spfa');
		$select->join('slt_profile_list_items','spfb','spfa.parent_id = %alias.id');
		$select->addField('spfa', 'code','zonecode');
		$select->addField('spfa', 'name','zonename');
		$select->condition('spfb.code', 'cre_sys_tmz');
		$timezone = $select->execute()->fetchAll();
		return $timezone;
	} catch (Exception $ex) {
		watchdog_exception('_getTimeZone', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * function thats returns true or false whether to show completed status or not in tproster
 * */
function isSessionStartForTP($programId){
	try {
		$timeZone = date_default_timezone(false);
		$defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;

		$select = db_select('slt_program','pgm');
		$select->leftJoin('slt_module_crs_mapping', 'crs', 'crs.program_id = pgm.id');
		$select->leftJoin('slt_course_class_session', 'ses', 'ses.course_id = crs.course_id');
		$select->leftJoin('slt_profile_list_items', 'prof', 'ses.timezone = prof.code');
		$select->addExpression("IF(CONCAT(DATE_FORMAT(MIN(ses.start_date),'%Y-%m-%d'),' ', TIME_FORMAT(ses.start_time,'%H:%i')) > CONVERT_TZ(NOW(),'".$defaultTimezone."',prof.attr2), FALSE, TRUE)", 'sessionFlag');
		$select->condition('pgm.id',$programId);
		$select->groupBy("ses.id  having sessionFlag ='0'");
		expDebug::dPrintDBAPI('$select = ' , $select);
		$sessionFlag = $select->execute()->fetchField();

		expDebug::dPrint('$sessionFlag  = ' .$sessionFlag, 4);
		$sessionFlagTp = $sessionFlag == '' ? true : false;
		expDebug::dPrint('$sessionFlagTp  = ' .$sessionFlagTp, 4);
		return $sessionFlagTp;
	} catch (Exception $ex) {
		watchdog_exception('isSessionStartForTP', $ex);
		expertusErrorThrow($ex);
	}
}

function isSessionStartForTPinClassLevel($enrollId){
    try {
        $timeZone = date_default_timezone(false);
        $defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;

        $select = db_select('slt_enrollment','enr');
        $select->leftJoin('slt_course_class_session', 'ses', 'enr.course_id = ses.course_id and enr.class_id = ses.class_id');
        $select->leftJoin('slt_master_enrollment', 'mas', 'mas.id = enr.master_enrollment_id');
        $select->leftJoin('slt_profile_list_items', 'prof', 'ses.timezone = prof.code');
        $select->addExpression("IF(CONCAT(DATE_FORMAT(MIN(ses.start_date),'%Y-%m-%d'),' ', TIME_FORMAT(ses.start_time,'%H:%i')) > CONVERT_TZ(NOW(),'".$defaultTimezone."',prof.attr2), FALSE, TRUE)", 'sessionFlag');
        $select->condition('mas.id',$enrollId);
        $select->groupBy("ses.id  having sessionFlag ='0'");
        expDebug::dPrintDBAPI('$select = ' , $select);
        $sessionFlag = $select->execute()->fetchField();

        expDebug::dPrint('$sessionFlag  = ' .$sessionFlag, 4);
        $sessionFlagTp = $sessionFlag == '' ? true : false;
        expDebug::dPrint('$sessionFlagTp  = ' .$sessionFlagTp, 4);
        return $sessionFlagTp;
    } catch (Exception $ex) {
        watchdog_exception('isSessionStartForTP', $ex);
        expertusErrorThrow($ex);
    }
}

/**
 * Adds ical to stored procedure generated notification(s), these notifications have send_status 'C'
 */
function addICalToNotification($enrId = null) {
	try {
		// Fetch the notification to which ical details are to be added
		$selStmt = db_select('slt_notification', 'noti');
		$selStmt->addField('noti', 'id' , 'id');
		$selStmt->addField('noti', 'msg_id', 'msg_id');
		$selStmt->addField('noti', 'ical', 'enr_id');
		$selStmt->condition('noti.send_status', 'C', '=');
		if (!empty($enrId)) {
			$selStmt->condition('noti.ical', $enrId, '=');
		}
		expDebug::dPrintDBAPI('$selStmt', $selStmt);
		$result = $selStmt->execute()->fetchAll();
		expDebug::dPrint('$result = ' . print_r($result, true), 4);

		// Fetch the iCal details and add it to the notification
		if (!empty($result)) {
			foreach ($result as $noti) {
				$iCal = null;
				switch ($noti->msg_id) {
					case 'register':
					case 'pay_confirm_admin':
					case 'waitlist_promo':
						$iCal = getPublishICal($noti->enr_id, 'Class');
						break;
					case 'class_cancel':
						$iCal = getCancelICal($noti->enr_id, 'Class');
						break;
					default:
						throw new Exception("ical stored procedure support: ical for $noti->msg_id notification is not getting generated");
						break;
				} //end switch

				if (!empty($iCal)) {
					$fields = array(
							'ical' => $iCal,
							'send_status' => 'N',
					);
					$updStmt = db_update('slt_notification');
					$updStmt->fields($fields);
					$updStmt->condition('id', $noti->id, '=');
					$updStmt->condition('send_status', 'C', '='); // before updating encertain the record is not already updated by another php thread
					expDebug::dPrintDBAPI('$updStmt', $updStmt);
					$updStmt->execute();
				}
			} //end foreach
		}
	}
	catch (Exception $ex) {
		watchdog_exception('addICalToNotification', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Fetches the list of tags for the specified entity types
 */
function getTags($entityTypesList = array(), $limit = null) {
	try {
		if (!empty($entityTypesList)) {
			if(in_array("Class", $entityTypesList))
				$entityTypesList[] = "Course";
			$selectStmt = db_select('slt_tag_entity', 'te');
			$selectStmt->leftJoin('slt_tagdefn', 'td', 'te.tagid = td.id');
			$selectStmt->condition('td.entity_type', $entityTypesList, 'IN');
			$selectStmt->condition('te.entity_type', $entityTypesList, 'IN');
			if(count($entityTypesList) == 1 && $entityTypesList[0] == 'Course'){
				$selectStmt->leftjoin('slt_course_template', 'crs','crs.id = te.entity_id and te.entity_type = \'Course\' ');
				filterByUserAccessPrivileges($selectStmt, 'crs','',1,'cre_sys_obt_crs','','crs');
			}elseif(count($entityTypesList) == 1 &&  $entityTypesList[0] == 'Class'){
				$selectStmt->leftjoin('slt_course_class', 'cls','cls.id = te.entity_id and te.entity_type = \'Class\' ');
				filterByUserAccessPrivileges($selectStmt, 'cls','',1,'cre_sys_obt_cls','','cls');
			}elseif(count($entityTypesList) == 1 &&  $entityTypesList[0] == 'Content'){
				$selectStmt->leftjoin('slt_content_master', 'cnt','cnt.id = te.entity_id and te.entity_type = \'Content\' ');
		  	filterByUserAccessPrivileges($selectStmt, 'cnt','',1,'cre_sys_obt_cnt','','cnt');
			}elseif(count($entityTypesList) == 1 &&  $entityTypesList[0] == 'sry_det_typ_sry_qus'){
				$selectStmt->leftjoin('slt_survey_questions', 'qus','qus.id = te.entity_id and te.entity_type = \'sry_det_typ_sry_qus\' ');
	  		filterByUserAccessPrivileges($selectStmt, 'qus','',1,'sry_det_typ_sry_qus','','qus');
			}elseif(count($entityTypesList) == 1 &&  $entityTypesList[0] == 'sry_det_typ_ass_qus'){
		  	$selectStmt->leftjoin('slt_survey_questions', 'qus','qus.id = te.entity_id and te.entity_type = \'sry_det_typ_ass_qus\' ');
	  		filterByUserAccessPrivileges($selectStmt, 'qus','',1,'sry_det_typ_ass_qus','','qus');
			}else{
				//$selectStmt->condition('te.entity_type',array('Certification','Curricula', 'Learning Plan'));
				$selectStmt->leftjoin('slt_program', 'prg','prg.id = te.entity_id and te.entity_type IN (\'Certification\',\'Curricula\', \'Learning Plan\') ');
				filterByUserAccessPrivileges($selectStmt, 'prg','',1,'cre_sys_obt_trp','','prg');
		}
		}
		else {
			$selectStmt = db_select('slt_tagdefn', 'td');
			$selectStmt->leftjoin('slt_tag_entity', 'ent','ent.tagid=td.id ');
			$selectStmt->condition('td.entity_type','Content','!=');
			$selectStmt->condition('ent.entity_type','Content','!=');
		}
		$selectStmt->isNotNull('td.activity_count');
                    $selectStmt->condition('td.activity_count', 0, '>');

		$selectStmt->addField('td', 'tagname', 'tagname');
		$selectStmt->addField('td', 'activity_count', 'activity_count');

		$selectStmt->groupBy('te.tagid');
		$selectStmt->orderBy('activity_count', 'DESC');
		$selectStmt->orderBy('tagname', 'ASC');



		expDebug::dPrintDBAPI('$selectStmt for tag based on activity count', $selectStmt);

		if(!isset($_REQUEST['apiname'])){
			if (!empty($limit)) {
				$selectStmt->range(0, $limit);
			}


		$selectTag1 = $selectStmt->extend("ExpertusSelectQueryExtender");
		$selectTag1->setIndex(array('td'=>'sli_tagdfn_cnt'));


		$selectTag = db_select($selectTag1,'tt');
		$selectTag->addField('tt','tagname','tagname');
		$selectTag->addExpression('SUM(tt.activity_count)', 'activity_count');
		//$selectTag->addField('tt','activity_count','activity_count');
		$selectTag->groupBy('tagname');
		$selectTag->orderBy('tagname','ASC');
		$selectTag1 = $selectTag->extend("ExpertusSelectQueryExtender");
		expDebug::dPrintDBAPI('$select tag query order by name', $selectTag1);

		$tagsList = $selectTag1->execute()->fetchAllKeyed();

		}else{
			$start = $limit * $_REQUEST['page'] - $limit;
			if($start < 0) {
				$start = 0;
			}
			$selectStmt->range($start,$limit);
			$tagsList = $selectStmt->execute()->fetchAll();
		}
		expDebug::dPrint('$tagsList = ' . print_r(count($tagsList), true));

	 return $tagsList;

	}
	catch (Exception $ex) {
		watchdog_exception('getTags', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Fetches the weighted list of tags for the specified entity types
 * If the entity types list is empty fetches all the tags in db
 * However, the number of tags fetched from db are limited by exp_sp.ini variable tags_cloud_limit (default: 10)
 */
function getWeightedTags($entityTypesList = array(), $steps = 8) {
	try {
		//37163: Tag cloud feature to be refined as per Murali's feedback
		/*	// Limit the number of tags
		 $tagsLimit = 100;

		 // Allow override through exp_sp.ini
		 require_once('sites/all/services/GlobalUtil.php');
		 $gutil = new GlobalUtil();
		 $expConfig = $gutil->getConfig();
		 $configTagsLimit = trim($expConfig['tags_cloud_limit']);
		 if (!empty($configTagsLimit)) {
			$tagsLimit = $configTagsLimit;
			}
			*/
		$tagsList = getTags($entityTypesList, 70);
		expDebug::dPrint('$tagsList = ' . print_r($tagsList, true), 4);

		// Compute weight of each tag, which is used as the font size in the tag cloud. Code taken from Drupal 7 tagclouds module.
		$min = 1e9;
		$max = -1e9;
		foreach ($tagsList as $tagName => $activityCount) {
			$tagsList[$tagName] = log($activityCount);
			$min = min($min, $tagsList[$tagName]);
			$max = max($max, $tagsList[$tagName]);
		}
		expDebug::dPrint('$tagsList after weightcount is calculated = ' . print_r($tagsList, true), 4);
		expDebug::dPrint('$max = '. $max, 4);
		expDebug::dPrint('$min = '. $min, 4);

		// Note: we need to ensure the range is slightly too large to make sure even
		// the largest element is rounded down.
		$range = max(.01, $max - $min) * 1.0001;
		expDebug::dPrint('$range = '. $range, 4);
		foreach ($tagsList as $tagName => $value) {
			$tagsList[$tagName] = 1 + floor($steps * ($value - $min) / $range);
		}

		expDebug::dPrint('$tagsList after weight is calculated = ' . print_r($tagsList, true), 4);
		return $tagsList;
	}
	catch (Exception $ex) {
		watchdog_exception('getWeightedTags', $ex);
		expertusErrorThrow($ex);
	}
}

function learnerGrpMap(&$selectStmt,$alisename='',$entity_type='',$page='',$cron='',$update='',$userId,&$uacond){
	if($page == 'Class'){
		expDebug::dPrint("class page  ".$page);
		// Commented by Shobana.N  for #0049456
		// Reason : No need to check the course if there is no access set to class.
	/*	$classsrQuery = 'if((select count(1) from slt_group_mapping grp_map where grp_map.entity_id = '.$alisename.'.id AND grp_map.entity_type = \'cre_sys_obt_cls\')>0,(group_map.entity_id='.$alisename.'.id
		AND group_map.entity_type = \'cre_sys_obt_cls\'),(group_map.entity_id='.$alisename.'.course_id AND group_map.entity_type = \'cre_sys_obt_crs\'))';*/

		$classsrQuery = 'group_map.entity_id='.$alisename.'.id AND group_map.entity_type = \'cre_sys_obt_cls\'';
		$classsrQuery1 = 'usrmap.entity_id='.$alisename.'.id AND usrmap.entity_type = \'cre_sys_obt_cls\'';
		$uacond1 = '(('.$classsrQuery1.') and usrmap.access_type = 0)';
		$uacond = $uacond1;
		if($cron != 'cron'){
			$uacond1 .= ' and usrmap.user_id = \''.$userId.'\'';
		}
		
		$selectStmt->leftJoin('slt_user_access_mapping','usrmap',$uacond1);
		$selectStmt->leftJoin('slt_group_mapping','group_map','('.$classsrQuery.') and group_map.group_type = 0  AND \'cre_sec_sts_atv\' = (SELECT status FROM slt_groups where id = group_map.group_id)');
		$selectStmt->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =0)');
		$selectStmt->leftJoin('slt_group_attributes','grpatt','catacs.id=grpatt.group_id');
		if($cron == 'cron'){
			$selectStmt->addField('catacs', 'id','groupId');
		/*	$selectStmt->leftJoin('slt_person','person','(if(FIND_IN_SET(person.id,catacs.added_users)>0,1=1,(catacs.removed_users IS NULL OR FIND_IN_SET(person.id,catacs.removed_users) <= 0)
AND( (if(catacs.country=\'All\',person.country!=\'\',(catacs.country IS NULL)) OR (catacs.country=\'\')) OR (FIND_IN_SET(person.country, catacs.country)) )
AND( (if(catacs.department=\'All\',person.dept_code!=\'\',(catacs.department IS NULL)) OR (catacs.department=\'\')) OR (FIND_IN_SET(person.dept_code, catacs.department)) )
AND( (if(catacs.employment_type=\'All\',person.employment_type!=\'\',(catacs.employment_type IS NULL)) OR (catacs.employment_type=\'\')) OR (FIND_IN_SET(person.employment_type, catacs.employment_type)) )
AND( (if(catacs.is_instructor !=\'N\',person.is_instructor=\'Y\',1=1)) )AND( (if(catacs.is_manager !=\'N\',person.is_manager=\'Y\',1=1)) )
AND( (if(catacs.job_role=\'All\',person.job_role!=\'\',(catacs.job_role IS NULL)) OR (catacs.job_role=\'\')) OR (FIND_IN_SET(person.job_role, catacs.job_role)) )
AND( (if(catacs.language=\'All\',person.preferred_language!=\'\',(catacs.language IS NULL)) OR (catacs.language=\'\')) OR (FIND_IN_SET(person.preferred_language, catacs.language)) )
AND( (if(catacs.org_id=\'All\',person.org_id!=\'\',(catacs.org_id IS NULL)) OR (catacs.org_id=\'\')) OR (FIND_IN_SET(person.org_id, catacs.org_id)) )
AND( (if(catacs.state=\'All\',person.state!=\'\',(catacs.state IS NULL)) OR (catacs.state=\'\')) OR (FIND_IN_SET(person.state, catacs.state)) )
AND( (if(catacs.user_type=\'All\',person.user_type!=\'\',(catacs.user_type IS NULL)) OR (catacs.user_type=\'\')) OR (FIND_IN_SET(person.user_type, catacs.user_type)) )
AND( (group_map.group_type = 0) OR (group_map.group_id IS NULL))) )');
			$selectStmt->leftJoin('slt_enrollment','enroll','enroll.class_id = '.$alisename.'.id AND enroll.course_id = '.$alisename.'.course_id AND enroll.user_id = person.id
AND enroll.reg_status IN  (\'lrn_crs_reg_cnf\', \'lrn_crs_reg_ppm\', \'lrn_crs_reg_wtl\')');

			//$selectStmt->addExpression('GROUP_CONCAT(distinct person.id)','userids');
			$selectStmt->addField('person', 'id','userids');
			if(!empty($update)){
				$selectStmt->addField('enroll', 'id','eid');
			}else{
				$selectStmt->leftJoin('slt_autoregister_temp','temp','temp.cls_id = '.$alisename.'.id and temp.userids = person.id');
			}*/
		}

	}if($page == 'Program'){
		$entity_type = '(\'cre_sys_obt_crt\',\'cre_sys_obt_cur\',\'cre_sys_obt_trn\')';
		$uacond1 = '(usrmap.entity_id='.$alisename.'.id AND usrmap.entity_type IN (\'cre_sys_obt_crt\',\'cre_sys_obt_cur\',\'cre_sys_obt_trn\') and usrmap.access_type = 0 )';
		$uacond = $uacond1;
		if($cron != 'cron'){
			$uacond1 .= ' and usrmap.user_id = \''.$userId.'\'';
		}
		expDebug::dPrint("program page  ".$page);
		$selectStmt->leftJoin('slt_user_access_mapping','usrmap',$uacond1);
		$selectStmt->leftJoin('slt_group_mapping','group_map','group_map.entity_id='.$alisename.'.id AND group_map.entity_type IN (\'cre_sys_obt_crt\',\'cre_sys_obt_cur\',\'cre_sys_obt_trn\') and group_map.group_type = 0 AND \'cre_sec_sts_atv\' = (SELECT status FROM slt_groups where id = group_map.group_id)');
		$selectStmt->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =0)');
		$selectStmt->leftJoin('slt_group_attributes','grpatt','catacs.id=grpatt.group_id');
		if($cron == 'cron'){
			$selectStmt->addField('catacs', 'id','groupId');
		/*	$selectStmt->leftJoin('slt_person','person','(if(FIND_IN_SET(person.id,catacs.added_users)>0,1=1,(catacs.removed_users IS NULL OR FIND_IN_SET(person.id,catacs.removed_users) <= 0)
AND( (if(catacs.country=\'All\',person.country!=\'\',(catacs.country IS NULL)) OR (catacs.country=\'\')) OR (FIND_IN_SET(person.country, catacs.country)) )
AND( (if(catacs.department=\'All\',person.dept_code!=\'\',(catacs.department IS NULL)) OR (catacs.department=\'\')) OR (FIND_IN_SET(person.dept_code, catacs.department)) )
AND( (if(catacs.employment_type=\'All\',person.employment_type!=\'\',(catacs.employment_type IS NULL)) OR (catacs.employment_type=\'\')) OR (FIND_IN_SET(person.employment_type, catacs.employment_type)) )
AND( (if(catacs.is_instructor !=\'N\',person.is_instructor=\'Y\',1=1)) )AND( (if(catacs.is_manager !=\'N\',person.is_manager=\'Y\',1=1)) )
AND( (if(catacs.job_role=\'All\',person.job_role!=\'\',(catacs.job_role IS NULL)) OR (catacs.job_role=\'\')) OR (FIND_IN_SET(person.job_role, catacs.job_role)) )
AND( (if(catacs.language=\'All\',person.preferred_language!=\'\',(catacs.language IS NULL)) OR (catacs.language=\'\')) OR (FIND_IN_SET(person.preferred_language, catacs.language)) )
AND( (if(catacs.org_id=\'All\',person.org_id!=\'\',(catacs.org_id IS NULL)) OR (catacs.org_id=\'\')) OR (FIND_IN_SET(person.org_id, catacs.org_id)) )
AND( (if(catacs.state=\'All\',person.state!=\'\',(catacs.state IS NULL)) OR (catacs.state=\'\')) OR (FIND_IN_SET(person.state, catacs.state)) )
AND( (if(catacs.user_type=\'All\',person.user_type!=\'\',(catacs.user_type IS NULL)) OR (catacs.user_type=\'\')) OR (FIND_IN_SET(person.user_type, catacs.user_type)) )
AND( (group_map.group_type = 0) OR (group_map.group_id IS NULL))) )');
			$selectStmt->leftJoin('slt_master_enrollment','master_enroll','master_enroll.program_id = prm.id and master_enroll.user_id = person.id
AND ((master_enroll.overall_status IN  (\'lrn_tpm_ovr_cmp\', \'lrn_tpm_ovr_enr\', \'lrn_tpm_ovr_inp\', \'lrn_tpm_ovr_ppv\', \'lrn_tpm_ovr_ppm\', \'lrn_tpm_ovr_exp\', \'lrn_tpm_ovr_wtl\')))');
			$selectStmt->leftJoin('slt_autoregister_temp','temp','temp.cls_id = '.$alisename.'.id and temp.userids = person.id');

			//$selectStmt->addExpression('GROUP_CONCAT(distinct person.id)','userids');
			$selectStmt->addField('person', 'id','userids');*/
		}
	}if($page == 'Course'){
		// Below change done by Vincent for #0066758 on July 23, 2016
		// TODO: This should be changed in future once the flat structure is implement for performance tuning
		//$selectStmt->leftJoin('slt_group_mapping','group_map','((group_map.entity_id='.$alisename.'.id AND group_map.entity_type = \''.$entity_type.'\') OR (group_map.entity_id=cls.id AND group_map.entity_type = \'cre_sys_obt_cls\')) and group_map.group_type = 0 AND \'cre_sec_sts_atv\' = (SELECT status FROM slt_groups where id = group_map.group_id)');
		
		/* Added/changed by ganeshbabuv on Feb 13th 2016 for  date, country and location filters issues #0071712 
		- Not showing the non-access class to user via Catalog Search API */ 
		
		$tmp_startdate = getRequestValue('startdate');
		$tmp_enddate = getRequestValue('enddate');
		$tmp_locationtype = getRequestValue('location');
		$tmp_contrytype = isset($_GET['cy_type']) ? $_GET['cy_type']:'';
		
		$cls_aliase='cls';
		if((!empty($tmp_startdate) || !empty($tmp_enddate) || !empty($tmp_locationtype) || !empty($tmp_contrytype)) && trim($_REQUEST['apiname']) == 'CatalogSearchAPI'){ //GaneshFilter
		  $cls_aliase='clsRollup';
		}
		$uacond1 = '((usrmap.parent_id=concat('.$alisename.'.id, \'-\', IFNULL('.$cls_aliase.'.id,0)) AND usrmap.parent_type = \'learning\') and usrmap.access_type = 0)';
		$uacond = $uacond1;
		if($cron != 'cron'){
			$uacond1 .= ' and usrmap.user_id = \''.$userId.'\'';
		}
		$selectStmt->leftJoin('slt_user_access_mapping','usrmap',$uacond1);
		$selectStmt->leftJoin('slt_group_mapping','group_map','(group_map.parent_id=concat('.$alisename.'.id, \'-\', IFNULL('.$cls_aliase.'.id,0)) AND group_map.parent_type = \'learning\') and group_map.group_type = 0 AND \'cre_sec_sts_atv\' = (SELECT status FROM slt_groups where id = group_map.group_id)');
		$selectStmt->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =0)');
		$selectStmt->leftJoin('slt_group_attributes','grpatt','catacs.id=grpatt.group_id');
		if($cron == 'cron'){
			$selectStmt->leftJoin('slt_person','person','((FIND_IN_SET(person.id,catacs.added_users)>0) OR (catacs.removed_users IS NULL OR FIND_IN_SET(person.id,catacs.removed_users) <= 0) or((catacs.job_role = \'\') OR FIND_IN_SET(lpjm.job_role,catacs.job_role)>0 or (catacs.job_role IS NULL ))
					AND ((catacs.user_type = \'\') OR FIND_IN_SET(person.user_type,catacs.user_type) or (catacs.user_type IS NULL ))
					AND ((catacs.org_id = \'\') OR FIND_IN_SET(person.org_id,catacs.org_id) or  (catacs.org_id IS NULL ))
					AND ((catacs.employment_type = \'\') OR FIND_IN_SET(person.employment_type,catacs.employment_type) or  (catacs.employment_type IS NULL ))
					AND ((catacs.country = \'\') OR FIND_IN_SET(person.country,catacs.country) or  (catacs.country IS NULL ))
					AND ((catacs.state = \'\') OR FIND_IN_SET(person.state,catacs.state) or  (catacs.state IS NULL ))
					AND ((catacs.department = \'\') OR FIND_IN_SET(person.dept_code,catacs.department) or  (catacs.department IS NULL ))
					AND ((catacs.language = \'\') OR FIND_IN_SET(person.preferred_language,catacs.language) or  (catacs.language IS NULL ))
					AND ((catacs.is_instructor = \'\') OR FIND_IN_SET(person.is_instructor,catacs.is_instructor) or  (catacs.is_instructor IS NULL ))
					AND ((catacs.is_manager = \'\') OR FIND_IN_SET(person.is_manager,catacs.is_manager) or  (catacs.is_manager IS NULL )))
					AND (if(grpatt.id is not null,if(grpatt.on_or_after_start_date is not null ,DATE_FORMAT(grpatt.on_or_after_start_date,\'%Y-%m-%d\') <= DATE_FORMAT(per.hire_date,\'%Y-%m-%d\'),1=0) OR 
            		if(grpatt.on_or_before_start_date is not null, DATE_FORMAT(grpatt.on_or_before_start_date,\'%Y-%m-%d\') >= DATE_FORMAT(per.hire_date,\'%Y-%m-%d\'), 1=0) OR
					if(grpatt.between_start_date is not null ,DATE_FORMAT(per.hire_date,\'%Y-%m-%d\') between DATE_FORMAT(grpatt.between_start_date,\'%Y-%m-%d\') AND DATE_FORMAT(grpatt.between_end_date,\'%Y-%m-%d\'), 1=0),1=1)  )');
			$selectStmt->leftjoin('slt_person_jobrole_mapping','lpjm','lpjm.user_id = person.id');
			//$selectStmt->addExpression('GROUP_CONCAT(distinct person.id)','userids');
			$selectStmt->addField('person', 'id','userids');
		}
	}
	if($page == 'lp'){
		$uacond1 = '(usrmap.entity_id=ms.pk_id AND IF(ms.type=\'Class\', usrmap.entity_type = \'cre_sys_obt_cls\', usrmap.entity_type IN (\'cre_sys_obt_trn\', \'cre_sys_obt_crt\', \'cre_sys_obt_cur\')) and usrmap.access_type = 0)';
		$uacond = $uacond1;
		if($cron != 'cron'){
			$uacond1 .= ' and usrmap.user_id = \''.$userId.'\'';
		}
		$selectStmt->leftJoin('slt_user_access_mapping','usrmap',$uacond1);
		$selectStmt->leftJoin('slt_group_mapping','group_map','group_map.entity_id=ms.pk_id AND IF(ms.type=\'Class\', group_map.entity_type = \'cre_sys_obt_cls\', group_map.entity_type IN (\'cre_sys_obt_trn\', \'cre_sys_obt_crt\', \'cre_sys_obt_cur\')) and group_map.group_type = 0 AND \'cre_sec_sts_atv\' = (SELECT status FROM slt_groups where id = group_map.group_id)');
		$selectStmt->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =0)');
		$selectStmt->leftJoin('slt_group_attributes','grpatt','catacs.id=grpatt.group_id');
	}else if($page == 'Spotlight'){
		$uacond1 = '(((usrmap.entity_id=sct.id AND usrmap.entity_type = \'cre_sys_obt_crs\') OR usrmap.entity_id=sp.id AND usrmap.entity_type IN (\'cre_sys_obt_cur\',\'cre_sys_obt_crt\',\'cre_sys_trn\')) and usrmap.access_type = 0)';
		$uacond = $uacond1;
		if($cron != 'cron'){
			$uacond1 .= ' and usrmap.user_id = \''.$userId.'\'';
		}
		$selectStmt->leftJoin('slt_user_access_mapping', 'usrmap', $uacond1);
		$selectStmt->leftJoin('slt_group_mapping', 'group_map', '((group_map.entity_id=sct.id AND group_map.entity_type = \'cre_sys_obt_crs\') OR group_map.entity_id=sp.id AND group_map.entity_type IN (\'cre_sys_obt_cur\',\'cre_sys_obt_crt\',\'cre_sys_trn\')) AND \'cre_sec_sts_atv\' = (SELECT status FROM slt_groups where id = group_map.group_id)');
		$selectStmt->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =0)');
		$selectStmt->leftJoin('slt_group_attributes','grpatt','catacs.id=grpatt.group_id');
	}else if($page == 'Myteam'){
		// Below change done by Vincent for #0066758 on July 23, 2016
			// TODO: This should be changed in future once the flat structure is implement for performance tuning
		// RAM : this is Changes for This Ticket #0037832, when Myteam Assign and View Learning status is Not Showing, when a Access is Set For Course level.
		/*$classsrQuery = 'if((select count(1) from slt_group_mapping grp_map where grp_map.entity_id = ms.pk_id AND grp_map.entity_type = \'cre_sys_obt_cls\')>0,(group_map.entity_id=ms.pk_id
		AND group_map.entity_type = \'cre_sys_obt_cls\'),(group_map.entity_id=ms.parent_pk AND group_map.entity_type = \'cre_sys_obt_crs\'))';*/
		$classsrQuery = '(group_map.parent_id=concat(IFNULL(ms.parent_pk,ms.pk_id),\'-\',IF(ms.parent_pk IS NULL, 0, ms.pk_id)) AND group_map.parent_type = \'learning\')';
		$uacond1 = '(usrmap.parent_id = ms.mapping_id and usrmap.parent_type=ms.mapping_type and usrmap.access_type = 0)';
		$uacond = $uacond1;
		if($cron != 'cron'){
			$uacond1 .= ' and usrmap.user_id = \''.$userId.'\'';
		}
		$selectStmt->leftJoin('slt_user_access_mapping','usrmap',$uacond1);
		$selectStmt->leftJoin('slt_group_mapping','group_map','group_map.parent_id = ms.mapping_id and group_map.parent_type=ms.mapping_type and group_map.group_type = 0 AND \'cre_sec_sts_atv\' = (SELECT status FROM slt_groups where id = group_map.group_id)');
		$selectStmt->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =0)');
		$selectStmt->leftJoin('slt_group_attributes','grpatt','catacs.id=grpatt.group_id');
	}else if($page == 'discount'){
		$selectStmt->leftjoin('slt_groups', 'catacs',	'FIND_IN_SET(catacs.id,'.$alisename.'.group_id) and catacs.is_admin=0 and catacs.status=\'cre_sec_sts_atv\'');
		$selectStmt->leftJoin('slt_group_attributes','grpatt','catacs.id=grpatt.group_id');
	}	else if($page == 'Forum'){
		// Below change done by Vincent for #0066758 on July 23, 2016
			// TODO: This should be changed in future once the flat structure is implement for performance tuning
		//$selectStmt->leftJoin('slt_group_mapping','group_map','(IF((select count(1) from slt_group_mapping grp_map where grp_map.entity_id = crs.id AND grp_map.entity_type = \'cre_sys_obt_crs\')>0,(group_map.entity_id=crs.id AND group_map.entity_type = \'cre_sys_obt_crs\'),(group_map.entity_id=cls.id AND group_map.entity_type = \'cre_sys_obt_cls\'))) and group_map.group_type = 0 AND \'cre_sec_sts_atv\' = (SELECT status FROM slt_groups where id = group_map.group_id)');
		$uacond1 = '((usrmap.parent_id=concat(crs.id, \'-\', IFNULL(cls.id,0)) AND usrmap.parent_type = \'learning\') and usrmap.access_type = 0)';
		$uacond = $uacond1;
		if($cron != 'cron'){
			$uacond1 .= ' and usrmap.user_id = \''.$userId.'\'';
		}
		$selectStmt->leftJoin('slt_user_access_mapping','usrmap',$uacond1);
		$selectStmt->leftJoin('slt_group_mapping','group_map','(group_map.parent_id=concat(crs.id, \'-\', IFNULL(cls.id,0)) AND group_map.parent_type = \'learning\') and group_map.group_type = 0 AND \'cre_sec_sts_atv\' = (SELECT status FROM slt_groups where id = group_map.group_id)');
		$selectStmt->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =0)');
		$selectStmt->leftJoin('slt_group_attributes','grpatt','catacs.id=grpatt.group_id');
	}
	else if($page != 'Program' && $page != 'Course' && $page != 'Class' && $page != 'lp' && $page != 'Spotlight'){
		expDebug::dPrint("else page  ".$page);
		$selectStmt->leftjoin('slt_group_mapping', 'group_map', 'group_map.entity_id='.$alisename.'.id AND group_map.entity_type = \''.$entity_type.'\' and group_map.group_type = 0 AND \'cre_sec_sts_atv\' = (SELECT status FROM slt_groups where id = group_map.group_id)');
		$uacond1 = '(usrmap.entity_id='.$alisename.'.id AND usrmap.entity_type = \''.$entity_type.'\' and usrmap.access_type = 0)';
		$uacond = $uacond1; 
		if($cron != 'cron'){
			$uacond1 .= ' and usrmap.user_id = \''.$userId.'\'';
		}
		$selectStmt->leftjoin('slt_user_access_mapping', 'usrmap', $uacond1);
		$selectStmt->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =0)');
		$selectStmt->leftJoin('slt_group_attributes','grpatt','catacs.id=grpatt.group_id');
	}
}
function getProgramObjDetails($programId){
	try {
		$select = db_select('slt_program', 'prg');
		$select->addField('prg','object_type');
		$select->addField('prg','title');
		$select->addField('prg','code');
		$select->addField('prg','price');
		$select->addField('prg','object_type');
		$select->addField('prg','expires_in_value');
		$select->addField('prg','short_desc');
		$select->addField('prg','currency_type');
		$select->addField('prg','lang_code');
		$select->addField('prg','created_on');
		$select->addField('prg','published_on');
		$select->addField('prg','expires_in_unit');
		$select->condition('prg.id',$programId);
		// Execute the query
		expDebug::dPrintDBAPI("get program details",$select);
		$prgType = $select->execute()->fetchAll();
		return $prgType;
	} catch (Exception $ex) {
		watchdog_exception('getProgramObjDetails', $ex);
		expertusErrorThrow($ex);
	}
}

function adminGrpMap(&$select,$alisename='',$entity_type='',$page='',$createByAdmin='',$userId='',$accessType = ''){
	expDebug::dPrint('$page : '.$page , 4);
	$isGroup=false;
	if($page == 'Class'){
		// Below change done by Vincent for #0066758 on July 23, 2016
			// TODO: This should be changed in future once the flat structure is implement for performance tuning
		/*#47849: Classes are showing for a particular user when user doesn't have visible acces*/
		/*$classsrQuery = 'if((select count(1) from slt_group_mapping grp_map where grp_map.entity_id = '.$alisename.'.id AND grp_map.entity_type = \'cre_sys_obt_cls\')>0,(group_map.entity_id='.$alisename.'.id
		AND group_map.entity_type = \'cre_sys_obt_cls\'),(group_map.entity_id='.$alisename.'.course_id AND group_map.entity_type = \'cre_sys_obt_crs\'))';*/
		$classsrQuery = '(group_map.parent_id=concat('.$alisename.'.course_id, \'-\', IFNULL('.$alisename.'.id,0)) AND group_map.parent_type = \'learning\')';
		$select->leftJoin('slt_group_mapping','group_map','('.$classsrQuery.') and group_map.group_type = 1 AND \'cre_sec_sts_atv\' = (SELECT status FROM slt_groups where id = group_map.group_id)');
		$select->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =1)');
		$select->leftJoin('slt_group_attributes','grpatt','catacs.id=grpatt.group_id');
// 		$select->leftJoin('slt_owner_details','detail','detail.group_id = catacs.id and detail.owner_id = '.$userId.'');
		$select->leftJoin('slt_group_privilege','priv','priv.group_id = catacs.id and priv.page_code= \'cre_sys_obt_crs\'');
		$pageCode='cre_sys_obt_crs';
		$entity_type = 'cre_sys_obt_cls';
	}else if($page == 'CourseList'){
		/*#47849: Classes are showing for a particular user when user doesn't have visible acces*/
		/*$classsrQuery = 'if((select count(1) from slt_group_mapping grp_map where grp_map.entity_id = '.$alisename.'.id AND grp_map.entity_type = \'cre_sys_obt_cls\')>0,(group_map.entity_id='.$alisename.'.id
		AND group_map.entity_type = \'cre_sys_obt_cls\'),(group_map.entity_id='.$alisename.'.course_id AND group_map.entity_type = \'cre_sys_obt_crs\'))';*/
		// Below change done by Vincent for #0066758 on July 23, 2016
			// TODO: This should be changed in future once the flat structure is implement for performance tuning
		//$select->leftJoin('slt_group_mapping','group_map','((group_map.entity_id=crs.id AND group_map.entity_type = \''.$entity_type.'\') OR (group_map.entity_id=crscls.id AND group_map.entity_type = \'cre_sys_obt_cls\')) and group_map.group_type = 1 AND \'cre_sec_sts_atv\' = (SELECT status FROM slt_groups where id = group_map.group_id)');
		$select->leftJoin('slt_group_mapping','group_map','(group_map.parent_id=concat(crs.id, \'-0\') AND group_map.parent_type = \'learning\') and group_map.group_type = 1 AND \'cre_sec_sts_atv\' = (SELECT status FROM slt_groups where id = group_map.group_id)');
		$select->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =1)');
		$select->leftJoin('slt_group_attributes','grpatt','catacs.id=grpatt.group_id');
// 		$select->leftJoin('slt_owner_details','detail','detail.group_id = catacs.id and detail.owner_id = '.$userId.'');
		$select->leftJoin('slt_group_privilege','priv','priv.group_id = catacs.id and priv.page_code= \'cre_sys_obt_crs\'');
		$pageCode='cre_sys_obt_crs';
		//$entity_type = 'cre_sys_obt_cls';
	}else if($page == 'tp'){
		$pageCode = 'cre_sys_obt_trp';
		$entity_type = 'cre_sys_obt_trp';
		$alisename = ($alisename == '') ?  'prg': $alisename ;
		$select->leftJoin('slt_group_mapping','group_map','group_map.entity_id='.$alisename.'.id AND group_map.entity_type IN (\'cre_sys_obt_crt\',\'cre_sys_obt_cur\',\'cre_sys_obt_trn\',\'cre_sys_obt_trp\') and group_map.group_type = 1 AND \'cre_sec_sts_atv\' = (SELECT status FROM slt_groups where id = group_map.group_id)');
		$select->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =1)');
		$select->leftJoin('slt_group_attributes','grpatt','catacs.id=grpatt.group_id');
// 		$select->leftJoin('slt_owner_details','detail','detail.group_id = catacs.id and detail.owner_id = '.$userId.'');
		$select->leftJoin('slt_group_privilege','priv','priv.group_id = catacs.id and priv.page_code= \'cre_sys_obt_trp\'');
	}else if($page == 'Courseedit'){
		$select->leftJoin('slt_group_mapping','group_map','group_map.entity_id='.$alisename.'.id AND group_map.entity_type = \''.$entity_type.'\' and group_map.group_type = 1 AND \'cre_sec_sts_atv\' = (SELECT status FROM slt_groups where id = group_map.group_id)');
		$select->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =1)');
		$select->leftJoin('slt_group_attributes','grpatt','catacs.id=grpatt.group_id');
// 		$select->leftJoin('slt_owner_details','detail','detail.group_id = catacs.id and detail.owner_id = '.$userId.''); // Added owner detiails table join for #0050368
	}else if($page == 'Search'){
		// $searchQuery = '(if((select count(1) from slt_group_mapping grp_map where grp_map.entity_id = mas.pk_id)>0,(group_map.entity_id=mas.pk_id OR group_map.entity_id=mas.parent_pk ),
		// (group_map.entity_id=mas.parent_pk)))';
		//$select->leftJoin('slt_group_mapping','group_map','('.$searchQuery.') and group_map.group_type = 1');
		$select->leftJoin('slt_group_mapping','group_map','group_map.entity_id=mas.pk_id and group_map.group_type = 1 AND \'cre_sec_sts_atv\' = (SELECT status FROM slt_groups where id = group_map.group_id)');
		$select->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =1)');
		$select->leftJoin('slt_group_attributes','grpatt','catacs.id=grpatt.group_id');
// 		$select->leftJoin('slt_owner_details','detail','detail.group_id = catacs.id and detail.owner_id = '.$userId.'');
	}else if($page == 'location'){
		/*$select->leftJoin('slt_group_mapping','group_map','group_map.entity_id='.$alisename.'.pk_id AND group_map.entity_type = \''.$entity_type.'\' and group_map.group_type = 1');
		$select->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =1) and catacs.status =\'cre_sec_sts_atv\'');
		$select->leftJoin('slt_group_privilege','priv','priv.group_id = catacs.id and priv.page_code= \''.$entity_type.'\'');
		$pageCode = $entity_type;*/
		$select->leftJoin('slt_group_mapping','group_map','group_map.entity_id='.$alisename.'.id AND group_map.entity_type = \''.$entity_type.'\' and group_map.group_type = 1 AND \'cre_sec_sts_atv\' = (SELECT status FROM slt_groups where id = group_map.group_id)');
		$select->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =1) and catacs.status =\'cre_sec_sts_atv\'');
		$select->leftJoin('slt_group_attributes','grpatt','catacs.id=grpatt.group_id');
// 		$select->leftJoin('slt_owner_details','detail','detail.group_id = catacs.id and detail.owner_id = '.$userId.'');
		$select->leftJoin('slt_group_privilege','priv','priv.group_id = catacs.id and priv.page_code= \''.$entity_type.'\'');
		$pageCode = $entity_type;
	}else if($page == 'autocomplete'){
		if($entity_type ==''){
			$select->leftJoin('slt_group_mapping','group_map','group_map.entity_id='.$alisename.'.id and group_map.group_type = 1 and group_map.entity_type IN (\'cre_sys_obt_crt\',\'cre_sys_obt_cur\',\'cre_sys_obt_trn\')');
			$select->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =1) and catacs.status =\'cre_sec_sts_atv\'');
			$select->leftJoin('slt_group_attributes','grpatt','catacs.id=grpatt.group_id');
// 			$select->leftJoin('slt_owner_details','detail','detail.group_id = catacs.id and detail.owner_id = '.$userId.'');
		}else{
			$select->leftJoin('slt_group_mapping','group_map','group_map.entity_id='.$alisename.'.id and group_map.group_type = 1 and group_map.entity_type = \''.$entity_type.'\'');
			$select->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =1) and catacs.status =\'cre_sec_sts_atv\'');
			$select->leftJoin('slt_group_attributes','grpatt','catacs.id=grpatt.group_id');
// 			$select->leftJoin('slt_owner_details','detail','detail.group_id = catacs.id and detail.owner_id = '.$userId.'');
		}
	}else if($page == "people_setting"){
		$select->leftJoin('slt_group_mapping','group_map','group_map.entity_id='.$alisename.'.id AND group_map.entity_type IN(\'cre_usr_dpt\',\'cre_usr_etp\',\'cre_usr_jrl\',\'cre_usr_jtl\',\'cre_usr_ptp\') and group_map.group_type = 1 AND \'cre_sec_sts_atv\' = (SELECT status FROM slt_groups where id = group_map.group_id)');
		$select->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =1)');
		$select->leftJoin('slt_group_attributes','grpatt','catacs.id=grpatt.group_id');
// 		$select->leftJoin('slt_owner_details','detail','detail.group_id = catacs.id and detail.owner_id = '.$userId.'');
	}else if($page == "Group"){
		$isGroup=true;
		$select->leftJoin('slt_group_mapping','group_map','group_map.entity_id='.$alisename.'.id AND group_map.entity_type = \''.$entity_type.'\' and group_map.group_type = 1 AND \'cre_sec_sts_atv\' = (SELECT status FROM slt_groups where id = group_map.group_id)');
$select->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =1) ');
$select->leftJoin('slt_group_attributes','grpatt','catacs.id=grpatt.group_id');
// 		$select->leftJoin('slt_owner_details','detail','detail.group_id = catacs.id and detail.owner_id = '.$userId.'');
		if($accessType == 'admin'){
			$select->leftJoin('slt_group_privilege','priv','priv.group_id = catacs.id and priv.page_code= \''.$entity_type.'\'');// admin group
			$pageCode = $entity_type;
		}else if($accessType == 'learner'){
			$select->leftJoin('slt_group_privilege','priv','priv.group_id = catacs.id and priv.page_code= \'cre_sec_learner\'');// Learner group
			$pageCode = 'cre_sec_learner';
		}else{
			$select->leftJoin('slt_group_privilege','priv','priv.group_id = catacs.id and
							   if(grp.is_admin = 1, priv.page_code= \'cre_sec\',priv.page_code = \'cre_sec_learner\')');// both group
			$pageCode = 'cre_sec_learner';
		}
	}else if($page == "attach"){
		if($entity_type != ''){
			$select->leftJoin('slt_group_mapping','group_map','group_map.entity_id='.$alisename.'.id AND group_map.entity_type = \''.$entity_type.'\' and group_map.group_type = 1 AND \'cre_sec_sts_atv\' = (SELECT status FROM slt_groups where id = group_map.group_id)');
			$select->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =1) and catacs.status =\'cre_sec_sts_atv\'');
			$select->leftJoin('slt_group_attributes','grpatt','catacs.id=grpatt.group_id');
// 			$select->leftJoin('slt_owner_details','detail','detail.group_id = catacs.id and detail.owner_id = '.$userId.'');
	}else{
			$select->leftJoin('slt_group_mapping','group_map','group_map.entity_id='.$alisename.'.id AND group_map.entity_type IN (\'cre_sys_obt_crt\',\'cre_sys_obt_cur\',\'cre_sys_obt_trn\') and group_map.group_type = 1 AND \'cre_sec_sts_atv\' = (SELECT status FROM slt_groups where id = group_map.group_id)');
			$select->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =1) and catacs.status =\'cre_sec_sts_atv\'');
			$select->leftJoin('slt_group_attributes','grpatt','catacs.id=grpatt.group_id');
// 			$select->leftJoin('slt_owner_details','detail','detail.group_id = catacs.id and detail.owner_id = '.$userId.'');
		}

	}else if($page == "discount"){
		$select->leftJoin('slt_group_mapping','group_map','group_map.entity_id='.$alisename.'.discount_id AND group_map.entity_type = \''.$entity_type.'\' and group_map.group_type = 1');
		$select->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =1) and catacs.status =\'cre_sec_sts_atv\'');
		$select->leftJoin('slt_group_attributes','grpatt','catacs.id=grpatt.group_id');
// 		$select->leftJoin('slt_owner_details','detail','detail.group_id = catacs.id and detail.owner_id = '.$userId.'');
		$select->leftJoin('slt_group_privilege','priv','priv.group_id = catacs.id and priv.page_code= \''.$entity_type.'\'');
		$pageCode = $entity_type;
	}else if($page == "Order"){
		//$select->leftJoin('slt_group_mapping','group_map','group_map.entity_id='.$alisename.'.order_id AND group_map.entity_type = \''.$entity_type.'\' and group_map.group_type = 1');
          $select->leftJoin('slt_group_attributes','grpatt','catacs.id=grpatt.group_id');
	}else{
		expDebug::dPrint('Else loop is called');
		$select->leftJoin('slt_group_mapping','group_map','group_map.entity_id='.$alisename.'.id AND group_map.entity_type = \''.$entity_type.'\' and group_map.group_type = 1 AND \'cre_sec_sts_atv\' = (SELECT status FROM slt_groups where id = group_map.group_id)');
		$select->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =1)');
		$select->leftJoin('slt_group_attributes','grpatt','catacs.id=grpatt.group_id');
// 			$select->leftJoin('slt_owner_details','detail','detail.group_id = catacs.id and detail.owner_id = '.$userId.'');
		$select->leftJoin('slt_group_privilege','priv','priv.group_id = catacs.id and priv.page_code= \''.$entity_type.'\'');
		$pageCode = $entity_type;

	}
	$subAlis = ($isGroup===true)?$createByAdmin:'g';
	$pageCode1 = ($page=='Class') ? 'cre_sys_obt_crs' : $entity_type;
	$etypeCond = ($page != 'tp') ? 'gm.entity_type=\''.$entity_type.'\'' : 'gm.entity_type IN (\'cre_sys_obt_crt\',\'cre_sys_obt_cur\',\'cre_sys_obt_trn\')';
	//$etypeCond = 'gm.entity_type=\''.$entity_type.'\'';
	if($page != 'Courseedit' && $page != 'Search' && $page != 'people_setting' && $page !='adminroster'){
	if($page == 'discount'){
			$id = 'discount_id';
		}elseif($page == 'location'){
			$id = 'id';
		}elseif($page == 'Order'){
			$id = 'order_id';
		}else{
			$id = 'id';
		}
		//$select->addExpression('(if(group_map.id is not null AND ('.$createByAdmin.'.created_by !='.$userId.' OR '.$createByAdmin.'.updated_by != '.$userId.') AND ((select ifnull(org_id,\'T\') from slt_person where id = '.$userId.') != (select ifnull(org_id,\'F\') from slt_person where id = '.$createByAdmin.'.created_by)) ,sum(ifnull(priv.priv_edit,0)),1))', 'sumEdit');
		//$select->addExpression('(if(group_map.id is not null AND ('.$createByAdmin.'.created_by !='.$userId.' OR '.$createByAdmin.'.updated_by != '.$userId.') AND ((select ifnull(org_id,\'T\') from slt_person where id = '.$userId.') != (select ifnull(org_id,\'F\') from slt_person where id = '.$createByAdmin.'.created_by)) ,sum(ifnull(priv.priv_delete,0)),1))', 'sumDelete');
		 $select->addExpression('(CASE when group_map.id IS NOT NULL AND catacs.status=\'cre_sec_sts_atv\' AND ('.$createByAdmin.'.created_by !='.$userId.' OR '.$createByAdmin.'.updated_by != '.$userId.')
														THEN (SELECT IF(gm.id IS NULL,1,ifnull(max(p.priv_edit),0))
	      											FROM slt_group_privilege p
	      											left join slt_groups g on p.group_id=g.id
	      											left join slt_group_mapping gm on gm.group_id = g.id and '.$etypeCond.' and gm.group_type=1
	      											where FIND_IN_SET('.$userId.',g.userslist) > 0 AND g.status = \'cre_sec_sts_atv\'
	      											AND if('.$subAlis.'.is_admin=1,p.page_code = \''.$pageCode1.'\',p.page_code = \''.$pageCode.'\') AND gm.entity_id='.$alisename.'.'.$id.')
														ELSE CASE WHEN '.$createByAdmin.'.created_by = '.$userId.' OR '.$createByAdmin.'.updated_by = '.$userId.' OR
														((SELECT ifnull(max(p.priv_edit),0)
	      											FROM slt_group_privilege p
	      											left join slt_groups g on p.group_id=g.id
	      											where FIND_IN_SET('.$userId.',g.userslist) > 0 AND g.status = \'cre_sec_sts_atv\'
	      											AND if('.$subAlis.'.is_admin=1,p.page_code = \''.$pageCode1.'\',p.page_code = \''.$pageCode.'\')) > 0)
      											THEN 1
 	 													ELSE 0 END
														END )','sumEdit');

		$select->addExpression('(CASE
														when group_map.id IS NOT NULL AND catacs.status=\'cre_sec_sts_atv\' AND ('.$createByAdmin.'.created_by != '.$userId.')
														THEN (SELECT IF(gm.id IS NULL,1,ifnull(max(p.priv_delete),0))
      											FROM slt_group_privilege p
      											left join slt_groups g on p.group_id=g.id
      											left join slt_group_mapping gm on gm.group_id = g.id and '.$etypeCond.' and gm.group_type=1
      											where FIND_IN_SET('.$userId.',g.userslist) > 0 AND g.status = \'cre_sec_sts_atv\'
      											AND if('.$subAlis.'.is_admin=1,p.page_code = \''.$pageCode1.'\',p.page_code = \''.$pageCode.'\') AND gm.entity_id='.$alisename.'.'.$id.')
														ELSE CASE WHEN '.$createByAdmin.'.created_by = '.$userId.' OR ((SELECT ifnull(max(p.priv_delete),0)
      											FROM slt_group_privilege p
      											left join slt_groups g on p.group_id=g.id
      											where FIND_IN_SET('.$userId.',g.userslist) > 0 AND g.status = \'cre_sec_sts_atv\'
      											AND if('.$subAlis.'.is_admin=1,p.page_code = \''.$pageCode1.'\',p.page_code = \''.$pageCode.'\')) > 0)
      											THEN 1
														ELSE 0 END
														END )','sumDelete');
	}
}
//AND p.page_code = 'cre_sec_learner') > 0)
function getEnabledAttrForVisibility(){
	try{

		if(!isset($_REQUEST['apiname'])){
			$enabledValues = cache_get('site_config_tbl')->data;
			expDebug::dPrint("site configuration result1 ".print_r($enabledValues,true),4);
			if($enabledValues)
				return $enabledValues;
		}

		$selectConfig = db_select('slt_site_configuration', 'config');
		$selectConfig->condition('config.enabled',1,'=');
		$selectConfig->addField('config', 'code');
		$selectConfig->addField('config','name');
		$selectConfig->orderBy('config.name');
		if(!isset($_REQUEST['apiname'])){
			$selectInd = $selectConfig->extend("ExpertusSelectQueryExtender");
			$selectInd->setIndex(array('config'=>'sli_sitconf_enbl'));
			expDebug::dPrintDBAPI("site config query --> ",$selectInd);
			$result = $selectInd->execute()->fetchAllKeyed();
		}else{
			$result = $selectConfig->execute()->fetchAllKeyed();
		}
		expDebug::dPrint("site configuration result ".print_r($result,true),4);
		$defaultArr = array('ste_con_cnt'=>'country','ste_con_dep'=>'department','ste_con_ety'=>'employment_type','ste_con_rol'=>'role','ste_con_jbr'=>'job_role','ste_con_lng'=>'language','ste_con_org'=>'org_id','ste_con_ste'=>'state','ste_con_usr'=>'user_type', 'ste_con_hdt' => 'hire_date');
		$enabledValues = array_intersect_key($defaultArr,$result);
		if(!isset($_REQUEST['apiname'])){
 			cache_set('site_config_tbl',$enabledValues);
		}
		expDebug::dPrint("site configuration result ".print_r($enabledValues,true),4);
		return $enabledValues;
	}catch(Exception $ex){
		watchdog_exception('addICalToNotification', $ex);
        expertusErrorThrow($ex);
    }
}
//#custom_attribute_0078975

function adminVisibilityForGlobalAdd($entityType,$count='',$type='',$custom_attr=''){
	try{
		$userId = getIdOfLoggedInUser();
		expDebug::dPrint('Entoty type caleld : '.$entityType);
		$select = db_select('slt_group_privilege','priv');
		$select->leftJoin('slt_groups','catacs','catacs.id=priv.group_id');
		$select->leftJoin('slt_group_attributes','grpatt','grpatt.group_id=catacs.id');
		// $select->leftJoin('slt_owner_details','det','det.group_id = catacs.id');
		$select->leftJoin('slt_group_mapping','group_map','group_map.entity_id=catacs.id AND group_map.entity_type = \''.$entityType.'\' and group_map.group_type = 1');
		if($count=='count'){
			$select->addExpression('COUNT(DISTINCT catacs.id)', 'count');
		}else{
			if($entityType == 'cre_sec' || $entityType == 'cre_sec_learner') // Conditioned For Only One user access is set it showed Both Groups
			$select->addExpression('if(catacs.id is null,0 ,if(catacs.id is not null,sum(ifnull(priv.priv_add,0)),1))','addvisible');
			else
			if ($type == 'admin_calendar')
				$select->addExpression('if(catacs.id is not null,sum(ifnull(priv.priv_add,0)),if(priv.page_code IS NOT NULL,1,0))','addvisible');
			else
		    if($custom_attr=='1'){
				$select->addExpression('if(catacs.id is not null,sum(ifnull(priv.priv_add,0)),-1)','addvisible');
			}
			else
				$select->addExpression('if(catacs.id is not null,sum(ifnull(priv.priv_add,0)),1)','addvisible');
		}
		//$select->addExpression('count(priv.edit)','editvisible');
		//$select->addExpression('count(priv.delete)','deletevisible');
		$select->condition('catacs.is_admin',1,'=');
		$select->condition('catacs.userslist','','!=');
		/*$select->condition(db_or()
					->condition('catacs.userslist','','!=')
					->condition('det.owner_id',$userId, '='));*/
		$select->condition('catacs.status','cre_sec_sts_atv','=');
		if($entityType == 'cre_sys_obt_trp' || $entityType == 'cre_sys_obt_crt' || $entityType == 'cre_sys_obt_cur')
		$select->condition('priv.page_code',array('cre_sys_obt_trp','cre_sys_obt_crt','cre_sys_obt_cur'),'IN');
		else
		$select->condition('priv.page_code',$entityType,'=');
		filterByUserAccessPrivileges($select,'catacs','',1,$entityType,'','',1);
		expDebug::dPrintDBAPI('Query group privilege for admin',$select);
		$result = $select->execute()->fetchAll();
		expDebug::dPrint("teste ".print_r($result,true),4);
		return $result;

	}catch (Exception $ex){
		watchdog_exception('template_preprocess_error_page', $ex);
		expertusErrorThrow($ex);
	}
}
function adminVisibilityForAddEditDelete($entityId,$entityType,$pageCode,$tableName = ''){
	try{
		expDebug::dPrint("Values Passed to this function".$entityId.'entity type'.$entityType.'Page code'.$pageCode,4);
		$entityId = (int)$entityId; //For Security Fix
		//44834: Unable to view delete option in the drop down menu on the save button of the class detail pop up
		$userId = getIdOfLoggedInUser();
		$select = db_select('slt_group_privilege','priv');
		$select->leftJoin('slt_groups','catacs','catacs.id=priv.group_id');
		$select->leftJoin('slt_group_attributes','grpatt','grpatt.group_id = catacs.id');
		// $select->leftJoin('slt_owner_details','det','det.group_id = catacs.id');
		$select->leftJoin('slt_group_mapping','group_map','group_map.group_id=priv.group_id');
		/*if(!empty($tableName)) {
			if($tableName == 'slt_course_class')
			$select->leftJoin("'".$tableName."'",'obj','if((select count(1) from slt_group_mapping grp_map where grp_map.entity_id = \''.$entityId.'\' AND group_map.entity_type = \''.$entityType.'\' > 0 ),
	          			group_map.entity_id=\''.$entityId.'\' AND group_map.entity_type = \''.$entityType.'\' and group_map.group_id=priv.group_id,1=1
	          	)');
			else
			$select->leftJoin("'".$tableName."'",'obj','if((select count(1) from slt_group_mapping grp_map where grp_map.entity_id = \''.$entityId.'\' AND group_map.entity_type = \''.$entityType.'\' > 0 ),
	          			group_map.entity_id=\''.$entityId.'\' AND group_map.entity_type = \''.$entityType.'\' and group_map.group_id=priv.group_id,1=1
	          	)');
			//$select->leftJoin($tableName,'obj','obj.id=\''.$entityId.'\'');
		}*/
		$select->leftJoin($tableName,'obj','obj.id=\''.$entityId.'\'');
		//$select->addExpression('ifnull(priv.priv_delete, 1)','deletevisible');

		$select->addExpression('(CASE
				when group_map.id IS NOT NULL AND catacs.status= \'cre_sec_sts_atv\' AND (obj.created_by != '.$userId.' OR obj.updated_by != '.$userId.')
				THEN ifnull(priv.priv_delete, 1)
				ELSE CASE WHEN obj.created_by = '.$userId.' OR obj.updated_by = '.$userId.' OR

				(SELECT ifnull(max(p.priv_delete),0)

				FROM slt_group_privilege p

				left join slt_groups g on p.group_id=g.id

				where FIND_IN_SET('.$userId.',g.userslist) > 0 AND g.status = \'cre_sec_sts_atv\'

				AND if(g.is_admin=1,p.page_code = \''.$pageCode.'\',p.page_code = \''.$pageCode.'\'))

                THEN 1
                ELSE 0 END
                END)','deletevisible');


        $select->addExpression('(if(group_map.id is not null AND (obj.created_by ='.$userId.') ,1,sum(ifnull(priv.priv_delete, 1))))','deleteCount');
		filterByUserAccessPrivileges($select,'catacs','',1,$entityType,'','',1);
		$select->condition('group_map.entity_id',$entityId,'=');
		$select->condition('group_map.entity_type',$entityType,'=');
        //$select->condition('group_map.group_type',1,'=');

        $select->condition('priv.page_code',$pageCode,'=');
        expDebug::dPrintDBAPI('Query groupsss privilege for admin',$select);
        $result = $select->execute()->fetchAll();
        return $result;
    }catch (Exception $ex){
        watchdog_exception('adminVisibilityForAddEditDelete', $ex);
        expertusErrorThrow($ex);
    }
}

function getGroupNamesForView($entityId, $entityType){
	try {
		expDebug::dPrint('$courseId$courseId'.$entityId);
		$select = db_select('slt_groups', 'grps');
		$select->leftjoin('slt_group_mapping', 'grpmap', 'grpmap.group_id=grps.id');
		$select->addField('grps','name', 'name');
		$select->condition('grpmap.entity_id',$entityId,'=');
		$select->condition('grpmap.entity_type',$entityType,'=');
		$select->condition('grps.status','cre_sec_sts_atv','=');
		$select->groupBy('grps.id');
		expDebug::dPrintDBAPI('$select$select$select',$select);
		$result = $select->execute()->fetchCol();
		expDebug::dPrint('$result sabeena'.print_r($result,true),4);
		return $result;
	}catch (Exception $ex) {
		watchdog_Exception('getClassViewResults', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getAdminUserJobRoleAutoComplete() - Returns the matched User Job Roles
 */
function getAdminUserJobRoleAutoComplete() {
	try {
		$itemsList = getMatchingProfileItemNamesKeyed($_GET['z'], 'cre_usr_jrl_');
		if(arg(2) == 'group'){
			array_unshift($itemsList,'All');
		}
		foreach ($itemsList as $code => $name) {
			print $name . "\n";
		}
	} catch (Exception $ex) {
		watchdog_exception('getAdminUserJobRoleAutoComplete', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getAdminUserCityStateAutoComplete() - Returns the matched city and state name
 */
function getAdminUserCityStateAutoComplete() {
	try{
		$searchCurText =  $_GET['z'];
		if(arg(2) != 'group'){
			$selectCity = db_select('slt_person', 'per');
			$selectCity->distinct();
			$selectCity->addField('per', 'city', 'name');
			$selectCity->condition('per.city', '%' . db_like($searchCurText) . '%', 'LIKE');
			expDebug::dPrintDBAPI(' $selectCity OBJ = ' , $selectCity);
			$matchedResultsCity = $selectCity->execute()->fetchAll();
		}
		$selectState = db_select('slt_country', 'sc');
		$selectState->leftjoin('slt_state', 'st', 'st.country_code = sc.country_code');
		$selectState->distinct();
		$selectState->addField('st', 'state_name', 'name');
		$selectState->condition('st.state_name', '%' . db_like($searchCurText) . '%', 'LIKE');
		expDebug::dPrintDBAPI(' $selectState OBJ = ' , $selectState);
		$matchedResultsState = $selectState->execute()->fetchAll();

		$mergedValues = array_merge($matchedResultsCity,$matchedResultsState);
		if(arg(2) == 'group'){
			array_unshift($mergedValues,(object) array('name' => 'All'));
		}
		foreach ( $mergedValues as $key => $resName) {
			print $resName->name. "\n";
		}
	} catch (Exception $ex) {
		watchdog_exception('getAdminUserCityStateAutoComplete', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getAdminUserEmplTypeAutoComplete() - Returns the matched User Employment Types
 */
function getAdminUserEmplTypeAutoComplete() {
	try {
	$itemsList = getMatchingProfileItemNamesKeyed($_GET['z'], 'cre_usr_etp_');
	if(arg(2) == 'group'){
		array_unshift($itemsList,'All');
	}
	foreach ($itemsList as $code => $name) {
		print $name . "\n";
	}
	} catch (Exception $ex) {
		watchdog_exception('getAdminUserEmplTypeAutoComplete', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getAdminUserDeptAutoComplete() - Returns the matched User Department Types
 */
function getAdminUserDeptAutoComplete() {
	try {
		$itemsList = getMatchingProfileItemNamesKeyed($_GET['z'], 'cre_usr_dpt_');
		if(arg(2) == 'group'){
			array_unshift($itemsList,'All');
		}
		foreach ($itemsList as $code => $name) {
			//print $name . "\n";
			print rawurlencode($name) . "\n"; // Raw Url encode is using for this ticket 0045190 regards Department with | symbol Search are breaking.
		}
	} catch (Exception $ex) {
		watchdog_exception('getAdminUserDeptAutoComplete', $ex);
		expertusErrorThrow($ex);
	}
}


/*
 * getAdminUserTypeAutoComplete() - Returns the matched User Types
 */
function getAdminUserTypeAutoComplete() {
	try {
		$itemsList = getMatchingProfileItemNamesKeyed($_GET['z'], 'cre_usr_ptp_');
		if(arg(2) == 'group'){
			array_unshift($itemsList,'All');
		}
		foreach ($itemsList as $code => $name) {
			print $name . "\n";
		}
	} catch (Exception $ex) {
		watchdog_exception('getAdminUserTypeAutoComplete', $ex);
		expertusErrorThrow($ex);
	}
}


/*
 * getAdminCountryAutoComplete() - Returns the matched city and state name
 */
function getAdminCountryAutoComplete() {
	try{
		$searchCurText =  $_GET['z'];

		$selectCountry = db_select('slt_country', 'sc');
		$selectCountry->distinct();
		$selectCountry->addField('sc', 'country_name', 'name');
		if(arg(2) == 'group'){
			$selectCountry->addField('sc', 'country_code', 'code');
		}
		$selectCountry->addExpression("CONCAT('country')", 'country');
		$selectCountry->condition('sc.country_name', '%' . db_like($searchCurText) . '%', 'LIKE');
		expDebug::dPrintDBAPI(' $selectState OBJddd = ' , $selectCountry);
		$matchedResultsCountry = $selectCountry->execute()->fetchAll();
		//	$mergedValues = array_merge($matchedResultsCity,$matchedResultsCountry);
		if(arg(2) == 'group'){
			array_unshift($matchedResultsCountry,(object) array('name' => 'All','country' => 'country'));
		}
		foreach ( $matchedResultsCountry as $key => $resName) {
			//print $resName->name."|". $resName->city_state. "\n";
			print $resName->code . EXP_AC_SEPARATOR . $resName->name. "\n";
		}
	} catch (Exception $ex) {
		watchdog_exception('getAdminCountryAutoComplete', $ex);
		expertusErrorThrow($ex);
	}
}
/* function getOwnergroups($pid){
	try{
		$select = db_select('slt_owner_details', 'owner');
		$select->addField('grp', 'name', 'groupName');
		$select->addField('grp', 'id', 'id');
		$select->innerjoin('slt_groups','grp','grp.id = owner.group_id and grp.status = \'cre_sec_sts_atv\'');
		$select->condition('owner.owner_id', $pid, '=');
		expDebug::dPrintDBAPI("gerOwner Groups ------>",$select);
		$groupNames = $select->execute()->fetchAll();
		expDebug::dPrint("result--->".print_r($groupName,true),5);
		return $groupNames;

	}catch (Exception $ex) {
		watchdog_exception('getOwnergroups', $ex);
		expertusErrorThrow($ex);
	}
} */
/*
 * funtion to fetch groups assigned for individual user
 * $pid - sltPersonId
 * $entityType - entity type
 * $is_admin - to fetch admin groups only and default null
 *
 * */
function getGroupsDetails($pid,$entityType='',$is_admin = '',$uType='',$inactive=1)
{
	try{
		if (!empty($pid)) {
			if($uType =='manager,instructor'){
				$uType='view_users_group';
			}
			// Select table
			$selectStmt = db_select('slt_groups', 'grp');
			$selectStmt->join('slt_person','per');
			// Select fields and/or add expressions
			$selectStmt->distinct();
			$selectStmt->addField('grp', 'name', 'name');
			$selectStmt->addField('grp', 'id', 'id');
			$selectStmt->addField('grp', 'code', 'grp_code');
			$selectStmt->leftjoin('slt_person_jobrole_mapping','lpjm','lpjm.user_id = per.id');
			$selectStmt->leftjoin('slt_group_attributes','grpatt','grp.id = grpatt.group_id');
			$selectStmt->where("(((((if(grp.org_id='All',per.org_id is not null,grp.org_id is null OR FIND_IN_SET(per.org_id,grp.org_id)>0))
					AND (if(grp.user_type='All',per.user_type is not null,grp.user_type is null OR FIND_IN_SET(ifnull(per.user_type,''),grp.user_type)>0 ))
					AND (if(grp.employment_type='All',per.employment_type is not null,grp.employment_type is null OR FIND_IN_SET(ifnull(per.employment_type,''),grp.employment_type)>0))
					AND (if(grp.country='All',per.country is not null,grp.country is null OR FIND_IN_SET(ifnull(per.country,''),grp.country)>0))
					AND (if(grp.state='All',per.state is not null,grp.state is null OR FIND_IN_SET(ifnull(concat(per.country,'-',per.state),''),grp.state)>0))
					AND (if(grp.department='All',per.dept_code is not null,grp.department is null OR FIND_IN_SET(ifnull(per.dept_code,''),grp.department)>0))
					AND (if(grp.job_role='All',(select count(1) from slt_person_jobrole_mapping as jobmap where jobmap.user_id = per.id)>0,grp.job_role is null OR FIND_IN_SET(ifnull(lpjm.job_role,''),ifnull(grp.job_role,''))>0))
					AND (if(grp.language='All',per.preferred_language is not null,grp.language is null OR FIND_IN_SET(ifnull(per.preferred_language,''),grp.language)>0))
			        AND (CASE WHEN (grp.is_manager='Y' AND grp.is_instructor='Y')
                                                    THEN
                                                    (ifnull(per.is_manager,'N') = grp.is_manager or ifnull(per.is_instructor,'N') = grp.is_instructor)
                                                    WHEN (grp.is_manager='Y' AND grp.is_instructor='N')
                                                    THEN
                                                    (ifnull(per.is_manager,'N') = grp.is_manager)
                                                    WHEN (grp.is_manager='N' AND grp.is_instructor='Y')
                                                    THEN
                                                    (ifnull(per.is_instructor,'N') = grp.is_instructor)
                                                    ELSE
                                                    1=1
                                                    END)
					AND (
					if(grpatt.id is not null,if(grpatt.on_or_after_start_date is not null ,DATE_FORMAT(grpatt.on_or_after_start_date,'%Y-%m-%d') <= DATE_FORMAT(per.hire_date,'%Y-%m-%d'),1=0) OR 
            		if(grpatt.on_or_before_start_date is not null, DATE_FORMAT(grpatt.on_or_before_start_date,'%Y-%m-%d') >= DATE_FORMAT(per.hire_date,'%Y-%m-%d'), 1=0) OR
					if(grpatt.between_start_date is not null ,DATE_FORMAT(per.hire_date,'%Y-%m-%d') between DATE_FORMAT(grpatt.between_start_date,'%Y-%m-%d') AND DATE_FORMAT(grpatt.between_end_date,'%Y-%m-%d'), 1=0),1=1)     
					)
					AND (grp.removed_users is null OR FIND_IN_SET(per.id,grp.removed_users)<= 0)) OR (FIND_IN_SET(per.id,grp.added_users)>0))))");
			// Set conditions
			if($inactive==1){
				$db_or_condition = db_or();
				$db_or_condition->condition('grp.status', 'cre_sec_sts_atv', '=');
				$db_or_condition->condition('grp.status', 'cre_sec_sts_itv', '=');
				$selectStmt->condition($db_or_condition);
			}else{
			$selectStmt->condition('grp.status', 'cre_sec_sts_atv', '=');
			}
			$selectStmt->condition('per.id', $pid, '=');
			if($uType=='manager'){
				$selectStmt->condition('grp.code', 'grp_ins', '!=');
			}else if($uType=='instructor'){
				$selectStmt->condition('grp.code', 'grp_mgr', '!=');
			}else if($uType !='view_users_group'){
				//$selectStmt->condition('grp.code', array('grp_ins','grp_mgr'), 'NOT IN');
			}
			if($is_admin){
				$selectStmt->condition('grp.is_admin', 1, '=');
				$selectStmt->where("(grp.org_id is not null or
						grp.user_type is not null or
						grp.employment_type is not null or
						grp.country is not null or
						grp.state is not null or
						grp.department is not null or
						grp.job_role is not null or
						grp.language is not null or
						grp.is_manager != 'N' or
						grp.is_instructor !='N' or
						(grpatt.on_or_after_start_date is not null or grpatt.on_or_before_start_date or grpatt.between_start_date or grpatt.between_end_date) )");
				expDebug::dPrintDBAPI(' $$$selectStmt obj =' , $selectStmt);
				$result = $selectStmt->execute()->fetchAll();
				return $result;
			}else{
				$selectStmt->condition('grp.is_admin', 0, '=');
			}

			//expDebug::dPrintDBAPI(' $selectStmt obj =' , $selectStmt);

			$union = db_select('slt_groups', 'grp');
			$union->join('slt_person','per');
			// Select fields and/or add expressions
			$union->addField('grp', 'name', 'name');
			$union->addField('grp', 'id', 'id');
			$union->addField('grp', 'code', 'grp_code');
			$union->where("(((FIND_IN_SET(per.id,grp.added_users)>0 OR FIND_IN_SET(per.id,grp.userslist)>0) and (grp.removed_users is null OR FIND_IN_SET(per.id,grp.removed_users)<= 0)))");
			// Set conditions
			$union->condition('grp.status', 'cre_sec_sts_atv', '=');
			$union->condition('per.id', $pid, '=');
			//$union->condition('grp.code', 'grp_adm', '=');
			$union->condition('grp.is_admin', 1, '=');
			//expDebug::dPrintDBAPI(' $$union obj =' , $union);
			$union->orderBy('name');
			/* if($is_admin){
				//$selectStmt->condition('grp.is_admin', 1, '=');
				expDebug::dPrintDBAPI(' $$$selectStmt obj =' , $union);
				$result = $union->execute()->fetchAll();
				return $result;
				} */
			// Execute query
			$resultQuery = $selectStmt->union($union,'UNION ALL');
			$result = $resultQuery->execute();
			$query = $result->fetchAll();
			expDebug::dPrintDBAPI(' Group Details =' , $resultQuery);
			expDebug::dPrint('$query'. print_r($query, true), 3);
			return $query;
		}
	}catch (Exception $ex) {
		watchdog_exception('getSecurityDefDetails', $ex);
		expertusErrorThrow($ex);
	}
}

function getGroupId($pid){
	try{
		if (!empty($pid)) {
			$enabledValues = getEnabledAttrForVisibility();
			$selectStmt = db_select('slt_groups', 'grp');
			$selectStmt->join('slt_person','per');
			// Select fields and/or add expressions
			$selectStmt->distinct();
			//$selectStmt->addField('grp', 'name', 'name');
			$selectStmt->addExpression('group_concat(grp.id)', 'id');
			$selectStmt->leftjoin('slt_person_jobrole_mapping','lpjm','lpjm.user_id = per.id');
			$selectStmt->leftjoin('slt_group_attributes','grpatt','grpatt.group_id = grp.id');
			$selectStmt->where("(if(grp.org_id='All',per.org_id is not null,grp.org_id is null OR FIND_IN_SET(per.org_id,grp.org_id)>0))
					AND (if(grp.user_type='All',per.user_type is not null,grp.user_type is null OR FIND_IN_SET(ifnull(per.user_type,''),grp.user_type)>0 ))
					AND (if(grp.employment_type='All',per.employment_type is not null,grp.employment_type is null OR FIND_IN_SET(ifnull(per.employment_type,''),grp.employment_type)>0))
					AND (if(grp.country='All',per.country is not null,grp.country is null OR FIND_IN_SET(ifnull(per.country,''),grp.country)>0))
					AND (if(grp.state='All',per.state is not null,grp.state is null OR FIND_IN_SET(ifnull(concat(per.country,'-',per.state),''),grp.state)>0))
					AND (if(grp.department='All',per.dept_code is not null,grp.department is null OR FIND_IN_SET(ifnull(per.dept_code,''),grp.department)>0))
					AND (if(grp.job_role='All',(select count(1) from slt_person_jobrole_mapping as jobmap where jobmap.user_id = per.id)>0,grp.job_role is null OR FIND_IN_SET(ifnull(lpjm.job_role,''),ifnull(grp.job_role,''))>0))
					AND (if(grp.language='All',per.preferred_language is not null,grp.language is null OR FIND_IN_SET(ifnull(per.preferred_language,''),grp.language)>0))
					AND (if(grp.is_manager='N',1=1,(ifnull(per.is_manager,'N') = grp.is_manager))
					AND	if(grp.is_instructor='N',1=1,(ifnull(per.is_instructor,'N') = grp.is_instructor)))
					AND (
					if(grpatt.id is not null,if(grpatt.on_or_after_start_date is not null ,DATE_FORMAT(grpatt.on_or_after_start_date,'%Y-%m-%d') <= DATE_FORMAT(per.hire_date,'%Y-%m-%d'),1=0) OR 
            		if(grpatt.on_or_before_start_date is not null, DATE_FORMAT(grpatt.on_or_before_start_date,'%Y-%m-%d') >= DATE_FORMAT(per.hire_date,'%Y-%m-%d'), 1=0) OR
					if(grpatt.between_start_date is not null ,DATE_FORMAT(per.hire_date,'%Y-%m-%d') between DATE_FORMAT(grpatt.between_start_date,'%Y-%m-%d') AND DATE_FORMAT(grpatt.between_end_date,'%Y-%m-%d'), 1=0),1=1)   
					)
					AND (grp.removed_users is null OR FIND_IN_SET(per.id,grp.removed_users)<= 0) OR (FIND_IN_SET(per.id,grp.added_users)>0)");
			// Set conditions
			$selectStmt->condition('grp.status', 'cre_sec_sts_atv', '=');
			$selectStmt->condition('per.id', $pid, '=');
			$selectStmt->condition('grp.is_admin', 0, '=');
			$result = $selectStmt->execute()->fetchField();
			expDebug::dPrintDBAPI(' $resultQuery$resultQuery = getGroupId' , $selectStmt);
			return $result;
		}
	}catch(Exception $ex){
		watchdog_exception('getSecurityDefDetails', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getAdminLanguageAutoComplete() - Returns the matched city and state name
 */
function getAdminLanguageAutoComplete() {
	try{
		$searchCurText =  $_GET['z'];
		$selectLanguage = db_select('slt_profile_list_items', 'pro');
		$selectLanguage->distinct();
		$selectLanguage->addField('pro', 'name', 'name');
		$selectLanguage->addExpression("CONCAT('language')", 'language');
		$selectLanguage->condition('pro.name', '%' . db_like($searchCurText) . '%', 'LIKE');
		$selectLanguage->condition('pro.code', '%' . db_like('cre_sys_lng_') . '%', 'LIKE');
		expDebug::dPrintDBAPI(' $selectState OBJddd = ' , $selectLanguage);
		$matchedResultsLanguage = $selectLanguage->execute()->fetchAll();
		if(arg(2) == 'group'){
			array_unshift($matchedResultsLanguage,(object) array('name' => 'All','language' => 'language'));
		}
		foreach ( $matchedResultsLanguage as $key => $resName) {
			//print $resName->name."|". $resName->city_state. "\n";
			print $resName->name. "\n";
		}

	} catch (Exception $ex) {
		watchdog_exception('getAdminCountryAutoComplete', $ex);
		expertusErrorThrow($ex);
	}
}
/*
 * getCustomFieldsForView() - to get custom label and value for View screen
 */
function getCustomFieldsForView($entityId, $entityType){
	try{
		$select = db_select('slt_custom_fields','fields');
		$select->addField('fields', 'label' , 'label');
		$select->addField('fields', 'value' , 'value');
		$select->condition('fields.entity_id',$entityId, '=');
		$select->condition('fields.entity_type',$entityType, '=');
		expDebug::dPrintDBAPI('$select getCustomFieldsForView = ' , $select);
		$result = $select->execute()->fetchAll();
		return $result;
	}catch (Exception $ex) {
		watchdog_exception('getCustomFieldsForView', $ex);
		expertusErrorThrow($ex);
	}
}


function getMandatoryOrNot($class_id,$userId='',$fromCls=''){
	try{

		if($_REQUEST['mobile_request']){
			include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_learning/exp_sp_administration_catalog/exp_sp_administration_catalog_class.inc";
		}

		$userId = ($userId) ? $userId : getIdOfLoggedInUser();
		$isHasAccessRow = getAccessCount($class_id);
		expDebug::dPrint('is has access in getMandatoryOrNot' . print_r($isHasAccessRow, 1), 4);
		if($fromCls == ''){
			$select =db_select('slt_course_class','cls');
			//$select->leftJoin('slt_catalog_access','catacs','catacs.entity_id=cls.id AND catacs.entity_type = \'cre_sys_obt_cls\'');
			//$select->leftJoin('slt_group_mapping','catacs','catacs.entity_id=cls.id AND catacs.entity_type = \'cre_sys_obt_cls\'');
			//	$select->leftJoin('slt_mro_mapping','mro','mro.entity_id=catacs.entity_id AND FIND_IN_SET(mro.job_role_id,catacs.job_role) AND mro.job_role_id = (SELECT job_role FROM slt_person WHERE id=\''.$userId.'\') AND mro.entity_type = \'cre_sys_obt_cls\'');
			$select->addExpression('if(usrmap.mro is not null, usrmap.mro, group_map.mro )', 'mro');
			$select->condition('cls.id',$class_id,'=');
			if ($isHasAccessRow) {
			 filterByUserAccessPrivileges($select,'cls',$userId,'','cre_sys_obt_cls');
			}  else {
			 $select->leftjoin('slt_course_template', 'crs','crs.id=cls.course_id ');
			 filterByUserAccessPrivileges($select, 'crs',$userId,'','cre_sys_obt_crs');
			}

		}else{
			$select =db_select('slt_program','prg');
			//$select->leftJoin('slt_catalog_access','catacs','catacs.entity_id=prg.id AND catacs.entity_type IN (\'cre_sys_obt_crt\',\'cre_sys_obt_cur\',\'cre_sys_obt_trn\')');
			//$select->leftJoin('slt_group_mapping','catacs','catacs.entity_id=prg.id AND catacs.entity_type IN (\'cre_sys_obt_crt\',\'cre_sys_obt_cur\',\'cre_sys_obt_trn\')');
			//$select->leftJoin('slt_mro_mapping','mro','mro.entity_id=catacs.entity_id AND FIND_IN_SET(mro.job_role_id,catacs.job_role) AND mro.job_role_id = (SELECT job_role FROM slt_person WHERE id=\''.$userId.'\') AND mro.entity_type IN (\'cre_sys_obt_crt\',\'cre_sys_obt_cur\',\'cre_sys_obt_trn\')');
			$select->addExpression('if(usrmap.mro is not null, usrmap.mro, group_map.mro )', 'mro');
			$select->condition('prg.id',$class_id,'=');
			filterByUserAccessPrivileges($select,'prg',$userId,'','','Program');
		}
		$select->addExpression("if(if(usrmap.mro is not null, usrmap.mro, group_map.mro ) = 'cre_sys_inv_opt',3,if(if(usrmap.mro is not null, usrmap.mro, group_map.mro ) = 'cre_sys_inv_man',1,2))", "ascen");
		$select->orderBy("ascen");
		$select->range(0,1);
		expDebug::dPrintDBAPI('Query for getMandatoryOrNot', $select);
		$resultarr = $select->execute()->fetchAll();
		$result = "";
		if(count($resultarr) > 0)
			$result = $resultarr[0]->mro;
		return $result;
	}catch(Exception $ex){
		watchdog_exception('getMandatoryOrNot', $ex);
		expertusErrorThrow($ex);
	}
}
/**
 * getClassDeliverTypeByEnrollId() to return the delivery/class type for the given enrollment id
 * Author: ayyappans
 * @param int $enrollId
 * @param string $returnType
 * @return string delivery_type
 */
function getClassDeliverTypeByEnrollId($enrollId, $returnType='Short') {
	expDebug::dPrint('$returnType : '.$returnType, 4);
	$select = db_select('slt_enrollment', 'en');
	$select->addField('en', 'class_id');
	$select->condition('en.id', $enrollId);
	$result = $select->execute();
	expDebug::dPrintDBAPI('$select getDeliverTypeByEnrollId = ' , $select);
	expDebug::dPrint('$result ayyappan'.$enrollId.$returnType);
	if(!$result->rowCount()) {
		return '';
	}
	else {
		$classId = $result->fetchField(0);
		expDebug::dPrint('dT'.$classId.getClassDeliveryType($classId, $returnType));
		return getClassDeliveryType($classId, $returnType);
	}

	//return $select->execute()->fetchAll();
}
/**
 * Used to trim the title based on the widget width
 * @param string $title
 * @param string $title_type
 * view_detail_page - Course/Class/TP Detailed page
 * catalog_page - Course/Class list page
 * $title_image - Title have image like complaince etc
 */
function widgetTitleController($title, $title_type = '',$title_image='') {
	global $theme_key,$language;
	expDebug::dPrint('title_image='.$title_image);
	if ($theme_key == 'expertusoneV2') { // new theme
		switch ($title_type) {
			case 'view_detail_page':
				if(!empty($title_image))
					$showTitle = titleController('WIDGET-VIEW-DETAIL-PAGE-COMPLIANCE', $title,70);
				else
					$showTitle = titleController('WIDGET-VIEW-DETAIL-PAGE', $title,80);
				break;
			case 'catalog_page':
				$showTitle = $title;
				break;
			default:
				$showTitle = $title;
				break;
		}
	} else {
		switch ($title_type) {
			case 'view_detail_page':
				if($language->language == 'ru'){
					if ($_SESSION['widget']['display_width'] > 635 && $_SESSION['widget']['display_width'] < 750){
						if(!empty($title_image))
						$showTitle = titleController(' ', $title,40);
						else
						$showTitle = titleController(' ', $title,40);
					}
					else if($_SESSION['widget']['display_width'] > 750 && $_SESSION['widget']['display_width'] < 900){
						if(!empty($title_image))
						$showTitle = titleController(' ', $title,50);
						else
						$showTitle = titleController(' ', $title,55);
					}
					else if($_SESSION['widget']['display_width'] > 900){
						if(!empty($title_image))
						$showTitle = titleController(' ', $title,60);
						else
						$showTitle = titleController(' ', $title,70);
					}
				}else{
					if ($_SESSION['widget']['display_width'] > 635 && $_SESSION['widget']['display_width'] < 750){
						if(!empty($title_image))
						$showTitle = titleController(' ', $title,50);
						else
						$showTitle = titleController(' ', $title,50);
					}
					else if($_SESSION['widget']['display_width'] > 750 && $_SESSION['widget']['display_width'] < 900){
						if(!empty($title_image))
						$showTitle = titleController(' ', $title,60);
						else
						$showTitle = titleController(' ', $title,65);
					}
					else if($_SESSION['widget']['display_width'] > 900){
						if(!empty($title_image))
						$showTitle = titleController(' ', $title,70);
						else
						$showTitle = titleController(' ', $title,80);
					}
				}
				break;
			case 'catalog_page':
				if($language->language == 'ru'){
					if ($_SESSION['widget']['display_width'] > 635 && $_SESSION['widget']['display_width'] < 750)
					$showTitle = titleController(' ', $title,30);
					else if($_SESSION['widget']['display_width'] > 750 && $_SESSION['widget']['display_width'] < 900)
					$showTitle = titleController(' ', $title,40);
					else if($_SESSION['widget']['display_width'] > 900)
					$showTitle = titleController(' ', $title,50);
				}else{
					if ($_SESSION['widget']['display_width'] > 635 && $_SESSION['widget']['display_width'] < 750)
					$showTitle = titleController(' ', $title,35);
					else if($_SESSION['widget']['display_width'] > 750 && $_SESSION['widget']['display_width'] < 900)
					$showTitle = titleController(' ', $title,45);
					else if($_SESSION['widget']['display_width'] > 900)
					$showTitle = titleController(' ', $title,55);
				}
				break;
			default:
				$showTitle = titleController(' ', $title,45);
				break;
		}
	}
	return $showTitle;
}

function getWidgetSelectedLanguage($qury=false){
	$lg_type = '';
	if(isset($_COOKIE['catalog_searchStr']) && $_COOKIE['catalog_searchStr']!=''){
		$parameters = explode('&',$_COOKIE['catalog_searchStr']);
		$lg_typr_str = '';
		foreach($parameters as $key=>$val)
		{
			$key_arr = explode('=',$parameters[$key]);
			if($key_arr[0]=='lg_type')
			$lg_typr_str = $key_arr[1];

		}
		if($qury)
		return $lg_typr_str;
		$lg_type = explode('|',$lg_typr_str);
	}else if(isset($_GET['lg_type']) && !empty($_GET['lg_type'])){
		if($qury)
		return $_GET['lg_type'];
		$lg_type = explode('|',$_GET['lg_type']);
	}
	return $lg_type;
}

/**
 * Function used to find the primary path of the link (Embed widget)
 */
function checkWidgetUrl($currUrl='') {
	try {
		global $base_url;

		$current_path = ($currUrl == '') ? arg(0) . '/' . arg(1) : $currUrl;
		//Added for #0066221.
		if(checkWidgetCallback($current_path) || arg(0) == 'share'){
			$system_base = parse_url($base_url);
			$referer = parse_url($_SERVER['HTTP_REFERER']);
			// validate the refer host with system base url and set the referer.
			if (($system_base['scheme'] == $referer['scheme']) && ($system_base['host'] == $referer['host'])) {
				return $_SERVER['HTTP_REFERER'];
			} else {
				return $base_url;
			}
		} else {
			return $_SERVER['HTTP_REFERER'];
		}
	} catch (Exception $ex) {
		watchdog_exception('checkWidgetUrl', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Check the callback from widget module
 * @param string $path
 */
function checkWidgetCallback($path='') {
	try {
		// path used in the widget callbacks
		$widgetPaths = array(
      'widget/catalog-search' => 'Catalog search',
      'widget/class-details' => 'Class details',
      'widget/course-details' => 'Course details',
      'widget/learning-plan-details' => 'Learning plan details',
      'widget/enrollment-search' => 'My enrollment section',
      'widget/register' => 'Register',
		);

		if (isset($widgetPaths[$path])) {
			return true;
		} else {
			return false;
		}
	} catch (Exception $ex) {
		watchdog_exception('checkWidgetCallback', $ex);
		expertusErrorThrow($ex);
	}
}
/**
 * Trim input values
 * @param string $inputVal
 */
function trimInput($inputVal = '') {
	try {
		return trim($inputVal);
	} catch(Exception $e) {
		watchdog_exception('trimInput', $exception);
		expertusErrorThrow($ex);
	}
}

/**
 * Trim Array of Fields values
 * @param array $inputFields
 */
function trimInputFields($inputFields = '') {
	try	{
		expDebug::dPrint('Input value before:' . print_r($inputFields, 1), 5);
		if (is_object($inputFields)) {
			foreach($inputFields as $key => $value) {
				//expDebug::dPrint('Input object key:' . $key .  ': value : ' . print_r($value, 1), 1);
				if (!is_array($value) && !is_null($value)){
					$inputFields->$key = trimInput($value);
				}
			}
		} else {
			foreach($inputFields as $key => $value) {
				//expDebug::dPrint('Input Array key:' . $key .  ': value : ' . print_r($value, 1), 1);
				if (!is_array($value) && !is_null($value)){
					$inputFields[$key] = trimInput($value);
				}
			}
		}
		expDebug::dPrint('Input value after:' . print_r($inputFields, 1), 5);
		return $inputFields;
	} catch (Exception $ex) {
		watchdog_exception('trimInputFields', $ex);
		expertusErrorThrow($ex);
	}
}


/**
 * Check Privelage for Learner or admin Group
 */
function privilegeChecking($admin_pri,$learner_pri) {
	try {
		$privelage = '';
		global $user;
		expDebug::dPrint('$user : '.print_r($user, 4));
		if(($admin_pri > 0 && $learner_pri > 0)||  $user->uid ==1 ){
			$privelage = 'both';
		}else if($admin_pri > 0){
			$privelage = 'admin';
		}else if($learner_pri > 0){
			$privelage = 'learner';
		}
		return $privelage;
	} catch(Exception $e) {
		watchdog_exception('privilegeChecking', $exception);
		expertusErrorThrow($ex);
	}
}
//method added by ayyappans for 46087
function check_if_enrollment_exists_by_node_id($user_id, $node_id) {
	$select = db_select('slt_node_learning_activity', 'sla');
	$select -> join('slt_course_class', 'scc', 'sla.entity_id = scc.id AND sla.entity_type = :entity_type', array('entity_type' => 'cre_sys_obt_cls'));
	$select -> join('slt_course_template', 'sct', 'scc.course_id = sct.id');
	$select -> addField('sct', 'title');
	$select -> addField('sct', 'id');
	$select -> condition('sla.node_id', $node_id);

	$result = $select -> execute() -> fetchAll();
	// 	expDebug::dPrintDBAPI('check equ', $select);

	$course_id = $result[0]->id;
	$course_title = $result[0]->title;
	// 	expDebug::dPrint('count'.print_r($result, 1));
	// 	expDebug::dPrint('count'.print_r($course_title, 1));
	if(count($result) > 0 && checkEquivalence($user_id, $course_id, '', false, true) > 0) {
 		return $course_title;
 	}
}
//Validate Date Formate
function validateDateFormat($date,$format='')
{
	if(empty($format))
		$format = 'Y-m-d';
	$d = DateTime::createFromFormat($format, $date);
	return $d && $d->format($format) == $date;
}
// Validate Date,Month and Year-- Validation for hire dates-- Current date-50 to Current date+5
function validateDateMonthYear($date,$format, $beforedate = 70, $afterdate = 5){

	$d = DateTime::createFromFormat($format, $date);
	if(strpos($date,'/'))
		$validate = explode('/',$d->format($format));
	else
		$validate = explode('-',$d->format($format));
	expDebug::dPrint(' Form state of date format ' .print_r($validate, true) , 5);
	expDebug::dPrint(' Form state of date format ' .print_r(now()+5, true) , 5);
	if($validate [0] >= 1 && $validate [0] <= 12 && $validate [1] >= 1 && $validate [1] <= 31 && $validate [2] >= now()-50 && $validate [2] <= now()+5){
		return true;
	}
}
//Validation added for 67741
function validateHireDateFormat($date,$format='')
{
	if(empty($format))
		$format = 'm/d/Y';
	$d = DateTime::createFromFormat($format, $date);
	expDebug::dPrint("date check test".print_r($d,1));
	return $d && $d->format($format) == $date;
}
// Method For Strip slashes
function stripslashes_deep($array) {
	$new = array();
	foreach ($array as $key => $val) {
		if (is_array($val)) {
			$new[$key] = stripslashes_deep($val);
		} else {
			$new[$key] = stripslashes($val);
		}
	}
	return $new;
}
// Method For url exist or not
function urlExists($url=NULL)
{
	if($url == NULL) return false;
	$ch = curl_init($url);
	curl_setopt($ch, CURLOPT_TIMEOUT, 5);
	curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 5);
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
	$data = curl_exec($ch);
	$httpcode = curl_getinfo($ch, CURLINFO_HTTP_CODE);
	curl_close($ch);
	if($httpcode>=200 && $httpcode<300){
		return true;
	} else {
		return false;
	}
}
/**
 * Check given job role is exists or not
 */
function checkValidUserAtrribute($attrValue,$code = '',$typename = '', $multipleChk='',$type='',$gid) {
	expDebug::dPrint(' $result = ' . print_r($type, true),3);
	$explodeAttrValue = explode(",",$attrValue);
	$uniqueAttrValue = array_unique(explode(",",$attrValue));
	$attrValueCount = count($uniqueAttrValue);
	if(count($explodeAttrValue) > 1 && $typename != 'job role' && $multipleChk != 'no'){
		$err_msg = "The given ".$typename." cannot have multiple values";
		$errobj=new stdClass();
		$errobj->isValidateError = 1;
		$errobj->errormsg = $err_msg;
		return $errobj;
	}
	if($attrValueCount > 0) {
		expDebug::dPrint(' $select groups' .$gid);
		$select = db_select("slt_profile_list_items", "spl");
		$select->addField("spl", "code");
		$select->condition("spl.code", $uniqueAttrValue, 'IN');
		$select->condition("spl.is_active",'Y','=');
	if($gid != ''){
		$select->leftJoin('slt_groups','grp','(grp.'.$type.'=\'All\' or find_in_set(spl.code,grp.'.$type.')>0) and grp.id=:gid',array('gid'=>$gid));
		$select->addExpression('if(grp.'.$type.'=\'All\',1,0)','allsel');
		$select->condition('grp.id',$gid,'=');
		}
		if(!empty($code))
			$select->condition("spl.code", db_like($code) . '%', 'LIKE');
		filterByUserAccessPrivileges($select, 'spl','',1,substr($code, 0, -1),'','spl');
		$result = $select->execute()->fetchCol();
		if($attrValueCount > count($result)){
			$role_diff = array_diff($uniqueAttrValue,$result);
			$combined_role = implode(",",$role_diff);
			$err_msg = "The given ".$typename." " . $combined_role." doesn't exists/inactive or USER PRIVILEGE";
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errormsg = $err_msg;
			return $errobj;
		}
		else {
			return TRUE;
		}
		
	}
}
function is_valid_country($countryCode, $chkMultiple = ""){
  try{

  	if($chkMultiple == "yes")
  	{
  		$explodeAttrValue = explode(",",$countryCode);
  		$uniqueAttrValue = array_unique(explode(",",$countryCode));
  		$countryCodeCount = count($uniqueAttrValue);
  		if($countryCodeCount > 0) {
  			$select = db_select('slt_country', 'country');
			$select->addField('country','country_code');
			$select->condition("country.country_code",$uniqueAttrValue,'IN');
  			$result = $select->execute()->fetchCol();
  			if($countryCodeCount > count($result)){
  				$role_diff = array_diff($uniqueAttrValue,$result);
  				$combined_role = implode(",",$role_diff);
  				$err_msg = "The given country " . $combined_role." doesn't exists";
  				$errobj=new stdClass();
  				$errobj->isValidateError = 1;
  				$errobj->errormsg = $err_msg;
  				return $errobj;
  			}
  			else {
  				return TRUE;
  			}
  		}
  	}
  	else {
    $select = db_select('slt_country', 'country');
		$select->addField('country','country_name');
		$select->condition("country.country_code",$countryCode,'=');
		expDebug::dPrintDBAPI(' $select is_valid_country = ' ,$select);
    $result = $select->execute()->fetchField();
    if(empty($result)){
    	$err_msg = "The given country ".$countryCode." doesn't exists or inactive";
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errormsg = $err_msg;
			return $errobj;
    }else{
    	return TRUE;
    }
  	}

  }catch (Exception $ex) {
    watchdog_exception('is_organization', $ex);
    expertusErrorThrow($ex);
  }
}

function is_valid_state($countryCode,$stateCode, $chkMultiple = ""){
  try{
  	if($chkMultiple == "yes")
  	{
  		$explodeAttrValue = explode(",",$stateCode);
  		$uniqueStateAttrValue = array_unique(explode(",",$stateCode));
  		$stateCodeCount = count($uniqueStateAttrValue);

  		$explodeAttrValue = explode(",",$countryCode);
  		$uniqueCountryAttrValue = array_unique(explode(",",$countryCode));
  		$countryCodeCount = count($uniqueCountryAttrValue);
  		if($stateCodeCount > 0) {
  			$select = db_select('slt_state', 'state');
			$select->addField('state','state_code');
			$select->condition("state.country_code",$uniqueCountryAttrValue,'IN');
			$select->condition("state.state_code",$uniqueStateAttrValue,'IN');
  			$result = $select->execute()->fetchCol();
  			if($stateCodeCount > count($result)){
  				$role_diff = array_diff($uniqueStateAttrValue,$result);
  				$combined_role = implode(",",$role_diff);
  				$err_msg = "The given state " . $combined_role." doesn't exists";
  				$errobj=new stdClass();
  				$errobj->isValidateError = 1;
  				$errobj->errormsg = $err_msg;
  				return $errobj;
  			}
  			else {
  				return TRUE;
  			}
  		}
  	}
  	else {
  	    $select = db_select('slt_state', 'state');
			$select->addField('state','state_name');
			$select->condition("state.country_code",$countryCode,'=');
			$select->condition("state.state_code",$stateCode,'=');
			expDebug::dPrintDBAPI(' $select is_valid_state = ' ,$select);
	    $result = $select->execute()->fetchField();

	  	if(empty($result)){
	    	$err_msg = "The given state ".$stateCode." doesn't exists or inactive";
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errormsg = $err_msg;
				return $errobj;
	    }else{
	    	return TRUE;
	    }
  	 }
  }catch (Exception $ex) {
    watchdog_exception('is_organization', $ex);
    expertusErrorThrow($ex);
  }
}

// To convert all characters to html entities
function sanitize_data($value) {
  if(($value != "") && (!empty($value))) {
  	return htmlentities($value,ENT_QUOTES,'UTF-8'); // 'UTF-8' added for #0053658
  }
}

/**
 * Clear local cached data which stored in session
 * @param $id - Should be a Drupal's user id
 * @return unknown_type
 */
function clearUserSessionVariables($id=''){
	global $user;
	$id = !empty($id) ? $id : $user->uid ;
	$sesUpd = db_update('sessions');
	$sesUpd->fields(array('session'=>''));
	$sesUpd->condition('uid',$id);
	expDebug::dPrintDBAPI('Clear stored session of user',$sesUpd);
	$sesUpd->execute();
}



function resetStoredSessionsForActiveUsers(){
	$memlimit = ini_get('memory_limit');
	expDebug::dPrint("Memory limit 0 - ".$memlimit);
	ini_set('memory_limit', '512M');
	$expier = time() - (60 * 60);

	$stSes = db_select('sessions','ses');
	$stSes->addField('ses','uid','uid');
	$stSes->addField('ses','timestamp','timestamp');
	$stSes->addField('ses','session','session');
	$stSes->condition('ses.timestamp',$expier,'>=');
	expDebug::dPrintDBAPI('Stored session list for unset values',$stSes);
	$result = $stSes->execute()->fetchAll(PDO::FETCH_ASSOC);
	expDebug::dPrint("Session result ".print_r($result,true),1);
	foreach($result as $rst){
		$tmp = resetStoredValues(unserializesession($rst['session']));
		$sesUpd = db_update('sessions');
		$sesUpd->fields(array('session'=>serialize($tmp)));
		$sesUpd->condition('uid',$rst['uid']);
		$sesUpd->condition('timestamp',$rst['timestamp']);
		expDebug::dPrintDBAPI('Update stored session list after unset values',$sesUpd);
		$sesUpd->execute();
	}
	ini_set('memory_limit', $memlimit);
}

function resetStoredValues($details){
	if(isset($details['parent_array'])) unset($details['parent_array']);
  if(isset($details['BasePath'])) unset($details['BasePath']);
  return $details;
}

function unserializesession( $data ) {
  if(strlen( $data) == 0) {
    return array();
  }

  // match all the session keys and offsets
  preg_match_all('/(^|;|\})([a-zA-Z0-9_]+)\|/i', $data, $matchesarray, PREG_OFFSET_CAPTURE);

  $returnArray = array();

  $lastOffset = null;
  $currentKey = '';
  foreach ( $matchesarray[2] as $value ) {
    $offset = $value[1];
    if(!is_null( $lastOffset)) {
      $valueText = substr($data, $lastOffset, $offset - $lastOffset );
      $returnArray[$currentKey] = unserialize($valueText);
    }
    $currentKey = $value[0];

    $lastOffset = $offset + strlen( $currentKey )+1;
  }

  $valueText = substr($data, $lastOffset );
  $returnArray[$currentKey] = unserialize($valueText);

  return $returnArray;
}
function getUserDefaultCurrency($callAPI=""){
  if($callAPI)
  {
//   		$userid = $_REQUEST['user_id'];
//   		if(empty($userid))
  			$userid = $_REQUEST['userid'];
//   		if($_REQUEST['apiname'] == 'ListEnrollmentsAPI')
//   			$userid = $_REQUEST['UserID'];
  		if(is_numeric($userid) && $userid>0)
		{
			if($_REQUEST['version'])
  			{
  				//Below code is for mobile app
  				$UserPrefCurrCode = 'USD';
  				$UserPrefCurrSym  = '$';
  			}
  			else if($_REQUEST['currency_type'] && $_REQUEST['currency_sym'])
			{
				$UserPrefCurrCode = $_REQUEST['currency_type'];
				$UserPrefCurrSym  = $_REQUEST['currency_sym'];
			}
			else if($_REQUEST['currency_type'])
			{
				$getDefaultCurrency = getDefaultCurrency();
				$UserPrefCurrCode = $getDefaultCurrency['preferred_currency'];
				$UserPrefCurrSym  = $getDefaultCurrency['attr2'];
				$UserPrefCurrLongCode  = $getDefaultCurrency['code'];
			}
			else
			{
			$selectStmt = db_select('slt_person', 'per');
			$selectStmt->leftjoin('slt_profile_list_items', 'spli', 'spli.attr1 = per.preferred_currency AND spli.code LIKE \'cre_sys_crn_%\' AND spli.is_active=\'Y\' AND spli.attr3=\'Y\'');
			// Set conditions
			$selectStmt->condition('per.id', $userid);

			//Amaran: Added status is active condition only condtion:0041257
			$selectStmt->condition('per.status', 'cre_usr_sts_atv');

			$selectStmt->addField('per','preferred_currency','preferred_currency');
			$selectStmt->addField('spli','attr2','attr2');
			$selectStmt->addField('spli','code','code');

			expDebug::dPrintDBAPI(' $selectStmt SQL = ' , $selectStmt);
			$result = $selectStmt->execute()->fetchAssoc();
			$UserPrefCurrCode = $result['preferred_currency'];
	  		$UserPrefCurrSym  = $result['attr2'];
	  		$UserPrefCurrLongCode  = $result['code'];

		  		if(empty($UserPrefCurrCode))
		  		{
		  			$getDefaultCurrency = getDefaultCurrency();
					$UserPrefCurrCode = $getDefaultCurrency['preferred_currency'];
					$UserPrefCurrSym  = $getDefaultCurrency['attr2'];
					$UserPrefCurrLongCode  = $getDefaultCurrency['code'];
		  		}
			}
		}
		else {
			$getDefaultCurrency = getDefaultCurrency();
			$UserPrefCurrCode = $getDefaultCurrency['preferred_currency'];
			$UserPrefCurrSym  = $getDefaultCurrency['attr2'];
			$UserPrefCurrLongCode = $getDefaultCurrency['code'];
		}
  }
  else
  {
	  if(isset($_SESSION['user_preferred_currency_code']) && !empty($_SESSION['user_preferred_currency_code'])){
	  	$UserPrefCurrCode = $_SESSION['user_preferred_currency_code'];
	  	$UserPrefCurrSym = $_SESSION['user_preferred_currency_sym'];
	  }else{
	  	$userInfo = getDefaultCurrency();
	  	$UserPrefCurrCode = $userInfo['preferred_currency'];
	  	$UserPrefCurrSym  = $userInfo['attr2'];
	  	$UserPrefCurrLongCode = $userInfo['code'];
	  }
  }

    if(empty($UserPrefCurrSym))
    	$UserPrefCurrSym = $UserPrefCurrCode;

    drupal_add_js(array('user_prefference' => array('currency_code' =>  $UserPrefCurrCode, 'currency_sym' =>  $UserPrefCurrSym)), 'setting');
	$currencyDet = array(
									'currency_code' => $UserPrefCurrCode,
									'currency_sym' => $UserPrefCurrSym,
									'currency_long_code' => $UserPrefCurrLongCode
											);
	return $currencyDet;
}
/**
 * Function to validate currency type is valid or not using attr1
 *
 * @param string $attr1
 * $attr1     - USD
 *
 * @return
 *   Returns id or null
 */
function currency_isValidOrNot($attr1){
	$selectStmt = db_select('slt_profile_list_items', 'plist');
	// Set conditions
	$selectStmt->condition('plist.code', $attr1);
	$selectStmt->condition('plist.is_active', 'Y');
	$selectStmt->condition('plist.attr3', 'Y');
	// Select fields
	$selectStmt->addField('plist', 'attr1', 'short_code');
	expDebug::dPrintDBAPI(' $selectStmt = ' ,$selectStmt);
	// Execute query
	$result = $selectStmt->execute();

	if ($result->rowCount() <= 0) {
		return null;
	}
	return $result->fetchField();
}


function expiredDaysCheck($prg_id,$expires_in_value,$expires_in_unit){
	try{
	$selectStmt = db_select('slt_program', 'prg');
	// Set conditions
	$selectStmt->condition('prg.id', $prg_id); 
	$selectStmt->condition('prg.object_type', "cre_sys_obt_crt"); 
	$selectStmt->condition('prg.expires_in_value', $expires_in_value,"!=");
	$selectStmt->condition('prg.expires_in_unit', $expires_in_unit);
	
	// Select fields
	//$selectStmt->addField('prg', 'expires_in_value', 'expires_in_value');
	$selectStmt->addExpression('COUNT(prg.id)', 'count');
	expDebug::dPrintDBAPI(' $selectStmt = ' ,$selectStmt);
	// Execute query
	$result = $selectStmt->execute();

	if ($result->rowCount() <= 0) {
		return null;
	}
	return $result->fetchField();
	
	}catch(Exception $ex){
		watchdog_exception('expiredDaysCheck', $ex);
		expertusErrorThrow($ex);
	}
}


/* get_sf_user_info_from_lms() - Get user and Details from ExpertusONE according to SF user
 * @param :  Filter str : user will be filtered according to this filter str  email,per_id,per_user_name,user_id
 * @return : returns the user details in array according tin filter str and filter value
 */

function get_sf_user_info_from_lms($filter_str,$filter_val){
	try{

		expDebug::dPrint('$filter_str = '.print_r($filter_str,true),5);
		expDebug::dPrint('$filter_val = '.print_r($filter_val,true),5);

		$select = db_select('slt_person', 'per');
		$select->innerjoin('users', 'usr', 'per.user_name=usr.name');
		$select->addField('per','first_name','first_name');
		$select->addField('per','last_name','last_name');
		$select->addField('per','email','email');
		$select->addField('per','id','per_id');
		$select->addField('per','user_name','user_name');
		$select->addField('per','preferred_language','preferred_language');
		$select->addField('usr','uid','usr_uid');
		$select->addField('usr','language','usr_language');

		if($filter_str=="email"){
			$select->condition('per.email',$filter_val,"=");
		}
		if($filter_str=="per_id"){
			$select->condition('per.id',$filter_val,"=");
		}
		if($filter_str=="user_id"){
			$select->condition('usr.uid',$filter_val,"=");
		}
		if($filter_str=="per_user_name"){
			$select->condition('per.user_name',$filter_val,"=");
		}

		expDebug::dPrintDBAPI("Select Query = ", $select);
		// Execute the query
		$result = $select->execute()->fetchAssoc();

		expDebug::dPrint('$result = '.print_r($result,true),5);

		return $result;


	}catch(Exception $ex){
		watchdog_exception('get_sf_user_info_from_lms', $ex);
		expertusErrorThrow($ex);
	}
}

/* get_previous_session_for_sf_cookieless() - Added by ganeshbabuv to implement the salesforce cookieless option (#0054508) on 30th Sep 2015 10:40 AM
 * If cookie doesn't set in ExpertusONE, it gets the session id from salesforce and reload the session in ExpertusONE according to session id  */

function get_previous_session_for_sf_cookieless($sid = NULL){
	try{
		if($sid == NULL) {
			$sid = $_REQUEST['exp_sess_id'];
		}
		$ses_value = db_query('SELECT session FROM {sessions} WHERE sid = :sid', array(':sid' => $sid))->fetchField();

		if($ses_value!=""){
			expDebug::dPrint('Insecure Session - Http - $ses_value = '.print_r($ses_value,true),5);
		}else{
			$ses_value = db_query('SELECT session FROM {sessions} WHERE ssid = :sid', array(':sid' => $sid))->fetchField();
			expDebug::dPrint('Secure Sesion - Https - $ses_value = '.print_r($ses_value,true),5);
		}

		expDebug::dPrint("ses_value =".print_r($ses_value,true),5);


		if($ses_value!=''){
			$ses_arr=unserializesession($ses_value);

			expDebug::dPrint("ses_arr =".print_r($ses_arr,true),5);

			foreach($ses_arr as $ses_key => $ses_val){
				$_SESSION[$ses_key]=$ses_val;
			}

			if(trim($_SESSION['logged_user_id'])!=''){


				expDebug::dPrint("logged_user_id in Session =".print_r($_SESSION['logged_user_id'],true),5);

				  //Get user details
				expDebug::dPrint('$_SESSION[sf_user_details] = '.print_r($_SESSION['sf_user_details'],true),5);

				if(isset($_SESSION['sf_user_details']) && !empty($_SESSION['sf_user_details'])){
					 $usr_det=$_SESSION['sf_user_details'];
				}else{
					  $usr_det=get_sf_user_info_from_lms("per_id",$_SESSION['logged_user_id']);
					  expDebug::dPrint('$usr_det from DB = '.print_r($usr_det,true),5);
					  $_SESSION['sf_user_details']=$usr_det;
			   }

			   global $user;

			   $user->uid=trim($usr_det['usr_uid']);
			   $user->language=trim($usr_det['usr_language']);
			   $user->name=trim($usr_det['user_name']);


			   $language_list = language_list();

			   expDebug::dPrint('$language_list='.print_r($language_list,true),5);

			   // Set Language

			   global $language;

			   $language = $language_list[$user->language];

			   $_SESSION['Pref_Language']=trim($usr_det['preferred_language']);

			   expDebug::dPrint('$languages = '.print_r($language,true),5);


				if ($user->uid!="" && $user->uid>0){
					//Set role to avoid the 403 issue when enable the cookieless option. Role must.
					$role_name='authenticated user';
					$role_obj=user_role_load_by_name($role_name);
					expDebug::dPrint("role_obj =".print_r($role_obj,true),5);

					if(!empty($role_obj) && isset($role_obj)){
						$user->roles = array($role_obj->rid => $role_obj->name);
					}else{
						expDebug::dPrint("There is no authentication role in roles table.Please check",5);
					}

				}

				expDebug::dPrint("user->uid in Get Previous Session =".print_r($user,true),5);
			}

			expDebug::dPrint("previous session has refreshed=".print_r($_SESSION,true),5);

		}else{
			expDebug::dPrint("No previous session available in sessions table",5);
		}

		$_SESSION['sf_exp_sess_id']=trim($_REQUEST['exp_sess_id']);
		expDebug::dPrint("sf_exp_sess_id in Session =". $_SESSION['sf_exp_sess_id'],5);

		unset($_REQUEST['exp_sess_id']);
		unset($_POST['exp_sess_id']);
		unset($_GET['exp_sess_id']);

	}catch(Exception $ex){
		watchdog_exception('get_previous_session_for_sf_cookieless', $ex);
		expertusErrorThrow($ex);
	}
}

function getClassIdFromEnrollmentId($enrId){
	try{
		$qry = db_select('slt_enrollment','enr');
		$qry->addField('enr', 'class_id');
		$qry->condition('enr.id',$enrId,'=');
		$clsId = $qry->execute()->fetchField();
		return core_encrypt($clsId);
	} catch(Exception $ex){
		watchdog_exception('getClassIdFromEnrollmentId', $ex);
		expertusErrorThrow($ex);
	}
}

function getComplianceMandatoryStatusFromExempted($enrId,$type,$classId='') {
	expDebug::dPrint("Values to the submit function".$exmType.$enrId.$type.$exmId,5);

	$user_id = getIdOfLoggedInUser();
	$field = array('id','is_mandatory','is_compliance','enrollment_id','exempted_status');
	$qry = db_select('slt_enrollment_exempted','exmp');
	$qry->condition('exmp.enrollment_id',$enrId,'=');
	$qry->condition('exmp.enroll_type',$type,'=');
	if($type == 'tp'){
		if(!empty($classId))
		$qry->condition('exmp.course_id',$classId,'=');
	}else{
		if(!empty($classId))
		$qry->condition('exmp.class_id',$classId,'=');
	}
	$qry->fields('exmp',$field);
	$qry->range(0,1);
	$qry->orderBy('id','desc');
	expDebug::dPrintDBAPI("Query for exempted enrollment values",$qry);
	$status = $qry->execute()->fetchAll();
	expDebug::dPrint("Result for exempted enrollment values".print_r($status,1),5);
	return $status;

}

function getRegisteredEnrollmentExmeptedorNot($type,$userId,$crsId,$clsId='',$enrolled_id=''){
	try{
		if(empty($userId) || empty($userId) ||empty ($crsId)) {
			return '';
		}else {
		if($type == 'class' || $type == 'course' ){
			$clsCon = '';
			if($clsId)
				$clsCon = ' AND e.class_id ='.$clsId;

			$inQry = db_select('slt_enrollment_exempted','exm');
			$inQry->innerJoin('slt_enrollment','e','e.id = exm.enrollment_id'.$clsCon.' AND e.course_id ='.$crsId.' AND e.user_id ='.$userId);
			$inQry->addField("exm", 'id');
			$inQry->addField("exm", 'enrollment_id','enrid');
			$inQry->addField("exm", 'exempted_status','exmsts');
			$inQry->condition("e.master_enrollment_id",'','IS NULL');
			if($enrolled_id!=''){
				$inQry->where("exm.enroll_type = 'class' AND exm.enrollment_id=$enrolled_id");
			}else{
				$inQry->where("exm.enroll_type = 'class'");
			}

			$inQry->orderBy("exm.id",'desc');
			$inQry->range(0,1);
			$qry = db_select('slt_enrollment','enr');
			$qry->innerJoin($inQry,'x','x.enrid = enr.id');
			$qry->addField('enr', 'id');
			$qry->condition('x.exmsts',1,'=');
				$qry->condition('enr.reg_status','lrn_crs_reg_can','!=');
				expDebug::dPrintDBAPI("getRegisteredEnrollmentExmeptedorNot for Class--->",$qry);
				$res = $qry->execute()->fetchField();
				expDebug::dPrint("Result of getRegisteredEnrollmentExmeptedorNot for Class".print_r($res,1));
			}else{
				$inQry = db_select('slt_enrollment_exempted','exm');
				$inQry->innerJoin('slt_master_enrollment','e','e.id = exm.enrollment_id AND e.program_id ='.$crsId.' AND e.user_id ='.$userId);
				$inQry->addField("exm", 'id');
				$inQry->addField("exm", 'enrollment_id','enrid');
				$inQry->addField("exm", 'exempted_status','exmsts');
				if($enrolled_id!=''){
					$inQry->where("exm.enroll_type = 'tp' AND exm.enrollment_id=$enrolled_id");
				}else{
					$inQry->where("exm.enroll_type = 'tp'");
				}
				$inQry->orderBy("exm.id",'desc');
				$inQry->range(0,1);
				$qry = db_select('slt_master_enrollment','enr');
				$qry->innerJoin($inQry,'x','x.enrid = enr.id');
				$qry->addField('enr', 'id');
				$qry->condition('x.exmsts',1,'=');
				$qry->condition('enr.overall_status','lrn_tpm_ovr_cln','!=');
				expDebug::dPrintDBAPI("getRegisteredEnrollmentExmeptedorNot for TP",$qry);
				$res = $qry->execute()->fetchField();
				expDebug::dPrint("Result of getRegisteredEnrollmentExmeptedorNot for TP".print_r($res,1));
			}
			return $res;
		}
	}catch(Exception $e){
		watchdog_exception('getRegisteredEnrollmentExmeptedorNot'.$e);
		expertusErrorThrow($e);
	}
}
function getClassExmeptedorNot($type,$entity_id){
	try{
		if($type == 'class'){
			$inQry = db_select('slt_enrollment_exempted','exm');
			$inQry->leftjoin('slt_enrollment','enr','enr.id = exm.enrollment_id');
			$inQry->addExpression('COUNT(exm.id)', 'count');
			$inQry->condition('exm.enroll_type','class','=');
			$inQry->condition('enr.class_id',$entity_id,'=');
			$count = $inQry->execute()->fetchField();
		}else if($type == 'tp'){
			$inQry = db_select('slt_enrollment_exempted','exm');
			$inQry->leftjoin('slt_master_enrollment','enr','enr.id = exm.enrollment_id');
			$inQry->addExpression('COUNT(exm.id)', 'count');
			$inQry->condition('exm.enroll_type','tp','=');
			$inQry->condition('enr.program_id',$entity_id,'=');
			$count = $inQry->execute()->fetchField();
		}
		return $count;
	}catch(Exception $e){
		watchdog_exception('getRegisteredEnrollmentExmeptedorNot'.$e);
		expertusErrorThrow($e);
	}
}

function getEnrollmentExmeptedorNot($type,$entity_id){
	try{
		if($type == 'class'){
			$inQry = db_select('slt_enrollment_exempted','exm');
			$inQry->leftjoin('slt_enrollment','enr','enr.id = exm.enrollment_id');
			$inQry->addExpression('COUNT(exm.id)', 'count');
			$inQry->condition('exm.enroll_type','class','=');
			$inQry->condition('enr.id',$entity_id,'=');
			$count = $inQry->execute()->fetchField();
		}else if($type == 'tp'){
			$inQry = db_select('slt_enrollment_exempted','exm');
			$inQry->leftjoin('slt_master_enrollment','enr','enr.id = exm.enrollment_id');
			$inQry->addExpression('COUNT(exm.id)', 'count');
			$inQry->condition('exm.enroll_type','tp','=');
			$inQry->condition('enr.id',$entity_id,'=');
			$count = $inQry->execute()->fetchField();
		}
		return $count;
	}catch(Exception $e){
		watchdog_exception('getRegisteredEnrollmentExmeptedorNot'.$e);
		expertusErrorThrow($e);
	}
}

function checkComplianceAndMandatory ($enrollmentId,$entityType,$fromExempted = false,$classId = '') {

	try{
		if($entityType == 'class' && !empty($enrollmentId) && $fromExempted == false) {
			$qry = db_select('slt_enrollment','enroll');
			$qry->addField('enroll', 'is_compliance');
			$qry->addField('enroll', 'mandatory');
			$qry->condition('enroll.id',$enrollmentId,'=');
			if($classId != '')
			$qry->condition('exem.class_id',$classId,'=');
			$qry->orderBy("enroll.id",'desc');
			$qry->range(0,1);
			$complianceDetails = $qry->execute()->fetchAll();
			expDebug::dPrint("Values for exempted enrollments".print_r($exmDet,true),3);
			return $complianceDetails;
		} else if ($fromExempted == true) {
			$qry = db_select('slt_enrollment_exempted','exem');
			$qry->addField('exem', 'is_compliance');
			$qry->addField('exem', 'is_mandatory');
			$qry->addField('exem','exempted_status');
			$qry->condition('exem.enrollment_id',$enrollmentId,'=');
			if($classId != '')
			$qry->condition('exem.class_id',$classId,'=');
			$qry->orderBy("exem.id",'desc');
			$qry->range(0,1);
			expDebug::dPrintDBAPI("checkComplianceAndMandatory qry--->",$qry);
			$complianceDetails = $qry->execute()->fetchAll();
			expDebug::dPrint("Values for exempted enrollments".print_r($complianceDetails,true),3);
			return $complianceDetails;
		}else if($entityType == 'tp' && !empty($enrollmentId) && $fromExempted == false){
			$qry = db_select('slt_master_enrollment','enroll');
			$qry->addField('enroll', 'mandatory');
			$qry->condition('enroll.id',$enrollmentId,'=');
			$qry->orderBy("enroll.id",'desc');
			$qry->range(0,1);
			$complianceDetails = $qry->execute()->fetchAll();
			expDebug::dPrintDBAPI("DFDSDSFDSFdafad",$qry);
			expDebug::dPrint("Values for tp enrollments".print_r($exmDet,true),3);
			return $complianceDetails;
		}
	} catch (Exception $ex) {
		watchdog_exception('enrollmentExemptedDetails', $ex);
		expertusErrorThrow($ex);
	}

}

function checkCourseClassCompliance($courseId,$entity_type='')
{
	try{
		if($entity_type == 'cre_sys_obt_cls'){
			$qry = db_select('slt_course_class','sltcc');
			$qry->addField('sltcc', 'is_compliance');
			$qry->condition('sltcc.id',$courseId,'=');
			expDebug::dPrintDBAPI("checkCourseClassCompliance qry--->",$qry);
		}
		else{
			$qry = db_select('slt_course_template','sltct');
			$qry->addField('sltct', 'is_compliance');
			$qry->condition('sltct.id',$courseId,'=');
			expDebug::dPrintDBAPI("checkCourseClassCompliance qry--->",$qry);
		}
			$complianceDetails = $qry->execute()->fetchAll();
			expDebug::dPrint("Values for exempted checkCourseClassCompliance".print_r($complianceDetails,true),3);
			return $complianceDetails;
		} catch (Exception $ex) {
		watchdog_exception('enrollmentExemptedDetails', $ex);
		expertusErrorThrow($ex);
}
}
/*
 *  Get a slt_order id from uc_order id
*/
function getSltOrderId($ucOrderId){
	try{
		$selectStmt = db_select('slt_order', 'ord');
		$selectStmt->condition('ord.uc_order_id', $ucOrderId);
		$selectStmt->addField('ord', 'id');
		expDebug::dPrintDBAPI(' $selectStmt SQL = ' , print_r($selectStmt, true));
		$sltOrderId = $selectStmt->execute()->fetchField();
		return $sltOrderId;
	}catch (Exception $ex) {
		watchdog_exception('getSltOrderId', $ex);
		expertusErrorThrow($ex);
	}
}

//method to change youtube urls to embeddable urls
function convert_youtube_url($url) {
    return preg_replace("/\s*[a-zA-Z\/\/:\.]*youtu(be.com\/watch\?v=|.be\/)([a-zA-Z0-9\-_]+)([a-zA-Z0-9\/\*\-\_\?\&\;\%\=\.]*)/i", "https://www.youtube.com/embed/$2", $url);
}

function insertPaymentErrorLog($order,$type,$msg,$fn_name) {
	try{
	$loggedusr = getSltpersonUserId();
	if(empty($order) && $order == '') {
		$order = NULL;
	}
	$insertStmt = db_insert('slt_payment_error_log');
	$fields = array(
			'order_id'    => $order,
			'payment_type'  => $type,
			'error_message'  => $msg,
			'created_by'  => $loggedusr,
			'created_on'  => now(),
			'updated_by'  => $loggedusr,
			'updated_on'  => now(),
			'function_name' => $fn_name

		);
	$insertStmt->fields($fields);
	$logId = $insertStmt->execute();
	return $logId;
	} catch (Exception $ex) {
		watchdog_exception('insertPaymentErrorLog', $ex);
		throw $ex;
	}
}
// Unset user session details
function unsetSessionValues(){
	unset($_SESSION['person_detail']);
	unset($_SESSION['job_role']);
}

function getAccessCount($classID){
	try{
		$select = db_select('slt_group_mapping', 'mapping');
		$select->addField('(mapping','id','accessCount');
		$select->condition('mapping.entity_id', $classID, '=');
		$select->condition('mapping.entity_type','cre_sys_obt_cls','=');
		$select->condition('mapping.group_type',0,'=');
		
		$select1 = db_select('slt_user_access_mapping', 'mapping');
		$select1->addField('(mapping','id','accessCount');
		$select1->condition('mapping.entity_id', $classID, '=');
		$select1->condition('mapping.entity_type','cre_sys_obt_cls','=');
		$select1->condition('mapping.access_type',0,'=');
		
		$select->union($select1,'UNION ALL');
		
		$select2 = db_select($select,'map');
		$select2->addExpression('COUNT(map.accessCount)','accessCount');
		
		//expDebug::dPrintDBAPI("getAccessCount", $select2);
		$getAccess  = $select2->execute()->fetchField();
		expDebug::dPrintDBAPI("getAccessCount", $select2);
		return $getAccess;
	}catch (Exception $ex) {
		watchdog_exception('getCountforComlienceAccessClass', $ex);
		expertusErrorThrow($ex);
	}
}
function getConcatenatedUserDets($type = NULL,$APIUid = ''){
  try{
  global $user;
  if($_REQUEST['mobile_request'] ){
  	$user->uid = $APIUid;
  }
  
  $selectStmt = db_select('slt_person', 'prsn');
  $selectStmt->join('users', 'usr', 'prsn.user_name = usr.name');
  $selectStmt->leftJoin('slt_profile_list_items', 'prfl', 'prfl.code = prsn.time_zone');
  $selectStmt->condition('usr.uid', $user->uid);
  $selectStmt->addField('usr', 'uid', 'id');
  $selectStmt->addExpression('CONCAT(prsn.first_name,\' \',prsn.last_name)','full_name');
  $selectStmt->addField('prsn', 'user_name', 'user_name');
  $selectStmt->addField('prfl', 'attr1', 'time_zone');
  $selectStmt->addField('prfl', 'code', 'time_zone_code');
  $selectStmt->addField('prfl', 'attr2', 'timezone_attr');
  expDebug::dPrintDBAPI('MY ESIGN INFO ' , $selectStmt);
  $result =  $selectStmt->execute()->fetchAll();
  $i = 0;
  $set_result = array();
  if($type != null){
	  	$currDateTime = date_format(date_create($ses_start_date_format),'Y-m-d H:i');
	    $defaultTimeZone = date_default_timezone(FALSE);
  		if(!$result[0]->timezone_attr){
	    	$result[0]->timezone_attr = 'Asia/Calcutta';
	    }
	    $convertedCurrTime  = timeZoneConvert($currDateTime , $defaultTimeZone , $result[0]->timezone_attr);
	    $formattedCurrTime 	= date_format($convertedCurrTime,'Y-m-d H:i');
	    $formattedCurrTime = date_format(date_create($formattedCurrTime),'Y-m-d H:i:s');
	   	$set_result = new stdClass();
	   	$set_result->time_zone = (!$result[0]->time_zone) ? t('LBL587') : $result[0]->time_zone;
	   	$set_result->curr_date_time = $formattedCurrTime;
		$set_result->time_zone_code = (!$result[0]->time_zone_code) ? 'cre_sys_tmz_059' : $result[0]->time_zone_code;
	  	return $set_result;
  }else{
    $defaultTimeZone = date_default_timezone(FALSE);
    $currDateTime    = date_format(date_create($date_time),'Y-m-d H:i');
  	foreach ($result as $record) {
	    if(!$record->timezone_attr){
	    	$record->timezone_attr = $defaultTimeZone;
	    }
	    $convertedCurrTime  = timeZoneConvert($currDateTime , $defaultTimeZone , $record->timezone_attr);
	    $formattedCurrTime 	= date_format($convertedCurrTime,'Y-m-d H:i');
	    $formattedCurrTime = date_format(date_create($formattedCurrTime),'M d, Y H:i:s');
	   	   $set_result[$i] = array(
		        'user_id'              => $record->id,
		        'full_name'            => $record->full_name,
		        'user_name'            => $record->user_name,
		        'time_zone'            => (!$record->time_zone) ? t('LBL587') : $record->time_zone,
		   	    'curr_date_time'	     => $formattedCurrTime,
		   	    'esign_static_message' => t("MSG351"),
		   	    'esign_title'		       => t('LBL584'),
		   	    'password_req_txt'	   => t('ERR137'),
		   	    'password_valid_txt'   => t('ERR138'),
		   	    'esign_date_txt'	     => t('LBL585'),
		   	    'esign_name_txt'	     => t('LBL107'),
		   	    'esign_cancel_txt'	   => t('LBL123'),
		   	    'esign_pass_txt'	     => t('LBL060'),
		   	    'esign_uname_txt'	     => t('LBL054'),
		   	    'esign_sign_txt'	     => t('LBL586'),
	        );
	        $i++;
	  }
	  return drupal_json_output($set_result);
  }
  }catch (Exception $ex) {
		watchdog_exception('getConcatenatedUserDets', $ex);
		expertusErrorThrow($ex);
  }
}
/*
 * getLeaderboardAutoComplete() - Returns the matched person names.
*                           The below search options are available which are to be sent in the extraParams option of the autocomplete jQuery plugin.
*
*                           search_by_username - When set to 1, also search the search text in user_name field apart from full_name field
*                           id - When set to a non-null string, return id of each user in the pipe separate results as the first field
*                           fetch_username - When set to 1, return it as a pipe separated field in result after the fullname.
*                                            When set to 2, append username in brackets to each user's full name.
*                           excluded_user_id - When set to 1, exclude this user from the results
*                           exclude_reportees_of_excluded_user - When set to 1, exclude all direct/indirect reportees of excluded_user_id from the results
*                           list_only_managers - When set to 1, list only the users for whom is_manager is set to 1 in slt_person table.
*                           list_only_instructors - When set to 1, list only the users for whom is_instructor is set to 1 in slt_person table.
*
*                           autocomplete jquery plugin sends the search string in 'z' and max number of records to fetch in 'limit'
*/
function getLeaderboardAutoComplete() {
	try{
		$loggedInUserId = getIdOfLoggedInUser();
		$searchByUsername = (isset($_GET['search_by_username']) && $_GET['search_by_username'] == 1)? true : false;
		expDebug::dPrint(' $searchByUsername = '. print_r($searchByUsername, true),4);

		$idRequired = (!empty($_GET['id']))? true : false;
		expDebug::dPrint(' $idRequired = '. print_r($idRequired, true),4);

		$userNameAsField = (isset($_GET['fetch_username'])) && ($_GET['fetch_username'] == 1)? true : false;
		expDebug::dPrint(' $userNameAsField = '. print_r($userNameAsField, true),4);

		$userNameAppended = (isset($_GET['fetch_username'])) && ($_GET['fetch_username'] == 2)? true : false;
		expDebug::dPrint(' $userNameAppended = '. print_r($userNameAppended, true),4);

		$excludedUserId = (isset($_GET['excluded_user_id']))? $_GET['excluded_user_id'] : 0;
		expDebug::dPrint(' $excludedUserId = '. print_r($excludedUserId, true),4);

		$excludeReportees = (isset($_GET['exclude_reportees_of_excluded_user']) && $_GET['exclude_reportees_of_excluded_user'] == 1)? true : false;
		expDebug::dPrint(' $excludeReportees = '. print_r($excludeReportees, true),4);

		$listOnlyManagers = (isset($_GET['list_only_managers']) && $_GET['list_only_managers'] == 1)? true : false;
		expDebug::dPrint(' $listOnlyManagers = '. print_r($listOnlyManagers, true),4);

		$listOnlyInstructors = (isset($_GET['list_only_instructors']) && $_GET['list_only_instructors'] == 1)? true : false;
		expDebug::dPrint(' $listOnlyInstructors = '. print_r($listOnlyInstructors, true),4);

		$exclude_logged_user = (isset($_GET['exclude_logged_user']) && $_GET['exclude_logged_user'] == 1)? true : false;
		expDebug::dPrint(' $exclude_logged_user = '. print_r($exclude_logged_user,true),4);

		$nameSubstr = $_GET['z'];
		expDebug::dPrint(' $nameSubstr = '. print_r($nameSubstr, true),4);

		$limit = (empty($_GET['limit']))? 0 : $_GET['limit'];
		expDebug::dPrint(' $limit = '. print_r($limit, true),4);

		$loggedUserId = getSltpersonUserId();
		// Select the table
		$select = db_select('slt_person', 'per');

		// Add fields to fetch
		if ($idRequired || $userNameAsField || $userNameAppended) {
			$select->addField('per', 'full_name', 'name');
			if ($idRequired) {
				$select->addField('per', 'id', 'id');
			}
			if ($userNameAsField || $userNameAppended) {
				$select->addField('per', 'user_name', 'user_name');
			}
		}
		else if(!empty($nameSubstr)) {
			$select->addField('per', 'full_name', 'name');
		}
		else { // When neither id is required nor username has been fetched, return distinct user full names.
			$select->addExpression('DISTINCT(per.full_name)', 'name');
		}

		// Add conditions
		// Return only the active records.
		$select->condition('per.status', 'cre_usr_sts_atv');

		// Always exclude 'admin' and 'guest' users
		$select->condition('per.user_name', array('admin', 'guest'), 'NOT IN');
		if($exclude_logged_user)	#fix for issues 0023841 & 0025347
			$select->condition('per.id', $loggedUserId, '<>');

		if ($searchByUsername) {
			$select->condition(db_or()
					->condition('per.full_name', '%' . db_like($nameSubstr) . '%', 'LIKE')
					->condition('per.user_name', '%' . db_like($nameSubstr) . '%', 'LIKE'));
		}
		else {
			$select->condition('per.full_name', '%' . db_like($nameSubstr) . '%', 'LIKE');
		}

		if (!empty($excludedUserId) && $excludedUserId > 0) {
			$excludedUserIdList = array($excludedUserId);
			if ($excludeReportees) {
				$excludedUserIdList = array_merge($excludedUserIdList, getAllDirectIndirectReporteesIds($excludedUserId));
			}
			$select->condition('per.id', $excludedUserIdList, 'NOT IN');
		}

		if ($listOnlyManagers) {
			$select->condition('per.is_manager', 'Y', '='); // @TODO Manager logic also includes users who have reportees
			// even if is_manager is not set to 1 for them
		}
		if ($listOnlyInstructors) {
			$select->condition('per.is_instructor', 'Y', '=');
		}
		$select->leftjoin('slt_person_jobrole_mapping','lpjm','lpjm.user_id = per.id ');
		$select->leftjoin('slt_groups','grp',"grp.status = 'cre_sec_sts_atv' AND grp.is_admin=0 AND (((((if(grp.org_id='All',per.org_id is not null,grp.org_id is null OR FIND_IN_SET(per.org_id,grp.org_id)>0))
				AND (if(grp.user_type='All',per.user_type is not null,grp.user_type is null OR FIND_IN_SET(ifnull(per.user_type,''),grp.user_type)>0 ))
				AND (if(grp.employment_type='All',per.employment_type is not null,grp.employment_type is null OR FIND_IN_SET(ifnull(per.employment_type,''),grp.employment_type)>0))
				AND (if(grp.country='All',per.country is not null,grp.country is null OR FIND_IN_SET(ifnull(per.country,''),grp.country)>0))
				AND (if(grp.state='All',per.state is not null,grp.state is null OR FIND_IN_SET(ifnull(concat(per.country,'-',per.state),''),grp.state)>0))
				AND (if(grp.department='All',per.dept_code is not null,grp.department is null OR FIND_IN_SET(ifnull(per.dept_code,''),grp.department)>0))
				AND (if(grp.job_role='All',(select count(1) from slt_person_jobrole_mapping as jobmap where per.id= jobmap.user_id)>0,grp.job_role IS NULL OR (FIND_IN_SET(ifnull(lpjm.job_role,''),ifnull(grp.job_role,''))>0)))
				AND (if(grp.language='All',per.preferred_language is not null,grp.language is null OR FIND_IN_SET(ifnull(per.preferred_language,''),grp.language)>0))
				AND (if(grp.is_manager='N',1=1,(ifnull(per.is_manager,'N') = grp.is_manager))
				AND	if(grp.is_instructor='N',1=1,(ifnull(per.is_instructor,'N') = grp.is_instructor)))
				AND (grp.removed_users is null OR FIND_IN_SET(per.id,grp.removed_users)<= 0)) OR (FIND_IN_SET(per.id,grp.added_users)>0))))");
		$select->leftJoin('slt_group_attributes','grpatt',"grpatt.group_id = grp.id AND (if(grpatt.id is not null,if(grpatt.on_or_after_start_date is not null ,DATE_FORMAT(grpatt.on_or_after_start_date,'%Y-%m-%d') <= DATE_FORMAT(per.hire_date,'%Y-%m-%d'),1=0) OR 
            		if(grpatt.on_or_before_start_date is not null, DATE_FORMAT(grpatt.on_or_before_start_date,'%Y-%m-%d') >= DATE_FORMAT(per.hire_date,'%Y-%m-%d'), 1=0) OR
					if(grpatt.between_start_date is not null ,DATE_FORMAT(per.hire_date,'%Y-%m-%d') between DATE_FORMAT(grpatt.between_start_date,'%Y-%m-%d') AND DATE_FORMAT(grpatt.between_end_date,'%Y-%m-%d'), 1=0),1=1)
				)");
		$select->orderBy('name'); // order by full name
		if($loggedInUserId>0){
			$grpId = getGroupId($loggedInUserId);
		}
		if($loggedInUserId <= 0 || empty($grpId)){
			$select->condition('grp.id','','IS NULL');
		}else if(!empty($grpId)){
			$select->condition('grp.id','','IS NOT NULL');
			$select->where("FIND_IN_SET(grp.id,'$grpId')> 0");
		}
		$select->groupBy('per.id');
		// limit number of records to be fetched and returned
		if ($limit > 0) {
			$select->range(0, $limit);
		}
		expDebug::dPrintDBAPI(' $select SQL = ' ,$select);
		$matchingPersons = $select->execute()->fetchAll();
		expDebug::dPrint(' $matchingPersons = ' . print_r($matchingPersons, true),3);
		foreach ($matchingPersons as $person) {
			expDebug::dPrint(' $person = ' . print_r($person, true),3);
			$choiceStr = '';
			if ($idRequired) {
				$choiceStr .= $person->id . '|';
			}

			$choiceStr .= $person->name;

			if ($userNameAsField) {
				$choiceStr .= '|' . $person->user_name;
			}
			else if ($userNameAppended) {
				$choiceStr .= ' (' . $person->user_name . ')';
			}
			expDebug::dPrint(' $choiceStr = ' . print_r($choiceStr, true),4);
			print $choiceStr . "\n";
		} // end foreach
	}
	catch (Exception $ex) {
		watchdog_exception('getPersonAutoComplete', $ex);
		expertusErrorThrow($ex);
	}
}

function adminVisibilityForAssign($pageCode){
	try{
		if($pageCode == "cre_sys_obt_crs" || $pageCode == "cre_sys_obt_cls" || $pageCode == "cre_sys_obt_trp"){
			$pageCode = array("cre_sys_obt_crs");
		}elseif($pageCode == "discount"){
			$pageCode = array("cre_sys_obt_crs","cre_sys_obt_trp");
		}
		else{
			$pageCode = array($pageCode);
		}
		$userId = getIdOfLoggedInUser();
		if($userId == 1){
			return true;
		}
		expDebug::dPrint("Values Passed to this function".$entityId.'entity type'.$entityType.'Page code'.$pageCode,4);
		$userId = getIdOfLoggedInUser();
		$select = db_select('slt_group_privilege','priv');
		$select->leftJoin('slt_groups','catacs','catacs.id=priv.group_id');
		$select->addExpression('priv.page_code','page_code');
		$select->where("FIND_IN_SET($userId,catacs.userslist)>0");
		$select->condition('priv.page_code',$pageCode,'IN');
		expDebug::dPrintDBAPI('Query groupsss privilege for admin',$select);
		$result = $select->execute()->fetchAll();
		return $result;
	}catch (Exception $ex){
		watchdog_exception('adminVisibilityForAssign', $ex);
		expertusErrorThrow($ex);
	}
}

function getGroupidByEntityid($entityId='',$entityType=''){
	try{
		$select = db_select('slt_group_mapping','grp_map');
		$select->join('slt_groups','grp','grp.id = grp_map.group_id');
		$select->addExpression('GROUP_CONCAT(grp_map.group_id)','group_id');
		$select->condition('grp_map.entity_id',$entityId,'=');
		$select->condition('grp_map.entity_type',$entityType,'IN');
		$select->condition('grp_map.group_type','0','=');
		$select->condition('grp.status','cre_sec_sts_atv','=');
		$result = $select->execute()->fetchField();
		expDebug::dPrintDBAPI('Query for getGroupidByEntityid',$select);
		return $result;
	}catch (Exception $ex){
		watchdog_exception('getGroupidByEntityid', $ex);
		expertusErrorThrow($ex);
	}
}
function filterByGroupAccessPrivileges(&$select,$entityId='',$entityType=''){
	try{
		if($entityType == 'TP'){
			$entityType = array('cre_sys_obt_crt','cre_sys_obt_cur','cre_sys_obt_trn','cre_sys_obt_trp');
		}else{
			$entityType = array('cre_sys_obt_cls','cre_sys_obt_crs');
		}
	//	if($groupids){
			$select->leftjoin('slt_person_jobrole_mapping', 'spjm', 'spjm.user_id = per.id');
			$select->leftJoin('slt_groups','grp',"(grp.is_admin = 0) and grp.status = 'cre_sec_sts_atv' AND (((((if(grp.org_id='All',per.org_id is not null,grp.org_id is null OR FIND_IN_SET(per.org_id,grp.org_id)>0))
					AND (if(grp.user_type='All',per.user_type is not null,grp.user_type is null OR FIND_IN_SET(ifnull(per.user_type,''),grp.user_type)>0 ))
					AND (if(grp.employment_type='All',per.employment_type is not null,grp.employment_type is null OR FIND_IN_SET(ifnull(per.employment_type,''),grp.employment_type)>0))
					AND (if(grp.country='All',per.country is not null,grp.country is null OR FIND_IN_SET(ifnull(per.country,''),grp.country)>0))
					AND (if(grp.state='All',per.state is not null,grp.state is null OR FIND_IN_SET(ifnull(concat(per.country,'-',per.state),''),grp.state)>0))
					AND (if(grp.department='All',per.dept_code is not null,grp.department is null OR FIND_IN_SET(ifnull(per.dept_code,''),grp.department)>0))
					AND (if(grp.job_role='All',(select count(1) from slt_person_jobrole_mapping as jobmap where jobmap.user_id=per.id)>0,grp.job_role is null OR FIND_IN_SET(ifnull(spjm.job_role,''),grp.job_role)>0))
					AND (if(grp.language='All',per.preferred_language is not null,grp.language is null OR FIND_IN_SET(ifnull(per.preferred_language,''),grp.language)>0))
					AND (if(grp.is_manager='N',1=1,(ifnull(per.is_manager,'N') = grp.is_manager))
					AND	if(grp.is_instructor='N',1=1,(ifnull(per.is_instructor,'N') = grp.is_instructor)))
					AND (grp.removed_users is null OR FIND_IN_SET(per.id,grp.removed_users)<= 0)) OR (FIND_IN_SET(per.id,grp.added_users)>0))))");

			//$select->condition('grp.is_admin',0,'=');



			/*$groupidArray = explode(",", $groupids);
			$select->condition('grp.id',$groupidArray,'IN');
			$select->groupby('per.id');*/
			$groupids = getGroupidByEntityid($entityId,$entityType);
			if($groupids){
				$select->where("(grp.id IN ($groupids))");
			}
			/*else{
				$select->isNull('grp.id');
			}*/
			expDebug::dPrintDBAPI('joooooo', $select);
		//}
	}catch (Exception $ex){
		watchdog_exception('getGroupidByEntityid', $ex);
		expertusErrorThrow($ex);
	}
}
/**
 * Function added by Vincent for #0071187
 * Simular to filterByGroupAccessPrivileges function instead of filtering the 
 * entity using group table the data will be fetched from pre-populated flat table
 * @param $select
 * @param $entityId
 * @param $entityType
 * @param $groupType
 * @return unknown_type
 */
function filterByGroupAccessPrivilegesUsingFlatTable(&$select,$entityId='',$entityType='',$groupType=0){
	try{
		if($entityType == 'TP'){
			$entityType = array('cre_sys_obt_crt','cre_sys_obt_cur','cre_sys_obt_trn','cre_sys_obt_trp');
		}else{
			$entityType = array('cre_sys_obt_cls');
		}
		$groupids = getGroupidByEntityid($entityId,$entityType);
		if($groupids){
			$userId = (isset($_REQUEST['apiname'])&& empty($userId)) ? $_REQUEST['userid'] :  $userId;
			$userId = empty($userId)? getIdOfLoggedInUser() : $userId;
			//$select->leftJoin('slt_group_mapping','gmap','gmap.entity_id=\''.$entityId.'\' AND gmap.entity_type IN ('.implode(',',$entityType).') and gmap.group_type ='.$groupType.' ');
			$select->leftJoin('slt_group_mapping','gmap','gmap.entity_id=\''.$entityId.'\' AND gmap.entity_type IN ( :etype ) and gmap.group_type ='.$groupType.' ',array(':etype'=>$entityType));
			$select->leftJoin('slt_group_user_mapping','usrg', 'usrg.group_id=gmap.group_id AND usrg.user_id = per.id AND usrg.group_status = \'cre_sec_sts_atv\' and usrgrp.user_type != \'R\'');
			$select->leftJoin('slt_user_access_mapping','umap', 'umap.user_id = per.id AND umap.access_type = 0 AND umap.entity_id = "'.$entityId.'" AND umap.entity_type IN ( :etype1 )',array(':etype1'=>$entityType));
			
			//$groupids = getGroupidByEntityid($entityId,$entityType);
			expDebug::dPrint("TEST 1".print_r($groupids,true),3);
		//if($groupids){
			$select->where("(usrg.group_id IN ($groupids) OR umap.id is not null)");
		}
	}catch (Exception $ex){
		watchdog_exception('getGroupidByEntityid', $ex);
		expertusErrorThrow($ex);
	}

}

function filterByUserAttributes($alisename,$where){
	if(empty($where)){$whr = ' AND ';}else{$whr = '';}
	$userId = getIdOfLoggedInUser();
	$userAccess  = getUserAccessPriviledges($userId);
	expDebug::dPrint("dfsdfsdfsdfdsf".print_r($userAccess,true),5);
	$str = "(if($alisename.created_by = $userId OR $alisename.updated_by = $userId OR FIND_IN_SET($userId,catacs.added_users)>0,1=1,(catacs.removed_users IS NULL OR FIND_IN_SET($userId,catacs.removed_users) <= 0)
	AND (if(catacs.code = 'grp_adm',FIND_IN_SET($userId,catacs.added_users)>0,1=1))
	AND( (if(catacs.country='All','$userAccess->country'!='',(catacs.country IS NULL)) OR (catacs.country = '') OR (FIND_IN_SET('$userAccess->country', catacs.country))) )
	AND( (if(catacs.department='All','$userAccess->department'!='',(catacs.department IS NULL)) OR (catacs.department = '') OR (FIND_IN_SET('$userAccess->department', catacs.department))) )
	AND( (if(catacs.employment_type='All','$userAccess->employment_type'!='',(catacs.employment_type IS NULL)) OR (catacs.employment_type = '') OR (FIND_IN_SET('$userAccess->employment_type', catacs.employment_type))) )
	AND( (if(catacs.is_instructor !='N','$userAccess->is_instructor'='Y',1=1)) )
	AND( (if(catacs.is_manager !='N','$userAccess->is_manager'='Y',1=1)) )
	AND( (if(catacs.job_role='All','$userAccess->job_role'!='',(catacs.job_role IS NULL)) OR (catacs.job_role = '') OR (FIND_IN_SET('$userAccess->job_role', catacs.job_role))) )
	AND( (if(catacs.language='All','$userAccess->language'!='',(catacs.language IS NULL)) OR (catacs.language = '') OR (FIND_IN_SET('$userAccess->language', catacs.language))) )
	AND( (if(catacs.org_id='All','$userAccess->org_id'!='',(catacs.org_id IS NULL)) OR (catacs.org_id = '') OR (FIND_IN_SET('$userAccess->org_id', catacs.org_id))) )
	AND( (if(catacs.state='All','$userAccess->state'!='',(catacs.state IS NULL)) OR (catacs.state = '') OR (FIND_IN_SET('$userAccess->state', catacs.state))) )
	AND( (if(catacs.user_type='All','$userAccess->user_type'!='',(catacs.user_type IS NULL)) OR (catacs.user_type = '') OR (FIND_IN_SET('$userAccess->user_type', catacs.user_type))) )
	AND (
		DATE_FORMAT(catacs.on_or_after_start_date,'%Y-%m-%d') <= DATE_FORMAT($userAccess->hire_date,'%Y-%m-%d') OR 
        DATE_FORMAT(catacs.on_or_before_start_date,'%Y-%m-%d') >= DATE_FORMAT($userAccess->hire_date,'%Y-%m-%d') OR
	    DATE_FORMAT($userAccess->hire_date,'%Y-%m-%d') between DATE_FORMAT(catacs.between_start_date,'%Y-%m-%d') AND DATE_FORMAT(catacs.between_end_date,'%Y-%m-%d')   
	)
	AND (if((catacs.is_admin = 1 AND catacs.userslist is not null AND catacs.userslist != ''),FIND_IN_SET($userId,catacs.userslist)>0,1=1))   )) $whr ";

	return $str;
}

function getUserIdforChat($uid) {

	try{
		global $theme_key;
		//$page = $_REQUEST['page']; // get the requested page
		//$limit = $_REQUEST['rows'];
		$sord = 'ASC'; // Sort is always ascending by full_name

		// Count users active within the defined period.
		//	$interval = REQUEST_TIME - variable_get('exp_sp_user_catalog_block_seconds_online', 900);
		//$loggedInUserId = getIdOfLoggedInUser();
		if($uid > 0)
			$loggedInUserId = getUserIdFromDrupalUid($uid);

		//expDebug::dPrint(' $start = ' . print_r($start, true),4);

		// Select sessions table
		$select = db_select('users', 'u');

		$select->leftJoin('slt_person', 'p', 'u.name = p.user_name');
		//$select->leftJoin('slt_profile_list_items', 'pli', 'p.job_title = pli.code');

		$select->addField('u', 'uid', 'uid');
		$select->addField('p', 'id', 'personid');
		$select->addField('p', 'full_name', 'full_name');

		// Set the conditions
		$select->condition('u.uid', array(0,$uid), 'NOT IN');
		$select->condition('u.status', 1, '=');
		$select->condition('p.status', array('cre_usr_sts_atv'), 'IN');
		$select->condition('p.user_name', 'guest', '!=');
		$select->leftjoin('slt_person_jobrole_mapping','lpjm','lpjm.user_id = p.id ');
		$select->leftjoin('slt_groups','grp',"grp.status = 'cre_sec_sts_atv' AND grp.is_admin=0 AND (((((if(grp.org_id='All',p.org_id is not null,grp.org_id is null OR FIND_IN_SET(p.org_id,grp.org_id)>0))
				AND (if(grp.user_type='All',p.user_type is not null,grp.user_type is null OR FIND_IN_SET(ifnull(p.user_type,''),grp.user_type)>0 ))
				AND (if(grp.employment_type='All',p.employment_type is not null,grp.employment_type is null OR FIND_IN_SET(ifnull(p.employment_type,''),grp.employment_type)>0))
				AND (if(grp.country='All',p.country is not null,grp.country is null OR FIND_IN_SET(ifnull(p.country,''),grp.country)>0))
				AND (if(grp.state='All',p.state is not null,grp.state is null OR FIND_IN_SET(ifnull(concat(p.country,'-',p.state),''),grp.state)>0))
				AND (if(grp.department='All',p.dept_code is not null,grp.department is null OR FIND_IN_SET(ifnull(p.dept_code,''),grp.department)>0))
				AND (if(grp.job_role='All',(select count(1) from slt_person_jobrole_mapping as jobmap where p.id= jobmap.user_id)>0,grp.job_role IS NULL OR (FIND_IN_SET(ifnull(lpjm.job_role,''),ifnull(grp.job_role,''))>0)))
				AND (if(grp.language='All',p.preferred_language is not null,grp.language is null OR FIND_IN_SET(ifnull(p.preferred_language,''),grp.language)>0))
				AND (if(grp.is_manager='N',1=1,(ifnull(p.is_manager,'N') = grp.is_manager))
				AND	if(grp.is_instructor='N',1=1,(ifnull(p.is_instructor,'N') = grp.is_instructor)))
				AND (grp.removed_users is null OR FIND_IN_SET(p.id,grp.removed_users)<= 0)) OR (FIND_IN_SET(p.id,grp.added_users)>0))))");
		$select->leftJoin('slt_group_attributes','grpatt',"grpatt.group_id = grp.id AND(if(grpatt.id is not null,if(grpatt.on_or_after_start_date is not null ,DATE_FORMAT(grpatt.on_or_after_start_date,'%Y-%m-%d') <= DATE_FORMAT(p.hire_date,'%Y-%m-%d'),1=0) OR 
            		if(grpatt.on_or_before_start_date is not null, DATE_FORMAT(grpatt.on_or_before_start_date,'%Y-%m-%d') >= DATE_FORMAT(p.hire_date,'%Y-%m-%d'), 1=0) OR
					if(grpatt.between_start_date is not null ,DATE_FORMAT(p.hire_date,'%Y-%m-%d') between DATE_FORMAT(grpatt.between_start_date,'%Y-%m-%d') AND DATE_FORMAT(grpatt.between_end_date,'%Y-%m-%d'), 1=0),1=1)
				)");
		//AND (if(grp.job_role='All',(select count(1) from slt_person_jobrole_mapping as jobmap where p.id= jobmap.user_id)>0,grp.job_role IS NULL OR (FIND_IN_SET(ifnull(lpjm.job_role,''),ifnull(grp.job_role,''))>0)))

		if($loggedInUserId>0){
			$grpId = getGroupId($loggedInUserId);
		}
		if($loggedInUserId <= 0 || empty($grpId)){
			$select->condition('grp.id','','IS NULL');
		}else if(!empty($grpId)){
			$select->condition('grp.id','','IS NOT NULL');
			$select->where("FIND_IN_SET(grp.id,'$grpId')> 0");
			$select->groupBy("p.id");
		}

		// Descending order by full_name
		$select->orderBy('full_name', $sord);

		// Limit to $limit records
		//$select->range($start, $limit);

		expDebug::dPrintDBAPI(' Fetch users list query 2 = ' , $select );


		// Execure the query and fetch all the records
		$onlineUsersList = $select->execute()->fetchAll();
		expDebug::dPrint(' $items =ss ' . print_r($onlineUsersList, true),3);

		// Prepare and send the response
		/* $response->page = $page;
		 $response->total = $total_pages;
		$response->records = $onlineUsersCount; */
		if (count($onlineUsersList) <= 0){
			$response[] = $uid;
			return $response;
		}
		//$i=0;
		foreach ($onlineUsersList as $onlineUser) {
			//expDebug::dPrint(' $onlineUser = '. print_r($onlineUser, true),3);

			/* $response->rows[$i]['id']=$onlineUser->uid;
			 $response->rows[$i]['personid']=$onlineUser->personid;
			$response->rows[$i]['cell']['id'] = $onlineUser->uid;
			$response->rows[$i]['cell']['personid'] = $onlineUser->personid;
			$response->rows[$i]['cell']['job_title'] = $onlineUser->job_title;
			$response->rows[$i]['cell']['css'] = 'exp-usercatalog-user-rec'; */
			$response[] =	$onlineUser->uid;




			//	$i++;
		}

		return $response;
	}catch(Exception $ex) {
		watchdog_exception('fetch_catalog_users_list_rs', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * Check the user has access permission for the given class or training
*/
function check_access_for_training($userId, $courseId, $classId, $type){
	try{
		if($type== 'class'){
			$select = db_select("slt_course_class", "cls");
			$select->addField("cls", "id");
			$select->addField("cls", "code");
			$select->condition("cls.id", $classId, '=');
			$select->condition("cls.course_id", $courseId, '=');
			filterByUserAccessPrivileges($select, 'cls',$userId,'','cre_sys_obt_cls','Class');
		}else{
			$select = db_select("slt_program", "cls");
			$select->addField("cls", "id");
			$select->addField("cls", "code");
			$select->condition("cls.id", $courseId, '=');
			filterByUserAccessPrivileges($select, 'cls',$userId,'','','Program');
		}

		expDebug::dPrintDBAPI('select = check_access_for_training' , $select);
		$result = $select->execute()->fetchAllKeyed();
		expDebug::dPrint(' $result = for access privilege ' . print_r($result, true) , 3);
		return $result;

	}catch(Exception $ex){
		watchdog_exception('check_access_for_training', $ex);
		expertusErrorThrow($ex);
	}
}

function getTagsWrapperAPI(){
	try{
		expDebug::dPrint("wdsafdsafsa" ,4);
		$entityTypes = array("Course", "Class", "Certification", "Curricula", "Learning Plan");
		$limit = $_REQUEST['limit'];
		$result = getTags($entityTypes, $limit);
		expDebug::dPrint("wdsafdsafsa".print_r($result,true) ,4);
		return $result;

	}catch (Exception $ex) {
		watchdog_exception('getTagsWrapperAPI', $ex);
		expertusErrorThrow($ex);
	}
}
/*
 * Get list of Instructor Scheduled Session
*/
function getInstructorScheduledSession($instructorId,$sessId){
	try {
		$select = db_select('slt_course_class_session', 'sess');
		$select->leftjoin('slt_session_instructor_details','ses_ins','ses_ins.session_id = sess.id');
		$select->leftjoin('slt_course_class','cls','cls.id = sess.class_id');
		$select->leftjoin('slt_person','per','per.id = ses_ins.instructor_id');
		$select->addField('sess', 'id','sessionid');
		$select->addField('sess', 'start_date');
		$select->addField('per', 'first_name');
		$select->addField('per', 'last_name');
		$select->addExpression("DATE_FORMAT((sess.start_date),'%Y-%m-%d')", 'ses_start_date');
		$select->addField('sess', 'start_time');
		$select->addField('cls', 'delivery_type');
		$select->addField('sess', 'end_time');
		$select->addField('sess', 'timezone');
		$select->addField('sess', 'location_id','locationid');
		$select->leftjoin('slt_profile_list_items', 'splt_timezone', 'splt_timezone.code=sess.timezone');
		$select->addField('splt_timezone','attr2', 'session_timezone');
		$select->condition('cls.status', array('lrn_cls_sts_dld','lrn_cls_sts_del','lrn_cls_sts_can'),'NOT IN');
		$select->condition('ses_ins.instructor_id', $instructorId);
		if(!empty($sessId)){
			$select->condition('sess.id', $sessId,'<>');
		}
		expDebug::dPrintDBAPI('$select instructererere', $select);
		$instructor = $select->execute()->fetchAll();
		return $instructor;
	} catch (Exception $ex) {
		watchdog_exception('getInstructorScheduledSession', $ex);
		expertusErrorThrow($ex);
	}
}
function checksortby($gridId, $column){
	try{
		expDebug::dPrint("wdsafdsafsa".print_r($gridId,true) ,4);
		switch($gridId) {
		   case 'content':
				$columnList = array("code","id","total_lesson","type");
				$default = 'id';
				break;
		   case 'equivalence':
		   case 'prerequisite':
		   case 'survey':
		   case 'assessment':
		   case 'TPAttachCourse':
		        $columnList = array("title","code");
		   		$default = 'id';
		   		break;
		   case 'CountrySetting':
		        $columnList = array("name","code");
		        $default = 'id';
		        break;
		   case 'SurAssAttachQuestion':
		   	    $columnList = array("question_txt");
		   	    $default = 'id';
		   	    break;
		   case 'enrolluser':
			   	$columnList = array('status','Date','user_name','full_name','score');/*Viswanathan added for #68932*/
			   	$default = 'id';
			   	break;
		   case 'enrolltpuser':
		   		$columnList = array("full_name","user_name","Date","manager_name","organization_name","overall_status_name");
		   		$default = 'id';
		   		break;
		   case 'grpAddUsers':
		     	$columnList = array("full_name","user_name");
		   		$default = 'user_id';
		   			break;
		   case 'addkeywords':
		   		$columnList = array("keywords","keyworddescription");
		   		$default = 'kid';
		   		break;
		   case 'session':
		   		$columnList = array("title","start_date","start_time","end_time","full_name");
		   		$default = 'session_id';
		   		break;
		   case 'getClassTpEnrollmentResult':
		   		$columnList = array("title","code","delivery_type","Date","Status");
		   		$default = 'id';
		   		break;
	   	   /* case 'grpAddOwners':
	   			$columnList =array("full_name","user_name","name");
	   			$default = 'user_id';
	   			break; */
		   default:
				$default = 'id';
				break;
		}
		if(in_array($column, $columnList)){
			return $column;
		}
		else {
			return $default;
		}
	}
	catch (Exception $ex) {
		watchdog_exception('getTagsWrapperAPI', $ex);
		expertusErrorThrow($ex);
	}
}

function validateSpecialChars($value, $type) {
	try{
		if (empty($value)) {
			return false;
		}

		switch($type) {
			case 'discount_code':
				$validateStatus = !preg_match('/[^a-z_\-0-9]/i', $value);
				break;
			default:
				$validateStatus = !preg_match('/[^a-z_\-0-9]/i', $value);
				break;
		}
		expDebug::dPrint("validate ststs: " . var_export($validateStatus, 1));
		return $validateStatus;
	}
	catch (Exception $ex) {
		watchdog_exception('getTagsWrapperAPI', $ex);
		expertusErrorThrow($ex);
	}
}
function apiUseraccesscheck($userid = '',$entity_type='',$entity_id='',$type = ''){
	try{					
	if(is_data_admin($userid) == true){
		expDebug::dPrint('user is part of Super Admin Group11', 5);
        return 1;
    }
	$users_groups = getGroupsDetails($userid,$entityType,$is_admin = '',$uType='',0);
 	/* $users_owner=getOwnergroups($userid);
	$users_groups=array_merge($users_groups,$users_owner); */
	$users_group_id = array_unique(array_map(function ($ar) {
		return $ar->id;
	}, $users_groups)); 
	$select = db_select('slt_groups','grp');
	$select->addField('grp','id','id');
	$select->condition('grp.id', $users_group_id, 'IN');
	$select->condition('grp.is_admin',1,'=');
	if($entity_type !='' && $entity_id !='' ) {
		$select->leftJoin('slt_group_mapping','sgm','grp.id = sgm.group_id');
		if($type == "tp"){
			$entity_type = explode(',',$entity_type);
			$select->condition('sgm.entity_type', $entity_type, 'IN');
		}
		else{
		$select->condition('sgm.entity_type', $entity_type, '=');
		}
		$select->condition('sgm.entity_id', $entity_id, '=');
	}
	$result = $select->execute()->fetchAll();
	expDebug::dPrintDBAPI('select entity access check', $select);
	return $result;
	}
	catch (Exception $ex) {
		watchdog_exception('apiUseraccesscheck', $ex);
		expertusErrorThrow($ex);
	}
}


function apiUserPrivilegeCheck($privilege_parameter = '', $userid = '', $entity_type_parameter='') {

    $users_groups = getGroupsDetails($userid,$entityType,$is_admin = '',$uType='',0);
    expDebug::dPrint('user belong to groups'.print_r($users_groups,1),5);
  /*  foreach ($users_groups as $group) {
        if($group->grp_code == 'grp_sup') {
            expDebug::dPrint('user is part of Super Admin Group', 5);
            return true;
        }
    }*/
    if(is_data_admin($userid)){
    	expDebug::dPrint('user is part of Super Admin Group11', 5);
    	return true;
    }
    expDebug::dPrint('user is not part of Super Admin Group', 5);
	$privilege_parameter = checkInApiNames($privilege_parameter);
	overrideEntityTypeParameter($entity_type_parameter);
	expDebug::dPrint('$privilege_parameter from function'.print_r($privilege_parameter, 1), 5);
	expDebug::dPrint('$entity_type_parameter from function'.print_r($entity_type_parameter, 1), 5);
// 	$users_owner = getOwnergroups($userid);
// 	expDebug::dPrint('user is the owner of groups'.print_r($users_owner,1),5);
// 	$users_groups=array_merge($users_groups,$users_owner);
	
	$users_group_id = array_unique(array_map(function ($ar) {
		return $ar->id;
	}, $users_groups));
    
    expDebug::dPrint('user group details'.print_r($users_group_id,1),5);
    $api_privilege_para_array = array_filter(explode("$$$",$privilege_parameter));
	
	/*$users_group_id=array_merge($users_group_id, $owner_group_id);
	$users_group_id=array_unique($users_group_id);
	expDebug::dPrint('$users_group_id'. print_r($users_group_id,true),5);*/
	//getEntityTypeByItName($entityName = '');    
	$ownerAccess = false;
    if($entity_type_parameter) {
    	expDebug::dPrint('entity param in access check in if'.$entity_type_parameter, 5);
    	$entity_type_with_id = array_filter(explode('$~$',$entity_type_parameter));
    	
    		foreach ($entity_type_with_id as $key1 => $value1 ) {
    			if($value1 != '') {    					
    				$value_array = array_filter(explode('$$$', $value1));
    				$entity_type = getEntityTypeByItName(trim($value_array[0]));   				
    				$table = getTableByEntityType($entity_type);
    				if($_REQUEST[trim($value_array[1])] != '' ){
    				$select = db_select($table);
    				$select->addField($table, 'id');
    				$select->condition('created_by', $userid, '=');
    				$select->condition('id', escape_string($_REQUEST[trim($value_array[1])]), '=');			
    				$result = $select->execute()->fetchAll();
    				if(count($result) == 0){
    					$ownerAccess = false;
    					break;
    				} else {
    					$ownerAccess = true;
    				}
    			}
    		}
    	}
    }
	if($ownerAccess){  // when user have a owner access skip remaining check
    	return true;
    }else{
    if( (!empty($api_privilege_para_array[1])) &&(!empty($users_group_id[0]))) {
		if(getMappedPermissionForEntityAndGroupID ('Manage Admin API',$users_group_id, $api_privilege_para_array[1],$entity_type_parameter)) {
			$user_access_privileges = getMappedPermissionForEntityAndGroupID ($api_privilege_para_array[0], $users_group_id, $api_privilege_para_array[1], $entity_type_parameter);
			return $user_access_privileges;
		}else {
			$user_access_privileges = false;
			expDebug::dPrint('Manage Admin API privilege is empty or user doesnt belong to any groups');
			return $user_access_privileges;
		}
	}
    }
}

function checkInApiNames($privilege_parameter='') {
    $default_array = array('PreRequisitesAPI','DeleteTagByAPI','UpdateTagByAPI','CreateTagByAPI','GroupCreationAPI','GroupUpdateAPI','UpdateCustomFieldAPI','CreateCustomFieldAPI','ListCustomFieldAPI','deleteCustomFieldAPI','UpdateRegistrationStatusToWaived','RemovePreRequisitesAPI');
	$req  = $_REQUEST;
	if(in_array($req['apiname'],$default_array)) {
		switch ($req['apiname']) {
			case 'GroupCreationAPI':
				if($req['is_admin'] == 1) {
					$prv_array = explode('$$$',$privilege_parameter);
					$prv_array[0] =  'Administrator group';
					$privilege_parameter = implode('$$$',$prv_array);
				}
			case 'GroupUpdateAPI':
				if($req['is_admin'] == 1) {
					$prv_array = explode('$$$',$privilege_parameter);
					$prv_array[0] =  'Administrator group';
					$privilege_parameter = implode('$$$',$prv_array);
				}
				break;
			case 'PreRequisitesAPI':
				if($req['EnitityType'] == 'cre_sys_obt_trp') {
					$prv_array = explode('$$$',$privilege_parameter);
					$prv_array[0] =   'Training Plan';
					$privilege_parameter = implode('$$$',$prv_array);
				}
				break;
			case 'UpdateRegistrationStatusToWaived':
					if($req['type'] == 'tp') {
					$prv_array = explode('$$$',$privilege_parameter);
					$prv_array[0] =   'Training Plan';
					$privilege_parameter = implode('$$$',$prv_array);
				}
				break;
			case 'CreateCustomFieldAPI':
					if(($req['entity_type']) == 'class' || ($req['entity_type']) == 'course')
						$privilege_parameter = 'Course/Class'.'$$$'.'create';
					else if(($req['entity_type']) == 'trainingplan')
						$privilege_parameter = 'Training Plan'.'$$$'.'create';
					else
							$privilege_parameter = ucfirst($_REQUEST['entity_type']).'$$$'.'create';
					break;
			case 'UpdateCustomFieldAPI':
				    if(($req['entity_type']) == 'class' || ($req['entity_type']) == 'course')
						$privilege_parameter = 'Course/Class'.'$$$'.'update';
					else if(($req['entity_type']) == 'trainingplan')
							$privilege_parameter = 'Training Plan'.'$$$'.'update';
					else 
						$privilege_parameter = ucfirst($_REQUEST['entity_type']).'$$$'.'update';
					break;
			case 'ListCustomFieldAPI':
						if(($req['entity_type']) == 'class' || ($req['entity_type']) == 'course')
							$privilege_parameter = 'Course/Class'.'$$$'.'view';
						else if(($req['entity_type']) == 'trainingplan')
								$privilege_parameter = 'Training Plan'.'$$$'.'view';
						else
								$privilege_parameter = ucfirst($_REQUEST['entity_type']).'$$$'.'view';
						break;
		   case 'deleteCustomFieldAPI':
								if(($req['entity_type']) == 'class' || ($req['entity_type']) == 'course')
									$privilege_parameter = 'Course/Class'.'$$$'.'delete';
								else if(($req['entity_type']) == 'trainingplan')
										$privilege_parameter = 'Training Plan'.'$$$'.'delete';
								else
										$privilege_parameter = ucfirst($_REQUEST['entity_type']).'$$$'.'delete';
								break;
			case 'RemovePreRequisitesAPI':
			                        $select = db_select("slt_common_mapping","map");
									$select->addField("map", "object_type", "object_type");
									$select->condition("map.id", $req['id'], "=");
									expDebug::dPrintDBAPI("Appended query-----> ",$select);
									$obj_type = $select->execute()->fetchField();
									if($obj_type == 'cre_sys_obt_crs'){
										$prv_array = explode('$$$',$privilege_parameter);
										$prv_array[0] =  'Course/Class';
										$privilege_parameter = implode('$$$',$prv_array);
									}
		  
									break;
                        case 'CreateTagByAPI':
                        case 'UpdateTagByAPI':
                        case 'DeleteTagByAPI':
                            if(($req['entity_type']) == 'Class' || ($req['entity_type']) == 'Course')
						$privilege_parameter = 'Course/Class'.'$$$'.'update';
					else if(($req['entity_type']) == 'Training Plan')
						$privilege_parameter = 'Training Plan'.'$$$'.'update';
                                        else if(($req['entity_type']) == 'Survey Question')
						$privilege_parameter = 'Survey Questions'.'$$$'.'update';
                                        else if(($req['entity_type']) == 'Assessment Question')
						$privilege_parameter = 'Assessment Questions'.'$$$'.'update';
                                        else if(($req['entity_type']) == 'Content')
						$privilege_parameter = 'Content'.'$$$'.'update';
                                        else
                                            $privilege_parameter = ucfirst($_REQUEST['entity_type']).'$$$'.'update';
                            break;                                                                       
		  
		}

	}
	return $privilege_parameter;
}

function overrideEntityTypeParameter(&$entity_type_parameter) {
	expDebug::dPrint('api name = '.$_REQUEST['apiname']);
	switch ($_REQUEST['apiname']) {
		case 'CreateCustomFieldAPI':
		case 'UpdateCustomFieldAPI':
		case 'ListCustomFieldAPI':
		case 'deleteCustomFieldAPI':
			if(($_REQUEST['entity_type']) == 'trainingplan')
					$entity_type_parameter = 'Training Plan'.'$$$'.'entity_id';
			else
					$entity_type_parameter = ucfirst($_REQUEST['entity_type']).'$$$'.'entity_id';
			break;
		case 'UpdateUserListValuesAPI':
			if(($_REQUEST['code']) == 'cre_usr_dpt')
				$entity_type_parameter = 'Department'.'$$$'.'id';
				else if(($_REQUEST['code']) == 'cre_usr_etp')
					$entity_type_parameter = 'Employment Type'.'$$$'.'id';
				else if(($_REQUEST['code']) == 'cre_usr_jrl')
					$entity_type_parameter = 'Job Role'.'$$$'.'id';
				else if(($_REQUEST['code']) == 'cre_usr_jtl')
					$entity_type_parameter = 'Job Title'.'$$$'.'id';
				else if(($_REQUEST['code']) == 'cre_usr_ptp')
					$entity_type_parameter = 'User Type'.'$$$'.'id';
				break;
		case 'AddUsersInGroup':
				$qry = db_select('slt_groups','grp');
				$qry -> addField('grp', 'id');
				$qry->condition('grp.name', $_REQUEST['group_name'], '=');
				$result= $qry->execute()->fetchField();
				expDebug::dPrintDBAPI('select query groups',$qry);
				$entity_type_parameter=	'Group$$$id';
				$_REQUEST['id'] = $result;
				break;
				
			case 'PreRequisitesAPI':
					if($_REQUEST['EnitityType'] == 'cre_sys_obt_trp')
						$entity_type_parameter= substr_replace($entity_type_parameter,'Training Plan', 0 , 6);
						if ($_REQUEST['PrequisitesType'] == 'cre_sys_pre_trp')
							$entity_type_parameter =  substr_replace($entity_type_parameter,'Training Plan',(strrpos($entity_type_parameter, 'Course', -5)),6);
						expDebug::dPrint('$entity_type_parameter result'.print_r($entity_type_parameter,1),5);
						break;
			case 'RemovePreRequisitesAPI':
					$qry = db_select('slt_common_mapping','cnmp');
					$qry -> addField('cnmp', 'object_type');
					$qry -> addField('cnmp', 'id1');
					$qry->condition('cnmp.id', $_REQUEST['id'], '=');
					$result= $qry->execute()->fetchAll();
					expDebug::dPrintDBAPI('select query',$qry);
					$id = $result[0]->id1;
					if($result[0]->object_type == 'cre_sys_obt_crs')
						$entity_type_parameter = 'Course$$$entity_id';
					else
						$entity_type_parameter = 'Training Plan$$$entity_id';
					$_REQUEST['entity_id'] = $id;
					break;
			case 'UpdateRosterAPI':
						$qry = db_select('slt_enrollment','enr');
						$qry -> addField('enr', 'user_id');
						$qry->condition('enr.id', $_REQUEST['EnrollId'], '=');
						$uid = $qry->execute()->fetchField();
						$entity_type_parameter = 'User$$$uid$~$Class$$$class_id';
						$_REQUEST['uid'] = $uid;
						break;
			case 'UpdateTPRosterAPI':
							$qry = db_select('slt_enrollment','enr');
							$qry -> addField('enr', 'user_id');
							$qry->condition('enr.master_enrollment_id', $_REQUEST['MasterEnrollId'], '=');
							$uid = $qry->execute()->fetchField();
							$entity_type_parameter = 'User$$$uid$~$Training Plan$$$ProgramId';
							$_REQUEST['uid'] = $uid;
							break;
                        case 'CreateTagByAPI':
                        case 'UpdateTagByAPI':
                        case 'DeleteTagByAPI':
                                              if(($_REQUEST['entity_type']) == 'Class')
						$entity_type_parameter = 'Class'.'$$$'.'entity_id';
                                              elseif(($_REQUEST['entity_type']) == 'Course')
                                                $entity_type_parameter = 'Course'.'$$$'.'entity_id';   
                                              else if(($_REQUEST['entity_type']) == 'Training Plan')
						 $entity_type_parameter = 'Training Plan'.'$$$'.'entity_id';
                                              else if(($_REQUEST['entity_type']) == 'Survey Question')
						$entity_type_parameter = 'Survey Questions'.'$$$'.'entity_id';
                                              else if(($_REQUEST['entity_type']) == 'Assessment Question')
						$entity_type_parameter = 'Assessment Questions'.'$$$'.'entity_id';
                                             else if(($_REQUEST['entity_type']) == 'Content')
						$entity_type_parameter = 'Content'.'$$$'.'entity_id';
                                             else 
                                                $entity_type_parameter = ucfirst($_REQUEST['entity_type']).'$$$'.'entity_id';
                                             break;
	}
}
/*
 * getMappedPermissionMapping() - Get Mapped Permission for the Group Id
 */
function getMappedPermissionForEntityAndGroupID($entity_type, $groupId_array='', $privilege_string ='', $entity_type_parameter=''){
	try {
		$entity_type = trim($entity_type);
		$privilege_string = trim($privilege_string);
		expDebug::dPrint('function api_privilege check  user_access_privileges22222333: '.print_r($privilege_string,1),5);
		$select = db_select('slt_groups','grp');
		$select->leftJoin('slt_group_privilege','priv','grp.id = priv.group_id');
		$select->leftJoin('slt_profile_list_items','prli','prli.code = priv.access_page');
		$select->condition('prli.name', $entity_type, '=');
		$select->condition('grp.is_admin',1,'=');
		$select->addField('priv','access_page','grantview');
		$select->addExpression('if(priv.priv_add = 1,concat(\'granted\'),\'\')', 'grantcreate');
		$select->addExpression('if(priv.priv_edit = 1,concat(\'granted\'),\'\')', 'grantupdate');
		$select->addExpression('if(priv.priv_delete = 1,concat(\'granted\'),\'\')', 'grantdelete');
		$select->condition('grp.id', $groupId_array, 'IN');
		expDebug::dPrint('$entity_type_parameter'.var_export($entity_type_parameter, 1), 5);
		if(isset($entity_type_parameter) && (!empty($entity_type_parameter))) {
			$select = objectLevelAccessCheck($entity_type_parameter, $select);
		}
		$result = $select->execute()->fetchAll();
		expDebug::dPrintDBAPI('select privilege', $select);
		foreach($result as $key1 => $value1) {
			foreach($value1 as $key2 => $value2) {
				if($value2 != '') {
					$privilege[$key2] = $value2;
				}
			}
		}
		$grant_privilege_string = 'grant'.$privilege_string;
		expDebug::dPrint('getMappedPermissionForEntityAndGroupID users privilege result :'.print_r($privilege,true),5);
		expDebug::dPrint('$grant_privilege_string :'.print_r($grant_privilege_string,true),5);
		if($grant_privilege_string == 'grantview' && !empty($privilege[$grant_privilege_string])) {
			return true;
		} else if ($privilege[$grant_privilege_string] == 'granted' && !empty($privilege[$grant_privilege_string])){
			return true;
		} else {
			return false;
		}
	}catch(Exception $ex) {
		watchdog_exception('getMappedPermissionMapping', $ex);
		expertusErrorThrow($ex);
	}
}

function objectLevelAccessCheck($entity_type_parameter='', $select='') {
	expDebug::dPrint('entity param in access check'.$entity_type_parameter, 5);
	if(substr_count($entity_type_parameter, '$~$')) {
		expDebug::dPrint('entity param in access check in if'.$entity_type_parameter, 5);
		$entity_type_with_id = array_filter(explode('$~$',$entity_type_parameter));
		$count = count($entity_type_with_id);
		
		if($count > 0 && $entity_type_with_id[0] != '' ) {
			foreach ($entity_type_with_id as $key1 => $value1 ) {
				if($value1 !='') {
					
					$value_array = array_filter(explode('$$$', $value1));
					$req = $_REQUEST;
					$entity_type = getEntityTypeByItName(trim($value_array[0]));
					$entity_id   = array($req[$value_array[1]]);
					expDebug::dPrint('$entity_id '.print_r($entity_id, 1), 5);
					expDebug::dPrint('$entity_type '.print_r($entity_type, 1), 5);
					if($entity_type !='' && $entity_id !='' && objectLevelAccessCheckInGroupMap($entity_type,$entity_id)) {
						$select->leftJoin('slt_group_mapping','sgm'.$key1,'grp.id = sgm'.$key1.'.group_id');
						$entity_type = explode(',',$entity_type);
						$select -> condition('sgm'.$key1.'.entity_type', $entity_type, 'IN');
						$select -> condition('sgm'.$key1.'.entity_id', $entity_id, 'IN');
					}
				}
			}
		}
	} else if(substr_count($entity_type_parameter, '$$$')){
		expDebug::dPrint('entity param in access check in else '.$entity_type_parameter, 5);
		$value_array = array_filter(explode('$$$', $entity_type_parameter));
		expDebug::dPrint('$value_array'.print_r($value_array, 1), 5);
		$req = $_REQUEST;
		$entity_type = getEntityTypeByItName(trim($value_array[0]));
		$entity_id   = array($req[$value_array[1]]);
		expDebug::dPrint('$entity_id '.print_r($entity_id, 1), 5);
		expDebug::dPrint('$entity_type '.print_r($entity_type, 1), 5);
		if($entity_type !='' && $entity_id !='' && objectLevelAccessCheckInGroupMap($entity_type,$entity_id)) {
			$select->leftJoin('slt_group_mapping','sgm','grp.id = sgm.group_id');
			$entity_type = explode(',',$entity_type);
			$select -> condition('sgm.entity_type', $entity_type, 'IN');
			$select -> condition('sgm.entity_id', $entity_id, 'IN');
		}

	}
	expDebug::dPrintDBAPI('select objectLevelAccessCheck', $select);
	return $select;
}

function objectLevelAccessCheckInGroupMap($entity_type,$entity_id) {
	$entity_type = explode(',',$entity_type);
	$select = db_select('slt_group_mapping', 'sgm');
	$select -> addField('sgm', 'id');
	$select -> condition('sgm.entity_type', $entity_type, 'IN');
	$select -> condition('sgm.entity_id', $entity_id, 'IN');
	$select -> condition('sgm.group_type', '1');
	expDebug::dPrintDBAPI('slt_group_map search select query :' , $select);
	$result = $select -> execute() -> fetchAll();
	if(count($result) > 0 ) {
		return true;
	}
}

/**
 * Get the display entity type for the name
 * @param string name
 * @return entity_code
 */
function getEntityTypeByItName($entityName = '') {
	try{
		$grpDisType = array();
		$grpDisType['cre_sys_obt_crs'] = 'Course';
		$grpDisType['cre_sys_obt_cls'] = 'Class';
		$grpDisType['cre_sys_obt_crt,cre_sys_obt_cur,cre_sys_obt_trn'] = 'Training Plan';
		$grpDisType['cre_usr'] = 'User'; // user
		$grpDisType['cre_org'] = 'Organization'; // organization
		$grpDisType['cre_sec'] = 'Group'; // group
		$grpDisType['sry_det_typ_sry'] = 'Survey'; //survey
		$grpDisType['sry_det_typ_ass'] = 'Assessment'; //assessment
		$grpDisType['sry_det_typ_sry_qus'] = 'Survey Questions'; //survey question
		$grpDisType['sry_det_typ_ass_qus'] = 'Assessment Questions'; // Assessment Question
		$grpDisType['cre_sys_obt_cnt'] = 'Content'; //content
		$grpDisType['cbn_anm_typ_ban'] = 'Banner'; //banner
		$grpDisType['cre_ntn'] = 'Notification'; //notification
		$grpDisType['cre_sys_obt_not'] = 'Announcement'; // announcement
		$grpDisType['cme_ord,cme_pmt'] = 'Order'; // order
		$grpDisType['cme_dis'] = 'Discount';
		$grpDisType['cre_cer'] = 'Certificate'; // Certificate
		$grpDisType['cre_sys_obt_loc'] = 'Location';// Location
		$grpDisType['cre_usr_dpt'] ='Department'; //Department
		$grpDisType['cre_usr_etp'] ='Employment Type'; // Employment Type
		$grpDisType['cre_usr_jrl']='Job Role'; //Job Role
		$grpDisType['cre_usr_jtl']='Job Title'; //Job Title
		$grpDisType['cre_usr_ptp']='User Type'; //User Type
		

		/*key and value pair are swaped. By this function(array_flip) only we get a code from it name */

		$grpDisType = array_flip($grpDisType);
		if (array_key_exists($entityName, $grpDisType)) {
			return $grpDisType[$entityName];
		} else {
			return t('LBL272'); // Details
		}
	}catch(Exception $ex){
		watchdog_exception('getEntityDisplayName', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * returns true if the given slt_person id is part of Data Administer group.
    If id is not given, currenly logged in id will be considered 
 * @param int $user_id - id from slt_person table
 * @return boolean
 */
function is_data_admin($user_id = -1) {
	try{
    global $user;
		if($user_id == 1 || (!empty($user->name) && $user->name == 'admin')) {
			return true;
		}
    if($user_id == -1 && !empty($user->name)) {
        $user_id = getIdForUserName($user->name);   //returns the id from slt_person for the logged in user
    }
    $select_group = db_select('slt_groups', 'grp');
    $select_group->addExpression('count(grp.id)', 'cnt');
    $select_group->condition('grp.code', 'grp_sup');
    $select_group->condition('grp.status', 'cre_sec_sts_atv');
    $select_group->where("(FIND_IN_SET(".$user_id.", grp.added_users) > 0 and (grp.removed_users is null OR FIND_IN_SET(".$user_id.", grp.removed_users) <= 0))");
    expDebug::dPrintDBAPI('check if user is in super admin group ', $select_group);
    $result = $select_group->execute()->fetchField();
    expDebug::dPrint('check if user '.$user_id.' is in super admin group - '.print_r($result, 1), 5);
    if($result > 0) {
        return true;
    }
    return false;
	}catch (Exception $ex){
		watchdog_exception('is_data_admin', $ex);
		expertusErrorThrow($ex);
}
}

function getTableByEntityType($entity_type) {
	try{
		$grpDisTable = array();
		$grpDisTable['cre_sys_obt_crs'] = 'slt_course_template';
		$grpDisTable['cre_sys_obt_cls'] = 'slt_course_class';
		$grpDisTable['cre_sys_obt_crt,cre_sys_obt_cur,cre_sys_obt_trn'] = 'slt_program';
		$grpDisTable['cre_usr'] = 'slt_person';
		$grpDisTable['cre_org'] = 'slt_organization';
		$grpDisTable['cre_sec'] = 'slt_groups';
		$grpDisTable['sry_det_typ_sry'] = 'slt_survey';
		$grpDisTable['sry_det_typ_ass'] = 'slt_survey';
		$grpDisTable['sry_det_typ_sry_qus'] = 'slt_survey_questions';
		$grpDisTable['sry_det_typ_ass_qus'] = 'slt_survey_questions';
		$grpDisTable['cre_sys_obt_cnt'] = 'slt_content_master';
		$grpDisTable['cbn_anm_typ_ban'] = 'slt_announcement_master';
		$grpDisTable['cre_ntn'] ='slt_notification_info';
		$grpDisTable['cme_ord,cme_pmt'] = 'slt_order';
		$grpDisTable['cre_cer' ]= 'slt_notification_info';
		$grpDisTable['cme_dis'] = 'slt_discounts';
		$grpDisTable['cre_sys_obt_not'] = 'slt_site_notice'; 
		$grpDisTable['cre_sys_obt_loc'] = 'slt_location';
		$grpDisTable['cre_usr_dpt'] ='Department'; //needs to be checked
		$grpDisTable['cre_usr_etp'] ='Employment Type'; //needs to be checked slt_profile_list_items
		$grpDisTable['cre_usr_jrl']='Job Role'; //needs to be checked
		$grpDisTable['cre_usr_jtl']='Job Title'; //needs to be checked
		$grpDisTable['cre_usr_ptp']='User Type'; //needs to be checked
		
		if (array_key_exists($entity_type, $grpDisTable)) {
			return $grpDisTable[$entity_type];	
		} else {
			return t('LBL272'); // Details
		}
	}catch(Exception $ex){
		watchdog_exception('getTableByEntityType', $ex);
		expertusErrorThrow($ex);
	}
}


// Test commit for CC unit test checking

/*
 * PHP function for Enroll register to program procedure slp_program_enrollment_ins
*/

function lp_program_enrollment_ins($user_id,$program_id,$class_id,$payment_flag,$crtduser_id,$reg_date,$recertifyValue,$cert_days_expire,
									$man,$usercurrencycode,$isMandatory){
	try{
		if($isMandatory==1)
		$man=1;
		$overall_status = 'lrn_tpm_ovr_enr';
		$recertifyFlag = 0;
		$mst_id = 0;
		$cnt_id = 0; 
		
		//Fetch enrollment details 
		$select = db_select('slt_master_enrollment', 'mst');
		//$select->addExpression('COUNT(1)','cnt_id');
		$select->addField('mst', 'id','mst_id');
		$select->addField('mst', 'recertify_path','recertifyFlag');
		$select->addField('mst', 'custom4','custom4');
		$select->addField('mst', 'overall_status','overall_status');
		$select->condition('mst.program_id',$program_id,'=');
		$select->condition('mst.user_id', $user_id , '=');
		$select->condition('mst.is_current', 'Y','=');
		$select->condition('mst.overall_status', array('lrn_tpm_ovr_cmp','lrn_tpm_ovr_exp','lrn_tpm_ovr_cln','lrn_tpm_ovr_inc'),'IN');
		expDebug::dPrintDBAPI('getUserEnrollmentClassCmpStatus = ', $select);
		$select->orderBy('mst.id', 'desc');
		$select->range(0, 1);
		$result = $select->execute()->fetchAll();
		expDebug::dPrint("master enrollment result details ".print_r($result,true),5);
		$objecttype = getTrainingProgramDetails($program_id, array('object_type')); // $objecttype['object_type']
		expDebug::DPrint(" insert object type before ".print_r($objecttype['object_type'],true),5);
		$custom4 = $result[0]->custom4;
		$recertifyFlag = $result[0]->recertifyflag;
		if($result[0]->mst_id > 0){
			if($custom4 == 'lrn_tpm_ovr_cmp'){ 
				$recertifyFlag = $recertifyFlag + 1;
			}
		}else{
			$recertifyFlag = 1;
		}
		if($reg_date == null || $reg_date == 'null' || $reg_date == '')
			$reg_date = now();
		
		if($payment_flag == 'yes') 
			$overall_status = 'lrn_tpm_ovr_ppm';
		elseif($payment_flag == 'no' || $reg_date == null || $reg_date == 'null')
			 $overall_status = 'lrn_tpm_ovr_enr';
		
		$return_status = '';
		
		$wait_list = lp_program_waitlist_check($user_id,$program_id);
		$already_enroll = check_user_enrolled_program($user_id,$program_id,$cert_days_expire);
		if($wait_list >0)
			$return_status = 'WL';
		else if($already_enroll)
			$return_status = 'EP';
		else{
			$insertStmt = db_insert('slt_master_enrollment');
			$fields = array(
				'program_id'    => $program_id,
				'user_id'  => $user_id,
				'overall_status'  => $overall_status,
				'reg_date'  => $reg_date,
				'recertify_path'  => $recertifyFlag,
				'cancel_date'  => NULL,
				'comp_date'  => NULL,
				'percentage_complete' => 0,
				'mandatory' => $man,
				'created_by' => $crtduser_id,
				'created_on' => now(),
				'updated_by' => $crtduser_id,
				'updated_on' => now(),
				'custom0' => NULL,
				'custom1' => NULL,
				'custom2' => NULL,
				'custom3' => NULL,
				'custom4' => NULL,	
				'launchable' => 0,
			);
			$insertStmt->fields($fields);
			expDebug::dPrintDBAPI("Insert TP ",$insertStmt);
			$menrId = $insertStmt->execute();
			
			// Update status from expired to complete
			if($result[0]->mst_id != '' && $result[0]->mst_id != null && $recertifyFlag > 1){
				
				// enrollment update
				$updateMaster = db_update('slt_master_enrollment');
		       // $updateMasterFields['overall_status']=  $custom4;
		        $updateMasterFields['is_current'] = 'N';
		        $updateMasterFields['updated_on'] = now();
		        $updateMaster->fields($updateMasterFields);
		        $updateMaster->condition('id', $menrId, '!=');
		        $updateMaster->condition('user_id', $user_id,'=');
		        $updateMaster->condition('program_id', $program_id,'=');
		        expDebug::dPrintDBAPI("Update TP custom4 ",$updateMaster);
	        	$UpdateId = $updateMaster->execute();
	        	
	        	if($result[0]->overall_status == 'lrn_tpm_ovr_exp'){
	        		$updateMaster = db_update('slt_master_enrollment');
			        $updateMasterFields['overall_status']=  $custom4;
			        $updateMasterFields['updated_on']=  now();
			        $updateMaster->fields($updateMasterFields);
			        $updateMaster->condition('id', $result[0]->mst_id, '=');
			        expDebug::dPrintDBAPI("Update TP custom4 for expirec record alone ",$updateMaster);
		        	$UpdateId = $updateMaster->execute();
	        		
	        	}
	        	
	        	// skill set update
	        	$updateskill = db_update('slt_skill_set');
		        $updateskillFields['is_deleted'] = '1';
		        $updateskill->fields($updateskillFields);
		        $updateskill->condition('id', $result[0]->mst_id,'=');
		        $updateskill->condition('program_id', $program_id,'=');
		        expDebug::dPrintDBAPI("Update TP custom4 ",$updateMaster);
	        	$Updateskill = $updateskill->execute();
        	}
			lp_prg_crs_enrollment_ins($user_id,$menrId,'',$class_id,$program_id,$objecttype['object_type'],$payment_flag,$crtduser_id,'','','','','',$reg_date,$recertifyValue,$usercurrencycode);
			
			$isProgramLaunchable = isProgramLaunchable($menrId);
			if($isProgramLaunchable){
				$updateStmt = db_update('slt_master_enrollment');
				$updateStmt->condition('id', $menrId,'=');
				$updateStmt->fields(array('launchable' => 1));
				expDebug::dPrintDBAPI(' update isProgramLaunchable master enrolment ' , $updateStmt);
				$master_enroll_upd = $updateStmt->execute();
			}
			
			
			
			if($overall_status == 'lrn_tpm_ovr_ppv'){
				$return_status = 'PS';
			}elseif(lp_program_waitlist_check($user_id,$program_id)){
				$return_status = 'WL';
			}else{
				$return_status = 'RG';
			}
			if($menrId !=0)
				lp_mark_program_complete($user_id, $menrId, $program_id, $crtduser_id,'','',$recertifyValue);
		}
		syncSolrData('MasterEnrollment');
		syncSolrData('User');
		//syncSolrData('MasterEnrollment');
		$result = new stdClass();
		$result->status = $return_status;
		$result->masterEnroll = $menrId;
		expDebug::dPrint("Final return status from  lp_program_enrollment_ins " .print_r($result,true),5);
		return $result;
	} catch(Exception $e){
		expDebug::dPrint('Error in lp_program_enrollment_ins'.$e,1);
		expertusLogError($e);
	}									
}

/*
 * PHP function to check user enrolled to program sql function slf_check_program_user_enrolled
*/
function check_user_enrolled_program($user_id,$program_id,$cert_days_expire){
	try{
		$user_already_enrolled = 0;
		if($user_id != ''){
			$selectStmt = db_select('slt_master_enrollment', 'enr');
			$selectStmt->addExpression('COUNT(1)','enr_cnt');
			$selectStmt->condition('enr.program_id',$program_id,'=');
			$selectStmt->condition('enr.user_id', $user_id,'=');
			$selectStmt->condition('enr.overall_status',array('lrn_tpm_ovr_cln','lrn_tpm_ovr_rsc','lrn_tpm_ovr_rsv','lrn_tpm_ovr_inc'),'NOT IN');
			expDebug::dPrintDBAPI("enrollment count >>>",$selectStmt);
			$user_enrollment_count = $selectStmt->execute()->fetchField();
			expDebug::dprint("user enrollment count ".$user_enrollment_count , 5);
			if($user_enrollment_count >0){
				$select = db_select('slt_master_enrollment', 'e');
				$select->leftjoin('slt_program', 'p','p.id=e.program_id');
				$select->addExpression('(CASE WHEN e.overall_status = \'lrn_tpm_ovr_cmp\' AND
										(CURDATE() <= date_format((case 
										when p.expires_in_unit = \'days\' then date_sub(date_add(e.comp_date, INTERVAL p.expires_in_value DAY), INTERVAL '.$cert_days_expire.' DAY)
										when p.expires_in_unit = \'months\' then date_sub(date_add(e.comp_date, INTERVAL p.expires_in_value DAY), INTERVAL '.$cert_days_expire.' MONTH)
										when p.expires_in_unit = \'years\' then date_sub(date_add(e.comp_date, INTERVAL p.expires_in_value DAY), INTERVAL '.$cert_days_expire.' YEAR)
									END),\'%Y-%m-%d\'))THEN 1
									 WHEN (e.overall_status NOT IN (\'lrn_tpm_ovr_cmp\',\'lrn_tpm_ovr_exp\')) THEN 1 
	 	 													ELSE 0 END)','user_already_enrolled');
	 	 		$select->condition('e.program_id',$program_id,'=');
				$select->condition('e.user_id', $user_id,'=');		
				$select->condition('e.is_current', 'Y','=');
				$select->condition('e.overall_status',array('lrn_tpm_ovr_cln','lrn_tpm_ovr_rsc','lrn_tpm_ovr_rsv','lrn_tpm_ovr_inc'),'NOT IN');
				expDebug::dPrintDBAPI("user enrollment count inside ",$select);
				$user_already_enrolled = $select->execute()->fetchField();	
			}else{
				$user_already_enrolled = $user_enrollment_count;
			}
		}
		expDebug::dprint("user enrollment count final ".$user_already_enrolled , 5);
		return $user_already_enrolled;
	} catch(Exception $e){
		expDebug::dPrint('Error in check_user_enrolled_program'.$e,1);
		expertusLogError($e);
	}
}

/*
 * PHP function to validate waitlist check sql function slf_program_waitlist_check
*/
function lp_program_waitlist_check($user_id,$program_id){
	try{
		//get waitlist count
		$selectStmt = db_select('slt_master_enrollment', 'wtl');
		$selectStmt->addExpression('COUNT(1)','wtl_cnt');
		$selectStmt->condition('wtl.program_id',$program_id,'=');
		$selectStmt->condition('wtl.user_id', $user_id,'=');
		$selectStmt->condition('wtl.overall_status','lrn_tpm_ovr_wtl','=');
		expDebug::dPrintDBAPI(' lp_program_waitlist_check  ',$selectStmt);
		$waitlist_count = $selectStmt->execute()->fetchField();
		expDebug::dPrint("waitlist query result ".$waitlist_count,4);
		return $waitlist_count;
	} catch(Exception $e){
		expDebug::dPrint('Error in lp_program_waitlist_check '.$e,1);
		expertusLogError($e);
	}	
}

/*
 * PHP function for sql procedure slp_program_crsenrollment_ins
*/
function lp_prg_crs_enrollment_ins($user_id,$menrId,$course_id = '',$class_id,$program_id,$objecttype,$payment_flag,
								$crtduser_id,$orderitem_custom0 = NULL,$orderitem_custom1 = NULL,$orderitem_custom2 = NULL,$orderitem_custom3 = NULL,$orderitem_custom4 = NULL,$reg_date,$recertifyValue,$usercurrencycode){
  try{
  	
  	$module_id = getModuleIdFromObjectId($program_id,$recertifyValue);
  	expDebug::dPrint("lp module_id ::".print_r($module_id,1),5);
  	
	$reg_status = NULL;
	$comp_status = NULL;
	$order_status = 'lrn_crs_reg_cnf';
	$class_ids = explode(',',$class_id);
	$enrollment_count = 0;
	if(strtolower($payment_flag) == 'yes'){
	 $reg_status = 'lrn_crs_reg_ppm';
	 $comp_status = NULL;
	 $order_status = 'lrn_crs_reg_ppm';
	}else if(strtolower($payment_flag) == 'no' || strtolower($payment_flag) == 'null' || $payment_flag == NULL){
	  $reg_status = 'lrn_crs_reg_cnf';
	  $comp_status = 'lrn_crs_cmp_enr';
	  $order_status = 'lrn_crs_reg_cnf';
	}
	expDebug::dPrint(' $reg_status '. $reg_status .' $comp_status '.$comp_status .' $order_status '. $order_status , 5);
	//Order insert
	$insertStmt = db_insert('slt_order');
    $insertStmt->fields(array(
      'user_id' => $user_id,
      'order_status' => $order_status,
      'created_by' => $crtduser_id,
      'created_on' => now(),
      'updated_by' => $crtduser_id,
      'updated_on' => now()
    ));
    expDebug::dPrintDBAPI("slt_order insert ",$insertStmt);
    $orderId = $insertStmt->execute();
	
    expDebug::dPrint("program object type ".$objecttype,4);
	
	if($objecttype == 'cre_sys_obt_crt' || $objecttype ==  'cre_sys_obt_cur' || $objecttype == 'cre_sys_obt_trn'){
	
		//if($recertifyValue != 'R')
			$selectStmt = db_select('slt_module_crs_mapping', 'map');
		//else
		//	$selectStmt = db_select('slt_re_certify', 'map');
	
		$selectStmt->innerJoin('slt_course_template','crs','crs.id = map.course_id and crs.status = \'lrn_crs_sts_atv\'');
		$selectStmt->addField('crs','id','id');
		$selectStmt->condition('map.program_id',$program_id,'=');
		$selectStmt->condition('map.module_id',$module_id[0]->module_id,'=');
		expDebug::dPrintDBAPI("select list of courses ",$selectStmt);
		$crs_ids = $selectStmt->execute()->fetchCol();
		expDebug::dPrint("result list of courses ".print_r($crs_ids,true),5);
		foreach($crs_ids as $cid){
		
			//Enrollment count
			$selectStmt = db_select('slt_enrollment', 'enr');
			$selectStmt->addExpression('COUNT(1)','enr_cnt');
			$selectStmt->condition('enr.course_id',$cid,'=');
			$selectStmt->condition('enr.user_id', $user_id,'=');
			$selectStmt->condition('enr.reg_status',array('lrn_crs_reg_cnf', 'lrn_crs_reg_ppm', 'lrn_crs_reg_rsv'),'IN');
			$selectStmt->where('IF(enr.reg_status = \'lrn_crs_reg_cnf\', enr.comp_status IN(\'lrn_crs_cmp_cmp\',\'lrn_crs_cmp_inp\',\'lrn_crs_cmp_enr\',\'lrn_crs_cmp_att\'),1=1)');
			expDebug::dPrintDBAPI("select enrollment count ",$selectStmt);
			$enrollment_count = $selectStmt->execute()->fetchField();
			expDebug::dPrint("enrollment count ".$enrollment_count,4);
			
			$select = db_select('slt_course_class', 'cls');
			$select->addField('cls','id','classid');
			$select->addField('cls','delivery_type','deliverytype');
			$select->condition('cls.course_id',$cid,'=');
			$select->condition('cls.id',$class_ids,'IN');
			$select->where('IF(('.$enrollment_count.' = 0) ,(STATUS = "lrn_cls_sts_atv"),1=1)');
			//$select->condition('cls.status','lrn_cls_sts_atv','=');
			$select->range(0,1);
			expDebug::dPrintDBAPI("get delivery type of class with enrollment count ",$select);
			$results = $select->execute()->fetchAll();
			expDebug::dPrint("final result set ".print_r($results,true),5);
			if($results[0]->deliverytype == 'lrn_cls_dty_ilt' || $results[0]->deliverytype == 'lrn_cls_dty_vcl'){
				//User enrolled for particular class
				$selectStmt = db_select('slt_enrollment', 'enr');
				$selectStmt->addExpression('COUNT(1)','enr_cnt');
				$selectStmt->condition('enr.course_id',$cid,'=');
				$selectStmt->condition('enr.user_id', $user_id,'=');
				$selectStmt->condition('enr.class_id', $results[0]->classid,'=');
				$selectStmt->condition('enr.reg_status',array('lrn_crs_reg_cnf','lrn_crs_reg_ppm', 'lrn_crs_reg_rsv'),'IN');
				expDebug::dPrintDBAPI("User enrolled for particular class  ",$selectStmt);
				$usrEnrcheck = $selectStmt->execute()->fetchField();
				
				//waitlist check 
				$wait_count = lp_waitlist_check_class($cid,$results[0]->classid);
				expDebug::dPrint(' $wait_count '.$wait_count,5);
				if($wait_count > 0 && $usrEnrcheck == 0){
					$reg_status = 'lrn_crs_reg_wtl';
			  		$comp_status = NULL;
			  		$waitlist_flag = 'lrn_crs_reg_wtl';
		  	  		$waitlist_priority = $wait_count;
				}else{
					$reg_status = 'lrn_crs_reg_cnf';
			   		$comp_status = 'lrn_crs_cmp_enr';
				    $order_status = 'lrn_crs_reg_cnf';
				    $waitlist_flag = NULL;
			  	    $waitlist_priority = NULL;
				}
			}else{
					$reg_status = 'lrn_crs_reg_cnf';
				  	$comp_status = 'lrn_crs_cmp_enr';
				  	$order_status = 'lrn_crs_reg_cnf';
				  	$waitlist_flag = NULL;
			  	  	$waitlist_priority = NULL;
			} 			
			//course enrollment insert
			lp_course_enrollment_ins($user_id,$menrId,$cid,$results[0]->classid,$orderId,$reg_status,$comp_status,$waitlist_flag,$waitlist_priority,$crtduser_id,$reg_date,$recertifyValue);
			insertOrderItem($orderId, $cid, $results[0]->classid,$user_id,$program_id,$objecttype,$crtduser_id,$orderitem_custom0,$orderitem_custom1,$orderitem_custom2,$orderitem_custom3,$orderitem_custom4);
		}	
	}
	//update order id in master enrollment 
	$updateStmt = db_update('slt_master_enrollment');
    $updateStmt->condition('id',$menrId,'=');
    $updateStmt->fields(array('order_id' => $orderId,'updated_on'=>now()));
    expDebug::dPrintDBAPI(' update master enrollment with order id ' , $updateStmt);
    $master_enroll_upd = $updateStmt->execute();
  } catch(Exception $e){
		expDebug::dPrint('Error in lp_prg_crs_enrollment_ins '.$e,1);
		expertusLogError($e);
	}
}

/*
 * PHP function for sql procedure slf_waitlist_check
*/
function lp_waitlist_check_class($course_id,$class_id){
	try{
		$enroll_count = 0;
		$waitlist_count = 0; 
		$max_capacity = -1;
		
		//Enrollment total count
		$selectStmt = db_select('slt_enrollment', 'enr');
		$selectStmt->addExpression('COUNT(1)','enr_cnt');
		$selectStmt->addField('enr','id','id');
		$selectStmt->condition('enr.course_id',$course_id,'=');
		$selectStmt->condition('enr.class_id', $class_id,'=');
		$selectStmt->condition('enr.reg_status',array('lrn_crs_reg_cnf','lrn_crs_reg_ppm','lrn_crs_reg_rsv'),'IN');
		$selectStmt->groupBy('enr.user_id');
	  	$selectStmt->groupBy('enr.reg_status');
	  	expDebug::dPrintDBAPI("Enrollment total count  ",$selectStmt);
		$enroll_count = $selectStmt->execute()->fetchAll();
		
		//waitlist count
		$selectStmt = db_select('slt_enrollment', 'enr');
		$selectStmt->addExpression('COUNT(1)','wtl_cnt');
		$selectStmt->addField('enr','id','id');
		$selectStmt->condition('enr.course_id',$course_id,'=');
		$selectStmt->condition('enr.class_id', $class_id,'=');
		$selectStmt->condition('enr.reg_status',array('lrn_crs_reg_wtl','lrn_crs_reg_cnf','lrn_crs_reg_ppm','lrn_crs_reg_rsv'),'IN');
		$selectStmt->condition('enr.waitlist_flag','lrn_crs_reg_wtl','=');
		$selectStmt->groupBy('enr.user_id');
	  	$selectStmt->groupBy('enr.reg_status');
	  	expDebug::dPrintDBAPI("waitlist count count  ",$selectStmt);
		$waitlist_count = $selectStmt->execute()->fetchAll();
		
		//Maximum capacity
		$max_capacity = getClassMaxCapacity($course_id, $class_id);
		
		expDebug::dPrint(' $enroll_count '. $enroll_count .' $waitlist_count '. $waitlist_count .' $max_capacity '.$max_capacity,4);
		if(count($enroll_count) >= $max_capacity){
			$retun_cnt = count($waitlist_count)+1; // Waitlist number for the new class. 
		}elseif(count($enroll_count) == ($max_capacity-1)){
			$retun_cnt = -1;
		}else{
			$retun_cnt = 0;
		}
		return $retun_cnt;
	} catch(Exception $e){
		expDebug::dPrint('Error in lp_prg_crs_enrollment_ins '.$e,1);
		expertusLogError($e);
	}
}

/*
 * PHP function for sql procedure slp_course_enrollment_ins
*/
function lp_course_enrollment_ins($user_id,$menrId,$courseid,$classid,$orderId,$reg_status,$comp_status,$waitlist_flag,$waitlist_priority,$crtduser_id,$reg_date,$recertifyValue){
	try{
	//Completed class cannot be enrolled again with Training Plan 
	$selectStmt = db_select('slt_enrollment', 'enr');
	$selectStmt->addExpression('COUNT(1)','enr_cnt');
	$selectStmt->condition('enr.user_id', $user_id,'=');
	$selectStmt->condition('enr.class_id', $classid,'=');
	$selectStmt->condition('enr.comp_status','lrn_crs_cmp_cmp','=');
	expDebug::dPrintDBAPI("Completed class cannot be enrolled ",$selectStmt);
	$clsCompleted = $selectStmt->execute()->fetchField();

	//Recertify Path of master enrollment
	$selectStmt = db_select('slt_master_enrollment', 'mst');
	$selectStmt->addField('mst','recertify_path','recertify_path');
	$selectStmt->condition('mst.id',$menrId,'=');
	expDebug::dPrintDBAPI("Recertify Path of master enrollment ",$selectStmt);
	$recertify_path = $selectStmt->execute()->fetchField(); 
	
	$comp_sts = ($clsCompleted > 0 && $recertifyValue != "R") ? 'lrn_crs_cmp_cmp' : '';
	$reg_date = ($reg_date == 'null' || $reg_date == null || $reg_date == '') ? now() : $reg_date;
	
	



	//Added for launchable check
	$launchable = 1;
	//get delivery type of class
	$cls_delivery_type = getClassDeliveryType($classid,'Short');
	expDebug::dPrint("class delivery type >>  ".print_r($cls_delivery_type,true),5);
	if(($cls_delivery_type == 'ilt' || $cls_delivery_type == 'vc')){
		$launchable  = 0;
		$isPostAssessmentAttached = isPostAssessmentAttached($classid,'class');
		if($isPostAssessmentAttached == 1){
			$launchable  = 1;
		}
	}
	
	expDebug::dPrint("lp_course_enrollment_ins launchable >>  ".$launchable,5);
	
	
	//slt_enrollment insert
	$insertStmt = db_insert('slt_enrollment');
	$fields = array(
		'master_enrollment_id'    => $menrId,
		'user_id'  => $user_id,
		'course_id'  => $courseid,
		'class_id'  => $classid,
		'order_id'  => $orderId,
		'reg_status'  => $reg_status,
		'reg_date'  => $reg_date,
		'reg_status_date' => $reg_date,
		'recertify_path' => $recertify_path,
		'comp_status' => $comp_status,
		'comp_date' => NULL,
		'valid_from' => NULL,
		'valid_to' => NULL,
		'score' => NULL,
		'grade' => NULL,
		'waitlist_flag' => $waitlist_flag,
		'waitlist_priority' => $waitlist_priority,
		'created_by' => $crtduser_id,
		'created_on' => now(),
		'updated_by' => $crtduser_id,
		'updated_on' => now(),
		'custom0' => NULL,
		'custom1' => NULL,
		'custom2' => NULL,
		'custom3' => NULL,
		'custom4' => NULL,
		'launchable'=>$launchable
	);
	$insertStmt->fields($fields);
	expDebug::dPrintDBAPI("Insert class enrollments ",$insertStmt);
	$enrId = $insertStmt->execute();
	
	//get delivery type of class
	$cls_delivery_type = getClassDeliveryType($classid,'Short');
	expDebug::dPrint("class delivery type >>  ".print_r($cls_delivery_type,true),5);
	if($cls_delivery_type == 'wbt' || $cls_delivery_type == 'vod'){
		//Insert content details
		$enrollContentObj = new stdClass();
		$enrollContentObj->class_id = $classid;
		$enrollContentObj->enroll_id = $enrId;
		$enrollContentObj->user_id = $user_id;
		$enrollContentObj->course_id = $courseid;
		$enrollContentObj->ctuser = $crtduser_id;
		insertEnrollmentContentMapping($enrollContentObj,'lp_register');
	}else{
		//active enrollment count
		$selectStmt = db_select('slt_enrollment', 'enr');
		$selectStmt->addExpression('COUNT(1)','enr_cnt');
		$selectStmt->condition('enr.course_id',$courseid,'=');
		$selectStmt->condition('enr.class_id', $classid,'=');
		$selectStmt->condition('enr.reg_status',array('lrn_crs_reg_cnf', 'lrn_crs_reg_ppm', 'lrn_crs_reg_rsv'),'IN');
		$selectStmt->where('IF(reg_status = \'lrn_crs_reg_cnf\', comp_status IN(\'lrn_crs_cmp_cmp\',\'lrn_crs_cmp_inp\',\'lrn_crs_cmp_enr\',\'lrn_crs_cmp_att\'),1=1)');
		$enrollment_cnt = $selectStmt->execute()->fetchField();
		$max_capacity = getClassMaxCapacity($courseid, $classid);
		if($enrollment_cnt == $max_capacity){
			insertNotification('ClassMaxCapacityReached', $courseid, $classid, $user_id, $crtduser_id);
		}
	}
	// If the user has already completed the course, credit the completion for the course 
	$completion_details = getLastCompCourseClass($courseid,$user_id,$classid,$menrId);
	if($completion_details->comp_status != ''){
		$updateStmt = db_update('slt_enrollment');
	    $updateStmt->condition('user_id', $user_id,'=');
	    $updateStmt->condition('reg_status', 'lrn_crs_reg_cnf','=');
	    $updateStmt->condition('comp_status', array('lrn_crs_cmp_cmp','lrn_crs_cmp_inc','lrn_crs_cmp_nsw'),'NOT IN');
		$updateStmt->condition('recertify_path', 1);
	    $updateStmt->condition('course_id', $courseid,'=');
	    $updateStmt->condition('class_id', $classid,'=');
	    $updateStmt->fields(array(
				          'comp_status' => $completion_details->comp_status,
				          'comp_date' => $completion_details->comp_date,
				          'comp_on' => $completion_details->comp_on,
				          'comp_by' => $completion_details->comp_by,
				          'content_status' => $completion_details->content_status,
				          'progress' => $completion_details->progress,
				          'score' => $completion_details->score,
	    				  'updated_on' =>now()
	                        ));
	    expDebug::dPrintDBAPI(' update slt_enrollment if class is already completed = ' , $updateStmt);
	    $num_updated = $updateStmt->execute();
	}
	if($waitlist_flag == 'lrn_crs_reg_wtl'){
		$updateStmt = db_update('slt_master_enrollment');
	    $updateStmt->condition('id', $menrId,'=');
	    $updateStmt->fields(array('overall_status' => 'lrn_tpm_ovr_wtl','updated_on'=>now()));
	    expDebug::dPrintDBAPI(' update slt_enrollment if class is already waitlisted = ' , $updateStmt);
	    $master_enroll_upd = $updateStmt->execute();
	}
	syncSolrData('User');
	syncSolrData('Enrollment');
  	} catch(Exception $e){
		expDebug::dPrint('Error in lp_course_enrollment_ins '.$e,1);
		expertusLogError($e);
	}
}

/*
 * PHP function for sql procedure slp_mark_program_complete
*/
function lp_mark_program_complete($user_id,$master_enrollment_id,$program_id,$upduser_id,$notifyby,$callfrom,$recertifyValue='',$score='',&$statements=array()){
  try{	
  	
	$mandatory = 0;
	$compDate = getMasterEnrollCompletionDate($master_enrollment_id);
  	
	//get tp content status
	$master_enroll_content_status = getTpContentStatus($master_enrollment_id);
	
	//get recertify path
	$selectStmt = db_select('slt_master_enrollment', 'enr');
	$selectStmt->addField('enr','recertify_path','recertify_path');
	$selectStmt->condition('enr.id',$master_enrollment_id,'=');
	$selectStmt->condition('enr.is_current','Y','=');
	$certify_flag = $selectStmt->execute()->fetchField();
	
	//Master enrollment details
	$selectStmt = db_select('slt_master_enrollment', 'enr');
	$selectStmt->addField('enr','overall_status','overall_status');
	//Adding fields for #91074
	$selectStmt->addField('enr','score','score');
	$selectStmt->addField('enr','comp_date','comp_date');
	$selectStmt->addField('enr','updated_by','updated_by');
	$selectStmt->addField('enr','updated_on','updated_on');
	$selectStmt->condition('enr.id',$master_enrollment_id,'=');
	$selectStmt->condition('enr.user_id',$user_id,'=');
	$selectStmt->condition('enr.program_id',$program_id,'=');
	//$prevStatus = $selectStmt->execute()->fetchField();
	$selectStmtRes = $selectStmt->execute()->fetchAll();
	$prevStatus = $selectStmtRes[0]->overall_status;

	// for audit trail
	$overallstaus_old = $prevStatus;
	
	//get module id from program id
	$module_id = getModuleIdFromObjectId($program_id,$certify_flag);
	
	//get mandatory count
	$selectStmt = db_select('slt_module_crs_mapping', 'map');
	$selectStmt->addExpression('COUNT(map.is_required)','required');
	$selectStmt->condition('map.program_id',$program_id,'=');
	$selectStmt->condition('map.is_required','Y','=');
	$selectStmt->condition('map.module_id',$module_id[0]->module_id,'=');
	$mandatory = $selectStmt->execute()->fetchField();
	
	if($callfrom != 'program' || $notifyby !='manager'){
	    $selectStmt = db_select('slt_survey_mapping', 'assmap');
	    $selectStmt->join('slt_survey', 'srv','srv.id = assmap.survey_id AND srv.type = \'sry_det_typ_ass\'');
		$selectStmt->addExpression('count(1)','assessment');
		$selectStmt->condition('assmap.object_type','cre_sys_obt_cls','!=');
		$selectStmt->condition('assmap.object_id',$program_id,'=');
		$selectStmt->condition('assmap.module_id',$module_id[0]->module_id,'=');
		$selectStmt->condition('assmap.pre_status',0,'=');
		$isHasAssesment = $selectStmt->execute()->fetchField();
	}else
    	$isHasAssesment = 0; 
    	
    //check incomplete class count
    $selectStmt = db_select('slt_enrollment', 'enroll');
    $selectStmt->join('slt_master_enrollment', 'menroll','menroll.id = enroll.master_enrollment_id');
    $selectStmt->innerjoin('slt_module_crs_mapping', 'map','map.program_id = menroll.program_id AND enroll.course_id = map.course_id');
	$selectStmt->addExpression('count(DISTINCT enroll.id)','incom_class');
	$selectStmt->condition('enroll.comp_status',array('lrn_crs_cmp_inc','lrn_crs_cmp_nsw'),'IN');
	$selectStmt->condition('map.is_required','Y','=');
	$selectStmt->condition('menroll.id',$master_enrollment_id,'=');
	$incom_class = $selectStmt->execute()->fetchField();	
  	if($isHasAssesment == 0 || $incom_class > 0){  
  		
      	 // Enrollment values needs to be checked before updating the program  
	      $selectEnrollQuery = db_select('slt_enrollment', 'enroll');
	      $selectEnrollQuery->innerjoin('slt_module_crs_mapping', 'mapping', 'mapping.course_id = enroll.course_id');
	      $selectEnrollQuery->addField('enroll', 'master_enrollment_id');
	      $selectEnrollQuery->addField('enroll', 'user_id');
	      if($mandatory>0){
	      	$selectEnrollQuery->addExpression('MAX(CASE WHEN enroll.comp_status=\'lrn_crs_cmp_cmp\' THEN 0 ELSE 1 END)', 'cmp_status');
	      	$selectEnrollQuery->addExpression('MAX(CASE WHEN enroll.comp_status in (\'lrn_crs_cmp_enr\',\'lrn_crs_cmp_inp\',\'lrn_crs_cmp_att\',\'lrn_crs_cmp_nsw\') THEN 1 ELSE 0 END)', 'inp_status');
	      	$selectEnrollQuery->addExpression('MAX(CASE WHEN enroll.comp_status in (\'lrn_crs_cmp_inc\',\'lrn_crs_cmp_nsw\') THEN 1 ELSE 0 END)', 'inc_status');
	      	$selectEnrollQuery->condition('mapping.is_required', 'Y','=');
	      }else{
	      	$selectEnrollQuery->addExpression('MIN(CASE WHEN enroll.comp_status=\'lrn_crs_cmp_cmp\' THEN 0 ELSE 1 END)', 'cmp_status');
	      	$selectEnrollQuery->addExpression('MAX(CASE WHEN enroll.comp_status in (\'lrn_crs_cmp_enr\',\'lrn_crs_cmp_inp\',\'lrn_crs_cmp_att\') THEN 1 ELSE 0 END)', 'inp_status');
	      	$selectEnrollQuery->addExpression('MAX(CASE WHEN enroll.comp_status in (\'lrn_crs_cmp_inc\',\'lrn_crs_cmp_nsw\') THEN 1 ELSE 0 END)', 'inc_status');
	      }
	      
	      $selectEnrollQuery->condition('mapping.program_id', $program_id,'=');
	      $selectEnrollQuery->condition('enroll.master_enrollment_id', $master_enrollment_id,'=');
	      $selectEnrollQuery->condition('mapping.module_id',$module_id[0]->module_id,'=');
	      $selectEnrollQuery->groupby('enroll.master_enrollment_id');
	      $selectEnrollQuery->groupby('enroll.user_id');
	      expDebug::dPrintDBAPI(' $selectEnrollQuery = ' , $selectEnrollQuery);
	      $selectEnrollResult = $selectEnrollQuery->execute()->fetchAll();

	      $cmp_status = $selectEnrollResult[0]->cmp_status;
	      $inp_status = $selectEnrollResult[0]->inp_status;
	      $inc_status = $selectEnrollResult[0]->inc_status;
	      expDebug::dPrint(' $completionStatus '.$cmp_status .' $InprogressStatus '.$inp_status .' $IncompleteStatus '.$inc_status.' $$mandatory '.$mandatory ,5);
	      //if(($completionStatus !='' && ($completionStatus == 0 || $completionStatus == 1)) && ($InprogressStatus != '' && ($InprogressStatus==0 || $IncompleteStatus==1))){
	      $overallstatus = '';
	      if($mandatory>0){
	          if($cmp_status == 0 && $inp_status == 0)
	          	$overallstatus = 'lrn_tpm_ovr_cmp';
	          else if($cmp_status == 1 && $inc_status == 1)
	          	$overallstatus = 'lrn_tpm_ovr_inc';
	      }else{
	      	  if($cmp_status == 0 && $inp_status == 0)
	          	$overallstatus = 'lrn_tpm_ovr_cmp';
	          else if($cmp_status == 1 && $inp_status == 0 && $inc_status == 1)
	          	$overallstatus='lrn_tpm_ovr_inc';
	      }  
		  
          $updateMasterField = array();
          // Update master enrollment
         expDebug::dPrint("overall status final >> ".$overallstatus ,4);
       if($overallstaus_old == 'lrn_tpm_ovr_cmp' && !empty($score)){
         	$updateMasterField['score'] = $score;
         }else{
          if(!empty($overallstatus)){
          	$updateMasterField['overall_status'] = $overallstatus;
          }
          if(!empty($score)){
          	$updateMasterField['score'] = $score;
          }
          if($mandatory>0){
		     if(($cmp_status == 0 || $cmp_status == 1) && ($inc_status == 1 || $inp_status == 0) ){ 
				$updateMasterField['comp_date'] = $compDate;
		      	$updateMasterField['updated_by'] = $upduser_id;
		      	$updateMasterField['updated_on'] = now();
		     }
          }else{
          	if(($cmp_status == 0 || $cmp_status == 1) && ($inp_status == 0) ){
          		$updateMasterField['comp_date'] = $compDate;
          		$updateMasterField['updated_by'] = $upduser_id;
          		$updateMasterField['updated_on'] = now();
          	}
          }
         }
         
      	  if(count($updateMasterField) > 0 ){
		      $updateMasterEnroll = db_update('slt_master_enrollment');
		      $updateMasterEnroll->fields($updateMasterField);
		      $updateMasterEnroll->condition('id', $master_enrollment_id,'=');
		      $updateMasterEnroll->condition('user_id', $user_id,'=');
		      $updateMasterEnroll->condition('program_id', $program_id,'=');
		      $updateMasterEnroll->condition('overall_status', array('lrn_tpm_ovr_cln','lrn_tpm_ovr_exp','lrn_tpm_ovr_ppm','lrn_tpm_ovr_ppv','lrn_tpm_ovr_inc'), 'NOT IN');
		      $updateMasterEnroll->execute();  
		      //commenting out for #0091074 
		    /*   if(!empty($statements)) {
		      	$statements[] = $updateMasterEnroll;
		      } else {
		      	$updateMasterEnroll->execute();
		      } */
		      expDebug::dPrintDBAPI('updateMasterEnroll query :',$updateMasterEnroll);
		      
		      $result= new stdClass();
		      $result->col_name = 'id';
		      $result ->col_value = $master_enrollment_id;
		      $result ->table_name ='slt_master_enrollment';
		      $result ->fields =array(
		              'overall_status' => $prevStatus,
		              'score' => $selectStmtRes[0]->score,
		              'comp_date' => $selectStmtRes[0]->comp_date,
		              'updated_by' => $selectStmtRes[0]->updated_by,
		              'updated_on' => $selectStmtRes[0]->updated_on
		      );
		      $statements['update'][] = $result;
		      
		      
		      
      	  }
	}
	
	// To update percentage of complete - old column
	updateTPEnrollmentPercentComplete($program_id, $master_enrollment_id, $certify_flag,$statements);

	// To update  a overall percentage complete used in content player - new column
	updateTPEnrollmentOverallPercentComplete($program_id, $master_enrollment_id, $certify_flag,'',$statements);
	
	$selectStmt = db_select('slt_master_enrollment', 'enr');
	$selectStmt->addField('enr','overall_status','overall_status');
	$selectStmt->condition('enr.id',$master_enrollment_id,'=');
	$selectStmt->condition('enr.user_id',$user_id,'=');
	$selectStmt->condition('enr.program_id',$program_id,'=');
	$aftrStatus = $selectStmt->execute()->fetchField();
	
	if($notifyby !='manager' && !empty($notifyby)){
		$call_type = explode($notifyby,'-');
		$fromAssesment = $call_type[0]; 
	 	if($fromAssesment != $notifyby)
			$prevStatus = $fromAssesment;
	}
	expDebug::dPrint("inside mark program complete22222 >>>> prev ". $prevStatus ."<<< ".$aftrStatus,4);
	if($prevStatus != $aftrStatus){
		if($aftrStatus == 'lrn_tpm_ovr_cmp'){
		 if($notifyby == 'manager'){
		 	 insertNotification('CertCurrLPCompleteByManager', $master_enrollment_id, $program_id, $user_id, $upduser_id);
		 }else{
		 	 insertNotification('CertCurrLPComplete', $master_enrollment_id, $program_id, $user_id, $upduser_id);
		 }
		}
		 //Skill Update for TP complete
		$objecttype = db_select('slt_program', 'prg');
		$objecttype->addField('prg','object_type','objecttype');
		$objecttype->addField('prg','title','title');
		$objecttype->condition('id', $program_id,'=');
		$objResult = $objecttype->execute()->fetchAll();
		expDebug::dPrintDBAPI('objecttype query :', $objecttype);
	  	expDebug::dPrint(' $object result>>>  '.print_r($objResult,true),5);
		if($objResult[0]->objecttype == 'cre_sys_obt_crt') {  
	      updateSkillForTpCompletion($program_id,$user_id,$master_enrollment_id,$objResult[0]->title,$upduser_id);
		}
		//get new overall status
		 $overallcomplete = db_select('slt_master_enrollment', 'enroll');
         $overallcomplete->addField('enroll', 'overall_status');
         $overallcomplete->addField('enroll', 'recertify_path');
         $overallcomplete->condition('id', $master_enrollment_id,'=');
         $overallcompleted = $overallcomplete->execute()->fetchAll();
         $overallstaus_new = $overallcompleted[0]->overall_status;
		
         //Update custom4 with completion status
         $updateMasterFields['custom4'] = $overallstaus_new;
         $updateMasterFields['overall_status'] = $overallstaus_new;
         /*if($certify_flag > 1 && ($overallstaus_new == 'lrn_tpm_ovr_inc' || $overallstaus_new == 'lrn_tpm_ovr_cln')){
         	  $updateMasterFields['is_current'] = 'N';
         }*/
	          $updateMasterEnroll = db_update('slt_master_enrollment');
		      $updateMasterEnroll->fields($updateMasterFields);
		      $updateMasterEnroll->condition('id', $master_enrollment_id,'=');
		      $updateMasterEnroll->condition('program_id', $program_id,'=');
		 //     $updateMasterEnroll->execute();   
		      if(!empty($statements)) {
		      	$statements[] = $updateMasterEnroll;
		      } else {
		      	$updateMasterEnroll->execute();
		      }
		      expDebug::dPrintDBAPI('previous enrollment update query :',$updateMasterEnroll);
		/*if($certify_flag > 1 && ($overallstaus_new == 'lrn_tpm_ovr_inc' || $overallstaus_new == 'lrn_tpm_ovr_cln')){    
		    $selectStmt = db_select('slt_master_enrollment', 'ms');
			$selectStmt->addField('ms','id','id');
			$selectStmt->addField('ms','custom4','custom4');
			$selectStmt->addField('ms','comp_date','comp_date');
			$selectStmt->condition('ms.program_id',$program_id,'=');
			$selectStmt->condition('ms.user_id',$user_id,'=');
			$selectStmt->condition('ms.recertify_path',($certify_flag-1),'=');
			$selectStmt->orderBy('ms.id', 'desc');
			$selectStmt->range(0, 1);
			expDebug::dPrintDBAPI('previous enrollment status update query :',$selectStmt);
			$prev_menr = $selectStmt->execute()->fetchAll();
			expDebug::dPrint("prev enrollment  >> ".print_r($prev_menr,true),5);
			$prev_menrid = $prev_menr[0]->id;
			$prev_custom4 = $prev_menr[0]->custom4;
			$prev_comp = $prev_menr[0]->comp_date;
	       	expDebug::dPrint("master enrollmetn id prev ". $prev_menrid ,5);
		       
	       	$cert_days_expire = isset($config["cert_expire_reminder_days"]) ? $config["cert_expire_reminder_days"] : 10;
	       	//get certificate expired or not
	       	$select = db_select('slt_program', 'p');
	       	$select->addExpression('(CASE WHEN(DATEDIFF(
	               IF (p.expires_in_unit = \'days\', DATE_ADD(\''.$prev_comp.'\', INTERVAL p.expires_in_value DAY),
	                     IF (p.expires_in_unit = \'months\', DATE_ADD(\''.$prev_comp.'\', INTERVAL p.expires_in_value MONTH),
	                           DATE_ADD(\''.$prev_comp.'\', INTERVAL p.expires_in_value YEAR))),
	               CURDATE()) < 0)THEN \'lrn_tpm_ovr_exp\'
	               ELSE \'lrn_tpm_ovr_cmp\' END )','prev_status');
				
	 	 	$select->condition('p.id',$program_id,'=');
	 	 	expDebug::dPrintDBAPI('certificate expiry or not :',$select);
	       	$prev_sts = $select->execute()->fetchField();	
	       
	        $updateMasterFields1['overall_status'] = $prev_sts;
	        $updateMasterFields1['is_current'] = 'Y';
	        $updateMasterEnroll1 = db_update('slt_master_enrollment');
	        $updateMasterEnroll1->fields($updateMasterFields1);
	        $updateMasterEnroll1->condition('id', $prev_menrid,'=');
	        $updateMasterEnroll1->condition('program_id', $program_id,'=');
	        expDebug::dPrintDBAPI('master enrollment update query 222 :',$updateMasterEnroll1);
	        $updateMasterEnroll1->execute();  
		 }*/
	    //Update enrollment table when the overall status of TP marked as incomplete
	    if($overallstaus_new == 'lrn_tpm_ovr_inc'){
	    	  $updateMasterEnroll = db_update('slt_enrollment');
		      $updateMasterEnroll->fields(array('comp_status'=>'lrn_crs_cmp_inc'));
		      $updateMasterEnroll->condition('master_enrollment_id', $master_enrollment_id,'=');
		      $updateMasterEnroll->condition('comp_status', array('lrn_crs_cmp_cmp','lrn_crs_cmp_nsw'),'NOT IN');
		   //   $updateMasterEnroll->execute();   
		      if(!empty($statements)) {
		      	$statements[] = $updateMasterEnroll;
		      } else {
		      	$updateMasterEnroll->execute();
		      }
		      expDebug::dPrintDBAPI('updateMasterEnroll query :',$updateMasterEnroll);
	    }
	    $operationstatus = ($aftrStatus == 'lrn_tpm_ovr_cmp') ? "tpcompleted" : "tpincompleted";
	    
	    //Audit trail entry
	    make_audit_trail_entries($updateMasterField['overall_status'], getIdOfLoggedInUser() , $program_id, $prevStatus , 'updated master_enrollment status',$objResult[0]->objecttype,'exp_sp_lnrlearningplan',$operationstatus,
                                                array('overall_status'));  
	    
	}
	
	
    	
  }catch(Exception $e){
		expDebug::dPrint('Error in getclassContentStatus '.$e,1);
		expertusLogError($e);
	}
}
/*
 * PHP function for sql procedure slp_attendance_summary_ins
*/
function lp_attendance_summary_ins($regid,$uid,$course_id,$class_id,$lesson_id,$version_id,
		$surveyid,$status,$pstatus,$score,$lessonloc,$type,$content_status,$completion_status,
			$launch_data,$suspend_data,$cmiexit,$aicc_data,$updateuser,$calltype,$assesscheck,$tottime,$launchflag,$content_type,$progress,$subtype='',&$statements=array()){
	try{
		expDebug::dPrint('func get args --->summ_ins'.var_export(func_get_args(), 1),5);
		//$assesscheck = 0 or null for post assessment and content; $assesscheck = 1 for preassessment
		
		$preassessment  = $assesscheck == '1' ? true : false;
		$postassessment = $assesscheck == '0' ? true : false;

		expDebug::dPrint('$asscount for pre assessment'.var_export($preassessment, 1));
		expDebug::dPrint('$asscount for post assessment'.var_export($postassessment, 1));
		$launchflag = (int) $launchflag;
		$totatempts = 0;
		if($type == 'Online' || $type == 'online') {

		}else{
			$totatempts = null;
			$tottime = null;
		}
		expDebug::dPrint("content status >>> ". $content_status);
		$content_status = strtolower($content_status);

		if(($content_status == 'completed' || $content_status == 'passed' || $content_status == 'failed' || $content_status == 'incomplete' || $content_status == 'inprogress')
				|| ($calltype == "'SCORM_2004'" && ($completion_status == 'incomplete' || ($content_status == 'unknown' && $completion_status != 'incomplete') ))){
			$totatempts=1;
		}

		if($launchflag == 0){
			$custom2 = null;
			$custom3 = null;
			$custom4 = null;
		}else{
			$custom2 = $totatempts;
			$custom3 = $tottime;
			$custom4 = 'is_current';
		}


		//Insert slt_attendance_summary
		$insertStmt = db_insert('slt_attendance_summary');
		$fields = array(
				'enrollment_id'    => $regid,
				'user_id'  => $uid,
				'course_id'  => $course_id,
				'class_id'  => $class_id,
				'content_version_id'  => $version_id,
				'lesson_id'  => $lesson_id,
				'assessment_id'  => $surveyid,
		 		'content_type' => $content_type,
		 		'progress' => $progress,
				'status' => $status,
				'total_attempts' => $totatempts,
				'last_attempt_date' => now(),
				'total_time_spend' => $tottime,
				'last_lesson_location' =>$lessonloc,
				'score' => $score,
				'launch_data' => $launch_data,
				'suspend_data' => $suspend_data,
				'cmiexit' => $cmiexit,
				'content_status' => $content_status,
				'completion_status' => $completion_status,
				'aicc_data' => $aicc_data,
				'created_by' => $updateuser,
				'created_on' => now(),
				'updated_by' => $updateuser,
				'updated_on' => now(),
				'custom2' => $custom2,
				'custom3' => $custom3,
				'custom4' => $custom4
		);
		if($content_type ==  "sry_det_typ_ass" || $content_type == 'sry_det_typ_sry'){
			unset($fields['total_attempts']);
		}
		$insertStmt->fields($fields);
		expDebug::dPrintDBAPI("Insert slt_attendance_summary ", $insertStmt);
		$attend_Id = $insertStmt->execute();
		//Skipped this for related transactions in this function
		/* if(!empty($statements)) {
			$statements[] = $insertStmt;
		} else {
			$attend_Id = $insertStmt->execute();
		} */
		$result= new stdClass();
		$result->col_name = 'id';
		$result ->col_value = $attend_Id;
		$result ->table_name ='slt_attendance_summary';
		$statements['insert'][] = $result;

		expDebug::dPrint("attendance summary id >>> ".$attend_Id);

		//get previous details from slt_enrollment
		$selectStmt = db_select('slt_enrollment', 'enr');
		$selectStmt->addField('enr','content_status','content_status');
		$selectStmt->addField('enr','comp_status','comp_status');
		$selectStmt->addField('enr','score','score');
		$selectStmt->addField('enr','master_enrollment_id','master_enrollment_id');
		$selectStmt->addField('enr','pre_status','pre_status');
		$selectStmt->addField('enr','pre_score','pre_score');
		$selectStmt->addField('enr','progress','prev_progress');
		$selectStmt->addField('enr','comp_date','comp_date');
		$selectStmt->addField('enr','comp_by','comp_by');
		$selectStmt->addField('enr','updated_by','updated_by');
		$selectStmt->addField('enr','updated_on','updated_on');
		$selectStmt->addField('enr','comp_date','comp_date');
		$selectStmt->addField('enr','comp_by','comp_by');
		$selectStmt->addField('enr','comp_on','comp_on');
		$selectStmt->condition('enr.id',$regid,'=');
		$enroll_details = $selectStmt->execute()->fetchAll();
		expDebug::dPrintDBAPI('get previous details from slt_enrollments query>>', $selectStmt);
		
		$prev_content_status = $enroll_details[0]->content_status;
		expDebug::dPrintDBAPI('get previous content status', $selectStmt);
		expDebug::dPrint('previous content status>> '.print_r($prev_content_status,true), 4);
		$prev_progress_enroll = $enroll_details[0]->prev_progress;
		$compstbef = $enroll_details[0]->comp_status;
		$prev_score = $enroll_details [0]->score;			//previous post asessment score or content score
		$prev_pre_score = $enroll_details [0]->pre_score;	//previous pre asessment score
		$prev_comp_date = $enroll_details [0]->comp_date;

		expDebug::dPrint('get previous details from slt_enrollments>> '.print_r($enroll_details,true), 5);


		if(strtolower($prev_content_status) == 'passed' || strtolower($prev_content_status) == 'completed')
			$enrollment_content_status =  $prev_content_status;
		else{
			$enrollment_content_status =  getclassContentStatus($class_id,$regid); // - check internal fun
		}
		$selectStmt = db_select('slt_enrollment', 'enr');
		$selectStmt->addField('enr','comp_status','comp_status');
		$selectStmt->condition('enr.id',$regid,'=');
		$compstbef = $selectStmt->execute()->fetchField();

		//Check whether post-assessment available for the class
			$selectStmt = db_select('slt_survey_mapping', 'assmap');
			$selectStmt->innerJoin('slt_survey', 'srv','srv.id = assmap.survey_id AND srv.type = \'sry_det_typ_ass\'');
			$selectStmt->addExpression('count(1)','asscount');
			$selectStmt->condition('assmap.object_type','cre_sys_obt_cls','=');
			$selectStmt->condition('assmap.object_id',$class_id,'=');
			$selectStmt->condition('assmap.pre_status',0,'=');
			expDebug::dPrintDBAPI('get previous content status',$selectStmt);
			$asscount = $selectStmt->execute()->fetchField();
		expDebug::dPrint('Check whether post-assessment available for the class '.print_r($asscount,true), 4);
				
		//get version ids of all the contents associated with the class based on enrollment
		$selectType = db_select('slt_course_class', 'cls');
		$selectType->addField('cls', 'delivery_type');
		$selectType->condition('cls.id', $class_id,'=');
		$delType = $selectType->execute()->fetchField();
		
		if ($delType == 'lrn_cls_dty_wbt' || $delType == 'lrn_cls_dty_vod') {
		$versions_arr=array();
		$versions_array = getcontentVersions($class_id,$regid,$uid);
		foreach ($versions_array as $version_record){
			if(!in_array($version_record->vid, $versions_arr)){
				$versions_arr[]=$version_record->vid;
			}
		}
		
		//get score of all the contents and their lessons from attendance summary
			$selectStmt = db_select('slt_attendance_summary', 'summ');
			$selectStmt->innerJoin('slt_enrollment_content_mapping', 'emap','emap.enroll_id = summ.enrollment_id	AND summ.content_version_id = emap.version_id');
			$selectStmt->addExpression('sum(summ.score)','tcontentscore');
			$selectStmt->condition('summ.user_id',$uid,'=');
			$selectStmt->condition('summ.course_id',$course_id,'=');
			$selectStmt->condition('summ.class_id',$class_id,'=');
			$selectStmt->condition('summ.enrollment_id',$regid,'=');			
		$selectStmt->condition('summ.content_version_id',$versions_arr,'IN');
		expDebug::dPrintDBAPI('get score from summary content query>> ',$selectStmt);
		$tcontentscore = $selectStmt->execute()->fetchField();		//sum of all the contents and their lessons
		expDebug::dPrint('get score from summary content '. $tcontentscore, 4);
		}			
		//get score from summary for post-assessment // Commented because $tassessmentscore is same to the input parameter value
	/*	$selectStmt = db_select('slt_attendance_summary', 'summ');
		$selectStmt->leftjoin('slt_enrollment_content_mapping', 'emap','emap.enroll_id = summ.enrollment_id AND summ.class_id = emap.class_id');
		$selectStmt->innerJoin('slt_survey', 'srv','srv.id = summ.assessment_id AND srv.type = \'sry_det_typ_ass\'');
		$selectStmt->innerJoin('slt_survey_mapping', 'assmap','srv.id = assmap.survey_id');
		$selectStmt->addField('summ','score','tassessmentscore');
		$selectStmt->condition('summ.user_id',$uid,'=');
		$selectStmt->condition('summ.course_id',$course_id,'=');
		$selectStmt->condition('summ.class_id',$class_id,'=');
		$selectStmt->condition('summ.enrollment_id',$regid,'=');
		$selectStmt->condition('summ.assessment_id','0','!=');
		$selectStmt->condition('summ.content_version_id','0','=');
		$selectStmt->condition('assmap.pre_status',0,'=');
		expDebug::dPrintDBAPI('get score from summary assessment query>>',$selectStmt);
		$tassessmentscore = $selectStmt->execute()->fetchField();		//score of post assessment if any
		expDebug::dPrint('get score from summary  assessment'. $tassessmentscore, 4);*/ 
		
		$tassessmentscore = $score;	//this should be commented if above code is uncommented
		$tscore = $prev_score;
		$prescore=$prev_pre_score;
		
		if ($asscount > 0) { // $asscount for availability of post assessment
			if ($postassessment) { // $assesscheck = 0 for post assessment
				$tscore = max($tassessmentscore, $prev_score);
			}else if ($preassessment) { // $assesscheck = 1 for pre assessment
				$prescore = max($tassessmentscore,$prev_pre_score);
		}
		} else {
			if ($preassessment) { // $assesscheck = 1 for pre assessment
				$prescore = max($tassessmentscore,$prev_pre_score);
			} else {
				$tscore = max($tcontentscore,$prev_score);
			}
		}
		
		expDebug::dPrint('Check prescore-->joola'.$prescore);
		expDebug::dPrint('Check score-->joola'.$tscore);
		if($enroll_details[0]->comp_status=='lrn_crs_cmp_cmp' && $calltype != "'assesmentTP'"){
			$contentstatus = $enroll_details[0]->comp_status;
		}
		else{
		if($calltype == "'assessment'" || $calltype == "'assesmentTP'"){
			$contentstatus =  $status;
		}else{
			$contentstatus = getcontentCount($class_id, $uid, $regid,$subtype);
			if($asscount >0){
				if($contentstatus == 'lrn_crs_cmp_cmp' || $contentstatus == 'lrn_crs_cmp_inc')
					$contentstatus = $enroll_details[0]->comp_status;
			}
		}
		}
		
		//Audit trail entry
		$master_enroll_id = $enroll_details[0]->master_enrollment_id;

		//get master enrollment status
		$selectStmt = db_select('slt_master_enrollment', 'enr');
		$selectStmt->addField('enr','overall_status','overall_status');
		$selectStmt->condition('enr.id',$master_enroll_id,'=');
		expDebug::dPrintDBAPI('get master enrollment status  ',$selectStmt);
		$overallstaus_old = $selectStmt->execute()->fetchField();

		if($contentstatus == 'lrn_crs_cmp_inc' ||  $contentstatus == 'lrn_crs_cmp_cmp')
			$comp_date =  now();
		else
			$comp_date =  NULL;

		if($calltype != "'assesmentTP'"){
			if($preassessment){
				$assessstatus = $pstatus;
			}else{
				$assessstatus = $enroll_details[0]->pre_status;
			}
			//Update status for incomplete and inprogress
			if($contentstatus != 'lrn_crs_cmp_cmp'){
				//update slt_enrollment
				$updateStmt = db_update('slt_enrollment');
				$updateStmt->fields(array(
						'comp_status' => $contentstatus,
						'pre_status' => $assessstatus,
						'comp_date' => $comp_date,
						'score' => $tscore,
						'content_status' => $enrollment_content_status,
						'pre_score' => $prescore,
						'updated_by' => $updateuser,
						'updated_on' => now ()
				));
				$updateStmt->condition('user_id',$uid,'=');
				$updateStmt->condition('course_id', $course_id,'=');
				$updateStmt->condition('class_id', $class_id,'=');
				$updateStmt->condition('id', $regid,'=');
				$updateStmt->condition('comp_status', array('lrn_crs_cmp_inc'),'NOT IN');
				$updateStmt->condition('reg_status', 'lrn_crs_reg_cnf' ,'=');
				expDebug::dPrintDBAPI(' update slt_enrollment1 = ' , $updateStmt);
				$enroll_upd = $updateStmt->execute();
				
				$result= new stdClass();
				$result->col_name = 'id';
				$result ->col_value = $regid;
				$result ->table_name ='slt_enrollment';
				$result ->fields =array(
						'comp_status' => $enroll_details[0]->comp_status,
						'pre_status' => $enroll_details[0]->pre_status,
						'comp_date' => $enroll_details[0]->comp_date,
						'score' => $enroll_details[0]->score,
						'content_status' => $enroll_details[0]->content_status,
						'pre_score' => $enroll_details[0]->pre_score,
						'updated_by' => $enroll_details[0]->updated_by,
						'updated_on' => $enroll_details[0]->updated_on
				);
				$statements['update'][] = $result;
				/* if(!empty($statements)) {
					$statements[] = $updateStmt;
				} else {
					$enroll_upd = $updateStmt->execute();
				}	 */	 
				if($contentstatus == 'lrn_crs_cmp_inc'){
					//update slt_survey_results
					$selectStmt = db_select('slt_survey_mapping', 'surmap');
					$selectStmt->innerJoin('slt_survey','sur','sur.id = surmap.survey_id');
					$selectStmt->addExpression('GROUP_CONCAT(distinct surmap.survey_id)','surveyids');
					$selectStmt->condition('surmap.object_id',$class_id,'=');
					$selectStmt->condition('surmap.object_type','cre_sys_obt_cls','=');
					$selectStmt->condition('sur.type',array('sry_det_typ_ass','sry_det_typ_sry'),'IN');
					expDebug::dPrintDBAPI("update slt_survey_results ",$selectStmt);
					$results = $selectStmt->execute()->fetchField();
					$survey_ids = explode(',',$results);

					//update slt_survey results
					$updateStmt = db_update('slt_survey_results');
					$updateStmt->fields(array('re_attend' => 1));
					$updateStmt->condition('entity_id',$class_id,'=');
					$updateStmt->condition('user_id', $uid,'=');
					$updateStmt->condition('survey_id', $survey_ids, 'IN');
					expDebug::dPrintDBAPI(' update slt_survey_results = ' , $updateStmt);
					//$survey_results = $updateStmt->execute();
					if(!empty($statements)) {
						$statements[] = $updateStmt;
					} else {
						$survey_results = $updateStmt->execute();
					}
				}
			}else{
				$compuserid = $enroll_details[0]->comp_by;

				//Update slt_enrollment
				$updateStmt = db_update('slt_enrollment');
				$updateFields = array(
						'comp_status' => $contentstatus,
						'pre_status' => $assessstatus,
						//'comp_date' => $comp_date,
						'score' => $tscore,
						'pre_score' => $prescore,
						'content_status' => $enrollment_content_status,
						'updated_by' => $updateuser,
						'updated_on' => now()
				);

				//master enroll ids
				$selectStmt = db_select('slt_enrollment', 'e');
				$selectStmt->addExpression('GROUP_CONCAT(e.id)','masterids');
				$selectStmt->condition('e.user_id',$uid,'=');
				$selectStmt->condition('e.class_id',$class_id,'=');
				$selectStmt->condition('e.master_enrollment_id','', 'IS NOT NULL');
				expDebug::dPrintDBAPI("update slt_enrollment for master enrollments ",$selectStmt);
				$results = $selectStmt->execute()->fetchField();
				$master_ids = explode(',',$results);
				
				if($compstbef != 'lrn_crs_cmp_cmp' && ($prev_comp_date == NULL || $prev_comp_date =='')){
					$updateFields['comp_date'] = $comp_date;
				}

				if(empty($compuserid) && empty($master_enroll_id)){
					$updateFields['comp_by'] = $updateuser;
					$updateFields['comp_on'] = now();
					$updateStmt->condition(db_or()
							->condition('id', $regid,'=')
							->condition('id', $master_ids , 'IN'));
					$updateStmt->condition('comp_status', array('lrn_crs_cmp_inc','lrn_crs_cmp_nsw','lrn_crs_cmp_cmp'),'NOT IN');
				}else{
					$updateStmt->condition('comp_status', array('lrn_crs_cmp_inc','lrn_crs_cmp_nsw'),'NOT IN');
				}
				$updateStmt->fields($updateFields);
				$updateStmt->condition('user_id',$uid,'=');
				$updateStmt->condition('course_id', $course_id,'=');
				$updateStmt->condition('class_id', $class_id,'=');
				//$updateStmt->condition('id', $regid);
				$updateStmt->condition('reg_status', 'lrn_crs_reg_cnf' ,'=');
				expDebug::dPrintDBAPI(' update slt_enrollment inside attendane summary = ' , $updateStmt);
				$enroll_upd = $updateStmt->execute();

			/* 	if(!empty($statements)) {
					$statements[] = $updateStmt;
				} else {
					$enroll_upd = $updateStmt->execute();
				} */
				$result= new stdClass();
				$result->col_name = 'id';
				$result ->col_value = $regid;
				$result ->table_name ='slt_enrollment';
				$result ->fields =array(
						'comp_status' => $enroll_details[0]->comp_status,
						'pre_status' => $enroll_details[0]->pre_status,
						//'comp_date' => $enroll_details[0]->comp_date,
						'score' => $enroll_details[0]->score,
						'content_status' => $enroll_details[0]->content_status,
						'pre_score' => $enroll_details[0]->pre_score,
						'updated_by' => $enroll_details[0]->updated_by,
						'updated_on' => $enroll_details[0]->updated_on,
						'comp_date' => $enroll_details[0]->comp_date,
						'comp_by' => $enroll_details[0]->comp_by,
						'comp_on' => $enroll_details[0]->comp_on,
				);
				$statements['update'][] = $result;
				
				/* if(isset($_SESSION['menrid'][$master_enroll_id]['next_launchable_sequence'])){
					unset($_SESSION['menrid'][$master_enroll_id]['next_launchable_sequence']);
				}  */
			}
			
					//progress update for enrollment.
					$sid = $uid."-".$course_id."-".$class_id;
					$ClassDelType = getClassDeliveryType($class_id, 'Short');
					expDebug::dPrint(' Class Type = ' . $ClassDelType , 4);
					include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_learning/exp_sp_learning_learner/modules/exp_sp_lnrenrollment/exp_sp_launch.inc');
					$launch_detail = ($ClassDelType=='wbt' || $ClassDelType=='vc' || $ClassDelType=='vod')?getLaunchDetail($regid,'',$sid):getLaunchDetail($regid);
					$progress = get_progress($launch_detail,$regid,1);
					if(($calltype == "'assessment'" || $calltype == "'assesmentTP'") && $contentstatus == "lrn_crs_cmp_cmp" && $content_type ==  "sry_det_typ_ass" ){
						$progress = 100;
					}
					$update_progress  = ($progress>$prev_progress_enroll) ? $progress : $prev_progress_enroll;

					$selectStmt = db_select('slt_enrollment', 'e');
					$selectStmt->addExpression('GROUP_CONCAT(e.id)','masterids');
					$selectStmt->condition('e.user_id',$uid,'=');
					$selectStmt->condition('e.course_id',$course_id,'=');
					$selectStmt->condition('e.class_id',$class_id,'=');
					$selectStmt->condition('e.master_enrollment_id','', 'IS NOT NULL');
					expDebug::dPrintDBAPI("update slt_enrollment for master enrollments ",$selectStmt);
					$results = $selectStmt->execute()->fetchField();
					$master_ids = explode(',',$results);
					
					$updateQuery = db_update('slt_enrollment');
					$updateQuery->fields(array('progress'=>$update_progress));
					if($update_progress == 100){ // progress rollup for enrollments
						if(empty($master_enroll_id)){
							$updateQuery->condition('user_id',$uid,'=');
							$updateQuery->condition('course_id', $course_id,'=');
							$updateQuery->condition('class_id', $class_id,'=');
						}else{
							$updateQuery->condition(db_or()
									->condition('id', $regid,'=')
									->condition('id', $master_ids , 'IN'));
						}
							$updateQuery->condition('comp_status', array('lrn_crs_cmp_inc','lrn_crs_cmp_nsw'),'NOT IN');
							$updateQuery->condition('reg_status', 'lrn_crs_reg_cnf' ,'=');
					}else{
					$updateQuery->condition('id',$regid,'=');
					}
					
					expDebug::dPrintDBAPI(' UpdateQuery = ' , $updateQuery,4);
					$rosUpdated = $updateQuery->execute();
					/* if(!empty($statements)) {
						$statements[] = $updateQuery;
					} else {
						$rosUpdated = $updateQuery->execute();
					} */
					
					$result= new stdClass();
					$result->col_name = 'id';
					$result ->col_value = $regid;
					$result ->table_name ='slt_enrollment';
					$result ->fields =array(
							'progress'=>$prev_progress_enroll
					);
					$statements['update'][] = $result;
					// Update launchable in slt_enrollment table
					
					
					$totalattemptsleft = CalculateTotalAttemptsLeft($class_id,$regid);
					expDebug::dPrint("totalattemptslefttotalattemptsleft : ".print_r($totalattemptsleft,true),5);
					if(($totalattemptsleft) <= 0){
						//$updateattempts = db_query("UPDATE slt_enrollment SET launchable=0 WHERE id=".$regid);
						$updateStmt = db_update('slt_enrollment');
						$updateStmt->fields(array('launchable' => 0));
						$updateStmt->condition('id',$regid,'=');
						//$updateattempts = $updateStmt->execute();
						if(!empty($statements)) {
							$statements[] = $updateStmt;
						} else {
							$updateattempts = $updateStmt->execute();
						}
					}
					
					// Update TP Overall Progress
					if (!empty($master_enroll_id)) {
						$program_id = getProgramIdFromEnrollment($master_enroll_id);
						// To update  a overall percentage complete used in content player - new column
						updateTPEnrollmentOverallPercentComplete($program_id, $master_enroll_id,'','',$statements);
					}
					
					
					
			
		}else{ // else for ($calltype != 'assesmentTP')
			//Update slt_enrollment
			$updateStmt = db_update('slt_enrollment');
			$updateStmt->fields(array(
					'comp_status' => $contentstatus,
					'pre_status' => $assessstatus,
					'comp_date' => $comp_date,
					'updated_by' => $updateuser,
					'content_status' => $enrollment_content_status,
					'updated_on' => now()
			));
			$updateStmt->condition('master_enrollment_id',$master_enroll_id,'=');
			$updateStmt->condition('comp_status', array('lrn_crs_cmp_cmp','lrn_crs_cmp_inc','lrn_crs_cmp_nsw'),'NOT IN');
			$updateStmt->condition('reg_status', 'lrn_crs_reg_cnf' ,'=');
			expDebug::dPrintDBAPI(' update slt_enrollment = ' , $updateStmt);
			//$enroll_upd = $updateStmt->execute();
			if(!empty($statements)) {
				$statements[] = $updateStmt;
			} else {
				$enroll_upd = $updateStmt->execute();
			}
		}

		// update slt_attendance summary
		$updateStmt = db_update('slt_attendance_summary');
		$updateStmt->fields(array('custom4' => NULL));
		$updateStmt->condition('enrollment_id',$regid,'=');
		expDebug::dPrintDBAPI(' update slt_attendance summary= ' , $updateStmt);
		//$summ = $updateStmt->execute();
		if(!empty($statements)) {
			$statements[] = $updateStmt;
		} else {
			$summ = $updateStmt->execute();
		}

		//get program id from slt_master_enrollment
		$selectStmt = db_select('slt_master_enrollment', 'menr');
		$selectStmt->addField('menr','program_id','program_id');
		$selectStmt->addField('menr','score','tpprevscore');
		$selectStmt->addField('menr','pre_score','tpprescore');
		$selectStmt->condition('menr.id',$master_enroll_id,'=');
		expDebug::dPrintDBAPI('get program id from slt_master_enrollment',$selectStmt);
		$programdetails = $selectStmt->execute()->fetchAll();
		expDebug::dPrint('programdetails id >>> ' .print_r($programdetails,true), 4);

		$prg_id =$programdetails[0]->program_id;
		$tpprev_score = $programdetails[0]->score;			//previous post asessment score or content score
		$tpprev_pre_score = $programdetails[0]->pre_score;	//previous pre asessment score
		
		expDebug::dPrint('master_enroll_idprogram id >>> ' .print_r($master_enroll_id,true), 4);
		
		//Get score from enrollment for TP
		$selectStmt = db_select('slt_enrollment', 'enr');
		$selectStmt->addExpression('sum(enr.score)','tpoverallscore');
		$selectStmt->condition('enr.user_id',$uid,'=');
		$selectStmt->condition('enr.master_enrollment_id',$master_enroll_id,'=');
		expDebug::dPrintDBAPI('get overallscore',$selectStmt);
		$tpoverallscore = $selectStmt->execute()->fetchField();
		expDebug::dPrint('Check overallscore for TP '.print_r($tpoverallscore,true), 4);

		//Check whether post assessment available for the TP
		$selectStmt = db_select('slt_survey_mapping', 'assmap');
		$selectStmt->innerJoin('slt_survey', 'srv','srv.id = assmap.survey_id AND srv.type = \'sry_det_typ_ass\'');
		$selectStmt->addExpression('count(1)','tpasscount');
		$selectStmt->condition('assmap.object_type','cre_sys_obt_cls','!=');
		$selectStmt->condition('assmap.object_id',$prg_id,'=');
		$selectStmt->condition('assmap.pre_status',0,'=');
		expDebug::dPrintDBAPI('get assessment available for TP',$selectStmt);
		$tpasscount = $selectStmt->execute()->fetchField();
		expDebug::dPrint('Check whether assessment available for TP '.print_r($tpasscount,true), 4);		
		
		//Get score from summary for assessment // Commented because $tassessmentscore is same to the input parameter value
	/*	$selectStmt = db_select('slt_attendance_summary', 'summ');
		$selectStmt->leftjoin('slt_enrollment', 'enr','enr.id = summ.enrollment_id AND enr.user_id = summ.user_id');
		$selectStmt->leftjoin('slt_survey', 'srv','srv.id = summ.assessment_id AND srv.type = \'sry_det_typ_ass\'');
		$selectStmt->leftjoin('slt_survey_mapping', 'assmap','srv.id = assmap.survey_id');
		$selectStmt->addField('summ','score','tpassessmentscore');
		$selectStmt->condition('summ.user_id',$uid,'=');
		$selectStmt->condition('summ.enrollment_id',$regid,'=');
		$selectStmt->condition('summ.assessment_id','0','!=');
		$selectStmt->condition('summ.content_version_id','0','=');
		$selectStmt->condition('assmap.pre_status',0,'=');
		$selectStmt->condition('assmap.object_id',$prg_id,'=');
		$selectStmt->condition('assmap.object_type','cre_sys_obt_cls','!=');
		$selectStmt->condition('enr.master_enrollment_id','', 'IS NOT NULL');
		expDebug::dPrintDBAPI('get score from summary assessment',$selectStmt);
		$tpassessmentscore =  $selectStmt->execute()->fetchField();
		expDebug::dPrint('get score from summary for TP post assessment'.$tpassessmentscore, 4);*/
		
		$tpassessmentscore=$score; //this should be commented if above code is uncommented
		$tpscore = $tpprev_score;
		$tpprescore=$tpprev_pre_score;
		
		if ($tpasscount > 0) {
			if ($calltype == "'assesmentTP'" && $postassessment) {
				$tpscore =max($tpassessmentscore,$tpprev_score);
			} else if ($calltype == "'assesmentTP'" && $preassessment) { // $assesscheck = 1 for pre assessment
				//$tpscore =max($tpassessmentscore,$tpprev_score);
				$tpprescore =max($tpassessmentscore,$tpprev_pre_score);
			}
		} else {
			if ($calltype == "'assesmentTP'" && $preassessment) { // $assesscheck = 1 for pre assessment
				$tpprescore =max($tpassessmentscore,$tpprev_pre_score);
			} else {
				$tpscore =max($tpoverallscore,$tpprev_score);
			}
		}
		
		//Roll up TP
		if($contentstatus != 'lrn_crs_cmp_inp' && $contentstatus != 'lrn_crs_cmp_enr'){
			$master_enroll_content_status = getTpContentStatus($master_enroll_id);
				
			if($calltype != "'assesmentTP'"){
				$selectStmt = db_select('slt_enrollment', 'enr');
				$selectStmt->addField('enr','master_enrollment_id','master_enrollment_id');
				$selectStmt->condition('enr.user_id',$uid,'=');
				$selectStmt->condition('enr.course_id',$course_id,'=');
				$selectStmt->condition('enr.reg_status','lrn_crs_reg_cnf','=');
				$selectStmt->condition('enr.master_enrollment_id','', 'IS NOT NULL');
				expDebug::dPrintDBAPI("select list of master enrollment ",$selectStmt);
				$menr_ids = $selectStmt->execute()->fetchCol();

				foreach($menr_ids as $mid){
					$program_id = getProgramIdFromEnrollment($mid);
					lp_mark_program_complete($uid, $mid, $program_id, $updateuser, '', '','',$tpscore,$statements);
				}

			}else {

				$tpstatus = ($contentstatus == 'lrn_crs_cmp_cmp') ? 'lrn_tpm_ovr_cmp' : 'lrn_tpm_ovr_inc';
				 
				$selectStmt = db_select('slt_master_enrollment', 'menr');
				//$selectStmt->addExpression('CONCAT(overall_status,'-','.$calltype.')','call_type');
				$selectStmt->addField('menr','overall_status','overall_status');
				$selectStmt->addField('menr','overall_progress','overall_progress');
				$selectStmt->addField('menr','comp_date','comp_date');
				$selectStmt->addField('menr','score','score');
				$selectStmt->addField('menr','content_status','content_status');
				$selectStmt->addField('menr','updated_on','updated_on');
				$selectStmt->addField('menr','updated_by','updated_by');
				$selectStmt->addExpression('IF(overall_status = \'lrn_tpm_ovr_cmp\', comp_date, NOW())','tpcomp_date');
				$selectStmt->condition('menr.id',$master_enroll_id,'=');
				expDebug::dPrintDBAPI('get previous content status',$selectStmt);
				$ovr_sts = $selectStmt->execute()->fetchAll();
				$updprogress =  $ovr_sts[0]->overall_progress;
				if($tpstatus == 'lrn_tpm_ovr_cmp' && $calltype == "'assesmentTP'"  && $content_type ==  'sry_det_typ_ass'){
					$updprogress = 100;
				}
				$calltypeforassess = $calltype;
				$calltype = $ovr_sts[0]->overall_status.'-'.$calltype;
				$tpcomp_date = $ovr_sts[0]->tpcomp_date;
				expDebug::dPrint('get call type value' .print_r($calltype,true), 4);
				
				$tpstatus = ($ovr_sts[0]->overall_status == 'lrn_tpm_ovr_cmp') ? $ovr_sts[0]->overall_status:$tpstatus;
				$updateStmt = db_update('slt_master_enrollment');
				// Set conditions
				$updateStmt->condition('id', $master_enroll_id,'=');
				$updateStmt->condition('overall_status', array('lrn_tpm_ovr_enr','lrn_tpm_ovr_inp','lrn_tpm_ovr_cmp'),'IN');
				// Set new values of fields
				$updateStmt->fields(array(
						'overall_status' => $tpstatus,
						'comp_date' => $tpcomp_date,
						'score' => $tpscore,
			    		'overall_progress' => $updprogress,
						'content_status' =>  $master_enroll_content_status,
						'updated_on' => now(),
						'updated_by' => $updateuser
				));
				expDebug::dPrintDBAPI(' $updateStmt slt_master_enrollment = ' , $updateStmt);
				// Execute query
				$num_updated = $updateStmt->execute();
				
				//Added for reverting the update when db_execution fails
				$result= new stdClass();
				$result->col_name = 'id';
				$result ->col_value = $master_enroll_id;
				$result ->table_name ='slt_master_enrollment';
				$result ->fields =array(
						'overall_status' => $ovr_sts[0]->overall_status,
						'comp_date' => $ovr_sts[0]->comp_date,
						'score' => $ovr_sts[0]->score,
						'overall_progress' => $ovr_sts[0]->overall_progress,
						'content_status' => $ovr_sts[0]->content_status,
						'updated_on' => $ovr_sts[0]->updated_on,
						'updated_by' => $ovr_sts[0]->updated_by
				);
				$statements['update'][] = $result;
				
				//may be here
				/* if(!empty($statements)) {
					$statements[] = $updateStmt;
				} else {
					$num_updated = $updateStmt->execute();
				} */
				expDebug::dPrint(' $num_updated = ' . print_r($num_updated, true) , 3);
				
				/* //get program id from slt_master_enrollment
				$selectStmt = db_select('slt_master_enrollment', 'menr');
				$selectStmt->addField('menr','program_id','program_id');
				$selectStmt->condition('menr.id',$master_enroll_id,'=');
				expDebug::dPrintDBAPI('get program id from slt_master_enrollment',$selectStmt);
				$prg_id = $selectStmt->execute()->fetchField();
				expDebug::dPrint('program id >>> ' .print_r($prg_id,true), 4); */

				if($tpstatus == 'lrn_tpm_ovr_inc'){
					//update slt_survey_results
					$selectStmt = db_select('slt_survey_mapping', 'surmap');
					$selectStmt->innerJoin('slt_survey','sur','sur.id = surmap.survey_id');
					$selectStmt->addExpression('GROUP_CONCAT(distinct surmap.survey_id)','surveyids');
					$selectStmt->condition('surmap.object_id',$prg_id,'=');
					$selectStmt->condition('surmap.object_type','cre_sys_obt_cls','!=');
					$selectStmt->condition('sur.type',array('sry_det_typ_ass','sry_det_typ_sry'),'IN');
					expDebug::dPrintDBAPI("update slt_survey_results ",$selectStmt);
					$results = $selectStmt->execute()->fetchField();
					$survey_ids = explode(',',$results);
						
					//update slt_survey results
					$updateStmt = db_update('slt_survey_results');
					$updateStmt->fields(array('re_attend' => 1));
					$updateStmt->condition('entity_id',$prg_id,'=');
					$updateStmt->condition('user_id', $uid,'=');
					$updateStmt->condition('survey_id', $survey_ids, 'IN');
					expDebug::dPrintDBAPI(' update slt_survey_results = ' , $updateStmt);
					//$survey_results = $updateStmt->execute();
					if(!empty($statements)) {
						$statements[] = $updateStmt;
					} else {
						$survey_results = $updateStmt->execute();
					}
				}
				lp_mark_program_complete($uid, $master_enroll_id, $prg_id, $updateuser ,$calltype ,'','','',$statements);
				
				//slp_mark_program_complete(xuid, xmaster_enrid, xprogram_id, xupdateuser, xcalltype, '');
			}

		}else{
			if($calltype == "'assesmentTP'" and !empty($pstatus) ){
				if($pstatus == 'lrn_crs_cmp_cmp'){
					$tpprestatus = 'lrn_tpm_ovr_cmp';
					//$tpprescore = $score;
				}else{
					$tpprestatus = 'lrn_tpm_ovr_inc';
					//$tpprescore = $score;
				}
				$updateStmt = db_update('slt_master_enrollment');
				// Set conditions
				$updateStmt->condition('id', $master_enroll_id,'=');
				$updateStmt->condition('overall_status', array('lrn_tpm_ovr_enr','lrn_tpm_ovr_inp'),'IN');
				// Set new values of fields
				$updateStmt->fields(array(
						'pre_status' => $tpprestatus,
						'pre_score' => $tpprescore,
						'content_status' => $master_enroll_content_status,
						'updated_by' => $updateuser,
						'updated_on' => now()
				));
				expDebug::dPrintDBAPI(' $updateStmt slt_master_enrollment = ' , $updateStmt);
				// Execute query
				//$num_updated = $updateStmt->execute();
				if(!empty($statements)) {
					$statements[] = $updateStmt;
				} else {
					$num_updated = $updateStmt->execute();
				}
				expDebug::dPrint(' $num_updated = ' . print_r($num_updated, true) , 3);
			}
			    // Update TP Overall Progress
			    if (!empty($master_enroll_id)) {
			    	$program_id = getProgramIdFromEnrollment($master_enroll_id);
			    	// To update  a overall percentage complete used in content player - new column
			    	//updateTPEnrollmentOverallPercentComplete($program_id, $master_enroll_id);
			    	updateTPEnrollmentOverallPercentComplete($program_id, $master_enroll_id,'','',$statements);
				}
		}
		if($calltype == "'assesmentTP'" || $calltypeforassess== "'assesmentTP'"){
			$program_id = getProgramIdFromEnrollment($master_enroll_id);
			updateAssSurTotAttempt($regid,$surveyid,$uid,$class_id,$course_id,$lesson_id,$version_id,$content_type,$program_id,$master_enroll_id,$statements);
		}
		else if($content_type ==  "sry_det_typ_ass" || $content_type == 'sry_det_typ_sry'){
			updateAssSurTotAttempt($regid,$surveyid,$uid,$class_id,$course_id,$lesson_id,$version_id,$content_type,0,0,$statements);
		}
		$selectStmt = db_select('slt_enrollment', 'enr');
		$selectStmt->addField('enr','comp_status','comp_status');
		$selectStmt->condition('enr.id',$regid,'=');
		$compstaft = $selectStmt->execute()->fetchField();

		$updsts = ($compstbef == $compstaft) ? "Not Changed" : getProfileListItemName($contentstatus);
		$result = new stdClass();
		$result->Id = $attend_Id;
		$result->CompStatus = $updsts;
		return $result;
	} catch(Exception $e){
		expDebug::dPrint('Error in lp_attendance_summary_ins '.$e,1);
		expertusLogError($e);
	}
}
/*
 * PHP function for sql procedure slp_attendance_summary_upd
 */
function lp_attendance_summary_upd($regid,$uid,$course_id,$class_id,$lesson_id,$version_id,
		$surveyid,$status,$pstatus,$score,$lessonloc,$type,$content_status,$completion_status,
			$launch_data,$suspend_data,$cmiexit,$aicc_data,$updateuser,$calltype,$assesscheck,$tottime,$launchflag,$content_type,$progress,$subtype='',&$statements=array()){
	try{
		expDebug::dPrint('func get args --->summ_upd'.var_export(func_get_args(), 1),5);
		
		$preassessment  = $assesscheck == '1' ? true : false;
		$postassessment = $assesscheck == '0' ? true : false;
		expDebug::dPrint('$asscount for pre assessment'.var_export($preassessment, 1));
		expDebug::dPrint('$asscount for post assessment'.var_export($postassessment, 1));
		
		$launchflag = (int) $launchflag;
		$totatempts = 0;
		if($type == 'Online' || $type == 'online') {

		}else{
			$totatempts=null;
			$tottime=null;
		}

		$tottime = (empty($tottime) || ($tottime==null) || ($tottime=='null')) ? '0.0' : $tottime;
		expDebug::dPrint("attendance pre summary details tottime>>> ".print_r($tottime,1),4);
		//73059: Content completion % is not getting updated unless we close the window and reopen it or move to the next content
		if (strpos($tottime, ".") !== false) {
			$int_timespend = "00:00:00";
			$timespend_split = explode(':',$int_timespend);
			$tmp = explode('.',$tottime);
			$tmp1 = explode(":",$tmp[0]);
			if($tmp1[0]>0) {
				$timespend_split[0] = intval($timespend_split[0])+(intval($tmp1[0])*60);
			}
			if($tmp1[1]>0) {
				$timespend_split[1] = intval($timespend_split[0])+intval($tmp1[1]);
			}
			if($tmp1[2]>0) {
				$timespend_split[2] = sprintf("%02s",($timespend_split[1] + $tmp1[2]));
			}
			$tottime =implode(":",$timespend_split);
		}
		expDebug::dPrint("attendance pre summary details after tottime>>> ".print_r($tottime,1),4);
		if($surveyid == 0){
			$summary_det = getAttendanceSummaryDetails($uid,$course_id,$class_id,$lesson_id,$version_id,$regid);
		}else{
			$summary_det =  getAttendanceSummaryDetails($uid,$course_id,$class_id,$lesson_id,$version_id,$regid,$surveyid);
		}
		
		//Added for reverting the update when db_execution fails
		$result= new stdClass();
		$result->col_name = 'id';
		$result ->col_value = $orderId;
		$result ->table_name ='slt_order';
		$result ->fields =array(
				'status' => $summary_det[0]->contentstatus,
				'last_attempt_date' => $summary_det[0]->contentlastdate,
				'score' => $summary_det[0]->contentscore,
				'assessment_id' => $surveyid,
				'last_lesson_location' => $summary_det[0]->plast_lesson_location,
				'launch_data' => $summary_det[0]->plaunch_data,
				'suspend_data' => $summary_det[0]->psuspend_data,
				'content_type' => $summary_det[0]->pcontent_type,
				'progress' => $summary_det[0]->progress,
				'cmiexit' => $summary_det[0]->pcmiexit,
				'aicc_data' => $summary_det[0]->paicc_data,
				'updated_by' => $summary_det[0]->pupdated_by,
				'updated_on' => $summary_det[0]->pupdated_on,
				'content_status'=>$summary_det[0]->pcontentsts,
				'completion_status'=>$summary_det[0]->pcompletionsts,
				'custom2'=>$summary_det[0]->pcustom2,
				'custom3'=>$summary_det[0]->pcustom3,
				'custom4'=>$summary_det[0]->pcustom
		);
		$statements['update'][] = $result;
		
		$pcontent_status = $summary_det[0]->pcontentsts;
		$pcompletion_status = $summary_det[0]->pcompletionsts;
		$prev_progress = $summary_det[0]->progress;
		if($summary_det[0]->contentscore > $score && $completion_status == 'lrn_crs_cmp_cmp'){
			$score = $summary_det[0]->contentscore;
			$content_status ='completed';
		}elseif($summary_det[0]->contentscore > $score){
			$score = $summary_det[0]->contentscore;
		}
		elseif($summary_det[0]->pcontentsts == 'completed'){
				
		}
		else{
			$pcontent_status = $content_status;
			$pcompletion_status = $completion_status;
		}
	    
	    expDebug::dPrint("attendance pre summary details >>> ".print_r($summary_det[0],1),4);
	    	if((int) $progress > (int) $prev_progress)
	    		$progressUpdate = $progress;
	    	else
	    		$progressUpdate = $prev_progress;
			//73059: Content completion % is not getting updated unless we close the window and reopen it or move to the next content
    	if(($content_type == 'sry_det_typ_sry'  || $content_type == 'sry_det_typ_ass') && $launchflag == 1 && $summary_det[0]->pcontentsts=='suspend')
    		$progressUpdate = $progress;
	    expDebug::dPrint("attendance New progress value >>> ".print_r($progressUpdate,1),4);
	    
		//Update slt_attendance_summary
		$updateStmt = db_update('slt_attendance_summary');
		$updateFields = array(
				'status' => $status,
				'last_attempt_date' => now(),
				'score' => $score,
				'assessment_id' => $surveyid,
				'last_lesson_location' => $lessonloc,
				'launch_data' => $launch_data,
				'suspend_data' => $suspend_data,
		   	    'content_type' => $content_type,
		   	    'progress' => $progressUpdate,
				'cmiexit' => $cmiexit,
				'aicc_data' => $aicc_data,
				'updated_by' => $updateuser,
				'updated_on' => now()
		);

			
		if($surveyid == 0){
			$content_status = strtolower($content_status);
			if(($content_status == 'completed' || $content_status == 'passed' || $content_status == 'failed' || $content_status == 'incomplete' || $content_status == 'inprogress')
					|| ($calltype == "'SCORM_2004'" && ($completion_status == 'incomplete' || ($content_status == 'unknown' && $completion_status != 'incomplete') ))){
				$totatempts=1;
			}else{
				$totatempts=0;
			}
			$updateFields['content_status'] = $pcontent_status;
			$updateFields['completion_status'] = $pcompletion_status;
			 
			//select custom fields
			$selectStmt = db_select('slt_attendance_summary', 'summ');
			$selectStmt->addField('summ','custom2');
			$selectStmt->addField('summ','custom3');
			$selectStmt->condition('summ.user_id',$uid,'=');
			$selectStmt->condition('summ.course_id',$course_id,'=');
			$selectStmt->condition('summ.class_id',$class_id,'=');
			$selectStmt->condition('summ.lesson_id',$lesson_id,'=');
			$selectStmt->condition('summ.content_version_id',$version_id,'=');
			$selectStmt->condition('summ.enrollment_id',$regid,'=');
			expDebug::dPrintDBAPI('get previous summary status',$selectStmt);
			$summ_res = $selectStmt->execute()->fetchAll();
			 
			$custom4 = ($launchflag == 1) ? 'is_current' : null;
			 
			if($summ_res[0]->custom2 != null){
				$custom2 = $totatempts;
				$custom3 = $tottime;
				$totatempts = ($totatempts - $summ_res[0]->custom2);
				// Time Difference calculation
				$date = "2013-04-15"; // Dummy date for time subtract
				$time = $tottime;
				$date1 = "2013-04-15"; // Dummy date for time subtract
				$time1 = $summ_res[0]->custom3;
			//73059: Content completion % is not getting updated unless we close the window and reopen it or move to the next content
				if (strpos($time1, ".") !== false) {
					$int_timespend = "00:00:00";
					$timespend_split = explode(':',$int_timespend);
					$tmp = explode('.',$time1);
					$tmp1 = explode(":",$tmp[0]);
					if($tmp1[0]>0) {
						$timespend_split[0] = intval($timespend_split[0])+(intval($tmp1[0])*60);
					}
					if($tmp1[1]>0) {
						$timespend_split[1] = intval($timespend_split[0])+intval($tmp1[1]);
					}
					if($tmp1[2]>0) {
						$timespend_split[2] = sprintf("%02s",($timespend_split[1] + $tmp1[2]));
					}else{
						$timespend_split[2] = '00';
					}
					$time1 =implode(":",$timespend_split);
				}
				expDebug::dPrint("time-------->".print_r($time,1),4);
				expDebug::dPrint("time1-------->".print_r($time1,1),4);
				if(($time !=null && $time1!=null && $time !='null' && $time1!='null')){
					$datetime1 = new DateTime($date .' '.$time);
					$datetime2 =  new DateTime($date1 .' '.$time1);
					$interval = $datetime1->diff($datetime2);
						
					$tottime = $interval->format('%H:%I:%S');
				}
			}else{
				$custom2 = $totatempts;
				$custom3 = $tottime;
			}
			 
							if ($launchflag == 0) {
				$custom2 = null;
				$custom3 = null;
			}
			 
			$updateFields['custom2'] = $custom2;
			$updateFields['custom3'] = $custom3;
			$updateFields['custom4'] = $custom4;
			 
		}else{
			$totatempts=1;
		    	$updateFields['content_status'] = $pcontent_status;
			$updateStmt->condition('assessment_id', $surveyid,'=');
		}
		$updateStmt->fields($updateFields);
		if($content_type !=  "sry_det_typ_ass" && $content_type != 'sry_det_typ_sry')
			$updateStmt->expression('total_attempts', 'total_attempts+'.$totatempts);
		$updateStmt->expression('total_time_spend', 'ADDTIME(total_time_spend,"'.$tottime.'")');
		$updateStmt->condition('user_id',$uid,'=');
		$updateStmt->condition('course_id', $course_id,'=');
		$updateStmt->condition('class_id', $class_id,'=');
		$updateStmt->condition('lesson_id', $lesson_id,'=');
		$updateStmt->condition('content_version_id', $version_id,'=');
		$updateStmt->condition('enrollment_id', $regid,'=');
		    expDebug::dPrintDBAPI(' update slt_attendance summary final = ' , $updateStmt);
		$summ_upd = $updateStmt->execute();
		
		//Skipped this statement for score/progress related slt_enrollment updates
		 /*    if(!empty($statements)) {
		    	$statements[] = $updateStmt;
		    } else {
		    	$summ_upd = $updateStmt->execute();
		    } */

		expDebug::dPrint("attendance summary id >>> ".$attend_Id);
			
		//get previous enrollment details
		$selectStmt = db_select('slt_enrollment', 'enr');
		$selectStmt->addField('enr','content_status','content_status');
		$selectStmt->addField('enr','comp_status','comp_status');
		$selectStmt->addField('enr','is_compliance','is_compliance');
		$selectStmt->addField('enr','score','score');
		$selectStmt->addField('enr','master_enrollment_id','master_enrollment_id');
		$selectStmt->addField('enr','pre_status','pre_status');
		$selectStmt->addField('enr','pre_score','pre_score');
			$selectStmt->addField('enr','progress','prev_progress');
		$selectStmt->addField('enr','comp_by','comp_by');
		$selectStmt->addExpression('IF(comp_status =\'lrn_crs_cmp_cmp\', comp_date, NOW())','comp_date');
		$selectStmt->condition('enr.id',$regid,'=');
		expDebug::dPrintDBAPI(' previous enrollment details query = ' , $selectStmt);
		$enroll_details = $selectStmt->execute()->fetchAll();
		$prev_content_status = $enroll_details[0]->content_status;
		$compstbef = $enroll_details[0]->comp_status;
		$prev_score = $enroll_details [0]->score;			//previous post asessment score or content score
		$prev_pre_score = $enroll_details [0]->pre_score;
		expDebug::dPrintDBAPI('get previous content status111',$selectStmt);
		expDebug::dPrint('previous content status>> '.print_r($prev_content_status,true), 4);
		$prev_progress_enroll = $enroll_details[0]->prev_progress;
		if(strtolower($prev_content_status) == 'passed' || strtolower($prev_content_status) == 'completed'){
			$enrollment_content_status =  $prev_content_status;
		}else{
			$enrollment_content_status =  getclassContentStatus($class_id,$regid);
		}
		$selectStmt = db_select('slt_enrollment', 'enr');
		$selectStmt->addField('enr','comp_status','comp_status');
		$selectStmt->condition('enr.id',$regid,'=');
		$compstbef = $selectStmt->execute()->fetchField();

			//Check whether post assessment available for the class
			$selectStmt = db_select('slt_survey_mapping', 'assmap');
			$selectStmt->innerJoin('slt_survey', 'srv','srv.id = assmap.survey_id AND srv.type = \'sry_det_typ_ass\'');
			$selectStmt->addExpression('count(1)','asscount');
			$selectStmt->condition('assmap.object_type','cre_sys_obt_cls','=');
			$selectStmt->condition('assmap.object_id',$class_id,'=');
			$selectStmt->condition('assmap.pre_status',0,'=');
			expDebug::dPrintDBAPI('get previous content status',$selectStmt);
			$asscount = $selectStmt->execute()->fetchField();
			expDebug::dPrint('Check whether assessment available for the class '.print_r($asscount,true), 4);

		//get version ids of all the contents associated with the class based on enrollment
			$selectType = db_select('slt_course_class', 'cls');
			$selectType->addField('cls', 'delivery_type');
			$selectType->condition('cls.id', $class_id,'=');
			$delType = $selectType->execute()->fetchField();
			
		if($delType == 'lrn_cls_dty_wbt' || $delType == 'lrn_cls_dty_vod') {
			$versions_arr = array();
			$versions_array = getcontentVersions($class_id, $regid, $uid);
			foreach($versions_array as $version_record) {
				if(! in_array($version_record->vid, $versions_arr)) {
					$versions_arr[] = $version_record->vid;
				}
			}
			
			// get score from summary for content
			$selectStmt = db_select('slt_attendance_summary', 'summ');
			$selectStmt->innerJoin('slt_enrollment_content_mapping', 'emap', 'emap.enroll_id = summ.enrollment_id AND summ.content_version_id = emap.version_id');
			$selectStmt->addExpression('sum(summ.score)', 'tscore');
			$selectStmt->condition('summ.user_id', $uid, '=');
			$selectStmt->condition('summ.course_id', $course_id, '=');
			$selectStmt->condition('summ.class_id', $class_id, '=');
			$selectStmt->condition('summ.enrollment_id', $regid, '=');
			$selectStmt->condition('summ.content_version_id', $versions_arr, 'IN');
			$selectStmt->condition('summ.assessment_id', '0', '=');
			expDebug::dPrintDBAPI('get score from summary ', $selectStmt);
			$tcontentscore = $selectStmt->execute()->fetchField();
			expDebug::dPrint('get score from summary  ' . $tcontentscore, 4);
		}
			
		// get score from summary for post assessment
		$selectStmt = db_select('slt_attendance_summary', 'summ');
	//	$selectStmt->leftjoin('slt_enrollment_content_mapping', 'emap', 'emap.enroll_id = summ.enrollment_id AND summ.class_id = emap.class_id');
		$selectStmt->innerJoin('slt_survey', 'srv', 'srv.id = summ.assessment_id AND srv.type = \'sry_det_typ_ass\'');
		$selectStmt->innerJoin('slt_survey_mapping', 'assmap', 'srv.id = assmap.survey_id and assmap.object_id = summ.class_id and assmap.object_type =\'cre_sys_obt_cls\'');
		$selectStmt->addExpression('summ.score', 'tassessmentscore');
		$selectStmt->condition('summ.user_id', $uid, '=');
		$selectStmt->condition('summ.course_id', $course_id, '=');
		$selectStmt->condition('summ.class_id', $class_id, '=');
		$selectStmt->condition('summ.enrollment_id', $regid, '=');
		$selectStmt->condition('summ.assessment_id', '0', '!=');
		$selectStmt->condition('summ.content_version_id', '0', '=');
		$selectStmt->condition('assmap.pre_status', 0, '=');
		expDebug::dPrintDBAPI('get score from summary assessment', $selectStmt);
		$tassessmentscore = $selectStmt->execute()->fetchField();
		expDebug::dPrint('get score from summary  assessment21212121-->' . $tassessmentscore, 4);
		
		if($preassessment) {
			// get score from summary for pre assessment
			$selectStmt = db_select('slt_attendance_summary', 'summ');
		//	$selectStmt->leftjoin('slt_enrollment_content_mapping', 'emap', 'emap.enroll_id = summ.enrollment_id AND summ.class_id = emap.class_id');
			$selectStmt->innerJoin('slt_survey', 'srv', 'srv.id = summ.assessment_id AND srv.type = \'sry_det_typ_ass\'');
			$selectStmt->innerJoin('slt_survey_mapping', 'assmap', 'srv.id = assmap.survey_id and assmap.object_id = summ.class_id and assmap.object_type =\'cre_sys_obt_cls\'');
			$selectStmt->addExpression('summ.score', 'tassessmentscore');
			$selectStmt->condition('summ.user_id', $uid, '=');
			$selectStmt->condition('summ.course_id', $course_id, '=');
			$selectStmt->condition('summ.class_id', $class_id, '=');
			$selectStmt->condition('summ.enrollment_id', $regid, '=');
			$selectStmt->condition('summ.assessment_id', '0', '!=');
			$selectStmt->condition('summ.content_version_id', '0', '=');
			$selectStmt->condition('assmap.pre_status', 1, '=');
			// $selectStmt->groupby('summ.enrollment_id');
			expDebug::dPrintDBAPI('get pre score from summary assessment', $selectStmt);
			$tpreassessmentscore = $selectStmt->execute()->fetchField();
			expDebug::dPrint('get pre score from summary  assessment21212121-->' . $tpreassessmentscore, 4);
		}

		$tscore = $prev_score;
		$prescore=$prev_pre_score;
		
		if ($asscount > 0) { // $asscount for availability of post assessment
			if ($postassessment) { // $assesscheck = 0 for post assessment
				$tscore = max($tassessmentscore, $prev_score);
			}else if ($preassessment) { // $assesscheck = 1 for pre assessment
				$prescore = max($tpreassessmentscore,$prev_pre_score);
		}
		} else {
			if ($preassessment) { // $assesscheck = 1 for pre assessment
				$prescore = max($tpreassessmentscore,$prev_pre_score);
			} else {
				$tscore = max($tcontentscore,$prev_score);
			}
		}
		
		expDebug::dPrint('Check prescore update-->joola'.$prescore);
		expDebug::dPrint('Check score update-->joola'.$tscore);
		
		if($enroll_details[0]->comp_status=='lrn_crs_cmp_cmp' && $calltype != "'assesmentTP'"){
			$contentstatus = $enroll_details[0]->comp_status;
		}
		else{
			if($calltype == "'assessment'" || $calltype == "'assesmentTP'"){
				$contentstatus =  $status;
			}else{
				$contentstatus = getcontentCount($class_id, $uid, $regid,$subtype);
				if($asscount >0){
					if($contentstatus == 'lrn_crs_cmp_cmp' || $contentstatus == 'lrn_crs_cmp_inc')
						$contentstatus = $enroll_details[0]->comp_status;
				}
			}
			
		}
		
		//Audit trail entry
		$master_enroll_id = $enroll_details[0]->master_enrollment_id;
		$pcomp_date = $enroll_details[0]->comp_date;
		//get master enrollment status
		$selectStmt = db_select('slt_master_enrollment', 'enr');
		$selectStmt->addField('enr','overall_status','overall_status');
		$selectStmt->condition('enr.id',$master_enroll_id,'=');
		expDebug::dPrintDBAPI('get master enrollment status  ',$selectStmt);
		$overallstaus_old = $selectStmt->execute()->fetchField();
		$master_enroll_content_status = getTpContentStatus($master_enroll_id);
			
			
		if($contentstatus == 'lrn_crs_cmp_inc' ||  $contentstatus == 'lrn_crs_cmp_cmp')
			$comp_date =  $pcomp_date;
		else
			$comp_date =  NULL;

		if($calltype != "'assesmentTP'"){
			if($assesscheck == 1){
				$assessstatus = $pstatus;
			}else{
				$assessstatus = $enroll_details[0]->pre_status;
				$iscompliance = $enroll_details[0]->is_compliance;
			}

			//Update status for incomplete and inprogress
			if($contentstatus != 'lrn_crs_cmp_cmp'){
				//update slt_enrollment
				$updateStmt = db_update('slt_enrollment');
				$updateStmt->fields(array(
						'comp_status' => $contentstatus,
						'pre_status' => $assessstatus,
						'comp_date' => $comp_date,
						'score' => $tscore,
						'content_status' => $enrollment_content_status,
						'pre_score' => $prescore,
						'updated_by' => $updateuser,
						'updated_on' => now()
				));
				$updateStmt->condition('user_id',$uid,'=');
				$updateStmt->condition('course_id', $course_id,'=');
				$updateStmt->condition('class_id', $class_id,'=');
				$updateStmt->condition('id', $regid,'=');
				$updateStmt->condition('comp_status', array('lrn_crs_cmp_inc'),'NOT IN');
				$updateStmt->condition('reg_status', 'lrn_crs_reg_cnf' ,'=');
				expDebug::dPrintDBAPI(' update slt_enrollment11joo = ' , $updateStmt);
				$enroll_upd = $updateStmt->execute();
				
				$result= new stdClass();
				$result->col_name = 'id';
				$result ->col_value = $regid;
				$result ->table_name ='slt_enrollment';
				$result ->fields =array(
						'comp_status' => $enroll_details[0]->comp_status,
						'pre_status' => $enroll_details[0]->pre_status,
						'comp_date' => $enroll_details[0]->comp_date,
						'score' => $enroll_details[0]->score,
						'content_status' => $enroll_details[0]->content_status,
						'pre_score' => $enroll_details[0]->pre_score,
						'updated_by' => $enroll_details[0]->updated_by,
						'updated_on' => $enroll_details[0]->updated_on
				);
				$statements['update'][] = $result;
				/* if(!empty($statements)) {
				 $statements[] = $updateStmt;
				 } else {
				 $enroll_upd = $updateStmt->execute();
				 } */
				if($contentstatus == 'lrn_crs_cmp_inc'){
					//update slt_survey_results
					$selectStmt = db_select('slt_survey_mapping', 'surmap');
					$selectStmt->innerJoin('slt_survey','sur','sur.id = surmap.survey_id');
					$selectStmt->addExpression('GROUP_CONCAT(distinct surmap.survey_id)','surveyids');
					$selectStmt->condition('surmap.object_id',$class_id,'=');
					$selectStmt->condition('surmap.object_type','cre_sys_obt_cls','=');
					$selectStmt->condition('sur.type',array('sry_det_typ_ass','sry_det_typ_sry'),'IN');
					expDebug::dPrintDBAPI("update slt_survey_results ",$selectStmt);
					$results = $selectStmt->execute()->fetchField();
					$survey_ids = explode(',',$results);

					//update slt_survey results
					$updateStmt = db_update('slt_survey_results');
					$updateStmt->fields(array('re_attend' => 1));
					$updateStmt->condition('entity_id',$class_id,'=');
					$updateStmt->condition('user_id', $uid,'=');
					$updateStmt->condition('survey_id', $survey_ids, 'IN');
					expDebug::dPrintDBAPI(' update slt_survey_results = ' , $updateStmt);
					//$survey_results = $updateStmt->execute();
					if(!empty($statements)) {
						$statements[] = $updateStmt;
					} else {
						$survey_results = $updateStmt->execute();
					}
				}
			}else{
				//$enrscore = ($tscore > $enrscore) ? $tscore : $enrscore;
				$compuserid = $enroll_details[0]->comp_by;
				$selectStmt = db_select('slt_enrollment', 'e');
				$selectStmt->addExpression('GROUP_CONCAT(e.id)','masterids');
				$selectStmt->condition('e.user_id',$uid,'=');
				$selectStmt->condition('e.class_id',$class_id,'=');
				$selectStmt->condition('e.master_enrollment_id','', 'IS NOT NULL');
				expDebug::dPrintDBAPI("update slt_enrollment for master enrollments ",$selectStmt);
				$results = $selectStmt->execute()->fetchField();
				$master_ids = explode(',',$results);
				if(empty($compuserid)){
					//Update slt_enrollment
					$updateStmt = db_update('slt_enrollment');
					$updateFields = array(
							'comp_status' => $contentstatus,
							'pre_status' => $assessstatus,
							'comp_date' => $comp_date,
							'score' => $tscore,
							'pre_score' => $prescore,
							'content_status' => $enrollment_content_status,
							'updated_by' => $updateuser,
							'updated_on' => now()
					);
					$updateFields['comp_by'] = $updateuser;
					$updateFields['comp_on'] = now();
					$updateStmt->condition('comp_status', array('lrn_crs_cmp_inc','lrn_crs_cmp_nsw'),'NOT IN');
					if(empty($master_enroll_id)){
						$updateStmt->condition(db_or()
								->condition('id', $regid,'=')
								->condition('id', $master_ids , 'IN'));
					}
					$updateStmt->fields($updateFields);
					$updateStmt->condition('user_id',$uid,'=');
					$updateStmt->condition('class_id', $class_id,'=');
					$updateStmt->condition('reg_status', 'lrn_crs_reg_cnf' ,'=');
					expDebug::dPrintDBAPI(' update slt_enrollment22 = ' , $updateStmt);
					$enroll_upd = $updateStmt->execute();
				/* 	if(!empty($statements)) {
					$statements[] = $updateStmt;
				} else {
					$enroll_upd = $updateStmt->execute();
				} */
				$result= new stdClass();
				$result->col_name = 'id';
				$result ->col_value = $regid;
				$result ->table_name ='slt_enrollment';
				$result ->fields =array(
						'comp_status' => $enroll_details[0]->comp_status,
						'pre_status' => $enroll_details[0]->pre_status,
						//'comp_date' => $enroll_details[0]->comp_date,
						'score' => $enroll_details[0]->score,
						'content_status' => $enroll_details[0]->content_status,
						'pre_score' => $enroll_details[0]->pre_score,
						'updated_by' => $enroll_details[0]->updated_by,
						'updated_on' => $enroll_details[0]->updated_on,
						'comp_date' => $enroll_details[0]->comp_date,
						'comp_by' => $enroll_details[0]->comp_by,
						'comp_on' => $enroll_details[0]->comp_on,
				);
				$statements['update'][] = $result;
				}
				$updateStmt = db_update('slt_enrollment');
				$updateFields = array(
						'comp_status' => $contentstatus,
						'pre_status' => $assessstatus,
						'comp_date' => $comp_date,
						'score' => $tscore,
						'pre_score' => $prescore,
						'content_status' => $enrollment_content_status,
						'updated_by' => $updateuser,
						'updated_on' => now()
				);
				$updateStmt->condition('comp_status', array('lrn_crs_cmp_inc','lrn_crs_cmp_nsw'),'NOT IN');
				if(empty($master_enroll_id)){
					$updateStmt->condition('id', $regid,'=');
				}
				$updateStmt->fields($updateFields);
				$updateStmt->condition('user_id',$uid,'=');
				$updateStmt->condition('class_id', $class_id,'=');
				$updateStmt->condition('reg_status', 'lrn_crs_reg_cnf' ,'=');
				expDebug::dPrintDBAPI(' update slt_enrollment333 = ' , $updateStmt);
				$enroll_upd = $updateStmt->execute();
				/* if(!empty($statements)) {
					$statements[] = $updateStmt;
				} else {
					$enroll_upd = $updateStmt->execute();
				} */
				$result= new stdClass();
				$result->col_name = 'id';
				$result ->col_value = $regid;
				$result ->table_name ='slt_enrollment';
				$result ->fields =array(
						'comp_status' => $enroll_details[0]->comp_status,
						'pre_status' => $enroll_details[0]->pre_status,
						'comp_date' => $enroll_details[0]->comp_date,
						'score' => $enroll_details[0]->score,
						'content_status' => $enroll_details[0]->content_status,
						'pre_score' => $enroll_details[0]->pre_score,
						'updated_by' => $enroll_details[0]->updated_by,
						'updated_on' => $enroll_details[0]->updated_on
				);
				$statements['update'][] = $result;
				
				/* if(isset($_SESSION['menrid'][$master_enroll_id]['next_launchable_sequence'])){
					unset($_SESSION['menrid'][$master_enroll_id]['next_launchable_sequence']);
				}  */
			}
						//progress update for enrollment.
						$sid = $uid."-".$course_id."-".$class_id;
						$ClassDelType = getClassDeliveryType($class_id, 'Short');
						expDebug::dPrint(' Class Type = ' . $ClassDelType , 4);
						include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_learning/exp_sp_learning_learner/modules/exp_sp_lnrenrollment/exp_sp_launch.inc');
						$launch_detail = ($ClassDelType=='wbt' || $ClassDelType=='vc' || $ClassDelType=='vod')?getLaunchDetail($regid,'',$sid):getLaunchDetail($regid);
						$progress = get_progress($launch_detail,$regid,1);
						if(($calltype == "'assessment'" || $calltype == "'assesmentTP'") && $contentstatus == "lrn_crs_cmp_cmp" && $content_type ==  "sry_det_typ_ass"  ){
							$progress = 100;
						}
						
						$update_progress  = ($progress>$prev_progress_enroll) ? $progress : $prev_progress_enroll;
						
						$selectStmt = db_select('slt_enrollment', 'e');
						$selectStmt->addExpression('GROUP_CONCAT(e.id)','masterids');
						$selectStmt->condition('e.user_id',$uid,'=');
						$selectStmt->condition('e.course_id',$course_id,'=');
						$selectStmt->condition('e.class_id',$class_id,'=');
						$selectStmt->condition('e.master_enrollment_id','', 'IS NOT NULL');
						expDebug::dPrintDBAPI("update slt_enrollment for master enrollments ",$selectStmt);
						$results = $selectStmt->execute()->fetchField();
						$master_ids = explode(',',$results);
						
						$updateQuery = db_update('slt_enrollment');
						$updateQuery->fields(array('progress'=>$update_progress));
						if($update_progress == 100){ //progress rollup for enrollments
							if(empty($master_enroll_id)){
								$updateQuery->condition('user_id',$uid,'=');
								$updateQuery->condition('course_id', $course_id,'=');
								$updateQuery->condition('class_id', $class_id,'=');
							}else{
								$updateQuery->condition(db_or()
										->condition('id', $regid,'=')
										->condition('id', $master_ids , 'IN'));
							}
								$updateQuery->condition('comp_status', array('lrn_crs_cmp_inc','lrn_crs_cmp_nsw'),'NOT IN');
								$updateQuery->condition('reg_status', 'lrn_crs_reg_cnf' ,'=');
						}else{
						$updateQuery->condition('id',$regid,'=');
						}
						expDebug::dPrintDBAPI(' UpdateQuery = ' , $updateQuery,4);
						//may be here
						$rosUpdated = $updateQuery->execute();
						/* if(!empty($statements)) {
							$statements[] = $updateQuery;
						} else {
							$rosUpdated = $updateQuery->execute();
						} */
						$result= new stdClass();
						$result->col_name = 'id';
						$result ->col_value = $regid;
						$result ->table_name ='slt_enrollment';
						$result ->fields =array(
								'progress'=>$prev_progress_enroll
						);
						$statements['update'][] = $result;
						
					// Update launchable in slt_enrollment table
					$totalattemptsleft = CalculateTotalAttemptsLeft($class_id,$regid);
					
					expDebug::dPrint("totalattemptslefttotalattemptsleft : ".print_r($totalattemptsleft,true),5);
					
					if(($totalattemptsleft) <= 0){
						//$updateattempts = db_query("UPDATE slt_enrollment SET launchable=0 WHERE id=".$regid);
						$updateStmt = db_update('slt_enrollment');
						$updateStmt->fields(array('launchable' => 0));
						$updateStmt->condition('id',$regid,'=');
						//$updateattempts = $updateStmt->execute();
						if(!empty($statements)) {
							$statements[] = $updateStmt;
						} else {
							$updateattempts = $updateStmt->execute();
						}
						expDebug::dPrintDBAPI(' $updateattempts = ' , $updateStmt,4);
					}
						
						
						
						
		}else{ // else for ($calltype != 'assesmentTP')
			//Update slt_enrollment
			$updateStmt = db_update('slt_enrollment');
			$updateStmt->fields(array(
					'comp_status' => $contentstatus,
					'pre_status' => $assessstatus,
					'comp_date' => $comp_date,
					'updated_by' => $updateuser,
					'content_status' => $enrollment_content_status,
					'updated_on' => now()
			));
			$updateStmt->condition('master_enrollment_id',$master_enroll_id,'=');
			$updateStmt->condition('comp_status', array('lrn_crs_cmp_cmp','lrn_crs_cmp_inc','lrn_crs_cmp_nsw'),'NOT IN');
			$updateStmt->condition('reg_status', 'lrn_crs_reg_cnf' ,'=');
			expDebug::dPrintDBAPI(' update slt_enrollment444 = ' , $updateStmt);
			//$enroll_upd = $updateStmt->execute();
			if(!empty($statements)) {
				$statements[] = $updateStmt;
			} else {
				$enroll_upd = $updateStmt->execute();
			}
		}
		if(!empty($master_enroll_id)){
		//get program details from slt_master_enrollment
		$selectStmt = db_select('slt_master_enrollment', 'menr');
		$selectStmt->addField('menr','program_id','program_id');
		$selectStmt->addField('menr','score','tpprevscore');
		$selectStmt->addField('menr','pre_score','tpprescore');
		$selectStmt->condition('menr.id',$master_enroll_id,'=');
		expDebug::dPrintDBAPI('get program id from slt_master_enrollment',$selectStmt);
		$programdetails = $selectStmt->execute()->fetchAll();
		expDebug::dPrint('programdetails id >>> ' .print_r($programdetails,true), 4);

		$prg_id =$programdetails[0]->program_id;
		$tpprev_score = $programdetails[0]->score;			//previous post asessment score or content score
		$tpprev_pre_score = $programdetails[0]->pre_score;	//previous pre asessment score

		expDebug::dPrint('$master_enroll_id >> '.print_r($master_enroll_id,true), 4);
		
		
		//Get score from enrollment for TP
		$selectStmt = db_select('slt_enrollment', 'enr');
		$selectStmt->addExpression('sum(enr.score)','tpoverallscore');
		$selectStmt->condition('enr.user_id',$uid,'=');
		$selectStmt->condition('enr.master_enrollment_id',$master_enroll_id,'=');
		expDebug::dPrintDBAPI('get overallscore',$selectStmt);
		$tpoverallscore = $selectStmt->execute()->fetchField();
		expDebug::dPrint('Check overallscore for TP '.print_r($tpoverallscore,true), 4);

		//Check whether post assessment available for the TP
		$selectStmt = db_select('slt_survey_mapping', 'assmap');
		$selectStmt->innerJoin('slt_survey', 'srv','srv.id = assmap.survey_id AND srv.type = \'sry_det_typ_ass\'');
		$selectStmt->addExpression('count(1)','tpasscount');
		$selectStmt->condition('assmap.object_type','cre_sys_obt_cls','!=');
		$selectStmt->condition('assmap.object_id',$prg_id,'=');
		$selectStmt->condition('assmap.pre_status',0,'=');
		expDebug::dPrintDBAPI('get assessment available for TP',$selectStmt);
		$tpasscount = $selectStmt->execute()->fetchField();
		expDebug::dPrint('Check whether assessment available for TP '.print_r($tpasscount,true), 4);		
		
		//Get score from summary for post assessment 
		$selectStmt = db_select('slt_attendance_summary', 'summ');
		$selectStmt->leftjoin('slt_enrollment', 'enr','enr.id = summ.enrollment_id AND enr.user_id = summ.user_id');
		$selectStmt->leftjoin('slt_survey', 'srv','srv.id = summ.assessment_id AND srv.type = \'sry_det_typ_ass\'');
			$selectStmt->leftjoin('slt_survey_mapping', 'assmap','srv.id = assmap.survey_id and assmap.object_id="'.$prg_id.'" and assmap.object_type IN (\'cre_sys_obt_crt\',\'cre_sys_obt_cur\',\'cre_sys_obt_trn\')');
		$selectStmt->addField('summ','score','tpassessmentscore');
		$selectStmt->condition('summ.user_id',$uid,'=');
		$selectStmt->condition('summ.enrollment_id',$regid,'=');
		$selectStmt->condition('summ.assessment_id','0','!=');
		$selectStmt->condition('summ.content_version_id','0','=');
		$selectStmt->condition('assmap.pre_status',0,'=');
		$selectStmt->condition('assmap.object_id',$prg_id,'=');
		$selectStmt->condition('assmap.object_type','cre_sys_obt_cls','!=');
		$selectStmt->condition('enr.master_enrollment_id','', 'IS NOT NULL');
		expDebug::dPrintDBAPI('get score from summary assessment',$selectStmt);
		$tpassessmentscore =  $selectStmt->execute()->fetchField();
		expDebug::dPrint('get score from summary for TP post assessment'.$tpassessmentscore, 4);
		
		
		//Get score from summary for pre assessment
		if($preassessment){
		$selectStmt = db_select('slt_attendance_summary', 'summ');
		$selectStmt->leftjoin('slt_enrollment', 'enr','enr.id = summ.enrollment_id AND enr.user_id = summ.user_id');
		$selectStmt->leftjoin('slt_survey', 'srv','srv.id = summ.assessment_id AND srv.type = \'sry_det_typ_ass\'');
			$selectStmt->leftjoin('slt_survey_mapping', 'assmap','srv.id = assmap.survey_id and assmap.object_id="'.$prg_id.'" and assmap.object_type IN (\'cre_sys_obt_crt\',\'cre_sys_obt_cur\',\'cre_sys_obt_trn\')');
		$selectStmt->addField('summ','score','tpassessmentscore');
		$selectStmt->condition('summ.user_id',$uid,'=');
		$selectStmt->condition('summ.enrollment_id',$regid,'=');
		$selectStmt->condition('summ.assessment_id','0','!=');
		$selectStmt->condition('summ.content_version_id','0','=');
		$selectStmt->condition('assmap.pre_status',1,'=');
		$selectStmt->condition('assmap.object_id',$prg_id,'=');
		$selectStmt->condition('assmap.object_type','cre_sys_obt_cls','!=');
		$selectStmt->condition('enr.master_enrollment_id','', 'IS NOT NULL');
		expDebug::dPrintDBAPI('get score from summary assessment',$selectStmt);
		$tppreassessmentscore =  $selectStmt->execute()->fetchField();
		expDebug::dPrint('get score from summary for TP pre assessment'.$tppreassessmentscore, 4);
		}
		
		$tpscore = $tpprev_score;
		$tpprescore = $tpprev_pre_score;
		
		if ($tpasscount > 0) {
			if ($calltype == "'assesmentTP'" && $postassessment) {
				$tpscore =max($tpassessmentscore,$tpprev_score);
			} else if ($calltype == "'assesmentTP'" && $preassessment) { // $assesscheck = 1 for pre assessment
				//$tpscore =max($tpassessmentscore,$tpprev_score);
				$tpprescore =max($tppreassessmentscore,$tpprev_pre_score);
			}
		} else {
			if ($calltype == "'assesmentTP'" && $preassessment) { // $assesscheck = 1 for pre assessment
				$tpprescore =max($tppreassessmentscore,$tpprev_pre_score);
			} else {
				$tpscore =max($tpoverallscore,$tpprev_score);
			}
		}
		}
		//Roll up TP
		if($contentstatus != 'lrn_crs_cmp_inp' && $contentstatus != 'lrn_crs_cmp_enr'){

			$master_enroll_content_status = getTpContentStatus($master_enroll_id);
			if($calltype != "'assesmentTP'"){
				$selectStmt = db_select('slt_enrollment', 'enr');
				$selectStmt->addField('enr','master_enrollment_id','master_enrollment_id');
				$selectStmt->condition('enr.user_id',$uid,'=');
				$selectStmt->condition('enr.course_id',$course_id,'=');
				$selectStmt->condition('enr.reg_status','lrn_crs_reg_cnf','=');
				$selectStmt->condition('enr.master_enrollment_id','', 'IS NOT NULL');
				expDebug::dPrintDBAPI("select list of master enrollment ",$selectStmt);
				$menr_ids = $selectStmt->execute()->fetchCol();
				expdebug::DPrint("master enroll ids >> ".print_r($menr_ids,true),5);
				foreach($menr_ids as $mid){
					expdebug::DPrint("master enroll ids 111>> ".print_r($mid,true),5);
					//expdebug::DPrint("master enroll ids 222>> ".$mid->master_enrollment_id);
					$program_id = getProgramIdFromEnrollment($mid);
					expdebug::DPrint("Program id for enrollment id >> ".$program_id);
					lp_mark_program_complete($uid, $mid, $program_id, $updateuser, '', '','',$tpscore,$statements);
				}
			}else {
					
				$tpstatus = ($contentstatus == 'lrn_crs_cmp_cmp') ? 'lrn_tpm_ovr_cmp' : 'lrn_tpm_ovr_inc';

				$selectStmt = db_select('slt_master_enrollment', 'menr');
				//$selectStmt->addExpression('CONCAT(overall_status,'-','.$calltype.')','call_type');
				$selectStmt->addField('menr','overall_status','overall_status');
				$selectStmt->addField('menr','overall_progress','overall_progress');
				$selectStmt->addField('menr','comp_date','comp_date');
				$selectStmt->addField('menr','score','score');
				$selectStmt->addField('menr','content_status','content_status');
				$selectStmt->addField('menr','updated_on','updated_on');
				$selectStmt->addField('menr','updated_by','updated_by');
				$selectStmt->condition('menr.id',$master_enroll_id,'=');
				expDebug::dPrintDBAPI('get previous content status',$selectStmt);
				$ovr_sts = $selectStmt->execute()->fetchField();
					$updprogress =  $ovr_sts[0]->overall_progress;
					if($tpstatus == 'lrn_tpm_ovr_cmp' && $calltype == "'assesmentTP'"  && $content_type ==  'sry_det_typ_ass'){
						$updprogress = 100;
					}
				$calltypeforassess= $calltype;
				$calltype = $ovr_sts.'-'.$calltype;
				expDebug::dPrint('get call type value inside update' .print_r($calltype,true), 4);
					
				$tpstatus = ($ovr_sts[0]->overall_status == 'lrn_tpm_ovr_cmp') ? $ovr_sts[0]->overall_status:$tpstatus;
				$updateStmt = db_update('slt_master_enrollment');
				// Set conditions
				$updateStmt->condition('id', $master_enroll_id,'=');
				$updateStmt->condition('overall_status', array('lrn_tpm_ovr_enr','lrn_tpm_ovr_inp','lrn_tpm_ovr_cmp'),'IN');
				// Set new values of fields
				$updateStmt->fields(array(
						'overall_status' => $tpstatus,
						'comp_date' => now(),
						'score' => $tpscore,
				    	'overall_progress' => $updprogress,
						'content_status' => $master_enroll_content_status,
						'updated_on' => now(),
						'updated_by' => $updateuser
				));
				expDebug::dPrintDBAPI(' $updateStmt slt_master_enrollment = ' , $updateStmt);
				// Execute query
				//may be hereee
				 $num_updated = $updateStmt->execute();
				
				//Added for reverting the update when db_execution fails
				$result= new stdClass();
				$result->col_name = 'id';
				$result ->col_value = $master_enroll_id;
				$result ->table_name ='slt_master_enrollment';
				$result ->fields =array(
						'overall_status' => $ovr_sts[0]->overall_status,
						'comp_date' => $ovr_sts[0]->comp_date,
						'score' => $ovr_sts[0]->score,
				    	'overall_progress' => $ovr_sts[0]->overall_progress,
						'content_status' => $ovr_sts[0]->content_status,
						'updated_on' => $ovr_sts[0]->updated_on,
						'updated_by' => $ovr_sts[0]->updated_by
				);
				$statements['update'][] = $result; 
				 /* if(!empty($statements)) {
					$statements[] = $updateStmt;
				} else {
					$num_updated = $updateStmt->execute();
				}  */
				expDebug::dPrint(' $num_updated = ' . print_r($num_updated, true) , 3);
				/* //get program id from slt_master_enrollment
				$selectStmt = db_select('slt_master_enrollment', 'menr');
				$selectStmt->addField('menr','program_id','program_id');
				$selectStmt->condition('menr.id',$master_enroll_id,'=');
				expDebug::dPrintDBAPI('get program id from slt_master_enrollment',$selectStmt);
				$prg_id = $selectStmt->execute()->fetchField();
				expDebug::dPrint('program id >>> ' .print_r($prg_id,true), 4); */
					
				if($tpstatus == 'lrn_tpm_ovr_inc'){
					//update slt_survey_results
					$selectStmt = db_select('slt_survey_mapping', 'surmap');
					$selectStmt->innerJoin('slt_survey','sur','sur.id = surmap.survey_id');
					$selectStmt->addExpression('GROUP_CONCAT(distinct surmap.survey_id)','surveyids');
					$selectStmt->condition('surmap.object_id',$prg_id,'=');
					$selectStmt->condition('surmap.object_type','cre_sys_obt_cls','!=');
					$selectStmt->condition('sur.type',array('sry_det_typ_ass','sry_det_typ_sry'),'IN');
					expDebug::dPrintDBAPI("update slt_survey_results ",$selectStmt);
					$results = $selectStmt->execute()->fetchField();
					$survey_ids = explode(',',$results);

					//update slt_survey results
					$updateStmt = db_update('slt_survey_results');
					$updateStmt->fields(array('re_attend' => 1));
					$updateStmt->condition('entity_id',$prg_id,'=');
					$updateStmt->condition('user_id', $uid,'=');
					$updateStmt->condition('survey_id', $survey_ids, 'IN');
					expDebug::dPrintDBAPI(' update slt_survey_results = ' , $updateStmt);
					//$survey_results = $updateStmt->execute();
					if(!empty($statements)) {
						$statements[] = $updateStmt;
					} else {
						$survey_results = $updateStmt->execute();
					}
				}
				lp_mark_program_complete($uid, $master_enroll_id, $prg_id, $updateuser ,$calltype ,'','','',$statements); // check the roster program with slp_mark_program_complete
				//slp_mark_program_complete(xuid, xmaster_enrid, xprogram_id, xupdateuser, xcalltype, '');
			}
				
		}else{
			if($calltype == "'assesmentTP'" && !empty($pstatus) ){
				$tpprestatus = ($pstatus == 'lrn_crs_cmp_cmp') ? 'lrn_tpm_ovr_cmp' : 'lrn_tpm_ovr_inc';
				$tpprescore = $score;
				$updateStmt = db_update('slt_master_enrollment');
				// Set conditions
				$updateStmt->condition('id', $master_enroll_id,'=');
				$updateStmt->condition('overall_status', array('lrn_tpm_ovr_enr','lrn_tpm_ovr_inp'),'IN');
				// Set new values of fields
				$updateStmt->fields(array(
						'pre_status' => $tpprestatus,
						'pre_score' => $tpprescore,
						'score' => $tpscore,
						'content_status' => $master_enroll_content_status,
						'updated_by' => $updateuser,
						'updated_on' => now()
				));
				expDebug::dPrintDBAPI(' $updateStmt slt_master_enrollment = ' , $updateStmt);
				// Execute query
				//$num_updated = $updateStmt->execute();
				if(!empty($statements)) {
					$statements[] = $updateStmt;
				} else {
					$num_updated = $updateStmt->execute();
				}
				expDebug::dPrint(' $num_updated = ' . print_r($num_updated, true) , 3);
			}
				// Update TP Overall Progress
				if (!empty($master_enroll_id)) {
					$program_id = getProgramIdFromEnrollment($master_enroll_id);
					// To update  a overall percentage complete used in content player - new column
					updateTPEnrollmentOverallPercentComplete($program_id, $master_enroll_id,'','',$statements);
				}
			}
		if($calltype == "'assesmentTP'"  || $calltypeforassess== "'assesmentTP'"){
			$program_id = getProgramIdFromEnrollment($master_enroll_id);
			updateAssSurTotAttempt($regid,$surveyid,$uid,$class_id,$course_id,$lesson_id,$version_id,$content_type,$program_id,$master_enroll_id,$statements);
		}
		else if($content_type ==  "sry_det_typ_ass" || $content_type == 'sry_det_typ_sry'){
			updateAssSurTotAttempt($regid,$surveyid,$uid,$class_id,$course_id,$lesson_id,$version_id,$content_type,0,0,$statements);
		}
		$selectStmt = db_select('slt_enrollment', 'enr');
		$selectStmt->addField('enr','comp_status','comp_status');
		$selectStmt->condition('enr.id',$regid,'=');
		$compstaft = $selectStmt->execute()->fetchField();
			
		$updsts = ($compstbef == $compstaft) ? "Not Changed" : getProfileListItemName($contentstatus);
		$result = new stdClass();
		$result->Id = $summ_upd;
		$result->CompStatus = $updsts;
		return $result;
	} catch(Exception $e){
		expDebug::dPrint('Error in lp_attendance_summary_upd '.$e,1);
		expertusLogError($e);
	}
}


/**
 * Calculates attempts left and total attempts for content
 */

function CalculateTotalAttempts($classid){
	
	$query1 = db_query("select `slf_get_class_content_postass_total_attempts`($classid) as total_attempts");
	$rosUpdated1 = $query1->fetchAll();
	expDebug::dPrintDBAPI(' UpdateQuery222 = ' , $query1,4);
	$total_attempts = $rosUpdated1[0]->total_attempts;
	return $total_attempts;
	
}

function CalculateAttemptsTaken($regid){
$query2 = db_query("select `slf_get_class_attempts_taken`($regid) as attempts_left");
$rosUpdated2 = $query2->fetchAll();
expDebug::dPrintDBAPI(' UpdateQuery123 = ' , $query2,4);
$attempts_left = $rosUpdated2[0]->attempts_left;
return $attempts_left;
}

function CalculateTotalAttemptsLeft($classid,$regid){
	$query1 = db_query("select `slf_get_class_content_postass_total_attempts`($classid) as total_attempts");
	$rosUpdated1 = $query1->fetchAll();
	expDebug::dPrintDBAPI(' UpdateQuery222 = ' , $query1,4);
	$total_attempts = $rosUpdated1[0]->total_attempts;
	
	
	$query2 = db_query("select `slf_get_class_attempts_taken`($regid) as attempts_left");
	$rosUpdated2 = $query2->fetchAll();
	expDebug::dPrintDBAPI(' UpdateQuery123 = ' , $query2,4);
	$attempts_left = $rosUpdated2[0]->attempts_left;
	
	$totalAttemptsLeft = ($total_attempts - $attempts_left);
	return $totalAttemptsLeft;
}


function CalculateTotalAttemptsLeftTp($programId,$masterEnrollmentid){
	
	expDebug::dPrint("enrollmentidenrollmentidenrollmentid : ".print_r($masterEnrollmentid,true),5);
	
	$query1 = db_query("select `slf_get_tp_content_postass_total_attempts`($programId,$masterEnrollmentid) as total_attempts");
	$rosUpdated1 = $query1->fetchAll();
	expDebug::dPrintDBAPI(' UpdateQuery222 = ' , $query1,4);
	$total_attempts = $rosUpdated1[0]->total_attempts;
	
	
	$query2 = db_query("select `slf_get_attempts_left`($masterEnrollmentid) as attempts_left");
	$rosUpdated2 = $query2->fetchAll();
	expDebug::dPrintDBAPI(' UpdateQuery123 = ' , $query2,4);
	$attempts_left = $rosUpdated2[0]->attempts_left;
	
	$totalAttemptsLefttp = ($total_attempts - $attempts_left);
	return $totalAttemptsLefttp;
	
}

/**
 * Calculates percentage complete of a tp enrollment and saves in slt_master_enrollment
 */
/* function updateTPEnrollmentPercentComplete($programId, $masterEnrollId, $recertFlag) {
	try {
    $select1 = db_select('slt_enrollment', 'enr');
    if ($recertFlag > 0) {
      $select1->join('slt_re_certify', 'crsmap', 'enr.course_id = crsmap.course_id AND crsmap.program_id = :prgmid',
                                                                                                     array(':prgmid' => $programId));
    }
    else {
      $select1->join('slt_module_crs_mapping', 'crsmap', 'enr.course_id = crsmap.course_id AND crsmap.program_id = :prgmid',
                                                                                                     array(':prgmid' => $programId));
    }
    $select1->addExpression("COUNT(CASE WHEN enr.comp_status = 'lrn_crs_cmp_cmp' AND crsmap.is_required = 'Y' THEN enr.id END)",
                                                  'mandatory_completed_courses_count');
    $select1->addExpression("COUNT(CASE WHEN enr.comp_status = 'lrn_crs_cmp_cmp' AND crsmap.is_required != 'Y' THEN enr.id END)",
                                                  'optional_completed_courses_count');
    $select1->addExpression("COUNT(CASE WHEN crsmap.is_required = 'Y' THEN enr.id END)", 'mandatory_courses_count');
    $select1->addExpression("COUNT(enr.id)", 'total_courses_count');
    $select1->condition('enr.master_enrollment_id', $masterEnrollId, '=');
    $select1->condition('enr.reg_status', array('lrn_crs_reg_rsv', 'lrn_crs_reg_rsc'), 'NOT IN');
    expDebug::dPrintDBAPI('$select1 to get completion counts', $select1);
   
    $select2 = db_select($select1, 'completion_data');
    $pcExpr = <<<EOD
      (CASE WHEN (total_courses_count = 0) THEN
         NULL
       ELSE
         ROUND(CASE WHEN (mandatory_courses_count = 0) THEN
                 optional_completed_courses_count / total_courses_count
               ELSE
                 mandatory_completed_courses_count / mandatory_courses_count
               END * 100, 0)
      END)
EOD;
     
    $select2->addExpression($pcExpr, 'percentage_completed');
    expDebug::dPrintDBAPI('$select2 for finding percentage completed', $select2);
    $percentageCompleted = $select2->execute()->fetchField();
    expDebug::dPrint('$percentageCompleted = ' . $percentageCompleted,3);
      
    $updStmt = db_update('slt_master_enrollment');
    $updStmt->fields(array('percentage_complete' => empty($percentageCompleted)? null : $percentageCompleted));
    $updStmt->condition('id', $masterEnrollId, '=');
    expDebug::dPrintDBAPI('$updStmt for updating percentage complete in master enrollment table', $updStmt);
    $updStmt->execute();
  }
  catch (Exception $ex) {
    watchdog_exception('updateTPEnrollmentPercentComplete', $ex);
    expertusErrorThrow($ex);
  }   	
} */

function getMasterEnrollCompletionDate($masterEnrollId){
  try {
    $select = db_select('slt_enrollment', 'enroll');
    $select->addExpression('MAX(enroll.comp_date)', 'comp_date');
    $select->condition('enroll.master_enrollment_id', $masterEnrollId,'=');
    $select->condition('enroll.comp_status', array('lrn_crs_cmp_enr','lrn_crs_cmp_inp','lrn_crs_cmp_att'),'NOT IN');
    $result = $select->execute()->fetchAssoc();
    return $result['comp_date'];
  } catch (Exception $ex) {
    watchdog_exception('catalogRoster', $ex);
    expertusErrorThrow($ex);
  }     
}

/*
 * PHP function for sql procedure slf_get_tp_content_status
*/
function getTpContentStatus($menrid){
	try{
		if(empty($menrid))
			$enr_quiz_status = "";
		
		//get Content launch count
		$selectStmt = db_select('slt_attendance_summary', 'summ');
		$selectStmt->leftjoin('slt_enrollment', 'enr','enr.id = summ.enrollment_id');
		$selectStmt->addExpression('COUNT(summ.id)','launch_count');
		$selectStmt->condition('enr.master_enrollment_id',$menrid,'=');
		expDebug::dPrintDBAPI('get Content launch count',$selectStmt);
		$launch_count = $selectStmt->execute()->fetchField();
		expDebug::dPrint('getContentStatus result'.print_r($launch_count,true), 4);
		
		//get total lesson count
		$selectStmt = db_select('slt_content_version', 'scv');
		$selectStmt->leftjoin('slt_enrollment_content_mapping', 'secm','secm.content_id = scv.content_master_id AND secm.version_id = scv.id');
		$selectStmt->leftjoin('slt_enrollment', 'enr','enr.id = secm.enroll_id');
		$selectStmt->addExpression('sum(scv.total_lesson)','lesson_count');
		$selectStmt->condition('enr.master_enrollment_id',$menrid,'=');
		expDebug::dPrintDBAPI('get total lesson count',$selectStmt);
		$lesson_count = $selectStmt->execute()->fetchField();
		expDebug::dPrint('get total lesson count result'.print_r($lesson_count,true), 4);
		
		if($launch_count != $lesson_count)
			$enr_quiz_status = "";
		else{
			//get content and completion status
			$selectStmt = db_select('slt_attendance_summary', 'summ');
			$selectStmt->leftjoin('slt_enrollment', 'enr','enr.id = summ.enrollment_id');
			$selectStmt->addField('summ','content_status');
			$selectStmt->addField('summ','completion_status');
			$selectStmt->condition('enr.master_enrollment_id',$menrid,'=');
			expDebug::dPrintDBAPI('get previous content status',$selectStmt);
			$result = $selectStmt->execute()->fetchAll();
			expDebug::dPrint('getContentStatus result'.print_r($launch_count,true), 4);
			foreach($result as $var){
				if($var->content_status == "failed" || $var->content_status == 'incomplete' || empty($var->content_status)){
					$enr_quiz_status = $var->content_status;
					break;
				}else if($var->completion_status == 'completed' || empty($var->completion_status)){
				 	$enr_quiz_status = "passed";
				}else{
					$enr_quiz_status = "";
				}
			}
		}
		return $enr_quiz_status;
	} catch(Exception $e){
		expDebug::dPrint('Error in getTpContentStatus '.$e,1);
		expertusLogError($e);
	}
}
/*
 * return module id for a object id 
*/
function getModuleIdFromObjectId($objectId,$recertifylvl='',$moduleId='',$start=-1,$rows='',$type=''){
	try{
	  $selectStmt = db_select('slt_module', 'mdl');
	  $selectStmt->addField('mdl','id','module_id');
	  $selectStmt->addField('mdl','sequence','sequence');
	  $selectStmt->condition('mdl.program_id',$objectId,'=');
	  if(!empty($recertifylvl)){
	 	 $selectStmt->condition('mdl.sequence', $recertifylvl,'=');
	  }//else{
	  	 $selectStmt->addField('mdl','title','module_title');
	  //}
	  if(!empty($moduleId)){
	  	$selectStmt->condition('mdl.id',$moduleId,'=');
	  }
	  $selectStmt->orderBy('mdl.id', 'ASC');
	  if($start != -1){
	  	$selectStmt->range($start,$rows);
	  }
	  expDebug::dPrintDBAPI('getModuleIdFromObjectId',$selectStmt);
	  if($type == 'count'){
	  	$result = $selectStmt->countQuery()->execute()->fetchField();
	  }else{
	  	$result = $selectStmt->execute()->fetchAll();
	  }
	  expDebug::dPrint('getModuleIdFromObjectId result'.print_r($result,true), 4);
	  return $result;
	}catch(Exception $e){
		expDebug::dPrint('Error in getModuleIdFromObjectId '.$e,1);
		expertusLogError($e);
	}
}

/*
 * PHP function for Enroll register to program procedure slp_program_enrollment_ins
*/

/* function lp_program_enrollment_ins($user_id,$program_id,$class_id,$payment_flag,$crtduser_id,$reg_date,$recertifyValue,$cert_days_expire,
									$man,$usercurrencycode){
	try{
		include_once(drupal_get_path('module', 'exp_sp_administration_catalog') .'/exp_sp_administration_catalog_roster.inc');
		include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_notification_insert.inc');
		$overall_status = 'lrn_tpm_ovr_enr';
		$recertifyFlag = 0;
		$mst_id = 0;
		$cnt_id = 0; 
		
		//Fetch enrollment details 
		$select = db_select('slt_master_enrollment', 'mst');
		$select->addExpression('COUNT(1)','cnt_id');
		$select->addField('mst', 'id','mst_id');
		$select->addField('mst', 'recertify_path','recertifyFlag');
		$select->addField('mst', 'custom4','custom4');
		$select->condition('mst.program_id',$program_id,'=');
		$select->condition('mst.user_id', $user_id , '=');
		$select->condition('mst.is_current', 'Y','=');
		$select->condition('mst.overall_status', array('lrn_tpm_ovr_cmp','lrn_tpm_ovr_exp'),'IN');
		expDebug::dPrintDBAPI('getUserEnrollmentClassCmpStatus = ', $select);
		$result = $select->execute()->fetchAll();
		expDebug::dPrint("master enrollment result details ".print_r($result,true),5);
		$objecttype = getTrainingProgramDetails($program_id, array('object_type')); // $objecttype['object_type']
		expDebug::DPrint(" object type ".print_r($objecttype['object_type'],true),5);
		$custom4 = $result[0]->custom4;
		$recertifyFlag = $result[0]->recertifyflag;
		if($result[0]->cnt_id > 0){
			if($custom4 == 'lrn_tpm_ovr_cmp'){ 
				$recertifyFlag = $recertifyFlag + 1;
			}
		}else{
			$recertifyFlag = 1;
		}
		if($reg_date == null || $reg_date == 'null' || $reg_date == '')
			$reg_date = now();
		
		if($payment_flag == 'yes') 
			$overall_status = 'lrn_tpm_ovr_ppm';
		elseif($payment_flag == 'no' || $reg_date == null || $reg_date == 'null')
			 $overall_status = 'lrn_tpm_ovr_enr';
		
		$return_status = '';
		
		$wait_list = lp_program_waitlist_check($user_id,$program_id);
		$already_enroll = check_user_enrolled_program($user_id,$program_id,$cert_days_expire);
		if($wait_list >0)
			$return_status = 'WL';
		else if($already_enroll)
			$return_status = 'EP';
		else{
			$insertStmt = db_insert('slt_master_enrollment');
			$fields = array(
				'program_id'    => $program_id,
				'user_id'  => $user_id,
				'overall_status'  => $overall_status,
				'reg_date'  => $reg_date,
				'recertify_path'  => $recertifyFlag,
				'cancel_date'  => NULL,
				'comp_date'  => NULL,
				'percentage_complete' => 0,
				'mandatory' => $man,
				'created_by' => $crtduser_id,
				'created_on' => now(),
				'updated_by' => $crtduser_id,
				'updated_on' => now(),
				'custom0' => NULL,
				'custom1' => NULL,
				'custom2' => NULL,
				'custom3' => NULL,
				'custom4' => NULL,			
			);
			$insertStmt->fields($fields);
			expDebug::dPrintDBAPI("Insert TP ",$insertStmt);
			$menrId = $insertStmt->execute();
			
			// Update status from expired to complete
			if($result[0]->mst_id != '' && $result[0]->mst_id != null){
				
				// enrollment update
				$updateMaster = db_update('slt_master_enrollment');
		        $updateMasterFields['overall_status']=  $custom4;
		        $updateMasterFields['is_current'] = 'N';
		        $updateMaster->fields($updateMasterFields);
		        $updateMaster->condition('id', $result[0]->mst_id);
		        $updateMaster->condition('user_id', $user_id);
		        $updateMaster->condition('program_id', $program_id);
		        expDebug::dPrintDBAPI("Update TP custom4 ",$updateMaster);
	        	$UpdateId = $updateMaster->execute();
	        	
	        	// skill set update
	        	$updateskill = db_update('slt_skill_set');
		        $updateskillFields['is_deleted'] = '1';
		        $updateskill->fields($updateskillFields);
		        $updateskill->condition('id', $result[0]->mst_id);
		        $updateskill->condition('program_id', $program_id);
		        expDebug::dPrintDBAPI("Update TP custom4 ",$updateMaster);
	        	$Updateskill = $updateskill->execute();
        	}
			lp_prg_crs_enrollment_ins($user_id,$menrId,'',$class_id,$program_id,$objecttype['object_type'],$payment_flag,$crtduser_id,'','','','','',$reg_date,$recertifyValue,$usercurrencycode);
			if($overall_status == 'lrn_tpm_ovr_ppv'){
				$return_status = 'PS';
			}elseif(lp_program_waitlist_check($user_id,$program_id)){
				$return_status = 'WL';
			}else{
				$return_status = 'RG';
			}
			if($menrId !=0)
				lp_mark_program_complete($user_id, $menrId, $program_id, $crtduser_id);
		}
		$result = new stdClass();
		$result->status = $return_status;
		$result->masterEnroll = $menrId;
		expDebug::dPrint("Final return status from  lp_program_enrollment_ins " .print_r($result,true),5);
		return $result;
	} catch(Exception $e){
		expDebug::dPrint('Error in lp_program_enrollment_ins'.$e,1);
		expertusLogError($e);
	}									
} */

/*
 * PHP function for sql procedure slp_program_crsenrollment_ins
*/
/* function lp_prg_crs_enrollment_ins($user_id,$menrId,$course_id = '',$class_id,$program_id,$objecttype,$payment_flag,
								$crtduser_id,$orderitem_custom0 = NULL,$orderitem_custom1 = NULL,$orderitem_custom2 = NULL,$orderitem_custom3 = NULL,$orderitem_custom4 = NULL,$reg_date,$recertifyValue,$usercurrencycode){
  try{
  	
  	$module_id = getModuleIdFromObjectId($program_id,$recertifyValue);
  	expDebug::dPrint("lp module_id ::".print_r($module_id,1),5);
  	
	$reg_status = NULL;
	$comp_status = NULL;
	$order_status = 'lrn_crs_reg_cnf';
	$class_ids = explode(',',$class_id);
	$enrollment_count = 0;
	if(strtolower($payment_flag) == 'yes'){
	 $reg_status = 'lrn_crs_reg_ppm';
	 $comp_status = NULL;
	 $order_status = 'lrn_crs_reg_ppm';
	}else if(strtolower($payment_flag) == 'no' || strtolower($payment_flag) == 'null' || $payment_flag == NULL){
	  $reg_status = 'lrn_crs_reg_cnf';
	  $comp_status = 'lrn_crs_cmp_enr';
	  $order_status = 'lrn_crs_reg_cnf';
	}
	expDebug::dPrint(' $reg_status '. $reg_status .' $comp_status '.$comp_status .' $order_status '. $order_status , 5);
	//Order insert
	$insertStmt = db_insert('slt_order');
    $insertStmt->fields(array(
      'user_id' => $user_id,
      'order_status' => $order_status,
      'created_by' => $crtduser_id,
      'created_on' => now(),
      'updated_by' => $crtduser_id,
      'updated_on' => now()
    ));
    expDebug::dPrintDBAPI("slt_order insert ",$insertStmt);
    $orderId = $insertStmt->execute();
	
    expDebug::dPrint("program object type ".$objecttype,4);
	
	if($objecttype == 'cre_sys_obt_crt' || $objecttype ==  'cre_sys_obt_cur' || $objecttype == 'cre_sys_obt_trn'){
	
		//if($recertifyValue != 'R')
		$selectStmt = db_select('slt_module_crs_mapping', 'map');
		//else
		//	$selectStmt = db_select('slt_re_certify', 'map');
	
		$selectStmt->innerJoin('slt_course_template','crs','crs.id = map.course_id and crs.status = \'lrn_crs_sts_atv\'');
		$selectStmt->addField('crs','id','id');
		$selectStmt->condition('map.program_id',$program_id);
		$selectStmt->condition('map.module_id',$module_id[0]->module_id);
		expDebug::dPrintDBAPI("select list of courses ",$selectStmt);
		$crs_ids = $selectStmt->execute()->fetchCol();
		expDebug::dPrint("result list of courses ".print_r($crs_ids,true),5);
		foreach($crs_ids as $cid){
		
			//Enrollment count
			$selectStmt = db_select('slt_enrollment', 'enr');
			$selectStmt->addExpression('COUNT(1)','enr_cnt');
			$selectStmt->condition('enr.course_id',$cid);
			$selectStmt->condition('enr.user_id', $user_id);
			$selectStmt->condition('enr.reg_status',array('lrn_crs_reg_cnf', 'lrn_crs_reg_ppm', 'lrn_crs_reg_rsv'),'IN');
			$selectStmt->where('IF(enr.reg_status = \'lrn_crs_reg_cnf\', enr.comp_status IN(\'lrn_crs_cmp_cmp\',\'lrn_crs_cmp_inp\',\'lrn_crs_cmp_enr\',\'lrn_crs_cmp_att\'),1=1)');
			expDebug::dPrintDBAPI("select enrollment count ",$selectStmt);
			$enrollment_count = $selectStmt->execute()->fetchField();
			expDebug::dPrint("enrollment count ".$enrollment_count,4);
			
			$select = db_select('slt_course_class', 'cls');
			$select->addField('cls','id','classid');
			$select->addField('cls','delivery_type','deliverytype');
			$select->condition('cls.course_id',$cid);
			$select->condition('cls.id',$class_ids,'IN');
			//$select->where('IF(('.$enrollment_count.' = 0) , STATUS = "lrn_cls_sts_atv",1=1)');
			$select->condition('cls.status','lrn_cls_sts_atv','=');
			$select->range(0,1);
			expDebug::dPrintDBAPI("get delivery type of class  ",$select);
			$results = $select->execute()->fetchAll();
			expDebug::dPrint("final result set ".print_r($results,true),5);
			if($results[0]->deliverytype == 'lrn_cls_dty_ilt' || $results[0]->deliverytype == 'lrn_cls_dty_vcl'){
				//User enrolled for particular class
				$selectStmt = db_select('slt_enrollment', 'enr');
				$selectStmt->addExpression('COUNT(1)','enr_cnt');
				$selectStmt->condition('enr.course_id',$cid);
				$selectStmt->condition('enr.user_id', $user_id);
				$selectStmt->condition('enr.class_id', $results[0]->classid);
				$selectStmt->condition('enr.reg_status',array('lrn_crs_reg_cnf','lrn_crs_reg_ppm', 'lrn_crs_reg_rsv'),'IN');
				expDebug::dPrintDBAPI("User enrolled for particular class  ",$selectStmt);
				$usrEnrcheck = $selectStmt->execute()->fetchField();
				
				//waitlist check 
				$wait_count = lp_waitlist_check_class($cid,$results[0]->classid);
				expDebug::dPrint(' $wait_count '.$wait_count,5);
				if($wait_count > 0 && $usrEnrcheck == 0){
					$reg_status = 'lrn_crs_reg_wtl';
			  		$comp_status = NULL;
			  		$waitlist_flag = 'lrn_crs_reg_wtl';
		  	  		$waitlist_priority = $wait_count;
				}else{
					$reg_status = 'lrn_crs_reg_cnf';
			   		$comp_status = 'lrn_crs_cmp_enr';
				    $order_status = 'lrn_crs_reg_cnf';
				    $waitlist_flag = NULL;
			  	    $waitlist_priority = NULL;
				}
			}else{
					$reg_status = 'lrn_crs_reg_cnf';
				  	$comp_status = 'lrn_crs_cmp_enr';
				  	$order_status = 'lrn_crs_reg_cnf';
				  	$waitlist_flag = NULL;
			  	  	$waitlist_priority = NULL;
			} 			
			//course enrollment insert
			lp_course_enrollment_ins($user_id,$menrId,$cid,$results[0]->classid,$orderId,$reg_status,$comp_status,$waitlist_flag,$waitlist_priority,$crtduser_id,$reg_date,$recertifyValue);
			expDebug::DPrint("curse enrollment completed ");
			insertOrderItem($orderId, $cid, $results[0]->classid,$user_id,$program_id,$objecttype,$crtduser_id,$orderitem_custom0,$orderitem_custom1,$orderitem_custom2,$orderitem_custom3,$orderitem_custom4);
		}	
	}
	if($waitlist_flag == 'lrn_crs_reg_wtl'){
	$updateStmt = db_update('slt_master_enrollment');
    $updateStmt->condition('id',$menrId);
	    $updateStmt->fields(array('overall_status' => 'lrn_tpm_ovr_wtl'));
	    expDebug::dPrintDBAPI(' update slt_enrollment if class is already waitlisted = ' , $updateStmt);
    $master_enroll_upd = $updateStmt->execute();
	}
  } catch(Exception $e){
		expDebug::dPrint('Error in lp_course_enrollment_ins '.$e,1);
		expertusLogError($e);
	}
}
/*
 * PHP function for sql procedure slp_attendance_details_ins
*/
function lp_attendance_details_ins($regid,$uid,$course_id,$class_id,$lesson_id,$version_id,$status,$pstatus,$score,
									$lessonloc,$content_status,$completion_status,$updateuser,$launch_data,$suspend_data,$cmiexit,
									$launchflag,$aicc_data,$calltype,$assesscheck,$surveyid,$lrs_enabled,$time_spend,$content_type,$progress,$subtype='',&$statements=array()){
	try{	
		expDebug::dPrint('func get args --->det_ins detai;ss'.var_export(func_get_args(), 1),5);
		$allowupdate=0;
		$launchflag = (int) $launchflag;
		$surveyid = (empty($surveyid) || ($surveyid==null) || ($surveyid=='null')) ? 0 : $surveyid;
		expDebug::dPrint('SurMan SurveyID Value: ' . $surveyid, 4);
		//get attendId and custom value
		$selectStmt = db_select('slt_attendance_details', 'det');
		$selectStmt->addExpression('COUNT(1)','att_cnt');
		$selectStmt->addField('det','id','id');
		$selectStmt->condition('det.user_id',$uid,'=');
		$selectStmt->condition('det.course_id', $course_id,'=');
		$selectStmt->condition('det.class_id',$class_id,'=');
		$selectStmt->condition('det.content_version_id',$version_id,'=');
		$selectStmt->condition('det.lesson_id',$lesson_id,'=');
		$selectStmt->condition('det.enrollment_id',$regid,'=');
		$selectStmt->condition('det.custom2',0,'!=');
	  	expDebug::dPrintDBAPI("launch flag count ",$selectStmt);
		$att_res = $selectStmt->execute()->fetchAll();
		$attendId = $att_res[0]->id; 
		$custom = $att_res[0]->att_cnt; 
		expDebug::dPrint('SurMan $content_status Value: ' . $content_status, 4);
		expDebug::dPrint('$custom Value: ' . $custom, 4);
		if(strtolower($content_status) != 'not attempted'){
			if($surveyid == 0){
				$attempts = getAttemptNumber($uid,$course_id,$class_id,$lesson_id,$version_id,$regid);		
				//fetch prev enroll status
				$selectStmt = db_select('slt_enrollment', 'enr');
				$selectStmt->addField('enr','comp_status');
				$selectStmt->condition('enr.user_id', $uid,'=');
				$selectStmt->condition('enr.course_id', $course_id,'=');
				$selectStmt->condition('enr.class_id',$class_id,'=');
				$selectStmt->condition('enr.id',$regid,'=');
				expDebug::dPrintDBAPI("Completed class cannot be enrolled ",$selectStmt);
				$enrollsts = $selectStmt->execute()->fetchField();	
				if($enrollsts == 'lrn_crs_cmp_cmp')
					$status = $enrollsts;
			}else{
				$attempts = getAttemptNumber($uid,$course_id,$class_id,$lesson_id,$version_id,$regid,$surveyid);
			}
		}else{
		 	$attempts = 0;
		}
		if($attempts == '' || $attempts == 'null') $attempts = 1; 
		
		expDebug::dPrint('SurMan $attempts Value: ' . $attempts, 4);
		
		// Based on a launchFlag value Insert / Update attendance details
		if($lrs_enabled == 'N'){
			if($custom == 0){
				expDebug::dPrint("lp_attendance_details_ins insert part", 4);
				//slt_attendance_details insert
				$insertStmt = db_insert('slt_attendance_details');
				$fields = array(
					'enrollment_id'    => $regid,
					'user_id'  => $uid,
					'course_id'  => $course_id,
					'class_id'  => $class_id,
					'content_version_id'  => $version_id,
					'lesson_id'  => $lesson_id,
					'assessment_id'  => $surveyid,
					'content_type' => $content_type,
					'status' => $status,
					'attempt_number' => $attempts,
					'attempt_date' => now(),
					'time_spend' => $time_spend,
					'score' => $score,
					'launch_data' => $launch_data,
					'suspend_data' => $suspend_data,
					'cmiexit' => $cmiexit,
					'aicc_data' => $aicc_data,
					'content_status' => $content_status,
					'completion_status' => $completion_status,
					'created_by' => $updateuser,
					'created_on' => now(),
					'updated_by' => $updateuser,
					'updated_on' => now(),
					'custom2' => $launchflag
				);
				$insertStmt->fields($fields);
				expDebug::dPrintDBAPI("Insert slt_attendance_details ",$insertStmt);
				//$attend_Id = $insertStmt->execute();
				if(!empty($statements)) {
					$statements[] = $insertStmt;
				} else {
					$attend_Id = $insertStmt->execute();
				}
				expDebug::dPrint("attendance details insert id ". $attend_Id ,4);
			}else{
				expDebug::dPrint("lp_attendance_details_ins update part", 4);
				$updateStmt = db_update('slt_attendance_details');
				$fields = array(
					'status'    => $status,
					'time_spend'  => $time_spend,
					'score'  => $score,
					'launch_data'  => $launch_data,
					'suspend_data'  => $suspend_data,
					'cmiexit'  => $cmiexit,
					'aicc_data'  => $aicc_data,
					'content_status' => $content_status,
					'completion_status' => $completion_status,
					'custom2' => $launchflag,
					'updated_on' => now()
				);
			    $updateStmt->fields($fields);
				$updateStmt->condition('id',$attendId,'=');
				$updateStmt->condition('enrollment_id', $regid,'=');
				$updateStmt->condition('user_id', $uid,'=');
				$updateStmt->condition('course_id', $course_id,'=');
				$updateStmt->condition('class_id', $class_id,'=');
				$updateStmt->condition('content_version_id', $version_id,'=');
				$updateStmt->condition('lesson_id', $lesson_id,'=');
			    expDebug::dPrintDBAPI(' update slt_attendance_details total attempts with survey id = ' , $updateStmt);
			    //$details_upd = $updateStmt->execute();
			    
			    if(!empty($statements)) {
			    	$statements[] = $updateStmt;
			    } else {
			    	$details_upd = $updateStmt->execute();
			    }
			}
		}
		
		// Collect summary detaits from tables
		if($surveyid == 0 ){ // for content
			$summary_det = getAttendanceSummaryDetails($uid,$course_id,$class_id,$lesson_id,$version_id,$regid);
		}else{ // for survey
			$summary_det =  getAttendanceSummaryDetails($uid,$course_id,$class_id,$lesson_id,$version_id,$regid,$surveyid);
		}
		
		expDebug::dPrint("Summary details >>> ". print_r($summary_det,true), 4);
		
		if(($summary_det[0]->contentstatus == 'lrn_crs_cmp_cmp' && $status != 'lrn_crs_cmp_cmp') || ($status == 'lrn_crs_cmp_cmp' && $score < $summary_det->contentscore) 
			   || (!empty($summary_det[0]->contentscore) && $summary_det[0]->contentscore != 0 && is_null($score))){
			   	$allowupdate=1;			   	
			   	expDebug::dPrint("attendance New progress value >>> ".print_r($progressUpdate,1),4);
			   	$prev_progress = $summary_det[0]->progress;
			   		if($progress >$prev_progress)
			   			$progressUpdate = $progress;
			   		else
			   			$progressUpdate = $prev_progress;
		   		//73059: Content completion % is not getting updated unless we close the window and reopen it or move to the next content
			   	if(($content_type == 'sry_det_typ_sry'  || $content_type == 'sry_det_typ_ass') && $launchflag == 1 && $summary_det[0]->pcontentsts=='suspend')
			   		$progressUpdate = $progress;
				//SELECT comp_status INTO xstatus  FROM slt_enrollment WHERE id= xregid AND class_id=xclass_id AND user_id=xuid;
				//SET @comstat = xstatus;
				$selectStmt = db_select('slt_enrollment', 'enr');
				$selectStmt->addField('enr','comp_status','comp_status');
			$selectStmt->condition('enr.id',$regid,'=');
			$selectStmt->condition('enr.class_id', $class_id,'=');
			$selectStmt->condition('enr.user_id', $uid,'=');
				$status = $selectStmt->execute()->fetchField();
				// expDebug::dPrintDBAPI(' update slt_enrollment $allowupdate = ' , $selectStmt);
			   if($surveyid == 0){
					if(strtolower($content_status) == 'completed' || strtolower($content_status) == 'passed' || strtolower($content_status) == 'failed' || (strtolower($content_status) == 'unknown' && $completion_status != 'incomplete') ){
						//update slt_attendance_summary
					$updateStmt = db_update('slt_attendance_summary');
					if($content_type !=  "sry_det_typ_ass" && $content_type != 'sry_det_typ_sry')
						$updateStmt->expression('total_attempts', 'total_attempts+1');
				    $updateStmt->fields(array('last_attempt_date' => now(),'progress' => $progressUpdate,'suspend_data' => $suspend_data));
				    $updateStmt->condition('user_id',$uid,'=');
					$updateStmt->condition('course_id', $course_id,'=');
					$updateStmt->condition('class_id', $class_id,'=');
					$updateStmt->condition('lesson_id', $lesson_id,'=');
					$updateStmt->condition('content_version_id', $version_id,'=');
					$updateStmt->condition('enrollment_id', $regid,'=');
				    expDebug::dPrintDBAPI(' update slt_attendance_summary total attempts = ' , $updateStmt);
				    //$summary_upd = $updateStmt->execute();
				    if(!empty($statements)) {
				    	$statements[] = $updateStmt;
				    } else {
				    	$summary_upd = $updateStmt->execute();
				    }
					}
				}else{
					//update slt_attendance_summary
					$updateStmt = db_update('slt_attendance_summary');
					if($content_type !=  "sry_det_typ_ass" && $content_type != 'sry_det_typ_sry')
						$updateStmt->expression('total_attempts', 'total_attempts+1');
				    $updateStmt->fields(array('last_attempt_date' => now(),'progress' => $progressUpdate,'suspend_data' => $suspend_data));
				    $updateStmt->condition('user_id',$uid,'=');
					$updateStmt->condition('course_id', $course_id,'=');
					$updateStmt->condition('class_id', $class_id,'=');
					$updateStmt->condition('lesson_id', $lesson_id,'=');
					$updateStmt->condition('content_version_id', $version_id,'=');
					$updateStmt->condition('enrollment_id', $regid,'=');
					$updateStmt->condition('assessment_id', $surveyid,'=');
				    expDebug::dPrintDBAPI(' update slt_attendance_summary total attempts with survey id = ' , $updateStmt);
				    //$summary_upd = $updateStmt->execute();
				    if(!empty($statements)) {
				    	$statements[] = $updateStmt;
				    } else {
				    	$summary_upd = $updateStmt->execute();
				    }
				    
				}
				$selectStmt = db_select('slt_enrollment', 'enr');
				$selectStmt->addField('enr','master_enrollment_id','master_enrollment_id');
				$selectStmt->condition('enr.id',$regid,'=');
				expDebug::dPrintDBAPI(' previous enrollment details query = ' , $selectStmt);
				$master_enroll_id = $selectStmt->execute()->fetchField();
				if($calltype == "'assesmentTP'"){
					$program_id = getProgramIdFromEnrollment($master_enroll_id);
					updateAssSurTotAttempt($regid,$surveyid,$uid,$class_id,$course_id,$lesson_id,$version_id,$content_type,$program_id,$master_enroll_id,$statements); 				}
 				else if($content_type ==  "sry_det_typ_ass" || $content_type == 'sry_det_typ_sry'){
 					updateAssSurTotAttempt($regid,$surveyid,$uid,$class_id,$course_id,$lesson_id,$version_id,$content_type,0,0,$statements);
 				}
		}
		
		expDebug::dPrint('SurMan $allowupdate Value: ' . $allowupdate, 4);
		
		if($allowupdate == 0){
			$type = 'Online';
			if(empty($summary_det[0]->summaryid)){
				$result = lp_attendance_summary_ins($regid,$uid,$course_id,$class_id,$lesson_id,$version_id,
				$surveyid,$status,$pstatus,$score,$lessonloc,$type,$content_status,$completion_status,
				$launch_data,$suspend_data,$cmiexit,$aicc_data,$updateuser,$calltype,$assesscheck,$time_spend,$launchflag,$content_type,$progress,$subtype,$statements);
			}else{
				$result = lp_attendance_summary_upd($regid,$uid,$course_id,$class_id,$lesson_id,$version_id,
				$surveyid,$status,$pstatus,$score,$lessonloc,$type,$content_status,$completion_status,
				$launch_data,$suspend_data,$cmiexit,$aicc_data,$updateuser,$calltype,$assesscheck,$time_spend,$launchflag,$content_type,$progress,$subtype,$statements);
			}	
		}
		syncSolrData('Enrollment');
		syncSolrData('MasterEnrollment');
		syncSolrData('User');
		return $result;
	} catch(Exception $e){
		expDebug::dPrint('Error in lp_attendance_details_ins '.$e,1);
		expertusLogError($e);
	}
}

/*
 * PHP function for sql procedure slp_mark_program_complete
*/
/* function lp_mark_program_complete($user_id,$master_enrollment_id,$program_id,$upduser_id,$notifyby,$callfrom){
  try{	
  	expDebug::dPrint("inside mark program complete >>>> ".$master_enrollment_id);
  	
	$mandatory = 0;
	$compDate = getMasterEnrollCompletionDate($master_enrollment_id);
  	
	//get tp content status
	$master_enroll_content_status = getTpContentStatus($master_enrollment_id);
	
	//get recertify path
	$selectStmt = db_select('slt_master_enrollment', 'enr');
	$selectStmt->addField('enr','recertify_path','recertify_path');
	$selectStmt->condition('enr.id',$master_enrollment_id,'=');
	$selectStmt->condition('enr.is_current','Y','=');
	$certify_flag = $selectStmt->execute()->fetchField();
	
	//Master enrollment details
	$selectStmt = db_select('slt_master_enrollment', 'enr');
	$selectStmt->addField('enr','overall_status','overall_status');
	$selectStmt->condition('enr.id',$master_enrollment_id,'=');
	$selectStmt->condition('enr.user_id',$user_id,'=');
	$selectStmt->condition('enr.program_id',$program_id,'=');
	$prevStatus = $selectStmt->execute()->fetchField();

	// for audit trail
	$overallstaus_old = $prevStatus;
	
	//get module id from program id
	$module_id = getModuleIdFromObjectId($program_id,$certify_flag);
	
	//get mandatory count
	$selectStmt = db_select('slt_module_crs_mapping', 'map');
	$selectStmt->addExpression('COUNT(map.is_required)','required');
	$selectStmt->condition('map.program_id',$program_id,'=');
	$selectStmt->condition('map.is_required','Y','=');
	$selectStmt->condition('map.module_id',$module_id[0]->module_id,'=');
	$mandatory = $selectStmt->execute()->fetchField();
	
	if($callfrom != 'program' || $notifyby !='manager'){
	    $selectStmt = db_select('slt_survey_mapping', 'assmap');
	    $selectStmt->join('slt_survey', 'srv','srv.id = assmap.survey_id AND srv.type = \'sry_det_typ_ass\'');
		$selectStmt->addExpression('count(1)','assessment');
		$selectStmt->condition('assmap.object_type','cre_sys_obt_cls','!=');
		$selectStmt->condition('assmap.object_id',$program_id,'=');
		$selectStmt->condition('assmap.module_id',$module_id[0]->module_id,'=');
		$selectStmt->condition('assmap.pre_status',0,'=');
		$isHasAssesment = $selectStmt->execute()->fetchField();
	}else
    	$isHasAssesment = 0; 
    	
    //check incomplete class count
    $selectStmt = db_select('slt_enrollment', 'enroll');
    $selectStmt->join('slt_master_enrollment', 'menroll','menroll.id = enroll.master_enrollment_id');
    $selectStmt->innerjoin('slt_module_crs_mapping', 'map','map.program_id = menroll.program_id AND enroll.course_id = map.course_id');
	$selectStmt->addExpression('count(DISTINCT enroll.id)','incom_class');
	$selectStmt->condition('enroll.comp_status',array('lrn_crs_cmp_inc','lrn_crs_cmp_nsw'),'IN');
	$selectStmt->condition('map.is_required','Y','=');
	$selectStmt->condition('menroll.id',$master_enrollment_id,'=');
	$incom_class = $selectStmt->execute()->fetchField();	
  	if($isHasAssesment == 0 || $incom_class > 0){  
  		
      	 // Enrollment values needs to be checked before updating the program  
	      $selectEnrollQuery = db_select('slt_enrollment', 'enroll');
	      $selectEnrollQuery->innerjoin('slt_module_crs_mapping', 'mapping', 'mapping.course_id = enroll.course_id');
	      $selectEnrollQuery->addField('enroll', 'master_enrollment_id');
	      $selectEnrollQuery->addField('enroll', 'user_id');
	      if($mandatory>0){
	      	$selectEnrollQuery->addExpression('MAX(CASE WHEN enroll.comp_status=\'lrn_crs_cmp_cmp\' THEN 0 ELSE 1 END)', 'cmp_status');
	      	$selectEnrollQuery->addExpression('MAX(CASE WHEN enroll.comp_status in (\'lrn_crs_cmp_enr\',\'lrn_crs_cmp_inp\',\'lrn_crs_cmp_att\',\'lrn_crs_cmp_nsw\') THEN 1 ELSE 0 END)', 'inp_status');
	      	$selectEnrollQuery->addExpression('MAX(CASE WHEN enroll.comp_status in (\'lrn_crs_cmp_inc\',\'lrn_crs_cmp_nsw\') THEN 1 ELSE 0 END)', 'inc_status');
	      	$selectEnrollQuery->condition('mapping.is_required', 'Y');
	      }else{
	      	$selectEnrollQuery->addExpression('MIN(CASE WHEN enroll.comp_status=\'lrn_crs_cmp_cmp\' THEN 0 ELSE 1 END)', 'cmp_status');
	      	$selectEnrollQuery->addExpression('MAX(CASE WHEN enroll.comp_status in (\'lrn_crs_cmp_enr\',\'lrn_crs_cmp_inp\',\'lrn_crs_cmp_att\') THEN 1 ELSE 0 END)', 'inp_status');
	      	$selectEnrollQuery->addExpression('0','inc_status');
	      }
	      
	      $selectEnrollQuery->condition('mapping.program_id', $program_id);
	      $selectEnrollQuery->condition('enroll.master_enrollment_id', $master_enrollment_id);
	      $selectEnrollQuery->groupby('enroll.master_enrollment_id');
	      $selectEnrollQuery->groupby('enroll.user_id');
	      expDebug::dPrintDBAPI(' $selectEnrollQuery = ' , $selectEnrollQuery);
	      $selectEnrollResult = $selectEnrollQuery->execute()->fetchAll();

	      $cmp_status = $selectEnrollResult[0]->cmp_status;
	      $inp_status = $selectEnrollResult[0]->inp_status;
	      $inc_status = $selectEnrollResult[0]->inc_status;
	      expDebug::dPrint(' $completionStatus '.$cmp_status .' $InprogressStatus '.$inp_status .' $IncompleteStatus '.$inc_status ,5);
	      //if(($completionStatus !='' && ($completionStatus == 0 || $completionStatus == 1)) && ($InprogressStatus != '' && ($InprogressStatus==0 || $IncompleteStatus==1))){
	      $overallstatus = '';
	      if($mandatory>0){
	          if($cmp_status == 0 && $inp_status == 0)
	          	$overallstatus = 'lrn_tpm_ovr_cmp';
	          else if($cmp_status == 1 && $inc_status == 1)
	          	$overallstatus = 'lrn_tpm_ovr_inc';
	      }else{
	      	  if($cmp_status == 0 && $inp_status == 0)
	          	$overallstatus = 'lrn_tpm_ovr_cmp';
	          else if($cmp_status == 1 && $inp_status == 0 && $inc_status == 1)
	          	$overallstatus='lrn_tpm_ovr_inc';
	      }  
		  
          $updateMasterField = array();
          // Update master enrollment
         expDebug::dPrint("overall status final >> ".$overallstatus );
          if(!empty($overallstatus))	
          	$updateMasterField['overall_status'] = $overallstatus;
      	  if(($cmp_status == 0 || $cmp_status == 1) && ($inp_status == 0)){   
      		$updateMasterField['comp_date'] = $compDate;
      		$updateMasterField['updated_by'] = $upduser_id;
      		$updateMasterField['updated_on'] = now();
      	  }
      	  if(count($updateMasterField) > 0 ){
		      $updateMasterEnroll = db_update('slt_master_enrollment');
		      $updateMasterEnroll->fields($updateMasterField);
		      $updateMasterEnroll->condition('id', $master_enrollment_id);
		      $updateMasterEnroll->condition('user_id', $user_id);
		      $updateMasterEnroll->condition('program_id', $program_id);
		      $updateMasterEnroll->condition('overall_status', array('lrn_tpm_ovr_cln','lrn_tpm_ovr_cmp','lrn_tpm_ovr_exp','lrn_tpm_ovr_ppm','lrn_tpm_ovr_ppv','lrn_tpm_ovr_inc'), 'NOT IN');
		      $updateMasterEnroll->execute();   
		      expDebug::dPrintDBAPI('updateMasterEnroll query :',$updateMasterEnroll);
      	  }
	}
	
	// To update percentage of complete - old column
	updateTPEnrollmentPercentComplete($program_id, $master_enrollment_id, $certify_flag);

	// To update  a overall percentage complete used in content player - new column
	updateTPEnrollmentOverallPercentComplete($program_id, $master_enrollment_id, $certify_flag);
	
	$selectStmt = db_select('slt_master_enrollment', 'enr');
	$selectStmt->addField('enr','overall_status','overall_status');
	$selectStmt->condition('enr.id',$master_enrollment_id,'=');
	$selectStmt->condition('enr.user_id',$user_id,'=');
	$selectStmt->condition('enr.program_id',$program_id,'=');
	$aftrStatus = $selectStmt->execute()->fetchField();
	
	if($notifyby !='manager' && !empty($notifyby)){
		$call_type = explode($notifyby,'-');
		$fromAssesment = $call_type[0]; 
	 	if($fromAssesment != $notifyby)
			$prevStatus = $fromAssesment;
	}
	
	expDebug::dPrint("inside mark program complete22222 >>>> prev ". $prevStatus ."<<< ".$aftrStatus);
	
	if($prevStatus != $aftrStatus){
		 if($notifyby == 'manager'){
		 	 insertNotification('CertCurrLPCompleteByManager', $master_enrollment_id, $program_id, $user_id, $upduser_id);
		 }else{
		 	 insertNotification('CertCurrLPComplete', $master_enrollment_id, $program_id, $user_id, $upduser_id);
		 }
		 expDebug::dPrint("inside mark program complete 33333>>>> ");
		 //Skill Update for TP complete
		$objecttype = db_select('slt_program', 'prg');
		$objecttype->addField('prg','object_type','objecttype');
		$objecttype->addField('prg','title','title');
		$objecttype->condition('id', $program_id);
		$objResult = $objecttype->execute()->fetchAll();
		expDebug::dPrintDBAPI('objecttype query :', $objecttype);
	  	expDebug::dPrint(' $object result>>>  '.print_r($objResult,true),5);
		if($objResult[0]->objecttype == 'cre_sys_obt_crt') {  
	      updateSkillForTpCompletion($program_id,$user_id,$master_enrollment_id,$objResult[0]->title,$upduser_id);
		}
		//get new overall status
		 $overallcomplete = db_select('slt_master_enrollment', 'enroll');
         $overallcomplete->addField('enroll', 'overall_status');
         $overallcomplete->addField('enroll', 'recertify_path');
         $overallcomplete->condition('id', $master_enrollment_id);
         $overallcompleted = $overallcomplete->execute()->fetchAll();
         $overallstaus_new = $overallcompleted[0]->overall_status;
		
         //Update custom4 with completion status
         $updateMasterFields['custom4'] = $overallstaus_new;
         $updateMasterFields['overall_status'] = $overallstaus_new;
         if($certify_flag > 1 && ($overallstaus_new == 'lrn_tpm_ovr_inc' || $overallstaus_new == 'lrn_tpm_ovr_cln')){
         	  $updateMasterFields['is_current'] = 'N';
         }
	          $updateMasterEnroll = db_update('slt_master_enrollment');
		      $updateMasterEnroll->fields($updateMasterFields);
		      $updateMasterEnroll->condition('id', $master_enrollment_id);
		      $updateMasterEnroll->condition('program_id', $program_id);
		      $updateMasterEnroll->execute();   
		      expDebug::dPrintDBAPI('previous enrollment update query :',$updateMasterEnroll);
		if($certify_flag > 1 && ($overallstaus_new == 'lrn_tpm_ovr_inc' || $overallstaus_new == 'lrn_tpm_ovr_cln')){    
		    $selectStmt = db_select('slt_master_enrollment', 'ms');
			$selectStmt->addField('ms','id','id');
			$selectStmt->addField('ms','custom4','custom4');
			$selectStmt->addField('ms','comp_date','comp_date');
			$selectStmt->condition('ms.program_id',$program_id,'=');
			$selectStmt->condition('ms.user_id',$user_id,'=');
			$selectStmt->condition('ms.recertify_path',($certify_flag-1),'=');
			$selectStmt->orderBy('ms.id', 'desc');
			$selectStmt->range(0, 1);
			expDebug::dPrintDBAPI('previous enrollment status update query :',$selectStmt);
			$prev_menr = $selectStmt->execute()->fetchAll();
			expDebug::dPrint("prev enrollment  >> ".print_r($prev_menr,true),5);
			$prev_menrid = $prev_menr[0]->id;
			$prev_custom4 = $prev_menr[0]->custom4;
			$prev_comp = $prev_menr[0]->comp_date;
	       	expDebug::dPrint("master enrollmetn id prev ". $prev_menrid ,5);
		       
	       	$cert_days_expire = isset($config["cert_expire_reminder_days"]) ? $config["cert_expire_reminder_days"] : 10;
	       	//get certificate expired or not
	       	$select = db_select('slt_program', 'p');
	       	$select->addExpression('(CASE WHEN(DATEDIFF(
	               IF (p.expires_in_unit = \'days\', DATE_ADD(\''.$prev_comp.'\', INTERVAL p.expires_in_value DAY),
	                     IF (p.expires_in_unit = \'months\', DATE_ADD(\''.$prev_comp.'\', INTERVAL p.expires_in_value MONTH),
	                           DATE_ADD(\''.$prev_comp.'\', INTERVAL p.expires_in_value YEAR))),
	               CURDATE()) < 0)THEN \'lrn_tpm_ovr_exp\'
	               ELSE \'lrn_tpm_ovr_cmp\' END )','prev_status');
				
	 	 	$select->condition('p.id',$program_id);
	 	 	expDebug::dPrintDBAPI('certificate expiry or not :',$select);
	       	$prev_sts = $select->execute()->fetchField();	
	       
	        $updateMasterFields1['overall_status'] = $prev_sts;
	        $updateMasterFields1['is_current'] = 'Y';
	        $updateMasterEnroll1 = db_update('slt_master_enrollment');
	        $updateMasterEnroll1->fields($updateMasterFields1);
	        $updateMasterEnroll1->condition('id', $prev_menrid);
	        $updateMasterEnroll1->condition('program_id', $program_id);
	        expDebug::dPrintDBAPI('master enrollment update query 222 :',$updateMasterEnroll1);
	        $updateMasterEnroll1->execute();  
		 }
	    //Update enrollment table when the overall status of TP marked as incomplete
	    if($overallstaus_new == 'lrn_tpm_ovr_inc'){
	    	  $updateMasterEnroll = db_update('slt_enrollment');
		      $updateMasterEnroll->fields(array('comp_status'=>'lrn_crs_cmp_inc'));
		      $updateMasterEnroll->condition('master_enrollment_id', $master_enrollment_id);
		      $updateMasterEnroll->condition('comp_status', array('lrn_crs_cmp_cmp','lrn_crs_cmp_nsw'));
		      $updateMasterEnroll->execute();   
		      expDebug::dPrintDBAPI('updateMasterEnroll query :',$updateMasterEnroll);
	    }
	    $operationstatus = ($aftrStatus == 'lrn_tpm_ovr_cmp') ? "tpcompleted" : "tpincompleted";
	    
	    //Audit trail entry
	    make_audit_trail_entries($updateMasterField['overall_status'], getIdOfLoggedInUser() , $program_id, $prevStatus , 'updated master_enrollment status',$objResult[0]->objecttype,'exp_sp_lnrlearningplan',$operationstatus,
                                                array('overall_status'));  
	    
	}
	
	
    	
  }catch(Exception $e){
		expDebug::dPrint('Error in getclassContentStatus '.$e,1);
		expertusLogError($e);
	}
} */
/**
 * Calculates percentage complete of a tp enrollment and saves in slt_master_enrollment
 */
function updateTPEnrollmentPercentComplete($programId, $masterEnrollId, $recertFlag,&$statements=array()) {
	try {
    $select1 = db_select('slt_enrollment', 'enr');
    $module_id = getModuleIdFromObjectId($programId,$recertFlag);
   /* if ($recertFlag > 1) {
      $select1->join('slt_re_certify', 'crsmap', 'enr.course_id = crsmap.course_id AND crsmap.program_id = :prgmid',
                                                                                                     array(':prgmid' => $programId));
    }
    else {*/
    $select1->join('slt_module_crs_mapping', 'crsmap', 'enr.course_id = crsmap.course_id AND crsmap.program_id = :prgmid',
                                                                                                     array(':prgmid' => $programId));
    //}
    $select1->addExpression("COUNT(CASE WHEN enr.comp_status = 'lrn_crs_cmp_cmp' AND crsmap.is_required = 'Y' THEN enr.id END)",'mandatory_completed_courses_count');
    $select1->addExpression("COUNT(CASE WHEN enr.comp_status = 'lrn_crs_cmp_cmp' AND crsmap.is_required != 'Y' THEN enr.id END)",'optional_completed_courses_count');
    $select1->addExpression("COUNT(CASE WHEN crsmap.is_required = 'Y' THEN enr.id END)", 'mandatory_courses_count');
    $select1->addExpression("COUNT(enr.id)", 'total_courses_count');
    $select1->condition('enr.master_enrollment_id', $masterEnrollId, '=');
    $select1->condition('crsmap.module_id', $module_id[0]->module_id, '=');
    $select1->condition('enr.reg_status', array('lrn_crs_reg_rsv', 'lrn_crs_reg_rsc'), 'NOT IN');
    expDebug::dPrintDBAPI('$select1 to get completion counts', $select1);
   
    $select2 = db_select($select1, 'completion_data');
    $pcExpr = <<<EOD
      (CASE WHEN (total_courses_count = 0) THEN
         NULL
       ELSE
         ROUND(CASE WHEN (mandatory_courses_count = 0) THEN
                 optional_completed_courses_count / total_courses_count
               ELSE
                 mandatory_completed_courses_count / mandatory_courses_count
               END * 100, 0)
      END)
EOD;
     
    $select2->addExpression($pcExpr, 'percentage_completed');
    expDebug::dPrintDBAPI('$select2 for finding percentage completed', $select2);
    $percentageCompleted = $select2->execute()->fetchField();
    expDebug::dPrint('$percentageCompleted = ' . $percentageCompleted);
      
    $updStmt = db_update('slt_master_enrollment');
    $updStmt->fields(array('percentage_complete' => empty($percentageCompleted)? null : $percentageCompleted));
    $updStmt->condition('id', $masterEnrollId, '=');
    $updStmt->condition('percentage_complete', $percentageCompleted, '<');
    expDebug::dPrintDBAPI('$updStmt for updating percentage complete in master enrollment table', $updStmt);
   // $updStmt->execute();
    if(!empty($statements)) {
    	$statements[] = $updStmt;
    } else {
    	$updStmt->execute();
    }
    
    
    if (!empty($masterEnrollId)) {
    	$program_id = getProgramIdFromEnrollment($masterEnrollId);
    
    	$totalattemptsleftTp = CalculateTotalAttemptsLeftTp($program_id,$masterEnrollId);
    	expDebug::dPrint("totalattemptslefttotalattemptsleftTp : ".print_r($totalattemptsleftTp,true),5);
    	
    	if(($totalattemptsleftTp) <= 0){
    		//$updateattempts = db_query("UPDATE slt_master_enrollment SET launchable=0 WHERE id=".$masterEnrollId);
    		$updateStmt = db_update('slt_master_enrollment');
    		$updateStmt->fields(array('launchable' => 0));
    		$updateStmt->condition('id',$masterEnrollId,'=');
    		//$updateattempts = $updateStmt->execute();
    		if(!empty($statements)) {
    			$statements[] = $updateStmt;
    		} else {
    			$updateattempts = $updateStmt->execute();
    		}
    		expDebug::dPrintDBAPI(' $updateattempts = ' , $updateQuery,4);
    	}
    	
    	
    }
    
    
  }
  catch (Exception $ex) {
    watchdog_exception('updateTPEnrollmentPercentComplete', $ex);
    expertusErrorThrow($ex);
  }   	
}

function updateAssSurTotAttempt($enrollId,$surId,$userId,$classId,$courseId,$lessonId,$versionId,$contentType,$program_id=0,$master_enroll_id=0,&$statements=array()){
	try{
		expDebug::dPrint("totalattempt master id : ".print_r($master_enroll_id,true),5);
		$objectType = 'cre_sys_obt_cls';
		if(isset($_REQUEST["objecttype"]) && !empty($_REQUEST["objecttype"])){
			$objectType   = ($_REQUEST["objecttype"]!=null && $_REQUEST["objecttype"]!='') ?"'".$_REQUEST["objecttype"]."'":"null";
			$objectType   = str_replace("'", "", $objectType);
		}
		expDebug::dPrint(' $objectType = ' . print_r($objectType, true) , 4);
		if($objectType == 'cre_sys_obt_cls'){
			$objectId = $classId;
		}else{
			$objectId = $lessonId;
		}
		if($program_id>0)
			$objectId = $program_id;
		if(isset($_REQUEST['apiname']) && $program_id>0 ){
			$select = db_select('slt_program', 'prg');
			$select->addField('prg','object_type','object_type');
			$select->condition('prg.id', $program_id);
			$objresult =$select->execute()->fetchField();
			expDebug::dPrintDBAPI('objecttype query:', $select);
			expDebug::dPrint('objresult'.$objresult,4);
			$objectType = $objresult;
		}
		if($versionId== null || $versionId== 'null'|| $versionId=='' || empty($versionId))
			$versionId = 0;
		if($master_enroll_id>0)
			$total_attempts = getTotalAttemptCountSurAss($master_enroll_id,$surId,$objectId,$objectType,$userId);
		else 
			$total_attempts = getTotalAttemptCountSurAss($enrollId,$surId,$objectId,$objectType,$userId);
		$updateStmt = db_update('slt_attendance_summary');
		$updateStmt->fields(array('total_attempts' => $total_attempts));
		$updateStmt->condition('user_id',$userId,'=');
		$updateStmt->condition('course_id', $courseId,'=');
		$updateStmt->condition('class_id', $classId,'=');
		$updateStmt->condition('lesson_id', $lessonId,'=');
		$updateStmt->condition('content_version_id', $versionId,'=');
		$updateStmt->condition('enrollment_id', $enrollId,'=');
		$updateStmt->condition('content_type', $contentType,'=');
		$updateStmt->condition('assessment_id', $surId,'=');
		expDebug::dPrintDBAPI(' update slt_attendance_summary total attempts with survey id = ' , $updateStmt);
		//$summary_upd = $updateStmt->execute();
		if(!empty($statements)) {
			$statements[] = $updateStmt;
		} else {
			$summary_upd = $updateStmt->execute();
		}
	}catch (Exception $ex) {
	    watchdog_exception('updateAssSurTotAttempt', $ex);
	    expertusErrorThrow($ex);
  } 
}
function getTotalAttemptCountSurAss($enrollId,$surId,$objectId,$objectType,$userId){
	$assquery = db_query('select count(id) as total_attempts from slt_survey_results where enrollment_id = :enrollId AND  survey_id = :surId AND entity_id=:objectId AND entity_type=:objectType AND user_id=:userId  group by question_id limit 1',
			array(':enrollId'=>$enrollId,':surId' => $surId,':objectId' => $objectId,':objectType' => $objectType,':userId' => $userId));
	expDebug::dPrintDBAPI("Asesment count details---->", $assquery,array(':enrollId'=>$enrollId,':surId' => $surId,':objectId' => $objectId,':objectType' => $objectType,':userId' => $userId));
	$assRes = $assquery->fetchAll();
	$total_attempts = $assRes[0]->total_attempts;
	expDebug::dPrint(' total_attempts = ' . print_r($total_attempts, true) , 4);
	return $total_attempts;
}
function updateTPEnrollmentOverallPercentComplete($programId, $masterEnrollId, $recertFlag='',$from='',&$statements=array()) {
	try {
		
		include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_survey/exp_sp_survey_learner/exp_sp_surveylearner.inc');
		include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_learning/exp_sp_learning_learner/modules/exp_sp_lnrlearningplan/exp_sp_lnrlearningplan.inc');
		
		$sum 		= 0;
		$total		= 0;
		$progress 	= 0;
		
		if (empty($recertFlag)) {
			//get recertify path
			$selectStmt = db_select('slt_master_enrollment', 'enr');
			$selectStmt->addField('enr','recertify_path','recertify_path');
			$selectStmt->condition('enr.id',$masterEnrollId,'=');
			$selectStmt->condition('enr.is_current','Y','=');
			$recertFlag = $selectStmt->execute()->fetchField();
		}
		
		// collect program details
		$selectTp = db_select('slt_master_enrollment', 'menr');
		$selectTp->innerJoin('slt_program','prg','prg.id=menr.program_id');
		$selectTp->addField('menr', 'user_id', 'user_id');
		$selectTp->addField('menr', 'program_id', 'program_id');
		$selectTp->addField('menr', 'overall_status', 'overall_status');
		$selectTp->addField('prg', 'object_type', 'object_type');
		$selectTp->addField('prg', 'title', 'class_title');
		$selectTp->condition('menr.id', $masterEnrollId);
		$result = $selectTp->execute()->fetchObject();
		
		$enrCntDetail = array();
		$enrCntDetail['pre_ass_detail'] 	= getAssignedSurveys($result->user_id, $result->program_id, $result->object_type, 'assessment','1',$masterEnrollId,'yes');
		$enrCntDetail['post_ass_detail'] 	= getAssignedSurveys($result->user_id, $result->program_id, $result->object_type, 'assessment','0',$masterEnrollId,'yes');
		$enrCntDetail['survey_detail']		= getAssignedSurveys($result->user_id, $result->program_id, $result->object_type, 'survey', '0', $masterEnrollId, 'yes');
		$programDetails 					= getProgramDetails($masterEnrollId);
		
		
		if(!empty($enrCntDetail['pre_ass_detail'][0]->surveyid)){
			//pre and post assessment summary details
			$percentage = getSummaryDetails($result->user_id, $programDetails[0]->class_id, $enrCntDetail['pre_ass_detail'][0]->surveyid, $programDetails[0]->id,'sry_det_typ_ass');
			$total = $total+1;
			$sum+= $percentage;
		}
		
		if(!empty($enrCntDetail['post_ass_detail'][0]->surveyid)){
			$percentage = getSummaryDetails($result->user_id, $programDetails[0]->class_id, $enrCntDetail['post_ass_detail'][0]->surveyid, $programDetails[0]->id, 'sry_det_typ_ass');
			$total = $total+1;
			$sum+= $percentage;
		}
		
		/* if(!empty($enrCntDetail['survey_detail'])){
			foreach ($enrCntDetail['survey_detail'] as $key =>$survey_details){
				$percentage = getSummaryDetails($result->user_id, $programDetails[0]->class_id, $survey_details->surveyid, $programDetails[0]->id, 'sry_det_typ_sry');
				$total = $total+1;
				$sum+= $percentage;
			}
		} */
		
		$select1 = db_select('slt_enrollment', 'enr');
		$module_id = getModuleIdFromObjectId($programId,$recertFlag);
		$select1->join('slt_module_crs_mapping', 'crsmap', 'enr.course_id = crsmap.course_id AND crsmap.program_id = :prgmid',
				array(':prgmid' => $programId));
		//}
		$select1->addExpression("SUM(progress)",'total_course_percentage');
		//$select1->addExpression("COUNT(CASE WHEN crsmap.is_required = 'Y' THEN enr.id END)", 'mandatory_courses_count');
		$select1->addExpression("COUNT(enr.id)", 'total_courses_count');
		$select1->condition('enr.master_enrollment_id', $masterEnrollId, '=');
		$select1->condition('crsmap.module_id', $module_id[0]->module_id, '=');
		$select1->condition('enr.reg_status', array('lrn_crs_reg_rsv', 'lrn_crs_reg_rsc'), 'NOT IN');
		expDebug::dPrintDBAPI('$select1 to get completion counts', $select1);
		$resultData =  $select1->execute()->fetchAll();
		
		expDebug::dPrint('result data collected: ' . print_r($resultData, 1), 4);
		
		$total += $resultData[0]->total_courses_count;
		$sum += $resultData[0]->total_course_percentage;
		
		expDebug::dPrint('class level total: ' . $total, 4);
		expDebug::dPrint('class level sum: ' . $sum, 4);
		
		
		$percentageCompleted = $sum/$total;
		expDebug::dPrint('overall progress : ' . $percentageCompleted, 4);
		
		//$progress = convertProgressValue($progress);
		if($result->overall_status =='lrn_tpm_ovr_cmp' ){
			$percentageCompleted =100;
		}
		
		$updatefields = array();
		if(!empty($from) && $from == 'content_version' && $percentageCompleted == '100'){
			$updatefields['overall_status'] = 'lrn_tpm_ovr_cmp';
			$updatefields['comp_date'] = now();
		}
		$updatefields['overall_progress'] = empty($percentageCompleted)? null : $percentageCompleted;
		$updatefields['updated_on'] = now();
		$updStmt = db_update('slt_master_enrollment');
		$updStmt->fields($updatefields);
		$updStmt->condition('id', $masterEnrollId, '=');
		//$updStmt->condition('overall_progress', $percentageCompleted, '<');
		if(empty($from)) /*skipped for content versioning*/
		  $updStmt->where('if(overall_progress IS NOT NULL ,overall_progress <  '.$percentageCompleted.',1=1)');
		expDebug::dPrintDBAPI('$updStmt for updating percentage complete in master enrollment table', $updStmt);
		if(!empty($statements)) {
			$statements[] = $updStmt;
		} else {
			$updStmt->execute();
		}
		//$updStmt->execute();
		
		
		if (!empty($masterEnrollId)) {
			$program_id = getProgramIdFromEnrollment($masterEnrollId);
			$totalattemptsleftTp = CalculateTotalAttemptsLeftTp($program_id,$masterEnrollId);
			expDebug::dPrint("totalattemptslefttotalattemptsleftTp : ".print_r($totalattemptsleftTp,true),5);
			if(($totalattemptsleftTp) <= 0){
			//	$updateattempts = db_query("UPDATE slt_master_enrollment SET launchable=0,updated_on=now() WHERE id=".$masterEnrollId);
				$updateStmt = db_update('slt_master_enrollment');
				$updateStmt->fields(array('launchable' => 0,'updated_on' => now()));
				$updateStmt->condition('id',$masterEnrollId,'=');
				//$updateattempts = $updateStmt->execute();
				if(!empty($statements)) {
					$statements[] = $updateStmt;
				} else {
					$updateattempts = $updateStmt->execute();
				}
				expDebug::dPrintDBAPI(' $updateattempts = ' , $updateQuery,4);
			}
		}
		
		
	}
	catch (Exception $ex) {
		watchdog_exception('updateTPEnrollmentPercentComplete', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * PHP function for sql procedure slf_get_class_content_status
*/
function getclassContentStatus($class_id,$regid){
	try{
		//get Content launch count
		$selectStmt = db_select('slt_attendance_summary', 'summ');
		$selectStmt->addExpression('COUNT(summ.id)','launch_count');
		$selectStmt->condition('summ.enrollment_id',$regid,'=');
		//$selectStmt->condition('summ.assessment_id', '','IS NULL');
		$selectStmt->condition(db_or()
							->condition('summ.assessment_id','','IS NULL')
							->condition('summ.assessment_id', 0 , '='));
		expDebug::dPrintDBAPI('get previous content status',$selectStmt);
		$launch_count = $selectStmt->execute()->fetchField();
		expDebug::dPrint('getContentStatus result'.print_r($launch_count,true), 4);
		
		//get total lesson count
		$selectStmt = db_select('slt_content_version', 'scv');
		$selectStmt->leftjoin('slt_enrollment_content_mapping', 'secm','secm.content_id = scv.content_master_id AND secm.version_id = scv.id');
		$selectStmt->addExpression('SUM(total_lesson)','lesson_count');
		$selectStmt->condition('secm.class_id',$class_id,'=');
		$selectStmt->condition('secm.enroll_id',$regid,'=');
		expDebug::dPrintDBAPI('get total lesson count',$selectStmt);
		$lesson_count = $selectStmt->execute()->fetchField();
		expDebug::dPrint('get total lesson count result'.print_r($lesson_count,true), 4);
		
		if($launch_count != $lesson_count)
			$enr_quiz_status = "";
		else{
			//get content and completion status
			$selectStmt = db_select('slt_attendance_summary', 'summ');
			$selectStmt->addField('summ','content_status','content_status');
			$selectStmt->addField('summ','completion_status','completion_status');
			$selectStmt->condition('summ.enrollment_id', $regid,'=');
			$result = $selectStmt->execute()->fetchAll();
			foreach($result as $key=>$var){
				if($var->content_status == "failed" || $var->content_status == 'incomplete' || trim($var->content_status) == ''){
					$enr_quiz_status = $var->content_status;
					break;
				}else if($var->completion_status == 'completed' || empty($var->completion_status)){
				 	$enr_quiz_status = "passed";
				}else{
					$enr_quiz_status = "";
				}
			}
		}
		expDebug::dPrint('get  $enr_quiz_status result'.print_r($enr_quiz_status,true), 4);
		return $enr_quiz_status;
	} catch(Exception $e){
		expDebug::dPrint('Error in getclassContentStatus '.$e,1);
		expertusLogError($e);
	}
}
/**
 * Function to get list of version for a particular enrollment
 * @param  $class_id
 * @param  $regid
 * @param  $uid
 * @return array of version ids
 */
function getcontentVersions($class_id,$regid,$uid){
	//Get version ids for the enrolled contents
	$selectStmt = db_select('slt_enrollment_content_mapping', 'emap');
	$selectStmt->addExpression('emap.version_id','vid');
	$selectStmt->condition('emap.class_id',$class_id,'=');
	$selectStmt->condition('emap.user_id',$uid,'=');
	$selectStmt->condition('emap.enroll_id', $regid,'=');
	expDebug::DPrintDBAPI("content version list test ",$selectStmt);
	$lesson_version_list = $selectStmt->execute()->fetchAll();
	expDebug::dPrint("lesson version list values ".print_r($lesson_version_list,true),5);
	return $lesson_version_list;
}
/*
 * PHP function for sql procedure slf_get_content_count
 */
function getcontentCount($class_id,$uid,$regid,$subtype=''){
	try{
		$incLessonFound = 0;
		$currentContentId = 0;
		$currentContentAttempts = 0;
	  
	  $h5pSet = 0; // For H5P
	  
		//Get number of lessons
		$selectStmt = db_select('slt_enrollment_content_mapping', 'emap');
		$selectStmt->leftJoin('slt_content_version', 'cv', 'cv.id = emap.version_id');
		$selectStmt->leftJoin('slt_content_lesson', 'cl', 'cv.id = cl.content_version_id');
		$selectStmt->leftJoin('slt_course_content_mapper', 'ccm', 'ccm.content_id=cv.content_master_id');
		$selectStmt->addExpression('DISTINCT(cl.id)','lessonid');
		$selectStmt->addExpression('ccm.content_id','content_id');
		$selectStmt->addExpression('ccm.max_attempts','max_attempts');
		$selectStmt->condition('ccm.class_id',$class_id,'=');
		$selectStmt->condition('emap.enroll_id', $regid,'=');
		$selectStmt->orderBy('ccm.content_id');
		$selectStmt->orderBy('cl.id');
		expDebug::DPrintDBAPI("content count() test ",$selectStmt);
		$lesson_list = $selectStmt->execute()->fetchAll();
		expDebug::dPrint("lesson list values ".print_r($lesson_list,true),5);
		foreach($lesson_list as $les){
			expDebug::dPrint("maximum attempt count() >> ".print_r($les,true),5);

			$unlimitedAttemptsContentCount = 0;
			//Maximum attempt count
			$select = db_select('slt_course_content_mapper', 'map');
			$select->addExpression('count(1)','cnt');
			$select->condition('map.class_id',$class_id,'=');
			$select->condition('map.content_id',$les->content_id,'=');
			$select->condition(db_or()
					->condition('map.max_attempts','','IS NULL')
					->condition('map.max_attempts', 0 , '='));
			$unlimitedAttemptsContentCount = $select->execute()->fetchField();
		
		expDebug::dPrint("maximum attempt count()".$unlimitedAttemptsContentCount,5);

			$select = db_select('slt_attendance_summary', 'summ');
			$select->addField('summ','status','status');
		$select->addField('summ','progress','progress'); // Added for h5p
			$select->condition('summ.lesson_id',$les->lessonid,'=');
			$select->condition('summ.user_id',$uid,'=');
			$select->condition('summ.class_id',$class_id,'=');
			$select->condition('summ.enrollment_id',$regid,'=');
			$select->condition('summ.content_version_id',0,'!=');
			expDebug::DPrintDBAPI("lesson status from summary ",$select);
		$contentDetails = $select->execute()->fetchAll();
		
		$lessonStatus = $contentDetails[0]->status;
		$subtypeProgress = $contentDetails[0]->progress;
	
		
		if($subtype == "h5p"  && $subtypeProgress == "100" && $lessonStatus == "lrn_crs_cmp_inc"){
			$h5pSet = 1;
		}
			
		
		if(empty($lessonStatus) || $lessonStatus != 'lrn_crs_cmp_cmp'){
			 if($unlimitedAttemptsContentCount >0 && $incLessonFound != 1 && $h5pSet != 1){ 
					$incLessonFound = 2;
				}else{
					if($currentContentId != $les->content_id) {
						$currentContentAttempts = 0;
						//sum of total attempts
						$select = db_select('slt_attendance_summary', 'atts');
						$select->Join('slt_content_version', 'cv');
						$select->Join('slt_content_lesson', 'cl');
						$select->Join('slt_enrollment_content_mapping', 'emap');
						$select->addExpression('SUM(atts.total_attempts)','attempts');
						$select->condition('cv.content_master_id',$les->content_id,'=');
						$select->where('cl.content_version_id = cv.id AND atts.lesson_id = cl.id AND emap.user_id = atts.user_id AND emap.version_id = cv.id');
					//$select->condition(atts.lesson_id,cl.id);
						$select->condition('atts.user_id',$uid,'=');
						$select->condition('atts.class_id',$class_id,'=');
					//$select->condition('emap.user_id',atts.user_id);
						$select->condition('emap.enroll_id',$regid,'=');
					//$select->condition(emap.version_id,cv.id);
						$select->condition('emap.class_id',$class_id,'=');
						$select->condition('atts.user_id',$uid,'=');
						$select->condition('atts.enrollment_id',$regid,'=');
						$select->condition('atts.custom4','','IS NULL');
						expDebug::DPrintDBAPI("sum of total attempts ",$select);
						$currentContentAttempts = $select->execute()->fetchField();
						expDebug::dPrint("current content attempts  ".$currentContentAttempts ,5);
						$currentContentAttempts = empty($currentContentAttempts) ? 0 : $currentContentAttempts;
							
						$currentContentId = $les->content_id;
					}
				 if(($les->max_attempts > $currentContentAttempts)&& $incLessonFound != 1)
				 	$incLessonFound = 2;
				 else
				 	$incLessonFound = 1;
				}
			}
		}
		expDebug::dPrint("getcontentCount incomplete lesson >>>". $incLessonFound ,5);
		if($incLessonFound == 0){
			return 'lrn_crs_cmp_cmp';
		}else if($incLessonFound == 2){
			return 'lrn_crs_cmp_inp';
		}else{
			return 'lrn_crs_cmp_inc';
		}
	} catch(Exception $e){
		expDebug::dPrint('Error in getTpContentStatus '.$e,1);
		expertusLogError($e);
	}
}

/*
 * get maximum attempt number for content
 */
function getAttemptNumber($uid,$course_id,$class_id,$lesson_id,$version_id,$regid,$surveyid=''){
	try{
		$selectStmt = db_select('slt_attendance_details', 'det');
		$selectStmt->addExpression('MAX(attempt_number)+1','attempts');
		$selectStmt->condition('det.user_id',$uid,'=');
		$selectStmt->condition('det.course_id', $course_id,'=');
		$selectStmt->condition('det.class_id', $class_id,'=');
		$selectStmt->condition('det.content_version_id', $version_id,'=');
		$selectStmt->condition('det.lesson_id', $lesson_id,'=');
		$selectStmt->condition('det.enrollment_id', $regid,'=');
		if(!empty($surveyid))
			$selectStmt->condition('det.assessment_id', $surveyid,'=');
		$result = $selectStmt->execute()->fetchField();
		expDebug::dPrintDBAPI('getAttemptNumber ',$selectStmt);
		expDebug::dPrint('getAttemptNumber result'.print_r($result,true), 4);
		return $result;
	} catch(Exception $e){
		expDebug::dPrint('Error in getAttemptNumber '.$e,1);
		expertusLogError($e);
	}
}

/*
 * get attendance details for an enrollment
*/
function getAttendanceSummaryDetails($uid,$course_id,$class_id,$lesson_id,$version_id,$regid,$surveyid=''){
	try{
	  $selectStmt = db_select('slt_attendance_summary', 'summ');
	  $selectStmt->addField('summ','id','summaryid');
	  $selectStmt->addField('summ','status','contentstatus');
	  $selectStmt->addField('summ','content_status','pcontentsts');
	  $selectStmt->addField('summ','completion_status','pcompletionsts');
	  $selectStmt->addField('summ','last_attempt_date','contentlastdate');
	  $selectStmt->addField('summ','score','contentscore');
	  $selectStmt->addField('summ','progress','progress');
	  //Added for db_execution #90353
	  $selectStmt->addField('summ','last_lesson_location','plast_lesson_location');
	  $selectStmt->addField('summ','launch_data','plaunch_data');
	  $selectStmt->addField('summ','suspend_data','psuspend_data');
	  $selectStmt->addField('summ','content_type','pcontent_type');
	  $selectStmt->addField('summ','cmiexit','pcmiexit');
	  $selectStmt->addField('summ','aicc_data','paicc_data');
	  $selectStmt->addField('summ','updated_by','pupdated_by');
	  $selectStmt->addField('summ','updated_on','pupdated_on');
	  $selectStmt->addField('summ','custom2','pcustom2');
	  $selectStmt->addField('summ','custom3','pcustom3');
	  $selectStmt->addField('summ','custom4','pcustom4');
	  
	  $selectStmt->condition('summ.user_id',$uid,'=');
		$selectStmt->condition('summ.course_id', $course_id,'=');
		$selectStmt->condition('summ.class_id', $class_id,'=');
		$selectStmt->condition('summ.content_version_id', $version_id,'=');
		$selectStmt->condition('summ.lesson_id', $lesson_id,'=');
		$selectStmt->condition('summ.enrollment_id', $regid,'=');
	  if(!empty($surveyid))
			$selectStmt->condition('summ.assessment_id', $surveyid,'=');
	  
	  expDebug::dPrintDBAPI('getAttendanceSummaryDetails list ',$selectStmt);
	  $result = $selectStmt->execute()->fetchAll();
	  expDebug::dPrint(' getAttendanceSummaryDetails list result'.print_r($result,true), 4);
	  return $result;
	}catch(Exception $e){
		expDebug::dPrint('Error in getAttendanceSummaryDetails '.$e,1);
		expertusLogError($e);
	}
}
/*
 * return recertify path
*/ 
function getModuleId($userId,$programId,$overall_status = array()) {
	try {  
		$selectStmt = db_select('slt_master_enrollment', 'masenroll');		
		$selectStmt->innerJoin('slt_program' ,'prg' ,'prg.id=masenroll.program_id');
		$selectStmt->addField('prg','object_type','object_type');		
		$selectStmt->addField('masenroll','overall_status','overall_status');
		$selectStmt->addField('masenroll','recertify_path','recertify_path');
		$selectStmt->addField('masenroll','comp_date', 'comp_date');
		$selectStmt->addField('prg','expires_in_value', 'expires_in_value');
		$selectStmt->addField('prg','expires_in_unit', 'expires_in_unit');
		$selectStmt->condition('masenroll.user_id',$userId);
		$selectStmt->condition('masenroll.program_id',$programId);	
		if(count($overall_status)>0) {
		$selectStmt->condition('masenroll.overall_status',$overall_status, 'IN');
		} else {			
		$selectStmt->where('masenroll.overall_status in (\'lrn_tpm_ovr_cmp\', \'lrn_tpm_ovr_enr\', \'lrn_tpm_ovr_inp\', \'lrn_tpm_ovr_ppv\', \'lrn_tpm_ovr_ppm\', \'lrn_tpm_ovr_exp\', \'lrn_tpm_ovr_wtl\')');
		}
 		$selectStmt->orderBy('masenroll.recertify_path', 'desc');
		$selectStmt->range(0, 1); 
		expDebug::dPrintDBAPI('getModuleId ::: ',$selectStmt);		
		$result = $selectStmt->execute()->fetchAll();
		expDebug::dPrint('getModuleId result'.print_r($result,true), 4);
		return $result;		
		
	} catch(Exception $e) {
		
		expDebug::dPrint('Error in getModuleId '.$e,1);
		expertusLogError($e);		
	}
}
function getCourseCntEnrollment($userId, $programId,$mandatory =''){
	try{
		$curr_path = getModuleId($userId,$programId);
		$path = $curr_path[0]->recertify_path > 0 ? $curr_path[0]->recertify_path : 1; 
		//$course_count = getGrpCourseCnt($programId,$path);
		/*course count*/
		$select = db_select('slt_module_crs_mapping', 'map');
		$select->innerJoin('slt_module' ,'mdl' ,'mdl.id = map.module_id');
		$select->addExpression('COUNT(map.id)','count');
		$select->condition('map.program_id', $programId, '=');
		if(!empty($mandatory))
			$select->condition('map.is_required', 'Y', '=');
		$select->condition('mdl.sequence', $path, '=');
		expDebug::dPrintDBAPI("enrollment count for module" ,$select);
		$coursecnt = $select->execute()->fetchField();
		return $coursecnt;
	} catch(Exception $e) {
		
		expDebug::dPrint('Error in getModuleId '.$e,1);
		expertusLogError($e);		
	}
}

/*
 * get course class cnt for each group
 * */
function getGrpCourseCnt($prgId,$moduleId,$grpId = ''){
	try{
		$select = db_select('slt_module_crs_mapping', 'map');
		$select->addExpression('COUNT(1)','count');
		$select->condition('map.program_id', $prgId, '=');
		$select->condition('map.module_id', $moduleId, '=');
		if(!empty($grpId))
			$select->condition('map.group_id', $grpId, '=');
		expDebug::dPrintDBAPI("enrollment count for module" ,$select);
		$enrcnt = $select->execute()->fetchField();
		return $enrcnt;
	}catch (Exception $ex) {
	    watchdog_exception('deleteModuleTP', $ex);
	    expertusErrorThrow($ex);
  	}
}


/* 
 * If module title is empty in API : Get Module title of Program
 */

function getModulename($programId){
	try {	
	$selectStmt = db_select('slt_module', 'module');
	$selectStmt->innerJoin('slt_program' ,'prg' ,'prg.id=module.program_id');
	$selectStmt->addField('module','title','title');
	$selectStmt->condition('module.program_id',$programId);
	$selectStmt->orderBy('module.sequence', 'desc');
	$selectStmt->range(0, 1);
	expDebug::dPrintDBAPI('getModulename ::: ',$selectStmt);
	$result = $selectStmt->execute()->fetchField();
	expDebug::dPrint('getModulename result'.print_r($result,true), 4);
	return $result;
	} catch(Exception $e) {	
		expDebug::dPrint('Error in getModulename '.$e,1);
		expertusLogError($e);
	}
} 

function FetchModuleId($programId){
	try {
		$selectStmt = db_select('slt_module', 'module');
		$selectStmt->innerJoin('slt_program' ,'prg' ,'prg.id=module.program_id');
		$selectStmt->addField('module','id','id');
		$selectStmt->condition('module.program_id',$programId);
		$selectStmt->orderBy('module.sequence', 'desc');
		$selectStmt->range(0, 1);
		expDebug::dPrintDBAPI('fetch module id : ',$selectStmt);
		$result = $selectStmt->execute()->fetchField();
		expDebug::dPrint('fetch module id result'.print_r($result,true), 4);
		return $result;
	} catch(Exception $e) {
		expDebug::dPrint('Error in fetchmoduleid '.$e,1);
		expertusLogError($e);
	}
}

function getModuleIdforSurvey($programId,$userId,$masenrollId){
	try {
	$selectStmt = db_select('slt_module', 'module');
	//$selectStmt->innerJoin('slt_survey_mapping' ,'map' ,'map.module_id=module.id');
	//$selectStmt->innerJoin('slt_program' ,'prg' ,'prg.id=map.object_id');	
	$selectStmt->innerJoin('slt_master_enrollment' ,'mas' ,'mas.program_id = module.program_id and mas.recertify_path=module.sequence');	
	$selectStmt->addField('module','id','module_id');
	//$selectStmt->condition('prg.id',$programId);
	//$selectStmt->condition('mas.user_id',$userId);
	$selectStmt->condition('mas.id',$masenrollId);
	expDebug::dPrintDBAPI('getModuleId for survey ::: ',$selectStmt);
	$result = $selectStmt->execute()->fetchField();
	expDebug::dPrint('getModuleIdforSurvey result'.print_r($result,true), 4);
	return $result;
	} catch(Exception $e) {	
		expDebug::dPrint('Error in getModuleIdforSurvey '.$e,1);
		expertusLogError($e);
	}
	
}

/*
 * return list of groups for program
*/
function getlistOfgroupsfromprogramId($programId,$moduleId,$recertify){
	try{
	  if($recertify == 1)
	  	$selectStmt = db_select('slt_module_crs_mapping', 'map');
	  else
	  	$selectStmt = db_select('slt_re_certify', 'map');
	  	
	  $selectStmt->addField('map','group_code','group_code');
	  $selectStmt->addField('map','group_title','group_title');
	  $selectStmt->condition('map.program_id',$programId);
	  $selectStmt->condition('map.module_id',$moduleId);
	  $selectStmt->groupBy('map.group_code');
  	  $result = $selectStmt->execute()->fetchAll();
	  expDebug::dPrintDBAPI('getlistOfgroupsfromprogramId',$selectStmt);
	  expDebug::dPrint('getlistOfgroupsfromprogramId result'.print_r($result,true), 4);
	  return $result;
	}catch(Exception $e){
		expDebug::dPrint('Error in getlistOfgroupsfromprogramId '.$e,1);
		expertusLogError($e);
	}
}
// testing 3
function _mime_content_type($filename) {
	$mime_types = array(
			'txt' => 'text/plain',
			'png' => 'image/png',
			'jpe' => 'image/jpeg',
			'jpeg'=> 'image/jpeg',
			'jpg' => 'image/jpeg',
			'gif' => 'image/gif',
			'pdf' => 'application/pdf',
			'doc' => 'application/msword',
			'docx'=> 'application/vnd.openxmlformats-officedocument.wordprocessingml.document',
			'xls' => 'application/vnd.ms-excel',
			'xlsx'=> 'application/vnd.openxmlformats-officedocument.spreadsheetml.sheet',
			'ppt' => 'application/vnd.ms-powerpoint',
			'pptx'=> 'application/vnd.openxmlformats-officedocument.presentationml.presentation',
			'pps' => 'application/vnd.ms-powerpoint',
			'ppsx'=> 'application/vnd.openxmlformats-officedocument.presentationml.slideshow'
	);
		
	$file_arr  = explode(".", $filename);
	$ext = strtolower(end(($file_arr)));
	return trim($mime_types[$ext]);
}
function getUserTimezone() {
	try {
		$userPHPTimezone = getUserPHPTimezoneName();
		$usertimezone = $userPHPTimezone[0]->zone_name;
		$timeZone = date_default_timezone(false);
		$defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;
		$usertimezone = empty($usertimezone)? $defaultTimezone : $usertimezone;
		return $usertimezone;
		
	}
	catch (Exception $ex) {
		watchdog_exception('getUserTimezone', $ex);
		expertusErrorThrow($ex);
	}
}
function GetIfExpired($courseID,$userId){
	try {
		$selectStmt = db_select('slt_course_template', 'crs');
		$selectStmt->innerJoin('slt_person', 'per');
		$selectStmt->addField('per','id','id');
		$selectStmt->condition('crs.id',$courseID);
		$selectStmt->condition('per.id',$userId);
		$selectStmt->isNotNull('per.hire_date');
		$selectStmt->condition('crs.compliance_completed','hire_days');
		$selectStmt->where("(CURDATE() >= date_format(date_add(per.hire_date, interval crs.complete_days+1 day),'%Y-%m-%d'))");
		expDebug::dPrintDBAPI('GetIfExpired selected',$selectStmt);
		$result = $selectStmt->execute()->fetchAll();
		expDebug::dPrint("result if expired--".print_r($result,true),1);
		return $result;
	}
	catch (Exception $ex) {
		watchdog_exception('GetIfExpired', $ex);
		expertusErrorThrow($ex);
	}
}
function CheckCourseCompExpiry($courseID){
	try {
		$selectStmt = db_select('slt_course_template', 'crs');
		$selectStmt->addField('crs','id','id');
		$selectStmt->condition('crs.id',$courseID);
		$selectStmt1 = db_or();
		$selectStmt1->where('if(crs.validity_date IS NOT NULL , crs.validity_date < curdate() , 1=2 )');
		$selectStmt1->where('if(crs.complete_date IS NOT NULL , crs.complete_date < curdate() , 1=2 )');
		$selectStmt->condition($selectStmt1);
		expDebug::dPrintDBAPI('GetIfExpired selected',$selectStmt);
		$result = $selectStmt->execute()->fetchAll();
		expDebug::dPrint("courseID if expired--".print_r($result,true),1);
		return $result;
	}
	catch (Exception $ex) {
		watchdog_exception('CheckCourseCompExpiry', $ex);
		expertusErrorThrow($ex);
	}
}

function get_drupal_ajax_settings($from_data){
	$settings = array();
	if(is_array($from_data)){
		foreach($from_data as $key=>$value){
			if(is_array($value) && array_key_exists('#attached',$value)){
				foreach($value['#attached']['js'] as $k=>$v){
					if(array_key_exists('type',$v) && $v['type'] == 'setting'){
						foreach($v['data']['ajax'] as $id=>$arr){
							$settings[$id] = $arr;
						}
					}
				}
			}
		}
	}
	expDebug::dPrint("Drupal Settings -- ".print_r($settings,true),1);
	return $settings;

}
// function moved from exp_sp_lnrsearch.inc
function getNodeActivityMapping($courseId) {
  try{
  $selectCls = db_select('slt_node_learning_activity', 'slt_nd');
  $selectCls->leftJoin('slt_course_class','cls','cls.id = slt_nd.entity_id');
  $selectCls->addField('slt_nd','node_id', 'node_id');
  $selectCls->condition('cls.course_id',$courseId,'=');
  $selectCls->condition('slt_nd.entity_type','cre_sys_obt_cls','=');
  $result = $selectCls->execute()->fetchCol();
  expDebug::dPrintDBAPI('getNodeActivityMapping query:', $selectCls);
  return $result;
  }catch (Exception $ex) {
  	watchdog_exception('getNodeActivityMapping', $ex);
  	expertusErrorThrow($ex);
  }
}

function get_waitlist_available($ClassID,$courseID){
	try {

		$waitlist = 0;

		$select = db_select('slt_enrollment', 'enr');
		$select->addExpression('COUNT(*)', 'count');
		$select->condition('enr.course_id', $courseID, '=');
		$select->condition('enr.class_id', $ClassID, '=');
		$select->condition('enr.reg_status', array('lrn_crs_reg_cnf','lrn_crs_reg_ppm'), 'IN');
		expDebug::dPrintDBAPI('get_waitlist_available count',$select);
		$tot_enroll = $select->execute()->fetchField();

		$selectStmt = db_select('slt_course_class_session', 'sess');
		$selectStmt->addField('sess','capacity_max','capacity_max');
		$selectStmt->range(0,1);
		expDebug::dPrintDBAPI('get_waitlist_available max capacity -->',$selectStmt);
		$max_capacity= $selectStmt->execute()->fetchField();

		if( $tot_enroll != 0  && !empty($max_capacity) && ($max_capacity == $tot_enroll) ){
			$waitlist =1;
		}
		expDebug::dPrint("result -->>".print_r($waitlist,true),1);
		return $waitlist;
	}

	catch (Exception $ex) {
		watchdog_exception('GetIfExpired', $ex);
		expertusErrorThrow($ex);
	}
}
function lp_mark_course_complete_class_level($enroll_id,$upduser_id,$comp_date,$score,$notifyby,$callfrom,&$statements=array()){
	try {
		$enroll_id = explode(',',$enroll_id);
		expDebug::dPrint('$$enrollResult dont sannddddlet meokkk dwn saa 11 sandhya1');
		$selectStmt = db_select('slt_enrollment', 'enr1');
		$selectStmt->leftJoin('slt_enrollment', 'enr2', 'enr1.user_id=enr2.user_id and enr1.class_id=enr2.class_id and enr1.course_id=enr2.course_id');
		$selectStmt->addField('enr1','id','id');
		$selectStmt->addField('enr1','master_enrollment_id','menrid');
		$selectStmt->addField('enr1','user_id','user_id');
		$selectStmt->addField('enr1','course_id','course_id');
		$selectStmt->addField('enr1','class_id','class_id');
		$selectStmt->condition('enr2.id',$enroll_id,'IN');
		expDebug::dPrintDBAPI('result statement',$selectStmt);
		$result = $selectStmt->execute()->fetchAll();
		expDebug::dPrint("result set".print_r($result,true),1);

		foreach($result as $list_enroll){
				
			$selectStmt = db_select('slt_enrollment', 'enr');
			$selectStmt->condition('enr.id', $list_enroll->id);
			$selectStmt->condition('enr.reg_status', array('lrn_crs_reg_ppm','lrn_crs_reg_cnf'),'IN');
			$selectStmt->where("IFNULL(comp_status,0 ) <> 'lrn_crs_cmp_cmp'");
			$selectStmt->addExpression('COUNT(1)', 'Count');
			expDebug::dPrintDBAPI(' $selectStmt notifycancnt= ' , $selectStmt);
			$cntCompStatus = $selectStmt->execute()->fetchField();

			$updateStmt = db_update('slt_enrollment');
			$updateStmt->fields(array(
					'comp_status' => 'lrn_crs_cmp_cmp',
					'progress' => 100,
					'comp_date' => !empty($comp_date)? $comp_date : NOW(),
					'score' => $score,
					'updated_by' => $upduser_id,
					'updated_on' => now(),
					'comp_by' =>  $upduser_id,
					'comp_on' => NOW()
			));
			$updateStmt->condition('id',$list_enroll->id);
			$updateStmt->condition('reg_status', array('lrn_crs_reg_ppm','lrn_crs_reg_cnf'), 'IN');
			$updateStmt->condition('class_id', $list_enroll->class_id);
			$updateStmt->where('(IFNULL(comp_status,0 ) NOT IN ("lrn_crs_cmp_cmp","lrn_crs_cmp_inc","lrn_crs_cmp_nsw"))');
			expDebug::dPrintDBAPI(' update slt_enrollment class level mark complete= ' , $updateStmt);
			//$enroll_upd = $updateStmt->execute();
			if(!empty($statements)) {
				$statements[] = $updateStmt;
			} else {
				$enroll_upd = $updateStmt->execute();
			}
				
			if(!empty($list_enroll->menrid)){
				$programid =  getProgramIdFromEnrollment($list_enroll->menrid);
				lp_mark_program_complete($list_enroll->user_id,$list_enroll->menrid,$programid,$upduser_id,$notifyby,$callfrom,'','',$statements);
			}elseif($cntCompStatus >0){
				if($notifyby=='manager')
					notificationInsert('ClsCompletedByManager',$list_enroll->id,$list_enroll->class_id,$list_enroll->user_id,$upduser_id,NULL);
				else
					notificationInsert('Completed',$list_enroll->id,$list_enroll->class_id,$list_enroll->user_id,$upduser_id,NULL);

			}

		}
	}
	catch (Exception $ex) {
		watchdog_exception('lp_mark_course_complete_class_level', $ex);
		expertusErrorThrow($ex);
	}
}
function lp_waitlist_promotion_upd($enrollid,$type,$updateuserid,$prevregstatus,&$statements=array()){
	try {
		expDebug::dPrint("result test set entered ".print_r($enrollid,true),1);
		$enrDet = getEnrollmentDetails($enrollid, array('course_id','class_id'));


		$selectStmt = db_select('slt_enrollment', 'enr');
		$selectStmt->addField('enr','id','enr_id');
		$selectStmt->addField('enr','course_id','course_id');
		$selectStmt->addField('enr','class_id','class_id');
		$selectStmt->addField('enr','user_id','user_id');
		$selectStmt->addField('enr','reg_status','reg_status');
		$selectStmt->condition('enr.class_id',$enrDet->class_id);
		$selectStmt->condition('enr.course_id',$enrDet->course_id);
		$selectStmt->condition('enr.reg_status','lrn_crs_reg_wtl','=');
		expDebug::dPrintDBAPI('result statement wait',$selectStmt);
		$result = $selectStmt->execute()->fetchAll();
		expDebug::dPrint("result set list".print_r($result,true),1);
		// $iscommerece = db_query("SELECT  isCommerce FROM system WHERE name ='exp_sp_commerce'")->fetchField();

		$isCommerce = 0;
		$xbrule5 = getProfileItemIdFromCode('lrn_cls_brl_005');

		$select =  db_select('slt_entity_profile_mapping','epm');
		$select->addExpression("if(epm.id is not null,'Yes','No')","status");
		$select->condition('epm.entity_id',$enrDet->class_id);
		$select->condition('epm.entity_type','21','=');
		expDebug::dPrintDBAPI('result brule-',$select);
		$brule5enable= $select->execute()->fetchField();
	  
		if($type == "seat_avail"){
			if($isCommerce ==1 ){
				//
				 
			}else{
				//$upd = db_query("update slt_enrollment set waitlist_flag = null,waitlist_priority=null WHERE id=$enrollid and reg_status = 'lrn_crs_reg_can'");
				$updateStmt = db_update('slt_enrollment');
				$updateStmt->fields(array('waitlist_flag' => 0,'waitlist_priority'=>null));
				$updateStmt->condition('id',$enrollid,'=');
				$updateStmt->condition('reg_status','lrn_crs_reg_can','=');
				//$updateattempts = $updateStmt->execute();
				if(!empty($statements)) {
					$statements[] = $updateStmt;
				} else {
					$upd = $updateStmt->execute();
				}
				$inc = 0;
				if($prevregstatus == 'lrn_crs_reg_wtl')
					$inc = 1;
				foreach($result as $enrlist){
					if($inc == 0 && $prevregstatus == 'lrn_crs_reg_cnf'){
						$upd = db_update('slt_enrollment');
						$updFields = array();
						if( $enrlist->reg_status == 'lrn_crs_reg_ppm' ||$enrlist->reg_status =='lrn_crs_reg_ppv' ){
							$updFields['waitlist_flag'] = NULL;
							$updFields['waitlist_priority'] = NULL;
							$updFields['updated_on'] = now();
							$updFields['updated_by'] = 1;
						}elseif($enrlist->reg_status == 'lrn_crs_reg_wtl') {
							$updFields['waitlist_flag'] = NULL;
							$updFields['waitlist_priority'] = NULL;
							$updFields['updated_on'] = now();
							$updFields['updated_by'] = 1;
							$updFields['reg_status'] = 'lrn_crs_reg_cnf';
							$updFields['comp_status'] = 'lrn_crs_cmp_enr';
						}
						$upd->fields($updFields);
						$upd->condition('id', $enrlist->enr_id);
						expDebug::dPrintDBAPI('result update slt_enrollment',$upd);
						//$did_update = $upd->execute();
						if(!empty($statements)) {
							$statements[] = $upd;
						} else {
							$did_update = $upd->execute();
						}
						notificationInsert('WaitlistPromotion',$enrlist->course_id,$enrlist->class_id,$enrlist->user_id,'Admin',$enrlist->enr_id,$statements);
						$inc++;
					}
				}
			}
		}else{
			if($isCommerce ==1 ){
				foreach($result as $enrlist){
					if( $brule5enable!='Yes') {
						notificationInsert('WaitlistAllocated',$enrlist->course_id,$enrlist->class_id,$enrlist->user_id,'Admin',NULL);
					}
				}
			}
		}
		syncSolrData('Enrollment');
	}
	catch (Exception $ex) {
		watchdog_exception('lp_waitlist_promotion_upd', $ex);
		expertusErrorThrow($ex);
	}
}
function lp_enrollment_upd($RegStatus,$regDate,$CompletionStatus,$compDate,$PaymentStatus,$paymentDate,$validateFrom,$validateTo,
		$Score='',$enrollid='',$updateuserid,$UserId='',$OrderId='',$ClassId='',$CourseId='',&$statements=array(),$solrSync =1){
	try {

		$waitlist_available = get_waitlist_available($ClassId,$CourseId);

		$select = db_select('slt_enrollment', 'enr');
		$select->addField('enr', 'reg_status');
		$select->addField('enr', 'valid_from');
		$select->condition('enr.user_id',$UserId);
		$select->condition('enr.order_id',$OrderId);
		$select->condition('enr.class_id',$ClassId);
		$select->condition('enr.course_id',$CourseId);
		expDebug::dPrintDBAPI('$result enr det--',$select);
		$prevdet = $select->execute()->fetchAll();
		 
			
		$select = db_select('slt_enrollment', 'enr');
		$select->addField('enr', 'progress');
		$select->condition('enr.user_id',$UserId);
		$select->condition('enr.id',$enrollid);
		$select->condition('enr.class_id',$ClassId);
		$select->condition('enr.course_id',$CourseId);
		expDebug::dPrintDBAPI('$result enr det progress--',$select);
		$prevprog = $select->execute()->fetchField();
		
		$select = db_select('slt_enrollment', 'enr');
		$select->addField('enr', 'reg_status');
		$select->addField('enr', 'reg_status_date');
		$select->addField('enr', 'comp_status');
		$select->addField('enr', 'comp_date');
		$select->addField('enr', 'progress');
		$select->addField('enr', 'score');
		$select->addField('enr', 'valid_from');
		$select->addField('enr', 'waitlist_flag');
		$select->addField('enr', 'waitlist_priority');
		$select->condition('enr.user_id',$UserId);
		$select->condition('enr.id',$enrollid);
		expDebug::dPrintDBAPI('$result enr details--',$select);
		$prevdetails = $select->execute()->fetchAll();
		
		$updateFields = array();
		//$OrderId = 0; /*Viswanathan commented for 75680*/
		$appendStr = 0;
		$updateQuery = db_update('slt_enrollment');
		if(!empty($RegStatus)) {
			$updateFields['reg_status'] =  $RegStatus;
			$updateFields['reg_status_date'] = $regDate;
			$appendStr = 1;
			if($RegStatus!=='lrn_crs_reg_cnf' && ($RegStatus == 'lrn_crs_reg_wtl' || $RegStatus == 'lrn_crs_reg_can') ){
				$updateFields['comp_status'] = NULL ;
				$updateFields['comp_date'] =  NULL;
				$updateQuery->where("comp_status IN ('lrn_crs_cmp_enr','lrn_crs_cmp_inp') OR comp_status IS NULL");
				$appendStr = 1;
			}
			if( $RegStatus=='lrn_crs_reg_cnf' ){
				$updateFields['comp_status'] = 'lrn_crs_cmp_enr' ;
				$updateFields['waitlist_flag'] =  NULL;
				$updateFields['waitlist_priority'] =  NULL;
				$updateQuery->condition('reg_status',array('lrn_crs_reg_can','lrn_crs_reg_ppm','lrn_crs_reg_rsv'),'NOT IN');
				$appendStr = 1;
			}
		}
			
		$notifyBy ='';
		$Progress = NULL;

		if(!empty($CompletionStatus)){
			if( $CompletionStatus=='lrn_crs_cmp_nsw_ins' ){
				$CompletionStatus = 'lrn_crs_cmp_nsw';
				$notifyBy	= 'instructor';
				$Progress	= 0;
			}
			if($CompletionStatus=='lrn_crs_cmp_cmp_ins') {
				$CompletionStatus = 'lrn_crs_cmp_cmp';
				$notifyBy	= 'instructor';
				$Progress	= 100;
			}
			if($CompletionStatus=='crs_comp_by_manager') {
				$CompletionStatus = 'lrn_crs_cmp_cmp';
				$notifyBy	= 'manager';
				$Progress	= 100;
			}

			if(!empty($prevprog) && $prevprog > 0 && $Progress != 100 ){
				$Progress =$prevprog;
			}
			$updateFields['comp_status'] =  $CompletionStatus;
			$updateFields['comp_date'] =  !empty($compDate)?$compDate :NULL ;
			$updateFields['progress'] =  $Progress;
			$updateQuery->where("(comp_status IS NULL OR comp_status NOT IN ('lrn_crs_cmp_cmp','lrn_crs_cmp_inc','lrn_crs_cmp_nsw') AND reg_status = 'lrn_crs_reg_cnf')");
			$appendStr = 1;
		}
			
		if(!empty($PaymentStatus)  && $PaymentStatus != 'cme_pmt_sts_rjt')  {
			$update = db_query("update slt_order set order_status = $PaymentStatus where id=$OrderId");
			$did_update = $update->rowCount();
			
			$result= new stdClass();
			$result->col_name = 'id';
			$result ->col_value = $OrderId;
			$result ->table_name ='slt_order';
			$result ->fields = array('order_status' => $PaymentStatus);
			$statements['update'][] = $result;
			
			if($RegStatus != 'lrn_crs_reg_wtl' ){
				$updateFields['reg_status'] =  'lrn_crs_reg_cnf';
				$updateFields['comp_status'] =  'lrn_crs_cmp_enr';
				$updateFields['comp_date'] =  NULL;
				$updateFields['reg_status_date'] =  now();
				$appendStr = 1;
			}
		}
			
		if(!empty($PaymentStatus)  && $PaymentStatus == 'cme_pmt_sts_rjt' ){
			$update = db_update('slt_order');
			$update->condition('id', $OrderId,'=');
			$update->fields(array(
					'order_status' => $PaymentStatus
			));
			$res = $update->execute();
			/* if(!empty($statements)) {
				$statements[] = $update;
			} else {
				$res = $update->execute();
			} */
			$did_update = $res->rowCount();
			
			$result= new stdClass();
			$result->col_name = 'id';
			$result ->col_value = $OrderId;
			$result ->table_name ='slt_order';
			$result ->fields = array('order_status' => $PaymentStatus);
			$statements['update'][] = $result;
			
			$updateFields['reg_status'] =  'lrn_crs_reg_can';
			$updateFields['comp_status'] =  NULL;
			$updateFields['comp_date'] =  NULL;
			$updateFields['reg_status_date'] =  now();
			$appendStr = 1;
		}
		if(!empty($validateFrom) && !empty($validateTo)) {
			$updateFields['valid_from'] =  $validateFrom;
			$updateFields['valid_from'] =  $validateTo;
			$appendStr = 1;
		}elseif(!empty($prevdet[0]->valid_from) && !empty($validateTo)  && $validateTo !='') {
			$updateFields['valid_from'] =  $validateTo;
			$appendStr = 1;
		}elseif(empty($prevdet[0]->valid_from) && !empty($validateTo)  && $validateTo !='' ){
			$updateFields['valid_from'] =  now();
			$updateFields['valid_from'] =  $validateTo;
			$appendStr = 1;
		}
		if(!empty($Score) && $Score != '') {
			$updateFields['score'] =  $Score;
			$appendStr = 1;
		}

		if($appendStr ==1){
				
			$selectStmt = db_select('slt_enrollment', 'enr');
			$selectStmt->condition('enr.id', $enrollid);
			$selectStmt->condition('enr.user_id', $UserId);
			$selectStmt->addExpression('COUNT(1)', 'Count');
			expDebug::dPrintDBAPI(' $selectStmt notifycancnt= ' , $selectStmt);
			$NotifyCanCnt = $selectStmt->execute()->fetchField();
			expDebug::dPrint('$result for  notifycancnt --'.print_r($NotifyCanCnt,true),4);
				
			$updateFields['updated_on'] =  now();
			$updateFields['updated_by'] =  $updateuserid;
			if(!empty($CompletionStatus)) {
				$updateFields['comp_on'] =  $compDate;
				$updateFields['comp_by'] =  $updateuserid;
			}
			$updateQuery->fields($updateFields);
			$updateQuery->condition('id', $enrollid);
			$updateQuery->condition('user_id', $UserId);
			expDebug::dPrintDBAPI('result update slt_enrollment',$updateQuery);
			$did_update = $updateQuery->execute();
			
			/* if(!empty($statements)) {
				$statements[] = $updateQuery;
			} else {
				$did_update = $updateQuery->execute();
			} */
			expDebug::dPrint('updatedd--> '.print_r($did_update,true),4);
			
			//Added for reverting the update when db_execution fails
			$result= new stdClass();
			$result->col_name = 'id';
			$result->col_value = $enrollid;
			$result->table_name ='slt_enrollment';
			$result->fields =array(
					'reg_status' => $prevdetails[0]->reg_status,
					'reg_status_date' => $prevdetails[0]->reg_status_date,
					'comp_status' => $prevdetails[0]->comp_status,
					'comp_date' => $prevdetails[0]->comp_date,
					'waitlist_flag' => $prevdetails[0]->waitlist_flag,
					'waitlist_priority' => $prevdetails[0]->waitlist_priority,
					'progress' => $prevdetails[0]->progress,
					'score' => $prevdetails[0]->score,
					'valid_from' => $prevdetails[0]->valid_from
			);
			$statements['update'][] = $result;
		}

		if($did_update > 0) {
			if($RegStatus == 'lrn_crs_reg_cnf'){
				$waitlist_available = get_waitlist_available($ClassId,$CourseId);
				$del_type = getClassDeliveryType($ClassId ,'Long');

				if( $del_type == 'lrn_cls_dty_ilt' ||  $del_type == 'lrn_cls_dty_vcl' )
					notificationInsert('Register', $CourseId, $ClassId, $UserId, $updateuserid, $enrollid,$statements);
				else
					notificationInsert('Register', $CourseId, $ClassId, $UserId, $updateuserid, NULL,$statements);
					
				if($waitlist_available == 1){
					lp_waitlist_promotion_upd($enrollid,'seat_occupied',$updateuserid,$prevRegStatus,$statements);
				}
			}

			if( $PaymentStatus == "'cme_pmt_sts_cnm'"){

				$select = db_select('slt_order','odr');
				$select->addField('odr', 'uc_order_id');
				$select->condition('odr.id', $OrderId);
				$ucOrderId = $select->execute()->fetchField();
					
				$selectStmt = db_select('slt_notification', 'note');
				$selectStmt->condition('note.msg_id', 'pay_confirm_admin','=');
				$selectStmt->condition('note.user_id', $UserId);
				$selectStmt->where("LOCATE(CONCAT('order_id>|',$ucOrderId),token_str) > 0 AND TIMESTAMPDIFF(MINUTE ,created_on,NOW() ) < 5");
				$selectStmt->addExpression('COUNT(1)', 'Count');
				expDebug::dPrintDBAPI(' $selectStmt notifycancnt= ' , $selectStmt);
				$NotiCnt = $selectStmt->execute()->fetchField();
				expDebug::dPrint('$result for  noticnt --'.print_r($NotiCnt,true),4);

				if($NotiCnt == 0) {
					$del_type = getClassDeliveryType($ClassId ,'Long');
					if( $del_type == 'lrn_cls_dty_ilt' ||  $del_type == 'lrn_cls_dty_vcl' )
						notificationInsert('PaymentRegister',$enrollid,$ClassId,$UserId,$updateuserid,$enrollid,$statements);
					else
						notificationInsert('PaymentRegister',$enrollid,$ClassId,$UserId,$updateuserid,NULL,$statements);
						
				}
			}
				
			if($RegStatus == 'lrn_crs_reg_can' || $RegStatus == 'lrn_crs_reg_rjt'){
				$del_type = getClassDeliveryType($ClassId ,'Long');
					
				if( $del_type == 'lrn_cls_dty_ilt' ||  $del_type == 'lrn_cls_dty_vcl' ){
					if($NotifyCanCnt == 0) {
						notificationInsert('Cancel',$enrollid,$ClassId,$UserId,$updateuserid,$enrollid,$statements);
					}
				}elseif( $del_type =='lrn_cls_dty_wbt' || $del_type =='lrn_cls_dty_vod'){
					notificationInsert('WBTCancel',$enrollid,$ClassId,$UserId,$updateuserid,NULL,$statements);
				}

				if($RegStatus == 'lrn_crs_reg_can') {
					notificationInsert('CancelLocationAdmin',$enrollid,$ClassId,$UserId,$updateuserid,NULL,$statements);
				}
				if($waitlist_available == 1 && ($prevdet[0]->reg_status =='lrn_crs_reg_cnf' || $prevdet[0]->reg_status =='lrn_crs_reg_wtl')) {
					lp_waitlist_promotion_upd($enrollid,'seat_avail',$updateuserid,$prevRegStatus,$statements);
				}
			}
				
			if($CompletionStatus == 'lrn_crs_cmp_cmp'){
				if($notifyBy == 'manager'){
					notificationInsert('ClsCompletedByManager',$enrollid,$ClassId,$UserId,$updateuserid,NULL,$statements);
				}elseif ($notifyBy == 'instructor') {
					notificationInsert('ClsCompletedByInstructor',$enrollid,$ClassId,$UserId,$updateuserid,NULL,$statements);
				}else{
					notificationInsert('Completed',$enrollid,$ClassId,$UserId,$updateuserid,NULL,$statements);
				}
				lp_mark_course_complete_class_level($enrollid,$updateuserid,$compDate,$Score,$notifyBy,'enrollment_upd',$statements);
			}
				
			if( $CompletionStatus == 'lrn_crs_cmp_nsw' ){
				notificationInsert('ClsCompletedByInstructorAsNSW',$enrollid,$ClassId,$UserId,$updateuserid,NULL,$statements);
				$enrdet =  getEnrollmentDetails($enrollid, array('master_enrollment_id','class_id'));
				$master_enroll_id =  $enrdet->master_enrollment_id ;
				if($master_enroll_id>0){
					$program_id = getProgramIdFromEnrollment($master_enroll_id);
					lp_mark_program_complete($UserId,$master_enroll_id,$program_id,$updateuserid,$notifyBy,'',$statements);
				}
			}
				
			if($RegStatus == 'lrn_crs_reg_rjt' )
				notificationInsert('Rejected',$CourseId,$ClassId,$UserId,$updateuserid,NULL,$statements);
				
				
			if($RegStatus == 'lrn_crs_reg_wtl' ){
				notificationInsert('Waitlist',$CourseId,$ClassId,$UserId,$updateuserid,NULL,$statements);
				notificationInsert('WaitlistLocationAdmin',$CourseId,$ClassId,$UserId,$updateuserid,NULL,$statements);
			}
				
			if( $PaymentStatus == 'cme_pmt_sts_rjt' ){
				
				$select = db_select('slt_order','odr');
				$select->addField('odr', 'uc_order_id');
				$select->condition('odr.id', $OrderId);
				$ucOrderId = $select->execute()->fetchField();
				 
				 
				$selectStmt = db_select('slt_notification', 'note');
				$selectStmt->condition('note.msg_id', 'pay_checkout_rejected','=');
				$selectStmt->condition('note.user_id', $UserId);
				$selectStmt->where("LOCATE(CONCAT('order_id>|',$ucOrderId),token_str) > 0 AND TIMESTAMPDIFF(MINUTE ,created_on,NOW() ) < 5");
				$selectStmt->addExpression('COUNT(1)', 'Count');
				expDebug::dPrintDBAPI(' $selectStmt notifycnt= ' , $selectStmt);
				$NotiCnt = $selectStmt->execute()->fetchField();
				if( $NotiCnt == 0 ){
					notificationInsert('pay_checkout_rejected',$enrollid,$ClassId,$UserId,$updateuserid,NULL,$statements);
				}
			}
		}
		if($solrSync== 1){
		  syncSolrData('Enrollment');
		}
       syncSolrData('User');
		
		return $did_update;
	}
	catch (Exception $ex) {
		watchdog_exception('GetIfExpired', $ex);
		expertusErrorThrow($ex);
	}
}
function lp_roster_upd_wrapper($program_id,$course_id,$class_id,$enroll_id,$upduser_id,$reg_status,$reg_status_dt,$comp_status,$comp_date,$score,$grade){
	try {
		
		$flag = 0;
		$errormsg = 'Successful';
				$rst = 0;
				if(!empty($reg_status)  && $reg_status!='' && $reg_status != 'lrn_crs_reg_cnf' &&  !empty($comp_status) ){
		 	$flag=1;
		 	$errormsg = "Invalid completion status.";
		}elseif( !empty($comp_status) && $comp_status != 'lrn_crs_cmp_cmp' && $comp_status != 'crs_comp_by_manager' && (!empty($score)  || !empty($grade)) ) {
		$flag=1;
		   $errormsg = "Invalid score/grade.";
		}elseif (!empty($reg_status_dt)  && !empty($comp_date)  && $reg_status_dt > $comp_date){
		$flag=1;
		    $errormsg = "Invalid date range";
		}
		if( $flag==0) {
		$result =	lp_mark_course_complete($program_id,$course_id,$class_id,$enroll_id,$upduser_id,$reg_status,$reg_status_dt,$comp_status,$comp_date,$score);
		    }
		
		    return $result;
	}
	catch (Exception $ex) {
	watchdog_exception('lp_roster_upd_wrapper', $ex);
			expertusErrorThrow($ex);
	}
}
function lp_mark_course_complete($program_id,$course_id,$class_id,$enroll_id,$upduser_id,$reg_status,$reg_status_dt,$comp_status,$comp_date,$score,$grade){
	try {

	if($comp_status=='crs_comp_by_manager') {
			$notifyby ='manager';
			$comp_status = 'lrn_crs_cmp_cmp';
		}
		$master_enrollment_id ='';
		if(!empty($program_id)&& !empty($course_id )) {
		$master_enrollment_id =	db_query("SELECT GROUP_CONCAT(master_enrollment_id)  FROM slt_enrollment WHERE FIND_IN_SET(id, $enroll_id)>0")->fetchField();
		}elseif(!empty($program_id) && empty($course_id)){
		$master_enrollment_id = $enroll_id;
		$enroll_id = db_query("SELECT GROUP_CONCAT(id) FROM slt_enrollment WHERE FIND_IN_SET(master_enrollment_id, $enroll_id)>0")->fetchField();
		}
		$updateQuery = db_update('slt_enrollment');
		$updateFields = array();

		if(!empty($program_id)  && !empty($master_enrollment_id) ) {
		$updateQuery->where("(find_in_set(master_enrollment_id,$master_enrollment_id) > 0)");
		}
		if(!empty($course_id ) ){
		$updateQuery->condition('course_id',$course_id);
		}
				if(!empty($class_id ) ){
				$updateQuery->condition('class_id',$class_id);
		}
		if(!empty($reg_status) && $reg_status!='') {
		$updateFields['reg_status'] =  $reg_status;
				if($reg_status=='lrn_crs_reg_can' ){
			$updateFields['comp_status'] =  NULL;
					$updateQuery->where("(comp_status IN ('lrn_crs_cmp_enr','lrn_crs_cmp_inp') OR comp_status IS NULL )");
						}
						if( $reg_status=='lrn_crs_reg_cnf' ){
							$updateFields['waitlist_flag'] =  NULL;
							$updateFields['waitlist_priority'] =  NULL;
							$updateQuery->condition('reg_status',array('lrn_crs_reg_can','lrn_crs_reg_ppm','lrn_crs_reg_rsv'),'NOT IN');
						}
						if( empty($comp_status)  || $comp_status!='lrn_crs_cmp_cmp') {
						$updateQuery->where("(IFNULL(comp_status,0)!='lrn_crs_cmp_cmp' )");
						$updateQuery->condition('comp_status',array('lrn_crs_cmp_cmp','lrn_crs_cmp_inc','lrn_crs_cmp_nsw'),'NOT IN');
						$updateQuery->condition('reg_status','lrn_crs_reg_cnf','=');
	}
	}
	if(!empty($reg_status_dt)){
			$updateFields['reg_status_date'] =  $reg_status_dt;
			if(empty($comp_status) || $comp_status!='lrn_crs_cmp_cmp') {
			$updateQuery->where("(IFNULL(comp_status,0)!='lrn_crs_cmp_cmp' )");
			$updateQuery->condition('comp_status',array('lrn_crs_cmp_cmp','lrn_crs_cmp_inc','lrn_crs_cmp_nsw'),'NOT IN');
			$updateQuery->condition('reg_status','lrn_crs_reg_cnf','=');
	}
	}


	if(!empty($comp_status) ) {
	$updateFields['comp_status'] =  $comp_status;
	$updateQuery->condition('comp_status',array('lrn_crs_cmp_cmp','lrn_crs_cmp_inc','lrn_crs_cmp_nsw'),'NOT IN');
	if(empty($reg_status)  || $reg_status!='lrn_crs_reg_cnf' ){
	$updateQuery->condition('reg_status','lrn_crs_reg_cnf','=');
	}
	if($comp_status=='lrn_crs_cmp_cmp' && empty($comp_date)){
		$updateFields['comp_date'] =  now();
		$updateFields['comp_on'] =  now();
		$updateFields['comp_by'] =  $upduser_id;

	}
	}

	if( !empty($comp_date )){
		$updateFields['comp_date'] =  $comp_date;
		$updateFields['comp_on'] =  $comp_date;
		$updateFields['comp_by'] =  $upduser_id;
		if( empty($reg_status)  || $reg_status!='lrn_crs_reg_cnf' ){
			$updateQuery->condition('reg_status','lrn_crs_reg_cnf','=');
		}
	}

	if(!empty($score)) {
		$updateFields['score'] =  $score;
		if(empty($comp_status) ||  $comp_status!='lrn_crs_cmp_cmp') {
			$updateQuery->condition('comp_status','lrn_crs_cmp_cmp','=');
		}
	}
	if( empty($reg_status) || ($reg_status != 'lrn_crs_reg_can' && $reg_status != 'lrn_crs_reg_cnf')) {
		$updateQuery->isNotNull('course_id');
	}
	if($comp_status =='lrn_crs_cmp_cmp') {
		$progress = 100;
		$updateFields['progress'] =  $progress;
	}

	$updateFields['updated_by'] =  $upduser_id;
	$updateFields['updated_on'] =  now();
	$updateQuery->fields($updateFields);
	$did_update = $updateQuery->execute();
	expDebug::dPrintDBAPI('result statement',$updateQuery);
	//expDebug::dPrint('$result updated '.print_r($did_update,true),4);

	expDebug::dPrint('$result check $reg_status '.print_r($reg_status,true),4);

	if( !empty($reg_status) && $reg_status!='' && $reg_status == 'lrn_crs_reg_can') {
		lp_mark_program_roster_complete($enroll_id,$master_enrollment_id,$program_id,$upduser_id);
			
		$role_user_id = db_query("SELECT GROUP_CONCAT(user_id) FROM slt_master_enrollment WHERE  FIND_IN_SET(id,$master_enrollment_id) >0")->fetchField();
		$role_map_id = db_query("SELECT GROUP_CONCAT(id)  FROM slt_object_role_mapping  WHERE object_id=$program_id AND object_type = 'cre_sys_obt_trp' AND FIND_IN_SET(user_id,$role_user_id)>0")->fetchField();

		db_query("delete from slt_attendance_summary where enrollment_id = :enrollment_id",array("enrollment_id"=>$enrollid));
		// db_query("DELETE FROM slt_object_role_mapping WHERE FIND_IN_SET(id,$role_map_id)>0;");
		$result = 1;
	}elseif( !empty($reg_status)  && $reg_status == 'lrn_crs_reg_cnf' ){
			
		$updateQuery1 = db_update('slt_master_enrollment');
		$fields = array(
				'overall_status'    => 'lrn_tpm_ovr_enr',
				'updated_by'  => $upduser_id,
				'updated_on'  => now()
		);
		$updateQuery1->fields($fields);
		$updateQuery1->condition('overall_status','lrn_tpm_ovr_cmp','!=');
		$updateQuery1->where("(FIND_IN_SET(id,$master_enrollment_id)>0)");
			
		expDebug::dPrintDBAPI(' update slt_master_enrollment ---= ' , $updateQuery1);
		$details_upd = $updateQuery1->execute();

	}elseif($comp_status=='lrn_crs_cmp_cmp')  {
		expDebug::dPrint('$$enrollResult dont sannddddlet meokkk dwn saa 1113333');
		lp_mark_course_complete_class_level($enroll_id,$upduser_id,$comp_date,$score,$notifyby,'program');
	}
	syncSolrData('Enrollment');
	syncSolrData('MasterEnrollment');
	syncSolrData('User');
	return $did_update;
	}
	catch (Exception $ex) {
		watchdog_exception('lp_mark_course_complete', $ex);
		expertusErrorThrow($ex);
	}
}
function lp_mark_program_roster_complete($user_id,$master_enrollment_id,$program_id,$upduser_id){
	try {
		$master_enrollment_id_Array = explode(',', $master_enrollment_id);
		$masterEnrolledIdsCount = count($master_enrollment_id_Array);
		for($i = 0; $i < $masterEnrolledIdsCount; $i++){
			$cur_id = $master_enrollment_id_Array[$i];
			if($cur_id != ''){
				$xxuesr_id = db_query("SELECT user_id FROM slt_master_enrollment WHERE id = $cur_id AND
						overall_status NOT IN ('lrn_tpm_ovr_cmp','lrn_tpm_ovr_cln') AND program_id = $program_id AND comp_date IS NULL" )->fetchField();

				$selectQuery = db_select('slt_enrollment', 'e');
				$selectQuery->addField('e', 'master_enrollment_id' ,'menrid');
				$selectQuery->addField('e', 'user_id' ,'master_user_id');
				$selectQuery->addExpression('CASE WHEN (e.reg_status="rn_crs_reg_can" OR (e.reg_status="lrn_crs_reg_cnf" AND e.comp_status="lrn_crs_cmp_cmp")) THEN 0 ELSE 1 END' ,'comp_status');
				$selectQuery->where("FIND_IN_SET(master_enrollment_id, $cur_id) >0");
				$selectQuery->groupby('e.master_enrollment_id');
				$selectQuery->groupby('e.user_id');
				$selectEnrollResult = $selectQuery->execute()->fetchAll();

				$updateresult = 	db_query("UPDATE slt_master_enrollment AS me
						SET overall_status = CASE WHEN '.$selectEnrollResult[0]->comp_status.' = 0 THEN 'lrn_tpm_ovr_cln' ELSE overall_status END
						,   cancel_date  = CASE WHEN '.$selectEnrollResult[0]->comp_status.' = 0 THEN NOW() ELSE cancel_date END
						,   updated_by = CASE WHEN '.$selectEnrollResult[0]->comp_status.' = 0 THEN $upduser_id ELSE updated_by END
						,   updated_on = CASE WHEN '.$selectEnrollResult[0]->comp_status.' = 0 THEN NOW() ELSE updated_on END
						WHERE me.id = '.$selectEnrollResult[0]->menrid.'
						AND   me.user_id = '.$selectEnrollResult[0]->master_user_id.'
						AND   me.overall_status NOT IN ('lrn_tpm_ovr_cln','lrn_tpm_ovr_cmp')
						AND   me.program_id = '.$program_id.'
						AND   me.comp_date IS NULL
						");
					
			}
			expDebug::dPrintDBAPI(' update slt_master_enrollment ---= ' , $updateresult);
		}
			
	}
	catch (Exception $ex) {
	watchdog_exception('lp_mark_program_roster_complete', $ex);
	expertusErrorThrow($ex);
	}
}
function getShareModuleStatus($criteria){
	try{
		$result='';
		include_once $_SERVER["DOCUMENT_ROOT"]. "/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_sitesetup/exp_sp_administration_module_info/exp_sp_administration_module_info.inc";
		$select = db_select('slt_profile_list_items', 'spli');
		$select->condition('spli.code', 'cre_ste_mod_shr', '=');
		$select->addField('spli', 'name', 'module_name');
		$select->addField('spli', 'is_active', 'module_status');
		expDebug::dPrintDBAPI(' $select obj = ' , $select);
		$result = $select->execute()->fetchAll();
		$count_details = count($result);
		$status = 2;
		if(!empty($result)){
			$status = ($result[0]->module_status == 'Y') ? 1 : 2;
		}


		if($criteria == 'api'){
			$select = db_select('variable', 'vari');
			$select->condition('vari.name', 'share_email', '=');
			$select->addExpression('SUBSTRING(vari.value,3,1)','email');
			$result = $select->execute()->fetchAll();
			$shareEmail = $result[0]->email;
			$select = db_select('variable', 'vari');
			$select->condition('vari.name', 'share_embed', '=');
			$select->addExpression('SUBSTRING(vari.value,3,1)','embed');
			$result = $select->execute()->fetchAll();
			$shareEmbed = $result[0]->embed;
		}else{
			$shareEmail = variable_get('share_email',0);
			$shareEmbed =  variable_get('share_embed',0);
		}

		expDebug::dPrint(' getShareModuleStatus $status = ' . $status,5);
		expDebug::dPrint(' getShareModuleStatus $shareEmail = ' . $shareEmail,5);

		if($criteria == 'user_points' || $criteria == 'profile' || $criteria == 'api' || $criteria == 'mylearning' )
		{
			if(($status == 1 && $shareEmail == 1)){
				return true;
			}
			return false;
		}
		elseif($criteria == 'catalog') {
			$loggedInUserId = getIdOfLoggedInUser();
			$checkIsAdmin = ifadminuser($loggedInUserId);			        
         	$checkIsManager=is_manager($loggedInUserId);
         	$checkIsInstructor=is_instructor($loggedInUserId);
			if($status==1 && $shareEmail==0 && $shareEmbed==1 && (($checkIsManager==1 && $checkIsAdmin == 1)||($checkIsInstructor==1 && $checkIsAdmin ==1)|| ($checkIsManager==1 && $checkIsInstructor==1 && $checkIsAdmin ==2))){
			return false;
            }
			if($status == 1 && ($shareEmail == 1 || $shareEmbed == 1 ) && $checkIsAdmin > 0 )
			{
				return true;
			}
			elseif($status == 1 && $shareEmail == 1 && $checkIsAdmin == 0) 
			{
				return true;
			}
			return false;
		}elseif($criteria=='class_details' || $criteria=='course_details' || $criteria=='lrnplan_details') {
			if($status == 1 && $shareEmail == 1)
				return true;
			else
				return false;
		}
		return array('status'=>$status,'share_email'=>$shareEmail,'share_embed'=>$shareEmbed);
	} catch (Exception $ex) {
		watchdog_exception('getShareModuleStatus', $ex);
		expertusErrorThrow($ex);
	}
}

//#custom_attribute_0078975
function getcustomattributemodulestatus()
{
    $resultcustom='';
    //include_once $_SERVER["DOCUMENT_ROOT"]. "/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_sitesetup/exp_sp_administration_module_info/exp_sp_administration_module_info.inc";
    $select = db_select('system', 'spli');
    $select->condition('spli.name', 'exp_sp_administration_customattribute', '=');
    $select->addField('spli', 'status', 'module_status');
    expDebug::dPrintDBAPI(' $select obj = ' , $select);
    $resultcustom = $select->execute()->fetchAll(); 
    $status = ($resultcustom[0]->module_status == 1) ? 1 : 0;
    if($status == 1)
        return true;
    else
        return false;
    
}
function get_progress($launch_detail, $enr_id, $callforupdate = 0) {
	try{
		include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_survey/exp_sp_survey_learner/exp_sp_surveylearner.inc');
		$sum =0;
		$total = 0;
		$progress = 0;

		$selectStmt = db_select('slt_enrollment', 'enr');
		$selectStmt->leftJoin('slt_course_class','scc','enr.class_id=scc.id');
		$selectStmt->leftJoin('slt_profile_list_items','spli','scc.delivery_type=spli.code');
		$selectStmt->condition('enr.id', $enr_id);
		$selectStmt->addField('enr', 'master_enrollment_id', 'master_enrollment_id');
		$selectStmt->addField('spli', 'attr1', 'basetype');
		$selectStmt->addField('enr', 'user_id', 'user_id');
		$selectStmt->addField('enr', 'course_id', 'course_id');
		$selectStmt->addField('enr', 'class_id', 'class_id');
		$selectStmt->addField('scc', 'title', 'class_title');
		expDebug::dPrintDBAPI('$selectStmt = ', $selectStmt);
		$result =$selectStmt->execute()->fetchObject();

		expDebug::dPrint("Enrollment details ---->".print_r($result,1),4);
		$objectType = 'cre_sys_obt_cls';

		$enrCntDetail = array();
		//pre assesment
		$enrCntDetail['pre_ass_detail'] 	=  getAssignedSurveys($result->user_id, $result->class_id, $objectType, 'assessment','1', $enr_id, 'yes');
		$enrCntDetail['post_ass_detail'] 	=  getAssignedSurveys($result->user_id, $result->class_id, $objectType, 'assessment','0', $enr_id, 'yes');
		$enrCntDetail['survey_detail'] 		=  getAssignedSurveys($result->user_id, $result->class_id, $objectType, 'survey', '0', $enr_id, 'yes');

		expDebug::dPrint('Final enr content details: ' . print_r($enrCntDetail, 1), 4);

		if(!empty($enrCntDetail['pre_ass_detail'][0]->surveyid)){
			//pre and post assessment summary details
			$percentage = getSummaryDetails($result->user_id,$enrCntDetail['pre_ass_detail'][0]->objectid,$enrCntDetail['pre_ass_detail'][0]->surveyid,$enr_id,'sry_det_typ_ass');
			$total = $total+1;
			$sum+= $percentage;
		}

		if(!empty($enrCntDetail['post_ass_detail'][0]->surveyid)){
			$percentage = getSummaryDetails($result->user_id,$enrCntDetail['post_ass_detail'][0]->objectid,$enrCntDetail['post_ass_detail'][0]->surveyid,$enr_id,'sry_det_typ_ass');
			$total = $total+1;
			$sum+= $percentage;
		}

		/* if(!empty($enrCntDetail['survey_detail'])){
			foreach ($enrCntDetail['survey_detail'] as $key =>$survey_details){
			$percentage = getSummaryDetails($result->user_id,$survey_details->objectid,$survey_details->surveyid,$enr_id,'sry_det_typ_sry');
			$total = $total+1;
			$sum+= $percentage;
			}
			} */

		expDebug::dPrint("total content to Launch Details -->".print_r($total,true),4);

		if($launch_detail[0]['LaunchType'] == 'WBT' || $launch_detail[0]['LaunchType'] == 'VOD') {
			$totalcontent = count($launch_detail);
			foreach ($launch_detail as $details){
				$percentage = $details['progress'];
				$sum+= $percentage;
			}
		}else {
			$totalcontent = 0;
		}
		expDebug::dPrint("launch detail check total content check".$totalcontent,5);
		$totalcontent = $total+$totalcontent;
		expDebug::dPrint("Launch Details -->".print_r($launch_detail,true),5);

		expDebug::dPrint('launch details percentage: ' . $percentage, 4);
		expDebug::dPrint('launch details total: ' . $totalcontent, 4);
		expDebug::dPrint('launch details sum: ' . $sum, 4);

		$progress = $sum/$totalcontent;
		expDebug::dPrint('launch details progress: ' . $progress, 4);

		if($callforupdate==1)
			return round($progress);
		$progress = convertProgressValue(round($progress));
		expDebug::dPrint("get progress".$progress);
		return $progress;
	}catch (Exception $ex) {
		watchdog_exception('get_progress', $ex);
		expertusErrorThrow($ex);
	}
}
//73059: Content completion % is not getting updated unless we close the window and reopen it or move to the next content				
function getContentMasterId($VersionId){
	$selectStmt = db_select('slt_content_version', 'scv');
	$selectStmt->condition('scv.id', $VersionId,'=');
	$selectStmt->addField('scv', 'content_master_id','content_id');
	expDebug::dPrintDBAPI(' $selectStmt slt_content_version= ' , $selectStmt);
	return $content_id = $selectStmt->execute()->fetchField();
}
function getMultiLessonDetails($VersionId,$lunObj=''){
	$selectStmt = db_select('slt_content_lesson', 'scl');
	$selectStmt->condition('scl.content_version_id', $VersionId,'=');
	// Select fields
	//$selectStmt->addExpression('COUNT(scl.id)', 'count');
	$selectStmt->addField('scl', 'id','lesson_id');
	expDebug::dPrintDBAPI(' $selectStmt obj = ' , $selectStmt);
	// Execute query
	$result = $selectStmt->execute()->fetchall();
	expDebug::dPrint(' $rowCount = ' . print_r($result, true),3);
	$tot_progress = 0;
	$cnt = count($result);
	if($cnt > 1){
		foreach ($result as $val){
			$sum_details = getAttendanceSummaryDetails($lunObj->userId,$lunObj->CourseId,$lunObj->ClassId,$val->lesson_id,$VersionId,$lunObj->RegId);
			$tot_progress = $tot_progress + $sum_details[0]->progress;
		}
		$tot_progress = $tot_progress/$cnt;
	}
	return $tot_progress;
}
function get_attendance_summary($enrollment_id, $course_id, $class_id, $lesson_id, $content_version_id, $assessment_id = NULL) {
	try{
		$select = db_select('slt_attendance_summary', 'summ');
		$select->addField('summ', 'suspend_data','suspend_data');
		$select->addField('summ', 'progress','progress');
		$select->condition('summ.enrollment_id' , $enrollment_id);
		$select->condition('summ.course_id' , $course_id);
		$select->condition('summ.class_id' , $class_id);
		$select->condition('summ.lesson_id' , $lesson_id);
		$select->condition('summ.content_version_id' , $content_version_id);
		if($assessment_id>0)
			$select->condition('summ.assessment_id' , $assessment_id);
		expDebug::dPrintDBAPI('select attn summary ', $select);
		$prev_attn_summary = $select->execute()->fetchAll();
		return isset($prev_attn_summary[0]) ? $prev_attn_summary[0] : null;
	} catch (Exception $ex) {
		watchdog_exception('get_attendance_summary', $ex);
		expertusErrorThrow($ex);
	}
}
function getIfCourseMandatory($courseId,$userId,$classId){
	try {
		$isMandatory = db_query('select mandatory from slt_enrollment enr
							where
							enr.course_id = '.$courseId.' and enr.class_id ='.$classId.' and enr.user_id='.$userId)->fetchField();
		expDebug::dPrint("launch detail check dddddddd content check".$isMandatory,5);
		return $isMandatory;
	} catch (Exception $ex) {
		watchdog_exception('getIfCourseValid', $ex);
		expertusErrorThrow($ex);
	}
}
function getContentCompletionStatus($object, $lessonStatus) {
	try {
		$statuses = array('completed', 'passed'); //'failed'
		$completeStatuses = array('completed', 'passed', 'failed');
		//select count(1) from slt_content_lesson where content_version_id =3
		$countQuery = db_select('slt_content_lesson', 'les');
		$countQuery->addField('les', 'id','lesson_id');
		$countQuery->condition('les.content_version_id', $object->VersionId, '=');
		expDebug::dPrintDBAPI('count query value ', $countQuery);
		$results = $countQuery->execute()->fetchCol();
		$lessonCount = count($results);
		expDebug::dPrint('lesson count value:' . $lessonCount, 4);
		$result = array(
				contentStatus => false,
				completionStatus => false,
		);
		if ($lessonCount > 1) {
			$completedCount = 0;
			$summaryQuery = db_select('slt_attendance_summary', 'sum');
			$summaryQuery->addField('sum', 'content_status','contentQuizStatus');
			$summaryQuery->addField('sum', 'content_type','ContentType');
			$summaryQuery->addField('sum', 'completion_status','ContentCompletionStatus');
			$summaryQuery->condition('sum.enrollment_id', $object->RegId, '=');
			$summaryQuery->condition('sum.class_id', $object->ClassId, '=');
			$summaryQuery->condition('sum.content_version_id', $object->VersionId, '=');
			expDebug::dPrintDBAPI('count summary query value ', $summaryQuery);
			$summaryResults = $summaryQuery->execute()->fetchAll();
				
			expDebug::dPrint('all summary results of the content:' . print_r($summaryResults, 1), 4);
				
			if (count($summaryResults) != $lessonCount) { // if summary count mismatced with lesson count, return false
				return $result;
			}
				
			foreach ($summaryResults as $launch) { // validate lesson status on each lesson
				$success_status = $launch->contentQuizStatus;
				if(( $launch->ContentCompletionStatus == 'completed' || $launch->ContentCompletionStatus == '') && $launch->ContentType != 'Knowledge Content') {
					$completedCount++;
				}
			}
				
			if ($lessonCount == $completedCount) { // All lesson in the content as completed state.
				$result['contentStatus'] = true;
				$result['completionStatus'] = true;
			}
			return $result;
		} else {
			$contentStatus = strtolower(trim($object->ContentStatus));
			if (($object->contentCode  == 'lrn_cnt_typ_srm_s12' || $object->contentCode  == 'lrn_cnt_typ_aic') && $lessonStatus == '') {
				$result['contentStatus'] = true;
				$result['completionStatus'] = true;
			} else if ($object->contentCode  == 'sry_det_typ_ass') {
				$result['contentStatus'] = in_array($contentStatus, $statuses);
				$result['completionStatus'] = in_array($contentStatus, $completeStatuses);
			} else if ($object->contentCode  == 'lrn_cnt_typ_vod') {
				$result['contentStatus'] = in_array($contentStatus, $statuses);
				$result['completionStatus'] = in_array($contentStatus, $completeStatuses);
			} else {
				$result['contentStatus'] = in_array($contentStatus, $statuses);
				$result['completionStatus'] = in_array($lessonStatus, $completeStatuses);
			}
			return $result;
		}
	} catch(Exception $ex) {
		watchdog_exception('getContentCompletionStatus', $ex);
		expertusErrorThrow($ex);
	}
}

function calculate_offset($page = NULL, $rows = NULL) {
	try {
		if(isset($_REQUEST['jqgrid_row_id'])) {
			// start should always be 0 when a pariticular row of jqgrid needs to be updated
			return 0;
		}
		if(is_null($page) || is_null($rows)) {
			$page = $_REQUEST['page'];
			$rows = $_REQUEST['rows'];
		}
		// $start = (($page - 1) * $rows) + 1;
		$start = ($rows * $page) - $rows;
		expDebug::dPrint('calculate_offset start '.$start.print_r(func_get_args(), 1), 4);
		return $start;
	} catch(Exception $ex) {
		expDebug::dPrint('calculate_offset exception '.$ex->getMessage(), 1);
		watchdog_exception('calculate_offset', $ex);
		expertusErrorThrow($ex);
	}
}
function add_row_id_condition(&$select, $table_alias, $table_field = NULL, $field_value = NULL) {
	try {
		if(is_null($field_value)) {
			$field_value = $_REQUEST['jqgrid_row_id'];
		}
		if(is_null($table_field)) {
			$table_field = 'id';
		}
		if(!is_null($field_value)) {
			$select->condition($table_alias . '.' . $table_field, $field_value, '=');
		}
	} catch (Exception $ex) {
		expDebug::dPrint('exception in add_row_id_condition method '.$ex->getMessage(), 1);
	}
}

function isPostAssessmentAttached($objectId,$objectType){
	if($objectType == "class"){
		$object_type = "('cre_sys_obt_cls')"	;
	}else{
		$object_type = "('cre_sys_obt_crt','cre_sys_obt_cur','cre_sys_obt_trn')"	;
	}
	$isPostAssessmentAttached = db_query("select count(id) as count from slt_survey_mapping ssm  where ssm.pre_status = 0  and ssm.object_type in ".$object_type." and ssm.object_id = ".$objectId)->fetchField();
	expDebug::dPrint('isPostAssessmentAttached' . print_r($exists, 1), 4);
	return $isPostAssessmentAttached;
}

function isProgramLaunchable($masterenrollid){
	$program_id = db_query("select program_id from `slt_master_enrollment` where id =".$masterenrollid)->fetchField();
	$isPostAssessmentAttached = isPostAssessmentAttached($program_id,'tp');
	expDebug::dPrint('isProgramLaunchable isPostAssessmentAttached' . $isPostAssessmentAttached, 4);
	$isLaunchableClass = db_query("select if(sum(launchable) > 0,1,0) as launchable from slt_enrollment where master_enrollment_id =". $masterenrollid)->fetchField();
	expDebug::dPrint('isProgramLaunchable islaunchableClass' . $isPostAssessmentAttached, 4);
	
	$isProgramLaunchable = $isPostAssessmentAttached || $isLaunchableClass;
	expDebug::dPrint('isProgramLaunchable ' . $isProgramLaunchable, 4);
	return $isProgramLaunchable;
}


/*
 * Function for the entry of deleted logs
 */
function deleted_log_entry($fields = array(),&$statements=array()) {
    try{
        $insertStmt = db_insert('report_deleted_logs');
        $insertStmt->fields($fields);

        expDebug::dPrintDBAPI(' $insertStmt obj = ' , $insertStmt);

        // Execute the insert statement. Fetch the new user's id in slt_person table.
       // $insertId = $insertStmt->execute();
        if(!empty($statements)) {
        	$statements[] = $insertStmt;
        } else {
        	$insertId = $insertStmt->execute();
        }
        expDebug::dPrint(' $insertId = ' . print_r($insertId, true) , 3);
        return $insertId;
    }catch (Exception $ex) {
        watchdog_exception('deleted_log_entry', $ex);
        expertusErrorThrow($ex);
    }
}

function deleted_log_entry_bulk($fields = array()) {
    try{
    	expDebug::dPrint('deleted_log_entry_bulk fields: ' . print_r($fields, 1), 4);
        
    	$recquery = db_select($fields['table_name'], 't');
		$recquery->addExpression("'".$fields['table_name']."'", "table_name");
		$recquery->addExpression("t.id", "entity_id");
        $recquery->addExpression(" '".$fields['entity_type']."'", "entity_type");
        $recquery->addExpression(" '".$fields['module_name']."'", "module_name");
        $recquery->addExpression(" '".$fields['functionality_name']."' ", "functionality_name");
        $recquery->addExpression(" '".$fields['api_name']."'", "api_name");
        $recquery->addExpression(" '".$fields['deleted_on']."'", "deleted_on");
        $recquery->addExpression(" '".$fields['deleted_by']."'", "deleted_by");
        $insertFields = array('table_name', 'entity_id', 'entity_type', 'module_name', 'functionality_name', 'api_name', 'deleted_on', 'deleted_by');
        if(!empty($fields['parent1_entity_type'])){
        	$recquery->addExpression(" t.".$fields['parent1_entity_type']." ", "parent1_entity_id");
            $recquery->addExpression(" '".$fields['parent1_entity_type']."'", "parent1_entity_type");
            array_push($insertFields,"parent1_entity_id","parent1_entity_type");
        }
        if(!empty($fields['parent2_entity_type'])){
        	$recquery->addExpression(" t.".$fields['parent2_entity_type']." ", "parent2_entity_id");
            $recquery->addExpression(" '".$fields['parent2_entity_type']."'", "parent2_entity_type");
            array_push($insertFields,"parent2_entity_id","parent2_entity_type");
        }
        if(!empty($fields['parent3_entity_type'])){
        	$recquery->addExpression(" t.".$fields['parent3_entity_type']." ", "parent3_entity_id");
            $recquery->addExpression(" '".$fields['parent3_entity_type']."'", "parent3_entity_type");
            array_push($insertFields,"parent3_entity_id","parent3_entity_type");
        }
        if(!empty($fields['parent4_entity_type'])){
        	$recquery->addExpression(" t.".$fields['parent4_entity_type']." ", "parent4_entity_id");
            $recquery->addExpression(" '".$fields['parent4_entity_type']."'", "parent4_entity_type");
            array_push($insertFields,"parent4_entity_id","parent4_entity_type");
        }    
        if(!empty($fields['parent5_entity_type'])){
        	$recquery->addExpression(" t.".$fields['parent5_entity_type']." ", "parent5_entity_id");
            $recquery->addExpression(" '".$fields['parent5_entity_type']."'", "parent5_entity_type");
            array_push($insertFields,"parent5_entity_id","parent5_entity_type");
        }
        if(!empty($fields['parent6_entity_type'] )){
        	$recquery->addExpression(" t.".$fields['parent6_entity_type']." ", "parent6_entity_id");
            $recquery->addExpression(" '".$fields['parent6_entity_type']."'", "parent6_entity_type");
            array_push($insertFields,"parent6_entity_id","parent6_entity_type");
        }
        $recquery->condition('t.dataload_by', $fields['dataload_by'],'=');
        
        expDebug::dPrintDBAPI($fields['table_name'], $recquery, array($fields['dataload_by']));
        
        $insertStmt = db_insert('report_deleted_logs');
        $insertStmt->fields($insertFields);
        $insertStmt->from($recquery);

        expDebug::dPrintDBAPI(' $insertStmt obj = ' , $insertStmt);

        // Execute the insert statement. Fetch the new user's id in slt_person table.
        $insertId = $insertStmt->execute();
        expDebug::dPrint(' $insertId = ' . print_r($insertId, true) , 3);
        return $insertId;
    }catch (Exception $ex) {
        watchdog_exception('deleted_log_entry', $ex);
        expertusErrorThrow($ex);
    }
}
function deleted_log_entry_bulk_from_select($select_from, $insert_fields = array()) {
	try {
		expDebug::dPrint('deleted_log_entry_bulk fields: ' . print_r($insert_fields, 1), 4);
		$insert_stmt = db_insert('report_deleted_logs');
		if(! empty($insert_fields)) {
			$insert_stmt->fields($insert_fields);
		}
		$insert_stmt->from($select_from);

		expDebug::dPrintDBAPI('$insert_stmt obj = ', $insert_stmt);

		// Execute the insert statement. Fetch the new user's id in slt_person table.
		$insert_id = $insert_stmt->execute();
		expDebug::dPrint('$insert_id = ' . print_r($insert_id, true), 3);
		return $insert_id;
	} catch(Exception $ex) {
		watchdog_exception('deleted_log_entry_bulk_from_select', $ex);
		expertusErrorThrow($ex);
	}
}

function updateEnrollmentsGrpUserTable() {
	try {
		// insert into slt_enrollment_update_batches table from following select queries
		//Records which need to be updated as compliance
		$sel_comp = "insert into slt_enrollment_update_batches(enrid, user_id, operation, created_on, created_by)
					select enr.id, enr.user_id as user_id, 'set_compliance' as operation, now() as created_on, '1' as created_by
					from slt_group_user_mapping grpusrmap
					join slt_enrollment enr on enr.user_id = grpusrmap.user_id and enr.master_enrollment_id is null
					          and (enr.is_compliance != 1 or enr.is_compliance is null)
					join slt_group_mapping grpmap on grpmap.group_id = grpusrmap.group_id and grpmap.group_type = 0
					          and ((grpmap.entity_type = 'cre_sys_obt_cls' and grpmap.entity_id = enr.class_id))
					join slt_course_class cls on cls.id = enr.class_id and cls.is_compliance = 1
					where grpusrmap.updated_on >= date_sub(date_format(now(),'%Y-%m-%d 00:00:00'), interval 1 day)
					UNION
					select enr.id, enr.user_id as user_id, 'set_compliance' as operation, now() as created_on, '1' as created_by
					from slt_group_user_mapping grpusrmap
					join slt_enrollment enr on enr.user_id = grpusrmap.user_id and enr.master_enrollment_id is null
					          and (enr.is_compliance != 1 or enr.is_compliance is null)
					join slt_group_mapping grpmap on grpmap.group_id = grpusrmap.group_id and grpmap.group_type = 0
					          and ((grpmap.entity_type = 'cre_sys_obt_crs' and grpmap.entity_id = enr.course_id))
					join slt_course_template crs on crs.id = enr.class_id and crs.is_compliance = 1
					where grpusrmap.updated_on >= date_sub(date_format(now(),'%Y-%m-%d 00:00:00'), interval 1 day);";
		expDebug::dPrint('Records which need to be updated as compliance = ' . $sel_comp);
		db_query($sel_comp);
				
		//Records which need to be updated as non compliance
		$sel_non_comp = "insert into slt_enrollment_update_batches(enrid, user_id, operation, created_on, created_by)
					select enr.id, enr.user_id as user_id, 'unset_compliance' as operation, now() as created_on, '1' as created_by
					from report_deleted_logs del
					left join slt_group_user_mapping usrmap ON del.parent3_entity_id = usrmap.user_id and del.parent3_entity_type = 'user_id'
					      and del.parent2_entity_id = usrmap.group_id and del.parent2_entity_type = 'group_id' and usrmap.group_type = 0 and usrmap.user_type != 'R'
					join slt_enrollment enr on enr.user_id = del.parent3_entity_id and enr.master_enrollment_id is null
								and (enr.is_compliance = 1)
					join slt_group_mapping grpmap on grpmap.group_id = del.parent2_entity_id and grpmap.group_type = 0
								and ((grpmap.entity_type = 'cre_sys_obt_cls' and grpmap.entity_id = enr.class_id))
					join slt_course_class cls on cls.id = enr.class_id and cls.is_compliance = 1
					where del.deleted_on >= date_sub(date_format(now(),'%Y-%m-%d 00:00:00'), interval 1 day)
					and usrmap.id is null
					and del.parent1_entity_id = 0
					UNION
					select enr.id, enr.user_id as user_id, 'unset_compliance' as operation, now() as created_on, '1' as created_by
					from report_deleted_logs del
					left join slt_group_user_mapping usrmap ON del.parent3_entity_id = usrmap.user_id and del.parent3_entity_type = 'user_id'
					      and del.parent2_entity_id = usrmap.group_id and del.parent2_entity_type = 'group_id' and usrmap.group_type = 0 and usrmap.user_type != 'R'
					join slt_enrollment enr on enr.user_id = del.parent3_entity_id and enr.master_enrollment_id is null
								and (enr.is_compliance = 1)
					join slt_group_mapping grpmap on grpmap.group_id = del.parent2_entity_id and grpmap.group_type = 0
								and ((grpmap.entity_type = 'cre_sys_obt_crs' and grpmap.entity_id = enr.course_id))
					join slt_course_class cls on cls.id = enr.class_id and cls.is_compliance = 1
					where del.deleted_on >= date_sub(date_format(now(),'%Y-%m-%d 00:00:00'), interval 1 day)
					and usrmap.id is null
					and del.parent1_entity_id = 0;";
		expDebug::dPrint('Records which need to be updated as non compliance = ' . $sel_non_comp);
		db_query($sel_non_comp);
				
		//Records which need to be updated as mandatory
		$select_mand = "insert into slt_enrollment_update_batches(enrid, user_id, operation, created_on, created_by)
					select enr.id, enr.user_id as user_id, 'set_mandatory' as operation, now() as created_on, '1' as created_by
					from slt_group_user_mapping grpusrmap
					join slt_enrollment enr on enr.user_id = grpusrmap.user_id and enr.master_enrollment_id is null
					          and (enr.mandatory != 'Y' or enr.mandatory is null)
					join slt_group_mapping grpmap on grpmap.group_id = grpusrmap.group_id and grpmap.group_type = 0
					          and ((grpmap.entity_type = 'cre_sys_obt_cls' and grpmap.entity_id = enr.class_id and grpmap.mro = 'cre_sys_inv_man'))
					where grpusrmap.updated_on >= date_sub(date_format(now(),'%Y-%m-%d 00:00:00'), interval 1 day)
					union
					select enr.id, enr.user_id as user_id, 'set_mandatory' as operation, now() as created_on, '1' as created_by
					from slt_group_user_mapping grpusrmap
					join slt_enrollment enr on enr.user_id = grpusrmap.user_id and enr.master_enrollment_id is null
					          and (enr.mandatory != 'Y' or enr.mandatory is null)
					join slt_group_mapping grpmap on grpmap.group_id = grpusrmap.group_id and grpmap.group_type = 0
					          and ((grpmap.entity_type = 'cre_sys_obt_crs' and grpmap.entity_id = enr.course_id and grpmap.mro = 'cre_sys_inv_man'))
					where grpusrmap.updated_on >= date_sub(date_format(now(),'%Y-%m-%d 00:00:00'), interval 1 day)";
		expDebug::dPrint('Records which need to be updated as non mandatory = ' . $select_mand);
		db_query($select_mand);
							
		//Records which need to be updated as non mandatory
		$select_non_mand = "insert into slt_enrollment_update_batches(enrid, user_id, operation, created_on, created_by)
						select enr.id, enr.user_id as user_id, 'unset_mandatory' as operation, now() as created_on, '1' as created_by
						from report_deleted_logs del
						left join slt_group_user_mapping usrmap ON del.parent3_entity_id = usrmap.user_id and del.parent3_entity_type = 'user_id'
								and del.parent2_entity_id = usrmap.group_id and del.parent2_entity_type = 'group_id' and usrmap.group_type = 0 and usrmap.user_type != 'R'
						join slt_enrollment enr on enr.user_id = del.parent3_entity_id and enr.master_enrollment_id is null
								and (enr.mandatory = 'Y' )
						join slt_group_mapping grpmap on grpmap.group_id = del.parent2_entity_id and grpmap.group_type = 0
								and ((grpmap.entity_type = 'cre_sys_obt_cls' and grpmap.entity_id = enr.class_id and grpmap.mro = 'cre_sys_inv_man'))
						where del.deleted_on >= date_sub(date_format(now(),'%Y-%m-%d 00:00:00'), interval 1 day)
						and usrmap.id is null
						and del.parent1_entity_id = 0
						UNION
						select enr.id, enr.user_id as user_id, 'unset_mandatory' as operation, now() as created_on, '1' as created_by
						from report_deleted_logs del
						left join slt_group_user_mapping usrmap ON del.parent3_entity_id = usrmap.user_id and del.parent3_entity_type = 'user_id'
								and del.parent2_entity_id = usrmap.group_id and del.parent2_entity_type = 'group_id' and usrmap.group_type = 0 and usrmap.user_type != 'R'
						join slt_enrollment enr on enr.user_id = del.parent3_entity_id and enr.master_enrollment_id is null
								and (enr.mandatory = 'Y' )
						join slt_group_mapping grpmap on grpmap.group_id = del.parent2_entity_id and grpmap.group_type = 0
								and ((grpmap.entity_type = 'cre_sys_obt_crs' and grpmap.entity_id = enr.course_id and grpmap.mro = 'cre_sys_inv_man'))
						where del.deleted_on >= date_sub(date_format(now(),'%Y-%m-%d 00:00:00'), interval 1 day)
						and usrmap.id is null
						and del.parent1_entity_id = 0";
		expDebug::dPrint('Records which need to be updated as non mandatory = ' . $select_non_mand);
		db_query($select_non_mand);
							
		//Master enrollment Records which need to be marked as mandatory
		$sel_mand_tp = "insert into slt_enrollment_update_batches(menrid, user_id, operation, created_on, created_by)
					select menr.id, menr.user_id as user_id, 'set_mandatory' as operation, now() as created_on, '1' as created_by
					from slt_group_user_mapping grpusrmap
					join slt_master_enrollment menr on menr.user_id = grpusrmap.user_id
					          and (menr.mandatory != 1 or menr.mandatory is null)
					join slt_group_mapping grpmap on grpmap.group_id = grpusrmap.group_id and grpmap.group_type = 0
					          and ((grpmap.entity_type in ('cre_sys_obt_crt','cre_sys_obt_cur','cre_sys_obt_trn') and grpmap.entity_id = menr.program_id and grpmap.mro = 'cre_sys_inv_man'))
					where grpusrmap.updated_on >= date_sub(date_format(now(), '%Y-%m-%d 00:00:00'), interval 1 day)";
		expDebug::dPrint('Master enrollment Records which need to be marked as mandatory = ' . $sel_mand_tp);
		db_query($sel_mand_tp);
							
		//Master enrollment records which need to be marked as non mandatory
		$sel_non_mand_tp = "insert into slt_enrollment_update_batches(menrid, user_id, operation, created_on, created_by)
						select menr.id, menr.user_id as user_id, 'unset_mandatory' as operation, now() as created_on, '1' as created_by
						from report_deleted_logs del
						left join slt_group_user_mapping usrmap ON del.parent3_entity_id = usrmap.user_id and del.parent3_entity_type = 'user_id'
								and del.parent2_entity_id = usrmap.group_id and del.parent2_entity_type = 'group_id' and usrmap.group_type = 0 and usrmap.user_type != 'R'
						join slt_master_enrollment menr on menr.user_id = del.parent3_entity_id
								and (menr.mandatory = 1)
						join slt_group_mapping grpmap on grpmap.group_id = del.parent2_entity_id and grpmap.group_type = 0
								and ((grpmap.entity_type in ('cre_sys_obt_crt','cre_sys_obt_cur','cre_sys_obt_trn') and grpmap.entity_id = menr.program_id and grpmap.mro = 'cre_sys_inv_man'))
						where del.deleted_on >= date_sub(date_format(now(),'%Y-%m-%d 00:00:00'), interval 1 day)
						and usrmap.id is null
						and del.parent1_entity_id = 0;";
		expDebug::dPrint('Master enrollment records which need to be marked as non mandatory = ' . $sel_non_mand_tp);
		db_query($sel_non_mand_tp);
							
		$done = false;
		$mdone = false;
		$batch_limit = 1000;
		// Process enrollments records in batches 
		while($done != true) {
			// fetch data from above table batch by batch
			$select_batch = db_query("select min(id) as batch_start, max(id) as batch_end
									from (select id from slt_enrollment_update_batches bat where bat.enrid is not null order by bat.id limit $batch_limit) bat;");
			$batch_range = $select_batch->fetch();
			expDebug::dPrint('fetch data from slt_users_group_list table batch by batch' . print_r($batch_range, true), 4);
				
			if(!empty($batch_range->batch_start) && !empty($batch_range->batch_end)) {
				$update_enrollment = "update
									slt_enrollment_update_batches bat
									left join slt_enrollment enr on enr.id = bat.enrid
									set enr.mandatory = IF(bat.operation = 'set_mandatory', 'Y', IF(bat.operation = 'unset_mandatory', NULL, enr.mandatory)),
									enr.is_compliance = IF(bat.operation = 'set_compliance', 1, IF(bat.operation = 'unset_compliance', NULL, enr.is_compliance)),
									enr.updated_on = now()
									where
									bat.id between " . $batch_range->batch_start . " and " . $batch_range->batch_end;
				expDebug::dPrint('update enrollments in batch ' . $update_enrollment, 2);
				$exec_enroll = db_query($update_enrollment);
				$delete_batch = "delete bat from slt_enrollment_update_batches bat
								where bat.id between " . $batch_range->batch_start . " and " . $batch_range->batch_end;
				expDebug::dPrint('$delete_batch '.$delete_batch, 2);
				db_query($delete_batch);
							} else {
				$done = true;	// get out of while loop as there are no records to process
							}
							}
		// Process Master enrollments records in batches
		while($mdone != true) {
			// fetch data from above table batch by batch
			$select_mbatch = db_query("select min(id) as mbatch_start, max(id) as mbatch_end
					from (select id from slt_enrollment_update_batches bat where bat.menrid is not null order by bat.id limit $batch_limit) bat;");
			$mbatch_range = $select_mbatch->fetch();
			expDebug::dPrint('fetch data from slt_users_group_list table batch by batch' . print_r($mbatch_range, true), 4);
							
			if(!empty($mbatch_range->mbatch_start) && !empty($mbatch_range->mbatch_end)) {
				$update_enrollment = "update
									slt_enrollment_update_batches bat
									left join slt_master_enrollment menr on menr.id = bat.menrid
									set menr.mandatory = IF(bat.operation = 'set_mandatory', 1, IF(bat.operation = 'unset_mandatory', NULL, menr.mandatory)),
									menr.updated_on = now()
									where
									bat.id between " . $mbatch_range->mbatch_start . " and " . $mbatch_range->mbatch_end;
				expDebug::dPrint('update enrollments in mbatch ' . $update_enrollment, 2);
				$exec_enroll = db_query($update_enrollment);
				$delete_mbatch = "delete bat from slt_enrollment_update_batches bat
								where bat.id between " . $mbatch_range->mbatch_start . " and " . $mbatch_range->mbatch_end;
				expDebug::dPrint('$delete_mbatch '.$delete_batch, 2);
				db_query($delete_mbatch);
			} else {
				$mdone = true;	// get out of while loop as there are no records to process
							}
						}
		// Solr bulk sync
		syncSolrData('Enrollment', '', 'Bulk');
		syncSolrData('MasterEnrollment', '', 'Bulk');
	} catch(Exception $ex) {
		watchdog_exception('updateEnrollmentsGrpUserTable', $ex);
		expertusErrorThrow($ex);
	}
}
function IframeDeniedCheck($launchURL) {
	try{
		expDebug::dPrint("Multiplecameeee".print_r($launchURL,1),4);

	//	$time_start_execute = microtime(true);
			
			
			
		// SCRIPT CODE
			
		//Create a variable for end time
			
		$ch = curl_init();
			
			
			
		$options = array(
				CURLOPT_URL            => $launchURL,
				CURLOPT_RETURNTRANSFER => true,
				CURLOPT_HEADER         => true,
				CURLOPT_FOLLOWLOCATION => true,
				CURLOPT_ENCODING       => "",
				CURLOPT_AUTOREFERER    => true,
				CURLOPT_CONNECTTIMEOUT => 120,
				CURLOPT_TIMEOUT        => 120,
				CURLOPT_MAXREDIRS      => 10,
		);
		curl_setopt_array($ch, $options);
		$response = curl_exec($ch);
		$httpCode = curl_getinfo($ch);
		$headers=substr($response, 0, $httpCode['header_size']);
		//$time_end_execute = microtime(true);
			
		//$executiontime = $time_end_execute - $time_start_execute;
	
			
		$result = array(
				'headers' => $headers
		);
		expDebug::dPrint("Multiplecameeeesaaa---->>>".print_r($headers,1),4);
		return $result;
	}catch (Exception $ex) {
		watchdog_exception('IframeDeniedCheck', $ex);
		expertusErrorThrow($ex);
	}
}
function getClsDetails($classId){
	try {
		$select = db_select('slt_course_class', 'class');
		$select->addField('class','title','title');
		$select->addField('class','code','code');
		$select->addField('class','short_description','description');
		$select->addField('class','lang_code','lang_code');
		$select->addField('class','price','price');
		$select->addField('class','currency_type','currency_type');
		$select->addField('class','delivery_type','delivery_type');
		$select->addField('class','course_id','course_id');
		$select->addField('class','created_on','created_on');
		$select->addField('class','published_on','published_on');
		$select->addField('class','location_id','location_id');
		$select->condition('class.id',$classId);
		expDebug::dPrintDBAPI("get program details",$select);
		$clsType = $select->execute()->fetchAll();
		return $clsType;
	} catch (Exception $ex) {
		watchdog_exception('getClsDetails', $ex);
		expertusErrorThrow($ex);
	}
}

function dbStatementsExecution($statements=array()){
	try {
		//expDebug::dPrint('func get args --->dbStatementsExecution'.var_export(func_get_args(), 1),5);
		$txn = db_transaction();
		try{
			expDebug::dPrint("total statements---->>>".print_r($statements,1),4);
	//		$statements[] = db_insert('slt_enrollment')->fields(array('test'=>'test')); //For testing the negative scenarios
			unset($statements['statement']);
			foreach($statements as $key=>$statement){
				expDebug::dPrint("Fetch all the possible key values".print_r($key,1),4);
				if($key!=='insert' && $key!=='update' && $key!=='statement'){
					expDebug::dPrint("key values for insert and update scripts".print_r($key,1),4);
					expDebug::dPrintDBAPI("statements inside foreach---->>>",$statement);
					$result = $statement->execute();
				}
			}
			//return $result;
		}catch (Exception $ex) {
			//expDebug::dPrint('first exception dbStatementsExecution'.$ex->getMessage());
			$txn->rollback();
			if(!empty($statements['insert'])){
				foreach(($statements['insert']) as $insert){
				  $revertinsert = db_delete($insert->table_name);
				  $revertinsert->condition($insert->col_name, $insert->col_value,'=');
				  expDebug::dPrintDBAPI(' $$revertinsert = ' ,$revertinsert);
				  $revertinsert->execute();
				// $revertinsert= db_delete("delete from .$value->table_name. where '.$value->col_name.' = .$value->col_value.");
				 
				}
			}
			if(!empty($statements['update'])){
				foreach(($statements['update']) as $update){
					  $revertupdate = db_update($update->table_name);
					  $revertupdate->condition($update->col_name, $update->col_value,'=');
					  $revertupdate->fields($update->fields);
					  expDebug::dPrintDBAPI(' $$revertupdate = ' ,$revertupdate);
					  $revertupdate->execute();
				}
			}
			unset($txn);
			watchdog_exception('dbStatementsExecution rollback catch', $ex);
			expertusErrorThrow($ex);
		}
		// Commit the transaction
		unset($txn);
	} catch (Exception $ex) {
		//expDebug::dPrint('final exception dbStatementsExecution'.$ex->getMessage());
		watchdog_exception('dbStatementsExecution', $ex);
		expertusErrorThrow($ex);
	}
}

?>