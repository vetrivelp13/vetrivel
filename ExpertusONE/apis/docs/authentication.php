<?php include 'header.php'; ?> 
  <div id="content-outer">
      <div id="breadcrumbs">
<div class="breadcrumb"><a href="/">Home</a> &rarr; <a href="/docs">Documentation</a><div><a href="#" data-count="none" data-related="" class="share-button">Tweet</a></div></div>      </div>

      <h1 id="title">Authentication</h1>

      <div id="content-inner">

        <div id="content-main">

<!-- <div class="doc-updated">
    Updated on Mon, 2011-07-11 05:24  </div>  -->  

<div>
  
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<ol>
	<li class="toc-level-1"><a href="#authentication">Authentication</a>

<ol>
	<li class="toc-level-2"><a href="#authorization">Which authorization path should I choose? </a></li>
	<li class="toc-level-2"><a href="#detailarchitecture">Detailed Architecture of Authentication & Authorization.  </a></li>
	<li class="toc-level-2"><a href="#sample">OAuth request samples</a></li>
	<li class="toc-level-2"><a href="#Beginners">OAuth with ExpertusONE</a></li>
	<li class="toc-level-2"><a href="#obugs">OAuth FAQ </a></li>
	
</ol>

</li>

</ol>
</div>
</div>

<h2 id="authentication">Authentication</h2>
<div class="section">
  <h3 id="authorization">Which authorization path should I choose? </h3>
  <p>
  ExpertusONE supports a few of the popular authentication methods and with a range of OAuth authentication styles available, you may be wondering which method you should be using. When choosing which authentication method to use you should understand the way that method will affect your users experience and the way you write your application.

Some of you may already know which type of authentication method you want to use and we want to help you confirm you've made the right choice.
   
</p>
<h3>Web Applications</h3>
<p>Being browser based, a web application or service is able to use the full web-based OAuth process which means the user has the smoothest of the authentication methods.

To authenticate your web application with ExpertusONE you must use OAuth. Requests to use xAuth will be declined.

Web applications often support multiple users so it is important you manage the association of OAuth tokens to user identities carefully. The way you do this will be dependent on your application and setup.
</p>
<h3>Desktop and Mobile Applications</h3>
<p> Mobile and Desktop applications are most often single user applications such as command line scripts and applications on a phone. For these applications it can be difficult to handle a callback URL if not impossible. If you can handle a callback in your application we strongly encourage you use OAuth to authorize users. If not you should consider Out-of-band/PIN code authentication
</p>
<h3>Out-of-band/PIN Code Authentication</h3>
<p>For applications that really can't handle the full OAuth process ExpertusONE provides the out-of-band/PIN code authentication mode, also known as oob.

This authentication flow is almost identical to full OAuth except instead of being directed back to your website the user is presented with a PIN code. The user is then asked to type this PIN code into your application which will then complete the token exchange.

In the full OAuth flow this manual process is performed by the callback URL and transparently to the end user.
</p>
<h3 id="detailarchitecture">Detailed Architecture of Authentication & Authorization. </h3>
<p>ExpertusONE uses the open authentication standard OAuth for authentication. The OAuth request cycle comprises: </p>
<ul>
<li>Request an access token with client details</li>
<li>Request will be validated and authorized by OAuth authorization server</li>
<li>Receive access token upon successfull authentication</li>
<li>Request API with received access token</li>
<li>Following diagram will give you the overview of OAuth mechanism</li>
</ul>
<img alt="detailarchitecture" src="images/oauth_overview.png"><br>
<p>Following will be the detailed steps to access the authentication API. </p>
<p>Get the access token using client_id, client_secret. Following will be the example data to get the request token.</p>
<table class="inline"  style="border: 1px solid #EEEEEE;padding-right:5px;">
	<tbody><tr class="row0">
		<th class="col0 leftalign" style="padding-left: 5px;"> Key  </th><th class="col1 leftalign"> Value                                                                      </th>
	</tr>
	<tr class="row1">
		<td class="col0 leftalign" style="padding-left: 5px;"> <acronym title="Hyper Text Transfer Protocol">HTTP</acronym> Method            </td><td class="col1 leftalign"> POST                                                                       </td>
	</tr>
	<tr class="row2">
		<td class="col0 leftalign" style="padding-left: 5px;"> REQUEST <acronym title="Uniform Resource Identifier">URI</acronym>            </td><td class="col1 leftalign"> <a rel="nofollow" title="<?php echo getDomainDoc();?>/apis/oauth2/Token.php" class="urlextern" href="<?php echo getDomainDoc();?>/apis/oauth2/Token.php"><?php echo getDomainDoc();?>/apis/oauth2/Token.php</a>                    </td>
	</tr>
	<tr class="row3">
		<td class="col0 leftalign" style="padding-left: 5px;"> client_id     </td><td class="col1 leftalign"> ef7c21f8966db0044802c1a7f19b8da2  </td>
	</tr>
	<tr class="row4">
		<td class="col0 leftalign" style="padding-left: 5px;"> client_secret    </td><td class="col1 leftalign"> 4500e1dc6ded85fecb61cacb403b162d </td>
	</tr>
	<tr class="row5">
		<td class="col0 leftalign" style="padding-left: 5px;"> grant_type          </td><td class="col1 leftalign"> client_credentials   </td>
	</tr>
</tbody></table>

