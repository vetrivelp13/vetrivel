<?php 
include_once $_SERVER['DOCUMENT_ROOT']."/sites/default/settings.php";

$js = array();

/*$js[] = "/sites/all/libraries/h5plibraries/H5P.TwitterUserFeed-1.0/twitter-user-feed.js";
$js[] = "/sites/all/libraries/h5plibraries/H5PEditor.InteractiveVideo-1.9/Scripts/guided-tours.js";
$js[] =	"/sites/all/libraries/h5plibraries/H5P.Table-1.1/scripts/table.js";
$js[] = "/sites/all/libraries/h5plibraries/H5P.Summary-1.4/js/summary.js";
$js[] = "/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/js/joubel-help-dialog.js";
$js[] ="/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/js/joubel-message-dialog.js";
$js[] ="/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/js/joubel-progress-circle.js" ;
$js[] ="/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/js/joubel-simple-rounded-button.js" ;
$js[] ="/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/js/joubel-speech-bubble.js";
$js[] ="/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/js/joubel-throbber.js" ;
$js[] ="/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/js/joubel-tip.js";
$js[] ="/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/js/joubel-slider.js";
$js[] ="/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/js/joubel-score-bar.js" ;
$js[] ="/sites/all/libraries/h5plibraries/H5P.JoubelUI-1.2/js/joubel-progressbar.js" ; */


//for video
$js[] = "/sites/all/libraries/h5plibraries/H5P.Video-1.2/scripts/youtube.js";
$js[] ="/sites/all/libraries/h5plibraries/H5P.Video-1.2/scripts/html5.js";
$js[] ="/sites/all/libraries/h5plibraries/H5P.Video-1.2/scripts/flash.js";
$js[] ="/sites/all/libraries/h5plibraries/H5P.Video-1.2/scripts/video.js" ;

//for audio
$js[] ="/sites/all/libraries/h5plibraries/H5P.Audio-1.2/scripts/audio.js";

//for sound effect in single choice set and fill in blank
$js[] ="/sites/all/libraries/h5plibraries/swfobject-1.0/scripts/swfobject.js";
$js [] = "/sites/all/libraries/h5plibraries/H5P.SoundJS-1.0/soundjs-0.6.0.min.js";
$js [] = "/sites/all/libraries/h5plibraries/H5P.SingleChoiceSet-1.3/scripts/sound-effects.js";

//for single choiceset
$js[] ="/sites/all/libraries/h5plibraries/H5P.SingleChoiceSet-1.3/scripts/sound-effects.js";
$js[] ="/sites/all/libraries/h5plibraries/H5P.SingleChoiceSet-1.3/scripts/event-emitter.js";
$js[] ="/sites/all/libraries/h5plibraries/H5P.SingleChoiceSet-1.3/scripts/result-slide.js";
$js[] ="/sites/all/libraries/h5plibraries/H5P.SingleChoiceSet-1.3/scripts/solution-view.js";
$js[] ="/sites/all/libraries/h5plibraries/H5P.SingleChoiceSet-1.3/scripts/single-choice-alternative.js";
$js[] ="/sites/all/libraries/h5plibraries/H5P.SingleChoiceSet-1.3/scripts/single-choice.js";
$js[] ="/sites/all/libraries/h5plibraries/H5P.SingleChoiceSet-1.3/scripts/single-choice-set.js";

//ejs
//$js[] ="/sites/all/libraries/h5plibraries/EmbeddedJS-1.0/js/ejs_production.js";
//$js[] ="/sites/all/libraries/h5plibraries/EmbeddedJS-1.0/js/ejs_viewhelpers.js";
 
//Advancedtext
//$js[] ="/sites/all/libraries/h5plibraries/H5P.AdvancedText-1.1/text.js";

//for image
$js[] = "/sites/all/libraries/h5plibraries/H5P.Image-1.0/image.js";

//for link
//$js[] = "/sites/all/libraries/h5plibraries/H5PEditor.UrlField-1.0/link-widget.js";
//$js[] = "/sites/all/libraries/h5plibraries/H5P.Link-1.1/link.js";

