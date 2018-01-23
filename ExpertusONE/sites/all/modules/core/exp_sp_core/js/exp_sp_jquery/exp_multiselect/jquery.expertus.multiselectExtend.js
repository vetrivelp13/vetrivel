(function($) {
	$.widget("ui.expMultiSelectDropdownExtend", {
		obj:'',
		selected:'',
		selId:[],
		
		_init: function(){
			this._callMultiSelect();
		},
		
		_callMultiSelect: function(){
			$(this.element).expmultiselectDropdown(this.options);
			this.obj = $(this.element).data('expmultiselectDropdown');
			
			this._afterCreate();
			this._dataLoadExtend();
			this._dropdownElementclickExtend();
			this._moveselecteditemsExtend();
			this._moveunselecteditemsExtend();
			this._multiselectcheckAllExtend();
			this._multiselectuncheckAllExtend();
			this._selectItemExtend();
			this._clearListExtend();
			//this._custombinding();
		},

		_afterCreate: function(){
			var sel = $(this.element).find('#ui-multiselect-con').find('#chk ul li');
			$.each(sel,function(){
				$(this).find('input').wrap('<div class="checkbox-selected"/>');
			});
			var unsel = $(this.element).find('#ui-multiselect-con').find('#unchk ul li');
			$.each(unsel,function(){
				$(this).find('input').wrap('<div class="checkbox-unselected"/>');
			});
			if(this.options.checkAll == true){
				$(this.element).find('#selectuncheckall').wrap('<div class="checkbox-selected"/>');
			}
			if(this.options.uncheckAll == true){
				$(this.element).find('#selectcheckall').wrap('<div class="checkbox-unselected"/>');
			}
		},
		
		_moveselecteditemsExtend: function(){
			this.obj._moveselecteditems = function(ele){
					$(ele).parent().parent()
					.removeClass('visible')
					.css('display', 'none');	
				
				var id = $(ele).attr('id'),
					mobj = this,
				ol = mobj.options.optlistmodel,
				opl = mobj.options.optional.optionlist,
				defval = mobj._defaultoptVal(opl); 
				html = $(ele).parent().parent().html();
				if(mobj.options.optional.enable == true && $(ele).attr('opt') == 'enable'){
					//38262: Unable to set up MRO access in Course/Class and TP added by yogaraja
					var k = html.substr(html.indexOf('>')+1,html.toLowerCase().indexOf('</span'));
					k = k.substr(k.indexOf('">')+2,k.toLowerCase().indexOf('</span'));
					k = k.substr(k.indexOf('">')+2,k.toLowerCase().indexOf('</span'));
					var rpl = ($.browser.msie && $.browser.version == 8)?'</SPAN>':'</span>';
					y= k.replace(rpl,'');
					if(y.indexOf('...')>0 || y.length > mobj.options.titlelength-18){
			            if(y.indexOf('...')>0){
			            	var z = y.substr(0,y.indexOf("...")-18)+"...";
			            }else{
			            	var z = y.substr(0,mobj.options.titlelength-18)+"...";
			            }
						if(html.indexOf("fade-out-title-container") == -1) {
						html = html.replace(k,z);
					}
				}
					html += '</span><div class="option-div"><span class="grey-pointer"></span><span id = "ui-opt-span" class="ui-opt-span" ><span class="selected-opt1" id ="option-'+id+'" val ="'+opl[0][ol.id]+'">'+Drupal.t(defval[opl[0][ol.id]])+'</span><div id ="dropdown-'+id+'" class = "dropdown background"></div></span></div>';
					html = html.replace("fade-out-title-container ", "fade-out-title-container group-name-option-div ");
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
					if(lst>1){
						$(this.elem).find('input[id="selectuncheckall"]')
						.attr('checked','checked')
						.parent().parent()
						.show();
						// .css('display', 'block');
					}else if(lst == 1){
						$(this.elem).find('input[id="selectuncheckall"]')
						.attr('checked','checked')
						.parent().parent()
						.hide();
						// .css('display', 'none');
					}
				}
				
				// Hide "Select All" option if there are less than 2 list items are available
				if($('#'+this.elemid+' #unchk ul li').filter(function() {
			 	    return $(this).css('display') !== 'none';
			 	}).length <= 1) {
					$(this.elem).find('input[id="selectcheckall"]')
					.removeAttr('checked')
					.parent().parent()
					.css('display', 'none');
				}
				
				$(el).parent()
					.removeClass('checkbox-unselected')
					.addClass('checkbox-selected');
				
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
				this._labelBind($(el).parent().next());
				this._dropdownHideShow('dropdown-'+id);
				if(typeof(this.options.onselect) == 'function')
					this.options.onselect(this.elem,JSON.parse(this._getJson(el,'{"optId":"","optval":""}')),this.options.widget);	
				
			};
		},
		
		_moveunselecteditemsExtend: function(){
			this.obj._moveunselecteditems = function(ele){
				var rdata = JSON.parse(this._getJson(ele,'{"optId":"","optval":""}'));
				$(ele).parent().parent().remove();
				var id = $(ele).attr('id');
				var el = $(this.elem).find('#unchk input[id="'+id+'"]');
				$(el).removeAttr('checked');
				$(el).parent().parent()
					.addClass('visible')
					.show();
					// .css('display', 'block');
				

				// Enable the "Select All" option if there are 2 or more items present in the unselected list
				var lst = $('#'+this.elemid+' #unchk ul li').filter(function() {
					 	    return $(this).css('display') !== 'none';
					 	}).length;
				if(this.options.checkAll == true){
					if(lst > 1){
						$(this.elem)
						.find('input[id="selectcheckall"]')
							.removeAttr('checked')
							.parent().parent()
							.show();
							// .css('display', 'block');
					}else if(lst == 1){
						$(this.elem)
						.find('input[id="selectcheckall"]')
							.removeAttr('checked')
							.parent().parent()
							.css('display', 'none');
					}
				}
				
				// Disable the "Unselect All" option if there are less than 2 items in selected list
				if($('#'+this.elemid+' #chk ul li').filter(function() {
			 	    return $(this).css('display') !== 'none';
			 	}).length <= 1) {
					$(this.elem).find('input[id="selectuncheckall"]')
					.removeAttr('checked')
					.parent().parent()
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
				
				// Call respective native functions
				this._sortlist();
				this._callScroll();
				if(typeof(this.options.onunselect) == 'function')
					this.options.onunselect(this.elem,rdata,this.options.widget);
			};
		},
		
		_clearListExtend: function(){
			//var o = this;
			this.obj._clearlist = function(){
				$(this.elem).find('#unchk ul li').remove();
				this.selected = $(this.elem).find('#chk ul').html();
				var obj = this;
				obj.selId =[];
				$.each($(this.elem).find('#chk ul li'), function(){
					obj.selId.push($(this).find('input').attr('id'));
				});
				this.selectLoaded = false;
				$(this.elem).find('#chk ul li').remove();
			};
		},
		
		_dataLoadExtend: function(){
			var o=this;
			this.obj._dataLoad = function(data){
				var obj = this;
				this.data1 = data;
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
					    	if(obj.options.checkAll==true && obj.options.uncheckAll==true) {
					    		select = ($(obj.elem).find('#selectcheckall').is(':checked')== true) ? 1 : this[l.selected] ;
					    	}
					    	else{
					    		select = this[l.selected];
					    	}
					    	if(obj.options.optional.enable == true && this[sl.enable] == 'enable'){
					             trimTitle = titleRestrictionFadeoutImage(this[l.title],'learner-group-names');
					             learnerClassAdd="learner-group-container";
						    }else {
						    	 var titlelengthchars = obj.options.titlelength != '' ? obj.options.titlelength : 30;
						             trimTitle = titleRestrictionFadeoutImage(this[l.title],'group-names');
						             learnerClassAdd="";
						    }
							if(select == 1 && $.inArray(this[l.id], obj.selId) < 0 ){
								var dropdownClass = '';
								if(obj.options.optional.enable == true && this[sl.enable] == 'enable'){
									//to add dropdown class for group name in checked state
									dropdownClass = ' group-name-option-div';	
								}
							   chk += '<li>';
							  chk += '<div class="checkbox-selected"><input type="checkbox" checked = "" name="' + unescape(this[l.title]).replace(/"/g, '\&quot;') + '" id="' + this[l.id] + '"></div><span id ="li-listitem" for="' + unescape(this[l.title]).replace(/"/g, '\&quot;') + '" class="li-selected label-text" title="' + unescape(this[l.title]).replace(/"/g, '\&quot;') + '">' + titleRestrictionFadeoutImage(this[l.dispTitle],'check-groupnames'+dropdownClass) + '</span>';
							  unchk += '<li style ='+selstyle+'><div class="checkbox-unselected"><input type="checkbox" name="' + unescape(this[l.title]).replace(/"/g, '\&quot;') + '" id="' + this[l.id] + '" opt="'+this[sl.enable]+'"></div><span class="label-text" for="' + unescape(this[l.title]).replace(/"/g, '\&quot;') + '" title="' + unescape(this[l.title]).replace(/"/g, '\&quot;') + '">' +  titleRestrictionFadeoutImage(this[l.dispTitle],'uncheck-groupnames') + '</span></li>';
							   if(obj.options.optional.enable == true && this[sl.enable] == 'enable'){
								   dval = (this[sl.defaultval] != null) ? this[sl.defaultval] : opl[0][ol.id];
								   chk += '<div class="option-div"><span class="grey-pointer"></span><span id = "ui-opt-span" class="ui-opt-span" ><span class="selected-opt1" id ="option-'+ this[l.id] +'" val ="'+dval+'">'+Drupal.t(defval[dval])+'</span><span id ="dropdown-'+ this[l.id] +'" class = "dropdown background"></span></span></div>';
							   }
							   chk += '</li>'; 
							   // Call onselect call back handler when populate data in selected list.
							   if(typeof(obj.options.onselect) == 'function' && obj.selected.indexOf(this[l.id])<=0 && this[l.selected] == 0){
								    var vl = '{"value":"'+this[l.id]+'","title":"'+unescape(this[l.title]).replace(/"/g, '\&quot;')+'","selected":"true"}';
									obj.options.onselect(obj.elem,JSON.parse(vl),obj.options.widget);
							   }
							}else{
								var style = ($.inArray(this[l.id], obj.selId) !== -1)?selstyle:unselstyle;
							   unchk += '<li style ='+style+' class = "visible"><div class="checkbox-unselected"><input type="checkbox" name="' + unescape(this[l.title]).replace(/"/g, '\&quot;') + '" id="' + this[l.id] + '" opt="'+this[sl.enable]+'"></div><span class="label-text" for="' + unescape(this[l.title]).replace(/"/g, '\&quot;') + '" title="' + unescape(this[l.title]).replace(/"/g, '\&quot;') + '">' +  titleRestrictionFadeoutImage(this[l.dispTitle],'group-name') + '</span></li>';
							}
					    });
				    
				    if(obj.selected != '' && obj.selectLoaded==false){
				    	//var newData='';
				    	obj.selected = '<ul>'+obj.selected+'</ul>';
				    	var newData='';
				    	$(obj.selected).find("li").each(function(){
				    		
				    		  var n = $(this).find('input');
				    		  var flag = false;
				    		  for(var i in data){
				    		      if(data[i].val == $(n).attr('name'))
				    		         flag = true;

				    		  }
				    		  if(flag==true)
				    			  $(this).parent().find("li").css('display','inline-block');
				    		  else
				    			  $(this).parent().find("li").css('display','none');
				    		  
				    		  newData += this.outerHTML;
				    		});
				    	//alert(newData);
				    	obj.selected = newData;
				    	$(this.elem).find('#chk ul').append(obj.selected);
						$.each($(obj.selected),function(){
							$(obj.elem).find('input[id="'+$(this).find('input').attr('id')+'"]').attr('checked','checked');
							obj._clickfuntion($(obj.elem).find('input[id="'+$(this).find('input').attr('id')+'"]'));
							obj._labelBind($(obj.elem).find('input[id="'+$(this).find('input').attr('id')+'"]').parent().next());
						});
						obj.selectLoaded=true;
				    } 
				    
				    
					$(this.elem).find('#unchk ul').append(unchk);
					$(this.elem).find('#chk ul').append(chk);
				}else{
					 $(this.elem).find('.no-rec-msg').show();
					 $(this.elem).find('.no-rec-msg').css('padding-top','50px');
					 $(this.elem).find('#container').hide();
				 }
				if(obj.options.searchfilter.enable == true){
					var srval = $(this.elem).find('#multiautocomplete').val().toLowerCase();
					srval = srval.replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;');
					srval = srval == ((obj.options.helpText.autocomplete).toLowerCase())?'':srval;
					if(srval!=''){
						$.each($(this.elem).find('#chk ul li span.label-text'), function(){
							if($(this).find('.title-lengthy-text').html().toLowerCase().indexOf(srval)<0){
								$(this).parent().css('display','none');
							}else{
								$(this).parent().show();//.css('display','block');
							}
						});
						if($(obj.elem).find('#chk ul li input').length==0){
							
						}
					}else{
						$.each($(this.elem).find('#chk ul li span'), function(){
							var spobj = this;
							for(var i in data){
				    		      if(data[i].val == $(this).find('span').html())
				    		    	  $(spobj).parent().show();

				    		  }
						});
					}
				}
				
				if($(this.elem).find('#optionallist').html()==''){
					optlist +='<ul>';
					 $.each(opl, function() { 
				    	optlist +='<li id="'+this[ol.id]+'">'+Drupal.t(this[ol.title])+'</li>';
					   });
					 optlist +='</ul>';
			     	$(this.elem).find('#optionallist').append(optlist);
				 }
			     $(this.elem).find('#optionallist').hide();
			     
			     // Show/Hide "Select All" Option
			    $(this.elem).find('#selectcheckall').parent().parent()
			     	[ (this.options.checkAll == true && $('#'+this.elemid+' #unchk ul li').filter(function() {
				 	    return $(this).css('display') !== 'none';
				 	}).length > 1) ? 'show' : 'hide' ]();
			     
			    // Show/Hide "Unselect All" Option with checked attribute
			 	if(this.options.uncheckAll == true && $('#'+this.elemid+' #chk ul li').filter(function() {
			 	    return $(this).css('display') !== 'none';
			 	}).length > 1) {
			 		$(this.elem).find('#selectuncheckall').attr('checked','checked');
			 		$(this.elem).find('#selectuncheckall').parent().parent().show();
			 	}else{
			 		$(this.elem).find('#selectuncheckall').removeAttr('checked');
			 		$(this.elem).find('#selectuncheckall').parent().parent().hide();
			 	}
	
				 $.each(data, function() {
					 obj._clickfuntion($(obj.elem).find('input[id="'+this[l.id]+'"]'));
					 if(obj.options.optional.enable == true && this[sl.enable] == 'enable'){
					     obj._dropdownHideShow('dropdown-'+this[l.id]); // this is newly added for img click
			         }
					 obj._labelBind($(obj.elem).find('input[id="'+this[l.id]+'"]').parent().next());
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
			};
		},
		
		/*_custombinding: function(){
			var obj = this;
			var elm = this.obj.elem;
			$(elm).find('#selectcheckall').bind('click',function(){
				obj.options.onselectAll(elm,obj.options.widget);
			});
			$(elm).find('#selectuncheckall').bind('click',function(){
				obj.options.onunselectAll(elm,obj.options.widget);
			});
		},*/
		
		_multiselectcheckAllExtend: function(){
			this.obj._multiselectcheckAll = function(){
				var b = this;
				$(this.elem).find('#container').addClass('loader');
				setTimeout(function(){
					$(b.elem).find('input[id="selectcheckall"]')
						.attr('checked','checked')
						.parent().parent()
						.css('display', 'none');
					$(b.elem).find('input[id="selectuncheckall"]')
						.attr('checked','checked')
						.parent().parent()
						.show();
						// .css('display', 'block');
					
					var srval = $(b.elem).find('#multiautocomplete').val().toLowerCase();
					srval = srval == ((b.options.helpText.autocomplete).toLowerCase())?'':srval;
					if(srval == ''){
						// Remove visible class to all the items from unselcted list 
						$(b.elem).find('#unchk ul li').css('display','none').removeClass('visible');
						
						// Copy the entire unselected list into selected list
						$(b.elem).find('#chk ul').html($(b.elem).find('#unchk ul').html());
						
						// Add visible class to all the items in selected list
						$(b.elem).find('#chk ul li').css('display','block').addClass('visible');
						
						// Change the class for the div 
						$(b.elem).find('#chk ul li div')
							.removeClass('checkbox-unselected')
							.addClass('checkbox-selected');
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
								b._moveselecteditems($(this).children().children());
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
			};
		},

		_multiselectuncheckAllExtend: function(){
			this.obj._multiselectuncheckAll = function(){
				var b = this;
				$(this.elem).find('#container').addClass('loader');
				setTimeout(function(){
					// Hide Unslect All option
					$(b.elem).find('input[id="selectuncheckall"]')
						.removeAttr('checked')
						.parent().parent()
						.css('display', 'none');
					// Show Select All option
					$(b.elem).find('input[id="selectcheckall"]')
						.removeAttr('checked')
						.parent().parent()
						.show();
						// .css('display', 'block');
					var srval = $(b.elem).find('#multiautocomplete').val().toLowerCase();
					srval = srval == ((b.options.helpText.autocomplete).toLowerCase())?'':srval;
					if(srval ==''){
						// Remove all listed items from checked list
						$(b.elem).find('#chk ul li').remove();
						
						// Show the items in uncheck list
						$(b.elem).find('#unchk ul li').show();//.css('display','block');
						$(b.elem).find('#unchk ul li').each(function (){
							if($(this).css('display') != 'none'){
								$(this).children().children().removeAttr('checked');
							}
						});
					}else{
						$(b.elem).find('#chk ul li').each(function (){
							if($(this).css('display') != 'none'){
								$(this).children().children().removeAttr('checked');
								b._moveunselecteditems($(this).children().children());
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
			};
		},
		/**
		 * Extensible function for select item.
		 * This is for select check bos if the lable is clicked
		 */
		_selectItemExtend: function(){
			this.obj._selectItem = function(ele){
				var i = $(ele).prev().children();
				if($(i).is(":checked")==true){
					$(ele).prev().children().removeAttr('checked');
					$(ele).prev().children().click();
				}else{
					$(ele).prev().children().attr('checked','checked');
					$(ele).prev().children().click();
				}
				
			};
		},
		_dropdownElementclickExtend : function(){
			this.obj._dropdownElementclick = function(ele){
			    var obj =this;
			    $(this.elem).find('#optionallist').find(ele).bind('click',function(){
			       var oid = $(this).parent().parent().attr('hid'),
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
			};
		}
	
});
$.extend($.ui.expMultiSelectDropdownExtend.prototype, EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget);
})(jQuery);


(function($) {
	$.widget("ui.expMultiSelectExtend", {
		obj:'',
		selected:'',
		selId:[],
		
		_init: function(){
			this._callMultiSelect();
		},
		
		_callMultiSelect: function(){
			$(this.element).expmultiselect(this.options);
			this.obj = $(this.element).data('expmultiselect');
			
			// Call extensible functions to support new theme. 
			this._afterCreate();
			this._dataLoadExtend();
			this._moveselecteditemsExtend();
			this._moveunselecteditemsExtend();
			this._multiselectcheckAllExtend();
			this._multiselectuncheckAllExtend();
			this._selectItemExtend();
			this._clearListExtend();
			//this._custombinding();
		},
		
		/**
		 * Function to enclose div for new theme with respective class for each check box item
		 */
		
		_afterCreate: function(){
			var sel = $(this.element).find('#ui-multiselect-con').find('#chk ul li');
			$.each(sel,function(){
				$(this).find('input').wrap('<div class="checkbox-selected"/>');
			});
			var unsel = $(this.element).find('#ui-multiselect-con').find('#unchk ul li');
			$.each(unsel,function(){
				$(this).find('input').wrap('<div class="checkbox-unselected"/>');
			});
			if(this.options.checkAll == true){
				$(this.element).find('#selectuncheckall').wrap('<div class="checkbox-selected"/>');
			}
			if(this.options.uncheckAll == true){
				$(this.element).find('#selectcheckall').wrap('<div class="checkbox-unselected"/>');
			}
		},
		
		/**
		 * Extensible function for move selected items to checked list
		 */
		_moveselecteditemsExtend: function(){
			this.obj._moveselecteditems = function(ele){
				// Remove visible class and display off the selected input list
				$(ele).parent().parent()
					.removeClass('visible')
					.css('display', 'none');	
				
				// Prepare a new list item to append in checked list
				var id = $(ele).attr('id');
				html = $(ele).parent().parent().html();
				html = '<li>'+html+'</li>';
				
				$(this.elem).find("#chk ul").append(html);
				// Create objcet for the new appended item
				var el = $(this.elem).find('#chk input[id="'+id+'"]');
				$(this.elem).find('#chk input[id="'+id+'"]').attr('checked','checked');
				
				// Show "Unselect All" option if there are 2 or more list items are available
				var lst = $('#'+this.elemid+' #chk ul li').filter(function() {
			 	    			return $(this).css('display') !== 'none';
			 				}).length;
				if(this.options.uncheckAll == true){
					if(lst>1){
						$(this.elem).find('input[id="selectuncheckall"]')
						.attr('checked','checked')
						.parent().parent()
						.show();
						// .css('display', 'block');
					}else if(lst == 1){
						$(this.elem).find('input[id="selectuncheckall"]')
						.attr('checked','checked')
						.parent().parent()
						.css('display', 'none');
					}
				}

				
				// Hide "Select All" option if there are less than 2 list items are available
				if($('#'+this.elemid+' #unchk ul li').filter(function() {
			 	    return $(this).css('display') !== 'none';
			 	}).length <= 1) {
					$(this.elem).find('input[id="selectcheckall"]')
					.removeAttr('checked')
					.parent().parent()
					.css('display', 'none');
				}
				
				// Set appropriate class for input's parent div
				$(el).parent()
					.removeClass('checkbox-unselected')
					.addClass('checkbox-selected');
				
				// Show or hide the separator line 
				if($('#'+this.elemid+' #unchk ul li').filter(function() {
			 	    return $(this).css('display') !== 'none';
			 	}).length <= 0 || $('#'+this.elemid+' #chk ul li').filter(function() {
			 	    return $(this).css('display') !== 'none';
			 	}).length <= 0)
					 $(this.elem).find('.line-separator').hide();
			     else
			    	 $(this.elem).find('.line-separator').show();
				
				// Call respective native functions
				this._sortlist();
				this._callScroll();
				this._clickfuntion(el);
				this._labelBind($(el).parent().next());
				
				// Trigger callback function for select item
				if(typeof(this.options.onselect) == 'function')
					this.options.onselect(this.elem,JSON.parse(this._getJson(el)),this.options.widget);
			};
		},
		
		/**
		 * Extensible function for move list from selected to unselect
		 */
		_moveunselecteditemsExtend: function(){
			this.obj._moveunselecteditems = function(ele){
				var rdata = JSON.parse(this._getJson(ele));
				// Remove item from selected list
				$(ele).parent().parent().remove();
				
				// Enable the unselected item in unselected list
				var id = $(ele).attr('id');
				var el = $(this.elem).find('#unchk input[id="'+id+'"]');
				$(el).removeAttr('checked');
				$(el).parent().parent()
					.addClass('visible')
					.show();
					// .css('display', 'block');
				
				// Enable the "Select All" option if there are 2 or more items present in the unselected list
				var lst = $('#'+this.elemid+' #unchk ul li').filter(function() {
						 	    return $(this).css('display') !== 'none';
						 	}).length;
				if(this.options.checkAll == true){
					if(lst > 1){
						$(this.elem)
						.find('input[id="selectcheckall"]')
							.removeAttr('checked')
							.parent().parent()
							.show();
							// .css('display', 'block');
					}else if(lst == 1){
						$(this.elem).find('input[id="selectcheckall"]')
							.removeAttr('checked')
							.parent().parent()
							.css('display', 'none');
					}
				}
				
				// Disable the "Unselect All" option if there are less than 2 items in selected list
				if($('#'+this.elemid+' #chk ul li').filter(function() {
			 	    return $(this).css('display') !== 'none';
			 	}).length <= 1) {
					$(this.elem).find('input[id="selectuncheckall"]')
					.removeAttr('checked')
					.parent().parent()
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
				
				if (id == 'ste_con_hdt_btw' || id == 'ste_con_hdt_ltn' || id == 'ste_con_hdt_gtn') {	
					$('#hidden_hire_start_'+id).val('');
					if(id == 'ste_con_hdt_btw')
						$('#hire_end').val('');
				}
				
				// Call respective native functions
				this._sortlist();
				this._callScroll();				
				//this._labelBind($(el).parent().next());
				// Trigger callback function for unselect item 
				if(typeof(this.options.onunselect) == 'function')
					this.options.onunselect(this.elem,rdata,this.options.widget);
			};
		},
		
		/**
		 * Extensible function for clear list
		 */
		_clearListExtend: function(){
			this.obj._clearlist = function(){
				$(this.elem).find('#unchk ul li').remove();
				// Store selected items and clear the list
				this.selected = $(this.elem).find('#chk ul').html();
				var obj = this;
				obj.selId =[];
				$.each($(this.elem).find('#chk ul li'), function(){
					obj.selId.push($(this).find('input').attr('id'));
				});
				this.selectLoaded = false;
				$(this.elem).find('#chk ul li').remove();
			};
		},
		
		/**
		 * Extensible function for data load, this will not be call on first load
		 * but will be executed on subsequent calls when loading data by scroll down
		 */
		_dataLoadExtend: function(){
			this.obj._dataLoad = function(data){
				var obj = this;
				this.data1 = data;
				var	l = this.options.listmodel, 
					selstyle = "display:none",
					unselstyle = "display:block",
					chk= '',
					unchk = '',
					addnData = '';
				if(data.length > 0){
					$(this.elem).find('.create-group-hidecontainer').css('height','auto');
					$(this.elem).find('.no-rec-msg').hide();
					// Prepare checked and unchecked list
				    $.each(data, function(index, val){
				    	var select;
				    	if(obj.options.checkAll==true && obj.options.uncheckAll==true)
				    		select = ($(obj.elem).find('#selectcheckall').is(':checked')== true) ? 1 : this[l.selected] ;
				    	else{
				    		select = this[l.selected];
				    	}
						if(select == 1 && $.inArray(this[l.id], obj.selId) < 0){
							if (data[index].ui_data != undefined && data[index].ui_data != '' ) {
								addnData = '<span class="ui_data_wrap">' + data[index].ui_data + '</span>';
							}
						   chk += '<li>';
						   chk += '<div class="checkbox-selected"><input type="checkbox" checked = "" name="' + unescape(this[l.title]).replace(/"/g, '\&quot;') + '" id="' + this[l.id] + '"></div><span id ="li-listitem" for="' + unescape(this[l.title]).replace(/"/g, '\&quot;') + '" class="li-selected label-text" title="' + unescape(this[l.title]).replace(/"/g, '\&quot;') + '">' + titleRestrictionFadeoutImage(this[l.title],'dropdown-multiselect-group') + '</span>';
						   unchk += '<li style ='+selstyle+'><div class="checkbox-unselected"><input type="checkbox" name="' + unescape(this[l.title]).replace(/"/g, '\&quot;') + '" id="' + this[l.id] + '" ></div><span class="label-text" for="' + unescape(this[l.title]).replace(/"/g, '\&quot;') + '" title="' + unescape(this[l.title]).replace(/"/g, '\&quot;') + '">' +  titleRestrictionFadeoutImage(this[l.title],'dropdown-multiselect-group') + '</span>' + addnData + '</li>';
						   chk += addnData; 
						   chk += '</li>'; 
						   // Call onselect call back handler when populate data in selected list.
						   if(typeof(obj.options.onselect) == 'function' && obj.selected.indexOf(this[l.id])<=0 && this[l.selected] == 0){
							    var vl = '{"value":"'+this[l.id]+'","title":"'+unescape(this[l.title]).replace(/"/g, '\&quot;')+'","selected":"true"}';
								obj.options.onselect(obj.elem,JSON.parse(vl),obj.options.widget);
						   }
						}else {
							var style = ($.inArray(this[l.id], obj.selId) == -1)?unselstyle:selstyle;
							if (data[index].ui_data != undefined && data[index].ui_data != '' ) {
								addnData = '<span class="ui_data_wrap">' + data[index].ui_data + '</span>';
							}
							unchk += '<li style ='+style+' class = "visible" ><div class="checkbox-unselected"><input  type="checkbox" name="' + unescape(this[l.title]).replace(/"/g, '\&quot;') + '" id="' + this[l.id] + '" ></div><span class="label-text" for="' + unescape(this[l.title]).replace(/"/g, '\&quot;') + '" title="' + unescape(this[l.title]).replace(/"/g, '\&quot;') + '">' +  titleRestrictionFadeoutImage(this[l.title],'dropdown-multiselect-group') + '</span>' + addnData + '</li>';
						}
				    });
	
				    // While loading data if there is any previously selected items available load that items
				    if(obj.selected != '' && obj.selectLoaded==false){
				    $(this.elem).find('#chk ul').append(obj.selected);
						$.each($(obj.selected),function(){
							$(obj.elem).find('input[id="'+$(this).find('input').attr('id')+'"]').attr('checked','checked');
							obj._clickfuntion($(obj.elem).find('input[id="'+$(this).find('input').attr('id')+'"]'));
							obj._labelBind($(obj.elem).find('input[id="'+$(this).find('input').attr('id')+'"]').parent().next());
						})
				    	//o.selected = '';
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
								$.each($(this).find('div span:first'), function(){
								if($(this).html().toLowerCase().indexOf(srval)<0){
										$(this).parent().parent().parent().hide();
								}else{
										$(this).parent().parent().parent().show();//.css('display','block');
								}
							})
							})
							if($(obj.elem).find('#chk ul li input').length==0){
								
							}
						}else{
							$.each($(this.elem).find('#chk ul li span'), function(){
								$(this).parent().show();//.css('display','block');
							})
						}
					}
					
					// Show/Hide "Select All" Option
				    $(this.elem).find('#selectcheckall').parent().parent()
				     	[ (this.options.checkAll == true && $('#'+this.elemid+' #unchk ul li').filter(function() {
					 	    return $(this).css('display') !== 'none';
					 	}).length > 1) ? 'show' : 'hide' ]();
				     
				    // Show/Hide "Unselect All" Option with checked attribute
				 	if(this.options.uncheckAll == true && $('#'+this.elemid+' #chk ul li').filter(function() {
				 	    return $(this).css('display') !== 'none';
				 	}).length > 1) {
				 		$(this.elem).find('#selectuncheckall').attr('checked','checked');
				 		$(this.elem).find('#selectuncheckall').parent().parent().show();
				 	}else{
				 		$(this.elem).find('#selectuncheckall').removeAttr('checked');
				 		$(this.elem).find('#selectuncheckall').parent().parent().hide();
				 	}
		
					// Call click buindings for a newly added items
					$.each(data, function() {
						 obj._clickfuntion($(obj.elem).find('input[id="'+this[l.id]+'"]'));
						 obj._labelBind($(obj.elem).find('input[id="'+this[l.id]+'"]').parent().next());
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
			};
			
		},
		
		/**
		 * Custom buinding function for select all and unselect all options
		 * This will call the respective callbacks 
		 */
		/*_custombinding: function(){
			var obj = this;
			var elm = this.obj.elem;
			$(elm).find('#selectcheckall').bind('click',function(){
				obj.options.onselectAll(elm,obj.options.widget);
			});
			$(elm).find('#selectuncheckall').bind('click',function(){
				obj.options.onunselectAll(elm,obj.options.widget);
			});
		},*/
		
		/**
		 * Extensible function for Select All
		 */
		_multiselectcheckAllExtend: function(){
			this.obj._multiselectcheckAll = function(){
				var b = this;
				$(this.elem).find('#container').addClass('loader');
				// Set time out for show loader otherwise the loader will not shown
				setTimeout(function(){
					var obj = this;
					// Hide Select All option
					$(b.elem).find('input[id="selectcheckall"]')
						.attr('checked','checked')
						.parent().parent()
						.css('display', 'none');
					// Show Unselect All option
					$(b.elem).find('input[id="selectuncheckall"]')
						.attr('checked','checked')
						.parent().parent()
						.show();
						// .css('display', 'block');
					
					
					var srval = $(b.elem).find('#multiautocomplete').val().toLowerCase();
					srval = srval == ((b.options.helpText.autocomplete).toLowerCase())?'':srval;
					if(srval == ''){
						// Remove visible class to all the items from unselcted list 
						$(b.elem).find('#unchk ul li').css('display','none').removeClass('visible');
						
						// Copy the entire unselected list into selected list
						$(b.elem).find('#chk ul').html($(b.elem).find('#unchk ul').html());
						
						// Add visible class to all the items in selected list
						$(b.elem).find('#chk ul li').show().addClass('visible');//.css('display','block')
						
						// Change the class for the div 
						/*$(b.elem).find('#chk ul li div')
							.removeClass('checkbox-unselected')
							.addClass('checkbox-selected');*/
						// Change the class for the div 
						$.each($(b.elem).find('#chk ul li'), function(){
							$(this).find('div:first').removeClass('checkbox-unselected')
							.addClass('checkbox-selected');
						})
						// Mark the input as checked
						$(b.elem).find('#chk ul li input').attr('checked','checked');
						
						// Call a bunding for each input items
						$(b.elem).find('#chk ul li input').each(function(){
							$(this).bind('click',function(){
								b._moveunselecteditems(this);
							});	
						})
						$(b.elem).find('#chk ul li .label-text').each(function(){
					 		b._labelBind(this);
					 	});
					}else{
						$(b.elem).find('#unchk ul li').each(function (){
							if($(this).css('display') != 'none'){
								//$(this).children().children().attr('checked','checked');
								b._moveselecteditems($(this).children().children());
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
				
			};
		},

		/**
		 * Extensible function for Unselect All
		 */
		_multiselectuncheckAllExtend: function(){
			this.obj._multiselectuncheckAll = function(){
				var b = this;
				$(this.elem).find('#container').addClass('loader');
				setTimeout(function(){
					// Hide Unslect All option
					$(b.elem).find('input[id="selectuncheckall"]')
						.removeAttr('checked')
						.parent().parent()
						.css('display', 'none');
					// Show Select All option
					$(b.elem).find('input[id="selectcheckall"]')
						.removeAttr('checked')
						.parent().parent()
						.show();
						// .css('display', 'block');
					var srval = $(b.elem).find('#multiautocomplete').val().toLowerCase();
					srval = srval == ((b.options.helpText.autocomplete).toLowerCase())?'':srval;
					if(srval ==''){
						// Remove all listed items from checked list
						$(b.elem).find('#chk ul li').remove();
						
						// Show the items in uncheck list
						$(b.elem).find('#unchk ul li').show();//.css('display','block');
						$(b.elem).find('#unchk ul li').each(function (){
							if($(this).css('display') != 'none'){
								$(this).children().children().removeAttr('checked');
							}
						});
					}else{
						$(b.elem).find('#chk ul li').each(function (){
							if($(this).css('display') != 'none'){
								$(this).children().children().removeAttr('checked');
								b._moveunselecteditems($(this).children().children());
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
			};
		},
		
		/**
		 * Extensible function for select item.
		 * This is for select check bos if the lable is clicked
		 */
		_selectItemExtend: function(){
			this.obj._selectItem = function(ele){
				var i = $(ele).prev().children()
				if($(i).is(":checked")==true){
					$(ele).prev().children().removeAttr('checked');
					$(ele).prev().children().click();
				}else{
					$(ele).prev().children().attr('checked','checked');
					$(ele).prev().children().click();
				}
				
			};
		}
});
$.extend($.ui.expMultiSelectExtend.prototype, EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget);
})(jQuery);
