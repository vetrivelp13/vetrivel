<?php

/**
 * @file
 * Definition of AntivirusScan.
 */

class AntivirusScan {

  public $scanners = array();

  public $targets = array();

  public $options = array();

  public $debug = FALSE;

  function __construct($scanners = array()) {
    if (count($scanners)) {
      $this->addScanners($scanners);
    }
    else {
      $this->addScanners(variable_get('antivirus_enabled_scanners', array()));
    }

    $this->debug = variable_get('antivirus_debug', FALSE);
  }

  public static function getScanner($scanners = array()) {
    return new AntivirusScan($scanners);
  }

  public function addScanners($scanners) {
    $this->scanners = array_merge($this->scanners, $scanners);

    return $this;
  }

  /**
   * Adds files to be scanned.
   *
   * @param $targets
   *   An array of files to be scanned. The contents of the array may be file
   *   IDs or full file objects.
   */
  public function addTargets($targets) {
    foreach ($targets as $key => $target) {
      // If the target is a file ID, load the file and extract its URI.
      if (is_numeric($target)) {
        $file = file_load($target);
        $target = $file->uri;
        $targets[$key] = $target;
      }
      // If the target is a file object, extract its URI.
      elseif (isset($target->uri)) {
        $target = $target->uri;
        $targets[$key] = $target;
      }

      // Remove stream wrappers for local file scanning.
      $scheme = file_uri_scheme($targets[$key]);
      if ($scheme && file_stream_wrapper_valid_scheme($scheme)) {
        $targets[$key] = file_stream_wrapper_get_instance_by_scheme($scheme)->realpath() . DIRECTORY_SEPARATOR . file_uri_target($targets[$key]);
      }
    }

    $this->targets = array_merge($this->targets, $targets);

    return $this;
  }

  public function addOptions($options) {
    $this->options = array_merge($this->options, $options);

    return $this;
  }

  public function scan() {
    $result = ANTIVIRUS_SCAN_OK;

    if (empty($this->scanners)) {
      return ANTIVIRUS_SCAN_ERROR;
    }

    foreach ($this->scanners as $scanner) {
      $scanner = antivirus_get_scanners($scanner);
      $scanner = new $scanner['class']();

      foreach ($this->targets as $target) {
        $result = $scanner->scan($target, $this->options, $this->debug) | $result;
      }
    }

    return $result;
  }

}

