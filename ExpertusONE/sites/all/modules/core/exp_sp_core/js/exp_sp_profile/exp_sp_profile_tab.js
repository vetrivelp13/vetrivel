registernamespace("EXPERTUS.SMARTPORTAL.ProfileTabManager");
EXPERTUS.SMARTPORTAL.ProfileTabManager=function(){
	try{

	this.loadProfileTagDefinitationRequest=function(obj){
		try{
		var request=new Object();
		request.query = new SOAPObject("ProfileTabLoadDataRequest").attr("xsi:type","null");
		request.query.appendChild(new SOAPObject("TagValue").attr("xsi:type","null").val(obj.TagValue));
		var sr = new SOAPRequest("ProfileTabLoadDataRequest", request);
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

$.extend(EXPERTUS.SMARTPORTAL.ProfileTabManager.prototype,EXPERTUS.SMARTPORTAL.AbstractManager);
registernamespace("EXPERTUS.SMARTPORTAL.ProfileTabWidget");
EXPERTUS.SMARTPORTAL.ProfileTabWidget=function(){
	try{

	this.widgetGlobalName	= "EXPERTUS.SMARTPORTAL.ProfileTabWidget";
	this.entProfWidget		= '';
	this.tabXml				= '';
	this.entityId;
	this.profileTag;
	this.parentObj;
	this.loaderDiv;
	this.lovXml;
	this.tabRenderCallback;
	this.tabSaveCallback;


    this.doAction= function(){
    	try{

		var oInput = {'TagValue':this.profileTag};
		var manager = new EXPERTUS.SMARTPORTAL.ProfileTabManager();
		//manager.setActionKey(eval('config.data[1].serviceaction[0].ProfileTabService'));
		manager.initialize(this);
		//manager.setLoaderObj(this.parentObj.uniqueWidgetId+""+this.loaderDiv);
		manager.setCallBack("renderTabs",this.loaderDiv);
		manager.requestXml = manager.loadProfileTagDefinitationRequest(oInput);
		manager.execute();
    	}catch(e){
    		// to do
    	}
    };

    this.initializeTabsProfile = function(paramObj) {
     try{	
    	this.profileTag		= paramObj.profileTag;
    	this.entityId		= paramObj.entityId;
    	this.parentObj		= paramObj.parentObj;
    	this.loaderDiv		= paramObj.loaderDiv;
    	var x=[
		       {'code':'cre_prf_ctg','language':'cre_sys_lng_eng'},
		       {'code':'cre_prf_tag','language':'cre_sys_lng_eng'}
		       ];

		var manager = new EXPERTUS.SMARTPORTAL.ProfileTabManager();
		manager.initialize(this);
		manager.setCallBack("getPickList");
		manager.requestXml = manager.getLovRequest(x);
		manager.execute();
     }catch(e){
 		// to do
 	}
    };

    this.getPickList=function(){
    	try{
		this.lovXml=this.responseXml;

		var oInput = {'TagValue':this.profileTag};
		var manager = new EXPERTUS.SMARTPORTAL.ProfileTabManager();
		//manager.setActionKey(eval('config.data[1].serviceaction[0].ProfileTabService'));
		manager.initialize(this);
		//manager.setLoaderObj(this.parentObj.uniqueWidgetId+""+this.loaderDiv);
		manager.setCallBack("renderTabs",this.loaderDiv);
		manager.requestXml = manager.loadProfileTagDefinitationRequest(oInput);
		manager.execute();
    	}catch(e){
    		// to do
    	}
	};

    this.renderTabs = function(loaderDiv) {
    	try{
    	var ostr='';
    	var liLength = 0;
    	this.tabXml	= this.responseXml;
    	var profileTab = this.parentObj.getUniqueWidgetId()+this.loaderDiv;
    	var instanceName = eval(this.getWidgetInstanceName());
    	//edit course tab fix var - Rajkumar U
    	var tabFound=false;
    	
    	if($(this.tabXml).find("Items>Item").length>0) {

        	if($('#'+profileTab).hasClass("ui-tabs-panel") == false) {
        		$('#'+profileTab).addClass("ui-tabs-panel ui-widget-content ui-corner-bottom ui-tabs-hide");
        	}

        	$("ul.tabNavigation >li > a").click(function() {
        		var spId = $(this).attr("href").split('#');
        		var clickId = spId[1];
        		if(profileTab != clickId) {
        			if($('#'+profileTab).hasClass("ui-tabs-hide") == false) {
        				$('#'+profileTab).addClass("ui-tabs-hide");
        				$('#'+clickId).removeClass("ui-tabs-hide");
        				instanceName.tabNavigationDesign(clickId);
        			}
        		}
        	});

    		if(document.getElementById(this.parentObj.uniqueWidgetId+"tabContainer") == null) {
		    	ostr += "<div class='tabs ui-tabs ui-widget ui-widget-content ui-corner-all' id='"+this.getUniqueWidgetId()+"tabContainer' style='margin-top: 0;'>";
				ostr += "<ul class='tabNavigation ui-tabs-nav ui-helper-reset ui-helper-clearfix ui-widget-header ui-corner-all'>";
				ostr += "<li class='ui-tstate-default ui-corner-top ui-tstate-active ui-tabs-selected'><a href='#' title='"+SMARTPORTAL.t("Profile")+"' id='tabProfile'>"+SMARTPORTAL.t("Profile")+"</a></li>";
				ostr += "</ul>";
				ostr += "<div class='ui-tabs-panel ui-widget-content ui-corner-bottom'><span id='profTabMsgWindow'></span>";
				 if($(this.tabXml).find("Items>Item").length>1) {
					ostr += "<select id='"+this.getUniqueWidgetId()+"profileTab' onchange='"+this.getWidgetInstanceName()+".getSubTabs()' style='width:150px;'>";
					$(this.tabXml).find("Items>Item").each(function() {
			    		ostr += "<option value='"+$(this).find("id").text()+"'>"+$(this).find("profile_cat").text()+"</option>";
			    	});
					ostr += "</select>";
				 } else {
					 ostr += "&nbsp;&nbsp;<b>"+$(this.tabXml).find("profile_cat").text()+"</b></br>";
					 ostr += "<input type='hidden' id='"+this.getUniqueWidgetId()+"profileTab' value='"+$(this.tabXml).find("id").text()+"'>";
				 }

				ostr += "<div id='"+this.getUniqueWidgetId()+''+this.loaderDiv+"'></div>";
				ostr += "</div>";
				ostr += "</div>";
				$("#"+this.parentObj.getUniqueWidgetId()+""+this.loaderDiv).html(ostr);
				this.getSubTabs();
    		} else {
    			liLength = $('ul.tabNavigation').find("li").length;
    			for(var l=0;l<liLength;l++) {
    				if($('ul.tabNavigation li:eq('+l+')').attr('id')=='tabProfile-li')
    				{
    					tabFound=true;
    					break;
    				}
    			}
    			if(tabFound) {
    				$('#tabProfile-li').html("<a href='#"+this.parentObj.uniqueWidgetId+""+this.loaderDiv+"' onclick='"+this.getWidgetInstanceName()+".showTabs()' title='"+SMARTPORTAL.t("Profile")+"'  id='tabProfile'>"+SMARTPORTAL.t("Profile")+"</a>");
    			}else if(liLength>1) {
    				$('ul.tabNavigation li:eq(1)').after("<li class='ui-corner-top ui-tstate-default' style='display: list-item;'><a href='#"+this.parentObj.uniqueWidgetId+""+this.loaderDiv+"' onclick='"+this.getWidgetInstanceName()+".showTabs()' title='"+SMARTPORTAL.t("Profile")+"'  id='tabProfile'>"+SMARTPORTAL.t("Profile")+"</a></li>");
    			} else {
    				$('ul.tabNavigation li').after("<li class='ui-corner-top ui-tstate-default' style='display: list-item;'><a href='#"+this.parentObj.uniqueWidgetId+""+this.loaderDiv+"' onclick='"+this.getWidgetInstanceName()+".showTabs()' title='"+SMARTPORTAL.t("Profile")+"'  id='tabProfile'>"+SMARTPORTAL.t("Profile")+"</a></li>");
    			}
    		}
    	}
    	}catch(e){
    		// to do
    	}
    };

    this.showTabs = function() {
    	try{
    	var ostr = '';
	    ostr += "<div class='ui-tabs-panel ui-widget-content ui-corner-bottom'><span id='profTabMsgWindow'></span>";

	    if($(this.tabXml).find("Items>Item").length>1) {
			ostr += "<select id='"+this.getUniqueWidgetId()+"profileTab' onchange='"+this.getWidgetInstanceName()+".getSubTabs()' style='width:150px;'>";
			$(this.tabXml).find("Items>Item").each(function() {
	    		ostr += "<option value='"+$(this).find("id").text()+"'>"+$(this).find("profile_cat").text()+"</option>";
	    	});
			ostr += "</select>";
	    } else {
	    	ostr += "<b>"+$(this.tabXml).find("profile_cat").text()+"</b></br>";
	    	ostr += "<input type='hidden' id='"+this.getUniqueWidgetId()+"profileTab' value='"+$(this.tabXml).find("id").text()+"'>";
	    }

		ostr += "<div id='"+this.getUniqueWidgetId()+''+this.loaderDiv+"'></div>";
		ostr += "</div>";

		$("#"+this.parentObj.getUniqueWidgetId()+""+this.loaderDiv).html(ostr);
		this.getSubTabs();
    	}catch(e){
    		// to do
    	}
    };

    this.getSubTabs = function() {
    	try{
    	$('#profTabMsgWindow').html('');
    	
    	var liId;
    	var profileTabId = this.parentObj.getUniqueWidgetId()+''+this.loaderDiv;
    	var instanceName = eval(this.getWidgetInstanceName());
    	instanceName.tabNavigationDesign(profileTabId);
    	var clickId;



    	$('ul.tabNavigation li').find("a").each(function () {
    		liId = $(this).attr("href").split('#');
    		var currLiId = liId[1];

    		if(currLiId != '' ) {

    			if(profileTabId == currLiId) {
    				if($('#'+profileTabId).hasClass("ui-tabs-hide")) {
    		    		$('#'+profileTabId).removeClass("ui-tabs-hide");
    		    	}
    			} else {
    				if($('#'+currLiId).hasClass("ui-tabs-hide") == false) {
    					$('#'+currLiId).addClass("ui-tabs-hide");
    				}

    			}
    		}

    	});

    	var profile_tag, rendercallback,savecallback,profile_cat,profile_multirow,profile_readOnly,profile_code;
    	var profileTabId = $("#"+this.getUniqueWidgetId()+"profileTab").find('option').filter(':selected').val();

    	if($("#"+this.getUniqueWidgetId()+"profileTab").attr("type") == "hidden") {
    		profileTabId	= $("#"+this.getUniqueWidgetId()+"profileTab").val();
    	} else {
    		profileTabId = $("#"+this.getUniqueWidgetId()+"profileTab").find('option').filter(':selected').val();
    	}
    	var crobj=eval(this.getWidgetInstanceName());
    	$(this.tabXml).find("Items>Item").each(function() {
    		if($(this).find("id").text() == profileTabId) {
    			profile_tag 			= $(this).find("profile_tag").text();
    			crobj.tabRenderCallback = $(this).find("rendercallback").text();
    			crobj.tabSaveCallback	= $(this).find("savecallback").text();
    			profile_cat				= $(this).find("profile_cat").text();
    			profile_code			= $(this).find("code").text();
    			profile_multirow		= $(this).find("is_multi_row").text();
    			profile_readOnly		= $(this).find("readonly").text();
    		}
    	});

    	if(profile_code != undefined && profile_code != '') {
		    	this.entProfWidget.initializeEntityProfile({
		    			entityId		: this.entityId,
		    			categoryId		: this.getLookUpId("cre_prf_ctg",profile_cat),
		    			funcId			: this.getLookUpCode("cre_prf_tag",this.profileTag) ,
		    			parentObj		: this,
		    			loaderObj		: this.loaderDiv,
		    			type			: profile_code,
		    			renderCallback	: 'defaultRenderCallback',
		    			saveCallBack	: 'defaultSaveCallback',
		    			multirow		: ((profile_multirow == 'Y') ? "true" : "false")
		    			// readOnly		: ((profile_readOnly == 'Y') ? true : false)
		    	});
		    	this.entProfWidget.render();
    	} else {
    		this.defaultRenderCallback();
    	}
    	}catch(e){
    		// to do
    	}
    };


    this.tabNavigationDesign = function(tabNavId) {
    	try{
    	var liId;

    	$('ul.tabNavigation li').each(function () {
    		liId = $(this).find("a").attr("href").split('#');
    		var currLiId;
    		currLiId = liId[1];
    		
    		if(currLiId != '' ) {

    			if(tabNavId == currLiId) {

    				if($(this).hasClass("ui-state-default")) {
    					$(this).addClass("ui-state-active");
    					$(this).addClass("ui-tabs-selected");
    				} else {
    					$(this).removeClass("ui-state-active");
    					$(this).removeClass("ui-tabs-selected");

    					$(this).addClass("ui-state-active");
    					$(this).addClass("ui-tabs-selected");
    				}
    			} else {
					$(this).removeClass("ui-state-active");
					$(this).removeClass("ui-tabs-selected");
				}
    		}
    	});
    	}catch(e){
    		// to do
    	}
    };



    this.defaultRenderCallback = function() {
    	try{
    	if(this.tabRenderCallback != '') {
    		var vCallbackfn =  this.parentObj.getWidgetInstanceName()+'.'+this.tabRenderCallback+'()';
    		eval(vCallbackfn);
    	}
    	}catch(e){
    		// to do
    	}
    };

    this.defaultSaveCallback = function() {
    	try{
    	if(this.tabSaveCallback != '') {
    		var vSaveCallbackfn =  this.parentObj.getWidgetInstanceName()+'.'+this.tabSaveCallback+'()';
    		eval(vSaveCallbackfn);
    	}
    	this.defCallBack();
    	}catch(e){
    		// to do
    	}
    };
    
    this.defCallBack = function(act) {
    	try{
    	var retMsg 	= $(this.entProfWidget.responseXml).find("Item>msg").text();    	
    	var msgHtml = '';
    	if(retMsg == 'updated') {
    		msgHtml += '<div class="sp_msgbox">'+SMARTPORTAL.t("Profile updated successfully.")+'</div>';
    	} else {
    		msgHtml += '<div class="sp_msgbox">'+SMARTPORTAL.t("Profile saved successfully.")+'</div>';    		
    	}
    	
    	$("#profTabMsgWindow").html(msgHtml);    	
    	}catch(e){
    		// to do
    	}
    };

    this.getLookUpCode = function(str1, str2){
    	try{
		var code;
		$(this.lovXml).find("ListItem").each(function(){

			if($(this).attr("code")==str1){
				$(this).find("Items>Item").each(function(){
					if($(this).find("Name").text()==str2){
						code = $(this).find("Code").text();
						return false;
					}
				});
			}
		});
		return code;
    	}catch(e){
    		// to do
    	}
	};

	 this.getLookUpId = function(str1, str2){
		 try{
			var id;
			$(this.lovXml).find("ListItem").each(function(){

				if($(this).attr("code")==str1){
					$(this).find("Items>Item").each(function(){
						if($(this).find("Name").text()==str2){
							id = $(this).find("Id").text();
							return false;
						}
					});
				}
			});
			return id;
		 }catch(e){
				// to do
			}
		};
	}catch(e){
		// to do
	}
};

$.extend(EXPERTUS.SMARTPORTAL.ProfileTabWidget.prototype,EXPERTUS.SMARTPORTAL.AbstractDetailsWidget);

function callProfileTabDetail(passParam){
	try{
	var oProfileTabWidget;
	if(typeof eval("EXPERTUS.SMARTPORTAL.ProfileTabWidget") == "object"){
		oProfileTabWidget=eval("EXPERTUS.SMARTPORTAL.ProfileTabWidget");
	} else {
		oProfileTabWidget=new EXPERTUS.SMARTPORTAL.ProfileTabWidget();
	}
		oProfileTabWidget.setUniqueWidgetId('ProfileTabService');
		oProfileTabWidget.setWidgetObject(oProfileTabWidget);
		oProfileTabWidget.setWidgetTitle("Profile Tab Details");
		oProfileTabWidget.renderBase({id:'ProfileTabService'},oProfileTabWidget);
		oProfileTabWidget.initializeTabsProfile(passParam);
		oProfileTabWidget.entProfWidget = getEntityProfile();
		//oProfileTabWidget.doAction();
	}catch(e){
		// to do
	}
}

