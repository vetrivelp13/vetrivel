<?php
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>DATA LOAD API</title>
<!-- <script src="/apis/expertusone_oauth/simulator/jquery161.js"></script>  -->
<script>
function loadParamForm(){
	var apin = document.getElementById("reqtype").value;
	
	//Please note any entity is added or removed should get modified here.
	
	var entityList =	[{val:'cre_usr_jrl',name:'Job role'},{val:'cre_usr_ptp',name:'User Type'},{val:'cre_usr_dpt',name:'Department'},
	                		{val:'cre_usr_etp',name:'Employment Type'},{val:'cre_usr_jtl',name:'Job title'},{val:'cre_org',name:'Organization'},{val:'cre_usr',name:'Users'}];

	var cnt = entityList.length;
	var data = '';
	var action ='';
  data += '<form id ="datareq" name="datareqform" style="width:65%;margin:auto;clear:both;" method ="POST" action ="">';
	data += '<table>';
	if(apin == 'authenticate'){
		data += '<tr><td><label>OAuth Client Id:</label><span>*</span></td><td><input type="text" name="Client_Id" id="client_id" value="" class = "mandatory"><span id="client_id-msg"></span></td></tr>';
		data += '<tr><td><label>OAuth Client Secret Key:</label><span>*</span></td><td><input type="text" name ="Client_Secret" id="client_secret" value="" class = "mandatory"><span id="client_secret-msg"></span></td></tr>';
		data += '<tr><td><label>User Id:</label><span>*</span></td><td><input type="text" name="UserId" id="userid" class = "mandatory" value="1"><span id="userid-msg"></span></td></tr>';
		action = "Execute";
	}else if(apin == 'createReq'){
		var date = getDateFormat();
		data += '<tr><td><label>File Upload:</label><span>*</span></td><td><input type="file" id="fileupload" name="File Upload" value="" class = "mandatory"><span id="fileupload-msg"></span></td></tr>';
		data += '<tr><td><label>Process Type:</label><span>*</span></td><td><select name ="Process Type" id="protype" class = "mandatory"><option value ="api">API</option><option value ="bulk">BULK</option></select><span id="protype-msg"></span></td></tr>';
		//data += '<tr><td><label>Upload Type:</label><span>*</span></td><td><select name ="Upload Type" id="uploadtype" class = "mandatory"><option value ="exist">Exist and Update</option><option value ="fresh">Fresh and Update</option></select><span id="uploadtype-msg"></span></td></tr>';
		data += '<tr><td><label>Entity Name:</label><span>*</span></td><td><select name="Entity type" id ="entitytype" class = "mandatory"><option value="">Select</option>';
		for(var i=0;i<cnt ;i++){
			data += '<option value="'+entityList[i].val+'">'+entityList[i].name+'</option>';
		}
		data +='</select><span id="entitytype-msg"></span></td></tr>';
		data += '<tr><td><label>User Id:</label><span>*</span></td><td><input type="text" name="UserId" id="userid" value="" class = "mandatory"><span id="userid-msg"></span></td></tr>';
		data += '<tr><td><label>Mail To:</label></td><td><input type="text" name="Mail To" id="email" value=""><span id="email-msg"><label style="color: #999;font-size: 13px">Separate email addresses with commas</label></span></td></tr>';
		data += '<tr><td><label>Starts When:</label></td><td><input type="datetime" name="Starts Date" id="startdate" value="'+date+'" class = ""><span id="startdate-msg"><label style="color: #999;font-size: 13px">MM/DD/YYYY HH:MM</label></span></td></tr>';
		action = "Execute";
	}else if(apin == 'stsReq'){
		data += '<tr><td><label>Job Id:</label><span>*</span></td><td><input type="text" name="JobId" id="jobid" value="" class = "mandatory"><span id="jobid-msg"></span></td></tr>';
		data += '<tr><td><label>Status Type:</label><span>*</span></td><td><select name ="Status Type" id="ststype" class = "mandatory"><option value ="short">Short</option><option value ="full">Detailed</option></select><span id="ststype-msg"></span></td></tr>';
		data += '<tr><td><label>User Id:</label><span>*</span></td><td><input type="text" name="UserId" id="userid" class = "mandatory" value=""><span id="userid-msg"></span></td></tr>';
		action = "Execute";
	}else if(apin == 'pauseReq'){
		data += '<tr><td><label>Job Id:</label><span>*</span></td><td><input type="text" name="JobId" id="jobid" value=""  class = "mandatory"><span id="jobid-msg"></span></td></tr>';
		data +=	'<tr><td><label>Action:</label><span>*</span></td><td><select name ="Action Type" id="acttype" class = "mandatory"><option value ="pause">Pause</option><option value ="resume">Resume</option></select><span id="acttype-msg"></span></td></tr>';
		data += '<tr><td><label>User Id:</label><span>*</span></td><td><input type="text" name="UserId" id="userid" class = "mandatory" value=""><span id="userid-msg"></span></td></tr>';
		action = "Execute";
	}else if(apin == 'endReq'){ 
		data += '<tr><td><label>Job Id:</label><span>*</span></td><td><input type="text" name="JobId" id="jobid" value=""  class = "mandatory" ><span id="jobid-msg"></span></td></tr>';
		data += '<tr><td><label>User Id:</label><span>*</span></td><td><input type="text" name="UserId" id="userid" class = "mandatory" value=""><span id="userid-msg"></span></td></tr>';
		action = "Terminate";
	}else{
		//document.getElementById("Request-content").innerHTML = '';
		document.getElementById("Detail-content").innerHTML = '';
		document.getElementById("api-response").innerHTML = '';
		return '';
	}
	
	data +='<tr><td><input type="hidden" name="req-type" value="'+apin+'"></td></tr><tr><td style="text-align:right;"><input type="button" value="'+action+'" onclick="mandatoryCheck(\''+apin+'\')"></td></tr>';
	data +='</table></form>';
	document.getElementById("Detail-content").innerHTML = data;
	document.getElementById("api-response").innerHTML = '';
		 
}

