(function($) {
	try{
	$.fn.cancelPopUp = function(entityId,entityType) {
	try{
	  $('#qtipAttachIdqtip_listvalues_visible_disp_'+entityId+'_'+entityType).closest('.qtip-active').remove();
	}catch(e){
			// to do
	}
	};
	}catch(e){
		// to do
	}
})(jQuery);

function closeListValuesQtipPopUp() {
	try{
	$('.qtip').find('.qtip-button > div').each(function(){
	 try{	
	    var x = $(this).attr('id');
		$(this).closest('.qtip').remove();
	 }catch(e){
			// to do
		}
	});
	}catch(e){
		// to do
	}
}