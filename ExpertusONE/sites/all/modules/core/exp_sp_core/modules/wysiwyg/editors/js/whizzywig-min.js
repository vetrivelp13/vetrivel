var wysiwygWhizzywig={currentField:null,fields:{}};var buttonPath=null;var w=function(a){if(a){wysiwygWhizzywig.fields[wysiwygWhizzywig.currentField]+=a;}return wysiwygWhizzywig.fields[wysiwygWhizzywig.currentField];};var o=function(a){if(a=="whizzy"+wysiwygWhizzywig.currentField&&wysiwygWhizzywig.fields[wysiwygWhizzywig.currentField]){jQuery("#"+wysiwygWhizzywig.currentField).after('<div id="'+wysiwygWhizzywig.currentField+'-whizzywig"></div>');jQuery("#"+wysiwygWhizzywig.currentField+"-whizzywig").html(w());wysiwygWhizzywig.fields[wysiwygWhizzywig.currentField]="";}if(jQuery("#"+a).size()){return jQuery("#"+a).get(0);}return jQuery("#"+a,w()).get(0);};(function(a){Drupal.wysiwyg.editor.attach.whizzywig=function(b,e,c){if(c.buttonPath){window.buttonPath=c.buttonPath;}wysiwygWhizzywig.currentField=e.field;wysiwygWhizzywig.fields[wysiwygWhizzywig.currentField]="";$field=a("#"+e.field);var d=Drupal.wysiwyg.instances[e.field];d.originalStyle=$field.attr("style");$field.css("width",$field.width()+"px");
makeWhizzyWig(e.field,(c.buttons?c.buttons:"all"));a("#whizzy"+e.field).contents().find("body").html(tidyD($field.val()));};Drupal.wysiwyg.editor.detach.whizzywig=function(d,f,b){var e=function(h){var j=whizzies[h],i=a("#"+j),g=Drupal.wysiwyg.instances[j];i.val(g.getContent());if(b=="serialize"){return;}a("#"+j+"-whizzywig").remove();whizzies.splice(h,1);i.removeAttr("style").attr("style",g.originalStyle);};if(typeof f!="undefined"){for(var c=0;c<whizzies.length;c++){if(whizzies[c]==f.field){e(c);break;}}}else{while(whizzies.length>0){e(0);}}};Drupal.wysiwyg.editor.instance.whizzywig={insert:function(b){insHTML(b.replace(/^js:/,"js&colon;"));},setContent:function(c){var b=a("#"+this.field);if(b.css("display")=="block"){b.val(c);}else{var d=a("#whizzy"+this.field).contents()[0];d.open();d.write(c);d.close();}},getContent:function(){var b=a("#"+this.field),c=(b.css("display")=="block"?b.val():a("#whizzy"+this.field).contents().find("body").html());c=tidyH(c);if(a.isFunction(window.get_xhtml)){var d=document.createElement("pre");
d.innerHTML=c;c=get_xhtml(d);}return c.replace(location.href+"#","#");}};})(jQuery);