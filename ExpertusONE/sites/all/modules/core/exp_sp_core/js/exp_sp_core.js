/**

 * Configurable Parameters of Expertus SmartPortal
 */
var config={
    "data":[
          /* Position 0 - Base URL */
          {"url":[
                  {
                	  admin_url:(typeof resource != "undefined") ? resource.lnr_service_url : "",
                      learner_url:(typeof resource != "undefined") ? resource.lnr_service_url : ""
                  },
                  ]
          },

          /*  Position 1 - Service Action Key */
          {
              "serviceaction":
                  [
                   {
                	   LnrAdvSearchService : 'AdvanceSerarchView',
                	   AnnouncementService : 'AnnouncementEdit',
                	   LnrAnnouncementService : 'AnnouncementView',
                	   AssociateSurveyService : 'AssociateSurvey',
                	   AttachSurveyQuestionService : 'AttachQuestion',
                	   authenticate : 'Authenticate',
                	   AutocompleteService : 'Autocomplete',
                	   BatchMonitorService : 'BatchMonitor',
                	   BulkRegistrationService : 'BulkRegistration',
                	   CatalogService : 'Catalog',
                	   CertMasterService : 'CertificationEdit',
                	   LnrCertificationService : 'CertificationView',
                	   ContentMasterService : 'Content',
                	   LnrCourseDetailService : 'CourseDetailView',
                	   DatatableService : 'Datatable',
                	   LnrEnrollmentService : 'Enrollment',
                	   EnrollStatisticsService : 'EnrollStatistic',
                	   EntityProfileService : 'EntityProfile',
                	   ForumService : 'Forum',
                	   HelpService : 'Help',
                	   LnrHighlyRatedService : 'HighlyRatedView',
                	   InstructorService : 'InstructorEdit',
                	   InstructorService : 'InstructorView',
                	   LaunchService : 'Launch',
                	   LnrLaunchService : 'LaunchView',
                	   LearnerListService : 'LearnerList',
                	   LnrLearningService : 'LearningRequest',
                	   LnrMostViewedService : 'MostViewed',
                	   NotificationService : 'Notification',
                	   LnrOnlineUserService : 'OnlineUserView',
                	   OrganizationService : 'OrganizationEdit',
                	   PreRequisiteService : 'Prerequisite',
                	   PriceUnitService : 'Price',
                	   ProfileConfigService : 'ProfileConfig',
                	   PickListService : 'ProfileList',
                	   ProfileTabService : 'ProfileTab',
                	   ProfileService : 'Profile',
                	   PromoteService : 'Promote',
                	   ReadingService : 'Reading',
                	   ReferService : 'Refer',
                	   LnrRegistrationService : 'RegistrationDetailView',
                	   ReportMasterService : 'Report',
                	   ReportConfigService : 'ReportConfig',
                	   ReportThemeService : 'ReportTheme',
                	   ResourceLocationService : 'Resource',
                	   SavequeryService : 'SaveQuery',
                	   SecurityManagementService : 'SecurityManagement',
                	   LnrSearchService : 'SerarchView',
                	   LnrShoppingCartService : 'ShoppingCart',
                	   SurveyService : 'SurveyEdit',
                	   SurveyQuestionService : 'SurveyQuestionEdit',
                	   LnrsurveyService : 'SurveyView',
                	   TableExport : 'TableExport',
                	   TagsService : 'Tags',
                	   TagsAdminService : 'TagsEdit',
                	   ThemeAssociateService : 'ThemeAssociate',
                	   LnrTrainingProgramCalendarViewService : 'TPCalendarView',
                	   TrainingProgramService : 'TrainingProgramEdit',
                	   LnrTrainingProgramService : 'TrainingProgramView',
                	   TreeService : 'Tree',
                	   UserService : 'UserEdit',
                	   UserRoleMappingService : 'UserRoleMapping',
                	   LnrUserService : 'UserView',
                	   LnrNewReleasesService : 'NewReleases',
                	   MyTeamService : 'MyTeam',
                	   LnrNewUserService : 'NewUser',
					   LnrUserProfileEditService : 'UserProfileEdit',
                	   LnrAlertsService : 'LnrAlerts',
                	   NotificationMappingService : 'NotificationMapping',
                	   LnrPaymentCatalogService : 'PaymentCatalogView',
                	   LinkPathService : 'LinkPath',
                	   LnrMostActiveUserService : 'LnrMostActiveUser'
                    }
                  ]
          },
          /* Position 2 - SmartConnect Licensekey details */
          {"licensekey":[
               {
            	   admin_licensekey:(typeof resource != "undefined") ? resource.admin_licensekey : ""
               }
          ]},
          /* Position 3 - SmartConnect service numner details */
          {"SCServiceno":
            [
             {
                 all:"0002",
                 FederatedSearch:"0003",
                 UserService:'0006',
                 LnrUserService:'0012'
                 //SecurityManagementService:'0009'
             }
             ]
          },
          /* Position 4 - Search Config details */
          {"SearchConfig":
            {"advanced":true, "faq":true,"CategoryBrowse":false}
          },
          /* Position 5 - TimeZone Details */
          {"TimeZone":
                 [
                    {id:'1', value:'Samoa Standard Time (GMT-11:00)'},
                    {id:'2', value:'Hawaii-Aleutian Standard Time (GMT-10:00)'},
                    {id:'3', value:'Hawaii Standard Time (GMT-10:00)'},
                    {id:'4', value:'Alaska Standard Time (GMT-09:00)'},
                    {id:'5', value:'Pacific Standard Time (GMT-08:00)'},
                    {id:'6', value:'Mountain Standard Time (GMT-07:00)'},
                    {id:'7', value:'Mountain Standard Time (GMT-07:00)'},
                    {id:'8', value:'Central Standard Time (GMT-06:00)'},
                    {id:'9', value:'Central Standard Time (GMT-06:00)'},
                    {id:'10', value:'Eastern Standard Time (GMT-05:00)'},
                    {id:'11', value:'Eastern Standard Time (GMT-05:00)'},
                    {id:'12', value:'Atlantic Standard Time (GMT-04:00)'},
                    {id:'13', value:'Atlantic Standard Time (GMT-04:00)'},
                    {id:'14', value:'Newfoundland Standard Time (GMT-03:30)'},
                    {id:'15', value:'India Standard Time (GMT+05:30)'},
                    {id:'16', value:'Chamorro Standard Time (GMT+10:00)'},
                    {id:'17', value:'Moscow Standard Time (GMT+03:00)'},
                    {id:'20', value:'Greenwich Mean Time (GMT)'},
                    {id:'21', value:'Australia-Eastern Standard Time (GMT+10:00)'},
                    {id:'22', value:'Australia-Eastern Standard Time (GMT+10:00)'},
                    {id:'23', value:'Australia-Central Standard Time (GMT+09:30)'},
                    {id:'24', value:'Australia-Central Standard Time (GMT+09:30)'},
                    {id:'25', value:'Western Standard Time (GMT+08:00)'},
                    {id:'26', value:'Central European Standard Time (GMT+01:00)'},
                    {id:'27', value:'Abu Dabi, Muscat (GMT+04:00)'},
                    {id:'28', value:'Kuwait, Riyadh (GMT+03:00)'},
                    {id:'29', value:'Singapore, Malaysia Time (GMT+08:00)'},
                    {id:'30', value:'Pacific DayLight Time (GMT-09:00)'}
                ]
          },
          /* Position 6 - Delivery Type Details */
          {"DeliveryType":
              [
               {
                   "DT_lrn_cls_dty_ilt":{name:'ILT',id:'35619'},
                   "DT_lrn_cls_dty_wbt":{name:'WBT',id:'35620'},
                   "DT_lrn_cls_dty_vcl":{name:'VC',id:'35658',decode:'LIVE',decodeNm:'Virtual Class'},
                   "DT_lrn_cls_dty_vod":{name:"VOD",id:'37129'}
               }
               ]
          }
      ]
};

/**
* It is used to registernamespace
* @param ns
* @public
*/
 function registernamespace(ns)
{
	 try{
    var nsParts = ns.split(".");
    var root = window;

    for(var i=0; i<nsParts.length; i++)
    {
        if(typeof root[nsParts[i]] == "undefined")
        root[nsParts[i]] = new Object();
        root = root[nsParts[i]];
    }
	 }catch(e){
			// to do
		}
};

registernamespace("SMARTPORTAL");
SMARTPORTAL={
    AlertDetailInstance:new Array(),
    SearchWidgetInstance:new Array(),

    t:function(msgstr){
        return Drupal.t(msgstr);
    },

    getAllTimeZonesasSelect:function(pmNameStr,pmSelectId){

        var vTimeZoneStr='<select ' +pmNameStr+'>';
            vTimeZoneStr+="<option value=''>Select</option>";
        var vSelectedId ='';

        for(var i=0; i<config.data[5].TimeZone.length; i++)
        {
            vSelectedId='';
            if(pmSelectId==config.data[5].TimeZone[i].id){
                vSelectedId='selected';
            }
            vTimeZoneStr += "<option "+vSelectedId+" value='"+config.data[5].TimeZone[i].id+"'>"+config.data[5].TimeZone[i].value+'</option>';

        }
        vTimeZoneStr +='</select>';
        return vTimeZoneStr;
    }
};

/* Function is used to load other user profile in the popup page*/
function onLoadClick(otheruserid) {
	try{
		// Issue is fixed For this Ticket #0039337
		onLoadClickSkills();
		if(document.getElementById('myprofile-myactivity-screen')){
			$('#myprofile-myactivity-screen table.myactivity-details').replaceWith('<table cellspacing="2" cellpadding="2" class="myactivity-details" border="0" id="myprofile-activity-userdetails"></table>');
			$('#myprofile-myactivity-screen div#activity-pager').replaceWith('<div id="userdetailsactivity-pager"></div>');
			$("#myprofile-myactivity-screen").myprofileactivity();
		}

	}catch(e){
		// to do
	}
}
/* Function is used to load the skills into the popup page
* Issue is fixed For this Ticket #0039337
*/
function onLoadClickSkills() {
	try{
		if(document.getElementById('myprofile-myskillset-screen')){
			$('#myprofile-myskillset-screen table.myskill-details"').replaceWith('<table cellspacing="2" cellpadding="2" class="myskill-details" border="0" id="myprofile-skill-userdetails"></table>');
			$('#myprofile-myskillset-screen div#skill-pager').replaceWith('<div id="userdetailsskill-pager"></div>');
			$("#myprofile-myskillset-screen").myprofileskills();
		}
	}catch(e){
		// to do
	}
}

/*
window.alert=function(msg){
    callAlertDetail(msg);
};
*/
function isObject(obj){
	try{
    if (obj.constructor.toString().indexOf("Object") == -1)
        return false;
    else
        return true;
	}catch(e){
		// to do
	}
};

function isArray(obj)
{
	try{
    if (obj.constructor.toString().indexOf("Array") == -1)
        return false;
    else
        return true;
	}catch(e){
		// to do
	}
};

function object2string(obj,isparentArray){
	try{
    var outArray = new Array();
    var outstr = "";
    for ( var i in obj) {
        if(i!="prototype")
        {
            if (!isObject(obj[i]) && !isArray(obj[i])){
                if(isparentArray)
                {
                    outArray[outArray.length] = object2string(obj[i]);
                }
                else
                {
                    outArray[outArray.length] = "'" + i + "':'" + obj[i] + "'";
                }
            }
            else if(isObject(obj[i])){
                if(isparentArray)
                {
                    outArray[outArray.length] = object2string(obj[i]);
                }
                else
                {
                    outArray[outArray.length] = "'" + i + "':" + object2string(obj[i])+ "";
                }
            }
            else if(isArray(obj[i])){
                outArray[outArray.length] = "'" + i + "':[" + object2string(obj[i],true)+ "]";
            }
        }
    }

    if(outArray[outArray.length-1]==null)
    {
        outArray.pop();
    }
    outstr += (outArray.join(","));
    if(isparentArray)
    {
        return outstr;
    }
    else if(outstr.length!=0)
    {
        return "{" + outstr + "}";
    }
    else
    {
        return null;
    }
	}catch(e){
		// to do
	}
};


