(function(b){var a=null;b.fn.extend({searchGrid:function(c){c=b.extend({top:0,left:0,width:360,height:80,modal:false,drag:true,closeicon:"ico-close.gif",dirty:false,sField:"searchField",sValue:"searchString",sOper:"searchOper",processData:"",checkInput:false,beforeShowSearch:null,afterShowSearch:null,onInitializeSearch:null,closeAfterSearch:false,sopt:null},b.jgrid.search,c||{});return this.each(function(){var n=this;if(!n.grid){return;}if(!c.imgpath){c.imgpath=n.p.imgpath;}var d=b("table:first",n.grid.bDiv).attr("id");var j={themodal:"srchmod"+d,modalhead:"srchhead"+d,modalcontent:"srchcnt"+d};if(b("#"+j.themodal).html()!=null){if(b.isFunction("beforeShowSearch")){c.beforeShowSearch(b("#srchcnt"+d));}viewModal("#"+j.themodal,{modal:c.modal});if(b.isFunction("afterShowSearch")){c.afterShowSearch(b("#srchcnt"+d));}}else{var h=n.p.colModel;var s="<select id='snames' class='search'>";var t,o,p;for(var m=0;m<h.length;m++){t=h[m].name;p=(h[m].search===false)?false:true;if(h[m].editrules&&h[m].editrules.searchhidden===true){o=true;
}else{if(h[m].hidden===true){o=false;}else{o=true;}}if(t!=="cb"&&t!=="subgrid"&&p&&o===true){var e=(h[m].index)?h[m].index:t;s+="<option value='"+e+"'>"+n.p.colNames[m]+"</option>";}}s+="</select>";var q=c.sopt||["bw","eq","ne","lt","le","gt","ge","ew","cn"];var f="<select id='sopt' class='search'>";for(var m=0;m<q.length;m++){f+=q[m]=="eq"?"<option value='eq'>"+c.odata[0]+"</option>":"";f+=q[m]=="ne"?"<option value='ne'>"+c.odata[1]+"</option>":"";f+=q[m]=="lt"?"<option value='lt'>"+c.odata[2]+"</option>":"";f+=q[m]=="le"?"<option value='le'>"+c.odata[3]+"</option>":"";f+=q[m]=="gt"?"<option value='gt'>"+c.odata[4]+"</option>":"";f+=q[m]=="ge"?"<option value='ge'>"+c.odata[5]+"</option>":"";f+=q[m]=="bw"?"<option value='bw'>"+c.odata[6]+"</option>":"";f+=q[m]=="ew"?"<option value='ew'>"+c.odata[7]+"</option>":"";f+=q[m]=="cn"?"<option value='cn'>"+c.odata[8]+"</option>":"";}f+="</select>";var r="<input id='sval' class='search' type='text' size='20' maxlength='100'/>";var k="<input id='sbut' class='buttonsearch' type='button' value='"+c.Find+"'/>";
var l="<input id='sreset' class='buttonsearch' type='button' value='"+c.Reset+"'/>";var g=b("<table width='100%'><tbody><tr style='display:none' id='srcherr'><td colspan='5'></td></tr><tr><td>"+s+"</td><td>"+f+"</td><td>"+r+"</td><td>"+k+"</td><td>"+l+"</td></tr></tbody></table>");createModal(j,g,c,n.grid.hDiv,n.grid.hDiv);if(b.isFunction("onInitializeSearch")){c.onInitializeSearch(b("#srchcnt"+d));}if(b.isFunction("beforeShowSearch")){c.beforeShowSearch(b("#srchcnt"+d));}viewModal("#"+j.themodal,{modal:c.modal});if(b.isFunction("afterShowSearch")){c.afterShowSearch(b("#srchcnt"+d));}if(c.drag){DnRModal("#"+j.themodal,"#"+j.modalhead+" td.modaltext");}b("#sbut","#"+j.themodal).click(function(){if(b("#sval","#"+j.themodal).val()!=""){var w=[true,"",""];b("#srcherr >td","#srchcnt"+d).html("").hide();n.p.searchdata[c.sField]=b("option[selected]","#snames").val();n.p.searchdata[c.sOper]=b("option[selected]","#sopt").val();n.p.searchdata[c.sValue]=b("#sval","#"+j.modalcontent).val();if(c.checkInput){for(var v=0;
v<h.length;v++){var u=(h[v].index)?h[v].index:t;if(u==n.p.searchdata[c.sField]){break;}}w=checkValues(n.p.searchdata[c.sValue],v,n);}if(w[0]===true){n.p.search=true;if(c.dirty){b(".no-dirty-cell",n.p.pager).addClass("dirty-cell");}n.p.page=1;b(n).trigger("reloadGrid");if(c.closeAfterSearch===true){hideModal("#"+j.themodal);}}else{b("#srcherr >td","#srchcnt"+d).html(w[1]).show();}}});b("#sreset","#"+j.themodal).click(function(){if(n.p.search){b("#srcherr >td","#srchcnt"+d).html("").hide();n.p.search=false;n.p.searchdata={};n.p.page=1;b("#sval","#"+j.themodal).val("");if(c.dirty){b(".no-dirty-cell",n.p.pager).removeClass("dirty-cell");}b(n).trigger("reloadGrid");}});}});},editGridRow:function(c,d){d=b.extend({top:0,left:0,width:0,height:0,modal:false,drag:true,closeicon:"ico-close.gif",imgpath:"",url:null,mtype:"POST",closeAfterAdd:false,clearAfterAdd:true,closeAfterEdit:false,reloadAfterSubmit:true,onInitializeForm:null,beforeInitData:null,beforeShowForm:null,afterShowForm:null,beforeSubmit:null,afterSubmit:null,onclickSubmit:null,afterComplete:null,onclickPgButtons:null,afterclickPgButtons:null,editData:{},recreateForm:false,addedrow:"first"},b.jgrid.edit,d||{});
a=d;return this.each(function(){var t=this;if(!t.grid||!c){return;}if(!d.imgpath){d.imgpath=t.p.imgpath;}var m=b("table:first",t.grid.bDiv).attr("id");var f={themodal:"editmod"+m,modalhead:"edithd"+m,modalcontent:"editcnt"+m};var z=b.isFunction(a.beforeShowForm)?a.beforeShowForm:false;var k=b.isFunction(a.afterShowForm)?a.afterShowForm:false;var v=b.isFunction(a.beforeInitData)?a.beforeInitData:false;var q=b.isFunction(a.onInitializeForm)?a.onInitializeForm:false;if(c=="new"){c="_empty";d.caption=d.addCaption;}else{d.caption=d.editCaption;}var w="FrmGrid_"+m;var o="TblGrid_"+m;if(d.recreateForm===true&&b("#"+f.themodal).html()!=null){b("#"+f.themodal).remove();}if(b("#"+f.themodal).html()!=null){b(".modaltext","#"+f.modalhead).html(d.caption);b("#FormError","#"+o).hide();if(v){v(b("#"+w));}h(c,t);if(c=="_empty"){b("#pData, #nData","#"+o).hide();}else{b("#pData, #nData","#"+o).show();}if(z){z(b("#"+w));}viewModal("#"+f.themodal,{modal:d.modal});if(k){k(b("#"+w));}}else{var x=b("<form name='FormPost' id='"+w+"' class='FormGrid'></form>");
var g=b("<table id='"+o+"' class='EditTable' cellspacing='0' cellpading='0' border='0'><tbody></tbody></table>");b(x).append(g);b(g).append("<tr id='FormError' style='display:none'><td colspan='2'>&nbsp;</td></tr>");if(v){v(b("#"+w));}var s=i(c,t,g);var u=t.p.imgpath;var p="<img id='pData' src='"+u+t.p.previmg+"'/>";var r="<img id='nData' src='"+u+t.p.nextimg+"'/>";var n="<input id='sData' type='button' class='EditButton' value='"+d.bSubmit+"'/>";var y="<input id='cData' type='button'  class='EditButton' value='"+d.bCancel+"'/>";b(g).append("<tr id='Act_Buttons'><td class='navButton'>"+p+"&nbsp;"+r+"</td><td class='EditButton'>"+n+"&nbsp;"+y+"</td></tr>");createModal(f,x,d,t.grid.hDiv,t.grid.hDiv);if(q){q(b("#"+w));}if(d.drag){DnRModal("#"+f.themodal,"#"+f.modalhead+" td.modaltext");}if(c=="_empty"){b("#pData,#nData","#"+o).hide();}else{b("#pData,#nData","#"+o).show();}if(z){z(b("#"+w));}viewModal("#"+f.themodal,{modal:d.modal});if(k){k(b("#"+w));}b("#sData","#"+o).click(function(E){var D={},C=[true,"",""],F={};
b("#FormError","#"+o).hide();var B=0;b(".FormElement","#"+o).each(function(I){var H=true;switch(b(this).get(0).type){case"checkbox":if(b(this).attr("checked")){D[this.name]=b(this).val();}else{var G=b(this).attr("offval");D[this.name]=G;F[this.name]=G;}break;case"select-one":D[this.name]=b("option:selected",this).val();F[this.name]=b("option:selected",this).text();break;case"select-multiple":D[this.name]=b(this).val();var J=[];b("option:selected",this).each(function(K,L){J[K]=b(L).text();});F[this.name]=J.join(",");break;case"password":case"text":case"textarea":D[this.name]=b(this).val();C=checkValues(D[this.name],s[I],t);if(C[0]===false){H=false;}else{D[this.name]=htmlEncode(D[this.name]);}break;}B++;if(!H){return false;}});if(B==0){C[0]=false;C[1]=b.jgrid.errors.norecords;}if(b.isFunction(a.onclickSubmit)){a.editData=a.onclickSubmit(d)||{};}if(C[0]){if(b.isFunction(a.beforeSubmit)){C=a.beforeSubmit(D,b("#"+w));}}var A=a.url?a.url:t.p.editurl;if(C[0]){if(!A){C[0]=false;C[1]+=" "+b.jgrid.errors.nourl;
}}if(C[0]===false){b("#FormError>td","#"+o).html(C[1]);b("#FormError","#"+o).show();}else{if(!d.processing){d.processing=true;b("div.loading","#"+f.themodal).fadeIn("fast");b(this).attr("disabled",true);D.oper=D.id=="_empty"?"add":"edit";D=b.extend(D,a.editData);b.ajax({url:A,type:a.mtype,data:D,complete:function(H,G){if(G!="success"){C[0]=false;C[1]=G+" Status: "+H.statusText+" Error code: "+H.status;}else{if(b.isFunction(a.afterSubmit)){C=a.afterSubmit(H,D);}}if(C[0]===false){b("#FormError>td","#"+o).html(C[1]);b("#FormError","#"+o).show();}else{D=b.extend(D,F);if(D.id=="_empty"){if(!C[2]){C[2]=parseInt(b(t).getGridParam("records"))+1;}D.id=C[2];if(a.closeAfterAdd){if(a.reloadAfterSubmit){b(t).trigger("reloadGrid");}else{b(t).addRowData(C[2],D,d.addedrow);b(t).setSelection(C[2]);}hideModal("#"+f.themodal);}else{if(a.clearAfterAdd){if(a.reloadAfterSubmit){b(t).trigger("reloadGrid");}else{b(t).addRowData(C[2],D,d.addedrow);}b(".FormElement","#"+o).each(function(I){switch(b(this).get(0).type){case"checkbox":b(this).attr("checked",0);
break;case"select-one":case"select-multiple":b("option",this).attr("selected","");break;case"password":case"text":case"textarea":if(this.name=="id"){b(this).val("_empty");}else{b(this).val("");}break;}});}else{if(a.reloadAfterSubmit){b(t).trigger("reloadGrid");}else{b(t).addRowData(C[2],D,d.addedrow);}}}}else{if(a.reloadAfterSubmit){b(t).trigger("reloadGrid");if(!a.closeAfterEdit){b(t).setSelection(D.id);}}else{if(t.p.treeGrid===true){b(t).setTreeRow(D.id,D);}else{b(t).setRowData(D.id,D);}}if(a.closeAfterEdit){hideModal("#"+f.themodal);}}if(b.isFunction(a.afterComplete)){setTimeout(function(){a.afterComplete(H,D,b("#"+w));},500);}}d.processing=false;b("#sData","#"+o).attr("disabled",false);b("div.loading","#"+f.themodal).fadeOut("fast");}});}}E.stopPropagation();});b("#cData","#"+o).click(function(A){hideModal("#"+f.themodal);A.stopPropagation();});b("#nData","#"+o).click(function(A){b("#FormError","#"+o).hide();var B=e();B[0]=parseInt(B[0]);if(B[0]!=-1&&B[1][B[0]+1]){if(b.isFunction(d.onclickPgButtons)){d.onclickPgButtons("next",b("#"+w),B[1][B[0]]);
}h(B[1][B[0]+1],t);b(t).setSelection(B[1][B[0]+1]);if(b.isFunction(d.afterclickPgButtons)){d.afterclickPgButtons("next",b("#"+w),B[1][B[0]+1]);}j(B[0]+1,B[1].length-1);}return false;});b("#pData","#"+o).click(function(B){b("#FormError","#"+o).hide();var A=e();if(A[0]!=-1&&A[1][A[0]-1]){if(b.isFunction(d.onclickPgButtons)){d.onclickPgButtons("prev",b("#"+w),A[1][A[0]]);}h(A[1][A[0]-1],t);b(t).setSelection(A[1][A[0]-1]);if(b.isFunction(d.afterclickPgButtons)){d.afterclickPgButtons("prev",b("#"+w),A[1][A[0]-1]);}j(A[0]-1,A[1].length-1);}return false;});}var l=e();j(l[0],l[1].length-1);function j(C,D,B){var A=t.p.imgpath;if(C==0){b("#pData","#"+o).attr("src",A+"off-"+t.p.previmg);}else{b("#pData","#"+o).attr("src",A+t.p.previmg);}if(C==D){b("#nData","#"+o).attr("src",A+"off-"+t.p.nextimg);}else{b("#nData","#"+o).attr("src",A+t.p.nextimg);}}function e(){var B=b(t).getDataIDs();var A=b("#id_g","#"+o).val();var C=b.inArray(A,B);return[C,B];}function i(D,H,F){var N,I,C,K,A,E=0,G,M,B,J=[];b("#"+D+" td",H.grid.bDiv).each(function(Q){N=H.p.colModel[Q].name;
if(H.p.colModel[Q].editrules&&H.p.colModel[Q].editrules.edithidden==true){I=false;}else{I=H.p.colModel[Q].hidden===true?true:false;}M=I?"style='display:none'":"";if(N!=="cb"&&N!=="subgrid"&&H.p.colModel[Q].editable===true){if(N==H.p.ExpandColumn&&H.p.treeGrid===true){G=b(this).text();}else{try{G=b.unformat(this,{colModel:H.p.colModel[Q]},Q);}catch(O){G=b.htmlDecode(b(this).html());}}var P=b.extend(H.p.colModel[Q].editoptions||{},{id:N,name:N});if(!H.p.colModel[Q].edittype){H.p.colModel[Q].edittype="text";}B=createEl(H.p.colModel[Q].edittype,P,G);b(B).addClass("FormElement");C=b("<tr "+M+"></tr>").addClass("FormData").attr("id","tr_"+N);K=b("<td></td>").addClass("CaptionTD");A=b("<td></td>").addClass("DataTD");b(K).html(H.p.colNames[Q]+": ");b(A).append(B);C.append(K);C.append(A);if(F){b(F).append(C);}else{b(C).insertBefore("#Act_Buttons");}J[E]=Q;E++;}});if(E>0){var L=b("<tr class='FormData' style='display:none'><td class='CaptionTD'>&nbsp;</td><td class='DataTD'><input class='FormElement' id='id_g' type='text' name='id' value='"+D+"'/></td></tr>");
if(F){b(F).append(L);}else{b(L).insertBefore("#Act_Buttons");}}return J;}function h(D,E){var A,F,C=0,B;b("#"+D+" td",E.grid.bDiv).each(function(H){A=E.p.colModel[H].name;if(E.p.colModel[H].editrules&&E.p.colModel[H].editrules.edithidden===true){F=false;}else{F=E.p.colModel[H].hidden===true?true:false;}if(A!=="cb"&&A!=="subgrid"&&E.p.colModel[H].editable===true){if(A==E.p.ExpandColumn&&E.p.treeGrid===true){B=b(this).text();}else{try{B=b.unformat(this,{colModel:E.p.colModel[H]},H);}catch(G){B=b.htmlDecode(b(this).html());}}A=A.replace(".","\\.");switch(E.p.colModel[H].edittype){case"password":case"text":B=b.htmlDecode(B);b("#"+A,"#"+o).val(B);break;case"textarea":if(B=="&nbsp;"||B=="&#160;"){B="";}b("#"+A,"#"+o).val(B);break;case"select":b("#"+A+" option","#"+o).each(function(I){if(!E.p.colModel[H].editoptions.multiple&&B==b(this).text()){this.selected=true;}else{if(E.p.colModel[H].editoptions.multiple){if(b.inArray(b(this).text(),B.split(","))>-1){this.selected=true;}else{this.selected=false;
}}else{this.selected=false;}}});break;case"checkbox":if(B==b("#"+A,"#"+o).val()){b("#"+A,"#"+o).attr("checked",true);b("#"+A,"#"+o).attr("defaultChecked",true);}else{b("#"+A,"#"+o).attr("checked",false);b("#"+A,"#"+o).attr("defaultChecked","");}break;}if(F){b("#"+A,"#"+o).parents("tr:first").hide();}C++;}});if(C>0){b("#id_g","#"+o).val(D);}else{b("#id_g","#"+o).val("");}return C;}});},delGridRow:function(c,d){d=b.extend({top:0,left:0,width:240,height:90,modal:false,drag:true,closeicon:"ico-close.gif",imgpath:"",url:"",mtype:"POST",reloadAfterSubmit:true,beforeShowForm:null,afterShowForm:null,beforeSubmit:null,onclickSubmit:null,afterSubmit:null,onclickSubmit:null,delData:{}},b.jgrid.del,d||{});return this.each(function(){var j=this;if(!j.grid){return;}if(!c){return;}if(!d.imgpath){d.imgpath=j.p.imgpath;}var l=typeof d.beforeShowForm==="function"?true:false;var f=typeof d.afterShowForm==="function"?true:false;if(isArray(c)){c=c.join();}var e=b("table:first",j.grid.bDiv).attr("id");var g={themodal:"delmod"+e,modalhead:"delhd"+e,modalcontent:"delcnt"+e};
var i="DelTbl_"+e;if(b("#"+g.themodal).html()!=null){b("#DelData>td","#"+i).text(c);b("#DelError","#"+i).hide();if(l){d.beforeShowForm(b("#"+i));}viewModal("#"+g.themodal,{modal:d.modal});if(f){d.afterShowForm(b("#"+i));}}else{var k=b("<table id='"+i+"' class='DelTable'><tbody></tbody></table>");b(k).append("<tr id='DelError' style='display:none'><td >&nbsp;</td></tr>");b(k).append("<tr id='DelData' style='display:none'><td >"+c+"</td></tr>");b(k).append("<tr><td >"+d.msg+"</td></tr>");var h="<input id='dData' type='button' value='"+d.bSubmit+"'/>";var m="<input id='eData' type='button' value='"+d.bCancel+"'/>";b(k).append("<tr><td class='DelButton'>"+h+"&nbsp;"+m+"</td></tr>");createModal(g,k,d,j.grid.hDiv,j.grid.hDiv);if(d.drag){DnRModal("#"+g.themodal,"#"+g.modalhead+" td.modaltext");}b("#dData","#"+i).click(function(q){var o=[true,""];var p=b("#DelData>td","#"+i).text();if(typeof d.onclickSubmit==="function"){d.delData=d.onclickSubmit(d)||{};}if(typeof d.beforeSubmit==="function"){o=d.beforeSubmit(p);
}var n=d.url?d.url:j.p.editurl;if(!n){o[0]=false;o[1]+=" "+b.jgrid.errors.nourl;}if(o[0]===false){b("#DelError>td","#"+i).html(o[1]);b("#DelError","#"+i).show();}else{if(!d.processing){d.processing=true;b("div.loading","#"+g.themodal).fadeIn("fast");b(this).attr("disabled",true);var r=b.extend({oper:"del",id:p},d.delData);b.ajax({url:n,type:d.mtype,data:r,complete:function(v,t){if(t!="success"){o[0]=false;o[1]=t+" Status: "+v.statusText+" Error code: "+v.status;}else{if(typeof d.afterSubmit==="function"){o=d.afterSubmit(v,p);}}if(o[0]===false){b("#DelError>td","#"+i).html(o[1]);b("#DelError","#"+i).show();}else{if(d.reloadAfterSubmit){if(j.p.treeGrid){b(j).setGridParam({treeANode:0,datatype:j.p.treedatatype});}b(j).trigger("reloadGrid");}else{var s=[];s=p.split(",");if(j.p.treeGrid===true){try{b(j).delTreeNode(s[0]);}catch(w){}}else{for(var u=0;u<s.length;u++){b(j).delRowData(s[u]);}}j.p.selrow=null;j.p.selarrrow=[];}if(b.isFunction(d.afterComplete)){setTimeout(function(){d.afterComplete(v,p);
},500);}}d.processing=false;b("#dData","#"+i).attr("disabled",false);b("div.loading","#"+g.themodal).fadeOut("fast");if(o[0]){hideModal("#"+g.themodal);}}});}}return false;});b("#eData","#"+i).click(function(n){hideModal("#"+g.themodal);return false;});if(l){d.beforeShowForm(b("#"+i));}viewModal("#"+g.themodal,{modal:d.modal});if(f){d.afterShowForm(b("#"+i));}}});},navGrid:function(f,h,e,g,d,c){h=b.extend({edit:true,editicon:"row_edit.gif",add:true,addicon:"row_add.gif",del:true,delicon:"row_delete.gif",search:true,searchicon:"find.gif",refresh:true,refreshicon:"refresh.gif",refreshstate:"firstpage",position:"left",closeicon:"ico-close.gif"},b.jgrid.nav,h||{});return this.each(function(){var k={themodal:"alertmod",modalhead:"alerthd",modalcontent:"alertcnt"};var n=this;if(!n.grid){return;}if(b("#"+k.themodal).html()==null){var m;var p;if(typeof window.innerWidth!="undefined"){m=window.innerWidth,p=window.innerHeight;}else{if(typeof document.documentElement!="undefined"&&typeof document.documentElement.clientWidth!="undefined"&&document.documentElement.clientWidth!=0){m=document.documentElement.clientWidth,p=document.documentElement.clientHeight;
}else{m=1024;p=768;}}createModal(k,"<div>"+h.alerttext+"</div>",{imgpath:n.p.imgpath,closeicon:h.closeicon,caption:h.alertcap,top:p/2-25,left:m/2-100,width:200,height:50},n.grid.hDiv,n.grid.hDiv,true);DnRModal("#"+k.themodal,"#"+k.modalhead);}var q=b("<table cellspacing='0' cellpadding='0' border='0' class='navtable'><tbody></tbody></table>").height(20);var l=document.createElement("tr");b(l).addClass("nav-row");var i=n.p.imgpath;var o;if(h.add){o=document.createElement("td");b(o).append("&nbsp;").css({border:"none",padding:"0px"});l.appendChild(o);o=document.createElement("td");o.title=h.addtitle||"";b(o).append("<table cellspacing='0' cellpadding='0' border='0' class='tbutton'><tr><td><img src='"+i+h.addicon+"'/></td><td>"+h.addtext+"&nbsp;</td></tr></table>").css("cursor","pointer").addClass("nav-button").click(function(){if(typeof h.addfunc=="function"){h.addfunc();}else{b(n).editGridRow("new",g||{});}return false;}).hover(function(){b(this).addClass("nav-hover");},function(){b(this).removeClass("nav-hover");
});l.appendChild(o);o=null;}if(h.edit){o=document.createElement("td");b(o).append("&nbsp;").css({border:"none",padding:"0px"});l.appendChild(o);o=document.createElement("td");o.title=h.edittitle||"";b(o).append("<table cellspacing='0' cellpadding='0' border='0' class='tbutton'><tr><td><img src='"+i+h.editicon+"'/></td><td valign='center'>"+h.edittext+"&nbsp;</td></tr></table>").css("cursor","pointer").addClass("nav-button").click(function(){var r=b(n).getGridParam("selrow");if(r){if(typeof h.editfunc=="function"){h.editfunc(r);}else{b(n).editGridRow(r,e||{});}}else{viewModal("#"+k.themodal);}return false;}).hover(function(){b(this).addClass("nav-hover");},function(){b(this).removeClass("nav-hover");});l.appendChild(o);o=null;}if(h.del){o=document.createElement("td");b(o).append("&nbsp;").css({border:"none",padding:"0px"});l.appendChild(o);o=document.createElement("td");o.title=h.deltitle||"";b(o).append("<table cellspacing='0' cellpadding='0' border='0' class='tbutton'><tr><td><img src='"+i+h.delicon+"'/></td><td>"+h.deltext+"&nbsp;</td></tr></table>").css("cursor","pointer").addClass("nav-button").click(function(){var r;
if(n.p.multiselect){r=b(n).getGridParam("selarrrow");if(r.length==0){r=null;}}else{r=b(n).getGridParam("selrow");}if(r){b(n).delGridRow(r,d||{});}else{viewModal("#"+k.themodal);}return false;}).hover(function(){b(this).addClass("nav-hover");},function(){b(this).removeClass("nav-hover");});l.appendChild(o);o=null;}if(h.search){o=document.createElement("td");b(o).append("&nbsp;").css({border:"none",padding:"0px"});l.appendChild(o);o=document.createElement("td");if(b(f)[0]==n.p.pager[0]){c=b.extend(c,{dirty:true});}o.title=h.searchtitle||"";b(o).append("<table cellspacing='0' cellpadding='0' border='0' class='tbutton'><tr><td class='no-dirty-cell'><img src='"+i+h.searchicon+"'/></td><td>"+h.searchtext+"&nbsp;</td></tr></table>").css({cursor:"pointer"}).addClass("nav-button").click(function(){b(n).searchGrid(c||{});return false;}).hover(function(){b(this).addClass("nav-hover");},function(){b(this).removeClass("nav-hover");});l.appendChild(o);o=null;}if(h.refresh){o=document.createElement("td");
b(o).append("&nbsp;").css({border:"none",padding:"0px"});l.appendChild(o);o=document.createElement("td");o.title=h.refreshtitle||"";var j=(b(f)[0]==n.p.pager[0])?true:false;b(o).append("<table cellspacing='0' cellpadding='0' border='0' class='tbutton'><tr><td><img src='"+i+h.refreshicon+"'/></td><td>"+h.refreshtext+"&nbsp;</td></tr></table>").css("cursor","pointer").addClass("nav-button").click(function(){n.p.search=false;switch(h.refreshstate){case"firstpage":n.p.page=1;b(n).trigger("reloadGrid");break;case"current":var r=n.p.multiselect===true?selarrrow:n.p.selrow;b(n).setGridParam({gridComplete:function(){if(n.p.multiselect===true){if(r.length>0){for(var t=0;t<r.length;t++){b(n).setSelection(r[t]);}}}else{if(r){b(n).setSelection(r);}}}});b(n).trigger("reloadGrid");break;}if(j){b(".no-dirty-cell",n.p.pager).removeClass("dirty-cell");}if(h.search){var s=b("table:first",n.grid.bDiv).attr("id");b("#sval","#srchcnt"+s).val("");}return false;}).hover(function(){b(this).addClass("nav-hover");
},function(){b(this).removeClass("nav-hover");});l.appendChild(o);o=null;}if(h.position=="left"){b(q).append(l).addClass("nav-table-left");}else{b(q).append(l).addClass("nav-table-right");}b(f).prepend(q);});},navButtonAdd:function(c,d){d=b.extend({caption:"newButton",title:"",buttonimg:"",onClickButton:null,position:"last"},d||{});return this.each(function(){if(!this.grid){return;}if(c.indexOf("#")!=0){c="#"+c;}var i=b(".navtable",c)[0];if(i){var e,h;var h=document.createElement("td");b(h).append("&nbsp;").css({border:"none",padding:"0px"});var g=b("tr:eq(0)",i)[0];if(d.position!="first"){g.appendChild(h);}tbd=document.createElement("td");tbd.title=d.title;var f=(d.buttonimg)?"<img src='"+d.buttonimg+"'/>":"&nbsp;";b(tbd).append("<table cellspacing='0' cellpadding='0' border='0' class='tbutton'><tr><td>"+f+"</td><td>"+d.caption+"&nbsp;</td></tr></table>").css("cursor","pointer").addClass("nav-button").click(function(j){if(typeof d.onClickButton=="function"){d.onClickButton();}j.stopPropagation();
return false;}).hover(function(){b(this).addClass("nav-hover");},function(){b(this).removeClass("nav-hover");});if(d.position!="first"){g.appendChild(tbd);}else{b(g).prepend(tbd);b(g).prepend(h);}tbd=null;h=null;}});},GridToForm:function(c,d){return this.each(function(){var g=this;if(!g.grid){return;}var f=b(g).getRowData(c);if(f){for(var e in f){if(b("[name="+e+"]",d).is("input:radio")){b("[name="+e+"]",d).each(function(){if(b(this).val()==f[e]){b(this).attr("checked","checked");}else{b(this).attr("checked","");}});}else{b("[name="+e+"]",d).val(f[e]);}}}});},FormToGrid:function(c,d){return this.each(function(){var g=this;if(!g.grid){return;}var e=b(d).serializeArray();var f={};b.each(e,function(h,j){f[j.name]=j.value;});b(g).setRowData(c,f);});}});})(jQuery);