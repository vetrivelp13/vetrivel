(function(a){Drupal.behaviors.context_ui_dialog={attach:function(e){var c=a("#context_ui_dialog-context-ui",e).not("context_ui_dialog-processed");if(c){c.addClass("context_ui_dialog-processed");c.detach();a("#page").prepend(c);var b=Drupal.t("Select Context");var d=Drupal.t("Hide");var g=a('<a href="javascript:" class="context-ui-dialog-open" title="Show Context Selector">'+d+"</a>");c.append(g);c.toggled=false;var f=a(c).outerWidth();g.click(function(j){if(c.toggled){c.stop(true,false).animate({left:0},400);c.toggled=false;a(this).text(d);}else{c.stop(true,false).animate({left:-f-4},400);c.toggled=true;a(this).text(b);}});a("#context_ui_dialog-context-ui").show();var i=Math.round(6*a(window).height()/10);var h=i-200;h=(h<50)?50:h;a("#context_ui_dialog-context-ui").height(i);a("#context_ui_dialog-context-ui .item-list").height(h);a("body").once().addClass("context-field-editor");}}};})(jQuery);