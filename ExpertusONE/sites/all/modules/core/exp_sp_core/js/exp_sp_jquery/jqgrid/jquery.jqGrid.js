function jqGridInclude()
{
    var pathtojsfiles = resource.jspath+'/exp_sp_jquery/jqgrid/js/'; // need to be ajusted
    // set include to false if you do not want some modules to be included
    var combineIntoOne = false; 
    var combinedInclude = new Array();
    var combinedIncludeURL = "combine.php?type=javascript&files=";
    var minver = true;
    var modules = [
        { include: false, incfile:'grid.locale-en.js',minfile: 'min/grid.locale-en-min.js'}, // jqGrid translation
        { include: false, incfile:'grid.base.js',minfile: 'grid.base.js'}, // jqGrid base
        { include: false, incfile:'grid.common.js',minfile: 'min/grid.common-min.js' }, // jqGrid common for editing
        { include: false, incfile:'grid.formedit.js',minfile: 'min/grid.formedit-min.js' }, // jqGrid Form editing
        { include: false, incfile:'grid.inlinedit.js',minfile: 'min/grid.inlinedit-min.js' }, // jqGrid inline editing
        { include: false, incfile:'grid.celledit.js',minfile: 'min/grid.celledit-min.js' }, // jqGrid cell editing
        { include: false, incfile:'grid.subgrid.js',minfile: 'min/grid.subgrid-min.js'}, //jqGrid subgrid
        { include: false, incfile:'grid.treegrid.js',minfile: 'min/grid.treegrid-min.js'}, //jqGrid treegrid
        { include: false, incfile:'grid.custom.js',minfile: 'grid.custom.js'}, //jqGrid custom 
        { include: false, incfile:'grid.postext.js',minfile: 'min/grid.postext-min.js'}, //jqGrid postext
        { include: false, incfile:'grid.tbltogrid.js',minfile: 'min/grid.tbltogrid-min.js'}, //jqGrid table to grid 
        { include: false, incfile:'grid.setcolumns.js',minfile: 'min/grid.setcolumns-min.js'}, //jqGrid setcolumns
        { include: false, incfile:'grid.import.js',minfile: 'min/grid.import-min.js'}, //jqGrid import
        { include: false, incfile:'jquery.fmatter.js',minfile: 'min/jquery.fmatter-min.js'}, //jqGrid formater
        { include: false, incfile:'json2.js',minfile: 'min/json2-min.js'}, //json utils
        { include: false, incfile:'JsonXml.js',minfile: 'min/JsonXml-min.js'} //xmljson utils
    ];
    var filename;
    for(var i=0;i<modules.length; i++)
    {
        if(modules[i].include === true) {
        	
        	if (minver !== true) filename = pathtojsfiles+modules[i].incfile;
        	else filename = pathtojsfiles+modules[i].minfile;
        	if (combineIntoOne !== true) {
        		if(jQuery.browser.safari || jQuery.browser.msie ) {
        			jQuery.ajax({url:filename,dataType:'script', async:false, cache: true});
        		} else {
        			IncludeJavaScript(filename);
        		}
        	} else {
        		combinedInclude[combinedInclude.length] = filename;
            }
        }
    }
	if ((combineIntoOne === true) && (combinedInclude.length>0) ) {
		var fileList = implode(",",combinedInclude);
		IncludeJavaScript(combinedIncludeURL+fileList);
	}
	function implode( glue, pieces ) {
    // http://kevin.vanzonneveld.net
    //original by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
    //example 1: implode(' ', ['Kevin', 'van', 'Zonneveld']);
    //returns 1: 'Kevin van Zonneveld'
		return ( ( pieces instanceof Array ) ? pieces.join ( glue ) : pieces );
    };
    
    function IncludeJavaScript(jsFile)
    {
        var oHead = document.getElementsByTagName('head')[0];
        var oScript = document.createElement('script');
        oScript.type = 'text/javascript;charset=utf-8';
        oScript.src = jsFile;
        oHead.appendChild(oScript);        
    };
};
jqGridInclude();