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
	<li class="toc-level-1"><a href="#otherres">List of APIs</a>

<ol>
	<li class="toc-level-2"><a href="#authentication">Authentication & Authorization API   </a></li>
	<li class="toc-level-2"><a href="#catalogsearch">Catalog Search API  </a></li>
	<li class="toc-level-2"><a href="#localationapi">Get Locations API   </a></li>
		<li class="toc-level-2"><a href="#getfacilities">Get Facilities API    </a></li>
	<li class="toc-level-2"><a href="#deliveryapi">Get Delivery Types API    </a></li>
	<li class="toc-level-2"><a href="#coursedetails">Get Course Details API   </a></li>
	<li class="toc-level-2"><a href="#classdeails">Get Class Details API   </a></li>
	<li class="toc-level-2"><a href="#registrationapi">Registration API  </a></li>
	<li class="toc-level-2"><a href="#enrollmentsapi">List Enrollments API   </a></li>
	
	<li class="toc-level-2"><a href="#cancelenrollment">Cancel Enrollment API   </a></li>
	<li class="toc-level-2"><a href="#lanchmodules">Get Launch Modules API  </a></li>
	<li class="toc-level-2"><a href="#scoreapi">Update Score API  </a></li>
	<li class="toc-level-2"><a href="#learnerannouncement">Learner Announcement API   </a></li>
	
</ol>

</li>

</ol>
</div>
</div>

<h2 id="otherres">List Of APIs</h2>
<div class="section">
  <h3 id="authentication">Authentication & Authorization API </h3>
  <p><strong>Description:</strong> This api is used to do the authentication against ExpertusONE LMS server. It is three steps process.   </p>
  
  <ul>
  <li>Get Request Token</li>
<li>Do Authorization</li>
<li>Get Access token after successful authorization.</li>
  </ul>
  <p><strong>End Point URL:</strong> to acquire request token - <a href="<?php echo getDomainDoc();?>/apis/core/oauth/requestToken"><?php echo getDomainDoc();?>/apis/core/oauth/requestToken</a><br> To authorize - <a href="<?php echo getDomainDoc();?>/apis/core/oauth/authorizeToken"><?php echo getDomainDoc();?>/apis/core/oauth/authorizeToken</a><br> To acquire access token - <a href="<?php echo getDomainDoc();?>/apis/core/oauth/accessToken"><?php echo getDomainDoc();?>/apis/core/oauth/accessToken</a><br> 
<strong>Pre requisites:</strong> consumer key and secret key needs to be purchased.  consumer key and secret key needs to be purchased. <br>
<strong>Input Parameters:</strong> Required CONSUMER_KEY, CONSUMER_SECRET and OAUTH_CALLBACK.</p>

<p>Example code snippet to do the authentication. </p>



<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre>
	require_once('../expertusone_oauth.php');
	require_once('../config.php');
	
	/* Build ExpertusONE_OAuth object with client credentials. It should be configured in config.php */
	$connection = new ExpertusONE_OAuth(CONSUMER_KEY, CONSUMER_SECRET);
	
	/* Get temporary credentials. */
	
	$request_token = $connection->getRequestToken(OAUTH_CALLBACK);
	
	/* Save temporary credentials to session. */
	$_SESSION['oauth_token'] = $token = $request_token['oauth_token'];
	$_SESSION['oauth_token_secret'] = $request_token['oauth_token_secret'];
	
	/* If last connection failed don't display authorization link. */
	switch ($connection->http_code) {
	case 200:
	
	/* Build authorize URL and redirect user to Twitter. */
	$url = $connection->getAuthorizeURL($token,"true");
	header('Location: ' . $url); 
	break;
	default:
	/* Show notification if something went wrong. */
	echo 'Could not connect to ExpertusONE. Refresh the page or try again later.';
	}
	</pre>
</div>
</div> 
</div>
<p>Above snippet will be doing the following process. </p>
<p>
1.Acquire request token from ExpertusONE LMS server.<br>
2.Redirect user to ExpertusONE server to authorize account. 
</p>
<p>After successful authorization, ExpertusONE server call the callback url along with oauth_verifier. Following code snippet will help to get the access token. </p>
<p>Code snippet from callback.php </p>

<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre>
	session_start();
	require_once('expertusone_oauth.php');
	require_once('config.php');
	
	$access_token= array();
	
	/* Create ExpertusONE_OAuth object with app key/secret and token key/secret from default phase */
	$connection = new ExpertusONE_OAuth(CONSUMER_KEY, CONSUMER_SECRET, 
	$_SESSION['oauth_token'], $_SESSION['oauth_token_secret']);
	
	
	/* Request access tokens from twitter */
	$access_token = $connection->getAccessToken($_REQUEST['oauth_verifier']);
	
	/* Save the access tokens. Normally these would be saved in a database for future use. */
	$_SESSION['access_token'] = $access_token;
	
	/* Remove no longer needed request tokens */
	unset($_SESSION['oauth_token']);
	unset($_SESSION['oauth_token_secret']);
	
	/* If HTTP response is 200 continue otherwise send to connect page to retry */
	if (200 == $connection->http_code) {
	/* The user has been verified and the access tokens can be saved for future use */
	$_SESSION['status'] = 'verified';
	
	/* store the access token in cookie, it will be utilized by the other apis wherever necessary. */
	setcookie("oauth_token", $access_token['oauth_token'], time()+(3600*24));  /* expire in 24 hours */
	setcookie("oauth_token_secret", $access_token['oauth_token_secret'], time()+(3600*24));  /* expire in 24 hours */
	setcookie("userid", $_REQUEST["userid"], time()+(3600*24));  /* expire in 24 hours */
	
	$callapi=true;
	
	}
	else
	{
	header('Location: clearsessions.php');
	}
	if($callapi==true)
	{
	echo "Authentication Successful. Call necessary APIs here.";
	}
	else
	{
	echo "Authentication unsuccessful. Contact customer care if it is persist.";
	}
 </pre>
</div>
</div> 
</div>


<h3 id="catalogsearch">Catalog Search API</h3>
<p><strong>Description:</strong> This api is used to retrieve the list of available courses/certifications/curriculums in the system.<br>
<strong>End Point URL:</strong> <a href="<?php echo getDomainDoc();?>/apis/ext/catalog/CatalogSearchResults "><?php echo getDomainDoc();?>/apis/ext/catalog/CatalogSearchResults</a><br>
<strong>Pre requisites:</strong> Access token needs to be populated before access this api.<br>
<strong>Input Parameters:</strong> Following will be the list of Optional/Mandatory parameters to make this api call.  
 </p>
