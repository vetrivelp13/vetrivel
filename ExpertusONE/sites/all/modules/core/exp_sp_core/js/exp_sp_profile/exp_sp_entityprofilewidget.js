registernamespace("EXPERTUS.SMARTPORTAL.EntityProfileManager");
EXPERTUS.SMARTPORTAL.EntityProfileManager=function(){
	try{
    this.isLnr=false;

    this.generateCreateRequest = function(obj){
    	try{
        var request=new Object();
        request.query = new SOAPObject("InsertProfile").attr("xsi:type","null");
        request.query.appendChild(new SOAPObject("list_id").attr("xsi:type","null").val(obj.list_id));
        request.query.appendChild(new SOAPObject("parent_id").attr("xsi:type","null").val(obj.parent_id));
        request.query.appendChild(new SOAPObject("level").attr("xsi:type","null").val(obj.level));
        request.query.appendChild(new SOAPObject("name").attr("xsi:type","null").val(obj.name));
        request.query.appendChild(new SOAPObject("root").attr("xsi:type","null").val(obj.root));
        if(obj.description!=undefined)
            request.query.appendChild(new SOAPObject("description").attr("xsi:type","null").val(obj.description));
        request.query.appendChild(new SOAPObject("attr1").attr("xsi:type","null").val(obj.attr1));
        request.query.appendChild(new SOAPObject("attr2").attr("xsi:type","null").val(obj.attr2));
        request.query.appendChild(new SOAPObject("attr3").attr("xsi:type","null").val(obj.attr3));
        request.query.appendChild(new SOAPObject("attr4").attr("xsi:type","null").val(obj.attr4));
        request.query.appendChild(new SOAPObject("type").attr("xsi:type","null").val(obj.type));
        request.query.appendChild(new SOAPObject("is_active").attr("xsi:type","null").val(obj.is_active));
        var sr = new SOAPRequest("InsertProfile", request);
        sr="<?xml version='1.0' encoding='UTF-8'?>"+sr;
        sr=sr.toString();
        return sr;
    	}catch(e){
    		// to do
    	}
    };

    this.generateDisplayRequest = function(obj){
    	try{
        var request=new Object();
        request.query = new SOAPObject("GetProfileFields").attr("xsi:type","null");
        request.query.appendChild(new SOAPObject("entity_id").attr("xsi:type","null").val(obj.entity_id));
        request.query.appendChild(new SOAPObject("category_id").attr("xsi:type","null").val(obj.category_id));
        request.query.appendChild(new SOAPObject("id2").attr("xsi:type","null").val(obj.id2));
        request.query.appendChild(new SOAPObject("function_id").attr("xsi:type","null").val(obj.function_id));
        request.query.appendChild(new SOAPObject("type").attr("xsi:type","null").val(obj.type));
        var sr = new SOAPRequest("GetProfileFields", request);
        sr="<?xml version='1.0' encoding='UTF-8'?>"+sr;
        sr=sr.toString();
        return sr;
    	}catch(e){
    		// to do
    	}
    };
    /*
    this.generateQueryString=function(){
        var qstr = "";
        var widgetObj = this.getwidgetObject();
        if(this.getActionKey()!=null && this.getActionKey()!=undefined && this.getActionKey().length>0)
            qstr += qstr.length>0?"&actionkey="+this.getActionKey():"?actionkey="+this.getActionKey();
        if(config.data[2].licensekey[0].admin_licensekey!=null && config.data[2].licensekey[0].admin_licensekey!=undefined && config.data[2].licensekey[0].admin_licensekey!=''){
            qstr += qstr.length>0?"&licensekey="+config.data[2].licensekey[0].admin_licensekey:"?licensekey="+config.data[2].licensekey[0].admin_licensekey;
            var srvno=eval('config.data[3].SCServiceno[0].'+widgetObj.getUniqueWidgetId());
            qstr += srvno==undefined?"&SCServiceno="+config.data[3].SCServiceno[0].all:"&SCServiceno="+srvno;
        }

        return qstr;
     };
     */
    this.setEndPointURL=function(url)
    {
    	try{
		 var widgetObj = this.getwidgetObject();
		 this.url = url + this.generateQueryString();
    	}catch(e){
    		// to do
    	}
    };
	}catch(e){
		// to do
	}
};

$.extend(EXPERTUS.SMARTPORTAL.EntityProfileManager.prototype,EXPERTUS.SMARTPORTAL.AbstractManager);

