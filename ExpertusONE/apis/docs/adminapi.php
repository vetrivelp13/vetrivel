<?php include 'header.php'; ?> 
  <div id="content-outer">
      <div id="breadcrumbs">
<div class="breadcrumb"><a href="/">Home</a> &rarr; <a href="#">Documentation</a><div><a href="#" data-count="none" data-related="" class="share-button">Tweet</a></div></div>      </div>

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
	<li class="toc-level-1"><a href="#otherres">List of APIs for Admin functionality</a>
<ol>
	<li class="toc-level-2"><a href="#coursecreation">Course Creation API   </a></li>
	<li class="toc-level-2"><a href="#courseupdation">Course Updation API </a></li>
	<li class="toc-level-2"><a href="#coursesearch">Course Search API  </a></li>
       <li class="toc-level-2"><a href="#classcreation">Class Creation API    </a></li>
	<li class="toc-level-2"><a href="#classupdation">Class Updation API   </a></li>
	<li class="toc-level-2"><a href="#classsearch">Class Search API   </a></li>
       <li class="toc-level-2"><a href="#contentsearch">Content Search API   </a></li>
	<li class="toc-level-2"><a href="#contentassociate">API for Associate Content with WBT Class   </a></li>
	<li class="toc-level-2"><a href="#editassociate">API for editing the associated Content with WBT Class  </a></li>
	<li class="toc-level-2"><a href="#associatefamily">API for Associate Facility,Location & Room with Class  </a></li>
	
	<li class="toc-level-2"><a href="#familylocation">API for Edit Associated Facility,Location & Room with Class </a></li>
	<li class="toc-level-2"><a href="#removecontent">API for Remove Content,Facility,Location & Room with Class </a></li>
	<li class="toc-level-2"><a href="#usercreateapi">User Creation API  </a></li>
	<li class="toc-level-2"><a href="#userupdation">User Updation API   </a></li>
	
	<li class="toc-level-2"><a href="#usersearchapi">User Search API   </a></li>
	<li class="toc-level-2"><a href="#orgcreation">Organization Creation API   </a></li>
	<li class="toc-level-2"><a href="#orgupdate">Organization Update API   </a></li>
	<li class="toc-level-2"><a href="#orgsearchapi">Organization Search API   </a></li>
	<li class="toc-level-2"><a href="#profilelookup">Profile Lookup Master API  </a></li>
	<li class="toc-level-2"><a href="#addroster">Add Roster API   </a></li>
	<li class="toc-level-2"><a href="#updateroster">Update Roster API   </a></li>
	<li class="toc-level-2"><a href="#listroster">List Roster API  </a></li>
	
</ol>

</li>

</ol>
</div>
</div>

<h2 id="otherres">List Of APIs</h2>
<div class="section">
  <h3 id="coursecreation">Course Creation API </h3>
  <p><strong>Description:</strong> This api is used to create the course in the system.<br>
<strong>End Point URL:</strong> <a href="<?php echo getDomainDoc();?>/apis/ext/catalog/CreateCatalog"><?php echo getDomainDoc();?>/apis/ext/catalog/CreateCatalog</a>  <br>
<strong>Pre requisites:</strong> Access token needs to be populated before access this api. <br>
<strong>Input Parameters:</strong> Following will be the list of Optional/Mandatory parameters to make this api call.
     </p>
  <h3>Optional Params </h3>
  <ul>
  	<li>None</li>
   </ul>
  
  <h3>Required params </h3>
  
<ul><li>userid - &laquo; userid &raquo;</li>
<li>actionkey - value for this param is "createcourse"</li>
<li>Title = Title of the course</li>
<li>Description - Description of the course.</li>
<li>ShortDescription - Short description of the course.</li>
<li>Status - Status of the course. Possible values for this field will be leveraged from profile lookup master.</li>
<li>LanguageId - Language code. Possible values for this field will be leveraged from profile lookup master.</li>
<li>Code - Course code</li>
<li>returntype - xml (Note: API will return either xml or json object.)</li>
<li>display_columns - Needed columns from the response schema.(It is comma separated values).</li>
<li>CONSUMER_KEY</li>
<li>CONSUMER_SECRET_KEY</li>
<li>ACCESS_OAUTH_TOKEN (It will be available in cookies or sessions after successful authentication.)</li>
<li>ACCESS_TOKEN_SECRET (It will be available in cookies or sessions after successful authentication.)</li>

</ul>
 <p><strong>API Response Schema</strong> API will give the following columns as response. </p> 
 <ul><li>id - Course id</li></ul> 
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
	$_GET["Title"]="Test Course";
	$_GET["Description"]="Test Description";
	$_GET["ShortDescription"]="Short Description";
	$_GET["Code"]="TestCode1";
	$_GET["Status"]="lrn_crs_sts_atv" // Get the proper value from profile lookup master
	$_GET["LanguageId"]="crs_sys_lng_eng" // Get the proper value from profile lookup master
	$_GET["display_columns"]="id";
	$_GET["returntype"]="xml";   	
	$_GET["actionkey"]="createcourse";   	

	/* Create connection using client credentials and access tokens */

	$connection = new ExpertusONE_OAuth(CONSUMER_KEY, CONSUMER_SECRET, $access_oauth_token,$access_token_secret);
	$content = $connection->get("/apis/ext/catalog/CreateCatalog",$_GET);
	echo $content;

</div>
</div> 
</div>

<p>Sample output for XML returntype </p>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre class="code XML"><span class="sc3"><span class="re1">&lt;?xml</span> <span class="re0">version</span>=<span class="st0">"1.0"</span> <span class="re0">encoding</span>=<span class="st0">"UTF-8"</span> <span class="re2">?&gt;</span></span>
<span class="sc3"><span class="re1">&lt;results<span class="re2">&gt;</span></span></span>
  <span class="sc3"><span class="re1">&lt;result<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;id<span class="re2">&gt;</span></span></span>213<span class="sc3"><span class="re1">&lt;/id<span class="re2">&gt;</span></span></span>
  <span class="sc3"><span class="re1">&lt;/result<span class="re2">&gt;</span></span></span>
<span class="sc3"><span class="re1">&lt;/results<span class="re2">&gt;</span></span></span></pre>
</div>
</div> 
</div>
<p>Sample output for JSON returntype. </p>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre>
{"results":{"jsonResponse":[
                  {"id":"213"}
             ]  
            }
}
</pre>
</div>
</div> 

<h3 id="courseupdation">Course Updation API</h3>
<p><strong>Description:</strong> This api is used to update the existing course in the system.<br>
<strong>End Point URL:</strong> <a href="<?php echo getDomainDoc();?>/apis/ext/catalog/UpdateCatalog"><?php echo getDomainDoc();?>/apis/ext/catalog/UpdateCatalog</a><br>
<strong>Pre requisites:</strong> Access token needs to be populated before access this api.<br>
<strong>Input Parameters:</strong> Following will be the list of Optional/Mandatory parameters to make this api call.</p> 
<h3>Optional Params </h3>
  <ul>
  	<li>None</li>
  </ul>
 <h3>Required params </h3>
  <ul><li>Id - Valid Course Id</li>
  <li>userid - &laquo; userid &raquo;</li>
<li>actionkey - value for this param is "updatecourse"</li>
<li>Title = Title of the course</li>
<li>Description - Description of the course.</li>
<li>ShortDescription - Short description of the course.</li>
<li>Status - Status of the course. ("lrn_crs_sts_atv" for active, "lrn_crs_sts_dft" for draft,"lrn_crs_sts_itv" for inactive)</li>
<li>LanguageId - ("crs_sys_lng_eng" for english)</li>
<li>Code - Course code</li>
<li>returntype - xml (Note: API will return either xml or json object.)</li>
<li>display_columns - Needed columns from the response schema.(It is comma separated values).</li>
<li>CONSUMER_KEY</li>
<li>CONSUMER_SECRET_KEY</li>
<li>ACCESS_OAUTH_TOKEN (It will be available in cookies or sessions after successful authentication.)</li>
<li>ACCESS_TOKEN_SECRET (It will be available in cookies or sessions after successful authentication.)</li>
</ul>
<p><strong>API Response Schema</strong> API will give the following columns as response. </p> 
 <ul><li>id </li></ul> 
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
	$_GET["Id"]=213; // Valid Course Id

	$_GET["Title"]="Test Course";
	$_GET["Description"]="Updated Test Description";
	$_GET["ShortDescription"]="Short Description";
	$_GET["Code"]="TestCode1";
	$_GET["Status"]="lrn_crs_sts_atv"
	$_GET["LanguageId"]="crs_sys_lng_eng"
	$_GET["display_columns"]="id";
	$_GET["returntype"]="xml";   	
	$_GET["actionkey"]="updatecourse";   	

	/* Create connection using client credentials and access tokens */

	$connection = new ExpertusONE_OAuth(CONSUMER_KEY, CONSUMER_SECRET, $access_oauth_token,$access_token_secret);
	$content = $connection->get("/apis/ext/catalog/UpdateCatalog",$_GET);
	echo $content;
</div>
</div> 
</div>
<p>Sample output for XML returntype </p>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre class="code XML"><span class="sc3"><span class="re1">&lt;?xml</span> <span class="re0">version</span>=<span class="st0">"1.0"</span> <span class="re0">encoding</span>=<span class="st0">"UTF-8"</span> <span class="re2">?&gt;</span></span>
<span class="sc3"><span class="re1">&lt;results<span class="re2">&gt;</span></span></span>
  <span class="sc3"><span class="re1">&lt;result<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;id<span class="re2">&gt;</span></span></span>213<span class="sc3"><span class="re1">&lt;/id<span class="re2">&gt;</span></span></span>
  <span class="sc3"><span class="re1">&lt;/result<span class="re2">&gt;</span></span></span>
<span class="sc3"><span class="re1">&lt;/results<span class="re2">&gt;</span></span></span></pre>
</div>
</div> 
</div>
<p>Sample output for JSON returntype. </p>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre>
{"results":{"jsonResponse":[
                  {"id":"213"}
             ]  
            }
}
</pre>
</div>
</div> 

<h3 id="coursesearch">Course Search API</h3>
<p><strong>Description:</strong> This api is used to retrieve the list of courses which is available in the system.<br>
<strong>End Point URL:</strong> <a href="<?php echo getDomainDoc();?>/apis/ext/catalog/ListCatalogs"><?php echo getDomainDoc();?>/apis/ext/catalog/ListCatalogs</a><br>
<strong>Pre requisites:</strong> Access token needs to be populated before access this api.<br>
<strong>Input Parameters:</strong> Following will be the list of Optional/Mandatory parameters to make this api call. 
</p>

<h3>Optional Params </h3>
  <ul>
    <li>Title - Title of the course</li>
   <li> Code - Code of the course</li>
  <li>  Language - Search by language</li>

