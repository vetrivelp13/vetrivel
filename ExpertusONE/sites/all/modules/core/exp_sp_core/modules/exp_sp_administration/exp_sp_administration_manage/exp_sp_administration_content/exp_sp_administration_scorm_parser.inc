<?php 
function ParseScormIMSManifestFile($inObj) {
  // PREPARATIONS
	try{
  // central array for resource data
  global $resourceData;
  
  // Fix for windows
  $manifestfile = preg_replace('/\\\\/','/',$inObj->ContentURLPath);
  
  $baseFile = explode('contentupload/',$manifestfile);

	$manifestfile = $inObj->basePath.'/contentupload';
	$fileSplit =  explode("/",$baseFile[1]);
	foreach($fileSplit as $fs){
		$manifestfile .= "/".$fs;
	}
	
  expDebug::dPrint("Incoming manifest file ".$manifestfile,4);
  // To support DOMXPath, (without get content the xpath not supported)
  $fileContent = file_get_contents($manifestfile);
  
  //expDebug::dPrint("Manifest file content -> ".print_r($fileContent,true),4);
  // load the imsmanifest.xml file
  $xmlfile = new DomDocument;
  $xmlfile->preserveWhiteSpace = FALSE;
  $xmlfile->loadXML($fileContent);

    if($xmlfile->documentURI=="" && $xmlfile->baseURI=="") {
    expDebug::dPrint(" -- Invalid Content -- ",5);
    return form_set_error('', t('MSG765'));
    }

  expDebug::dPrint("DOM Document -- ".print_r($xmlfile,true),4);
  // adlcp namespace
  $manifest = $xmlfile->getElementsByTagName('manifest');
  $adlcp = $manifest->item(0)->getAttribute('xmlns:adlcp');
  $xmlns = $manifest->item(0)->getAttribute('xmlns');
  $version = $xmlfile->getElementsByTagName('schemaversion')->item(0)->nodeValue;
  $newformatversion = $xmlfile->getElementsByTagName('metadatascheme')->item(0)->nodeValue; 
  expDebug::dPrint("Manifest node count".count($manifest),4);
  expDebug::dPrint("Manifest adlcp count".print_r($adlcp,1),4);
  // READ THE RESOURCES LIST

  // array to store the results
  $resourceData = array();
 
  // Get sco details using xpath
  $domXpath = new DOMXPath($xmlfile);
  expDebug::dPrint("XPAth object ".print_r($domXpath,true),4);
  $domXpath->registerNamespace('xmlns',$xmlns);
  $query = "//xmlns:resources/xmlns:resource[@adlcp:scormtype = 'sco' or @adlcp:scormType = 'sco']";
  expDebug::dPrint("XPath search query string -- ".$query,4);
  $resourceList = $domXpath->query($query);
	
  expDebug::dPrint("SCO resource list count - ".count($resourceList),4);
  expDebug::dPrint("SCO resource list - ".print_r($resourceList,true),4);

  foreach ($resourceList as $rtemp) {
		expDebug::dPrint("Parsing manifest xml");
		expDebug::dPrint($rtemp , 4);
    // decode the resource attributes
    $identifier = $rtemp->getAttribute('identifier');
    expDebug::dPrint("Parsing manifest identifier");
		expDebug::dPrint($identifier ,4);
    $resourceData[$identifier]['type'] = $rtemp->getAttribute('type');
    expDebug::dPrint("Parsing manifest type");
		expDebug::dPrint($resourceData[$identifier]['type'] , 4);
    $resourceData[$identifier]['scormtype'] = $rtemp->getAttribute('adlcp:scormtype')!=''
    					?$rtemp->getAttribute('adlcp:scormtype'):$rtemp->getAttribute('adlcp:scormType');
    $resourceData[$identifier]['scorm1.2'] = $rtemp->getAttribute('adlcp:scormtype');
    $resourceData[$identifier]['scorm2004'] = $rtemp->getAttribute('adlcp:scormType');
    expDebug::dPrint("Parsing manifest scormtype");
		expDebug::dPrint($resourceData[$identifier]['scormtype'] , 4);
    $resourceData[$identifier]['href'] = $rtemp->getAttribute('href');
    expDebug::dPrint("Parsing manifest href");
		expDebug::dPrint($resourceData[$identifier]['href'] , 4);

    // list of files
    $fileList = $rtemp->getElementsByTagName('file');

    foreach ($fileList as $ftemp) {
      $resourceData[$identifier]['files'][$f] =  $ftemp->getAttribute('href');
    }

    // list of dependencies
    $dependencyList = $rtemp->getElementsByTagName('dependency');

    foreach ($dependencyList as $dtemp) {
      $resourceData[$identifier]['dependencies'][$d] =  $dtemp->getAttribute('identifierref');
    }

  }

  // resolve resource dependencies to create the file lists for each resource
  foreach ($resourceData as $identifier => $resource) {
    $resourceData[$identifier]['files'] = resolveIMSManifestDependencies($identifier);
  }

  // READ THE ITEMS LIST

  // arrays to store the results
  $itemData = array();

  // get the list of resource element
  $itemList = $xmlfile->getElementsByTagName('item');

	 foreach ($itemList as $itemp) {

    // decode the resource attributes
    $identifier = $itemp->getAttribute('identifier');
    $itemData[$identifier]['identifierref'] = $itemp->getAttribute('identifierref');
    $itemData[$identifier]['title'] = $itemp->getElementsByTagName('title')->item(0)->nodeValue;
    $itemData[$identifier]['masteryscore'] = isset($itemp->getElementsByTagNameNS($adlcp,'masteryscore')->item(0)->nodeValue)?
    	$itemp->getElementsByTagNameNS($adlcp,'masteryscore')->item(0)->nodeValue:'';
    $itemData[$identifier]['datafromlms'] = isset($itemp->getElementsByTagNameNS($adlcp,'datafromlms')->item(0)->nodeValue)?
    	$itemp->getElementsByTagNameNS($adlcp,'datafromlms')->item(0)->nodeValue:'';



  }

  // PROCESS THE ITEMS LIST TO FIND SCOS
  
  // array for the results
  $SCOdata = array();

  // loop through the list of items
  foreach ($itemData as $identifier => $item) {

    // find the linked resource
    $identifierref = $item['identifierref'];
    
    // is the linked resource a SCO? if not, skip this item
    if (strtolower($resourceData[$identifierref]['scormtype']) != 'sco') { continue; }

    // save data that we want to the output array
    $SCOdata[$identifier]['title'] = $item['title'];
    $SCOdata[$identifier]['masteryscore'] = $item['masteryscore'];
    $SCOdata[$identifier]['datafromlms'] = $item['datafromlms'];
    $SCOdata[$identifier]['href'] = $resourceData[$identifierref]['href'];
    $SCOdata[$identifier]['files'] = $resourceData[$identifierref]['files'];
    $SCOdata[$identifier]['scorm']  =$resourceData[$identifierref]['scorm1.2'];
    $SCOdata[$identifier]['scorm2004']  =$resourceData[$identifierref]['scorm2004'];
    $SCOdata[$identifier]['version'] = $version;
    $SCOdata[$identifier]['new_format_version'] = $newformatversion;

  }

  expDebug::dPrint("Parsing manifest SCOdata");
	expDebug::dPrint($SCOdata , 4);
  return $SCOdata;
  } catch (Exception $ex) {
  	watchdog_exception('ParseScormIMSManifestFile', $ex);
  	expertusErrorThrow($ex);
  }
}
 
// ------------------------------------------------------------------------------------

// recursive function used to resolve the dependencies (see above) 
  
 function resolveIMSManifestDependencies($identifier) {
 	try {
  global $resourceData;

  $files = $resourceData[$identifier]['files'];
  $dependencies = isset($resourceData[$identifier]['dependencies'])?$resourceData[$identifier]['dependencies']:'';
  if (is_array($dependencies)) {
    foreach ($dependencies as $d => $dependencyidentifier) {
      $files = array_merge($files,resolveIMSManifestDependencies($dependencyidentifier));
      unset($resourceData[$identifier]['dependencies'][$d]); 
    }
    $files = array_unique($files);
  }
  return $files;
  } catch (Exception $ex) {
  	watchdog_exception('resolveIMSManifestDependencies', $ex);
  	expertusErrorThrow($ex);
  }
}
?>