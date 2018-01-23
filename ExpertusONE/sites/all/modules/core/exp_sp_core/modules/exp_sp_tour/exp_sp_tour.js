(function($) {
$.widget("ui.exp_sp_tour", {
	getCurrentUrl: function(page) {
	    try{
			var url = window.location.href.split("/?q=");
			return url[1];
		}catch(e){
		// to do
			// console.log(e, e.stack);
	    }
	},
	getSteps: function() {
	    try{
	    	var steps = [];
			if(Drupal.settings.exp_sp_tour !== undefined) {
				steps = JSON.parse(Drupal.settings.exp_sp_tour.tourconfig);
			}
			return steps;
	    }catch(e){
			// to do
			// console.log(e, e.stack);
	    }
	},

	_init: function() {
	    try{
	    			    	
	    	var isTourDivExist = ($('#tour_mylearn').length > 0 ? true : false);
			var steps = this.getSteps();
			// console.log('steps', steps);
			// console.log(this.rearrangeStep(steps));
			//append tour div if not exists and steps for current page is available
			if(!isTourDivExist && ((steps !== undefined && steps.length > 0) || $('[data-intro]').length > 0)) {
				$('<div class = "tour-outer-wrapper"><div id = "tour_mylearn" onclick="$(\'body\').data(\'exp_sp_tour\').startTour();"><p>'+Drupal.t('TOUR')+'</p></div></div>').prependTo('body');	
			}
			if(Drupal.settings.exp_sp_tour.auto_start == 1){
	    		setTimeout(function(){	    			
	    			$("#tour_mylearn").click()}, 500);	    		
	    	}
	    }catch(e){
			// to do
			// console.log(e, e.stack);
	    }
	},

	startTour: function() {
		try {
			var introguide = introJs();
			var steps = this.getSteps();
			var obj = this;
			// console.log('steps', steps);
			var options = {
				showStepNumbers: false,
				overlayOpacity: 0.5,
				exitOnOverlayClick: false,
				nextLabel: Drupal.t('LBL693'),
				prevLabel: Drupal.t('LBL692'),
				skipLabel: Drupal.t('LBL3213'),
				doneLabel: Drupal.t('LBL569'),
				// steps: steps
			};
			if(steps !== undefined) {
				options.steps = obj.rearrangeStep(steps).filter(function(item) {
					// console.log($(item.element).length);
					// console.log(item.welcomeText);
					var displayAlways = ['#paintCriteriaResults', '#refine-filter-pin-icon.pin-icon-white'];
					return (($(item.element).length > 0 && $(item.element).is(':visible')) || item.welcomeText || displayAlways.indexOf(item.element) != -1);
				});
				// options.steps = obj.rearrangeStep(options.steps)
				// console.log(options.steps);
			}
			introguide.setOptions(options)
			.onbeforechange(function (targetElement) {
			try {
				// console.log($(targetElement));
				var refineHidden = $('#paintCriteriaResults').hasClass('searchcriteria-div-unpinned');
				if(refineHidden) {
					if($(targetElement).attr('id') == 'paintCriteriaResults') {
						$('#lnr-catalog-search').data('lnrcatalogsearch').pinUnpinFilterCriteria();
					} else if($(targetElement).is('#refine-filter-pin-icon.pin-icon-white')) {
						$('#lnr-catalog-search').data('lnrcatalogsearch').pinUnpinFilterCriteria();
					}
				} else {
					if($(targetElement).is('#refine-filter-pin-icon.pin-icon-black')) {
						$('#lnr-catalog-search').data('lnrcatalogsearch').pinUnpinFilterCriteria();
						$('.catalog-criteria-refine-icon').trigger('mouseenter');
					}
				}
				//delete exiting mask div if any
				$('.introjs-helperLayer-Mask').remove();
			} catch(e) {
				// console.log(e, e.stack);
			}
            })
			.onafterchange(function(targetElement){
				//add a mask div to restrict actions on target element if any
				$('.introjs-helperLayer').clone().appendTo('body')
					.addClass('introjs-helperLayer-Mask');
				$('.tour-common').css("height","41px");
				$('.tour-filter').css("width","63px");
				$('.tour-filter').css("margin-left","-7px");
				$('.tour-common').css("margin-top","3px");
			})
			.oncomplete(function() {
				$('.introjs-helperLayer-Mask').remove();
			})
			.onexit(function() {
				$('.introjs-helperLayer-Mask').remove();
			})
			.start();
			// console.log(options.steps);
		}catch(e){
			// to do
			// console.log(e, e.stack);
		}
	},
	rearrangeStep: function(steps) {
		if(this.getCurrentUrl() == 'learning/enrollment-search') {
			var elementsArr = ['#block-exp-sp-lnrlearningplan-tab-my-learningplan-customized', '#block-exp-sp-lnrenrollment-tab-my-enrollment-customized', '#block-exp-sp-instructor-desk-tab-instructor-desk-customized'];
			var compare = function (a, b) {
				// console.log(a, b);
				// console.log($.inArray(a.element, elementsArr), $.inArray(b.element, elementsArr));
				if($.inArray(a.element, elementsArr) != -1 && $.inArray(b.element, elementsArr) != -1) {
					var selectorArr = $(a.element+', '+b.element);
					// console.log(selectorArr);
					var aIndex = 0;
					var bIndex = 0;
					$(a.element+', '+b.element).each(function(index, item) {
						// console.log(index, item);
						if($(item).is(a.element)) {
							aIndex = index;
						} else if($(item).is(b.element)) {
							bIndex = index;
						}
					});
					// console.log('aIndex', aIndex, 'bIndex', bIndex);
					if (aIndex < bIndex)
						return -1;
					if (aIndex > bIndex)
						return 1;
				}
				return 0;
			}
			return steps.sort(compare);
		}
		return steps;
	}
});

$.extend($.ui.exp_sp_tour.prototype,EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget,{defaults:{start:true,catStart:true}});

})(jQuery);

$(function() {
	try{
	$('body').exp_sp_tour();
	}catch(e){
		// to do
	}
});