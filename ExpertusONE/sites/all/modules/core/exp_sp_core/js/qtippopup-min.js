(function(a){a.fn.qtipPopup=function(o){try{var f=o.postype;if(f.indexOf("custom")!=-1){a(this).qtipCustomLeft(o);}else{var t=Drupal.settings.ajaxPageState.theme;var d=o.wid;var q=o.poslwid;var n=o.posrwid;var h=o.heg;var m=o.popupDispId;var s=o.onClsFn;var j=o.entityId;var p=o.disp;var g=o.linkid;var i=o.dispDown;var b;var k=o.top;var c=o.catalogVisibleId;var u=o.beforeShow;var v=o.afterShow;var l=o.afterPosition;setTimeout(function(){var L=a("#"+g).height();var M=a("#"+g).width();var G=a("#"+g).offset();var D=a("#"+g).position();var H=(((G.top-h)>=(h/2)||p=="ctool")&&i!="Y")?"bottom-qtip-tip-visible":"bottom-qtip-tip-up-visible";a(this).qtipPopupShow(m,H,d,h,j,s,o);var J=a("."+H).height();var K=a("."+H).width();var x=M/2;var A=D.top+(L+J-12);var I=a("."+H).position();if(k===undefined||k===null){var y=(((G.top-h)>=(h/2)||p=="ctool")&&i!="Y")?(D.top-L):(D.top+L);}else{var y=k;}var F=D.left+(parseInt(x)-(K/2));a(this).tipPositioning(H,y,F,m);if(m.indexOf("qtip_owner_disp")===0){if(a("div[id^='qtip_owner_disp']").find("span[class='qtip-popup-visible']").length>0){a("div[id^='qtip_owner_disp']").find("span[class='qtip-popup-visible']").addClass("qtip-popup-visible_for_ie");
}}if(t=="expertusoneV2"){var w=!!(navigator.userAgent.match(/Trident/)&&!navigator.userAgent.match(/MSIE/))?true:false;if(c=="enrolled-all-exempted-disp"||c=="qtipAttachIdqtip_exempted_disp_all"||c=="qtipAttachIdqtip_exempted_single_disp"){b=a(this).qtipExemptedPosition(c);}else{if(a.browser.msie&&a.browser.version==10){b=8;}else{if(a.browser.msie&&a.browser.version==11){b=8;}else{if(a.browser.msie&&a.browser.version==9){b=8;}else{if(a.browser.msie&&a.browser.version==8){b=8;}else{if(navigator.userAgent.indexOf("Chrome")>0){b=8;}else{b=8;}}}}}}}var C=(D.top==0)?D.top:D.top-L;if(g.indexOf("widget-share-")==0){C=(parseInt(D.top)==0)?D.top:D.top-L;}var B=(((G.top-h)>=(h/2)||p=="ctool")&&i!="Y")?(C+J-b):A;var N=(((G.top-h)>=(h/2)||p=="ctool")&&i!="Y")?"bottom":"top";if(f=="middle"){try{a(this).qtipMiddle(B,d,I,q,n,x,K,D,m,j,N);}catch(E){alert(E);}}else{if(f=="bottomleft"||f=="topleft"){try{a(this).qtipLeft(B,d,I,q,x,K,D,m,j,N);}catch(E){alert(E);}}else{if(f=="bottomright"||f=="topright"){try{a(this).qtipRight(B,d,I,q,x,K,D,m,j,N);
}catch(E){alert(E);}}}}if(o.afterPosition!==undefined){o.afterPosition.call();}},10);}}catch(r){}};a.fn.qtipPopupShow=function(k,i,f,g,m,j,c){try{var b='<div class="'+i+'"></div> <table cellspacing="0" cellpadding="0" style="z-index:100;" height="'+g+'px" width="'+f+'px" id="bubble-face-table"> <tbody><tr><td class="bubble-tl"></td><td class="bubble-t"></td><td class="bubble-tr"><a id="admin-bubble-close" class="qtip-close-button-visible" onclick="closeQtip(\''+k+"','"+m+"',"+j+');"></a></td></tr><tr><td class="bubble-cl"></td><td valign="top" class="bubble-c">';var l='</td><td class="bubble-cr"></td></tr><tr><td class="bubble-bl"></td><td class="bubble-b-visible"></td><td class="bubble-br"></td></tr></tbody></table>';var d=b+"<div id='paintContentVisiblePopup'><div id='show_expertus_message'></div><div id='paintContent"+k+"'></div></div>"+l;a("#"+k+" #visible-popup-"+m).html(d);if(c.beforeShow!==undefined){c.beforeShow.call();}a("#"+k+" #visible-popup-"+m).show();if(c.afterShow!==undefined){c.afterShow.call();
}}catch(h){alert(h);}};a.fn.tipPositioning=function(d,b,g,c){try{a("#"+c+" ."+d).css("position","absolute").css("top",b+"px").css("left",g+"px");}catch(f){alert(f);}};a.fn.qtipMiddle=function(i,h,n,f,p,g,o,j,r,t,q){try{var l=0;var d=(h-40)/3;if(f==""&&p==""){var c=d/3;var s=c/2;l=d+parseInt(c)+parseInt(s)-(g-(o/2));}else{if(f!=""){l=d+parseInt(f);}else{l=d-p;}}var b=n.left-l+j.left;if(q=="bottom"){a("#"+r+" #visible-popup-"+t+" #bubble-face-table").css("position","absolute").css("bottom",i+"px").css("left",b+"px");}else{a("#"+r+" #visible-popup-"+t+" #bubble-face-table").css("position","absolute").css("top",i+"px").css("left",b+"px");}}catch(k){alert(k);}};a.fn.qtipLeft=function(i,h,n,f,g,o,j,q,s,p){try{var l=0;var d=(h-40)/3;if(f==""){var c=d/3;var r=c/2;l=(c)+(r)-(g-(o/2));}else{tipleft1=f;if(tipleft1>0){l=tipleft1;}}var b=n.left-l+j.left;if(p=="bottom"){a("#"+q+" #visible-popup-"+s+" #bubble-face-table").css("position","absolute").css("bottom",i+"px").css("left",b+"px");}else{a("#"+q+" #visible-popup-"+s+" #bubble-face-table").css("position","absolute").css("top",i+"px").css("left",b+"px");
}}catch(k){alert(k);}};a.fn.qtipRight=function(v,h,u,q,f,o,i,k,j,p){try{var l=0;var b=(h-40)/3;if(q==""){var n=b/3;var c=n/2;l=(b*2)+parseInt(n)+parseInt(c)-(f-(o/2));}else{var t=(b*2);var g=h-(f+(o/2)+10);tipleft1=t+parseInt(q);if(tipleft1>g){l=(f+(o/2))+t;}else{l=tipleft1;}}var r=u.left-l-10+i.left;if(p=="bottom"){a("#"+k+" #visible-popup-"+j+" #bubble-face-table").css("position","absolute").css("bottom",v+"px").css("left",r+"px");}else{a("#"+k+" #visible-popup-"+j+" #bubble-face-table").css("position","absolute").css("top",v+"px").css("left",r+"px");}}catch(s){alert(s);}};a.fn.qtipExemptedPosition=function(b){try{var d=!!(navigator.userAgent.match(/Trident/)&&!navigator.userAgent.match(/MSIE/))?true:false;if(b=="enrolled-all-exempted-disp"){if(a.browser.msie&&a.browser.version==10){posmin=32;}else{if(d){posmin=32;}else{if(a.browser.msie&&a.browser.version==9){posmin=12;}else{if(navigator.userAgent.indexOf("Chrome")>0){posmin=32;}else{posmin=8;}}}}}else{if(b=="qtipAttachIdqtip_exempted_single_disp"){if(a.browser.msie&&a.browser.version==10){posmin=18;
}else{if(d){posmin=18;}else{if(a.browser.msie&&a.browser.version==9){posmin=18;}else{posmin=15;}}}}else{if(b=="qtipAttachIdqtip_exempted_disp_all"){if(a.browser.msie&&a.browser.version==10){posmin=21;}else{if(d){posmin=21;}else{if(a.browser.msie&&a.browser.version==9){posmin=8;}else{posmin=21;}}}}}}return posmin;}catch(c){alert(c);}};a.fn.qtipCustomLeft=function(c){try{var g=Drupal.settings.ajaxPageState.theme;var i=c.wid;var f=c.poslwid;var m=c.posrwid;var j=c.heg;var q=c.popupDispId;var o=c.onClsFn;var r=c.entityId;var n=c.disp;var l=c.linkid;var p=c.dispDown;var b;var d=c.catalogVisibleId;var h=c.possubtype;setTimeout(function(){h=(h==null||h==undefined)?"middleLeft":h;var v=a("#"+l).position();var A=a("#"+l).height();var B=a("#"+l).width();var w=a("#"+l).offset();var x="qtip-tip-point-right";a(this).qtipPopupShow(q,x,i,j,r,o,c);var y=a("."+x).height();var z=a("."+x).width();var C=parseInt(y/2)-20;var t=parseInt(z/2)-z;a("."+x).css("top",C+"px").css("left",t+"px");var e=a("."+x).position();
var s=e.left-i+20;var u=(v.top==0)?v.top:y+(y/2)-j;if(h=="middleLeft"){u=parseInt(j/2)-(y/2)+10;s=e.left-i+20;}else{if(h=="bottomleft"||h=="topleft"){}else{if(h=="bottomright"||h=="topright"){}}}a("#"+q+" #visible-popup-"+r+" #bubble-face-table").css("position","absolute").css("top","-"+u+"px").css("left",s+"px");},10);}catch(k){alert(k);}};})(jQuery);