(function($) {
$.widget("ui.shoppingcart", {
	_init: function() {
		try{
		var self = this;
		//this.widgetObj = '$("#learner-enrollment-tab-inner").data("shoppingcart")';
		}catch(e){
			// to do
		}
	}
});

$.extend($.ui.shoppingcart.prototype,EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget);

})(jQuery);



$(function() {
	try{
	//$( "#learner-enrollment-tab-inner" ).shoppingcart();
	}catch(e){
		// to do
	}
});