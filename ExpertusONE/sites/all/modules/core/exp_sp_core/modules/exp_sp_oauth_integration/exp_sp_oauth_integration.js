$(document).ready(function(){
	$("#edit-client-type").change(function(){
		var scopeChange = $( "#edit-client-type option:selected" ).val();
		$('#edit-client-url').val('');
		if(scopeChange == 'API' || scopeChange == 'DL'){
			var base_url = window.location.origin; // Base url
			// var host = window.location.host; // Host
			$('#edit-client-url').val(base_url);
			$("#edit-client-url").prop("readonly",true);
		}else{
			$("#edit-client-url").prop("readonly",false);
		}
	});
});