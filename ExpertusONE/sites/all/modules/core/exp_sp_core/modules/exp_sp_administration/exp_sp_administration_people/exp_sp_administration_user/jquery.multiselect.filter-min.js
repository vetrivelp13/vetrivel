(function(a){var b=/[\-\[\]{}()*+?.,\\\^$|#\s]/g;a.widget("ech.multiselectfilter",{options:{label:"Filter:",width:null,placeholder:"Enter keywords",autoReset:false},_create:function(){var d=this,e=this.options,c=(this.instance=a(this.element).data("multiselect")),h=(this.header=c.menu.find(".ui-multiselect-header").addClass("ui-multiselect-hasfilter")),g=(this.wrapper=a('<div class="ui-multiselect-filter">'+(e.label.length?e.label:"")+'<input placeholder="'+e.placeholder+'" type="search"'+(/\d/.test(e.width)?'style="width:'+e.width+'px"':"")+" /></div>").prependTo(this.header));this.inputs=c.menu.find('input[type="checkbox"], input[type="radio"]');this.input=g.find("input").bind({keydown:function(i){if(i.which===13){i.preventDefault();}},keyup:a.proxy(d._handler,d),click:a.proxy(d._handler,d)});this.updateCache();c._toggleChecked=function(k,n){var m=(n&&n.length)?n:this.labels.find("input"),j=this,i=d.instance._isOpen?":disabled, :hidden":":disabled";m=m.not(i).each(this._toggleState("checked",k));
this.update();var l=m.map(function(){return this.value;}).get();this.element.find("option").filter(function(){if(!this.disabled&&a.inArray(this.value,l)>-1){j._toggleState("selected",k).call(this);}});};var f=a(document).bind("multiselectrefresh",function(){d.updateCache();d._handler();});if(this.options.autoReset){f.bind("multiselectclose",a.proxy(this._reset,this));}},_handler:function(i){var f=a.trim(this.input[0].value.toLowerCase()),h=this.rows,c=this.inputs,d=this.cache;if(!f){h.show();}else{h.hide();var g=new RegExp(f.replace(b,"\\$&"),"gi");this._trigger("filter",i,a.map(d,function(e,j){if(e.search(g)!==-1){h.eq(j).show();return c.get(j);}return null;}));}this.instance.menu.find(".ui-multiselect-optgroup-label").each(function(){var j=a(this);var e=j.nextUntil(".ui-multiselect-optgroup-label").filter(function(){return a.css(this,"display")!=="none";}).length;j[e?"show":"hide"]();});},_reset:function(){this.input.val("").trigger("keyup");},updateCache:function(){this.rows=this.instance.menu.find(".ui-multiselect-checkboxes li:not(.ui-multiselect-optgroup-label)");
this.cache=this.element.children().map(function(){var c=a(this);if(this.tagName.toLowerCase()==="optgroup"){c=c.children();}return c.map(function(){return this.innerHTML.toLowerCase();}).get();}).get();},widget:function(){return this.wrapper;},destroy:function(){a.Widget.prototype.destroy.call(this);this.input.val("").trigger("keyup");this.wrapper.remove();}});})(jQuery);