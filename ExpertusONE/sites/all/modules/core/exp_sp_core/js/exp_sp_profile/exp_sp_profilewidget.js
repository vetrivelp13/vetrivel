registernamespace("EXPERTUS.SMARTPORTAL.ProfileManager");
EXPERTUS.SMARTPORTAL.ProfileManager=function(){
try{
	/*
	this.execute=function(obj){

	};
	*/
	this.generateCreateRequest = function(obj){
		try{
		var request=new Object();
		request.query = new SOAPObject("InsertProfile").attr("xsi:type","null");
		request.query.appendChild(new SOAPObject("list_id").attr("xsi:type","null").val(obj.list_id));
		request.query.appendChild(new SOAPObject("parent_id").attr("xsi:type","null").val(obj.parent_id));
		request.query.appendChild(new SOAPObject("level").attr("xsi:type","null").val(obj.level));
		request.query.appendChild(new SOAPObject("name").attr("xsi:type","null").val(obj.name));
		request.query.appendChild(new SOAPObject("root").attr("xsi:type","null").val(obj.root));
		if(obj.description!=undefined)
			request.query.appendChild(new SOAPObject("description").attr("xsi:type","null").val(obj.description));

		request.query.appendChild(new SOAPObject("code").attr("xsi:type","null").val(obj.code));
		request.query.appendChild(new SOAPObject("language_code").attr("xsi:type","null").val(obj.language_code));
		request.query.appendChild(new SOAPObject("attr1").attr("xsi:type","null").val(obj.attr1));
		request.query.appendChild(new SOAPObject("attr2").attr("xsi:type","null").val(obj.attr2));
		request.query.appendChild(new SOAPObject("attr3").attr("xsi:type","null").val(obj.attr3));
		request.query.appendChild(new SOAPObject("attr4").attr("xsi:type","null").val(obj.attr4));
		request.query.appendChild(new SOAPObject("type").attr("xsi:type","null").val(obj.type));
		request.query.appendChild(new SOAPObject("is_active").attr("xsi:type","null").val(obj.is_active));
		var sr = new SOAPRequest("InsertProfile", request);
		sr="<?xml version='1.0' encoding='UTF-8'?>"+sr;
		sr=sr.toString();
		return sr;
		}catch(e){
			// to do
		}
	};

	this.generateProfileRequest = function(obj){
		try{
		var request=new Object();
		request.query = new SOAPObject("GetProfile").attr("xsi:type","null");
		request.query.appendChild(new SOAPObject("dummy").attr("xsi:type","null").val("1"));
		var sr = new SOAPRequest("GetProfile", request);
		sr="<?xml version='1.0' encoding='UTF-8'?>"+sr;
		sr=sr.toString();
		return sr;
		}catch(e){
			// to do
		}
	};

	this.generateProfileAsyncRequest = function(obj){
		try{
		var request=new Object();
		request.query = new SOAPObject("GetProfileAsync").attr("xsi:type","null");
		request.query.appendChild(new SOAPObject("id").attr("xsi:type","null").val(obj.id));
		request.query.appendChild(new SOAPObject("level").attr("xsi:type","null").val(obj.level));
		request.query.appendChild(new SOAPObject("root").attr("xsi:type","null").val(obj.root));
		var sr = new SOAPRequest("GetProfileAsync", request);
		sr="<?xml version='1.0' encoding='UTF-8'?>"+sr;
		sr=sr.toString();
		return sr;
		}catch(e){
			// to do
		}
	};

	this.generateProfileDetsRequest = function(obj){
		try{
		var request=new Object();
		request.query = new SOAPObject("GetProfileDets").attr("xsi:type","null");
		request.query.appendChild(new SOAPObject("id").attr("xsi:type","null").val(obj.id));
		request.query.appendChild(new SOAPObject("level").attr("xsi:type","null").val(obj.level));
		request.query.appendChild(new SOAPObject("root").attr("xsi:type","null").val(obj.root));
		var sr = new SOAPRequest("GetProfile", request);
		sr="<?xml version='1.0' encoding='UTF-8'?>"+sr;
		sr=sr.toString();
		return sr;
		}catch(e){
			// to do
		}
	};

	this.generateRenameProfileRequest = function(obj){
		try{
		var request=new Object();
		request.query = new SOAPObject("RenameProfile").attr("xsi:type","null");
		request.query.appendChild(new SOAPObject("id").attr("xsi:type","null").val(obj.id));
		request.query.appendChild(new SOAPObject("newname").attr("xsi:type","null").val(obj.newname));
		request.query.appendChild(new SOAPObject("level").attr("xsi:type","null").val(obj.level));
		request.query.appendChild(new SOAPObject("root").attr("xsi:type","null").val(obj.root));
		var sr = new SOAPRequest("RenameProfile", request);
		sr="<?xml version='1.0' encoding='UTF-8'?>"+sr;
		sr=sr.toString();
		return sr;
		}catch(e){
			// to do
		}
	};

	this.generateDeleteProfileRequest = function(obj){
		try{
		var request=new Object();
		request.query = new SOAPObject("DeleteProfile").attr("xsi:type","null");
		request.query.appendChild(new SOAPObject("id").attr("xsi:type","null").val(obj.id));
		request.query.appendChild(new SOAPObject("level").attr("xsi:type","null").val(obj.level));
		request.query.appendChild(new SOAPObject("root").attr("xsi:type","null").val(obj.root));
		var sr = new SOAPRequest("DeleteProfile", request);
		sr="<?xml version='1.0' encoding='UTF-8'?>"+sr;
		sr=sr.toString();
		return sr;
		}catch(e){
			// to do
		}
	};
	this.generateUpdateProfileRequest = function(obj){
		try{
		var request=new Object();
		request.query = new SOAPObject("UpdateProfile").attr("xsi:type","null");
		request.query.appendChild(new SOAPObject("id").attr("xsi:type","null").val(obj.id));
		request.query.appendChild(new SOAPObject("name").attr("xsi:type","null").val(obj.name));
		request.query.appendChild(new SOAPObject("root").attr("xsi:type","null").val(obj.root));
		if(obj.description!=undefined)
			request.query.appendChild(new SOAPObject("description").attr("xsi:type","null").val(obj.description));
		request.query.appendChild(new SOAPObject("code").attr("xsi:type","null").val(obj.code));
		request.query.appendChild(new SOAPObject("language_code").attr("xsi:type","null").val(obj.language_code));
		request.query.appendChild(new SOAPObject("attr1").attr("xsi:type","null").val(obj.attr1));
		request.query.appendChild(new SOAPObject("attr2").attr("xsi:type","null").val(obj.attr2));
		request.query.appendChild(new SOAPObject("attr3").attr("xsi:type","null").val(obj.attr3));
		request.query.appendChild(new SOAPObject("attr4").attr("xsi:type","null").val(obj.attr4));
		request.query.appendChild(new SOAPObject("level").attr("xsi:type","null").val(obj.level));
		request.query.appendChild(new SOAPObject("type").attr("xsi:type","null").val(obj.type));
		request.query.appendChild(new SOAPObject("is_active").attr("xsi:type","null").val(obj.is_active));
		var sr = new SOAPRequest("UpdateProfile", request);
		sr="<?xml version='1.0' encoding='UTF-8'?>"+sr;
		sr=sr.toString();
		return sr;
		}catch(e){
			// to do
		}
	};

	this.generateProfileCodeRequest = function(obj){
		try{
		var request=new Object();
		request.query = new SOAPObject("GetProfileCode").attr("xsi:type","null");
		request.query.appendChild(new SOAPObject("id").attr("xsi:type","null").val(obj.parent_id));
		var sr = new SOAPRequest("GetProfileCode", request);
		sr="<?xml version='1.0' encoding='UTF-8'?>"+sr;
		sr=sr.toString();
		return sr;
		}catch(e){
			// to do
		}
	};

	this.generateProfileByTypeRequest = function(obj){
		try{
		var request=new Object();
		request.query = new SOAPObject("getProfileByType").attr("xsi:type","null");
		request.query.appendChild(new SOAPObject("type").attr("xsi:type","null").val(obj.type));
		var sr = new SOAPRequest("getProfileByType", request);
		sr="<?xml version='1.0' encoding='UTF-8'?>"+sr;
		sr=sr.toString();
		return sr;
		}catch(e){
			// to do
		}
	};

	this.exec = function(node){
		/*
		 ** comment for create sub tree

		$.tree.focused().lock(true);
		var img = "<img src='"+themepath+"/images/throbber.gif' id='throbber'/>";
		$(node).find("a:first").after(img);
		 */
		try{
		this.execute();
		}catch(e){
			// to do
		}
	};

	this.execute=function(loader){
		try{
		  	var widgetObj = this.getwidgetObject();
		  	if(loader)
		  		widgetObj.createLoader(widgetObj.getUniqueWidgetId());
		 // Implemented for Ajax Request failing in IE8
		  	jQuery.ajaxSetup({
	            xhr: function() {
	                    //return new window.XMLHttpRequest();
	                    try{
	                        if(window.ActiveXObject)
	                            return new window.ActiveXObject("Microsoft.XMLHTTP");
	                    } catch(e) { }

	                    return new window.XMLHttpRequest();
	                }
	        });
			jQuery.ajax({
			   type: "POST",
			   url: this.url,
			   processData: false,
			   data:  this.requestXml,
			   dataType:'text',
			   contentType:'text/xml',
			   async: true,
			   success: function(respText){
		  			widgetObj.responseText=widgetObj.Trim(respText);
		  			widgetObj.convertResponseXml();
		  			if(loader)
		  				widgetObj.destroyLoader(widgetObj.getUniqueWidgetId());
		  			widgetObj.renderResults();
		  		},
		  		failure:function(msg){
		  			widgetObj.responseText=widgetObj.Trim(msg);
		  			widgetObj.convertResponseXml();
		  			if(loader)
		  				widgetObj.destroyLoader(widgetObj.getUniqueWidgetId());
		  			widgetObj.renderError();
			   },error:function (XMLHttpRequest, textStatus, errorThrown) {
				   widgetObj.responseText=widgetObj.Trim(XMLHttpRequest.responseText);
				   widgetObj.convertResponseXml();
				   if(loader)
					   widgetObj.destroyLoader(widgetObj.getUniqueWidgetId());
				   widgetObj.renderError();
			   	 }
			 });
		}catch(e){
			// to do
		}
	  };
}catch(e){
	// to do
}
};

