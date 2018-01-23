/*with(new menuname("main Tree Menu")){
	//alert("DrawMenu")
top=105
left=2
style = tstyle1;
alwaysvisible = 1;
itemwidth=200
//margin=AllMargin
//position="relative"
//aI("text=Home;");

aI("text=Standard Course;showmenu=StandardCourse;type=tree;url=javascript:mainHeading(1);");
aI("text=Blended Course;showmenu=BlendedCourse;type=tree;url=javascript:mainHeading(2);");
aI("text=Class;showmenu=Class;type=tree;url=javascript:mainHeading(3);");
//aI("text=Editing a Course/Class;showmenu=EditingClass;type=tree;url=javascript:mainHeading(5);");
//aI("text=Cancelling a Class (ILT/VC);url=javascript:mainHeading(6);");
//aI("text=Generating Roster Report;url=javascript:mainHeading(7);");	
}

with(new menuname("StandardCourse")){
	style = sub1Style;
	margin=AllMargin
	aI("text=&nbsp;&nbsp;&nbsp;&nbsp;Search for a Course;url=javascript:showPage(0,1);");
	aI("text=&nbsp;&nbsp;&nbsp;&nbsp;Create a Course;url=javascript:showPage(1,2);");
	aI("text=&nbsp;&nbsp;&nbsp;&nbsp;Edit a Course;url=javascript:showPage(4,3);");
}
with(new menuname("Class")){
		style = sub1Style;
		margin=AllMargin
		aI("text=&nbsp;&nbsp;&nbsp;&nbsp;Search	for a Class;url=javascript:loadSubPage(8);");		
		aI("text=&nbsp;&nbsp;&nbsp;&nbsp;Create Class;showmenu=CreateClass;type=tree;url=javascript:loadSubPage(10);");
		aI("text=&nbsp;&nbsp;&nbsp;&nbsp;Edit Class;showmenu=EditClass;type=tree;url=javascript:loadSubPage(10);");
		aI("text=&nbsp;&nbsp;&nbsp;&nbsp;Cancel Class (ILT/VC);url=javascript:loadSubPage(10);");
		aI("text=&nbsp;&nbsp;&nbsp;&nbsp;Registrar Activities;showmenu=RegistrarActivities;type=tree;url=javascript:loadSubPage(10);");
	}


with(new menuname("BlendedCourse")){
	style = sub1Style;
	margin=AllMargin
	aI("text=&nbsp;&nbsp;&nbsp;&nbsp;Search for a Blended Course;url=javascript:loadSubPage(1);");
	aI("text=&nbsp;&nbsp;&nbsp;&nbsp;Create a Blended Course;url=javascript:loadSubPage(8);");
	aI("text=&nbsp;&nbsp;&nbsp;&nbsp;Edit a Blended Course;url=javascript:loadSubPage(8);");
}    

  	with(new menuname("CreateClass")){
		style = sub2Style;
		margin=AllMargin
		aI("text=&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ILT;url=javascript:loadPage(11,10);");
		aI("text=&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Virtual Class;url=javascript:loadPage(12,10);");
		aI("text=&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Online;url=javascript:loadPage(13,10);");	
	}
	
	with(new menuname("EditClass")){
		style = sub2Style;
		margin=AllMargin
		aI("text=&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ILT;url=javascript:loadPage(11,10);");
		aI("text=&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Virtual Class;url=javascript:loadPage(12,10);");
		aI("text=&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Online;url=javascript:loadPage(13,10);");	
	}
	
	
	with(new menuname("RegistrarActivities")){
		style = sub2Style;
		margin=AllMargin
		aI("text=&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Register a Learner;url=javascript:loadPage(11,10);");
		aI("text=&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Cancel a Learner;url=javascript:loadPage(12,10);");
		aI("text=&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Completion Marking;url=javascript:loadPage(13,10);");		
	}    

//drawMenus()*/



/// Navigation

var myFnames=new Array();
var subFEditnames= new Array();
// configurecourseobject Edit File Names

subFEditnames[0]="EditReOccuranceSetting"; 	 
subFEditnames[1]="Edit_BusinessRules";
subFEditnames[2]="Edit_Notification";
subFEditnames[3]="Edit_Owners"; 
subFEditnames[4]="Edit_AccessProfile"; 
subFEditnames[5]="EditPricing"
subFEditnames[6]="Edit_Pre_Requisite"
subFEditnames[7]="EditEquivalance";
subFEditnames[8]="EditForumTopic"; 
subFEditnames[9]="Edit_Folksonomy"; 
subFEditnames[10]="EditSuggestedReading"; 			

var subFnames= new Array();
// configurecourseobject Others File Names

subFnames[0]="define_re_occurance_setting"; 	 
subFnames[1]="enable_disable_businessrules";
subFnames[2]="enable_disable_notification";
subFnames[3]="addowners"; 
subFnames[4]="defineAudience_accessProfile"; 		
subFnames[5]="addPreRequisite"
subFnames[6]="addEquivalance";
subFnames[7]="addForumTopics"; 
subFnames[8]="addUserTagsFolksonomy"; 
subFnames[9]="addSuggestedReading"; 		
subFnames[10]="definePricing";

