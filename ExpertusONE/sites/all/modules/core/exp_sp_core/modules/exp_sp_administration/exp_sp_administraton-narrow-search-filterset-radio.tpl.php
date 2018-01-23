<?php expDebug::dPrint('narrow search filterset radio tpl file : $radio_list = ' . print_r($radio_list, true),4);?>
<?php $radioName = $code; ?>
<div id='<?php print $html_id; ?>' class='narrow-search-filterset'>
  <ul>
    <li>
      <span id='<?php print $code; ?>_filterset_title' class='cls-show ' onclick='$("<?php print '#' . $js_object_info['init_id']; ?>").data("<?php print $js_object_info['name']; ?>").showHide("<?php print $code; ?>_filterset_title", "<?php print $code; ?>_filterset");'>
        <a class='narrow-search-filterset-heading'><?php print $title; ?></a>
      </span>
    </li>
  </ul>
  <div id='<?php print $code; ?>_filterset'>
  <?php foreach ($radio_list as $code => $name): ?>
    <?php 
     $nameTitle = titleController('NARROW-SEARCH-FILTERSET-RADIO-LABEL', t($name),20);  /* Changed by ganeshbabuv for #0078270*/ ?> 
    <?php $radioSelectedClass =  (isset($radio_checked_list[$code]))? 'narrow-search-filterset-radio-selected': 'narrow-search-filterset-radio-unselected'; ?>
    <?php $labelSelectedClass =  (isset($radio_checked_list[$code]))? 'narrow-search-filterset-item-label-selected': 'narrow-search-filterset-item-label-unselected'; ?>
    <?php $checkedStatus =  (isset($radio_checked_list[$code]))? 'checked': ''; ?>
    <div class='narrow-search-filterset-radio-container' style='display:block'>
      <label for='radio_<?php print $code; ?>' class='<?php print $radioSelectedClass; ?>' >
        <input id='radio_<?php print $code; ?>' class='narrow-search-filterset-radio' name='<?php print $radioName; ?>' type='radio' <?php print $checkedStatus; ?> value="<?php print $code; ?>" onClick='$("#root-admin").data("narrowsearch").showHideCourseClass(this.value); $("<?php print '#' . $js_object_info['init_id']; ?>").data("<?php print $js_object_info['name']; ?>").narrowSearch();'/>
      </label>
      <label for='radio_<?php print $code; ?>' class='narrow-search-filterset-item-label <?php print $labelSelectedClass;?>' title='<?php print t($name); ?>'>
        <?php print $nameTitle;  /* Changed by ganeshbabuv for #0078270*/ ?>
      </label>
    </div>
  <?php endforeach; ?>
  </div> 
</div>
<script language="Javascript" type="text/javascript">$("#root-admin").data("narrowsearch").showHideCourseClass(this.value);</script>