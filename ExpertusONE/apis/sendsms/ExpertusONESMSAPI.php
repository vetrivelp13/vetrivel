<?php
class ExpertusONESMSAPI
{
	/*
	 * ExpertusONESMSAPI class is accessing current SMS API Gateway which is setted in exp_sp.ini
	 * 
	 * All API should used three mandatory parameters like API Path, API Username and API Password or API Key.
	 * some of the API has some additional parameters like API ID.
	 * 
	 * API Path					:	URL Link for the API
	 * API Username				:	Username for the API account
	 * API Password / API Key	:	Password for the API account
	 * API ID					:	Account ID for the API
	 * 
	 * gateway160 API
	 * ==============
	 * Parameters	:	apiPath,apiUname,apiKey
	 * Methods		:	setDetailsForGateway160Provider (for initializing parameters)
	 * 					post_request	(processing parameters and sent request to API)
	 * 					sendSMS (for sending SMS to mobile)
	 * 
	 * clickatell API
	 * ==============
	 * Parameters	:	apiPath,apiId,apiUname,apiKey
	 * Methods		:	setDetailsForClickATellProvider (for initializing parameters)
	 * 					sendSMS (for sending SMS to mobile)
	 * 
	 */
	
	private $provider;
	
	public function __construct()
	{
		$provider = "";
	}
	
	public function setProvider($provider)
	{
		$this->provider = $provider;
	}
	
	private $gateway160_apiPath,$gateway160_apiUname,$gateway160_apiKey;
	
	public function setDetailsForGateway160Provider($apiPath,$apiUname,$apiKey)
	{
		$this->gateway160_apiPath = $apiPath;
		$this->gateway160_apiUname = $apiUname;
		$this->gateway160_apiKey = $apiKey;
	}

	private $clickatell_apiPath,$clickatell_apiId,$clickatell_apiUname,$clickatell_apiKey;
	
