//Added for Custom Attribute #custom_attribute_0078975 
function displayCustomAttributeDisableMessage(msg,option,id){ 
	try{
		 
	 
	var tmp_msg='<div id="disable-msg-container"><div id="show_expertus_message"><div><div id="message-container" style="visibility: visible;"><div class="messages error"><ul><li class=""><span>'+msg+
				  '</span></li></ul><div class="msg-close-btn" onclick="$(\'body\').data(\'learningcore\').closeErrorMessage();"></div></div></div><img style="display:none;" src="sites/all/themes/core/expertusoneV2/expertusone-internals/images/close.png" width="0" height="0"></div></div></div>';
            
              
             var ctool_popup=$("#modalContent #ctools-modal-content #ctools-face-table .popups-container-newui .block-title-left").length; 
             if(ctool_popup==1){
               $("#modalContent #ctools-modal-content #ctools-face-table .popups-container-newui .block-title-left #disable-msg-container").remove(); 
               $("#modalContent #ctools-modal-content #ctools-face-table .popups-container-newui .block-title-left").append(tmp_msg);	
             }
             
             var qtip_popup=$(".narrow-search-results-item-details #bubble-face-table .bubble-c #paintContentVisiblePopup").length;
             if(qtip_popup==1){
               $(".narrow-search-results-item-details #bubble-face-table .bubble-c #paintContentVisiblePopup #disable-msg-container").remove(); 
               $(".narrow-search-results-item-details #bubble-face-table .bubble-c #paintContentVisiblePopup").append(tmp_msg);	
             }
             
             
             if(option=='screen'){
	             	 $('#view_opt_' + $(id).attr('value')).parent().removeClass('checkbox-selected');
				$('#view_opt_' + $(id).attr('value')).parent().addClass('checkbox-unselected'); 
				$('#view_opt_' + $(id).attr('value')).prop( "checked", false );
				$('#edit_opt_' + $(id).attr('value')).parent().removeClass('checkbox-selected');
				$('#edit_opt_' + $(id).attr('value')).parent().addClass('checkbox-unselected'); 
				$('#edit_opt_' + $(id).attr('value')).prop( "checked", false );
             } 
             
             if(option=='api'){
	            $('#edit_opt_' + $(id).attr('value')).parent().removeClass('checkbox-selected');
				$('#edit_opt_' + $(id).attr('value')).parent().addClass('checkbox-unselected'); 
				$('#edit_opt_' + $(id).attr('value')).prop( "checked", false );
             }
             
			
 
	}catch(e){
			// To Do
		}
}
 
//Dynamic translation
function launchTranslationsPopup(langCodes,mode)
{
	
	var err =  Drupal.t("LBL1273"); //use once for caching
	    err = Drupal.t("LBL3210"); //use once for caching
	    err = Drupal.t("LBL3032");
	    err = Drupal.t("MSG403");
	    err = Drupal.t("LBL3034");
	    err = Drupal.t("ERR351");
	    err = Drupal.t("LBL356");
	    err = Drupal.t("LBL141");
	    err = Drupal.t("LBL123");
	    
	    err = Drupal.t("LBL272");
	    err = Drupal.t("MSG601");
	    
	    
	    
	    
	var url = "/translatetool/listlabels.php?langCodes="+escape(langCodes);
	var iframe = "<iframe id='iframedata' src=\""+url+"\" mode='"+mode+"' style=' width:100%; height:100%; border:none; margin:0; padding:0; overflow:hidden;' width='100%' height='100%'></iframe>";
	//display_translation_popup(iframe,"cre_ste_mod_aut", "exp_sp_administration_contentauthor",1000,520);
	$("#root-admin").data("narrowsearch").displayH5PPreview(iframe,"", "customattr",1000,200);
	window.setTimeout(function(){
		$("#loader").remove();
	},600);
    $(".ui-dialog-title").html(Drupal.t("Translations"));
    $(".ui-dialog-titlebar").append('<label style="color: #fff;text-align: right;padding-right: 10px;padding-top:2px;font-size: 12px;">'+Drupal.t("LBL304")+'&nbsp;<input type="text" id="search_txtbox_trans" value="'+Drupal.t("LBL304")+'" class="search_txtbox_trans" onfocus="clearDefaultValueForTrSearch();" onblur="placeDefaultValueForTrSearch();" onkeyup="doTranslationFilter(this);" ></label>');
    //$(".ui-dialog-titlebar").append("<div style='text-align:right;padding-right:20px;'><label style='color:#fff;'>Search:</label> <input type='text' size='10'></input></div>");
}

function createLoaderNew(id)
{
	//alert("in loader function")
		   var height = 480;
		   var width = 300;
			$("#"+id).prepend("<div id='loader' class='loadercontent' style='z-index:10007;height:480px;width:100%;margin-top:-125px;'></div>");
			$("#loader").html('<table border="0" style="width: 100%; height: 100%;"><tr><td width="40%" height="'+height+'px">&nbsp;</td><td align="center" height="100%" valign="middle"><div class="loaderimg"><span class="ui-accordion-header"></span></div></td><td width="40%" height="'+height+'px">&nbsp;</td></tr></table>');
 }

function doTranslationFilter(obj)
{
	try
	{
	document.getElementById('iframedata').contentWindow['oTable'].fnFilter($(obj).val()).fnDraw();
	}catch(e){
		
	}
}

function clearDefaultValueForTrSearch()
{
	console.log(":::"+$("#search_txtbox_trans").val() +"====="+ Drupal.t("LBL304"));
	if($("#search_txtbox_trans").val() == Drupal.t("LBL304"))
	{
		$("#search_txtbox_trans").val("");
	}
}


function placeDefaultValueForTrSearch()
{
	console.log(":::"+$("#search_txtbox_trans").val() +"====="+ Drupal.t("LBL304"));
	if($("#search_txtbox_trans").val() == ""){ 
		$("#search_txtbox_trans").val(Drupal.t("LBL304"));
	}
}



