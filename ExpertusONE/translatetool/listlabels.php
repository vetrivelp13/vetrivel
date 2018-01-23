<!DOCTYPE html>
<html>
<head>
   <title></title>
	<script type="text/javascript" src="/misc/jquery.js?v=1.4.4"></script>
   
   <!--   <script type="text/javascript" src="https://code.jquery.com/jquery-1.9.1.min.js"></script> -->
   <script type="text/javascript" src="/translatetool/translation.js?q=1"></script>
   <link rel="stylesheet" type="text/css" href="/translatetool/datatable.css">
   <link rel="stylesheet" type="text/css" href="/translatetool/translation.css"> 
   <script type="text/javascript" charset="utf8" src="/translatetool/dt.js"></script> 
   
   <script type="text/javascript" src="/sites/all/modules/core/exp_sp_learning/exp_sp_learning_learner/modules/exp_sp_lnrreports/jscrollbar/jquery.mousewheel.js?oyiqzm"></script>
   <script type="text/javascript" src="/sites/all/modules/core/exp_sp_learning/exp_sp_learning_learner/modules/exp_sp_lnrreports/jscrollbar/jquery.jscrollpane.js?oyiqzm"></script>
   
   <!--   <link rel="stylesheet" type="text/css" href="https://cdn.datatables.net/v/dt/dt-1.10.16/fh-3.1.3/datatables.min.css"/>
 
  <script type="text/javascript" src="https://cdn.datatables.net/v/dt/dt-1.10.16/fh-3.1.3/datatables.min.js"></script>
 <script type="text/javascript" src="https://cdn.datatables.net/fixedheader/3.1.3/js/dataTables.fixedHeader.min.js"></script> -->   
   
    <link rel="stylesheet" type="text/css" href="/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_learning/exp_sp_administration_data_grid_v2.css">
   <link rel="stylesheet" type="text/css" href="/sites/all/themes/core/expertusoneV2/expertusone-internals/css/layout-fixed.css">
   <link rel="stylesheet" type="text/css" href="/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_learning/exp_sp_administration_learning_v2.css">     
   <link rel="stylesheet" type="text/css" href="/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_v2.css">
   <link rel="stylesheet" type="text/css" href="/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_manage/exp_sp_administration_customattribute/exp_sp_administration_customattribute_v2.css">

<style>
 
</style>
</head>
<body>
<div id='loader' style="overflow:hidden;position:absolute;top:0px;left:0px;z-index:10007;width:90%;"></div>
<div class="e1_error"  style="position:absolute;top:5px;left:40%;height:auto;"></div>
<div class="container" style='overflow:hidden;' id='customattr-translation-container'>
<!--<div id="dtcontainer" style='height:439px;margin:10px;;margin-bottom:10px; overflow-x: auto;overflow-y: auto;border-top:0px;' class='jspScrollable'>-->
    <div id="dtcontainer" style='width:980px;max-height:439px;min-height:439px;margin:10px;;margin-bottom:10px;border-top:0px;'> 
  <table id="my-example" >
    <thead>
      <tr id='throw'>
        
      </tr>
    </thead>
  </table>
  </div>
 <div class="addedit-form-cancel-container-actions" style="margin-right:11px;width:200px;">
 
 <div class="admin-edit-button-container" >
   <div class="admin-edit-button-left-bg"></div>
   <input onclick='submitChanges();' style='float:right;' id='savebtn' class="save-button admin-edit-button-middle-bg" type="button" name="op"  value="Save">
   <div class="admin-edit-button-right-bg"></div>
</div>   
 
 <div style='float:right;'>&nbsp;</div>
   <div class="white-btn-bg-left"></div>
   <input style='float:right;' class="admin-action-button-middle-bg addedit-form-expertusone-throbber exp-addedit-form-cancel-button white-btn-bg-middle "
    data-wrapperid="contentauthor-addedit-form" onclick="window.parent.$('#delete-msg-wizard').remove();return false;" id="closebtn" tabindex="6" type="submit"  name="Close" value="Close">
   <div class="white-btn-bg-right"></div>
   
   
   
   <div class="clearBoth"></div>
</div>
</div>

</body>

<script type="text/javascript">
$("#closebtn").val(window.parent.Drupal.t("LBL123"));
$("#savebtn").val(window.parent.Drupal.t("LBL141"));

var editor;
var oTable;
var langCodes = "<?php print $_GET['langCodes'];?>";

