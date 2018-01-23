var checkoutInterval;
function checkoutTimeCount(to_order) {
	var cookieVar = (to_order != undefined && to_order == 1) ? 'admin_cart_elapsed_time'  : 'cart_elapsed_time';
	var sec = readCookie(cookieVar);
	var alertMsg = true;
	if($("#checkoutTimeRemin1") != undefined){ 
		clearInterval(checkoutInterval);			
	}

	if(($("#checkoutTimeRemin1") != undefined)) {
		checkoutInterval = setInterval(function() {
			if (sec > 0) {	
				var showsec = sec % 60;
				var showmin = Math.floor(sec/60); 
				 remainTime = "<font color='red'>"+(((showmin<10) ? ("0"+showmin) : showmin)+" : "+((showsec<10) ? ("0"+showsec) : showsec))+"</font>";
				 //remainTime = "<font color='red'>"+(((min%2) == 0) ? ""+min+"" : min)+" : "+(((sec%2) == 0) ? ""+sec+"" : sec)+"</font>";				
				$("#checkoutTimeRemin1").html(remainTime);
				sec--; 
				if(readCookie(cookieVar)!= 'checkout'){
					createCookie(cookieVar,sec);
				}
				else{
					alertMsg = false;
					createCookie(cookieVar,sec);
				}
			 }	
			else{
				if(alertMsg && $("#checkoutTimeRemin1").html()){
					clearInterval(checkoutInterval);
					$('#cart_chechout_timer').html('');
					alert(Drupal.t('MSG564'));
					//$("#checkoutTimeRemin1").html("<br><br><font color='red'><b><center>Sorry! Your checkout time has expired. Please Checkout again.</center></b></font><br><br>");
					eraseCookie(cookieVar);
					if(to_order==1)
						window.location = resource.base_url+'/?q=administration/commerce/order';
					else
						window.location = resource.base_url;
				}				
			}
		}, 1000);		
	}	 
}

function removeTimercheckout(){
	clearInterval(checkoutInterval);
	$('#cart_chechout_timer').html('');
}

function readCookie(name) {
	var nameEQ = name + "=";
	var ca = document.cookie.split(';');
	for(var i=0;i < ca.length;i++) {
		var c = ca[i];
		while (c.charAt(0)==' ') c = c.substring(1,c.length);
		if (c.indexOf(nameEQ) == 0) return c.substring(nameEQ.length,c.length);
	}
	return null;
}

function createCookie(name,value,days) {
	if (days) {
		var date = new Date();
		date.setTime(date.getTime()+(days*24*60*60*1000));
		var expires = "; expires="+date.toGMTString();
	}
	else var expires = "";
	document.cookie = name+"="+value+expires+"; path=/";
}

function eraseCookie(name) {
	createCookie(name,"",-1);
}