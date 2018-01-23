<?php

    function getRandomWord($len = 10) {
	$word = array_merge(range('a', 'z'), range('A', 'Z'));
	shuffle($word);
	return substr(implode($word), 0, $len);
    }
    
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
    
    function getIdOfLoggedInUser() {
    	try{
    		global $user;
    		// Added by Vincent for performance imporvement on Sep 2, 2015
    		if($user->uid == 0 && !$_REQUEST['apiname']) return 0;
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
    
    
    
    
    
    
    
    
    
    $random_word = getRandomWord();
    $random_num = rand(2,1000);
    $cart = db_query("select cart_id from uc_cart_products order by cart_item_id DESC limit 1  ")->fetchField();
    $cart1 = getDrupalIdOfUser($cart);
    expDebug::dPrint(" CheckingAbhi ".print_r($cart));
    expDebug::dPrint(" CheckingAbhi ".print_r($cart1));
    
    $testCases = array(
    
    		'ListItemsInCartAPI' => array(
    				'datasource' => array(
    						array('input'=>array('user_id'=>$cart1,'action'=>"get_count",'currency_code'=>"USD",'userid'=>1),
    								'output'=>array('classid','classtitle','courseid','description','price','deliverytypename','locname','loccity','loccountry','deliverytypeid','order_id','total_product','payment_gateway','enabledPaymentMethod'),
    								'result'=>array('<results totalrecords =','<classid>','<classtitle>','<courseid>','<description>','<price>','<deliverytypename>','<locname>','<loccity>','<loccountry>','<deliverytypeid>','<order_id>','<total_product>','<payment_gateway>','<enabledPaymentMethod>'),
    								'output_type'=>'xml'
    						),
    						array('input'=>array('user_id'=>$cart1,'action'=>"delete",'currency_code'=>"USD",'userid'=>1),
    								'output'=>array('classid','classtitle','courseid','description','price','deliverytypename','locname','loccity','loccountry','deliverytypeid','order_id','total_product','payment_gateway','enabledPaymentMethod'),
    								'result'=>array('<results totalrecords =','<classid>','<classtitle>','<courseid>','<description>','<price>','<deliverytypename>','<locname>','<loccity>','<loccountry>','<deliverytypeid>','<order_id>','<total_product>','<payment_gateway>','<enabledPaymentMethod>'),
    								'output_type'=>'xml'
    						),
    						array('input'=>array('user_id'=>$cart1,'action'=>"get_count",'currency_code'=>"USD",'userid'=>1),
    								'output'=>array('classid','classtitle','courseid','description','price','deliverytypename','locname','loccity','loccountry','deliverytypeid','order_id','total_product','payment_gateway','enabledPaymentMethod'),
    								'result'=>array('jsonResponse','classid','classtitle','courseid','description','price','deliverytypename','locname','loccity','loccountry','deliverytypeid','order_id','total_product','payment_gateway','enabledPaymentMethod'),
    								'output_type'=>'json'
    						),
    						array('input'=>array('user_id'=>$cart1,'action'=>"delete",'currency_code'=>"USD",'userid'=>1),
    								'output'=>array('classid','classtitle','courseid','description','price','deliverytypename','locname','loccity','loccountry','deliverytypeid','order_id','total_product','payment_gateway','enabledPaymentMethod'),
    								'result'=>array('jsonResponse','classid','classtitle','courseid','description','price','deliverytypename','locname','loccity','loccountry','deliverytypeid','order_id','total_product','payment_gateway','enabledPaymentMethod'),
    								'output_type'=>'json'
    						)
    				),
    				
    				'validate' => array(
    						array('input'=>array('user_id'=>'','action'=>"get_count",'currency_code'=>"USD",'userid'=>1),
    								'output'=>array(),
    								'result'=>array('<error>','<error_code>'."L_005".'</error_code>','Mandatory Fields are missing in the feed','user_id'),
    								'output_type'=>'xml'
    						),
    						array('input'=>array('user_id'=>'','action'=>"delete",'currency_code'=>"USD",'userid'=>1),
    								'output'=>array(),
    								'result'=>array('<error>','<error_code>'."L_005".'</error_code>','Mandatory Fields are missing in the feed','user_id'),
    								'output_type'=>'xml'
    						),
    						array('input'=>array('user_id'=>$cart1,'action'=>"",'currency_code'=>"USD",'userid'=>1),
    								'output'=>array(),
    								'result'=>array('<error>','<error_code>'."L_005".'</error_code>','Mandatory Fields are missing in the feed','action'),
    								'output_type'=>'xml'
    						),
    				
    				// Invalid Data type
    						array('input'=>array('user_id'=>$cart1,'action'=>"get_count",'currency_code'=>$random_word,'userid'=>1),
    								'output'=>array(),
    								'result'=>array('<error>','<error_code>'."5".'</error_code>','The given Currency type is invalid or inactive status','The given Currency type is invalid or inactive status'),
    								'output_type'=>'xml'
    						),
    						array('input'=>array('user_id'=>$cart1,'action'=>"delete",'currency_code'=>$random_word,'userid'=>1),
    								'output'=>array(),
    								'result'=>array('<error>','<error_code>'."5".'</error_code>','The given Currency type is invalid or inactive status','The given Currency type is invalid or inactive status'),
    								'output_type'=>'xml'
    						),
    				
    			)
    		)
    )
    
  ?>