</ul>
 <h3>Required params </h3>
  <ul>
  <li>userid - &laquo; userid &raquo;</li>
<li>actionkey - value for this param is "GetCourses"</li>	
<li>returntype - xml (Note: API will return either xml or json object.)</li>
<li>display_columns - Needed columns from the response schema.(It is comma separated values).</li>
<li>CONSUMER_KEY</li>
<li>CONSUMER_SECRET_KEY</li>
<li>ACCESS_OAUTH_TOKEN (It will be available in cookies or sessions after successful authentication.)</li>
<li>ACCESS_TOKEN_SECRET (It will be available in cookies or sessions after successful authentication.)</li>
</ul>
<p><strong>API Response Schema</strong> API will give the following columns as response. </p> 
<ul>
<li>Title</li>
<li>Code</li>
<li>Description</li>
<li>Type</li>
<li>Rating</li>
<li>LastEditingDays</li>
<li>Status</li>
<li>Delivery_type</li>
<li>Language</li>
<li>Stats1</li>
<li>Stats2</li>
<li>Stats3</li>
<li>Stats4</li>
<li>Stats5</li>
<li>Stats6</li>
<li>Stats7</li>
<li>Stats8</li>
<li>Stats9</li>
<li>Stats10</li>
<li>pk - Course ID</li>
<li>parentpk</li>
<li>IsBlended</li>
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
  	$_GET["display_columns"]="Title,Description";
  	$_GET["returntype"]="xml";   	
  	$_GET["actionkey"]="GetCourses";   	
 
    /* Create connection using client credentials and access tokens */
 
  	$connection = new ExpertusONE_OAuth(CONSUMER_KEY, CONSUMER_SECRET, $access_oauth_token,$access_token_secret);
  	$content = $connection->get("/apis/ext/catalog/ListCatalogs",$_GET);
    echo $content;
</div>
</div> 
<p>Sample output for XML returntype </p>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre class="code XML"><span class="sc3"><span class="re1">&lt;?xml</span> <span class="re0">version</span>=<span class="st0">"1.0"</span> <span class="re0">encoding</span>=<span class="st0">"UTF-8"</span> <span class="re2">?&gt;</span></span>
<span class="sc3"><span class="re1">&lt;results<span class="re2">&gt;</span></span></span>
  <span class="sc3"><span class="re1">&lt;result<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;Title<span class="re2">&gt;</span></span></span>Test Course1<span class="sc3"><span class="re1">&lt;/Title<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;Description<span class="re2">&gt;</span></span></span>Test Description1<span class="sc3"><span class="re1">&lt;/Description<span class="re2">&gt;</span></span></span>
  <span class="sc3"><span class="re1">&lt;/result<span class="re2">&gt;</span></span></span>
  <span class="sc3"><span class="re1">&lt;result<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;Title<span class="re2">&gt;</span></span></span>Test Course2<span class="sc3"><span class="re1">&lt;/Title<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;Description<span class="re2">&gt;</span></span></span>Test Description2<span class="sc3"><span class="re1">&lt;/Description<span class="re2">&gt;</span></span></span>
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
                  {"Title":"Test Course1","Description":"Test Description1"},
                  {"Title":"Test Course2","Description":"Test Description2"}
             ]  
            }
}
</pre>
</div>
</div> 

<h3 id="classcreation">Class Creation API</h3>
<p><strong>Description:</strong> This api is used to create the class for the given course id.<br>
<strong> Point URL:</strong><a href="<?php echo getDomainDoc();?>/apis/ext/catalog/CreateCatalog"><?php echo getDomainDoc();?>/apis/ext/catalog/CreateCatalog</a><br>
<strong>Pre requisites:</strong> Access token needs to be populated before access this api.<br>
<strong>Input Parameters:</strong> Following will be the list of Optional/Mandatory parameters to make this api call. 
</p>
<h3>Optional Params </h3>
<ul>
	<li>None</li>
</ul>
 <h3>Required params </h3>
  <ul>
	  <li>userid - &laquo; userid &raquo;</li>
	  <li>actionkey - value for this param is "createclass"</li>
	  <li>CourseId - Course Id</li> 
	  <li>Title = Title of the course</li>	
	   <li>Description - Description of the course.</li>
	  <li>ShortDescription - Short description of the course.</li>
	  <li>Status - Status of the course. ("lrn_crs_sts_atv" for active, "lrn_crs_sts_dft" for draft,"lrn_crs_sts_itv" for inactive)</li>
	  <li>LanguageId - ("crs_sys_lng_eng" for english)</li>
	<li>Code - Class code</li>	  
	<li>DeliveryTypeId - Possible values for this field will be leveraged from profile lookup master.</li> 	
	<li>IsBlended - 0 or 1 </li>
	<li>Price - Price for the class.(Default is "")</li>
	<li>Currency - Currency of the class.(Default is "")</li>
	<li>TrainingUnits - Training Unit value.(Default is "")</li>
	<li>CancellationFee - Cancellation Fee.(Default is "")</li>
	<li>LateCancellationFee - Late Cancellation Fee.(Default is "")</li>
	<li>NoShowFee - No Show Fee.(Default is "")</li>
	<li>InactiveReason - Reason for inactiving the course.(Default is "")</li>
	<li>ExportCompliance - Y or N (Default "N")</li>
	<li>RegistrationEnd - Registration End.(Default is "")</li>
	<li>RegDeadLineDate - Deadline for registration.(Default is "")</li>
	<li>Duration - Duration of the class.(Default is "")</li>

<li>returntype - xml (Note: API will return either xml or json object.)</li>
<li>display_columns - Needed columns from the response schema.(It is comma separated values).</li>
<li>CONSUMER_KEY</li>
<li>CONSUMER_SECRET_KEY</li>
<li>ACCESS_OAUTH_TOKEN (It will be available in cookies or sessions after successful authentication.)</li>
<li>ACCESS_TOKEN_SECRET (It will be available in cookies or sessions after successful authentication.)</li>
</ul>
<p><strong>API Response Schema</strong> API will give the following columns as response. </p> 
<ul>
<li>id</li></ul>

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
  	$_GET["CourseId"]="1";
    $_GET["Title"]="Test Class";
    $_GET["Description"]="Test Description";
    $_GET["ShortDescription"]="Short Description";
    $_GET["Code"]="TestCode1";
    $_GET["Status"]="lrn_crs_sts_atv"
    $_GET["LanguageId"]="crs_sys_lng_eng"
    $_GET["DeliveryTypeId"]="lrn_cls_dty_wbt"
  	$_GET["display_columns"]="idi";
  	$_GET["returntype"]="xml";   	
  	$_GET["actionkey"]="createclass";   	
 
    /* Create connection using client credentials and access tokens */
 
  	$connection = new ExpertusONE_OAuth(CONSUMER_KEY, CONSUMER_SECRET, $access_oauth_token,$access_token_secret);
  	$content = $connection->get("/apis/ext/catalog/CreateCatalog",$_GET);
    echo $content;
</div>
</div> 
<p>Sample output for XML returntype </p>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre class="code XML"><span class="sc3"><span class="re1">&lt;?xml</span> <span class="re0">version</span>=<span class="st0">"1.0"</span> <span class="re0">encoding</span>=<span class="st0">"UTF-8"</span> <span class="re2">?&gt;</span></span>
<span class="sc3"><span class="re1">&lt;results<span class="re2">&gt;</span></span></span>
  <span class="sc3"><span class="re1">&lt;result<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;id<span class="re2">&gt;</span></span></span>213<span class="sc3"><span class="re1">&lt;/id<span class="re2">&gt;</span></span></span>
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
                  {"id":"213"}
             ]  
            }
}	
</pre>
</div>
</div> 

<h3 id="classupdation">Class Updation API</h3>
<p><strong>Description:</strong> This api is used to update the existing class in the system.<br>
<strong>End Point URL:</strong> <a href="<?php echo getDomainDoc();?>/apis/ext/catalog/UpdateCatalog"><?php echo getDomainDoc();?>/apis/ext/catalog/UpdateCatalog</a><br>
<strong>Pre requisites:</strong> Access token needs to be populated before access this api.<br>
<strong>Input Parameters:</strong> Following will be the list of Optional/Mandatory parameters to make this api call. 
</p>
<h3>Optional Params </h3>
<ul>
<li>None</li>
</ul>
<h3>Required params </h3>
  <ul>
  <li>CourseId - &laquo; Valid Course Id &raquo;</li>
  <li>ClassId - &laquo; Valid Class Id &raquo;</li>
   <li>userid - &laquo; userid Id &raquo;</li>
  <li>actionkey - value for this param is "updateclass"</li> 
  <li>Title = Title of the course</li>	
   <li>Description - Description of the course.</li>
  <li>ShortDescription - Short description of the course.</li>
  <li>Status - Status of the course. ("lrn_crs_sts_atv" for active, "lrn_crs_sts_dft" for draft,"lrn_crs_sts_itv" for inactive)</li>
  <li>LanguageId - ("crs_sys_lng_eng" for english)</li>
	<li>Code - Class code</li>	  
	<li>DeliveryTypeId - Possible values for this field will be leveraged from profile lookup master.</li> 	
	<li>IsBlended - 0 or 1 </li>
	<li>Price - Price for the class.(Default is "")</li>
	<li>Currency - Currency of the class.(Default is "")</li>
	<li>TrainingUnits - Training Unit value.(Default is "")</li>
	<li>CancellationFee - Cancellation Fee.(Default is "")</li>
	<li>LateCancellationFee - Late Cancellation Fee.(Default is "")</li>
	<li>NoShowFee - No Show Fee.(Default is "")</li>
	<li>InactiveReason - Reason for inactiving the course.(Default is "")</li>
	<li>ExportCompliance - Y or N (Default "N")</li>
	<li>RegistrationEnd - Registration End.(Default is "")</li>
	<li>RegDeadLineDate - Deadline for registration.(Default is "")</li>
	<li>Duration - Duration of the class.(Default is "")</li>


<li>returntype - xml (Note: API will return either xml or json object.)</li>
<li>display_columns - Needed columns from the response schema.(It is comma separated values).</li>
<li>CONSUMER_KEY</li>
<li>CONSUMER_SECRET_KEY</li>
<li>ACCESS_OAUTH_TOKEN (It will be available in cookies or sessions after successful authentication.)</li>
<li>ACCESS_TOKEN_SECRET (It will be available in cookies or sessions after successful authentication.)</li>
</ul>
<p><strong>API Response Schema</strong> API will give the following columns as response. </p> 
<ul>
<li>id</li></ul>
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
	$_GET["CourseId"]="1"; //Valid Course ID
	$_GET["ClassId"]=6 // Valid class ID
	$_GET["Title"]="Test Class";
	$_GET["Description"]="Test Description";
	$_GET["ShortDescription"]="Short Description";
	$_GET["Code"]="TestCode1";
	$_GET["Status"]="lrn_crs_sts_atv"
	$_GET["LanguageId"]="crs_sys_lng_eng"
	$_GET["DeliveryTypeId"]="lrn_cls_dty_wbt"
	$_GET["display_columns"]="id";
	$_GET["returntype"]="xml";   	
	$_GET["actionkey"]="updateclass";   	

	/* Create connection using client credentials and access tokens */

	$connection = new ExpertusONE_OAuth(CONSUMER_KEY, CONSUMER_SECRET, $access_oauth_token,$access_token_secret);
	$content = $connection->get("/apis/ext/catalog/UpdateCatalog",$_GET);
