// JavaScript Document
//Singleton SOAP Client
var SOAPClient = {
	url: "",
	SOAPServer: "",
	ContentType: "text/xml",
	CharSet: "utf-8",
	ResponseXML: null,
	ResponseText: "",
	Status: 0,
	ContentLength: 0,
	Namespace: function(name, uri) {
		return {"name":name, "uri":uri};
	},
	SendRequest: function(soapReq, callback,errorcallback) {		
		if(SOAPClient.url != null) {
			SOAPClient.ResponseText = "";
			SOAPClient.ResponseXML = null;
			SOAPClient.Status = 0;
			
			var content = soapReq.toString();
			SOAPClient.ContentLength = content.length;
			
			function getResponse(xData) {
				if(callback != null) {
					SOAPClient.Status = xData.status;
					SOAPClient.ResponseText = xData.responseText;
					SOAPClient.ResponseXML = xData.responseXML;
					callback(xData.responseXML);
				}
			}
			
			function getErrorResponse(xData)
			{
				errorcallback(xData.responseXML);
			}
			
			$.ajax({
				 type: "POST",
				 url: SOAPClient.url,
				 dataType: "xml",
				 processData: false,
				 data: content,
				 complete: getResponse,
				 error: getErrorResponse,
				 beforeSend: function(req) {
					req.setRequestHeader("Method", "POST");
				 	req.setRequestHeader("Content-Length", SOAPClient.ContentLength);
					req.setRequestHeader("Content-Type", SOAPClient.ContentType + "; charset=\"" + SOAPClient.CharSet + "\"");
					req.setRequestHeader("SOAPServer", SOAPClient.SOAPServer);
					req.setRequestHeader("SOAPAction", soapReq.Action);
				 }
			});
		}
	},	
	ToXML: function(soapObj) {
		var out = "";
		var isNSObj=false;
		try {
			if(soapObj!=null&&typeof(soapObj)=="object"&&soapObj.typeOf=="SOAPObject") {								
				//Namespaces
				if(soapObj.ns!=null) {
					if(typeof(soapObj.ns)=="object") {
						isNSObj=true;
						out+="<"+soapObj.ns.name+":"+soapObj.name;
						out+=" xmlns:"+soapObj.ns.name+"=\""+soapObj.ns.uri+"\"";
					} else  {
						out+="<"+soapObj.name;
						out+=" xmlns=\""+soapObj.ns+"\"";
					}
				} else {
					out+="<"+soapObj.name;
				}
				//Node Attributes
				if(soapObj.attributes.length > 0) {
					 var cAttr;
					 var aLen=soapObj.attributes.length-1;
					 do {
						 cAttr=soapObj.attributes[aLen];
						 if(isNSObj) {
						 	out+=" "+soapObj.ns.name+":"+cAttr.name+"=\""+cAttr.value+"\"";
						 } else {
							out+=" "+cAttr.name+"=\""+cAttr.value+"\"";
						 }
					 } while(aLen--);					 					 
				}
				out+=">";
				//Node children
				if(soapObj.hasChildren()) {					
					var cPos, cObj;
					for(cPos in soapObj.children){
						cObj = soapObj.children[cPos];
						if(typeof cObj == "object"){out+=SOAPClient.ToXML(cObj);}
					}
				}
				//Node Value
				if(soapObj.value != null){out+=soapObj.value;}
				//Close Tag
				if(isNSObj){out+="</"+soapObj.ns.name+":"+soapObj.name+">";}
				else {out+="</"+soapObj.name+">";}
				return out;
			}
		} catch(e){alert("Unable to process SOAPObject! Object must be an instance of SOAPObject");}
	}
}
//Soap request - this is what being sent using SOAPClient.SendRequest
//var SOAPRequest=function(action, soapObj) { //Arthi - changed to cater to soap headers - start
var SOAPRequest=function(action, soapObj,soapHdr) {
// Arthi - changed to cater to soap headers - end	
	this.Action=action;	
	var nss=[];
	//Arthi - changed to cater to soap headers - start
	//var headers=[];
	var headers=(soapHdr!=null)?[soapHdr]:[];
	//Arthi - changed to cater to soap headers - end
	var bodies=(soapObj!=null)?[soapObj]:[];
	this.addNamespace=function(ns, uri){nss.push(new SOAPClient.Namespace(ns, uri));}	
	this.addHeader=function(soapObj){headers.push(soapObj);};
	this.addBody=function(soapObj){bodies.push(soapObj);}
	this.toString=function() {
		var soapEnv = new SOAPObject("SOAP-ENV:Envelope");
		soapEnv.attr("xmlns:SOAP-ENC","http://schemas.xmlsoap.org/soap/encoding/");
		soapEnv.attr("xmlns:xsi","http://www.w3.org/2001/XMLSchema-instance");
		soapEnv.attr("xmlns:xsd","http://www.w3.org/2001/XMLSchema");
		soapEnv.attr("SOAP-ENV:encodingStyle","http://schemas.xmlsoap.org/soap/encoding/");
		soapEnv.attr("xmlns:SOAP-ENV","http://schemas.xmlsoap.org/soap/envelope/");
		//Add Namespace(s)
		if(nss.length>0){
			var tNs, tNo;
			for(tNs in nss){tNo=nss[tNs];if(typeof(tNo)=="object"){soapEnv.attr("xmlns:"+tNo.name, tNo.uri)}}
		}
		//Add Header(s)
		if(headers.length>0) {
			var soapHeader = soapEnv.appendChild(new SOAPObject("SOAP-ENV:Header"));
			//var tHdr;
			//for(tHdr in headers){soapHeader.appendChild(headers[tHdr]);}
			populateSoapBody(soapHeader,headers[0]); /* Vincent: soapBody is replaced instead of soapAction to avoid the sub level root */
		}
		//Add Body(s)
		if(bodies.length>0) {
			var soapBody = soapEnv.appendChild(new SOAPObject("SOAP-ENV:Body"));
			//var soapAction =soapBody.appendChild(new SOAPObject(action).attr("xmlns","null")); /* Vincent: for SmartPortal to avoid child root in request*/
			populateSoapBody(soapBody,bodies[0]); /* Vincent: soapBody is replaced instead of soapAction to avoid the sub level root */
		}
		return soapEnv.toString();		
	}
}

