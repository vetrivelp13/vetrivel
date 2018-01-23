
var qtipListArr 	= new Array();
var qtipIdListArr	= new Array();

$('document').ready(function(){
try{	
	$(".tooltip_title").mouseover(function(){
		var data 	= eval($(this).attr('data'));
		var dispId 	= $(this).attr('id');
		courseTooltipOnLoad(data.id,dispId);
	});
	/*--- Home page customization block start ---*/
	var mainblkLen 		= $(".region-highlight .block").length;
	// Allocate block margin space
	if (mainblkLen == 3) {
		$(".region-highlight .block:eq(1)").css('margin-right', '9px'); // First  block in second row
		$(".region-highlight .block:eq(2)").css('margin-left', '9px');  // second block in second row
	}
	if (mainblkLen == 4) {
		$(".region-highlight .block:eq(1)").css('margin-right','9px'); // First  block in second row
		$(".region-highlight .block:eq(2)").css({'margin-left':'9px', 'margin-right':'9px'});  // second block in second row
		$(".region-highlight .block:eq(3)").css('margin-left','9px');  // third block in second row
	}
	/*--- Home page customization block end   ---*/
}catch(e){
	// to do
}
});

function log(logStr) {
}

function courseTooltipOnLoad(pId,dispId) {
	try{
	var inQtip 		= $.inArray(pId,qtipListArr);
	var inQtipId 	= $.inArray(dispId,qtipIdListArr);

	if((inQtip == -1) || (inQtipId == -1)) {
		qtipListArr.push(pId);
		qtipIdListArr.push(dispId);
	}

	if((inQtip == -1) || (inQtipId == -1)) {
		QtipFn(dispId);
		$.ajax({
			type: "GET",
		    url : resource.base_host+"/?q=learning/course-shortdetails/"+pId,
		    complete : function(xmlHttpRequest, textStatus) {
			    if((parseInt(xmlHttpRequest.status) != 0) && (parseInt(xmlHttpRequest.status / 100) != 2)) {
			        return;
			    }

			    var responseText = jQuery.parseJSON(xmlHttpRequest.responseText);
			    callQtip(responseText,dispId);
		    }
	   });
	}
	}catch(e){
		// to do
	}
}


function callQtip (respTxt,dispId) {
	try{
	var nodeId 		= respTxt.Id;
	var title 		= respTxt.Title;
	var code 		= respTxt.Code;
	var desc		= respTxt.ShortDescription;
	var DType		= respTxt.DeliveryType;
	var dispContent	= '';
	dispContent = "<div class='tool-tip-container'>";
	dispContent += "<div class='tool-tip-row'><span class='PopupRenderTxt-title'>"+title+"</span></div></div>";
	dispContent += "<div class='tool-tip-row'><div class='tool-tip-column1'><span class='PopupTltHeading'>"+SMARTPORTAL.t("Code")+" :</span></div><div class='tool-tip-column2'><span class='PopupRenderTxt_code'>"+code+"</span></div></div>";
	dispContent += "<div class='tool-tip-row'><div class='tool-tip-column1'><span class='PopupTltHeading'>"+SMARTPORTAL.t("Delivery Model")+" :</span></div><div class='tool-tip-column2'><span class='PopupRenderTxt_DeliveryType'>"+DType+"</span></div></div>";
	dispContent += "<div class='tool-tip-row'><div class='tool-tip-column1'><span class='PopupTltHeading'>"+SMARTPORTAL.t("Description")+" :</span></div><div class='tool-tip-column2'><span class='PopupRenderTxt_desc'>"+desc+"</span></div></div></div>";
	$("#paintDiv"+dispId).html(dispContent);
	}catch(e){
		// to do
	}
};

function QtipFn(dispId) {
	try{
	var dispContent = '<div id="paintDiv'+dispId+'">Loading ... </div>';
	$('#'+dispId).qtip({
		 content: dispContent,
		// url: 'http://idev.expertusone.com/?q=learning/course-shortdetails/342',
	     show:{
			when:{
				event:'mouseover'
			},
			effect:'slide'
		 },
		 hide: {
			when:{
				event:'mouseout'
			},
			effect:'slide'
		},
		position: {
		    corner: {
		       target: 'bottomMiddle',
		       tooltip: 'topMiddle'
		    }
		},
		style: {
			width: 320,
//			height: 150,
			background: '#cde6ac',
			border: {
				radius: 6,
				width: 1,
				color: '#cde6ac'
			},
			color: '#333333'
		}
	});
	}catch(e){
		// to do
	}
}