</div>
</div> 
<p>Sample output for XML returntype </p>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre class="code XML"><span class="sc3"><span class="re1">&lt;?xml</span> <span class="re0">version</span>=<span class="st0">"1.0"</span> <span class="re0">encoding</span>=<span class="st0">"UTF-8"</span> <span class="re2">?&gt;</span></span>
<span class="sc3"><span class="re1">&lt;results<span class="re2">&gt;</span></span></span>
  <span class="sc3"><span class="re1">&lt;result<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;id<span class="re2">&gt;</span></span></span>213<span class="sc3"><span class="re1">&lt;/id<span class="re2">&gt;</span></span></span>
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
                  {"id":"213"}
             ]  
            }
}
</pre>
</div>
</div> 

<h3 id="classsearch">Class Search API</h3>
<p><strong>Description:</strong> This api is used to search the list of classes which is available in the system.<br>
<strong>End Point URL:</strong> <a href="<?php echo getDomainDoc();?>/apis/ext/catalog/ListCatalogs"><?php echo getDomainDoc();?>/apis/ext/catalog/ListCatalogs</a><br>
<strong>Pre requisites:</strong> Access token needs to be populated before access this api.<br>
<strong>Input Parameters:</strong> Following will be the list of Optional/Mandatory parameters to make this api call. </p>
<h3>Optional Params</h3>
<ul>   <li>Title - Title of the class</li>
   <li> Code - Class Code</li>
   <li> Language - Search by language</li>
   <li> DeliveryType - Search by delivery type</li>
   </ul>
   
 <h3>Required params </h3>
  <ul>
  <li>userid - &laquo; userid Id &raquo;</li>
  <li>actionkey - value for this param is "GetClasses"</li>  	
<li>returntype - xml (Note: API will return either xml or json object.)</li>
<li>display_columns - Needed columns from the response schema.(It is comma separated values).</li>
<li>CONSUMER_KEY</li>
<li>CONSUMER_SECRET_KEY</li>
<li>ACCESS_OAUTH_TOKEN (It will be available in cookies or sessions after successful authentication.)</li>
<li>ACCESS_TOKEN_SECRET (It will be available in cookies or sessions after successful authentication.)</li>
</ul>  
 <p><strong>API Response Schema</strong> API will give the following columns as response. </p> 
<ul>
   <li> Id</li>
   <li> Title</li>
   <li> Code</li>
   <li> Description</li>
  <li>  Type</li>
  <li>  Rating</li>
 <li>   LastEditingDays</li>
<li>    Status</li>
 <li>   Delivery_type</li>
 <li>   Language</li>
 <li>   Stats1</li>
<li>    Stats2</li>
<li>    Stats3</li>
 <li>   Stats4</li>
<li>    Stats5</li>
<li>    Stats6</li>
 <li>   Stats7</li>
 <li>   Stats8</li>
 <li>   Stats9</li>
<li>    Stats10</li>
 <li>   pk - Class ID</li>
  <li>  parentpk - Course ID</li>
  <li>  IsBlended</li>
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
	$_GET["CourseId"]="1"; //Valid Course ID
	$_GET["ClassId"]=6 // Valid class ID
	$_GET["Title"]="Test Class";
	$_GET["Description"]="Test Description";
	$_GET["ShortDescription"]="Short Description";
	$_GET["Code"]="TestCode1";
	$_GET["Status"]="lrn_crs_sts_atv"
	$_GET["LanguageId"]="crs_sys_lng_eng"
	$_GET["DeliveryTypeId"]="lrn_cls_dty_wbt"
	$_GET["display_columns"]="id";
	$_GET["returntype"]="xml";   	
	$_GET["actionkey"]="updateclass";   	

	/* Create connection using client credentials and access tokens */

	$connection = new ExpertusONE_OAuth(CONSUMER_KEY, CONSUMER_SECRET, $access_oauth_token,$access_token_secret);
	$content = $connection->get("/apis/ext/catalog/UpdateCatalog",$_GET);
</div>
</div> 
<p>Sample output for XML returntype </p>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre class="code XML"><span class="sc3"><span class="re1">&lt;?xml</span> <span class="re0">version</span>=<span class="st0">"1.0"</span> <span class="re0">encoding</span>=<span class="st0">"UTF-8"</span> <span class="re2">?&gt;</span></span>
<span class="sc3"><span class="re1">&lt;results<span class="re2">&gt;</span></span></span>
  <span class="sc3"><span class="re1">&lt;result<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;Title<span class="re2">&gt;</span></span></span>Test Class1<span class="sc3"><span class="re1">&lt;/Title<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;Description<span class="re2">&gt;</span></span></span>Test Description1<span class="sc3"><span class="re1">&lt;/Description<span class="re2">&gt;</span></span></span>
  <span class="sc3"><span class="re1">&lt;/result<span class="re2">&gt;</span></span></span>
  <span class="sc3"><span class="re1">&lt;result<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;Title<span class="re2">&gt;</span></span></span>Test Class2<span class="sc3"><span class="re1">&lt;/Title<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;Description<span class="re2">&gt;</span></span></span>Test Description2<span class="sc3"><span class="re1">&lt;/Description<span class="re2">&gt;</span></span></span>
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
                  {"Title":"Test Class1","Description":"Test Description1"},
                  {"Title":"Test Class2","Description":"Test Description2"}
             ]  
            }
}
</pre>
</div>
</div> 



<h3 id="contentsearch">Content Search API</h3>
<p><strong>Description:</strong> This api is used to search the list of classes which is available in the system.<br>
<strong>End Point URL:</strong> <a href="<?php echo getDomainDoc();?>/apis/ext/resources/ListResources"><?php echo getDomainDoc();?>/apis/ext/resources/ListResources</a><br>
<strong>Pre requisites:</strong> Access token needs to be populated before access this api.<br>
<strong>Input Parameters:</strong> Following will be the list of Optional/Mandatory parameters to make this api call. </p>
<h3>Optional Params</h3>
<ul><li>none</li></ul>
   
 <h3>Required params </h3>
  <ul>
  <li>actionkey - value for this param is "getavailablecontents"</li>  	
<li>cm_code - Code of the content, provide "" as default</li>
<li>status - Retrieve the content by status. Possible values for this field will be leveraged from profile lookup master. provide "" as default.</li>
<li>sort - Retrieve the content by sort order. Provide "" as default
<li>language_code - Retrieve the content by language. Possible values for this field will be leveraged from profile lookup master. provide "" as default.</li>
<li>type - Retrieve the content by type "".Possible values for this field will be leveraged from profile lookup master. provide "" as default.</li>
<li>returntype - xml (Note: API will return either xml or json object.)</li>
<li>display_columns - Needed columns from the response schema.(It is comma separated values).</li>
<li>CONSUMER_KEY</li>
<li>CONSUMER_SECRET_KEY</li>
<li>ACCESS_OAUTH_TOKEN (It will be available in cookies or sessions after successful authentication.)</li>
<li>ACCESS_TOKEN_SECRET (It will be available in cookies or sessions after successful authentication.)</li>
</ul>  
 <p><strong>API Response Schema</strong> API will give the following columns as response. </p> 
<ul>
    <li>id</li>
    <li>code</li>
    <li>type</li>
   <li> description</li>
    <li>lang_code</li>
   <li> status</li>
   <li> content_type_name</li>
   <li> language_name</li>
   <li> content_status_name</li>
   <li> title</li>
   <li> lastediteddays</li>
   <li> contentversionid</li>
  <li>  contentversion</li>

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
	$_GET["sort"]="";
	$_GET["type"]="";
	$_GET["status"]="";
	$_GET["language_code"]="";
	$_GET["cm_code"]=""

	$_GET["display_columns"]="title,id";
	$_GET["returntype"]="xml";   	
	$_GET["actionkey"]="getavailablecontents";   	

	/* Create connection using client credentials and access tokens */

	$connection = new ExpertusONE_OAuth(CONSUMER_KEY, CONSUMER_SECRET, $access_oauth_token,$access_token_secret);
	$content = $connection->get("/apis/ext/resources/ListResources",$_GET);
	echo $content;
</div>
</div> 
<p>Sample output for XML returntype </p>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre class="code XML"><span class="sc3"><span class="re1">&lt;?xml</span> <span class="re0">version</span>=<span class="st0">"1.0"</span> <span class="re0">encoding</span>=<span class="st0">"UTF-8"</span> <span class="re2">?&gt;</span></span>
<span class="sc3"><span class="re1">&lt;results<span class="re2">&gt;</span></span></span>
  <span class="sc3"><span class="re1">&lt;result<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;id<span class="re2">&gt;</span></span></span>1<span class="sc3"><span class="re1">&lt;/id<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;title<span class="re2">&gt;</span></span></span>SCORM Content test<span class="sc3"><span class="re1">&lt;/title<span class="re2">&gt;</span></span></span>
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
                  {"id":"1","title":"SCORM Content test"}
             ]  
            }
}
</pre>
</div>
</div> 

<h3 id="contentassociate">API for Associate Content with WBT Class</h3>
<p><strong>Description:</strong> This api is used to map the content to the wbt class entity.<br>
<strong>End Point URL:</strong> <a href="<?php echo getDomainDoc();?>/apis/ext/catalog/UpdateCatalog"><?php echo getDomainDoc();?>/apis/ext/catalog/UpdateCatalog</a><br>
<strong>Pre requisites:</strong> Access token needs to be populated before access this api.<br>
<strong>Input Parameters:</strong> Following will be the list of Optional/Mandatory parameters to make this api call. </p>
     <h3> Optional Params</h3>
       <ul> <li>MasteryScore - Mastery Score</li>
        <li>MaxAttempts- Maximum attempt for the given content.</li>
       <li> SequenceNo</li>
        </ul>
 <h3>Required params </h3>
  <ul>
  <li>userid - &laquo; userid Id &raquo;</li>
  <li>actionkey - value for this param is "associatecontent"</li>     
 <li>CourseId - Course ID</li>
 <li>ClassId - Class ID</li>
 <li>ContentId - Content ID</li>
 <li>DTType - Base delivery type, provide base delivery type label (Example: ILT,VC, WBT)</li>
 <li>ContentCode - Content code</li>
 <li>Title - Title of the content.</li>
 <li>returntype - xml (Note: API will return either xml or json object.)</li>
 <li>display_columns - Needed columns from the response schema.(It is comma separated values).</li>
 <li>CONSUMER_KEY</li>
 <li>CONSUMER_SECRET_KEY</li>
 <li>ACCESS_OAUTH_TOKEN (It will be available in cookies or sessions after successful authentication.)</li>
 <li>ACCESS_TOKEN_SECRET (It will be available in cookies or sessions after successful authentication.)</li>
   </ul>
 <p><strong>API Response Schema</strong> API will give the following columns as response. </p> 
