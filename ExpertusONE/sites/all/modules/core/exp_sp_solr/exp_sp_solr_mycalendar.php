<?php
require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solrclient.php';
class MyCalendarSolrSearch extends SolrClient{

    function __construct(){
        // Get collection prefix from configuration
        $prefix = getConfigValue('collection_prefix');

        // Set exact collection name (should match with solr's collection)
        $this->collName = $prefix . '_' . SolrClient::CatalogCore;

        expDebug::dPrint("Core name -- ".$this->collName);

        //Call parent constructor
        parent::__construct();
    }

    public function getUpcomingList($dateRange = '', $mode = ''){
    	global $Solr_User;
        $fieldList = array(
                'classid:EntityId',
                'eventdate:SessionStartDate',
                'title:Title',
                'timezone:TimeZone',
                'startdatetime:SessionStartTime',
                'enddatetime:SessionEndTime',
                'delivery_type:Type',
                'session_timezone:SessionTimeZone',
                'statustext:EntityId', // modifed with EnrollCore
                'session_end_datetime:SessionEnd',
                'myclass:InstructorId',
                'instructor_id:InstructorId',
                'session_count:SessionId'
        );
       
        if($mode == 'day'){           
            $fieldList1 = array(
                    'cdesc:Description',
                    'code:Code',
                    'reg_status:EntityId', // modifed with EnrollCore
                    'comp_status:EntityId', // modifed with EnrollCore
                    'session_timezone_code:SessionTimeZoneCode',
                    'tz_code:TimeZoneCode',
                    'fulldate:SessionStartDate'
            );
            $fieldList = array_merge($fieldList,$fieldList1);
        }
       
        if (empty($Solr_User)) {
            require_once $_SERVER ['DOCUMENT_ROOT'] . '/sites/all/modules/core/exp_sp_solr/exp_sp_solr_user.php';
            $userObj = new UserSolrSearch ();
            $userDetails = $userObj->getLoggedUserDetails ();
        } else {
            $userDetails = $Solr_User;
        }
        $uid = $userDetails[0]->id;
       
        $filter = array();
        // set startdate and enddate
        if($mode == 'day'){
            $startDate     = $dateRange . ' 00:00:00';
            $endDate     = $dateRange . ' 23:59:59';
        }
        else {
            $currmonth = date('Y-m');
            $startDate = ($currmonth == $dateRange) ? date('Y-m-d').' 00:00:00' : $dateRange . '-01 00:00:00';
            $endDate     = date('Y-m-t 23:59:59', strtotime($startDate));
            }
       
        // calculate date range chosen is past or future
        $dateType = 'future';
        if (strtotime($endDate) <= strtotime(date('Y-m-d H:i:s'))) {
                $dateType = 'past';
        }
        expDebug::dPrint("training end date: " . $dateType, 4);
       
        // Fetch enrollments of user within the date range -- fetch classid,regstatus,compstatus
        $class_id = $this->getEnrollments($startDate,$endDate,$uid);
        expDebug::dPrint("Details of enroollmmmeeenttt--- " . print_r($class_id, true),4);
       
        if( $dateType == 'future'){
            $this->setDefaultFilters($filter);
            $this->setAccessFilters($filter,$userDetails);
            $this->setSessionDateRangeFilter($filter,$startDate,$endDate);
        }
        if($dateType == 'past'){
            $this->setFilterForPast($filter,$class_id);
        }
        $results = $this->getCalendarDetails($filter,$fieldList,'GET',$class_id);
           
        return $results;
    }
   
   
    private function setAccessFilters(&$filter,$userDetails){
        $grpStr = '';
        $uid = $userDetails[0]->id;
        $user_hire = $userDetails[0]->HireDate;
        $user_name = $userDetails[0]->username;
        expDebug::dPrint ( 'Learner group for userss' . print_r ( $userDetails, true ), 4 );
        if($userDetails[0]->learner_groups != ''){
            $userAdminGrp = explode(',',$userDetails[0]->learner_groups);
            expDebug::dPrint ( 'Learner group for userss' . print_r ( $userDetails, true ), 4 );
            foreach($userAdminGrp as $key=>$val){
                if($key==0)
                    $grpStr = 'LnrGroupName:"*'.trim(str_replace(' ','?',$val)).'*"';
                else
                    $grpStr .= ' OR LnrGroupName:"*'.trim(str_replace(' ','?',$val)).'*"';
            }
            $grpStr .= ' OR (LnrGroupName:"*Open_Entity*" AND LnrAccessUserName:"*Open_Entity*")';
        }
           
        else
            $grpStr = '(LnrGroupName:"*Open_Entity*" AND LnrAccessUserName:"*Open_Entity*")';
   
        if($user_name)
            $grpStr .= ' OR LnrAccessUserName:"*`'.$user_name.'`*"';
        array_push($filter, urlencode($grpStr));
        if($user_hire){
            $hire_date_filter = $this->setHireDateFilter($user_hire);
            array_push($filter, urlencode($hire_date_filter));
        }
    }
   
