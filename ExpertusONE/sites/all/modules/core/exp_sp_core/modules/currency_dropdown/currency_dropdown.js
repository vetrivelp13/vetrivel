(function ($) {
Drupal.behaviors.currency_dropdown = {
  attach: function (context, settings) {
    var settings = settings || Drupal.settings;
    if (settings.currency_dropdown) {      
        $.each( function(index, value) {
          $('select.currency-dropdown-select-element option[value="' + index + '"]').attr('title', value);
        });
      var msddSettings = settings.currency_dropdown.jsWidget;
      
      $('select.currency-dropdown-select-element').msDropDown1({
        visibleRows: msddSettings.visibleRows,
        rowHeight: msddSettings.rowHeight,
        animStyle: msddSettings.animStyle,
        mainCSS: msddSettings.mainCSS
      });
    }

    try {    
      /*-- #45391: Safari-New theme-All currencys-UI-currency Selection --*/
      $("#currency-dropdown-form #edit-currency-dropdown-select_arrow").click(function(){
    	  if (navigator.userAgent.indexOf("Chrome")>0 || navigator.userAgent.indexOf("Safari")!=-1){
    		  $('#edit-currency-dropdown-select_child').css('width', '75px');
    	  }
      });
    } catch (e) {
    }

    $('form#currency-dropdown-form').after('<div style="clear:both;"></div>');
  }
};
  
/* Change Currency session */
$('select.currency-dropdown-select-element').change(function() {
    var currency = this.options[this.selectedIndex].value;
   $.ajax({
    	type: "GET",
    	url: "/?q=ajax/currency_dropdown/update/"+currency,        	
    	success: function(data) {
    	    $.cookie("preferredcurrencychange", "1");
    			if(data=='success'){
    				window.location.reload();
    			}
    	}
    	});
  });

})(jQuery);

/* Apply Jscroll panel */
$('#edit-currency-dropdown-select_arrow').live('click', function(){
	setTimeout(function(){
        if( $('#edit-currency-dropdown-select_child').css('display') == 'block'){
        	if($("#edit-currency-dropdown-select_child a").length > 5){
        		$('#edit-currency-dropdown-select_child').css({'height': '137px', 'overflow': 'hidden', 'width': '63px'});
        		$('#page-container .dd #edit-currency-dropdown-select_child a').css({'width':'100%'});
                $('#edit-currency-dropdown-select_child').jScrollPane({showArrows: true});
        	}
          
        }
      },100);
	});

$(document).ready(function() {
	if (navigator.userAgent.indexOf("Chrome")>0 || navigator.userAgent.indexOf("Safari")!=-1){
		$('.region-multi-language').css('float', 'left');
	}
});