<ul>
<li>id - created session id</li>  
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
	$_GET["CourseId"]="1";
	$_GET["ContentId"]="4";
	$_GET["ContentCode"]="SCORM";
	$_GET["Title"]="test";
	$_GET["ClassId"]="2";
	$_GET["DTType"]="WBT";
	$_GET["MasteryScore"]="80";
	$_GET["MaxAttempts"]="100";



	$_GET["display_columns"]="id";
	$_GET["returntype"]="xml";   	
	$_GET["actionkey"]="associatecontent";   	

	/* Create connection using client credentials and access tokens */

	$connection = new ExpertusONE_OAuth(CONSUMER_KEY, CONSUMER_SECRET, $access_oauth_token,$access_token_secret);
	$content = $connection->get("/apis/ext/catalog/UpdateCatalog",$_GET);
	echo $content;a
</div>
</div> 
<p>Sample output for XML returntype </p>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre class="code XML"><span class="sc3"><span class="re1">&lt;?xml</span> <span class="re0">version</span>=<span class="st0">"1.0"</span> <span class="re0">encoding</span>=<span class="st0">"UTF-8"</span> <span class="re2">?&gt;</span></span>
<span class="sc3"><span class="re1">&lt;results<span class="re2">&gt;</span></span></span>
  <span class="sc3"><span class="re1">&lt;result<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;id<span class="re2">&gt;</span></span></span>17<span class="sc3"><span class="re1">&lt;/id<span class="re2">&gt;</span></span></span>
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
                  {"id":"17"}
             ]  
            }
}
</pre>
</div>
</div> 

<h3 id="editassociate">API for editing the associated Content with WBT Class</h3>
<p><strong>Description:</strong> This api is used to edit the mapped data in the wbt class entity.<br>
<strong>End Point URL:</strong> <a href="<?php echo getDomainDoc();?>/apis/ext/catalog/UpdateCatalog"><?php echo getDomainDoc();?>/apis/ext/catalog/UpdateCatalog</a><br>
<strong>Pre requisites:</strong> Access token needs to be populated before access this api.<br>
<strong>Input Parameters:</strong> Following will be the list of Optional/Mandatory parameters to make this api call. </p>
<h3>Optional Params </h3>
<ul>
<li>MasteryScore - Mastery Score</li>
<li>MaxAttempts- Maximum attempt for the given content.</li>
<li>SequenceNo</li>
</ul>
<h3>Required params </h3>
  <ul>
   <li>userid - &laquo; userid Id &raquo;</li>
  <li>actionkey - value for this param is "edit_associated_resources"</li> 
     <li>     CourseId - Course ID</li>
     <li>   SessionId - Session ID ( Id which created during the wbt class and content mapping)</li>
     <li>   ClassId - Class ID</li>
     <li>   ContentId - Content ID</li>
    <li>    DTType - Base delivery type, provide base delivery type label (Example: ILT,VC, WBT)</li>
     <li>   ContentCode - Content code</li>
     <li>   Title - Title of the content.</li>
     <li>   returntype - xml (Note: API will return either xml or json object.)</li>
     <li>   display_columns - Needed columns from the response schema.(It is comma separated values).</li>
    <li>    CONSUMER_KEY</li>
    <li>    CONSUMER_SECRET_KEY</li>
     <li>   ACCESS_OAUTH_TOKEN (It will be available in cookies or sessions after successful authentication.)</li>
     <li>   ACCESS_TOKEN_SECRET (It will be available in cookies or sessions after successful authentication.)</li>
</ul>  
 <p><strong>API Response Schema</strong> API will give the following columns as response. </p> 
<ul>
<li>id - created session id</li>  
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
	$_GET["CourseId"]="1";
	$_GET["SessionId"]="197";
	$_GET["ContentId"]="4";
	$_GET["ContentCode"]="SCORM";
	$_GET["Title"]="test";
	$_GET["ClassId"]="2";
	$_GET["DTType"]="WBT";
	$_GET["MasteryScore"]="90";
	$_GET["MaxAttempts"]="100";

	$_GET["display_columns"]="id";
	$_GET["returntype"]="xml";   	
	$_GET["actionkey"]="edit_associated_resources";   	

	/* Create connection using client credentials and access tokens */

	$connection = new ExpertusONE_OAuth(CONSUMER_KEY, CONSUMER_SECRET, $access_oauth_token,$access_token_secret);
	$content = $connection->get("/apis/ext/catalog/UpdateCatalog",$_GET);
	echo $content; 
</pre>	
</div>
</div> 
<p>Sample output for XML returntype </p>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre class="code XML"><span class="sc3"><span class="re1">&lt;?xml</span> <span class="re0">version</span>=<span class="st0">"1.0"</span> <span class="re0">encoding</span>=<span class="st0">"UTF-8"</span> <span class="re2">?&gt;</span></span>
<span class="sc3"><span class="re1">&lt;results<span class="re2">&gt;</span></span></span>
  <span class="sc3"><span class="re1">&lt;result<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;id<span class="re2">&gt;</span></span></span>17<span class="sc3"><span class="re1">&lt;/id<span class="re2">&gt;</span></span></span>
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
                  {"id":"17"}
             ]  
            }
}
</pre>
</div>
</div> 	
<h3 id="associatefamily">API for Associate Facility,Location & Room with Class</h3> 
 <p><strong>Description:</strong> This api is used to map the facility, location and room with the class entity.<br>
<strong>End Point URL:</strong> <a href="<?php echo getDomainDoc();?>/apis/ext/catalog/UpdateCatalog"><?php echo getDomainDoc();?>/apis/ext/catalog/UpdateCatalog</a><br>
<strong>Pre requisites:</strong> Access token needs to be populated before access this api.<br>
<strong>Input Parameters:</strong> Following will be the list of Optional/Mandatory parameters to make this api call. </p> 
<h3>Optional Params </h3>
<ul>
    <li>ClassRoomId- Id of the classroom</li>
   <li> InstructorId - Id of the instructor</li>
   <li> CapacityMin- Minimum capacity of the class.</li>
 <li>   CapacityMax - Maximum capacity of the class.</li>
  <li>  TotalWaitlist - No of waitlist seats</li>
  <li>  DocumentURL -</li>
  <li>  ContentURL -</li>
  <li>  PresenterURL -</li>
  <li>  AttendeeURL -</li>
  <li>  Type -  Type of the class. Possible values for this field will be leveraged from profile lookup master. (Example: "lrn_cls_vct_lmt" for Live Meeting)</li>
 <li>   TimeZone - Timezone code. Possible values for this field will be leveraged from profile lookup master. (Example: "lrn_cls_vct_lmt" for Live Meeting. It is mandatory for VC delivery type) </li>
  <li>  Title -</li>
</ul>
<h3>Required params </h3>
  <ul>
   <li>userid - &laquo; userid Id &raquo;</li>
 <li> actionkey - value for this param is "associateresources"</li>
 <li>CourseId - Course ID</li>
 <li>ClassId - Class ID</li>
 <li>DTType - Base delivery type, provide base delivery type label (Example: ILT,VC, WBT)</li>
 <li>StartDate - Format MM/DD/YYYY</li>
 <li>StartTime - HH:MM </li>
 <li>EndTime - HH:MM </li>
 <li>LocationId - Id of the location.</li>
 <li>FacilityId - Facility of the location.</li>
 <li>returntype - xml (Note: API will return either xml or json object.)</li>
 <li>display_columns - Needed columns from the response schema.(It is comma separated values).</li>
 <li>CONSUMER_KEY</li>
 <li>CONSUMER_SECRET_KEY</li>
 <li>ACCESS_OAUTH_TOKEN (It will be available in cookies or sessions after successful authentication.)</li>
 <li>ACCESS_TOKEN_SECRET (It will be available in cookies or sessions after successful authentication.)</li>
</ul>  

<p><strong>API Response Schema</strong> API will give the following columns as response. </p> 
<ul>
<li>id - created session id</li>  
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
	$_GET["CourseId"]="1";
	$_GET["ClassId"]="2";
	$_GET["DTType"]="ILT";
	$_GET["StartDate"]="09/28/2011";
	$_GET["StartTime"]="00:30";
	$_GET["EndTime"]="02:30";
	$_GET["LocationId"]="1"
	$_GET["FacilityId"]="1"


	$_GET["display_columns"]="id";
	$_GET["returntype"]="xml";   	
	$_GET["actionkey"]="associateresources";   	

	/* Create connection using client credentials and access tokens */

	$connection = new ExpertusONE_OAuth(CONSUMER_KEY, CONSUMER_SECRET, $access_oauth_token,$access_token_secret);
	$content = $connection->get("/apis/ext/catalog/UpdateCatalog",$_GET);
	echo $content;        
</pre>	
</div>
</div> 
<p>Sample output for XML returntype </p>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre class="code XML"><span class="sc3"><span class="re1">&lt;?xml</span> <span class="re0">version</span>=<span class="st0">"1.0"</span> <span class="re0">encoding</span>=<span class="st0">"UTF-8"</span> <span class="re2">?&gt;</span></span>
<span class="sc3"><span class="re1">&lt;results<span class="re2">&gt;</span></span></span>
  <span class="sc3"><span class="re1">&lt;result<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;id<span class="re2">&gt;</span></span></span>19<span class="sc3"><span class="re1">&lt;/id<span class="re2">&gt;</span></span></span>
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
                  {"id":"19"}
             ]  
            }
}
</pre>
</div>
</div> 	

<h3 id="familylocation">API for Edit Associated Facility,Location & Room with Class</h3> 
 <p><strong>Description:</strong> This api is used to edit the existing mapping in the facility, location and room with the class entity.<br>
<strong>End Point URL:</strong> <a href="<?php echo getDomainDoc();?>/apis/ext/catalog/UpdateCatalog"><?php echo getDomainDoc();?>/apis/ext/catalog/UpdateCatalog</a><br>
<strong>Pre requisites:</strong> Access token needs to be populated before access this api.<br>
<strong>Input Parameters:</strong> Following will be the list of Optional/Mandatory parameters to make this api call. </p> 
<h3>Optional Params </h3>
<ul>
    <li>ClassRoomId- Id of the classroom</li>
   <li> InstructorId - Id of the instructor</li>
   <li> CapacityMin- Minimum capacity of the class.</li>
 <li>   CapacityMax - Maximum capacity of the class.</li>
  <li>  TotalWaitlist - No of waitlist seats</li>
  <li>  DocumentURL -</li>
  <li>  ContentURL -</li>
  <li>  PresenterURL -</li>
  <li>  AttendeeURL -</li>
  <li>  Type -  Type of the class. Possible values for this field will be leveraged from profile lookup master. (Example: "lrn_cls_vct_lmt" for Live Meeting)</li>
  <li>   TimeZone - Timezone code. Possible values for this field will be leveraged from profile lookup master. (Example: "lrn_cls_vct_lmt" for Live Meeting. It is mandatory for VC delivery type) </li>
  <li>  Title -</li>