$.extend(EXPERTUS.SMARTPORTAL.ProfileManager.prototype,EXPERTUS.SMARTPORTAL.AbstractManager);


registernamespace("EXPERTUS.SMARTPORTAL.ProfileWidget");
EXPERTUS.SMARTPORTAL.ProfileWidget=function(){
	try{
	this.widgetGlobalName="EXPERTUS.SMARTPORTAL.ProfileWidget";
	this.mode="";
	this.current_node;
	this.created=0;
	this.tree;
	this.level;
	this.type;
	this.node_name;
	this.clop_node ="";
	this.openall=false;
	this.parent;
	this.root;
	this.perm;
	this.rootId;
	this.iniLevel;
	this.conTrStat;

	this.insertparam;
	this.parentCode;
	this.currentID;
	this.parentList;
	this.setDelId;

	this.render=function(serviceattributes)
	{
		this.renderBase(serviceattributes,this);

		var arrFunc =[
                    	{divId: "titleDiv", funcName: "titleMessage"},
                    	{divId: "mainDiv", funcName: "showMain" }
                   ];
		$("#"+this.getUniqueWidgetId()).setPageLayout(arrFunc, this);
		$(".sp_msgwrap").hide();
//		$(document.body).append('<div id="conBlk" style="width:230px;display:none;position:absolute;left:800px;top:100px;"><fieldset style="background-color:#ffffff"><legend id="legend">Test</legend><div id="conTree"></div></fieldset></div>');
//		$(document.body).append('<div id="conBlk" style="width:230px;display:none;position:fixed;left:800px;top:140px;"><fieldset style="background-color:#ffffff"><legend id="legend">Test</legend><div id="conTree"></div></fieldset></div>');
		/*$("#conBlk").scrollFollow({
		     speed: 0,
		     offset: 100,
		     easing: 0
		    });*/

		/*$(".links-fieldset").click(function(){
			if($("#conTree").css("display")=="none")
				$("#conTree").show("slow");
			else
				$("#conTree").hide("slow");
		});*/
		this.showTree();
		var x=[ {'code':'cre_sys_lng','language':'cre_sys_lng_eng'}];
		var PMgr = new EXPERTUS.SMARTPORTAL.ProfileManager();
		PMgr.initialize(this);
		reqStr = PMgr.getLovRequest(x);
		PMgr.requestXml = reqStr;
		PMgr.execute();
	};

	this.toggleConTree = function(){
		if($("#conTreeDiv").css("display")=="none")
			$("#conTreeDiv").slideDown();
		else
			$("#conTreeDiv").slideUp(); //"slide",{direction:"up"},1000);
		this.conTrStat = $("#conTree").css("display");
	};

	this.titleMessage = function(){
		return '<div class="front-block-header block-header sp_content_header"><h2 style="width: auto;">'+SMARTPORTAL.t(this.root=="R"?"Lookup Master":this.root)+'</h2></div>';
	};


	this.showMain = function(){
		var html = "<div id='"+this.getUniqueWidgetId()+"profilemain' style='width:700px;'><div id='"+this.getUniqueWidgetId()+"profiletree' style='margin-top:10px;'></div></div>";
//		html +='<div id="conBlk" style="position:fixed;width:230px;height:50px;top:200px;left:850px;display:none;"><fieldset style="background-color:#ffffff"><legend id="legend">Test</legend><div id="conTree"></div></fieldset></div>';
//		html +='<div id="conBlk" style="width:230px;display:none;position:absolute;left:400px;top:100px;"><fieldset style="background-color:#ffffff"><legend id="legend">Test</legend><div id="conTree"></div></fieldset></div>';
		return html;
	};

	this.showTree = function (){
		var obj = this;
		var level;
		var opened;
		var manager = new EXPERTUS.SMARTPORTAL.ProfileManager();
		manager.initialize(this);
		$('#'+this.getUniqueWidgetId()+"profiletree").tree({
			callback:{
				beforeopen: function(node,objTree){
					if($(node).find("li ul").length==0)
						opened = false;
					else
						opened = true;
					return true;
				},
				onopen: function(node,objTree){
						if($(node).find("li>span").length==0){
							var nodes = objTree.children(node);
							var links="";
							for(var i=0;i<nodes.length;i++){
								links="";
								if($(nodes[i]).attr("stats1")!=undefined && $(nodes[i]).attr("stats1")!="" && $(nodes[i]).attr("stats1")!="0")
									links += '<span style="color:red" onclick="loadReportInfo()" class="imglink" title="'+ $(nodes[i]).attr("stats1_desc")+'">('+$(nodes[i]).attr("stats1")+')</span>';
								if($(nodes[i]).attr("stats2")!=undefined && $(nodes[i]).attr("stats2")!="" && $(nodes[i]).attr("stats2")!="0")
									links += '<span style="color:blue"  class="imglink" title="'+ $(nodes[i]).attr("stats2_desc")+'">('+$(nodes[i]).attr("stats2")+')</span>';
								if($(nodes[i]).attr("stats3")!=undefined && $(nodes[i]).attr("stats3")!="" && $(nodes[i]).attr("stats3")!="0")
									links += '<span style="color:orange"  class="imglink" title="'+ $(nodes[i]).attr("stats3_desc")+'">('+$(nodes[i]).attr("stats3")+')</span>';
								if($(nodes[i]).attr("stats4")!=undefined && $(nodes[i]).attr("stats4")!="" && $(nodes[i]).attr("stats4")!="0")
									links += '<span style="color:green"  class="imglink" title="'+ $(nodes[i]).attr("stats4_desc")+'">('+$(nodes[i]).attr("stats4")+')</span>';
								if(links!="")
									$(nodes[i]).find("a:eq(0)").after(links);
							}
						}
						if(obj.mode!="createnode" && obj.mode!="savenode"){
							$(node).find("li").each(function(){
								if($(this).find("img").length==0){// only if not already appended
									id = $(this).attr("id");
									$(this).append(obj.getAccessHtml(id));
								}
							});
						}

					},

				beforedata : function(node, objTree){
					level = obj.getLevel(node, objTree, -1);
					var params = {id:$(node).attr("id"),level:level, root: obj.root};
					if(!node){
						params.id=0;
						params.level=obj.iniLevel;
						params.root = obj.root;
					}
					manager.requestXml = manager.generateProfileAsyncRequest(params);
					return manager.requestXml;
				},
				onselect :function(node, objTree) {
					$("#conBlk").remove();

//					alert($(node).position().top);
					var html ='<div id="conBlk" style="left:800px;width:230px;display:none;position:absolute;top:'+($(node).position().top+115)+'px;"><fieldset class="links-fieldset" ><legend onclick="'+obj.getWidgetInstanceName()+'.toggleConTree();" id="legend">Test</legend><div id="conTreeDiv"><div id="conTree" style="display:block;"></div></div></fieldset></div>';
//					$(node).find("img:eq(2)").after(html);
					$(document.body).append(html);

					$("#legend").html(objTree.get_text(node));
					obj.showConTextTree($(node).attr("attr3"), $(node).attr("attr4"));
				},
				ondata : function (data,objTree){
					return data;
				},
				onparse: function (str, objTree){
					return str;
				},
				beforecreate: function(node, ref_node, type, objTree){
					obj.current_node = ref_node;
					obj.tree = objTree;
					obj.type = type;
					obj.level = obj.getLevel(ref_node, objTree, -1);
					if(type=="inside")
						obj.level++;
					if(obj.mode!="savenode"){
						obj.mode="createnode";

						//if(obj.level >1)
						//{
							var param = {parent_id:obj.currentID};
							var manager = new EXPERTUS.SMARTPORTAL.ProfileManager();
							manager.initialize(obj);
							manager.requestXml = manager.generateProfileCodeRequest(param);
							manager.exec(node);
						/*
							obj.showCreateDialog(obj.level);
						}
						else{
						obj.showCreateDialog(obj.level);
						}
						*/
						if(type=="inside")
							obj.parent = ref_node;
						else
							obj.parent = objTree.parent(ref_node);
						return false;
					}else
						return true;
				},
				oncreate: function(node, objTree, rb){
					obj.created=1;
					obj.current_node = node;

				},
				beforerename: function(node, lang, objTree){
					obj.remProf();
					obj.level = obj.getLevel(node, objTree, -1);
					obj.node_name = objTree.get_text(node);
					if( (obj.mode=="createnode" || obj.mode=="savenode") && level!=1){ //level check => attempt to create a level1 node then edit level1 node
						objTree.select_branch(node);
						return true;
					}else{
						if(obj.mode=="updated"){
							return true;
						}
						if(obj.mode=="updatenode"){
							obj.mode="";
							return true;
						}
						obj.mode = "renamenode";
						obj.current_node = node;
							var manager = new EXPERTUS.SMARTPORTAL.ProfileManager();
							manager.initialize(obj);
							manager.requestXml = manager.generateProfileDetsRequest({id:node.id,level:obj.level,root:obj.root});
							manager.exec(node);
							return false;
					}
				},
				onrename: function(node, objTree, rb){
					level = obj.getLevel(node, objTree, -1);
					if(obj.mode=="updated"){
						obj.mode="";
						$("#profile_update_wizard").dialog('close');
						$("#profile_update_wizard").dialog('destroy');
				        $("#profile_update_wizard").remove();
						return;
					}
					if(obj.created==1){
						obj.created=0;
						var par = objTree.parent(node);
						var param = {parent_id:$(par).attr("id"),level:level,name:objTree.get_text(node),description:'test', root: obj.root};
						obj.mode="createnode";
						var manager = new EXPERTUS.SMARTPORTAL.ProfileManager();
						manager.initialize(obj);
						manager.requestXml = manager.generateCreateRequest(param);
						manager.exec(node);

					}else{
						if(obj.level==1){
							return;
						}else{
							if(obj.node_name==objTree.get_text(node)) //no change in node name
								return;
							var par = objTree.parent(node);
							var param = {id:node.id,newname:objTree.get_text(node),level:level, root:obj.root};
							obj.mode="renamenode";
							var manager = new EXPERTUS.SMARTPORTAL.ProfileManager();
							manager.initialize(obj);
							manager.requestXml = manager.generateRenameProfileRequest(param);
							manager.exec(node);
						}
					}
				},
				beforedelete: function(node, objTree, rb){
					//var conf = confirm(SMARTPORTAL.t("Are you sure you want to delete?"));
					//if(!conf)
					//	return false;
					//else{
					    obj.remProf();
						level = obj.getLevel(node, objTree, -1);
						obj.current_node = objTree.parent(node);
						return true;
					//}
				},
				ondelete: function(node, objTree, rb){
					var param = {id:node.id,level:level, root: obj.root};
					obj.mode="deletenode";
					var manager = new EXPERTUS.SMARTPORTAL.ProfileManager();
					manager.initialize(obj);
					manager.requestXml = manager.generateDeleteProfileRequest(param);
					$("#profile_delete_wizard").dialog('close');
					$("#profile_delete_wizard").dialog('destroy');
					$("#profile_delete_wizard").remove();
					manager.exec(obj.current_node);
				},
				check_move:function(node, ref_node, type, objTree, rb){
					return false;
				},
				onmove: function(node, ref_node, type, objTree, rb){
					alert(objTree.get_text(node) +" "+ type+" "+objTree.get_text(ref_node));
				},
				onsearch: function(node, objTree){
					objTree.container.find('.clicked',"contains").removeClass('clicked');
					$(node).find("a:eq(0)").addClass('clicked');
				},
				onload : function(objTree){
					$(".ltr").find("ul>li").each(function(){
						id = $(this).attr("id");
						$(this).find("a:eq(0)").after(obj.getAccessHtml(id));
					});
					obj.rootId = $(".ltr").find("li:eq(0)").attr("id");
					if(obj.perm & "100")
						$(".ltr").find("a:eq(0)").after('<img class="imglink" title="'+SMARTPORTAL.t('Add Profile')+'" src="'+themepath+'/images/fplus_rtl.gif" onclick="'+ obj.getWidgetInstanceName() +'.addNode(\''+ obj.rootId +'\')" />&nbsp;');
					$(".ltr").after("<div style='clear:both'>&nbsp;</div>");

					//close and open to get stats for level 1 nodes
					objTree.close_all();
					objTree.open_branch("#"+$(".ltr li:eq(0)").attr("id"));

				}
			},
			data:{
				type:"xml_flat",
				async : true,
				opts: {
					url: manager.url,
					method: "POST"

				}
			},
			ui:{
				theme_name: "classic",
				animation: 300

			},
			rules:{
				multiple: false
			}

		});

		var t = $.tree.focused();

		var prependStr = '<div style="margin:15px 0 20px 5px;">';
		prependStr += '</div>';


		$('#'+this.getUniqueWidgetId()+"profiletree").append('<div style="clear:both;padding:20px;"></div>');
		$('#'+this.getUniqueWidgetId()+"search").html(prependStr);

		t.open_branch("#ProfileServiceprofiletree");
	};

	this.profileDeleteWizard =function(){
		var dialogBody = '<div id="deleteprofile" style="padding:5px 0;clear:both;">';
		dialogBody += '<table>';
		dialogBody += '<tr><td align="center">Are you sure you want to delete?</td></tr>';
		dialogBody += '</table></div>';
		return dialogBody;
	};
	this.deleteProfileWiz=function(id){
		this.setDelId=id;
		this.mode = "deletenode";
		var param = {parent_id:id};
		var manager = new EXPERTUS.SMARTPORTAL.ProfileManager();
		manager.initialize(this);
		manager.requestXml = manager.generateProfileCodeRequest(param);
		manager.exec(this.current_node);
	};
	this.showDeleteDialog = function(){
		var delId=this.setDelId;
	        $("#profile_delete_wizard").remove();
	        var html = '';
	        html+='<div id="profile_delete_wizard" style="display:none">';
	        html+=this.profileDeleteWizard();
	        html+='</div>';
	        $("body").append(html);
	        var closeButt={};
	        closeButt[SMARTPORTAL.t('No')]=function(){$(this).dialog('close');$(this).dialog('destroy');$("#profile_delete_wizard").remove();EXPERTUS.SMARTPORTAL.ProfileWidget.closeDialog();};
		    closeButt[SMARTPORTAL.t('Yes')]=function(){ EXPERTUS.SMARTPORTAL.ProfileWidget.deleteNode(delId)};

	        $("#profile_delete_wizard").dialog({
	            bgiframe: true,
	            height: 250,
	            width:460,
	            resizable:false,
	            draggable:false,
	            closeOnEscape: false,
	            modal: true,
	            title:SMARTPORTAL.t('Delete Profile List - ' + this.parentList),
	            buttons:closeButt,
	            position:[(getWindowWidth()-440)/2,100],
	            overlay:
	             {
	               opacity: 0.9,
	               background: "black"
	             }
	        });
	        $('.ui-dialog').wrap("<div class='portal-ui-element'></div>");
	        $('.ui-widget-overlay').wrap("<div class='portal-ui-element'></div>");
	        //$('.ui-dialog-titlebar-close').remove();

	        $("#profile_delete_wizard").css('height','auto');
	        $("#profile_delete_wizard").show();

	        $('.ui-dialog-titlebar-close').click(function(){
	            $("#profile_delete_wizard").remove();
	        });

	    };

	this.showConTextTree = function(attr3, attr4){
		if(attr3=="" || attr3==undefined){
			$("#conBlk").hide();
			$('#conTree').html("");
			return;
		}
		$("#conBlk").show();
		var widObj = this;
		$('#conTree').tree({
			callback:{
				beforeclose: function(node, objTree){
					return false;
				},
				onsearch: function(node, objTree){
					objTree.container.find('.clicked').removeClass('clicked');
					node.addClass('clicked'); //found_node
				}
			},
			data:{
				type:"xml_flat",
				opts: {
					'static': widObj.getXmlFlat(attr3)
				}
			},
			ui:{
				theme_name: "classic",
				animation: 300
			},
			rules:{
				multiple: false
			}

		});
		var tree =  $.tree.reference('#conTree');
		tree.open_all();
		tree.search(attr4);
//		alert(this.conTrStat);
		if(this.conTrStat=="none")
			$("#conTree").hide();
//		alert(attr4);
//		tree.lock(true);
	};

	this.getXmlFlat = function(attr4){
		var arr = attr4.split(":");
		var html ="<root>";
		for(var i=0;i<arr.length;i++){
			html += '<item parent_id="'+(i==0?0:i)+ '" id="'+(i+1)+'"><content><name>'+arr[i]+'</name></content></item>';
		}
		html += "</root>";
		return html;
	};

	this.searchTree = function(val){
		var tree = $.tree.reference("#ProfileServiceprofiletree");
		tree.search(val);
	};

	this.close_all = function(){
		var tree = $.tree.reference("#ProfileServiceprofiletree");
		tree.close_all();
		tree.open_branch("#ProfileServiceprofiletree");
	};

	this.open_all = function(){
		var tree = $.tree.reference("#ProfileServiceprofiletree");
		this.openall = true;
		tree.open_all();
		this.openall = false;
	};

	this.remProf = function(){
		if($("#newprofile").length){
			//$("#newprofile").slideUp();
			$("#newprofile").remove();
		}
	};

	this.test = function(){
		var manager = new EXPERTUS.SMARTPORTAL.ProfileManager();
		manager.initialize(this);
		manager.requestXml = manager.generateProfileByTypeRequest({type:'1'});
		manager.execute();
	};

	this.profileCreateWizard = function(level){
		//This will remove an existing dialog
		this.remProf();
		var t = $.tree.focused();
		var ref_level = this.getLevel(this.current_node, $.tree.focused(), -1);
		var dialogBody = '<div id="newprofile" style="padding:5px 0;clear:both;width:465px;"><a name="newprof">&nbsp;</a>';
		dialogBody += '<table>';
		if(level > 1 )
		dialogBody += '<tr><td>Parent Code:</td><td colspan="3"><input type="text" id="parent_code" name="ParentCode" value="'+this.parentCode+'" class="" readonly /></td></tr>';
		dialogBody += '<tr><td width="25%">Code:<div class="sp_label_mandatory">*</div></td><td width="25%"><input type="text" id="code" name="Code"  class="mandatory" /><span id="profilecode_exist_err" class="error" style="display: none;">This code is already in use. Please choose another one.</span></td>';
		dialogBody += '<td width="25%">Language:<div class="sp_label_mandatory">*</div></td><td width="25%"><select id="language_code" name="Language" class="mandatory" ><option value="">'+SMARTPORTAL.t("Select")+'</option></select></td></tr>';

		dialogBody += '<tr><td>Name:<div class="sp_label_mandatory">*</div></td><td><input type="text" name="Name" id="prof_name"  class="mandatory" /></td>';
		dialogBody += '<td>Active:</td><td><select id="prof_active"><option value="Y">Yes</option><option value="N">No</option></select></td></tr>';

		if(this.root=="R" && (level==0 || level==1) )
		dialogBody += '<tr><td width="25%">Description :</td><td colspan="3" width="25%"><input type="text" id="prof_desc" value="" /></td></tr>';
		dialogBody += '<tr><td>Attribute 1:</td><td><input type="text" id="attr1" value="" /></td><td>Attribute 2:</td> <td><input type="text" id="attr2" value="" /></td></tr>';
		dialogBody += '<tr><td>Attribute 3:</td><td><input type="text" id="attr3" value="" /></td><td>Attribute 4:</td><td><input type="text" id="attr4" value="" /></td></tr>';
		dialogBody += '</table></div>';
			//$("#ProfileServiceprofiletree").append(dialogBody); // append to root node
			//this.updateLovOptions('language_code', 'Languages','');
			//$("#newprofile").slideDown();
			//$("#prof_name").focus();
		return dialogBody;
	};

	 this.showCreateDialog = function(level){
	        $("#profile_create_wizard").remove();
	        var html = '';
	        html+='<div id="profile_create_wizard" style="display:none">';
	        html+=this.profileCreateWizard(level);
	        //html+='<span style="display:none" class="createlnr-mandatory">'+SMARTPORTAL.t("Please enter values for all mandatory fields (marked in red).")+'</span>';
	        html+='</div>';
	        $("body").append(html);

	        //this.updateLovOptions('language_code', 'Languages','');
	        this.updateLovOptions('language_code','cre_sys_lng','cre_sys_lng_eng');
	        var closeButt={};
	        closeButt[SMARTPORTAL.t('Cancel')]=function(){ $(this).dialog('close');$(this).dialog('destroy');$("#profile_create_wizard").remove();EXPERTUS.SMARTPORTAL.ProfileWidget.closeDialog();};
	        closeButt[SMARTPORTAL.t('Save')]=function(){ EXPERTUS.SMARTPORTAL.ProfileWidget.saveNode(level)};

	        $("#profile_create_wizard").dialog({
	            bgiframe: true,
	            height: 250,
	            width:660,
	            resizable:false,
	            draggable:false,
	            closeOnEscape: false,
	            modal: true,
	            title:SMARTPORTAL.t('Create Profile List -  ' + this.parentList ),
	            buttons:closeButt,
	            position:[(getWindowWidth()-660)/2,100],
	            overlay:
	             {
	               opacity: 0.9,
	               background: "black"
	             }
	        });
	        $('.ui-dialog').wrap("<div class='portal-ui-element'></div>");
	        $('.ui-widget-overlay').wrap("<div class='portal-ui-element'></div>");
	        //$('.ui-dialog-titlebar-close').remove();

	        $("#profile_create_wizard").css('height','auto');
	        $("#profile_create_wizard").show();

	        $('.ui-dialog-titlebar-close').click(function(){
	            $("#profile_create_wizard").remove();
	        });

	    };

	    this.profileUpdateWizard = function(){
	    var dialogBody = '<div id="newprofile" style="padding:5px 0;width:465px;">';
		dialogBody += '<table>';
		dialogBody += '<tr><td width="25%">Code:<div class="sp_label_mandatory">*</div></td><td width="25%"><input type="text" id="code" readonly name="Code"  class="mandatory" /></td>';
		dialogBody += '<td width="25%">Language:<div class="sp_label_mandatory">*</div></td><td width="25%"><select id="language_code" name="Language" disabled  class="mandatory"><option value="">'+SMARTPORTAL.t("Select")+'</option></select></td></tr>';
		dialogBody += '<tr><td>Name:<div class="sp_label_mandatory">*</div></td><td><input type="text" id="prof_name" name="Name"  class="mandatory" /></td>';
		dialogBody += '<td>Active:</td><td><select id="prof_active"><option value="Y">Yes</option><option value="N">No</option></select></td></tr>';

		if(this.root=="R" && (this.level==0 || this.level==1))
		dialogBody += '<tr><td width="25%">Description :</td><td colspan="3" width="25%"><input type="text" id="prof_desc" value="" /></td></tr>';
		dialogBody += '<tr><td>Attribute 1:</td><td><input type="text" id="attr1" value="" /></td><td>Attribute 2:</td> <td><input type="text" id="attr2" value="" /></td></tr>';
		dialogBody += '<tr><td>Attribute 3:</td><td><input type="text" id="attr3" value="" /></td><td>Attribute 4:</td><td><input type="text" id="attr4" value="" /></td></tr>';

		dialogBody += '</table></div>';
		return dialogBody;
	    };

		 this.showUpdateDialog = function(){
		        $("#profile_update_wizard").remove();
		        var html = '';
		        html+='<div id="profile_update_wizard" style="display:none">';
		        html+=this.profileUpdateWizard();
		        //html+='<span style="display:none" class="createlnr-mandatory">'+SMARTPORTAL.t("Please enter values for all mandatory fields (marked in red).")+'</span>';
		        html+='</div>';
		        $("body").append(html);

		        this.updateLovOptions('language_code','cre_sys_lng','cre_sys_lng_eng');

		        var closeButt={};
		        closeButt[SMARTPORTAL.t('Cancel')]=function(){$(this).dialog('close');$(this).dialog('destroy');$("#profile_update_wizard").remove();EXPERTUS.SMARTPORTAL.ProfileWidget.closeDialog();};
		        closeButt[SMARTPORTAL.t('Save')]=function(){ EXPERTUS.SMARTPORTAL.ProfileWidget.updateRootNode()};

		        $("#profile_update_wizard").dialog({
		            bgiframe: true,
		            height: 250,
		            width:660,
		            resizable:false,
		            draggable:false,
		            closeOnEscape: false,
		            modal: true,
		            title:SMARTPORTAL.t('Update Profile List -  ' + this.parentList),
		            buttons:closeButt,
		            position:[(getWindowWidth()-660)/2,100],
		            overlay:
		             {
		               opacity: 0.9,
		               background: "black"
		             }
		        });
		        $('.ui-dialog').wrap("<div class='portal-ui-element'></div>");
		        $('.ui-widget-overlay').wrap("<div class='portal-ui-element'></div>");
		        //$('.ui-dialog-titlebar-close').remove();

		        $("#profile_update_wizard").css('height','auto');
		        $("#profile_update_wizard").show();

		        $('.ui-dialog-titlebar-close').click(function(){
		            $("#profile_update_wizard").remove();
		        });

		    };

	this.closeDialog = function(){
		//$("#newprofile").slideUp();
		$("#newprofile").remove();
		this.mode="";

	};

/*	this.renderError=function(){
		alert(this.responseText);
	};*/

	this.getAccessHtml = function(id){
		var html = "";
		if(this.perm & "100")
			html += '&nbsp;<img class="imglink" title="'+SMARTPORTAL.t('Add Profile')+'" src="'+themepath+'/images/fplus_rtl.gif" onclick="'+ this.getWidgetInstanceName() +'.addNode(\''+ id +'\')" />';
		if(this.perm & "010")
			html += '&nbsp;<img class="imglink" title="'+SMARTPORTAL.t('Edit Profile')+'" src="'+themepath+'/images/pen34.png" width="9" height="9" onclick="renNode(\''+ id +'\');" />';
		if(this.perm & "001")
			html += '&nbsp;<img class="imglink" title="'+SMARTPORTAL.t('Delete Profile')+'" src="'+themepath+'/images/del.png" onclick="'+ this.getWidgetInstanceName() +'.deleteProfileWiz(\''+ id +'\')" />';
		return html;
	};

	this.renderResults=function(){
		if($(this.responseXml).find("ListItem").length){
		this.lovXml=this.responseXml;
		}
		if($(this.responseXml).find("faultcode").length)
			this.renderError();
		if(this.mode=="getprofile"){
			this.showTree();
		}else if (this.mode=="createnode"){
			if($(this.responseXml).find("GetProfileCodeResponse").length){
				this.parentCode = $(this.responseXml).find("parent_code").text();
				this.parentList = $(this.responseXml).find("parent_list").text();
				this.showCreateDialog(this.level);
			}
			else{
			this.current_node.id=$(this.responseXml).find("InsertProfileResponse").text();
			$("#"+this.current_node.id).find("a:eq(0)").after(this.getAccessHtml(this.current_node.id));
			this.mode="";
			}
		}else if(this.mode=="savenode"){
			var resStatus = $(this.responseXml).find("InsertProfileResponse").text();
			if(resStatus =='-1' || resStatus=='F-1' ){
				$('#profilecode_exist_err').show();
			}
			else{
				var t = $.tree.focused();
				t.create(this.insertparam, this.current_node, this.type);
				$(this.current_node).attr("id",$(this.responseXml).find("InsertProfileResponse").text());
				$("#"+$(this.current_node).attr("id")).find("a:eq(0)").after(this.getAccessHtml(this.current_node.id));
				this.mode="";
				 $("#profile_create_wizard").dialog('close');
				$("#profile_create_wizard").dialog('destroy');
		        $("#profile_create_wizard").remove();
			}
		}
		else if(this.mode == "renamenode"){
				this.remProf();
				this.parentList = $(this.responseXml).find("parent_list").text();
				this.showUpdateDialog();
				//$("#"+this.current_node.id).append(dialogBody);

				$("#code").val($(this.responseXml).find("code").text());
				//$("#language_code").val($(this.responseXml).find("lang_code_id").text());
				$("#language_code").val($(this.responseXml).find("lang_code").text());
				$("#prof_name").val($(this.responseXml).find("name").text());
				$("#prof_desc").val($(this.responseXml).find("description").text());
				$("#prof_active").val($(this.responseXml).find("is_active").text());
				$("#attr1").val($(this.responseXml).find("attr1").text());
				$("#attr2").val($(this.responseXml).find("attr2").text());
				$("#attr3").val($(this.responseXml).find("attr3").text());
				$("#attr4").val($(this.responseXml).find("attr4").text());

				//$("#newprofile").slideDown();
				$("#prof_name").focus();
		}else if(this.mode == "updatenode"){
			this.mode="updated";
			$.tree.focused().lock(false);
			$.tree.focused().rename(this.current_node,$("#prof_name").val());
			//$("#newprofile").slideUp();
			$("#newprofile").remove();

		}
		else if(this.mode == "deletenode"){
			if($(this.responseXml).find("GetProfileCodeResponse").length){
			this.parentList = $(this.responseXml).find("parent_list").text();
			this.showDeleteDialog();
			}
		}
		$("#throbber").remove();
		$.tree.focused().lock(false);

	};

	this.updateRootNode = function(){
		if(this.mandatoryCheck('profile_update_wizard')){
		this.mode="updatenode";
		var prof_name = $("#prof_name").val();
		var prof_type = "1";//prof_name; //$("#prof_type").val();
		var prof_desc = $("#prof_desc").val(); //$("#prof_type option:selected").text();
		var prof_active = $("#prof_active").val();
		var code = $("#code").val();
		var language_code = $("#language_code").val();
		var attr1 = $("#attr1").val();
		var attr2 = $("#attr2").val();
		var attr3 = $("#attr3").val();
		var attr4 = $("#attr4").val();
		var param = {id:this.current_node.id, name:prof_name, type:prof_type, description: prof_desc, is_active: prof_active, code:code, language_code:language_code, attr1: attr1, attr2: attr2, attr3: attr3, attr4: attr4, level:this.level, root: this.root };
		var manager = new EXPERTUS.SMARTPORTAL.ProfileManager();
		manager.initialize(this);
		manager.requestXml = manager.generateUpdateProfileRequest(param);
		//$("#newprofile").slideUp();
		manager.exec(this.current_node);
		}
	};

	this.addNode = function(id){
		this.currentID = id;
		var t = $.tree.focused();
		t.select_branch("#"+id);
		if(t.selected)
			t.create();
		else
			alert("select a node first");
	};

	this.deleteNode=function(id){
		var t = $.tree.focused();
		t.select_branch("#"+id);
		if(t.selected)
			t.remove();
		else
			alert("select a node first");
	};

	this.saveNode=function(level){
		$('#profilecode_exist_err').hide();
		if(this.mandatoryCheck('profile_create_wizard')){
		//var t = $.tree.focused();

		var prof_name = $("#prof_name").val();
		var prof_desc = $("#prof_desc").val();
		var prof_type = "1";
		var prof_active = $("#prof_active").val();

		var code = $("#code").val();
		if(level > 1 ){
			var parent_code = $("#parent_code").val();
			var setcode= parent_code +'_'+code;
		}
		else{
			var setcode= code;
		}
		var language_code = $("#language_code").val();
		var attr1 = $("#attr1").val();
		var attr2 = $("#attr2").val();
		var attr3 = $("#attr3").val();
		var attr4 = $("#attr4").val();
		var param = {data:prof_name, name:prof_name, list_id: getRootId(this.parent,$.tree.focused()), parent_id: $(this.parent).attr("id"), type:prof_type, description: prof_desc, is_active: prof_active, level: level, code:setcode,language_code:language_code, attr1:attr1, attr2:attr2, attr3:attr3, attr4:attr4, root: this.root};
		this.insertparam = param;
		this.mode="savenode";
		var manager = new EXPERTUS.SMARTPORTAL.ProfileManager();
		manager.initialize(this);
		manager.requestXml = manager.generateCreateRequest(param);
		//t.create(param, this.current_node, this.type);
		manager.exec(this.current_node);
		//this.remProf();
	  }
	};

	this.getLevel = function(node, objTree, level){
		var par = objTree.parent(node);
		if(par == false || par == -1)
			return level+this.iniLevel+1;
		return this.getLevel(par,objTree, level+1);
	};
	}catch(e){
		// to do
	}
};