<div id='sample'>
<h3> Request sample using cURL </h3>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre>

/* Request for getting access token */
curl -u <client_id>:<client_secret>  <site url>/apis/oauth2/Token.php -d grant_type=client_credentials

eg: curl -u ef7c21f8966db0044802c1a7f19b8da2:4500e1dc6ded85fecb61cacb403b162d  <?php echo $base_url; ?>/apis/oauth2/Token.php -d grant_type=client_credentials

/* Above request will return the access token. Use it for API call */

curl <site url>/apis/ext/ExpertusOneAPI.php -d "userid=1&display_cols=id,username,firstname,lasttname,email&returntype=json&limit=10&userstatus=cre_usr_sts_atv&apiname=ListUserAPI&access_token=<access token>"  

eg: curl <?php echo $base_url; ?>/apis/ext/ExpertusOneAPI.php -d "userid=1&display_cols=id,username,firstname,lasttname,email&returntype=json&limit=10&userstatus=cre_usr_sts_atv&apiname=ListUserAPI&access_token=479f3da09a1f15b023d5aa42339ea2b6606bfaac"  

</pre>
</div>
</div>

<h3> Request sample using PHP </h3>
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<pre>
 session_start();

/* Request for access token */

$base_url = "<?php echo $base_url; ?>";
$client_id="ef7c21f8966db0044802c1a7f19b8da2";
$client_secret="4500e1dc6ded85fecb61cacb403b162d";

$client_details = base64_encode("$client_id:$client_secret");

$context = stream_context_create(array(
	'http' => array(
	'method' => 'POST',
	'header' => "Content-Type: application/x-www-form-urlencoded\r\n" .
				"Authorization : Basic $client_details\r\n",
	'content' => "grant_type=client_credentials"
	)
));
 
$response = file_get_contents($base_url.'/apis/oauth2/Token.php', false, $context);
$res =  json_decode($response);
$token = $res->access_token;

/* Request for API */

// API params
$data = array('apiname'=>'ListUserAPI',
			  'userstatus'=>'cre_usr_sts_atv',
			  'display_cols'=>'id,username,firstname,lasttname,email',
			  'userid'=>1,
			  'returntype'=>'json',
			  'limit'=>'10'
			  );
			  
$fields_string = http_build_query($data);

$context = stream_context_create(array(
	'http' => array(
	'method' => 'POST',
	'header' => "Content-Type: application/x-www-form-urlencoded\r\n" .
				"Authorization : Bearer $token\r\n",
	'content' => $fields_string
	)
)); 

$response = file_get_contents($base_url.'/apis/ext/ExpertusOneAPI.php', false, $context);
$res =  json_decode($response);
var_dump($res);
	
</pre>
</div>
</div>
</div>


<h3 id="Beginners">OAuth with ExpertusONE</h3>

<p>
	<ul>
		<li>In order to access ExpertusONE API, you need to register and purchase a license. Contact system administrator for more details</li>
		<li>While purchasing license, you will get two keys (client id & client secret)</li>
		<li>Using these keys you can genetate a access token</li>
		<li>Passing this access token along with API parameters you can receive data from ExpertusONE</li>
	</ul>
</p>

<p></p>

<p>OAuth 2.0 supports four grant types such as Authorization Code, Client Credential, Implicit and Resource owner password. But ExpertusONE integrated with only Client Credential other grant types will not be supported by ExpertusONE.</p>

<p>Resource access will be restricted by scope. ExpertusONE comes with three types of scopes for their resource access such as E1.API, E1.APP and E1.DL.
	<ul>
		<li>E1.API – Clients who have this scope can access all API to update and retrieve information.</li>
		<li>E1.APP – Clients who have this scope can access ExpertusONE mobile APP.</li>
		<li>E1.DL – Clients who have this scope can success Data Load specific APIs to upload data</li>
	</ul>
</p>

<h3 id="obugs">OAuth FAQ</h3>
<h3>What is OAuth?</h3>
<p>OAuth is an authentication protocol that allows users to approve application to act on their behalf without sharing their password. More information can be found at oauth.net or in the excellent Beginner's Guide to OAuth from Hueniverse. </p>
<h3>What is OAuth2.0?</h3>
<p>OAuth 2.0 is the next evolution of the OAuth protocol and is not backwards compatible with OAuth 1.0. OAuth 2.0 focuses on client developer simplicity while providing specific authorization flows for web applications
The OAuth 2.0 authorization framework enables a third-party application to obtain limited access to an HTTP service, either on behalf of a resource owner by orchestrating an approval interaction between the resource owner and the HTTP service, or by allowing the third-party application to obtain access on its own behalf.
</p>
<h3>Does ExpertusONE support both OAuth 1.0 and OAuth 2.0?</h3>
<p>No. ExpertusONE supports only OAuth 2.0</p>
<h3>How long does an access token last?</h3>
<p>Access token expiry time is configurable. Default value is 60 minutes</p>
<h3>What is scope? How many scopes are available and what are they?</h3>
<p>Scope is a boundary of the resource accessibility. ExpertusONE comes with three types of scopes for their resource access for API, Mobile APP and Data Load. </p>
<h3>Does a single client key support all three scopes?</h3>
<p>No, a single key will support only one scope.</p>
</div>


</div>
        </div>
   
      </div>    </div>

   <?php include 'footer.php'; ?> 