<h3>Optional Params </h3>
<ul><li>title -  course title needs to be searched</li>
<li>type - catalog type - course/class </li>
<li>startdate - Format (MM/DD/YYYY)</li>
<li>enddate  - Format (MM/DD/YYYY)</li>
<li>dl_type - Delivery type. Possible values for this field will be leveraged from profile lookup master.</li>
<li>lg_type - Language Code.  Possible values for this field will be leveraged from profile lookup master.</li>
<li>location - Name of the location</li>
<!--   <li>region</li> --> 
<li>price_start</li>
<li>price_end</li>
<!--   <li>rating</li> -->
<li>sortby - Possible value are az or za or Time or ClassStartDate</li>
<li>limit</li>

</ul>
<h3>Required params </h3>
<ul>
   <li> returntype - xml (Note: API will return either xml or json object.)</li>
   <li> userid - Valid user id</li>
    <li>display_columns - Needed columns from the response schema.(It is comma separated values).</li>
  <li>  CONSUMER_KEY</li>
   <li> CONSUMER_SECRET_KEY</li>
    <li>ACCESS_OAUTH_TOKEN (It will be available in cookies or sessions after successful authentication.)</li>
    <li>ACCESS_TOKEN_SECRET (It will be available in cookies or sessions after successful authentication.)</li>

</ul>
<p><strong>API Response Schema</strong> API will give the following columns as response. </p>
<ul>
<li>crs_title</li>
 <li>   crs_id </li> 
  <li>  crs_code</li> 
  <li>  cls_id</li> 
 <li>   cls_code</li> 
 <li>   cls_title</li> 
 <li>   cls_short_description</li> 
 <li>   currency_type</li> 
 <li>   export_compliance</li> 
 <li>   delivery_type_name</li> 
 <li>   status</li> 
<li>   language</li> 
 <li>   location</li> 
 <li>   price</li> 
 <li>  session_id</li> 
  <li>  sess_start_date</li> 
 <li>   sess_start_time</li> 
 <li>   sess_end_time</li> 
 <li>   sess_end_date</li> 
  <li>  country_name</li> 
<li>    language_code</li> 
   <li> country_code</li> 
 <li>   registration_end_on</li> 

</ul>

<p>Example code snippet to make this api call. </p>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre>
 	session_start();
	require_once('expertusone_oauth.php');
	require_once('config.php');
 
	/* retrieve access_token from cookies */
  	$access_oauth_token=$_COOKIE["oauth_token"];
  	$access_token_secret=$_COOKIE["oauth_token_secret"];
  	$userid=$_COOKIE["userid"];
 
 	/*set necessary parameters for this api call. */
  	$_GET["oauth_token"]=$access_oauth_token;
  	$_GET["oauth_token_secret"]=$access_token_secret;
  	$_GET["userid"]=$userid;
  	$_GET["display_columns"]="crs_code,crs_title";
  	$_GET["limit"]="10";
  	$_GET["title"]="WBT";  // retrieve only WBT titled courses.
  	$_GET["returntype"]="xml";   	
    /* Create connection using client credentials and access tokens */
 
  	$connection = new ExpertusONE_OAuth(CONSUMER_KEY, CONSUMER_SECRET, $access_oauth_token,$access_token_secret);
  	$content = $connection->get("/apis/ext/catalog/CatalogSearchResults",$_GET);
        echo $content;
	</pre>
</div>
</div> 
</div>
<p>Sample output for XML returntype </p>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre class="code XML">
<span class="sc3"><span class="re1">&lt;?xml</span>
 <span class="re0">version</span>=<span class="st0">"1.0"</span> 
 <span class="re0">encoding</span>=<span class="st0">"UTF-8"</span> 
 <span class="re2">?&gt;</span></span>
<span class="sc3"><span class="re1">&lt;results<span class="re2">&gt;</span></span></span>
  <span class="sc3"><span class="re1">&lt;result<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;crs_code<span class="re2">&gt;</span></span>
    </span>WEP<span class="sc3"><span class="re1">&lt;/crs_code<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;crs_title<span class="re2">&gt;</span></span></span>Wellbore Planner - WBT<span class="sc3"><span class="re1">&lt;/crs_title<span class="re2">&gt;</span></span></span>
  <span class="sc3"><span class="re1">&lt;/result<span class="re2">&gt;</span></span></span>
<span class="sc3"><span class="re1">&lt;/results<span class="re2">&gt;</span></span></span>
</pre>
</div>
</div> 
</div>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre>
 
  &lt;title&gt;Chennai&lt;/title&gt; &lt;/result&gt;
   &lt;/results&gt; &lt;/code&gt; 
</pre>  
</pre>
</div>
</div> 

<p>Sample output for JSON returntype. </p>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre>
{"results":{"jsonResponse":[
                   {"crs_code":"WEP","crs_title":"Wellbore Planner - WBT"}
             ]  
            }
}
	</pre>
</div>
</div> 


<h3 id="localationapi"> Get Locations API</h3>
<p>
<strong>Description:</strong> This api is used to get the list of locations in the system.<br>
<strong>End Point URL:</strong> <a href="<?php echo getDomainDoc();?>/apis/ext/resources/ListResources"><?php echo getDomainDoc();?>/apis/ext/resources/ListResources</a><br>
<strong>Pre requisites:</strong> Access token needs to be populated before access this api.<br>
<strong>Input Parameters:</strong> Following will be the list of Optional/Mandatory parameters to make this api call. 
</p>
<h3>Optional Params </h3>
<ul><li>none</li></ul>
<h3>Required params  </h3>
<ul><li>actionkey - value for this param is "getlocations"</li>
<li>returntype - xml (Note: API will return either xml or json object.)</li>
<li>display_columns - Needed columns from the response schema.(It is comma separated values).</li>
<li>CONSUMER_KEY</li>
<li>CONSUMER_SECRET_KEY</li>
<li>ACCESS_OAUTH_TOKEN (It will be available in cookies or sessions after successful authentication.)</li>
<li>ACCESS_TOKEN_SECRET (It will be available in cookies 	or sessions after successful authentication.)</li>
</ul>
<p><strong>API Response Schema</strong> API will give the following columns as response. </p>
<ul><li>PID</li>
<li>TITLE</li>
<li>CODE</li>
</ul>
<p>Example code snippet to make this api call. </p>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre>
    session_start();
	require_once('expertusone_oauth.php');
	require_once('config.php');
 
	/* retrieve access_token from cookies */
  	$access_oauth_token=$_COOKIE["oauth_token"];
  	$access_token_secret=$_COOKIE["oauth_token_secret"];
  	$userid=$_COOKIE["userid"];
 
 	/*set necessary parameters for this api call. */
  	$_GET["oauth_token"]=$access_oauth_token;
  	$_GET["oauth_token_secret"]=$access_token_secret;
  	$_GET["userid"]=$userid;
  	$_GET["display_columns"]="title,code";
  	$_GET["returntype"]="xml";   	
  	$_GET["actionkey"]="getlocations";   	
 
    /* Create connection using client credentials and access tokens */
 
  	$connection = new ExpertusONE_OAuth(CONSUMER_KEY, CONSUMER_SECRET, $access_oauth_token,$access_token_secret);
  	$content = $connection->get("/apis/ext/resources/ListResources",$_GET);
        echo $content;
	</pre>
