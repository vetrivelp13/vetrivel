(function(a){if(typeof a.metadata=="undefined"){throw"jsTree metadata: jQuery metadata plugin not included.";}a.extend(a.tree.plugins,{metadata:{defaults:{attribute:"data"},callbacks:{check:function(f,e,d,b){var c=a.extend(true,{},a.tree.plugins.metadata.defaults,this.settings.plugins.metadata);if(typeof a(e).metadata({type:"attr",name:c.attribute})[f]!="undefined"){return a(e).metadata()[f];}}}}});})(jQuery);