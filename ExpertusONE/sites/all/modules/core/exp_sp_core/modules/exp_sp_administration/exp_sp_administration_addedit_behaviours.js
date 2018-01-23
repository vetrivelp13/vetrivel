( function($) {
	
  function initAddEditDateField(selector, defaultText) {
    try{
    // Show default text instead of ''
    var fieldValue = $(selector).val();
    if (fieldValue == '' || fieldValue == defaultText) {
      $(selector).removeClass('narrow-search-filterset-daterange-nonempty');
      $(selector).addClass('narrow-search-filterset-daterange-empty');
      $(selector).val(defaultText);
    }

    // Set the on blur, on focus and on change behaviours
    var data = selector + '&' + defaultText;
    var curObj=this;
    $(selector).blur(data, function(event) { // Can pass a string data only. but not an object data as objects are passed by reference
            var data = event.data;
            var tokens = data.split("&");
            var selector = tokens[0];
            var fieldValue = $(selector).val();
            var defaultText = tokens[1];

            if (fieldValue == '' || fieldValue == defaultText) {
              $(selector).removeClass('narrow-search-filterset-daterange-nonempty');
              $(selector).addClass('narrow-search-filterset-daterange-empty');
              $(selector).val(defaultText);
            } else if (fieldValue != defaultText) {
              $(selector).removeClass('narrow-search-filterset-daterange-empty');
              $(selector).addClass('narrow-search-filterset-daterange-nonempty');
            }
          });

    $(selector).focus(data,function(event) { // Can pass a string data only.but not an object data as objects are passed by  reference
            var data = event.data;
            var tokens = data.split("&");
            var selector = tokens[0];
            var fieldValue = $(selector).val();
            var defaultText = tokens[1];

            if (fieldValue == defaultText) {
              $(selector).val('');
            }
            $(selector).removeClass('narrow-search-filterset-daterange-empty');
            $(selector).addClass('narrow-search-filterset-daterange-nonempty');

          });

  $(selector).change(data,function(event) { // Can pass a string data only.but not an object data as objects are passed by reference
            var data = event.data;
            var tokens = data.split("&");
            var selector = tokens[0];
            var fieldValue = $(selector).val();
            var defaultText = tokens[1];

            if (fieldValue == '' || fieldValue == defaultText) {
              $(selector).removeClass('narrow-search-filterset-daterange-nonempty');
              $(selector).addClass('narrow-search-filterset-daterange-empty');
              $(selector).val(defaultText);
            } else {
              $(selector).removeClass('narrow-search-filterset-daterange-empty');
              $(selector).addClass('narrow-search-filterset-daterange-nonempty');
            }
          });
    }catch(e){
		// To Do
	}
}
  Drupal.behaviors.fadeeffect =  {

		    attach: function (context, settings) {
		    	if($('.admin-course-class-long-description .trunk8-fade').length>0){
		    	$('.trunk8-fade').trunk8(trunk8.admin_short_desc);
		    	}
		    }
  };
  Drupal.behaviors.addDatePickerToAddEditDateField =  {

	    attach: function (context, settings) {
	    	try{
		  if($("#multi_currency_setup_div").length > 0 && $("#edit-addedit-currency-save-button").length == 0)
	      {
			  dynamicWidthHeight(false);
	      }
	      $('.addedit-edit-datefield:not(.addedit-datepicker-added)').addClass('addedit-datepicker-added').each(function () {
	    	var pickerId = $(this).attr('id');
	        var datePickerTooltip = $('#'+pickerId).data('datePickerTooltip');
	        var datePickerShowTime = false;
	       // alert($('#'+pickerId).data('datePickerShowTime')+ 'ji' + pickerId + 'attr id ' + $(this).attr('id'))
	        if($('#'+pickerId).data('datePickerShowTime') == true){
	        	datePickerShowTime = true;
	        }
	        var calendarIcon = resource.base_url + '/' + themepath + '/expertusone-internals/images/calendar_icon.JPG';
	        try{
	        $('#'+pickerId).datepicker('destroy');
	        }catch(x){  }
	        try{
		        $('#'+pickerId).datetimepicker('destroy');
	        }catch(x){ }
	        try{
	        	$('#'+pickerId).timepicker('destroy');
	        }catch(x){ }

	        if(datePickerShowTime == false){
		        $('#'+pickerId).datepicker({
		          duration : '',
		          showTime : false,
		          constrainInput : false,
		          stepMinutes : 5,
		          stepHours : 1,
		          time24h : true,
		          dateFormat : 'mm-dd-yy',
		          buttonImage : calendarIcon,
		          buttonImageOnly : true,
		          firstDay : 0,
		          showOn : 'both',
		          buttonText : datePickerTooltip,
		          showButtonPanel : true,
		          changeMonth : true,
		          changeYear : true,
		          daterangeDiffDays : 20000,
		          beforeShow : function(input, inst) {
		            var dateFieldId = '#' + input.id;
		            //alert('dateFieldId = ' + dateFieldId);
		            //alert('input = ' + JSON.stringify(input));
		            //alert('inst = ' + JSON.stringify(inst));
		            var datesDisplayOption = $(dateFieldId).data('datePickerDatesDisplayOption');
		            if (datesDisplayOption == '' || datesDisplayOption == null || datesDisplayOption == 'undefined') {
		              datesDisplayOption = 'all';
		            }
		            setTimeout(function(){$('.ui-datepicker').css('z-index', 10000);}, 0);
		            var dateMin = null;
		            var dateMax = null;
	    		            var yearRange = '-10:+10';
	    		            if (datesDisplayOption != 'all' && datesDisplayOption != 'hiredate') {
		              var daterangeDiffDays = $(dateFieldId).datepicker('option', 'daterangeDiffDays');
		              var dateMin = new Date();
		              var dateMax = new Date();
		              switch (datesDisplayOption) {
		              case 'future': // Show future dates including today's date as the selectable date options in the datepicker. Disable all other dates.
		                dateMax.setDate(dateMin.getDate() + daterangeDiffDays);
		                break;
		              case 'past': // Show past dates including today's date as the selectable date options in the datepicker. Disable all other dates.
		                dateMin.setDate(dateMax.getDate() - daterangeDiffDays);
		                break;
		              } // end switch
	    		            }else if(datesDisplayOption == 'hiredate') {
	    		            	yearRange ='-70:+5';
		            } // end if (datesDisplayOption != "all")
		            if(pickerId=='end_date'){
                    	var start_date_value = $('#start_date').val();                            	
                    	return {
                    		defaultDate:start_date_value,
                    		minDate : start_date_value                            		
	    		            };
	    		    }else if(pickerId =='hire_end_ste_con_hdt_btw'){
	    		            	var start_date_value = $('#hidden_hire_start_ste_con_hdt_btw').val();                            	
                            	return {
                            		yearRange:'-70:+5',
                            		defaultDate: start_date_value,
                            		minDate : start_date_value                            		
      	    		 			};
      	    		 		
		            }else if(pickerId =='valid_to') {
		            var start_date_value = $('#valid_from').val();                            	
                            	return {
                            		yearRange:'-70:+5',
                            		defaultDate: start_date_value,
                            		minDate : start_date_value                            		
      	    		 			};
		            }else{
		            return {
	    		            	
	    		            		yearRange : yearRange,
		              minDate : dateMin,
		              maxDate : dateMax
		            };
		           }
		          } // end beforeShow
		        }); // end datepicker
	        }
	        else{
	          if($('#'+pickerId).data('datePickerShowTimeOnly') == true){
	        	 $('#'+pickerId).timePicker({
	        		  //startTime: "00.00", // Using string. Can take string or Date object.
	        		  //endTime: new Date(0, 0, 0, 15, 30, 0), // Using Date object here.
	        		  show24Hours: true,
	        		  separator: ':',
	        		  step: 5});
	          }
	          else{
	        	  $('#'+pickerId).datetimepicker({
		          duration : '',
		          showTime : true,
		          //timeOnly: false,
		          constrainInput : true,
		          stepMinutes : 5,
		          stepHours : 1,
		          time24h : true,
		         // dateFormat : 'mm-dd-yyyy',
		          buttonImage : calendarIcon,
		          buttonImageOnly : true,
		          firstDay : 1,
		          showOn : 'both',
		          buttonText : datePickerTooltip,
		          showButtonPanel : true,
		          changeMonth : true,
		          changeYear : true,
		          daterangeDiffDays : 20000,
		          beforeShow : function(input, inst) {
		            var dateFieldId = '#' + input.id;
		            var datesDisplayOption = $(dateFieldId).data('datePickerDatesDisplayOption');
		            if (datesDisplayOption == '' || datesDisplayOption == null || datesDisplayOption == 'undefined') {
		              datesDisplayOption = 'all';
		            }

		            var dateMin = null;
		            var dateMax = null;
		            if (datesDisplayOption != 'all') {
		              var daterangeDiffDays = $(dateFieldId).datepicker('option', 'daterangeDiffDays');
		              var dateMin = new Date();
		              var dateMax = new Date();
		              switch (datesDisplayOption) {
		              case 'future': // Show future dates including today's date as the selectable date options in the datepicker. Disable all other dates.
		                dateMax.setDate(dateMin.getDate() + daterangeDiffDays);
		                break;
		              case 'past': // Show past dates including today's date as the selectable date options in the datepicker. Disable all other dates.
		                dateMin.setDate(dateMax.getDate() - daterangeDiffDays);
		                break;
		              } // end switch
		            } // end if (datesDisplayOption != "all")
		            return {
		              minDate : dateMin,
		              maxDate : dateMax
		            };
		          } // end beforeShow
		        });
	        }
	      }

	        // Add display effects for empty value in field on blur, on focus and on change
	        var emptyDateFieldText = $('#'+pickerId).data('emptyDateFieldText');
	        if (emptyDateFieldText == '' || emptyDateFieldText == null || emptyDateFieldText == 'undefined') {
	          emptyDateFieldText = Drupal.t('LBL112');
	        }
	        initAddEditDateField('#' + pickerId, emptyDateFieldText);

	      });
	      vtip();
	    }
	    	catch(e){
				// To Do
			}
	 }
	  };

  Drupal.behaviors.resizeModalBackdropOnTextareaResize =  {
      attach: function (context, settings) {
    	  try{
        //console.log('In resizeModalBackdropOnTextareaResize');
        $('textarea.addedit-edit-textarea:not(.adjust-modalbackdrop-onresize)').addClass('adjust-modalbackdrop-onresize').each(function () {

          // Save the height and width as data on the textarea field itself
          var height = $(this).height();
          var width = $(this).width();
          $(this).data({'height': height, 'width': width});
          //console.log($(this).data());

          // Set the mouseup event handler
          $(this).mouseup(function(event) {
            if (typeof(Drupal.ajax.prototype.commands.CtoolsModalAdjust) !== 'undefined') {
              // Fetch the previous dimensions
              var prevHeight = $(this).data('height');
              var prevWidth = $(this).data('width');
              // Get the new dimensions
              var newHeight = $(this).height();
              var newWidth = $(this).width();
              //var dimensions = {'prevHeight': prevHeight, 'prevWidth' : prevWidth, 'newHeight': newHeight, 'newWidth': newWidth};
              //console.log(dimensions);
              if (newHeight != prevHeight || newWidth != prevWidth) {
                //console.log('resizeModalBackdropOnTextareaResize() calling CtoolsModalAdjust()');
                Drupal.ajax.prototype.commands.CtoolsModalAdjust(null, null, null);
              }
              // Save the new dimensions
              $(this).data({'height': newHeight, 'width': newWidth});

              // Below lines for debugging
              //var finalHeight = $(this).data('height');
              //var finalWidth = $(this).data('width');
              //dimensions = {'finalHeight': finalHeight, 'finalWidth' : finalWidth};
              //console.log(dimensions);
            } // end if
          }); // end mouseup
        }); // end each
    	  }catch(e){
  			// To Do
  		}
     } // end attach
  };

  Drupal.behaviors.resizeModalBackdropOnCKEditorResize =  {
      attach: function (context, settings) {
    	 try{
        //console.log('In resizeModalBackdropOnCKEditorResize');
        $('.cke_resizer:not(.adjust-modalbackdrop-onresize)').addClass('adjust-modalbackdrop-onresize').each(function () {
          // Set the mouseup event handler
          $(this).bind('mousedown.expckeresizer',function(event) {
            //console.log(event);
            //console.log('resizeModalBackdropOnCKEditorResize calling mousedown()');
            $(document).unbind('mouseup.expckeresizer'); // needed, as mouseup on iframe does not invoke mouseup on document
              $(document).one('mouseup.expckeresizer', function() {
                //console.log('resizeModalBackdropOnCKEditorResize calling mouseup() after mousedown()');
                if (typeof(Drupal.ajax.prototype.commands.CtoolsModalAdjust) !== 'undefined') {
                  //console.log('resizeModalBackdropOnCKEditorResize calling CtoolsModalAdjust()');
                     Drupal.ajax.prototype.commands.CtoolsModalAdjust(null, null, null);
                } // end if
              });
          }); // end mouseup
        }); // end each
    	 }catch(e){
   			// To Do
   		}
     } // end attach cke_resizer
  };

  // Command to refresh narrow search results displayed in jqGrid on form submit
  Drupal.ajax.prototype.commands.refreshnarrowsearchresults = function(ajax, response, status) {
	  try{
  	if(response.refreshMode == 'immediate') {
		if (response.refreshGrid || (typeof $(response.js_object_selector).data(response.js_object).refreshLastAccessedRow != 'undefined' && $(response.js_object_selector).data(response.js_object).refreshLastAccessedRow() == false)) {
			$(response.js_object_selector).data(response.js_object).refreshGrid();
		}
  	}
  	else {
  	  if (typeof Drupal.CTools.Modal.closeCommand.refreshNarrowSearchResults === 'undefined') {
  	    Drupal.CTools.Modal.closeCommand.refreshNarrowSearchResults =
  	      (function (jsObjectSelector, jsObject, refreshGrid) {
			//console.log('jsObjectSelector', jsObjectSelector, 'jsObject', jsObject, 'refreshGrid', refreshGrid);
			try {
  	        return {
                     cmd: function () {
                	          //console.log('jsObjectSelector = ', jsObjectSelector, ',\n jsObject = ', jsObject);
                	          //console.log('refresh grid func', $(jsObjectSelector).data('narrowsearch').refreshLastAccessedRow);
                	          if (refreshGrid || (typeof $(jsObjectSelector).data('narrowsearch').refreshLastAccessedRow != 'undefined' && $(jsObjectSelector).data('narrowsearch').refreshLastAccessedRow() == false)) {
                	          	$(jsObjectSelector).data(jsObject).refreshGrid();
                	          }
                	          // $(jsObjectSelector).data('narrowsearch').refreshLastAccessedRow();
                          }
                   };
			} catch(e) {
				//console.log(e, e.stack);
			}
  	        })(response.js_object_selector, response.js_object, response.refreshGrid);
  	   }
    }
	  }catch(e){
			// To Do
			//console.log(e, e.stack);
		}
  };

  // Command to refresh narrow search results displayed in jqGrid on form submit
  Drupal.ajax.prototype.commands.refresnarrowsearchonsubmit = function(ajax, response, status) {
	  try{
  	if(response.click_object) {
  		$(response.click_object).click();
  	}
  	if(response.refreshMode == 'immediate') {
  		$(response.js_object_selector).data(response.js_object).refreshGrid();
  	}
	  }catch(e){
			// To Do
		}
  };

  // Command to destroy CKEditors on ctools modal close (if present)
  Drupal.ajax.prototype.commands.destroyCKEditorsOnCtoolsModalClose = function(ajax, response, status) {
	  try{
    //console.log('In destroyCKEditorsOnCtoolsModalClose()');
    if (typeof Drupal.CTools.Modal.closeCommand.destroyCKEditors === 'undefined') {
      Drupal.CTools.Modal.closeCommand.destroyCKEditors = {
          cmd: function () {
             if ((typeof(CKEDITOR) != 'undefined') && typeof(CKEDITOR.instances) != 'undefined') {
               //console.log('Before');
               //console.log(CKEDITOR.instances);
               $.each(CKEDITOR.instances, function () {
                 //console.log('Destroying editor');
                 //console.log(this.name);
                 this.destroy(true);
               }); // end $.each
               //console.log('After');
               //console.log(CKEDITOR.instances);
             }; // end if
           } // end function
         };
    } // end if
	    //62832 - close calendar loader in admin calendar
	    if ($("#calendarprimaryview").size())
	    	EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.destroyLoader('calendar');
	    //Refresh tabs once the tp is created
	    if($( "#page-container-tabs-prg" ).size()){
	    //console.log('test');
	    	// Always the last tab should be in active while loading
	    	var pos = $( "#page-container-tabs-prg .visible-main-tab:last" ).index();
	    	if($( ".first-arrow" ).size() > 0 ){
	    		pos = pos - 1;
	    	}
	    	$( "#page-container-tabs-prg" ).tabs({
	    		selected: pos
	    	});
	    	//Diable attach courses
	    	var str = $('#program_attach_tabs .ui-state-active').attr('id');
			var currmod = str.split('-');
			moduleTabclick(currmod[2]);
	    	onloadsearch();
	    }
	  }catch(e){
			// To Do
		}
  };

  // Command to hide drupal messages after a period
  Drupal.ajax.prototype.commands.addEditHideMessages = function(ajax, response, status) {
	  try{
    //console.log('exp_sp_administration_addedit_behaviours.js : expHideDrupalMessages() : response');
    //console.log(response);
    var msgContext;
    //var errContext;
    if (typeof response.form_wrapper_id != 'undefined' && response.wrapper_id != '') {
      msgContext = $("#" + response.wrapper_id + " .messages");
      //errContext = $("#" + response.wrapper_id + " .error");
    }
    else {
      msgContext = $(".messages");
      //errContext = $(".error");
    }

    var height_errMsg = $(".messages").height();
    if ($(".admin-class-tp-enrollment").length > 0) {
    	$("#"+response.wrapper_id).css("height",Math.round(425+height_errMsg)+"px");
    }

    var delay = 10000;
    if (typeof response.delay != 'undefined') {
      if (typeof response.delay == "number" && response.delay > 0) {
        delay = response.delay;
      }
      else if (typeof response.delay == "string" && response.delay != '' && parseInt(response.delay) > 0) {
        delay = parseInt(response.delay);
      }
    }

    setTimeout(function() {
      msgContext.hide();
      if ($(".admin-class-tp-enrollment").length > 0) {
    	  $("#"+response.wrapper_id).css("height","403px");
      }
      //errContext.removeClass('error');
      Drupal.ajax.prototype.commands.CtoolsModalAdjust();
  }, delay);
	  }catch(e){
			// To Do
		}
  };

  // Command to scroll when we add new class in the admin screen
  Drupal.ajax.prototype.commands.addEditPopupDialogScroll = function(ajax, response, status) {
	  try{
    var dialogHeight;

    //The model dialog fixed height is 582px
    var normalHeight = 582;
    dialogHeight = $('.admin-popup-container').height();
    if(dialogHeight > normalHeight){
    	dialogHeight = dialogHeight-850;
    	$('#modal-content').scrollTop(dialogHeight);
    }
    else {
    	$('#modal-content').scrollTop(0);
    }
	  }catch(e){
			// To Do
		}
  };


  //#custom_attribute_0078975
  Drupal.behaviors.removeInputGreyfromCustomAttribute =  {
    attach: function (context, settings) {
    try{ 

		 
	  //Scroll issue in ClassAddEDITFORM #88022
		var count_class_custom_attr=$('.page-administration-learning-catalog #catalog-class-basic-addedit-form-scroll-container #custom_attr_container').length; 
    	if(count_class_custom_attr>0){  
    							//$('.catalog-course-basic-addedit-form-container').css('min-height','450px');
    							$('.catalog-course-basic-addedit-form-container').addClass('parent_custom_attr_scroll_class_container');
				    			var cls_page_height=$('#catalog-class-basic-addedit-form-scroll-container').height();
								$('#catalog-class-basic-addedit-form-scroll-container').css('height',cls_page_height);
								$('#catalog-class-basic-addedit-form-scroll-container').jScrollPane({  
									autoReinitialise: true,
									showArrows: false,
									verticalGutter:0,
									horizontalGutter:0
								});		 
	    } 

		
	    
    	//To place the scrollbar  when vertically displays options - added by ganesh for custom attribute
    	var count_vertical_opt=$('#custom_attr_container #customattribute-display-vertical').length; 
    	if(count_vertical_opt>0){
    		
    		 $('#custom_attr_container #customattribute-display-vertical').each(function() {   
			        var inp_count=$(this).find('input').length;		    	 
		    		if(inp_count>0 && inp_count>3){ //Yes there are options under verticatl div   
							$(this).addClass('cus_attr_jscroll_set');  
							$(this).jScrollPane({ 
								verticalGutter:0,
								horizontalGutter:0
							}); 
		    		}
			 }); 
    	}  
    	
    	//Remove the grey class from custom attribute container if not matched the help text - Added by ganesh for Custom Attribute
    	var cattr_page=$('.page-administration-manage-customattribute #create-dd-list li').length;
    	if(cattr_page>0){
    		$('.page-administration-manage-customattribute #create-dd-list li a span').click(function(){
    			var tmp_clicked_show_in_screen=$('.page-administration-manage-customattribute  .narrow-search-multi-action-container #bubble-face-table').length;
    			if(tmp_clicked_show_in_screen>0){
    				closeCustomAttributeQtip(0);
    			}
    		});
    	}
    	
    	 
    	 //Get Grey for HelpText    		
		var tmp_textarea=$('#custom_attr_container .addedit-edit-textarea').length;
		var tmp_textbox=$('#custom_attr_container .addedit-edit-textfield').length;
		
    	if(tmp_textarea>0 || tmp_textbox>0){
    		$('#custom_attr_container .addedit-edit-textarea').each(function() { 
    			if($(this).data('default-text')!=undefined){
    				 var defaultText = $(this).data('default-text');
    				 if(isNaN(defaultText)){ 
				      var defaultText1 = $(this).data('default-text').toLowerCase();;
				     }else{ 
				     	var defaultText1 = $(this).data('default-text');
				     }
				      
				     if($(this).val().toLowerCase() == defaultText1) { 
				        $(this).val(defaultText); 
				        $(this).addClass('input-field-grey');
				      }else { 
				        $(this).removeClass('input-field-grey');
				      }
				      
				        // Attach the event handlers
				      $(this).blur(defaultText, function(event) {
				        var defaultText = event.data;
				        if($(this).val() == '' || $(this).val().toLowerCase() == defaultText.toLowerCase()) {
				          $(this).val(defaultText);
				          $(this).addClass('input-field-grey');
				        }
				        else{
				          $(this).removeClass('input-field-grey');
				        }
				      });
			
				      $(this).focus(defaultText, function(event) {
				        var defaultText = event.data;	        
				        if($(this).val().toLowerCase() == defaultText.toLowerCase()) {
				          $(this).val('');
				          $(this).removeClass('input-field-grey');
				        }
				     });
    			}
    		}); 
    		
    		$('#custom_attr_container .addedit-edit-textfield').each(function() { 
    			if($(this).data('default-text')!=undefined){
    				 var defaultText = $(this).data('default-text');
    				 if(isNaN(defaultText)){ 
				      var defaultText1 = $(this).data('default-text').toLowerCase();;
				     }else{ 
				     	var defaultText1 = $(this).data('default-text');
				     }
				      
				     if($(this).val().toLowerCase() == defaultText1) { 
				        $(this).val(defaultText); 
				        $(this).addClass('input-field-grey');
				      }else { 
				        $(this).removeClass('input-field-grey');
				      }
				      
				        // Attach the event handlers
				      $(this).blur(defaultText, function(event) {
				        var defaultText = event.data;
				        if($(this).val() == '' || $(this).val().toLowerCase() == defaultText.toLowerCase()) {
				          $(this).val(defaultText);
				          $(this).addClass('input-field-grey');
				        }
				        else{
				          $(this).removeClass('input-field-grey');
				        }
				      });
			
				      $(this).focus(defaultText, function(event) {
				        var defaultText = event.data;	        
				        if($(this).val().toLowerCase() == defaultText.toLowerCase()) {
				          $(this).val('');
				          $(this).removeClass('input-field-grey');
				        }
				     });
    			}
    		});     		    		   	
    	}    	  
    }
    catch(e){
		// To Do
	}
    }
  };
 
  // Override multiselect behaviour and jQuery extensions so that
  // a. on Save, to select only from the current form
  // b. and to handle the 'none_selected' option.
  // Original code in multiselect.js of multiselect module.
  Drupal.behaviors.multiselect = {
    attach: function(context) {
    	try{
      //console.log('In exp_sp_administration multiselect attach()');
      // Remove the items that haven't been selected from the select box.
      $('select.multiselect_unsel:not(.multiselect-processed)', context).addClass('multiselect-processed').each(function() {
        unselclass = '.' + this.id + '_unsel';
        selclass = '.' + this.id + '_sel';
        $(unselclass).removeContentsFrom($(selclass));
      });

      // - This code will affect esignature form as well as script error in all the forms
      /*
      // Note: Doesn't matter what sort of submit button it is really (preview or submit)
      // Selects all the items in the selected box (so they are actually selected) when submitted
      // Expertus: If data-wrapperid is defined on the button, items are selected for multiselect existing inside the wrapperid div
      $('input.form-submit:not(.multiselect-processed)', context).addClass('multiselect-processed').click(function() {
        //console.log('In exp_sp_administration multiselect submit click()');
        var formWrapperId = $(this).data('wrapperid');
        //console.log('exp_sp_administration multiselect submit click() : formWrapperId = ' + formWrapperId);
        if (typeof(formWrapperId) !== 'undefined' && typeof(formWrapperId) === 'string' && formWrapperId != '') {
          $('#' + formWrapperId + ' select.multiselect_sel').selectAll();
        }
        else {
          $('select.multiselect_sel').selectAll();
        }
      });
      */

      // Moves selection if it's double clicked to selected box
      $('select.multiselect_unsel:not(.multiselect-unsel-processed)', context).addClass('multiselect-unsel-processed').dblclick(function() {
        unselclass = '.' + this.id + '_unsel';
        selclass = '.' + this.id + '_sel';
        $(unselclass).moveSelectionTo($(selclass));
      });

      // Moves selection if it's double clicked to unselected box
      $('select.multiselect_sel:not(.multiselect-sel-processed)', context).addClass('multiselect-sel-processed').dblclick(function() {
        unselclass = '.' + this.id + '_unsel';
        selclass = '.' + this.id + '_sel';
        $(selclass).moveSelectionTo($(unselclass));
      });

      // Moves selection if add is clicked to selected box
      $('li.multiselect_add:not(.multiselect-add-processed)', context).addClass('multiselect-add-processed').click(function() {
        unselclass = '.' + this.id + '_unsel';
        selclass = '.' + this.id + '_sel';
        $(unselclass).moveSelectionTo($(selclass));
      });

      // Moves selection if remove is clicked to selected box
      $('li.multiselect_remove:not(.multiselect-remove-processed)', context).addClass('multiselect-remove-processed').click(function() {
        unselclass = '.' + this.id + '_unsel';
        selclass = '.' + this.id + '_sel';
        $(selclass).moveSelectionTo($(unselclass));
      });
    	}catch(e){
  			// To Do
  		}
    }
  };

  //Removes the content of this select box from the target
  //usage $('nameofselectbox').removeContentsFrom(target_selectbox)
  jQuery.fn.removeContentsFrom = function() {
   //alert('removeContentsFrom');
	  try{
   var dest = arguments[0];
   this.each(function() {
     var lastOptionIdx = this.options.length - 1;
     for (var x = lastOptionIdx; x >= 0; x--) {
       dest.removeOption(this.options[x].value);
     }
   });
	  }catch(e){
			// To Do
		}
  }

  //Moves the selection to the select box specified
  //usage $('nameofselectbox').moveSelectionTo(destination_selectbox)
  jQuery.fn.moveSelectionTo = function() {
    //alert('moveSelectionTo');
 try{
   var dest = arguments[0];
   this.each(function() {
     var optionsLength = this.options.length;
     for (var x = 0; x < optionsLength; x++) {
       //alert('optionsLength before loop = ' + optionsLength);
       //alert('x before loop = ' + x);
       var option = this.options[x];
       //alert('option');
       if (option.selected && option.value != 'none_selected') {
         //alert('Calling addOption');
         dest.addOption(option);
         //alert('Returned from addOption');
         this.remove(x);
         //alert('Removed moved option');
         //$(this).triggerHandler('option-removed', option);
         x--; // The next option has taken deleted option's place. The number of options are reduced. We need to move back up.
         //alert('x end of loop = ' + x);
         optionsLength = this.options.length; // Reset length to the new length
         //alert('optionsLength end of loop = ' + optionsLength);
       }
     }
   });
 	}catch(e){
		// To Do
	}
  };

  //Adds an option to a select box
  //usage $('nameofselectbox').addOption(optiontoadd);
  jQuery.fn.addOption = function() {
	try{
    //alert('addOption');
   var option = arguments[0];
   //alert(option.value + ' => ' + option.text);
   this.each(function() {
     //alert('addOption(): Inside this.each: ' + option.value + ' => ' + option.text);
     //had to alter code to this to make it work in IE
     var anOption = document.createElement('option');
     anOption.text = option.text;
     anOption.value = option.value;
     var numOptions = this.options.length;
     if (numOptions <= 0) {
       this.options[numOptions] = anOption;
       //alert('option added at end as the options list is empty.');
     }
     else {
       var lastOption = this.options[numOptions - 1];
       //alert('lastOption.value = ' + lastOption.value);
       if (lastOption.value == 'none_selected') {
         this.options[numOptions - 1] = anOption;
         this.options[numOptions] = lastOption;
         //alert('option added before non_selected.');
       }
       else {
         this.options[numOptions] = anOption;
         //alert('option added at end.');
       }
     }
     //$(this).triggerHandler('option-added', anOption);
     return false;
   });
	}catch(e){
			// To Do
		}
  };

  //Removes an option from a select box
  //usage $('nameofselectbox').removeOption(valueOfOptionToRemove);
  jQuery.fn.removeOption = function() {
	  try{
    //alert('removeOption');
   var targOption = arguments[0];
   this.each(function() {
     var lastOptionIdx = this.options.length - 1;
     for (var x = lastOptionIdx; x >= 0; x--) {
       var option = this.options[x];
       if (option.value == targOption && option.value != 'none_selected') {
         //alert('removing option ' +  option.value + ' => ' + option.text);
         this.remove(x);
         //$(this).triggerHandler('option-removed', option);
       }
     }
   });
	  }catch(e){
			// To Do
		}
  };

})(jQuery);