//for blanks
$js[] = "/sites/all/libraries/h5plibraries/H5P.Blanks-1.4/js/blanks.js";
$js[] = "/sites/all/libraries/h5plibraries/H5P.Blanks-1.4/js/cloze.js";

//Drag text
$js[] = "/sites/all/libraries/h5plibraries/H5PEditor.DragQuestion-1.4/H5PEditor.DragQuestion.js";
$js[] = "/sites/all/libraries/h5plibraries/H5P.DragQuestion-1.5/js/dragquestion.js";
$js[] = "/sites/all/libraries/h5plibraries/H5P.DragText-1.4/drag-text.js";

//mark the word
$js[] = "/sites/all/libraries/h5plibraries/H5P.MarkTheWords-1.5/mark-the-words.js";


/*$js[] ="/sites/all/libraries/h5plibraries/H5P.GoToQuestion-1.0/scripts/go-to-question.js" ;
$js[] ="/sites/all/libraries/h5plibraries/Shepherd-1.0/scripts/shepherd.js" ;
$js[] ="/sites/all/libraries/h5plibraries/H5P.GuidedTour-1.0/scripts/h5p-guided-tour.js";
$js[] = "/sites/all/libraries/h5plibraries/H5PEditor.SummaryTextualEditor-1.0/summary-textual-editor.js";

$js[] =  "/sites/all/libraries/h5plibraries/H5PEditor.InteractiveVideo-1.9/Scripts/image-radio-button-group.js" ;
$js[] = "/sites/all/libraries/h5plibraries/H5PEditor.InteractiveVideo-1.9/Scripts/interactive-video-editor.js" ;
$js[] = "/sites/all/libraries/h5plibraries/H5P.InteractiveVideo-1.9/scripts/interactive-video.js";

/*


$css = array();

$css[]="/sites/all/modules/core/exp_sp_core/modules/h5p/library/styles/h5p.css";
$css[]="/sites/all/modules/core/exp_sp_core/modules/h5p/library/styles/h5p-confirmation-dialog.css";
$css[]="/sites/all/modules/core/exp_sp_core/modules/h5p/library/styles/h5p-core-button.css";
$css[]="/sites/all/libraries/h5plibraries/H5P.AdvancedText-1.1/text.css";
$css[]="/sites/all/libraries/h5plibraries/H5P.Image-1.0/image.css";
$css[]="/sites/all/libraries/h5plibraries/H5P.Video-1.2/styles/video.css";
$css[]="/sites/all/libraries/h5plibraries/FontAwesome-4.5/h5p-font-awesome.min.css";
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
$css[]="/sites/all/libraries/h5plibraries/H5P.CoursePresentation-1.9/styles/cp.css";
$css[]="/sites/all/libraries/h5plibraries/H5P.CoursePresentation-1.9/styles/summary-slide.css";
$css[]="/sites/all/libraries/h5plibraries/H5P.CoursePresentation-1.9/styles/navigation-line.css";
$css[]="/sites/all/libraries/h5plibraries/H5P.CoursePresentation-1.9/styles/slide-background.css";
$css[]="/sites/all/libraries/h5plibraries/H5P.CoursePresentation-1.9/styles/print.css";
$css[]="/sites/all/modules/core/exp_sp_core/modules/h5p/modules/h5peditor/h5p-overrides.css";

*/
$jsdata = "";
$cssdata = "";
global $base_url;

for($i = 0; $i< count($js); $i++)
{
$jsdata .= ";".file_get_contents($base_url."".$js[$i]);
}
file_put_contents($_SERVER['DOCUMENT_ROOT']."/h5pmerge/lazyloadpresentationscripts.js",$jsdata);

/*for($i = 0; $i< count($css); $i++)
{
$cssdata .= ";".file_get_contents($base_url."".$css[$i]);
}
file_put_contents($_SERVER['DOCUMENT_ROOT']."/h5pmerge/embedpresentcss.css",$cssdata);*/
$jsdata = "";
//$cssdata = "";
echo "done";

?>
