function checkSelectedUnselectedCommon(id,entityId,entityType){
	try{
	if($(id).attr('type')!='radio'){
		if($(id).is(':checked')){
			$(id).parent().removeClass('checkbox-unselected');
			$(id).parent().addClass('checkbox-selected');
		}
		else {
			$(id).parent().removeClass('checkbox-selected');
			$(id).parent().addClass('checkbox-unselected');
		}
	}
	var txtInput = $(id).attr('name').split('-');
	if(txtInput[0].indexOf('attach_attributes[]') != -1) {
		var bool=true;
		$("input[name='"+id.name+"']").each(function(){
			if(!($(this).is(":checked")))
			{
				bool=false; 
			}
		}); 		
		if(bool){
				$("#shr_select").parent().removeClass('checkbox-unselected');
				$("#shr_select").parent().addClass('checkbox-selected'); 
		}
		else
		{
			$("#shr_select").parent().removeClass('checkbox-selected');
			$("#shr_select").parent().addClass('checkbox-unselected'); 
		}	
	} 	
	}catch(e){
		// to do
	}
}
function sharecheckedall(){
	try{		
		$(document).ready(function(){
		    $("#shr_select").change(function(){
		    	
		    	$(".attach-group-list").attr('checked', $(this).attr("checked"));
		    	if($('#shr_select').attr('checked')) {
		    		$('.shr-muliselect').removeClass('checkbox-unselected').addClass('checkbox-selected');
		    		$('.multichk').removeClass('checkbox-unselected').addClass('checkbox-selected');
		    	} else {
		    		$('.shr-muliselect').removeClass('checkbox-selected').addClass('checkbox-unselected');
		    		$('.multichk').removeClass('checkbox-selected').addClass('checkbox-unselected');
		    	}
		    
		    });
		   
	    });	
	}		
	catch(e){
			
	}
} 