<?php

class H5peditor {

  public static $styles = array(
    'libs/darkroom.css',
    'styles/css/application.css'
  );
  
  //h5pcustomize
  public static $scripts = array(
    'scripts/h5peditor.js',
    'scripts/h5peditor-semantic-structure.js',
    'scripts/h5peditor-editor.js',
    'scripts/h5peditor-library-selector.js',
    'scripts/h5peditor-form.js',
    'scripts/h5peditor-text.js',
    'scripts/h5peditor-html.js',
    'scripts/h5peditor-number.js',
    'scripts/h5peditor-textarea.js',
    'scripts/h5peditor-file-uploader.js',
    'scripts/h5peditor-file.js',
    'scripts/h5peditor-image.js',
    'scripts/h5peditor-image-popup.js',
    'scripts/h5peditor-av.js',
    'scripts/h5peditor-group.js',
    'scripts/h5peditor-boolean.js',
    'scripts/h5peditor-list.js',
    'scripts/h5peditor-list-editor.js',
    'scripts/h5peditor-library.js',
    'scripts/h5peditor-library-list-cache.js',
    'scripts/h5peditor-select.js',
    'scripts/h5peditor-dimensions.js',
    'scripts/h5peditor-coordinates.js',
    'scripts/h5peditor-none.js', 
    'ckeditor/ckeditor.js'
  	
  );
  /*public static $scripts = array(
  		'scripts/h5peditor-editor.js',
  		'scripts/h5peditor-core.js',
  		'ckeditor/ckeditor.js',
  );*/
  
  private $h5p, $storage, $files_directory, $basePath, $relativePathRegExp;

  /**
   * Constructor for the core editor library.
   *
   * @param \H5PCore $h5p Instance of core.
   * @param mixed $storage Instance of h5peditor storage.
   * @param string $basePath Url path to prefix assets with.
   * @param string $filesDir H5P files directory.
   * @param string $editorFilesDir Optional custom editor files directory outside h5p files directory.
   */
  function __construct($h5p, $storage, $basePath, $filesDir, $editorFilesDir = NULL, $relativePathRegExp = '/^(\.\.\/){1,2}(\d+|editor)\/(.+)$/') {
    $this->h5p = $h5p;
    $this->storage = $storage;
    $this->basePath = $basePath;
    $this->contentFilesDir = $filesDir; // . DIRECTORY_SEPARATOR. 'content'; //h5pcustomization - restructure
    $this->editorFilesDir = ($editorFilesDir === NULL ? $filesDir . DIRECTORY_SEPARATOR . 'editor' : $editorFilesDir);
    $this->relativePathRegExp = $relativePathRegExp;
  }

  /**
   * Get list of libraries.
   *
   * @return array
   */
  public function getLibraries() {
    if (isset($_POST['libraries'])) {
      // Get details for the specified libraries.
      $libraries = array();
      foreach ($_POST['libraries'] as $libraryName) {
        $matches = array();
        preg_match_all('/(.+)\s(\d+)\.(\d+)$/', $libraryName, $matches);
        if ($matches) {
          $libraries[] = (object) array(
            'uberName' => $libraryName,
            'name' => $matches[1][0],
            'majorVersion' => $matches[2][0],
            'minorVersion' => $matches[3][0]
          );
        }
      }
    }

    $libraries = $this->storage->getLibraries(!isset($libraries) ? NULL : $libraries);

    if ($this->h5p->development_mode & H5PDevelopment::MODE_LIBRARY) {
      $devLibs = $this->h5p->h5pD->getLibraries();

      // Replace libraries with devlibs
      for ($i = 0, $s = count($libraries); $i < $s; $i++) {
        $lid = $libraries[$i]->name . ' ' . $libraries[$i]->majorVersion . '.' . $libraries[$i]->minorVersion;
        if (isset($devLibs[$lid])) {
          $libraries[$i] = (object) array(
            'uberName' => $lid,
            'name' => $devLibs[$lid]['machineName'],
            'title' => $devLibs[$lid]['title'],
            'majorVersion' => $devLibs[$lid]['majorVersion'],
            'minorVersion' => $devLibs[$lid]['minorVersion'],
            'runnable' => $devLibs[$lid]['runnable'],
          );
        }
      }
    }

    return json_encode($libraries);
  }

  /**
   * Keep track of temporary files.
   *
   * @param object file
   */
  public function addTmpFile($file) {
    $this->storage->addTmpFile($file);
  }

