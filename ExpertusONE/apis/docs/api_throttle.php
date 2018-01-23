<?php include 'header.php'; ?>
<div id="content-outer">
	<div id="breadcrumbs">
		<div class="breadcrumb"><a href="index.php">Home</a> &rarr; <a href="#">Documentation</a>
			<div><a href="#" data-count="none" data-related="" class="share-button">Tweet</a></div>
		</div>
</div>

<h1 id="title">API Throttle</h1>

<div id="content-inner">

<div id="content-main"><!--  <div class="doc-updated">
    Updated on Fri, 2015-02-27 13:24  </div> -->

<div>

<h2 id="about">About Throttling</h2>
<p>
API throttle is a mechanism to limit the number of API requests for a specific time frame. Throttling is used to prevent abuse of APIs as well as to prevent application server from crashing/going down.
</p>
<h2 id="throttle">API Throttle in ExpertusONE</h2>

<p>
ExpertusONE limits the number of API requests in two different ways. One way is by limiting each API and the other way is by limiting the total number of requests (any API request). This will be controlled by a configuration which can be viewed in the configuration file (ini).
</p>
<div>
	<table id="throttle" style="border: 1px solid; border-collapse: collapse;">
		<tr>
			<td><strong>Parameter</strong></td>
			<td><strong>Default Value</strong></td>
			<td><strong>Possible Values</strong></td>
			<td><strong>Purpose</strong></td>
		</tr>
		<tr>
			<td>throttling_by_each_api</td>
			<td>0</td>
			<td>0 or 1</td>
			<td>0 (Limit by total number of request.) <br/> 1 (Limit by API request.)</td>
		</tr>
		<tr>
			<td>max_api_request_per_minute</td>
			<td>50</td>
			<td>Any</td>
			<td>Default throttle value per minute</td>
		</tr>
	</table>
</div>
<h4>Limiting by total number of requests</h3>
<p>
When the parameter &quotthrottling_by_each_api&quot is configured to 0 then the system will limit the requests by the number configured in &quotmax_api_request_per_minute&quot (assume value configured as 50). In this case the system will not consider the throttle value which is configured in each API. 
</p>
<p>
For example, List User APIs throttle value is configured as 100 and the List Organization API does not have any throttle value. Then the total numbers of allowed requests are 50 only (irrespective of the number of APIs).
</p>
<h4>Limiting by API request</h3>
<p>
API level throttle value can be configured against each API's configuration parameter &quotthrottle&quot in REST API configuration file(ini).
</p>
<p>
If the parameter &quotthrottling_by_each_api&quot is configured to 1, then based on each API level throttle value the system will allow the relevant number of requests for each API, if not configured then it will pick the values configured in &quotmax_api_request_per_minute&quot. 
</p>
<p>For example, List User API's throttle value is configured as 100 and List Organization API does not have any throttle value. Then the system will allow 100 requests(value configured at API level) per minute for List User and 50 requests(values configured in &quotmax_api_request_per_minute&quot) per minute for List Organization. The total numbers of allowed requests are 150 per minute.</p>
</div></div></div>
<br/><br/>
<div>
<h2>FAQ</h2>
<h4>Can the throttle by turned OFF?</h4>
<p>
No, the throttle itself cannot be turned OFF. But, the default values can be increased or decreased.
</p>
<h4>Can clients change the default values?</h4>
<p>
No, the default value cannot be changed by clients.
</p>
<h4>How to change the default values?</h4>
<p>
To change the default values, reach out to the ExpertusONE support team. The support team will change the default value based on the customer's request.
</p>
<h4>What are the terms and condition for changing the default values?</h4>
<p>
Reach out to the support team to know the terms and condition for changing the default values.
</p>
</div>
</div>

<?php include 'footer.php'; ?>