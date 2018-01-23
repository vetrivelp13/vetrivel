<?php 
include_once $_SERVER['DOCUMENT_ROOT']."/sites/default/settings.php";

 $js = array();
 $path ='/sites/all/modules/core/exp_sp_core/modules/h5p/modules/h5peditor/h5peditor/';
 $js[] =$path.'scripts/h5peditor.js';
 $js[] = $path.'scripts/h5peditor-semantic-structure.js';
 //$js[] =$path.'scripts/h5peditor-editor.js';
 $js[] =$path.'scripts/h5peditor-library-selector.js';
 $js[] =$path.'scripts/h5peditor-form.js';
 $js[] =$path.'scripts/h5peditor-text.js';
 $js[] =$path.'scripts/h5peditor-html.js';
 $js[] =$path.'scripts/h5peditor-number.js';
 $js[] =$path.'scripts/h5peditor-textarea.js';
 $js[] =$path.'scripts/h5peditor-file-uploader.js';
 $js[] =$path.'scripts/h5peditor-file.js';
 $js[] =$path.'scripts/h5peditor-image.js';
 $js[] =$path.'scripts/h5peditor-image-popup.js';
 $js[] =$path.'scripts/h5peditor-av.js';
 $js[] =$path.'scripts/h5peditor-group.js';
 $js[] =$path.'scripts/h5peditor-boolean.js';
 $js[] =$path.'scripts/h5peditor-list.js';
 $js[] =$path.'scripts/h5peditor-list-editor.js';
 $js[] =$path.'scripts/h5peditor-library.js';
 $js[] =$path.'scripts/h5peditor-library-list-cache.js';
 $js[] =$path.'scripts/h5peditor-select.js';
 $js[] =$path.'scripts/h5peditor-dimensions.js';
 $js[] =$path.'scripts/h5peditor-coordinates.js';
 $js[] =$path.'scripts/h5peditor-none.js';
 

 $css = array();
 
 $jsdata = "";
 $cssdata = "";
 global $base_url;
  
  for($i = 0; $i< count($js); $i++)
 {
 	$jsdata .= ";\n".file_get_contents($base_url."".$js[$i]."?ts=".time());
 }
 file_put_contents($_SERVER['DOCUMENT_ROOT'].$path."scripts/h5peditor-core.js",$jsdata);
 
 /*for($i = 0; $i< count($css); $i++)
 {
 	$cssdata .= "\n".file_get_contents($base_url."".$css[$i]);
 }
 file_put_contents($_SERVER['DOCUMENT_ROOT']."/h5pmerge/embedvideocss.css",$cssdata);*/
 $jsdata = "";
 $cssdata = "";
 echo "done";
 
?>
