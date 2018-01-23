<?php

/**
 * @file
 * Default theme implementation to display a printable Ubercart invoice.
 *
 * @see template_preprocess_uc_order_invoice_page()
 */
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">

<head>
  <title>
  <?php global $theme_key?>
  <?php print $head_title; ?>
  </title>
   
  <?php 
  /********** start ************/
  $cert_css_path = $base_url. '/'. drupal_get_path('module', 'theme_override');
  $filename = $_SERVER['DOCUMENT_ROOT'].'/'. drupal_get_path('module', 'theme_override').'/css/expertusone_en_override.css';
  
    if ((function_exists('theme_override_init')) && (file_exists($filename))) { ?>
    	<link href="<?php echo $cert_css_path; ?>/css/expertusone_en_override.css" rel="stylesheet">
  <?php }else {?>	
		    <style>
		        .admin-action-button-left-bg {
  				   display:none
				}
				.admin-action-button-middle-bg {
 					background:#27245e;
 					border:1px solid #27245e;
 					border-radius:6px;
				}
				.admin-action-button-right-bg {
  					display:none
				}
				.admin-action-button-container{
  					margin: 8px 30px 20px 10px;
  				}
				
  	</style>
  <?php }
  /********** End ************/
  
  if($theme_key != 'expertusoneV2') {?>
  <style type="text/css">
body {
  margin:8px;
}
.view-button {
  background:none;
  border:0px solid #FFF;
  color:#4686DD;
  cursor:pointer;
  text-align:center;
  font-size:12px;
  float:left;
  margin-top:3px;
}
.admin-save-button-left-bg {
  float: left;
  height: 22px;
  width: 7px;
 }
.admin-save-button-middle-bg {
  border: 0 none;
  color: #FFFFFF;
  float: left;
  font-size: 12px;
  font-weight: normal;
  height: 21px;
  line-height: 22px;
  margin: 0;
  padding: 0;  	
}
.admin-save-button-right-bg {
  float: left;
  height: 22px;
  width: 7px;  	
}
.print-invoice-action-btn-container {
  margin: 7px 30px 3px 4px;
  float:right;
}
.print-invoice-action-btn-container {
  margin-right: 25px\9;
}

  </style>
</head>
<body>
  <?php print $content; ?>
  <div align="right" class="print-invoice-action-btn-container">
    <input id="btnClose" class ="view-button" type="button" value="<?php print t('LBL123'); ?>" onclick="window.close();" />
    <span class="admin-save-button-left-bg"></span>
    <input id="btnPrint" class ="view-button admin-save-button-middle-bg" type="button" value="<?php print t('LBL308'); ?>" onclick="window.print();" />
    <span class="admin-save-button-right-bg"></span>
  </div>  
</body>
</html>
<?php }else{?>
  <style type="text/css">
body {
  margin:8px;
}
.admin-action-button-left-bg {
  float: left;
  height: 34px;
  width: 15px;
 }
.admin-action-button-middle-bg {
  float: left;
  text-align: center;
  height: 34px;
  line-height: 34px;
  padding: 0 5px;
  color: #fff; 
  font-family: ProximaNovaBold;
  font-weight:bold;
  text-transform: uppercase;
  font-size: 11px;
  margin: 0;
  border:0
}
.admin-action-button-right-bg {
  float: left;
  height: 34px;
  width: 15px;	
}
.admin-action-button-container{
  margin: 7px 30px 3px 4px;
  float:right;
}
.white-btn-bg-left {
  float: left;
  height: 34px;
  width: 15px;
  display:none
}
.white-btn-bg-right{
  float: left;
  width: 15px;
  height: 34px;
  display:none
}
.white-btn-bg-middle {
  float: left ;
  color: #474747 ;
  height: 35px;
  padding: 0 5px;
  line-height: 36px;
  font-weight:bold;
  font-size: 11px;
  text-transform: uppercase ;
  font-family: ProximaNovaBold ;
  margin:0px;
  background:#fff;
  border:1px solid #dedede;
  border-radius:6px
}
.white-btn-bg-container{
  float:right;
  margin-top:8px;
}
.action-button-container{
  margin-right:-3px;
} 
@media print { 
/*safari hack Start*/
@media screen and (min-color-index:0)and(-webkit-min-device-pixel-ratio:0), not all and (min-resolution:.001dpcm) { @media { _:-webkit-full-screen,
     .new-theme-container .newInvoice .invoice-view-status{
        padding:0;
        line-height:13px;
        padding-bottom:2px;
        white-space:nowrap
      }
}}
/*safari hack End*/
 }
  </style>
</head>
<body>
  <?php print $content; ?>
  <div class="action-button-container">
  <div align="right" class="admin-action-button-container">
    <span class="admin-action-button-left-bg"></span>
    <input id="btnPrint" class ="admin-action-button-middle-bg" type="button" value="<?php print t('LBL308'); ?>" onclick="window.print();" />
    <span class="admin-action-button-right-bg"></span>
  </div>
   <div align="right" class="white-btn-bg-container">
  <span class="white-btn-bg-left"></span>
  <input id="btnClose" class ="white-btn-bg-middle" type="button" value="<?php print t('LBL123'); ?>" onclick="window.close();" />
  <span class="white-btn-bg-right"></span>
  </div>
  </div>
</body>
<?php }?>