</ul>
<h3>Required params </h3>
  <ul>
   <li>userid - &laquo; userid Id &raquo;</li>
 <li> actionkey - value for this param is "edit_associated_resources"</li>
 <li>CourseId - Course ID</li>
 <li>ClassId - Class ID</li>
 <li>ContentId - Resource mapping ID</li>
 <li>DTType - Base delivery type, provide base delivery type label (Example: ILT,VC, WBT)</li>
 <li>StartDate - Format MM/DD/YYYY</li>
 <li>StartTime - HH:MM </li>
 <li>EndTime - HH:MM </li>
 <li>LocationId - Id of the location.</li>
 <li>FacilityId - Facility of the location.</li>
 <li>returntype - xml (Note: API will return either xml or json object.)</li>
 <li>display_columns - Needed columns from the response schema.(It is comma separated values).</li>
 <li>CONSUMER_KEY</li>
 <li>CONSUMER_SECRET_KEY</li>
 <li>ACCESS_OAUTH_TOKEN (It will be available in cookies or sessions after successful authentication.)</li>
 <li>ACCESS_TOKEN_SECRET (It will be available in cookies or sessions after successful authentication.)</li>
</ul>  

<p><strong>API Response Schema</strong> API will give the following columns as response. </p> 
<ul>
<li>id - created session id</li>  
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
	$_GET["CourseId"]="1";
	$_GET["ClassId"]="2";
	$_GET["SessionId"]="2";
	$_GET["DTType"]="ILT";
	$_GET["StartDate"]="09/28/2011";
	$_GET["StartTime"]="00:30";
	$_GET["EndTime"]="02:30";
	$_GET["LocationId"]="1"
	$_GET["FacilityId"]="1"


	$_GET["display_columns"]="id";
	$_GET["returntype"]="xml";   	
	$_GET["actionkey"]="edit_associated_resources";   	

	/* Create connection using client credentials and access tokens */

	$connection = new ExpertusONE_OAuth(CONSUMER_KEY, CONSUMER_SECRET, $access_oauth_token,$access_token_secret);
	$content = $connection->get("/apis/ext/catalog/UpdateCatalog",$_GET);
	echo $content;        
</pre>	
</div>
</div> 
<p>Sample output for XML returntype </p>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre class="code XML"><span class="sc3"><span class="re1">&lt;?xml</span> <span class="re0">version</span>=<span class="st0">"1.0"</span> <span class="re0">encoding</span>=<span class="st0">"UTF-8"</span> <span class="re2">?&gt;</span></span>
<span class="sc3"><span class="re1">&lt;results<span class="re2">&gt;</span></span></span>
  <span class="sc3"><span class="re1">&lt;result<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;id<span class="re2">&gt;</span></span></span>19<span class="sc3"><span class="re1">&lt;/id<span class="re2">&gt;</span></span></span>
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
                  {"id":"2"}
             ]  
            }
}
</pre>
</div>
</div> 	


<h3 id="removecontent">API for Remove Content,Facility,Location & Room with Class</h3>
<p><strong>Description:</strong> This api is used to remove the content,facility, location, room with the class entity.<br>
<strong>End Point URL:</strong> <a href="<?php echo getDomainDoc();?>/apis/ext/catalog/UpdateCatalog"><?php echo getDomainDoc();?>/apis/ext/catalog/UpdateCatalog</a><br>
<strong>Pre requisites:</strong> Access token needs to be populated before access this api.<br>
<strong>Input Parameters:</strong> Following will be the list of Optional/Mandatory parameters to make this api call. </p>
<h3>Optional Params </h3>
<ul>
    <li>None - </li>
</ul>
<h3>Required params </h3>
  <ul>
   <li>userid - &laquo; userid Id &raquo;</li>
 <li> actionkey - value for this param is "unassociateresources"</li>   
   <li>SessionId - Id of the record which needs to be modify.</li>
  <li>returntype - xml (Note: API will return either xml or json object.)</li>
  <li>display_columns - Needed columns from the response schema.(It is comma separated values).</li>
  <li>CONSUMER_KEY</li>
  <li>CONSUMER_SECRET_KEY</li>
  <li>ACCESS_OAUTH_TOKEN (It will be available in cookies or sessions after successful authentication.)</li>
  <li>ACCESS_TOKEN_SECRET (It will be available in cookies or sessions after successful authentication.)</li>
   </ul>
<p><strong>API Response Schema</strong> API will give the following columns as response. </p> 
<ul>
<li>status- it gives "success" or "failure" as status</li>  
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
	$_GET["SessionId"]="1";


	$_GET["display_columns"]="status";
	$_GET["returntype"]="xml";   	
	$_GET["actionkey"]="unassociateresources";   	

	/* Create connection using client credentials and access tokens */

	$connection = new ExpertusONE_OAuth(CONSUMER_KEY, CONSUMER_SECRET, $access_oauth_token,$access_token_secret);
	$content = $connection->get("/apis/ext/catalog/UpdateCatalog",$_GET);
	echo $content;
</pre>	
</div>
</div> 
<p>Sample output for XML returntype </p>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre class="code XML"><span class="sc3"><span class="re1">&lt;?xml</span> <span class="re0">version</span>=<span class="st0">"1.0"</span> <span class="re0">encoding</span>=<span class="st0">"UTF-8"</span> <span class="re2">?&gt;</span></span>
<span class="sc3"><span class="re1">&lt;results<span class="re2">&gt;</span></span></span>
  <span class="sc3"><span class="re1">&lt;result<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;status<span class="re2">&gt;</span></span></span>success<span class="sc3"><span class="re1">&lt;/status<span class="re2">&gt;</span></span></span>
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
                  {"status":"success"}
             ]  
            }
}
</pre>
</div>
</div> 	
<h3 id="usercreateapi">User Creation API</h3> 
 <p><strong>Description:</strong> This api is used to create the user in LMS system. <br>
<strong>End Point URL:</strong> <a href="<?php echo getDomainDoc();?>/apis/ext/user/CreateUser"><?php echo getDomainDoc();?>/apis/ext/user/CreateUser</a><br>
<strong>Pre requisites:</strong> Access token needs to be populated before access this api.<br>
<strong>Input Parameters:</strong> Following will be the list of Optional/Mandatory parameters to make this api call. </p>
<h3>Optional Params </h3>
<ul>
       <li>MiddleName - Learner middle name</li>
   <li> Notification - possible values would be "sendnotification" or "". (If value is "sendnotification" then system will send the notification for Reset Password.)</li>
   <li> PhoneNo - Phone no</li>
  <li>  Address1 -</li>
  <li>  Address2 -</li>
  <li>  City -</li>
  <li>  State -</li>
  <li>  Country -</li>
  <li>  PostalCode -</li>
 <li>   UserType - Type of the user. Possible values for this field will be leveraged from profile lookup master. (Example: "cre_usr_ptp_emp" for Employee)</li>
  <li>  EmploymentType - Employeement type. Possible values for this field will be leveraged from profile lookup master. (Example: "cre_usr_etp_ftm" for full time employee )</li>
  <li>  JobTitle - Job title. Possible values for this field will be leveraged from profile lookup master. (Example: "cre_usr_jtl_dtm" for Data Manager )</li>
  <li>  HireDate - Hire date ( Format : 2011-10-28 14:00)</li>
  <li>  TerminationDate - Termination Date (Format : 2011-10-28 14:00)</li>
  <li>  IsRehire - Possible values "Y" or "N".</li>
  <li>  IsInstructor - Possible values "Y" or "N".</li>
  <li>  ManagerId - Userid of the manager.</li>
  <li>  ManagerName - Manager name</li>
  <li>  OrganizationId - Organization Id</li>
  <li>  OrganizationName - Name of the organization.</li>
  <li>  DepartmentId - Department Id. Possible values for this field will be leveraged from profile lookup master.</li>
  <li>  DepartmentName - Department name. Possible values for this field will be leveraged from profile lookup master.</li>
  <li>  TimeZone -</li>
  <li>  Avatar -</li>
   <li> LocationId - Id of the location.</li>
  <li>  LocationName - Name of the location.</li>
 

</ul>
<h3>Required params </h3>
  <ul>
  <li>FirstName -</li>
<li>LastName -</li>
<li>UserName -</li>
<li>Password -</li>
<li>Email -</li>
<li>Status - cre_usr_sts_atv</li>
<li>actionkey - createuser</li>
<li>returntype - xml (Note: API will return either xml or json object.)</li>
<li>display_columns - Needed columns from the response schema.(It is comma separated values).</li>
<li>CONSUMER_KEY</li>
<li>CONSUMER_SECRET_KEY</li>
<li>ACCESS_OAUTH_TOKEN (It will be available in cookies or sessions after successful authentication.)</li>
<li>ACCESS_TOKEN_SECRET (It will be available in cookies or sessions after successful authentication.)</li>  
   </ul>
<p><strong>API Response Schema</strong> API will give the following columns as response. </p> 
<ul>
<li>Id - UserId </li>  
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
  	$_GET["UserName"]="testuser100";
  	$_GET["FirstName"]="test";
  	$_GET["Email"]="testuser@expertus.com";
  	$_GET["Password"]="welcome";
  	$_GET["Status"]="cre_usr_sts_atv";
    	$_GET["display_columns"]="Id";
  	$_GET["returntype"]="xml";   	
  	$_GET["actionkey"]="createuser";   	
 
    /* Create connection using client credentials and access tokens */
 
  	$connection = new ExpertusONE_OAuth(CONSUMER_KEY, CONSUMER_SECRET, $access_oauth_token,$access_token_secret);
  	$content = $connection->get("/apis/ext/user/CreateUser",$_GET);
        echo $content;
</pre>	
</div>
</div> 
<p>Sample output for XML returntype </p>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre class="code XML"><span class="sc3"><span class="re1">&lt;?xml</span> <span class="re0">version</span>=<span class="st0">"1.0"</span> <span class="re0">encoding</span>=<span class="st0">"UTF-8"</span> <span class="re2">?&gt;</span></span>
<span class="sc3"><span class="re1">&lt;results<span class="re2">&gt;</span></span></span>
  <span class="sc3"><span class="re1">&lt;result<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;Id<span class="re2">&gt;</span></span></span>6<span class="sc3"><span class="re1">&lt;/Id<span class="re2">&gt;</span></span></span>
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
                  {"Id":"6"}
             ]  
            }
}
</pre>
</div>
</div> 
<h3 id="userupdation">User Updation API</h3> 
 <p><strong>Description:</strong> This api is used to create the user in LMS system. <br>
