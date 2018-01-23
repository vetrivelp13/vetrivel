/*
 *
 * between LMS & SCO.
 * 
 */
 
   //LMS SCORM Adaptor API - Starts
    function SCORM_API_2004() 
    {
    	try{
    		
    	}catch(e){
			// to do
    	}
         
    }
    
    SCORM_API_2004.prototype._version = "SCORM 2004 LMSAPI1.0.0";
    SCORM_API_2004.prototype._desc = "SCORM 2004 LMS API Adaptor Version 1.0.0";
    SCORM_API_2004.prototype._callback = "";
    SCORM_API_2004.prototype._isInitialized = "false";
    SCORM_API_2004.prototype._cmiBooleanFalse = "";
    SCORM_API_2004.prototype._cmiBooleanTrue = "" ;
    //current error code
    SCORM_API_2004.prototype._errorCode = 0;
    SCORM_API_2004.prototype._valueStr = "";
    SCORM_API_2004.prototype._errStr = "";
    SCORM_API_2004.prototype._CMIDATA = new Array();
    SCORM_API_2004.prototype._ErrorCodes = {"201":"Invalid argument error",
                                          "202":"Element cannot have children","203":"Element not an array. Cannot have count.",
                                          "301":"Not initialized","401":"Not implemented error",
                                          "402":"Invalid set value, element is a keyword","403":"Element is read only.",
                                          "404":"Element is write only","405":"Element is write only",
                                           "0":"No Error.The previous LMS API Function call completed successfully.",
                                           "101":"General Exception.An unspecified, unexpected exception has occured"
                                         };
    

    /*
    * Method: Initialize(String param) 
    * Input: String param - must be null string - reserved for future use 
    * Output: CMIBoolean  "false" if fails, "true" if succeeds
    * This function must be called by a SCO before any other API calls are made.
    * It can not be called more than once  consecutively unless Finish is called.
    */
    SCORM_API_2004.prototype.Initialize = function(param) 
    {
    	try{
        debugLog('inside actual Initialize function');
        result = this._cmiBooleanFalse;  // assume failure
        // Make sure param is empty string "" - as per the API spec
        if  (param !="")  
        {
              this._errorCode = 201;
              this._isInitialized = "false";
        }else{
            this._isInitialized = "true";
        }
        return this._isInitialized;
    	}catch(e){
			// to do
    	}
    };
    
    /*
    * Method: Finish(String param) 
    * Input: String param - must be null string - reserved for future use 
    * Output: CMIBoolean  "false" if fails, "true" if succeeds
    * Description: Signals completion of communication with LMS
    * Now call a javascript function that should be present in the window this javascript is located in
    * that will change the SCO content frame.
    */
    SCORM_API_2004.prototype.Terminate = function(param){
    	try{
        debugLog('inside actual Terminate function');
        if (param =="")
        {
            result = this.Commit("");
            //alert(2);
            if (!result)
            {
                //log into debug console
                debugLog("Error Occured in Terminate while calling Commit; ErrorCode:" + this._errorCode);
                this._errorCode = 201;
            
                return "false";
            }
            else
            {
                debugLog('inside Commit is succeeded');
                this._isInitialized = "false";
                result = this._cmiBooleanTrue;  // successful completion
                //now call the callback defined by the Caller.;
                //eval(this._callback+'()');
                this._callback();
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
    * Method: Commit(String param) 
    * Input: none Output: none Description: Applies the SCO data model elements set using LMSSetValue to LMS data model 
    * elements and saves them (on the server). Call function to save the data in your SmartPortal LMS.
    */
    SCORM_API_2004.prototype.Commit = function(param) 
    {
    	try{
            //Invoke LMS Web Service to save the learner's score and other details in database
            debugLog('inside LMSCommit');
            if  (param !="")  
            {
                  this._errorCode = 201;
                  this._isInitialized = "false";
            }else{
                this._errorCode = 0;
                this._isInitialized = "true";
            }
            return this._isInitialized;
    	}catch(e){
			// to do
    	}
    };
    
    SCORM_API_2004.prototype.GetValue = function(param) 
    {
     try{	
        if(param!=""){  
            var value = this._CMIDATA[param];
                if (value==null)
                {
                    value = " ";
                }
                //return this._CMIDATA[param];
        }else{
            this._errorCode = 301;
            value = ""; 
        }
        return value;
     }catch(e){
			// to do
 	 }
    };

    SCORM_API_2004.prototype.SetValue = function(param,value) 
    {
    	try{
             debugLog("==> param:" + param + " value:" + value);
           this._CMIDATA[param] = value;
           if(param!=null && param!=undefined && param!='')
               return "true";
           else{
               this._errorCode = 351;
               return "false";
           }
           
           //alert('array length:' + this._CMIDATA.length);
    	}catch(e){
			// to do
    	}
    };
    
    SCORM_API_2004.prototype.GetLastError = function() 
    {
    	try{
         return this._errorCode;
    	}catch(e){
			// to do
    	}
    };
    SCORM_API_2004.prototype.GetErrorString = function(errcode) 
    {
      try{	
        var errstr = this._ErrorCodes[errcode];
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
    SCORM_API_2004.prototype.GetDiagnostic = function() 
    {
     try{	
         return " ";
     }catch(e){
			// to do
	  }
    };
    SCORM_API_2004.prototype.getAllCMIData = function() 
    {
    	try{
         return this._CMIDATA;
    	}catch(e){
			// to do
  	  	}
    };
    SCORM_API_2004.prototype.setCallBack = function (callbackfn)
    {
    	try{
        this._callback = callbackfn;
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