    private function setHireDateFilter(&$user_hire){
        $hire_date_filter = '';
        $datetime1 = date_create($user_hire);
        $datetime2 = date_create(now());
        $interval = (date_diff($datetime1, $datetime2)->format("%R%a days"));
        if(strpos($interval,'+') !== false){
            preg_match_all('!\d+!', $interval, $days);
            $days_check = $days[0][0]-1;
            $hire_date_filter = '-HireDays:[1 TO '.$days_check.']';
        }
        return $hire_date_filter;
    }
   
    private function setDefaultFilters(&$filter){
        $entity_type = 'EntityTypeName:Class';
        array_push($filter, urlencode($entity_type));
        // It should have a session
        $session_exits = 'SessionId:*';
        array_push($filter, urlencode($session_exits));
        // Class should be active
        $status = 'Status:lrn_cls_sts_atv';
        array_push($filter, urlencode($status));
    }
   
    private function setSessionDateRangeFilter(&$filter,$startDate,$endDate){
        $startDate = str_replace(' ','T',$startDate);
        $endDate = str_replace(' ','T',$endDate);
        $startDate .='Z';
        $endDate .= 'Z';
        $session = 'SessionStartDate:['.$startDate.' TO '.$endDate.']';
        array_push($filter, urlencode($session));
    }
   
    private function getEnrollments($startDate,$endDate,$uid){
        require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solr_mylearning_enrollment.php';
        $solrObj = new MyLearningEnrollmentSolrSearch();
        $search_results = $solrObj->getEnrollmentsForCalendar($startDate,$endDate,$uid);
        $class_id = $search_results['result'];
        $count = $search_results['count'];
        expDebug::dPrint("Result in Solr Search classiddd-- ".print_r($class_id,true),4);
        return $class_id;
    }
   
    private function setFilterForPast(&$filter,$class_id){
        expDebug::dPrint("Result in Solr Search classiddd-- ".print_r($class_id,true),4);
        if($class_id){
            $entity_type = 'EntityTypeName:Class';
            array_push($filter, urlencode($entity_type));
            foreach($class_id as $key=>$val){
                expDebug::dPrint("Result in Solr Search classiddd val-- ".print_r($val,true),4);
                expDebug::dPrint("Result in Solr Search classiddd val-- ".$val->class_id);
                if($key == 0)
                    $fil = 'EntityId:'.$val->class_id.'';
                else
                    $fil .= ' OR EntityId:'.$val->class_id.'';
                }
        }
        else {
            $fil = '-EntityId:*';
        }
        expDebug::dPrint("Result in Solr Search classiddd val-- ".print_r($fil,true),1);
        array_push($filter, urlencode($fil));
        }
   
