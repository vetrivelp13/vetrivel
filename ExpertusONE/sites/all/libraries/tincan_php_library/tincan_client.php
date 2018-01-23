<?php

require_once('api.php');
include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/services/GlobalUtil.php";

final class TinCanClient {

  const VERSION = "1";
  const STR_PHP = 'php';
  const STR_JSON = 'json';
  
  public $subdomain = 'api';
  public $tincan_home_url = '';
  public $key = '';
  public $secret = '';
  public $response_format = self::STR_JSON;
  public $version = self::VERSION;
  public $default_page = 1;
  public $default_limit = 20;
  public $api_client;
  public $lrs_enabled;

  public function api_base_url() {
    return $this->tincan_home_url . '/api/' . 'v' . $this->version;
  }

  public static function Instance() {
    static $inst = null;
    if ($inst === null) {
      $inst = new TinCanClient();
    }
    return $inst;
  }

  /**
   * Private constuctor so nobody else can instance it.
   */
  private function __construct() {
  	$util=new GlobalUtil();
  	$config=$util->getConfig();
  	  $this->key = $config['tincan_key'];
  	  $this->secret = $config['tincan_secret'];
  	  $this->tincan_home_url = $config['tincan_home_url'];
  	  $this->lrs_enabled = $config['lrs_enable'];
  }

  private function request_headers() {
    $ver = $this->version ? $this->version . '.0.0' : '1.0.0';
    return array('Content-Type' => 'application/json', 'X-Experience-API-Version' => $ver);
  }

  private function default_params() {
    return array("page" => $this->default_page, "limit" => $this->default_limit,
        "key" => $this->key, "secret" => $this->secret, "response_format" => $this->response_format);
  }

  private function get_api_client() {
    if ($this->api_client == null) {
      $this->api_client = new TinCanApi(array('base_url' => $this->api_base_url(),
          'format' => $this->response_format, 'headers' => $this->request_headers()));
    }

    return $this->api_client;
  }

  private function url_with_query_string($url, $query_str) {
    return ((strlen($query_str) > 0) ? $url .= "?" . $query_str : $url);
  }
  
  
  /*
  *BEGIN OF API FUNCTIONS
  */
  
  public function list_verbs($params = array()) {
    $req_params = array_merge($this->default_params(), $params);
    return $this->get_api_client()->get("verbs", $req_params);
  }

  public function list_agents($params = array()) {
    $req_params = array_merge($this->default_params(), $params);
    return $this->get_api_client()->get("agents", $req_params);
  }

  public function list_statements($params = array()) {
    $req_params = array_merge($this->default_params(), $params);
    return $this->get_api_client()->get("statements", $req_params);
  }


  public function create_statements($params = array()) {
  	expDebug::dPrint("create staete".print_r($params,true)."lrs enabled or not".$this->lrs_enabled,5);
  	if(!empty($this->lrs_enabled)){
  	  $req_params = array_merge($this->default_params(), $params);
      unset($req_params["statements"]);
      return $this->get_api_client()->post("statements", $req_params, json_encode($params));
  	}
  }

  public function update_statements($params = array()) {
    $req_params = array_merge($this->default_params(), $params);
    unset($req_params["statements"]);
    return $this->get_api_client()->put("statements", $req_params, json_encode($params));
  }

  public function delete_statements($params = array()) {
    $req_params = array_merge($this->default_params(), $params);
    return $this->get_api_client()->delete("statements", $req_params);
  }

  public function get_activity_state($params = array(), $query_str = '') {
    $req_params = array_merge($this->default_params(), $params);
    $end_point_url = $this->url_with_query_string("activities/state", $query_str);
    return $this->get_api_client()->get($end_point_url, $req_params);
  }

  public function create_activity_state($params = array(), $query_str = '') {
    $req_params = array_merge($this->default_params(), $params);
    $end_point_url = $this->url_with_query_string("activities/state", $query_str);
    return $this->get_api_client()->post($end_point_url, $req_params);
  }

  public function update_activity_state($params = array(), $query_str = '') {
    $req_params = array_merge($this->default_params(), $params);
    $end_point_url = $this->url_with_query_string("activities/state", $query_str);
    return $this->get_api_client()->put($end_point_url, $req_params);
  }

  public function delete_activity_state($params = array(), $query_str = '') {
    $req_params = array_merge($this->default_params(), $params);
    $end_point_url = $this->url_with_query_string("activities/state", $query_str);
    return $this->get_api_client()->delete($end_point_url, $req_params);
  }

  public function get_activities_report($params = array()) {
    $req_params = array_merge($this->default_params(), $params);
    return $this->get_api_client()->get("reports/activities", $req_params);
  }

  public function get_actors_report($params = array()) {
    $req_params = array_merge($this->default_params(), $params);
    return $this->get_api_client()->get("reports/actors", $req_params);
  }

  public function get_verbs_report($params = array()) {
    $req_params = array_merge($this->default_params(), $params);
    return $this->get_api_client()->get("reports/verbs", $req_params);
  }

  public function get_activity_details_report($params = array()) {
    $req_params = array_merge($this->default_params(), $params);
    return $this->get_api_client()->get("reports/activity", $req_params);
  }

  public function get_actor_details_report($params = array()) {
    $req_params = array_merge($this->default_params(), $params);
    return $this->get_api_client()->get("reports/actor", $req_params);
  }
}

?>