// Editor Empty validation highlight color
Drupal.behaviors.highlightEditorOnValidationError =  {
	    attach: function (context, settings) {
	    	try{
	    	  $('span.cke_skin_kama:not(.exp-highlighted)').addClass('exp-highlighted').each(function () {
	           var geteditorid=$(this).attr("id");
	           var textAreaId=geteditorid.substring(4);
	           //alert(textAreaId);
	           if ($('#' + textAreaId).hasClass('error')) {
	        	 //  alert('sss');
	        	   $(this).css('border', '2px solid red');

	           }

	      });
	    	}catch(e){
	  			// To Do
	  		}
	    }
};

/* Behaviour of autofocus first field */
/*Drupal.behaviors.autoFoucusFirstsFields = {
			  attach: function(content, settings) {
				  try{
	        // Instead of use the class name we have conflict for Course and class. So we hanlde that in inc file. Other Pages using the form id.
	        $('#program-addedit-form:not(.auto-foucused)').addClass('auto-foucused').each(function () {
	        	$('form').find('select,textarea,input[type=text]').filter(':visible:first').focus();
			    });
	        $('#user-basic-addedit-form:not(.auto-foucused)').addClass('auto-foucused').each(function () {
	        	$('form').find('select,textarea,input[type=text]').filter(':visible:first').focus();
			    });
	        $('#organization-addedit-form:not(.auto-foucused)').addClass('auto-foucused').each(function () {
	        	$('form').find('select,textarea,input[type=text]').filter(':visible:first').focus();
			    });
	        $('#catalog-survey-assesment-basic-addedit-form:not(.auto-foucused)').addClass('auto-foucused').each(function () {
	        	$('form').find('select,textarea,input[type=text]').filter(':visible:first').focus();
			    });
	        $('#catalog-question-basic-addedit-form:not(.auto-foucused)').addClass('auto-foucused').each(function () {
	        	$('form').find('select,textarea,input[type=text]').filter(':visible:first').focus();
			    });
	        $('#content-addedit-form:not(.auto-foucused)').addClass('auto-foucused').each(function () {
	        	$('form').find('select,textarea,input[type=text]').filter(':visible:first').focus();
			    });
	        $('#location-basic-addedit-form:not(.auto-foucused)').addClass('auto-foucused').each(function () {
	        	$('form').find('select,textarea,input[type=text]').filter(':visible:first').focus();
			    });
	        $('#banner-basic-addedit-form:not(.auto-foucused)').addClass('auto-foucused').each(function () {
	        	$('form').find('select,textarea,input[type=text]').filter(':visible:first').focus();
			    });
	        //content-addedit-form location-basic-addedit-form
	        // For pre-requisite,equivalence,survey and assessment for data grid
	        $('.admin_add_multi_search_container:not(.auto-foucused)').addClass('auto-foucused').each(function () {
	        	$('form').find('select,textarea,input[type=text]').filter(':visible:first').focus();
			    });
				  }catch(e){
			  			// To Do
			  		}
	      } // end attach
};  */

