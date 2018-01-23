//Initialize and set behaviour for question_type field
Drupal.behaviors.expAdminSurveyQuestionType =  {
  attach: function (context, settings) {
    $('.addedit-edit-question_type:not(.question_type-initialized)').addClass('question_type-initialized').each(function () {
      var qType = $(this).val();
      if (qType == '0' || qType == 0) {
        $(this).removeClass('select-normal-text');
        $(this).addClass('select-greyed-out-text');
      }
      else {
        $(this).removeClass('select-greyed-out-text');
        $(this).addClass('select-normal-text');
      }
      
      if ($(this).is(':disabled')) {
        $(this).removeClass('select-normal-text');
        $(this).addClass('select-greyed-out-text');
      }
      else {
        $(this).removeClass('select-greyed-out-text');
        $(this).addClass('select-normal-text');        
      }
      
      $(this).change(function(event) {
        var qType = $(this).val();
        if (qType == '0' || qType == 0) {
          $(this).removeClass('select-normal-text');
          $(this).addClass('select-greyed-out-text');
        }
        else {
          $(this).removeClass('select-greyed-out-text');
          $(this).addClass('select-normal-text');
        }
      });  
   });
  } // end attach()
};
