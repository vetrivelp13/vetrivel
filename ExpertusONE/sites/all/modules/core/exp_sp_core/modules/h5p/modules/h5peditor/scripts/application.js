var H5PEditor = H5PEditor || {};
var ns = H5PEditor;
var h5peditor;

(function($) {
  ns.init = function () {
	 
   // $("body").append("<script src='/sites/default/files/h5p/libraries/EmbeddedJS-1.0/js/ejs.js'></script>");
   

    var $upload = $('input[name="files[h5p]"]').parents('.form-item');
    
    var $editor =$('.h5p-editor');
    
    var $create = $('#edit-h5p-editor').hide();
    var $type = $('input[name="h5p_type"]');
    var $params = $("#json_content");//[0].json_content;//.value;//$('input[name="json_content"]');
    var $library = $("#library");//("#exp-sp-administration-contentauthor-presentation-addedit-form")[0].library;//$('input[name="h5p_library"]');
    var library = $library.val();
    var h5pId = "";
    
    if(window.parent.document.getElementById("contentauthor-presentation-addedit-form"))
    {
    	$("#json_content").val($("#json_content").val());
     	//$("#json_content").val(decodeURIComponent($("#json_content").val()));
    	$params = $("#json_content");
    	
    	$("#edit-submit").val("SAVE");
    	$("#edit-delete").val("Close");
    	h5pId =	$("#h5pId").val();
    	h5pId =	$("#h5pEditorId").val();
    }
    else
    {
    	var p = replaceAllStr(decodeURIComponent($("#json_content").val()),"+"," ");
        $("#json_content").val(p);
    	$params = $("#json_content");
    	h5pId = $params.attr("h5pId");
    	$("#edit-delete").hide();
    	$("#edit-submit").val("Done");
    }
    if($("#h5peditorsettings").size() > 0)
    	jQuery.extend(Drupal.settings,JSON.parse($("#h5peditorsettings").val()));
    ns.$ = H5P.jQuery;
    
    ns.basePath = Drupal.settings.basePath +  Drupal.settings.h5peditor.modulePath + '/h5peditor/';
    ns.contentId = h5pId;// Drupal.settings.h5peditor.nodeVersionId;
    ns.fileIcon = Drupal.settings.h5peditor.fileIcon;
    ns.ajaxPath = Drupal.settings.h5peditor.ajaxPath;//"?q=h5peditor\/fa50558432651\/"+h5pId+"\/";
    ns.filesPath = Drupal.settings.h5peditor.filesPath;
    ns.relativeUrl = Drupal.settings.h5peditor.relativeUrl;
    ns.contentRelUrl = Drupal.settings.h5peditor.contentRelUrl;
    ns.editorRelUrl = Drupal.settings.h5peditor.editorRelUrl;
    console.log(Drupal.settings);

    // Semantics describing what copyright information can be stored for media.
    ns.copyrightSemantics = Drupal.settings.h5peditor.copyrightSemantics;

    // Required styles and scripts for the editor
    ns.assets = Drupal.settings.h5peditor.assets;
    // Required for assets
    ns.baseUrl = Drupal.settings.basePath;
    
   // if (h5peditor === undefined) 
     {

		h5peditor = new ns.Editor(library, $params.val(), $editor[0]);
 		$create.show();
 		$upload.hide();
 	//h5pcustomize hide
 	try
 	{
 		$("#edit-h5p-editor").find(':first-child').hide();
 	}catch(e){}
 
 	
 	}
	
	//h5pcustomize hide
   /*$type.change(function () {
      if ($type.filter(':checked').val() === 'upload') {
        $create.hide();
        $upload.show();
      }
      else {
        $upload.hide();
        if (h5peditor === undefined) {
          h5peditor = new ns.Editor(library, $params.val(), $editor[0]);
        }
        $create.show();
      }
    }).change();
	*/

    //$('#h5p-content-node-form').submit(function (obj) {
    $('#saveInteractionsH5P').click(function (obj) {


	    

		var actionType = "";
		if(window.parent.document.getElementById("contentauthor-presentation-addedit-form"))
		{
			actionType = "contentAuthorPresentationInteractions";
		}
		else
		{
			actionType = "contentAuthorAddInteractions";
		
		}
		if($('#iframe_editor_wrapper').attr("esigndisplayed") == "on")
		{
		}
		else
		{
		if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != "")
		{
			var esignObj = {'popupDiv':'addedit-edit-newtheme-save-button','esignFor':actionType};
			window.parent.$.fn.getNewEsignPopup(esignObj);
    		$('#iframe_editor_wrapper').attr("esigndisplayed","on");
            return false;
		}
		}

		
    	//var formAction = $(this).attr("action");
    	
      if (h5peditor !== undefined) {
      
        var params = h5peditor.getParams();
        
        if (params === false) {
          // return false;
          /*
           * TODO: Give good feedback when validation fails. Currently it seems save and delete buttons
           * aren't working, but the user doesn't get any indication of why they aren't working.
           */
        }

        if (params !== undefined) {
          $library.val(h5peditor.getLibrary());
          $params.val(JSON.stringify(params));
        }
      }
      
  //    var title 		= document.forms[0].title.value;
  	  
  	  var json_content_obj = JSON.parse(document.forms[0].json_content.value);
  	  if(window.parent.document.getElementById("contentauthor-presentation-addedit-form"))
  	  {
  	  		var showSolution =  window.parent.$("#edit-show-solution-enabled:checked").val();
			if(showSolution == "undefined" || showSolution == undefined)
				showSolution = false;
			else
				showSolution = true;

			var retry 		 =  window.parent.$("#edit-retry-enabled:checked").val();
			if(retry == "undefined" || retry == undefined)
				retry = false;
			else
				retry = true;
			
			
  	  
  	  	json_content_obj.override.overrideButtons = true;
  	  	json_content_obj.override.overrideShowSolutionButton = showSolution;
  	  	json_content_obj.override.overrideRetry = retry;
  	  }
  	  
      var json_content 	= encodeURIComponent(JSON.stringify(json_content_obj));
      //var json_content 	= document.forms[0].json_content.value;
      
      
		
		
		if(window.parent.document.getElementById("contentauthor-presentation-addedit-form"))
		{
			createLoaderNew("contentauthor-presentation-addedit-form");
			//var title = window.parent.document.getElementById("edit-title").value;
			//var id = window.parent.$("#h5p_presentation_frame").attr("scmid"); //document.getElementById("entityId").value;

			var title 	= document.getElementById("edit-title").value;
			var id 		= $("#h5p_presentation_frame").attr("scmid"); //document.getElementById("entityId").value;

			$.ajax({
				url:'?q=ajax/administration/contentauthor/presentation/saveOrUpdatePresentationH5P',
				method:'post',
				data:'action=saveorupdate&title='+title+'&h5p_id='+ns.contentId+"&json_content="+json_content+"&id="+id,
				success:function(data)
				{
					destroyLoader();
					console.log("Data:"+data);
					var obj = JSON.parse(data);
					console.log("Obj:"+JSON.stringify(obj));
					ShowExpertusMessage1(obj.status);
					//window.parent.$("body").append("<input type='hidden' id='entityId' value="+obj.id+" name='entityId'></input>");
					//window.parent.$("#root-admin").data("narrowsearch").refreshGrid();
					$("#entityId").remove();
					$("body").append("<input type='hidden' id='entityId' value="+obj.id+" name='entityId'></input>");
					$("#root-admin").data("narrowsearch").refreshGrid();
					
				},
				failure:function(err)
				{
					destroyLoader();
					var obj = JSON.parse(err);
					ShowExpertusMessage1(obj.status);
				},
				error:function(err)
				{
					destroyLoader();
					var obj = JSON.parse(err);
					ShowExpertusMessage1(obj.status);
				}
			});
	
		}
		else
		{
		//	alert("first value");
			createLoaderNew("contentauthor-addedit-form");
			$.ajax({
	    		type        : 'POST',
	    		url     	: "?q=ajax/administration/contentauthor/updateh5p",
	    		data        : "json_content="+json_content+"&action=updatecoreh5p&content_id="+ns.contentId,
	    		//processData : false,
	    		//contentType : false,                        
	    		success     : function(r) {
	    			//alert(r);
	   			destroyLoader();
	    			ShowExpertusMessage1(r,'video');
			   	},
	    		error       : function(r) { 
	    			alert('jQuery Error'); 
	    		}
			});
      	}
	
		 
      return false;
    });
    
    //Initializing the variables for dynamically loaded JS files--Content Authoring. 
/*	 var a = Drupal.t('MSG801');
	  var b = Drupal.t('MSG802');
	  var c = Drupal.t('MSG803');
	  var d = Drupal.t('MSG804');
	  var e = Drupal.t('LBL3038');
	  var f = Drupal.t('LBL3027');
	  var g = Drupal.t('LBL3030');
	  var h = Drupal.t('LBL3031');
	  var i = Drupal.t('LBL3034');
	  var j = Drupal.t('LBL3029');  
	  var i = Drupal.t('LBL3032');
	  var i = Drupal.t('LBL611');
	  var i = Drupal.t('MSG801');
	  var i = Drupal.t('LBL3046'); 
	var x = Drupal.t('LBL3039');
	var x = Drupal.t('LBL3044');
	var y = Drupal.t('LBL3040');
	var y = Drupal.t('LBL3041');
	var y = Drupal.t('LBL3042');
	var y = Drupal.t('LBL3043');
	var y = Drupal.t('LBL387'); */
	//var y = Drupal.t('LBL906');
	//var i = 0;
	for(var i=3027; i < 3048 ; i++)
		{
		var temp = "LBL"+i;
		var x = Drupal.t(temp);
		}
	var y=  Drupal.t('LBL814');
	var y = Drupal.t('LBL3047');
	var y = Drupal.t('LBL3033');
	var y = Drupal.t('LBL870');
	var y = Drupal.t('LBL240');
	var b = Drupal.t('MSG802');
	var c = Drupal.t('LBL569');
	var c = Drupal.t('LBL123');
	var c = Drupal.t('Link');
	var c = Drupal.t('Label');
	var c = Drupal.t('Text');
	var c = Drupal.t('Image');
	var c = Drupal.t('Single Choice Set');
	var c = Drupal.t('Fill in the Blanks');
	var c = Drupal.t('Drag Text');
	var c = Drupal.t('Mark the Words');
	var c = Drupal.t('Go To Question');
	var c = Drupal.t('Advanced Text');
	var c = Drupal.t('Audio');
	var c = Drupal.t('Go To Slide');
	var c = Drupal.t('Advanced Text');
	var c = Drupal.t('MSG779');
	var c = Drupal.t('MSG892');
	var c = Drupal.t('MSG893');
	var c = Drupal.t('LBL3032');
	var c = Drupal.t('Add Question Set');
	var c = Drupal.t('Fill-in-the-blank Questions');
	var c = Drupal.t('LBL083');
	var c = Drupal.t('LBL141');
	var c = Drupal.t('LBL614');
	var c = Drupal.t('LBL571');
	var c = Drupal.t('LBL714');
	var c = Drupal.t('Video');
	var c = Drupal.t('MSG782');
	var c = Drupal.t('MSG369');
	var c = Drupal.t('Add Controls');
	var c = Drupal.t('Autoplay');
	var c = Drupal.t('MSG898');
	var c = Drupal.t('MSG899');
	var c = Drupal.t("MSG912");
	var c = Drupal.t('MSG894');
	var c = Drupal.t('MSG895');
	var c = Drupal.t('MSG896');
	var c = Drupal.t('MSG897');
	var c = Drupal.t('MSG900');
	var c = Drupal.t('MSG901');
	var c = Drupal.t('MSG902');
	var c = Drupal.t('ERR260');
	var c = Drupal.t('ERR259');
	var c = Drupal.t('MSG904');
	var c = Drupal.t('MSG903');
	var c = Drupal.t('ERR101');
	var c = Drupal.t('ERR261');
	var c = Drupal.t('ERR262');
	var c = Drupal.t('Clone current slide');
	var c = Drupal.t('Set slide background');
	var c = Drupal.t('Delete current slide');
	var c = Drupal.t('Clone current slide');
	var c = Drupal.t('Add new slide');
	var c = Drupal.t('Reset to default');
	var c = Drupal.t('Slide background');
	var c = Drupal.t('Color fill background');
	var c = Drupal.t('This slide');
	var c = Drupal.t('LBL3091');
	var c = Drupal.t('LBL3092');
	var c = Drupal.t('LBL3093');
	var c = Drupal.t('ERR101');
	var c = Drupal.t('LBL325');
	var c = Drupal.t('LBL3034');
	var c = Drupal.t("LBL3044");
	var c = Drupal.t("Video");
	var c = Drupal.t("Audio");
	var c = Drupal.t("LBL387");
	var c = Drupal.t("LBL611");
	var c = Drupal.t("LBL3097");

	var c = Drupal.t("LBL3094");
	var c = Drupal.t("LBL3095");
	var c = Drupal.t("LBL3096");
	var c = Drupal.t("LBL714");
	var c = Drupal.t("LBL3098");
	var c = Drupal.t("Goto Slide");
	var c = Drupal.t("LBL3099");
	var c = Drupal.t("LBL3200");
	var c = Drupal.t("LBL3201");
	var c = Drupal.t("LBL3202");
	var c = Drupal.t("LBL3203");
	var c = Drupal.t("LBL212");
	var c = Drupal.t("LBL3207");
	var c = Drupal.t("Check");
	var c = Drupal.t("LBL3021");
	var c = Drupal.t("LBL3098");
	var c = Drupal.t("LBL986");
	var c = Drupal.t("LBL3209");
	var c = Drupal.t("LBL3210");
	var c = Drupal.t("LBL3211");
	var c = Drupal.t("MSG905");
	var c = Drupal.t("MSG906");
	
	var c = Drupal.t("slide");
	var c = Drupal.t("LBL3099");
	var c = Drupal.t("LBL692");
	var c = Drupal.t("LBL693");
	
	var c = Drupal.t("LBL3205");
	var c = Drupal.t("Position");
	var c = Drupal.t("LBL1288");
	
	
	var c = Drupal.t("LBL082");
	var c = Drupal.t("LBL063");
	var c = Drupal.t("LBL3206");
	var c = Drupal.t("LBL3204");
	var c = Drupal.t("LBL325");
	var c = Drupal.t("ERR213");
	var c = Drupal.t('MSG357');
	var c = Drupal.t('Content');
	var c = Drupal.t('MSG913');
	var c = Drupal.t('MSG909');
	var c = Drupal.t('MSG914');
	var c = Drupal.t('MSG916');
	var c = Drupal.t('LBL437');
	var c = Drupal.t('MSG917');
	var c = Drupal.t('MSG918');
	var c = Drupal.t('MSG919');
	var c = Drupal.t('MSG920');
	var c = Drupal.t('LBL563');
	var c = Drupal.t('Pick a color');
	var c = Drupal.t('LBL3035');
	var c = Drupal.t('Play');
	//var c = Drupal.t('Pause');
	var c = Drupal.t('Mute');
	var c = Drupal.t('Unmute');
	var c = Drupal.t('Resize');
	var c = Drupal.t('Already Sorted');
	var c =  Drupal.t('LBL613');
  };

  ns.getAjaxUrl = function (action, parameters) {
    var url = Drupal.settings.h5peditor.ajaxPath + action;

    if (parameters !== undefined) {
      for (var key in parameters) {
        url += '/' + parameters[key];
      }
    }

    return url;
  };

  $(document).ready(function(){
  //$("body").append("<div class='h5p-editor'></div>");
   
   
  //	ns.init();
  //checkIframeLoaded();
  //$("body").css("background-color","#fff");
  
  
  
  }
  );
})(H5P.jQuery);