//Initialize and set behaviour for custom label and value fields.
Drupal.behaviors.customField =  {
  attach: function (context, settings) {
	  try{
    var filter = '.addedit-edit-custom-label-field:not(.custom-field-initialized), .addedit-edit-custom-value-field:not(.custom-field-initialized)';
    $(filter).addClass('custom-field-initialized').each(function () {
      var defaultText = $(this).data('default-text');
      //console.log('default text = ' + defaultText);
      // Initialize the field
      if ($(this).val() == '' || $(this).val() == defaultText) {
        $(this).val(defaultText);
        $(this).addClass('input-field-grey');
      }
      else {
        $(this).removeClass('input-field-grey');
      }

      // Attach the event handlers
      $(this).blur(defaultText, function(event) {
        var defaultText = event.data;
        //console.log('blurred. default text = ' + defaultText);
        if($(this).val() == '' || $(this).val() == defaultText) {
          $(this).val(defaultText);
          $(this).addClass('input-field-grey');
        }
        else {
          $(this).removeClass('input-field-grey');
        }
      });

      $(this).focus(defaultText, function(event) {
        var defaultText = event.data;
        //console.log('gained focus. default text = ' + defaultText);
        if($(this).val() == defaultText) {
          $(this).val('');
          $(this).removeClass('input-field-grey');
        }
     });
   });
	  }catch(e){
			// To Do
		}
  }
};

