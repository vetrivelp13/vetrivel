(function($) {

$.widget("ui.surveydetailssearch", $.ui.narrowsearch, {
	
	_init: function() {
		try{
	    var initData = new Array();
	    initData['widget_name'] = 'surveydetailssearch';
	    initData['widget_init_id'] = 'surveydetails-admin-search';
	    initData['search_base_path'] = 'administration/survey/surveydetails/search/all/';
	    initData['initial_sort_type'] = 'AZ';
	    initData['initial_sort_type_html_id'] = 'surveydetails-sort-az';
	    initData['filter_sets'] = new Array(
	    		                   {type: "checkbox", code:"surveydetailsstatus"},
	                               {type: "checkbox", code:"surveydetailslang"},
	                               {type: "addltext", code:"surveyquestion", acpath:"administration/survey/surveydetails/survey-question-autocomplete",defaultText:"Type a survey question"}
	                              );
	    initData['text_filter_ac_path'] = 'administration/survey/surveydetails/textfilter-autocomplete';
	    initData['results_export_path'] = 'administration/survey/surveydetails/export/';
	    
	    this.initializeObject(initData);
		}catch(e){
			// To Do
		}
	}	 
}); 

})(jQuery);

$(function() { 
	try{
	$("#surveydetails-admin-search").surveydetailssearch();
	}catch(e){
		// To Do
	}
});