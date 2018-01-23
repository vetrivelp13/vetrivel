<?php include 'header.php'; ?> 
  <div id="content-outer">
      <div id="breadcrumbs">
<div class="breadcrumb"><a href="index.php">Home</a> &rarr; <a href="#">Documentation</a><div><a href="#" data-count="none" data-related="" class="share-button">Tweet</a></div></div>      </div>

      <h1 id="title">Other Resourcess</h1>

      <div id="content-inner">

        <div id="content-main">

<!--  <div class="doc-updated">
    Updated on Mon, 2011-07-11 05:24  </div> --> 

<div>
  
<div class="toc" id="toc">
<div class="toc-title"></div>
<div class="toc-list">
<ol>
	<li class="toc-level-1"><a href="#otherres">Other Resources</a>

<ol>
	<li class="toc-level-2"><a href="#security">Security Best Practices  </a></li>
	<!-- <li class="toc-level-2"><a href="#expertuslibrary">Download & Installation of ExpertusONE ReST library  </a></li>  -->
	
	
</ol>

</li>

</ol>
</div>
</div>

<h2 id="otherres">Other Resourcess</h2>
<div class="section">
  <h3 id="security">Security Best Practices</h3>
  <h3>Introduction</h3>
  <p>
  A successful ExpertusONE application is likely to get some attention. Most of that attention will be good: users singing your praises, other developers complimenting your programming chops. Some of that attention, though, might be negative. As ExpertusONE has increased in popularity, the ExpertusONE ecosystem has become a more visible target for malicious hackers. Some are out to spread spam and malware, while others may just be spreading mayhem for fun. Whatever their motives, your application may be a target.

This page is designed to put you on a path towards better security in your application. It's not the final word; far from it. If there's anything you'd like to see added to it, please let us know. If you've discovered a security issue that directly affects ExpertusONE, please email sysadmin@expertus.com.
  
   
</p>
<h3>Password Retention</h3>
<p>
In short, do not retain passwords. ExpertusONE's support of HTTP Basic Authentication (the standard way to authenticate over the web with a username and password) will be deprecated. Please use OAuth.

