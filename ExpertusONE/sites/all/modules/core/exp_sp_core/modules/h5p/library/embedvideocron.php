<?php 
include_once $_SERVER['DOCUMENT_ROOT']."/sites/default/settings.php";

$js = array();
 $js[] = "/sites/all/modules/core/exp_sp_core/modules/h5p/library/js/jquery.js";
 $js[] = "/sites/all/modules/core/exp_sp_core/modules/h5p/library/js/h5p.js";
 $js[] = "/sites/all/modules/core/exp_sp_core/modules/h5p/library/js/h5p-event-dispatcher.js";
 $js[] = "/sites/all/modules/core/exp_sp_core/modules/h5p/library/js/h5p-x-api-event.js";
 $js[] = "/sites/all/modules/core/exp_sp_core/modules/h5p/library/js/h5p-x-api.js";
 $js[] = "/sites/all/modules/core/exp_sp_core/modules/h5p/library/js/h5p-content-type.js";
 $js[] = "/sites/all/modules/core/exp_sp_core/modules/h5p/library/js/h5p-confirmation-dialog.js";
 $js[] = "/sites/all/libraries/h5plibraries/H5P.Text-1.1/scripts/text.js";
 $js[] = "/sites/all/libraries/h5plibraries/H5P.Link-1.1/link.js";
 $js[] = "/sites/all/libraries/h5plibraries/H5P.Image-1.0/image.js";
 $js[] = "/sites/all/libraries/h5plibraries/Tether-1.0/scripts/tether.min.js";
 $js[] = "/sites/all/libraries/h5plibraries/Drop-1.0/js/drop.min.js";
 $js[] = "/sites/all/libraries/h5plibraries/H5P.Transition-1.0/transition.js";
 $js[] = "/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/js/joubel-help-dialog.js";
 $js[] = "/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/js/joubel-message-dialog.js";
 $js[] = "/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/js/joubel-progress-circle.js";
 $js[] = "/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/js/joubel-simple-rounded-button.js";
 $js[] = "/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/js/joubel-speech-bubble.js";
 $js[] = "/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/js/joubel-throbber.js";
 $js[] = "/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/js/joubel-tip.js";
 $js[] = "/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/js/joubel-slider.js";
 $js[] = "/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/js/joubel-score-bar.js";
 $js[] = "/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/js/joubel-progressbar.js";
 $js[] = "/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/js/joubel-ui.js";
 $js[] = "/sites/all/libraries/h5plibraries/H5P.Question-1.1/scripts/question.js";
 $js[] = "/sites/all/libraries/h5plibraries/H5P.SoundJS-1.0/soundjs-0.6.0.min.js";
 $js[] = "/sites/all/libraries/h5plibraries/H5P.SingleChoiceSet-1.3/scripts/sound-effects.js";
 $js[] = "/sites/all/libraries/h5plibraries/H5P.SingleChoiceSet-1.3/scripts/event-emitter.js";
 $js[] = "/sites/all/libraries/h5plibraries/H5P.SingleChoiceSet-1.3/scripts/result-slide.js";
 $js[] = "/sites/all/libraries/h5plibraries/H5P.SingleChoiceSet-1.3/scripts/solution-view.js";
 $js[] = "/sites/all/libraries/h5plibraries/H5P.SingleChoiceSet-1.3/scripts/single-choice-alternative.js";
 $js[] = "/sites/all/libraries/h5plibraries/H5P.SingleChoiceSet-1.3/scripts/single-choice.js";
 $js[] = "/sites/all/libraries/h5plibraries/H5P.SingleChoiceSet-1.3/scripts/single-choice-set.js";
 $js[] = "/sites/all/libraries/h5plibraries/H5P.Blanks-1.4/js/blanks.js";
 $js[] = "/sites/all/libraries/h5plibraries/H5P.Blanks-1.4/js/cloze.js";
 $js[] = "/sites/all/libraries/h5plibraries/H5P.MarkTheWords-1.5/mark-the-words.js";
 $js[] = "/sites/all/libraries/h5plibraries/jQuery.ui-1.10/js/jquery-ui.js";
 $js[] = "/sites/all/libraries/h5plibraries/H5P.DragText-1.4/drag-text.js";
 $js[] = "/sites/all/libraries/h5plibraries/H5P.GoToQuestion-1.0/scripts/go-to-question.js";
 $js[] = "/sites/all/libraries/h5plibraries/H5P.Summary-1.4/js/summary.js";
 $js[] = "/sites/all/libraries/h5plibraries/H5P.DragNDrop-1.1/drag-n-drop.js";
 $js[] = "/sites/all/libraries/h5plibraries/H5P.DragNResize-1.2/H5P.DragNResize.js";
 $js[] = "/sites/all/libraries/h5plibraries/H5P.DragNBar-1.2/scripts/drag-n-bar.js";
 $js[] = "/sites/all/libraries/h5plibraries/H5P.DragNBar-1.2/scripts/context-menu.js";
 $js[] = "/sites/all/libraries/h5plibraries/H5P.DragNBar-1.2/scripts/dialog.js";
 $js[] = "/sites/all/libraries/h5plibraries/H5P.DragNBar-1.2/scripts/drag-n-bar-element.js";
 $js[] = "/sites/all/libraries/h5plibraries/flowplayer-1.0/scripts/flowplayer-3.2.12.min.js";
 $js[] = "/sites/all/modules/core/exp_sp_core/js/videojs/video.js";
 $js[] = "/sites/all/libraries/h5plibraries/H5P.Video-1.2/scripts/youtube.js";
 $js[] = "/sites/all/libraries/h5plibraries/H5P.Video-1.2/scripts/html5.js";
 $js[] = "/sites/all/libraries/h5plibraries/H5P.Video-1.2/scripts/flash.js";
 $js[] = "/sites/all/libraries/h5plibraries/H5P.Video-1.2/scripts/video.js";
 $js[] = "/sites/all/libraries/h5plibraries/H5P.InteractiveVideo-1.9/scripts/interaction.js";
 $js[] = "/sites/all/libraries/h5plibraries/H5P.InteractiveVideo-1.9/scripts/interactive-video.js";

 $css = array();
 
 $css[] = "/sites/all/modules/core/exp_sp_core/modules/h5p/library/styles/h5p.css";
 $css[] = "/sites/all/modules/core/exp_sp_core/modules/h5p/library/styles/h5p-confirmation-dialog.css";
 $css[] = "/sites/all/modules/core/exp_sp_core/modules/h5p/library/styles/h5p-core-button.css";
 $css[] = "/sites/all/libraries/h5plibraries/H5P.Text-1.1/styles/text.css";
 $css[] = "/sites/all/libraries/h5plibraries/H5P.Image-1.0/image.css";
 $css[] = "/sites/all/libraries/h5plibraries/FontAwesome-4.5/h5p-font-awesome.min.css";
 $css[] = "/sites/all/libraries/h5plibraries/Tether-1.0/styles/tether.min.css";
 $css[] = "/sites/all/libraries/h5plibraries/Drop-1.0/css/drop-theme-arrows-bounce.min.css";
 $css[] = "/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/css/joubel-help-dialog.css";
 $css[] = "/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/css/joubel-message-dialog.css";
 $css[] = "/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/css/joubel-progress-circle.css";
 $css[] = "/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/css/joubel-simple-rounded-button.css";
 $css[] = "/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/css/joubel-speech-bubble.css";
 $css[] = "/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/css/joubel-tip.css";
 $css[] = "/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/css/joubel-slider.css";
 $css[] = "/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/css/joubel-score-bar.css";
 $css[] = "/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/css/joubel-progressbar.css";
 $css[] = "/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/css/joubel-ui.css";
 $css[] = "/sites/all/libraries/h5plibraries/H5P.Question-1.1/styles/question.css";
 $css[] = "/sites/all/libraries/h5plibraries/H5P.SingleChoiceSet-1.3/styles/single-choice-set.css";
 $css[] = "/sites/all/libraries/h5plibraries/H5P.Blanks-1.4/css/blanks.css";
 $css[] = "/sites/all/libraries/h5plibraries/H5P.MarkTheWords-1.5/mark-the-words.css";
 $css[] = "/sites/all/libraries/h5plibraries/jQuery.ui-1.10/development-bundle/themes/base/jquery-ui.css?ver=1.10.14";
 $css[] = "/sites/all/libraries/h5plibraries/H5P.DragText-1.4/drag-text.css?ver=1.4.5";
 $css[] = "/sites/all/libraries/h5plibraries/H5P.GoToQuestion-1.0/styles/go-to-question.css";
 $css[] = "/sites/all/libraries/h5plibraries/H5P.Summary-1.4/css/summary.css";
 $css[] = "/sites/all/libraries/h5plibraries/H5P.DragNResize-1.2/H5P.DragNResize.css";
 $css[] = "/sites/all/libraries/h5plibraries/H5P.DragNBar-1.2/styles/drag-n-bar.css";
 $css[] = "/sites/all/libraries/h5plibraries/H5P.DragNBar-1.2/styles/dialog.css";
 $css[] = "/sites/all/libraries/h5plibraries/H5P.DragNBar-1.2/styles/context-menu.css";
 $css[] = "/sites/all/libraries/h5plibraries/H5P.Video-1.2/styles/video.css";
 $css[] = "/sites/all/modules/core/exp_sp_core/modules/h5p/modules/h5peditor/h5p-overrides.css";
 $css[] ="/sites/all/modules/core/exp_sp_core/js/videojs/video-js.min.css";
 $jsdata = "";
 $cssdata = "";
 global $base_url;
  
  for($i = 0; $i< count($js); $i++)
 {
 	$jsdata .= ";".file_get_contents($base_url."".$js[$i]."?ts=".time());
 }
 
 file_put_contents($_SERVER['DOCUMENT_ROOT']."/h5pmerge/embedvideojs.js",$jsdata);
 
 for($i = 0; $i< count($css); $i++)
 {
 	$cssdata .= "\n".file_get_contents($base_url."".$css[$i]);
 }
 file_put_contents($_SERVER['DOCUMENT_ROOT']."/h5pmerge/embedvideocss.css",$cssdata);
 $jsdata = "";
 $cssdata = "";
 echo "done";
 
?>
