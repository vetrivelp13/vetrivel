(function(a){a.widget("ui.learnerannouncement",{_init:function(){try{var b=this;this.renderMyLearningAnnouncement();}catch(c){}},paintMyLearningAnnouncement:function(b,d,j){try{var g=j.title;var c=j.full_title;var i=j.shortdesc;var f="";f+='<div class="mylearning-announcement-item clearfix">';f+='<table border="0" cellpadding="0" cellspacing="0" class="mt-content-table"><tr>';f+="<td>";f+='<div class="spotlight-block">';f+='<p class="spotlight-block">';f+='<a class="spotlight-item-title vtip" title="'+htmlEntities(c)+'">'+g+"</a>";f+='<span class="spotlight-title-breaker">&nbsp;</span>';f+=i;f+="</div>";f+="</td>";f+="</tr></table>";f+="</div>";return f;}catch(h){}},renderMyLearningAnnouncement:function(){try{var c=a("#learner_mylearning_announcement").data("learnerannouncement");a("#my_learning_announcement_jqgrid").jqGrid({url:this.constructUrl("learning/announcement"),datatype:"json",mtype:"GET",colNames:[""],colModel:[{name:"id",index:"id",width:268,title:false,sortable:false,formatter:c.paintMyLearningAnnouncement}],rowNum:a("#exp_sp_mylearning_announcement_block_max_list_count").val(),viewrecords:true,loadui:false,height:"auto",header:false,pager:"#my_learning_announcement_pager",pgtext:"{0} "+Drupal.t("LBL981")+" {1}",toppager:false,emptyrecords:"",loadtext:"",recordtext:"",beforeRequest:c.myLearningAnnouncementBeforeRequest,gridComplete:c.myLearningAnnouncementGridComplete,loadComplete:c.myLearningAnnouncementLoadComplete});
a(".ui-jqgrid").css("margin","0px");}catch(b){}},myLearningAnnouncementBeforeRequest:function(){try{a("#learner_mylearning_announcement").data("learnerannouncement").createLoader("mylearning_announcement_loader");}catch(b){}},myLearningAnnouncementGridComplete:function(b){try{a("#gbox_my_learning_announcement_jqgrid .ui-jqgrid-hdiv").remove();a("#gbox_my_learning_announcement_jqgrid").show();a("#prev_my_learning_announcement_pager").removeClass("ui-state-disabled");a("#next_my_learning_announcement_pager").removeClass("ui-state-disabled");a("#learner_mylearning_announcement").data("learnerannouncement").destroyLoader("mylearning_announcement_loader");a("#my_learning_announcement_pager_left, #my_learning_announcement_pager_right").hide();a(".ui-pg-table").attr("align","right");a("#my_learning_announcement_jqgrid").find(".mylearning-announcement-item").last().css({border:"none","padding-bottom":"8px"});a("#my_learning_announcement_jqgrid *").css("border-right","none 0px");}catch(c){}},myLearningAnnouncementLoadComplete:function(d){try{var c=parseInt(a("#exp_sp_mylearning_announcement_block_max_list_count").val());
if(d.records<=c){a("#my_learning_announcement_pager").hide();}if(parseInt(d.records)>0){a(".mylearning-announcement-inner").css("min-height","auto");}var b=a("#my_learning_announcement_jqgrid").jqGrid("getDataIDs");if(b.length<=0){a("#mylearning_announcement_msg").show();a("#mylearning_announcement_list").hide();}else{a("#mylearning_announcement_msg").hide();a("#mylearning_announcement_list").show();}vtip();}catch(f){}}});a.extend(a.ui.learnerannouncement.prototype,EXPERTUS_SMARTPORTAL_AbstractManager,EXPERTUS_SMARTPORTAL_AbstractDetailsWidget);})(jQuery);$(function(){try{$("#learner_mylearning_announcement").learnerannouncement();}catch(a){}});