If you are retaining passwords, please encrypt them. At ExpertusONE, we use bcrypt-ruby, but there are lots of other ways to store encrypted passwords. But, don't. Don't do it. Don't store passwords. Just store OAuth tokens. Please.
<p>
<h3>Input Validation</h3>
<p>Don't assume that your users will provide you with valid, trustworthy data. Sanitize all data, checking for sane string lengths, valid file types, etc. ExpertusONE attempts to sanitize data POSTed to our APIs, but a little client-side help goes a long way. Whitelist the types of input that are acceptable to your application and discard everything that isn't on the whitelist. 
</p>
<h3>Unencrypted Communication (no SSL)</h3>
<p>ExpertusONE provides all REST API methods over SSL. Whenever your code might be operating on an untrusted network (that is, if you're developing a client application), please make use of SSL for all authenticated or sensitive requests. For example, posting a status, requesting recent direct messages, and updating profile attributes should all be performed over SSL in a ExpertusONE client. It's safe and suggested to use SSL in conjunction with OAuth. Service-to-service communication may not benefit from SSL if you trust your hosting provider (or are your own hosting provider). 
</p>
<h3>Exposed Debugging Information</h3>
<p>Be sure that you're not exposing sensitive information through debugging screens/logs. Some web frameworks make it easy to access debugging information if your application is not properly configured. For desktop and mobile developers, it's easy to accidentally ship a build with debugging flags or symbols enabled. Build checks for these configurations into your deployment/build process. </p>


<h3>Inadequate Testing</h3>
<p>Ensure that your tests (you do have tests, right?) check not just that you can do what should be able to do, but that bad guys can't do what they shouldn't be able to do. Put yourself in an attacker's mindset and whip up some evil tests.</p>

<h3>Not Letting People Help</h3>
<p>Have you set up security@yourapplication.com? Do those emails go right to your phone? Make it easy for people to contact you about potential security issues with your application. If someone does report a security flaw to you, be nice to them; they've just done you a huge favor. Thank them for their time and fix the issue promptly. It's fairly common for security researchers to write about vulnerabilities they've discovered once the hole has been closed, so don't be upset if your application ends up in a blog post or research paper. Security is hard, and nobody is perfect. As long as you're fixing the issues that are reported to you, you're doing right.

Consider hiring security professionals to do an audit and/or penetration test. You can't depend solely on the kindness of strangers; for every vulnerability that someone was nice enough to report to you, there's ten more that malicious hackers have found. A good security firm will dig deep to uncover issues. Look for firms and individual consultants that do more than run a few automated tools.
</p>
<h3>The Law</h3>
<p>If your application is (going to be) handling money, you may be required by law to adhere to certain security practices and regulations. Find out what's applicable to you and make sure you're up to code. </p>

<h3>Web Application Security</h3>


<h3>Unfiltered Input, Unescaped Output</h3>
<p>One easy-to-remember approach to input validation is FIEO: Filter Input, Escape Output. Filter anything from outside your application, including ExpertusONE API data, cookie data, user-supplied form input, URL parameters, data from databases, etc. Escape all output being sent by your application, including SQL sent to your database server, HTML to you send to users' browsers, JSON/XML output sent to other systems, and commands sent to shell programs. </p>


<h3>Cross-Site Scripting (XSS)</h3>
<p>XSS attacks are, by most measures, the most common form of security problem on the web. In short, if an attacker can get their own JavaScript code into your application, they can do bad stuff. Anywhere you store and display untrusted, user-supplied data needs to be checked, sanitized, and HTML escaped. Getting this right is hard, because hackers have many different ways to land XSS attacks. Your language or web development framework probably has a popular, well-tested mechanism for defending against cross-site scripting; please make use of it. </p>
<p>
Generally: if HTML isn't needed from some user-facing form, filter it out; for example, there's no reason to allow anything other than integers when storing a phone number. If HTML is needed, use a known-good whitelist filter. HTMLPurifier for PHP is one such solution. Different contexts may require different filtering approaches. See the OWASP XSS Prevention Cheat Sheet for more on filtering. 
</p>

<h3>SQL Injection</h3>
<p>If your application makes use of a database, you need to be aware of SQL injection. Again, anywhere you accept input is a potential target for an attacker to break out of their input field and into your database. Use database libraries that protect against SQL injection in a systematic way. If you break out of that approach and write custom SQL, write aggressive tests to be sure you aren't exposing yourself to this form of attack.</p>
<p>The two main approaches to defending against SQL injection are escaping before constructing your SQL statement and using parameterized input to create statements. The latter is recommended, as it's less prone to programmer error. </p>

<h3>Cross-Site Request Forgery (CSRF)</h3>
<p>Are you sure that requests to your application are coming from your application? CSRF attacks exploit this lack of knowledge by forcing logged-in users of your site to silently open URLs that perform actions. In the case of a ExpertusONE app, this could mean that attackers are using your app to force users to post unwanted calls. You can learn more about this sort of attack on PHP security. </p>

<p>The most thorough way to deal with CSRF is to include a random token in every form that's stored someplace trusted; if a form doesn't have the right token, throw an error. Modern web frameworks have systematic ways of handling this, and might even be doing it by default if you're lucky. A simple preventative step (but by no means the only step you should take) is to make any actions that create, modify, or destroy data require a POST request. </p>

<h3>Limit the spammers and attackers</h3>
<p>CAPTCHAs where appropriate to slow down potential spammers and attackers. </p>


<h3>Lack of Information about Threats</h3>
<p>If you think there's an issue with your web application, how do you find out for sure? Have critical exceptions and errors emailed to you and keep good logs. You may want to put together a dashboard of critical statistics so that you can see at a glance if something is going wrong (or staying right). </p>

<!-- 
  <h3 id="expertuslibrary">Download & Installation of ExpertusONE ReST library (PHP) </h3>
  <p>Download <a href="/apis/expertusone_oauth/expertusone_oauth.zip">expertusone_oauth.zip</a> file. Do the following steps. </p>
  <ol>
  	<li>Extract the zip file and copy expertusone_oauth folder into your web application folder. </li>
  	<li> Execute the steps which is mentioned in readme.txt
  	<li>Now, you are ready with the installation. Refer authentication API to proceed with invoking APIs. If you have any questions regarding the installation please write an email to sysadmin@expertus.com </li>
  </ol>
  <h3 id="expertuslibrary">Download & Installation of ExpertusONE ReST library (JAVA) </h3>
  <p>If you want call ReST API from JAVA, please refer this URL <a target="_blank" href="http://code.google.com/p/oauth-signpost">http://code.google.com/p/oauth-signpost</a>.</p>
 -->
</div>


</div>
        </div>
   
      </div>    </div>

   <?php include 'footer.php'; ?> 