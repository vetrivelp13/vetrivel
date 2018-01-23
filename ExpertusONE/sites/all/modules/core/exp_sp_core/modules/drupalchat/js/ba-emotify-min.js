/*!
 * JavaScript Emotify - v0.6 - 11/17/2009
 * http://benalman.com/projects/javascript-emotify/
 * 
 * Copyright (c) 2009 "Cowboy" Ben Alman
 * Dual licensed under the MIT and GPL licenses.
 * http://benalman.com/about/license/
 */
;window.emotify=(function(){var c,a,b={},d=[];c=function(e,f){f=f||function(g,j,h,i){j=(j+", "+h).replace(/"/g,"&quot;").replace(/</g,"&lt;");return'<img src="'+g+'" title="'+j+'" alt="" class="smiley"/>';};return e.replace(a,function(h,g,m){var j=0,k=m,l=b[m];if(!l){while(j<d.length&&!d[j].regexp.test(m)){j++;}k=d[j].name;l=b[k];}return l?(g+f(l[0],l[1],k,m)):h;});};c.emoticons=function(){var l=Array.prototype.slice.call(arguments),o=typeof l[0]==="string"?l.shift():"",f=typeof l[0]==="boolean"?l.shift():false,g=l[0],k,j=[],n,h,m;if(g){if(f){b={};d=[];}for(k in g){b[k]=g[k];b[k][0]=o+b[k][0];}for(k in b){if(b[k].length>2){n=b[k].slice(2).concat(k);h=n.length;while(h--){if(typeof(n[h])!="undefined"){n[h]=n[h].replace(/(\W)/g,"\\$1");}}m=n.join("|");d.push({name:k,regexp:new RegExp("^"+m+"$")});
}else{m=k.replace(/(\W)/g,"\\$1");}j.push(m);}a=new RegExp("(^|\\s)("+j.join("|")+")(?=(?:$|\\s))","g");}return b;};return c;})();