//Initialize and set behaviour for custom label and value fields.
Drupal.behaviors.expAddAutocompleteToField =  {
  attach: function (context, settings) {
	  try{
    $('.exp-init-acfield:not(.exp-acfield-initialized)').addClass('exp-acfield-initialized').each(function () {
      var url = $(this).data('exp-acfield-url');
      //console.log('In expAddAutocompleteToField() : url = ' + url);
      var optionsStr = $(this).data('exp-acfield-options');
      //console.log('In expAddAutocompleteToField() : optionsStr = ' + optionsStr);
      var options = eval("(" + optionsStr + ")");
      //console.log(options);

      var preText = $(this).data('exp-acfield-pretext');
      //console.log('In expAddAutocompleteToField() : preText = ' + preText);
      if (preText != '') {
        if ($(this).val() == '' || $(this).val() == preText) {
          $(this).val(preText);
          $(this).addClass('input-field-grey');
        }
        else {
          $(this).removeClass('input-field-grey');
        }

        // Attach the event handlers
        $(this).blur(preText, function(event) {
          var preText = event.data;
          //console.log('blurred. preText = ' + preText);
          if($(this).val() == '' || $(this).val() == preText) {
            $(this).val(preText);
            $(this).addClass('input-field-grey');
          }
          else {
            $(this).removeClass('input-field-grey');
          }
        });

        $(this).focus(preText, function(event) {
          var preText = event.data;
          //console.log('gained focus. preText = ' + preText);
          if($(this).val() == preText) {
            $(this).val('');
            $(this).removeClass('input-field-grey');
          }
       });
      }

      $(this).autocomplete('?q=' + url, options);
   });
	  }catch(e){
			// To Do
		}
  }
};

