<?php
/**
* @class or PHP Name		: UserInfo
* @author(s)				: Product Team
* Version         			: 1.0
* Date						: 09/09/2009
* PHP Version          		: 5.2.6
* Description     			: Class used to get the currently logged in user related information.
*/
//include "../../../getLearnerInfo.php";
//include "/getLearnerInfo.php";
include $_SERVER['DOCUMENT_ROOT']."/getLearnerInfo.php";
class UserInfo 
{
  
	public  function getLoggedInUserId()
	{
		$obj = new GetLearnerInfo();
		expDebug::dPrint("UserInfo Data : ");
		expDebug::dPrint($obj->getValue('userid'));
		return $obj->getValue('userid');
	}
}
?>