(function($) {
	$.widget("exp.expmultiselect", {
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
		titlelength:20,
		rownum:5,
		url:'',
		onselect:'',
		onunselect:'',
		onload:'',
		selectedText: "#",
		selectedList: 0,
		show: '',
		hide: '',
		sort:'asc',
		autoOpen: false,
		multiple: true,
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
	selectLoaded:true,

_init: function(){
	if(this.options.url != '')
		this._scrollevent();
},

_create: function(){
	  var o = this.options,
	  	obj = this,
		html ='',
		auto = '',
		chkAll = '',
		chk= '<ul></ul>',
		unchk = '<ul></ul>',
		unchkAll = '',
	    cHight=o.height - 30;
 	 this.elem = this.element;
 	 this.elemid = $(this.elem).attr('id');
 	 if(o.searchfilter.enable == true){
 		 auto = '<div id ="autocomplete" class = "autocomplete" ><input type="text" name="autocomplete" id="multiautocomplete" value="'+o.helpText.autocomplete+'"><span><div id ="autoimg"></div></span></div>';
 		cHight = o.height - 65;
 	 }
 	 if(this.options.checkAll == true)
		 chkAll +='<div><ul><li style="display:none;"><input type="checkbox" name="checkall" id="selectcheckall"><span for="checkall" class="label-text">'+o.helpText.checkAll+'</span></li></ul></div>';

	 if(this.options.uncheckAll == true)
		 unchkAll +='<div><ul><li style="display:none;"><input type="checkbox" name="checkall" id="selectuncheckall"><span for="uncheckall" class="label-text">'+o.helpText.uncheckAll+'</span></li></ul></div>';

 	html +='<div id ="ui-multiselect-con" class = "ui-multiselect-con-class" >'+auto+'<div class="create-group-hidecontainer"><div class = "no-rec-msg" style="display:none;">'+Drupal.t('MSG403')+'</div><div id = "container" class ="mcontainerclass">'+unchkAll+'<div id ="chk">'+chk+'</div><div class="line-separator" style="display:none;"/>'+chkAll+'<div id ="unchk">'+unchk+'</div><div id ="overflowitems" class ="overflowitems"></div></div></div></div>';
    $(this.elem).append(html);
    $(this.elem).find('#ui-multiselect-con')
    	.addClass(o.classes)
		.css({'width':o.width,'height':o.height});
    $(this.elem).find('#container')
		.css({'height':cHight});
    if(o.searchfilter.enable == true){
	    $(this.elem).find('#multiautocomplete')
		.css({'width':o.width-30,'padding-left':'3px'});
	    this._autoHelpText();
    }
    $(this.elem).find('.label-text').each(function(){
 		obj._labelBind(this);
 	});
    if(o.url != '')
    	this._constructajaxcall(true);
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
	var	l = this.options.listmodel,
		selstyle = "display:none",
		unselstyle = "display:block",
		chk= '',
		unchk = '';
	if(data.length > 0){
		$(this.elem).find('.create-group-hidecontainer').css('height','auto');
		$(this.elem).find('.no-rec-msg').hide();
		// Prepare checked and unchecked list
	    $.each(data, function(){
	    	var select;
	    	if(obj.options.checkAll==true && obj.options.uncheckAll==true)
	    		select = ($(obj.elem).find('#selectcheckall').is(':checked')== true) ? 1 : this[l.selected] ;
	    	else{
	    		select = this[l.selected];
	    	}
			if(select == 1 && $.inArray(this[l.id], obj.selId) < 0){
			   chk += '<li>';
			   chk += '<input type="checkbox" checked = "" name="' + unescape(this[l.title]).replace(/"/g, '\&quot;') + '" id="' + this[l.id] + '"><span id ="li-listitem" for="' + unescape(this[l.title]).replace(/"/g, '\&quot;') + '" class="li-selected  label-text" title="' + unescape(this[l.title]).replace(/"/g, '\&quot;') + '">' + obj._titleRestricted(this[l.title],25) + '</span>';
			   unchk += '<li style ='+selstyle+'><input type="checkbox" name="' + unescape(this[l.title]).replace(/"/g, '\&quot;') + '" id="' + this[l.id] + '" ><span class="label-text" for="' + unescape(this[l.title]).replace(/"/g, '\&quot;') + '" title="' + unescape(this[l.title]).replace(/"/g, '\&quot;') + '">' +  obj._titleRestricted(this[l.title],25) + '</span></li>';
			   chk += '</li>';
			   // Call onselect call back handler when populate data in selected list.
			   if(typeof(obj.options.onselect) == 'function' && obj.selected.indexOf(this[l.id])<=0 && this[l.selected] == 0 ){
				    var vl = '{"value":"'+this[l.id]+'","title":"'+unescape(this[l.title]).replace(/"/g, '\&quot;')+'","selected":"true"}';
					obj.options.onselect(obj.elem,JSON.parse(vl),obj.options.widget);
			   }
			}else {
				var style = (obj.selected.indexOf(this[l.id])<=0)?unselstyle:selstyle;
				unchk += '<li style ='+style+' class = "visible" ><input  type="checkbox" name="' + unescape(this[l.title]).replace(/"/g, '\&quot;') + '" id="' + this[l.id] + '" ><span class="label-text" for="' + unescape(this[l.title]).replace(/"/g, '\&quot;') + '" title="' + unescape(this[l.title]).replace(/"/g, '\&quot;') + '">' +  obj._titleRestricted(this[l.title],25) + '</span></li>';
			}
	    });

	    // While loading data if there is any previously selected items available load that items
	    if(obj.selected != '' && obj.selectLoaded==false){
	    	$(this.elem).find('#chk ul').append(obj.selected);
			$.each($(obj.selected),function(){
				$(obj.elem).find('input[id="'+$(this).find('input').attr('id')+'"]').attr('checked','checked');
				obj._clickfuntion($(obj.elem).find('input[id="'+$(this).find('input').attr('id')+'"]'));
				obj._labelBind($(obj.elem).find('input[id="'+$(this).find('input').attr('id')+'"]').next());
			});
	    	//obj.selected = '';
			obj.selectLoaded=true;
	    }

		 $(this.elem).find('#unchk ul').append(unchk);
		 $(this.elem).find('#chk ul').append(chk);
		 }else{
			 $(this.elem).find('.create-group-hidecontainer').css('height','40px');
			 $(this.elem).find('.no-rec-msg').show();
		 }
		// While search bu autocomplete, display only the matching items in checked list
		if(obj.options.searchfilter.enable == true){
			var srval = $(this.elem).find('#multiautocomplete').val().toLowerCase();
			srval = srval == ((obj.options.helpText.autocomplete).toLowerCase())?'':srval;
			if(srval!=''){
				$.each($(this.elem).find('#chk ul li span'), function(){
					if($(this).html().toLowerCase().indexOf(srval)<0){
						$(this).parent().css('display','none');
					}else{
						$(this).parent().css('display','block');
					}
				});
				if($(obj.elem).find('#chk ul li input').length==0){

				}
			}else{
				$.each($(this.elem).find('#chk ul li span'), function(){
					$(this).parent().css('display','block');
				});
			}
		}
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

			// Call click buindings for a newly added items
			$.each(data, function() {
				 obj._clickfuntion($(obj.elem).find('input[id="'+this[l.id]+'"]'));
				 obj._labelBind($(obj.elem).find('input[id="'+this[l.id]+'"]').next());
			 });

			// Show/Hide separator
			if($(this.elem).find('#unchk ul li').filter(function() {
		 	    return $(this).css('display') !== 'none';
		 	}).length <= 0 || $(this.elem).find('#chk ul li').filter(function() {
		 	    return $(this).css('display') !== 'none';
		 	}).length <= 0)
				 $(this.elem).find('.line-separator').hide();
		    else
		    	 $(this.elem).find('.line-separator').show();

			 if(typeof(obj.options.afterload) == 'function'){
					obj.options.afterload(obj);
			   }

},

_binding : function() {
	var theme = Drupal.settings.ajaxPageState.theme;
	var obj = this,
	 elm = obj.elem;
	if(this.options.checkAll == true){
		$(this.elem).find('#selectcheckall').unbind('click')
			.click(function () {
				obj._multiselectcheckAll();
			});
	}
	if(this.options.uncheckAll == true){
		$(this.elem).find('#selectuncheckall').unbind('click')
			.click(function () {
				obj._multiselectuncheckAll();
			});
	}

	if(this.options.searchfilter.enable == true){
		$(this.elem).find('#autoimg').bind('click',function(){
			obj._searchautocomplete();
		});
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

	//Custom binding function for select all and unselect all options

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

	var id = $(ele).attr('id');
	html = $(ele).parent().html();
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
	this._labelBind(el.next());
	if(typeof(this.options.onselect) == 'function')
		this.options.onselect(this.elem,JSON.parse(this._getJson(el)),this.options.widget);
},

_getJson: function(el){
//	change by ayyappans for 40441: Unable to deselect group names in access popup
	return '{"value":"'+$(el).attr('id')+'","title":"'+unescape($(el).attr('name')).replace(/"/g, '\&quot;').replace(/\\/g, '&#92;')+'","selected":"'+$(el).is(':checked')+'"}';
},

_moveunselecteditems : function(ele){
	var rdata = JSON.parse(this._getJson(ele));
	$(ele).parent().remove();
	var id = $(ele).attr('id');
	var el = $(this.elem).find('#unchk input[id="'+id+'"]');
	$(el).removeAttr('checked');
	$(el).parent()
		.addClass('visible')
		.css('display', 'block');

	// Enable the "Select All" option if there are 2 or more items present in the unselected list
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
	$(this.elem).find('#chk ul li .li-selected')
		.css('width',o.width/2-10);
	$(this.elem).find('#chk ul li .option-div')
		.css('width',o.width/2-22);
	/**	0037564: Code Re-Factoring - Browser Compatibility - Old Theme - Admin side - Chrome - People
		2. People > Group > Create Group - Administer > When select and unselect, the items exceed the content border.
		Root Cause: jscrollPane gets re initialized only if the container elements are more than eight.
		Fix: jscrollPane is re initialized only if the container elements are more than five.
	*/
	if(this.data1.length >= 6 || this.recs > this.options.rownum){
		if($(this.elem).find('.jspContainer').length==0){
			$(this.elem).find('#container').jScrollPane({});
			$(this.elem).find('#container').css({"overflow":"visible"});
		}else{
			$(this.elem).find('#container').jScrollPane('destroy');
			$(this.elem).find('#container').jScrollPane('reinitialise');
			$(this.elem).find('#container').css({"overflow":"visible"});
		}
	}else if($(this.elem).find('.jspContainer').length!=0){
		$(this.elem).find('#container').jScrollPane('destroy');
	}
},
_bindBehaviors : function(ele) {
	var obj = this;
	var id = $(ele).attr('id');
	if($("#role-addedit-form").length) { // attach date-picker
		if (id == 'ste_con_hdt_ltn' || id == 'ste_con_hdt_gtn' || id == 'ste_con_hdt_btw' ) {
			var stDate = $('#hidden_hire_start_'+id ).val();
			var enDate = $('#hire_end').val();
			$('#hire_start_'+id ).removeClass('addedit-datepicker-added hasDatepicker narrow-search-filterset-daterange-empty').val(stDate);
			if (id == 'ste_con_hdt_btw') 
				$('#hire_end_'+id ).removeClass('addedit-datepicker-added hasDatepicker narrow-search-filterset-daterange-empty').val(enDate);
			Drupal.attachBehaviors();
			//Reset unchecked data (if any)
			$('#unchk ul li .addedit-edit-datefield').each(function(){
				$(this).addClass('addedit-datepicker-added hasDatepicker narrow-search-filterset-daterange-empty').val('').blur();
				$(this).addClass('addedit-datepicker-added hasDatepicker narrow-search-filterset-daterange-empty').val('').blur();
				Drupal.attachBehaviors();
			});
		}
		/*if ($("#hire_start_"+id).val() == '') {	
			$('#hidden_hire_start_'+id).val('');
			$('#hire_end').val('');
		}*/
		if (obj.elemid == 'avil_ste_con_hdt') {
			Drupal.attachBehaviors();
		}
		$('#hire_start_'+id ).parent().find('img').remove();
	}
},
_clickfuntion : function(ele){
	var obj = this;
	$(ele).bind('click',function(){
		if($(this).is(':checked') == true) {
			obj._moveselecteditems(this);
			obj._bindBehaviors(this);
		} else {
			obj._moveunselecteditems(this);
			obj._bindBehaviors(this);
		}	
	});
	
},

_constructajaxcall : function(isFirst){
	var res = '';
	var obj = this;
	var url = this.options.url;
	var ht = this.options.helpText.autocomplete;
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
		setTimeout(function(){
			jQuery.ajax({
				   type: "POST",
				   url: url,
				   processData: false,
				   contentType:'text/xml',
				   async: false,
				   success: function(respText){
					  res = respText;
					  obj.timer=0;
			  		}
				 });
			obj.reqcnt++;
			obj.recs = res.records;
			obj.page++;
			if(obj.reqcnt == 1)
				obj._clearlist();
			obj._dataLoad(res.data);
			$(obj.elem).find('#container').removeClass('loader');
			/*if(isFirst==true && obj.recs<10){
				$(obj.elem).find('#autocomplete').css('display','none');
				$(obj.elem).find('#ui-multiselect-con').css('height',obj.recs*26+40);
			}*/
			/*added for 79610*/ 
			if(obj.elemid != 'avil_ste_con_hdt') {
				obj._callScroll();
			}
			obj._bindBehaviors();
		},50);
	}
},

_multiselectcheckAll :function(){
	var b = this;
	$(this.elem).find('#container').addClass('loader');
	// Set time out for show loader otherwise the loader will not shown
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
			/**	37564: Code Re-Factoring - Browser Compatibility - Old Theme - Admin side - Chrome - People
			 *	1. People > Group > Create Group - Administer > Unselect all doesn't unselect previous selected items.
			 *	Root Cause: Unselected items checked attribute were retained
			 *	Fix: checked attribute was removed from all the unselected items
			 */
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
		this._constructajaxcall(false);
	}
},

_clearlist : function(){

	$(this.elem).find('#unchk ul li').remove();
	this.selected = $(this.elem).find('#chk ul').html();
	var obj = this;
	obj.selId =[];
	$.each($(this.elem).find('#chk ul li'), function(){
		obj.selId.push($(this).find('input').attr('id'));
	});
	this.selectLoaded=false;
	$(this.elem).find('#chk ul li').remove();

	$(this.elem).find('#unchk ul li').remove();
	$(this.elem).find('#chk ul li').remove();
},

_titleRestricted : function(title,titlechar) {
	try{
		if(this.options.titlelength!=0){
			if(titlechar){
				var chara = titlechar;
			}else{
				var chara = this.options.titlelength;
			}
			var restrictTitle;
			if(title){
			  if(title.length >= chara) {
				restrictTitle = title.substring(0,chara);
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
		}
	},

_autoHelpText: function(){
		o=this.options;
		ob = this;
		oid = this.elemid;
		//clear all events
		$(this.elem).find('#multiautocomplete').each(function() {
	  		  try{
	  			$(this).unbind('focus');
	  			$(this).unbind('change');
	  			$(this).unbind('blur');
	  			$(this).attr('elemid',oid);
	  		  }catch(e){

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
  			  //$('#'+Id).css('font-size','12px');
  			  $(this).css('font-size','12px');
  		  }
  		  }catch(e){
  		        //Nothing to do
  			}
  	  }).focus(function() {
  		  try{
  		  oid = $(this).attr('elemid');
  		  oob = $('#'+oid).data('expmultiselect');
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
  			}

  	  }).change(function() {
  		  try{
  		  // Fires on blur if the content has been changed by the user
  		  $.data(this, 'edited', this.value != "");
  		  }catch(e){
  		        //Nothing to do
  			}
  	  }).blur(function() {
  		  try{
  		  oid = $(this).attr('elemid');
  		  oob = $('#'+oid).data('expmultiselect');
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
},

showControl : function(){
	$(this.elem).find("#ui-multiselect-con").show();
},

hideControl : function(){
	$(this.elem).find("#ui-multiselect-con").hide();
},

getValue: function(op){
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
		l = o._getJson($(this).find('input'));
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