//Initialize and set behaviour for time picker field
Drupal.behaviors.timePicker = {
  attach: function (context, settings) {
	  try{
    $('.exp-timepicker-for-addedit-form:not(.exp-timepicker-inited)').addClass('exp-timepicker-inited').each(function () {
      // Build the timepicker dropdown
      var timePickerFieldId = $(this).attr('id');
      var expDropDownId = 'exp-dropdown-' + timePickerFieldId;
	  var hours, minutes;
	  var timePickerListHolderHTML =
	    '<div id="' + expDropDownId + '" data="' + timePickerFieldId + '" class="exp-timepicker-selection" style="display:none;">' +
	      '<ul></ul>' +
	    '</div>';

	  $(this).after(timePickerListHolderHTML);

	  for (var i = 0; i <= 1425; i += 15) {
	    hours = Math.floor(i / 60);
	    minutes = i % 60;
		if (hours < 10) {
		  hours = '0' + hours;
		}
		if (minutes < 10) {
	      minutes = '0' + minutes;
		}
		var timePickerListItemHTML = '<li>' + hours + ':' + minutes + '</li>';
		$('#' + expDropDownId + ' ul').append(timePickerListItemHTML);
	  } //end for

	  // Add click handler for freq time textfield
      $(this).click(function() {
  	    var timePickerFieldId = $(this).attr('id');
  		var expDropDownId = 'exp-dropdown-' + timePickerFieldId;
  		$('#' + expDropDownId).toggle();
  	  });

      // Add click handler for date picker dropdown list items
	  $('#' + expDropDownId + ' ul li').click(function() {
		  // Get the selected option value
		  var selectedTime = $(this).html();

		  // Show the selected value in the freq time textfield
		  var timePickerFieldId = $(this).closest('.exp-timepicker-selection').attr('data');
		  $('#' + timePickerFieldId).val(selectedTime);

		  // Close the time picker drop-down
		  $(this).closest('.exp-timepicker-selection').css('display', 'none');
	  });
    });
	  }catch(e){
			// To Do
		}
  }
};
/**Change by: ayyappans
 * #36300: Issue in Tool tip incorrectly showing in Peoples settings page
 * Root Cause: exp_sp_administration_setup_addedit_form is loaded via ajax call.
 * So vtip() method doesn't apply required changes to the title attribute of the target elements
 * Fix: explicit call has been made to vtip() funtion once the ajax call gets complete
 */
