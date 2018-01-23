<?php

function ConvertScorm2004TimeToMS($strIso8601Time) {
	
	$intTotalMs = 0;
	
	$Seconds = 0; // 100 hundreths of a seconds
	$Minutes = 0; // 60 seconds
	$Hours = 0; // 60 minutes
	$Days = 0; // 24 hours
	$Months = 0; // assumed to be an "average" month (figures a leap year every 4 years) = ((365*4) + 1) / 48 days - 30.4375 days per month
	$Years = 0; // assumed to be 12 "average" months
	
	$MILLISECONDS_PER_SECOND = 1000;
	$MILLISECONDS_PER_MINUTE = $MILLISECONDS_PER_SECOND * 60;
	$MILLISECONDS_PER_HOUR = $MILLISECONDS_PER_MINUTE * 60;
	$MILLISECONDS_PER_DAY = $MILLISECONDS_PER_HOUR * 24;
	$MILLISECONDS_PER_MONTH = $MILLISECONDS_PER_DAY * (((365 * 4) + 1) / 48);
	$MILLISECONDS_PER_YEAR = $MILLISECONDS_PER_MONTH * 12;
	
	// strIso8601Time = new String(strIso8601Time);
	
	$strNumberBuilder = "";
	$strCurrentCharacter = "";
	$blnInTimeSection = false;
	
	// start at 1 to get past the "P"
	for($i = 1; $i < strlen($strIso8601Time); $i ++) {
		
		$strCurrentCharacter = $strIso8601Time[$i];
		
		if(IsIso8601SectionDelimiter($strCurrentCharacter)) {
			
			switch(strtoupper($strCurrentCharacter)) {
				
				case "Y":
					$Years = intval($strNumberBuilder, 10);
					break;
				
				case "M":
					if($blnInTimeSection) {
						$Minutes = intval($strNumberBuilder, 10);
					} else {
						$Months = intval($strNumberBuilder, 10);
					}
					break;
				
				case "D":
					$Days = intval($strNumberBuilder, 10);
					break;
				
				case "H":
					$Hours = intval($strNumberBuilder, 10);
					break;
				
				case "S":
					$Seconds = floatval($strNumberBuilder);
					break;
				
				case "T":
					$blnInTimeSection = true;
					break;
			}
			
			$strNumberBuilder = "";
		} else {
			$strNumberBuilder .= "" . $strCurrentCharacter; // use "" to keep the number as string concats instead of numeric additions
		}
	}
	
	
	$intTotalMs = ($Years * $MILLISECONDS_PER_YEAR) + ($Months * $MILLISECONDS_PER_MONTH) + ($Days * $MILLISECONDS_PER_DAY) + ($Hours * $MILLISECONDS_PER_HOUR) + ($Minutes * $MILLISECONDS_PER_MINUTE) + ($Seconds * $MILLISECONDS_PER_SECOND);
	
	// necessary because in JavaScript, some values (such as 2.01) will have a lot of decimal
	// places when multiplied by a larger number. For instance, 2.01 turns into 2009.999999999999997.
	$intTotalMs = round($intTotalMs);
	
	
	return $intTotalMs;
}

function IsIso8601SectionDelimiter($str) {
	if(preg_match("/[PYMDTHS]/", $str) > 0) {
		return true;
	} else {
		return false;
	}
}

