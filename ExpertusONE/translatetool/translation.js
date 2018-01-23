function processKey(obj) {
	var oldData = "";
	var lang = "";
	if ($(obj).parent().attr("olddata") == undefined
			|| $(obj).parent().attr("olddata") == "") {
		$(obj).parent().attr("olddata", $(obj).val());
		$(obj).parent().addClass("modified");
	}
	oldData = $(obj).parent().attr("olddata");
	if ($(obj).parent().hasClass("en"))
		lang = "en";
	else if ($(obj).parent().hasClass("ja"))
		lang = "ja";
	else if ($(obj).parent().hasClass("zh"))
		lang = "zh";
	else if ($(obj).parent().hasClass("it"))
		lang = "it";
	else if ($(obj).parent().hasClass("ko"))
		lang = "ko";
	else if ($(obj).parent().hasClass("de"))
		lang = "de";
	else if ($(obj).parent().hasClass("es"))
		lang = "es";
	else if ($(obj).parent().hasClass("ru"))
		lang = "ru";
	else if ($(obj).parent().hasClass("pt"))
		lang = "pt";
	else if ($(obj).parent().hasClass("fr"))
		lang = "fr";

}
function isValidLabel(str){
	 return !/[~`!#$%\^&*()+=\\[\]\\';,/{}|\\":<>\?]/g.test(str);
	}


function submitChanges() {
	var dataArr = new Array();
	var error = "";
	$(".trans_error").removeClass("trans_error");
	$(".modified").each(function(index) {
		var oldData = $(this).attr("olddata");
		var lang = "";
		if ($(this).hasClass("en"))
			lang = "en";
		else if ($(this).hasClass("ja"))
			lang = "ja";
		else if ($(this).hasClass("zh"))
			lang = "zh";
		else if ($(this).hasClass("it"))
			lang = "it";
		else if ($(this).hasClass("ko"))
			lang = "ko";
		else if ($(this).hasClass("de"))
			lang = "de";
		else if ($(this).hasClass("es"))
			lang = "es";
		else if ($(this).hasClass("ru"))
			lang = "ru";
		else if ($(this).hasClass("pt"))
			lang = "pt";
		else if ($(this).hasClass("fr"))
			lang = "fr";

		var newData = $(this).find("input").val();
		var id = $(this).find("input").attr("id");
		if(!isValidLabel(newData))
			{
			$(this).find("input").addClass("trans_error");
			error = window.parent.Drupal.t("LBL3034")+" "+window.parent.Drupal.t("ERR351");
			return false;

			}
		var row = {
			"olddata" : oldData,
			"newdata" : newData,
			"lang" : lang,
			"id" : id
		};
		dataArr.push(row);

	});
	if(error != "")
		displayh5pexpertusmsg(error);
	else if (dataArr.length == 0)
		displayh5pexpertusmsg(window.parent.Drupal.t("LBL272")+" "+window.parent.Drupal.t("MSG601"));
	else {
		error = "";
		createLoaderNew("loader");
		$(".msg-close-btn").click();
		$
				.ajax({
					type : "POST",
					url : "/?q=ajax/administration/manage/customattribute/updateLabels",
					data : "data=" + JSON.stringify(dataArr),
					success : function(data) {
						
						$("#loader").html("");
						if(data == "success"){
							displayh5pexpertusmsg(window.parent.Drupal.t("LBL272")+" "+window.parent.Drupal.t("MSG601"));
						}
						else
							displayh5pexpertusmsg(data);
						$(".modified").removeClass("modified");
						$("td").attr("olddata", "");
					} 
				});
	}

}

function createLoaderNew(id) {
	//alert("in loader function")
	var height = 100;
	var width = "100%";
	$("#" + id)
			.prepend(
					"<div id='loader' class='loadercontent' style='z-index:10007;height:480px;width:100%;margin-top:-125px;'></div>");
	$("#loader")
			.html(
					'<table border="0" style="width: 100%; height: 100%;"><tr><td width="53%" height="'
							+ height
							+ 'px">&nbsp;</td><td align="center" height="100%" valign="middle"><div class="loaderimg"><span class="ui-accordion-header"></span></div></td><td width="40%" height="'
							+ height + 'px">&nbsp;</td></tr></table>');
}

function displayh5pexpertusmsg(msg) {
	$('.e1_error').html("");
	$('.e1_error').show();
	var data = '<div id="show_expertus_message"><div><div id="message-container" style="visibility: visible;"><div class="messages error" style="min-width:150px;">'
			+ msg
			+ '<div class="msg-close-btn" onclick=\"$(\'.e1_error\').hide();\"></div></div></div><img  style=\"display:none;\"  src=\"/sites/all/themes/core/expertusoneV2/expertusone-internals/images/close.png\" height=\"0\" width=\"0\"></div></div>';
	$(".e1_error").html(data);
}


