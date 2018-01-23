/*! Copyright (c) 2013 Brandon Aaron (http://brandon.aaron.sh)
 * Licensed under the MIT License (LICENSE.txt).
 *
 * Version: 3.1.9
 *
 * Requires: jQuery 1.2.2+
 */
(function(a){if(typeof define==="function"&&define.amd){define(["jquery"],a);}else{if(typeof exports==="object"){module.exports=a;}else{a(jQuery);}}}(function(c){var d=["wheel","mousewheel","DOMMouseScroll","MozMousePixelScroll"],k=("onwheel" in document||document.documentMode>=9)?["wheel"]:["mousewheel","DomMouseScroll","MozMousePixelScroll"],h=Array.prototype.slice,j,b;if(c.event.fixHooks){for(var e=d.length;e;){c.event.fixHooks[d[--e]]=c.event.mouseHooks;}}var f=c.event.special.mousewheel={version:"3.1.9",setup:function(){if(this.addEventListener){for(var m=k.length;m;){this.addEventListener(k[--m],l,false);}}else{this.onmousewheel=l;}c.data(this,"mousewheel-line-height",f.getLineHeight(this));c.data(this,"mousewheel-page-height",f.getPageHeight(this));},teardown:function(){if(this.removeEventListener){for(var m=k.length;
m;){this.removeEventListener(k[--m],l,false);}}else{this.onmousewheel=null;}},getLineHeight:function(i){return parseInt(c(i)["offsetParent" in c.fn?"offsetParent":"parent"]().css("fontSize"),10);},getPageHeight:function(i){return c(i).height();},settings:{adjustOldDeltas:true}};c.fn.extend({mousewheel:function(i){return i?this.bind("mousewheel",i):this.trigger("mousewheel");},unmousewheel:function(i){return this.unbind("mousewheel",i);}});function l(i){var n=i||window.event,r=h.call(arguments,1),t=0,p=0,o=0,q=0;i=c.event.fix(n);i.type="mousewheel";if("detail" in n){o=n.detail*-1;}if("wheelDelta" in n){o=n.wheelDelta;}if("wheelDeltaY" in n){o=n.wheelDeltaY;}if("wheelDeltaX" in n){p=n.wheelDeltaX*-1;}if("axis" in n&&n.axis===n.HORIZONTAL_AXIS){p=o*-1;o=0;}t=o===0?p:o;if("deltaY" in n){o=n.deltaY*-1;t=o;}if("deltaX" in n){p=n.deltaX;if(o===0){t=p*-1;}}if(o===0&&p===0){return;}if(n.deltaMode===1){var s=c.data(this,"mousewheel-line-height");t*=s;o*=s;p*=s;}else{if(n.deltaMode===2){var m=c.data(this,"mousewheel-page-height");
t*=m;o*=m;p*=m;}}q=Math.max(Math.abs(o),Math.abs(p));if(!b||q<b){b=q;if(a(n,q)){b/=40;}}if(a(n,q)){t/=40;p/=40;o/=40;}t=Math[t>=1?"floor":"ceil"](t/b);p=Math[p>=1?"floor":"ceil"](p/b);o=Math[o>=1?"floor":"ceil"](o/b);i.deltaX=p;i.deltaY=o;i.deltaFactor=b;i.deltaMode=0;r.unshift(i,t,p,o);if(j){clearTimeout(j);}j=setTimeout(g,200);return(c.event.dispatch||c.event.handle).apply(this,r);}function g(){b=null;}function a(m,i){return f.settings.adjustOldDeltas&&m.type==="mousewheel"&&i%120===0;}}));