  /**
   * Create directories for uploaded content.
   *
   * @param int $id
   * @return boolean
   */
  public function createDirectories($id) {
    $this->content_directory = $this->contentFilesDir . DIRECTORY_SEPARATOR . $id . DIRECTORY_SEPARATOR;

    if (!is_dir($this->contentFilesDir)) {
      mkdir($this->contentFilesDir, 0777, true);
    }

    $sub_directories = array('', 'files', 'images', 'videos', 'audios');
    foreach ($sub_directories AS $sub_directory) {
      $sub_directory = $this->content_directory . $sub_directory;
      if (!is_dir($sub_directory) && !mkdir($sub_directory)) {
        return FALSE;
      }
    }

    return TRUE;
  }

  /**
   * Move uploaded files, remove old files and update library usage.
   *
   * @param string $oldLibrary
   * @param string $oldParameters
   * @param array $newLibrary
   * @param string $newParameters
   */
  public function processParameters($contentId, $newLibrary, $newParameters, $oldLibrary = NULL, $oldParameters = NULL) {
    $newFiles = array();
    $oldFiles = array();

    // Find new libraries/content dependencies and files.
    // Start by creating a fake library field to process. This way we get all the dependencies of the main library as well.
    $field = (object) array(
      'type' => 'library'
    );
    $libraryParams = (object) array(
      'library' => H5PCore::libraryToString($newLibrary),
      'params' => $newParameters
    );
    $this->processField($field, $libraryParams, $newFiles);

    if ($oldLibrary !== NULL) {
      // Find old files and libraries.
      $this->processSemantics($oldFiles, $this->h5p->loadLibrarySemantics($oldLibrary['name'], $oldLibrary['majorVersion'], $oldLibrary['minorVersion']), $oldParameters);

      // Remove old files.
      for ($i = 0, $s = count($oldFiles); $i < $s; $i++) {
        if (!in_array($oldFiles[$i], $newFiles) &&
            preg_match('/^(\w+:\/\/|\.\.\/)/i', $oldFiles[$i]) === 0) {
          $removeFile = $this->content_directory . $oldFiles[$i];
          unlink($removeFile);
          $this->storage->removeFile($removeFile);
        }
      }
    }
  }

  /**
   * Recursive function that moves the new files in to the h5p content folder and generates a list over the old files.
   * Also locates all the librares.
   *
   * @param array $files
   * @param array $libraries
   * @param array $semantics
   * @param array $params
   */
  private function processSemantics(&$files, $semantics, &$params) {
    for ($i = 0, $s = count($semantics); $i < $s; $i++) {
      $field = $semantics[$i];
      if (!isset($params->{$field->name})) {
        continue;
      }
      $this->processField($field, $params->{$field->name}, $files);
    }
  }

  /**
   * Process a single field.
   *
   * @staticvar string $h5peditor_path
   * @param object $field
   * @param mixed $params
   * @param array $files
   */
  private function processField(&$field, &$params, &$files) {
    switch ($field->type) {
      case 'file':
      case 'image':
        if (isset($params->path)) {
          $this->processFile($params, $files);

          // Process original image
          if (isset($params->originalImage) && isset($params->originalImage->path)) {
            $this->processFile($params->originalImage, $files);
          }
        }
        break;

      case 'video':
      case 'audio':
        if (is_array($params)) {
          for ($i = 0, $s = count($params); $i < $s; $i++) {
            $this->processFile($params[$i], $files);
          }
        }
        break;

      case 'library':
        if (isset($params->library) && isset($params->params)) {
          $library = H5PCore::libraryFromString($params->library);
          $semantics = $this->h5p->loadLibrarySemantics($library['machineName'], $library['majorVersion'], $library['minorVersion']);

          // Process parameters for the library.
          $this->processSemantics($files, $semantics, $params->params);
        }
        break;

      case 'group':
        if (isset($params)) {
          if (count($field->fields) == 1) {
            $params = (object) array($field->fields[0]->name => $params);
          }
          $this->processSemantics($files, $field->fields, $params);
        }
        break;

      case 'list':
        if (is_array($params)) {
          for ($j = 0, $t = count($params); $j < $t; $j++) {
            $this->processField($field->field, $params[$j], $files);
          }
        }
        break;
    }
  }

