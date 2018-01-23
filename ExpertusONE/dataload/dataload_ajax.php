<?php

/**
 * API Request authentication
 */
// Prepare request for

$operation = isset($_POST['operation'])?$_POST['operation']:'';
$access_token = $_POST['access_token'];

// Exit the operation if it is not authenticated
if($operation != 'authenticate' && $access_token==''){
	$ErrMsg = "Authentication is not yet happened";
	$response['Status']	= 'Failure';
	$response['ErrorMessage'] = $ErrMsg;
	echo json_encode($response);
	exit;
}
if(!filter_var($_POST['baseurl'], FILTER_VALIDATE_URL)){
	echo 'Invalid Base url';
	exit;
}

$fields_string = '';

if($operation=='authenticate'){
	$client_id = $_POST['client_id'];
	$client_secret = $_POST['client_secret'];
	$client_details = base64_encode("$client_id:$client_secret");
	if($access_token==''){
		$data = array("grant_type"=>"client_credentials",
		'client_id'=>$client_id,
		'client_secret'=>$client_secret
		);
	}else{
		$data = array("grant_type"=>"client_credentials",
		'client_id'=>$client_id,
		'client_secret'=>$client_secret,
									'access_token'=>$access_token,
									'action'=>'create_on_expire'
									);
	}
	foreach($data as $key=>$value) { $fields_string .= $key.'='.$value.'&'; }
	rtrim($fields_string, '&');
	
	// TODO: peer_veriry config value should be pulled from the given baseurl
	// $peerVerify = $config['peer_verify'] == 0 ? FALSE : TRUE;
	$peerVerify = FALSE;
	$context = stream_context_create(array(
		'http' => array(
		'method' => 'POST',
		'header' => "Content-Type: application/x-www-form-urlencoded",
		//"Authorization : Basic $client_details\r\n", //.
		//"Authorization : Bearer $access_token\r\n",
		'content' => $fields_string
			),
		'ssl' => array('verify_peer'=>$peerVerify, 
           'verify_peer_name'=>$peerVerify)	
	));

	$response = file_get_contents($_POST['baseurl'].'/apis/oauth2/Token.php', false, $context);
	echo $response;
}else{

	try{
		
		if(!filter_var($_POST['baseurl'], FILTER_VALIDATE_URL)){
			echo 'Invalid Base url';
			exit;
		}
		$url = $_POST['baseurl'].'/dataload/dataload_handler.php';
		if($_POST['Request_Type'] == 'createReq'){
			ini_set('memory_limit', '512M');
			$data = "";
			$boundary = "---------------------".substr(md5(rand(0,32000)), 0, 10);

			//Collect Postdata
			foreach($_POST as $key => $val)
			{
				$data .= "--$boundary\n";
				$data .= "Content-Disposition: form-data; name=\"".$key."\"\n\n".$val."\n";
			}

			$data .= "--$boundary\n";

			//Collect Filedata
			$fileContents = file_get_contents($_FILES['File_Upload']['tmp_name']);

			$data .= "Content-Disposition: form-data; name=File_Upload; filename=\"{$_FILES['File_Upload']['name']}\"\n";
			$data .= "Content-Type: text/csv; charset=utf-8\n";
			$data .= "Content-Transfer-Encoding: binary\n\n";
			$data .= $fileContents."\n";
			$data .= "--$boundary--\n";

			// TODO: peer_veriry config value should be pulled from the given baseurl
			// $peerVerify = $config['peer_verify'] == 0 ? FALSE : TRUE;
			$peerVerify = FALSE;
			$params = array(
					'http' => array(
		           'method' => 'POST',  
		           'header' => "Authorization : Bearer $access_token \r\n".
		           			   'Content-Type: multipart/form-data; boundary='.$boundary,  
		           'content' => $data  
							),
					'ssl' => array('verify_peer'=>$peerVerify, 
           'verify_peer_name'=>$peerVerify)
				);
			$ctx = stream_context_create($params);
			$fp = fopen($url, 'rb', false, $ctx);

			if (!$fp) {
				throw new Exception("Problem with $url, $php_errormsg");
			}

			$response = @stream_get_contents($fp);
			ini_set('memory_limit', '128M');
		}else{
			$data = $_POST;
			foreach($data as $key=>$value) { $fields_string .= $key.'='.$value.'&'; }
			rtrim($fields_string, '&');
			
			// TODO: peer_veriry config value should be pulled from the given baseurl
			// $peerVerify = $config['peer_verify'] == 0 ? FALSE : TRUE;
			$peerVerify = FALSE;
			$context = stream_context_create(array(
				'http' => array(
						'method' => 'POST',
						'header' => "Authorization : Bearer $access_token \r\n".
									'Content-Type: application/x-www-form-urlencoded',
						'content' => $fields_string
					),
				'ssl' => array('verify_peer'=>$peerVerify, 
           'verify_peer_name'=>$peerVerify)
			));
			$response = file_get_contents($url, false, $context);
		}
		echo $response;
	}catch(Exception $e){
		echo $e->getMessage();
	}
}


?>