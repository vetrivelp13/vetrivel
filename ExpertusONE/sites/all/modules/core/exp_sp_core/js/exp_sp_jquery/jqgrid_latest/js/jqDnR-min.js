(function(g){g.fn.jqDrag=function(f){return c(this,f,"d");};g.fn.jqResize=function(i,f){return c(this,i,"r",f);};g.jqDnR={dnr:{},e:0,drag:function(f){if(h.k=="d"){e.css({left:h.X+f.pageX-h.pX,top:h.Y+f.pageY-h.pY});}else{e.css({width:Math.max(f.pageX-h.pX+h.W,0),height:Math.max(f.pageY-h.pY+h.H,0)});if(M1){a.css({width:Math.max(f.pageX-M1.pX+M1.W,0),height:Math.max(f.pageY-M1.pY+M1.H,0)});}}return false;},stop:function(){g(document).unbind("mousemove",b.drag).unbind("mouseup",b.stop);}};var b=g.jqDnR,h=b.dnr,e=b.e,a,c=function(l,j,i,f){return l.each(function(){j=(j)?g(j,l):l;j.bind("mousedown",{e:l,k:i},function(k){var q=k.data,o={};e=q.e;a=f?g(f):false;if(e.css("position")!="relative"){try{e.position(o);}catch(n){}}h={X:o.left||d("left")||0,Y:o.top||d("top")||0,W:d("width")||e[0].scrollWidth||0,H:d("height")||e[0].scrollHeight||0,pX:k.pageX,pY:k.pageY,k:q.k};if(a&&q.k!="d"){M1={X:o.left||f1("left")||0,Y:o.top||f1("top")||0,W:a[0].offsetWidth||f1("width")||0,H:a[0].offsetHeight||f1("height")||0,pX:k.pageX,pY:k.pageY,k:q.k};
}else{M1=false;}if(g("input.hasDatepicker",e[0])[0]){try{g("input.hasDatepicker",e[0]).datepicker("hide");}catch(m){}}g(document).mousemove(g.jqDnR.drag).mouseup(g.jqDnR.stop);return false;});});},d=function(f){return parseInt(e.css(f))||false;};f1=function(f){return parseInt(a.css(f))||false;};})(jQuery);