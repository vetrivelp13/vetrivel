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
		request.query.appendChild(new SOAPObject("parent_id").attr("xsi:type","null").val(obj.parent_id));
		request.query.appendChild(new SOAPObject("level").attr("xsi:type","null").val(obj.level));
		request.query.appendChild(new SOAPObject("name").attr("xsi:type","null").val(obj.name));
		request.query.appendChild(new SOAPObject("description").attr("xsi:type","null").val(obj.description));
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
		request.query.appendChild(new SOAPObject("dummy").attr("xsi:type","null").val(""));
		var sr = new SOAPRequest("GetProfile", request);
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
		request.query.appendChild(new SOAPObject("description").attr("xsi:type","null").val(obj.description));
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
		try{
        //$(node).find("a").append("<div><img src='sites/"+resourse.admin_site_name+"/modules/jquery_update/jstree/source/themes/classic/throbber.gif' /></div>");
        //alert($(node).find("a:first").text());
        //alert($(node));
        var img = "<img src='sites/"+resourse.admin_site_name+"/modules/jquery_update/jstree/source/themes/classic/throbber.gif' id='throbber'/>";
		//$(node).append();
		$(node).find("a:first").after(img);
		//return;
		this.execute();
		}catch(e){
			// to do
		}
	}
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

	this.render=function(serviceattributes)
	{
		try{
		var body="<div id='"+this.getUniqueWidgetId()+"profilemain' style='width:700px;'></div><div id='"+this.getUniqueWidgetId()+"profilecontent'></div><div id='"+this.getUniqueWidgetId()+"languagecontent'></div>";

		this.renderBase(serviceattributes,this);
		//$('#'+this.getUniqueWidgetId()).html(body);
		this.bodyHtml = this.mapper(serviceattributes);
		//alert($('#'+this.getUniqueWidgetId()+"profilemain").html());
		$('#'+this.getUniqueWidgetId()).html(this.bodyHtml);
		//$('#'+this.getUniqueWidgetId()+"profilemain").innerHTML = this.bodyHtml;

		paramObj={
				"searchServiceWidgetId":serviceattributes.id,
				"searchParams":[
	                {"title":SMARTPORTAL.t("Profile Name"),"key":"prof_name","autocomplete_type":"location"},
	                {"title":SMARTPORTAL.t("Type"),"key":"prof_type","autocomplete_type":"location"}
	          	],
	          	"searchParamsDivId":this.getUniqueWidgetId()+"Search",
	          	"searchResults":{
		             "colNames":[
						{"title" : "Profile Type ID", "sort" : true, "filter": true},
						{"title" : "Profile Type Name", "sort" : true, "filter": true},
						{"title" : "Profile Value ID", "sort" : true, "filter": true},
						{"title" : "Profile Value", "sort" : true, "filter": true},
						{"title" : "Actions", "sort":false, "filter": false}
					 ],
		             "colModel":[
						{xmlmap:"profiletype_id",width:200,align:'left'},
						{xmlmap:"profiletype_name",width:150,align:'center'},
						{xmlmap:"profilevalue_name",width:100,align:'center'},
						{xmlmap:"profilevalue_id",width:100,align:'center'},
					 ],
		             "height":320,
		             "width":700
				},
	          	"searchResultsDivId":this.getUniqueWidgetId()+"Details",
	          	"parentWidgetObj":this.widgetGlobalName,
	          	"childView":false,
	          	"actionView":true,
	          	"searchConfig":{"advanced":false,"faq":false},
	          	"parentWidgetUniqueId":this.getUniqueWidgetId()
			};
			callSearchWidget(paramObj);


		//this.bodyHtml= this.mapper();
/*		this.mode="getprofile";
		var manager = new EXPERTUS.SMARTPORTAL.ProfileManager();
		manager.initialize(this);
		manager.requestXml = manager.generateProfileRequest();
		manager.execute();
*/
		//$('#'+this.getUniqueWidgetId()).html(body);
		//$('#'+this.getUniqueWidgetId()+'profilemain').html(this.bodyHtml);
		//this.showTree();
		}catch(e){
			// to do
		}
	};

	this.mapper = function(serviceattributes){
		var outhtml="<div id='"+serviceattributes.id+"Search'></div>";
		outhtml+="<div id='"+serviceattributes.id+"AdditionalDetails'><a href='javascript:void(0)' onclick='"+this.getWidgetInstanceName()+".displayAddResourceLocationDetails();'>"+SMARTPORTAL.t('Configure Profile Tree')+"</a></div>";
		outhtml+="<div id='"+serviceattributes.id+"Details'></div>";
		return outhtml;
	}


	this.showTree = function (){
		try{
		var obj = this;
		var xml = $(this.responseText).find("GetProfileResponse").text() ;
		var level;
		$('#'+this.getUniqueWidgetId()+"profilemain").tree({
			callback:{
				beforecreate: function(node, ref_node, type, objTree){
					//alert("beforecreate");
					obj.current_node = ref_node;
					obj.tree = objTree;
					obj.type = type;
					level = getLevel(ref_node, objTree, -1);
					obj.level = level;
					//alert(level);
					if((level==0 || (level==1 && type!="inside") ) && obj.mode!="createnode"){ //obj.mode!="createnode" to not execute this block second when create
						obj.mode="createnode";
						obj.showDialog();
						return false;
					}
					else{
						obj.mode="createnode";
						return true;
					}
				},
				oncreate: function(node, objTree, rb){
					//alert("created "+$(node).text());
					obj.created=1;
					obj.current_node = node;

					/*if(obj.level==1){

					}*/

				},
				beforerename: function(node, lang, objTree){
					//alert("bf ren "+obj.mode);
					if(obj.mode=="createnode"){
						//$.tree.focused().select_branch("#"+node.id);
						return true;
					}
					if(obj.mode=="updatenode"){
						obj.mode="";
						return true;
					}
					obj.mode = "renamenode";
					obj.current_node = node;
					level = getLevel(node, objTree, -1);
					obj.level = level;
					if(level==1){
						//get other atrributes and let user edit them
						var manager = new EXPERTUS.SMARTPORTAL.ProfileManager();
						manager.initialize(obj);
						manager.requestXml = manager.generateProfileDetsRequest({id:node.id});
						//manager.execute();
						manager.exec(node);
						return false;
					}
					else
						return true;
				},
				onrename: function(node, objTree, rb){
					level = getLevel(node, objTree, -1);
					if(obj.created==1){
						obj.created=0;
						var par = objTree.parent(node);
						var param = {parent_id:$(par).attr("id"),level:level,name:objTree.get_text(node),description:'test'};
						obj.mode="createnode";
						var manager = new EXPERTUS.SMARTPORTAL.ProfileManager();
						manager.initialize(obj);
						manager.requestXml = manager.generateCreateRequest(param);
						//alert(manager.requestXml);
						manager.exec(node);

					}else{ //rename
						//obj.current_node = node;
						if(obj.level==1){
							return;
						}else{
							var par = objTree.parent(node);
							var param = {id:node.id,newname:objTree.get_text(node),level:level};
							obj.mode="renamenode";
							var manager = new EXPERTUS.SMARTPORTAL.ProfileManager();
							manager.initialize(obj);
							manager.requestXml = manager.generateRenameProfileRequest(param);
							//manager.execute();
							manager.exec(node);
						}
					}
				},
				beforedelete: function(node, objTree, rb){
					level = getLevel(node, objTree, -1);
					this.current_node = objTree.parent(node);
					return true;
				},
				ondelete: function(node, objTree, rb){
					var param = {id:node.id,level:level};
					obj.mode="deletenode";
					var manager = new EXPERTUS.SMARTPORTAL.ProfileManager();
					manager.initialize(obj);
					manager.requestXml = manager.generateDeleteProfileRequest(param);
					manager.exec(this.current_node);
					//manager.execute();
				},
				check_move:function(node, ref_node, type, objTree, rb){
					return false;
				},
				onmove: function(node, ref_node, type, objTree, rb){
					alert(objTree.get_text(node) +" "+ type+" "+objTree.get_text(ref_node));
				}
			},
			data:{
				type:"xml_flat",
				opts: {
					'static': xml
					//static: '<root><item parent_id="0" id="xml_1"><content><name>Root node 1</name></content></item><item parent_id="xml_1" id="xml_2"><content><name>Child node 1</name></content></item><item parent_id="xml_1" id="xml_3"><content><name>Child Node 2</name></content></item></root>'
					//url: './async_json_data.json'
				}
			}

		});
		//$('#'+this.getUniqueWidgetId()).css("float","left");
		//$('#'+this.getUniqueWidgetId()).append('<div style="clear:both;padding:20px;"><input type="button" value="Add" onclick="EXPERTUS.SMARTPORTAL.ProfileWidget.showDialog();" /><input type="button" value="Rename" onclick="renNode();" /></div>');
		$('#'+this.getUniqueWidgetId()+"profilemain").append('<div style="clear:both;padding:20px;">');
		//$('#'+this.getUniqueWidgetId()+"profilemain").append('<input type="button" value="Add" onclick="addNode();" />');
		$('#'+this.getUniqueWidgetId()+"profilemain").append('<input type="button" value="Add" onclick="'+ this.getWidgetInstanceName() +'.addNode();" />');
		$('#'+this.getUniqueWidgetId()+"profilemain").append('<input type="button" value="Edit" onclick="renNode();" />');
		$('#'+this.getUniqueWidgetId()+"profilemain").append('<input type="button" value="Delete" onclick="deleteNode();" />');
//		$('#'+this.getUniqueWidgetId()+"profilemain").append('<input type="button" value="test" onclick="'+ this.getWidgetInstanceName() +'.test();" />');
		//$('#'+this.getUniqueWidgetId()).append('<input type="button" value="" onclick="renNode();" />');
		$('#'+this.getUniqueWidgetId()+"profilemain").append('</div>');
		var t = $.tree.focused;
		var id;
		//alert(t);
		/*$("#R").find("li").each(function(){
			//alert("t");
			id = $(this).attr("id");
			$(this).find("a:eq(0)").after('<strong><a href="#" onclick="'+ obj.getWidgetInstanceName() +'.addNode(\''+ id +'\');"> + </a>&nbsp;<a href="#"> / </a>&nbsp;<a href="#"> &minus; </a></strong>');
		});*/
		}catch(e){
			// to do
		}
	};

	this.test = function(){
		try{
		var manager = new EXPERTUS.SMARTPORTAL.ProfileManager();
		manager.initialize(this);
		manager.requestXml = manager.generateProfileByTypeRequest({type:'1'});
		manager.execute();
		}catch(e){
			// to do
		}
	}

	this.showDialog = function(){
		try{
			var t = $.tree.focused();
			//this.current_node = t.selected;
			//alert(t.id);
			//alert($(t[id="R"]).text());
			//this.current_node = t.get_node($(t.selected).find("a:first"));
			//var level = getLevel(this.current_node, this.tree, -1);
			var ref_level = getLevel(this.current_node, $.tree.focused(), -1);
			//this.level=ref_level;
/*			alert(this.level);
			if(this.level==1){
*/				var dialogBody = '<div id="newprofile" style="display:none;padding:5px 0;"><table><tr><td>'+SMARTPORTAL.t("Name :")+'</td><td><input type="text" id="prof_name" size="10"/></td>';
				dialogBody += '<td>'+SMARTPORTAL.t("Type : ")+'</td><td><select id="prof_type"><option value="1">1</option><option value="2">2</option><option value="3">3</option></select></td></tr>';
				dialogBody += '<tr><td>'+SMARTPORTAL.t("Description :")+'</td><td><input type="text" id="prof_desc" value="" /></td>';
				dialogBody += '<td>'+SMARTPORTAL.t("Active :")+'</td><td><select id="prof_active"><option value="Y">'+SMARTPORTAL.t("Yes")+'</option><option value="N">'+SMARTPORTAL.t("No")+'</option></select></td></tr>';
				dialogBody += '<tr><td colspan="2" align="center"><input type="button" value="'+SMARTPORTAL.t("Save")+'" onclick="'+this.getWidgetInstanceName();saveNode(ref_level);+'" /></td><td colspan="2"><input type="button" value="'+SMARTPORTAL.t("Close")+'" onclick="$('+'\"#newprofile\"'+').remove();"/></td></tr></table></div>';
				//$("#"+this.current_node.id).append(dialogBody);
				$("#R").append(dialogBody); // append to root node
				$("#newprofile").show('slow');
/*			}else
				this.saveNode();*/
		}catch(e){
			// to do
		}
	};

	this.renderError=function(){
		try{
		//alert(this.responseXml);
		alert(this.responseText);
		}catch(e){
			// to do
		}
	};

	this.renderResults=function(){
		try{
		//alert("render "+this.mode);
		if(this.mode=="getprofile"){
			this.showTree();
		}else if (this.mode=="createnode"){
				this.current_node.id=$(this.responseText).find("InsertProfileResponse").text();
				this.mode="";
		}else if(this.mode == "renamenode"){
			if(this.level==1){
				var dialogBody = '<div id="newprofile" style="display:none;padding:5px 0;"><table><tr><td>'+SMARTPORTAL.t("Name :")+'</td><td><input type="text" id="prof_name" size="10"/></td>';
				dialogBody += '<td>'+SMARTPORTAL.t("Type : ")+'</td><td><select id="prof_type"><option value="1">1</option><option value="2">2</option><option value="3">3</option></select></td></tr>';
				dialogBody += '<tr><td>'+SMARTPORTAL.t("Description :")+'</td><td><input type="text" id="prof_desc" value="" /></td>';
				dialogBody += '<td>'+SMARTPORTAL.t("Active :")+'</td><td><select id="prof_active"><option value="Y">'+SMARTPORTAL.t("Yes")+'</option><option value="N">'+SMARTPORTAL.t("No")+'</option></select></td></tr>';
				dialogBody += '<tr><td colspan="2" align="center"><input type="button" value="'+SMARTPORTAL.t("Save")+'" onclick="'+this.getWidgetInstanceName();updateRootNode();+'" /></td><td colspan="2"><input type="button" value="'+SMARTPORTAL.t("Close")+'" onclick="$('+'\"#newprofile\"'+').remove();"/></td></tr></table></div>';

				$("#newprofile").remove(); // remove if one already exists
				$("#"+this.current_node.id).append(dialogBody);
				$("#prof_name").val($(this.responseText).find("name").text());
				$("#prof_desc").val($(this.responseText).find("description").text());
				$("#prof_active").val($(this.responseText).find("is_active").text());
				$("#prof_type").val($(this.responseText).find("type").text());

				$("#newprofile").show('slow');

			}
		}else if(this.mode == "updatenode"){
			$.tree.focused().rename(this.current_node,$("#prof_name").val());
			$("#newprofile").hide('slow');
			$("#newprofile").remove();

		}
		$("#throbber").remove();
		}catch(e){
			// to do
		}
	};

	this.updateRootNode = function(){
		try{
		this.mode="updatenode";
		var prof_name = $("#prof_name").val();
		var prof_type = $("#prof_type").val();
		var prof_desc = $("#prof_desc").val();
		var prof_active = $("#prof_active").val();
		var param = {id:this.current_node.id, name:prof_name, type:prof_type, description: prof_desc, is_active: prof_active };
		var manager = new EXPERTUS.SMARTPORTAL.ProfileManager();
		manager.initialize(this);
		manager.requestXml = manager.generateUpdateProfileRequest(param);
		//manager.execute();
		$("#newprofile").hide('slow');
		manager.exec(this.current_node);
		}catch(e){
			// to do
		}
	};

	this.addNode = function(id){ //
		try{
		var t = $.tree.focused();
	//	t.select_branch("#"+id);
		if(t.selected)
			t.create();
		else
			alert("select a node first");
		//alert("created");
		}catch(e){
			// to do
		}
	};

	this.saveNode=function(level){
		try{
		var t = $.tree.focused();


		// create using the name
		// ajax to create the node in the backend
		var prof_name = $("#prof_name").val();
		var prof_type = $("#prof_type").val();
		var prof_desc = $("#prof_desc").val();
		var prof_active = $("#prof_active").val();
		var param = {data:prof_name, name:prof_name, type:prof_type, description: prof_desc, is_active: prof_active, level: "1"};
		this.mode="createnode";
		var manager = new EXPERTUS.SMARTPORTAL.ProfileManager();
		manager.initialize(this);
		manager.requestXml = manager.generateCreateRequest(param);
		//alert(this.current_node.id);
		t.create(param, this.current_node, this.type);
		//manager.execute();
		manager.exec(this.current_node);

		$("#newprofile").hide('slow');
		$("#newprofile").remove();
		}catch(e){
			// to do
		}

	};
	}catch(e){
		// to do
	}
};
$.extend(EXPERTUS.SMARTPORTAL.ProfileWidget.prototype,EXPERTUS.SMARTPORTAL.AbstractDetailsWidget);

function callprofiledetail(){
	try{
	var courseDetailObj=new EXPERTUS.SMARTPORTAL.ProfileWidget();
	courseDetailObj.setUniqueWidgetId('ProfileService');
	courseDetailObj.setWidgetObject(courseDetailObj);
	courseDetailObj.setWidgetTitle("Profile Master");
	courseDetailObj.render({id:'ProfileService'});
	}catch(e){
		// to do
	}
}

function getLevel(node, objTree, level){
	try{
	var par = objTree.parent(node);
	if(par == false || par == -1)
		return level+1;
	return getLevel(par,objTree, level+1);
	}catch(e){
		// to do
	}
}

function renNode(){
	try{
	var t = $.tree.focused();
	if(t.selected)
		t.rename();
	else
		alert(SMARTPORTAL.t("select a node first"));
	}catch(e){
		// to do
	}
}

function deleteNode(){
	try{
	var t = $.tree.focused();
	if(t.selected)
		t.remove();
	else
		alert(SMARTPORTAL.t("select a node first"));
	}catch(e){
		// to do
	}
}


