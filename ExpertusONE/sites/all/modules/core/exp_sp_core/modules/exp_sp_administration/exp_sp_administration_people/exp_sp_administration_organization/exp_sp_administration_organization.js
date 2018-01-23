(function($) {

$.widget("ui.orgsearch", $.ui.narrowsearch, {
	
	_init: function() {
		try{
	    var initData = new Array();
	    initData['widget_name'] = 'orgsearch';
	    initData['widget_init_id'] = 'org-admin-search';
	    initData['search_base_path'] = 'administration/people/organization/search/all/';
	    initData['initial_sort_type'] = 'AZ';
	    initData['initial_sort_type_html_id'] = 'org-sort-az';
	    initData['filter_sets'] = new Array(
	    		                   {type: "checkbox", code:"orgstatus"},
	                               {type: "checkbox", code:"orgtype"}
	                              );
	    initData['text_filter_ac_path'] = 'administration/people/organization/textfilter-autocomplete';
	    initData['results_export_path'] = 'administration/people/organization/export/';
	    initData['results_print_path'] = 'administration/people/organization/print/';
	    
	    this.initializeObject(initData);
		}catch(e){
			// to do
		}
	},

	addItem : function() {	 
		try{
    	return true;
		}catch(e){
			// to do
		}
    },
	
	editItem : function(orgId) {
		try{
    	return true;
		}catch(e){
			// to do
		}
    }
});

})(jQuery);

$(function() {
	try{
	$( "#org-admin-search" ).orgsearch();
	}catch(e){
		// to do
	}
});