<strong>End Point URL:</strong> <a href="<?php echo getDomainDoc();?>/apis/ext/user/UpdateUser"><?php echo getDomainDoc();?>/apis/ext/user/UpdateUser</a><br>
<strong>Pre requisites:</strong> Access token needs to be populated before access this api.<br>
<strong>Input Parameters:</strong> Following will be the list of Optional/Mandatory parameters to make this api call. </p>
<h3>Optional Params </h3>
<ul>
       <li>MiddleName - Learner middle name</li>
   <li> Notification - possible values would be "sendnotification" or "". (If value is "sendnotification" then system will send the notification for Reset Password.)</li>
   <li> PhoneNo - Phone no</li>
  <li>  Address1 -</li>
  <li>  Address2 -</li>
  <li>  City -</li>
  <li>  State -</li>
  <li>  Country -</li>
  <li>  PostalCode -</li>
 <li>   UserType - Type of the user. Possible values for this field will be leveraged from profile lookup master. (Example: "cre_usr_ptp_emp" for Employee)</li>
  <li>  EmploymentType - Employeement type. Possible values for this field will be leveraged from profile lookup master. (Example: "cre_usr_etp_ftm" for full time employee )</li>
  <li>  JobTitle - Job title. Possible values for this field will be leveraged from profile lookup master. (Example: "cre_usr_jtl_dtm" for Data Manager )</li>
  <li>  HireDate - Hire date ( Format : 2011-10-28 14:00)</li>
  <li>  TerminationDate - Termination Date (Format : 2011-10-28 14:00)</li>
  <li>  IsRehire - Possible values "Y" or "N".</li>
  <li>  IsInstructor - Possible values "Y" or "N".</li>
  <li>  ManagerId - Userid of the manager.</li>
  <li>  ManagerName - Manager name</li>
  <li>  OrganizationId - Organization Id</li>
  <li>  OrganizationName - Name of the organization.</li>
  <li>  DepartmentId - Department Id. Possible values for this field will be leveraged from profile lookup master.</li>
  <li>  DepartmentName - Department name. Possible values for this field will be leveraged from profile lookup master.</li>
  <li>  TimeZone -</li>
  <li>  Avatar -</li>
   <li> LocationId - Id of the location.</li>
  <li>  LocationName - Name of the location.</li>
 <li>   AssignedRoles - Application level roles, (i.e Catalog Admin) Possible values "3|4" (Role ids needs to be pipe delimited for multiple ids)</li>

</ul>
<h3>Required params </h3>
  <ul>
  <li>Id - Id of the user which needs to be updated.</li>
  <li>FirstName -</li>
<li>LastName -</li>
<li>Password -</li>
<li>Email -</li>
<li>Status - cre_usr_sts_atv</li>
<li>actionkey - createuser</li>
<li>returntype - xml (Note: API will return either xml or json object.)</li>
<li>display_columns - Needed columns from the response schema.(It is comma separated values).</li>
<li>CONSUMER_KEY</li>
<li>CONSUMER_SECRET_KEY</li>
<li>ACCESS_OAUTH_TOKEN (It will be available in cookies or sessions after successful authentication.)</li>
<li>ACCESS_TOKEN_SECRET (It will be available in cookies or sessions after successful authentication.)</li>  
   </ul>
<p><strong>API Response Schema</strong> API will give the following columns as response. </p> 
<ul>
<li>Id - UserId </li>  
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
	$_GET["UserName"]="testuser100";
	$_GET["Id"]="6";
	$_GET["FirstName"]="test";
	$_GET["Email"]="testuser@expertus.com";
	$_GET["Password"]="welcome";
	$_GET["Status"]="cre_usr_sts_atv";
	$_GET["display_columns"]="Id";
	$_GET["returntype"]="xml";   	
	$_GET["actionkey"]="updateuser";   	

	/* Create connection using client credentials and access tokens */

	$connection = new ExpertusONE_OAuth(CONSUMER_KEY, CONSUMER_SECRET, $access_oauth_token,$access_token_secret);
	$content = $connection->get("/apis/ext/user/UpdateUser",$_GET);
	echo $content;
</pre>	
</div>
</div> 
<p>Sample output for XML returntype </p>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre class="code XML"><span class="sc3"><span class="re1">&lt;?xml</span> <span class="re0">version</span>=<span class="st0">"1.0"</span> <span class="re0">encoding</span>=<span class="st0">"UTF-8"</span> <span class="re2">?&gt;</span></span>
<span class="sc3"><span class="re1">&lt;results<span class="re2">&gt;</span></span></span>
  <span class="sc3"><span class="re1">&lt;result<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;Id<span class="re2">&gt;</span></span></span>6<span class="sc3"><span class="re1">&lt;/Id<span class="re2">&gt;</span></span></span>
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
                  {"Id":"6"}
             ]  
            }
}
</pre>
</div>
</div>  
<h3 id="usersearchapi">User Search API</h3>
<p><strong>Description:</strong> This api is used to retrieve the list of users in the system. <br>
<strong>End Point URL:</strong> <a href="<?php echo getDomainDoc();?>/apis/ext/user/ListUsers"><?php echo getDomainDoc();?>/apis/ext/user/ListUsers</a><br>
<strong>Pre requisites:</strong> Access token needs to be populated before access this api.<br>
<strong>Input Parameters:</strong> Following will be the list of Optional/Mandatory parameters to make this api call. </p>
<h3>Optional Params </h3>
<ul>
      <li>  Name -</li>
   <li> UserName -</li>
  <li>  status -</li>
      <li>emptype -</li>
     <li> rehired -</li>
     <li> instructor -</li>
    <li>  startdate -</li>
     <li> enddate -</li>
</ul>
<h3>Required params </h3>
  <ul>
<li>userid -</li>
<li>actionkey - listusers</li>
<li>returntype - xml (Note: API will return either xml or json object.)</li>
<li>display_columns - Needed columns from the response schema.(It is comma separated values).</li>
<li>CONSUMER_KEY</li>
<li>CONSUMER_SECRET_KEY</li>
<li>ACCESS_OAUTH_TOKEN (It will be available in cookies or sessions after successful authentication.)</li>
<li>ACCESS_TOKEN_SECRET (It will be available in cookies or sessions after successful authentication.)</li>
</ul>
<p><strong>API Response Schema</strong> API will give the following columns as response. </p> 
<ul>
<li>Id - UserId </li>  
 <li>   Name - User fullname</li>
   <li>   UserName</li>
   <li>   UserType</li>
    <li>  EmploymentType</li>
   <li>   JobTitle</li>
   <li>   Email</li>
   <li>   ManagerId</li>
   <li>   ManagerName</li>
   <li>   Status</li>
   <li>   Stats1</li>
    <li>  Stats2</li>
   <li>   Stats3</li>
    <li>  Stats4</li>
   <li>   Stats5</li>
    <li>  Stats6</li>
    <li>  Stats7</li>
    <li>  Stats8</li>
    <li>  Stats9</li>
    <li>  Stats10</li>
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
  	$_GET["UserName"]="testuser";
    	$_GET["display_columns"]="Id,UserName";
  	$_GET["returntype"]="xml";   	
  	$_GET["actionkey"]="listusers";   	
 
    /* Create connection using client credentials and access tokens */
 
  	$connection = new ExpertusONE_OAuth(CONSUMER_KEY, CONSUMER_SECRET, $access_oauth_token,$access_token_secret);
  	$content = $connection->get("/apis/ext/user/ListUsers",$_GET);
    echo $content;
</pre>	
</div>
</div> 
<p>Sample output for XML returntype </p>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre class="code XML"><span class="sc3"><span class="re1">&lt;?xml</span> <span class="re0">version</span>=<span class="st0">"1.0"</span> <span class="re0">encoding</span>=<span class="st0">"UTF-8"</span> <span class="re2">?&gt;</span></span>
<span class="sc3"><span class="re1">&lt;results<span class="re2">&gt;</span></span></span>
  <span class="sc3"><span class="re1">&lt;result<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;Id<span class="re2">&gt;</span></span></span>6<span class="sc3"><span class="re1">&lt;/Id<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;UserName<span class="re2">&gt;</span></span></span>testuser<span class="sc3"><span class="re1">&lt;/UserName<span class="re2">&gt;</span></span></span>
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
                  {"Id":"6","UserName":"testuser"}
             ]  
            }
}
</pre>
</div>
</div>  
<h3 id="orgcreation">Organization Creation API</h3>
<p><strong>Description:</strong> This api is used to retrieve the list of users in the system. <br>
<strong>End Point URL:</strong> <a href="<?php echo getDomainDoc();?>/apis/ext/org/CreateOrg"><?php echo getDomainDoc();?>/apis/ext/org/CreateOrg</a><br>
<strong>Pre requisites:</strong> Access token needs to be populated before access this api.<br>
<strong>Input Parameters:</strong> Following will be the list of Optional/Mandatory parameters to make this api call. </p>
<h3>Optional Params </h3>
<ul>
    <li>Number - Organization number</li>
    <li>ParentOrgId -</li>
    <li>ParentOrgName -</li>
    <li>Address1 -</li>
    <li>Address2 -</li>
   <li> City -</li>
   <li> State -</li>
   <li> WebSite -</li>
   <li> PostalCode -</li>
   <li> CostCenter -</li>
   <li> ContactId -</li>
   <li> ContactName -</li>
   <li> SalesRepId -</li>
   <li> SalesRepName -</li>
   <li> PhoneNo -</li>
   <li> FaxNo -</li>
   <li> Email -</li>
 </ul>
 
<h3>Required params </h3>
  <ul>
 <li>userid - &laquo; userId &raquo;</li>
 <li> actionkey - "createorg"</li> 
  <li> Name - Name of the organization</li>
     <li> Type - Type of the org. Possible values for this field will be leveraged from profile lookup master. (Example: "cre_org_typ_int" for internal organization. )</li> 
    <li> Status - Possible values for this field will be leveraged from profile lookup master. (Example: "cre_org_sts_act" for internal organization. )  
    <li> Country - Country of the Organization </li>
    <li> returntype - xml   (Note: API will return either xml or json object.)</li>
    <li> display_columns - Needed columns from the response schema.(It is comma separated values).</li>
     <li> CONSUMER_KEY</li>
     <li> CONSUMER_SECRET_KEY</li>
     <li> ACCESS_OAUTH_TOKEN (It will be available in cookies or sessions after successful authentication.)</li>
     <li> ACCESS_TOKEN_SECRET (It will be available in cookies or sessions after successful authentication.)</li>
  </ul>   
