<?php 
include_once $_SERVER['DOCUMENT_ROOT']."/sites/default/settings.php";

$js = array();
$js[]="/sites/all/modules/core/exp_sp_core/modules/h5p/library/js/jquery.js";
$js[]="/sites/all/modules/core/exp_sp_core/modules/h5p/library/js/h5p.js";
$js[]="/sites/all/modules/core/exp_sp_core/modules/h5p/library/js/h5p-event-dispatcher.js";
$js[]="/sites/all/modules/core/exp_sp_core/modules/h5p/library/js/h5p-x-api-event.js";
$js[]="/sites/all/modules/core/exp_sp_core/modules/h5p/library/js/h5p-x-api.js";
$js[]="/sites/all/modules/core/exp_sp_core/modules/h5p/library/js/h5p-content-type.js";
$js[]="/sites/all/modules/core/exp_sp_core/modules/h5p/library/js/h5p-confirmation-dialog.js";
$js[]="/sites/all/libraries/h5plibraries/H5P.AdvancedText-1.1/text.js";
$js[]="/sites/all/libraries/h5plibraries/H5P.Link-1.1/link.js";
$js[]="/sites/all/libraries/h5plibraries/H5P.Image-1.0/image.js";
$js[]="/sites/all/libraries/h5plibraries/flowplayer-1.0/scripts/flowplayer-3.2.12.min.js";
$js[]="/sites/all/libraries/h5plibraries/H5P.Video-1.2/scripts/youtube.js";
$js[]="/sites/all/libraries/h5plibraries/H5P.Video-1.2/scripts/html5.js";
$js[]="/sites/all/libraries/h5plibraries/H5P.Video-1.2/scripts/flash.js";
$js[]="/sites/all/libraries/h5plibraries/H5P.Video-1.2/scripts/video.js";
$js[]="/sites/all/libraries/h5plibraries/H5P.Audio-1.2/scripts/audio.js";
$js[]="/sites/all/libraries/h5plibraries/Tether-1.0/scripts/tether.min.js";
$js[]="/sites/all/libraries/h5plibraries/Drop-1.0/js/drop.min.js";
$js[]="/sites/all/libraries/h5plibraries/H5P.Transition-1.0/transition.js";
$js[]="/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/js/joubel-help-dialog.js";
$js[]="/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/js/joubel-message-dialog.js";
$js[]="/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/js/joubel-progress-circle.js";
$js[]="/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/js/joubel-simple-rounded-button.js";
$js[]="/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/js/joubel-speech-bubble.js";
$js[]="/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/js/joubel-throbber.js";
$js[]="/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/js/joubel-tip.js";
$js[]="/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/js/joubel-slider.js";
$js[]="/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/js/joubel-score-bar.js";
$js[]="/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/js/joubel-progressbar.js";
$js[]="/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/js/joubel-ui.js";
$js[]="/sites/all/libraries/h5plibraries/H5P.Question-1.1/scripts/question.js";
$js[]="/sites/all/libraries/h5plibraries/H5P.Blanks-1.4/js/blanks.js";
$js[]="/sites/all/libraries/h5plibraries/H5P.Blanks-1.4/js/cloze.js";
$js[]="/sites/all/libraries/h5plibraries/H5P.SoundJS-1.0/soundjs-0.6.0.min.js";
$js[]="/sites/all/libraries/h5plibraries/H5P.SingleChoiceSet-1.3/scripts/sound-effects.js";
$js[]="/sites/all/libraries/h5plibraries/H5P.SingleChoiceSet-1.3/scripts/event-emitter.js";
$js[]="/sites/all/libraries/h5plibraries/H5P.SingleChoiceSet-1.3/scripts/result-slide.js";
$js[]="/sites/all/libraries/h5plibraries/H5P.SingleChoiceSet-1.3/scripts/solution-view.js";
$js[]="/sites/all/libraries/h5plibraries/H5P.SingleChoiceSet-1.3/scripts/single-choice-alternative.js";
$js[]="/sites/all/libraries/h5plibraries/H5P.SingleChoiceSet-1.3/scripts/single-choice.js";
$js[]="/sites/all/libraries/h5plibraries/H5P.SingleChoiceSet-1.3/scripts/single-choice-set.js";
$js[]="/sites/all/libraries/h5plibraries/H5P.Summary-1.4/js/summary.js";
$js[]="/sites/all/libraries/h5plibraries/jQuery.ui-1.10/js/jquery-ui.js";
$js[]="/sites/all/libraries/h5plibraries/H5P.DragText-1.4/drag-text.js";
$js[]="/sites/all/libraries/h5plibraries/H5P.MarkTheWords-1.5/mark-the-words.js";
$js[]="/sites/all/libraries/h5plibraries/H5P.CoursePresentation-1.9/scripts/cp.js";
$js[]="/sites/all/libraries/h5plibraries/H5P.CoursePresentation-1.9/scripts/go-to-slide.js";
$js[]="/sites/all/libraries/h5plibraries/H5P.CoursePresentation-1.9/scripts/summary-slide.js";
$js[]="/sites/all/libraries/h5plibraries/H5P.CoursePresentation-1.9/scripts/navigation-line.js";
$js[]="/sites/all/libraries/h5plibraries/H5P.CoursePresentation-1.9/scripts/slide-backgrounds.js";
$js[]="/sites/all/libraries/h5plibraries/H5P.CoursePresentation-1.9/scripts/printer.js";

