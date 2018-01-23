<?php 
function ParsetinCanScormXmlFile($tincanfile) {
  // PREPARATIONS
	try{
  // central array for resource data
  global $resourceData;
	expDebug::dPrint('resource validate teest -->> ');
  // load the imsmanifest.xml file
  $xmlfile = new DomDocument;
  $xmlfile->preserveWhiteSpace = FALSE;
  $xmlfile->load($tincanfile);

  // adlcp namespace
  $manifest = $xmlfile->getElementsByTagName('tincan');
  //$adlcp = $manifest->item(0)->getAttribute('xmlns:adlcp');

  // READ THE RESOURCES LIST

  // array to store the results
  $resourceData = array();

  // get the list of resource element
  $resourceList = $xmlfile->getElementsByTagName('activity');
	expdebug::dPrint('resource list '. print_r($resourceList,true),4);
  $r = 0;
  foreach ($resourceList as $rtemp) {
		expDebug::dPrint("Parsing tincan xml");
		expDebug::dPrint($resourceList->item($r) , 4);
    // decode the resource attributes
    $identifier = $resourceList->item($r)->getAttribute('id');
    expDebug::dPrint("Parsing manifest identifier");
		expDebug::dPrint($identifier ,4);
    //$resourceData[$identifier]['name'] = $resourceList->item($r)->getElementsByTagName('name')->item(0)->nodeValue;
    expDebug::dPrint("Parsing manifest name");
		//expDebug::dPrint($resourceData[$identifier]['name'] , 4);
    //$resourceData[$identifier]['scormtype'] = $resourceList->item($r)->getAttribute('adlcp:scormtype')!=''
    	//				?$resourceList->item($r)->getAttribute('adlcp:scormtype'):$resourceList->item($r)->getAttribute('adlcp:scormType');
    //expDebug::dPrint("Parsing manifest scormtype");
		//expDebug::dPrint($resourceData[$identifier]['scormtype'] , 4);
    //$resourceData[$identifier]['launch'] = $resourceList->item($r)->getElementsByTagName('launch')->item(0)->nodeValue;
    expDebug::dPrint("Parsing manifest launch");
		//expDebug::dPrint($resourceData[$identifier]['launch'] , 4);
		
		if($resourceList->item($r)->getElementsByTagName('launch')->item(0)->nodeValue != ''){
			$resourceData[$identifier]['name'] = $resourceList->item($r)->getElementsByTagName('name')->item(0)->nodeValue;
			expDebug::dPrint("Parsing manifest name");
			expDebug::dPrint($resourceData[$identifier]['name'] , 4);
			$resourceData[$identifier]['launch'] = $resourceList->item($r)->getElementsByTagName('launch')->item(0)->nodeValue;
		}
		
    // list of files
   /*  $fileList = $resourceList->item($r)->getElementsByTagName('file');

    $f = 0;
    foreach ($fileList as $ftemp) {
      $resourceData[$identifier]['files'][$f] =  $fileList->item($f)->getAttribute('launch');
      $f++;
    }

    // list of dependencies
    $dependencyList = $resourceList->item($r)->getElementsByTagName('dependency');

    $d = 0;
    foreach ($dependencyList as $dtemp) {
      $resourceData[$identifier]['dependencies'][$d] =  $dependencyList->item($d)->getAttribute('identifierref');
      $d++;
    }*/

    $r++; 

  }

  // resolve resource dependencies to create the file lists for each resource
 /*  foreach ($resourceData as $identifier => $resource) {
    $resourceData[$identifier]['files'] = resolveTincanDependencies($identifier);
  } */

  // READ THE ITEMS LIST

  // arrays to store the results
  $itemData = array();

  // get the list of resource element
  //$itemList = $xmlfile->getElementsByTagName('item');
	/* expDebug::dPrint('$itemList ' . print_r($itemList,true),5);
  $i = 0;
  foreach ($itemList as $itemp) { */

    // decode the resource attributes
    //$identifier = $itemList->item($i)->getAttribute('identifier');
   // $itemData[$identifier]['identifierref'] = $itemList->item($i)->getAttribute('identifierref');
   // $itemData[$identifier]['name'] = $itemList->item($i)->getElementsByTagName('name')->item(0)->nodeValue;
    /* $itemData[$identifier]['masteryscore'] = isset($itemList->item($i)->getElementsByTagNameNS($adlcp,'masteryscore')->item(0)->nodeValue)?
    	$itemList->item($i)->getElementsByTagNameNS($adlcp,'masteryscore')->item(0)->nodeValue:'';
    $itemData[$identifier]['datafromlms'] = isset($itemList->item($i)->getElementsByTagNameNS($adlcp,'datafromlms')->item(0)->nodeValue)?
    	$itemList->item($i)->getElementsByTagNameNS($adlcp,'datafromlms')->item(0)->nodeValue:'';
 */
   // $i++;

  //}

  // PROCESS THE ITEMS LIST TO FIND SCOS
  
  // array for the results
  //$SCOdata = array();

  // loop through the list of items
 // foreach ($itemData as $identifier => $item) {

    // find the linked resource
    //$identifierref = $item['identifierref'];
    
    // is the linked resource a SCO? if not, skip this item
    //if (strtolower($resourceData[$identifierref]['scormtype']) != 'sco') { continue; }

    // save data that we want to the output array
 //   $SCOdata[$identifier]['name'] = $item['name'];
   // $SCOdata[$identifier]['masteryscore'] = $item['masteryscore'];
    //$SCOdata[$identifier]['datafromlms'] = $item['datafromlms'];
 //   $SCOdata[$identifier]['launch'] = $resourceData[$identifierref]['launch'];
    //$SCOdata[$identifier]['files'] = $resourceData[$identifierref]['files'];

  //}

  expDebug::dPrint("Parsing manifest SCOdata");
	expDebug::dPrint($resourceData , 4);
  return $resourceData;
  } catch (Exception $ex) {
  	watchdog_exception('ParsetinCanScormXmlFile', $ex);
  	expertusErrorThrow($ex);
  }
}
 
// ------------------------------------------------------------------------------------

// recursive function used to resolve the dependencies (see above) 
  
 /* function resolveTincanDependencies($identifier) {
 	try {
  global $resourceData;

  $files = $resourceData[$identifier]['files'];
  $dependencies = isset($resourceData[$identifier]['dependencies'])?$resourceData[$identifier]['dependencies']:'';
  if (is_array($dependencies)) {
    foreach ($dependencies as $d => $dependencyidentifier) {
      $files = array_merge($files,resolveTincanDependencies($dependencyidentifier));
      unset($resourceData[$identifier]['dependencies'][$d]); 
    }
    $files = array_unique($files);
  }
  return $files;
  } catch (Exception $ex) {
  	watchdog_exception('resolveTincanDependencies', $ex);
  	expertusErrorThrow($ex);
  }
} */
?>