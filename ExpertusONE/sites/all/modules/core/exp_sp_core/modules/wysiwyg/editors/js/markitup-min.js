(function(a){Drupal.wysiwyg.editor.attach.markitup=function(b,d,c){a("#"+d.field,b).markItUp(c);a.each(c.markupSet,function(e){a("."+c.nameSpace+" ."+this.className+" a").css({backgroundImage:"url("+c.root+"sets/default/images/"+e+".png)"}).parents("li").css({backgroundImage:"none"});});};Drupal.wysiwyg.editor.detach.markitup=function(c,d,b){if(b=="serialize"){return;}if(typeof d!="undefined"){a("#"+d.field,c).markItUpRemove();}else{a(".markItUpEditor",c).markItUpRemove();}};Drupal.wysiwyg.editor.instance.markitup={insert:function(b){a.markItUp({replaceWith:b});},setContent:function(b){a("#"+this.field).val(b);},getContent:function(){return a("#"+this.field).val();}};})(jQuery);