function DateCmp(date1, date2) {
	try{
    /* format 2009-12-31 */

    date1= date1.replace('-','/');
    date1= date1.replace('-','/');
    date2= date2.replace('-','/');
    date2= date2.replace('-','/');

    if(date1.length==10 && date1.length==10) {

    var d1=new Date(date1);
    var d2=new Date(date2);
    var diff=d2 - d1;

    return diff;
    }
    return -1;
	}catch(e){
		// to do
	}
};

function datetimeinmiliseconds(dt){
try{
    var completiondt_month=dt.substring((dt.indexOf('/')-2),dt.indexOf('/'));
    var completiondt_date=dt.substring((dt.lastIndexOf('/')-2),dt.lastIndexOf('/'));
    var completiondt_year=dt.substring((dt.lastIndexOf('/')+5),dt.lastIndexOf('/')+1);

    var completiondt_ts=new Date(completiondt_year,parseInt(completiondt_month,10)-1,completiondt_date);

    completiondt_ts=completiondt_ts.getTime();

    return completiondt_ts
}catch(e){
	// to do
}
}

/**********************************************************************
 * Component ID : after
 * Input        : Name          Type                    Description
 *                ------------- ----------------------- ------------
 *                date1          string                  -
 *                date2          string                  -
 * Output       : int
 * Description  : To check whether date2 is after date1.
 *                Return 0 - if date2 is after date1.
 *                Return 1 - if date1 is blank or is not a valid date.
 *                Return 2 - if date2 is blank or is not a valid date.
 *                Return 3 - if date2 is not after date1.
 **********************************************************************
 */
function after(date1, date2) {
	try{
    if (date1.length == 8 || date1.length == 9 || date1.length == 10) {
        arrTmp = date2.split ("/");

        date2 = new Date();
        date2.setFullYear(arrTmp[2]);
        date2.setMonth(arrTmp[0]-1);
        date2.setDate(arrTmp[1]);
        arrTmp = date1.split ("/");

        date1 = new Date();
        date1.setFullYear(arrTmp[2]);
        date1.setMonth(arrTmp[0]-1); // January = 0
        date1.setDate(arrTmp[1]);

        if (date2 > date1) {
            return 0;
        } else {
            return 3;
        }
    }
    if (date1.length == 6 || date1.length == 7) {
        arrTmp = date2.split ("/");

        date2 = new Date();
        date2.setFullYear(arrTmp[2]);
        date2.setMonth(arrTmp[0]-1);
        date2.setDate(arrTmp[1]);

        arrTmp = date1.split ("/");

        date1 = new Date();
        date1.setFullYear(arrTmp[1]);
        date1.setMonth(arrTmp[0]-1); // January = 0
        date1.setDate("1");

        if (date2 > date1) {
            return 0;
        } else {
            return 3;
        }
    }

    if (date1.length == 4) {
        arrTmp = date2.split ("/");

        date2 = new Date();
        date2.setFullYear(arrTmp[2]);
        date2.setMonth(arrTmp[0]-1);
        date2.setDate(arrTmp[1]);

        date1 = new Date();
        date1.setFullYear(date1);
        date1.setMonth("0");
        date1.setDate("1");

        if (date2 > date1) {
            return 0;
        } else {
            return 3;
        }
    }
    return 3;
	}catch(e){
		// to do
	}
}

function renderStars(val,id)
{
	try{
      if(val>5)
      {
            val = 5;
      }
      val=Math.round(val*100)/100;
      if(val.toString().indexOf('.') <0){
            val = val.toString() + '.00';
      }

      var onstars=Math.floor(val);
      var outhtml="";
      var decPart=0;
      var starcount=0;
       outhtml+="<div class='fivestar-widget-static' title='"+val+"'>";
      /*if(val!=0)
      {
            outhtml+="<div class='fivestar-widget-static' title='"+t("Rating for this item is")+"&nbsp;"+val+"&nbsp;"+t("out of")+"&nbsp;5'>";
      }
      else if(val==0 && id==undefined )
      {
            outhtml+="<div class='fivestar-widget-static' title='"+t("Please complete the course to rate")+".'>";
      }
      else
      {
            outhtml+="<div class='fivestar-widget-static' title='"+t("Course is not yet rated")+".'>";
      }*/
      if(onstars<val)
      {
            var temp=val.toString().split(".")[1];
                decPart=(eval("."+temp)*100)-5;
      }

      for(var i=0;i<onstars;i++)
      {
            outhtml+="<div class='star'><span class='on'></span></div>"
            starcount++;
      }
      if(eval(decPart)!=0)
      {
            var rate=decPart+"%";
            outhtml+="<div class='star'><span class='on' style='width:"+rate+"'></span></div>"
            starcount++;
      }
      if(starcount<5)
      {
            var j=5-starcount;
            for(var i=0;i<j;i++)
            {
                  outhtml+="<div class='star'><span class='off'></span></div>"
            }
      }
      outhtml+="</div>";
      if(id!=undefined && id.length!=0)
      {
            document.getElementById(id).innerHTML=outhtml;
      }
      return outhtml;
	}catch(e){
		// to do
	}
}

function stringToXml(xmlstr){
    try //Internet Explorer
    {
      xmlDoc=new ActiveXObject("Microsoft.XMLDOM");
      xmlDoc.async="false";
      xmlDoc.loadXML(xmlstr);
    }
    catch(e)
    {
        try{
            parser=new DOMParser();
            xmlDoc = parser.parseFromString(xmlstr,"text/xml");
        }catch(e){
            //alert(e.message);

        }
    }

    return xmlDoc;
}

function openDivAtPos(xDivId,xTop,xLeft){
	try{
    var style='top:'+xTop+';left:'+xLeft;
    var html='<div class="featured_video_div" style="'+style+'" id="'+xDivId+'" ></div>';
    if($('#'+xDivId)) $('#'+xDivId).remove();
    if($('body') != undefined) $('body').append(html);
	}catch(e){
		// to do
	}
}

function showDecimal(n) {
	try{
      ans = n * 1000
      ans = Math.round(ans /10) + ""
      while (ans.length < 3) {ans = "0" + ans}
      len = ans.length
      ans = ans.substring(0,len-2) + "." + ans.substring(len-2,len)
      return ans
	}catch(e){
		// to do
	}
}

function getWindowWidth(){
	try{
    var winW=0;
    if (parseInt(navigator.appVersion)>3) {
        if (navigator.appName=="Netscape") {
            winW = window.innerWidth-16;
        }
        if (navigator.appName.indexOf("Microsoft")!=-1) {
            winW = document.body.offsetWidth-20;
        }
    }
    return winW;
	}catch(e){
		// to do
	}
}

function titleRestricted(title,chara) {
	try{
	var restrictTitle;
	if(title.length > chara) {
		restrictTitle = title.substring(0,chara);
		restrictTitle = restrictTitle+'...';
	}else {
		restrictTitle =  title;
	}
	return restrictTitle;
	}catch(e){
		// to do
	}
}

function titleRestrictionFadeoutImage(title, uniqval) {
	try{
		var restrictTitleNew;
		if(title) {
		  class_fadeout = uniqval;
		  restrictTitleNew =  '<div class="fade-out-title-container-unprocessed fade-out-title-container '+class_fadeout+'"><span class="title-lengthy-text">'+title+'</span><span class="fade-out-image"></span></div>';
		}
		else {
			restrictTitleNew = '';
		}

	return restrictTitleNew;
	}catch(e){
		// to do
	}
}

function detailsShowHide(obj) {
	try{
	curObj = $(obj).parent(".more-text").siblings(".item-long-desc");
	elipsis = $(obj).parent(".more-text").siblings(".item-elipsis");
	if($(obj).hasClass("show-full-text")) {
		$(curObj).hide();
		$(elipsis).show();
		$(obj).removeClass("show-full-text");
		$(obj).addClass("show-short-text");
	} else {
		$(curObj).show();
		$(elipsis).hide();
		$(obj).removeClass("show-short-text");
		$(obj).addClass("show-full-text");
	}
	/*-- #34270 - scroll pane for the dynamic contents in team assign learn popup --*/
	if($("#my-team-dialog #gview_tbl-paintCatalogContentResults").length) {
		$("#my-team-dialog #gview_tbl-paintCatalogContentResults").jScrollPane();
	}
	}catch(e){
		// to do
	}
}

function getClickMylearn(){
	try{
	$.cookie("isclickmyenroll", "1");
	}catch(e){
		// to do
	}
}
function getClickSignIn(){
	try{
	$.cookie("isclickmyenroll", null);
	}catch(e){
		// to do
	}
}
 (function($) {
	$(document).ready( function() {
		try{
			$.metadata.setType('attr', 'data');
			$('.links-fieldset').find('legend').live("click",function(){
				$('.row_link').slideToggle();
			});
			$('#tabDiv').find("li").css("background-color","#DDDDDD");
			$('#tabDiv').find("li.ui-state-active").css("background-color","#EEEEEE");
			$('.secondary-menu-overlay').css('display','none');

			if($('#learner_announcements').size() == 0){$('#horiz-menu > table > tbody > tr > td> #navigation').css('height','46px');}
			$( "#resetPwdLink" ).parent().css( "display", "none" );

			if($('#learner_announcements').size() == 0){$('#horiz-menu > table > tbody > tr > td> #navigation').css('height','46px');}    
			if($.cookie('SurInvalidUser')) {
				if($.cookie('SurInvalidUser') == 1) {
					$('#signin').click();
					$.cookie('SurInvalidUser', null);
				}
			}
			$('#signin').bind("contextmenu",function(e){
				return false;
			});
			$('#shopping_cart').bind("contextmenu",function(e){
				return false;
			});
			afterRenderRefineIcon();
			lengthyNameFadeout();
			$('#secondary-menu').find('.secondary-menu-account-length').live("click",function(){
				showAccountSettings();
			});
			
		}catch(e){
			// to do
			// window.console.log(e, e.stack);
		}

	});
 })(jQuery);
 
 function lengthyNameFadeout() {
	    var secondaryMenuAccountSetting = $('#secondary-menu').find('#my-account-settings');
	    var secondaryMenuAccountSettingVal = secondaryMenuAccountSetting[0].innerHTML;
	    var secondaryMenuAccountSettingValLength = secondaryMenuAccountSettingVal.length;
	    if(secondaryMenuAccountSettingValLength > 50) {
	    	secondaryMenuAccountSetting[0].innerHTML =  titleRestrictionFadeoutImage(secondaryMenuAccountSetting[0].innerHTML, 'secondary-menu-account-length');
	    	$('#secondary-menu').find('.my-account-arrow-new').each(function() {
	    		$(this).css('background','url("/sites/all/themes/core/expertusoneV2/expertusone-internals/images/layoutBg.gif") no-repeat right -1493px');
	    	});
	    }
	  vtip();
	}