function ConvertMilliSecondsIntoSCORM2004Time($intTotalMilliseconds) {
	
	$ScormTime = "";
	
	// $HundredthsOfASecond; //decrementing counter - work at the hundreths of a second level because that is all the precision that is required
	
	// $Seconds; // 100 hundreths of a seconds
	// $Minutes; // 60 seconds
	// $Hours; // 60 minutes
	// $Days; // 24 hours
	// $Months; // assumed to be an "average" month (figures a leap year every 4 years) = ((365*4) + 1) / 48 days - 30.4375 days per month
	// $Years; // assumed to be 12 "average" months
	
	$HUNDREDTHS_PER_SECOND = 100;
	$HUNDREDTHS_PER_MINUTE = $HUNDREDTHS_PER_SECOND * 60;
	$HUNDREDTHS_PER_HOUR = $HUNDREDTHS_PER_MINUTE * 60;
	$HUNDREDTHS_PER_DAY = $HUNDREDTHS_PER_HOUR * 24;
	$HUNDREDTHS_PER_MONTH = $HUNDREDTHS_PER_DAY * (((365 * 4) + 1) / 48);
	$HUNDREDTHS_PER_YEAR = $HUNDREDTHS_PER_MONTH * 12;
	
	$HundredthsOfASecond = floor($intTotalMilliseconds / 10);
	
	$Years = floor($HundredthsOfASecond / $HUNDREDTHS_PER_YEAR);
	$HundredthsOfASecond -= ($Years * $HUNDREDTHS_PER_YEAR);
	
	$Months = floor($HundredthsOfASecond / $HUNDREDTHS_PER_MONTH);
	$HundredthsOfASecond -= ($Months * $HUNDREDTHS_PER_MONTH);
	
	$Days = floor($HundredthsOfASecond / $HUNDREDTHS_PER_DAY);
	$HundredthsOfASecond -= ($Days * $HUNDREDTHS_PER_DAY);
	
	$Hours = floor($HundredthsOfASecond / $HUNDREDTHS_PER_HOUR);
	$HundredthsOfASecond -= ($Hours * $HUNDREDTHS_PER_HOUR);
	
	$Minutes = floor($HundredthsOfASecond / $HUNDREDTHS_PER_MINUTE);
	$HundredthsOfASecond -= ($Minutes * $HUNDREDTHS_PER_MINUTE);
	
	$Seconds = floor($HundredthsOfASecond / $HUNDREDTHS_PER_SECOND);
	$HundredthsOfASecond -= ($Seconds * $HUNDREDTHS_PER_SECOND);
	
	if($Years > 0) {
		$ScormTime .= $Years . "Y";
	}
	if($Months > 0) {
		$ScormTime .= $Months . "M";
	}
	if($Days > 0) {
		$ScormTime .= $Days . "D";
	}
	
	// check to see if we have any time before adding the "T"
	if(($HundredthsOfASecond + $Seconds + $Minutes + $Hours) > 0) {
		
		$ScormTime .= "T";
		
		if($Hours > 0) {
			$ScormTime .= $Hours . "H";
		}
		
		if($Minutes > 0) {
			$ScormTime .= $Minutes . "M";
		}
		
		if(($HundredthsOfASecond + $Seconds) > 0) {
			$ScormTime .= $Seconds;
			
			if($HundredthsOfASecond > 0) {
				$ScormTime .= "." . $HundredthsOfASecond;
			}
			
			$ScormTime .= "S";
		}
	}
	
	if($ScormTime == "") {
		$ScormTime = "0S";
	}
	
	$ScormTime = "P" . $ScormTime;
	
	
	return $ScormTime;
}

function ZeroPad($intNum, $intNumDigits) {
	
	// $strTemp;
	// $intLen;
	// $i;
	
	$strTemp = $intNum;
	$intLen = strlen($strTemp);
	
	if($intLen > $intNumDigits) {
		$strTemp = substr($strTemp, 0, $intNumDigits);
	} else {
		for($i = $intLen; $i < $intNumDigits; $i ++) {
			$strTemp = "0" . $strTemp;
		}
	}
	
	return $strTemp;
}

