(function($) {	
	jQuery.fn.expertusSearch = function (aWidgetId,aTitle,addOpts,aDefn,aConfig,customOpts){
		return this.each(function(){
			$.fn.expertusSearchRender(this.id,aWidgetId,aTitle,addOpts,aDefn,aConfig,customOpts);
		});
   };   
   jQuery.fn.expertusSearchRender = function (aDivId,aWidgetId,aTitle,addOpts,aDefn,aConfig,customOpts){
	   //Validate the plugin input
	   var err = 'Unable to render search options!';	   
	   
	   if(aDefn == null) {
		   alert(SMARTPORTAL.t(err+' No search params are specified.'));return false;
	   }
	   /*if(aDefn != null && (aDefn.length < 2 || aDefn.length > 3)) {
		   alert(SMARTPORTAL.t(err+' Number of search params should be 2 or 3.'));return false;
	   }*/
	   
	   for(var i=0;i<aDefn.length;i++){
		   if(aDefn[i].title=='' || aDefn[i].autocomplete==''){
			   alert(SMARTPORTAL.t(err+' Search param title and autocomplete type and subtype values are mandatory. Check param ') + '['+(i+1)+']');return false;
		   }
	   }
	   
	   $('#'+aDivId).html($.fn.expertusSearchHtml(aWidgetId,aTitle,addOpts,aDefn,aConfig,customOpts));
	   
	   //set the float left property
	   $('.searchadv').addClass('sp_divflt');
	   $('.searchoptsseparator').addClass('sp_divflt');
   };
   
   
   jQuery.fn.expertusSearchHtml = function (aWidgetId,aTitle,addOpts,aDefn,aConfig,customOpts){
	   //title
	   //var searchHtml = '<div class="sp_lms_area_header">'+aTitle+'</div>';
	   var searchHtml='';
	   if(aTitle!=''){
		   //searchHtml='<div class="front-block-header block-header sp_content_header"><h2>'+aTitle+'</h2><div class="contentGuide" id="contentGuide"></div></div>';
		   searchHtml+='<div class="block-title-left sp_content_header"><div class="block-title-right block-header"><div class="block-title-middle"><h2 style="width: auto;padding:0px 0px 0px 7px;">'+aTitle+'</h2>';
		   searchHtml+='<div class="contentGuide" id="contentGuide"></div></div></div></div>';
		  }
   
	   searchHtml += "<div id='"+aWidgetId+"_newsearchopts'>";
	   searchHtml += "<table id='"+aWidgetId+"_searchtable' class='searchtable' cellpadding='0' cellspacing='0' >";
	   
	   searchHtml += "<tr><td colspan='2'>";
	   searchHtml += "<div id='"+aWidgetId+"_advsearchdisplay'>";
	   if(aConfig.advanced){		
		   searchHtml += "<div id='searchopts' style='float:left;'><span class='advance-search' id='"+aWidgetId+"_advancedsearch'>"+SMARTPORTAL.t('Advanced Search')+"</span></div>";
			// searchHtml += "<div id='"+aWidgetId+"_advancedsearchDiv' class='advance-search' style='display:none;left:5px;width:521px;'>";
		 // searchHtml += "</div>";
		   
	   }
	   if(aConfig.faq){
		   searchHtml += "<div id='searchopts' style='margin-left:15px;float:left;'><span class='advance-search' id='"+aWidgetId+"_faqsearch'>"+SMARTPORTAL.t('Analytical Search')+"</span></div>";
		   //searchHtml += "<td><input style='margin-left:3px;' class='spButtonSpan' id='"+aWidgetId+"_faqsearch' type='submit' value='"+SMARTPORTAL.t('Analytical')+"' />";
		  // searchHtml += "<div id='"+aWidgetId+"_faqsearchDiv' class='advance-search' style='display:none;left:256px;'>";
		  // searchHtml += "</div>";
	   }
	   if(aConfig.CategoryBrowse){
		   searchHtml += "<div id='searchopts' style='margin-left:15px;float:left;'><span class='advance-search' id='"+aWidgetId+"_categorybrowse'>"+SMARTPORTAL.t('Taxonomy Search')+"</span></div>";
	  }
	  if(aConfig.customBtn){
		   var obj=aConfig.customBtnDt;
		   searchHtml += "<div id='searchopts' style='margin-left:15px;float:left;'><span><a class='advance-search' style='cursor:pointer;' id='"+aWidgetId+"_custonbtn' title='"+SMARTPORTAL.t(obj.title)+"'>"+obj.title+"</a></span></div>";
	  }
	   searchHtml += "</div>";
	   searchHtml += "</td>";
	   searchHtml += "</tr>";
	   
	   searchHtml += "<tr>";
	   searchHtml += "<td class='searchtableoptions' style='padding-bottom:2px;'>";
	   if(aDefn.length>1){
		   for(var i=0;i<aDefn.length;i++){
			   searchHtml += "<input class='"+aWidgetId+"_searchparam' name='searchparam' type='radio' "+(i==0?" checked ":"")+" onclick='javascript:$.fn.expertusAutocompleteFulsh(\""+aWidgetId+"_searchtext\");$.fn.expertusAutocomplete(\""+aWidgetId+"_searchtext\",\""+aDefn[i].autocomplete_type+"\",\""+aDefn[i].key+"\",\""+aDefn[i].autocomplete_formatindex+"\",\""+aDefn[i].entityType+"\");' value='"+aDefn[i].key+"' /><span>"+aDefn[i].title+"</span>";		   
		   }	   
	   }else{
		   searchHtml += "<input class='"+aWidgetId+"_searchparam' name='searchparam' type='hidden' onclick='javascript:$.fn.expertusAutocompleteFulsh(\""+aWidgetId+"_searchtext\");$.fn.expertusAutocomplete(\""+aWidgetId+"_searchtext\",\""+aDefn[0].autocomplete_type+"\",\""+aDefn[0].key+"\",\""+aDefn[0].autocomplete_formatindex+"\",\""+aDefn[0].entityType+"\");' value='"+aDefn[0].key+"' /><span>&nbsp;&nbsp;"+aDefn[0].title+"</span>";
	   }
	   searchHtml += "</td>";
	   
	   searchHtml += "<td>";
	   searchHtml += '<div class="sp_action">';
	   if(aConfig.savequery){
		   searchHtml += '<div style="margin-top:-1px;text-align:right;"><img style="cursor:pointer;" src="'+themepath+'/images/save1.jpg" id="'+aWidgetId+'_savequery" title="'+SMARTPORTAL.t("Save query")+'"><img style="margin-left:5px;cursor:pointer;" src="'+themepath+'/images/open1.jpg" id="'+aWidgetId+'_findsavequery" title="'+SMARTPORTAL.t("Retrive saved query")+'"></div>';
		   searchHtml += "<div id='"+aWidgetId+"_savequeryDiv' class='saved-query-div' style='display:none;'></div>";
	   }else{
		   searchHtml += '<div style="margin-top:12px;"></div>';
	   }
	   searchHtml += '<div>';
	   if(addOpts != undefined){
		   if(addOpts.searchtype!='AssociateSearch') {
			   searchHtml += '<span><input class="spButtonSpan" type="button" value="'+addOpts.title+'" onclick="javascript:'+addOpts.script+'"/></span>&nbsp;';
		   }
	   }
	   if(customOpts != undefined && customOpts.length >0){
		   if(customOpts[0].searchtype!='AssociateSearch') {
			   for(var i=0;i<customOpts.length;i++){
				   searchHtml += '<span><input class="spButtonSpan" type="button" value="'+customOpts[i].title+'" onclick="javascript:'+customOpts[i].script+'"/></span>';
			   }
		   }
	   }
	   searchHtml += '</div>';
	   searchHtml += '</div>';	
	   searchHtml += "</td>";
	   
	   searchHtml += "</tr>";
	   searchHtml += "<tr>";
	   searchHtml += "<td style='width:70%' colspan='2'>";
	   searchHtml += "<input id='"+aWidgetId+"_searchtext' class='searchtext' style='width: 350px;' type='text'/>";
	   searchHtml += "<input style='margin-left:3px;' class='spButtonSpan' id='"+aWidgetId+"_simplesearch' type='submit' value='"+(Drupal.t('LBL304')).toUpperCase()+"' /></td></tr>";
	   searchHtml += "<tr><td colspan='2' style='height:10px;'></td></tr>";
	   //searchHtml+='<div style="margin:1px"><button id="'+aWidgetId+'_simplesearch" sp-button="true" image="'+themepath+'/images/add_16x16.png" imagewidth="16">'+SMARTPORTAL.t("Search")+'</button></div>';
	   /*
	   if(aConfig.advanced||aConfig.faq){		   
		   if(aConfig.advanced){
			   if(aConfig.faq){
				   searchHtml += "<div class='searchadv'><a id='"+aWidgetId+"_advancedsearch' href='javascript:void(0);'>"+SMARTPORTAL.t('Advanced Search')+"</a></div>";
				   searchHtml += "<div class='searchoptsseparator'>|</div>";
				   searchHtml += "<div class='searchfaq'><a id='"+aWidgetId+"_faqsearch' href='javascript:void(0);'>"+SMARTPORTAL.t('Frequently Searched')+"</a></div>";			   
			   } else {
				   searchHtml += "<div class='searchadv'><a id='"+aWidgetId+"_advancedsearch' href='javascript:void(0);'>"+SMARTPORTAL.t('Advanced Search')+"</a></div>";				   
			   }
		   } else if(aConfig.faq) {
			   searchHtml += "<div class='searchfaq'><a id='"+aWidgetId+"_faqsearch' href='javascript:void(0);'>"+SMARTPORTAL.t('Frequently Searched')+"</a></div>";			   
		   }		   
	   }*/

	   searchHtml += "</table>";
	   searchHtml += "</div>";
	   if(aConfig.advanced || aConfig.faq){
	   //The below div create a problem in search catalog widget 'Vijay'
	   //searchHtml += "<div style='clear:both;'></div>";
	   }
	   return searchHtml;
   };
   
   jQuery.fn.expertusAutocompleteFulsh = function(divId) {
	   $('#' + divId).flushCache();
	   
   };
   
   jQuery.fn.expertusAutocomplete= function(textId,type,key,formatIndex,entityType){
	   if(formatIndex == undefined || formatIndex == 'undefined' || formatIndex == '' || formatIndex == NaN){
		   $('#'+textId).setOptions({	   			
	   			extraParams :{'type':type,'subtype':key,'returntype':'text','actionkey':'Autocomplete','entityType':entityType}
			});		   
	   } else {		   
		   $('#'+textId).setOptions({
	   			formatItem:function(pmRec){return formatIndex == undefined ? pmRec[0]: pmRec[formatIndex];},
	   			extraParams :{'type':type,'subtype':key,'returntype':'text','actionkey':'Autocomplete','entityType':entityType}
			});
	   }
   };
	
})(jQuery); 