var m_id=1;
var path;
var sub_id;
var spath;
//File Names
function mainHeading(m_id){	
   sub_id=m_id;
 if (m_id == 1){		
	path="standardcourse/"
	myFnames[0]="SearchCourse";	
	myFnames[1]="CreateCourse"; 		
	myFnames[2]="EditCourse";	
	 $("#divimg").load(path+myFnames[0]+".html"); 	
 }else if(m_id==2){	
		path="blendedcourse/"
		myFnames[0]="BlendedSearchCourse";	
		myFnames[1]="BlendedCreateCourse"; 		
		myFnames[2]="BlendedEditCourse";			
	 $("#divimg").load(path+myFnames[0]+".html"); 		
 }else if(m_id==3){	    
		path="class/"
		myFnames[0]="SearchClass"; 
		//Create ILT
		myFnames[1]="creating_new_ilt_class"; 
		myFnames[2]="creating_existing_ilt_class";
		 // Create New Create Course	
		myFnames[3]="CreateCourse";
		myFnames[4]="CreateCourse_Procedure1";
		myFnames[5]="CreateCourse_Procedure2"; 
		myFnames[6]="CreateCourse_Procedure3";
		myFnames[7]="CreateILTClass"
		myFnames[8]="CreateILTClass_procedure1";
		myFnames[9]="CreateILTClass_procedure2";
		//Create ILT  for Exisiting Course
		myFnames[10]="CourseValidation"; 
		myFnames[11]="CourseValidation_Procedure1"; 		
		myFnames[12]="ValidateCreateILTClass";
		//Create ILT  for Exisiting Course
		myFnames[13]="ValidateCreateILTClass_procedure1";
		myFnames[14]="ValidateCreateILTClass_procedure2";
		myFnames[15]="ValidateCreateILTClass_procedure3";	
		
		//Create Virtual
		myFnames[16]="creating_new_virtual_class"; 
		myFnames[17]="creating_existing_virtual_class";
		 // Create New Virtual	
		myFnames[18]="CreateCourse";
		myFnames[19]="CreateCourse_Procedure1";
		myFnames[20]="CreateCourse_Procedure2"; 
		myFnames[21]="CreateCourse_Procedure3";
		myFnames[22]="CreateVC"
		myFnames[23]="CreateVirtualClass_Procedure1";
		myFnames[24]="CreateVirtualClass_Procedure2";
		//Create Virtual New Exisiting Course
		myFnames[25]="CourseValidation"; 
		myFnames[26]="CourseValidation_Procedure1"; 
		myFnames[26]="CourseValidation_Procedure2"; 
		myFnames[27]="ValidateCreateVC";
		//Create Virtual  for Exisiting Course
		myFnames[28]="ValidateCreateVC_procedure1";
		myFnames[29]="ValidateCreateVC_procedure2";
		myFnames[30]="ValidateCreateVC_procedure3";	
		
		
		 $("#divimg").load(path+myFnames[0]+".html");		
	}
	 
}

// PopUP Center Function

function popUp(a_str_windowURL, a_str_windowName, a_int_windowWidth, a_int_windowHeight, a_bool_scrollbars,a_bool_resizable) {
  var int_windowLeft = (screen.width - a_int_windowWidth) / 2;
  var int_windowTop = (screen.height - a_int_windowHeight) / 2;
  var str_windowProperties = 'height=' + a_int_windowHeight + ',width=' + a_int_windowWidth + ',top=' + int_windowTop + ',left=' + int_windowLeft + ',scrollbars=' + a_bool_scrollbars + ',resizable=' + a_bool_resizable +'';
  var obj_window = window.open(a_str_windowURL, a_str_windowName, str_windowProperties)
    if (parseInt(navigator.appVersion) >= 4) {
      obj_window.window.focus();
    }
}



function popUp1(URL) {	

win2 = window.open(URL, "Window2", "width=800,height=600,toolbar=0,scrollbars=yes"); 

}      

var CurrentPage;
var dual_id=1;
var intPage=true;
var tmr;

function loadPage(id,m_name){
 intPage=false;
 CurrentPage=id;  
 loadSubPage(m_name)
 popUp(path+myFnames[id]+".html",'Window2', 800, 600, 1,0)
}

function showPage(id,m_id){	
mainHeading(m_id)
  // dual_id=s_id;
 //alert("werwerwer")
 // subProcedure();
	 $("#divimg").load(path+myFnames[id]+".html");  	
}

function loadSubPage(id){	
	popUp(path+myFnames[id]+".html",'Window2', 800, 600, 1,0)
 // $("#myDiv1").load(path+myFnames[id]+".html"); 
}
function loadSubPage1(id){
 $("#myDiv1").load(path+myFnames[id]+".html"); 
}

function loadEditSubPage(id){
 $("#divimg").load(path+myFnames[id]+".html"); 
}


function loadEditSubProcedure(id){	
	popUp(spath+subFEditnames[id]+".html",'Window2', 800, 600, 1,0); 
//  $("#myDiv1").load(spath+subFnames[id]+".html"); 

}

function loadSubProcedure(id){	
   popUp(spath+subFnames[id]+".html",'Window2', 800, 600, 1,0);
 // $("#myDiv2").load(spath+subFnames[id]+".html"); 
}



spath="configurecourseobject/others/";	
function subProcedure(){	
   if(dual_id==3){	 
  // alert("edit")
	   spath="configurecourseobject/edit/";		
	}else {
	   spath="configurecourseobject/others/";			
	}
	
}
