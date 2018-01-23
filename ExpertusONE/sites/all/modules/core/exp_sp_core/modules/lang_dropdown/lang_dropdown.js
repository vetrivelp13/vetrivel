(function ($) {
	Drupal.behaviors.lang_dropdown = {
		attach: function (context, settings) {
			var settings = settings || Drupal.settings;
			
			if (settings.lang_dropdown) {
				var flags = settings.lang_dropdown.jsWidget.languageicons;
				if (flags) {
					$.each(flags, function(index, value) {
						$('select.lang-dropdown-select-element option[value="' + index + '"]').attr('title', value);
					});
				}
				var msddSettings = settings.lang_dropdown.jsWidget;
				$('select.lang-dropdown-select-element').msDropDown({
					visibleRows: msddSettings.visibleRows,
					rowHeight: msddSettings.rowHeight,
					animStyle: msddSettings.animStyle,
					mainCSS: msddSettings.mainCSS,
				});
				$('#edit-lang-dropdown-select_child').css('width', '78px');
				var oHandler = $('select.lang-dropdown-select-element').data("dd");
				if(oHandler !== undefined) {
					oHandler.addMyEvent('onOpen', function(){
						if($('#edit-lang-dropdown-select_child').data('jsp') === undefined) {
							//initialize jscrollPane for the first time
							$('#edit-lang-dropdown-select_child').jScrollPane({showArrows: true});
						}
					});
				}
			}
			
			try {
				$('select.lang-dropdown-select-element').change(function() {
					var lang = this.options[this.selectedIndex].value;
					var href = $(this).parents('form').find('input[name="' + lang + '"]').val().replace("&amp;language", "&language");
					window.location.href = href;
				});
			} catch (e) {
			}
			
			$('form#lang-dropdown-form').after('<div style="clear:both;"></div>');
    $('form#lang-dropdown-form').find("#edit-lang-dropdown-select_arrow").addClass("");
		}
	};
})(jQuery);