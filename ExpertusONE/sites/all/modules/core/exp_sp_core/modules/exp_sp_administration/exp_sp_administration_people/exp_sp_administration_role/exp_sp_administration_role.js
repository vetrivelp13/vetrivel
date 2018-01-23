function getUsersearch(){
	try{
    	var gptype= $('#search_all_user_type-hidden').val();
    	$('#hiddengptype').val(gptype);
	}catch(e){
		// To Do
	}
    }

function moreRoleHideShow() {
	try{
        $('.more-drop-down').slideToggle();
        $('.more-drop-down li:last').css('border-bottom','0px none');
	}catch(e){
		// To Do
	}
    }