;/*! jQuery v1.9.1 | (c) 2005, 2012 jQuery Foundation, Inc. | jquery.org/license
*/(function(e,t){var n,r,i=typeof t,o=e.document,a=e.location,s=e.jQuery,u=e.$,l={},c=[],p="1.9.1",f=c.concat,d=c.push,h=c.slice,g=c.indexOf,m=l.toString,y=l.hasOwnProperty,v=p.trim,b=function(e,t){return new b.fn.init(e,t,r)},x=/[+-]?(?:\d*\.|)\d+(?:[eE][+-]?\d+|)/.source,w=/\S+/g,T=/^[\s\uFEFF\xA0]+|[\s\uFEFF\xA0]+$/g,N=/^(?:(<[\w\W]+>)[^>]*|#([\w-]*))$/,C=/^<(\w+)\s*\/?>(?:<\/\1>|)$/,k=/^[\],:{}\s]*$/,E=/(?:^|:|,)(?:\s*\[)+/g,S=/\\(?:["\\\/bfnrt]|u[\da-fA-F]{4})/g,A=/"[^"\\\r\n]*"|true|false|null|-?(?:\d+\.|)\d+(?:[eE][+-]?\d+|)/g,j=/^-ms-/,D=/-([\da-z])/gi,L=function(e,t){return t.toUpperCase()},H=function(e){(o.addEventListener||"load"===e.type||"complete"===o.readyState)&&(q(),b.ready())},q=function(){o.addEventListener?(o.removeEventListener("DOMContentLoaded",H,!1),e.removeEventListener("load",H,!1)):(o.detachEvent("onreadystatechange",H),e.detachEvent("onload",H))};b.fn=b.prototype={jquery:p,constructor:b,init:function(e,n,r){var i,a;if(!e)return this;if("string"==typeof e){if(i="<"===e.charAt(0)&&">"===e.charAt(e.length-1)&&e.length>=3?[null,e,null]:N.exec(e),!i||!i[1]&&n)return!n||n.jquery?(n||r).find(e):this.constructor(n).find(e);if(i[1]){if(n=n instanceof b?n[0]:n,b.merge(this,b.parseHTML(i[1],n&&n.nodeType?n.ownerDocument||n:o,!0)),C.test(i[1])&&b.isPlainObject(n))for(i in n)b.isFunction(this[i])?this[i](n[i]):this.attr(i,n[i]);return this}if(a=o.getElementById(i[2]),a&&a.parentNode){if(a.id!==i[2])return r.find(e);this.length=1,this[0]=a}return this.context=o,this.selector=e,this}return e.nodeType?(this.context=this[0]=e,this.length=1,this):b.isFunction(e)?r.ready(e):(e.selector!==t&&(this.selector=e.selector,this.context=e.context),b.makeArray(e,this))},selector:"",length:0,size:function(){return this.length},toArray:function(){return h.call(this)},get:function(e){return null==e?this.toArray():0>e?this[this.length+e]:this[e]},pushStack:function(e){var t=b.merge(this.constructor(),e);return t.prevObject=this,t.context=this.context,t},each:function(e,t){return b.each(this,e,t)},ready:function(e){return b.ready.promise().done(e),this},slice:function(){return this.pushStack(h.apply(this,arguments))},first:function(){return this.eq(0)},last:function(){return this.eq(-1)},eq:function(e){var t=this.length,n=+e+(0>e?t:0);return this.pushStack(n>=0&&t>n?[this[n]]:[])},map:function(e){return this.pushStack(b.map(this,function(t,n){return e.call(t,n,t)}))},end:function(){return this.prevObject||this.constructor(null)},push:d,sort:[].sort,splice:[].splice},b.fn.init.prototype=b.fn,b.extend=b.fn.extend=function(){var e,n,r,i,o,a,s=arguments[0]||{},u=1,l=arguments.length,c=!1;for("boolean"==typeof s&&(c=s,s=arguments[1]||{},u=2),"object"==typeof s||b.isFunction(s)||(s={}),l===u&&(s=this,--u);l>u;u++)if(null!=(o=arguments[u]))for(i in o)e=s[i],r=o[i],s!==r&&(c&&r&&(b.isPlainObject(r)||(n=b.isArray(r)))?(n?(n=!1,a=e&&b.isArray(e)?e:[]):a=e&&b.isPlainObject(e)?e:{},s[i]=b.extend(c,a,r)):r!==t&&(s[i]=r));return s},b.extend({noConflict:function(t){return e.$===b&&(e.$=u),t&&e.jQuery===b&&(e.jQuery=s),b},isReady:!1,readyWait:1,holdReady:function(e){e?b.readyWait++:b.ready(!0)},ready:function(e){if(e===!0?!--b.readyWait:!b.isReady){if(!o.body)return setTimeout(b.ready);b.isReady=!0,e!==!0&&--b.readyWait>0||(n.resolveWith(o,[b]),b.fn.trigger&&b(o).trigger("ready").off("ready"))}},isFunction:function(e){return"function"===b.type(e)},isArray:Array.isArray||function(e){return"array"===b.type(e)},isWindow:function(e){return null!=e&&e==e.window},isNumeric:function(e){return!isNaN(parseFloat(e))&&isFinite(e)},type:function(e){return null==e?e+"":"object"==typeof e||"function"==typeof e?l[m.call(e)]||"object":typeof e},isPlainObject:function(e){if(!e||"object"!==b.type(e)||e.nodeType||b.isWindow(e))return!1;try{if(e.constructor&&!y.call(e,"constructor")&&!y.call(e.constructor.prototype,"isPrototypeOf"))return!1}catch(n){return!1}var r;for(r in e);return r===t||y.call(e,r)},isEmptyObject:function(e){var t;for(t in e)return!1;return!0},error:function(e){throw Error(e)},parseHTML:function(e,t,n){if(!e||"string"!=typeof e)return null;"boolean"==typeof t&&(n=t,t=!1),t=t||o;var r=C.exec(e),i=!n&&[];return r?[t.createElement(r[1])]:(r=b.buildFragment([e],t,i),i&&b(i).remove(),b.merge([],r.childNodes))},parseJSON:function(n){return e.JSON&&e.JSON.parse?e.JSON.parse(n):null===n?n:"string"==typeof n&&(n=b.trim(n),n&&k.test(n.replace(S,"@").replace(A,"]").replace(E,"")))?Function("return "+n)():(b.error("Invalid JSON: "+n),t)},parseXML:function(n){var r,i;if(!n||"string"!=typeof n)return null;try{e.DOMParser?(i=new DOMParser,r=i.parseFromString(n,"text/xml")):(r=new ActiveXObject("Microsoft.XMLDOM"),r.async="false",r.loadXML(n))}catch(o){r=t}return r&&r.documentElement&&!r.getElementsByTagName("parsererror").length||b.error("Invalid XML: "+n),r},noop:function(){},globalEval:function(t){t&&b.trim(t)&&(e.execScript||function(t){e.eval.call(e,t)})(t)},camelCase:function(e){return e.replace(j,"ms-").replace(D,L)},nodeName:function(e,t){return e.nodeName&&e.nodeName.toLowerCase()===t.toLowerCase()},each:function(e,t,n){var r,i=0,o=e.length,a=M(e);if(n){if(a){for(;o>i;i++)if(r=t.apply(e[i],n),r===!1)break}else for(i in e)if(r=t.apply(e[i],n),r===!1)break}else if(a){for(;o>i;i++)if(r=t.call(e[i],i,e[i]),r===!1)break}else for(i in e)if(r=t.call(e[i],i,e[i]),r===!1)break;return e},trim:v&&!v.call("\ufeff\u00a0")?function(e){return null==e?"":v.call(e)}:function(e){return null==e?"":(e+"").replace(T,"")},makeArray:function(e,t){var n=t||[];return null!=e&&(M(Object(e))?b.merge(n,"string"==typeof e?[e]:e):d.call(n,e)),n},inArray:function(e,t,n){var r;if(t){if(g)return g.call(t,e,n);for(r=t.length,n=n?0>n?Math.max(0,r+n):n:0;r>n;n++)if(n in t&&t[n]===e)return n}return-1},merge:function(e,n){var r=n.length,i=e.length,o=0;if("number"==typeof r)for(;r>o;o++)e[i++]=n[o];else while(n[o]!==t)e[i++]=n[o++];return e.length=i,e},grep:function(e,t,n){var r,i=[],o=0,a=e.length;for(n=!!n;a>o;o++)r=!!t(e[o],o),n!==r&&i.push(e[o]);return i},map:function(e,t,n){var r,i=0,o=e.length,a=M(e),s=[];if(a)for(;o>i;i++)r=t(e[i],i,n),null!=r&&(s[s.length]=r);else for(i in e)r=t(e[i],i,n),null!=r&&(s[s.length]=r);return f.apply([],s)},guid:1,proxy:function(e,n){var r,i,o;return"string"==typeof n&&(o=e[n],n=e,e=o),b.isFunction(e)?(r=h.call(arguments,2),i=function(){return e.apply(n||this,r.concat(h.call(arguments)))},i.guid=e.guid=e.guid||b.guid++,i):t},access:function(e,n,r,i,o,a,s){var u=0,l=e.length,c=null==r;if("object"===b.type(r)){o=!0;for(u in r)b.access(e,n,u,r[u],!0,a,s)}else if(i!==t&&(o=!0,b.isFunction(i)||(s=!0),c&&(s?(n.call(e,i),n=null):(c=n,n=function(e,t,n){return c.call(b(e),n)})),n))for(;l>u;u++)n(e[u],r,s?i:i.call(e[u],u,n(e[u],r)));return o?e:c?n.call(e):l?n(e[0],r):a},now:function(){return(new Date).getTime()}}),b.ready.promise=function(t){if(!n)if(n=b.Deferred(),"complete"===o.readyState)setTimeout(b.ready);else if(o.addEventListener)o.addEventListener("DOMContentLoaded",H,!1),e.addEventListener("load",H,!1);else{o.attachEvent("onreadystatechange",H),e.attachEvent("onload",H);var r=!1;try{r=null==e.frameElement&&o.documentElement}catch(i){}r&&r.doScroll&&function a(){if(!b.isReady){try{r.doScroll("left")}catch(e){return setTimeout(a,50)}q(),b.ready()}}()}return n.promise(t)},b.each("Boolean Number String Function Array Date RegExp Object Error".split(" "),function(e,t){l["[object "+t+"]"]=t.toLowerCase()});function M(e){var t=e.length,n=b.type(e);return b.isWindow(e)?!1:1===e.nodeType&&t?!0:"array"===n||"function"!==n&&(0===t||"number"==typeof t&&t>0&&t-1 in e)}r=b(o);var _={};function F(e){var t=_[e]={};return b.each(e.match(w)||[],function(e,n){t[n]=!0}),t}b.Callbacks=function(e){e="string"==typeof e?_[e]||F(e):b.extend({},e);var n,r,i,o,a,s,u=[],l=!e.once&&[],c=function(t){for(r=e.memory&&t,i=!0,a=s||0,s=0,o=u.length,n=!0;u&&o>a;a++)if(u[a].apply(t[0],t[1])===!1&&e.stopOnFalse){r=!1;break}n=!1,u&&(l?l.length&&c(l.shift()):r?u=[]:p.disable())},p={add:function(){if(u){var t=u.length;(function i(t){b.each(t,function(t,n){var r=b.type(n);"function"===r?e.unique&&p.has(n)||u.push(n):n&&n.length&&"string"!==r&&i(n)})})(arguments),n?o=u.length:r&&(s=t,c(r))}return this},remove:function(){return u&&b.each(arguments,function(e,t){var r;while((r=b.inArray(t,u,r))>-1)u.splice(r,1),n&&(o>=r&&o--,a>=r&&a--)}),this},has:function(e){return e?b.inArray(e,u)>-1:!(!u||!u.length)},empty:function(){return u=[],this},disable:function(){return u=l=r=t,this},disabled:function(){return!u},lock:function(){return l=t,r||p.disable(),this},locked:function(){return!l},fireWith:function(e,t){return t=t||[],t=[e,t.slice?t.slice():t],!u||i&&!l||(n?l.push(t):c(t)),this},fire:function(){return p.fireWith(this,arguments),this},fired:function(){return!!i}};return p},b.extend({Deferred:function(e){var t=[["resolve","done",b.Callbacks("once memory"),"resolved"],["reject","fail",b.Callbacks("once memory"),"rejected"],["notify","progress",b.Callbacks("memory")]],n="pending",r={state:function(){return n},always:function(){return i.done(arguments).fail(arguments),this},then:function(){var e=arguments;return b.Deferred(function(n){b.each(t,function(t,o){var a=o[0],s=b.isFunction(e[t])&&e[t];i[o[1]](function(){var e=s&&s.apply(this,arguments);e&&b.isFunction(e.promise)?e.promise().done(n.resolve).fail(n.reject).progress(n.notify):n[a+"With"](this===r?n.promise():this,s?[e]:arguments)})}),e=null}).promise()},promise:function(e){return null!=e?b.extend(e,r):r}},i={};return r.pipe=r.then,b.each(t,function(e,o){var a=o[2],s=o[3];r[o[1]]=a.add,s&&a.add(function(){n=s},t[1^e][2].disable,t[2][2].lock),i[o[0]]=function(){return i[o[0]+"With"](this===i?r:this,arguments),this},i[o[0]+"With"]=a.fireWith}),r.promise(i),e&&e.call(i,i),i},when:function(e){var t=0,n=h.call(arguments),r=n.length,i=1!==r||e&&b.isFunction(e.promise)?r:0,o=1===i?e:b.Deferred(),a=function(e,t,n){return function(r){t[e]=this,n[e]=arguments.length>1?h.call(arguments):r,n===s?o.notifyWith(t,n):--i||o.resolveWith(t,n)}},s,u,l;if(r>1)for(s=Array(r),u=Array(r),l=Array(r);r>t;t++)n[t]&&b.isFunction(n[t].promise)?n[t].promise().done(a(t,l,n)).fail(o.reject).progress(a(t,u,s)):--i;return i||o.resolveWith(l,n),o.promise()}}),b.support=function(){var t,n,r,a,s,u,l,c,p,f,d=o.createElement("div");if(d.setAttribute("className","t"),d.innerHTML="  <link/><table></table><a href='/a'>a</a><input type='checkbox'/>",n=d.getElementsByTagName("*"),r=d.getElementsByTagName("a")[0],!n||!r||!n.length)return{};s=o.createElement("select"),l=s.appendChild(o.createElement("option")),a=d.getElementsByTagName("input")[0],r.style.cssText="top:1px;float:left;opacity:.5",t={getSetAttribute:"t"!==d.className,leadingWhitespace:3===d.firstChild.nodeType,tbody:!d.getElementsByTagName("tbody").length,htmlSerialize:!!d.getElementsByTagName("link").length,style:/top/.test(r.getAttribute("style")),hrefNormalized:"/a"===r.getAttribute("href"),opacity:/^0.5/.test(r.style.opacity),cssFloat:!!r.style.cssFloat,checkOn:!!a.value,optSelected:l.selected,enctype:!!o.createElement("form").enctype,html5Clone:"<:nav></:nav>"!==o.createElement("nav").cloneNode(!0).outerHTML,boxModel:"CSS1Compat"===o.compatMode,deleteExpando:!0,noCloneEvent:!0,inlineBlockNeedsLayout:!1,shrinkWrapBlocks:!1,reliableMarginRight:!0,boxSizingReliable:!0,pixelPosition:!1},a.checked=!0,t.noCloneChecked=a.cloneNode(!0).checked,s.disabled=!0,t.optDisabled=!l.disabled;try{delete d.test}catch(h){t.deleteExpando=!1}a=o.createElement("input"),a.setAttribute("value",""),t.input=""===a.getAttribute("value"),a.value="t",a.setAttribute("type","radio"),t.radioValue="t"===a.value,a.setAttribute("checked","t"),a.setAttribute("name","t"),u=o.createDocumentFragment(),u.appendChild(a),t.appendChecked=a.checked,t.checkClone=u.cloneNode(!0).cloneNode(!0).lastChild.checked,d.attachEvent&&(d.attachEvent("onclick",function(){t.noCloneEvent=!1}),d.cloneNode(!0).click());for(f in{submit:!0,change:!0,focusin:!0})d.setAttribute(c="on"+f,"t"),t[f+"Bubbles"]=c in e||d.attributes[c].expando===!1;return d.style.backgroundClip="content-box",d.cloneNode(!0).style.backgroundClip="",t.clearCloneStyle="content-box"===d.style.backgroundClip,b(function(){var n,r,a,s="padding:0;margin:0;border:0;display:block;box-sizing:content-box;-moz-box-sizing:content-box;-webkit-box-sizing:content-box;",u=o.getElementsByTagName("body")[0];u&&(n=o.createElement("div"),n.style.cssText="border:0;width:0;height:0;position:absolute;top:0;left:-9999px;margin-top:1px",u.appendChild(n).appendChild(d),d.innerHTML="<table><tr><td></td><td>t</td></tr></table>",a=d.getElementsByTagName("td"),a[0].style.cssText="padding:0;margin:0;border:0;display:none",p=0===a[0].offsetHeight,a[0].style.display="",a[1].style.display="none",t.reliableHiddenOffsets=p&&0===a[0].offsetHeight,d.innerHTML="",d.style.cssText="box-sizing:border-box;-moz-box-sizing:border-box;-webkit-box-sizing:border-box;padding:1px;border:1px;display:block;width:4px;margin-top:1%;position:absolute;top:1%;",t.boxSizing=4===d.offsetWidth,t.doesNotIncludeMarginInBodyOffset=1!==u.offsetTop,e.getComputedStyle&&(t.pixelPosition="1%"!==(e.getComputedStyle(d,null)||{}).top,t.boxSizingReliable="4px"===(e.getComputedStyle(d,null)||{width:"4px"}).width,r=d.appendChild(o.createElement("div")),r.style.cssText=d.style.cssText=s,r.style.marginRight=r.style.width="0",d.style.width="1px",t.reliableMarginRight=!parseFloat((e.getComputedStyle(r,null)||{}).marginRight)),typeof d.style.zoom!==i&&(d.innerHTML="",d.style.cssText=s+"width:1px;padding:1px;display:inline;zoom:1",t.inlineBlockNeedsLayout=3===d.offsetWidth,d.style.display="block",d.innerHTML="<div></div>",d.firstChild.style.width="5px",t.shrinkWrapBlocks=3!==d.offsetWidth,t.inlineBlockNeedsLayout&&(u.style.zoom=1)),u.removeChild(n),n=d=a=r=null)}),n=s=u=l=r=a=null,t}();var O=/(?:\{[\s\S]*\}|\[[\s\S]*\])$/,B=/([A-Z])/g;function P(e,n,r,i){if(b.acceptData(e)){var o,a,s=b.expando,u="string"==typeof n,l=e.nodeType,p=l?b.cache:e,f=l?e[s]:e[s]&&s;if(f&&p[f]&&(i||p[f].data)||!u||r!==t)return f||(l?e[s]=f=c.pop()||b.guid++:f=s),p[f]||(p[f]={},l||(p[f].toJSON=b.noop)),("object"==typeof n||"function"==typeof n)&&(i?p[f]=b.extend(p[f],n):p[f].data=b.extend(p[f].data,n)),o=p[f],i||(o.data||(o.data={}),o=o.data),r!==t&&(o[b.camelCase(n)]=r),u?(a=o[n],null==a&&(a=o[b.camelCase(n)])):a=o,a}}function R(e,t,n){if(b.acceptData(e)){var r,i,o,a=e.nodeType,s=a?b.cache:e,u=a?e[b.expando]:b.expando;if(s[u]){if(t&&(o=n?s[u]:s[u].data)){b.isArray(t)?t=t.concat(b.map(t,b.camelCase)):t in o?t=[t]:(t=b.camelCase(t),t=t in o?[t]:t.split(" "));for(r=0,i=t.length;i>r;r++)delete o[t[r]];if(!(n?$:b.isEmptyObject)(o))return}(n||(delete s[u].data,$(s[u])))&&(a?b.cleanData([e],!0):b.support.deleteExpando||s!=s.window?delete s[u]:s[u]=null)}}}b.extend({cache:{},expando:"jQuery"+(p+Math.random()).replace(/\D/g,""),noData:{embed:!0,object:"clsid:D27CDB6E-AE6D-11cf-96B8-444553540000",applet:!0},hasData:function(e){return e=e.nodeType?b.cache[e[b.expando]]:e[b.expando],!!e&&!$(e)},data:function(e,t,n){return P(e,t,n)},removeData:function(e,t){return R(e,t)},_data:function(e,t,n){return P(e,t,n,!0)},_removeData:function(e,t){return R(e,t,!0)},acceptData:function(e){if(e.nodeType&&1!==e.nodeType&&9!==e.nodeType)return!1;var t=e.nodeName&&b.noData[e.nodeName.toLowerCase()];return!t||t!==!0&&e.getAttribute("classid")===t}}),b.fn.extend({data:function(e,n){var r,i,o=this[0],a=0,s=null;if(e===t){if(this.length&&(s=b.data(o),1===o.nodeType&&!b._data(o,"parsedAttrs"))){for(r=o.attributes;r.length>a;a++)i=r[a].name,i.indexOf("data-")||(i=b.camelCase(i.slice(5)),W(o,i,s[i]));b._data(o,"parsedAttrs",!0)}return s}return"object"==typeof e?this.each(function(){b.data(this,e)}):b.access(this,function(n){return n===t?o?W(o,e,b.data(o,e)):null:(this.each(function(){b.data(this,e,n)}),t)},null,n,arguments.length>1,null,!0)},removeData:function(e){return this.each(function(){b.removeData(this,e)})}});function W(e,n,r){if(r===t&&1===e.nodeType){var i="data-"+n.replace(B,"-$1").toLowerCase();if(r=e.getAttribute(i),"string"==typeof r){try{r="true"===r?!0:"false"===r?!1:"null"===r?null:+r+""===r?+r:O.test(r)?b.parseJSON(r):r}catch(o){}b.data(e,n,r)}else r=t}return r}function $(e){var t;for(t in e)if(("data"!==t||!b.isEmptyObject(e[t]))&&"toJSON"!==t)return!1;return!0}b.extend({queue:function(e,n,r){var i;return e?(n=(n||"fx")+"queue",i=b._data(e,n),r&&(!i||b.isArray(r)?i=b._data(e,n,b.makeArray(r)):i.push(r)),i||[]):t},dequeue:function(e,t){t=t||"fx";var n=b.queue(e,t),r=n.length,i=n.shift(),o=b._queueHooks(e,t),a=function(){b.dequeue(e,t)};"inprogress"===i&&(i=n.shift(),r--),o.cur=i,i&&("fx"===t&&n.unshift("inprogress"),delete o.stop,i.call(e,a,o)),!r&&o&&o.empty.fire()},_queueHooks:function(e,t){var n=t+"queueHooks";return b._data(e,n)||b._data(e,n,{empty:b.Callbacks("once memory").add(function(){b._removeData(e,t+"queue"),b._removeData(e,n)})})}}),b.fn.extend({queue:function(e,n){var r=2;return"string"!=typeof e&&(n=e,e="fx",r--),r>arguments.length?b.queue(this[0],e):n===t?this:this.each(function(){var t=b.queue(this,e,n);b._queueHooks(this,e),"fx"===e&&"inprogress"!==t[0]&&b.dequeue(this,e)})},dequeue:function(e){return this.each(function(){b.dequeue(this,e)})},delay:function(e,t){return e=b.fx?b.fx.speeds[e]||e:e,t=t||"fx",this.queue(t,function(t,n){var r=setTimeout(t,e);n.stop=function(){clearTimeout(r)}})},clearQueue:function(e){return this.queue(e||"fx",[])},promise:function(e,n){var r,i=1,o=b.Deferred(),a=this,s=this.length,u=function(){--i||o.resolveWith(a,[a])};"string"!=typeof e&&(n=e,e=t),e=e||"fx";while(s--)r=b._data(a[s],e+"queueHooks"),r&&r.empty&&(i++,r.empty.add(u));return u(),o.promise(n)}});var I,z,X=/[\t\r\n]/g,U=/\r/g,V=/^(?:input|select|textarea|button|object)$/i,Y=/^(?:a|area)$/i,J=/^(?:checked|selected|autofocus|autoplay|async|controls|defer|disabled|hidden|loop|multiple|open|readonly|required|scoped)$/i,G=/^(?:checked|selected)$/i,Q=b.support.getSetAttribute,K=b.support.input;b.fn.extend({attr:function(e,t){return b.access(this,b.attr,e,t,arguments.length>1)},removeAttr:function(e){return this.each(function(){b.removeAttr(this,e)})},prop:function(e,t){return b.access(this,b.prop,e,t,arguments.length>1)},removeProp:function(e){return e=b.propFix[e]||e,this.each(function(){try{this[e]=t,delete this[e]}catch(n){}})},addClass:function(e){var t,n,r,i,o,a=0,s=this.length,u="string"==typeof e&&e;if(b.isFunction(e))return this.each(function(t){b(this).addClass(e.call(this,t,this.className))});if(u)for(t=(e||"").match(w)||[];s>a;a++)if(n=this[a],r=1===n.nodeType&&(n.className?(" "+n.className+" ").replace(X," "):" ")){o=0;while(i=t[o++])0>r.indexOf(" "+i+" ")&&(r+=i+" ");n.className=b.trim(r)}return this},removeClass:function(e){var t,n,r,i,o,a=0,s=this.length,u=0===arguments.length||"string"==typeof e&&e;if(b.isFunction(e))return this.each(function(t){b(this).removeClass(e.call(this,t,this.className))});if(u)for(t=(e||"").match(w)||[];s>a;a++)if(n=this[a],r=1===n.nodeType&&(n.className?(" "+n.className+" ").replace(X," "):"")){o=0;while(i=t[o++])while(r.indexOf(" "+i+" ")>=0)r=r.replace(" "+i+" "," ");n.className=e?b.trim(r):""}return this},toggleClass:function(e,t){var n=typeof e,r="boolean"==typeof t;return b.isFunction(e)?this.each(function(n){b(this).toggleClass(e.call(this,n,this.className,t),t)}):this.each(function(){if("string"===n){var o,a=0,s=b(this),u=t,l=e.match(w)||[];while(o=l[a++])u=r?u:!s.hasClass(o),s[u?"addClass":"removeClass"](o)}else(n===i||"boolean"===n)&&(this.className&&b._data(this,"__className__",this.className),this.className=this.className||e===!1?"":b._data(this,"__className__")||"")})},hasClass:function(e){var t=" "+e+" ",n=0,r=this.length;for(;r>n;n++)if(1===this[n].nodeType&&(" "+this[n].className+" ").replace(X," ").indexOf(t)>=0)return!0;return!1},val:function(e){var n,r,i,o=this[0];{if(arguments.length)return i=b.isFunction(e),this.each(function(n){var o,a=b(this);1===this.nodeType&&(o=i?e.call(this,n,a.val()):e,null==o?o="":"number"==typeof o?o+="":b.isArray(o)&&(o=b.map(o,function(e){return null==e?"":e+""})),r=b.valHooks[this.type]||b.valHooks[this.nodeName.toLowerCase()],r&&"set"in r&&r.set(this,o,"value")!==t||(this.value=o))});if(o)return r=b.valHooks[o.type]||b.valHooks[o.nodeName.toLowerCase()],r&&"get"in r&&(n=r.get(o,"value"))!==t?n:(n=o.value,"string"==typeof n?n.replace(U,""):null==n?"":n)}}}),b.extend({valHooks:{option:{get:function(e){var t=e.attributes.value;return!t||t.specified?e.value:e.text}},select:{get:function(e){var t,n,r=e.options,i=e.selectedIndex,o="select-one"===e.type||0>i,a=o?null:[],s=o?i+1:r.length,u=0>i?s:o?i:0;for(;s>u;u++)if(n=r[u],!(!n.selected&&u!==i||(b.support.optDisabled?n.disabled:null!==n.getAttribute("disabled"))||n.parentNode.disabled&&b.nodeName(n.parentNode,"optgroup"))){if(t=b(n).val(),o)return t;a.push(t)}return a},set:function(e,t){var n=b.makeArray(t);return b(e).find("option").each(function(){this.selected=b.inArray(b(this).val(),n)>=0}),n.length||(e.selectedIndex=-1),n}}},attr:function(e,n,r){var o,a,s,u=e.nodeType;if(e&&3!==u&&8!==u&&2!==u)return typeof e.getAttribute===i?b.prop(e,n,r):(a=1!==u||!b.isXMLDoc(e),a&&(n=n.toLowerCase(),o=b.attrHooks[n]||(J.test(n)?z:I)),r===t?o&&a&&"get"in o&&null!==(s=o.get(e,n))?s:(typeof e.getAttribute!==i&&(s=e.getAttribute(n)),null==s?t:s):null!==r?o&&a&&"set"in o&&(s=o.set(e,r,n))!==t?s:(e.setAttribute(n,r+""),r):(b.removeAttr(e,n),t))},removeAttr:function(e,t){var n,r,i=0,o=t&&t.match(w);if(o&&1===e.nodeType)while(n=o[i++])r=b.propFix[n]||n,J.test(n)?!Q&&G.test(n)?e[b.camelCase("default-"+n)]=e[r]=!1:e[r]=!1:b.attr(e,n,""),e.removeAttribute(Q?n:r)},attrHooks:{type:{set:function(e,t){if(!b.support.radioValue&&"radio"===t&&b.nodeName(e,"input")){var n=e.value;return e.setAttribute("type",t),n&&(e.value=n),t}}}},propFix:{tabindex:"tabIndex",readonly:"readOnly","for":"htmlFor","class":"className",maxlength:"maxLength",cellspacing:"cellSpacing",cellpadding:"cellPadding",rowspan:"rowSpan",colspan:"colSpan",usemap:"useMap",frameborder:"frameBorder",contenteditable:"contentEditable"},prop:function(e,n,r){var i,o,a,s=e.nodeType;if(e&&3!==s&&8!==s&&2!==s)return a=1!==s||!b.isXMLDoc(e),a&&(n=b.propFix[n]||n,o=b.propHooks[n]),r!==t?o&&"set"in o&&(i=o.set(e,r,n))!==t?i:e[n]=r:o&&"get"in o&&null!==(i=o.get(e,n))?i:e[n]},propHooks:{tabIndex:{get:function(e){var n=e.getAttributeNode("tabindex");return n&&n.specified?parseInt(n.value,10):V.test(e.nodeName)||Y.test(e.nodeName)&&e.href?0:t}}}}),z={get:function(e,n){var r=b.prop(e,n),i="boolean"==typeof r&&e.getAttribute(n),o="boolean"==typeof r?K&&Q?null!=i:G.test(n)?e[b.camelCase("default-"+n)]:!!i:e.getAttributeNode(n);return o&&o.value!==!1?n.toLowerCase():t},set:function(e,t,n){return t===!1?b.removeAttr(e,n):K&&Q||!G.test(n)?e.setAttribute(!Q&&b.propFix[n]||n,n):e[b.camelCase("default-"+n)]=e[n]=!0,n}},K&&Q||(b.attrHooks.value={get:function(e,n){var r=e.getAttributeNode(n);return b.nodeName(e,"input")?e.defaultValue:r&&r.specified?r.value:t},set:function(e,n,r){return b.nodeName(e,"input")?(e.defaultValue=n,t):I&&I.set(e,n,r)}}),Q||(I=b.valHooks.button={get:function(e,n){var r=e.getAttributeNode(n);return r&&("id"===n||"name"===n||"coords"===n?""!==r.value:r.specified)?r.value:t},set:function(e,n,r){var i=e.getAttributeNode(r);return i||e.setAttributeNode(i=e.ownerDocument.createAttribute(r)),i.value=n+="","value"===r||n===e.getAttribute(r)?n:t}},b.attrHooks.contenteditable={get:I.get,set:function(e,t,n){I.set(e,""===t?!1:t,n)}},b.each(["width","height"],function(e,n){b.attrHooks[n]=b.extend(b.attrHooks[n],{set:function(e,r){return""===r?(e.setAttribute(n,"auto"),r):t}})})),b.support.hrefNormalized||(b.each(["href","src","width","height"],function(e,n){b.attrHooks[n]=b.extend(b.attrHooks[n],{get:function(e){var r=e.getAttribute(n,2);return null==r?t:r}})}),b.each(["href","src"],function(e,t){b.propHooks[t]={get:function(e){return e.getAttribute(t,4)}}})),b.support.style||(b.attrHooks.style={get:function(e){return e.style.cssText||t},set:function(e,t){return e.style.cssText=t+""}}),b.support.optSelected||(b.propHooks.selected=b.extend(b.propHooks.selected,{get:function(e){var t=e.parentNode;return t&&(t.selectedIndex,t.parentNode&&t.parentNode.selectedIndex),null}})),b.support.enctype||(b.propFix.enctype="encoding"),b.support.checkOn||b.each(["radio","checkbox"],function(){b.valHooks[this]={get:function(e){return null===e.getAttribute("value")?"on":e.value}}}),b.each(["radio","checkbox"],function(){b.valHooks[this]=b.extend(b.valHooks[this],{set:function(e,n){return b.isArray(n)?e.checked=b.inArray(b(e).val(),n)>=0:t}})});var Z=/^(?:input|select|textarea)$/i,et=/^key/,tt=/^(?:mouse|contextmenu)|click/,nt=/^(?:focusinfocus|focusoutblur)$/,rt=/^([^.]*)(?:\.(.+)|)$/;function it(){return!0}function ot(){return!1}b.event={global:{},add:function(e,n,r,o,a){var s,u,l,c,p,f,d,h,g,m,y,v=b._data(e);if(v){r.handler&&(c=r,r=c.handler,a=c.selector),r.guid||(r.guid=b.guid++),(u=v.events)||(u=v.events={}),(f=v.handle)||(f=v.handle=function(e){return typeof b===i||e&&b.event.triggered===e.type?t:b.event.dispatch.apply(f.elem,arguments)},f.elem=e),n=(n||"").match(w)||[""],l=n.length;while(l--)s=rt.exec(n[l])||[],g=y=s[1],m=(s[2]||"").split(".").sort(),p=b.event.special[g]||{},g=(a?p.delegateType:p.bindType)||g,p=b.event.special[g]||{},d=b.extend({type:g,origType:y,data:o,handler:r,guid:r.guid,selector:a,needsContext:a&&b.expr.match.needsContext.test(a),namespace:m.join(".")},c),(h=u[g])||(h=u[g]=[],h.delegateCount=0,p.setup&&p.setup.call(e,o,m,f)!==!1||(e.addEventListener?e.addEventListener(g,f,!1):e.attachEvent&&e.attachEvent("on"+g,f))),p.add&&(p.add.call(e,d),d.handler.guid||(d.handler.guid=r.guid)),a?h.splice(h.delegateCount++,0,d):h.push(d),b.event.global[g]=!0;e=null}},remove:function(e,t,n,r,i){var o,a,s,u,l,c,p,f,d,h,g,m=b.hasData(e)&&b._data(e);if(m&&(c=m.events)){t=(t||"").match(w)||[""],l=t.length;while(l--)if(s=rt.exec(t[l])||[],d=g=s[1],h=(s[2]||"").split(".").sort(),d){p=b.event.special[d]||{},d=(r?p.delegateType:p.bindType)||d,f=c[d]||[],s=s[2]&&RegExp("(^|\\.)"+h.join("\\.(?:.*\\.|)")+"(\\.|$)"),u=o=f.length;while(o--)a=f[o],!i&&g!==a.origType||n&&n.guid!==a.guid||s&&!s.test(a.namespace)||r&&r!==a.selector&&("**"!==r||!a.selector)||(f.splice(o,1),a.selector&&f.delegateCount--,p.remove&&p.remove.call(e,a));u&&!f.length&&(p.teardown&&p.teardown.call(e,h,m.handle)!==!1||b.removeEvent(e,d,m.handle),delete c[d])}else for(d in c)b.event.remove(e,d+t[l],n,r,!0);b.isEmptyObject(c)&&(delete m.handle,b._removeData(e,"events"))}},trigger:function(n,r,i,a){var s,u,l,c,p,f,d,h=[i||o],g=y.call(n,"type")?n.type:n,m=y.call(n,"namespace")?n.namespace.split("."):[];if(l=f=i=i||o,3!==i.nodeType&&8!==i.nodeType&&!nt.test(g+b.event.triggered)&&(g.indexOf(".")>=0&&(m=g.split("."),g=m.shift(),m.sort()),u=0>g.indexOf(":")&&"on"+g,n=n[b.expando]?n:new b.Event(g,"object"==typeof n&&n),n.isTrigger=!0,n.namespace=m.join("."),n.namespace_re=n.namespace?RegExp("(^|\\.)"+m.join("\\.(?:.*\\.|)")+"(\\.|$)"):null,n.result=t,n.target||(n.target=i),r=null==r?[n]:b.makeArray(r,[n]),p=b.event.special[g]||{},a||!p.trigger||p.trigger.apply(i,r)!==!1)){if(!a&&!p.noBubble&&!b.isWindow(i)){for(c=p.delegateType||g,nt.test(c+g)||(l=l.parentNode);l;l=l.parentNode)h.push(l),f=l;f===(i.ownerDocument||o)&&h.push(f.defaultView||f.parentWindow||e)}d=0;while((l=h[d++])&&!n.isPropagationStopped())n.type=d>1?c:p.bindType||g,s=(b._data(l,"events")||{})[n.type]&&b._data(l,"handle"),s&&s.apply(l,r),s=u&&l[u],s&&b.acceptData(l)&&s.apply&&s.apply(l,r)===!1&&n.preventDefault();if(n.type=g,!(a||n.isDefaultPrevented()||p._default&&p._default.apply(i.ownerDocument,r)!==!1||"click"===g&&b.nodeName(i,"a")||!b.acceptData(i)||!u||!i[g]||b.isWindow(i))){f=i[u],f&&(i[u]=null),b.event.triggered=g;try{i[g]()}catch(v){}b.event.triggered=t,f&&(i[u]=f)}return n.result}},dispatch:function(e){e=b.event.fix(e);var n,r,i,o,a,s=[],u=h.call(arguments),l=(b._data(this,"events")||{})[e.type]||[],c=b.event.special[e.type]||{};if(u[0]=e,e.delegateTarget=this,!c.preDispatch||c.preDispatch.call(this,e)!==!1){s=b.event.handlers.call(this,e,l),n=0;while((o=s[n++])&&!e.isPropagationStopped()){e.currentTarget=o.elem,a=0;while((i=o.handlers[a++])&&!e.isImmediatePropagationStopped())(!e.namespace_re||e.namespace_re.test(i.namespace))&&(e.handleObj=i,e.data=i.data,r=((b.event.special[i.origType]||{}).handle||i.handler).apply(o.elem,u),r!==t&&(e.result=r)===!1&&(e.preventDefault(),e.stopPropagation()))}return c.postDispatch&&c.postDispatch.call(this,e),e.result}},handlers:function(e,n){var r,i,o,a,s=[],u=n.delegateCount,l=e.target;if(u&&l.nodeType&&(!e.button||"click"!==e.type))for(;l!=this;l=l.parentNode||this)if(1===l.nodeType&&(l.disabled!==!0||"click"!==e.type)){for(o=[],a=0;u>a;a++)i=n[a],r=i.selector+" ",o[r]===t&&(o[r]=i.needsContext?b(r,this).index(l)>=0:b.find(r,this,null,[l]).length),o[r]&&o.push(i);o.length&&s.push({elem:l,handlers:o})}return n.length>u&&s.push({elem:this,handlers:n.slice(u)}),s},fix:function(e){if(e[b.expando])return e;var t,n,r,i=e.type,a=e,s=this.fixHooks[i];s||(this.fixHooks[i]=s=tt.test(i)?this.mouseHooks:et.test(i)?this.keyHooks:{}),r=s.props?this.props.concat(s.props):this.props,e=new b.Event(a),t=r.length;while(t--)n=r[t],e[n]=a[n];return e.target||(e.target=a.srcElement||o),3===e.target.nodeType&&(e.target=e.target.parentNode),e.metaKey=!!e.metaKey,s.filter?s.filter(e,a):e},props:"altKey bubbles cancelable ctrlKey currentTarget eventPhase metaKey relatedTarget shiftKey target timeStamp view which".split(" "),fixHooks:{},keyHooks:{props:"char charCode key keyCode".split(" "),filter:function(e,t){return null==e.which&&(e.which=null!=t.charCode?t.charCode:t.keyCode),e}},mouseHooks:{props:"button buttons clientX clientY fromElement offsetX offsetY pageX pageY screenX screenY toElement".split(" "),filter:function(e,n){var r,i,a,s=n.button,u=n.fromElement;return null==e.pageX&&null!=n.clientX&&(i=e.target.ownerDocument||o,a=i.documentElement,r=i.body,e.pageX=n.clientX+(a&&a.scrollLeft||r&&r.scrollLeft||0)-(a&&a.clientLeft||r&&r.clientLeft||0),e.pageY=n.clientY+(a&&a.scrollTop||r&&r.scrollTop||0)-(a&&a.clientTop||r&&r.clientTop||0)),!e.relatedTarget&&u&&(e.relatedTarget=u===e.target?n.toElement:u),e.which||s===t||(e.which=1&s?1:2&s?3:4&s?2:0),e}},special:{load:{noBubble:!0},click:{trigger:function(){return b.nodeName(this,"input")&&"checkbox"===this.type&&this.click?(this.click(),!1):t}},focus:{trigger:function(){if(this!==o.activeElement&&this.focus)try{return this.focus(),!1}catch(e){}},delegateType:"focusin"},blur:{trigger:function(){return this===o.activeElement&&this.blur?(this.blur(),!1):t},delegateType:"focusout"},beforeunload:{postDispatch:function(e){e.result!==t&&(e.originalEvent.returnValue=e.result)}}},simulate:function(e,t,n,r){var i=b.extend(new b.Event,n,{type:e,isSimulated:!0,originalEvent:{}});r?b.event.trigger(i,null,t):b.event.dispatch.call(t,i),i.isDefaultPrevented()&&n.preventDefault()}},b.removeEvent=o.removeEventListener?function(e,t,n){e.removeEventListener&&e.removeEventListener(t,n,!1)}:function(e,t,n){var r="on"+t;e.detachEvent&&(typeof e[r]===i&&(e[r]=null),e.detachEvent(r,n))},b.Event=function(e,n){return this instanceof b.Event?(e&&e.type?(this.originalEvent=e,this.type=e.type,this.isDefaultPrevented=e.defaultPrevented||e.returnValue===!1||e.getPreventDefault&&e.getPreventDefault()?it:ot):this.type=e,n&&b.extend(this,n),this.timeStamp=e&&e.timeStamp||b.now(),this[b.expando]=!0,t):new b.Event(e,n)},b.Event.prototype={isDefaultPrevented:ot,isPropagationStopped:ot,isImmediatePropagationStopped:ot,preventDefault:function(){var e=this.originalEvent;this.isDefaultPrevented=it,e&&(e.preventDefault?e.preventDefault():e.returnValue=!1)},stopPropagation:function(){var e=this.originalEvent;this.isPropagationStopped=it,e&&(e.stopPropagation&&e.stopPropagation(),e.cancelBubble=!0)},stopImmediatePropagation:function(){this.isImmediatePropagationStopped=it,this.stopPropagation()}},b.each({mouseenter:"mouseover",mouseleave:"mouseout"},function(e,t){b.event.special[e]={delegateType:t,bindType:t,handle:function(e){var n,r=this,i=e.relatedTarget,o=e.handleObj;
return(!i||i!==r&&!b.contains(r,i))&&(e.type=o.origType,n=o.handler.apply(this,arguments),e.type=t),n}}}),b.support.submitBubbles||(b.event.special.submit={setup:function(){return b.nodeName(this,"form")?!1:(b.event.add(this,"click._submit keypress._submit",function(e){var n=e.target,r=b.nodeName(n,"input")||b.nodeName(n,"button")?n.form:t;r&&!b._data(r,"submitBubbles")&&(b.event.add(r,"submit._submit",function(e){e._submit_bubble=!0}),b._data(r,"submitBubbles",!0))}),t)},postDispatch:function(e){e._submit_bubble&&(delete e._submit_bubble,this.parentNode&&!e.isTrigger&&b.event.simulate("submit",this.parentNode,e,!0))},teardown:function(){return b.nodeName(this,"form")?!1:(b.event.remove(this,"._submit"),t)}}),b.support.changeBubbles||(b.event.special.change={setup:function(){return Z.test(this.nodeName)?(("checkbox"===this.type||"radio"===this.type)&&(b.event.add(this,"propertychange._change",function(e){"checked"===e.originalEvent.propertyName&&(this._just_changed=!0)}),b.event.add(this,"click._change",function(e){this._just_changed&&!e.isTrigger&&(this._just_changed=!1),b.event.simulate("change",this,e,!0)})),!1):(b.event.add(this,"beforeactivate._change",function(e){var t=e.target;Z.test(t.nodeName)&&!b._data(t,"changeBubbles")&&(b.event.add(t,"change._change",function(e){!this.parentNode||e.isSimulated||e.isTrigger||b.event.simulate("change",this.parentNode,e,!0)}),b._data(t,"changeBubbles",!0))}),t)},handle:function(e){var n=e.target;return this!==n||e.isSimulated||e.isTrigger||"radio"!==n.type&&"checkbox"!==n.type?e.handleObj.handler.apply(this,arguments):t},teardown:function(){return b.event.remove(this,"._change"),!Z.test(this.nodeName)}}),b.support.focusinBubbles||b.each({focus:"focusin",blur:"focusout"},function(e,t){var n=0,r=function(e){b.event.simulate(t,e.target,b.event.fix(e),!0)};b.event.special[t]={setup:function(){0===n++&&o.addEventListener(e,r,!0)},teardown:function(){0===--n&&o.removeEventListener(e,r,!0)}}}),b.fn.extend({on:function(e,n,r,i,o){var a,s;if("object"==typeof e){"string"!=typeof n&&(r=r||n,n=t);for(a in e)this.on(a,n,r,e[a],o);return this}if(null==r&&null==i?(i=n,r=n=t):null==i&&("string"==typeof n?(i=r,r=t):(i=r,r=n,n=t)),i===!1)i=ot;else if(!i)return this;return 1===o&&(s=i,i=function(e){return b().off(e),s.apply(this,arguments)},i.guid=s.guid||(s.guid=b.guid++)),this.each(function(){b.event.add(this,e,i,r,n)})},one:function(e,t,n,r){return this.on(e,t,n,r,1)},off:function(e,n,r){var i,o;if(e&&e.preventDefault&&e.handleObj)return i=e.handleObj,b(e.delegateTarget).off(i.namespace?i.origType+"."+i.namespace:i.origType,i.selector,i.handler),this;if("object"==typeof e){for(o in e)this.off(o,n,e[o]);return this}return(n===!1||"function"==typeof n)&&(r=n,n=t),r===!1&&(r=ot),this.each(function(){b.event.remove(this,e,r,n)})},bind:function(e,t,n){return this.on(e,null,t,n)},unbind:function(e,t){return this.off(e,null,t)},delegate:function(e,t,n,r){return this.on(t,e,n,r)},undelegate:function(e,t,n){return 1===arguments.length?this.off(e,"**"):this.off(t,e||"**",n)},trigger:function(e,t){return this.each(function(){b.event.trigger(e,t,this)})},triggerHandler:function(e,n){var r=this[0];return r?b.event.trigger(e,n,r,!0):t}}),function(e,t){var n,r,i,o,a,s,u,l,c,p,f,d,h,g,m,y,v,x="sizzle"+-new Date,w=e.document,T={},N=0,C=0,k=it(),E=it(),S=it(),A=typeof t,j=1<<31,D=[],L=D.pop,H=D.push,q=D.slice,M=D.indexOf||function(e){var t=0,n=this.length;for(;n>t;t++)if(this[t]===e)return t;return-1},_="[\\x20\\t\\r\\n\\f]",F="(?:\\\\.|[\\w-]|[^\\x00-\\xa0])+",O=F.replace("w","w#"),B="([*^$|!~]?=)",P="\\["+_+"*("+F+")"+_+"*(?:"+B+_+"*(?:(['\"])((?:\\\\.|[^\\\\])*?)\\3|("+O+")|)|)"+_+"*\\]",R=":("+F+")(?:\\(((['\"])((?:\\\\.|[^\\\\])*?)\\3|((?:\\\\.|[^\\\\()[\\]]|"+P.replace(3,8)+")*)|.*)\\)|)",W=RegExp("^"+_+"+|((?:^|[^\\\\])(?:\\\\.)*)"+_+"+$","g"),$=RegExp("^"+_+"*,"+_+"*"),I=RegExp("^"+_+"*([\\x20\\t\\r\\n\\f>+~])"+_+"*"),z=RegExp(R),X=RegExp("^"+O+"$"),U={ID:RegExp("^#("+F+")"),CLASS:RegExp("^\\.("+F+")"),NAME:RegExp("^\\[name=['\"]?("+F+")['\"]?\\]"),TAG:RegExp("^("+F.replace("w","w*")+")"),ATTR:RegExp("^"+P),PSEUDO:RegExp("^"+R),CHILD:RegExp("^:(only|first|last|nth|nth-last)-(child|of-type)(?:\\("+_+"*(even|odd|(([+-]|)(\\d*)n|)"+_+"*(?:([+-]|)"+_+"*(\\d+)|))"+_+"*\\)|)","i"),needsContext:RegExp("^"+_+"*[>+~]|:(even|odd|eq|gt|lt|nth|first|last)(?:\\("+_+"*((?:-\\d)?\\d*)"+_+"*\\)|)(?=[^-]|$)","i")},V=/[\x20\t\r\n\f]*[+~]/,Y=/^[^{]+\{\s*\[native code/,J=/^(?:#([\w-]+)|(\w+)|\.([\w-]+))$/,G=/^(?:input|select|textarea|button)$/i,Q=/^h\d$/i,K=/'|\\/g,Z=/\=[\x20\t\r\n\f]*([^'"\]]*)[\x20\t\r\n\f]*\]/g,et=/\\([\da-fA-F]{1,6}[\x20\t\r\n\f]?|.)/g,tt=function(e,t){var n="0x"+t-65536;return n!==n?t:0>n?String.fromCharCode(n+65536):String.fromCharCode(55296|n>>10,56320|1023&n)};try{q.call(w.documentElement.childNodes,0)[0].nodeType}catch(nt){q=function(e){var t,n=[];while(t=this[e++])n.push(t);return n}}function rt(e){return Y.test(e+"")}function it(){var e,t=[];return e=function(n,r){return t.push(n+=" ")>i.cacheLength&&delete e[t.shift()],e[n]=r}}function ot(e){return e[x]=!0,e}function at(e){var t=p.createElement("div");try{return e(t)}catch(n){return!1}finally{t=null}}function st(e,t,n,r){var i,o,a,s,u,l,f,g,m,v;if((t?t.ownerDocument||t:w)!==p&&c(t),t=t||p,n=n||[],!e||"string"!=typeof e)return n;if(1!==(s=t.nodeType)&&9!==s)return[];if(!d&&!r){if(i=J.exec(e))if(a=i[1]){if(9===s){if(o=t.getElementById(a),!o||!o.parentNode)return n;if(o.id===a)return n.push(o),n}else if(t.ownerDocument&&(o=t.ownerDocument.getElementById(a))&&y(t,o)&&o.id===a)return n.push(o),n}else{if(i[2])return H.apply(n,q.call(t.getElementsByTagName(e),0)),n;if((a=i[3])&&T.getByClassName&&t.getElementsByClassName)return H.apply(n,q.call(t.getElementsByClassName(a),0)),n}if(T.qsa&&!h.test(e)){if(f=!0,g=x,m=t,v=9===s&&e,1===s&&"object"!==t.nodeName.toLowerCase()){l=ft(e),(f=t.getAttribute("id"))?g=f.replace(K,"\\$&"):t.setAttribute("id",g),g="[id='"+g+"'] ",u=l.length;while(u--)l[u]=g+dt(l[u]);m=V.test(e)&&t.parentNode||t,v=l.join(",")}if(v)try{return H.apply(n,q.call(m.querySelectorAll(v),0)),n}catch(b){}finally{f||t.removeAttribute("id")}}}return wt(e.replace(W,"$1"),t,n,r)}a=st.isXML=function(e){var t=e&&(e.ownerDocument||e).documentElement;return t?"HTML"!==t.nodeName:!1},c=st.setDocument=function(e){var n=e?e.ownerDocument||e:w;return n!==p&&9===n.nodeType&&n.documentElement?(p=n,f=n.documentElement,d=a(n),T.tagNameNoComments=at(function(e){return e.appendChild(n.createComment("")),!e.getElementsByTagName("*").length}),T.attributes=at(function(e){e.innerHTML="<select></select>";var t=typeof e.lastChild.getAttribute("multiple");return"boolean"!==t&&"string"!==t}),T.getByClassName=at(function(e){return e.innerHTML="<div class='hidden e'></div><div class='hidden'></div>",e.getElementsByClassName&&e.getElementsByClassName("e").length?(e.lastChild.className="e",2===e.getElementsByClassName("e").length):!1}),T.getByName=at(function(e){e.id=x+0,e.innerHTML="<a name='"+x+"'></a><div name='"+x+"'></div>",f.insertBefore(e,f.firstChild);var t=n.getElementsByName&&n.getElementsByName(x).length===2+n.getElementsByName(x+0).length;return T.getIdNotName=!n.getElementById(x),f.removeChild(e),t}),i.attrHandle=at(function(e){return e.innerHTML="<a href='#'></a>",e.firstChild&&typeof e.firstChild.getAttribute!==A&&"#"===e.firstChild.getAttribute("href")})?{}:{href:function(e){return e.getAttribute("href",2)},type:function(e){return e.getAttribute("type")}},T.getIdNotName?(i.find.ID=function(e,t){if(typeof t.getElementById!==A&&!d){var n=t.getElementById(e);return n&&n.parentNode?[n]:[]}},i.filter.ID=function(e){var t=e.replace(et,tt);return function(e){return e.getAttribute("id")===t}}):(i.find.ID=function(e,n){if(typeof n.getElementById!==A&&!d){var r=n.getElementById(e);return r?r.id===e||typeof r.getAttributeNode!==A&&r.getAttributeNode("id").value===e?[r]:t:[]}},i.filter.ID=function(e){var t=e.replace(et,tt);return function(e){var n=typeof e.getAttributeNode!==A&&e.getAttributeNode("id");return n&&n.value===t}}),i.find.TAG=T.tagNameNoComments?function(e,n){return typeof n.getElementsByTagName!==A?n.getElementsByTagName(e):t}:function(e,t){var n,r=[],i=0,o=t.getElementsByTagName(e);if("*"===e){while(n=o[i++])1===n.nodeType&&r.push(n);return r}return o},i.find.NAME=T.getByName&&function(e,n){return typeof n.getElementsByName!==A?n.getElementsByName(name):t},i.find.CLASS=T.getByClassName&&function(e,n){return typeof n.getElementsByClassName===A||d?t:n.getElementsByClassName(e)},g=[],h=[":focus"],(T.qsa=rt(n.querySelectorAll))&&(at(function(e){e.innerHTML="<select><option selected=''></option></select>",e.querySelectorAll("[selected]").length||h.push("\\["+_+"*(?:checked|disabled|ismap|multiple|readonly|selected|value)"),e.querySelectorAll(":checked").length||h.push(":checked")}),at(function(e){e.innerHTML="<input type='hidden' i=''/>",e.querySelectorAll("[i^='']").length&&h.push("[*^$]="+_+"*(?:\"\"|'')"),e.querySelectorAll(":enabled").length||h.push(":enabled",":disabled"),e.querySelectorAll("*,:x"),h.push(",.*:")})),(T.matchesSelector=rt(m=f.matchesSelector||f.mozMatchesSelector||f.webkitMatchesSelector||f.oMatchesSelector||f.msMatchesSelector))&&at(function(e){T.disconnectedMatch=m.call(e,"div"),m.call(e,"[s!='']:x"),g.push("!=",R)}),h=RegExp(h.join("|")),g=RegExp(g.join("|")),y=rt(f.contains)||f.compareDocumentPosition?function(e,t){var n=9===e.nodeType?e.documentElement:e,r=t&&t.parentNode;return e===r||!(!r||1!==r.nodeType||!(n.contains?n.contains(r):e.compareDocumentPosition&&16&e.compareDocumentPosition(r)))}:function(e,t){if(t)while(t=t.parentNode)if(t===e)return!0;return!1},v=f.compareDocumentPosition?function(e,t){var r;return e===t?(u=!0,0):(r=t.compareDocumentPosition&&e.compareDocumentPosition&&e.compareDocumentPosition(t))?1&r||e.parentNode&&11===e.parentNode.nodeType?e===n||y(w,e)?-1:t===n||y(w,t)?1:0:4&r?-1:1:e.compareDocumentPosition?-1:1}:function(e,t){var r,i=0,o=e.parentNode,a=t.parentNode,s=[e],l=[t];if(e===t)return u=!0,0;if(!o||!a)return e===n?-1:t===n?1:o?-1:a?1:0;if(o===a)return ut(e,t);r=e;while(r=r.parentNode)s.unshift(r);r=t;while(r=r.parentNode)l.unshift(r);while(s[i]===l[i])i++;return i?ut(s[i],l[i]):s[i]===w?-1:l[i]===w?1:0},u=!1,[0,0].sort(v),T.detectDuplicates=u,p):p},st.matches=function(e,t){return st(e,null,null,t)},st.matchesSelector=function(e,t){if((e.ownerDocument||e)!==p&&c(e),t=t.replace(Z,"='$1']"),!(!T.matchesSelector||d||g&&g.test(t)||h.test(t)))try{var n=m.call(e,t);if(n||T.disconnectedMatch||e.document&&11!==e.document.nodeType)return n}catch(r){}return st(t,p,null,[e]).length>0},st.contains=function(e,t){return(e.ownerDocument||e)!==p&&c(e),y(e,t)},st.attr=function(e,t){var n;return(e.ownerDocument||e)!==p&&c(e),d||(t=t.toLowerCase()),(n=i.attrHandle[t])?n(e):d||T.attributes?e.getAttribute(t):((n=e.getAttributeNode(t))||e.getAttribute(t))&&e[t]===!0?t:n&&n.specified?n.value:null},st.error=function(e){throw Error("Syntax error, unrecognized expression: "+e)},st.uniqueSort=function(e){var t,n=[],r=1,i=0;if(u=!T.detectDuplicates,e.sort(v),u){for(;t=e[r];r++)t===e[r-1]&&(i=n.push(r));while(i--)e.splice(n[i],1)}return e};function ut(e,t){var n=t&&e,r=n&&(~t.sourceIndex||j)-(~e.sourceIndex||j);if(r)return r;if(n)while(n=n.nextSibling)if(n===t)return-1;return e?1:-1}function lt(e){return function(t){var n=t.nodeName.toLowerCase();return"input"===n&&t.type===e}}function ct(e){return function(t){var n=t.nodeName.toLowerCase();return("input"===n||"button"===n)&&t.type===e}}function pt(e){return ot(function(t){return t=+t,ot(function(n,r){var i,o=e([],n.length,t),a=o.length;while(a--)n[i=o[a]]&&(n[i]=!(r[i]=n[i]))})})}o=st.getText=function(e){var t,n="",r=0,i=e.nodeType;if(i){if(1===i||9===i||11===i){if("string"==typeof e.textContent)return e.textContent;for(e=e.firstChild;e;e=e.nextSibling)n+=o(e)}else if(3===i||4===i)return e.nodeValue}else for(;t=e[r];r++)n+=o(t);return n},i=st.selectors={cacheLength:50,createPseudo:ot,match:U,find:{},relative:{">":{dir:"parentNode",first:!0}," ":{dir:"parentNode"},"+":{dir:"previousSibling",first:!0},"~":{dir:"previousSibling"}},preFilter:{ATTR:function(e){return e[1]=e[1].replace(et,tt),e[3]=(e[4]||e[5]||"").replace(et,tt),"~="===e[2]&&(e[3]=" "+e[3]+" "),e.slice(0,4)},CHILD:function(e){return e[1]=e[1].toLowerCase(),"nth"===e[1].slice(0,3)?(e[3]||st.error(e[0]),e[4]=+(e[4]?e[5]+(e[6]||1):2*("even"===e[3]||"odd"===e[3])),e[5]=+(e[7]+e[8]||"odd"===e[3])):e[3]&&st.error(e[0]),e},PSEUDO:function(e){var t,n=!e[5]&&e[2];return U.CHILD.test(e[0])?null:(e[4]?e[2]=e[4]:n&&z.test(n)&&(t=ft(n,!0))&&(t=n.indexOf(")",n.length-t)-n.length)&&(e[0]=e[0].slice(0,t),e[2]=n.slice(0,t)),e.slice(0,3))}},filter:{TAG:function(e){return"*"===e?function(){return!0}:(e=e.replace(et,tt).toLowerCase(),function(t){return t.nodeName&&t.nodeName.toLowerCase()===e})},CLASS:function(e){var t=k[e+" "];return t||(t=RegExp("(^|"+_+")"+e+"("+_+"|$)"))&&k(e,function(e){return t.test(e.className||typeof e.getAttribute!==A&&e.getAttribute("class")||"")})},ATTR:function(e,t,n){return function(r){var i=st.attr(r,e);return null==i?"!="===t:t?(i+="","="===t?i===n:"!="===t?i!==n:"^="===t?n&&0===i.indexOf(n):"*="===t?n&&i.indexOf(n)>-1:"$="===t?n&&i.slice(-n.length)===n:"~="===t?(" "+i+" ").indexOf(n)>-1:"|="===t?i===n||i.slice(0,n.length+1)===n+"-":!1):!0}},CHILD:function(e,t,n,r,i){var o="nth"!==e.slice(0,3),a="last"!==e.slice(-4),s="of-type"===t;return 1===r&&0===i?function(e){return!!e.parentNode}:function(t,n,u){var l,c,p,f,d,h,g=o!==a?"nextSibling":"previousSibling",m=t.parentNode,y=s&&t.nodeName.toLowerCase(),v=!u&&!s;if(m){if(o){while(g){p=t;while(p=p[g])if(s?p.nodeName.toLowerCase()===y:1===p.nodeType)return!1;h=g="only"===e&&!h&&"nextSibling"}return!0}if(h=[a?m.firstChild:m.lastChild],a&&v){c=m[x]||(m[x]={}),l=c[e]||[],d=l[0]===N&&l[1],f=l[0]===N&&l[2],p=d&&m.childNodes[d];while(p=++d&&p&&p[g]||(f=d=0)||h.pop())if(1===p.nodeType&&++f&&p===t){c[e]=[N,d,f];break}}else if(v&&(l=(t[x]||(t[x]={}))[e])&&l[0]===N)f=l[1];else while(p=++d&&p&&p[g]||(f=d=0)||h.pop())if((s?p.nodeName.toLowerCase()===y:1===p.nodeType)&&++f&&(v&&((p[x]||(p[x]={}))[e]=[N,f]),p===t))break;return f-=i,f===r||0===f%r&&f/r>=0}}},PSEUDO:function(e,t){var n,r=i.pseudos[e]||i.setFilters[e.toLowerCase()]||st.error("unsupported pseudo: "+e);return r[x]?r(t):r.length>1?(n=[e,e,"",t],i.setFilters.hasOwnProperty(e.toLowerCase())?ot(function(e,n){var i,o=r(e,t),a=o.length;while(a--)i=M.call(e,o[a]),e[i]=!(n[i]=o[a])}):function(e){return r(e,0,n)}):r}},pseudos:{not:ot(function(e){var t=[],n=[],r=s(e.replace(W,"$1"));return r[x]?ot(function(e,t,n,i){var o,a=r(e,null,i,[]),s=e.length;while(s--)(o=a[s])&&(e[s]=!(t[s]=o))}):function(e,i,o){return t[0]=e,r(t,null,o,n),!n.pop()}}),has:ot(function(e){return function(t){return st(e,t).length>0}}),contains:ot(function(e){return function(t){return(t.textContent||t.innerText||o(t)).indexOf(e)>-1}}),lang:ot(function(e){return X.test(e||"")||st.error("unsupported lang: "+e),e=e.replace(et,tt).toLowerCase(),function(t){var n;do if(n=d?t.getAttribute("xml:lang")||t.getAttribute("lang"):t.lang)return n=n.toLowerCase(),n===e||0===n.indexOf(e+"-");while((t=t.parentNode)&&1===t.nodeType);return!1}}),target:function(t){var n=e.location&&e.location.hash;return n&&n.slice(1)===t.id},root:function(e){return e===f},focus:function(e){return e===p.activeElement&&(!p.hasFocus||p.hasFocus())&&!!(e.type||e.href||~e.tabIndex)},enabled:function(e){return e.disabled===!1},disabled:function(e){return e.disabled===!0},checked:function(e){var t=e.nodeName.toLowerCase();return"input"===t&&!!e.checked||"option"===t&&!!e.selected},selected:function(e){return e.parentNode&&e.parentNode.selectedIndex,e.selected===!0},empty:function(e){for(e=e.firstChild;e;e=e.nextSibling)if(e.nodeName>"@"||3===e.nodeType||4===e.nodeType)return!1;return!0},parent:function(e){return!i.pseudos.empty(e)},header:function(e){return Q.test(e.nodeName)},input:function(e){return G.test(e.nodeName)},button:function(e){var t=e.nodeName.toLowerCase();return"input"===t&&"button"===e.type||"button"===t},text:function(e){var t;return"input"===e.nodeName.toLowerCase()&&"text"===e.type&&(null==(t=e.getAttribute("type"))||t.toLowerCase()===e.type)},first:pt(function(){return[0]}),last:pt(function(e,t){return[t-1]}),eq:pt(function(e,t,n){return[0>n?n+t:n]}),even:pt(function(e,t){var n=0;for(;t>n;n+=2)e.push(n);return e}),odd:pt(function(e,t){var n=1;for(;t>n;n+=2)e.push(n);return e}),lt:pt(function(e,t,n){var r=0>n?n+t:n;for(;--r>=0;)e.push(r);return e}),gt:pt(function(e,t,n){var r=0>n?n+t:n;for(;t>++r;)e.push(r);return e})}};for(n in{radio:!0,checkbox:!0,file:!0,password:!0,image:!0})i.pseudos[n]=lt(n);for(n in{submit:!0,reset:!0})i.pseudos[n]=ct(n);function ft(e,t){var n,r,o,a,s,u,l,c=E[e+" "];if(c)return t?0:c.slice(0);s=e,u=[],l=i.preFilter;while(s){(!n||(r=$.exec(s)))&&(r&&(s=s.slice(r[0].length)||s),u.push(o=[])),n=!1,(r=I.exec(s))&&(n=r.shift(),o.push({value:n,type:r[0].replace(W," ")}),s=s.slice(n.length));for(a in i.filter)!(r=U[a].exec(s))||l[a]&&!(r=l[a](r))||(n=r.shift(),o.push({value:n,type:a,matches:r}),s=s.slice(n.length));if(!n)break}return t?s.length:s?st.error(e):E(e,u).slice(0)}function dt(e){var t=0,n=e.length,r="";for(;n>t;t++)r+=e[t].value;return r}function ht(e,t,n){var i=t.dir,o=n&&"parentNode"===i,a=C++;return t.first?function(t,n,r){while(t=t[i])if(1===t.nodeType||o)return e(t,n,r)}:function(t,n,s){var u,l,c,p=N+" "+a;if(s){while(t=t[i])if((1===t.nodeType||o)&&e(t,n,s))return!0}else while(t=t[i])if(1===t.nodeType||o)if(c=t[x]||(t[x]={}),(l=c[i])&&l[0]===p){if((u=l[1])===!0||u===r)return u===!0}else if(l=c[i]=[p],l[1]=e(t,n,s)||r,l[1]===!0)return!0}}function gt(e){return e.length>1?function(t,n,r){var i=e.length;while(i--)if(!e[i](t,n,r))return!1;return!0}:e[0]}function mt(e,t,n,r,i){var o,a=[],s=0,u=e.length,l=null!=t;for(;u>s;s++)(o=e[s])&&(!n||n(o,r,i))&&(a.push(o),l&&t.push(s));return a}function yt(e,t,n,r,i,o){return r&&!r[x]&&(r=yt(r)),i&&!i[x]&&(i=yt(i,o)),ot(function(o,a,s,u){var l,c,p,f=[],d=[],h=a.length,g=o||xt(t||"*",s.nodeType?[s]:s,[]),m=!e||!o&&t?g:mt(g,f,e,s,u),y=n?i||(o?e:h||r)?[]:a:m;if(n&&n(m,y,s,u),r){l=mt(y,d),r(l,[],s,u),c=l.length;while(c--)(p=l[c])&&(y[d[c]]=!(m[d[c]]=p))}if(o){if(i||e){if(i){l=[],c=y.length;while(c--)(p=y[c])&&l.push(m[c]=p);i(null,y=[],l,u)}c=y.length;while(c--)(p=y[c])&&(l=i?M.call(o,p):f[c])>-1&&(o[l]=!(a[l]=p))}}else y=mt(y===a?y.splice(h,y.length):y),i?i(null,a,y,u):H.apply(a,y)})}function vt(e){var t,n,r,o=e.length,a=i.relative[e[0].type],s=a||i.relative[" "],u=a?1:0,c=ht(function(e){return e===t},s,!0),p=ht(function(e){return M.call(t,e)>-1},s,!0),f=[function(e,n,r){return!a&&(r||n!==l)||((t=n).nodeType?c(e,n,r):p(e,n,r))}];for(;o>u;u++)if(n=i.relative[e[u].type])f=[ht(gt(f),n)];else{if(n=i.filter[e[u].type].apply(null,e[u].matches),n[x]){for(r=++u;o>r;r++)if(i.relative[e[r].type])break;return yt(u>1&&gt(f),u>1&&dt(e.slice(0,u-1)).replace(W,"$1"),n,r>u&&vt(e.slice(u,r)),o>r&&vt(e=e.slice(r)),o>r&&dt(e))}f.push(n)}return gt(f)}function bt(e,t){var n=0,o=t.length>0,a=e.length>0,s=function(s,u,c,f,d){var h,g,m,y=[],v=0,b="0",x=s&&[],w=null!=d,T=l,C=s||a&&i.find.TAG("*",d&&u.parentNode||u),k=N+=null==T?1:Math.random()||.1;for(w&&(l=u!==p&&u,r=n);null!=(h=C[b]);b++){if(a&&h){g=0;while(m=e[g++])if(m(h,u,c)){f.push(h);break}w&&(N=k,r=++n)}o&&((h=!m&&h)&&v--,s&&x.push(h))}if(v+=b,o&&b!==v){g=0;while(m=t[g++])m(x,y,u,c);if(s){if(v>0)while(b--)x[b]||y[b]||(y[b]=L.call(f));y=mt(y)}H.apply(f,y),w&&!s&&y.length>0&&v+t.length>1&&st.uniqueSort(f)}return w&&(N=k,l=T),x};return o?ot(s):s}s=st.compile=function(e,t){var n,r=[],i=[],o=S[e+" "];if(!o){t||(t=ft(e)),n=t.length;while(n--)o=vt(t[n]),o[x]?r.push(o):i.push(o);o=S(e,bt(i,r))}return o};function xt(e,t,n){var r=0,i=t.length;for(;i>r;r++)st(e,t[r],n);return n}function wt(e,t,n,r){var o,a,u,l,c,p=ft(e);if(!r&&1===p.length){if(a=p[0]=p[0].slice(0),a.length>2&&"ID"===(u=a[0]).type&&9===t.nodeType&&!d&&i.relative[a[1].type]){if(t=i.find.ID(u.matches[0].replace(et,tt),t)[0],!t)return n;e=e.slice(a.shift().value.length)}o=U.needsContext.test(e)?0:a.length;while(o--){if(u=a[o],i.relative[l=u.type])break;if((c=i.find[l])&&(r=c(u.matches[0].replace(et,tt),V.test(a[0].type)&&t.parentNode||t))){if(a.splice(o,1),e=r.length&&dt(a),!e)return H.apply(n,q.call(r,0)),n;break}}}return s(e,p)(r,t,d,n,V.test(e)),n}i.pseudos.nth=i.pseudos.eq;function Tt(){}i.filters=Tt.prototype=i.pseudos,i.setFilters=new Tt,c(),st.attr=b.attr,b.find=st,b.expr=st.selectors,b.expr[":"]=b.expr.pseudos,b.unique=st.uniqueSort,b.text=st.getText,b.isXMLDoc=st.isXML,b.contains=st.contains}(e);var at=/Until$/,st=/^(?:parents|prev(?:Until|All))/,ut=/^.[^:#\[\.,]*$/,lt=b.expr.match.needsContext,ct={children:!0,contents:!0,next:!0,prev:!0};b.fn.extend({find:function(e){var t,n,r,i=this.length;if("string"!=typeof e)return r=this,this.pushStack(b(e).filter(function(){for(t=0;i>t;t++)if(b.contains(r[t],this))return!0}));for(n=[],t=0;i>t;t++)b.find(e,this[t],n);return n=this.pushStack(i>1?b.unique(n):n),n.selector=(this.selector?this.selector+" ":"")+e,n},has:function(e){var t,n=b(e,this),r=n.length;return this.filter(function(){for(t=0;r>t;t++)if(b.contains(this,n[t]))return!0})},not:function(e){return this.pushStack(ft(this,e,!1))},filter:function(e){return this.pushStack(ft(this,e,!0))},is:function(e){return!!e&&("string"==typeof e?lt.test(e)?b(e,this.context).index(this[0])>=0:b.filter(e,this).length>0:this.filter(e).length>0)},closest:function(e,t){var n,r=0,i=this.length,o=[],a=lt.test(e)||"string"!=typeof e?b(e,t||this.context):0;for(;i>r;r++){n=this[r];while(n&&n.ownerDocument&&n!==t&&11!==n.nodeType){if(a?a.index(n)>-1:b.find.matchesSelector(n,e)){o.push(n);break}n=n.parentNode}}return this.pushStack(o.length>1?b.unique(o):o)},index:function(e){return e?"string"==typeof e?b.inArray(this[0],b(e)):b.inArray(e.jquery?e[0]:e,this):this[0]&&this[0].parentNode?this.first().prevAll().length:-1},add:function(e,t){var n="string"==typeof e?b(e,t):b.makeArray(e&&e.nodeType?[e]:e),r=b.merge(this.get(),n);return this.pushStack(b.unique(r))},addBack:function(e){return this.add(null==e?this.prevObject:this.prevObject.filter(e))}}),b.fn.andSelf=b.fn.addBack;function pt(e,t){do e=e[t];while(e&&1!==e.nodeType);return e}b.each({parent:function(e){var t=e.parentNode;return t&&11!==t.nodeType?t:null},parents:function(e){return b.dir(e,"parentNode")},parentsUntil:function(e,t,n){return b.dir(e,"parentNode",n)},next:function(e){return pt(e,"nextSibling")},prev:function(e){return pt(e,"previousSibling")},nextAll:function(e){return b.dir(e,"nextSibling")},prevAll:function(e){return b.dir(e,"previousSibling")},nextUntil:function(e,t,n){return b.dir(e,"nextSibling",n)},prevUntil:function(e,t,n){return b.dir(e,"previousSibling",n)},siblings:function(e){return b.sibling((e.parentNode||{}).firstChild,e)},children:function(e){return b.sibling(e.firstChild)},contents:function(e){return b.nodeName(e,"iframe")?e.contentDocument||e.contentWindow.document:b.merge([],e.childNodes)}},function(e,t){b.fn[e]=function(n,r){var i=b.map(this,t,n);return at.test(e)||(r=n),r&&"string"==typeof r&&(i=b.filter(r,i)),i=this.length>1&&!ct[e]?b.unique(i):i,this.length>1&&st.test(e)&&(i=i.reverse()),this.pushStack(i)}}),b.extend({filter:function(e,t,n){return n&&(e=":not("+e+")"),1===t.length?b.find.matchesSelector(t[0],e)?[t[0]]:[]:b.find.matches(e,t)},dir:function(e,n,r){var i=[],o=e[n];while(o&&9!==o.nodeType&&(r===t||1!==o.nodeType||!b(o).is(r)))1===o.nodeType&&i.push(o),o=o[n];return i},sibling:function(e,t){var n=[];for(;e;e=e.nextSibling)1===e.nodeType&&e!==t&&n.push(e);return n}});function ft(e,t,n){if(t=t||0,b.isFunction(t))return b.grep(e,function(e,r){var i=!!t.call(e,r,e);return i===n});if(t.nodeType)return b.grep(e,function(e){return e===t===n});if("string"==typeof t){var r=b.grep(e,function(e){return 1===e.nodeType});if(ut.test(t))return b.filter(t,r,!n);t=b.filter(t,r)}return b.grep(e,function(e){return b.inArray(e,t)>=0===n})}function dt(e){var t=ht.split("|"),n=e.createDocumentFragment();if(n.createElement)while(t.length)n.createElement(t.pop());return n}var ht="abbr|article|aside|audio|bdi|canvas|data|datalist|details|figcaption|figure|footer|header|hgroup|mark|meter|nav|output|progress|section|summary|time|video",gt=/ jQuery\d+="(?:null|\d+)"/g,mt=RegExp("<(?:"+ht+")[\\s/>]","i"),yt=/^\s+/,vt=/<(?!area|br|col|embed|hr|img|input|link|meta|param)(([\w:]+)[^>]*)\/>/gi,bt=/<([\w:]+)/,xt=/<tbody/i,wt=/<|&#?\w+;/,Tt=/<(?:script|style|link)/i,Nt=/^(?:checkbox|radio)$/i,Ct=/checked\s*(?:[^=]|=\s*.checked.)/i,kt=/^$|\/(?:java|ecma)script/i,Et=/^true\/(.*)/,St=/^\s*<!(?:\[CDATA\[|--)|(?:\]\]|--)>\s*$/g,At={option:[1,"<select multiple='multiple'>","</select>"],legend:[1,"<fieldset>","</fieldset>"],area:[1,"<map>","</map>"],param:[1,"<object>","</object>"],thead:[1,"<table>","</table>"],tr:[2,"<table><tbody>","</tbody></table>"],col:[2,"<table><tbody></tbody><colgroup>","</colgroup></table>"],td:[3,"<table><tbody><tr>","</tr></tbody></table>"],_default:b.support.htmlSerialize?[0,"",""]:[1,"X<div>","</div>"]},jt=dt(o),Dt=jt.appendChild(o.createElement("div"));At.optgroup=At.option,At.tbody=At.tfoot=At.colgroup=At.caption=At.thead,At.th=At.td,b.fn.extend({text:function(e){return b.access(this,function(e){return e===t?b.text(this):this.empty().append((this[0]&&this[0].ownerDocument||o).createTextNode(e))},null,e,arguments.length)},wrapAll:function(e){if(b.isFunction(e))return this.each(function(t){b(this).wrapAll(e.call(this,t))});if(this[0]){var t=b(e,this[0].ownerDocument).eq(0).clone(!0);this[0].parentNode&&t.insertBefore(this[0]),t.map(function(){var e=this;while(e.firstChild&&1===e.firstChild.nodeType)e=e.firstChild;return e}).append(this)}return this},wrapInner:function(e){return b.isFunction(e)?this.each(function(t){b(this).wrapInner(e.call(this,t))}):this.each(function(){var t=b(this),n=t.contents();n.length?n.wrapAll(e):t.append(e)})},wrap:function(e){var t=b.isFunction(e);return this.each(function(n){b(this).wrapAll(t?e.call(this,n):e)})},unwrap:function(){return this.parent().each(function(){b.nodeName(this,"body")||b(this).replaceWith(this.childNodes)}).end()},append:function(){return this.domManip(arguments,!0,function(e){(1===this.nodeType||11===this.nodeType||9===this.nodeType)&&this.appendChild(e)})},prepend:function(){return this.domManip(arguments,!0,function(e){(1===this.nodeType||11===this.nodeType||9===this.nodeType)&&this.insertBefore(e,this.firstChild)})},before:function(){return this.domManip(arguments,!1,function(e){this.parentNode&&this.parentNode.insertBefore(e,this)})},after:function(){return this.domManip(arguments,!1,function(e){this.parentNode&&this.parentNode.insertBefore(e,this.nextSibling)})},remove:function(e,t){var n,r=0;for(;null!=(n=this[r]);r++)(!e||b.filter(e,[n]).length>0)&&(t||1!==n.nodeType||b.cleanData(Ot(n)),n.parentNode&&(t&&b.contains(n.ownerDocument,n)&&Mt(Ot(n,"script")),n.parentNode.removeChild(n)));return this},empty:function(){var e,t=0;for(;null!=(e=this[t]);t++){1===e.nodeType&&b.cleanData(Ot(e,!1));while(e.firstChild)e.removeChild(e.firstChild);e.options&&b.nodeName(e,"select")&&(e.options.length=0)}return this},clone:function(e,t){return e=null==e?!1:e,t=null==t?e:t,this.map(function(){return b.clone(this,e,t)})},html:function(e){return b.access(this,function(e){var n=this[0]||{},r=0,i=this.length;if(e===t)return 1===n.nodeType?n.innerHTML.replace(gt,""):t;if(!("string"!=typeof e||Tt.test(e)||!b.support.htmlSerialize&&mt.test(e)||!b.support.leadingWhitespace&&yt.test(e)||At[(bt.exec(e)||["",""])[1].toLowerCase()])){e=e.replace(vt,"<$1></$2>");try{for(;i>r;r++)n=this[r]||{},1===n.nodeType&&(b.cleanData(Ot(n,!1)),n.innerHTML=e);n=0}catch(o){}}n&&this.empty().append(e)},null,e,arguments.length)},replaceWith:function(e){var t=b.isFunction(e);return t||"string"==typeof e||(e=b(e).not(this).detach()),this.domManip([e],!0,function(e){var t=this.nextSibling,n=this.parentNode;n&&(b(this).remove(),n.insertBefore(e,t))})},detach:function(e){return this.remove(e,!0)},domManip:function(e,n,r){e=f.apply([],e);var i,o,a,s,u,l,c=0,p=this.length,d=this,h=p-1,g=e[0],m=b.isFunction(g);if(m||!(1>=p||"string"!=typeof g||b.support.checkClone)&&Ct.test(g))return this.each(function(i){var o=d.eq(i);m&&(e[0]=g.call(this,i,n?o.html():t)),o.domManip(e,n,r)});if(p&&(l=b.buildFragment(e,this[0].ownerDocument,!1,this),i=l.firstChild,1===l.childNodes.length&&(l=i),i)){for(n=n&&b.nodeName(i,"tr"),s=b.map(Ot(l,"script"),Ht),a=s.length;p>c;c++)o=l,c!==h&&(o=b.clone(o,!0,!0),a&&b.merge(s,Ot(o,"script"))),r.call(n&&b.nodeName(this[c],"table")?Lt(this[c],"tbody"):this[c],o,c);if(a)for(u=s[s.length-1].ownerDocument,b.map(s,qt),c=0;a>c;c++)o=s[c],kt.test(o.type||"")&&!b._data(o,"globalEval")&&b.contains(u,o)&&(o.src?b.ajax({url:o.src,type:"GET",dataType:"script",async:!1,global:!1,"throws":!0}):b.globalEval((o.text||o.textContent||o.innerHTML||"").replace(St,"")));l=i=null}return this}});function Lt(e,t){return e.getElementsByTagName(t)[0]||e.appendChild(e.ownerDocument.createElement(t))}function Ht(e){var t=e.getAttributeNode("type");return e.type=(t&&t.specified)+"/"+e.type,e}function qt(e){var t=Et.exec(e.type);return t?e.type=t[1]:e.removeAttribute("type"),e}function Mt(e,t){var n,r=0;for(;null!=(n=e[r]);r++)b._data(n,"globalEval",!t||b._data(t[r],"globalEval"))}function _t(e,t){if(1===t.nodeType&&b.hasData(e)){var n,r,i,o=b._data(e),a=b._data(t,o),s=o.events;if(s){delete a.handle,a.events={};for(n in s)for(r=0,i=s[n].length;i>r;r++)b.event.add(t,n,s[n][r])}a.data&&(a.data=b.extend({},a.data))}}function Ft(e,t){var n,r,i;if(1===t.nodeType){if(n=t.nodeName.toLowerCase(),!b.support.noCloneEvent&&t[b.expando]){i=b._data(t);for(r in i.events)b.removeEvent(t,r,i.handle);t.removeAttribute(b.expando)}"script"===n&&t.text!==e.text?(Ht(t).text=e.text,qt(t)):"object"===n?(t.parentNode&&(t.outerHTML=e.outerHTML),b.support.html5Clone&&e.innerHTML&&!b.trim(t.innerHTML)&&(t.innerHTML=e.innerHTML)):"input"===n&&Nt.test(e.type)?(t.defaultChecked=t.checked=e.checked,t.value!==e.value&&(t.value=e.value)):"option"===n?t.defaultSelected=t.selected=e.defaultSelected:("input"===n||"textarea"===n)&&(t.defaultValue=e.defaultValue)}}b.each({appendTo:"append",prependTo:"prepend",insertBefore:"before",insertAfter:"after",replaceAll:"replaceWith"},function(e,t){b.fn[e]=function(e){var n,r=0,i=[],o=b(e),a=o.length-1;for(;a>=r;r++)n=r===a?this:this.clone(!0),b(o[r])[t](n),d.apply(i,n.get());return this.pushStack(i)}});function Ot(e,n){var r,o,a=0,s=typeof e.getElementsByTagName!==i?e.getElementsByTagName(n||"*"):typeof e.querySelectorAll!==i?e.querySelectorAll(n||"*"):t;if(!s)for(s=[],r=e.childNodes||e;null!=(o=r[a]);a++)!n||b.nodeName(o,n)?s.push(o):b.merge(s,Ot(o,n));return n===t||n&&b.nodeName(e,n)?b.merge([e],s):s}function Bt(e){Nt.test(e.type)&&(e.defaultChecked=e.checked)}b.extend({clone:function(e,t,n){var r,i,o,a,s,u=b.contains(e.ownerDocument,e);if(b.support.html5Clone||b.isXMLDoc(e)||!mt.test("<"+e.nodeName+">")?o=e.cloneNode(!0):(Dt.innerHTML=e.outerHTML,Dt.removeChild(o=Dt.firstChild)),!(b.support.noCloneEvent&&b.support.noCloneChecked||1!==e.nodeType&&11!==e.nodeType||b.isXMLDoc(e)))for(r=Ot(o),s=Ot(e),a=0;null!=(i=s[a]);++a)r[a]&&Ft(i,r[a]);if(t)if(n)for(s=s||Ot(e),r=r||Ot(o),a=0;null!=(i=s[a]);a++)_t(i,r[a]);else _t(e,o);return r=Ot(o,"script"),r.length>0&&Mt(r,!u&&Ot(e,"script")),r=s=i=null,o},buildFragment:function(e,t,n,r){var i,o,a,s,u,l,c,p=e.length,f=dt(t),d=[],h=0;for(;p>h;h++)if(o=e[h],o||0===o)if("object"===b.type(o))b.merge(d,o.nodeType?[o]:o);else if(wt.test(o)){s=s||f.appendChild(t.createElement("div")),u=(bt.exec(o)||["",""])[1].toLowerCase(),c=At[u]||At._default,s.innerHTML=c[1]+o.replace(vt,"<$1></$2>")+c[2],i=c[0];while(i--)s=s.lastChild;if(!b.support.leadingWhitespace&&yt.test(o)&&d.push(t.createTextNode(yt.exec(o)[0])),!b.support.tbody){o="table"!==u||xt.test(o)?"<table>"!==c[1]||xt.test(o)?0:s:s.firstChild,i=o&&o.childNodes.length;while(i--)b.nodeName(l=o.childNodes[i],"tbody")&&!l.childNodes.length&&o.removeChild(l)
}b.merge(d,s.childNodes),s.textContent="";while(s.firstChild)s.removeChild(s.firstChild);s=f.lastChild}else d.push(t.createTextNode(o));s&&f.removeChild(s),b.support.appendChecked||b.grep(Ot(d,"input"),Bt),h=0;while(o=d[h++])if((!r||-1===b.inArray(o,r))&&(a=b.contains(o.ownerDocument,o),s=Ot(f.appendChild(o),"script"),a&&Mt(s),n)){i=0;while(o=s[i++])kt.test(o.type||"")&&n.push(o)}return s=null,f},cleanData:function(e,t){var n,r,o,a,s=0,u=b.expando,l=b.cache,p=b.support.deleteExpando,f=b.event.special;for(;null!=(n=e[s]);s++)if((t||b.acceptData(n))&&(o=n[u],a=o&&l[o])){if(a.events)for(r in a.events)f[r]?b.event.remove(n,r):b.removeEvent(n,r,a.handle);l[o]&&(delete l[o],p?delete n[u]:typeof n.removeAttribute!==i?n.removeAttribute(u):n[u]=null,c.push(o))}}});var Pt,Rt,Wt,$t=/alpha\([^)]*\)/i,It=/opacity\s*=\s*([^)]*)/,zt=/^(top|right|bottom|left)$/,Xt=/^(none|table(?!-c[ea]).+)/,Ut=/^margin/,Vt=RegExp("^("+x+")(.*)$","i"),Yt=RegExp("^("+x+")(?!px)[a-z%]+$","i"),Jt=RegExp("^([+-])=("+x+")","i"),Gt={BODY:"block"},Qt={position:"absolute",visibility:"hidden",display:"block"},Kt={letterSpacing:0,fontWeight:400},Zt=["Top","Right","Bottom","Left"],en=["Webkit","O","Moz","ms"];function tn(e,t){if(t in e)return t;var n=t.charAt(0).toUpperCase()+t.slice(1),r=t,i=en.length;while(i--)if(t=en[i]+n,t in e)return t;return r}function nn(e,t){return e=t||e,"none"===b.css(e,"display")||!b.contains(e.ownerDocument,e)}function rn(e,t){var n,r,i,o=[],a=0,s=e.length;for(;s>a;a++)r=e[a],r.style&&(o[a]=b._data(r,"olddisplay"),n=r.style.display,t?(o[a]||"none"!==n||(r.style.display=""),""===r.style.display&&nn(r)&&(o[a]=b._data(r,"olddisplay",un(r.nodeName)))):o[a]||(i=nn(r),(n&&"none"!==n||!i)&&b._data(r,"olddisplay",i?n:b.css(r,"display"))));for(a=0;s>a;a++)r=e[a],r.style&&(t&&"none"!==r.style.display&&""!==r.style.display||(r.style.display=t?o[a]||"":"none"));return e}b.fn.extend({css:function(e,n){return b.access(this,function(e,n,r){var i,o,a={},s=0;if(b.isArray(n)){for(o=Rt(e),i=n.length;i>s;s++)a[n[s]]=b.css(e,n[s],!1,o);return a}return r!==t?b.style(e,n,r):b.css(e,n)},e,n,arguments.length>1)},show:function(){return rn(this,!0)},hide:function(){return rn(this)},toggle:function(e){var t="boolean"==typeof e;return this.each(function(){(t?e:nn(this))?b(this).show():b(this).hide()})}}),b.extend({cssHooks:{opacity:{get:function(e,t){if(t){var n=Wt(e,"opacity");return""===n?"1":n}}}},cssNumber:{columnCount:!0,fillOpacity:!0,fontWeight:!0,lineHeight:!0,opacity:!0,orphans:!0,widows:!0,zIndex:!0,zoom:!0},cssProps:{"float":b.support.cssFloat?"cssFloat":"styleFloat"},style:function(e,n,r,i){if(e&&3!==e.nodeType&&8!==e.nodeType&&e.style){var o,a,s,u=b.camelCase(n),l=e.style;if(n=b.cssProps[u]||(b.cssProps[u]=tn(l,u)),s=b.cssHooks[n]||b.cssHooks[u],r===t)return s&&"get"in s&&(o=s.get(e,!1,i))!==t?o:l[n];if(a=typeof r,"string"===a&&(o=Jt.exec(r))&&(r=(o[1]+1)*o[2]+parseFloat(b.css(e,n)),a="number"),!(null==r||"number"===a&&isNaN(r)||("number"!==a||b.cssNumber[u]||(r+="px"),b.support.clearCloneStyle||""!==r||0!==n.indexOf("background")||(l[n]="inherit"),s&&"set"in s&&(r=s.set(e,r,i))===t)))try{l[n]=r}catch(c){}}},css:function(e,n,r,i){var o,a,s,u=b.camelCase(n);return n=b.cssProps[u]||(b.cssProps[u]=tn(e.style,u)),s=b.cssHooks[n]||b.cssHooks[u],s&&"get"in s&&(a=s.get(e,!0,r)),a===t&&(a=Wt(e,n,i)),"normal"===a&&n in Kt&&(a=Kt[n]),""===r||r?(o=parseFloat(a),r===!0||b.isNumeric(o)?o||0:a):a},swap:function(e,t,n,r){var i,o,a={};for(o in t)a[o]=e.style[o],e.style[o]=t[o];i=n.apply(e,r||[]);for(o in t)e.style[o]=a[o];return i}}),e.getComputedStyle?(Rt=function(t){return e.getComputedStyle(t,null)},Wt=function(e,n,r){var i,o,a,s=r||Rt(e),u=s?s.getPropertyValue(n)||s[n]:t,l=e.style;return s&&(""!==u||b.contains(e.ownerDocument,e)||(u=b.style(e,n)),Yt.test(u)&&Ut.test(n)&&(i=l.width,o=l.minWidth,a=l.maxWidth,l.minWidth=l.maxWidth=l.width=u,u=s.width,l.width=i,l.minWidth=o,l.maxWidth=a)),u}):o.documentElement.currentStyle&&(Rt=function(e){return e.currentStyle},Wt=function(e,n,r){var i,o,a,s=r||Rt(e),u=s?s[n]:t,l=e.style;return null==u&&l&&l[n]&&(u=l[n]),Yt.test(u)&&!zt.test(n)&&(i=l.left,o=e.runtimeStyle,a=o&&o.left,a&&(o.left=e.currentStyle.left),l.left="fontSize"===n?"1em":u,u=l.pixelLeft+"px",l.left=i,a&&(o.left=a)),""===u?"auto":u});function on(e,t,n){var r=Vt.exec(t);return r?Math.max(0,r[1]-(n||0))+(r[2]||"px"):t}function an(e,t,n,r,i){var o=n===(r?"border":"content")?4:"width"===t?1:0,a=0;for(;4>o;o+=2)"margin"===n&&(a+=b.css(e,n+Zt[o],!0,i)),r?("content"===n&&(a-=b.css(e,"padding"+Zt[o],!0,i)),"margin"!==n&&(a-=b.css(e,"border"+Zt[o]+"Width",!0,i))):(a+=b.css(e,"padding"+Zt[o],!0,i),"padding"!==n&&(a+=b.css(e,"border"+Zt[o]+"Width",!0,i)));return a}function sn(e,t,n){var r=!0,i="width"===t?e.offsetWidth:e.offsetHeight,o=Rt(e),a=b.support.boxSizing&&"border-box"===b.css(e,"boxSizing",!1,o);if(0>=i||null==i){if(i=Wt(e,t,o),(0>i||null==i)&&(i=e.style[t]),Yt.test(i))return i;r=a&&(b.support.boxSizingReliable||i===e.style[t]),i=parseFloat(i)||0}return i+an(e,t,n||(a?"border":"content"),r,o)+"px"}function un(e){var t=o,n=Gt[e];return n||(n=ln(e,t),"none"!==n&&n||(Pt=(Pt||b("<iframe frameborder='0' width='0' height='0'/>").css("cssText","display:block !important")).appendTo(t.documentElement),t=(Pt[0].contentWindow||Pt[0].contentDocument).document,t.write("<!doctype html><html><body>"),t.close(),n=ln(e,t),Pt.detach()),Gt[e]=n),n}function ln(e,t){var n=b(t.createElement(e)).appendTo(t.body),r=b.css(n[0],"display");return n.remove(),r}b.each(["height","width"],function(e,n){b.cssHooks[n]={get:function(e,r,i){return r?0===e.offsetWidth&&Xt.test(b.css(e,"display"))?b.swap(e,Qt,function(){return sn(e,n,i)}):sn(e,n,i):t},set:function(e,t,r){var i=r&&Rt(e);return on(e,t,r?an(e,n,r,b.support.boxSizing&&"border-box"===b.css(e,"boxSizing",!1,i),i):0)}}}),b.support.opacity||(b.cssHooks.opacity={get:function(e,t){return It.test((t&&e.currentStyle?e.currentStyle.filter:e.style.filter)||"")?.01*parseFloat(RegExp.$1)+"":t?"1":""},set:function(e,t){var n=e.style,r=e.currentStyle,i=b.isNumeric(t)?"alpha(opacity="+100*t+")":"",o=r&&r.filter||n.filter||"";n.zoom=1,(t>=1||""===t)&&""===b.trim(o.replace($t,""))&&n.removeAttribute&&(n.removeAttribute("filter"),""===t||r&&!r.filter)||(n.filter=$t.test(o)?o.replace($t,i):o+" "+i)}}),b(function(){b.support.reliableMarginRight||(b.cssHooks.marginRight={get:function(e,n){return n?b.swap(e,{display:"inline-block"},Wt,[e,"marginRight"]):t}}),!b.support.pixelPosition&&b.fn.position&&b.each(["top","left"],function(e,n){b.cssHooks[n]={get:function(e,r){return r?(r=Wt(e,n),Yt.test(r)?b(e).position()[n]+"px":r):t}}})}),b.expr&&b.expr.filters&&(b.expr.filters.hidden=function(e){return 0>=e.offsetWidth&&0>=e.offsetHeight||!b.support.reliableHiddenOffsets&&"none"===(e.style&&e.style.display||b.css(e,"display"))},b.expr.filters.visible=function(e){return!b.expr.filters.hidden(e)}),b.each({margin:"",padding:"",border:"Width"},function(e,t){b.cssHooks[e+t]={expand:function(n){var r=0,i={},o="string"==typeof n?n.split(" "):[n];for(;4>r;r++)i[e+Zt[r]+t]=o[r]||o[r-2]||o[0];return i}},Ut.test(e)||(b.cssHooks[e+t].set=on)});var cn=/%20/g,pn=/\[\]$/,fn=/\r?\n/g,dn=/^(?:submit|button|image|reset|file)$/i,hn=/^(?:input|select|textarea|keygen)/i;b.fn.extend({serialize:function(){return b.param(this.serializeArray())},serializeArray:function(){return this.map(function(){var e=b.prop(this,"elements");return e?b.makeArray(e):this}).filter(function(){var e=this.type;return this.name&&!b(this).is(":disabled")&&hn.test(this.nodeName)&&!dn.test(e)&&(this.checked||!Nt.test(e))}).map(function(e,t){var n=b(this).val();return null==n?null:b.isArray(n)?b.map(n,function(e){return{name:t.name,value:e.replace(fn,"\r\n")}}):{name:t.name,value:n.replace(fn,"\r\n")}}).get()}}),b.param=function(e,n){var r,i=[],o=function(e,t){t=b.isFunction(t)?t():null==t?"":t,i[i.length]=encodeURIComponent(e)+"="+encodeURIComponent(t)};if(n===t&&(n=b.ajaxSettings&&b.ajaxSettings.traditional),b.isArray(e)||e.jquery&&!b.isPlainObject(e))b.each(e,function(){o(this.name,this.value)});else for(r in e)gn(r,e[r],n,o);return i.join("&").replace(cn,"+")};function gn(e,t,n,r){var i;if(b.isArray(t))b.each(t,function(t,i){n||pn.test(e)?r(e,i):gn(e+"["+("object"==typeof i?t:"")+"]",i,n,r)});else if(n||"object"!==b.type(t))r(e,t);else for(i in t)gn(e+"["+i+"]",t[i],n,r)}b.each("blur focus focusin focusout load resize scroll unload click dblclick mousedown mouseup mousemove mouseover mouseout mouseenter mouseleave change select submit keydown keypress keyup error contextmenu".split(" "),function(e,t){b.fn[t]=function(e,n){return arguments.length>0?this.on(t,null,e,n):this.trigger(t)}}),b.fn.hover=function(e,t){return this.mouseenter(e).mouseleave(t||e)};var mn,yn,vn=b.now(),bn=/\?/,xn=/#.*$/,wn=/([?&])_=[^&]*/,Tn=/^(.*?):[ \t]*([^\r\n]*)\r?$/gm,Nn=/^(?:about|app|app-storage|.+-extension|file|res|widget):$/,Cn=/^(?:GET|HEAD)$/,kn=/^\/\//,En=/^([\w.+-]+:)(?:\/\/([^\/?#:]*)(?::(\d+)|)|)/,Sn=b.fn.load,An={},jn={},Dn="*/".concat("*");try{yn=a.href}catch(Ln){yn=o.createElement("a"),yn.href="",yn=yn.href}mn=En.exec(yn.toLowerCase())||[];function Hn(e){return function(t,n){"string"!=typeof t&&(n=t,t="*");var r,i=0,o=t.toLowerCase().match(w)||[];if(b.isFunction(n))while(r=o[i++])"+"===r[0]?(r=r.slice(1)||"*",(e[r]=e[r]||[]).unshift(n)):(e[r]=e[r]||[]).push(n)}}function qn(e,n,r,i){var o={},a=e===jn;function s(u){var l;return o[u]=!0,b.each(e[u]||[],function(e,u){var c=u(n,r,i);return"string"!=typeof c||a||o[c]?a?!(l=c):t:(n.dataTypes.unshift(c),s(c),!1)}),l}return s(n.dataTypes[0])||!o["*"]&&s("*")}function Mn(e,n){var r,i,o=b.ajaxSettings.flatOptions||{};for(i in n)n[i]!==t&&((o[i]?e:r||(r={}))[i]=n[i]);return r&&b.extend(!0,e,r),e}b.fn.load=function(e,n,r){if("string"!=typeof e&&Sn)return Sn.apply(this,arguments);var i,o,a,s=this,u=e.indexOf(" ");return u>=0&&(i=e.slice(u,e.length),e=e.slice(0,u)),b.isFunction(n)?(r=n,n=t):n&&"object"==typeof n&&(a="POST"),s.length>0&&b.ajax({url:e,type:a,dataType:"html",data:n}).done(function(e){o=arguments,s.html(i?b("<div>").append(b.parseHTML(e)).find(i):e)}).complete(r&&function(e,t){s.each(r,o||[e.responseText,t,e])}),this},b.each(["ajaxStart","ajaxStop","ajaxComplete","ajaxError","ajaxSuccess","ajaxSend"],function(e,t){b.fn[t]=function(e){return this.on(t,e)}}),b.each(["get","post"],function(e,n){b[n]=function(e,r,i,o){return b.isFunction(r)&&(o=o||i,i=r,r=t),b.ajax({url:e,type:n,dataType:o,data:r,success:i})}}),b.extend({active:0,lastModified:{},etag:{},ajaxSettings:{url:yn,type:"GET",isLocal:Nn.test(mn[1]),global:!0,processData:!0,async:!0,contentType:"application/x-www-form-urlencoded; charset=UTF-8",accepts:{"*":Dn,text:"text/plain",html:"text/html",xml:"application/xml, text/xml",json:"application/json, text/javascript"},contents:{xml:/xml/,html:/html/,json:/json/},responseFields:{xml:"responseXML",text:"responseText"},converters:{"* text":e.String,"text html":!0,"text json":b.parseJSON,"text xml":b.parseXML},flatOptions:{url:!0,context:!0}},ajaxSetup:function(e,t){return t?Mn(Mn(e,b.ajaxSettings),t):Mn(b.ajaxSettings,e)},ajaxPrefilter:Hn(An),ajaxTransport:Hn(jn),ajax:function(e,n){"object"==typeof e&&(n=e,e=t),n=n||{};var r,i,o,a,s,u,l,c,p=b.ajaxSetup({},n),f=p.context||p,d=p.context&&(f.nodeType||f.jquery)?b(f):b.event,h=b.Deferred(),g=b.Callbacks("once memory"),m=p.statusCode||{},y={},v={},x=0,T="canceled",N={readyState:0,getResponseHeader:function(e){var t;if(2===x){if(!c){c={};while(t=Tn.exec(a))c[t[1].toLowerCase()]=t[2]}t=c[e.toLowerCase()]}return null==t?null:t},getAllResponseHeaders:function(){return 2===x?a:null},setRequestHeader:function(e,t){var n=e.toLowerCase();return x||(e=v[n]=v[n]||e,y[e]=t),this},overrideMimeType:function(e){return x||(p.mimeType=e),this},statusCode:function(e){var t;if(e)if(2>x)for(t in e)m[t]=[m[t],e[t]];else N.always(e[N.status]);return this},abort:function(e){var t=e||T;return l&&l.abort(t),k(0,t),this}};if(h.promise(N).complete=g.add,N.success=N.done,N.error=N.fail,p.url=((e||p.url||yn)+"").replace(xn,"").replace(kn,mn[1]+"//"),p.type=n.method||n.type||p.method||p.type,p.dataTypes=b.trim(p.dataType||"*").toLowerCase().match(w)||[""],null==p.crossDomain&&(r=En.exec(p.url.toLowerCase()),p.crossDomain=!(!r||r[1]===mn[1]&&r[2]===mn[2]&&(r[3]||("http:"===r[1]?80:443))==(mn[3]||("http:"===mn[1]?80:443)))),p.data&&p.processData&&"string"!=typeof p.data&&(p.data=b.param(p.data,p.traditional)),qn(An,p,n,N),2===x)return N;u=p.global,u&&0===b.active++&&b.event.trigger("ajaxStart"),p.type=p.type.toUpperCase(),p.hasContent=!Cn.test(p.type),o=p.url,p.hasContent||(p.data&&(o=p.url+=(bn.test(o)?"&":"?")+p.data,delete p.data),p.cache===!1&&(p.url=wn.test(o)?o.replace(wn,"$1_="+vn++):o+(bn.test(o)?"&":"?")+"_="+vn++)),p.ifModified&&(b.lastModified[o]&&N.setRequestHeader("If-Modified-Since",b.lastModified[o]),b.etag[o]&&N.setRequestHeader("If-None-Match",b.etag[o])),(p.data&&p.hasContent&&p.contentType!==!1||n.contentType)&&N.setRequestHeader("Content-Type",p.contentType),N.setRequestHeader("Accept",p.dataTypes[0]&&p.accepts[p.dataTypes[0]]?p.accepts[p.dataTypes[0]]+("*"!==p.dataTypes[0]?", "+Dn+"; q=0.01":""):p.accepts["*"]);for(i in p.headers)N.setRequestHeader(i,p.headers[i]);if(p.beforeSend&&(p.beforeSend.call(f,N,p)===!1||2===x))return N.abort();T="abort";for(i in{success:1,error:1,complete:1})N[i](p[i]);if(l=qn(jn,p,n,N)){N.readyState=1,u&&d.trigger("ajaxSend",[N,p]),p.async&&p.timeout>0&&(s=setTimeout(function(){N.abort("timeout")},p.timeout));try{x=1,l.send(y,k)}catch(C){if(!(2>x))throw C;k(-1,C)}}else k(-1,"No Transport");function k(e,n,r,i){var c,y,v,w,T,C=n;2!==x&&(x=2,s&&clearTimeout(s),l=t,a=i||"",N.readyState=e>0?4:0,r&&(w=_n(p,N,r)),e>=200&&300>e||304===e?(p.ifModified&&(T=N.getResponseHeader("Last-Modified"),T&&(b.lastModified[o]=T),T=N.getResponseHeader("etag"),T&&(b.etag[o]=T)),204===e?(c=!0,C="nocontent"):304===e?(c=!0,C="notmodified"):(c=Fn(p,w),C=c.state,y=c.data,v=c.error,c=!v)):(v=C,(e||!C)&&(C="error",0>e&&(e=0))),N.status=e,N.statusText=(n||C)+"",c?h.resolveWith(f,[y,C,N]):h.rejectWith(f,[N,C,v]),N.statusCode(m),m=t,u&&d.trigger(c?"ajaxSuccess":"ajaxError",[N,p,c?y:v]),g.fireWith(f,[N,C]),u&&(d.trigger("ajaxComplete",[N,p]),--b.active||b.event.trigger("ajaxStop")))}return N},getScript:function(e,n){return b.get(e,t,n,"script")},getJSON:function(e,t,n){return b.get(e,t,n,"json")}});function _n(e,n,r){var i,o,a,s,u=e.contents,l=e.dataTypes,c=e.responseFields;for(s in c)s in r&&(n[c[s]]=r[s]);while("*"===l[0])l.shift(),o===t&&(o=e.mimeType||n.getResponseHeader("Content-Type"));if(o)for(s in u)if(u[s]&&u[s].test(o)){l.unshift(s);break}if(l[0]in r)a=l[0];else{for(s in r){if(!l[0]||e.converters[s+" "+l[0]]){a=s;break}i||(i=s)}a=a||i}return a?(a!==l[0]&&l.unshift(a),r[a]):t}function Fn(e,t){var n,r,i,o,a={},s=0,u=e.dataTypes.slice(),l=u[0];if(e.dataFilter&&(t=e.dataFilter(t,e.dataType)),u[1])for(i in e.converters)a[i.toLowerCase()]=e.converters[i];for(;r=u[++s];)if("*"!==r){if("*"!==l&&l!==r){if(i=a[l+" "+r]||a["* "+r],!i)for(n in a)if(o=n.split(" "),o[1]===r&&(i=a[l+" "+o[0]]||a["* "+o[0]])){i===!0?i=a[n]:a[n]!==!0&&(r=o[0],u.splice(s--,0,r));break}if(i!==!0)if(i&&e["throws"])t=i(t);else try{t=i(t)}catch(c){return{state:"parsererror",error:i?c:"No conversion from "+l+" to "+r}}}l=r}return{state:"success",data:t}}b.ajaxSetup({accepts:{script:"text/javascript, application/javascript, application/ecmascript, application/x-ecmascript"},contents:{script:/(?:java|ecma)script/},converters:{"text script":function(e){return b.globalEval(e),e}}}),b.ajaxPrefilter("script",function(e){e.cache===t&&(e.cache=!1),e.crossDomain&&(e.type="GET",e.global=!1)}),b.ajaxTransport("script",function(e){if(e.crossDomain){var n,r=o.head||b("head")[0]||o.documentElement;return{send:function(t,i){n=o.createElement("script"),n.async=!0,e.scriptCharset&&(n.charset=e.scriptCharset),n.src=e.url,n.onload=n.onreadystatechange=function(e,t){(t||!n.readyState||/loaded|complete/.test(n.readyState))&&(n.onload=n.onreadystatechange=null,n.parentNode&&n.parentNode.removeChild(n),n=null,t||i(200,"success"))},r.insertBefore(n,r.firstChild)},abort:function(){n&&n.onload(t,!0)}}}});var On=[],Bn=/(=)\?(?=&|$)|\?\?/;b.ajaxSetup({jsonp:"callback",jsonpCallback:function(){var e=On.pop()||b.expando+"_"+vn++;return this[e]=!0,e}}),b.ajaxPrefilter("json jsonp",function(n,r,i){var o,a,s,u=n.jsonp!==!1&&(Bn.test(n.url)?"url":"string"==typeof n.data&&!(n.contentType||"").indexOf("application/x-www-form-urlencoded")&&Bn.test(n.data)&&"data");return u||"jsonp"===n.dataTypes[0]?(o=n.jsonpCallback=b.isFunction(n.jsonpCallback)?n.jsonpCallback():n.jsonpCallback,u?n[u]=n[u].replace(Bn,"$1"+o):n.jsonp!==!1&&(n.url+=(bn.test(n.url)?"&":"?")+n.jsonp+"="+o),n.converters["script json"]=function(){return s||b.error(o+" was not called"),s[0]},n.dataTypes[0]="json",a=e[o],e[o]=function(){s=arguments},i.always(function(){e[o]=a,n[o]&&(n.jsonpCallback=r.jsonpCallback,On.push(o)),s&&b.isFunction(a)&&a(s[0]),s=a=t}),"script"):t});var Pn,Rn,Wn=0,$n=e.ActiveXObject&&function(){var e;for(e in Pn)Pn[e](t,!0)};function In(){try{return new e.XMLHttpRequest}catch(t){}}function zn(){try{return new e.ActiveXObject("Microsoft.XMLHTTP")}catch(t){}}b.ajaxSettings.xhr=e.ActiveXObject?function(){return!this.isLocal&&In()||zn()}:In,Rn=b.ajaxSettings.xhr(),b.support.cors=!!Rn&&"withCredentials"in Rn,Rn=b.support.ajax=!!Rn,Rn&&b.ajaxTransport(function(n){if(!n.crossDomain||b.support.cors){var r;return{send:function(i,o){var a,s,u=n.xhr();if(n.username?u.open(n.type,n.url,n.async,n.username,n.password):u.open(n.type,n.url,n.async),n.xhrFields)for(s in n.xhrFields)u[s]=n.xhrFields[s];n.mimeType&&u.overrideMimeType&&u.overrideMimeType(n.mimeType),n.crossDomain||i["X-Requested-With"]||(i["X-Requested-With"]="XMLHttpRequest");try{for(s in i)u.setRequestHeader(s,i[s])}catch(l){}u.send(n.hasContent&&n.data||null),r=function(e,i){var s,l,c,p;try{if(r&&(i||4===u.readyState))if(r=t,a&&(u.onreadystatechange=b.noop,$n&&delete Pn[a]),i)4!==u.readyState&&u.abort();else{p={},s=u.status,l=u.getAllResponseHeaders(),"string"==typeof u.responseText&&(p.text=u.responseText);try{c=u.statusText}catch(f){c=""}s||!n.isLocal||n.crossDomain?1223===s&&(s=204):s=p.text?200:404}}catch(d){i||o(-1,d)}p&&o(s,c,p,l)},n.async?4===u.readyState?setTimeout(r):(a=++Wn,$n&&(Pn||(Pn={},b(e).unload($n)),Pn[a]=r),u.onreadystatechange=r):r()},abort:function(){r&&r(t,!0)}}}});var Xn,Un,Vn=/^(?:toggle|show|hide)$/,Yn=RegExp("^(?:([+-])=|)("+x+")([a-z%]*)$","i"),Jn=/queueHooks$/,Gn=[nr],Qn={"*":[function(e,t){var n,r,i=this.createTween(e,t),o=Yn.exec(t),a=i.cur(),s=+a||0,u=1,l=20;if(o){if(n=+o[2],r=o[3]||(b.cssNumber[e]?"":"px"),"px"!==r&&s){s=b.css(i.elem,e,!0)||n||1;do u=u||".5",s/=u,b.style(i.elem,e,s+r);while(u!==(u=i.cur()/a)&&1!==u&&--l)}i.unit=r,i.start=s,i.end=o[1]?s+(o[1]+1)*n:n}return i}]};function Kn(){return setTimeout(function(){Xn=t}),Xn=b.now()}function Zn(e,t){b.each(t,function(t,n){var r=(Qn[t]||[]).concat(Qn["*"]),i=0,o=r.length;for(;o>i;i++)if(r[i].call(e,t,n))return})}function er(e,t,n){var r,i,o=0,a=Gn.length,s=b.Deferred().always(function(){delete u.elem}),u=function(){if(i)return!1;var t=Xn||Kn(),n=Math.max(0,l.startTime+l.duration-t),r=n/l.duration||0,o=1-r,a=0,u=l.tweens.length;for(;u>a;a++)l.tweens[a].run(o);return s.notifyWith(e,[l,o,n]),1>o&&u?n:(s.resolveWith(e,[l]),!1)},l=s.promise({elem:e,props:b.extend({},t),opts:b.extend(!0,{specialEasing:{}},n),originalProperties:t,originalOptions:n,startTime:Xn||Kn(),duration:n.duration,tweens:[],createTween:function(t,n){var r=b.Tween(e,l.opts,t,n,l.opts.specialEasing[t]||l.opts.easing);return l.tweens.push(r),r},stop:function(t){var n=0,r=t?l.tweens.length:0;if(i)return this;for(i=!0;r>n;n++)l.tweens[n].run(1);return t?s.resolveWith(e,[l,t]):s.rejectWith(e,[l,t]),this}}),c=l.props;for(tr(c,l.opts.specialEasing);a>o;o++)if(r=Gn[o].call(l,e,c,l.opts))return r;return Zn(l,c),b.isFunction(l.opts.start)&&l.opts.start.call(e,l),b.fx.timer(b.extend(u,{elem:e,anim:l,queue:l.opts.queue})),l.progress(l.opts.progress).done(l.opts.done,l.opts.complete).fail(l.opts.fail).always(l.opts.always)}function tr(e,t){var n,r,i,o,a;for(i in e)if(r=b.camelCase(i),o=t[r],n=e[i],b.isArray(n)&&(o=n[1],n=e[i]=n[0]),i!==r&&(e[r]=n,delete e[i]),a=b.cssHooks[r],a&&"expand"in a){n=a.expand(n),delete e[r];for(i in n)i in e||(e[i]=n[i],t[i]=o)}else t[r]=o}b.Animation=b.extend(er,{tweener:function(e,t){b.isFunction(e)?(t=e,e=["*"]):e=e.split(" ");var n,r=0,i=e.length;for(;i>r;r++)n=e[r],Qn[n]=Qn[n]||[],Qn[n].unshift(t)},prefilter:function(e,t){t?Gn.unshift(e):Gn.push(e)}});function nr(e,t,n){var r,i,o,a,s,u,l,c,p,f=this,d=e.style,h={},g=[],m=e.nodeType&&nn(e);n.queue||(c=b._queueHooks(e,"fx"),null==c.unqueued&&(c.unqueued=0,p=c.empty.fire,c.empty.fire=function(){c.unqueued||p()}),c.unqueued++,f.always(function(){f.always(function(){c.unqueued--,b.queue(e,"fx").length||c.empty.fire()})})),1===e.nodeType&&("height"in t||"width"in t)&&(n.overflow=[d.overflow,d.overflowX,d.overflowY],"inline"===b.css(e,"display")&&"none"===b.css(e,"float")&&(b.support.inlineBlockNeedsLayout&&"inline"!==un(e.nodeName)?d.zoom=1:d.display="inline-block")),n.overflow&&(d.overflow="hidden",b.support.shrinkWrapBlocks||f.always(function(){d.overflow=n.overflow[0],d.overflowX=n.overflow[1],d.overflowY=n.overflow[2]}));for(i in t)if(a=t[i],Vn.exec(a)){if(delete t[i],u=u||"toggle"===a,a===(m?"hide":"show"))continue;g.push(i)}if(o=g.length){s=b._data(e,"fxshow")||b._data(e,"fxshow",{}),"hidden"in s&&(m=s.hidden),u&&(s.hidden=!m),m?b(e).show():f.done(function(){b(e).hide()}),f.done(function(){var t;b._removeData(e,"fxshow");for(t in h)b.style(e,t,h[t])});for(i=0;o>i;i++)r=g[i],l=f.createTween(r,m?s[r]:0),h[r]=s[r]||b.style(e,r),r in s||(s[r]=l.start,m&&(l.end=l.start,l.start="width"===r||"height"===r?1:0))}}function rr(e,t,n,r,i){return new rr.prototype.init(e,t,n,r,i)}b.Tween=rr,rr.prototype={constructor:rr,init:function(e,t,n,r,i,o){this.elem=e,this.prop=n,this.easing=i||"swing",this.options=t,this.start=this.now=this.cur(),this.end=r,this.unit=o||(b.cssNumber[n]?"":"px")},cur:function(){var e=rr.propHooks[this.prop];return e&&e.get?e.get(this):rr.propHooks._default.get(this)},run:function(e){var t,n=rr.propHooks[this.prop];return this.pos=t=this.options.duration?b.easing[this.easing](e,this.options.duration*e,0,1,this.options.duration):e,this.now=(this.end-this.start)*t+this.start,this.options.step&&this.options.step.call(this.elem,this.now,this),n&&n.set?n.set(this):rr.propHooks._default.set(this),this}},rr.prototype.init.prototype=rr.prototype,rr.propHooks={_default:{get:function(e){var t;return null==e.elem[e.prop]||e.elem.style&&null!=e.elem.style[e.prop]?(t=b.css(e.elem,e.prop,""),t&&"auto"!==t?t:0):e.elem[e.prop]},set:function(e){b.fx.step[e.prop]?b.fx.step[e.prop](e):e.elem.style&&(null!=e.elem.style[b.cssProps[e.prop]]||b.cssHooks[e.prop])?b.style(e.elem,e.prop,e.now+e.unit):e.elem[e.prop]=e.now}}},rr.propHooks.scrollTop=rr.propHooks.scrollLeft={set:function(e){e.elem.nodeType&&e.elem.parentNode&&(e.elem[e.prop]=e.now)}},b.each(["toggle","show","hide"],function(e,t){var n=b.fn[t];b.fn[t]=function(e,r,i){return null==e||"boolean"==typeof e?n.apply(this,arguments):this.animate(ir(t,!0),e,r,i)}}),b.fn.extend({fadeTo:function(e,t,n,r){return this.filter(nn).css("opacity",0).show().end().animate({opacity:t},e,n,r)},animate:function(e,t,n,r){var i=b.isEmptyObject(e),o=b.speed(t,n,r),a=function(){var t=er(this,b.extend({},e),o);a.finish=function(){t.stop(!0)},(i||b._data(this,"finish"))&&t.stop(!0)};return a.finish=a,i||o.queue===!1?this.each(a):this.queue(o.queue,a)},stop:function(e,n,r){var i=function(e){var t=e.stop;delete e.stop,t(r)};return"string"!=typeof e&&(r=n,n=e,e=t),n&&e!==!1&&this.queue(e||"fx",[]),this.each(function(){var t=!0,n=null!=e&&e+"queueHooks",o=b.timers,a=b._data(this);if(n)a[n]&&a[n].stop&&i(a[n]);else for(n in a)a[n]&&a[n].stop&&Jn.test(n)&&i(a[n]);for(n=o.length;n--;)o[n].elem!==this||null!=e&&o[n].queue!==e||(o[n].anim.stop(r),t=!1,o.splice(n,1));(t||!r)&&b.dequeue(this,e)})},finish:function(e){return e!==!1&&(e=e||"fx"),this.each(function(){var t,n=b._data(this),r=n[e+"queue"],i=n[e+"queueHooks"],o=b.timers,a=r?r.length:0;for(n.finish=!0,b.queue(this,e,[]),i&&i.cur&&i.cur.finish&&i.cur.finish.call(this),t=o.length;t--;)o[t].elem===this&&o[t].queue===e&&(o[t].anim.stop(!0),o.splice(t,1));for(t=0;a>t;t++)r[t]&&r[t].finish&&r[t].finish.call(this);delete n.finish})}});function ir(e,t){var n,r={height:e},i=0;for(t=t?1:0;4>i;i+=2-t)n=Zt[i],r["margin"+n]=r["padding"+n]=e;return t&&(r.opacity=r.width=e),r}b.each({slideDown:ir("show"),slideUp:ir("hide"),slideToggle:ir("toggle"),fadeIn:{opacity:"show"},fadeOut:{opacity:"hide"},fadeToggle:{opacity:"toggle"}},function(e,t){b.fn[e]=function(e,n,r){return this.animate(t,e,n,r)}}),b.speed=function(e,t,n){var r=e&&"object"==typeof e?b.extend({},e):{complete:n||!n&&t||b.isFunction(e)&&e,duration:e,easing:n&&t||t&&!b.isFunction(t)&&t};return r.duration=b.fx.off?0:"number"==typeof r.duration?r.duration:r.duration in b.fx.speeds?b.fx.speeds[r.duration]:b.fx.speeds._default,(null==r.queue||r.queue===!0)&&(r.queue="fx"),r.old=r.complete,r.complete=function(){b.isFunction(r.old)&&r.old.call(this),r.queue&&b.dequeue(this,r.queue)},r},b.easing={linear:function(e){return e},swing:function(e){return.5-Math.cos(e*Math.PI)/2}},b.timers=[],b.fx=rr.prototype.init,b.fx.tick=function(){var e,n=b.timers,r=0;for(Xn=b.now();n.length>r;r++)e=n[r],e()||n[r]!==e||n.splice(r--,1);n.length||b.fx.stop(),Xn=t},b.fx.timer=function(e){e()&&b.timers.push(e)&&b.fx.start()},b.fx.interval=13,b.fx.start=function(){Un||(Un=setInterval(b.fx.tick,b.fx.interval))},b.fx.stop=function(){clearInterval(Un),Un=null},b.fx.speeds={slow:600,fast:200,_default:400},b.fx.step={},b.expr&&b.expr.filters&&(b.expr.filters.animated=function(e){return b.grep(b.timers,function(t){return e===t.elem}).length}),b.fn.offset=function(e){if(arguments.length)return e===t?this:this.each(function(t){b.offset.setOffset(this,e,t)});var n,r,o={top:0,left:0},a=this[0],s=a&&a.ownerDocument;if(s)return n=s.documentElement,b.contains(n,a)?(typeof a.getBoundingClientRect!==i&&(o=a.getBoundingClientRect()),r=or(s),{top:o.top+(r.pageYOffset||n.scrollTop)-(n.clientTop||0),left:o.left+(r.pageXOffset||n.scrollLeft)-(n.clientLeft||0)}):o},b.offset={setOffset:function(e,t,n){var r=b.css(e,"position");"static"===r&&(e.style.position="relative");var i=b(e),o=i.offset(),a=b.css(e,"top"),s=b.css(e,"left"),u=("absolute"===r||"fixed"===r)&&b.inArray("auto",[a,s])>-1,l={},c={},p,f;u?(c=i.position(),p=c.top,f=c.left):(p=parseFloat(a)||0,f=parseFloat(s)||0),b.isFunction(t)&&(t=t.call(e,n,o)),null!=t.top&&(l.top=t.top-o.top+p),null!=t.left&&(l.left=t.left-o.left+f),"using"in t?t.using.call(e,l):i.css(l)}},b.fn.extend({position:function(){if(this[0]){var e,t,n={top:0,left:0},r=this[0];return"fixed"===b.css(r,"position")?t=r.getBoundingClientRect():(e=this.offsetParent(),t=this.offset(),b.nodeName(e[0],"html")||(n=e.offset()),n.top+=b.css(e[0],"borderTopWidth",!0),n.left+=b.css(e[0],"borderLeftWidth",!0)),{top:t.top-n.top-b.css(r,"marginTop",!0),left:t.left-n.left-b.css(r,"marginLeft",!0)}}},offsetParent:function(){return this.map(function(){var e=this.offsetParent||o.documentElement;while(e&&!b.nodeName(e,"html")&&"static"===b.css(e,"position"))e=e.offsetParent;return e||o.documentElement})}}),b.each({scrollLeft:"pageXOffset",scrollTop:"pageYOffset"},function(e,n){var r=/Y/.test(n);b.fn[e]=function(i){return b.access(this,function(e,i,o){var a=or(e);return o===t?a?n in a?a[n]:a.document.documentElement[i]:e[i]:(a?a.scrollTo(r?b(a).scrollLeft():o,r?o:b(a).scrollTop()):e[i]=o,t)},e,i,arguments.length,null)}});function or(e){return b.isWindow(e)?e:9===e.nodeType?e.defaultView||e.parentWindow:!1}b.each({Height:"height",Width:"width"},function(e,n){b.each({padding:"inner"+e,content:n,"":"outer"+e},function(r,i){b.fn[i]=function(i,o){var a=arguments.length&&(r||"boolean"!=typeof i),s=r||(i===!0||o===!0?"margin":"border");return b.access(this,function(n,r,i){var o;return b.isWindow(n)?n.document.documentElement["client"+e]:9===n.nodeType?(o=n.documentElement,Math.max(n.body["scroll"+e],o["scroll"+e],n.body["offset"+e],o["offset"+e],o["client"+e])):i===t?b.css(n,r,s):b.style(n,r,i,s)},n,a?i:t,a,null)}})}),e.jQuery=e.$=b,"function"==typeof define&&define.amd&&define.amd.jQuery&&define("jquery",[],function(){return b})})(window);

// Snap this specific version of jQuery into H5P. jQuery.noConflict will
// revert the globals to what they were before this file was loaded.
var H5P = H5P || {};

/**
 * jQuery v1.9.1
 *
 * @member
 */
H5P.jQuery = jQuery.noConflict(true);
;
/*jshint multistr: true */
// TODO: Should we split up the generic parts needed by the editor(and others), and the parts needed to "run" H5Ps?
 
/** @namespace */
var H5P = H5P || {};

/**
 * Tells us if we're inside of an iframe.
 * @member {boolean}
 */

H5P.storeCurrentState = function()
{
	 for (var i = 0; i < H5P.instances.length; i++) {
         var instance = H5P.instances[i];
         if (instance.getCurrentState instanceof Function ||
             typeof instance.getCurrentState === 'function') {
           var state = instance.getCurrentState();
           if (state !== undefined) {
             // Async is not used to prevent the request from being cancelled.
             H5P.setUserData(instance.contentId, 'state', state, {deleteOnChange: true, async: false});
           }
         }
       }
}
H5P.isFramed = (window.self !== window.parent);
H5P.learnerLaunch = false;
H5P.recordCloned = false;
/**
 * jQuery instance of current window.
 * @member {H5P.jQuery}
 */
H5P.$window = H5P.jQuery(window);

/** 
 * List over H5P instances on the current page.
 * @member {Array}
 */
H5P.instances = [];

// Detect if we support fullscreen, and what prefix to use.
if (document.documentElement.requestFullScreen) {
  /**
   * Browser prefix to use when entering fullscreen mode.
   * undefined means no fullscreen support.
   * @member {string}
   */
  H5P.fullScreenBrowserPrefix = '';
}
else if (document.documentElement.webkitRequestFullScreen) {
  H5P.safariBrowser = navigator.userAgent.match(/Version\/(\d)/);
  H5P.safariBrowser = (H5P.safariBrowser === null ? 0 : parseInt(H5P.safariBrowser[1]));

  // Do not allow fullscreen for safari < 7.
  if (H5P.safariBrowser === 0 || H5P.safariBrowser > 6) {
    H5P.fullScreenBrowserPrefix = 'webkit';
  }
}
else if (document.documentElement.mozRequestFullScreen) {
  H5P.fullScreenBrowserPrefix = 'moz';
}
else if (document.documentElement.msRequestFullscreen) {
  H5P.fullScreenBrowserPrefix = 'ms';
}

/** @const {number} */
H5P.DISABLE_NONE = 0;

/** @const {number} */
H5P.DISABLE_FRAME = 1;

/** @const {number} */
H5P.DISABLE_DOWNLOAD = 2;

/** @const {number} */
H5P.DISABLE_EMBED = 4;

/** @const {number} */
H5P.DISABLE_COPYRIGHT = 8;

/** @const {number} */
H5P.DISABLE_ABOUT = 16;

/**
 * Keep track of when the H5Ps where started.
 *
 * @type {Object[]}
 */
H5P.opened = {};

/**
 * Initialize H5P content.
 * Scans for ".h5p-content" in the document and initializes H5P instances where found.
 *
 * @param {Object} target DOM Element
 */
H5P.init = function (target) {
  // Useful jQuery object.
  if (H5P.$body === undefined) {
    H5P.$body = H5P.jQuery(document.body);
  }

  // Determine if we can use full screen
  if (H5P.canHasFullScreen === undefined) {
    /**
     * Use this variable to check if fullscreen is supported. Fullscreen can be
     * restricted when embedding since not all browsers support the native
     * fullscreen, and the semi-fullscreen solution doesn't work when embedded.
     * @type {boolean}
     */
     
     //h5pcustomize
    H5P.canHasFullScreen = (H5P.isFramed && H5P.externalEmbed !== false) ? ((document.fullscreenEnabled || document.webkitFullscreenEnabled || document.mozFullScreenEnabled) ? true : false) : true;
    // We should consider document.msFullscreenEnabled when they get their
    // element sizing corrected. Ref. https://connect.microsoft.com/IE/feedback/details/838286/ie-11-incorrectly-reports-dom-element-sizes-in-fullscreen-mode-when-fullscreened-element-is-within-an-iframe
  }

  // H5Ps added in normal DIV.
  var $containers = H5P.jQuery('.h5p-content:not(.h5p-initialized)', target).each(function () {
    var $element = H5P.jQuery(this).addClass('h5p-initialized');
    var $container = H5P.jQuery('<div class="h5p-container"></div>').appendTo($element);
    var contentId = $element.data('content-id');
    var contentData = H5PIntegration.contents['cid-' + contentId];
    if (contentData === undefined) {
      return H5P.error('No data for content id ' + contentId + '. Perhaps the library is gone?');
    }
    var library = {
      library: contentData.library,
      params: JSON.parse(contentData.jsonContent)
    };
    console.log("library:",library);
    H5P.getUserData(contentId, 'state', function (err, previousState) {
      if (previousState) {
        library.userDatas = {
          state: previousState
        };
      }
      else if (previousState === null && H5PIntegration.saveFreq) {
        // Content has been reset. Display dialog.
        delete contentData.contentUserData;
        var dialog = new H5P.Dialog('content-user-data-reset', 'Data Reset', '<p>' + H5P.t('contentChanged') + '</p><p>' + H5P.t('startingOver') + '</p><div class="h5p-dialog-ok-button" tabIndex="0" role="button">OK</div>', $container);
        H5P.jQuery(dialog).on('dialog-opened', function (event, $dialog) {

          var closeDialog = function (event) {
            if (event.type === 'click' || event.which === 32) {
              dialog.close();
              H5P.deleteUserData(contentId, 'state', 0);
            }
          };

          $dialog.find('.h5p-dialog-ok-button').click(closeDialog).keypress(closeDialog);
        });
        dialog.open();
      }
    });

    // Create new instance.
    var instance = H5P.newRunnable(library, contentId, $container, true, {standalone: true});

    // Check if we should add and display a fullscreen button for this H5P.
    if (contentData.fullScreen == 1 && H5P.canHasFullScreen) {
      H5P.jQuery('<div class="h5p-content-controls"><div role="button" tabindex="0" class="h5p-enable-fullscreen" title="' + H5P.t('fullscreen') + '"></div></div>').prependTo($container).children().click(function () {
        H5P.fullScreen($container, instance);
      });
    }

    // Create action bar
    var $actions = H5P.jQuery('<ul class="h5p-actions"></ul>');

    /**
     * Helper for creating action bar buttons.
     *
     * @private
     * @param {string} type
     * @param {function} handler
     * @param {string} customClass Instead of type class
     */
    var addActionButton = function (type, handler, customClass) {
      H5P.jQuery('<li/>', {
        'class': 'h5p-button h5p-' + (customClass ? customClass : type),
        role: 'button',
        tabindex: 0,
        title: H5P.t(type + 'Description'),
        html: H5P.t(type),
        on: {
          click: handler,
          keypress: function (e) {
            if (e.which === 32) {
              handler();
              e.preventDefault(); // (since return false will block other inputs)
            }
          }
        },
        appendTo: $actions
      });
    };

    // Register action bar buttons
    if (!(contentData.disable & H5P.DISABLE_DOWNLOAD)) {
      // Add export button
      addActionButton('download', function () {
        // Use button for download to avoid people linking directly to the .h5p
        window.location.href = contentData.exportUrl;
      }, 'export');
    }
    if (!(contentData.disable & H5P.DISABLE_COPYRIGHT)) {
      var copyright = H5P.getCopyrights(instance, library.params, contentId);

      if (copyright) {
        // Add copyright dialog button
        addActionButton('copyrights', function () {
          // Open dialog with copyright information
          var dialog = new H5P.Dialog('copyrights', H5P.t('copyrightInformation'), copyright, $container);
          dialog.open();
        });
      }
    }
    if (!(contentData.disable & H5P.DISABLE_EMBED)) {
      // Add embed button
      addActionButton('embed', function () {
        // Open dialog with embed information
        H5P.openEmbedDialog($actions, contentData.embedCode, contentData.resizeCode, {
          width: $element.width(),
          height: $element.height()
        });
      });
    }

    if (!(contentData.disable & H5P.DISABLE_ABOUT)) {
      // Add about H5P button icon
      H5P.jQuery('<li><a class="h5p-link" href="http://h5p.org" target="_blank" title="' + H5P.t('h5pDescription') + '"></a></li>').appendTo($actions);
    }

    // Insert action bar if it has any content
    if (!(contentData.disable & H5P.DISABLE_FRAME) && $actions.children().length) {
      $actions.insertAfter($container);
      $element.addClass('h5p-frame');
    }
    else {
      $element.addClass('h5p-no-frame');
    }

    // Keep track of when we started
    H5P.opened[contentId] = new Date();

    // Handle events when the user finishes the content. Useful for logging exercise results.
    H5P.on(instance, 'finish', function (event) {
      if (event.data !== undefined) {
        H5P.setFinished(contentId, event.data.score, event.data.maxScore, event.data.time);
      }
    });

    // Listen for xAPI events.
    H5P.on(instance, 'xAPI', H5P.xAPICompletedListener);

    // Auto save current state if supported
    if (H5PIntegration.saveFreq !== false && (
        instance.getCurrentState instanceof Function ||
        typeof instance.getCurrentState === 'function')) {

      var saveTimer, save = function () {
        var state = instance.getCurrentState();
        if (state !== undefined) {
          H5P.setUserData(contentId, 'state', state, {deleteOnChange: true});
        }
        if (H5PIntegration.saveFreq) {
          // Continue autosave
          saveTimer = setTimeout(save, H5PIntegration.saveFreq * 1000);
        }
      };

      if (H5PIntegration.saveFreq) {
        // Start autosave
        saveTimer = setTimeout(save, H5PIntegration.saveFreq * 1000);
      }

      // xAPI events will schedule a save in three seconds.
      H5P.on(instance, 'xAPI', function (event) {
        var verb = event.getVerb();
        if (verb === 'completed' || verb === 'progressed') {
          clearTimeout(saveTimer);
          saveTimer = setTimeout(save, 3000);
        }
      });
    }

    if (H5P.isFramed) {
      var resizeDelay;
      if (H5P.externalEmbed === false) {
        // Internal embed
        // Make it possible to resize the iframe when the content changes size. This way we get no scrollbars.
        var iframe = window.parent.document.getElementById('h5p-iframe-' + contentId);
        var resizeIframe = function () {
          if (window.parent.H5P.isFullscreen) {
            return; // Skip if full screen.
          }

          // Retain parent size to avoid jumping/scrolling
          var parentHeight = iframe.parentElement.style.height;
          iframe.parentElement.style.height = iframe.parentElement.clientHeight + 'px';

          // Reset iframe height, in case content has shrinked.
          iframe.style.height = '1px';

          // Resize iframe so all content is visible.
          iframe.style.height = (iframe.contentDocument.body.scrollHeight) + 'px';

          // Free parent
          iframe.parentElement.style.height = parentHeight;
        };

        H5P.on(instance, 'resize', function () {
          // Use a delay to make sure iframe is resized to the correct size.
          clearTimeout(resizeDelay);
          resizeDelay = setTimeout(function () {
            resizeIframe();
          }, 1);
        });
      }
      else if (H5P.communicator) {
        // External embed
        var parentIsFriendly = false;

        // Handle that the resizer is loaded after the iframe
        H5P.communicator.on('ready', function () {
          H5P.communicator.send('hello');
        });

        // Handle hello message from our parent window
        H5P.communicator.on('hello', function () {
          // Initial setup/handshake is done
          parentIsFriendly = true;

          // Make iframe responsive
          document.body.style.height = 'auto';

          // Hide scrollbars for correct size
          document.body.style.overflow = 'hidden';

          // Content need to be resized to fit the new iframe size
          H5P.trigger(instance, 'resize');
        });

        // When resize has been prepared tell parent window to resize
        H5P.communicator.on('resizePrepared', function (data) {
          H5P.communicator.send('resize', {
            scrollHeight: document.body.scrollHeight
          });
        });

        H5P.communicator.on('resize', function () {
          H5P.trigger(instance, 'resize');
        });

        H5P.on(instance, 'resize', function () {
          if (H5P.isFullscreen) {
            return; // Skip iframe resize
          }

          // Use a delay to make sure iframe is resized to the correct size.
          clearTimeout(resizeDelay);
          resizeDelay = setTimeout(function () {
            // Only resize if the iframe can be resized
            if (parentIsFriendly) {
              H5P.communicator.send('prepareResize', {
                scrollHeight: document.body.scrollHeight,
                clientHeight: document.body.clientHeight
              });
            }
            else {
              H5P.communicator.send('hello');
            }
          }, 0);
        });
      }
    }

    if (!H5P.isFramed || H5P.externalEmbed === false) {
      // Resize everything when window is resized.
      H5P.jQuery(window.parent).resize(function () {
        if (window.parent.H5P.isFullscreen) {
          // Use timeout to avoid bug in certain browsers when exiting fullscreen. Some browser will trigger resize before the fullscreenchange event.
          H5P.trigger(instance, 'resize');
        }
        else {
          H5P.trigger(instance, 'resize');
        }
      });
    }

    H5P.instances.push(instance);

    // Resize content.
    H5P.trigger(instance, 'resize');
  });

  // Insert H5Ps that should be in iframes.
  H5P.jQuery('iframe.h5p-iframe:not(.h5p-initialized)', target).each(function () {
    var contentId = H5P.jQuery(this).addClass('h5p-initialized').data('content-id');
    this.contentDocument.open();
    this.contentDocument.write('<!doctype html><html class="h5p-iframe"><head>' + H5P.getHeadTags(contentId) + '</head><body><div class="h5p-content" data-content-id="' + contentId + '"/></body></html>');
    this.contentDocument.close();
  });
};

/**
 * Loop through assets for iframe content and create a set of tags for head.
 *
 * @private
 * @param {number} contentId
 * @returns {string} HTML
 */
H5P.getHeadTags = function (contentId) {
  var createStyleTags = function (styles) {
    var tags = '';
    for (var i = 0; i < styles.length; i++) {
      tags += '<link rel="stylesheet" href="' + styles[i] + '">';
    }
    return tags;
  };

  var createScriptTags = function (scripts) {
    var tags = '';
    for (var i = 0; i < scripts.length; i++) {
      tags += '<script src="' + scripts[i] + '"></script>';
    }
    return tags;
  };

  return '<base target="_parent">' +
         createStyleTags(H5PIntegration.core.styles) +
         createStyleTags(H5PIntegration.contents['cid-' + contentId].styles) +
         createScriptTags(H5PIntegration.core.scripts) +
         createScriptTags(H5PIntegration.contents['cid-' + contentId].scripts) +
         '<script>H5PIntegration = window.parent.H5PIntegration; var H5P = H5P || {}; H5P.externalEmbed = false;</script>';
};

/**
 * When embedded the communicator helps talk to the parent page.
 *
 * @type {Communicator}
 */
H5P.communicator = (function () {
  /**
   * @class
   * @private
   */
  function Communicator() {
    var self = this;

    // Maps actions to functions
    var actionHandlers = {};

    // Register message listener
    window.addEventListener('message', function receiveMessage(event) {
      if (window.parent !== event.source || event.data.context !== 'h5p') {
        return; // Only handle messages from parent and in the correct context
      }

      if (actionHandlers[event.data.action] !== undefined) {
        actionHandlers[event.data.action](event.data);
      }
    } , false);


    /**
     * Register action listener.
     *
     * @param {string} action What you are waiting for
     * @param {function} handler What you want done
     */
    self.on = function (action, handler) {
      actionHandlers[action] = handler;
    };

    /**
     * Send a message to the all mighty father.
     *
     * @param {string} action
     * @param {Object} [data] payload
     */
    self.send = function (action, data) {
      if (data === undefined) {
        data = {};
      }
      data.context = 'h5p';
      data.action = action;

      // Parent origin can be anything
      window.parent.postMessage(data, '*');
    };
  }

  return (window.postMessage && window.addEventListener ? new Communicator() : undefined);
})();

/**
 * Enter fullscreen for the given H5P instance.
 *
 * @param {H5P.jQuery} $element Content container.
 * @param {Object} instance
 * @param {function} exitCallback Callback function called when user exits fullscreen.
 * @param {H5P.jQuery} $body For internal use. Gives the body of the iframe.
 */
H5P.fullScreen = function ($element, instance, exitCallback, body) {
	H5P.jQuery(".Fullsc").attr("style","max-height:1100px;max-width:1400px;min-height:1100px;min-width:1400px;");
	
  if (H5P.exitFullScreen !== undefined) {
    return; // Cannot enter new fullscreen until previous is over
  }

  if (H5P.isFramed && H5P.externalEmbed === false) {
    // Trigger resize on wrapper in parent window.
    window.parent.H5P.fullScreen($element, instance, exitCallback, H5P.$body.get());
    H5P.isFullscreen = true;
    H5P.exitFullScreen = function () {
      window.parent.H5P.exitFullScreen();
    };
    H5P.on(instance, 'exitFullScreen', function () {
    	H5P.jQuery(".Fullsc").attr("style","max-height:468px;max-width:860px;margin-top:0px");
      H5P.isFullscreen = false;
      H5P.exitFullScreen = undefined;
    });
    return;
  }

  var $container = $element;
  var $classes, $iframe;
  if (body === undefined)  {
    $body = H5P.$body;
  }
  else {
    // We're called from an iframe.
    $body = H5P.jQuery(body);
    $classes = $body.add($element.get());
    var iframeSelector = '#h5p-iframe-' + $element.parent().data('content-id');
    $iframe = H5P.jQuery(iframeSelector);
    $element = $iframe.parent(); // Put iframe wrapper in fullscreen, not container.
  }

  $classes = $element.add(H5P.$body).add($classes);

  /**
   * Prepare for resize by setting the correct styles.
   *
   * @private
   * @param {string} classes CSS
   */
  var before = function (classes) {
    $classes.addClass(classes);

    if ($iframe !== undefined) {
      // Set iframe to its default size(100%).
      $iframe.css('height', '');
    }
  };

  /**
   * Gets called when fullscreen mode has been entered.
   * Resizes and sets focus on content.
   *
   * @private
   */
  var entered = function () {
    // Do not rely on window resize events.
    H5P.trigger(instance, 'resize');
    H5P.trigger(instance, 'focus');
    H5P.trigger(instance, 'enterFullScreen');
  };

  /**
   * Gets called when fullscreen mode has been exited.
   * Resizes and sets focus on content.
   *
   * @private
   * @param {string} classes CSS
   */
  var done = function (classes) {
    H5P.isFullscreen = false;
    $classes.removeClass(classes);

    // Do not rely on window resize events.
    H5P.trigger(instance, 'resize');
    H5P.trigger(instance, 'focus');

    H5P.exitFullScreen = undefined;
    if (exitCallback !== undefined) {
    	exitCallback();
      
    }

    H5P.trigger(instance, 'exitFullScreen');
  };

  H5P.isFullscreen = true;
  if (H5P.fullScreenBrowserPrefix === undefined) {
    // Create semi fullscreen.

    if (H5P.isFramed) {
      return; // TODO: Should we support semi-fullscreen for IE9 & 10 ?
    }

    before('h5p-semi-fullscreen');
    var $disable = H5P.jQuery('<div role="button" tabindex="0" class="h5p-disable-fullscreen" title="' + H5P.t('disableFullscreen') + '"></div>').appendTo($container.find('.h5p-content-controls'));
    var keyup, disableSemiFullscreen = H5P.exitFullScreen = function () {
      if (prevViewportContent) {
        // Use content from the previous viewport tag
        h5pViewport.content = prevViewportContent;
      }
      else {
        // Remove viewport tag
        head.removeChild(h5pViewport);
      }
      $disable.remove();
      $body.unbind('keyup', keyup);
      done('h5p-semi-fullscreen');
    };
    keyup = function (event) {
      if (event.keyCode === 27) {
        disableSemiFullscreen();
      }
    };
    $disable.click(disableSemiFullscreen);
    $body.keyup(keyup);

    // Disable zoom
    var prevViewportContent, h5pViewport;
    var metaTags = document.getElementsByTagName('meta');
    for (var i = 0; i < metaTags.length; i++) {
      if (metaTags[i].name === 'viewport') {
        // Use the existing viewport tag
        h5pViewport = metaTags[i];
        prevViewportContent = h5pViewport.content;
        break;
      }
    }
    if (!prevViewportContent) {
      // Create a new viewport tag
      h5pViewport = document.createElement('meta');
      h5pViewport.name = 'viewport';
    }
    h5pViewport.content = 'width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=0';
    if (!prevViewportContent) {
      // Insert the new viewport tag
      var head = document.getElementsByTagName('head')[0];
      head.appendChild(h5pViewport);
    }

    entered();
  }
  else {
    // Create real fullscreen.

    before('h5p-fullscreen');
    var first, eventName = (H5P.fullScreenBrowserPrefix === 'ms' ? 'MSFullscreenChange' : H5P.fullScreenBrowserPrefix + 'fullscreenchange');
    document.addEventListener(eventName, function () {
      if (first === undefined) {
        // We are entering fullscreen mode
        first = false;
        entered();
        return;
      }

      // We are exiting fullscreen
      done('h5p-fullscreen');
      document.removeEventListener(eventName, arguments.callee, false);
    });

    if (H5P.fullScreenBrowserPrefix === '') {
      $element[0].requestFullScreen();
    }
    else {
      var method = (H5P.fullScreenBrowserPrefix === 'ms' ? 'msRequestFullscreen' : H5P.fullScreenBrowserPrefix + 'RequestFullScreen');
      var params = (H5P.fullScreenBrowserPrefix === 'webkit' && H5P.safariBrowser === 0 ? Element.ALLOW_KEYBOARD_INPUT : undefined);
      $element[0][method](params);
    }

    // Allows everone to exit
    H5P.exitFullScreen = function () {
      if (H5P.fullScreenBrowserPrefix === '') {
        document.exitFullscreen();
      }
      else if (H5P.fullScreenBrowserPrefix === 'moz') {
        document.mozCancelFullScreen();
      }
      else {
        document[H5P.fullScreenBrowserPrefix + 'ExitFullscreen']();
      }
    };
  }
};

/**
 * Find the path to the content files based on the id of the content.
 * Also identifies and returns absolute paths.
 *
 * @param {string} path
 *   Relative to content folder or absolute.
 * @param {number} contentId
 *   ID of the content requesting the path.
 * @returns {string}
 *   Complete URL to path.
 */
H5P.getPath = function (path, contentId) {
  var hasProtocol = function (path) {
    return path.match(/^[a-z0-9]+:\/\//i);
  };

  if (hasProtocol(path)) {
    return path;
  }

  var prefix;
  if (contentId !== undefined) {
  	//h5pcustomization - restructure
    //prefix = H5PIntegration.url + '/content/' + contentId;
    prefix = H5PIntegration.url + '/' + contentId;
  }
  else if (window.H5PEditor !== undefined) {
    prefix = H5PEditor.filesPath;
  }
  else {
    return;
  }

  if (!hasProtocol(prefix)) {
    // Use absolute urls
    prefix = window.location.protocol + "//" + window.location.host + prefix;
  }

  return prefix + '/' + path;
};

/**
 * THIS FUNCTION IS DEPRECATED, USE getPath INSTEAD
 * Will be remove march 2016.
 *
 * Find the path to the content files folder based on the id of the content
 *
 * @deprecated
 *   Will be removed march 2016.
 * @param contentId
 *   Id of the content requesting the path
 * @returns {string}
 *   URL
 */
H5P.getContentPath = function (contentId) {
  return H5PIntegration.url + '/content/' + contentId;
};

/**
 * Get library class constructor from H5P by classname.
 * Note that this class will only work for resolve "H5P.NameWithoutDot".
 * Also check out {@link H5P.newRunnable}
 *
 * Used from libraries to construct instances of other libraries' objects by name.
 *
 * @param {string} name Name of library
 * @returns {Object} Class constructor
 */
H5P.classFromName = function (name) {
  var arr = name.split(".");
  return this[arr[arr.length - 1]];
};

/**
 * A safe way of creating a new instance of a runnable H5P.
 *
 * @param {Object} library
 *   Library/action object form params.
 * @param {number} contentId
 *   Identifies the content.
 * @param {H5P.jQuery} [$attachTo]
 *   Element to attach the instance to.
 * @param {boolean} [skipResize]
 *   Skip triggering of the resize event after attaching.
 * @param {Object} [extras]
 *   Extra parameters for the H5P content constructor
 * @returns {Object}
 *   Instance.
 */
H5P.newRunnable = function (library, contentId, $attachTo, skipResize, extras) {
  var nameSplit, versionSplit, machineName;
  try {
    nameSplit = library.library.split(' ', 2);
    machineName = nameSplit[0];
    versionSplit = nameSplit[1].split('.', 2);
  }
  catch (err) {
    return H5P.error('Invalid library string: ' + library.library);
  }

  if ((library.params instanceof Object) !== true || (library.params instanceof Array) === true) {
    H5P.error('Invalid library params for: ' + library.library);
    return H5P.error(library.params);
  }

  // Find constructor function
  var constructor;
  try {
    nameSplit = nameSplit[0].split('.');
    constructor = window;
    for (var i = 0; i < nameSplit.length; i++) {
      constructor = constructor[nameSplit[i]];
    }
    if (typeof constructor !== 'function') {
      throw null;
    }
  }
  catch (err) {
    return H5P.error('Unable to find constructor for: ' + library.library);
  }

  if (extras === undefined) {
    extras = {};
  }
  if (library.subContentId) {
    extras.subContentId = library.subContentId;
  }

  if (library.userDatas && library.userDatas.state && H5PIntegration.saveFreq) {
    extras.previousState = library.userDatas.state;
  }

  // Makes all H5P libraries extend H5P.ContentType:
  var standalone = extras.standalone || false;
  // This order makes it possible for an H5P library to override H5P.ContentType functions!
  constructor.prototype = H5P.jQuery.extend({}, H5P.ContentType(standalone).prototype, constructor.prototype);

  var instance;
  // Some old library versions have their own custom third parameter.
  // Make sure we don't send them the extras.
  // (they will interpret it as something else)
  if (H5P.jQuery.inArray(library.library, ['H5P.CoursePresentation 1.0', 'H5P.CoursePresentation 1.1', 'H5P.CoursePresentation 1.2', 'H5P.CoursePresentation 1.3']) > -1) {
    instance = new constructor(library.params, contentId);
  }
  else {
    instance = new constructor(library.params, contentId, extras);
  }

  if (instance.$ === undefined) {
    instance.$ = H5P.jQuery(instance);
  }

  if (instance.contentId === undefined) {
    instance.contentId = contentId;
  }
  if (instance.subContentId === undefined && library.subContentId) {
    instance.subContentId = library.subContentId;
  }
  if (instance.parent === undefined && extras && extras.parent) {
    instance.parent = extras.parent;
  }
  if (instance.libraryInfo === undefined) {
    instance.libraryInfo = {
      versionedName: library.library,
      versionedNameNoSpaces: machineName + '-' + versionSplit[0] + '.' + versionSplit[1],
      machineName: machineName,
      majorVersion: versionSplit[0],
      minorVersion: versionSplit[1]
    };
  }

  if ($attachTo !== undefined) {
    $attachTo.toggleClass('h5p-standalone', standalone);
    instance.attach($attachTo);
    H5P.trigger(instance, 'domChanged', {
      '$target': $attachTo,
      'library': machineName,
      'key': 'newLibrary'
    }, {'bubbles': true, 'external': true});

    if (skipResize === undefined || !skipResize) {
      // Resize content.
      H5P.trigger(instance, 'resize');
    }
  }
  return instance;
};

/**
 * Used to print useful error messages. (to JavaScript error console)
 *
 * @param {*} err Error to print.
 */
H5P.error = function (err) {
  if (window.console !== undefined && console.error !== undefined) {
    console.error(err.stack ? err.stack : err);
  }
};

/**
 * Translate text strings.
 *
 * @param {string} key
 *   Translation identifier, may only contain a-zA-Z0-9. No spaces or special chars.
 * @param {Object} [vars]
 *   Data for placeholders.
 * @param {string} [ns]
 *   Translation namespace. Defaults to H5P.
 * @returns {string}
 *   Translated text
 */
H5P.t = function (key, vars, ns) {
  if (ns === undefined) {
    ns = 'H5P';
  }

  if (H5PIntegration.l10n[ns] === undefined) {
    return '[Missing translation namespace "' + ns + '"]';
  }

  if (H5PIntegration.l10n[ns][key] === undefined) {
    return '[Missing translation "' + key + '" in "' + ns + '"]';
  }

  var translation = H5PIntegration.l10n[ns][key];

  if (vars !== undefined) {
    // Replace placeholder with variables.
    for (var placeholder in vars) {
      translation = translation.replace(placeholder, vars[placeholder]);
    }
  }

  return translation;
};

/**
 * Creates a new popup dialog over the H5P content.
 *
 * @class
 * @param {string} name
 *   Used for html class.
 * @param {string} title
 *   Used for header.
 * @param {string} content
 *   Displayed inside the dialog.
 * @param {H5P.jQuery} $element
 *   Which DOM element the dialog should be inserted after.
 */
H5P.Dialog = function (name, title, content, $element) {
  var self = this;
  var $dialog = H5P.jQuery('<div class="h5p-popup-dialog h5p-' + name + '-dialog">\
                              <div class="h5p-inner">\
                                <h2>' + title + '</h2>\
                                <div class="h5p-scroll-content">' + content + '</div>\
                                <div class="h5p-close" role="button" tabindex="0" title="' + H5P.t('close') + '">\
                              </div>\
                            </div>')
    .insertAfter($element)
    .click(function () {
      self.close();
    })
    .children('.h5p-inner')
      .click(function () {
        return false;
      })
      .find('.h5p-close')
        .click(function () {
          self.close();
        })
        .end()
      .find('a')
        .click(function (e) {
          e.stopPropagation();
        })
      .end()
    .end();

  this.open = function () {
    setTimeout(function () {
      $dialog.addClass('h5p-open'); // Fade in
      // Triggering an event, in case something has to be done after dialog has been opened.
      H5P.jQuery(self).trigger('dialog-opened', [$dialog]);
    }, 1);
  };

  this.close = function () {
    $dialog.removeClass('h5p-open'); // Fade out
    setTimeout(function () {
      $dialog.remove();
    }, 100);
  };
};

/**
 * Gather copyright information for the given content.
 *
 * @param {Object} instance
 *   H5P instance to get copyright information for.
 * @param {Object} parameters
 *   Parameters of the content instance.
 * @param {number} contentId
 *   Identifies the H5P content
 * @returns {string} Copyright information.
 */
H5P.getCopyrights = function (instance, parameters, contentId) {
  var copyrights;

  if (instance.getCopyrights !== undefined) {
    try {
      // Use the instance's own copyright generator
      copyrights = instance.getCopyrights();
    }
    catch (err) {
      // Failed, prevent crashing page.
    }
  }

  if (copyrights === undefined) {
    // Create a generic flat copyright list
    copyrights = new H5P.ContentCopyrights();
    H5P.findCopyrights(copyrights, parameters, contentId);
  }

  if (copyrights !== undefined) {
    // Convert to string
    copyrights = copyrights.toString();
  }
  return copyrights;
};

/**
 * Gather a flat list of copyright information from the given parameters.
 *
 * @param {H5P.ContentCopyrights} info
 *   Used to collect all information in.
 * @param {(Object|Array)} parameters
 *   To search for file objects in.
 * @param {number} contentId
 *   Used to insert thumbnails for images.
 */
H5P.findCopyrights = function (info, parameters, contentId) {
  // Cycle through parameters
  for (var field in parameters) {
    if (!parameters.hasOwnProperty(field)) {
      continue; // Do not check
    }
    var value = parameters[field];

    if (value instanceof Array) {
      // Cycle through array
      H5P.findCopyrights(info, value, contentId);
    }
    else if (value instanceof Object) {
      // Check if object is a file with copyrights
      if (value.copyright === undefined ||
          value.copyright.license === undefined ||
          value.path === undefined ||
          value.mime === undefined) {

        // Nope, cycle throught object
        H5P.findCopyrights(info, value, contentId);
      }
      else {
        // Found file, add copyrights
        var copyrights = new H5P.MediaCopyright(value.copyright);
        if (value.width !== undefined && value.height !== undefined) {
          copyrights.setThumbnail(new H5P.Thumbnail(H5P.getPath(value.path, contentId), value.width, value.height));
        }
        info.addMedia(copyrights);
      }
    }
    else {
    }
  }
};

/**
 * Display a dialog containing the embed code.
 *
 * @param {H5P.jQuery} $element
 *   Element to insert dialog after.
 * @param {string} embedCode
 *   The embed code.
 * @param {string} resizeCode
 *   The advanced resize code
 * @param {Object} size
 *   The content's size.
 * @param {number} size.width
 * @param {number} size.height
 */
H5P.openEmbedDialog = function ($element, embedCode, resizeCode, size) {
  var fullEmbedCode = embedCode + resizeCode;
  var dialog = new H5P.Dialog('embed', H5P.t('embed'), '<textarea class="h5p-embed-code-container" autocorrect="off" autocapitalize="off" spellcheck="false"></textarea>' + H5P.t('size') + ': <input type="text" value="' + Math.ceil(size.width) + '" class="h5p-embed-size"/> ‚Äö√Ñ√∂‚àö‚Ä†‚àö‚àÇ‚Äö√†√∂‚Äö√¢‚Ä¢ <input type="text" value="' + Math.ceil(size.height) + '" class="h5p-embed-size"/> px<br/><div role="button" tabindex="0" class="h5p-expander">' + H5P.t('showAdvanced') + '</div><div class="h5p-expander-content"><p>' + H5P.t('advancedHelp') + '</p><textarea class="h5p-embed-code-container" autocorrect="off" autocapitalize="off" spellcheck="false">' + resizeCode + '</textarea></div>', $element);

  // Selecting embed code when dialog is opened
  H5P.jQuery(dialog).on('dialog-opened', function (event, $dialog) {
    var $inner = $dialog.find('.h5p-inner');
    var $scroll = $inner.find('.h5p-scroll-content');
    var diff = $scroll.outerHeight() - $scroll.innerHeight();
    var positionInner = function () {
      var height = $inner.height();
      if ($scroll[0].scrollHeight + diff > height) {
        $inner.css('height', ''); // 100%
      }
      else {
        $inner.css('height', 'auto');
        height = $inner.height();
      }
      $inner.css('marginTop', '-' + (height / 2) + 'px');
    };

    // Handle changing of width/height
    var $w = $dialog.find('.h5p-embed-size:eq(0)');
    var $h = $dialog.find('.h5p-embed-size:eq(1)');
    var getNum = function ($e, d) {
      var num = parseFloat($e.val());
      if (isNaN(num)) {
        return d;
      }
      return Math.ceil(num);
    };
    var updateEmbed = function () {
      $dialog.find('.h5p-embed-code-container:first').val(fullEmbedCode.replace(':w', getNum($w, size.width)).replace(':h', getNum($h, size.height)));
    };

    $w.change(updateEmbed);
    $h.change(updateEmbed);
    updateEmbed();

    // Select text and expand textareas
    $dialog.find('.h5p-embed-code-container').each(function(index, value) {
      H5P.jQuery(this).css('height', this.scrollHeight + 'px').focus(function() {
          H5P.jQuery(this).select();
        });
    });
    $dialog.find('.h5p-embed-code-container').eq(0).select();
    positionInner();

    // Expand advanced embed
    var expand = function () {
      var $expander = H5P.jQuery(this);
      var $content = $expander.next();
      if ($content.is(':visible')) {
        $expander.removeClass('h5p-open').text(H5P.t('showAdvanced'));
        $content.hide();
      }
      else {
        $expander.addClass('h5p-open').text(H5P.t('hideAdvanced'));
        $content.show();
      }
      $dialog.find('.h5p-embed-code-container').each(function(index, value) {
        H5P.jQuery(this).css('height', this.scrollHeight + 'px');
      });
      positionInner();
    };
    $dialog.find('.h5p-expander').click(expand).keypress(function (event) {
      if (event.keyCode === 32) {
        expand.apply(this);
      }
    });
  });

  dialog.open();
};

/**
 * Copyrights for a H5P Content Library.
 *
 * @class
 */
H5P.ContentCopyrights = function () {
  var label;
  var media = [];
  var content = [];

  /**
   * Set label.
   *
   * @param {string} newLabel
   */
  this.setLabel = function (newLabel) {
    label = newLabel;
  };

  /**
   * Add sub content.
   *
   * @param {H5P.MediaCopyright} newMedia
   */
  this.addMedia = function (newMedia) {
    if (newMedia !== undefined) {
      media.push(newMedia);
    }
  };

  /**
   * Add sub content.
   *
   * @param {H5P.ContentCopyrights} newContent
   */
  this.addContent = function (newContent) {
    if (newContent !== undefined) {
      content.push(newContent);
    }
  };

  /**
   * Print content copyright.
   *
   * @returns {string} HTML.
   */
  this.toString = function () {
    var html = '';

    // Add media rights
    for (var i = 0; i < media.length; i++) {
      html += media[i];
    }

    // Add sub content rights
    for (i = 0; i < content.length; i++) {
      html += content[i];
    }


    if (html !== '') {
      // Add a label to this info
      if (label !== undefined) {
        html = '<h3>' + label + '</h3>' + html;
      }

      // Add wrapper
      html = '<div class="h5p-content-copyrights">' + html + '</div>';
    }

    return html;
  };
};

/**
 * A ordered list of copyright fields for media.
 *
 * @class
 * @param {Object} copyright
 *   Copyright information fields.
 * @param {Object} [labels]
 *   Translation of labels.
 * @param {Array} [order]
 *   Order of the fields.
 * @param {Object} [extraFields]
 *   Add extra copyright fields.
 */
H5P.MediaCopyright = function (copyright, labels, order, extraFields) {
  var thumbnail;
  var list = new H5P.DefinitionList();

  /**
   * Get translated label for field.
   *
   * @private
   * @param {string} fieldName
   * @returns {string}
   */
  var getLabel = function (fieldName) {
    if (labels === undefined || labels[fieldName] === undefined) {
      return H5P.t(fieldName);
    }

    return labels[fieldName];
  };

  /**
   * Get humanized value for field.
   *
   * @private
   * @param {string} fieldName
   * @param {string} value
   * @returns {string}
   */
  var humanizeValue = function (fieldName, value) {
    if (fieldName === 'license') {
      return H5P.copyrightLicenses[value];
    }

    return value;
  };

  if (copyright !== undefined) {
    // Add the extra fields
    for (var field in extraFields) {
      if (extraFields.hasOwnProperty(field)) {
        copyright[field] = extraFields[field];
      }
    }

    if (order === undefined) {
      // Set default order
      order = ['title', 'author', 'year', 'source', 'license'];
    }

    for (var i = 0; i < order.length; i++) {
      var fieldName = order[i];
      if (copyright[fieldName] !== undefined) {
        list.add(new H5P.Field(getLabel(fieldName), humanizeValue(fieldName, copyright[fieldName])));
      }
    }
  }

  /**
   * Set thumbnail.
   *
   * @param {H5P.Thumbnail} newThumbnail
   */
  this.setThumbnail = function (newThumbnail) {
    thumbnail = newThumbnail;
  };

  /**
   * Checks if this copyright is undisclosed.
   * I.e. only has the license attribute set, and it's undisclosed.
   *
   * @returns {boolean}
   */
  this.undisclosed = function () {
    if (list.size() === 1) {
      var field = list.get(0);
      if (field.getLabel() === getLabel('license') && field.getValue() === humanizeValue('license', 'U')) {
        return true;
      }
    }
    return false;
  };

  /**
   * Print media copyright.
   *
   * @returns {string} HTML.
   */
  this.toString = function () {
    var html = '';

    if (this.undisclosed()) {
      return html; // No need to print a copyright with a single undisclosed license.
    }

    if (thumbnail !== undefined) {
      html += thumbnail;
    }
    html += list;

    if (html !== '') {
      html = '<div class="h5p-media-copyright">' + html + '</div>';
    }

    return html;
  };
};

/**
 * Maps copyright license codes to their human readable counterpart.
 *
 * @type {Object}
 */
H5P.copyrightLicenses = {
  'U': 'Undisclosed',
  'CC BY': '<a href="http://creativecommons.org/licenses/by/4.0/legalcode" target="_blank">Attribution 4.0</a>',
  'CC BY-SA': '<a href="https://creativecommons.org/licenses/by-sa/4.0/legalcode" target="_blank">Attribution-ShareAlike 4.0</a>',
  'CC BY-ND': '<a href="https://creativecommons.org/licenses/by-nd/4.0/legalcode" target="_blank">Attribution-NoDerivs 4.0</a>',
  'CC BY-NC': '<a href="https://creativecommons.org/licenses/by-nc/4.0/legalcode" target="_blank">Attribution-NonCommercial 4.0</a>',
  'CC BY-NC-SA': '<a href="https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode" target="_blank">Attribution-NonCommercial-ShareAlike 4.0</a>',
  'CC BY-NC-ND': '<a href="https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode" target="_blank">Attribution-NonCommercial-NoDerivs 4.0</a>',
  'GNU GPL': '<a href="http://www.gnu.org/licenses/gpl-3.0-standalone.html" target="_blank">General Public License v3</a>',
  'PD': 'Public Domain',
  'ODC PDDL': '<a href="http://opendatacommons.org/licenses/pddl/1.0/" target="_blank">Public Domain Dedication and Licence</a>',
  'CC PDM': 'Public Domain Mark',
  'C': 'Copyright'
};

/**
 * A simple and elegant class for creating thumbnails of images.
 *
 * @class
 * @param {string} source
 * @param {number} width
 * @param {number} height
 */
H5P.Thumbnail = function (source, width, height) {
  var thumbWidth, thumbHeight = 100;
  if (width !== undefined) {
    thumbWidth = Math.round(thumbHeight * (width / height));
  }

  /**
   * Print thumbnail.
   *
   * @returns {string} HTML.
   */
  this.toString = function () {
    return '<img src="' + source + '" alt="' + H5P.t('thumbnail') + '" class="h5p-thumbnail" height="' + thumbHeight + '"' + (thumbWidth === undefined ? '' : ' width="' + thumbWidth + '"') + '/>';
  };
};

/**
 * Simple data structure class for storing a single field.
 *
 * @class
 * @param {string} label
 * @param {string} value
 */
H5P.Field = function (label, value) {
  /**
   * Public. Get field label.
   *
   * @returns {String}
   */
  this.getLabel = function () {
    return label;
  };

  /**
   * Public. Get field value.
   *
   * @returns {String}
   */
  this.getValue = function () {
    return value;
  };
};

/**
 * Simple class for creating a definition list.
 *
 * @class
 */
H5P.DefinitionList = function () {
  var fields = [];

  /**
   * Add field to list.
   *
   * @param {H5P.Field} field
   */
  this.add = function (field) {
    fields.push(field);
  };

  /**
   * Get Number of fields.
   *
   * @returns {number}
   */
  this.size = function () {
    return fields.length;
  };

  /**
   * Get field at given index.
   *
   * @param {number} index
   * @returns {H5P.Field}
   */
  this.get = function (index) {
    return fields[index];
  };

  /**
   * Print definition list.
   *
   * @returns {string} HTML.
   */
  this.toString = function () {
    var html = '';
    for (var i = 0; i < fields.length; i++) {
      var field = fields[i];
      html += '<dt>' + field.getLabel() + '</dt><dd>' + field.getValue() + '</dd>';
    }
    return (html === '' ? html : '<dl class="h5p-definition-list">' + html + '</dl>');
  };
};

/**
 * THIS FUNCTION/CLASS IS DEPRECATED AND WILL BE REMOVED.
 *
 * Helper object for keeping coordinates in the same format all over.
 *
 * @deprecated
 *   Will be removed march 2016.
 * @class
 * @param {number} x
 * @param {number} y
 * @param {number} w
 * @param {number} h
 */
H5P.Coords = function (x, y, w, h) {
  if ( !(this instanceof H5P.Coords) )
    return new H5P.Coords(x, y, w, h);

  /** @member {number} */
  this.x = 0;
  /** @member {number} */
  this.y = 0;
  /** @member {number} */
  this.w = 1;
  /** @member {number} */
  this.h = 1;

  if (typeof(x) === 'object') {
    this.x = x.x;
    this.y = x.y;
    this.w = x.w;
    this.h = x.h;
  } else {
    if (x !== undefined) {
      this.x = x;
    }
    if (y !== undefined) {
      this.y = y;
    }
    if (w !== undefined) {
      this.w = w;
    }
    if (h !== undefined) {
      this.h = h;
    }
  }
  return this;
};

/**
 * Parse library string into values.
 *
 * @param {string} library
 *   library in the format "machineName majorVersion.minorVersion"
 * @returns {Object}
 *   library as an object with machineName, majorVersion and minorVersion properties
 *   return false if the library parameter is invalid
 */
H5P.libraryFromString = function (library) {
  var regExp = /(.+)\s(\d+)\.(\d+)$/g;
  var res = regExp.exec(library);
  if (res !== null) {
    return {
      'machineName': res[1],
      'majorVersion': res[2],
      'minorVersion': res[3]
    };
  }
  else {
    return false;
  }
};

/**
 * Get the path to the library
 *
 * @param {string} library
 *   The library identifier in the format "machineName-majorVersion.minorVersion".
 * @returns {string}
 *   The full path to the library.
 */
H5P.getLibraryPath = function (library) {
  return (H5PIntegration.libraryUrl !== undefined ? H5PIntegration.libraryUrl + '/' : H5PIntegration.url + '/libraries/') + library;
};

/**
 * Recursivly clone the given object.
 *
 * @param {Object|Array} object
 *   Object to clone.
 * @param {boolean} [recursive]
 * @returns {Object|Array}
 *   A clone of object.
 */
H5P.cloneObject = function (object, recursive) {
  // TODO: Consider if this needs to be in core. Doesn't $.extend do the same?
  var clone = object instanceof Array ? [] : {};

  for (var i in object) {
    if (object.hasOwnProperty(i)) {
      if (recursive !== undefined && recursive && typeof object[i] === 'object') {
        clone[i] = H5P.cloneObject(object[i], recursive);
      }
      else {
        clone[i] = object[i];
      }
    }
  }

  return clone;
};

/**
 * Remove all empty spaces before and after the value.
 *
 * @param {string} value
 * @returns {string}
 */
H5P.trim = function (value) {
  return value.replace(/^\s+|\s+$/g, '');

  // TODO: Only include this or String.trim(). What is best?
  // I'm leaning towards implementing the missing ones: http://kangax.github.io/compat-table/es5/
  // So should we make this function deprecated?
};

/**
 * Check if JavaScript path/key is loaded.
 *
 * @param {string} path
 * @returns {boolean}
 */
H5P.jsLoaded = function (path) {
  H5PIntegration.loadedJs = H5PIntegration.loadedJs || [];
  return H5P.jQuery.inArray(path, H5PIntegration.loadedJs) !== -1;
};

/**
 * Check if styles path/key is loaded.
 *
 * @param {string} path
 * @returns {boolean}
 */
H5P.cssLoaded = function (path) {
  H5PIntegration.loadedCss = H5PIntegration.loadedCss || [];
  return H5P.jQuery.inArray(path, H5PIntegration.loadedCss) !== -1;
};

/**
 * Shuffle an array in place.
 *
 * @param {Array} array
 *   Array to shuffle
 * @returns {Array}
 *   The passed array is returned for chaining.
 */
H5P.shuffleArray = function (array) {
  // TODO: Consider if this should be a part of core. I'm guessing very few libraries are going to use it.
  if (!(array instanceof Array)) {
    return;
  }

  var i = array.length, j, tempi, tempj;
  if ( i === 0 ) return false;
  while ( --i ) {
    j       = Math.floor( Math.random() * ( i + 1 ) );
    tempi   = array[i];
    tempj   = array[j];
    array[i] = tempj;
    array[j] = tempi;
  }
  return array;
};

/**
 * Post finished results for user.
 *
 * @deprecated
 *   Do not use this function directly, trigger the finish event instead.
 *   Will be removed march 2016
 * @param {number} contentId
 *   Identifies the content
 * @param {number} score
 *   Achieved score/points
 * @param {number} maxScore
 *   The maximum score/points that can be achieved
 * @param {number} [time]
 *   Reported time consumption/usage
 */
H5P.setFinished = function (contentId, score, maxScore, time) {


  if (H5PIntegration.postUserStatistics === true) {
    /**
     * Return unix timestamp for the given JS Date.
     *
     * @private
     * @param {Date} date
     * @returns {Number}
     */
    var toUnix = function (date) {
      return Math.round(date.getTime() / 1000);
    };
    //h5pcustomize
    var urlarr = window.location.href.split("&");
    var enrollId = '';
    if(urlarr != null)
    {
   		var enrollIdStr = urlarr[1];
   		if(enrollIdStr != null)
   		{
	   		var keyvalpair = enrollIdStr.split("=");
		   if(keyvalpair[0] == "enrollId")
		   {
   			 enrollId = keyvalpair[1];
   			}
   		}
   	}
   	

    // Post the results
    H5P.jQuery.post(H5PIntegration.ajax.setFinished, {
      contentId: contentId,
      score: score,
      maxScore: maxScore,
      opened: toUnix(H5P.opened[contentId]),
      finished: toUnix(new Date()),
      time: time,
      token: H5PIntegration.tokens.result,
      enrollId:enrollId
    });
  }
};

// Add indexOf to browsers that lack them. (IEs)
if (!Array.prototype.indexOf) {
  Array.prototype.indexOf = function (needle) {
    for (var i = 0; i < this.length; i++) {
      if (this[i] === needle) {
        return i;
      }
    }
    return -1;
  };
}

// Need to define trim() since this is not available on older IEs,
// and trim is used in several libs
if (String.prototype.trim === undefined) {
  String.prototype.trim = function () {
    return H5P.trim(this);
  };
}

/**
 * Trigger an event on an instance
 *
 * Helper function that triggers an event if the instance supports event handling
 *
 * @param {Object} instance
 *   Instance of H5P content
 * @param {string} eventType
 *   Type of event to trigger
 * @param {*} data
 * @param {Object} extras
 */
H5P.trigger = function (instance, eventType, data, extras) {
  // Try new event system first
  if (instance.trigger !== undefined) {
    instance.trigger(eventType, data, extras);
  }
  // Try deprecated event system
  else if (instance.$ !== undefined && instance.$.trigger !== undefined) {
    instance.$.trigger(eventType);
  }
};

/**
 * Register an event handler
 *
 * Helper function that registers an event handler for an event type if
 * the instance supports event handling
 *
 * @param {Object} instance
 *   Instance of H5P content
 * @param {string} eventType
 *   Type of event to listen for
 * @param {H5P.EventCallback} handler
 *   Callback that gets triggered for events of the specified type
 */
H5P.on = function (instance, eventType, handler) {
  // Try new event system first
  if (instance.on !== undefined) {
    instance.on(eventType, handler);
  }
  // Try deprecated event system
  else if (instance.$ !== undefined && instance.$.on !== undefined) {
    instance.$.on(eventType, handler);
  }
};

/**
 * Generate random UUID
 *
 * @returns {string} UUID
 */
H5P.createUUID = function () {
  return 'xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx'.replace(/[xy]/g, function(char) {
    var random = Math.random()*16|0, newChar = char === 'x' ? random : (random&0x3|0x8);
    return newChar.toString(16);
  });
};

/**
 * Create title
 *
 * @param {string} rawTitle
 * @param {number} maxLength
 * @returns {string}
 */
H5P.createTitle = function (rawTitle, maxLength) {
  if (!rawTitle) {
    return '';
  }
  if (maxLength === undefined) {
    maxLength = 60;
  }
  var title = H5P.jQuery('<div></div>')
    .text(
      // Strip tags
      rawTitle.replace(/(<([^>]+)>)/ig,"")
    // Escape
    ).text();
  if (title.length > maxLength) {
    title = title.substr(0, maxLength - 3) + '...';
  }
  return title;
};

// Wrap in privates
(function ($) {

  /**
   * Creates ajax requests for inserting, updateing and deleteing
   * content user data.
   *
   * @private
   * @param {number} contentId What content to store the data for.
   * @param {string} dataType Identifies the set of data for this content.
   * @param {string} subContentId Identifies sub content
   * @param {function} [done] Callback when ajax is done.
   * @param {object} [data] To be stored for future use.
   * @param {boolean} [preload=false] Data is loaded when content is loaded.
   * @param {boolean} [invalidate=false] Data is invalidated when content changes.
   * @param {boolean} [async=true]
   */
  function contentUserDataAjax(contentId, dataType, subContentId, done, data, preload, invalidate, async) {
    if (H5PIntegration.user === undefined) {
      // Not logged in, no use in saving.
      done('Not signed in.');
      return;
    }
    //h5pcustomize
    var urlarr = window.location.href.split("&");
    var attachQryStr = "";
    if(urlarr != null)
    {
   		var enrollIdStr = urlarr[1];
   		if(enrollIdStr != null)
   		{
	   		var keyvalpair = enrollIdStr.split("=");
   			var enrollId = 0,preview= false;
   
		   if(keyvalpair[0] == "enrollId")
		   {
   			 enrollId = keyvalpair[1];
	   	 	attachQryStr = "&enrollId="+enrollId;
   			}
   			else if(keyvalpair[0] == "preview")
   			{
		   	//	preview = true;
		   	//	attachQryStr = "&preview="+preview;
   			}
   		}
   	}
   	
   	var totalVideoTime =$(".h5p-total").html();
    var options = {
      url: H5PIntegration.ajax.contentUserData.replace(':contentId', contentId).replace(':dataType', dataType).replace(':subContentId', subContentId ? subContentId : 0)+attachQryStr,
      dataType: 'json',
      async: async === undefined ? true : async
    };
    if (data !== undefined) {
      
      data = JSON.parse(data);
      data.totaltime = totalVideoTime;
      data = JSON.stringify(data);
      options.type = 'POST';
      options.data = {
        data: (data === null ? 0 : data),
        preload: (preload ? 1 : 0),
        invalidate: (invalidate ? 1 : 0),
        token: H5PIntegration.tokens.contentUserData
      };
      //options.data = options.data;
    }
    else {
      options.type = 'GET';
    }
    if (done !== undefined) {
      options.error = function (xhr, error) {
        done(error);
      };
      options.success = function (response) {
        if (!response.success) {
          done(response.message);
          return;
        }

        if (response.data === false || response.data === undefined) {
          done();
          return;
        }

        done(undefined, response.data);
      };
    }
   
    $.ajax(options);
  }

  /**
   * Get user data for given content.
   *
   * @param {number} contentId
   *   What content to get data for.
   * @param {string} dataId
   *   Identifies the set of data for this content.
   * @param {function} done
   *   Callback with error and data parameters.
   * @param {string} [subContentId]
   *   Identifies which data belongs to sub content.
   */
  H5P.getUserData = function (contentId, dataId, done, subContentId) {
    if (!subContentId) {
      subContentId = 0; // Default
    }

    H5PIntegration.contents = H5PIntegration.contents || {};
    var content = H5PIntegration.contents['cid-' + contentId] || {};
    var preloadedData = content.contentUserData;
    if (preloadedData && preloadedData[subContentId] && preloadedData[subContentId][dataId]) {
      if (preloadedData[subContentId][dataId] === 'RESET') {
        done(undefined, null);
        return;
      }
      try {
        done(undefined, JSON.parse(preloadedData[subContentId][dataId]));
      }
      catch (err) {
        done(err);
      }
    }
    else {
    	
      contentUserDataAjax(contentId, dataId, subContentId, function (err, data) {
        if (err || data === undefined) {
          done(err, data);
          return; // Error or no data
        }

        // Cache in preloaded
        if (content.contentUserData === undefined) {
          content.contentUserData = preloadedData = {};
        }
        if (preloadedData[subContentId] === undefined) {
          preloadedData[subContentId] = {};
        }
        preloadedData[subContentId][dataId] = data;

        // Done. Try to decode JSON
        try {
          done(undefined, JSON.parse(data));
        }
        catch (e) {
          done(e);
        }
      });
    }
  };

  /**
   * Async error handling.
   *
   * @callback H5P.ErrorCallback
   * @param {*} error
   */

  /**
   * Set user data for given content.
   *
   * @param {number} contentId
   *   What content to get data for.
   * @param {string} dataId
   *   Identifies the set of data for this content.
   * @param {Object} data
   *   The data that is to be stored.
   * @param {Object} [extras]
   *   Extra properties
   * @param {string} [extras.subContentId]
   *   Identifies which data belongs to sub content.
   * @param {boolean} [extras.preloaded=true]
   *   If the data should be loaded when content is loaded.
   * @param {boolean} [extras.deleteOnChange=false]
   *   If the data should be invalidated when the content changes.
   * @param {H5P.ErrorCallback} [extras.errorCallback]
   *   Callback with error as parameters.
   * @param {boolean} [extras.async=true]
   */
  H5P.setUserData = function (contentId, dataId, data, extras) {
    var options = H5P.jQuery.extend(true, {}, {
      subContentId: 0,
      preloaded: true,
      deleteOnChange: false,
      async: true
    }, extras);

    try {
      data = JSON.stringify(data);
    }
    catch (err) {
      if (options.errorCallback) {
        options.errorCallback(err);
      }
      return; // Failed to serialize.
    }

    var content = H5PIntegration.contents['cid-' + contentId];
    if (content === undefined) {
      content = H5PIntegration.contents['cid-' + contentId] = {};
    }
    if (!content.contentUserData) {
      content.contentUserData = {};
    }
    var preloadedData = content.contentUserData;
    if (preloadedData[options.subContentId] === undefined) {
      preloadedData[options.subContentId] = {};
    }
    if (data === preloadedData[options.subContentId][dataId]) {
      return; // No need to save this twice.
    }

    preloadedData[options.subContentId][dataId] = data;
    
    contentUserDataAjax(contentId, dataId, options.subContentId, function (error, data) {
      if (options.errorCallback && error) {
        options.errorCallback(error);
      }
      
      
     console.log("this is condition : " ,H5P.CoursePresentation);
      //#0083775 Refreshing the tracked progress for Video On demand - starts  -- WILL BE SENT IN NEXT BUILD AFTER THROUGH TESTING.
/*   if(H5P.CoursePresentation == undefined) 
   {
     var callbackdata = window.parent.$(".active-content").find(".clsMenulistTitle").attr("onclick");
     console.log("the callback dfata : ",callbackdata);
     var string1 = "";
     string1 += callbackdata;
     var rep = "function onclick(event) {"
     string1 = string1.replace(rep,'');
     var rep1 = "";
     if (string1.indexOf("#learner-enrollment-tab-inner") >= 0)
    	 rep1 = "$('#learner-enrollment-tab-inner').data('contentPlayer').changePlayerContent(";
     else if(string1.indexOf("#lnr-catalog-search") >= 0)
    	 rep1 = "$('#lnr-catalog-search').data('contentPlayer').changePlayerContent(";
     else if(string1.indexOf("#learningplan-tab-inner") >= 0)
    	 rep1 = "$('#learningplan-tab-inner').data('contentPlayer').changePlayerContent(";
    	 

     string1 = string1.replace(rep1,'');
     console.log("string222 : " + string1);
     string1 = string1.replace(');','');
     string1 = string1.replace(string1.substr(string1.length - 2),'');
     var jsonData1 =JSON.stringify(string1);
     console.log("trying this now jsonnaa : " , H5P);
    
    
     jsonData1 = jsonData1.replace(/'/g, '"');
     jsonData1 = jsonData1.replace('"\n', '');
     jsonData1 = jsonData1.replace('}"', '}');
     jsonData1 = jsonData1.replace(jsonData1.substr(0,3),'');
     jsonData1 = jsonData1.replace(jsonData1.substr(jsonData1.length - 2),'}');
     console.log("trying this now jsonnaaaa : " , jsonData1);
     jsonData1 = JSON.parse(jsonData1);
    
     console.log("trying this now jsonnaaaa111 : " , jsonData1.contentId);
  
     //if(jsonData1.contentType == "Video on Demand")
    // {
    
        var prevContentStatus = '';
		var contentProgress = '';
		var callbackdata = {launchflag:1,data:undefined};
 
   var h5pLaunchedFrom = "";
    if(jsonData1.LaunchFrom == "ME")
      h5pLaunchedFrom = "learner-enrollment-tab-inner";
    else if(jsonData1.LaunchFrom == "LP")
    	 h5pLaunchedFrom = "learningplan-tab-inner";
    else if(jsonData1.LaunchFrom == "CL")
    	 h5pLaunchedFrom = "lnr-catalog-search";
     
     
     window.parent.$("#" + h5pLaunchedFrom).data('contentLaunch').updateVODScoreOnCtoolsModalClose(jsonData1.LaunchFrom, jsonData1.courseId, jsonData1.classId, jsonData1.LessonId, jsonData1.VersionId, jsonData1.enrollId, prevContentStatus, contentProgress, callbackdata);

   } */
    
    }, data, options.preloaded, options.deleteOnChange, options.async);
  };

  /**
   * Delete user data for given content.
   *
   * @param {number} contentId
   *   What content to remove data for.
   * @param {string} dataId
   *   Identifies the set of data for this content.
   * @param {string} [subContentId]
   *   Identifies which data belongs to sub content.
   */
  H5P.deleteUserData = function (contentId, dataId, subContentId) {
    if (!subContentId) {
      subContentId = 0; // Default
    }

    // Remove from preloaded/cache
    var preloadedData = H5PIntegration.contents['cid-' + contentId].contentUserData;
    if (preloadedData && preloadedData[subContentId] && preloadedData[subContentId][dataId]) {
      delete preloadedData[subContentId][dataId];
    }
    
    contentUserDataAjax(contentId, dataId, subContentId, undefined, null);
  };

  // Init H5P when page is fully loadded
  $(document).ready(function () {
 
    //h5pcustomize

   try
   { 
    window.parent.EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.destroyLoader("modal-content");
  }catch(e){}
    /**
     * Prevent H5P Core from initializing. Must be overriden before document ready.
     * @member {boolean} H5P.preventInit
     */
    if (!H5P.preventInit) {
      // Note that this start script has to be an external resource for it to
      // load in correct order in IE9.
      H5P.init(document.body);
    }

    if (H5PIntegration.saveFreq !== false) {
      // When was the last state stored
      var lastStoredOn = 0;
      // Store the current state of the H5P when leaving the page.
      var storeCurrentState = function () {
        // Make sure at least 250 ms has passed since last save
        var currentTime = new Date().getTime();
        if (currentTime - lastStoredOn > 250) {
          lastStoredOn = currentTime;
          for (var i = 0; i < H5P.instances.length; i++) {
            var instance = H5P.instances[i];
            if (instance.getCurrentState instanceof Function ||
                typeof instance.getCurrentState === 'function') {
              var state = instance.getCurrentState();
              if (state !== undefined) {
                // Async is not used to prevent the request from being cancelled.
                H5P.setUserData(instance.contentId, 'state', state, {deleteOnChange: true, async: false});
              }
            }
          }
        }
      };
      // iPad does not support beforeunload, therefore using unload
      H5P.$window.one('beforeunload unload', function () {
        // Only want to do this once
        H5P.$window.off('pagehide beforeunload unload');
        storeCurrentState();
      });
      // pagehide is used on iPad when tabs are switched
      H5P.$window.on('pagehide', storeCurrentState);
    }

    /**
     * Indicates if H5P is embedded on an external page using iframe.
     * @member {boolean} H5P.externalEmbed
     */

    // Relay events to top window.
    if (H5P.isFramed && H5P.externalEmbed === false) {
      H5P.externalDispatcher.on('*', function (event) {
        window.parent.H5P.externalDispatcher.trigger.call(this, event);
      });
    }
  });

})(H5P.jQuery);

//h5pcustomize
//Following code for uploaded videos to operate play/pause operation on video click

var isRequestOn = false;

H5P.jQuery(document).ready(function(){
//width:947px;height:450px;
	
	//Below line is added for playing video when onclick of video. #74107

	initiateClickEventVideo();
	/*H5P.jQuery('video').click(function(){
		this.paused?this.play():this.pause();
		return false;
	}); */
	
	
	//H5P.jQuery('.h5p-video-wrapper video').click(function(){this.paused?this.play():this.pause();});
	
	/*H5P.jQuery(".h5p-video").css("height","480px");
	H5P.jQuery(".h5p-iframe").css("height","480px");
	window.parent.$(".h5p-iframe").css("height","480px"); */
	//H5P.jQuery(".h5p-initialized").attr("style","max-height:505px;");
	
//	H5P.jQuery(".h5p-video").addClass("Fullsc");
//	H5P.jQuery("[id^=h5p-youtube]").addClass("FullscYoutube");
//	alert(8888888555555555);
	//H5P.jQuery(".Fullsc").attr("style","max-height:468px;max-width:860px;");
/*	H5P.jQuery(".Fullsc").attr("style","max-height:468px;max-width:947px;");   
 CHANGING TO ADJUST WITH THE CONTENT PLAYER LAUNCH WINDOW 
 */
//	H5P.jQuery(".Fullsc").attr("style","max-height:419px;max-width:955px;"); 
//	
//	
//	//H5P.jQuery(".FullscYoutube").attr("style","min-width:955px;max-width:955px;"); 
//	H5P.jQuery(".FullscYoutube").attr("style","max-width:955px;");
//	alert("This is learner's 999 side");
//	
//	if (H5P.jQuery(".h5p-container").hasClass('h5p-standalone') && H5P.jQuery(".h5p-container").hasClass('h5p-minimal')) {
//        H5P.jQuery(".Fullsc").attr("style","max-height:180px;max-width:480px;margin-top:0px");
//        H5P.jQuery(".FullscYoutube").attr("style","max-width:305px;max-height:171px"); 
//        alert("This is admin's 447774 side");
//	}
	
	H5P.jQuery(".h5p-video").find(".h5p-video").addClass("Fullsc");
	//if learner side launch, remove fullscreen
	try
	{
	if(window.parent.$("#h5pplayer").size() > 0 || window.parent.$("#iframedata").size() > 0 )
	{	
		H5P.canHasFullScreen = false;
		H5P.learnerLaunch = true;
		
	}
	}catch(e){}
	var width_cp = window.parent.$("#cp-content-main").width() ;
	var height_cp = window.parent.$("#cp-content-main").height() - 15;

	var height_cp_wrapper = height_cp - 25;
	
  H5P.jQuery("[id^=h5p-youtube]").addClass("FullscYoutube");
  if(window.parent.$("#h5pplayer").size() > 0)
	{
	
	  H5P.jQuery(".FullscYoutube").attr("style","max-width:"+width_cp+"px;max-height:"+height_cp+"px;min-width:"+width_cp+"px;min-height:"+height_cp+"px"); 
	  H5P.jQuery(".FullscYoutube").parent().attr("style","max-width:"+width_cp+"px;max-height:"+height_cp_wrapper+"px;min-width:"+width_cp+"px;min-height:"+height_cp_wrapper+"px"); 

	  H5P.jQuery(".Fullsc").attr("style","max-width:"+width_cp+"px;max-height:"+height_cp+"px;min-width:"+width_cp+"px;min-height:"+height_cp+"px"); 
	  H5P.jQuery(".Fullsc").parent().attr("style","max-width:"+width_cp+"px;max-height:"+height_cp_wrapper+"px;min-width:"+width_cp+"px;min-height:"+height_cp_wrapper+"px"); 
	  //Added to avoid scrollbar
	  window.parent.$("#h5pplayer").css("height",window.parent.$("#cp-content-main").height());
	  if(window.parent.$("#h5pplayer").attr("subtype") == "h5p-Vimeo")
		  window.parent.$("#h5pplayer").contents().find("#my_video_2").attr("style","height:180%;width:100%;left:0;position:absolute;top:-40%");
	  
	}
  else{
	  H5P.jQuery(".FullscYoutube").attr("style","min-width:1000px"); // For admin side preview width adjustments
  }
	
/*  
   var iframe = H5P.jQuery("iframe"),
  		domPrefixes = 'Webkit Moz O ms Khtml'.split(' ');
  		alert(iframe);
  	var fullscreen = function(elem) {
          var prefix;
          // Mozilla and webkit intialise fullscreen slightly differently
          for ( var i = -1, len = domPrefixes.length; ++i < len; ) {
            prefix = domPrefixes[i].toLowerCase();

            if ( elem[prefix + 'EnterFullScreen'] ) {
              // Webkit uses EnterFullScreen for video
              return prefix + 'EnterFullScreen';
              break;
            } else if( elem[prefix + 'RequestFullScreen'] ) {
              // Mozilla uses RequestFullScreen for all elements and webkit uses it for non video elements
              return prefix + 'RequestFullScreen';
              break;
            }
          }

          return false;
      };  
  
      var fullscreenother = fullscreen(document.createElement("iframe"));

      if(!fullscreen) {
          alert("Fullscreen won't work, please make sure you're using a browser that supports it and you have enabled the feature");
          return;
      }
	
  

	H5P.jQuery("body").find("[id^=h5p-youtube]").keyup(function(e){
	  if (e.keyCode == 27) {  
				alert('hi');
		  }   // 
	});  
  */
 			if(window.H5P)
            {
 				//alert("launch123"+window.location.href);
 				//fadeoutArrows('slideNew');
				if(window.location.href.indexOf("preview=true") <= 0)
				{
                H5P.externalDispatcher.on('xAPI', function (event) {
                	if(isRequestOn){
                		return false;
                	}
                
                var dataId = "";	
                if( window.parent.$("#h5pplayer").size() > 0)
                {	
                	dataId  = "#h5pplayer";
                }	
                else
                {
                	dataId  = "#cp-menucontainer";                	
                }
				var contentId		= window.parent.$(dataId).attr("contentId");
        		var contentTitle	= window.parent.$(dataId).attr("contentTitle");
        		var contentStatus 	= window.parent.$(dataId).attr("Status");
        	//	var classId 		= window.parent.$(dataId).attr("courseId"); 
        		var classId 		= window.parent.$(dataId).attr("classId"); 
        	//	var classTitle 		= window.parent.$(dataId).attr("classTitle");
        		
        		//console.log("classTitle121 :",className);
        		//alert("classTitle is :" +className);
                	event.data.statement.context.content_id = contentId;
               // 	event.data.statement.context.course_name = decodeURIComponent(classTitle);
                	event.data.statement.context.course_name = decodeURIComponent(className);
                	event.data.statement.context.registration = tincan_registration_id;
                	event.data.statement.context.content_name = decodeURIComponent(contentTitle);
                	//classId = "13";
                	event.data.statement.context.course_id = classId;
                	if(contentStatus =="Completed")
                		contentStatus = true;
                	else
                		contentStatus = false;
                	event.data.statement.context.course_completed = contentStatus;
                	//event.data.statement.context.content_completed = contentStatus;
                	
                	if(dataId == "#cp-menucontainer")
                	{
                		//record comes as "Progressed"
                		var verb = "";
                		for (var prop in event.data.statement.verb.display) {
        						if(prop == "en-US")
        							verb = event.data.statement.verb.display[prop];
       							

                		}
                		if(verb == "progressed")
                		{
                			
                			var slideNo = "";
                			for (var prop in event.data.statement.object.definition.extensions) {
                			    if(prop == "http://id.tincanapi.com/extension/ending-point")
                			    {
                			    	slideNo = event.data.statement.object.definition.extensions[prop];
                			    }
    						}
                			event.data.statement.object.definition.name={"en-US":" slide "+slideNo};
                			
                		}
                		
                	}
                	
                	//remove h5p references - Ex  "category": [{"id": "http://h5p.org/libraries/H5P.MarkTheWords-1.5","objectType": "Activity"}]
                	event.data.statement.object.definition.extensions = {};
                	event.data.statement.context.contextActivities.category = {};
                	event.data.statement.actor.name = user_fullname;
                    var data = {
                        token:h5ptincantoken,// moduleSettings.token,
                        statement: JSON.stringify(event.data.statement)
                    };
                  //  console.log("Statements:"+JSON.stringify(event.data.statement));
                    //$.post(moduleSettings.relayUrl, data);
		            isRequestOn = true;            
		            H5P.jQuery.ajax(
		            {
		            	url: "?q=ajax/tincanapi/relay",
		            	method:"POST", 
		            	data:data,
		            	success: function(result){
							isRequestOn = false;
					    }
					});        
                });
                }else{          		  
            		H5P.jQuery('.h5p-video.Fullsc').css('max-height','562px');
            		
            		//H5P.jQuery('.h5p-video.Fullsc').css('min-height','562px');      
            }
            }
            
  
});




function initiateClickEventVideo()
{
	var htmlPlayer = document.getElementsByTagName('video');
	for(var i = 0; i < htmlPlayer.length; i++){
    	htmlPlayer[i].onclick = function() {
        	if ( this.paused || this.ended ) {    
            	if ( this.ended ) { this.currentTime = 0; }
            	this.play();
        	}  else { 
            	this.pause();
        	}
    	}
	}

}
function loadCssFilesH5p(fileName)
{
	var link =document.createElement('link');
	link.rel='stylesheet';
	link.href=fileName;
	link.id = 'h5pMultiLanguage';
	//script.async = "async";
	H5P.jQuery("head").append(link);
}
function checkBrowser(){
	var browser = "";
	var isAtLeastIE11 = !!(navigator.userAgent.match(/Trident/) && !navigator.userAgent.match(/MSIE/))? true : false;
	
   var c = navigator.userAgent.search("Chrome");
   var f = navigator.userAgent.search("Firefox");
   var ie = navigator.userAgent.search("MSIE ");
   var ie11=navigator.appVersion.indexOf('Trident/');
 //  m9 = navigator.userAgent.search("MSIE 9.0");
   var s  = navigator.userAgent.toLowerCase().search("safari");
//if(ie11 > -1){
	if(isAtLeastIE11==1){
    	 browser ="MSIE"
    }else{
     	if (navigator.userAgent.toLowerCase().search("safari") >= 0 && navigator.userAgent.toLowerCase().search("chrome") < 0)
      	{      	
      		browser = "safari";
      	} 
        else if (c > -1) {        
            browser = "Chrome";
        } else if (f > -1) {        	
            browser = "Firefox";
        } else if (ie > -1) {        	
            browser ="MSIE";
        } else if (m8 > -1) {        	
            browser ="MSIE 8.0";
        }
 }
     return browser;
}


function loadScriptsForTinyMCE(fileName)
{
	var script=document.createElement('script');
	script.type='text/javascript';
	script.src=fileName;
	script.async = "async";
	H5P.jQuery("body").append(script);
}
function removeInteractionMessage(that, type){
	 var left_pos = "";
	 if(type == 'pre' || type == 'slide')
	  {

		 left_pos = 470;
		 if(ns.$(".h5p_int_overlay").size() == 0)
			 ns.$("<div class='h5p_int_overlay'  ></div>").insertBefore(ns.$(".h5p-course-presentation .h5p-wrapper"));
	  }
	 else
	   {
		 left_pos = 574;
		 if(ns.$(".h5p_int_overlay").size() == 0)
			 ns.$("<div class='h5p_int_overlay' style='left: 108px; z-index: 100; height: 98%; top:5px;'></div>").insertBefore(ns.$(".h5p-interactive-video"));
		 
	   }
	var msg = "";
	var invokedFunc = "";
	if(type == "deleteVideo")
	{
		msg =  window.parent.Drupal.t("MSG922");	
		invokedFunc = "DeleteVideo()";
	}
	else if(type == undefined || type == 'undefined' || type == 'pre')
	{
		//msg = "Are you sure you want to delete this interaction?";
		
		msg = window.parent.Drupal.t("LBL3203")+" "+window.parent.Drupal.t("LBL3021").toLocaleLowerCase()+"?";
		 invokedFunc = "DeleteInteraction()";
	}
	else if(type == 'slide')
	{
		msg = window.parent.Drupal.t("LBL3203")+" "+window.parent.Drupal.t("LBL3204")+"?";
		 invokedFunc = "DeleteInteraction()";
	}
	var rem="";
	rem +='<div id="delete-object-dialog" class="popupLeftCorner popupRightCorner"> ';
            rem +='<div class="ui-dialog ui-widget ui-widget-content ui-corner-all  popupLeftCorner popupRightCorner expertusV2PopupContainer" tabindex="-1" role="dialog" aria-labelledby="ui-dialog-title-delete-msg-wizard" style="display: block; z-index: 10007; outline: 0px; height: auto; width: 300px; top: 225px; left:'+left_pos+'px;box-shadow: none;">';
	
				     rem +='<div class="popupLeftCorner">';
						rem +='<div class="popupRightCorner">';
							rem +='<div class="ui-dialog-titlebar ui-widget-header ui-corner-all ui-helper-clearfix">';
							rem +='	<span class="ui-dialog-title" id="ui-dialog-title-delete-msg-wizard">'+window.parent.Drupal.t("LBL286")+'</span>';
								rem +='<a href="#" class="ui-dialog-titlebar-close ui-corner-all" role="button" onclick="NotDeleteInteraction()">';
								rem +='	<span class="ui-icon ui-icon-closethick">close</span>';
								rem +='</a>';
							rem +='</div>';
						rem +='</div>';
					rem +='</div>';
					rem +='<div id="delete-msg-wizard" style="padding: 0px; width: auto; height: auto;" class="ui-dialog-content ui-widget-content">';
						rem +='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
							rem +='<tbody>';
								rem +='<tr>';
									rem +='<td class="commanTitleAll">'+msg;			
										rem +='</td>';
									rem +='</tr>';
								rem +='</tbody>';
							rem +='</table>';
						rem +='</div>';
						rem +='<div class="ui-dialog-buttonpane ui-widget-content ui-helper-clearfix">';
							rem +='<div class="ui-dialog-buttonset">';
								rem +='<div class="white-btn-bg-left"></div>';
								rem +='<button type="button" class="removebutton white-btn-bg-middle" onclick="NotDeleteInteraction()">'+window.parent.Drupal.t('LBL109')+'</button>';
								rem +='<div class="white-btn-bg-right"></div>';
								rem +='<div class="admin-save-button-left-bg"></div>';
								rem +='<button type="button" class="removingH5pInteraction" onclick="'+invokedFunc+'">'+window.parent.Drupal.t('Yes')+'</button>';
								rem +='<div class="admin-save-button-right-bg"></div>';
							rem +='</div>';
						rem +='</div>';
						rem +='<div class="popupBotLeftCorner ui-helper-clearfix">';
							rem +='<div class="popupBotRightCorner">';
							rem +='	<div class="popupBotmiddle"></div>';
							rem +='</div>';
						rem +='</div>';
					rem +='</div>';
				rem +='</div>';
	
	//ns.$(".page-administration-contentauthor-video").html(rem);
			
				
	ns.$("body").prepend(rem);
	if(type == 'slide' || type == 'pre'){
	//	ns.$("#delete-object-dialog").find(".ui-dialog").css({"background-color": "#ececec"});
	}	
	
}
function DeleteVideo(){
	ns.$('#delete-object-dialog').hide();
	ns.$('.h5p_int_overlay').remove();

 window.parent.$("#tmpdata").remove();
 window.parent.$("#edit-video").show();
 window.parent.$("#edit-video").parent().removeClass("uploadiconbghide");
 window.parent.$("#edit-video").parent().addClass("uploadiconbgshow");
 window.parent.$("#edit-video").parent().css("width","200px");
 window.parent.$("#edit-video").removeAttr("disabled");


 window.parent.$(".customlabelfileuploadhelp").html(window.parent.Drupal.t('MSG807'));
 window.parent.$(".customlabelfileuploadhelp").show();
 window.parent.$(".customlabelfileupload").show();
 window.parent.$("#VIDEO_DELETE").val("TRUE");
 window.parent.$("#videofilenamedisplay").html("");
	 var recordType = window.parent.$("#recordtype").val();
	 
	 window.parent.$("#iframe_editor_wrapper_container").attr("videochanged","true");
	 window.parent.$("#iframe_editor_wrapper_container").hide();
	 
	 //Enable url field
	 if(recordType == "new")
	 {
		 window.parent.$("#edit-video").val("");
		 window.parent.$("#edit-url").attr("disabled","");		
		 window.parent.$("#two-col-row-url_uploadvideo .addedit-twocol-firstcol .addedit-new-field-title .addedit-mandatory").show();
		 window.parent.$("#edit-url").removeClass("addedit-readonly-textfield");
	 }
	
	
}

;var H5P = H5P || {};

/**
 * The Event class for the EventDispatcher.
 *
 * @class
 * @param {string} type
 * @param {*} data
 * @param {Object} [extras]
 * @param {boolean} [extras.bubbles]
 * @param {boolean} [extras.external]
 */
H5P.Event = function(type, data, extras) {
  this.type = type;
  this.data = data;
  var bubbles = false;

  // Is this an external event?
  var external = false;

  // Is this event scheduled to be sent externally?
  var scheduledForExternal = false;

  if (extras === undefined) {
    extras = {};
  }
  if (extras.bubbles === true) {
    bubbles = true;
  }
  if (extras.external === true) {
    external = true;
  }

  /**
   * Prevent this event from bubbling up to parent
   */
  this.preventBubbling = function() {
    bubbles = false;
  };

  /**
   * Get bubbling status
   *
   * @returns {boolean}
   *   true if bubbling false otherwise
   */
  this.getBubbles = function() {
    return bubbles;
  };

  /**
   * Try to schedule an event for externalDispatcher
   *
   * @returns {boolean}
   *   true if external and not already scheduled, otherwise false
   */
  this.scheduleForExternal = function() {
    if (external && !scheduledForExternal) {
      scheduledForExternal = true;
      return true;
    }
    return false;
  };
};

/**
 * Callback type for event listeners.
 *
 * @callback H5P.EventCallback
 * @param {H5P.Event} event
 */

H5P.EventDispatcher = (function () {

  /**
   * The base of the event system.
   * Inherit this class if you want your H5P to dispatch events.
   *
   * @class
   * @memberof H5P
   */
  function EventDispatcher() {
    var self = this;

    /**
     * Keep track of listeners for each event.
     *
     * @private
     * @type {Object}
     */
    var triggers = {};

    /**
     * Add new event listener.
     *
     * @throws {TypeError}
     *   listener must be a function
     * @param {string} type
     *   Event type
     * @param {H5P.EventCallback} listener
     *   Event listener
     * @param {Object} [thisArg]
     *   Optionally specify the this value when calling listener.
     */
    this.on = function (type, listener, thisArg) {
      if (typeof listener !== 'function') {
        throw TypeError('listener must be a function');
      }

      // Trigger event before adding to avoid recursion
      self.trigger('newListener', {'type': type, 'listener': listener});

      var trigger = {'listener': listener, 'thisArg': thisArg};
      if (!triggers[type]) {
        // First
        triggers[type] = [trigger];
      }
      else {
        // Append
        triggers[type].push(trigger);
      }
    };

    /**
     * Add new event listener that will be fired only once.
     *
     * @throws {TypeError}
     *   listener must be a function
     * @param {string} type
     *   Event type
     * @param {H5P.EventCallback} listener
     *   Event listener
     * @param {Object} thisArg
     *   Optionally specify the this value when calling listener.
     */
    this.once = function (type, listener, thisArg) {
      if (!(listener instanceof Function)) {
        throw TypeError('listener must be a function');
      }

      var once = function (event) {
        self.off(event.type, once);
        listener.call(this, event);
      };

      self.on(type, once, thisArg);
    };

    /**
     * Remove event listener.
     * If no listener is specified, all listeners will be removed.
     *
     * @throws {TypeError}
     *   listener must be a function
     * @param {string} type
     *   Event type
     * @param {H5P.EventCallback} listener
     *   Event listener
     */
    this.off = function (type, listener) {
      if (listener !== undefined && !(listener instanceof Function)) {
        throw TypeError('listener must be a function');
      }

      if (triggers[type] === undefined) {
        return;
      }

      if (listener === undefined) {
        // Remove all listeners
        delete triggers[type];
        self.trigger('removeListener', type);
        return;
      }

      // Find specific listener
      for (var i = 0; i < triggers[type].length; i++) {
        if (triggers[type][i].listener === listener) {
          triggers[type].splice(i, 1);
          self.trigger('removeListener', type, {'listener': listener});
          break;
        }
      }

      // Clean up empty arrays
      if (!triggers[type].length) {
        delete triggers[type];
      }
    };

    /**
     * Try to call all event listeners for the given event type.
     *
     * @private
     * @param {string} Event type
     */
    var call = function (type, event) {
      if (triggers[type] === undefined) {
        return;
      }

      // Clone array (prevents triggers from being modified during the event)
      var handlers = triggers[type].slice();

      // Call all listeners
      for (var i = 0; i < handlers.length; i++) {
        var trigger = handlers[i];
        var thisArg = (trigger.thisArg ? trigger.thisArg : this);
        trigger.listener.call(thisArg, event);
      }
    };

    /**
     * Dispatch event.
     *
     * @param {string|H5P.Event} event
     *   Event object or event type as string
     * @param {*} [eventData]
     *   Custom event data(used when event type as string is used as first
     *   argument).
     * @param {Object} [extras]
     * @param {boolean} [extras.bubbles]
     * @param {boolean} [extras.external]
     */
    this.trigger = function (event, eventData, extras) {
      if (event === undefined) {
        return;
      }
      if (event instanceof String || typeof event === 'string') {
        event = new H5P.Event(event, eventData, extras);
      }
      else if (eventData !== undefined) {
        event.data = eventData;
      }

      // Check to see if this event should go externally after all triggering and bubbling is done
      var scheduledForExternal = event.scheduleForExternal();

      // Call all listeners
      call.call(this, event.type, event);

      // Call all * listeners
      call.call(this, '*', event);

      // Bubble
      if (event.getBubbles() && self.parent instanceof H5P.EventDispatcher &&
          (self.parent.trigger instanceof Function || typeof self.parent.trigger === 'function')) {
        self.parent.trigger(event);
      }

      if (scheduledForExternal) {
        H5P.externalDispatcher.trigger.call(this, event);
      }
    };
  }

  return EventDispatcher;
})();
;var H5P = H5P || {};

/**
 * Used for xAPI events.
 *
 * @class
 * @extends H5P.Event
 */
H5P.XAPIEvent = function () {
  H5P.Event.call(this, 'xAPI', {'statement': {}}, {bubbles: true, external: true});
};

H5P.XAPIEvent.prototype = Object.create(H5P.Event.prototype);
H5P.XAPIEvent.prototype.constructor = H5P.XAPIEvent;

/**
 * Set scored result statements.
 *
 * @param {number} score
 * @param {number} maxScore
 * @param {object} instance
 * @param {boolean} completion
 * @param {boolean} success
 */
H5P.XAPIEvent.prototype.setScoredResult = function (score, maxScore, instance, completion, success) {
  this.data.statement.result = {};
  
  if (typeof score !== 'undefined') {
    if (typeof maxScore === 'undefined') {
      this.data.statement.result.score = {'raw': score};
    }
    else {
      this.data.statement.result.score = {
        'min': 0,
        'max': maxScore,
        'raw': score
      };
      if (maxScore > 0) {
        this.data.statement.result.score.scaled = Math.round(score / maxScore * 10000) / 10000;
      }
    }
  }
  
  if (typeof completion === 'undefined') {
    this.data.statement.result.completion = (this.getVerb() === 'completed' || this.getVerb() === 'answered');
  }
  else {
    this.data.statement.result.completion = completion;
  }
  
  if (typeof success !== 'undefined') {
    this.data.statement.result.success = success;
  }
  
  if (instance && instance.activityStartTime) {
    var duration = Math.round((Date.now() - instance.activityStartTime ) / 10) / 100;
    // xAPI spec allows a precision of 0.01 seconds
    
    this.data.statement.result.duration = 'PT' + duration + 'S';
  }
};

/**
 * Set a verb.
 *
 * @param {string} verb
 *   Verb in short form, one of the verbs defined at
 *   {@link http://adlnet.gov/expapi/verbs/|ADL xAPI Vocabulary}
 *
 */
H5P.XAPIEvent.prototype.setVerb = function (verb) {
  if (H5P.jQuery.inArray(verb, H5P.XAPIEvent.allowedXAPIVerbs) !== -1) {
    this.data.statement.verb = {
      'id': 'http://adlnet.gov/expapi/verbs/' + verb,
      'display': {
        'en-US': verb
      }
    };
  }
  else if (verb.id !== undefined) {
    this.data.statement.verb = verb;
  }
};

/**
 * Get the statements verb id.
 *
 * @param {boolean} full
 *   if true the full verb id prefixed by http://adlnet.gov/expapi/verbs/
 *   will be returned
 * @returns {string}
 *   Verb or null if no verb with an id has been defined
 */
H5P.XAPIEvent.prototype.getVerb = function (full) {
  var statement = this.data.statement;
  if ('verb' in statement) {
    if (full === true) {
      return statement.verb;
    }
    return statement.verb.id.slice(31);
  }
  else {
    return null;
  }
};

/**
 * Set the object part of the statement.
 *
 * The id is found automatically (the url to the content)
 *
 * @param {Object} instance
 *   The H5P instance
 */
H5P.XAPIEvent.prototype.setObject = function (instance) {
  if (instance.contentId) {
    this.data.statement.object = {
      'id': this.getContentXAPIId(instance),
      'objectType': 'Activity',
      'definition': {
        'extensions': {
          'http://h5p.org/x-api/h5p-local-content-id': instance.contentId
        }
      }
    };
    if (instance.subContentId) {
      this.data.statement.object.definition.extensions['http://h5p.org/x-api/h5p-subContentId'] = instance.subContentId;
      // Don't set titles on main content, title should come from publishing platform
      if (typeof instance.getTitle === 'function') {
        this.data.statement.object.definition.name = {
          "en-US": instance.getTitle()
        };
      }
    }
    else {
      if (H5PIntegration && H5PIntegration.contents && H5PIntegration.contents['cid-' + instance.contentId].title) {
        this.data.statement.object.definition.name = {
          "en-US": H5P.createTitle(H5PIntegration.contents['cid-' + instance.contentId].title)
        };
      }
    }
  }
};

/**
 * Set the context part of the statement.
 *
 * @param {Object} instance
 *   The H5P instance
 */
H5P.XAPIEvent.prototype.setContext = function (instance) {
  if (instance.parent && (instance.parent.contentId || instance.parent.subContentId)) {
    var parentId = instance.parent.subContentId === undefined ? instance.parent.contentId : instance.parent.subContentId;
    this.data.statement.context = {
      "contextActivities": {
        "parent": [
          {
            "id": this.getContentXAPIId(instance.parent),
            "objectType": "Activity"
          }
        ]
      }
    };
  }
  if (instance.libraryInfo) {
    if (this.data.statement.context === undefined) {
      this.data.statement.context = {"contextActivities":{}};
    }
    this.data.statement.context.contextActivities.category = [
      {
        "id": "http://h5p.org/libraries/" + instance.libraryInfo.versionedNameNoSpaces,
        "objectType": "Activity"
      }
    ];
  }
};

/**
 * Set the actor. Email and name will be added automatically.
 */
H5P.XAPIEvent.prototype.setActor = function () {
  if (H5PIntegration.user !== undefined) {
    this.data.statement.actor = {
      'name': H5PIntegration.user.name,
      'mbox': 'mailto:' + H5PIntegration.user.mail,
      'objectType': 'Agent'
    };
  }
  else {
    var uuid;
    if (localStorage.H5PUserUUID) {
      uuid = localStorage.H5PUserUUID;
    }
    else {
      uuid = H5P.createUUID();
      localStorage.H5PUserUUID = uuid;
    }
    this.data.statement.actor = {
      'account': {
        'name': uuid,
        'homePage': H5PIntegration.siteUrl
      },
      'objectType': 'Agent'
    };
  }
};

/**
 * Get the max value of the result - score part of the statement
 *
 * @returns {number}
 *   The max score, or null if not defined
 */
H5P.XAPIEvent.prototype.getMaxScore = function() {
  return this.getVerifiedStatementValue(['result', 'score', 'max']);
};

/**
 * Get the raw value of the result - score part of the statement
 *
 * @returns {number}
 *   The score, or null if not defined
 */
H5P.XAPIEvent.prototype.getScore = function() {
  return this.getVerifiedStatementValue(['result', 'score', 'raw']);
};

/**
 * Get content xAPI ID.
 *
 * @param {Object} instance
 *   The H5P instance
 */
H5P.XAPIEvent.prototype.getContentXAPIId = function (instance) {
  var xAPIId;
  if (instance.contentId && H5PIntegration && H5PIntegration.contents) {
    xAPIId =  H5PIntegration.contents['cid-' + instance.contentId].url;
    if (instance.subContentId) {
      xAPIId += '?subContentId=' +  instance.subContentId;
    }
  }
  return xAPIId;
};

/**
 * Figure out if a property exists in the statement and return it
 *
 * @param {string[]} keys
 *   List describing the property we're looking for. For instance
 *   ['result', 'score', 'raw'] for result.score.raw
 * @returns {*}
 *   The value of the property if it is set, null otherwise.
 */
H5P.XAPIEvent.prototype.getVerifiedStatementValue = function(keys) {
  var val = this.data.statement;
  for (var i = 0; i < keys.length; i++) {
    if (val[keys[i]] === undefined) {
      return null;
    }
    val = val[keys[i]];
  }
  return val;
};

/**
 * List of verbs defined at {@link http://adlnet.gov/expapi/verbs/|ADL xAPI Vocabulary}
 *
 * @type Array
 */
H5P.XAPIEvent.allowedXAPIVerbs = [
  'answered',
  'asked',
  'attempted',
  'attended',
  'commented',
  'completed',
  'exited',
  'experienced',
  'failed',
  'imported',
  'initialized',
  'interacted',
  'launched',
  'mastered',
  'passed',
  'preferred',
  'progressed',
  'registered',
  'responded',
  'resumed',
  'scored',
  'shared',
  'suspended',
  'terminated',
  'voided'
];
;var H5P = H5P || {};

/**
 * The external event dispatcher. Others, outside of H5P may register and
 * listen for H5P Events here.
 *
 * @type {H5P.EventDispatcher}
 */
H5P.externalDispatcher = new H5P.EventDispatcher();

// EventDispatcher extensions

/**
 * Helper function for triggering xAPI added to the EventDispatcher.
 *
 * @param {string} verb
 *   The short id of the verb we want to trigger
 * @param {Oject} [extra]
 *   Extra properties for the xAPI statement
 */
H5P.EventDispatcher.prototype.triggerXAPI = function (verb, extra) {
  this.trigger(this.createXAPIEventTemplate(verb, extra));
};

/**
 * Helper function to create event templates added to the EventDispatcher.
 *
 * Will in the future be used to add representations of the questions to the
 * statements.
 *
 * @param {string} verb
 *   Verb id in short form
 * @param {Object} [extra]
 *   Extra values to be added to the statement
 * @returns {H5P.XAPIEvent}
 *   Instance
 */
H5P.EventDispatcher.prototype.createXAPIEventTemplate = function (verb, extra) {
  var event = new H5P.XAPIEvent();

  event.setActor();
  event.setVerb(verb);
  if (extra !== undefined) {
    for (var i in extra) {
      event.data.statement[i] = extra[i];
    }
  }
  if (!('object' in event.data.statement)) {
    event.setObject(this);
  }
  if (!('context' in event.data.statement)) {
    event.setContext(this);
  }
  return event;
};

/**
 * Helper function to create xAPI completed events
 *
 * DEPRECATED - USE triggerXAPIScored instead
 *
 * @deprecated
 *   since 1.5, use triggerXAPIScored instead.
 * @param {number} score
 *   Will be set as the 'raw' value of the score object
 * @param {number} maxScore
 *   will be set as the "max" value of the score object
 * @param {boolean} success
 *   will be set as the "success" value of the result object
 */
H5P.EventDispatcher.prototype.triggerXAPICompleted = function (score, maxScore, success) {
  this.triggerXAPIScored(score, maxScore, 'completed', true, success);
};

/**
 * Helper function to create scored xAPI events
 *
 * @param {number} score
 *   Will be set as the 'raw' value of the score object
 * @param {number} maxScore
 *   Will be set as the "max" value of the score object
 * @param {string} verb
 *   Short form of adl verb
 * @param {boolean} completion
 *   Is this a statement from a completed activity?
 * @param {boolean} success
 *   Is this a statement from an activity that was done successfully?
 */
H5P.EventDispatcher.prototype.triggerXAPIScored = function (score, maxScore, verb, completion, success) {
	
	if(verb == "answered" || verb == "completed") 
	{
		return ;
	}
		
  var event = this.createXAPIEventTemplate(verb);
  event.setScoredResult(score, maxScore, this, completion, success);
  this.trigger(event);
};

H5P.EventDispatcher.prototype.setActivityStarted = function() {
  if (this.activityStartTime === undefined) {
    // Don't trigger xAPI events in the editor
    if (this.contentId !== undefined &&
        H5PIntegration.contents !== undefined &&
        H5PIntegration.contents['cid-' + this.contentId] !== undefined) {
      this.triggerXAPI('attempted');
    }
    this.activityStartTime = Date.now();
  }
};

/**
 * Internal H5P function listening for xAPI completed events and stores scores
 *
 * @param {H5P.XAPIEvent} event
 */
H5P.xAPICompletedListener = function (event) {
  if ((event.getVerb() === 'completed' || event.getVerb() === 'answered') && !event.getVerifiedStatementValue(['context', 'contextActivities', 'parent'])) {
    var score = event.getScore();
    var maxScore = event.getMaxScore();
    var contentId = event.getVerifiedStatementValue(['object', 'definition', 'extensions', 'http://h5p.org/x-api/h5p-local-content-id']);
    H5P.setFinished(contentId, score, maxScore);
  }
};
;/**
 * H5P.ContentType is a base class for all content types. Used by newRunnable()
 *
 * Functions here may be overridable by the libraries. In special cases,
 * it is also possible to override H5P.ContentType on a global level.
 * */
H5P.ContentType = function (isRootLibrary, library) {

  function ContentType() {};

  // Inherit from EventDispatcher.
  ContentType.prototype = new H5P.EventDispatcher();

  /**
   * Is library standalone or not? Not beeing standalone, means it is
   * included in another library
   *
   * @method isStandalone
   * @return {Boolean}
   */
  ContentType.prototype.isRoot = function () {
    return isRootLibrary;
  };

  /**
   * Returns the file path of a file in the current library
   * @method getLibraryFilePath
   * @param  {string} filePath The path to the file relative to the library folder
   * @return {string} The full path to the file
   */
  ContentType.prototype.getLibraryFilePath = function (filePath) {
    return H5P.getLibraryPath(this.libraryInfo.versionedNameNoSpaces) + '/' + filePath;
  };

  return ContentType;
};
;/*global H5P*/
H5P.ConfirmationDialog = (function (EventDispatcher) {
  "use strict";

  /**
   * Create a confirmation dialog
   *
   * @param [options] Options for confirmation dialog
   * @param [options.instance] Instance that uses confirmation dialog
   * @param [options.headerText] Header text
   * @param [options.dialogText] Dialog text
   * @param [options.cancelText] Cancel dialog button text
   * @param [options.confirmText] Confirm dialog button text
   * @constructor
   */
  function ConfirmationDialog(options) {
    EventDispatcher.call(this);
    var self = this;

    // Make sure confirmation dialogs have unique id
    H5P.ConfirmationDialog.uniqueId += 1;
    var uniqueId = H5P.ConfirmationDialog.uniqueId;

    // Default options
    options = options || {};
    options.headerText = options.headerText || H5P.t('confirmDialogHeader');
    options.dialogText = options.dialogText || H5P.t('confirmDialogBody');
    options.cancelText = options.cancelText || H5P.t('cancelLabel');
    options.confirmText = options.confirmText || H5P.t('confirmLabel');

    /**
     * Handle confirming event
     * @param {Event} e
     */
    function dialogConfirmed(e) {
      self.hide();
      self.trigger('confirmed');
      e.preventDefault();
    }

    /**
     * Handle dialog canceled
     * @param {Event} e
     */
    function dialogCanceled(e) {
      self.hide();
      self.trigger('canceled');
      e.preventDefault();
    }

    /**
     * Flow focus to element
     * @param {HTMLElement} element Next element to be focused
     * @param {Event} e Original tab event
     */
    function flowTo(element, e) {
      element.focus();
      e.preventDefault();
    }

    // Offset of exit button
    var exitButtonOffset = 2 * 16;
    var shadowOffset = 8;

    // Determine if we are too large for our container and must resize
    var resizeIFrame = false;

    // Create background
    var popupBackground = document.createElement('div');
    popupBackground.classList
      .add('h5p-confirmation-dialog-background', 'hidden', 'hiding');

    // Create outer popup
    var popup = document.createElement('div');
    popup.classList.add('h5p-confirmation-dialog-popup', 'hidden');
    popup.setAttribute('role', 'dialog');
    popup.setAttribute('aria-labelledby', 'h5p-confirmation-dialog-dialog-text-' + uniqueId);
    popupBackground.appendChild(popup);
    popup.addEventListener('keydown', function (e) {
      if (e.which === 27) {// Esc key
        // Exit dialog
        dialogCanceled(e);
      }
    });

    // Popup header
    var header = document.createElement('div');
    header.classList.add('h5p-confirmation-dialog-header');
    popup.appendChild(header);

    // Header text
    var headerText = document.createElement('div');
    headerText.classList.add('h5p-confirmation-dialog-header-text');
    headerText.innerHTML = options.headerText;
    header.appendChild(headerText);

    // Popup body
    var body = document.createElement('div');
    body.classList.add('h5p-confirmation-dialog-body');
    popup.appendChild(body);

    // Popup text
    var text = document.createElement('div');
    text.classList.add('h5p-confirmation-dialog-text');
    text.innerHTML = options.dialogText;
    text.id = 'h5p-confirmation-dialog-dialog-text-' + uniqueId;
    body.appendChild(text);

    // Popup buttons
    var buttons = document.createElement('div');
    buttons.classList.add('h5p-confirmation-dialog-buttons');
    body.appendChild(buttons);

    // Cancel button
    var cancelButton = document.createElement('button');
    cancelButton.classList.add('h5p-core-cancel-button');
    cancelButton.textContent = options.cancelText;

    // Confirm button
    var confirmButton = document.createElement('button');
    confirmButton.classList.add('h5p-core-button',
      'h5p-confirmation-dialog-confirm-button');
    confirmButton.textContent = options.confirmText;

    // Exit button
    var exitButton = document.createElement('button');
    exitButton.classList.add('h5p-confirmation-dialog-exit');
    exitButton.setAttribute('aria-hidden', 'true');
    exitButton.tabIndex = -1;
    exitButton.title = options.cancelText;

    // Cancel handler
    cancelButton.addEventListener('click', dialogCanceled);
    cancelButton.addEventListener('keydown', function (e) {
      if (e.which === 32) { // Space
        dialogCanceled(e);
      }
      else if (e.which === 9 && e.shiftKey) { // Shift-tab
        flowTo(confirmButton, e);
      }
    });
    buttons.appendChild(cancelButton);

    // Confirm handler
    confirmButton.addEventListener('click', dialogConfirmed);
    confirmButton.addEventListener('keydown', function (e) {
      if (e.which === 32) { // Space
        dialogConfirmed(e);
      }
      else if (e.which === 9 && !e.shiftKey) { // Tab
        flowTo(cancelButton, e);
      }
    });
    buttons.appendChild(confirmButton);

    // Exit handler
    exitButton.addEventListener('click', dialogCanceled);
    exitButton.addEventListener('keydown', function (e) {
      if (e.which === 32) { // Space
        dialogCanceled(e);
      }
    });
    popup.appendChild(exitButton);

    // Wrapper element
    var wrapperElement;

    // Focus capturing
    var focusPredator;

    // Maintains hidden state of elements
    var wrapperSiblingsHidden = [];
    var popupSiblingsHidden = [];

    // Element with focus before dialog
    var previouslyFocused;

    /**
     * Set parent of confirmation dialog
     * @param {HTMLElement} wrapper
     * @returns {H5P.ConfirmationDialog}
     */
    this.appendTo = function (wrapper) {
      wrapperElement = wrapper;
      return this;
    };

    /**
     * Capture the focus element, send it to confirmation button
     * @param {Event} e Original focus event
     */
    var captureFocus = function (e) {
      if (!popupBackground.contains(e.target)) {
        e.preventDefault();
        confirmButton.focus();
      }
    };

    /**
     * Hide siblings of element from assistive technology
     *
     * @param {HTMLElement} element
     * @returns {Array} The previous hidden state of all siblings
     */
    var hideSiblings = function (element) {
      var hiddenSiblings = [];
      var siblings = element.parentNode.children;
      var i;
      for (i = 0; i < siblings.length; i += 1) {
        // Preserve hidden state
        hiddenSiblings[i] = siblings[i].getAttribute('aria-hidden') ?
          true : false;

        if (siblings[i] !== element) {
          siblings[i].setAttribute('aria-hidden', true);
        }
      }
      return hiddenSiblings;
    };

    /**
     * Restores assistive technology state of element's siblings
     *
     * @param {HTMLElement} element
     * @param {Array} hiddenSiblings Hidden state of all siblings
     */
    var restoreSiblings = function (element, hiddenSiblings) {
      var siblings = element.parentNode.children;
      var i;
      for (i = 0; i < siblings.length; i += 1) {
        if (siblings[i] !== element && !hiddenSiblings[i]) {
          siblings[i].removeAttribute('aria-hidden');
        }
      }
    };

    /**
     * Start capturing focus of parent and send it to dialog
     */
    var startCapturingFocus = function () {
      focusPredator = wrapperElement.parentNode || wrapperElement;
      focusPredator.addEventListener('focus', captureFocus, true);
    };

    /**
     * Clean up event listener for capturing focus
     */
    var stopCapturingFocus = function () {
      focusPredator.removeAttribute('aria-hidden');
      focusPredator.removeEventListener('focus', captureFocus, true);
    };

    /**
     * Hide siblings in underlay from assistive technologies
     */
    var disableUnderlay = function () {
      wrapperSiblingsHidden = hideSiblings(wrapperElement);
      popupSiblingsHidden = hideSiblings(popupBackground);
    };

    /**
     * Restore state of underlay for assistive technologies
     */
    var restoreUnderlay = function () {
      restoreSiblings(wrapperElement, wrapperSiblingsHidden);
      restoreSiblings(popupBackground, popupSiblingsHidden);
    };

    /**
     * Fit popup to container. Makes sure it doesn't overflow.
     * @params {number} [offsetTop] Offset of popup
     */
    var fitToContainer = function (offsetTop) {
      var popupOffsetTop = parseInt(popup.style.top, 10);
      if (offsetTop) {
        popupOffsetTop = offsetTop;
      }

      // Overflows height
      if (popupOffsetTop + popup.offsetHeight > wrapperElement.offsetHeight) {
        popupOffsetTop = wrapperElement.offsetHeight - popup.offsetHeight - shadowOffset;
      }

      if (popupOffsetTop - exitButtonOffset <= 0) {
        popupOffsetTop = exitButtonOffset + shadowOffset;

        // We are too big and must resize
        resizeIFrame = true;
      }
      popup.style.top = popupOffsetTop + 'px';
    };

    /**
     * Show confirmation dialog
     * @params {number} offsetTop Offset top
     * @returns {H5P.ConfirmationDialog}
     */
    this.show = function (offsetTop) {
      // Capture focused item
      previouslyFocused = document.activeElement;
      wrapperElement.appendChild(popupBackground);
      startCapturingFocus();
      disableUnderlay();
      popupBackground.classList.remove('hidden');
      fitToContainer(offsetTop);
      setTimeout(function () {
        popup.classList.remove('hidden');
        popupBackground.classList.remove('hiding');

        setTimeout(function () {
          // Focus confirm button
          confirmButton.focus();

          // Resize iFrame if necessary
          if (resizeIFrame && options.instance) {
            var minHeight = parseInt(popup.offsetHeight, 10) +
              exitButtonOffset + (2 * shadowOffset);
            wrapperElement.style.minHeight = minHeight + 'px';
            options.instance.trigger('resize');
            resizeIFrame = false;
          }
        }, 100);
      }, 0);

      return this;
    };

    /**
     * Hide confirmation dialog
     * @returns {H5P.ConfirmationDialog}
     */
    this.hide = function () {
      popupBackground.classList.add('hiding');
      popup.classList.add('hidden');

      // Restore focus
      stopCapturingFocus();
      previouslyFocused.focus();
      restoreUnderlay();
      setTimeout(function () {
        popupBackground.classList.add('hidden');
        wrapperElement.removeChild(popupBackground);
      }, 100);

      return this;
    };
  }

  ConfirmationDialog.prototype = Object.create(EventDispatcher.prototype);
  ConfirmationDialog.prototype.constructor = ConfirmationDialog;

  return ConfirmationDialog;

}(H5P.EventDispatcher));

H5P.ConfirmationDialog.uniqueId = -1;
;H5P.AdvancedText = (function ($) {

  /**
   * A simple library for displaying text with advanced styling.
   *
   * @class H5P.AdvancedText
   * @param {Object} parameters
   * @param {Object} [parameters.text='New text']
   * @param {number} id
   */
  function AdvancedText(parameters, id) {
    var self = this;

    var html = (parameters.text === undefined ? '<em>New text</em>' : parameters.text);

    /**
     * Wipe container and add text html.
     *
     * @alias H5P.AdvancedText#attach
     * @param {H5P.jQuery} $container
     */
    self.attach = function ($container) {
      $container.addClass('h5p-advanced-text').html(html);
    };
  }

  return AdvancedText;

})(H5P.jQuery);
;var H5P = H5P || {};

/**
 * H5P Link Library Module.
 */
H5P.Link = (function ($) {

  /**
   * Link constructor.
   *
   * @param {Object} parameters
   * @param {Number} id
   */
  function Link(parameters, id) {
    // Add default parameters
    parameters = $.extend(true, {
      title: 'New link',
      linkWidget: {
        protocol: '',
        url: ''
      }
    }, parameters);

    var url = '';
    //h5pcustomize - commented out from appending protocol attribute. It is customized based on e1 style and 
    //values will not be populated for parameters.linkWidget.protocol
    /*if (parameters.linkWidget.protocol !== 'other') {
    
       url += parameters.linkWidget.protocol;
    }*/
    url += parameters.linkWidget.url;
    

    /**
     * Public. Attach.
     *
     * @param {jQuery} $container
     */
    this.attach = function ($container) {
      var sanitizedUrl = sanitizeUrlProtocol(url);
      $container.addClass('h5p-link').html('<a href="' + sanitizedUrl + '" target="_blank">' + parameters.title + '</a>');
    };

    /**
     * Return url
     *
     * @returns {string}
     */
    this.getUrl = function () {
      return url;
    };

    /**
     * Private. Remove illegal url protocols from uri
     */
    var sanitizeUrlProtocol = function(uri) {
      var allowedProtocols = ['http', 'https', 'ftp', 'irc', 'mailto', 'news', 'nntp', 'rtsp', 'sftp', 'ssh', 'tel', 'telnet', 'webcal'];

      var first = true;
      var before = '';
      while (first || uri != before) {
        first = false;
        before = uri;
        var colonPos = uri.indexOf(':');
        if (colonPos > 0) {
          // We found a possible protocol
          var protocol = uri.substr(0, colonPos);
          // If the colon is preceeded by a hash, slash or question mark it isn't a protocol
          if (protocol.match(/[/?#]/g)) {
            break;
          }
          // Is this a forbidden protocol?
          if (allowedProtocols.indexOf(protocol.toLowerCase()) == -1) {
            // If illegal, remove the protocol...
            uri = uri.substr(colonPos + 1);
          }
        }
      }
      return uri;
    };
  }

  return Link;
})(H5P.jQuery);
;var H5P = H5P || {};

/**
 * Constructor.
 *
 * @param {Object} params Options for this library.
 * @param {Number} id Content identifier
 * @returns {undefined}
 */
(function ($) {
  H5P.Image = function (params, id) {
    H5P.EventDispatcher.call(this);
    if (params.file === undefined || !(params.file instanceof Object)) {
      this.source = 'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAARgAAACaCAIAAAD948C8AAAACXBIWXMAAAsTAAALEwEAmpwYAAAAB3RJTUUH3QQQCBoH57dqzgAADlJJREFUeNrtnVtsHNd5x89lZmdnr7zrSlkKA6Vx3DZuDVRF0rzUKGAEaYMESIIgAfIQJOlrgfSlL3npU4A+NwkKF66LtkGQpoqNtIXbpLFjS45sSRRlWRQlilySu9z7ZXYuO3PO6cOSFEVSFHfI5W3+P62I5XLnDOc78+N3zpkze+g/vP6AhEG9cOr7vzf29+RYMFn8zrX8dwmhBIBQaGE3pNfyf93wJv5k/LtHPQRv5r5/r/pFnApgN7DdbHyv+sXL05eP9PFfnr4Mi8ABi0QIKTvPvTp182HjpSN35A8bL706dbPsPIeTAOweGraPtJFzmTdevPCto3LYb8z+cL75IqofHJaMtMZ888VXp27eKX/9kB/wnfLXX526CYvAIc1Ia2SNB586+zcnU1cP26EWrD/6zcLfNryPoNbBERCpy5D5wR+f+d6J5LXDcJDL7RfeWfxe1XkW9Q2OmEir2Wn2d8d+eHHo3w7q8KarX75V/FbDu4CaBkdYpC46t85l/ucPTv5dOpbbn6NqdcbfL/zVfPNPfZFCHYNjItIaplY6m/n1syP/OGze7kf5FecTH5S/sdD8jBOMomrBsRVpDY05WeP+qdSV8wO/GEtc301RRfv5h/WX8talhjcRSBM1CiIk0mO/AZEx3kzoxXRsPmM8zBhz6dh8Ui/EtXKMW5QKpXhHpNxgpO2fbHXONb1nmt75Vuec7Y91REbt3Qg+AOFzw4H/BoowTwx4YqDmXkR9gCMK/pwDAJEAgEgAQCQAAEQCACIBAJEAgEgAAIgEAEQC4PCiIQTgkCC8YJjJL/zFx9Ze+el/3K1Ixg0NIgHwdKSQn790amQ0seH1rlTlkv2zK3nGD3XrCU07cMB4Deebn5vYbNEaI6OJb35uwms4EAmArbGLrW9/6eNqB3znKx+3iy2IBMBGnEr721/9hBAy8Dc+pFCbH3/5teecSvtwHsvB39gHoolvd772Z+fp43/JKX36Ogav/GJWT8QgEgBESfXnL4wSSQgh613aINJmr5RSlNF/v7pM2eFaOgSjduAA+PRHM/VSu6vKmizdZxsMWe+SUqr75DMXs2/ONCESiDRuzS7P+SvarIlEHxm0zq3NuWytECc+mIBIILqclv5yzu7qQhkl3X+EULbyZL1CW2Ykosj5uF5ARgKRpTVf8wq1bvah9JEnlHb/k3XiPKkXtKKTPdhJnRuESCCKYwytyRxdJwylK0lo7SW640EEVbCSZwcOyagDRAL7h71Qaze9tWTzKB1tlYGeNBT+qIFHSGqxlhgfgkggSulIqdk37m6fcCjtLb3MvnH32W9c6nUriASOMNZCXQTy6ef8zqVQK8Wmxw++p4QpQmCfWHz7ASE7mFQnd/xQSim1+PahmFGAjAQIIaQ5Vy3eXLSLrcRYeuz3z2Se2eOOh9dw3Hpfpm+7dcdrOEbWhEjgIBFecP/1KbtkrTTA8g0r30iMpiY++9we3lFXnsr37xDKU/kznzrgFU3RtIs0gRtMvXJ1zaI17JI19crVwA32ZC9SyNLUUv+OojS1JIWESOBgUFLN/HxSSRXipz3RLvR9Xtw+7AIiga0p3lhwq/Z23Y+qXbyxsAcZ4+Ziv49lH3YBkcDWvf/8b+ee+rb8b+d2eY+39EUzV+v34TRzNekLiAT2m/lfTu/5O7dudC3v0/3h+7ajLcGoXRSxi62dn3bt5VZ3WDzcvip39mmW9ux/39mclLjOWUzTkzEjE48PJuLDSXMwoaVijDGIBHbLwlv3e33/xS98MsSOlFT1B+X9Oagtm3bCF8IXftvb/MEp5kgye24o88xQfCS5e68gUhR7R5vHu5+SwUpWuIuebs0+tHFwym2n3C68nyOEaKY+MDE6/Dtj5nAKIoEdUbq1FG6rs5+e6HkMYL52JGISOH55aqk8tUQISY8Pjn3ybPp0FiKBJ7d/Alm+HWaSQfl2/vSlC0zrrQlU7ud12D7RytVauRohZOCjoyf/cDw+kIBIYCPWUn0322bO9TAHT3SEb3eObqzqM6X6TEkz9dOXLgxdHNv+zRj+jhbFG4v7tu1h7iD11Oqb/+X0jR+8Vbg2J6WESIBIX1j5RviMlG/0dNGztVA7TtErvJeb/NHb+XcfbqkTRIoQu79k2VMJteni8Yvh8vWFyR+9XZpcgkjRpXp3ed9KUEJ6Tfe4RnLxnQc3fvBWa6EOkSKHkqo2U9plIbWZ0g7ng3cs79iH9P7rUzOXJ0UngEgRotPy9rMcp9yOQlStfPPWy1cqd5YhUlSw8vX9LKe1WI9ObHO/vgeRokJturSf5dT3aHdHBYgUlQ7Sbga+H89Ijad2k/LvzgkhIhVhzGyIBHs7w8C3O7GU8cQu+GtTkWrXQaQIsbddf6fc3lIk0Qk+/PH7frsTwQijaReNkYY9TRFbltax3FsvX4mmRRApKuzt3XWbS3Nr9gf/fC3KEUbTLhIjDXveR1JSra2nYpes6Z/eiHiQkZGOP4HT6V+ZTqUNiyBSJPAabp/K9BrO3Z9cR4QhUiRwylY/ygxc/86/vofwoo8UFaw+fJyvlW8W3sshthApSiL14fJoY65KlEJs0bSLCkoq0enDbB1YhIwUIYuO9d11EAn0K/kEju81HLvUspYaR+Uz5SASOHB1iO903KptLdXrDyq7XDYCQKQouaOUb3lWvll/UG7OVREQiAR6IHD8dqFZnS42HlYQDYgEeks+Xt1pzFaWbywc7CpaACIdSX/cql2dLpYmFxENiAR6xms41bvF5euYMQCRQO8IL2jMVhbfedCXK6cAIh17nEp7+Xqufr+MUEAk0HsvSKrmfDX3fzOB6yMaEAn0jPRFdbrY6xKuACKB1Y5QJyh/UMhffYhQQCQQKgsFony7sHRlFqGASCBkX6h2rzj/q3sIBUQCIbGWGjOv3SK4bQcigXD4bW/+V/fWr0IFIBLopS2nVOV2YeE3GJSDSCAsXt2Zee1WZD+zFyKBPaB8O4+rQxAJhCdw/dn/vNNebiIUEAmEpL3cvPezScQBQCQ05wBEOiCUVLk3Z6ofLiMUACKFRPpi5ue37JKFUACIFJLA9e/8y3uiEyAUACKFxLc7t//pXcQBbAaf/Q2LwF5kpP9q/kSQIFCBUEFAAqECSYRQQpBAKimIECpQREkipRKKSEWUIkoppTbNx6SEUkop6T4Yo5wRRgnlVOOEM8o40TjljHBONY1onGoa1TjRNKpzyjWqa0TXqMap1n2iUX31nbpGOSfaalG8+5URxgijlHV32tcWHSwC24k05e7ZGrqKPC7XQUx8TrCUyRImTSVYMs4SJk3EmWlQ02Bxg8Zj1NCpoVNdo3rXYUY5I5RsK6EMxIc/fh/nCohQH8mWli0tQoq9bphimTQfSLNsimUSPJVYVTFG4oX/fegKm3HOJKOK4qQBGGx4IpZsWrKZ3/JnzxPy/KPveKAlrKTZTsRtM+6aMceIeYbeiem+zgPOBGcSPU+IBJ6G0ILWQKM10NjmPUywZCudbKW6vhmuGXMN3dc1X+eCU4m0BpHADpBcbi+b4cRTjUyylUq0k3HbNNy43olpvsYER+sRIoGd4pmuZ7qVk1v05aiimdpAVzOznYg7pu4Zmq+hhwaRQA8oqhpDtcbQFuvtxTwjWx1ccyzmGnonBsEgEuiNjuGVThVKpwobG4quka0MpevZZCtl2omYG+9mMEQMIoFeGopxr3gmXzyzcdBxsDycqQ6kG9mElTScuObrsAsigZ6pjVRqIxtX+Bsqjg5UBlONTMJKGm6cBxpahhAJ9Ex1rFQdK61/xbQTg8WRbHUw1UzHbVP3Yxigh0igZ5yE7ZyfXzo/v/aK3tGHl8cGKkOpRsZsJ6AWRAJh8GN+YXyxMP5o7U3DNUYKJ7pqxW1T83VECSKBnvHi3uL5+cV1WStbHRwujGWrg0krFfOMiKcsiARCsuFiV9wxR5dODpaHU42M4caZYBAJgJ5xTSc3MZubWFm9RvP1Ewunh0oj6XrGcM1j7xVEAn0h0P3FC3OLF+a638Y840Tu9FBppJuvjt+AO3325S+h1kGfzzJFKCFEEaooU4SqpJUcLA+nrFSso1OqhB5IPZC6L7RA6oHQfakHUguE7ktNSC1YfQjFpeSB4lIyqZhUXCiqFFOEKkXV4/ukRFEqKVWUCk4lY5JRwZjQqGAs4CzQVh+c+zoLNOZr3NeZr/FAY90nvsa6Pwo4DzQaaExwFnAqGJOcSkpW52chI4EQVqwqwSRhknBBuSRcUE0Qrfs1IJqg2hM/a8kjpNDnX7PrleLd7/q+6DVEAo9LwhRlkqyIERBNUD0guk9jPo35hGLhNIgEVlSRlMuVjBHrGtKhRodwifBAJLApsXBBNEF1nxo+MTxqetu0tQBEijYr6SWgMZ/GPWp61HTRAINIYLs8QzVB9IAaHRp3acKlBlYHhEjgqalGE8To0LjHEg5NOggJRAI7yzaxDjU9lrJpAtpAJLDDhKMHNO7RpMPSbcIFQgKRwM5yTteclM2yLYwHQCSwY7ig8Q5L2jTbojqGniES2Hni0QNqujTdZtkWogGRQE9RDFjCpRmLZbAeJkQCvY4WxD2WsdhQA8EAEKn3llvKZkN1XAwFEKl3f4wOy1hstIpQAIgUyp+BJhuuIxQAIoVov/l0oMWRfwBECjd+wNJtdqKC2w0ARArbhBuus4EmQgEgUiiFkjY/VaIxH6cCgEghBFIs3eZnCzgDAEQKq1DG4meWUfcAIoWVKGVr55ZQ6wAihVUo5vOzBRr3UOUAIoVtyw3X+VgFlQ0gUvhEpJ1fIBruPwUQKSws3ebjedQxgEi7sGi4xk+gOQcg0i7gYxU2UkPtgv37w30MD2mkBosARNp1vwgDdAAi7a6hKjC6ACDSrrtGJ8qoUXAg/D+bbRxow/jhAQAAAABJRU5ErkJggg==';
    }
    else {
		//h5pcustomize
		
		if($(".h5p-interactivevideo-editor").size() == 0 || $("#h5p-image-radio-button-1:checked").size() > 0)
    		if(params.file != null)
			{
				if(params.file.path.indexOf("^^^") > 0)
					params.file.path =params.file.path.split("^^^")[1];
			}
		
    	
      this.source = H5P.getPath(params.file.path, id);
      this.width = params.file.width;
      this.height = params.file.height;

      // Use new copyright information if available. Fallback to old.
      if (params.file.copyright !== undefined) {
        this.copyright = params.file.copyright;
      }
      else if (params.copyright !== undefined) {
        this.copyright = params.copyright;
      }
    }

    this.alt = params.alt !== undefined ? params.alt : 'New image';

    if (params.title !== undefined) {
      this.title = params.title;
    }
  };

  H5P.Image.prototype = Object.create(H5P.EventDispatcher.prototype);
  H5P.Image.prototype.constructor = H5P.Image;

  /**
   * Wipe out the content of the wrapper and put our HTML in it.
   *
   * @param {jQuery} $wrapper
   * @returns {undefined}
   */
  H5P.Image.prototype.attach = function ($wrapper) {
    var self = this;

    if (self.$img === undefined) {
      self.$img = $('<img>', {
        width: '100%',
        height: '100%',
        src: this.source,
        alt: this.alt,
        title: this.title === undefined ? '' : this.title,
        load: function () {
          self.trigger('loaded');
        }
      });
    }

    $wrapper.addClass('h5p-image').html(self.$img);
  };

  /**
   * Gather copyright information for the current content.
   *
   * @returns {H5P.ContentCopyright}
   */
  H5P.Image.prototype.getCopyrights = function () {
    if (this.copyright === undefined) {
      return;
    }

    var info = new H5P.ContentCopyrights();

    var image = new H5P.MediaCopyright(this.copyright);
    image.setThumbnail(new H5P.Thumbnail(this.source, this.width, this.height));
    info.addMedia(image);

    return info;
  };

  return H5P.Image;
}(H5P.jQuery));
;/*
 * flowplayer.js 3.2.12. The Flowplayer API
 *
 * Copyright 2009-2011 Flowplayer Oy
 *
 * This file is part of Flowplayer.
 *
 * Flowplayer is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Flowplayer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Flowplayer.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Date: ${date}
 * Revision: ${revision}
 */
!function(){function h(p){console.log("$f.fireEvent",[].slice.call(p))}function l(r){if(!r||typeof r!="object"){return r}var p=new r.constructor();for(var q in r){if(r.hasOwnProperty(q)){p[q]=l(r[q])}}return p}function n(u,r){if(!u){return}var p,q=0,s=u.length;if(s===undefined){for(p in u){if(r.call(u[p],p,u[p])===false){break}}}else{for(var t=u[0];q<s&&r.call(t,q,t)!==false;t=u[++q]){}}return u}function c(p){return document.getElementById(p)}function j(r,q,p){if(typeof q!="object"){return r}if(r&&q){n(q,function(s,t){if(!p||typeof t!="function"){r[s]=t}})}return r}function o(t){var r=t.indexOf(".");if(r!=-1){var q=t.slice(0,r)||"*";var p=t.slice(r+1,t.length);var s=[];n(document.getElementsByTagName(q),function(){if(this.className&&this.className.indexOf(p)!=-1){s.push(this)}});return s}}function g(p){p=p||window.event;if(p.preventDefault){p.stopPropagation();p.preventDefault()}else{p.returnValue=false;p.cancelBubble=true}return false}function k(r,p,q){r[p]=r[p]||[];r[p].push(q)}function e(p){return p.replace(/&amp;/g,"%26").replace(/&/g,"%26").replace(/=/g,"%3D")}function f(){return"_"+(""+Math.random()).slice(2,10)}var i=function(u,s,t){var r=this,q={},v={};r.index=s;if(typeof u=="string"){u={url:u}}j(this,u,true);n(("Begin*,Start,Pause*,Resume*,Seek*,Stop*,Finish*,LastSecond,Update,BufferFull,BufferEmpty,BufferStop").split(","),function(){var w="on"+this;if(w.indexOf("*")!=-1){w=w.slice(0,w.length-1);var x="onBefore"+w.slice(2);r[x]=function(y){k(v,x,y);return r}}r[w]=function(y){k(v,w,y);return r};if(s==-1){if(r[x]){t[x]=r[x]}if(r[w]){t[w]=r[w]}}});j(this,{onCuepoint:function(y,x){if(arguments.length==1){q.embedded=[null,y];return r}if(typeof y=="number"){y=[y]}var w=f();q[w]=[y,x];if(t.isLoaded()){t._api().fp_addCuepoints(y,s,w)}return r},update:function(x){j(r,x);if(t.isLoaded()){t._api().fp_updateClip(x,s)}var w=t.getConfig();var y=(s==-1)?w.clip:w.playlist[s];j(y,x,true)},_fireEvent:function(w,z,x,B){if(w=="onLoad"){n(q,function(C,D){if(D[0]){t._api().fp_addCuepoints(D[0],s,C)}});return false}B=B||r;if(w=="onCuepoint"){var A=q[z];if(A){return A[1].call(t,B,x)}}if(z&&"onBeforeBegin,onMetaData,onStart,onUpdate,onResume".indexOf(w)!=-1){j(B,z);if(z.metaData){if(!B.duration){B.duration=z.metaData.duration}else{B.fullDuration=z.metaData.duration}}}var y=true;n(v[w],function(){y=this.call(t,B,z,x)});return y}});if(u.onCuepoint){var p=u.onCuepoint;r.onCuepoint.apply(r,typeof p=="function"?[p]:p);delete u.onCuepoint}n(u,function(w,x){if(typeof x=="function"){k(v,w,x);delete u[w]}});if(s==-1){t.onCuepoint=this.onCuepoint}};var m=function(q,s,r,u){var p=this,t={},v=false;if(u){j(t,u)}n(s,function(w,x){if(typeof x=="function"){t[w]=x;delete s[w]}});j(this,{animate:function(z,A,y){if(!z){return p}if(typeof A=="function"){y=A;A=500}if(typeof z=="string"){var x=z;z={};z[x]=A;A=500}if(y){var w=f();t[w]=y}if(A===undefined){A=500}s=r._api().fp_animate(q,z,A,w);return p},css:function(x,y){if(y!==undefined){var w={};w[x]=y;x=w}s=r._api().fp_css(q,x);j(p,s);return p},show:function(){this.display="block";r._api().fp_showPlugin(q);return p},hide:function(){this.display="none";r._api().fp_hidePlugin(q);return p},toggle:function(){this.display=r._api().fp_togglePlugin(q);return p},fadeTo:function(z,y,x){if(typeof y=="function"){x=y;y=500}if(x){var w=f();t[w]=x}this.display=r._api().fp_fadeTo(q,z,y,w);this.opacity=z;return p},fadeIn:function(x,w){return p.fadeTo(1,x,w)},fadeOut:function(x,w){return p.fadeTo(0,x,w)},getName:function(){return q},getPlayer:function(){return r},_fireEvent:function(x,w,y){if(x=="onUpdate"){var A=r._api().fp_getPlugin(q);if(!A){return}j(p,A);delete p.methods;if(!v){n(A.methods,function(){var C=""+this;p[C]=function(){var D=[].slice.call(arguments);var E=r._api().fp_invoke(q,C,D);return E==="undefined"||E===undefined?p:E}});v=true}}var B=t[x];if(B){var z=B.apply(p,w);if(x.slice(0,1)=="_"){delete t[x]}return z}return p}})};function b(r,H,u){var x=this,w=null,E=false,v,t,G=[],z={},y={},F,s,q,D,p,B;j(x,{id:function(){return F},isLoaded:function(){return(w!==null&&w.fp_play!==undefined&&!E)},getParent:function(){return r},hide:function(I){if(I){r.style.height="0px"}if(x.isLoaded()){w.style.height="0px"}return x},show:function(){r.style.height=B+"px";if(x.isLoaded()){w.style.height=p+"px"}return x},isHidden:function(){return x.isLoaded()&&parseInt(w.style.height,10)===0},load:function(K){if(!x.isLoaded()&&x._fireEvent("onBeforeLoad")!==false){var I=function(){if(v&&!flashembed.isSupported(H.version)){r.innerHTML=""}if(K){K.cached=true;k(y,"onLoad",K)}flashembed(r,H,{config:u})};var J=0;n(a,function(){this.unload(function(L){if(++J==a.length){I()}})})}return x},unload:function(K){if(v.replace(/\s/g,"")!==""){if(x._fireEvent("onBeforeUnload")===false){if(K){K(false)}return x}E=true;try{if(w){if(w.fp_isFullscreen()){w.fp_toggleFullscreen()}w.fp_close();x._fireEvent("onUnload")}}catch(I){}var J=function(){w=null;r.innerHTML=v;E=false;if(K){K(true)}};if(/WebKit/i.test(navigator.userAgent)&&!/Chrome/i.test(navigator.userAgent)){setTimeout(J,0)}else{J()}}else{if(K){K(false)}}return x},getClip:function(I){if(I===undefined){I=D}return G[I]},getCommonClip:function(){return t},getPlaylist:function(){return G},getPlugin:function(I){var K=z[I];if(!K&&x.isLoaded()){var J=x._api().fp_getPlugin(I);if(J){K=new m(I,J,x);z[I]=K}}return K},getScreen:function(){return x.getPlugin("screen")},getControls:function(){return x.getPlugin("controls")._fireEvent("onUpdate")},getLogo:function(){try{return x.getPlugin("logo")._fireEvent("onUpdate")}catch(I){}},getPlay:function(){return x.getPlugin("play")._fireEvent("onUpdate")},getConfig:function(I){return I?l(u):u},getFlashParams:function(){return H},loadPlugin:function(L,K,N,M){if(typeof N=="function"){M=N;N={}}var J=M?f():"_";x._api().fp_loadPlugin(L,K,N,J);var I={};I[J]=M;var O=new m(L,null,x,I);z[L]=O;return O},getState:function(){return x.isLoaded()?w.fp_getState():-1},play:function(J,I){var K=function(){if(J!==undefined){x._api().fp_play(J,I)}else{x._api().fp_play()}};if(x.isLoaded()){K()}else{if(E){setTimeout(function(){x.play(J,I)},50)}else{x.load(function(){K()})}}return x},getVersion:function(){var J="flowplayer.js 3.2.12";if(x.isLoaded()){var I=w.fp_getVersion();I.push(J);return I}return J},_api:function(){if(!x.isLoaded()){throw"Flowplayer "+x.id()+" not loaded when calling an API method"}return w},setClip:function(I){n(I,function(J,K){if(typeof K=="function"){k(y,J,K);delete I[J]}else{if(J=="onCuepoint"){$f(r).getCommonClip().onCuepoint(I[J][0],I[J][1])}}});x.setPlaylist([I]);return x},getIndex:function(){return q},bufferAnimate:function(I){w.fp_bufferAnimate(I===undefined||I);return x},_swfHeight:function(){return w.clientHeight}});n(("Click*,Load*,Unload*,Keypress*,Volume*,Mute*,Unmute*,PlaylistReplace,ClipAdd,Fullscreen*,FullscreenExit,Error,MouseOver,MouseOut").split(","),function(){var I="on"+this;if(I.indexOf("*")!=-1){I=I.slice(0,I.length-1);var J="onBefore"+I.slice(2);x[J]=function(K){k(y,J,K);return x}}x[I]=function(K){k(y,I,K);return x}});n(("pause,resume,mute,unmute,stop,toggle,seek,getStatus,getVolume,setVolume,getTime,isPaused,isPlaying,startBuffering,stopBuffering,isFullscreen,toggleFullscreen,reset,close,setPlaylist,addClip,playFeed,setKeyboardShortcutsEnabled,isKeyboardShortcutsEnabled").split(","),function(){var I=this;x[I]=function(K,J){if(!x.isLoaded()){return x}var L=null;if(K!==undefined&&J!==undefined){L=w["fp_"+I](K,J)}else{L=(K===undefined)?w["fp_"+I]():w["fp_"+I](K)}return L==="undefined"||L===undefined?x:L}});x._fireEvent=function(R){if(typeof R=="string"){R=[R]}var S=R[0],P=R[1],N=R[2],M=R[3],L=0;if(u.debug){h(R)}if(!x.isLoaded()&&S=="onLoad"&&P=="player"){w=w||c(s);p=x._swfHeight();n(G,function(){this._fireEvent("onLoad")});n(z,function(T,U){U._fireEvent("onUpdate")});t._fireEvent("onLoad")}if(S=="onLoad"&&P!="player"){return}if(S=="onError"){if(typeof P=="string"||(typeof P=="number"&&typeof N=="number")){P=N;N=M}}if(S=="onContextMenu"){n(u.contextMenu[P],function(T,U){U.call(x)});return}if(S=="onPluginEvent"||S=="onBeforePluginEvent"){var I=P.name||P;var J=z[I];if(J){J._fireEvent("onUpdate",P);return J._fireEvent(N,R.slice(3))}return}if(S=="onPlaylistReplace"){G=[];var O=0;n(P,function(){G.push(new i(this,O++,x))})}if(S=="onClipAdd"){if(P.isInStream){return}P=new i(P,N,x);G.splice(N,0,P);for(L=N+1;L<G.length;L++){G[L].index++}}var Q=true;if(typeof P=="number"&&P<G.length){D=P;var K=G[P];if(K){Q=K._fireEvent(S,N,M)}if(!K||Q!==false){Q=t._fireEvent(S,N,M,K)}}n(y[S],function(){Q=this.call(x,P,N);if(this.cached){y[S].splice(L,1)}if(Q===false){return false}L++});return Q};function C(){if($f(r)){$f(r).getParent().innerHTML="";q=$f(r).getIndex();a[q]=x}else{a.push(x);q=a.length-1}B=parseInt(r.style.height,10)||r.clientHeight;F=r.id||"fp"+f();s=H.id||F+"_api";H.id=s;v=r.innerHTML;if(typeof u=="string"){u={clip:{url:u}}}u.playerId=F;u.clip=u.clip||{};if(r.getAttribute("href",2)&&!u.clip.url){u.clip.url=r.getAttribute("href",2)}if(u.clip.url){u.clip.url=e(u.clip.url)}t=new i(u.clip,-1,x);u.playlist=u.playlist||[u.clip];var J=0;n(u.playlist,function(){var M=this;if(typeof M=="object"&&M.length){M={url:""+M}}if(M.url){M.url=e(M.url)}n(u.clip,function(N,O){if(O!==undefined&&M[N]===undefined&&typeof O!="function"){M[N]=O}});u.playlist[J]=M;M=new i(M,J,x);G.push(M);J++});n(u,function(M,N){if(typeof N=="function"){if(t[M]){t[M](N)}else{k(y,M,N)}delete u[M]}});n(u.plugins,function(M,N){if(N){z[M]=new m(M,N,x)}});if(!u.plugins||u.plugins.controls===undefined){z.controls=new m("controls",null,x)}z.canvas=new m("canvas",null,x);v=r.innerHTML;function L(M){if(/iPad|iPhone|iPod/i.test(navigator.userAgent)&&!/.flv$/i.test(G[0].url)&&!K()){return true}if(!x.isLoaded()&&x._fireEvent("onBeforeClick")!==false){x.load()}return g(M)}function K(){return x.hasiPadSupport&&x.hasiPadSupport()}function I(){if(v.replace(/\s/g,"")!==""){if(r.addEventListener){r.addEventListener("click",L,false)}else{if(r.attachEvent){r.attachEvent("onclick",L)}}}else{if(r.addEventListener&&!K()){r.addEventListener("click",g,false)}x.load()}}setTimeout(I,0)}if(typeof r=="string"){var A=c(r);if(!A){throw"Flowplayer cannot access element: "+r}r=A;C()}else{C()}}var a=[];function d(p){this.length=p.length;this.each=function(r){n(p,r)};this.size=function(){return p.length};var q=this;for(name in b.prototype){q[name]=function(){var r=arguments;q.each(function(){this[name].apply(this,r)})}}}window.flowplayer=window.$f=function(){var q=null;var p=arguments[0];if(!arguments.length){n(a,function(){if(this.isLoaded()){q=this;return false}});return q||a[0]}if(arguments.length==1){if(typeof p=="number"){return a[p]}else{if(p=="*"){return new d(a)}n(a,function(){if(this.id()==p.id||this.id()==p||this.getParent()==p){q=this;return false}});return q}}if(arguments.length>1){var u=arguments[1],r=(arguments.length==3)?arguments[2]:{};if(typeof u=="string"){u={src:u}}u=j({bgcolor:"#000000",version:[10,1],expressInstall:"http://releases.flowplayer.org/swf/expressinstall.swf",cachebusting:false},u);if(typeof p=="string"){if(p.indexOf(".")!=-1){var t=[];n(o(p),function(){t.push(new b(this,l(u),l(r)))});return new d(t)}else{var s=c(p);return new b(s!==null?s:l(p),l(u),l(r))}}else{if(p){return new b(p,l(u),l(r))}}}return null};j(window.$f,{fireEvent:function(){var q=[].slice.call(arguments);var r=$f(q[0]);return r?r._fireEvent(q.slice(1)):null},addPlugin:function(p,q){b.prototype[p]=q;return $f},each:n,extend:j});if(typeof jQuery=="function"){jQuery.fn.flowplayer=function(r,q){if(!arguments.length||typeof arguments[0]=="number"){var p=[];this.each(function(){var s=$f(this);if(s){p.push(s)}});return arguments.length?p[arguments[0]]:new d(p)}return this.each(function(){$f(this,l(r),q?l(q):{})})}}}();!function(){var h=document.all,j="http://get.adobe.com/flashplayer",c=typeof jQuery=="function",e=/(\d+)[^\d]+(\d+)[^\d]*(\d*)/,b={width:"100%",height:"100%",id:"_"+(""+Math.random()).slice(9),allowfullscreen:true,allowscriptaccess:"always",quality:"high",version:[3,0],onFail:null,expressInstall:null,w3c:false,cachebusting:false};if(window.attachEvent){window.attachEvent("onbeforeunload",function(){__flash_unloadHandler=function(){};__flash_savedUnloadHandler=function(){}})}function i(m,l){if(l){for(var f in l){if(l.hasOwnProperty(f)){m[f]=l[f]}}}return m}function a(f,n){var m=[];for(var l in f){if(f.hasOwnProperty(l)){m[l]=n(f[l])}}return m}window.flashembed=function(f,m,l){if(typeof f=="string"){f=document.getElementById(f.replace("#",""))}if(!f){return}if(typeof m=="string"){m={src:m}}return new d(f,i(i({},b),m),l)};var g=i(window.flashembed,{conf:b,getVersion:function(){var m,f;try{f=navigator.plugins["Shockwave Flash"].description.slice(16)}catch(o){try{m=new ActiveXObject("ShockwaveFlash.ShockwaveFlash.7");f=m&&m.GetVariable("$version")}catch(n){try{m=new ActiveXObject("ShockwaveFlash.ShockwaveFlash.6");f=m&&m.GetVariable("$version")}catch(l){}}}f=e.exec(f);return f?[1*f[1],1*f[(f[1]*1>9?2:3)]*1]:[0,0]},asString:function(l){if(l===null||l===undefined){return null}var f=typeof l;if(f=="object"&&l.push){f="array"}switch(f){case"string":l=l.replace(new RegExp('(["\\\\])',"g"),"\\$1");l=l.replace(/^\s?(\d+\.?\d*)%/,"$1pct");return'"'+l+'"';case"array":return"["+a(l,function(o){return g.asString(o)}).join(",")+"]";case"function":return'"function()"';case"object":var m=[];for(var n in l){if(l.hasOwnProperty(n)){m.push('"'+n+'":'+g.asString(l[n]))}}return"{"+m.join(",")+"}"}return String(l).replace(/\s/g," ").replace(/\'/g,'"')},getHTML:function(o,l){o=i({},o);var n='<object width="'+o.width+'" height="'+o.height+'" id="'+o.id+'" name="'+o.id+'"';if(o.cachebusting){o.src+=((o.src.indexOf("?")!=-1?"&":"?")+Math.random())}if(o.w3c||!h){n+=' data="'+o.src+'" type="application/x-shockwave-flash"'}else{n+=' classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000"'}n+=">";if(o.w3c||h){n+='<param name="movie" value="'+o.src+'" />'}o.width=o.height=o.id=o.w3c=o.src=null;o.onFail=o.version=o.expressInstall=null;for(var m in o){if(o[m]){n+='<param name="'+m+'" value="'+o[m]+'" />'}}var p="";if(l){for(var f in l){if(l[f]){var q=l[f];p+=f+"="+(/function|object/.test(typeof q)?g.asString(q):q)+"&"}}p=p.slice(0,-1);n+='<param name="flashvars" value=\''+p+"' />"}n+="</object>";return n},isSupported:function(f){return k[0]>f[0]||k[0]==f[0]&&k[1]>=f[1]}});var k=g.getVersion();function d(f,n,m){if(g.isSupported(n.version)){f.innerHTML=g.getHTML(n,m)}else{if(n.expressInstall&&g.isSupported([6,65])){f.innerHTML=g.getHTML(i(n,{src:n.expressInstall}),{MMredirectURL:encodeURIComponent(location.href),MMplayerType:"PlugIn",MMdoctitle:document.title})}else{if(!f.innerHTML.replace(/\s/g,"")){f.innerHTML="<h2>Flash version "+n.version+" or greater is required</h2><h3>"+(k[0]>0?"Your version is "+k:"You have no flash plugin installed")+"</h3>"+(f.tagName=="A"?"<p>Click here to download latest version</p>":"<p>Download latest version from <a href='"+j+"'>here</a></p>");if(f.tagName=="A"||f.tagName=="DIV"){f.onclick=function(){location.href=j}}}if(n.onFail){var l=n.onFail.call(this);if(typeof l=="string"){f.innerHTML=l}}}}if(h){window[n.id]=document.getElementById(n.id)}i(this,{getRoot:function(){return f},getOptions:function(){return n},getConf:function(){return m},getApi:function(){return f.firstChild}})}if(c){jQuery.tools=jQuery.tools||{version:"3.2.12"};jQuery.tools.flashembed={conf:b};jQuery.fn.flashembed=function(l,f){return this.each(function(){$(this).data("flashembed",flashembed(this,l,f))})}}}();;/** @namespace H5P */
H5P.VideoYouTube = (function ($) {

  /**
   * YouTube video player for H5P.
   *
   * @class
   * @param {Array} sources Video files to use
   * @param {Object} options Settings for the player
   * @param {Object} l10n Localization strings
   */
  function YouTube(sources, options, l10n) {
    var self = this;

    var player;
    var id = 'h5p-youtube-' + numInstances;
    numInstances++;

    var $wrapper = $('<div/>');
    var $placeholder = $('<div/>', {
      id: id,
      text: l10n.loading
    }).appendTo($wrapper);


    /**
     * Use the YouTube API to create a new player
     *
     * @private
     */
    var create = function () {
      if (!$placeholder.is(':visible') || player !== undefined) {
        return;
      }

      if (window.YT === undefined) {
        // Load API first
        loadAPI(create);
        return;
      }

      var width = $wrapper.width();
      if (width < 200) {
        width = 200;
      }
      var disablekb = 0;
     
      player = new YT.Player(id, {
        width: width,
        height: width * (9/16),
        videoId: getId(sources[0].path),
        playerVars: {
          origin: ORIGIN,
          autoplay: options.autoplay ? 1 : 0,
          controls: options.controls ? 1 : 0,
          //disablekb: options.controls ? 0 : 1,
          disablekb: disablekb,//options.controls ? 0 : 1,
          fs: 0,
          loop: options.loop ? 1 : 0,
          rel: 0,
          showinfo: 0,
          iv_load_policy: 3,
          wmode: "opaque",
          start: options.startAt
        },
        events: {
          onReady: function () {
            self.trigger('ready');
            self.trigger('loaded');
          },
          onStateChange: function (state) {
            if (state.data > -1 && state.data < 4) {
              self.trigger('stateChange', state.data);
            }
          },
          onPlaybackQualityChange: function (quality) {
            self.trigger('qualityChange', quality.data);
          },
          onError: function (error) {
            var message;
            switch (error.data) {
              case 2:
                message = l10n.invalidYtId;
                break;

              case 100:
                message = l10n.unknownYtId;
                break;

              case 101:
              case 150:
                message = l10n.restrictedYt;
                break;

              default:
                message = l10n.unknownError + ' ' + error.data;
                break;
            }
            self.trigger('error', message);
          }
        }
      });
    };

    /**
     * Indicates if the video must be clicked for it to start playing.
     * For instance YouTube videos on iPad must be pressed to start playing.
     *
     * @public
     */
    self.pressToPlay = navigator.userAgent.match(/iPad/i) ? true : false;

    /**
    * Appends the video player to the DOM.
    *
    * @public
    * @param {jQuery} $container
    */
    self.appendTo = function ($container) {
      $container.addClass('h5p-youtube').append($wrapper);
      create();
    };

    /**
     * Get list of available qualities. Not available until after play.
     *
     * @public
     * @returns {Array}
     */
    self.getQualities = function () {
      if (!player || !player.getAvailableQualityLevels) {
        return;
      }

      var qualities = player.getAvailableQualityLevels();
      if (!qualities.length) {
        return; // No qualities
      }

      // Add labels
      for (var i = 0; i < qualities.length; i++) {
        var quality = qualities[i];
        var label = (LABELS[quality] !== undefined ? LABELS[quality] : 'Unknown'); // TODO: l10n
        qualities[i] = {
          name: quality,
          label: LABELS[quality]
        };
      }

      return qualities;
    };

    /**
     * Get current playback quality. Not available until after play.
     *
     * @public
     * @returns {String}
     */
    self.getQuality = function () {
      if (!player || !player.getPlaybackQuality) {
        return;
      }

      var quality = player.getPlaybackQuality();
      return quality === 'unknown' ? undefined : quality;
    };

    /**
     * Set current playback quality. Not available until after play.
     * Listen to event "qualityChange" to check if successful.
     *
     * @public
     * @params {String} [quality]
     */
    self.setQuality = function (quality) {
      if (!player || !player.setPlaybackQuality) {
        return;
      }

      player.setPlaybackQuality(quality);
    };

    /**
     * Starts the video.
     *
     * @public
     */
    self.play = function () {
      if (!player || !player.playVideo) {
        self.on('ready', self.play);
        return;
      }

      player.playVideo();
    };

    /**
     * Pauses the video.
     *
     * @public
     */
    self.pause = function () {
      self.off('ready', self.play);
      if (!player || !player.pauseVideo) {
        return;
      }
      player.pauseVideo();
    };

    /**
     * Seek video to given time.
     *
     * @public
     * @param {Number} time
     */
    self.seek = function (time) {
      if (!player || !player.seekTo) {
        return;
      }

      player.seekTo(time, true);
    };

    /**
     * Get elapsed time since video beginning.
     *
     * @public
     * @returns {Number}
     */
    self.getCurrentTime = function () {
      if (!player || !player.getCurrentTime) {
        return;
      }

      return player.getCurrentTime();
    };

    /**
     * Get total video duration time.
     *
     * @public
     * @returns {Number}
     */
    self.getDuration = function () {
      if (!player || !player.getDuration) {
        return;
      }

      return player.getDuration();
    };

    /**
     * Get percentage of video that is buffered.
     *
     * @public
     * @returns {Number} Between 0 and 100
     */
    self.getBuffered = function () {
      if (!player || !player.getVideoLoadedFraction) {
        return;
      }

      return player.getVideoLoadedFraction() * 100;
    };

    /**
     * Turn off video sound.
     *
     * @public
     */
    self.mute = function () {
      if (!player || !player.mute) {
        return;
      }

      player.mute();
    };

    /**
     * Turn on video sound.
     *
     * @public
     */
    self.unMute = function () {
      if (!player || !player.unMute) {
        return;
      }

      player.unMute();
    };

    /**
     * Check if video sound is turned on or off.
     *
     * @public
     * @returns {Boolean}
     */
    self.isMuted = function () {
      if (!player || !player.isMuted) {
        return;
      }

      return player.isMuted();
    };

    /**
     * Returns the video sound level.
     *
     * @public
     * @returns {Number} Between 0 and 100.
     */
    self.getVolume = function () {
      if (!player || !player.getVolume) {
        return;
      }

      return player.getVolume();
    };

    /**
     * Set video sound level.
     *
     * @public
     * @param {Number} level Between 0 and 100.
     */
    self.setVolume = function (level) {
      if (!player || !player.setVolume) {
        return;
      }

      player.setVolume(level);
    };

    // Respond to resize events by setting the YT player size.
    self.on('resize', function () {
      if (!player) {
        // Player isn't created yet. Try again.
        create();
        return;
      }

      // Use as much space as possible
      $wrapper.css({
        width: '100%',
        height: '100%'
      });

      var width = $wrapper[0].clientWidth;
      var height = options.fit ? $wrapper[0].clientHeight : (width * (9/16));

      // Set size
      $wrapper.css({
        width: width + 'px',
        height: height + 'px'
      });

      player.setSize(width, height);
    });
  }

  /**
   * Check to see if we can play any of the given sources.
   *
   * @public
   * @static
   * @param {Array} sources
   * @returns {Boolean}
   */
  YouTube.canPlay = function (sources) {
    return getId(sources[0].path);
  };

  /**
   * Find id of YouTube video from given URL.
   *
   * @private
   * @param {String} url
   * @returns {String} YouTube video identifier
   */
  var getId = function (url) {
    var matches = url.match(/^https?:\/\/(www.youtube.com|youtu.be|y2u.be)\/(.+=)?(\S+)$/i);
    if (matches && matches[3]) {
      return matches[3];
    }
  };

  /**
   * Load the IFrame Player API asynchronously.
   */
  var loadAPI = function (loaded) {
    if (window.onYouTubeIframeAPIReady !== undefined) {
      // Someone else is loading, hook in
      var original = window.onYouTubeIframeAPIReady;
      window.onYouTubeIframeAPIReady = function (id) {
        loaded(id);
        original(id);
      };
    }
    else {
      // Load the API our self
      var tag = document.createElement('script');
      tag.src = "https://www.youtube.com/iframe_api";
      var firstScriptTag = document.getElementsByTagName('script')[0];
      firstScriptTag.parentNode.insertBefore(tag, firstScriptTag);
      window.onYouTubeIframeAPIReady = loaded;
    }
  };

  /** @constant {Object} */
  var LABELS = {
    highres: '2160p',
    hd1440: '1440p',
    hd1080: '1080p',
    hd720: '720p',
    large: '480p',
    medium: '360p',
    small: '240p',
    tiny: '144p',
    auto: 'Auto'
  };

  /** @private */
  var numInstances = 0;

  // Extract the current origin (used for security)
  var ORIGIN = window.location.href.match(/http[s]?:\/\/[^\/]+/)[0];

  return YouTube;
})(H5P.jQuery);

// Register video handler
H5P.videoHandlers = H5P.videoHandlers || [];
H5P.videoHandlers.push(H5P.VideoYouTube);
;/** @namespace H5P */
H5P.VideoHtml5 = (function ($) {

  /**
   * HTML5 video player for H5P.
   *
   * @class
   * @param {Array} sources Video files to use
   * @param {Object} options Settings for the player
   * @param {Object} l10n Localization strings
   */
  function Html5(sources, options, l10n) {
    var self = this;

    /**
     * Displayed when the video is buffering
     * @private
     */
    var $throbber = $('<div/>', {
      'class': 'h5p-video-loading'
    });

    /**
     * Used to display error messages
     * @private
     */
    var $error = $('<div/>', {
      'class': 'h5p-video-error'
    });

    /**
     * Keep track of current state when changing quality.
     * @private
     */
    var stateBeforeChangingQuality;
    var currentTimeBeforeChangingQuality;

    /**
     * Avoids firing the same event twice.
     * @private
     */
    var lastState;

    // Create player
    var video = document.createElement('video');

    // Sort sources into qualities
    var qualities = getQualities(sources, video);

    // Select quality and source
    var currentQuality = getPreferredQuality();
    if (currentQuality === undefined || qualities[currentQuality] === undefined) {
      // No preferred quality, pick the first.
      for (currentQuality in qualities) {
        if (qualities.hasOwnProperty(currentQuality)) {
          break;
        }
      }
    }
    video.src = qualities[currentQuality].source.path;

    // Set options
    video.controls = (options.controls ? true : false);
    video.autoplay = (options.autoplay ? true : false);
    video.loop = (options.loop ? true : false);
    video.className = 'h5p-video';
    
    video.style.display = 'none';
    video.style.position="absolute"; //for safari avoid sliding the video editor. 
    video.style.top="0";
    video.style.left="0";
    
    if (options.fit) {
      // Style is used since attributes with relative sizes aren't supported by IE9.
      video.style.width = '100%';
      video.style.height = '100%';
    }
    // Add poster if provided
    if (options.poster) {
      video.poster = options.poster;
    }

    /**
     * Register track to video
     *
     * @param {Object} trackData Track object
     * @param {string} trackData.kind Kind of track
     * @param {Object} trackData.track Source path
     * @param {string} [trackData.label] Label of track
     * @param {string} [trackData.srcLang] Language code
     */
    var addTrack = function (trackData) {
      // Skip invalid tracks
      if (!trackData.kind || !trackData.track.path) {
        return;
      }

      var track = document.createElement('track');
      track.kind = trackData.kind;
      track.src = trackData.track.path;
      if (trackData.label) {
        track.label = trackData.label;
      }

      if (trackData.srcLang) {
        track.srcLang = trackData.srcLang;
      }

      return track;
    };

    // Register tracks
    options.tracks.forEach(function (track, i) {
      var trackElement = addTrack(track);
      if (i === 0) {
        trackElement.default = true;
      }
      if (trackElement) {
        video.appendChild(trackElement);
      }
    });

    /**
     * Helps registering events.
     *
     * @private
     * @param {String} native Event name
     * @param {String} h5p Event name
     * @param {String} [arg] Optional argument
     */
    var mapEvent = function (native, h5p, arg) {
      video.addEventListener(native, function () {
        switch (h5p) {
          case 'stateChange':
            if (lastState === arg) {
              return; // Avoid firing event twice.
            }

            var validStartTime = options.startAt && options.startAt > 0;
            if (arg === H5P.Video.PLAYING && validStartTime) {
              video.currentTime = options.startAt;
              delete options.startAt;
            }
            break;

          case 'loaded':
            if (stateBeforeChangingQuality !== undefined) {
              return; // Avoid loaded event when changing quality.
            }

            // Remove any errors
            if ($error.is(':visible')) {
              $error.remove();
            }

            if (OLD_ANDROID_FIX) {
              var andLoaded = function () {
                video.removeEventListener('durationchange', andLoaded, false);
                // On Android seeking isn't ready until after play.
                self.trigger(h5p);
              };
              video.addEventListener('durationchange', andLoaded, false);
              return;
            }
            break;

          case 'error':
            // Handle error and get message.
            arg = error(arguments[0], arguments[1]);
            break;
        }
        self.trigger(h5p, arg);
      }, false);
    };

    /**
     * Handle errors from the video player.
     *
     * @private
     * @param {Object} code Error
     * @param {String} [message]
     * @returns {String} Human readable error message.
     */
    var error = function (code, message) {
      if (code instanceof Event) {

        // No error code
        if (!code.target.error) {
          return '';
        }

        switch (code.target.error.code) {
          case MediaError.MEDIA_ERR_ABORTED:
            message = l10n.aborted;
            break;
          case MediaError.MEDIA_ERR_NETWORK:
            message = l10n.networkFailure;
            break;
          case MediaError.MEDIA_ERR_DECODE:
            message = l10n.cannotDecode;
            break;
          case MediaError.MEDIA_ERR_SRC_NOT_SUPPORTED:
            message = l10n.formatNotSupported;
            break;
          case MediaError.MEDIA_ERR_ENCRYPTED:
            message = l10n.mediaEncrypted;
            break;
        }
      }
      if (!message) {
        message = l10n.unknownError;
      }

      // Hide throbber
      $throbber.remove();

      // Display error message to user
      $error.text(message).insertAfter(video);

      // Pass message to our error event
      return message;
    };

    /**
     * Appends the video player to the DOM.
     *
     * @public
     * @param {jQuery} $container
     */
    self.appendTo = function ($container) {
    	  //$(".h5p-editor-iframe").contents().find(".h5peditor-form > .tree > .wizard > .h5peditor-label").hide()
  	  H5P.jQuery(".h5peditor-form > .tree > .wizard > .h5peditor-label").hide(); //Interactive editor title
	  H5P.jQuery(".h5peditor-form > .tree > .wizard > .h5peditor-tabs").hide(); //tabs

    	  $container.append(video);
      H5P.jQuery("video.h5p-video").css("display","block");
      H5P.jQuery("video.h5p-video").css("position","relative"); 
      
    };
    

    self.attachVideoJS = function($container){
     	$wrapper.appendTo($container);
     	window.parent.$(".loadercontent").remove();
        var h = window.parent.$("#iframedata").height()-38;
     	var w = window.parent.$("#iframedata").width();
     	loadScriptsForTinyMCE("/sites/all/modules/core/exp_sp_core/js/videojs/video.js");
     	loadScriptsForTinyMCE("/sites/all/modules/core/exp_sp_core/js/videojs/Vimeo.js");
     	loadScriptsForTinyMCE("/sites/all/libraries/h5plibraries/H5P.Video-1.2/scripts/vimeoplayer.js");
     	
     	videojs.options.flash.swf = "/sites/all/modules/core/exp_sp_core/js/videojs/video-js.swf";
    	
    	H5P.jQuery(".h5p-interactive-video").css("height","inherit");
    	H5P.jQuery(".h5p-video-wrapper").css("height","inherit");
    	window.parent.$(".loadercontent").remove();	
    	
    	if(sources[0].path.indexOf("vimeo") > 0)
    	{
    		// get vimeo id from url
    		var r = /(videos|video|channels|\.com)\/([\d]+)/;
    		var vimeoId=sources[0].path.match(r)[2];
    		

    		html = '<iframe style="height:180%;width:100%;left:0;position:absolute;top:-40%" src="https://player.vimeo.com/video/'+vimeoId+'?player_id=my_video_2" id="my_video_2" class="Fullsc video-js" width="100%" height="100%" frameborder="0" webkitallowfullscreen mozallowfullscreen allowfullscreen></iframe>';
    		// html = '<iframe style="width:100%;"
    		// src="https://player.vimeo.com/video/76979871?api=1&player_id=my_video_2"
    		// id="my_video_2" class="Fullsc video-js" width="100%" height="100%"
    		// frameborder="0" webkitallowfullscreen mozallowfullscreen
    		// allowfullscreen></iframe>';
    		H5P.jQuery(".h5p-video-flash").append(html);
    		var iframe = document.getElementById('my_video_2');
    	    fp = new Vimeo.Player(iframe);
    	    console.log("fp:",fp);
    	    console.log("size::",H5P.jQuery(".h5p-video-flash").size());
    	    
    	    window.setTimeout(function(){
    	    	    
    	    	fp.on("loaded", function () {
    	    	console.log("fp.addEvent ready1....");
    	   	fp.play();
    	    	window.parent.$("#iframe_editor_wrapper").css("visibility","visible");

    	    fp.getDuration().then(function(duration) {
    	    		vimeoDuration = duration;
    	    }).catch(function(error) {
    	        // an error occurred
    	    });
    	    
    	    
    	    

    		
    		fp.getVideoWidth().then(function(width) {
    			vimeoWidth = width;
    		}).catch(function(error) {
    		    // an error occurred
    		});
    		fp.getVideoHeight().then(function(height) {
    			vimeoHeight = height;
    		}).catch(function(error) {
    		    // an error occurred
    		});
    		fp.on('timeupdate',function(evt){
        		vimeoCurrentTime = evt.seconds;
    		})
    		fp.on('progress',function(evt){
        			vimeoBuffered = evt.percent*100;
    		})

    	
    	    
    	    
    	    
    	    
    	    fp.on('play', function() {
    	    		self.trigger('stateChange', H5P.Video.PLAYING);
    	        console.log('played the video!');
    	    });

    	    fp.on('pause', function() {
    	    		self.trigger('stateChange', H5P.Video.PAUSED);
    	        console.log('pause the video!');
    	    });
    	    fp.on( 'ended', 			function( e ) { window.console.log( 'videoJS: ended111' );self.trigger('stateChange', H5P.Video.ENDED);});
    	    });
    	    },1000); // timeout

    	}
    	else
    	{
    		html =   "<video id='my_video_2' class='Fullsc video-js' autoplay=true controls='false' preload='auto' width='"+w+"' height='"+h+"' data-setup=\"{}\">";
    	// html += "<source
    	// src='rtmp://192.241.173.210/oflaDemo/Saravanan_4/256/Saravanan.mp4'
    	// type='rtmp/mp4' />"; //what about webm?
    	    html += "<source src='"+sources[0].path+"' type='rtmp/mp4' />";
    		html +="</video>";

    	H5P.jQuery(".h5p-video-flash").append(html);

    	console.log("1115:"+sources[0].path);
    	var playing = false,firstTime = true;
    	fp = videojs("my_video_2",{controls: false}).ready(function(){
      	var player = this;
      	// this.play();
        player.on( 'loadedmetadata', 	function( e ) { 
      																				console.log('load meta...');
      																				self.trigger('ready');
      																				self.trigger('loaded');
      																				self.trigger("addControls");
      																				window.setTimeout(function(){
      																				playing = true;
    																				firstTime = false;
      																				
      																				self.trigger('stateChange', H5P.Video.PLAYING);
      																				},50);
      														
      												});
    	// player.on( 'timeupdate', function( e ) { window.console.log( 'videoJS:
    	// ended111' );self.trigger('stateChange', H5P.Video.ENDED);});

    	player.on( 'ended', 			function( e ) { window.console.log( 'videoJS: ended111' );self.trigger('stateChange', H5P.Video.ENDED);});
    	player.on( 'pause', 			function( e ) { window.console.log( 'videoJS: pause' );
    				firstTime = false;
    				self.trigger('stateChange', H5P.Video.PAUSED); 
    				});
    	
    	
    	player.on( 'play', 				function( e ) {
    													if(firstTime == false)
    														self.trigger('stateChange', H5P.Video.PLAYING);
    												 });
    	
    	
    	player.on( 'seeking', 			function( e ) { console.log( 'videoJS: seeking' );
    	
    			self.trigger('stateChange', H5P.Video.BUFFERING); 
    			});
    	// play/pause
    	var obt = H5P.jQuery(".vjs-text-track-display");  	
      	obt.css({ "pointer-events": "auto" });  	
      	obt.on('click', function() {  		
      		if (player.paused()) { 
      			player.play();
      	  	  }else if(player.play()){
      	  		player.pause();
      	  	  }
      	});
      	
    	/*
    	 * player.on( 'waiting', function( e ) { window.console.log( 'videoJS:
    	 * waiting' ); //self.trigger('stateChange', H5P.Video.BUFFERING); });
    	 */


    	// player.on( 'loadstart', function( e ) { window.console.log( 'videoJS:
    	// loadstart' ); self.trigger('stateChange', H5P.Video.PLAYING);});
    	
    	
      	// player.on( 'durationchange', function( e ) { window.console.log(
    	// 'videoJS: durationchange' ); self.trigger('stateChange',
    	// H5P.Video.PLAYING); });
    	
    	// player.on( 'error', function( e ) { window.console.log( 'videoJS: error'
    	// ); self.trigger('error', message);});
    	
    	// player.on( 'loadedalldata', function( e ) { window.console.log( 'videoJS:
    	// loadedalldata' );self.trigger('stateChange', H5P.Video.PLAYING); });
    	
    	
    	
    	
    	
    	// player.on( 'seeked', function( e ) { alert( 'videoJS: seeked' ); });
    	// player.on( 'seeking', function( e ) { alert( 'videoJS: seeking'
    	// );self.trigger('stateChange', InteractiveVideo.SEEKING); });
    	// player.on( 'waiting', function( e ) { window.console.log( 'videoJS:
    	// waiting' );self.trigger('stateChange', H5P.Video.BUFFERING); });
    // player.on( 'contentplayback', function( e ) { window.console.log( 'videoJS:
    // contentplayback' );self.trigger('stateChange', H5P.Video.PLAYING); });
    	});
    	}
    	
    	H5P.jQuery(".h5p-video").css("margin-top","0px");
    	
     	};
     	

    /**
     * Get list of available qualities. Not available until after play.
     *
     * @public
     * @returns {Array}
     */
    self.getQualities = function () {
      // Create reverse list
      var options = [];
      for (var q in qualities) {
        if (qualities.hasOwnProperty(q)) {
          options.splice(0, 0, {
            name: q,
            label: qualities[q].label
          });
        }
      }

      if (options.length < 2) {
        // Do not return if only one quality.
        return;
      }

      return options;
    };

    /**
     * Get current playback quality. Not available until after play.
     *
     * @public
     * @returns {String}
     */
    self.getQuality = function () {
      return currentQuality;
    };

    /**
     * Set current playback quality. Not available until after play.
     * Listen to event "qualityChange" to check if successful.
     *
     * @public
     * @params {String} [quality]
     */
    self.setQuality = function (quality) {
      if (qualities[quality] === undefined || quality === currentQuality) {
        return; // Invalid quality
      }

      // Keep track of last choice
      setPreferredQuality(quality);

      // Avoid multiple loaded events if changing quality multiple times.
      if (!stateBeforeChangingQuality) {
        // Keep track of last state
        stateBeforeChangingQuality = lastState;

        // Keep track of current time
        currentTimeBeforeChangingQuality = video.currentTime;

        // Seek and start video again after loading.
        var loaded = function () {
          video.removeEventListener('loadedmetadata', loaded, false);
          if (OLD_ANDROID_FIX) {
            var andLoaded = function () {
              video.removeEventListener('durationchange', andLoaded, false);
              // On Android seeking isn't ready until after play.
              self.seek(currentTimeBeforeChangingQuality);
            };
            video.addEventListener('durationchange', andLoaded, false);
          }
          else {
            // Seek to current time.
            self.seek(currentTimeBeforeChangingQuality);
          }

          // Always play to get image.
          video.play();

          if (stateBeforeChangingQuality !== H5P.Video.PLAYING) {
            // Do not resume playing
            video.pause();
          }

          // Done changing quality
          stateBeforeChangingQuality = undefined;

          // Remove any errors
          if ($error.is(':visible')) {
            $error.remove();
          }
        };
        video.addEventListener('loadedmetadata', loaded, false);
      }

      // Keep track of current quality
      currentQuality = quality;
      self.trigger('qualityChange', currentQuality);

      // Display throbber
      self.trigger('stateChange', H5P.Video.BUFFERING);

      // Change source
      video.src = qualities[quality].source.path; // (iPad does not support #t=).
    };

    /**
     * Starts the video.
     *
     * @public
     */
    self.play = function () {
      if ($error.is(':visible')) {
        return;
      }

      video.play();
    };

    /**
     * Pauses the video.
     *
     * @public
     */
    self.pause = function () {
      video.pause();
    };

    /**
     * Seek video to given time.
     *
     * @public
     * @param {Number} time
     */
    self.seek = function (time) {
      if (lastState === undefined) {
        // Make sure we always play before we seek to get an image.
        // If not iOS devices will reset currentTime when pressing play.
        video.play();
        video.pause();
      }

      video.currentTime = time;
    };

    /**
     * Get elapsed time since video beginning.
     *
     * @public
     * @returns {Number}
     */
    self.getCurrentTime = function () {
      return video.currentTime;
    };

    /**
     * Get total video duration time.
     *
     * @public
     * @returns {Number}
     */
    self.getDuration = function () {
      if (isNaN(video.duration)) {
        return;
      }

      return  video.duration;
    };

    /**
     * Get percentage of video that is buffered.
     *
     * @public
     * @returns {Number} Between 0 and 100
     */
    self.getBuffered = function () {
      // Find buffer currently playing from
      var buffered = 0;
      for (var i = 0; i < video.buffered.length; i++) {
        var from = video.buffered.start(i);
        var to = video.buffered.end(i);

        if (video.currentTime > from && video.currentTime < to) {
          buffered = to;
          break;
        }
      }

      // To percentage
      return buffered ? (buffered / video.duration) * 100 : 0;
    };

    /**
     * Turn off video sound.
     *
     * @public
     */
    self.mute = function () {
      video.muted = true;
    };

    /**
     * Turn on video sound.
     *
     * @public
     */
    self.unMute = function () {
      video.muted = false;
    };

    /**
     * Check if video sound is turned on or off.
     *
     * @public
     * @returns {Boolean}
     */
    self.isMuted = function () {
      return video.muted;
    };

    /**
     * Returns the video sound level.
     *
     * @public
     * @returns {Number} Between 0 and 100.
     */
    self.getVolume = function () {
      return video.volume * 100;
    };

    /**
     * Set video sound level.
     *
     * @public
     * @param {Number} level Between 0 and 100.
     */
    self.setVolume = function (level) {
      video.volume = level / 100;
    };

    // Register event listeners
    mapEvent('ended', 'stateChange', H5P.Video.ENDED);
    mapEvent('playing', 'stateChange', H5P.Video.PLAYING);
    mapEvent('pause', 'stateChange', H5P.Video.PAUSED);
    mapEvent('waiting', 'stateChange', H5P.Video.BUFFERING);
    mapEvent('loadedmetadata', 'loaded');
    mapEvent('error', 'error');

    if (!video.controls) {
      // Disable context menu(right click) to prevent controls.
      video.addEventListener('contextmenu', function (event) {
        event.preventDefault();
      }, false);
    }

    // Display throbber when buffering/loading video.
    self.on('stateChange', function (event) {
      var state = event.data;
      lastState = state;
      if (state === H5P.Video.BUFFERING) {
        $throbber.insertAfter(video);
      }
      else {
        $throbber.remove();
      }
    });

    // Video controls are ready
    setTimeout(function () {
      self.trigger('ready');
      
      H5P.jQuery(window).keypress(function(e) {
    	      var keyCode = e.keyCode || e.charCode;
    	      
          if (keyCode == 32) { //space bar
        	  e.preventDefault();
        	  if(e.target.tagName.toLowerCase() == "body" || e.target.tagName.toLowerCase() == "video") 
        		  H5P.jQuery("video.h5p-video").get(0).paused?H5P.jQuery("video.h5p-video").get(0).play() :H5P.jQuery("video.h5p-video").get(0).pause();
          }
      });
      
      
      
    }, 0);
  }

  /**
   * Check to see if we can play any of the given sources.
   *
   * @public
   * @static
   * @param {Array} sources
   * @returns {Boolean}
   */
  Html5.canPlay = function (sources) {
    var video = document.createElement('video');
    if (video.canPlayType === undefined) {
      return false; // Not supported
    }
    
    // Cycle through sources
    for (var i = 0; i < sources.length; i++) {
      //if it is an editor, use h5p video handlers. Do not return false to attach videojs	
    	
    	 	var playerType = "flowplayer";
 	    if(sources[i].path.indexOf("rtmp://")==0)
 	    		playerType = "videojs";
 	    else if(sources[i].path.indexOf("vimeo")>0 )
 	    		playerType = "vimeo";
 	   console.log("playerType:",playerType); 
 	   
      if(sources[i].path.indexOf("rtmp:") >=0  && (window.parent.$("#iframe_editor_wrapper_container").size() == 0 && window.parent.$("#contentauthor-presentation-addedit-form").size() == 0))
      	return false;
      var type = getType(sources[i]);
      if(playerType == "vimeo")
    	    return false;
      if (type && video.canPlayType(type) !== '') {
        // We should be able to play this
        return true;
      }
    }

    return false;
  };

  /**
   * Find source type.
   *
   * @private
   * @param {Object} source
   * @returns {String}
   */
  var getType = function (source) {
    var type = source.mime;
    if (!type) {
      // Try to get type from URL
      var matches = source.path.match(/\.(\w+)$/);
      if (matches && matches[1]) {
        type = 'video/' + matches[1];
      }
    }

    if (type && source.codecs) {
      // Add codecs
      type += '; codecs="' + source.codecs + '"';
    }

    return type;
  };

  /**
   * Sort sources into qualities.
   *
   * @private
   * @static
   * @param {Array} sources
   * @param {Object} video
   * @returns {Object} Quality mapping
   */
  var getQualities = function (sources, video) {
    var qualities = {};
    var qualityIndex = 1;
    var lastQuality;

    // Cycle through sources
    for (var i = 0; i < sources.length; i++) {
      var source = sources[i];

      // Find and update type.
      var type = source.type = getType(source);

      // Check if we support this type
      if (!type || video.canPlayType(type) === '') {
        continue; // We cannot play this source
      }

      if (source.quality === undefined) {
        /* No quality metadata. Create a dummy tag to seperate multiple
        sources of the same type, e.g. if two mp4 files have been uploaded. */

        if (lastQuality === undefined || qualities[lastQuality].source.type === type) {
          // Create a new quality tag
          source.quality = {
            name: 'q' + qualityIndex,
            label: 'Quality ' + qualityIndex // TODO: l10n
          };
          qualityIndex++;
        }
        else {
          // Tag as the same quality as the last source
          source.quality = qualities[lastQuality].source.quality;
        }
      }

      // Log last quality
      lastQuality = source.quality.name;

      // Look to see if quality exists
      var quality = qualities[lastQuality];
      if (quality) {
        // We have a source with this quality. Check if we have a better format.
        if (source.mime.split('/')[1] === PREFERRED_FORMAT) {
          quality.source = source;
        }
      }
      else {
        // Add new source with quality.
        qualities[source.quality.name] = {
          label: source.quality.label,
          source: source
        };
      }
    }

    return qualities;
  };

  /**
   * Set preferred video quality.
   *
   * @private
   * @static
   * @param {String} quality Index of preferred quality
   */
  var setPreferredQuality = function (quality) {
    var settings = document.cookie.split(';');
    for (var i = 0; i < settings.length; i++) {
      var setting = settings[i].split('=');
      if (setting[0] === 'H5PVideoQuality') {
        setting[1] = quality;
        settings[i] = setting.join('=');
        document.cookie = settings.join(';');
        return;
      }
    }

    document.cookie = 'H5PVideoQuality=' + quality + '; ' + document.cookie;
  };

  /**
   * Set preferred video quality.
   *
   * @private
   * @static
   * @returns {String} Index of preferred quality
   */
  var getPreferredQuality = function () {
    var quality, settings = document.cookie.split(';');
    for (var i = 0; i < settings.length; i++) {
      var setting = settings[i].split('=');
      if (setting[0] === 'H5PVideoQuality') {
        quality = setting[1];
        break;
      }
    }

    return quality;
  };

  /** @constant {Boolean} */
  var OLD_ANDROID_FIX = false;

  /** @constant {Boolean} */
  var PREFERRED_FORMAT = 'mp4';

  if (navigator.userAgent.indexOf('Android') !== -1) {
    // We have Android, check version.
    var version = navigator.userAgent.match(/AppleWebKit\/(\d+\.?\d*)/);
    if (version && version[1] && Number(version[1]) <= 534.30) {
      // Include fix for devices running the native Android browser.
      // (We don't know when video was fixed, so the number is just the lastest
      // native android browser we found.)
      OLD_ANDROID_FIX = true;
    }
  }
  else {
    if (navigator.userAgent.indexOf('Chrome') !== -1) {
      // If we're using chrome on a device that isn't Android, prefer the webm
      // format. This is because Chrome has trouble with some mp4 codecs.
      PREFERRED_FORMAT = 'webm';
    }
  }

  return Html5;
})(H5P.jQuery);

// Register video handler
H5P.videoHandlers = H5P.videoHandlers || [];
H5P.videoHandlers.push(H5P.VideoHtml5);
;/** @namespace H5P */

/*
 * h5pcustomize - Removed flowplayer support and added videojs for flash
 * objects.Backup filename for flowplayer flash.js_backup
 */

H5P.VideoFlash = (function ($) {

  /**
	 * Flash video player for H5P.
	 * 
	 * @class
	 * @param {Array}
	 *            sources Video files to use
	 * @param {Object}
	 *            options Settings for the player
	 */
  function Flash(sources, options) {
    console.log("sources:",sources);
    var self = this;
    var vimeoHeight =0,vimeoWidth = 0;
    var vimeoDuration = 0,vimeoCurrentTime = 0,vimeoBuffered = 0,vimeoVolume = 0;
    var videoSeen = 0;
    //var playerType = "flowplayer";
    var playerType = "videojs";
    if(sources[0].path.indexOf("rtmp://")==0)
    		playerType = "videojs";
    else if(sources[0].path.indexOf("vimeo")>0 )
    		playerType = "vimeo";
    
   // playerType = "videojs";
   // var playerType = (sources[0].path.indexOf("rtmp://")==0 ||
	// sources[0].path.indexOf("vimeo")>0 )?"videojs":"flowplayer";
    console.log("playerType:",playerType);

    // Player wrapper
    var $wrapper = $('<div/>', {
      'class': 'h5p-video-flash',
      'id':'h5p-video-flash',
      css: {
        width: '100%',
        height: '100%'
      }
    });

    /**
	 * Used to display error messages
	 * 
	 * @private
	 */
    var $error = $('<div/>', {
      'class': 'h5p-video-error'
    });

    /**
	 * Keep track of current state when changing quality.
	 * 
	 * @private
	 */
    var stateBeforeChangingQuality;
    var currentTimeBeforeChangingQuality;

    // Sort sources into qualities
    // var qualities = getQualities(sources);
    var currentQuality;
    

    // Create player options
    var playerOptions = {
    
      buffering: true,
      
      clip: {
        url: sources[0].path, // getPreferredQuality(),
        autoPlay: options.autoplay,
        autoBuffering: true,
        scaling: 'fit',
        onSeek: function () {
          if (stateBeforeChangingQuality) {
            // ????
          }
        },
        onMetaData: function () {
          setTimeout(function () {
            if (stateBeforeChangingQuality !== undefined) {
              fp.seek(currentTimeBeforeChangingQuality);
              if (stateBeforeChangingQuality === H5P.Video.PLAYING) {
                // Resume play
                fp.play();
              }

              // Done changing quality
              stateBeforeChangingQuality = undefined;

              // Remove any errors
              if ($error.is(':visible')) {
                $error.remove();
              }
            }
            else {
              self.trigger('ready');
              self.trigger('loaded');
            }
          }, 0); // Run on next tick
        },
        onBegin: function () {
          self.trigger('stateChange', H5P.Video.PLAYING);
        },
        onResume: function () {
          self.trigger('stateChange', H5P.Video.PLAYING);
        },
        onPause: function () {
          self.trigger('stateChange', H5P.Video.PAUSED);
        },
        onFinish: function () {
          self.trigger('stateChange', H5P.Video.ENDED);
        },
        onError: function (code, message) {
          console.log('ERROR', code, message); // TODO
          self.trigger('error', message);
        }
      },
       plugins: {
        rtmp: {
          url: "http://releases.flowplayer.org/swf/flowplayer.rtmp-3.2.13.swf"
        },
        controls: { display: 'none'},
        
      },
  	  autoplay: true,
      play: null, // Disable overlay controls
      onPlaylistReplace: function () {
        that.playlistReplaced();
      }
    };

    if (options.controls) {
      playerOptions.plugins.controls = {};
      delete playerOptions.play;
    }
    var fp = null;
 	if(playerType == "flowplayer")
 	{
      var fp = flowplayer($wrapper[0], {
      src: "http://releases.flowplayer.org/swf/flowplayer-3.2.16.swf",
      wmode: "opaque"
    }, playerOptions); 
	}
	
	
    /**
	 * Appends the video player to the DOM.
	 * 
	 * @public
	 * @param {jQuery}
	 *            $container
	 */
    self.appendTo = function ($container) {
    	
      $wrapper.appendTo($container);
    };

 	self.attachVideoJS = function($container){
 	console.log("$container:",$container);	
 	$wrapper.appendTo($container);
 	window.parent.$(".loadercontent").remove();
    var h = window.parent.$("#iframedata").height()-38;
 	var w = window.parent.$("#iframedata").width();
 	loadScriptsForTinyMCE("/sites/all/modules/core/exp_sp_core/js/videojs/video.js");
 	loadCssFilesH5p('/sites/all/modules/core/exp_sp_core/js/videojs/video-js.min.css');

 	
 	
 	videojs.options.flash.swf = "/sites/all/modules/core/exp_sp_core/js/videojs/video-js.swf";
	
	H5P.jQuery(".h5p-interactive-video").css("height","inherit");
	H5P.jQuery(".h5p-video-wrapper").css("height","inherit");
	window.parent.$(".loadercontent").remove();	
	
	if(sources[0].path.indexOf("vimeo") > 0)
	{
	 	loadScriptsForTinyMCE("/sites/all/modules/core/exp_sp_core/js/videojs/Vimeo.js");
	 	loadScriptsForTinyMCE("/sites/all/libraries/h5plibraries/H5P.Video-1.2/scripts/vimeoplayer.js");

		// get vimeo id from url
		var r = /(videos|video|channels|\.com)\/([\d]+)/;
		var vimeoId=sources[0].path.match(r)[2];
		

		html = '<iframe style="height:180%;width:100%;left:0;position:absolute;top:-40%" src="https://player.vimeo.com/video/'+vimeoId+'?player_id=my_video_2" id="my_video_2" class="Fullsc video-js" width="100%" height="100%" frameborder="0" webkitallowfullscreen mozallowfullscreen allowfullscreen></iframe>';
		// html = '<iframe style="width:100%;"
		// src="https://player.vimeo.com/video/76979871?api=1&player_id=my_video_2"
		// id="my_video_2" class="Fullsc video-js" width="100%" height="100%"
		// frameborder="0" webkitallowfullscreen mozallowfullscreen
		// allowfullscreen></iframe>';
		
		H5P.jQuery(".h5p-video-flash").append(html);
		var iframe = document.getElementById('my_video_2');
	    fp = new Vimeo.Player(iframe);
	    console.log("fp:",fp);
	    console.log("size::",H5P.jQuery(".h5p-video-flash").size());
	    
	    window.setTimeout(function(){
	    	    
	    	fp.on("loaded", function () {
	    	console.log("fp.addEvent ready1....");
	   	fp.play();
	    	window.parent.$("#iframe_editor_wrapper").css("visibility","visible");

	    fp.getDuration().then(function(duration) {
	    		vimeoDuration = duration;
	    }).catch(function(error) {
	        // an error occurred
	    });
	    
	    
	    

		
		fp.getVideoWidth().then(function(width) {
			vimeoWidth = width;
		}).catch(function(error) {
		    // an error occurred
		});
		fp.getVideoHeight().then(function(height) {
			vimeoHeight = height;
		}).catch(function(error) {
		    // an error occurred
		});
		fp.on('timeupdate',function(evt){
    		vimeoCurrentTime = evt.seconds;
		})
		fp.on('progress',function(evt){
    			vimeoBuffered = evt.percent*100;
		})

	
	    
	    
	    
	    
	    fp.on('play', function() {
	    		self.trigger('stateChange', H5P.Video.PLAYING);
	        console.log('played the video!');
	    });

	    fp.on('pause', function() {
	    		self.trigger('stateChange', H5P.Video.PAUSED);
	        console.log('pause the video!');
	    });
	    fp.on( 'ended', 			function( e ) { window.console.log( 'videoJS: ended111' );self.trigger('stateChange', H5P.Video.ENDED);});
	    });
	    },1000); // timeout

	}
	else
	{
		var video_dom_leng = H5P.jQuery(".my_video_presentation").size() == 0?1:H5P.jQuery(".my_video_presentation").size()+1;
		
		if(window.parent.$("#contentauthor-presentation-addedit-form").size() > 0 || H5P.jQuery(".h5p-course-presentation").size() > 0)
			html =   "<video id='my_video_presentation"+video_dom_leng+"' class='video-js my_video_presentation  vjs-default-skin vjs-big-play-centered  vjs-has-started vjs-nofull vjs-paused vjs-ended vjs-user-active ' style='height:100%;width:100%;'  autoplay=false   data-setup=\"{}\">"; //style='height:inherit;width:auto;'
		else
			html =   "<video id='my_video_2' class='Fullsc video-js' autoplay=true controls='false' preload='auto' width='"+w+"' height='"+h+"' data-setup=\"{}\">";
	// html += "<source
	// src='rtmp://192.241.173.210/oflaDemo/Saravanan_4/256/Saravanan.mp4'
	// type='rtmp/mp4' />"; //what about webm?

	    html += "<source src='"+sources[0].path+"' type='video/mp4' />";
	    html += "<source src='"+sources[0].path+"' type='video/webm' />";
	    html += "<source src='"+sources[0].path+"' type='rtmp/mp4' />";
		html +="</video>";
	
	$container.find(".h5p-video-flash").append(html);

	console.log("1115:"+sources[0].path);
	var playing = false,firstTime = true;
	
	if(window.parent.$("#contentauthor-presentation-addedit-form").size() > 0 || H5P.jQuery(".h5p-course-presentation").size() > 0)
		{
		fp = videojs("my_video_presentation"+video_dom_leng,{controls: options.controls,autoplay:options.autoplay}).ready(function(){});
		
		}
	else
		{
	fp = videojs("my_video_2",{controls: options.controls,autoplay:options.autoplay}).ready(function(){
  	var player = this;
  	// this.play();
    player.on( 'loadedmetadata', 	function( e ) { 
  																				console.log('load meta...');
  																				self.trigger('ready');
  																				self.trigger('loaded');
  																				self.trigger("addControls");
  																				window.setTimeout(function(){
  																				playing = true;
																				firstTime = false;
  																				
  																				self.trigger('stateChange', H5P.Video.PLAYING);
  																				},50);
  														
  												});
	// player.on( 'timeupdate', function( e ) { window.console.log( 'videoJS:
	// ended111' );self.trigger('stateChange', H5P.Video.ENDED);});

	player.on( 'ended', 			function( e ) { window.console.log( 'videoJS: ended111' );self.trigger('stateChange', H5P.Video.ENDED);});
	player.on( 'pause', 			function( e ) { window.console.log( 'videoJS: pause' );
				firstTime = false;
				self.trigger('stateChange', H5P.Video.PAUSED); 
				});
	
	
	player.on( 'play', 				function( e ) {
													if(firstTime == false)
														self.trigger('stateChange', H5P.Video.PLAYING);
												 });
	
	
	player.on( 'seeking', 			function( e ) { console.log( 'videoJS: seeking' );
	
			self.trigger('stateChange', H5P.Video.BUFFERING); 
			});
	
	// play/pause
	
	var obt = H5P.jQuery(".vjs-text-track-display");  	
  	
	obt.css({ "pointer-events": "auto" });  	
  	
  	obt.on('click', function() {  		
  		if (player.paused()) { 
  			player.play();
  	  	  }else if(player.play()){
  	  		player.pause();
  	  	  }
  	});
  	
  	
	/*
	 * player.on( 'waiting', function( e ) { window.console.log( 'videoJS:
	 * waiting' ); //self.trigger('stateChange', H5P.Video.BUFFERING); });
	 */


	// player.on( 'loadstart', function( e ) { window.console.log( 'videoJS:
	// loadstart' ); self.trigger('stateChange', H5P.Video.PLAYING);});
	
	
  	// player.on( 'durationchange', function( e ) { window.console.log(
	// 'videoJS: durationchange' ); self.trigger('stateChange',
	// H5P.Video.PLAYING); });
	
	// player.on( 'error', function( e ) { window.console.log( 'videoJS: error'
	// ); self.trigger('error', message);});
	
	// player.on( 'loadedalldata', function( e ) { window.console.log( 'videoJS:
	// loadedalldata' );self.trigger('stateChange', H5P.Video.PLAYING); });
	
	
	
	
	
	// player.on( 'seeked', function( e ) { alert( 'videoJS: seeked' ); });
	// player.on( 'seeking', function( e ) { alert( 'videoJS: seeking'
	// );self.trigger('stateChange', InteractiveVideo.SEEKING); });
	// player.on( 'waiting', function( e ) { window.console.log( 'videoJS:
	// waiting' );self.trigger('stateChange', H5P.Video.BUFFERING); });
// player.on( 'contentplayback', function( e ) { window.console.log( 'videoJS:
// contentplayback' );self.trigger('stateChange', H5P.Video.PLAYING); });
	});
		}
	}
	
	H5P.jQuery(".h5p-video").css("margin-top","0px");
	
 	};
    /**
	 * Get list of available qualities. Not available until after play.
	 * 
	 * @public
	 * @returns {Array}
	 */
    self.getQualities = function () {
      return;
      // Create reverse list
      var options = [];
      for (var q in qualities) {
        if (qualities.hasOwnProperty(q)) {
          options.splice(0, 0, {
            name: q,
            label: qualities[q].label
          });
        }
      }

      if (options.length < 2) {
        // Do not return if only one quality.
        return;
      }

      return options;
    };

    /**
	 * Get current playback quality. Not available until after play.
	 * 
	 * @public
	 * @returns {String}
	 */
    self.getQuality = function () {
      return currentQuality;
    };

    /**
	 * Set current playback quality. Not available until after play. Listen to
	 * event "qualityChange" to check if successful.
	 * 
	 * @public
	 * @params {String} [quality]
	 */
    self.setQuality = function (quality) {
      if (qualities[quality] === undefined || quality === currentQuality) {
        return; // Invalid quality
      }

      // Keep track of last choice
      setPreferredQuality(quality);

      // Avoid multiple loaded events if changing quality multiple times.
      if (!stateBeforeChangingQuality) {
        // Keep track of last state
        stateBeforeChangingQuality = lastState;

        // Keep track of current time
        currentTimeBeforeChangingQuality = video.currentTime;
      }

      // Keep track of current quality
      currentQuality = quality;
      self.trigger('qualityChange', currentQuality);

      // Display throbber
      self.trigger('stateChange', H5P.Video.BUFFERING);

      // Change source
      fp.setClip(qualities[quality].source.path);
      fp.startBuffering();
    };

    /**
	 * Starts the video.
	 * 
	 * @public
	 */
    self.play = function () {
      if ($error.is(':visible')) {
        return;
      }

      fp.play();
    };

    /**
	 * Pauses the video.
	 * 
	 * @public
	 */
    self.pause = function () {
      fp.pause();
    };

    /**
	 * Seek video to given time.
	 * 
	 * @public
	 * @param {Number}
	 *            time
	 */
    self.seek = function (time) {
     if(playerType == "flowplayer")
     	fp.seek(time);
     else if(playerType  == "videojs")
     	fp.currentTime(time);
     else if(playerType == "vimeo")
    	 {
    	 fp.setCurrentTime(time).then(function(seconds) {
    		    // seconds = the actual time that the player seeked to
    		}).catch(function(error) {
    		    switch (error.name) {
    		        case 'RangeError':
    		            // the time was less than 0 or greater than the video’s
						// duration
    		            break;

    		        default:
    		            // some other error occurred
    		            break;
    		    }
    		});
    	 }
      
    };

    /**
	 * Get elapsed time since video beginning.
	 * 
	 * @public
	 * @returns {Number}
	 */
    self.getCurrentTime = function () {
        if(playerType == "flowplayer")
        		return fp.getTime();
        else if(playerType  == "videojs")
        		return fp.currentTime();
        else if(playerType == "vimeo")
        	 	return vimeoCurrentTime;
    };

    /**
	 * Get total video duration time.
	 * 
	 * @public
	 * @returns {Number}
	 */
    self.getDuration = function () {
		if(playerType == "flowplayer")
    		return fp.getClip().metaData.duration;
    	else if(playerType  == "videojs")
    		return fp.duration();
    	else if(playerType == "vimeo")
    	{
    		return vimeoDuration;
    	}
    };

    /**
	 * Get percentage of video that is buffered.
	 * 
	 * @public
	 * @returns {Number} Between 0 and 100
	 */
    self.getBuffered = function () {
    	 console.log("self.getbuffered called...:"+vimeoBuffered+"===="+playerType);
       // console.log("fp.currentTime()/fp.duration()*100:"+fp.currentTime()/fp.duration()*100);
		if(playerType == "flowplayer")
			return fp.getClip().buffer;
    	else if(playerType  == "videojs")
    		return fp.currentTime()/fp.duration()*100;
    	else if(playerType == "vimeo")
    		return vimeoBuffered;
    };

    /**
	 * Turn off video sound.
	 * 
	 * @public
	 */
    self.mute = function () {
    	  console.log("mute:");
    	  player.getVolume().then(function(volume) {
    		     vimeoVolume = volume;
    		}).catch(function(error) {
    		    // an error occurred
    		});
    	  
    	  if(playerType == "vimeo")
    	  {
    		  fp.setVolume(0.0).then(function(volume) {
    			    // volume was set
    			}).catch(function(error) {
    			    switch (error.name) {
    			        case 'RangeError':
    			            // the volume was less than 0 or greater than 1
    			            break;

    			        default:
    			            // some other error occurred
    			            break;
    			    }
    			});
    	  }
    	  else
      fp.mute();
    };

    /**
	 * Turn on video sound.
	 * 
	 * @public
	 */
    self.unMute = function () {
    	console.log("unMute:");
    	 if(playerType == "vimeo")
   	  {
   		  fp.setVolume(vimeoVolume).then(function(volume) {
   			    // volume was set
   			}).catch(function(error) {
   			    switch (error.name) {
   			        case 'RangeError':
   			            // the volume was less than 0 or greater than 1
   			            break;

   			        default:
   			            // some other error occurred
   			            break;
   			    }
   			});
   	  }
    	 else
      fp.unmute();
    };

    /**
	 * Check if video sound is turned on or off.
	 * 
	 * @public
	 * @returns {Boolean}
	 */
    self.isMuted = function () {
      return fp.muted;
    };

    /**
	 * Returns the video sound level.
	 * 
	 * @public
	 * @returns {Number} Between 0 and 100.
	 */
    self.getVolume = function () {
    	 console.log("getVolume:"+fp.volumeLevel);
      return fp.volumeLevel * 100;
    };

    /**
	 * Set video sound level.
	 * 
	 * @public
	 * @param {Number}
	 *            volume Between 0 and 100.
	 */
    self.setVolume = function (level) {
      fp.volume(level / 100);
    };

    // Handle resize events
    self.on('resize', function () {
		if(playerType == "flowplayer"){
	      var $object = H5P.jQuery(fp.getParent()).children('object');
    	  var clip = fp.getClip();

      	  if (clip !== undefined) {
        	$object.css('height', $object.width() * (clip.metaData.height / clip.metaData.width));
      	  }
    	}else  if(playerType  == "videojs")
    	{
    	
    	var $object = H5P.jQuery("#my_video_2").children('object');
    	// alert(fp.height()+"==="+fp.width());
    	$object.css('height', $object.width() * (fp.height() / fp.width()));
    	}
    	else if(playerType == "vimeo"){
    		var $object = H5P.jQuery("#my_video_2");
    		console.log("vimeoWidth:"+vimeoWidth+"===vimeoHeight:"+vimeoHeight);
    		// $object.css('height', vimeoWidth* (vimeoHeight / vimeoWidth));
    		
    		
        	
    	}
    });
  }

  /**
	 * Check to see if we can play any of the given sources.
	 * 
	 * @public
	 * @static
	 * @param {Array}
	 *            sources
	 * @returns {Boolean}
	 */
  Flash.canPlay = function (sources) {
    // Cycle through sources
    
    for (var i = 0; i < sources.length; i++) {
    	  // vimeo support
      if (sources[i].mime == "video/vimeo" || sources[i].mime === 'video/mp4' || /\.mp4$/.test(sources[i].mime)) {
        return true; // We only play mp4
      }
    }
  };

  return Flash;
})(H5P.jQuery);

// Register video handler
H5P.videoHandlers = H5P.videoHandlers || [];
H5P.videoHandlers.push(H5P.VideoFlash);
;/** @namespace H5P */
H5P.Video = (function ($, ContentCopyrights, MediaCopyright, handlers) {

  /**
   * The ultimate H5P video player!
   *
   * @class
   * @param {Object} parameters Options for this library.
   * @param {Object} parameters.visuals Visual options
   * @param {Object} parameters.playback Playback options
   * @param {Object} parameters.a11y Accessibility options
   * @param {Boolean} [parameters.startAt] Start time of video
   * @param {Number} id Content identifier
   */
  function Video(parameters, id) {
	  
    var self = this;
    // Initialize event inheritance
    H5P.EventDispatcher.call(self);
    
    // Default language localization
    parameters = $.extend(true, parameters, {
      l10n: {
        name: 'Video',
        loading: 'Video player loading...',
        noPlayers: 'Found no video players that supports the given video format.',
        noSources: 'Video is missing sources.',
        aborted: 'Media playback has been aborted.',
        networkFailure: 'Network failure.',
        cannotDecode: 'Unable to decode media.',
        formatNotSupported: 'Video format not supported.',
        mediaEncrypted: 'Media encrypted.',
        unknownError: 'Unknown error.',
        invalidYtId: 'Invalid YouTube ID.',
        unknownYtId: 'Unable to find video with the given YouTube ID.',
        restrictedYt: 'The owner of this video does not allow it to be embedded.'
      }
    });

    parameters.a11y = parameters.a11y || [];
    parameters.playback = parameters.playback || {};
    parameters.visuals = parameters.visuals || {};

    /** @private */

    var sources = [];
    if (parameters.sources) {
      for (var i = 0; i < parameters.sources.length; i++) {
        // Clone to avoid changing of parameters.
        var source = $.extend(true, {}, parameters.sources[i]);

        // Create working URL without html entities.
        source.path = H5P.getPath($cleaner.html(source.path).text(), id);
         //h5pcustomize
	      if(source.path != null && source.path.indexOf("^^^")>0)
    	  {
	  		 var pathArr = source.path.split("^^^");
	  		 var path1withfileextension = pathArr[0];
	  		 var path1withfileextensionarr = path1withfileextension.split("/");
	  		 var actualUrl = "";
	  		 for(var j = 0; j < path1withfileextensionarr.length -1; j++)
	  		 {
	  		 	if(actualUrl == "")
	  		 		actualUrl = path1withfileextensionarr[j];
	  		 	else
	  		 		actualUrl += "/"+path1withfileextensionarr[j];
	  		 	
	  		 }
	  		 
	  		 source.path = actualUrl + "/"+pathArr[1];
	  		}
	  	
        sources.push(source);
      }
    }
    

    /** @private */
    var tracks = [];
    
	
    parameters.a11y.forEach(function (track) {
      // Clone to avoid changing of parameters.
      var clone = $.extend(true, {}, track);

      // Create working URL without html entities
      if (clone.track && clone.track.path) {
        clone.track.path = H5P.getPath($cleaner.html(clone.track.path).text(), id);
        tracks.push(clone);
      }
    });

    /**
     * Attaches the video handler to the given container.
     * Inserts text if no handler is found.
     *
     * @public
     * @param {jQuery} $container
     */
    self.attach = function ($container) {
      $container.addClass('h5p-video').html('');
	  H5P.jQuery(".h5peditor-form > .tree > .wizard > .h5peditor-label").hide(); //Interactive editor title
	  H5P.jQuery(".h5peditor-form > .tree > .wizard > .h5peditor-tabs").hide(); //tabs
	  console.log("self::",self);
      if (self.appendTo !== undefined) {
        //alert(self.attachVideoJS);
    	    console.log('self.....',self);
    	    var playerType = "flowplayer";
    	    if(sources[0].path.indexOf("rtmp://")==0)
    	    		playerType = "videojs";
    	    else if(sources[0].path.indexOf("vimeo")>0 )
    	    		playerType = "vimeo"; 
    	    if(window.parent.$("#contentauthor-presentation-addedit-form").size() > 0 || H5P.jQuery(".h5p-course-presentation").size() > 0)
    	    		playerType = "videojs";
        if(playerType == "videojs") // && (window.parent.$("#iframe_editor_wrapper_container").size() == 0)) // && window.parent.$("#contentauthor-presentation-addedit-form").size() == 0))//only for preview
        		self.attachVideoJS($container);
        else if(playerType == "vimeo")
        		self.attachVideoJS($container);
        else
        		self.appendTo($container);
      }
      else {
        if (sources.length) {
          $container.text(parameters.l10n.noPlayers);
        }
        else {
          $container.text(parameters.l10n.noSources);
        }
      }

    };

    /**
     * Gather copyright information for the current video.
     *
     * @public
     * @returns {ContentCopyrights}
     */
    self.getCopyrights = function () {
      if (!sources[0] || !sources[0].copyright) {
        return;
      }

      // Use copyright information from H5P media field
      var info = new ContentCopyrights();
      info.addMedia(new MediaCopyright(sources[0].copyright));

      return info;
    };

    // Resize the video when we know its aspect ratio
    self.on('loaded', function () {
      self.trigger('resize');
    });

    // Find player for video sources
    if (sources.length) {
     console.log("sources:",sources);
     console.log("handlers:",handlers);
     
     if(window.parent.$("#contentauthor-presentation-addedit-form").size() > 0 || H5P.jQuery(".h5p-course-presentation").size()>0) //for presentation editor and preview only use videojs
    	 {
    	 	var handler = handlers[2]; //flash videojs player
    	 	handler.call(self, sources, {
	            controls: parameters.visuals.controls,
	            autoplay: parameters.playback.autoplay,
	            loop: parameters.playback.loop,
	            fit: parameters.visuals.fit,
	            poster:undefined, //h5pcustomize parameters.visuals.poster === undefined ? undefined : H5P.getPath(parameters.visuals.poster.path, id),
	            startAt: parameters.startAt || 0,
	            tracks: tracks
	          }, parameters.l10n);
    	 	return "";
    	 }
     else
    	 {
	      for (var i = 0; i < handlers.length; i++) {
	      	
	        var handler = handlers[i];
	        if (handler.canPlay !== undefined && handler.canPlay(sources))
	         {
	          handler.call(self, sources, {
	            controls: parameters.visuals.controls,
	            autoplay: parameters.playback.autoplay,
	            loop: parameters.playback.loop,
	            fit: parameters.visuals.fit,
	            poster:undefined, //h5pcustomize parameters.visuals.poster === undefined ? undefined : H5P.getPath(parameters.visuals.poster.path, id),
	            startAt: parameters.startAt || 0,
	            tracks: tracks
	          }, parameters.l10n); 
	          return;
	        }
	      }
	    }
     }
  }

  // Extends the event dispatcher
  Video.prototype = Object.create(H5P.EventDispatcher.prototype);
  Video.prototype.constructor = Video;

  // Player states
  /** @constant {Number} */
  Video.ENDED = 0;
  /** @constant {Number} */
  Video.PLAYING = 1;
  /** @constant {Number} */
  Video.PAUSED = 2;
  /** @constant {Number} */
  Video.BUFFERING = 3;

  // Used to convert between html and text, since URLs have html entities.
  var $cleaner = H5P.jQuery('<div/>');

  return Video;
})(H5P.jQuery, H5P.ContentCopyrights, H5P.MediaCopyright, H5P.videoHandlers || []);
;var H5P = H5P || {};

/**
 * H5P audio module
 *
 * @external {jQuery} $ H5P.jQuery
 */
H5P.Audio = (function ($) {
  /**
  * @param {Object} params Options for this library.
  * @param {Number} id Content identifier
  * @returns {undefined}
  */
  function C(params, id) {
    H5P.EventDispatcher.call(this);
    this.contentId = id;
    this.params = params;

    this.params = $.extend({}, {
      playerMode: 'minimalistic',
      fitToWrapper: false,
      controls: true,
      autoplay: false
    }, params);

    // Use new copyright information if available. Fallback to old.
    if (params.files !== undefined
      && params.files[0] !== undefined
      && params.files[0].copyright !== undefined) {

      this.copyright = params.files[0].copyright;
    }
    else if (params.copyright !== undefined) {
      this.copyright = params.copyright;
    }
    this.on('resize', this.resize, this);
  }

  C.prototype = Object.create(H5P.EventDispatcher.prototype);
  C.prototype.constructor = C;

  /**
   * Adds a minimalistic audio player with only "play" and "pause" functionality.
   *
   * @param {jQuery} $container Container for the player.
   */
  C.prototype.addMinimalAudioPlayer = function ($container) {
    var INNER_CONTAINER = 'h5p-audio-inner';
    var AUDIO_BUTTON = 'h5p-audio-minimal-button';
    var PLAY_BUTTON = 'h5p-audio-minimal-play';
    var PAUSE_BUTTON = 'h5p-audio-minimal-pause';

    var self = this;
    this.$container = $container;

    self.$inner = $('<div/>', {
      'class': INNER_CONTAINER
    }).appendTo($container);

    var audioButton = $('<button/>', {
      'class': AUDIO_BUTTON+" "+PLAY_BUTTON
    }).appendTo(self.$inner)
      .click( function () {
        if (self.audio.paused) {
          self.play();
        } else {
          self.pause();
        }
      });

    //Fit to wrapper
    if (this.params.fitToWrapper) {
      audioButton.css({
        'width': '100%',
        'height': '100%'
      });
    }

    // cpAutoplay is passed from coursepresentation
    if (this.params.autoplay) {
      self.play();
    }

    //Event listeners that change the look of the player depending on events.
    self.audio.addEventListener('ended', function () {
      audioButton.removeClass(PAUSE_BUTTON).addClass(PLAY_BUTTON);
    });

    self.audio.addEventListener('play', function () {
      audioButton.removeClass(PLAY_BUTTON).addClass(PAUSE_BUTTON);
    });

    self.audio.addEventListener('pause', function () {
      audioButton.removeClass(PAUSE_BUTTON).addClass(PLAY_BUTTON);
    });

    this.$audioButton = audioButton;
    //Scale icon to container
    self.resize();
  };

  /**
   * Resizes the audio player icon when the wrapper is resized.
   */
  C.prototype.resize = function () {
    // Find the smallest value of height and width, and use it to choose the font size.
    if (this.params.fitToWrapper && this.$container && this.$container.width()) {
      var w = this.$container.width();
      var h = this.$container.height();
      if (w < h) {
        this.$audioButton.css({'font-size': w / 2 + 'px'});
      }
      else {
        this.$audioButton.css({'font-size': h / 2 + 'px'});
      }
    }
  };


  return C;
})(H5P.jQuery);

/**
 * Wipe out the content of the wrapper and put our HTML in it.
 *
 * @param {jQuery} $wrapper Our poor container.
 */
H5P.Audio.prototype.attach = function ($wrapper) {
  $wrapper.addClass('h5p-audio-wrapper');

  // Check if browser supports audio.
  var audio = document.createElement('audio');
  if (audio.canPlayType === undefined) {
    // Try flash
    this.attachFlash($wrapper);
    return;
  }

  // Add supported source files.
  if (this.params.files !== undefined && this.params.files instanceof Object) {
    for (var i = 0; i < this.params.files.length; i++) {
      var file = this.params.files[i];
      //h5pcustomize
      if(file.path != null && file.path.indexOf("^^^")>0)
      {
	  	var pathArr = file.path.split("^^^");
	  	file.path = pathArr[1];
	  }
      if (audio.canPlayType(file.mime)) {
        var source = document.createElement('source');
        source.src = H5P.getPath(file.path, this.contentId);
        source.type = file.mime;
        audio.appendChild(source);
      }
    }
  }

  if (!audio.children.length) {
    // Try flash
    this.attachFlash($wrapper);
    return;
  }

  if (this.endedCallback !== undefined) {
    audio.addEventListener('ended', this.endedCallback, false);
  }

  audio.className = 'h5p-audio';
  audio.controls = this.params.controls === undefined ? true : this.params.controls;
  audio.preload = 'auto';
  audio.style.display = 'block';

  if (this.params.fitToWrapper === undefined || this.params.fitToWrapper) {
    audio.style.width = '100%';
    audio.style.height = '100%';
  }

  this.audio = audio;

  if (this.params.playerMode === 'minimalistic') {
    audio.controls = false;
    this.addMinimalAudioPlayer($wrapper);
  }
  else {
    audio.autoplay = this.params.autoplay === undefined ? false : this.params.autoplay;
    $wrapper.html(audio);
  }
};

/**
 * Attaches a flash audio player to the wrapper.
 *
 * @param {jQuery} $wrapper Our dear container.
 */
H5P.Audio.prototype.attachFlash = function ($wrapper) {
  if (this.params.files !== undefined && this.params.files instanceof Object) {
    for (var i = 0; i < this.params.files.length; i++) {
      var file = this.params.files[i];
      if (file.mime === 'audio/mpeg' || file.mime === 'audio/mp3') {
        var audioSource = H5P.getPath(file.path, this.contentId);
        break;
      }
    }
  }

  if (audioSource === undefined) {
    $wrapper.text('No supported audio files found.');
    if (this.endedCallback !== undefined) {
      this.endedCallback();
    }
    return;
  }

  var options = {
    buffering: true,
    clip: {
      url: window.location.protocol + '//' + window.location.host + audioSource,
      autoPlay: this.params.autoplay === undefined ? false : this.params.autoplay,
      scaling: 'fit'
    },
    plugins: {
      controls: null
    }
  };

  if (this.params.controls === undefined || this.params.controls) {
    options.plugins.controls = {
      fullscreen: false,
      autoHide: false
    };
  }

  if (this.endedCallback !== undefined) {
    options.clip.onFinish = this.endedCallback;
    options.clip.onError = this.endedCallback;
  }

  this.flowplayer = flowplayer($wrapper[0], {
    src: "http://releases.flowplayer.org/swf/flowplayer-3.2.16.swf",
    wmode: "opaque"
  }, options);
};

/**
 * Stop the audio. TODO: Rename to pause?
 *
 * @returns {undefined}
 */
H5P.Audio.prototype.stop = function () {
  if (this.flowplayer !== undefined) {
    this.flowplayer.stop().close().unload();
  }
  if (this.audio !== undefined) {
    this.audio.pause();
  }
};

/**
 * Play
 */
H5P.Audio.prototype.play = function () {
  if (this.flowplayer !== undefined) {
    this.flowplayer.play();
  }
  if (this.audio !== undefined) {
    this.audio.play();
  }
};

/**
 * @public
 * Pauses the audio.
 */
H5P.Audio.prototype.pause = function () {
  if (this.audio !== undefined) {
    this.audio.pause();
  }
};

/**
 * Gather copyright information for the current content.
 *
 * @returns {H5P.ContentCopyrights} Copyright information
 */
H5P.Audio.prototype.getCopyrights = function () {
  if (this.copyright === undefined) {
    return;
  }

  var info = new H5P.ContentCopyrights();
  info.addMedia(new H5P.MediaCopyright(this.copyright));

  return info;
};
;var oldTether = window.Tether;
!function(t,e){"function"==typeof define&&define.amd?define(e):"object"==typeof exports?module.exports=e(require,exports,module):t.Tether=e()}(this,function(t,e,o){"use strict";function i(t,e){if(!(t instanceof e))throw new TypeError("Cannot call a class as a function")}function n(t){var e=getComputedStyle(t),o=e.position;if("fixed"===o)return t;for(var i=t;i=i.parentNode;){var n=void 0;try{n=getComputedStyle(i)}catch(r){}if("undefined"==typeof n||null===n)return i;var s=n.overflow,a=n.overflowX,f=n.overflowY;if(/(auto|scroll)/.test(s+f+a)&&("absolute"!==o||["relative","absolute","fixed"].indexOf(n.position)>=0))return i}return document.body}function r(t){var e=void 0;t===document?(e=document,t=document.documentElement):e=t.ownerDocument;var o=e.documentElement,i={},n=t.getBoundingClientRect();for(var r in n)i[r]=n[r];var s=x(e);return i.top-=s.top,i.left-=s.left,"undefined"==typeof i.width&&(i.width=document.body.scrollWidth-i.left-i.right),"undefined"==typeof i.height&&(i.height=document.body.scrollHeight-i.top-i.bottom),i.top=i.top-o.clientTop,i.left=i.left-o.clientLeft,i.right=e.body.clientWidth-i.width-i.left,i.bottom=e.body.clientHeight-i.height-i.top,i}function s(t){return t.offsetParent||document.documentElement}function a(){var t=document.createElement("div");t.style.width="100%",t.style.height="200px";var e=document.createElement("div");f(e.style,{position:"absolute",top:0,left:0,pointerEvents:"none",visibility:"hidden",width:"200px",height:"150px",overflow:"hidden"}),e.appendChild(t),document.body.appendChild(e);var o=t.offsetWidth;e.style.overflow="scroll";var i=t.offsetWidth;o===i&&(i=e.clientWidth),document.body.removeChild(e);var n=o-i;return{width:n,height:n}}function f(){var t=void 0===arguments[0]?{}:arguments[0],e=[];return Array.prototype.push.apply(e,arguments),e.slice(1).forEach(function(e){if(e)for(var o in e)({}).hasOwnProperty.call(e,o)&&(t[o]=e[o])}),t}function h(t,e){if("undefined"!=typeof t.classList)e.split(" ").forEach(function(e){e.trim()&&t.classList.remove(e)});else{var o=new RegExp("(^| )"+e.split(" ").join("|")+"( |$)","gi"),i=u(t).replace(o," ");p(t,i)}}function l(t,e){if("undefined"!=typeof t.classList)e.split(" ").forEach(function(e){e.trim()&&t.classList.add(e)});else{h(t,e);var o=u(t)+(" "+e);p(t,o)}}function d(t,e){if("undefined"!=typeof t.classList)return t.classList.contains(e);var o=u(t);return new RegExp("(^| )"+e+"( |$)","gi").test(o)}function u(t){return t.className instanceof SVGAnimatedString?t.className.baseVal:t.className}function p(t,e){t.setAttribute("class",e)}function c(t,e,o){o.forEach(function(o){-1===e.indexOf(o)&&d(t,o)&&h(t,o)}),e.forEach(function(e){d(t,e)||l(t,e)})}function i(t,e){if(!(t instanceof e))throw new TypeError("Cannot call a class as a function")}function g(t,e){var o=void 0===arguments[2]?1:arguments[2];return t+o>=e&&e>=t-o}function m(){return"undefined"!=typeof performance&&"undefined"!=typeof performance.now?performance.now():+new Date}function v(){for(var t={top:0,left:0},e=arguments.length,o=Array(e),i=0;e>i;i++)o[i]=arguments[i];return o.forEach(function(e){var o=e.top,i=e.left;"string"==typeof o&&(o=parseFloat(o,10)),"string"==typeof i&&(i=parseFloat(i,10)),t.top+=o,t.left+=i}),t}function y(t,e){return"string"==typeof t.left&&-1!==t.left.indexOf("%")&&(t.left=parseFloat(t.left,10)/100*e.width),"string"==typeof t.top&&-1!==t.top.indexOf("%")&&(t.top=parseFloat(t.top,10)/100*e.height),t}function b(t,e){return"scrollParent"===e?e=t.scrollParent:"window"===e&&(e=[pageXOffset,pageYOffset,innerWidth+pageXOffset,innerHeight+pageYOffset]),e===document&&(e=e.documentElement),"undefined"!=typeof e.nodeType&&!function(){var t=r(e),o=t,i=getComputedStyle(e);e=[o.left,o.top,t.width+o.left,t.height+o.top],U.forEach(function(t,o){t=t[0].toUpperCase()+t.substr(1),"Top"===t||"Left"===t?e[o]+=parseFloat(i["border"+t+"Width"]):e[o]-=parseFloat(i["border"+t+"Width"])})}(),e}var w=function(){function t(t,e){for(var o=0;o<e.length;o++){var i=e[o];i.enumerable=i.enumerable||!1,i.configurable=!0,"value"in i&&(i.writable=!0),Object.defineProperty(t,i.key,i)}}return function(e,o,i){return o&&t(e.prototype,o),i&&t(e,i),e}}(),C=void 0;"undefined"==typeof C&&(C={modules:[]});var O=function(){var t=0;return function(){return++t}}(),E={},x=function(t){var e=t._tetherZeroElement;"undefined"==typeof e&&(e=t.createElement("div"),e.setAttribute("data-tether-id",O()),f(e.style,{top:0,left:0,position:"absolute"}),t.body.appendChild(e),t._tetherZeroElement=e);var o=e.getAttribute("data-tether-id");if("undefined"==typeof E[o]){E[o]={};var i=e.getBoundingClientRect();for(var n in i)E[o][n]=i[n];T(function(){delete E[o]})}return E[o]},A=[],T=function(t){A.push(t)},S=function(){for(var t=void 0;t=A.pop();)t()},W=function(){function t(){i(this,t)}return w(t,[{key:"on",value:function(t,e,o){var i=void 0===arguments[3]?!1:arguments[3];"undefined"==typeof this.bindings&&(this.bindings={}),"undefined"==typeof this.bindings[t]&&(this.bindings[t]=[]),this.bindings[t].push({handler:e,ctx:o,once:i})}},{key:"once",value:function(t,e,o){this.on(t,e,o,!0)}},{key:"off",value:function(t,e){if("undefined"==typeof this.bindings||"undefined"==typeof this.bindings[t])if("undefined"==typeof e)delete this.bindings[t];else for(var o=0;o<this.bindings[t].length;)this.bindings[t][o].handler===e?this.bindings[t].splice(o,1):++o}},{key:"trigger",value:function(t){if("undefined"!=typeof this.bindings&&this.bindings[t])for(var e=0;e<this.bindings[t].length;){var o=this.bindings[t][e],i=o.handler,n=o.ctx,r=o.once,s=n;"undefined"==typeof s&&(s=this);for(var a=arguments.length,f=Array(a>1?a-1:0),h=1;a>h;h++)f[h-1]=arguments[h];i.apply(s,f),r?this.bindings[t].splice(e,1):++e}}}]),t}();C.Utils={getScrollParent:n,getBounds:r,getOffsetParent:s,extend:f,addClass:l,removeClass:h,hasClass:d,updateClasses:c,defer:T,flush:S,uniqueId:O,Evented:W,getScrollBarSize:a};var M=function(){function t(t,e){var o=[],i=!0,n=!1,r=void 0;try{for(var s,a=t[Symbol.iterator]();!(i=(s=a.next()).done)&&(o.push(s.value),!e||o.length!==e);i=!0);}catch(f){n=!0,r=f}finally{try{!i&&a["return"]&&a["return"]()}finally{if(n)throw r}}return o}return function(e,o){if(Array.isArray(e))return e;if(Symbol.iterator in Object(e))return t(e,o);throw new TypeError("Invalid attempt to destructure non-iterable instance")}}(),w=function(){function t(t,e){for(var o=0;o<e.length;o++){var i=e[o];i.enumerable=i.enumerable||!1,i.configurable=!0,"value"in i&&(i.writable=!0),Object.defineProperty(t,i.key,i)}}return function(e,o,i){return o&&t(e.prototype,o),i&&t(e,i),e}}();if("undefined"==typeof C)throw new Error("You must include the utils.js file before tether.js");var P=C.Utils,n=P.getScrollParent,r=P.getBounds,s=P.getOffsetParent,f=P.extend,l=P.addClass,h=P.removeClass,c=P.updateClasses,T=P.defer,S=P.flush,a=P.getScrollBarSize,k=function(){for(var t=document.createElement("div"),e=["transform","webkitTransform","OTransform","MozTransform","msTransform"],o=0;o<e.length;++o){var i=e[o];if(void 0!==t.style[i])return i}}(),B=[],_=function(){B.forEach(function(t){t.position(!1)}),S()};!function(){var t=null,e=null,o=null,i=function n(){return"undefined"!=typeof e&&e>16?(e=Math.min(e-16,250),void(o=setTimeout(n,250))):void("undefined"!=typeof t&&m()-t<10||("undefined"!=typeof o&&(clearTimeout(o),o=null),t=m(),_(),e=m()-t))};["resize","scroll","touchmove"].forEach(function(t){window.addEventListener(t,i)})}();var z={center:"center",left:"right",right:"left"},F={middle:"middle",top:"bottom",bottom:"top"},L={top:0,left:0,middle:"50%",center:"50%",bottom:"100%",right:"100%"},Y=function(t,e){var o=t.left,i=t.top;return"auto"===o&&(o=z[e.left]),"auto"===i&&(i=F[e.top]),{left:o,top:i}},H=function(t){var e=t.left,o=t.top;return"undefined"!=typeof L[t.left]&&(e=L[t.left]),"undefined"!=typeof L[t.top]&&(o=L[t.top]),{left:e,top:o}},X=function(t){var e=t.split(" "),o=M(e,2),i=o[0],n=o[1];return{top:i,left:n}},j=X,N=function(){function t(e){var o=this;i(this,t),this.position=this.position.bind(this),B.push(this),this.history=[],this.setOptions(e,!1),C.modules.forEach(function(t){"undefined"!=typeof t.initialize&&t.initialize.call(o)}),this.position()}return w(t,[{key:"getClass",value:function(){var t=void 0===arguments[0]?"":arguments[0],e=this.options.classes;return"undefined"!=typeof e&&e[t]?this.options.classes[t]:this.options.classPrefix?this.options.classPrefix+"-"+t:t}},{key:"setOptions",value:function(t){var e=this,o=void 0===arguments[1]?!0:arguments[1],i={offset:"0 0",targetOffset:"0 0",targetAttachment:"auto auto",classPrefix:"tether"};this.options=f(i,t);var r=this.options,s=r.element,a=r.target,h=r.targetModifier;if(this.element=s,this.target=a,this.targetModifier=h,"viewport"===this.target?(this.target=document.body,this.targetModifier="visible"):"scroll-handle"===this.target&&(this.target=document.body,this.targetModifier="scroll-handle"),["element","target"].forEach(function(t){if("undefined"==typeof e[t])throw new Error("Tether Error: Both element and target must be defined");"undefined"!=typeof e[t].jquery?e[t]=e[t][0]:"string"==typeof e[t]&&(e[t]=document.querySelector(e[t]))}),l(this.element,this.getClass("element")),this.options.addTargetClasses!==!1&&l(this.target,this.getClass("target")),!this.options.attachment)throw new Error("Tether Error: You must provide an attachment");this.targetAttachment=j(this.options.targetAttachment),this.attachment=j(this.options.attachment),this.offset=X(this.options.offset),this.targetOffset=X(this.options.targetOffset),"undefined"!=typeof this.scrollParent&&this.disable(),this.scrollParent="scroll-handle"===this.targetModifier?this.target:n(this.target),this.options.enabled!==!1&&this.enable(o)}},{key:"getTargetBounds",value:function(){if("undefined"==typeof this.targetModifier)return r(this.target);if("visible"===this.targetModifier){if(this.target===document.body)return{top:pageYOffset,left:pageXOffset,height:innerHeight,width:innerWidth};var t=r(this.target),e={height:t.height,width:t.width,top:t.top,left:t.left};return e.height=Math.min(e.height,t.height-(pageYOffset-t.top)),e.height=Math.min(e.height,t.height-(t.top+t.height-(pageYOffset+innerHeight))),e.height=Math.min(innerHeight,e.height),e.height-=2,e.width=Math.min(e.width,t.width-(pageXOffset-t.left)),e.width=Math.min(e.width,t.width-(t.left+t.width-(pageXOffset+innerWidth))),e.width=Math.min(innerWidth,e.width),e.width-=2,e.top<pageYOffset&&(e.top=pageYOffset),e.left<pageXOffset&&(e.left=pageXOffset),e}if("scroll-handle"===this.targetModifier){var t=void 0,o=this.target;o===document.body?(o=document.documentElement,t={left:pageXOffset,top:pageYOffset,height:innerHeight,width:innerWidth}):t=r(o);var i=getComputedStyle(o),n=o.scrollWidth>o.clientWidth||[i.overflow,i.overflowX].indexOf("scroll")>=0||this.target!==document.body,s=0;n&&(s=15);var a=t.height-parseFloat(i.borderTopWidth)-parseFloat(i.borderBottomWidth)-s,e={width:15,height:.975*a*(a/o.scrollHeight),left:t.left+t.width-parseFloat(i.borderLeftWidth)-15},f=0;408>a&&this.target===document.body&&(f=-11e-5*Math.pow(a,2)-.00727*a+22.58),this.target!==document.body&&(e.height=Math.max(e.height,24));var h=this.target.scrollTop/(o.scrollHeight-a);return e.top=h*(a-e.height-f)+t.top+parseFloat(i.borderTopWidth),this.target===document.body&&(e.height=Math.max(e.height,24)),e}}},{key:"clearCache",value:function(){this._cache={}}},{key:"cache",value:function(t,e){return"undefined"==typeof this._cache&&(this._cache={}),"undefined"==typeof this._cache[t]&&(this._cache[t]=e.call(this)),this._cache[t]}},{key:"enable",value:function(){var t=void 0===arguments[0]?!0:arguments[0];this.options.addTargetClasses!==!1&&l(this.target,this.getClass("enabled")),l(this.element,this.getClass("enabled")),this.enabled=!0,this.scrollParent!==document&&this.scrollParent.addEventListener("scroll",this.position),t&&this.position()}},{key:"disable",value:function(){h(this.target,this.getClass("enabled")),h(this.element,this.getClass("enabled")),this.enabled=!1,"undefined"!=typeof this.scrollParent&&this.scrollParent.removeEventListener("scroll",this.position)}},{key:"destroy",value:function(){var t=this;this.disable(),B.forEach(function(e,o){return e===t?void B.splice(o,1):void 0})}},{key:"updateAttachClasses",value:function(t,e){var o=this;t=t||this.attachment,e=e||this.targetAttachment;var i=["left","top","bottom","right","middle","center"];"undefined"!=typeof this._addAttachClasses&&this._addAttachClasses.length&&this._addAttachClasses.splice(0,this._addAttachClasses.length),"undefined"==typeof this._addAttachClasses&&(this._addAttachClasses=[]);var n=this._addAttachClasses;t.top&&n.push(this.getClass("element-attached")+"-"+t.top),t.left&&n.push(this.getClass("element-attached")+"-"+t.left),e.top&&n.push(this.getClass("target-attached")+"-"+e.top),e.left&&n.push(this.getClass("target-attached")+"-"+e.left);var r=[];i.forEach(function(t){r.push(o.getClass("element-attached")+"-"+t),r.push(o.getClass("target-attached")+"-"+t)}),T(function(){"undefined"!=typeof o._addAttachClasses&&(c(o.element,o._addAttachClasses,r),o.options.addTargetClasses!==!1&&c(o.target,o._addAttachClasses,r),delete o._addAttachClasses)})}},{key:"position",value:function(){var t=this,e=void 0===arguments[0]?!0:arguments[0];if(this.enabled){this.clearCache();var o=Y(this.targetAttachment,this.attachment);this.updateAttachClasses(this.attachment,o);var i=this.cache("element-bounds",function(){return r(t.element)}),n=i.width,f=i.height;if(0===n&&0===f&&"undefined"!=typeof this.lastSize){var h=this.lastSize;n=h.width,f=h.height}else this.lastSize={width:n,height:f};var l=this.cache("target-bounds",function(){return t.getTargetBounds()}),d=l,u=y(H(this.attachment),{width:n,height:f}),p=y(H(o),d),c=y(this.offset,{width:n,height:f}),g=y(this.targetOffset,d);u=v(u,c),p=v(p,g);for(var m=l.left+p.left-u.left,b=l.top+p.top-u.top,w=0;w<C.modules.length;++w){var O=C.modules[w],E=O.position.call(this,{left:m,top:b,targetAttachment:o,targetPos:l,elementPos:i,offset:u,targetOffset:p,manualOffset:c,manualTargetOffset:g,scrollbarSize:A,attachment:this.attachment});if(E===!1)return!1;"undefined"!=typeof E&&"object"==typeof E&&(b=E.top,m=E.left)}var x={page:{top:b,left:m},viewport:{top:b-pageYOffset,bottom:pageYOffset-b-f+innerHeight,left:m-pageXOffset,right:pageXOffset-m-n+innerWidth}},A=void 0;return document.body.scrollWidth>window.innerWidth&&(A=this.cache("scrollbar-size",a),x.viewport.bottom-=A.height),document.body.scrollHeight>window.innerHeight&&(A=this.cache("scrollbar-size",a),x.viewport.right-=A.width),(-1===["","static"].indexOf(document.body.style.position)||-1===["","static"].indexOf(document.body.parentElement.style.position))&&(x.page.bottom=document.body.scrollHeight-b-f,x.page.right=document.body.scrollWidth-m-n),"undefined"!=typeof this.options.optimizations&&this.options.optimizations.moveElement!==!1&&"undefined"==typeof this.targetModifier&&!function(){var e=t.cache("target-offsetparent",function(){return s(t.target)}),o=t.cache("target-offsetparent-bounds",function(){return r(e)}),i=getComputedStyle(e),n=o,a={};if(["Top","Left","Bottom","Right"].forEach(function(t){a[t.toLowerCase()]=parseFloat(i["border"+t+"Width"])}),o.right=document.body.scrollWidth-o.left-n.width+a.right,o.bottom=document.body.scrollHeight-o.top-n.height+a.bottom,x.page.top>=o.top+a.top&&x.page.bottom>=o.bottom&&x.page.left>=o.left+a.left&&x.page.right>=o.right){var f=e.scrollTop,h=e.scrollLeft;x.offset={top:x.page.top-o.top+f-a.top,left:x.page.left-o.left+h-a.left}}}(),this.move(x),this.history.unshift(x),this.history.length>3&&this.history.pop(),e&&S(),!0}}},{key:"move",value:function(t){var e=this;if("undefined"!=typeof this.element.parentNode){var o={};for(var i in t){o[i]={};for(var n in t[i]){for(var r=!1,a=0;a<this.history.length;++a){var h=this.history[a];if("undefined"!=typeof h[i]&&!g(h[i][n],t[i][n])){r=!0;break}}r||(o[i][n]=!0)}}var l={top:"",left:"",right:"",bottom:""},d=function(t,o){var i="undefined"!=typeof e.options.optimizations,n=i?e.options.optimizations.gpu:null;if(n!==!1){var r=void 0,s=void 0;t.top?(l.top=0,r=o.top):(l.bottom=0,r=-o.bottom),t.left?(l.left=0,s=o.left):(l.right=0,s=-o.right),l[k]="translateX("+Math.round(s)+"px) translateY("+Math.round(r)+"px)","msTransform"!==k&&(l[k]+=" translateZ(0)")}else t.top?l.top=o.top+"px":l.bottom=o.bottom+"px",t.left?l.left=o.left+"px":l.right=o.right+"px"},u=!1;(o.page.top||o.page.bottom)&&(o.page.left||o.page.right)?(l.position="absolute",d(o.page,t.page)):(o.viewport.top||o.viewport.bottom)&&(o.viewport.left||o.viewport.right)?(l.position="fixed",d(o.viewport,t.viewport)):"undefined"!=typeof o.offset&&o.offset.top&&o.offset.left?!function(){l.position="absolute";var i=e.cache("target-offsetparent",function(){return s(e.target)});s(e.element)!==i&&T(function(){e.element.parentNode.removeChild(e.element),i.appendChild(e.element)}),d(o.offset,t.offset),u=!0}():(l.position="absolute",d({top:!0,left:!0},t.page)),u||"BODY"===this.element.parentNode.tagName||(this.element.parentNode.removeChild(this.element),document.body.appendChild(this.element));var p={},c=!1;for(var n in l){var m=l[n],v=this.element.style[n];""!==v&&""!==m&&["top","left","bottom","right"].indexOf(n)>=0&&(v=parseFloat(v),m=parseFloat(m)),v!==m&&(c=!0,p[n]=m)}c&&T(function(){f(e.element.style,p)})}}}]),t}();N.modules=[],C.position=_;var R=f(N,C),M=function(){function t(t,e){var o=[],i=!0,n=!1,r=void 0;try{for(var s,a=t[Symbol.iterator]();!(i=(s=a.next()).done)&&(o.push(s.value),!e||o.length!==e);i=!0);}catch(f){n=!0,r=f}finally{try{!i&&a["return"]&&a["return"]()}finally{if(n)throw r}}return o}return function(e,o){if(Array.isArray(e))return e;if(Symbol.iterator in Object(e))return t(e,o);throw new TypeError("Invalid attempt to destructure non-iterable instance")}}(),P=C.Utils,r=P.getBounds,f=P.extend,c=P.updateClasses,T=P.defer,U=["left","top","right","bottom"];C.modules.push({position:function(t){var e=this,o=t.top,i=t.left,n=t.targetAttachment;if(!this.options.constraints)return!0;var s=this.cache("element-bounds",function(){return r(e.element)}),a=s.height,h=s.width;if(0===h&&0===a&&"undefined"!=typeof this.lastSize){var l=this.lastSize;h=l.width,a=l.height}var d=this.cache("target-bounds",function(){return e.getTargetBounds()}),u=d.height,p=d.width,g=[this.getClass("pinned"),this.getClass("out-of-bounds")];this.options.constraints.forEach(function(t){var e=t.outOfBoundsClass,o=t.pinnedClass;e&&g.push(e),o&&g.push(o)}),g.forEach(function(t){["left","top","right","bottom"].forEach(function(e){g.push(t+"-"+e)})});var m=[],v=f({},n),y=f({},this.attachment);return this.options.constraints.forEach(function(t){var r=t.to,s=t.attachment,f=t.pin;"undefined"==typeof s&&(s="");var l=void 0,d=void 0;if(s.indexOf(" ")>=0){var c=s.split(" "),g=M(c,2);d=g[0],l=g[1]}else l=d=s;var w=b(e,r);("target"===d||"both"===d)&&(o<w[1]&&"top"===v.top&&(o+=u,v.top="bottom"),o+a>w[3]&&"bottom"===v.top&&(o-=u,v.top="top")),"together"===d&&(o<w[1]&&"top"===v.top&&("bottom"===y.top?(o+=u,v.top="bottom",o+=a,y.top="top"):"top"===y.top&&(o+=u,v.top="bottom",o-=a,y.top="bottom")),o+a>w[3]&&"bottom"===v.top&&("top"===y.top?(o-=u,v.top="top",o-=a,y.top="bottom"):"bottom"===y.top&&(o-=u,v.top="top",o+=a,y.top="top")),"middle"===v.top&&(o+a>w[3]&&"top"===y.top?(o-=a,y.top="bottom"):o<w[1]&&"bottom"===y.top&&(o+=a,y.top="top"))),("target"===l||"both"===l)&&(i<w[0]&&"left"===v.left&&(i+=p,v.left="right"),i+h>w[2]&&"right"===v.left&&(i-=p,v.left="left")),"together"===l&&(i<w[0]&&"left"===v.left?"right"===y.left?(i+=p,v.left="right",i+=h,y.left="left"):"left"===y.left&&(i+=p,v.left="right",i-=h,y.left="right"):i+h>w[2]&&"right"===v.left?"left"===y.left?(i-=p,v.left="left",i-=h,y.left="right"):"right"===y.left&&(i-=p,v.left="left",i+=h,y.left="left"):"center"===v.left&&(i+h>w[2]&&"left"===y.left?(i-=h,y.left="right"):i<w[0]&&"right"===y.left&&(i+=h,y.left="left"))),("element"===d||"both"===d)&&(o<w[1]&&"bottom"===y.top&&(o+=a,y.top="top"),o+a>w[3]&&"top"===y.top&&(o-=a,y.top="bottom")),("element"===l||"both"===l)&&(i<w[0]&&"right"===y.left&&(i+=h,y.left="left"),i+h>w[2]&&"left"===y.left&&(i-=h,y.left="right")),"string"==typeof f?f=f.split(",").map(function(t){return t.trim()}):f===!0&&(f=["top","left","right","bottom"]),f=f||[];var C=[],O=[];o<w[1]&&(f.indexOf("top")>=0?(o=w[1],C.push("top")):O.push("top")),o+a>w[3]&&(f.indexOf("bottom")>=0?(o=w[3]-a,C.push("bottom")):O.push("bottom")),i<w[0]&&(f.indexOf("left")>=0?(i=w[0],C.push("left")):O.push("left")),i+h>w[2]&&(f.indexOf("right")>=0?(i=w[2]-h,C.push("right")):O.push("right")),C.length&&!function(){var t=void 0;t="undefined"!=typeof e.options.pinnedClass?e.options.pinnedClass:e.getClass("pinned"),m.push(t),C.forEach(function(e){m.push(t+"-"+e)})}(),O.length&&!function(){var t=void 0;t="undefined"!=typeof e.options.outOfBoundsClass?e.options.outOfBoundsClass:e.getClass("out-of-bounds"),m.push(t),O.forEach(function(e){m.push(t+"-"+e)})}(),(C.indexOf("left")>=0||C.indexOf("right")>=0)&&(y.left=v.left=!1),(C.indexOf("top")>=0||C.indexOf("bottom")>=0)&&(y.top=v.top=!1),(v.top!==n.top||v.left!==n.left||y.top!==e.attachment.top||y.left!==e.attachment.left)&&e.updateAttachClasses(y,v)}),T(function(){e.options.addTargetClasses!==!1&&c(e.target,m,g),c(e.element,m,g)}),{top:o,left:i}}});var P=C.Utils,r=P.getBounds,c=P.updateClasses,T=P.defer;C.modules.push({position:function(t){var e=this,o=t.top,i=t.left,n=this.cache("element-bounds",function(){return r(e.element)}),s=n.height,a=n.width,f=this.getTargetBounds(),h=o+s,l=i+a,d=[];o<=f.bottom&&h>=f.top&&["left","right"].forEach(function(t){var e=f[t];(e===i||e===l)&&d.push(t)}),i<=f.right&&l>=f.left&&["top","bottom"].forEach(function(t){var e=f[t];(e===o||e===h)&&d.push(t)});var u=[],p=[],g=["left","top","right","bottom"];return u.push(this.getClass("abutted")),g.forEach(function(t){u.push(e.getClass("abutted")+"-"+t)}),d.length&&p.push(this.getClass("abutted")),d.forEach(function(t){p.push(e.getClass("abutted")+"-"+t)}),T(function(){e.options.addTargetClasses!==!1&&c(e.target,p,u),c(e.element,p,u)}),!0}});var M=function(){function t(t,e){var o=[],i=!0,n=!1,r=void 0;try{for(var s,a=t[Symbol.iterator]();!(i=(s=a.next()).done)&&(o.push(s.value),!e||o.length!==e);i=!0);}catch(f){n=!0,r=f}finally{try{!i&&a["return"]&&a["return"]()}finally{if(n)throw r}}return o}return function(e,o){if(Array.isArray(e))return e;if(Symbol.iterator in Object(e))return t(e,o);throw new TypeError("Invalid attempt to destructure non-iterable instance")}}();return C.modules.push({position:function(t){var e=t.top,o=t.left;if(this.options.shift){var i=this.options.shift;"function"==typeof this.options.shift&&(i=this.options.shift.call(this,{top:e,left:o}));var n=void 0,r=void 0;if("string"==typeof i){i=i.split(" "),i[1]=i[1]||i[0];var s=M(i,2);n=s[0],r=s[1],n=parseFloat(n,10),r=parseFloat(r,10)}else n=i.top,r=i.left;return e+=n,o+=r,{top:e,left:o}}}}),R});
H5P.Tether = Tether;
window.Tether = oldTether;
;var oldDrop = window.Drop;
var oldTether = window.Tether;
Tether = H5P.Tether;
!function(t,e){"function"==typeof define&&define.amd?define(["tether"],e):"object"==typeof exports?module.exports=e(require("tether")):t.Drop=e(t.Tether)}(this,function(t){"use strict";function e(t,e){if(!(t instanceof e))throw new TypeError("Cannot call a class as a function")}function n(t,e){if("function"!=typeof e&&null!==e)throw new TypeError("Super expression must either be null or a function, not "+typeof e);t.prototype=Object.create(e&&e.prototype,{constructor:{value:t,enumerable:!1,writable:!0,configurable:!0}}),e&&(Object.setPrototypeOf?Object.setPrototypeOf(t,e):t.__proto__=e)}function o(t){var e=t.split(" "),n=a(e,2),o=n[0],i=n[1];if(["left","right"].indexOf(o)>=0){var s=[i,o];o=s[0],i=s[1]}return[o,i].join(" ")}function i(t,e){for(var n=void 0,o=[];-1!==(n=t.indexOf(e));)o.push(t.splice(n,1));return o}function s(){var a=arguments.length<=0||void 0===arguments[0]?{}:arguments[0],u=function(){for(var t=arguments.length,e=Array(t),n=0;t>n;n++)e[n]=arguments[n];return new(r.apply(b,[null].concat(e)))};p(u,{createContext:s,drops:[],defaults:{}});var g={classPrefix:"drop",defaults:{position:"bottom left",openOn:"click",beforeClose:null,constrainToScrollParent:!0,constrainToWindow:!0,classes:"",remove:!1,tetherOptions:{}}};p(u,g,a),p(u.defaults,g.defaults,a.defaults),"undefined"==typeof x[u.classPrefix]&&(x[u.classPrefix]=[]),u.updateBodyClasses=function(){for(var t=!1,e=x[u.classPrefix],n=e.length,o=0;n>o;++o)if(e[o].isOpened()){t=!0;break}t?d(document.body,u.classPrefix+"-open"):c(document.body,u.classPrefix+"-open")};var b=function(s){function r(t){if(e(this,r),l(Object.getPrototypeOf(r.prototype),"constructor",this).call(this),this.options=p({},u.defaults,t),this.target=this.options.target,"undefined"==typeof this.target)throw new Error("Drop Error: You must provide a target.");var n="data-"+u.classPrefix,o=this.target.getAttribute(n);o&&(this.options.content=o);for(var i=["position","openOn"],s=0;s<i.length;++s){var a=this.target.getAttribute(n+"-"+i[s]);a&&(this.options[i[s]]=a)}this.options.classes&&this.options.addTargetClasses!==!1&&d(this.target,this.options.classes),u.drops.push(this),x[u.classPrefix].push(this),this._boundEvents=[],this.bindMethods(),this.setupElements(),this.setupEvents(),this.setupTether()}return n(r,s),h(r,[{key:"_on",value:function(t,e,n){this._boundEvents.push({element:t,event:e,handler:n}),t.addEventListener(e,n)}},{key:"bindMethods",value:function(){this.transitionEndHandler=this._transitionEndHandler.bind(this)}},{key:"setupElements",value:function(){var t=this;if(this.drop=document.createElement("div"),d(this.drop,u.classPrefix),this.options.classes&&d(this.drop,this.options.classes),this.content=document.createElement("div"),d(this.content,u.classPrefix+"-content"),"function"==typeof this.options.content){var e=function(){var e=t.options.content.call(t,t);if("string"==typeof e)t.content.innerHTML=e;else{if("object"!=typeof e)throw new Error("Drop Error: Content function should return a string or HTMLElement.");t.content.innerHTML="",t.content.appendChild(e)}};e(),this.on("open",e.bind(this))}else"object"==typeof this.options.content?this.content.appendChild(this.options.content):this.content.innerHTML=this.options.content;this.drop.appendChild(this.content)}},{key:"setupTether",value:function(){var e=this.options.position.split(" ");e[0]=E[e[0]],e=e.join(" ");var n=[];this.options.constrainToScrollParent?n.push({to:"scrollParent",pin:"top, bottom",attachment:"together none"}):n.push({to:"scrollParent"}),this.options.constrainToWindow!==!1?n.push({to:"window",attachment:"together"}):n.push({to:"window"});var i={element:this.drop,target:this.target,attachment:o(e),targetAttachment:o(this.options.position),classPrefix:u.classPrefix,offset:"0 0",targetOffset:"0 0",enabled:!1,constraints:n,addTargetClasses:this.options.addTargetClasses};this.options.tetherOptions!==!1&&(this.tether=new t(p({},i,this.options.tetherOptions)))}},{key:"setupEvents",value:function(){var t=this;if(this.options.openOn){if("always"===this.options.openOn)return void setTimeout(this.open.bind(this));var e=this.options.openOn.split(" ");if(e.indexOf("click")>=0)for(var n=function(e){t.toggle(e),e.preventDefault()},o=function(e){t.isOpened()&&(e.target===t.drop||t.drop.contains(e.target)||e.target===t.target||t.target.contains(e.target)||t.close(e))},i=0;i<y.length;++i){var s=y[i];this._on(this.target,s,n),this._on(document,s,o)}var r=!1,a=null,h=function(e){r=!0,t.open(e)},l=function(e){r=!1,"undefined"!=typeof a&&clearTimeout(a),a=setTimeout(function(){r||t.close(e),a=null},50)};e.indexOf("hover")>=0&&(this._on(this.target,"mouseover",h),this._on(this.drop,"mouseover",h),this._on(this.target,"mouseout",l),this._on(this.drop,"mouseout",l)),e.indexOf("focus")>=0&&(this._on(this.target,"focus",h),this._on(this.drop,"focus",h),this._on(this.target,"blur",l),this._on(this.drop,"blur",l))}}},{key:"isOpened",value:function(){return this.drop?f(this.drop,u.classPrefix+"-open"):void 0}},{key:"toggle",value:function(t){this.isOpened()?this.close(t):this.open(t)}},{key:"open",value:function(t){var e=this;this.isOpened()||(this.drop.parentNode||document.body.appendChild(this.drop),"undefined"!=typeof this.tether&&this.tether.enable(),d(this.drop,u.classPrefix+"-open"),d(this.drop,u.classPrefix+"-open-transitionend"),setTimeout(function(){e.drop&&d(e.drop,u.classPrefix+"-after-open")}),"undefined"!=typeof this.tether&&this.tether.position(),this.trigger("open"),u.updateBodyClasses())}},{key:"_transitionEndHandler",value:function(t){t.target===t.currentTarget&&(f(this.drop,u.classPrefix+"-open")||c(this.drop,u.classPrefix+"-open-transitionend"),this.drop.removeEventListener(m,this.transitionEndHandler))}},{key:"beforeCloseHandler",value:function(t){var e=!0;return this.isClosing||"function"!=typeof this.options.beforeClose||(this.isClosing=!0,e=this.options.beforeClose(t,this)!==!1),this.isClosing=!1,e}},{key:"close",value:function(t){this.isOpened()&&this.beforeCloseHandler(t)&&(c(this.drop,u.classPrefix+"-open"),c(this.drop,u.classPrefix+"-after-open"),this.drop.addEventListener(m,this.transitionEndHandler),this.trigger("close"),"undefined"!=typeof this.tether&&this.tether.disable(),u.updateBodyClasses(),this.options.remove&&this.remove(t))}},{key:"remove",value:function(t){this.close(t),this.drop.parentNode&&this.drop.parentNode.removeChild(this.drop)}},{key:"position",value:function(){this.isOpened()&&"undefined"!=typeof this.tether&&this.tether.position()}},{key:"destroy",value:function(){this.remove(),"undefined"!=typeof this.tether&&this.tether.destroy();for(var t=0;t<this._boundEvents.length;++t){var e=this._boundEvents[t],n=e.element,o=e.event,s=e.handler;n.removeEventListener(o,s)}this._boundEvents=[],this.tether=null,this.drop=null,this.content=null,this.target=null,i(x[u.classPrefix],this),i(u.drops,this)}}]),r}(v);return u}var r=Function.prototype.bind,a=function(){function t(t,e){var n=[],o=!0,i=!1,s=void 0;try{for(var r,a=t[Symbol.iterator]();!(o=(r=a.next()).done)&&(n.push(r.value),!e||n.length!==e);o=!0);}catch(h){i=!0,s=h}finally{try{!o&&a["return"]&&a["return"]()}finally{if(i)throw s}}return n}return function(e,n){if(Array.isArray(e))return e;if(Symbol.iterator in Object(e))return t(e,n);throw new TypeError("Invalid attempt to destructure non-iterable instance")}}(),h=function(){function t(t,e){for(var n=0;n<e.length;n++){var o=e[n];o.enumerable=o.enumerable||!1,o.configurable=!0,"value"in o&&(o.writable=!0),Object.defineProperty(t,o.key,o)}}return function(e,n,o){return n&&t(e.prototype,n),o&&t(e,o),e}}(),l=function(t,e,n){for(var o=!0;o;){var i=t,s=e,r=n;a=l=h=void 0,o=!1,null===i&&(i=Function.prototype);var a=Object.getOwnPropertyDescriptor(i,s);if(void 0!==a){if("value"in a)return a.value;var h=a.get;return void 0===h?void 0:h.call(r)}var l=Object.getPrototypeOf(i);if(null===l)return void 0;t=l,e=s,n=r,o=!0}},u=t.Utils,p=u.extend,d=u.addClass,c=u.removeClass,f=u.hasClass,v=u.Evented,y=["click"];"ontouchstart"in document.documentElement&&y.push("touchstart");var g={WebkitTransition:"webkitTransitionEnd",MozTransition:"transitionend",OTransition:"otransitionend",transition:"transitionend"},m="";for(var b in g)if({}.hasOwnProperty.call(g,b)){var O=document.createElement("p");"undefined"!=typeof O.style[b]&&(m=g[b])}var E={left:"right",right:"left",top:"bottom",bottom:"top",middle:"middle",center:"center"},x={},P=s();return document.addEventListener("DOMContentLoaded",function(){P.updateBodyClasses()}),P});
H5P.Drop = Drop;
window.Drop = oldDrop;
window.Tether = oldTether;
;var H5P = H5P || {};
/**
 * Transition contains helper function relevant for transitioning
 */
H5P.Transition = (function ($) {

  /**
   * @class
   * @namespace H5P
   */
  Transition = {};

  /**
   * @private
   */
  Transition.transitionEndEventNames = {
    'WebkitTransition': 'webkitTransitionEnd',
    'transition':       'transitionend',
    'MozTransition':    'transitionend',
    'OTransition':      'oTransitionEnd',
    'msTransition':     'MSTransitionEnd'
  };

  /**
   * @private
   */
  Transition.cache = [];

  /**
   * Get the vendor property name for an event
   *
   * @function H5P.Transition.getVendorPropertyName
   * @static
   * @private
   * @param  {string} prop Generic property name
   * @return {string}      Vendor specific property name
   */
  Transition.getVendorPropertyName = function (prop) {

    if (Transition.cache[prop] !== undefined) {
      return Transition.cache[prop];
    }

    var div = document.createElement('div');

    // Handle unprefixed versions (FF16+, for example)
    if (prop in div.style) {
      Transition.cache[prop] = prop;
    }
    else {
      var prefixes = ['Moz', 'Webkit', 'O', 'ms'];
      var prop_ = prop.charAt(0).toUpperCase() + prop.substr(1);

      if (prop in div.style) {
        Transition.cache[prop] = prop;
      }
      else {
        for (var i = 0; i < prefixes.length; ++i) {
          var vendorProp = prefixes[i] + prop_;
          if (vendorProp in div.style) {
            Transition.cache[prop] = vendorProp;
            break;
          }
        }
      }
    }

    return Transition.cache[prop];
  };

  /**
   * Get the name of the transition end event
   *
   * @static
   * @private
   * @return {string}  description
   */
  Transition.getTransitionEndEventName = function () {
    return Transition.transitionEndEventNames[Transition.getVendorPropertyName('transition')] || undefined;
  };

  /**
   * Helper function for listening on transition end events
   *
   * @function H5P.Transition.onTransitionEnd
   * @static
   * @param  {domElement} $element The element which is transitioned
   * @param  {function} callback The callback to be invoked when transition is finished
   * @param  {number} timeout  Timeout in milliseconds. Fallback if transition event is never fired
   */
  Transition.onTransitionEnd = function ($element, callback, timeout) {
    // Fallback on 1 second if transition event is not supported/triggered
    timeout = timeout || 1000;
    Transition.transitionEndEventName = Transition.transitionEndEventName || Transition.getTransitionEndEventName();
    var callbackCalled = false;

    var doCallback = function () {
      if (callbackCalled) {
        return;
      }
      $element.off(Transition.transitionEndEventName, callback);
      callbackCalled = true;
      clearTimeout(timer);
      callback();
    };

    var timer = setTimeout(function () {
      doCallback();
    }, timeout);

    $element.on(Transition.transitionEndEventName, function () {
      doCallback();
    });
  };

  /**
   * Wait for a transition - when finished, invokes next in line
   *
   * @private
   *
   * @param {Object[]}    transitions             Array of transitions
   * @param {H5P.jQuery}  transitions[].$element  Dom element transition is performed on
   * @param {number=}     transitions[].timeout   Timeout fallback if transition end never is triggered
   * @param {bool=}       transitions[].break     If true, sequence breaks after this transition
   * @param {number}      index                   The index for current transition
   */
  var runSequence = function (transitions, index) {
    if (index >= transitions.length) {
      return;
    }

    var transition = transitions[index];
    H5P.Transition.onTransitionEnd(transition.$element, function () {
      if (transition.end) {
        transition.end();
      }
      if (transition.break !== true) {
        runSequence(transitions, index+1);
      }
    }, transition.timeout || undefined);
  };

  /**
   * Run a sequence of transitions
   *
   * @function H5P.Transition.sequence
   * @static
   * @param {Object[]}    transitions             Array of transitions
   * @param {H5P.jQuery}  transitions[].$element  Dom element transition is performed on
   * @param {number=}     transitions[].timeout   Timeout fallback if transition end never is triggered
   * @param {bool=}       transitions[].break     If true, sequence breaks after this transition
   */
  Transition.sequence = function (transitions) {
    runSequence(transitions, 0);
  };

  return Transition;
})(H5P.jQuery);
;var H5P = H5P || {};

/**
 * Class responsible for creating a help text dialog
 */
H5P.JoubelHelpTextDialog = (function ($) {

  /**
   * Display a pop-up containing a message.
   *
   * @param {H5P.jQuery}  $container  The container which message dialog will be appended to
   * @param {string}      message     The message
   * @return {H5P.jQuery}
   */
  function JoubelHelpTextDialog(header, message) {
    var $helpTextDialogBox = $('<div>', {
      'class': 'joubel-help-text-dialog-box'
    });

    var $helpTextDialogBackground = $('<div>', {
      'class': 'joubel-help-text-dialog-background'
    }).appendTo($helpTextDialogBox);

    var $helpTextDialogContainer = $('<div>', {
      'class': 'joubel-help-text-dialog-container'
    }).appendTo($helpTextDialogBox);

    $('<div>', {
      'class': 'joubel-help-text-header',
      'html': header
    }).appendTo($helpTextDialogContainer);

    $('<div>', {
      'class': 'joubel-help-text-body',
      'html': message
    }).appendTo($helpTextDialogContainer);

    $('<div>', {
      'class': 'joubel-help-text-remove',
      'tabindex': 0
    }).click(function () {
      $helpTextDialogBox.remove();
    }).keydown(function (e) {
      var keyPressed = e.which;
      // 32 - space
      if (keyPressed === 32) {
        $(this).click();
        e.preventDefault();
      }
    }).appendTo($helpTextDialogContainer);

    return $helpTextDialogBox;
  }

  return JoubelHelpTextDialog;
}(H5P.jQuery));
;var H5P = H5P || {};

/**
 * Class responsible for creating auto-disappearing dialogs
 */
H5P.JoubelMessageDialog = (function ($) {

  /**
   * Display a pop-up containing a message.
   *
   * @param {H5P.jQuery} $container The container which message dialog will be appended to
   * @param {string} message The message
   * @return {H5P.jQuery}
   */
  function JoubelMessageDialog ($container, message) {
    var timeout;

    var removeDialog = function () {
      $warning.remove();
      clearTimeout(timeout);
      $container.off('click.messageDialog');
    };

    // Create warning popup:
    var $warning = $('<div/>', {
      'class': 'joubel-message-dialog',
      text: message
    }).appendTo($container);

    // Remove after 3 seconds or if user clicks anywhere in $container:
    timeout = setTimeout(removeDialog, 3000);
    $container.on('click.messageDialog', removeDialog);

    return $warning;
  }

  return JoubelMessageDialog;
})(H5P.jQuery);
;var H5P = H5P || {};

/**
 * Class responsible for creating a circular progress bar
 */

H5P.JoubelProgressCircle = (function ($) {

  /**
   * Constructor for the Progress Circle
   *
   * @param {Number} number The amount of progress to display
   * @param {string} progressColor Color for the progress meter
   * @param {string} backgroundColor Color behind the progress meter
   */
  function ProgressCircle(number, progressColor, fillColor, backgroundColor) {
    progressColor = progressColor || '#096bcb';
    fillColor = fillColor || '#f0f0f0';
    backgroundColor = backgroundColor || '#ffffff';
    var progressColorRGB = this.hexToRgb(progressColor);

    //Verify number
    try {
      number = Number(number);
      if (number === '') {
        throw 'is empty';
      }
      if (isNaN(number)) {
        throw 'is not a number';
      }
    } catch (e) {
      console.log('Progress circle input' + e);
      number = 'err';
    }

    //Draw circle
    if (number > 100) {
      number = 100;
    }

    var decimalNumber = number / 100;

    // We can not use rgba, since they will stack on top of each other.
    // Instead we create the equivalent of the rgba color
    // and applies this to the activeborder and background color.
    var progressColorString = 'rgb(' + parseInt(this.rgbFromAlpha(progressColorRGB.r, decimalNumber), 10) +
      ',' + parseInt(this.rgbFromAlpha(progressColorRGB.g, decimalNumber), 10) +
      ',' + parseInt(this.rgbFromAlpha(progressColorRGB.b, decimalNumber), 10) + ')';

    // Circle wrapper
    var $wrapper = $('<div/>', {
      'class': "joubel-progress-circle-wrapper"
    });

    //Active border indicates progress
    var $activeBorder = $('<div/>', {
      'class': "joubel-progress-circle-active-border"
    }).appendTo($wrapper);

    //Background circle
    var $backgroundCircle = $('<div/>', {
      'class': "joubel-progress-circle-circle"
    }).appendTo($activeBorder);

    //Progress text/number
    $('<span/>', {
      'text': number,
      'class': "joubel-progress-circle-percentage"
    }).appendTo($backgroundCircle);

    var deg = number * 3.6;
    if (deg <= 180) {
      $activeBorder.css('background-image',
        'linear-gradient(' + (90 + deg) + 'deg, transparent 50%, ' + fillColor + ' 50%),' +
        'linear-gradient(90deg, ' + fillColor + ' 50%, transparent 50%)')
        .css('border', '2px solid' + backgroundColor)
        .css('background-color', progressColorString);
    } else {
      $activeBorder.css('background-image',
        'linear-gradient(' + (deg - 90) + 'deg, transparent 50%, ' + progressColorString + ' 50%),' +
        'linear-gradient(90deg, ' + fillColor + ' 50%, transparent 50%)')
        .css('border', '2px solid' + backgroundColor)
        .css('background-color', progressColorString);
    }

    this.$activeBorder = $activeBorder;
    this.$backgroundCircle = $backgroundCircle;
    this.$wrapper = $wrapper;

    this.initResizeFunctionality();

    return $wrapper;
  }

  /**
   * Initializes resize functionality for the progress circle
   */
  ProgressCircle.prototype.initResizeFunctionality = function () {
    var self = this;

    $(window).resize(function () {
      // Queue resize
      setTimeout(function () {
        self.resize();
      });
    });

    // First resize
    setTimeout(function () {
      self.resize();
    }, 0);
  };

  /**
   * Resize function makes progress circle grow or shrink relative to parent container
   */
  ProgressCircle.prototype.resize = function () {
    var $parent = this.$wrapper.parent();

    if ($parent !== undefined && $parent) {

      // Measurements
      var fontSize = parseInt($parent.css('font-size'), 10);

      // Static sizes
      var fontSizeMultiplum = 3.75;
      var progressCircleWidthPx = parseInt((fontSize / 4.5), 10) % 2 === 0 ? parseInt((fontSize / 4.5), 10) : parseInt((fontSize / 4.5), 10) + 1;
      var progressCircleOffset = progressCircleWidthPx / 2;

      var width = fontSize * fontSizeMultiplum;
      var height = fontSize * fontSizeMultiplum;
      this.$activeBorder.css({
        'width': width,
        'height': height
      });

      this.$backgroundCircle.css({
        'width': width - progressCircleWidthPx,
        'height': height - progressCircleWidthPx,
        'top': progressCircleOffset,
        'left': progressCircleOffset
      });
    }
  };

  /**
   * Hex to RGB conversion
   * @param hex
   * @returns {{r: Number, g: Number, b: Number}}
   */
  ProgressCircle.prototype.hexToRgb = function (hex) {
    var result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
    return result ? {
      r: parseInt(result[1], 16),
      g: parseInt(result[2], 16),
      b: parseInt(result[3], 16)
    } : null;
  };

  /**
   * Convert rgb and opacity to new rgb
   * @param {number} colorValue
   * @param {float} opacity
   * @returns {number} blended colorValue
   */
  ProgressCircle.prototype.rgbFromAlpha = function (colorValue, opacity) {
    return (opacity * colorValue) + (1 - opacity) * 255;
  };

  return ProgressCircle;

}(H5P.jQuery));
;var H5P = H5P || {};

H5P.SimpleRoundedButton = (function ($) {

  /**
   * Creates a new tip
   */
  function SimpleRoundedButton(text) {

    var $simpleRoundedButton = $('<div>', {
      'class': 'joubel-simple-rounded-button',
      'title': text,
      'role': 'button',
      'tabindex': '0'
    }).keydown(function (e) {
      var keyPressed = e.which;
      // 32 - space
      if (keyPressed === 32) {
        $(this).click();
        e.preventDefault();
      }
      $(this).focus();
    });

    $('<span>', {
      'class': 'joubel-simple-rounded-button-text',
      'html': text
    }).appendTo($simpleRoundedButton);

    return $simpleRoundedButton;
  }

  return SimpleRoundedButton;
}(H5P.jQuery));
;var H5P = H5P || {};

/**
 * Class responsible for creating speech bubbles
 */
H5P.JoubelSpeechBubble = (function ($) {

  var $currentSpeechBubble;
  var $currentContainer;

  var DEFAULT_MAX_WIDTH = 400;

  var iDevice = navigator.userAgent.match(/iPod|iPhone|iPad/g) ? true : false;

  /**
   * Creates a new speech bubble
   *
   * @param {H5P.jQuery} $container The speaking object
   * @param {string} text The text to display
   * @param {number} maxWidth The maximum width of the bubble
   * @return {H5P.JoubelSpeechBubble}
   */
  function JoubelSpeechBubble($container, text, maxWidth) {
    maxWidth = maxWidth || DEFAULT_MAX_WIDTH;
    $currentContainer = $container;

    this.isHidden = function () {
      return ($currentSpeechBubble === undefined);
    };

    this.remove = function () {
      remove();
    };

    if ($currentSpeechBubble !== undefined) {
      remove();
    }

    var $h5pContainer = $container.closest('.h5p-frame');

    // Check closest h5p frame first, then check for container in case there is no frame.
    if (!$h5pContainer.length) {
      $h5pContainer = $container.closest('.h5p-container');
    }

    // Create bubble
    $currentSpeechBubble = $('<div class="joubel-speech-bubble"><div class="joubel-speech-bubble-inner"><div class="joubel-speech-bubble-text">' + text + '</div></div></div>').appendTo($h5pContainer);

    // Show speech bubble with transition
    setTimeout(function () {
      $currentSpeechBubble.addClass('show');
    }, 0);

    // Setting width to 90% of parent
    var width = $h5pContainer.width()*0.9;

    // If width is more than max width, use max width
    width = width > maxWidth ? maxWidth : width;
    var left = $container.offset().left - width + $container.outerWidth() - $h5pContainer.offset().left - ($container.width()/2) + 20;

    // If width makes element go outside of body, make it smaller.
    // TODO - This is not ideal, e.g if the $container is far to the left.
    // Improvement: support left- and right-"aligned" bubbles
    if (left < 0) {
      // 3px is hard coded here just to get some margin
      // to the left side
      width += left-3;
      left = 3;
    }

    // Need to set font-size, since element is appended to body.
    // Using same font-size as parent. In that way it will grow accordingly
    // when resizing
    var fontSize = 16;//parseFloat($parent.css('font-size'));

    // Set max-width:
    $currentSpeechBubble.css({
      width: width + 'px',
      top: ($container.offset().top + $container.outerHeight() - $h5pContainer.offset().top) + 'px',
      left: left + 'px',
      fontSize: fontSize + 'px'
    });

    // Handle click to close
    H5P.$body.on('click.speechBubble', remove);

    // Handle clicks when inside IV which blocks bubbling.
    $container.parents('.h5p-dialog')
      .on('click.speechBubble', remove);

    if (iDevice) {
      H5P.$body.css('cursor', 'pointer');
    }

    return this;
  }

  // Remove speechbubble if it belongs to a dom element that is about to be hidden
  H5P.externalDispatcher.on('domHidden', function (event) {
    if ($currentSpeechBubble !== undefined && event.data.$dom.find($currentContainer).length !== 0) {
      remove();
    }
  });

  /**
   * Static function for removing the speechbubble
   */
  var remove = function() {
    H5P.$body.off('click.speechBubble');
    $currentContainer.parents('.h5p-dialog').off('click.speechBubble');
    if (iDevice) {
      H5P.$body.css('cursor', '');
    }
    if ($currentSpeechBubble !== undefined) {
      // Apply transition, then remove speech bubble
      $currentSpeechBubble.removeClass('show');
      setTimeout(function () {
        $currentSpeechBubble.remove();
        $currentSpeechBubble = undefined;
      }, 500);
    }
    // Don't return false here. If the user e.g. clicks a button when the bubble is visible,
    // we want the bubble to disapear AND the button to receive the event
  };


  return JoubelSpeechBubble;
})(H5P.jQuery);
;var H5P = H5P || {};

H5P.JoubelThrobber = (function ($) {

  /**
   * Creates a new tip
   */
  function JoubelThrobber() {

    // h5p-throbber css is described in core
    var $throbber = $('<div/>', {
      'class': 'h5p-throbber'
    });

    return $throbber;
  }

  return JoubelThrobber;
}(H5P.jQuery));
;var H5P = H5P || {};

H5P.JoubelTip = (function ($) {

  /**
   * Creates a new tip
   *
   * @param {string} text The text to display in the popup
   * @param {object} params Additional parameters
   */
  function JoubelTip(text, params) {
    var speechBubble;

    params = $.extend({
      showSpeechBubble: true
    }, params);

    var parsedTitle = text;
    if ($.parseHTML($.trim(text)).length) {
      parsedTitle = $.parseHTML($.trim(text))[0].textContent;
    }

    var $tip = $('<div/>', {
      'class': 'joubel-tip-container' + (params.showSpeechBubble ? '' : ' be-quiet'),
      title: parsedTitle,
      click: function () {

        if (speechBubble !== undefined && !speechBubble.isHidden()) {
          speechBubble.remove();
          speechBubble = undefined;
        }
        else if (params.showSpeechBubble) {
          speechBubble = H5P.JoubelSpeechBubble($tip, text);
        }
        return false;
      }
    }).append($('<div/>', {
      'class': 'joubel-tip-icon'
    }));
    return $tip;
  }

  return JoubelTip;
})(H5P.jQuery);
;var H5P = H5P || {};

H5P.JoubelSlider = (function ($) {

  /**
   * Creates a new Slider
   *
   * @param {object} params Additional parameters
   */
  function JoubelSlider(params) {
    H5P.EventDispatcher.call(this);
    var self = this;

    this.$slider = $('<div>', {
      'class': 'h5p-joubel-ui-slider'
    });
    this.$slides = [];
    this.currentIndex = 0;
    this.numSlides = 0;
  }
  JoubelSlider.prototype = Object.create(H5P.EventDispatcher.prototype);
  JoubelSlider.prototype.constructor = JoubelSlider;

  JoubelSlider.prototype.addSlide = function ($content) {
    $content.addClass('h5p-joubel-ui-slide').css({
      'left': (this.numSlides*100) + '%'
    });
    this.$slider.append($content);
    this.$slides.push($content);

    this.numSlides++;

    if(this.numSlides === 1) {
      $content.addClass('current');
    }
  };

  JoubelSlider.prototype.attach = function ($container) {
    $container.append(this.$slider);
  };

  JoubelSlider.prototype.move = function (index) {
    var self = this;

    if(index === 0) {
      self.trigger('first-slide');
    }
    if(index+1 === self.numSlides) {
      self.trigger('last-slide');
    }
    self.trigger('move');

    var $previousSlide = self.$slides[this.currentIndex];
    H5P.Transition.onTransitionEnd(this.$slider, function () {
      $previousSlide.removeClass('current');
      self.trigger('moved');
    });
    this.$slides[index].addClass('current');

    var translateX = 'translateX(' + (-index*100) + '%)';
    this.$slider.css({
      '-webkit-transform': translateX,
      '-moz-transform': translateX,
      '-ms-transform': translateX,
      'transform': translateX
    });

    this.currentIndex = index;
  };

  JoubelSlider.prototype.remove = function () {
    this.$slider.remove();
  };

  JoubelSlider.prototype.next = function () {
    if(this.currentIndex+1 >= this.numSlides) {
      return;
    }

    this.move(this.currentIndex+1);
  };

  JoubelSlider.prototype.previous = function () {
    this.move(this.currentIndex-1);
  };

  JoubelSlider.prototype.first = function () {
    this.move(0);
  };

  JoubelSlider.prototype.last = function () {
    this.move(this.numSlides-1);
  };

  return JoubelSlider;
})(H5P.jQuery);
;var H5P = H5P || {};

/**
 * @module
 */
H5P.JoubelScoreBar = (function ($) {

  /**
   * Creates a score bar
   * @class H5P.JoubelScoreBar
   * @param {number=} maxScore  Maximum score
   * @param {string} [label] Makes it easier for readspeakers to identify the scorebar
   */
  function JoubelScoreBar(maxScore, label) {
    var self = this;

    self.maxScore = maxScore;
    self.score = 0;

    /**
     * @method hasFullScore
     * @private
     * @return {Boolean} true if full score, else false
     */
    var hasFullScore = function () {
      return self.score === self.maxScore;
    };

    /**
     * @function appendTo
     * @memberOf H5P.JoubelScoreBar#
     * @param {H5P.jQuery}  $wrapper  Dom container
     */
    self.appendTo = function ($wrapper) {
      self.$scoreBar.appendTo($wrapper);
    };

    /**
     * Creates the html for this widget
     *
     * @method createHtml
     * @private
     */
    var createHtml = function () {
      // Container div
      self.$scoreBar = $('<div>', {
        'class': 'h5p-joubelui-score-bar',
        'role': 'progressbar',
        'aria-valuenow': 0,
        'aria-valuemin': 0,
        'aria-valuemax': self.maxScore
      });
      if (label) {
        self.$scoreBar.attr('aria-label', label + '.');
      }

      // The progress bar wrapper
      self.$progressWrapper = $('<div>', {
        'class': 'h5p-joubelui-score-bar-progress-wrapper'
      }).appendTo(self.$scoreBar);

      self.$progress = $('<div>', {
        'class': 'h5p-joubelui-score-bar-progress'
      }).appendTo(self.$progressWrapper);

      // The star
      self.$endWrapper = $('<div>', {
        'class': 'h5p-joubelui-score-bar-end'
      }).appendTo(self.$scoreBar);

      // The default star
      self.$defaultStar = $('<span>', {
        'class': 'h5p-joubelui-score-bar-default-star'
      }).appendTo(self.$endWrapper);

      // The full score star
      self.$fullScoreStar = $('<span>', {
        'class': 'h5p-joubelui-score-bar-full-score-star'
      }).appendTo(self.$endWrapper);
    };

    /**
     * Set the current score
     * @method setScore
     * @memberOf H5P.JoubelScoreBar#
     * @param  {number} score
     */
    self.setScore = function (score) {
      // Do nothing if score hasn't changed
      if (score === self.score) {
        return;
      }
      self.score = score > self.maxScore ? self.maxScore : score;
      self.updateVisuals();
    };

    /**
     * Increment score
     * @method incrementScore
     * @memberOf H5P.JoubelScoreBar#
     * @param  {number=}        incrementBy Optional parameter, defaults to 1
     */
    self.incrementScore = function (incrementBy) {
      self.setScore(self.score + (incrementBy || 1));
    };

    /**
     * Set the max score
     * @method setMaxScore
     * @memberOf H5P.JoubelScoreBar#
     * @param  {number}    maxScore The max score
     */
    self.setMaxScore = function (maxScore) {
      self.maxScore = maxScore;
    };

    /**
     * Updates the progressbar visuals
     * @memberOf H5P.JoubelScoreBar#
     * @method updateVisuals
     */
    self.updateVisuals = function () {
      var fullscore = hasFullScore();
      self.$scoreBar.attr('aria-valuenow', self.score);

      setTimeout(function () {
        self.$progress.addClass('animate');
        self.$progress.css({
          width: (fullscore ? '102' : (self.maxScore - 1 !== 0 ? (self.score * 100 / (self.maxScore - 1)) : 0)) + '%'
        });
        H5P.Transition.sequence([
          {
            $element: self.$progress,
            timeout: 600,
            end: function () {
              self.$progress.removeClass('animate');
              self.$scoreBar.toggleClass('full-score', fullscore);
              if (fullscore) {
                self.$fullScoreStar.addClass('animate-background');
              }
            },
            break: !fullscore
          },
          {
            $element: self.$fullScoreStar,
            timeout: 400,
            end: function () {
              self.$fullScoreStar.addClass('animate-star show-star');
            }
          },
          {
            $element: self.$fullScoreStar,
            end: function () {
              self.$fullScoreStar.removeClass('animate-star');
              self.$fullScoreStar.addClass('animate-star-blink');
            }
          }
        ]);
      }, 300);
    };

    /**
     * Removes all classes
     * @method reset
     */
    self.reset = function () {
      self.$fullScoreStar.removeClass('animate-star animate-star-blink show-star animate-background');
      self.$scoreBar.removeClass('full-score');
    };

    createHtml();
  }

  return JoubelScoreBar;
})(H5P.jQuery);
;var H5P = H5P || {};

H5P.JoubelProgressbar = (function ($) {

  /**
   * Joubel progressbar class
   * @method JoubelProgressbar
   * @constructor
   * @param  {number}          steps Number of steps
   */
  function JoubelProgressbar(steps) {
    var self = this;
    this.currentStep = 0;
    this.steps = steps;

    this.$progressbar = $('<div>', {
      'class': 'h5p-joubelui-progressbar',
      on: {
        click: function () {
          self.toggleTooltip();
          return false;
        },
        mouseenter: function () {
          self.showTooltip();
        },
        mouseleave: function () {
          setTimeout(function () {
            self.hideTooltip();
          }, 1500)
        }
      }
    });
    this.$background = $('<div>', {
      'class': 'h5p-joubelui-progressbar-background'
    }).appendTo(this.$progressbar);

    $('body').click(function () {
      self.toggleTooltip(true);
    });
  }

  /**
   * Display tooltip
   * @method showTooltip
   */
  JoubelProgressbar.prototype.showTooltip = function () {
    var self = this;

    if (this.currentStep === 0 || this.tooltip !== undefined) {
      return;
    }

    var parentWidth = self.$progressbar.width();

    this.tooltip = new H5P.Drop({
      target: this.$background.get(0),
      content: this.currentStep + '/' + this.steps,
      classes: 'drop-theme-arrows-bounce h5p-joubelui-drop',
      position: 'top right',
      openOn: 'always',
      tetherOptions: {
        attachment: 'bottom center',
        targetAttachment: 'top right'
      }
    });
    this.tooltip.on('open', function () {
      var $drop = $(self.tooltip.drop);
      var left = $drop.position().left;
      var dropWidth = $drop.width();

      // Need to handle drops getting outside of the progressbar:
      if (left < 0) {
        $drop.css({marginLeft: (-left) + 'px'});
      }
      else if (left + dropWidth > parentWidth) {
        $drop.css({marginLeft: (parentWidth - (left + dropWidth)) + 'px'});
      }
    });
  }

  /**
   * Hides tooltip
   * @method hideTooltip
   */
  JoubelProgressbar.prototype.hideTooltip = function () {
    if (this.tooltip !== undefined) {
      this.tooltip.remove();
      this.tooltip.destroy();
      this.tooltip = undefined;
    }
  }

  /**
   * Toggles tooltip-visibility
   * @method toggleTooltip
   * @param  {boolean}      closeOnly Don't show, only close if open
   */
  JoubelProgressbar.prototype.toggleTooltip = function (closeOnly) {
    if (this.tooltip === undefined && !closeOnly) {
      this.showTooltip();
    }
    else if (this.tooltip !== undefined) {
      this.hideTooltip();
    }
  };

  /**
   * Appends to a container
   * @method appendTo
   * @param  {H5P.jquery} $container
   */
  JoubelProgressbar.prototype.appendTo = function ($container) {
    this.$progressbar.appendTo($container);
  };

  /**
   * Update progress
   * @method setProgress
   * @param  {number}    step
   */
  JoubelProgressbar.prototype.setProgress = function (step) {
    // Check for valid value:
    if (step > this.steps || step < 0) {
      return;
    }
    this.currentStep = step;
    this.$background.css({
      width: ((this.currentStep/this.steps)*100) + '%'
    });
  };

  /**
   * Increment progress with 1
   * @method next
   */
  JoubelProgressbar.prototype.next = function () {
    this.setProgress(this.currentStep+1);
  };

  /**
   * Reset progressbar
   * @method reset
   */
  JoubelProgressbar.prototype.reset = function () {
    this.setProgress(0);
  };

  /**
   * Check if last step is reached
   * @method isLastStep
   * @return {Boolean}
   */
  JoubelProgressbar.prototype.isLastStep = function () {
    return this.steps === this.currentStep;
  };

  return JoubelProgressbar;
})(H5P.jQuery);
;var H5P = H5P || {};

/**
 * H5P Joubel UI library.
 *
 * This is a utility library, which does not implement attach. I.e, it has to bee actively used by
 * other libraries
 * @module
 */
H5P.JoubelUI = (function ($) {

  /**
   * The internal object to return
   * @class H5P.JoubelUI
   * @static
   */
  function JoubelUI() {}

  /* Public static functions */

  /**
   * Create a tip icon
   * @method H5P.JoubelUI.createTip
   * @param  {string}  text   The textual tip
   * @param  {Object}  params Parameters
   * @return {H5P.JoubelTip}
   */
  JoubelUI.createTip = function (text, params) {
    return new H5P.JoubelTip(text, params);
  };

  /**
   * Create message dialog
   * @method H5P.JoubelUI.createMessageDialog
   * @param  {H5P.jQuery}               $container The dom container
   * @param  {string}                   message    The message
   * @return {H5P.JoubelMessageDialog}
   */
  JoubelUI.createMessageDialog = function ($container, message) {
    return new H5P.JoubelMessageDialog($container, message);
  };

  /**
   * Create help text dialog
   * @method H5P.JoubelUI.createHelpTextDialog
   * @param  {string}             header  The textual header
   * @param  {string}             message The textual message
   * @return {H5P.JoubelHelpTextDialog}
   */
  JoubelUI.createHelpTextDialog = function (header, message) {
    return new H5P.JoubelHelpTextDialog(header, message);
  };

  /**
   * Create progress circle
   * @method H5P.JoubelUI.createProgressCircle
   * @param  {number}             number          The progress (0 to 100)
   * @param  {string}             progressColor   The progress color in hex value
   * @param  {string}             fillColor       The fill color in hex value
   * @param  {string}             backgroundColor The background color in hex value
   * @return {H5P.JoubelProgressCircle}
   */
  JoubelUI.createProgressCircle = function (number, progressColor, fillColor, backgroundColor) {
    return new H5P.JoubelProgressCircle(number, progressColor, fillColor, backgroundColor);
  };

  /**
   * Create throbber for loading
   * @method H5P.JoubelUI.createThrobber
   * @return {H5P.JoubelThrobber}
   */
  JoubelUI.createThrobber = function () {
    return new H5P.JoubelThrobber();
  };

  /**
   * Create simple rounded button
   * @method H5P.JoubelUI.createSimpleRoundedButton
   * @param  {string}                  text The button label
   * @return {H5P.SimpleRoundedButton}
   */
  JoubelUI.createSimpleRoundedButton = function (text) {
    return new H5P.SimpleRoundedButton(text);
  };

  /**
   * Create Slider
   * @method H5P.JoubelUI.createSlider
   * @param  {H5P.jQuery}     $container The dom container
   * @param  {Object}         params     Parameters
   * @return {H5P.JoubelSlider}
   */
  JoubelUI.createSlider = function ($container, params) {
    return new H5P.JoubelSlider(params);
  };

  /**
   * Create Score Bar
   * @method H5P.JoubelUI.createScoreBar
   * @param  {number=}       maxScore The maximum score
   * @param {string} [label] Makes it easier for readspeakers to identify the scorebar
   * @return {H5P.JoubelScoreBar}
   */
  JoubelUI.createScoreBar = function (maxScore, label) {
    return new H5P.JoubelScoreBar(maxScore, label);
  };

  /**
   * Create Progressbar
   * @method H5P.JoubelUI.createProgressbar
   * @param  {number=}       numSteps The total numer of steps
   * @return {H5P.JoubelProgressbar}
   */
  JoubelUI.createProgressbar = function (numSteps) {
    return new H5P.JoubelProgressbar(numSteps);
  };

  /**
   * Create standard Joubel button
   *
   * @method H5P.JoubelUI.createButton
   * @param {object} params
   *  May hold any properties allowed by jQuery. If href is set, an A tag
   *  is used, if not a button tag is used.
   * @return {H5P.jQuery} The jquery element created
   */
  JoubelUI.createButton = function(params) {
    var type = 'button';
    if (params.href) {
      type = 'a';
    }
    else {
      params.type = 'button';
    }
    if (params.class) {
      params.class += ' h5p-joubelui-button';
    }
    else {
      params.class = 'h5p-joubelui-button';
    }
    return $('<' + type + '/>', params);
  };

  return JoubelUI;
})(H5P.jQuery);
;H5P.Question = (function ($, EventDispatcher, JoubelUI) {

  /**
   * Extending this class make it alot easier to create tasks for other
   * content types.
   *
   * @class H5P.Question
   * @extends H5P.EventDispatcher
   * @param {string} type
   */
  function Question(type) {
    var self = this;

    // Inheritance
    EventDispatcher.call(self);

    // Register default section order
    self.order = ['video', 'image', 'introduction', 'content', 'feedback', 'buttons'];

    // Keep track of registered sections
    var sections = {};

    // Buttons
    var buttons = {};
    var buttonOrder = [];

    // Wrapper when attached
    var $wrapper;

    // ScoreBar
    var scoreBar;

    // Keep track of the feedback's visual status.
    var showFeedback;

    // Keep track of which buttons are scheduled for hiding.
    var buttonsToHide = [];

    // Keep track of which buttons are scheduled for showing.
    var buttonsToShow = [];

    // Keep track of the hiding and showing of buttons.
    var toggleButtonsTimer;
    var toggleButtonsTransitionTimer;
    var buttonTruncationTimer;

    // Keeps track of initialization of question
    var initialized = false;

    /**
     * @type {Object} behaviour Behaviour of Question
     * @property {Boolean} behaviour.disableFeedback Set to true to disable feedback section
     */
    var behaviour = {
      disableFeedback: false
    };

    // Keeps track of thumb state
    var imageThumb = true;

    // Keeps track of image transitions
    var imageTransitionTimer;

    // Keep track of whether sections is transitioning.
    var sectionsIsTransitioning = false;

    // Keep track of auto play state
    var disableAutoPlay = false;

    // Feedback transition timer
    var feedbackTransitionTimer;

    /**
     * Register section with given content.
     *
     * @private
     * @param {string} section ID of the section
     * @param {(string|H5P.jQuery)} content
     */
    var register = function (section, content) {
      sections[section] = {};
      var $e = sections[section].$element = $('<div/>', {
        'class': 'h5p-question-' + section,
      });
      if (content) {
        $e[content instanceof $ ? 'append' : 'html'](content);
      }
    };

    /**
     * Update registered section with content.
     *
     * @private
     * @param {string} section ID of the section
     * @param {(string|H5P.jQuery)} content
     */
    var update = function (section, content) {
      if (content instanceof $) {
        sections[section].$element.html('').append(content);
      }
      else {
        sections[section].$element.html(content);
      }
    };

    /**
     * Insert element with given ID into the DOM.
     *
     * @private
     * @param {array} order
     * List with ordered element IDs
     * @param {string} id
     * ID of the element to be inserted
     * @param {Object} elements
     * Maps ID to the elements
     * @param {H5P.jQuery} $container
     * Parent container of the elements
     */
    var insert = function (order, id, elements, $container) {
      // Try to find an element id should be after
      for (var i = 0; i < order.length; i++) {
        if (order[i] === id) {
          // Found our pos
          while (i > 0 &&
          (elements[order[i - 1]] === undefined ||
          !elements[order[i - 1]].isVisible)) {
            i--;
          }
          if (i === 0) {
            // We are on top.
            elements[id].$element.prependTo($container);
          }
          else {
            // Add after element
            elements[id].$element.insertAfter(elements[order[i - 1]].$element);
          }
          elements[id].isVisible = true;
          break;
        }
      }
    };

    /**
     * Set element max height, used for animations.
     *
     * @param {H5P.jQuery} $element
     */
    var setElementHeight = function ($element) {
      if (!$element.is(':visible')) {
        // No animation
        $element.css('max-height', 'none');
        return;
      }

      // Get natural element height
      var $tmp = $element.clone()
        .css({
          'position': 'absolute',
          'max-height': 'none'
        }).appendTo($element.parent());

      // Apply height to element
      var h = Math.round($tmp.get(0).getBoundingClientRect().height);
      var fontSize = parseFloat($element.css('fontSize'));
      var relativeH = h / fontSize;
      $element.css('max-height', relativeH + 'em');
      $tmp.remove();

      if (h > 0 && sections.buttons && sections.buttons.$element === $element) {

        // Make sure buttons section is visible
        sections.buttons.$element.addClass('h5p-question-visible');

        // Resize buttons after resizing button section
        setTimeout(function () {
          resizeButtons();
        }, 150);
      }
      return h;
    };

    /**
     * Does the actual job of hiding the buttons scheduled for hiding.
     *
     * @private
     * @param {boolean} [relocateFocus] Find a new button to focus
     */
    var hideButtons = function (relocateFocus) {
      for (var i = 0; i < buttonsToHide.length; i++) {
        hideButton(buttonsToHide[i].id);
      }
      buttonsToHide = [];

      if (relocateFocus) {
        self.focusButton();
      }
    };

    /**
     * Does the actual hiding.
     * @private
     * @param {string} buttonId
     */
    var hideButton = function (buttonId) {
      // Using detach() vs hide() makes it harder to cheat.
      buttons[buttonId].$element.detach();
      buttons[buttonId].isVisible = false;
    };

    /**
     * Shows the buttons on the next tick. This is to avoid buttons flickering
     * If they're both added and removed on the same tick.
     *
     * @private
     */
    var toggleButtons = function () {

      // Clear transition timer, reevaluate if buttons will be detached
      clearTimeout(toggleButtonsTransitionTimer);

      // Show buttons
      for (var i = 0; i < buttonsToShow.length; i++) {
        insert(buttonOrder, buttonsToShow[i].id, buttons, sections.buttons.$element);
        buttons[buttonsToShow[i].id].isVisible = true;
      }
      buttonsToShow = [];

      // Hide buttons
      var numToHide = 0;
      var relocateFocus = false;
      for (var j = 0; j < buttonsToHide.length; j++) {
        var button = buttons[buttonsToHide[j].id];
        if (button.isVisible) {
          numToHide += 1;
        }
        if (button.$element.is(':focus')) {
          // Move focus to the first visible button.
          relocateFocus = true;
        }
      }

      if (sections.buttons && numToHide === sections.buttons.$element.children().length) {
        // All buttons are going to be hidden. Hide container using transition.
        sections.buttons.$element.removeClass('h5p-question-visible');
        sections.buttons.$element.css('max-height', '');
        sectionsIsTransitioning = true;

        // Wait for animations before detaching buttons
        toggleButtonsTransitionTimer = setTimeout(function () {
          hideButtons(relocateFocus);
          sectionsIsTransitioning = false;
        }, 150);
      }
      else {
        hideButtons(relocateFocus);

        // Show button section
        if (!sections.buttons.$element.is(':empty')) {
          sections.buttons.$element.addClass('h5p-question-visible');
          setElementHeight(sections.buttons.$element);

          // Trigger resize after animation
          toggleButtonsTransitionTimer = setTimeout(function () {
            self.trigger('resize');
          }, 150);
        }
      }

      // Resize buttons to fit container
      resizeButtons();

      toggleButtonsTimer = undefined;
    };

    /**
     * Allows for scaling of the question image.
     */
    var scaleImage = function () {
      var $imgSection = sections.image.$element;
      clearTimeout(imageTransitionTimer);

      // Add this here to avoid initial transition of the image making
      // content overflow. Alternatively we need to trigger a resize.
      $imgSection.addClass('animatable');

      if (imageThumb) {

        // Expand image
        $imgSection.addClass('h5p-question-image-fill-width');
        imageThumb = false;

        imageTransitionTimer = setTimeout(function () {
          self.trigger('resize');
        }, 600);
      }
      else {

        // Scale down image
        $imgSection.removeClass('h5p-question-image-fill-width');
        imageThumb = true;

        imageTransitionTimer = setTimeout(function () {
          self.trigger('resize');
        }, 600);
      }
    };

    /**
     * Get scrollable ancestor of element
     *
     * @private
     * @param {H5P.jQuery} $element
     * @param {Number} [currDepth=0] Current recursive calls to ancestor, stop at maxDepth
     * @param {Number} [maxDepth=5] Maximum depth for finding ancestor.
     * @returns {H5P.jQuery} Parent element that is scrollable
     */
    var findScrollableAncestor = function ($element, currDepth, maxDepth) {
      if (!currDepth) {
        currDepth = 0;
      }
      if (!maxDepth) {
        maxDepth = 5;
      }
      // Check validation of element or if we have reached document root
      if (!$element || !($element instanceof $) || document === $element.get(0) || currDepth >= maxDepth) {
        return;
      }

      if ($element.css('overflow-y') === 'auto') {
        return $element;
      }
      else {
        return findScrollableAncestor($element.parent(), currDepth + 1, maxDepth);
      }
    };

    /**
     * Scroll to bottom of Question.
     *
     * @private
     */
    var scrollToBottom = function () {
      if (!$wrapper || ($wrapper.hasClass('h5p-standalone') && !H5P.isFullscreen)) {
        return; // No scroll
      }

      var scrollableAncestor = findScrollableAncestor($wrapper);

      // Scroll to bottom of scrollable ancestor
      if (scrollableAncestor) {
        scrollableAncestor.animate({
          scrollTop: $wrapper.css('height')
        }, "slow");
      }
    };

    /**
     * Resize buttons to fit container width
     *
     * @private
     */
    var resizeButtons = function () {
      if (!buttons || !sections.buttons) {
        return;
      }

      // Clear button truncation timer if within a button truncation function
      if (buttonTruncationTimer) {
        clearTimeout(buttonTruncationTimer);
      }
	
      // Allow button section to attach before getting width
      buttonTruncationTimer = setTimeout(function () {

        // A static margin is added as buffer for smoother transitions
        var buttonsWidth = 0;
        for (var i in buttons) {
          var $element = buttons[i].$element;
          if (buttons[i].isVisible) {
			
            //Calculate exact button width
            var buttonInstanceWidth = $element.get(0).offsetWidth +
              parseFloat($element.css('margin-left')) +
              parseFloat($element.css('margin-right'));
            buttonsWidth += Math.ceil(buttonInstanceWidth) + 1;
          }
        }


        // Button section reduced by 1 pixel for cross-broswer consistency.
        var buttonSectionWidth = Math.floor(sections.buttons.$element.get(0).offsetWidth) - 1;
		
        
        buttonTruncationTimer = undefined;
        
        
        //h5pcustomize
		
		/*	H5P.jQuery(".h5p-question-show-solution").html(window.parent.Drupal.t("LBL3099"));
			H5P.jQuery(".h5p-question-show-solution").attr("title",window.parent.Drupal.t("LBL3099"));
			 
			H5P.jQuery(".h5p-question-try-again").html(window.parent.Drupal.t("LBL3200"));
			H5P.jQuery(".h5p-question-try-again").attr("title",window.parent.Drupal.t("LBL3200"));
			 
			H5P.jQuery(".h5p-question-check-answer").html(window.parent.Drupal.t("Check"));
			H5P.jQuery(".h5p-question-check-answer").attr("title",window.parent.Drupal.t("Check"));
			H5P.jQuery(".h5p-question-iv-continue").html(window.parent.Drupal.t("LBL986"));
			H5P.jQuery(".h5p-question-iv-continue").attr("title",window.parent.Drupal.t("LBL986"));*/
		
			// Remove button labels if width of buttons are too wide
			
        /*if (buttonsWidth >= buttonSectionWidth) {
          //removeButtonLabels(buttonsWidth, buttonSectionWidth);
            reduceButtonLabelsSize(buttonsWidth, buttonSectionWidth);
        }
        else {
          restoreButtonLabels(buttonsWidth, buttonSectionWidth);
        }*/
				
		var contWidth = parseInt($wrapper.find(".h5p-question-content").css("width"),10)- 50; //buffer 25
		var buttonsWidth =	parseInt($wrapper.find(".h5p-question-buttons").css("width"),10);
		console.log("contWidth:"+contWidth+"===buttonsWidth:"+buttonsWidth);
		if(contWidth > 0 && contWidth <	buttonsWidth)
		{
			console.log("reduced button");
			reduceButtonLabelsSize(buttonsWidth, buttonSectionWidth);
		}
		else
		{
		 	restoreButtonLabels(buttonsWidth, buttonSectionWidth);
		}
      }, 0);
    };
    
    /**
     * Remove button labels until they use less than max width.
     *
     * @private
     * @param {Number} buttonsWidth Total width of all buttons
     * @param {Number} maxButtonsWidth Max width allowed for buttons
     */
    var reduceButtonLabelsSize = function (buttonsWidth, maxButtonsWidth) {
      // Reverse traversal
      for (var i = buttonOrder.length - 1; i >= 0; i--) {
        var buttonId = buttonOrder[i];
        if (!buttons[buttonId].isTruncated && buttons[buttonId].isVisible) {
          var $button = buttons[buttonId].$element;
          var $tmp = $button.clone()
            .css({
              'position': 'absolute',
              'white-space': 'nowrap',
              'max-width': 'none'
            })
            .addClass('reducedh5pbuttonsize')
            .html('')
            .appendTo($button.parent());

          // Calculate new total width of buttons
          buttonsWidth = buttonsWidth - $button.outerWidth(true) + $tmp.outerWidth(true);
          // Remove label
          //$button.html('');
          $button.addClass('reducedh5pbuttonsize');
          
          buttons[buttonId].isTruncated = true;
          $tmp.remove();
          if (buttonsWidth < maxButtonsWidth) {
            // Buttons are small enough.
            return;
          }
        }
      }
    };
    

    /**
     * Remove button labels until they use less than max width.
     *
     * @private
     * @param {Number} buttonsWidth Total width of all buttons
     * @param {Number} maxButtonsWidth Max width allowed for buttons
     */
    var removeButtonLabels = function (buttonsWidth, maxButtonsWidth) {
      // Reverse traversal
      for (var i = buttonOrder.length - 1; i >= 0; i--) {
        var buttonId = buttonOrder[i];
        if (!buttons[buttonId].isTruncated && buttons[buttonId].isVisible) {
          var $button = buttons[buttonId].$element;
          var $tmp = $button.clone()
            .css({
              'position': 'absolute',
              'white-space': 'nowrap',
              'max-width': 'none'
            })
            .addClass('truncated')
            .html('')
            .appendTo($button.parent());

          // Calculate new total width of buttons
          buttonsWidth = buttonsWidth - $button.outerWidth(true) + $tmp.outerWidth(true);
          // Remove label
          $button.html('');
          $button.addClass('truncated');
          buttons[buttonId].isTruncated = true;
          $tmp.remove();
          if (buttonsWidth < maxButtonsWidth) {
            // Buttons are small enough.
            return;
          }
        }
      }
    };

    /**
     * Restore button labels until it fills maximum possible width without exceeding the max width.
     *
     * @private
     * @param {Number} buttonsWidth Total width of all buttons
     * @param {Number} maxButtonsWidth Max width allowed for buttons
     */
    var restoreButtonLabels = function (buttonsWidth, maxButtonsWidth) {
      for (var i = 0; i < buttonOrder.length; i++) {
        var buttonId = buttonOrder[i];
        if (buttons[buttonId].isTruncated && buttons[buttonId].isVisible) {

          // Check if adding label exceeds allowed width
          var $button = buttons[buttonId].$element;
          var $tmp = $button.clone()
            .css({
              'position': 'absolute',
              'white-space': 'nowrap',
              'max-width': 'none'
            }).removeClass('truncated')
            .html(buttons[buttonId].text)
            .appendTo($button.parent());

          // Make sure clone was successfull
          if(!$button.length || !$tmp.length) {
            return;
          }

          var oldButtonSize = Math.floor($button.get(0).offsetWidth) - 1;
          var newButtonSize = Math.ceil($tmp.get(0).offsetWidth) + 1;

          // Calculate new total width of buttons with a static pixel for consistency cross-browser
          buttonsWidth = buttonsWidth - Math.floor(oldButtonSize) + Math.ceil(newButtonSize) + 1;

          $tmp.remove();
          if (buttonsWidth >= maxButtonsWidth) {
            return;
          }
          // Restore label
          $button.html(buttons[buttonId].text);
          $button.removeClass('truncated');
          
          buttons[buttonId].isTruncated = false;
          
          //h5pcustomize
          $button.removeClass('reducedh5pbuttonsize');
        }
      }
    };

    /**
     * Helper function for finding index of keyValue in array
     *
     * @param {String} keyValue Value to be found
     * @param {String} key In key
     * @param {Array} array In array
     * @returns {number}
     */
    var existsInArray = function (keyValue, key, array) {
      var i;
      for (i = 0; i < array.length; i++) {
        if (array[i][key] === keyValue) {
          return i;
        }
      }
      return -1;
    };

    /**
     * Set behaviour for question.
     *
     * @param {Object} options An object containing behaviour that will be extended by Question
     */
    self.setBehaviour = function (options) {
      $.extend(behaviour, options);
    };

    /**
     * A video to display above the task.
     *
     * @param {object} params
     */
    self.setVideo = function (params) {
      sections.video = {
        $element: $('<div/>', {
          'class': 'h5p-question-video'
        })
      };

      if (disableAutoPlay) {
        params.params.autoplay = false;
      }

      // Never fit to wrapper
      params.params.fit = false;
      sections.video.instance = H5P.newRunnable(params, self.contentId, sections.video.$element, true);
      var fromVideo = false; // Hack to avoid never ending loop
      sections.video.instance.on('resize', function () {
        fromVideo = true;
        self.trigger('resize');
        fromVideo = false;
      });
      self.on('resize', function () {
        if (!fromVideo) {
          sections.video.instance.trigger('resize');
        }
      });

      return self;
    };

    /**
     * Will stop any playback going on in the task.
     */
    self.pause = function () {
      if (sections.video && sections.video.isVisible) {
        sections.video.instance.pause();
      }
    };

    /**
     * Start playback of video
     */
    self.play = function () {
      if (sections.video && sections.video.isVisible) {
        sections.video.instance.play();
      }
    };

    /**
     * Disable auto play, useful in editors.
     */
    self.disableAutoPlay = function () {
      disableAutoPlay = true;
    };

    /**
     * Add task image.
     *
     * @param {string} path Relative
     * @param {Object} [options] Options object
     * @param {string} [options.alt] Text representation
     * @param {Boolean} [options.disableImageZooming] Set as true to disable image zooming
     */
    self.setImage = function (path, options) {
      options = options ? options : {};
      sections.image = {};
      // Image container
      sections.image.$element = $('<div/>', {
        'class': 'h5p-question-image h5p-question-image-fill-width'
      });

      // Inner wrap
      var $imgWrap = $('<div/>', {
        'class': 'h5p-question-image-wrap',
        appendTo: sections.image.$element
      });

      // Image element
      var $img = $('<img/>', {
        src: H5P.getPath(path, self.contentId),
        alt: (options.alt === undefined ? '' : options.alt),
        on: {
          load: function () {
            self.trigger('imageLoaded', this);
            self.trigger('resize');
          }
        },
        appendTo: $imgWrap
      });

      // Disable image zooming
      if (options.disableImageZooming) {
        $img.css('maxHeight', 'none');

        // Make sure we are using the correct amount of width at all times
        var determineImgWidth = function () {

          // Remove margins if natural image width is bigger than section width
          var imageSectionWidth = sections.image.$element.get(0).getBoundingClientRect().width;

          // Do not transition, for instant measurements
          $imgWrap.css({
            '-webkit-transition': 'none',
            'transition': 'none'
          });

          // Margin as translateX on both sides of image.
          var diffX = 2 * ($imgWrap.get(0).getBoundingClientRect().left -
            sections.image.$element.get(0).getBoundingClientRect().left);

          if ($img.get(0).naturalWidth >= imageSectionWidth - diffX) {
            sections.image.$element.addClass('h5p-question-image-fill-width');
          }
          else { // Use margin for small res images
            sections.image.$element.removeClass('h5p-question-image-fill-width');
          }

          // Reset transition rules
          $imgWrap.css({
            '-webkit-transition': '',
            'transition': ''
          });
        };

        // Determine image width
        if ($img.is(':visible')) {
          determineImgWidth();
        }
        else {
          $img.load(function () {
            determineImgWidth();
          });
        }

        // Skip adding zoom functionality
        return;
      }

      var sizeDetermined = false;
      var determineSize = function () {

        if (sizeDetermined || !$img.is(':visible')) {
          return; // Try again next time.
        }

        $img.attr('role', 'button').attr('tabIndex', '0');
        $imgWrap.addClass('h5p-question-image-scalable')
          .on('click', function (event) {
            if (event.which === 1) {
              scaleImage(); // Left mouse button click
            }
          }).on('keypress', function (event) {
          if (event.which === 32) {
            scaleImage(); // Space bar pressed
          }
        });
        sections.image.$element.removeClass('h5p-question-image-fill-width');

        sizeDetermined  = true; // Prevent any futher events
      };

      self.on('resize', determineSize);

      return self;
    };

    /**
     * Add the introduction section.
     *
     * @param {(string|H5P.jQuery)} content
     */
    self.setIntroduction = function (content) {
      register('introduction', content);

      return self;
    };

    /**
     * Add the content section.
     *
     * @param {(string|H5P.jQuery)} content
     * @param {Object} [options]
     * @param {string} [options.class]
     */
    self.setContent = function (content, options) {
      register('content', content);

      if (options && options.class) {
        sections.content.$element.addClass(options.class);
      }

      return self;
    };

    /**
     * Force readspeaker to read text. Useful when you have to use
     * setTimeout for animations.
     */
    self.read = function (content) {
      // Read text from content
      var $el = $('<div/>', {
        'aria-live': 'assertive',
        'class': 'h5p-hidden-read',
        'html': content,
        appendTo: $wrapper
      });
      setTimeout(function () { $el.remove(); }, 1);
    };

    /**
     * Set feedback message.
     * Setting the message to blank or undefined will hide it again.
     *
     * @param {string} content
     * @param {number} score The score
     * @param {number} maxScore The maximum score for this question
     * @param {string} [scoreBarLabel] Makes it easier for readspeakers to identify the scorebar
     */
    self.setFeedback = function (content, score, maxScore, scoreBarLabel) {

      // Feedback is disabled
      if (behaviour.disableFeedback) {
        return self;
      }
      clearTimeout(feedbackTransitionTimer);

      if (content) {
        var $feedback = $('<div>', {
          'class': 'h5p-question-feedback-container'
        });

        if (scoreBar === undefined) {
          scoreBar = JoubelUI.createScoreBar(maxScore, scoreBarLabel);
        }
        scoreBar.appendTo($feedback);
        scoreBar.setScore(score);
        $feedback.append($('<div>', {
          'class': 'h5p-question-feedback-content',
          'html': content
        }));

        // Feedback for readspeakers
        self.read(content);

        showFeedback = true;
        if (sections.feedback) {
          // Update section
          update('feedback', $feedback);
        }
        else {
          // Create section
          register('feedback', $feedback);
          if (initialized && $wrapper) {
            insert(self.order, 'feedback', sections, $wrapper);
          }
        }

        // Show feedback section
        feedbackTransitionTimer = setTimeout(function () {
          sections.feedback.$element.addClass('h5p-question-visible');
          setElementHeight(sections.feedback.$element);
          sectionsIsTransitioning = true;

          // Scroll to bottom after showing feedback
          scrollToBottom();

          // Trigger resize after animation
          feedbackTransitionTimer = setTimeout(function () {
            sectionsIsTransitioning = false;
            self.trigger('resize');
          }, 150);
        }, 0);

      }
      else if (sections.feedback && showFeedback) {
        showFeedback = false;

        // Hide feedback section
        sections.feedback.$element.removeClass('h5p-question-visible');
        sections.feedback.$element.css('max-height', '');
        sectionsIsTransitioning = true;

        // Detach after transition
        feedbackTransitionTimer = setTimeout(function () {
          // Avoiding Transition.onTransitionEnd since it will register multiple events, and there's no way to cancel it if the transition changes back to "show" while the animation is happening.
          if (!showFeedback) {
            sections.feedback.$element.children().detach();

            // Trigger resize after animation
            self.trigger('resize');
          }
          sectionsIsTransitioning = false;
          scoreBar.setScore(0);
        }, 150);
      }

      return self;
    };

    /**
     * Set feedback content (no animation).
     *
     * @param {string} content
     * @param {boolean} [extendContent] True will extend content, instead of replacing it
     */
    self.updateFeedbackContent = function (content, extendContent) {
      if (sections.feedback && sections.feedback.$element) {

        if (extendContent) {
          content = $('.h5p-question-feedback-content', sections.feedback.$element).html() + ' ' + content;
        }

        // Update feedback content html
        $('.h5p-question-feedback-content', sections.feedback.$element).html(content);
      }

      return self;
    };

    /**
     * Checks to see if button is registered.
     *
     * @param {string} id
     * @returns {boolean}
     */
    self.hasButton = function (id) {
      return (buttons[id] !== undefined);
    };

    /**
     * Register buttons for the task.
     *
     * @param {string} id
     * @param {string} text label
     * @param {function} clicked
     * @param {boolean} [visible=true]
     * @param {Object} [options] Options for button
     */
    self.addButton = function (id, text, clicked, visible, options) {
      if (buttons[id]) {
        return self; // Already registered
      }

      if (sections.buttons === undefined)  {
        // We have buttons, register wrapper
        register('buttons');
        if (initialized) {
          insert(self.order, 'buttons', sections, $wrapper);
        }
      }

      options = options || {};

     
      //h5pcustomize - apply multi lang button labels in better way #84566
      if(text == "Continue"){
    	  	text = window.parent.Drupal.t("LBL986");    	  
      }
      else if(text == "Check")
    	  {
    	  	text = window.parent.Drupal.t("Check");
    	  }
      else if(text == "Show solutions")
    	  {
    	  	text = window.parent.Drupal.t("LBL3099");
    	  }
      else if(text =="Retry")
    	  {
    		text = window.parent.Drupal.t("LBL3200");
    	  }
      
      buttons[id] = {
    	        isTruncated: false,
    	        text: text
    	      };
      var $e = buttons[id].$element = JoubelUI.createButton($.extend({
        'class': 'h5p-question-' + id,
        html: text,
        title: text,
        on: {
          click: function () {
            clicked();
          }
        }
      }, options));
      buttonOrder.push(id);

      if (visible === undefined || visible) {
        // Button should be visible
        $e.appendTo(sections.buttons.$element);
        buttons[id].isVisible = true;
        sections.buttons.$element.addClass('h5p-question-visible');
      }

      return self;
    };

    /**
     * Show registered button with given identifier.
     *
     * @param {string} id
     * @param {Number} [priority]
     */
    self.showButton = function (id, priority) {
      if (buttons[id] === undefined) {
        return self;
      }

      priority = priority || 0;

      // Skip if already being shown
      var indexToShow = existsInArray(id, 'id', buttonsToShow);
      if (indexToShow !== -1) {

        // Update priority
        if (buttonsToShow[indexToShow].priority < priority) {
          buttonsToShow[indexToShow].priority = priority;
        }

        return self;
      }

      // Check if button is going to be hidden on next tick
      var exists = existsInArray(id, 'id', buttonsToHide);
      if (exists !== -1) {

        // Skip hiding if higher priority
        if (buttonsToHide[exists].priority <= priority) {
          buttonsToHide.splice(exists, 1);
          buttonsToShow.push({id: id, priority: priority});
        }

      } // If button is not shown
      else if (!buttons[id].$element.is(':visible')) {

        // Show button on next tick
        buttonsToShow.push({id: id, priority: priority});
      }

      if (!toggleButtonsTimer) {
        toggleButtonsTimer = setTimeout(toggleButtons, 0);
      }

      return self;
    };

    /**
     * Hide registered button with given identifier.
     *
     * @param {string} id
     * @param {number} [priority]
     */
    self.hideButton = function (id, priority) {
      if (buttons[id] === undefined) {
        return self;
      }

      priority = priority || 0;

      // Skip if already being hidden
      var indexToHide = existsInArray(id, 'id', buttonsToHide);
      if (indexToHide !== -1) {

        // Update priority
        if (buttonsToHide[indexToHide].priority < priority) {
          buttonsToHide[indexToHide].priority = priority;
        }

        return self;
      }

      // Check if buttons is going to be shown on next tick
      var exists = existsInArray(id, 'id', buttonsToShow);
      if (exists !== -1) {

        // Skip showing if higher priority
        if (buttonsToShow[exists].priority <= priority) {
          buttonsToShow.splice(exists, 1);
          buttonsToHide.push({id: id, priority: priority});
        }
      }
      else if (!buttons[id].$element.is(':visible')) {

        // Make sure it is detached in case the container is hidden.
        hideButton(id);
      }
      else {

        // Hide button on next tick.
        buttonsToHide.push({id: id, priority: priority});
      }

      if (!toggleButtonsTimer) {
        toggleButtonsTimer = setTimeout(toggleButtons, 0);
      }

      return self;
    };

    /**
     * Set focus to the given button. If no button is given the first visible
     * button gets focused. This is useful if you lose focus.
     *
     * @param {string} [id]
     */
    self.focusButton = function (id) {
      if (id === undefined) {
        // Find first button that is visible.
        for (var i = 0; i < buttonOrder.length; i++) {
          if (buttons[buttonOrder[i]].isVisible) {
            // Give that button focus
            buttons[buttonOrder[i]].$element.focus();
            break;
          }
        }
      }
      else if (buttons[id].$element.is(':visible')) {
        // Set focus to requested button
        buttons[id].$element.focus();
      }

      return self;
    };

    /**
     * Set new element for section.
     *
     * @param {String} id
     * @param {H5P.jQuery} $element
     */
    self.insertSectionAtElement = function (id, $element) {
      if (sections[id] === undefined) {
        register(id);
      }
      sections[id].parent = $element;

      // Insert section if question is not initialized
      if (!initialized) {
        insert([id], id, sections, $element);
      }

      return self;
    };

    /**
     * Attach content to given container.
     *
     * @param {H5P.jQuery} $container
     */
    self.attach = function ($container) {
      if (self.isRoot()) {
        self.setActivityStarted();
      }

      // The first time we attach we also create our DOM elements.
      if ($wrapper === undefined) {
        if (self.registerDomElements !== undefined &&
           (self.registerDomElements instanceof Function ||
           typeof self.registerDomElements === 'function')) {

           // Give the question type a chance to register before attaching
          self.registerDomElements();
        }
        self.trigger('registerDomElements');
      }

      // Prepare container
      $wrapper = $container;
      $container.html('')
        .attr('role', 'application')
        .addClass('h5p-question h5p-' + type);

      // Add sections in given order
      var $sections = [];
      for (var i = 0; i < self.order.length; i++) {
        var section = self.order[i];
        if (sections[section]) {
          if (sections[section].parent) {
            // Section has a different parent
            sections[section].$element.appendTo(sections[section].parent);
          }
          else {
            $sections.push(sections[section].$element);
          }
          sections[section].isVisible = true;
        }
      }

      // Only append once to DOM for optimal performance
      $container.append($sections);

      // Let others react to dom changes
      self.trigger('domChanged', {
        '$target': $container,
        'library': self.libraryInfo.machineName,
        'contentId': self.contentId,
        'key': 'newLibrary'
      }, {'bubbles': true, 'external': true});

      // ??
      initialized = true;

      return self;
    };

    /**
     * Detach all sections from their parents
     */
    self.detachSections = function () {
      // Deinit Question
      initialized = false;

      // Detach sections
      for (var section in sections) {
        sections[section].$element.detach();
      }

      return self;
    };

    // Listen for resize
    self.on('resize', function () {
      // Allow elements to attach and set their height before resizing
      if (!sectionsIsTransitioning && sections.feedback && showFeedback) {
        // Resize feedback to fit
        setElementHeight(sections.feedback.$element);
      }

      resizeButtons();
    });
  }

  // Inheritance
  Question.prototype = Object.create(EventDispatcher.prototype);
  Question.prototype.constructor = Question;

  return Question;
})(H5P.jQuery, H5P.EventDispatcher, H5P.JoubelUI);
;/*global H5P*/
H5P.Blanks = (function ($, Question) {
  /**
   * @constant
   * @default
   */
  var STATE_ONGOING = 'ongoing';
  var STATE_CHECKING = 'checking';
  var STATE_SHOWING_SOLUTION = 'showing-solution';
  var STATE_FINISHED = 'finished';

  /**
   * Initialize module.
   *
   * @class H5P.Blanks
   * @extends H5P.Question
   * @param {Object} params Behavior settings
   * @param {number} id Content identification
   * @param {Object} contentData Task specific content data
   */
  function Blanks(params, id, contentData) {
    var self = this;

    // Inheritance
    Question.call(self, 'blanks');

    // IDs
    this.contentId = id;

//h5pcustomize
	var showSol = false;
	var retry = false;
	try{
	if(contentData != undefined && contentData.parent != undefined && contentData.parent.override != undefined && contentData.parent.override.overrideButtons !=undefined &&  contentData.parent.override.overrideButtons == "true" || contentData.parent.override.overrideButtons == true)
	{
	
		if(contentData.parent.override.overrideShowSolutionButton == "true" || contentData.parent.override.overrideShowSolutionButton == true)
		{
	
			showSol =true;
		}
		if(contentData.parent.override.overrideRetry == "true" || contentData.parent.override.overrideRetry == true)
		{
	
			retry =true;
		}
			
	} 
	}catch(e){}
	
    // Set default behavior.
    this.params = $.extend(true, {}, {
      text: "Fill in",
      questions: [
        ""
      ],
      userAnswers: [],
      score: window.parent.Drupal.t("LBL3202")+ " @score "+window.parent.Drupal.t("LBL981")+" @total "+window.parent.Drupal.t("LBL1064"), //"You got @score of @total points.",
      showSolutions: window.parent.Drupal.t("LBL3099"),
      tryAgain: window.parent.Drupal.t("LBL3200"),
      checkAnswer: window.parent.Drupal.t("Check"),
      changeAnswer: "Change answer",
      notFilledOut: window.parent.Drupal.t("LBL3201"),
      behaviour: {
        enableRetry: retry,
        enableSolutionsButton: showSol,
        caseSensitive: false,
        showSolutionsRequiresInput: true,
        autoCheck: false,
        separateLines: false,
        disableImageZooming: false
      }
    }, params);

    // Delete empty questions
    for (var i = this.params.questions.length - 1; i >= 0; i--) {
      if (!this.params.questions[i]) {
        this.params.questions.splice(i, 1);
      }
    }

    // Previous state
    this.contentData = contentData;
    if (this.contentData !== undefined && this.contentData.previousState !== undefined) {
      this.previousState = this.contentData.previousState;
    }

    // Clozes
    this.clozes = [];

    // Keep track tabbing forward or backwards
    this.shiftPressed = false;

    H5P.$body.keydown(function (event) {
      if (event.keyCode === 16) {
        self.shiftPressed = true;
      }
    }).keyup(function (event) {
      if (event.keyCode === 16) {
        self.shiftPressed = false;
      }
    });
  }

  // Inheritance
  Blanks.prototype = Object.create(Question.prototype);
  Blanks.prototype.constructor = Blanks;

  /**
   * Registers this question type's DOM elements before they are attached.
   * Called from H5P.Question.
   */
  Blanks.prototype.registerDomElements = function () {
    var self = this;

    if (self.params.image) {
      // Register task image
      self.setImage(self.params.image.path, {disableImageZooming: self.params.behaviour.disableImageZooming});
    }

    // Register task introduction text
    self.setIntroduction(self.params.text);

    // Register task content area
    self.setContent(self.createQuestions(), {
      'class': self.params.behaviour.separateLines ? 'h5p-separate-lines' : ''
    });

    // ... and buttons
    self.registerButtons();

    // Restore previous state
    self.setH5PUserState();
  };

  /**
   * Create all the buttons for the task
   */
  Blanks.prototype.registerButtons = function () {
    var self = this;

    if (!self.params.behaviour.autoCheck) {
      // Check answer button
      self.addButton('check-answer', self.params.checkAnswer, function () {
        self.toggleButtonVisibility(STATE_CHECKING);
        self.markResults();
        self.showEvaluation();
        self.triggerAnswered();
      });
    }

    // Check answer button
    self.addButton('show-solution', self.params.showSolutions, function () {
      if (self.allBlanksFilledOut()) {
        self.toggleButtonVisibility(STATE_SHOWING_SOLUTION);
        self.showCorrectAnswers();
      }
    }, self.params.behaviour.enableSolutionsButton);

    // Try again button
    if (self.params.behaviour.enableRetry === true) {
      self.addButton('try-again', self.params.tryAgain, function () {
        self.resetTask();
        self.$questions.filter(':first').find('input:first').focus();
      });
    }
    self.toggleButtonVisibility(STATE_ONGOING);
  };

  /**
   * Find blanks in a string and run a handler on those blanks
   *
   * @param {string} question
   *   Question text containing blanks enclosed in asterisks.
   * @param {function} handler
   *   Replaces the blanks text with an input field.
   * @returns {string}
   *   The question with blanks replaced by the given handler.
   */
  Blanks.prototype.handleBlanks = function (question, handler) {
    // Go through the text and run handler on all asterix
    var clozeEnd, clozeStart = question.indexOf('*');
    var self = this;
    while (clozeStart !== -1 && clozeEnd !== -1) {
      clozeStart++;
      clozeEnd = question.indexOf('*', clozeStart);
      if (clozeEnd === -1) {
        continue; // No end
      }
      var replacer = handler(self.parseSolution(question.substring(clozeStart, clozeEnd)));
      clozeEnd++;
      question = question.slice(0, clozeStart - 1) + replacer + question.slice(clozeEnd);
      clozeEnd -= clozeEnd - clozeStart - replacer.length;

      // Find the next cloze
      clozeStart = question.indexOf('*', clozeEnd);
    }
    return question;
  };

  /**
   * Create questitons html for DOM
   */
  Blanks.prototype.createQuestions = function () {
    var self = this;

    var html = '';
    for (var i = 0; i < self.params.questions.length; i++) {
      var question = self.params.questions[i];

      // Go through the question text and replace all the asterisks with input fields
      question = self.handleBlanks(question, function(solution) {
        // Create new cloze
        var defaultUserAnswer = (self.params.userAnswers.length > self.clozes.length ? self.params.userAnswers[self.clozes.length] : null);
        var cloze = new Blanks.Cloze(solution, self.params.behaviour, defaultUserAnswer);

        self.clozes.push(cloze);
        return cloze;
      });

      html += '<div>' + question + '</div>';
    }

    this.$questions = $(html);

    // Set input fields.
    this.$questions.find('input').each(function (i) {
      var afterCheck;
      if (self.params.behaviour.autoCheck) {
        afterCheck = function () {
          if (self.done || self.getAnswerGiven()) {
            // All answers has been given. Show solutions button.
            self.toggleButtonVisibility(STATE_CHECKING);
            self.showEvaluation();
            self.done = true;
            self.triggerAnswered();
          }
        };
      }
      self.clozes[i].setInput($(this), afterCheck, function () {
        self.toggleButtonVisibility(STATE_ONGOING);
        if (!self.params.behaviour.autoCheck) {
          self.hideEvaluation();
        }
      });
    }).keydown(function (event) {
      self.autoGrowTextField($(this));

      if (event.keyCode === 13) {
        return false; // Prevent form submission on enter key
      }

      // Refocus buttons after they have been toggled if last input
      if (event.keyCode === 9 && self.params.behaviour.autoCheck) {
        var $inputs = self.$questions.find('.h5p-input-wrapper:not(.h5p-correct) .h5p-text-input');
        var $lastInput = $inputs[$inputs.length - 1];
        if ($(this).is($lastInput) && !self.shiftPressed) {
          setTimeout(function () {
            self.focusButton();
          }, 10);
        }
      }
    }).on('change', function () {
      self.triggerXAPI('interacted');
    });

    self.on('resize', function () {
      self.resetGrowTextField();
    });

    return this.$questions;
  };

  /**
   *
   */
  Blanks.prototype.autoGrowTextField = function ($input) {
    // Do not set text field size when separate lines is enabled
    if (this.params.behaviour.separateLines) {
      return;
    }

    var self = this;
    var fontSize = parseInt($input.css('font-size'), 10);
    var minEm = 3;
    var minPx = fontSize * minEm;
    var rightPadEm = 3.25;
    var rightPadPx = fontSize * rightPadEm;
    var static_min_pad = 0.5 * fontSize;

    setTimeout(function(){
      var tmp = $('<div>', {
        'html': $input.val()
      });
      tmp.css({
        'position': 'absolute',
        'white-space': 'nowrap',
        'font-size': $input.css('font-size'),
        'font-family': $input.css('font-family'),
        'padding': $input.css('padding'),
        'width': 'initial'
      });
      $input.parent().append(tmp);
      var width = tmp.width();
      var parentWidth = self.$questions.width();
      tmp.remove();
  /*    if (width <= minPx) {       // commented for removing the inline width for textbox
        // Apply min width
        $input.width(minPx + static_min_pad);
      } else if (width + rightPadPx >= parentWidth) {

        // Apply max width of parent
        $input.width(parentWidth - rightPadPx);
      } else {

        // Apply width that wraps input
        $input.width(width + static_min_pad);
      } */

    }, 1);
  };

  /**
   * Resize all text field growth to current size.
   */
  Blanks.prototype.resetGrowTextField = function () {
    var self = this;

    this.$questions.find('input').each(function () {
      self.autoGrowTextField($(this));
    });
  };

  /**
   * Toggle buttons dependent of state.
   *
   * Using CSS-rules to conditionally show/hide using the data-attribute [data-state]
   */
  Blanks.prototype.toggleButtonVisibility = function (state) {
    // The show solutions button is hidden if all answers are correct
    var allCorrect = (this.getScore() === this.getMaxScore());
    if (this.params.behaviour.autoCheck && allCorrect) {
      // We are viewing the solutions
      state = STATE_FINISHED;
    }

    if (this.params.behaviour.enableSolutionsButton) {
      if (state === STATE_CHECKING && !allCorrect) {
        this.showButton('show-solution');

      }
      else {
        this.hideButton('show-solution');
      }
    }

    if (this.params.behaviour.enableRetry) {
      if ((state === STATE_CHECKING && !allCorrect) || state === STATE_SHOWING_SOLUTION) {
        this.showButton('try-again');
      }
      else {
        this.hideButton('try-again');
      }
    }

    if (state === STATE_ONGOING) {
      this.showButton('check-answer');
    }
    else {
      this.hideButton('check-answer');
    }

    this.trigger('resize');
  };

  /**
   * Check if all blanks are filled out. Warn user if not
   */
  Blanks.prototype.allBlanksFilledOut = function () {
    var self = this;

    if (!self.getAnswerGiven()) {
    
      //h5pcustomize
      self.params.notFilledOut =  window.parent.Drupal.t("LBL3201");
       
      this.updateFeedbackContent(self.params.notFilledOut);
      return false;
    }

    return true;
  };

  /**
   * Mark which answers are correct and which are wrong and disable fields if retry is off.
   */
  Blanks.prototype.markResults = function () {
    var self = this;
    for (var i = 0; i < self.clozes.length; i++) {
      self.clozes[i].checkAnswer();
      if (!self.params.behaviour.enableRetry) {
        self.clozes[i].disableInput();
      }
    }
    this.trigger('resize');
  };

  /**
   * Removed marked results
   */
  Blanks.prototype.removeMarkedResults = function () {
    this.$questions.find('.h5p-input-wrapper').removeClass('h5p-correct h5p-wrong');
    this.$questions.find('.h5p-input-wrapper > input').attr('disabled', false);
    this.trigger('resize');
  };


  /**
   * Displays the correct answers
   */
  Blanks.prototype.showCorrectAnswers = function () {
    var self = this;
    this.hideSolutions();

    for (var i = 0; i < self.clozes.length; i++) {
      self.clozes[i].showSolution();
    }
    this.trigger('resize');
  };

  /**
   * Display the correct solution for the input boxes.
   *
   * This is invoked from CP - be carefull!
   */
  Blanks.prototype.showSolutions = function () {
    this.params.behaviour.enableSolutionsButton = true;
    this.toggleButtonVisibility(STATE_FINISHED);
    this.markResults();
    this.showCorrectAnswers();
    this.showEvaluation();
    //Hides all buttons in "show solution" mode.
    this.hideButtons();
  };

  /**
   * Resets the complete task.
   * Used in contracts.
   * @public
   */
  Blanks.prototype.resetTask = function () {
    this.hideEvaluation();
    this.hideSolutions();
    this.clearAnswers();
    this.removeMarkedResults();
    this.toggleButtonVisibility(STATE_ONGOING);
    this.resetGrowTextField();
    this.done = false;
  };

  /**
   * Hides all buttons.
   * @public
   */
  Blanks.prototype.hideButtons = function () {
    this.toggleButtonVisibility(STATE_FINISHED);
  };

  /**
   * Trigger xAPI answered event
   */
  Blanks.prototype.triggerAnswered = function() {
    var xAPIEvent = this.createXAPIEventTemplate('answered');
    this.addQuestionToXAPI(xAPIEvent);
    this.addResponseToXAPI(xAPIEvent);
    this.trigger(xAPIEvent);
  };

  /**
   * Add the question itselt to the definition part of an xAPIEvent
   */
  Blanks.prototype.addQuestionToXAPI = function(xAPIEvent) {
    var definition = xAPIEvent.getVerifiedStatementValue(['object', 'definition']);
    definition.description = {
      'en-US': this.params.text
    };
    definition.type = 'http://adlnet.gov/expapi/activities/cmi.interaction';
    definition.interactionType = 'fill-in';
    definition.correctResponsesPattern = ['{case_matters=' + this.params.behaviour.caseSensitive + '}'];
    var firstCorrectResponse = true;
    // xAPI forces us to create solution patterns for all possible solution combinations
    for (var i = 0; i < this.params.questions.length; i++) {
      var question = this.handleBlanks(this.params.questions[i], function(solution) {
        // Store new patterns for each extra alternative answer
        var newPatterns = [];
        for (var j = 0; j < definition.correctResponsesPattern.length; j++) {
          if (!firstCorrectResponse) {
            definition.correctResponsesPattern[j] += '[,]';
          }
          var prefix = definition.correctResponsesPattern[j];
          for (var k = 0; k < solution.solutions.length; k++) {
            if (k === 0) {
              // This is the first possible answr, just add it to the pattern
              definition.correctResponsesPattern[j] += solution.solutions[k];
            }
            else {
              // This is an alternative possible answer, we need to create a new permutation
              newPatterns.push(prefix + solution.solutions[k])
            }
          }
        }
        // Add any new permutations to the list of response patterns
        definition.correctResponsesPattern = definition.correctResponsesPattern.concat(newPatterns);
        
        firstCorrectResponse = false;
        
        // We replace the solutions in the question with a "blank"
        return '__________';
      });
      definition.description['en-US'] += question;
    }
  };
  
  /**
   * Parse the solution text (text between the asterix)
   * 
   * @param {string} solutionText
   * @returns {object} with the following properties
   *  - tip: the tip text for this solution, undefined if no tip
   *  - solutions: array of solution words
   */
  Blanks.prototype.parseSolution = function (solutionText) {
  
    //h5pcustomize - #73817
    this.params.behaviour.caseSensitive = false;
    var solutions = [];
    var tip;

    var solutionsAndTip = solutionText.split(':');
    
    if (solutionsAndTip.length > 0) {
      solutions = solutionsAndTip[0].split('/');
    }
    
    if (solutionsAndTip.length === 2) {
      tip = solutionsAndTip[1];
    }
    // Trim solutions
    for (var i = 0; i < solutions.length; i++) {
      solutions[i] = H5P.trim(solutions[i]);
      if (this.params.behaviour.caseSensitive !== true) {
        solutions[i] = solutions[i].toLowerCase();
      }
    }
    
    return {
      tip: tip,
      solutions: solutions
    };
  }

  /**
   * Add the response part to an xAPI event
   *
   * @param {H5P.XAPIEvent} xAPIEvent
   *  The xAPI event we will add a response to
   */
  Blanks.prototype.addResponseToXAPI = function (xAPIEvent) {
    xAPIEvent.setScoredResult(this.getScore(), this.getMaxScore(), this);
    var usersAnswers = this.getCurrentState();

    xAPIEvent.data.statement.result.response = usersAnswers.join('[,]');
  };

  /**
   * Show evaluation widget, i.e: 'You got x of y blanks correct'
   */
  Blanks.prototype.showEvaluation = function () {
    var maxScore = this.getMaxScore();
    var score = this.getScore();
     //h5pcustomize
    this.params.score = window.parent.Drupal.t("LBL3202")+ " @score "+window.parent.Drupal.t("LBL981")+" @total "+window.parent.Drupal.t("LBL3207"); 
    var scoreText = this.params.score.replace('@score', score).replace('@total', maxScore);
    
    this.setFeedback(scoreText, score, maxScore);

    if (score === maxScore) {
      this.toggleButtonVisibility(STATE_FINISHED);
    }
  };

  /**
   * Hide the evaluation widget
   */
  Blanks.prototype.hideEvaluation = function () {
    // Clear evaluation section.
    this.setFeedback();
  };

  /**
   * Hide solutions. (/try again)
   */
  Blanks.prototype.hideSolutions = function () {
    // Clean solution from quiz
    this.$questions.find('.h5p-correct-answer').remove();
  };

  /**
   * Get maximum number of correct answers.
   *
   * @returns {Number} Max points
   */
  Blanks.prototype.getMaxScore = function () {
    var self = this;
    return self.clozes.length;
  };

  /**
   * Count the number of correct answers.
   *
   * @returns {Number} Points
   */
  Blanks.prototype.getScore = function () {
    var self = this;
    var correct = 0;
    for (var i = 0; i < self.clozes.length; i++) {
      if (self.clozes[i].correct()) {
        correct++;
      }
      self.params.userAnswers[i] = self.clozes[i].getUserAnswer();
    }

    return correct;
  };

  Blanks.prototype.getTitle = function() {
    return H5P.createTitle(this.params.text);
  };

  /**
   * Clear the user's answers
   */
  Blanks.prototype.clearAnswers = function () {
    this.$questions.find('.h5p-text-input').val('');
  };

  /**
   * Checks if all has been answered.
   *
   * @returns {Boolean}
   */
  Blanks.prototype.getAnswerGiven = function () {
    var self = this;

    if (this.params.behaviour.showSolutionsRequiresInput === true) {
      for (var i = 0; i < self.clozes.length; i++) {
        if (!self.clozes[i].filledOut()) {
          return false;
        }
      }
    }

    return true;
  };

  /**
   * Helps set focus the given input field.
   * @param {jQuery} $input
   */
  Blanks.setFocus = function ($input) {
    setTimeout(function () {
      $input.focus();
    }, 1);
  };

  /**
   * Returns an object containing content of each cloze
   *
   * @returns {object} object containing content for each cloze
   */
  Blanks.prototype.getCurrentState = function () {
    var clozesContent = [];

    // Get user input for every cloze
    this.clozes.forEach(function (cloze) {
      clozesContent.push(cloze.getUserInput());
    });
    return clozesContent;
  };

  /**
   * Sets answers to current user state
   */
  Blanks.prototype.setH5PUserState = function () {
    var self = this;
    var isValidState = (this.previousState !== undefined &&
                        this.previousState.length &&
                        this.previousState.length === this.clozes.length);

    // Check that stored user state is valid
    if (!isValidState) {
      return;
    }

    // Set input from user state
    var hasAllClozesFilled = true;
    this.previousState.forEach(function (clozeContent, ccIndex) {
      var cloze = self.clozes[ccIndex];
      cloze.setUserInput(clozeContent);

      // Handle instant feedback
      if (self.params.behaviour.autoCheck) {
        if (cloze.filledOut()) {
          cloze.checkAnswer();
        } else {
          hasAllClozesFilled = false;
        }
      }
    });

    if (self.params.behaviour.autoCheck && hasAllClozesFilled) {
      self.showEvaluation();
      self.toggleButtonVisibility(STATE_CHECKING);
    }
    //h5pcustomize
    if(self.params.behaviour.enableRetry == false)
    {
     		self.toggleButtonVisibility(STATE_CHECKING);
        	self.markResults();
        	self.showEvaluation();
        	self.triggerAnswered();
    }
        
  };

  /**
   * Disables any active input. Useful for freezing the task and dis-allowing
   * modification of wrong answers.
   */
  Blanks.prototype.disableInput = function () {
    this.$questions.find('input').attr('disabled', true);
  };

  return Blanks;
})(H5P.jQuery, H5P.Question);
;(function ($, Blanks) {

  /**
   * Simple private class for keeping track of clozes.
   *
   * @class H5P.Blanks.Cloze
   * @param {string} answer
   * @param {Object} behaviour Behaviour for the task
   * @param {string} defaultUserAnswer
   */
  Blanks.Cloze = function (solution, behaviour, defaultUserAnswer) {
    var self = this;
    var $input, $wrapper;
    var answers = solution.solutions;
    var answer = answers.join('/');
    var tip = solution.tip;

    /**
     * Check if the answer is correct.
     *
     * @private
     * @param {string} answered
     */
    var correct = function (answered) {
      if (behaviour.caseSensitive !== true) {
        answered = answered.toLowerCase();
      }

      for (var i = 0; i < answers.length; i++) {
        if (answered === answers[i]) {
          return true;
        }
      }
      return false;
    };

    /**
     * Check if filled out.
     *
     * @param {boolean}
     */
    this.filledOut = function () {
      var answered = this.getUserAnswer();
      // Blank can be correct and is interpreted as filled out.
      return (answered !== '' || correct(answered));
    };

    /**
     * Check the cloze and mark it as wrong or correct.
     */
    this.checkAnswer = function () {
      var isCorrect = correct(this.getUserAnswer());
      if (isCorrect) {
        $wrapper.addClass('h5p-correct');
        $input.attr('disabled', true);
      }
      else {
        $wrapper.addClass('h5p-wrong');
      }
    };

    /**
     * Disables further input.
     */
    this.disableInput = function () {
      $input.attr('disabled', true);
    };

    /**
     * Show the correct solution.
     */
    this.showSolution = function () {
      if (correct(this.getUserAnswer())) {
        return; // Only for the wrong ones
      }

      $('<span class="h5p-correct-answer"> ' + answer + '</span>').insertAfter($wrapper);
      $input.attr('disabled', true);
    };

    /**
     * @returns {boolean}
     */
    this.correct = function () {
      return correct(this.getUserAnswer());
    };

    /**
     * Set input element.
     *
     * @param {H5P.jQuery} $element
     * @param {function} afterCheck
     * @param {function} afterFocus
     */
    this.setInput = function ($element, afterCheck, afterFocus) {
      $input = $element;
      $wrapper = $element.parent();

      // Add tip if tip is set
      if(tip !== undefined && tip.trim().length > 0) {
        $wrapper.addClass('has-tip').append(H5P.JoubelUI.createTip(tip, $wrapper.parent()));
      }

      if (afterCheck !== undefined) {
        $input.blur(function () {
          if (self.filledOut()) {
            // Check answers
            if (!behaviour.enableRetry) {
              self.disableInput();
            }
            self.checkAnswer();
            afterCheck();
          }
        });
      }
      $input.focus(function () {
        $wrapper.removeClass('h5p-wrong');
        if (afterFocus !== undefined) {
          afterFocus();
        }
      });
    };

    /**
     * @returns {string} Cloze html
     */
    this.toString = function () {
      var extra = defaultUserAnswer ? ' value="' + defaultUserAnswer + '"' : '';
      return '<span class="h5p-input-wrapper"><input type="text" class="h5p-text-input" autocapitalize="off"' + extra + '></span>';
    };

    /**
     * @returns {string} Trimmed answer
     */
    this.getUserAnswer = function () {
      return H5P.trim($input.val());
    };

    /**
     * @returns {string} Answer
     */
    this.getUserInput = function () {
      return $input.val();
    };

    /**
     * @param {string} text New input text
     */
    this.setUserInput = function (text) {
      $input.val(text);
    };
  };

})(H5P.jQuery, H5P.Blanks);
;/*!
* @license SoundJS
* Visit http://createjs.com/ for documentation, updates and examples.
*
* Copyright (c) 2011-2013 gskinner.com, inc.
*
* Distributed under the terms of the MIT license.
* http://www.opensource.org/licenses/mit-license.html
*
* This notice shall be included in all copies or substantial portions of the Software.
*/

/**!
 * SoundJS FlashAudioPlugin also includes swfobject (http://code.google.com/p/swfobject/)
 */

var old = this.createjs;

this.createjs=this.createjs||{},function(){var a=createjs.SoundJS=createjs.SoundJS||{};a.version="0.6.0",a.buildDate="Thu, 11 Dec 2014 23:32:09 GMT"}(),this.createjs=this.createjs||{},createjs.extend=function(a,b){"use strict";function c(){this.constructor=a}return c.prototype=b.prototype,a.prototype=new c},this.createjs=this.createjs||{},createjs.promote=function(a,b){"use strict";var c=a.prototype,d=Object.getPrototypeOf&&Object.getPrototypeOf(c)||c.__proto__;if(d){c[(b+="_")+"constructor"]=d.constructor;for(var e in d)c.hasOwnProperty(e)&&"function"==typeof d[e]&&(c[b+e]=d[e])}return a},this.createjs=this.createjs||{},createjs.indexOf=function(a,b){"use strict";for(var c=0,d=a.length;d>c;c++)if(b===a[c])return c;return-1},this.createjs=this.createjs||{},function(){"use strict";createjs.proxy=function(a,b){var c=Array.prototype.slice.call(arguments,2);return function(){return a.apply(b,Array.prototype.slice.call(arguments,0).concat(c))}}}(),this.createjs=this.createjs||{},function(){"use strict";var a=Object.defineProperty?!0:!1,b={};try{Object.defineProperty(b,"bar",{get:function(){return this._bar},set:function(a){this._bar=a}})}catch(c){a=!1}createjs.definePropertySupported=a}(),this.createjs=this.createjs||{},function(){"use strict";function a(){throw"BrowserDetect cannot be instantiated"}var b=a.agent=window.navigator.userAgent;a.isWindowPhone=b.indexOf("IEMobile")>-1||b.indexOf("Windows Phone")>-1,a.isFirefox=b.indexOf("Firefox")>-1,a.isOpera=null!=window.opera,a.isChrome=b.indexOf("Chrome")>-1,a.isIOS=(b.indexOf("iPod")>-1||b.indexOf("iPhone")>-1||b.indexOf("iPad")>-1)&&!a.isWindowPhone,a.isAndroid=b.indexOf("Android")>-1&&!a.isWindowPhone,a.isBlackberry=b.indexOf("Blackberry")>-1,createjs.BrowserDetect=a}(),this.createjs=this.createjs||{},function(){"use strict";function a(){this._listeners=null,this._captureListeners=null}var b=a.prototype;a.initialize=function(a){a.addEventListener=b.addEventListener,a.on=b.on,a.removeEventListener=a.off=b.removeEventListener,a.removeAllEventListeners=b.removeAllEventListeners,a.hasEventListener=b.hasEventListener,a.dispatchEvent=b.dispatchEvent,a._dispatchEvent=b._dispatchEvent,a.willTrigger=b.willTrigger},b.addEventListener=function(a,b,c){var d;d=c?this._captureListeners=this._captureListeners||{}:this._listeners=this._listeners||{};var e=d[a];return e&&this.removeEventListener(a,b,c),e=d[a],e?e.push(b):d[a]=[b],b},b.on=function(a,b,c,d,e,f){return b.handleEvent&&(c=c||b,b=b.handleEvent),c=c||this,this.addEventListener(a,function(a){b.call(c,a,e),d&&a.remove()},f)},b.removeEventListener=function(a,b,c){var d=c?this._captureListeners:this._listeners;if(d){var e=d[a];if(e)for(var f=0,g=e.length;g>f;f++)if(e[f]==b){1==g?delete d[a]:e.splice(f,1);break}}},b.off=b.removeEventListener,b.removeAllEventListeners=function(a){a?(this._listeners&&delete this._listeners[a],this._captureListeners&&delete this._captureListeners[a]):this._listeners=this._captureListeners=null},b.dispatchEvent=function(a){if("string"==typeof a){var b=this._listeners;if(!b||!b[a])return!1;a=new createjs.Event(a)}else a.target&&a.clone&&(a=a.clone());try{a.target=this}catch(c){}if(a.bubbles&&this.parent){for(var d=this,e=[d];d.parent;)e.push(d=d.parent);var f,g=e.length;for(f=g-1;f>=0&&!a.propagationStopped;f--)e[f]._dispatchEvent(a,1+(0==f));for(f=1;g>f&&!a.propagationStopped;f++)e[f]._dispatchEvent(a,3)}else this._dispatchEvent(a,2);return a.defaultPrevented},b.hasEventListener=function(a){var b=this._listeners,c=this._captureListeners;return!!(b&&b[a]||c&&c[a])},b.willTrigger=function(a){for(var b=this;b;){if(b.hasEventListener(a))return!0;b=b.parent}return!1},b.toString=function(){return"[EventDispatcher]"},b._dispatchEvent=function(a,b){var c,d=1==b?this._captureListeners:this._listeners;if(a&&d){var e=d[a.type];if(!e||!(c=e.length))return;try{a.currentTarget=this}catch(f){}try{a.eventPhase=b}catch(f){}a.removed=!1,e=e.slice();for(var g=0;c>g&&!a.immediatePropagationStopped;g++){var h=e[g];h.handleEvent?h.handleEvent(a):h(a),a.removed&&(this.off(a.type,h,1==b),a.removed=!1)}}},createjs.EventDispatcher=a}(),this.createjs=this.createjs||{},function(){"use strict";function a(a,b,c){this.type=a,this.target=null,this.currentTarget=null,this.eventPhase=0,this.bubbles=!!b,this.cancelable=!!c,this.timeStamp=(new Date).getTime(),this.defaultPrevented=!1,this.propagationStopped=!1,this.immediatePropagationStopped=!1,this.removed=!1}var b=a.prototype;b.preventDefault=function(){this.defaultPrevented=this.cancelable&&!0},b.stopPropagation=function(){this.propagationStopped=!0},b.stopImmediatePropagation=function(){this.immediatePropagationStopped=this.propagationStopped=!0},b.remove=function(){this.removed=!0},b.clone=function(){return new a(this.type,this.bubbles,this.cancelable)},b.set=function(a){for(var b in a)this[b]=a[b];return this},b.toString=function(){return"[Event (type="+this.type+")]"},createjs.Event=a}(),this.createjs=this.createjs||{},function(){"use strict";function a(a,b,c){this.Event_constructor("error"),this.title=a,this.message=b,this.data=c}var b=createjs.extend(a,createjs.Event);b.clone=function(){return new createjs.ErrorEvent(this.title,this.message,this.data)},createjs.ErrorEvent=createjs.promote(a,"Event")}(),this.createjs=this.createjs||{},function(){"use strict";function a(a,b){this.Event_constructor("progress"),this.loaded=a,this.total=null==b?1:b,this.progress=0==b?0:this.loaded/this.total}var b=createjs.extend(a,createjs.Event);b.clone=function(){return new createjs.ProgressEvent(this.loaded,this.total)},createjs.ProgressEvent=createjs.promote(a,"Event")}(window),this.createjs=this.createjs||{},function(){"use strict";function a(){this.src=null,this.type=null,this.id=null,this.maintainOrder=!1,this.callback=null,this.data=null,this.method=createjs.LoadItem.GET,this.values=null,this.headers=null,this.withCredentials=!1,this.mimeType=null,this.crossOrigin="Anonymous",this.loadTimeout=8e3}var b=a.prototype={},c=a;c.create=function(b){if("string"==typeof b){var d=new a;return d.src=b,d}if(b instanceof c)return b;if(b instanceof Object)return b;throw new Error("Type not recognized.")},b.set=function(a){for(var b in a)this[b]=a[b];return this},createjs.LoadItem=c}(),function(){var a={};a.ABSOLUTE_PATT=/^(?:\w+:)?\/{2}/i,a.RELATIVE_PATT=/^[./]*?\//i,a.EXTENSION_PATT=/\/?[^/]+\.(\w{1,5})$/i,a.parseURI=function(b){var c={absolute:!1,relative:!1};if(null==b)return c;var d=b.indexOf("?");d>-1&&(b=b.substr(0,d));var e;return a.ABSOLUTE_PATT.test(b)?c.absolute=!0:a.RELATIVE_PATT.test(b)&&(c.relative=!0),(e=b.match(a.EXTENSION_PATT))&&(c.extension=e[1].toLowerCase()),c},a.formatQueryString=function(a,b){if(null==a)throw new Error("You must specify data.");var c=[];for(var d in a)c.push(d+"="+escape(a[d]));return b&&(c=c.concat(b)),c.join("&")},a.buildPath=function(a,b){if(null==b)return a;var c=[],d=a.indexOf("?");if(-1!=d){var e=a.slice(d+1);c=c.concat(e.split("&"))}return-1!=d?a.slice(0,d)+"?"+this._formatQueryString(b,c):a+"?"+this._formatQueryString(b,c)},a.isCrossDomain=function(a){var b=document.createElement("a");b.href=a.src;var c=document.createElement("a");c.href=location.href;var d=""!=b.hostname&&(b.port!=c.port||b.protocol!=c.protocol||b.hostname!=c.hostname);return d},a.isLocal=function(a){var b=document.createElement("a");return b.href=a.src,""==b.hostname&&"file:"==b.protocol},a.isBinary=function(a){switch(a){case createjs.AbstractLoader.IMAGE:case createjs.AbstractLoader.BINARY:return!0;default:return!1}},a.isImageTag=function(a){return a instanceof HTMLImageElement},a.isAudioTag=function(a){return window.HTMLAudioElement?a instanceof HTMLAudioElement:!1},a.isVideoTag=function(a){return window.HTMLVideoElement?a instanceof HTMLVideoElement:void 0},a.isText=function(a){switch(a){case createjs.AbstractLoader.TEXT:case createjs.AbstractLoader.JSON:case createjs.AbstractLoader.MANIFEST:case createjs.AbstractLoader.XML:case createjs.AbstractLoader.CSS:case createjs.AbstractLoader.SVG:case createjs.AbstractLoader.JAVASCRIPT:return!0;default:return!1}},a.getTypeByExtension=function(a){if(null==a)return createjs.AbstractLoader.TEXT;switch(a.toLowerCase()){case"jpeg":case"jpg":case"gif":case"png":case"webp":case"bmp":return createjs.AbstractLoader.IMAGE;case"ogg":case"mp3":case"webm":return createjs.AbstractLoader.SOUND;case"mp4":case"webm":case"ts":return createjs.AbstractLoader.VIDEO;case"json":return createjs.AbstractLoader.JSON;case"xml":return createjs.AbstractLoader.XML;case"css":return createjs.AbstractLoader.CSS;case"js":return createjs.AbstractLoader.JAVASCRIPT;case"svg":return createjs.AbstractLoader.SVG;default:return createjs.AbstractLoader.TEXT}},createjs.RequestUtils=a}(),this.createjs=this.createjs||{},function(){"use strict";function a(a,b,c){this.EventDispatcher_constructor(),this.loaded=!1,this.canceled=!1,this.progress=0,this.type=c,this.resultFormatter=null,this._item=a?createjs.LoadItem.create(a):null,this._preferXHR=b,this._result=null,this._rawResult=null,this._loadedItems=null,this._tagSrcAttribute=null,this._tag=null}var b=createjs.extend(a,createjs.EventDispatcher),c=a;c.POST="POST",c.GET="GET",c.BINARY="binary",c.CSS="css",c.IMAGE="image",c.JAVASCRIPT="javascript",c.JSON="json",c.JSONP="jsonp",c.MANIFEST="manifest",c.SOUND="sound",c.VIDEO="video",c.SPRITESHEET="spritesheet",c.SVG="svg",c.TEXT="text",c.XML="xml",b.getItem=function(){return this._item},b.getResult=function(a){return a?this._rawResult:this._result},b.getTag=function(){return this._tag},b.setTag=function(a){this._tag=a},b.load=function(){this._createRequest(),this._request.on("complete",this,this),this._request.on("progress",this,this),this._request.on("loadStart",this,this),this._request.on("abort",this,this),this._request.on("timeout",this,this),this._request.on("error",this,this);var a=new createjs.Event("initialize");a.loader=this._request,this.dispatchEvent(a),this._request.load()},b.cancel=function(){this.canceled=!0,this.destroy()},b.destroy=function(){this._request&&(this._request.removeAllEventListeners(),this._request.destroy()),this._request=null,this._item=null,this._rawResult=null,this._result=null,this._loadItems=null,this.removeAllEventListeners()},b.getLoadedItems=function(){return this._loadedItems},b._createRequest=function(){this._request=this._preferXHR?new createjs.XHRRequest(this._item):new createjs.TagRequest(this._item,this._tag||this._createTag(),this._tagSrcAttribute)},b._createTag=function(){return null},b._sendLoadStart=function(){this._isCanceled()||this.dispatchEvent("loadstart")},b._sendProgress=function(a){if(!this._isCanceled()){var b=null;"number"==typeof a?(this.progress=a,b=new createjs.ProgressEvent(this.progress)):(b=a,this.progress=a.loaded/a.total,b.progress=this.progress,(isNaN(this.progress)||1/0==this.progress)&&(this.progress=0)),this.hasEventListener("progress")&&this.dispatchEvent(b)}},b._sendComplete=function(){if(!this._isCanceled()){this.loaded=!0;var a=new createjs.Event("complete");a.rawResult=this._rawResult,null!=this._result&&(a.result=this._result),this.dispatchEvent(a)}},b._sendError=function(a){!this._isCanceled()&&this.hasEventListener("error")&&(null==a&&(a=new createjs.ErrorEvent("PRELOAD_ERROR_EMPTY")),this.dispatchEvent(a))},b._isCanceled=function(){return null==window.createjs||this.canceled?!0:!1},b.resultFormatter=null,b.handleEvent=function(a){switch(a.type){case"complete":this._rawResult=a.target._response;var b=this.resultFormatter&&this.resultFormatter(this),c=this;b instanceof Function?b(function(a){c._result=a,c._sendComplete()}):(this._result=b||this._rawResult,this._sendComplete());break;case"progress":this._sendProgress(a);break;case"error":this._sendError(a);break;case"loadstart":this._sendLoadStart();break;case"abort":case"timeout":this._isCanceled()||this.dispatchEvent(a.type)}},b.buildPath=function(a,b){return createjs.RequestUtils.buildPath(a,b)},b.toString=function(){return"[PreloadJS AbstractLoader]"},createjs.AbstractLoader=createjs.promote(a,"EventDispatcher")}(),this.createjs=this.createjs||{},function(){"use strict";function a(a,b,c){this.AbstractLoader_constructor(a,b,c),this.resultFormatter=this._formatResult,this._tagSrcAttribute="src"}var b=createjs.extend(a,createjs.AbstractLoader);b.load=function(){this._tag||(this._tag=this._createTag(this._item.src)),this._tag.preload="auto",this._tag.load(),this.AbstractLoader_load()},b._createTag=function(){},b._createRequest=function(){this._request=this._preferXHR?new createjs.XHRRequest(this._item):new createjs.MediaTagRequest(this._item,this._tag||this._createTag(),this._tagSrcAttribute)},b._formatResult=function(a){return this._tag.removeEventListener&&this._tag.removeEventListener("canplaythrough",this._loadedHandler),this._tag.onstalled=null,this._preferXHR&&(a.getTag().src=a.getResult(!0)),a.getTag()},createjs.AbstractMediaLoader=createjs.promote(a,"AbstractLoader")}(),this.createjs=this.createjs||{},function(){"use strict";var a=function(a){this._item=a},b=createjs.extend(a,createjs.EventDispatcher);b.load=function(){},b.destroy=function(){},b.cancel=function(){},createjs.AbstractRequest=createjs.promote(a,"EventDispatcher")}(),this.createjs=this.createjs||{},function(){"use strict";function a(a,b,c){this.AbstractRequest_constructor(a),this._tag=b,this._tagSrcAttribute=c,this._loadedHandler=createjs.proxy(this._handleTagComplete,this),this._addedToDOM=!1,this._startTagVisibility=null}var b=createjs.extend(a,createjs.AbstractRequest);b.load=function(){null==this._tag.parentNode&&(window.document.body.appendChild(this._tag),this._addedToDOM=!0),this._tag.onload=createjs.proxy(this._handleTagComplete,this),this._tag.onreadystatechange=createjs.proxy(this._handleReadyStateChange,this);var a=new createjs.Event("initialize");a.loader=this._tag,this.dispatchEvent(a),this._hideTag(),this._tag[this._tagSrcAttribute]=this._item.src},b.destroy=function(){this._clean(),this._tag=null,this.AbstractRequest_destroy()},b._handleReadyStateChange=function(){clearTimeout(this._loadTimeout);var a=this._tag;("loaded"==a.readyState||"complete"==a.readyState)&&this._handleTagComplete()},b._handleTagComplete=function(){this._rawResult=this._tag,this._result=this.resultFormatter&&this.resultFormatter(this)||this._rawResult,this._clean(),this._showTag(),this.dispatchEvent("complete")},b._clean=function(){this._tag.onload=null,this._tag.onreadystatechange=null,this._addedToDOM&&null!=this._tag.parentNode&&this._tag.parentNode.removeChild(this._tag)},b._hideTag=function(){this._startTagVisibility=this._tag.style.visibility,this._tag.style.visibility="hidden"},b._showTag=function(){this._tag.style.visibility=this._startTagVisibility},b._handleStalled=function(){},createjs.TagRequest=createjs.promote(a,"AbstractRequest")}(),this.createjs=this.createjs||{},function(){"use strict";function a(a,b,c){this.AbstractRequest_constructor(a),this._tag=b,this._tagSrcAttribute=c,this._loadedHandler=createjs.proxy(this._handleTagComplete,this)}var b=createjs.extend(a,createjs.TagRequest);b.load=function(){this._tag.onstalled=createjs.proxy(this._handleStalled,this),this._tag.onprogress=createjs.proxy(this._handleProgress,this),this._tag.addEventListener&&this._tag.addEventListener("canplaythrough",this._loadedHandler,!1),this.TagRequest_load()},b._handleReadyStateChange=function(){clearTimeout(this._loadTimeout);var a=this._tag;("loaded"==a.readyState||"complete"==a.readyState)&&this._handleTagComplete()},b._handleStalled=function(){},b._handleProgress=function(a){if(a&&!(a.loaded>0&&0==a.total)){var b=new createjs.ProgressEvent(a.loaded,a.total);this.dispatchEvent(b)}},b._clean=function(){this._tag.removeEventListener&&this._tag.removeEventListener("canplaythrough",this._loadedHandler),this._tag.onstalled=null,this._tag.onprogress=null,this.TagRequest__clean()},createjs.MediaTagRequest=createjs.promote(a,"TagRequest")}(),this.createjs=this.createjs||{},function(){"use strict";function a(a){this.AbstractRequest_constructor(a),this._request=null,this._loadTimeout=null,this._xhrLevel=1,this._response=null,this._rawResponse=null,this._canceled=!1,this._handleLoadStartProxy=createjs.proxy(this._handleLoadStart,this),this._handleProgressProxy=createjs.proxy(this._handleProgress,this),this._handleAbortProxy=createjs.proxy(this._handleAbort,this),this._handleErrorProxy=createjs.proxy(this._handleError,this),this._handleTimeoutProxy=createjs.proxy(this._handleTimeout,this),this._handleLoadProxy=createjs.proxy(this._handleLoad,this),this._handleReadyStateChangeProxy=createjs.proxy(this._handleReadyStateChange,this),!this._createXHR(a)}var b=createjs.extend(a,createjs.AbstractRequest);a.ACTIVEX_VERSIONS=["Msxml2.XMLHTTP.6.0","Msxml2.XMLHTTP.5.0","Msxml2.XMLHTTP.4.0","MSXML2.XMLHTTP.3.0","MSXML2.XMLHTTP","Microsoft.XMLHTTP"],b.getResult=function(a){return a&&this._rawResponse?this._rawResponse:this._response},b.cancel=function(){this.canceled=!0,this._clean(),this._request.abort()},b.load=function(){if(null==this._request)return void this._handleError();this._request.addEventListener("loadstart",this._handleLoadStartProxy,!1),this._request.addEventListener("progress",this._handleProgressProxy,!1),this._request.addEventListener("abort",this._handleAbortProxy,!1),this._request.addEventListener("error",this._handleErrorProxy,!1),this._request.addEventListener("timeout",this._handleTimeoutProxy,!1),this._request.addEventListener("load",this._handleLoadProxy,!1),this._request.addEventListener("readystatechange",this._handleReadyStateChangeProxy,!1),1==this._xhrLevel&&(this._loadTimeout=setTimeout(createjs.proxy(this._handleTimeout,this),this._item.loadTimeout));try{this._item.values&&this._item.method!=createjs.AbstractLoader.GET?this._item.method==createjs.AbstractLoader.POST&&this._request.send(createjs.RequestUtils.formatQueryString(this._item.values)):this._request.send()}catch(a){this.dispatchEvent(new createjs.ErrorEvent("XHR_SEND",null,a))}},b.setResponseType=function(a){this._request.responseType=a},b.getAllResponseHeaders=function(){return this._request.getAllResponseHeaders instanceof Function?this._request.getAllResponseHeaders():null},b.getResponseHeader=function(a){return this._request.getResponseHeader instanceof Function?this._request.getResponseHeader(a):null},b._handleProgress=function(a){if(a&&!(a.loaded>0&&0==a.total)){var b=new createjs.ProgressEvent(a.loaded,a.total);this.dispatchEvent(b)}},b._handleLoadStart=function(){clearTimeout(this._loadTimeout),this.dispatchEvent("loadstart")},b._handleAbort=function(a){this._clean(),this.dispatchEvent(new createjs.ErrorEvent("XHR_ABORTED",null,a))},b._handleError=function(a){this._clean(),this.dispatchEvent(new createjs.ErrorEvent(a.message))},b._handleReadyStateChange=function(){4==this._request.readyState&&this._handleLoad()},b._handleLoad=function(){if(!this.loaded){this.loaded=!0;var a=this._checkError();if(a)return void this._handleError(a);this._response=this._getResponse(),this._clean(),this.dispatchEvent(new createjs.Event("complete"))}},b._handleTimeout=function(a){this._clean(),this.dispatchEvent(new createjs.ErrorEvent("PRELOAD_TIMEOUT",null,a))},b._checkError=function(){var a=parseInt(this._request.status);switch(a){case 404:case 0:return new Error(a)}return null},b._getResponse=function(){if(null!=this._response)return this._response;if(null!=this._request.response)return this._request.response;try{if(null!=this._request.responseText)return this._request.responseText}catch(a){}try{if(null!=this._request.responseXML)return this._request.responseXML}catch(a){}return null},b._createXHR=function(a){var b=createjs.RequestUtils.isCrossDomain(a),c={},d=null;if(window.XMLHttpRequest)d=new XMLHttpRequest,b&&void 0===d.withCredentials&&window.XDomainRequest&&(d=new XDomainRequest);else{for(var e=0,f=s.ACTIVEX_VERSIONS.length;f>e;e++){{s.ACTIVEX_VERSIONS[e]}try{d=new ActiveXObject(axVersions);break}catch(g){}}if(null==d)return!1}a.mimeType&&d.overrideMimeType&&d.overrideMimeType(a.mimeType),this._xhrLevel="string"==typeof d.responseType?2:1;var h=null;if(h=a.method==createjs.AbstractLoader.GET?createjs.RequestUtils.buildPath(a.src,a.values):a.src,d.open(a.method||createjs.AbstractLoader.GET,h,!0),b&&d instanceof XMLHttpRequest&&1==this._xhrLevel&&(c.Origin=location.origin),a.values&&a.method==createjs.AbstractLoader.POST&&(c["Content-Type"]="application/x-www-form-urlencoded"),b||c["X-Requested-With"]||(c["X-Requested-With"]="XMLHttpRequest"),a.headers)for(var i in a.headers)c[i]=a.headers[i];for(i in c)d.setRequestHeader(i,c[i]);return d instanceof XMLHttpRequest&&void 0!==a.withCredentials&&(d.withCredentials=a.withCredentials),this._request=d,!0},b._clean=function(){clearTimeout(this._loadTimeout),this._request.removeEventListener("loadstart",this._handleLoadStartProxy),this._request.removeEventListener("progress",this._handleProgressProxy),this._request.removeEventListener("abort",this._handleAbortProxy),this._request.removeEventListener("error",this._handleErrorProxy),this._request.removeEventListener("timeout",this._handleTimeoutProxy),this._request.removeEventListener("load",this._handleLoadProxy),this._request.removeEventListener("readystatechange",this._handleReadyStateChangeProxy)},b.toString=function(){return"[PreloadJS XHRRequest]"},createjs.XHRRequest=createjs.promote(a,"AbstractRequest")}(),this.createjs=this.createjs||{},function(){"use strict";function a(a,b){this.AbstractMediaLoader_constructor(a,b,createjs.AbstractLoader.SOUND),createjs.RequestUtils.isAudioTag(a)?this._tag=a:createjs.RequestUtils.isAudioTag(a.src)?this._tag=a:createjs.RequestUtils.isAudioTag(a.tag)&&(this._tag=createjs.RequestUtils.isAudioTag(a)?a:a.src),null!=this._tag&&(this._preferXHR=!1)}var b=createjs.extend(a,createjs.AbstractMediaLoader),c=a;c.canLoadItem=function(a){return a.type==createjs.AbstractLoader.SOUND},b._createTag=function(a){var b=document.createElement("audio");return b.autoplay=!1,b.preload="none",b.src=a,b},createjs.SoundLoader=createjs.promote(a,"AbstractMediaLoader")}(),this.createjs=this.createjs||{},function(){"use strict";function a(){throw"Sound cannot be instantiated"}function b(a,b){this.init(a,b)}var c=a;c.INTERRUPT_ANY="any",c.INTERRUPT_EARLY="early",c.INTERRUPT_LATE="late",c.INTERRUPT_NONE="none",c.PLAY_INITED="playInited",c.PLAY_SUCCEEDED="playSucceeded",c.PLAY_INTERRUPTED="playInterrupted",c.PLAY_FINISHED="playFinished",c.PLAY_FAILED="playFailed",c.SUPPORTED_EXTENSIONS=["mp3","ogg","mpeg","wav","m4a","mp4","aiff","wma","mid"],c.EXTENSION_MAP={m4a:"mp4"},c.FILE_PATTERN=/^(?:(\w+:)\/{2}(\w+(?:\.\w+)*\/?))?([/.]*?(?:[^?]+)?\/)?((?:[^/?]+)\.(\w+))(?:\?(\S+)?)?$/,c.defaultInterruptBehavior=c.INTERRUPT_NONE,c.alternateExtensions=[],c.activePlugin=null,c._pluginsRegistered=!1,c._lastID=0,c._masterVolume=1,c._masterMute=!1,c._instances=[],c._idHash={},c._preloadHash={},c.addEventListener=null,c.removeEventListener=null,c.removeAllEventListeners=null,c.dispatchEvent=null,c.hasEventListener=null,c._listeners=null,createjs.EventDispatcher.initialize(c),c.getPreloadHandlers=function(){return{callback:createjs.proxy(c.initLoad,c),types:["sound"],extensions:c.SUPPORTED_EXTENSIONS}},c._handleLoadComplete=function(a){var b=a.target.getItem().src;if(c._preloadHash[b])for(var d=0,e=c._preloadHash[b].length;e>d;d++){var f=c._preloadHash[b][d];if(c._preloadHash[b][d]=!0,c.hasEventListener("fileload")){var a=new createjs.Event("fileload");a.src=f.src,a.id=f.id,a.data=f.data,a.sprite=f.sprite,c.dispatchEvent(a)}}},c._handleLoadError=function(a){var b=a.target.getItem().src;if(c._preloadHash[b])for(var d=0,e=c._preloadHash[b].length;e>d;d++){var f=c._preloadHash[b][d];if(c._preloadHash[b][d]=!1,c.hasEventListener("fileerror")){var a=new createjs.Event("fileerror");a.src=f.src,a.id=f.id,a.data=f.data,a.sprite=f.sprite,c.dispatchEvent(a)}}},c._registerPlugin=function(a){return a.isSupported()?(c.activePlugin=new a,!0):!1},c.registerPlugins=function(a){c._pluginsRegistered=!0;for(var b=0,d=a.length;d>b;b++)if(c._registerPlugin(a[b]))return!0;return!1},c.initializeDefaultPlugins=function(){return null!=c.activePlugin?!0:c._pluginsRegistered?!1:c.registerPlugins([createjs.WebAudioPlugin,createjs.HTMLAudioPlugin])?!0:!1},c.isReady=function(){return null!=c.activePlugin},c.getCapabilities=function(){return null==c.activePlugin?null:c.activePlugin._capabilities},c.getCapability=function(a){return null==c.activePlugin?null:c.activePlugin._capabilities[a]},c.initLoad=function(a){return c._registerSound(a)},c._registerSound=function(a){if(!c.initializeDefaultPlugins())return!1;var d=c._parsePath(a.src);if(null==d)return!1;a.src=d.src,a.type="sound";var e=a.data,f=c.activePlugin.defaultNumChannels||null;if(null!=e&&(isNaN(e.channels)?isNaN(e)||(f=parseInt(e)):f=parseInt(e.channels),e.audioSprite))for(var g,h=e.audioSprite.length;h--;)g=e.audioSprite[h],c._idHash[g.id]={src:a.src,startTime:parseInt(g.startTime),duration:parseInt(g.duration)};null!=a.id&&(c._idHash[a.id]={src:a.src});var i=c.activePlugin.register(a,f);return b.create(a.src,f),null!=e&&isNaN(e)?a.data.channels=f||b.maxPerChannel():a.data=f||b.maxPerChannel(),i.type&&(a.type=i.type),i},c.registerSound=function(a,b,d,e){var f={src:a,id:b,data:d};a instanceof Object&&(e=b,f=a),f=createjs.LoadItem.create(f),null!=e&&(f.src=e+a);var g=c._registerSound(f);if(!g)return!1;if(c._preloadHash[f.src]||(c._preloadHash[f.src]=[]),c._preloadHash[f.src].push(f),1==c._preloadHash[f.src].length)g.on("complete",createjs.proxy(this._handleLoadComplete,this)),g.on("error",createjs.proxy(this._handleLoadError,this)),c.activePlugin.preload(g);else if(1==c._preloadHash[f.src][0])return!0;return f},c.registerSounds=function(a,b){var c=[];a.path&&(b?b+=a.path:b=a.path);for(var d=0,e=a.length;e>d;d++)c[d]=createjs.Sound.registerSound(a[d].src,a[d].id,a[d].data,b);return c},c.registerManifest=function(a,b){try{console.log("createjs.Sound.registerManifest is deprecated, please use createjs.Sound.registerSounds.")}catch(c){}return this.registerSounds(a,b)},c.removeSound=function(a,d){if(null==c.activePlugin)return!1;a instanceof Object&&(a=a.src),a=c._getSrcById(a).src,null!=d&&(a=d+a);var e=c._parsePath(a);if(null==e)return!1;a=e.src;for(var f in c._idHash)c._idHash[f].src==a&&delete c._idHash[f];return b.removeSrc(a),delete c._preloadHash[a],c.activePlugin.removeSound(a),!0},c.removeSounds=function(a,b){var c=[];a.path&&(b?b+=a.path:b=a.path);for(var d=0,e=a.length;e>d;d++)c[d]=createjs.Sound.removeSound(a[d].src,b);return c},c.removeManifest=function(a,b){try{console.log("createjs.Sound.removeManifest is deprecated, please use createjs.Sound.removeSounds.")}catch(d){}return c.removeSounds(a,b)},c.removeAllSounds=function(){c._idHash={},c._preloadHash={},b.removeAll(),c.activePlugin&&c.activePlugin.removeAllSounds()},c.loadComplete=function(a){if(!c.isReady())return!1;var b=c._parsePath(a);return a=b?c._getSrcById(b.src).src:c._getSrcById(a).src,1==c._preloadHash[a][0]},c._parsePath=function(a){"string"!=typeof a&&(a=a.toString());var b=a.match(c.FILE_PATTERN);if(null==b)return!1;for(var d=b[4],e=b[5],f=c.getCapabilities(),g=0;!f[e];)if(e=c.alternateExtensions[g++],g>c.alternateExtensions.length)return null;a=a.replace("."+b[5],"."+e);var h={name:d,src:a,extension:e};return h},c.play=function(a,b,d,e,f,g,h,i,j){b instanceof Object&&(d=b.delay,e=b.offset,f=b.loop,g=b.volume,h=b.pan,i=b.startTime,j=b.duration,b=b.interrupt);var k=c.createInstance(a,i,j),l=c._playInstance(k,b,d,e,f,g,h);return l||k._playFailed(),k},c.createInstance=function(a,d,e){if(!c.initializeDefaultPlugins())return new createjs.DefaultSoundInstance(a,d,e);a=c._getSrcById(a);var f=c._parsePath(a.src),g=null;return null!=f&&null!=f.src?(b.create(f.src),null==d&&(d=a.startTime),g=c.activePlugin.create(f.src,d,e||a.duration)):g=new createjs.DefaultSoundInstance(a,d,e),g.uniqueId=c._lastID++,g},c.setVolume=function(a){if(null==Number(a))return!1;if(a=Math.max(0,Math.min(1,a)),c._masterVolume=a,!this.activePlugin||!this.activePlugin.setVolume||!this.activePlugin.setVolume(a))for(var b=this._instances,d=0,e=b.length;e>d;d++)b[d].setMasterVolume(a)},c.getVolume=function(){return c._masterVolume},c.setMute=function(a){if(null==a)return!1;if(this._masterMute=a,!this.activePlugin||!this.activePlugin.setMute||!this.activePlugin.setMute(a))for(var b=this._instances,c=0,d=b.length;d>c;c++)b[c].setMasterMute(a);return!0},c.getMute=function(){return this._masterMute},c.stop=function(){for(var a=this._instances,b=a.length;b--;)a[b].stop()},c._playInstance=function(a,b,d,e,f,g,h){if(b instanceof Object&&(d=b.delay,e=b.offset,f=b.loop,g=b.volume,h=b.pan,b=b.interrupt),b=b||c.defaultInterruptBehavior,null==d&&(d=0),null==e&&(e=a.getPosition()),null==f&&(f=a.loop),null==g&&(g=a.volume),null==h&&(h=a.pan),0==d){var i=c._beginPlaying(a,b,e,f,g,h);if(!i)return!1}else{var j=setTimeout(function(){c._beginPlaying(a,b,e,f,g,h)},d);a.delayTimeoutId=j}return this._instances.push(a),!0},c._beginPlaying=function(a,c,d,e,f,g){if(!b.add(a,c))return!1;var h=a._beginPlaying(d,e,f,g);if(!h){var i=createjs.indexOf(this._instances,a);return i>-1&&this._instances.splice(i,1),!1}return!0},c._getSrcById=function(a){return c._idHash[a]||{src:a}},c._playFinished=function(a){b.remove(a);var c=createjs.indexOf(this._instances,a);c>-1&&this._instances.splice(c,1)},createjs.Sound=a,b.channels={},b.create=function(a,c){var d=b.get(a);return null==d?(b.channels[a]=new b(a,c),!0):!1},b.removeSrc=function(a){var c=b.get(a);return null==c?!1:(c._removeAll(),delete b.channels[a],!0)},b.removeAll=function(){for(var a in b.channels)b.channels[a]._removeAll();b.channels={}},b.add=function(a,c){var d=b.get(a.src);return null==d?!1:d._add(a,c)},b.remove=function(a){var c=b.get(a.src);return null==c?!1:(c._remove(a),!0)},b.maxPerChannel=function(){return d.maxDefault},b.get=function(a){return b.channels[a]};var d=b.prototype;d.constructor=b,d.src=null,d.max=null,d.maxDefault=100,d.length=0,d.init=function(a,b){this.src=a,this.max=b||this.maxDefault,-1==this.max&&(this.max=this.maxDefault),this._instances=[]},d._get=function(a){return this._instances[a]},d._add=function(a,b){return this._getSlot(b,a)?(this._instances.push(a),this.length++,!0):!1},d._remove=function(a){var b=createjs.indexOf(this._instances,a);return-1==b?!1:(this._instances.splice(b,1),this.length--,!0)},d._removeAll=function(){for(var a=this.length-1;a>=0;a--)this._instances[a].stop()},d._getSlot=function(b){var c,d;if(b!=a.INTERRUPT_NONE&&(d=this._get(0),null==d))return!0;for(var e=0,f=this.max;f>e;e++){if(c=this._get(e),null==c)return!0;if(c.playState==a.PLAY_FINISHED||c.playState==a.PLAY_INTERRUPTED||c.playState==a.PLAY_FAILED){d=c;break}b!=a.INTERRUPT_NONE&&(b==a.INTERRUPT_EARLY&&c.getPosition()<d.getPosition()||b==a.INTERRUPT_LATE&&c.getPosition()>d.getPosition())&&(d=c)}return null!=d?(d._interrupt(),this._remove(d),!0):!1},d.toString=function(){return"[Sound SoundChannel]"}}(),this.createjs=this.createjs||{},function(){"use strict";var a=function(a,b,c,d){this.EventDispatcher_constructor(),this.src=a,this.uniqueId=-1,this.playState=null,this.delayTimeoutId=null,this._startTime=Math.max(0,b||0),this._volume=1,createjs.definePropertySupported&&Object.defineProperty(this,"volume",{get:this.getVolume,set:this.setVolume}),this._pan=0,createjs.definePropertySupported&&Object.defineProperty(this,"pan",{get:this.getPan,set:this.setPan}),this._duration=Math.max(0,c||0),createjs.definePropertySupported&&Object.defineProperty(this,"duration",{get:this.getDuration,set:this.setDuration}),this._playbackResource=null,createjs.definePropertySupported&&Object.defineProperty(this,"playbackResource",{get:this.getPlaybackResource,set:this.setPlaybackResource}),d!==!1&&d!==!0&&this.setPlaybackResource(d),this._position=0,createjs.definePropertySupported&&Object.defineProperty(this,"position",{get:this.getPosition,set:this.setPosition}),this._loop=0,createjs.definePropertySupported&&Object.defineProperty(this,"loop",{get:this.getLoop,set:this.setLoop}),this._muted=!1,createjs.definePropertySupported&&Object.defineProperty(this,"muted",{get:this.getMuted,set:this.setMuted}),this._paused=!1,createjs.definePropertySupported&&Object.defineProperty(this,"paused",{get:this.getPaused,set:this.setPaused})
},b=createjs.extend(a,createjs.EventDispatcher);b.play=function(a,b,c,d,e,f){return this.playState==createjs.Sound.PLAY_SUCCEEDED?(a instanceof Object&&(c=a.offset,d=a.loop,e=a.volume,f=a.pan),null!=c&&this.setPosition(c),null!=d&&this.setLoop(d),null!=e&&this.setVolume(e),null!=f&&this.setPan(f),void(this._paused&&this.setPaused(!1))):(this._cleanUp(),createjs.Sound._playInstance(this,a,b,c,d,e,f),this)},b.pause=function(){return this._paused||this.playState!=createjs.Sound.PLAY_SUCCEEDED?!1:(this.setPaused(!0),!0)},b.resume=function(){return this._paused?(this.setPaused(!1),!0):!1},b.stop=function(){return this._position=0,this._paused=!1,this._handleStop(),this._cleanUp(),this.playState=createjs.Sound.PLAY_FINISHED,this},b.destroy=function(){this._cleanUp(),this.src=null,this.playbackResource=null,this.removeAllEventListeners()},b.toString=function(){return"[AbstractSoundInstance]"},b.getPaused=function(){return this._paused},b.setPaused=function(a){return a!==!0&&a!==!1||this._paused==a||1==a&&this.playState!=createjs.Sound.PLAY_SUCCEEDED?void 0:(this._paused=a,a?this._pause():this._resume(),clearTimeout(this.delayTimeoutId),this)},b.setVolume=function(a){return a==this._volume?this:(this._volume=Math.max(0,Math.min(1,a)),this._muted||this._updateVolume(),this)},b.getVolume=function(){return this._volume},b.setMute=function(a){this.setMuted(a)},b.getMute=function(){return this._muted},b.setMuted=function(a){return a===!0||a===!1?(this._muted=a,this._updateVolume(),this):void 0},b.getMuted=function(){return this._muted},b.setPan=function(a){return a==this._pan?this:(this._pan=Math.max(-1,Math.min(1,a)),this._updatePan(),this)},b.getPan=function(){return this._pan},b.getPosition=function(){return this._paused||this.playState!=createjs.Sound.PLAY_SUCCEEDED?this._position:this._calculateCurrentPosition()},b.setPosition=function(a){return this._position=Math.max(0,a),this.playState==createjs.Sound.PLAY_SUCCEEDED&&this._updatePosition(),this},b.getDuration=function(){return this._duration},b.setDuration=function(a){return a==this._duration?this:(this._duration=Math.max(0,a||0),this._updateDuration(),this)},b.setPlaybackResource=function(a){return this._playbackResource=a,0==this._duration&&this._setDurationFromSource(),this},b.getPlaybackResource=function(){return this._playbackResource},b.getLoop=function(){return this._loop},b.setLoop=function(a){null!=this._playbackResource&&(0!=this._loop&&0==a&&this._removeLooping(a),0==this._loop&&0!=a&&this._addLooping(a)),this._loop=a},b._sendEvent=function(a){var b=new createjs.Event(a);this.dispatchEvent(b)},b._cleanUp=function(){clearTimeout(this.delayTimeoutId),this._handleCleanUp(),this._paused=!1,createjs.Sound._playFinished(this)},b._interrupt=function(){this._cleanUp(),this.playState=createjs.Sound.PLAY_INTERRUPTED,this._sendEvent("interrupted")},b._beginPlaying=function(a,b,c,d){return this.setPosition(a),this.setLoop(b),this.setVolume(c),this.setPan(d),null!=this._playbackResource&&this._position<this._duration?(this._paused=!1,this._handleSoundReady(),this.playState=createjs.Sound.PLAY_SUCCEEDED,this._sendEvent("succeeded"),!0):(this._playFailed(),!1)},b._playFailed=function(){this._cleanUp(),this.playState=createjs.Sound.PLAY_FAILED,this._sendEvent("failed")},b._handleSoundComplete=function(){return this._position=0,0!=this._loop?(this._loop--,this._handleLoop(),void this._sendEvent("loop")):(this._cleanUp(),this.playState=createjs.Sound.PLAY_FINISHED,void this._sendEvent("complete"))},b._handleSoundReady=function(){},b._updateVolume=function(){},b._updatePan=function(){},b._updateDuration=function(){},b._setDurationFromSource=function(){},b._calculateCurrentPosition=function(){},b._updatePosition=function(){},b._removeLooping=function(){},b._addLooping=function(){},b._pause=function(){},b._resume=function(){},b._handleStop=function(){},b._handleCleanUp=function(){},b._handleLoop=function(){},createjs.AbstractSoundInstance=createjs.promote(a,"EventDispatcher"),createjs.DefaultSoundInstance=createjs.AbstractSoundInstance}(),this.createjs=this.createjs||{},function(){"use strict";var a=function(){this._capabilities=null,this._loaders={},this._audioSources={},this._soundInstances={},this._loaderClass,this._soundInstanceClass},b=a.prototype;a._capabilities=null,a.isSupported=function(){return!0},b.register=function(a){if(this._audioSources[a.src]=!0,this._soundInstances[a.src]=[],this._loaders[a.src])return this._loaders[a.src];var b=new this._loaderClass(a);return b.on("complete",createjs.proxy(this._handlePreloadComplete,this)),this._loaders[a.src]=b,b},b.preload=function(a){a.on("error",createjs.proxy(this._handlePreloadError,this)),a.load()},b.isPreloadStarted=function(a){return null!=this._audioSources[a]},b.isPreloadComplete=function(a){return!(null==this._audioSources[a]||1==this._audioSources[a])},b.removeSound=function(a){if(this._soundInstances[a]){for(var b=this._soundInstances[a].length;b--;){var c=this._soundInstances[a][b];c.destroy()}delete this._soundInstances[a],delete this._audioSources[a],this._loaders[a]&&this._loaders[a].destroy(),delete this._loaders[a]}},b.removeAllSounds=function(){for(var a in this._audioSources)this.removeSound(a)},b.create=function(a,b,c){this.isPreloadStarted(a)||this.preload(this.register(a));var d=new this._soundInstanceClass(a,b,c,this._audioSources[a]);return this._soundInstances[a].push(d),d},b.setVolume=function(a){return this._volume=a,this._updateVolume(),!0},b.getVolume=function(){return this._volume},b.setMute=function(){return this._updateVolume(),!0},b.toString=function(){return"[AbstractPlugin]"},b._handlePreloadComplete=function(a){var b=a.target.getItem().src;this._audioSources[b]=a.result;for(var c=0,d=this._soundInstances[b].length;d>c;c++){var e=this._soundInstances[b][c];e.setPlaybackResource(this._audioSources[b])}},b._handlePreloadError=function(){},b._updateVolume=function(){},createjs.AbstractPlugin=a}(),this.createjs=this.createjs||{},function(){"use strict";function a(a){this.AbstractLoader_constructor(a,!0,createjs.AbstractLoader.SOUND)}var b=createjs.extend(a,createjs.AbstractLoader);a.context=null,b.toString=function(){return"[WebAudioLoader]"},b._createRequest=function(){this._request=new createjs.XHRRequest(this._item,!1),this._request.setResponseType("arraybuffer")},b._sendComplete=function(){a.context.decodeAudioData(this._rawResult,createjs.proxy(this._handleAudioDecoded,this),createjs.proxy(this._handleError,this))},b._handleAudioDecoded=function(a){this._result=a,this.AbstractLoader__sendComplete()},createjs.WebAudioLoader=createjs.promote(a,"AbstractLoader")}(),this.createjs=this.createjs||{},function(){"use strict";function a(a,b,d,e){this.AbstractSoundInstance_constructor(a,b,d,e),this.gainNode=c.context.createGain(),this.panNode=c.context.createPanner(),this.panNode.panningModel=c._panningModel,this.panNode.connect(this.gainNode),this.sourceNode=null,this._soundCompleteTimeout=null,this._sourceNodeNext=null,this._playbackStartTime=0,this._endedHandler=createjs.proxy(this._handleSoundComplete,this)}var b=createjs.extend(a,createjs.AbstractSoundInstance),c=a;c.context=null,c.destinationNode=null,c._panningModel="equalpower",b.destroy=function(){this.AbstractSoundInstance_destroy(),this.panNode.disconnect(0),this.panNode=null,this.gainNode.disconnect(0),this.gainNode=null},b.toString=function(){return"[WebAudioSoundInstance]"},b._updatePan=function(){this.panNode.setPosition(this._pan,0,-.5)},b._removeLooping=function(){this._sourceNodeNext=this._cleanUpAudioNode(this._sourceNodeNext)},b._addLooping=function(){this.playState==createjs.Sound.PLAY_SUCCEEDED&&(this._sourceNodeNext=this._createAndPlayAudioNode(this._playbackStartTime,0))},b._setDurationFromSource=function(){this._duration=1e3*this.playbackResource.duration},b._handleCleanUp=function(){this.sourceNode&&this.playState==createjs.Sound.PLAY_SUCCEEDED&&(this.sourceNode=this._cleanUpAudioNode(this.sourceNode),this._sourceNodeNext=this._cleanUpAudioNode(this._sourceNodeNext)),0!=this.gainNode.numberOfOutputs&&this.gainNode.disconnect(0),clearTimeout(this._soundCompleteTimeout),this._playbackStartTime=0},b._cleanUpAudioNode=function(a){return a&&(a.stop(0),a.disconnect(0),a=null),a},b._handleSoundReady=function(){this.gainNode.connect(c.destinationNode);var a=.001*this._duration,b=.001*this._position;this.sourceNode=this._createAndPlayAudioNode(c.context.currentTime-a,b),this._playbackStartTime=this.sourceNode.startTime-b,this._soundCompleteTimeout=setTimeout(this._endedHandler,1e3*(a-b)),0!=this._loop&&(this._sourceNodeNext=this._createAndPlayAudioNode(this._playbackStartTime,0))},b._createAndPlayAudioNode=function(a,b){var d=c.context.createBufferSource();d.buffer=this.playbackResource,d.connect(this.panNode);var e=.001*this._duration;return d.startTime=a+e,d.start(d.startTime,b+.001*this._startTime,e-b),d},b._pause=function(){this._position=1e3*(c.context.currentTime-this._playbackStartTime),this.sourceNode=this._cleanUpAudioNode(this.sourceNode),this._sourceNodeNext=this._cleanUpAudioNode(this._sourceNodeNext),0!=this.gainNode.numberOfOutputs&&this.gainNode.disconnect(0),clearTimeout(this._soundCompleteTimeout)},b._resume=function(){this._handleSoundReady()},b._updateVolume=function(){var a=this._muted?0:this._volume;a!=this.gainNode.gain.value&&(this.gainNode.gain.value=a)},b._calculateCurrentPosition=function(){return 1e3*(c.context.currentTime-this._playbackStartTime)},b._updatePosition=function(){this.sourceNode=this._cleanUpAudioNode(this.sourceNode),this._sourceNodeNext=this._cleanUpAudioNode(this._sourceNodeNext),clearTimeout(this._soundCompleteTimeout),this._paused||this._handleSoundReady()},b._handleLoop=function(){this._cleanUpAudioNode(this.sourceNode),this.sourceNode=this._sourceNodeNext,this._playbackStartTime=this.sourceNode.startTime,this._sourceNodeNext=this._createAndPlayAudioNode(this._playbackStartTime,0),this._soundCompleteTimeout=setTimeout(this._endedHandler,this._duration)},b._updateDuration=function(){this._pause(),this._resume()},createjs.WebAudioSoundInstance=createjs.promote(a,"AbstractSoundInstance")}(),this.createjs=this.createjs||{},function(){"use strict";function a(){this.AbstractPlugin_constructor(),this._panningModel=c._panningModel,this._volume=1,this.context=c.context,this.dynamicsCompressorNode=this.context.createDynamicsCompressor(),this.dynamicsCompressorNode.connect(this.context.destination),this.gainNode=this.context.createGain(),this.gainNode.connect(this.dynamicsCompressorNode),createjs.WebAudioSoundInstance.destinationNode=this.gainNode,this._capabilities=c._capabilities,this._loaderClass=createjs.WebAudioLoader,this._soundInstanceClass=createjs.WebAudioSoundInstance,this._addPropsToClasses()}var b=createjs.extend(a,createjs.AbstractPlugin),c=a;c._capabilities=null,c._panningModel="equalpower",c.context=null,c.isSupported=function(){var a=createjs.BrowserDetect.isIOS||createjs.BrowserDetect.isAndroid||createjs.BrowserDetect.isBlackberry;return"file:"!=location.protocol||a||this._isFileXHRSupported()?(c._generateCapabilities(),null==c.context?!1:!0):!1},c.playEmptySound=function(){var a=c.context.createBufferSource();a.buffer=c.context.createBuffer(1,1,22050),a.connect(c.context.destination),a.start(0,0,0)},c._isFileXHRSupported=function(){var a=!0,b=new XMLHttpRequest;try{b.open("GET","WebAudioPluginTest.fail",!1)}catch(c){return a=!1}b.onerror=function(){a=!1},b.onload=function(){a=404==this.status||200==this.status||0==this.status&&""!=this.response};try{b.send()}catch(c){a=!1}return a},c._generateCapabilities=function(){if(null==c._capabilities){var a=document.createElement("audio");if(null==a.canPlayType)return null;if(null==c.context)if(window.AudioContext)c.context=new AudioContext;else{if(!window.webkitAudioContext)return null;c.context=new webkitAudioContext}c._compatibilitySetUp(),c.playEmptySound(),c._capabilities={panning:!0,volume:!0,tracks:-1};for(var b=createjs.Sound.SUPPORTED_EXTENSIONS,d=createjs.Sound.EXTENSION_MAP,e=0,f=b.length;f>e;e++){var g=b[e],h=d[g]||g;c._capabilities[g]="no"!=a.canPlayType("audio/"+g)&&""!=a.canPlayType("audio/"+g)||"no"!=a.canPlayType("audio/"+h)&&""!=a.canPlayType("audio/"+h)}c.context.destination.numberOfChannels<2&&(c._capabilities.panning=!1)}},c._compatibilitySetUp=function(){if(c._panningModel="equalpower",!c.context.createGain){c.context.createGain=c.context.createGainNode;var a=c.context.createBufferSource();a.__proto__.start=a.__proto__.noteGrainOn,a.__proto__.stop=a.__proto__.noteOff,c._panningModel=0}},b.toString=function(){return"[WebAudioPlugin]"},b._addPropsToClasses=function(){var a=this._soundInstanceClass;a.context=this.context,a.destinationNode=this.gainNode,a._panningModel=this._panningModel,this._loaderClass.context=this.context},b._updateVolume=function(){var a=createjs.Sound._masterMute?0:this._volume;a!=this.gainNode.gain.value&&(this.gainNode.gain.value=a)},createjs.WebAudioPlugin=createjs.promote(a,"AbstractPlugin")}(),this.createjs=this.createjs||{},function(){"use strict";function a(a){this.src=a,this.length=0,this.available=0,this.tags=[],this.duration=0}var b=a.prototype;b.constructor=a;var c=a;c.tags={},c.get=function(b){var d=c.tags[b];return null==d&&(d=c.tags[b]=new a(b)),d},c.remove=function(a){var b=c.tags[a];return null==b?!1:(b.removeAll(),delete c.tags[a],!0)},c.getInstance=function(a){var b=c.tags[a];return null==b?null:b.get()},c.setInstance=function(a,b){var d=c.tags[a];return null==d?null:d.set(b)},c.getDuration=function(a){var b=c.tags[a];return null==b?0:b.getDuration()},b.add=function(a){this.tags.push(a),this.length++,this.available++},b.removeAll=function(){for(var a;this.length--;)a=this.tags[this.length],a.parentNode&&a.parentNode.removeChild(a),delete this.tags[this.length];this.src=null,this.tags.length=0},b.get=function(){if(0==this.tags.length)return null;this.available=this.tags.length;var a=this.tags.pop();return null==a.parentNode&&document.body.appendChild(a),a},b.set=function(a){var b=createjs.indexOf(this.tags,a);-1==b&&this.tags.push(a),this.available=this.tags.length},b.getDuration=function(){return this.duration||(this.duration=1e3*this.tags[this.tags.length-1].duration),this.duration},b.toString=function(){return"[HTMLAudioTagPool]"},createjs.HTMLAudioTagPool=a}(),this.createjs=this.createjs||{},function(){"use strict";function a(a,b,c,d){this.AbstractSoundInstance_constructor(a,b,c,d),this._audioSpriteStopTime=null,this._delayTimeoutId=null,this._endedHandler=createjs.proxy(this._handleSoundComplete,this),this._readyHandler=createjs.proxy(this._handleTagReady,this),this._stalledHandler=createjs.proxy(this.playFailed,this),this._audioSpriteEndHandler=createjs.proxy(this._handleAudioSpriteLoop,this),this._loopHandler=createjs.proxy(this._handleSoundComplete,this),c?this._audioSpriteStopTime=.001*(b+c):this._duration=createjs.HTMLAudioTagPool.getDuration(this.src)}var b=createjs.extend(a,createjs.AbstractSoundInstance);b.setMasterVolume=function(){this._updateVolume()},b.setMasterMute=function(){this._updateVolume()},b.toString=function(){return"[HTMLAudioSoundInstance]"},b._removeLooping=function(){null!=this._playbackResource&&(this._playbackResource.loop=!1,this._playbackResource.removeEventListener(createjs.HTMLAudioPlugin._AUDIO_SEEKED,this._loopHandler,!1))},b._addLooping=function(){null==this._playbackResource||this._audioSpriteStopTime||(this._playbackResource.addEventListener(createjs.HTMLAudioPlugin._AUDIO_SEEKED,this._loopHandler,!1),this._playbackResource.loop=!0)},b._handleCleanUp=function(){var a=this._playbackResource;if(null!=a){a.pause(),a.loop=!1,a.removeEventListener(createjs.HTMLAudioPlugin._AUDIO_ENDED,this._endedHandler,!1),a.removeEventListener(createjs.HTMLAudioPlugin._AUDIO_READY,this._readyHandler,!1),a.removeEventListener(createjs.HTMLAudioPlugin._AUDIO_STALLED,this._stalledHandler,!1),a.removeEventListener(createjs.HTMLAudioPlugin._AUDIO_SEEKED,this._loopHandler,!1),a.removeEventListener(createjs.HTMLAudioPlugin._TIME_UPDATE,this._audioSpriteEndHandler,!1);try{a.currentTime=this._startTime}catch(b){}createjs.HTMLAudioTagPool.setInstance(this.src,a),this._playbackResource=null}},b._beginPlaying=function(a,b,c,d){return this._playbackResource=createjs.HTMLAudioTagPool.getInstance(this.src),this.AbstractSoundInstance__beginPlaying(a,b,c,d)},b._handleSoundReady=function(){if(4!==this._playbackResource.readyState){var a=this._playbackResource;return a.addEventListener(createjs.HTMLAudioPlugin._AUDIO_READY,this._readyHandler,!1),a.addEventListener(createjs.HTMLAudioPlugin._AUDIO_STALLED,this._stalledHandler,!1),a.preload="auto",void a.load()}this._updateVolume(),this._playbackResource.currentTime=.001*(this._startTime+this._position),this._audioSpriteStopTime?this._playbackResource.addEventListener(createjs.HTMLAudioPlugin._TIME_UPDATE,this._audioSpriteEndHandler,!1):(this._playbackResource.addEventListener(createjs.HTMLAudioPlugin._AUDIO_ENDED,this._endedHandler,!1),0!=this._loop&&(this._playbackResource.addEventListener(createjs.HTMLAudioPlugin._AUDIO_SEEKED,this._loopHandler,!1),this._playbackResource.loop=!0)),this._playbackResource.play()},b._handleTagReady=function(){this._playbackResource.removeEventListener(createjs.HTMLAudioPlugin._AUDIO_READY,this._readyHandler,!1),this._playbackResource.removeEventListener(createjs.HTMLAudioPlugin._AUDIO_STALLED,this._stalledHandler,!1),this._handleSoundReady()},b._pause=function(){this._playbackResource.pause()},b._resume=function(){this._playbackResource.play()},b._updateVolume=function(){if(null!=this._playbackResource){var a=this._muted||createjs.Sound._masterMute?0:this._volume*createjs.Sound._masterVolume;a!=this._playbackResource.volume&&(this._playbackResource.volume=a)}},b._calculateCurrentPosition=function(){return 1e3*this._playbackResource.currentTime-this._startTime},b._updatePosition=function(){this._playbackResource.removeEventListener(createjs.HTMLAudioPlugin._AUDIO_SEEKED,this._loopHandler,!1),this._playbackResource.addEventListener(createjs.HTMLAudioPlugin._AUDIO_SEEKED,this._handleSetPositionSeek,!1);try{this._playbackResource.currentTime=.001*(this._position+this._startTime)}catch(a){this._handleSetPositionSeek(null)}},b._handleSetPositionSeek=function(){null!=this._playbackResource&&(this._playbackResource.removeEventListener(createjs.HTMLAudioPlugin._AUDIO_SEEKED,this._handleSetPositionSeek,!1),this._playbackResource.addEventListener(createjs.HTMLAudioPlugin._AUDIO_SEEKED,this._loopHandler,!1))},b._handleAudioSpriteLoop=function(){this._playbackResource.currentTime<=this._audioSpriteStopTime||(this._playbackResource.pause(),0==this._loop?this._handleSoundComplete(null):(this._position=0,this._loop--,this._playbackResource.currentTime=.001*this._startTime,this._paused||this._playbackResource.play(),this._sendEvent("loop")))},b._handleLoop=function(){0==this._loop&&(this._playbackResource.loop=!1,this._playbackResource.removeEventListener(createjs.HTMLAudioPlugin._AUDIO_SEEKED,this._loopHandler,!1))},b._updateDuration=function(){this._audioSpriteStopTime=.001*(startTime+duration),this.playState==createjs.Sound.PLAY_SUCCEEDED&&(this._playbackResource.removeEventListener(createjs.HTMLAudioPlugin._AUDIO_ENDED,this._endedHandler,!1),this._playbackResource.addEventListener(createjs.HTMLAudioPlugin._TIME_UPDATE,this._audioSpriteEndHandler,!1))},createjs.HTMLAudioSoundInstance=createjs.promote(a,"AbstractSoundInstance")}(),this.createjs=this.createjs||{},function(){"use strict";function a(){this.AbstractPlugin_constructor(),this.defaultNumChannels=2,this._capabilities=c._capabilities,this._loaderClass=createjs.SoundLoader,this._soundInstanceClass=createjs.HTMLAudioSoundInstance}var b=createjs.extend(a,createjs.AbstractPlugin),c=a;c.MAX_INSTANCES=30,c._AUDIO_READY="canplaythrough",c._AUDIO_ENDED="ended",c._AUDIO_SEEKED="seeked",c._AUDIO_STALLED="stalled",c._TIME_UPDATE="timeupdate",c._capabilities=null,c.enableIOS=!1,c.isSupported=function(){return c._generateCapabilities(),null==c._capabilities?!1:!0},c._generateCapabilities=function(){if(null==c._capabilities){var a=document.createElement("audio");if(null==a.canPlayType)return null;c._capabilities={panning:!0,volume:!0,tracks:-1};for(var b=createjs.Sound.SUPPORTED_EXTENSIONS,d=createjs.Sound.EXTENSION_MAP,e=0,f=b.length;f>e;e++){var g=b[e],h=d[g]||g;c._capabilities[g]="no"!=a.canPlayType("audio/"+g)&&""!=a.canPlayType("audio/"+g)||"no"!=a.canPlayType("audio/"+h)&&""!=a.canPlayType("audio/"+h)}}},b.register=function(a,b){for(var c=createjs.HTMLAudioTagPool.get(a.src),d=null,e=0;b>e;e++)d=this._createTag(a.src),c.add(d);var f=this.AbstractPlugin_register(a,b);return f.setTag(d),f},b.removeSound=function(a){this.AbstractPlugin_removeSound(a),createjs.HTMLAudioTagPool.remove(a)},b.create=function(a,b,c){var d=this.AbstractPlugin_create(a,b,c);return d.setPlaybackResource(null),d},b.toString=function(){return"[HTMLAudioPlugin]"},b.setVolume=b.getVolume=b.setMute=null,b._createTag=function(a){var b=document.createElement("audio");return b.autoplay=!1,b.preload="none",b.src=a,b},createjs.HTMLAudioPlugin=createjs.promote(a,"AbstractPlugin")}();

H5P.SoundJS = this.createjs.Sound;

this.createjs = old || this.createjs;
;H5P.SingleChoiceSet = H5P.SingleChoiceSet || {};

H5P.SingleChoiceSet.SoundEffects = (function ($) {
  var isDefined = false;

  SoundEffects = {
    types: [
      'positive-short',
      'negative-short'
    ],
    sounds: [],
    libraryPath: undefined,
    muted: false
  };

  /**
   * Setup defined sounds
   *
   * @return {boolean} True if setup was successfull, otherwise false
   */
  SoundEffects.setup = function () {
    if (isDefined || !H5P.SoundJS.initializeDefaultPlugins()) {
      return false;
    }

    SoundEffects.libraryPath = H5P.getLibraryPath('H5P.SingleChoiceSet-1.3');
    H5P.SoundJS.alternateExtensions = ['mp3'];
    for (var i = 0; i < SoundEffects.types.length; i++) {
      var type = SoundEffects.types[i];
      H5P.SoundJS.registerSound(SoundEffects.libraryPath + '/sounds/' + type + '.ogg', type);
    }
    isDefined = true;

    return true;
  };

  /**
   * Play a sound
   *
   * @param  {string} type  Name of the sound as defined in [SoundEffects.types]{@link H5P.SoundEffects.SoundEffects#types}
   * @param  {number} delay Delay in milliseconds
   */
  SoundEffects.play = function (type, delay) {
    if (SoundEffects.muted === false) {
      H5P.SoundJS.play(type, H5P.SoundJS.INTERRUPT_NONE, (delay || 0));
    }
  };

  /**
   * Mute. Subsequent invocations of SoundEffects.play() will not make any sounds beeing played.
   */
  SoundEffects.mute = function () {
    SoundEffects.muted = true;
  };

  /**
   * Unmute
   */
  SoundEffects.unmute = function () {
    SoundEffects.muted = false;
  };

  return SoundEffects;
})(H5P.jQuery);
;var H5P = H5P || {};
H5P.SingleChoiceSet = H5P.SingleChoiceSet || {};

/**
 * SingleChoiceEventEmitter makes it possible for other classes to
 * trigger and listen to events
 */
H5P.SingleChoiceSet.EventEmitter = (function () {

  /**
   * @constructor
   */
  function EventEmitter() {
    this.listeners = {};
  }

  /**
   * EventEmitter.prototype.on - Register a listener
   *
   * @param  {string} type        The name of the event
   * @param  {function} listener  The function to call when event is triggered
   */
  EventEmitter.prototype.on = function (type, listener, self) {
    if (typeof listener === 'function') {
      if (this.listeners[type] === undefined) {
        this.listeners[type] = [];
      }
      this.listeners[type].push({
        function: listener,
        self: self
      });
    }
  };


  /**
   * EventEmitter.prototype.off - Unregister a listener
   *
   * @param  {string} type        The name of the event
   * @param  {function} listener  The function to unregister
   */
  EventEmitter.prototype.off = function (type, listener) {
    if (this.listeners[type] !== undefined) {
      var removeIndex = listeners[type].indexOf(listener);
      if (removeIndex) {
        listeners[type].splice(removeIndex, 1);
      }
    }
  };

  /**
   * EventEmitter.prototype.trigger - Trigger an event
   *
   * @param  {string} type  The name of the event
   * @param  {object} event Object data
   */
  EventEmitter.prototype.trigger = function (type, event) {
    if (event === null) {
      event = {};
    }

    if (this.listeners[type] !== undefined) {
      for (var i = 0; i < this.listeners[type].length; i++) {
        var listener = this.listeners[type][i];
        listener.function.apply(listener.self, [event]);
      }
    }
  };

  return EventEmitter;
})();
;var H5P = H5P || {};
H5P.SingleChoiceSet = H5P.SingleChoiceSet || {};
/**
 * SingleChoiceResultSlide - Represents the result slide
 */
H5P.SingleChoiceSet.ResultSlide = (function ($, EventEmitter) {

  /**
  * @constructor
  * @param {number} maxscore Max score
  */
  function ResultSlide(maxscore) {
    EventEmitter.call(this);
    this.maxscore = maxscore;
    var self = this;
    this.$feedbackContainer = $('<div>', {
     'class': 'h5p-sc-feedback-container'
    });

    this.$buttonContainer = $('<div/>', {
      'class': 'h5p-sc-button-container'
    });

    var $resultContainer = $('<div/>', {
      'class': 'h5p-sc-result-container'
    }).append(this.$feedbackContainer)
      .append(this.$buttonContainer);

    this.$resultSlide = $('<div>', {
      'class': 'h5p-sc-slide h5p-sc-set-results',
      'css': {left: (maxscore * 100) + '%'}
    }).append($resultContainer);
  }
  ResultSlide.prototype = Object.create(EventEmitter.prototype);
  ResultSlide.prototype.constructor = ResultSlide;


  /**
   * Append the resultslide to a container
   *
   * @param  {domElement} $container The container
   * @return {domElement}            This dom element
   */
  ResultSlide.prototype.appendTo = function ($container) {
    this.$resultSlide.appendTo($container);
    return this.$resultSlide;
  };

  return ResultSlide;

})(H5P.jQuery, H5P.SingleChoiceSet.EventEmitter);
;var H5P = H5P || {};
H5P.SingleChoiceSet = H5P.SingleChoiceSet || {};

H5P.SingleChoiceSet.SolutionView = (function ($) {
  /**
  * Constructor function.
  */
  function SolutionView (choices, l10n){
    var self = this;
    this.choices = choices;

    this.$solutionView = $('<div>', {
      'class': 'h5p-sc-solution-view'
    });

    self.populate();  // Brought it before 
    
    
    // Add header
    this.$header = $('<div>', {
      'class': 'h5p-sc-solution-view-header'
    }).appendTo(this.$solutionView);

    // Close solution view button
    $('<button>', {
      'class': 'h5p-joubelui-button h5p-sc-close-solution-view',
      'html':window.parent.Drupal.t("LBL212"),
      'click': function () {
        self.hide();
      }
    }).appendTo(this.$header);
    
//    this.$header.append('<div class="h5p-sc-solution-view-title">' + l10n.solutionViewTitle + '</div>'); //not required

//    self.populate();
  }
	
	
	

  /**
   * Will append the solution view to a container DOM
   * @param  {jQuery} $container The DOM object to append to
   */
  SolutionView.prototype.appendTo = function ($container) {
    this.$solutionView.appendTo($container);
  };

  /**
   * Shows the solution view
   */
  SolutionView.prototype.show = function () {
    var self = this;
    self.$solutionView.addClass('visible');

    $(document).on('keyup.solutionview', function (event) {
      if (event.keyCode === 27) { // Escape
        self.hide();
        $(document).off('keyup.solutionview');
      }
    });
  };

  /**
   * Hides the solution view
   */
  SolutionView.prototype.hide = function () {
    this.$solutionView.removeClass('visible');
  };

  /**
   * Populates the solution view
   */
  SolutionView.prototype.populate = function () {
    var self = this;
    self.$choices = $('<div>', {
      'class': 'h5p-sc-solution-choices'
    });
    this.choices.forEach(function (choice) {
      if (choice.question && choice.answers && choice.answers.length !== 0) {
        self.$choices.append($('<div>', {
          'class': 'h5p-sc-solution-question',
          html: choice.question
        }));
        self.$choices.append($('<div>', {
          'class': 'h5p-sc-solution-answer',
          html: choice.answers[0]
        }));
      }
    });
    self.$choices.appendTo(this.$solutionView);
  };

  return SolutionView;
})(H5P.jQuery);
;var H5P = H5P || {};
H5P.SingleChoiceSet = H5P.SingleChoiceSet || {};

H5P.SingleChoiceSet.Alternative = (function ($, EventEmitter) {

  /**
  * @constructor
  *
  * @param {object} options Options for the alternative
  */
  function Alternative(options){
    EventEmitter.call(this);
    var self = this;

    this.options = options;

    var triggerAlternativeSelected = function () {
      self.trigger('alternative-selected', {
        correct: self.options.correct,
        $element: self.$alternative
      });
    };


    this.$alternative = $('<li>', {
      'class': 'h5p-sc-alternative h5p-sc-is-' + (this.options.correct ? 'correct' : 'wrong'),
      tabindex: 1,
      click: triggerAlternativeSelected,
      keypress: function (event) {
        // If enter or space has been pushed
        if(event.which === 13 || event.which === 32) {
          triggerAlternativeSelected();
        }
      }
    });

    this.$alternative.append($('<div>', {
      'class': 'h5p-sc-progressbar'
    }));

    this.$alternative.append($('<div>', {
      'class': 'h5p-sc-label',
      'html': this.options.text
    }));

    this.$alternative.append($('<div>', {
      'class': 'h5p-sc-status'
    }));

  }
  Alternative.prototype = Object.create(EventEmitter.prototype);
  Alternative.prototype.constructor = Alternative;

  /**
   * Is this alternative the correct one?
   *
   * @return {boolean}  Correct or not?
   */
  Alternative.prototype.isCorrect = function () {
    return this.options.correct;
  };


  /**
   * Append the alternative to a DOM container
   *
   * @param  {domElement} $container The Dom element to append to
   * @return {domElement}            This dom element
   */
  Alternative.prototype.appendTo = function ($container) {
    var self = this;
    $container.append(this.$alternative);
    return this.$alternative;
  };

  return Alternative;

})(H5P.jQuery, H5P.SingleChoiceSet.EventEmitter);
;var H5P = H5P || {};
H5P.SingleChoiceSet = H5P.SingleChoiceSet || {};

H5P.SingleChoiceSet.SingleChoice = (function ($, EventEmitter, Alternative, SoundEffects) {
  /**
   * Constructor function.
   */
  function SingleChoice(options, index) {
    EventEmitter.call(this);
    // Extend defaults with provided options
    this.options = $.extend(true, {}, {
      question: '',
      answers: []
    }, options);
    // Keep provided id.
    this.index = index;

    for (var i = 0; i < this.options.answers.length; i++) {
      this.options.answers[i] = {text: this.options.answers[i], correct: i===0};
    }
    // Randomize alternatives
    this.options.answers = H5P.shuffleArray(this.options.answers);
  }
  SingleChoice.prototype = Object.create(EventEmitter.prototype);
  SingleChoice.prototype.constructor = SingleChoice;

  /**
   * appendTo function invoked to append SingleChoice to container
   *
   * @param {jQuery} $container
   */
   SingleChoice.prototype.appendTo = function ($container, current) {
    var self = this;
    this.$container = $container;

    this.$choice = $('<div>', {
      'class': 'h5p-sc-slide h5p-sc' + (current ? ' h5p-sc-current-slide' : ''),
      css: {'left': (self.index*100) + '%'}
    });

    this.$choice.append($('<div>', {
      'class': 'h5p-sc-question',
      'html': this.options.question
    }));

    var $alternatives = $('<ul>', {
      'class': 'h5p-sc-alternatives'
    });


    /**
     * Handles click on an alternative
     */
    var handleAlternativeSelected = function (data) {
      if (data.$element.parent().hasClass('h5p-sc-selected')) {
        return;
      }
      // Can't play it after the transition end is received, since this is not
      // accepted on iPad. Therefore we are playing it here with a delay instead
      SoundEffects.play(data.correct ? 'positive-short' : 'negative-short', 700);

      H5P.Transition.onTransitionEnd(data.$element.find('.h5p-sc-progressbar'), function () {
        data.$element.addClass('h5p-sc-drummed');
        self.showResult(data.correct);
      }, 700);

      data.$element.addClass('h5p-sc-selected').parent().addClass('h5p-sc-selected');
    };

    for (var i = 0; i < this.options.answers.length; i++) {
      var alternative = new Alternative(this.options.answers[i]);
      alternative.appendTo($alternatives);
      alternative.on('alternative-selected', handleAlternativeSelected);
    }
    this.$choice.append($alternatives);
    $container.append(this.$choice);
    return this.$choice;
  };

  /**
   * Reveals the result for a question
   * @param  {boolean} correct True uf answer was correct, otherwise false
   */
  SingleChoice.prototype.showResult = function (correct) {
    var self = this;

    var $correctAlternative = self.$choice.find('.h5p-sc-is-correct');

    H5P.Transition.onTransitionEnd($correctAlternative, function () {
      self.trigger('finished', {correct: correct});
    }, 600);

    // Reveal corrects and wrong
    self.$choice.find('.h5p-sc-is-wrong').addClass('h5p-sc-reveal-wrong');
    $correctAlternative.addClass('h5p-sc-reveal-correct');
  };

  return SingleChoice;

})(H5P.jQuery, H5P.SingleChoiceSet.EventEmitter, H5P.SingleChoiceSet.Alternative, H5P.SingleChoiceSet.SoundEffects);
;var H5P = H5P || {};

H5P.SingleChoiceSet = (function ($, Question, SingleChoice, SolutionView, ResultSlide, SoundEffects) {
  /**
   * @constructor
   * @extends Question
   * @param {object} options Options for single choice set
   * @param {string} contentId H5P instance id
   * @param {Object} contentData H5P instance data
   */
  function SingleChoiceSet(options, contentId, contentData) {
    var self = this;
	
	//h5pcustomize
	var showSol = false;
	var retry = false;
	try{
	if(contentData != undefined && contentData.parent != undefined && contentData.parent.override != undefined && contentData.parent.override.overrideButtons !=undefined &&  contentData.parent.override.overrideButtons == "true" || contentData.parent.override.overrideButtons == true)
	{
	
		if(contentData.parent.override.overrideShowSolutionButton == "true" || contentData.parent.override.overrideShowSolutionButton == true)
		{
	
			showSol =true;
		}
		if(contentData.parent.override.overrideRetry == "true" || contentData.parent.override.overrideRetry == true)
		{
	
			retry =true;
		}
			
	} 
	}catch(e){}
	/*for(var key in contentData){
		console.log(key+"::"+contentData[key]);
		for (var key1 in contentData[key])
		console.log(key1+"::"+contentData[key][key1]);
	}*/
    // Extend defaults with provided options
    this.contentId = contentId;
    Question.call(this, 'single-choice-set');
    this.options = $.extend(true, {}, {
      choices: [],
      behaviour: {
        timeoutCorrect: 50,
        timeoutWrong: 100,
        soundEffectsEnabled: true,
        enableRetry: retry,
        enableSolutionsButton: showSol
      }
    }, options);
    if (contentData && contentData.previousState !== undefined) {
    	//if(retry == false) //fix for 84393
    		{
    			this.currentIndex = contentData.previousState.progress;
    			this.results = contentData.previousState.answers;
    		}
    }
    this.currentIndex = this.currentIndex || 0;
    this.results = this.results || {
      corrects: 0,
      wrongs: 0
    };
    this.l10n = H5P.jQuery.extend({
    	
      resultSlideTitle: window.parent.Drupal.t("LBL3202") + " :numcorrect "+window.parent.Drupal.t("LBL981")+" :maxscore "+window.parent.Drupal.t("LBL714"), //'You got :numcorrect of :maxscore correct',
      showSolutionButtonLabel: window.parent.Drupal.t("LBL3099"),
      retryButtonLabel: window.parent.Drupal.t("LBL3200"),
      solutionViewTitle: 'Solution'
    }, {});
    // h5pcustomize options.l10n !== undefined ? options.l10n : {});

    this.$container = $('<div>', {
      'class': 'h5p-sc-set-wrapper'
    });

    this.$slides = [];
    // An array containing the SingleChoice instances
    this.choices = [];

    this.solutionView = new SolutionView(this.options.choices, this.l10n);

    this.$choices = $('<div>', {
      'class': 'h5p-sc-set h5p-sc-animate'
    });
    this.$progressbar = $('<div>', {
      'class': 'h5p-sc-set-progress'
      
    });
    this.$progressCompleted = $('<div>', {
      'class': 'h5p-sc-completed'
    }).appendTo(this.$progressbar);

    // Validate "slides", reverse traversal since we remove entries as we go
    for (var slideIndex = this.options.choices.length - 1; slideIndex >= 0; slideIndex--) {

      // Prune invalid slide
      if (!this.options.choices[slideIndex].answers) {
        this.options.choices.splice(slideIndex, 1);
      }
    }

    for (var i = 0; i < this.options.choices.length; i++) {
      var choice = new SingleChoice(this.options.choices[i], i);
      choice.on('finished', this.handleQuestionFinished, this);
      choice.appendTo(this.$choices, (i === this.currentIndex));
      this.choices.push(choice);
      this.$slides.push(choice.$choice);
    }

    this.resultSlide = new ResultSlide(this.options.choices.length);
    this.resultSlide.appendTo(this.$choices);
    this.resultSlide.on('retry', this.resetTask, this);
    this.resultSlide.on('view-solution', this.handleViewSolution, this);
    this.$slides.push(this.resultSlide.$resultSlide);
    this.on('resize', this.resize, this);

    // Use the correct starting slide
    this.recklessJump(this.currentIndex);
      
    if (this.options.choices.length === this.currentIndex) {
      // Make sure results slide is displayed
      this.resultSlide.$resultSlide.addClass('h5p-sc-current-slide');
      this.setScore(this.results.corrects, true, 0);

      
    }
    setTimeout(function () {
      SoundEffects.setup();
    },1);

    SoundEffects.muted = (this.options.behaviour.soundEffectsEnabled === false);

    var hideButtons = [];

    /**
     * Keeps track of buttons that will be hidden
     * @type {Array}
     */
    self.buttonsToBeHidden = [];

    /**
     * Override Question's hideButton function
     * to be able to hide buttons after delay
     *
     * @override
     * @param {string} id
     */
    this.superHideButton = self.hideButton;
    this.hideButton = (function () {
      return function (id) {

        if (!self.scoreTimeout) {
          return self.superHideButton(id);
        }

        self.buttonsToBeHidden.push(id);
        return this;
      };
    })();
  }

  SingleChoiceSet.prototype = Object.create(Question.prototype);
  SingleChoiceSet.prototype.constructor = SingleChoiceSet;

  /**
   * Handler invoked when question is done
   *
   * @param  {object} data An object containing a single boolean property: "correct".
   */
  SingleChoiceSet.prototype.handleQuestionFinished = function (data) {
    var self = this;
    self.triggerXAPI('interacted');
    if (data.correct) {
      self.results.corrects++;
    }
    else {
      self.results.wrongs++;
    }

    if (self.currentIndex+1 >= self.options.choices.length) {
      self.setScore(self.results.corrects);
    }

    var letsMove = function () {
      // Handle impatient users
      self.$container.off('click.impatient keydown.impatient');
      clearTimeout(timeout);
      self.move(self.currentIndex+1);
    };

    var timeout = setTimeout(function () {
      letsMove();
    }, data.correct ? self.options.behaviour.timeoutCorrect : self.options.behaviour.timeoutWrong);

    self.$container.on('click.impatient', function () {
      letsMove();
    });
    self.$container.on('keydown.impatient', function (event) {
      // If return, space or right arrow
      if(event.which === 13 || event.which === 32 || event.which === 39) {
        letsMove();
      }
    });
  };

  /**
   * Handles buttons that are queued for hiding
   */
  SingleChoiceSet.prototype.handleQueuedButtonChanges = function () {
    var self = this;

    if (self.buttonsToBeHidden.length) {
      self.buttonsToBeHidden.forEach(function (id) {
        self.superHideButton(id);
      });
    }
    self.buttonsToBeHidden = [];
  };

  /**
   * Set score and feedback
   *
   * @params {Number} score Number of correct answers
   */
  SingleChoiceSet.prototype.setScore = function (score, noXAPI, timeout) {
    var self = this;

    // Find last selected alternative, and determine timeout before solution slide shows
    if (!self.choices.length) {
      return;
    }
    var lastSelected = self.choices[self.choices.length - 1]
      .$choice
      .find('.h5p-sc-alternative.h5p-sc-selected');
    var timeout = (timeout !== undefined) ? timeout : (lastSelected.is('.h5p-sc-is-correct') ?
      this.options.behaviour.timeoutCorrect :
      this.options.behaviour.timeoutWrong);

    /**
     * Show feedback and buttons on result slide
     */
    var showFeedback = function () {
        
      self.setFeedback(self.l10n.resultSlideTitle
          .replace(':numcorrect', score)
          .replace(':maxscore', self.options.choices.length),
        score, self.options.choices.length);
      if (score === self.options.choices.length) {
        self.hideButton('try-again');
        self.hideButton('show-solution');
      } else {
        self.showButton('try-again');
        self.showButton('show-solution');
      }
      self.handleQueuedButtonChanges();
      self.scoreTimeout = undefined;
      if (!noXAPI) {
        self.triggerXAPIScored(score, self.options.choices.length, 'completed');
      }

      self.trigger('resize');
      //h5pcustomize
      
      self.$container.parent().css("height","200px"); //h5p-question-content
      var actualHeight = self.$container.parent().parent().parent().parent().css("height");
      self.$container.parent().parent().parent().parent().attr("actualHeight",actualHeight);
      self.$container.parent().parent().parent().parent().css("height","200px"); //h5p-question
	  
	  
      
        
        //H5P.jQuery(".h5p-sc-set-wrapper").scrollTop(0);
        if(self.$container != null)
        	self.$container.parent().find(".h5p-sc-set-wrapper").scrollTop(0);

	
    //    self.$container.find(".h5p-sc-button-container").css("position","absolute");
     //   self.$container.find(".h5p-sc-button-container").css("bottom","0px");
        
        
        //self.$container.find(".h5p-sc-button-container").css("top","135px");
		/*if(window.parent.Drupal.settings.user.language !== undefined && $.inArray(window.parent.Drupal.settings.user.language, ["ru"]) != -1) {
        	self.$container.find(".h5p-sc-button-container").css("margin-left","35%");
        }else
        {
        	self.$container.find(".h5p-sc-button-container").css("margin-left","50%");
        }*/
        

       	
        $(".h5p-sc-set-progress").css("display","none");
    };

    /**
     * Wait for result slide animation
     */
    self.scoreTimeout = setTimeout(function () {
      showFeedback();
    }, (timeout));

    /**
     * Listen for impatient clicks.
     * On impatient clicks clear timeout and immediately show feedback.
     */
    self.$container.on('click.impatient', function () {
      clearTimeout(self.scoreTimeout);
      showFeedback();
    });
  };

  /**
   * Handler invoked when view solution is selected
   */
  SingleChoiceSet.prototype.handleViewSolution = function () {
    this.solutionView.show();
  };

  /**
   * Register DOM elements before they are attached.
   * Called from H5P.Question.
   */
  SingleChoiceSet.prototype.registerDomElements = function () {
    // Register task content area.
    this.setContent(this.createQuestion());

    // Register buttons with question.
    this.addButtons();

    // Insert feedback and buttons section on the result slide
    this.insertSectionAtElement('feedback', this.resultSlide.$feedbackContainer);
    this.insertSectionAtElement('buttons', this.resultSlide.$buttonContainer);

    // Question is finished
    if (this.options.choices.length === this.currentIndex) {
      this.trigger('question-finished');
    }
  };

  /**
   * Add Buttons to question.
   */
  SingleChoiceSet.prototype.addButtons = function () {
    var self = this;
    if (this.options.behaviour.enableRetry) {
      this.addButton('try-again', this.l10n.retryButtonLabel, function () {
        //h5pcustomize - display progress  when click on try again
        $(".h5p-sc-set-progress").css("display","block");
        self.resetTask();
        
      }, self.results.corrects !== self.options.choices.length);
    }

    if (this.options.behaviour.enableSolutionsButton) {
      this.addButton('show-solution', this.l10n.showSolutionButtonLabel, function () {
        self.showSolutions();
      }, self.results.corrects !== self.options.choices.length);
    }
  };

  /**
   * Create main content
   */
  SingleChoiceSet.prototype.createQuestion = function () {
    var self = this;

    self.$container.append(self.$choices);
    self.$container.append(self.$progressbar);

    if (self.options.behaviour.soundEffectsEnabled) {
      self.$container.append($('<div>', {
        'class': 'h5p-sc-sound-control',
        'click': function () {
          SoundEffects.muted = !SoundEffects.muted;
          $(this).toggleClass('muted', SoundEffects.muted);
        }
      }));
    }

    // Append solution view - hidden by default:
    self.solutionView.appendTo(self.$container);

    self.resize();

    // Hide all other slides than the current one:
    self.$container.addClass('initialized');

    return self.$container;
  };

  /**
   * Resize if something outside resizes
   */
  SingleChoiceSet.prototype.resize = function () {
    var self = this;
    var maxHeight = 0;
    self.choices.forEach(function (choice) {
      var choiceHeight = choice.$choice.outerHeight();
      maxHeight = choiceHeight > maxHeight ? choiceHeight : maxHeight;
    });

    // Set minimum height for choices
    self.$choices.css({minHeight: maxHeight + 'px'});
    //h5pcustomize
    self.$container.parent().find(".h5p-sc-set").css("min-height","200px"); //h5p-question-content
  };

  /**
   * Will jump to the given slide without any though to animations,
   * current slide etc.
   *
   * @public
   */
  SingleChoiceSet.prototype.recklessJump = function (index) {
    var tX = 'translateX('+(-index*100)+'%)';
    this.$choices.css({'-webkit-transform':tX,'-moz-transform':tX,'-ms-transform':tX,'transform':tX});
    this.$progressCompleted.css({width:((index+1)/(this.options.choices.length+1))*100 + '%'});
    //always scroll to top when view question - #84481
    this.$choices.parent().parent().scrollTop(0);    
  };

  /**
   * Move to slide n
   * @param  {number} index The slide number    to move to
   */
  SingleChoiceSet.prototype.move = function (index) {
    if (index === this.currentIndex) {
      return;
    }

    var $previousSlide = this.$slides[this.currentIndex];

    H5P.Transition.onTransitionEnd(this.$choices, function () {
      $previousSlide.removeClass('h5p-sc-current-slide');
    }, 600);

    this.$slides[index].addClass('h5p-sc-current-slide');
    this.recklessJump(index);

    this.currentIndex = index;
  };

  /**
   * The following functions implements the CP and IV - Contracts v 1.0 documented here:
   * http://h5p.org/node/1009
   */
  SingleChoiceSet.prototype.getScore = function () {
    return this.results.corrects;
  };
  SingleChoiceSet.prototype.getMaxScore = function () {
   return this.options.choices.length;
  };
  SingleChoiceSet.prototype.getAnswerGiven = function () {
    return (this.results.corrects + this.results.wrongs) > 0;
  };
  SingleChoiceSet.prototype.getTitle = function() {
    return H5P.createTitle(this.options.choices[0].question);
  };
  SingleChoiceSet.prototype.showSolutions = function () {
    this.handleViewSolution();
  };
  /**
   * Reset all answers. This is equal to refreshing the quiz
   */
  SingleChoiceSet.prototype.resetTask = function () {
    var self = this;
    
    //h5pcustomize
 	var actualHeight = self.$container.parent().parent().parent().parent().attr("actualHeight");
 	if(actualHeight != null && actualHeight != undefined)
 	{
 		self.$container.parent().parent().parent().parent().css("height",actualHeight);
 		self.$container.parent().css("height",actualHeight);
 	}
 		
    // Close solution view if visible:
    this.solutionView.hide();

    // Reset the user's answers
    var classes = ['h5p-sc-reveal-wrong', 'h5p-sc-reveal-correct', 'h5p-sc-selected', 'h5p-sc-drummed', 'h5p-sc-correct-answer'];
    for (var i = 0; i < classes.length; i++) {
      this.$choices.find('.' + classes[i]).removeClass(classes[i]);
    }
    this.results = {
      corrects: 0,
      wrongs: 0
    };

    this.move(0);

    // Wait for transition, then remove feedback.
    H5P.Transition.onTransitionEnd(this.$choices, function () {
      self.setFeedback();
    }, 600);
  };

  /**
   * Clever comment.
   *
   * @public
   * @returns {object}
   */
  SingleChoiceSet.prototype.getCurrentState = function () {
    return {
      progress: this.currentIndex,
      answers: this.results
    };
  };

  return SingleChoiceSet;

})(H5P.jQuery, H5P.Question, H5P.SingleChoiceSet.SingleChoice, H5P.SingleChoiceSet.SolutionView, H5P.SingleChoiceSet.ResultSlide, H5P.SingleChoiceSet.SoundEffects);
;H5P.Summary = (function ($, Question) {

  function Summary(options, contentId, contentData) {
    if (!(this instanceof H5P.Summary)) {
      return new H5P.Summary(options, contentId);
    }
    this.id = this.contentId = contentId;
    Question.call(this, 'summary');
    this.offset = 0;
    this.score = 0;
    this.progress = 0;
    this.answers = [];
    this.answer = [];
    this.error_counts = [];
    if (contentData && contentData.previousState !== undefined &&
        contentData.previousState.progress !== undefined &&
        contentData.previousState.answers) {
      this.progress = contentData.previousState.progress || this.progress;
      this.answers = contentData.previousState.answers || this.answers;

      var currentProgress = this.progress;

      // Do not count score screen as an error
      if (this.progress >= options.summaries.length) {
        currentProgress = options.summaries.length - 1;
      }

      for (var i = 0; i <= currentProgress; i++) {
        if (this.error_counts[i] === undefined) {
          this.error_counts[i] = 0;
        }
        if (this.answers[i]) {
          this.score += this.answers[i].length;
          this.error_counts[i]++;
        }
      }
    }
    var that = this;
    this.options = H5P.jQuery.extend({}, {
      response: {
        scorePerfect:
        {
          title: "PERFECT!",
          message: "You got everything correct on your first try. Be proud!"
        },
        scoreOver70:
        {
          title: "Great!",
          message: "You got most of the statements correct on your first try!"
        },
        scoreOver40:
        {
          title: "Ok",
          message: "You got some of the statements correct on your first try. There is still room for improvement."
        },
        scoreOver0:
        {
          title: "Not good",
          message: "You need to work more on this"
        }
      },
      summary: "You got @score of @total statements (@percent %) correct on your first try.",
      resultLabel: "Your result:",
      intro: "Choose the correct statement.",
      solvedLabel: "Solved:",
      scoreLabel: "Wrong answers:",
      postUserStatistics: (H5P.postUserStatistics === true)
    }, options);

    this.summaries = that.options.summaries;

    // Required questiontype contract function
    this.showSolutions = function() {
      // intentionally left blank, no solution view exists
    };

    // Required questiontype contract function
    this.getMaxScore = function() {
      return this.summaries.length;
    };

    this.getScore = function() {
      return this.getMaxScore() - this.countErrors();
    };

    this.getTitle = function() {
      return H5P.createTitle(this.options.intro);
    };

    this.getCurrentState = function () {
      return {
        progress: this.progress,
        answers: this.answers
      };
    };
  }

  Summary.prototype = Object.create(Question.prototype);
  Summary.prototype.constructor = Summary;

  /**
   * Registers DOM elements before they are attached.
   * Called from H5P.Question.
   */
  Summary.prototype.registerDomElements = function () {
    // Register task content area
    this.setContent(this.createQuestion());
  };

  // Function for attaching the multichoice to a DOM element.
  Summary.prototype.createQuestion = function() {
    var that = this;
    var c = 0; // element counter
    var elements = [];
    var $ = H5P.jQuery;
    this.$myDom = $('<div>', {
      'class': 'summary-content'
    });

    if (that.summaries === undefined || that.summaries.length === 0) {
      return;
    }

    // Create array objects
    for (var i = 0; i < that.summaries.length; i++) {
      if (!(that.summaries[i].summary && that.summaries[i].summary.length)) {
        continue;
      }

      elements[i] = {
        tip: that.summaries[i].tip,
        summaries: []
      };

      for (var j = 0; j < that.summaries[i].summary.length; j++) {
        that.answer[c] = (j === 0); // First claim is correct
        elements[i].summaries[j] = {
          id: c++,
          text: that.summaries[i].summary[j]
        };
      }

      // Randomize elements
      for (var k = elements[i].summaries.length - 1; k > 0; k--) {
        var j = Math.floor(Math.random() * (k + 1));
        var temp = elements[i].summaries[k];
        elements[i].summaries[k] = elements[i].summaries[j];
        elements[i].summaries[j] = temp;
      }
    }

    // Create content panels
    var $summary_container = $('<div class="summary-container"></div>');
    var $summary_list = $('<ul></ul>');
    var $evaluation = $('<div class="summary-evaluation"></div>');
    var $evaluation_content = $('<div class="summary-evaluation-content">' + that.options.intro + '</div>');
    var $score = $('<div class="summary-score"></div>');
    var $options = $('<div class="summary-options"></div>');
    var $progress = $('<div class="summary-progress"></div>');
    var options_padding = parseInt($options.css('paddingLeft'));

    if (this.score) {
      $score.html(that.options.scoreLabel + ' ' + this.score).show();
    }

    // Insert content
    $summary_container.append($summary_list);
    this.$myDom.append($summary_container);
    this.$myDom.append($evaluation);
    this.$myDom.append($options);
    $evaluation.append($evaluation_content);
    $evaluation.append($evaluation);
    $evaluation.append($progress);
    $evaluation.append($score);

    $progress.html(that.options.solvedLabel + ' ' + this.progress + '/' + that.summaries.length);

    // Add elements to content
    for (var i = 0; i < elements.length; i++) {
      var element = elements[i];

      if (i < that.progress) { // i is panel_id
        for (var j = 0; j < element.summaries.length; j++) {
          var sum = element.summaries[j];
          if (that.answer[sum.id]) {
            $summary_list.append('<li style="display:block">' + sum.text + '</li>');
            $summary_container.addClass('has-results');
            break;
          }
        }
        // Cannot use continue; due to id/animation system
      }

      var $page = $('<ul class="h5p-panel" data-panel="' + i + '"></ul>');


      // Create initial tip for first summary-list if tip is available
      if (i==0 && element.tip !== undefined && element.tip.trim().length > 0) {
        $evaluation_content.append(H5P.JoubelUI.createTip(element.tip));
      }

      for (var j = 0; j < element.summaries.length; j++) {
        var summaryLineClass = 'summary-claim-unclicked';

        // If progress is at current task
        if (that.progress === i && that.answers[that.progress]) {
          // Check if there are any previous wrong answers.
          for (var k = 0; k < that.answers[that.progress].length; k++) {
            if (that.answers[that.progress][k] === element.summaries[j].id) {
              summaryLineClass = 'summary-failed';
              break;
            }
          }
        }

        var $node = $('<li data-bit="' + element.summaries[j].id + '" class="' + summaryLineClass + '">' + element.summaries[j].text + '</li>');

        // Do not add click event for failed nodes
        if (summaryLineClass === 'summary-failed') {
          $page.append($node);
          continue;
        }

        // When correct claim is clicked:
        // - Add claim to summary list
        // - Move claim over clicked element
        // - Animate correct claim into correct position
        // - Show next panel
        // When wrong claim is clicked:
        // - Remove clickable
        // - Add error background image (css)
        $node.click(function() {
          that.triggerXAPI('interacted');
          var $el = $(this);
          var node_id = Number($el.attr('data-bit'));
          var classname = that.answer[node_id] ? 'success' : 'failed';
          var panel_id = Number($el.parent().data('panel'));
          if (that.error_counts[panel_id] === undefined) {
            that.error_counts[panel_id] = 0;
          }

          // Correct answer?
          if (that.answer[node_id]) {
            that.progress++;
            var position = $el.position();
            var summary = $summary_list.position();
            var $answer = $('<li>' + $el.html() + '</li>');

            $progress.html(that.options.solvedLabel + ' '  + (panel_id + 1) + '/' + that.summaries.length);

            // Insert correct claim into summary list
            $summary_list.append($answer);
            $summary_container.addClass('has-results');
            that.adjustTargetHeight($summary_container, $summary_list, $answer);

            // Move into position over clicked element
            $answer.css({display: 'block', width: $el.css('width'), height: $el.css('height')});
            $answer.css({position: 'absolute', top: position.top, left: position.left});
            $answer.css({backgroundColor: '#9dd8bb', border: ''});
            setTimeout(function () {
              $answer.css({backgroundColor: ''});
            }, 1);
            //$answer.animate({backgroundColor: '#eee'}, 'slow');

            var panel = parseInt($el.parent().attr('data-panel'));
            var $curr_panel = $('.h5p-panel:eq(' + panel + ')', that.$myDom);
            var $next_panel = $('.h5p-panel:eq(' + (panel + 1) + ')', that.$myDom);
            var height = $curr_panel.parent().css('height');

            // Disable panel while waiting for animation
            $curr_panel.addClass('panel-disabled');

            // Update tip:
            $evaluation_content.find('.joubel-tip-container').remove();
            if (elements[that.progress] !== undefined &&
              elements[that.progress].tip !== undefined &&
              elements[that.progress].tip.trim().length > 0) {
              $evaluation_content.append(H5P.JoubelUI.createTip(elements[that.progress].tip));
            }

            $answer.animate(
              {
                top: summary.top + that.offset,
                left: '-=' + options_padding + 'px',
                width: '+=' + (options_padding * 2) + 'px'
              },
              {
                complete: function() {
                  // Remove position (becomes inline);
                  $(this).css('position', '').css({
                    width: '',
                    height: '',
                    top: '',
                    left: ''
                  });
                  $summary_container.css('height', '');

                  // Calculate offset for next summary item
                  var tpadding = parseInt($answer.css('paddingTop')) * 2;
                  var tmargin = parseInt($answer.css('marginBottom'));
                  var theight = parseInt($answer.css('height'));
                  that.offset += theight + tpadding + tmargin + 1;

                  // Fade out current panel
                  $curr_panel.fadeOut('fast', function () {
                    $curr_panel.parent().css('height', 'auto');
                    // Show next panel if present
                    if ($next_panel.length) {
                      $next_panel.fadeIn('fast');
                    } else {
                      // Hide intermediate evaluation
                      $evaluation_content.html(that.options.resultLabel);

                      that.do_final_evaluation($summary_container, $options, $summary_list, that.score);
                    }
                    that.trigger('resize');
                  });
                }
              }
            );
          }
          else {
            // Remove event handler (prevent repeated clicks) and mouseover effect
            $el.off('click');
            $el.addClass('summary-failed');
            $el.removeClass('summary-claim-unclicked');

            $evaluation.children('.summary-score').css('display', 'block');
            $score.html(that.options.scoreLabel + ' ' + (++that.score));
            that.error_counts[panel_id]++;
            if (that.answers[panel_id] === undefined) {
              that.answers[panel_id] = [];
            }
            that.answers[panel_id].push(node_id);
          }

          that.trigger('resize');
        });

        $page.append($node);
      }

      $options.append($page);
    }

    if (that.progress === elements.length) {
      $evaluation_content.html(that.options.resultLabel);
      that.do_final_evaluation($summary_container, $options, $summary_list, that.score);
    }
    else {
      // Show first panel
      $('.h5p-panel:eq(' + (that.progress) + ')', that.$myDom).css({display: 'block'});
      if (that.progress) {
        that.offset = ($('.summary-claim-unclicked:visible:first', that.$myDom).outerHeight() * that.error_counts.length);
      }
    }

    that.trigger('resize');

    return this.$myDom;
  };

  /**
   * Calculate final score and display feedback.
   *
   * @param container
   * @param options_panel
   * @param list
   * @param score
   */
  Summary.prototype.do_final_evaluation = function (container, options_panel, list, score) {
    var that = this;
    var error_count = this.countErrors();

    // Calculate percentage
    var percent = 100 - (error_count / that.error_counts.length * 100);

    // Find evaluation message
    var from = 0;
    for (var i in that.options.response) {
      switch (i) {
        case "scorePerfect":
          from = 100;
          break;
        case "scoreOver70":
          from = 70;
          break;
        case "scoreOver40":
          from = 40;
          break;
        case "scoreOver0":
          from = 0;
          break;
      }
      if (percent >= from) {
        break;
      }
    }

    // Show final evaluation
    var summary = that.options.summary.replace('@score', that.summaries.length - error_count).replace('@total', that.summaries.length).replace('@percent', Math.round(percent));
    this.setFeedback(summary, that.summaries.length - error_count, that.summaries.length);

    that.trigger('resize');
    var myScore = Math.max(that.error_counts.length - error_count, 0);
    that.triggerXAPIScored(myScore, that.error_counts.length, 'answered');
  };

  /**
   * Resets the complete task back to its' initial state.
   * Used for contracts.
   */
  Summary.prototype.resetTask = function () {
    // Summary is not yet able to Reset itself
  };

  /**
   * Adjust height of container.
   *
   * @param container
   * @param elements
   * @param el
   */
  Summary.prototype.adjustTargetHeight = function (container, elements, el) {
    var new_height = parseInt(elements.outerHeight()) + parseInt(el.outerHeight()) + parseInt(el.css('marginBottom')) + parseInt(el.css('marginTop'));
    if (new_height > parseInt(container.css('height'))) {
      container.animate({height: new_height});
    }
  };

  /**
   * Count amount of wrong answers
   *
   * @returns {number}
   */
  Summary.prototype.countErrors = function() {
    var error_count = 0;

    // Count boards without errors
    for (var i = 0; i < this.summaries.length; i++) {
      if (this.error_counts[i] === undefined) {
        error_count++;
      }
      else {
        error_count += this.error_counts[i] ? 1 : 0;
      }
    }

    return error_count;
  };

  return Summary;

})(H5P.jQuery, H5P.Question);
;var oldJQuery=jQuery;jQuery=H5P.jQuery,function(a,b){function e(b,c){var d,e,g,h=b.nodeName.toLowerCase();return"area"===h?(d=b.parentNode,e=d.name,!(!b.href||!e||"map"!==d.nodeName.toLowerCase())&&(!!(g=a("img[usemap=#"+e+"]")[0])&&f(g))):(/input|select|textarea|button|object/.test(h)?!b.disabled:"a"===h?b.href||c:c)&&f(b)}function f(b){return a.expr.filters.visible(b)&&!a(b).parents().addBack().filter(function(){return"hidden"===a.css(this,"visibility")}).length}var c=0,d=/^ui-id-\d+$/;a.ui=a.ui||{},a.extend(a.ui,{version:"1.10.2",keyCode:{BACKSPACE:8,COMMA:188,DELETE:46,DOWN:40,END:35,ENTER:13,ESCAPE:27,HOME:36,LEFT:37,NUMPAD_ADD:107,NUMPAD_DECIMAL:110,NUMPAD_DIVIDE:111,NUMPAD_ENTER:108,NUMPAD_MULTIPLY:106,NUMPAD_SUBTRACT:109,PAGE_DOWN:34,PAGE_UP:33,PERIOD:190,RIGHT:39,SPACE:32,TAB:9,UP:38}}),a.fn.extend({focus:function(b){return function(c,d){return"number"==typeof c?this.each(function(){var b=this;setTimeout(function(){a(b).focus(),d&&d.call(b)},c)}):b.apply(this,arguments)}}(a.fn.focus),scrollParent:function(){var b;return b=a.ui.ie&&/(static|relative)/.test(this.css("position"))||/absolute/.test(this.css("position"))?this.parents().filter(function(){return/(relative|absolute|fixed)/.test(a.css(this,"position"))&&/(auto|scroll)/.test(a.css(this,"overflow")+a.css(this,"overflow-y")+a.css(this,"overflow-x"))}).eq(0):this.parents().filter(function(){return/(auto|scroll)/.test(a.css(this,"overflow")+a.css(this,"overflow-y")+a.css(this,"overflow-x"))}).eq(0),/fixed/.test(this.css("position"))||!b.length?a(document):b},zIndex:function(c){if(c!==b)return this.css("zIndex",c);if(this.length)for(var e,f,d=a(this[0]);d.length&&d[0]!==document;){if(("absolute"===(e=d.css("position"))||"relative"===e||"fixed"===e)&&(f=parseInt(d.css("zIndex"),10),!isNaN(f)&&0!==f))return f;d=d.parent()}return 0},uniqueId:function(){return this.each(function(){this.id||(this.id="ui-id-"+ ++c)})},removeUniqueId:function(){return this.each(function(){d.test(this.id)&&a(this).removeAttr("id")})}}),a.extend(a.expr[":"],{data:a.expr.createPseudo?a.expr.createPseudo(function(b){return function(c){return!!a.data(c,b)}}):function(b,c,d){return!!a.data(b,d[3])},focusable:function(b){return e(b,!isNaN(a.attr(b,"tabindex")))},tabbable:function(b){var c=a.attr(b,"tabindex"),d=isNaN(c);return(d||c>=0)&&e(b,!d)}}),a("<a>").outerWidth(1).jquery||a.each(["Width","Height"],function(c,d){function h(b,c,d,f){return a.each(e,function(){c-=parseFloat(a.css(b,"padding"+this))||0,d&&(c-=parseFloat(a.css(b,"border"+this+"Width"))||0),f&&(c-=parseFloat(a.css(b,"margin"+this))||0)}),c}var e="Width"===d?["Left","Right"]:["Top","Bottom"],f=d.toLowerCase(),g={innerWidth:a.fn.innerWidth,innerHeight:a.fn.innerHeight,outerWidth:a.fn.outerWidth,outerHeight:a.fn.outerHeight};a.fn["inner"+d]=function(c){return c===b?g["inner"+d].call(this):this.each(function(){a(this).css(f,h(this,c)+"px")})},a.fn["outer"+d]=function(b,c){return"number"!=typeof b?g["outer"+d].call(this,b):this.each(function(){a(this).css(f,h(this,b,!0,c)+"px")})}}),a.fn.addBack||(a.fn.addBack=function(a){return this.add(null==a?this.prevObject:this.prevObject.filter(a))}),a("<a>").data("a-b","a").removeData("a-b").data("a-b")&&(a.fn.removeData=function(b){return function(c){return arguments.length?b.call(this,a.camelCase(c)):b.call(this)}}(a.fn.removeData)),a.ui.ie=!!/msie [\w.]+/.exec(navigator.userAgent.toLowerCase()),a.support.selectstart="onselectstart"in document.createElement("div"),a.fn.extend({disableSelection:function(){return this.bind((a.support.selectstart?"selectstart":"mousedown")+".ui-disableSelection",function(a){a.preventDefault()})},enableSelection:function(){return this.unbind(".ui-disableSelection")}}),a.extend(a.ui,{plugin:{add:function(b,c,d){var e,f=a.ui[b].prototype;for(e in d)f.plugins[e]=f.plugins[e]||[],f.plugins[e].push([c,d[e]])},call:function(a,b,c){var d,e=a.plugins[b];if(e&&a.element[0].parentNode&&11!==a.element[0].parentNode.nodeType)for(d=0;d<e.length;d++)a.options[e[d][0]]&&e[d][1].apply(a.element,c)}},hasScroll:function(b,c){if("hidden"===a(b).css("overflow"))return!1;var d=c&&"left"===c?"scrollLeft":"scrollTop",e=!1;return b[d]>0||(b[d]=1,e=b[d]>0,b[d]=0,e)}})}(jQuery),function(a,b){var c=0,d=Array.prototype.slice,e=a.cleanData;a.cleanData=function(b){for(var d,c=0;null!=(d=b[c]);c++)try{a(d).triggerHandler("remove")}catch(a){}e(b)},a.widget=function(b,c,d){var e,f,g,h,i={},j=b.split(".")[0];b=b.split(".")[1],e=j+"-"+b,d||(d=c,c=a.Widget),a.expr[":"][e.toLowerCase()]=function(b){return!!a.data(b,e)},a[j]=a[j]||{},f=a[j][b],g=a[j][b]=function(a,b){if(!this._createWidget)return new g(a,b);arguments.length&&this._createWidget(a,b)},a.extend(g,f,{version:d.version,_proto:a.extend({},d),_childConstructors:[]}),h=new c,h.options=a.widget.extend({},h.options),a.each(d,function(b,d){if(!a.isFunction(d))return void(i[b]=d);i[b]=function(){var a=function(){return c.prototype[b].apply(this,arguments)},e=function(a){return c.prototype[b].apply(this,a)};return function(){var f,b=this._super,c=this._superApply;return this._super=a,this._superApply=e,f=d.apply(this,arguments),this._super=b,this._superApply=c,f}}()}),g.prototype=a.widget.extend(h,{widgetEventPrefix:f?h.widgetEventPrefix:b},i,{constructor:g,namespace:j,widgetName:b,widgetFullName:e}),f?(a.each(f._childConstructors,function(b,c){var d=c.prototype;a.widget(d.namespace+"."+d.widgetName,g,c._proto)}),delete f._childConstructors):c._childConstructors.push(g),a.widget.bridge(b,g)},a.widget.extend=function(c){for(var h,i,e=d.call(arguments,1),f=0,g=e.length;f<g;f++)for(h in e[f])i=e[f][h],e[f].hasOwnProperty(h)&&i!==b&&(a.isPlainObject(i)?c[h]=a.isPlainObject(c[h])?a.widget.extend({},c[h],i):a.widget.extend({},i):c[h]=i);return c},a.widget.bridge=function(c,e){var f=e.prototype.widgetFullName||c;a.fn[c]=function(g){var h="string"==typeof g,i=d.call(arguments,1),j=this;return g=!h&&i.length?a.widget.extend.apply(null,[g].concat(i)):g,h?this.each(function(){var d,e=a.data(this,f);return e?a.isFunction(e[g])&&"_"!==g.charAt(0)?(d=e[g].apply(e,i),d!==e&&d!==b?(j=d&&d.jquery?j.pushStack(d.get()):d,!1):void 0):a.error("no such method '"+g+"' for "+c+" widget instance"):a.error("cannot call methods on "+c+" prior to initialization; attempted to call method '"+g+"'")}):this.each(function(){var b=a.data(this,f);b?b.option(g||{})._init():a.data(this,f,new e(g,this))}),j}},a.Widget=function(){},a.Widget._childConstructors=[],a.Widget.prototype={widgetName:"widget",widgetEventPrefix:"",defaultElement:"<div>",options:{disabled:!1,create:null},_createWidget:function(b,d){d=a(d||this.defaultElement||this)[0],this.element=a(d),this.uuid=c++,this.eventNamespace="."+this.widgetName+this.uuid,this.options=a.widget.extend({},this.options,this._getCreateOptions(),b),this.bindings=a(),this.hoverable=a(),this.focusable=a(),d!==this&&(a.data(d,this.widgetFullName,this),this._on(!0,this.element,{remove:function(a){a.target===d&&this.destroy()}}),this.document=a(d.style?d.ownerDocument:d.document||d),this.window=a(this.document[0].defaultView||this.document[0].parentWindow)),this._create(),this._trigger("create",null,this._getCreateEventData()),this._init()},_getCreateOptions:a.noop,_getCreateEventData:a.noop,_create:a.noop,_init:a.noop,destroy:function(){this._destroy(),this.element.unbind(this.eventNamespace).removeData(this.widgetName).removeData(this.widgetFullName).removeData(a.camelCase(this.widgetFullName)),this.widget().unbind(this.eventNamespace).removeAttr("aria-disabled").removeClass(this.widgetFullName+"-disabled ui-state-disabled"),this.bindings.unbind(this.eventNamespace),this.hoverable.removeClass("ui-state-hover"),this.focusable.removeClass("ui-state-focus")},_destroy:a.noop,widget:function(){return this.element},option:function(c,d){var f,g,h,e=c;if(0===arguments.length)return a.widget.extend({},this.options);if("string"==typeof c)if(e={},f=c.split("."),c=f.shift(),f.length){for(g=e[c]=a.widget.extend({},this.options[c]),h=0;h<f.length-1;h++)g[f[h]]=g[f[h]]||{},g=g[f[h]];if(c=f.pop(),d===b)return g[c]===b?null:g[c];g[c]=d}else{if(d===b)return this.options[c]===b?null:this.options[c];e[c]=d}return this._setOptions(e),this},_setOptions:function(a){var b;for(b in a)this._setOption(b,a[b]);return this},_setOption:function(a,b){return this.options[a]=b,"disabled"===a&&(this.widget().toggleClass(this.widgetFullName+"-disabled ui-state-disabled",!!b).attr("aria-disabled",b),this.hoverable.removeClass("ui-state-hover"),this.focusable.removeClass("ui-state-focus")),this},enable:function(){return this._setOption("disabled",!1)},disable:function(){return this._setOption("disabled",!0)},_on:function(b,c,d){var e,f=this;"boolean"!=typeof b&&(d=c,c=b,b=!1),d?(c=e=a(c),this.bindings=this.bindings.add(c)):(d=c,c=this.element,e=this.widget()),a.each(d,function(d,g){function h(){if(b||!0!==f.options.disabled&&!a(this).hasClass("ui-state-disabled"))return("string"==typeof g?f[g]:g).apply(f,arguments)}"string"!=typeof g&&(h.guid=g.guid=g.guid||h.guid||a.guid++);var i=d.match(/^(\w+)\s*(.*)$/),j=i[1]+f.eventNamespace,k=i[2];k?e.delegate(k,j,h):c.bind(j,h)})},_off:function(a,b){b=(b||"").split(" ").join(this.eventNamespace+" ")+this.eventNamespace,a.unbind(b).undelegate(b)},_delay:function(a,b){function c(){return("string"==typeof a?d[a]:a).apply(d,arguments)}var d=this;return setTimeout(c,b||0)},_hoverable:function(b){this.hoverable=this.hoverable.add(b),this._on(b,{mouseenter:function(b){a(b.currentTarget).addClass("ui-state-hover")},mouseleave:function(b){a(b.currentTarget).removeClass("ui-state-hover")}})},_focusable:function(b){this.focusable=this.focusable.add(b),this._on(b,{focusin:function(b){a(b.currentTarget).addClass("ui-state-focus")},focusout:function(b){a(b.currentTarget).removeClass("ui-state-focus")}})},_trigger:function(b,c,d){var e,f,g=this.options[b];if(d=d||{},c=a.Event(c),c.type=(b===this.widgetEventPrefix?b:this.widgetEventPrefix+b).toLowerCase(),c.target=this.element[0],f=c.originalEvent)for(e in f)e in c||(c[e]=f[e]);return this.element.trigger(c,d),!(a.isFunction(g)&&!1===g.apply(this.element[0],[c].concat(d))||c.isDefaultPrevented())}},a.each({show:"fadeIn",hide:"fadeOut"},function(b,c){a.Widget.prototype["_"+b]=function(d,e,f){"string"==typeof e&&(e={effect:e});var g,h=e?!0===e||"number"==typeof e?c:e.effect||c:b;e=e||{},"number"==typeof e&&(e={duration:e}),g=!a.isEmptyObject(e),e.complete=f,e.delay&&d.delay(e.delay),g&&a.effects&&a.effects.effect[h]?d[b](e):h!==b&&d[h]?d[h](e.duration,e.easing,f):d.queue(function(c){a(this)[b](),f&&f.call(d[0]),c()})}})}(jQuery),function(a,b){var c=!1;a(document).mouseup(function(){c=!1}),a.widget("ui.mouse",{version:"1.10.2",options:{cancel:"input,textarea,button,select,option",distance:1,delay:0},_mouseInit:function(){var b=this;this.element.bind("mousedown."+this.widgetName,function(a){return b._mouseDown(a)}).bind("click."+this.widgetName,function(c){if(!0===a.data(c.target,b.widgetName+".preventClickEvent"))return a.removeData(c.target,b.widgetName+".preventClickEvent"),c.stopImmediatePropagation(),!1}),this.started=!1},_mouseDestroy:function(){this.element.unbind("."+this.widgetName),this._mouseMoveDelegate&&a(document).unbind("mousemove."+this.widgetName,this._mouseMoveDelegate).unbind("mouseup."+this.widgetName,this._mouseUpDelegate)},_mouseDown:function(b){if(!c){this._mouseStarted&&this._mouseUp(b),this._mouseDownEvent=b;var d=this,e=1===b.which,f=!("string"!=typeof this.options.cancel||!b.target.nodeName)&&a(b.target).closest(this.options.cancel).length;return!(e&&!f&&this._mouseCapture(b))||(this.mouseDelayMet=!this.options.delay,this.mouseDelayMet||(this._mouseDelayTimer=setTimeout(function(){d.mouseDelayMet=!0},this.options.delay)),this._mouseDistanceMet(b)&&this._mouseDelayMet(b)&&(this._mouseStarted=!1!==this._mouseStart(b),!this._mouseStarted)?(b.preventDefault(),!0):(!0===a.data(b.target,this.widgetName+".preventClickEvent")&&a.removeData(b.target,this.widgetName+".preventClickEvent"),this._mouseMoveDelegate=function(a){return d._mouseMove(a)},this._mouseUpDelegate=function(a){return d._mouseUp(a)},a(document).bind("mousemove."+this.widgetName,this._mouseMoveDelegate).bind("mouseup."+this.widgetName,this._mouseUpDelegate),b.preventDefault(),c=!0,!0))}},_mouseMove:function(b){return a.ui.ie&&(!document.documentMode||document.documentMode<9)&&!b.button?this._mouseUp(b):this._mouseStarted?(this._mouseDrag(b),b.preventDefault()):(this._mouseDistanceMet(b)&&this._mouseDelayMet(b)&&(this._mouseStarted=!1!==this._mouseStart(this._mouseDownEvent,b),this._mouseStarted?this._mouseDrag(b):this._mouseUp(b)),!this._mouseStarted)},_mouseUp:function(b){return a(document).unbind("mousemove."+this.widgetName,this._mouseMoveDelegate).unbind("mouseup."+this.widgetName,this._mouseUpDelegate),this._mouseStarted&&(this._mouseStarted=!1,b.target===this._mouseDownEvent.target&&a.data(b.target,this.widgetName+".preventClickEvent",!0),this._mouseStop(b)),!1},_mouseDistanceMet:function(a){return Math.max(Math.abs(this._mouseDownEvent.pageX-a.pageX),Math.abs(this._mouseDownEvent.pageY-a.pageY))>=this.options.distance},_mouseDelayMet:function(){return this.mouseDelayMet},_mouseStart:function(){},_mouseDrag:function(){},_mouseStop:function(){},_mouseCapture:function(){return!0}})}(jQuery),function(a,b){a.widget("ui.draggable",a.ui.mouse,{version:"1.10.2",widgetEventPrefix:"drag",options:{addClasses:!0,appendTo:"parent",axis:!1,connectToSortable:!1,containment:!1,cursor:"auto",cursorAt:!1,grid:!1,handle:!1,helper:"original",iframeFix:!1,opacity:!1,refreshPositions:!1,revert:!1,revertDuration:500,scope:"default",scroll:!0,scrollSensitivity:20,scrollSpeed:20,snap:!1,snapMode:"both",snapTolerance:20,stack:!1,zIndex:!1,drag:null,start:null,stop:null},_create:function(){"original"!==this.options.helper||/^(?:r|a|f)/.test(this.element.css("position"))||(this.element[0].style.position="relative"),this.options.addClasses&&this.element.addClass("ui-draggable"),this.options.disabled&&this.element.addClass("ui-draggable-disabled"),this.element[0].style.msTouchAction=this.element[0].style.touchAction="none",this._mouseInit()},_destroy:function(){this.element.removeClass("ui-draggable ui-draggable-dragging ui-draggable-disabled"),this._mouseDestroy()},_mouseCapture:function(b){var c=this.options;return!(this.helper||c.disabled||a(b.target).closest(".ui-resizable-handle").length>0)&&(this.handle=this._getHandle(b),!!this.handle&&(a(!0===c.iframeFix?"iframe":c.iframeFix).each(function(){a("<div class='ui-draggable-iframeFix' style='background: #fff;'></div>").css({width:this.offsetWidth+"px",height:this.offsetHeight+"px",position:"absolute",opacity:"0.001",zIndex:1e3}).css(a(this).offset()).appendTo("body")}),!0))},_mouseStart:function(b){var c=this.options;return this.helper=this._createHelper(b),this.helper.addClass("ui-draggable-dragging"),this._cacheHelperProportions(),a.ui.ddmanager&&(a.ui.ddmanager.current=this),this._cacheMargins(),this.cssPosition=this.helper.css("position"),this.scrollParent=this.helper.scrollParent(),this.offset=this.positionAbs=this.element.offset(),this.offset={top:this.offset.top-this.margins.top,left:this.offset.left-this.margins.left},a.extend(this.offset,{click:{left:b.pageX-this.offset.left,top:b.pageY-this.offset.top},parent:this._getParentOffset(),relative:this._getRelativeOffset()}),this.originalPosition=this.position=this._generatePosition(b),this.originalPageX=b.pageX,this.originalPageY=b.pageY,c.cursorAt&&this._adjustOffsetFromHelper(c.cursorAt),c.containment&&this._setContainment(),!1===this._trigger("start",b)?(this._clear(),!1):(this._cacheHelperProportions(),a.ui.ddmanager&&!c.dropBehaviour&&a.ui.ddmanager.prepareOffsets(this,b),this._mouseDrag(b,!0),a.ui.ddmanager&&a.ui.ddmanager.dragStart(this,b),!0)},_mouseDrag:function(b,c){if(this.position=this._generatePosition(b),this.positionAbs=this._convertPositionTo("absolute"),!c){var d=this._uiHash();if(!1===this._trigger("drag",b,d))return this._mouseUp({}),!1;this.position=d.position}return this.options.axis&&"y"===this.options.axis||(this.helper[0].style.left=this.position.left+"px"),this.options.axis&&"x"===this.options.axis||(this.helper[0].style.top=this.position.top+"px"),a.ui.ddmanager&&a.ui.ddmanager.drag(this,b),!1},_mouseStop:function(b){var c,d=this,e=!1,f=!1;for(a.ui.ddmanager&&!this.options.dropBehaviour&&(f=a.ui.ddmanager.drop(this,b)),this.dropped&&(f=this.dropped,this.dropped=!1),c=this.element[0];c&&(c=c.parentNode);)c===document&&(e=!0);return!(!e&&"original"===this.options.helper)&&("invalid"===this.options.revert&&!f||"valid"===this.options.revert&&f||!0===this.options.revert||a.isFunction(this.options.revert)&&this.options.revert.call(this.element,f)?a(this.helper).animate(this.originalPosition,parseInt(this.options.revertDuration,10),function(){!1!==d._trigger("stop",b)&&d._clear()}):!1!==this._trigger("stop",b)&&this._clear(),!1)},_mouseUp:function(b){return a("div.ui-draggable-iframeFix").each(function(){this.parentNode.removeChild(this)}),a.ui.ddmanager&&a.ui.ddmanager.dragStop(this,b),a.ui.mouse.prototype._mouseUp.call(this,b)},cancel:function(){return this.helper.is(".ui-draggable-dragging")?this._mouseUp({}):this._clear(),this},_getHandle:function(b){return!this.options.handle||!!a(b.target).closest(this.element.find(this.options.handle)).length},_createHelper:function(b){var c=this.options,d=a.isFunction(c.helper)?a(c.helper.apply(this.element[0],[b])):"clone"===c.helper?this.element.clone().removeAttr("id"):this.element;return d.parents("body").length||d.appendTo("parent"===c.appendTo?this.element[0].parentNode:c.appendTo),d[0]===this.element[0]||/(fixed|absolute)/.test(d.css("position"))||d.css("position","absolute"),d},_adjustOffsetFromHelper:function(b){"string"==typeof b&&(b=b.split(" ")),a.isArray(b)&&(b={left:+b[0],top:+b[1]||0}),"left"in b&&(this.offset.click.left=b.left+this.margins.left),"right"in b&&(this.offset.click.left=this.helperProportions.width-b.right+this.margins.left),"top"in b&&(this.offset.click.top=b.top+this.margins.top),"bottom"in b&&(this.offset.click.top=this.helperProportions.height-b.bottom+this.margins.top)},_getParentOffset:function(){this.offsetParent=this.helper.offsetParent();var b=this.offsetParent.offset();return"absolute"===this.cssPosition&&this.scrollParent[0]!==document&&a.contains(this.scrollParent[0],this.offsetParent[0])&&(b.left+=this.scrollParent.scrollLeft(),b.top+=this.scrollParent.scrollTop()),(this.offsetParent[0]===document.body||this.offsetParent[0].tagName&&"html"===this.offsetParent[0].tagName.toLowerCase()&&a.ui.ie)&&(b={top:0,left:0}),{top:b.top+(parseInt(this.offsetParent.css("borderTopWidth"),10)||0),left:b.left+(parseInt(this.offsetParent.css("borderLeftWidth"),10)||0)}},_getRelativeOffset:function(){if("relative"===this.cssPosition){var a=this.element.position();return{top:a.top-(parseInt(this.helper.css("top"),10)||0)+this.scrollParent.scrollTop(),left:a.left-(parseInt(this.helper.css("left"),10)||0)+this.scrollParent.scrollLeft()}}return{top:0,left:0}},_cacheMargins:function(){this.margins={left:parseInt(this.element.css("marginLeft"),10)||0,top:parseInt(this.element.css("marginTop"),10)||0,right:parseInt(this.element.css("marginRight"),10)||0,bottom:parseInt(this.element.css("marginBottom"),10)||0}},_cacheHelperProportions:function(){this.helperProportions={width:this.helper.outerWidth(),height:this.helper.outerHeight()}},_setContainment:function(){var b,c,d,e=this.options;if("parent"===e.containment&&(e.containment=this.helper[0].parentNode),"document"!==e.containment&&"window"!==e.containment||(this.containment=["document"===e.containment?0:a(window).scrollLeft()-this.offset.relative.left-this.offset.parent.left,"document"===e.containment?0:a(window).scrollTop()-this.offset.relative.top-this.offset.parent.top,("document"===e.containment?0:a(window).scrollLeft())+a("document"===e.containment?document:window).width()-this.helperProportions.width-this.margins.left,("document"===e.containment?0:a(window).scrollTop())+(a("document"===e.containment?document:window).height()||document.body.parentNode.scrollHeight)-this.helperProportions.height-this.margins.top]),/^(document|window|parent)$/.test(e.containment)||e.containment.constructor===Array)e.containment.constructor===Array&&(this.containment=e.containment);else{if(c=a(e.containment),!(d=c[0]))return;b="hidden"!==a(d).css("overflow"),this.containment=[(parseInt(a(d).css("borderLeftWidth"),10)||0)+(parseInt(a(d).css("paddingLeft"),10)||0),(parseInt(a(d).css("borderTopWidth"),10)||0)+(parseInt(a(d).css("paddingTop"),10)||0),(b?Math.max(d.scrollWidth,d.offsetWidth):d.offsetWidth)-(parseInt(a(d).css("borderRightWidth"),10)||0)-(parseInt(a(d).css("paddingRight"),10)||0)-this.helperProportions.width-this.margins.left-this.margins.right,(b?Math.max(d.scrollHeight,d.offsetHeight):d.offsetHeight)-(parseInt(a(d).css("borderBottomWidth"),10)||0)-(parseInt(a(d).css("paddingBottom"),10)||0)-this.helperProportions.height-this.margins.top-this.margins.bottom],this.relative_container=c}},_convertPositionTo:function(b,c){c||(c=this.position);var d="absolute"===b?1:-1,e="absolute"!==this.cssPosition||this.scrollParent[0]!==document&&a.contains(this.scrollParent[0],this.offsetParent[0])?this.scrollParent:this.offsetParent,f=/(html|body)/i.test(e[0].tagName);return{top:c.top+this.offset.relative.top*d+this.offset.parent.top*d-("fixed"===this.cssPosition?-this.scrollParent.scrollTop():f?0:e.scrollTop())*d,left:c.left+this.offset.relative.left*d+this.offset.parent.left*d-("fixed"===this.cssPosition?-this.scrollParent.scrollLeft():f?0:e.scrollLeft())*d}},_generatePosition:function(b){var c,d,e,f,g=this.options,h="absolute"!==this.cssPosition||this.scrollParent[0]!==document&&a.contains(this.scrollParent[0],this.offsetParent[0])?this.scrollParent:this.offsetParent,i=/(html|body)/i.test(h[0].tagName),j=b.pageX,k=b.pageY;return this.originalPosition&&(this.containment&&(this.relative_container?(d=this.relative_container.offset(),c=[this.containment[0]+d.left,this.containment[1]+d.top,this.containment[2]+d.left,this.containment[3]+d.top]):c=this.containment,b.pageX-this.offset.click.left<c[0]&&(j=c[0]+this.offset.click.left),b.pageY-this.offset.click.top<c[1]&&(k=c[1]+this.offset.click.top),b.pageX-this.offset.click.left>c[2]&&(j=c[2]+this.offset.click.left),b.pageY-this.offset.click.top>c[3]&&(k=c[3]+this.offset.click.top)),g.grid&&(e=g.grid[1]?this.originalPageY+Math.round((k-this.originalPageY)/g.grid[1])*g.grid[1]:this.originalPageY,k=c?e-this.offset.click.top>=c[1]||e-this.offset.click.top>c[3]?e:e-this.offset.click.top>=c[1]?e-g.grid[1]:e+g.grid[1]:e,f=g.grid[0]?this.originalPageX+Math.round((j-this.originalPageX)/g.grid[0])*g.grid[0]:this.originalPageX,j=c?f-this.offset.click.left>=c[0]||f-this.offset.click.left>c[2]?f:f-this.offset.click.left>=c[0]?f-g.grid[0]:f+g.grid[0]:f)),{top:k-this.offset.click.top-this.offset.relative.top-this.offset.parent.top+("fixed"===this.cssPosition?-this.scrollParent.scrollTop():i?0:h.scrollTop()),left:j-this.offset.click.left-this.offset.relative.left-this.offset.parent.left+("fixed"===this.cssPosition?-this.scrollParent.scrollLeft():i?0:h.scrollLeft())}},_clear:function(){this.helper.removeClass("ui-draggable-dragging"),this.helper[0]===this.element[0]||this.cancelHelperRemoval||this.helper.remove(),this.helper=null,this.cancelHelperRemoval=!1},_trigger:function(b,c,d){return d=d||this._uiHash(),a.ui.plugin.call(this,b,[c,d]),"drag"===b&&(this.positionAbs=this._convertPositionTo("absolute")),a.Widget.prototype._trigger.call(this,b,c,d)},plugins:{},_uiHash:function(){return{helper:this.helper,position:this.position,originalPosition:this.originalPosition,offset:this.positionAbs}}}),a.ui.plugin.add("draggable","connectToSortable",{start:function(b,c){var d=a(this).data("ui-draggable"),e=d.options,f=a.extend({},c,{item:d.element});d.sortables=[],a(e.connectToSortable).each(function(){var c=a.data(this,"ui-sortable");c&&!c.options.disabled&&(d.sortables.push({instance:c,shouldRevert:c.options.revert}),c.refreshPositions(),c._trigger("activate",b,f))})},stop:function(b,c){var d=a(this).data("ui-draggable"),e=a.extend({},c,{item:d.element});a.each(d.sortables,function(){this.instance.isOver?(this.instance.isOver=0,d.cancelHelperRemoval=!0,this.instance.cancelHelperRemoval=!1,this.shouldRevert&&(this.instance.options.revert=this.shouldRevert),this.instance._mouseStop(b),this.instance.options.helper=this.instance.options._helper,"original"===d.options.helper&&this.instance.currentItem.css({top:"auto",left:"auto"})):(this.instance.cancelHelperRemoval=!1,this.instance._trigger("deactivate",b,e))})},drag:function(b,c){var d=a(this).data("ui-draggable"),e=this;a.each(d.sortables,function(){var f=!1,g=this;this.instance.positionAbs=d.positionAbs,this.instance.helperProportions=d.helperProportions,this.instance.offset.click=d.offset.click,this.instance._intersectsWith(this.instance.containerCache)&&(f=!0,a.each(d.sortables,function(){return this.instance.positionAbs=d.positionAbs,this.instance.helperProportions=d.helperProportions,this.instance.offset.click=d.offset.click,this!==g&&this.instance._intersectsWith(this.instance.containerCache)&&a.contains(g.instance.element[0],this.instance.element[0])&&(f=!1),f})),f?(this.instance.isOver||(this.instance.isOver=1,this.instance.currentItem=a(e).clone().removeAttr("id").appendTo(this.instance.element).data("ui-sortable-item",!0),this.instance.options._helper=this.instance.options.helper,this.instance.options.helper=function(){return c.helper[0]},b.target=this.instance.currentItem[0],this.instance._mouseCapture(b,!0),this.instance._mouseStart(b,!0,!0),this.instance.offset.click.top=d.offset.click.top,this.instance.offset.click.left=d.offset.click.left,this.instance.offset.parent.left-=d.offset.parent.left-this.instance.offset.parent.left,this.instance.offset.parent.top-=d.offset.parent.top-this.instance.offset.parent.top,d._trigger("toSortable",b),d.dropped=this.instance.element,d.currentItem=d.element,this.instance.fromOutside=d),this.instance.currentItem&&this.instance._mouseDrag(b)):this.instance.isOver&&(this.instance.isOver=0,this.instance.cancelHelperRemoval=!0,this.instance.options.revert=!1,this.instance._trigger("out",b,this.instance._uiHash(this.instance)),this.instance._mouseStop(b,!0),this.instance.options.helper=this.instance.options._helper,this.instance.currentItem.remove(),this.instance.placeholder&&this.instance.placeholder.remove(),d._trigger("fromSortable",b),d.dropped=!1)})}}),a.ui.plugin.add("draggable","cursor",{start:function(){var b=a("body"),c=a(this).data("ui-draggable").options;b.css("cursor")&&(c._cursor=b.css("cursor")),b.css("cursor",c.cursor)},stop:function(){var b=a(this).data("ui-draggable").options;b._cursor&&a("body").css("cursor",b._cursor)}}),a.ui.plugin.add("draggable","opacity",{start:function(b,c){var d=a(c.helper),e=a(this).data("ui-draggable").options;d.css("opacity")&&(e._opacity=d.css("opacity")),d.css("opacity",e.opacity)},stop:function(b,c){var d=a(this).data("ui-draggable").options;d._opacity&&a(c.helper).css("opacity",d._opacity)}}),a.ui.plugin.add("draggable","scroll",{start:function(){var b=a(this).data("ui-draggable");b.scrollParent[0]!==document&&"HTML"!==b.scrollParent[0].tagName&&(b.overflowOffset=b.scrollParent.offset())},drag:function(b){var c=a(this).data("ui-draggable"),d=c.options,e=!1;c.scrollParent[0]!==document&&"HTML"!==c.scrollParent[0].tagName?(d.axis&&"x"===d.axis||(c.overflowOffset.top+c.scrollParent[0].offsetHeight-b.pageY<d.scrollSensitivity?c.scrollParent[0].scrollTop=e=c.scrollParent[0].scrollTop+d.scrollSpeed:b.pageY-c.overflowOffset.top<d.scrollSensitivity&&(c.scrollParent[0].scrollTop=e=c.scrollParent[0].scrollTop-d.scrollSpeed)),d.axis&&"y"===d.axis||(c.overflowOffset.left+c.scrollParent[0].offsetWidth-b.pageX<d.scrollSensitivity?c.scrollParent[0].scrollLeft=e=c.scrollParent[0].scrollLeft+d.scrollSpeed:b.pageX-c.overflowOffset.left<d.scrollSensitivity&&(c.scrollParent[0].scrollLeft=e=c.scrollParent[0].scrollLeft-d.scrollSpeed))):(d.axis&&"x"===d.axis||(b.pageY-a(document).scrollTop()<d.scrollSensitivity?e=a(document).scrollTop(a(document).scrollTop()-d.scrollSpeed):a(window).height()-(b.pageY-a(document).scrollTop())<d.scrollSensitivity&&(e=a(document).scrollTop(a(document).scrollTop()+d.scrollSpeed))),d.axis&&"y"===d.axis||(b.pageX-a(document).scrollLeft()<d.scrollSensitivity?e=a(document).scrollLeft(a(document).scrollLeft()-d.scrollSpeed):a(window).width()-(b.pageX-a(document).scrollLeft())<d.scrollSensitivity&&(e=a(document).scrollLeft(a(document).scrollLeft()+d.scrollSpeed)))),!1!==e&&a.ui.ddmanager&&!d.dropBehaviour&&a.ui.ddmanager.prepareOffsets(c,b)}}),a.ui.plugin.add("draggable","snap",{start:function(){var b=a(this).data("ui-draggable"),c=b.options;b.snapElements=[],a(c.snap.constructor!==String?c.snap.items||":data(ui-draggable)":c.snap).each(function(){var c=a(this),d=c.offset();this!==b.element[0]&&b.snapElements.push({item:this,width:c.outerWidth(),height:c.outerHeight(),top:d.top,left:d.left})})},drag:function(b,c){var d,e,f,g,h,i,j,k,l,m,n=a(this).data("ui-draggable"),o=n.options,p=o.snapTolerance,q=c.offset.left,r=q+n.helperProportions.width,s=c.offset.top,t=s+n.helperProportions.height;for(l=n.snapElements.length-1;l>=0;l--)h=n.snapElements[l].left,i=h+n.snapElements[l].width,j=n.snapElements[l].top,k=j+n.snapElements[l].height,h-p<q&&q<i+p&&j-p<s&&s<k+p||h-p<q&&q<i+p&&j-p<t&&t<k+p||h-p<r&&r<i+p&&j-p<s&&s<k+p||h-p<r&&r<i+p&&j-p<t&&t<k+p?("inner"!==o.snapMode&&(d=Math.abs(j-t)<=p,e=Math.abs(k-s)<=p,f=Math.abs(h-r)<=p,g=Math.abs(i-q)<=p,d&&(c.position.top=n._convertPositionTo("relative",{top:j-n.helperProportions.height,left:0}).top-n.margins.top),e&&(c.position.top=n._convertPositionTo("relative",{top:k,left:0}).top-n.margins.top),f&&(c.position.left=n._convertPositionTo("relative",{top:0,left:h-n.helperProportions.width}).left-n.margins.left),g&&(c.position.left=n._convertPositionTo("relative",{top:0,left:i}).left-n.margins.left)),m=d||e||f||g,"outer"!==o.snapMode&&(d=Math.abs(j-s)<=p,e=Math.abs(k-t)<=p,f=Math.abs(h-q)<=p,g=Math.abs(i-r)<=p,d&&(c.position.top=n._convertPositionTo("relative",{top:j,left:0}).top-n.margins.top),e&&(c.position.top=n._convertPositionTo("relative",{top:k-n.helperProportions.height,left:0}).top-n.margins.top),f&&(c.position.left=n._convertPositionTo("relative",{top:0,left:h}).left-n.margins.left),g&&(c.position.left=n._convertPositionTo("relative",{top:0,left:i-n.helperProportions.width}).left-n.margins.left)),!n.snapElements[l].snapping&&(d||e||f||g||m)&&n.options.snap.snap&&n.options.snap.snap.call(n.element,b,a.extend(n._uiHash(),{snapItem:n.snapElements[l].item})),n.snapElements[l].snapping=d||e||f||g||m):(n.snapElements[l].snapping&&n.options.snap.release&&n.options.snap.release.call(n.element,b,a.extend(n._uiHash(),{snapItem:n.snapElements[l].item})),n.snapElements[l].snapping=!1)}}),a.ui.plugin.add("draggable","stack",{start:function(){var b,c=this.data("ui-draggable").options,d=a.makeArray(a(c.stack)).sort(function(b,c){return(parseInt(a(b).css("zIndex"),10)||0)-(parseInt(a(c).css("zIndex"),10)||0)});d.length&&(b=parseInt(a(d[0]).css("zIndex"),10)||0,a(d).each(function(c){a(this).css("zIndex",b+c)}),this.css("zIndex",b+d.length))}}),a.ui.plugin.add("draggable","zIndex",{start:function(b,c){var d=a(c.helper),e=a(this).data("ui-draggable").options;d.css("zIndex")&&(e._zIndex=d.css("zIndex")),d.css("zIndex",e.zIndex)},stop:function(b,c){var d=a(this).data("ui-draggable").options;d._zIndex&&a(c.helper).css("zIndex",d._zIndex)}})}(jQuery),function(a,b){function c(a,b,c){return a>b&&a<b+c}a.widget("ui.droppable",{version:"1.10.2",widgetEventPrefix:"drop",options:{accept:"*",activeClass:!1,addClasses:!0,greedy:!1,hoverClass:!1,scope:"default",tolerance:"intersect",activate:null,deactivate:null,drop:null,out:null,over:null},_create:function(){var b=this.options,c=b.accept;this.isover=!1,this.isout=!0,this.accept=a.isFunction(c)?c:function(a){return a.is(c)},this.proportions={width:this.element[0].offsetWidth,height:this.element[0].offsetHeight},a.ui.ddmanager.droppables[b.scope]=a.ui.ddmanager.droppables[b.scope]||[],a.ui.ddmanager.droppables[b.scope].push(this),b.addClasses&&this.element.addClass("ui-droppable")},_destroy:function(){for(var b=0,c=a.ui.ddmanager.droppables[this.options.scope];b<c.length;b++)c[b]===this&&c.splice(b,1);this.element.removeClass("ui-droppable ui-droppable-disabled")},_setOption:function(b,c){"accept"===b&&(this.accept=a.isFunction(c)?c:function(a){return a.is(c)}),a.Widget.prototype._setOption.apply(this,arguments)},_activate:function(b){var c=a.ui.ddmanager.current;this.options.activeClass&&this.element.addClass(this.options.activeClass),c&&this._trigger("activate",b,this.ui(c))},_deactivate:function(b){var c=a.ui.ddmanager.current;this.options.activeClass&&this.element.removeClass(this.options.activeClass),c&&this._trigger("deactivate",b,this.ui(c))},_over:function(b){var c=a.ui.ddmanager.current;c&&(c.currentItem||c.element)[0]!==this.element[0]&&this.accept.call(this.element[0],c.currentItem||c.element)&&(this.options.hoverClass&&this.element.addClass(this.options.hoverClass),this._trigger("over",b,this.ui(c)))},_out:function(b){var c=a.ui.ddmanager.current;c&&(c.currentItem||c.element)[0]!==this.element[0]&&this.accept.call(this.element[0],c.currentItem||c.element)&&(this.options.hoverClass&&this.element.removeClass(this.options.hoverClass),this._trigger("out",b,this.ui(c)))},_drop:function(b,c){var d=c||a.ui.ddmanager.current,e=!1;return!(!d||(d.currentItem||d.element)[0]===this.element[0])&&(this.element.find(":data(ui-droppable)").not(".ui-draggable-dragging").each(function(){var b=a.data(this,"ui-droppable");if(b.options.greedy&&!b.options.disabled&&b.options.scope===d.options.scope&&b.accept.call(b.element[0],d.currentItem||d.element)&&a.ui.intersect(d,a.extend(b,{offset:b.element.offset()}),b.options.tolerance))return e=!0,!1}),!e&&(!!this.accept.call(this.element[0],d.currentItem||d.element)&&(this.options.activeClass&&this.element.removeClass(this.options.activeClass),this.options.hoverClass&&this.element.removeClass(this.options.hoverClass),this._trigger("drop",b,this.ui(d)),this.element)))},ui:function(a){return{draggable:a.currentItem||a.element,helper:a.helper,position:a.position,offset:a.positionAbs}}}),a.ui.intersect=function(a,b,d){if(!b.offset)return!1;var e,f,g=(a.positionAbs||a.position.absolute).left,h=g+a.helperProportions.width,i=(a.positionAbs||a.position.absolute).top,j=i+a.helperProportions.height,k=b.offset.left,l=k+b.proportions.width,m=b.offset.top,n=m+b.proportions.height;switch(d){case"fit":return k<=g&&h<=l&&m<=i&&j<=n;case"intersect":return k<g+a.helperProportions.width/2&&h-a.helperProportions.width/2<l&&m<i+a.helperProportions.height/2&&j-a.helperProportions.height/2<n;case"pointer":return e=(a.positionAbs||a.position.absolute).left+(a.clickOffset||a.offset.click).left,f=(a.positionAbs||a.position.absolute).top+(a.clickOffset||a.offset.click).top,c(f,m,b.proportions.height)&&c(e,k,b.proportions.width);case"touch":return(i>=m&&i<=n||j>=m&&j<=n||i<m&&j>n)&&(g>=k&&g<=l||h>=k&&h<=l||g<k&&h>l);default:return!1}},a.ui.ddmanager={current:null,droppables:{default:[]},prepareOffsets:function(b,c){var d,e,f=a.ui.ddmanager.droppables[b.options.scope]||[],g=c?c.type:null,h=(b.currentItem||b.element).find(":data(ui-droppable)").addBack();a:for(d=0;d<f.length;d++)if(!(f[d].options.disabled||b&&!f[d].accept.call(f[d].element[0],b.currentItem||b.element))){for(e=0;e<h.length;e++)if(h[e]===f[d].element[0]){f[d].proportions.height=0;continue a}f[d].visible="none"!==f[d].element.css("display"),f[d].visible&&("mousedown"===g&&f[d]._activate.call(f[d],c),f[d].offset=f[d].element.offset(),f[d].proportions={width:f[d].element[0].offsetWidth,height:f[d].element[0].offsetHeight})}},drop:function(b,c){var d=!1;return a.each((a.ui.ddmanager.droppables[b.options.scope]||[]).slice(),function(){this.options&&(!this.options.disabled&&this.visible&&a.ui.intersect(b,this,this.options.tolerance)&&(d=this._drop.call(this,c)||d),!this.options.disabled&&this.visible&&this.accept.call(this.element[0],b.currentItem||b.element)&&(this.isout=!0,this.isover=!1,this._deactivate.call(this,c)))}),d},dragStart:function(b,c){b.element.parentsUntil("body").bind("scroll.droppable",function(){b.options.refreshPositions||a.ui.ddmanager.prepareOffsets(b,c)})},drag:function(b,c){b.options.refreshPositions&&a.ui.ddmanager.prepareOffsets(b,c),a.each(a.ui.ddmanager.droppables[b.options.scope]||[],function(){if(!this.options.disabled&&!this.greedyChild&&this.visible){var d,e,f,g=a.ui.intersect(b,this,this.options.tolerance),h=!g&&this.isover?"isout":g&&!this.isover?"isover":null;h&&(this.options.greedy&&(e=this.options.scope,f=this.element.parents(":data(ui-droppable)").filter(function(){return a.data(this,"ui-droppable").options.scope===e}),f.length&&(d=a.data(f[0],"ui-droppable"),d.greedyChild="isover"===h)),d&&"isover"===h&&(d.isover=!1,d.isout=!0,d._out.call(d,c)),this[h]=!0,this["isout"===h?"isover":"isout"]=!1,this["isover"===h?"_over":"_out"].call(this,c),d&&"isout"===h&&(d.isout=!1,d.isover=!0,d._over.call(d,c)))}})},dragStop:function(b,c){b.element.parentsUntil("body").unbind("scroll.droppable"),b.options.refreshPositions||a.ui.ddmanager.prepareOffsets(b,c)}}}(jQuery),function(a,b){function c(a){return parseInt(a,10)||0}function d(a){return!isNaN(parseInt(a,10))}a.widget("ui.resizable",a.ui.mouse,{version:"1.10.2",widgetEventPrefix:"resize",options:{alsoResize:!1,animate:!1,animateDuration:"slow",animateEasing:"swing",aspectRatio:!1,autoHide:!1,containment:!1,ghost:!1,grid:!1,handles:"e,s,se",helper:!1,maxHeight:null,maxWidth:null,minHeight:10,minWidth:10,zIndex:90,resize:null,start:null,stop:null},_create:function(){var b,c,d,e,f,g=this,h=this.options;if(this.element.addClass("ui-resizable"),a.extend(this,{_aspectRatio:!!h.aspectRatio,aspectRatio:h.aspectRatio,originalElement:this.element,_proportionallyResizeElements:[],_helper:h.helper||h.ghost||h.animate?h.helper||"ui-resizable-helper":null}),this.element[0].nodeName.match(/canvas|textarea|input|select|button|img/i)&&(this.element.wrap(a("<div class='ui-wrapper' style='overflow: hidden;'></div>").css({position:this.element.css("position"),width:this.element.outerWidth(),height:this.element.outerHeight(),top:this.element.css("top"),left:this.element.css("left")})),this.element=this.element.parent().data("ui-resizable",this.element.data("ui-resizable")),this.elementIsWrapper=!0,this.element.css({marginLeft:this.originalElement.css("marginLeft"),marginTop:this.originalElement.css("marginTop"),marginRight:this.originalElement.css("marginRight"),marginBottom:this.originalElement.css("marginBottom")}),this.originalElement.css({marginLeft:0,marginTop:0,marginRight:0,marginBottom:0}),this.originalResizeStyle=this.originalElement.css("resize"),this.originalElement.css("resize","none"),this._proportionallyResizeElements.push(this.originalElement.css({position:"static",zoom:1,display:"block"})),this.originalElement.css({margin:this.originalElement.css("margin")}),this._proportionallyResize()),this.handles=h.handles||(a(".ui-resizable-handle",this.element).length?{n:".ui-resizable-n",e:".ui-resizable-e",s:".ui-resizable-s",w:".ui-resizable-w",se:".ui-resizable-se",sw:".ui-resizable-sw",ne:".ui-resizable-ne",nw:".ui-resizable-nw"}:"e,s,se"),this.handles.constructor===String)for("all"===this.handles&&(this.handles="n,e,s,w,se,sw,ne,nw"),b=this.handles.split(","),this.handles={},c=0;c<b.length;c++)d=a.trim(b[c]),f="ui-resizable-"+d,e=a("<div class='ui-resizable-handle "+f+"'></div>"),e.css({zIndex:h.zIndex}),"se"===d&&e.addClass("ui-icon ui-icon-gripsmall-diagonal-se"),this.handles[d]=".ui-resizable-"+d,this.element.append(e);this._renderAxis=function(b){var c,d,e,f;b=b||this.element;for(c in this.handles)this.handles[c].constructor===String&&(this.handles[c]=a(this.handles[c],this.element).show()),this.elementIsWrapper&&this.originalElement[0].nodeName.match(/textarea|input|select|button/i)&&(d=a(this.handles[c],this.element),f=/sw|ne|nw|se|n|s/.test(c)?d.outerHeight():d.outerWidth(),e=["padding",/ne|nw|n/.test(c)?"Top":/se|sw|s/.test(c)?"Bottom":/^e$/.test(c)?"Right":"Left"].join(""),b.css(e,f),this._proportionallyResize()),a(this.handles[c]).length},this._renderAxis(this.element),this._handles=a(".ui-resizable-handle",this.element).disableSelection(),this._handles.mouseover(function(){g.resizing||(this.className&&(e=this.className.match(/ui-resizable-(se|sw|ne|nw|n|e|s|w)/i)),g.axis=e&&e[1]?e[1]:"se")}),h.autoHide&&(this._handles.hide(),a(this.element).addClass("ui-resizable-autohide").mouseenter(function(){h.disabled||(a(this).removeClass("ui-resizable-autohide"),g._handles.show())}).mouseleave(function(){h.disabled||g.resizing||(a(this).addClass("ui-resizable-autohide"),g._handles.hide())})),this._mouseInit()},_destroy:function(){this._mouseDestroy();var b,c=function(b){a(b).removeClass("ui-resizable ui-resizable-disabled ui-resizable-resizing").removeData("resizable").removeData("ui-resizable").unbind(".resizable").find(".ui-resizable-handle").remove()};return this.elementIsWrapper&&(c(this.element),b=this.element,this.originalElement.css({position:b.css("position"),width:b.outerWidth(),height:b.outerHeight(),top:b.css("top"),left:b.css("left")}).insertAfter(b),b.remove()),this.originalElement.css("resize",this.originalResizeStyle),c(this.originalElement),this},_mouseCapture:function(b){var c,d,e=!1;for(c in this.handles)((d=a(this.handles[c])[0])===b.target||a.contains(d,b.target))&&(e=!0);return!this.options.disabled&&e},_mouseStart:function(b){var d,e,f,g=this.options,h=this.element.position(),i=this.element;return this.resizing=!0,/absolute/.test(i.css("position"))?i.css({position:"absolute",top:i.css("top"),left:i.css("left")}):i.is(".ui-draggable")&&i.css({position:"absolute",top:h.top,left:h.left}),this._renderProxy(),d=c(this.helper.css("left")),e=c(this.helper.css("top")),g.containment&&(d+=a(g.containment).scrollLeft()||0,e+=a(g.containment).scrollTop()||0),this.offset=this.helper.offset(),this.position={left:d,top:e},this.size=this._helper?{width:i.outerWidth(),height:i.outerHeight()}:{width:i.width(),height:i.height()},this.originalSize=this._helper?{width:i.outerWidth(),height:i.outerHeight()}:{width:i.width(),height:i.height()},this.originalPosition={left:d,top:e},this.sizeDiff={width:i.outerWidth()-i.width(),height:i.outerHeight()-i.height()},this.originalMousePosition={left:b.pageX,top:b.pageY},this.aspectRatio="number"==typeof g.aspectRatio?g.aspectRatio:this.originalSize.width/this.originalSize.height||1,f=a(".ui-resizable-"+this.axis).css("cursor"),a("body").css("cursor","auto"===f?this.axis+"-resize":f),i.addClass("ui-resizable-resizing"),this._propagate("start",b),!0},_mouseDrag:function(b){var c,d=this.helper,e={},f=this.originalMousePosition,g=this.axis,h=this.position.top,i=this.position.left,j=this.size.width,k=this.size.height,l=b.pageX-f.left||0,m=b.pageY-f.top||0,n=this._change[g];return!!n&&(c=n.apply(this,[b,l,m]),this._updateVirtualBoundaries(b.shiftKey),(this._aspectRatio||b.shiftKey)&&(c=this._updateRatio(c,b)),c=this._respectSize(c,b),this._updateCache(c),this._propagate("resize",b),this.position.top!==h&&(e.top=this.position.top+"px"),this.position.left!==i&&(e.left=this.position.left+"px"),this.size.width!==j&&(e.width=this.size.width+"px"),this.size.height!==k&&(e.height=this.size.height+"px"),d.css(e),!this._helper&&this._proportionallyResizeElements.length&&this._proportionallyResize(),a.isEmptyObject(e)||this._trigger("resize",b,this.ui()),!1)},_mouseStop:function(b){this.resizing=!1;var c,d,e,f,g,h,i,j=this.options,k=this;return this._helper&&(c=this._proportionallyResizeElements,d=c.length&&/textarea/i.test(c[0].nodeName),e=d&&a.ui.hasScroll(c[0],"left")?0:k.sizeDiff.height,f=d?0:k.sizeDiff.width,g={width:k.helper.width()-f,height:k.helper.height()-e},h=parseInt(k.element.css("left"),10)+(k.position.left-k.originalPosition.left)||null,i=parseInt(k.element.css("top"),10)+(k.position.top-k.originalPosition.top)||null,j.animate||this.element.css(a.extend(g,{top:i,left:h})),k.helper.height(k.size.height),k.helper.width(k.size.width),this._helper&&!j.animate&&this._proportionallyResize()),a("body").css("cursor","auto"),this.element.removeClass("ui-resizable-resizing"),this._propagate("stop",b),this._helper&&this.helper.remove(),!1},_updateVirtualBoundaries:function(a){var b,c,e,f,g,h=this.options;g={minWidth:d(h.minWidth)?h.minWidth:0,maxWidth:d(h.maxWidth)?h.maxWidth:1/0,minHeight:d(h.minHeight)?h.minHeight:0,maxHeight:d(h.maxHeight)?h.maxHeight:1/0},(this._aspectRatio||a)&&(b=g.minHeight*this.aspectRatio,e=g.minWidth/this.aspectRatio,c=g.maxHeight*this.aspectRatio,f=g.maxWidth/this.aspectRatio,b>g.minWidth&&(g.minWidth=b),e>g.minHeight&&(g.minHeight=e),c<g.maxWidth&&(g.maxWidth=c),f<g.maxHeight&&(g.maxHeight=f)),this._vBoundaries=g},_updateCache:function(a){this.offset=this.helper.offset(),d(a.left)&&(this.position.left=a.left),d(a.top)&&(this.position.top=a.top),d(a.height)&&(this.size.height=a.height),d(a.width)&&(this.size.width=a.width)},_updateRatio:function(a){var b=this.position,c=this.size,e=this.axis;return d(a.height)?a.width=a.height*this.aspectRatio:d(a.width)&&(a.height=a.width/this.aspectRatio),"sw"===e&&(a.left=b.left+(c.width-a.width),a.top=null),"nw"===e&&(a.top=b.top+(c.height-a.height),a.left=b.left+(c.width-a.width)),a},_respectSize:function(a){var b=this._vBoundaries,c=this.axis,e=d(a.width)&&b.maxWidth&&b.maxWidth<a.width,f=d(a.height)&&b.maxHeight&&b.maxHeight<a.height,g=d(a.width)&&b.minWidth&&b.minWidth>a.width,h=d(a.height)&&b.minHeight&&b.minHeight>a.height,i=this.originalPosition.left+this.originalSize.width,j=this.position.top+this.size.height,k=/sw|nw|w/.test(c),l=/nw|ne|n/.test(c);return g&&(a.width=b.minWidth),h&&(a.height=b.minHeight),e&&(a.width=b.maxWidth),f&&(a.height=b.maxHeight),g&&k&&(a.left=i-b.minWidth),e&&k&&(a.left=i-b.maxWidth),h&&l&&(a.top=j-b.minHeight),f&&l&&(a.top=j-b.maxHeight),a.width||a.height||a.left||!a.top?a.width||a.height||a.top||!a.left||(a.left=null):a.top=null,a},_proportionallyResize:function(){if(this._proportionallyResizeElements.length){var a,b,c,d,e,f=this.helper||this.element;for(a=0;a<this._proportionallyResizeElements.length;a++){if(e=this._proportionallyResizeElements[a],!this.borderDif)for(this.borderDif=[],c=[e.css("borderTopWidth"),e.css("borderRightWidth"),e.css("borderBottomWidth"),e.css("borderLeftWidth")],d=[e.css("paddingTop"),e.css("paddingRight"),e.css("paddingBottom"),e.css("paddingLeft")],b=0;b<c.length;b++)this.borderDif[b]=(parseInt(c[b],10)||0)+(parseInt(d[b],10)||0);e.css({height:f.height()-this.borderDif[0]-this.borderDif[2]||0,width:f.width()-this.borderDif[1]-this.borderDif[3]||0})}}},_renderProxy:function(){var b=this.element,c=this.options;this.elementOffset=b.offset(),this._helper?(this.helper=this.helper||a("<div style='overflow:hidden;'></div>"),this.helper.addClass(this._helper).css({width:this.element.outerWidth()-1,height:this.element.outerHeight()-1,position:"absolute",left:this.elementOffset.left+"px",top:this.elementOffset.top+"px",zIndex:++c.zIndex}),this.helper.appendTo("body").disableSelection()):this.helper=this.element},_change:{e:function(a,b){return{width:this.originalSize.width+b}},w:function(a,b){var c=this.originalSize;return{left:this.originalPosition.left+b,width:c.width-b}},n:function(a,b,c){var d=this.originalSize;return{top:this.originalPosition.top+c,height:d.height-c}},s:function(a,b,c){return{height:this.originalSize.height+c}},se:function(b,c,d){return a.extend(this._change.s.apply(this,arguments),this._change.e.apply(this,[b,c,d]))},sw:function(b,c,d){return a.extend(this._change.s.apply(this,arguments),this._change.w.apply(this,[b,c,d]))},ne:function(b,c,d){return a.extend(this._change.n.apply(this,arguments),this._change.e.apply(this,[b,c,d]))},nw:function(b,c,d){return a.extend(this._change.n.apply(this,arguments),this._change.w.apply(this,[b,c,d]))}},_propagate:function(b,c){a.ui.plugin.call(this,b,[c,this.ui()]),"resize"!==b&&this._trigger(b,c,this.ui())},plugins:{},ui:function(){return{originalElement:this.originalElement,element:this.element,helper:this.helper,position:this.position,size:this.size,originalSize:this.originalSize,originalPosition:this.originalPosition}}}),a.ui.plugin.add("resizable","animate",{stop:function(b){var c=a(this).data("ui-resizable"),d=c.options,e=c._proportionallyResizeElements,f=e.length&&/textarea/i.test(e[0].nodeName),g=f&&a.ui.hasScroll(e[0],"left")?0:c.sizeDiff.height,h=f?0:c.sizeDiff.width,i={width:c.size.width-h,height:c.size.height-g},j=parseInt(c.element.css("left"),10)+(c.position.left-c.originalPosition.left)||null,k=parseInt(c.element.css("top"),10)+(c.position.top-c.originalPosition.top)||null;c.element.animate(a.extend(i,k&&j?{top:k,left:j}:{}),{duration:d.animateDuration,easing:d.animateEasing,step:function(){var d={width:parseInt(c.element.css("width"),10),height:parseInt(c.element.css("height"),10),top:parseInt(c.element.css("top"),10),left:parseInt(c.element.css("left"),10)};e&&e.length&&a(e[0]).css({width:d.width,height:d.height}),c._updateCache(d),c._propagate("resize",b)}})}}),a.ui.plugin.add("resizable","containment",{start:function(){var b,d,e,f,g,h,i,j=a(this).data("ui-resizable"),k=j.options,l=j.element,m=k.containment,n=m instanceof a?m.get(0):/parent/.test(m)?l.parent().get(0):m;n&&(j.containerElement=a(n),/document/.test(m)||m===document?(j.containerOffset={left:0,top:0},j.containerPosition={left:0,top:0},j.parentData={element:a(document),left:0,top:0,width:a(document).width(),height:a(document).height()||document.body.parentNode.scrollHeight}):(b=a(n),d=[],a(["Top","Right","Left","Bottom"]).each(function(a,e){d[a]=c(b.css("padding"+e))}),j.containerOffset=b.offset(),j.containerPosition=b.position(),j.containerSize={height:b.innerHeight()-d[3],width:b.innerWidth()-d[1]},e=j.containerOffset,f=j.containerSize.height,g=j.containerSize.width,h=a.ui.hasScroll(n,"left")?n.scrollWidth:g,i=a.ui.hasScroll(n)?n.scrollHeight:f,j.parentData={element:n,left:e.left,top:e.top,width:h,height:i}))},resize:function(b){var c,d,e,f,g=a(this).data("ui-resizable"),h=g.options,i=g.containerOffset,j=g.position,k=g._aspectRatio||b.shiftKey,l={top:0,left:0},m=g.containerElement;m[0]!==document&&/static/.test(m.css("position"))&&(l=i),j.left<(g._helper?i.left:0)&&(g.size.width=g.size.width+(g._helper?g.position.left-i.left:g.position.left-l.left),k&&(g.size.height=g.size.width/g.aspectRatio),g.position.left=h.helper?i.left:0),j.top<(g._helper?i.top:0)&&(g.size.height=g.size.height+(g._helper?g.position.top-i.top:g.position.top),k&&(g.size.width=g.size.height*g.aspectRatio),g.position.top=g._helper?i.top:0),g.offset.left=g.parentData.left+g.position.left,g.offset.top=g.parentData.top+g.position.top,c=Math.abs((g._helper,g.offset.left-l.left+g.sizeDiff.width)),d=Math.abs((g._helper?g.offset.top-l.top:g.offset.top-i.top)+g.sizeDiff.height),e=g.containerElement.get(0)===g.element.parent().get(0),f=/relative|absolute/.test(g.containerElement.css("position")),e&&f&&(c-=g.parentData.left),c+g.size.width>=g.parentData.width&&(g.size.width=g.parentData.width-c,k&&(g.size.height=g.size.width/g.aspectRatio)),d+g.size.height>=g.parentData.height&&(g.size.height=g.parentData.height-d,k&&(g.size.width=g.size.height*g.aspectRatio))},stop:function(){var b=a(this).data("ui-resizable"),c=b.options,d=b.containerOffset,e=b.containerPosition,f=b.containerElement,g=a(b.helper),h=g.offset(),i=g.outerWidth()-b.sizeDiff.width,j=g.outerHeight()-b.sizeDiff.height;b._helper&&!c.animate&&/relative/.test(f.css("position"))&&a(this).css({left:h.left-e.left-d.left,width:i,height:j}),b._helper&&!c.animate&&/static/.test(f.css("position"))&&a(this).css({left:h.left-e.left-d.left,width:i,height:j})}}),a.ui.plugin.add("resizable","alsoResize",{start:function(){var b=a(this).data("ui-resizable"),c=b.options,d=function(b){a(b).each(function(){var b=a(this);b.data("ui-resizable-alsoresize",{width:parseInt(b.width(),10),height:parseInt(b.height(),10),left:parseInt(b.css("left"),10),top:parseInt(b.css("top"),10)})})};"object"!=typeof c.alsoResize||c.alsoResize.parentNode?d(c.alsoResize):c.alsoResize.length?(c.alsoResize=c.alsoResize[0],d(c.alsoResize)):a.each(c.alsoResize,function(a){d(a)})},resize:function(b,c){var d=a(this).data("ui-resizable"),e=d.options,f=d.originalSize,g=d.originalPosition,h={height:d.size.height-f.height||0,width:d.size.width-f.width||0,top:d.position.top-g.top||0,left:d.position.left-g.left||0},i=function(b,d){a(b).each(function(){var b=a(this),e=a(this).data("ui-resizable-alsoresize"),f={},g=d&&d.length?d:b.parents(c.originalElement[0]).length?["width","height"]:["width","height","top","left"];a.each(g,function(a,b){var c=(e[b]||0)+(h[b]||0);c&&c>=0&&(f[b]=c||null)}),b.css(f)})};"object"!=typeof e.alsoResize||e.alsoResize.nodeType?i(e.alsoResize):a.each(e.alsoResize,function(a,b){i(a,b)})},stop:function(){a(this).removeData("resizable-alsoresize")}}),a.ui.plugin.add("resizable","ghost",{start:function(){var b=a(this).data("ui-resizable"),c=b.options,d=b.size;b.ghost=b.originalElement.clone(),b.ghost.css({opacity:.25,display:"block",position:"relative",height:d.height,width:d.width,margin:0,left:0,top:0}).addClass("ui-resizable-ghost").addClass("string"==typeof c.ghost?c.ghost:""),b.ghost.appendTo(b.helper)},resize:function(){var b=a(this).data("ui-resizable");b.ghost&&b.ghost.css({position:"relative",height:b.size.height,width:b.size.width})},stop:function(){var b=a(this).data("ui-resizable");b.ghost&&b.helper&&b.helper.get(0).removeChild(b.ghost.get(0))}}),a.ui.plugin.add("resizable","grid",{resize:function(){var b=a(this).data("ui-resizable"),c=b.options,d=b.size,e=b.originalSize,f=b.originalPosition,g=b.axis,h="number"==typeof c.grid?[c.grid,c.grid]:c.grid,i=h[0]||1,j=h[1]||1,k=Math.round((d.width-e.width)/i)*i,l=Math.round((d.height-e.height)/j)*j,m=e.width+k,n=e.height+l,o=c.maxWidth&&c.maxWidth<m,p=c.maxHeight&&c.maxHeight<n,q=c.minWidth&&c.minWidth>m,r=c.minHeight&&c.minHeight>n;c.grid=h,q&&(m+=i),r&&(n+=j),o&&(m-=i),p&&(n-=j),/^(se|s|e)$/.test(g)?(b.size.width=m,b.size.height=n):/^(ne)$/.test(g)?(b.size.width=m,b.size.height=n,b.position.top=f.top-l):/^(sw)$/.test(g)?(b.size.width=m,b.size.height=n,b.position.left=f.left-k):(b.size.width=m,b.size.height=n,b.position.top=f.top-l,b.position.left=f.left-k)}})}(jQuery),function(a,b){a.widget("ui.selectable",a.ui.mouse,{version:"1.10.2",options:{appendTo:"body",autoRefresh:!0,distance:0,filter:"*",tolerance:"touch",selected:null,selecting:null,start:null,stop:null,unselected:null,unselecting:null},_create:function(){var b,c=this;this.element.addClass("ui-selectable"),this.dragged=!1,this.refresh=function(){b=a(c.options.filter,c.element[0]),b.addClass("ui-selectee"),b.each(function(){var b=a(this),c=b.offset();a.data(this,"selectable-item",{element:this,$element:b,left:c.left,top:c.top,right:c.left+b.outerWidth(),bottom:c.top+b.outerHeight(),startselected:!1,selected:b.hasClass("ui-selected"),selecting:b.hasClass("ui-selecting"),unselecting:b.hasClass("ui-unselecting")})})},this.refresh(),this.selectees=b.addClass("ui-selectee"),this._mouseInit(),this.helper=a("<div class='ui-selectable-helper'></div>")},_destroy:function(){this.selectees.removeClass("ui-selectee").removeData("selectable-item"),this.element.removeClass("ui-selectable ui-selectable-disabled"),this._mouseDestroy()},_mouseStart:function(b){var c=this,d=this.options;this.opos=[b.pageX,b.pageY],this.options.disabled||(this.selectees=a(d.filter,this.element[0]),this._trigger("start",b),a(d.appendTo).append(this.helper),this.helper.css({left:b.pageX,top:b.pageY,width:0,height:0}),d.autoRefresh&&this.refresh(),this.selectees.filter(".ui-selected").each(function(){var d=a.data(this,"selectable-item");d.startselected=!0,b.metaKey||b.ctrlKey||(d.$element.removeClass("ui-selected"),d.selected=!1,d.$element.addClass("ui-unselecting"),d.unselecting=!0,c._trigger("unselecting",b,{unselecting:d.element}))}),a(b.target).parents().addBack().each(function(){var d,e=a.data(this,"selectable-item");if(e)return d=!b.metaKey&&!b.ctrlKey||!e.$element.hasClass("ui-selected"),e.$element.removeClass(d?"ui-unselecting":"ui-selected").addClass(d?"ui-selecting":"ui-unselecting"),e.unselecting=!d,e.selecting=d,e.selected=d,d?c._trigger("selecting",b,{selecting:e.element}):c._trigger("unselecting",b,{unselecting:e.element}),!1}))},_mouseDrag:function(b){if(this.dragged=!0,!this.options.disabled){var c,d=this,e=this.options,f=this.opos[0],g=this.opos[1],h=b.pageX,i=b.pageY;return f>h&&(c=h,h=f,f=c),g>i&&(c=i,i=g,g=c),this.helper.css({left:f,top:g,width:h-f,height:i-g}),this.selectees.each(function(){var c=a.data(this,"selectable-item"),j=!1;c&&c.element!==d.element[0]&&("touch"===e.tolerance?j=!(c.left>h||c.right<f||c.top>i||c.bottom<g):"fit"===e.tolerance&&(j=c.left>f&&c.right<h&&c.top>g&&c.bottom<i),j?(c.selected&&(c.$element.removeClass("ui-selected"),c.selected=!1),c.unselecting&&(c.$element.removeClass("ui-unselecting"),c.unselecting=!1),c.selecting||(c.$element.addClass("ui-selecting"),c.selecting=!0,d._trigger("selecting",b,{selecting:c.element}))):(c.selecting&&((b.metaKey||b.ctrlKey)&&c.startselected?(c.$element.removeClass("ui-selecting"),c.selecting=!1,c.$element.addClass("ui-selected"),c.selected=!0):(c.$element.removeClass("ui-selecting"),c.selecting=!1,c.startselected&&(c.$element.addClass("ui-unselecting"),c.unselecting=!0),d._trigger("unselecting",b,{unselecting:c.element}))),c.selected&&(b.metaKey||b.ctrlKey||c.startselected||(c.$element.removeClass("ui-selected"),c.selected=!1,c.$element.addClass("ui-unselecting"),c.unselecting=!0,d._trigger("unselecting",b,{unselecting:c.element})))))}),!1}},_mouseStop:function(b){var c=this;return this.dragged=!1,a(".ui-unselecting",this.element[0]).each(function(){var d=a.data(this,"selectable-item");d.$element.removeClass("ui-unselecting"),d.unselecting=!1,d.startselected=!1,c._trigger("unselected",b,{unselected:d.element})}),a(".ui-selecting",this.element[0]).each(function(){var d=a.data(this,"selectable-item");d.$element.removeClass("ui-selecting").addClass("ui-selected"),d.selecting=!1,d.selected=!0,d.startselected=!0,c._trigger("selected",b,{selected:d.element})}),this._trigger("stop",b),this.helper.remove(),!1}})}(jQuery),function(a,b){function c(a,b,c){return a>b&&a<b+c}function d(a){return/left|right/.test(a.css("float"))||/inline|table-cell/.test(a.css("display"))}a.widget("ui.sortable",a.ui.mouse,{version:"1.10.2",widgetEventPrefix:"sort",ready:!1,options:{appendTo:"parent",axis:!1,connectWith:!1,containment:!1,cursor:"auto",cursorAt:!1,dropOnEmpty:!0,forcePlaceholderSize:!1,forceHelperSize:!1,grid:!1,handle:!1,helper:"original",items:"> *",opacity:!1,placeholder:!1,revert:!1,scroll:!0,scrollSensitivity:20,scrollSpeed:20,scope:"default",tolerance:"intersect",zIndex:1e3,activate:null,beforeStop:null,change:null,deactivate:null,out:null,over:null,receive:null,remove:null,sort:null,start:null,stop:null,update:null},_create:function(){var a=this.options;this.containerCache={},this.element.addClass("ui-sortable"),this.refresh(),this.floating=!!this.items.length&&("x"===a.axis||d(this.items[0].item)),this.offset=this.element.offset(),this._mouseInit(),this.ready=!0},_destroy:function(){this.element.removeClass("ui-sortable ui-sortable-disabled"),this._mouseDestroy();for(var a=this.items.length-1;a>=0;a--)this.items[a].item.removeData(this.widgetName+"-item");return this},_setOption:function(b,c){"disabled"===b?(this.options[b]=c,this.widget().toggleClass("ui-sortable-disabled",!!c)):a.Widget.prototype._setOption.apply(this,arguments)},_mouseCapture:function(b,c){var d=null,e=!1,f=this;return!this.reverting&&(!this.options.disabled&&"static"!==this.options.type&&(this._refreshItems(b),a(b.target).parents().each(function(){if(a.data(this,f.widgetName+"-item")===f)return d=a(this),!1}),a.data(b.target,f.widgetName+"-item")===f&&(d=a(b.target)),!!d&&(!(this.options.handle&&!c&&(a(this.options.handle,d).find("*").addBack().each(function(){this===b.target&&(e=!0)}),!e))&&(this.currentItem=d,this._removeCurrentsFromItems(),!0))))},_mouseStart:function(b,c,d){var e,f,g=this.options;if(this.currentContainer=this,this.refreshPositions(),this.helper=this._createHelper(b),this._cacheHelperProportions(),this._cacheMargins(),this.scrollParent=this.helper.scrollParent(),this.offset=this.currentItem.offset(),this.offset={top:this.offset.top-this.margins.top,left:this.offset.left-this.margins.left},a.extend(this.offset,{click:{left:b.pageX-this.offset.left,top:b.pageY-this.offset.top},parent:this._getParentOffset(),relative:this._getRelativeOffset()}),this.helper.css("position","absolute"),this.cssPosition=this.helper.css("position"),this.originalPosition=this._generatePosition(b),this.originalPageX=b.pageX,this.originalPageY=b.pageY,g.cursorAt&&this._adjustOffsetFromHelper(g.cursorAt),this.domPosition={prev:this.currentItem.prev()[0],parent:this.currentItem.parent()[0]},this.helper[0]!==this.currentItem[0]&&this.currentItem.hide(),this._createPlaceholder(),g.containment&&this._setContainment(),g.cursor&&"auto"!==g.cursor&&(f=this.document.find("body"),this.storedCursor=f.css("cursor"),f.css("cursor",g.cursor),this.storedStylesheet=a("<style>*{ cursor: "+g.cursor+" !important; }</style>").appendTo(f)),g.opacity&&(this.helper.css("opacity")&&(this._storedOpacity=this.helper.css("opacity")),this.helper.css("opacity",g.opacity)),g.zIndex&&(this.helper.css("zIndex")&&(this._storedZIndex=this.helper.css("zIndex")),this.helper.css("zIndex",g.zIndex)),this.scrollParent[0]!==document&&"HTML"!==this.scrollParent[0].tagName&&(this.overflowOffset=this.scrollParent.offset()),this._trigger("start",b,this._uiHash()),this._preserveHelperProportions||this._cacheHelperProportions(),!d)for(e=this.containers.length-1;e>=0;e--)this.containers[e]._trigger("activate",b,this._uiHash(this));return a.ui.ddmanager&&(a.ui.ddmanager.current=this),a.ui.ddmanager&&!g.dropBehaviour&&a.ui.ddmanager.prepareOffsets(this,b),this.dragging=!0,this.helper.addClass("ui-sortable-helper"),this._mouseDrag(b),!0},_mouseDrag:function(b){var c,d,e,f,g=this.options,h=!1;for(this.position=this._generatePosition(b),this.positionAbs=this._convertPositionTo("absolute"),this.lastPositionAbs||(this.lastPositionAbs=this.positionAbs),this.options.scroll&&(this.scrollParent[0]!==document&&"HTML"!==this.scrollParent[0].tagName?(this.overflowOffset.top+this.scrollParent[0].offsetHeight-b.pageY<g.scrollSensitivity?this.scrollParent[0].scrollTop=h=this.scrollParent[0].scrollTop+g.scrollSpeed:b.pageY-this.overflowOffset.top<g.scrollSensitivity&&(this.scrollParent[0].scrollTop=h=this.scrollParent[0].scrollTop-g.scrollSpeed),this.overflowOffset.left+this.scrollParent[0].offsetWidth-b.pageX<g.scrollSensitivity?this.scrollParent[0].scrollLeft=h=this.scrollParent[0].scrollLeft+g.scrollSpeed:b.pageX-this.overflowOffset.left<g.scrollSensitivity&&(this.scrollParent[0].scrollLeft=h=this.scrollParent[0].scrollLeft-g.scrollSpeed)):(b.pageY-a(document).scrollTop()<g.scrollSensitivity?h=a(document).scrollTop(a(document).scrollTop()-g.scrollSpeed):a(window).height()-(b.pageY-a(document).scrollTop())<g.scrollSensitivity&&(h=a(document).scrollTop(a(document).scrollTop()+g.scrollSpeed)),b.pageX-a(document).scrollLeft()<g.scrollSensitivity?h=a(document).scrollLeft(a(document).scrollLeft()-g.scrollSpeed):a(window).width()-(b.pageX-a(document).scrollLeft())<g.scrollSensitivity&&(h=a(document).scrollLeft(a(document).scrollLeft()+g.scrollSpeed))),!1!==h&&a.ui.ddmanager&&!g.dropBehaviour&&a.ui.ddmanager.prepareOffsets(this,b)),this.positionAbs=this._convertPositionTo("absolute"),this.options.axis&&"y"===this.options.axis||(this.helper[0].style.left=this.position.left+"px"),this.options.axis&&"x"===this.options.axis||(this.helper[0].style.top=this.position.top+"px"),c=this.items.length-1;c>=0;c--)if(d=this.items[c],e=d.item[0],(f=this._intersectsWithPointer(d))&&d.instance===this.currentContainer&&!(e===this.currentItem[0]||this.placeholder[1===f?"next":"prev"]()[0]===e||a.contains(this.placeholder[0],e)||"semi-dynamic"===this.options.type&&a.contains(this.element[0],e))){if(this.direction=1===f?"down":"up","pointer"!==this.options.tolerance&&!this._intersectsWithSides(d))break;this._rearrange(b,d),this._trigger("change",b,this._uiHash());break}return this._contactContainers(b),a.ui.ddmanager&&a.ui.ddmanager.drag(this,b),this._trigger("sort",b,this._uiHash()),this.lastPositionAbs=this.positionAbs,!1},_mouseStop:function(b,c){if(b){if(a.ui.ddmanager&&!this.options.dropBehaviour&&a.ui.ddmanager.drop(this,b),this.options.revert){var d=this,e=this.placeholder.offset(),f=this.options.axis,g={};f&&"x"!==f||(g.left=e.left-this.offset.parent.left-this.margins.left+(this.offsetParent[0]===document.body?0:this.offsetParent[0].scrollLeft)),f&&"y"!==f||(g.top=e.top-this.offset.parent.top-this.margins.top+(this.offsetParent[0]===document.body?0:this.offsetParent[0].scrollTop)),this.reverting=!0,a(this.helper).animate(g,parseInt(this.options.revert,10)||500,function(){d._clear(b)})}else this._clear(b,c);return!1}},cancel:function(){if(this.dragging){this._mouseUp({target:null}),"original"===this.options.helper?this.currentItem.css(this._storedCSS).removeClass("ui-sortable-helper"):this.currentItem.show();for(var b=this.containers.length-1;b>=0;b--)this.containers[b]._trigger("deactivate",null,this._uiHash(this)),this.containers[b].containerCache.over&&(this.containers[b]._trigger("out",null,this._uiHash(this)),this.containers[b].containerCache.over=0)}return this.placeholder&&(this.placeholder[0].parentNode&&this.placeholder[0].parentNode.removeChild(this.placeholder[0]),"original"!==this.options.helper&&this.helper&&this.helper[0].parentNode&&this.helper.remove(),a.extend(this,{helper:null,dragging:!1,reverting:!1,_noFinalSort:null}),this.domPosition.prev?a(this.domPosition.prev).after(this.currentItem):a(this.domPosition.parent).prepend(this.currentItem)),this},serialize:function(b){var c=this._getItemsAsjQuery(b&&b.connected),d=[];return b=b||{},a(c).each(function(){var c=(a(b.item||this).attr(b.attribute||"id")||"").match(b.expression||/(.+)[\-=_](.+)/);c&&d.push((b.key||c[1]+"[]")+"="+(b.key&&b.expression?c[1]:c[2]))}),!d.length&&b.key&&d.push(b.key+"="),d.join("&")},toArray:function(b){var c=this._getItemsAsjQuery(b&&b.connected),d=[];return b=b||{},c.each(function(){d.push(a(b.item||this).attr(b.attribute||"id")||"")}),d},_intersectsWith:function(a){var b=this.positionAbs.left,c=b+this.helperProportions.width,d=this.positionAbs.top,e=d+this.helperProportions.height,f=a.left,g=f+a.width,h=a.top,i=h+a.height,j=this.offset.click.top,k=this.offset.click.left,l=d+j>h&&d+j<i&&b+k>f&&b+k<g;return"pointer"===this.options.tolerance||this.options.forcePointerForContainers||"pointer"!==this.options.tolerance&&this.helperProportions[this.floating?"width":"height"]>a[this.floating?"width":"height"]?l:f<b+this.helperProportions.width/2&&c-this.helperProportions.width/2<g&&h<d+this.helperProportions.height/2&&e-this.helperProportions.height/2<i},_intersectsWithPointer:function(a){var b="x"===this.options.axis||c(this.positionAbs.top+this.offset.click.top,a.top,a.height),d="y"===this.options.axis||c(this.positionAbs.left+this.offset.click.left,a.left,a.width),e=b&&d,f=this._getDragVerticalDirection(),g=this._getDragHorizontalDirection();return!!e&&(this.floating?g&&"right"===g||"down"===f?2:1:f&&("down"===f?2:1))},_intersectsWithSides:function(a){var b=c(this.positionAbs.top+this.offset.click.top,a.top+a.height/2,a.height),d=c(this.positionAbs.left+this.offset.click.left,a.left+a.width/2,a.width),e=this._getDragVerticalDirection(),f=this._getDragHorizontalDirection();return this.floating&&f?"right"===f&&d||"left"===f&&!d:e&&("down"===e&&b||"up"===e&&!b)},_getDragVerticalDirection:function(){var a=this.positionAbs.top-this.lastPositionAbs.top;return 0!==a&&(a>0?"down":"up")},_getDragHorizontalDirection:function(){var a=this.positionAbs.left-this.lastPositionAbs.left;return 0!==a&&(a>0?"right":"left")},refresh:function(a){return this._refreshItems(a),this.refreshPositions(),this},_connectWith:function(){var a=this.options;return a.connectWith.constructor===String?[a.connectWith]:a.connectWith},_getItemsAsjQuery:function(b){var c,d,e,f,g=[],h=[],i=this._connectWith();if(i&&b)for(c=i.length-1;c>=0;c--)for(e=a(i[c]),d=e.length-1;d>=0;d--)(f=a.data(e[d],this.widgetFullName))&&f!==this&&!f.options.disabled&&h.push([a.isFunction(f.options.items)?f.options.items.call(f.element):a(f.options.items,f.element).not(".ui-sortable-helper").not(".ui-sortable-placeholder"),f]);for(h.push([a.isFunction(this.options.items)?this.options.items.call(this.element,null,{options:this.options,item:this.currentItem}):a(this.options.items,this.element).not(".ui-sortable-helper").not(".ui-sortable-placeholder"),this]),c=h.length-1;c>=0;c--)h[c][0].each(function(){g.push(this)});return a(g)},_removeCurrentsFromItems:function(){var b=this.currentItem.find(":data("+this.widgetName+"-item)");this.items=a.grep(this.items,function(a){for(var c=0;c<b.length;c++)if(b[c]===a.item[0])return!1;return!0})},_refreshItems:function(b){this.items=[],this.containers=[this];var c,d,e,f,g,h,i,j,k=this.items,l=[[a.isFunction(this.options.items)?this.options.items.call(this.element[0],b,{item:this.currentItem}):a(this.options.items,this.element),this]],m=this._connectWith();if(m&&this.ready)for(c=m.length-1;c>=0;c--)for(e=a(m[c]),d=e.length-1;d>=0;d--)(f=a.data(e[d],this.widgetFullName))&&f!==this&&!f.options.disabled&&(l.push([a.isFunction(f.options.items)?f.options.items.call(f.element[0],b,{item:this.currentItem}):a(f.options.items,f.element),f]),this.containers.push(f));for(c=l.length-1;c>=0;c--)for(g=l[c][1],h=l[c][0],d=0,j=h.length;d<j;d++)i=a(h[d]),i.data(this.widgetName+"-item",g),k.push({item:i,instance:g,width:0,height:0,left:0,top:0})},refreshPositions:function(b){this.offsetParent&&this.helper&&(this.offset.parent=this._getParentOffset());var c,d,e,f;for(c=this.items.length-1;c>=0;c--)d=this.items[c],d.instance!==this.currentContainer&&this.currentContainer&&d.item[0]!==this.currentItem[0]||(e=this.options.toleranceElement?a(this.options.toleranceElement,d.item):d.item,b||(d.width=e.outerWidth(),d.height=e.outerHeight()),f=e.offset(),d.left=f.left,d.top=f.top);if(this.options.custom&&this.options.custom.refreshContainers)this.options.custom.refreshContainers.call(this);else for(c=this.containers.length-1;c>=0;c--)f=this.containers[c].element.offset(),this.containers[c].containerCache.left=f.left,this.containers[c].containerCache.top=f.top,this.containers[c].containerCache.width=this.containers[c].element.outerWidth(),this.containers[c].containerCache.height=this.containers[c].element.outerHeight();return this},_createPlaceholder:function(b){b=b||this;var c,d=b.options;d.placeholder&&d.placeholder.constructor!==String||(c=d.placeholder,d.placeholder={element:function(){var d=b.currentItem[0].nodeName.toLowerCase(),e=a(b.document[0].createElement(d)).addClass(c||b.currentItem[0].className+" ui-sortable-placeholder").removeClass("ui-sortable-helper");return"tr"===d?e.append("<td colspan='99'>&#160;</td>"):"img"===d&&e.attr("src",b.currentItem.attr("src")),c||e.css("visibility","hidden"),e},update:function(a,e){c&&!d.forcePlaceholderSize||(e.height()||e.height(b.currentItem.innerHeight()-parseInt(b.currentItem.css("paddingTop")||0,10)-parseInt(b.currentItem.css("paddingBottom")||0,10)),e.width()||e.width(b.currentItem.innerWidth()-parseInt(b.currentItem.css("paddingLeft")||0,10)-parseInt(b.currentItem.css("paddingRight")||0,10)))}}),b.placeholder=a(d.placeholder.element.call(b.element,b.currentItem)),b.currentItem.after(b.placeholder),d.placeholder.update(b,b.placeholder)},_contactContainers:function(b){var e,f,g,h,i,j,k,l,m,n,o=null,p=null;for(e=this.containers.length-1;e>=0;e--)if(!a.contains(this.currentItem[0],this.containers[e].element[0]))if(this._intersectsWith(this.containers[e].containerCache)){if(o&&a.contains(this.containers[e].element[0],o.element[0]))continue;o=this.containers[e],p=e}else this.containers[e].containerCache.over&&(this.containers[e]._trigger("out",b,this._uiHash(this)),this.containers[e].containerCache.over=0);if(o)if(1===this.containers.length)this.containers[p].containerCache.over||(this.containers[p]._trigger("over",b,this._uiHash(this)),this.containers[p].containerCache.over=1);else{for(g=1e4,h=null,n=o.floating||d(this.currentItem),i=n?"left":"top",j=n?"width":"height",k=this.positionAbs[i]+this.offset.click[i],f=this.items.length-1;f>=0;f--)a.contains(this.containers[p].element[0],this.items[f].item[0])&&this.items[f].item[0]!==this.currentItem[0]&&(n&&!c(this.positionAbs.top+this.offset.click.top,this.items[f].top,this.items[f].height)||(l=this.items[f].item.offset()[i],m=!1,Math.abs(l-k)>Math.abs(l+this.items[f][j]-k)&&(m=!0,l+=this.items[f][j]),Math.abs(l-k)<g&&(g=Math.abs(l-k),h=this.items[f],this.direction=m?"up":"down")));if(!h&&!this.options.dropOnEmpty)return;if(this.currentContainer===this.containers[p])return;h?this._rearrange(b,h,null,!0):this._rearrange(b,null,this.containers[p].element,!0),this._trigger("change",b,this._uiHash()),this.containers[p]._trigger("change",b,this._uiHash(this)),this.currentContainer=this.containers[p],this.options.placeholder.update(this.currentContainer,this.placeholder),this.containers[p]._trigger("over",b,this._uiHash(this)),this.containers[p].containerCache.over=1}},_createHelper:function(b){var c=this.options,d=a.isFunction(c.helper)?a(c.helper.apply(this.element[0],[b,this.currentItem])):"clone"===c.helper?this.currentItem.clone():this.currentItem;return d.parents("body").length||a("parent"!==c.appendTo?c.appendTo:this.currentItem[0].parentNode)[0].appendChild(d[0]),d[0]===this.currentItem[0]&&(this._storedCSS={width:this.currentItem[0].style.width,height:this.currentItem[0].style.height,position:this.currentItem.css("position"),top:this.currentItem.css("top"),left:this.currentItem.css("left")}),d[0].style.width&&!c.forceHelperSize||d.width(this.currentItem.width()),d[0].style.height&&!c.forceHelperSize||d.height(this.currentItem.height()),d},_adjustOffsetFromHelper:function(b){"string"==typeof b&&(b=b.split(" ")),a.isArray(b)&&(b={left:+b[0],top:+b[1]||0}),"left"in b&&(this.offset.click.left=b.left+this.margins.left),"right"in b&&(this.offset.click.left=this.helperProportions.width-b.right+this.margins.left),"top"in b&&(this.offset.click.top=b.top+this.margins.top),"bottom"in b&&(this.offset.click.top=this.helperProportions.height-b.bottom+this.margins.top)},_getParentOffset:function(){this.offsetParent=this.helper.offsetParent();var b=this.offsetParent.offset();return"absolute"===this.cssPosition&&this.scrollParent[0]!==document&&a.contains(this.scrollParent[0],this.offsetParent[0])&&(b.left+=this.scrollParent.scrollLeft(),b.top+=this.scrollParent.scrollTop()),(this.offsetParent[0]===document.body||this.offsetParent[0].tagName&&"html"===this.offsetParent[0].tagName.toLowerCase()&&a.ui.ie)&&(b={top:0,left:0}),{top:b.top+(parseInt(this.offsetParent.css("borderTopWidth"),10)||0),left:b.left+(parseInt(this.offsetParent.css("borderLeftWidth"),10)||0)}},_getRelativeOffset:function(){if("relative"===this.cssPosition){var a=this.currentItem.position();return{top:a.top-(parseInt(this.helper.css("top"),10)||0)+this.scrollParent.scrollTop(),left:a.left-(parseInt(this.helper.css("left"),10)||0)+this.scrollParent.scrollLeft()}}return{top:0,left:0}},_cacheMargins:function(){this.margins={left:parseInt(this.currentItem.css("marginLeft"),10)||0,top:parseInt(this.currentItem.css("marginTop"),10)||0}},_cacheHelperProportions:function(){this.helperProportions={width:this.helper.outerWidth(),height:this.helper.outerHeight()}},_setContainment:function(){var b,c,d,e=this.options;"parent"===e.containment&&(e.containment=this.helper[0].parentNode),"document"!==e.containment&&"window"!==e.containment||(this.containment=[0-this.offset.relative.left-this.offset.parent.left,0-this.offset.relative.top-this.offset.parent.top,a("document"===e.containment?document:window).width()-this.helperProportions.width-this.margins.left,(a("document"===e.containment?document:window).height()||document.body.parentNode.scrollHeight)-this.helperProportions.height-this.margins.top]),/^(document|window|parent)$/.test(e.containment)||(b=a(e.containment)[0],c=a(e.containment).offset(),d="hidden"!==a(b).css("overflow"),this.containment=[c.left+(parseInt(a(b).css("borderLeftWidth"),10)||0)+(parseInt(a(b).css("paddingLeft"),10)||0)-this.margins.left,c.top+(parseInt(a(b).css("borderTopWidth"),10)||0)+(parseInt(a(b).css("paddingTop"),10)||0)-this.margins.top,c.left+(d?Math.max(b.scrollWidth,b.offsetWidth):b.offsetWidth)-(parseInt(a(b).css("borderLeftWidth"),10)||0)-(parseInt(a(b).css("paddingRight"),10)||0)-this.helperProportions.width-this.margins.left,c.top+(d?Math.max(b.scrollHeight,b.offsetHeight):b.offsetHeight)-(parseInt(a(b).css("borderTopWidth"),10)||0)-(parseInt(a(b).css("paddingBottom"),10)||0)-this.helperProportions.height-this.margins.top])},_convertPositionTo:function(b,c){c||(c=this.position);var d="absolute"===b?1:-1,e="absolute"!==this.cssPosition||this.scrollParent[0]!==document&&a.contains(this.scrollParent[0],this.offsetParent[0])?this.scrollParent:this.offsetParent,f=/(html|body)/i.test(e[0].tagName);return{top:c.top+this.offset.relative.top*d+this.offset.parent.top*d-("fixed"===this.cssPosition?-this.scrollParent.scrollTop():f?0:e.scrollTop())*d,left:c.left+this.offset.relative.left*d+this.offset.parent.left*d-("fixed"===this.cssPosition?-this.scrollParent.scrollLeft():f?0:e.scrollLeft())*d}},_generatePosition:function(b){var c,d,e=this.options,f=b.pageX,g=b.pageY,h="absolute"!==this.cssPosition||this.scrollParent[0]!==document&&a.contains(this.scrollParent[0],this.offsetParent[0])?this.scrollParent:this.offsetParent,i=/(html|body)/i.test(h[0].tagName);return"relative"!==this.cssPosition||this.scrollParent[0]!==document&&this.scrollParent[0]!==this.offsetParent[0]||(this.offset.relative=this._getRelativeOffset()),this.originalPosition&&(this.containment&&(b.pageX-this.offset.click.left<this.containment[0]&&(f=this.containment[0]+this.offset.click.left),b.pageY-this.offset.click.top<this.containment[1]&&(g=this.containment[1]+this.offset.click.top),b.pageX-this.offset.click.left>this.containment[2]&&(f=this.containment[2]+this.offset.click.left),b.pageY-this.offset.click.top>this.containment[3]&&(g=this.containment[3]+this.offset.click.top)),e.grid&&(c=this.originalPageY+Math.round((g-this.originalPageY)/e.grid[1])*e.grid[1],g=this.containment?c-this.offset.click.top>=this.containment[1]&&c-this.offset.click.top<=this.containment[3]?c:c-this.offset.click.top>=this.containment[1]?c-e.grid[1]:c+e.grid[1]:c,d=this.originalPageX+Math.round((f-this.originalPageX)/e.grid[0])*e.grid[0],f=this.containment?d-this.offset.click.left>=this.containment[0]&&d-this.offset.click.left<=this.containment[2]?d:d-this.offset.click.left>=this.containment[0]?d-e.grid[0]:d+e.grid[0]:d)),{top:g-this.offset.click.top-this.offset.relative.top-this.offset.parent.top+("fixed"===this.cssPosition?-this.scrollParent.scrollTop():i?0:h.scrollTop()),left:f-this.offset.click.left-this.offset.relative.left-this.offset.parent.left+("fixed"===this.cssPosition?-this.scrollParent.scrollLeft():i?0:h.scrollLeft())}},_rearrange:function(a,b,c,d){c?c[0].appendChild(this.placeholder[0]):b.item[0].parentNode.insertBefore(this.placeholder[0],"down"===this.direction?b.item[0]:b.item[0].nextSibling),this.counter=this.counter?++this.counter:1;var e=this.counter;this._delay(function(){e===this.counter&&this.refreshPositions(!d)})},_clear:function(a,b){this.reverting=!1;var c,d=[];if(!this._noFinalSort&&this.currentItem.parent().length&&this.placeholder.before(this.currentItem),this._noFinalSort=null,this.helper[0]===this.currentItem[0]){for(c in this._storedCSS)"auto"!==this._storedCSS[c]&&"static"!==this._storedCSS[c]||(this._storedCSS[c]="");this.currentItem.css(this._storedCSS).removeClass("ui-sortable-helper")}else this.currentItem.show();for(this.fromOutside&&!b&&d.push(function(a){this._trigger("receive",a,this._uiHash(this.fromOutside))}),!this.fromOutside&&this.domPosition.prev===this.currentItem.prev().not(".ui-sortable-helper")[0]&&this.domPosition.parent===this.currentItem.parent()[0]||b||d.push(function(a){this._trigger("update",a,this._uiHash())}),this!==this.currentContainer&&(b||(d.push(function(a){this._trigger("remove",a,this._uiHash())}),d.push(function(a){return function(b){a._trigger("receive",b,this._uiHash(this))}}.call(this,this.currentContainer)),d.push(function(a){return function(b){a._trigger("update",b,this._uiHash(this))}}.call(this,this.currentContainer)))),c=this.containers.length-1;c>=0;c--)b||d.push(function(a){return function(b){a._trigger("deactivate",b,this._uiHash(this))}}.call(this,this.containers[c])),this.containers[c].containerCache.over&&(d.push(function(a){return function(b){a._trigger("out",b,this._uiHash(this))}}.call(this,this.containers[c])),this.containers[c].containerCache.over=0);if(this.storedCursor&&(this.document.find("body").css("cursor",this.storedCursor),this.storedStylesheet.remove()),this._storedOpacity&&this.helper.css("opacity",this._storedOpacity),this._storedZIndex&&this.helper.css("zIndex","auto"===this._storedZIndex?"":this._storedZIndex),this.dragging=!1,this.cancelHelperRemoval){if(!b){for(this._trigger("beforeStop",a,this._uiHash()),c=0;c<d.length;c++)d[c].call(this,a);this._trigger("stop",a,this._uiHash())}return this.fromOutside=!1,!1}if(b||this._trigger("beforeStop",a,this._uiHash()),this.placeholder[0].parentNode.removeChild(this.placeholder[0]),this.helper[0]!==this.currentItem[0]&&this.helper.remove(),this.helper=null,!b){for(c=0;c<d.length;c++)d[c].call(this,a);this._trigger("stop",a,this._uiHash())}return this.fromOutside=!1,!0},_trigger:function(){!1===a.Widget.prototype._trigger.apply(this,arguments)&&this.cancel()},_uiHash:function(b){var c=b||this;return{helper:c.helper,placeholder:c.placeholder||a([]),position:c.position,originalPosition:c.originalPosition,offset:c.positionAbs,item:c.currentItem,sender:b?b.element:null}}})}(jQuery),function(a,b){var c="ui-effects-";a.effects={effect:{}},function(a,b){function m(a,b,c){var d=h[b.type]||{};return null==a?c||!b.def?null:b.def:(a=d.floor?~~a:parseFloat(a),isNaN(a)?b.def:d.mod?(a+d.mod)%d.mod:0>a?0:d.max<a?d.max:a)}function n(b){var c=f(),d=c._rgba=[];return b=b.toLowerCase(),l(e,function(a,e){var f,h=e.re.exec(b),i=h&&e.parse(h),j=e.space||"rgba";if(i)return f=c[j](i),c[g[j].cache]=f[g[j].cache],d=c._rgba=f._rgba,!1}),d.length?("0,0,0,0"===d.join()&&a.extend(d,k.transparent),c):k[b]}function o(a,b,c){return c=(c+1)%1,6*c<1?a+(b-a)*c*6:2*c<1?b:3*c<2?a+(b-a)*(2/3-c)*6:a}var k,c="backgroundColor borderBottomColor borderLeftColor borderRightColor borderTopColor color columnRuleColor outlineColor textDecorationColor textEmphasisColor",d=/^([\-+])=\s*(\d+\.?\d*)/,e=[{re:/rgba?\(\s*(\d{1,3})\s*,\s*(\d{1,3})\s*,\s*(\d{1,3})\s*(?:,\s*(\d?(?:\.\d+)?)\s*)?\)/,parse:function(a){return[a[1],a[2],a[3],a[4]]}},{re:/rgba?\(\s*(\d+(?:\.\d+)?)\%\s*,\s*(\d+(?:\.\d+)?)\%\s*,\s*(\d+(?:\.\d+)?)\%\s*(?:,\s*(\d?(?:\.\d+)?)\s*)?\)/,parse:function(a){return[2.55*a[1],2.55*a[2],2.55*a[3],a[4]]}},{re:/#([a-f0-9]{2})([a-f0-9]{2})([a-f0-9]{2})/,parse:function(a){return[parseInt(a[1],16),parseInt(a[2],16),parseInt(a[3],16)]}},{re:/#([a-f0-9])([a-f0-9])([a-f0-9])/,parse:function(a){return[parseInt(a[1]+a[1],16),parseInt(a[2]+a[2],16),parseInt(a[3]+a[3],16)]}},{re:/hsla?\(\s*(\d+(?:\.\d+)?)\s*,\s*(\d+(?:\.\d+)?)\%\s*,\s*(\d+(?:\.\d+)?)\%\s*(?:,\s*(\d?(?:\.\d+)?)\s*)?\)/,space:"hsla",parse:function(a){return[a[1],a[2]/100,a[3]/100,a[4]]}}],f=a.Color=function(b,c,d,e){return new a.Color.fn.parse(b,c,d,e)},g={rgba:{props:{red:{idx:0,type:"byte"},green:{idx:1,type:"byte"},blue:{idx:2,type:"byte"}}},hsla:{props:{hue:{idx:0,type:"degrees"},saturation:{idx:1,type:"percent"},lightness:{idx:2,type:"percent"}}}},h={byte:{floor:!0,max:255},percent:{max:1},degrees:{mod:360,floor:!0}},i=f.support={},j=a("<p>")[0],l=a.each;j.style.cssText="background-color:rgba(1,1,1,.5)",i.rgba=j.style.backgroundColor.indexOf("rgba")>-1,l(g,function(a,b){b.cache="_"+a,b.props.alpha={idx:3,type:"percent",def:1}}),f.fn=a.extend(f.prototype,{parse:function(c,d,e,h){if(c===b)return this._rgba=[null,null,null,null],this;(c.jquery||c.nodeType)&&(c=a(c).css(d),d=b);var i=this,j=a.type(c),o=this._rgba=[];return d!==b&&(c=[c,d,e,h],j="array"),"string"===j?this.parse(n(c)||k._default):"array"===j?(l(g.rgba.props,function(a,b){o[b.idx]=m(c[b.idx],b)}),this):"object"===j?(c instanceof f?l(g,function(a,b){c[b.cache]&&(i[b.cache]=c[b.cache].slice())}):l(g,function(b,d){var e=d.cache;l(d.props,function(a,b){if(!i[e]&&d.to){if("alpha"===a||null==c[a])return;i[e]=d.to(i._rgba)}i[e][b.idx]=m(c[a],b,!0)}),i[e]&&a.inArray(null,i[e].slice(0,3))<0&&(i[e][3]=1,d.from&&(i._rgba=d.from(i[e])))}),this):void 0},is:function(a){var b=f(a),c=!0,d=this;return l(g,function(a,e){var f,g=b[e.cache];return g&&(f=d[e.cache]||e.to&&e.to(d._rgba)||[],l(e.props,function(a,b){if(null!=g[b.idx])return c=g[b.idx]===f[b.idx]})),c}),c},_space:function(){var a=[],b=this;return l(g,function(c,d){b[d.cache]&&a.push(c)}),a.pop()},transition:function(a,b){var c=f(a),d=c._space(),e=g[d],i=0===this.alpha()?f("transparent"):this,j=i[e.cache]||e.to(i._rgba),k=j.slice();return c=c[e.cache],l(e.props,function(a,d){var e=d.idx,f=j[e],g=c[e],i=h[d.type]||{};null!==g&&(null===f?k[e]=g:(i.mod&&(g-f>i.mod/2?f+=i.mod:f-g>i.mod/2&&(f-=i.mod)),k[e]=m((g-f)*b+f,d)))}),this[d](k)},blend:function(b){if(1===this._rgba[3])return this;var c=this._rgba.slice(),d=c.pop(),e=f(b)._rgba;return f(a.map(c,function(a,b){return(1-d)*e[b]+d*a}))},toRgbaString:function(){var b="rgba(",c=a.map(this._rgba,function(a,b){return null==a?b>2?1:0:a});return 1===c[3]&&(c.pop(),b="rgb("),b+c.join()+")"},toHslaString:function(){var b="hsla(",c=a.map(this.hsla(),function(a,b){return null==a&&(a=b>2?1:0),b&&b<3&&(a=Math.round(100*a)+"%"),a});return 1===c[3]&&(c.pop(),b="hsl("),b+c.join()+")"},toHexString:function(b){var c=this._rgba.slice(),d=c.pop();return b&&c.push(~~(255*d)),"#"+a.map(c,function(a){return a=(a||0).toString(16),1===a.length?"0"+a:a}).join("")},toString:function(){return 0===this._rgba[3]?"transparent":this.toRgbaString()}}),f.fn.parse.prototype=f.fn,g.hsla.to=function(a){if(null==a[0]||null==a[1]||null==a[2])return[null,null,null,a[3]];var k,l,b=a[0]/255,c=a[1]/255,d=a[2]/255,e=a[3],f=Math.max(b,c,d),g=Math.min(b,c,d),h=f-g,i=f+g,j=.5*i;return k=g===f?0:b===f?60*(c-d)/h+360:c===f?60*(d-b)/h+120:60*(b-c)/h+240,l=0===h?0:j<=.5?h/i:h/(2-i),[Math.round(k)%360,l,j,null==e?1:e]},g.hsla.from=function(a){if(null==a[0]||null==a[1]||null==a[2])return[null,null,null,a[3]];var b=a[0]/360,c=a[1],d=a[2],e=a[3],f=d<=.5?d*(1+c):d+c-d*c,g=2*d-f;return[Math.round(255*o(g,f,b+1/3)),Math.round(255*o(g,f,b)),Math.round(255*o(g,f,b-1/3)),e]},l(g,function(c,e){var g=e.props,h=e.cache,i=e.to,j=e.from;f.fn[c]=function(c){if(i&&!this[h]&&(this[h]=i(this._rgba)),c===b)return this[h].slice();var d,e=a.type(c),k="array"===e||"object"===e?c:arguments,n=this[h].slice();return l(g,function(a,b){var c=k["object"===e?a:b.idx];null==c&&(c=n[b.idx]),n[b.idx]=m(c,b)}),j?(d=f(j(n)),d[h]=n,d):f(n)},l(g,function(b,e){f.fn[b]||(f.fn[b]=function(f){var k,g=a.type(f),h="alpha"===b?this._hsla?"hsla":"rgba":c,i=this[h](),j=i[e.idx];return"undefined"===g?j:("function"===g&&(f=f.call(this,j),g=a.type(f)),null==f&&e.empty?this:("string"===g&&(k=d.exec(f))&&(f=j+parseFloat(k[2])*("+"===k[1]?1:-1)),i[e.idx]=f,this[h](i)))})})}),f.hook=function(b){var c=b.split(" ");l(c,function(b,c){a.cssHooks[c]={set:function(b,d){var e,g,h="";if("transparent"!==d&&("string"!==a.type(d)||(e=n(d)))){if(d=f(e||d),!i.rgba&&1!==d._rgba[3]){for(g="backgroundColor"===c?b.parentNode:b;(""===h||"transparent"===h)&&g&&g.style;)try{h=a.css(g,"backgroundColor"),g=g.parentNode}catch(a){}d=d.blend(h&&"transparent"!==h?h:"_default")}d=d.toRgbaString()}try{b.style[c]=d}catch(a){}}},a.fx.step[c]=function(b){b.colorInit||(b.start=f(b.elem,c),b.end=f(b.end),b.colorInit=!0),a.cssHooks[c].set(b.elem,b.start.transition(b.end,b.pos))}})},f.hook(c),a.cssHooks.borderColor={expand:function(a){var b={};return l(["Top","Right","Bottom","Left"],function(c,d){b["border"+d+"Color"]=a}),b}},k=a.Color.names={aqua:"#00ffff",black:"#000000",blue:"#0000ff",fuchsia:"#ff00ff",gray:"#808080",green:"#008000",lime:"#00ff00",maroon:"#800000",navy:"#000080",olive:"#808000",purple:"#800080",red:"#ff0000",silver:"#c0c0c0",teal:"#008080",white:"#ffffff",yellow:"#ffff00",transparent:[null,null,null,0],_default:"#ffffff"}}(jQuery),function(){function e(b){var c,d,e=b.ownerDocument.defaultView?b.ownerDocument.defaultView.getComputedStyle(b,null):b.currentStyle,f={};if(e&&e.length&&e[0]&&e[e[0]])for(d=e.length;d--;)c=e[d],"string"==typeof e[c]&&(f[a.camelCase(c)]=e[c]);else for(c in e)"string"==typeof e[c]&&(f[c]=e[c]);return f}function f(b,c){var f,g,e={};for(f in c)g=c[f],b[f]!==g&&(d[f]||!a.fx.step[f]&&isNaN(parseFloat(g))||(e[f]=g));return e}var c=["add","remove","toggle"],d={border:1,borderBottom:1,borderColor:1,borderLeft:1,borderRight:1,borderTop:1,borderWidth:1,margin:1,padding:1};a.each(["borderLeftStyle","borderRightStyle","borderBottomStyle","borderTopStyle"],function(b,c){a.fx.step[c]=function(a){("none"!==a.end&&!a.setAttr||1===a.pos&&!a.setAttr)&&(jQuery.style(a.elem,c,a.end),a.setAttr=!0)}}),a.fn.addBack||(a.fn.addBack=function(a){return this.add(null==a?this.prevObject:this.prevObject.filter(a))}),a.effects.animateClass=function(b,d,g,h){var i=a.speed(d,g,h);return this.queue(function(){var h,d=a(this),g=d.attr("class")||"",j=i.children?d.find("*").addBack():d;j=j.map(function(){return{el:a(this),start:e(this)}}),h=function(){a.each(c,function(a,c){b[c]&&d[c+"Class"](b[c])})},h(),j=j.map(function(){return this.end=e(this.el[0]),this.diff=f(this.start,this.end),this}),d.attr("class",g),j=j.map(function(){var b=this,c=a.Deferred(),d=a.extend({},i,{queue:!1,complete:function(){c.resolve(b)}});return this.el.animate(this.diff,d),c.promise()}),a.when.apply(a,j.get()).done(function(){h(),a.each(arguments,function(){var b=this.el;a.each(this.diff,function(a){b.css(a,"")})}),i.complete.call(d[0])})})},a.fn.extend({addClass:function(b){return function(c,d,e,f){return d?a.effects.animateClass.call(this,{add:c},d,e,f):b.apply(this,arguments)}}(a.fn.addClass),removeClass:function(b){return function(c,d,e,f){return arguments.length>1?a.effects.animateClass.call(this,{remove:c},d,e,f):b.apply(this,arguments)}}(a.fn.removeClass),toggleClass:function(c){return function(d,e,f,g,h){return"boolean"==typeof e||e===b?f?a.effects.animateClass.call(this,e?{add:d}:{remove:d},f,g,h):c.apply(this,arguments):a.effects.animateClass.call(this,{toggle:d},e,f,g)}}(a.fn.toggleClass),switchClass:function(b,c,d,e,f){return a.effects.animateClass.call(this,{add:c,remove:b},d,e,f)}})}(),function(){function d(b,c,d,e){return a.isPlainObject(b)&&(c=b,b=b.effect),b={effect:b},null==c&&(c={}),a.isFunction(c)&&(e=c,d=null,c={}),("number"==typeof c||a.fx.speeds[c])&&(e=d,d=c,c={}),a.isFunction(d)&&(e=d,d=null),c&&a.extend(b,c),d=d||c.duration,b.duration=a.fx.off?0:"number"==typeof d?d:d in a.fx.speeds?a.fx.speeds[d]:a.fx.speeds._default,b.complete=e||c.complete,b}function e(b){return!(b&&"number"!=typeof b&&!a.fx.speeds[b])||("string"==typeof b&&!a.effects.effect[b]||(!!a.isFunction(b)||"object"==typeof b&&!b.effect))}a.extend(a.effects,{version:"1.10.2",save:function(a,b){for(var d=0;d<b.length;d++)null!==b[d]&&a.data(c+b[d],a[0].style[b[d]])},restore:function(a,d){var e,f;for(f=0;f<d.length;f++)null!==d[f]&&(e=a.data(c+d[f]),e===b&&(e=""),a.css(d[f],e))},setMode:function(a,b){return"toggle"===b&&(b=a.is(":hidden")?"show":"hide"),b},getBaseline:function(a,b){var c,d;switch(a[0]){case"top":c=0;break;case"middle":c=.5;break;case"bottom":c=1;break;default:c=a[0]/b.height}switch(a[1]){case"left":d=0;break;case"center":d=.5;break;case"right":d=1;break;default:d=a[1]/b.width}return{x:d,y:c}},createWrapper:function(b){if(b.parent().is(".ui-effects-wrapper"))return b.parent();var c={width:b.outerWidth(!0),height:b.outerHeight(!0),float:b.css("float")},d=a("<div></div>").addClass("ui-effects-wrapper").css({fontSize:"100%",background:"transparent",border:"none",margin:0,padding:0}),e={width:b.width(),height:b.height()},f=document.activeElement;try{f.id}catch(a){f=document.body}return b.wrap(d),(b[0]===f||a.contains(b[0],f))&&a(f).focus(),d=b.parent(),"static"===b.css("position")?(d.css({position:"relative"}),b.css({position:"relative"})):(a.extend(c,{position:b.css("position"),zIndex:b.css("z-index")}),a.each(["top","left","bottom","right"],function(a,d){c[d]=b.css(d),isNaN(parseInt(c[d],10))&&(c[d]="auto")}),b.css({position:"relative",top:0,left:0,right:"auto",bottom:"auto"})),b.css(e),d.css(c).show()},removeWrapper:function(b){var c=document.activeElement;return b.parent().is(".ui-effects-wrapper")&&(b.parent().replaceWith(b),(b[0]===c||a.contains(b[0],c))&&a(c).focus()),b},setTransition:function(b,c,d,e){return e=e||{},a.each(c,function(a,c){var f=b.cssUnit(c);f[0]>0&&(e[c]=f[0]*d+f[1])}),e}}),a.fn.extend({effect:function(){function g(c){function h(){a.isFunction(e)&&e.call(d[0]),a.isFunction(c)&&c()}var d=a(this),e=b.complete,g=b.mode;(d.is(":hidden")?"hide"===g:"show"===g)?(d[g](),h()):f.call(d[0],b,h)}var b=d.apply(this,arguments),c=b.mode,e=b.queue,f=a.effects.effect[b.effect];return a.fx.off||!f?c?this[c](b.duration,b.complete):this.each(function(){b.complete&&b.complete.call(this)}):!1===e?this.each(g):this.queue(e||"fx",g)},show:function(a){return function(b){if(e(b))return a.apply(this,arguments);var c=d.apply(this,arguments);return c.mode="show",this.effect.call(this,c)}}(a.fn.show),hide:function(a){return function(b){if(e(b))return a.apply(this,arguments);var c=d.apply(this,arguments);return c.mode="hide",this.effect.call(this,c)}}(a.fn.hide),toggle:function(a){return function(b){if(e(b)||"boolean"==typeof b)return a.apply(this,arguments);var c=d.apply(this,arguments);return c.mode="toggle",this.effect.call(this,c)}}(a.fn.toggle),cssUnit:function(b){var c=this.css(b),d=[];return a.each(["em","px","%","pt"],function(a,b){c.indexOf(b)>0&&(d=[parseFloat(c),b])}),d}})}(),function(){var b={};a.each(["Quad","Cubic","Quart","Quint","Expo"],function(a,c){b[c]=function(b){return Math.pow(b,a+2)}}),a.extend(b,{Sine:function(a){return 1-Math.cos(a*Math.PI/2)},Circ:function(a){return 1-Math.sqrt(1-a*a)},Elastic:function(a){return 0===a||1===a?a:-Math.pow(2,8*(a-1))*Math.sin((80*(a-1)-7.5)*Math.PI/15)},Back:function(a){return a*a*(3*a-2)},Bounce:function(a){for(var b,c=4;a<((b=Math.pow(2,--c))-1)/11;);return 1/Math.pow(4,3-c)-7.5625*Math.pow((3*b-2)/22-a,2)}}),a.each(b,function(b,c){a.easing["easeIn"+b]=c,a.easing["easeOut"+b]=function(a){return 1-c(1-a)},a.easing["easeInOut"+b]=function(a){return a<.5?c(2*a)/2:1-c(-2*a+2)/2}})}()}(jQuery),function(a,b){var c=0,d={},e={};d.height=d.paddingTop=d.paddingBottom=d.borderTopWidth=d.borderBottomWidth="hide",e.height=e.paddingTop=e.paddingBottom=e.borderTopWidth=e.borderBottomWidth="show",a.widget("ui.accordion",{version:"1.10.2",options:{active:0,animate:{},collapsible:!1,event:"click",header:"> li > :first-child,> :not(li):even",heightStyle:"auto",icons:{activeHeader:"ui-icon-triangle-1-s",header:"ui-icon-triangle-1-e"},activate:null,beforeActivate:null},_create:function(){var b=this.options;this.prevShow=this.prevHide=a(),this.element.addClass("ui-accordion ui-widget ui-helper-reset").attr("role","tablist"),b.collapsible||!1!==b.active&&null!=b.active||(b.active=0),this._processPanels(),b.active<0&&(b.active+=this.headers.length),this._refresh()},_getCreateEventData:function(){return{header:this.active,panel:this.active.length?this.active.next():a(),content:this.active.length?this.active.next():a()}},_createIcons:function(){var b=this.options.icons;b&&(a("<span>").addClass("ui-accordion-header-icon ui-icon "+b.header).prependTo(this.headers),this.active.children(".ui-accordion-header-icon").removeClass(b.header).addClass(b.activeHeader),this.headers.addClass("ui-accordion-icons"))},_destroyIcons:function(){this.headers.removeClass("ui-accordion-icons").children(".ui-accordion-header-icon").remove()},_destroy:function(){var a;this.element.removeClass("ui-accordion ui-widget ui-helper-reset").removeAttr("role"),this.headers.removeClass("ui-accordion-header ui-accordion-header-active ui-helper-reset ui-state-default ui-corner-all ui-state-active ui-state-disabled ui-corner-top").removeAttr("role").removeAttr("aria-selected").removeAttr("aria-controls").removeAttr("tabIndex").each(function(){/^ui-accordion/.test(this.id)&&this.removeAttribute("id")}),this._destroyIcons(),a=this.headers.next().css("display","").removeAttr("role").removeAttr("aria-expanded").removeAttr("aria-hidden").removeAttr("aria-labelledby").removeClass("ui-helper-reset ui-widget-content ui-corner-bottom ui-accordion-content ui-accordion-content-active ui-state-disabled").each(function(){/^ui-accordion/.test(this.id)&&this.removeAttribute("id")}),"content"!==this.options.heightStyle&&a.css("height","")},_setOption:function(a,b){if("active"===a)return void this._activate(b);"event"===a&&(this.options.event&&this._off(this.headers,this.options.event),this._setupEvents(b)),this._super(a,b),"collapsible"!==a||b||!1!==this.options.active||this._activate(0),"icons"===a&&(this._destroyIcons(),b&&this._createIcons()),"disabled"===a&&this.headers.add(this.headers.next()).toggleClass("ui-state-disabled",!!b)},_keydown:function(b){if(!b.altKey&&!b.ctrlKey){var c=a.ui.keyCode,d=this.headers.length,e=this.headers.index(b.target),f=!1;switch(b.keyCode){case c.RIGHT:case c.DOWN:f=this.headers[(e+1)%d];break;case c.LEFT:case c.UP:f=this.headers[(e-1+d)%d];break;case c.SPACE:case c.ENTER:this._eventHandler(b);break;case c.HOME:f=this.headers[0];break;case c.END:f=this.headers[d-1]}f&&(a(b.target).attr("tabIndex",-1),a(f).attr("tabIndex",0),f.focus(),b.preventDefault())}},_panelKeyDown:function(b){b.keyCode===a.ui.keyCode.UP&&b.ctrlKey&&a(b.currentTarget).prev().focus()},refresh:function(){var b=this.options;this._processPanels(),(!1===b.active&&!0===b.collapsible||!this.headers.length)&&(b.active=!1,this.active=a()),!1===b.active?this._activate(0):this.active.length&&!a.contains(this.element[0],this.active[0])?this.headers.length===this.headers.find(".ui-state-disabled").length?(b.active=!1,this.active=a()):this._activate(Math.max(0,b.active-1)):b.active=this.headers.index(this.active),this._destroyIcons(),this._refresh()},_processPanels:function(){this.headers=this.element.find(this.options.header).addClass("ui-accordion-header ui-helper-reset ui-state-default ui-corner-all"),this.headers.next().addClass("ui-accordion-content ui-helper-reset ui-widget-content ui-corner-bottom").filter(":not(.ui-accordion-content-active)").hide()},_refresh:function(){var b,d=this.options,e=d.heightStyle,f=this.element.parent(),g=this.accordionId="ui-accordion-"+(this.element.attr("id")||++c);this.active=this._findActive(d.active).addClass("ui-accordion-header-active ui-state-active ui-corner-top").removeClass("ui-corner-all"),this.active.next().addClass("ui-accordion-content-active").show(),this.headers.attr("role","tab").each(function(b){var c=a(this),d=c.attr("id"),e=c.next(),f=e.attr("id");d||(d=g+"-header-"+b,c.attr("id",d)),f||(f=g+"-panel-"+b,e.attr("id",f)),c.attr("aria-controls",f),e.attr("aria-labelledby",d)}).next().attr("role","tabpanel"),this.headers.not(this.active).attr({"aria-selected":"false",tabIndex:-1}).next().attr({"aria-expanded":"false","aria-hidden":"true"}).hide(),this.active.length?this.active.attr({"aria-selected":"true",tabIndex:0}).next().attr({"aria-expanded":"true","aria-hidden":"false"}):this.headers.eq(0).attr("tabIndex",0),this._createIcons(),this._setupEvents(d.event),"fill"===e?(b=f.height(),this.element.siblings(":visible").each(function(){var c=a(this),d=c.css("position");"absolute"!==d&&"fixed"!==d&&(b-=c.outerHeight(!0))}),this.headers.each(function(){b-=a(this).outerHeight(!0)}),this.headers.next().each(function(){a(this).height(Math.max(0,b-a(this).innerHeight()+a(this).height()))}).css("overflow","auto")):"auto"===e&&(b=0,this.headers.next().each(function(){b=Math.max(b,a(this).css("height","").height())}).height(b))},_activate:function(b){var c=this._findActive(b)[0];c!==this.active[0]&&(c=c||this.active[0],this._eventHandler({target:c,currentTarget:c,preventDefault:a.noop}))},_findActive:function(b){return"number"==typeof b?this.headers.eq(b):a()},_setupEvents:function(b){var c={keydown:"_keydown"};b&&a.each(b.split(" "),function(a,b){c[b]="_eventHandler"}),this._off(this.headers.add(this.headers.next())),this._on(this.headers,c),this._on(this.headers.next(),{keydown:"_panelKeyDown"}),this._hoverable(this.headers),this._focusable(this.headers)},_eventHandler:function(b){var c=this.options,d=this.active,e=a(b.currentTarget),f=e[0]===d[0],g=f&&c.collapsible,h=g?a():e.next(),i=d.next(),j={oldHeader:d,oldPanel:i,newHeader:g?a():e,newPanel:h};b.preventDefault(),f&&!c.collapsible||!1===this._trigger("beforeActivate",b,j)||(c.active=!g&&this.headers.index(e),this.active=f?a():e,this._toggle(j),d.removeClass("ui-accordion-header-active ui-state-active"),c.icons&&d.children(".ui-accordion-header-icon").removeClass(c.icons.activeHeader).addClass(c.icons.header),f||(e.removeClass("ui-corner-all").addClass("ui-accordion-header-active ui-state-active ui-corner-top"),c.icons&&e.children(".ui-accordion-header-icon").removeClass(c.icons.header).addClass(c.icons.activeHeader),e.next().addClass("ui-accordion-content-active")))},_toggle:function(b){var c=b.newPanel,d=this.prevShow.length?this.prevShow:b.oldPanel;this.prevShow.add(this.prevHide).stop(!0,!0),this.prevShow=c,this.prevHide=d,this.options.animate?this._animate(c,d,b):(d.hide(),c.show(),this._toggleComplete(b)),d.attr({"aria-expanded":"false","aria-hidden":"true"}),d.prev().attr("aria-selected","false"),c.length&&d.length?d.prev().attr("tabIndex",-1):c.length&&this.headers.filter(function(){return 0===a(this).attr("tabIndex")}).attr("tabIndex",-1),c.attr({"aria-expanded":"true","aria-hidden":"false"}).prev().attr({"aria-selected":"true",tabIndex:0})},_animate:function(a,b,c){var f,g,h,i=this,j=0,k=a.length&&(!b.length||a.index()<b.index()),l=this.options.animate||{},m=k&&l.down||l,n=function(){i._toggleComplete(c)};return"number"==typeof m&&(h=m),"string"==typeof m&&(g=m),g=g||m.easing||l.easing,h=h||m.duration||l.duration,b.length?a.length?(f=a.show().outerHeight(),b.animate(d,{duration:h,easing:g,step:function(a,b){b.now=Math.round(a)}}),void a.hide().animate(e,{duration:h,easing:g,complete:n,step:function(a,c){c.now=Math.round(a),"height"!==c.prop?j+=c.now:"content"!==i.options.heightStyle&&(c.now=Math.round(f-b.outerHeight()-j),j=0)}})):b.animate(d,h,g,n):a.animate(e,h,g,n)},_toggleComplete:function(a){var b=a.oldPanel;b.removeClass("ui-accordion-content-active").prev().removeClass("ui-corner-top").addClass("ui-corner-all"),b.length&&(b.parent()[0].className=b.parent()[0].className),this._trigger("activate",null,a)}})}(jQuery),function(a,b){var c=0;a.widget("ui.autocomplete",{version:"1.10.2",defaultElement:"<input>",options:{appendTo:null,autoFocus:!1,delay:300,minLength:1,position:{my:"left top",at:"left bottom",collision:"none"},source:null,change:null,close:null,focus:null,open:null,response:null,search:null,select:null},pending:0,_create:function(){var b,c,d,e=this.element[0].nodeName.toLowerCase(),f="textarea"===e,g="input"===e;this.isMultiLine=!!f||!g&&this.element.prop("isContentEditable"),this.valueMethod=this.element[f||g?"val":"text"],this.isNewMenu=!0,this.element.addClass("ui-autocomplete-input").attr("autocomplete","off"),this._on(this.element,{keydown:function(e){if(this.element.prop("readOnly"))return b=!0,d=!0,void(c=!0);b=!1,d=!1,c=!1;var f=a.ui.keyCode;switch(e.keyCode){case f.PAGE_UP:b=!0,this._move("previousPage",e);break;case f.PAGE_DOWN:b=!0,this._move("nextPage",e);break;case f.UP:b=!0,this._keyEvent("previous",e);break;case f.DOWN:b=!0,this._keyEvent("next",e);break;case f.ENTER:case f.NUMPAD_ENTER:this.menu.active&&(b=!0,e.preventDefault(),this.menu.select(e));break;case f.TAB:this.menu.active&&this.menu.select(e);break;case f.ESCAPE:this.menu.element.is(":visible")&&(this._value(this.term),this.close(e),e.preventDefault());break;default:c=!0,this._searchTimeout(e)}},keypress:function(d){if(b)return b=!1,void d.preventDefault();if(!c){var e=a.ui.keyCode;switch(d.keyCode){case e.PAGE_UP:this._move("previousPage",d);break;case e.PAGE_DOWN:this._move("nextPage",d);break;case e.UP:this._keyEvent("previous",d);break;case e.DOWN:this._keyEvent("next",d)}}},input:function(a){if(d)return d=!1,void a.preventDefault();this._searchTimeout(a)},focus:function(){this.selectedItem=null,this.previous=this._value()},blur:function(a){if(this.cancelBlur)return void delete this.cancelBlur;clearTimeout(this.searching),this.close(a),this._change(a)}}),this._initSource(),this.menu=a("<ul>").addClass("ui-autocomplete ui-front").appendTo(this._appendTo()).menu({input:a(),role:null}).hide().data("ui-menu"),this._on(this.menu.element,{mousedown:function(b){b.preventDefault(),this.cancelBlur=!0,this._delay(function(){delete this.cancelBlur});var c=this.menu.element[0];a(b.target).closest(".ui-menu-item").length||this._delay(function(){var b=this;this.document.one("mousedown",function(d){d.target===b.element[0]||d.target===c||a.contains(c,d.target)||b.close()})})},menufocus:function(b,c){if(this.isNewMenu&&(this.isNewMenu=!1,b.originalEvent&&/^mouse/.test(b.originalEvent.type)))return this.menu.blur(),void this.document.one("mousemove",function(){a(b.target).trigger(b.originalEvent)});var d=c.item.data("ui-autocomplete-item");!1!==this._trigger("focus",b,{item:d})?b.originalEvent&&/^key/.test(b.originalEvent.type)&&this._value(d.value):this.liveRegion.text(d.value)},menuselect:function(a,b){var c=b.item.data("ui-autocomplete-item"),d=this.previous;this.element[0]!==this.document[0].activeElement&&(this.element.focus(),this.previous=d,this._delay(function(){this.previous=d,this.selectedItem=c})),!1!==this._trigger("select",a,{item:c})&&this._value(c.value),this.term=this._value(),this.close(a),this.selectedItem=c}}),this.liveRegion=a("<span>",{role:"status","aria-live":"polite"}).addClass("ui-helper-hidden-accessible").insertAfter(this.element),this._on(this.window,{beforeunload:function(){this.element.removeAttr("autocomplete")}})},_destroy:function(){clearTimeout(this.searching),this.element.removeClass("ui-autocomplete-input").removeAttr("autocomplete"),this.menu.element.remove(),this.liveRegion.remove()},_setOption:function(a,b){this._super(a,b),"source"===a&&this._initSource(),"appendTo"===a&&this.menu.element.appendTo(this._appendTo()),"disabled"===a&&b&&this.xhr&&this.xhr.abort()},_appendTo:function(){var b=this.options.appendTo;return b&&(b=b.jquery||b.nodeType?a(b):this.document.find(b).eq(0)),b||(b=this.element.closest(".ui-front")),b.length||(b=this.document[0].body),b},_initSource:function(){var b,c,d=this;a.isArray(this.options.source)?(b=this.options.source,this.source=function(c,d){d(a.ui.autocomplete.filter(b,c.term))}):"string"==typeof this.options.source?(c=this.options.source,this.source=function(b,e){d.xhr&&d.xhr.abort(),d.xhr=a.ajax({url:c,data:b,dataType:"json",success:function(a){e(a)},error:function(){e([])}})}):this.source=this.options.source},_searchTimeout:function(a){clearTimeout(this.searching),this.searching=this._delay(function(){this.term!==this._value()&&(this.selectedItem=null,this.search(null,a))},this.options.delay)},search:function(a,b){return a=null!=a?a:this._value(),this.term=this._value(),a.length<this.options.minLength?this.close(b):!1!==this._trigger("search",b)?this._search(a):void 0},_search:function(a){this.pending++,this.element.addClass("ui-autocomplete-loading"),this.cancelSearch=!1,this.source({term:a},this._response())},_response:function(){var a=this,b=++c;return function(d){b===c&&a.__response(d),--a.pending||a.element.removeClass("ui-autocomplete-loading")}},__response:function(a){a&&(a=this._normalize(a)),this._trigger("response",null,{content:a}),!this.options.disabled&&a&&a.length&&!this.cancelSearch?(this._suggest(a),this._trigger("open")):this._close()},close:function(a){this.cancelSearch=!0,this._close(a)},_close:function(a){this.menu.element.is(":visible")&&(this.menu.element.hide(),this.menu.blur(),this.isNewMenu=!0,this._trigger("close",a))},_change:function(a){this.previous!==this._value()&&this._trigger("change",a,{item:this.selectedItem})},_normalize:function(b){return b.length&&b[0].label&&b[0].value?b:a.map(b,function(b){return"string"==typeof b?{label:b,value:b}:a.extend({label:b.label||b.value,value:b.value||b.label},b)})},_suggest:function(b){var c=this.menu.element.empty();this._renderMenu(c,b),this.isNewMenu=!0,this.menu.refresh(),c.show(),this._resizeMenu(),c.position(a.extend({of:this.element},this.options.position)),this.options.autoFocus&&this.menu.next()},_resizeMenu:function(){var a=this.menu.element;a.outerWidth(Math.max(a.width("").outerWidth()+1,this.element.outerWidth()))},_renderMenu:function(b,c){var d=this;a.each(c,function(a,c){d._renderItemData(b,c)})},_renderItemData:function(a,b){return this._renderItem(a,b).data("ui-autocomplete-item",b)},_renderItem:function(b,c){return a("<li>").append(a("<a>").text(c.label)).appendTo(b)},_move:function(a,b){return this.menu.element.is(":visible")?this.menu.isFirstItem()&&/^previous/.test(a)||this.menu.isLastItem()&&/^next/.test(a)?(this._value(this.term),void this.menu.blur()):void this.menu[a](b):void this.search(null,b)},widget:function(){return this.menu.element},_value:function(){return this.valueMethod.apply(this.element,arguments)},_keyEvent:function(a,b){this.isMultiLine&&!this.menu.element.is(":visible")||(this._move(a,b),b.preventDefault())}}),a.extend(a.ui.autocomplete,{escapeRegex:function(a){return a.replace(/[\-\[\]{}()*+?.,\\\^$|#\s]/g,"\\$&")},filter:function(b,c){var d=new RegExp(a.ui.autocomplete.escapeRegex(c),"i");return a.grep(b,function(a){return d.test(a.label||a.value||a)})}}),a.widget("ui.autocomplete",a.ui.autocomplete,{options:{messages:{noResults:"No search results.",results:function(a){return a+(a>1?" results are":" result is")+" available, use up and down arrow keys to navigate."}}},__response:function(a){var b;this._superApply(arguments),this.options.disabled||this.cancelSearch||(b=a&&a.length?this.options.messages.results(a.length):this.options.messages.noResults,this.liveRegion.text(b))}})}(jQuery),function(a,b){var c,d,e,f,g="ui-button ui-widget ui-state-default ui-corner-all",i="ui-button-icons-only ui-button-icon-only ui-button-text-icons ui-button-text-icon-primary ui-button-text-icon-secondary ui-button-text-only",j=function(){var b=a(this).find(":ui-button");setTimeout(function(){b.button("refresh")},1)},k=function(b){var c=b.name,d=b.form,e=a([]);return c&&(c=c.replace(/'/g,"\\'"),e=d?a(d).find("[name='"+c+"']"):a("[name='"+c+"']",b.ownerDocument).filter(function(){return!this.form})),e};a.widget("ui.button",{version:"1.10.2",defaultElement:"<button>",options:{disabled:null,text:!0,label:null,icons:{primary:null,secondary:null}},_create:function(){this.element.closest("form").unbind("reset"+this.eventNamespace).bind("reset"+this.eventNamespace,j),"boolean"!=typeof this.options.disabled?this.options.disabled=!!this.element.prop("disabled"):this.element.prop("disabled",this.options.disabled),this._determineButtonType(),this.hasTitle=!!this.buttonElement.attr("title");var b=this,h=this.options,i="checkbox"===this.type||"radio"===this.type,l=i?"":"ui-state-active",m="ui-state-focus";null===h.label&&(h.label="input"===this.type?this.buttonElement.val():this.buttonElement.html()),this._hoverable(this.buttonElement),this.buttonElement.addClass(g).attr("role","button").bind("mouseenter"+this.eventNamespace,function(){h.disabled||this===c&&a(this).addClass("ui-state-active")}).bind("mouseleave"+this.eventNamespace,function(){h.disabled||a(this).removeClass(l)}).bind("click"+this.eventNamespace,function(a){h.disabled&&(a.preventDefault(),a.stopImmediatePropagation())}),this.element.bind("focus"+this.eventNamespace,function(){b.buttonElement.addClass(m)}).bind("blur"+this.eventNamespace,function(){b.buttonElement.removeClass(m)}),i&&(this.element.bind("change"+this.eventNamespace,function(){f||b.refresh()}),this.buttonElement.bind("mousedown"+this.eventNamespace,function(a){h.disabled||(f=!1,d=a.pageX,e=a.pageY)}).bind("mouseup"+this.eventNamespace,function(a){h.disabled||d===a.pageX&&e===a.pageY||(f=!0)})),"checkbox"===this.type?this.buttonElement.bind("click"+this.eventNamespace,function(){if(h.disabled||f)return!1}):"radio"===this.type?this.buttonElement.bind("click"+this.eventNamespace,function(){if(h.disabled||f)return!1;a(this).addClass("ui-state-active"),b.buttonElement.attr("aria-pressed","true");var c=b.element[0];k(c).not(c).map(function(){return a(this).button("widget")[0]}).removeClass("ui-state-active").attr("aria-pressed","false")}):(this.buttonElement.bind("mousedown"+this.eventNamespace,function(){if(h.disabled)return!1;a(this).addClass("ui-state-active"),c=this,b.document.one("mouseup",function(){c=null})}).bind("mouseup"+this.eventNamespace,function(){if(h.disabled)return!1;a(this).removeClass("ui-state-active")}).bind("keydown"+this.eventNamespace,function(b){if(h.disabled)return!1;b.keyCode!==a.ui.keyCode.SPACE&&b.keyCode!==a.ui.keyCode.ENTER||a(this).addClass("ui-state-active")}).bind("keyup"+this.eventNamespace+" blur"+this.eventNamespace,function(){a(this).removeClass("ui-state-active")}),this.buttonElement.is("a")&&this.buttonElement.keyup(function(b){b.keyCode===a.ui.keyCode.SPACE&&a(this).click()})),this._setOption("disabled",h.disabled),this._resetButton()},_determineButtonType:function(){var a,b,c;this.element.is("[type=checkbox]")?this.type="checkbox":this.element.is("[type=radio]")?this.type="radio":this.element.is("input")?this.type="input":this.type="button","checkbox"===this.type||"radio"===this.type?(a=this.element.parents().last(),b="label[for='"+this.element.attr("id")+"']",this.buttonElement=a.find(b),this.buttonElement.length||(a=a.length?a.siblings():this.element.siblings(),this.buttonElement=a.filter(b),this.buttonElement.length||(this.buttonElement=a.find(b))),this.element.addClass("ui-helper-hidden-accessible"),c=this.element.is(":checked"),c&&this.buttonElement.addClass("ui-state-active"),this.buttonElement.prop("aria-pressed",c)):this.buttonElement=this.element},widget:function(){return this.buttonElement},_destroy:function(){this.element.removeClass("ui-helper-hidden-accessible"),this.buttonElement.removeClass(g+" ui-state-hover ui-state-active  "+i).removeAttr("role").removeAttr("aria-pressed").html(this.buttonElement.find(".ui-button-text").html()),this.hasTitle||this.buttonElement.removeAttr("title")},_setOption:function(a,b){if(this._super(a,b),"disabled"===a)return void(b?this.element.prop("disabled",!0):this.element.prop("disabled",!1));this._resetButton()},refresh:function(){var b=this.element.is("input, button")?this.element.is(":disabled"):this.element.hasClass("ui-button-disabled");b!==this.options.disabled&&this._setOption("disabled",b),"radio"===this.type?k(this.element[0]).each(function(){a(this).is(":checked")?a(this).button("widget").addClass("ui-state-active").attr("aria-pressed","true"):a(this).button("widget").removeClass("ui-state-active").attr("aria-pressed","false")}):"checkbox"===this.type&&(this.element.is(":checked")?this.buttonElement.addClass("ui-state-active").attr("aria-pressed","true"):this.buttonElement.removeClass("ui-state-active").attr("aria-pressed","false"))},_resetButton:function(){if("input"===this.type)return void(this.options.label&&this.element.val(this.options.label));var b=this.buttonElement.removeClass(i),c=a("<span></span>",this.document[0]).addClass("ui-button-text").html(this.options.label).appendTo(b.empty()).text(),d=this.options.icons,e=d.primary&&d.secondary,f=[];d.primary||d.secondary?(this.options.text&&f.push("ui-button-text-icon"+(e?"s":d.primary?"-primary":"-secondary")),d.primary&&b.prepend("<span class='ui-button-icon-primary ui-icon "+d.primary+"'></span>"),d.secondary&&b.append("<span class='ui-button-icon-secondary ui-icon "+d.secondary+"'></span>"),this.options.text||(f.push(e?"ui-button-icons-only":"ui-button-icon-only"),this.hasTitle||b.attr("title",a.trim(c)))):f.push("ui-button-text-only"),b.addClass(f.join(" "))}}),a.widget("ui.buttonset",{version:"1.10.2",options:{items:"button, input[type=button], input[type=submit], input[type=reset], input[type=checkbox], input[type=radio], a, :data(ui-button)"},_create:function(){this.element.addClass("ui-buttonset")},_init:function(){this.refresh()},_setOption:function(a,b){"disabled"===a&&this.buttons.button("option",a,b),this._super(a,b)},refresh:function(){var b="rtl"===this.element.css("direction");this.buttons=this.element.find(this.options.items).filter(":ui-button").button("refresh").end().not(":ui-button").button().end().map(function(){return a(this).button("widget")[0]}).removeClass("ui-corner-all ui-corner-left ui-corner-right").filter(":first").addClass(b?"ui-corner-right":"ui-corner-left").end().filter(":last").addClass(b?"ui-corner-left":"ui-corner-right").end().end()},_destroy:function(){this.element.removeClass("ui-buttonset"),this.buttons.map(function(){return a(this).button("widget")[0]}).removeClass("ui-corner-left ui-corner-right").end().button("destroy")}})}(jQuery),function(a,b){function f(){this._curInst=null,this._keyEvent=!1,this._disabledInputs=[],this._datepickerShowing=!1,this._inDialog=!1,this._mainDivId="ui-datepicker-div",this._inlineClass="ui-datepicker-inline",this._appendClass="ui-datepicker-append",this._triggerClass="ui-datepicker-trigger",this._dialogClass="ui-datepicker-dialog",this._disableClass="ui-datepicker-disabled",this._unselectableClass="ui-datepicker-unselectable",this._currentClass="ui-datepicker-current-day",this._dayOverClass="ui-datepicker-days-cell-over",this.regional=[],this.regional[""]={closeText:"Done",prevText:"Prev",nextText:"Next",currentText:"Today",monthNames:["January","February","March","April","May","June","July","August","September","October","November","December"],monthNamesShort:["Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"],dayNames:["Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"],dayNamesShort:["Sun","Mon","Tue","Wed","Thu","Fri","Sat"],dayNamesMin:["Su","Mo","Tu","We","Th","Fr","Sa"],weekHeader:"Wk",dateFormat:"mm/dd/yy",firstDay:0,isRTL:!1,showMonthAfterYear:!1,yearSuffix:""},this._defaults={showOn:"focus",showAnim:"fadeIn",showOptions:{},defaultDate:null,appendText:"",buttonText:"...",buttonImage:"",buttonImageOnly:!1,hideIfNoPrevNext:!1,navigationAsDateFormat:!1,gotoCurrent:!1,changeMonth:!1,changeYear:!1,yearRange:"c-10:c+10",showOtherMonths:!1,selectOtherMonths:!1,showWeek:!1,calculateWeek:this.iso8601Week,shortYearCutoff:"+10",minDate:null,maxDate:null,duration:"fast",beforeShowDay:null,beforeShow:null,onSelect:null,onChangeMonthYear:null,onClose:null,numberOfMonths:1,showCurrentAtPos:0,stepMonths:1,stepBigMonths:12,altField:"",altFormat:"",constrainInput:!0,showButtonPanel:!1,autoSize:!1,disabled:!1},a.extend(this._defaults,this.regional[""]),this.dpDiv=g(a("<div id='"+this._mainDivId+"' class='ui-datepicker ui-widget ui-widget-content ui-helper-clearfix ui-corner-all'></div>"))}function g(b){var c="button, .ui-datepicker-prev, .ui-datepicker-next, .ui-datepicker-calendar td a";return b.delegate(c,"mouseout",function(){a(this).removeClass("ui-state-hover"),-1!==this.className.indexOf("ui-datepicker-prev")&&a(this).removeClass("ui-datepicker-prev-hover"),-1!==this.className.indexOf("ui-datepicker-next")&&a(this).removeClass("ui-datepicker-next-hover")}).delegate(c,"mouseover",function(){a.datepicker._isDisabledDatepicker(e.inline?b.parent()[0]:e.input[0])||(a(this).parents(".ui-datepicker-calendar").find("a").removeClass("ui-state-hover"),a(this).addClass("ui-state-hover"),-1!==this.className.indexOf("ui-datepicker-prev")&&a(this).addClass("ui-datepicker-prev-hover"),-1!==this.className.indexOf("ui-datepicker-next")&&a(this).addClass("ui-datepicker-next-hover"))})}function h(b,c){a.extend(b,c);for(var d in c)null==c[d]&&(b[d]=c[d]);return b}a.extend(a.ui,{datepicker:{version:"1.10.2"}});var e,c="datepicker",d=(new Date).getTime();a.extend(f.prototype,{markerClassName:"hasDatepicker",maxRows:4,_widgetDatepicker:function(){return this.dpDiv},setDefaults:function(a){return h(this._defaults,a||{}),this},_attachDatepicker:function(b,c){var d,e,f;d=b.nodeName.toLowerCase(),e="div"===d||"span"===d,b.id||(this.uuid+=1,b.id="dp"+this.uuid),f=this._newInst(a(b),e),f.settings=a.extend({},c||{}),"input"===d?this._connectDatepicker(b,f):e&&this._inlineDatepicker(b,f)},_newInst:function(b,c){return{id:b[0].id.replace(/([^A-Za-z0-9_\-])/g,"\\\\$1"),input:b,selectedDay:0,selectedMonth:0,selectedYear:0,drawMonth:0,drawYear:0,inline:c,dpDiv:c?g(a("<div class='"+this._inlineClass+" ui-datepicker ui-widget ui-widget-content ui-helper-clearfix ui-corner-all'></div>")):this.dpDiv}},_connectDatepicker:function(b,d){var e=a(b);d.append=a([]),d.trigger=a([]),e.hasClass(this.markerClassName)||(this._attachments(e,d),e.addClass(this.markerClassName).keydown(this._doKeyDown).keypress(this._doKeyPress).keyup(this._doKeyUp),this._autoSize(d),a.data(b,c,d),d.settings.disabled&&this._disableDatepicker(b))},_attachments:function(b,c){var d,e,f,g=this._get(c,"appendText"),h=this._get(c,"isRTL");c.append&&c.append.remove(),g&&(c.append=a("<span class='"+this._appendClass+"'>"+g+"</span>"),b[h?"before":"after"](c.append)),b.unbind("focus",this._showDatepicker),c.trigger&&c.trigger.remove(),d=this._get(c,"showOn"),"focus"!==d&&"both"!==d||b.focus(this._showDatepicker),"button"!==d&&"both"!==d||(e=this._get(c,"buttonText"),f=this._get(c,"buttonImage"),c.trigger=a(this._get(c,"buttonImageOnly")?a("<img/>").addClass(this._triggerClass).attr({src:f,alt:e,title:e}):a("<button type='button'></button>").addClass(this._triggerClass).html(f?a("<img/>").attr({src:f,alt:e,title:e}):e)),b[h?"before":"after"](c.trigger),c.trigger.click(function(){return a.datepicker._datepickerShowing&&a.datepicker._lastInput===b[0]?a.datepicker._hideDatepicker():a.datepicker._datepickerShowing&&a.datepicker._lastInput!==b[0]?(a.datepicker._hideDatepicker(),a.datepicker._showDatepicker(b[0])):a.datepicker._showDatepicker(b[0]),!1}))},_autoSize:function(a){if(this._get(a,"autoSize")&&!a.inline){var b,c,d,e,f=new Date(2009,11,20),g=this._get(a,"dateFormat");g.match(/[DM]/)&&(b=function(a){for(c=0,d=0,e=0;e<a.length;e++)a[e].length>c&&(c=a[e].length,d=e);return d},f.setMonth(b(this._get(a,g.match(/MM/)?"monthNames":"monthNamesShort"))),f.setDate(b(this._get(a,g.match(/DD/)?"dayNames":"dayNamesShort"))+20-f.getDay())),a.input.attr("size",this._formatDate(a,f).length)}},_inlineDatepicker:function(b,d){var e=a(b);e.hasClass(this.markerClassName)||(e.addClass(this.markerClassName).append(d.dpDiv),a.data(b,c,d),this._setDate(d,this._getDefaultDate(d),!0),this._updateDatepicker(d),this._updateAlternate(d),d.settings.disabled&&this._disableDatepicker(b),d.dpDiv.css("display","block"))},_dialogDatepicker:function(b,d,e,f,g){var i,j,k,l,m,n=this._dialogInst;return n||(this.uuid+=1,i="dp"+this.uuid,this._dialogInput=a("<input type='text' id='"+i+"' style='position: absolute; top: -100px; width: 0px;'/>"),this._dialogInput.keydown(this._doKeyDown),a("body").append(this._dialogInput),n=this._dialogInst=this._newInst(this._dialogInput,!1),n.settings={},a.data(this._dialogInput[0],c,n)),h(n.settings,f||{}),d=d&&d.constructor===Date?this._formatDate(n,d):d,this._dialogInput.val(d),this._pos=g?g.length?g:[g.pageX,g.pageY]:null,this._pos||(j=document.documentElement.clientWidth,k=document.documentElement.clientHeight,l=document.documentElement.scrollLeft||document.body.scrollLeft,m=document.documentElement.scrollTop||document.body.scrollTop,this._pos=[j/2-100+l,k/2-150+m]),this._dialogInput.css("left",this._pos[0]+20+"px").css("top",this._pos[1]+"px"),n.settings.onSelect=e,this._inDialog=!0,this.dpDiv.addClass(this._dialogClass),this._showDatepicker(this._dialogInput[0]),a.blockUI&&a.blockUI(this.dpDiv),a.data(this._dialogInput[0],c,n),this},_destroyDatepicker:function(b){var d,e=a(b),f=a.data(b,c);e.hasClass(this.markerClassName)&&(d=b.nodeName.toLowerCase(),a.removeData(b,c),"input"===d?(f.append.remove(),f.trigger.remove(),e.removeClass(this.markerClassName).unbind("focus",this._showDatepicker).unbind("keydown",this._doKeyDown).unbind("keypress",this._doKeyPress).unbind("keyup",this._doKeyUp)):"div"!==d&&"span"!==d||e.removeClass(this.markerClassName).empty())},_enableDatepicker:function(b){var d,e,f=a(b),g=a.data(b,c);f.hasClass(this.markerClassName)&&(d=b.nodeName.toLowerCase(),"input"===d?(b.disabled=!1,g.trigger.filter("button").each(function(){this.disabled=!1}).end().filter("img").css({opacity:"1.0",cursor:""})):"div"!==d&&"span"!==d||(e=f.children("."+this._inlineClass),e.children().removeClass("ui-state-disabled"),e.find("select.ui-datepicker-month, select.ui-datepicker-year").prop("disabled",!1)),this._disabledInputs=a.map(this._disabledInputs,function(a){return a===b?null:a}))},_disableDatepicker:function(b){var d,e,f=a(b),g=a.data(b,c);f.hasClass(this.markerClassName)&&(d=b.nodeName.toLowerCase(),"input"===d?(b.disabled=!0,g.trigger.filter("button").each(function(){this.disabled=!0}).end().filter("img").css({opacity:"0.5",cursor:"default"})):"div"!==d&&"span"!==d||(e=f.children("."+this._inlineClass),e.children().addClass("ui-state-disabled"),e.find("select.ui-datepicker-month, select.ui-datepicker-year").prop("disabled",!0)),this._disabledInputs=a.map(this._disabledInputs,function(a){return a===b?null:a}),this._disabledInputs[this._disabledInputs.length]=b)},_isDisabledDatepicker:function(a){if(!a)return!1;for(var b=0;b<this._disabledInputs.length;b++)if(this._disabledInputs[b]===a)return!0;return!1},_getInst:function(b){try{return a.data(b,c)}catch(a){throw"Missing instance data for this datepicker"}},_optionDatepicker:function(c,d,e){var f,g,i,j,k=this._getInst(c);if(2===arguments.length&&"string"==typeof d)return"defaults"===d?a.extend({},a.datepicker._defaults):k?"all"===d?a.extend({},k.settings):this._get(k,d):null;f=d||{},"string"==typeof d&&(f={},f[d]=e),k&&(this._curInst===k&&this._hideDatepicker(),g=this._getDateDatepicker(c,!0),i=this._getMinMaxDate(k,"min"),j=this._getMinMaxDate(k,"max"),h(k.settings,f),null!==i&&f.dateFormat!==b&&f.minDate===b&&(k.settings.minDate=this._formatDate(k,i)),null!==j&&f.dateFormat!==b&&f.maxDate===b&&(k.settings.maxDate=this._formatDate(k,j)),"disabled"in f&&(f.disabled?this._disableDatepicker(c):this._enableDatepicker(c)),this._attachments(a(c),k),this._autoSize(k),this._setDate(k,g),this._updateAlternate(k),this._updateDatepicker(k))},_changeDatepicker:function(a,b,c){this._optionDatepicker(a,b,c)},_refreshDatepicker:function(a){var b=this._getInst(a);b&&this._updateDatepicker(b)},_setDateDatepicker:function(a,b){var c=this._getInst(a);c&&(this._setDate(c,b),this._updateDatepicker(c),this._updateAlternate(c))},_getDateDatepicker:function(a,b){var c=this._getInst(a);return c&&!c.inline&&this._setDateFromField(c,b),c?this._getDate(c):null},_doKeyDown:function(b){var c,d,e,f=a.datepicker._getInst(b.target),g=!0,h=f.dpDiv.is(".ui-datepicker-rtl");if(f._keyEvent=!0,a.datepicker._datepickerShowing)switch(b.keyCode){case 9:a.datepicker._hideDatepicker(),g=!1;break;case 13:return e=a("td."+a.datepicker._dayOverClass+":not(."+a.datepicker._currentClass+")",f.dpDiv),e[0]&&a.datepicker._selectDay(b.target,f.selectedMonth,f.selectedYear,e[0]),c=a.datepicker._get(f,"onSelect"),c?(d=a.datepicker._formatDate(f),c.apply(f.input?f.input[0]:null,[d,f])):a.datepicker._hideDatepicker(),!1;case 27:a.datepicker._hideDatepicker();break;case 33:a.datepicker._adjustDate(b.target,b.ctrlKey?-a.datepicker._get(f,"stepBigMonths"):-a.datepicker._get(f,"stepMonths"),"M");break;case 34:a.datepicker._adjustDate(b.target,b.ctrlKey?+a.datepicker._get(f,"stepBigMonths"):+a.datepicker._get(f,"stepMonths"),"M");break;case 35:(b.ctrlKey||b.metaKey)&&a.datepicker._clearDate(b.target),g=b.ctrlKey||b.metaKey;break;case 36:(b.ctrlKey||b.metaKey)&&a.datepicker._gotoToday(b.target),g=b.ctrlKey||b.metaKey;break;case 37:(b.ctrlKey||b.metaKey)&&a.datepicker._adjustDate(b.target,h?1:-1,"D"),g=b.ctrlKey||b.metaKey,b.originalEvent.altKey&&a.datepicker._adjustDate(b.target,b.ctrlKey?-a.datepicker._get(f,"stepBigMonths"):-a.datepicker._get(f,"stepMonths"),"M");break;case 38:(b.ctrlKey||b.metaKey)&&a.datepicker._adjustDate(b.target,-7,"D"),g=b.ctrlKey||b.metaKey;break;case 39:(b.ctrlKey||b.metaKey)&&a.datepicker._adjustDate(b.target,h?-1:1,"D"),g=b.ctrlKey||b.metaKey,b.originalEvent.altKey&&a.datepicker._adjustDate(b.target,b.ctrlKey?+a.datepicker._get(f,"stepBigMonths"):+a.datepicker._get(f,"stepMonths"),"M");break;case 40:(b.ctrlKey||b.metaKey)&&a.datepicker._adjustDate(b.target,7,"D"),g=b.ctrlKey||b.metaKey;break;default:g=!1}else 36===b.keyCode&&b.ctrlKey?a.datepicker._showDatepicker(this):g=!1;g&&(b.preventDefault(),b.stopPropagation())},_doKeyPress:function(b){var c,d,e=a.datepicker._getInst(b.target);if(a.datepicker._get(e,"constrainInput"))return c=a.datepicker._possibleChars(a.datepicker._get(e,"dateFormat")),d=String.fromCharCode(null==b.charCode?b.keyCode:b.charCode),b.ctrlKey||b.metaKey||d<" "||!c||c.indexOf(d)>-1},_doKeyUp:function(b){var c,d=a.datepicker._getInst(b.target);if(d.input.val()!==d.lastVal)try{c=a.datepicker.parseDate(a.datepicker._get(d,"dateFormat"),d.input?d.input.val():null,a.datepicker._getFormatConfig(d)),c&&(a.datepicker._setDateFromField(d),a.datepicker._updateAlternate(d),a.datepicker._updateDatepicker(d))}catch(a){}return!0},_showDatepicker:function(b){if(b=b.target||b,"input"!==b.nodeName.toLowerCase()&&(b=a("input",b.parentNode)[0]),!a.datepicker._isDisabledDatepicker(b)&&a.datepicker._lastInput!==b){var c,d,e,f,g,i,j;c=a.datepicker._getInst(b),a.datepicker._curInst&&a.datepicker._curInst!==c&&(a.datepicker._curInst.dpDiv.stop(!0,!0),c&&a.datepicker._datepickerShowing&&a.datepicker._hideDatepicker(a.datepicker._curInst.input[0])),d=a.datepicker._get(c,"beforeShow"),e=d?d.apply(b,[b,c]):{},!1!==e&&(h(c.settings,e),c.lastVal=null,a.datepicker._lastInput=b,a.datepicker._setDateFromField(c),a.datepicker._inDialog&&(b.value=""),a.datepicker._pos||(a.datepicker._pos=a.datepicker._findPos(b),a.datepicker._pos[1]+=b.offsetHeight),f=!1,a(b).parents().each(function(){return!(f|="fixed"===a(this).css("position"))}),g={left:a.datepicker._pos[0],top:a.datepicker._pos[1]},a.datepicker._pos=null,c.dpDiv.empty(),c.dpDiv.css({position:"absolute",display:"block",top:"-1000px"}),a.datepicker._updateDatepicker(c),g=a.datepicker._checkOffset(c,g,f),c.dpDiv.css({position:a.datepicker._inDialog&&a.blockUI?"static":f?"fixed":"absolute",display:"none",left:g.left+"px",top:g.top+"px"}),c.inline||(i=a.datepicker._get(c,"showAnim"),j=a.datepicker._get(c,"duration"),c.dpDiv.zIndex(a(b).zIndex()+1),a.datepicker._datepickerShowing=!0,a.effects&&a.effects.effect[i]?c.dpDiv.show(i,a.datepicker._get(c,"showOptions"),j):c.dpDiv[i||"show"](i?j:null),c.input.is(":visible")&&!c.input.is(":disabled")&&c.input.focus(),a.datepicker._curInst=c))}},_updateDatepicker:function(b){this.maxRows=4,e=b,b.dpDiv.empty().append(this._generateHTML(b)),this._attachHandlers(b),b.dpDiv.find("."+this._dayOverClass+" a").mouseover();var c,d=this._getNumberOfMonths(b),f=d[1];b.dpDiv.removeClass("ui-datepicker-multi-2 ui-datepicker-multi-3 ui-datepicker-multi-4").width(""),f>1&&b.dpDiv.addClass("ui-datepicker-multi-"+f).css("width",17*f+"em"),b.dpDiv[(1!==d[0]||1!==d[1]?"add":"remove")+"Class"]("ui-datepicker-multi"),b.dpDiv[(this._get(b,"isRTL")?"add":"remove")+"Class"]("ui-datepicker-rtl"),b===a.datepicker._curInst&&a.datepicker._datepickerShowing&&b.input&&b.input.is(":visible")&&!b.input.is(":disabled")&&b.input[0]!==document.activeElement&&b.input.focus(),b.yearshtml&&(c=b.yearshtml,setTimeout(function(){c===b.yearshtml&&b.yearshtml&&b.dpDiv.find("select.ui-datepicker-year:first").replaceWith(b.yearshtml),c=b.yearshtml=null},0))},_getBorders:function(a){var b=function(a){return{thin:1,medium:2,thick:3}[a]||a};return[parseFloat(b(a.css("border-left-width"))),parseFloat(b(a.css("border-top-width")))]},_checkOffset:function(b,c,d){var e=b.dpDiv.outerWidth(),f=b.dpDiv.outerHeight(),g=b.input?b.input.outerWidth():0,h=b.input?b.input.outerHeight():0,i=document.documentElement.clientWidth+(d?0:a(document).scrollLeft()),j=document.documentElement.clientHeight+(d?0:a(document).scrollTop());return c.left-=this._get(b,"isRTL")?e-g:0,c.left-=d&&c.left===b.input.offset().left?a(document).scrollLeft():0,c.top-=d&&c.top===b.input.offset().top+h?a(document).scrollTop():0,c.left-=Math.min(c.left,c.left+e>i&&i>e?Math.abs(c.left+e-i):0),c.top-=Math.min(c.top,c.top+f>j&&j>f?Math.abs(f+h):0),c},_findPos:function(b){for(var c,d=this._getInst(b),e=this._get(d,"isRTL");b&&("hidden"===b.type||1!==b.nodeType||a.expr.filters.hidden(b));)b=b[e?"previousSibling":"nextSibling"];return c=a(b).offset(),[c.left,c.top]},_hideDatepicker:function(b){var d,e,f,g,h=this._curInst;!h||b&&h!==a.data(b,c)||this._datepickerShowing&&(d=this._get(h,"showAnim"),e=this._get(h,"duration"),f=function(){a.datepicker._tidyDialog(h)},a.effects&&(a.effects.effect[d]||a.effects[d])?h.dpDiv.hide(d,a.datepicker._get(h,"showOptions"),e,f):h.dpDiv["slideDown"===d?"slideUp":"fadeIn"===d?"fadeOut":"hide"](d?e:null,f),d||f(),this._datepickerShowing=!1,g=this._get(h,"onClose"),g&&g.apply(h.input?h.input[0]:null,[h.input?h.input.val():"",h]),this._lastInput=null,this._inDialog&&(this._dialogInput.css({position:"absolute",left:"0",top:"-100px"}),a.blockUI&&(a.unblockUI(),a("body").append(this.dpDiv))),this._inDialog=!1)},_tidyDialog:function(a){a.dpDiv.removeClass(this._dialogClass).unbind(".ui-datepicker-calendar")},_checkExternalClick:function(b){if(a.datepicker._curInst){var c=a(b.target),d=a.datepicker._getInst(c[0]);(c[0].id===a.datepicker._mainDivId||0!==c.parents("#"+a.datepicker._mainDivId).length||c.hasClass(a.datepicker.markerClassName)||c.closest("."+a.datepicker._triggerClass).length||!a.datepicker._datepickerShowing||a.datepicker._inDialog&&a.blockUI)&&(!c.hasClass(a.datepicker.markerClassName)||a.datepicker._curInst===d)||a.datepicker._hideDatepicker()}},_adjustDate:function(b,c,d){var e=a(b),f=this._getInst(e[0]);this._isDisabledDatepicker(e[0])||(this._adjustInstDate(f,c+("M"===d?this._get(f,"showCurrentAtPos"):0),d),this._updateDatepicker(f))},_gotoToday:function(b){var c,d=a(b),e=this._getInst(d[0]);this._get(e,"gotoCurrent")&&e.currentDay?(e.selectedDay=e.currentDay,e.drawMonth=e.selectedMonth=e.currentMonth,e.drawYear=e.selectedYear=e.currentYear):(c=new Date,e.selectedDay=c.getDate(),e.drawMonth=e.selectedMonth=c.getMonth(),e.drawYear=e.selectedYear=c.getFullYear()),this._notifyChange(e),this._adjustDate(d)},_selectMonthYear:function(b,c,d){var e=a(b),f=this._getInst(e[0]);f["selected"+("M"===d?"Month":"Year")]=f["draw"+("M"===d?"Month":"Year")]=parseInt(c.options[c.selectedIndex].value,10),this._notifyChange(f),this._adjustDate(e)},_selectDay:function(b,c,d,e){var f,g=a(b);a(e).hasClass(this._unselectableClass)||this._isDisabledDatepicker(g[0])||(f=this._getInst(g[0]),f.selectedDay=f.currentDay=a("a",e).html(),f.selectedMonth=f.currentMonth=c,f.selectedYear=f.currentYear=d,this._selectDate(b,this._formatDate(f,f.currentDay,f.currentMonth,f.currentYear)))},_clearDate:function(b){var c=a(b);this._selectDate(c,"")},_selectDate:function(b,c){var d,e=a(b),f=this._getInst(e[0]);c=null!=c?c:this._formatDate(f),f.input&&f.input.val(c),this._updateAlternate(f),d=this._get(f,"onSelect"),d?d.apply(f.input?f.input[0]:null,[c,f]):f.input&&f.input.trigger("change"),f.inline?this._updateDatepicker(f):(this._hideDatepicker(),this._lastInput=f.input[0],"object"!=typeof f.input[0]&&f.input.focus(),this._lastInput=null)},_updateAlternate:function(b){var c,d,e,f=this._get(b,"altField");f&&(c=this._get(b,"altFormat")||this._get(b,"dateFormat"),d=this._getDate(b),e=this.formatDate(c,d,this._getFormatConfig(b)),a(f).each(function(){a(this).val(e)}))},noWeekends:function(a){var b=a.getDay();return[b>0&&b<6,""]},iso8601Week:function(a){var b,c=new Date(a.getTime());return c.setDate(c.getDate()+4-(c.getDay()||7)),b=c.getTime(),c.setMonth(0),c.setDate(1),Math.floor(Math.round((b-c)/864e5)/7)+1},parseDate:function(b,c,d){if(null==b||null==c)throw"Invalid arguments";if(""===(c="object"==typeof c?c.toString():c+""))return null;var e,f,g,t,h=0,i=(d?d.shortYearCutoff:null)||this._defaults.shortYearCutoff,j="string"!=typeof i?i:(new Date).getFullYear()%100+parseInt(i,10),k=(d?d.dayNamesShort:null)||this._defaults.dayNamesShort,l=(d?d.dayNames:null)||this._defaults.dayNames,m=(d?d.monthNamesShort:null)||this._defaults.monthNamesShort,n=(d?d.monthNames:null)||this._defaults.monthNames,o=-1,p=-1,q=-1,r=-1,s=!1,u=function(a){var c=e+1<b.length&&b.charAt(e+1)===a;return c&&e++,c},v=function(a){var b=u(a),d="@"===a?14:"!"===a?20:"y"===a&&b?4:"o"===a?3:2,e=new RegExp("^\\d{1,"+d+"}"),f=c.substring(h).match(e);if(!f)throw"Missing number at position "+h;return h+=f[0].length,parseInt(f[0],10)},w=function(b,d,e){var f=-1,g=a.map(u(b)?e:d,function(a,b){return[[b,a]]}).sort(function(a,b){return-(a[1].length-b[1].length)});if(a.each(g,function(a,b){var d=b[1];if(c.substr(h,d.length).toLowerCase()===d.toLowerCase())return f=b[0],h+=d.length,!1}),-1!==f)return f+1;throw"Unknown name at position "+h},x=function(){if(c.charAt(h)!==b.charAt(e))throw"Unexpected literal at position "+h;h++};for(e=0;e<b.length;e++)if(s)"'"!==b.charAt(e)||u("'")?x():s=!1;else switch(b.charAt(e)){case"d":q=v("d");break;case"D":w("D",k,l);break;case"o":r=v("o");break;case"m":p=v("m");break;case"M":p=w("M",m,n);break;case"y":o=v("y");break;case"@":t=new Date(v("@")),o=t.getFullYear(),p=t.getMonth()+1,q=t.getDate();break;case"!":t=new Date((v("!")-this._ticksTo1970)/1e4),o=t.getFullYear(),p=t.getMonth()+1,q=t.getDate();break;case"'":u("'")?x():s=!0;break;default:x()}if(h<c.length&&(g=c.substr(h),!/^\s+/.test(g)))throw"Extra/unparsed characters found in date: "+g;if(-1===o?o=(new Date).getFullYear():o<100&&(o+=(new Date).getFullYear()-(new Date).getFullYear()%100+(o<=j?0:-100)),r>-1)for(p=1,q=r;;){if(f=this._getDaysInMonth(o,p-1),q<=f)break;p++,q-=f}if(t=this._daylightSavingAdjust(new Date(o,p-1,q)),t.getFullYear()!==o||t.getMonth()+1!==p||t.getDate()!==q)throw"Invalid date";return t},ATOM:"yy-mm-dd",COOKIE:"D, dd M yy",ISO_8601:"yy-mm-dd",RFC_822:"D, d M y",RFC_850:"DD, dd-M-y",RFC_1036:"D, d M y",RFC_1123:"D, d M yy",RFC_2822:"D, d M yy",RSS:"D, d M y",TICKS:"!",TIMESTAMP:"@",W3C:"yy-mm-dd",_ticksTo1970:24*(718685+Math.floor(492.5)-Math.floor(19.7)+Math.floor(4.925))*60*60*1e7,formatDate:function(a,b,c){if(!b)return"";var d,e=(c?c.dayNamesShort:null)||this._defaults.dayNamesShort,f=(c?c.dayNames:null)||this._defaults.dayNames,g=(c?c.monthNamesShort:null)||this._defaults.monthNamesShort,h=(c?c.monthNames:null)||this._defaults.monthNames,i=function(b){var c=d+1<a.length&&a.charAt(d+1)===b;return c&&d++,c},j=function(a,b,c){var d=""+b;if(i(a))for(;d.length<c;)d="0"+d;return d},k=function(a,b,c,d){return i(a)?d[b]:c[b]},l="",m=!1;if(b)for(d=0;d<a.length;d++)if(m)"'"!==a.charAt(d)||i("'")?l+=a.charAt(d):m=!1;else switch(a.charAt(d)){case"d":l+=j("d",b.getDate(),2);break;case"D":l+=k("D",b.getDay(),e,f);break;case"o":l+=j("o",Math.round((new Date(b.getFullYear(),b.getMonth(),b.getDate()).getTime()-new Date(b.getFullYear(),0,0).getTime())/864e5),3);break;case"m":l+=j("m",b.getMonth()+1,2);break;case"M":l+=k("M",b.getMonth(),g,h);break;case"y":l+=i("y")?b.getFullYear():(b.getYear()%100<10?"0":"")+b.getYear()%100;break;case"@":l+=b.getTime();break;case"!":l+=1e4*b.getTime()+this._ticksTo1970;break;case"'":i("'")?l+="'":m=!0;break;default:l+=a.charAt(d)}return l},_possibleChars:function(a){var b,c="",d=!1,e=function(c){var d=b+1<a.length&&a.charAt(b+1)===c;return d&&b++,d};for(b=0;b<a.length;b++)if(d)"'"!==a.charAt(b)||e("'")?c+=a.charAt(b):d=!1;else switch(a.charAt(b)){case"d":case"m":case"y":case"@":c+="0123456789";break;case"D":case"M":return null;case"'":e("'")?c+="'":d=!0;break;default:c+=a.charAt(b)}return c},_get:function(a,c){return a.settings[c]!==b?a.settings[c]:this._defaults[c]},_setDateFromField:function(a,b){if(a.input.val()!==a.lastVal){var c=this._get(a,"dateFormat"),d=a.lastVal=a.input?a.input.val():null,e=this._getDefaultDate(a),f=e,g=this._getFormatConfig(a);try{f=this.parseDate(c,d,g)||e}catch(a){d=b?"":d}a.selectedDay=f.getDate(),a.drawMonth=a.selectedMonth=f.getMonth(),a.drawYear=a.selectedYear=f.getFullYear(),a.currentDay=d?f.getDate():0,a.currentMonth=d?f.getMonth():0,a.currentYear=d?f.getFullYear():0,this._adjustInstDate(a)}},_getDefaultDate:function(a){return this._restrictMinMax(a,this._determineDate(a,this._get(a,"defaultDate"),new Date))},_determineDate:function(b,c,d){var e=function(a){var b=new Date;return b.setDate(b.getDate()+a),b},f=function(c){try{return a.datepicker.parseDate(a.datepicker._get(b,"dateFormat"),c,a.datepicker._getFormatConfig(b))}catch(a){}for(var d=(c.toLowerCase().match(/^c/)?a.datepicker._getDate(b):null)||new Date,e=d.getFullYear(),f=d.getMonth(),g=d.getDate(),h=/([+\-]?[0-9]+)\s*(d|D|w|W|m|M|y|Y)?/g,i=h.exec(c);i;){switch(i[2]||"d"){case"d":case"D":g+=parseInt(i[1],10);break;case"w":case"W":g+=7*parseInt(i[1],10);break;case"m":case"M":f+=parseInt(i[1],10),g=Math.min(g,a.datepicker._getDaysInMonth(e,f));break;case"y":case"Y":e+=parseInt(i[1],10),g=Math.min(g,a.datepicker._getDaysInMonth(e,f))}i=h.exec(c)}return new Date(e,f,g)},g=null==c||""===c?d:"string"==typeof c?f(c):"number"==typeof c?isNaN(c)?d:e(c):new Date(c.getTime());return g=g&&"Invalid Date"===g.toString()?d:g,g&&(g.setHours(0),g.setMinutes(0),g.setSeconds(0),g.setMilliseconds(0)),this._daylightSavingAdjust(g)},_daylightSavingAdjust:function(a){return a?(a.setHours(a.getHours()>12?a.getHours()+2:0),a):null},_setDate:function(a,b,c){var d=!b,e=a.selectedMonth,f=a.selectedYear,g=this._restrictMinMax(a,this._determineDate(a,b,new Date));a.selectedDay=a.currentDay=g.getDate(),a.drawMonth=a.selectedMonth=a.currentMonth=g.getMonth(),a.drawYear=a.selectedYear=a.currentYear=g.getFullYear(),e===a.selectedMonth&&f===a.selectedYear||c||this._notifyChange(a),this._adjustInstDate(a),a.input&&a.input.val(d?"":this._formatDate(a))},_getDate:function(a){return!a.currentYear||a.input&&""===a.input.val()?null:this._daylightSavingAdjust(new Date(a.currentYear,a.currentMonth,a.currentDay))},_attachHandlers:function(b){var c=this._get(b,"stepMonths"),e="#"+b.id.replace(/\\\\/g,"\\");b.dpDiv.find("[data-handler]").map(function(){var b={prev:function(){window["DP_jQuery_"+d].datepicker._adjustDate(e,-c,"M")},next:function(){window["DP_jQuery_"+d].datepicker._adjustDate(e,+c,"M")},hide:function(){window["DP_jQuery_"+d].datepicker._hideDatepicker()},today:function(){window["DP_jQuery_"+d].datepicker._gotoToday(e)},selectDay:function(){return window["DP_jQuery_"+d].datepicker._selectDay(e,+this.getAttribute("data-month"),+this.getAttribute("data-year"),this),!1},selectMonth:function(){return window["DP_jQuery_"+d].datepicker._selectMonthYear(e,this,"M"),!1},selectYear:function(){return window["DP_jQuery_"+d].datepicker._selectMonthYear(e,this,"Y"),!1}};a(this).bind(this.getAttribute("data-event"),b[this.getAttribute("data-handler")])})},_generateHTML:function(a){var b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,A,B,C,D,E,F,G,H,I,J,K,L,M,N,O=new Date,P=this._daylightSavingAdjust(new Date(O.getFullYear(),O.getMonth(),O.getDate())),Q=this._get(a,"isRTL"),R=this._get(a,"showButtonPanel"),S=this._get(a,"hideIfNoPrevNext"),T=this._get(a,"navigationAsDateFormat"),U=this._getNumberOfMonths(a),V=this._get(a,"showCurrentAtPos"),W=this._get(a,"stepMonths"),X=1!==U[0]||1!==U[1],Y=this._daylightSavingAdjust(a.currentDay?new Date(a.currentYear,a.currentMonth,a.currentDay):new Date(9999,9,9)),Z=this._getMinMaxDate(a,"min"),$=this._getMinMaxDate(a,"max"),_=a.drawMonth-V,aa=a.drawYear;if(_<0&&(_+=12,aa--),$)for(b=this._daylightSavingAdjust(new Date($.getFullYear(),$.getMonth()-U[0]*U[1]+1,$.getDate())),b=Z&&b<Z?Z:b;this._daylightSavingAdjust(new Date(aa,_,1))>b;)--_<0&&(_=11,aa--);for(a.drawMonth=_,a.drawYear=aa,c=this._get(a,"prevText"),c=T?this.formatDate(c,this._daylightSavingAdjust(new Date(aa,_-W,1)),this._getFormatConfig(a)):c,d=this._canAdjustMonth(a,-1,aa,_)?"<a class='ui-datepicker-prev ui-corner-all' data-handler='prev' data-event='click' title='"+c+"'><span class='ui-icon ui-icon-circle-triangle-"+(Q?"e":"w")+"'>"+c+"</span></a>":S?"":"<a class='ui-datepicker-prev ui-corner-all ui-state-disabled' title='"+c+"'><span class='ui-icon ui-icon-circle-triangle-"+(Q?"e":"w")+"'>"+c+"</span></a>",e=this._get(a,"nextText"),e=T?this.formatDate(e,this._daylightSavingAdjust(new Date(aa,_+W,1)),this._getFormatConfig(a)):e,f=this._canAdjustMonth(a,1,aa,_)?"<a class='ui-datepicker-next ui-corner-all' data-handler='next' data-event='click' title='"+e+"'><span class='ui-icon ui-icon-circle-triangle-"+(Q?"w":"e")+"'>"+e+"</span></a>":S?"":"<a class='ui-datepicker-next ui-corner-all ui-state-disabled' title='"+e+"'><span class='ui-icon ui-icon-circle-triangle-"+(Q?"w":"e")+"'>"+e+"</span></a>",g=this._get(a,"currentText"),h=this._get(a,"gotoCurrent")&&a.currentDay?Y:P,g=T?this.formatDate(g,h,this._getFormatConfig(a)):g,i=a.inline?"":"<button type='button' class='ui-datepicker-close ui-state-default ui-priority-primary ui-corner-all' data-handler='hide' data-event='click'>"+this._get(a,"closeText")+"</button>",j=R?"<div class='ui-datepicker-buttonpane ui-widget-content'>"+(Q?i:"")+(this._isInRange(a,h)?"<button type='button' class='ui-datepicker-current ui-state-default ui-priority-secondary ui-corner-all' data-handler='today' data-event='click'>"+g+"</button>":"")+(Q?"":i)+"</div>":"",k=parseInt(this._get(a,"firstDay"),10),k=isNaN(k)?0:k,l=this._get(a,"showWeek"),m=this._get(a,"dayNames"),n=this._get(a,"dayNamesMin"),o=this._get(a,"monthNames"),p=this._get(a,"monthNamesShort"),q=this._get(a,"beforeShowDay"),r=this._get(a,"showOtherMonths"),s=this._get(a,"selectOtherMonths"),t=this._getDefaultDate(a),u="",w=0;w<U[0];w++){for(x="",this.maxRows=4,y=0;y<U[1];y++){if(z=this._daylightSavingAdjust(new Date(aa,_,a.selectedDay)),A=" ui-corner-all",B="",X){if(B+="<div class='ui-datepicker-group",U[1]>1)switch(y){case 0:B+=" ui-datepicker-group-first",A=" ui-corner-"+(Q?"right":"left");break;case U[1]-1:B+=" ui-datepicker-group-last",A=" ui-corner-"+(Q?"left":"right");break;default:B+=" ui-datepicker-group-middle",A=""}B+="'>"}for(B+="<div class='ui-datepicker-header ui-widget-header ui-helper-clearfix"+A+"'>"+(/all|left/.test(A)&&0===w?Q?f:d:"")+(/all|right/.test(A)&&0===w?Q?d:f:"")+this._generateMonthYearHeader(a,_,aa,Z,$,w>0||y>0,o,p)+"</div><table class='ui-datepicker-calendar'><thead><tr>",C=l?"<th class='ui-datepicker-week-col'>"+this._get(a,"weekHeader")+"</th>":"",v=0;v<7;v++)D=(v+k)%7,C+="<th"+((v+k+6)%7>=5?" class='ui-datepicker-week-end'":"")+"><span title='"+m[D]+"'>"+n[D]+"</span></th>";for(B+=C+"</tr></thead><tbody>",E=this._getDaysInMonth(aa,_),aa===a.selectedYear&&_===a.selectedMonth&&(a.selectedDay=Math.min(a.selectedDay,E)),F=(this._getFirstDayOfMonth(aa,_)-k+7)%7,G=Math.ceil((F+E)/7),H=X&&this.maxRows>G?this.maxRows:G,this.maxRows=H,I=this._daylightSavingAdjust(new Date(aa,_,1-F)),J=0;J<H;J++){for(B+="<tr>",K=l?"<td class='ui-datepicker-week-col'>"+this._get(a,"calculateWeek")(I)+"</td>":"",v=0;v<7;v++)L=q?q.apply(a.input?a.input[0]:null,[I]):[!0,""],M=I.getMonth()!==_,N=M&&!s||!L[0]||Z&&I<Z||$&&I>$,K+="<td class='"+((v+k+6)%7>=5?" ui-datepicker-week-end":"")+(M?" ui-datepicker-other-month":"")+(I.getTime()===z.getTime()&&_===a.selectedMonth&&a._keyEvent||t.getTime()===I.getTime()&&t.getTime()===z.getTime()?" "+this._dayOverClass:"")+(N?" "+this._unselectableClass+" ui-state-disabled":"")+(M&&!r?"":" "+L[1]+(I.getTime()===Y.getTime()?" "+this._currentClass:"")+(I.getTime()===P.getTime()?" ui-datepicker-today":""))+"'"+(M&&!r||!L[2]?"":" title='"+L[2].replace(/'/g,"&#39;")+"'")+(N?"":" data-handler='selectDay' data-event='click' data-month='"+I.getMonth()+"' data-year='"+I.getFullYear()+"'")+">"+(M&&!r?"&#xa0;":N?"<span class='ui-state-default'>"+I.getDate()+"</span>":"<a class='ui-state-default"+(I.getTime()===P.getTime()?" ui-state-highlight":"")+(I.getTime()===Y.getTime()?" ui-state-active":"")+(M?" ui-priority-secondary":"")+"' href='#'>"+I.getDate()+"</a>")+"</td>",I.setDate(I.getDate()+1),I=this._daylightSavingAdjust(I);B+=K+"</tr>"}_++,_>11&&(_=0,aa++),B+="</tbody></table>"+(X?"</div>"+(U[0]>0&&y===U[1]-1?"<div class='ui-datepicker-row-break'></div>":""):""),x+=B}u+=x}return u+=j,a._keyEvent=!1,u},_generateMonthYearHeader:function(a,b,c,d,e,f,g,h){var i,j,k,l,m,n,o,p,q=this._get(a,"changeMonth"),r=this._get(a,"changeYear"),s=this._get(a,"showMonthAfterYear"),t="<div class='ui-datepicker-title'>",u="";if(f||!q)u+="<span class='ui-datepicker-month'>"+g[b]+"</span>";else{for(i=d&&d.getFullYear()===c,j=e&&e.getFullYear()===c,u+="<select class='ui-datepicker-month' data-handler='selectMonth' data-event='change'>",k=0;k<12;k++)(!i||k>=d.getMonth())&&(!j||k<=e.getMonth())&&(u+="<option value='"+k+"'"+(k===b?" selected='selected'":"")+">"+h[k]+"</option>");u+="</select>"}if(s||(t+=u+(!f&&q&&r?"":"&#xa0;")),!a.yearshtml)if(a.yearshtml="",f||!r)t+="<span class='ui-datepicker-year'>"+c+"</span>";else{for(l=this._get(a,"yearRange").split(":"),m=(new Date).getFullYear(),n=function(a){var b=a.match(/c[+\-].*/)?c+parseInt(a.substring(1),10):a.match(/[+\-].*/)?m+parseInt(a,10):parseInt(a,10);return isNaN(b)?m:b},o=n(l[0]),p=Math.max(o,n(l[1]||"")),o=d?Math.max(o,d.getFullYear()):o,p=e?Math.min(p,e.getFullYear()):p,a.yearshtml+="<select class='ui-datepicker-year' data-handler='selectYear' data-event='change'>";o<=p;o++)a.yearshtml+="<option value='"+o+"'"+(o===c?" selected='selected'":"")+">"+o+"</option>";a.yearshtml+="</select>",t+=a.yearshtml,a.yearshtml=null}return t+=this._get(a,"yearSuffix"),s&&(t+=(!f&&q&&r?"":"&#xa0;")+u),t+="</div>"},_adjustInstDate:function(a,b,c){var d=a.drawYear+("Y"===c?b:0),e=a.drawMonth+("M"===c?b:0),f=Math.min(a.selectedDay,this._getDaysInMonth(d,e))+("D"===c?b:0),g=this._restrictMinMax(a,this._daylightSavingAdjust(new Date(d,e,f)));a.selectedDay=g.getDate(),a.drawMonth=a.selectedMonth=g.getMonth(),a.drawYear=a.selectedYear=g.getFullYear(),"M"!==c&&"Y"!==c||this._notifyChange(a)},_restrictMinMax:function(a,b){var c=this._getMinMaxDate(a,"min"),d=this._getMinMaxDate(a,"max"),e=c&&b<c?c:b;return d&&e>d?d:e},_notifyChange:function(a){var b=this._get(a,"onChangeMonthYear");b&&b.apply(a.input?a.input[0]:null,[a.selectedYear,a.selectedMonth+1,a])},_getNumberOfMonths:function(a){var b=this._get(a,"numberOfMonths");return null==b?[1,1]:"number"==typeof b?[1,b]:b},_getMinMaxDate:function(a,b){return this._determineDate(a,this._get(a,b+"Date"),null)},_getDaysInMonth:function(a,b){return 32-this._daylightSavingAdjust(new Date(a,b,32)).getDate()},_getFirstDayOfMonth:function(a,b){return new Date(a,b,1).getDay()},_canAdjustMonth:function(a,b,c,d){var e=this._getNumberOfMonths(a),f=this._daylightSavingAdjust(new Date(c,d+(b<0?b:e[0]*e[1]),1));return b<0&&f.setDate(this._getDaysInMonth(f.getFullYear(),f.getMonth())),this._isInRange(a,f)},_isInRange:function(a,b){var c,d,e=this._getMinMaxDate(a,"min"),f=this._getMinMaxDate(a,"max"),g=null,h=null,i=this._get(a,"yearRange");return i&&(c=i.split(":"),d=(new Date).getFullYear(),g=parseInt(c[0],10),h=parseInt(c[1],10),c[0].match(/[+\-].*/)&&(g+=d),c[1].match(/[+\-].*/)&&(h+=d)),(!e||b.getTime()>=e.getTime())&&(!f||b.getTime()<=f.getTime())&&(!g||b.getFullYear()>=g)&&(!h||b.getFullYear()<=h)},_getFormatConfig:function(a){var b=this._get(a,"shortYearCutoff");return b="string"!=typeof b?b:(new Date).getFullYear()%100+parseInt(b,10),{shortYearCutoff:b,dayNamesShort:this._get(a,"dayNamesShort"),dayNames:this._get(a,"dayNames"),monthNamesShort:this._get(a,"monthNamesShort"),monthNames:this._get(a,"monthNames")}},_formatDate:function(a,b,c,d){b||(a.currentDay=a.selectedDay,a.currentMonth=a.selectedMonth,a.currentYear=a.selectedYear);var e=b?"object"==typeof b?b:this._daylightSavingAdjust(new Date(d,c,b)):this._daylightSavingAdjust(new Date(a.currentYear,a.currentMonth,a.currentDay));return this.formatDate(this._get(a,"dateFormat"),e,this._getFormatConfig(a))}}),a.fn.datepicker=function(b){if(!this.length)return this;a.datepicker.initialized||(a(document).mousedown(a.datepicker._checkExternalClick),a.datepicker.initialized=!0),0===a("#"+a.datepicker._mainDivId).length&&a("body").append(a.datepicker.dpDiv);var c=Array.prototype.slice.call(arguments,1);return"string"!=typeof b||"isDisabled"!==b&&"getDate"!==b&&"widget"!==b?"option"===b&&2===arguments.length&&"string"==typeof arguments[1]?a.datepicker["_"+b+"Datepicker"].apply(a.datepicker,[this[0]].concat(c)):this.each(function(){"string"==typeof b?a.datepicker["_"+b+"Datepicker"].apply(a.datepicker,[this].concat(c)):a.datepicker._attachDatepicker(this,b)}):a.datepicker["_"+b+"Datepicker"].apply(a.datepicker,[this[0]].concat(c))},a.datepicker=new f,a.datepicker.initialized=!1,a.datepicker.uuid=(new Date).getTime(),a.datepicker.version="1.10.2",window["DP_jQuery_"+d]=a}(jQuery),function(a,b){var c={buttons:!0,height:!0,maxHeight:!0,maxWidth:!0,minHeight:!0,minWidth:!0,width:!0},d={maxHeight:!0,maxWidth:!0,minHeight:!0,minWidth:!0};a.widget("ui.dialog",{version:"1.10.2",options:{appendTo:"body",autoOpen:!0,buttons:[],closeOnEscape:!0,closeText:"close",dialogClass:"",draggable:!0,hide:null,height:"auto",maxHeight:null,maxWidth:null,minHeight:150,minWidth:150,modal:!1,position:{my:"center",at:"center",of:window,collision:"fit",using:function(b){var c=a(this).css(b).offset().top;c<0&&a(this).css("top",b.top-c)}},resizable:!0,show:null,title:null,width:300,beforeClose:null,close:null,drag:null,dragStart:null,dragStop:null,focus:null,open:null,resize:null,resizeStart:null,resizeStop:null},_create:function(){this.originalCss={display:this.element[0].style.display,width:this.element[0].style.width,minHeight:this.element[0].style.minHeight,maxHeight:this.element[0].style.maxHeight,height:this.element[0].style.height},this.originalPosition={parent:this.element.parent(),index:this.element.parent().children().index(this.element)},this.originalTitle=this.element.attr("title"),this.options.title=this.options.title||this.originalTitle,this._createWrapper(),this.element.show().removeAttr("title").addClass("ui-dialog-content ui-widget-content").appendTo(this.uiDialog),this._createTitlebar(),this._createButtonPane(),this.options.draggable&&a.fn.draggable&&this._makeDraggable(),this.options.resizable&&a.fn.resizable&&this._makeResizable(),this._isOpen=!1},_init:function(){this.options.autoOpen&&this.open()},_appendTo:function(){var b=this.options.appendTo;return b&&(b.jquery||b.nodeType)?a(b):this.document.find(b||"body").eq(0)},_destroy:function(){var a,b=this.originalPosition;this._destroyOverlay(),this.element.removeUniqueId().removeClass("ui-dialog-content ui-widget-content").css(this.originalCss).detach(),this.uiDialog.stop(!0,!0).remove(),this.originalTitle&&this.element.attr("title",this.originalTitle),a=b.parent.children().eq(b.index),a.length&&a[0]!==this.element[0]?a.before(this.element):b.parent.append(this.element)},widget:function(){return this.uiDialog},disable:a.noop,enable:a.noop,close:function(b){var c=this;this._isOpen&&!1!==this._trigger("beforeClose",b)&&(this._isOpen=!1,this._destroyOverlay(),this.opener.filter(":focusable").focus().length||a(this.document[0].activeElement).blur(),this._hide(this.uiDialog,this.options.hide,function(){c._trigger("close",b)}))},isOpen:function(){return this._isOpen},moveToTop:function(){this._moveToTop()},_moveToTop:function(a,b){var c=!!this.uiDialog.nextAll(":visible").insertBefore(this.uiDialog).length;return c&&!b&&this._trigger("focus",a),c},open:function(){var b=this;if(this._isOpen)return void(this._moveToTop()&&this._focusTabbable());this._isOpen=!0,this.opener=a(this.document[0].activeElement),this._size(),this._position(),this._createOverlay(),this._moveToTop(null,!0),this._show(this.uiDialog,this.options.show,function(){b._focusTabbable(),b._trigger("focus")}),this._trigger("open")},_focusTabbable:function(){var a=this.element.find("[autofocus]");a.length||(a=this.element.find(":tabbable")),a.length||(a=this.uiDialogButtonPane.find(":tabbable")),a.length||(a=this.uiDialogTitlebarClose.filter(":tabbable")),a.length||(a=this.uiDialog),a.eq(0).focus()},_keepFocus:function(b){function c(){var b=this.document[0].activeElement;this.uiDialog[0]===b||a.contains(this.uiDialog[0],b)||this._focusTabbable()}b.preventDefault(),c.call(this),this._delay(c)},_createWrapper:function(){this.uiDialog=a("<div>").addClass("ui-dialog ui-widget ui-widget-content ui-corner-all ui-front "+this.options.dialogClass).hide().attr({tabIndex:-1,role:"dialog"}).appendTo(this._appendTo()),this._on(this.uiDialog,{keydown:function(b){if(this.options.closeOnEscape&&!b.isDefaultPrevented()&&b.keyCode&&b.keyCode===a.ui.keyCode.ESCAPE)return b.preventDefault(),void this.close(b);if(b.keyCode===a.ui.keyCode.TAB){var c=this.uiDialog.find(":tabbable"),d=c.filter(":first"),e=c.filter(":last");b.target!==e[0]&&b.target!==this.uiDialog[0]||b.shiftKey?b.target!==d[0]&&b.target!==this.uiDialog[0]||!b.shiftKey||(e.focus(1),b.preventDefault()):(d.focus(1),b.preventDefault())}},mousedown:function(a){this._moveToTop(a)&&this._focusTabbable()}}),this.element.find("[aria-describedby]").length||this.uiDialog.attr({"aria-describedby":this.element.uniqueId().attr("id")})},_createTitlebar:function(){var b;this.uiDialogTitlebar=a("<div>").addClass("ui-dialog-titlebar ui-widget-header ui-corner-all ui-helper-clearfix").prependTo(this.uiDialog),this._on(this.uiDialogTitlebar,{mousedown:function(b){a(b.target).closest(".ui-dialog-titlebar-close")||this.uiDialog.focus()}}),this.uiDialogTitlebarClose=a("<button></button>").button({label:this.options.closeText,icons:{primary:"ui-icon-closethick"},text:!1}).addClass("ui-dialog-titlebar-close").appendTo(this.uiDialogTitlebar),this._on(this.uiDialogTitlebarClose,{click:function(a){a.preventDefault(),this.close(a)}}),b=a("<span>").uniqueId().addClass("ui-dialog-title").prependTo(this.uiDialogTitlebar),this._title(b),this.uiDialog.attr({"aria-labelledby":b.attr("id")})},_title:function(a){this.options.title||a.html("&#160;"),a.text(this.options.title)},_createButtonPane:function(){this.uiDialogButtonPane=a("<div>").addClass("ui-dialog-buttonpane ui-widget-content ui-helper-clearfix"),this.uiButtonSet=a("<div>").addClass("ui-dialog-buttonset").appendTo(this.uiDialogButtonPane),this._createButtons()},_createButtons:function(){var b=this,c=this.options.buttons;if(this.uiDialogButtonPane.remove(),this.uiButtonSet.empty(),a.isEmptyObject(c)||a.isArray(c)&&!c.length)return void this.uiDialog.removeClass("ui-dialog-buttons");a.each(c,function(c,d){var e,f;d=a.isFunction(d)?{click:d,text:c}:d,d=a.extend({type:"button"},d),e=d.click,d.click=function(){e.apply(b.element[0],arguments)},f={icons:d.icons,text:d.showText},delete d.icons,delete d.showText,a("<button></button>",d).button(f).appendTo(b.uiButtonSet)}),this.uiDialog.addClass("ui-dialog-buttons"),this.uiDialogButtonPane.appendTo(this.uiDialog)},_makeDraggable:function(){function d(a){return{position:a.position,offset:a.offset}}var b=this,c=this.options;this.uiDialog.draggable({cancel:".ui-dialog-content, .ui-dialog-titlebar-close",handle:".ui-dialog-titlebar",containment:"document",start:function(c,e){a(this).addClass("ui-dialog-dragging"),b._blockFrames(),b._trigger("dragStart",c,d(e))},drag:function(a,c){b._trigger("drag",a,d(c))},stop:function(e,f){c.position=[f.position.left-b.document.scrollLeft(),f.position.top-b.document.scrollTop()],a(this).removeClass("ui-dialog-dragging"),b._unblockFrames(),b._trigger("dragStop",e,d(f))}})},_makeResizable:function(){function g(a){return{originalPosition:a.originalPosition,originalSize:a.originalSize,position:a.position,size:a.size}}var b=this,c=this.options,d=c.resizable,e=this.uiDialog.css("position"),f="string"==typeof d?d:"n,e,s,w,se,sw,ne,nw";this.uiDialog.resizable({cancel:".ui-dialog-content",containment:"document",alsoResize:this.element,maxWidth:c.maxWidth,maxHeight:c.maxHeight,minWidth:c.minWidth,minHeight:this._minHeight(),handles:f,start:function(c,d){a(this).addClass("ui-dialog-resizing"),b._blockFrames(),b._trigger("resizeStart",c,g(d))},resize:function(a,c){b._trigger("resize",a,g(c))},stop:function(d,e){c.height=a(this).height(),c.width=a(this).width(),a(this).removeClass("ui-dialog-resizing"),b._unblockFrames(),b._trigger("resizeStop",d,g(e))}}).css("position",e)},_minHeight:function(){var a=this.options;return"auto"===a.height?a.minHeight:Math.min(a.minHeight,a.height)},_position:function(){var a=this.uiDialog.is(":visible");a||this.uiDialog.show(),this.uiDialog.position(this.options.position),a||this.uiDialog.hide()},_setOptions:function(b){var e=this,f=!1,g={};a.each(b,function(a,b){e._setOption(a,b),a in c&&(f=!0),a in d&&(g[a]=b)}),f&&(this._size(),this._position()),this.uiDialog.is(":data(ui-resizable)")&&this.uiDialog.resizable("option",g)},_setOption:function(a,b){var c,d,e=this.uiDialog;"dialogClass"===a&&e.removeClass(this.options.dialogClass).addClass(b),"disabled"!==a&&(this._super(a,b),"appendTo"===a&&this.uiDialog.appendTo(this._appendTo()),"buttons"===a&&this._createButtons(),"closeText"===a&&this.uiDialogTitlebarClose.button({label:""+b}),"draggable"===a&&(c=e.is(":data(ui-draggable)"),c&&!b&&e.draggable("destroy"),!c&&b&&this._makeDraggable()),"position"===a&&this._position(),"resizable"===a&&(d=e.is(":data(ui-resizable)"),d&&!b&&e.resizable("destroy"),d&&"string"==typeof b&&e.resizable("option","handles",b),d||!1===b||this._makeResizable()),"title"===a&&this._title(this.uiDialogTitlebar.find(".ui-dialog-title")))},_size:function(){var a,b,c,d=this.options;this.element.show().css({width:"auto",minHeight:0,maxHeight:"none",height:0}),d.minWidth>d.width&&(d.width=d.minWidth),a=this.uiDialog.css({height:"auto",width:d.width}).outerHeight(),b=Math.max(0,d.minHeight-a),c="number"==typeof d.maxHeight?Math.max(0,d.maxHeight-a):"none","auto"===d.height?this.element.css({minHeight:b,maxHeight:c,height:"auto"}):this.element.height(Math.max(0,d.height-a)),this.uiDialog.is(":data(ui-resizable)")&&this.uiDialog.resizable("option","minHeight",this._minHeight())},_blockFrames:function(){this.iframeBlocks=this.document.find("iframe").map(function(){var b=a(this);return a("<div>").css({position:"absolute",width:b.outerWidth(),height:b.outerHeight()}).appendTo(b.parent()).offset(b.offset())[0]})},_unblockFrames:function(){this.iframeBlocks&&(this.iframeBlocks.remove(),delete this.iframeBlocks)},_allowInteraction:function(b){return!!a(b.target).closest(".ui-dialog").length||!!a(b.target).closest(".ui-datepicker").length},_createOverlay:function(){if(this.options.modal){var b=this,c=this.widgetFullName;a.ui.dialog.overlayInstances||this._delay(function(){a.ui.dialog.overlayInstances&&this.document.bind("focusin.dialog",function(d){b._allowInteraction(d)||(d.preventDefault(),a(".ui-dialog:visible:last .ui-dialog-content").data(c)._focusTabbable())})}),this.overlay=a("<div>").addClass("ui-widget-overlay ui-front").appendTo(this._appendTo()),this._on(this.overlay,{mousedown:"_keepFocus"}),a.ui.dialog.overlayInstances++}},_destroyOverlay:function(){this.options.modal&&this.overlay&&(a.ui.dialog.overlayInstances--,a.ui.dialog.overlayInstances||this.document.unbind("focusin.dialog"),this.overlay.remove(),this.overlay=null)}}),a.ui.dialog.overlayInstances=0,!1!==a.uiBackCompat&&a.widget("ui.dialog",a.ui.dialog,{_position:function(){var e,b=this.options.position,c=[],d=[0,0];b?(("string"==typeof b||"object"==typeof b&&"0"in b)&&(c=b.split?b.split(" "):[b[0],b[1]],1===c.length&&(c[1]=c[0]),a.each(["left","top"],function(a,b){+c[a]===c[a]&&(d[a]=c[a],c[a]=b)}),b={my:c[0]+(d[0]<0?d[0]:"+"+d[0])+" "+c[1]+(d[1]<0?d[1]:"+"+d[1]),at:c.join(" ")}),b=a.extend({},a.ui.dialog.prototype.options.position,b)):b=a.ui.dialog.prototype.options.position,e=this.uiDialog.is(":visible"),e||this.uiDialog.show(),this.uiDialog.position(b),e||this.uiDialog.hide()}})}(jQuery),function(a,b){var c=/up|down|vertical/,d=/up|left|vertical|horizontal/;a.effects.effect.blind=function(b,e){var p,q,r,f=a(this),g=["position","top","bottom","left","right","height","width"],h=a.effects.setMode(f,b.mode||"hide"),i=b.direction||"up",j=c.test(i),k=j?"height":"width",l=j?"top":"left",m=d.test(i),n={},o="show"===h;f.parent().is(".ui-effects-wrapper")?a.effects.save(f.parent(),g):a.effects.save(f,g),f.show(),p=a.effects.createWrapper(f).css({overflow:"hidden"}),q=p[k](),r=parseFloat(p.css(l))||0,n[k]=o?q:0,m||(f.css(j?"bottom":"right",0).css(j?"top":"left","auto").css({position:"absolute"}),n[l]=o?r:q+r),o&&(p.css(k,0),m||p.css(l,r+q)),p.animate(n,{duration:b.duration,easing:b.easing,queue:!1,complete:function(){"hide"===h&&f.hide(),a.effects.restore(f,g),a.effects.removeWrapper(f),e()}})}}(jQuery),function(a,b){a.effects.effect.bounce=function(b,c){var q,r,s,d=a(this),e=["position","top","bottom","left","right","height","width"],f=a.effects.setMode(d,b.mode||"effect"),g="hide"===f,h="show"===f,i=b.direction||"up",j=b.distance,k=b.times||5,l=2*k+(h||g?1:0),m=b.duration/l,n=b.easing,o="up"===i||"down"===i?"top":"left",p="up"===i||"left"===i,t=d.queue(),u=t.length;for((h||g)&&e.push("opacity"),a.effects.save(d,e),d.show(),a.effects.createWrapper(d),j||(j=d["top"===o?"outerHeight":"outerWidth"]()/3),h&&(s={opacity:1},s[o]=0,d.css("opacity",0).css(o,p?2*-j:2*j).animate(s,m,n)),g&&(j/=Math.pow(2,k-1)),s={},s[o]=0,q=0;q<k;q++)r={},r[o]=(p?"-=":"+=")+j,d.animate(r,m,n).animate(s,m,n),j=g?2*j:j/2;g&&(r={opacity:0},r[o]=(p?"-=":"+=")+j,d.animate(r,m,n)),d.queue(function(){g&&d.hide(),a.effects.restore(d,e),a.effects.removeWrapper(d),c()}),u>1&&t.splice.apply(t,[1,0].concat(t.splice(u,l+1))),d.dequeue()}}(jQuery),function(a,b){a.effects.effect.clip=function(b,c){var m,n,o,d=a(this),e=["position","top","bottom","left","right","height","width"],f=a.effects.setMode(d,b.mode||"hide"),g="show"===f,h=b.direction||"vertical",i="vertical"===h,j=i?"height":"width",k=i?"top":"left",l={};a.effects.save(d,e),d.show(),m=a.effects.createWrapper(d).css({overflow:"hidden"}),n="IMG"===d[0].tagName?m:d,o=n[j](),g&&(n.css(j,0),n.css(k,o/2)),l[j]=g?o:0,l[k]=g?0:o/2,n.animate(l,{queue:!1,duration:b.duration,easing:b.easing,complete:function(){g||d.hide(),a.effects.restore(d,e),a.effects.removeWrapper(d),c()}})}}(jQuery),function(a,b){a.effects.effect.drop=function(b,c){var l,d=a(this),e=["position","top","bottom","left","right","opacity","height","width"],f=a.effects.setMode(d,b.mode||"hide"),g="show"===f,h=b.direction||"left",i="up"===h||"down"===h?"top":"left",j="up"===h||"left"===h?"pos":"neg",k={opacity:g?1:0};a.effects.save(d,e),d.show(),a.effects.createWrapper(d),l=b.distance||d["top"===i?"outerHeight":"outerWidth"](!0)/2,g&&d.css("opacity",0).css(i,"pos"===j?-l:l),k[i]=(g?"pos"===j?"+=":"-=":"pos"===j?"-=":"+=")+l,d.animate(k,{queue:!1,duration:b.duration,easing:b.easing,complete:function(){"hide"===f&&d.hide(),a.effects.restore(d,e),a.effects.removeWrapper(d),c()}})}}(jQuery),function(a,b){a.effects.effect.explode=function(b,c){function s(){l.push(this),l.length===d*e&&t()}function t(){f.css({visibility:"visible"}),a(l).remove(),h||f.hide(),c()}var m,n,o,p,q,r,d=b.pieces?Math.round(Math.sqrt(b.pieces)):3,e=d,f=a(this),g=a.effects.setMode(f,b.mode||"hide"),h="show"===g,i=f.show().css("visibility","hidden").offset(),j=Math.ceil(f.outerWidth()/e),k=Math.ceil(f.outerHeight()/d),l=[];for(m=0;m<d;m++)for(p=i.top+m*k,r=m-(d-1)/2,n=0;n<e;n++)o=i.left+n*j,q=n-(e-1)/2,f.clone().appendTo("body").wrap("<div></div>").css({position:"absolute",visibility:"visible",left:-n*j,top:-m*k}).parent().addClass("ui-effects-explode").css({position:"absolute",overflow:"hidden",width:j,height:k,left:o+(h?q*j:0),top:p+(h?r*k:0),opacity:h?0:1}).animate({left:o+(h?0:q*j),top:p+(h?0:r*k),opacity:h?1:0},b.duration||500,b.easing,s)}}(jQuery),function(a,b){a.effects.effect.fade=function(b,c){var d=a(this),e=a.effects.setMode(d,b.mode||"toggle");d.animate({opacity:e},{queue:!1,duration:b.duration,easing:b.easing,complete:c})}}(jQuery),function(a,b){a.effects.effect.fold=function(b,c){var o,p,d=a(this),e=["position","top","bottom","left","right","height","width"],f=a.effects.setMode(d,b.mode||"hide"),g="show"===f,h="hide"===f,i=b.size||15,j=/([0-9]+)%/.exec(i),k=!!b.horizFirst,l=g!==k,m=l?["width","height"]:["height","width"],n=b.duration/2,q={},r={};a.effects.save(d,e),d.show(),o=a.effects.createWrapper(d).css({overflow:"hidden"}),p=l?[o.width(),o.height()]:[o.height(),o.width()],j&&(i=parseInt(j[1],10)/100*p[h?0:1]),g&&o.css(k?{height:0,width:i}:{height:i,width:0}),q[m[0]]=g?p[0]:i,r[m[1]]=g?p[1]:0,o.animate(q,n,b.easing).animate(r,n,b.easing,function(){h&&d.hide(),a.effects.restore(d,e),a.effects.removeWrapper(d),c()})}}(jQuery),function(a,b){a.effects.effect.highlight=function(b,c){var d=a(this),e=["backgroundImage","backgroundColor","opacity"],f=a.effects.setMode(d,b.mode||"show"),g={backgroundColor:d.css("backgroundColor")};"hide"===f&&(g.opacity=0),a.effects.save(d,e),d.show().css({backgroundImage:"none",backgroundColor:b.color||"#ffff99"}).animate(g,{queue:!1,duration:b.duration,easing:b.easing,complete:function(){"hide"===f&&d.hide(),a.effects.restore(d,e),c()}})}}(jQuery),function(a,b){a.effects.effect.pulsate=function(b,c){var n,d=a(this),e=a.effects.setMode(d,b.mode||"show"),f="show"===e,g="hide"===e,h=f||"hide"===e,i=2*(b.times||5)+(h?1:0),j=b.duration/i,k=0,l=d.queue(),m=l.length;for(!f&&d.is(":visible")||(d.css("opacity",0).show(),k=1),n=1;n<i;n++)d.animate({opacity:k},j,b.easing),k=1-k;d.animate({opacity:k},j,b.easing),d.queue(function(){g&&d.hide(),c()}),m>1&&l.splice.apply(l,[1,0].concat(l.splice(m,i+1))),d.dequeue()}}(jQuery),function(a,b){a.effects.effect.puff=function(b,c){var d=a(this),e=a.effects.setMode(d,b.mode||"hide"),f="hide"===e,g=parseInt(b.percent,10)||150,h=g/100,i={height:d.height(),width:d.width(),outerHeight:d.outerHeight(),outerWidth:d.outerWidth()};a.extend(b,{effect:"scale",queue:!1,fade:!0,mode:e,complete:c,percent:f?g:100,from:f?i:{height:i.height*h,width:i.width*h,outerHeight:i.outerHeight*h,outerWidth:i.outerWidth*h}}),d.effect(b)},a.effects.effect.scale=function(b,c){var d=a(this),e=a.extend(!0,{},b),f=a.effects.setMode(d,b.mode||"effect"),g=parseInt(b.percent,10)||(0===parseInt(b.percent,10)?0:"hide"===f?0:100),h=b.direction||"both",i=b.origin,j={height:d.height(),width:d.width(),outerHeight:d.outerHeight(),outerWidth:d.outerWidth()},k={y:"horizontal"!==h?g/100:1,x:"vertical"!==h?g/100:1};e.effect="size",e.queue=!1,e.complete=c,"effect"!==f&&(e.origin=i||["middle","center"],e.restore=!0),e.from=b.from||("show"===f?{height:0,width:0,outerHeight:0,outerWidth:0}:j),e.to={height:j.height*k.y,width:j.width*k.x,outerHeight:j.outerHeight*k.y,outerWidth:j.outerWidth*k.x},e.fade&&("show"===f&&(e.from.opacity=0,e.to.opacity=1),"hide"===f&&(e.from.opacity=1,e.to.opacity=0)),d.effect(e)},a.effects.effect.size=function(b,c){var d,e,f,g=a(this),h=["position","top","bottom","left","right","width","height","overflow","opacity"],i=["position","top","bottom","left","right","overflow","opacity"],j=["width","height","overflow"],k=["fontSize"],l=["borderTopWidth","borderBottomWidth","paddingTop","paddingBottom"],m=["borderLeftWidth","borderRightWidth","paddingLeft","paddingRight"],n=a.effects.setMode(g,b.mode||"effect"),o=b.restore||"effect"!==n,p=b.scale||"both",q=b.origin||["middle","center"],r=g.css("position"),s=o?h:i,t={height:0,width:0,outerHeight:0,outerWidth:0};"show"===n&&g.show(),d={height:g.height(),width:g.width(),outerHeight:g.outerHeight(),outerWidth:g.outerWidth()},"toggle"===b.mode&&"show"===n?(g.from=b.to||t,g.to=b.from||d):(g.from=b.from||("show"===n?t:d),g.to=b.to||("hide"===n?t:d)),f={from:{y:g.from.height/d.height,x:g.from.width/d.width},to:{y:g.to.height/d.height,x:g.to.width/d.width}},"box"!==p&&"both"!==p||(f.from.y!==f.to.y&&(s=s.concat(l),g.from=a.effects.setTransition(g,l,f.from.y,g.from),g.to=a.effects.setTransition(g,l,f.to.y,g.to)),f.from.x!==f.to.x&&(s=s.concat(m),g.from=a.effects.setTransition(g,m,f.from.x,g.from),g.to=a.effects.setTransition(g,m,f.to.x,g.to))),"content"!==p&&"both"!==p||f.from.y!==f.to.y&&(s=s.concat(k).concat(j),g.from=a.effects.setTransition(g,k,f.from.y,g.from),g.to=a.effects.setTransition(g,k,f.to.y,g.to)),a.effects.save(g,s),g.show(),a.effects.createWrapper(g),g.css("overflow","hidden").css(g.from),q&&(e=a.effects.getBaseline(q,d),g.from.top=(d.outerHeight-g.outerHeight())*e.y,g.from.left=(d.outerWidth-g.outerWidth())*e.x,g.to.top=(d.outerHeight-g.to.outerHeight)*e.y,g.to.left=(d.outerWidth-g.to.outerWidth)*e.x),g.css(g.from),"content"!==p&&"both"!==p||(l=l.concat(["marginTop","marginBottom"]).concat(k),m=m.concat(["marginLeft","marginRight"]),j=h.concat(l).concat(m),g.find("*[width]").each(function(){var c=a(this),d={height:c.height(),width:c.width(),outerHeight:c.outerHeight(),outerWidth:c.outerWidth()};o&&a.effects.save(c,j),c.from={height:d.height*f.from.y,width:d.width*f.from.x,outerHeight:d.outerHeight*f.from.y,outerWidth:d.outerWidth*f.from.x},c.to={height:d.height*f.to.y,width:d.width*f.to.x,outerHeight:d.height*f.to.y,outerWidth:d.width*f.to.x},f.from.y!==f.to.y&&(c.from=a.effects.setTransition(c,l,f.from.y,c.from),c.to=a.effects.setTransition(c,l,f.to.y,c.to)),f.from.x!==f.to.x&&(c.from=a.effects.setTransition(c,m,f.from.x,c.from),c.to=a.effects.setTransition(c,m,f.to.x,c.to)),c.css(c.from),c.animate(c.to,b.duration,b.easing,function(){o&&a.effects.restore(c,j)})})),g.animate(g.to,{queue:!1,duration:b.duration,easing:b.easing,complete:function(){0===g.to.opacity&&g.css("opacity",g.from.opacity),"hide"===n&&g.hide(),a.effects.restore(g,s),o||("static"===r?g.css({position:"relative",top:g.to.top,left:g.to.left}):a.each(["top","left"],function(a,b){g.css(b,function(b,c){var d=parseInt(c,10),e=a?g.to.left:g.to.top;return"auto"===c?e+"px":d+e+"px"})})),a.effects.removeWrapper(g),c()}})}}(jQuery),function(a,b){a.effects.effect.shake=function(b,c){var q,d=a(this),e=["position","top","bottom","left","right","height","width"],f=a.effects.setMode(d,b.mode||"effect"),g=b.direction||"left",h=b.distance||20,i=b.times||3,j=2*i+1,k=Math.round(b.duration/j),l="up"===g||"down"===g?"top":"left",m="up"===g||"left"===g,n={},o={},p={},r=d.queue(),s=r.length;for(a.effects.save(d,e),d.show(),a.effects.createWrapper(d),n[l]=(m?"-=":"+=")+h,o[l]=(m?"+=":"-=")+2*h,p[l]=(m?"-=":"+=")+2*h,d.animate(n,k,b.easing),q=1;q<i;q++)d.animate(o,k,b.easing).animate(p,k,b.easing);d.animate(o,k,b.easing).animate(n,k/2,b.easing).queue(function(){"hide"===f&&d.hide(),a.effects.restore(d,e),a.effects.removeWrapper(d),c()}),s>1&&r.splice.apply(r,[1,0].concat(r.splice(s,j+1))),d.dequeue()}}(jQuery),function(a,b){a.effects.effect.slide=function(b,c){var k,d=a(this),e=["position","top","bottom","left","right","width","height"],f=a.effects.setMode(d,b.mode||"show"),g="show"===f,h=b.direction||"left",i="up"===h||"down"===h?"top":"left",j="up"===h||"left"===h,l={};a.effects.save(d,e),d.show(),k=b.distance||d["top"===i?"outerHeight":"outerWidth"](!0),a.effects.createWrapper(d).css({overflow:"hidden"}),g&&d.css(i,j?isNaN(k)?"-"+k:-k:k),l[i]=(g?j?"+=":"-=":j?"-=":"+=")+k,d.animate(l,{queue:!1,duration:b.duration,easing:b.easing,complete:function(){"hide"===f&&d.hide(),a.effects.restore(d,e),a.effects.removeWrapper(d),c()}})}}(jQuery),function(a,b){a.effects.effect.transfer=function(b,c){var d=a(this),e=a(b.to),f="fixed"===e.css("position"),g=a("body"),h=f?g.scrollTop():0,i=f?g.scrollLeft():0,j=e.offset(),k={top:j.top-h,left:j.left-i,height:e.innerHeight(),width:e.innerWidth()},l=d.offset(),m=a("<div class='ui-effects-transfer'></div>").appendTo(document.body).addClass(b.className).css({top:l.top-h,left:l.left-i,height:d.innerHeight(),width:d.innerWidth(),position:f?"fixed":"absolute"}).animate(k,b.duration,b.easing,function(){m.remove(),c()})}}(jQuery),function(a,b){a.widget("ui.menu",{version:"1.10.2",defaultElement:"<ul>",delay:300,options:{icons:{submenu:"ui-icon-carat-1-e"},menus:"ul",position:{my:"left top",at:"right top"},role:"menu",blur:null,focus:null,select:null},_create:function(){this.activeMenu=this.element,this.mouseHandled=!1,this.element.uniqueId().addClass("ui-menu ui-widget ui-widget-content ui-corner-all").toggleClass("ui-menu-icons",!!this.element.find(".ui-icon").length).attr({role:this.options.role,tabIndex:0}).bind("click"+this.eventNamespace,a.proxy(function(a){this.options.disabled&&a.preventDefault()},this)),this.options.disabled&&this.element.addClass("ui-state-disabled").attr("aria-disabled","true"),this._on({"mousedown .ui-menu-item > a":function(a){a.preventDefault()},"click .ui-state-disabled > a":function(a){a.preventDefault()},"click .ui-menu-item:has(a)":function(b){var c=a(b.target).closest(".ui-menu-item");!this.mouseHandled&&c.not(".ui-state-disabled").length&&(this.mouseHandled=!0,this.select(b),c.has(".ui-menu").length?this.expand(b):this.element.is(":focus")||(this.element.trigger("focus",[!0]),this.active&&1===this.active.parents(".ui-menu").length&&clearTimeout(this.timer)))},"mouseenter .ui-menu-item":function(b){var c=a(b.currentTarget);c.siblings().children(".ui-state-active").removeClass("ui-state-active"),this.focus(b,c)},mouseleave:"collapseAll","mouseleave .ui-menu":"collapseAll",focus:function(a,b){var c=this.active||this.element.children(".ui-menu-item").eq(0);b||this.focus(a,c)},blur:function(b){this._delay(function(){a.contains(this.element[0],this.document[0].activeElement)||this.collapseAll(b)})},keydown:"_keydown"}),this.refresh(),this._on(this.document,{click:function(b){a(b.target).closest(".ui-menu").length||this.collapseAll(b),this.mouseHandled=!1}})},_destroy:function(){this.element.removeAttr("aria-activedescendant").find(".ui-menu").addBack().removeClass("ui-menu ui-widget ui-widget-content ui-corner-all ui-menu-icons").removeAttr("role").removeAttr("tabIndex").removeAttr("aria-labelledby").removeAttr("aria-expanded").removeAttr("aria-hidden").removeAttr("aria-disabled").removeUniqueId().show(),this.element.find(".ui-menu-item").removeClass("ui-menu-item").removeAttr("role").removeAttr("aria-disabled").children("a").removeUniqueId().removeClass("ui-corner-all ui-state-hover").removeAttr("tabIndex").removeAttr("role").removeAttr("aria-haspopup").children().each(function(){var b=a(this);b.data("ui-menu-submenu-carat")&&b.remove()}),this.element.find(".ui-menu-divider").removeClass("ui-menu-divider ui-widget-content")},_keydown:function(b){function i(a){return a.replace(/[\-\[\]{}()*+?.,\\\^$|#\s]/g,"\\$&")}var c,d,e,f,g,h=!0;switch(b.keyCode){case a.ui.keyCode.PAGE_UP:this.previousPage(b);break;case a.ui.keyCode.PAGE_DOWN:this.nextPage(b);break;case a.ui.keyCode.HOME:this._move("first","first",b);break;case a.ui.keyCode.END:this._move("last","last",b);break;case a.ui.keyCode.UP:this.previous(b);break;case a.ui.keyCode.DOWN:this.next(b);break;case a.ui.keyCode.LEFT:this.collapse(b);break;case a.ui.keyCode.RIGHT:this.active&&!this.active.is(".ui-state-disabled")&&this.expand(b);break;case a.ui.keyCode.ENTER:case a.ui.keyCode.SPACE:this._activate(b);break;case a.ui.keyCode.ESCAPE:this.collapse(b);break;default:h=!1,d=this.previousFilter||"",e=String.fromCharCode(b.keyCode),f=!1,clearTimeout(this.filterTimer),e===d?f=!0:e=d+e,g=new RegExp("^"+i(e),"i"),c=this.activeMenu.children(".ui-menu-item").filter(function(){return g.test(a(this).children("a").text())}),c=f&&-1!==c.index(this.active.next())?this.active.nextAll(".ui-menu-item"):c,c.length||(e=String.fromCharCode(b.keyCode),g=new RegExp("^"+i(e),"i"),c=this.activeMenu.children(".ui-menu-item").filter(function(){return g.test(a(this).children("a").text())})),c.length?(this.focus(b,c),c.length>1?(this.previousFilter=e,this.filterTimer=this._delay(function(){delete this.previousFilter},1e3)):delete this.previousFilter):delete this.previousFilter}h&&b.preventDefault()},_activate:function(a){this.active.is(".ui-state-disabled")||(this.active.children("a[aria-haspopup='true']").length?this.expand(a):this.select(a))},refresh:function(){var b,c=this.options.icons.submenu,d=this.element.find(this.options.menus);d.filter(":not(.ui-menu)").addClass("ui-menu ui-widget ui-widget-content ui-corner-all").hide().attr({role:this.options.role,"aria-hidden":"true","aria-expanded":"false"}).each(function(){var b=a(this),d=b.prev("a"),e=a("<span>").addClass("ui-menu-icon ui-icon "+c).data("ui-menu-submenu-carat",!0);d.attr("aria-haspopup","true").prepend(e),b.attr("aria-labelledby",d.attr("id"))}),b=d.add(this.element),b.children(":not(.ui-menu-item):has(a)").addClass("ui-menu-item").attr("role","presentation").children("a").uniqueId().addClass("ui-corner-all").attr({tabIndex:-1,role:this._itemRole()}),b.children(":not(.ui-menu-item)").each(function(){var b=a(this);/[^\-\u2014\u2013\s]/.test(b.text())||b.addClass("ui-widget-content ui-menu-divider")}),b.children(".ui-state-disabled").attr("aria-disabled","true"),this.active&&!a.contains(this.element[0],this.active[0])&&this.blur()},_itemRole:function(){return{menu:"menuitem",listbox:"option"}[this.options.role]},_setOption:function(a,b){"icons"===a&&this.element.find(".ui-menu-icon").removeClass(this.options.icons.submenu).addClass(b.submenu),this._super(a,b)},focus:function(a,b){var c,d;this.blur(a,a&&"focus"===a.type),this._scrollIntoView(b),this.active=b.first(),d=this.active.children("a").addClass("ui-state-focus"),this.options.role&&this.element.attr("aria-activedescendant",d.attr("id")),this.active.parent().closest(".ui-menu-item").children("a:first").addClass("ui-state-active"),a&&"keydown"===a.type?this._close():this.timer=this._delay(function(){this._close()},this.delay),c=b.children(".ui-menu"),c.length&&/^mouse/.test(a.type)&&this._startOpening(c),this.activeMenu=b.parent(),this._trigger("focus",a,{item:b})},_scrollIntoView:function(b){var c,d,e,f,g,h;this._hasScroll()&&(c=parseFloat(a.css(this.activeMenu[0],"borderTopWidth"))||0,d=parseFloat(a.css(this.activeMenu[0],"paddingTop"))||0,e=b.offset().top-this.activeMenu.offset().top-c-d,f=this.activeMenu.scrollTop(),g=this.activeMenu.height(),h=b.height(),e<0?this.activeMenu.scrollTop(f+e):e+h>g&&this.activeMenu.scrollTop(f+e-g+h))},blur:function(a,b){b||clearTimeout(this.timer),this.active&&(this.active.children("a").removeClass("ui-state-focus"),this.active=null,this._trigger("blur",a,{item:this.active}))},_startOpening:function(a){clearTimeout(this.timer),"true"===a.attr("aria-hidden")&&(this.timer=this._delay(function(){this._close(),this._open(a)},this.delay))},_open:function(b){var c=a.extend({of:this.active},this.options.position);clearTimeout(this.timer),this.element.find(".ui-menu").not(b.parents(".ui-menu")).hide().attr("aria-hidden","true"),b.show().removeAttr("aria-hidden").attr("aria-expanded","true").position(c)},collapseAll:function(b,c){clearTimeout(this.timer),this.timer=this._delay(function(){var d=c?this.element:a(b&&b.target).closest(this.element.find(".ui-menu"));d.length||(d=this.element),this._close(d),this.blur(b),this.activeMenu=d},this.delay)},_close:function(a){a||(a=this.active?this.active.parent():this.element),a.find(".ui-menu").hide().attr("aria-hidden","true").attr("aria-expanded","false").end().find("a.ui-state-active").removeClass("ui-state-active")},collapse:function(a){var b=this.active&&this.active.parent().closest(".ui-menu-item",this.element);b&&b.length&&(this._close(),this.focus(a,b))},expand:function(a){var b=this.active&&this.active.children(".ui-menu ").children(".ui-menu-item").first();b&&b.length&&(this._open(b.parent()),this._delay(function(){this.focus(a,b)}))},next:function(a){this._move("next","first",a)},previous:function(a){this._move("prev","last",a)},isFirstItem:function(){return this.active&&!this.active.prevAll(".ui-menu-item").length},isLastItem:function(){return this.active&&!this.active.nextAll(".ui-menu-item").length},_move:function(a,b,c){var d;this.active&&(d="first"===a||"last"===a?this.active["first"===a?"prevAll":"nextAll"](".ui-menu-item").eq(-1):this.active[a+"All"](".ui-menu-item").eq(0)),d&&d.length&&this.active||(d=this.activeMenu.children(".ui-menu-item")[b]()),this.focus(c,d)},nextPage:function(b){var c,d,e;if(!this.active)return void this.next(b);this.isLastItem()||(this._hasScroll()?(d=this.active.offset().top,e=this.element.height(),this.active.nextAll(".ui-menu-item").each(function(){return c=a(this),c.offset().top-d-e<0}),this.focus(b,c)):this.focus(b,this.activeMenu.children(".ui-menu-item")[this.active?"last":"first"]()))},previousPage:function(b){var c,d,e;if(!this.active)return void this.next(b);this.isFirstItem()||(this._hasScroll()?(d=this.active.offset().top,e=this.element.height(),this.active.prevAll(".ui-menu-item").each(function(){return c=a(this),c.offset().top-d+e>0}),this.focus(b,c)):this.focus(b,this.activeMenu.children(".ui-menu-item").first()))},_hasScroll:function(){return this.element.outerHeight()<this.element.prop("scrollHeight")},select:function(b){this.active=this.active||a(b.target).closest(".ui-menu-item");var c={item:this.active};this.active.has(".ui-menu").length||this.collapseAll(b,!0),this._trigger("select",b,c)}})}(jQuery),function(a,b){function m(a,b,c){return[parseFloat(a[0])*(k.test(a[0])?b/100:1),parseFloat(a[1])*(k.test(a[1])?c/100:1)]}function n(b,c){return parseInt(a.css(b,c),10)||0}function o(b){var c=b[0];return 9===c.nodeType?{width:b.width(),height:b.height(),offset:{top:0,left:0}}:a.isWindow(c)?{width:b.width(),height:b.height(),offset:{top:b.scrollTop(),left:b.scrollLeft()}}:c.preventDefault?{width:0,height:0,offset:{top:c.pageY,left:c.pageX}}:{width:b.outerWidth(),height:b.outerHeight(),offset:b.offset()}}a.ui=a.ui||{};var c,d=Math.max,e=Math.abs,f=Math.round,g=/left|center|right/,h=/top|center|bottom/,i=/[\+\-]\d+(\.[\d]+)?%?/,j=/^\w+/,k=/%$/,l=a.fn.position;a.position={scrollbarWidth:function(){if(c!==b)return c;var d,e,f=a("<div style='display:block;width:50px;height:50px;overflow:hidden;'><div style='height:100px;width:auto;'></div></div>"),g=f.children()[0];return a("body").append(f),d=g.offsetWidth,f.css("overflow","scroll"),e=g.offsetWidth,d===e&&(e=f[0].clientWidth),f.remove(),c=d-e},getScrollInfo:function(b){var c=b.isWindow?"":b.element.css("overflow-x"),d=b.isWindow?"":b.element.css("overflow-y"),e="scroll"===c||"auto"===c&&b.width<b.element[0].scrollWidth;return{width:"scroll"===d||"auto"===d&&b.height<b.element[0].scrollHeight?a.position.scrollbarWidth():0,height:e?a.position.scrollbarWidth():0}},getWithinInfo:function(b){var c=a(b||window),d=a.isWindow(c[0]);return{element:c,isWindow:d,offset:c.offset()||{left:0,top:0},scrollLeft:c.scrollLeft(),scrollTop:c.scrollTop(),width:d?c.width():c.outerWidth(),height:d?c.height():c.outerHeight()}}},a.fn.position=function(b){if(!b||!b.of)return l.apply(this,arguments);b=a.extend({},b);var c,k,p,q,r,s,t=a(b.of),u=a.position.getWithinInfo(b.within),v=a.position.getScrollInfo(u),w=(b.collision||"flip").split(" "),x={};return s=o(t),t[0].preventDefault&&(b.at="left top"),k=s.width,p=s.height,q=s.offset,r=a.extend({},q),a.each(["my","at"],function(){var c,d,a=(b[this]||"").split(" ");1===a.length&&(a=g.test(a[0])?a.concat(["center"]):h.test(a[0])?["center"].concat(a):["center","center"]),a[0]=g.test(a[0])?a[0]:"center",a[1]=h.test(a[1])?a[1]:"center",c=i.exec(a[0]),d=i.exec(a[1]),x[this]=[c?c[0]:0,d?d[0]:0],b[this]=[j.exec(a[0])[0],j.exec(a[1])[0]]}),1===w.length&&(w[1]=w[0]),"right"===b.at[0]?r.left+=k:"center"===b.at[0]&&(r.left+=k/2),"bottom"===b.at[1]?r.top+=p:"center"===b.at[1]&&(r.top+=p/2),c=m(x.at,k,p),r.left+=c[0],r.top+=c[1],this.each(function(){var g,h,i=a(this),j=i.outerWidth(),l=i.outerHeight(),o=n(this,"marginLeft"),s=n(this,"marginTop"),y=j+o+n(this,"marginRight")+v.width,z=l+s+n(this,"marginBottom")+v.height,A=a.extend({},r),B=m(x.my,i.outerWidth(),i.outerHeight());"right"===b.my[0]?A.left-=j:"center"===b.my[0]&&(A.left-=j/2),"bottom"===b.my[1]?A.top-=l:"center"===b.my[1]&&(A.top-=l/2),A.left+=B[0],A.top+=B[1],a.support.offsetFractions||(A.left=f(A.left),A.top=f(A.top)),g={marginLeft:o,marginTop:s},a.each(["left","top"],function(d,e){a.ui.position[w[d]]&&a.ui.position[w[d]][e](A,{targetWidth:k,targetHeight:p,elemWidth:j,elemHeight:l,collisionPosition:g,collisionWidth:y,collisionHeight:z,offset:[c[0]+B[0],c[1]+B[1]],my:b.my,at:b.at,within:u,elem:i})}),b.using&&(h=function(a){var c=q.left-A.left,f=c+k-j,g=q.top-A.top,h=g+p-l,m={target:{element:t,left:q.left,top:q.top,width:k,height:p},element:{element:i,left:A.left,top:A.top,width:j,height:l},horizontal:f<0?"left":c>0?"right":"center",vertical:h<0?"top":g>0?"bottom":"middle"};k<j&&e(c+f)<k&&(m.horizontal="center"),p<l&&e(g+h)<p&&(m.vertical="middle"),d(e(c),e(f))>d(e(g),e(h))?m.important="horizontal":m.important="vertical",b.using.call(this,a,m)}),i.offset(a.extend(A,{using:h}))})},a.ui.position={fit:{left:function(a,b){var j,c=b.within,e=c.isWindow?c.scrollLeft:c.offset.left,f=c.width,g=a.left-b.collisionPosition.marginLeft,h=e-g,i=g+b.collisionWidth-f-e;b.collisionWidth>f?h>0&&i<=0?(j=a.left+h+b.collisionWidth-f-e,a.left+=h-j):a.left=i>0&&h<=0?e:h>i?e+f-b.collisionWidth:e:h>0?a.left+=h:i>0?a.left-=i:a.left=d(a.left-g,a.left)},top:function(a,b){var j,c=b.within,e=c.isWindow?c.scrollTop:c.offset.top,f=b.within.height,g=a.top-b.collisionPosition.marginTop,h=e-g,i=g+b.collisionHeight-f-e;b.collisionHeight>f?h>0&&i<=0?(j=a.top+h+b.collisionHeight-f-e,a.top+=h-j):a.top=i>0&&h<=0?e:h>i?e+f-b.collisionHeight:e:h>0?a.top+=h:i>0?a.top-=i:a.top=d(a.top-g,a.top)}},flip:{left:function(a,b){var n,o,c=b.within,d=c.offset.left+c.scrollLeft,f=c.width,g=c.isWindow?c.scrollLeft:c.offset.left,h=a.left-b.collisionPosition.marginLeft,i=h-g,j=h+b.collisionWidth-f-g,k="left"===b.my[0]?-b.elemWidth:"right"===b.my[0]?b.elemWidth:0,l="left"===b.at[0]?b.targetWidth:"right"===b.at[0]?-b.targetWidth:0,m=-2*b.offset[0];i<0?((n=a.left+k+l+m+b.collisionWidth-f-d)<0||n<e(i))&&(a.left+=k+l+m):j>0&&((o=a.left-b.collisionPosition.marginLeft+k+l+m-g)>0||e(o)<j)&&(a.left+=k+l+m)},top:function(a,b){var o,p,c=b.within,d=c.offset.top+c.scrollTop,f=c.height,g=c.isWindow?c.scrollTop:c.offset.top,h=a.top-b.collisionPosition.marginTop,i=h-g,j=h+b.collisionHeight-f-g,k="top"===b.my[1],l=k?-b.elemHeight:"bottom"===b.my[1]?b.elemHeight:0,m="top"===b.at[1]?b.targetHeight:"bottom"===b.at[1]?-b.targetHeight:0,n=-2*b.offset[1];i<0?(p=a.top+l+m+n+b.collisionHeight-f-d,a.top+l+m+n>i&&(p<0||p<e(i))&&(a.top+=l+m+n)):j>0&&(o=a.top-b.collisionPosition.marginTop+l+m+n-g,a.top+l+m+n>j&&(o>0||e(o)<j)&&(a.top+=l+m+n))}},flipfit:{left:function(){a.ui.position.flip.left.apply(this,arguments),a.ui.position.fit.left.apply(this,arguments)},top:function(){a.ui.position.flip.top.apply(this,arguments),a.ui.position.fit.top.apply(this,arguments)}}},function(){var b,c,d,e,f,g=document.getElementsByTagName("body")[0],h=document.createElement("div");b=document.createElement(g?"div":"body"),d={visibility:"hidden",width:0,height:0,border:0,margin:0,background:"none"},g&&a.extend(d,{position:"absolute",left:"-1000px",top:"-1000px"});for(f in d)b.style[f]=d[f];b.appendChild(h),c=g||document.documentElement,c.insertBefore(b,c.firstChild),h.style.cssText="position: absolute; left: 10.7432222px;",e=a(h).offset().left,a.support.offsetFractions=e>10&&e<11,b.innerHTML="",c.removeChild(b)}()}(jQuery),function(a,b){a.widget("ui.progressbar",{version:"1.10.2",options:{max:100,value:0,change:null,complete:null},min:0,_create:function(){this.oldValue=this.options.value=this._constrainedValue(),this.element.addClass("ui-progressbar ui-widget ui-widget-content ui-corner-all").attr({role:"progressbar","aria-valuemin":this.min}),this.valueDiv=a("<div class='ui-progressbar-value ui-widget-header ui-corner-left'></div>").appendTo(this.element),this._refreshValue()},_destroy:function(){this.element.removeClass("ui-progressbar ui-widget ui-widget-content ui-corner-all").removeAttr("role").removeAttr("aria-valuemin").removeAttr("aria-valuemax").removeAttr("aria-valuenow"),this.valueDiv.remove()},value:function(a){if(a===b)return this.options.value;this.options.value=this._constrainedValue(a),this._refreshValue()},_constrainedValue:function(a){return a===b&&(a=this.options.value),this.indeterminate=!1===a,"number"!=typeof a&&(a=0),!this.indeterminate&&Math.min(this.options.max,Math.max(this.min,a))},_setOptions:function(a){var b=a.value;delete a.value,this._super(a),this.options.value=this._constrainedValue(b),this._refreshValue()},_setOption:function(a,b){"max"===a&&(b=Math.max(this.min,b)),this._super(a,b)},_percentage:function(){return this.indeterminate?100:100*(this.options.value-this.min)/(this.options.max-this.min)},_refreshValue:function(){var b=this.options.value,c=this._percentage();this.valueDiv.toggle(this.indeterminate||b>this.min).toggleClass("ui-corner-right",b===this.options.max).width(c.toFixed(0)+"%"),this.element.toggleClass("ui-progressbar-indeterminate",this.indeterminate),this.indeterminate?(this.element.removeAttr("aria-valuenow"),this.overlayDiv||(this.overlayDiv=a("<div class='ui-progressbar-overlay'></div>").appendTo(this.valueDiv))):(this.element.attr({"aria-valuemax":this.options.max,"aria-valuenow":b}),this.overlayDiv&&(this.overlayDiv.remove(),this.overlayDiv=null)),this.oldValue!==b&&(this.oldValue=b,this._trigger("change")),b===this.options.max&&this._trigger("complete")}})}(jQuery),function(a,b){var c=5;a.widget("ui.slider",a.ui.mouse,{version:"1.10.2",widgetEventPrefix:"slide",options:{animate:!1,distance:0,max:100,min:0,orientation:"horizontal",range:!1,step:1,value:0,values:null,change:null,slide:null,start:null,stop:null},_create:function(){this._keySliding=!1,this._mouseSliding=!1,this._animateOff=!0,this._handleIndex=null,this._detectOrientation(),this._mouseInit(),this.element.addClass("ui-slider ui-slider-"+this.orientation+" ui-widget ui-widget-content ui-corner-all"),this._refresh(),this._setOption("disabled",this.options.disabled),this._animateOff=!1},_refresh:function(){this._createRange(),this._createHandles(),this._setupEvents(),this._refreshValue()},_createHandles:function(){var b,c,d=this.options,e=this.element.find(".ui-slider-handle").addClass("ui-state-default ui-corner-all"),g=[];for(c=d.values&&d.values.length||1,e.length>c&&(e.slice(c).remove(),e=e.slice(0,c)),b=e.length;b<c;b++)g.push("<a class='ui-slider-handle ui-state-default ui-corner-all' href='#'></a>");this.handles=e.add(a(g.join("")).appendTo(this.element)),this.handle=this.handles.eq(0),this.handles.each(function(b){a(this).data("ui-slider-handle-index",b)})},_createRange:function(){var b=this.options,c="";b.range?(!0===b.range&&(b.values?b.values.length&&2!==b.values.length?b.values=[b.values[0],b.values[0]]:a.isArray(b.values)&&(b.values=b.values.slice(0)):b.values=[this._valueMin(),this._valueMin()]),this.range&&this.range.length?this.range.removeClass("ui-slider-range-min ui-slider-range-max").css({left:"",bottom:""}):(this.range=a("<div></div>").appendTo(this.element),c="ui-slider-range ui-widget-header ui-corner-all"),this.range.addClass(c+("min"===b.range||"max"===b.range?" ui-slider-range-"+b.range:""))):this.range=a([])},_setupEvents:function(){var a=this.handles.add(this.range).filter("a");this._off(a),this._on(a,this._handleEvents),this._hoverable(a),this._focusable(a)},_destroy:function(){this.handles.remove(),this.range.remove(),this.element.removeClass("ui-slider ui-slider-horizontal ui-slider-vertical ui-widget ui-widget-content ui-corner-all"),this._mouseDestroy()},_mouseCapture:function(b){var c,d,e,f,g,i,j,k=this,l=this.options;return!l.disabled&&(this.elementSize={width:this.element.outerWidth(),height:this.element.outerHeight()},this.elementOffset=this.element.offset(),c={x:b.pageX,y:b.pageY},d=this._normValueFromMouse(c),e=this._valueMax()-this._valueMin()+1,this.handles.each(function(b){var c=Math.abs(d-k.values(b));(e>c||e===c&&(b===k._lastChangedValue||k.values(b)===l.min))&&(e=c,f=a(this),g=b)}),!1!==this._start(b,g)&&(this._mouseSliding=!0,this._handleIndex=g,f.addClass("ui-state-active").focus(),i=f.offset(),j=!a(b.target).parents().addBack().is(".ui-slider-handle"),this._clickOffset=j?{left:0,top:0}:{left:b.pageX-i.left-f.width()/2,top:b.pageY-i.top-f.height()/2-(parseInt(f.css("borderTopWidth"),10)||0)-(parseInt(f.css("borderBottomWidth"),10)||0)+(parseInt(f.css("marginTop"),10)||0)},this.handles.hasClass("ui-state-hover")||this._slide(b,g,d),this._animateOff=!0,!0))},_mouseStart:function(){return!0},_mouseDrag:function(a){var b={x:a.pageX,y:a.pageY},c=this._normValueFromMouse(b);return this._slide(a,this._handleIndex,c),!1},_mouseStop:function(a){return this.handles.removeClass("ui-state-active"),this._mouseSliding=!1,this._stop(a,this._handleIndex),this._change(a,this._handleIndex),this._handleIndex=null,this._clickOffset=null,this._animateOff=!1,!1},_detectOrientation:function(){this.orientation="vertical"===this.options.orientation?"vertical":"horizontal"},_normValueFromMouse:function(a){var b,c,d,e,f;return"horizontal"===this.orientation?(b=this.elementSize.width,c=a.x-this.elementOffset.left-(this._clickOffset?this._clickOffset.left:0)):(b=this.elementSize.height,c=a.y-this.elementOffset.top-(this._clickOffset?this._clickOffset.top:0)),d=c/b,d>1&&(d=1),d<0&&(d=0),"vertical"===this.orientation&&(d=1-d),e=this._valueMax()-this._valueMin(),f=this._valueMin()+d*e,this._trimAlignValue(f)},_start:function(a,b){var c={handle:this.handles[b],value:this.value()};return this.options.values&&this.options.values.length&&(c.value=this.values(b),c.values=this.values()),this._trigger("start",a,c)},_slide:function(a,b,c){var d,e,f;this.options.values&&this.options.values.length?(d=this.values(b?0:1),2===this.options.values.length&&!0===this.options.range&&(0===b&&c>d||1===b&&c<d)&&(c=d),c!==this.values(b)&&(e=this.values(),e[b]=c,f=this._trigger("slide",a,{handle:this.handles[b],value:c,values:e}),d=this.values(b?0:1),!1!==f&&this.values(b,c,!0))):c!==this.value()&&!1!==(f=this._trigger("slide",a,{handle:this.handles[b],value:c}))&&this.value(c)},_stop:function(a,b){var c={handle:this.handles[b],value:this.value()};this.options.values&&this.options.values.length&&(c.value=this.values(b),c.values=this.values()),this._trigger("stop",a,c)},_change:function(a,b){if(!this._keySliding&&!this._mouseSliding){var c={handle:this.handles[b],value:this.value()};this.options.values&&this.options.values.length&&(c.value=this.values(b),c.values=this.values()),this._lastChangedValue=b,this._trigger("change",a,c)}},value:function(a){return arguments.length?(this.options.value=this._trimAlignValue(a),this._refreshValue(),void this._change(null,0)):this._value()},values:function(b,c){var d,e,f;if(arguments.length>1)return this.options.values[b]=this._trimAlignValue(c),this._refreshValue(),void this._change(null,b);if(!arguments.length)return this._values();if(!a.isArray(arguments[0]))return this.options.values&&this.options.values.length?this._values(b):this.value();for(d=this.options.values,e=arguments[0],f=0;f<d.length;f+=1)d[f]=this._trimAlignValue(e[f]),this._change(null,f);this._refreshValue()},_setOption:function(b,c){var d,e=0;switch("range"===b&&!0===this.options.range&&("min"===c?(this.options.value=this._values(0),this.options.values=null):"max"===c&&(this.options.value=this._values(this.options.values.length-1),this.options.values=null)),a.isArray(this.options.values)&&(e=this.options.values.length),a.Widget.prototype._setOption.apply(this,arguments),b){case"orientation":this._detectOrientation(),this.element.removeClass("ui-slider-horizontal ui-slider-vertical").addClass("ui-slider-"+this.orientation),this._refreshValue();break;case"value":this._animateOff=!0,this._refreshValue(),this._change(null,0),this._animateOff=!1;break;case"values":for(this._animateOff=!0,this._refreshValue(),d=0;d<e;d+=1)this._change(null,d);this._animateOff=!1;break;case"min":case"max":this._animateOff=!0,this._refreshValue(),this._animateOff=!1;break;case"range":this._animateOff=!0,this._refresh(),this._animateOff=!1}},_value:function(){var a=this.options.value;return a=this._trimAlignValue(a)},_values:function(a){var b,c,d;if(arguments.length)return b=this.options.values[a],b=this._trimAlignValue(b);if(this.options.values&&this.options.values.length){for(c=this.options.values.slice(),d=0;d<c.length;d+=1)c[d]=this._trimAlignValue(c[d]);return c}return[]},_trimAlignValue:function(a){if(a<=this._valueMin())return this._valueMin();if(a>=this._valueMax())return this._valueMax();var b=this.options.step>0?this.options.step:1,c=(a-this._valueMin())%b,d=a-c;return 2*Math.abs(c)>=b&&(d+=c>0?b:-b),parseFloat(d.toFixed(5))},_valueMin:function(){return this.options.min},_valueMax:function(){return this.options.max},_refreshValue:function(){var b,c,d,e,f,g=this.options.range,h=this.options,i=this,j=!this._animateOff&&h.animate,k={};this.options.values&&this.options.values.length?this.handles.each(function(d){c=(i.values(d)-i._valueMin())/(i._valueMax()-i._valueMin())*100,k["horizontal"===i.orientation?"left":"bottom"]=c+"%",a(this).stop(1,1)[j?"animate":"css"](k,h.animate),!0===i.options.range&&("horizontal"===i.orientation?(0===d&&i.range.stop(1,1)[j?"animate":"css"]({left:c+"%"},h.animate),1===d&&i.range[j?"animate":"css"]({width:c-b+"%"},{queue:!1,duration:h.animate})):(0===d&&i.range.stop(1,1)[j?"animate":"css"]({bottom:c+"%"},h.animate),1===d&&i.range[j?"animate":"css"]({height:c-b+"%"},{queue:!1,duration:h.animate}))),b=c}):(d=this.value(),e=this._valueMin(),f=this._valueMax(),c=f!==e?(d-e)/(f-e)*100:0,k["horizontal"===this.orientation?"left":"bottom"]=c+"%",this.handle.stop(1,1)[j?"animate":"css"](k,h.animate),"min"===g&&"horizontal"===this.orientation&&this.range.stop(1,1)[j?"animate":"css"]({width:c+"%"},h.animate),"max"===g&&"horizontal"===this.orientation&&this.range[j?"animate":"css"]({width:100-c+"%"},{queue:!1,duration:h.animate}),"min"===g&&"vertical"===this.orientation&&this.range.stop(1,1)[j?"animate":"css"]({height:c+"%"},h.animate),"max"===g&&"vertical"===this.orientation&&this.range[j?"animate":"css"]({height:100-c+"%"},{queue:!1,duration:h.animate}))},_handleEvents:{keydown:function(b){var e,f,g,h=a(b.target).data("ui-slider-handle-index");switch(b.keyCode){case a.ui.keyCode.HOME:case a.ui.keyCode.END:case a.ui.keyCode.PAGE_UP:case a.ui.keyCode.PAGE_DOWN:case a.ui.keyCode.UP:case a.ui.keyCode.RIGHT:case a.ui.keyCode.DOWN:case a.ui.keyCode.LEFT:if(b.preventDefault(),!this._keySliding&&(this._keySliding=!0,a(b.target).addClass("ui-state-active"),!1===this._start(b,h)))return}switch(g=this.options.step,e=f=this.options.values&&this.options.values.length?this.values(h):this.value(),b.keyCode){case a.ui.keyCode.HOME:f=this._valueMin();break;case a.ui.keyCode.END:f=this._valueMax();break;case a.ui.keyCode.PAGE_UP:f=this._trimAlignValue(e+(this._valueMax()-this._valueMin())/c);break;case a.ui.keyCode.PAGE_DOWN:f=this._trimAlignValue(e-(this._valueMax()-this._valueMin())/c);break;case a.ui.keyCode.UP:case a.ui.keyCode.RIGHT:if(e===this._valueMax())return;f=this._trimAlignValue(e+g);break;case a.ui.keyCode.DOWN:case a.ui.keyCode.LEFT:if(e===this._valueMin())return;f=this._trimAlignValue(e-g)}this._slide(b,h,f)},click:function(a){a.preventDefault()},keyup:function(b){var c=a(b.target).data("ui-slider-handle-index");this._keySliding&&(this._keySliding=!1,this._stop(b,c),this._change(b,c),a(b.target).removeClass("ui-state-active"))}}})}(jQuery),function(a){function b(a){return function(){var b=this.element.val();a.apply(this,arguments),this._refresh(),b!==this.element.val()&&this._trigger("change")}}a.widget("ui.spinner",{version:"1.10.2",defaultElement:"<input>",widgetEventPrefix:"spin",options:{culture:null,icons:{down:"ui-icon-triangle-1-s",up:"ui-icon-triangle-1-n"},incremental:!0,max:null,min:null,numberFormat:null,page:10,step:1,change:null,spin:null,start:null,stop:null},_create:function(){this._setOption("max",this.options.max),this._setOption("min",this.options.min),this._setOption("step",this.options.step),this._value(this.element.val(),!0),this._draw(),this._on(this._events),this._refresh(),this._on(this.window,{beforeunload:function(){this.element.removeAttr("autocomplete")}})},_getCreateOptions:function(){var b={},c=this.element;return a.each(["min","max","step"],function(a,d){var e=c.attr(d);void 0!==e&&e.length&&(b[d]=e)}),b},_events:{keydown:function(a){this._start(a)&&this._keydown(a)&&a.preventDefault()},keyup:"_stop",focus:function(){this.previous=this.element.val()},blur:function(a){if(this.cancelBlur)return void delete this.cancelBlur;this._stop(),this._refresh(),this.previous!==this.element.val()&&this._trigger("change",a)},mousewheel:function(a,b){if(b){if(!this.spinning&&!this._start(a))return!1;this._spin((b>0?1:-1)*this.options.step,a),clearTimeout(this.mousewheelTimer),this.mousewheelTimer=this._delay(function(){this.spinning&&this._stop(a)},100),a.preventDefault()}},"mousedown .ui-spinner-button":function(b){function d(){this.element[0]===this.document[0].activeElement||(this.element.focus(),this.previous=c,this._delay(function(){this.previous=c}))}var c;c=this.element[0]===this.document[0].activeElement?this.previous:this.element.val(),b.preventDefault(),d.call(this),this.cancelBlur=!0,this._delay(function(){delete this.cancelBlur,d.call(this)}),!1!==this._start(b)&&this._repeat(null,a(b.currentTarget).hasClass("ui-spinner-up")?1:-1,b)},"mouseup .ui-spinner-button":"_stop","mouseenter .ui-spinner-button":function(b){if(a(b.currentTarget).hasClass("ui-state-active"))return!1!==this._start(b)&&void this._repeat(null,a(b.currentTarget).hasClass("ui-spinner-up")?1:-1,b)},"mouseleave .ui-spinner-button":"_stop"},_draw:function(){var a=this.uiSpinner=this.element.addClass("ui-spinner-input").attr("autocomplete","off").wrap(this._uiSpinnerHtml()).parent().append(this._buttonHtml());this.element.attr("role","spinbutton"),this.buttons=a.find(".ui-spinner-button").attr("tabIndex",-1).button().removeClass("ui-corner-all"),this.buttons.height()>Math.ceil(.5*a.height())&&a.height()>0&&a.height(a.height()),this.options.disabled&&this.disable()},_keydown:function(b){var c=this.options,d=a.ui.keyCode;switch(b.keyCode){case d.UP:return this._repeat(null,1,b),!0;case d.DOWN:return this._repeat(null,-1,b),!0;case d.PAGE_UP:return this._repeat(null,c.page,b),!0;case d.PAGE_DOWN:return this._repeat(null,-c.page,b),!0}return!1},_uiSpinnerHtml:function(){return"<span class='ui-spinner ui-widget ui-widget-content ui-corner-all'></span>"},_buttonHtml:function(){return"<a class='ui-spinner-button ui-spinner-up ui-corner-tr'><span class='ui-icon "+this.options.icons.up+"'>&#9650;</span></a><a class='ui-spinner-button ui-spinner-down ui-corner-br'><span class='ui-icon "+this.options.icons.down+"'>&#9660;</span></a>"},_start:function(a){return!(!this.spinning&&!1===this._trigger("start",a))&&(this.counter||(this.counter=1),this.spinning=!0,!0)},_repeat:function(a,b,c){a=a||500,clearTimeout(this.timer),this.timer=this._delay(function(){this._repeat(40,b,c)},a),this._spin(b*this.options.step,c)},_spin:function(a,b){var c=this.value()||0;this.counter||(this.counter=1),c=this._adjustValue(c+a*this._increment(this.counter)),this.spinning&&!1===this._trigger("spin",b,{value:c})||(this._value(c),this.counter++)},_increment:function(b){var c=this.options.incremental;return c?a.isFunction(c)?c(b):Math.floor(b*b*b/5e4-b*b/500+17*b/200+1):1},_precision:function(){var a=this._precisionOf(this.options.step);return null!==this.options.min&&(a=Math.max(a,this._precisionOf(this.options.min))),a},_precisionOf:function(a){var b=a.toString(),c=b.indexOf(".");return-1===c?0:b.length-c-1},_adjustValue:function(a){var b,c,d=this.options;return b=null!==d.min?d.min:0,c=a-b,c=Math.round(c/d.step)*d.step,a=b+c,a=parseFloat(a.toFixed(this._precision())),null!==d.max&&a>d.max?d.max:null!==d.min&&a<d.min?d.min:a},_stop:function(a){this.spinning&&(clearTimeout(this.timer),clearTimeout(this.mousewheelTimer),this.counter=0,this.spinning=!1,this._trigger("stop",a))},_setOption:function(a,b){if("culture"===a||"numberFormat"===a){var c=this._parse(this.element.val());return this.options[a]=b,void this.element.val(this._format(c))}"max"!==a&&"min"!==a&&"step"!==a||"string"==typeof b&&(b=this._parse(b)),"icons"===a&&(this.buttons.first().find(".ui-icon").removeClass(this.options.icons.up).addClass(b.up),this.buttons.last().find(".ui-icon").removeClass(this.options.icons.down).addClass(b.down)),this._super(a,b),"disabled"===a&&(b?(this.element.prop("disabled",!0),this.buttons.button("disable")):(this.element.prop("disabled",!1),this.buttons.button("enable")))},_setOptions:b(function(a){this._super(a),this._value(this.element.val())}),_parse:function(a){return"string"==typeof a&&""!==a&&(a=window.Globalize&&this.options.numberFormat?Globalize.parseFloat(a,10,this.options.culture):+a),""===a||isNaN(a)?null:a},_format:function(a){return""===a?"":window.Globalize&&this.options.numberFormat?Globalize.format(a,this.options.numberFormat,this.options.culture):a},_refresh:function(){this.element.attr({"aria-valuemin":this.options.min,"aria-valuemax":this.options.max,"aria-valuenow":this._parse(this.element.val())})},_value:function(a,b){var c;""!==a&&null!==(c=this._parse(a))&&(b||(c=this._adjustValue(c)),a=this._format(c)),this.element.val(a),this._refresh()},_destroy:function(){this.element.removeClass("ui-spinner-input").prop("disabled",!1).removeAttr("autocomplete").removeAttr("role").removeAttr("aria-valuemin").removeAttr("aria-valuemax").removeAttr("aria-valuenow"),this.uiSpinner.replaceWith(this.element)},stepUp:b(function(a){this._stepUp(a)}),_stepUp:function(a){this._start()&&(this._spin((a||1)*this.options.step),this._stop())},stepDown:b(function(a){this._stepDown(a)}),_stepDown:function(a){this._start()&&(this._spin((a||1)*-this.options.step),this._stop())},pageUp:b(function(a){this._stepUp((a||1)*this.options.page)}),pageDown:b(function(a){this._stepDown((a||1)*this.options.page)}),value:function(a){if(!arguments.length)return this._parse(this.element.val());b(this._value).call(this,a)},widget:function(){return this.uiSpinner}})}(jQuery),function(a,b){function e(){return++c}function f(a){return a.hash.length>1&&decodeURIComponent(a.href.replace(d,""))===decodeURIComponent(location.href.replace(d,""))}var c=0,d=/#.*$/;a.widget("ui.tabs",{version:"1.10.2",delay:300,options:{active:null,collapsible:!1,event:"click",heightStyle:"content",hide:null,show:null,activate:null,beforeActivate:null,beforeLoad:null,load:null},_create:function(){var b=this,c=this.options;this.running=!1,this.element.addClass("ui-tabs ui-widget ui-widget-content ui-corner-all").toggleClass("ui-tabs-collapsible",c.collapsible).delegate(".ui-tabs-nav > li","mousedown"+this.eventNamespace,function(b){a(this).is(".ui-state-disabled")&&b.preventDefault()}).delegate(".ui-tabs-anchor","focus"+this.eventNamespace,function(){a(this).closest("li").is(".ui-state-disabled")&&this.blur()}),this._processTabs(),c.active=this._initialActive(),a.isArray(c.disabled)&&(c.disabled=a.unique(c.disabled.concat(a.map(this.tabs.filter(".ui-state-disabled"),function(a){return b.tabs.index(a)}))).sort()),!1!==this.options.active&&this.anchors.length?this.active=this._findActive(c.active):this.active=a(),this._refresh(),this.active.length&&this.load(c.active)},_initialActive:function(){var b=this.options.active,c=this.options.collapsible,d=location.hash.substring(1);return null===b&&(d&&this.tabs.each(function(c,e){if(a(e).attr("aria-controls")===d)return b=c,!1}),null===b&&(b=this.tabs.index(this.tabs.filter(".ui-tabs-active"))),null!==b&&-1!==b||(b=!!this.tabs.length&&0)),!1!==b&&-1===(b=this.tabs.index(this.tabs.eq(b)))&&(b=!c&&0),!c&&!1===b&&this.anchors.length&&(b=0),b},_getCreateEventData:function(){return{tab:this.active,panel:this.active.length?this._getPanelForTab(this.active):a()}},_tabKeydown:function(b){var c=a(this.document[0].activeElement).closest("li"),d=this.tabs.index(c),e=!0;if(!this._handlePageNav(b)){switch(b.keyCode){case a.ui.keyCode.RIGHT:case a.ui.keyCode.DOWN:d++;break;case a.ui.keyCode.UP:case a.ui.keyCode.LEFT:e=!1,d--;break;case a.ui.keyCode.END:d=this.anchors.length-1;break;case a.ui.keyCode.HOME:d=0;break;case a.ui.keyCode.SPACE:return b.preventDefault(),clearTimeout(this.activating),void this._activate(d);case a.ui.keyCode.ENTER:return b.preventDefault(),clearTimeout(this.activating),void this._activate(d!==this.options.active&&d);default:return}b.preventDefault(),clearTimeout(this.activating),d=this._focusNextTab(d,e),b.ctrlKey||(c.attr("aria-selected","false"),this.tabs.eq(d).attr("aria-selected","true"),this.activating=this._delay(function(){this.option("active",d)},this.delay))}},_panelKeydown:function(b){this._handlePageNav(b)||b.ctrlKey&&b.keyCode===a.ui.keyCode.UP&&(b.preventDefault(),this.active.focus())},_handlePageNav:function(b){return b.altKey&&b.keyCode===a.ui.keyCode.PAGE_UP?(this._activate(this._focusNextTab(this.options.active-1,!1)),!0):b.altKey&&b.keyCode===a.ui.keyCode.PAGE_DOWN?(this._activate(this._focusNextTab(this.options.active+1,!0)),!0):void 0},_findNextTab:function(b,c){function e(){return b>d&&(b=0),b<0&&(b=d),b}for(var d=this.tabs.length-1;-1!==a.inArray(e(),this.options.disabled);)b=c?b+1:b-1;return b},_focusNextTab:function(a,b){return a=this._findNextTab(a,b),this.tabs.eq(a).focus(),a},_setOption:function(a,b){return"active"===a?void this._activate(b):"disabled"===a?void this._setupDisabled(b):(this._super(a,b),"collapsible"===a&&(this.element.toggleClass("ui-tabs-collapsible",b),b||!1!==this.options.active||this._activate(0)),"event"===a&&this._setupEvents(b),void("heightStyle"===a&&this._setupHeightStyle(b)))},_tabId:function(a){return a.attr("aria-controls")||"ui-tabs-"+e()},_sanitizeSelector:function(a){return a?a.replace(/[!"$%&'()*+,.\/:;<=>?@\[\]\^`{|}~]/g,"\\$&"):""},refresh:function(){var b=this.options,c=this.tablist.children(":has(a[href])");b.disabled=a.map(c.filter(".ui-state-disabled"),function(a){return c.index(a)}),this._processTabs(),!1!==b.active&&this.anchors.length?this.active.length&&!a.contains(this.tablist[0],this.active[0])?this.tabs.length===b.disabled.length?(b.active=!1,this.active=a()):this._activate(this._findNextTab(Math.max(0,b.active-1),!1)):b.active=this.tabs.index(this.active):(b.active=!1,this.active=a()),this._refresh()},_refresh:function(){this._setupDisabled(this.options.disabled),this._setupEvents(this.options.event),this._setupHeightStyle(this.options.heightStyle),this.tabs.not(this.active).attr({"aria-selected":"false",tabIndex:-1}),this.panels.not(this._getPanelForTab(this.active)).hide().attr({"aria-expanded":"false","aria-hidden":"true"}),this.active.length?(this.active.addClass("ui-tabs-active ui-state-active").attr({"aria-selected":"true",tabIndex:0}),this._getPanelForTab(this.active).show().attr({"aria-expanded":"true","aria-hidden":"false"})):this.tabs.eq(0).attr("tabIndex",0)},_processTabs:function(){var b=this;this.tablist=this._getList().addClass("ui-tabs-nav ui-helper-reset ui-helper-clearfix ui-widget-header ui-corner-all").attr("role","tablist"),this.tabs=this.tablist.find("> li:has(a[href])").addClass("ui-state-default ui-corner-top").attr({role:"tab",tabIndex:-1}),this.anchors=this.tabs.map(function(){return a("a",this)[0]}).addClass("ui-tabs-anchor").attr({role:"presentation",tabIndex:-1}),this.panels=a(),this.anchors.each(function(c,d){var e,g,h,i=a(d).uniqueId().attr("id"),j=a(d).closest("li"),k=j.attr("aria-controls");f(d)?(e=d.hash,g=b.element.find(b._sanitizeSelector(e))):(h=b._tabId(j),e="#"+h,g=b.element.find(e),g.length||(g=b._createPanel(h),g.insertAfter(b.panels[c-1]||b.tablist)),g.attr("aria-live","polite")),g.length&&(b.panels=b.panels.add(g)),k&&j.data("ui-tabs-aria-controls",k),j.attr({"aria-controls":e.substring(1),"aria-labelledby":i}),g.attr("aria-labelledby",i)}),this.panels.addClass("ui-tabs-panel ui-widget-content ui-corner-bottom").attr("role","tabpanel")},_getList:function(){return this.element.find("ol,ul").eq(0)},_createPanel:function(b){return a("<div>").attr("id",b).addClass("ui-tabs-panel ui-widget-content ui-corner-bottom").data("ui-tabs-destroy",!0)},_setupDisabled:function(b){a.isArray(b)&&(b.length?b.length===this.anchors.length&&(b=!0):b=!1);for(var d,c=0;d=this.tabs[c];c++)!0===b||-1!==a.inArray(c,b)?a(d).addClass("ui-state-disabled").attr("aria-disabled","true"):a(d).removeClass("ui-state-disabled").removeAttr("aria-disabled");this.options.disabled=b},_setupEvents:function(b){var c={click:function(a){a.preventDefault()}};b&&a.each(b.split(" "),function(a,b){c[b]="_eventHandler"}),this._off(this.anchors.add(this.tabs).add(this.panels)),this._on(this.anchors,c),this._on(this.tabs,{keydown:"_tabKeydown"}),this._on(this.panels,{keydown:"_panelKeydown"}),this._focusable(this.tabs),this._hoverable(this.tabs)},_setupHeightStyle:function(b){var c,d=this.element.parent();"fill"===b?(c=d.height(),c-=this.element.outerHeight()-this.element.height(),this.element.siblings(":visible").each(function(){var b=a(this),d=b.css("position");"absolute"!==d&&"fixed"!==d&&(c-=b.outerHeight(!0))}),this.element.children().not(this.panels).each(function(){c-=a(this).outerHeight(!0)}),this.panels.each(function(){a(this).height(Math.max(0,c-a(this).innerHeight()+a(this).height()))}).css("overflow","auto")):"auto"===b&&(c=0,this.panels.each(function(){c=Math.max(c,a(this).height("").height())}).height(c))},_eventHandler:function(b){var c=this.options,d=this.active,e=a(b.currentTarget),f=e.closest("li"),g=f[0]===d[0],h=g&&c.collapsible,i=h?a():this._getPanelForTab(f),j=d.length?this._getPanelForTab(d):a(),k={oldTab:d,oldPanel:j,newTab:h?a():f,newPanel:i};b.preventDefault(),f.hasClass("ui-state-disabled")||f.hasClass("ui-tabs-loading")||this.running||g&&!c.collapsible||!1===this._trigger("beforeActivate",b,k)||(c.active=!h&&this.tabs.index(f),this.active=g?a():f,this.xhr&&this.xhr.abort(),j.length||i.length||a.error("jQuery UI Tabs: Mismatching fragment identifier."),i.length&&this.load(this.tabs.index(f),b),this._toggle(b,k))},_toggle:function(b,c){function g(){d.running=!1,d._trigger("activate",b,c)}function h(){c.newTab.closest("li").addClass("ui-tabs-active ui-state-active"),e.length&&d.options.show?d._show(e,d.options.show,g):(e.show(),g())}var d=this,e=c.newPanel,f=c.oldPanel;this.running=!0,f.length&&this.options.hide?this._hide(f,this.options.hide,function(){c.oldTab.closest("li").removeClass("ui-tabs-active ui-state-active"),h()}):(c.oldTab.closest("li").removeClass("ui-tabs-active ui-state-active"),f.hide(),h()),f.attr({"aria-expanded":"false","aria-hidden":"true"}),c.oldTab.attr("aria-selected","false"),e.length&&f.length?c.oldTab.attr("tabIndex",-1):e.length&&this.tabs.filter(function(){return 0===a(this).attr("tabIndex")}).attr("tabIndex",-1),e.attr({"aria-expanded":"true","aria-hidden":"false"}),c.newTab.attr({"aria-selected":"true",tabIndex:0})},_activate:function(b){var c,d=this._findActive(b);d[0]!==this.active[0]&&(d.length||(d=this.active),c=d.find(".ui-tabs-anchor")[0],this._eventHandler({target:c,currentTarget:c,preventDefault:a.noop}))},_findActive:function(b){return!1===b?a():this.tabs.eq(b)},_getIndex:function(a){return"string"==typeof a&&(a=this.anchors.index(this.anchors.filter("[href$='"+a+"']"))),a},_destroy:function(){this.xhr&&this.xhr.abort(),this.element.removeClass("ui-tabs ui-widget ui-widget-content ui-corner-all ui-tabs-collapsible"),this.tablist.removeClass("ui-tabs-nav ui-helper-reset ui-helper-clearfix ui-widget-header ui-corner-all").removeAttr("role"),this.anchors.removeClass("ui-tabs-anchor").removeAttr("role").removeAttr("tabIndex").removeUniqueId(),this.tabs.add(this.panels).each(function(){a.data(this,"ui-tabs-destroy")?a(this).remove():a(this).removeClass("ui-state-default ui-state-active ui-state-disabled ui-corner-top ui-corner-bottom ui-widget-content ui-tabs-active ui-tabs-panel").removeAttr("tabIndex").removeAttr("aria-live").removeAttr("aria-busy").removeAttr("aria-selected").removeAttr("aria-labelledby").removeAttr("aria-hidden").removeAttr("aria-expanded").removeAttr("role")}),this.tabs.each(function(){var b=a(this),c=b.data("ui-tabs-aria-controls");c?b.attr("aria-controls",c).removeData("ui-tabs-aria-controls"):b.removeAttr("aria-controls")}),this.panels.show(),"content"!==this.options.heightStyle&&this.panels.css("height","")},enable:function(c){var d=this.options.disabled;!1!==d&&(c===b?d=!1:(c=this._getIndex(c),d=a.isArray(d)?a.map(d,function(a){return a!==c?a:null}):a.map(this.tabs,function(a,b){return b!==c?b:null})),this._setupDisabled(d))},disable:function(c){var d=this.options.disabled;if(!0!==d){if(c===b)d=!0;else{if(c=this._getIndex(c),-1!==a.inArray(c,d))return;d=a.isArray(d)?a.merge([c],d).sort():[c]}this._setupDisabled(d)}},load:function(b,c){b=this._getIndex(b);var d=this,e=this.tabs.eq(b),g=e.find(".ui-tabs-anchor"),h=this._getPanelForTab(e),i={tab:e,panel:h};f(g[0])||(this.xhr=a.ajax(this._ajaxSettings(g,c,i)),this.xhr&&"canceled"!==this.xhr.statusText&&(e.addClass("ui-tabs-loading"),h.attr("aria-busy","true"),this.xhr.success(function(a){setTimeout(function(){h.html(a),d._trigger("load",c,i)},1)}).complete(function(a,b){setTimeout(function(){"abort"===b&&d.panels.stop(!1,!0),e.removeClass("ui-tabs-loading"),h.removeAttr("aria-busy"),a===d.xhr&&delete d.xhr},1)})))},_ajaxSettings:function(b,c,d){var e=this;return{url:b.attr("href"),beforeSend:function(b,f){return e._trigger("beforeLoad",c,a.extend({jqXHR:b,ajaxSettings:f},d))}}},_getPanelForTab:function(b){var c=a(b).attr("aria-controls");return this.element.find(this._sanitizeSelector("#"+c))}})}(jQuery),function(a){function c(b,c){var d=(b.attr("aria-describedby")||"").split(/\s+/);d.push(c),b.data("ui-tooltip-id",c).attr("aria-describedby",a.trim(d.join(" ")))}function d(b){var c=b.data("ui-tooltip-id"),d=(b.attr("aria-describedby")||"").split(/\s+/),e=a.inArray(c,d);-1!==e&&d.splice(e,1),b.removeData("ui-tooltip-id"),d=a.trim(d.join(" ")),d?b.attr("aria-describedby",d):b.removeAttr("aria-describedby")}var b=0;a.widget("ui.tooltip",{version:"1.10.2",options:{content:function(){var b=a(this).attr("title")||"";return a("<a>").text(b).html()},hide:!0,items:"[title]:not([disabled])",position:{my:"left top+15",at:"left bottom",collision:"flipfit flip"},show:!0,tooltipClass:null,track:!1,close:null,open:null},_create:function(){this._on({mouseover:"open",focusin:"open"}),this.tooltips={},this.parents={},this.options.disabled&&this._disable()},_setOption:function(b,c){var d=this;if("disabled"===b)return this[c?"_disable":"_enable"](),void(this.options[b]=c);this._super(b,c),"content"===b&&a.each(this.tooltips,function(a,b){d._updateContent(b)})},_disable:function(){var b=this;a.each(this.tooltips,function(c,d){var e=a.Event("blur");e.target=e.currentTarget=d[0],b.close(e,!0)}),this.element.find(this.options.items).addBack().each(function(){var b=a(this);b.is("[title]")&&b.data("ui-tooltip-title",b.attr("title")).attr("title","")})},_enable:function(){this.element.find(this.options.items).addBack().each(function(){var b=a(this);b.data("ui-tooltip-title")&&b.attr("title",b.data("ui-tooltip-title"))})},open:function(b){var c=this,d=a(b?b.target:this.element).closest(this.options.items);d.length&&!d.data("ui-tooltip-id")&&(d.attr("title")&&d.data("ui-tooltip-title",d.attr("title")),d.data("ui-tooltip-open",!0),b&&"mouseover"===b.type&&d.parents().each(function(){var d,b=a(this);b.data("ui-tooltip-open")&&(d=a.Event("blur"),d.target=d.currentTarget=this,c.close(d,!0)),b.attr("title")&&(b.uniqueId(),c.parents[this.id]={element:this,title:b.attr("title")},b.attr("title",""))}),this._updateContent(d,b))},_updateContent:function(a,b){var c,d=this.options.content,e=this,f=b?b.type:null;if("string"==typeof d)return this._open(b,a,d);(c=d.call(a[0],function(c){a.data("ui-tooltip-open")&&e._delay(function(){b&&(b.type=f),this._open(b,a,c)})}))&&this._open(b,a,c)},_open:function(b,d,e){function j(a){i.of=a,f.is(":hidden")||f.position(i)}var f,g,h,i=a.extend({},this.options.position);if(e){if(f=this._find(d),f.length)return void f.find(".ui-tooltip-content").html(e);d.is("[title]")&&(b&&"mouseover"===b.type?d.attr("title",""):d.removeAttr("title")),f=this._tooltip(d),c(d,f.attr("id")),f.find(".ui-tooltip-content").html(e),this.options.track&&b&&/^mouse/.test(b.type)?(this._on(this.document,{mousemove:j}),j(b)):f.position(a.extend({of:d},this.options.position)),f.hide(),this._show(f,this.options.show),this.options.show&&this.options.show.delay&&(h=this.delayedShow=setInterval(function(){f.is(":visible")&&(j(i.of),clearInterval(h))},a.fx.interval)),this._trigger("open",b,{tooltip:f}),g={keyup:function(b){if(b.keyCode===a.ui.keyCode.ESCAPE){var c=a.Event(b);c.currentTarget=d[0],this.close(c,!0)}},remove:function(){this._removeTooltip(f)}},b&&"mouseover"!==b.type||(g.mouseleave="close"),b&&"focusin"!==b.type||(g.focusout="close"),this._on(!0,d,g)}},close:function(b){var c=this,e=a(b?b.currentTarget:this.element),f=this._find(e);this.closing||(clearInterval(this.delayedShow),e.data("ui-tooltip-title")&&e.attr("title",e.data("ui-tooltip-title")),d(e),f.stop(!0),this._hide(f,this.options.hide,function(){c._removeTooltip(a(this))}),e.removeData("ui-tooltip-open"),this._off(e,"mouseleave focusout keyup"),e[0]!==this.element[0]&&this._off(e,"remove"),this._off(this.document,"mousemove"),b&&"mouseleave"===b.type&&a.each(this.parents,function(b,d){a(d.element).attr("title",d.title),delete c.parents[b]}),this.closing=!0,this._trigger("close",b,{tooltip:f}),this.closing=!1)},_tooltip:function(c){var d="ui-tooltip-"+b++,e=a("<div>").attr({id:d,role:"tooltip"}).addClass("ui-tooltip ui-widget ui-corner-all ui-widget-content "+(this.options.tooltipClass||""));return a("<div>").addClass("ui-tooltip-content").appendTo(e),e.appendTo(this.document[0].body),this.tooltips[d]=c,e},_find:function(b){var c=b.data("ui-tooltip-id");return c?a("#"+c):a()},_removeTooltip:function(a){a.remove(),delete this.tooltips[a.attr("id")]},_destroy:function(){var b=this;a.each(this.tooltips,function(c,d){var e=a.Event("blur");e.target=e.currentTarget=d[0],b.close(e,!0),a("#"+c).remove(),d.data("ui-tooltip-title")&&(d.attr("title",d.data("ui-tooltip-title")),d.removeData("ui-tooltip-title"))})}})}(jQuery),function(a){function e(a,b){if(!(a.originalEvent.touches.length>1)){a.preventDefault();var c=a.originalEvent.changedTouches[0],d=document.createEvent("MouseEvents");d.initMouseEvent(b,!0,!0,window,1,c.screenX,c.screenY,c.clientX,c.clientY,!1,!1,!1,!1,0,null),a.target.dispatchEvent(d)}}if(a.support.touch="ontouchend"in document,a.support.touch){var d,b=a.ui.mouse.prototype,c=b._mouseInit;b._touchStart=function(a){var b=this;!d&&b._mouseCapture(a.originalEvent.changedTouches[0])&&(d=!0,b._touchMoved=!1,e(a,"mouseover"),e(a,"mousemove"),e(a,"mousedown"))},b._touchMove=function(a){d&&(this._touchMoved=!0,e(a,"mousemove"),a.stopPropagation())},b._touchEnd=function(a){d&&(e(a,"mouseup"),e(a,"mouseout"),this._touchMoved||e(a,"click"),d=!1)},b._mouseInit=function(){var b=this;b.element.bind("touchstart",a.proxy(b,"_touchStart")).bind("touchmove",a.proxy(b,"_touchMove")).bind("touchend",a.proxy(b,"_touchEnd")),c.call(b)}}}(jQuery),function(a){a.widget("ui.dialog",a.ui.dialog,{_allowInteraction:function(a){return!0}}),a.ui.dialog.prototype._focusTabbable=function(){}}(jQuery),jQuery=oldJQuery;
;
/**
 * Drag Text module
 * @external {jQuery} $ H5P.jQuery
 */
H5P.DragText = (function ($, Question) {
  //CSS Main Containers:
  var MAIN_CONTAINER = "h5p-drag";
  var INNER_CONTAINER = "h5p-drag-inner";
  var TASK_CONTAINER = "h5p-drag-task";
  var WORDS_CONTAINER = "h5p-drag-droppable-words";
  var DROPZONE_CONTAINER = "h5p-drag-dropzone-container";
  var DRAGGABLES_CONTAINER = "h5p-drag-draggables-container";

  //Special Sub-containers:
  var DROPZONE = "h5p-drag-dropzone";
  var DRAGGABLE = "h5p-drag-draggable";
  var SHOW_SOLUTION_CONTAINER = "h5p-drag-show-solution-container";
  var DRAGGABLES_WIDE_SCREEN = 'h5p-drag-wide-screen';
  var DRAGGABLE_ELEMENT_WIDE_SCREEN = 'h5p-drag-draggable-wide-screen';

  //CSS Dropzone feedback:
  var CORRECT_FEEDBACK = 'h5p-drag-correct-feedback';
  var WRONG_FEEDBACK = 'h5p-drag-wrong-feedback';

  //CSS Draggable feedback:
  var DRAGGABLE_DROPPED = 'h5p-drag-dropped';
  var DRAGGABLE_FEEDBACK_CORRECT = 'h5p-drag-draggable-correct';
  var DRAGGABLE_FEEDBACK_WRONG = 'h5p-drag-draggable-wrong';

  /**
   * Initialize module.
   *
   * @class H5P.DragText
   * @extends H5P.Question
   * @param {Object} params Behavior settings
   * @param {Number} contentId Content identification
   * @param {Object} contentData Object containing task specific content data
   *
   * @returns {Object} DragText Drag Text instance
   */
  function DragText(params, contentId, contentData) {
    this.$ = $(this);
    this.contentId = contentId;
    Question.call(this, 'drag-text');



	//h5pcustomize
	var showSol = false;
	var retry = false;
	try{
	if(contentData != undefined && contentData.parent != undefined && contentData.parent.override != undefined && contentData.parent.override.overrideButtons !=undefined &&  contentData.parent.override.overrideButtons == "true" || contentData.parent.override.overrideButtons == true)
	{
	
		if(contentData.parent.override.overrideShowSolutionButton == "true" || contentData.parent.override.overrideShowSolutionButton == true)
		{
	
			showSol =true;
		}
		if(contentData.parent.override.overrideRetry == "true" || contentData.parent.override.overrideRetry == true)
		{
	
			retry =true;
		}
			
	} 
	}catch(e){}
	
	
    // Set default behavior.
    this.params = $.extend({}, {
      taskDescription: "",
      textField: "",
      checkAnswer: window.parent.Drupal.t("Check"), //"Check",
      tryAgain: window.parent.Drupal.t("LBL3200"), // "Retry",
      behaviour: {
        enableRetry: retry,
        enableSolutionsButton: showSol,
        instantFeedback: false
      },
      score: window.parent.Drupal.t("LBL3202")+ " @score "+window.parent.Drupal.t("LBL981")+" @total "+window.parent.Drupal.t("LBL1064"), //"Score : @score of @total.",
      showSolution : window.parent.Drupal.t("LBL3099")
    }, params);
	
	//h5pcustomize - override
	this.params.score =  window.parent.Drupal.t("LBL3202")+ " @score "+window.parent.Drupal.t("LBL981")+" @total "+window.parent.Drupal.t("LBL1064"); //"Score : @score of @total.",
    
    this.contentData = contentData;
    if (this.contentData !== undefined && this.contentData.previousState !== undefined && this.contentData.previousState.length !== undefined) {
      this.previousState = this.contentData.previousState;
    }

    // Keeps track of if Question has been answered
    this.answered = false;

    // Init drag text task
    this.initDragText();

    this.on('resize', this.resize, this);
  }

  DragText.prototype = Object.create(Question.prototype);
  DragText.prototype.constructor = DragText;

  /**
   * Registers this question type's DOM elements before they are attached.
   * Called from H5P.Question.
   */
  DragText.prototype.registerDomElements = function () {
    // Register task introduction text
    this.setIntroduction('<p>' + this.params.taskDescription + '</p>');

    // Register task content area
    this.setContent(this.$inner);

    // Register buttons
    this.addButtons();
  };

  /**
   * Initialize drag text task
   */
  DragText.prototype.initDragText = function () {
    this.$inner = $('<div/>', {
      class: INNER_CONTAINER
    });

    // Create task
    this.addTaskTo(this.$inner);

    // Set stored user state
    this.setH5PUserState();
	
    return this.$inner;
  };

  /**
   * Changes layout responsively when resized.
   * @public
   */
  DragText.prototype.resize = function () {
    this.changeLayoutToFitWidth();
  };

  /**
  * Adds the draggables on the right side of the screen if widescreen is detected.
  * @public
  */
  DragText.prototype.changeLayoutToFitWidth = function () {
    var self = this;
    self.addDropzoneWidth();

    //Find ratio of width to em, and make sure it is less than the predefined ratio, make sure widest draggable is less than a third of parent width.
    if ((self.$inner.width() / parseFloat(self.$inner.css("font-size"), 10) > 43) && (self.widestDraggable <= (self.$inner.width() / 3))) {
      // Adds a class that floats the draggables to the right.
      self.$draggables.addClass(DRAGGABLES_WIDE_SCREEN);
      // Detach and reappend the wordContainer so it will fill up the remaining space left by draggables.
      //Commented h5pcustomize
      //self.$wordContainer.detach().appendTo(self.$taskContainer);
      //added - To put draggable words down
      self.$wordContainer.detach().prependTo(self.$taskContainer);
      
      // Set margin so the wordContainer does not expand when there are no more draggables left.
      self.$wordContainer.css({'margin-right': self.widestDraggable});
      // Set all draggables to be blocks
      self.draggables.forEach(function (draggable) {
        draggable.getDraggableElement().addClass(DRAGGABLE_ELEMENT_WIDE_SCREEN);
      });
    } else {
      // Remove the specific wide screen settings.
      self.$wordContainer.css({'margin-right': 0});
      self.$draggables.removeClass(DRAGGABLES_WIDE_SCREEN);
      self.$draggables.detach().appendTo(self.$taskContainer);
      self.draggables.forEach(function (draggable) {
        draggable.getDraggableElement().removeClass(DRAGGABLE_ELEMENT_WIDE_SCREEN);
      });
    }
  };

  /**
   * Add check solution, show solution and retry buttons, and their functionality.
   * @public
   */
  DragText.prototype.addButtons = function () {
    var self = this;

    // Checking answer button
    self.addButton('check-answer', self.params.checkAnswer, function () {
      if (!self.showEvaluation()) {
        if (self.params.behaviour.enableRetry) {
          self.showButton('try-again');
        }
        if (self.params.behaviour.enableSolutionsButton) {
          self.showButton('show-solution');
        }
        self.hideButton('check-answer');
        self.disableDraggables();
      } else {
        self.hideButton('show-solution');
        self.hideButton('try-again');
        self.hideButton('check-answer');
      }
    }, !self.params.behaviour.instantFeedback);

    //Retry button
    self.addButton('try-again', self.params.tryAgain, function () {
      // Reset and shuffle draggables if Question is answered
      if (self.answered) {
        self.resetDraggables();
        self.addDraggablesRandomly(self.$draggables);
      }
      self.answered = false;

      self.hideEvaluation();

      self.hideButton('try-again');
      self.hideButton('show-solution');

      if (self.params.behaviour.instantFeedback) {
        self.enableAllDropzonesAndDraggables();
      } else {
        self.showButton('check-answer');
        self.enableDraggables();
      }
      self.hideAllSolutions();
    }, self.initShowTryAgainButton || false);

    //Show Solution button
    self.addButton('show-solution', self.params.showSolution, function () {
      self.droppables.forEach(function (droppable) {
        droppable.showSolution();
      });
      self.disableDraggables();
      self.hideButton('show-solution');
    }, self.initShowShowSolutionButton || false);
  };

  /**
   * Shows feedback for dropzones.
   * @public
   */
  DragText.prototype.showDropzoneFeedback = function () {
    this.droppables.forEach(function (droppable) {
      droppable.addFeedback();
    });
  };

  /**
   * Evaluate task and display score text for word markings.
   *
   * @return {Boolean} Returns true if maxScore was achieved.
   */
  DragText.prototype.showEvaluation = function () {
    this.hideEvaluation();
    this.calculateScore();
    this.showDropzoneFeedback();

    var score = this.correctAnswers;
    var maxScore = this.droppables.length;

    this.triggerXAPIScored(score, maxScore, 'answered');

    var scoreText = this.params.score.replace(/@score/g, score.toString())
      .replace(/@total/g, maxScore.toString());

    if (score === maxScore) {
      //Hide buttons and disable task
      this.hideButton('check-answer');
      this.hideButton('show-solution');
      this.hideButton('try-again');
      this.disableDraggables();
    }
    this.trigger('resize');

    // Set feedback score
    this.setFeedback(scoreText, score, maxScore);

    return score === maxScore;
  };

  /**
   * Calculate score and store them in class variables.
   * @public
   */
  DragText.prototype.calculateScore = function () {
    var self = this;
    self.correctAnswers = 0;
    self.droppables.forEach(function (entry) {
      if (entry.isCorrect()) {
        self.correctAnswers += 1;
      }
    });
  };

  /**
   * Clear the evaluation text.
   */
  DragText.prototype.hideEvaluation = function () {
    this.setFeedback();
    this.trigger('resize');
  };

  /**
   * Hides solution text for all dropzones.
   */
  DragText.prototype.hideAllSolutions = function () {
    this.droppables.forEach(function (droppable) {
      droppable.hideSolution();
    });
    this.trigger('resize');
  };

  /**
   * Handle task and add it to container.
   * @public
   * @param {jQuery} $container The object which our task will attach to.
   */
  DragText.prototype.addTaskTo = function ($container) {
    var self = this;
    self.widest = 0;
    self.widestDraggable = 0;
    self.droppables = [];
    self.draggables = [];

    self.$taskContainer = $('<div/>', {
      'class': TASK_CONTAINER
    });

    self.$draggables = $('<div/>', {
      'class': DRAGGABLES_CONTAINER
    });

    self.$wordContainer = $('<div/>', {'class': WORDS_CONTAINER});
    self.handleText();

    self.addDraggablesRandomly(self.$draggables);
    self.$wordContainer.appendTo(self.$taskContainer);
    self.$draggables.appendTo(self.$taskContainer);
    self.$taskContainer.appendTo($container);
    self.addDropzoneWidth();
  };

  /**
   * Parses the text and sends identified dropzones to the addDragNDrop method for further handling.
   * Appends the parsed text to wordContainer.
   * @public
   */
  DragText.prototype.handleText = function () {
    var self = this;

    //Replace newlines with break line tag
    var textField = self.params.textField.replace(/(\r\n|\n|\r)/gm, "<br/>");

    // Go through the text and replace all the asterisks with input fields
    var dropStart = textField.indexOf('*');
    var dropEnd = -1;
    var currentIndex = 0;
    //While the start of a dropbox is found
    while (dropStart !== -1) {
      dropStart += 1;
      dropEnd = textField.indexOf('*', dropStart);
      if (dropEnd === -1) {
        dropStart = -1;
      } else {
        //Appends the text between each dropzone
        self.$wordContainer.append(textField.slice(currentIndex, dropStart - 1));

        //Adds the drag n drop functionality when an answer is found
        self.addDragNDrop(textField.substring(dropStart, dropEnd));
        dropEnd += 1;
        currentIndex = dropEnd;

        //Attempts to find the beginning of the next answer.
        dropStart = textField.indexOf('*', dropEnd);
      }
    }
    //Appends the remaining part of the text.
    self.$wordContainer.append(textField.slice(currentIndex, textField.length));
  };

  /**
   * Matches the width of all dropzones to the widest draggable, and sets widest class variable.
   * @public
   */
  DragText.prototype.addDropzoneWidth = function () {
    var self = this;
    var widest = 0;
    var widestDragagble = 0;
    var fontSize = parseInt(this.$inner.css('font-size'), 10);
    var staticMinimumWidth = 3 * fontSize;
    var staticPadding = fontSize; // Needed to make room for feedback icons

    //Find widest draggable
    this.draggables.forEach(function (draggable) {
      var $draggableElement = draggable.getDraggableElement();

      //Find the initial natural width of the draggable.
      var $tmp = $draggableElement.clone().css({
        'position': 'absolute',
        'white-space': 'nowrap',
        'width': 'auto',
        'padding': 0,
        'margin': 0
      }).html(draggable.getAnswerText())
        .appendTo($draggableElement.parent());
      var width = $tmp.width();

      widestDragagble = width > widestDragagble ? width : widestDragagble;

      // Measure how big truncated draggable should be
      if ($tmp.text().length >= 20) {
        $tmp.html(draggable.getShortFormat());
        width = $tmp.width();
      }

      if (width + staticPadding > widest) {
        widest = width + staticPadding;
      }
      $tmp.remove();
    });
    // Set min size
    if (widest < staticMinimumWidth) {
      widest = staticMinimumWidth;
    }
    this.widestDraggable = widestDragagble;
    this.widest = widest;

    //Adjust all droppable to widest size.
    this.droppables.forEach(function (droppable) {
    
      droppable.getDropzone().width(self.widest);
    });
    //h5pcustomize override width
    $(".h5p-drag-dropped").parent().css("width","auto");
  };

  /**
   * Makes a drag n drop from the specified text.
   * @public
   * @param {String} text Text for the drag n drop.
   */
  DragText.prototype.addDragNDrop = function (text) {
    var self = this;
    var tip;
    var answer = text;
    var answersAndTip = answer.split(':');

    if (answersAndTip.length > 0) {
      answer = answersAndTip[0];
      tip = answersAndTip[1];
    }

    //Make the draggable
    var $draggable = $('<div/>', {
      html: answer,
      'class': DRAGGABLE
    }).draggable({
      revert: function (isValidDrop) { 
        var dropzone = droppable;
        if (!isValidDrop) {
          if (!self.$draggables.children().length) {
            // Show draggables container
            self.$draggables.removeClass('hide');
          }

          self.moveDraggableToDroppable(draggable, null);
          return false;
        }
        if (self.params.behaviour.instantFeedback) {
          if (dropzone !== null) {
            dropzone.addFeedback();
          }
          self.instantFeedbackEvaluation();
        }
		 //h5pcustomize
		 if($(".h5p-presentation-wrapper").size() == 0) //only for video
		 {
		 	event.target.style = "width:auto;";
		 	$(".h5p-drag-dropped").parent().css("width","auto");
		 }        
        return false;
      },
      containment: self.$taskContainer
    });

    var draggable = new Draggable(answer, $draggable);
    draggable.on('addedToZone', function (event) {
      self.triggerXAPI('interacted');
    });

    //Make the dropzone
    var $dropzoneContainer = $('<div/>', {
      'class': DROPZONE_CONTAINER
    });
    var $dropzone = $('<div/>', {
      'class': DROPZONE
    }).appendTo($dropzoneContainer)
      .droppable({
        tolerance: 'pointer',
        drop: function (event, ui) {
        
          self.draggables.forEach(function (draggable) {
            if (draggable.getDraggableElement().is(ui.draggable)) {
              self.moveDraggableToDroppable(draggable, droppable);
              
            }
          });
          if (self.params.behaviour.instantFeedback) {
            droppable.addFeedback();
            if (!self.params.behaviour.enableRetry) {
              droppable.disableDropzoneAndContainedDraggable();
            }
            if (droppable.isCorrect()) {
              droppable.disableDropzoneAndContainedDraggable();
            }
          }
		  //h5pcustomize - adjust the width of the dropped zone based on the content
		  if($(".h5p-presentation-wrapper").size() == 0) //only for video
		  {
		  	event.target.style = "width:auto;";
		  	$(".h5p-drag-dropped").parent().css("width","auto");
		  }
		  
          // Hide draggables container if it is empty
          self.$draggables.toggleClass('hide', !self.$draggables.children().length);
        }
      });

    var droppable = new Droppable(answer, tip, $dropzone, $dropzoneContainer);
    droppable.appendDroppableTo(self.$wordContainer);

    self.draggables.push(draggable);
    self.droppables.push(droppable);
  };

  /**
   * Moves a draggable onto a droppable, and updates all parameters in the objects.
   * @public
   * @param {Draggable} draggable Draggable instance.
   * @param {Droppable} droppable The droppable instance the draggable is put on.
   */
  DragText.prototype.moveDraggableToDroppable = function (draggable, droppable) {
    draggable.removeFromZone();
    if (droppable !== null) {
      this.answered = true;
      droppable.appendInsideDroppableTo(this.$draggables);
      droppable.setDraggable(draggable);
      draggable.appendDraggableTo(droppable.getDropzone());
    } else {
      draggable.revertDraggableTo(this.$draggables);
    }
    this.trigger('resize');
  };

  /**
   * Adds the draggable words to the provided container in random order.
   * @public
   * @param {jQuery} $container Container the draggables will be added to.
   */
  DragText.prototype.addDraggablesRandomly = function ($container) {
    var tempArray = this.draggables.slice();
    var randIndex = 0;
    while (tempArray.length >= 1) {
      randIndex = parseInt(Math.random() * tempArray.length, 10);
      tempArray[randIndex].appendDraggableTo($container);
      tempArray.splice(randIndex, 1);
    }
  };

  /**
   * Feedback function for checking if all fields are filled, and show evaluation if that is the case.
   */
  DragText.prototype.instantFeedbackEvaluation = function () {
    var self = this;
    var allFilled = self.isAllAnswersFilled();

    if (allFilled) {
      //Shows "retry" and "show solution" buttons.
      if (self.params.behaviour.enableSolutionsButton) {
        self.showButton('show-solution');
      }
      if (self.params.behaviour.enableRetry) {
        self.showButton('try-again');
      }

      // Shows evaluation text
      self.showEvaluation();
    } else {
      //Hides "retry" and "show solution" buttons.
      self.hideButton('try-again');
      self.hideButton('show-solution');

      //Hides evaluation text.
      self.hideEvaluation();
    }
  };

  /**
   * Check if all answers are filled
   * @returns {boolean} allFilled Returns true if all answers are answered
   */
  DragText.prototype.isAllAnswersFilled = function () {
    var self = this;
    var allFilled = true;
    self.draggables.forEach(function (entry) {
      if (entry.insideDropzone === null) {
        allFilled = false;
      }
    });

    return allFilled;
  };

  /**
   * Enables all dropzones and all draggables.
   */
  DragText.prototype.enableAllDropzonesAndDraggables = function () {
    this.enableDraggables();
    this.droppables.forEach(function (droppable) {
      droppable.enableDropzone();
    });
  };

  /**
   * Disables all draggables, user will not be able to interact with them any more.
   * @public
   */
  DragText.prototype.disableDraggables = function () {
    this.draggables.forEach(function (entry) {
      entry.disableDraggable();
    });
  };

  /**
   * Enables all draggables, user will be able to interact with them again.
   * @public
   */
  DragText.prototype.enableDraggables = function () {
    this.draggables.forEach(function (entry) {
      entry.enableDraggable();
    });
  };

  /**
   * Used for contracts.
   * Checks if the parent program can proceed. Always true.
   * @public
   * @returns {Boolean} true
   */
  DragText.prototype.getAnswerGiven = function () {
    return this.answered;
  };

  /**
   * Used for contracts.
   * Checks the current score for this task.
   * @public
   * @returns {Number} The current score.
   */
  DragText.prototype.getScore = function () {
    this.calculateScore();
    return this.correctAnswers;
  };

  /**
   * Used for contracts.
   * Checks the maximum score for this task.
   * @public
   * @returns {Number} The maximum score.
   */
  DragText.prototype.getMaxScore = function () {
    return this.droppables.length;
  };

  /**
   * Get title of task
   * @returns {string} title
   */
  DragText.prototype.getTitle = function () {
    return H5P.createTitle(this.params.taskDescription);
  };

  /**
   * Used for contracts.
   * Sets feedback on the dropzones.
   * @public
   */
  DragText.prototype.showSolutions = function () {
    this.droppables.forEach(function (droppable) {
      droppable.addFeedback();
      droppable.showSolution();
    });
    this.disableDraggables();
    //Remove all buttons in "show solution" mode.
    this.hideButton('try-again');
    this.hideButton('show-solution');
    this.hideButton('check-answer');
    this.trigger('resize');
  };

  /**
   * Used for contracts.
   * Resets the complete task back to its' initial state.
   * @public
   */
  DragText.prototype.resetTask = function () {
    var self = this;
    // Reset task answer
    self.answered = false;
    //Reset draggables parameters and position
    self.resetDraggables();
    //Hides solution text and re-enable draggables
    self.hideEvaluation();
    self.enableAllDropzonesAndDraggables();
    //Show and hide buttons
    self.hideButton('try-again');
    self.hideButton('show-solution');

    if (!self.params.behaviour.instantFeedback) {
      self.showButton('check-answer');
    }
    self.hideAllSolutions();
    this.trigger('resize');
  };

  /**
   * Resets the position of all draggables.
   */
  DragText.prototype.resetDraggables = function () {
    var self = this;
    // Show draggables container
    self.$draggables.removeClass('hide');
    self.draggables.forEach(function (entry) {
      self.moveDraggableToDroppable(entry, null);
    });
    this.trigger('resize');
  };

  /**
   * Returns an object containing the dropped words
   * @returns {object} containing indexes of dropped words
   */
  DragText.prototype.getCurrentState = function () {
    var self = this;
    var draggedDraggablesIndexes = [];

    // Return undefined if task is not initialized
    if (this.draggables === undefined) {
      return undefined;
    }

    // Find draggables that has been dropped
    this.draggables.forEach(function (draggable, draggableIndex) {
      if (draggable.getInsideDropzone() !== null) {
        draggedDraggablesIndexes.push({draggable: draggableIndex, droppable: self.droppables.indexOf(draggable.getInsideDropzone())});
      }
    });
    return draggedDraggablesIndexes;
  };

  /**
   * Sets answers to current user state
   */
  DragText.prototype.setH5PUserState = function () {
    var self = this;

    // Do nothing if user state is undefined
    if (this.previousState === undefined) {
      return;
    }
     
    // Select words from user state
    //h5pcustomize. if retry is disabled, do not allow user to change
    if(self.params.behaviour.enableRetry == false && this.previousState.length > 0) // >0 attempted
    	self.params.behaviour.instantFeedback = true;
    this.previousState.forEach(function (draggedDraggableIndexes) {
      var draggableIndexIsInvalid = isNaN(draggedDraggableIndexes.draggable) ||
        draggedDraggableIndexes.draggable >= self.draggables.length ||
        draggedDraggableIndexes.draggable < 0;

      var droppableIndexIsInvalid = isNaN(draggedDraggableIndexes.droppable) ||
        draggedDraggableIndexes.droppable >= self.droppables.length ||
        draggedDraggableIndexes.droppable < 0;

      if (draggableIndexIsInvalid || droppableIndexIsInvalid) {
        throw new Error('Stored user state is invalid');
      }

      var moveDraggable = self.draggables[draggedDraggableIndexes.draggable];
      var moveToDroppable = self.droppables[draggedDraggableIndexes.droppable];
      self.moveDraggableToDroppable(moveDraggable, moveToDroppable);
      
      if (self.params.behaviour.instantFeedback) {
        // Add feedback to dropzone
        if (moveToDroppable !== null) {
          moveToDroppable.addFeedback();
        }

        // Add feedback to draggable
        if (moveToDroppable.isCorrect()) {
          moveToDroppable.disableDropzoneAndContainedDraggable();
        }
      }
    });

    // Show evaluation if task is finished
    if (self.params.behaviour.instantFeedback) {

      // Show buttons if not max score and all answers filled
      if (self.isAllAnswersFilled() && !self.showEvaluation()) {

        //Shows "retry" and "show solution" buttons.
        if (self.params.behaviour.enableSolutionsButton) {
          self.initShowShowSolutionButton = true;
        }
        if (self.params.behaviour.enableRetry) {
          self.initShowTryAgainButton = true;
        }
      }
    }
  };

  /**
   * Private class for keeping track of draggable text.
   * @private
   * @param {String} text String that will be turned into a selectable word.
   * @param {jQuery} draggable Draggable object.
   */
  function Draggable(text, draggable) {
    H5P.EventDispatcher.call(this);
    var self = this;
    self.text = text;
    self.insideDropzone = null;
    self.$draggable = $(draggable);

    self.shortFormat = self.text;
    //Shortens the draggable string if inside a dropbox.
    if (self.shortFormat.length > 20) {
      self.shortFormat = self.shortFormat.slice(0, 17) + '...';
    }
  }

  Draggable.prototype = Object.create(H5P.EventDispatcher.prototype);
  Draggable.prototype.constructor = Draggable;

  /**
   * Moves the draggable to the provided container.
   * @public
   * @param {jQuery} $container Container the draggable will append to.
   */
  Draggable.prototype.appendDraggableTo = function ($container) {
    this.$draggable.detach().css({left: 0, top: 0}).appendTo($container);
  };

  /**
   * Reverts the draggable to its' provided container.
   * @public
   * @params {jQuery} $container The parent which the draggable will revert to.
   */
  Draggable.prototype.revertDraggableTo = function ($container) {
    // get the relative distance between draggable and container.
    var offLeft = this.$draggable.offset().left - $container.offset().left;
    var offTop = this.$draggable.offset().top - $container.offset().top;

    // Prepend draggable to new container, but keep the offset,
    // then animate to new container's top:0, left:0
    this.$draggable.detach()
      .prependTo($container)
      .css({left: offLeft, top: offTop})
      .animate({left: 0, top: 0});
  };

  /**
   * Sets dropped feedback if the on the draggable if parameter is true.
   * @public
   * @params {Boolean} isDropped Decides whether the draggable has been dropped.
   */
  Draggable.prototype.toggleDroppedFeedback = function (isDropped) {
    if (isDropped) {
      this.$draggable.addClass(DRAGGABLE_DROPPED);
    } else {
      this.$draggable.removeClass(DRAGGABLE_DROPPED);
    }
  };

  /**
   * Disables the draggable, making it immovable.
   * @public
   */
  Draggable.prototype.disableDraggable = function () {
    this.$draggable.draggable({ disabled: true});
  };

  /**
   * Enables the draggable, making it movable.
   * @public
   */
  Draggable.prototype.enableDraggable = function () {
    this.$draggable.draggable({ disabled: false});
  };

  /**
   * Gets the draggable jQuery object for this class.
   * @public
   *
   * @returns {jQuery} Draggable item.
   */
  Draggable.prototype.getDraggableElement = function () {
    return this.$draggable;
  };

  /**
   * Removes this draggable from its dropzone, if it is contained in one.
   * @public
   */
  Draggable.prototype.removeFromZone = function () {
    if (this.insideDropzone !== null) {
      this.insideDropzone.removeFeedback();
      this.insideDropzone.removeDraggable();
    }
    this.toggleDroppedFeedback(false);
    this.removeShortFormat();
    this.insideDropzone = null;
  };

  /**
   * Adds this draggable to the given dropzone.
   * @public
   * @param {Droppable} droppable The droppable this draggable will be added to.
   */
  Draggable.prototype.addToZone = function (droppable) {
    this.trigger('addedToZone');
    if (this.insideDropzone !== null) {
      this.insideDropzone.removeDraggable();
    }
    this.toggleDroppedFeedback(true);
    this.insideDropzone = droppable;
    this.setShortFormat();
  };

  /**
   * Gets the answer text for this draggable.
   * @public
   *
   * @returns {String} The answer text in this draggable.
   */
  Draggable.prototype.getAnswerText = function () {
    return this.text;
  };

  /**
   * Sets short format of draggable when inside a dropbox.
   * @public
   */
  Draggable.prototype.setShortFormat = function () {
    this.$draggable.html(this.shortFormat);
  };

  /**
   * Get short format of draggable when inside a dropbox.
   * @returns {String|*}
   */
  Draggable.prototype.getShortFormat = function () {
    return this.shortFormat;
  };

  /**
   * Removes the short format of draggable when it is outside a dropbox.
   * @public
   */
  Draggable.prototype.removeShortFormat = function () {
    this.$draggable.html(this.text);
  };

  /**
   * Get the droppable this draggable is inside
   * @returns {Droppable} Droppable
   */
  Draggable.prototype.getInsideDropzone = function () {
    return this.insideDropzone;
  };

  /**
   * Private class for keeping track of droppable zones.
   * @private
   *
   * @param {String} text Correct text string for this drop box.
   * @param {undefined/String} tip Tip for this container, optional.
   * @param {jQuery} dropzone Dropzone object.
   * @param {jQuery} dropzoneContainer Container Container for the dropzone.
   */
  function Droppable(text, tip, dropzone, dropzoneContainer) {
    var self = this;
    self.text = text;
    self.tip = tip;
    self.containedDraggable = null;
    self.$dropzone = $(dropzone);
    self.$dropzoneContainer = $(dropzoneContainer);

    if (self.tip !== undefined) {
      self.$dropzone.append(H5P.JoubelUI.createTip(self.tip, self.$dropzone));
    }

    self.$showSolution = $('<div/>', {
      'class': SHOW_SOLUTION_CONTAINER
    }).appendTo(self.$dropzoneContainer).hide();
  }

  /**
   * Displays the solution next to the drop box if it is not correct.
   * @public
   */
  Droppable.prototype.showSolution = function () {
    if (!((this.containedDraggable !== null) && (this.containedDraggable.getAnswerText() === this.text))) {
      this.$showSolution.html(this.text);
      this.$showSolution.show();
    }
  };

  /**
   * Hides the solution.
   * @public
   */
  Droppable.prototype.hideSolution = function () {
    this.$showSolution.html('');
    this.$showSolution.hide();
  };

  /**
   * Appends the droppable to the provided container.
   * @public
   * @param {jQuery} $container Container which the dropzone will be appended to.
   */
  Droppable.prototype.appendDroppableTo = function ($container) {
    this.$dropzoneContainer.appendTo($container);
  };
  /**
   * Appends the draggable contained within this dropzone to the argument.
   * @public
   * @param {jQuery} $container Container which the draggable will append to.
   */
  Droppable.prototype.appendInsideDroppableTo = function ($container) {
    if (this.containedDraggable !== null) {
      this.containedDraggable.revertDraggableTo($container);
    }
  };

  /**
   * Sets the contained draggable in this drop box to the provided argument.
   * @public
   * @param {Draggable} droppedDraggable A draggable that has been dropped on this box.
   */
  Droppable.prototype.setDraggable = function (droppedDraggable) {
    var self = this;
    if (self.containedDraggable === droppedDraggable) {
      return;
    }
    if (self.containedDraggable !== null) {
      self.containedDraggable.removeFromZone();
    }
    self.containedDraggable = droppedDraggable;
    droppedDraggable.addToZone(self);
  };

  /**
   * Removes the contained draggable in this box.
   * @public
   */
  Droppable.prototype.removeDraggable = function () {
    if (this.containedDraggable !== null) {
      this.containedDraggable = null;
    }
  };

  /**
   * Checks if this drop box contains the correct draggable.
   * @public
   *
   * @returns {Boolean} True if this box has the correct answer.
   */
  Droppable.prototype.isCorrect = function () {
    if (this.containedDraggable === null) {
      return false;
    }
    return this.containedDraggable.getAnswerText() === this.text;
  };

  /**
   * Sets CSS styling feedback for this drop box.
   * @public
   */
  Droppable.prototype.addFeedback = function () {
    //Draggable is correct
    if (this.isCorrect()) {
      this.$dropzone.removeClass(WRONG_FEEDBACK).addClass(CORRECT_FEEDBACK);

      //Draggable feedback
      this.containedDraggable.getDraggableElement().removeClass(DRAGGABLE_FEEDBACK_WRONG).addClass(DRAGGABLE_FEEDBACK_CORRECT);
    } else if (this.containedDraggable === null) {
      //Does not contain a draggable
      this.$dropzone.removeClass(WRONG_FEEDBACK).removeClass(CORRECT_FEEDBACK);
    } else {
      //Draggable is wrong
      this.$dropzone.removeClass(CORRECT_FEEDBACK).addClass(WRONG_FEEDBACK);

      //Draggable feedback
      if (this.containedDraggable !== null) {
        this.containedDraggable.getDraggableElement().addClass(DRAGGABLE_FEEDBACK_WRONG).removeClass(DRAGGABLE_FEEDBACK_CORRECT);
      }
    }
  };

  /**
   * Removes all CSS styling feedback for this drop box.
   * @public
   */
  Droppable.prototype.removeFeedback = function () {
    this.$dropzone.removeClass(WRONG_FEEDBACK).removeClass(CORRECT_FEEDBACK);

    //Draggable feedback
    if (this.containedDraggable !== null) {
      this.containedDraggable.getDraggableElement().removeClass(DRAGGABLE_FEEDBACK_WRONG).removeClass(DRAGGABLE_FEEDBACK_CORRECT);
    }
  };

  /**
   * Sets short format of draggable when inside a dropbox.
   * @public
   */
  Droppable.prototype.setShortFormat = function () {
    if (this.containedDraggable !== null) {
      this.containedDraggable.setShortFormat();
    }
  };

  /**
   * Disables dropzone and the contained draggable.
   */
  Droppable.prototype.disableDropzoneAndContainedDraggable = function () {
    if (this.containedDraggable !== null) {
      this.containedDraggable.disableDraggable();
    }
    this.$dropzone.droppable({ disabled: true});
  };

  /**
   * Enable dropzone.
   */
  Droppable.prototype.enableDropzone = function () {
    this.$dropzone.droppable({ disabled: false});
  };

  /**
   * Removes the short format of draggable when it is outside a dropbox.
   * @public
   */
  Droppable.prototype.removeShortFormat = function () {
    if (this.containedDraggable !== null) {
      this.containedDraggable.removeShortFormat();
    }
  };

  /**
   * Gets this object's dropzone jQuery object.
   * @public
   *
   * @returns {jQuery} This object's dropzone.
   */
  Droppable.prototype.getDropzone = function () {
    return this.$dropzone;
  };

  return DragText;

}(H5P.jQuery, H5P.Question));
;/*global H5P*/

/**
 * Mark The Words module
 * @external {jQuery} $ H5P.jQuery
 */
H5P.MarkTheWords = (function ($, Question) {
  // CSS Main Containers:
  var MAIN_CONTAINER = "h5p-word";
  var INNER_CONTAINER = "h5p-word-inner";
  var WORDS_CONTAINER = "h5p-word-selectable-words";
  var BUTTON_CONTAINER = "h5p-button-bar";

  // CSS Classes for marking words:
  var MISSED_MARK = "h5p-word-missed";
  var CORRECT_MARK = "h5p-word-correct";
  var WRONG_MARK = "h5p-word-wrong";
  var SELECTED_MARK = "h5p-word-selected";
  var SELECTABLE_MARK = "h5p-word-selectable";
  var WORD_DISABLED = "h5p-word-disabled";

  /**
   * Initialize module.
   *
   * @class H5P.MarkTheWords
   * @extends H5P.Question
   * @param {Object} params Behavior settings
   * @param {Number} contentId Content identification
   * @param {Object} contentData Object containing task specific content data
   *
   * @returns {Object} MarkTheWords Mark the words instance
   */
  function MarkTheWords(params, contentId, contentData) {
    this.contentId = contentId;

    Question.call(this, 'mark-the-words');
    
    //h5pcustomize
	var showSol = false;
	var retry = false;
	try{
	if(contentData != undefined && contentData.parent != undefined && contentData.parent.override != undefined && contentData.parent.override.overrideButtons !=undefined &&  contentData.parent.override.overrideButtons == "true" || contentData.parent.override.overrideButtons == true)
	{
	
		if(contentData.parent.override.overrideShowSolutionButton == "true" || contentData.parent.override.overrideShowSolutionButton == true)
		{
	
			showSol =true;
		}
		if(contentData.parent.override.overrideRetry == "true" || contentData.parent.override.overrideRetry == true)
		{
	
			retry =true;
		}
			
	} 
	}catch(e){}
	
	
    // Set default behavior.
    this.params = $.extend({}, {
      taskDescription: "",
      textField: "This is a *nice*, *flexible* content type.",
      behaviour: {
        enableRetry: retry,
        enableSolutionsButton: showSol
      },
      checkAnswerButton: window.parent.Drupal.t("Check"),
      tryAgainButton: window.parent.Drupal.t("LBL3200"),
      showSolutionButton: window.parent.Drupal.t("LBL3099"),
      score: window.parent.Drupal.t("LBL3202")+ " @score "+window.parent.Drupal.t("LBL981")+" @total "+window.parent.Drupal.t("LBL1064") // "You got @score of @total points."
    }, params);
    
    //h5pcustomize - override h5p label
    this.params.score= window.parent.Drupal.t("LBL3202")+ " @score "+window.parent.Drupal.t("LBL981")+" @total "+window.parent.Drupal.t("LBL1064");
    this.contentData = contentData;
    if (this.contentData !== undefined && this.contentData.previousState !== undefined) {
    	//if(retry == false) //fix for 84393
      this.previousState = this.contentData.previousState;
    }

    this.initMarkTheWords();
    
  }
;
  MarkTheWords.prototype = Object.create(H5P.EventDispatcher.prototype);
  MarkTheWords.prototype.constructor = MarkTheWords;

  /**
   * Initialize Mark The Words task
   */
  MarkTheWords.prototype.initMarkTheWords = function () {
    this.$inner = $('<div class=' + INNER_CONTAINER + '></div>');

    this.addTaskTo(this.$inner);

    // Set user state
    this.setH5PUserState();
  };

  /**
   * Recursive function that creates html for the words
   * @method createHtmlForWords
   * @param  {Array}           nodes Array of dom nodes
   * @return {string}
   */
  MarkTheWords.prototype.createHtmlForWords = function (nodes) {
    var self = this;
    var html = '';
    for (var i = 0; i < nodes.length; i++) {
      var node = nodes[i];

      if (node instanceof Text) {
        var text = $(node).text();
        var selectableStrings = text.replace(/(&nbsp;|\r\n|\n|\r)/g, ' ').match(/ \*[^\*]+\* |[^\s]+/g);
        var spaceIncludedSelectableStrings = new Array();
        var itr = 0;
        var invalid = false;
        var invalidSelectableWordArray = new Array();
        for(var i = 0; i < selectableStrings.length;i++)
        	{
        		var w = selectableStrings[i];
        		if(w.indexOf("*") < 0 && !invalid) //not found selectable 
        		{
        			spaceIncludedSelectableStrings[itr++]= w;
        		}else if(w[0] == "*" && w[w.length-1] == "*" && !invalid) //right one
        		{
        			spaceIncludedSelectableStrings[itr++]= w;	
        		}
        		else { //invalid
        			invalid = true;
        			invalidSelectableWordArray[invalidSelectableWordArray.length] = w;
        			if(w.indexOf("*") > 0)
        			{
        				invalid = false;
        				spaceIncludedSelectableStrings[itr++] = invalidSelectableWordArray.join(' ');
        				invalidSelectableWordArray = new Array();
         		}
        			
        		}
        	}
        selectableStrings = spaceIncludedSelectableStrings;
        if (selectableStrings) {
          selectableStrings.forEach(function (entry) {
            entry = entry.trim();

            // Words
            if (html) {
              // Add space before
              html += ' ';
            }

            // Remove prefix punctuations from word
            var prefix = entry.match(/^[\[\({⟨¿¡"«„]+/);
            var start = 0;
            if (prefix !== null) {
              start = prefix.length;
              html += prefix;
            }

            // Remove suffix punctuations from word
            var suffix = entry.match(/[",….:;?!\]\)}⟩»”]+$/);
            var end = entry.length - start;
            if (suffix !== null) {
              end -= suffix.length;
            }

            // Word
            entry = entry.substr(start, end);
            if (entry.length) {
              html += '<span class="' + SELECTABLE_MARK + '">' + entry + '</span>';
            }

            if (suffix !== null) {
              html += suffix;
            }
          });
        }
        else if ((selectableStrings !== null) && text.length) {
          html += '<span class="' + SELECTABLE_MARK + '">' + text + '</span>';
        }
      }
      else {
        if (node.nodeName === 'BR') {
          html += '<br/>';
        }
        else {
          var attributes = ' ';
          for (var j = 0; j < node.attributes.length; j++) {
            attributes +=node.attributes[j].name + '="' + node.attributes[j].nodeValue + '" ';
          }
          html += '<' + node.nodeName +  attributes + '>';
          html += self.createHtmlForWords(node.childNodes);
          html += '</' + node.nodeName + '>';
        }
      }
    }
    console.log(html);
    return html;
  };

  /**
   * Handle task and add it to container.
   * @param {jQuery} $container The object which our task will attach to.
   */
  MarkTheWords.prototype.addTaskTo = function ($container) {
    var self = this;
    self.selectableWords = [];
    self.answers = 0;

    // Wrapper
    var $wordContainer = $('<div/>', {
      'class': WORDS_CONTAINER,
      html: self.createHtmlForWords($.parseHTML(self.params.textField))
    });

    $wordContainer.find('.' + SELECTABLE_MARK).each(function () {
      var selectableWord = new Word($(this));
      selectableWord.on('xAPI', function (event) {
        if (event.getVerb() === 'interacted') {
          self.triggerXAPI('interacted');
        }
      });
      if (selectableWord.isAnswer()) {
        self.answers += 1;
      }
      self.selectableWords.push(selectableWord);
    });

    $wordContainer.appendTo($container);
    self.$wordContainer = $wordContainer;
  };

  /**
   * Add check solution and retry buttons.
   */
  MarkTheWords.prototype.addButtons = function () {
    var self = this;
    self.$buttonContainer = $('<div/>', {'class': BUTTON_CONTAINER});

    this.addButton('check-answer', this.params.checkAnswerButton, function () {
      self.setAllSelectable(false);
      self.feedbackSelectedWords();
      self.hideButton('check-answer');
      if (!self.showEvaluation()) {
        // Only show if a correct answer was not found.
        if (self.params.behaviour.enableSolutionsButton && (self.correctAnswers < self.answers)) {
          self.showButton('show-solution');
        }
        if (self.params.behaviour.enableRetry) {
          self.showButton('try-again');
        }
      }
    });

    this.addButton('try-again', this.params.tryAgainButton, function () {
      self.clearAllMarks();
      self.hideEvaluation();
      self.setAllSelectable(true);
      self.hideButton('try-again');
      self.hideButton('show-solution');
      self.showButton('check-answer');
    }, false);

    this.addButton('show-solution', this.params.showSolutionButton, function () {
      self.setAllSelectable(false);
      self.setAllMarks();
      self.hideButton('check-answer');
      self.hideButton('show-solution');
      if (self.params.behaviour.enableRetry) {
        self.showButton('try-again');
      }
    }, false);
  };

  /**
   * Set whether all the words should be selectable.
   * @public
   * @param {Boolean} selectable Set to true to make the words selectable.
   */
  MarkTheWords.prototype.setAllSelectable = function (selectable) {
    this.selectableWords.forEach(function (entry) {
      entry.setSelectable(selectable);
    });

  };

  /**
   * Mark the words as correct, wrong or missed.
   */
  MarkTheWords.prototype.setAllMarks = function () {
    this.selectableWords.forEach(function (entry) {
      entry.markCheck();
    });
    this.trigger('resize');
  };

  /**
   * Mark the selected words as correct or wrong.
   */
  MarkTheWords.prototype.feedbackSelectedWords = function () {
    this.selectableWords.forEach(function (entry) {
      if (entry.isSelected()) {
        entry.markCheck();
      }
    });
    this.trigger('resize');
  };

  /**
   * Evaluate task and display score text for word markings.
   *
   * @return {Boolean} Returns true if maxScore was achieved.
   */
  MarkTheWords.prototype.showEvaluation = function () {
    this.hideEvaluation();
    this.calculateScore();
    
    var score = ((this.correctAnswers - this.wrongAnswers) <= 0) ? 0 : (this.correctAnswers - this.wrongAnswers);
    //replace editor variables with values, uses regexp to replace all instances.
    var scoreText = this.params.score.replace(/@score/g, score.toString())
      .replace(/@total/g, this.answers.toString())
      .replace(/@correct/g, this.correctAnswers.toString())
      .replace(/@wrong/g, this.wrongAnswers.toString())
      .replace(/@missed/g, this.missedAnswers.toString());
      

    this.setFeedback(scoreText, score, this.answers);

    this.triggerXAPIScored(score, this.answers, 'answered');
    this.trigger('resize');
    return score === this.answers;
  };

  /**
   * Clear the evaluation text.
   */
  MarkTheWords.prototype.hideEvaluation = function () {
    this.setFeedback();
    this.trigger('resize');
  };

  /**
   * Calculate score and store them in class variables.
   */
  MarkTheWords.prototype.calculateScore = function () {
    var self = this;
    self.correctAnswers = 0;
    self.wrongAnswers = 0;
    self.missedAnswers = 0;
    self.selectableWords.forEach(function (entry) {
      if (entry.isCorrect()) {
        self.correctAnswers += 1;
      } else if (entry.isWrong()) {
        self.wrongAnswers += 1;
      } else if (entry.isMissed()) {
        self.missedAnswers += 1;
      }
    });
  };

  /**
   * Clear styling on marked words.
   */
  MarkTheWords.prototype.clearAllMarks = function () {
    this.selectableWords.forEach(function (entry) {
      entry.markClear();
    });
    this.trigger('resize');
  };

  /**
   * Needed for contracts.
   * Always returns true, since MTW has no required actions to give an answer. Also calculates score.
   *
   * @returns {Boolean} Always returns true.
   */
  MarkTheWords.prototype.getAnswerGiven = function () {
    return true;
  };

  /**
   * Needed for contracts.
   * Counts the score, which is correct answers subtracted by wrong answers.
   *
   * @returns {Number} score The amount of points achieved.
   */
  MarkTheWords.prototype.getScore = function () {
    this.calculateScore();
    return ((this.correctAnswers - this.wrongAnswers) <= 0) ? 0 : (this.correctAnswers - this.wrongAnswers);
  };

  /**
   * Needed for contracts.
   * Gets max score for this task.
   *
   * @returns {Number} maxScore The maximum amount of points achievable.
   */
  MarkTheWords.prototype.getMaxScore = function () {
    return this.answers;
  };

  /**
   * Get title
   * @returns {string}
   */
  MarkTheWords.prototype.getTitle = function () {
    return H5P.createTitle(this.params.taskDescription);
  };

  /**
   * Needed for contracts.
   * Display the evaluation of the task, with proper markings.
   */
  MarkTheWords.prototype.showSolutions = function () {
    this.showEvaluation();
    this.setAllMarks();
    this.hideAllButtons();
    this.setAllSelectable(false);
  };

  /**
   * Needed for contracts.
   * Resets the task back to its' initial state.
   */
  MarkTheWords.prototype.resetTask = function () {
    this.clearAllMarks();
    this.hideEvaluation();
    this.setAllSelectable(true);
    this.hideButton('try-again');
    this.hideButton('show-solution');
    this.showButton('check-answer');
  };

  /**
   * Hide all buttons. Used to disable further input for user.
   */
  MarkTheWords.prototype.hideAllButtons = function () {
    this.hideButton('try-again');
    this.hideButton('show-solution');
    this.hideButton('check-answer');
    this.trigger('resize');
  };

  /**
   * Returns an object containing the selected words
   *
   * @returns {object} containing indexes of selected words
   */
  MarkTheWords.prototype.getCurrentState = function () {
    var selectedWordsIndexes = [];
    if (this.selectableWords === undefined) {
      return undefined;
    }

    this.selectableWords.forEach(function (selectableWord, swIndex) {
      if (selectableWord.isSelected()) {
        selectedWordsIndexes.push(swIndex);
      }
    });
    return selectedWordsIndexes;
  };

  /**
   * Sets answers to current user state
   */
  MarkTheWords.prototype.setH5PUserState = function () {
    var self = this;

    // Do nothing if user state is undefined
    if (this.previousState === undefined || this.previousState.length === undefined) {
      return;
    }
    // Select words from user state
    this.previousState.forEach(function (answeredWordIndex) {
      if (isNaN(answeredWordIndex) || answeredWordIndex >= self.selectableWords.length || answeredWordIndex < 0) {
        throw new Error('Stored user state is invalid');
      }
      self.selectableWords[answeredWordIndex].toggleMark();
    });

	//h5pcustomize
 	if(self.params.behaviour.enableRetry == false && this.previousState.length > 0) //>0 means attempted
 	{   
		self.setAllSelectable(false);
    }
  
    
  };

  MarkTheWords.prototype.registerDomElements = function () {

    // Register description
    this.setIntroduction(this.params.taskDescription);

    // Register content
    this.setContent(this.$inner, {
      'class': MAIN_CONTAINER
    });

    // Register buttons
    this.addButtons();
  };

  /**
   * Private class for keeping track of selectable words.
   *
   * @class
   * @param {H5P.jQuery} $word
   */
  function Word($word) {
    var self = this;
    H5P.EventDispatcher.call(self);

    var input = $word.text();
    var handledInput = input;

    // Check if word is an answer
    var isAnswer = checkForAnswer();

    // Remove single asterisk and escape double asterisks.
    handleAsterisks();

    var isSelectable = true;
    var isSelected = false;

    if (isAnswer) {
      $word.text(handledInput);
    }

    // Handle click events
    $word.click(function () {
      if (!isSelectable) {
        return;
      }
      self.toggleMark();
    });

    /**
     * Checks if the word is an answer by checking the first, second to last and last character of the word.
     * @private
     * @return {Boolean} Returns true if the word is an answer.
     */
    function checkForAnswer() {
      // Check last and next to last character, in case of punctuations.
      var wordString = removeDoubleAsterisks(input);
      if (wordString.charAt(0) === ('*') && wordString.length > 2) {
        if (wordString.charAt(wordString.length - 1) === ('*')) {
          handledInput = input.slice(1, input.length - 1);
          return true;
        }
        // If punctuation, add the punctuation to the end of the word.
        else if(wordString.charAt(wordString.length - 2) === ('*')) {
          handledInput = input.slice(1, input.length - 2);
          return true;
        }
        return false;
      }
      return false;
    }

    /**
     * Removes double asterisks from string, used to handle input.
     * @private
     * @param {String} wordString The string which will be handled.
     * @results {String} slicedWord Returns a string without double asterisks.
     */
    function removeDoubleAsterisks(wordString) {
      var asteriskIndex = wordString.indexOf('*');
      var slicedWord = wordString;
      while (asteriskIndex !== -1) {
        if (wordString.indexOf('*', asteriskIndex + 1) === asteriskIndex + 1) {
          slicedWord = wordString.slice(0, asteriskIndex) + wordString.slice(asteriskIndex + 2, input.length);
        }
        asteriskIndex = wordString.indexOf('*', asteriskIndex + 1);
      }
      return slicedWord;
    }

    /**
     * Escape double asterisks ** = *, and remove single asterisk.
     * @private
     */
    function handleAsterisks() {
      var asteriskIndex = handledInput.indexOf('*');
      while (asteriskIndex !== -1) {
        handledInput = handledInput.slice(0, asteriskIndex) + handledInput.slice(asteriskIndex + 1, handledInput.length);
        asteriskIndex = handledInput.indexOf('*', asteriskIndex + 1);
      }
    }

    /**
     * Toggle the marking of a word.
     * @public
     */
    this.toggleMark = function () {
      self.triggerXAPI('interacted');
      $word.toggleClass(SELECTED_MARK);
      isSelected = !isSelected;
    };

    /**
     * Clears all marks from the word.
     * @public
     */
    this.markClear = function () {
      $word.removeClass(MISSED_MARK)
        .removeClass(CORRECT_MARK)
        .removeClass(WRONG_MARK)
        .removeClass(SELECTED_MARK);
      isSelected = false;
    };

    /**
     * Sets correct styling if word is an answer.
     * @public
     */
    this.showSolution = function () {
      $word.removeClass(MISSED_MARK)
        .removeClass(CORRECT_MARK)
        .removeClass(WRONG_MARK)
        .removeClass(SELECTED_MARK);
      if (isAnswer) {
        $word.addClass(CORRECT_MARK);
      }
    };

    /**
     * Check if the word is correctly marked and style it accordingly.
     * @public
     */
    this.markCheck = function () {
      if (isSelected) {
        if (isAnswer) {
          $word.addClass(CORRECT_MARK);
        } else {
          $word.addClass(WRONG_MARK);
        }
      } else if (isAnswer) {
        $word.addClass(MISSED_MARK);
      }
      $word.removeClass(SELECTED_MARK);
    };

    /**
     * Set whether the word should be selectable, and proper feedback.
     * @public
     * @param {Boolean} selectable Set to true to make word selectable.
     */
    this.setSelectable = function (selectable) {
      isSelectable = selectable;
      //Toggle feedback class
      if (selectable) {
        $word.removeClass(WORD_DISABLED);
      } else {
        $word.addClass(WORD_DISABLED);
      }
    };

    /**
     * Checks if the word is marked correctly.
     * @public
     * @returns {Boolean} True if the marking is correct.
     */
    this.isCorrect = function () {
      return (isAnswer && isSelected);
    };

    /**
     * Checks if the word is marked wrong.
     * @public
     * @returns {Boolean} True if the marking is wrong.
     */
    this.isWrong = function () {
      return (!isAnswer && isSelected);
    };

    /**
     * Checks if the word is correct, but has not been marked.
     * @public
     * @returns {Boolean} True if the marking is missed.
     */
    this.isMissed = function () {
      return (isAnswer && !isSelected);
    };

    /**
     * Checks if the word is an answer.
     * @public
     * @returns {Boolean} True if the word is an answer.
     */
    this.isAnswer = function () {
      return isAnswer;
    };

    /**
     * Checks if the word is selected.
     * @public
     * @returns {Boolean} True if the word is selected.
     */
    this.isSelected = function () {
      return isSelected;
    };
  }
  Word.prototype = Object.create(H5P.EventDispatcher.prototype);
  Word.prototype.constructor = Word;

  return MarkTheWords;
}(H5P.jQuery, H5P.Question));
;var H5P = H5P || {};

/**
 * Constructor.
 *
 * @param {object} params Start paramteres.
 * @param {int} id Content identifier
 * @param {function} editor
 *  Set if an editor is initiating this library
 * @returns {undefined} Nothing.
 */
H5P.CoursePresentation = function (params, id, extras) {
  H5P.EventDispatcher.call(this);
  var that = this;
  this.presentation = params.presentation;
  this.slides = this.presentation.slides;
  this.contentId = id;
  this.currentSlideIndex = 0;
  this.elementInstances = []; // elementInstances holds the instances for elements in an array.
  this.elementsAttached = []; // Map to keep track of which slide has attached elements
  this.slidesWithSolutions = [];
  this.hasAnswerElements = false;
  this.ignoreResize = false;

  //load lang files h5pcustomize
  try
  {
  var browser = checkBrowser();
  if(window.parent.Drupal.settings.user.language !== undefined && H5P.jQuery.inArray(window.parent.Drupal.settings.user.language, ["ru"]) != -1) {
  	loadCssFilesH5p('/sites/all/themes/core/expertusoneV2/expertusone-internals/lang-css/ru/lang_h5p_ru.css');
  }
  else if(window.parent.Drupal.settings.user.language !== undefined && H5P.jQuery.inArray(window.parent.Drupal.settings.user.language, ["it"]) != -1) {
	  	loadCssFilesH5p('/sites/all/themes/core/expertusoneV2/expertusone-internals/lang-css/it/lang_h5p_it.css');
	  }
  else if(window.parent.Drupal.settings.user.language !== undefined && H5P.jQuery.inArray(window.parent.Drupal.settings.user.language, ["ja"]) != -1) {
	  	loadCssFilesH5p('/sites/all/themes/core/expertusoneV2/expertusone-internals/lang-css/ja/lang_h5p_ja.css');
	  }
  else if(window.parent.Drupal.settings.user.language !== undefined && H5P.jQuery.inArray(window.parent.Drupal.settings.user.language, ["pt-pt"]) != -1) {
	  	loadCssFilesH5p('/sites/all/themes/core/expertusoneV2/expertusone-internals/lang-css/pt-pt/lang_h5p_pt-pt.css');
	  }
  else if(window.parent.Drupal.settings.user.language !== undefined && H5P.jQuery.inArray(window.parent.Drupal.settings.user.language, ["fr"]) != -1) {
	  	loadCssFilesH5p('/sites/all/themes/core/expertusoneV2/expertusone-internals/lang-css/fr/lang_h5p_fr.css');
	  }
  else if(window.parent.Drupal.settings.user.language !== undefined && H5P.jQuery.inArray(window.parent.Drupal.settings.user.language, ["zh-hans"]) != -1) {
	  	loadCssFilesH5p('/sites/all/themes/core/expertusoneV2/expertusone-internals/lang-css/zh-hans/lang_h5p_zh-hans.css');
	  }
  else if(window.parent.Drupal.settings.user.language !== undefined && H5P.jQuery.inArray(window.parent.Drupal.settings.user.language, ["de"]) != -1) {
	  	loadCssFilesH5p('/sites/all/themes/core/expertusoneV2/expertusone-internals/lang-css/de/lang_h5p_de.css');
	  }
  else if(window.parent.Drupal.settings.user.language !== undefined && H5P.jQuery.inArray(window.parent.Drupal.settings.user.language, ["es"]) != -1) {
	  	loadCssFilesH5p('/sites/all/themes/core/expertusoneV2/expertusone-internals/lang-css/es/lang_h5p_es.css');
	  }
  else if(window.parent.Drupal.settings.user.language !== undefined && H5P.jQuery.inArray(window.parent.Drupal.settings.user.language, ["ko"]) != -1) {
	  	loadCssFilesH5p('/sites/all/themes/core/expertusoneV2/expertusone-internals/lang-css/ko/lang_h5p_ko.css');
	  }
  if(browser == "MSIE" && window.parent.Drupal.settings.user.language !== undefined && H5P.jQuery.inArray(window.parent.Drupal.settings.user.language, ["ru"]) != -1)
  {
  	loadCssFilesH5p('/sites/all/themes/core/expertusoneV2/expertusone-internals/lang-css/ru/ie11_h5p_ru.css');
  }else if(browser == "MSIE" && window.parent.Drupal.settings.user.language !== undefined && H5P.jQuery.inArray(window.parent.Drupal.settings.user.language, ["fr"]) != -1)
  {
	  	loadCssFilesH5p('/sites/all/themes/core/expertusoneV2/expertusone-internals/lang-css/fr/ie11_h5p_fr.css');
  }
  }catch(e){
  }
  
  
  if (extras.cpEditor) {
    this.editor = extras.cpEditor;
  }
  
 

  if (extras) {
    this.previousState = extras.previousState;
  }
  
  this.presentation.keywordListEnabled = (params.presentation.keywordListEnabled === undefined ? true : params.presentation.keywordListEnabled);

  this.l10n = H5P.jQuery.extend({
    slide: window.parent.Drupal.t('Slide'),
    yourScore: 'Your score',
    maxScore: 'Max score',
    goodScore: 'Congratulations! You got @percent correct!',
    okScore: 'Nice effort! You got @percent correct!',
    badScore: 'You need to work more on this. You only got @percent correct...',
    total: 'TOTAL',
    showSolutions: window.parent.Drupal.t('LBL3099'), //'Show solutions',
    summary: 'summary',
    retry: 'Retry',
    exportAnswers: 'Export text',
    close: 'Close',
    hideKeywords: 'Hide keywords list',
    showKeywords: 'Show keywords list',
    fullscreen: 'Fullscreen',
    exitFullscreen: 'Exit fullscreen',
    prevSlide: window.parent.Drupal.t("LBL692")+" "+window.parent.Drupal.t("Slide") , //'Previous slide',
    nextSlide:window.parent.Drupal.t("LBL693")+" "+window.parent.Drupal.t("Slide") ,// 'Next slide',
    currentSlide: window.parent.Drupal.t("Current")+" "+window.parent.Drupal.t("Slide"), //'Current slide',
    lastSlide: window.parent.Drupal.t("Last")+" "+window.parent.Drupal.t("Slide"), //'Last slide',
    solutionModeTitle: 'Exit solution mode',
    solutionModeText: 'Solution Mode',
    summaryMultipleTaskText: 'Multiple tasks',
    scoreMessage: 'You achieved:',
    shareFacebook: 'Share on Facebook',
    shareTwitter: 'Share on Twitter',
    goToSlide: 'Go to slide :num',
    solutionsButtonTitle: 'Show comments',
    printTitle: 'Print',
    printIngress: 'How would you like to print this presentation?',
    printAllSlides: 'Print all slides',
    printCurrentSlide: 'Print current slide'
    //h5pcustomize - do not read label from DB. Use the label which defined here.
  //}, params.l10n !== undefined ? params.l10n : {});
    },{});

  if (!!params.override) {
    this.activeSurface = !!params.override.activeSurface;
    this.overrideButtons = !!params.override.overrideButtons;
    this.overrideShowSolutionsButton = !!params.override.overrideShowSolutionButton;
    this.overrideRetry = !!params.override.overrideRetry;
    this.hideSummarySlide = !!params.override.hideSummarySlide;
  }
  this.on('resize', this.resize, this);

  this.on('printing', function (event) {
    that.ignoreResize = !event.data.finished;

    if (event.data.finished) {
      that.resize();
    }
    else if (event.data.allSlides) {
      that.attachAllElements();
    }
  });
};

H5P.CoursePresentation.prototype = Object.create(H5P.EventDispatcher.prototype);
H5P.CoursePresentation.prototype.constructor = H5P.CoursePresentation;

/**
 * @public
 */
H5P.CoursePresentation.prototype.getCurrentState = function () {
  var state = this.previousState ? this.previousState : {};
  state.progress = this.$current.index();
  if (!state.answers) {
    state.answers = [];
  }
  if (!state.answered) {
    state.answered = [];
  }

  // Get answers and answered
  for (var slide = 0; slide < this.elementInstances.length; slide++) {
    if (this.progressbarParts) {
      state.answered[slide] = this.progressbarParts[slide].children('.h5p-progressbar-part-has-task').hasClass('h5p-answered');
    }
    if (this.elementInstances[slide]) {
      for (var element = 0; element < this.elementInstances[slide].length; element++) {
        var instance = this.elementInstances[slide][element];
        if (instance.getCurrentState instanceof Function ||
            typeof instance.getCurrentState === 'function') {
          if (!state.answers[slide]) {
            state.answers[slide] = [];
          }
          state.answers[slide][element] = instance.getCurrentState();
        }
      }
    }
  }

  return state;
};

/**
 * Render the presentation inside the given container.
 *
 * @param {H5P.jQuery} $container Container for this presentation.
 * @returns {undefined} Nothing.
 */
H5P.CoursePresentation.prototype.attach = function ($container) {
  var that = this;

  // isRoot is undefined in the editor
  if (this.isRoot !== undefined && this.isRoot()) {
    this.setActivityStarted();
  }

  var html =
          '<div class="h5p-wrapper" tabindex="0" style="height:489px">' +
          '  <div class="h5p-box-wrapper">' +
          '    <div class="h5p-presentation-wrapper">' +
          '      <div class="h5p-keywords-wrapper"></div>' +
          '      <div class="h5p-slides-wrapper"></div>' +
          '    </div>' +
          '  </div>' +
          '  <div class="h5p-progressbar"></div>' +
          '  <div class="h5p-footer"></div>' +
          '</div>';

  $container.addClass('h5p-course-presentation').html(html);

  //Detect ie version
  var ie = (function () {
    var undef;
    var v = 3;
    var div = document.createElement('div');
    var all = div.getElementsByTagName('i');

    while (
      div.innerHTML = '<!--[if gt IE ' + (++v) + ']><i></i>< ![endif]-->',
        all[0]
      );

    return v > 4 ? v : undef;

  }());

  if (ie <= 9) {
    $container.addClass('old-ie-browser');
  }

  if (window.navigator.userAgent.indexOf('MSIE 8.0') !== -1) {
    $container.find('.h5p-box-wrapper').css({
      border: '1px solid #a9a9a9',
      boxSizing: 'border-box'
    });
  }

  this.$container = $container;
  this.$wrapper = $container.children('.h5p-wrapper').focus(function () {
    that.initKeyEvents();
  }).blur(function () {
    H5P.jQuery('body').unbind('keydown', that.keydown);
    delete that.keydown;
  }).click(function (event) {
    var $target = H5P.jQuery(event.target);
    if (!$target.is('input, textarea') && !that.editor) {
      // Add focus to the wrapper so that it may capture keyboard events
      that.$wrapper.focus();
    }

    if (that.keywordsClicked) {
      that.keywordsClicked = false;
    }
    else if (that.presentation.keywordListEnabled &&
            !that.presentation.keywordListAlwaysShow &&
            that.presentation.keywordListAutoHide) {
      that.hideKeywords();
    }
  });

  // Get intended base width from CSS.
  this.width = parseInt(this.$wrapper.css('width'));
 // this.height = parseInt(this.$wrapper.css('height'));
   this.height = parseInt(this.$wrapper.css('489px'));

  this.ratio = 16/9;
  // Intended base font size cannot be read from CSS, as it might be modified
  // by mobile browsers already. (The Android native browser does this.)
  this.fontSize = 16;

  this.$boxWrapper = this.$wrapper.children('.h5p-box-wrapper');
  var $presentationWrapper = this.$boxWrapper.children('.h5p-presentation-wrapper');
  this.$slidesWrapper = $presentationWrapper.children('.h5p-slides-wrapper');
  this.$keywordsWrapper = $presentationWrapper.children('.h5p-keywords-wrapper');
  this.$progressbar = this.$wrapper.children('.h5p-progressbar');
  this.$footer = this.$wrapper.children('.h5p-footer');

  this.initKeywords = (this.presentation.keywordListEnabled === undefined || this.presentation.keywordListEnabled === true || this.editor !== undefined);
  if (this.activeSurface && this.editor === undefined) {
    this.initKeywords = false;
    this.$boxWrapper.css('height', '100%');
  }
  this.isSolutionMode = false;

  // Create keywords html
  var keywords = '';
  var foundKeywords = false;
  var first, slide, $slide;	
  for (var i = 0; i < this.slides.length; i++) {
    slide = this.slides[i];
    $slide = H5P.jQuery(H5P.CoursePresentation.createSlide(slide)).appendTo(this.$slidesWrapper);
    first = i === 0;

    if (first) {
      this.$current = $slide.addClass('h5p-current');
    }

	//h5pcustomize - load interactions realted js files
	//console.log("test3");
	
  if(H5P.jQuery(".h5p-coursepresentation-editor").size() > 0)
  {
  /*	if(H5P.jQuery("#lazyloadinteractions_script").size()  == 0)
  	{
  		var script=document.createElement('script');
  		script.id = "lazyloadinteractions_script";
  		script.type='text/javascript';
  		script.src="/h5pmerge/lazyloadpresentationscripts.js";
  		H5P.jQuery("body").append(script);
  	}*/
  }
  
    this.addElements(slide, $slide, i);

    if (!foundKeywords && slide.keywords !== undefined && slide.keywords.length) {
      foundKeywords = true;
    }
    if (this.initKeywords) {
      keywords += this.keywordsHtml(slide.keywords, first);
    }
  }

  // Determine if summary slide should be added
  var $summarySlide;
  this.showSummarySlide = false;

  if (this.hideSummarySlide) {
    this.showSummarySlide = !this.hideSummarySlide;
  } else {
    // Check for task
    this.slidesWithSolutions.forEach(function (slide) {
      that.showSummarySlide = slide.length;
    });
  }

  var summarySlideData = [];
  if ((this.editor === undefined) && (this.showSummarySlide || this.hasAnswerElements)) {
    summarySlideData = {
      elements: [],
      keywords: []
    };
    this.slides.push(summarySlideData);

    slide = this.slides[this.slides.length - 1];
    $slide = H5P.jQuery(H5P.CoursePresentation.createSlide(slide)).appendTo(this.$slidesWrapper);

    this.addElements(slide, $slide, i);

    if (!foundKeywords && slide.keywords !== undefined && slide.keywords.length) {
      foundKeywords = true;
    }
    if (this.initKeywords) {
      keywords += this.keywordsHtml(slide.keywords, first);
    }

    $slide.addClass('h5p-summary-slide');
    $summarySlide = H5P.jQuery('.h5p-summary-slide');
  }

  if (!foundKeywords && this.editor === undefined) {
    this.initKeywords = false; // Do not show keywords pane if it's empty!
  }

  // Initialize keywords
  if (this.initKeywords) {
    this.initKeywordsList(keywords);
    if (this.presentation.keywordListAlwaysShow) {
      this.showKeywords();
    }
  }
  else {
    this.$keywordsWrapper.remove();
  }

  if (this.editor !== undefined || !this.activeSurface) {
    // Initialize touch events
    this.initTouchEvents();

    // init navigation line
    this.navigationLine = new H5P.CoursePresentation.NavigationLine(this);

    this.summarySlideObject = new H5P.CoursePresentation.SummarySlide(this, $summarySlide);
  }
  else {
    this.$progressbar.add(this.$footer).remove();

    if (H5P.canHasFullScreen) {
      // Create full screen button
      this.$fullScreenButton = H5P.jQuery('<div/>', {
        'class': 'h5p-toggle-full-screen',
        title: this.l10n.fullscreen,
        role: 'button',
        tabindex: 0,
        on: {
          click: function () {
            that.toggleFullScreen();
          },
          keypress: function (event) {
            // Buttons must respond to space bar
            if (event.which === 32) {
              that.toggleFullScreen();
            }
          }
        },
        appendTo: this.$wrapper
      });
    }
  }

  new H5P.CoursePresentation.SlideBackground(this);
  if (this.previousState && this.previousState.progress) {
    this.jumpToSlide(this.previousState.progress);
  }
//added for h5pcustomization #0084699
  //fadeoutArrows("slideFirst");slideNew
  fadeoutArrows("slideNew");
  
  //h5pcustomize - sometimes not remove loader icon. added timeout
  window.setTimeout(function(){
	  console.log("loaader remove before:");
	  console.log("loader size:"+window.parent.$("#loader").size());
	  window.parent.$("#loader").remove();
	  	console.log("loaader remove after:");
  },10);
  
};

/**
 * Updates the feedback icons for the progres bar.
 *
 * @param slideScores
 */
H5P.CoursePresentation.prototype.setProgressBarFeedback = function (slideScores) {
  var that = this;

  if (slideScores !== undefined && slideScores) {
    // Set feedback icons for progress bar.
    slideScores.forEach(function (singleSlide) {
      if (that.progressbarParts[singleSlide.slide-1].children('.h5p-progressbar-part-has-task').hasClass('h5p-answered')) {
        if (singleSlide.score >= singleSlide.maxScore) {
          that.progressbarParts[singleSlide.slide-1]
            .children('.h5p-progressbar-part-has-task')
            .addClass('h5p-is-correct');
        } else {
          that.progressbarParts[singleSlide.slide-1]
            .children('.h5p-progressbar-part-has-task')
            .addClass('h5p-is-wrong');
        }
      }
    });
  } else {
    // Remove all feedback icons.
    that.progressbarParts.forEach(function (pbPart) {
      pbPart.children('.h5p-progressbar-part-has-task').removeClass('h5p-is-correct');
      pbPart.children('.h5p-progressbar-part-has-task').removeClass('h5p-is-wrong');
    });
  }
};

/**
 * Toggle keywords list on/off depending on current state
 */
H5P.CoursePresentation.prototype.toggleKeywords = function () {
  // Check state of keywords
  if (this.$keywordsWrapper.hasClass('h5p-open')) {
    // Already open, remove keywords
    this.hideKeywords();
  }
  else {
    // Open keywords
    this.showKeywords();
  }
};

/**
 * Hide keywords
 */
H5P.CoursePresentation.prototype.hideKeywords = function () {
  if (this.$keywordsButton !== undefined) {
    this.$keywordsButton.attr('title', this.l10n.showKeywords);
  }
  this.$keywordsWrapper.add(this.$keywordsButton).removeClass('h5p-open');
};

/**
 * Show keywords
 */
H5P.CoursePresentation.prototype.showKeywords = function () {
  if (this.$keywordsButton !== undefined) {
    this.$keywordsButton.attr('title', this.l10n.hideKeywords);
  }
  this.$keywordsWrapper.add(this.$keywordsButton).addClass('h5p-open');
};

/**
 * Change the background opacity of the keywords list.
 *
 * @param {Number} value 0 - 100
 */
H5P.CoursePresentation.prototype.setKeywordsOpacity = function (value) {
  var self = this;
  var color = self.$keywordsWrapper.css('background-color').split(/\(|\)|,/g);
  self.$keywordsWrapper.css('background-color', 'rgba(' + color[1] + ', ' + color[2] + ', ' + color[3] + ',' + (value / 100) + ')');
};

/**
 * Makes continuous text smaller if it does not fit inside its container.
 * Only works in view mode.
 *
 * @returns {undefined}
 */
H5P.CoursePresentation.prototype.fitCT = function () {
  if (this.editor !== undefined) {
    return;
  }

  this.$current.find('.h5p-ct').each(function () {
    var percent = 100;
    var $ct = H5P.jQuery(this);
    var parentHeight = $ct.parent().height();
    while ($ct.outerHeight() > parentHeight) {
      percent--;
      $ct.css({
        fontSize: percent + '%',
        lineHeight: (percent + 65) + '%'
      });

      if (percent < 0) {
        break; // Just in case.
      }
    }
  });
};

/**
 * Resize handling.
 *
 * @param {Boolean} fullscreen
 * @returns {undefined}
 */
H5P.CoursePresentation.prototype.resize = function () {
  var fullscreenOn = H5P.$body.hasClass('h5p-fullscreen') || H5P.$body.hasClass('h5p-semi-fullscreen');

  if (this.ignoreResize) {
    return; // When printing.
  }

  // Fill up all available width
  this.$wrapper.css('width', 'auto');
  var width = this.$container.width();
  var style = {};

  if (fullscreenOn) {
    var maxHeight = this.$container.height();
    if (width / maxHeight > this.ratio) {
      // Top and bottom would be cut off so scale down.
      width = maxHeight * this.ratio;
      style.width = width + 'px';
    }
  }

  // TODO: Add support for -16 when content conversion script is created?
  var widthRatio = width / this.width;
  //alert("customizing the height for presentation in CP")
 // style.height = (width / this.ratio) + 'px';
  style.fontSize = (this.fontSize * widthRatio) + 'px';

  if (this.editor !== undefined) {
    this.editor.setContainerEm(this.fontSize * widthRatio * 0.75);
  }

  this.$wrapper.css(style);

  this.swipeThreshold = widthRatio * 100; // Default swipe threshold is 50px.

  // Resize elements
  var instances = this.elementInstances[this.$current.index()];
  if (instances !== undefined) {
    var slideElements = this.slides[this.$current.index()].elements;
    for (var i = 0; i < instances.length; i++) {
      var instance = instances[i];
      if ((instance.preventResize === undefined || instance.preventResize === false) && instance.$ !== undefined && !slideElements[i].displayAsButton) {
        H5P.trigger(instance, 'resize');
      }
    }
  }

  this.fitCT();
  if(this.editor != undefined && this.editor.addSlideInProgress == true)
	  this.editor.addSlideInProgress = false;  
};

/**
 * Enter/exit full screen mode.
 */
H5P.CoursePresentation.prototype.toggleFullScreen = function () {
  if (H5P.isFullscreen || this.$container.hasClass('h5p-fullscreen') || this.$container.hasClass('h5p-semi-fullscreen')) {
    // Downscale fullscreen font size
    this.$footer.removeClass('footer-full-screen');
    this.$fullScreenButton.attr('title', this.l10n.fullscreen);

    // Cancel fullscreen
    if (H5P.exitFullScreen !== undefined && H5P.fullScreenBrowserPrefix !== undefined) {
      H5P.exitFullScreen();
    } else {
      // Use old system
      if (H5P.fullScreenBrowserPrefix === undefined) {
        // Click button to disable fullscreen
        H5P.jQuery('.h5p-disable-fullscreen').click();
      }
      else {
        if (H5P.fullScreenBrowserPrefix === '') {
          window.top.document.exitFullScreen();
        }
        else if (H5P.fullScreenBrowserPrefix === 'ms') {
          window.top.document.msExitFullscreen();
        }
        else {
          window.top.document[H5P.fullScreenBrowserPrefix + 'CancelFullScreen']();
        }
      }
    }
  }
  else {
    // Rescale footer buttons
    this.$footer.addClass('footer-full-screen');

    this.$fullScreenButton.attr('title', this.l10n.exitFullscreen);
    H5P.fullScreen(this.$container, this);
    if (H5P.fullScreenBrowserPrefix === undefined) {
      // Hide disable full screen button. We have our own!
      H5P.jQuery('.h5p-disable-fullscreen').hide();
    }
  }
};

/**
 * Set focus.
 */
H5P.CoursePresentation.prototype.focus = function () {
  this.$wrapper.focus();
};

/**
 *
 * @param {jQuery} $keyword
 * @returns {undefined}
 */
H5P.CoursePresentation.prototype.keywordClick = function ($keyword) {
  if ($keyword.hasClass('h5p-current')) {
    return;
  }

  if (this.presentation.keywordListEnabled &&
      !this.presentation.keywordListAlwaysShow &&
      this.presentation.keywordListAutoHide) {
    // Auto-hide keywords list
    this.hideKeywords();
  }

  this.jumpToSlide($keyword.index());
};



/**
 * Add all element to the given slide.
 *
 * @param {Object} slide
 * @param {jQuery} $slide
 * @param {Number} index
 */
H5P.CoursePresentation.prototype.addElements = function (slide, $slide, index) {
  if (slide.elements === undefined) {
    return;
  }
  this.addElementsASync(slide, $slide, index);

};

H5P.CoursePresentation.prototype.addElementsASync = function (slide, $slide, index) {
  console.log("addElementsASync11111.... ");
  var attach = ( index === 0 || index === 1);
  if(H5P.recordCloned)
	  attach = (this.editor !== undefined || index === 0 || index === 1);
  for (var i = 0; i < slide.elements.length; i++) {
    var element = slide.elements[i];
    var instance = this.addElement(element, $slide, index);
    if (attach) {
      // The editor requires all fields to be attached/rendered right away
      
      this.attachElement(element, instance, $slide, index);
      
           	
    }
    //h5pcustomize - if other than first slide media autoplay should be stopped #75326
    console.log("addElementsASync11111abhi.... " , instance );
      if(index != 0)
    {
		this.pauseMedia(instance);
	}
      else
    	  {
    	
    	// if( !(instance instanceof H5P.CoursePresentation.GoToSlide))// && instance.params.autoplay != undefined )
    	  if(!(instance instanceof H5P.CoursePresentation.GoToSlide) && (instance.libraryInfo.machineName == "H5P.Audio" || instance.libraryInfo.machineName == "H5P.Video"))
    		 {
    	        if(  instance != undefined && instance.params != undefined && instance.params.autoplay )
    	        		this.playMedia(instance);
    		 }
    	  }
  }

  
  	H5P.recordCloned = false;
    //h5pcustomize
//	H5P.jQuery(".h5p-go-to-slide").parent().parent().css("height","auto");


	

  if (attach) {
    this.elementsAttached[index] = true;
    this.trigger('domChanged', {
      '$target': $slide,
      'library': 'H5P.CoursePresentation',
      'key': 'newSlide'
    }, {'bubbles': true, 'external': true});
  }
  /*
  //console.log("this.previousState:",this.previousState,":index:",index,":this.slides.length:",this.slides.length);
  console.log("this.previousState::",this.previousState);
  if (this.previousState!=undefined && this.previousState.progress != undefined){
    if(!isNaN(this.previousState.progress))
  	  if ((this.previousState.progress+1) == index || ( this.slides.length == (index+1) && this.slides.length == (this.previousState.progress+1) ) ) {
    	this.jumpToSlide(this.previousState.progress);
  	}
  }*/
  
};

/**
 * Add element to the given slide and stores elements with solutions.
 *
 * @param {Object} element The Element to add.
 * @param {jQuery} $slide Optional, the slide. Defaults to current.
 * @param {Number} index Optional, the index of the slide we're adding elements to.
 * @returns {unresolved}
 */
H5P.CoursePresentation.prototype.addElement = function (element, $slide, index) {
  var instance;
  if (element.action === undefined) {
    // goToSlide, internal element
    instance = new H5P.CoursePresentation.GoToSlide(element.title, element.goToSlide, element.invisible, this);
  }
  else {
    // H5P library
    var defaults;
    if (this.overrideButtons) {
      defaults = {
        params: {
          behaviour: {
            enableSolutionsButton: this.overrideShowSolutionsButton,
            enableRetry: this.overrideRetry
          }
        }
      };
    }
    else {
      defaults = {
        params: {
        }
      };
    }

    var library;
    if (this.editor !== undefined) {


      // Clone the whole tree to avoid libraries accidentally changing params while running.
      library = H5P.jQuery.extend(true, {}, element.action, defaults);
 console.log("library check : ",library);
    }
    else {
      // Add defaults
      library = H5P.jQuery.extend(true, element.action, defaults);
    }

    /* If library allows autoplay, control this from CP */
    if (library.params.autoplay) {
      library.params.autoplay = false;
      library.params.cpAutoplay = true;
    }
    else if (library.params.media &&
      library.params.media.params &&
      library.params.media.params.autoplay) {
      // Control libraries that has content with autoplay through CP
      library.params.media.params.autoplay = false;
      library.params.cpAutoplay = true;
    }

    var internalSlideId = this.elementInstances[index] ? this.elementInstances[index].length : 0;
    if (this.previousState && this.previousState.answers && this.previousState.answers[index] && this.previousState.answers[index][internalSlideId]) {
      // Restore previous state
      library.userDatas = {
        state: this.previousState.answers[index][internalSlideId]
      };
    }

    instance = H5P.newRunnable(library, this.contentId, undefined, undefined, {parent: this});
    if (instance.preventResize !== undefined) {
      instance.preventResize = true;
    }
  }

  if (this.elementInstances[index] === undefined) {
    this.elementInstances[index] = [instance];
  }
  else {
    this.elementInstances[index].push(instance);
  }

  if (this.checkForSolutions(instance)) {
    instance.coursePresentationIndexOnSlide = this.elementInstances[index].length - 1;
    if (this.slidesWithSolutions[index] === undefined) {
      this.slidesWithSolutions[index] = [];
    }
    this.slidesWithSolutions[index].push(instance);
  }

  //Check if it is a exportable text area
  if (instance.exportAnswers !== undefined && instance.exportAnswers) {
    this.hasAnswerElements = true;
  }

  return instance;
};

/**
 * Attach all element instances to slide.
 *
 * @param {jQuery} $slide
 * @param {Number} index
 */
H5P.CoursePresentation.prototype.attachElements = function ($slide, index) {
  if (this.elementsAttached[index] !== undefined) {
    return; // Already attached
  }

  var slide = this.slides[index];
  var instances = this.elementInstances[index];
  if (slide.elements !== undefined) {
    for (var i = 0; i < slide.elements.length; i++) {
      this.attachElement(slide.elements[i], instances[i], $slide, index);
    }
  }
  this.trigger('domChanged', {
      '$target': $slide,
      'library': 'H5P.CoursePresentation',
      'key': 'newSlide'
    }, {'bubbles': true, 'external': true});

  this.elementsAttached[index] = true;
};

/**
 * Attach element to slide container.
 *
 * @param {Object} element
 * @param {Object} instance
 * @param {jQuery} $slide
 * @param {Number} index
 * @returns {jQuery}
 */
H5P.CoursePresentation.prototype.attachElement = function (element, instance, $slide, index) {
  var that = this;
  var newWidth ='',newHeight='';
  var displayAsButton = (element.displayAsButton !== undefined && element.displayAsButton);
	
	var displayEmptyBox = "display:block";
	var $elementContainer;
	//do it only for editor h5pcustomize
	if(window.parent.$(".h5p-editor-iframe").size() > 0 && element.isEdit == undefined)
	{
		displayEmptyBox = "display:none";
		$elementContainer = H5P.jQuery('<div class="h5p-element' + (displayAsButton ? ' h5p-element-button-wrapper' : '') + '" style="'+displayEmptyBox+';left: ' + element.x + '%; top: ' + element.y + '%; width: ' + element.width + '%; height: ' + element.height + '%;"></div>').appendTo($slide);
	}
	else if(window.parent.$(".h5p-editor-iframe").size() > 0 && element.isEdit == false)
	{
		$elementContainer = H5P.jQuery('<div class="h5p-element' + (displayAsButton ? ' h5p-element-button-wrapper' : '') + '" style="'+displayEmptyBox+';left: ' + element.x + '%; top: ' + element.y + '%; width: ' + element.width + '%; height: ' + element.height + '%;"></div>').appendTo($slide);
	}
	else
	{
		//h5pcustomize
		var playerWidth = parseInt( H5P.jQuery(".h5p-slide").width(),10);
		var playerHeight = parseInt( H5P.jQuery(".h5p-slide").height(),10);
		if( element.action != undefined)
		{
			if(element.action.library == "H5P.Image 1.0" && element.action.params.resize != undefined && element.action.params.resize != true ) //use same height and width for image when specified in editor
			{
			//	alert(JSON.stringify(element));
				/*if(element.x > 55)
					element.x = element.x + 10;*/
		
					newWidth = element.action.params.file.widthPX+"px";
					newHeight = element.action.params.file.heightPX+"px";
				
				//			alert("newWidth::"+newWidth+"newHeight::"+newHeight);
			}
			/*else if(element.action.library == "H5P.MarkTheWords 1.5")
			{
			var excess_difference = element.action.params.excess_difference;
				//alert("excess:"+excess_difference);
			//alert("total_height:"+total_height+ "title_height:"+title_height+ "body_height:"+body_height+"button_height:"+button_height);
			if(excess_difference > 22){
				alert("excess11:"+excess_difference);
				
				newHeight = (element.height-(excess_difference - 22))+"%";
			}
			else{
				newHeight = element.height+"%";
			}
				newWidth = element.width+"%";
//			newHeight = element.height+"%";
			}	*/
			else 
			{
			if(element.action.params.heightPX == undefined){
				newHeight = element.height+"%";
			}
			else{
				//alert("height aaa11:"+element.action.params.heightPX);
				if(element.action.library == "H5P.Blanks 1.4" || element.action.library == "H5P.DragText 1.4" || element.action.library == "H5P.MarkTheWords 1.5")
				  newHeight = (element.action.params.heightPX - 10)+"px";
				else
				 newHeight = element.action.params.heightPX+"px";
				
			}
//			newWidth = element.action.params.widthPX+"px";
//			newHeight = element.action.params.heightPX+"px";
////			alert("newWidth111::"+newWidth+"newHeight222::"+newHeight);
//			
			newWidth = element.width+"%";
			//newHeight = element.height+"%";
//			alert("new height:"+newHeight);
		}
		
		
		}else{
			newHeight = element.height+"%";
			newWidth = element.width+"%";
		}
		//console.log(JSON.stringify(element));
		if( element.action != undefined)
	 	{
	 	/*
		if(element.action.library == "H5P.Image 1.0")
		{
		if(element.x >= 5)
			element.x = (element.x/723 * 970) - 12;
		else
			element.x = (element.x/723) * 970;
		}
		*/
	 	}
		//console.log("old element.y"+element.y);
		/*if(element.y >= 4) 
			element.y = (element.y/440 * 439) - 4;
		else
			element.y = (element.y/440) * 439; */
		//console.log("new element.y"+element.y);
		$elementContainer = H5P.jQuery('<div class="h5p-element' + (displayAsButton ? ' h5p-element-button-wrapper' : '') + '" style="'+displayEmptyBox+';left: ' + element.x + '%; top: ' + element.y + '%; width: ' + newWidth + '; height: ' + newHeight + ';"></div>').appendTo($slide);
//		alert(JSON.stringify($elementContainer));
		//if window.setimeout not applied then actual width is not populated in playerwidth - do not remove
		window.setTimeout(function(){
		if(element.action.library == "H5P.Image 1.0")
			return false;
		var playerWidth = parseInt( H5P.jQuery(".h5p-slide").width(),10);
		var playerHeight = parseInt( H5P.jQuery(".h5p-slide").height(),10);
	
		
		var posterTop = parseInt($elementContainer.css("top"),10);
		var posterLeft = parseInt($elementContainer.css("left"),10);
		var posterHeight = parseInt($elementContainer.css("height"),10);
		var posterWidth  = parseInt($elementContainer.css("width"),10);
		var progressBarHeight  = parseInt(H5P.jQuery(".h5p-progressbar").height(),10);
		
		if(posterTop <= 0)
		  $elementContainer.css("top","5px");
		if(posterLeft <= 0)
		  $elementContainer.css("left","5px");
		
		var posterTopAndHeight =  posterTop+posterHeight;
		playerHeight =playerHeight - (progressBarHeight+20);
		console.log("posterTopAndHeight:"+posterTopAndHeight+"==playerHeight:"+playerHeight+":posterTop:"+posterTop);
		if(posterTopAndHeight > playerHeight )
		{
			var newTop = posterTop - (posterTopAndHeight - playerHeight);
			$elementContainer.css("top",newTop);
		}
		
		var posterLeftAndWidth =  posterLeft+posterWidth;
		
		if(posterLeftAndWidth > playerWidth )
		{
			//2 posterLeft:506::posterWidth:112:playerWidth:610:playerHeight:414
			//3 posterLeft:506::posterWidth:112:playerWidth:610:playerHeight:414
			var newLeft = posterLeft - (posterLeftAndWidth - (playerWidth+5)); //5 for additional space
			$elementContainer.css("left",newLeft);
		}
		var cName = "H5P.Image 1.0"; 
		
		
		if(element.action != undefined && element.action.library != undefined)
		{
		 var n = element.action.library.localeCompare(cName);
		 if(n!=0)
		  {
			
//				H5P.jQuery(".h5p-element-outer:not('.h5p-image-outer-element')").css({"position":"relative"});
          
				H5P.jQuery(".h5p-element-outer:not('.h5p-image-outer-element,.h5p-singlechoiceset-outer-element, .h5p-advancedtext-outer-element, .h5p-dragtext-outer-element, .h5p-markthewords-outer-element, .h5p-video-outer-element, .h5p-audio-outer-element, .h5p-blanks-outer-element ')").css({"height":"auto","position":"relative"});
				//$elementContainer.css("height","auto");
				$elementContainer.find(".h5p-element-outer:not('.h5p-image-outer-element,.h5p-singlechoiceset-outer-element, .h5p-advancedtext-outer-element , .h5p-dragtext-outer-element, .h5p-markthewords-outer-element, .h5p-video-outer-element, .h5p-audio-outer-element, .h5p-blanks-outer-element ')").parent().css("height","auto");
             //if(h5p-advancedtext-outer-element)
			}
		}
		
		
			},200);
		
		 
		  
		
		
	}
	
  
  	
  
  var isTransparent = element.backgroundOpacity === undefined || element.backgroundOpacity === 0;
  $elementContainer.toggleClass('h5p-transparent', isTransparent);
  var libTypePmz = '';
  if (displayAsButton) {
    var $buttonElement = H5P.jQuery('<div class="h5p-button-element"></div>');
    instance.attach($buttonElement);
    // Parameterize library name to use as html class.
    libTypePmz = element.action.library.split(' ')[0].toLowerCase().replace(/[\W]/g, '-');
    H5P.jQuery('<a href="#" class="h5p-element-button ' + libTypePmz + '-button"></a>').appendTo($elementContainer).click(function () {
    	
      if (that.editor === undefined) {
			
        // Handle exit fullscreen
        var exitFullScreen = function () {
          that.$footer.removeClass('footer-full-screen');
          that.$fullScreenButton.attr('title', this.l10n.fullscreen);
          instance.trigger('resize');
        };

        // Listen for exit fullscreens not triggered by button, for instance using 'esc'
        that.on('exitFullScreen', exitFullScreen);

        $buttonElement.appendTo(that.showPopup('', function () {
          that.pauseMedia(instance);
          $buttonElement.detach();

          // Remove listener, we only need it for active popups
          that.off('exitFullScreen', exitFullScreen);
        }, libTypePmz).find('.h5p-popup-wrapper'));
        H5P.trigger(instance, 'resize');

        // Resize images to fit popup dialog
        if (libTypePmz === 'h5p-image') {
          that.resizePopupImage($buttonElement);
        }
        if (typeof instance.setActivityStarted === 'function' && typeof instance.getScore === 'function') {
          instance.setActivityStarted();
        }

        // Autoplay media
        if (element.action.params && element.action.params.cpAutoplay && typeof instance.play === 'function') {
          instance.play();
        }
      }
      return false;
    });
    if (element.action !== undefined && element.action.library.substr(0, 20) === 'H5P.InteractiveVideo') {
      instance.on('controls', function () {
        if (instance.controls.$fullscreen) {
          instance.controls.$fullscreen.remove();
        }
      });
    }
  }
  else {
    if (element.action && element.action.library) {
      libTypePmz = element.action.library.split(' ')[0].toLowerCase().replace(/[\W]/g, '-');
    }
    else {
      libTypePmz = 'other';
    }
    var outerElementLibrary = libTypePmz + '-outer-element';
    var $outerElementContainer = H5P.jQuery('<div>', {
      'class': 'h5p-element-outer ' + outerElementLibrary
      
    }).css({
      background: 'rgba(255,255,255,' + (element.backgroundOpacity === undefined ? 0 : element.backgroundOpacity / 100) + ')'
    }).appendTo($elementContainer);

    var $innerElementContainer = H5P.jQuery('<div>', {
      'class': 'h5p-element-inner'
    }).appendTo($outerElementContainer);

	
    instance.attach($innerElementContainer);
    if (element.action !== undefined && element.action.library.substr(0, 20) === 'H5P.InteractiveVideo') {
      var handleIV = function () {
        instance.$container.addClass('h5p-fullscreen');
        if (instance.controls.$fullscreen) {
          instance.controls.$fullscreen.remove();
        }
        instance.hasFullScreen = true;
        if (instance.controls.$play.hasClass('h5p-pause')) {
          instance.$controls.addClass('h5p-autohide');
        }
        else {
          instance.enableAutoHide();
        }
      };
      if (instance.controls !== undefined) {
        handleIV();
      }
      else {
        instance.on('controls', handleIV);
      }
    }
  }

  if (this.editor !== undefined) {
    // If we're in the H5P editor, allow it to manipulate the elementInstances
    this.editor.processElement(element, $elementContainer, index, instance);
  }
  else {
    if (element.solution) {
      this.addElementSolutionButton(element, instance, $elementContainer);
    }

    /* When in view mode, we need to know if there are any answer elements,
     * so that we can display the export answers button on the last slide */
    this.hasAnswerElements = this.hasAnswerElements || instance.exportAnswers !== undefined;
  }

  return $elementContainer;
};

/**
 * Resize image inside popup dialog.
 *
 * @public
 * @param {H5P.jQuery} $wrapper
 */
H5P.CoursePresentation.prototype.resizePopupImage = function ($wrapper) {
  // Get fontsize, needed for scale
  var fontSize = Number($wrapper.css('fontSize').replace('px', ''));
  var $img = $wrapper.find('img');

  /**
   * Resize image to fit inside popup.
   *
   * @private
   * @param {Number} width
   * @param {Number} height
   */
  var resize = function (width, height) {
    if ((height / fontSize) < 18.5) {
      return;
    }

    var ratio = (width / height);
    height = 18.5 * fontSize;
    $wrapper.css({
      width: height * ratio,
      height: height
    });
  };

  if (!$img.height()) {
    // Wait for image to load
    $img.one('load', function () {
      resize(this.width, this.height);
    });
  }
  else {
    // Image already loaded, resize!
    resize($img.width(), $img.height());
  }
};

/**
 * Adds a info button
 *
 * @param {Object} element Properties from params.
 * @param {Object} elementInstance Instance of the element.
 * @param {jQuery} $elementContainer Wrapper for the element.
 * @returns {undefined}
 */
H5P.CoursePresentation.prototype.addElementSolutionButton = function (element, elementInstance, $elementContainer) {
  var that = this;
  elementInstance.showCPComments = function () {
    var $stripHtml = H5P.jQuery('<div>');
    if (!$elementContainer.children('.h5p-element-solution').length && $stripHtml.html(element.solution).text().trim()) {
      H5P.jQuery('<a href="#" class="h5p-element-solution" title="' + that.l10n.solutionsButtonTitle + '"></a>')
        .click(function(event) {
          event.preventDefault();
          that.showPopup(element.solution);
        })
        .appendTo($elementContainer);
    }
  };
  if (element.alwaysDisplayComments !== undefined && element.alwaysDisplayComments) {
    elementInstance.showCPComments();
  }
};

/**
 * Displays a popup.
 *
 * @param {String} popupContent
 * @param {Function} [remove] Gets called before the popup is removed.
 * @returns {undefined}
 */
H5P.CoursePresentation.prototype.showPopup = function (popupContent, remove, classes) {
  var doNotClose;
  var self = this;

  /** @private */
  var close = function(event) {
    if (doNotClose) {
      // Prevent closing the popup
      doNotClose = false;
      return;
    }

    // Remove popup
    if (remove !== undefined) {
      remove();
    }
    event.preventDefault();
    $popup.remove();
  };

  var $popup = H5P.jQuery(
    '<div class="h5p-popup-overlay ' + (classes || 'h5p-popup-comment-field') + '">' +
      '<div class="h5p-popup-container">' +
        '<div class="h5p-popup-wrapper">' + popupContent + '</div>' +
        '<div role="button" tabindex="1" class="h5p-close-popup" title="' + this.l10n.close + '"></div>' +
      '</div>' +
    '</div>')
    .prependTo(this.$wrapper)
    .click(close)
    .find('.h5p-popup-container')
      .click(function () {
        doNotClose = true;
      })
      .end()
    .find('.h5p-close-popup')
      .click(close)
      .end();

  return $popup;
};

/**
 * Checks if an element has a solution
 *
 * @param {H5P library instance} elementInstance
 * @returns {Boolean}
 *  true if the element has a solution
 *  false otherwise
 */
H5P.CoursePresentation.prototype.checkForSolutions = function (elementInstance) {
  return (elementInstance.showSolutions !== undefined ||
          elementInstance.showCPComments !== undefined);
};

/**
 * Generate html for the given keywords.
 *
 * @param {Array} keywords List of keywords.
 * @param {Boolean} first Indicates if this is the first slide.
 * @returns {String} HTML.
 */
H5P.CoursePresentation.prototype.keywordsHtml = function (keywords, first) {
  var html = '';
  if (keywords === undefined) {
    keywords = [];
  }
  for (var i = 0; i < keywords.length; i++) {
    var keyword = keywords[i];

    html += '<li class="h5p-keywords-li"><span>' + keyword.main + '</span>';

    if (keyword.subs !== undefined && keyword.subs.length) {
      html += '<ol class="h5p-keywords-ol">';
      for (var j = 0; j < keyword.subs.length; j++) {
        html += '<li class="h5p-keywords-li h5p-sub-keyword"><span>' + keyword.subs[j] + '</span></li>';
      }
      html += '</ol>';
    }
    html += '</li>';
  }
  if (html) {
    html = '<ol class="h5p-keywords-ol">' + html + '</ol>';
  }

  return '<li class="h5p-keywords-li' + (first ? ' h5p-current' : '') + '">' + html + '</li>';
};

/**
 * Initialize list of keywords
 *
 * @param {string} keywords Html string list entries for keywords
 */
H5P.CoursePresentation.prototype.initKeywordsList = function (keywords) {
  var that = this;

  this.$keywords = this.$keywordsWrapper.html('<ol class="h5p-keywords-ol">' + keywords + '</ol>').children('ol');
  this.$currentKeyword = this.$keywords.children('.h5p-current');

  this.$keywords.children('li').click(function () {
    that.keywordClick(H5P.jQuery(this));
  });

  this.setKeywordsOpacity(this.presentation.keywordListOpacity === undefined ? 90 : this.presentation.keywordListOpacity);

};

/**
 * Initialize key press events.
 *
 * @returns {undefined} Nothing.
 */
H5P.CoursePresentation.prototype.initKeyEvents = function () {
  if (this.keydown !== undefined || this.activeSurface) {
    return;
  }

  var that = this;
  var wait = false;

  this.keydown = function (event) {
    if (wait) {
      return;
    }

    // Left
    if (event.keyCode === 37 && that.previousSlide()) {
      wait = true;
    }

    // Right
    else if (event.keyCode === 39 && that.nextSlide()) {
      wait = true;
    }

    if (wait) {
      // Make sure we only change slide every 300ms.
      setTimeout(function () {
        wait = false;
      }, 50);
    }
  };

  H5P.jQuery('body').keydown(this.keydown);
};

/**
 * Initialize touch events
 *
 * @returns {undefined} Nothing.
 */
H5P.CoursePresentation.prototype.initTouchEvents = function () {
  var that = this;
  var startX, startY, lastX, prevX, nextX, scroll;
  var containerWidth = this.$slidesWrapper.width();
  var containerPercentageForScrolling = 0.6; // 60% of container width used to reach endpoints with touch
  var slidesNumbers = this.slides.length;
  var pixelsPerSlide = (containerWidth * containerPercentageForScrolling) / slidesNumbers;
  var startTime;
  var currentTime;
  var navigateTimer = 300; // 500ms before navigation popup starts.
  var isTouchJump = false;
  var nextSlide;
  var transform = function (value) {
    return {
      '-webkit-transform': value,
      '-moz-transform': value,
      '-ms-transform': value,
      'transform': value
    };
  };
  var reset = transform('');
  var getTranslateX = function ($element) {
    var prefixes = ['', '-webkit-', '-moz-', '-ms-'];
    for (var i = 0; i < prefixes.length; i++) {
      var matrix = $element.css(prefixes[i] + 'transform');
      if (matrix !== undefined) {
        return parseInt(matrix.match(/\d+/g)[4]);
      }
    }
  };

  this.$slidesWrapper.bind('touchstart', function (event) {
    isTouchJump = false;
    // Set start positions
    lastX = startX = event.originalEvent.touches[0].pageX;
    startY = event.originalEvent.touches[0].pageY;
    prevX = getTranslateX(that.$current.addClass('h5p-touch-move').prev().addClass('h5p-touch-move'));
    nextX = getTranslateX(that.$current.next().addClass('h5p-touch-move'));
    containerWidth = H5P.jQuery(this).width();
    startTime = new Date().getTime();

    scroll = null;

  }).bind('touchmove', function (event) {
    var touches = event.originalEvent.touches;

    // Determine horizontal movement
    lastX = touches[0].pageX;
    var movedX = startX - lastX;

    if (scroll === null) {
      // Detemine if we're scrolling horizontally or changing slide
      scroll = Math.abs(startY - event.originalEvent.touches[0].pageY) > Math.abs(movedX);
    }
    if (touches.length !== 1 || scroll) {
      // Do nothing if we're scrolling, zooming etc.
      return;
    }

    // Disable horizontal scrolling when changing slide
    event.preventDefault();

    // Create popup longer time than navigateTimer has passed
    if (!isTouchJump) {
/*      currentTime = new Date().getTime();
      var timeLapsed = currentTime - startTime;
      if (timeLapsed > navigateTimer) {
        isTouchJump = true;
      }*/

      // Fast swipe to next slide
      if (movedX < 0) {
        // Move previous slide
        that.$current.next().css(reset);
        that.$current.prev().css(transform('translateX(' + (prevX - movedX) + 'px'));
      }
      else {
        // Move next slide
        that.$current.prev().css(reset);
        that.$current.next().css(transform('translateX(' + (nextX - movedX) + 'px)'));
      }

      // Move current slide
      that.$current.css(transform('translateX(' + (-movedX) + 'px)'));
    }
    // TODO: Jumping over multiple slides disabled until redesigned.

    /* else {
      that.$current.css(reset);
      // Update slider popup.
      nextSlide = parseInt(that.currentSlideIndex + (movedX / pixelsPerSlide), 10);
      if (nextSlide >= that.slides.length -1) {
        nextSlide = that.slides.length -1;
      } else if (nextSlide < 0) {
        nextSlide = 0;
      }
      // Create popup at initial touch point
      that.updateTouchPopup(that.$slidesWrapper, nextSlide, startX, startY);
    }*/

  }).bind('touchend', function () {
    if (!scroll) {
/*      if (isTouchJump) {
        that.jumpToSlide(nextSlide);
        that.updateTouchPopup();
        return;
      }*/
      // If we're not scrolling detemine if we're changing slide
      var moved = startX - lastX;
      if (moved > that.swipeThreshold && that.nextSlide() || moved < -that.swipeThreshold && that.previousSlide()) {
        return;
      }
    }
    // Reset.
    that.$slidesWrapper.children().css(reset).removeClass('h5p-touch-move');
  });
};

/**
 *
 * @param $container
 * @param slideNumber
 * @param xPos
 * @param yPos
 */
H5P.CoursePresentation.prototype.updateTouchPopup = function ($container, slideNumber, xPos, yPos) {
  // Remove popup on no arguments
  if (arguments.length <= 0) {
    if(this.touchPopup !== undefined) {
      this.touchPopup.remove();
    }
    return;
  }

  var keyword = '';
  var yPosAdjustment = 0.15; // Adjust y-position 15% higher for visibility

  if ((this.$keywords !== undefined) && (this.$keywords.children(':eq(' + slideNumber + ')').find('span').html() !== undefined)) {
    keyword += this.$keywords.children(':eq(' + slideNumber + ')').find('span').html();
  } else {
    var slideIndexToNumber = slideNumber+1;
    keyword += this.l10n.slide + ' ' + slideIndexToNumber;
  }

  // Summary slide keyword
  if (this.editor === undefined) {
    if (slideNumber >= this.slides.length - 1) {
      keyword = this.l10n.showSolutions;
    }
  }

  if (this.touchPopup === undefined) {
    this.touchPopup = H5P.jQuery('<div/>', {
      'class': 'h5p-touch-popup'
    }).insertAfter($container);
  } else {
    this.touchPopup.insertAfter($container);
  }

  // Adjust yPos above finger.
  if ((yPos - ($container.parent().height() * yPosAdjustment)) < 0) {
    yPos = 0;
  } else {
    yPos -= ($container.parent().height() * yPosAdjustment);
  }

  this.touchPopup.css({
    'max-width': $container.width() - xPos,
    'left': xPos,
    'top': yPos
  });
  this.touchPopup.html(keyword);
};

/**
 * Switch to previous slide
 *
 * @param {Boolean} noScroll Skip UI scrolling.
 * @returns {Boolean} Indicates if the move was made.
 */
H5P.CoursePresentation.prototype.previousSlide = function (noScroll) {

	 //added for h5pcustomization #0084699
	fadeoutArrows('slidePrevious');
		
  var $prev = this.$current.prev();
  if (!$prev.length) {
    return false;
  }

  return this.jumpToSlide($prev.index(), noScroll);
};

/**
 * Switch to next slide.
 *
 * @param {Boolean} noScroll Skip UI scrolling.
 * @returns {Boolean} Indicates if the move was made.
 */
H5P.CoursePresentation.prototype.nextSlide = function (noScroll) {
	
	 //added for h5pcustomization #0084699
	fadeoutArrows('slideNext');
	
  var $next = this.$current.next();
  if (!$next.length) {
    return false;
  }

  return this.jumpToSlide($next.index(), noScroll);
};

/**
 * Loads all slides (Needed by print)
 * @method attachAllElements
 */
H5P.CoursePresentation.prototype.attachAllElements = function () {
  var $slides = this.$slidesWrapper.children();

  for (var i=0; i<this.slides.length; i++) {
    this.attachElements($slides.eq(i), i);
  }

  // Need to force updating summary slide! This is normally done
  // only when summary slide is about to be viewed
  if (this.summarySlideObject !== undefined) {
    this.summarySlideObject.updateSummarySlide(this.slides.length-1, true);
  }
};

/**
 * Jump to the given slide.
 *
 * @param {type} slideNumber The slide number to jump to.
 * @param {Boolean} noScroll Skip UI scrolling.
 * @returns {Boolean} Always true.
 */
H5P.CoursePresentation.prototype.jumpToSlide = function (slideNumber, noScroll) {
  var that = this;

  //added for h5pcustomization #0084699
    fadeoutArrows('slideJump',slideNumber);
  
  
  if (this.editor === undefined) {
    var progressedEvent = this.createXAPIEventTemplate('progressed');
    progressedEvent.data.statement.object.definition.extensions['http://id.tincanapi.com/extension/ending-point'] = slideNumber + 1;
    this.trigger(progressedEvent);
  }

  if (this.$current.hasClass('h5p-animate')) {
	  console.log("animate.....");
    return;
  }

  // Jump to given slide and enable animation.
  var $old = this.$current.addClass('h5p-animate');
  var $slides = that.$slidesWrapper.children();
  var $prevs = $slides.filter(':lt(' + slideNumber + ')');
  this.$current = $slides.eq(slideNumber).addClass('h5p-animate');
  var previousSlideIndex = this.currentSlideIndex;
  this.currentSlideIndex = slideNumber;

  // Attach elements for this slide
  this.attachElements(this.$current, slideNumber);

  // Attach elements for next slide
  var $nextSlide = this.$current.next();
  if ($nextSlide.length) {
    this.attachElements($nextSlide, slideNumber + 1);
  }

  // Stop media on old slide
  // this is done no mather what autoplay says
  //alert("previousSlideIndex:"+previousSlideIndex+"====slideNumber:"+slideNumber);
  var instances = this.elementInstances[previousSlideIndex];
  if (instances !== undefined) {
    for (var i = 0; i < instances.length; i++) {
      if (!this.slides[previousSlideIndex].elements[i].displayAsButton) {
        // Only pause media elements displayed as posters.
        that.pauseMedia(instances[i]);
      }
    }
  }
  
  //h5pcusomize Play current slides media if autoplay true
  var instances = this.elementInstances[slideNumber];
  if (instances !== undefined) {
    for (var i = 0; i < instances.length; i++) {
      if (!this.slides[slideNumber].elements[i].displayAsButton) {
        // Only play media elements displayed as posters.
        try{
        if(this.slides[slideNumber].elements[i].action.params.playback != undefined && this.slides[slideNumber].elements[i].action.params.playback.autoplay == true)
        	that.playMedia(instances[i]);
        }catch(e){}
      }
    }
    
  }
  

  setTimeout(function () {
    // Play animations
    $old.removeClass('h5p-current');
    $slides.css({
      '-webkit-transform': '',
      '-moz-transform': '',
      '-ms-transform': '',
      'transform': ''
    }).removeClass('h5p-touch-move').removeClass('h5p-previous');
    $prevs.addClass('h5p-previous');
    that.$current.addClass('h5p-current');
    that.trigger('changedSlide', that.$current.index());
  }, 1);

  // Done animating
  that.$slidesWrapper.children().removeClass('h5p-animate');

  setTimeout(function () {
   
    if (that.editor !== undefined) {
      return;
    }

    // Start media on new slide for elements beeing setup with autoplay!
    var instances = that.elementInstances[that.currentSlideIndex];
    var instanceParams = that.slides[that.currentSlideIndex].elements;
    if (instances !== undefined) {
      for (var i = 0; i < instances.length; i++) {
        // TODO: Check instance type instead to avoid accidents?
        if (instanceParams[i] &&
            instanceParams[i].action &&
            instanceParams[i].action.params &&
            instanceParams[i].action.params.cpAutoplay &&
            !instanceParams[i].displayAsButton &&
            typeof instances[i].play === 'function') {

          // Autoplay media if not button
          instances[i].play();
        }

        if (!instanceParams[i].displayAsButton && typeof instances[i].setActivityStarted === 'function' && typeof instances[i].getScore === 'function') {
          instances[i].setActivityStarted();
        }
      }
    }
  }, 150); //default 250

  // Jump keywords
  if (this.$keywords !== undefined) {
    this.$currentKeyword.removeClass('h5p-current');
    this.$currentKeyword = this.$keywords.children(':eq(' + slideNumber + ')').addClass('h5p-current');

    if (!noScroll) {
      this.scrollToKeywords();
    }

    if (this.editor !== undefined) {
      // Move add keywords button if using editor
      this.editor.$newKeyword.appendTo(this.$currentKeyword);
    }
  }

  // Show keywords if they should always show
  if (that.presentation.keywordListEnabled && that.presentation.keywordListAlwaysShow) {
    that.showKeywords();
  }

  if (that.navigationLine) {
    // Update progress bar
    that.navigationLine.updateProgressBar(slideNumber, previousSlideIndex, this.isSolutionMode);

    // Update footer
    that.navigationLine.updateFooter(slideNumber);
  }

  if (that.summarySlideObject) {
    // Update summary slide if on last slide, do not jump
    that.summarySlideObject.updateSummarySlide(slideNumber, true);
  }

  // Editor specific settings
  if (this.editor !== undefined && this.editor.dnb !== undefined) {
    // Update drag and drop menu bar container
    this.editor.dnb.setContainer(this.$current);
    this.editor.dnb.blurAll();
  }

  this.trigger('resize'); // Triggered to resize elements.
  this.fitCT();
  return true;
};

/**
 * Scroll to current keywords.
 *
 * @returns {undefined} Nothing
 */
H5P.CoursePresentation.prototype.scrollToKeywords = function () {
  var $parent = this.$currentKeyword.parent();
  var move = $parent.scrollTop() + this.$currentKeyword.position().top - 8;

  if (H5P.CoursePresentation.isiPad) {
    // scrollTop animations does not work well on ipad.
    // TODO: Check on iPhone.
    $parent.scrollTop(move);
  }
  else {
    $parent.stop().animate({scrollTop: move}, 250);
  }
};

/**
 * @type Boolean Indicate if this is an ipad user.
 */
H5P.CoursePresentation.isiPad = navigator.userAgent.match(/iPad/i) !== null;

/**
 * Create HTML for a slide.
 *
 * @param {object} slide Params.
 * @returns {String} HTML.
 */
H5P.CoursePresentation.createSlide = function (slide) {
  return '<div class="h5p-slide"' + (slide.background !== undefined ? ' style="background:' + slide.background + '"' : '') + '"></div>';
};

/**
 * Reset the content for all slides.
 * @public
 */
H5P.CoursePresentation.prototype.resetTask = function () {
  this.summarySlideObject.toggleSolutionMode(false);
  for (var i = 0; i < this.slidesWithSolutions.length; i++) {
    if (this.slidesWithSolutions[i] !== undefined) {
      for (var j = 0; j < this.slidesWithSolutions[i].length; j++) {
        var elementInstance = this.slidesWithSolutions[i][j];
        if (elementInstance.resetTask) {
          elementInstance.resetTask();
        }
      }
    }
  }
  this.navigationLine.updateProgressBar(0);
  this.jumpToSlide(0, false);
  this.$container.find('.h5p-popup-overlay').remove();
};

/**
 * Show solutions for all slides that have solutions
 *
 * @returns {undefined}
 */
H5P.CoursePresentation.prototype.showSolutions = function () {
  var jumpedToFirst = false;
  var slideScores = [];
  var hasScores = false;
  for (var i = 0; i < this.slidesWithSolutions.length; i++) {
    if (this.slidesWithSolutions[i] !== undefined) {
      if (!this.elementsAttached[i]) {
        // Attach elements before showing solutions
        this.attachElements(this.$slidesWrapper.children(':eq(' + i + ')'), i);
      }
      if (!jumpedToFirst) {
        this.jumpToSlide(i, false);
        jumpedToFirst = true; // TODO: Explain what this really does.
      }
      var slideScore = 0;
      var slideMaxScore = 0;
      var indexes = [];
      for (var j = 0; j < this.slidesWithSolutions[i].length; j++) {
        var elementInstance = this.slidesWithSolutions[i][j];
        if (elementInstance.addSolutionButton !== undefined) {
          elementInstance.addSolutionButton();
        }
        if (elementInstance.showSolutions) {
          elementInstance.showSolutions();
        }
        if (elementInstance.showCPComments) {
          elementInstance.showCPComments();
        }
        if (elementInstance.getMaxScore !== undefined) {
          slideMaxScore += elementInstance.getMaxScore();
          slideScore += elementInstance.getScore();
          hasScores = true;
          indexes.push(elementInstance.coursePresentationIndexOnSlide);
        }
      }
      slideScores.push({
        indexes: indexes,
        slide: (i + 1),
        score: slideScore,
        maxScore: slideMaxScore
      });
    }
  }
  if (hasScores) {
    return slideScores;
  }
};

/**
 * Gets slides scores for whole cp
 * @returns {Array} slideScores Array containing scores for all slides.
 */
H5P.CoursePresentation.prototype.getSlideScores = function (noJump) {
  var jumpedToFirst = (noJump === true);
  var slideScores = [];
  var hasScores = false;
  for (var i = 0; i < this.slidesWithSolutions.length; i++) {
    if (this.slidesWithSolutions[i] !== undefined) {
      if (!this.elementsAttached[i]) {
        // Attach elements before showing solutions
        this.attachElements(this.$slidesWrapper.children(':eq(' + i + ')'), i);
      }
      if (!jumpedToFirst) {
        this.jumpToSlide(i, false);
        jumpedToFirst = true; // TODO: Explain what this really does.
      }
      var slideScore = 0;
      var slideMaxScore = 0;
      var indexes = [];
      for (var j = 0; j < this.slidesWithSolutions[i].length; j++) {
        var elementInstance = this.slidesWithSolutions[i][j];
        if (elementInstance.getMaxScore !== undefined) {
          slideMaxScore += elementInstance.getMaxScore();
          slideScore += elementInstance.getScore();
          hasScores = true;
          indexes.push(elementInstance.coursePresentationIndexOnSlide);
        }
      }
      slideScores.push({
        indexes: indexes,
        slide: (i + 1),
        score: slideScore,
        maxScore: slideMaxScore
      });
    }
  }
  if (hasScores) {
    return slideScores;
  }
};

/**
 * Gather copyright information for the current content.
 *
 * @returns {H5P.ContentCopyrights}
 */
H5P.CoursePresentation.prototype.getCopyrights = function () {
  var info = new H5P.ContentCopyrights();

  var elementCopyrights;
  for (var slide = 0; slide < this.elementInstances.length; slide++) {
    var slideInfo = new H5P.ContentCopyrights();
    slideInfo.setLabel(this.l10n.slide + ' ' + (slide + 1));

    if (this.elementInstances[slide] !== undefined) {
      for (var element = 0; element < this.elementInstances[slide].length; element++) {
        var instance = this.elementInstances[slide][element];
        var params = this.slides[slide].elements[element].action.params;

        elementCopyrights = undefined;
        if (instance.getCopyrights !== undefined) {
          // Use the instance's own copyright generator
          elementCopyrights = instance.getCopyrights();
        }
        if (elementCopyrights === undefined) {
          // Create a generic flat copyright list
          elementCopyrights = new H5P.ContentCopyrights();
          H5P.findCopyrights(elementCopyrights, params, this.contentId);
        }

        var label = (element + 1);
        if (params.contentName !== undefined) {
          label += ': ' + params.contentName;
        }
        else if (instance.getTitle !== undefined) {
          label += ': ' + instance.getTitle();
        }
        elementCopyrights.setLabel(label);

        slideInfo.addContent(elementCopyrights);
      }
    }

    info.addContent(slideInfo);
  }

  return info;
};

/**
 * Stop the given element's playback if any.
 *
 * @param {object} instance
 */
H5P.CoursePresentation.prototype.pauseMedia = function (instance) {
  try {
    if (instance.pause !== undefined &&
        (instance.pause instanceof Function ||
          typeof instance.pause === 'function')) {
      instance.pause();
    }
    else if (instance.video !== undefined &&
             instance.video.pause !== undefined &&
             (instance.video.pause instanceof Function ||
               typeof instance.video.pause === 'function')) {
      instance.video.pause();
    }
    else if (instance.stop !== undefined &&
             (instance.stop instanceof Function ||
               typeof instance.stop === 'function')) {
      instance.stop();
    }
  }
  catch (err) {
    // Prevent crashing, but tell developers there's something wrong.
    H5P.error(err);
  }
};


/**
 * Stop the given element's playback if any.
 *
 * @param {object} instance
 */
H5P.CoursePresentation.prototype.playMedia = function (instance) {
  try {
    if (instance.play !== undefined &&
        (instance.play instanceof Function ||
          typeof instance.play === 'function')) {
      instance.play();
    }
    else if (instance.video !== undefined &&
             instance.video.play !== undefined &&
             (instance.video.play instanceof Function ||
               typeof instance.video.play === 'function')) {
      instance.video.play();
    }
    else if (instance.play !== undefined &&
             (instance.play instanceof Function ||
               typeof instance.play === 'function')) {
      instance.play();
    }
  }
  catch (err) {
    // Prevent crashing, but tell developers there's something wrong.
    H5P.error(err);
  }
};

//added for h5pcustomization #0084699
function fadeoutArrows(from,slideNumber)
{
  if(from == "slidePrevious")
  {
	 if(H5P.jQuery(".h5p-footer .h5p-footer-slide-count-current").html() == "1" || H5P.jQuery(".h5p-footer .h5p-footer-slide-count-current").html() == "2" ){
		 H5P.jQuery(".h5p-footer .h5p-footer-previous-slide").css({'color':'#cccccc'});
	 }
	 else{
		 H5P.jQuery(".h5p-footer .h5p-footer-previous-slide").css({'color':'#474747;'});
	 }
	 H5P.jQuery(".h5p-footer .h5p-footer-next-slide").css("");
	 if(H5P.jQuery(".h5p-footer .h5p-footer-slide-count-current").html() == H5P.jQuery(".h5p-footer .h5p-footer-slide-count-max").html() )
    	{
 	 H5P.jQuery(".h5p-footer .h5p-footer-next-slide").css({'color':'#cccccc'});
 	}
	 else
		 H5P.jQuery(".h5p-footer .h5p-footer-next-slide").css({'color':'#474747;'});
	}
  else if(from == "slideNext"){
	  if(H5P.jQuery(".h5p-footer .h5p-footer-slide-count-current").html() == H5P.jQuery(".h5p-footer .h5p-footer-slide-count-max").html()   || H5P.jQuery(".h5p-footer .h5p-footer-slide-count-current").html() == (H5P.jQuery(".h5p-footer .h5p-footer-slide-count-max").html() - 1))
			 H5P.jQuery(".h5p-footer .h5p-footer-next-slide").css({'color':'#cccccc'});
		 else
			 H5P.jQuery(".h5p-footer .h5p-footer-next-slide").css({'color':'#474747;'});
		 
		 H5P.jQuery(".h5p-footer .h5p-footer-previous-slide").css("");
		 H5P.jQuery(".h5p-footer .h5p-footer-previous-slide").css({'color':'#474747;'});
  }
  else if(from == 'slideJump'){
	  if((slideNumber + 1) == H5P.jQuery(".h5p-footer .h5p-footer-slide-count-max").html())
		{	
		 H5P.jQuery(".h5p-footer .h5p-footer-next-slide").css({'color':'#cccccc'});
		}
		 else
		 H5P.jQuery(".h5p-footer .h5p-footer-next-slide").css({'color':'#474747;'});
	  
	  if(slideNumber  == 0) 
		{	
		 H5P.jQuery(".h5p-footer .h5p-footer-previous-slide").css({'color':'#cccccc'});
		}
		 else
			H5P.jQuery(".h5p-footer .h5p-footer-previous-slide").css({'color':'#474747;'});
    }
  else if(from == 'slideRemove'){
	  if($(".h5p-footer .h5p-footer-slide-count-current").html() == "1" || $(".h5p-footer .h5p-footer-slide-count-current").html() == "2")
      {
      	H5P.jQuery(".h5p-footer .h5p-footer-previous-slide").css({'color':'#cccccc'});
      }
      if(H5P.jQuery(".h5p-footer .h5p-footer-slide-count-current").html() == H5P.jQuery(".h5p-footer .h5p-footer-slide-count-max").html()  || (H5P.jQuery(".h5p-footer .h5p-footer-slide-count-current").html() ) == (H5P.jQuery(".h5p-footer .h5p-footer-slide-count-max").html() - 1))
      	{
      	 H5P.jQuery(".h5p-footer .h5p-footer-next-slide").css({'color':'#cccccc'});
      	}
  }
  else if(from == 'slideNew'){
	  if(H5P.jQuery(".h5p-footer .h5p-footer-slide-count-current").html() == "1" && H5P.jQuery(".h5p-footer .h5p-footer-slide-count-max").html() == "1")
      {
       H5P.jQuery(".h5p-footer .h5p-footer-next-slide").css({'color':'#cccccc'});
       H5P.jQuery(".h5p-footer .h5p-footer-previous-slide").css({'color':'#cccccc'});
      }
      else if(H5P.jQuery(".h5p-footer .h5p-footer-slide-count-current").html() == "1" && H5P.jQuery(".h5p-footer .h5p-footer-slide-count-max").html() != "1")
     {
    	  H5P.jQuery(".h5p-footer .h5p-footer-previous-slide").css({'color':'#cccccc'});
     }
  }
}

;/** @namespace H5P */
H5P.CoursePresentation.GoToSlide = (function ($) {

  /**
   * Element for linking between slides in presentations.
   *
   * @class
   * @param {Number} slideNum
   * @param {CoursePresentation} cp
   */
  function GoToSlide(title, slideNum, invisible, cp) {
    var self = this;

    var classes = 'h5p-press-to-go';
    var tabindex = 1;
    
    if (invisible) {
      title = undefined;
      tabindex = -1;
    }
    else {
      title = title ? title : cp.l10n.goToSlide.replace(':num', slideNum);
      classes += ' h5p-visible';
    }

    slideNum--;

    /**
     * @private
     */
    var go = function () {
      if (cp.editor === undefined && cp.slides[slideNum] !== undefined) {
        cp.jumpToSlide(slideNum);
      }
    };

    // Create button that leads to another slide
    var $button = $('<div/>', {
      'class': classes,
      role: 'button',
      tabindex: tabindex,
      title: title,
      on: {
        click: go,
        keypress: function (event) {
          if (event.which === 13) {
            go();
          }
        }
      }
    });
    $button.html(title);

    /**
     * Attach element to the given container.
     *
     * @public
     * @param {jQuery} $container
     */
    self.attach = function ($container) {
      $container.addClass('h5p-go-to-slide').append($button);
    };
  }

  return GoToSlide;
})(H5P.jQuery);
;var H5P = H5P || {};
H5P.CoursePresentation = H5P.CoursePresentation || {};

H5P.CoursePresentation.SummarySlide = (function ($, JoubelUI) {

  /**
   * Constructor for summary slide
   * @param {H5P.CoursePresentation} coursePresentation Course presentation parent of summary slide
   * @param {$} $summarySlide Summary slide element
   * @constructor
   */
  function SummarySlide(coursePresentation, $summarySlide) {
    // Create summary slide if not an editor
    this.$summarySlide = $summarySlide;
    this.cp = coursePresentation;
  }

  /**
   * Updates the provided summary slide with current values.
   *
   * @param {$} $summarySlide Summary slide that will be updated
   */
  SummarySlide.prototype.updateSummarySlide = function (slideNumber, noJump) {
    var that = this;
    // Validate update.
    var isValidUpdate = (this.cp.editor === undefined) && (this.$summarySlide !== undefined) && (slideNumber >= this.cp.slides.length - 1);
    var isExportSlide = (!this.cp.showSummarySlide && this.cp.hasAnswerElements);
    if (!isValidUpdate) {
      return;
    }

    // Hide keywordlist on summary slide
    if (that.cp.presentation.keywordListEnabled && that.cp.presentation.keywordListAlwaysShow) {
      that.cp.hideKeywords();
    }

    // Remove old content
    this.$summarySlide.children().remove();

    // Get scores and updated html for summary slide
    var slideScores = that.cp.getSlideScores(noJump);
    var htmlText = that.outputScoreStats(slideScores);
    $(htmlText).appendTo(that.$summarySlide);

    if (!isExportSlide) {
      // Get total scores and construct progress circle
      var totalScores = that.totalScores(slideScores);
      if (isNaN(totalScores.totalPercentage)) {
        JoubelUI.createProgressCircle(0)
          .appendTo($('.h5p-score-message-percentage', that.$summarySlide));
      }
      else {
        JoubelUI.createProgressCircle(totalScores.totalPercentage)
          .appendTo($('.h5p-score-message-percentage', that.$summarySlide));
      }

      // TODO: Get approved App-id for posting to facebook.
      // Construct facebook share score link
      //var $facebookContainer = $('.h5p-summary-facebook-message', that.$summarySlide).remove();
      //this.addFacebookScoreLinkTo($facebookContainer, totalScores.totalPercentage);
      $('.h5p-summary-facebook-message', that.$summarySlide).remove();

      // Construct twitter share score link
      var $twitterContainer = $('.h5p-summary-twitter-message', that.$summarySlide);
      this.addTwitterScoreLinkTo($twitterContainer, totalScores.totalPercentage);

      // Update slide links
      var links = that.$summarySlide.find('.h5p-td > .h5p-slide-link');
      links.each(function () {
        var slideLink = $(this);
        slideLink.click(function (event) {
          that.cp.jumpToSlide(parseInt(slideLink.data('slide'), 10) - 1);
          event.preventDefault();
        });
      });
    }

    // Button container ref
    var $summaryFooter = $('.h5p-summary-footer', that.$summarySlide);

    // Show solutions button
    JoubelUI.createButton({
      'class': 'h5p-show-solutions',
      html: that.cp.l10n.showSolutions,
      on: {
        click: function (event) {
          // Enable solution mode
          that.toggleSolutionMode(true);
        }
      },
      appendTo: $summaryFooter
    });

    // Show solutions button
    JoubelUI.createButton({
      'class': 'h5p-cp-retry-button',
      html: that.cp.l10n.retry,
      on: {
        click: function (event) {
          that.cp.resetTask();
          // event.preventDefault();
        }
      },
      appendTo: $summaryFooter
    });

    // Only make export button if there is an export area in CP
    if (that.cp.hasAnswerElements) {
      JoubelUI.createButton({
        'class': 'h5p-eta-export',
        html: that.cp.l10n.exportAnswers,
        on: {
          click: function (event) {
            H5P.ExportableTextArea.Exporter.run(that.cp.slides, that.cp.elementInstances);
            // event.preventDefault();
          }
        },
        appendTo: $summaryFooter
      });
    }
  };

  /**
   * Gets html for summary slide.
   *
   * @param slideScores Scores for all pages
   * @returns {string} html
   */
  SummarySlide.prototype.outputScoreStats = function (slideScores) {
    var self = this;
    if (slideScores === undefined) {
      this.$summarySlide.addClass('h5p-summary-only-export');
      return '<div class="h5p-summary-footer"></div>';
    }
    var that = this;
    var totalScore = 0;
    var totalMaxScore = 0;
    var tds = ''; // For saving the main table rows
    var i;
    var slidePercentageScore = 0;
    var slideDescription = '';
    var slideElements;
    var action;
    for (i = 0; i < slideScores.length; i += 1) {
      slideDescription = self.getSlideDescription(slideScores[i]);
      
      // Get percentage score for slide
      slidePercentageScore = Math.round((slideScores[i].score / slideScores[i].maxScore) * 100);
      if (isNaN(slidePercentageScore)) {
        slidePercentageScore = 0;
      }
      tds +=
        '<tr>' +
          '<td class="h5p-td h5p-summary-task-title">' +
            '<span role="button" class="h5p-slide-link" data-slide="' + slideScores[i].slide + '">' + that.cp.l10n.slide + ' ' + slideScores[i].slide + ': ' + (slideDescription.replace(/(<([^>]+)>)/ig, "")) + '</span>' +
          '</td>' +
          '<td class="h5p-td h5p-summary-score-bar">' +
            '<div title="' + slidePercentageScore + '%" class="h5p-summary-score-meter">' +
              '<span style="width: ' + slidePercentageScore + '%; opacity: ' + (slidePercentageScore / 100) + '"></span>' +
            '</div>' +
          '</td>' +
        '</tr>';
      totalScore += slideScores[i].score;
      totalMaxScore += slideScores[i].maxScore;
    }

    that.cp.triggerXAPICompleted(totalScore, totalMaxScore);

    var percentScore = Math.round((totalScore / totalMaxScore) * 100);

    var html =
      '<div class="h5p-score-message">' +
      '<div class="h5p-score-message-percentage">' + that.cp.l10n.scoreMessage + '</div>' +
      '<div class="h5p-summary-facebook-message"></div>' +
      '<div class="h5p-summary-twitter-message"></div>' +
      '</div>' +
      '<div class="h5p-summary-table-holder">' +
      ' <div class="h5p-summary-table-pages">' +
      '   <table class="h5p-score-table">' +
      '     <tbody>' + tds + '</tbody>' +
      '   </table>' +
      ' </div>' +
      ' <table class="h5p-summary-total-table" style="width: 100%">' +
      '    <tr>' +
      '     <td class="h5p-td h5p-summary-task-title">' + that.cp.l10n.total + '</td>' +
      '     <td class="h5p-td h5p-summary-score-bar">' +
      '       <div title="' + percentScore + '%" class="h5p-summary-score-meter">' +
      '         <span style="width: ' + percentScore + '%; opacity: ' + (percentScore / 100) + '"></span>' +
      '       </div>' +
      '     </td>' +
      '   </tr>' +
      ' </table>' +
      '</div>' +
      '<div class="h5p-summary-footer">' +
      '</div>';

    return html;
  };

  SummarySlide.prototype.getSlideDescription = function (slideScoresSlide) {
    var self = this;
    // Get task description, task name or identify multiple tasks:
    var slideDescription, action;
    var slideElements = self.cp.slides[slideScoresSlide.slide - 1].elements;
    if (slideScoresSlide.indexes.length > 1) {
      slideDescription = self.cp.l10n.summaryMultipleTaskText;
    } else if (slideElements[slideScoresSlide.indexes[0]] !== undefined && slideElements[0]) {
      action = slideElements[slideScoresSlide.indexes[0]].action;
      if (typeof self.cp.elementInstances[slideScoresSlide.slide - 1][slideScoresSlide.indexes[0]].getTitle === 'function') {
        slideDescription = self.cp.elementInstances[slideScoresSlide.slide - 1][slideScoresSlide.indexes[0]].getTitle();
      } else if (action.library !== undefined && action.library) {

        // Remove major, minor version and h5p prefix, Split on uppercase
        var humanReadableLibrary = action.library
          .split(' ')[0]
          .split('.')[1]
          .split(/(?=[A-Z])/);
        var humanReadableString = '';

        // Make library human readable
        humanReadableLibrary.forEach(function (readableWord, index) {

          // Make sequential words lowercase
          if (index !== 0) {
            readableWord = readableWord.toLowerCase();
          }
          humanReadableString += readableWord;

          // Add space between words
          if (index <= humanReadableLibrary.length - 1) {
            humanReadableString += ' ';
          }
        });
        slideDescription = humanReadableString;
      }
    }
    return slideDescription;
  };

  /**
   * Adds a link to the given container which will link achieved score to facebook.
   *
   * @param {jQuery} $facebookContainer Container that should hold the facebook link.
   * @param {Number} percentageScore Percentage score that should be linked.
   */
  SummarySlide.prototype.addFacebookScoreLinkTo = function ($facebookContainer, percentageScore) {
    var that = this;
    $('<span class="show-facebook-icon">' + that.cp.l10n.shareFacebook + '</span>')
      .appendTo($facebookContainer);

    var facebookString = 'http://www.facebook.com/dialog/feed?' +
      'app_id=1385640295075628&' +
      'link=http://h5p.org/&' +
      'name=H5P&20task&' +
      'caption=I%20just%20finished%20a%20H5P%20task!&' +
      'description=I%20got%20' + percentageScore + '%25%20at:%20' + window.location.href + '&' +
      'redirect_uri=http://h5p.org/';

    var popupWidth = 800;
    var popupHeight = 500;
    var leftPos = (window.innerWidth / 2);
    var topPos = (window.innerHeight / 2);

    $facebookContainer.attr('tabindex', '0')
      .attr('role', 'button')
      .click(function () {
        window.open(facebookString,
          that.cp.l10n.shareFacebook,
          'width=' + popupWidth +
          ',height=' + popupHeight +
          ',left=' + leftPos +
          ',top=' + topPos);
        return false;
      });
  };

  /**
   * Adds a link to the given container which will link achieved score to twitter.
   *
   * @param {jQuery} $twitterContainer Container that should hold the twitter link.
   * @param {Number} percentageScore Percentage score that should be linked.
   */
  SummarySlide.prototype.addTwitterScoreLinkTo = function ($twitterContainer, percentageScore) {
    var that = this;
    var twitterString = 'http://twitter.com/share?text=I%20got%20' + percentageScore + '%25%20on%20this%20task:';

    var popupWidth = 800;
    var popupHeight = 250;
    var leftPos = (window.innerWidth / 2);
    var topPos = (window.innerHeight / 2);

    $twitterContainer.attr('tabindex', '0')
      .attr('role', 'button')
      .click(function () {
        window.open(twitterString,
          that.cp.l10n.shareTwitter,
          'width=' + popupWidth +
          ',height=' + popupHeight +
          ',left=' + leftPos +
          ',top=' + topPos);
        return false;
      });

    $('<span class="show-twitter-icon">' + that.cp.l10n.shareTwitter + '</span>')
      .appendTo($twitterContainer);
  };

  /**
   * Gets total scores for all slides
   * @param {Array} slideScores
   * @returns {{totalScore: number, totalMaxScore: number, totalPercentage: number}} totalScores Total scores object
   */
  SummarySlide.prototype.totalScores = function (slideScores) {
    if (slideScores === undefined) {
      return {
        totalScore: 0,
        totalMaxScore: 0,
        totalPercentage: 0
      };
    }
    var totalScore = 0;
    var totalMaxScore = 0;
    var i;
    for (i = 0; i < slideScores.length; i += 1) {
      // Get percentage score for slide
      totalScore += slideScores[i].score;
      totalMaxScore += slideScores[i].maxScore;
    }

    var totalPercentage = Math.round((totalScore / totalMaxScore) * 100);
    if (isNaN(totalPercentage)) {
      totalPercentage = 0;
    }

    return {
      totalScore: totalScore,
      totalMaxScore: totalMaxScore,
      totalPercentage: totalPercentage
    };
  };

  /**
   * Toggles solution mode on/off.
   *
   * @params {Boolean} enableSolutionMode Enable/disable solution mode
   */
  SummarySlide.prototype.toggleSolutionMode = function (enableSolutionMode) {
    var that = this;

    this.cp.isSolutionMode = enableSolutionMode;
    if (enableSolutionMode) {
      // Get scores for summary slide
      var slideScores = that.cp.showSolutions();

      // Update feedback icons in solution mode
      this.cp.setProgressBarFeedback(slideScores);
      this.cp.$footer.addClass('h5p-footer-solution-mode');
      this.setFooterSolutionModeText(this.cp.l10n.solutionModeText);
    }
    else {
      this.cp.$footer.removeClass('h5p-footer-solution-mode');
      this.setFooterSolutionModeText();
      this.cp.setProgressBarFeedback();
    }
  };

  /**
   * Sets the solution mode button text in footer.
   *
   * @param solutionModeText
   */
  SummarySlide.prototype.setFooterSolutionModeText = function (solutionModeText) {
    if (solutionModeText !== undefined && solutionModeText) {
      this.cp.$exitSolutionModeText.html(solutionModeText);
    }
    else if (this.cp.$exitSolutionModeText) {
      this.cp.$exitSolutionModeText.html('');
    }
  };

  return SummarySlide;
})(H5P.jQuery, H5P.JoubelUI);
;var H5P = H5P || {};
H5P.CoursePresentation = H5P.CoursePresentation || {};

H5P.CoursePresentation.NavigationLine = (function ($) {

  function NavigationLine(coursePresentation) {
    this.cp = coursePresentation;
    this.initProgressbar(this.cp.slidesWithSolutions);
    this.initFooter();
    this.initTaskAnsweredListener();
  }

  /**
   * Initializes xAPI event listener, updates progressbar when a task is changed.
   */
  NavigationLine.prototype.initTaskAnsweredListener = function () {
    var that = this;

    this.cp.elementInstances.forEach(function (element) {
      element.forEach(function (elementInstance) {
        if (elementInstance.on !== undefined) {
          elementInstance.on('xAPI', function (event) {
            var shortVerb = event.getVerb();
            if (shortVerb === 'interacted') {
              that.updateProgressBarTasksAtSlideNumber(that.cp.currentSlideIndex);
            }
            else if (shortVerb === 'completed') {
              event.setVerb('answered');
            }
            if (event.data.statement.context.extensions === undefined) {
              event.data.statement.context.extensions = [];
            }
            event.data.statement.context.extensions['http://id.tincanapi.com/extension/ending-point'] = that.cp.currentSlideIndex + 1;
          });
        }
      });
    });
  };

  /**
   * Initialize progress bar
   */
  NavigationLine.prototype.initProgressbar = function (slidesWithSolutions) {
    var supportsHover = true;
    if (navigator.userAgent.match(/iPad|iPod|iPhone/i) !== null) {
      supportsHover = false;
    }

    var that = this;
    var progressbarPercentage = (1 / this.cp.slides.length) * 100;

    // Remove existing progressbar
    if (this.cp.progressbarParts !== undefined && this.cp.progressbarParts) {
      this.cp.progressbarParts.forEach(function (pbPart) {
        pbPart.remove();
      });
    }

    that.cp.progressbarParts = [];

    var i;
    var slide;
    var $progressbarPart;
    var progressbarPartTitle;

    var clickProgressbar = function () {
      that.cp.jumpToSlide($(this).data('slideNumber'));
    };

    var mouseenterProgressbar = function (event) {
      that.createProgressbarPopup(event, $(this));
    };

    var mouseleaveProgressbar = function () {
      that.removeProgressbarPopup();
    };

    for (i = 0; i < this.cp.slides.length; i += 1) {
      slide = this.cp.slides[i];

      // Generate tooltip for progress bar slides
      progressbarPartTitle = String(that.cp.l10n.slide) + (i + 1);
      if (slide.keywords !== undefined && slide.keywords.length) {
        progressbarPartTitle = slide.keywords[0].main;
      } else if (that.cp.editor === undefined && i >= this.cp.slides.length - 1 && this.cp.showSummarySlide) {
        progressbarPartTitle = that.cp.l10n.summary;
      }

      $progressbarPart = $('<div>', {
        'width': progressbarPercentage + '%',
        'class': 'h5p-progressbar-part'
      }).data('slideNumber', i)
        .data('keyword', progressbarPartTitle)
        .data('percentageWidth', progressbarPercentage)
        .click(clickProgressbar)
        .appendTo(that.cp.$progressbar);
     
      // Add hover effect if not an ipad or iphone.
      if (supportsHover) {
        $progressbarPart
          .mouseenter(mouseenterProgressbar)
          .mouseleave(mouseleaveProgressbar);
      }

      if ((this.cp.editor === undefined) && (i === this.cp.slides.length - 1) && this.cp.showSummarySlide) {
        $progressbarPart.addClass('progressbar-part-summary-slide');

        // Add svg icons to summary slide
        $('<div>', {
          'class': 'summary-slide-left-svg'
        }).appendTo($progressbarPart);
        $('<div>', {
          'class': 'summary-slide-right-svg'
        }).appendTo($progressbarPart);
      }

      if (i === 0) {
        $progressbarPart.addClass('h5p-progressbar-part-show');
      }
      // Create task indicator if less than 60 slides and not in editor
      if (this.cp.slides.length <= 75) {
        if (slide.elements !== undefined && slide.elements.length) {
          if (slidesWithSolutions[i] !== undefined && slidesWithSolutions[i].length) {
            var elementOptions = {
              'class': 'h5p-progressbar-part-has-task'
            };
            if (that.cp.previousState && that.cp.previousState.answered && that.cp.previousState.answered[i]) {
              elementOptions.class += ' h5p-answered';
            }

            $('<div>', elementOptions).appendTo($progressbarPart);
          }
        }
      }
      that.cp.progressbarParts.push($progressbarPart);
    }
  };

  NavigationLine.prototype.createProgressbarPopup = function (event, $parent) {
    var progressbarTitle = $parent.data('keyword');
    var slidenum = progressbarTitle.substr(5);   //h5pcustomisation
    progressbarTitle = "";
    progressbarTitle += window.parent.Drupal.t("LBL3204") + slidenum; //h5pcustomisation
    if (this.$progressbarPopup === undefined) {
      this.$progressbarPopup = H5P.jQuery('<div/>', {
        'class': 'h5p-progressbar-popup',
        'html':  progressbarTitle
      }).appendTo($parent);
    } else {
      this.$progressbarPopup.appendTo($parent);
      this.$progressbarPopup.html(progressbarTitle);
    }
    var pbpartPercentWidth = $parent.data('percentageWidth');
    var width = this.$progressbarPopup.width();
    var popupPercentageWidth = (width / this.cp.$container.width()) * 100;
    var leftPos = (pbpartPercentWidth * $parent.data('slideNumber')) + (pbpartPercentWidth / 2) - (popupPercentageWidth / 2);
    var height = '10%';

    if ((((leftPos / 100) * this.cp.$container.width()) + width) >= this.cp.$container.width()) {
      leftPos -= (width / (this.cp.$container.width() / 100));
    }

    this.$progressbarPopup.css({
      'left': leftPos + '%',
      'bottom': height
    });
  };

  NavigationLine.prototype.removeProgressbarPopup = function () {
    if (this.$progressbarPopup !== undefined) {
      this.$progressbarPopup.remove();
    }
  };

  /**
   * Initialize footer.
   */
  NavigationLine.prototype.initFooter = function () {
    var that = this;
    var $footer = this.cp.$footer;

    // Inner footer adjustment containers
    var $leftFooter = $('<div/>', {
      'class': 'h5p-footer-left-adjusted'
    }).appendTo($footer);

    var $rightFooter = $('<div/>', {
      'class': 'h5p-footer-right-adjusted'
    }).appendTo($footer);

    var $centerFooter = $('<div/>', {
      'class': 'h5p-footer-center-adjusted'
    }).appendTo($footer);

    // Left footer elements

    // Toggle keywords menu
    //h5pcustomization
    this.cp.$keywordsButton = $('<div/>');
   /* this.cp.$keywordsButton = $('<div/>', {
      'class': "h5p-footer-button h5p-footer-toggle-keywords",
      'title': this.cp.l10n.showKeywords,
      'role': 'button',
      'tabindex': '0'
    }).click(function (event) {
      if (!that.cp.presentation.keywordListAlwaysShow) {
        that.cp.toggleKeywords();
        event.stopPropagation();
      }
    }).keydown(function (e) { // Trigger the click event from the keyboard
      var code = e.which;
      // 32 = Space
      if (code === 32) {
        $(this).click();
        e.preventDefault();
      }
      $(this).focus();
    }).appendTo($leftFooter);
    */

    if (this.cp.presentation.keywordListAlwaysShow || !this.cp.initKeywords) {
      this.cp.$keywordsButton.hide();
    }

    if (!this.cp.presentation.keywordListEnabled) {
      // Hide in editor when disabled.
      this.cp.$keywordsWrapper.add(this.$keywordsButton).hide();
    }

    // Update keyword for first slide.
    this.updateFooterKeyword(0);

    // Center footer elements

    // Previous slide
    $('<div/>', {
      'class': 'h5p-footer-button h5p-footer-previous-slide',
      'title': this.cp.l10n.prevSlide,
      'role': 'button',
      'tabindex': '0'
    }).click(function () {
    
      that.cp.previousSlide();
		
		initiateClickEventVideo();


    }).keydown(function (e) { // Trigger the click event from the keyboard
      var code = e.which;
      // 32 = Space
      if (code === 32) {
        $(this).click();
        e.preventDefault();
      }
      $(this).focus();
    }).appendTo($centerFooter);

    // Current slide count
    this.cp.$footerCurrentSlide = $('<div/>', {
      'html': '1',
      'class': 'h5p-footer-slide-count-current',
      'title': this.cp.l10n.currentSlide
    }).appendTo($centerFooter);

    // Count delimiter, content configurable in css
    $('<div/>', {
      'html': '/',
      'class': 'h5p-footer-slide-count-delimiter'
    }).appendTo($centerFooter);

    // Max slide count
    this.cp.$footerMaxSlide = $('<div/>', {
      'html': this.cp.slides.length,
      'class': 'h5p-footer-slide-count-max',
      'title': this.cp.l10n.lastSlide
    }).appendTo($centerFooter);

    // Next slide
    $('<div/>', {
      'class': 'h5p-footer-button h5p-footer-next-slide',
      'title': this.cp.l10n.nextSlide,
      'role': 'button',
      'tabindex': '0'
    }).click(function () {
      that.cp.nextSlide();
      
	initiateClickEventVideo();
      
    }).keydown(function (e) { // Trigger the click event from the keyboard
      var code = e.which;
      // 32 = Space
      if (code === 32) {
        $(this).click();
        e.preventDefault();
      }
      $(this).focus();
    }).appendTo($centerFooter);

    // *********************
    // Right footer elements
    // *********************

    // Do not add these buttons in editor mode
    if (this.cp.editor === undefined) {

      // Exit solution mode button
      this.cp.$exitSolutionModeButton = $('<div/>', {
        'class': 'h5p-footer-exit-solution-mode',
        'title': this.cp.l10n.solutionModeTitle,
        'tabindex': '0'
      }).click(function (event) {
        that.cp.jumpToSlide(that.cp.slides.length - 1);
        event.preventDefault();
      }).keydown(function (e) { // Trigger the click event from the keyboard
        var code = e.which;
        // 32 = Space
        if (code === 32) {
          $(this).click();
          e.preventDefault();
        }
        $(this).focus();
      }).appendTo($rightFooter);

      // Print button
      if (H5P.CoursePresentation.Printer.supported()) {
        this.cp.$printButton = $('<div/>', {
          'class': 'h5p-footer-button h5p-footer-print',
          'title': this.cp.l10n.printTitle,
          'role': 'button',
          'tabindex': '0'
        }).click(function () {
          var $h5pWrapper = $('.h5p-wrapper');

          H5P.CoursePresentation.Printer.showDialog(that.cp.l10n, $h5pWrapper, function (printAllslides) {
            H5P.CoursePresentation.Printer.print(that.cp, $h5pWrapper, printAllslides);
          });
        });
        this.cp.$printButton.appendTo($rightFooter);
      }

      if (H5P.canHasFullScreen) {
        // Toggle full screen button
        this.cp.$fullScreenButton = $('<div/>', {
          'class': 'h5p-footer-button h5p-footer-toggle-full-screen',
          'title': this.cp.l10n.fullscreen,
          'role': 'button',
          'tabindex': '0'
        }).click(function () {
          that.cp.toggleFullScreen();
        }).keydown(function (e) { // Trigger the click event from the keyboard
          var code = e.which;
          // 32 = Space
          if (code === 32) {
            $(this).click();
            e.preventDefault();
          }
          $(this).focus();
        });

        this.cp.$fullScreenButton.appendTo($rightFooter);
      }
    }

    // Solution mode text
    this.cp.$exitSolutionModeText = $('<div/>', {
      'html': '',
      'class': 'h5p-footer-exit-solution-mode-text'
    }).appendTo(this.cp.$exitSolutionModeButton);
  };

  /**
   * Updates progress bar.
   */
  NavigationLine.prototype.updateProgressBar = function (slideNumber, prevSlideNumber, solutionMode) {
    var that = this;

    // Updates progress bar progress (blue line)
    var i;
    for (i = 0; i < that.cp.progressbarParts.length; i += 1) {
      if (slideNumber + 1 > i) {
        that.cp.progressbarParts[i].addClass('h5p-progressbar-part-show');
      } else {
        that.cp.progressbarParts[i].removeClass('h5p-progressbar-part-show');
      }
    }

    if (prevSlideNumber === undefined) {
      that.cp.progressbarParts.forEach(function (pbPart) {
        pbPart.children('.h5p-progressbar-part-has-task').removeClass('h5p-answered');
      });
      return;
    }
    // Don't mark answers as answered if in solution mode or editor mode.
    if (solutionMode || (that.cp.editor !== undefined)) {
      return;
    }
  };

  /**
   * Update progress bar task at provided slide number
   * @param {Number} slideNumber Slide number which will be updated
   */
  NavigationLine.prototype.updateProgressBarTasksAtSlideNumber = function (slideNumber) {
    var that = this;

    // Updates previous slide answer.
    var answered = true;
    if (this.cp.slidesWithSolutions[slideNumber] !== undefined && this.cp.slidesWithSolutions[slideNumber]) {
      this.cp.slidesWithSolutions[slideNumber].forEach(function (slideTask) {
        if (slideTask.getAnswerGiven !== undefined) {
          if (!slideTask.getAnswerGiven()) {
            answered = false;
          }
        }
      });
    }

    if (answered) {
      that.cp.progressbarParts[slideNumber]
        .children('.h5p-progressbar-part-has-task')
        .addClass('h5p-answered');
    }
  };

  /**
   * Update footer with current slide data
   *
   * @param {Number} slideNumber Current slide number
   */
  NavigationLine.prototype.updateFooter = function (slideNumber) {

    // Update current slide number in footer
    this.cp.$footerCurrentSlide.html(slideNumber + 1);
    this.cp.$footerMaxSlide.html(this.cp.slides.length);

    // Hide exit solution mode button on summary slide
    if (this.cp.isSolutionMode && slideNumber === this.cp.slides.length - 1) {
      this.cp.$footer.addClass('summary-slide');
    } else {
      this.cp.$footer.removeClass('summary-slide');
    }

    // Update keyword in footer
    this.updateFooterKeyword(slideNumber);
  };

  /**
   * Update keyword in footer with current slide data
   *
   * @param {Number} slideNumber Current slide number
   */
  NavigationLine.prototype.updateFooterKeyword = function (slideNumber) {
    var keywordString = '';
    // Get current keyword
    if (this.cp.$currentKeyword !== undefined && this.cp.$currentKeyword) {
      keywordString = this.cp.$currentKeyword.find('span').html();
    }

    // Summary slide keyword
    if (this.cp.editor === undefined && this.cp.showSummarySlide) {
      if (slideNumber >= this.cp.slides.length - 1) {
        keywordString = this.cp.l10n.summary;
      }
    }

    // Empty string if no keyword defined
    if (keywordString === undefined) {
      keywordString = '';
    }

    // Set footer keyword
    this.cp.$keywordsButton.html(keywordString);
  };

  return NavigationLine;
})(H5P.jQuery);
;H5P.CoursePresentation.SlideBackground = (function ($) {
  /**
   * Create a Slide specific background selector
   *
   * @class H5P.CoursePresentation.SlideBackground
   * @param {H5P.CoursePresentation} cp Course Presentation instance
   */
  function SlideBackground (cp) {
    var params = cp.presentation;

    // Extend defaults
    params = $.extend(true, {
      globalBackgroundSelector: {
        fillGlobalBackground: "",
        imageGlobalBackground: {}
      },
      slides: [
        {
          slideBackgroundSelector: {
            fillSlideBackground: "",
            imageSlideBackground: {}
          }
        }
      ]
    }, params);

    /**
     * Set global background
     * @private
     */
    var setGlobalBackground = function () {
      var globalSettings = params.globalBackgroundSelector;
      setBackground(globalSettings.fillGlobalBackground, globalSettings.imageGlobalBackground);
    };

    /**
     * Set single slide background
     * @private
     */
    var setSlideBackgrounds = function () {
      params.slides.forEach(function (slideParams, idx) {
        var bgParams = slideParams.slideBackgroundSelector;
        if (bgParams) {
          setBackground(bgParams.fillSlideBackground, bgParams.imageSlideBackground, idx);
        }
      });
    };

    /**
     * Set background of slide(s)
     *
     * @private
     * @param {Object} fillSettings Background color settings
     * @param {Object} imageSettings Image background settings
     * @param {number} [index] Optional target slide index, otherwise all slides.
     */
    var setBackground = function (fillSettings, imageSettings, index) {
      var $updateSlides = cp.$slidesWrapper.children().filter(':not(.h5p-summary-slide)');

      if (index !== undefined) {
        $updateSlides = $updateSlides.eq(index);
      }

      if (fillSettings && fillSettings !== "") {

        // Fill with background color
        $updateSlides.addClass('has-background')
          .css('background-image', '')
          .css('background-color', '#' + fillSettings);
      }
      else if (imageSettings && imageSettings.path) {

        // Fill with image
        $updateSlides.addClass('has-background')
          .css('background-color', '')
          .css('background-image', 'url(' + H5P.getPath(imageSettings.path, cp.contentId) + ')');
      }
    };

    // Set backgrounds
    setGlobalBackground();
    setSlideBackgrounds();
  }

  return SlideBackground;

})(H5P.jQuery);
;H5P.CoursePresentation.Printer = (function ($) {
  /**
   * Printer class
   * @class Printer
   */
  function Printer(){}

  /**
   * Check if printing is supported
   *
   * @method supported
   * @static
   * @return {boolean} True if supported, else false.
   */
  Printer.supported = function () {
    // Need window.print to be available
    return (typeof window.print === 'function');
  };

  /**
   * Do the actual printing
   *
   * @method print
   * @static
   * @param  {H5P.CoursePresentation} cp Reference to cp instance
   * @param  {H5P.jQuery} $wrapper  The CP dom wrapper
   * @param  {boolean} allSlides If true, all slides are printed. If false or
   *                             undefined, the currentSlide is printed.
   */
  Printer.print = function (cp, $wrapper, allSlides) {
    // Let CP know we are about to print
    cp.trigger('printing', {finished: false, allSlides: allSlides});

    // Find height of a slide:
    var $currentSlide = $('.h5p-slide.h5p-current');
    var slideHeight = $currentSlide.height();
    var slideWidth = $currentSlide.width();

    // Use 670px as width when printing. We can't use 100% percent, since user can
    // change between landscape and portrait without us ever knowing about it.
    // More info: http://stackoverflow.com/a/11084797/2797106
    var ratio = slideWidth/670;

    var $slides = $('.h5p-slide');

    $slides.css({
      height: slideHeight/ratio + 'px',
      width: '670px',
      fontSize: Math.floor(100/ratio) + '%'
    });

    var wrapperHeight = $wrapper.height();
    $wrapper.css('height', 'auto');

    // Let printer css know which slides to print:
    $slides.toggleClass('doprint', allSlides === true);
    $currentSlide.addClass('doprint');

    // Need timeout for some browsers.
    setTimeout(function () {
      // Do the actual printing of the iframe content
      window.print();

      // Reset CSS
      $slides.css({
        height: '',
        width: '',
        fontSize: ''
      });
      $wrapper.css('height', wrapperHeight+'px');

      // Let CP know we are finished printing
      cp.trigger('printing', {finished: true});
    }, 500);
  };

  /**
   * Show the print dialog. Wanted to use H5P.Dialog, but it does not support getting a jQuery object as the content
   *
   * @method showDialog
   * @param  {object}       texts    Translated texts
   * @param  {H5P.jQuery}   $element Dom object to insert dialog after
   * @param  {Function}     callback Function invoked when printing is done.
   */
  Printer.showDialog = function (texts, $element, callback) {
    var self = this;
    /*jshint multistr: true */
    var $dialog = $('<div class="h5p-popup-dialog h5p-print-dialog">\
                      <div class="h5p-inner">\
                        <h2>' + texts.printTitle + '</h2>\
                        <div class="h5p-scroll-content"></div>\
                        <div class="h5p-close" role="button" tabindex="1" title="' + H5P.t('close') + '">\
                      </div>\
                    </div>')
      .insertAfter($element)
      .click(function () {
        self.close();
      })
      .children('.h5p-inner')
      .click(function () {
          return false;
      })
      .find('.h5p-close')
      .click(function () {
        self.close();
      }).end().end();

    var $content = $dialog.find('.h5p-scroll-content');

    $content.append($('<div>', {
      'class': 'h5p-cp-print-ingress',
      html: texts.printIngress
    }));

    var $buttonAllSlides = H5P.JoubelUI.createButton({
      html: texts.printAllSlides,
      'class': 'h5p-cp-print-all-slides',
      click: function () {
        self.close();
        callback(true);
      }
    }).appendTo($content);

    var $buttonCurrentSlide = H5P.JoubelUI.createButton({
      html: texts.printCurrentSlide,
      'class': 'h5p-cp-print-current-slide',
      click: function () {
        self.close();
        callback(false);
      }
    }).appendTo($content);

    this.open = function () {
      setTimeout(function () {
        $dialog.addClass('h5p-open'); // Fade in
        // Triggering an event, in case something has to be done after dialog has been opened.
        H5P.jQuery(self).trigger('dialog-opened', [$dialog]);
      }, 1);
    };

    this.close = function () {
      $dialog.removeClass('h5p-open'); // Fade out
      setTimeout(function () {
        $dialog.remove();
      }, 200);
    };

    this.open();
  };

  return Printer;

})(H5P.jQuery);
