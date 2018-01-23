registernamespace("EXPERTUS.SMARTPORTAL.ProfileMapManager");
EXPERTUS.SMARTPORTAL.ProfileMapManager=function(){
try{
	this.generateProfileSubTreeReq = function(obj){
		try{
		var request=new Object();
		request.query = new SOAPObject("getProfileSubTree").attr("xsi:type","null");
		request.query.appendChild(new SOAPObject("type").attr("xsi:type","null").val(obj.type));
		request.query.appendChild(new SOAPObject("id1").attr("xsi:type","null").val(obj.id1));
		request.query.appendChild(new SOAPObject("parent_id").attr("xsi:type","null").val(obj.parent_id));
		//request.query.appendChild(new SOAPObject("op").attr("xsi:type","null").val(obj.op));
		request.query.appendChild(new SOAPObject("map_attr").attr("xsi:type","null").val(obj.mapAttribute));
		var sr = new SOAPRequest("getProfileSubTree", request);
		sr="<?xml version='1.0' encoding='UTF-8'?>"+sr;
		sr=sr.toString();
		return sr;
		}catch(e){
			// to do
		}
	};

	this.generateSaveProfileMapReq = function(arr, mapAttrVal, params){ //, op){ //params
		try{
		var request=new Object();
		request.query = new SOAPObject("saveProfileMapping").attr("xsi:type","null");
		//arr = eval(arr);
		request.query.appendChild(new SOAPObject("id1").attr("xsi:type","null").val(params.id1));
		request.query.appendChild(new SOAPObject("parent_id").attr("xsi:type","null").val(params.parent_id));
		//request.query.appendChild(new SOAPObject("op").attr("xsi:type","null").val(op));
		for(var i=0;i<arr.length;i++){
			var rowset = request.query.appendChild(new SOAPObject("rowset").attr("xsi:type","null"));
			var tags = rowset.appendChild(new SOAPObject("tags").attr("xsi:type","null"));
			for(var j=0;j<arr[i].length;j++){
				tags.appendChild(new SOAPObject("mapping").attr("xsi:type","null").val(arr[i][j]));
			}
			var attr = rowset.appendChild(new SOAPObject("attr").attr("xsi:type","null"));
			attr.appendChild(new SOAPObject("type").attr("xsi:type","null").val(params.mapAttribute));
			attr.appendChild(new SOAPObject("value").attr("xsi:type","null").val(mapAttrVal[i]));
		}
		var sr = new SOAPRequest("saveProfileMapping", request);
		sr="<?xml version='1.0' encoding='UTF-8'?>"+sr;
		sr=sr.toString();
		return sr;
		}catch(e){
			// to do
		}
	};
}catch(e){
	// to do
}
};

$.extend(EXPERTUS.SMARTPORTAL.ProfileMapManager.prototype,EXPERTUS.SMARTPORTAL.AbstractManager);