    private function getCalendarDetails($filter,$fieldList,$method,$classDetails){
    	global $Solr_User;
        // List of required fields to be set as NULL,
        // because Solr will not keep any null fields
        $baseList = array(
                'classid'=>NULL,
                'eventdate'=>NULL,
                'title'=>NULL,
                'timezone'=>NULL,
                'startdatetime'=>NULL,
                'enddatetime'=>NULL,
                'delivery_type'=>NULL,
                'session_timezone'=>NULL,
                'statustext'=>NULL,
                'session_end_datetime'=>NULL,
                'myclass'=>NULL,
                'instructor_id'=>NULL,
                'session_count'=>NULL,
                'cdesc'=>NULL,
                'code'=>NULL,
                'reg_status'=>NULL,
                'comp_status'=>NULL,
                'session_timezone_code'=>NULL,
                'tz_code'=>NULL,
                'fulldate'=>NULL,
        );
       
   
        // Append field list
        $data = 'fl=' . implode(',',$fieldList);
        // Append filters
        $data .= '&fq=' . implode('&fq=',$filter);
        //Pagination
        $start = 0;
        $rows = 1000;
        // Sort
        $sortby = 'SessionStart+asc';
   
        $data .= '&sort='.$sortby.'&indent=on&q=*:*&start='.$start.'&rows='.$rows.'';
   
        try{
            $srcRst = $this->getData($this->collName,$data,'GET');
            expDebug::dPrint('Solr Out put -- > '.print_r(($srcRst),true),4);
   
            $srcRst1 = $srcRst['response']['docs'];
            expDebug::dPrint("Solr result count ".print_r($srcRst1,1),4);
               
            $srcRst2 = array();
            $srcRst2['count'] = $srcRst['response']['numFound'];

            expDebug::dPrint('Solr Out putenrollments -- > '.print_r(($classDetails),true),4);
            $class_id = array();
            if($classDetails){
                foreach ($classDetails as $key => $val ) {
                    array_push ($class_id, $val->class_id );
                }
            }
            expDebug::dPrint('Solr Out putenrollments123 -- > '.print_r(($class_id),true),4);
        
		        if(empty($Solr_User)){
		            $userdetails = $this->getLoggedUserDetails();
		        }else{
		            $userdetails = $Solr_User;
		        }
            foreach($srcRst1 as $doc){
                $tdoc  = array_merge($baseList, $doc);
                $this->modifyResultSet($tdoc,$class_id,$userdetails);
                $result =     $this->restAddDocsForMultiSession($tdoc);
                foreach($result as $key=>$tdoc){
                    $srcRst2['result'][] = $this->arrayToObject($tdoc);
                }
            }
           
            expDebug::dPrint("Solr search result - Final ".print_r($srcRst2,1),4);
            return $srcRst2;
        }catch(Exception $e){
            expDebug::dPrint("Error in Solr Catalog Search -- ".$e->getMessage(),1);
        }
    }
   
    private function modifyResultSet(&$tdoc,$class_id='',$userdetails){
        expDebug::dPrint('Solr Out putenrollments 321-- > '.print_r(($class_id),true),4);
        $uid = $userdetails[0]->id;
        // Reset status text field
        $this->resetStatusText($tdoc,$class_id);
        // Reset Instructor field
        $this->resetInstructor($tdoc,$uid);
        // Reset session count
        $this->resetSessionCount($tdoc);
    }
   
    public function resetStatusText(&$tdoc,$class_id=''){
        expDebug::dPrint('Solr Out putenrollments 456-- > '.print_r(($class_id),true),4);
        $tdoc['reg_status'] = '';
        $tdoc['comp_status'] = '';
        if($class_id[0] != ''){
            if(in_array($tdoc['statustext'], $class_id)){
                $tdoc['statustext'] = 'Registered';
                $tdoc['reg_status'] = $val->reg_status;
                $tdoc['comp_status'] = $val->comp_status;
            }
            else {
                $tdoc['statustext'] = 'Not Registered';
                }
        }
        else {
                $tdoc['statustext'] = 'Not Registered';
                }
        }
   
