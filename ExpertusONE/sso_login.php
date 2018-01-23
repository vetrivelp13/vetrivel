<HTML>
<BODY>
<H1>Welcome to SSO Login</H1>
<form action="sso_ldap.php" method='post'>
<frameset style='border:1px SOLID #000;'>
<legend>Login</legend>
<br/>
<table cellspacing=0 cellpadding=0 style='padding:5px;width:30%;height:auto;'>
<tr>
<td style='color:red;font-size:18px;text-align:center;' colspan=2>
<?php if(isset($_GET["sso_err"])) echo $_GET["sso_err"];?>	
</td></tr>
<tr>
	<td style='padding:5px;'>Username:</td><td><input type='text' size=20 name='username' value=''></input></td>
</tr>
<tr>
	<td  style='padding:5px;'>Password:</td><td><input type='password' size=20 name='pwd' value=''></input></td>
</tr>
<tr><td colspan='2'><input type='submit' name='submit' value='Submit'></td>
</table>
</frameset>
</form>
</BODY>
</HTML>
