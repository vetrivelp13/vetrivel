<?php

/**
    * This class is used to maintain the error messages/code.
    * @author Sureshkumar.v
*/
class ErrorMessages
{

	/**
    * This api is used to get the error message.
    * @param $errcode string
    * @return string
    */
	public static function getErrorMessage($errobj)
	{
		$errmessage=new stdClass();
		$errmessage->code=$errobj->errcode;
		switch($errmessage->code)
		{
			case "0":
					$errmessage->shortmessage="DB ERROR";
					$errmessage->longmessage=$errobj->errormsg;
					$errmessage->correctivesolution="";
					break;
		  case "5":
					$errmessage->shortmessage=$errobj->errormsg;
					$errmessage->longmessage=$errobj->errormsg;
					$errmessage->correctivesolution="";
					break;
			case "L_001":

					$errmessage->shortmessage="Login unsuccessful";
					$errmessage->longmessage="Login unsuccessful";
					$errmessage->correctivesolution="";
					break;
			case "L_002":
					$errmessage->shortmessage="User Cancelled Request";
					$errmessage->longmessage="User Cancelled Request";
					$errmessage->correctivesolution="";
					break;
			case "L_003":
					$errmessage->shortmessage="Invalid authorization";
					$errmessage->longmessage="Invalid authorization";
					$errmessage->correctivesolution="";
					break;
			case "L_004":
					$errmessage->shortmessage="OAuth Verification Failed:";
					$errmessage->longmessage=$errobj->errormsg;
					$errmessage->correctivesolution="Please do authentication then access apis.";
					break;
			case "L_005":
					$errmessage->shortmessage="Mandatory Fields are missing in the feed. List provided below:";
					$errmessage->longmessage=$errobj->errormsg;
					$errmessage->correctivesolution="The above mentioned fields are mandatory and required to process the feed.";
					break;
			case "L_006":
					$errmessage->shortmessage="Application rate limit exceeded. Please try again later";
					$errmessage->longmessage="Application rate limit exceeded. Please try again later";
					$errmessage->correctivesolution="Application rate limit exceeded. Please try again later";
					break;
		    case "L_007":
					$errmessage->shortmessage="Invalid User";
					$errmessage->longmessage="Invalid User";
					$errmessage->correctivesolution="Pls wait";
					break;

		    case "L_008":
					$errmessage->shortmessage = "Permission restricted";
					$errmessage->longmessage = "You do not have privileges to access this API";
					$errmessage->correctivesolution = "";
					break;

			case "L_009":
				$errmessage->shortmessage="INVALID ACCESS";
				$errmessage->longmessage="Invalid access token or expired access token.";
				$errmessage->correctivesolution="Please try with a new access token or check with site admin";
				break;
			case "L_010":
					$errmessage->shortmessage="INVALID ACCESS";
					$errmessage->longmessage="Userid is invalid";
					$errmessage->correctivesolution="Please check with site admin";
					break;
			case "L_011":
				$errmessage->shortmessage="INVALID APINAME";
				$errmessage->longmessage="API name is invalid";
				$errmessage->correctivesolution="Please check with documentation";
				break;
			case "L_012": case "2":       //for validation errors
				$errmessage->shortmessage = $errobj->errormsg;
				$errmessage->longmessage = $errobj->errormsg;
				$errmessage->correctivesolution= $errobj->correctivesolution;
				break;
			case "L_013":
				$errmessage->shortmessage="USER PRIVILEGE";
				$errmessage->longmessage="User dont have this privilege";
				$errmessage->correctivesolution="Kindly check userid's privilege";
				break;

			case "":
					$errmessage->shortmessage=$errobj->errormsg;
					$errmessage->longmessage=$errobj->errormsg;
					$errmessage->correctivesolution="";
					break;



		}
		return $errmessage;
	}
	public static function getErrorMessageNew($errors)
	{
		$errobjs = (isset($errors->errors) && is_array($errors->errors)) ? $errors->errors : (is_array($errors) ? $errors : array($errors));
		$error_messages = array();

		foreach ($errobjs as $errobj) {
			$error_message = new stdClass();
			switch ($errobj->errcode) {
				case "0" :
					$error_message->type = "DB ERROR";
					$error_message->message = $errobj->errormsg;
					//$error_message->correctivesolution = "";
					break;
				case "2" : case "L_012":
					$error_message->type = "Validation Failed";
					$error_message->message = $errobj->errormsg;
					$error_message->field = $errobj->errorfield;
					//$error_message->correctivesolution = $errobj->correctivesolution;
					break;
				case "3" :
					$error_message->type = "Data Error";
					$error_message->message = $errobj->errormsg;
					//$error_message->correctivesolution = $errobj->correctivesolution;
					break;
				case "4" :
					$error_message->type = "Failure Status";
					$error_message->message = $errobj->errormsg;
					//$error_message->correctivesolution = $errobj->correctivesolution;
					break;
				case "5" :
					$error_message->type = $errobj->errormsg;
					$error_message->message = $errobj->errormsg;
					//$error_message->correctivesolution = "";
					break;
				case "6";
				$error_message->type = "XSS SQL INJECTION ERROR";
				$error_message->field = $errobj->errorfield;
				$error_message->message = $errobj->errormsg;
				break;
				case "L_001" :
					$error_message->type = "Login unsuccessful";
					$error_message->message = "Login unsuccessful";
					//$error_message->correctivesolution = "";
					break;
				case "L_002" :
					$error_message->type = "User Cancelled Request";
					$error_message->message = "User Cancelled Request";
					//$error_message->correctivesolution = "";
					break;
				case "L_003" :
					$error_message->type = "Invalid authorization";
					$error_message->message = "Invalid authorization";
					//$error_message->correctivesolution = "";
					break;
				case "L_004" :
					$error_message->type = "OAuth Verification Failed";
					$error_message->message = "Please do authentication then access apis.";
					//$error_message->correctivesolution = "Please do authentication then access apis.";
					break;
				case "L_005" :
					$error_message->type = "Required Param";
					$error_message->message = $errobj->errormsg;
					$error_message->field = $errobj->errorfield;
					//$error_message->correctivesolution = "The above mentioned fields are mandatory and required to process the feed.";
					break;
				case "L_006" :
					$error_message->type = "Application rate limit exceeded. Please try again later";
					$error_message->message = "Application rate limit exceeded. Please try again later";
					//$error_message->correctivesolution = "Please try again later";
					break;
				case "L_007" :
					$error_message->type = "Invalid User";
					$error_message->message = "Invalid User";
					//$error_message->correctivesolution = "Pls wait";
					break;

				case "L_008" :
					$error_message->type = "Permission restricted";
					$error_message->message = "You do not have privileges to access this API";
					break;

				case "L_009" :
					$error_message->type = "INVALID ACCESS";
					$error_message->message = "Invalid access token or expired access token.";
					//$error_message->correctivesolution = "Please try with a new access token or check with site admin";
					break;
				case "L_010" :
					$error_message->type = "INVALID ACCESS";
					$error_message->message = "Userid is invalid";
					//$error_message->correctivesolution = "Please check with site admin";
					break;
				case "L_011" :
					$error_message->type = "INVALID APINAME";
					$error_message->message = "API name is invalid";
					//$error_message->correctivesolution = "Please check with documentation";
					break;
				case "L_013":
					$error_message->type = "USER PRIVILEGE";
					$error_message->message = "User doesn't have this privilege";
					$error_message->field = 'userid';
					break;
				case "" :
					$error_message->type = $errobj->errormsg;
					$error_message->message = $errobj->errormsg;
					//$error_message->correctivesolution = "";
					break;
			}
			$error_messages[] = $error_message;
		}
		expDebug::dPrint('$error_messages'.print_r($error_messages, 1));
		return $error_messages;
	}
}
?>