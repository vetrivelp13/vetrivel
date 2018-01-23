<?php include 'header.php'; ?> 
  <div id="content-outer">
      <div id="breadcrumbs">
<div class="breadcrumb"><a href="index.php">Home</a> &rarr; <a href="#">Documentation</a><div><a href="#" data-count="none" data-related="" class="share-button">Tweet</a></div></div>      </div>

      <h1 id="title">The REST API</h1>

      <div id="content-inner">

        <div id="content-main">

 <!--  <div class="doc-updated">
    Updated on Mon, 2011-07-11 05:24  </div> -->

<div>
  
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<ol>
	<li class="toc-level-1"><a href="#otherres">The REST API</a>

<ol>
	<li class="toc-level-2"><a href="#">Comprehensive Resource Documentation   </a></li>
	<li class="toc-level-2"><a href="#Thingsapi">Things to know about the REST API  </a></li>
	<li class="toc-level-2"><a href="#Updateapi">Update API</a></li>
	<li class="toc-level-2"><a href="#historyapi">History of the ReST APIs   </a></li>
	<li class="toc-level-2"><a href="learnerapi_list.php">Learner functionality ReST APIs    </a></li>
	<li class="toc-level-2"><a href="adminapi.php">Admin functionality ReST APIs    </a></li>
	<li class="toc-level-2"><a href="#errorcode">Error Codes & Responses   </a></li>
	
	
	
</ol>

</li>

</ol>
</div>
</div>

<h2 id="otherres">Other Resourcess</h2>
<div class="section">
  <h3 id="Thingsapi">Things to know about the REST API </h3>
  <p>There are limits to how many calls and changes you can make in a day

API usage is limited with additional fair use limits to protect ExpertusONE from abuse.
  </p>
  
  <h3 id="Updateapi">Update API</h3>
  <p>When updating any entity using API if existing non mandatory field values need to be retained in the system, then the values need to be provided explicitly in the API otherwise non mandatory fields will be updated with empty values.</p>
  
  <h3>The API is entirely HTTP-based</h3>
  <p>
 Methods to retrieve data from the ExpertusONE API require a GET or POST request. API Methods that require a particular HTTP method will return an error if you do not make your request with the correct one. HTTP Response Codes are meaningful. 
 </p>
<h3>The API is a RESTful resource</h3>
<p>
The ExpertusONE API attempts to conform to the design principles of Representational State Transfer (REST). Simply change the format extension a request to get results in the format of your choice. The API presently supports the following data formats: XML and JSON. 
<p>
<h3 id="historyapi">History of the REST APIs</h3>
<p>The ExpertusONE API consists of two parts: Learner functionality APIs and admin functionality APIs. It is in our pipeline to unify the APIs, but until resources allow the REST API and Search API will remain as separate entities.

The ExpertusONE REST API methods allow developers to access core ExpertusONE data. This includes catalog search, registration, transcript and user information.

To learn about the methods and data available through the API, review the ExpertusONE API technical documentation.

The API is updated regularly with new features. We occasionally deprecate features. All of these changes are communicated through mail to the partners.
 
</p>
<h3 id="errorcode">Error Codes & Responses</h3>
<p> The ExpertusONE API attempts to return appropriate HTTP status codes for every request. It is possible to suppress response codes for the REST API. </p>
<ul>
    <li>200 OK: Success!</li>
   <li> 304 Not Modified: There was no new data to return.</li>
   <li> 400 Bad Request: The request was invalid. An accompanying error message will explain why. This is the status code will be returned during rate limiting.</li>
  <li>  401 Unauthorized: Authentication credentials were missing or incorrect.</li>
  <li>  403 Forbidden: The request is understood, but it has been refused. An accompanying error message will explain why. This code is used when requests are being denied due to update limits.</li>
  <li>  404 Not Found: The URI requested is invalid or the resource requested, such as a user, does not exists.</li>
   <li> 406 Not Acceptable: Returned by the Search API when an invalid format is specified in the request.</li>
  <li>  420 Enhance Your Calm: Returned by the Search and Trends API when you are being rate limited.</li>
  <li>  500 Internal Server Error: Something is broken. Please post to the group so the ExpertusONE team can investigate.</li>
  <li>  503 Service Unavailable: The ExpertusONE servers are up, but overloaded with requests. Try again later.</li>
</ul>

<p>Error Messages </p>
<p>When the ExpertusONE API returns error messages, it does so in your requested format. For example, an error from an XML method might look like this: </p>

<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<?php 
	include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/services/GlobalUtil.php";
    $util=new GlobalUtil();
    $config=$util->getConfig();
    $errorResponseFormat = isset($config['api_error_response_format']) && trim($config['api_error_response_format']) == "1" ? 1 :0;
	
    if($errorResponseFormat === 1) {
?>
	<pre class="code XML">&lt;?xml version='1.0' encoding='UTF-8' ?&gt;
	<span class="sc3"><span class="re1">&lt;errors<span class="re2">&gt;</span></span></span>
		<span class="sc3"><span class="re1">&lt;error<span class="re2">&gt;</span></span></span>
		         <span class="sc3"><span class="re1">&lt;type<span class="re2">&gt;</span></span></span>OAUTH_FAILED<span class="sc3"><span class="re1">&lt;/type<span class="re2">&gt;</span></span></span>
		         <span class="sc3"><span class="re1">&lt;field<span class="re2">&gt;</span></span></span><span class="sc3"><span class="re1">&lt;/field<span class="re2">&gt;</span></span></span>
		         <span class="sc3"><span class="re1">&lt;message<span class="re2">&gt;</span></span></span>OAuth authentication failed.<span class="sc3"><span class="re1">&lt;/message<span class="re2">&gt;</span></span></span>
		<span class="sc3"><span class="re1">&lt;/error<span class="re2">&gt;</span></span></span>
	<span class="sc3"><span class="re1">&lt;/errors<span class="re2">&gt;</span></span></span></pre>
<?php } else { 
?>
	<pre class="code XML">&lt;?xml version='1.0' encoding='UTF-8' ?&gt;
	<span class="sc3"><span class="re1">&lt;error<span class="re2">&gt;</span></span></span>
	         <span class="sc3"><span class="re1">&lt;error_code<span class="re2">&gt;</span></span></span>OAUTH_FAILED<span class="sc3"><span class="re1">&lt;/error_code<span class="re2">&gt;</span></span></span>
	         <span class="sc3"><span class="re1">&lt;short_msg<span class="re2">&gt;</span></span></span>Authentication Failed<span class="sc3"><span class="re1">&lt;/short_msg<span class="re2">&gt;</span></span></span>
	         <span class="sc3"><span class="re1">&lt;long_msg<span class="re2">&gt;</span></span></span>OAuth authentication failed.<span class="sc3"><span class="re1">&lt;/long_msg<span class="re2">&gt;</span></span></span>
	         <span class="sc3"><span class="re1">&lt;corrective_solution<span class="re2">&gt;</span></span></span>Check your API key.<span class="sc3"><span class="re1">&lt;/corrective_solution<span class="re2">&gt;</span></span></span>
	<span class="sc3"><span class="re1">&lt;/error<span class="re2">&gt;</span></span></span></pre>
<?php }
?>
</div>
</div>



</div>


</div>
        </div>
   
      </div>    </div>

   <?php include 'footer.php'; ?> 