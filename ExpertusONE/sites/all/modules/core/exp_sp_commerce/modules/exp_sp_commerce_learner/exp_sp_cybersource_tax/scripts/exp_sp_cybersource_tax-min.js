var expSpCybersourceTax={};Drupal.behaviors.expSpCybersourceTax=function(){try{expSpCybersourceTax.checkboxListener();}catch(a){}};expSpCybersourceTax.checkboxListener=function(){try{$("#edit-panes-cybersource-tax-cybersource-tax").click(function(){switch($(this).attr("checked")){case false:remove_line_item("cybersource-tax");break;case true:}});$(document).ready(function(b){});}catch(a){}};