(function(a){if(typeof a.cookie=="undefined"){throw"jsTree cookie: jQuery cookie plugin not included.";}a.extend(a.tree.plugins,{cookie:{defaults:{prefix:"",options:{expires:false,path:false,domain:false,secure:false},types:{selected:true,open:true},keep_selected:false,keep_opened:false},set_cookie:function(b){var c=a.extend(true,{},a.tree.plugins.cookie.defaults,this.settings.plugins.cookie);if(c.types[b]!==true){return false;}switch(b){case"selected":if(this.settings.rules.multiple!=false&&this.selected_arr.length>1){var e=Array();a.each(this.selected_arr,function(){if(this.attr("id")){e.push(this.attr("id"));}});e=e.join(",");}else{var e=this.selected?this.selected.attr("id"):false;}a.cookie(c.prefix+"selected",e,c.options);break;case"open":var d="";this.container.find("li.open").each(function(f){if(this.id){d+=this.id+",";}});a.cookie(c.prefix+"open",d.replace(/,$/ig,""),c.options);break;}},callbacks:{oninit:function(c){var d=a.extend(true,{},a.tree.plugins.cookie.defaults,this.settings.plugins.cookie);
var b=false;b=a.cookie(d.prefix+"open");if(b){b=b.split(",");if(d.keep_opened){this.settings.opened=a.unique(a.merge(b,this.settings.opened));}else{this.settings.opened=b;}}b=a.cookie(d.prefix+"selected");if(b){b=b.split(",");if(d.keep_selected){this.settings.selected=a.unique(a.merge(b,this.settings.opened));}else{this.settings.selected=b;}}},onchange:function(){a.tree.plugins.cookie.set_cookie.apply(this,["selected"]);},onopen:function(){a.tree.plugins.cookie.set_cookie.apply(this,["open"]);},onclose:function(){a.tree.plugins.cookie.set_cookie.apply(this,["open"]);},ondelete:function(){a.tree.plugins.cookie.set_cookie.apply(this,["open"]);},oncopy:function(){a.tree.plugins.cookie.set_cookie.apply(this,["open"]);},oncreate:function(){a.tree.plugins.cookie.set_cookie.apply(this,["open"]);},onmoved:function(){a.tree.plugins.cookie.set_cookie.apply(this,["open"]);}}}});})(jQuery);