</div>
</div> 

<p>Sample output for XML returntype </p>

<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre>
 
  &lt;title&gt;Chennai&lt;/title&gt; &lt;/result&gt;
   &lt;/results&gt; &lt;/code&gt; 
</pre>  
</pre>
</div>
</div> 

<p>Sample output for JSON returntype. </p>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre>
   {"results":{"jsonResponse":[
                  {"code":"213","title":"Chennai"}
             ]  
            }
}</pre>
</div>
</div> 

<h3 id="getfacilities">Get Facilities API</h3>
<p><strong>Description:</strong> This api is used to get the list of facilities in the system.<br>
<strong>End Point URL:</strong> <a href="<?php echo getDomainDoc();?>/apis/ext/resources/ListResources"><?php echo getDomainDoc();?>/apis/ext/resources/ListResources</a><br>
<strong>Pre requisites:</strong> Access token needs to be populated before access this api.<br>
<strong>Input Parameters:</strong> Following will be the list of Optional/Mandatory parameters to make this api call. </p>
<h3>Optional Params </h3>
<ul><li>none</li></ul>
<h3>Required params  </h3>
<ul><li>actionkey - value for this param is "getfacilities"</li>
<li>returntype - xml (Note: API will return either xml or json object.)</li>
<li> display_columns - Needed columns from the response schema.(It is comma separated values).</li>
<li>  CONSUMER_KEY</li>
  <li>      CONSUMER_SECRET_KEY</li>
  <li>      ACCESS_OAUTH_TOKEN (It will be available in cookies or sessions after successful authentication.)</li>
   <li>     ACCESS_TOKEN_SECRET (It will be available in cookies or sessions after successful authentication.)</li>
</ul>
<p>API Response Schema API will give the following columns as response. </p>
<ul><li>PID</li>
   <li> TITLE</li>
   <li> CODE</li>
  <li>  DESCRIPTION</li>
</ul>
<p>Example code snippet to make this api call. </p>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre>
	session_start();
	require_once('expertusone_oauth.php');
	require_once('config.php');
 
	/* retrieve access_token from cookies */
  	$access_oauth_token=$_COOKIE["oauth_token"];
  	$access_token_secret=$_COOKIE["oauth_token_secret"];
  	$userid=$_COOKIE["userid"];
 
 	/*set necessary parameters for this api call. */
  	$_GET["oauth_token"]=$access_oauth_token;
  	$_GET["oauth_token_secret"]=$access_token_secret;
  	$_GET["userid"]=$userid;
  	$_GET["display_columns"]="title,code";
  	$_GET["returntype"]="xml";   	
  	$_GET["actionkey"]="getfacilities";   	
 
    /* Create connection using client credentials and access tokens */
 
  	$connection = new ExpertusONE_OAuth(CONSUMER_KEY, CONSUMER_SECRET, $access_oauth_token,$access_token_secret);
  	$content = $connection->get("/apis/ext/resources/ListResources",$_GET);
    echo $content;
	</pre>
</div>
</div> 
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre>
 
  &lt;title&gt;Chennai&lt;/title&gt; &lt;/result&gt;
   &lt;/results&gt; &lt;/code&gt; 
</pre>  

</div>
</div> 
<p>Sample output for JSON returntype. </p>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre>
{"results":{"jsonResponse":[
                  {"code":"213","title":"Chennai"}
             ]  
            }
}
</pre>
</div>
</div> 

<h3 id="deliveryapi">Get DeliveryTypes API</h3>
<p><strong>Description:</strong> This api is used to get the list of delivery types in the system.<br>
<strong>End Point URL:</strong> <a href="<?php echo getDomainDoc();?>/apis/ext/resources/ListResources"><?php echo getDomainDoc();?>/apis/ext/resources/ListResources</a>	<br>
<strong>Pre requisites:</strong> Access token needs to be populated before access this api.<br>
<strong>Input Parameters:</strong> Following will be the list of Optional/Mandatory parameters to make this api call. </p>
<h3>Optional Params </h3>
<ul><li>none</li></ul>
<h3>Required params  </h3>
<ul><li>actionkey - value for this param is "getdeliverytypes"</li>
<li>returntype - xml (Note: API will return either xml or json object.)
<li>display_columns - Needed columns from the response schema.(It is comma separated values).</li>
<li>CONSUMER_KEY</li>
<li>CONSUMER_SECRET_KEY</li>
<li>ACCESS_OAUTH_TOKEN (It will be available in cookies or sessions after successful authentication.)</li>
<li>ACCESS_TOKEN_SECRET (It will be available in cookies or sessions after successful authentication.)</li>
</ul>
<p>API Response Schema API will give the following columns as response.</p> 
<ul>
<li>ID</li>
<li>NAME</li>
<li> CODE</li>
<li>LANG_CODE</li>
<li>ATTR1</li>
</ul>
<p>Example code snippet to make this api call. </p>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre>
    session_start();
	require_once('expertusone_oauth.php');
	require_once('config.php');
 
	/* retrieve access_token from cookies */
  	$access_oauth_token=$_COOKIE["oauth_token"];
  	$access_token_secret=$_COOKIE["oauth_token_secret"];
  	$userid=$_COOKIE["userid"];
 
 	/*set necessary parameters for this api call. */
  	$_GET["oauth_token"]=$access_oauth_token;
  	$_GET["oauth_token_secret"]=$access_token_secret;
  	$_GET["userid"]=$userid;
  	$_GET["display_columns"]="id,name,code";
  	$_GET["returntype"]="xml";   	
  	$_GET["actionkey"]="getdeliverytypes";   	
 
    /* Create connection using client credentials and access tokens */
 
  	$connection = new ExpertusONE_OAuth(CONSUMER_KEY, CONSUMER_SECRET, $access_oauth_token,$access_token_secret);
  	$content = $connection->get("/apis/ext/resources/ListResources",$_GET);           
</pre>
</div>
</div> 
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre class="code XML"><span class="sc3"><span class="re1">&lt;?xml</span> <span class="re0">version</span>=<span class="st0">"1.0"</span> <span class="re0">encoding</span>=<span class="st0">"UTF-8"</span> <span class="re2">?&gt;</span></span>
<span class="sc3"><span class="re1">&lt;results<span class="re2">&gt;</span></span></span>
  <span class="sc3"><span class="re1">&lt;result<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;id<span class="re2">&gt;</span></span></span>35619<span class="sc3"><span class="re1">&lt;/id<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;code<span class="re2">&gt;</span></span></span>lrn_cls_dty_ilt 
  <span class="sc3"><span class="re1">&lt;name&gt;Classroom&lt;/name&gt; &lt;/result&gt;</span></span>
  <span class="sc3"><span class="re1">&lt;/results&gt; &lt;/code&gt; </span></span></pre>
