(function(a){a.extend(a.tree.plugins,{checkbox:{defaults:{three_state:true},get_checked:function(b){if(!b){b=a.tree.focused();}return b.container.find("a.checked").parent();},get_undeterminded:function(b){if(!b){b=a.tree.focused();}return b.container.find("a.undetermined").parent();},get_unchecked:function(b){if(!b){b=a.tree.focused();}return b.container.find("a:not(.checked, .undetermined)").parent();},check:function(d){if(!d){return false;}var b=a.tree.reference(d);d=b.get_node(d);if(d.children("a").hasClass("checked")){return true;}var c=a.extend(true,{},a.tree.plugins.checkbox.defaults,b.settings.plugins.checkbox);if(c.three_state){d.find("li").andSelf().children("a").removeClass("unchecked undetermined").addClass("checked");d.parents("li").each(function(){if(a(this).children("ul").find("a:not(.checked):eq(0)").size()>0){a(this).parents("li").andSelf().children("a").removeClass("unchecked checked").addClass("undetermined");return false;}else{a(this).children("a").removeClass("unchecked undetermined").addClass("checked");
}});}else{d.children("a").removeClass("unchecked").addClass("checked");}return true;},uncheck:function(d){if(!d){return false;}var b=a.tree.reference(d);d=b.get_node(d);if(d.children("a").hasClass("unchecked")){return true;}var c=a.extend(true,{},a.tree.plugins.checkbox.defaults,b.settings.plugins.checkbox);if(c.three_state){d.find("li").andSelf().children("a").removeClass("checked undetermined").addClass("unchecked");d.parents("li").each(function(){if(a(this).find("a.checked, a.undetermined").size()-1>0){a(this).parents("li").andSelf().children("a").removeClass("unchecked checked").addClass("undetermined");return false;}else{a(this).children("a").removeClass("checked undetermined").addClass("unchecked");}});}else{d.children("a").removeClass("checked").addClass("unchecked");}return true;},toggle:function(c){if(!c){return false;}var b=a.tree.reference(c);c=b.get_node(c);if(c.children("a").hasClass("checked")){a.tree.plugins.checkbox.uncheck(c);}else{a.tree.plugins.checkbox.check(c);}},callbacks:{onchange:function(c,b){a.tree.plugins.checkbox.toggle(c);
}}}});})(jQuery);