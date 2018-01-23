//$Id: uc_discounts.js,v 1.1 2011-04-11 10:21:26 muthusamys Exp $

var uc_discountsisUpdating = false;
//Handles onload calls for uc_discounts.
function uc_discountsOnLoad(e) {
  context = $('body');
  $discountFrom = $("input[name=checkout_from_admin]:hidden").val();
  if($("textarea[id*=uc-discounts-codes]", context).val() && $discountFrom != 'admin_order'){
	  uc_discountsProcessCodes(context, e);
  }
  
//Add click event listener to discounts pane button once
  Drupal.behaviors.orderAdminApplyDiscountButton =  {
      attach: function (context, settings) {
        $('input[id*=uc-discounts-button]:not(.uc_discountsOnLoad-processed)').addClass('uc_discountsOnLoad-processed').each(function () {
          $(this).click(function(e) {
            uc_discountsProcessCodes($('body'), e);
            //Return false to prevent default actions and propogation
            return false;
          });
        }); // end each
     } // end attach cke_resizer
  };

  //Add click event listener to discounts pane button once
  $("input[id*=uc-discounts-button]:not(.uc_discountsOnLoad-processed)", 
    context).addClass("uc_discountsOnLoad-processed").click(function(e) {
      uc_discountsProcessCodes(context, e);
      //Return false to prevent default actions and propogation
      return false;
    });
}

//Processes currently entered discounts
function uc_discountsProcessCodes(context, e) {
	//Check the input box empty. if empty give error msg add yogaraja
	textareaVal = $.trim($("textarea[id*=uc-discounts-codes]", context).val());
	if(textareaVal == null || textareaVal == "")
	{
		$('#message-container').html('');
		var error = new Array();
		error[0] = Drupal.t("LBL752")+' '+Drupal.t("MSG697")+'.';
		var message_call = expertus_error_message(error,'error');
		$('#show_expertus_message').show();
		$('#show_expertus_message').html(message_call);
		return false;
	}

  //If currently updating, wait
  if (uc_discountsisUpdating) {
    setTimeout(function() { uc_discountsProcessCodes(context); }, 200);
    return;
  }

  var parameterMap = {};
  parameterMap["uc-discounts-codes"] = $("textarea[id*=uc-discounts-codes]", context).val();
  var queryString = "?q=cart/checkout/uc_discounts/calculate";
  if($("input[name=checkout_from_admin]:hidden").val() == 'admin_order'){
	  queryString = "?q=administration/create/uc_discounts/calculate";
	  parameterMap["uc-discounts-codes"] = (parameterMap["uc-discounts-codes"] == Drupal.t("MSG592")) ? '' : parameterMap["uc-discounts-codes"];
  }

  //Create and show expertusone loader
  abstractDetailWidget = EXPERTUS_SMARTPORTAL_AbstractDetailsWidget;
  abstractDetailWidget.createLoader('clone_cart-pane');
  $.ajax({
    type: "POST",
    url: Drupal.settings.basePath + queryString,
    data: parameterMap,
    complete : function(xmlHttpRequest, textStatus) {
      // Destroy expertusone loader
      abstractDetailWidget.destroyLoader('clone_cart-pane');

      //If status is not 2XX
      if ( parseInt(xmlHttpRequest.status) != 0 && parseInt(xmlHttpRequest.status / 100) != 2) {
        alert(Drupal.settings.uc_discounts.err_msg);
        return;
      }

      var responseText = xmlHttpRequest.responseText;
      var calculateDiscountResponse = null;
      try {
        responseText = xmlHttpRequest.responseText;
        calculateDiscountResponse = $.parseJSON(responseText);
      }
      catch (err) {
        alert(Drupal.settings.uc_discounts.response_parse_err_msg + responseText);
        return;
      }

      try {
        uc_discountsProcessCalculateDiscountResponse(calculateDiscountResponse, context);
        // Re-process shipping quotes since discount codes can affect shipping via conditional actions
        // Only re-process if they clicked the button so as to not override their shipping preference
        if (e.type == 'click') {
          $("input[id*=quote-button]").click();
        }
      }
      catch (err) {
    	  	//alert(Drupal.settings.uc_discounts.err_msg);
    	  	var error = new Array();
			error[0] = Drupal.settings.uc_discounts.err_msg;
			var message_call = expertus_error_message(error,'error');
			$('#show_expertus_message').show();
			$('#show_expertus_message').html(message_call);
			window.scrollTo(0, 0);
			return;
      }
    }
  });
}