/**
 * Global variable which holds the portal configuration details
 */
 //IE 6 select tag issue

				 function hide_select()
				 {
					 try{
							if(/MSIE (\d+\.\d+);/.test(navigator.userAgent)){ //test for MSIE x.x;
								var ieversion=new Number(RegExp.$1);// capture x.x portion and store as a number
									if (ieversion<=7){
										$('.select_hide').hide();
									}
							}
					 }catch(e){
							// to do
						}
				}
				 function show_select()
				 {
					 try{
							if(/MSIE (\d+\.\d+);/.test(navigator.userAgent)){ //test for MSIE x.x;
								var ieversion=new Number(RegExp.$1);// capture x.x portion and store as a number
									if (ieversion<=7){
										$('.select_hide').show();
									}
							}
					 }catch(e){
							// to do
						}
				}
				function hide_findTraining()
				 {
					try{
						if(/MSIE (\d+\.\d+);/.test(navigator.userAgent)){ //test for MSIE x.x;
							var ieversion=new Number(RegExp.$1);// capture x.x portion and store as a number
								if (ieversion<=7){
									$('.select_fndtrng').hide();
								}
						}
					}catch(e){
						// to do
					}
				}
				 function show_findTraining()
				 {
					 try{
						if(/MSIE (\d+\.\d+);/.test(navigator.userAgent)){ //test for MSIE x.x;
							var ieversion=new Number(RegExp.$1);// capture x.x portion and store as a number
								if (ieversion<=7){
									$('.select_fndtrng').show();
								}
						}
					 }catch(e){
							// to do
						}
				}
var portalconfig;
var availableFunctionalities;
(function($) {
	try{
	$(document).ready(function(){
		if($('#enabledFunction').length > 0){
			var tmp = eval($('#enabledFunction').val());
			availableFunctionalities=tmp[0];
			$('#enabledFunction').remove();
		}
	});
	}catch(e){
		// to do
	}
})(jQuery);



/* Suresh modified for OAuth Integration */


function getUrlVars()
{
	try{
	 var vars = [], hash;
	    var hashes = window.location.href.slice(window.location.href.indexOf('?') + 1).split('&');
	    for(var i = 0; i < hashes.length; i++)
	    {
	        hash = hashes[i].split('=');
	        vars.push(hash[0]);
	        vars[hash[0]] = hash[1];
	    }
    return vars;
	}catch(e){
		// to do
	}
}
var sourceOAuth=getUrlVars()["source"];
if(sourceOAuth=="external")
{
$(document).ready(function()
{
	try{
	// $(".ctools-modal-ctools-login-style").click();
		$("#signin").click();
	}catch(e){
		// to do
	}
});
}

/* Suresh - End */


/* "Vtip" - Dislaying restrcted character in tooltip start */
/* Vtip - Width restricted while character length reach 500 */
function vtip(selector) {
	try{
	
		this.xOffset = -10; // x distance from mouse
		this.yOffset = 10; // y distance from mouse
		if(selector == null || selector == undefined)
			selector = '.vtip';
		else
			selector = selector + ' .vtip';
		$(selector).unbind().hover(
				function(e) {
					this.pageCon=$("#page-container").offset();
					this.docW=$(document).width();
					this.pageConLpos=this.pageCon.left;
					this.pageConw=$("#page-container").width();
					/* change done tio override browse button's default title behavior (No file Chosen)
			Done for Roster Upload icon in Enrollments popup*/
					if($(this).hasClass('browse-button-vtip')) {
						this.titletemp = this.getAttribute('titletemp');
					}
					this.t = (this.title != '' ? this.title : (this.titletemp !== undefined ? this.titletemp : ''));
					this.title = '';
					if(this.t != '') {
						this.top = (e.pageY + yOffset);
						if(this.t == Drupal.t('LBL847') || this.t == Drupal.t('LBL846')) {
							this.top = (e.pageY + yOffset)-95;
						} else {
							this.top = (e.pageY + yOffset);
						}
						this.left = (e.pageX + xOffset);
						var pageUrl = window.location.search;
						if(window.location.href.indexOf("admincalendar") >0 ) {
							if(this.t.length > 100) {
							  timeLen  = $(this).find(".titletimestyle").html().length + 1;
							  this.strLen = this.t.length-timeLen;
							}
							else {
							  this.strLen = this.t.length
							}
						} else {
						this.strLen = this.t.length
						}  
						//this.strLen = this.t.length;
						this.curWidth=this.left - this.pageConLpos;
						this.totWidthC=this.pageConw -this.curWidth;
						if(this.t == Drupal.t('LBL847') || this.t == Drupal.t('LBL846')) {
							$('body').append( '<p id="vtip"><img style="right:5px;" id="vtipArrow" />' + this.t + '</p>' );
						} else if($(this).hasClass('info-enr-upload')) {
							$('body').append( '<p id="vtip"><img style="right:5px;" id="vtipArrow" />' + this.t.replace(/<br>/g, '<br>') + '</p>' );
						} else {
							$('body').append( '<p id="vtip"><img style="left:5px;" id="vtipArrow" />' + htmlEntities(this.t) + '</p>' ); // 46942 - html encode support added
						}

						if(this.t == Drupal.t('LBL847') || this.t == Drupal.t('LBL846')) {
							$('body').append( '<p id="vtip"><span style="right:5px;" id="vtipArrow" class="vtip-arrow" > </span>' + this.t + '</p>' );
						} else if($(this).hasClass('info-enr-upload')) {
							$('body').append( '<p id="vtip"><span style="left:5px;" id="vtipArrow" class="vtip-arrow"> </span>' + this.t.replace(/<br>/g, '<br>') + '</p>' );
						} else {
							$('body').append( '<p id="vtip"><span style="left:5px;" id="vtipArrow" class="vtip-arrow"> </span>' + htmlEntities(this.t) + '</p>' ); // 46942 - html encode support added
						}
						//$('p#vtip #vtipArrow').attr("src", '/sites/all/themes/core/expertusone/expertusone-internals/images/vtip_arrow.png');
						this.vtipW=$('p#vtip').width();
						if(this.strLen>100) {
							var posLeft = this.left;
							if(this.totWidthC < 500){
								if($(this).hasClass('info-enr-upload')){
									$('p#vtip #vtipArrow').css('left','195px');
									posLeft = posLeft -200;
								}else if($("#cp-modalcontainer").hasClass('fullscreen-mode')){
								posLeft = posLeft -pvtipwidth;
								$('p#vtip #vtipArrow').css('left',vtiparrow+'px');
								}else{ 
								$('p#vtip #vtipArrow').css('left','200px');
								posLeft = posLeft -300;
								}
							}
							$('p#vtip').css("top", this.top+"px").css("left", posLeft+"px").css('width','500px').fadeIn(100);
						} else {
							var pvtipwidth = $("p#vtip").width();
							var posLeft = this.left;
							var vtiparrow = pvtipwidth -15;
							if(this.totWidthC < pvtipwidth){
								$('p#vtip #vtipArrow').css('left',vtiparrow+'px');
								posLeft = posLeft -pvtipwidth;
							}
							$('p#vtip').css("top", this.top+"px").css("left", posLeft+"px").fadeIn(100);
							$("p#vtip").css('max-width', '600px');
						}
						if($(this).hasClass('short-vtip')) {
							$("p#vtip").css('width', '200px').css('word-break', 'break-all');
						}
						else if($(this).hasClass('info-enr-upload')) {
							$("p#vtip").css('max-width', '230px');
						}
					}
					this.titletemp = this.t;
					
				},
				function() {
					if($(this).hasClass('browse-button-vtip')) {
						this.title = "";
					} else {
						this.title = this.t;
					}
					$("p#vtip").fadeOut("slow").remove();
				}
		).mousemove(
				function(e) {
					if(this.t != '') {
						this.pageCon=$("#page-container").offset();
						this.docW=$(document).width();
						this.pageConLpos=this.pageCon.left;
						this.pageConw=$("#page-container").width();
						this.curWidth=this.left - this.pageConLpos;
						this.totWidthC=this.pageConw -this.curWidth;
						this.top = (e.pageY + yOffset);
						//this.left = (e.pageX + xOffset);

						if(this.t == Drupal.t('LBL847') || this.t == Drupal.t('LBL846')) {
							this.left = (e.pageX + xOffset)-95;
						} else {
							this.left = (e.pageX + xOffset);
						}
						if(this.strLen>100) {
							var pvtipwidth = $("p#vtip").width();
							var posLeft = this.left;
							var vtiparrow = pvtipwidth -15;
							if(this.totWidthC < 500){
								if($(this).hasClass('info-enr-upload')){
									$('p#vtip #vtipArrow').css('left','195px');
									posLeft = posLeft -200;
								}else if($("#cp-modalcontainer").hasClass('fullscreen-mode')){
								posLeft = posLeft -pvtipwidth;
								$('p#vtip #vtipArrow').css('left',vtiparrow+'px');
								}else{ 
								$('p#vtip #vtipArrow').css('left','200px');
								posLeft = posLeft -300;
								}
							}else{
								$('p#vtip #vtipArrow').css('left','5px');
							}
							
							$('p#vtip').css("top", this.top+"px").css("left", posLeft+"px").css('width','500px').fadeIn(100);
						} else {
							var pvtipwidth = $("p#vtip").width();
							var posLeft = this.left;
							var vtiparrow = pvtipwidth -10;
							if(this.totWidthC < pvtipwidth){
								posLeft = posLeft -pvtipwidth;
								$('p#vtip #vtipArrow').css('left',vtiparrow+'px');
							}else{
								$('p#vtip #vtipArrow').css('left','5px');
							}
							$('p#vtip').css("top", this.top+"px").css("left", posLeft+"px").fadeIn(100);
							$("p#vtip").css('max-width', '600px');
						}
						if($(this).hasClass('short-vtip')) {
							$("p#vtip").css('width', '200px').css('word-break', 'break-all');
						}
						else if($(this).hasClass('info-enr-upload')) {
							$("p#vtip").css('max-width', '230px');
						}
					}
				}
		).mousedown(
				function(e) {
					if($(this).hasClass('browse-button-vtip')) {
						this.title = "";
					} else {
						this.title = this.t;
					}
					$("p#vtip").fadeOut("slow").remove();
				}
		);
		$('.lnrreports-search-display-table-name').removeClass('fade-out-title-container-unprocessed');
		$('.lnrreports-search-table-accordion-content').removeClass('fade-out-title-container-unprocessed');//fix for reports issue
		$('.fade-out-title-container-unprocessed').each(function() {
			//console.log('unprocessed'+$(this).width());
			//console.log('span'+$(this).find('.title-lengthy-text').width());
			if(($(this).width() >= $(this).find('.title-lengthy-text').width() && $(this).find('.title-lengthy-text').width() != 0) || $(this).find('.title-lengthy-text').text() == '') {
				$(this).find('.fade-out-image').remove();
			}
			$(this).removeClass('fade-out-title-container-unprocessed');
		});
		/*0051180: Title Restriction - Space available to display the report name */
		$('#paintLearnerReportsResults').find('.spotlight-item-title').each(function() {
			if($(this).width() >= $(this).find('.title-lengthy-text').width() && $(this).find('.title-lengthy-text').width() != 0) {
				$(this).find('.fade-out-image').remove();
			}
		});
		/*0051207: In Class page,fadeout effect were applied even it is small text. */
		$('#narrow-search-filtersets-holder').find('.narrow-search-filterset-item-label').each(function() {
			if($(this).width() >= $(this).find('.title-lengthy-text').width() && $(this).find('.title-lengthy-text').width() != 0) {
				$(this).find('.fade-out-image').remove();
			}
		});
		$('#paintLanguage').find('.srch-label-cls').each(function() {
			if($(this).width() >= $(this).find('.title-lengthy-text').width() && $(this).find('.title-lengthy-text').width() != 0) {
				$(this).find('.fade-out-image').remove();
			}
		});
		$('#admin-add-permissions').find('.user-list-detail').each(function() {
			if($(this).width() >= $(this).find('.title-lengthy-text').width() && $(this).find('.title-lengthy-text').width() != 0) {
				$(this).find('.fade-out-image').remove();
			}
		});
		$('#group-scroll-wrapper').find('.cls-access-list-select').each(function() {
			if($(this).width() >= $(this).find('.title-lengthy-text').width() && $(this).find('.title-lengthy-text').width() != 0) {
				$(this).find('.fade-out-image').remove();
			}
		});
		/*$('.session-details-warpper').find('.class-detail-session-instructor-fadeout-container').each(function() {
			if($(this).width() >= $(this).find('.title-lengthy-text').width() && $(this).find('.title-lengthy-text').width() != 0) {
				$(this).find('.fade-out-image').remove();
			}
		});
		$('.content-instructor-detail').find('.class-detail-ins-details-name-fadeout-container').each(function() {
			if($(this).width() >= $(this).find('.title-lengthy-text').width() && $(this).find('.title-lengthy-text').width() != 0) {
				$(this).find('.fade-out-image').remove();
			}
		});
		$('.content-instructor-detail').find('.class-detail-ins-details-job-fadeout-container').each(function() {
			if($(this).width() >= $(this).find('.title-lengthy-text').width() && $(this).find('.title-lengthy-text').width() != 0) {
				$(this).find('.fade-out-image').remove();
			}
		});*/
	}catch(e){
		// to do
	}
}
/*
 * Added by Vincent on July 18, 2012 to fix issue #0013955: The tooltip of announcement in homepage is displayed as 'undefined'
 * The root cause of the above issue is the vtip() is called on mouseover then only it has initialized and show the content
 * but before that the browsers default tooltip will be displayed. To avoid that here I called this function to initialize it.
 */