	public function setDetailsForClickATellProvider($apiPath,$apiId,$apiUname,$apiKey)
	{
		$this->clickatell_apiPath = $apiPath;
		$this->clickatell_apiId = $apiId;
		$this->clickatell_apiUname = $apiUname;
		$this->clickatell_apiKey = $apiKey;
	}
	
	
 /* private $nexmo_account_key,$nexmo_secret;
	private $vianett_account_key,$vianett_password;
 	public function setDetailsForVianettProvider($accountkey,$secret)
	{
		$this->vianett_account_key = $accountkey;
		$this->vianett_password = $secret;
	}
	public function setDetailsForNexmoProvider($accountkey,$secret)
	{
		$this->nexmo_account_key = $accountkey;
		$this->nexmo_secret = $secret;
	} */

	
	public function sendSMS($tonumber,$msg)
	{
		$text = urlencode($msg);
		expDebug::dPrint(' sendSMS $text = '.$text);
		
		// gateway160 API Start
		if($this->provider == "gateway160") 
		{
			$text = urldecode($msg);
			$post_data = array(
					'accountName' => $this->gateway160_apiUname,
					'key' => $this->gateway160_apiKey,
					'phoneNumber' => $tonumber,
					'message' => $text, //please perform URLEncode on this field
			);
			try
			{
				$result = post_request($this->gateway160_apiPath, $post_data);
				if($result == "1")
				{
					echo "Sent Successfully";
					expDebug::dPrint(' sendSMStoUser via gateway160 : Successfully Sent ');
				}
				else
				{
					//error  (check the response code from the chart above)
					echo "Error";
					expDebug::dPrint(' sendSMStoUser via gateway160 : Message Sending Falied ');
				}
			}
			catch (Exception $e)
			{
				echo "Exception: " . $e->getMessage();
			}
		}// gateway160 API End
		
		// clickatell API Start
		else if($this->provider == "clickatell")
		{
			// auth call
			$url = $this->clickatell_apiPath."/http/auth?user=".$this->clickatell_apiUname."&password=".$this->clickatell_apiKey."&api_id=".$this->clickatell_apiId;
			//."&to=".$tonumber."&text=".$text;
			
			// do auth call
			$ret = file($url);
	 
			// explode our response. return string is on first line of the data returned
			$sess = explode(":",$ret[0]);
			if ($sess[0] == "OK") {
	 
			$sess_id = trim($sess[1]); // remove any whitespace
			$url = $this->clickatell_apiPath."/http/sendmsg?session_id=".$sess_id."&to=".$tonumber."&text=".$text;
	 
			// do sendmsg call
			$ret = file($url);
			$send = explode(":",$ret[0]);
	 
				if ($send[0] == "ID") {
					echo "successnmessage ID: ". $send[1];
					expDebug::dPrint(' sendSMStoUser via clickatell : Message Sending Succesfully ');
					expDebug::dPrint(' sendSMStoUser via clickatell : successnmessage ID '.$send[1]);
				} else {
					echo "send message failed";
					expDebug::dPrint(' sendSMStoUser via clickatell : Message Sending Falied ');
				}
			
			} else {
				echo "Authentication failure: ". $ret[0];
				expDebug::dPrint(' sendSMStoUser via clickatell : Authentication failure ');
			}
		}// clickatell API End
		
	/*	else if($this->provider == "nexmo")
		{
			include_once $_SERVER["DOCUMENT_ROOT"]."sendsms/nexmo/NexmoMessage.php";
			$sms = new NexmoMessage($this->nexmo_account_key,$this->nexmo_secret);
			$info = $sms->sendText( $tonumber, 'MyApp', $text);
			echo $sms->displayOverview($info);
		}
		else if($this->provider == "vianett")
		{
			include_once $_SERVER["DOCUMENT_ROOT"]."sendsms/vianett/ViaNettSMS.php";
			// Create ViaNettSMS object with params $Username and $Password
			$ViaNettSMS = new ViaNettSMS($this->vianett_account_key,$this->vianett_password);
			try
			{
				// Send SMS through the HTTP API
				$Result = $ViaNettSMS->SendSMS("919944841615", $tonumber, $text);
				// Check result object returned and give response to end user according to success or not.
				if ($Result->Success == true)
					$Message = "Message successfully sent!";
				else
					$Message = "Error occured while sending SMS<br />Errorcode: " . $Result->ErrorCode . "<br />Errormessage: " . $Result->ErrorMessage;
			}catch (Exception $e)
			{
				//Error occured while connecting to server.
				$Message = $e->getMessage();
			}
			echo $Message;
		}
		else if($this->provider == "freesms")
		{
			$url = "http://www.FreeSMSGateway.com/api_send";
			$post_contacts = array('919841547494');
			$json_contacts = json_encode($post_contacts);

			$fields = array (
			'access_token' =>'3c81031031d08c1acb467d1820b85b74',
			'message' =>$text,
			'send_to' => "919944841615",//$json_contacts,
			);

			$fields_string = '';

			foreach($fields as $key=>$value)
			{
				$fields_string .= $key.'='.$value.'&';
			}
			rtrim($fields_string, '&');
			echo $fields_string;

			$ch = curl_init();
			curl_setopt($ch,CURLOPT_URL,$url);
			curl_setopt($ch, CURLOPT_POST, count($fields));
			curl_setopt($ch,CURLOPT_POSTFIELDS,$fields_string);
			curl_setopt($ch,CURLOPT_RETURNTRANSFER,true);
			$result = curl_exec($ch);
			curl_close($ch);
			print $result;
		} */

	}
	
	
}// End ExpertusONESMSAPI

// post_request() for gateway160 API
function post_request($url, $data, $optional_headers = null)
{
    // data needs to be in the form of foo=bar&bar=foo
    $data = http_build_query($data);
    $params = array(
            'http' => array(
                'method' => 'POST',
                'content' => $data,
                // you need to specify the content length header
                'header' => "Content-type: application/x-www-form-urlencoded\r\n"
                    . 'Content-Length: ' . strlen($data) . "\r\n"
                ));
    if ($optional_headers !== null) {
        // append optional_headers if it is not null
        $params['http']['header'] .= $optional_headers;
    }
    $ctx = stream_context_create($params);
    $fp = fopen($url, 'r', false, $ctx);
    if (!$fp) {
        throw new Exception("Problem with $url, $php_errormsg");
    }
    $response = stream_get_contents($fp);
    if ($response === false) {
        throw new Exception("Problem reading data from $url, $php_errormsg");
    }
    fclose($fp);
    return $response;
}// End post_request()


?>