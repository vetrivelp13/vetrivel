(function($){
	$.fn.qtipPopup = function(qtipObj){ // convert to json
		try{
			var ptype = qtipObj.postype;
			if(ptype.indexOf('custom') != -1)
				$(this).qtipCustomLeft(qtipObj);
			else{
				var theme = Drupal.settings.ajaxPageState.theme;
				var qwid = qtipObj.wid;	
				var mlwid = qtipObj.poslwid;
				var mrwid = qtipObj.posrwid;
				var qheg = qtipObj.heg;
				var popupDispId = qtipObj.popupDispId;
				var onClsFn = qtipObj.onClsFn;
				var entId = qtipObj.entityId;
				var disp = qtipObj.disp;
				var lId = qtipObj.linkid;
				var dispDown= qtipObj.dispDown;
				var posmin;
			var setTop=qtipObj.top;
				var catalogVisibleId = qtipObj.catalogVisibleId;	
			var beforeShow = qtipObj.beforeShow;
			var afterShow = qtipObj.afterShow;
			var afterPosition = qtipObj.afterPosition;
			
				setTimeout(function(){
					var lheg = $('#'+lId).height();
					var lwid = $('#'+lId).width();
					
					var popoff = $('#'+lId).offset();
					var lpos = $('#'+lId).position();
				var tcls = (((popoff.top - qheg) >= (qheg/2) || disp=='ctool') && dispDown !='Y') ? 'bottom-qtip-tip-visible' : 'bottom-qtip-tip-up-visible';
					
				$(this).qtipPopupShow(popupDispId,tcls,qwid,qheg,entId,onClsFn,qtipObj); // pop up
					
					var theg = $('.' + tcls).height();
					var twid = $('.' + tcls).width();
					var llwid = lwid / 2;
					var tpos = lpos.top + (lheg + theg - 12);
					var orgtpos = $('.'+tcls).position();
				if (setTop === undefined || setTop === null) 
					var ttop = (((popoff.top-qheg) >= (qheg/2) || disp=='ctool') && dispDown !='Y') ? (lpos.top-lheg) : (lpos.top + lheg);
				else
					var ttop =setTop;
					var z = lpos.left + (parseInt(llwid) - (twid / 2));
					
					$(this).tipPositioning(tcls,ttop,z,popupDispId); 
					if(popupDispId.indexOf("qtip_owner_disp") === 0)
					{
						if($("div[id^='qtip_owner_disp']").find("span[class='qtip-popup-visible']").length > 0)
						{
							$("div[id^='qtip_owner_disp']").find("span[class='qtip-popup-visible']").addClass("qtip-popup-visible_for_ie");
						}
					}
					
					if(theme == 'expertusoneV2'){
						var isAtLeastIE11 = !!(navigator.userAgent.match(/Trident/) && !navigator.userAgent.match(/MSIE/))? true : false;
						if(catalogVisibleId =='enrolled-all-exempted-disp' || catalogVisibleId == 'qtipAttachIdqtip_exempted_disp_all' || catalogVisibleId == 'qtipAttachIdqtip_exempted_single_disp'){
							posmin = $(this).qtipExemptedPosition(catalogVisibleId);
						}else{
							if($.browser.msie && $.browser.version == 10) 
								posmin = 8;
							else if($.browser.msie && $.browser.version == 11) 
								posmin = 8;
							else if($.browser.msie && $.browser.version == 9) 
								posmin = 8;
							else if($.browser.msie && $.browser.version == 8)
								posmin = 8;
							else if(navigator.userAgent.indexOf("Chrome")>0) 
								posmin = 8;
							else
								posmin = 8;
						}
					}
					//var posmin = (Drupal.settings.ajaxPageState.theme == 'expertusone') ? 13 : 8;
					
					var lps = (lpos.top == 0 ) ? lpos.top : lpos.top-lheg;
					
				    if(lId.indexOf("widget-share-")==0 )
					  lps = (parseInt(lpos.top) == 0 ) ? lpos.top : lpos.top-lheg; 
			       
			        var ppos = (((popoff.top-qheg) >= (qheg/2) || disp=='ctool') && dispDown !='Y') ? (lps+theg-posmin) : tpos;
					var post = (((popoff.top-qheg) >= (qheg/2) || disp=='ctool') && dispDown !='Y') ? 'bottom' : 'top';
					if(ptype == 'middle'){
						try{
							$(this).qtipMiddle(ppos,qwid,orgtpos,mlwid,mrwid,llwid,twid,lpos,popupDispId,entId,post); //Middle
							
						}catch(e){
							alert(e);
						}
					}else if(ptype == 'bottomleft' || ptype == 'topleft'){
						try{
							$(this).qtipLeft(ppos,qwid,orgtpos,mlwid,llwid,twid,lpos,popupDispId,entId,post); //left
						}catch(e){
							alert(e);
						}
					}else if(ptype == 'bottomright' || ptype == 'topright'){
						try{
							$(this).qtipRight(ppos,qwid,orgtpos,mlwid,llwid,twid,lpos,popupDispId,entId,post); //right
						}catch(e){
							alert(e);
						}
					}

					if(qtipObj.afterPosition !== undefined) {
						qtipObj.afterPosition.call();
					}
				},10)
			}
		}catch(e){
			//alert(e);
		}

	};
	
	$.fn.qtipPopupShow = function(popupDispId,tcls,qwid,qheg,entId,onClsFn,qtipObj){
		try{
			var bpTop = '<div class="'+tcls+'"></div> <table cellspacing="0" cellpadding="0" style="z-index:100;" height="'+qheg+'px" width="'+qwid+'px" id="bubble-face-table"> <tbody><tr><td class="bubble-tl"></td><td class="bubble-t"></td><td class="bubble-tr"><a id="admin-bubble-close" class="qtip-close-button-visible" onclick="closeQtip(\''+popupDispId+'\',\''+entId+'\','+onClsFn+');"></a></td></tr><tr><td class="bubble-cl"></td><td valign="top" class="bubble-c">';
			var bpBot = '</td><td class="bubble-cr"></td></tr><tr><td class="bubble-bl"></td><td class="bubble-b-visible"></td><td class="bubble-br"></td></tr></tbody></table>';
			var paintHtml = bpTop+"<div id='paintContentVisiblePopup'><div id='show_expertus_message'></div><div id='paintContent"+popupDispId+"'></div></div>"+bpBot;
			$('#'+popupDispId+' #visible-popup-'+entId).html(paintHtml);
			if(qtipObj.beforeShow !== undefined) {
				qtipObj.beforeShow.call();
			}
			$('#'+popupDispId+' #visible-popup-'+entId).show();
			if(qtipObj.afterShow !== undefined) {
				qtipObj.afterShow.call();
			}
		}catch(e){
			alert(e);
		}
	};
	
	$.fn.tipPositioning = function(tcls,ttop,z,popupDispId){
		try{
			$('#'+popupDispId+' .' + tcls).css('position', 'absolute').css('top', ttop+'px').css('left', z+'px');
		}catch(e){
			alert(e);
		}
	};
	
	$.fn.qtipMiddle = function(ppos,qwid,orgtpos,mlwid,mrwid,llwid,twid,lpos,popupDispId,entId,post){
		try{
			var tipleft = 0;
			var qpos = (qwid-40)/3;
			if(mlwid == '' && mrwid == ''){
				var m = qpos/3;
				var qm = m/2;
				tipleft = qpos + parseInt(m) + parseInt(qm) - (llwid - (twid / 2));
			}else{
				if(mlwid !=''){
					tipleft = qpos + parseInt(mlwid);
				}else{
					tipleft = qpos - mrwid;
				}
			}
			var tleft = orgtpos.left - tipleft + lpos.left;
			if(post == 'bottom')
				$('#'+popupDispId+' #visible-popup-'+entId+' #bubble-face-table').css('position','absolute').css('bottom',ppos+'px').css('left',tleft+'px');
			else
				$('#'+popupDispId+' #visible-popup-'+entId+' #bubble-face-table').css('position','absolute').css('top',ppos+'px').css('left',tleft+'px');
			
		}catch(e){
			alert(e);
		}
		
	};
	
	$.fn.qtipLeft = function(ppos,qwid,orgtpos,mlwid,llwid,twid,lpos,popupDispId,entId,post){
		try{
		//alert(ppos);
			var tipleft = 0;
			var qpos = (qwid-40)/3;
			if(mlwid == ''){
				var m = qpos/3;
				var qm = m/2;
				tipleft =  (m) + (qm) - (llwid - (twid / 2));
			}else{
				tipleft1 = mlwid;
				if(tipleft1 > 0){
					tipleft = tipleft1;//(llwid + (twid / 2)) - mwid;
				}
			}
			
			var tleft = orgtpos.left - tipleft + lpos.left;
			if(post == 'bottom')
				$('#'+popupDispId+' #visible-popup-'+entId+' #bubble-face-table').css('position','absolute').css('bottom',ppos+'px').css('left',tleft+'px');
			else
				$('#'+popupDispId+' #visible-popup-'+entId+' #bubble-face-table').css('position','absolute').css('top',ppos+'px').css('left',tleft+'px');
		}catch(e){
			alert(e);
		}
	};
	
	$.fn.qtipRight = function(ppos,qwid,orgtpos,mlwid,llwid,twid,lpos,popupDispId,entId,post){
		try{
			var tipleft = 0;
			var qpos = (qwid-40)/3;
			if(mlwid == ''){
				var m = qpos/3;
				var qm = m/2;
				tipleft = (qpos*2) + parseInt(m) + parseInt(qm) - (llwid - (twid / 2));
			}else{
				var d = (qpos*2);
				var balpos = qwid - (llwid + (twid / 2) +10);
				tipleft1 = d + parseInt(mlwid);
				if(tipleft1 > balpos){
					tipleft = (llwid + (twid / 2)) + d;
				}else{
					tipleft = tipleft1;
				}
			}
			var tleft = orgtpos.left - tipleft -10 + lpos.left;
			if(post == 'bottom')
				$('#'+popupDispId+' #visible-popup-'+entId+' #bubble-face-table').css('position','absolute').css('bottom',ppos+'px').css('left',tleft+'px');
			else
				$('#'+popupDispId+' #visible-popup-'+entId+' #bubble-face-table').css('position','absolute').css('top',ppos+'px').css('left',tleft+'px');
		}catch(e){
			alert(e);
		}
	};
	
	

	$.fn.qtipExemptedPosition =  function(catalogVisibleId) {
		try {
			var isAtLeastIE11 = !!(navigator.userAgent.match(/Trident/) && !navigator.userAgent.match(/MSIE/))? true : false;
			if(catalogVisibleId=='enrolled-all-exempted-disp'){
				if($.browser.msie && $.browser.version == 10) 
					posmin = 32;
				else if(isAtLeastIE11) 
					posmin = 32;
				else if($.browser.msie && $.browser.version == 9) 
					posmin = 12;
				else if(navigator.userAgent.indexOf("Chrome")>0) 
					posmin = 32;
				else
					posmin = 8;	
			}else if(catalogVisibleId=='qtipAttachIdqtip_exempted_single_disp'){
				if($.browser.msie && $.browser.version == 10) {
					posmin = 18;
				}
				else if(isAtLeastIE11) 
					posmin = 18;
				else if($.browser.msie && $.browser.version == 9) 
					posmin = 18;
				else
					posmin = 15;
			}else if(catalogVisibleId == 'qtipAttachIdqtip_exempted_disp_all'){
				if($.browser.msie && $.browser.version == 10) 
					posmin = 21;
				else if(isAtLeastIE11) 
					posmin = 21;
				else if($.browser.msie && $.browser.version == 9) 
					posmin = 8;
				else
					posmin = 21;
			}
			return posmin;
		}catch(e){
			alert(e);
		}
	};
	$.fn.qtipCustomLeft = function(qtipObj){
		try{
			// Collect qtipObj values
			var theme = Drupal.settings.ajaxPageState.theme;
			var qwid = qtipObj.wid;	
			var mlwid = qtipObj.poslwid;
			var mrwid = qtipObj.posrwid;
			var qheg = qtipObj.heg;
			var popupDispId = qtipObj.popupDispId;
			var onClsFn = qtipObj.onClsFn;
			var entId = qtipObj.entityId;
			var disp = qtipObj.disp;
			var lId = qtipObj.linkid;
			var dispRight= qtipObj.dispDown;
			var posmin;
			var catalogVisibleId = qtipObj.catalogVisibleId;
			var psubtype = qtipObj.possubtype;
			setTimeout(function(){
				psubtype = (psubtype==null || psubtype == undefined )?'middleLeft':psubtype;
				// Get positions of the triggering link
				var lpos = $('#'+lId).position();
				var lheg = $('#'+lId).height();
				var lwid = $('#'+lId).width();
				var popoff = $('#'+lId).offset();
				
				//Class name of the qtip
				var tcls = 'qtip-tip-point-right';
				// Show qtip popup
				$(this).qtipPopupShow(popupDispId,tcls,qwid,qheg,entId,onClsFn,qtipObj);
				
				// Get qtip positions
				var theg = $('.' + tcls).height();
				var twid = $('.' + tcls).width();

				//Calculate tip Position
				var tipTop = parseInt(theg/2) - 20;  //since tip height is 40
				var tipRight = parseInt(twid/2) - twid;
				
				// Position the tip
				$('.' + tcls).css('top',tipTop+'px').css('left',tipRight+'px');
				
				//Calculate popup possisions
				var tlcpos = $('.'+tcls).position();
				var left = tlcpos.left-qwid+20;
				var ppos = (lpos.top == 0) ? lpos.top : theg+(theg/2)-qheg;
				
				if(psubtype == 'middleLeft'){
					ppos = parseInt(qheg/2)-(theg/2)+10;
					left = tlcpos.left-qwid+20;
				}else if(psubtype == 'bottomleft' || psubtype == 'topleft'){
					
				}else if(psubtype == 'bottomright' || psubtype == 'topright'){
					
				}
				$('#'+popupDispId+' #visible-popup-'+entId+' #bubble-face-table').css('position','absolute').css('top','-'+ppos+'px').css('left',left+'px');
			},10)
		}catch(e){
			alert(e);
		}
	};
	
})(jQuery);