</div>
</div> 
<p>Sample output for JSON returntype. </p>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre>
{"results":{"jsonResponse":[
                  {"id":"35619","code":"lrn_cls_dty_ilt","name":"Classroom"}
             ]  
            }
}</pre></div></div> 

<h3 id="coursedetails">Get Course Details API</h3>
<p><strong>Description:</strong> This api is used to retrieve the details of the course.<br>
<strong>End Point URL:</strong> <a href="<?php echo getDomainDoc();?>/apis/ext/coursedetails/GetCourseDetails"><?php echo getDomainDoc();?>/apis/ext/coursedetails/GetCourseDetails</a><br>
<strong>Pre requisites: </strong>Access token needs to be populated before access this api.<br>
<strong>Input Parameters: </strong>Following will be the list of Optional/Mandatory parameters to make this api call. </p>
<h3>Optional Params </h3>
<ul><li>none</li></ul>
<h3>Required params  </h3>
<ul><li>coursecode- Course Code</li>
<li>returntype - xml (Note: API will return either xml or json object.)</li>
<li>display_columns - Needed columns from the response schema.(It is comma separated values).</li>
<li>CONSUMER_KEY</li>
<li>CONSUMER_SECRET_KEY</li>
<li>ACCESS_OAUTH_TOKEN (It will be available in cookies or sessions after successful authentication.)</li>
<li>ACCESS_TOKEN_SECRET (It will be available in cookies or sessions after successful authentication.)</li>
</ul>

<p><strong>API Response Schema API</strong> will give the following columns as response.</p> 
<ul>
<li>code</li>
  <li>  title</li>
  <li>  description</li>
  <li> short_description</li> 
   <li> view_count</li>
</ul>
<p>Example code snippet to make this api call. </p>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre>
session_start();
require_once('expertusone_oauth.php');
require_once('config.php');

/* retrieve access_token from cookies */
$access_oauth_token=$_COOKIE["oauth_token"];
$access_token_secret=$_COOKIE["oauth_token_secret"];
$userid=$_COOKIE["userid"];

/*set necessary parameters for this api call. */
$_GET["oauth_token"]=$access_oauth_token;
$_GET["oauth_token_secret"]=$access_token_secret;
$_GET["userid"]=$userid;
$_GET["display_columns"]="title,code,description";
$_GET["limit"]="10";
$_GET["returntype"]="xml"; 
$_GET["coursecode"]="WEP"; 

/* Create connection using client credentials and access tokens */

$connection = new ExpertusONE_OAuth(CONSUMER_KEY, CONSUMER_SECRET, $access_oauth_token,$access_token_secret);
$content = $connection->get("/apis/ext/coursedetails/GetCourseDetails",$_GET);
echo $content;      
</pre>
</div>
</div> 
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre class="code XML"><span class="sc3"><span class="re1">&lt;?xml</span> <span class="re0">version</span>=<span class="st0">"1.0"</span> <span class="re0">encoding</span>=<span class="st0">"UTF-8"</span> <span class="re2">?&gt;</span></span>
<span class="sc3"><span class="re1">&lt;results<span class="re2">&gt;</span></span></span>
  <span class="sc3"><span class="re1">&lt;result<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;Code<span class="re2">&gt;</span></span></span>WEP<span class="sc3"><span class="re1">&lt;/Code<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;Title<span class="re2">&gt;</span></span></span>Wellbore Planner - WBT<span class="sc3"><span class="re1">&lt;/Title<span class="re2">&gt;</span></span></span>
     <span class="sc3"><span class="re1">&lt;Description<span class="re2">&gt;</span></span></span>test description.<span class="sc3"><span class="re1">&lt;/Description<span class="re2">&gt;</span></span></span>
  <span class="sc3"><span class="re1">&lt;/result<span class="re2">&gt;</span></span></span>
<span class="sc3"><span class="re1">&lt;/results<span class="re2">&gt;</span></span></span></pre>
</div>
</div> 
<p>Sample output for JSON returntype. </p>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre>
{"results":{"jsonResponse":[
                   {"Code":"WEP","Title":"Wellbore Planner - WBT","Description":"test"}
             ]  
            }
}</pre></div></div>
<h3 id="classdeails">Get Class Details API</h3> 
<p><strong>Description:</strong> This api is used to retrieve the details of the class.<br>
<strong>End Point URL:</strong> <a href="<?php echo getDomainDoc();?>/apis/ext/classdetails/GetClassDetails"><?php echo getDomainDoc();?>/apis/ext/classdetails/GetClassDetails</a><br>
<strong>Pre requisites:</strong> Access token needs to be populated before access this api.<br>
<strong>Input Parameters:</strong> Following will be the list of Optional/Mandatory parameters to make this api call. </p>
<h3>Optional Params </h3>
<ul><li>none</li></ul>
<h3>Required params  </h3>
<ul><li>class_id - Class Id</li>
<li>session_id - Session Id</li>
<li> returntype - xml (Note: API will return either xml or json object.)</li>
<li>display_columns - Needed columns from the response schema.(It is comma separated values).</li>
<li>CONSUMER_KEY</li>
<li>CONSUMER_SECRET_KEY</li>
<li>ACCESS_OAUTH_TOKEN (It will be available in cookies or sessions after successful authentication.)</li>
<li>ACCESS_TOKEN_SECRET (It will be available in cookies or sessions after successful authentication.)</li>
</ul>
<p><strong>API Response Schema</strong> API will give the following columns as response.</p> 
<ul>
<li>Title</li>
<li>classid</li>
 <li>   courseid</li>
  <li>  Code</li>
 <li>   Description</li>
 <li>   Duration</li>
 <li>  exportcompliance</li>
  <li>  ClsLang</li>
   <li>  deliverytypecode</li>
 <li>    ClsDeliveryType</li>
  <li>   startdatetime</li>
   <li>  enddatetime</li>
   <li>  sessionTitle</li>
   <li>  sessionStartTime</li>
<li>sessionEndTime</li>
<li>LocationId</li>
<li>LocationName</li>
<li> LocationAddr1</li>
<li>LocationAddr2</li>
 <li>LocationCity</li>
<li>LocationState</li>
<li>LocationCountry</li>
<li> LocationCountryName</li>
<li>LocationZip</li>
<li>LocationPhone</li>
<li>Latitude</li>
<li>Longitude</li>
<li>node_id</li>
<li>TimeZone</li>
<li>price</li>
<li>currencytypecode</li>
<li>CurrencyPrefixSymbol</li>
<li>CurrencyPostfixSymbol</li>
<li>MaxCapacity</li>
<li>AvailablibleSeatCount</li>
<li>EnrollmentValidity</li>
<li>registration_end_on</li>
<li>availableSeats</li>
<li>multi_register</li>
</ul>

