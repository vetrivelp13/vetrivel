(function ($) {
Drupal.behaviors.currency_dropdown = {
		  attach: function (context, settings) {
		    var settings = settings || Drupal.settings;
		    if (settings.currency_dropdown) {
		        $.each( function(index, value) {
		          $('select.admin-currency-dropdown-select-element option[value="' + index + '"]').attr('title', value);
		        });
		      var msddSettings = settings.currency_dropdown.jsWidget;

		      $('select.admin-currency-dropdown-select-element').msDropDown1({
		        visibleRows: msddSettings.visibleRows,
		        rowHeight: msddSettings.rowHeight,
		        animStyle: msddSettings.animStyle,
		        mainCSS: msddSettings.mainCSS
		      });
		    }

		    try {
		      /*-- #45391: Safari-New theme-All currencys-UI-currency Selection --*/
		      $("#admin-currency-dropdown-form #edit-admin-currency-dropdown-select_arrow").click(function(){
		    	  if (navigator.userAgent.indexOf("Chrome")>0 || navigator.userAgent.indexOf("Safari")!=-1){
		    		  $('#edit-admin-currency-dropdown-select_child').css('width', '75px');
		    	  }
		      });
		    } catch (e) {
		    }

		    $('form#admin-currency-dropdown-form').after('<div style="clear:both;"></div>');
		  }
		};
/* Change admin Currency session */
$('select#edit-admin-currency-dropdown-select').live('change',function() {
	var obj = $("body").data("mulitselectdatagrid");
	obj.createLoader('admin_product_addorder_container');
    var currency = this.options[this.selectedIndex].value;
   $.ajax({
    	type: "GET",
    	url: "/?q=ajax/admin_currency_dropdown/update/"+currency,
    	success: function(data) {
    			if(data=='success'){
    				$("#datagrid-container-add-product-admin-order").trigger("reloadGrid");
    			}
    	}
    	});
  });
})(jQuery);

/* Apply Jscroll panel */
$('#edit-admin-currency-dropdown-select_arrow').live('click', function(){
	setTimeout(function(){
        if( $('#edit-admin-currency-dropdown-select_child').css('display') == 'block'){
        	if($("#edit-admin-currency-dropdown-select_child a").length > 5){
          $('#edit-admin-currency-dropdown-select_child').css({'height': '137px', 'overflow': 'hidden'});
          $('.form-item-admin-currency-dropdown-select .dd #edit-admin-currency-dropdown-select_child a').css({'width':'100%'});
          $('#edit-admin-currency-dropdown-select_child').jScrollPane({showArrows: true});
        	}
        }
      },100);
	});
