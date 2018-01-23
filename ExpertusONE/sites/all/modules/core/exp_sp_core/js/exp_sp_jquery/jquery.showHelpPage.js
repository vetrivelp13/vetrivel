(function($) {
	jQuery.fn.showHelpPage = function(aRegion,aKeyName){		
		return this.each(function(){
			var title = '';			
			//alert('inSIDE PLUGIN___'+themepath);			
			if(aRegion == 'top'){				
				title = SMARTPORTAL.t("User Guide");	
				var html = '<a class="userGuide" href="#" Key="'+aKeyName+'">'+title+'</a>';
				$(this).html(html);
			} else if (aRegion = 'screen'){
				title = SMARTPORTAL.t("How To");				
				//var html='<img style="margin-top:-2px;cursor:pointer;" title="'+title+'" alt="'+title+'" class="userGuide" src="'+themepath+'/images/exp_sp_icon_20x20_Help.gif" Key="'+aKeyName+'" >';
				// ORIGINAL REPLACED INLINE STYLE TO CSS
				var html='<img title="'+title+'" alt="'+title+'" class="userGuide" src="'+themepath+'/images/exp_sp_icon_20x20_Help.gif" Key="'+aKeyName+'" >';
				$(this).html(html);
			} 				
		});
	};
	$(document).ready(function() {
		$('.userGuide').live('click', function(){
		
			var html = "<div id='divPUConstruction' style='width: 400px; text-align: center; padding-top: 40px;'>Under Construction.</div>";
			$('#divPUConstruction').remove();

			$('body').append(html);
			$('#divPUConstruction').dialog({
				width: 400,
				modal: true,	
				resizable:false,
				title:'Under Construction'
			});
			
			$('.ui-dialog').wrap("<div class='portal-ui-element'></div>");
			$('.ui-widget-overlay').wrap("<div class='portal-ui-element'></div>");
			$('#divPUConstruction').css({'width':'400px'});
		});
	});		
})(jQuery);