//Initialize and set behaviour to initialize vtip on ajax call of exp_sp_administration_setup_addedit_form
Drupal.behaviors.vtipInitialize = {
  attach: function (context, settings) {
	  try{
		  $('.exp_sp_administration_setup_addedit_form').ajaxComplete(function(event, xhr, settings) {
			  vtip();
		  });
	  }catch(e){
			// To Do
		}
  }
};
//23872: Unable to make a class to be Shown in Catalog, when no content is associated
function showContentNeedMessage(){
	var error = new Array();
	 error[0] = Drupal.t('MSG498');
	 var message_call = expertus_error_message(error,'error');
	 $('#bubble-face-table #show_expertus_message').show();
	 $('#bubble-face-table #show_expertus_message').html(message_call);
	 return false;
}
//35264: Issue with Cancellation at Class Level
function showUserEnrolledMessage(){
	var error = new Array();
	 error[0] = Drupal.t('LBL1246');
	 var message_call = expertus_error_message(error,'error');
	 $('#bubble-face-table #show_expertus_message').show();
	 $('#bubble-face-table #show_expertus_message').html(message_call);
	 return false;
}
function checkIfContentAttemptsSet($uniqueId,type) {
	//atempts validation
	if(type != "recertify"){
	$('#hidden_idlist_content-'.$uniqueId).val();
//	var contentList = $('input[type=hidden][name=hidden_idlist_'+$uniqueId+']').val().split(',');
	var contentList = document.getElementById('hidden_idlist_'+$uniqueId) !== null ? document.getElementById('hidden_idlist_'+$uniqueId).value.split(',') : '';
	var attemptsNotSet = false;
	if(contentList != "" && contentList.length) {
		$.each(contentList, function(){
			var attempt = document.getElementById('content-maxattempt-'+this+'-hidden') !== null ? document.getElementById('content-maxattempt-'+this+'-hidden').value : '';
			if($.trim(attempt) == "") {
				attemptsNotSet = true;
				return false;
			}
	});
	}
		var msg = Drupal.t('MSG727')+' '+Drupal.t('MSG728');
		var drupalTitle = Drupal.t("LBL857");
	}else{
		var attemptsNotSet = true;
		var selected = $('input[name="selected_enroll_path_name"]').val();
		var msg = Drupal.t("MSG810")+' '+selected+' .'+Drupal.t("MSG728");
		var drupalTitle = Drupal.t("LBL429");
	}
	
	if(attemptsNotSet) {
		var uniqueClassPopup = 'unique-delete-class';
		var theme = Drupal.settings.ajaxPageState.theme;
		var wSize = (wSize) ? wSize : 300;
		var yesButAction = '';
		$('#content-attempt-message').remove();
		var html = '';
		html+='<div id="content-attempt-message" style="display:none; padding: 0px;">';
		html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';

		if(theme == 'expertusoneV2'){
			html+= '<tr><td style="padding: 30px 24px;" class="commanTitleAll">'+msg+'</td></tr>';
		} else {
			html+= '<tr><td color="#333333" style="padding: 30px 24px; text-align: left;">'+msg+'</td></tr>';
		}
		html+='</table>';
		html+='</div>';
		$("body").append(html);
		var closeButt={};
		closeButt[Drupal.t('No')]=function(){
			$(this).dialog('destroy');
			$(this).dialog('close');
			$('#content-attempt-message').remove();
		};
		yesButAction = function(){
			$(this).dialog('destroy');
			$(this).dialog('close');
			if(type == "recertify"){
				$('#submit-enrolltp-'+$uniqueId).click();
			}else{
			$('#submit-content-'+$uniqueId).click();
			}
			$('#content-attempt-message').remove();
		};
		closeButt[Drupal.t('Yes')]= yesButAction;

		$("#content-attempt-message").dialog({
			position:[(getWindowWidth()-400)/2,200],
			bgiframe: true,
			width:wSize,
			resizable:false,
			modal: true,
			title:Drupal.t(drupalTitle),
			buttons:closeButt,
			closeOnEscape: false,
			draggable:false,
			overlay:
			{
				opacity: 0.9,
				background: "black"
			}
		});
		$('.ui-dialog').wrap("<div id='delete-object-dialog' class='"+uniqueClassPopup+"'></div>");

		$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
		$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg"></div>');
		$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("admin-save-button-middle-bg").end();
		$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');

		$("#content-attempt-message").show();

		$('.ui-dialog-titlebar-close').click(function(){
			$("#content-attempt-message").remove();
		});
		if(theme == 'expertusoneV2'){
			changeDialogPopUI();
		}
		if($('div.qtip-defaults').length > 0) {
			var prevZindex = $('.qtip-defaults').css('z-index');
			$('#delete-object-dialog .ui-widget-content').css('z-index', prevZindex+2);
			$('.ui-widget-overlay').css('z-index', prevZindex+1);
		}else if($('div#modalBackdrop').length > 0){
			var prevZindex = $('#modalBackdrop').css('z-index');
			$('#delete-object-dialog .ui-widget-content').css('z-index', prevZindex+2);
			$('.ui-widget-overlay').css('z-index', prevZindex+1);
		}
	}
	else {
		$('#submit-content-'+$uniqueId).click();
	}
    return false;
}

