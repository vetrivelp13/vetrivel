/**
 * @file
 * Send h5p statements to statement relay
 */
var isRequestOn = false;
(function ($) {
    Drupal.behaviors.h5pTincanBridge = {
        attach: function (context, settings) {
            if(window.H5P)
            {
            	
                var moduleSettings = settings.h5pTincanBridge;
                //console.log(JSON.stringify(moduleSettings));
                H5P.externalDispatcher.on('xAPI', function (event) {
                
                	if(isRequestOn){
                		return false;
                	}

                    var data = {
                        token: moduleSettings.token,
                        statement: JSON.stringify(event.data.statement)
                    };
                    //$.post(moduleSettings.relayUrl, data);
		            isRequestOn = true;            
		            $.ajax(
		            {
		            	url: "?q=ajax/tincanapi/relay",
		            	method:"POST", 
		            	data:data,
		            	success: function(result){
							isRequestOn = false;
					    }
					});        
                });
            }
        }
    };
}(jQuery));
