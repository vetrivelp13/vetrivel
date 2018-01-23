(function($) {	
	jQuery.fn.expertusClueTip = function (aDefn){	
			var defn = jQuery.extend({					
					'type':'',
					'subtype':'',
					'activation':'blur',
					'contentHeader':SMARTPORTAL.t('Similar values exist. Few are listed below.'),
					'itemInfo':[],
					'excludeItem':{}
			}, aDefn);   	
		return this.each(function(){
			$.fn.expertusClueTipRender(this.id,aDefn);
		});
   };
   
   jQuery.fn.expertusClueTipRender = function (objId,tipDefn){
	   tipDefn.activation=(tipDefn.activation==undefined)?'blur':tipDefn.activation;
	   tipDefn.contentHeader=(tipDefn.contentHeader==undefined)?SMARTPORTAL.t('Similar values exist. Few are listed below.'):tipDefn.contentHeader;
	   if(tipDefn.type == undefined){
		   return;
	   }
	   var contentHtml='<div>'+tipDefn.contentHeader+'</div>';
//	   contentHtml += '<div style="border-bottom: 1px solid #77BB11;clear:both;margin:5px 0;">&nbsp;</div>';
//	   alert(objId);
	   
	   $('#'+objId).qtip({			
			api:{	  			
				beforeShow:function(){
		   			var self = this;
		   			contentHtml='';
		   			var showTip=false;
		   			var url = resource.service_url + '?actionkey=Autocomplete' 
		   				+ '&type='+ tipDefn.type 
		   				+ (tipDefn.subtype == undefined?'':'&subtype='+tipDefn.subtype)
		   				+ '&q=' + this.elements.target.val()
		   				+ '&isClueTip=true';
		   			
		   			//make the ajax call to get the list of values
				   	jQuery.ajax({
				   		type: "GET",
					   	url:url,
					   	dataType:'text',
					   	contentType:'text/xml',
					   	async: false,
					   	success: function(respText){
				   			var responseXml;
				   			try	{
						  	  xmlDoc=new ActiveXObject("Microsoft.XMLDOM");
						  	  xmlDoc.async="true";
						  	  xmlDoc.loadXML(respText);
						  	  responseXml = xmlDoc;						  	  
						  	} catch(e) {	
								try{
									parser=new DOMParser();
									responseXml = parser.parseFromString(respText,"text/xml");									
								}catch(e){
									showTip=false;
								}
						  	}
						  	if(responseXml != undefined){						  		
						  		if($("Items>Item", responseXml).size()==0){
						  			showTip=false;
						  		} else {
						  			contentHtml='<div>';
						  			contentHtml+='<div style="float:left;width:95%;font-weight:bold;">'+tipDefn.contentHeader+'</div>';
						  			
						  			contentHtml+='<div style="text-align:center;float:right;width:5%"><img title="Close" alt="Close" style="width: 14px; height: 13px; cursor: pointer;" src="'+themepath+'/images/close.jpg" onclick="$(\'#'+objId+'\').qtip(\'hide\');" id="cluetip-close"></div>';
						  			contentHtml+='<div style="clear:both;"></div>';
						  			
						  			contentHtml += '<div style="border-bottom: 1px solid #77BB11;clear:both;margin:5px 0;"></div>';
						  			contentHtml+='<ul style="font-size:.9em;">';
						  			var exclude=tipDefn.excludeItem;
						  			var itemInfo=tipDefn.itemInfo;
						  			var count=0;
						  			$("Items>Item", responseXml).each(function(){
						  				if(exclude != undefined){
						  					var excludeKey=exclude.key;
						  					var excludeVal=exclude.value;						  					
						  					if($(excludeKey,this).text() != excludeVal){
						  						contentHtml+='<li style="padding:2px 0;">';								  				
								  				if(itemInfo!=undefined){
								  					for(var i=0;i<itemInfo.length;i++){
								  						var show=itemInfo[i].show;
								  						var key=itemInfo[i].key;						  						
								  						contentHtml+=show+' '+$(key,this).text();
								  						contentHtml+=', ';
								  					}
								  				}						  				
								  				contentHtml+='</li>';
								  				showTip=true;
						  					} else {
						  						showTip=showTip?true:false;
						  					}
						  				} else {
						  					contentHtml+='<li>';							  				
							  				if(itemInfo!=undefined){
							  					for(var i=0;i<itemInfo.length;i++){
							  						var show=itemInfo[i].show;
							  						var key=itemInfo[i].key;						  						
							  						contentHtml+=show+' '+$(key,this).text();
							  						contentHtml+=', ';
							  					}
							  				}						  				
							  				contentHtml+='</li>';
							  				showTip=true;
						  				}
						  				if(count==5)
						  					return false;
						  				count++;
						  				
							  		});
						  			contentHtml+='</ul>';
						  			contentHtml+='<div style="border-bottom: 1px solid #77BB11;clear:both;margin:9px 0 5px 0;"></div>';
						  			contentHtml+='</div>';
						  			if(!showTip) 
						  				contentHtml='';
						  			if(showTip)
						  				self.updateContent(contentHtml,false);						  				
						  		}
						  	}						  	
				   		}				   	
				   });
				   return showTip;
	   			}	   			
			},			
			show:tipDefn.activation,
			hide: { 
				when:{
					event:'unfocus'
				},
				effect:'slide'								
			},
			position: {
			      corner: {
			         target: 'rightTop',
			         tooltip: 'leftTop'
			      }
			   },
			style:{
				   width:275,   
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

				/*background:'#F2F2F2',				
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
				}*/
			}
		});
	   
   };
	
			
})(jQuery); 