function displayCol(data, type, full,colLang,mode)
{
	if(data == null || data == "null")
		data = "";
	var disableTextbox = "";
	var disabled = '';
	if(colLang == full.src_lang || mode =='v'){
		disableTextbox = "addedit-readonly-textfield";
		disabled = "disabled"
	}
	return "<input type=\"text\" class='textbox "+disableTextbox+"' "+disabled+" id=\""+full.id+"\" maxlength ='255' value=\""+data+"\"  onkeydown='processKey(this);'></input>";
	
}
  $(document).ready(function() {
	  var mode = window.parent.$("#iframedata").attr("mode");
	  if(mode == "v")
		  $("#savebtn").remove();
	  
	  var langs =langCodes.split(",");
	  
	  $('#dtcontainer').jScrollPane({
	        autoReinitialise: true,
            showArrows: false, 
            verticalGutter:0,
            horizontalGutter:0
        });  
        
	  var cols = "";
	/*  var langArr =[
          { mData: 'english', "sClass": "en" } ,
          { mData: 'chinese', "sClass": "zh" },
          { mData: 'japanese', "sClass": "ja"  },
          { mData: 'italiano' , "sClass": "it" },
        ];
*/

	  var langArr = new Array();
	  var thHtml="";
	  var col = ""; 
		col= { mData: 'english', "sClass": "en","mRender":function(data, type, full){
				return displayCol(data, type, full,"en",mode);
  		}};
	 	thHtml+="<th>English</th>";
	 	
		langArr.push(col);
	  for(var i = 0; i < langs.length;i++)
	  {
		  	col = "";
		    if(langs[i] == "it"){
		  		col= { mData: 'italiano', "sClass": "it","mRender":function(data, type, full){
		  				return displayCol(data, type, full,'it',mode);
			  		}};
		  		thHtml+="<th>Italiano</th>";
		  	 }
			else if(langs[i] == "ja"){
				col= { mData: 'japanese', "sClass": "ja","mRender":function(data, type, full){
					return displayCol(data, type, full,'ja',mode);
		  		} };
				thHtml+="<th>Japanese</th>";
			}
			else if(langs[i] == "zh-hans"){
				col= { mData: 'chinese', "sClass": "zh","mRender":function(data, type, full){
					return displayCol(data, type, full,'zh',mode);
		  		} };
				thHtml+="<th>Chinese</th>";
			}
			else if(langs[i] == "ko"){
				col= { mData: 'korean', "sClass": "ko","mRender":function(data, type, full){
					return displayCol(data, type, full,'ko',mode);
		  		} };
				thHtml+="<th>Korean</th>";
			}
			else if(langs[i] == "fr"){
				col= { mData: 'french', "sClass": "fr","mRender":function(data, type, full){
					return displayCol(data, type, full,'fr',mode);
		  		} };
				thHtml+="<th>French</th>";
				
			}
			else if(langs[i] == "pt-pt"){
				col= { mData: 'portuguese', "sClass": "pt","mRender":function(data, type, full){
					return  displayCol(data, type, full,'pt',mode);
		  		}};
				thHtml+="<th>Portuguese</th>";
			}
			else if(langs[i] == "pt-br"){
				col= { mData: 'portuguese', "sClass": "pt","mRender":function(data, type, full){
					return  displayCol(data, type, full,'pt',mode);
		  		}};
				thHtml+="<th>Portuguese</th>";
			}
			else if(langs[i] == "es"){
				col= { mData: 'spanish', "sClass": "es","mRender":function(data, type, full){
					return displayCol(data, type, full,'es',mode);
		  		} };
				thHtml+="<th>Spanish</th>";
				
			}
			else if(langs[i] == "ru"){
				col= { mData: 'russian', "sClass": "ru" ,"mRender":function(data, type, full){
					return displayCol(data, type, full,'ru',mode);
		  		}};
				thHtml+="<th>Russian</th>";
				
			}
			else if(langs[i] == "de"){
				col= { mData: 'german', "sClass": "de","mRender":function(data, type, full){
					return displayCol(data, type, full,'de',mode);
		  		} };
				thHtml+="<th>German</th>";
				
			}
		    if(col != "")
		  		langArr.push(col);
	  }
	  $("#throw").html(thHtml);
	  createLoaderNew("loader");
	  
	   oTable = $('#my-example').dataTable({
		  "fnInitComplete": function (oSettings) {
			  $("#loader").html("");
			  if($(oTable[0]).find("tr").length > 13){
			      //$("#my-example").jScrollPane();
			      $(".admin-edit-button-container").show();
			  }
			  	
			  if($(oTable[0]).find("tr").length  == 2){
				$(".dataTables_empty").html(window.parent.Drupal.t("MSG403")).show();
				$("#savebtn").hide();
				$('#dtcontainer').css({'min-height':'61px','max-height':'61px'});  
				
			   }
			     //$("#dtcontainer").css("overflow","unset");
			     $("#dtcontainer").css("overflow","hidden");
			      $("#customattr-translation-container").css("overflow","hidden");
			      
			      var recLeng = $(oTable[0]).find("tr").length;
	      		  var requiredHeight = recLeng*28;
	      		 // $("#dtcontainer").css("height",requiredHeight);  
	      		 // $(".jspContainer").css("min-height",requiredHeight);
	      		  
				  window.parent.$("#iframedata").css("height",requiredHeight+50);
				  window.parent.$("#delete-msg-wizard").css("height",requiredHeight+50);
				  window.parent.$("#delete-msg-wizard").css("max-height","485px");
				  
					
			  },
        "bProcessing": true,
        "bPaginate":false,
        "bSort" : false,
        "paging": false,
        
        "bLengthChange" : false, //thought this line could hide the LengthMenu
        "bInfo":false,
        "sAjaxSource": "/?q=ajax/administration/manage/customattribute/listLabels",
      //  "aoColumns":langArr,
        "aoColumns":langArr,
        
                   
      });  


	  /* $("body").on("click",'td',function(event){
		    if($(this).hasClass("processed"))
		    {
			    
		    }else
		    {
		    		var d = $(this).html();
		    		$(this).attr("olddata",d);
		    		var html = "<input type='text' size='20' value=\""+d+"\" onkeypress='processKey(this);'></input>";
				$(this).html(html).addClass("processed");
		    }
		});  */    
  });



  
</script>
</html>