<p><strong>API Response Schema</strong> API will give the following columns as response. </p> 
<ul>
<li>Id </li>
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
	$_GET["Name"]="Expertus Inc";
	$_GET["Type"]="cre_org_typ_int";
	$_GET["Status"]="cre_org_sts_act";
	$_GET["Country"]="India";
	$_GET["display_columns"]="Id";
	$_GET["returntype"]="xml";   	
	$_GET["actionkey"]="createorg";   	

	/* Create connection using client credentials and access tokens */

	$connection = new ExpertusONE_OAuth(CONSUMER_KEY, CONSUMER_SECRET, $access_oauth_token,$access_token_secret);
	$content = $connection->get("/apis/ext/org/CreateOrg",$_GET);
	echo $content;
</pre>	
</div>
</div> 
<p>Sample output for XML returntype </p>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre class="code XML"><span class="sc3"><span class="re1">&lt;?xml</span> <span class="re0">version</span>=<span class="st0">"1.0"</span> <span class="re0">encoding</span>=<span class="st0">"UTF-8"</span> <span class="re2">?&gt;</span></span>
<span class="sc3"><span class="re1">&lt;results<span class="re2">&gt;</span></span></span>
  <span class="sc3"><span class="re1">&lt;result<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;Id<span class="re2">&gt;</span></span></span>6<span class="sc3"><span class="re1">&lt;/Id<span class="re2">&gt;</span></span></span>
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
                  {"Id":"6"}
             ]  
            }
}
</pre>
</div>
</div>        
 <h3 id="orgupdate"">Organization Update API</h3>
<p><strong>Description:</strong> This api is used to retrieve the list of users in the system. <br>
<strong>End Point URL:</strong> <a href="<?php echo getDomainDoc();?>/apis/ext/org/CreateOrg"><?php echo getDomainDoc();?>/apis/ext/org/CreateOrg</a><br>
<strong>Pre requisites:</strong> Access token needs to be populated before access this api.<br>
<strong>Input Parameters:</strong> Following will be the list of Optional/Mandatory parameters to make this api call. </p>
<h3>Optional Params </h3>
<ul><li>None</li></ul>
 <h3>Required params </h3>
  <ul>
 <li>userid - &laquo; userId &raquo;</li>
 <li>Id - Id of the org needs to be updated.</li>
 <li> actionkey - "createorg"</li> 
  <li> Name - Name of the organization</li>
     <li> Type - Type of the org. Possible values for this field will be leveraged from profile lookup master. (Example: "cre_org_typ_int" for internal organization. )</li> 
    <li> Status - Possible values for this field will be leveraged from profile lookup master. (Example: "cre_org_sts_act" for internal organization. )  
    <li> Country - Country of the Organization </li>
    <li> returntype - xml   (Note: API will return either xml or json object.)</li>
    <li> display_columns - Needed columns from the response schema.(It is comma separated values).</li>
     <li> CONSUMER_KEY</li>
     <li> CONSUMER_SECRET_KEY</li>
     <li> ACCESS_OAUTH_TOKEN (It will be available in cookies or sessions after successful authentication.)</li>
     <li> ACCESS_TOKEN_SECRET (It will be available in cookies or sessions after successful authentication.)</li>
  </ul>   
<p><strong>API Response Schema</strong> API will give the following columns as response. </p> 
<ul>
<li>Id </li>
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
	$_GET["Name"]="Expertus Inc";
	$_GET["Type"]="cre_org_typ_int";
	$_GET["Status"]="cre_org_sts_act";
	$_GET["Country"]="India";
	$_GET["display_columns"]="Id";
	$_GET["returntype"]="xml";   	
	$_GET["actionkey"]="updateorg";   	

	/* Create connection using client credentials and access tokens */

	$connection = new ExpertusONE_OAuth(CONSUMER_KEY, CONSUMER_SECRET, $access_oauth_token,$access_token_secret);
	$content = $connection->get("/apis/ext/org/UpdateOrg",$_GET);
	echo $content;
</pre>	
</div>
</div> 
<p>Sample output for XML returntype </p>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre class="code XML"><span class="sc3"><span class="re1">&lt;?xml</span> <span class="re0">version</span>=<span class="st0">"1.0"</span> <span class="re0">encoding</span>=<span class="st0">"UTF-8"</span> <span class="re2">?&gt;</span></span>
<span class="sc3"><span class="re1">&lt;results<span class="re2">&gt;</span></span></span>
  <span class="sc3"><span class="re1">&lt;result<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;Id<span class="re2">&gt;</span></span></span>6<span class="sc3"><span class="re1">&lt;/Id<span class="re2">&gt;</span></span></span>
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
                  {"Id":"6"}
             ]  
          }
}
</pre>
</div>
</div>   
 <h3 id="orgsearchapi">Organization Search API</h3>  
 <p><strong>Description:</strong> This api is used to retrieve the list of users in the system. <br>
<strong>End Point URL:</strong> <a href="<?php echo getDomainDoc();?>/apis/ext/org/ListOrgs"><?php echo getDomainDoc();?>/apis/ext/org/ListOrgs</a><br>
<strong>Pre requisites:</strong> Access token needs to be populated before access this api.<br>
<strong>Input Parameters:</strong> Following will be the list of Optional/Mandatory parameters to make this api call. </p>
<h3>Optional Params </h3>
<ul>
      <li> Name- Organization name</li>
   <li> Number -</li>
    <li>Id -</li>
   
 </ul>
 
<h3>Required params </h3>
  <ul>
<li>userid - Userid</li>
<li>actionkey - listorgs</li>
<li>returntype - xml (Note: API will return either xml or json object.)</li>
<li>display_columns - Needed columns from the response schema.(It is comma separated values).</li>
<li>CONSUMER_KEY</li>
<li>CONSUMER_SECRET_KEY</li>
<li>ACCESS_OAUTH_TOKEN (It will be available in cookies or sessions after successful authentication.)</li>
<li>ACCESS_TOKEN_SECRET (It will be available in cookies or sessions after successful authentication.)</li>
  </ul>   
<p><strong>API Response Schema</strong> API will give the following columns as response. </p> 
<ul>
<li>Id </li>
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
  	$_GET["Name"]="Expertus Inc";
      	$_GET["display_columns"]="Id,Name,Number";
  	$_GET["returntype"]="xml";   	
  	$_GET["actionkey"]="listorgs";   	
 
    /* Create connection using client credentials and access tokens */
 
  	$connection = new ExpertusONE_OAuth(CONSUMER_KEY, CONSUMER_SECRET, $access_oauth_token,$access_token_secret);
  	$content = $connection->get("/apis/ext/org/ListOrgs",$_GET);
        echo $content;
</pre>	
</div>
</div> 
<p>Sample output for XML returntype </p>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre class="code XML"><span class="sc3"><span class="re1">&lt;?xml</span> <span class="re0">version</span>=<span class="st0">"1.0"</span> <span class="re0">encoding</span>=<span class="st0">"UTF-8"</span> <span class="re2">?&gt;</span></span>
<span class="sc3"><span class="re1">&lt;results<span class="re2">&gt;</span></span></span>
  <span class="sc3"><span class="re1">&lt;result<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;Id<span class="re2">&gt;</span></span></span>6<span class="sc3"><span class="re1">&lt;/Id<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;Name<span class="re2">&gt;</span></span></span>Expertus Inc<span class="sc3"><span class="re1">&lt;/Name<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;Number<span class="re2">&gt;</span></span></span>1000<span class="sc3"><span class="re1">&lt;/Number<span class="re2">&gt;</span></span></span>
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
                  {"Id":"6","Name":"Expertus Inc","Number":"100"}
             ]  
          }
}
</pre>
</div>
</div>    
<h3 id="profilelookup">Profile Lookup Master API</h3>
 <p><strong>Description:</strong> This api is used to retrieve the list of users in the system. <br>
<strong>End Point URL:</strong> <a href="<?php echo getDomainDoc();?>/apis/ext/profile_lookup/ListProfileValues"><?php echo getDomainDoc();?>/apis/ext/profile_lookup/ListProfileValues</a><br>
<strong>Pre requisites:</strong> Access token needs to be populated before access this api.<br>
<strong>Input Parameters:</strong> Following will be the list of Optional/Mandatory parameters to make this api call. </p>
<h3>Optional Params </h3>
<ul>
      <li> Name- Organization name</li>
   <li> Number -</li>
    <li>Id -</li>
   
 </ul>
 
<h3>Required params </h3>
  <ul>
<li>userid - Userid</li>
<li>id - Id of the entity</li>
<li>level - (0 for root level data, 001 next level, 101,201,301,401 and etc…)</li>
<li>root - Possible value for this field is "R"</li>
<li>actionkey - listvalues</li>
<li>returntype - xml (Note: API will return either xml or json object.)</li>
<li>display_columns - Needed columns from the response schema.(It is comma separated values).</li>
<li>CONSUMER_KEY</li>
<li>CONSUMER_SECRET_KEY</li>
<li>ACCESS_OAUTH_TOKEN (It will be available in cookies or sessions after successful authentication.)</li>
<li>ACCESS_TOKEN_SECRET (It will be available in cookies or sessions after successful authentication.)</li>
</ul>   
<p><strong>API Response Schema</strong> API will give the following columns as response. </p> 
<ul>
<li>Id </li>
<li>name</li>
<li>attr3</li>
<li>attr4</li>

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
  	$_GET["id"]="0";
  	$_GET["level"]="0";
  	$_GET["root"]="R";
      	$_GET["display_columns"]="id,name";
  	$_GET["returntype"]="xml";   	
  	$_GET["actionkey"]="listvalues";   	
 
    /* Create connection using client credentials and access tokens */
 
  	$connection = new ExpertusONE_OAuth(CONSUMER_KEY, CONSUMER_SECRET, $access_oauth_token,$access_token_secret);
  	$content = $connection->get("/apis/ext/profile_lookup/ListProfileValues",$_GET);
     echo $content;
</pre>	
</div>
</div> 
<p>Sample output for XML returntype </p>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre class="code XML"><span class="sc3"><span class="re1">&lt;?xml</span> <span class="re0">version</span>=<span class="st0">"1.0"</span> <span class="re0">encoding</span>=<span class="st0">"UTF-8"</span> <span class="re2">?&gt;</span></span>
<span class="sc3"><span class="re1">&lt;results<span class="re2">&gt;</span></span></span>
  <span class="sc3"><span class="re1">&lt;result<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;Id<span class="re2">&gt;</span></span></span>60<span class="sc3"><span class="re1">&lt;/Id<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;Name<span class="re2">&gt;</span></span></span>Client Profile Data<span class="sc3"><span class="re1">&lt;/Name<span class="re2">&gt;</span></span></span>
   <span class="sc3"><span class="re1">&lt;/result<span class="re2">&gt;</span></span></span>
  <span class="sc3"><span class="re1">&lt;result<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;Id<span class="re2">&gt;</span></span></span>53<span class="sc3"><span class="re1">&lt;/Id<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;Name<span class="re2">&gt;</span></span></span>Collabration<span class="sc3"><span class="re1">&lt;/Name<span class="re2">&gt;</span></span></span>
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
                  {"id":"60","name":"Client Profile Data"},
                  {"id":"53","name":"Collabration"}
             ]  
          }
}
</pre>
</div>
</div> 
<h3 id="addroster">Add Roster API</h3>
 <p><strong>Description:</strong> This api is used to retrieve the list of users in the system. <br>
