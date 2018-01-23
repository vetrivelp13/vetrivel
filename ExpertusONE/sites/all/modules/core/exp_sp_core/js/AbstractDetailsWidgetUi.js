/**
 * The Abstract Details Widget is the base component of LPP framework. It consists the base LPP framework libary methods.
 * It helps to develop a any new learning widget as a subwidget of this.
 * @class AbstractDetailsWidget
 * @module BaseWidget
 * @namespace EXPERTUS.SMARTPORTAL
 * @uses JQuery libraries, prototype library
 * @constructor
 * @param panelAttributes {namedargument}-  It contains the panel level attributes(height, width,etc...)
 * @param serviceAttributes1 {namedargument} It contains the service level attributes ( certificateid, learnerid,etc....)
 */

/**
 * Contains the id of the previous widget which should be destroyed when a new widget is invoked
 * @property destroyPreviousWidget
 * @type String
 * @default null
 * @scope Stores the value through out the application.
*/
//registernamespace("EXPERTUS.SMARTPORTAL.AbstractDetailsWidget");
var EXPERTUS_SMARTPORTAL_AbstractDetailsWidget ={


        /**
        * If it is false it makes the sync request to the LMS system.
        * @property this.asyncReq
        * @type Boolean -
        * @default true
        * @Instance level variable
        */
        asyncReq:true,

        /**
        * It is used to store the labels for the learning components.
        * @property this.labelsBundle
        * @type JSONObject
        * @default null
        * @Instance level variable
        */
        labelsBundle:null,

        /**
        * It is used to display the grey out box in the browser.
        * @property this.isModal
        * @type boolean
        * @default true
        * @Instance level variable
        */
        isModal:true,

        /**
        * It is used to persist the web service response xml as a text from xmlhttprequest
        * @property this.responseText
        * @type String
        * @default null
        * @Instance level variable
        */
        responseText:null,
        /**
        * It is used to persist  the web service response xml object from xmlhttprequest
        * @property this.responseXml
        * @type Object
        * @default null
        * @Instance level variable
        */
        responseXml:null,

        /**
        * It says how long web service request will be alive. If web service request takes processing time more than specified the value,
        * application abort the request. It will throw the error "Connection timeout".
        * @property this.timeOut
        * @type Object
        * @default null
        * @Instance level variable
        */
        timeOut:null,


        /**
        * Endpoint URL. It specifies where web service request has to hit.
        * @property this.url
        * @type String
        * @default null
        * @Instance level variable
        */
        url:null,

        /**
        * It is used to maintain the service level attributes values (Example:- certificateid,learnerid, etc... )
        * @property this.serviceAttributes
        * @type namedargument
        * @default  values populated from sub widget.
        * @Instance level variable
        */
        serviceAttributes:null,

        /**
        * In order to maintain the parent widget reference to the sub widget. It helps.
        * @property this.widgetCallingFrom
        * @type Object
        * @default null
        * @Instance level variable
        */
        widgetCallingFrom:null,

        /**
        * It is used to store the xmlhttprequest this. It will help to do the request object based services.E.g: abort the connection, check the
        * status of http request.
        * @property this.httpRequest
        * @type Object
        * @default null
        * @Instance level variable
        */
        httpRequest:null,

        /**
        * It is used to render the content of the panel. It persist all the html element whichever needs to be render.
        * @property this.bodyHtml
        * @type String
        * @default  ""
        * @Instance level variable
        */
        bodyHtml:"",

        /**
        * panel object will load using this value.It is nothing but unique div id value which is used to load the panel this.
        * @property this.uniqueWidgetId
        * @type String
        * @default null
        * @Instance level variable
        */
        uniqueWidgetId:null,

        /**
        * Message to display the widget title.
        * @property this.widgetTitle
        * @type String
        * @default null
        * @Instance level variable
        */
        widgetTitle:null,

        /**
        * It is used to the xml and cookies related operations.
        * @property this.xmlutil
        * @type Object
        * @default null
        * @Instance level variable
        */
        xmlutil:null,

        /**
        * if inline==true then widget will display using accordian effect. (inline widget).It will not show the header and footer.
        * @property this.inline
        * @type boolean
        * @default false
        * @Instance level variable
        */
        inline:false,

        /**
        * According to the locale value, widget will pick the labels and other properties from i18n repository.
        * @property this.locale
        * @type String
        * @default english
        * @Instance level variable
        */
        locale:null,

        /**
        * Variable to hold the soap action name.
        * @property this.SOAPActionName
        * @type String
        * @default english
        * @Instance level variable
        */
        SOAPActionName:null,

        /**
        * Variable to hold the request method name.
        * @property this.methodName
        * @type String
        * @default english
        * @Instance level variable
        */
        methodName:null,

        /**
        * Variable to hold the service name or number.
        * @property this.serviceName
        * @type String
        * @default english
        * @Instance level variable
        */
        actionKey:null,

        /**
        * It is used to store the widget details. - Dont use this method.
        * @property this.widgetDetailsObject
        * @type Object
        * @default null
        * @Instance level variable
        */
        widgetDetailsObject:null,

        lovXml:null,

        /**
        * Overridable method which is used to set the locale of the widget.
        * @method this.setLocale
        * @param locale{String} locale value
        * @public
        */
        setLocale:function(locale)
        {
            this.locale=locale;
        },

        /**
        * Overridable method which is used to set the soap action name of the widget.
        * @method this.setSOAPActionName
        * @param SOAPActionName{String} soap action name
        * @public
        */
        setSOAPActionName:function(SOAPActionName)
        {
            this.SOAPActionName=SOAPActionName;
        },

        /**
        * Overridable method which is used to get the SOAP action name of the widget.
        * @method this.getSOAPActionName
        * @public
        * @return String
        */
        getSOAPActionName:function()
        {
            return this.SOAPActionName;
        },

        /**
        * Overridable method which is used to set the method name of the request.
        * @method this.setMethodName
        * @param methodName{String} soap action name
        * @public
        */
        setMethodName:function(methodName)
        {
            this.methodName=methodName;
        },

        /**
        * Overridable method which is used to get the name of the request.
        * @method this.getSOAPActionName
        * @public
        * @return String
        */
        getMethodName:function()
        {
            return this.methodName;
        },

        /**
        * Overridable method which is used to get the locale of the widget.
        * @method this.getLocale
        * @return String
        * @public
        */
        getLocale:function()
        {
            return  this.locale;
        },

          /**
         * Overridable method which is used to set the http request from manager layer.
         * @param reqObj - XmlHttpRequestObj
         * @method this.setHttpReqObj
         * @public
         *
         */
        setHttpReqObj:function(reqObj)
        {
            this.httpRequest=reqObj;
        },

        /**
         * Overridable method which is used to get the current Instance Object name.
         * @method this.getObjectName
         * @return {String} Name of the widget instance.
         * @public
         */
        getObjectName:function()
        {
            return this.widgetGlobalName;
        },

        /**
        * Method which is used trim a string means remove the leading and trailing space.
        * @method this.Trim()
        * @return {String}
        * @public
        */
         Trim:function(str){
            //return str.replace(/(^\s*)|(\s*$)/g, "");
        	return $.trim(str);
         },

        /**
        * Method which is used to Left trim a string.
        * @method this.LTrim()
        * @return {String}
        * @public
        */
         LTrim:function(str){
             //return str.replace(/(^\s*)/g, "");
        	 return $.trim(str);
        },

        /**
        * Method which is used to Right trim a string.
        * @method this.RTrim()
        * @return {String}
        * @public
        */
        RTrim:function(str)
        {
            //return str.replace(/(\s*$)/g, "");
        	return $.trim(str);
        },

        /**
         * Overridable method which is used to get the current Instance Object name with index position of WidgetEngine properties.
         * @method this.getWidgetInstanceName
         * @return {String} Name of the widget instance.
         * @public
         */
        getWidgetInstanceName:function()
        {
            //return this.widgetGlobalName+"["+this.widgetIdx+"]";
            return this.widgetGlobalName;
        },

        /**
        * Overridable method which is used to executes when the web service response is success.
        * @method this.getResponseXml
        * @return responseXml {Object}
        * @public
        */
        getResponseXml:function()
        {
               return this.responseXml
        },

        /**
        * Overridable method which is used to convert the response into proper xml format (incase the response
        * contain &gt; instead of >).
        * @method this.convertResponseXml
        * @return responseXml {Object}
        * @public
        */

       convertResponseXml:function()
       {

            //var formated=this.replaceAll(this.responseText,"&","&amp;");
            // var formated=(this.responseText).replace( new RegExp('&',"g"), 'ampersand');

    /*      var formated=this.replaceAll(this.responseText,"&lt;","<");
            formated=this.replaceAll(formated,"&gt;",">");
            formated=formated.replace( new RegExp('&',"g"), '&amp;');
            //prompt('',formated);*/
            var responsetext =this.responseText;
            try //Internet Explorer
            {
              xmlDoc=new ActiveXObject("Microsoft.XMLDOM");
              xmlDoc.async="false";
              xmlDoc.loadXML(responsetext);
            this.responseXml = xmlDoc;
            }
            catch(e)
            {
                try{
                    parser=new DOMParser();
                    this.responseXml = parser.parseFromString(responsetext,"text/xml");
                }catch(e){
                    //alert(e.message);
                }

            }
       },


        /**
        * Overridable method which is used to set the  web service url.
        * @method this.setEndPointURL
        * @param {String} - URL of the LMS Server
        * @public
        */
        setEndPointURL:function(url)
        {
            this.url = url;
            //alert("URL_:"+this.url);
        },

        /**
        * Overridable method which is used to get the  web service url.
        * @method this.getEndPointURL
        * @return  {String} - URL of the LMS Server
        * @public
        */
        getEndPointURL:function()
        {
            return this.url;
        },

        /**
        * Overridable method which is used to set the  service number or name .
        * @method this.setactionKey
        * @param  {String} - Service number or name
        * @public
        */
        setActionKey:function(no){
            this.actionKey=no;
            //alert(this.actionKey);
        },

        /**
        * Overridable method which is used to get the  service number or name .
        * @method this.getActionKey
        * @return  {String} - Service number or name
        * @public
        */
        getActionKey:function(){
            return this.actionKey;
        },

        /**
        * Overridable method which is used to set the timeout value for ajax call.
        * @method this.setTimeout
        * @param {int} - time in milliseconds
        * @public
        */
        setTimeout:function(timeoutvalue)
        {
            callback.timeout=timeoutvalue;
        },

        /**
        * Overridable method which is used to get the timeout value for ajax call.
        * @method this.getTimeout
        * @return {int} - time in milliseconds
        * @public
        */
        getTimeout:function()
        {
            return callback.timeout;
        },


        /**
        * Overridable method which is used to set the parent widget to the current widget object.
        * @method this.setWidgetCallingFrom
        * @param {Object} - Parent widget object
        * @public
        */
        setWidgetCallingFrom:function(widgetCallFrom)
        {
            this.widgetCallingFrom=widgetCallFrom;
        },

        /**
        * Overridable method which is used to get the parent widget of the current widget object.
        * @method this.getWidgetCallingFrom
        * @return {Object} - Parent widget object
        * @public
        */
        getWidgetCallingFrom:function()
        {
            return this.widgetCallingFrom;
        },

        /**
        * Overridable method which is used to set unique widget id which is used to creating the panel dom ids.
        * @method this.setUniqueWidgetId
        * @param {String} - Unique widget id of the widget
        * @public
        */
        setUniqueWidgetId:function(uniqueWidgetId)
        {
            this.uniqueWidgetId=uniqueWidgetId
            this.setAlertSpace(uniqueWidgetId);
        },

        setAlertSpace:function(id){
            var id = document.getElementById("AlertMessage");
            if(id==null || id==undefined || id=='') {
            	(function($) {
            		$('#'+id).parent().append("<div id='AlertMessage'></div>");
            	})(jQuery);
            }
        },

        /**
        * Overridable method which is used to get unique widget id which is used to creating the panel dom ids.
        * @method this.getUniqueWidgetId
        * @return {String} - Unique widget id of the widget
        * @public
        */
        getUniqueWidgetId:function()
        {
            return this.uniqueWidgetId;
        },

        /**
        * Overridable method which is used to set the necessary parameters for the widget and set
        * the base display for the widget.
        * @method this.renderDialog
        * @param {namedarguments} - It contains panel attributes i.e width,height,x,y
        * @public
        */
        renderDialog:function(params,serviceAttributes)
        {
            /* Get User Options */
            //this.setServiceAttributes(serviceAttributes);
            //alert(this.serviceAttributes.Certificate)
            var isResizable=params.resizable;
            var isDraggable=params.draggable;
            var isAutoOpen=params.autoOpen;
            var isModal=params.modal;
            var panelwidth=params.width;
            var height=params.height;

            //var position=params.position;
            //place the dialog at the center
            var wleft = (screen.width - panelwidth) / 2;
            var wtop = 60;
            if(height!='undefined' && height != 'auto') {
                wtop=(screen.height - height) / 2;
            }
            var position = [wleft,wtop];

            var isCloseOnEscape=params.CloseOnEscape;
            var panelTitle=(params.title==undefined)?this.getWidgetTitle():params.title;
            var panelButtons=params.buttons;
            var v = document.createElement("div");
            var destroyable=params.destroyable;
            var isAlert = params.isAlert;
            // If a widget is said as destroyable.
            var body = (params.body==null || params.body=='')?this.bodyHtml:params.body;
            var uniqueID=this.getUniqueWidgetId();
            var dialogid=(params.dialogid==undefined)?'':params.dialogid;
            var stack = params.stack;
            v.id =uniqueID+dialogid;
            var btns=new Array("Validate","Change Password ")
            v.style.position='relative';
            var obj=document.getElementById(uniqueID+params.parentid);
            if(obj!=null){
                //$(obj).add(v);
                obj.appendChild(v);
            }else{
                document.body.appendChild(v);
            }
            $("#"+v.id).html(body);
            jQuery.ui.dialog.defaults.bgiframe = true;
            var wobj = eval(this.getWidgetInstanceName());
            var wname = this.getWidgetInstanceName();
            $(function() {
            /*  $("#"+v.id).dialog({
                    position: position,
                    resizable:isResizable,
                    draggable:isDraggable,
                    autoOpen: isAutoOpen,
                    width:panelwidth,
                    closeOnEscape:isCloseOnEscape,
                    title:panelTitle,
                    height:height,
                    buttons:panelButtons,
                    modal:isModal,
                    bgiframe:true,
                    close:function(){
                        jQuery(this).remove();
                    }
                });*/
                $("#"+v.id).dialog("open");
                    var closeButt={};
                    closeButt[SMARTPORTAL.t('Close')]=function(){ wobj.destroyDialog(this,v.id); };
                    var actions = params.dialogActions;
                    if(actions!=undefined && actions.length>0){
                        for(var i=0;i<actions.length;i++){
                            var fn=actions[i].actionFn;
                            fn=wname+'.'+fn+'()';
                            closeButt[actions[i].actionName]=function(){eval(fn);};
                        }
                    }

                    $("#"+v.id).dialog({
                        width:panelwidth,
                        height:height,
                        title:panelTitle,
                        //top:top,
                        //left:left,
                        position: position,
                        modal: isModal,
                        resizable:false,
                        bgiframe:true,
                        stack:stack,
                        buttons:closeButt,
                        overlay: {
                            opacity: 0.5,
                            background: "black"
                        }
                    });
            });


        /*
                if(destroyPreviousWidget !=''){
                    if(destroyPreviousWidget != v.id && !isAlert){
                        this.destroyDialog(destroyPreviousWidget);
                    }
                }
                if(destroyable){
                    destroyPreviousWidget=v.id;
                }
            */
            // jQuery("#"+v.id).show('blind', {},800,'');
        },

        destroyDialog:function(obj,id){
            $(obj).dialog('destroy');
            $(obj).dialog('close');
            $('#'+id).remove();
        },

         /**
           *  @method Method to display alert messages.
           */
        createLoader:function(resultPanel,scroll){
            //alert("resultPanel :"+resultPanel);
        	scroll = (scroll == undefined) ? true : false;
            var divid= "loaderdiv"+resultPanel;
            if(document.getElementById(divid)==null && document.getElementById(resultPanel)!=null){
                var divobj=document.createElement('div');
                divobj.id=divid;
                var height;
                if(navigator.appName=="Microsoft Internet Explorer") {
                    height = document.getElementById(resultPanel).offsetHeight;
                } else {
                    height = document.getElementById(resultPanel).clientHeight;
                }
                var width = document.getElementById(resultPanel).offsetWidth;
                //38708: Location creating twice when clicking save button twice
               	var org_height = height;
                if(height < 100) {
	                 height = 75;
	            } else if (height >= 1024) {
	                 height = height/3;
	            } else if (height < 1024 ) {
	                 height = height/2;
	            }
                //42360: when we clicks on share link catalog page loader icon is not showing
                if(parseInt(org_height)==0)
                	org_height = height;
                divobj.innerHTML='<div height="'+parseInt(height)+'" width="'+parseInt(width)+'"><table border="0" width="100%"><tr><td align="center" height="'+parseInt(height)+'" width="'+parseInt(width)+'" valign="middle"><div class="loaderimg"></div></td></tr></table></div>';
                if(width == 0) {
                	width = '800';
                }
            	$($("#"+resultPanel)).prepend(divobj);
                $('#'+divid).addClass("loadercontent");
                $('#'+divid).width(width);
                $('#'+divid).height(org_height);
                $('#'+divid).css("z-index",1003);

                if(((divid.indexOf('divtag') == -1) && (divid.indexOf('divcatalog-attachment') > 0)) || ((divid.indexOf('divtag') > 0) && (divid.indexOf('divcatalog-attachment') == -1))){
                } else {
                	$('#'+divid).css("left","0px");
                }

                var marginHeight = (height-80)/2;
            	$('#'+divid+' .loaderimg').css("margin-top",marginHeight+"px");
            	if(resultPanel == "enroll-lp-result-container")
            		$('#'+divid).css("height","100%");
            }
          },

          /**
           *  @method Method to display alert messages.
           */
        destroyLoader:function(loaderTarget){
              var divid= "loaderdiv"+loaderTarget;
                if(document.getElementById(divid)!=null){
                    //Element.remove(divid);
                        $('#'+divid).remove();

                }
          },


        /**
        * Overridable method which is used to set the base tempate of the widget. It is mandatory to set the div id as given in this method
        * otherwise CSS will break depends on the browser type.
        * @method this.setBodyTemplate
        * @param {obj} - It contains the current widget object.
        * @public
        */
        setBodyTemplate:function(obj,inline)
        {
            var width1=this.panelAttributes.width;
            width1 = width1.split('px');
            var width = width1[0]-80;
            var widthmid = width1[0]-40
            /* Condition added for avoid leftside panel for inline widget : Vincent */
            this.bodyHtml='';
            if(inline==true){
                this.bodyHtml=this.bodyHtml+"<div  style=''></div>";
                this.bodyHtml=this.bodyHtml+"  <div class='bdc-in'>";
            }else{
                this.bodyHtml=this.bodyHtml+"<div id='"+this.uniqueWidgetId+"utilcontainer' ></div><div class='bl'></div>";
                this.bodyHtml=this.bodyHtml+"<div class='bdc'>";

                //Sangeetha: Added Padding style for Greyed part out of border in registration widget
                this.bodyHtml=this.bodyHtml+"<div style='width:"+widthmid+"px;padding:6px;'><div class='top-img'><div class='left-top'></div><div class='middle-bg' style='width:190px;'></div><div class='right-top'></div></div><div class='mid-img' style='width:230px; padding-top:1em;'>";
                //this.bodyHtml=this.bodyHtml+"<div style='width:98%;padding:15px;overflow:hidden;'><div class='top-img'><div class='left-top'></div><div class='middle-bg' style='width:98%;'></div><div class='right-top'></div></div><div class='mid-img' style='width:100%'>";
            }
        },

        /**
        * Overridable method which is used to set the closure phase of base tempate of the widget. It is mandatory to set the div id as given in this method
        * otherwise CSS will break depends on the browser type.
        * @method this.setBodyCloseAndFooterTemplate
        * @param {obj} - It contains the current widget object.
        * @public
        */
        setBodyCloseAndFooterTemplate:function(obj)
        {
            var width1=this.panelAttributes.width;
            width1 = width1.split('px');
            var width = width1[0]-80;
            if(this.inline!=true)
            {
                this.bodyHtml=this.bodyHtml+"</div><div class='top-img'><div class='left-bot'></div><div class='middle-bg' style='width:190px;'></div><div class='right-bot'></div></div>";
                //this.bodyHtml=this.bodyHtml+"</div><div class='top-img'><div class='middle-bg' style='width:98%;'><div class='left-bot'></div><div class='right-bot'></div></div></div>";
                //this.bodyHtml=this.bodyHtml+"</div><div class='top-img'><div class='left-bot'></div><div class='middle-bg' style='width:98%;'></div><div class='right-bot'></div></div>";
                this.bodyHtml=this.bodyHtml+"</div><div class='br'></div>";
            }
            else
            {
                this.bodyHtml=this.bodyHtml+"</div>";
            }
        },

        /**
        * Overridable method which is used to set the header section of the widget(including close,min,refresh button).
        * @method this.setHeaderHtml
        * @param {obj} - It contains the current widget object.
        * @public
        */
        setHeaderHtml:function(obj)
        {
            var locale=obj.getLocale();

            var objName=obj.getObjectName();
            var width1=this.panelAttributes.width;
            width1 = width1.split('px');
            var width = width1[0]-14;
            this.headerHtml="<div class='header' id='headerdiv' style='width:"+width1[0]+"px;'><div class='header-lf'></div><div class='header-md' id='headermddiv' style='width:"+width+"px;'><div class='header-lg'><img src='"+config.data[1].paths[0].componentroot+config.data[1].paths[3].imgpath+"logo.gif'/></div>";
            var objj=eval(objName+"["+this.widgetIdx+"]");
             if(this.panelAttributes.close==true)
             {
                    var clickParams="";
                     if(objName!=null && objName=="EXPERTUS.service.WidgetEngine.dropPolicyWidgetInstances")
                     {
                         clickParams=objName+"["+this.widgetIdx+"].closeWidget()";
                     }
                     else
                     {
                         clickParams="EXPERTUS.util.GlobalMethods.closeWidget("+objName+"["+this.widgetIdx+"])";
                     }
                     this.headerHtml=this.headerHtml+EXPERTUS.util.GlobalMethods.getCloseButton(objj,clickParams);
                     this.headerHtml=this.headerHtml+"</div></div><div class='header-rt'></div></div>";
             }
             if(this.panelAttributes.min==true)
             {
                       this.headerHtml=this.headerHtml+EXPERTUS.util.GlobalMethods.getMinimizeButton(objj);
             }
             if(this.panelAttributes.max==true)
             {
                       this.headerHtml=this.headerHtml+EXPERTUS.util.GlobalMethods.getMaximizeButton(objj);
             }
             if(this.panelAttributes.refresh==true)
             {
                       var clickParams=objName+"["+this.widgetIdx+"].refreshWidget()";
                       this.headerHtml=this.headerHtml+EXPERTUS.util.GlobalMethods.getRefreshButton(objj,clickParams);
             }
             if(this.panelAttributes.showthemes!=null && (this.panelAttributes.showthemes==true || this.panelAttributes.showthemes=='true'))
             {
                       var clickParams=objName+"["+this.widgetIdx+"].displayChangeThemePopup()";
                       this.headerHtml=this.headerHtml+EXPERTUS.util.GlobalMethods.getThemesIcon(objj,clickParams);
             }
             if(this.panelAttributes.showlocale!=null && (this.panelAttributes.showlocale==true || this.panelAttributes.showlocale=='true'))
             {
                       var clickParams=objName+"["+this.widgetIdx+"].displayLocalePopup()";
                       this.headerHtml=this.headerHtml+EXPERTUS.util.GlobalMethods.getLocaleIcon(objj,clickParams);
             }
             if(this.panelAttributes.showReport!=null && (this.panelAttributes.showReport==true || this.panelAttributes.showReport=='true'))
             {
                       this.headerHtml=this.headerHtml+EXPERTUS.util.GlobalMethods.getReportIcon(objj,clickParams);
             }
             if(this.panelAttributes.autoCenter!=null && (this.panelAttributes.autoCenter==true || this.panelAttributes.autoCenter=='true'))
             {
                       //EXPERTUS.util.GlobalMethods.centerWidget(this.uniqueWidgetId);
             }

        },

        /**
        * Overridable method which is used to set the footer section of the widget.
        * @method this.setFooterHtml
        * @param {obj} - It contains the current widget object.
        * @public
        */
        setFooterHtml:function(obj)
        {
              this.footerHtml="";
              this.footerHtml=this.footerHtml+"<div class='dl' width='100%'></div>";
              this.footerHtml=this.footerHtml+"<div class='dr'></div>";

        },

        /**
        * Overridable method which is used to set the mask effect to the web page.
        * @method this.setGreyOut
        * @public
        */
        setGreyOut:function()
        {
                var modaldiv=document.createElement("div");
                modaldiv.className="mask";
                modaldiv.id="LPPMASK";
                document.getElementsByTagName("body")[0].appendChild(modaldiv);
        },


        /**
        * Overridable method which is used to hide the drop down when sub widget launch on top of parent widget.
        * @method this.dropDownOverLayHide
        * @public
        */
        dropDownOverLayHide:function()
        {
            try
            {
                        if(this.widgetCallingFrom!=null)
                        {
                            var dropDownlist=$(this.widgetCallingFrom.uniqueWidgetId).getElementsByTagName("select");
                            var droplength=$(this.widgetCallingFrom.uniqueWidgetId).getElementsByTagName("select").length;
                            for(var i=0;i<droplength;i++)
                            {
                                dropDownlist[i].style.visibility='hidden';
                                if(dropDownlist[i].style.visibility=='visible')
                                {
                                    dropDownlist[i].style.visibility='hidden';
                                }
                            }
                        }
             }
             catch(e)
             {}
        },

        /**
        * Overridable method which is used to show the drop down in the parent widget when sub widget getting closed.
        * @method this.dropDownOverLayHide
        * @public
        */
        dropDownOverLayShow:function()
        {
            try
            {
                        if(this.widgetCallingFrom!=null)
                        {
                            var dropDownlist=$(this.widgetCallingFrom.uniqueWidgetId).getElementsByTagName("select");
                            var droplength=$(this.widgetCallingFrom.uniqueWidgetId).getElementsByTagName("select").length;
                            for(var i=0;i<droplength;i++)
                            {
                                dropDownlist[i].style.visibility='visible';
                            }
                        }
             }
             catch(e)
             {}
        },


        /**
        * Overridable method which is used to initiate the web service request.
        * @method this.initiateRequestBase
        * @param {String} - It contains the div object which is used by the ImageLoader component.
        * @public
        */
        initiateRequestBase:function(loaderDivName)
        {
            this.abortReq();
            $(this.uniqueWidgetId+loaderDivName).innerHTML=EXPERTUS.util.GlobalMethods.loaderImage({align:'center'});
            this.initiateRequest();
        },

        /**
        * Overridable method which is used to initialize the widget instance in widgetengine.
        * Without core team concern dont do the changes in this method.
        * @method this.initializeWidgetInstance
        * @param {Object} - It contains the current widget object.
        * @public
        */
        initializeWidgetInstance:function(obj)
        {
            //this.widgetIdx=EXPERTUS.service.WidgetEngine.getIdxOfWidget(eval(obj.getObjectName()));
            //var assignObj=obj.getObjectName()+"["+this.widgetIdx+"]=obj";
            var assignObj=obj.getObjectName()+"=obj";
            eval(assignObj);
        },

        /**
        * Overridable method which is used to print the datatable contents in new window.
        * @method this.doPrintWidgetHtml
        * @public
        */
        doPrintWidgetHtml:function()
        {
                this.getPrinthtml();
        },

        /**
        * Overridable method which is used to export  the datatable contents in CSV format.
        * @method this.doPrintWidgetHtml
        * @public
        */
        doExportWidgetHtml:function()
        {
                this.getExporthtml();
        },

        /**
        * Overridable method which is used to clone the result set object.
        * @method this.cloneResultSet
        * @param {Object} - Datatable object
        * @public
        */
        cloneResultSet:function(dataTableObj)
        {
                var rs=dataTableObj.getRecordSet();
                var len=rs.getLength();
                for(var i=0;i<len;i++)
                {
                    this.rsCompleteObj[i]=rs.getRecord(i);
                }

                this.rsCompleteObj=this.rsCompleteObj.clone();


        },


            /**
        * Overridable method which is used to set the result set according to the user filteration in widget.
        * (User filteration currently support checkbox.)
        * @method this.setResultSet
        * @param {Object} - Widget Object.
        * @param {String} - Filter control  id which is used to get the current selection of the records.
        * How many checkbox field enabled and disabled.
        * @public
        */
        setResultSetNew:function(obj,id)
        {
                var ll=document.getElementById(id).parentNode.childNodes.length;
                for(var kk=0;kk<ll;kk++)
                {
                    id=document.getElementById(id).parentNode.childNodes[kk].id;
                    var colNameCount=id.lastIndexOf("_");
                    var colName=id.substring((colNameCount+1),id.length);
                    var idval=escape(id+"_"+obj.value);
                    if(document.getElementById(idval)!=null && document.getElementById(idval).checked==true)
                    {
                    }
                    else
                    {
                        this.dontDisplayValues[0]={"colName":colName,"contains":obj.value,"display":false};

                    }
                }
                var existDT=this.getDataTableObj();
                existDT.showPage(1);
        },

        /**
        * Overridable method which is used to set the result set according to the user filteration in widget.
        * (User filteration currently support checkbox.)
        * @method this.setResultSet
        * @param {Object} - Widget Object.
        * @param {String} - Filter control  id which is used to get the current selection of the records.
        * How many checkbox field enabled and disabled.
        * @public
        */
        setResultSet:function(obj,id)
        {

                var widObj=this.getWidgetObject();
                var locale = widObj.getLocale();
                var existDT=this.getDataTableObj();
                var existRs=existDT.getRecordSet();
                existDT.deleteRows(0,existRs.getLength());
                var colSet=existDT.getColumnSet().keys;
                var isShow="";
                var totalCount=0;
                var isDTEmpty=true;
                var len=this.rsCompleteObj.length;

                for(var i=0;i<len;i++)
                {
                    var row=this.rsCompleteObj[i];
                    if(row!=null)
                    {

                        var ll=document.getElementById(id).parentNode.childNodes.length;

                        enabledCheckBoxCount=0;
                        isShow=false;
                        for(var kk=0;kk<ll;kk++)
                        {
                            id=document.getElementById(id).parentNode.childNodes[kk].id;
                            var colNameCount=id.lastIndexOf("_");
                            var colName=id.substring((colNameCount+1),id.length);
                            var rowCol="row.getData('"+colName+"')";
                            var idval=escape(id+"_"+eval(rowCol));

                            if(document.getElementById(idval)!=null && document.getElementById(idval).checked==true)
                            {
                                enabledCheckBoxCount++;
                            }
                        }

                        if(enabledCheckBoxCount==ll)
                        {
                            isShow=true;
                            totalCount++;
                        }
                        if(isShow)
                        {
                            existDT.addRow(row._oData);
                            isDTEmpty=false;
                        }
                    }
                }
                if(isDTEmpty==true)
                {

                    if($(this.uniqueWidgetId+"_menu"))
                        $(this.uniqueWidgetId+"_menu").style.display="none";
                        EXPERTUS.util.GlobalMethods.alerts(this.labelsBundle.i18nSelectDelivery,locale); // "Select Delivery mode and Status to view"
                        this.dataTableObj.showTableMessage("<b>"+ this.labelsBundle.i18nEnrollmentEmptyTableMesg + "</b>","yui-dt-liner; yui-dt-empty");  // No Enrollments/Transcripts found
                }
                else
                {
                    if($(this.uniqueWidgetId+"_menu"))
                        $(this.uniqueWidgetId+"_menu").style.display="block";
                    if(existDT.initViewChart=="true")
                    {
                        this.viewChart(this.uniqueWidgetId+"chartView");
                    }
                }
        },

        /**
        * Overridable method which is used to get the default time out of the ajax request setted by LPP.
        * @method this.getDefaultTimeOut
        * @param {String} - name of the widget.
        * @public
        */
        getDefaultTimeOut:function(widget)
        {
            var retValue="";
            if(this.timeOut!=null)  //  Widget specific timeout value , first preference
            {
                return this.timeOut;
            }
            else if(cfgObj==null || cfgObj=='undefined')
            {
                retValue=config.data[3].timeout;
            }
            else
            {
                var obName=this.getObjectName();
                var objName=obName.substring("EXPERTUS.service.WidgetEngine.".length,obName.length);
                var cfgObj=eval("config.data[4]."+objName);
                retValue=cfgObj;
            }
            return retValue;
        },

        /**
        * It is used to set the widget object.
        * @property this.setWidgetObject
        * @param {Object} obj contains the widget Object.
        */
        setWidgetObject:function(widgetObj)
        {
            this.widgetDetailsObject = widgetObj;
        },

        /**
        * Overridable method which is used to set the  widget Title.
        * @method this.setWidgetTitle
        * @param {String} - Widget title
        * @public
        */
        setWidgetTitle:function(widgetTitle)
        {
            this.widgetTitle=widgetTitle;
        },

        /**
        * Overridable method which is used to get the  widget Title.
        * @method this.getWidgetTitle
        * @return {String} - Widget title
        * @public
        */
        getWidgetTitle:function()
        {
            return this.widgetTitle;
        },

        /**
        * It is used to get the widget object.
        * @property this.getWidgetObject
        * @param {Object} obj contains the widget Object.
        */
        getWidgetObject:function()
        {
            return this.widgetDetailsObject;
        },

        /**
        * Overridable method which is used to check locale values already loaded or not.
        * @method this.checkLocaleAlreadyLoaded
        * @param {fileId} - It contains the id value of the locale script tag.
        * @public
        */
        checkLocaleAlreadyLoaded:function(fileId)
        {
            if($(fileId))
            {
                return true;
            }
            else
            {
                return false;
            }
        },


        /**
        * @author Sanjeth
        * @date 26/Nov/08
        * Overridable method which is used to load the i18n labels js.
        * @method this.loadi18nFiles
        * @param {obj} - It contains the current widget object.
        * @public
        */

        loadi18nFiles:function()
        {
                var wa=this.getWidgetInstanceName();
                var l="EXPERTUS.service.WidgetEngine.".length;
                var label=wa.substring(l,wa.length);
                var removeBrack=label.indexOf("Instance");
                var wName = label.substring(0,removeBrack);

                var url="";
                url=config.data[1].paths[0].componentroot+config.data[1].paths[6].localepath+wName+"_labels_"+this.locale+".js";
                var oXML=getXMLHttpObj();
                oXML.open('GET', url, false);
                oXML.setRequestHeader("Accept-Charset","UTF-8");
                oXML.send('');
                var e = document.createElement("script");
                e.text=oXML.responseText;
                e.setAttribute("charset","UTF-8");
                e.setAttribute("id",wName + "_labels_"+this.locale+".js");
                document.getElementsByTagName("head")[0].appendChild(e);

                this.setLocaleJsonObject();

        },



        /**
        * Overridable method which is used to set the locale label values in  json object.
        * @method this.getLocaleJsonObject
        * @public
        */
        setLocaleJsonObject:function()
        {
            var na=this.getWidgetInstanceName();
            var l="EXPERTUS.service.WidgetEngine.".length;
            var label=na.substring(l,na.length);
            var removeBrack=label.indexOf("[");
            this.labelsBundle=label.substring(0,removeBrack)+"_labels_"+this.getLocale();
            this.labelsBundle=eval(this.labelsBundle);

        },

        /**
        * Overridable method which is used to get the locale json object.
        * @method this.getLocaleJsonObject
        * @return jsonObject
        * @public
        */
        getLocaleJsonObject:function()
        {
            return this.labelsBundle;
        },
        /**
        * Overridable method which is used to set the online request flag.
        * @method this.setOnlineRequest
        * @param Boolean
        * @public
        */
        setOnlineRequest:function(val)
        {
            this.onlineRequest=val;
        },


        /**
        * Overridable method which is used to get the online request flag.
        * @method this.getOnlineRequest
        * @return Boolean|String
        * @public
        */
        getOnlineRequest:function()
        {
            return this.onlineRequest;
        },


        /**
        * Overridable method which is used to set the necessary parameters for the widget and set
        * the base display for the widget.
        * @method this.renderBase
        * @param {namedarguments} - It contains panel attributes i.e width,height,x,y
        * @param {namedarguments} - It contains service  attributes i.e soap certificate id and parameters required for web services.
        * @param {boolean} - If it true widget will display as a inline model.
        * @param {Object} - It contains the current widget object. It equals to  (this).
        * @public
        */
        renderBase:function(serviceattributes,obj)
        {
            this.serviceAttributes = serviceattributes;
            this.initializeWidgetInstance(obj);
            this.setWidgetObject(obj);
            if(this.uniqueWidgetId.toLowerCase().indexOf('lnr')>=0 || this.uniqueWidgetId.toLowerCase().indexOf('team')>=0){
            	this.setEndPointURL(config.data[0].url[0].learner_url);
            }else{
            	this.setEndPointURL(config.data[0].url[0].admin_url);
            }

            this.setActionKey(eval('config.data[1].serviceaction[0].'+this.uniqueWidgetId))
        },


        /**
        * Overridable method which is used to the service attributes.
        * @method this.setServiceAttributes
        * @param sa {named argument}
        * @public
        */
        setServiceAttributes:function(sa)
        {
            this.serviceAttributes=sa;
        },

        /**
        * Overridable method which is used to set http request type sync or async.
        * @method this.setRequestType
        * @param {boolean} - true | false
        * @public
        */
        setRequestTypeASync:function(syncReq)
        {
            this.asyncReq=syncReq;
        },

        /**
        * This method is used to get the datatable object.
        * @method this.getDataTableObj
        * @public
        * @return dataTableObj.
        */
        setTitleForXML:function(title)
        {
            this.xmlTitle=title;
        },

        /**
         * Overridable method which handles the soap errors.
         * If any common operations like session timed out are
         * handled in this method, after executing this method this
         * will calls the default renderError method (if the error is not
         * a common error).
         * @method this.renderError()
         * @return error string
         */
        renderErrorCommon:function(){
        	 var errcode =$(this.responseXml).find("faultcode").text();
             var errstring = $(this.responseXml).find("faultstring").text();
             var errmsg ="";
             if(errcode!=null && errcode!=undefined)
                 errmsg += "["+errcode+"]: ";
             errmsg += errstring;
             if(errstring=='Your session has expired'){
             	var loc = '';
             	//var ermsg=SMARTPORTAL.t('ERR055');
 	           	//alert(ermsg);
             	var user=this.getUserName();
             	/**
             	 * Clear all cookies
             	 */
             	var cok = document.cookie;
             	var lst = cok.split(";");
             	for(var x=0;x<lst.length;x++){
             		var name=lst[x].split("=");
             		document.cookie = name[0]+"=";
             	}
             	if(user != null && user != undefined && user != '' && user != 'guest')
             		loc = resource.base_url+"/?q=user/logout";
             	else
             		loc = resource.base_url;
             	window.location.href=loc;
             }else{
            	 this.renderError();
             }
        },

        /**
         * Overridable method which handles the soap errors.
         * @method this.renderError()
         * @return error string
         */
        renderError:function(){
            var errcode =$(this.responseXml).find("faultcode").text();
            var errstring = $(this.responseXml).find("faultstring").text();
            var errmsg ="";
            if(errcode!=null && errcode!=undefined)
                errmsg += "["+errcode+"]: ";
            errmsg += errstring;
            if(document.getElementById("msgDiv")){
                $('#msgDiv').css('display','block');
                $('#msgDiv').removeClass('sp_msgbox');
                $('#msgDiv').addClass('sp_alertbox');
                $('#msgDiv').html(SMARTPORTAL.t(errmsg));
            }else if(document.getElementById("main-content")){
                var newdiv=document.createElement("div");
                newdiv.id='msgDiv';
                $('#main-content').before(newdiv);
                $('#msgDiv').css('display','block');
                $('#msgDiv').css('width','688px');
                $('#msgDiv').addClass('sp_alertbox');
                $('#msgDiv').html(SMARTPORTAL.t(errmsg));
            }else{
            	//alert(SMARTPORTAL.t(errmsg));
            	var newdiv=document.createElement("div");
                newdiv.id='msgDiv';
                $("div :first").before(newdiv);
                $('#msgDiv').css('display','block');
                $('#msgDiv').css('width','688px');
                $('#msgDiv').css('color','red');
                $('#msgDiv').addClass('sp_alertbox');
                $('#msgDiv').html(SMARTPORTAL.t(errmsg));
            }
            return errmsg;
        },

        mandatoryCheck:function(divId){
            var str="";
            var flag=true;
            var newDiv="";
            var nextDiv="";
            str1 = new RegExp( );
     	    str1.compile( '^[\s ]*$','gi' );
            if(divId != undefined){
                if($('#'+divId).find('.mandatory')){
                    $('#'+divId).find('.mandatory').each(function(){
                        var valEntered=$(this).val();
                        var name=$(this).attr('name');
                            str =$(this).val();
                            var inputType = $(this).attr("type");
                            newDiv = '';
              		      /* For Learner side Survey*/
                            if(inputType=="checkbox" || inputType=="radio"){
                            	  var id=$(this).attr('id');
                            	  var checked = $("input[id="+id+"]:checked").length;
                            	  if (checked == 0)
                                  {
                            		newDiv=document.createElement('div');
                            		$(newDiv).addClass('local_style_for_div_newid'); // class to be used for fine tuning display of error message in widgets
                                    newDiv.id=this.id+"_newid";
                                    $("#"+this.id+"_newid").remove();
                                    $(newDiv).css({'color':'red','display':'none','float':'left','font-size':'0.9em','text-align':'right','width':'50%'});
                                    $(newDiv).html(Drupal.t("ERR106"));
                                    flag=false;
                                  }
                            	  $('#'+id+'_'+inputType).html(newDiv);
                                  $("#"+this.id+"_newid").show();  /* End For Learner side Survey*/
                            } else if( valEntered=="" || valEntered == null || valEntered == undefined || str1.test(valEntered)==true ){
                            	if (inputType=="select-one") {
                            		newDiv=document.createElement('span');
                            		$(newDiv).addClass('local_style_for_span_newid'); // class to be used for fine tuning display of error message in widgets
                            		newDiv.id=this.id+"_newid";
                            		$("#"+this.id+"_newid").remove();
                            		$(newDiv).css({'color':'red','display':'none','font-size':'0.9em','padding-left': '10px'});
                            		//$(newDiv).html(SMARTPORTAL.t("Please select ") + " " + SMARTPORTAL.t(name) + ".");
                            		$(newDiv).html(Drupal.t("MSG687"));
                            		$(this).closest(".expertus-dropdown-bg").after(newDiv);
                            		$("#"+this.id+"_newid").show();
                            		flag=false;
                            	} else if ($(this).hasClass('hasDatepicker')) {
                            		newDiv=document.createElement('span');
                            		$(newDiv).addClass('local_style_for_span_newid'); // class to be used for fine tuning display of error message in widgets
                            		newDiv.id=this.id+"_newid";
                            		$("#"+this.id+"_newid").remove();
                            		$(newDiv).css({'color':'red','display':'none','font-size':'0.9em','padding-left': '10px'});
                            		$(newDiv).html(SMARTPORTAL.t("Please enter ") + " " + SMARTPORTAL.t(name) + ".");
                            		nextDiv = $(this).next();
                            		if($(nextDiv).is('img') && $(nextDiv).hasClass('ui-datepicker-trigger')) {
                            			$(nextDiv).after(newDiv);
                            		} else {
                            			$(this).after(newDiv);
                            		}
                            		$("#"+this.id+"_newid").show();
                            		flag=false;
                            	} else {
                            		newDiv=document.createElement('span');
                            		$(newDiv).addClass('local_style_for_span_newid'); // class to be used for fine tuning display of error message in widgets
                            		newDiv.id=this.id+"_newid";
                            		$("#"+this.id+"_newid").remove();
                            		$(newDiv).css({'color':'red','display':'none','font-size':'0.9em','padding-left': '10px'});
                            		//$(newDiv).html(SMARTPORTAL.t("Please enter ") + " " + SMARTPORTAL.t(name) + ".");
                            		// Multi lang Issue Is fixed for this Ticket #0044270
                            		$(newDiv).html(Drupal.t("LBL868"));
                            		$(this).after(newDiv);
                            		$("#"+this.id+"_newid").show();
                            		flag=false;
                            	}
                            }else{
                            	$("#"+this.id+"_newid").remove();
                            }
                 });
            }
        }
        return flag;
        },

        addAccordionToTable:function(openCloseClass,openpos,closepos,childClass,callback,obj,parentObj,isRemove) {
            if(openCloseClass!=undefined && childClass!=undefined && callback!=undefined) {
                //var mainObj=$(obj).parent().get(0).nodeName.toLowerCase()=="td"?$(obj).parent():$(obj).parent().parent();
            	var mainObj=$(obj).parents('td');
                $('.'+openCloseClass).each(function(){
                    $(this).css("background-position",closepos);
                });

            	//SULTHAN
            	//ORIGINAL LINE - THIS WILL FETCH ANY ELEMENTS THAT IS PARENT OF THE CURRENT ELEMENT STORED IN OBJ
                //var cols=mainObj.parent().children().size();
            	//THE FOLLOWING LINE ONLY FETCHES THE COUNT OF THE ELEMENT IF IT IS 'TD'
            	var cols = $(obj).parent('td').siblings().length;
            	if(String(mainObj.parent().next().attr("class")).indexOf(childClass+" dt-acc-row")<0) {
                    $("tr."+childClass+" > td > div").slideUp({complete:function(){
                            if(isRemove!=undefined && isRemove==true)
                                $(this).parent().parent().remove();
                            else
                                $(this).parent().parent().css('display','none');
                        //  mainObj.parent().parent().css('border-collapse','collapse');
                        }
                    });
                    var newObj=mainObj.parent().after("<tr class='"+childClass+" dt-acc-row'><td colspan='"+cols+"'><div class='"+childClass+"-data' style='display:none;width=100%;'></div></td></tr>").next(0).find("div."+childClass+"-data");
                    if(callback!=undefined) {
                        callback(newObj,$(obj),parentObj);
                    }
                    $(obj).children().css("background-position",openpos);
                }else{
                    if(mainObj.parent().next().css('display')=='table-row' || mainObj.parent().next().css('display')!='none'){
                        $(obj).children().css("background-position",closepos);
                        $("tr."+childClass+" > td > div").slideUp({complete:function(){
                                if(isRemove!=undefined && isRemove==true)
                                    $(this).parent().parent().remove();
                                else
                                    $(this).parent().parent().css('display','none');
                            //  mainObj.parent().parent().css('border-collapse','collapse');
                            }
                        });
                    } else {
                        $("tr."+childClass+" > td > div").slideUp({complete:function(){
                                $(this).parent().parent().css('display','none');

                            }
                        });
                        //mainObj.parent().parent().css('height','auto');
                        mainObj.parent().next().css('display','table-row');
                        mainObj.parent().after().next(0).find("div."+childClass+"-data").css('display','block');
                        $(obj).children().css("background-position",openpos);
                    }
                }
            } else {
                throw("addAccordionTable : Parameters not specified");
            }
        },

        updateLovOptions:function(opid,opname,selected){

            var obj =  document.getElementById(opid);
            obj.option=0;
            var ct=1;
            $(this.lovXml).find("ListItem").each(function(){
                if($(this).attr("code")==opname){
                    $(this).find("Item").each(function(){
                        var op = new Option();
                        var tname = $(this).find("Name").text();
                        var tvalue = $(this).find("Code").text();
                        op.text = tname;
                        op.value = tvalue;
                        op.title = tname;
                        if((tname==selected)||(tvalue==selected)){
                            op.selected=true;
                        }
                        if(tname){
                            obj.options[ct]=op;
                            ct++;
                        }
                    });
                }
            });
        },

        updateAttrValues:function(opname,opid){
        var op='';
        	$(this.lovXml).find("ListItem").each(function(){
        		$(this).find("Item").each(function(){
        			if($(this).find("Code").text()==opname){
        				op = $(this).find("attr"+opid).text();
        				return op;
        			}
        	  	});
        	});

        	return op;
        },

	    getLovValue:function(opname,opval,type){
            var outvar = '';
            $(this.lovXml).find("ListItem").each(function(){
                if($(this).attr("name")==opname || $(this).attr("code")==opname){
                    $(this).find("Item").each(function(){
                        var tname = $(this).find("Name").text();
                        var tvalue = $(this).find("Code").text();
                        if((tname==opval)||(tvalue==opval)){
                            if(type=='Name' || type=='name'){
                                outvar = tname;
                            }else{
                                outvar = tvalue;
                            }
                            return false;
                        }
                    });
                }
            });
            return outvar;
        },

        validateFieldCheck:function(divId){
            var isValid=true;
            var str="";
            var validateTt="";
            var flag=true;
            var newDiv="";
            //$(newDiv).remove();
            if(divId != undefined){
                if($('#'+divId).find('.validateText')){
                    $('#'+divId).find('.validateText').each(function(){
                        var valEntered=$(this).val();
                            str =$(this).val();
                            validateTt = str.match(/[^\d\w\s*!<>.,#:&%(-_+=)]/);
                            if(validateTt){
                                //alert("validatetext:");
                                newDiv=document.createElement('div');
                                newDiv.id=this.id+"_newid";
                                $("#"+this.id+"_newid").remove();
                                $(newDiv).css({'color':'red','display':'none','float':'right'});
                                $(newDiv).html(SMARTPORTAL.t("Please enter a valid text."));
                                $(this).before(newDiv);
                                $("#"+this.id+"_newid").show();
                                flag=false;
                            }else{
                                $("#"+this.id+"_newid").remove();
                            }
                     });
                }
                 if($('#'+divId).find('.validateNum')){
                     $('#'+divId).find('.validateNum').each(function(){
                      var valEntered=$(this).val();
                      str =$(this).val();
                      var tval = valEntered.split('.');
                      if(tval[0]==undefined)
                          tval[0]='';
                      if(tval[1]==undefined)
                          tval[1]='';
                      var format=$(this).metadata();

                      if(format.formater != undefined && format.formater!="" && format.formater != null){
                        var isFormat=format.formater.split('.');
                        if(isFormat[0]==undefined)
                            isFormat[0]='';
                        if(isFormat[1]==undefined)
                            isFormat[1]='';
                        if(!isNaN(valEntered)){
                          if(isFormat[0]!='*' || isFormat[1]!='*'){}
                            if((isFormat[0].indexOf("#")==0 && isFormat[1].indexOf("#")==0) || (isFormat[0].indexOf("#")==0) || (isFormat[1].indexOf("#")==0)){
                              if((isFormat[0].indexOf("#")==0) && (!(tval[0].length<=isFormat[0].length)) && (isFormat[0].indexOf("*")!=0)){
                                  newDiv=document.createElement('div');
                                  newDiv.id=this.id+"_newid";
                                  $("#"+this.id+"_newid").remove();
                                  $(newDiv).css({'color':'red','display':'none','float':'right'});
                                  $(newDiv).html('Invalid Number format ');
                                  $(this).before(newDiv);
                                  $("#"+this.id+"_newid").show();
                                  flag=false;
                                  }
                              if((isFormat[1].indexOf("#")==0) && (!(tval[1].length<=isFormat[1].length)) && (isFormat[1].indexOf("*")!=0) && tval[1]!=''){
                                  newDiv=document.createElement('div');
                                  newDiv.id=this.id+"_newid";
                                  $(newDiv).css({'color':'red','display':'none','float':'right'});
                                  $(newDiv).html('Invalid Number format ');
                                  $(this).before(newDiv);
                                  $("#"+this.id+"_newid").show();
                                  flag=false;
                              }
                            }
                    //  }
                        }else{
                          newDiv=document.createElement('div');
                          newDiv.id=this.id+"_newid";
                          $("#"+this.id+"_newid").remove();
                          $(newDiv).css({'color':'red','display':'none','float':'right'});
                          $(newDiv).html(SMARTPORTAL.t("Please enter a valid number."))
                          $(this).before(newDiv);
                          $("#"+this.id+"_newid").show();
                          flag=false;
                      }
                      }else if($(this).val().match(/[^\d\-()]/)){
                          newDiv=document.createElement('div');
                          newDiv.id=this.id+"_newid";
                          $("#"+this.id+"_newid").remove();
                          $(newDiv).css({'color':'red','display':'none','float':'right'});
                          $(newDiv).html(SMARTPORTAL.t("Please enter a valid number."))
                          $(this).before(newDiv);
                          $("#"+this.id+"_newid").show();
                          flag=false;
                      }else{
                          $("#"+this.id+"_newid").remove();
                      }
                      });
                     }
                 if($('#'+divId).find('.validatePho')){
                        $('#'+divId).find('.validatePho').each(function(){
                            var valEntered=$(this).val();
                                var s =$(this).val();
                                if(s != ''){
                                    //var rePhoneNumber = new RegExp(/^\([1-9]\d{2}\)\s?\d{3}\-\d{4}$/);
                                    //var rePhoneNumber = new RegExp(/^\d{3}\-\d{3}\-\d{4}$/);
                                    //  var rePhoneNumber = new RegExp(/^\d{10}$/);
                                    var rePhoneNumber = new RegExp(/^-{0,1}\d*\.{0,1}\d+$/);
                                    //var rePhoneNumber = new RegExp(/^\([1-9]\d{2}\)\s$/);
                                    if(!rePhoneNumber.test(s)){
                                     // if(isNaN(parseInt(s)))
                                     // {
                                        newDiv=document.createElement('div');
                                        newDiv.id=this.id+"_newid";
                                        $("#"+this.id+"_newid").remove();
                                        $(newDiv).css({'color':'red','display':'none','float':'right'});
                                        $(newDiv).html(SMARTPORTAL.t('Please enter no. as: XXXXXXXXXX'));
                                        $(this).before(newDiv);
                                        $("#"+this.id+"_newid").show();
                                        flag=false;
                                    }
                                    else{
                                        $("#"+this.id+"_newid").remove();
                                    }
                                } else{
                                    $("#"+this.id+"_newid").remove();
                                }

                        });
                    }
                if($('#'+divId).find('.validatePh')){
                    $('#'+divId).find('.validatePh').each(function(){
                        var valEntered=$(this).val();
                            var s =$(this).val();
                            if(s != ''){
                                //var rePhoneNumber = new RegExp(/^\([1-9]\d{2}\)\s?\d{3}\-\d{4}$/);
                                var rePhoneNumber = new RegExp(/^\d{3}\-\d{3}\-\d{4}$/);
                                //var rePhoneNumber = new RegExp(/^\d{10}$/);
                                //var rePhoneNumber = new RegExp(/^\([1-9]\d{2}\)$/);
                                if(!rePhoneNumber.test(s)){
                                    newDiv=document.createElement('div');
                                    newDiv.id=this.id+"_newid";
                                    $("#"+this.id+"_newid").remove();
                                    $(newDiv).css({'color':'red','display':'none','float':'right'});
                                    $(newDiv).html(SMARTPORTAL.t('Please enter no. as: XXX-XXX-XXXX'));
                                    $(this).before(newDiv);
                                    $("#"+this.id+"_newid").show();
                                    flag=false;
                                }
                                else{
                                    $("#"+this.id+"_newid").remove();
                                }
                            } else{
                                $("#"+this.id+"_newid").remove();
                            }

                    });
                }
                if($('#'+divId).find('.validateDate')){
                    $('#'+divId).find('.validateDate').each(function(){
                        var dateStr =$(this).val();
                        var datePat = /^(\d{1,2})(\/|-)(\d{1,2})(\/|-)(\d{4})$/;
                        var matchArray = dateStr.match(datePat); // is the format ok?
                        newDiv=document.createElement('div');
                        newDiv.id=this.id+"_newid";
                        $("#"+this.id+"_newid").remove();
                        $(newDiv).css({'color':'red','display':'none','float':'right'});
                        $(this).before(newDiv);
                        if (matchArray == null) {
                            $(newDiv).html('Date can be mm/dd/yyyy or mm-dd-yyyy.')
                            $("#"+this.id+"_newid").show();
                            flag=false;
                        }
                        month = matchArray[1]; // p@rse date into variables
                        day = matchArray[3];
                        year = matchArray[5];
                        if (month < 1 || month > 12) { // check month range
                            $(newDiv).html('Month must be between 1 and 12.');
                            $("#"+this.id+"_newid").show();
                            flag=false;
                        }
                        if (day < 1 || day > 31) {
                            $(newDiv).html('Day must be between 1 and 31.');
                            $("#"+this.id+"_newid").show();
                            flag=false;
                        }
                        if ((month==4 || month==6 || month==9 || month==11) && day==31) {
                            $(newDiv).html("Month "+month+" doesn`t have 31 days:");
                            $("#"+this.id+"_newid").show();
                            flag=false;
                        }
                        if (month == 2) { // check for february 29th
                        var isleap = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
                        if (day > 29 || (day==29 && !isleap)) {
                            $(newDiv).html("February " + year + " doesn`t have " + day + " days!");
                            $("#"+this.id+"_newid").show();
                            flag=false;
                        }
                        }
                    //  flag=true; // date is valid
                    });
                }
                if($('#'+divId).find('.validateURL')){
                    $('#'+divId).find('.validateURL').each(function(){
                        var url=$(this).val();
                      if(url!="" && url!= null && url!= undefined){
                        var RegExp = /(ftp|http|https):\/\/(\w+:{0,1}\w*@)?(\S+)(:[0-9]+)?(\/|\/([\w#!:.?+=&%@!\-\/]))?/;
                        //var RegExp = /http:\/\/[A-Za-z0-9\.-]{3,}\.[A-Za-z]{3}/;
                            if(!RegExp.test(url)){
                                newDiv=document.createElement('div');
                                newDiv.id=this.id+"_newid";
                                $("#"+this.id+"_newid").remove();
                                $(newDiv).css({'color':'red','display':'none','float':'right'});
                                $(newDiv).html(SMARTPORTAL.t("Please enter a valid URL."));
                                $(this).before(newDiv);
                                $("#"+this.id+"_newid").show();
                                flag=false;
                            }else{
                                $("#"+this.id+"_newid").remove();
                            }
                         }

                    });
                }
                if($('#'+divId).find('.validateEmail')){
                    $('#'+divId).find('.validateEmail').each(function(){
                        var email=$(this).val();
                        //var RegExp = /^((([a-z]|[0-9]|!|#|$|%|&|'|\*|\+|\-|\/|=|\?|\^|_|`|\{|\||\}|~)+(\.([a-z]|[0-9]|!|#|$|%|&|'|\*|\+|\-|\/|=|\?|\^|_|`|\{|\||\}|~)+)*)@((((([a-z]|[0-9])([a-z]|[0-9]|\-){0,61}([a-z]|[0-9])\.))*([a-z]|[0-9])([a-z]|[0-9]|\-){0,61}([a-z]|[0-9])\.)[\w]{2,4}|(((([0-9]){1,3}\.){3}([0-9]){1,3}))|(\[((([0-9]){1,3}\.){3}([0-9]){1,3})\])))$/;
                        var RegExp = /^((([a-z]|[A-Z]|[0-9]|!|#|$|%|&|'|\*|\+|\-|\/|=|\?|\^|_|`|\{|\||\}|~)+(\.([a-z]|[A-Z]|[0-9]|!|#|$|%|&|'|\*|\+|\-|\/|=|\?|\^|_|`|\{|\||\}|~)+)*)@((((([a-z]|[A-Z]|[0-9])([a-z]|[A-Z]|[0-9]|\-){0,61}([a-z]|[A-Z]|[0-9])\.))*([a-z]|[A-Z]|[0-9])([a-z]|[A-Z]|[0-9]|\-){0,61}([a-z]|[A-Z]|[0-9])\.)[\w]{2,4}|(((([0-9]){1,3}\.){3}([0-9]){1,3}))|(\[((([0-9]){1,3}\.){3}([0-9]){1,3})\])))$/;
                        newDiv=document.createElement('div');
                        newDiv.id=this.id+"_newid";
                        $("#"+this.id+"_newid").remove();
                        $(newDiv).css({'color':'red','display':'none','float':'right'});
                        $(newDiv).html(SMARTPORTAL.t("Please enter a valid e-mail address."));
                        $(this).before(newDiv);

                        if(email != ''){
                            if(!RegExp.test(email)){
                                $("#"+this.id+"_newid").show();
                                flag=false;
                            }else{
                                $("#"+this.id+"_newid").remove();
                            }
                        }else{
                            $("#"+this.id+"_newid").remove();
                        }


                    });
                }
            }
            return flag;
        },

        formatRelatedLinks: function(params){
            //var html ='<div id="related_links" style="width:'+params.width+'px;">';
            var html ='<div id="related_links" style="padding-right:8px;">';
            html += '<fieldset class="links-fieldset">';
            html += '<legend align="left">'+SMARTPORTAL.t("Related Links")+'</legend>';

            //html +='<ul>';
            for(var i=0;i<params.items.length;i++){
                html += '<div class="row_link"><div class="bullet_par"><div class="bullet"></div></div><div class="link"><a title="'+params.items[i].desc+'" href="javascript:void(0);" onclick="'+this.getWidgetInstanceName()+'.'+params.items[i].jsFunc+'">'+ params.items[i].title +'</a></div></div>';
            }
            //html+="</ul>";
            html += "</fieldset>";
            html += "</div>";

            return html;
        },
        include:function(filename,callback,widgetObj)
        {
            var LazyLoader = {};
            LazyLoader.timer = {};
            try{
                var body = document.getElementsByTagName('body').item(0);
                var crob = eval(this.getWidgetInstanceName());
                script = document.createElement('script');
                script.src = filename;
                script.type = 'text/javascript';
                body.appendChild(script);
                if(callback){
                    script.onreadystatechange = function () {
                        if(script.readyState=="complete" || script.readyState=='loaded'){ //IE Support
                            callback(script,widgetObj);
                        }
                    }
                    script.onload = function(){
                        callback(script,widgetObj);
                    }
                    if ((Prototype.Browser.WebKit && !navigator.userAgent.match(/Version\/3/)) || Prototype.Browser.Opera) { // safari
                        LazyLoader.timer[url] = setInterval(function() {
                            if (/loaded|complete/.test(document.readyState)) {
                                clearInterval(LazyLoader.timer[url]);
                                callback(script,widgetObj);
                            }
                        }, 10);
                    }
                }
            }catch(e){}
        },
      //0033772: Code Re-Factoring - Home page - Remove unwanted JavaScript
        includecss:function(filename)
        {
            try{
            	$('head').append('<link rel="stylesheet" href= "'+filename+'" type="text/css" />');
            }catch(e){}
        },

        /**
         * @desc convert a string to xmlobject
         * @param string - xml string
         * @return object - xml object
         */
        stringToXml:function(xmlstr){
            try //Internet Explorer
            {
              xmlDoc=new ActiveXObject("Microsoft.XMLDOM");
              xmlDoc.async="false";
              xmlDoc.loadXML(xmlstr);
            }
            catch(e)
            {
                try{
                    parser=new DOMParser();
                    xmlDoc = parser.parseFromString(xmlstr,"text/xml");
                }catch(e){
                    //alert(e.message);
                }

            }
            return xmlDoc;
        },

        renderNoRecs:function(mesg, buttString, func){
            var html='';
            if(buttString!=null && func!=null && buttString!=undefined && func!=undefined ){
                html ='<div style="display: block;"><table style="padding: 5px;width:100%;"><tr><td class="sp_erromsg" width="100%">'+SMARTPORTAL.t(mesg)+'</td><td align="right"><input type="button" class="spButtonSpan" onclick="' + func +';" value="'+ buttString +'" style="margin-left: 25px;"/></td></tr></table></div>';
            }else{
                html ='<div style="display: block;"><table style="width:100%;"><tr><td class="sp_erromsg" width="100%">'+SMARTPORTAL.t(mesg)+'</td><td></td></tr></table></div>';
            }
            return html;
        },


        openWindow:function(obj,callback,widgetObj){
            if(obj.options==undefined)
                obj.options='';
            if(obj.url==undefined)
                obj.url='';
            if(obj.name==undefined)
                obj.name=='_blank';

            var newWindow = window.open(obj.url,obj.name,obj.options);
            this.checkIfLoaded(newWindow,callback,widgetObj);

        },

        checkIfLoaded:function(obname,callback,widgetObj){
            if((window.navigator.appName).indexOf("Microsoft")>=0){
                if(obname.document.readyState=="complete"){
                    callback(obname,widgetObj);
                }else{
                    this.checkIfLoaded(obname,callback);
                }
            }else{
                obname.onload=callback(obname,widgetObj);
            }
        },

        pausescript:function(millis){
            var date = new Date();
            var curDate = null;
            do {
                curDate = new Date();
            }while(curDate - date < millis);
        },

        getLearnerDetail:function(prm){
        	var stdinfo=false;
        	jQuery.ajaxSetup({
	            xhr: function() {
	                    //return new window.XMLHttpRequest();
/*	                    try{
	                        if(window.ActiveXObject)
	                            return new window.ActiveXObject("Microsoft.XMLHTTP");
	                    } catch(e) { }

	                    return new window.XMLHttpRequest();*/
	         //AJAX xmlHTTPRequest to IE browser(all browsers)
	             if(window.ActiveXObject) {
	         	       try {
	         	    	   ajxobj = new ActiveXObject("Msxml2.XMLHTTP");
	         	       } catch (e) {
		         	       try {
		         	    	  ajxobj = new ActiveXObject("Microsoft.XMLHTTP");
		         	       } catch (e) {
		         	    	  ajxobj = false;
		         	       }
	         	      }
	         	  }else{
	         		 ajxobj = new XMLHttpRequest();
	        	  }
        	  return ajxobj;
	          }
	        });
        	
        	/* Added by ganeshbabuv on Sep 30th 2015 to pass the salesforce session variable to avoid to take the value (logged user details) from cookie. Instead of that, reload values from session varible. Ref :0054508 on 30th sep 2015 10:40 AM */
        	var sf_exp_ses_value = $('.salesforce-widget #widget').attr('data-exp-sess-id'); 
        	//alert('sf_exp_ses_value='+sf_exp_ses_value);
        	if (typeof sf_exp_ses_value !== typeof undefined && sf_exp_ses_value !== false) {
                var getvalue_url=resource.base_host+"/?q=getLearnerInfo/getvalue/"+prm+"/"+sf_exp_ses_value;
             }else{
            	var getvalue_url=resource.base_host+"/getLearnerInfo.php?getvalue="+prm;
             } 
        	
        	jQuery.ajax({
			   type: "GET",
			   url: getvalue_url,  //Changed according to cookieless option
			   processData: false,
			   dataType:'text',
			   contentType:'text/xml',
			   async: false,
			   cache: false,
			   success: function(respText){
					stdinfo = respText;
			   }
			 });
			return stdinfo;
        },

        getLearnerId:function(){
        	return this.getLearnerDetail("userid");
        },

        getUserName:function(){
        	return this.getLearnerDetail("username");
        },

        getUserFullName:function(){
        	return this.getLearnerDetail("userfullname");
        },

        getUserEmail:function(){
        	return this.getLearnerDetail("usermail");
        },

        getUserRole:function(){
        	return this.getLearnerDetail("userrole");
        },

        readCookie:function(cookieName){
            var theCookie=""+document.cookie;
            var ind=theCookie.indexOf(cookieName);
            var ind1=theCookie.indexOf(';',ind);
            if (ind1==-1) ind1=theCookie.length;
            var cookinfo = unescape(theCookie.substring(ind+cookieName.length+1,ind1));
            return cookinfo;
        },

        setReportHeader:function(){
            var html='';
            html +='<div class="report-header-left"></div><div class="report-header-middle">';
            html +='<a href="#" title="Home"><img src="'+themepath+'/logo.png" alt="Home" id="logo"></a>';
            html += '<div id="top-sign" style="padding: 0px 8px 0px 0px; position: relative; top: 25px; color: white; text-align: right; font-family: arial; font-size: 10px; font-weight: bold; text-decoration: none;">';
            html += '<br><br><br><br><br></div>';
            html +='</div><div class="report-header-right"></div>';
            return html;
        },

        setReportFooter:function(){
            var html ="";

            /*html += '<div class="copyright-page">';
            html += '<div class="ftrLeft">Copyright 2011 Expertus Inc. All rights reserved.&nbsp;&nbsp;&nbsp;</div>';
            html += '<div class="ftrRight"><a href="" target="_blank"><img src="sites/all/themes/core/AdministrationTheme/images/expertus_power.png"></a>';
            html += '</div></div>';*/

            html += '<div class="region region-footer"><div class="block block-menu contextual-links-region first odd" id="block-menu-menu-footer-menu">';
            html += '<h2 class="title">Footer Menu</h2><div class="content"><ul class="menu"><li class="first leaf"><a title="" href="/?q=node/1">About Us</a></li>';
            html += '<li class="leaf"><a title="" href="/?q=node/2">Feedback</a></li><li class="leaf"><a title="" href="/?q=node/5">Legal Notices</a></li>';
            html += '<li class="leaf"><a title="" href="/?q=node/3">Privacy Policy</a></li><li class="last leaf"><a title="" href="/?q=node/4">Trademark</a></li>';
            html += '</ul></div></div><div class="block block-exp-sp-footer contextual-links-region last even" id="block-exp-sp-footer-footer">';
            html += '<div class="content"><div id="footer"><div class="copyright-page">';
            html += '<div class="ftrLeft"><span class="FooterCpyText">v3.2.7.3GA | Copyright 2011 Expertus, Inc. All rights reserved.</span>';
            html += '</div><div class="poweredby"><a href="#"><img height="25" width="140" src="sites/all/themes/core/AdministrationTheme/images/powered-by.png"></a></div>';
            html += '</div></div></div></div></div>';
            return html;
        },

        setHourMin:function(totalminutes){

            var tHours = Math.floor(totalminutes/60);
            var tMinutes = totalminutes%60;

            if(tHours > 1) {
            var Hours = tHours + " Hours";
            }else{
            var Hours = tHours + " Hour";
            }

            if(tMinutes > 0) {
            var Minutes = ", " + tMinutes + " Mins" ;
            }else{
            var Minutes = '';
            }

            var Time = Hours + Minutes;

            return Time;
        },
        setDrupalCall:function(requestData){
			var widObj = this;
			var requestDataType ='';
			/*if(widObj.renderId === undefined || widObj.renderId === null)
				alert("renderId is undefined");*/
			if(requestData === undefined || requestData === null)
				requestData = {};
			if(requestData.returntype === undefined || requestData.returntype === null)
				requestDataType = "text";
			if(requestData.returntype == 'xml')
				requestDataType = "type/xml";

			this.currentAjaxRequest=$.ajax({
				type: "POST",
				url: resource.drupal_service_url,
				data:requestData,
				processData:true,
				dataType:requestDataType,
				async: true,
				beforeSend: function() {
					//widObj.createLoader(widObj.renderId);
				},
				success: function(outputData){
					//try
					//{
						if(requestData.returntype == "xml")
						{
							widObj.responseText=widObj.Trim(outputData);
							widObj.convertResponseXml(outputData);
							widObj.successHandler(outputData);
							widObj.destroyLoader(widObj.renderId);
						}
						else if(requestDataType == "text")
						{
							var json = eval("(" + outputData + ")");
							widObj.successHandler(json);
							//widObj.destroyLoader(widObj.renderId);
						}
					//}
					/*catch(error)
					{
						alert("Invalid Ajax callback function name."+error.toString());
					}*/
		  		},
		  		failure:function(msg){
		  			widObj.renderError();
		  		},
		  		error:function (XMLHttpRequest, textStatus, errorThrown) {
		  			widObj.renderError();
			   	}
			});
			return this.currentAjaxRequest;
		},
        setRedirectUrl:function(querystring){
        	if(querystring !='' && querystring != undefined)
        		window.location = this.constructUrl(querystring);
        	else
        		window.location = resource.base_url;
        },
        constructUrl:function(querystring){
        	if(querystring !='' && querystring != undefined)
        		return resource.base_url+'/?q='+querystring;
        	else
        		return resource.base_url;
        },
        windowOpen:function(url,title,width,height)
        {
        	title=title!=undefined?title:"ExpertusONE";
        	var windowWidth=width!=undefined?width:850;
        	var windowHeight=height!=undefined?height:800;
        	var windowName=title;
        	var centerWidth = (window.screen.width - windowWidth) / 2;
            var centerHeight = (window.screen.height - windowHeight) / 2;
            var windowOptions="toolbar=yes,menubar=no,scrollbars=yes,status=yes,location=no,resizable=1,width="+windowWidth+",height="+windowHeight+",left="+centerWidth+",top="+centerHeight;
            var newWindow = window.open(url,'',windowOptions);
            // newWindow.document.title=title;
            //newWindow.document.location.href=url;
            if(newWindow)
        	{
        		 newWindow.focus();
        		 return newWindow;
        	}
        	else
        	{
        		alert(SMARTPORTAL.t("Your browser blocked the popup. Disable popup blocker in your browser and try again."));
        	}
        },


        /**
         * Overridable method which is used to split the string.
         * @method this.spiltString
         * @param String,Int
         * @public
         */
		spiltString : function(str, n){
		    var split= str.indexOf(' ', n);
		    if(split== -1) return str;
		    return str.substring(0, split);
		},


        /**
		 * Overridable method which is used to get the updated time.
		*/
		modifiedTime:function(days, hrs, mins){
		   var timeStamp='';
			if(days > 0) {
				timeStamp = days + ' ' + SMARTPORTAL.t('days');
			if(hrs > 0) {
				timeStamp += ','+ hrs + ' ' + SMARTPORTAL.t('hours ago');
			}else{
				timeStamp += ','+ mins + ' ' + SMARTPORTAL.t('mins ago');
			}

			}else if(days == 0 && hrs > 0) {
			timeStamp = hrs + ' ' + SMARTPORTAL.t('hours')+','+ mins + ' ' + SMARTPORTAL.t('mins ago');
			}else{
			timeStamp = mins + ' ' + SMARTPORTAL.t('mins ago');
			}
          return timeStamp;
        },

        /**
		 * Overridable method which is used to get the popup for all alert messages.
		*/
		getAlertPopup:function(msg){
			$('#alertMsg-wizard').remove();
	        var html = '';
	        html+='<div id="alertMsg-wizard" style="display:none; padding: 0px;">';
	        html+='<table width="100%" border="0" valign="center">';
	        html+='<tr><td height="30"></td></tr>';
	        html+='<tr>';
	       	html+= '<td align="center"><span><b>'+msg+'<b></span><br /></td>';
	        html+='</tr>';
	        html+='</table>';
	        html+='</div>';
	        $("body").append(html);
	        var closeButt={};

		    closeButt[SMARTPORTAL.t('OK')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#alertMsg-wizard').remove();};

	        $("#alertMsg-wizard").dialog({
	            position:[(getWindowWidth()-500)/2,100],
	            bgiframe: true,
	            width:500,
	            resizable:false,
	            modal: true,
	            title:'Launch',
	            buttons:closeButt,
	            closeOnEscape: false,
	            draggable:false,
	            overlay:
	             {
	               opacity: 0.9,
	               background: "black"
	             }
	        });

	        $('.ui-dialog').wrap("<div class='portal-ui-element'></div>");
	        $('.ui-widget-overlay').wrap("<div class='portal-ui-element'></div>");
	        //$('.ui-dialog-titlebar-close').remove();

	        $("#alertMsg-wizard").css({height:'auto', width:'100%'});
	        $("#alertMsg-wizard").show();

			$('.ui-dialog-titlebar-close').click(function(){
	            $("#alertMsg-wizard").remove();
	        });

		}



};