registernamespace("EXPERTUS.SMARTPORTAL.EntityProfileWidget");
EXPERTUS.SMARTPORTAL.EntityProfileWidget=function(){
	try{
    this.widgetGlobalName="EXPERTUS.SMARTPORTAL.EntityProfileWidget";
    this.loaderObj;
    this.divId;
    this.entityId;
    this.categoryId;
    this.funcId;
    this.objFields;
    this.parentObj;
    this.renderCallback;
    this.saveCallBack;
    this.multirow;
    this.initialXml;
    this.idx;
    this.rghtCol;
    this.type;
    this.id2;
    this.readOnly;
    this.Collapsed;
    this.isLnr=false;
    this.random;

    this.render=function(serviceattributes)
    {
    	try{
        this.renderBase({id:'EntityProfileService'},this);
        if(serviceattributes != null && serviceattributes.isLnr){
            this.setEndPointURL(resource.admin_site_url+'/sites/all/SmartConnect/SmartConnect_Server.php');
        }
        var params = { entity_id:this.entityId, id2:this.id2, category_id:this.categoryId, function_id:this.funcId, type:this.type };
        var manager = new EXPERTUS.SMARTPORTAL.EntityProfileManager();
        manager.isLnr=this.isLnr;
        manager.initialize(this);
        if(serviceattributes != null && serviceattributes.isLnr){
            manager.isLnr = true;
        }
        manager.setLoaderObj(this.loaderObj);
        manager.setActionKey('EntityProfile');
        manager.setEndPointURL(this.getEndPointURL());
        manager.requestXml = manager.generateDisplayRequest(params);
        manager.setCallBack("renderFields");
        manager.execute();
    	}catch(e){
    		// to do
    	}
    };

    this.renderHtml = function(){
    	try{

    	}catch(e){
    		// to do
    	}

    };

    this.renderRow = function(){
    	try{
        this.idx++;
        var html = '<tr id="row'+ this.idx +'"><td class="border_rght"><table class="tbl_profilefields" width="100%">';
        var widObj = this;
        var colCnt=0;
        var arrLDates = new Array();
        var arrLTextbox = new Array();
        var arrLTextarea = new Array();
        var arrLRadio = new Array();
        var arrLCheck = new Array();
        var arrLListbox = new Array();
        var arrLDd = new Array();
        var arrLTree = new Array();

        var arrRDates = new Array();
        var arrRTextbox = new Array();
        var arrRTextarea = new Array();
        var arrRRadio = new Array();
        var arrRCheck = new Array();
        var arrRListbox = new Array();
        var arrRDd = new Array();
        var arrRTree = new Array();

        var arr;
        var i=0,j=0, arrStr;
        var serializer = new XMLSerializer();
        $(this.objFields).find("field").each(function(){
            if($(this).find("display").text()=="Date Picker"){
                if($(this).find("display_on").text()=="L")
                    eval("arr = arrLDates");
                else
                    eval("arr = arrRDates");
            }
            else if($(this).find("display").text()=="Textbox"){
                if($(this).find("display_on").text()=="L")
                    eval("arr = arrLTextbox");
                else
                    eval("arr = arrRTextbox");
            }
            else if($(this).find("display").text()=="Textarea"){
                if($(this).find("display_on").text()=="L")
                    eval("arr = arrLTextarea");
                else
                    eval("arr = arrRTextarea");
            }
            else if($(this).find("display").text()=="Radio Button"){
                if($(this).find("display_on").text()=="L")
                    eval("arr = arrLRadio");
                else
                    eval("arr = arrRRadio");
            }
            else if($(this).find("display").text()=="Checkbox"){
                if($(this).find("display_on").text()=="L")
                    eval("arr = arrLCheck");
                else
                    eval("arr = arrRCheck");
            }
            else if($(this).find("display").text()=="Listbox"){
                if($(this).find("display_on").text()=="L")
                    eval("arr = arrLListbox");
                else
                    eval("arr = arrRListbox");
            }
            else if($(this).find("display").text()=="Dropdown"){
                if($(this).find("display_on").text()=="L")
                    eval("arr = arrLDd");
                else
                    eval("arr = arrRDd");
            }
            else if($(this).find("display").text()=="Tree"){
                if($(this).find("display_on").text()=="L")
                    eval("arr = arrLTree");
                else
                    eval("arr = arrRTree");
            }

            arr[arr.length] = new Array();
            arr[arr.length-1][0] = $(this).find(">id").text()+"_"+ widObj.idx;
            arr[arr.length-1][1] = $(this);

        });



        html += this.renderDisplayField(arrLTextbox);
        html += this.renderDisplayField(arrLTextarea);
        html += this.renderDisplayField(arrLDates);
        html += this.renderDisplayField(arrLRadio);
        html += this.renderDisplayField(arrLCheck);
        html += this.renderDisplayField(arrLListbox);
        html += this.renderDisplayField(arrLDd);
        html += this.renderTree(arrLTree);
        html += '</table></td>';

        if(arrRCheck.length==0 && arrRDates.length==0 && arrRDd.length==0
                && arrRListbox.length==0 && arrRRadio.length==0 && arrRTextarea.length==0
                && arrRTextbox.length==0 && arrRTree.length==0 ){
            this.rghtCol = false;
            $("#tbl_accessprofile th:eq(1)").remove();
            $("#tbl_accessprofile th:eq(0)").css("width","auto");
        }

        if(this.rghtCol){
            html += '<td>';
            html += '<table width="100%">';
            html += this.renderRghtDisplayField(arrRTextbox);
            html += this.renderRghtDisplayField(arrRTextarea);
            html += this.renderRghtDisplayField(arrRDates);
            html += this.renderRghtDisplayField(arrRRadio);
            html += this.renderRghtDisplayField(arrRCheck);
            html += this.renderRghtDisplayField(arrRListbox);
            html += this.renderRghtDisplayField(arrRDd);
            html += this.renderTree(arrRTree);
            html += '</table>';
            html += '</td>';
        }
        html += '</tr>';
        if(($(this.objFields).find("field").length > 0))
        {
            if(this.rghtCol){
                html += '<tr id="row'+ this.idx +'_action"><td class="border_bott border_rght"></td>';
            }else{
                html += '<tr id="row'+ this.idx +'_action"><td class="border_bott"></td>';
            }
            html += '<td class="border_bott" style="align:right;">';
            html += '<div id="savedir_' + this.idx + '" class="savediv">';
                if((!this.readOnly) && (!this.isLnr)){
                    if(this.multirow=="true")
                    	html += '<img style="cursor:pointer;" title="Cancel" src="'+themepath+'/images/exp_sp_icon_16x16_Undo.gif" onclick="'+this.getWidgetInstanceName()+'.cancelAddRow(\'row'+this.idx+'\');"  />';
                }
            html += '</div>';

            html += '</td></tr>';
        }
        else
        {
            html += '<tr><td style="padding-left:10px; border-bottom:0px;">'+ SMARTPORTAL.t('No Results Found.')+'</td></tr>';
        }

        if ($('#multi_row_no_profile_records_msg').length > 0) {
        	$('#multi_row_no_profile_records_msg').remove();
        }

        $("#tbl_accessprofile").append(html);
        if(($(this.objFields).find("field").length  == 0))
        {
        $("#row"+this.idx+" td").css("border-bottom","0px");
        }
        if(!this.rghtCol)
            $("#row"+this.idx+" td:eq(0)").removeClass("border_rght");

        this.setFields(arrLDates, arrLTree);
        this.setFields(arrRDates, arrRTree);
        var arrTree = new Array();
        arrTree[0] = arrLTree;
        arrTree[1] = arrRTree;
        return arrTree;
    	}catch(e){
    		// to do
    	}
    };

    this.renderRowEdit = function(){
    	try{
        this.idx++;
        var html = '<tr id="row'+ this.idx +'"><td class="border_rght"><table class="tbl_profilefields" width="100%">';
        var widObj = this;
        var colCnt=0;
        var arrLDates = new Array();
        var arrLTextbox = new Array();
        var arrLTextarea = new Array();
        var arrLRadio = new Array();
        var arrLCheck = new Array();
        var arrLListbox = new Array();
        var arrLDd = new Array();
        var arrLTree = new Array();

        var arrRDates = new Array();
        var arrRTextbox = new Array();
        var arrRTextarea = new Array();
        var arrRRadio = new Array();
        var arrRCheck = new Array();
        var arrRListbox = new Array();
        var arrRDd = new Array();
        var arrRTree = new Array();

        var arr;
        var i=0,j=0, arrStr;
        var serializer = new XMLSerializer();
        $(this.objFields).find("field").each(function(){
            if($(this).find("display").text()=="Date Picker"){
                if($(this).find("display_on").text()=="L")
                    eval("arr = arrLDates");
                else
                    eval("arr = arrRDates");
            }
            else if($(this).find("display").text()=="Textbox"){
                if($(this).find("display_on").text()=="L")
                    eval("arr = arrLTextbox");
                else
                    eval("arr = arrRTextbox");
            }
            else if($(this).find("display").text()=="Textarea"){
                if($(this).find("display_on").text()=="L")
                    eval("arr = arrLTextarea");
                else
                    eval("arr = arrRTextarea");
            }
            else if($(this).find("display").text()=="Radio Button"){
                if($(this).find("display_on").text()=="L")
                    eval("arr = arrLRadio");
                else
                    eval("arr = arrRRadio");
            }
            else if($(this).find("display").text()=="Checkbox"){
                if($(this).find("display_on").text()=="L")
                    eval("arr = arrLCheck");
                else
                    eval("arr = arrRCheck");
            }
            else if($(this).find("display").text()=="Listbox"){
                if($(this).find("display_on").text()=="L")
                    eval("arr = arrLListbox");
                else
                    eval("arr = arrRListbox");
            }
            else if($(this).find("display").text()=="Dropdown"){
                if($(this).find("display_on").text()=="L")
                    eval("arr = arrLDd");
                else
                    eval("arr = arrRDd");
            }
            else if($(this).find("display").text()=="Tree"){
                if($(this).find("display_on").text()=="L")
                    eval("arr = arrLTree");
                else
                    eval("arr = arrRTree");
            }

            arr[arr.length] = new Array();
            arr[arr.length-1][0] = $(this).find(">id").text()+"_"+ widObj.idx;
            arr[arr.length-1][1] = $(this);

        });



        html += this.renderDisplayField(arrLTextbox);
        html += this.renderDisplayField(arrLTextarea);
        html += this.renderDisplayField(arrLDates);
        html += this.renderDisplayField(arrLRadio);
        html += this.renderDisplayField(arrLCheck);
        html += this.renderDisplayField(arrLListbox);
        html += this.renderDisplayField(arrLDd);
        html += this.renderTree(arrLTree);
        html += '</table></td>';

        if(arrRCheck.length==0 && arrRDates.length==0 && arrRDd.length==0
                && arrRListbox.length==0 && arrRRadio.length==0 && arrRTextarea.length==0
                && arrRTextbox.length==0 && arrRTree.length==0 ){
            this.rghtCol = false;
            $("#tbl_accessprofile th:eq(1)").remove();
            $("#tbl_accessprofile th:eq(0)").css("width","auto");
        }

        if(this.rghtCol){
            html += '<td>';
            html += '<table width="100%">';
            html += this.renderRghtDisplayField(arrRTextbox);
            html += this.renderRghtDisplayField(arrRTextarea);
            html += this.renderRghtDisplayField(arrRDates);
            html += this.renderRghtDisplayField(arrRRadio);
            html += this.renderRghtDisplayField(arrRCheck);
            html += this.renderRghtDisplayField(arrRListbox);
            html += this.renderRghtDisplayField(arrRDd);
            html += this.renderTree(arrRTree);
            html += '</table>';
            html += '</td>';
        }
        html += '</tr>';
        if(($(this.objFields).find("field").length > 0))
        {
            if(this.rghtCol){
                html += '<tr id="row'+ this.idx +'_action"><td class="border_bott border_rght"></td>';
            }else{
                html += '<tr id="row'+ this.idx +'_action"><td class="border_bott"></td>';
            }
            html += '<td class="border_bott" style="align:right;">';
            html += '<div id="savedir_' + this.idx + '" class="savediv">';
                if((!this.readOnly) && (!this.isLnr)){
                    if(this.multirow=="true")
                    	html += '<img style="cursor:pointer;" title="Cancel" src="'+themepath+'/images/exp_sp_icon_16x16_Undo.gif" onclick="'+this.getWidgetInstanceName()+'.cancelAddRow(\'row'+this.idx+'\');"  />';
                }
            html += '</div>';

            html += '</td></tr>';
        }
        else
        {
            html += '<tr><td style="padding-left:10px; border-bottom:0px;">'+ SMARTPORTAL.t('No Results Found.')+'</td></tr>';
        }

        if ($('#multi_row_no_profile_records_msg').length > 0) {
        	$('#multi_row_no_profile_records_msg').remove();
        }

        $("#tbl_accessprofile").append(html);
        if(($(this.objFields).find("field").length  == 0))
        {
        $("#row"+this.idx+" td").css("border-bottom","0px");
        }
        if(!this.rghtCol)
            $("#row"+this.idx+" td:eq(0)").removeClass("border_rght");

        var arrTree = new Array();
        arrTree[0] = arrLTree;
        arrTree[1] = arrRTree;
        return arrTree;
    	}catch(e){
    		// to do
    	}
    };

    this.renderFields = function(){
    	try{
    	$("#"+this.divId).html('');
        $("#tbl_accessprofile").remove();
        var html = "<table id='tbl_accessprofile' cellpadding='0' width='100%'>";

//      if(this.initialXml==undefined)
//          this.initialXml = this.responseXml;
        this.objFields = $(this.responseXml).find("fields");


        html += '</table>';
        if(($(this.responseXml).find("fields>field").length > 0))
        {
            html += '<div class="savediv">';
            if(!this.readOnly){
                if(this.multirow=="true") {
                	html += '<img style="cursor:pointer;" title="Add Entry" src="'+themepath+'/images/add.jpeg" onclick="'+this.getWidgetInstanceName()+'.renderRow();"  />&nbsp;&nbsp;&nbsp;';
                }

                if ((!this.isLnr)) {
            		html += '<img style="cursor:pointer;" title="'+SMARTPORTAL.t("Save")+'" src="'+themepath+'/images/exp_sp_icon_16x16_Save.gif" onclick="'+this.getWidgetInstanceName()+'.saveEntityProfileTagging();"  />&nbsp;&nbsp;&nbsp;';
            	}

            }
            html += '</div>';
        }

        $("#"+this.divId).html(html);
        if($(this.responseXml).find("StoredValues>Item").length==0) {
        	if (this.multirow == "false" || this.multirow ==false) {
        		this.renderRow();
        	} else {
        		html = '<tr id="multi_row_no_profile_records_msg"><td style="padding-left:10px; padding-top: 20px; border-bottom:0px;">'+ SMARTPORTAL.t('No Results Found.')+'</td></tr>';
        		$("#tbl_accessprofile").append(html);
        	}
        } else
            this.renderEditRows();

        if(this.readOnly){
            $("#tbl_accessprofile input:all").attr("disabled","disabled");
            $("#tbl_accessprofile select:all").attr("disabled","disabled");
        }
        if(this.Collapsed){
            $("#tbl_accessprofile li:all").addClass('closed');
        }
        /* Callback after rendering the profile fields */
        var readInd = this.readOnly;

        $('#tbl_accessprofile > tbody > tr > td > .tbl_profilefields > tbody > tr > td > div > ul > li:first').removeClass('closed');
        $('#tbl_accessprofile > tbody > tr > td > .tbl_profilefields > tbody > tr > td > div > ul > li:first').addClass('open');
        $('#tbl_accessprofile > tbody > tr > td > .tbl_profilefields > tbody > tr > td > div > ul > li > a:first').css('cursor','default');
        $('#tbl_accessprofile > tbody > tr > td > .tbl_profilefields  > tbody > tr > td > div > ul > li:first > ul > li').each(function(){
            $(this).click(function(){
                var cur = $(this);
                //close all the nodes
                $('#tbl_accessprofile > tbody > tr > td > .tbl_profilefields > tbody > tr > td > div > ul > li:first > ul > li').each(function(){

                    if(cur.attr("id") != $(this).attr("id")){
                        if($(this).hasClass('open')){
                            $(this).removeClass('open');
                            $(this).addClass('closed');
                        }
                    }
                });
                if(readInd){
                    if(cur.hasClass('open')){
                        cur.removeClass('open');
                        cur.addClass('closed');
                    } else {
                        cur.removeClass('closed');
                        cur.addClass('open');
                    }
                    return false;
                }
               // $('html, body').animate({ scrollTop: $('#tbl_accessprofile').OFFSET().top }, 'slow');
            })
        });

        eval(this.parentObj.getObjectName()+"."+this.renderCallback+"()");
    	}catch(e){
    		// to do
    	}
    };

    this.renderEditRows = function(){
    	try{
        var widObj = this;
        var item;
        var idx=0;
        $(this.responseXml).find("StoredValues>Item").each(function(){
            var arrTree = widObj.renderRowEdit();
            item = this;
            $(widObj.objFields).find("field").each(function(){
                if($(this).find("display_on").text()=="R")
                    var val = $(item).find("custom0").text();
                else
                    var val = $(item).find("col"+$(this).find("column").text()).text();
                var id = $(this).find(">id").text()+"_"+idx;
                var display = $(this).find("display").text();
                if(display=="Tree"){
                    if($(this).find("display_on").text()=="L")
                        widObj.selectTreeNodes(id,val,arrTree[0]);
                    else
                        widObj.selectTreeNodes(id,val,arrTree[1]);
                }else if(display=="Radio Button" || display=="Checkbox"){
                    widObj.setCheckItems(id, val);
                }else if(display=="Listbox"){
                    widObj.setSelectItems(id, val);
                }
                else
                    $("#"+id).val(val);
            });

            if ($('#savedir_'+idx).length > 0) {
            	var html="";
            	if((!widObj.readOnly) && (!widObj.isLnr)){
            		//html += '<img style="cursor:pointer;" title="'+SMARTPORTAL.t("Save")+'" src="'+themepath+'/images/exp_sp_icon_16x16_Save.gif" onclick="'+widObj.getWidgetInstanceName()+'.saveEntityProfileTagging();"  />&nbsp;&nbsp;&nbsp;';
            		if(widObj.multirow=="true")
            			html += '<img style="cursor:pointer;" title="Delete" src="'+themepath+'/images/exp_sp_icon_16x16_Delete.gif" onclick="'+widObj.getWidgetInstanceName()+'.deleteRow(\'row'+idx+'\');"  />';
            	}
            	$('#savedir_'+idx).html(html);
            }

            idx++;
        });
    	}catch(e){
    		// to do
    	}
    };

    this.selectTreeNodes = function(id,val, arrTree){
    	try{
        var tree;
        for(var i=0;i<arrTree.length;i++){
            if(arrTree[i][0]==id){
                tree = arrTree[i][1];
                break;
            }
        }
        var serializer = new XMLSerializer();
        var treeText = serializer.serializeToString($(tree).find("values>profile>root")[0]);
        $("#Ent_"+this.random+"_"+id+"_Tree").tree({
            callback:{
                check_move: function(node, ref_node, type, objTree, rb){
                    return false;
                }
            },
            selected: val.split(","),
            data:{
                type:"xml_flat",
                opts: {
                    'static': treeText
                }
            },
            type: {
                "default": {
                    draggable : false
                }
            },
            ui: {
                theme_name : "checkbox",
                animation: 200,
                selected_parent_close : false
            },
            plugins : {
                   "checkbox" : { }
            }
        });
        $("#Ent_"+this.random+"_"+id+"_Tree").find("a:eq(0)").css(
                {
                    "font-weight":"bold",
                    "color":"#4F4F7A"
                }
        );
        var tree = $.tree.reference("#Ent_"+this.random+"_"+id+"_Tree");
        if(this.readOnly)
            tree.lock(true);
    	}catch(e){
    		// to do
    	}
    };

    this.setCheckItems = function(id, values){
    	try{
        var arr = values.split(",");
        var widObj = this;
        $("input[name='"+id+"']").each(function(){
            if(widObj.checkInArray(arr,$(this).val())){
                $(this).attr("checked","checked");
            }
        });
    	}catch(e){
    		// to do
    	}
    };

    this.setSelectItems = function(id, values){
    	try{
        var arr = values.split(",");
        var widObj = this;

        $("#"+id+" option").each(function(){
            if(widObj.checkInArray(arr,$(this).val())){

                $(this).attr("selected","selected");
            }
        });
    	}catch(e){
    		// to do
    	}
    };

    this.checkInArray = function(arr, val){
    	try{
        for(var i=0;i<arr.length;i++){
            if(arr[i]==val)
                return true;
        }
        return false;
    	}catch(e){
    		// to do
    	}
    };

    this.setFields = function(arrDates, arrTree){
    	try{
        var titletxt=SMARTPORTAL.t("Click to choose a date");
        for(var i=0;i<arrDates.length;i++){
            $("#"+arrDates[i][0]).datepicker({  //New date picker plugin added by Sadeesh
                  duration: '',
                  showTime: false,
                  constrainInput: false,
                  stepMinutes: 5,
                  stepHours: 1,
                  time24h: true,
                  dateFormat: "mm/dd/yy",
                  buttonImage: themepath+'/images/calendar_icon.JPG',
                  buttonImageOnly: true,
                  firstDay: 1,
                  showOn: 'both',
                  showButtonPanel: true
            });
        }
        var serializer = new XMLSerializer();
        for(var i=0;i<arrTree.length;i++){
            var treeText = serializer.serializeToString($(arrTree[i][1]).find("values>profile>root")[0]);
            $("#Ent_"+this.random+"_"+arrTree[i][0]+"_Tree").tree({
                callback:{
                    check_move: function(node, ref_node, type, objTree, rb){
                        return false;
                    }
                },
                data:{
                    type:"xml_flat",
                    opts: {
                        'static': treeText
                    }
                },
                type: {
                    "default": {
                        draggable : false
                    }
                },
                ui: {
                    theme_name : "checkbox",
                    animation: 200,
                    selected_parent_close : false
                },
                plugins : {
                       "checkbox" : { }
                }
            });
            $("#Ent_"+this.random+"_"+arrTree[i][0]+"_Tree").find("a:eq(0)").css(
                    {
                        "font-weight":"bold",
                        "color":"#4F4F7A"
                    }
            );
        }
    	}catch(e){
    		// to do
    	}
    };


    this.renderTree = function(arrTree){
    	try{
        var html ="<tr><td colspan='2' style='padding:10px 0;'>";
        for(i=0;i<arrTree.length;i++){
            if(i!=0 && i%2==0){
                html += '</tr></tr><tr><td colspan="2">';
            }
            html += '<div id="Ent_'+this.random+"_"+arrTree[i][0]+'_Tree" style="width:50%;float:left;"></div>';
        }
        html += '<div class="clearBoth"></div></td></tr>';
        return html;
    	}catch(e){
    		// to do
    	}
    };
    this.renderRghtDisplayField = function(arrUI){
    	try{
        var html ="";
        for(var i=0;i<arrUI.length;i++)
            html += "<tr><td class='label' >"+ $(arrUI[i][1]).find(">name").text()+"<br />"+this.getDisplayField(arrUI[i][0],arrUI[i][1]) + "</td></tr>";
        return html;
    	}catch(e){
    		// to do
    	}
    };

    this.renderDisplayField = function(arrUI){
    	try{
        var html ="";
        for(var i=0;i<arrUI.length;i++)
            html += "<tr><td class='label' >"+ $(arrUI[i][1]).find(">name").text()+"</td><td class='field' >"+this.getDisplayField(arrUI[i][0],arrUI[i][1]) + "</td></tr>";
        return html;
    	}catch(e){
    		// to do
    	}
    };

    this.getDisplayField = function(id, arrUI){
    	try{
        var type = $(arrUI).find("type").text();
        var display = $(arrUI).find("display").text();
        var values = $(arrUI).find("values");
        var label = this.getLabel($(arrUI.find(">name")).text());

        var html = "";
        if(display=="Date Picker"){
            html = '<input class="profile_field_'+label+'" type="text" id="'+id+'" value="" />';
            //html += "<img src='"+themepath+"/images/calendar_icon.JPG' class='calender-img' style='padding-top:0px;' title='"+SMARTPORTAL.t("Click to select start date.")+"'/>";
        }else if(display == "Textbox"){
            html = '<input class="profile_field_'+label+'" type="text" id="'+id+'" value="" />';
        }else if(display=="Textarea"){
            html = '<textarea class="profile_field_'+label+'" id="'+id+'"></textarea>';

        }else if(display == "Dropdown"){
            html = '<select class="profile_field_'+label+'" id="'+id+'">';
            $(values).find("Item").each(function(){
                html += '<option value="'+$(this).find("id").text()+'">'+$(this).find("name").text()+'</option>';
            });
            html += '</select>';
        }else if(display == "Listbox"){
            html = '<select class="profile_field_'+label+'" id="'+id+'" multiple="multiple" size="3">';
            $(values).find("Item").each(function(){
                html += '<option value="'+$(this).find("id").text()+'">'+$(this).find("name").text()+'</option>';
            });
            html += '</select>';
        }else if (display == "Tree"){
            html = '<div class="profile_field_'+label+'" id="'+id+'"></div>';
        }else if (display == "Radio Button"){
            $(values).find("Item").each(function(){
                html += '<input class="profile_field_'+label+'" name="'+id+'" type="radio" value="'+$(this).find("id").text()+'" />'+$(this).find("name").text() + '<br />';
            });
        }else if (display == "Checkbox"){
            $(values).find("Item").each(function(){
                html += '<input class="profile_field_'+label+'" name="'+ id +'" type="checkbox" id="'+$(this).find("id").text()+'" value="'+$(this).find("id").text()+'"/> '+$(this).find("name").text() + '<br/>';
            });
        }
        return html;
    	}catch(e){
    		// to do
    	}
    };

    this.getLabel = function(str){
    	try{
        return str.replace(" ","_");
    	}catch(e){
    		// to do
    	}
    };

    this.deleteRow = function(id){
    	try{
        if(id!=undefined){
            if(confirm("Are you sure you want to delete this profile?")){
                $("#"+id).remove();
                $("#"+id+'_action').remove();
                this.saveEntityProfileTagging();
            }

            var i;
            for(i=0;i<=this.idx;i++){
                if($("#row"+i).length>0)
                    break;
            }
            if (i > this.idx) {
        		var html = '<tr id="multi_row_no_profile_records_msg"><td style="padding-left:10px; padding-top: 20px; border-bottom:0px;">'+ SMARTPORTAL.t('No Results Found.')+'</td></tr>';
        		$("#tbl_accessprofile").append(html);
            }
        }
    	}catch(e){
    		// to do
    	}
    };

    this.cancelAddRow = function(id){
    	try{
        if(id!=undefined){
                $("#"+id).remove();
                $("#"+id+'_action').remove();

	        var i;
	        for(i=0;i<=this.idx;i++){
	            if($("#row"+i).length>0)
	                break;
	        }
	        if (i > this.idx) {
	    		var html = '<tr id="multi_row_no_profile_records_msg"><td style="padding-left:10px; padding-top: 20px; border-bottom:0px;">'+ SMARTPORTAL.t('No Results Found.')+'</td></tr>';
	    		$("#tbl_accessprofile").append(html);
	        }
        }
    	}catch(e){
    		// to do
    	}
    };

    this.saveEntityProfileTagging = function(){
    	try{
        var widObj = this;
        var request=new Object();
        request.query = new SOAPObject("SaveEntityProfile").attr("xsi:type","null");
        request.query.appendChild(new SOAPObject("entity_id").attr("xsi:type","null").val(this.entityId));
        request.query.appendChild(new SOAPObject("type").attr("xsi:type","null").val(this.type));
        if(this.id2)
            request.query.appendChild(new SOAPObject("id2").attr("xsi:type","null").val(this.id2));

        var val, id;
        var custom0="";
        for(var i=0;i<=this.idx;i++){
            if($("#row"+i).length==0)
                continue;
            var profile_row = request.query.appendChild(new SOAPObject("profile_row").attr("xsi:type","null"));
            $(this.objFields).find("field").each(function(){
                id = $(this).find(">id").text()+"_"+i;

                var items = profile_row.appendChild(new SOAPObject("Item").attr("xsi:type","null"));
                if($(this).find("display_on").text()=="R")
                    items.appendChild(new SOAPObject("column").attr("xsi:type","null").val("custom0"));
                else
                    items.appendChild(new SOAPObject("column").attr("xsi:type","null").val($(this).find(">column").text()));
                var val;
                switch($(this).find("display").text()){
                    case "Tree":
                        val = widObj.getSelectedNodes(id);
                        break;
                    case "Radio Button":
                    case "Checkbox":
                        val = widObj.getCheckedItems(id);
                        break;
                    case "Listbox":
                        val = widObj.getSelectedItems(id);
                        break;
                    default:
                        val = $("#"+id).val();
                        break;
                }
//              alert(val);
                items.appendChild(new SOAPObject("value").attr("xsi:type","null").val(val));
            });
        }

        var sr = new SOAPRequest("SaveEntityProfile", request);
        sr="<?xml version='1.0' encoding='UTF-8'?>"+sr;
        sr=sr.toString();
        var manager = new EXPERTUS.SMARTPORTAL.EntityProfileManager();
        manager.isLnr=this.isLnr;
        manager.initialize(this);
        manager.setLoaderObj(this.loaderObj);
        manager.setActionKey('EntityProfile');
        manager.setEndPointURL(this.getEndPointURL());
        manager.requestXml = sr;
        manager.setCallBack("afterSave");

    //  alert(sr);
        manager.execute();
    	}catch(e){
    		// to do
    	}
    };

    this.afterSave = function(){
    	try{
        for(var i=0;i<=this.idx;i++){
            if($('#savedir_'+i).length==0)
                continue;
            var html="";
        	if((!this.readOnly) && (!this.isLnr)){
        		//html += '<img style="cursor:pointer;" title="'+SMARTPORTAL.t("Save")+'" src="'+themepath+'/images/exp_sp_icon_16x16_Save.gif" onclick="'+this.getWidgetInstanceName()+'.saveEntityProfileTagging();"  />&nbsp;&nbsp;&nbsp;';
        		if(this.multirow=="true")
        			html += '<img style="cursor:pointer;" title="Delete" src="'+themepath+'/images/exp_sp_icon_16x16_Delete.gif" onclick="'+this.getWidgetInstanceName()+'.deleteRow(\'row'+i+'\');"  />';
        	}
        	$('#savedir_'+i).html(html);
        }

    	var saveCallback = this.parentObj.getWidgetInstanceName()+"."+this.saveCallBack+"()";
        eval(saveCallback);
    	}catch(e){
    		// to do
    	}
    };


    this.getSelectedItems = function(id){
    	try{
        var str = "";
        $("#"+id+" option:selected").each(function(){
            str += $(this).val()+",";
        });
        return str.substring(0,str.length-1);
    	}catch(e){
    		// to do
    	}
    };

    this.getCheckedItems = function(id){
    	try{
        var str = "";
        $("input[name='"+ id+"']:checked").each(function(){
            str += $(this).val()+",";
        });
        return str.substring(0,str.length-1);
    	}catch(e){
    		// to do
    	}
    };

    this.getSelectedNodes = function(id){
    	try{
        var tree = $.tree.reference("#Ent_"+this.random+"_"+id+"_Tree");
        var nodes = $.tree.plugins.checkbox.get_checked(tree);
        var selNodes = "";
        for(var i=0;i<nodes.length;i++){
            if(tree.children(nodes[i]).length==0){
                selNodes += nodes[i].id+",";
            }
        }
        return selNodes.substring(0,selNodes.length-1);
    	}catch(e){
    		// to do
    	}
    };

    this.initializeEntityProfile = function(obj){
    	try{
        this.loaderObj = obj.loaderObj;
        this.entityId = obj.entityId;
        this.id2 = obj.id2;
        this.categoryId = obj.categoryId;
        this.funcId = obj.funcId;
        this.type=obj.type;
        this.setWidgetObject(this);
        this.setWidgetTitle("Entity Profile");
        this.setUniqueWidgetId(obj.parentObj.getUniqueWidgetId());
        this.divId = obj.parentObj.getUniqueWidgetId()+this.loaderObj;
        this.parentObj = obj.parentObj;
        this.renderCallback = obj.renderCallback;
        this.saveCallBack=obj.saveCallBack;
        this.multirow = obj.multirow;
        this.idx=-1;
        this.rghtCol = true;
        this.readOnly = obj.readOnly;
        this.Collapsed = obj.Collapsed;
        var randomnumber=Math.floor(Math.random()*11)
        this.random=randomnumber;
    	}catch(e){
    		// to do
    	}
    };
	}catch(e){
		// to do
	}
};

$.extend(EXPERTUS.SMARTPORTAL.EntityProfileWidget.prototype,EXPERTUS.SMARTPORTAL.AbstractDetailsWidget);


function getEntityProfile(isLnr){
	try{
    var entityProfile;
    var createNew=false;

    if(typeof eval("EXPERTUS.SMARTPORTAL.EntityProfileWidget") == "object"){
        entityProfile=eval("EXPERTUS.SMARTPORTAL.EntityProfileWidget");
    } else {
       createNew=true;
       entityProfile = new EXPERTUS.SMARTPORTAL.EntityProfileWidget();
    }
    if(isLnr != undefined) entityProfile.isLnr=isLnr;
    return entityProfile;
	}catch(e){
		// to do
	}
}