resetFadeOutByClass('.embedWigetPageContainer #class_detail_content .content-row-container','content-detail-code','line-item-container','class_details'); 
resetFadeOutByClass('.embedWigetPageContainer #course_detail_content .content-row-container','content-detail-code','line-item-container','class_details'); 
resetFadeOutByClass('.embedWigetPageContainer #lnp-details-content .content-row-container','content-detail-code','line-item-container','class_details'); 
 
vtip();

/* "Vtip" - tooltip end*/

//#6034 - To popup the user login screen, while user click the account link from mail
$(function() {
	try{
    var curUrl     	= window.location.search.substring(1);
    var splitUrl 	= curUrl.split('&');

    if(splitUrl[1] == 'destination=learning/my-account' || splitUrl[1] == 'destination=learning/enrollment-search' || splitUrl[1] == 'destination=learning/myteam-search') {
    	$(document).ready(function() {
    		//$(".ctools-modal-ctools-login-style").click();
    		$("#signin").click();
    	});
    }
	}catch(e){
		// to do
	}
});
/*
* The below statement is common for all the ctool popup start
*/
$('body').live('hover', function(){
	try{
	if($('#modalBackdrop').is(':visible') == true) {
		var getPopupCurrentHeight = '';
		getPopupCurrentHeight = $(document).height();
		$('#modalBackdrop').css('height',getPopupCurrentHeight+'px');
	}
	}catch(e){
		// to do
	}
});
$('body').live('mouseover', function(){
	try{
	if($('#modalBackdrop').is(':visible') == true) {
		var getPopupCurrentHeight = '';
		getPopupCurrentHeight = $(document).height();
		$('#modalBackdrop').css('height',getPopupCurrentHeight+'px');
	}
	}catch(e){
		// to do
	}
});
$('#modalBackdrop').live('hover', function(){
	try{
	if($('#modalBackdrop').is(':visible') == true) {
		var getPopupCurrentHeight = '';
		getPopupCurrentHeight = $(document).height();
		$('#modalBackdrop').css('height',getPopupCurrentHeight+'px');
	}
	}catch(e){
		// to do
	}
});
$('body').live('mousedown', function(){
	try{
	if($('#modalBackdrop').is(':visible') == true) {
		var getPopupCurrentHeight = '';
		getPopupCurrentHeight = $(document).height();
		$('#modalBackdrop').css('height',getPopupCurrentHeight+'px');
	}
	}catch(e){
		// to do
	}
});
$('body').live('DOMMouseScroll', function(){
	try{
	if($('#modalBackdrop').is(':visible') == true) {
		var getPopupCurrentHeight = '';
		getPopupCurrentHeight = $(document).height();
		$('#modalBackdrop').css('height',getPopupCurrentHeight+'px');
	}
	}catch(e){
		// to do
	}
});
$('body').live('keydown', function(){
	try{
	if($('#modalBackdrop').is(':visible') == true) {
		var getPopupCurrentHeight = '';
		getPopupCurrentHeight = $(document).height();
		$('#modalBackdrop').css('height',getPopupCurrentHeight+'px');
	}
	}catch(e){
		// to do
	}
});
/*
* The changes end
*/
$('body').click(function (event) {
	try{
	if(event.target.id!='pub-unpub-action-btn') {
		$('.narrowsearch-pub-add-list').hide();
	}
	}catch(e){
		// to do
	}
});
$('body').click(function (event) {
	try{
	if(event.target.className!='enroll-launch-more') {
		$('.enroll-action').hide();
	}
	}catch(e){
		// to do
	}
});
$('body').click(function (event) {
	try{
	if(event.target.className!='myteam-learning-arrow') {
		$('.myteam-more-action').hide();
	}
	}catch(e){
		// to do
	}
});
$('body').click(function (event) {
	try{
	if(event.target.className!='resource-add-action-wrapper') {
		$('.resource-add-list').hide();
	}
	}catch(e){
		// to do
	}
});

$('body').click(function (event) {
	try{
	if(event.target.id!='pub-unpub-action-btn' && event.target.id!='completedandsave_trigger' && $('.ui-datepicker').is(':hidden')
			&& !$(event.target).hasClass('enrollement-date-picker') && !$(event.target).hasClass('completion_container_roster')) {
		$('.catalog-pub-add-list').hide();
	}
	}catch(e){
		// to do
	}
});
$('body').click(function (event) {
	try{
	if(event.target.className!='man-opt-selection') {
			$('.sub-menu').hide();
		}
	}catch(e){
		// to do
	}
});
/* Catalog addmore button implemented  */
$('body').click(function (event) {
	try{
	if(event.target.className!='pub-unpub-add-action-wrapper') {
		$('.resource-add-list').hide();
	}
	}catch(e){
		// to do
	}
});
$('body .item-long-desc a,body .detail-desc a,body .item-short-desc a').live("click",function() {
	try{
	 $(this).attr('target', '_blank');
	}catch(e){
		// to do
	}
});
$('body .more-text a').live("click",function(event) {
	try{
	event.preventDefault();
	}catch(e){
		// to do
	}
});
$('body').click(function(event){
	try{
	if(Drupal.settings.ajaxPageState.theme == "expertusoneV2"){
	if(event.target.className != 'find-trng-sort-text' && event.target.className != 'frm-topic-list-sort-text' && event.target.className != 'frm-list-sort-text' && event.target.className != 'sort-by-text' && event.target.className !='narrow-search-sortbar-sorttext' && event.target.className !='find-trng-sortby' && event.target.className !='find-trng-sort-arrow-icon' && event.target.className != 'sort-by-text active') {
         $('.sort-by-links').hide();
		$('.sort-by-links').parents('#find-trng-sort-display').removeClass('sort-active');
         $('.narrow-search-sortbar-sortlinks').hide();
         $('.sort-by-change-links').hide();
        }
	if(event.target.className !='myteam-cat-delivery-type-link')	{
		       $('#myteam-cat-delivery-type-list').hide();
		}
	 var target = $(event.target).parents('.attach-grp-links');
	 if(target.length == 0 && $(event.target).parents('#list-options-tp').length == 0 && !$(event.target).hasClass('msg-close-btn')){
			$('.attach-grp-links').hide();
		}
	}

	}catch(e){
		// to do
	}
    });

/* Vtip tooltip */
$('.vtip').mouseover(function () {
	try{
	vtip();
	}catch(e){
		// to do
	}
});

/* System admin in home page */
$("#secondary-menu li a:contains('System Admin')").replaceWith('<span class="system-admin-arrow-link">System Admin</span>');
$('.system-admin-arrow-link').click(function() {
	try{
	$('#system-admin-menu').toggle();
	}catch(e){
		// to do
	}
});
$('body').click(function (event) {
	try{
	if(event.target.className!='system-admin-arrow-link') {
		$('#system-admin-menu').hide();
	}
	}catch(e){
		// to do
	}
});

/*
 * updateSelectFieldStyleOnChange() - 'Select' text in select fields when showing is to be greyed out.
 */
function updateSelectFieldStyleOnChange(obj) {
	try{
  //console.log('In updateSelectFieldStyleOnChange');
  //console.log(obj);
  var selectTxt = $('option:selected', obj).text();
  //console.log('selectTxt = ' + selectTxt);
  if(selectTxt == 'Select') {
    $(obj).removeClass('select-normal-text');
    $(obj).addClass('select-greyed-out-text');
  } else {
    $(obj).removeClass('select-greyed-out-text');
    $(obj).addClass('select-normal-text');
  }
	}catch(e){
		// to do
	}
}

/*
 * Drupal.behaviors.expSelectFieldStyle to show 'Select' text in select fields as greyed out
 */
Drupal.behaviors.expSelectFieldStyle =  {
  attach: function (context, settings) {
	  try{
    $('.exp-select-field-style:not(.exp-select-style-processed)').addClass('exp-select-style-processed').each(function () {
      var selectTxt = $('option:selected', this).text();
      //console.log('selectTxt = ' + selectTxt);
      if(selectTxt == 'Select') {
        $(this).removeClass('select-normal-text');
        $(this).addClass('select-greyed-out-text');
      } else {
        $(this).removeClass('select-greyed-out-text');
        $(this).addClass('select-normal-text');
      }
    });
	  }catch(e){
			// to do
	  }
  }
};

/**
 * Added by vincent on 09 Jan 2012 to fix serializeToString issue
 * for IE9. IE9 is not supporting serializeToString function and
 * it throws "no such interface supported" error. To avoid this
 * error and make our product to support IE9 the following fix
 * is needed. This is a temporary fix, if this issue is sorted out
 * in IE9 remove this fix.
 */
//if(Sarissa._SARISSA_IS_IE && parseFloat(navigator.appVersion.substring(navigator.appVersion.indexOf("MSIE")+5)) >=9) {
if(parseFloat(navigator.appVersion.substring(navigator.appVersion.indexOf("MSIE")+5)) >=9) {
	try{
	window.XMLSerializer = function(){};
	window.XMLSerializer.prototype.serializeToString=function(oNode){return oNode.xml;}
	}catch(e){
		// to do
	}
}

$(document).keydown(function(e){
	try{
    if ( e.keyCode == 8 && e.target.tagName != 'INPUT' && e.target.tagName != 'TEXTAREA') {
        e.preventDefault();
    }
	}catch(e){
		// to do
	}
});

function showAccountSettings () {
	try{
	if(Drupal.settings.ajaxPageState.theme == "expertusoneV2"){
		$('#account_setting').toggle();
	}else{
		$('#account_setting').show();
	}
	}catch(e){
		// to do
	}
}

//Close styling pop-up
$('.account-close-popup').live('click', function(){
	try{
	$('#account_setting').css('display','none');
	}catch(e){
		// to do
	}
});

$('body').bind('click', function(event) {
 try{
  if(event.target.id != 'my-account-settings'){
   $('#account_setting').css('display','none');
  }
 }catch(e){
		// to do
	}
});


/*
 * descController() - To Return Short and Long Description according to the number of characters required. Already we have same function in core.inc for PHP usage
 * This Function has Two Parameters
 * No.1 : $keyword - a unique keyword to be checked inside to change the function level variable
 * No.2 : $cString - the source string which would be worked upon.
 *
 * RETURN VALUES
 *     1. The Short Description String along with its tag element.
 *     2. The ellipsis along with its tag element
 *     3. The Long Description String along with its tag elements.
 *     4. The Anchor link showing up the arrow, which carries its show/hide functionality.
 */
