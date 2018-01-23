registernamespace("EXPERTUS.SMARTPORTAL.ProfileconfigManager");
EXPERTUS.SMARTPORTAL.ProfileconfigManager=function(){
	/* Sample Generate Request */
	try{
	this.generateRenderRequest = function(){
		try{
		var request=new Object();
		request.query = new SOAPObject("GetInitialData").attr("xsi:type","null");
		request.query.appendChild(new SOAPObject("dummy").attr("xsi:type","null"));
		var sr = new SOAPRequest("GetInitialData", request);
		sr="<?xml version='1.0' encoding='UTF-8'?>"+sr;
		sr=sr.toString();
		return sr;
		}catch(e){
			// to do
		}
	};
	
	this.generateLoadProfileReq = function(obj){
		try{
		var request=new Object();
		request.query = new SOAPObject("LoadProfileFields").attr("xsi:type","null");
		request.query.appendChild(new SOAPObject("id").attr("xsi:type","null").val(obj.cat_id));
		var sr = new SOAPRequest("LoadProfileFields", request);
		sr="<?xml version='1.0' encoding='UTF-8'?>"+sr;
		sr=sr.toString();
		return sr;
		}catch(e){
			// to do
		}
	};
 
	this.generateScreenRequest = function(obj){
		try{
		var request=new Object();
		request.query = new SOAPObject("GetScreenData").attr("xsi:type","null");
		request.query.appendChild(new SOAPObject("func_name").attr("xsi:type","null").val(obj.func_name));
		var sr = new SOAPRequest("GetScreenData", request);
		sr="<?xml version='1.0' encoding='UTF-8'?>"+sr;
		sr=sr.toString();
		return sr;
		}catch(e){
			// to do
		}
	};
	
	this.generateAvailColsRequest = function(obj){
		try{
		var request=new Object();
		request.query = new SOAPObject("AvailCols").attr("xsi:type","null");
		request.query.appendChild(new SOAPObject("id").attr("xsi:type","null").val(obj.cat_id));
		var sr = new SOAPRequest("AvailCols", request);
		sr="<?xml version='1.0' encoding='UTF-8'?>"+sr;
		sr=sr.toString();
		return sr;
		}catch(e){
			// to do
		}
	}; 
	
	this.generateSaveProfFieldRequest = function(obj){
		try{
		var request=new Object();
		request.query = new SOAPObject("SaveProfileField").attr("xsi:type","null");
		request.query.appendChild(new SOAPObject("config_id").attr("xsi:type","null").val(obj.config_id));
		request.query.appendChild(new SOAPObject("entity_id").attr("xsi:type","null").val(obj.entity_id));
		request.query.appendChild(new SOAPObject("category_id").attr("xsi:type","null").val(obj.category_id));
		request.query.appendChild(new SOAPObject("profile_field_id").attr("xsi:type","null").val(obj.profile_field_id));
		request.query.appendChild(new SOAPObject("field_name").attr("xsi:type","null").val(obj.field_name));
		request.query.appendChild(new SOAPObject("code").attr("xsi:type","null").val(obj.profile_code));
		request.query.appendChild(new SOAPObject("language_code").attr("xsi:type","null").val(obj.language_code));
		request.query.appendChild(new SOAPObject("attr1").attr("xsi:type","null").val(obj.attr1));
		request.query.appendChild(new SOAPObject("attr2").attr("xsi:type","null").val(obj.attr2));
		request.query.appendChild(new SOAPObject("attr3").attr("xsi:type","null").val(obj.attr3));
		request.query.appendChild(new SOAPObject("attr4").attr("xsi:type","null").val(obj.attr4));
		request.query.appendChild(new SOAPObject("data_type_id").attr("xsi:type","null").val(obj.data_type_id));
		if(obj.hier_id!=undefined)
			request.query.appendChild(new SOAPObject("hier_id").attr("xsi:type","null").val(obj.hier_id));
		if(obj.sel_id!=undefined)
			request.query.appendChild(new SOAPObject("sel_id").attr("xsi:type","null").val(obj.sel_id));
		if(obj.bool_id!=undefined)
			request.query.appendChild(new SOAPObject("bool_id").attr("xsi:type","null").val(obj.bool_id));
		
		request.query.appendChild(new SOAPObject("ui_id").attr("xsi:type","null").val(obj.ui_id));
		request.query.appendChild(new SOAPObject("column").attr("xsi:type","null").val(obj.column));
		request.query.appendChild(new SOAPObject("function").attr("xsi:type","null").val(obj.functions));
		request.query.appendChild(new SOAPObject("display_on").attr("xsi:type","null").val(obj.display_on));
		var sr = new SOAPRequest("SaveProfileField", request);
		sr="<?xml version='1.0' encoding='UTF-8'?>"+sr;
		sr=sr.toString();
		return sr;
		}catch(e){
			// to do
		}
	};
	
	this.generateEditProfileRequest = function(obj){
		try{
		var request=new Object();
		request.query = new SOAPObject("LoadProfileField").attr("xsi:type","null");
		request.query.appendChild(new SOAPObject("id").attr("xsi:type","null").val(obj.id));
		var sr = new SOAPRequest("LoadProfileField", request);
		sr="<?xml version='1.0' encoding='UTF-8'?>"+sr;
		sr=sr.toString();
		return sr;
		}catch(e){
			// to do
		}
	};
	
	this.generateDeleteProfileRequest = function(obj){
		try{
		var request=new Object();
		request.query = new SOAPObject("DeleteProfileField").attr("xsi:type","null");
		request.query.appendChild(new SOAPObject("id").attr("xsi:type","null").val(obj.id));
		var sr = new SOAPRequest("DeleteProfileField", request);
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


$.extend(EXPERTUS.SMARTPORTAL.ProfileconfigManager.prototype,EXPERTUS.SMARTPORTAL.AbstractManager);

registernamespace("EXPERTUS.SMARTPORTAL.ProfileConfigWidget");
EXPERTUS.SMARTPORTAL.ProfileConfigWidget=function(){
	try{
	this.widgetGlobalName="EXPERTUS.SMARTPORTAL.ProfileConfigWidget";
	this.objDataType;
	this.objFunctions;
	this.objUI;
	this.objFields;
	this.objHier;
	this.categoryId;
	this.objCategory;
	this.delId;
	this.objSel;
	this.objBoolTypes;
	this.objFunctionsVal;
	this.objLanguage;
	
	this.renderResults = function(){
		try{
			
		}catch(e){
			// to do
		}
		
	};
	
	this.render=function(serviceattributes){
		try{
		this.renderBase(serviceattributes,this);
		$("#"+this.getUniqueWidgetId()).html(this.renderHtml());
		var obj,loaderObj,param;
		}catch(e){
			// to do
		}
	};

	this.renderHtml=function(){
		try{
		var arrFunc =[
                      	{divId: "titleDiv", funcName: "titleMessage"},
                      	{divId: "parentDiv", funcName: "showMain" }
                     ];
		$("#"+this.getUniqueWidgetId()).setPageLayout(arrFunc, this);

		var manager = new EXPERTUS.SMARTPORTAL.ProfileconfigManager();
		manager.initialize(this);
		manager.setCallBack("displayTable");
		//manager.setLoaderObj(loaderObj); 
		manager.requestXml = manager.generateRenderRequest();
		manager.execute();
		}catch(e){
			// to do
		}
	};
	
	this.titleMessage = function(){
		try{
		return '<div class="front-block-header block-header sp_content_header"><h2 style="width: auto;">'+SMARTPORTAL.t("Profile Config Dictionary")+'</h2></div>';
		}catch(e){
			// to do
		}
	};
	
	this.showMain = function(){
		try{
		return "";
		}catch(e){
			// to do
		}
	};
	
	
	this.displayTable = function(){
		try{
		$("#msgDiv").hide();
		var html = "";
		
 		this.objCategory = $(this.responseXml).find("Category");
 		this.objDataType =  $(this.responseXml).find("DataType");
 		this.objFunctions =  $(this.responseXml).find("Functions");
 		this.objUI =  $(this.responseXml).find("UI");
 		this.objHier = $(this.responseXml).find("Hierarchy");
 		this.objLanguage = $(this.responseXml).find("Languages");
 		this.objSel = $(this.responseXml).find("Select");
 		this.objBoolTypes = $(this.responseXml).find("BoolTypes");
 		
 		html = this.showCategory();
 		
 		html += '<table id="datafields" ><tr><td class="sp_erromsg">'+SMARTPORTAL.t("Please select a category to configure Profile Fields")+'</td></tr><table>';

		//$("#parentDiv").html(html);
		$("#"+this.getUniqueWidgetId()+"parentDiv").html(html);
		$("#datafields tr:eq(1) td:first").css("border-left","1px solid #aaaaaa");
		$("#datafields tr:eq(1) td:last").css("border-right","1px solid #aaaaaa");
		}catch(e){
			// to do
		}
	};
	
	this.loadProfileFields = function(){
		try{
		$("#msgDiv").hide();
		
		this.categoryId = $("#category").val();
		this.invokeLoadProfileFields();
		}catch(e){
			// to do
		}
	};
	
	this.showCategory = function(){
		try{
		var html ="";
		html += "<table width='100%'>"; 
		html += "<td colspan='2'>"+SMARTPORTAL.t('Category')+"</td>";
		html += "<td>";
		html += '<select id="category" onchange="'+ this.getWidgetInstanceName()+'.loadProfileFields();"><option value="" >'+SMARTPORTAL.t("Select")+'</option>';
		$(this.objCategory).find("Item").each(function(){
			html+= '<option value="'+ $(this).find("Code").text() +'">' + $(this).find("Name").text() +'</option>';
		});
 		html += "</td></tr></table>";
 		return html;
		}catch(e){
			// to do
		}
	};
	
	this.showUpdatedProfileList = function(){
		try{
		if($(this.responseXml).find("ResponseCode").length==0)
		{
		$('#profilecode_exist_err').hide();	
		var arrFunc =[
                    	{divId: "titleDiv", funcName: "titleMessage"},
                    	{divId: "parentDiv", funcName: "showCategory" }
                   ];
		$("#"+this.getUniqueWidgetId()).setPageLayout(arrFunc, this);
		$("#category").val(this.categoryId);
		this.invokeLoadProfileFields();
		}
		else
		{	
			$('#profilecode_exist_err').show();	
		}
		}catch(e){
			// to do
		}
	};
	
	this.invokeLoadProfileFields = function(){
		try{
		var manager = new EXPERTUS.SMARTPORTAL.ProfileconfigManager();
		manager.initialize(this);
		manager.setCallBack("showProfileFields");
		//manager.setLoaderObj(loaderObj);
		manager.requestXml = manager.generateLoadProfileReq({cat_id:this.categoryId});
		manager.execute();
		}catch(e){
			// to do
		}
	};
	
	this.showProfileFields = function(){
		try{
		$("#datafields").remove();
		$("#functions").remove();
		$("#profileDets").remove();
		var widObj = this;
		if($(this.responseXml).find("Item").length==0){
			//$("#parentDiv").append(widObj.getEmptyFieldTable());
			$("#"+this.getUniqueWidgetId()+"parentDiv").append(widObj.getEmptyFieldTable());
			return;
		}
		
		var html ="";

 		if($("#category").length==0){
 			$("#msgDiv").html("Profile Field saved successfully.");
 			$("#msgDiv").show();
			html += "<table width='100%'>"; 
			html += "<td colspan='2'>Category</td>";
			html += "<td>";
			html += '<select id="category" onchange="'+ this.getWidgetInstanceName()+'.loadProfileFields();"><option value="" >'+SMARTPORTAL.t("Select")+'</option>';
			$(this.objCategory).find("Item").each(function(){
				html+= '<option value="'+ $(this).find("Code").text() +'">' + $(this).find("Name").text() +'</option>';
			});
	 		html += "</td></tr></table>";
 		}
		html += '<table width="100%" id="datafields" cellspacing="0" style="margin-top:25px;">';
		
		$(this.responseXml).find("StoredValues>field").each(function(){
			html += '<tr id="field'+ $(this).find("id").text() +'"><td style="padding:10px 0;line-height:1.5em;" width="85%">';
			html += '<span style="color:#003399;font-weight:bold;">'+$(this).find("name").text() + '</span><br> '+SMARTPORTAL.t("Data Type")+' <span style="color:#003399;">'+$(this).find("data_type").text() + '</span>';
			if($(this).find("data_type").text()=="Hierarchy")
				html += ' Mapped to <span style="color:#003399;">' + $(this).find("profile_value_name").text() + '</span>';
			html += '<br>Displayed as <span style="color:#003399;">' + $(this).find("ui").text() +'</span>';
			html += '</td><td width="15%"><img title="Edit Profile Field" class="imglink" src="'+ themepath +'/images/edit1.jpeg" onclick="'+widObj.getWidgetInstanceName()+'.editProfileField(\''+  $(this).find("id").text() +'\')" />';
			html += '&nbsp;&nbsp;&nbsp;<img title="Delete Profile Field" class="imglink" src="'+ themepath +'/images/delete1.jpeg" onclick="'+widObj.getWidgetInstanceName()+'.deleteProfileField(\''+  $(this).find("id").text() +'\')" />';
			html += '</td></tr>';
		});
		html += '<tr><td colspan="2" align="right" style="padding-top:10px;"><input type="button" value="Add Profile Field" onclick="' + this.getWidgetInstanceName() +'.getAvailCols();"/></td></tr>';
		html += '</table>';
		
		//$("#parentDiv").append(html);
		$("#"+this.getUniqueWidgetId()+"parentDiv").append(html);
		
		$("#category").val(this.categoryId);
		
		this.drawBorders();
		}catch(e){
			// to do
		}
	};
	
	this.drawBorders = function(){
		try{
		$("#datafields tr td").css("border","none");
		$("#datafields tr:first td").css("border-top","1px solid #aaaaaa");
		$("#datafields tr td").css("border-bottom","1px solid #aaaaaa");
		$("#datafields tr:last td").css("border","none");
		}catch(e){
			// to do
		}
	};
	
	
	this.deleteProfileField = function(id){
		try{
		lconfirm = confirm(SMARTPORTAL.t("Are you sure you want to delete this Profile Field?"));
		if(!lconfirm)
			return false;
		
		this.delId = id;
		var manager = new EXPERTUS.SMARTPORTAL.ProfileconfigManager();
		manager.initialize(this);
		manager.setCallBack("remProfileFields");
		manager.requestXml = manager.generateDeleteProfileRequest({id:id});
		manager.execute();
		}catch(e){
			// to do
		}
	};
	
	this.remProfileFields = function(id){
		try{
		$("#field"+this.delId).remove();
		$("#msgDiv").html(SMARTPORTAL.t("Profile Field deleted successfully"));
		$("#msgDiv").show();
		if($("#datafields tr").length==1){
			$("#datafields").html(this.getEmptyFieldRow());
		}else{
			this.drawBorders();	
		}
		}catch(e){
			// to do
		}
	};
	
	this.getEmptyFieldTable = function(){
		try{
		return '<table id="datafields">'+ this.getEmptyFieldRow() +'</table>';
		}catch(e){
			// to do
		}
	};
	
	this.getEmptyFieldRow = function(){
		try{
		return '<tr><td class="sp_erromsg" width="70%">'+SMARTPORTAL.t("No Profile Fields are defined")+'</td><td><input type="button" value="Add Profile Field" onclick="' + this.getWidgetInstanceName() +'.getAvailCols();"/></td></tr>';
		}catch(e){
			// to do
		}
	};
	
	this.editProfileField = function(id){
		try{
		var manager = new EXPERTUS.SMARTPORTAL.ProfileconfigManager();
		manager.initialize(this);
		manager.setCallBack("setDefProfileFields");
		manager.requestXml = manager.generateEditProfileRequest({id:id});
		manager.execute();
		}catch(e){
			// to do
		}
	};
	
	this.setDefProfileFields = function(){
		try{
		this.addProfileFields();
		$("#field_name").val($(this.responseXml).find("profile_field_name").text());
		$("#profile_code").val($(this.responseXml).find("code").text());
		$('#profile_code').attr('readonly',true);
		$("#language_code").val($(this.responseXml).find("lang_code").text());
		$('#language_code').attr('disabled','disabled');
		$("#attr1").val($(this.responseXml).find("attr1").text());
		$("#attr2").val($(this.responseXml).find("attr2").text());
		$("#attr3").val($(this.responseXml).find("attr3").text());
		$("#attr4").val($(this.responseXml).find("attr4").text());
		$("#dtype").val($(this.responseXml).find("data_type_id").text());
		$("#col").val($(this.responseXml).find("cols").text());
		$("#config_id").val($(this.responseXml).find("id").text());		
		$("#profile_field_id").val($(this.responseXml).find("profile_field_id").text());
		var display_on = $(this.responseXml).find("display_on").text();
		$("input[name='display_on']").each(function(){
			if($(this).val()==display_on)
				$(this).attr("checked","checked");
		});
		this.toggleCol();
		
		var data_type = $(this.responseXml).find("data_type").text();
		if(data_type=="Hierarchy")
			$("#hier").val($(this.responseXml).find("profile_value_id").text());
		else if(data_type=="Single Select" || data_type=="Multi Select")
			$("#sel").val($(this.responseXml).find("profile_value_id").text());
		else if(data_type=="Boolean")
			$("#blTyp").val($(this.responseXml).find("profile_value_id").text());
		
		this.toggleHier($("#dtype"));
		$("#ui").val($(this.responseXml).find("ui_id").text());
		$("#field_name").focus();
		
		var func = $(this.responseXml).find("functions").text();
		this.objFunctionsVal = func;
		
		var arr = func.split(",");
		for(var i=0;i<arr.length;i++){
			var ele = arr[i].split(":");			
			if(ele[1]==1){
				$("#chk"+ele[0]).attr("checked","true");
			}
		}
		}catch(e){
			// to do
		}
	};
	
	
	this.getAvailCols = function(){
		try{
		var manager = new EXPERTUS.SMARTPORTAL.ProfileconfigManager();
		manager.initialize(this);
		manager.setCallBack("addProfileFields");
		//manager.setLoaderObj(loaderObj);//t
		manager.requestXml = manager.generateAvailColsRequest({cat_id:this.categoryId});
		manager.execute();
		}catch(e){
			// to do
		}
	};
	
	this.showLinks = function(){
		try{
		var html = "";
		params = {
	            width:"140",
	            items: [	                    
	                {title: SMARTPORTAL.t("View/Edit Profile Fields"), jsFunc:"showUpdatedProfileList()", desc: SMARTPORTAL.t("View/Edit Profile Fields")}
	                  ]
	         };
		return this.formatRelatedLinks(params);
		}catch(e){
			// to do
		}
	};
	
	this.showAddProfileFields = function(){
		try{
		var html ="";
		//html += '<fieldset style="background-color:white;"><legend>Profile Field Details</legend>';
		html += '<div id="'+this.getUniqueWidgetId()+'profileDets" style="width:100%">';
		html += '<table class="addProfile">'; //<tr><td colspan="2" style="background-color:#cccccc;height:30px;font-weight:bold;padding-left:5px;">Profile Field Details</td></tr>
		html += '<tr><td width="30%">Name:<div class="sp_label_mandatory">*</div></td>';
		html += '<td width="70%"><input type="text" name="Name" id="field_name" value="" class="mandatory validateText"/></td></tr>';
		html += '<tr><td width="30%">Code:<div class="sp_label_mandatory">*</div></td>';
		html += '<td width="70%"><input type="text" name="ProfileCode" id="profile_code" value="" class="mandatory"/><span id="profilecode_exist_err" class="error" style="display: none;">This code is already in use. Please choose another one.</span></td></tr>';
		html += '<tr><td width="30%">Language:<div class="sp_label_mandatory">*</div></td>';
		html += '<td width="70%"><select id="language_code" name="Language" class="mandatory" ><option value="">'+SMARTPORTAL.t("Select")+'</option>';
		html += this.getDataTypes(this.objLanguage)+'</select></td></tr>';
		html += '<tr><td>Attribute 1:</td>';
		html += '<td><input type="text" id="attr1" value="" /></td></tr>';
		html += '<tr><td>Attribute 2:</td>';
		html += '<td><input type="text" id="attr2" value="" /></td></tr>';
		html += '<tr><td>Attribute 3:</td>';
		html += '<td><input type="text" id="attr3" value="" /></td></tr>';
		html += '<tr><td>Attribute 4:</td>';
		html += '<td><input type="text" id="attr4" value="" /></td></tr>';
		html += '<tr><td>Data Type:<div class="sp_label_mandatory">*</div></td>';
		html += '<td><select id="dtype" class="mandatory" name="Data Type" onchange="'+ this.getWidgetInstanceName()+'.toggleHier($(this));"><option value="">'+SMARTPORTAL.t("Select")+'</option>';
		html += this.getDataTypes(this.objDataType);
		html += '</select><br />';
		html += '<select id="sel" name="Select Field"  style="display:none;"><option value="">Select</option>'+ this.getDataTypesID(this.objSel) +'</select>';
		html += '<select id="hier" name="Hierarchy"  style="display:none;"><option value="">Select</option>'+ this.getDataTypesID(this.objHier) +'</select>';
		html += '<select id="blTyp" name="Boolean" style="display:none;"><option value="">Select</option>'+ this.getDataTypesID(this.objBoolTypes) +'</select></td></tr>';
		
		html += '<tr><td>Mapped To:<div class="sp_label_mandatory">*</div></td>';
		html += '<td><select id="col" class="mandatory" name="Mapped To">'; //<option value="">Select</option>
		$(this.responseXml).find("col").each(function(){
			html += '<option value="'+ $(this).text() +'">Column '+ $(this).text()+ '</option>';
		});
		
		html += '</select></td></tr>';
		html += '<tr><td>Display as:<div class="sp_label_mandatory">*</div></td>';
		html += '<td><select id="ui" class="mandatory" name="Display as"><option value="">'+SMARTPORTAL.t("Select")+'</option>';
		html += this.getDataTypes(this.objUI);
		html += '</select></td></tr>';

		html += '<tr><td>Display on:<div class="sp_label_mandatory">*</div></td>';
		html += '<td>';
		html += '<input type="radio" checked id="d" name="display_on" value="L" onclick="'+ this.getWidgetInstanceName()+'.toggleCol();"/> Left <input id="d2" type="radio" name="display_on" value="R" onclick="'+ this.getWidgetInstanceName()+'.toggleCol();"/> Right';
		html += '</td></tr>';
		
		//html += '<tr height="40"><td></td><td>';
		//html += '<input type="button" value="Mark Participating Functions" onclick="'+ this.getWidgetInstanceName()+'.showFunctions();"/>';
		//html += '</td></tr>';
		html += '</table>';
		html += '<input type="hidden" id="profile_field_id" value="" />';
		html += '<input type="hidden" id="config_id" value="" />';
		html += '</div>';
		//html += '</fieldset>';
		
		
		html+="<div id='"+this.getUniqueWidgetId()+"functionAccordianList'>";
		html+='<div class="sp_labeldatadiv">';
		html+='<table>';
		html+='<tr><td class="sp_label">';
		html+='<div onclick="javascript:'+this.getWidgetInstanceName()+'.addAccordionToTable(\'open_close\',\'0 0\',\'0 -61px\',\'dt-child-row\','+this.getWidgetInstanceName()+'.showFunctionList,this,'+this.getWidgetInstanceName()+');" style="float:left" title="'+SMARTPORTAL.t("Click to show Participating Functions")+'">';
		html+='<div class="open_close"></div>';
		html+='<div id="accordion_part_func" class="accordion-text" style="width:180px;">'+SMARTPORTAL.t("Participating Functions")+'</div></div>';		
		html+='</td>';
		html+='</tr>';
		html+='</table>';
		html+='</div>';	
		return html;
		}catch(e){
			// to do
		}
	};
	this.showSaveButton=function(){
		try{
		var ostr = "";
		ostr +='<div class="sp_labeldatadiv" style="text-align:right;margin-right:160px;">';
		ostr +='<input type="button" style="left:145px;position:relative;" value="'+SMARTPORTAL.t("Save")+'" onclick="'+this.getWidgetInstanceName()+'.saveProfileField()">';
		ostr +='</div>';
		return ostr;
		}catch(e){
			// to do
		}
	};
	this.showFunctionList=function(catdiv,accordionLink,parentObj)
	{
	try{	
	var html='';
	html += '<div id="'+parentObj.getUniqueWidgetId()+'functions" class="catalog-accordion" style="border:0px;">';//style="display:none;margin-top:12px;"
	html += '<table class="addProfile">';
	var i=0;	
	//$(this.objFunctions).find("Item").each(function(){
	$(parentObj.objFunctions).find("Item").each(function(){
		
		html += "<tr>";
		html += '<td width="30"><input type="checkbox" id="chk'+$(this).find("Code").text()+'" value="0" /></td>';
		html += "<td>"+$(this).find("Name").text()+"</td>";		
		html += "</tr>";
		i++;
	});
	html += '</table>';
	
	html +=	'</div>';
	catdiv.html(html);
	catdiv.css('display','block');
	
	var func = $(parentObj.responseXml).find("functions").text();
	this.objFunctionsVal = func;
	var arr = func.split(",");
		for(var i=0;i<arr.length;i++){
			var ele = arr[i].split(":");			
			if(ele[1]==1){
				$("#chk"+ele[0]).attr("checked","true");
			}
		}
	}catch(e){
		// to do
	}
	};
	
	
	this.addProfileFields = function(){
		try{
		var arrFunc =[
                    	{divId: "titleDiv", funcName: "titleMessage"},
                    	{divId: "linkDiv", funcName: "showLinks"},
                    	{divId: "parentDiv", funcName: "showAddProfileFields" }, //mainDiv
                    	{divId: "buttonDiv",   funcName: "showSaveButton"}
                   ];
		$("#"+this.getUniqueWidgetId()).setPageLayout(arrFunc, this);
		$("#language_code").val('cre_sys_lng_eng');
		}catch(e){
			// to do
		}
	};
	
	this.showFunctions = function(){
		try{
		$("#functions").slideDown();
		}catch(e){
			// to do
		}
	};
	
	this.toggleCol = function(){
		try{
		if($("input[name='display_on']:checked").val()=="R")
			$("#col").attr("disabled","disabled");
		else
			$("#col").attr("disabled","");
		}catch(e){
			// to do
		}
	};
	
	this.saveProfileField = function(){
		try{
		if(this.mandatoryCheck(this.getUniqueWidgetId()+"profileDets")){
			if(this.validateFieldCheck(this.getUniqueWidgetId()+"profileDets")){
				var j=0,funcStr="";
				if(document.getElementById(this.getUniqueWidgetId()+"functions")!=null){
					$(this.objFunctions).find("Item").each(function(){
					if(j!=0)
						funcStr += ",";
					funcStr += $(this).find("Code").text()+ ":"+ ($("#chk"+$(this).find("Code").text()).attr("checked")?"1":"0");
					j++;
					});
				}
				else
				{
					funcStr = this.objFunctionsVal;
				}
								
		//		alert($("input[name='display_on']:checked").val());
				var params = {config_id: $("#config_id").val(), entity_id:"1", category_id:this.categoryId, profile_field_id:$("#profile_field_id").val(), field_name:$("#field_name").val(), data_type_id:$("#dtype").val(), hier_id: ($("#dtype option:selected").text()=="Hierarchy" ? $("#hier").val():"") ,
						sel_id: ( ($("#dtype option:selected").text()=="Single Select" || $("#dtype option:selected").text()=="Multi Select") ? $("#sel").val():""),profile_code: $("#profile_code").val(), language_code: $("#language_code").val(),
						bool_id:( ($("#dtype option:selected").text()=="Boolean")?$("#blTyp").val():""), column: $("#col").val(), ui_id: $("#ui").val(), functions: funcStr, display_on: $("input[name='display_on']:checked").val(), attr1: $("#attr1").val(), attr2: $("#attr2").val(), attr3: $("#attr3").val(), attr4: $("#attr4").val() };
				var manager = new EXPERTUS.SMARTPORTAL.ProfileconfigManager();
				manager.initialize(this);
				manager.setCallBack("showUpdatedProfileList");
				
				manager.setLoaderObj('parentPageLayout');
				manager.requestXml = manager.generateSaveProfFieldRequest(params);
				manager.execute();
			}
		}
		}catch(e){
			// to do
		}
	};
	
	this.showProfile = function(){
		try{
		$("#functions").hide("slide",{direction:"right"},1000);
		$("#profileDets").show("slide",{direction:"left"},1000);
		}catch(e){
			// to do
		}
	};
	
	this.showFunc = function(){
		try{
		$("#functions").animate({width:'show'});
		}catch(e){
			// to do
		}
	};
	
	this.appProfileFields = function(){
		try{
		$("#datafields tr:gt(0)").remove();
		var html ="";
		var dataTypes = this.getDataTypes(this.objDataType);
		var ui = this.getDataTypes(this.objUI);
		var hier = this.getDataTypes(this.objHier);
		var row=0;
		var widObj = this;
		this.objFields = $(this.responseXml).find("Items");
		$(this.responseXml).find("Item").each(function(){
			html += "<tr height='65'>";
			html += '<td style="border-left:1px solid #aaaaaa;padding-left:5px;vertical-align:top;padding-top:10px;">' + $(this).find("name").text();
			html += '<input type="hidden" id="field'+row+'" value="'+ $(this).find("id").text() +'" />';
			html += '</td>';
			html += '<td style="vertical-align:top;padding-top:10px;"><select id="dtype'+row+'" onchange="'+ widObj.getWidgetInstanceName()+'.toggleHier($(this))">'+ dataTypes +'</select>';
			html += '<div style="display:none;padding-top:5px;"><select id="hier'+row+'"><option value="">Select</option>'+ hier +'</select></div>';
			html += '</td>';
			html += '<td style="vertical-align:top;padding-top:10px;"><select id="ui'+row+'">'+ ui +'</select></td>';
			html += '<td style="border-right:1px solid #aaaaaa;vertical-align:top;padding-top:10px;">' + '<select id="col'+row + '">';
			for(var i=1;i<=26;i++){ //26 to be obtained from a dictionary
				html += '<option value="'+ i +'">Col'+ i +'</option>';
			}
			html += '</select></td>';
			/*for(var i=0;i<$(widObj.objFunctions).find("Item").length;i++){
				if(i==0)
					html += "<td style='text-align:center;border-left:1px solid #aaaaaa;'>";
				else if(i==$(widObj.objFunctions).find("Item").length-1)
					html += "<td style='text-align:center;border-right:1px solid #aaaaaa;'>";
				else
					html += "<td style='text-align:center;'>";
				html += '<input type="checkbox" id="chk'+row+i+'" value="0" />';
				html += "</td>";
			}*/
			html += "</tr>";
			html += "<tr><td colspan='4' style='padding:0 5px;border-left:1px solid #aaaaaa;border-right:1px solid #aaaaaa;'>";
			html += '<div style="color:orange;cursor:pointer;" onclick="'+ widObj.getWidgetInstanceName()+'.toggleFuncs(\'funcs'+row+'\')">Functions>></div>';
			html += '<div id="funcs'+row+'" style="overflow:auto;display:none;"><table><tr>'; 
			$(widObj.objFunctions).find("Item").each(function(){
				html += "<td style='text-align:center;' width='30'>"+ $(this).find("name").text() +"</td>";
			});
			
			html += '</tr><tr>';
			for(var i=0;i<$(widObj.objFunctions).find("Item").length;i++){
				html += "<td style='text-align:center;'>";
				html += '<input type="checkbox" id="chk'+row+i+'" value="0" />';
				html += "</td>";
			}
			html += '</tr></table>';
			html += '</div>';
			html += '<div class="line-separater" style="margin-top:5px;"></div>';
			html += "</td></tr>";
			row++;
		});
		var colcnt = 4 + $(this.objFunctions).find("Item").length;
//		html += '<td colspan="'+ Math.ceil(colcnt/2) +'"><input type="button" value="Save" onclick=""/></td></tr>' ;
		$("#datafields").append(html);
		
	/*	alert($("#datafields tr:last").text());
		alert($("#datafields tr:last td:last").find("div:last").length);*/
//		alert($(".line-separater:last").length);
		$(".line-separater:last").removeClass("line-separater");
		
		html = '<tr><td colspan="'+ colcnt +'" align="right" colspan="6" style="padding-right: 30px; height: 50px;border-top:1px solid #aaaaaa"><input type="button" value="Save" onclick="'+this.getWidgetInstanceName()+'.saveDataMap();" align="center"/></td></tr>';
		$("#datafields").append(html);
		}catch(e){
			// to do
		}
	};
	
	this.getDataTypes = function(obj){
		try{
		var html ="";
		$(obj).find("Item").each(function(){
			html += '<option value="'+ $(this).find("Code").text() +'">'+ $(this).find("Name").text()+'</option>';
		});
		return html;
		}catch(e){
			// to do
		}
	};
	
	this.getDataTypesID = function(obj){
		try{
		var html ="";
		$(obj).find("Item").each(function(){
			html += '<option value="'+ $(this).find("Id").text() +'">'+ $(this).find("Name").text()+'</option>';
		});
		return html;
		}catch(e){
			// to do
		}
	};
	
	this.toggleHier = function(obj){
		try{
		var selDT = $(obj).find("option:selected").text();
		if(selDT=="Hierarchy"){
			
			$('#blTyp_newid').html('');
			$('#sel_newid').html('');
			$('#hier_newid').html('Please enter a value for Hierarchy.');
			$("#sel").css("display","none");
			$("#blTyp").css("display","none");
			$("#hier").css("display","inline");
			$("#hier").addClass("mandatory");
			$("#blTyp").removeClass("mandatory");
			$("#sel").removeClass("mandatory");
			$("#hier").focus();
		}else if(selDT=="Single Select" || selDT=="Multi Select"){  
			$('#hier_newid').html('');
			$('#blTyp_newid').html('');
			$('#sel_newid').html('Please enter a value for Select.');
			$("#hier").css("display","none");
			$("#blTyp").css("display","none");
			$("#sel").css("display","inline");
			$("#sel").addClass("mandatory");
			$("#blTyp").removeClass("mandatory");
			$("#hier").removeClass("mandatory");
			$("#sel").focus(); 
		}else if(selDT=="Boolean"){  
			$('#hier_newid').html('');
			$('#sel_newid').html('');
			$('#blTyp_newid').html('Please enter a value for Boolean.');
			$("#hier").css("display","none");
			$("#sel").css("display","none");
			$("#blTyp").css("display","inline");
			$("#blTyp").addClass("mandatory");		
			$("#sel").removeClass("mandatory");
			$("#hier").removeClass("mandatory");
			$("#blTyp").focus();
		}
		else{  
			$("#blTyp").css("display","none");
			$("#hier").css("display","none");
			$("#sel").css("display","none");	
			$('#hier_newid').html('');
			$('#sel_newid').html('');
			$('#blTyp_newid').html('');
			$("#sel").removeClass("mandatory");
			$("#hier").removeClass("mandatory");
			$("#blTyp").removeClass("mandatory");
		}

		if(selDT == "Boolean"){
			$("#ui").find("option:all").each(function(){
				if($(this).text() != "Radio Button"){
					$(this).attr("disabled","disabled");
				}else{
					$(this).attr("disabled","");
					$(this).attr("selected","selected");
				}
			});

		}else if(selDT == "Date"){
			$("#ui").find("option:all").each(function(){
				if($(this).text() != "Date Picker"){
					$(this).attr("disabled","disabled");
				}else{
					$(this).attr("disabled","");
					$(this).attr("selected","selected");
				}
			});
		}else if(selDT== "Single Select"){
			$("#ui").find("option:all").each(function(){
				if($(this).text() != "Dropdown" && $(this).text() != "Radio Button"){
					$(this).attr("disabled","disabled");
				}else{
					$(this).attr("disabled","");
					$(this).attr("selected","selected");
				}
			});
		}else if(selDT == "Multi Select"){
			$("#ui").find("option:all").each(function(){
				if($(this).text() != "Listbox" && $(this).text() != "Checkbox" ){
					$(this).attr("disabled","disabled");
				}else{
					$(this).attr("disabled","");
					$(this).attr("selected","selected");
				}
			});
		}else if(selDT == "Text"){
			$("#ui").find("option:all").each(function(){
				if($(this).text() != "Textbox" && $(this).text() != "Textarea"){
					$(this).attr("disabled","disabled");
				}else{
					$(this).attr("disabled","");
					$(this).attr("selected","selected");
				}
			});
			
		}else if(selDT == "Hierarchy"){
			$("#ui").find("option:all").each(function(){
				if($(this).text() != "Tree" ){
					$(this).attr("disabled","disabled");
				}else{
					$(this).attr("disabled","");
					$(this).attr("selected","selected");
				}
			});
		}
		}catch(e){
			// to do
		}
	};
	
	this.toggleFuncs = function(id){
		try{
		if($("#"+id).css("display")=="block")
			$("#"+id).slideUp();
		else
			$("#"+id).slideDown();
		}catch(e){
			// to do
		}
	};
	
	this.saveDataMap = function(){
		try{
		var request=new Object();
		request.query = new SOAPObject("SaveDataMap").attr("xsi:type","null");
		request.query.appendChild(new SOAPObject("entity").attr("xsi:type","null").val("1"));
		request.query.appendChild(new SOAPObject("category").attr("xsi:type","null").val($("#category").val()));
		var fieldCnt = $(this.objFields).find("Item").length;
		if(!this.validateCols(fieldCnt))
			return;
		
		for(var i=0;i<fieldCnt;i++){
			var field = request.query.appendChild(new SOAPObject("field").attr("xsi:type","null"));
			field.appendChild(new SOAPObject("field_id").attr("xsi:type","null").val($("#field"+i).val()));
			field.appendChild(new SOAPObject("data_type").attr("xsi:type","null").val($("#dtype"+i).val()));
			if($("#dtype"+i).find("option:selected").text()=="Hierarchy" && $("#hier"+i).val()!="")
				field.appendChild(new SOAPObject("hier_id").attr("xsi:type","null").val($("#hier"+i).val()));
			else
				field.appendChild(new SOAPObject("hier_id").attr("xsi:type","null"));
			if(($("#dtype"+i).find("option:selected").text()=="Single Select" || $("#dtype"+i).find("option:selected").text()=="Multi Select") && $("#sel"+i).val()!="")
				field.appendChild(new SOAPObject("sel_id").attr("xsi:type","null").val($("#sel"+i).val()));
			else
				field.appendChild(new SOAPObject("sel_id").attr("xsi:type","null"));
			field.appendChild(new SOAPObject("ui").attr("xsi:type","null").val($("#ui"+i).val()));
			field.appendChild(new SOAPObject("column").attr("xsi:type","null").val($("#col"+i).val()));
			var j=0, funcStr="";
			$(this.objFunctions).find("Item").each(function(){
				if(j!=0)
					funcStr += ",";
				funcStr += $(this).find("Code").text()+ ":"+ ($("#chk"+i+j).attr("checked")?"1":"0");
				j++;
			});
			field.appendChild(new SOAPObject("function").attr("xsi:type","null").val(funcStr));
		}
		
		var sr = new SOAPRequest("SaveDataMap", request);
		sr="<?xml version='1.0' encoding='UTF-8'?>"+sr;
		sr=sr.toString();
		var manager = new EXPERTUS.SMARTPORTAL.ProfileconfigManager();
		manager.initialize(this);
		manager.setCallBack("showMesg");
		//manager.setLoaderObj(loaderObj);
		manager.requestXml = sr;
		manager.execute();
		}catch(e){
			// to do
		}
	};
	
	this.validateCols = function(fieldCnt){
		try{
		var arr = new Array();
		for(var i=0;i<fieldCnt;i++){
			if(arr[$("#col"+i).val()]=="1"){
				alert(SMARTPORTAL.t("Please choose different column"));
				$("#col"+i).focus();
				return false;
			}else
				arr[$("#col"+i).val()]="1";
		}
		return true;
		}catch(e){
			// to do
		}
	};
	
	this.showMesg = function(){
		try{
		$("#msgDiv").html(SMARTPORTAL.t("Profile Config saved successfully."));
		$("#msgDiv").show();
		}catch(e){
			// to do
		}
	};

	this.getScreenData = function(func_name){
		try{
		var manager = new EXPERTUS.SMARTPORTAL.ProfileconfigManager();
		manager.initialize(this);
//		manager.setCallBack("showMesg");
		//manager.setLoaderObj(loaderObj);
//		alert(manager.getActionKey());
		manager.setActionKey('ProfileConfig');
		manager.setEndPointURL(this.getEndPointURL());
		manager.requestXml = manager.generateScreenRequest({func_name:func_name});
//		alert(manager.requestXml);
		manager.execute();
		}catch(e){
			// to do
		}
	};
	}catch(e){
		// to do
	}
};
$.extend(EXPERTUS.SMARTPORTAL.ProfileConfigWidget.prototype,EXPERTUS.SMARTPORTAL.AbstractDetailsWidget);
 
function callprofileconfigdetail(){
	try{
	var profileconfigDetailObj; 
	if(typeof eval("EXPERTUS.SMARTPORTAL.ProfileConfigWidget") == "object"){
		profileconfigDetailObj=eval("EXPERTUS.SMARTPORTAL.ProfileConfigWidget");
	} 
	else{
		profileconfigDetailObj = new EXPERTUS.SMARTPORTAL.ProfileConfigWidget();
		profileconfigDetailObj.setUniqueWidgetId("ProfileConfigService");
		profileconfigDetailObj.setWidgetObject(profileconfigDetailObj);
		profileconfigDetailObj.setWidgetTitle("ProfileConfig Details");
		profileconfigDetailObj.render({id:"ProfileConfigService"});
	}
	}catch(e){
		// to do
	}
}

function getProfileConfig(){
	try{
	var profileconfigDetailObj = new EXPERTUS.SMARTPORTAL.ProfileConfigWidget();
	profileconfigDetailObj.setUniqueWidgetId("CatalogService");
	profileconfigDetailObj.setWidgetObject(profileconfigDetailObj);
	profileconfigDetailObj.setWidgetTitle("ProfileConfig Details");
	profileconfigDetailObj.renderBase({id:"CatalogService"},profileconfigDetailObj);
//	profileconfigDetailObj.render({id:"ProfileConfigService"});
	return profileconfigDetailObj;
	}catch(e){
		// to do
	}
}

$(document).ready(function(){
    if(document.getElementById("ProfileConfigService")!=null)
     {
    	try{
    	callprofileconfigdetail();
    	}catch(e){
			// to do
		}
     }

});