  /**
   * @param mixed $params
   * @param array $files
   */
  private function processFile(&$params, &$files) {
    static $h5peditor_path;
    if (!$h5peditor_path) {
      $h5peditor_path = $this->editorFilesDir . DIRECTORY_SEPARATOR;
    }

    // File could be copied from another content folder.
    $matches = array();
    if (preg_match($this->relativePathRegExp, $params->path, $matches)) {
      // Create copy of file
      $source = $this->content_directory . $params->path;
      $destination = $this->content_directory . $matches[3];
      if (file_exists($source) && !file_exists($destination)) {
        copy($source, $destination);
      }
      $params->path = $matches[3];
    }
    else {
      // Check if tmp file
      $oldPath = $h5peditor_path . $params->path;
      $newPath = $this->content_directory . $params->path;
      if (file_exists($newPath)) {
        // Uploaded to content folder, make sure the cleanup script doesn't get it.
        $this->storage->keepFile($newPath, $newPath);
      }
      elseif (file_exists($oldPath)) {
        // Copy file from editor tmp folder to content folder
        copy($oldPath, $newPath);
        // Not moved in-case it has been copied to multiple content.
      }
    }

    $files[] = $params->path;
  }

  /**
   * TODO: Consider moving to core.
   */
  public function getLibraryLanguage($machineName, $majorVersion, $minorVersion, $languageCode) {
    if ($this->h5p->development_mode & H5PDevelopment::MODE_LIBRARY) {
      // Try to get language development library first.
      $language = $this->h5p->h5pD->getLanguage($machineName, $majorVersion, $minorVersion, $languageCode);
    }

    if (isset($language) === FALSE) {
      $language = $this->storage->getLanguage($machineName, $majorVersion, $minorVersion, $languageCode);
    }

    return ($language === FALSE ? NULL : $language);
  }

  /**
   * Return all libraries used by the given editor library.
   *
   * @param string $machineName Library identfier part 1
   * @param int $majorVersion Library identfier part 2
   * @param int $minorVersion Library identfier part 3
   */
  public function findEditorLibraries($machineName, $majorVersion, $minorVersion) {
    $library = $this->h5p->loadLibrary($machineName, $majorVersion, $minorVersion);
    $dependencies = array();
    $this->h5p->findLibraryDependencies($dependencies, $library);

    // Order dependencies by weight
    $orderedDependencies = array();
    for ($i = 1, $s = count($dependencies); $i <= $s; $i++) {
      foreach ($dependencies as $dependency) {
        if ($dependency['weight'] === $i && $dependency['type'] === 'editor') {
          // Only load editor libraries.
          $orderedDependencies[$dependency['library']['libraryId']] = $dependency['library'];
          break;
        }
      }
    }

    return $orderedDependencies;
  }