function descController(keyword, cString,len) {
try{
  var cString 		= cString.replace(/(<([^>]+)>)/ig,"");
  var string_len 	= cString.length;
  var keyword 		= keyword.toUpperCase();
  var maxLength 	= 0;
  switch(keyword) {
    case 'ADMIN FORUM COURSES':
      maxLength = 250;
      break;
    case 'CALENDAR CODE':
      maxLength = 7;
      break;
  }
  if(typeof len != 'undefined')
	  maxLength = len;
  //var isutfstr = cString.fromCharCode(ascii_code);
  var res ='';
  if (typeof(cString) == 'string'){
	  if(string_len > maxLength) {
		  if(keyword != 'FADE OUT'){
	 		res +=  '<span class="item-short-desc">'+cString.substr(0, maxLength-1)+'</span>';
	        res += '<span class="item-elipsis">...</span>';
	        res += '<span class="item-long-desc">'+cString.substr(maxLength-1)+'</span>';
	        res += '<span class="more-text">';
	        res += '<a href="javascript:void(0)" onclick="detailsShowHide(this);" class="show-short-text more-icon-sec">&nbsp;</a></span>';
		  }else{
			  var sText = cString.substr(0, maxLength-1);
			  res += sText+'<span class ="textfade">'+cString.substr(sText.length, 2)+'</span><span class ="textfade1">'+cString.substr((sText.length + 2), 1)+'</span>';
		  }
	  } else {
		  if(keyword != 'FADE OUT'){
			  res += '<span class="item-short-desc">'+cString+'</span>';
		  }else{
			  res += cString;
		  }
	  }
  }else{
		if(string_len > maxLength) {
				var pos=0;
				for(var i = maxLength; i >= 0; i--){
					var chara = Substr(cString,i,1);
					if(empty(chara) || chara == ' ' || ctype_space(chara) || preg_match('/\s/', chara, m, PREG_OFFSET_CAPTURE) || is_null(chara)){
						pos = i;
						break;
					}
				}
				if(pos != 0) {
					if(keyword != 'FADE OUT'){
						res += '<span class="item-short-desc">'+cString.substr(0, pos)+'</span>';
						res += '<span class="item-elipsis">...</span>';
						res += '<span class="item-long-desc">'+cString.substr(pos)+'</span>';
						res += '<span class="more-text">';
						res += '<a href="javascript:void(0)" onclick="detailsShowHide(this);" class="show-short-text">&nbsp;</a></span>';
					}else{
						var sText = cString.substr(0, pos);
						res += sText+'<span class ="textfade">'+cString.substr(sText.length, 2)+'</span><span class ="textfade1">'+cString.substr((sText.length + 2), 1)+'</span>';
					}
	      } else {
	    	  if(keyword != 'FADE OUT'){
	    		  res += '<span class="item-short-desc">'+cString+'</span>';
	    	  }else{
	    		  res += cString;
	    	  }
	      }
  	} else {
  		if(keyword != 'FADE OUT'){
  			res += '<span class="item-short-desc">'+cString+'</span>';
  		}else{
  			res += cString;
  		}
  	}

  }
  return res;
}catch(e){
	// to do
}
}
/*Strip tag for removing html in JS*/

/*function strip_tags_addComments(html) {
	//PROCESS STRING
	if(arguments.length < 3) {
		html=html.replace(/<\/?(?!\!)[^>]*>/gi, '');
	} else {
		var allowed = arguments[1];
		var specified = eval("["+arguments[2]+"]");
		if(allowed){
			var regex='</?(?!(' + specified.join('|') + '))\b[^>]*>';
			html=html.replace(new RegExp(regex, 'gi'), '');
		} else{
			var regex='</?(' + specified.join('|') + ')\b[^>]*>';
			html=html.replace(new RegExp(regex, 'gi'), '');
		}
	}

	//CHANGE NAME TO CLEAN JUST BECAUSE
	var shortForum_string = html;

	//RETURN THE CLEAN STRING
	return shortForum_string;
}*/
/**
 * To display forum description with html entities.
 * @param shortString
 * @param fullString
 * @return
 */
function addExpanColapse(shortString,fullString,dataDelTypeCode,type,enr_id){
	try{
	if(Drupal.settings.ajaxPageState.theme == "expertusoneV2") {
		if(typeof(dataDelTypeCode)==='undefined') dataDelTypeCode = '';
	}else{
		dataDelTypeCode = '';
	}
	var res='';
	//var shortString=strip_tags_addComments(shortString);//strip tag for forum add comments
	var dispLength = shortString.length;
	var fullLenght = fullString.length;
	//console.log(shortDescForum);
	if(shortString.substr(dispLength-3)=="..."){
		res +=  '<span class="item-short-desc">'+shortString.substr(0,dispLength-3)+'</span>';
	    res += '<span class="item-elipsis">...</span>';
	    res += '<span class="item-long-desc">'+fullString+'</span>';
	    res += '<span class="more-text">';
	    if(type!='' && type != null && type!= undefined){
	    	if(type == 'TP')
	    		res += '<a href="javascript:void(0)" onclick="$(\'#prg-accodion-prg_'+enr_id+'\').click();" class="show-short-text more-icon-'+dataDelTypeCode.split('_').pop()+'">&nbsp;</a></span>';
	    	if(type=='ME')
	    		res += '<a href="javascript:void(0)" onclick="$(\'#class-accodion-'+enr_id+'\').click();" class="show-short-text more-icon-'+dataDelTypeCode.split('_').pop()+'">&nbsp;</a></span>';
	    	if(type=='MC')
	    		res += '<a href="javascript:void(0)" onclick="$(\'#enroll-main-action-'+enr_id+' [name='+Drupal.t('LBL816')+']\').click();" class="show-short-text more-icon-'+dataDelTypeCode.split('_').pop()+'">&nbsp;</a></span>';

	    }else
	    	res += '<a href="javascript:void(0)" onclick="descriptionShowHide(this);" class="show-short-text more-icon-'+dataDelTypeCode.split('_').pop()+'">&nbsp;</a></span>';
	}else{
		res += '<span class="item-short-desc">'+/*strip_tags_addComments(*/shortString/*)*/+'</span>';
	}
    return res;
	}catch(e){
		// to do
	}
}

/**
 * Forum description show/hide function
 * @param obj
 * @return
 */

function descriptionShowHide(obj){
	try{
	curObj = $(obj).parent(".more-text").siblings(".item-long-desc");
	shortObj = $(obj).parent(".more-text").siblings(".item-short-desc");
	elipsis = $(obj).parent(".more-text").siblings(".item-elipsis");
	if($(obj).hasClass("show-full-text")) {
		$(curObj).hide();
		$(elipsis).show();
		$(shortObj).show();
		$(obj).removeClass("show-full-text");
		$(obj).addClass("show-short-text");
		if($('#my-team-dialog #tbl-paintCatalogContentResults .find-training-txt span.more-text .show-full-text').length == 0) {
			$('#my-team-dialog #gview_tbl-paintCatalogContentResults').data('jsp') !== undefined && $('#my-team-dialog #gview_tbl-paintCatalogContentResults').data('jsp').destroy();
		}

	} else {
		$(curObj).show();
		$(elipsis).hide();
		$(shortObj).hide();
		$(obj).removeClass("show-short-text");
		$(obj).addClass("show-full-text");
		if($('#my-team-dialog #gview_tbl-paintCatalogContentResults').length){
		  $('#my-team-dialog #gview_tbl-paintCatalogContentResults').jScrollPane();
		}
	}
	/*-- #40207 - Jscrollpane container has more course/class --*/
	if ($('#lnr-prerequisite-container #gview_lnr-prerequisite').length) {
		$('#lnr-prerequisite-container #gview_lnr-prerequisite').jScrollPane();
	}
	if($('#select-class-dialog #gview_tbl-paintCatalogContentResults').length){
		$('#select-class-dialog #gview_tbl-paintCatalogContentResults').jScrollPane();
	}
	if($('.calviewmorepopup-content').length){
		$('.calviewmorepopup-content').jScrollPane();
	}
	}catch(e){
		// to do
	}
}

/**
 * Setting the cursor possion at the end of the content in the html editor
 * @param ed - tinyMCE active editor
 * @param node - tinyMCE active editor body node
 * @param start - (0 or 1)whether to update the dummy character at start or end
 * @param return_node - booleon whether to remove the added dummy character or not
 * @return
 */

function setCursor(ed, node, start, return_node){
	try{
	  tn = ed.getDoc().createTextNode("");
	  if (start){
	    node.insertBefore(tn, node.firstChild);
	  }
	  else node.lastChild.appendChild(tn);
	  rng = ed.selection.getRng();
	  rng.selectNode(tn);
	  rng.setStartBefore(tn);
	  rng.setStartAfter(tn);
	  ed.selection.setRng(rng);
	  if (return_node) return tn;
	  node.lastChild.removeChild(tn);
	  //callMessageWindow('Warnning','Maximum character reached',ed);
	  ed.focus();
	}catch(e){
		// to do
	}
 }

