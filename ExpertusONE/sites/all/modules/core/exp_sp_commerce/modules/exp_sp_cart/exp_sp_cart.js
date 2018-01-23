(function($) {

	$.widget("ui.Cart", {
		_init: function() {
			try{
				var self = this;
			}catch(e){
				// to do
			}
		},
		CartMessagePopup : function(){
			try{
				setTimeout(function() { // After 5 Sec Close the Popup
					$("#uc-cart-checkout-loader-wrapper").remove();
					$('#uc-cart-checkout-form').removeClass('cart-checkout-step-two');
					 $(".limit-title").trunk8(trunk8.shoppingCart_title);
				 }, 5000);
				
			}catch(e){
				// to do
			}
		}
	});
	$.extend($.ui.Cart.prototype, EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget);
})(jQuery);

$(function() {
	try{
		$("body").Cart();
		$("body").data('Cart').CartMessagePopup();
	}catch(e){
		// To Do
	}
});

(function($) {
	$(document).ready( function() {	
		$("#step-three #uc-cart-checkout-loader-wrapper").remove();
		$('#uc-cart-checkout-form').removeClass('cart-checkout-step-three');
		// $(".limit-title").trunk8(trunk8.shoppingCart_title);
	});
})(jQuery);