function webex_config(uniqueid){
	$('.form-item-webex-radio').addClass(uniqueid);
	
	if(uniqueid == 'webex_User'){
		$('#webex_org_wrapper').css('display','none');
		} else if(uniqueid == 'webex_Organization'){
		$('#webex_org_wrapper').css('display','block');
	}	
}

	function webex_onload() {
	   var webex_radio_val = $("input[name='webex_radio']:checked").val();
       if(webex_radio_val == 'webex_Organization') {
			$('#webex_org_wrapper').css('display','block');	   
       } else if(webex_radio_val == 'webex_User') { 
    	   $('#webex_org_wrapper').css('display','none');	
       } else {
    	   $('#webex_org_wrapper').css('display','block');	
       }
	}
	
		
	function webex_presenter_validate(presenterid,user_lang) {
		
		var otherSessionWidth	= '';
		
		if (user_lang == 'ru') {
			otherSessionWidth = 333;
		} else if (user_lang == 'de'){
			otherSessionWidth = 310;
		} else if (user_lang == 'es'){
			otherSessionWidth = 335;
		} else if (user_lang == 'fr'){
			otherSessionWidth = 328;
		} else if (user_lang == 'it'){
			otherSessionWidth = 296;
		} else if (user_lang == 'pt-pt'){
			otherSessionWidth = 278;
		} else if (user_lang == 'ja'){
			otherSessionWidth = 300;
		} else if (user_lang == 'ko'){
			otherSessionWidth = 290;
		} else if (user_lang == 'zh-hans'){
			otherSessionWidth = 253;
		} else{
			otherSessionWidth = 279;
		}
		
		var popupentityId = 0;
		var popupAddSessionentityType = 'presenter';
		var popupAddSessionIdInit = popupentityId+'_'+popupAddSessionentityType;
		var popupAddpresentervisibPopupId  = 'qtip_visible_disp_addpresenter_'+popupAddSessionIdInit;
				
		var popupAddSessionVC = jQuery.parseJSON( '{"entityId":"","entityType":"'+popupAddSessionentityType+'","url":"ajax/class-presenter-check","popupDispId":"exp_meeting_'+popupAddpresentervisibPopupId+'","catalogVisibleId":"'+popupAddpresentervisibPopupId+'","wBubble":'+otherSessionWidth+',"hBubble":"auto","tipPosition":"tipfaceMiddleRight","qtipClass":"admin-qtip-presenter-access-parent"}' );
							
		$.ajax({
			 type: "POST",
		         url: '?q=ajax/presenter/webex-credentials/'+presenterid,
		         data:'',
		         success: function(result){
	        	
		        	if(result == 1) {
		        		$('#root-admin').data('narrowsearch').getQtipDiv(popupAddSessionVC);
		        		$('#admin-qtip-access-parent-lrn_cls_vct_web').css('bottom','32px');
		        		if (user_lang == 'fr'){
		       			$('.admin-qtip-presenter-access-parent').css('position','absolute').css('left','485px').css('bottom','-115px').css('top','-82px').css('z-index','100');
		        		$('#admin-qtip-presenter-access-parent').css('left','489px').css('top','-46px');
		        			$('label[for*="edit-presenter-"]').css('width','135px');
		        		} else if (user_lang == 'de'){
		        		$('.admin-qtip-presenter-access-parent').css('position','absolute').css('left',+parseInt(-208+675)+'px').css('bottom','-115px').css('top','-90px').css('z-index','100');
		        		$('#admin-qtip-presenter-access-parent').css('left','471px').css('top','-50px');
		        		$('label[for*="edit-presenter-"]').css('width','116px');
		        		} else if (user_lang == 'it'){
		        		$('.admin-qtip-presenter-access-parent').css('position','absolute').css('left',+parseInt(-208+675)+'px').css('bottom','-115px').css('top','-90px').css('z-index','100');
		        		$('#admin-qtip-presenter-access-parent').css('left','471px').css('top','-50px');
		        		$('label[for*="edit-presenter-"]').css('width','102px');
		        		} else if (user_lang == 'ja'){
		        		$('.admin-qtip-presenter-access-parent').css('position','absolute').css('left',+parseInt(-208+675)+'px').css('bottom','-115px').css('top','-90px').css('z-index','100');
		        		$('#admin-qtip-presenter-access-parent').css('left','471px').css('top','-56px');
		        		$('label[for*="edit-presenter-"]').css('width','87px');
		        		} else if (user_lang == 'ko'){
		        		$('.admin-qtip-presenter-access-parent').css('position','absolute').css('left',+parseInt(-208+675)+'px').css('bottom','-115px').css('top','-90px').css('z-index','100');
		        		$('#admin-qtip-presenter-access-parent').css('left','471px').css('top','-50px');
		        		$('label[for*="edit-presenter-"]').css('width','80px');
		        		} else if (user_lang == 'ru'){
		        		$('.admin-qtip-presenter-access-parent').css('position','absolute').css('left','473px').css('bottom','-115px').css('top','-90px').css('z-index','100');
		        		$('#admin-qtip-presenter-access-parent').css('left','477px').css('top','-56px');
		        		$('label[for*="edit-presenter-"]').css('width','139px');
		        		} else if (user_lang == 'zh-hans'){
		        		$('.admin-qtip-presenter-access-parent').css('position','absolute').css('left',+parseInt(-208+675)+'px').css('bottom','-115px').css('top','-90px').css('z-index','100');
		        		$('#admin-qtip-presenter-access-parent').css('left','471px').css('top','-56px');
		        		$('label[for*="edit-presenter-"]').css('width','58px');
		        		} else if (user_lang == 'es'){
		        		$('.admin-qtip-presenter-access-parent').css('position','absolute').css('left','499px').css('bottom','-115px').css('top','-90px').css('z-index','100');
		        		$('#admin-qtip-presenter-access-parent').css('left','503px').css('top','-56px');
		        		$('label[for*="edit-presenter-"]').css('width','141px');
		        		} else {
		        		$('.admin-qtip-presenter-access-parent').css('position','absolute').css('left',+parseInt(-208+675)+'px').css('bottom','-115px').css('top','-86px').css('z-index','100');
		        		$('#admin-qtip-presenter-access-parent').css('left','471px').css('top','-50px');
		        		$('label[for*="edit-presenter-"]').css('width','79px');
		        		}
		        	} 
		        	if(result == 0) {
		        		$('#exp_meeting_qtip_visible_disp_addpresenter_0_presenter').empty();
		        	}
		        }
		});
	}

function metatag_error_validate () {
	if($('#metatag-basic-addedit-form-container .addedit-edit-description').hasClass('error')) {
		$('.form-textarea-wrapper').addClass('error');
	}
}