//Processes calculateDiscountResponse from drupal
function uc_discountsProcessCalculateDiscountResponse(calculateDiscountResponse, context) {
  if (uc_discountsisUpdating) {
    return;
  }
  uc_discountsisUpdating = true;

  try {
    var i;

    if (calculateDiscountResponse == null) {
      alert(Drupal.settings.uc_discounts.err_msg);
      return;
    }

    var line_items = null;
    var errors = null;
    var messages = null;
    var total_discount = null;
    var total = null;

    try {
      line_items = calculateDiscountResponse[Drupal.settings.uc_discounts.calculate_discount_response_line_items_key];
      errors = calculateDiscountResponse[Drupal.settings.uc_discounts.calculate_discount_response_errors_key];
      messages = calculateDiscountResponse[Drupal.settings.uc_discounts.calculate_discount_response_messages_key];
      total_discount = calculateDiscountResponse.total_discount_theme;
      total = calculateDiscountResponse.total_theme;
    }    
    catch (err) { }
    var disCode = $("textarea[id*=uc-discounts-codes]", context).val();
    disCode = $.trim(disCode);
    if(disCode.length == 0){
      errors.push(t('MSG697'));
    }
    //Add errors and messages to messages container
    if ( (errors != null) && (errors.length > 0) ) {
    	//42861: When discount code contains single quote, if its invalid discount error incorrectly showing
    	$('#show_expertus_message').empty();
		var message_call = expertus_error_message(errors,'error');
		var msgVal = $('#message-container').html();
		if(msgVal != null && msgVal !='' && msgVal != 'null'){
			$('#message-container .error').append(message_call);
		}else{
			$('#show_expertus_message').html(message_call);
		}
		$('#show_expertus_message').show();

		window.scrollTo(0, 0); // when there is an error message displayed, scroll to the top
    }

    if ( (messages != null) && (messages.length > 0) ) {
      var message_list = $("<ul><" + "/ul>");
      for (var i = 0; i < messages.length; i++) {
        message_list.append( $("<li><" + "/li>").append(messages[i]) );
      }
      discounts_messages_container.append( $("<div class='uc-discounts-messages messages'> <" + "/div>").append(message_list) );      
    }
    if((total != null) && (total_discount != null)) {
      $('#exp-clone-cart-discount').html(total_discount);
      $('#exp-clone-cart-total').html(total);
    }

    uc_discountsisUpdating = false;
  }
  catch (err) {
    uc_discountsisUpdating = false;
    throw err;
  }
}

//Updates the discount line items list and updates totals
function uc_discountsRenderLineItems(line_items, show_message) {
  if ((window.set_line_item == null) || (line_items == null)) {
    return;
  }

  //Remove total discount line item
  remove_line_item(Drupal.settings.uc_discounts.line_item_key_name);

  var total_amount = 0;
  for (i = 0; i < line_items.length; i++) {
    var line_item = line_items[i];
    total_amount += parseFloat(line_item["amount"]);
  }

  //Add total discount line item
  if (line_items.length > 0) {
    set_line_item(Drupal.settings.uc_discounts.line_item_key_name,
                  Drupal.settings.uc_discounts.total_discount_text, total_amount,
                  parseFloat(Drupal.settings.uc_discounts.line_item_weight) + 0.5,
                  1,
                  false);
  }

  // Update total
  if (window.render_line_items) {
    render_line_items();
  }

  //If there is tax in the system, recalculate tax
  if (window.getTax) {
    getTax();
  }
}