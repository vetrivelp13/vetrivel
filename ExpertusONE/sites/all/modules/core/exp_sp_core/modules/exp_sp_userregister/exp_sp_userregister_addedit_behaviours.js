// Added by Kannan for 0052634 
(function($) {
	$.widget("ui.registeruser", {
		callMultiSelect: function(ele){
			var oc = $('#attr_open').val();
			var obj = this;
			this.currTheme = Drupal.settings.ajaxPageState.theme;
			$('.multiselect-custom-dropdown-results').css('display','none');
			$('.ui-multiselect-menu').css('display','none');
			$('#avil_'+ele).css('display','block');
			$('#attr_open').val(ele);
			$('#avil_'+oc).prev().find('span:last')
			.removeClass('down-tip-arrow')
			.addClass('right-tip-arrow');

			$('#avil_'+ele).prev().find('span:last')
			.removeClass('right-tip-arrow')
			.addClass('down-tip-arrow');
			
			//Construct data load and autocomplete urls
			
				var jobroleList = $('#load_multiselect_jobrole').val();
				jobroleList = jobroleList == '' ? undefined : jobroleList;
				var url = this.constructUrl("learning/register/jobrole/"+jobroleList);
				var autourl = this.constructUrl("learning/register/jobrole-autocomplete/"+ele);
				var list_height = 200;
				var titlelength = 28;
				var list_width = 250;
						
			var option;
			option = {
					url: url,
					dataType:"json",
					titlelength: titlelength,
					searchfilter:{url:autourl,enable:true},
					rownum:20,
					width:list_width,
					height: list_height,
					onselect:this.onselectfn,
					onunselect:this.onunselectfn,
					helpText: {autocomplete:Drupal.t('Search for refine'),checkAll:'Select All',uncheckAll:'Unselect All'},
					widget:this,
					afterload: function() {
						$('#avil_'+ele).find('.label-text').addClass('vtip');
						vtip();
						$('#avil_'+ele).find('.label-text').each(function() {
							$('#avil_'+ele).data('expmultiselectDropdown')._labelBind($(this));
						});
						$('#autoimg').bind('click',function(){
							$('#avil_'+ele).data('expmultiselectDropdown').options.url = obj.constructUrl("learning/register/jobrole/"+$('#load_multiselect_jobrole').val());
							
						});
					}
			};
			$('#avil_'+ele).expmultiselectDropdown(option);
			$('#avil_'+ele).css('display','block');
			//change by ayyappans for 41736: Issue in user creation page
			$('#avil_'+ele).find('.footeraccess').hide();
			$('#avil_'+ele).data('expmultiselectDropdown')._callScroll();
		},
		onselectfn: function(ele,data,obj) {
			var eid = $(ele).attr('id').replace('avil_','');
			var dt=new Array();
			var v = $('#load_multiselect_'+eid).val();
			if(v!=''){
				dt=v.split(',');
			}
			dt.push(data.value);
			$('#load_multiselect_'+eid).val(dt.join(','));
			$("#ajax-userregister").data('registeruser').updateSelectedText(eid, dt.length);
			$(ele).find('#chk .label-text').addClass('vtip');
			vtip();
			$(ele).find('.label-text').each(function() {
				$(ele).data('expmultiselectDropdown')._labelBind($(this));
			});
		},
		onunselectfn: function(ele,data,obj) {
			var eid = $(ele).attr('id').replace('avil_','');
			var dt=new Array();
			var v = $('#load_multiselect_'+eid).val();
			if(v!=''){
				dt=v.split(',');
			}
			var index = $.inArray(data.value, dt);
			dt.splice(index, 1);
			$('#load_multiselect_'+eid).val(dt.join(','));
			$("#ajax-userregister").data('registeruser').updateSelectedText(eid, dt.length);
		},
		updateSelectedText: function(eid, count) {
			var selectedText;
			//	for 41196: Can't able to save the page,system gets hang for a while
			if(count === undefined) {
				var dt=new Array();
				var v = $('#load_multiselect_'+eid).val();
				if(v!=''){
					dt=v.split(',');
				}
				count = dt.length;
			}
			if(count > 0) {
				selectedText = count + ' '+Drupal.t('LBL646');
				}
			else {
				selectedText = Drupal.t('LBL674');
				}
			$('#menu_'+eid).find('.selected-list-text').text(selectedText);
		}
	});
	$.extend($.ui.registeruser.prototype, EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget);
})(jQuery);

$(function() {
	try{
		$("#ajax-userregister").registeruser();
	}catch(e){
			// To Do
	}
});
$(function() {
	try {
		$('body').live('click', function(event) {
			if($(event.target).is('.multiselect-custom-dropdown') || ($(event.target).parents('div.multiselect-custom-dropdown-results').length && $('.multiselect-custom-dropdown-results').is(':visible'))) {
				//change for 42102: Unable to move to any other modules while clicking any modules link from the user profile page
				return false;
			}
			else if (!$(event.target).parents('.multiselect-custom-dropdown-results, .multiselect-custom-dropdown').length) {
				$('.multiselect-custom-dropdown-results').hide();
			}
		});
	}
	catch (e) {
		// TODO: handle exception
	}
});
function restrictMultiselectTitle(multiselectName, length) {
	$('div.ui-multiselect-menu.'+multiselectName).find('.ui-multiselect-checkboxes label.ui-corner-all span').each(function() {
		var labelText = titleRestricted($(this).text(), length);
		$(this).text(labelText);
	});
}

function onloadimage(){
	try{
		//For country and State
		var ob = $('#count').val();
		getstatesforcountry(ob);
	   	//For Job role
		$("#ajax-userregister").registeruser();
		$("#ajax-userregister").data('registeruser').updateSelectedText('jobrole');
        }
	catch (e){
		
	}
}

function getstatesforcountry(obj)
{
var url2=resource.base_host+"/?q=learning/register/countrycheck/"+obj;
var st = $('#stateid').val();
if (obj != ''){
	$.ajax({
			type: "GET",
		    url : url2,
		    data: { get_param: 'value' }, 
    		dataType: 'json',
    		 success: function (data) { 
    			 
    		    $('#state option').remove();
    		    $('#state').append($('<option>', { 
                    value: '',
                    text : Drupal.t('LBL674') 
                  }));
		        $.each(data, function(index, element) {
        
		        	   $('#state').append($('<option>', { 
                            value: element.sta_code,
                            text : element.sta_name 
                        }));
		        	   
		                if (st != "") {
		                	$('#state').val(st).attr("selected", "selected");
		                	}
		                });
		     	

		        
		    },
		    complete : function(xmlHttpRequest, textStatus) {
		   
			    if((parseInt(xmlHttpRequest.status) != 0) && (parseInt(xmlHttpRequest.status / 100) != 2)) {
			        return;
			    }
			   
			    var responseText = xmlHttpRequest.responseText;
			    
			   
		    }
		    
		  
	   });}
if (obj == '') { 
	$('#state option').remove();
	$('#state').html('<option value="">'+Drupal.t('LBL674')+'</option>');
}
}