var populateSoapBody=function(soapBody,bodies)
{
	var tBdy;
	for(tBdy in bodies)
			{
		    	soapBody.appendChild(bodies[tBdy]);
			}
	
}

var isString=function(obj) 
{
	if (typeof(obj) == 'string') 
	{
		return true;
	}
	if (typeof(obj) == 'object') 
	{  
		var criterion =  obj.constructor.toString().match(/string/i); 
		return (criterion != null);  
	}
	return false;
}


//Soap Object - Used to build body envelope and other structures
var SOAPObject = function(name) {
	this.typeOf="SOAPObject";
	this.ns=null;
	this.name=name;
	this.attributes=[];
	this.children=[]
	this.value=null;
	this.attr=function(name, value){this.attributes.push({"name":name, "value":value});return this;};
	this.appendChild=function(obj){this.children.push(obj);return obj;};
	this.hasChildren=function(){return (this.children.length > 0)?true:false;};

	this.EncodeXmlSpecialChar=function(pmStr)
	{  		  
		if(typeof(pmStr)=="string"){
			
		  //pmStr = pmStr.replace('&','&amp;').replace('<','&lt;').replace('>','&gt;').replace('\'','&apos;').replace('"','&quot;');
			pmStr = Drupal.checkPlain(pmStr);   
		  
		}
		return pmStr;   
	}   
 
	this.val=function(v)
		{
			if(v==null)
				{ 
					return this.value
				}
			else
				{  //vivekanandan : for SmartPortal Encode  special XML characters     
					this.value=	this.EncodeXmlSpecialChar(v);     
					return this;    
				}
		};	
	this.toString=function(){return SOAPClient.ToXML(this);}		
}