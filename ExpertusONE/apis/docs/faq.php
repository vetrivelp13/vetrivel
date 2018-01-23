<?php include 'header.php'; ?> 
  <div id="content-outer">
      <div id="breadcrumbs">
<div class="breadcrumb"><a href="index.php">Home</a> &rarr; <a href="#">Documentation</a><div><a href="#" data-count="none" data-related="" class="share-button">Tweet</a></div></div>      </div>

      <h1 id="title">API FAQ</h1>

      <div id="content-inner">

        <div id="content-main">

<!--   <div class="doc-updated">
    Updated on Mon, 2011-07-11 05:24  </div> -->

<div>
  
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<ol>
	<li class="toc-level-1"><a href="#basics">Basics</a>

<ol>
	<li class="toc-level-2"><a href="#api">What's an API?</a></li>
	<li class="toc-level-2"><a href="#how">How do I use ExpertusONE API?</a></li>
	<li class="toc-level-2"><a href="#proper_data">How do I ensure APIs return proper data?</a></li>
	<!--  TBD Answered Menu are Commented For this Ticket #0040458 -->
	<!-- <li class="toc-level-2"><a href="#bugs">How do I report bugs and request features?</a></li>
	<li class="toc-level-2"><a href="#changeapi">How can I keep up with changes to the ExpertusONE API?</a></li>
	<li class="toc-level-2"><a href="#down">Is ExpertusONE API down?</a></li>  -->
	<li class="toc-level-2"><a href="#blacklist">Is my IP banned or blacklisted?</a></li>
</ol>

</li>

</ol>
</div>
</div>

<h2 id="basics">Basics</h2>
<div class="section">
  <h3 id="api">What's an API?</h3>
  <p>
  The acronym &quotAPI&quot stands for &quotApplication Programming Interface&quot. An API is a defined way for a program to accomplish a task, usually by retrieving or modifying data. In ExpertusONE's case, we provide an API method for mandatory functionalities in LMS that you can see on our website. Programmers use the ExpertusONE API to make applications, websites, widgets, and other projects that interact with ExpertusONE. Programs talk to the ExpertusONE API over HTTP, the same protocol that your browser uses to visit and interact with web pages. 
</p>

  <h3 id="how">How do I use ExpertusONE API?</h3>
  <p>A license is mandatory to access the ExpertusONE API. To purchase a license, please contact the ExpertusONE Administrator. After purchasing the license, read the technical documentation to use the ExpertusONE APIs for your applications. </p>

  <h3 id="proper_data">How do I ensure APIs return proper data?</h3>

  <p>APIs are the wrapper classes for the existing functionalities. Data returns from API should be matched with the data in web site. To verify the data, login to the site and compare the data in the site with the data returned through API.. </p>
  <!--  TBD Answer are Commented For this Ticket #0040458 -->
  <!-- <h3 id="bugs">How do I report bugs and request features?</h3>

  <p>TBD  </p>
  
  <h3 id="changeapi">How can I keep up with changes to the ExpertusONE API?</h3>

  <p>TBD .</p>
  
  <h3 id="down">Is ExpertusONE API down?</h3>

  <p>TBD .</p> -->
  <h3 id="blacklist">Is my IP banned or blacklisted?</h3>

  <p>
  Being banned or blacklisted means the ExpertusONE APIs will not respond to requests you make to them. If this happens, the first thing to do is stop any requests your application is making. Then check if you can reach other URLs using a command line tool like &quotcURL&quot. If you can access other URLs but not ExpertusONE, you should login to ExpertusONE.com and file a ticket with our support team. The support team will then reach out to you with the remedial steps to be taken. 
  </p>
  <p>The best way to avoid being blacklisted is to pay attention to the remaining API requests you are allowed to make, and to handle errors appropriately. Handling errors appropriately means reducing your request frequency (throttling) or stopping requests until you can identify why the request failed. </p>
</div>


</div>
        </div>
   
      </div>    </div>

   <?php include 'footer.php'; ?> 