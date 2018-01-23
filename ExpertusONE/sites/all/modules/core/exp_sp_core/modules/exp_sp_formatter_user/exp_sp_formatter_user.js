$(document).ready(function(){
	try{
 	if($('.user-profile').size()){
		getUserProfile() ; 
 	}
	}catch(e){
		// to do
	}
});

this.getUserProfile=function(userListObj, refreshInterval){
//	var parObject=this;
//	var obj = userListObj;
	try{
	var obj = $("#expertus-online-users").data("lnrwhoisonline");
	//var userListObj = $("#expertus-online-users").data("lnrwhoisonline");
	$('.user-profile').each(function(){
		try{
		$(this).qtip({
			   content: {
			 	text: "<div id='whois-online-loader'></div>",
			 	title: {
				      text: '<div class="block-title-left"><div class="block-title-right"><div class="block-title-middle"><h2 class="block-title">User Details</h2></div></div></div>',
				      button: 'X'
				   }
		 		},
			   api:{
			 		onShow: function() {
			 			var selectedId = jQuery(this.elements['target']).attr('id');
			 			if (refreshInterval != null && refreshInterval > 0) {
			 				// If refreshInterval is not set or is <= 0, the user list in the invoking object does not have refresh logic
				 			if (userListObj.setIntervalId != null) {
				 				//alert('getUserProfile() : qtip : onShow() : clearInterval');
				 				clearInterval(userListObj.setIntervalId);
				 				userListObj.setIntervalId = null;
				 			}
			 			}
/*					 	var SPLearnerLicense = EXPERTUS.SMARTPORTAL.AbstractManager.getCookie("SPLearnerLicense")!== undefined?EXPERTUS.SMARTPORTAL.AbstractManager.getCookie("SPLearnerLicense"):'';
					 	var SPCertificate = EXPERTUS.SMARTPORTAL.AbstractManager.getCookie("SPCertificate")!== undefined?EXPERTUS.SMARTPORTAL.AbstractManager.getCookie("SPCertificate"):''; */
			 			var self = this;
			   			var showTip = false;
			   			if(document.getElementById('whois-online-loader')) {
			   				obj.createLoader('whois-online-loader');
			   			}
			   			var url = '?q=ajax/user/profile/'+selectedId;
			   			$.ajax({ 
				   	         type: "POST",
				   	         url: url,
				   	         data:  '',
				   	         success: function(data){
			   						if(document.getElementById('whois-online-loader')) {
			   							obj.destroyLoader('whois-online-loader');
			   						}
			   						$(".UserProfile_box").remove();
				   			 		self.updateContent(data, false);
		   	                     }
			   			});
			   			$('.qtip-contentWrapper').each(function(){
				        	 if($(this).hasClass("whoisonline-holder") == false) {
					   				$(this).addClass("whoisonline-holder");
					   		 }
			        	 });
			        	 $('.qtip-title').each(function(){
		        			var appObj = $(this).find(".block-title-middle");
		        			$(this).find(".qtip-button").appendTo(appObj);
		        			$(this).find(".qtip-button").attr("title","Close");
			        	});
			   			
			   			
			   			return showTip;
			         },
			         onHide: function() {
			        	 if (refreshInterval != null && refreshInterval > 0) {
			        		// If refreshInterval is not set or is <= 0, the user list in the invoking object does not have refresh logic
				        	//alert('getUserProfile() : qtip : onHide() : setInterval');
			        		 userListObj.setIntervalId = setInterval(userListObj.reloadUsersGrid, refreshInterval);
			        	 }
			         }
			      },
			 style: {
			    	  background: 'none',
				      width:370,
				      height:'auto',
				      padding:0,
				      border:'1px solid #333'
				    },
				    position: { adjust: { x: -330, y: 0 } },

			   show: { when: { event: 'click' }},
			   hide: { when: { event: 'unfocus'}}
	});
		}catch(e){
			// to do
		}	
	});
	}catch(e){
		// to do
	}
};

