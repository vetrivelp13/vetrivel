<html>
<body>
<form method="post" >
<select id='language' name='language'>
	<option value='---Select---'>Select One</option>
	<option value='zh-CN'>Simplied Chinese</option>
	<option value='ja'>Japanese</option>
	<option value='ko'>Korean</option>
	<option value='it'>Italian</option>
	<option value='pt'>Portuguese</option>
	<option value='ru'>Russian</option>
	<option value='fr'>French</option>
	<option value='es'>Spanish</option>
	<option value='de'>German</option>
</select>	
<input type='submit' name="submit"></input>
</form>

<?php
if(isset($_REQUEST["submit"]))
{
 $toLang = $_REQUEST["language"];
 ?> selected lang:<?php print $toLang;?><br/><br/>
 <?php
if ($file = fopen("en.txt", "r")) {
	$msgidkey = "";
	//ob_start();
    while(!feof($file)) {
        $line = fgets($file);
		if(trim($line) != "")
		{
	        $pieces = explode("\"", $line);
	        if(isset($pieces[0]))
		        $key = trim($pieces[0]);
	        if(isset($pieces[1]))
		        $val = trim($pieces[1]);
	        if($key == "msgid")
	        {
		        $msgidkey = $val;
	        }
    	    else if($key == "msgstr")
    	    {
    	    sleep(1);
    	    $data =  file_get_contents("http://translate.googleapis.com/translate_a/single?client=gtx&sl=auto&tl=".$toLang."&dt=t&q=".urlencode($pieces[1]));
    	    $jsonArr = json_decode($data,true);
			echo "msgid \"".$msgidkey."\"<br/>msgstr \"". $jsonArr[0][0][0]."\"</br></br>";
			//echo "msgid \"".$msgidkey."\"<br/>msgstr \"". $val."\"</br></br>";
		//	 ob_flush();
			}
    	}
    }
    fclose($file);
}
}
?>
</body>