function expertus_error_message(message,type){
	try{
	var nsParts = window.location.href.split("/?q=");
	var html = '';
	var html1 = '';
	var commonErrorMessage = Drupal.t('ERR101');
	var multiplemessage =  Drupal.t('ERR169');
	var conMessage = Drupal.t('LBL647');
	conMessage = conMessage.toLowerCase();
	conMessage = ' '+conMessage+' ';
	var newmessage = new Array();
	newmessage[0] = '';
	var count = message.length;
	var newcount;
	var pos;
	var inc = 1;
	var replace_message;
	if (count>1){
		for(var i=0;i<count;i++){
			message[i] =  $.trim(message[i]);
			if(message[i].indexOf(commonErrorMessage)>=0){
				replace_message = message[i].replace(commonErrorMessage,'');
				replace_message = $.trim(replace_message);
				newmessage[0] = (newmessage[0] =='') ? replace_message : newmessage[0]+','+replace_message;
			}
			else{
				newmessage[inc] = message[i];
		        inc++;
		      }
		}
		if(newmessage[0] != ''){
			var position = newmessage[0].lastIndexOf(',');
			if(position>=0){
				var firstMsg = newmessage[0].substring(0,position);
				var secondMsg = newmessage[0].substring(position);
				secondMsg = secondMsg.replace(',' , conMessage);
				newmessage[0] = firstMsg+secondMsg +multiplemessage;
			}
			else{
				newmessage[0] = newmessage[0]+commonErrorMessage;
			}
			message = newmessage;
		}
	}
	newcount = message.length;
	if(newcount >0){
		html1+='<div id="message-container"><div class="messages '+type+'">';
		if(newcount >1){
			var outputval = '';
			for(var start=0; start < newcount; start++){
				outputval += '<li class="hide"><span>' + message[start]+ '</span></li>';
	        }
			html+='<span class="qtip-error-msg-display"><ul>'+outputval+'</ul></span>';
		}
		else {
			html+='<ul><li><span>'+message[0]+'</span></li></ul>';
		}
	}
	/*if(newcount>1){
		html+= "<div class='msg-minmax-icon'></div>";
      }*/
	if(nsParts[1] == 'cart/checkout'){
	var msgVal = $('#message-container').html();
	if(msgVal == null || msgVal ==''){
		html=html1+html;
		html+='<div class="msg-close-btn" onclick="$(\'body\').data(\'learningcore\').closeErrorMessage();"></div></div></div>';
		html+='<img onload="$(\'body\').data(\'learningcore\').showHideMultipleLi();" style="display:none;" src="sites/all/themes/core/expertusone/expertusone-internals/images/close.png" height="0" width="0" />';
	}else{
		html = '</br>'+html;
	}
	return html;
	}else{
		html=html1+html;
		html+='<div class="msg-close-btn" onclick="$(\'body\').data(\'learningcore\').closeErrorMessage();"></div></div></div>';
		html+='<img onload="$(\'body\').data(\'learningcore\').showHideMultipleLi();" style="display:none;" src="sites/all/themes/core/expertusone/expertusone-internals/images/close.png" height="0" width="0" />';
		return html;
	}
	}catch(e){
		// to do
	}
}
function checkboxSelectedUnselectedCommon(id,entityId,entityType){
	try{
	if($(id).attr('type')!='radio'){
		if($(id).is(':checked')){
			$(id).parent().removeClass('checkbox-unselected');
			$(id).parent().addClass('checkbox-selected');
		}
		else {
			$(id).parent().removeClass('checkbox-selected');
			$(id).parent().addClass('checkbox-unselected');
		}
	}

	var txtInput = $(id).attr('name').split('-');
	if(txtInput[0].indexOf('attach_attributes[]') != -1) {
		var bool=true;
		$("input[name='"+id.name+"']").each(function(){
			if(!($(this).is(":checked")))
			{
				bool=false;
			}
		});
		if(bool)
		{
			$("#usr_select").parent().removeClass('checkbox-unselected');
			$("#usr_select").parent().addClass('checkbox-selected');
		}
		else
		{
			$("#usr_select").parent().removeClass('checkbox-selected');
			$("#usr_select").parent().addClass('checkbox-unselected');
		}
	}
	
	/*--#38292 - issue fix--*/
	if(txtInput[0].indexOf('attach_business_rules') != -1) {
		var cheklen = $('#business-rules-table .brl-checkbox-input').length;
		var tempResult = $('#business-rules-table #business-rule-' + $(id).attr('value')).attr('checked');
		if(cheklen > 1) {
			$('#business-rules-table .brl-checkbox-input').each(function(index, value) {
				$(this).parent('div').addClass('checkbox-unselected');
				$(this).removeAttr('checked');
			});
		}
		if(tempResult == true) {
			$('#business-rules-table #business-rule-' + $(id).attr('value')).parent('div').removeClass('checkbox-selected');
			$('#business-rules-table #business-rule-' + $(id).attr('value')).parent('div').removeClass('checkbox-unselected');
			$('#business-rules-table #business-rule-' + $(id).attr('value')).parent('div').addClass('checkbox-selected');
			$('#business-rules-table #business-rule-' + $(id).attr('value')).attr('checked', 'checked');
		}
	}
	if(txtInput[1] == 'assessment' && $(id).attr('type') == 'radio'){
		var getID = $(id).attr('name').split('_');
        $('#assessment-maxattempt-'+getID[4]).removeAttr('disabled');
        $('#assessment-maxattempt-'+getID[4]).val("1");
        var $inputs = $("#datagrid-container-assessment-"+entityId+"-"+entityType+" :text");
        var values = {};
        $inputs.each(function() {
            values[this.name] = $(this).attr('id');
            if($('#assessment-maxattempt-'+getID[4])){
            	$('#assessment-maxattempt-'+getID[4]).removeAttr('disabled');
                $('#assessment-maxattempt-'+getID[4]).val("1");
            }
            if('#'+values[this.name] != '#assessment-maxattempt-'+getID[4]){
            	$('#'+values[this.name]).attr('disabled','disabled');
            	$('#'+values[this.name]).val("");
            }


        });
	}else if(txtInput[0] == 'groupuser'){
		var attr = $(id).attr('id').split('-'),
		grpId = attr[2],
		usrId = attr[3],
		action = '';
		if($(id).is(':checked')){
			action = 'remove';
		}else{
			action = 'add';
		}
		$('#root-admin').data('peoplegroup').rowEditGroupuser(grpId,usrId,action);
	}

	//Custom Attribute Changes #custom_attribute_0078975
	if(txtInput[0].indexOf('ent_view_opt[]') != -1) {

		/*if($('input[id^="view_opt_"]').parent().hasClass('checkbox-selected')){
			$('input[id^="edit_opt_"]').parent().removeClass('checkbox-unselected');
			$('input[id^="edit_opt_"]').parent().addClass('checkbox-selected'); 
			$('input[id^="edit_opt_"]').prop( "checked", true );
			
		}*/
		if($('#view_opt_' + $(id).attr('value')).parent().hasClass('checkbox-selected')){
			$('#edit_opt_' + $(id).attr('value')).parent().removeClass('checkbox-unselected');
			$('#edit_opt_' + $(id).attr('value')).parent().addClass('checkbox-selected'); 
			$('#edit_opt_' + $(id).attr('value')).prop( "checked", true );
			
		}	
	} else if(txtInput[0].indexOf('ent_edit_opt[]') != -1) {

		/*if($('input[id^="edit_opt_"]').parent().hasClass('checkbox-unselected')){
			$('input[id^="view_opt_"]').parent().removeClass('checkbox-selected');
			$('input[id^="view_opt_"]').parent().addClass('checkbox-unselected');
			$('input[id^="view_opt_"]').prop( "checked", false );
		}*/
		if($('#edit_opt_' + $(id).attr('value')).parent().hasClass('checkbox-unselected')){
			$('#view_opt_' + $(id).attr('value')).parent().removeClass('checkbox-selected');
			$('#view_opt_' + $(id).attr('value')).parent().addClass('checkbox-unselected');
			$('#view_opt_' + $(id).attr('value')).prop( "checked", false );
		}
	}
	
	}catch(e){
		// to do
	}
}

function sortTypeToggle(toggleClassName) {
	try{
	$('.'+toggleClassName).toggle();
	}catch(e){
		// to do
	}
}
//$('#message-container').remove();

function changeDialogPopUI()
{
	try{
	var recls=$('.removebutton').css("display");
	 var chkchangeClsOverride=$( ".ui-dialog" ).hasClass("popupLeftCorner" );
     if(chkchangeClsOverride==false){
	 $('.ui-dialog-titlebar').wrapAll(document.createElement('div'));
	 $('.ui-dialog-titlebar').parents('div').addClass('popupLeftCorner');
	 $('.ui-dialog-titlebar').wrapAll(document.createElement('div'));
	 $('.ui-dialog-titlebar').parents('div').addClass('popupRightCorner');
	 $('.ui-dialog').find('.popupLeftCorner').removeClass('popupRightCorner');
	 $('.ui-dialog').addClass('expertusV2PopupContainer');
	 $('.ui-dialog-buttonpane,.launch-wizard-content,.user-popout-container,.assign-learning-team,.edit-report-wizard, .launch-lp-wizard-content, #lesson-wizard, .pre-select-class').after('<div class="popupBotLeftCorner ui-helper-clearfix"><div class="popupBotRightCorner"><div class="popupBotmiddle"></div></div></div>');

	//close button script
	// $('.removebutton').wrapAll(document.createElement('div'));
	 //$('.removebutton').parent('div').addClass('white-btn-bg-container');
	// alert(recls);
	 if(recls=="block" || recls=='inline-block'){
	 $('.removebutton').before('<div class="white-btn-bg-left"></div>');
	 $('.removebutton').addClass('white-btn-bg-middle');
	 $('.removebutton').after('<div class="white-btn-bg-right"></div>');
	 }
	}}catch(e){
		// to do
	}
}
function changeChildDialogPopUI(parentDiv)
{
	try{
	var recls=$('#' + parentDiv + ' .removebutton').css("display");
	//var chkchangeClsOverride1=$( ".ui-dialog" ).hasClass("popupLeftCorner" );
	var chkchangeClsOverride1=$( '#' + parentDiv + ' .ui-dialog' ).hasClass("popupLeftCorner" );
    if(chkchangeClsOverride1==false){
	 $('#' + parentDiv + ' .ui-dialog-titlebar').wrapAll(document.createElement('div'));
	 $('#' + parentDiv + ' .ui-dialog-titlebar').parents('div').addClass('popupLeftCorner');
	 $('#' + parentDiv + ' .ui-dialog-titlebar').wrapAll(document.createElement('div'));
	 $('#' + parentDiv + ' .ui-dialog-titlebar').parents('div').addClass('popupRightCorner');
	 $('#' + parentDiv + ' .ui-dialog').find('.popupLeftCorner').removeClass('popupRightCorner');
	 $('#' + parentDiv + ' .ui-dialog').addClass('expertusV2PopupContainer');
	 $('#' + parentDiv + ' .ui-dialog-buttonpane').after('<div class="popupBotLeftCorner ui-helper-clearfix"><div class="popupBotRightCorner"><div class="popupBotmiddle"></div></div></div>');

	 if(recls=="block" || recls=='inline-block'){
	   $('#' + parentDiv + ' .removebutton').before('<div class="white-btn-bg-left"></div>');
	   $('#' + parentDiv + ' .removebutton').addClass('white-btn-bg-middle');
	   $('#' + parentDiv + ' .removebutton').after('<div class="white-btn-bg-right"></div>');
	 }
	}}catch(e){
		// to do
	}
}

