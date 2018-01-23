// $Id: form.js,v 1.1 2011-01-03 13:10:05 srprabhu Exp $

Drupal.behaviors.multiselectSelector = function() {
  // Automatically selects the right radio button in a multiselect control.
  $('.multiselect select:not(.multiselectSelector-processed)')
    .addClass('multiselectSelector-processed').change(function() {
      $('.multiselect input:radio[value="'+ this.id.substr(5) +'"]')
        .attr('checked', true);
  });
};
