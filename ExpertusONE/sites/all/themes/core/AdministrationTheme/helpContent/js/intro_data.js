// Intro Navigation


var myFnames=new Array();
var subFnames= new Array();
var m_id=1;
var path;
var sub_id;
var spath;
var CurrentPage;
var intPage=true;
var tmr;

//File Names
path="introduction/"
myFnames[0]="PortalArchitecture"; 	
myFnames[1]="FeaturesAndBenefits";	

function loadPage(id){
$("#divimg").load(path+myFnames[id]+".html"); 

}



// Popup Center

function popUp(a_str_windowURL, a_str_windowName, a_int_windowWidth, a_int_windowHeight, a_bool_scrollbars) {
  var int_windowLeft = (screen.width - a_int_windowWidth) / 2;
  var int_windowTop = (screen.height - a_int_windowHeight) / 2;
  var str_windowProperties = 'height=' + a_int_windowHeight + ',width=' + a_int_windowWidth + ',top=' + int_windowTop + ',left=' + int_windowLeft + ',scrollbars=' + a_bool_scrollbars + '';
  var obj_window = window.open(a_str_windowURL, a_str_windowName, str_windowProperties)
    if (parseInt(navigator.appVersion) >= 4) {
      obj_window.window.focus();
    }
}