    private function resetInstructor(&$tdoc,$uid){
        $inst = $tdoc['myclass'];
        $res = 0;
        foreach($inst as $key=>$val){
            if($uid == $val)
                $res++;
        }
        $tdoc['myclass'] = ($res > 0) ? 'Instructor' : 'No';
        expDebug::dPrint("Error in Solr Catalog Search user-- ".$uid,4);
    }
   
    private function resetEndDateTime(&$tdoc){
       
        // Modify eventdate
        $date = (array)$tdoc['eventdate'];
        $modify = str_replace('T', ' ', $date[0]);
        $modify = str_replace('Z', '', $modify);
        $tdoc['eventdate'] = $modify;
        expDebug::dPrint("Error in Solr Catalog Searchevent date -- ".print_r($date,true),4);
       
        // Modify startdatetime
        $start = (array)$tdoc['startdatetime'];
        $tdoc['startdatetime'] = $start[0];
       
        // Modify enddatetime
        $end = (array)$tdoc['enddatetime'];
        $tdoc['enddatetime'] = $end[0];
       
        // Modify session_end_datetime
        $inst = $tdoc['session_end_datetime'];
        $date = explode(' ',$tdoc['eventdate']);
        expDebug::dPrint("Error in Solr Catalog Searchevent session date explode -- ".print_r($date,true),4);
        $session_end_date = $date[0];
        $session_end_date .= ' ';
        $session_end_date .= $tdoc['enddatetime'];
        $session_end_date .= ':00';
        $tdoc['session_end_datetime'] = $session_end_date;
       
        // Modify fulldate
        if($tdoc['fulldate']){
            $inst = $tdoc['session_end_datetime'];
            $date = explode(' ',$tdoc['eventdate']);
            $session_end_date = $date[0];
            $session_end_date .= ' ';
            $session_end_date .= $tdoc['startdatetime'];
            $session_end_date .= ':00';
            $tdoc['fulldate'] = $session_end_date;
        }
       
    }
   
    private function resetSessionCount(&$tdoc){
        $session_id = (array)$tdoc['session_count'];
        $session_count = count($session_id);
        $tdoc['session_count'] = $session_count;
    }
   
    private function restAddDocsForMultiSession(&$tdoc){
        $count = $tdoc['session_count'];
        $start = (array)$tdoc['startdatetime'];
        $end = (array)$tdoc['enddatetime'];
        $date = (array)$tdoc['eventdate'];
        $sesdocs = array();
   
        for($i=0;$i<$count;$i++){
            $sesdocs[$i] = $tdoc;
            //eventdate
            $sesdocs[$i]['eventdate'] = date("Y-m-d H:i:s", strtotime($date[$i]));
            // startdate and time
            $sesdocs[$i]['startdatetime'] = $start[$i];
            $sesdocs[$i]['enddatetime'] = $end[$i];
            // full date
            $date1 = explode(' ',$sesdocs[$i]['eventdate']);
            $session_end_date = $date1[0];
            $session_end_date .= ' ';
            $session_end_date .= $sesdocs[$i]['startdatetime'];
            $session_end_date .= ':00';
            $sesdocs[$i]['fulldate'] = $session_end_date;
           
            // session_end_datetime
           
           
            $session_end_date1 = $date1[0];
            $session_end_date1 .= ' ';
            $session_end_date1 .= $sesdocs[$i]['enddatetime'];
            $session_end_date1 .= ':00';
            $sesdocs[$i]['session_end_datetime'] = $session_end_date1;
        }
       
       
        expDebug::dPrint("Error in Solr Catalog Searchevent session date explode sesdocs-- ".print_r($sesdocs,true),4);
        expDebug::dPrint("Error in Solr Catalog Searchevent session date explode tdocsss-- ".print_r($tdoc,true),4);
        return $sesdocs;
        expDebug::dPrint("Error in Solr Catalog Searchevent session date explode tdocsss after -- ".print_r($tdoc,true),4);
   
    }

}
?>