$.extend(EXPERTUS.SMARTPORTAL.ProfileWidget.prototype,EXPERTUS.SMARTPORTAL.AbstractDetailsWidget);

function callprofiledetail(root,perm, iniLevel){
	try{
	var profileDetailObj=new EXPERTUS.SMARTPORTAL.ProfileWidget();
	profileDetailObj.root = root;
	profileDetailObj.perm = perm;
	profileDetailObj.iniLevel = iniLevel;
	profileDetailObj.setUniqueWidgetId('ProfileService');
	profileDetailObj.setWidgetObject(profileDetailObj);
	profileDetailObj.setWidgetTitle("Profile Master");
	profileDetailObj.render({id:'ProfileService'});
	}catch(e){
		// to do
	}
}



function getRootId(node, objTree){
	try{
	var par = objTree.parent(node);
	if(par==-1 || par==false){
		return $(node).attr("id");
	}
	else if($(par).attr("id")=="R")
		return $(node).attr("id");
	else if($(objTree.parent(par)).attr("id")=="R"){
		return $(par).attr("id");
	}
	else
		return getRootId(par,objTree);
	}catch(e){
		// to do
	}
}

function renNode(id){
	try{
	var t = $.tree.focused();

	t.select_branch("#"+id);
	if(t.selected){
		t.rename();
	}
	else
		alert("select a node first");
	}catch(e){
		// to do
	}
}

jQuery.expr[':'].contains = function(a,i,m){
	try{
	return jQuery(a).text().toUpperCase().substring(1, jQuery(a).text().length) == m[3].toUpperCase();
	}catch(e){
		// to do
	}
};

$(document).ready(function(){
	try{
    if(document.getElementById("ProfileService")!=null){
    	var access=$("#profileAccess").val();
    	var title=$("#profileTitle").val();
    	var type=$("#profileType").val();
    	callprofiledetail(title,access,type);
    	var catalogst=$("#catStatus").val();
    	if(catalogst=='1'){
    		var courseDetailObj=new EXPERTUS.SMARTPORTAL.CourseDetailWidget();
 			courseDetailObj.setUniqueWidgetId("CatalogService");
 		    courseDetailObj.setWidgetObject("courseDetailObj");
 			courseDetailObj.renderBase({id:"CatalogService"},courseDetailObj);
    	}
     }
	}catch(e){
		// to do
	}
});