<p>Example code snippet to make this api call. </p>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre>
 	session_start();
	require_once('expertusone_oauth.php');
	require_once('config.php');
 
	/* retrieve access_token from cookies */
  	$access_oauth_token=$_COOKIE["oauth_token"];
  	$access_token_secret=$_COOKIE["oauth_token_secret"];
  	$userid=$_COOKIE["userid"];
 
 	/*set necessary parameters for this api call. */
  	$_GET["oauth_token"]=$access_oauth_token;
  	$_GET["oauth_token_secret"]=$access_token_secret;
  	$_GET["userid"]=$userid;
  	$_GET["class_id"]="5";
  	$_GET["session_id"]="15";
    	$_GET["display_columns"]="title,code,description";
  	$_GET["limit"]="10";
  	$_GET["returntype"]="xml"; 
 
 
    /* Create connection using client credentials and access tokens */
 
  	$connection = new ExpertusONE_OAuth(CONSUMER_KEY, CONSUMER_SECRET, $access_oauth_token,$access_token_secret);
  	$content = $connection->get("/apis/ext/classdetails/GetClassDetails",$_GET);
        echo $content;           
</pre>
</div>
</div> 
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre class="code XML"><span class="sc3"><span class="re1">&lt;?xml</span> <span class="re0">version</span>=<span class="st0">"1.0"</span> <span class="re0">encoding</span>=<span class="st0">"UTF-8"</span> <span class="re2">?&gt;</span></span>
<span class="sc3"><span class="re1">&lt;results<span class="re2">&gt;</span></span></span>
  <span class="sc3"><span class="re1">&lt;result<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;Code<span class="re2">&gt;</span></span></span>WEP<span class="sc3"><span class="re1">&lt;/Code<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;Title<span class="re2">&gt;</span></span></span>Wellbore Planner - WBT<span class="sc3"><span class="re1">&lt;/Title<span class="re2">&gt;</span></span></span>
     <span class="sc3"><span class="re1">&lt;Description<span class="re2">&gt;</span></span></span>test description.<span class="sc3"><span class="re1">&lt;/Description<span class="re2">&gt;</span></span></span>
  <span class="sc3"><span class="re1">&lt;/result<span class="re2">&gt;</span></span></span>
<span class="sc3"><span class="re1">&lt;/results<span class="re2">&gt;</span></span></span></pre>
</div>
</div> 
<p>Sample output for JSON returntype. </p>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre>
{"results":{"jsonResponse":[
                   {"Code":"WEP","Title":"Wellbore Planner - WBT","Description":"test"}
             ]  
            }
}</pre></div></div> 
<h3 id="registrationapi">Registration API</h3>
<p><strong>Description:</strong> This api is used to enroll the learner for the course/class.<br>
<strong>End Point URL:</strong> <a href="<?php echo getDomainDoc();?>/apis/ext/registration/DoRegistration"><?php echo getDomainDoc();?>/apis/ext/registration/DoRegistration</a><br>
<strong>Pre requisites:</strong> Access token needs to be populated before access this api.<br>
<strong>Input Parameters:</strong> Following will be the list of Optional/Mandatory parameters to make this api call. </p>
<h3>Optional Params </h3>
<ul><li>none</li></ul>
<h3>Required params  </h3>
<ul>
<li>userid</li>
 <li>CourseId</li>
 <li>ClassId</li>
 <li>Active - possible value 1 or 0</li>
 <li>ForceRegister - possible value 1 or 0</li>
 <li>actionkey - possible value 'doregister'</li>
 <li>returntype - xml (Note: API will return either xml or json object.)</li>
 <li>display_columns - Needed columns from the response schema.(It is comma separated values).</li>
 <li>CONSUMER_KEY</li>
 <li>CONSUMER_SECRET_KEY</li>
 <li>ACCESS_OAUTH_TOKEN (It will be available in cookies or sessions after successful authentication.)</li>
 <li>ACCESS_TOKEN_SECRET (It will be available in cookies or sessions after successful authentication.)</li>
</ul>
<p><strong>API Response Schema</strong> API will give the following columns as response.</p> 
<ul><li>ID</li>
<li> DESCRIPTION</li>
</ul>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre>
    session_start();
	require_once('expertusone_oauth.php');
	require_once('config.php');
 
	/* retrieve access_token from cookies */
  	$access_oauth_token=$_COOKIE["oauth_token"];
  	$access_token_secret=$_COOKIE["oauth_token_secret"];
  	$userid=$_COOKIE["userid"];
 
 	/*set necessary parameters for this api call. */
  	$_GET["oauth_token"]=$access_oauth_token;
  	$_GET["oauth_token_secret"]=$access_token_secret;
  	$_GET["userid"]=$userid;
  	$_GET["display_columns"]="id,description";
  	$_GET["returntype"]="xml"; 
  	$_GET["CourseId"]="1"; 
	$_GET["ClassId"]="1"; 
	$_GET["Active"]="1"; 
	$_GET["ForceRegister"]="1"; 
	$_GET["actionkey"]="doregister"; 
 
    /* Create connection using client credentials and access tokens */
 
  	$connection = new ExpertusONE_OAuth(CONSUMER_KEY, CONSUMER_SECRET, $access_oauth_token,$access_token_secret);
  	$content = $connection->get("/apis/ext/register/DoRegistration",$_GET);
        echo $content;        
</pre>
</div>
</div> 
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre class="code XML"><span class="sc3"><span class="re1">&lt;?xml</span> <span class="re0">version</span>=<span class="st0">"1.0"</span> <span class="re0">encoding</span>=<span class="st0">"UTF-8"</span> <span class="re2">?&gt;</span></span>
<span class="sc3"><span class="re1">&lt;results<span class="re2">&gt;</span></span></span>
  <span class="sc3"><span class="re1">&lt;result<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;Id<span class="re2">&gt;</span></span></span>0<span class="sc3"><span class="re1">&lt;/Code<span class="re2">&gt;</span></span></span>
     <span class="sc3"><span class="re1">&lt;Description<span class="re2">&gt;</span></span></span>Registration is successful.<span class="sc3"><span class="re1">&lt;/Description<span class="re2">&gt;</span></span></span>
  <span class="sc3"><span class="re1">&lt;/result<span class="re2">&gt;</span></span></span>
