<?php

// include_once $_SERVER['DOCUMENT_ROOT']."/sites/all/services/GlobalUtil.php";
// include_once $_SERVER['DOCUMENT_ROOT']."/sites/all/services/Trace.php";
// define('DRUPAL_ROOT', $_SERVER["DOCUMENT_ROOT"]);

// include_once DRUPAL_ROOT . '/includes/bootstrap.inc';

// drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
// Added for push notification code.....
class GCM {

	//put your code down here.
	// constructor
	function __construct() {

	}

	/**
	 * Sending Push Notification
	 */
	public function send_notification($registatoin_ids = "", $message = "", $action_detail = "", $notif_type = "", $userNotificationId = "") {
		expDebug::dPrint('login test login test2222');

		// include config
		include_once './config.php';

		// Set POST variables
		$url = 'https://android.googleapis.com/gcm/send';

		$message['action_detail'] = $action_detail;
		$message['notification_id'] = $userNotificationId;
		$message['notif_type'] = $notif_type;
		
		$list = explode('~|', $action_detail);
						
			$arr = explode(">|", $list[3]);
			
			expDebug::dPrint(' arayyyyyyy11111 = ' .$arr[1], 4);
			
			if($arr[1] == 'lrn_cls_sts_atv' || $arr[1] == '' || $arr [1] == 'certification_expire' || $arr [1] == 'opensurveymobile')
			{
				$message['show_action'] = "true";
			}
			else {
			$message['show_action'] = "false";
			}
			

		$fields = array('registration_ids' => array($registatoin_ids), 'data' => $message, );

		expDebug::dPrint(' innnnnn222221111 = ' . print_r($fields, true), 4);

		$headers = array('Authorization: key=AIzaSyDM_4v9Z0uTpMpOw10IJl6sg_FX124ua18', //.GOOGLE_API_KEY,
		'Content-Type: application/json');

		// Open connection
		$ch = curl_init();

		// Set the url, number of POST vars, POST data
		curl_setopt($ch, CURLOPT_URL, $url);

		curl_setopt($ch, CURLOPT_POST, true);
		curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);

		// Disabling SSL Certificate support temporarly
		curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);

		curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode($fields));

		expDebug::dPrint('login test login afterrrr');
		// Execute post
		$result = curl_exec($ch);
		expDebug::dPrint('login test login afterrrr' . $result);
		if ($result === FALSE) {
			die('Curl failed: ' . curl_error($ch));
		}

		// Close connection
		curl_close($ch);
		// echo $result;
	}

	public function send_notificationAPN($deviceToken = "", $message = "", $action_detail = "", $notif_type = "", $userNotificationId = "") {
		//     	expDebug::dPrint('login test login test2222 APN');

		// Put your private key's passphrase here:
		$passphrase = 'expertus';

		//$ctx = stream_context_create();
		$contextOptions = array(
				'ssl' => array(
						'verify_peer' => false,
						'verify_peer_name' => false
				)
		);
		
		
		$ctx = stream_context_create($contextOptions);
		
		stream_context_set_option($ctx, 'ssl', 'local_cert', $_SERVER['DOCUMENT_ROOT'] . '/mobile/apns-dev.pem');
		stream_context_set_option($ctx, 'ssl', 'passphrase', $passphrase);

		expDebug::dPrint('login test login test2222 APN device tokennnnn' . $deviceToken);

		// Open a connection to the APNS server

		$fp = stream_socket_client('ssl://gateway.push.apple.com:2195', $err, $errstr, 60, STREAM_CLIENT_CONNECT | STREAM_CLIENT_PERSISTENT, $ctx);

		expDebug::dPrint('login test login test2222 APN innnnntesttttt' . $fp);
		
		if (!$fp)
			exit("Failed to connect: $err $errstr" . PHP_EOL);

		expDebug::dPrint('login test login test2222 APN innnnn222222' . $message);
		expDebug::dPrint('login test login test2222 APN innnnn222222' . $action_detail);

		echo 'Connected to APNS' . PHP_EOL;
		
		$list = explode('~|', $action_detail);
						
			$arr = explode(">|", $list[3]);
			
			expDebug::dPrint(' action detail valueeee = ' . print_r($arr, true),4);
			
		if ($arr[1] == 'lrn_cls_sts_atv' || $arr[1] == '') {
			
			expDebug::dPrint(' ainninnnn  = ' .$arr[1],4);
			// Create the payload body
			$body['aps'] = array('alert' => $message, 'sound' => 'default', 'category' => 'ShowE1Actions', // Category in payload will define the action buttons in push notification
			'action_detail' => $action_detail, 'notification_id' => $userNotificationId);
		}else if ($arr [1] == 'opensurveymobile') {
				expDebug::dPrint ( ' opensurvey' );
				// Create the payload body
				$body ['aps'] = array (
						'alert' => $message,
						'sound' => 'default',
						'category' => 'TakeSurvey', // Category in payload will define the action buttons in push notification
						'action_detail' => $action_detail,
						'notification_id' => $userNotificationId
				);
		}
		else if ($arr [1] == 'certification_expire') {
				expDebug::dPrint ( ' certification_expire' );
				// Create the payload body
				$body ['aps'] = array (
						'alert' => $message,
						'sound' => 'default',
						'category' => 'Recertify', // Category in payload will define the action buttons in push notification
						'action_detail' => $action_detail,
						'notification_id' => $userNotificationId
				);
		} else {
			expDebug::dPrint(' outttttt  = ' .$arr[1],4);
			// Create the payload body
			$body['aps'] = array('alert' => $message, 'sound' => 'default',
			    				// 'category' => 'ShowE1Actions',
			    				// 'action-loc-key' => "PLAY",
			'action_detail' => $action_detail, 'notification_id' => $userNotificationId);
		}

		// Encode the payload as JSON
		$payload = json_encode($body);

		// Build the binary notification
		$msg = chr(0) . pack('n', 32) . pack('H*', $deviceToken) . pack('n', strlen($payload)) . $payload;

		// Send it to the server
		$result = fwrite($fp, $msg, strlen($msg));
		//$result = fwrite($fp, $msg);
		if (!$result) {
			expDebug::dPrint('Message not delivered' . PHP_EOL);
			echo 'Message not delivered' . PHP_EOL;

		} else {
			expDebug::dPrint('Message successfully delivered' . PHP_EOL);
			echo 'Message successfully delivered' . PHP_EOL;
		}

		// Close the connection to the server
		fclose($fp);
	}

}
?>
