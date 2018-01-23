<?php
 
class ExpertusSelectQueryExtender extends SelectQueryExtender {
  protected $index = '';

  public function __construct(SelectQueryInterface $query, DatabaseConnection $connection) {
  	try {
      parent::__construct($query, $connection);

      // Add pager tag. Do this here to ensure that it is always added before
      // preExecute() is called.
      $this->addTag('ExpertusSelectQueryExtender');
      parent::__construct($query, $connection);
  	}
    catch (Exception $ex) {
      watchdog_exception('ExpertusSelectQueryExtender::__construct', $ex);
      expertusErrorThrow($ex);
    }
  }
  
  public function setIndex($index = array()) {
  	try {
      $this->index = $index;
  	}
    catch (Exception $ex) {
      watchdog_exception('ExpertusSelectQueryExtender::index', $ex);
      expertusErrorThrow($ex);
    }
  }

  public function execute() {
  	try {
	    // If validation fails, simply return NULL.
	    // Note that validation routines in preExecute() may throw exceptions instead.
	    if (!$this->preExecute()) {
	      return NULL;
	    }
	
	    $args = $this->query->getArguments();
	    //expDebug::dPrint('$args = ' . print_r($args, true), 4);
	    $query = (string) $this;
	    //expDebug::dPrint('(string) $this = ' . $query, 4);
	    $ret = $this->connection->query($query, $args, array()); // @TODO: orginal query options should be used in third argument
	    
	    return $ret;
	    //return parent::execute(); 
	    //$this->query->execute();
  	}
    catch (Exception $ex) {
      watchdog_exception('ExpertusSelectQueryExtender::execute', $ex);
      expertusErrorThrow($ex);
    }
  }
  
  public function __toString() {
  	try {
	    $query = (string) $this->query;
	    //expDebug::dPrint('$query before index is added = ' . print_r($query, true), 4);
	    $tablesM = $this->query->getTables();
	    $union = $this->query->getUnion();
	    if($union){
	    	$tablesU = $union[0]['query']->getTables();
	    	$tables = array_merge($tablesM,$tablesU);
	    }else{
	    	$tables = $tablesM;
	    }
	//    expDebug::dPrint("INDEX --> ".print_r($this->index,true),4);
	//    expDebug::dPrint("tables --> ".print_r($tables,true),4);
	   
	    $mi = $this->index;
	    expDebug::dPrint('$mi debug'.var_export($mi, 1));
	    if(!empty($mi)) {
	    foreach($mi as $alias=>$index){
	    //foreach($this->index as $alias=>$index){
	    	$pattern = "/{".$tables[$alias]['table']."} ". $tables[$alias]['alias'] . "/";
	    //	expDebug::dPrint('$pattern --> '.$pattern);
	    	$replace = "{".$tables[$alias]['table']."} ". $tables[$alias]['alias'] . "  FORCE INDEX (". $index .")";
	    //	expDebug::dPrint('$$replace --> '.$replace);
	    	$query = preg_replace($pattern,$replace,$query,1);
	    }
	    }
	
	 //   expDebug::dPrint('$query after index is added = ' . print_r($query, true), 4);
	    return $query;
  	}
    catch (Exception $ex) {
      watchdog_exception('ExpertusSelectQueryExtender::__toString', $ex);
      expertusErrorThrow($ex);
    }
  }
	public function countQuery() {
    // Create our new query object that we will mutate into a count query.
    $count = clone($this);
		expDebug::dPrint("test count query");
    $group_by = $count->getGroupBy();

    if (!$count->distinct) {
      // When not executing a distinct query, we can zero-out existing fields
      // and expressions that are not used by a GROUP BY.  Fields listed in
      // the GROUP BY clause need to be present in the query.
      $fields =& $count->getFields();
      foreach (array_keys($fields) as $field) {
        if (empty($group_by[$field])) {
          unset($fields[$field]);
        }
      }
      $expressions =& $count->getExpressions();
      foreach (array_keys($expressions) as $field) {
        if (empty($group_by[$field])) {
          unset($expressions[$field]);
        }
      }

      // Also remove 'all_fields' statements, which are expanded into tablename.*
      // when the query is executed.
      foreach ($count->tables as $alias => &$table) {
        unset($table['all_fields']);
      }
    }

    // If we've just removed all fields from the query, make sure there is at
    // least one so that the query still runs.
    $count->addExpression('1');

    // Ordering a count query is a waste of cycles, and breaks on some
    // databases anyway.
    $orders = &$count->getOrderBy();
    $orders = array();

    if ($count->distinct && !empty($group_by)) {
      // If the query is distinct and contains a GROUP BY, we need to remove the
      // distinct because SQL99 does not support counting on distinct multiple fields.
      $count->distinct = FALSE;
    }

    $query = $this->connection->select($count);
    $query->addExpression('COUNT(*)');

    return $query;
  }
  
  
  public function catalogExecute(){
  	try{
  		
  		if (!$this->preExecute()) {
  			return NULL;
  		}
  		
  		$args = $this->query->getArguments();
  		
  		//expDebug::dPrint("Query string before modified".print_r($args,1),4);
  		$query = (string) $this;
  		
//   		foreach($args as $argAlias=>$argVal){
//   		  $query = preg_replace("/".$argAlias."\b/","'$argVal'",$query);
  			
//   			$query = preg_replace("/[{}]/","",$query);
//   		}
  		$QryStr = explode('SELECT acc.cls_id', $query);
  		$resQry = "SELECT SQL_CALC_FOUND_ROWS acc.cls_id". $QryStr[1];
  		expDebug::dPrintDBAPI("CATALOG SEARCH QUERY :",$resQry, $args);
  		$selectCatQry = db_query($resQry, $args);
  		
  		return $selectCatQry;
  	} catch(Exception $e){
  		watchdog_exception('ExpertusSelectQueryExtender::catalogExecute', $e);
  		expertusErrorThrow($e);
  	}
  }
}

?>