<span class="sc3"><span class="re1">&lt;/results<span class="re2">&gt;</span></span></span></pre>
</div>
</div> 
<p>Sample output for JSON returntype. </p>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre>
{"results":{"jsonResponse":[
                   {"Id":"0","Description":"Registration is successful."}
             ]  
            }
}</pre></div></div> 
<h3 id="enrollmentsapi">Get Enrollments API</h3>
<p><strong>Description:</strong> This api is used to retrieve the learner's enrollments.<br>
<strong>End Point URL:</strong> <a href="<?php echo getDomainDoc();?>/apis/ext/enrollments/ListEnrollments"><?php echo getDomainDoc();?>/apis/ext/enrollments/ListEnrollments</a><br>
<strong>Pre requisites:</strong> Access token needs to be populated before access this api.<br>
<strong>Input Parameters:</strong> Following will be the list of Optional/Mandatory parameters to make this api call. </p>
<h3>Optional Params </h3>
<ul><li>none</li></ul>
<h3>Required params  </h3>
<ul>
<li>userid</li>
<li>actionkey - value for this param is "getenrollments"</li>
<li>limit - No of records should be retrieved.</li>
<li>returntype - xml (Note: API will return either xml or json object.)</li>
<li>display_columns - Needed columns from the response schema.(It is comma separated values).</li>
<li>CONSUMER_KEY</li>
<li>CONSUMER_SECRET_KEY</li>
<li>ACCESS_OAUTH_TOKEN (It will be available in cookies or sessions after successful authentication.)</li>
<li>ACCESS_TOKEN_SECRET (It will be available in cookies or sessions after successful authentication.)</li>
<p><strong>API Response Schema</strong> API will give the following columns as response.</p> 
<ul> <li> ID</li>
<li> USER_ID</li>
<li> MASTER_ENROLLMENT_ID</li>
<li>  USR_NAME</li>
<li>  CLASS_ID</li>
<li> COURSE_ID</li>
<li>  REG_STATUS</li>
 <li>   <li>   REG_STATUS_CODE</li>
<li>  STATUS_CODE</li>
 <li>  REG_DATE</li>
<li> COMP_STATUS</li>
<li>  COMP_DATE</li>
<li> VALID_FROM</li>
<li> VALID_TO</li>
<li> SCORE</li>
<li>  GRADE</li>
<li>  UPDATED_ON</li>
<li>  TITLE</li>
<li>  CLS_TITLE</li>
<li>  CODE</li>
<li>  DESCRIPTION</li>
<li>  DELIVERY_TYPE</li>
<li>  DELIVERY_TYPE_CODE</li>
<li>  BASETYPE</li>
<li>   COURSEID</li>
<li>   CLASSPRICE</li>
 <li>   ORDERDATETIME</li>
<li>   SURVEYSTATUS</li>
</ul>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre>
 	session_start();
	require_once('expertusone_oauth.php');
	require_once('config.php');
 
	/* retrieve access_token from cookies */
  	$access_oauth_token=$_COOKIE["oauth_token"];
  	$access_token_secret=$_COOKIE["oauth_token_secret"];
  	$userid=$_COOKIE["userid"];
 
 	/*set necessary parameters for this api call. */
  	$_GET["oauth_token"]=$access_oauth_token;
  	$_GET["oauth_token_secret"]=$access_token_secret;
  	$_GET["userid"]=$userid;
  	$_GET["display_columns"]="id,class_id,course_id,reg_status,title,code";
  	$_GET["returntype"]="xml";   	
  	$_GET["actionkey"]="getenrollments";   	
 
    /* Create connection using client credentials and access tokens */
 
  	$connection = new ExpertusONE_OAuth(CONSUMER_KEY, CONSUMER_SECRET, $access_oauth_token,$access_token_secret);
  	$content = $connection->get("/apis/ext/enrollments/ListEnrollments",$_GET);
        echo $content;       
</pre>
</div>
</div> 
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre class="code XML"><span class="sc3"><span class="re1">&lt;?xml</span> <span class="re0">version</span>=<span class="st0">"1.0"</span> <span class="re0">encoding</span>=<span class="st0">"UTF-8"</span> <span class="re2">?&gt;</span></span>
<span class="sc3"><span class="re1">&lt;results<span class="re2">&gt;</span></span></span>
  <span class="sc3"><span class="re1">&lt;result<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;id<span class="re2">&gt;</span></span></span>8<span class="sc3"><span class="re1">&lt;/id<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;class_id<span class="re2">&gt;</span></span></span>1<span class="sc3"><span class="re1">&lt;/class_id<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;course_id<span class="re2">&gt;</span></span></span>2<span class="sc3"><span class="re1">&lt;/course_id<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;reg_status<span class="re2">&gt;</span></span></span>Enrolled<span class="sc3"><span class="re1">&lt;/reg_status<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;title<span class="re2">&gt;</span></span></span>Wellbore Planner<span class="sc3"><span class="re1">&lt;/title<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;CODE<span class="re2">&gt;</span></span></span>WEP<span class="sc3"><span class="re1">&lt;/CODE<span class="re2">&gt;</span></span></span>
  <span class="sc3"><span class="re1">&lt;/result<span class="re2">&gt;</span></span></span>
<span class="sc3"><span class="re1">&lt;/results<span class="re2">&gt;</span></span></span></pre>
</div>
</div> 
<p>Sample output for JSON returntype. </p>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre>
{"results":{"jsonResponse":[
                  {"id":"8","class_id":"1","course_id":"2","reg_status":"Enrolled","title":"Wellbore Planner","CODE":"WEP"}
             ]  
            }
}
</pre></div></div> 

<h3 id="cancelenrollment"">Cancel Enrollment API</h3>
<p><strong>Description:</strong> This api is used to cancel the learner's enrollment.<br>
<strong>End Point URL:</strong> <a href="<?php echo getDomainDoc();?>/apis/ext/enrollments/DropEnrollments"><?php echo getDomainDoc();?>/apis/ext/enrollments/DropEnrollments</a><br>
<strong>Pre requisites:</strong> Access token needs to be populated before access this api.<br>
<strong>Input Parameters:</strong> Following will be the list of Optional/Mandatory parameters to make this api call. </p>
<h3>Optional Params </h3>
<ul><li>none</li></ul>
<h3>Required params  </h3>
<ul>
<li>userid -  &laquo; userid &raquo;</li>
<li>enrollid - &laquo; regid of the enrolled class  &raquo;</li>
   <li>actionkey - value for this param is "cancelenrollment"</li>
   <li> returntype - xml (Note: API will return either xml or json object.)</li>
     <li>display_columns - Needed columns from the response schema.(It is comma separated values).</li>
     <li>CONSUMER_KEY</li>
    <li> CONSUMER_SECRET_KEY</li>
     <li>ACCESS_OAUTH_TOKEN (It will be available in cookies or sessions after successful authentication.)</li>
    <li> ACCESS_TOKEN_SECRET (It will be available in cookies or sessions after successful authentication.)</li>
</ul>
<p><strong>Description:</strong> This api is used to retrieve the learner's enrollments.
<ul><li> STATUS</li></ul>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre>
 	session_start();
	require_once('expertusone_oauth.php');
	require_once('config.php');
 
	/* retrieve access_token from cookies */
  	$access_oauth_token=$_COOKIE["oauth_token"];
  	$access_token_secret=$_COOKIE["oauth_token_secret"];
  	$userid=$_COOKIE["userid"];
 
 	/*set necessary parameters for this api call. */
  	$_GET["oauth_token"]=$access_oauth_token;
  	$_GET["oauth_token_secret"]=$access_token_secret;
  	$_GET["userid"]=$userid;
  	$_GET["display_columns"]="status";
  	$_GET["returntype"]="xml";   	
        $_GET["enrollid"]="5";   	
  	$_GET["actionkey"]="cancelenrollment";   	
 
    /* Create connection using client credentials and access tokens */
 
  	$connection = new ExpertusONE_OAuth(CONSUMER_KEY, CONSUMER_SECRET, $access_oauth_token,$access_token_secret);
  	$content = $connection->get("/apis/ext/enrollments/DropEnrollments",$_GET);
        echo $content;