function getDataLoad(apin)
{
 var formData = new FormData();
	switch(apin){
	case 'authenticate':
		formData.append('operation','authenticate');
		formData.append('client_id',document.getElementById('client_id').value);
		formData.append('client_secret',document.getElementById('client_secret').value);
		formData.append('User_Id',document.getElementById('userid').value);
		break;
	case 'createReq':
		var files = document.getElementById('fileupload').files;
		formData.append('File_Upload', files[0], files[0].name);
		formData.append('Process_Type',document.getElementById('protype').value);
		//formData.append('Upload_Type',document.getElementById('uploadtype').value);
		formData.append('Entity_Name',document.getElementById('entitytype').value);
		formData.append('User_Id',document.getElementById('userid').value);
		formData.append('Mail_To',document.getElementById('email').value);
		formData.append('Starts_when',document.getElementById('startdate').value);
		break;
	case 'stsReq':
		formData.append('Job_Id',document.getElementById('jobid').value);
		formData.append('Status_Type',document.getElementById('ststype').value);
		formData.append('User_Id',document.getElementById('userid').value);
		break;
	case 'pauseReq':
		formData.append('Job_Id',document.getElementById('jobid').value);
		formData.append('Action_Type',document.getElementById('acttype').value);
		formData.append('User_Id',document.getElementById('userid').value);
		break;
	default:
		formData.append('Job_Id',document.getElementById('jobid').value);
		formData.append('User_Id',document.getElementById('userid').value);
		break;
}
	formData.append('baseurl',document.getElementById("baseurl").value);
	formData.append('Request_Type',document.getElementById("reqtype").value);
	formData.append('Return_Type',document.getElementById("rettype").value);
	formData.append('access_token',document.getElementById("access_token").value);
	
	var xhr;
    try{
        xhr = new XMLHttpRequest();
    }catch (e){
        try{
            xhr = new XDomainRequest();
        } catch (e){
            try{
                xhr = new ActiveXObject('Msxml2.XMLHTTP');
            }catch (e){
                try{
                    xhr = new ActiveXObject('Microsoft.XMLHTTP');
                }catch (e){
                    statusField('\nYour browser is not' + 
                        ' compatible with XHR2');                           
                }
            }
        }
    }
  document.getElementById("api-response").innerHTML = "Processing.."; // Remove previous entry if any
	xhr.open('POST', 'dataload_ajax.php', true);
	xhr.onload = function () {
		  if (xhr.status === 200) {
			  // your code for show processing.... 
		  } else {
		    alert('An error occurred!');
		  }
		};
		xhr.onreadystatechange = function (e) {
	        if (xhr.readyState == 4 && xhr.status == 200) {
	            // your code upon success 
	            //alert(xhr.responseText)
	           // alert(JSON.parse(xhr.responseText))
	           //alert(xhr.responseXML)
	           if(apin=='authenticate' && xhr.responseText != 'Invalid Base url'){
		            var res = JSON.parse(xhr.responseText.trim()); 
								var token = res.access_token;
								if(typeof token != 'undefined'){
									document.getElementById("access_token").value=token;
									document.getElementById("api-response").innerHTML = "Authenticated successfully";
								}else{
									document.getElementById("api-response").innerHTML = xhr.responseText;
								}
	           }else{
	            document.getElementById("api-response").innerHTML = xhr.responseText;
	           }
	        }else{
							// your code upon failure
		        }
	    };
	xhr.send(formData);
	/*var form = document.getElementById("datareq");
	form.action = "http://priyadev.expertusone.com/sites/all/modules/core/exp_sp_core/modules/exp_sp_data_load/dataload_handler.php";
	form.submit();*/
	
}
function mandatoryCheck(submitbut){
	var fields = document.getElementById("main-content").getElementsByClassName("mandatory");
	var fieldLen = fields.length;
	var errFlag = 0;
	for(var i=0; i < fieldLen; i++ ){
			if(fields[i].value == null || fields[i].value == undefined || fields[i].value == ''){
				document.getElementById(fields[i].id+"-msg").innerHTML= fields[i].name +' is required';
				errFlag = 1;
			}else{
				/*if(fields[i].id == 'email'){
					 var re = /^(([^<>()[\]\\.,;:\s@\"]+(\.[^<>()[\]\\.,;:\s@\"]+)*)|(\".+\"))@((\[[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\])|(([a-zA-Z\-0-9]+\.)+[a-zA-Z]{2,}))$/;
					 if(re.test(fields[i].value) == false)
						 document.getElementById(fields[i].id+"-msg").innerHTML= ' Invalid Email ';
					 else
						 document.getElementById(fields[i].id+"-msg").innerHTML= '';
					}else{*/
						document.getElementById(fields[i].id+"-msg").innerHTML= '';
				//}
			}
	} 
	if(errFlag == 0)
		getDataLoad(submitbut);

}
function getDateFormat(){
	 var today = new Date();
   var dd = today.getDate();
   var mm = today.getMonth()+1; //January is 0!
   var yyyy = today.getFullYear();
   today.setMinutes(today.getMinutes() + 30 ); // Adding 30 min to job in past date
   var hh  = today.getHours();
   var mn = today.getMinutes(); 
   if(dd<10){
       dd='0'+dd
   } 
   if(mm<10){
       mm='0'+mm
   } 
	 var today = mm+'/'+dd+'/'+yyyy+' '+hh+':'+mn;
	 return today;
}

