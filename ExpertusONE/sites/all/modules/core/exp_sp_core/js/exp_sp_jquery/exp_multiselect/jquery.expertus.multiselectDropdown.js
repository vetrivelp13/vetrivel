(function($) {
	$.widget("exp.expmultiselectDropdown", {
options: {
		listmodel :{id:'opt',title:'val',dispTitle:'dispval',selected:'sel'},
		sublistmodel :{enable:'dropdown',defaultval:'defaultval'},
		optlistmodel :{id :'opt',title:'val'},
		data:'',
		dataType:'',
		searchfilter:{url:'',enable:false},
		helpText: {autocomplete:'Search',checkAll:'Check All',uncheckAll:'Uncheck All'},
		header: true,
		height: 275,
		width: 310,
		classes: '',
		checkAll:false,
		uncheckAll:false,
		noneSelectedText:'',
		titlelength:'',
		rownum:5,
		url:'',
		onselect:'',
		onunselect:'',
		onload:'',
		ondropdown:'',
		selectedText: "#",
		selectedList: 0,
		show: '',
		hide: '',
		sort:'asc',
		autoOpen: false,
		multiple: true,
		optional :{optionlist:'',enable:true},
		position: {},
		widget:''
	},
	data1 : '',
	elem:'',
	elemid:'',
	recs : 1,
	page :1,
	reqcnt :0,
	timer:0,
	selected:'',
	selId:[],
	
_init: function(){
	if(this.options.url != '')
		this._scrollevent();
	this._onclickHideShow();
},

_create: function(){
	  var o = this.options,
		html ='',
		auto = '',
		chkAll = '',
		chk= '<ul style="width:100%;"></ul>',
		unchk = '<ul style="width:100%;"></ul>',
		unchkAll = '',
		cHight=o.height - 30;
 	 this.elem = this.element;
 	 this.elemid = $(this.elem).attr('id');
 	 if(o.searchfilter.enable == true){
 		 auto = '<div id ="autocomplete" class = "autocomplete" ><input type="text" name="autocomplete" id="multiautocomplete" value="'+o.helpText.autocomplete+'"><span><div id ="autoimg"></div></span></div>';
 		cHight = o.height - 65;
 	 }
 	 if(this.options.checkAll == true)
		 chkAll +='<div><ul style="width:100%;"><li style="display:none;"><input type="checkbox" name="checkall" id="selectcheckall"><span for="checkall">'+o.helpText.checkAll+'</span></li></ul></div>';
	 
	 if(this.options.uncheckAll == true)
		 unchkAll +='<div><ul style="width:100%;"><li style="display:none;"><input type="checkbox" checked = "" name="checkall" id="selectuncheckall"><span for="uncheckall">'+o.helpText.uncheckAll+'</span></li></ul></div>';
 	 
 	html +='<div id ="ui-multiselect-con" class = "ui-multiselect-con-class" >'+auto+'<div class = "no-rec-msg" style="display:none;">'+Drupal.t('MSG403')+'</div><div id = "container" class ="mcontainerclass"><div class="outer-container">'+unchkAll+'<div id ="chk">'+chk+'</div><div id = "optionallist"></div><div class="line-separator" style="display:none;"/>'+chkAll+'<div id ="unchk">'+unchk+'</div><div id ="overflowitems" class ="overflowitems"></div></div>';
 	// Created footeraccess Div For The Learner Group Option is Not showing Correctly #0039479
 	html +='<div class="footeraccess" style="height:60px;display:block">&nbsp;</div></div></div>';
 	$(this.elem).append(html);
    $(this.elem).find('#optionallist').hide();
    $(this.elem).find('#ui-multiselect-con')
    	.addClass(o.classes)
		.css({'width':o.width,'height':o.height});
    $(this.elem).find('#container')
		.css({'height':cHight});
    if(o.searchfilter.enable == true){
	    $(this.elem).find('#multiautocomplete')
		.css({'width':o.width-40});
	    this._autoHelpText();
    }
    
    if(o.url != '')  
    	this._constructajaxcall(); 
    else{ 
    	this._dataLoad(o.data);
    	this._callScroll();
    }
    this._binding();
    this._sortlist();
    this.autocompleteInit();
    if(typeof(o.onload) == 'function')
    	o.onload(this.elem,o.widget);
    
},
_dataLoad : function(data){
	var obj = this;
	this.data1 = data;
	var trimTitle;
	var learnerClassAdd;
	var	l = this.options.listmodel,
		sl = this.options.sublistmodel,
		ol = this.options.optlistmodel,
		opl =obj.options.optional.optionlist, 
		selstyle = "display:none",
		unselstyle = "display:block",
		optlist = '',
		chk= '',
		unchk = '',
		dval = '',
		defval = this._defaultoptVal(opl);
	if(data.length > 0){
		$(this.elem).find('.no-rec-msg').hide();
		$(this.elem).find('#container').show();
    $.each(data, function() {
    	if(this[l.dispTitle] == null || this[l.dispTitle] == undefined)
			this[l.dispTitle] = this[l.title];
    	var select;
    	if(obj.options.checkAll==true && obj.options.uncheckAll==true)
    		select = ($(obj.elem).find('#selectcheckall').is(':checked')== true) ? 1 : this[l.selected] ;
    	else{
    		select = this[l.selected];
    	}
     if(obj.options.optional.enable == true && this[sl.enable] == 'enable'){
             trimTitle = titleRestrictionFadeoutImage(this[l.dispTitle],'learner-group-names');
             learnerClassAdd="learner-group-container";
     } else {
    	 var titlelengthchars = obj.options.titlelength != '' ? obj.options.titlelength : 30;
             trimTitle = titleRestrictionFadeoutImage(this[l.dispTitle],'group-names');
             learnerClassAdd="";
     }
		if(select == 1 && $.inArray(this[l.id], obj.selId) < 0){
			//var titlelengthchars = obj.options.titlelength != '' ? obj.options.titlelength : 30;
			//if(titlelengthchars != 30) {
			//	trimTitle = obj._titleRestricted(this[l.title], titlelengthchars);
			//}
			
		   chk += '<li class='+learnerClassAdd+'>';
			   chk += '<input type="checkbox" checked = "" name="' + unescape(this[l.title]).replace(/"/g, '\&quot;') + '" id="' + this[l.id] + '"><span id ="li-listitem" for="' + unescape(this[l.title]).replace(/"/g, '\&quot;') + '" class="li-selected label-text" title="' + unescape(this[l.title]).replace(/"/g, '\&quot;') + '">' + trimTitle + '</span>';
			   unchk += '<li style ='+selstyle+'><input type="checkbox" name="' + unescape(this[l.title]).replace(/"/g, '\&quot;') + '" id="' + this[l.id] + '" opt="'+this[sl.enable]+'"><span class="label-text" for="' + unescape(this[l.title]).replace(/"/g, '\&quot;') + '" title="' + unescape(this[l.title]).replace(/"/g, '\&quot;') + '">' +  titleRestrictionFadeoutImage(this[l.dispTitle],'check-groupnames') + '</span></li>';
		   if(obj.options.optional.enable == true && this[sl.enable] == 'enable'){
			   dval = (this[sl.defaultval] != null) ? this[sl.defaultval] : opl[0][ol.id];
			   chk += '<div class="option-div"><span class="grey-pointer"></span><span id = "ui-opt-span" class="ui-opt-span" ><span class="selected-opt1" id ="option-'+ this[l.id] +'" val ="'+dval+'">'+defval[dval]+'</span><div id ="dropdown-'+ this[l.id] +'" class = "dropdown background"></div></span></div>';
		   }
		   chk += '</li>'; 
		   // Call onselect call back handler when populate data in selected list.
		   if(typeof(obj.options.onselect) == 'function' && obj.selected.indexOf(this[l.id])<=0 && this[l.selected] == 0){
			    var vl = '{"value":"'+this[l.id]+'","title":"'+unescape(this[l.title]).replace(/"/g, '\&quot;')+'","selected":"true"}';
				obj.options.onselect(obj.elem,JSON.parse(vl),obj.options.widget);
		   }
		}else{
		   var titlelengthchars = obj.options.titlelength != '' ? obj.options.titlelength : 30;
		   var style = ($.inArray(this[l.id], obj.selId) !== -1)?selstyle:unselstyle;
			   unchk += '<li style ='+style+' class = "visible" ><input  type="checkbox" name="' + unescape(this[l.title]).replace(/"/g, '\&quot;') + '" id="' + this[l.id] + '" opt="'+this[sl.enable]+'" ><span class="label-text" for="' + unescape(this[l.title]).replace(/"/g, '\&quot;') + '" title="' + unescape(this[l.title]).replace(/"/g, '\&quot;') + '">' +  titleRestrictionFadeoutImage(this[l.dispTitle],'group-name') + '</span></li>';
		}
    });
    // While loading data if there is any previously selected items available load that items
    if(obj.selected != ''  && obj.selectLoaded==false){
    	$(this.elem).find('#chk ul').append(obj.selected);
		$.each($(obj.selected),function(){
			$(obj.elem).find('input[id="'+$(this).find('input').attr('id')+'"]').attr('checked','checked');
			obj._clickfuntion($(obj.elem).find('input[id="'+$(this).find('input').attr('id')+'"]'));
			obj._labelBind($(obj.elem).find('input[id="'+$(this).find('input').attr('id')+'"]').next());
		});
		obj.selectLoaded=true;
    } 
    
    
	 $(this.elem).find('#unchk ul').append(unchk);
	 $(this.elem).find('#chk ul').append(chk);
	}else{
		 $(this.elem).find('.no-rec-msg').show();
		 $(this.elem).find('#container').hide();
		// $(this.elem).find('.no-rec-msg').css('padding-top','50px');
	 }
	 
	// While search bu autocomplete, display only the matching items in checked list
		if(obj.options.searchfilter.enable == true){
			var srval = $(this.elem).find('#multiautocomplete').val().toLowerCase();
			srval = srval == ((obj.options.helpText.autocomplete).toLowerCase())?'':srval;
			if(srval!=''){
				$.each($(this.elem).find('#chk ul li'), function(){
					if($(this).find('span:first').html().toLowerCase().indexOf(srval)<0){
						$(this).find('span:first').parent().css('display','none');
					}else{
						$(this).find('span:first').parent().css('display','block');
					}
				});
				if($(obj.elem).find('#chk ul li input').length==0){
					
				}
			}else{
				$.each($(this.elem).find('#chk ul li'), function(){
					$(this).find('span:first').parent().css('display','block');
				});
			}
		}
	 
	 if($(this.elem).find('#optionallist').html()==''){
		 $.each(opl, function() { 
	    	optlist +='<li id="'+this[ol.id]+'">'+this[ol.title]+'</li>';
		   });
     	$(this.elem).find('#optionallist').append(optlist);
	 }
     $(this.elem).find('#optionallist').hide();
     
     // Show/Hide "Select All" Option
    $(this.elem).find('#selectcheckall').parent()
     	[ (this.options.checkAll == true && $('#'+this.elemid+' #unchk ul li').filter(function() {
	 	    return $(this).css('display') !== 'none';
	 	}).length > 1) ? 'show' : 'hide' ]();
     
    // Show/Hide "Unselect All" Option with checked attribute
 	if(this.options.uncheckAll == true && $('#'+this.elemid+' #chk ul li').filter(function() {
 	    return $(this).css('display') !== 'none';
 	}).length > 1) {
 		$(this.elem).find('#selectuncheckall').attr('checked','checked');
 		$(this.elem).find('#selectuncheckall').parent().show();
 	}else{
 		$(this.elem).find('#selectuncheckall').removeAttr('checked');
 		$(this.elem).find('#selectuncheckall').parent().hide();
 	}
	
	$.each(data, function() {
		 obj._clickfuntion($(obj.elem).find('input[id="'+this[l.id]+'"]'));
		 if(obj.options.optional.enable == true && this[sl.enable] == 'enable'){
		     obj._dropdownHideShow('dropdown-'+this[l.id]); // this is newly added for img click
        }
		 obj._labelBind($(obj.elem).find('input[id="'+this[l.id]+'"]').next());
	 });
	$.each(opl, function() { 
            obj._dropdownElementclick('#'+this[ol.id]);
     });
	
	if($(this.elem).find('#unchk ul li').filter(function() {
 	    return $(this).css('display') !== 'none';
 	}).length <= 0 || $(this.elem).find('#chk ul li').filter(function() {
 	    return $(this).css('display') !== 'none';
 	}).length <= 0)
		 $(this.elem).find('.line-separator').hide();
    else
    	 $(this.elem).find('.line-separator').show();
	
	if(typeof(obj.options.afterload) == 'function') {
		obj.options.afterload(obj);
	}
},
_binding : function() {
	var theme = Drupal.settings.ajaxPageState.theme;
	var obj = this,
		elm = obj.elem;
	$(this.elem).find('#selectcheckall').unbind('click')
		.click(function () {
			obj._multiselectcheckAll();
		});
	$(this.elem).find('#selectuncheckall').unbind('click')
		.click(function () {
			obj._multiselectuncheckAll();
		});
	if(this.options.searchfilter.enable == true){
		$(this.elem).find('#autoimg').bind('click',function(){
			$('#exp-sp-administration-catalog-access-addedit-form-html .outer-container').css('z-index','-1').css('position','absolute');
			obj._searchautocomplete();
			
		});
		/*-- 37534: Code Re-Factoring - Browser Compatibility Fix --*/
		$(this.elem).find('#multiautocomplete').bind('keyup',function(){
			if(parseInt($((obj.elem).find('#multiautocomplete').val().length)>2 ||
					$(obj.elem).find('#multiautocomplete').hasClass('ac_loading')==true) && theme == 'expertusoneV2'){
				$(obj.elem).find('#autoimg').hide();
			}
		});
		$(this.elem).find('#multiautocomplete').bind('blur',function(){
			$(obj.elem).find('#autoimg').show();
		});
	}
	
	$(elm).find('#selectcheckall').bind('click',function(){
		var srval = $(obj.elem).find('#multiautocomplete').val().toLowerCase();
		srval = srval == ((obj.options.helpText.autocomplete).toLowerCase())?'':srval;
		if(srval ==''){
			obj.options.onselectAll(elm,obj.options.widget);
		}
	});
	$(elm).find('#selectuncheckall').bind('click',function(){
		var srval = $(obj.elem).find('#multiautocomplete').val().toLowerCase();
		srval = srval == ((obj.options.helpText.autocomplete).toLowerCase())?'':srval;
		if(srval ==''){
			obj.options.onunselectAll(elm,obj.options.widget);
		}
	});

	// Prevent enter key from form submit
	$(elm).keypress(function(event){
		if (event.keyCode == 13) {
	        event.preventDefault();
	        return false;
	    }
	});
},
_labelBind:function(ele){
	var obj = this;
	$(ele).bind('click',function(){
		obj._selectItem(this);
	});
},

_selectItem: function(ele){
	var i = $(ele).prev();
	if($(i).is(":checked")==true){
		$(ele).prev().removeAttr('checked');
		$(ele).prev().click();
	}else{
		$(ele).prev().attr('checked','checked');
		$(ele).prev().click();
	}
},
_moveselecteditems : function(ele){
	$(ele).parent()
		.removeClass('visible')
		.css('display', 'none');	
	var id = $(ele).attr('id'),
		obj = this,
	ol = obj.options.optlistmodel,
	opl =obj.options.optional.optionlist,
	defval = obj._defaultoptVal(opl); 
	html = $(ele).parent().html();
	if(obj.options.optional.enable == true && $(ele).attr('opt') == 'enable'){
		//38262: Unable to set up MRO access in Course/Class and TP added by yogaraja
		var k = html.substr(html.indexOf('">')+2,html.toLowerCase().indexOf('</span'));
		k = k.substr(k.indexOf('">')+2,k.toLowerCase().indexOf('</span'));
		var rpl = ($.browser.msie && $.browser.version == 8)?'</SPAN>':'</span>';
		y= k.replace(rpl,'');
		if(y.indexOf('...')>0 || y.length > obj.options.titlelength-18){
            if(y.indexOf('...')>0){
            	var z = y.substr(0,y.indexOf("...")-18)+"...";
            }else{
            	var z = y.substr(0,obj.options.titlelength-18)+"...";
            }
			html = html.replace(new RegExp(k, "ig"), z);	// to replace title+</span> in html ignoring case
		}
		html += '</span><div class="option-div"><span class="grey-pointer"></span><span id = "ui-opt-span" class="ui-opt-span" ><span class="selected-opt1" id ="option-'+id+'" val ="'+opl[0][ol.id]+'">'+defval[opl[0][ol.id]]+'</span><div id ="dropdown-'+id+'" class = "dropdown background"></div></span></div>';
	}
	html = '<li>'+html+'</li>';
	$(this.elem).find("#chk ul").append(html);
	var el = $(this.elem).find('#chk input[id="'+id+'"]');
	
	$(this.elem).find('#chk input[id="'+id+'"]').attr('checked','checked');
	
	// Show "Unselect All" option if there are 2 or more list items are available
	var lst = $('#'+this.elemid+' #chk ul li').filter(function() {
				    return $(this).css('display') !== 'none';
				}).length;
	if(this.options.uncheckAll == true){
		if(lst > 1){
			$(this.elem).find('input[id="selectuncheckall"]')
				.attr('checked','checked')
				.parent()
				.css('display', 'block');
		}else if(lst == 1){
			$(this.elem).find('input[id="selectuncheckall"]')
				.attr('checked','checked')
				.parent()
				.css('display', 'none');
		}
	}
	
	// Hide "Select All" option if there are less than 2 list items are available
	if($('#'+this.elemid+' #unchk ul li').filter(function() {
		    return $(this).css('display') !== 'none';
		}).length <= 1) {
		$(this.elem).find('input[id="selectcheckall"]')
		.removeAttr('checked')
		.parent()
		.css('display', 'none');
	}
	
	// Show or hide the separator line 
	if($('#'+this.elemid+' #unchk ul li').filter(function() {
		    return $(this).css('display') !== 'none';
		}).length <= 0 || $('#'+this.elemid+' #chk ul li').filter(function() {
		    return $(this).css('display') !== 'none';
		}).length <= 0)
		 $(this.elem).find('.line-separator').hide();
	 else
		 $(this.elem).find('.line-separator').show();
	
	this._sortlist();
	this._callScroll();
	this._clickfuntion(el);
	this._labelBind($(el).next());
	this._dropdownHideShow('dropdown-'+id);
	if(typeof(this.options.onselect) == 'function')
		this.options.onselect(this.elem,JSON.parse(this._getJson(el,'{"optId":"","optval":""}')),this.options.widget);
	
},
_getJson: function(el,opt){
	//change by ayyappans for 40441: Unable to deselect group names in access popup
	return '{"value":"'+$(el).attr('id')+'","title":"'+unescape($(el).attr('name')).replace(/"/g, '\&quot;').replace(/\\/g, '&#92;')+'","selected":"'+$(el).is(':checked')+'","selectedOption":'+opt+'}';
},
_moveunselecteditems : function(ele){
	var rdata = JSON.parse(this._getJson(ele,'{"optId":"","optval":""}'));
	$(ele).parent().remove();
	var id = $(ele).attr('id');
	var el = $(this.elem).find('#unchk input[id="'+id+'"]');
	$(el).removeAttr('checked');
	$(el).parent()
		.addClass('visible')
		.css('display', 'block');
	
	var lst = $('#'+this.elemid+' #unchk ul li').filter(function() {
		 	    return $(this).css('display') !== 'none';
		 	}).length;
	if(this.options.checkAll == true){
		if(lst > 1){
			$(this.elem)
			.find('input[id="selectcheckall"]')
				.removeAttr('checked')
				.parent()
				.css('display', 'block');
		}else if(lst == 1){
			$(this.elem).find('input[id="selectcheckall"]')
				.removeAttr('checked')
				.parent()
				.css('display', 'none');
		}
	}
	// Disable the "Unselect All" option if there are less than 2 items in selected list
	if($('#'+this.elemid+' #chk ul li').filter(function() {
 	    return $(this).css('display') !== 'none';
 	}).length <= 1) {
		$(this.elem).find('input[id="selectuncheckall"]')
		.removeAttr('checked')
		.parent()
		.css('display', 'none');
	}
	
	// Show or Hide the separator line
	if($('#'+this.elemid+' #unchk ul li').filter(function() {
 	    return $(this).css('display') !== 'none';
 	}).length <= 0 || $('#'+this.elemid+' #chk ul li').filter(function() {
 	    return $(this).css('display') !== 'none';
 	}).length <= 0)
		 $(this.elem).find('.line-separator').hide();
     else
    	 $(this.elem).find('.line-separator').show();
	
	this._sortlist();
	this._callScroll();
	if(typeof(this.options.onunselect) == 'function')
		this.options.onunselect(this.elem,rdata,this.options.widget);
},
_callScroll: function(){
	var o = this.options;
	/*$(this.elem).find('#chk ul li .li-selected')
		.css('width',o.width/2-10);*/
	$(this.elem).find('#chk ul li .option-div')
		.css('width',o.width/2-25);
	if(this.data1.length >8 || this.recs > this.options.rownum){
		if($(this.elem).find('.jspContainer').length==0){
			$(this.elem).find('#container').jScrollPane({});
		}else{
			$(this.elem).find('#container').jScrollPane('destroy');
			$(this.elem).find('#container').jScrollPane('reinitialise');
		}
	}
},
_clickfuntion : function(ele){
	var obj = this;
	$(ele).bind('click',function(){
		if($(this).is(':checked') == true)
			obj._moveselecteditems(this);
		else
			obj._moveunselecteditems(this);
	});	
},
_constructajaxcall : function(){
		var res = '';
		var obj = this;
	var url = this.options.url;
	var ht = this.options.helpText.autocomplete
		var get = '';
		url += (url.indexOf('?')>=0) ? '&page='+this.page+'&rows='+this.options.rownum : '?page='+this.page+'&rows='+this.options.rownum ;
	if(this.options.searchfilter.enable == true){
		get = $(this.elem).find('#multiautocomplete').val();
			get = (get!= '' && get != ht)?'&z='+encodeURIComponent(get) :'';
			url += get ;
		}
	var pg = this.options.rownum*this.reqcnt;
	if(pg < this.recs){
		$(this.elem).find('#container').addClass('loader');
		
		obj.timer = 1;
		setTimeout(function(){
			jQuery.ajax({
				   type: "POST",
				   url: url,
				   processData: false,
				   contentType:'text/xml',
				   async: false,
				   success: function(respText){
					  res = respText;
					 // obj.timer = 0;
			  		}
				 });
			obj.reqcnt++;
			obj.recs = res.records;
			obj.page++;
			if(obj.reqcnt == 1)
				obj._clearlist();
			obj._dataLoad(res.data);
			$(obj.elem).find('#container').removeClass('loader');
			$('#exp-sp-administration-catalog-access-addedit-form-html .outer-container').css('z-index','').css('position','');
			if(res.data.length > 8)
				obj._callScroll();
	        else
	            $(obj.elem).find('#container').jScrollPane('destroy');
			obj.timer = 0;
		},50);
	}
},
_multiselectcheckAll :function(){
	var b = this;
	$(this.elem).find('#container').addClass('loader');
	setTimeout(function(){
		$(b.elem).find('input[id="selectcheckall"]')
			.attr('checked','checked')
			.parent()
			.css('display', 'none');
		$(b.elem).find('input[id="selectuncheckall"]')
			.attr('checked','checked')
			.parent()
			.css('display', 'block');
		
		var srval = $(b.elem).find('#multiautocomplete').val().toLowerCase();
		srval = srval == ((b.options.helpText.autocomplete).toLowerCase())?'':srval;
		if(srval == ''){
			// Remove visible class to all the items from unselcted list 
			$(b.elem).find('#unchk ul li').css('display','none').removeClass('visible');
			
			// Copy the entire unselected list into selected list
			$(b.elem).find('#chk ul').html($(b.elem).find('#unchk ul').html());
			
			// Add visible class to all the items in selected list
			$(b.elem).find('#chk ul li').css('display','block').addClass('visible');
			
			// Mark the input as checked
			$(b.elem).find('#chk ul li input').attr('checked','checked');
			
			// Call a bunding for each input items
			$(b.elem).find('#chk ul li input').each(function(){
				$(this).bind('click',function(){
					b._moveunselecteditems(this);
				});	
			});
			$(b.elem).find('#chk ul li .label-text').each(function(){
		 		b._labelBind(this);
		 	});
		}else{
			$(b.elem).find('#unchk ul li').each(function (){
				if($(this).css('display') != 'none'){
					//$(this).children().children().attr('checked','checked');
					b._moveselecteditems($(this).children());
				}
			});
		}
		// Show/Hide separator 
		if($(b.elem).find('#unchk ul li').filter(function() {
	 	    return $(this).css('display') !== 'none';
	 	}).length <= 0 || $(b.elem).find('#chk ul li').filter(function() {
	 	    return $(this).css('display') !== 'none';
	 	}).length <= 0)
			 $(b.elem).find('.line-separator').hide();
	    else
	    	 $(b.elem).find('.line-separator').show();
		// Remove loader
		$(b.elem).find('#container').removeClass('loader');
	},200);
},
_multiselectuncheckAll :function(){
	var b = this;
	$(this.elem).find('#container').addClass('loader');
	setTimeout(function(){
		$(b.elem).find('input[id="selectuncheckall"]')
			.removeAttr('checked')
			.parent()
			.css('display', 'none');
		$(b.elem).find('input[id="selectcheckall"]')
			.removeAttr('checked')
			.parent()
			.css('display', 'block');
		
		var srval = $(b.elem).find('#multiautocomplete').val().toLowerCase();
		srval = srval == ((b.options.helpText.autocomplete).toLowerCase())?'':srval;
		if(srval ==''){
			// Remove all listed items from checked list
			$(b.elem).find('#chk ul li').remove();
			
			// Show the items in uncheck list
			$(b.elem).find('#unchk ul li').css('display','block');
			$(b.elem).find('#unchk ul li').each(function (){
				if($(this).css('display') != 'none'){
					$(this).children().removeAttr('checked');
				}
			});
		}else{
			$(b.elem).find('#chk ul li').each(function (){
				if($(this).css('display') != 'none'){
					$(this).children().removeAttr('checked');
					b._moveunselecteditems($(this).children());
				}
			});
		}
		// Show/Hide separator 
		if($(b.elem).find('#unchk ul li').filter(function() {
	 	    return $(this).css('display') !== 'none';
	 	}).length <= 0 || $(b.elem).find('#chk ul li').filter(function() {
	 	    return $(this).css('display') !== 'none';
	 	}).length <= 0)
			 $(b.elem).find('.line-separator').hide();
	    else
	    	 $(b.elem).find('.line-separator').show();
		
		// Remove loader
		$(b.elem).find('#container').removeClass('loader');
	},200);
},

_sortlist :function(){
	var mylist = $(this.elem).find('#chk ul'); 
	var srt = this.options.sort;
	var listitems = mylist.children('li').get();
	listitems.sort(function(a, b) {
		if(srt == 'desc')
			return $(b).text().toUpperCase().localeCompare($(a).text().toUpperCase());
		else
			return $(a).text().toUpperCase().localeCompare($(b).text().toUpperCase());
	});
	$.each(listitems, function(idx, itm) { mylist.append(itm); });

},

_scrollevent : function(){
	var obj = this;
	$(this.elem).find( "#container" ).scroll(function() {
		if( $(obj.elem).find('#overflowitems').length > 0 ) { // if target element exists in DOM
			if( $(obj.elem).find('#overflowitems').is_on_screen()){ //{ // if target element is visible on screen after DOM loaded
				if(obj.timer==0){
					obj.timer=1;
					obj._constructajaxcallPreProcess();
				}
			}
			
		}
	});
},

_constructajaxcallPreProcess: function(){
	var obj = this;
	setTimeout(function(){if(obj.timer==1){obj._constructajaxcall(false);}},500);
},

autocompleteInit : function(){
	$(this.elem).find('#multiautocomplete').unbind(".autocomplete");
	$(this.elem).find('#multiautocomplete').autocomplete(
			this.options.searchfilter.url,{
			minChars :3,
			max :50, 
			autoFill :true,
			mustMatch :false,
			matchContains :false
	});
},
_searchautocomplete : function(){
	if(this.options.searchfilter.enable == true && this.options.searchfilter.url != ''){
		this.recs = 1;
		this.page = 1;
		this.reqcnt = 0;
		this._constructajaxcall();
	}
},
_dropdownHideShow : function(ele){
   var obj = this;
    $(this.elem).find('#'+ele).bind('click',function(){
     obj._dropdownClick(this);
    });
},
_dropdownClick : function(ele){
    var t =ele.offsetTop+19,
    // r =ele.offsetLeft -100,
    id =$(ele).attr('id');
    $(this.elem).find('#optionallist')
    .show()
    .attr('hid',id)
    .css({'position':'absolute','top':t+'px','right':'0','background-color':'#FFFFFF','cursor':'pointer'});  
     
},
_dropdownElementclick : function(ele){
    var obj =this;
    $(this.elem).find('#optionallist').find(ele).bind('click',function(){
       var oid = $(this).parent().attr('hid'),
           val = $(this).html(),
           vid = $(this).attr('id');
       oid = oid.replace('dropdown','option');
       $(obj.elem).find('#'+oid)
       .html(val)
       .attr('val',vid);
       $(obj.elem).find('#optionallist')
       .hide();
       var el = $(obj.elem).find('#chk input[id="'+oid.replace('option-','')+'"]');
       var opt = '{"optId":"'+vid+'","optval":"'+val+'"}';
       obj.options.ondropdown(obj.elem,JSON.parse(obj._getJson(el,opt)),obj.options.widget);
   });
},
_clearlist : function(){
	$(this.elem).find('#unchk ul li').remove();
	this.selected = $(this.elem).find('#chk ul').html();
	var obj = this;
	obj.selId =[];
	$.each($(this.elem).find('#chk ul li'), function(){
		obj.selId.push($(this).find('input').attr('id'));
	});
	this.selectLoaded = false;
	$(this.elem).find('#chk ul li').remove();
	$(this.elem).find('#unchk ul li').remove();
},
_defaultoptVal : function(opt){
	var a = new Array();
    var b;
   $.each(opt, function(i, e){
   var c = '';
   var d = 0;
       $.each(e, function(key,val){
       c += '"'+val+'"';
       c += (d == 0) ? ':':'';
       d = 1;
       });
       a.push(c);

   });
   b = "{"+a+"}";
   b = JSON.parse(b);
   return b;
},
_onclickHideShow : function(){
	var obj = this;
	$(document).click(function(event) { 
	    if(!$(event.target).closest('#ui-opt-span').length) {
	    	 if($('#optionallist').is(":visible")) {
	    		 $(obj.elem).find('#optionallist').hide();
	         }
	    }        
	});
},
_titleRestricted : function(title, strlenth) {
    try{
            if(this.options.titlelength!=0){
                    var localchar;
                    if(strlenth) {
                    	localchar = strlenth;
                    } else {
                    	localchar = this.options.titlelength;
                    }
			var restrictTitle;
			if(title){
			  if(title.length > localchar) {
				restrictTitle = title.substring(0,localchar);
				//restrictTitle = restrictTitle+'...';
				return restrictTitle+'...';
			  }else {
				//restrictTitle =  title;
				return title;
			  }
			}else {
			  //restrictTitle =  '';
				return '';
			}
			//return restrictTitle;
		}else{
			return title;
		}
		}catch(e){
			// to do
			 //console.log(e);
		}
	},
	
_autoHelpText: function(){
		o=this.options;
		oid = this.elemid;
		//clear all events
		$(this.elem).find('#multiautocomplete').each(function() {
	  		  try{
	  			$(this).unbind('focus');
	  			$(this).unbind('change');
	  			$(this).unbind('blur');
	  			$(this).attr('elemid',oid);
	  		  }catch(e){
	  			// console.log(e);
	  		  }
		});
		// Set events
		$(this.elem).find('#multiautocomplete').each(function() {
  		  try{
  		  // Stores the default value 
  		  Id = $(this).attr('id');
  		  $.data(this, 'default', this.value);
  		  $(this).attr('elemid',oid);
  		  if(this.value == o.helpText.autocomplete){
	    		  $(this).css('color','#999999');
	    		  $(this).css('font-size','10px');
	    		  $(this).css('line-height','21px');
  		  }else{
  			  $(this).css('color','#333333');
	    	  $(this).css('font-size','12px');
  		  }
  		  }catch(e){
  		        //Nothing to do
  			 //console.log(e);
  			}
  	  }).focus(function() {
  		  try{
  		  oid = $(this).attr('elemid');
  		  oob = $('#'+oid).data('expmultiselectDropdown');
  		  o=oob.options;
  		  // If the user has NOT edited the text clear it when they gain focus
  		  if(this.value != o.helpText.autocomplete){
  			  $.data(this, 'edited', this.value);
  		  }
  		  if (!$.data(this, 'edited')) {
  			  this.value = "";
  			  $(this).css('color','#333333');
	    	  $(this).css('font-size','12px');
  		  }
  		  
  		  }catch(e){
  		        //Nothing to do
  			// console.log(e);
  			}
  		  
  	  }).change(function() {
  		  try{
  		  // Fires on blur if the content has been changed by the user
  		  $.data(this, 'edited', this.value != "");
  		  }catch(e){
  		        //Nothing to do
  			// console.log(e);
  			}
  	  }).blur(function() {
  		  try{
  		  oid = $(this).attr('elemid');
  		  oob = $('#'+oid).data('expmultiselectDropdown');
  		  o=oob.options;
  		  // Put the default text back in the textarea if its not been edited
  		  if(this.value == "" && !$.data(this, 'edited')){
  			  this.value = o.helpText.autocomplete;
  			  $.data(this, 'default', this.value);
  			  $(this).css('color','#999999');
	    	  $(this).css('font-size','10px');
  		  }
  		  }catch(e){
  		        //Nothing to do
  			  //console.log(e);
  			}
  	  });
	},

resetAutoText: function(msg){
	if(msg!=null && msg!=undefined && msg!=''){
		this.options.helpText.autocomplete = msg;
	}
	$(this.elem).find('#multiautocomplete').val(this.options.helpText.autocomplete)
	this._autoHelpText();
},
	
refresh : function(){
	$(this.elem).find( "#ui-multiselect-con" ).remove();
	this.recs = 1;
	this.page = 1;
	this.reqcnt = 0;
	this._create();
	if(this.options.url != '')
		this._scrollevent();
	this._onclickHideShow();
	
},
showControl : function(){
	$(this.elem).find("#ui-multiselect-con").show();
},
hideControl : function(){
	$(this.elem).find("#ui-multiselect-con").hide();
},
getValue : function(op){
	var x,o=this;
	op = (op==null || op==undefined || op=='')?'selected':op;
	if(op=='selected')
		x = $(this.elem).find('#chk ul li');
	else
		x = $(this.elem).find('#unchk ul li');
	var seleId = '';
	var selOpt = '';
	var seleList = '[';
	$.each(x, function() {
		var i = $(this).find('input');
		var opt;
		if(document.getElementById('option-'+i.attr('id')) != null){
			var t = $(this).find('#option-'+i.attr('id'));
			opt = op=='selected'?'{"optId":"'+t.attr('val')+'","optval":"'+t.html()+'"}':'{"optId":"","optval":""}';
		}else{
			opt = '{"optId":"","optval":""}';
		}
		var l = o._getJson(i,opt);
		seleList += seleList=='['?l:","+l;
	});
	seleList+="]";
	return eval(seleList);
}
});
})(jQuery);

(function($) {
	$.fn.is_on_screen = function(){
	    var win = $(window);
	    var viewport = {
	        top : win.scrollTop(),
	        left : win.scrollLeft()
	    };
	    viewport.right = viewport.left + win.width();
	    viewport.bottom = viewport.top + win.height();//-100;
	 
	    var bounds = this.offset();
	    bounds.right = bounds.left + this.outerWidth();
	    bounds.bottom = bounds.top + this.outerHeight();
	 
	    return (!(viewport.right < bounds.left || viewport.left > bounds.right || viewport.bottom < bounds.top || viewport.top > bounds.bottom));
	};
})(jQuery);