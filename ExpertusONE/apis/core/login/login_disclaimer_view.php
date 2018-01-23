<?php

/**
    * When user logged in through external system, it will be getting displayed. 
    * It will Grant or Deny the access to the external system based on the user selection.
    * @author Sureshkumar.v
*/


include "../HeaderView.inc";
?>
<style>
#grantaccess,#denyaccess
{

 	/*background: url("/sites/all/themes/core/expertusone/expertusone-internals/images/ExpertusIcons.png") no-repeat scroll -76px -208px transparent; */
    border: 0 none;
    color: white;
    cursor: pointer;
    float: right;
    font-size: 0.9em;
    margin-left: 10px;
    margin-top: 10px;
    padding: 5px 0;
	-moz-border-radius: 4px;
  	-webkit-border-radius: 4px;
 	border-radius: 4px;
 	border:1px solid #aaccee;
  	padding:5px 0px;
  	width:215px;
  	background-color:#F83C05;
}
</style>

        <div id="content-area">
         <fieldset>
<legend style='font-weight:bold;color:red;'>Disclaimer</legend>
 <br/><h3  style='padding-left:5px;'>ExpertusONE accounts</h3>   
<br>
<p style='padding-left:5px;'>
The site www.externalsite.com is requesting access to your expertusone's enrollment data. ExpertusONE is not affiliated with www.externalsite.com, and we recommend that you grant access only if you trust the site.<br>
<br></p>
<p  style='padding-left:5px;'>
If you grant access, you can revoke access any time under "My Profile". www.externalsite.com will not have access to your password or any other personal information from your ExpertusONE account.
</p>
<p><br></p>
<p  style='padding-left:5px;text-align:right;'><input id='grantaccess' value="Grant Access" class='form-submit' type="button" onclick='doGrantAccess();'>&nbsp;<input id='denyaccess' class='form-submit'  value="Deny Access" type="button" onclick='window.close();'></p><br/>
</fieldset>

 </div>

        
        
      </div></div><!-- /.section, /#content -->

      
      
      
    </div></div><!-- /#main, /#main-wrapper -->

         
  
<?php
include "../FooterView.inc";
?>