</script>
</head>
<body style="background-color: #FFFFFF">
<div style="width:60%;clear:both;margin:auto;">
<div id="main-content" style="width: 1000px;height: 750px;">
<!--<h1 style="text-align:center;">Data Load</h1>  -->
<div id ="Request-content" style="height: 150px;">
<span>Request type:</span>
<select id = "reqtype" class = "mandatory" onChange="loadParamForm()" >
	<option value= "">Select</option>
	<option value="authenticate">Authenticate</option>
  <option value="createReq">Create Job</option>
  <option value="stsReq">Get Status</option>
  <option value="pauseReq">Pause/Resume</option>
  <option value="endReq">Terminate</option>
</select><span id ="reqtype-msg"></span>
<span>Return Type:</span>
<select id= "rettype" class = "mandatory">
  <option value="json">JSON</option>
  <option value="xml">XML</option>
</select><span id ="rettype-msg"></span>
<span>Base Url:*</span>
<input id = "baseurl" class = "mandatory" type="text" name="URL" value =""><span id ="baseurl-msg"></span>
</div>
<div id ="Detail-content" style="height: 350px;">
</div>
<div id ="Response-content" style="height: 250px;">
<textarea id = "api-response" rows="14" cols="100">
JSON Response.
</textarea>
</div>
</div>
</div>
<input type="hidden" id="access_token" value="">
</body>
</html>
