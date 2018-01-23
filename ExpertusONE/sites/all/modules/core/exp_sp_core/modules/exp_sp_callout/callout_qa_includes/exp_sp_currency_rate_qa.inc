<?php

function processCalloutCurrencyRateRequest($apifrom,$activeCurrencies) {
	try {
		expDebug::dPrint('$processCalloutCurrencyRateRequest firt');
		$base_url 		= "https://xecdapi.xe.com/v1"; //$conf['server_host'] ;
		$authentication = "expertus890076010:jvogl7rrsep7mobcetukigf2dn"; //$conf['account_id'] . ':' . $conf['api_key'];
		$responseMode	= "json"; //$conf['response'];
		$responseQuery  = isset($responseMode) ? '.' . $responseMode : '';
		expDebug::dPrint('$processCalloutCurrencyRateRequest-$apifrom= '.$apifrom);
		// Make server request based on the active currency
		
		expDebug::dPrint('$activeCurrencies $to_array = ' . print_r($activeCurrencies, true), 5);
		require_once DRUPAL_ROOT. '/sites/all/modules/core/exp_sp_core/modules/exp_sp_callout/exp_sp_callout.inc';
		sameBaseTypeCurrencyCheck($activeCurrencies);
		 foreach($activeCurrencies as $fromCurrency) {
			$toCurrencies = removeActiveCurrency($activeCurrencies, $fromCurrency);
			$toCurrencyList = implode($toCurrencies, ',');
			expDebug::dPrint($fromCurrency . ':  ' . $toCurrencyList, 5);
			$method = '/convert_from'. $responseQuery .'/?from='.$fromCurrency.'&to='.$toCurrencyList.'&amount=1';
			$url = $base_url . $method;		
			if ($fromCurrency != '' && $toCurrencyList != '') {
				expDebug::dPrint('convert url:' . $url, 3);
				$response = makeServerRequest($url, $responsetype, $authentication, $http_status);
				if ($response != '') {
					if ($responseMode == 'json') {
						expDebug::dPrint('Inside Json:' . $url, 3);	
						//require_once DRUPAL_ROOT. '/sites/all/modules/core/exp_sp_core/modules/exp_sp_callout/exp_sp_callout.inc';
						$fields = array(
								'vendor_id'         => 'callout',
								'base_type'      => $fromCurrency,
								'request'        => serialize($url),
								'response'        => serialize($response),
								'created_on'        => date('Y-m-d H:i:s')
						);
						updateCurrencyConversionRate($response,$apifrom); // Default handler as JSON
					} elseif ($responseMode == 'xml') {
						expDebug::dprint('$Currency Call out $$xml '.print_r($response,true),5);
						$fields = array(
								'vendor_id'         => 'callout',
								'base_type'      => $fromCurrency,
								'request'        => serialize($url),
								'response'        => serialize($json),
								'created_on'        => date('Y-m-d H:i:s')
						);
						$xml = simplexml_load_string($response);
						$result_response = json_encode($xml);
						updateCurrencyConversionRate($result_response,$apifrom); 
					}	
					$insert_cc_log = db_insert('slt_currency_server_logs');
					
					$insert_cc_log->fields($fields);
					expDebug::dPrintDBAPI('INSERT of slt_currency_server_logs record ', $insert_cc_log);
					$insert_cc_log->execute();
				} else {
					break;
				}		
			} else {
				break;
			}		
		} 		
	} catch(Exception $e) {
		watchdog_exception('processCalloutCurrencyRateRequest', $e);
		expertusErrorThrow($e);
	}
}

function removeActiveCurrency($array = array(), $item = '') {
	$index = array_search($item, $array);
	if ( $index !== false ) {
		unset( $array[$index] );
	}
	return $array;
}

function makeServerRequest($url, $responsetype, $authentication, &$http_status, &$header = null) {
	try {
		$ch=curl_init();

		// user credencial
		curl_setopt($ch, CURLOPT_USERPWD, $authentication);
		curl_setopt($ch, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
		curl_setopt($ch, CURLOPT_URL, $url);

		if (!is_null($header)) {
			curl_setopt($ch, CURLOPT_HEADER, true);
		}
		curl_setopt($ch, CURLOPT_FOLLOWLOCATION, true);
		curl_setopt($ch, CURLOPT_HTTPHEADER, array('Accept: application/'.$responsetype.''));
		curl_setopt($ch, CURLOPT_VERBOSE, true);
		//curl_setopt($ch, CURLOPT_HEADER, 1);
		$response = curl_exec($ch);
		expDebug::dPrint('External Server Response:' . print_r($response, 1), 3);
		$body = null;
		// error
		if (!$response) {
			$body = curl_error($ch);
			// HostNotFound, No route to Host, etc  Network related error
			$http_status = -1;
		} else {
			//parsing http status code
			$http_status = curl_getinfo($ch, CURLINFO_HTTP_CODE);
			if (!is_null($header)) {
				$header_size = curl_getinfo($ch, CURLINFO_HEADER_SIZE);
				$header = substr($response, 0, $header_size);
				$body = substr($response, $header_size);
			} else {
				$body = $response;
			}
		}
		curl_close($ch);
		return $response;
	} catch (Exception $e) {
		expertusLogError($e);
	}
}

function sameBaseTypeCurrencyCheck($currencyList){
	if (empty($currencyList)) {
		expDebug::dPrint( 'currencyList value is null', 4);
		return false;
	}
	expDebug::dPrint( 'sameBaseTypeCurrencyCheck', 4);
	$loggedUserId = getSltpersonUserId();
	foreach($currencyList as $currencyCode) {
		$codeexist=checkCurrencyExist($currencyCode,$currencyCode);
		if($codeexist<=0){
			$insertstmt = db_insert('slt_currency_conversion_details');
			$fields = array(
					'base_type'         => $currencyCode,
					'convert_type'      => $currencyCode,
					'rate'              => roundCurrency('1.0000000000'),
					'created_by'        => $loggedUserId,
					'created_on'        => date('Y-m-d H:i:s')
			);
			$insertstmt->fields($fields);
			expDebug::dPrintDBAPI('Insert sameBaseTypeCurrencyCheck record ', $insertstmt);
			$insertstmt->execute();
		}
	}	
	
}