function ShowExpertusMessage1(msg,type)
{
	
	if(type == 'video')
		var className = 'h5p-video-msg';
	else
		var className = 'h5p-pre-msg';
	
	var disp="";
//	disp += '<div style="position:absolute;top:0px;left:350px;">';
	disp += '<div class="'+className+'" >';
     disp += ' <div id="message-container" style="visibility: visible;">';
       disp += '  <div class="messages status">';
         disp += '   <ul>';
           disp += '    <li class=""><span> '+msg+' </span></li>';
          disp += '  </ul>';
          disp += '<div class="msg-close-btn" onclick="$(\'body\').data(\'learningcore\').closeErrorMessage();"></div>';
        disp += ' </div>';
      disp += '</div>';
     disp +='  <img onload="try{$(\'body\').data(\'learningcore\').showHideMultipleLi();}catch(e){}" style="display:none;" src="sites/all/themes/core/expertusoneV2/expertusone-internals/images/close.png" height="0" width="0"> ';
  disp += ' </div>';
  
  window.parent.$("#modal-content").prepend(disp);
   //$("body").prepend(disp);
}

function checkIframeLoaded() {
    // Get a handle to the iframe element
     iframe = document.getElementById('iframe_editor');

    var iframeDoc = iframe.contentDocument || iframe.contentWindow.document;

    // Check if loading is complete
    if (  iframeDoc.readyState  == 'complete' ) {

        //iframe.contentWindow.alert("Hello");

        iframe.contentWindow.onload = function(){
            alert("I am loaded");
        };

     

        
        return;
    } 

    // If we are here, it is not loaded. Set things up so we check   the status again in 100 milliseconds
    window.setTimeout('checkIframeLoaded();', 100);
}


