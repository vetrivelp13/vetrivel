// first I create an empty object as the namespace for my script. All functions will be added to this namespace to prevent conflicts with other modules.
var expSpCybersourceTax = {}; 
 
// Next, I create the onload function for the script. Drupal.behaviors is the Drupal version of an onload function, and I add myModule in order to create an onload namespace and prevent onload conflicts with other modules. All code executed inside this function will be executed onload.
Drupal.behaviors.expSpCybersourceTax = function()  
{
	try{
	expSpCybersourceTax.checkboxListener(); // I will create this function below. 
	}catch(e){
		// to do}
	}
};
 
expSpCybersourceTax.checkboxListener = function() // This is the function called by my onload definition, and this function is part of the namespace defined at the top of the script
{
	try{
    //remove_line_item("cybersource-tax"); // this value has to be the same as the ID defined in hook_line_item() or it will not update the total
    // we add the line item
    // The definition for add_line_item() is: set_line_item(key, title, value, weight)
    /*set_line_item
    (
      "cybersource-tax", // this value has to be the same as the ID defined in hook_line_item() or it will not update the total
      Drupal.t("Cybersource tax"), // This is the text that will show up in the order total. I run it through Drupal.t() which is the javascript version of the t() translation function
      10 // Drupal.settings.myModule.giftWrappingPrice is the value of the discount that I passed from the PHP function
    );*/
   // render_line_items();
  $("#edit-panes-cybersource-tax-cybersource-tax").click(function()
  {
    switch($(this).attr("checked"))
    {
      case false: // This indicates that the checkbox has been unchecked, so we remove the line item
        remove_line_item("cybersource-tax"); // this value has to be the same as the ID defined in hook_line_item() or it will not update the total
        break;
      case true: // This indicates that the checkbox has been checked, so we add the line item
        // The definition for add_line_item() is: set_line_item(key, title, value, weight)
        /*set_line_item
        (
          "cybersource-tax", // this value has to be the same as the ID defined in hook_line_item() or it will not update the total
          Drupal.t("Cybersource tax"), // This is the text that will show up in the order total. I run it through Drupal.t() which is the javascript version of the t() translation function
          Drupal.settings.expSpCybersourceTax.cybersourceTaxPrice, // Drupal.settings.myModule.giftWrappingPrice is the value of the discount that I passed from the PHP function
          10
        );
        render_line_items();
        break;*/
    }
  });
  $(document).ready(function(e) {
	  
	  
  });
	}catch(e){
		// to do}
	}
};