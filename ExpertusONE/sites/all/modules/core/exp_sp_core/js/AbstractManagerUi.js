/**
 * The Abstract Manager is the base component of LPP framework. It consists the base LPP framework libary methods.
 * @module BaseWidget
 * @requires JQuery Lib
 * @title AbstractManager
 * @beta
 */


/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

/**
 * AbstractManager  class for the LPP Learning widgets
 * @namespace EXPERTUS.widget
 * @class AbstractManager
 * @uses JQuery Lib
 * @constructor
 */

//registernamespace("EXPERTUS.SMARTPORTAL.AbstractManager");
var EXPERTUS_SMARTPORTAL_AbstractManager ={

		/**
	    * If it is false it makes the sync request to the LMS system.
	    * @property this.asyncReq
	    * @type Boolean -
	    * @default true
	    * @Instance level variable
		*/
		asyncReq:true,


		/**
	    * It is used to persist the requestXml of the web service.
	    * @property this.requestXml
	    * @type String - Value hold as xml format.
	    * @default ""
	    * @Instance level variable
		*/
		requestXml:"",

		/**
	    * It is used to track the http request connection status.
	    * @property this.connectionStatus
	    * @type String - http status value.
	    * @default ""
	    * @Instance level variable
		*/
		connectionStatus:"",

		/**
	    * It is used to hold the http object.
	    * @property this.httpRequest
	    * @type Object - xmlhttprequest object.
	    * @default null
	    * @Instance level variable
		*/
		httpRequest:null,

		/**
	    * Web  service URL.
	    * @property this.url
	    * @type String - url
	    * @default ""
	    * @Instance level variable
		*/
		url:"",

		/**
	    * Web  service name which gets invoked in the Ajax request.
	    * @property this.soapAction
	    * @type String - web service name
	    * @default ""
	    * @Instance level variable
		*/
		soapAction:"",

		/**
	    * Method name of the http request.
	    * @property this.methodName
	    * @type String - GET/POST/PUT
	    * @default ""
	    * @Instance level variable
		*/
		methodName:"",

		/**
	    * Service name.
	    * @property this.serviceName
	    * @type String -
	    * @default null
	    * @Instance level variable
		*/
		actionKey:null,

		/**
	    * Callback handler of the http request. It hold the methods for success and failure scenario.
	    * @property this.callback
	    * @type {Object}
	    * @default ""
	    * @Instance level variable
		*/
		callback:"",

		/**
	    * Timeout value of the http request.
	    * @property this.timeOut
	    * @type {int}
	    * @default null
	    * @Instance level variable
		*/
		timeOut:null,

		/**
	    * Web services related parameters hold into this variable.
	    * @property this.serviceAttributes
	    * @type {named argument}
	    * @default null
	    * @Instance level variable
		*/
		serviceAttributes:null,

		/**
	    * Context path of the LPP framework
	    * @property this.xmlResponseBaseURL
	    * @type {String} - URL
	    * @default _ It inherits from config.js
	    * @Instance level variable
		*/
		xmlResponseBaseURL:null,

		/**
	    * Used to hold the widget object
	    * @property this.widgetObject
	    * @type {Object} - URL
	    * @Instance level variable
		*/
		widgetObject:null,

		/**
	    * Used to hold the callback function of ajax call
	    * @property this.callback
	    * @type {string} - funcion
	    * @Instance level variable
		*/
		callback:null,

		/**
	    * Used to hold the id of the div where the loader will place
	    * @property this.loaderobj
	    * @type {string} - id
	    * @Instance level variable
		*/
		loaderobj:null,

		/**
	    * Overridable method used to initialize abstract manager variables
	    * @property this.initialize
	    * @param {Obj} - Detail widget object
	    * @Instance level variable
		*/
		initialize:function(obj){
			this.setwidgetObject(obj);
			this.setActionKey(obj.getActionKey());
			this.setEndPointURL(obj.getEndPointURL());

		},

		/**
	    * Overridable method used to set the loader id
	    * @property this.setLoaderObj
	    * @param {String} - id
	    * @Instance level variable
		*/
		setLoaderObj:function(){
			this.loaderobj=id;
		},

		/**
	    * Overridable method used to get the loader id
	    * @property this.setLoaderObj
	    * @param {String} - id
	    * @Instance level variable
		*/
		getLoaderObj:function(){
			return this.loaderobj;
		},

		/**
	    * Overridable method used to set the widget object
	    * @property this.initialize
	    * @param {Obj} - Detail widget object
	    * @Instance level variable
		*/
		setwidgetObject:function(obj){
			this.widgetObject=obj;
		},

		/**
		 * Overridable method used to set the callback funtion for
		 * ajax call.
		 * @property this.setCallBack()
		 * @param String - call back function name
		 * @param obj - arguments list (if any)
		 * @Instance level variable
		 */

		setCallBack:function(fnName,obj){
			this.callback = fnName;
			if(obj!=null && obj!=undefined && obj!='' && typeof obj == 'object' ){
				var t=this.convertJsonToString(obj);
				this.callback = this.callback+"("+t+")";
			}else if(obj!=null && obj!=undefined && obj!='' && typeof obj == 'string' ){
				this.callback = this.callback+"(\""+obj+"\")";
			}else{
				this.callback = this.callback+"()";
			}
		},
		
		convertJsonToString:function(obj){
			var t='';
			if(obj!=null && obj!=undefined && obj!='' && typeof obj == 'object' ){
				t = "{";

				for (var key in obj){
					if(t.length>2)
						t+=",";
					var tmp=(typeof obj[key]=='object')?this.convertJsonToString(obj[key]):
						(typeof obj[key]=='string'?(obj[key].indexOf('"')>0?"'"+obj[key]+"'":'"'+obj[key]+'"'):obj[key]);
					tmp=String(tmp).length>0?tmp:'""';
					t += '"'+key+'":'+tmp;
				}
				t += "}";
			}
			return t;
		},

		/**
		 * Overridable method used to get the callback funtion for
		 * ajax call.
		 * @property this.setCallBack()
		 * @Instance level variable
		 */

		getCallBack:function(){
			return this.callback;
		},

		/**
	    * Overridable method used to get the widget object
	    * @property this.initialize
	    * @return {Obj} - Detail widget object
	    * @Instance level variable
		*/
		getwidgetObject:function(){
			return this.widgetObject;
		},

 		/**
		* Overridable method which is used to set the appropriate parameter for web service request and initiate the request from sub widget.
		* @method this.execute
		* @param {namedargument} - Service related paramaters.
		* @public
		*/

		execute:function()
		  {
				var returnFunction = (this.getCallBack()==null || this.getCallBack()==undefined)?'renderResults()':this.getCallBack();
			  	var widgetObj = this.getwidgetObject();
			  	var loader = '';

			  	if(this.getLoaderObj()!=null && this.getLoaderObj() != undefined && this.getLoaderObj() != 'null'){
				  	if(document.getElementById(this.getLoaderObj())!=null)
				  		loader = this.getLoaderObj()
				  	else
				  		loader = widgetObj.getUniqueWidgetId()+((this.getLoaderObj()!=null && this.getLoaderObj()!=undefined)?this.getLoaderObj():'');
			  	}else{
			  		loader = null;
			  	}
			  	if(loader!=null)
			  		widgetObj.createLoader(loader);
			  	$('#msgDiv').css('display','none');
			  	var mgr=this;
			  	// Implemented for Ajax Request failing in IE8
			  	jQuery.ajaxSetup({
		            xhr: function() {
		                    //return new window.XMLHttpRequest();
		                    try{
		                        if(window.ActiveXObject)
		                            return new window.ActiveXObject("Microsoft.XMLHTTP");
		                    } catch(e) { }

		                    return new window.XMLHttpRequest();
		                }
		        });
				jQuery.ajax({
				   type: "POST",
				   url: this.url,
				   processData: false,
				   data:  this.requestXml,
				   dataType:'text',
				   contentType:'text/xml',
				   async: true,
				   //Arthi - added to send the soapaction in the request header - start
				   beforeSend: function(req) {
						req.setRequestHeader("SOAPAction",mgr.getActionKey());
						req.setRequestHeader("Accept-Encoding", "gzip, deflate");
					 },
					 //Arthi - added to send the soapaction in the request header - end
				   success: function(respText){
			  			widgetObj.responseText=widgetObj.Trim(respText);
			  			widgetObj.convertResponseXml();
			  			if(loader!=null)
			  				widgetObj.destroyLoader(loader);
			  			eval('widgetObj.'+returnFunction);
			  			//widgetObj.renderResults();
			  		},
			  		failure:function(msg){
			  			widgetObj.responseText=widgetObj.Trim(msg);
			  			widgetObj.convertResponseXml();
			  			if(loader!=null)
			  				widgetObj.destroyLoader(loader);
			  			widgetObj.renderErrorCommon();
				   },error:function (XMLHttpRequest, textStatus, errorThrown) {
					   widgetObj.responseText=widgetObj.Trim(XMLHttpRequest.responseText);
					   widgetObj.convertResponseXml();
					   if(loader!=null)
						   widgetObj.destroyLoader(loader);
					   widgetObj.renderErrorCommon();
				   	 }
				 });

		  },


		/**
		* Overridable method which is used to get the request xml of the xmlhttprequest.
		* @method this.getRequestXML
		* @return {String} - Request string in xml format
		* @public
		*/
		getRequestXML:function()
		 {
			return this.requestXml;
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
		* Overridable method which is used to get the response xml of the xmlhttprequest.
		* @method this.getResponseXML
		* @return {Object} - Response object.
		* @public
		*/
		getResponseXML:function()
		 {
			return this.responseXml;
		 },


		/**
		* Overridable method which is used to get the request object of the xmlhttprequest.
		* @method this.getRequestObj
		* @return {Object} - Request object.
		* @public
		*/
		getRequestObj:function()
		 {
			return this.httpRequest;
		 },

		/**
		* Set the request method name.
		* @method this.setMethodName
		* @param {String} - GET/POST/PUT
		* @public
		*/
		setMethodName:function(methodname)
		 {
			this.methodName = methodname;
		 },

		/**
		* Overridable method which is used to set the  service number or name .
		* @method this.setServiceName
		* @param  {String} - Service number or name
		* @public
		*/
		setActionKey:function(no){
			this.actionKey=no;
		},

		/**
		* Overridable method which is used to get the  service number or name .
		* @method this.getServiceName
		* @return  {String} - Service number or name
		* @public
		*/
		getActionKey:function(){
			return this.actionKey;
		},

		/**
		* Set the soap action name.
		* @method this.setSOAPAction
		* @param {String} - Web service name
		* @public
		*/
		setSOAPAction:function(soapaction)
		 {

			this.soapAction = soapaction;
		 },

		/**
		* Set the end point URL.
		* @method this.setEndPointURL
		* @param {String} - URL
		* @public
		*/
        setEndPointURL:function(url)
         {
			 var widgetObj = this.getwidgetObject();
			 if(widgetObj.getUniqueWidgetId().toLowerCase().indexOf('lnr')>=0 || widgetObj.getUniqueWidgetId().toLowerCase().indexOf('team')>=0){
				 this.url = url + widgetObj.getUniqueWidgetId();
			 }else{
				 this.url = url + this.generateQueryString();
			 }

         },
         /**
         * Method used to set the Query String which append to the end point url.
         * @method this.generateQueryString
         * @param {String}
         * @public
         */
         generateQueryString:function(){
            var qstr = "";
            var widgetObj = this.getwidgetObject();
            if(this.getActionKey()!=null && this.getActionKey()!=undefined && this.getActionKey().length>0)
            	//Arthi - changed toremove the service no from QueryString - start
                qstr += qstr.length>0?"&actionkey="+this.getActionKey():"?actionkey="+this.getActionKey();
            	//Arthi - changed toremove the service no from QueryString - end
                if(config.data[2].licensekey[0].admin_licensekey!=null && config.data[2].licensekey[0].admin_licensekey!=undefined && config.data[2].licensekey[0].admin_licensekey!=''){
                    qstr += qstr.length>0?"&licensekey="+config.data[2].licensekey[0].admin_licensekey:"?licensekey="+config.data[2].licensekey[0].admin_licensekey;
                	var srvno=eval('config.data[3].SCServiceno[0].'+widgetObj.getUniqueWidgetId());
                    qstr += srvno==undefined?"&SCServiceno="+config.data[3].SCServiceno[0].all:"&SCServiceno="+srvno;

            }
            return qstr;
         },
		/**
		* Set the call back functions.
		* @method this.setCallbackFunctions
		* @param {Object}
		* @public
		*/
		setCallbackFunctions:function(callback)
		 {
			this.callback = callback;
		 },

		 getLovRequest:function(obj){
				var widgetObj = this.getwidgetObject();
				this.setActionKey(eval('config.data[1].serviceaction[0].PickListService'));
				this.setEndPointURL(widgetObj.getEndPointURL());
				var request=new Object();
				request.query = new SOAPObject("PickList").attr("xsi:type","null");
				if(obj!=undefined && obj.length>0){
					for(var i=0;i<obj.length;i++){
						var request1=new Object();
						var code = (obj[i].code!=null && obj[i].code!=undefined)?obj[i].code:'';
						var language = (obj[i].language!=null && obj[i].language!=undefined)?obj[i].language:'';
						request1.query = new SOAPObject("Lists").attr("xsi:type","null");
						request1.query.appendChild(new SOAPObject("Code").attr("xsi:type","null").val(code));
						request1.query.appendChild(new SOAPObject("Language").attr("xsi:type","null").val(language));
						request.query.appendChild(request1.query);
					}
				}
				//var sr = new SOAPRequest("PickList", request); Arthi - changed to include SOAP headers
				var sr = new SOAPRequest("PickList",request,this.getSOAPHeader());
				sr="<?xml version='1.0' encoding='UTF-8'?>"+sr;
				sr=sr.toString();
				return sr;
			},

		getLinkPathRequest:function(obj){
				var widgetObj = this.getwidgetObject();
				this.setActionKey(eval('config.data[1].serviceaction[0].LinkPathService'));
				this.setEndPointURL(widgetObj.getEndPointURL());
				var request=new Object();
				request.query = new SOAPObject("LinkPath").attr("xsi:type","null");
				request.query.appendChild(new SOAPObject("LinkTitle").attr("xsi:type","null").val(obj.LinkTitle));
				var sr = new SOAPRequest("LinkPath",request,this.getSOAPHeader());
				sr="<?xml version='1.0' encoding='UTF-8'?>"+sr;
				sr=sr.toString();
				return sr;
			},

		//Arthi - Added to generate the generate SOAP request, with SOAP headers  - start//
		getSOAPRequest:function(action,soapObj){
			var sr = new SOAPRequest(action,soapObj,this.getSOAPHeader());
			sr = "<?xml version='1.0' encoding='UTF-8'?>" + sr;
			sr = sr.toString();
			return sr;
		},

		getSOAPHeader:function(){
			var hdr = new Object();
			hdr.query = new SOAPObject("MessageHeaderInfo").attr(
					"xsi:type", "null");
			hdr.query.appendChild(new SOAPObject("PortalLicenseKey").attr("xsi:type","null").val(this.getCookie("SPLearnerLicense")));
			hdr.query.appendChild(new SOAPObject("PortalAuthKey").attr("xsi:type","null").val(this.getCookie("SPCertificate")));
			return hdr;
		},

		getCookie:function(c_name){
			if (document.cookie.length>0)
			  {
			  c_start=document.cookie.indexOf(c_name + "=");
			  if (c_start!=-1)
			    {
			    c_start=c_start + c_name.length+1;
			    c_end=document.cookie.indexOf(";",c_start);
			    if (c_end==-1) c_end=document.cookie.length;
			    return unescape(document.cookie.substring(c_start,c_end));
			    }
			  }
			return "";
		}
		//Arthi - Added to generate the generate SOAP request, with SOAP headers  - end


};
