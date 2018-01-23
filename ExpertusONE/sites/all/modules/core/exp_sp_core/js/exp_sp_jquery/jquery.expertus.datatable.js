(function($) {
	jQuery.fn.expertusDataTable = function (aDefn,aConfig){
		var config = jQuery.extend({
			"paging" : true,
			"exportTbl" : true,
			"print" : true,
			"refresh": true,
			"loadonce" :true,
			"imagePath" : themepath+'/images/',
			"pageLength":10
		}, aConfig);
		return this.each(function(){
			$.fn.expertusDataTableRender(this.id, config, aDefn);
		});
	};


	jQuery.fn.expertusDataTableRender = function (tblId, tblConfig, tblDefn){
		var cols = new Array();
		for(var i=0;i<tblDefn.colNames.length;i++){
			cols[i] = tblDefn.colNames[i].title;
		}
		var xmlObj=tblDefn.dataobj;
		var itemLoop = "Items>Item";
		if(tblDefn.xmlReader != undefined){
			itemLoop=tblDefn.xmlReader.root+">"+tblDefn.xmlReader.row;
		}
		var totalCount=$(itemLoop,xmlObj).size();
		var gviewId = 'gview_'+tblId;
		$('#'+tblId).jqGrid({
			datatype: tblDefn.datatype,
			datastr : tblDefn.datastr,
			xmlReader : tblDefn.xmlReader,
			colNames:cols,
			colModel:tblDefn.colModel,
			height:'100%',
			width:tblDefn.width,
			loadonce:true,
			shrinkToFit:true,
			viewrecords:true,
			pagination: {startIndex:0,maxRecordsPerPage:tblConfig.pageLength,currentPage:1,totalPages:0,totalRecords:totalCount},
			loadComplete:function(){$.fn.expertusDataTableCSActions(tblConfig, tblDefn, tblId);}
		}) ;

	};

	jQuery.fn.expertusDataTableCSActions = function (tblOptions, colOptions, tblId){
		var xmlObj=colOptions.dataobj;
		var parentObjName=colOptions.parentWidgetObj;
		var totalCount=$('#'+tblId).getGridParam('pagination').totalRecords;
		$('#'+tblId).getGridParam('pagination').totalPages =
			(totalCount==0)?1:Math.ceil(totalCount/($('#'+tblId).getGridParam('pagination').maxRecordsPerPage));
		$('#'+tblId).getGridParam('pagination').currentPage =
			(($('#'+tblId).getGridParam('pagination').startIndex)==0) ? 1 :
				(($('#'+tblId).getGridParam('pagination').startIndex)/($('#'+tblId).getGridParam('pagination').maxRecordsPerPage))+1;

		//$('.datatable-top-actions').remove();

		// check if there are records
		var norecords=false;
		var itemLoop = "Items>Item";
		if(colOptions.xmlReader != undefined){
			itemLoop=colOptions.xmlReader.root+">"+colOptions.xmlReader.row;
		}
		var sizeCnt=$(itemLoop,xmlObj).size();
		var nodeName='';
		if(sizeCnt == 1){
			nodeName =($(itemLoop,xmlObj).children().get(0).nodeName);
		}
		norecords=(sizeCnt==0)||(sizeCnt == 1 && nodeName == 'Actions');


		var opsleft = '';
		if(norecords){
			// insert one row with a message
			var noRecs="<tr><td><div class='sp_noRecords'>"+SMARTPORTAL.t("No Results Found.")+"</div></td></tr>";

			// remove all children of the tbody
			$('#'+tblId).find('tbody').children('tr').each(function(){
				$(this).remove();
			});
			//var finalnoRecs = "<tr><td>"+noRecs+"</td></tr>";
			$('#'+tblId).find('tbody').html(noRecs);

		} else {
			// set the edit/delete script in the first td of each row
			var obj=colOptions.parentWidgetObj;
			var actions = '';
			var count=0;
			$(itemLoop, xmlObj).each(function(){
				var val = '{';
				var itemObj=this;
				$("Actions>Action",this).each(
						function() {
							var type = $(this).attr('type');
							if (type == 'Ajax') {
								var actionNm = $(this).attr('name');
								var param = $('Parameters',this).text();
								try{
									param=param.trim();
								}catch(e){
									param=param;
								}
								param=escape(obj+'.'+param);
								if(val != '{'){
									val=val+',';
								}
								val+='\''+actionNm+'\''+':'+'\''+param+'\'';
							}

						});
				$(itemObj).children().each(function(){
					val+=',';
					var nodeName=$(this).get(0).nodeName;
					if(nodeName != 'Actions' && nodeName != 'Action' && nodeName != 'Parameters'){
						val+="'"+nodeName+"'";
						val+=":";
						val+="'"+escape($(this).text())+"'";
					}
				});
				val+='}';
				// set the json to the firs td of the row
				$('#'+tblId).find('tbody:eq(0) > tr').eq(count).find('td:eq(0)').attr('data',val);
				count++;
				val='';
			});

			if($('.datatable-top-left').size()== 0){
				// add the export, print and filter options
				opsleft="<div class='datatable-top-actions datatable-top-left' style='float:left;'>";
				/*if(tblOptions.sort || tblOptions.filter){
				opsleft+="<a class='spTblOptions' style='background-position:-5px -20px;padding-left:15px;' href='javascript:void(0);' onclick=javascript:$.fn.expertusDataTableToggleOps('"+tblId+"'); >"+SMARTPORTAL.t("Show Options")+"</a>";
				opsleft+='|';
				}*/

				var append=false;
				if(tblOptions.print && colOptions.printCols != undefined){
					if(append){
						opsleft+='|';
					}
					var espDataStr = escape(colOptions.datastr);
					var colNames=colOptions.printCols[0];
					colNames = escape(colNames );
					/*for(var i=0;i<colOptions.colNames.length;i++){
						colNames += "'"+i+"':'"+colOptions.colNames[i].title+"',";
					}
					colNames = colNames.substring(0,colNames.length-1);
					colNames = "{" + colNames+"}";
					colNames = escape(colNames );

					var xmlMap = "";
					for(var i=0;i<colOptions.colModel.length;i++){
						xmlMap += "'"+i+"':'"+colOptions.colModel[i].xmlmap+"',";
					}
					xmlMap = "{" + xmlMap +"}";*/
					var xmlMap = colOptions.printCols[1];
					xmlMap = escape(xmlMap);
					opsleft+="<a href='javascript:void(0);' onclick=javascript:$.fn.expertusDataTablePrint('"+ espDataStr +"','"+colNames+"','"+ xmlMap +"','"+obj+"');>"+SMARTPORTAL.t('Print')+"</a>";
					append=true;
				}

				if(tblOptions.exportTbl && colOptions.exportCols != undefined){
					if(append){
						opsleft+='|';
					}
					var espDataStr = escape(colOptions.datastr);
					var colNames=colOptions.exportCols[0];
					colNames = escape(colNames );
					/*for(var i=0;i<colOptions.colNames.length;i++){
						colNames += "'"+i+"':'"+colOptions.colNames[i].title+"',";
					}
					colNames = colNames.substring(0,colNames.length-1);
					colNames = "{" + colNames+"}";
					colNames = escape(colNames );*/

					var xmlMap = colOptions.exportCols[1];
					/*for(var i=0;i<colOptions.colModel.length;i++){
						xmlMap += "'"+i+"':'"+colOptions.colModel[i].xmlmap+"',";
					}
					xmlMap = "{" + xmlMap +"}";*/
					xmlMap = escape(xmlMap);
					opsleft+="<a href='javascript:void(0);' onclick=javascript:$.fn.expertusDataTableExport('"+ espDataStr +"','"+colNames+"','"+ xmlMap +"','"+parentObjName+"'); >"+SMARTPORTAL.t("Export")+"</a>";
					//opsleft+="<a href='javascript:void(0);' onclick=javascript:$.fn.expertusDataTableExport('"+ espDataStr +"','"+colNames+"'); >"+SMARTPORTAL.t("Export")+"</a>";
					append=true;
				}
				opsleft += '</div>';

			}
		}

		//$('.datatable-top-right').remove();
		//get the calling widget
		var widgetNm = colOptions.parentWidgetObj;

		$('#'+tblId.substring(0,tblId.indexOf('Tbl'))).find('.datatable-top-right').remove();
		var opsright="<div class='datatable-top-actions datatable-top-right'>";
		opsright+="<a id='showFirstPage' href='javascript:void(0);' title='"+SMARTPORTAL.t('First Page')+"' onClick= 'jQuery(\"#"+tblId+"\").showFirstPage(\""+widgetNm+"\");'>"+SMARTPORTAL.t('<< First')+' '+"</a>";
		opsright+="<a id='showPreviousPage' href='javascript:void(0);' title='"+SMARTPORTAL.t('Previous')+"' onClick= 'jQuery(\"#"+tblId+"\").showPreviousPage(\""+widgetNm+"\");'>"+SMARTPORTAL.t('< Prev')+"</a>";
		opsright+="<span><span> ("+$('#'+tblId).getGridParam('pagination').currentPage+"</span> of "+$('#'+tblId).getGridParam('pagination').totalPages+")</span>";
		opsright+=" <a id='showNextPage' href='javascript:void(0);' title='"+SMARTPORTAL.t('Next')+"' onClick= 'jQuery(\"#"+tblId+"\").showNextPage(\""+widgetNm+"\");'>"+SMARTPORTAL.t('Next')+"></a>";
		opsright+="<a id='showLastPage' href='javascript:void(0);' title='"+SMARTPORTAL.t('Last Page')+"' onClick='jQuery(\"#"+tblId+"\").showLastPage(\""+widgetNm+"\");'> "+SMARTPORTAL.t('Last >>')+"</a>";
        pagetot= $("#"+tblId).getGridParam('pagination').totalPages;
		if(pagetot>=20)
		{
			//opsright+="<div  style='color:#ff0000'>Search returned more records than what you see in the results. Please refine your search.</div>";

		}
		var ops=opsleft+opsright;
		//$('.grid_hdiv:first').prepend(ops);
		$('#'+tblId.substring(0,tblId.length-3)+'DataTblDiv').before(ops);
		$('#'+tblId).after(opsright);

		$('#'+tblId).addClass("scroll");
		if(!tblOptions.pageTitle)
			$('.grid_htable').hide();
		$('.grid_hdiv').css('width','100%');
		$('.grid_bdiv').css('width','100%');
		$('#'+tblId).css('width','100%');
		$('#lui_'+tblId).hide();
		Drupal.attachBehaviors(); 
	};

	jQuery.fn.expertusDataTableExport=function(dataStr, colNames, xmlMap,parentObj){
		$('.datatable-ops').remove();
		dataStr = unescape(dataStr);
		colNames = unescape(colNames);
		var cols = (new Function("return "+colNames))();
		xmlMap = unescape(xmlMap);
		xmlMap = (new Function("return "+xmlMap))();
		var counter=0;
		var str = "";
		for(var props in cols)
			counter++;

		for(var i=0;i<counter;i++){
			str += cols[i]+",";

		}

		str = str.substring(0,str.length-1);
		str +="<br/>";
		counter=0;
		for(var props in xmlMap)
			counter++;

		var obj1=eval(parentObj);
		var dataStr=obj1.stringToXml(dataStr);

		$(dataStr).find("Item").each(function(){
			for(var i=0;i<counter;i++){


				var str1 = $(this).find(xmlMap[i]).text().replace(/,/g,'');
				str += str1+",";

			}
			str = str.substring(0,str.length-1);
			str += "<br/>";
		});


		var elemIF = document.createElement("form");
		elemIF.action = resource.service_url+'?actionkey=TableExport';
		elemIF.method='POST';
		elemIF.id='CSVform';
		elemIF.name='CSVform';
		elemIF.style.display = "none";
		document.body.appendChild(elemIF);

		var CSVformelement = document.getElementById('CSVform');
		var CSVbody = document.createElement("input");
		CSVbody.type = 'text';
		CSVbody.value=str;
		CSVbody.id='CSVbody';
		CSVbody.name='content';
		CSVbody.style.display = "none";
		CSVformelement.appendChild(CSVbody);


		document.getElementById('CSVform').submit();

		var CSVformelement = document.getElementById('CSVform');
		if (CSVformelement && CSVformelement.parentNode &&CSVformelement.parentNode.removeChild) {
			CSVformelement.parentNode.removeChild(CSVformelement);
		}

	};


	$.fn.expertusDataTableToggleOps=function(tblId){
		var margin = $('#'+tblId.substring(0,tblId.length-3)+'DataTblDiv').css('margin-left');
		if(margin == '5px'){
			$('#'+tblId.substring(0,tblId.length-3)+'DataTblDiv').css('margin-left', '110px');
			$('#'+tblId+'OpsDiv').slideToggle("slow");
			$('.spTblOptions').html(SMARTPORTAL.t('Hide Options'));
			$('.spTblOptions').css("background-position",'-5px -3px');

		} else {
			$('#'+tblId+'OpsDiv').slideToggle("slow", function(){
				$('#'+tblId.substring(0,tblId.length-3)+'DataTblDiv').css('margin-left', '5px');
			});
			$('.spTblOptions').html(SMARTPORTAL.t('Show Options'));
			$('.spTblOptions').css("background-position",'-5px -20px');

		}

	};

	jQuery.fn.expertusDataTableRefresh=function(caller,searchXml){
		eval(caller+'.refreshAction("'+searchXml+'")');
	};

	jQuery.fn.expertusDataTablePrint = function(dataStr, colNames, xmlMap,parentObj){
		dataStr = unescape(dataStr);
		colNames = unescape(colNames);
		var cols = (new Function("return "+colNames))();
		xmlMap = unescape(xmlMap);
		xmlMap = (new Function("return "+xmlMap))();
		var counter=0;

		for(var props in cols)
			counter++;

		var minWidth = 100;
		var cellWidth=0;
		if((counter-1)*minWidth < 600){
			tblWidth = 100*(counter-1);
			cellWidth = 100;
		}
		else{
			tblWidth = 600;
			cellWidth = Math.round(600 / (counter-1));
		}

		//var tableStr = "<table cellspacing='0' width='"+ tblWidth +"' style='border:solid 1px #000000;margin:0 auto;'><tr>";
		var tableStr = "<table cellspacing='0' width='90%' style='font-size:11px;border:solid 1px #000000;margin:0 auto;'><tr>";
		for(var i=0;i<counter;i++){
			//tableStr += "<th width='"+ cellWidth +"'>"+cols[i]+"</th>";
			tableStr += "<th style='background:#DDDDDD none repeat scroll 0 0;text-align:left;border-bottom:1px solid #000000;padding:4px;'>"+cols[i]+"</th>";
		}
		tableStr += "</tr>";

		counter=0;

		for(var props in xmlMap)
			counter++;
		var obj1=eval(parentObj);
		dataStr=obj1.stringToXml(dataStr);

		$(dataStr).find("Item").each(function(){
			tableStr += "<tr>";
			for(var i=0;i<counter;i++){
				tableStr += "<td style='padding:4px;vertical-align:top;'>"+$(this).find(xmlMap[i]).text()+"</td>";
			}
			tableStr += "</tr>";

		});
		tableStr += "</table>";
		var params = {"title":SMARTPORTAL.t("Print")};
		$.fn.printDiv(tableStr,params);
	};

	jQuery.fn.printDiv = function(bodyStr,params){
        var outhtml="";
        outhtml+="<link href='"+themepath+"/print.css?r' media='all' rel='stylesheet' type='text/css'>";
        outhtml+="<div align='center' style='text-align:left;font-family:arial;margin:50px 0 15px 0;width:100%;'>"; //border-style:solid;border-color:#000000;border-width:1px
        outhtml+=bodyStr;
        outhtml+="</div>";
        outhtml+="<div id='print_buttons' style='text-align:center;padding:10px;margin:15px;'>" +
		//Type="button" attribute include #912 fixed 'VJ' 05/05/2011
                     "<input onclick='window.focus();window.print();' type='button' class='redbutton' style='cursor:pointer' value='"+SMARTPORTAL.t("Print")+"'/>&nbsp;&nbsp;<input class='redbutton' type='button' value='"+SMARTPORTAL.t("Close")+"' style='cursor:pointer' onclick='self.focus();self.close()'/>"
                     "</div>";
        $.fn.getPrintPopup("<div id='adp_mytraining_tab_main'>"+outhtml+"</div>", params);
	};

	$.fn.getPrintPopup= function(outstr,params){
		params=params!=undefined?params:{title:undefined,width:undefined,height:undefined};
		var windowWidth=params.width!=undefined?params.width:"800";
		var windowHeight=params.height!=undefined?params.height:"800";
		var windowUri="";
		var windowName=SMARTPORTAL.t("PrintDetails");
		var centerWidth = (window.screen.width - windowWidth) / 2;
		var centerHeight = (window.screen.height - windowHeight) / 2;


		var windowOptions="toolbar=no,menubar=no,scrollbars=1,status=yes,location=no,resizable=1,width="+windowWidth+",height="+windowHeight+",left="+centerWidth+",top="+centerHeight;
		newWindow = window.open(windowUri,windowName,windowOptions);

		//newWindow = window.open(windowUri,'',windowOptions);
		newWindow.document.write("<!DOCTYPE html PUBLIC '-//W3C//DTD XHTML 1.0 Transitional//EN' 'http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd'>");
		newWindow.document.write(outstr);
		if(params.title!=undefined) {
			newWindow.document.title=params.title;
		}
		newWindow.document.close();
		if(newWindow) {
			newWindow.focus();
			return newWindow;
		}
		else
		{
			alert(SMARTPORTAL.t("Your browser blocked the popup. Disable popup blocker in your browser and try again."));
		}

	};

	/*************************** Obsolete Code **********************************/
	/*jQuery.fn.expertusDataTableShowOps=function(tblId,ops,obj){
		$('.datatable-ops').remove();
		var id;
		var script;
		if(ops!='Add'){
			id= $("#"+tblId).getGridParam('selrow');
			if(!id){
				alert(SMARTPORTAL.t("Please select row"));
				return;
			} else {
				var data=$('#'+tblId+' tr.ui-state-highlight td:first').metadata();
				script=ops=='Edit'?data.Edit:data.Delete;
				script=unescape(script);
			}
		} else {
			script=$(obj).metadata().script;
		}
		var renderCallback=$(obj).metadata().renderCallback;
		var html=$.fn.expertusDataTableOpsContent(tblId,ops,obj);
		$('#gview_'+tblId+'> .datatable-top-actions').eq(1).append(html);

		html=eval(script);
		$('#'+tblId+'_'+ops+'_opsdetails_content').html(html);
		if($('#'+tblId+'_'+ops+'_opsdetail').css('display')=='block'){
			$('#'+tblId+'_'+ops+'_opsdetail').hide("slide",{direction:"up"},'fast');
		} else {
			$('#'+tblId+'_'+ops+'_opsdetail').show("slide",{direction:"up"},'fast');
		}
		if(ops!='Add'){
			var mdata=$('#'+tblId+' tr.ui-state-highlight td:first').metadata();
			mdata=object2string(mdata);
			if(renderCallback != undefined) eval(renderCallback+'('+mdata+')');
		} else {
			if(renderCallback != undefined) eval(renderCallback+'()');
		}
		if(ops == 'Delete'){
			$('.datatable-ops-closebtn').css('display','none');
		}
		$('#'+tblId+'_'+ops+'_closebtn').click(function(){
			$('.datatable-ops').remove();
		});

		$('.ui-icon-close').click(function(){
			$('.datatable-ops').remove();
		});
	}

	jQuery.fn.expertusDataTableOpsContent=function(tblId,ops,obj){
		var title=$(obj).metadata().title;
		var width=$(obj).metadata().width;
		var height=$(obj).metadata().height;
		var opsdetail="<div id='"+tblId+"_"+ops+"_opsdetail' style=\"z-index:1002;display:none;position:absolute;height:"+height+";width:"+width+";\" class=\"datatable-ops ui-dialog ui-widget ui-widget-content ui-corner-all ui-draggable\">";
		opsdetail+="<div class=\"ui-dialog-titlebar ui-widget-header ui-corner-all ui-helper-clearfix\">";
		opsdetail+="<span class=\"ui-dialog-title\">";
		opsdetail+=title;
		opsdetail+="</span>";
		opsdetail+="<a href=\"javascript:void(0);\" class=\"ui-dialog-titlebar-close ui-corner-all\" role=\"button\" unselectable=\"on\" style=\"-moz-user-select: none;\">";
		opsdetail+="<span class=\"ui-icon ui-icon-close\" unselectable=\"on\" style=\"-moz-user-select: none;\"></span>";
		opsdetail+="</a>";
		opsdetail+="</div>";
		opsdetail+="<div id=\""+tblId+"_"+ops+"_opsdetails_content\" style=\"position:relative;height:auto;min-height:76px;width:auto;\" class=\"ui-dialog-content ui-widget-content\">";
		opsdetail+="</div>";
		opsdetail+="<div class=\"ui-dialog-buttonpane ui-widget-content ui-helper-clearfix\">";
		opsdetail+="<div class=\"datatable-ops-closebtn\">";
		opsdetail+="<button id=\""+tblId+"_"+ops+"_"+"closebtn\" type=\"button\" class=\"ui-state-default ui-corner-all\">"+SMARTPORTAL.t("Close")+"</button>";
		opsdetail+="</div>";
		var actions=$(obj).metadata().actionBtns;
		if(actions!=undefined && actions.length>0){
			for(var i=0;i<actions.length;i++){
				opsdetail+="<button id=\""+actions[i].actionId+"\" type=\"button\" class=\"ui-state-default ui-corner-all\">"+actions[i].actionName+"</button>";
			}
		}
		opsdetail+="</div>";
		opsdetail+="</div>";
		return opsdetail;
	};

	jQuery.fn.expertusDataTableToggleFilter=function(tblId){
		$('.datatable-ops').remove();
		if($('.dt-acc-row').size() == 1){
			$('.dt-acc-row').remove();
			$('.open_close').css("background-position",'0 -61px');
		}
		$("#"+tblId).find("#tableFilter_header").toggle();
	};
	*
	*/
	/*************************** Obsolete Code **********************************/


})(jQuery);

