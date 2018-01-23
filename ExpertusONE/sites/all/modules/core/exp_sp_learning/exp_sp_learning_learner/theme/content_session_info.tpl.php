<div class="paindContentResults clsSeeMorePlaceholderdiv"
	id="paindContentResults_1"></div>
<?php
$sessDetInfo = array (
		'title' => "session title goes here",
		'start_date' => "Saturday Mar 11 2017",
		'session_id' => 1,
		'sess_start_time' => "12:15",
		'ses_st_time_format' => "PM",
		'sess_end_time' => "2:15",
		'ses_end_time_format' => "PM",
		'sess_timezone' => 'PST',
		'session_address1' => "High Court",
		'session_address2' => "Parrys Corner",
		'session_city' => "Chennai",
		'session_state' => "Tamil Nadu",
		'session_country' => "India",
		'session_zipcode' => "600001" 
)
;
$userId = getSltpersonUserId();
$timezone_details = getPersonDetails($userId);
$result = new stdClass ();
$result->clsdeliverytype = 'lrn_cls_dty_ilt';
$result->locationname = 'Chennai';
$result->sessionDetailInfo = array (
		0 => $sessDetInfo 
);
/*
$location_details = $sessionInfo ["ilt_start_date"] . ' ' . $sessionInfo ["ilt_start_time"] . ' ' .
		'<div class="time-zone-text">' . $sessionInfo ["ilt_start_time_format"] . '</div>' .
		' to ' . $sessionInfo ["ilt_end_time"] .
		' <div class="time-zone-text">' . $sessionInfo ["ilt_end_time_format"] . '</div> '
				. $sessionInfo ["session_code"] . ' ' . $sessionInfo ["tz_code"] . '';
expDebug::dPrint ( 'fifth: ' . print_r ( $location_details, true ), 4 );
*/
?>
<div class="session-details-wrapper clearBoth">
<?php
if (isset ( $result->sessionDetailInfo ) && ($result->clsdeliverytype == 'lrn_cls_dty_ilt' || $result->clsdeliverytype == 'lrn_cls_dty_vcl')) {
	expDebug::dPrint ( '$session startdd ' );
	?>
	
	<div class="code-container location">
<?php
	$sessinc = 1;
	foreach ( $result->sessionDetailInfo as $sessionInfo ) {
		if ($sessinc == 1) {
			$sessinc ++;
			if (! empty ( $result->locationname ))
				$sessAddDet = $result->locationname;
				// if(!empty($sessionInfo["session_name"])){
				// $sessAddDet .= ",<br>".$sessionInfo["session_name"];
				// }
			if (! empty ( $sessionInfo ["session_address1"] )) {
				$sessAddDet .= "<span>," . $sessionInfo ["session_address1"]."</span>";
			}
			if (! empty ( $sessionInfo ["session_address2"] )) {
				$sessAddDet .= "<span>," . $sessionInfo ["session_address2"]."</span>";
			}
			if (! empty ( $sessionInfo ["session_city"] )) {
				$sessAddDet .= "<span>," . $sessionInfo ["session_city"]."</span>";
			}
			if (! empty ( $sessionInfo ["session_state"] )) {
				$sessAddDet .= "<span>, " . $sessionInfo ["session_state"]."</span>";
			}
			if (! empty ( $sessionInfo ["session_country"] )) {
				$sessAddDet .= "<span>," . $sessionInfo ["session_country"]."</span>";
			}
			if (! empty ( $sessionInfo ["session_zipcode"] )) {
				$sessAddDet .= "<span>&nbsp;" . $sessionInfo ["session_zipcode"]."</span>";
			}
			
			print "<div class='detail-code'>" . t ( "Location" ) . "</div><div class='detail-desc clearBoth'>" . $sessAddDet."</div>";
		}
	}
	?>	
	</div>





	<div class="code-container session clearBoth">
		<div class="detail-code"><?php print t("LBL249"); ?></div>
		<div class="detail-desc clearBoth">
			<div>
<?php
	$i = 0;
	expDebug::dPrint ( ' $result session= ' . print_r ( $result, true ), 3 );
	foreach ( $result->sessionDetailInfo as $sessionInfo ) {
		expDebug::dPrint ( ' $result session idss= ' . print_r ( $sessionInfo, true ), 3 );
		?>
    				<div class="class-session-detail">
					<div class="clearBoth">
						<div class="float-left"><?php print t("Name"); ?> &nbsp;:&nbsp;</div>
						<div class="class-detail-session-name vtip float-left"
							title="<?php print $sessionInfo["title"]  ;  ?>">
								<?php print titleController('CLASS-DETAIL-SESSION-TITLE',$sessionInfo["title"],20); '&nbsp;'?>
							</div>
					</div>

					<div class="clearBoth">
						<div class="float-left"><?php print t("LBL042"); ?>&nbsp;:&nbsp;</div>
						<div class="float-left"><?php print  $sessionInfo ["start_date"]; ?></div>
						<div class="padding-left float-left"><?php print $sessionInfo ["sess_start_time"]; ?><span
								class="time-zone-format padding-left"><?php print $sessionInfo ["ses_st_time_format"]; ?>&nbsp;to</span>
						</div>
						<div class="padding-left float-left"><?php print $sessionInfo ["sess_end_time"]; ?><span
								class="time-zone-format padding-left"><?php print $sessionInfo ["ses_end_time_format"]; ?></span>
						</div>
	<?php
		if ($result->clsdeliverytype == 'lrn_cls_dty_ilt' && $sessionInfo ["sess_timezone"] != $timezone_details ['attr2']) {
			$qtip = qtip_popup_paint ( $sessionInfo ["session_id"], $location_details, '', 1 );
			echo $qtip;
		}
		?>							
						</div>
				</div>
<?php
		$i ++;
	}
	?>
				</div>
		</div>
	</div>
	
	
<?php }  ?>

</div>