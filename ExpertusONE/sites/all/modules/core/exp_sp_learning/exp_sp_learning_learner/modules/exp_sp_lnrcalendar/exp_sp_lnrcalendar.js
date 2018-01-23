(function($) {
	var dWidth='';
	$.widget("ui.mycalendar", {
		_init: function() {
			try{
			var self = this;
			//var xml=decodeURIComponent($("#catalog-admin-cal-xml").val());
			//this.launchMyCalendar(xml);
			}catch(e){
				// to do
			}
		},

		launchMyCalendar : function (){
			try{
				
				if ($("#actualDate").val() != "") {
					var selDate = $("#actualDate").val();
					var dt = new Date(selDate);
				} else {
					var dt = new Date();
					$("#actualDate").val(dt.getFullYear()+'-'+ (dt.getMonth()+1) +'-01');
				}
				var selYear = dt.getFullYear();
				var selMonth = getFullMonth(dt.getMonth()+1);
				$("#catalog-admin-cal").expertusCalendar({
					"parent":"",
					"xmlout": "",
					"calType":"isHomePage",
					"selYear": selYear,
					"selMonth": selMonth,
				});
			}catch(e){
				// to do
				console.log(e);
			}
		},
		
		reloadMyCalendar : function (){
			try{
				$("#catalog-admin-cal").data("mycalendar").launchMyCalendar();
				//console.log("reloadMyCalendar");
			/*var url = EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.constructUrl("ajax/calendar-xml-list");
			EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.createLoader('catalog-admin-style');
			$.ajax({
				type: "POST",
				url: url,
				data: '',
				success: function(result){
					$("#catalog-admin-cal").datepicker('destroy');
					$("#catalog-admin-cal").data("mycalendar").launchMyCalendar(result);
					EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.destroyLoader('catalog-admin-style');
				},
				failure : function(){
					EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.destroyLoader('catalog-admin-style');
				}
		    });*/
			}catch(e){
				// to do
			}
		}
	});
	$.extend(EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget);
})(jQuery);

$(function(){
	try{
	$( "#catalog-admin-cal" ).mycalendar(); 
	}catch(e){
		// to do
	}
});


$(function() {
	try{
$(".page-learning-enrollment-search #page-container #catalog-admin-style .ui-datepicker-calendar td" ).each(function(){
	$(this).click(function(){
		try{
		if ($.browser.msie && $.browser.version == 8 || $.browser.msie && $.browser.version == 9 ) {
		$(this).css('background', 'transparent');
        /*$(this).toggleClass('selected');
        $(this).prev('td').toggleClass('selected-right');
        $(this).parent().prev('tr').find('td:nth-child('+(this.cellIndex+1)+')').toggleClass('selected-bottom');*/
		}
		}catch(e){
			// to do
		}
	});
	});
	}catch(e){
		// to do
	}
});

function getFullMonth(month) {
	return (month < 10) ? '0'+ month: month;
}