function xss_validation(x){
	try{
		//change by ayyappans for 40543: Irrelavant Validation message appears
		var Re = /(<[^>]+?[\x00-\x20\"\'])(?:on|xmlns)|(<[^>]+?[\x00-\x20\"\'])(?:script)|(<+?(script|img|image|input|body|alert|meta|&lt;|&gt;|xmlns|iframe|br|frameset)\s)|(<+?(script|xmlns|iframe|frameset|body))/ig;
		x = x.replace(Re," ");
		return x;
	}catch(e){
		// to do
	}
}

var nsParts = window.location.href.split("/?q=");
// Restrict the Page For Expertus Show Message
if(nsParts[1] != 'cart/checkout' && nsParts[1] != 'cart/checkout/paymethod' && nsParts[1] != 'cart/checkout/review' && nsParts[1] != 'user/password'
	&& (window.location.href.indexOf('user/password&language')< 0) && nsParts[1] !='learning/reset-password' && nsParts[1] != 'administration/order/create'
	&& nsParts[1] != 'administration/order/pay' && nsParts[1] != 'cart' && (nsParts[1] !=undefined && nsParts[1].indexOf('clientcreation') < 0)){
	$('#message-container').remove();
}
Drupal.t('English');
Drupal.t('Simplified Chinese');
Drupal.t('German');
Drupal.t('French');
Drupal.t('Spanish');
Drupal.t('Italian');
Drupal.t('Japanese');
Drupal.t('Korean');
Drupal.t('Russian');

function editorOnClick(id){
	try{
	if(document.getElementById('cursor_id')!= null){//id == 'edit-message-value'){
		$('#cursor_id').val(id);
	}
	$('#message-container').remove();
	}catch(e){
			// To Do
		}
}

// Added by Vincent on 17 Jul 2014.
// Since IE8 does not support indexOf on array
// Below code will make that support.
if($.browser.msie && $.browser.version == 8){
	if (!Array.prototype.indexOf) {
		Array.prototype.indexOf = function(val,start){
		    var len = this.length;
		    start = (typeof start === 'number')?start:0;
		    if(start>=0){
		    	for (; start < len; start++){
		  	      if (start in this && this[start] === val) return start;
		  	    }
		    }else{
		    	start = len;
		    	for (; start >= 0; start--){
		  	      if (start in this && this[start] === val) return start;
		  	    }
		    }
		    return -1;
		  };
	}
}

//widget class/course/tp detail page table title controler
function class_course_tp_title_controler_for_widget(title){
	var widget_class_title = titleRestrictionFadeoutImage(title,'widget-class-title-default-value');
	var iwidth = Math.max(document.documentElement.clientWidth, window.innerWidth || 0);
	return widget_class_title;
}

function scrollBar_Refresh(page){
	if(typeof(page)==='undefined') page = '';
	if(page == 'learning'){
		$('#my-team-dialog #gview_tbl-paintCatalogContentResults').css('height','662px'); // Default Height of the change class popup in my learning & programs
	}else if(page == 'catalog'){
		$('#select-class-dialog #gview_tbl-paintCatalogContentResults').css('height','780px'); // Default Height of the Select class popup in catalog
	}else if(page == 'tp-select-class'){
	    $('.lnr-tainingplan-register').css('height','500px');	    
		$('.lnr-tainingplan-register').jScrollPane().data().jsp.reinitialise();
		$("body").data("learningcore").refreshJScrollPane(".lnr-tainingplan-register");
		//return;
	}else{
		$('#my-team-dialog #gview_tbl-paintCatalogContentResults').css('height','600px'); // Default Height of the My team assign popup
	}
	var Resizedheight = $('#tbl-paintCatalogContentResults').height(); // Calculate Height

	if(page != 'catalog'){ // For learning and My team  and prerequisite pages
		if(page == 'prerequisite'){
			var preResizedheight = $('#gview_lnr-prerequisite #lnr-prerequisite').height(); // Calculate Height
			var PredefaultSize = $('#gbox_lnr-prerequisite .ui-jqgrid-view').height();
			if (PredefaultSize > preResizedheight) { // Resize the grid
				preResizedheight = (preResizedheight < 300) ? 300 : preResizedheight;
				$("#gview_lnr-prerequisite ").css("height", (preResizedheight + 5) + "px");
				$("#gview_lnr-prerequisite .jspContainer").css("height", (preResizedheight + 5) + "px");
			}
			if ($('#PrerequisteErrorMsg').length) {
				$("#gview_lnr-prerequisite ").css("height", (150) + "px");
				$("#gview_lnr-prerequisite .jspContainer").css("height", (150) + "px");
			}
			$("#gbox_lnr-prerequisite .jspContainer").jScrollPane();
		}else if(page == 'tp-select-class'){
		var Resizedheight = $('.lnr-tainingplan-register .enroll-show-morecourse').height(); // Calculate Height
		var defaultSize = $('.lnr-tainingplan-register').height();
			if (defaultSize > Resizedheight) { // Resize the grid
				$(".lnr-tainingplan-register").css("height", (Resizedheight + 3) + "px");
				$(".lnr-tainingplan-register .jspContainer").css("height", (Resizedheight + 3) + "px");
			}
		}else{
			var defaultSize = $('#my-team-dialog .ui-jqgrid-view').height();
			if (defaultSize > Resizedheight) { // Resize the grid
				$("#my-team-dialog #gview_tbl-paintCatalogContentResults ").css("height", (Resizedheight + 3) + "px");
				$("#my-team-dialog #gview_tbl-paintCatalogContentResults .jspContainer").css("height", (Resizedheight + 3) + "px");
			}
			$("#my-team-dialog #gview_tbl-paintCatalogContentResults").jScrollPane();
		}

	}else{
		var defaultSize = $('#select-class-dialog .ui-jqgrid-view ').height();
		if (defaultSize > Resizedheight) { // Resize the grid
			$("#select-class-dialog #gview_tbl-paintCatalogContentResults ").css("height", (Resizedheight + 20) + "px");
			$("#select-class-dialog #gview_tbl-paintCatalogContentResults .jspContainer").css("height", (Resizedheight + 20) + "px");
		}
		$("#select-class-dialog #gview_tbl-paintCatalogContentResults").jScrollPane();
	}
}

//after login auto register or add to cart related work start
$(function() {
	try{
		$("#user-login .learner-sign-close-link").live('click',function(){
			var user_selected_class_id = $.cookie("user_selected_class_id");
			 if(user_selected_class_id != null && user_selected_class_id !=undefined){
				 $.cookie("user_selected_class_id",'',{expires: -300});
				 $.cookie("user_selected_url", '',{ expires: -300 });
			 }
			 var user_selected_page_number = $.cookie("user_selected_page_number");
			 if(user_selected_page_number != null && user_selected_page_number !=undefined)
				 $.cookie("user_selected_page_number",'',{expires: -300});
			 var user_selected_row_number = $.cookie("user_selected_row_number");
			 if(user_selected_row_number != null && user_selected_row_number !=undefined)
				$.cookie("user_selected_row_number",'',{expires: -300});
		});
		$(".ctool-login-modal .popups-close").live('hover',function(){
			$('form').each( function(){
				if($(this).attr('id') == 'user-login'){
					$(".ctool-login-modal .popups-close a").each(function(){
					  this.setAttribute('onclick','clearCookie();');
					});
				}
			});
		});
	}catch(e){
		 // to do
	 }
});
function clearCookie(){
	var user_selected_class_id = $.cookie("user_selected_class_id");
	if(user_selected_class_id != null && user_selected_class_id !=undefined){
		 $.cookie("user_selected_class_id",'',{expires: -300});
		 $.cookie("user_selected_url", '',{ expires: -300 });
	}
	var user_selected_page_number = $.cookie("user_selected_page_number");
	if(user_selected_page_number != null && user_selected_page_number !=undefined)
		 $.cookie("user_selected_page_number",'',{expires: -300});
	var user_selected_row_number = $.cookie("user_selected_row_number");
	if(user_selected_row_number != null && user_selected_row_number !=undefined)
		 $.cookie("user_selected_row_number",'',{expires: -300});
}
//after login auto register or add to cart related work End
function HtmlEncode(s)
{
  var el = document.createElement("div");
  el.innerText = el.textContent = s;
  s = el.innerHTML;
  return s;
}

//To replace some special characters into html entities in javascript
function htmlEntities(str) {
    return String(str).replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;').replace(/"/g, '&quot;').replace(/'/g, '&#039;'); //54112
}

//after login auto register or add to cart related work End
$(function() {
	try{
	$('.enroll-launch-more').live("click",function(){
	var chkOverflow=$('#enroll-lp-result-container .enroll-action, #enroll-result-container .enroll-action').css("overflow");
    if(chkOverflow=="hidden" || chkOverflow=="visible" ){
    	$('#enroll-lp-result-container .enroll-action, #enroll-result-container .enroll-action').css("overflow","");
    }
	});
	}catch(e){
		 // to do
	}
});

function generateMyLearningActionList(actionList, actionKey) {
	  var listStr = '';
	  //console.log('actionKey: ' + actionKey);
	  for (var key in actionList) {
		if (key != actionKey) {
			listStr += '<li class="action-enable">' + actionList[key] + '</li>';
		}
	  }
	  return listStr;
}
function calRefreshAftClose() {
	if ($("#calendarprimaryview").size()) {
		calendarallpopupclose();
		getCalendarData();
		if ($.cookie != undefined) {
			$.cookie("delivery_type", "");
        	$.cookie("startdate", "");
		}
	}
}

function adminCalSuffix()
{
	if($.cookie != undefined && $.cookie("startdate") != null && $.cookie("startdate") != '')
		$("#start_date").val($.cookie("startdate"));//, moment(date).format("MM-DD-YYYY"));

	if($.cookie != undefined && $.cookie("delivery_type") != null && $.cookie("delivery_type") != "null" && $.cookie("delivery_type") != "") 
	{
		$("#delivery_type").val($.cookie("delivery_type"));
	}
	if($.cookie != undefined && $.cookie("delivery_type") == "lrn_cls_dty_vcl")
	{
		$(".addedit-edit-new_location").addClass("addedit-readonly-textfield");
		$(".addedit-edit-new_location").attr("disabled","disabled");
		$(".addedit-edit-new_location").attr("value","");
		$("#edit-max-seats").focus();
		
		
	}
}
/**
	function to dispose all/specific videojs player(s)
**/
function disposeVideoJSPlayer(playerId) {
  if(videojs.getPlayers()[playerId] !== undefined && videojs.getPlayers()[playerId] !== null) {
    videojs(playerId).dispose();
  } else if (playerId == 'all') {
	$.each(videojs.getPlayers(), function(index, player){
		if(player !== undefined && player != null) {
			videojs(index).dispose();
		}
	});
  }
 saveInterval = (typeof saveInterval == 'undefined' ? null : saveInterval);
 ajaxInterval = (typeof ajaxInterval == 'undefined' ? null : ajaxInterval);
 updateStatusToDB = (typeof updateStatusToDB == 'undefined' ? null : updateStatusToDB);
 videoTrackerProgress = null;
 clearInterval(saveInterval);
 clearInterval(ajaxInterval);
 clearInterval(updateStatusToDB);
}
function saniztizeJSData(str) {
	if (str == '' || str === null) {
		return '';
	}
	return htmlEntities(decodeURIComponent(str));
}
function openAttachmentCommon(url){
	try{
		var pattern = /^((http|https|ftp):\/\/)/;
		if(!pattern.test(url)) {		
			url = prepareFileURL(url);
		}	
	var woption = "width=800,height=900,toolbar=no,location=yes,status=yes,menubar=no,scrollbars=yes,resizable=1";
	window.open(url, "_blank", woption);
	}catch(e){
		 // to do
	 }
}
function prepareFileURL(url){
	if(url=='')
		return '';
	url =decodeURIComponent(url);
	var url_arr = url.split('/');
	var url_len = url_arr.length;
	var old_file = url_arr[url_len-1];
	var new_file = encodeURIComponent(url_arr[url_len-1]);
	url = url.replace(old_file, new_file);
	return url;
}

function addslashesForReport(string) {
	try{
        return string.replace(/\\/g, '\\\\').replace(/</g, '&lt;').replace(/>/g, '&gt;').
        replace(/'/g, '\\\'').
        replace(/"/g, '&quot;'); 
  	}catch(e){
 		//Nothing to do
	}
}


function qtip_popup_paint(session_id,sessionDate,multisession,label, pageFrom){
	try {
	//	console.log(session_id);
		var html = '';
		var label = (label == 1 ? '(Location TZ)' : '(TZ)');	
		var width = (multisession > 1 ? '370' : '304');     
	    var  session_id = session_id+"-"+Math.floor(Math.random() * 1000000);
		var $qtipOptClassObj = {
			'entityId': session_id,
			'popupDispId' : 'location-session-details_'+session_id,
			'wid': width,
			'heg' : 60,
			'postype' : 'bottomright',
			'linkid' : 'manage-location-time-'+session_id,
			'dispDown' : 'Y'
		}
		
		if(pageFrom == 'mylearning-calendar') {
			$qtipOptClassObj.wid = 214;
			$qtipOptClassObj.top = -14;
			$qtipOptClassObj.heg = 57;
			$qtipOptClassObj.postype = 'bottomleft';
			$qtipOptClassObj.dispDown = 'N';
		}
		var stringified = JSON.stringify($qtipOptClassObj).replace(/"/g, '\'');
		
		html += '<div id="location-session-details_'+session_id+'" class="location-session-detail line-item-container">';
		html += '<span><a id="manage-location-time-'+session_id+'" class="manage-location-time" onclick="locationdetails('+stringified+');" href="javascript:void(0);">'+label+'</a></span>';
		html += '<span id="selSessionId-'+session_id+'" style="display: none;">'+sessionDate+'</span>';
		html += '<span style=" position:absolute; left:0px; top:0px;" class="qtip-popup-visible" id="visible-popup-'+session_id+'"></span>';
		html += '</div>';
		html += '<span id="visible-popup-'+session_id+'" class="qtip-popup-visible" style="display:none; position:absolute; left:0px; top:0px;"></span>';
		//html += '</div>';

		return html;

	} catch (e) {
		// TODO: handle exception
	window.console.log(e, e.stack);
	}
}
function enableLogin(e) {
    if (e.keyCode == 13) {
    	$('#my-login-container').find('form').submit();
 	    return false;  
    }
}
function checkEncodedState(data) {
try{
	if (encodeURIComponent(decodeURIComponent(data)) === data) {
		//console.log('$data is urlencoded');
		return true;
	} else {
		//console.log('$data is NOT urlencoded');
		return false;
	}
	}catch (e) {
       // window.console.log(e, e.stack);
       return false;
    }
}

function afterRenderRefineIcon(selector) {
    try {
        if (selector !== undefined) {
            var refineSelector = $(selector).find('.criteria-refine-icon');
        } else {
            var refineSelector = $('.criteria-refine-icon');
        }
        refineSelector.each(function() {
            //window.console.log($(this).find('div.refine-text'), $(this).find('div.refine-text .refine-text-span'));
            //window.console.log("'div.refine-text').width()", $(this).find('div.refine-text').width(), "('div.refine-text .refine-text-span').width()", $(this).find('div.refine-text .refine-text-span').width());
            if (($(this).find('div.refine-text').width() >= $(this).find('div.refine-text .refine-text-span').width()) && $(this).find('div.refine-text .refine-text-span').width() != 0) {
                $(this).find('div.refine-text .refine-text-fade').remove();
            }
        });
    } catch (e) {
        window.console.log(e, e.stack);
    }
}

function progressBarRound(dataId, value1, section,type,obj) {
	try {
		if(typeof obj == 'undefined'){
			obj = this;
		}
		var elementId = type + dataId;
		var strokeVal = 7;
		// If browser is IE then set value as 6
		if (navigator.userAgent.indexOf('MSIE') !== -1 || navigator.appVersion.indexOf('Trident/') != -1 || navigator.userAgent.indexOf('Edge/') != -1) {
			strokeVal = 6;   
		}
		if (!$('#'+elementId).hasClass('progress-updated')) {
    		setTimeout(function(){
    			if(typeof obj.bar == 'undefined' || typeof obj.bar[elementId] == 'undefined') {
    				var bar = new ProgressBar.Circle('#'+elementId, {
					  color: '#505050',
					 
					  // This has to be the same size as the maximum width to
					  // prevent clipping
					  strokeWidth: strokeVal,
					  trailWidth: strokeVal,
					  easing: 'easeInOut',
					  duration: 1400,
					  text: {
					    autoStyleContainer: false
					  },
					  from: { color: '#86D752', width: strokeVal},
					  to: { color: '#86D752', width: strokeVal},
					  // Set default step function for all animate calls
					  step: function(state, circle) {
					    circle.path.setAttribute('stroke', state.color);
					    circle.path.setAttribute('stroke-width', state.width);

					    var value = Math.round(value1 * 100);
					      circle.setText(value+"%");
					  }
					});
					if(typeof obj.bar == 'undefined') {
						obj.bar = [];
					}
    				obj.bar[elementId] = bar;
        			if (section == 'lp_class_progress') {
        				bar.text.style.fontSize = '9px';
        				bar.text.style.marginTop = '1px';
        			} else {
        				bar.text.style.fontSize = '0.8rem';
        			}
					bar.text.style.fontFamily = 'Ariel,,"Raleway", Helvetica, sans-serif';
					bar.animate(value1);
    			} else {
    				obj.bar[elementId].destroy();
    				var bar = new ProgressBar.Circle('#'+elementId, {
  					  color: '#505050',
  					 
  					  // This has to be the same size as the maximum width to
  					  // prevent clipping
  					  strokeWidth: strokeVal,
  					  trailWidth: strokeVal,
  					  easing: 'easeInOut',
  					  duration: 1400,
  					  text: {
  					    autoStyleContainer: false
  					  },
  					  from: { color: '#86D752', width: strokeVal},
  					  to: { color: '#86D752', width: strokeVal},
  					  // Set default step function for all animate calls
  					  step: function(state, circle) {
  					    circle.path.setAttribute('stroke', state.color);
  					    circle.path.setAttribute('stroke-width', state.width);
  					    var value = Math.round(value1 * 100);
					      circle.setText(value+"%");
  					  }
  					});
      				obj.bar[elementId] = bar;
          			if (section == 'lp_class_progress') {
          				bar.text.style.fontSize = '9px';
          				bar.text.style.marginTop = '1px';
          			} else {
          				bar.text.style.fontSize = '0.8rem';
          			}
  					bar.text.style.fontFamily = 'Ariel,,"Raleway", Helvetica, sans-serif';
  					bar.animate(value1);
    			}
			},100);
		}	
	} catch(e) {
		// console.log(e);
	}
}



function resetFadeOutByClass(selector,pcont,lcont,fromPage){
	try{
		// Selector required if page has multiple line item widgets 
		selector = (selector == null || selector == undefined) ? '' : selector; 
		pcont = (pcont == null || pcont == undefined) ? 'content-detail-code' : pcont; // Class of the parent level container
		lcont = (lcont == null || lcont == undefined) ? 'line-item-container' : lcont; // Class of each attribute container
		var lineWidth = $(selector + " ."+pcont).width() - 10; // Total width of the entire attribute line
		$(selector + ' .'+pcont).each(function(){ // Loop start for the parent level 
			var cnt = $(this).find("."+lcont).size(); // Number of line items
			//var parent = this;
			var data = [];
			if(cnt > 1){ // Line has more than one item
				// Calculate container total width with subtract the space of the pipe separator
				var contWidth = lineWidth - ((cnt - 1) * 10);
				// Calculate applicable width allocation for each item
				var avgWidth = Math.ceil(contWidth/cnt);
				var sWidth = contWidth; // Set total width of the container into the sharable width variable
				$(this).find('.'+lcont).not('.fadeout-applied').each(function(){ // Start loop for each items for calculation
					// Max with of the attribute
					if($(this).find('.fade-out-title-container').size() > 0){
						var mWidth = $(this).find('.fade-out-title-container').css('max-width').replace('px','');
						// Actual text width
						var aWidth = $(this).find('.title-lengthy-text').width();
						// Check for fade out required or not
						var fadeReq = (aWidth > avgWidth) ? 1 : 0;
						if(fadeReq == 0){
							// subtract the total container width with the actual text width if fade out not required
							// for identify the sharable width
							sWidth -= aWidth; 
							// Subtract the item count if fade out not required
							cnt--;
						}
						var tmp = {
						           'maxWidth':mWidth,
						           'textWidth':aWidth,
						           'avgSpace':avgWidth,
						           'fade':fadeReq
						};
					}else{
						var aWidth = $(this).width();
						var tmp = {
						           'maxWidth':0,
						           'textWidth':aWidth,
						           'avgSpace':0,
						           'fade':0
						};
						sWidth -= aWidth; 
						cnt--;
					}
					
					data.push(tmp);
				}); // End of line item loop
				//console.log(data);
				contWidth = sWidth; // reset container width with actual sharable width
				var idx = 0;
				$(this).find('.'+lcont).not('.fadeout-applied').each(function(){ // Start loop for each line item for set width
					var removeFade = false;
					if(data[idx].fade == 0){
						if($(this).find('.fade-out-title-container').size() > 0){
							$(this).find('.fade-out-title-container').css('max-width',data[idx].textWidth+"px");
							removeFade = true;
						}
					}else{
						if($(this).find('.location-session-detail').size() > 0){
							// Subtract width of the timezone link
							contWidth -= $(this).find('.location-session-detail').width() + 5;
						}
						//console.log("contewidht "+contWidth+" / "+cnt);
						
						// Recalculate the available space for each item based on the sharable space
						avgWidth = Math.ceil(contWidth/cnt); 
						mWidth = data[idx].maxWidth;
						aWidth = data[idx].textWidth;
						var setWidth = ''; 
						//console.log('aw - '+aWidth+" avw - "+avgWidth + " mw - "+mWidth);
						if(aWidth >= avgWidth){ // if actual text width grater than the available space for each item
							setWidth = avgWidth;
							contWidth -= avgWidth; 
						}else{
							setWidth = aWidth;
							contWidth -= aWidth; 
							removeFade = true;
						}
						
						$(this).find('.fade-out-title-container').css('max-width',setWidth+"px");
						cnt--; 
					}
					idx++;
					// Remove fade out image if not required
					if(removeFade == true) {
						$(this).find('.fade-out-image').remove();
					};
					$(this).addClass('fadeout-applied');
				});
			}else{
				var aWidth = $(this).find('.title-lengthy-text').width();
				if(aWidth <= lineWidth){
					$(this).find('.fade-out-title-container').css('max-width',aWidth+"px");
					$(this).find('.fade-out-image').remove();
				}else{
					$(this).find('.fade-out-title-container').css('max-width',lineWidth+"px");
				}
				$(this).addClass('fadeout-applied');
			}
		});
	}catch(e){
		//console.log(e);
	}
}
/* Need to look at this function later for session-width which might change when the function is called from any page other than My Learning */
function resetFadeOutForAttributes(selector,pcont,lcont,fcont,fromtab){
	try{
		var width = new Array();
		var finalWidth;
		var maxwidth;
		var lineWidth = $(selector).width();
		if(fromtab == 'myenrollment' || fromtab == 'myinstructor')
			lineWidth = lineWidth - 10;
		else if(fromtab == 'myprogramclass' || fromtab == 'myprogramtp')
			 lineWidth = lineWidth - 80;
		$(selector + ' .'+pcont).each(function(){  //Process through each main-item-container
			$(this).find("."+lcont).each(function(){  // Process through each line-item-container
					var awidth  = 	$(this).find("."+fcont).width(); // Width of each container
					if(awidth != null){
						width.push(awidth);
	    			}
	    		maxwidth = Math.max.apply(null,width);
	    		width = [];
	    	});
	    		var session_width = $(this).find('.session-container').width(); // check whether session exist
	    		if(session_width == null)
	    			 finalWidth = lineWidth - maxwidth;
	    		else 
	    			finalWidth = lineWidth - maxwidth - session_width ;
	    	
	    	$(this).find('.fade-out-title-container').css('max-width',finalWidth+"px");
	    });
 }catch(e){
		//console.log(e);
	}
}

//h5pcustomization - 
$(document).ready(function()
{
	try
	{
		if(availableFunctionalities.exp_sp_administration_contentauthor != undefined && availableFunctionalities.exp_sp_administration_contentauthor == "exp_sp_administration_contentauthor" )
		{
			if(window.location.href.indexOf("?q=learning/class-details") > 0 || window.location.href.indexOf("?q=learning/course-details") > 0 || window.location.href.indexOf("?q=learning/learning-plan-details") > 0|| window.location.href.indexOf("?q=learning/catalog-search") > 0 || window.location.href.indexOf("?q=learning/enrollment-search") > 0)
			{
				preLoadH5PFiles();
			}
		}
	}catch(e){}
});
function preLoadH5PFiles()
{
$.ajax({
  type: "GET",
  url: "/h5pmerge/embedpresentjs.js",
  dataType: "text",
  async:true,
  success:function(data){
	var pre=document.createElement('pre');
	pre.style ="display:none;"
	pre.value=data;
	pre.id="previewpresentationjsfiles";
	$("body").append(pre);
	}
	});



$.ajax({
  type: "GET",
  url: "/h5pmerge/embedpresentcss.css",
  dataType: "text",
  async:true,
  success:function(data){
	var pre=document.createElement('pre');
	pre.style ="display:none;"
	pre.value=data;
	pre.id="previewpresentationcssfiles";
	$("body").append(pre);
	}
	});


$.ajax({
  type: "GET",
  url: "/h5pmerge/embedvideojs.js",
  dataType: "text",
  async:true,
  success:function(data){
	var pre=document.createElement('pre');
	pre.style ="display:none;"
	pre.value=data;
	pre.id="previewvideojsfiles";
	$("body").append(pre);
	}
	});



$.ajax({
  type: "GET",
  url: "/h5pmerge/embedvideocss.css",
  dataType: "text",
  async:true,
  success:function(data){
	var pre=document.createElement('pre');
	pre.style ="display:none;"
	pre.value=data;
	pre.id="previewvideocssfiles";
	$("body").append(pre);
	}
	});


}
