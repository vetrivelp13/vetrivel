<?php

/**
* @class or PHP Name		: DrupalBaseService
* @author(s)				: vivekanandan.R
* Version         			: 1.0
* Date						: 09/01/2010
* PHP Version          		: 5.2.6
* Description     			: This is used for accesing drupal Functions, Modules, Database from Smartportal Service Layer  
*/

class DrupalBaseService {
	private $mCurrentPath;

	function __construct(){		
		$this->mCurrentPath = getcwd();		
	}	
	
	/*public function setDrupalBaseDir()
	{
		
      chdir("../../../");         
      require_once "./includes/bootstrap.inc";
	  drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
	         		
	}*/
	public function setDrupalBaseDir() {
      chdir("../../../");
      if (!defined('DRUPAL_ROOT')) 
      {
      	 define('DRUPAL_ROOT', getcwd());
         
      }
 	if (!defined('DRUPAL_BOOTSTRAP_FULL'))
 		require_once DRUPAL_ROOT."/includes/bootstrap.inc"; 
     
      drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
    } 

	
	public function setWorkingDir($pmVar)
	{
      chdir($pmVar);		
	}
	
	public function storeWorkingDir($pmVar)
	{
      $this->mCurrentPath = $pmVar;		
	}	
	public function restoreWorkingDir()
	{
      chdir($this->mCurrentPath);		
	}
		
}

?>