</pre>
</div>
</div> 
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre class="code XML"><span class="sc3"><span class="re1">&lt;?xml</span> <span class="re0">version</span>=<span class="st0">"1.0"</span> <span class="re0">encoding</span>=<span class="st0">"UTF-8"</span> <span class="re2">?&gt;</span></span>
<span class="sc3"><span class="re1">&lt;results<span class="re2">&gt;</span></span></span>
  <span class="sc3"><span class="re1">&lt;result<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;Status<span class="re2">&gt;</span></span></span>Successfully Dropped the course.<span class="sc3"><span class="re1">&lt;/Status<span class="re2">&gt;</span></span></span>
  <span class="sc3"><span class="re1">&lt;/result<span class="re2">&gt;</span></span></span>
<span class="sc3"><span class="re1">&lt;/results<span class="re2">&gt;</span></span></span></pre>
</div>
</div> 
<p>Sample output for JSON returntype. </p>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre>
{"results":{"jsonResponse":[
                  {"status":"Successfully Dropped the course."}]}
}
</pre></div></div> 

<h3 id="lanchmodules">Get Launch Modules API</h3>
<p><strong>Description:</strong> This api is used to retrieve the list of launch modules for the given enrolled id.
<strong>End Point URL: </strong><a href="<?php echo getDomainDoc();?>/apis/ext/content/ListLaunchModules"><?php echo getDomainDoc();?>/apis/ext/content/ListLaunchModules</a>
<strong>Pre requisites:</strong> Access token needs to be populated before access this api.
<strong>Input Parameters:</strong> Following will be the list of Optional/Mandatory parameters to make this api call. </p>
<h3>Optional Params </h3>
<ul><li>none</li></ul>
<h3>Required params  </h3>
<ul>
<li>userid -  &laquo; userid &raquo;</li>
<li>enrollid - &laquo; regid of the enrolled class  &raquo;</li>
<li>actionkey - value for this param is "getlaunchdetails"</li>
   <li> returntype - xml (Note: API will return either xml or json object.)</li>
     <li>display_columns - Needed columns from the response schema.(It is comma separated values).</li>
     <li>CONSUMER_KEY</li>
    <li> CONSUMER_SECRET_KEY</li>
     <li>ACCESS_OAUTH_TOKEN (It will be available in cookies or sessions after successful authentication.)</li>
    <li> ACCESS_TOKEN_SECRET (It will be available in cookies or sessions after successful authentication.)</li>
</ul>
<p><strong>Description:</strong> This api is used to retrieve the learner's enrollments.</p>
<ul>
    <li>TITLE</li>
    <li>LEARNERLAUNCHURL</li>
    <li>CONTENTTYPE</li>
    <li>STATUS</li>
    <li>MAXATTEMPTS</li>
    <li>ID</li></ul>
<p>Example code snippet to make this api call. </p>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre>
  	session_start();
	require_once('expertusone_oauth.php');
	require_once('config.php');
 
	/* retrieve access_token from cookies */
  	$access_oauth_token=$_COOKIE["oauth_token"];
  	$access_token_secret=$_COOKIE["oauth_token_secret"];
  	$userid=$_COOKIE["userid"];
 
 	/*set necessary parameters for this api call. */
  	$_GET["oauth_token"]=$access_oauth_token;
  	$_GET["oauth_token_secret"]=$access_token_secret;
  	$_GET["userid"]=$userid;
  	$_GET["display_columns"]="title,learnerlaunchurl";
  	$_GET["returntype"]="xml";   	
        $_GET["enrollid"]="5";   	
  	$_GET["actionkey"]="getlaunchdetails";   	
 
    /* Create connection using client credentials and access tokens */
 
  	$connection = new ExpertusONE_OAuth(CONSUMER_KEY, CONSUMER_SECRET, $access_oauth_token,$access_token_secret);
  	$content = $connection->get("/apis/ext/content/ListLaunchModules",$_GET);
        echo $content;
</pre>
</div>
</div> 
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre class="code XML"><span class="sc3"><span class="re1">&lt;?xml</span> <span class="re0">version</span>=<span class="st0">"1.0"</span> <span class="re0">encoding</span>=<span class="st0">"UTF-8"</span> <span class="re2">?&gt;</span></span>
<span class="sc3"><span class="re1">&lt;results<span class="re2">&gt;</span></span></span>
  <span class="sc3"><span class="re1">&lt;result<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;id<span class="re2">&gt;</span></span></span>213<span class="sc3"><span class="re1">&lt;/id<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;title<span class="re2">&gt;</span></span></span>Cloud Server Introduction<span class="sc3"><span class="re1">&lt;/title<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;contenttype<span class="re2">&gt;</span></span></span>SCORM<span class="sc3"><span class="re1">&lt;/contenttype<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;learnerlaunchurl<span class="re2">&gt;</span></span></span><?php echo getDomainDoc();?>/content/start.html<span class="sc3"><span class="re1">&lt;/learnerlaunchurl<span class="re2">&gt;</span></span></span>
  <span class="sc3"><span class="re1">&lt;/result<span class="re2">&gt;</span></span></span>
<span class="sc3"><span class="re1">&lt;/results<span class="re2">&gt;</span></span></span></pre>	
</div>
</div> 
<p>Sample output for JSON returntype. </p>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre>
{"results":{"jsonResponse":[
                  {"id":"213","title":"","contenttype":"scorm","learnerlaunchurl":"<?php echo getDomainDoc();?>/content/start.html"}
             ]  
            }
}
</pre></div></div> 

<h3 id="scoreapi">Update Score API</h3>
<p><strong>Description:</strong> This api is used to update the score and status to LMS.
<strong>End Point URL:</strong> <a href="<?php echo getDomainDoc();?>/apis/ext/content/UpdateScore"><?php echo getDomainDoc();?>/apis/ext/content/UpdateScore</a>
<strong>Pre requisites:</strong> Access token needs to be populated before access this api.
<strong>Input Parameters:</strong> Following will be the list of Optional/Mandatory parameters to make this api call. </p>
<h3>Optional Params </h3>
<ul><li>none</li></ul>
<h3>Required params  </h3>
<ul><li>userid -  &laquo; userid &raquo;</li>
<li>enrollid - &laquo; regid of the enrolled class  &raquo;</li>
<li>courseid -</li>
<li>classid -</li>
<li>lessionid -</li>
<li>status - </li>
<li>sessiontime - </li>
<li>grade </li>
<li>location</li>
<li>actionkey - value for this param is "updatescore"</li>
<li>returntype - xml (Note: API will return either xml or json object.)</li>
<li>display_columns - Needed columns from the response schema.(It is comma separated values).</li>
<li>CONSUMER_KEY</li>
<li>CONSUMER_SECRET_KEY</li>
<li>ACCESS_OAUTH_TOKEN (It will be available in cookies or sessions after successful authentication.)</li>
<li>ACCESS_TOKEN_SECRET (It will be available in cookies or sessions after successful authentication.)</li>
</ul>
<p><strong>Description:</strong> This api is used to retrieve the learner's enrollments.</p>
<ul>
    <li>STATUS  &laquo; Completion Status  &raquo;</li><li>ID -  &laquo; ATTEMPT ID  &raquo;</li>
    </ul>