  /**
   * Get all scripts, css and semantics data for a library
   *
   * @param string $library_name
   *  Name of the library we want to fetch data for
   * @param string $prefix Optional. Files are relative to another dir.
   */
  public function getLibraryData($machineName, $majorVersion, $minorVersion, $languageCode, $path = '', $prefix = '') {
    $libraryData = new stdClass();
	expDebug::dPrint("machineName::::".$machineName);
    $libraries = $this->findEditorLibraries($machineName, $majorVersion, $minorVersion);
    $libraryData->semantics = $this->h5p->loadLibrarySemantics($machineName, $majorVersion, $minorVersion);
    $libraryData->language = $this->getLibraryLanguage($machineName, $majorVersion, $minorVersion, $languageCode);

    $files = $this->h5p->getDependenciesFiles($libraries, $prefix);
    $this->storage->alterLibraryFiles($files, $libraries);

    if ($path) {
      $path .= '/';
    }

    // Javascripts
    //h5pcustomize
   if($machineName != "H5P.InteractiveVideo" && $machineName != "H5P.CoursePresentation")
    {
    if (!empty($files['scripts'])) {
      foreach ($files['scripts'] as $script) {
        if (preg_match ('/:\/\//', $script->path) === 1) {
          // External file
          $libraryData->javascript[$script->path . $script->version] = "\n" . file_get_contents($script->path);
        }
        else {
          // Local file
        	
          $libraryData->javascript[$this->h5p->url . $script->path . $script->version] = "\n" . file_get_contents($path . $script->path);
        }
      }
    }

    // Stylesheets
    if (!empty($files['styles'])) {
      foreach ($files['styles'] as $css) {
        if (preg_match ('/:\/\//', $css->path) === 1) {
          // External file
          $libraryData->css[$css->path. $css->version] = file_get_contents($css->path);
        }
        else {
          // Local file
          H5peditor::buildCssPath(NULL, $this->h5p->url . dirname($css->path) . '/');
          $libraryData->css[$this->h5p->url . $css->path . $css->version] = preg_replace_callback('/url\([\'"]?(?![a-z]+:|\/+)([^\'")]+)[\'"]?\)/i', 'H5peditor::buildCssPath', file_get_contents($path . $css->path));
        }
      }
    }
    }
 	else{
 		
 		if($machineName == "H5P.CoursePresentation")
 		{
 			$libraryData->javascript = array();
 			$libraryData->css = array();
 			/*$required_files = array();
 			
 			$required_files[] = "";
 			$required_files[] = "/sites/all/libraries/h5plibraries/H5P.AppearIn-1.0/h5p-appear-in.js?ver=1.0.3";
 			$required_files[] = "/sites/all/libraries/h5plibraries/H5P.AdvancedText-1.1/text.js?ver=1.1.0";
 			$required_files[] = "/sites/all/libraries/h5plibraries/Tether-1.0/scripts/tether.min.js?ver=1.0.1";
 			$required_files[] = "/sites/all/libraries/h5plibraries/Drop-1.0/js/drop.min.js?ver=1.0.1";
 			$required_files[] = "/sites/all/libraries/h5plibraries/H5P.Transition-1.0/transition.js?ver=1.0.2";
 			$required_files[] ="/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/js/joubel-message-dialog.js?ver=1.2.7";
 			
 			$required_files[] ="/sites/all/libraries/h5plibraries/EmbeddedJS-1.0/js/ejs_production.js?ver=1.0.3";
 			$required_files[] ="/sites/all/libraries/h5plibraries/EmbeddedJS-1.0/js/ejs_viewhelpers.js?ver=1.0.3";
 					
 			$required_files[] = "/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/js/joubel-simple-rounded-button.js?ver=1.2.7";
 			$required_files[] ="/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/js/joubel-ui.js?ver=1.2.7";
 			$required_files[] ="/sites/all/libraries/h5plibraries/H5P.Question-1.1/scripts/question.js?ver=1.1.7";
 			$required_files[] ="/sites/all/libraries/h5plibraries/H5P.CoursePresentation-1.9/scripts/cp.js?ver=1.9.0";
 			$required_files[] ="/sites/all/libraries/h5plibraries/H5P.CoursePresentation-1.9/scripts/go-to-slide.js?ver=1.9.0";
 			$required_files[] ="/sites/all/libraries/h5plibraries/H5P.CoursePresentation-1.9/scripts/summary-slide.js?ver=1.9.0";
 			$required_files[] ="/sites/all/libraries/h5plibraries/H5P.CoursePresentation-1.9/scripts/navigation-line.js?ver=1.9.0";
 			$required_files[] ="/sites/all/libraries/h5plibraries/H5P.CoursePresentation-1.9/scripts/slide-backgrounds.js?ver=1.9.0";
 			$required_files[] ="/sites/all/libraries/h5plibraries/H5PEditor.VerticalTabs-1.1/vertical-tabs.js?ver=1.1.3";
 			$required_files[] ="/sites/all/libraries/h5plibraries/H5P.DragNDrop-1.1/drag-n-drop.js?ver=1.1.2";
 			$required_files[] ="/sites/all/libraries/h5plibraries/H5P.DragNResize-1.2/H5P.DragNResize.js?ver=1.2.0";
 			$required_files[] ="/sites/all/libraries/h5plibraries/H5P.DragNBar-1.2/scripts/drag-n-bar.js?ver=1.2.1";
 			$required_files[] = "/sites/all/libraries/h5plibraries/H5P.DragNBar-1.2/scripts/context-menu.js?ver=1.2.1";
 			$required_files[] ="/sites/all/libraries/h5plibraries/H5P.DragNBar-1.2/scripts/dialog.js?ver=1.2.1";
 			$required_files[] ="/sites/all/libraries/h5plibraries/H5P.DragNBar-1.2/scripts/drag-n-bar-element.js?ver=1.2.1";
 			$required_files[] ="/sites/all/libraries/h5plibraries/H5P.DragNSort-1.1/scripts/drag-n-sort.js?ver=1.1.1";
 			$required_files[] ="/sites/all/libraries/h5plibraries/jQuery.ui-1.10/js/jquery-ui.js?ver=1.10.14";
 			//$required_files[] ="/sites/all/libraries/h5plibraries/H5P.Video-1.2/scripts/video.js?ver=1.2.0";
 			$required_files[] ="/sites/all/libraries/h5plibraries/H5PEditor.Timecode-1.0/timecode.js?ver=1.0.11";
 			$required_files[] ="/sites/all/libraries/h5plibraries/H5P.GoToQuestion-1.0/scripts/go-to-question.js?ver=1.0.2";
 			$required_files[] ="/sites/all/libraries/h5plibraries/H5P.Text-1.1/scripts/text.js?ver=1.1.0";
 			$required_files[] ="/sites/all/libraries/h5plibraries/H5PEditor.Duration-1.0/scripts/duration.js?ver=1.0.4";
 			$required_files[] ="/sites/all/libraries/h5plibraries/H5PEditor.ColorSelector-1.0/scripts/spectrum.js?ver=1.0.2";
 			$required_files[] ="/sites/all/libraries/h5plibraries/H5PEditor.ColorSelector-1.0/scripts/color-selector.js?ver=1.0.2";
 			$required_files[] ="/sites/all/libraries/h5plibraries/H5PEditor.RadioSelector-1.0/radio-selector.js?ver=1.0.1";
 			$required_files[] ="/sites/all/libraries/h5plibraries/H5PEditor.CoursePresentation-1.9/scripts/cp-editor.js?ver=1.9.0";
 			$required_files[] ="/sites/all/libraries/h5plibraries/H5PEditor.CoursePresentation-1.9/scripts/slide-selector.js?ver=1.9.0";
 			$required_files[] ="/sites/all/libraries/h5plibraries/H5PEditor.CoursePresentation-1.9/scripts/bg-selector.js?ver=1.9.0";
 			$required_files[] = "/sites/all/libraries/h5plibraries/H5PEditor.UrlField-1.0/link-widget.js?ver=1.0.0";
 			$required_files[] = "/sites/all/libraries/h5plibraries/H5P.Link-1.1/link.js?ver=1.1.0";
 			*/
 		
 			if($_GET["H5P_DEVELOPMENT_MODE"] == 1)
 			{
	 			foreach ($files['scripts'] as $script) 
	 			{
	 				//if(in_array("/".$script->path.$script->version,$required_files))
	 				{
	 					 expDebug::dPrint("tttt:"."/".$script->path.$script->version);
	 					 $changedUrl = str_replace("sites/default/files/contentupload/libraries/","sites/all/libraries/h5plibraries/",$script->path);
	 					 $script->path = $changedUrl;
	 					 
	 				if (preg_match ('/:\/\//', $script->path) === 1) {
	 					// External file
	 					$libraryData->javascript[$script->path . $script->version] = "\n" . file_get_contents($script->path);
	 				}
	 				else {
	 					// Local file
	 					$libraryData->javascript[$this->h5p->url . $script->path . $script->version] = "\n" . file_get_contents($path . $script->path);
	 				}
	 				}
	 			}
	 			
	 			if (!empty($files['styles'])) {
	 				foreach ($files['styles'] as $css) {
	 					$changedUrl = str_replace("sites/default/files/contentupload/libraries/","sites/all/libraries/h5plibraries/",$css->path);
	 					$css->path = $changedUrl;
	 						
	 					if (preg_match ('/:\/\//', $css->path) === 1) {
	 						// External file
	 						$libraryData->css[$css->path. $css->version] = file_get_contents($css->path);
	 					}
	 					else {
	 						// Local file
	 						H5peditor::buildCssPath(NULL, $this->h5p->url . dirname($css->path) . '/');
	 						$libraryData->css[$this->h5p->url . $css->path . $css->version] = preg_replace_callback('/url\([\'"]?(?![a-z]+:|\/+)([^\'")]+)[\'"]?\)/i', 'H5peditor::buildCssPath', file_get_contents($path . $css->path));
	 					}
	 				}
	 			}
 			}
 			
 		}else if($machineName == "H5P.InteractiveVideo")
 		{
	 		
 			$required_files = array();
 			$libraryData->javascript = array();
 			$libraryData->css = array();
 		/*	$required_files[] = "/sites/all/libraries/h5plibraries/Tether-1.0/scripts/tether.min.js?ver=1.0.1";
 			$required_files[] = "/sites/all/libraries/h5plibraries/Drop-1.0/js/drop.min.js?ver=1.0.1";
 			$required_files[] = "/sites/all/libraries/h5plibraries/H5P.Transition-1.0/transition.js?ver=1.0.2";
 			//$required_files[] = "/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/js/joubel-help-dialog.js?ver=1.2.7";
 			//$required_files[] = "/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/js/joubel-message-dialog.js?ver=1.2.7";
 			//$required_files[] = "/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/js/joubel-progress-circle.js?ver=1.2.7";
 			$required_files[] = "/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/js/joubel-simple-rounded-button.js?ver=1.2.7";
 			//$required_files[] = "/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/js/joubel-speech-bubble.js?ver=1.2.7";
 			//$required_files[] = "/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/js/joubel-throbber.js?ver=1.2.7";
 			$required_files[] = "/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/js/joubel-tip.js?ver=1.2.7";
 			//$required_files[] = "/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/js/joubel-slider.js?ver=1.2.7";
 			//$required_files[] = "/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/js/joubel-score-bar.js?ver=1.2.7";
 			//$required_files[] = "/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/js/joubel-progressbar.js?ver=1.2.7";
 			$required_files[] = "/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/js/joubel-ui.js?ver=1.2.7";
 			$required_files[] = "/sites/all/libraries/h5plibraries/H5P.Question-1.1/scripts/question.js?ver=1.1.7";
 			//$required_files[] = "/sites/all/libraries/h5plibraries/H5P.Blanks-1.4/js/blanks.js?ver=1.4.6";
 			//$required_files[] = "/sites/all/libraries/h5plibraries/H5P.Blanks-1.4/js/cloze.js?ver=1.4.6";
 			$required_files[] = "/sites/all/libraries/h5plibraries/H5P.DragNDrop-1.1/drag-n-drop.js?ver=1.1.2";
 			$required_files[] = "/sites/all/libraries/h5plibraries/H5P.DragNResize-1.2/H5P.DragNResize.js?ver=1.2.0";
 			$required_files[] = "/sites/all/libraries/h5plibraries/H5P.DragNBar-1.2/scripts/drag-n-bar.js?ver=1.2.1";
 			$required_files[] = "/sites/all/libraries/h5plibraries/H5P.DragNBar-1.2/scripts/context-menu.js?ver=1.2.1";
 			$required_files[] = "/sites/all/libraries/h5plibraries/H5P.DragNBar-1.2/scripts/dialog.js?ver=1.2.1";
 			$required_files[] = "/sites/all/libraries/h5plibraries/H5P.DragNBar-1.2/scripts/drag-n-bar-element.js?ver=1.2.1";
 			$required_files[] = "/sites/all/libraries/h5plibraries/jQuery.ui-1.10/js/jquery-ui.js?ver=1.10.14";
 			$required_files[] = "/sites/all/libraries/h5plibraries/H5P.AdvancedText-1.1/text.js?ver=1.1.0";
 			$required_files[] = "/sites/all/libraries/h5plibraries/H5P.Image-1.0/image.js?ver=1.0.25";
 			$required_files[] = "/sites/all/libraries/h5plibraries/H5PEditor.Wizard-1.0/Scripts/Wizard.js?ver=1.0.9";
 		//	$required_files[] = "/sites/all/libraries/h5plibraries/H5PEditor.DragQuestion-1.4/H5PEditor.DragQuestion.js?ver=1.4.3";
 		//	$required_files[] = "/sites/all/libraries/h5plibraries/H5PEditor.DragQuestion-1.4/H5PEditor.DynamicCheckboxes.js?ver=1.4.3";
 		//	$required_files[] = "/sites/all/libraries/h5plibraries/H5P.DragQuestion-1.5/js/dragquestion.js?ver=1.5.7";
 		//	$required_files[] = "/sites/all/libraries/h5plibraries/H5P.DragText-1.4/drag-text.js?ver=1.4.5";
 			$required_files[] = "/sites/all/libraries/h5plibraries/H5PEditor.Timecode-1.0/timecode.js?ver=1.0.11";
 			$required_files[] = "/sites/all/libraries/h5plibraries/H5P.GoToQuestion-1.0/scripts/go-to-question.js?ver=1.0.2";
 		//	$required_files[] = "/sites/all/libraries/h5plibraries/Shepherd-1.0/scripts/shepherd.js?ver=1.0.1";
 			//$required_files[] = "/sites/all/libraries/h5plibraries/H5P.GuidedTour-1.0/scripts/h5p-guided-tour.js?ver=1.0.1";
 			$required_files[] = "/sites/all/libraries/h5plibraries/flowplayer-1.0/scripts/flowplayer-3.2.12.min.js?ver=1.0.4";
 			$required_files[] = "/sites/all/libraries/h5plibraries/H5P.Video-1.2/scripts/youtube.js?ver=1.2.0";
 			$required_files[] = "/sites/all/libraries/h5plibraries/H5P.Video-1.2/scripts/html5.js?ver=1.2.0";
 			$required_files[] = "/sites/all/libraries/h5plibraries/H5P.Video-1.2/scripts/flash.js?ver=1.2.0";
 			$required_files[] = "/sites/all/libraries/h5plibraries/H5P.Video-1.2/scripts/video.js?ver=1.2.0";
 			$required_files[] = "/sites/all/libraries/h5plibraries/H5P.InteractiveVideo-1.9/scripts/interaction.js?ver=1.9.0";
 			$required_files[] = "/sites/all/libraries/h5plibraries/H5P.InteractiveVideo-1.9/scripts/interactive-video.js?ver=1.9.0";
 //			$required_files[] = "/sites/all/libraries/h5plibraries/H5PEditor.UrlField-1.0/link-widget.js?ver=1.0.0";
 //			$required_files[] = "/sites/all/libraries/h5plibraries/H5P.Link-1.1/link.js?ver=1.1.0";
 //			$required_files[] = "/sites/all/libraries/h5plibraries/H5P.MarkTheWords-1.5/mark-the-words.js?ver=1.5.5";
 			$required_files[] = "/sites/all/libraries/h5plibraries/EmbeddedJS-1.0/js/ejs_production.js?ver=1.0.3";
 			$required_files[] = "/sites/all/libraries/h5plibraries/EmbeddedJS-1.0/js/ejs_viewhelpers.js?ver=1.0.3";
 //			$required_files[] = "/sites/all/libraries/h5plibraries/H5P.MultiChoice-1.6/js/multichoice.js?ver=1.6.0";
 //			$required_files[] = "/sites/all/libraries/h5plibraries/H5P.SoundJS-1.0/soundjs-0.6.0.min.js?ver=1.0.0";
// 			$required_files[] = "/sites/all/libraries/h5plibraries/H5PEditor.VerticalTabs-1.1/vertical-tabs.js?ver=1.1.3";
// 			$required_files[] = "/sites/all/libraries/h5plibraries/H5P.SingleChoiceSet-1.3/scripts/sound-effects.js?ver=1.3.5";
// 			$required_files[] = "/sites/all/libraries/h5plibraries/H5P.SingleChoiceSet-1.3/scripts/event-emitter.js?ver=1.3.5";
// 			$required_files[] = "/sites/all/libraries/h5plibraries/H5P.SingleChoiceSet-1.3/scripts/result-slide.js?ver=1.3.5";
// 			$required_files[] = "/sites/all/libraries/h5plibraries/H5P.SingleChoiceSet-1.3/scripts/solution-view.js?ver=1.3.5";
// 			$required_files[] = "/sites/all/libraries/h5plibraries/H5P.SingleChoiceSet-1.3/scripts/single-choice-alternative.js?ver=1.3.5";
// 			$required_files[] = "/sites/all/libraries/h5plibraries/H5P.SingleChoiceSet-1.3/scripts/single-choice.js?ver=1.3.5";
 //			$required_files[] = "/sites/all/libraries/h5plibraries/H5P.SingleChoiceSet-1.3/scripts/single-choice-set.js?ver=1.3.5";
 //			$required_files[] = "/sites/all/libraries/h5plibraries/H5PEditor.SummaryTextualEditor-1.0/summary-textual-editor.js?ver=1.0.2";
 	//		$required_files[] = "/sites/all/libraries/h5plibraries/H5P.Summary-1.4/js/summary.js?ver=1.4.5";
 //			$required_files[] = "/sites/all/libraries/h5plibraries/H5P.Table-1.1/scripts/table.js?ver=1.1.0";
 //			$required_files[] = "/sites/all/libraries/h5plibraries/H5P.Text-1.1/scripts/text.js?ver=1.1.0";
 			$required_files[] = "/sites/all/libraries/h5plibraries/H5PEditor.Duration-1.0/scripts/duration.js?ver=1.0.4";
 			$required_files[] = "/sites/all/libraries/h5plibraries/H5PEditor.InteractiveVideo-1.9/Scripts/image-radio-button-group.js?ver=1.9.1";
 			$required_files[] = "/sites/all/libraries/h5plibraries/H5PEditor.InteractiveVideo-1.9/Scripts/interactive-video-editor.js?ver=1.9.1";
 			$required_files[] = "/sites/all/libraries/h5plibraries/H5PEditor.InteractiveVideo-1.9/Scripts/guided-tours.js?ver=1.9.1";
 			*/
 			
 			if($_GET["H5P_DEVELOPMENT_MODE"] == 1)
 			{
	 			foreach ($files['scripts'] as $script)
	 			{
	 				//if(in_array("/".$script->path.$script->version,$required_files))
	 				{
	 					
	 												 
	 					$changedUrl = str_replace("sites/default/files/contentupload/libraries/","sites/all/libraries/h5plibraries/",$script->path);
	 					$script->path = $changedUrl;
	 					if (preg_match ('/:\/\//', $script->path) === 1) {
	 						// External file
	 						$libraryData->javascript[$script->path . $script->version] = "\n" . file_get_contents($script->path);
	 					}
	 					else {
	 						// Local file
	 						
	 						$libraryData->javascript[$this->h5p->url . $script->path . $script->version] = "\n" . file_get_contents($path . $script->path);
	 					}
	 				}
	 			}
	 			
	 			if (!empty($files['styles'])) {
	 				foreach ($files['styles'] as $css) {
	 					if (preg_match ('/:\/\//', $css->path) === 1) {
	 						// External file
	 						$libraryData->css[$css->path. $css->version] = file_get_contents($css->path);
	 					}
	 					else {
	 						// Local file
	 						expDebug::dPrint("css tttt5:".$css->path);
	 						H5peditor::buildCssPath(NULL, $this->h5p->url . dirname($css->path) . '/');
	 						$changedUrl = str_replace("sites/default/files/contentupload/libraries/","sites/all/libraries/h5plibraries/",$css->path);
	 						$css->path = $changedUrl;
	 						
	 						$libraryData->css[$this->h5p->url . $css->path . $css->version] = preg_replace_callback('/url\([\'"]?(?![a-z]+:|\/+)([^\'")]+)[\'"]?\)/i', 'H5peditor::buildCssPath', file_get_contents($path . $css->path));
	 					}
	 				}
	 			} 
 			}
	  
 		}
 	} 
    
    // Add translations for libraries. - Not required for e1. If it is enabled then editor will not load in multi lang
    /*foreach ($libraries as $library) {
      $language = $this->getLibraryLanguage($library['machineName'], $library['majorVersion'], $library['minorVersion'], $languageCode);
      if ($language !== NULL) {
        $lang = '; H5PEditor.language["' . $library['machineName'] . '"] = ' . $language . ';';
        $libraryData->javascript[md5($lang)] = $lang;
      }
    }*/

    $retData = json_encode($libraryData);
    //expDebug::dPrint("H5P_DEVELOPMENT_MODE:::".$_GET["H5P_DEVELOPMENT_MODE"]."=sssss1:".$_SERVER["DOCUMENT_ROOT"]."/h5pmerge/videofilesjson.json");
    
    if($_GET["H5P_DEVELOPMENT_MODE"] == 1 && $machineName == "H5P.InteractiveVideo")
    	file_put_contents($_SERVER["DOCUMENT_ROOT"]."/h5pmerge/videofilesjson.json","var H5PVIDEO_RESOURCES =".$retData);
    else if($_GET["H5P_DEVELOPMENT_MODE"] ==1 && $machineName == "H5P.CoursePresentation")
    	file_put_contents($_SERVER["DOCUMENT_ROOT"]."/h5pmerge/courseprefilesjson.json","var H5PCOURSEPRE_RESOURCES =".$retData);
    
    return $retData;
  }

  /**
   * This function will prefix all paths within a CSS file.
   * Copied from Drupal 6.
   *
   * @staticvar type $_base
   * @param type $matches
   * @param type $base
   * @return type
   */
  public static function buildCssPath($matches, $base = NULL) {
    static $_base;
    // Store base path for preg_replace_callback.
    if (isset($base)) {
      $_base = $base;
    }

    // Prefix with base and remove '../' segments where possible.
    $path = $_base . $matches[1];
    $last = '';
    while ($path != $last) {
      $last = $path;
      $path = preg_replace('`(^|/)(?!\.\./)([^/]+)/\.\./`', '$1', $path);
    }
    return 'url('. $path .')';
  }
}
