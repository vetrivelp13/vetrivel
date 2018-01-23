<?php
/*
 slp_taxable_states_sel`(IN xcountry text,
                                           IN xdeliverytype VARCHAR (20),
                                           IN xtype VARCHAR (255),
                                           OUT xval text)
  BEGIN
      DECLARE xxval   text DEFAULT '';
      IF xtype = 'State'
      THEN
         SELECT group_concat(state)
           INTO xxval
           FROM slt_tax
          WHERE     country = xcountry
                AND delivery_type = xdeliverytype
                AND is_tax = 'Y';
      ELSE
         SELECT group_concat(country)
           INTO xxval
           FROM slt_tax
          WHERE delivery_type = xdeliverytype
                AND is_tax = 'Y'
                AND country NOT IN('US','CA');
      END IF;
      SET xval   = xxval;
   END$$
 */

/*
 * getTaxableStates() - Based on $returnType ('State' or 'Country'), returns a comma separated list of either states or non-US and non-CA countries that levy taxes for a $deliveryType. Replaces stored procedure slp_taxable_states_sel()
 */
function getTaxableStates($country, $deliveryType, $returnType) {
 try {
  $returnList = '';

  if ($returnType == 'State') {
    // Select table
    $selectStmt = db_select('slt_tax', 'tax');

    // Set conditions
    $selectStmt->condition('tax.country', $country);
    $selectStmt->condition('tax.delivery_type', $deliveryType);
    $selectStmt->condition('tax.is_tax', 'Y');

    // Select fields
    $selectStmt->addField('tax', 'state', 'state');

    expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);

    // Execute query
    $result = $selectStmt->execute();

    while ($record = $result->fetchAssoc()) {
      $returnList .= (($returnList == '')? '' : ',') . $record['state'];
    }
  }
  else {
    // Select table
    $selectStmt = db_select('slt_tax', 'tax');

    // Set conditions
    $selectStmt->condition('tax.country', array('US','CA'), 'NOT IN');
    $selectStmt->condition('tax.delivery_type', $deliveryType);
    $selectStmt->condition('tax.is_tax', 'Y');

    // Select fields
    $selectStmt->addField('tax', 'country', 'country');

    expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);

    // Execute query
    $result = $selectStmt->execute();

    while ($record = $result->fetchAssoc()) {
      $returnList .= (($returnList == '')? '' : ',') . $record['country'];
    }
  }

  return $returnList;
  }catch (Exception $ex) {
  	watchdog_exception('getTaxableStates', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * hasTaxExemption - Get whether the organization has exemption from tax?
 */
function hasTaxExemption($organizationId, $profileMapColumnName) {
 try {
	// Select table
  $selectStmt = db_select('slt_entity_profile_mapping', 'spm');
  // Join other tables.
  $selectStmt->leftJoin('slt_profile_list_items', 'spli', 'spm.' . $profileMapColumnName . ' = spli.id');

  // Set conditions
  $selectStmt->condition('spm.entity_id', $organizationId);
  $selectStmt->condition('spm.entity_type', 12); // 12 - profile_cat = Custom Profile, profile_tag = Organization (see slt_profile_tagging_defn table)

  // Select fields and/or add expressions
  $selectStmt->addField('spli', 'name', 'has_tax_exemption');
  expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);

  // Execute query
  $result = $selectStmt->execute();

  return $result->fetchField();
  }catch (Exception $ex) {
  	watchdog_exception('hasTaxExemption', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getCountryVATNumber($countryCode) - Fetches a country's VAT number
 */
function getCountryVATNumber($countryCode) {
 try{
  // Select table
  $selectStmt = db_select('slt_country', 'country');

  // Set conditions
  $selectStmt->condition('country.country_code', $countryCode);

  // Select fields and/or add expressions
  $selectStmt->addField('country', 'vat_no', 'vat_no');

  // Limit to 1 record (the first record)
  $selectStmt->range(0, 1);

  expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);

  // Execute query
  $result = $selectStmt->execute();

  return $result->fetchField();
  }catch (Exception $ex) {
  	watchdog_exception('getCountryVATNumber', $ex);
  	expertusErrorThrow($ex);
  }
}


/*
 * getProductCodeId() - Fetches product code id from slt_tax for country and state
 */
function getProductCodeId($countryCode, $stateCode) {
 try{
  // Select table
  $selectStmt = db_select('slt_tax', 'tax');

  // Set conditions
  $selectStmt->condition('tax.country', $countryCode);
  $selectStmt->condition('tax.state', $stateCode);

  // Select fields and/or add expressions
  $selectStmt->addField('tax', 'product_code_id', 'product_code_id');

  // Limit to 1 record (the first record)
  $selectStmt->range(0, 1);

  expDebug::dPrintDBAPI(' $selectStmt = ' ,$selectStmt);

  // Execute query
  $result = $selectStmt->execute();

  return $result->fetchField();
  }catch (Exception $ex) {
  	watchdog_exception('getProductCodeId', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getPriceOfClass() - Fetches price of a class from slt_course_class
 */
function getPriceOfClass($courseId, $classId,$programId) {
 try{
  if($programId){
	  $selectStmt = db_select('slt_program', 'prg');
	  // Set conditions
	  $selectStmt->condition('prg.id', $programId);
	  // Select fields and/or add expressions
	  $selectStmt->addField('prg', 'price', 'price');
  }
  else{
	  $selectStmt = db_select('slt_course_class', 'cls');
	  // Set conditions
	  $selectStmt->condition('cls.id', $classId);
	  $selectStmt->condition('cls.course_id', $courseId);
	  // Select fields and/or add expressions
	  $selectStmt->addField('cls', 'price', 'price');
  }
  // Limit to 1 record (the first record)
  $selectStmt->range(0, 1);
  expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);

  // Execute query
  $result = $selectStmt->execute();

  return $result->fetchField();
  }catch (Exception $ex) {
  	watchdog_exception('getPriceOfClass', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getPriceUserCurrencyOfClass() - Fetches currency of a class from slt_course_class
*/
function getPriceUserCurrencyOfClass($courseId, $classId,$programId) {
	try{
		if(current_path() == 'cart'){
			$currency = $_SESSION['user_preferred_currency_code'];
		}else{
			if(isset($_SESSION['admin_shop_cart_currency_code']) && !empty($_SESSION['admin_shop_cart_currency_code'])){
				$currency = $_SESSION['admin_shop_cart_currency_code'];
			}else{
				$currency = (isset($_SESSION['shop_cart_currency_code']) && !empty($_SESSION['shop_cart_currency_code']))?$_SESSION['shop_cart_currency_code']:$_SESSION['user_preferred_currency_code'];
			}
			expDebug::dPrint("currency_type code => ".$currency,4);
		}
		if($programId>0){
	  $selectStmt = db_select('slt_program', 'prg');
	  // Set conditions
	  $selectStmt->condition('prg.id', $programId);
	  // Select fields and/or add expressions
	  $selectStmt->addExpression("slf_convert_price(prg.price,prg.currency_type,'".$currency."')", 'price');
		}
		else{
	  $selectStmt = db_select('slt_course_class', 'cls');
	  // Set conditions
	  $selectStmt->condition('cls.id', $classId);
	  $selectStmt->condition('cls.course_id', $courseId);
	  // Select fields and/or add expressions
	  $selectStmt->addExpression("slf_convert_price(cls.price,cls.currency_type,'".$currency."')", 'price');
		}

		// Limit to 1 record (the first record)
		$selectStmt->range(0, 1);
		expDebug::dPrintDBAPI(' $selectStmt getPriceUserCurrencyOfClass = ' , $selectStmt);

		// Execute query
		$result = $selectStmt->execute();

		return $result->fetchField();
	}catch (Exception $ex) {
		watchdog_exception('getPriceUserCurrencyOfClass', $ex);
		expertusErrorThrow($ex);
	}
}
/*
 * getCurrencyconvertionRateOfClass() - Fetches currency of a class from slt_course_class
*/
function getCurrencyconvertionRateOfClass($courseId, $classId,$programId,$pre_currency) {
	try{
		if($programId>0){
	  $selectStmt = db_select('slt_program', 'prg');
	  // Set conditions
	  $selectStmt->condition('prg.id', $programId);
	  $selectStmt->condition('sccd.convert_type', $pre_currency,'=');
	  // Select fields and/or add expressions
	  $selectStmt->addField('sccd', 'rate', 'rate');
	  // Join other tables.
	  $selectStmt->leftJoin('slt_currency_conversion_details', 'sccd', 'sccd.base_type=prg.currency_type');
		}
		else{
	  $selectStmt = db_select('slt_course_class', 'cls');
	  // Set conditions
	  $selectStmt->condition('cls.id', $classId);
	  $selectStmt->condition('cls.course_id', $courseId);
	  $selectStmt->condition('sccd.convert_type', $pre_currency,'=');
	  // Select fields and/or add expressions
	  $selectStmt->addField('sccd', 'rate', 'rate');
	  // Join other tables.
	  $selectStmt->leftJoin('slt_currency_conversion_details', 'sccd', 'sccd.base_type=cls.currency_type');
		}

		// Limit to 1 record (the first record)
		$selectStmt->range(0, 1);
		expDebug::dPrintDBAPI(' $selectStmt getCurrencyconvertionRateOfClass = ' , $selectStmt);

		// Execute query
		$result = $selectStmt->execute()->fetchField();
		
		if(!empty($result)){
			return $result;
		}else{
			return 1;
		}
		
	}catch (Exception $ex) {
		watchdog_exception('getCurrencyconvertionRateOfClass', $ex);
		expertusErrorThrow($ex);
	}
}
/*
 * getCartCount() - Get total cart items
 */
function getCartCount(){
 try{
  $items = uc_cart_get_contents( null, 'rebuild');
  $count = count($items);
  return (!empty($count)) ? $count : '0 ';
  }catch (Exception $ex) {
  	watchdog_exception('getCartCount', $ex);
  	expertusErrorThrow($ex);
  }
}

function getSltLMSOrderId($ucOrderId){
	try{
		$selectStmt = db_select('slt_order', 'ord');
		$selectStmt->condition('ord.uc_order_id', $ucOrderId);
		$selectStmt->addField('ord', 'id');
		expDebug::dPrintDBAPI(' $selectStmt SQL = ' , print_r($selectStmt, true));
		$sltOrderId = $selectStmt->execute()->fetchField();
		return $sltOrderId;
	}catch (Exception $ex) {
		watchdog_exception('getSltLMSOrderId', $ex);
		expertusErrorThrow($ex);
	}
}

?>