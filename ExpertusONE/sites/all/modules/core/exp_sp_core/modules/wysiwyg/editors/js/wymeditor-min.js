(function(a){Drupal.wysiwyg.editor.attach.wymeditor=function(b,d,c){c.wymPath=c.basePath+c.wymPath;c.postInit=function(e){a(e._doc).focus(function(){Drupal.wysiwyg.activeId=d.field;});};a("#"+d.field).wymeditor(c);};Drupal.wysiwyg.editor.detach.wymeditor=function(e,g,d){if(typeof g!="undefined"){var f=a("#"+g.field);var c=f.data(WYMeditor.WYM_INDEX);if(typeof c!="undefined"){var b=WYMeditor.INSTANCES[c];b.update();if(d!="serialize"){a(b._box).remove();a(b._element).show();delete b;}}if(d!="serialize"){f.show();}}else{jQuery.each(WYMeditor.INSTANCES,function(){this.update();if(d!="serialize"){a(this._box).remove();a(this._element).show();delete this;}});}};Drupal.wysiwyg.editor.instance.wymeditor={insert:function(b){this.getInstance().insert(b);},setContent:function(b){this.getInstance().html(b);},getContent:function(){return this.getInstance().xhtml();},getInstance:function(){var c=a("#"+this.field);var b=c.data(WYMeditor.WYM_INDEX);if(typeof b!="undefined"){return WYMeditor.INSTANCES[b];
}return null;}};})(jQuery);