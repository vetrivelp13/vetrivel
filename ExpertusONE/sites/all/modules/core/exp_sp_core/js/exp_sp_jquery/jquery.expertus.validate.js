(function($) {	
	jQuery.fn.expertusValidate = function (aDefn){
		var defn = jQuery.extend({					
			'type':'text'			
		}, aDefn); 
		return this.each(function(){
			$.fn.expertusValidateRender(this.id,aDefn);
		});
   };
   
   jQuery.fn.expertusValidateRender = function (objId,tipDefn){	  
	   $('#'+objId).qtip({
		   api:{
			   	beforeShow:function(){
			   		var self=this;
			   		var type=tipDefn.type;
			   		var valEntered=this.elements.target.val()
			   		var isValid=true;			   		
			   		switch(type){
			   			case 'email':
			   				isValid=validateField(valEntered,'email');		   				
			   				if(!isValid){
			   					self.updateContent(SMARTPORTAL.t('Invalid Email'),false);
			   				}
			   				break;
			   			case 'text':
			   				isValid=validateField(valEntered,'text');
			   				if(!isValid){
			   					self.updateContent(SMARTPORTAL.t('Invalid Text'),false);
			   				}
			   				break;
			   			case 'date':
			   				break;
			   			case 'num':
			   				isValid=validateField(valEntered,'num');
			   				if(!isValid){
			   					self.updateContent(SMARTPORTAL.t('Invalid Number'),false);
			   				}
			   				break;
			   		}
			   		return !isValid;				
		   		}
	   	},
   		show:'blur',
   		hide: { 
           when:{
                event:'unfocus'
           },
       		effect:'slide'                                                                                          
		},
		position: {
		     corner: {
					target: 'rightMiddle',
		            tooltip: 'leftTop'
		     }
		},
		style:{
			background:'#F2F2F2',                                      
			border: {                                            
				color: '#4F4F7A'                                                        
	     	},
		    tip:{
		        corner: 'leftTop', // We declare our corner within the object using the corner sub-option
		        color: '#4F4F7A',
		        size: {
		               x: 10, // Be careful that the x and y values refer to coordinates on screen, not height or width.
		               y : 4 // Depending on which corner your tooltip is at, x and y could mean either height or width!
		        }
     		}
		}
			
	  });
   };
			
})(jQuery); 

