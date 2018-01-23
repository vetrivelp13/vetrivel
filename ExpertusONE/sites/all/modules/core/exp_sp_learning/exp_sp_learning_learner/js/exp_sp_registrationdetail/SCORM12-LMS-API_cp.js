/*
 *
 * between LMS & SCO.
 * 
 */
   //LMS SCORM Adaptor API - Starts
    function SCORM_API_12() 
    {
         try{
        	 
         }catch(e){
 			// to do
         }
    }
    SCORM_API_12.prototype._version = "SCORM 1.2 LMSAPI1.0.0";
    SCORM_API_12.prototype._desc = "SCORM 1.2 LMS API Adaptor Version 1.0.0";
    SCORM_API_12.prototype._callback = "";
    SCORM_API_12.prototype._idleCallback = "";
    SCORM_API_12.prototype._resetvalue = "";
    SCORM_API_12.prototype._isLMSInitialized = "false";
    SCORM_API_12.prototype._cmiBooleanFalse = "";
    SCORM_API_12.prototype._cmiBooleanTrue = "" ;
    //current error code
    SCORM_API_12.prototype._errorCode = 0;
    SCORM_API_12.prototype._valueStr = "";
    SCORM_API_12.prototype._errStr = "";
    SCORM_API_12.prototype._CMIDATA = new Array();
    SCORM_API_12.prototype._ErrorCodes = {"201":"Invalid argument error",
                                          "202":"Element cannot have children","203":"Element not an array. Cannot have count.",
                                          "301":"Not initialized","401":"Not implemented error",
                                          "402":"Invalid set value, element is a keyword","403":"Element is read only.",
                                          "404":"Element is write only","405":"Element is write only",
                                           "0":"No Error.The previous LMS API Function call completed successfully.",
                                           "101":"General Exception.An unspecified, unexpected exception has occured"
                                         };
    

    /*
    * Method: LMSInitialize(String param) 
    * Input: String param - must be null string - reserved for future use 
    * Output: CMIBoolean  "false" if fails, "true" if succeeds
    * This function must be called by a SCO before any other API calls are made.
    * It can not be called more than once  consecutively unless LMSFinish is called.
    */
    SCORM_API_12.prototype.LMSInitialize = function(param,callFrom)
    {
    	try{
        debugLog('inside actual LMSInitialize function');
        result = this._cmiBooleanFalse;  // assume failure
        // Make sure param is empty string "" - as per the API spec
        if  (param !="")  
        {
              this._errorCode = 201;
              this._isLMSInitialized = "false";
        }
        this._isLMSInitialized = "true";
        var rtn = this._isLMSInitialized;
        if(rtn == 'true' && callFrom != 'LMS'){ this.LMSSetValue("cmi.core.lesson_status",this._resetvalue);};
        return rtn;
    	}catch(e){
			// to do
    	}
    };
    /*
    * Method: LMSFinish(String param) 
    * Input: none Output: none Description: Signals completion of communication with LMS
    * Now call a javascript function that should be present in the window this javascript is located in
    * that will change the SCO content frame.
    */
    SCORM_API_12.prototype.LMSFinish = function(param) 
    {
    	try{
        debugLog('inside actual LMSFinish function');
        if (param =="")
        {
            result = this.LMSCommit("");
            //alert(2);
            if (!result)
            {
                //log into debug console
                debugLog("Error Occured in LMSFinish while calling LMSCommit; ErrorCode:" + this._errorCode);
                this._errorCode = 201;
            
                return "false";
            }
            else
            {
                debugLog('inside LMSCommit is succeeded');
                this._isLMSInitialized = "false";
                result = this._cmiBooleanTrue;  // successful completion
                //now call the callback defined by the Caller.;
                //eval(this._callback+'()');
                if(this._callback!='' && this._callback!= undefined ){
                    this._callback();
                }
                //return true;
            
                
            }
        }
        else
        {
            
                this._errorCode = 201;
                debugLog("Error Occured in LMSFinish. ErrorCode:" + this._errorCode);
                return "false";
        }   

        return "true";  
    	}catch(e){
			// to do
    	}
    };
    /*
    * Method: LMSCommit(String param) 
    * Input: none Output: none Description: Applies the SCO data model elements set using LMSSetValue to LMS data model 
    * elements and saves them (on the server). Call function to save the data in your SmartPortal LMS.
    */
    SCORM_API_12.prototype.LMSCommit = function(param) 
    {
    	try{
            //Invoke LMS Web Service to save the learner's score and other details in database
            debugLog('inside LMSCommit');
            return "true";
    	}catch(e){
			// to do
    	}
    };
    SCORM_API_12.prototype.LMSGetValue = function(param) 
    {
    	try{
            var value = this._CMIDATA[param];
         //   if(param == 'cmi.core.lesson_status') console.log("GETValue == "+value);
            if (value==null)
            {
                value = " ";
            }
            //return this._CMIDATA[param];
            return value;
    	}catch(e){
			// to do
    	}
    };

    SCORM_API_12.prototype.LMSSetValue = function(param,value) 
    {
    	try{
           debugLog("==> param:" + param + " value:" + value);
           this._CMIDATA[param] = value;
           if(param!=null && param!=undefined && param!=''
               && value!=null && value!=undefined && value!='')
               return "true";
           else
               return "false";
           
           //alert('array length:' + this._CMIDATA.length);
    	}catch(e){
			// to do
    	}
    };
    
    SCORM_API_12.prototype.LMSGetLastError = function() 
    {
    	try{
         return this._errorCode;
    	}catch(e){
			// to do
    	}
    };
    SCORM_API_12.prototype.LMSGetLastErrorString = function() 
    {
    	try{
        var errstr = this._ErrorCodes[this._errorCode];
        if (errstr==null)
        {
            errstr = "Undefined Error Code";
        }
         return errstr;
    	}catch(e){
			// to do
    	}
    };
    /*
    * Method: LMSGetDiagnostic(String param) 
    * Input: none 
    * Output: Any additional diagonistic information provided by LMS Vendor.  
    */
    SCORM_API_12.prototype.LMSGetDiagnostic = function() 
    {
    	try{
         return " ";
    	}catch(e){
			// to do
    	}
    };
    SCORM_API_12.prototype.getAllCMIData = function() 
    {
    	try{
         return this._CMIDATA;
    	}catch(e){
			// to do
    	}
    };
    SCORM_API_12.prototype.setCallBack = function (callbackfn)
    {
    	try{
        this._callback = callbackfn;
    	}catch(e){
			// to do
    	}
    };
    SCORM_API_12.prototype.setIdleCallBack = function (callbackfn)
    {
    	try{
        this._idleCallback = callbackfn;
    	}catch(e){
			// to do
    	}
    };
    
    SCORM_API_12.prototype.resetValue = function (value)
    {
    	try{
        this._resetvalue = value;
    	}catch(e){
			// to do
    	}
    };
    
    SCORM_API_12.prototype.LMSGetErrorString = function(isErrorNumber) 
    {
    	try{
        if(isErrorNumber == null || isErrorNumber == undefined || isErrorNumber == "" || isErrorNumber == "null")
          isErrorNumber = this._errorCode;
          
        var errstr = this._ErrorCodes[isErrorNumber];
        if (errstr==null)
        {
            errstr = "Undefined Error Code";
        }
         return errstr;
    	}catch(e){
			// to do
    	}
    };
    //LMS SCORM Adaptor API - Ends
    var findAPITries = 0;
    function findAPI(win)
    {
    	try{
        // Check to see if the window (win) contains the API
        // if the window (win) does not contain the API and
        // the window (win) has a parent window and the parent window
        // is not the same as the window (win)
        debugLog("inside findAPI");
        while ( (win.API == null) && (win.parent != null) && (win.parent != win) )
        {
               // increment the number of findAPITries
               findAPITries++;
            
               // Note: 7 is an arbitrary number, but should be more than sufficient
               if (findAPITries > 7)
               {
                  debugLog("Error finding API -- too deeply nested.");
                  return null;
               }
            
               // set the variable that represents the window being
               // being searched to be the parent of the current window
               // then search for the API again
               win = win.parent;
        }
        return win.API;
    	}catch(e){
			// to do
    	}
    };

    function getAPI()
    {
    	try{
           debugLog("inside getAPI");
           // start by looking for the API in the current window
           var theAPI = findAPI(window);
        
           // if the API is null (could not be found in the current window)
           // and the current window has an opener window
           if ( (theAPI == null) && (window.opener != null) && (typeof(window.opener) != "undefined") )
           {
              // try to find the API in the current window’s opener
              theAPI = findAPI(window.opener);
           }
           // if the API has not been found
           if (theAPI == null)
           {
              // Alert the user that the API Adapter could not be found
              debugLog("Unable to find an API adapter");
           }
           return theAPI;
    	}catch(e){
			// to do
    	}
    }
    function debugLog(msgstr)
    {
    	try{
        var debugConsoleId = document.getElementById("debugConsole");
        var timestamp = new Date().toString();
        if (debugConsoleId != null)
        {
            var logmessage = document.getElementById("debugConsole").innerHTML;
            document.getElementById("debugConsole").innerHTML = logmessage + "<br>" + timestamp + " " + msgstr ;
        }
    	}catch(e){
			// to do
    	}
    }