registernamespace("EXPERTUS.SMARTPORTAL.ProfileMapWidget");
EXPERTUS.SMARTPORTAL.ProfileMapWidget=function(){
	try{
	this.divId;
	this.mode;
	this.widgetGlobalName='EXPERTUS.SMARTPORTAL.ProfileMapWidget';
	this.mapAttribute;
	this.parentObj;
	this.op;
	this.delRow;
	this.treeData;
	this.multirow;
	this.id1;
	this.parent_id;

	this.renderResults = function(){
		//alert("render");
		var obj = this;
		switch(this.mode){
			case "showtree":
				this.renderTree();
				break;
			case "savemapping":
				//$("#"+this.divId).html("");
				break;
		}
	};

	this.renderTree = function(){
		var tblId = "tbl_prof_tree";
		var html = '<table class="tbl_prof_map" cellspacing="0" width="100%" id="'+ tblId +'"><tr><th colspan="2" style="text-align:right;padding:10px 25px 0 0">';
		if(this.multirow=="true")
			html +='<a href="#" onclick="EXPERTUS.SMARTPORTAL.ProfileMapWidget.renderTreeRow(\''+tblId+'\');">'+SMARTPORTAL.t('Add Row')+'</a> | <a href="#" onclick="'+this.widgetGlobalName+'.removeRow();">'+SMARTPORTAL.t('Remove')+'</a> |';
		html += '<a href="#" onclick="EXPERTUS.SMARTPORTAL.ProfileMapWidget.saveMapping();">'+SMARTPORTAL.t('Save')+'</a></th></tr></table>';

		$("#"+this.divId).html(html);

		$("#"+this.divId).append('<div></div>');
		var obj = this;
		this.treeData = this.responseText;
		if($(this.treeData).find("selNodes").length){
			$(this.treeData).find("row").each(function(){
				//var row = $(this).find("row");
				obj.renderTreeRow(tblId, $(this));
			});
		}else
			this.renderTreeRow(tblId,null);
	};

	this.validateChecked = function(tree){
		//var t = $.tree.reference(id);
		var t = $.tree.plugins.checkbox.get_checked(tree);
		if(t.length==0)
			return false;
		else
			return true;
		/*for(var i=0;i<t.length;i++){
			alert(t[i].id);
		}*/
	};

	this.renderTreeRow = function(tblId, row){
		var obj = this;
		//var rowNo = $("#"+tblId).find("tr").length;
		var rowNo;
		if($("#"+tblId).find("tr:last").attr("rowno")==undefined)
			rowNo = 1;
		else
			rowNo = $("#"+tblId).find("tr:last").attr("rowno")+1;
		if($(this.treeData).find("mapAttr").length)
			$("#"+tblId+" tr:last").after('<tr id="prRow'+ rowNo +'" class="profmap_row" rowno="'+ rowNo +'"><td id="prCell'+ rowNo +'"> </td><td width="140" id="attrCell'+ rowNo +'"></td></tr>').slideDown(300);
		else
			$("#"+tblId+" tr:last").after('<tr id="prRow'+ rowNo +'" class="profmap_row" rowno="'+ rowNo +'"><td id="prCell'+ rowNo +'"></tr>').slideDown(300);

		$("#prRow"+ rowNo).click(function(){
			$("#"+obj.oldRow).removeClass("profmap_row_clicked");
			$(this).addClass("profmap_row_clicked");
			obj.oldRow = $(this).attr("id");
			obj.delRow = $(this).attr("id");
		});
		var i =0;
		$(this.treeData).find("profile").each(function(){
			$("#prCell"+rowNo).append("<div id='treeDiv"+rowNo+i+"' style='float:left;width:auto;padding:10px;'></div>");
			var qulfy = $(this).find("item:eq(0)").attr("qulfy");
			var selNodes = "";
			$(row).find("nodes").each(function(){
				if($(this).attr("qulfy")==qulfy)
					selNodes = $(this).text();
			});

			var arrNodes = new Array();
			if(selNodes.length){
				arrNodes = selNodes.split(",");
			}

			$("#treeDiv"+rowNo+i).attr("qulfy",qulfy);
			$("#treeDiv"+rowNo+i).tree({
				callback:{
					check_move: function(node, ref_node, type, objTree, rb){
						return false;
					},
					beforeclose: function(node, objTree){
						return false;
					}
				},
				selected: arrNodes,
				data:{
					type:"xml_flat",
					opts: {
						'static': $(this).html()
					}
				},
				type: {
					"default": {
						draggable : false
					}
				},
				ui:	{
					theme_name : "checkbox",
					animation: 200
				},
				plugins : {
					   "checkbox" : { }
				}
			});

			var t = $.tree.reference("#treeDiv"+rowNo+i);
			t.open_all();

			$("#treeDiv"+rowNo+i).find("a:eq(0)").css(
					{
						"font-weight":"bold",
						"color":"#4F4F7A"
					}
			);
			i++;
		});
		if($(this.treeData).find("mapAttr").length){
			var selStr = '<select id="selAttr'+rowNo+'">';
			//alert($(this.responseText).find("mapAttr").length);
			$(this.treeData).find("mapAttr").each(function(){
				selStr += '<option value="'+$(this).find("id").text()+'">'+ $(this).find("name").text() +'</option>';
			});
			selStr += "</select>";
			$("#attrCell"+rowNo).append(selStr);
			$("#selAttr"+rowNo).val($(row).find("type").text());
		}

		return false;
	};

	this.removeRow = function(){
		if($("#"+this.delRow).length)
			$("#"+this.delRow).remove();
		else
			alert(SMARTPORTAL.t("Please select a row"));
	};

	this.saveMapping = function(){
		var str = "";
		var arrCnt=1;
		var b = new Array();
		var mapAttr = new Array();
		var obj = this;
		var idx=0;
		var errFlg=false;
		var loaderObj;
		/*if($(".profmap_row").length==0){
			alert("No Profile to save");
			return;
		}*/

		$(".profmap_row").each(function(){
				var col=0;
				var a = new Array();
				var j=0;
				var row = $(this).attr("rowno");
				//alert(row);
				$(this).find(".tree-checkbox").each(function(){
					a[j] = new Array();
					var tree = $.tree.reference("#treeDiv"+row+col);
					if(!obj.validateChecked(tree)){
						alert(SMARTPORTAL.t("Please check atleast a profile"));
						tree.focus();
						errFlg = true;
						return false; //breaks the each loop
					}

					//tree.lock(true);
					var qulfy = $("#treeDiv"+row+col).attr("qulfy");
					var nodes = $.tree.plugins.checkbox.get_checked(tree);
					k=0;
					for(var i=0;i<nodes.length;i++){
						if(tree.children(nodes[i]).length==0){
							a[j][k]= qulfy+":"+nodes[i].id;
							k++;
						}
					}
					col++;
					j++;
				});
				if(errFlg)
					return false; // breaks the each loop
				b[arrCnt-1] = obj.getProfileRows(a, 0, new Array());
				mapAttr[arrCnt-1] = $("#selAttr"+row).val();

				//c[idx++] = b[row-1];
				arrCnt++;
		});

		var str;
		for(var i=0;i<b.length;i++){
			str="";
			for(var m=0;m<b[i].length;m++){
				//b[i][m] = "\""+ b[i][m] +"\"";
				str += b[i][m]+"\n";
				//alert(i+" "+m+" "+b[i][m]);
			}
			//alert(b[i].length+"\n"+str);
		}


		var manager = new EXPERTUS.SMARTPORTAL.ProfileMapManager();
		this.mode="savemapping";
		manager.initialize(this);
		manager.setLoaderObj(this.loaderObj);
		manager.setActionKey('Profile');
		manager.setEndPointURL(this.getEndPointURL());
		var params = {mapAttribute:this.mapAttribute, parent_id:this.parent_id, id1:this.id1};
		//manager.requestXml = manager.generateSaveProfileMapReq(b, mapAttr, this.mapAttribute, this.parentObj.templateId);
		manager.requestXml = manager.generateSaveProfileMapReq(b, mapAttr, params);
		//alert(manager.requestXml);
		manager.execute();

	};

	this.getProfileRows = function(a, i, str){

		var str2 = new Array();
		var m=0;
		for(var j=0;j<a[i].length;j++){
			for(var k=0;k<str.length;k++)
				str2[m++] = a[i][j]+","+str[k];
			if(str.length==0)
				str2[m++] = a[i][j];
		}
		if(a.length-1==i)
			return str2;
		else
			return this.getProfileRows(a, i+1, str2);
	};

	this.render = function(obj){
		this.renderBase(obj,this);
		//this.divId = obj.divId;
		var manager = new EXPERTUS.SMARTPORTAL.ProfileMapManager();
		this.mode="showtree";
		manager.initialize(this);
		manager.setLoaderObj(this.loaderObj);
		manager.setActionKey('Profile');
		manager.setEndPointURL(this.getEndPointURL());
		manager.requestXml = manager.generateProfileSubTreeReq({type:obj.typeId, mapAttribute: obj.mapAttribute,op:this.op, id1: this.id1, parent_id:this.parent_id });
		manager.execute();
	};

	this.initializeProfile = function(obj){
		this.loaderObj = obj.divId;
		this.setWidgetObject(this);
		this.setWidgetTitle("Profile Master");
		this.parentObj = obj.parentObj;
		this.id1=obj.id1;
		this.parent_id=obj.parent_id;
		//this.divId = obj.divId; //profileMapObj.parentObj.getUniqueWidgetId()+obj.divId; //parentObj.getUniqueWidgetId()+"profileMap";
		this.divId = obj.treeDiv;
		//this.divId = obj.treeDiv;
		this.mapAttribute = obj.mapAttribute;
		this.multirow = obj.multirow;
		//profileMapObj.parentObj = obj.parentObj;
		this.setUniqueWidgetId(obj.parentObj.getUniqueWidgetId());
	};
	}catch(e){
		// to do
	}

};

