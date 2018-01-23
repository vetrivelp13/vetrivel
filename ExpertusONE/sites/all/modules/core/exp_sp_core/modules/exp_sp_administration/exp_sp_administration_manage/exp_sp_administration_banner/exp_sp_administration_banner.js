( function($) {
  Drupal.behaviors.initImageInBannerAdmin =  {
      attach: function (context, settings) {
    	  try{
        $('#banner-image-field-wrapper:not(.banner-admin-initialized)').addClass('banner-admin-initialized').each(function () {
        	try{
          // Set the mouseup event handler
          $fileUploaderState = $('#banner-file-uploader-state').val();
          if ($fileUploaderState == 'show') {
            $('#banner-file-image-wrapper').hide();
            $('#banner-file-uploader-wrapper').show();
          }
          else {
            $('#banner-file-uploader-wrapper').hide();
            $('#banner-file-image-wrapper').show();
          }
        	}catch(e){
    			// to do
    		}
        }); // end each

        // Initialize Access link in banner admin form to launch qtip
        $('#exp-sp-administration-banner-addedit-form:not(.banner-admin-initialized)').addClass('banner-admin-initialized').each(function () {
         try{	
          var accessPopupLabel = $('#access-qtip-link-label').val();
          var entityId = $('#banner-basic-addedit-form #id').val();
          var emptyId = $('#banner-basic-addedit-form #empty_id').val();
          if (entityId == null || entityId == '') {
            entityId = 0;
          }
          else {
            emptyId = 0;
          }
          var entityType = 'cre_sys_obt_anm';
          var qtipIdInit = entityId + '_' + entityType;
          var popupDispId = 'qtip_visible_disp_' + qtipIdInit;
          var catalogVisibleId = 'qtipBannerAccessqtip_visible_disp_' + qtipIdInit;
          var qtipOptAccessObj  = {
                  'entityId' : entityId
                  , 'entityType' : entityType
                  , 'url' : 'administration/catalogaccess/qtip_visible_disp_'+qtipIdInit+'/' + entityId + '/' + entityType + '/' + emptyId
                  , 'popupDispId' : popupDispId
                  , 'catalogVisibleId' : catalogVisibleId
                  , 'wBubble' : (Drupal.settings.ajaxPageState.theme == 'expertusoneV2')?770:750
                  , 'hBubble' : 'auto'
                  , 'tipPosition' : 'bottomCorner'
                  , 'qtipClass' : 'admin-qtip-access-parent'
               };
            
          var accessLinkHTML = 
                  '<div class="crs-tab-titles-container">' +
                    '<div class="access-tab-icon"></div>' +
                    '<div id="' + popupDispId + '" class="tab-title">' + accessPopupLabel + '</div>' +
                  '</div>';
          $(this).append(accessLinkHTML);
          $('#root-admin').data('narrowsearch').getBubblePopup(qtipOptAccessObj);
         }catch(e){
 			// to do
 		}
        });
    	  }catch(e){
  			// to do
  		}
     } // end attach
  };
})(jQuery);