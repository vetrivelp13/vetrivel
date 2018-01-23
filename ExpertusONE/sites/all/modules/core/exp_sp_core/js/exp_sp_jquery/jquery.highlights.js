(function($){
	jQuery.fn.highlights = function(params){
		return this.each(function(){
			$(this).html($.fn.renderHighLights(params));
		});
	};
	
	$.fn.renderHighLights = function(params){
		var html='<div id="highlights" style="width:'+ params.width +'px">';
		html += '<fieldset><legend align="left">Highlights</legends>';
		html += '<table>';
		for(var i=0;i<params.items.length;i++){
			html += "<tr><td width='70%'>"+ params.items[i].name +"</td><td width='30%' style='color:#000000'>"+params.items[i].value+"</td></tr>";
		}
		html += "</table>";
		html += "</fieldset></div>";
		return html;
	};

})(jQuery);