/*
setIntval = window.setInterval(function(){
	
	if(ns.loadedSemantics != null && ns.loadedSemantics["H5P.InteractiveVideo 1.9"] != undefined)
	{
		$("#edit-h5p-editor").find(':first-child').hide();
		window.clearInterval(setIntval);
	}
},1000);
*/

function destroyLoader()
{
	$("#loader").remove();
}
function createLoaderNew(id)
{
		   var height = 480;
		   var width = 300;
			$("#"+id).prepend("<div id='loader' class='loadercontent' style='z-index:1010;height:480px;width:100%'></div>");
			$("#loader").html('<table border="0" style="width: 100%; height: 100%;"><tr><td width="40%" height="'+height+'px">&nbsp;</td><td align="center" height="100%" valign="middle"><div class="loaderimg"><span class="ui-accordion-header">Loading data...</span></div></td><td width="40%" height="'+height+'px">&nbsp;</td></tr></table>');
 }

 
function colorChange(color){
	$('table#edit-short-description-value_tbl .mceIframeContainer').css('border-color', '#'+color);
    $('table#edit-short-description-value_tbl .mceToolbar').css('border-color', '#'+color);
	$('table#edit-short-description-value_tbl .mceLayout').css('border-color', '#'+color); 
}
 
 function saveInteractionsH5PWrapper(obj, ifpublishOrUnpublish)
 {
	 //if any interaction opened, remove this
	 H5P.jQuery(".h5p-editor-iframe")[0].contentWindow.removeFromParentWindowChild();
	//alert("from video esign");	
		var paramForPublish = "";
		if(ifpublishOrUnpublish == undefined || ifpublishOrUnpublish == "null" || ifpublishOrUnpublish == null)
			paramForPublish = "";
		else
		{
			paramForPublish = ifpublishOrUnpublish;
		}
 		var actionType = "";
		if(window.parent.document.getElementById("contentauthor-presentation-addedit-form"))
		{
			actionType = "contentAuthorPresentationInteractions";
		}
		else
		{
			actionType = "contentAuthorAddInteractions";
			
		}
		

	//if($ifpublish != 1)
	//{	
	//alert("not save n publish");
      if (h5peditor !== undefined) {
        var params = h5peditor.getParams();
        if (params === false) {
          // return false;
          /*
           * TODO: Give good feedback when validation fails. Currently it seems save and delete buttons
           * aren't working, but the user doesn't get any indication of why they aren't working.
           */
        }
        if (params !== undefined) {
          //$library.val(h5peditor.getLibrary());
          //$params.val(JSON.stringify(params));
        }
      }
      
  //    var title 		= document.forms[0].title.value;
  	  
  	  var json_content_obj = h5peditor.getParams();//JSON.parse(document.forms[0].json_content.value);
  	  if($("#contentauthor-presentation-addedit-form").size() >0)
  	  {
  	  		var showSolution =  $("#show_solution:checked").val();
			if(showSolution == "undefined" || showSolution == undefined || showSolution == "Disabled")
				showSolution = false;
			else
				showSolution = true;

			var retry 		 =  $("#retry:checked").val();
			if(retry == "undefined" || retry == undefined || retry == "Disabled")
				retry = false;
			else
				retry = true;
  	  	json_content_obj.override.overrideButtons = true;
  	  	json_content_obj.override.overrideShowSolutionButton = showSolution;
  	  	json_content_obj.override.overrideRetry = retry;
  	  	json_content_obj.presentation.passingscore = $("#edit-passingscore").val();
  	  }
  	  

  	$("#edit-passingscore").removeClass("error");
	$("#edit-title").removeClass("error"); 
	colorChange("CCC");
	
	  
  	var passScore = $("#edit-passingscore").val().trim();
  	var description=tinymce.get("edit-short-description-value").getContent();	
 	var title = $("#edit-title").val();
 	var fields = "";
    var fields1 = "";
 	  if(passScore == "" && title == "" && description == "" ){  	  
		$("#edit-passingscore").addClass("error");
 	  	$("#edit-title").addClass("error");
 	  	colorChange("f00");
 	  	/* 	   $('table#edit-short-description-value_tbl .mceIframeContainer').css('border-color', '#f00');
	   $('table#edit-short-description-value_tbl .mceToolbar').css('border-color', '#f00');
	   $('table#edit-short-description-value_tbl .mceLayout').css('border-color', '#f00'); */
 	 // 	fields = "Title and Passing Score are required.";
 	  	fields = Drupal.t('LBL083')+","+Drupal.t('LBL229')+","+Drupal.t('LBL3045')+" "+Drupal.t('ERR169'); 	  
 	  }else{ 
 		 if (title == "" && passScore == "" && description != "" ) {
 			 
 			//fields = "Title and Passing Score are required.";
 			fields = Drupal.t('LBL083')+","+Drupal.t('LBL3045')+" "+Drupal.t('ERR169');
 			$("#edit-title").addClass("error");
 			$("#edit-passingscore").addClass("error");

 			}else if (title == "" && passScore != "" && description == "" ) {

 			//fields = "Title and Description are required.";
 			fields = Drupal.t('LBL083')+","+Drupal.t('LBL229')+" "+Drupal.t('ERR169');
 			$("#edit-title").addClass("error");
 			$("#edit-short-description-value").addClass("error");
 			colorChange("f00");
 			}else if (title != "" && passScore == "" && description == "" ) {

 			//fields = "Passing Score and Description are required.";
 			fields = Drupal.t('LBL3045')+","+Drupal.t('LBL229')+" "+Drupal.t('ERR169');
 			$("#edit-passingscore").addClass("error");
 			colorChange("f00");
 			}else if (title == "" && passScore != "" && description != "" ) {

 			//fields = "Title is required.";
 			fields = Drupal.t('LBL083')+" "+Drupal.t('ERR101');
 			$("#edit-title").addClass("error");

 			}else if (title != "" && passScore == "" && description != "" ) {

 			//fields = "Passing Score is required.";
 			fields = Drupal.t('LBL3045')+" "+Drupal.t('ERR101');
 			$("#edit-passingscore").addClass("error");
 			
 			}else if (title != "" && passScore != "" && description == "" ) {

 			//fields = "Description is required.";
 			fields = Drupal.t('LBL229')+" "+Drupal.t('ERR101');
 			colorChange("f00");
 			}
 		 if(passScore != ""){
 			//passScore = parseInt(passScore,10);
 			
 		  	if(H5P.jQuery.isNumeric(passScore) == false || passScore.indexOf('.') !== -1 || passScore != "" && !(passScore>= 0 && passScore <= 100))
 	 	  	{
 		  		
 	 	  		if(fields == ""){
 	 	  		if(passScore.indexOf('.') != -1){ 	 	  		
 	 	 	  		fields = Drupal.t('LBL3045')+" "+Drupal.t('ERR185'); 	 	  	
 	 	 	  		}else{	
 	 	 	  		//fields = fields +","+"Passing score must be a numerical value between 0 and 100.";
 	 	 	  			fields = Drupal.t('LBL3045')+" "+Drupal.t('LBL3095');
 	 	 	  		} 	 	  			
 	 	  		}else if(passScore.indexOf('.') != -1){ 	 	  		
 	 	  		fields = fields +"</br>"+Drupal.t('LBL3045')+" "+Drupal.t('ERR185'); 	 	  	
 	 	  		}else{	
 	 	  		//fields = fields +","+"Passing score must be a numerical value between 0 and 100.";
 	 	  			fields = fields +"</br>"+Drupal.t('LBL3045')+" "+Drupal.t('LBL3095');
 	 	  		}
 				$("#edit-passingscore").addClass("error");
 	 	  			
 	 	  	} 
 		  	else
 		  		$("#edit-passingscore").val(passScore);
 		  	if(isNaN($("#edit-passingscore").val()))
 		  		pScore=$("#edit-passingscore").val();
 		  	else
 		  	var pScore = parseFloat($("#edit-passingscore").val(),10);
 		  
 		  	
 		  	$("#edit-passingscore").val(pScore);
 		  	
 		 }
 	
 	  }
 	  if(fields != "")
 	  {
 		// var fields = fields.split(",").join("<br />");
 	  	ShowExpertusMessage1(fields);
		if(document.getElementById("contentauthor-presentation-addedit-form"))
 	  		return false;
 	  }
  	  
  	  
  	  //check interactions are there
  	  
 	  if($('#iframe_editor_wrapper').attr("esigndisplayed") == "on")
		{
			//alert(12345678);
			$('#iframe_editor_wrapper').attr("esigndisplayed","off");
		}
		else if($(".h5p-editor-iframe").attr("esigndisplayed") == "on")
		{
			$('.h5p-editor-iframe').attr("esigndisplayed","off");
		}
		else
		{
			
			if(availableFunctionalities != undefined && availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != "" && actionType != "contentAuthorAddInteractions")
			{
			//	alert("2nd esign");
				var esignObj = {'popupDiv':'addedit-edit-newtheme-save-button','esignFor':actionType,'paramForPublish':paramForPublish};
				window.parent.$.fn.getNewEsignPopup(esignObj);
				if(actionType == "contentAuthorAddInteractions")
 				$('#iframe_editor_wrapper').attr("esigndisplayed","on");
 			else if(actionType == "contentAuthorPresentationInteractions")
 				$('.h5p-editor-iframe').attr("esigndisplayed","on");
 			
 			
         	return false;
         	
			}
		}  
  	  
  	  
  	  
  	  
     // var json_content 	= encodeURIComponent(JSON.stringify(json_content_obj));
    var json_content = JSON.stringify(json_content_obj);
		
		if(document.getElementById("contentauthor-presentation-addedit-form"))
		{
		  	  createLoaderNew("contentauthor-presentation-addedit-form");
			var title 	= document.getElementById("edit-title").value;
			var id 		=document.getElementById("entityId").value;// $("#h5p_presentation_frame").attr("scmid"); 
			//presentation new desc
			
			var description=tinymce.get("edit-short-description-value").getContent();
			$.ajax({
				url:'?q=ajax/administration/contentauthor/presentation/saveOrUpdatePresentationH5P',
				type:'POST',
				contentType: "application/x-www-form-urlencoded;charset=UTF8",
				data:'action=saveorupdate&title='+encodeURIComponent(title)+'&h5p_id='+ns.contentId+"&json_content="+encodeURIComponent(json_content)+"&id="+id+"&"+paramForPublish+"&description="+encodeURIComponent(description),
				success:function(data)
				{
					destroyLoader();
					console.log("Data:"+data);
					var obj = JSON.parse(data);
					console.log("Obj:"+JSON.stringify(obj));
					ShowExpertusMessage1(obj.status);
					$("#entityId").remove();
					$(".popups-close .close").attr("entityId",obj.id);
					$(".popups-close .close").attr("actionexecuted","1");
					$("#contentauthor-presentation-addedit-form").append("<input type='hidden' actionexecuted='1' id='entityId' value="+obj.id+" name='entityId'></input>");
					//$("#root-admin").data("narrowsearch").refreshGrid();
					if(paramForPublish != "")
					{
						if(paramForPublish == "pre_status=unpublished_pre")
						{
							PRESENTATION_ACTION_PARAMS.PRESENTATION_SAVE_AND_PUBLISH ="FALSE";
							PRESENTATION_ACTION_PARAMS.PRESENTATION_UNPUBLISH ="TRUE";
						}
						else if(paramForPublish == "pre_status=published_pre")
						{
							PRESENTATION_ACTION_PARAMS.PRESENTATION_SAVE_AND_PUBLISH ="TRUE";
							PRESENTATION_ACTION_PARAMS.PRESENTATION_UNPUBLISH ="FALSE";

						}
						var htm = new EJS({url: '/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_contentauthor/presentation_actions.ejs?ts='+new Date().getTime()}).render(PRESENTATION_ACTION_PARAMS);
						$(".addedit-form-cancel-container-actions").html(htm);
						
					}
					
				},
				failure:function(err)
				{
					destroyLoader();
					var obj = JSON.parse(err);
					ShowExpertusMessage1(obj.status);
				},
				error:function(err)
				{
					destroyLoader();
					var obj = JSON.parse(err);
					ShowExpertusMessage1(obj.status);
				}
			});
	
		}
		else
		{
		
			//createLoaderNew("contentauthor-addedit-form");
			return "json_content="+encodeURIComponent(json_content)+"&action=updatecoreh5p&content_id="+ns.contentId+"&slt_c_id="+slt_c_id;
			var slt_c_id = $("#exp-sp-administration-contentauthor-addedit-form").find(".addedit-left-col").find('[name="mapped_course_id"]').val();
			//alert($("#exp-sp-administration-contentauthor-addedit-form").find(".addedit-left-col").find('[name="mapped_course_id"]').size());
			//alert(slt_c_id);
			$.ajax({
	    		type        : 'POST',
	    		url     	: "?q=ajax/administration/contentauthor/updateh5p",
	    		contentType: "application/x-www-form-urlencoded;charset=UTF8",
	    		data        : "json_content="+encodeURIComponent(json_content)+"&action=updatecoreh5p&content_id="+ns.contentId+"&slt_c_id="+slt_c_id,
	    		//processData : false,
	    		//contentType : false,                        
	    		success     : function(r) {
	    			//alert(r);
	    			destroyLoader();
	    			ShowExpertusMessage1(r,'video');
			   	},
	    		error       : function(r) { 
	    			alert('jQuery Error'); 
	    		}
			});
      	}
/*	 }
	else {
		// alert("save n publish");
		 var id = $("#entityId").val();
		 savePresentationH5P();
		 $("#root-admin").data("narrowsearch").publishAndUnpublishPresentation(id,this);
		
	}  	*/
		
      return false;
     
    
 }
 
 
 
 
 
 
 
 function saveInteractionsH5PWrapper_presentation()
 {
 	var pre_status = '';
 	if( $("#contentAuthorPresentation_save_pub_btn").size() == 0)
 	{	
		pre_status = 'pre_status=published_pre';
	}
	else if($("#contentAuthorPresentation_save_unpub_btn").size() == 0)
	{ 
		pre_status = 'pre_status=unpublished_pre';
	}
	saveInteractionsH5PWrapper(null,pre_status);
 
 }
 
 			
 function saveInteractionsH5PWrapper_presentation_old($form,$form_state)
 {
	alert('old saveInteractionsH5PWrapper_presentation_old');
		
 		var actionType = "";

			actionType = "contentAuthorPresentationInteractions";

		if($('#iframe_editor_wrapper').attr("esigndisplayed") == "on")
		{
			$('#iframe_editor_wrapper').attr("esigndisplayed","off");
		}
		else
		{
			if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != "")
			{
				var esignObj = {'popupDiv':'addedit-edit-newtheme-save-button','esignFor':actionType};
				window.parent.$.fn.getNewEsignPopup(esignObj);
    			$('#iframe_editor_wrapper').attr("esigndisplayed","on");
            	return false;
			}
		}

      if (h5peditor !== undefined) {
        var params = h5peditor.getParams();
        if (params === false) {
          // return false;
          /*
           * TODO: Give good feedback when validation fails. Currently it seems save and delete buttons
           * aren't working, but the user doesn't get any indication of why they aren't working.
           */
        }
        if (params !== undefined) {
          //$library.val(h5peditor.getLibrary());
          //$params.val(JSON.stringify(params));
        }
      }

  	  
  	  var json_content_obj = h5peditor.getParams();//JSON.parse(document.forms[0].json_content.value);
  	  if($("#contentauthor-presentation-addedit-form").size() >0)
  	  {
  	  		var showSolution =  $("#show_solution:checked").val();
			if(showSolution == "undefined" || showSolution == undefined || showSolution == "Disabled")
				showSolution = false;
			else
				showSolution = true;

			var retry 		 =  $("#retry:checked").val();
			if(retry == "undefined" || retry == undefined || retry == "Disabled")
				retry = false;
			else
				retry = true;
  	  	json_content_obj.override.overrideButtons = true;
  	  	json_content_obj.override.overrideShowSolutionButton = showSolution;
  	  	json_content_obj.override.overrideRetry = retry;
  	  	json_content_obj.presentation.passingscore = $("#edit-passingscore").val();
  	  }
  	  

  /*	$("#edit-passingscore").removeClass("error");
	  	$("#edit-title").removeClass("error");  
  	 var passScore = $("#edit-passingscore").val();
 	  var title = $("#edit-title").val();
 	  var fields = "";
 	  var fields1 = "";
 	  if(passScore == "" && title == "")
 	  {
		$("#edit-passingscore").addClass("error");
 	  	$("#edit-title").addClass("error");
 	  //	fields = "Title and Passing Score are required.";
 	  	fields = Drupal.t('LBL083')+","+Drupal.t('LBL3045')+" "+Drupal.t('ERR169');
 	  }
 	  else
 	  { 
 	  	if(title == "" && passScore != "")
 	  	{
 	  		//fields = "Title is required.";
 	  		fields = Drupal.t('LBL083')+" "+Drupal.t('ERR101');
 	  		$("#edit-title").addClass("error");
 	  	}	
 	  	else if(passScore == "" && title != "")
	  		{
 	  			//fields = "Passing Score is required";
 	  		fields = Drupal.t('LBL3045')+" "+Drupal.t('ERR101');
				$("#edit-passingscore").addClass("error");
 	  			
 	  		}
 	  	if(passScore != "" && !(parseInt(passScore,10) > 0 && parseInt(passScore,10) <= 100))
 	  	{
 	  		if(fields == ""){
 	  			//fields = "Passing score must be a numerical value between 0 and 100.";
 	  			fields = Drupal.t('LBL3045')+" "+Drupal.t('LBL3095');
 	  		}
 	  		else{
 	  		//fields = fields +","+"Passing score must be a numerical value between 0 and 100.";	
	  		fields = fields +"</br>"+Drupal.t('LBL3045')+" "+Drupal.t('LBL3095');
 	  		}
			$("#edit-passingscore").addClass("error");
 	  			
 	  	}
 	  }
 	  if(fields != "")
 	  {
 		 var fields = fields.split(",").join("<br />");
 	  	ShowExpertusMessage1(fields);
 	  	return false;
 	  }
  	  */
    	$("#edit-passingscore").removeClass("error");
	  	$("#edit-title").removeClass("error");  
  	 var passScore = $("#edit-passingscore").val();
 	  var title = $("#edit-title").val();
 	  var fields = "";
 	  var fields1 = "";
       
     	  if(passScore == "" && title == "" && description == "" ){  
    		$("#edit-passingscore").addClass("error");
     	  	$("#edit-title").addClass("error");
     	  	colorChange("f00");
    /* 	   $('table#edit-short-description-value_tbl .mceIframeContainer').css('border-color', '#f00');
    	   $('table#edit-short-description-value_tbl .mceToolbar').css('border-color', '#f00');
    	   $('table#edit-short-description-value_tbl .mceLayout').css('border-color', '#f00'); */
     	 // 	fields = "Title and Passing Score are required.";
     	  	fields = Drupal.t('LBL083')+","+Drupal.t('LBL229')+","+Drupal.t('LBL3045')+" "+Drupal.t('ERR169'); 	  
     	  }else{ 
     		 if (title == "" && passScore == "" && description != "" ) {
     			
     			//fields = "Title and Passing Score are required.";
     			fields = Drupal.t('LBL083')+","+Drupal.t('LBL3045')+" "+Drupal.t('ERR169');
     			$("#edit-title").addClass("error");
     			$("#edit-passingscore").addClass("error");

     			}else if (title == "" && passScore != "" && description == "" ) {

     			//fields = "Title and Description are required.";
     			fields = Drupal.t('LBL083')+","+Drupal.t('LBL229')+" "+Drupal.t('ERR169');
     			$("#edit-title").addClass("error");
     			$("#edit-short-description-value").addClass("error");
     			colorChange("f00");
     			}else if (title != "" && passScore == "" && description == "" ) {

     			//fields = "Passing Score and Description are required.";
     			fields = Drupal.t('LBL3045')+","+Drupal.t('LBL229')+" "+Drupal.t('ERR169');
     			$("#edit-passingscore").addClass("error");
     			colorChange("f00");
     			}else if (title == "" && passScore != "" && description != "" ) {

     			//fields = "Title is required.";
     			fields = Drupal.t('LBL083')+" "+Drupal.t('ERR101');
     			$("#edit-title").addClass("error");

     			}else if (title != "" && passScore == "" && description != "" ) {

     			//fields = "Passing Score is required.";
     			fields = Drupal.t('LBL3045')+" "+Drupal.t('ERR101');
     			$("#edit-passingscore").addClass("error");
     			
     			}else if (title != "" && passScore != "" && description == "" ) {

     			//fields = "Description is required.";
     			fields = Drupal.t('LBL229')+" "+Drupal.t('ERR101');
     			colorChange("f00");
     			}
     		 
     	  	if(H5P.jQuery.isNumeric(passScore) == false || passScore != "" && !(parseInt(passScore,10) > 0 && parseInt(passScore,10) <= 100))
     	  	{
     	  		if(fields == ""){
     	  			//fields = "Passing score must be a numerical value between 0 and 100.";
     	  			fields = Drupal.t('LBL3045')+" "+Drupal.t('LBL3095');
     	  		}
     	  		else{
     	  		//fields = fields +","+"Passing score must be a numerical value between 0 and 100.";
     	  			fields = fields +"</br>"+Drupal.t('LBL3045')+" "+Drupal.t('LBL3095');
     	  		}
    			$("#edit-passingscore").addClass("error");
     	  			
     	  	}
     	  }
     	  if(fields != "")
     	  {
     		// var fields = fields.split(",").join("<br />");
     	  	ShowExpertusMessage1(fields);
    		if(document.getElementById("contentauthor-presentation-addedit-form"))
     	  		return false;
     	  }
  	  
     // var json_content 	= encodeURIComponent(JSON.stringify(json_content_obj));
    var json_content = JSON.stringify(json_content_obj);
		
		if(document.getElementById("contentauthor-presentation-addedit-form"))
		{
		  	  createLoaderNew("contentauthor-presentation-addedit-form");
		
			var title 	= document.getElementById("edit-title").value;
			var id 		=document.getElementById("entityId").value;// $("#h5p_presentation_frame").attr("scmid"); 
			var description=tinymce.get("edit-short-description-value").getContent();
			if( $("#contentAuthorPresentation_save_pub_btn").size() == 0){	
//				alert("Content is in published state");
				var pre_status = 'published_pre';
			}
			else if($("#contentAuthorPresentation_save_unpub_btn").size() == 0){ //if($("#PRESENTATION_SAVE_AND_PUBLISH").val() == "TRUE"){
				//alert("Content is in unpublished state");
				var pre_status = 'unpublished_pre';
			}
			$.ajax({
				url:'?q=ajax/administration/contentauthor/presentation/saveOrUpdatePresentationH5P',
				type:'POST',
				data:'action=saveorupdate&title='+encodeURIComponent(title)+'&h5p_id='+ns.contentId+"&json_content="+encodeURIComponent(json_content)+"&id="+id+'&pre_status='+pre_status+"&description="+encodeURIComponent(description),
				success:function(data)
				{
					destroyLoader();
					console.log("Data:"+data);
					var obj = JSON.parse(data);
					console.log("Obj:"+JSON.stringify(obj));
					ShowExpertusMessage1(obj.status);
					$("#entityId").remove();
					$("body").append("<input type='hidden' id='entityId' value="+obj.id+" name='entityId'></input>");
					//$("#root-admin").data("narrowsearch").refreshGrid();
					
				},
				failure:function(err)
				{
					destroyLoader();
					var obj = JSON.parse(err);
					ShowExpertusMessage1(obj.status);
				},
				error:function(err)
				{
					destroyLoader();
					var obj = JSON.parse(err);
					ShowExpertusMessage1(obj.status);
				}
			});
	
		}
		
      return false;
     
    
 }

 function replaceAllStr(str, find, replace) {
		str = str.split(find).join(replace);
		return str;
		/*console.log("str:"+str);
		if(str == null || str == "")
			return "";
		
		str = str.split(find).join(replace);

		var i = str.indexOf(find);
	  	if (i > -1){
	    	str = str.replace(find, replace); 
	    	i = i + str.length;
	    	var st2 = str.substring(i);
	    	if(st2.indexOf(find) > -1){
	      		str = str.substring(0,i) + replaceAllStr(st2, find, replace);
	    	}       
	  	}
	  //alert(str);
	  return str; */
	}





	function ShowExpertusMessage(msg){
		var disp="";
		disp += '<div>';
	     disp += ' <div id="message-container" style="visibility: visible;">';
	       disp += '  <div class="messages status">';
	         disp += '   <ul>';
	           disp += '    <li class=""><span> '+msg+' </span></li>';
	          disp += '  </ul>';
	          disp += '<div class="msg-close-btn" onclick="$(\'body\').data(\'learningcore\').closeErrorMessage();"></div>';
	        disp += ' </div>';
	      disp += '</div>';
	     disp +='  <img onload="try{$(\'body\').data(\'learningcore\').showHideMultipleLi();}catch(e){}" style="display:none;" src="sites/all/themes/core/expertusoneV2/expertusone-internals/images/close.png" height="0" width="0"> ';
	  disp += ' </div>';
	   
	   $("#show_expertus_message").html(disp);
	}


	function summaryMandatoryFields()
	{
		$(".summarymandatoryfields").show();
	}



	function savePresentationH5P()
	{
		//var json_content = $("#h5p_presentation_frame").contents().find("input[name='json_content']").val();
		
		var json_content = JSON.stringify(h5peditor.getParams());
		
		
		
		 var json_content_obj = JSON.parse(json_content);
	  	 var showSolution =  window.parent.$("#edit-show-solution-enabled:checked").val();
		 if(showSolution == "undefined" || showSolution == undefined)
			showSolution = false;
		else
			showSolution = true;

		var retry 		 =  window.parent.$("#edit-retry-enabled:checked").val();
		if(retry == "undefined" || retry == undefined)
			retry = false;
		else
			retry = true;
	  	json_content_obj.override.overrideButtons = true;
	  	json_content_obj.override.overrideShowSolutionButton = showSolution;
	  	json_content_obj.override.overrideRetry = retry;
	  	  
	  	  
	      var json_content 	= encodeURIComponent(JSON.stringify(json_content_obj));
	      
	      
		
		var title = $("#edit-title").val();
		var h5pId = $("#h5pId").val();
		var description=tinymce.get("edit-short-description-value").getContent();
		var id = $("#entityId").val();
		EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.createLoader("exp-sp-administration-contentauthor-presentation-addedit-form");
		$.ajax({
		url:'?q=ajax/administration/contentauthor/presentation/saveOrUpdatePresentationH5P',
		method:'POST',
		type:'POST',
		data:'action=saveorupdate&title='+encodeURIComponent(title)+'&h5p_id='+h5pId+"&json_content="+json_content+"&id="+id+"&description="+encodeURIComponent(description),
		success:function(data)
		{
			EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.destroyLoader("exp-sp-administration-contentauthor-presentation-addedit-form");
			var obj = JSON.parse(data);
			$("body").append("<input type='hidden' id='entityId' value="+obj.id+" name='entityId'></input>");
		
			ShowExpertusMessage(obj.status);
		},
		failure:function(err)
		{
			var obj = JSON.parse(err);
			EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.destroyLoader("exp-sp-administration-contentauthor-addedit-form");
			ShowExpertusMessage(obj.status);
		},
		error:function(err)
		{
			var obj = JSON.parse(err);
			EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.destroyLoader("exp-sp-administration-contentauthor-addedit-form");
			ShowExpertusMessage(obj.status);
		}
		});
			
	}

	function closeInteractionPopup(obj)
	{ 
		Drupal.CTools.Modal.dismiss();
		$('#root-admin').data('narrowsearch').refreshGrid();
		//window.location.href= "?q=administration/contentauthor/video";
	}

	function showHelpIconsForContentAuthor()
	{
		var helpicon ="<img src='/sites/all/themes/core/expertusoneV2/expertusone-internals/images/help.png' class='vtip info-enr-upload' title='Option \"Enabled\"  will allow user to view answers to the question, if wrongly answered.'>";
		var helpicon1 ="<img src='/sites/all/themes/core/expertusoneV2/expertusone-internals/images/help.png' class='vtip info-enr-upload' title='Option \"Enabled\" will allow user to re-attempt a question, if wrongly answered.'>";
		$("#two-col-row-showsol_retry .addedit-twocol-firstcol .addedit-new-field-title").append(helpicon);
		$("#two-col-row-showsol_retry .addedit-twocol-secondcol .addedit-new-field-title").append(helpicon1);

	}



	function createLoaderNew(id)
	{
		//alert("in loader function")
			   var height = 480;
			   var width = 300;
				$("#"+id).prepend("<div id='loader' class='loadercontent' style='z-index:10007;height:480px;width:100%;margin-top:-125px;'></div>");
				$("#loader").html('<table border="0" style="width: 100%; height: 100%;"><tr><td width="40%" height="'+height+'px">&nbsp;</td><td align="center" height="100%" valign="middle"><div class="loaderimg"><span class="ui-accordion-header"></span></div></td><td width="40%" height="'+height+'px">&nbsp;</td></tr></table>');
	 }

 
 
 