function ConvertMilliSecondsToSCORMTime($intTotalMilliseconds, $blnIncludeFraction = null) {
	
	// var intHours;
	// var intintMinutes;
	// var intSeconds;
	// var intMilliseconds;
	// var intHundredths;
	// var strCMITimeSpan;
	
	if($blnIncludeFraction == null) {
		$blnIncludeFraction = true;
	}
	
	// extract time parts
	$intMilliseconds = $intTotalMilliseconds % 1000;
	
	$intSeconds = (($intTotalMilliseconds - $intMilliseconds) / 1000) % 60;
	
	$intMinutes = (($intTotalMilliseconds - $intMilliseconds - ($intSeconds * 1000)) / 60000) % 60;
	
	$intHours = ($intTotalMilliseconds - $intMilliseconds - ($intSeconds * 1000) - ($intMinutes * 60000)) / 3600000;
	
	
	/*
	 * deal with exceptional case when content used a huge amount of time and interpreted CMITimstamp
	 * to allow a number of intMinutes and seconds greater than 60 i.e. 9999:99:99.99 instead of 9999:60:60:99
	 * note - this case is permissable under SCORM, but will be exceptionally rare
	 */
	
	if($intHours == 10000) {
		
		$intHours = 9999;
		
		$intMinutes = ($intTotalMilliseconds - ($intHours * 3600000)) / 60000;
		if($intMinutes == 100) {
			$intMinutes = 99;
		}
		$intMinutes = floor($intMinutes);
		
		$intSeconds = ($intTotalMilliseconds - ($intHours * 3600000) - ($intMinutes * 60000)) / 1000;
		if($intSeconds == 100) {
			$intSeconds = 99;
		}
		$intSeconds = floor($intSeconds);
		
		$intMilliseconds = ($intTotalMilliseconds - ($intHours * 3600000) - ($intMinutes * 60000) - ($intSeconds * 1000));
		
	}
	
	// drop the extra precision from the milliseconds
	$intHundredths = floor($intMilliseconds / 10);
	
	// put in padding 0's and concatinate to get the proper format
	$strCMITimeSpan = ZeroPad($intHours, 4) . ":" . ZeroPad($intMinutes, 2) . ":" . ZeroPad($intSeconds, 2);
	
	if($blnIncludeFraction) {
		$strCMITimeSpan .= "." . $intHundredths;
	}
	
	
	// check for case where total milliseconds is greater than max supported by strCMITimeSpan
	if($intHours > 9999) {
		$strCMITimeSpan = "9999:99:99";
		
		if($blnIncludeFraction) {
			$strCMITimeSpan .= ".99";
		}
	}
	
	
	return $strCMITimeSpan;
}

function ConvertCMITimeSpanToMS($strTime) {
	
	// var aryParts;
	// var intHours;
	// var intMinutes;
	// var intSeconds;
	
	// var intTotalMilliSeconds;
	
	// split the string into its parts
	$aryParts = explode(':', $strTime);
	
	// make sure it's valid before we knock ourselves out
	if(! IsValidCMITimeSpan($strTime)) {
		// SetErrorInfo(SCORM_ERROR_GENERAL, "LMS ERROR - Invalid time span passed to ConvertCMITimeSpanToMS, please contact technical support");
		return 0;
	}
	
	// seperate the parts and multiply by the appropriate constant (3600000 = num milliseconds in an hour, etc)
	$intHours = $aryParts[0];
	$intMinutes = $aryParts[1];
	$intSeconds = $aryParts[2]; // don't need to worry about milliseconds b/c they are expressed as fractions of a second
	
	
	$intTotalMilliseconds = ($intHours * 3600000) + ($intMinutes * 60000) + ($intSeconds * 1000);
	
	// necessary because in JavaScript, some values for intSeconds (such as 2.01) will have a lot of decimal
	// places when multiplied by 1000. For instance, 2.01 turns into 2009.999999999999997.
	$intTotalMilliseconds = round($intTotalMilliseconds);
	
	
	return $intTotalMilliseconds;
}

function IsValidCMITimeSpan($strValue) {
	
	// note that the spec does not say that minutes or seconds have to be < 60
	
	$regValid = "/^\d?\d?\d?\d:\d?\d:\d?\d(.\d\d?)?$/";
	if(preg_match($regValid, $strValue) > 0) {
		return true;
	} else {
		return false;
	}
}