<strong>End Point URL:</strong> <a href="<?php echo getDomainDoc();?>/apis/ext/roster/AddRoster"><?php echo getDomainDoc();?>/apis/ext/roster/AddRoster</a><br>
<strong>Pre requisites:</strong> Access token needs to be populated before access this api.<br>
<strong>Input Parameters:</strong> Following will be the list of Optional/Mandatory parameters to make this api call. </p>
<h3>Optional Params </h3>
<ul><li> None</li></ul> 
<h3>Required params </h3>
  <ul>
<li>userid - Userid</li>
<li>ClassId - Id of the Class.</li>
<li>CourseId - Id of the Course.</li>
<li>LnrUserStr - List of user ids with comma separated values to register.</li>
<li>Active - Y</li>
<li>actionkey - addroster</li>
<li>returntype - xml (Note: API will return either xml or json object.)</li>
<li>display_columns - Needed columns from the response schema.(It is comma separated values).</li>
<li>CONSUMER_KEY</li>
<li>CONSUMER_SECRET_KEY</li>
<li>ACCESS_OAUTH_TOKEN (It will be available in cookies or sessions after successful authentication.)</li>
<li>ACCESS_TOKEN_SECRET (It will be available in cookies or sessions after successful authentication.)</li>
</ul>   
<p><strong>API Response Schema</strong> API will give the following columns as response. </p> 
<ul>
   <li> UserName</li>
    <li>Id - Enrollment Id</li>
    <li>Status</li>
    <li>Description</li>
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
	$_GET["LnrUserStr"]="34";
	$_GET["ClassId"]="7";
	$_GET["CourseId"]="4";
	$_GET["Active"]="Y";
	$_GET["display_columns"]="UserName,Status,Description";
	$_GET["returntype"]="xml";   	
	$_GET["actionkey"]="addroster";   	

	/* Create connection using client credentials and access tokens */

	$connection = new ExpertusONE_OAuth(CONSUMER_KEY, CONSUMER_SECRET, $access_oauth_token,$access_token_secret);
	$content = $connectio
</pre>	
</div>
</div> 
<p>Sample output for XML returntype </p>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre class="code XML"><span class="sc3"><span class="re1">&lt;?xml</span> <span class="re0">version</span>=<span class="st0">"1.0"</span> <span class="re0">encoding</span>=<span class="st0">"UTF-8"</span> <span class="re2">?&gt;</span></span>
<span class="sc3"><span class="re1">&lt;results<span class="re2">&gt;</span></span></span>
  <span class="sc3"><span class="re1">&lt;result<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;UserName<span class="re2">&gt;</span></span></span>Sunil<span class="sc3"><span class="re1">&lt;/UserName<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;Status<span class="re2">&gt;</span></span></span>Success<span class="sc3"><span class="re1">&lt;/Status<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;Description<span class="re2">&gt;</span></span></span>Registration is success.<span class="sc3"><span class="re1">&lt;/Description<span class="re2">&gt;</span></span></span>
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
                  {"UserName":"sunil","Status":"Success","Description":"Registration is success."}
 
             ]  
          }
}
</pre>
</div>
</div> 
<h3 id="updateroster">Update Roster API</h3>
 <p><strong>Description:</strong> This api is used to retrieve the list of users in the system. <br>
<strong>End Point URL:</strong> <a href="<?php echo getDomainDoc();?>/apis/ext/roster/UpdateRoster"><?php echo getDomainDoc();?>/apis/ext/roster/UpdateRoster</a><br>
<strong>Pre requisites:</strong> Access token needs to be populated before access this api.<br>
<strong>Input Parameters:</strong> Following will be the list of Optional/Mandatory parameters to make this api call. </p>
<h3>Optional Params </h3>
<ul>
  <li>  RegistrationStatus</li>
  <li>  CompletionStatus</li>
  <li>  Score</li>
  <li>  Grade</li>
   <li> PaymentStatus</li>
  <li>  CompletionDate</li>
  <li>  RegistrationDate</li>
    <li>  PaymentDate</li>
     <li> ValidFrom</li>
     <li> ValidTo</li>
</ul> 
<h3>Required params </h3>
  <ul>
<li>userid - Userid</li>
<li>Enrollids - List of enrollment ids with comma separated</li>
<li>ClassId - Id of the Class.</li>
<li>CourseId - Id of the Course.</li>
<li>actionkey - updateroster</li>
<li>returntype - xml (Note: API will return either xml or json object.)</li>
<li>display_columns - Needed columns from the response schema.(It is comma separated values).</li>
<li>CONSUMER_KEY</li>
<li>CONSUMER_SECRET_KEY</li>
<li>ACCESS_OAUTH_TOKEN (It will be available in cookies or sessions after successful authentication.)</li>
<li>ACCESS_TOKEN_SECRET (It will be available in cookies or sessions after successful authentication.)</li>	
</ul>   
<p><strong>API Response Schema</strong> API will give the following columns as response. </p> 
<ul>
   <li> UserName</li>
    <li>Status</li>
    <li>Description</li>
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
	$_GET["Enrollids"]="1,2";
	$_GET["ClassId"]="7";
	$_GET["CourseId"]="4";
	$_GET["RegistrationStatus"]="4";

	$_GET["display_columns"]="Message";
	$_GET["returntype"]="xml";   	
	$_GET["actionkey"]="updateroster";   	

	/* Create connection using client credentials and access tokens */

	$connection = new ExpertusONE_OAuth(CONSUMER_KEY, CONSUMER_SECRET, $access_oauth_token,$access_token_secret);
	$content = $connection->get("/apis/ext/roster/UpdateRoster",$_GET);
	echo $content;
</pre>	
</div>
</div> 
<p>Sample output for XML returntype </p>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre class="code XML"><span class="sc3"><span class="re1">&lt;?xml</span> <span class="re0">version</span>=<span class="st0">"1.0"</span> <span class="re0">encoding</span>=<span class="st0">"UTF-8"</span> <span class="re2">?&gt;</span></span>
<span class="sc3"><span class="re1">&lt;results<span class="re2">&gt;</span></span></span>
  <span class="sc3"><span class="re1">&lt;result<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;Message<span class="re2">&gt;</span></span></span>RegStatus:Ok,CompStatus:NULL,PayStatus:NULL,ValStatus:NULL,ScoreStatus:NULL,GradeStatus:NULL,UserId:34,UserName:Kumar6,OrderId:0<span class="sc3"><span class="re1">&lt;/Message<span class="re2">&gt;</span></span></span>
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
                  {"Message":"RegStatus:Ok,CompStatus:NULL,PayStatus:NULL,ValStatus:NULL,ScoreStatus:NULL,GradeStatus:NULL,UserId:34,UserName:Kumar6,OrderId:0"}
 
             ]  
          }
}
</pre>
</div>
</div> 

<h3 id="listroster">List Roster API</h3>
 <p><strong>Description:</strong> This api is used to retrieve the list of users in the system. <br>
<strong>End Point URL:</strong> <a href="<?php echo getDomainDoc();?>/apis/ext/roster/ListRoster"><?php echo getDomainDoc();?>/apis/ext/roster/ListRoster</a><br>
<strong>Pre requisites:</strong> Access token needs to be populated before access this api.<br>
<strong>Input Parameters:</strong> Following will be the list of Optional/Mandatory parameters to make this api call. </p>
<h3>Optional Params </h3>
<ul>
  <li>  None</li></ul> 
<h3>Required params </h3>
  <ul>
<li>userid - Userid</li>
<li> ClassId - Id of the Class.</li>
<li>CourseId - Id of the Course.</li>
<li>actionkey - listroster</li>
<li>returntype - xml (Note: API will return either xml or json object.)</li>
<li>display_columns - Needed columns from the response schema.(It is comma separated values).</li>
<li>CONSUMER_KEY</li>
<li>CONSUMER_SECRET_KEY</li>
<li>ACCESS_OAUTH_TOKEN (It will be available in cookies or sessions after successful authentication.)</li>
<li>ACCESS_TOKEN_SECRET (It will be available in cookies or sessions after successful authentication.)</li>
</ul>
   
<p><strong>API Response Schema</strong> API will give the following columns as response. </p> 
<ul>
<li>enrol_pid</li>
<li>user_id</li>
<li>class_id</li>
<li>course_id</li>
<li>reg_status</li>
<li>reg_date</li>
<li>comp_status</li>
<li>comp_date</li>
<li>userfullname</li>
<li>email</li>
<li>score</li>
<li>grade</li>
<li>comp_status</li>
<li>comp_status_name</li>
<li>grade_name</li>

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
	$_GET["userid"]="1";
	$_GET["ClassId"]="7";
	$_GET["CourseId"]="4";
	$_GET["display_columns"]="enroll_pid,userid,userfulname";
	$_GET["returntype"]="xml";   	
	$_GET["actionkey"]="listroster";   	

	/* Create connection using client credentials and access tokens */

	$connection = new ExpertusONE_OAuth(CONSUMER_KEY, CONSUMER_SECRET, $access_oauth_token,$access_token_secret);
	$content = $connection->get("/apis/ext/roster/ListRoster",$_GET);
	echo $content;
</pre>	
</div>
</div> 
<p>Sample output for XML returntype </p>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre class="code XML"><span class="sc3"><span class="re1">&lt;?xml</span> <span class="re0">version</span>=<span class="st0">"1.0"</span> <span class="re0">encoding</span>=<span class="st0">"UTF-8"</span> <span class="re2">?&gt;</span></span>
<span class="sc3"><span class="re1">&lt;results<span class="re2">&gt;</span></span></span>
  <span class="sc3"><span class="re1">&lt;result<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;enroll_pid<span class="re2">&gt;</span></span></span>1<span class="sc3"><span class="re1">&lt;/enroll_pid<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;userid<span class="re2">&gt;</span></span></span>1<span class="sc3"><span class="re1">&lt;/userid<span class="re2">&gt;</span></span></span>
    <span class="sc3"><span class="re1">&lt;userfulname<span class="re2">&gt;</span></span></span>Admin<span class="sc3"><span class="re1">&lt;/userfulname<span class="re2">&gt;</span></span></span>
&nbsp;
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
                  {"enroll_pid":"1","userid":"1","userfulname":"Admin"}
 
             ]  
          }
}
</pre>
</div>
</div> 


   
</div>
        </div>
   
      </div>    </div>

   <?php include 'footer.php'; ?> 
