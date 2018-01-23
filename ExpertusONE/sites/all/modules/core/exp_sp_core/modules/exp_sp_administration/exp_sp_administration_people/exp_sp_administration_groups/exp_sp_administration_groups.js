(function($) {
	$.widget("ui.peoplegroup", {
		getUsersearch: function(){
			try{
		    	var gptype= $('#search_all_user_type-hidden').val();
		    	$('#hiddengptype').val(gptype);
			}catch(e){
				// To Do
			}
	    },

	    moreRoleHideShow: function() {
			try{
		        $('.more-drop-down').slideToggle();
		        $('.more-drop-down li:last').css('border-bottom','0px none');
			}catch(e){
				// To Do
			}
	    },

		callMultiSelect: function(ele,id){
	    	var oc = $('#attr_open').val();
	    	this.currTheme = Drupal.settings.ajaxPageState.theme;
	    	$('#avil_'+oc).css('display','none');
	    	$('#avil_'+ele).css('display','block');
	    	$('#attr_open').val(ele);
	    	
	    	$('#avil_'+oc).prev().find('span:last')
	    		.removeClass('down-tip-arrow')
	    		.addClass('right-tip-arrow');
	    	
	    	$('#avil_'+ele).prev().find('span:last')
    		.removeClass('right-tip-arrow')
    		.addClass('down-tip-arrow');
	    	
			id = (id==null || id == undefined || id == '') ? 0 : id;
			
			//Construct data load and autocomplete urls
			var url = this.constructUrl("administration/people/groups/attributs/"+ele+"/"+id);
			var autourl = this.constructUrl("administration/people/groups/attributs/autocomplete//"+ele);
			$msobj = $("#root-admin").data('peoplegroup');
			// return to avoid re-initialize
	    	if($('#avil_'+ele).html()!=''){
	    		if(ele=='ste_con_ste'){
	    			url += '&country='+ encodeURIComponent($('#atr_ste_con_cnt').val());
					autourl += '&country='+ encodeURIComponent($('#atr_ste_con_cnt').val());
					var ob = $('#avil_ste_con_ste').data('expmultiselect');					
					ob.options.url = url;
					ob.options.searchfilter.url = autourl;
					ob.recs = 1;
					ob.page = 1;
					ob.reqcnt = 0;
					ob.autocompleteInit();
					ob._constructajaxcall();
					var heightAp = $('.attr-select-pane').css('height', 'auto').height();
				    $('#attr-separator, .No-attr-sel, .attr-pane').css('min-height', heightAp+'px');
				     $('#atr-sel-pane').css('min-height', heightAp+'px');
				     $msobj.callScrollBar();
					return true;
					
	    		}else{
	    			var heightAp = $('.attr-select-pane').css('height', 'auto').height();
				    $('#attr-separator, .No-attr-sel, .attr-pane').css('min-height', heightAp+'px');
				     $('#atr-sel-pane').css('min-height', heightAp+'px');	
				     $msobj.callScrollBar();
	    			return true;
	    			
	    		}
	    	}  
	    	
			// Validatation for state, without country select state should not load
			if(ele=='ste_con_ste' && $('#atr_ste_con_cnt').val()==''){
				var heightAp = $('.attr-select-pane').css('height', 'auto').height();
			    $('#attr-separator, .No-attr-sel, .attr-pane').css('min-height', heightAp+'px');
			     $('#atr-sel-pane').css('min-height', heightAp+'px');
				var error = new Array();
				error[0] = Drupal.t('LBL674')+' '+Drupal.t('LBL039');
				var message_call = expertus_error_message(error,'error');
				$('#show_expertus_message').show();
				$('#show_expertus_message').html(message_call);
				return false;
			}else if(ele=='ste_con_ste' && $('#atr_ste_con_cnt').val()!=''){
				url += '&country='+ encodeURIComponent($('#atr_ste_con_cnt').val());
				autourl += '&country='+ encodeURIComponent($('#atr_ste_con_cnt').val());
			}
			var option;
			if(ele!='ste_con_rol' && ele!='ste_con_hdt'){
				option = {
						url: url,
						dataType:"json",
						searchfilter:{url:autourl,enable:true},
						rownum:20,
						width:200,
						height:225,
						checkAll :true,
						uncheckAll :true,
						onselect:this.onselectfn,
						onunselect:this.onunselectfn,
						onselectAll:this.selectAllFn,
						onunselectAll:this.unselectAllFn,
						afterload:function (){var heightAp = $('.attr-select-pane').css('height', 'auto').height();
					       $('#attr-separator, .No-attr-sel, .attr-pane').css('min-height', heightAp+'px');
					        $('#atr-sel-pane').css('min-height', heightAp+'px');},
						helpText: {autocomplete:Drupal.t('Search for refine'),checkAll:Drupal.t('Select All'),uncheckAll:Drupal.t('Unselect All')},
						//onload:this.onloadfn,
						widget:this
					};
			}else{				
				option = {
						url: url,
						dataType:"json",
						searchfilter:{url:autourl,enable:false},
						rownum:20,
						width:200,
						height:75,
						checkAll :false,
						uncheckAll :false,
						onselect:this.onselectfn,
						onunselect:this.onunselectfn,
						onselectAll:this.selectAllFn,
						onunselectAll:this.unselectAllFn,
						afterload:function (){var heightAp = $('.attr-select-pane').css('height', 'auto').height();
					       $('#attr-separator, .No-attr-sel, .attr-pane').css('min-height', heightAp+'px');
					        $('#atr-sel-pane').css('min-height', heightAp+'px');},
						widget:this
					}
			}
			
			
			
			if(this.currTheme == 'expertusoneV2')
				$('#avil_'+ele).expMultiSelectExtend(option);
			else
				$('#avil_'+ele).expmultiselect(option);
		},

		onselectfn: function(ele,data,obj){
			var eid = $(ele).attr('id').replace('avil_','');
			if($('#atr_'+eid).val()=='All'){
				obj.headerShowHide(eid, 'sel');
				$('#avil_'+eid).prev().find('span:first')
				.removeClass('empty-group').addClass('list-group');
				return true;
			}
			var dt=new Array(),i='';
			var v = $('#sel_'+eid).html();
			if(eid == "ste_con_hdt"){
			    $('#avil_'+eid).prev().find('span:first')
				.removeClass('empty-group').addClass('list-group');
				if($('#hidden_hire_start_ste_con_hdt_ltn').val() != '')
                  dt.push('<span class="item-short-desc">'+ Drupal.t('LBL1138') + ' ' +$('#hidden_hire_start_ste_con_hdt_ltn').val()+'</span>');

				if($('#hidden_hire_start_ste_con_hdt_gtn').val() != '')
                  dt.push('<span class="item-short-desc">'+ Drupal.t('LBL1136') + ' ' +$('#hidden_hire_start_ste_con_hdt_gtn').val()+'</span>');

				if($('#hidden_hire_start_ste_con_hdt_btw').val() != '')
                  dt.push('<span class="item-short-desc">'+ Drupal.t('LBL1144') + ' ' +$('#hidden_hire_start_ste_con_hdt_btw').val() +' '+Drupal.t('LBL647').toLowerCase()+' '+ $('#hire_end').val()+'</span>');

			}else{
			if(v!=''){
				dt=v.split(', ');
			}else{
				$('#avil_'+eid).prev().find('span:first')
				.removeClass('empty-group').addClass('list-group');
			}
				dt.push(data.title);
			}
			dt = obj.doSort(dt);
			obj.headerShowHide(eid, 'sel');
				if(eid == "ste_con_hdt"){
					$('#sel_'+eid).html(dt.join(''));
					$('#show_sel_'+eid).html($('#sel_'+eid).html());
				}
				else {
					$('#sel_'+eid).html(dt.join(', '));
					$('#show_sel_'+eid).html(descController('',$('#sel_'+eid).html(),100));
				}
			obj.setMoreIconClick($('#sel_'+eid));
			
			i = $('#atr_'+eid).val();
			if(eid == "ste_con_hdt"){
			if(($('#hidden_hire_start_'+data.value).val() != '' && (data.value == "ste_con_hdt_gtn" || data.value == "ste_con_hdt_ltn"))
					|| (data.value == "ste_con_hdt_btw" && $('#hidden_hire_start_'+data.value).val() != '' && $('#hire_end').val() != '')){
					
					if(i.indexOf(data.value)<0)
						i += (i=="")?data.value:","+data.value;
				}
			}else{
				i += (i=="")?data.value:","+data.value;
			}
		
			
			$('#atr_'+eid).val(i);
			$('input[id="grp_war"]').attr('value',0);
			if($('.attr-content').children('.list-group').length > 0){
				$('#no-attr-sel').hide();
				$('#atr-sel-pane').show();
			}else{
				$('#atr-sel-pane').hide();
				$('#no-attr-sel').show();
			}
			var heightAp = $('.attr-select-pane').css('height', 'auto').height();
		      $('#attr-separator, .No-attr-sel, .attr-pane').css('min-height', heightAp+'px');
			 $('#atr-sel-pane').css('min-height', heightAp+'px');
			obj.callScrollBar();
		},

		doSort: function(x){
			return x.sort(function(a,b){
				return a.toUpperCase().localeCompare(b.toUpperCase());
			});
		},
		
		onunselectfn: function(ele,data,obj){
			var eid = $(ele).attr('id').replace('avil_','');
			
			if($('#atr_'+eid).val()=='All'){
				$('#sel_'+eid).html('');
				$('#show_sel_'+eid).html('');
				$('#atr_'+eid).val('');
				var selVal = $('#avil_'+eid).expmultiselect('getValue','selected');
				var t = new Array();
				var v = new Array();
				$.each(selVal, function(){
					t.push(this.title);
					v.push(this.value);
				});
				obj.headerShowHide(eid, 'sel');
				$('#sel_'+eid).html(t.join(', '));
				$('#show_sel_'+eid).html(descController('',$('#sel_'+eid).html(),100));
				obj.setMoreIconClick($('#show_sel_'+eid));
				$('#atr_'+eid).val(v.join(','));
				obj.callScrollBar();
				
				// To remove curresponding state of removed country
				if(eid == 'ste_con_cnt'){
					obj.removeStates(data.value,'all');
				}
				return true;
			}
			var dt = new Array();
			var i=new Array();
				if(eid == "ste_con_hdt"){
				if($('#hidden_hire_start_ste_con_hdt_ltn').val() != '')
                    dt.push('<span class="item-short-desc">'+ Drupal.t('LBL1138') + ' ' +$('#hidden_hire_start_ste_con_hdt_ltn').val()+'</span>');
                if($('#hidden_hire_start_ste_con_hdt_gtn').val() != '')
					dt.push('<span class="item-short-desc">'+ Drupal.t('LBL1136') + ' ' +$('#hidden_hire_start_ste_con_hdt_gtn').val()+'</span>');
				if($('#hidden_hire_start_ste_con_hdt_btw').val() != '')
					dt.push('<span class="item-short-desc">'+ Drupal.t('LBL1144') + ' ' +$('#hidden_hire_start_ste_con_hdt_btw').val() +' '+Drupal.t('LBL647').toLowerCase()+' '+ $('#hire_end').val()+'</span>');
			}else{
			var dt = unescape($('#sel_'+eid).html()).replace(/"/g, '\&quot;').replace(/&amp;/g, '&').split(', ');			
			if(dt.indexOf(data.title)>=0){
				dt.splice(dt.indexOf(data.title),1);
				}
			}
				if(eid == "ste_con_hdt"){
					$('#sel_'+eid).html(dt.join(''));
					$('#show_sel_'+eid).html($('#sel_'+eid).html());
				}
				else {
					$('#sel_'+eid).html(dt.join(', '));
					$('#show_sel_'+eid).html(descController('',$('#sel_'+eid).html(),100));
				}
				obj.setMoreIconClick($('#show_sel_'+eid));
			i = $('#atr_'+eid).val().split(',');
			if(i.indexOf(data.value)>=0){
				i.splice(i.indexOf(data.value),1);
				$('#atr_'+eid).val(i.join(','));
			}
			if($('#sel_'+eid).html()==''){
				$('#avil_'+eid).prev().find('span:first')
				.removeClass('list-group').addClass('empty-group');
			}
			obj.headerShowHide(eid, 'unsel');
			$('input[id="grp_war"]').attr('value',0);
			
			if($('.attr-content').children('.list-group').length > 0){
				$('#no-attr-sel').hide();
				$('#atr-sel-pane').show();
			}else{
				$('#atr-sel-pane').hide();
				$('#no-attr-sel').show();
			}
			obj.callScrollBar();
			
			// To remove curresponding state of removed country
			if(eid == 'ste_con_cnt'){
				obj.removeStates(data.value,'single');
			}
			var heightAp = $('.attr-select-pane').css('height', 'auto').height();
		      $('#attr-separator, .No-attr-sel, .attr-pane').css('height', heightAp+'px');
		       $('#atr-sel-pane').css('height', heightAp+'px');
		},
		
		removeStates: function(cid,type){
			if($('#sel_ste_con_ste').html()!=''){
				if(type=='single'){
					$('#avil_ste_con_ste').find('#ui-multiselect-con').find('#chk ul li input').each(function(){
						if($(this).attr('id').substring(0,2)==cid){
							$(this).removeAttr('checked');
							$(this).click();
						}
					});
				}else if(type=='all'){ //selectuncheckall
					$('#avil_ste_con_ste').find('#ui-multiselect-con').find('#selectuncheckall').each(function(){
						$(this).removeAttr('checked');
						$(this).click();
					});
				}
			}
		},
		
		headerShowHide: function(eid,opt){
			// Enable Title and title separator
			var rcls, acls, ch;
			if(opt=='sel'){
				rcls = 'empty-group', acls = 'list-group';
			}else{
				rcls = 'list-group', acls = 'empty-group';
			}
			if($('#sel_'+eid).html()==""){
				$('#header_'+eid).removeClass(rcls).addClass(acls);
				var al = $('#atr_list').val();
				var ar = al.split(',');
				var f=true,h='';
				var idx = ar.indexOf(eid);
				for(j=idx-1;j>0;j--){
					h = $('#sel_'+ar[j]).html();
					if((h!=null && h!='')){
						if(opt=='unsel')f=false;
						if($('#and_'+eid).hasClass(rcls) && f==false){
							$('#and_'+eid).removeClass(rcls).addClass(acls);
						}else{
							$('#and_'+ar[j]).removeClass(rcls).addClass(acls);
						}
						
						break;
					}
				}
				if(f===true){
					for(j=idx+1;j<ar.length;j++){
						h = $('#sel_'+ar[j]).html();
						if((h!=null && h!='')){
							$('#and_'+eid).removeClass(rcls).addClass(acls);
							break;
						}
					}
				}
			}
		},
		
		rowEditGroupuser : function(grpId,usrId,action){
			var url = this.constructUrl("administration/people/groups/removeusers/"+grpId+"/"+usrId+"/"+action);
			$.ajax({
				url : url,
				type: "POST",
				async: false,
				success: function(data) {}
			});
		},		
		callScrollBar: function(){
			var aHight = $('.attr-pane').height();
			var sHight = $('.attr-selected-pane').height();
			if(sHight>=aHight){
				if($('.attr-selected-pane').find('.jspContainer').length>0){
//					$('.attr-selected-pane').jScrollPane('reinitialise');
					$('.attr-selected-pane').jScrollPane().data().jsp.reinitialise();
				}else{
					$('.attr-selected-pane').jScrollPane({});
				}
				var hit = $('.attr-selected-pane').height() - 2;
				$('.attr-selected-pane .jspVerticalBar').css('height',hit);
			}else{
				if($('.attr-selected-pane').find('.jspContainer').length>0){
					$('.attr-selected-pane').jScrollPane('destroy');
					$('.attr-selected-pane').html($('.attr-selected-pane .jspPane').html());
				}
			}
		},
		
		/**
		 * Callback function of multi-select control
		 * will be called on selecting select all option
		 */
		selectAllFn: function(ele,obj){
			var eid = $(ele).attr('id').replace('avil_','');
			obj.headerShowHide(eid, 'sel');
			// Set 'All' as a value of the respective attribute
			$('#sel_'+eid).html(Drupal.t('LBL1039'));
			$('#show_sel_'+eid).html(descController('',$('#sel_'+eid).html(),100));
			$('#atr_'+eid).val('All');
			// Show/Hide empty message if none of the attribute seleted
			if($('.attr-content').children('.list-group').length > 0){
				$('#no-attr-sel').hide();
				$('#atr-sel-pane').show();
			}else{
				$('#atr-sel-pane').hide();
				$('#no-attr-sel').show();
			}
			obj.callScrollBar();
			// Show the respective attributes headers
			if($('#atr_'+eid).val()=='All'){
				obj.headerShowHide(eid, 'sel');
				$('#avil_'+eid).prev().find('span:first')
				.removeClass('empty-group').addClass('list-group');
				return true;
			}
			
			var heightAp = $('.attr-select-pane').css('height', 'auto').height();
		      $('#attr-separator, .No-attr-sel, .attr-pane').css('height', heightAp+'px');
		       $('#atr-sel-pane').css('height', heightAp+'px');
		},
		/**
		 * Callback function of multi-select control
		 * will be called on selecting un-select all option
		 */
		unselectAllFn: function(ele,obj){
			var eid = $(ele).attr('id').replace('avil_','');
			// Clear all assigned values of the respective attribute
			$('#sel_'+eid).html('');
			$('#show_sel_'+eid).html('');
			$('#atr_'+eid).val('');
			obj.callScrollBar();
			// Hide the respective attributes headers
			if($('#atr_'+eid).val()==''){
				obj.headerShowHide(eid, 'unsel');
				$('#avil_'+eid).prev().find('span:first')
				.removeClass('list-group').addClass('empty-group');
				
				// Show/Hide empty message if none of the attribute seleted
				if($('.attr-content').children('.list-group').length > 0){
					$('#no-attr-sel').hide();
					$('#atr-sel-pane').show();
				}else{
					$('#atr-sel-pane').hide();
					$('#no-attr-sel').show();
				}

				// To remove curresponding state of removed country
				if(eid == 'ste_con_cnt'){
					obj.removeStates('','all');
				}
				return true;
			}
			// Hide the respective attributes headers
			obj.headerShowHide(eid, 'unsel');
			

			// To remove curresponding state of removed country
			if(eid == 'ste_con_cnt'){
				obj.removeStates('','all');
			}
			var heightAp = $('.attr-select-pane').css('height', 'auto').height();
		      $('#attr-separator, .No-attr-sel, .attr-pane').css('height', heightAp+'px');
		       $('#atr-sel-pane').css('height', heightAp+'px');
		},
		
		// Right side panel more icon's click event
		setMoreIconClick: function(o){
			var obj = this;
			$(o).find('.more-icon-sec').click(function(){
				obj.callScrollBar();
			});
		}
		
	});
	$.extend($.ui.peoplegroup.prototype, EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget);
})(jQuery);

$(function() {
	try{
		$("#root-admin").peoplegroup();	
	}catch(e){
			// To Do
	}
});

Drupal.ajax.prototype.commands.onAfterLoadForm = function(ajax, response, status) {
	setTimeout(function(){
		if($('.attr-content').children('.list-group').length > 0){
			$('#no-attr-sel').hide();
			$('#atr-sel-pane').show();
		}else{
			$('#atr-sel-pane').hide();
			$('#no-attr-sel').show();
		}
		var s = $('#atr_list').val().split(',');
		var code = $('#code').val();
		if(code != 'grp_adm' && code != 'grp_sup' && code != 'grp_ins' && code != 'grp_mgr'){
			var idval = $(".addedit-edit-name").attr('id');
			if(response.grpId>0){
				if(response.reload &&( $("#edit-name").val()== '' || $('#'+idval).val()== '' )){
					$('#'+idval).addClass('error');
				}else{
					if($('#sel_'+s[s.length-1]).html()=='')
						$('.attr-selected-pane .and-class:last').removeClass('list-group').addClass('empty-group');
					$('#root-admin').data('peoplegroup').callScrollBar();
					$('#root-admin').data('peoplegroup').callMultiSelect($('#attr_open').val(),response.grpId);
					$('#'+idval).removeClass('error');
				}
			}else{
				if(response.reload &&( $("#edit-name").val()== '' ||  $('#'+idval).val()== '' )){
					$('#'+idval).addClass('error');
				}else{
					$('#root-admin').data('peoplegroup').callMultiSelect($('#attr_open').val(),response.grpId);
					$('#'+idval).removeClass('error');
				}
			}
			
		}else if(code == 'grp_ins' || code == 'grp_mgr'){
			var opv = $('#attr_open').val();
			$('#root-admin').data('peoplegroup').callMultiSelect(opv,response.grpId);
			$('#avil_'+opv).append("<div id='hideover'></div>");
			var sty = {'margin-top':-80,
			           'height':$('#avil_'+opv).height(),
			           'width':$('#avil_'+opv).width(),
			           'position':'absolute'};
			$('#hideover').css(sty)
		}
		$('.more-icon-sec').click(function(){
			$('#root-admin').data('peoplegroup').callScrollBar();
		});
		// Prevent enter key from form submit
		$('#role-addedit-form').keypress(function(event){
			if (event.keyCode == 13) {
		        event.preventDefault();
		        return false;
		    }
		});
	},200);
	
	
	return true;
};

function countryBind() {
	$('#grpcontry_filterset').append('<input type="hidden" id="country-sel" value="" >');
	var addlTextFilterACPath = "/?q=administration/people/group/country&aa=44";
	$('#grpcontry-addltext-filter').unbind('.autocomplete');
	$('#grpcontry-addltext-filter') .autocomplete(addlTextFilterACPath,{
		minChars : 3,
		max : 50,
		autoFill : true,
		mustMatch : false,
		matchContains : false,
		inputClass : "ac_input",
		loadingClass : "ac_loading",
		multiple: true,
		formatItem: function(row) { return row[1]; },
		extraParams:{
			id :'country-sel'
		}
	});
	$( "#grpcontry-addltext-filter" ).blur(function() {
		if( $("#grpcontry-addltext-filter").val() != "Type a Country" && $("#grpcontry-addltext-filter").val() != ''){
			$('#grploc-addltext-filter').removeAttr("disabled");
			$('#grploc_filterset').find('.filter-search-start-date-left-bg').removeClass("group-state-disabled-left-bg");
			$('#grploc_filterset').find('.filter-search-start-date-middle-bg').removeClass("group-state-disabled-middle-bg");
			$('#grploc_filterset').find('.filter-search-start-date-right-bg').removeClass("group-state-disabled-right-bg");
			$('#grploc_filterset').find('.filter-search-start-date-middle-bg').focus();
		}else{
			$('#country-sel').val('');
			$('#grploc-addltext-filter').attr("disabled","disabled");
			$('#grploc_filterset').find('.filter-search-start-date-left-bg').addClass("group-state-disabled-left-bg");
			$('#grploc_filterset').find('.filter-search-start-date-middle-bg').addClass("group-state-disabled-middle-bg");
			$('#grploc_filterset').find('.filter-search-start-date-right-bg').addClass("group-state-disabled-right-bg");
			$('#grploc_filterset').find('.filter-search-start-date-middle-bg').val('');
			$('#grploc-addltext-filter').val("Type a State");
			$('#grploc-addltext-filter').css('color', '#999999');
		}
		addlTextFilterACPath = "/?q=administration/people/group/location&country="+$('#country-sel').val();
		$('#grploc-addltext-filter').unbind('.autocomplete');
		$('#grploc-addltext-filter') .autocomplete(addlTextFilterACPath,{
			minChars : 3,
			max : 50,
			autoFill : true,
			mustMatch : false,
			matchContains : false,
			inputClass : "ac_input",
			loadingClass : "ac_loading",
			multiple: true
		});
	});
}

function permdetailsShowHide(ele,id,calltype){
	//console.log(calltype);
	if($(ele).hasClass('more-icon-close')){
		$(ele).removeClass('more-icon-close').addClass('more-icon-open');
	}else{
		$(ele).removeClass('more-icon-open').addClass('more-icon-close');
	}
	if(calltype == 'subparentlevel'){
		$('#permission-td-table .subchild'+id).toggle()
	}else{
		if($('#permission-td-table  .sub-sub-level'+id ).is(":visible")){
			$('#permission-td-table .child'+id+' .more-text a').removeClass('more-icon-open');
			$('#permission-td-table .child'+id+' .more-text a').addClass('more-icon-close'); 
			$('#permission-td-table .sub-sub-level'+id).hide();
		}
		$('#permission-td-table .child'+id).toggle();
	}
	$('#admin-add-scroll').jScrollPane('reinitialise');
	vtip();
}

function setPrivilege(e,p){	
	p = (typeof p=='undefined')?0:p;
	var id = $(e).attr('id');	
	var sp = id.split('_');
	
//	console.log(sp.toSource())
	if(sp[0]=='group' || sp[0]=='subgroup'){
		if(sp[0]=='subgroup'){var sp4 = "_"+sp[4];}else{sp4="";}
		switch(sp[1]){
		case 'view':
			if($(e).is(':checked') === true){
				selectGroup(e,true);
				if(p==1){
					selectGroup($('#'+sp[0]+'_delete_'+sp[2]+'_'+sp[3]+sp4),true,p);
					selectGroup($('#'+sp[0]+'_edit_'+sp[2]+'_'+sp[3]+sp4),true,p);
					selectGroup($('#'+sp[0]+'_create_'+sp[2]+'_'+sp[3]+sp4),true,p);
				}
				if(sp[0]=='group'){
					 $( '.subgroup_view'+'_'+sp[3]).each(function(){					     
					      var st = $(this).attr('id').split('_');													
							selectGroup($('#'+st[0]+'_view_'+st[2]+'_'+st[3]+'_'+st[4]),true);
												      
					     });
				}
			}else{
				selectGroup(e,false);
				if(p==1){
					selectGroup($('#'+sp[0]+'_delete_'+sp[2]+'_'+sp[3]+sp4),false,p);
					selectGroup($('#'+sp[0]+'_edit_'+sp[2]+'_'+sp[3]+sp4),false,p);
					selectGroup($('#'+sp[0]+'_create_'+sp[2]+'_'+sp[3]+sp4),false,p);
				}else{
					selectGroup($('#'+sp[0]+'_delete_'+sp[2]+'_'+sp[3]+sp4),false);
					selectGroup($('#'+sp[0]+'_edit_'+sp[2]+'_'+sp[3]+sp4),false);
					selectGroup($('#'+sp[0]+'_create_'+sp[2]+'_'+sp[3]+sp4),false);
				}
				
				if(sp[0]=='group'){
					 $( '.subgroup_view'+'_'+sp[3]).each(function(){					     
					      var st = $(this).attr('id').split('_');
							selectGroup($('#'+st[0]+'_delete_'+st[2]+'_'+st[3]+'_'+st[4]),false);
							selectGroup($('#'+st[0]+'_view_'+st[2]+'_'+st[3]+'_'+st[4]),false);
							selectGroup($('#'+st[0]+'_create_'+st[2]+'_'+st[3]+'_'+st[4]),false);
							selectGroup($('#'+st[0]+'_edit_'+st[2]+'_'+st[3]+'_'+st[4]),false);						      
					     });
				}
				
			}
			break;
		case 'create':
			if($(e).is(':checked') === true){
				selectGroup(e,true);
				selectGroup($('#'+sp[0]+'_view_'+sp[2]+'_'+sp[3]+sp4),true);
				if(sp[0]=='group'){
					 $( '.subgroup_create'+'_'+sp[3]).each(function(){					     
					      var st = $(this).attr('id').split('_');													
							selectGroup($('#'+st[0]+'_view_'+st[2]+'_'+st[3]+'_'+st[4]),true);
							selectGroup($('#'+st[0]+'_create_'+st[2]+'_'+st[3]+'_'+st[4]),true);					      
					     });
				}
			}else{
				selectGroup(e,false);
				selectGroup($('#'+sp[0]+'_delete_'+sp[2]+'_'+sp[3]+sp4),false);
				selectGroup($('#'+sp[0]+'_edit_'+sp[2]+'_'+sp[3]+sp4),false);
				if(sp[0]=='group'){
					 $( '.subgroup_create'+'_'+sp[3]).each(function(){					     
					      var st = $(this).attr('id').split('_');						
							selectGroup($('#'+st[0]+'_create_'+st[2]+'_'+st[3]+'_'+st[4]),false);	
							selectGroup($('#'+st[0]+'_edit_'+st[2]+'_'+st[3]+'_'+st[4]),false);	
							selectGroup($('#'+st[0]+'_delete_'+st[2]+'_'+st[3]+'_'+st[4]),false);	
					     });
				}
			}
			break;
		case 'edit':
			if($(e).is(':checked') === true){
				selectGroup(e,true);
				selectGroup($('#'+sp[0]+'_view_'+sp[2]+'_'+sp[3]+sp4),true);
				selectGroup($('#'+sp[0]+'_create_'+sp[2]+'_'+sp[3]+sp4),true);
				if(sp[0]=='group'){
					 $( '.subgroup_edit'+'_'+sp[3]).each(function(){					     
					      var st = $(this).attr('id').split('_');						
					      selectGroup($('#'+st[0]+'_view_'+st[2]+'_'+st[3]+'_'+st[4]),true);
						  selectGroup($('#'+st[0]+'_create_'+st[2]+'_'+st[3]+'_'+st[4]),true);					      
						  selectGroup($('#'+st[0]+'_edit_'+st[2]+'_'+st[3]+'_'+st[4]),true);
					     });
				}
			}else{
				selectGroup(e,false);
				selectGroup($('#'+sp[0]+'_delete_'+sp[2]+'_'+sp[3]+sp4),false);
				if(sp[0]=='group'){
					 $( '.subgroup_edit'+'_'+sp[3]).each(function(){					     
					      var st = $(this).attr('id').split('_');						
							selectGroup($('#'+st[0]+'_edit_'+st[2]+'_'+st[3]+'_'+st[4]),false);			
							selectGroup($('#'+st[0]+'_delete_'+st[2]+'_'+st[3]+'_'+st[4]),false);		
					     });
				}
			}
			break;
		case 'delete':
			if($(e).is(':checked') === true){
				
				selectGroup(e,true);
				selectGroup($('#'+sp[0]+'_view_'+sp[2]+'_'+sp[3]+sp4),true);
				selectGroup($('#'+sp[0]+'_create_'+sp[2]+'_'+sp[3]+sp4),true);
				selectGroup($('#'+sp[0]+'_edit_'+sp[2]+'_'+sp[3]+sp4),true);	
				if(sp[0]=='group'){
					 $( ".subgroup_delete"+'_'+sp[3] ).each(function(){					     
					      var st = $(this).attr('id').split('_');
							selectGroup($('#'+st[0]+'_delete_'+st[2]+'_'+st[3]+'_'+st[4]),true);
							selectGroup($('#'+st[0]+'_view_'+st[2]+'_'+st[3]+'_'+st[4]),true);
							selectGroup($('#'+st[0]+'_create_'+st[2]+'_'+st[3]+'_'+st[4]),true);
							selectGroup($('#'+st[0]+'_edit_'+st[2]+'_'+st[3]+'_'+st[4]),true);						      
					     });
				}
			}else{
				selectGroup(e,false);
				if(sp[0]=='group'){
					 $( ".subgroup_delete"+'_'+sp[3] ).each(function(){					     
					      var st = $(this).attr('id').split('_');
							selectGroup($('#'+st[0]+'_delete_'+st[2]+'_'+st[3]+'_'+st[4]),false);
										      
					     });
				}
			}
			break;
		}
	}else{
		if(sp[0]=='sub' || sp[0]=='subchild'){
			if(sp[0]=='subchild'){var sp4 = "_"+sp[4];}else{sp4="";}
		switch(sp[1]){
		case 'view':
			if($(e).is(':checked') === true){
				selectSubLevel(e,true);
				if(p==1){
					selectSubLevel($('#'+sp[0]+'_delete_'+sp[2]+'_'+sp[3]+sp4),true,p);
					selectSubLevel($('#'+sp[0]+'_edit_'+sp[2]+'_'+sp[3]+sp4),true,p);
					selectSubLevel($('#'+sp[0]+'_create_'+sp[2]+'_'+sp[3]+sp4),true,p);
				}
				
			}else{
				selectSubLevel(e,false);
				if(p==1){
					selectSubLevel($('#'+sp[0]+'_delete_'+sp[2]+'_'+sp[3]+sp4),false,p);
					selectSubLevel($('#'+sp[0]+'_edit_'+sp[2]+'_'+sp[3]+sp4),false,p);
					selectSubLevel($('#'+sp[0]+'_create_'+sp[2]+'_'+sp[3]+sp4),false,p);
				}else{
					selectSubLevel($('#'+sp[0]+'_delete_'+sp[2]+'_'+sp[3]+sp4),false);
					selectSubLevel($('#'+sp[0]+'_edit_'+sp[2]+'_'+sp[3]+sp4),false);
					selectSubLevel($('#'+sp[0]+'_create_'+sp[2]+'_'+sp[3]+sp4),false);
				}
				
				
				
			}
			break;
		case 'create':
			if($(e).is(':checked') === true){
				selectSubLevel(e,true);
				selectSubLevel($('#'+sp[0]+'_view_'+sp[2]+'_'+sp[3]+sp4),true);
			}else{
				selectSubLevel(e,false);
				selectSubLevel($('#'+sp[0]+'_delete_'+sp[2]+'_'+sp[3]+sp4),false);
				selectSubLevel($('#'+sp[0]+'_edit_'+sp[2]+'_'+sp[3]+sp4),false);
			}
			break;
		case 'edit':
			if($(e).is(':checked') === true){
				selectSubLevel(e,true);
				selectSubLevel($('#'+sp[0]+'_view_'+sp[2]+'_'+sp[3]+sp4),true);
				selectSubLevel($('#'+sp[0]+'_create_'+sp[2]+'_'+sp[3]+sp4),true);
			}else{
				selectSubLevel(e,false);
				selectSubLevel($('#'+sp[0]+'_delete_'+sp[2]+'_'+sp[3]+sp4),false);
			}
			break;
		case 'delete':
			if($(e).is(':checked') === true){
				selectSubLevel(e,true);
				selectSubLevel($('#'+sp[0]+'_view_'+sp[2]+'_'+sp[3]+sp4),true);
				selectSubLevel($('#'+sp[0]+'_create_'+sp[2]+'_'+sp[3]+sp4),true);
				selectSubLevel($('#'+sp[0]+'_edit_'+sp[2]+'_'+sp[3]+sp4),true);
			}else{
				selectSubLevel(e,false);
			}
			break;
		}
	}
	}
}

function selectGroup(e,st,p){
	p = (typeof p=='undefined')?0:p;
	var id = $(e).attr('id');
	var si = new Array();
	var n = 0;
	//console.log(id)
	var sp = id.split('_');
	//var child = (sp[0] == 'subgroup') ? 'subchild' : 'sub';
	
	
	if(sp[0] == 'subgroup'){
		si[0] = 'subchild_'+sp[1]+'_'+sp[3]+'_'+sp[4];
		n=1;
	}else{
		si[0] = 'sub_'+sp[1]+'_'+sp[3];
		si[1] = 'subchild_'+sp[1]+'_'+sp[3]+'_'+sp[4];
		si[2] = 'subgroup_'+sp[1]+'_'+sp[3]+'_'+sp[4];
		n=3;
	}
	var ac,rc;
	for(var i=0;i<n;i++){
	if(document.getElementById(si[i])!=null)
		p = ($('input[obname="'+si[i]+'"]').parent().attr('class').indexOf('readonly')>0)?1:0;
	(p==1)?(ac='checkbox-selected-readonly',rc='checkbox-unselected-readonly'):
			(ac='checkbox-selected',rc='checkbox-unselected');
	if(st === true){
		$(e).attr('checked','checked')
		   .parent()
           .removeClass(rc)
           .addClass(ac);
		$('input[obname="'+si[i]+'"]').each(function(){
			p = ($(this).parent().attr('class').indexOf('readonly')>0)?1:0;
			(p==1)?(ac='checkbox-selected-readonly',rc='checkbox-unselected-readonly'):
					(ac='checkbox-selected',rc='checkbox-unselected');
			$(this).attr('checked','checked')
	           .parent()
	           .removeClass(rc)
	           .addClass(ac);
			if($(this).attr('isParent')==1){
				var id1 = $(this).attr('id');
				var sp1 = id1.split('_');
				
				selectSubLevel($('#sub_delete_'+sp1[2]+'_'+sp1[3]),true,1);
				selectSubLevel($('#sub_edit_'+sp1[2]+'_'+sp1[3]),true,1);
				selectSubLevel($('#sub_create_'+sp1[2]+'_'+sp1[3]),true,1);				
			}
	     });
		
	}else{
		$(e).removeAttr('checked')
		   .parent()
           .removeClass(ac)
           .addClass(rc);
		$('input[obname="'+si[i]+'"]').each(function(){
			p = ($(this).parent().attr('class').indexOf('readonly')>0)?1:0;
			(p==1)?(ac='checkbox-selected-readonly',rc='checkbox-unselected-readonly'):
					(ac='checkbox-selected',rc='checkbox-unselected');
			$(this).removeAttr('checked')
	           .parent()
	           .removeClass(ac)
	           .addClass(rc);
			if($(this).attr('isParent')==1){
				var id1 = $(this).attr('id');
				var sp1 = id1.split('_');
				selectSubLevel($('#sub_delete_'+sp1[2]+'_'+sp1[3]),false,1);
				selectSubLevel($('#sub_edit_'+sp1[2]+'_'+sp1[3]),false,1);
				selectSubLevel($('#sub_create_'+sp1[2]+'_'+sp1[3]),false,1);
			}
	     });
	}
	}
}

function selectSubLevel(e,st,p){
	p = (typeof p=='undefined')?0:p;
	var id = $(e).attr('id');
	
	
	var sp = id.split('_');
	if(sp[0]=='subchild'){
		var sp4 = "_"+sp[4];		
		}else{
			sp4="";
	}	
	var par = (sp[0] == 'subchild') ? 'subgroup' : 'group';
	var gsi = par+'_'+sp[1]+"_"+sp[3]+sp4;
	var si = sp[0]+'_'+sp[1]+"_"+sp[3]+sp4;
	

	var ac,rc,ac1,rc1;
	if(document.getElementById(si)!=null)
		p = ($('input[obname="'+si+'"]').parent().attr('class').indexOf('readonly')>0)?1:p;
	(p==1)?(ac='checkbox-selected-readonly',rc='checkbox-unselected-readonly'):
			(ac='checkbox-selected',rc='checkbox-unselected');
	if(st === true){
		$(e).attr('checked','checked')
		   .parent()
           .removeClass(rc)
           .addClass(ac);
		var tc = $('input[obname="'+si+'"]').length;
		var sc = $('input[obname="'+si+'"]').filter(':checked').length;
		if(tc == sc){
			p = ($('input[obname="'+gsi+'"]').parent().attr('class').indexOf('readonly')>0)?1:0;
			(p==1)?(ac='checkbox-selected-readonly',rc='checkbox-unselected-readonly'):
					(ac='checkbox-selected',rc='checkbox-unselected');
			$('input[obname="'+gsi+'"]').attr('checked','checked')
	           .parent()
	           .removeClass(rc)
	           .addClass(ac);
		}
		
	}else{
		$(e).removeAttr('checked')
		   .parent()
           .removeClass(ac)
           .addClass(rc);
		p = ($('input[obname="'+gsi+'"]').parent().attr('class').indexOf('readonly')>0)?1:0;
		(p==1)?(ac='checkbox-selected-readonly',rc='checkbox-unselected-readonly'):
				(ac='checkbox-selected',rc='checkbox-unselected');
		$('input[obname="'+gsi+'"]').removeAttr('checked')
           .parent()
           .removeClass(ac)
           .addClass(rc);
	}
}

function resetPopPosision(op){
	if(op.indexOf('qtip_addusers_visible_disp')>=0){
		if($.browser.msie){
			$('#'+op+' #bubble-face-table').css('bottom','12px');
		}
	}
}
$(document).ajaxComplete(function( event, xhr, settings ) {
    var targetUrl = $('#root-admin').data('peoplegroup').constructUrl("administration/people/groups/attributs");
    var target = $(".page-administration-people-group .ui-multiselect-con-class:visible");  
    if(settings.url.indexOf(targetUrl) > -1 && xhr.responseText != undefined && xhr.responseText != '' && $.parseJSON(xhr.responseText).data.length <= 5) {           
            target.find('.mcontainerclass').css('height', 'auto');
            target.find('.create-group-hidecontainer').css('margin-bottom', '10px');
            target.css('height', 'auto');            
    }
    target.css('width', '257px'); 
    target.find("#multiautocomplete").css('width', '230px');
    $(".page-administration-people-group").find(".access-mro-visibility .ui-multiselect-con-class").css('width','270px');
    // Check for background jobs
    var bj = $('#background_job').val();
    if(bj > 0){
	    var error = new Array();
		error[0] = Drupal.t('MSG921');
		var message_call = expertus_error_message(error,'error');
		$('#show_expertus_message').show();
		$('#show_expertus_message').html(message_call);
	}
    
});
function getUsersearchType(){
	try{
    	var gptype= $('#search_all_user_type-hidden').val();
    	$('#hiddengptype').val(gptype);
	}catch(e){
		// To Do
	}
    }
function updHireDate(elem) {
	try {
		var selectedVal = $(elem).val();
		var elementId = $(elem).attr('name');
		if (elementId.indexOf('hire_start')>=0)
			$('#hidden_'+elementId).val(selectedVal);
		else 
			$('#hire_end').val(selectedVal);
		
		//Below code responsible for 
		//While selecting date in unselected area the element needs to be selected automatically
		//Also change the date in selected area needs to be update in right panel
		if(elementId.indexOf('hire_start')>=0){
		
			id = $(elem).attr('id').replace('hire_start_','');
			if($('#'+id).is(":checked")==false && elementId.indexOf('ste_con_hdt_btw') == -1){
				$('#'+id).attr('checked','checked');
				$('#'+id).click();
			}else if($('#'+id).is(":checked")==true){
				var vl = $('#avil_ste_con_hdt').expmultiselect('getValue','selected');
				$.each(vl, function(){
					if(this.value == id){
						res = '{"value":"'+this.value+'","title":"'+unescape(this.title).replace(/"/g, '\&quot;').replace(/\\/g, '&#92;')+'","selected":"'+this.selected+'"}';
					}
				});
				
				$msobj = $("#root-admin").data('peoplegroup');
				$msobj.onselectfn($('#avil_ste_con_hdt'),JSON.parse(res),$msobj);
			}
		}else if(elementId.indexOf('hire_end')>=0){
			id = $(elem).attr('id').replace('hire_end_','');
			if($('#'+id).is(":checked")==false && elementId.indexOf('ste_con_hdt_btw') != -1){
				$('#'+id).attr('checked','checked');
				$('#'+id).click();
			}else if($('#'+id).is(":checked")==true){
				var vl = $('#avil_ste_con_hdt').expmultiselect('getValue','selected');
				$msobj = $("#root-admin").data('peoplegroup');
				$msobj.onselectfn($('#avil_ste_con_hdt'),vl[0],$msobj);
			}
		}
	} catch(e) {
		console.log(e);
	}
}