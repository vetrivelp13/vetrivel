(function($) {

$.widget("ui.announcements", {

	_init: function() {
		try{
		this.options.inc 		= 1;
		this.options.announcementInterval;
		this.options.totalRecords	= Drupal.settings.exp_sp_announcement.totalRecords; // Total no of announcement posted count
		this.options.block_no_of_display = Drupal.settings.exp_sp_announcement.exp_sp_announcement_block_no_of_display; //VJ
		this.options.block_seconds_pager = Drupal.settings.exp_sp_announcement.exp_sp_announcement_block_seconds_pager;
		$.ui.announcements.pause_announcement = 0;
		$.ui.announcements.short_txt_def_height = $('.announce-desc').height();
		$.ui.announcements.ann_img_def_height = $('.announcement_img_div').height();
		$.ui.announcements.ann_img_def_width = $('.announcement_img_div').width();
		if (this.options.totalRecords > 1) {
			this._showAnnouncement(true);
		}
		// Pause and release the announcement on mouseover and mouse out
	    $("#announcement_details").mouseover(function(){
	    	$.ui.announcements.pause_announcement = 1;
	    });
	    $("#announcement_details").mouseout(function(){
	    	$.ui.announcements.pause_announcement = 0;
	    });
		}catch(e){
			// to do
		}
	},	
	_showAnnouncement : function(init) {
		try{
		var self = this;
		$('.button').click(function(){
			toggleAnnouncementDesc(2);
			var counter 	= $(this).attr('id');
			$('#announce_'+ counter).show();
            $('#circle_' + counter).removeClass('announce_page_disable');
            $('#circle_' + counter).addClass('announce_page_enable');
			
            self.options.inc = parseInt(counter)-1;			
            self.autoPaginationCallType1(self.options.block_no_of_display);
	     });
		self._paginationIntervalType1();
		}catch(e){
			// to do
		}
    },
    autoPaginationCallType1 : function(numrec) {
    	try{
        for ( var i = 1; i <= numrec; i++) {
            $('#announce_'+i).hide();
            $('#circle_' + i).removeClass('announce_page_enable');
            $('#circle_' + i).addClass('announce_page_disable');
        }
    	if(this.options.inc<this.options.totalRecords) {
			this.options.inc++;
		} else {
			this.options.inc = 1;
		}
        $('#announce_'+this.options.inc).show();
        $('#circle_' + this.options.inc).removeClass('announce_page_disable');
        $('#circle_' + this.options.inc).toggleClass('announce_page_enable');
        //this.paginationInterval_type1();
        var currAnnId = this.options.inc-1;
        var mainDivHeight = $('#'+currAnnId+ '_announcement_img_div').height();
        var newDivHeight = $('#'+currAnnId+'_announce-desc').height() - $('#'+currAnnId+'_announce-desc').height(); 
        var newwidth = $('#'+currAnnId+ '_announcemnt-img').width() - 20;
		$('.announce-desc').css('width',newwidth+'px');
		if(this.options.totalRecords > 1){
			$('.announce-desc-pager').css('width',((this.options.totalRecords * 23 ) + 35 ) + 'px');	
		}
		else {
			$('.announce-desc-pager').css('width','0px');
		}
		
        newDivHeight += mainDivHeight; 
        $('.announcement_img_div').css('height',newDivHeight+'px');
        
        $('.homebanner-limit-title').trunk8(trunk8.homebanner_title);
        $('.homebanner-limit-desc').trunk8(trunk8.homebanner_desc);
    	}catch(e){
			// to do
		}
    },
    _paginationIntervalType1 : function() {
    	try{
    	var self = this;
    	clearInterval(self.options.announcementInterval);    
    	//self.options.announcementInterval 	= setInterval("$.ui.announcements.autoPaginationCallType1("+self.options.block_no_of_display+")",self.options.block_seconds_pager);
    	self.options.announcementInterval 	= setInterval(function() {
    		if($.ui.announcements.pause_announcement == 0){
    			toggleAnnouncementDesc(2);    			
    			self.autoPaginationCallType1(self.options.block_no_of_display );
    		}
		}, self.options.block_seconds_pager);
    	}catch(e){
			// to do
		}
    }
    
});

$.extend($.ui.announcements, {
	version: '1.7.3',
	getter: 'length',
	defaults: {
		ajaxOptions: null,
		cache: false,
		cookie: null, // e.g. { expires: 7, path: '/', domain: 'jquery.com', secure: true }
		collapsible: false,
		disabled: [],
		event: 'click',
		fx: null, // e.g. { height: 'toggle', opacity: 'toggle', duration: 200 }
		idPrefix: 'ui-tabs-',
		panelTemplate: '<div></div>',
		spinner: '<em>Loading&#8230;</em>',
		tabTemplate: '<li><a href="#{href}"><span>#{label}</span></a></li>'
	}
});

/*
 * Tabs Extensions
 */

})(jQuery);
$(function() {
	try{
	$( "#announcement_type1" ).announcements();
	}catch(e){
		// to do
	}
});

function announcementPopup(title, data){
	try{
    $('#announcement-popup').remove();
    var html = '';
    html+='<div id="announcement-popup" class="announcement-popup">'+data+'</div>';
    $("body").append(html);
    $("#announcement-popup").dialog({
        position:[(getWindowWidth()-600)/2,100],
        bgiframe: true,
        width:600,
        resizable:false,
        modal: true,
        title: SMARTPORTAL.t(title),
        closeOnEscape: false,
        draggable:true,
        overlay:
         {
           opacity: 0.5,
           background: "black"
         }
    });	
    $("#announcement-popup").show();	
	$('.ui-dialog-titlebar-close').click(function(){
        $("#announcement-popup").remove();
    });
	}catch(e){
		// to do
	}
}

function toggleAnnouncementDesc(id, opt){
	try{
	$('.announcement-short-desc .short-desc-toggle').hide();
	$('.announcement-short-desc .long-desc-toggle').hide();
	if(opt == 1){
		$('#'+id+'_announce-desc').removeClass('announce-desc-mxhi');
		$.ui.announcements.short_txt_def_height = $('#'+id+'_announce-desc').height();
		$.ui.announcements.ann_img_def_height = $('#'+id+ '_announcement_img_div').height();
		$('.announcement-short-desc #'+id+'_long-desc-toggle').show();
		var shortTxtDiff = $('#'+id+'_announce-desc').height() - $.ui.announcements.short_txt_def_height;
		if(shortTxtDiff > 0){
			var ann_new_height =  $.ui.announcements.ann_img_def_height + shortTxtDiff;
			$('.announcement_img_div').css('height', ann_new_height+'px');
		}
	} else {
		$('#'+id+'_announce-desc').addClass('announce-desc-mxhi');
		$('.announcement_img_div').css('height', $.ui.announcements.ann_img_def_height+'px');
		
		$('.announcement-short-desc .short-desc-toggle').show();
	}
	}catch(e){
		// to do
	}
}