$.extend(EXPERTUS.SMARTPORTAL.ProfileMapWidget.prototype,EXPERTUS.SMARTPORTAL.AbstractDetailsWidget);

function getProfileMap(){
	try{
	var profileMapObj=new EXPERTUS.SMARTPORTAL.ProfileMapWidget();
	return profileMapObj;
	}catch(e){
		// to do
	}
}


/*function getProfileMap(obj){
	var profileMapObj=new EXPERTUS.SMARTPORTAL.ProfileMapWidget();
	profileMapObj.loaderObj = obj.divId;
	profileMapObj.setWidgetObject(profileMapObj);
	profileMapObj.setWidgetTitle("Profile Master");
	profileMapObj.parentObj = obj.parentObj;
	profileMapObj.divId = profileMapObj.parentObj.getUniqueWidgetId()+obj.divId; //parentObj.getUniqueWidgetId()+"profileMap";
	profileMapObj.mapAttribute = obj.mapAttribute;
	profileMapObj.parentObj = obj.parentObj;
	profileMapObj.setUniqueWidgetId(obj.parentObj.getUniqueWidgetId());
//	profileMapObj.op = obj.op;
//	alert(obj.parentObj.templateId);
	//profileMapObj.render({divId:obj.divId, typeId:obj.typeId, mapAttribute: obj.mapAttribute});
	return profileMapObj;
}*/