$css = array();

$css[]="/sites/all/modules/core/exp_sp_core/modules/h5p/library/styles/h5p.css";
$css[]="/sites/all/modules/core/exp_sp_core/modules/h5p/library/styles/h5p-confirmation-dialog.css";
$css[]="/sites/all/modules/core/exp_sp_core/modules/h5p/library/styles/h5p-core-button.css";
$css[]="/sites/all/libraries/h5plibraries/H5P.AdvancedText-1.1/text.css";
$css[]="/sites/all/libraries/h5plibraries/H5P.Image-1.0/image.css";
$css[]="/sites/all/libraries/h5plibraries/H5P.Video-1.2/styles/video.css";
//$css[]="/sites/all/libraries/h5plibraries/FontAwesome-4.5/h5p-font-awesome.min.css";
$css[]="/sites/all/libraries/h5plibraries/H5P.Audio-1.2/styles/audio.css";
$css[]="/sites/all/libraries/h5plibraries/Tether-1.0/styles/tether.min.css";
$css[]="/sites/all/libraries/h5plibraries/Drop-1.0/css/drop-theme-arrows-bounce.min.css";
$css[]="/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/css/joubel-help-dialog.css";
$css[]="/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/css/joubel-message-dialog.css";
$css[]="/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/css/joubel-progress-circle.css";
$css[]="/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/css/joubel-simple-rounded-button.css";
$css[]="/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/css/joubel-speech-bubble.css";
$css[]="/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/css/joubel-tip.css";
$css[]="/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/css/joubel-slider.css";
$css[]="/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/css/joubel-score-bar.css";
$css[]="/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/css/joubel-progressbar.css";
$css[]="/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/css/joubel-ui.css";
$css[]="/sites/all/libraries/h5plibraries/H5P.Question-1.1/styles/question.css";
$css[]="/sites/all/libraries/h5plibraries/H5P.Blanks-1.4/css/blanks.css";
$css[]="/sites/all/libraries/h5plibraries/H5P.SingleChoiceSet-1.3/styles/single-choice-set.css";
$css[]="/sites/all/libraries/h5plibraries/H5P.Summary-1.4/css/summary.css";
$css[]="/sites/all/libraries/h5plibraries/jQuery.ui-1.10/development-bundle/themes/base/jquery-ui.css";
$css[]="/sites/all/libraries/h5plibraries/H5P.DragText-1.4/drag-text.css";
$css[]="/sites/all/libraries/h5plibraries/H5P.MarkTheWords-1.5/mark-the-words.css";
$css[]="/sites/all/libraries/h5plibraries/H5P.CoursePresentation-1.9/styles/summary-slide.css";
$css[]="/sites/all/libraries/h5plibraries/H5P.CoursePresentation-1.9/styles/navigation-line.css";
$css[]="/sites/all/libraries/h5plibraries/H5P.CoursePresentation-1.9/styles/slide-background.css";
$css[]="/sites/all/libraries/h5plibraries/H5P.CoursePresentation-1.9/styles/print.css";
$css[]="/sites/all/modules/core/exp_sp_core/modules/h5p/modules/h5peditor/h5p-overrides.css";

$jsdata = "";
$cssdata = "";
global $base_url;

for($i = 0; $i< count($js); $i++)
{
$jsdata .= ";".file_get_contents($base_url."".$js[$i]);
}
file_put_contents($_SERVER['DOCUMENT_ROOT']."/h5pmerge/embedpresentjs.js",$jsdata);

for($i = 0; $i< count($css); $i++)
{
$cssdata .= "\n".file_get_contents($base_url."".$css[$i]);
}
file_put_contents($_SERVER['DOCUMENT_ROOT']."/h5pmerge/embedpresentcss.css",$cssdata);
$jsdata = "";
$cssdata = "";
echo "done";

?>