<p>Example code snippet to make this api call. </p>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre>
	 session_start();
	require_once('expertusone_oauth.php');
	require_once('config.php');
	
	/* retrieve access_token from cookies */
	$access_oauth_token=$_COOKIE["oauth_token"];
	$access_token_secret=$_COOKIE["oauth_token_secret"];
	$userid=$_COOKIE["userid"];
	
	/*set necessary parameters for this api call. */
	$_GET["oauth_token"]=$access_oauth_token;
	$_GET["oauth_token_secret"]=$access_token_secret;
	$_GET["userid"]=$userid;
	$_GET["courseid"]="1";
	$_GET["classid"]="2";
	$_GET["lessionid"]="2";
	$_GET["status"]="Completed";
	$_GET["sessiontime"]="09:00";
	$_GET["grade"]="Pass";
	$_GET["location"]="";
	$_GET["display_columns"]="id,status";
	$_GET["returntype"]="xml";   	
	$_GET["actionkey"]="updatescore";   	
	
	/* Create connection using client credentials and access tokens */
	
	$connection = new ExpertusONE_OAuth(CONSUMER_KEY, CONSUMER_SECRET, $access_oauth_token,$access_token_secret);
	$content = $connection->get("/apis/ext/content/UpdateScore",$_GET);
	echo $content;
</pre>
</div>
</div> 
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre class="code XML"><span class="sc3"><span class="re1">&lt;?xml</span> <span class="re0">version</span>=<span class="st0">"1.0"</span> <span class="re0">encoding</span>=<span class="st0">"UTF-8"</span> <span class="re2">?&gt;</span></span>
<span class="sc3"><span class="re1">&lt;results<span class="re2">&gt;</span></span></span>
  <span class="sc3"><span class="re1">&lt;result<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;id<span class="re2">&gt;</span></span></span>213<span class="sc3"><span class="re1">&lt;/id<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;status<span class="re2">&gt;</span></span></span>Completed<span class="sc3"><span class="re1">&lt;/status<span class="re2">&gt;</span></span></span>
  <span class="sc3"><span class="re1">&lt;/result<span class="re2">&gt;</span></span></span>
<span class="sc3"><span class="re1">&lt;/results<span class="re2">&gt;</span></span></span></pre>
</div>
</div> 
<p>Sample output for JSON returntype. </p>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre>
{"results":{"jsonResponse":[
                  {"id":"213","status":"Completed"}
             ]  
            }
}
</pre></div></div> 

<h3 id="learnerannouncement">Learner Announcement API</h3>
<p><strong>Description:</strong> This api is used to get the list of announcements which is available for learner's profile.
<strong>End Point URL:</strong> <a href="<?php echo getDomainDoc();?>/apis/ext/announcement/ListAnnouncements"><?php echo getDomainDoc();?>/apis/ext/announcement/ListAnnouncements</a>
<strong>Pre requisites:</strong> Access token needs to be populated before access this api.
<strong>Input Parameters: </strong>Following will be the list of Optional/Mandatory parameters to make this api call. </p>
<h3>Optional Params </h3>
<ul><li>none</li></ul>
<h3>Required params  </h3>
<ul>
<li>userid -  &laquo; userid &raquo;</li>
<li>actionkey - value for this param is “listannouncements�?</li>
<li>returntype - xml (Note: API will return either xml or json object.)</li>
<li>display_columns - Needed columns from the response schema.(It is comma separated values).</li>
<li>CONSUMER_KEY</li>
<li>CONSUMER_SECRET_KEY</li>
<li>ACCESS_OAUTH_TOKEN (It will be available in cookies or sessions after successful authentication.)</li>
<li>ACCESS_TOKEN_SECRET (It will be available in cookies or sessions after successful authentication.)</li>
<p><strong>API Response Schema</strong> API will give the following columns as response.</p> 

<ul>
    <li>TITLE</li>
   <li> ANNOUNCEMENTID</li>
   <li> SHORTDESC</li>
    <li>DESCRIPTION</li>
    <li>THUMBNAILFILENAME</li>
    <li>SEQNUM</li>

</ul>
<p>Example code snippet to make this api call. </p>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre>
    session_start();
	require_once('expertusone_oauth.php');
	require_once('config.php');
 
	/* retrieve access_token from cookies */
  	$access_oauth_token=$_COOKIE["oauth_token"];
  	$access_token_secret=$_COOKIE["oauth_token_secret"];
  	$userid=$_COOKIE["userid"];
 
 	/*set necessary parameters for this api call. */
  	$_GET["oauth_token"]=$access_oauth_token;
  	$_GET["oauth_token_secret"]=$access_token_secret;
  	$_GET["userid"]=$userid;
  	$_GET["display_columns"]="announcementid,title";
  	$_GET["returntype"]="xml";   	
  	$_GET["actionkey"]="listannouncements";   	
 
    /* Create connection using client credentials and access tokens */
 
  	$connection = new ExpertusONE_OAuth(CONSUMER_KEY, CONSUMER_SECRET, $access_oauth_token,$access_token_secret);
</pre>
</div>
</div> 
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre class="code XML"><span class="sc3"><span class="re1">&lt;?xml</span> <span class="re0">version</span>=<span class="st0">"1.0"</span> <span class="re0">encoding</span>=<span class="st0">"UTF-8"</span> <span class="re2">?&gt;</span></span>
<span class="sc3"><span class="re1">&lt;results<span class="re2">&gt;</span></span></span>
  <span class="sc3"><span class="re1">&lt;result<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;announcementid<span class="re2">&gt;</span></span></span>213<span class="sc3"><span class="re1">&lt;/announcementid<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;title<span class="re2">&gt;</span></span></span>Learner Announcement1<span class="sc3"><span class="re1">&lt;/title<span class="re2">&gt;</span></span></span>
  <span class="sc3"><span class="re1">&lt;/result<span class="re2">&gt;</span></span></span>
<span class="sc3"><span class="re1">&lt;/results<span class="re2">&gt;</span></span></span></pre>
</div>
</div> 
<p>Sample output for JSON returntype. </p>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre>
{"results":{"jsonResponse":[
                  {"announcementid":"213","title":"Learner Announcement1"}
             ]  
            }
}
</pre></div></div> 	



</div>
        </div>
   
      </div>    </div>

   <?php include 'footer.php'; ?> 