;
var H5P_DEVELOPMENT_MODE = 0;




/**
 * This file contains helper functions for the editor.
 */

// Use resources set in parent window
var ns = H5PEditor = window.parent.H5PEditor;
ns.$ = H5P.jQuery;

// Load needed resources from parent.
H5PIntegration = window.parent.H5PIntegration;

/**
 * Keep track of our widgets.
 */
ns.widgets = {};

/**
 * Keeps track of which semantics are loaded.
 */
ns.loadedSemantics = {};

/**
 * Keeps track of callbacks to run once a semantic gets loaded.
 */
ns.semanticsLoaded = {};

/**
 * Indiciates if the user is using Internet Explorer.
 */
ns.isIE = navigator.userAgent.match(/; MSIE \d+.\d+;/) !== null;

/**
 * Loads the given library, inserts any css and js and
 * then runs the callback with the samantics as an argument.
 *
 * @param {string} libraryName
 *  On the form machineName.majorVersion.minorVersion
 * @param {function} callback
 * @returns {undefined}
 */
ns.loadLibrary = function (libraryName, callback) {
  switch (ns.loadedSemantics[libraryName]) {
    default:
      // Get semantics from cache.
      callback(ns.loadedSemantics[libraryName]);
      break;

    case 0:
      // Add to queue.
      ns.semanticsLoaded[libraryName].push(callback);
      break;

    case undefined:
      // Load semantics.
      ns.loadedSemantics[libraryName] = 0; // Indicates that others should queue.
      ns.semanticsLoaded[libraryName] = []; // Other callbacks to run once loaded.
      var library = ns.libraryFromString(libraryName);
	   
	   console.log("new libraryName:"+libraryName);
      var url = ns.getAjaxUrl('libraries', library);

      // Add content language to URL
      if (ns.contentLanguage !== undefined) {
        url += (url.indexOf('?') === -1 ? '?' : '&') + 'language=' + ns.contentLanguage;
      }

		//h5pcustomize
		  ns.$("#h5peditor-library").hide();
		  ns.$("#edit-actions").hide();
		  console.log("libraryName:"+libraryName);
		  
		if(	  libraryName == "H5P.Text 1.1" 			|| libraryName == "H5P.Blanks 1.4" ||  libraryName == "H5P.Nil 1.0" ||
			 libraryName == "H5P.Link 1.1" 			||  libraryName == "H5P.Image 1.0" 			|| libraryName == "H5P.SingleChoiceSet 1.3" ||
			 libraryName ==  "H5P.MarkTheWords 1.5" ||  libraryName == "H5P.GoToQuestion 1.0" 	|| libraryName ==  "H5P.DragText 1.4" || libraryName ==  "H5P.AdvancedText 1.1"
			 )
		{
		 	var libraryData = window.parent.INTERACTIONS_META_DATA[libraryName];
		  	ns.librarySuccessCallback(libraryData,libraryName,callback); //avoid ajax request always
		}
		else if(H5P_DEVELOPMENT_MODE == 0 && libraryName == "H5P.InteractiveVideo 1.9" )
		{
		  	var libraryData = window.parent.H5PVIDEO_RESOURCES;
		  	ns.librarySuccessCallback(libraryData,libraryName,callback); //avoid ajax request in performance mode
		}else if(H5P_DEVELOPMENT_MODE == 0 && libraryName == "H5P.CoursePresentation 1.9")
		{
		 	var libraryData = window.parent.H5PCOURSEPRE_RESOURCES;
		  	ns.librarySuccessCallback(libraryData,libraryName,callback); //avoid ajax request in performance mode
		}
		else
		{
		  
      // Fire away!
      ns.$.ajax({
        url: url+"&ts="+new Date().getTime()+"&H5P_DEVELOPMENT_MODE="+H5P_DEVELOPMENT_MODE,
        success: function (libraryData) {
         var st = new Date().getTime();
         console.log("Resources loading time start:"+st);	
			
	
		 if(libraryData == undefined || libraryData == 'undefined')
		 	return "";
		 if(H5P_DEVELOPMENT_MODE == 0 && libraryName == "H5P.InteractiveVideo 1.9")
		 {
		 	libraryData = window.parent.H5PVIDEO_RESOURCES;
		 }
		 else if(H5P_DEVELOPMENT_MODE ==0 && libraryName == "H5P.CoursePresentation 1.9")
		 {
		 	libraryData = window.parent.H5PCOURSEPRE_RESOURCES;
		 }
          var semantics = libraryData.semantics;
         
         
          if (libraryData.language !== null && libraryData.language != undefined && libraryData.language != "undefined") {
            var language = JSON.parse(libraryData.language);
            semantics = ns.$.extend(true, [], semantics, language.semantics);
          }
          libraryData.semantics = semantics;
          ns.loadedSemantics[libraryName] = libraryData.semantics;
		
          // Add CSS.
          if (libraryData.css !== undefined) {
            var css = '';
            
            /* if(H5P_DEVELOPMENT_MODE == 0 && libraryName == "H5P.InteractiveVideo 1.9"){
             		ns.$("#video_merged_cssloaded").remove();
             		ns.$('head').append('<style type="text/css" id="video_merged_cssloaded">' + window.parent.$("#video_merged_css").attr("value")+ '</style>');
         	 }
          	 else if(H5P_DEVELOPMENT_MODE == 0 &&  libraryName == "H5P.CoursePresentation 1.9"){
          	  		ns.$("#coursepre_merged_cssloaded").remove();
             		ns.$('head').append('<style type="text/css"  id="coursepre_merged_cssloaded">' + window.parent.$("#coursepre_mergecss").attr("value")+ '</style>');
         	 }
          	 else */ 
          	 {
          
           	 	for (var path in libraryData.css) {
              if ((H5P_DEVELOPMENT_MODE ==1 || (libraryName != "H5P.InteractiveVideo 1.9" && libraryName != "H5P.CoursePresentation 1.9")) && !H5P.cssLoaded(path)) {
              
                css += libraryData.css[path];
                H5PIntegration.loadedCss.push(path);
              }
              else if(libraryName == "H5P.InteractiveVideo 1.9" || libraryName == "H5P.CoursePresentation 1.9")
              {
              	css += libraryData.css[path];
              	
              }
            }
            if (css && (H5P_DEVELOPMENT_MODE ==1 || (libraryName != "H5P.InteractiveVideo 1.9" && libraryName != "H5P.CoursePresentation 1.9"))) {
              ns.$('head').append('<style type="text/css">' + css + '</style>');
            }else if(libraryName == "H5P.InteractiveVideo 1.9" || libraryName == "H5P.CoursePresentation 1.9")
            {
			  ns.$("#loadedcss").remove();
              ns.$('head').append('<style id="loadedcss" type="text/css">' + css + '</style>');
            }
            
            }
          }
          
          
          // Add JS.
          if (libraryData.javascript !== undefined) {
            var js = '';
           /*if(H5P_DEVELOPMENT_MODE == 0 &&  libraryName == "H5P.InteractiveVideo 1.9"){
           		js += window.parent.$("#video_merged_js").attr("value");
           		 if (js) {
             		 eval.apply(window, [js]);
            	 }
         	}
         	 else if(H5P_DEVELOPMENT_MODE == 0 &&  libraryName == "H5P.CoursePresentation 1.9"){
           		js += window.parent.$("#coursepre_mergedjs1").attr("value");
           		js += window.parent.$("#coursepre_mergedjs2").attr("value");
           		js += window.parent.$("#coursepre_mergedjs3").attr("value");
           		 if (js) {
             		 eval.apply(window, [js]);
            	 }

         	 }
             else */
            {
            	if(H5PIntegration.loadedJs == undefined)
            		H5PIntegration.loadedJs = new Array();
            for (var path in libraryData.javascript) {
            	//console.log("JS11:"+path);
            	
              if ((H5P_DEVELOPMENT_MODE == 1 ||(libraryName != "H5P.InteractiveVideo 1.9" && libraryName != "H5P.CoursePresentation 1.9")) && !H5P.jsLoaded(path)) 
              {
                   js += libraryData.javascript[path];
 	               H5PIntegration.loadedJs.push(path);
              }
              else if(libraryName == "H5P.InteractiveVideo 1.9" || libraryName == "H5P.CoursePresentation 1.9")
              {
                js += libraryData.javascript[path];
              }
            }
            var et = new Date().getTime();
        	 console.log("Resources loading time end:"+et+" in difference:"+(et-st));	
		
            if (js && (H5P_DEVELOPMENT_MODE ==1 || ( libraryName != "H5P.InteractiveVideo 1.9" && libraryName != "H5P.CoursePresentation 1.9"))) {
              eval.apply(window, [js]);
            }else if(libraryName == "H5P.InteractiveVideo 1.9" || libraryName == "H5P.CoursePresentation 1.9")
            {
            	ns.$("#loadedscript").remove();
            	ns.$("head").append("<script id='loadedscript'>"+js+"</script>");
			
            }
            var et1 = new Date().getTime();
        	console.log("Resources processing time "+et1+" in difference:"+(et1-et));	
            }
          }
          callback(libraryData.semantics);
          
          // Run queue.
          for (var i = 0; i < ns.semanticsLoaded[libraryName].length; i++) {
            ns.semanticsLoaded[libraryName][i](libraryData.semantics);
          }
           /* hide unnecessary h5p ui components behavior settings and etc...*/
          if(ns.$(".coursepresentation").size() > 0)
          {
            
          	ns.$(".coursepresentation").next().hide();
          	var children = ns.$(".coursepresentation").parent().parent().children();
          	ns.$(children[1]).css("display","none");
          	
          	
          }
		 var et2 = new Date().getTime();
        console.log("After callback "+et2+" in difference:"+(et2-et1));
          
          
        },
        error: function(jqXHR, textStatus, errorThrown) {
          if (window['console'] !== undefined) {
            console.log('Ajax request failed');
            console.log(jqXHR);
            console.log(textStatus);
            console.log(errorThrown);
          }
        },
        dataType: 'json'
      });
      }
  }
};

/**
 * Recursive processing of the semantics chunks.
 *
 * @param {array} semanticsChunk
 * @param {object} params
 * @param {jQuery} $wrapper
 * @param {mixed} parent
 * @returns {undefined}
 */
ns.processSemanticsChunk = function (semanticsChunk, params, $wrapper, parent) {
  var ancestor;
  //console.log("semanticsChunk:"+JSON.stringify(semanticsChunk)+"-params:"+params+"-wrapper:"+JSON.stringify($wrapper)+"-parent:"+JSON.stringify(parent));
  parent.children = [];

  //h5pcustomize
  if(semanticsChunk == undefined)
    return "";
  if (parent.passReadies === undefined) {
    throw 'Widget tried to run processSemanticsChunk without handling ready callbacks. [field:' + parent.field.type + ':' + parent.field.name + ']';
  }

  if (!parent.passReadies) {
    // If the parent can't pass ready callbacks we need to take care of them.
    parent.readies = [];
  }
  for (var i = 0; i < semanticsChunk.length; i++) {
    var field = semanticsChunk[i];

    // Check generic field properties.
    if (field.name === undefined) {
      throw ns.t('core', 'missingProperty', {':index': i, ':property': 'name'});
    }
    if (field.type === undefined) {
      throw ns.t('core', 'missingProperty', {':index': i, ':property': 'type'});
    }

    // Set default value.
    if (params[field.name] === undefined && field['default'] !== undefined) {
      params[field.name] = field['default'];
    }

    var widget = ns.getWidgetName(field);
	//console.log("widget name:"+widget);
    // TODO: Remove later, this is here for debugging purposes.
    if (ns.widgets[widget] === undefined) {
      //console.log("field.type:"+field.type+"="+field.name);
    
      $wrapper.append('<div>[field:' + field.type + ':' + widget + ':' + field.name + ']</div>');
      continue;
    }

    // Add common fields to bottom of form.
    if (field.common !== undefined && field.common) {
      if (ancestor === undefined) {
        ancestor = ns.findAncestor(parent);
      }

      ns.addCommonField(field, parent, params, ancestor);
      continue;
    }

    var fieldInstance = new ns.widgets[widget](parent, field, params[field.name], function (field, value) {
      if (value === undefined) {
        delete params[field.name];
      }
      else {
        params[field.name] = value;
      }
    });
    
   // console.log("wrapper:"+JSON.stringify($wrapper)+"-fieldInstance:"+JSON.stringify(fieldInstance));
    fieldInstance.appendTo($wrapper);
    parent.children.push(fieldInstance);
  }

  if (!parent.passReadies) {
    // Run ready callbacks.
    for (var i = 0; i < parent.readies.length; i++) {
      parent.readies[i]();
    }
    delete parent.readies;
  }
};

/**
 * Add a field to the common container.
 *
 * @param {object} field
 * @param {object} parent
 * @param {object} params
 * @param {object} ancestor
 * @returns {undefined}
 */
ns.addCommonField = function (field, parent, params, ancestor) {
  var commonField;
  if (ancestor.commonFields[parent.library] === undefined) {
    ancestor.commonFields[parent.library] = {};
  }

  if (ancestor.commonFields[parent.library][field.name] === undefined) {
    var widget = ns.getWidgetName(field);
    ancestor.commonFields[parent.library][field.name] = {
      instance: new ns.widgets[widget](parent, field, params[field.name], function (field, value) {
          for (var i = 0; i < commonField.setValues.length; i++) {
            commonField.setValues[i](field, value);
          }
        }),
      setValues: [],
      parents: []
    };
  }

  commonField = ancestor.commonFields[parent.library][field.name];
  commonField.parents.push(ns.findLibraryAncestor(parent));
  commonField.setValues.push(function (field, value) {
    if (value === undefined) {
      delete params[field.name];
    }
    else {
      params[field.name] = value;
    }
  });

  if (commonField.setValues.length === 1) {
    ancestor.$common.parent().removeClass('hidden');
    commonField.instance.appendTo(ancestor.$common);
    commonField.params = params[field.name];
  }
  else {
    params[field.name] = commonField.params;
  }

  parent.children.push(commonField.instance);
};

/**
 * Find the nearest library ancestor. Used when adding commonfields.
 *
 * @param {object} parent
 * @returns {ns.findLibraryAncestor.parent|@exp;ns@call;findLibraryAncestor}
 */
ns.findLibraryAncestor = function (parent) {
  if (parent.parent === undefined || parent.field.type === 'library') {
    return parent;
  }
  return ns.findLibraryAncestor(parent.parent);
};

/**
 * Find the nearest ancestor which handles commonFields.
 *
 * @param {type} parent
 * @returns {@exp;ns@call;findAncestor|ns.findAncestor.parent}
 */
ns.findAncestor = function (parent) {
  if (parent.commonFields === undefined) {
    return ns.findAncestor(parent.parent);
  }
  return parent;
};

/**
 * Call remove on the given children.
 *
 * @param {Array} children
 * @returns {unresolved}
 */
ns.removeChildren = function (children) {
  if (children === undefined) {
    return;
  }

  for (var i = 0; i < children.length; i++) {
    // Common fields will be removed by library.
    if (children[i].field === undefined ||
        children[i].field.common === undefined ||
        !children[i].field.common) {
      children[i].remove();
    }
  }
};

/**
 * Find field from path.
 *
 * @param {String} path
 * @param {Object} parent
 * @returns {@exp;ns.Form@call;findField|Boolean}
 */
ns.findField = function (path, parent) {
  if (typeof path === 'string') {
    path = path.split('/');
  }

  if (path[0] === '..') {
    path.splice(0, 1);
    return ns.findField(path, parent.parent);
  }
  if (parent.children) {
    for (var i = 0; i < parent.children.length; i++) {
      if (parent.children[i].field.name === path[0]) {
        path.splice(0, 1);
        if (path.length) {
          return ns.findField(path, parent.children[i]);
        }
        else {
          return parent.children[i];
        }
      }
    }
  }

  return false;
};

/**
 * Follow a field and get all changes to its params.
 *
 * @param {Object} parent The parent object of the field.
 * @param {String} path Relative to parent object.
 * @param {Function} callback Gets called for params changes.
 * @returns {undefined}
 */
ns.followField = function (parent, path, callback) {
  if (path === undefined) {
    return;
  }

  // Find field when tree is ready.
  parent.ready(function () {
    var def;

    if (path instanceof Object) {
      // We have an object with default values
      def = H5P.cloneObject(path);

      if (path.field === undefined) {
        callback(path, null);
        return; // Exit if we have no field to follow.
      }

      path = def.field;
      delete def.field;
    }

    var field = ns.findField(path, parent);

    if (!field) {
      throw ns.t('core', 'unknownFieldPath', {':path': path});
    }
    if (field.changes === undefined) {
      throw ns.t('core', 'noFollow', {':path': path});
    }

    var params = (field.params === undefined ? def : field.params);
    callback(params, field.changes.length + 1);

    field.changes.push(function () {
      var params = (field.params === undefined ? def : field.params);
      callback(params);
    });
  });
};

/**
 * Create HTML wrapper for error messages.
 *
 * @param {String} message
 * @returns {String}
 */
ns.createError = function (message) {
  return '<p>' + message + '</p>';
};

/**
 * Create HTML wrapper for field items.
 *
 * @param {String} type
 * @param {String} content
 * @returns {String}
 */
ns.createItem = function (type, content, description) {
  var html = '<div class="field ' + type + '">' + content + '<div class="h5p-errors"></div>';
  if (description !== undefined) {
    html += '<div class="h5peditor-field-description">' + description + '</div>';
  }
  return html + '</div>';
};

/**
 * Create HTML for select options.
 *
 * @param {String} value
 * @param {String} text
 * @param {Boolean} selected
 * @returns {String}
 */
ns.createOption = function (value, text, selected) {
  return '<option value="' + value + '"' + (selected !== undefined && selected ? ' selected="selected"' : '') + '>' + text + '</option>';
};

/**
 * Create HTML for text input.
 *
 * @param {String} description
 * @param {String} value
 * @param {Integer} maxLength
 * @returns {String}
 */
ns.createText = function (value, maxLength, placeholder) {
  var html = '<input class="h5peditor-text" type="text"';

  if (value !== undefined) {
    html += ' value="' + value + '"';
  }

  if (placeholder !== undefined) {
    html += ' placeholder="' + placeholder + '"';
  }

  html += ' maxlength="' + (maxLength === undefined ? 255 : maxLength) + '"/>';

  return html;
};

/**
 * Create a label to wrap content in.
 *
 * @param {Object} field
 * @param {String} content
 * @returns {String}
 */
ns.createLabel = function (field, content) {
  var html = '<label class="h5peditor-label-wrapper">';

  if (field.label !== 0) {
    html += '<span class="h5peditor-label' + (field.optional ? '' : ' h5peditor-required') + '">' + (field.label === undefined ? field.name : field.label) + '</span>';
  }

  return html + content + '</label>';
};

/**
 * Check if any errors has been set.
 *
 * @param {jQuery} $errors
 * @param {jQuery} $input
 * @param {String} value
 * @returns {mixed}
 */
ns.checkErrors = function ($errors, $input, value) {
  if ($errors.children().length) {
    $input.keyup(function (event) {
      if (event.keyCode === 9) { // TAB
        return;
      }
      $errors.html('');
      $input.unbind('keyup');
    });

    return false;
  }
  return value;
};

/**
 * @param {object} library
 *  with machineName, majorVersion and minorVersion params
 * @returns {string}
 *  Concatinated version of the library
 */
ns.libraryToString = function (library) {
  return library.name + ' ' + library.majorVersion + '.' + library.minorVersion;
};

/**
 * TODO: Remove from here, and use from H5P instead(move this to the h5p.js...)
 *
 * @param {string} library
 *  library in the format machineName majorVersion.minorVersion
 * @returns
 *  library as an object with machineName, majorVersion and minorVersion properties
 *  return false if the library parameter is invalid
 */
ns.libraryFromString = function (library) {
  var regExp = /(.+)\s(\d+)\.(\d+)$/g;
  var res = regExp.exec(library);
  if (res !== null) {
    return {
      'machineName': res[1],
      'majorVersion': res[2],
      'minorVersion': res[3]
    };
  }
  else {
    H5P.error('Invalid ‚àö¬∫berName');
    return false;
  }
};

/**
 * Helper function for detecting field widget.
 *
 * @param {Object} field
 * @returns {String} Widget name
 */
ns.getWidgetName = function (field) {
  return (field.widget === undefined ? field.type : field.widget);
};

/**
 * Mimics how php's htmlspecialchars works (the way we uses it)
 */
ns.htmlspecialchars = function(string) {
  return string.toString().replace(/</g, '&lt;').replace(/>/g, '&gt;').replace(/'/g, '&#039;').replace(/"/g, '&quot;');
};

ns.librarySuccessCallback = function(libraryData,libraryName,callback){

         var st = new Date().getTime();
         console.log("Resources loading time start:"+st);	
			
	
		 if(libraryData == undefined || libraryData == 'undefined')
		 	return "";
		 if(H5P_DEVELOPMENT_MODE == 0 && libraryName == "H5P.InteractiveVideo 1.9")
		 {
		 	libraryData = window.parent.H5PVIDEO_RESOURCES;
		 }
		 else if(H5P_DEVELOPMENT_MODE ==0 && libraryName == "H5P.CoursePresentation 1.9")
		 {
		 	libraryData = window.parent.H5PCOURSEPRE_RESOURCES;
		 }
          var semantics = libraryData.semantics;
         
         
          if (libraryData.language !== null && libraryData.language != undefined && libraryData.language != "undefined") {
            var language = JSON.parse(libraryData.language);
            semantics = ns.$.extend(true, [], semantics, language.semantics);
          }
          libraryData.semantics = semantics;
          ns.loadedSemantics[libraryName] = libraryData.semantics;
		
          // Add CSS.
          if (libraryData.css !== undefined) {
            var css = '';
            
            /* if(H5P_DEVELOPMENT_MODE == 0 && libraryName == "H5P.InteractiveVideo 1.9"){
             		ns.$("#video_merged_cssloaded").remove();
             		ns.$('head').append('<style type="text/css" id="video_merged_cssloaded">' + window.parent.$("#video_merged_css").attr("value")+ '</style>');
         	 }
          	 else if(H5P_DEVELOPMENT_MODE == 0 &&  libraryName == "H5P.CoursePresentation 1.9"){
          	  		ns.$("#coursepre_merged_cssloaded").remove();
             		ns.$('head').append('<style type="text/css"  id="coursepre_merged_cssloaded">' + window.parent.$("#coursepre_mergecss").attr("value")+ '</style>');
         	 }
          	 else */ 
          	 {
          
           	 	for (var path in libraryData.css) {
              if ((H5P_DEVELOPMENT_MODE ==1 || (libraryName != "H5P.InteractiveVideo 1.9" && libraryName != "H5P.CoursePresentation 1.9")) && !H5P.cssLoaded(path)) {
              
                css += libraryData.css[path];
                H5PIntegration.loadedCss.push(path);
              }
              else if(libraryName == "H5P.InteractiveVideo 1.9" || libraryName == "H5P.CoursePresentation 1.9")
              {
              	css += libraryData.css[path];
              	
              }
            }
            if (css && (H5P_DEVELOPMENT_MODE ==1 || (libraryName != "H5P.InteractiveVideo 1.9" && libraryName != "H5P.CoursePresentation 1.9"))) {
              ns.$('head').append('<style type="text/css">' + css + '</style>');
            }else if(libraryName == "H5P.InteractiveVideo 1.9" || libraryName == "H5P.CoursePresentation 1.9")
            {
			  ns.$("#loadedcss").remove();
              ns.$('head').append('<style id="loadedcss" type="text/css">' + css + '</style>');
            }
            
            }
          }
          
          
          // Add JS.
          if (libraryData.javascript !== undefined) {
            var js = '';
           /*if(H5P_DEVELOPMENT_MODE == 0 &&  libraryName == "H5P.InteractiveVideo 1.9"){
           		js += window.parent.$("#video_merged_js").attr("value");
           		 if (js) {
             		 eval.apply(window, [js]);
            	 }
         	}
         	 else if(H5P_DEVELOPMENT_MODE == 0 &&  libraryName == "H5P.CoursePresentation 1.9"){
           		js += window.parent.$("#coursepre_mergedjs1").attr("value");
           		js += window.parent.$("#coursepre_mergedjs2").attr("value");
           		js += window.parent.$("#coursepre_mergedjs3").attr("value");
           		 if (js) {
             		 eval.apply(window, [js]);
            	 }

         	 }
             else */
            {
            	if(H5PIntegration.loadedJs == undefined)
            		H5PIntegration.loadedJs = new Array();
            for (var path in libraryData.javascript) {
            	//console.log("JS11:"+path);
            	
              if ((H5P_DEVELOPMENT_MODE == 1 ||(libraryName != "H5P.InteractiveVideo 1.9" && libraryName != "H5P.CoursePresentation 1.9")) && !H5P.jsLoaded(path)) 
              {
                   js += libraryData.javascript[path];
 	               H5PIntegration.loadedJs.push(path);
              }
              else if(libraryName == "H5P.InteractiveVideo 1.9" || libraryName == "H5P.CoursePresentation 1.9")
              {
                js += libraryData.javascript[path];
              }
            }
            var et = new Date().getTime();
        	 console.log("Resources loading time end:"+et+" in difference:"+(et-st));	
		
            if (js && (H5P_DEVELOPMENT_MODE ==1 || ( libraryName != "H5P.InteractiveVideo 1.9" && libraryName != "H5P.CoursePresentation 1.9"))) {
              eval.apply(window, [js]);
            }else if(libraryName == "H5P.CoursePresentation 1.9" || libraryName == "H5P.InteractiveVideo 1.9") 
            {
            	ns.$("#loadedscript").remove();
            	ns.$("head").append("<script id='loadedscript'>"+js+"</script>");
			
            }
            var et1 = new Date().getTime();
        	console.log("Resources processing time "+et1+" in difference:"+(et1-et));	
            }
          }
          callback(libraryData.semantics);
          
          // Run queue.
          for (var i = 0; i < ns.semanticsLoaded[libraryName].length; i++) {
            ns.semanticsLoaded[libraryName][i](libraryData.semantics);
          }
           /* hide unnecessary h5p ui components behavior settings and etc...*/
          if(ns.$(".coursepresentation").size() > 0)
          {
            
          	ns.$(".coursepresentation").next().hide();
          	var children = ns.$(".coursepresentation").parent().parent().children();
          	ns.$(children[1]).css("display","none");
          	
          	
          }
		 var et2 = new Date().getTime();
        console.log("After callback "+et2+" in difference:"+(et2-et1));
          
          
        
}

ns.$(document).ready(function()
{
/*	var js = "";
	var libraryData = window.parent.H5PVIDEO_RESOURCES;
  for (var path in libraryData.javascript) {
      js += libraryData.javascript[path];
  }
  ns.$("#loadedscript").remove();
  ns.$("head").append("<script id='loadedscript'>"+js+"</script>");*/
  		 
});


;
/** @namespace H5PEditor */
var H5PEditor = H5PEditor || {};

H5PEditor.SemanticStructure = (function ($) {
  var self = this;

  /**
   * The base of the semantic structure system.
   * All semantic structure class types will inherit this class.
   *
   * @class
   * @param {Object} field
   * @param {Object} defaultWidget
   */
  function SemanticStructure(field, defaultWidget) {
    var self = this;

    // Initialize event inheritance
    H5P.EventDispatcher.call(self);

    /**
     * Determine this fields label. Used in error messages.
     * @public
     */
    self.label = (field.label === undefined ? field.name : field.label);

    // Support old editor libraries
    self.field = {};

    /**
     * Global instance variables.
     * @private
     */
    var $widgetSelect, $wrapper, $inner, $errors, $description, $helpText, widgets;

    /**
     * Initialize. Wrapped to avoid leaking variables
     * @private
     */
    var init = function () {
      // Create field wrapper
      $wrapper = $('<div/>', {
        'class': 'field ' + field.type
      });

      /* We want to be in control of the label, description and errors
      containers to give the editor some structure. Also we do not provide
      direct access to the field object to avoid cluttering semantics.json with
      non-semantic properties and options. Getters and setters will be
      created for what is needed. */

      // Create field label
      var $label;
      if (field.label !== 0) {
        // Add label
        $label = createLabel(self.label, field.optional).appendTo($wrapper);
      }

      var innerClass;
      widgets = getValidWidgets();
      if (widgets.length > 1) {
        // Create widget select box
        $widgetSelect = $('<ul/>', {
          'class': 'h5peditor-widget-select',
          title: H5PEditor.t('core', 'editMode'),
          appendTo: $wrapper
        });
        for (var i = 0; i < widgets.length; i++) {
          addWidgetOption(widgets[i], i === 0);
        }

        // Allow custom styling when selector is present
        $wrapper.addClass('h5peditor-widgets');
      }

      // Create inner wrapper
      $inner = $('<div/>', {
        'class': 'h5peditor-widget-wrapper',
        appendTo: $wrapper
      });

      // Create errors container
      $errors = $('<div/>', {
        'class': 'h5p-errors'
      });

      // Create description
      if (field.description !== undefined) {
        $description = $('<div/>', {
          'class': 'h5peditor-field-description',
          text: field.description
        });
      }

      // Create help text
      $helpText = $('<div/>', {
        'class': 'h5p-help-text'
      });
    };

    /**
     * Add widget select option.
     *
     * @private
     */
    var addWidgetOption = function (widget, active) {
      var $option = $('<li/>', {
        'class': 'h5peditor-widget-option' + (active ? ' ' + CLASS_WIDGET_ACTIVE : ''),
        text: widget.label,
        role: 'button',
        tabIndex: 1,
        on: {
          click: function () {
            // Update UI
            $widgetSelect.children('.' + CLASS_WIDGET_ACTIVE).removeClass(CLASS_WIDGET_ACTIVE);
            $option.addClass(CLASS_WIDGET_ACTIVE);

            // Change Widget
            changeWidget(widget.name);
          }
        }
      }).appendTo($widgetSelect);
    };

    /**
     * Get a list of widgets that are valid and loaded.
     *
     * @private
     * @throws {TypeError} widgets must be an array
     * @returns {Array} List of valid widgets
     */
    var getValidWidgets = function () {
      if (field.widgets === undefined) {
        // No widgets specified use default
        return [defaultWidget];
      }
      if (!(field.widgets instanceof Array)) {
        throw TypeError('widgets must be an array');
      }

      // Check if specified widgets are valid
      var validWidgets = [];
      for (var i = 0; i < field.widgets.length; i++) {
        var widget = field.widgets[i];
        if (getWidget(widget.name)) {
          validWidgets.push(widget);
        }
      }

      if (!validWidgets.length) {
        // There are no valid widgets, add default
        validWidgets.push(self.default);
      }

      return validWidgets;
    };

    /**
     * Finds the widget class with the given name.
     *
     * @private
     * @param {String} name
     * @returns {Class}
     */
    var getWidget = function (name) {
      return H5PEditor[name];
    };

    /**
     * Change the UI widget.
     *
     * @private
     * @param {String} name
     */
    var changeWidget = function (name) {
      if (self.widget !== undefined) {
        // Validate our fields first to makes sure all "stored" from their widgets
        self.validate();

        // Remove old widgets
        self.widget.remove();
      }

      // TODO: Improve error handling?
      var widget = getWidget(name);
      self.widget = new widget(self);
      self.trigger('changeWidget');
      self.widget.appendTo($inner);

      // Add errors container and description.
      $errors.appendTo($inner);
      if ($description !== undefined) {
        $description.appendTo($inner);
      }
      $helpText.html(self.widget.helpText !== undefined ? self.widget.helpText : '').appendTo($inner);
    };

    /**
     * Appends the field widget to the given container.
     *
     * @public
     * @param {jQuery} $container
     */
    self.appendTo = function ($container) {
      // Use first widget by default
      changeWidget(widgets[0].name);

      $wrapper.appendTo($container);
    };

    /**
     * Remove this field and widget.
     *
     * @public
     */
    self.remove = function () {
      self.widget.remove();
      $wrapper.remove();
    };

    /**
     * Remove this field and widget.
     *
     * @public
     * @param {String} message
     */
    self.setError = function (message) {
      $errors.append(H5PEditor.createError(message));
    };

    /**
     * Clear error messages.
     *
     * @public
     */
    self.clearErrors = function () {
      $errors.html('');
    };

    /**
     * Get the name of this field.
     *
     * @public
     * @returns {String} Name of the current field
     */
    self.getName = function () {
      return field.name;
    };

    // Must be last
    init();
  }

  // Extends the event dispatcher
  SemanticStructure.prototype = Object.create(H5P.EventDispatcher.prototype);
  SemanticStructure.prototype.constructor = SemanticStructure;

  /**
   * Create generic editor label.
   *
   * @private
   * @param {String} text
   * @returns {jQuery}
   */
  var createLabel = function (text, optional) {
    return $('<label/>', {
      'class': 'h5peditor-label' + (optional ? '' : ' h5peditor-required'),
      text: text
    });
  };

  /**
   * @constant
   */
  var CLASS_WIDGET_ACTIVE = 'h5peditor-widget-active';

  return SemanticStructure;
})(H5P.jQuery);
;
var H5PEditor = H5PEditor || {};
var ns = H5PEditor;

/**
 * Construct a library selector.
 *
 * @param {Array} libraries
 * @param {String} defaultLibrary
 * @param {Object} defaultParams
 * @returns {ns.LibrarySelector}
 */
ns.LibrarySelector = function (libraries, defaultLibrary, defaultParams) {
  var that = this;
  var firstTime = true;
  var options = '<option value="-">-</option>';

  try {
    this.defaultParams = JSON.parse(defaultParams);
    if (!(this.defaultParams instanceof Object)) {
      throw true;
    }
  }
  catch (event) {
    // Content parameters are broken. Reset. (This allows for broken content to be reused without deleting it)
    this.defaultParams = {};
    // TODO: Inform the user?
  }

  this.defaultLibrary = this.currentLibrary = defaultLibrary;
  this.defaultLibraryParameterized = defaultLibrary ? defaultLibrary.replace('.', '-').toLowerCase() : undefined;

  for (var i = 0; i < libraries.length; i++) {
    var library = libraries[i];
    var libraryName = ns.libraryToString(library);

    // Never deny editing existing content
    // For new content deny old or restricted libs.
    if (this.defaultLibrary === libraryName ||
       ((library.restricted === undefined || !library.restricted) &&
         library.isOld !== true
      )
    ) {
      options += '<option value="' + libraryName + '"';
      if (libraryName === defaultLibrary || library.name === this.defaultLibraryParameterized) {
        options += ' selected="selected"';
      }
      if (library.tutorialUrl !== undefined) {
        options += ' data-tutorial-url="' + library.tutorialUrl + '"';
      }
      options += '>' + library.title + (library.isOld===true ? ' (deprecated)' : '') + '</option>';
    }
  }

  //Add tutorial link:
  //this.$tutorialUrl = ns.$('<a class="h5p-tutorial-url" target="_blank">' + ns.t('core', 'tutorialAvailable') + '</a>').hide();

  // Create confirm dialog
  var changeLibraryDialog = new H5P.ConfirmationDialog({
    headerText: H5PEditor.t('core', 'changeLibrary'),
    dialogText: H5PEditor.t('core', 'confirmChangeLibrary')
  }).appendTo(document.body);

  // Change library on confirmed
  changeLibraryDialog.on('confirmed', function () {
    changeLibraryToSelector();
  });

  // Revert selector on cancel
  changeLibraryDialog.on('canceled', function () {
    that.$selector.val(that.currentLibrary);
  });

  // Change library to selected
  var changeLibraryToSelector = function () {
    var library = that.$selector.val();
    that.loadSemantics(library);
    that.currentLibrary = library;
	
    if (library !== '-') {
      firstTime = false;
    }

  //  var tutorialUrl = that.$selector.find(':selected').data('tutorial-url');
   // that.$tutorialUrl.attr('href', tutorialUrl).toggle(tutorialUrl !== undefined && tutorialUrl !== null && tutorialUrl.length !== 0);
  };
	//h5pcustomize
  this.$selector = ns.$('<select id="h5peditor-library" style="visibility:hidden" name="h5peditor-library" title="' + ns.t('core', 'selectLibrary') + '">' + options + '</select>').change(function () {
    // Use timeout to avoid bug in Chrome >44, when confirm is used inside change event.
    // Ref. https://code.google.com/p/chromium/issues/detail?id=525629
    setTimeout(function () {
      if (!firstTime) {
        changeLibraryDialog.show(that.$selector.offset().top);
      }
      else {
        changeLibraryToSelector();
      }
    }, 0);
  });
};

/**
 * Append the selector html to the given container.
 *
 * @param {jQuery} $element
 * @returns {undefined}
 */
ns.LibrarySelector.prototype.appendTo = function ($element) {
  this.$parent = $element;

  this.$selector.appendTo($element);
  //this.$tutorialUrl.appendTo($element);

  //$element.append('<div class="h5p-more-libraries">' + ns.t('core', 'moreLibraries') + '</div>');
};

/**
 * Display loading message and load library semantics.
 *
 * @param {String} library
 * @returns {unresolved}
 */
ns.LibrarySelector.prototype.loadSemantics = function (library) {
  var that = this;

  if (this.form !== undefined) {
    // Remove old form.
    this.form.remove();
  }

  if (library === '-') {
    // No library chosen.
    this.$parent.attr('class', 'h5peditor');
    return;
  }
  this.$parent.attr('class', 'h5peditor ' + library.split(' ')[0].toLowerCase().replace('.', '-') + '-editor');

  // Display loading message
  
  var $loading = ns.$('<div class="h5peditor-loading h5p-throbber">' + ns.t('core', 'loading', {':type': 'semantics'}) + '</div>').appendTo(this.$parent);

  //h5pcustomize
  $loading.hide();
  
  if(window.parent.location.href.indexOf("presentation") >0)
  	window.parent.EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.createLoader("exp-sp-administration-contentauthor-presentation-addedit-form");
  else
  {
  
 //	window.parent.EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.createLoader("exp-sp-administration-contentauthor-addedit-form");
  }
  this.$selector.attr('disabled', true);
	
    
  ns.loadLibrary(library, function (semantics) {
    if (!semantics) {

  if(window.parent.location.href.indexOf("presentation") >0)
  	window.parent.EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.destroyLoader("exp-sp-administration-contentauthor-presentation-addedit-form");
  else
  {
  //	window.parent.EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.destroyLoader("exp-sp-administration-contentauthor-addedit-form");
  }


      that.form = ns.$('<div/>', {
        'class': 'h5p-errors',
        text: H5PEditor.t('core', 'noSemantics'),
        insertAfter: $loading
      });
    }
    else {
    if(window.parent.location.href.indexOf("presentation") >0)
  		window.parent.EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.destroyLoader("exp-sp-administration-contentauthor-presentation-addedit-form");
  	else{
  	
  	//	window.parent.EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.destroyLoader("exp-sp-administration-contentauthor-addedit-form");
  	}
    
      that.form = new ns.Form();
      that.form.replace($loading);
      that.form.processSemantics(semantics, (library === that.defaultLibrary || library === that.defaultLibraryParameterized ? that.defaultParams : {}));
      
      
    }

    that.$selector.attr('disabled', false);
    $loading.remove();
  }); 

};

/**
 * Return params needed to start library.
 */
ns.LibrarySelector.prototype.getParams = function () {
  if (this.form === undefined) {
    return;
  }

  // Only return if all fields has validated.
  var valid = true;

  if (this.form.children !== undefined) {
    for (var i = 0; i < this.form.children.length; i++) {
      if (this.form.children[i].validate() === false) {
        valid = false;
      }
    }
  }

  //return valid ? this.form.params : false;
  return this.form.params; // TODO: Switch to the line above when we are able to tell the user where the validation fails
};
;
var H5PEditor = H5PEditor || {};
var ns = H5PEditor;

/**
 * Construct a form from library semantics.
 */
ns.Form = function () {
  var self = this;
  
  this.params = {};
  this.passReadies = false;
  this.commonFields = {};
  //h5pcustomize style="display:none"
  this.$form = ns.$('<div class="h5peditor-form"><div class="tree"></div><div class="common collapsed hidden" style="display:none;"><div class="h5peditor-label"><span class="icon"></span>' + ns.t('core', 'commonFields') + '</div><div class="fields"><p class="desc">' + ns.t('core', 'commonFieldsDescription') + '</p></div></div></div>');
  this.$common = this.$form.find('.common > .fields');
  this.library = '';
  
  this.$common.prev().click(function () {
    self.$common.parent().toggleClass('collapsed');
  });
};

/**
 * Replace the given element with our form.
 *
 * @param {jQuery} $element
 * @returns {undefined}
 */
ns.Form.prototype.replace = function ($element) {
  $element.replaceWith(this.$form);
  this.offset = this.$form.offset();
  // Prevent inputs and selects in an h5peditor form from submitting the main
  // framework form.
  this.$form.on('keydown', 'input,select', function (event) {
    if (event.keyCode === 13) {
      // Prevent enter key from submitting form.
      return false;
    }
  });
};

/**
 * Remove the current form.
 */
ns.Form.prototype.remove = function () {
  this.$form.remove();
};

/**
 * Wrapper for processing the semantics.
 *
 * @param {Array} semantics
 * @param {Object} defaultParams
 * @returns {undefined}
 */
ns.Form.prototype.processSemantics = function (semantics, defaultParams) {
  this.params = defaultParams;
  ns.processSemanticsChunk(semantics, this.params, this.$form.children('.tree'), this);
};

/**
 * Collect functions to execute once the tree is complete.
 *
 * @param {function} ready
 * @returns {undefined}
 */
ns.Form.prototype.ready = function (ready) {
  this.readies.push(ready);
};
;
var H5PEditor = H5PEditor || {};
var ns = H5PEditor;

/**
 * Create a text field for the form.
 *
 * @param {mixed} parent
 * @param {Object} field
 * @param {mixed} params
 * @param {function} setValue
 * @returns {ns.Text}
 */
ns.Text = function (parent, field, params, setValue) {
  this.field = field;
  this.value = params;
  this.setValue = setValue;
  this.changeCallbacks = [];
};

/**
 * Append field to wrapper.
 *
 * @param {type} $wrapper
 * @returns {undefined}
 */
ns.Text.prototype.appendTo = function ($wrapper) {
  var that = this;

  this.$item = ns.$(this.createHtml()).appendTo($wrapper);
  this.$input = this.$item.children('label').children('input');
  this.$errors = this.$item.children('.h5p-errors');

  this.$input.change(function () {
    // Validate
    var value = that.validate();

    if (value !== false) {
      // Set param
      if (H5P.trim(value) === '') {
        // Avoid storing empty strings. (will be valid if field is optional)
        delete that.value;
        that.setValue(that.field);
      }
      else {
        that.value = value;
        that.setValue(that.field, ns.htmlspecialchars(value));
      }

      for (var i = 0; i < that.changeCallbacks.length; i++) {
        that.changeCallbacks[i](value);
      }
    }
  });
};

/**
 * Run callback when value changes.
 *
 * @param {function} callback
 * @returns {Number|@pro;length@this.changeCallbacks}
 */
ns.Text.prototype.change = function (callback) {
  this.changeCallbacks.push(callback);
  callback();

  return this.changeCallbacks.length - 1;
};

/**
 * Create HTML for the text field.
 */
ns.Text.prototype.createHtml = function () {
  var input = ns.createText(this.value, this.field.maxLength, this.field.placeholder);
  var label = ns.createLabel(this.field, input);

  return ns.createItem(this.field.type, label, this.field.description);
};

/**
 * Validate the current text field.
 */
ns.Text.prototype.validate = function () {
  var that = this;

  var value = H5P.trim(this.$input.val());

  if ((that.field.optional === undefined || !that.field.optional) && !value.length) {
    this.$errors.append(ns.createError(ns.t('core', 'requiredProperty', {':property': 'text field'})));
  }
  else if (value.length > this.field.maxLength) {
    this.$errors.append(ns.createError(ns.t('core', 'tooLong', {':max': this.field.maxLength})));
  }
  else if (this.field.regexp !== undefined && value.length && !value.match(new RegExp(this.field.regexp.pattern, this.field.regexp.modifiers))) {
    this.$errors.append(ns.createError(ns.t('core', 'invalidFormat')));
  }

  return ns.checkErrors(this.$errors, this.$input, value);
};

/**
 * Remove this item.
 */
ns.Text.prototype.remove = function () {
  this.$item.remove();
};

// Tell the editor what widget we are.
ns.widgets.text = ns.Text;
;
var H5PEditor = H5PEditor || {};
var ns = H5PEditor;

/**
 * Adds a html text field to the form.
 *
 * @param {type} parent
 * @param {type} field
 * @param {type} params
 * @param {type} setValue
 * @returns {undefined}
 */
ns.Html = function (parent, field, params, setValue) {
  this.field = field;
  this.value = params;
  this.setValue = setValue;
  this.tags = ns.$.merge(['br'], (this.field.tags || this.defaultTags));
};
ns.Html.first = true;

ns.Html.prototype.defaultTags = ['strong', 'em', 'del', 'h2', 'h3', 'a', 'ul', 'ol', 'table', 'hr'];

// This should probably be named "hasTag()" instead...
// And might be more efficient if this.tags.contains() were used?
ns.Html.prototype.inTags = function (value) {
  return (ns.$.inArray(value.toLowerCase(), this.tags) >= 0);
};

ns.Html.prototype.createToolbar = function () {
  var basicstyles = [];
  var paragraph = [];
  var formats = [];
  var inserts = [];
  var toolbar = [];

  // Basic styles
  if (this.inTags("strong") || this.inTags("b")) {
    basicstyles.push('Bold');
    // Might make "strong" duplicated in the tag lists. Which doesn't really
    // matter. Note: CKeditor will only make strongs.
    this.tags.push("strong");
  }
  if (this.inTags("em") || this.inTags("i")) {
    basicstyles.push('Italic');
    // Might make "em" duplicated in the tag lists. Which again
    // doesn't really matter. Note: CKeditor will only make ems.
    this.tags.push("em");
  }
  if (this.inTags("u")) basicstyles.push('Underline');
  if (this.inTags("strike") || this.inTags("del") || this.inTags("s")) {
    basicstyles.push('Strike');
    // Might make "strike" or "del" or both duplicated in the tag lists. Which
    // again doesn't really matter.
    this.tags.push("strike");
    this.tags.push("del");
    this.tags.push("s");
  }
  if (this.inTags("sub")) basicstyles.push("Subscript");
  if (this.inTags("sup")) basicstyles.push("Superscript");
  if (basicstyles.length > 0) {
    basicstyles.push("-");
    basicstyles.push("RemoveFormat");
    toolbar.push({
      name: 'basicstyles',
      items: basicstyles
    });
  }

  // Alignment is added to all wysiwygs
  toolbar.push({
    name: "justify",
    items: ["JustifyLeft", "JustifyCenter", "JustifyRight"]
  });

  // Paragraph styles
  if (this.inTags("ul")) {
    paragraph.push("BulletedList");
    this.tags.push("li");
  }
  if (this.inTags("ol")) {
    paragraph.push("NumberedList");
    this.tags.push("li");
  }
  if (this.inTags("blockquote")) paragraph.push("Blockquote");
  if (paragraph.length > 0) {
    toolbar.push(paragraph);
  }

  // Links.
  if (this.inTags("a")) {
    var items = ["Link", "Unlink"];
    if (this.inTags("anchor")) {
      items.push("Anchor");
    }
    toolbar.push({
      name: "links",
      items: items
    });
  }

  // Inserts
  if (this.inTags("img")) inserts.push("Image");
  if (this.inTags("table")) {
    inserts.push("Table");
    ns.$.merge(this.tags, ["tr", "td", "th", "colgroup", "thead", "tbody", "tfoot"]);
  }
  if (this.inTags("hr")) inserts.push("HorizontalRule");
  if (inserts.length > 0) {
    toolbar.push({
      name: "insert",
      items: inserts
    });
  }

  // Create wrapper for text styling options
  var styles = {
    name: "styles",
    items: []
  };
  var colors = {
    name: "colors",
    items: []
  };

  // Add format group if formatters in tags (h1, h2, etc). Formats use their
  // own format_tags to filter available formats.
  if (this.inTags("h1")) formats.push("h1");
  if (this.inTags("h2")) formats.push("h2");
  if (this.inTags("h3")) formats.push("h3");
  if (this.inTags("h4")) formats.push("h4");
  if (this.inTags("h5")) formats.push("h5");
  if (this.inTags("h6")) formats.push("h6");
  if (this.inTags("address")) formats.push("address");
  if (this.inTags("pre")) formats.push("pre");
  if (formats.length > 0 || this.inTags('p') || this.inTags('div')) {
    formats.push("p");   // If the formats are shown, always have a paragraph..
    this.tags.push("p");
    styles.items.push('Format');
  }

  var ret = {
    toolbar: toolbar
  };

  if (this.field.font !== undefined) {
    this.tags.push('span');

    /**
     * Help set specified values for property.
     *
     * @private
     * @param {Array} values list
     * @param {string} prop Property
     * @param {string} [defProp] Default property name
     */
    var setValues = function (values, prop, defProp) {
      ret[prop] = '';
      for (var i = 0; i < values.length; i++) {
        var val = values[i];
        if (val.label && val.css) {
          // Add label and CSS
          ret[prop] += val.label + '/' + val.css + ';';

          // Check if default value
          if (defProp && val.default) {
            ret[defProp] = val.label;
          }
        }
      }
    };

    /**
     * @private
     * @param {Array} values
     * @returns {string}
     */
    var getColors = function (values) {
      var colors = '';
      for (var i = 0; i < values.length; i++) {
        var val = values[i];
        if (val.label && val.css) {
          var css = val.css.match(/^#?([a-f0-9]{3}[a-f0-9]{3}?)$/i);
          if (!css) {
            continue;
          }

          // Add label and CSS
          if (colors) {
            colors += ',';
          }
          colors += val.label + '/' + css[1];
        }
      }
      return colors;
    };

    if (this.field.font.family) {
      // Font family chooser
      styles.items.push('Font');

      if (this.field.font.family instanceof Array) {
        // Use specified families
        setValues(this.field.font.family, 'font_names', 'font_defaultLabel');
      }
    }

    if (this.field.font.size) {
      // Font size chooser
      styles.items.push('FontSize');

      ret.fontSize_sizes = '';
      if (this.field.font.size instanceof Array) {
        // Use specified sizes
        setValues(this.field.font.size, 'fontSize_sizes', 'fontSize_defaultLabel');
      }
      else {
        ret.fontSize_defaultLabel = '100%';

        // Standard font size that is used. (= 100%)
        var defaultFont = 16;

        // Standard font sizes that is available.
        var defaultAvailable = [8, 9, 10, 11, 12, 14, 16, 18, 20, 22, 24, 26, 28, 36, 48, 72];
        for (var i = 0; i < defaultAvailable.length; i++) {
          // Calculate percentage of standard font size. This enables scaling
          // in content types without rounding errors across browsers.
          var em = defaultAvailable[i] / 16;
          ret.fontSize_sizes += (em * 100) + '%/' + em + 'em;';
        }

      }

    }

    if (this.field.font.color) {
      // Text color chooser
      colors.items.push('TextColor');

      if (this.field.font.color instanceof Array) {
        ret.colorButton_colors = getColors(this.field.font.color);
        ret.colorButton_enableMore = false;
      }
    }

    if (this.field.font.background) {
      // Text background color chooser
      colors.items.push('BGColor');

      if (this.field.font.background instanceof Array) {
        ret.colorButton_colors = getColors(this.field.font.color);
        ret.colorButton_enableMore = false;
      }
    }
  }

  // Add the text styling options
  if (styles.items.length) {
    toolbar.push(styles);
  }
  if (colors.items.length) {
    toolbar.push(colors);
  }

  // Set format_tags if not empty. CKeditor does not like empty format_tags.
  if (formats.length) {
    ret.format_tags = formats.join(';');
  }

  // Enable selection of enterMode in module semantics.
  if (this.field.enterMode === 'p' || formats.length > 0) {
    this.tags.push('p');
    ret.enterMode = CKEDITOR.ENTER_P;
  } else {
    // Default to DIV, not allowing BR at all.
    this.tags.push('div');
    ret.enterMode = CKEDITOR.ENTER_DIV;
  }

  return ret;
};

/**
 * Append field to wrapper.
 *
 * @param {type} $wrapper
 * @returns {undefined}
 */
ns.Html.prototype.appendTo = function ($wrapper) {
  var that = this;

  this.$item = ns.$(ns.createItem(this.field.type, this.createHtml(), this.field.description)).appendTo($wrapper);

  this.$input = this.$item.children('.ckeditor');
  this.$errors = this.$item.children('.h5p-errors');

  var ckConfig = {
    extraPlugins: "",
    startupFocus: true,
    enterMode: CKEDITOR.ENTER_DIV,
    allowedContent: true, // Disables the ckeditor content filter, might consider using it later... Must make sure it doesn't remove math...
    protectedSource: []
  };
  ns.$.extend(ckConfig, this.createToolbar());

  // Look for additions in HtmlAddons
  if (ns.HtmlAddons) {
    for (var tag in ns.HtmlAddons) {
      if (that.inTags(tag)) {
        for (var provider in ns.HtmlAddons[tag]) {
          ns.HtmlAddons[tag][provider](ckConfig, that.tags);
        }
      }
    }
  }

  this.$item.children('.ckeditor').focus(function () {
		
    // Blur is not fired on destroy. Therefore we need to keep track of it!
    var blurFired = false;

    // Remove placeholder
    that.$placeholder = that.$item.find('.h5peditor-ckeditor-placeholder').detach();

    if (ns.Html.first) {
      CKEDITOR.basePath = ns.basePath + '/ckeditor/';
    }

    if (ns.Html.current === that) {
      return;
    }
    // Remove existing CK instance.
    ns.Html.removeWysiwyg();

    ns.Html.current = that;
    ckConfig.width = this.offsetWidth - 8; // Avoid miscalculations
    
    //h5pcustomize
    ckConfig.width = "428px";
    that.ckeditor = CKEDITOR.replace(this, ckConfig);

    that.ckeditor.on('focus', function () {
      blurFired = false;
    });

    that.ckeditor.once('destroy', function () {

      // In some cases, the blur event is not fired. Need to be sure it is, so that
      // validation and saving is done
      if (!blurFired) {
        blur();
      }

      // Display placeholder if:
      // -- The value held by the field is empty AND
      // -- The value shown in the UI is empty AND
      // -- A placeholder is defined
      var value = that.ckeditor !== undefined ? that.ckeditor.getData() : that.$input.html();
      if (that.$placeholder.length !== 0 && (value === undefined || value.length === 0) && (that.value === undefined || that.value.length === 0)) {
        that.$placeholder.appendTo(that.$item.find('.ckeditor'));
      }
    });

    var blur = function () {
      blurFired = true;
      // Do not validate if the field has been hidden.
      if (that.$item.is(':visible')) {
        that.validate();
      }
    };
    
    that.ckeditor.on('blur', blur);

    // Add events to ckeditor. It is beeing done here since we know it exists
    // at this point... Use case from commit message: "Make the default
    // linkTargetType blank for ckeditor" - STGW
    if (ns.Html.first) {
      CKEDITOR.on('dialogDefinition', function(e) {
        // Take the dialog name and its definition from the event data.
        var dialogName = e.data.name;
        var dialogDefinition = e.data.definition;

        // Check if the definition is from the dialog window you are interested in (the "Link" dialog window).
        if (dialogName === 'link') {
          // Get a reference to the "Link Info" tab.
          var targetTab = dialogDefinition.getContents('target');

          // Set the default value for the URL field.
          var urlField = targetTab.get('linkTargetType');
          urlField['default'] = '_blank';
        }

        // Override show event handler
        var onShow = dialogDefinition.onShow;
        dialogDefinition.onShow = function () {
          if (onShow !== undefined) {
            onShow.apply(this, arguments);
          }

          // Grab current item
          var $item = ns.Html.current.$item;

          // Position dialog above text field
          var itemPos = $item.offset();
          var itemWidth = $item.width();
          var itemHeight = $item.height();
          var dialogSize = this.getSize();

          var x = itemPos.left + (itemWidth / 2) - (dialogSize.width / 2);
          var y = itemPos.top + (itemHeight / 2) - (dialogSize.height / 2);

          this.move(x, y, true);
        };
      });
      ns.Html.first = false;
    }
  });
};

/**
 * Create HTML for the HTML field.
 */
ns.Html.prototype.createHtml = function () {
  var html = '';
  console.log("createHtml:"+this.field.label);
  if (this.field.label !== undefined) {
    html += '<label class="h5peditor-label' + (this.field.optional ? '' : ' h5peditor-required') + '">' + this.field.label + '</label>';
  }

  html += '<div class="ckeditor" tabindex="0" contenteditable="true">';

  if (this.value !== undefined) {
    html += this.value;
  }
  else if (this.field.placeholder !== undefined) {
    html += '<span class="h5peditor-ckeditor-placeholder">' + this.field.placeholder + '</span>';
  }

  html += '</div>';

  return html;
};

/**
 * Validate the current text field.
 */
ns.Html.prototype.validate = function () {
  var that = this;
  if (that.$errors.children().length) {
    that.$errors.empty();
  }

  // Get contents from editor
  var value = this.ckeditor !== undefined ? this.ckeditor.getData() : this.$input.html();

  // Remove placeholder text if any:
  value = value.replace(/<span class="h5peditor-ckeditor-placeholder">.*<\/span>/, '');

  var $value = ns.$('<div>' + value + '</div>');
  var textValue = $value.text();

  // Check if we have any text at all.
  if (!this.field.optional && !textValue.length) {
    // We can accept empty text, if there's an image instead.
    if (! (this.inTags("img") && $value.find('img').length > 0)) {
      //h5pcustomize
      //this.$errors.append(ns.createError(this.field.label + ' is required and must have some text or at least an image in it.'));
      this.$errors.append(ns.createError(this.field.label + ' is required.'));
      
    }
  }

  // Verify HTML tags.  Removes tags not in allowed tags.  Will replace with
  // the tag's content.  So if we get an unallowed container, the contents
  // will remain, without the container.
  $value.find('*').each(function () {
    if (! that.inTags(this.tagName)) {
      ns.$(this).replaceWith(ns.$(this).contents());
    }
  });
  value = $value.html();

  // Display errors and bail if set.
  if (that.$errors.children().length) {
    return false;
  }

  this.value = value;
  this.setValue(this.field, value);
  this.$input.change(); // Trigger change event.

  return value;
};

/**
 * Destroy H5PEditor existing CK instance. If it exists.
 */
ns.Html.removeWysiwyg = function () {
  if (ns.Html.current !== undefined) {
    try {
      ns.Html.current.ckeditor.destroy();
    }
    catch (e) {
      // No-op, just stop error from propagating. This usually occurs if
      // the CKEditor DOM has been removed together with other DOM data.
    }
    ns.Html.current = undefined;
  }
};

/**
 * Remove this item.
 */
ns.Html.prototype.remove = function () {
  this.$item.remove();
};

ns.widgets.html = ns.Html;

;
var H5PEditor = H5PEditor || {};
var ns = H5PEditor;

/**
 * Create a number picker field for the form.
 *
 * @param {mixed} parent
 * @param {Object} field
 * @param {mixed} params
 * @param {function} setValue
 * @returns {ns.Number}
 */
ns.Number = function (parent, field, params, setValue) {
  this.field = field;
  this.value = params;
  this.setValue = setValue;
};

/**
 * Append field to wrapper.
 *
 * @param {jQuery} $wrapper
 * @returns {undefined}
 */
ns.Number.prototype.appendTo = function ($wrapper) {
  var that = this;

  this.$item = ns.$(this.createHtml()).appendTo($wrapper);
  this.$errors = this.$item.children('.h5p-errors');
  var $inputs = this.$item.children('label').children('input');
  if ($inputs.length === 1) {
    this.$input = $inputs;
  }
  else {
    this.$range = $inputs.filter(':first');
    this.$input = this.$range.next();
  }

  this.$input.change(function () {
    // Validate
    var value = that.validate();

    if (value !== false) {
      // Set param
      that.value = value;
      that.setValue(that.field, value);
      if (that.$range !== undefined) {
        that.$range.val(value);
      }
    }
  });

  if (this.$range !== undefined) {
    if (this.$range.attr('type') === 'range') {
      this.$range.change(function () {
        that.$input.val(that.$range.val()).change();
      });

      // Add some styles for IE.
      if (H5PEditor.isIE) {
        this.$range.css('margin-top', 0);
        this.$input.css('margin-top', '7px');
      }
    }
    else {
      this.$range.remove();
    }
  }
};

/**
 * Create HTML for the field.
 */
ns.Number.prototype.createHtml = function () {
  var input = ns.createText(this.value, 15);
  /* TODO: Add back in when FF gets support for input:range....
   *if (this.field.min !== undefined && this.field.max !== undefined && this.field.step !== undefined) {
    input = '<input type="range" min="' + this.field.min + '" max="' + this.field.max + '" step="' + this.field.step + '"' + (this.value === undefined ? '' : ' value="' + this.value + '"') + '/>' + input;
  }
   */

  var label = ns.createLabel(this.field, input);

  return ns.createItem(this.field.type, label, this.field.description);
};

/**
 * Validate the current text field.
 */
ns.Number.prototype.validate = function () {
  var that = this;

  var value = H5P.trim(this.$input.val());
  var decimals = this.field.decimals !== undefined && this.field.decimals;

  if (!value.length) {
    if (that.field.optional === true) {
      // Field is optional and does not have a value, nothing more to validate
      return;
    }

    // Field must have a value
    this.$errors.append(ns.createError(ns.t('core', 'requiredProperty', {':property': 'number field'})));
  }
  else if (decimals && !value.match(new RegExp('^-?[0-9]+(.|,)[0-9]{1,}$'))) {
    this.$errors.append(ns.createError(ns.t('core', 'onlyNumbers', {':property': 'number field'})));
  }
  else if (!decimals && !value.match(new RegExp('^-?[0-9]+$'))) {
    this.$errors.append(ns.createError(ns.t('core', 'onlyNumbers', {':property': 'number field'})));
  }
  else {
    if (decimals) {
      value = parseFloat(value.replace(',', '.'));
    }
    else {
      value = parseInt(value);
    }

    if (this.field.max !== undefined && value > this.field.max) {
      this.$errors.append(ns.createError(ns.t('core', 'exceedsMax', {':property': 'number field', ':max': this.field.max})));
    }
    else if (this.field.min !== undefined && value < this.field.min) {
      this.$errors.append(ns.createError(ns.t('core', 'belowMin', {':property': 'number field', ':min': this.field.min})));
    }
    else if (this.field.step !== undefined && value % this.field.step)  {
      this.$errors.append(ns.createError(ns.t('core', 'outOfStep', {':property': 'number field', ':step': this.field.step})));
    }
  }

  return ns.checkErrors(this.$errors, this.$input, value);
};

/**
 * Remove this item.
 */
ns.Number.prototype.remove = function () {
  this.$item.remove();
};

// Tell the editor what widget we are.
ns.widgets.number = ns.Number;
;
// DEPRECATED: This widget will be removed and replaced with the HTML widget
var H5PEditor = H5PEditor || {};
var ns = H5PEditor;

/**
 * Create a text field for the form.
 *
 * @param {mixed} parent
 * @param {Object} field
 * @param {mixed} params
 * @param {function} setValue
 * @returns {ns.Textarea}
 */
ns.Textarea = function (parent, field, params, setValue) {
  this.field = field;
  this.value = params;
  this.setValue = setValue;
};

/**
 * Append field to wrapper.
 *
 * @param {jQuery} $wrapper
 * @returns {undefined}
 */
ns.Textarea.prototype.appendTo = function ($wrapper) {
  var that = this;

  this.$item = ns.$(this.createHtml()).appendTo($wrapper);
  this.$input = this.$item.children('label').children('textarea');
  this.$errors = this.$item.children('.h5p-errors');

  this.$input.change(function () {
    // Validate
    var value = that.validate();

    if (value !== false) {
      // Set param
      that.setValue(that.field, ns.htmlspecialchars(value));
    }
  });
};

/**
 * Create HTML for the text field.
 */
ns.Textarea.prototype.createHtml = function () {
  var input = '<textarea cols="30" rows="4"';
  if (this.field.placeholder !== undefined) {
    input += ' placeholder="' + this.field.placeholder + '"';
  }
  input += '>';
  if (this.value !== undefined) {
    input += this.value;
  }
  input += '</textarea>';

  var label = ns.createLabel(this.field, input);

  return ns.createItem(this.field.type, label, this.field.description);
};

/**
 * Validate the current text field.
 */
ns.Textarea.prototype.validate = function () {
  var value = H5P.trim(this.$input.val());

  if ((this.field.optional === undefined || !this.field.optional) && !value.length) {
    this.$errors.append(ns.createError(ns.t('core', 'requiredProperty', {':property': 'text field'})));
  }
  else if (value.length > this.field.maxLength) {
    this.$errors.append(ns.createError(ns.t('core', 'tooLong', {':max': this.field.maxLength})));
  }
  else if (this.field.regexp !== undefined && !value.match(new RegExp(this.field.regexp.pattern, this.field.regexp.modifiers))) {
    this.$errors.append(ns.createError(ns.t('core', 'invalidFormat')));
  }

  return ns.checkErrors(this.$errors, this.$input, value);
};

/**
 * Remove this item.
 */
ns.Textarea.prototype.remove = function () {
  this.$item.remove();
};

// Tell the editor what semantic field we are.
ns.widgets.textarea = ns.Textarea;;
H5PEditor.FileUploader = (function ($, EventDispatcher) {
  var nextIframe;

  /**
   * File Upload API for H5P
   *
   * @class H5PEditor.FileUploader
   * @extends H5P.EventDispatcher
   * @param {Object} field Required for validating the uploaded file
   */
  function FileUploader(field) {
    var self = this;

    // Initialize event inheritance
    EventDispatcher.call(self);

    var isUploadingData;

    /**
     * Trigger uploading start.
     *
     * @private
     * @param {string} [data] Optional for uploading string data (URI)
     * @return {boolean} false if the iframe is unavailable and the caller should try again later
     */
    var upload = function (data) {
      if (!nextIframe.isReady()) {
        return false; // Iframe isn't loaded. The caller should try again later
      }

      isUploadingData = (data !== undefined);

      // Add event listeners
      nextIframe.on('upload', function (event) {
        self.trigger(event);
      });
      nextIframe.on('uploadComplete', function (event) {
        self.trigger(event);
      });

      // Update field
      nextIframe.setField(field, data);

      return true;
    };

    /**
     * Prepare an iframe and triggers the opening of the file selector
     * @return {boolean} false if the iframe is unavailable and the caller should try again later
     */
    self.openFileSelector = function () {
      return upload();
    };

    /**
     * Prepare an iframe and trigger upload of the given data.
     *
     * @param {string} data
     * @return {boolean} false if the iframe is unavailable and the caller should try again later
     */
    self.uploadData = function (data) {
      if (data === undefined) {
        throw('Missing data.');
      }
      return upload(data);
    };

    /**
     * Makes it possible to check if it is data or a file being uploaded.
     *
     * @return {boolean}
     */
    self.isUploadingData = function () {
      return isUploadingData;
    };

    if (!nextIframe) {
      // We must always have an iframe available for the next upload
      nextIframe = new Iframe();
    }
  }

  // Extends the event dispatcher
  FileUploader.prototype = Object.create(EventDispatcher.prototype);
  FileUploader.prototype.constructor = FileUploader;

  /**
   * Iframe for file uploading. Only available for the FileUploader class.
   * Iframes are discarded after the upload is completed.
   *
   * @private
   * @class Iframe
   * @extends H5P.EventDispatcher
   */
  function Iframe() {
    var self = this;

    // Initialize event inheritance
    EventDispatcher.call(self);

    var ready = false;
    var $iframe, $form, $file, $data, $field;

    /**
     * @private
     */
    var upload = function () {
      // Iframe isn't really bound to a field until the upload starts
      ready = false;

      // Trigger upload event and submit upload form
      self.trigger('upload');
      $form.submit();

      // This iframe is used, we must add another for the next upload
      nextIframe = new Iframe();
    };

    /**
     * Create and insert iframe into the DOM.
     *
     * @private
     */
    var insertIframe = function () {
      $iframe = $('<iframe/>', {
        css: {
          position: 'absolute',
          width: '1px',
          height: '1px',
          top: '-1px',
          border: 0,
          overflow: 'hidden'
        },
        one: {
          load: function () {
            ready = true;
          }
        },
        appendTo: 'body'
      });
    };

    /**
     * Create and add upload form to the iframe.
     *
     * @private
     */
    var insertForm = function () {
      // Create upload form
      $form = $('<form/>', {
        method: 'post',
        enctype: 'multipart/form-data',
        action: H5PEditor.getAjaxUrl('files')
      });

      // Create input fields
      $file = $('<input/>', {
        type: 'file',
        name: 'file',
        on: {
          change: upload
        },
        appendTo: $form
      });
      $data = $('<input/>', {
        type: 'hidden',
        name: 'dataURI',
        appendTo: $form
      });
      $field = $('<input/>', {
        type: 'hidden',
        name: 'field',
        appendTo: $form
      });
      $('<input/>', {
        type: 'hidden',
        name: 'contentId',
        value: H5PEditor.contentId ? H5PEditor.contentId : 0,
        appendTo: $form
      });

      // Add form to iframe
      var $body = $iframe.contents().find('body');
      $form.appendTo($body);

      // Add event handler for processing results
      $iframe.on('load', processResponse);
    };

    /**
     * Handler for processing server response when upload form is submitted.
     *
     * @private
     */
    var processResponse = function () {
      // Upload complete, get response text
      var $body = $iframe.contents().find('body');
      var response = $body.text();

      // Clean up all our DOM elements
      $iframe.remove();

      // Try to parse repsonse
      if (response) {
        var result;
        var uploadComplete = {
          error: null,
          data: null
        };

        try {
          result = JSON.parse(response);
        }
        catch (err) {
          H5P.error(err);
          // Add error data to event object
          uploadComplete.error = H5PEditor.t('core', 'fileToLarge');
        }

        if (result !== undefined) {
          if (result.error !== undefined) {
            uploadComplete.error = result.error;
          }
          if (result.success === false) {
            uploadComplete.error = (result.message ? result.message : H5PEditor.t('core', 'unknownFileUploadError'));
          }
        }

        if (uploadComplete.error === null) {
          // No problems, add response data to event object
          uploadComplete.data = result;
        }

        // Allow the widget to process the result
        self.trigger('uploadComplete', uploadComplete);
      }
    };

    /**
     * Prepare the upload form for the given field.
     * Opens the file selector or if data is provided, submits the form
     * straight away.
     *
     * @param {Object} field
     * @param {string} [data] Optional URI
     */
    self.setField = function (field, data) {
      // Determine allowed file mimes
      var mimes;
      if (field.mimes) {
        mimes = field.mimes.join(',');
      }
      else if (field.type === 'image') {
        mimes = 'image/jpeg,image/png,image/gif';
      }
      else if (field.type === 'audio') {
        mimes = 'audio/mpeg,audio/x-wav,audio/ogg';
      }
      else if (field.type === 'video') {
        mimes = 'video/mp4,video/webm,video/ogg';
      }

      $file.attr('accept', mimes);

      // Set field
      $field.val(JSON.stringify(field));

      if (data !== undefined) {
        // Upload given data
        $data.val(data);
        upload();
      }
      else {
        // Trigger file selector
        $file.click();
      }
    };

    /**
     * Indicates if this iframe is ready to be used
     */
    self.isReady = function () {
      if (!ready) {
        return false;
      }

      if (!$form) {
        // Insert form if not present
        insertForm();
      }
      else {
        // If present clear any event handlers (was used by another field)
        self.off('upload');
        self.off('uploadComplete');
      }

      return true;
    };

    // Always insert iframe on construct
    insertIframe();
    // The iframe must be loaded before the click event that sets the field,
    // async clicking won't work for security reasons in the browser.
  }

  // Extends the event dispatcher
  Iframe.prototype = Object.create(EventDispatcher.prototype);
  Iframe.prototype.constructor = Iframe;

  return FileUploader;
})(H5P.jQuery, H5P.EventDispatcher);
;
var H5PEditor = H5PEditor || {};
var ns = H5PEditor;

/**
 * Adds a file upload field to the form.
 *
 * @param {mixed} parent
 * @param {object} field
 * @param {mixed} params
 * @param {function} setValue
 * @returns {ns.File}
 */
ns.File = function (parent, field, params, setValue) {
  var self = this;

  // Initialize inheritance
  ns.FileUploader.call(self, field);

  this.parent = parent;
  this.field = field;
  this.params = params;
  this.setValue = setValue;
  this.library = parent.library + '/' + field.name;

  if (params !== undefined) {
    this.copyright = params.copyright;
  }

  this.changes = [];
  this.passReadies = true;
  parent.ready(function () {
    self.passReadies = false;
  });

  // Create remove file dialog
  this.confirmRemovalDialog = new H5P.ConfirmationDialog({
    headerText: H5PEditor.t('core', 'removeFile'),
    dialogText: H5PEditor.t('core', 'confirmRemoval', {':type': 'file'})
  }).appendTo(document.body);

  // Remove file on confirmation
  this.confirmRemovalDialog.on('confirmed', function () {
    delete self.params;
    self.setValue(self.field);
    self.addFile();

    for (var i = 0; i < self.changes.length; i++) {
      self.changes[i]();
    }
  });

  // When uploading starts
  self.on('upload', function () {
    // Insert throbber
    self.$file.html('<div class="h5peditor-uploading h5p-throbber">' + ns.t('core', 'uploading') + '</div>');

    // Clear old error messages
    self.$errors.html('');
  });

  // Handle upload complete
  self.on('uploadComplete', function (event) {
    var result = event.data;

    try {
      if (result.error) {
        throw result.error;
      }

      self.params = self.params || {};
      self.params.path = result.data.path;
      self.params.mime = result.data.mime;
      self.params.copyright = self.copyright;

      // Make it possible for other widgets to process the result
      self.trigger('fileUploaded', result.data);

      self.setValue(self.field, self.params);

      for (var i = 0; i < self.changes.length; i++) {
        self.changes[i](self.params);
      }
    }
    catch (error) {
      self.$errors.append(ns.createError(error));
    }

    self.addFile();
  });
};

ns.File.prototype = Object.create(ns.FileUploader.prototype);
ns.File.prototype.constructor = ns.File;

/**
 * Append field to the given wrapper.
 *
 * @param {jQuery} $wrapper
 * @returns {undefined}
 */
ns.File.prototype.appendTo = function ($wrapper) {
  var self = this;

  var label = '';
  if (this.field.label !== 0) {
    label = '<span class="h5peditor-label' + (this.field.optional ? '' : ' h5peditor-required') + '">' + (this.field.label === undefined ? this.field.name : this.field.label) + '</span>';
  }

  var html = ns.createItem(this.field.type, label + '<div class="file"></div><a class="h5p-copyright-button" href="#">' + ns.t('core', 'editCopyright') + '</a><div class="h5p-editor-dialog"><a href="#" class="h5p-close" title="' + ns.t('core', 'close') + '"></a></div>', this.field.description);

  var $container = ns.$(html).appendTo($wrapper);
  this.$file = $container.find('.file');
  this.$errors = $container.find('.h5p-errors');
  this.addFile();

  var $dialog = $container.find('.h5p-editor-dialog');
  $container.find('.h5p-copyright-button').add($dialog.find('.h5p-close')).click(function () {
    $dialog.toggleClass('h5p-open');
    return false;
  });

  var group = new ns.widgets.group(self, ns.copyrightSemantics, self.copyright, function (field, value) {
    if (self.params !== undefined) {
      self.params.copyright = value;
    }
    self.copyright = value;
  });
  group.appendTo($dialog);
  group.expand();
  group.$group.find('.title').remove();
  this.children = [group];
};


/**
 * Sync copyright between all video files.
 *
 * @returns {undefined}
 */
ns.File.prototype.setCopyright = function (value) {
  this.copyright = this.params.copyright = value;
};


/**
 * Creates thumbnail HTML and actions.
 *
 * @returns {Boolean}
 */
ns.File.prototype.addFile = function () {
  var that = this;

  if (this.params === undefined) {
    this.$file.html('<a href="#" class="add" title="' + ns.t('core', 'addFile') + '"></a>').children('.add').click(function () {
      that.openFileSelector();
      return false;
    });
    return;
  }

  var thumbnail;
  if (this.field.type === 'image') {
    thumbnail = {};
    thumbnail.path = H5P.getPath(this.params.path, H5PEditor.contentId);
    thumbnail.height = 100;
    if (this.params.width !== undefined) {
      thumbnail.width = thumbnail.height * (this.params.width / this.params.height);
    }
  }
  else {
    thumbnail = ns.fileIcon;
  }

  this.$file.html('<a href="#" title="' + ns.t('core', 'changeFile') + '" class="thumbnail"><img ' + (thumbnail.width === undefined ? '' : ' width="' + thumbnail.width + '"') + 'height="' + thumbnail.height + '" alt="' + (this.field.label === undefined ? '' : this.field.label) + '"/><a href="#" class="remove" title="' + ns.t('core', 'removeFile') + '"></a></a>').children(':eq(0)').click(function () {
    that.openFileSelector();
    return false;
  }).children('img').attr('src', thumbnail.path).end().next().click(function (e) {
    that.confirmRemovalDialog.show(H5P.jQuery(this).offset().top);
    return false;
  });
};

/**
 * Validate this item
 */
ns.File.prototype.validate = function () {
  return true;
};

/**
 * Remove this item.
 */
ns.File.prototype.remove = function () {
  // TODO: Check what happens when removed during upload.
  this.$file.parent().remove();
};

/**
 * Collect functions to execute once the tree is complete.
 *
 * @param {function} ready
 * @returns {undefined}
 */
ns.File.prototype.ready = function (ready) {
  if (this.passReadies) {
    this.parent.ready(ready);
  }
  else {
    ready();
  }
};

// Tell the editor what widget we are.
ns.widgets.file = ns.File;
;
/*global H5P*/
var H5PEditor = H5PEditor || {};
var ns = H5PEditor;

/**
 * Adds an image upload field with image editing tool to the form.
 *
 * @param {Object} parent Parent widget of this widget
 * @param {Object} field Semantic fields
 * @param {Object} params Existing image parameters
 * @param {function} setValue Function for updating parameters
 * @returns {ns.widgets.image}
 */
ns.widgets.image = function (parent, field, params, setValue) {
  var self = this;

  // Initialize inheritance
  ns.File.call(self, parent, field, params, setValue);

  this.parent = parent;
  this.field = field;
  this.params = params;
  this.setValue = setValue;
  this.library = parent.library + '/' + field.name;

  if (params !== undefined) {
    this.copyright = params.copyright;
  }

  // Keep track of editing image
  this.isEditing = false;

  // Keep track of type of image that is being uploaded
  this.isOriginalImage = false;

  this.changes = [];
  this.passReadies = true;
  parent.ready(function () {
    self.passReadies = false;
  });

  this.confirmationDialog = new H5P.ConfirmationDialog({
    headerText: H5PEditor.t('core', 'removeImage'),
    bodyText: H5PEditor.t('core', 'confirmImageRemoval')
  });

  this.confirmationDialog.on('confirmed', function () {
    self.removeImage();
  });

  // When uploading starts
  self.on('upload', function () {
    // Hide edit image button
    self.$editImage.addClass('hidden');

    if (!self.isUploadingData()) {
      // Uploading new original image
      self.isOriginalImage = true;
    }
  });

  // When a new file has been uploaded
  self.on('fileUploaded', function (event) {
    // Uploaded new original image
    if (self.isOriginalImage) {
      self.isOriginalImage = false;
      delete self.params.originalImage;
    }

    // Store width and height
    self.params.width = event.data.width;
    self.params.height = event.data.height;

    // Show edit image button
    self.$editImage.removeClass('hidden');
    self.isEditing = false;
  });
};

ns.widgets.image.prototype = Object.create(ns.File.prototype);
ns.widgets.image.prototype.constructor = ns.widgets.image;

/**
 * Append field to the given wrapper.
 *
 * @param {jQuery} $wrapper
 */
ns.widgets.image.prototype.appendTo = function ($wrapper) {
  var self = this;

  var label = '';
  if (this.field.label !== 0) {
    var labelString = this.field.label === undefined ? this.field.name : this.field.label;
    label = '<span class="h5peditor-label' + (this.field.optional ? '' : ' h5peditor-required') + '">' + labelString + '</span>';
  }

  var htmlString = label + '<div class="file"></div>' +
    '<div class="h5p-editor-image-buttons">' +
      '<button class="h5p-editing-image-button">' + ns.t('core', 'editImage') + '</button>' +
      '<button class="h5p-copyright-button">' + ns.t('core', 'editCopyright') + '</button>' +
    '</div>' +
    '<div class="h5p-editor-dialog">' +
      '<a href="#" class="h5p-close" title="' + ns.t('core', 'close') + '"></a>' +
    '</div>';

  var html = ns.createItem(this.field.type, htmlString, this.field.description);
  var $container = ns.$(html).appendTo($wrapper);
  this.$editImage = $container.find('.h5p-editing-image-button');
  this.$file = $container.find('.file');
  this.$errors = $container.find('.h5p-errors');
  this.addFile();

  var $dialog = $container.find('.h5p-editor-dialog');
  $container.find('.h5p-copyright-button').add($dialog.find('.h5p-close')).click(function () {
    $dialog.toggleClass('h5p-open');
    return false;
  });

  var editImagePopup = new H5PEditor.ImageEditingPopup(this.field.ratio);
  editImagePopup.on('savedImage', function (e) {

    // Not editing any longer
    self.isEditing = false;

    // No longer an original image
    self.isOriginalImage = false;

    // Set current source as original image, if no original image
    if (!self.params.originalImage) {
      self.params.originalImage = {
        path: self.params.path,
        mime: self.params.mime,
        height: self.params.height,
        width: self.params.width
      };
    }

    // Upload new image
    self.uploadData(e.data);
  });

  editImagePopup.on('resetImage', function () {
    var imagePath = self.params.originalImage ? self.params.originalImage.path
      : self.params.path;
    var imageSrc = H5P.getPath(imagePath, H5PEditor.contentId);
    editImagePopup.setImage(imageSrc);
  });

  editImagePopup.on('canceled', function () {
    self.isEditing = false;
  });

  editImagePopup.on('initialized', function () {
    // Remove throbber from image
    self.$editImage.removeClass('loading');
  });

  $container.find('.h5p-editing-image-button').click(function () {
    if (self.params && self.params.path) {
      var imageSrc;
      if (!self.isEditing) {
        imageSrc = H5P.getPath(self.params.path, H5PEditor.contentId);
        self.isEditing = true;
      }
      self.$editImage.toggleClass('loading');

      // Add throbber to image
      editImagePopup.show(ns.$(this).offset(), imageSrc);
    }
  });

  var group = new ns.widgets.group(self, ns.copyrightSemantics, self.copyright,
    function (field, value) {
      if (self.params !== undefined) {
        self.params.copyright = value;
      }
      self.copyright = value;
    });
  group.appendTo($dialog);
  group.expand();
  group.$group.find('.title').remove();
  this.children = [group];
};


/**
 * Sync copyright.
 */
ns.widgets.image.prototype.setCopyright = function (value) {
  this.copyright = this.params.copyright = value;
};


/**
 * Creates thumbnail HTML and actions.
 *
 * @returns {boolean} True if file was added, false if file was removed
 */
ns.widgets.image.prototype.addFile = function () {
  var that = this;

  
  if (this.params === undefined) {

    // No image look
    this.$file
      .html('<a href="#" class="add" title="' + ns.t('core', 'addFile') + '">Upload</a>')
      .children('.add')
      .click(function () {
        that.openFileSelector();
        return false;
      });

    // Remove edit image button
    this.$editImage.addClass('hidden');
    this.isEditing = false;

    return false;
  }

  var source = H5P.getPath(this.params.path, H5PEditor.contentId);
  //h5pcustomize
  var validImageFileLastCharCheck =  source.substr(-1);
  if(validImageFileLastCharCheck == "/")
   return false;
   
   
  var thumbnail = {};
  //h5pcustomize
  //http://testing.exphosted.com/sites/default/files//h5p/content/454/image.jpg^^^images/file-584687691a1d7.jpg
  if(source != null && source.indexOf("^^^") > 0)
  {
	var removeFileName = source.split("^^^")[0];
	var removeFileNameArr = removeFileName.split("/");
	removeFileNameArr[removeFileNameArr.length-1]="";
	source =removeFileNameArr.join("/")+source.split("^^^")[1];
  }
  
  thumbnail.path = source;
  thumbnail.height = 100;
  if (this.params.width !== undefined) {
    thumbnail.width = thumbnail.height * (this.params.width / this.params.height);
  }

  var thumbnailWidth = thumbnail.width === undefined ? '' : ' width="' + thumbnail.width + '"';
  var altText = (this.field.label === undefined ? '' : this.field.label);
  var fileHtmlString =
    '<a href="#" title="' + ns.t('core', 'changeFile') + '" class="thumbnail">' +
      '<img ' + thumbnailWidth + 'height="' + thumbnail.height + '" alt="' + altText + '"/>' +
    '</a>' +
    '<a href="#" class="remove" title="' + ns.t('core', 'removeFile') + '"></a>';


  //thumbnail.path =  "http://testing.exphosted.com/sites/default/files//h5p/content/455/images/poster.png";
  this.$file.html(fileHtmlString)
    .children(':eq(0)')
    .click(function () {
      that.openFileSelector();
      return false;
    })
    .children('img')
    .attr('src', thumbnail.path)
    .end()
    .next()
    .click(function () {
      that.confirmRemovalDialog.show(that.$file.offset().top);
      return false;
    });

  // Uploading original image
  that.$editImage.removeClass('hidden');

  // Notify listeners that image was changed to params
  that.trigger('changedImage', this.params);

  return true;
};

/**
 * Remove image
 */
ns.widgets.image.prototype.removeImage = function () {

  // Notify listeners that we removed image with params
  this.trigger('removedImage', this.params);

  delete this.params;
  this.setValue(this.field);
  this.addFile();

  for (var i = 0; i < this.changes.length; i++) {
    this.changes[i]();
  }
};

/**
 * Validate this item
 */
ns.widgets.image.prototype.validate = function () {
  return true;
};

/**
 * Remove this item.
 */
ns.widgets.image.prototype.remove = function () {
  // TODO: Check what happens when removed during upload.
  this.$file.parent().remove();
};

/**
 * Collect functions to execute once the tree is complete.
 *
 * @param {function} ready
 */
ns.widgets.image.prototype.ready = function (ready) {
  if (this.passReadies) {
    this.parent.ready(ready);
  }
  else {
    ready();
  }
};
;
/*global H5PEditor, H5P, ns, Darkroom*/
H5PEditor.ImageEditingPopup = (function ($, EventDispatcher) {
  var instanceCounter = 0;
  var scriptsLoaded = false;

  /**
   * Popup for editing images
   *
   * @param {number} [ratio] Ratio that cropping must keep
   * @constructor
   */
  function ImageEditingPopup(ratio) {
    EventDispatcher.call(this);
    var self = this;
    var uniqueId = instanceCounter;
    var isShowing = false;
    var isReset = false;
    var topOffset = 0;
    var maxWidth;
    var maxHeight;

    // Create elements
    var background = document.createElement('div');
    background.className = 'h5p-editing-image-popup-background hidden';

    var popup = document.createElement('div');
    popup.className = 'h5p-editing-image-popup';
    background.appendChild(popup);

    var header = document.createElement('div');
    header.className = 'h5p-editing-image-header';
    popup.appendChild(header);

    var headerTitle = document.createElement('div');
    headerTitle.className = 'h5p-editing-image-header-title';
    headerTitle.textContent = 'Edit Image!';
    header.appendChild(headerTitle);

    var headerButtons = document.createElement('div');
    headerButtons.className = 'h5p-editing-image-header-buttons';
    header.appendChild(headerButtons);

    var editingContainer = document.createElement('div');
    editingContainer.className = 'h5p-editing-image-editing-container';
    popup.appendChild(editingContainer);

    var imageLoading = document.createElement('div');
    imageLoading.className = 'h5p-editing-image-loading';
    imageLoading.textContent = ns.t('core', 'loadingImageEditor');
    popup.appendChild(imageLoading);

    // Create editing image
    var editingImage = new Image();
    editingImage.className = 'h5p-editing-image hidden';
    editingImage.id = 'h5p-editing-image-' + uniqueId;
    editingContainer.appendChild(editingImage);

    // Close popup on background click
    background.addEventListener('click', function () {
      this.hide();
    }.bind(this));

    // Prevent closing popup
    popup.addEventListener('click', function (e) {
      e.stopPropagation();
    });

    // Make sure each ImageEditingPopup instance has a unique ID
    instanceCounter += 1;

    /**
     * Create header button
     *
     * @param {string} coreString Must be specified in core translations
     * @param {string} className Unique button identifier that will be added to classname
     * @param {function} clickEvent OnClick function
     */
    var createButton = function (coreString, className, clickEvent) {
      var button = document.createElement('button');
      button.textContent = ns.t('core', coreString);
      button.className = className;
      button.addEventListener('click', clickEvent);
      headerButtons.appendChild(button);
    };

    /**
     * Set max width and height for image editing tool
     */
    var setDarkroomDimensions = function () {

      // Set max dimensions
      var dims = ImageEditingPopup.staticDimensions;
      maxWidth = H5P.$body.get(0).offsetWidth - dims.backgroundPaddingWidth -
        dims.darkroomPadding;

      // Only use 65% of screen height
      var maxScreenHeight = screen.height * dims.maxScreenHeightPercentage;

      // Calculate editor max height
      var editorHeight = H5P.$body.get(0).offsetHeight -
        dims.backgroundPaddingHeight - dims.popupHeaderHeight -
        dims.darkroomToolbarHeight - dims.darkroomPadding;

      // Use smallest of screen height and editor height,
      // we don't want to overflow editor or screen
      maxHeight = maxScreenHeight < editorHeight ? maxScreenHeight : editorHeight;
    };

    /**
     * Create image editing tool from image.
     */
    var createDarkroom = function () {
      window.requestAnimationFrame(function () {
        self.darkroom = new Darkroom('#h5p-editing-image-' + uniqueId, {
          initialize: function () {
            // Reset transformations
            this.transformations = [];

            H5P.$body.get(0).classList.add('h5p-editor-image-popup');
            background.classList.remove('hidden');
            imageLoading.classList.add('hidden');
            self.trigger('initialized');
          },
          maxWidth: maxWidth,
          maxHeight: maxHeight,
          plugins: {
            crop: {
              ratio: ratio || null
            },
            save : false
          }
        });
      });
    };

    /**
     * Load a script dynamically
     *
     * @param {string} path Path to script
     * @param {function} [callback]
     */
    var loadScript = function (path, callback) {
      $.ajax({
        url: path,
        dataType: 'script',
        success: function () {
          if (callback) {
            callback();
          }
        },
        async: true
      });
    };

    /**
     * Load scripts dynamically
     */
    var loadScripts = function () {
      loadScript(H5PEditor.basePath + 'libs/fabric.js', function () {
        loadScript(H5PEditor.basePath + 'libs/darkroom.js', function () {
          createDarkroom();
          scriptsLoaded = true;
        });
      });
    };

    /**
     * Grab canvas data and pass data to listeners.
     */
    var saveImage = function () {

      var isCropped = self.darkroom.plugins.crop.hasFocus();
      var canvas = self.darkroom.canvas.getElement();

      var convertData = function () {
        var newImage = self.darkroom.canvas.toDataURL();
        self.trigger('savedImage', newImage);
        canvas.removeEventListener('crop:update', convertData, false);
      };

      // Check if image has changed
      if (self.darkroom.transformations.length || isReset || isCropped) {

        if (isCropped) {
          //self.darkroom.plugins.crop.okButton.element.click();
          self.darkroom.plugins.crop.cropCurrentZone();

          canvas.addEventListener('crop:update', convertData, false);
        } else {
          convertData();
        }
      }

      isReset = false;
    };

    /**
     * Adjust popup offset.
     * Make sure it is centered on top of offset.
     *
     * @param {Object} [offset] Offset that popup should center on.
     * @param {number} [offset.top] Offset to top.
     */
    this.adjustPopupOffset = function (offset) {
      if (offset) {
        topOffset = offset.top;
      }

      // Only use 65% of screen height
      var maxScreenHeight = screen.height * 0.65;

      // Calculate editor max height
      var dims = ImageEditingPopup.staticDimensions;
      var backgroundHeight = H5P.$body.get(0).offsetHeight - dims.backgroundPaddingHeight;
      var popupHeightNoImage = dims.darkroomToolbarHeight + dims.popupHeaderHeight +
        dims.darkroomPadding;
      var editorHeight =  backgroundHeight - popupHeightNoImage;

      // Available editor height
      var availableHeight = maxScreenHeight < editorHeight ? maxScreenHeight : editorHeight;

      // Check if image is smaller than available height
      var actualImageHeight;
      if (editingImage.naturalHeight < availableHeight) {
        actualImageHeight = editingImage.naturalHeight;
      }
      else {
        actualImageHeight = availableHeight;

        // We must check ratio as well
        var imageRatio = editingImage.naturalHeight / editingImage.naturalWidth;
        var maxActualImageHeight = maxWidth * imageRatio;
        if (maxActualImageHeight < actualImageHeight) {
          actualImageHeight = maxActualImageHeight;
        }
      }

      var popupHeightWImage = actualImageHeight + popupHeightNoImage;
      var offsetCentered = topOffset - (popupHeightWImage / 2) -
        (dims.backgroundPaddingHeight / 2);

      // Min offset is 0
      offsetCentered = offsetCentered > 0 ? offsetCentered : 0;

      // Check that popup does not overflow editor
      if (popupHeightWImage + offsetCentered > backgroundHeight) {
        var newOffset = backgroundHeight - popupHeightWImage;
        offsetCentered = newOffset < 0 ? 0 : newOffset;
      }

      popup.style.top = offsetCentered + 'px';
    };

    /**
     * Set new image in editing tool
     *
     * @param {string} imgSrc Source of new image
     */
    this.setImage = function (imgSrc) {
      // Set new image
      var darkroom = popup.querySelector('.darkroom-container');
      if (darkroom) {
        darkroom.parentNode.removeChild(darkroom);
      }

      editingImage.src = imgSrc;
      imageLoading.classList.remove('hidden');
      editingImage.classList.add('hidden');
      editingContainer.appendChild(editingImage);

      createDarkroom();
    };

    /**
     * Show popup
     *
     * @param {Object} [offset] Offset that popup should center on.
     * @param {string} [imageSrc] Source of image that will be edited
     */
    this.show = function (offset, imageSrc) {
      H5P.$body.get(0).appendChild(background);
      setDarkroomDimensions();
      if (imageSrc) {

        // Load image editing scripts dynamically
        if (!scriptsLoaded) {
          editingImage.src = imageSrc;
          loadScripts();
        }
        else {
          self.setImage(imageSrc);
        }

        if (offset) {
          var imageLoaded = function () {
            this.adjustPopupOffset(offset);
            editingImage.removeEventListener('load', imageLoaded);
          }.bind(this);

          editingImage.addEventListener('load', imageLoaded);
        }
      }
      else {
        H5P.$body.get(0).classList.add('h5p-editor-image-popup');
        background.classList.remove('hidden');
        self.trigger('initialized');
      }

      isShowing = true;
    };

    /**
     * Hide popup
     */
    this.hide = function () {
      isShowing = false;
      H5P.$body.get(0).classList.remove('h5p-editor-image-popup');
      background.classList.add('hidden');
      H5P.$body.get(0).removeChild(background);
    };

    /**
     * Toggle popup visibility
     */
    this.toggle = function () {
      if (isShowing) {
        this.hide();
      } else {
        this.show();
      }
    };

    // Create header buttons
    createButton('resetToOriginalLabel', 'h5p-editing-image-reset-button h5p-remove', function () {
      self.trigger('resetImage');
      isReset = true;
    });
    createButton('cancelLabel', 'h5p-editing-image-cancel-button', function () {
      self.trigger('canceled');
      self.hide();
    });
    createButton('saveLabel', 'h5p-editing-image-save-button h5p-done', function () {
      saveImage();
      self.hide();
    });
  }

  ImageEditingPopup.prototype = Object.create(EventDispatcher.prototype);
  ImageEditingPopup.prototype.constructor = ImageEditingPopup;

  ImageEditingPopup.staticDimensions = {
    backgroundPaddingWidth: 32,
    backgroundPaddingHeight: 96,
    darkroomPadding: 64,
    darkroomToolbarHeight: 40,
    maxScreenHeightPercentage: 0.65,
    popupHeaderHeight: 59
  };

  return ImageEditingPopup;

}(H5P.jQuery, H5P.EventDispatcher));
;
var H5PEditor = H5PEditor || {};

/**
 * Audio/Video module.
 * Makes it possible to add audio or video through file uploads and urls.
 *
 */
H5PEditor.widgets.video = H5PEditor.widgets.audio = H5PEditor.AV = (function ($) {

  /**
   * Constructor.
   *
   * @param {mixed} parent
   * @param {object} field
   * @param {mixed} params
   * @param {function} setValue
   * @returns {_L3.C}
   */
  function C(parent, field, params, setValue) {
    var self = this;

    // Initialize inheritance
    H5PEditor.FileUploader.call(self, field);
	
    this.parent = parent;
    this.field = field;
    this.params = params;
    this.setValue = setValue;
    this.changes = [];

    if (params !== undefined && params[0] !== undefined) {
      this.setCopyright(params[0].copyright);
    }

    // When uploading starts
    self.on('upload', function () {
      // Insert throbber
      self.$uploading = $('<div class="h5peditor-uploading h5p-throbber">' + H5PEditor.t('core', 'uploading') + '</div>').insertAfter(self.$add.hide());

      // Clear old error messages
      self.$errors.html('');

      // Close dialog
      self.$addDialog.removeClass('h5p-open');
    });

    // Handle upload complete
    self.on('uploadComplete', function (event) {
      var result = event.data;

      try {
        if (result.error) {
          throw err;
        }

        // Set params if none is set
        if (self.params === undefined) {
          self.params = [];
          self.setValue(self.field, self.params);
        }

        // Add a new file/source
        var file = {
          path: result.data.path,
          mime: result.data.mime,
          copyright: self.copyright
        };
        var index = (self.updateIndex !== undefined ? self.updateIndex : self.params.length);
        self.params[index] = file;
        self.addFile(index);

        // Trigger change callbacks (old event system)
        for (var i = 0; i < self.changes.length; i++) {
          self.changes[i](file);
        }
      }
      catch (error) {
        // Display errors
        self.$errors.append(H5PEditor.createError(error));
      }

      if (self.$uploading !== undefined && self.$uploading.length !== 0) {
        // Hide throbber and show add button
        self.$uploading.remove();
        self.$add.show();
      }
    });
    
  }

  C.prototype = Object.create(ns.FileUploader.prototype);
  C.prototype.constructor = C;

  /**
   * Append widget to given wrapper.
   *
   * @param {jQuery} $wrapper
   */
  C.prototype.appendTo = function ($wrapper) {
    var self = this;

    var label = '';
    if (this.field.label !== 0) {
      label = '<span class="h5peditor-label' + (this.field.optional ? '' : ' h5peditor-required') + '">' + (this.field.label === undefined ? this.field.name : this.field.label) + '</span>';
    }
    console.log("label:"+label);

    var html = H5PEditor.createItem(this.field.type, label + '<div class="file">' + C.createAdd() + '</div><a class="h5p-copyright-button" href="#">' + ns.t('core', 'editCopyright') + '</a><div class="h5p-editor-dialog"><a href="#" class="h5p-close" title="' + ns.t('core', 'close') + '"></a></div>', this.field.description);
	//console.log("html:"+html);
    var $container = $(html).appendTo($wrapper);
    var $file = $container.children('.file');
    this.$add = $file.children('.h5p-add-file').click(function () {
      self.$addDialog.addClass('h5p-open');
    });
    this.$addDialog = this.$add.next();
    var $url = this.$addDialog.find('.h5p-file-url');
    this.$addDialog.find('.h5p-cancel').click(function () {
      self.updateIndex = undefined;
      $url.val('');
      self.$addDialog.removeClass('h5p-open');
    });
    this.$addDialog.find('.h5p-file-upload').click(function () {
      self.openFileSelector();
    });
    this.$addDialog.find('.h5p-insert').click(function () {
      self.useUrl($url.val().trim());
      self.$addDialog.removeClass('h5p-open');
      $url.val('');
    });

    this.$errors = $container.children('.h5p-errors');

    if (this.params !== undefined) {
      for (var i = 0; i < this.params.length; i++) {
        this.addFile(i);
      }
    }

  	var $dialog = $container.find('.h5p-editor-dialog');
    $container.find('.h5p-copyright-button').add($dialog.find('.h5p-close')).click(function () {
      $dialog.toggleClass('h5p-open');
      return false;
    });

    var group = new H5PEditor.widgets.group(self, H5PEditor.copyrightSemantics, self.copyright, function (field, value) {
      self.setCopyright(value);
    });
    group.appendTo($dialog);
    group.expand();
    group.$group.find('.title').remove();
    this.children = [group];
  	
    //h5pcustomization hide
	 try
    {
    
    window.setTimeout(function(){
		ns.$("#h5peditor-library").hide();
		ns.$(".h5peditor-tab-assets").click();
		ns.$(".h5peditor-tabs").hide();
		ns.$(".h5peditor-guided-tour").parent().hide();
		
		var htm ='<div class="addedit-new-field-title" style="width: 90px;font-family: ProximaNovaBold, Arial;padding: 6px 5px 2px 0;font-size: 12px;color: #474747;margin-bottom: -17px;">Video:</div>';
		ns.$(".h5peditor-panes .interactiveVideo").prepend(htm);
		
		
		ns.$(".h5peditor-panes .interactiveVideo").show();
		ns.$(".h5peditor-panes .interactiveVideo").prev().hide();
		ns.$(".h5peditor-panes .interactiveVideo").next().hide();
	
		
		
		
 		//window.parent.parent.document.getElementById("iframe_editor").style.display="block";
		//window.parent.parent.document.getElementById("h5pe1loading").style.display="none";
		
		
		$(".form-item-title").hide();
		$(".form-item-h5p-type").hide();
		//$(".h5peditor-form .h5peditor-label").hide();
		
		},2000);
		}catch(e){}
		
    
  };

  /**
   * Add file icon with actions.
   *
   * @param {Number} index
   */
  C.prototype.addFile = function (index) {
    var that = this;
    var file = this.params[index];

    if (that.updateIndex !== undefined) {
      this.$add.parent().children(':eq(' + index + ')').find('.h5p-type').attr('title', file.mime).text(file.mime.split('/')[1]);
      this.updateIndex = undefined;
      return;
    }

    var $file = $('<div class="h5p-thumbnail"><div class="h5p-type" title="' + file.mime + '">' + file.mime.split('/')[1] + '</div><div role="button" tabindex="1" class="h5p-remove" title="' + H5PEditor.t('core', 'removeFile') + '"></div></div>')
      .insertBefore(this.$add)
      .click(function () {
        if (!that.$add.is(':visible')) {
          return; // Do not allow editing of file while uploading
        }
        that.$addDialog.addClass('h5p-open').find('.h5p-file-url').val(that.params[index].path);
        that.updateIndex = index;
      })
      .children('.h5p-remove')
        .click(function () {
          if (that.$add.is(':visible')) {
            confirmRemovalDialog.show($file.offset().top);
          }

          return false;
        })
        .end();

    // Create remove file dialog
    var confirmRemovalDialog = new H5P.ConfirmationDialog({
      headerText: H5PEditor.t('core', 'removeFile'),
      dialogText: H5PEditor.t('core', 'confirmRemoval', {':type': 'file'})
    }).appendTo(document.body);

    // Remove file on confirmation
    confirmRemovalDialog.on('confirmed', function () {
      // Remove from params.
      if (that.params.length === 1) {
        delete that.params;
        that.setValue(that.field);
      }
      else {
        that.params.splice(index, 1);
      }

      $file.remove();

      for (var i = 0; i < that.changes.length; i++) {
        that.changes[i]();
      }
    });
  };

  C.prototype.useUrl = function (url) {
    if (this.params === undefined) {
      this.params = [];
      this.setValue(this.field, this.params);
    }

    var mime;
    var matches = url.match(/\.(webm|mp4|ogv|m4a|mp3|ogg|oga|wav)/i);
    if (matches !== null) {
      mime = matches[matches.length - 1];
    }
    else {
      // Try to find a provider
      for (var i = 0; i < C.providers.length; i++) {
        if (C.providers[i].regexp.test(url)) {
          mime = C.providers[i].name;
          break;
        }
      }
    }

    var file = {
      path: url,
      mime: this.field.type + '/' + (mime ? mime : 'unknown'),
      copyright: this.copyright
    };
    var index = (this.updateIndex !== undefined ? this.updateIndex : this.params.length);
    this.params[index] = file;
    this.addFile(index);

    for (var i = 0; i < this.changes.length; i++) {
      this.changes[i](file);
    }
  };

  /**
   * Validate the field/widget.
   *
   * @returns {Boolean}
   */
  C.prototype.validate = function () {
    return true;
  };

  /**
   * Remove this field/widget.
   */
  C.prototype.remove = function () {
    // TODO: Check what happens when removed during upload.
    this.$errors.parent().remove();
  };

  /**
   * Sync copyright between all video files.
   *
   * @returns {undefined}
   */
  C.prototype.setCopyright = function (value) {
    this.copyright = value;
    if (this.params !== undefined) {
      for (var i = 0; i < this.params.length; i++) {
        this.params[i].copyright = value;
      }
    }
  };

  /**
   * Collect functions to execute once the tree is complete.
   *
   * @param {function} ready
   * @returns {undefined}
   */
  C.prototype.ready = function (ready) {
    if (this.passReadies) {
      this.parent.ready(ready);
    }
    else {
      ready();
    }
  };

  /**
   * HTML for add button.
   *
   * @returns {String}
   */
  C.createAdd = function () {
    return '<div role="button" tabindex="1" class="h5p-add-file" title="' + H5PEditor.t('core', 'addFile') + '"></div><div class="h5p-add-dialog"><div class="h5p-dialog-box"><button class="h5p-file-upload">Select file to upload</button></div><div class="h5p-or"><span>or</span></div><div class="h5p-dialog-box"><input type="text" placeholder="Type in file url (YouTube is supported for videos)" class="h5p-file-url h5peditor-text"/></div><div class="h5p-buttons"><button class="h5p-insert">Insert</button><button class="h5p-cancel">Cancel</button></div></div>';
  };

  /**
   * Providers incase mime type is unknown.
   * @public
   */
  C.providers = [{
    name: 'YouTube',
    regexp: /^https?:\/\/(youtu.be|(www.)?youtube.com)\//i
  }];

  return C;
})(H5P.jQuery);


function formSubmitIframe()
{
	alert(2);
}
;
var H5PEditor = H5PEditor || {};
var ns = H5PEditor;

/**
 * Create a group of fields.
 *
 * @param {mixed} parent
 * @param {object} field
 * @param {mixed} params
 * @param {function} setValue
 * @returns {ns.Group}
 */
ns.Group = function (parent, field, params, setValue) {
  // Support for events
  H5P.EventDispatcher.call(this);

  if (field.label === undefined) {
    field.label = field.name;
  }
  else if (field.label === 0) {
    field.label = '';
  }

  this.parent = parent;
  this.passReadies = true;
  this.params = params;
  this.setValue = setValue;
  this.library = parent.library + '/' + field.name;

  if (field.deprecated !== undefined && field.deprecated) {
    this.field = H5P.cloneObject(field, true);
    var empties = 0;
    for (var i = 0; i < this.field.fields.length; i++) {
      var f = this.field.fields[i];
      if (params !== undefined && params[f.name] === '') {
        delete params[f.name];
      }
      if (params === undefined || params[f.name] === undefined) {
        f.widget = 'none';
        empties++;
      }
    }
    if (i === empties) {
      this.field.fields = [];
    }
  }
  else {
    this.field = field;
  }

  if (this.field.optional === true) {
    // If this field is optional, make sure child fields are aswell
    for (var j = 0; j < this.field.fields.length; j++) {
      this.field.fields[j].optional = true;
    }
  }
};

// Extends the event dispatcher
ns.Group.prototype = Object.create(H5P.EventDispatcher.prototype);
ns.Group.prototype.constructor = ns.Group;

/**
 * Append group to its wrapper.
 *
 * @param {jQuery} $wrapper
 * @returns {undefined}
 */
ns.Group.prototype.appendTo = function ($wrapper) {
  var that = this;

  if (this.field.fields.length === 0) {
    // No fields or all are deprecated
    this.setValue(this.field);
    return;
  }

  // Add fieldset wrapper for group
  this.$group = ns.$('<fieldset/>', {
    'class': 'field group',
    appendTo: $wrapper
  });

  // Add title expand/collapse button
  ns.$('<div/>', {
    'class': 'title',
    title: ns.t('core', 'expandCollapse'),
    role: 'button',
    tabIndex: 0,
    on: {
      click: function () {
        that.toggle();
      },
      keypress: function (event) {
        if ((event.charCode || event.keyCode) === 32) {
          that.toggle();
        }
      }
    },
    appendTo: this.$group
  });

  // Add content container
  var $content = ns.$('<div/>', {
    'class': 'content',
    appendTo: this.$group
  });

  if (this.field.fields.length === 1) {
    $content.addClass('h5peditor-single');
    this.children = [];
    var field = this.field.fields[0];
    var widget = field.widget === undefined ? field.type : field.widget;
    this.children[0] = new ns.widgets[widget](this, field, this.params, function (field, value) {
      that.setValue(that.field, value);
    });
    this.children[0].appendTo($content);
  }
  else {
    if (this.params === undefined) {
      this.params = {};
      this.setValue(this.field, this.params);
    }
    ns.processSemanticsChunk(this.field.fields, this.params, $content, this);
  }

  // Set summary
  this.findSummary();

  // Check if group should be expanded.
  // Default is to be collapsed unless explicity defined in semantics by optional attribute expanded
  if (this.field.expanded === true) {
    this.expand();
  }
};

/**
 * Toggle expand/collapse for the given group.
 */
ns.Group.prototype.toggle = function () {
  if (this.$group.hasClass('expanded')) {
    this.collapse();
  }
  else {
    this.expand();
  }
};

/**
 * Expand the given group.
 */
ns.Group.prototype.expand = function () {
  this.$group.addClass('expanded');
  this.trigger('expanded');
};

/**
 * Collapse the given group (if valid)
 */
ns.Group.prototype.collapse = function () {
  // Do not collapse before valid!
  var valid = true;
  for (var i = 0; i < this.children.length; i++) {
    if (this.children[i].validate() === false) {
      valid = false;
    }
  }
  if (valid) {
    this.$group.removeClass('expanded');
    this.trigger('collapsed');
  }
};

/**
 * Find summary to display in group header.
 */
ns.Group.prototype.findSummary = function () {
  var that = this;
  var summary;
  for (var j = 0; j < this.children.length; j++) {
    var child = this.children[j];
    if (child.field === undefined) {
      continue;
    }
    var params = this.field.fields.length === 1 ? this.params : this.params[child.field.name];
    var widget = ns.getWidgetName(child.field);

    if (widget === 'text') {
      if (params !== undefined && params !== '') {
        summary = params.replace(/(<([^>]+)>)/ig, "");
      }

      child.$input.change(function () {
        var params = that.field.fields.length === 1 ? that.params : that.params[child.field.name];
        if (params !== undefined && params !== '') {
          that.setSummary(params.replace(/(<([^>]+)>)/ig, ""));
        }
      });
      break;
    }
    else if (widget === 'library') {
      if (params !== undefined) {
        summary = child.$select.children(':selected').text();
      }
      child.change(function (library) {
      	if(library != undefined)
        	that.setSummary(library.title);
      });
      break;
    }
  }
  this.setSummary(summary);
};

/**
 * Set the given group summary.
 *
 * @param {string} summary
 * @returns {undefined}
 */
ns.Group.prototype.setSummary = function (summary) {
  var summaryText;

  // Parse html
  var summaryTextNode = ns.$.parseHTML(summary);

  if (summaryTextNode !== null) {
    summaryText = summaryTextNode[0].nodeValue;
  }

  if (summaryText !== undefined) {
    summaryText = this.field.label + ': ' + (summaryText.length > 48 ? summaryText.substr(0, 45) + '...' : summaryText);
  }
  else {
    summaryText = this.field.label;
  }

  this.$group.children('.title').html(summaryText);
};

/**
 * Validate all children.
 */
ns.Group.prototype.validate = function () {
  var valid = true;

  if (this.children !== undefined) {
    for (var i = 0; i < this.children.length; i++) {
      if (this.children[i].validate() === false) {
        valid = false;
      }
    }
  }

  return valid;
};

/**
 * Allows ancestors and widgets to do stuff with our children.
 *
 * @public
 * @param {Function} task
 */
ns.Group.prototype.forEachChild = function (task) {
  for (var i = 0; i < this.children.length; i++) {
    task(this.children[i], i);
  }
};

/**
 * Collect functions to execute once the tree is complete.
 *
 * @param {function} ready
 * @returns {undefined}
 */
ns.Group.prototype.ready = function (ready) {
  this.parent.ready(ready);
};

/**
 * Remove this item.
 */
ns.Group.prototype.remove = function () {
  if (this.$group !== undefined) {
    ns.removeChildren(this.children);
    this.$group.remove();
  }
};

// Tell the editor what widget we are.
ns.widgets.group = ns.Group;
;
var H5PEditor = H5PEditor || {};
var ns = H5PEditor;

/**
 * Creates a boolean field for the editor.
 *
 * @param {mixed} parent
 * @param {object} field
 * @param {mixed} params
 * @param {function} setValue
 * @returns {ns.Boolean}
 */
ns.Boolean = function (parent, field, params, setValue) {
  if (params === undefined) {
    this.value = false;
    setValue(field, this.value);
  }
  else {
    this.value = params;
  }

  this.field = field;
  this.setValue = setValue;
};

/**
 * Create HTML for the boolean field.
 */
ns.Boolean.prototype.createHtml = function () {
  var input = '<input type="checkbox"';
  if (this.value !== undefined && this.value) {
    input += ' checked="checked"';
  }
  input += '/>';

  var html = '<label class="h5peditor-label">' + input;
  if (this.field.label !== 0) {
    html += this.field.label === undefined ? this.field.name : this.field.label;
  }
  html += '</label>';

  return ns.createItem(this.field.type, html, this.field.description);
};

/**
 * "Validate" the current boolean field.
 */
ns.Boolean.prototype.validate = function () {
  return true;
};

/**
 * Append the boolean field to the given wrapper.
 *
 * @param {jQuery} $wrapper
 * @returns {undefined}
 */
ns.Boolean.prototype.appendTo = function ($wrapper) {
  var that = this;

  this.$item = ns.$(this.createHtml()).appendTo($wrapper);
  this.$input = this.$item.children('label').children('input');
  this.$errors = this.$item.children('.h5p-errors');

  this.$input.change(function () {
    // Validate
    that.value = that.$input.is(':checked') ? true : false;
    that.setValue(that.field, that.value);
  });
};

/**
 * Remove this item.
 */
ns.Boolean.prototype.remove = function () {
  this.$item.remove();
};

// Tell the editor what widget we are.
ns.widgets['boolean'] = ns.Boolean;;
/** @namespace H5PEditor */
var H5PEditor = H5PEditor || {};

H5PEditor.List = (function ($) {
  /**
   * List structure.
   *
   * @class
   * @param {*} parent structure
   * @param {Object} field Semantic description of field
   * @param {Array} [parameters] Default parameters for this field
   * @param {Function} setValue Call to set our parameters
   */
  function List(parent, field, parameters, setValue) {
    var self = this;

    // Initialize semantics structure inheritance
    H5PEditor.SemanticStructure.call(self, field, {
      name: 'ListEditor',
      label: H5PEditor.t('core', 'listLabel')
    });

    // Make it possible to travel up tree.
    self.parent = parent; // (Could this be done a better way in the future?)

    /**
     * Keep track of child fields. Should not be exposed directly,
     * create functions for using or finding the children.
     *
     * @private
     * @type {Array}
     */
    var children = [];

    // Prepare the old ready callback system
    var readyCallbacks = [];
    var passReadyCallbacks = true;
    parent.ready(function () {
      passReadyCallbacks = false;
    }); // (In the future we should use the event system for this, i.e. self.once('ready'))

    // Listen for widget changes
    self.on('changeWidget', function () {
      // Append all items to new widget
      for (var i = 0; i < children.length; i++) {
        self.widget.addItem(children[i], i);
      }
    });

    /**
     * Add all items to list without appending to DOM.
     *
     * @public
     */
    var init = function () {
      var i;
      if (parameters !== undefined && parameters.length) {
        for (i = 0; i < parameters.length; i++) {
          if (parameters[i] === null) {
            parameters[i] = undefined;
          }
          addItem(i);
        }
      }
      else {
        if (field.defaultNum === undefined) {
          // Use min or 1 if no default item number is set.
          field.defaultNum = (field.min !== undefined ? field.min : 1);
        }
        // Add default number of fields.
        for (i = 0; i < field.defaultNum; i++) {
          addItem(i);
        }
      }
    };

    /**
     * Make sure list is created when setting a parameter.
     *
     * @private
     * @param {number} index
     * @param {*} value
     */
    var setParameters = function (index, value) {
      if (parameters === undefined) {
        // Create new parameters for list
        parameters = [];
        setValue(field, parameters);
      }
      parameters[index] = value;
    };

    /**
     * Add item to list.
     *
     * @private
     * @param {Number} index
     * @param {*} [paramsOverride] Override params using this value.
     */
    var addItem = function (index, paramsOverride) {
      var childField = field.field;
      var widget = H5PEditor.getWidgetName(childField);

      if ((parameters === undefined || parameters[index] === undefined) && childField['default'] !== undefined) {
        // Use default value
        setParameters(index, childField['default']);
      }
      if (paramsOverride !== undefined) {
        // Use override params
        setParameters(index, paramsOverride);
      }

      var child = children[index] = new H5PEditor.widgets[widget](self, childField, parameters === undefined ? undefined : parameters[index], function (myChildField, value) {
        var i = findIndex(child);
        setParameters(i === undefined ? index : i, value);
      });

      if (!passReadyCallbacks) {
        // Run collected ready callbacks
        for (var i = 0; i < readyCallbacks.length; i++) {
          readyCallbacks[i]();
        }
        readyCallbacks = []; // Reset
      }

      return child;
    };

    /**
     * Finds the index for the given child.
     *
     * @private
     * @param {Object} child field instance
     * @returns {Number} index
     */
    var findIndex = function (child) {
      for (var i = 0; i < children.length; i++) {
        if (children[i] === child) {
          return i;
        }
      }
    };

    /**
     * Get the singular form of the items added in the list.
     *
     * @public
     * @returns {String} The entity type
     */
    self.getEntity = function () {
      return (field.entity === undefined ? 'item' : field.entity);
    };

    /**
     * Adds a new list item and child field at the end of the list
     *
     * @public
     * @param {*} [paramsOverride] Override params using this value.
     * @returns {Boolean}
     */
    self.addItem = function (paramsOverride) {
      var id = children.length;
      if (field.max === id) {
        return false;
      }

      var child = addItem(id, paramsOverride);
      self.widget.addItem(child, id);
      return true;
    };

    /**
     * Removes the list item at the given index.
     *
     * @public
     * @param {Number} index
     */
    self.removeItem = function (index) {
      // Remove child field
      children[index].remove();
      children.splice(index, 1);

      if (parameters !== undefined) {
        // Clean up parameters
        parameters.splice(index, 1);
        if (!parameters.length) {
          // Create new parameters for list
          parameters = undefined;
          setValue(field);
        }
      }
    };

    /**
     * Removes all items.
     * This is useful if a widget wants to reset the list.
     *
     * @public
     */
    self.removeAllItems = function () {
      if (parameters === undefined) {
        return;
      }

      // Remove child fields
      for (var i = 0; i < children.length; i++) {
        children[i].remove();
      }
      children = [];

      // Clean up parameters
      parameters = undefined;
      setValue(field);
    };

    /**
     * Change the order of the items in the list.
     * Be aware that this may change the index of other existing items.
     *
     * @public
     * @param {Number} currentIndex
     * @param {Number} newIndex
     */
    self.moveItem = function (currentIndex, newIndex) {
      // Update child fields
      var child = children.splice(currentIndex, 1);
      children.splice(newIndex, 0, child[0]);

      // Update parameters
      if (parameters) {
        var params = parameters.splice(currentIndex, 1);
        parameters.splice(newIndex, 0, params[0]);
      }
    };

    /**
     * Allows ancestors and widgets to do stuff with our children.
     *
     * @public
     * @param {Function} task
     */
    self.forEachChild = function (task) {
      for (var i = 0; i < children.length; i++) {
        task(children[i], i);
      }
    };

    /**
     * Collect callback to run when the editor is ready. If this item isn't
     * ready yet, jusy pass them on to the parent item.
     *
     * @public
     * @param {Function} ready
     */
    self.ready = function (ready) {
      if (passReadyCallbacks) {
        parent.ready(ready);
      }
      else {
        readyCallbacks.push(ready);
      }
    };

    /**
     * Make sure that this field and all child fields are valid.
     *
     * @public
     * @returns {Boolean}
     */
    self.validate = function () {
      var self = this;
      var valid = true;

      // Remove old error messages
      self.clearErrors();

      // Make sure child fields are valid
      for (var i = 0; i < children.length; i++) {
        if (children[i].validate() === false) {
          valid = false;
        }
      }

      // Validate our self
      if (field.max !== undefined && field.max > 0 &&
          parameters !== undefined && parameters.length > field.max) {
        // Invalid, more parameters than max allowed.
        valid = false;
        self.setError(H5PEditor.t('core', 'exceedsMax', {':property': '<em>' + self.label + '</em>', ':max': field.max}));
      }
      if (field.min !== undefined && field.min > 0 &&
          (parameters === undefined || parameters.length < field.min)) {
        // Invalid, less parameters than min allowed.
        valid = false;
        self.setError(H5PEditor.t('core', 'belowMin', {':property': '<em>' + self.label + '</em>', ':min': field.min}));
      }

      return valid;
    };

    /**
     * Creates a copy of the current valid value. A copy is created to avoid
     * mistakes like directly editing the parameter values, which will cause
     * inconsistencies between the parameters and the editor widgets.
     *
     * @public
     * @returns {Array}
     */
    self.getValue = function () {
      return (parameters === undefined ? parameters : $.extend(true, [], parameters));
    };

    // Start the party!
    init();
  }

  // Extends the semantics structure
  List.prototype = Object.create(H5PEditor.SemanticStructure.prototype);
  List.prototype.constructor = List;

  return List;
})(H5P.jQuery);

// Register widget
H5PEditor.widgets.list = H5PEditor.List;
;
/** @namespace H5PEditor */
var H5PEditor = H5PEditor || {};

H5PEditor.ListEditor = (function ($) {

  /**
   * Draws the list.
   *
   * @class
   * @param {List} list
   */
  function ListEditor(list) {
    var self = this;

    var entity = list.getEntity();

    // Create list html
    var $list = $('<ul/>', {
      'class': 'h5p-ul'
    });

    // Create add button
    var $button = $('<button/>', {
      text: H5PEditor.t('core', 'addEntity', {':entity': entity})
    }).click(function () {
      list.addItem();
    });

    // Used when dragging items around
    var adjustX, adjustY, marginTop, formOffset;

    /**
     * @private
     * @param {jQuery} $item
     * @param {jQuery} $placeholder
     * @param {Number} x
     * @param {Number} y
     */
    var moveItem = function ($item, $placeholder, x, y) {
      var currentIndex;

      // Adjust so the mouse is placed on top of the icon.
      x = x - adjustX;
      y = y - adjustY;
      $item.css({
        top: y - marginTop - formOffset.top,
        left: x - formOffset.left
      });

      // Try to move up.
      var $prev = $item.prev().prev();
      if ($prev.length && y < $prev.offset().top + ($prev.height() / 2)) {
        $prev.insertAfter($item);

        currentIndex = $item.index();
        list.moveItem(currentIndex, currentIndex - 1);

        return;
      }

      // Try to move down.
      var $next = $item.next();
      if ($next.length && y + $item.height() > $next.offset().top + ($next.height() / 2)) {
        $next.insertBefore($placeholder);

        currentIndex = $item.index() - 2;
        list.moveItem(currentIndex, currentIndex + 1);
      }
    };

    /**
     * Adds UI items to the widget.
     *
     * @public
     * @param {Object} item
     */
    self.addItem = function (item) {
      var $placeholder;
      var $item = $('<li/>', {
        'class' : 'h5p-li',
      });

      // Create confirmation dialog for removing list item
      var confirmRemovalDialog = new H5P.ConfirmationDialog({
        dialogText: H5PEditor.t('core', 'confirmRemoval', {':type': entity})
      }).appendTo(document.body);

      // Remove list item on confirmation
      confirmRemovalDialog.on('confirmed', function () {
        list.removeItem($item.index());
        $item.remove();
      });

      /**
       * Mouse move callback
       *
       * @private
       * @param {Object} event
       */
      var move = function (event) {
        moveItem($item, $placeholder, event.pageX, event.pageY);
      };

      /**
       * Mouse button release callback
       *
       * @private
       */
      var up = function () {
        H5P.$body
          .unbind('mousemove', move)
          .unbind('mouseup', up)
          .unbind('mouseleave', up)
          .attr('unselectable', 'off')
          .css({
            '-moz-user-select': '',
            '-webkit-user-select': '',
            'user-select': '',
            '-ms-user-select': ''
          })
          [0].onselectstart = H5P.$body[0].ondragstart = null;

        $item.removeClass('moving').css({
          width: 'auto',
          height: 'auto'
        });
        $placeholder.remove();
      };

      /**
       * Mouse button down callback
       *
       * @private
       */
      var down = function (event) {
        if (event.which !== 1) {
          return; // Only allow left mouse button
        }

        // Prevent wysiwyg becoming unresponsive
        H5PEditor.Html.removeWysiwyg();

        // Start tracking mouse
        H5P.$body
          .attr('unselectable', 'on')
          .mouseup(up)
          .bind('mouseleave', up)
          .css({
            '-moz-user-select': 'none',
            '-webkit-user-select': 'none',
            'user-select': 'none',
            '-ms-user-select': 'none'
          })
          .mousemove(move)
          [0].onselectstart = H5P.$body[0].ondragstart = function () {
            return false;
          };

        var offset = $item.offset();
        adjustX = event.pageX - offset.left;
        adjustY = event.pageY - offset.top;
        marginTop = parseInt($item.css('marginTop'));
        formOffset = $list.offsetParent().offset();
        // TODO: Couldn't formOffset and margin be added?

        var width = $item.width();
        var height = $item.height();

        $item.addClass('moving').css({
          width: width,
          height: height
        });
        $placeholder = $('<li/>', {
          'class': 'placeholder h5p-li',
          css: {
            width: width,
            height: height
          }
        }).insertBefore($item);

        move(event);
        return false;
      };

      // List item title bar
      var $titleBar = $('<div/>', {
        'class': 'list-item-title-bar',
        appendTo: $item
      });

      // Append order button
      $('<div/>', {
        'class' : 'order',
        role: 'button',
        tabIndex: 1,
        on: {
          mousedown: down
        }
      }).appendTo($titleBar);

      // Append remove button
      $('<div/>', {
        'class' : 'remove',
        role: 'button',
        tabIndex: 1,
        on: {
          click: function () {
            confirmRemovalDialog.show($(this).offset().top);
          }
        }
      }).appendTo($titleBar);

      // Append new field item to content wrapper
      if (item instanceof H5PEditor.Group) {
        // Append to item
        item.appendTo($item);
        $item.addClass('listgroup');

        // Move label
        $item.children('.field').children('.title').appendTo($titleBar).addClass('h5peditor-label');

        // Handle expand and collapse
        item.on('expanded', function () {
          $item.addClass('expanded').removeClass('collapsed');
        });
        item.on('collapsed', function () {
          $item.removeClass('expanded').addClass('collapsed');
        });
      }
      else {
        // Append content wrapper
        var $content = $('<div/>', {
          'class' : 'content'
        }).appendTo($item);

        // Append field
        item.appendTo($content);

        if (item.field.label !== 0) {
          // Try to find and move the label to the title bar
          $content.children('.field').find('.h5peditor-label:first').appendTo($titleBar);
        }
      }

      // Append item to list
      $item.appendTo($list);

      // Good UX: automatically expand groups if not explicitly disabled by semantics
      if (item instanceof H5PEditor.Group) {
        item.expand();
      }
    };

    /**
     * Puts this widget at the end of the given container.
     *
     * @public
     * @param {jQuery} $container
     */
    self.appendTo = function ($container) {
      $list.appendTo($container);
      $button.appendTo($container);
    };

    /**
     * Remove this widget from the editor DOM.
     *
     * @public
     */
    self.remove = function () {
      $list.remove();
      $button.remove();
    };
  }

  return ListEditor;
})(H5P.jQuery);
;
var H5PEditor = (H5PEditor || {});
var ns = H5PEditor;

/**
 * Callback for setting new parameters.
 *
 * @callback H5PEditor.newParams
 * @param {Object} field Current field details.
 * @param {Object} params New parameters.
 */

/**
 * Create a field where one can select and include another library to the form.
 *
 * @class H5PEditor.Library
 * @extends H5P.EventDispatcher
 * @param {Object} parent Parent field in editor.
 * @param {Object} field Details for current field.
 * @param {Object} params Default parameters.
 * @param {newParams} setValue Callback for setting new parameters.
 */
ns.Library = function (parent, field, params, setValue) {
  var self = this;
  H5P.EventDispatcher.call(this);
  if (params === undefined) {
    this.params = {
      params: {}
    };
    // If you do a console log here it might show that this.params is
    // something else than what we set it to. One of life's big mysteries...
    setValue(field, this.params);
  } else {
    this.params = params;
  }
  this.field = field;
  this.parent = parent;
  this.changes = [];
  this.optionsLoaded = false;
  this.library = parent.library + '/' + field.name;

  this.passReadies = true;
  parent.ready(function () {
    self.passReadies = false;
  });

  // Confirmation dialog for changing library
  this.confirmChangeLibrary = new H5P.ConfirmationDialog({
    headerText: H5PEditor.t('core', 'changeLibrary'),
    dialogText: H5PEditor.t('core', 'confirmChangeLibrary')
  }).appendTo(document.body);

  // Load library on confirmation
  this.confirmChangeLibrary.on('confirmed', function () {
    self.loadLibrary(self.$select.val());
  });

  // Revert to current library on cancel
  this.confirmChangeLibrary.on('canceled', function () {
    self.$select.val(self.currentLibrary);
  });
};

ns.Library.prototype = Object.create(H5P.EventDispatcher.prototype);
ns.Library.prototype.constructor = ns.Library;

/**
 * Append the library selector to the form.
 *
 * @alias H5PEditor.Library#appendTo
 * @param {H5P.jQuery} $wrapper
 */
ns.Library.prototype.appendTo = function ($wrapper) {
  var that = this;
  var html = '';
  if (this.field.label !== 0) {
    html = '<label class="h5peditor-label' + (this.field.optional ? '' : ' h5peditor-required') + '">' + (this.field.label === undefined ? this.field.name : this.field.label) + '</label>';
  }

  html = '<div class="field ' + this.field.type + '">' + html + '<select>' + ns.createOption('-', 'Loading...') + '</select>';
  if (this.field.description !== undefined) {
    html += '<div class="h5peditor-field-description">' + this.field.description + '</div>';
  }
  // TODO: Remove errors, it is deprecated
  html += '<div class="errors h5p-errors"></div><div class="libwrap"></div></div>';

  this.$myField = ns.$(html).appendTo($wrapper);
  this.$select = this.$myField.children('select');
  this.$libraryWrapper = this.$myField.children('.libwrap');
  ns.LibraryListCache.getLibraries(that.field.options, that.librariesLoaded, that);
};

/**
 * Handler for when the library list has been loaded
 *
 * @alias H5PEditor.Library#librariesLoaded
 * @param {Array} libList
 */
ns.Library.prototype.librariesLoaded = function (libList) {
  this.libraries = libList;
  var self = this;
  var options = ns.createOption('-', '-');
  for (var i = 0; i < self.libraries.length; i++) {
    var library = self.libraries[i];
    if (library.uberName === self.params.library ||
        (library.title !== undefined && (library.restricted === undefined || !library.restricted))) {
      options += ns.createOption(library.uberName, library.title, library.uberName === self.params.library);
    }
  }

  self.$select.html(options).change(function () {
    // Use timeout to avoid bug in Chrome >44, when confirm is used inside change event.
    // Ref. https://code.google.com/p/chromium/issues/detail?id=525629
    setTimeout(function () {

      // Check if library is selected
      if (self.params.library) {

        // Confirm changing library
        self.confirmChangeLibrary.show(self.$select.offset().top);
      } else {

        // Load new library
        self.loadLibrary(self.$select.val());
      }
    }, 0);
  });

  if (self.libraries.length === 1) {
    self.$select.hide();
    self.$myField.children('.h5peditor-label').hide();
    self.loadLibrary(self.$select.children(':last').val(), true);
  }

  if (self.runChangeCallback === true) {
    // In case a library has been selected programmatically trigger change events, e.g. a default library.
    self.change();
    self.runChangeCallback = false;
  }
  // Load default library.
  if (this.params.library !== undefined) {
    self.loadLibrary(this.params.library, true);
  }
};

/**
 * Load the selected library.
 *
 * @alias H5PEditor.Library#loadLibrary
 * @param {string} libraryName On the form machineName.majorVersion.minorVersion
 * @param {boolean} [preserveParams]
 */
ns.Library.prototype.loadLibrary = function (libraryName, preserveParams) {
  var that = this;

  this.removeChildren();

  if (libraryName === '-') {
    delete this.params.library;
    delete this.params.params;
    delete this.params.subContentId;
    this.$libraryWrapper.attr('class', 'libwrap');
    return;
  }
  

	//h5pcustomize
  //this.$libraryWrapper.html(ns.t('core', 'loading', {':type': 'semantics'})).attr('class', 'libwrap ' + libraryName.split(' ')[0].toLowerCase().replace('.', '-') + '-editor');
  this.$libraryWrapper.html("<div id='loaderdiv'></div>").attr('class', 'libwrap ' + libraryName.split(' ')[0].toLowerCase().replace('.', '-') + '-editor');
  //window.parent.EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.createLoader("contentauthor-addedit-form");
  //createLoaderH5P("h5p-dialog");
  ns.loadLibrary(libraryName, function (semantics) {
  	//destroyLoaderH5P("h5p-dialog");
    that.currentLibrary = libraryName;
    that.params.library = libraryName;

    if (preserveParams === undefined || !preserveParams) {
      // Reset params
      that.params.params = {};
    }
    if (that.params.subContentId === undefined) {
      that.params.subContentId = H5P.createUUID();
    }

    ns.processSemanticsChunk(semantics, that.params.params, that.$libraryWrapper.html(''), that);

	
    if (that.libraries !== undefined) {
      that.change();
    }
    else {
      that.runChangeCallback = true;
    }
    });
  
  
};

/**
 * Add the given callback or run it.
 *
 * @alias H5PEditor.Library#change
 * @param {Function} callback
 */
ns.Library.prototype.change = function (callback) {
  if (callback !== undefined) {
    // Add callback
    this.changes.push(callback);
  }
  else {
    // Find library
    var library, i;
    for (i = 0; i < this.libraries.length; i++) {
      if (this.libraries[i].uberName === this.currentLibrary) {
        library = this.libraries[i];
        break;
      }
    }

    // Run callbacks
    for (i = 0; i < this.changes.length; i++) {
      this.changes[i](library);
    }
  }
};

/**
 * Validate this field and its children.
 *
 * @alias H5PEditor.Library#validate
 * @returns {boolean}
 */
ns.Library.prototype.validate = function () {
  if (this.params.library === undefined) {
    return (this.field.optional === true);
  }

  var valid = true;

  if (this.children) {
    for (var i = 0; i < this.children.length; i++) {
      if (this.children[i].validate() === false) {
        valid = false;
      }
    }
  }

  return valid;
};

/**
 * Collect functions to execute once the tree is complete.
 *
 * @alias H5PEditor.Library#ready
 * @param {Function} ready
 */
ns.Library.prototype.ready = function (ready) {
  if (this.passReadies) {
    this.parent.ready(ready);
  }
  else {
    this.readies.push(ready);
  }
};

/**
 * Custom remove children that supports common fields.
 *
 * * @alias H5PEditor.Library#removeChildren
 */
ns.Library.prototype.removeChildren = function () {
  if (this.currentLibrary === '-' || this.children === undefined) {
    return;
  }

  var ancestor = ns.findAncestor(this.parent);

  for (var libraryPath in ancestor.commonFields) {
    var library = libraryPath.split('/')[0];

    if (library === this.currentLibrary) {
      var remove = false;

      for (var fieldName in ancestor.commonFields[libraryPath]) {
        var field = ancestor.commonFields[libraryPath][fieldName];
        if (field.parents.length === 1) {
          field.instance.remove();
          remove = true;
        }

        for (var i = 0; i < field.parents.length; i++) {
          if (field.parents[i] === this) {
            field.parents.splice(i, 1);
            field.setValues.splice(i, 1);
          }
        }
      }

      if (remove) {
        delete ancestor.commonFields[libraryPath];
      }
    }
  }
  ns.removeChildren(this.children);
};

/**
 * Allows ancestors and widgets to do stuff with our children.
 *
 * @alias H5PEditor.Library#forEachChild
 * @param {Function} task
 */
ns.Library.prototype.forEachChild = function (task) {
  for (var i = 0; i < this.children.length; i++) {
    if (task(this.children[i], i)) {
      return;
    }
  }
};

/**
 * Called when this item is being removed.
 *
 * @alias H5PEditor.Library#remove
 */
ns.Library.prototype.remove = function () {
  this.removeChildren();
  if (this.$select !== undefined) {
    this.$select.parent().remove();
  }
};

// Tell the editor what widget we are.
ns.widgets.library = ns.Library;


function createLoaderH5P(resultPanel)
{
            var divid= "loaderdiv"+resultPanel;
            if(document.getElementById(divid)==null && ns.$("."+resultPanel).size() > 0)
            {
                var divobj=document.createElement('div');
                divobj.id=divid;
                var height;
                if(navigator.appName=="Microsoft Internet Explorer") {
                    height = ns.$("."+resultPanel).outerHeight();
                } else {
                    height = ns.$("."+resultPanel).height();
                }
                var width = ns.$("."+resultPanel).width();
	            divobj.innerHTML='<table border="0" style="width: 100%; height: 100%;"><tr><td width="40%" height="'+height+'px">&nbsp;</td><td align="center" height="100%" valign="middle"><div class="loaderimg"><span class="ui-accordion-header"></span></div></td><td width="40%" height="'+height+'px">&nbsp;</td></tr></table>';
                ns.$("."+resultPanel).prepend(divobj);
                ns.$('#'+divid).addClass("loadercontent");
                ns.$('#'+divid).width(width);
                ns.$('#'+divid).height(height);
                ns.$('#'+divid).css("z-index",1003);
            }
 }


 function destroyLoaderH5P(loaderTarget)
 {
              var divid= "loaderdiv"+loaderTarget;
              if(document.getElementById(divid)!=null)
                {
                        ns.$('#'+divid).remove();
                }
}
;
/** @namespace H5PEditor */
var H5PEditor = H5PEditor || {};

/**
 * The library list cache
 * 
 * @type Object
 */
var llc = H5PEditor.LibraryListCache = {
  libraryCache: {},
  librariesComingIn: {},
  librariesMissing: {},
  que: []
};

/**
 * Get data for a list of libraries
 * 
 * @param {Array} libraries - list of libraries to load info for (uber names)
 * @param {Function} handler - Callback when list of libraries is loaded
 * @param {Function} thisArg - Context for the callback function
 */
llc.getLibraries = function(libraries, handler, thisArg) {
  var cachedLibraries = [];
  var status = 'hasAll';
  for (var i = 0; i < libraries.length; i++) {
    if (libraries[i] in llc.libraryCache) {
      // Libraries that are missing on the server are set to null...
      if (llc.libraryCache[libraries[i]] !== null) {
        cachedLibraries.push(llc.libraryCache[libraries[i]]);
      }
    }
    else if (libraries[i] in llc.librariesComingIn) {
      if (status === 'hasAll') {
        status = 'onTheWay';
      }
    }
    else {
      status = 'requestThem';
      llc.librariesComingIn[libraries[i]] = true;
    }
  }
  switch (status) {
    case 'hasAll':
      handler.call(thisArg, cachedLibraries);
      break;
  case 'onTheWay':
    llc.que.push({libraries: libraries, handler: handler, thisArg: thisArg});
    break;
  case 'requestThem':
    //h5pcustomize - not required for summary performance improvement
    if(libraries[0] == "H5P.Summary 1.4")
    {
    }
    else if(JSON.stringify(libraries) == '["H5P.AdvancedText 1.1","H5P.Link 1.1","H5P.Image 1.0","H5P.Video 1.2","H5P.Audio 1.2","H5P.Blanks 1.4","H5P.SingleChoiceSet 1.3","H5P.DragText 1.4","H5P.MarkTheWords 1.5"]')
    {
    	data = [{"uberName":"H5P.AdvancedText 1.1","name":"H5P.AdvancedText","majorVersion":"1","minorVersion":"1","tutorialUrl":null,"title":"Advanced Text","runnable":"0","restricted":false},{"uberName":"H5P.Link 1.1","name":"H5P.Link","majorVersion":"1","minorVersion":"1","tutorialUrl":null,"title":"Link","runnable":"0","restricted":false},{"uberName":"H5P.Image 1.0","name":"H5P.Image","majorVersion":"1","minorVersion":"0","tutorialUrl":null,"title":"Image","runnable":"0","restricted":false},{"uberName":"H5P.Video 1.2","name":"H5P.Video","majorVersion":"1","minorVersion":"2","tutorialUrl":null,"title":"Video","runnable":"0","restricted":false},{"uberName":"H5P.Audio 1.2","name":"H5P.Audio","majorVersion":"1","minorVersion":"2","tutorialUrl":null,"title":"Audio","runnable":"1","restricted":false},{"uberName":"H5P.Blanks 1.4","name":"H5P.Blanks","majorVersion":"1","minorVersion":"4","tutorialUrl":null,"title":"Fill in the Blanks","runnable":"1","restricted":false},{"uberName":"H5P.SingleChoiceSet 1.3","name":"H5P.SingleChoiceSet","majorVersion":"1","minorVersion":"3","tutorialUrl":null,"title":"Single Choice Set","runnable":"1","restricted":false},{"uberName":"H5P.DragText 1.4","name":"H5P.DragText","majorVersion":"1","minorVersion":"4","tutorialUrl":null,"title":"Drag Text","runnable":"1","restricted":false},{"uberName":"H5P.MarkTheWords 1.5","name":"H5P.MarkTheWords","majorVersion":"1","minorVersion":"5","tutorialUrl":null,"title":"Mark the Words","runnable":"1","restricted":false}];
		llc.setLibraries(data, libraries);
        handler.call(thisArg, data);
        llc.runQue();
    }
	else if(JSON.stringify(libraries) == '["H5P.Nil 1.0","H5P.Text 1.1","H5P.Link 1.1","H5P.Image 1.0","H5P.SingleChoiceSet 1.3","H5P.Blanks 1.4","H5P.MarkTheWords 1.5","H5P.DragText 1.4","H5P.GoToQuestion 1.0"]')
	{
		data = [{"uberName":"H5P.Nil 1.0","name":"H5P.Nil","majorVersion":"1","minorVersion":"0","tutorialUrl":null,"title":"Nil","runnable":"0","restricted":false},{"uberName":"H5P.Text 1.1","name":"H5P.Text","majorVersion":"1","minorVersion":"1","tutorialUrl":null,"title":"Text","runnable":"0","restricted":false},{"uberName":"H5P.Link 1.1","name":"H5P.Link","majorVersion":"1","minorVersion":"1","tutorialUrl":null,"title":"Link","runnable":"0","restricted":false},{"uberName":"H5P.Image 1.0","name":"H5P.Image","majorVersion":"1","minorVersion":"0","tutorialUrl":null,"title":"Image","runnable":"0","restricted":false},{"uberName":"H5P.SingleChoiceSet 1.3","name":"H5P.SingleChoiceSet","majorVersion":"1","minorVersion":"3","tutorialUrl":"https:\/\/h5p.org\/documentation\/content-author-guide\/tutorials-for-authors\/single-choice-set","title":"Single Choice Set","runnable":"1","restricted":false},{"uberName":"H5P.Blanks 1.4","name":"H5P.Blanks","majorVersion":"1","minorVersion":"4","tutorialUrl":"https:\/\/h5p.org\/tutorial-fill-in-the-blanks","title":"Fill in the Blanks","runnable":"1","restricted":false},{"uberName":"H5P.MarkTheWords 1.5","name":"H5P.MarkTheWords","majorVersion":"1","minorVersion":"5","tutorialUrl":"https:\/\/h5p.org\/documentation\/content-author-guide\/tutorials-for-authors\/mark-the-words","title":"Mark the Words","runnable":"1","restricted":false},{"uberName":"H5P.DragText 1.4","name":"H5P.DragText","majorVersion":"1","minorVersion":"4","tutorialUrl":"https:\/\/h5p.org\/documentation\/content-author-guide\/tutorials-for-authors\/drag-the-words","title":"Drag Text","runnable":"1","restricted":false},{"uberName":"H5P.GoToQuestion 1.0","name":"H5P.GoToQuestion","majorVersion":"1","minorVersion":"0","tutorialUrl":null,"title":"Go To Question","runnable":"0","restricted":false}];
		llc.setLibraries(data, libraries);
        handler.call(thisArg, data);
        llc.runQue();
	}
	else 
	{
	  	console.log("from1 h5peditor-library-list-cache.js libraries:"+JSON.stringify(libraries));
		console.log("form111111 list-cache.js:"+JSON.stringify(libraries));
	
    var ajaxParams = {
      type: "POST",
      url: H5PEditor.getAjaxUrl('libraries'),
      success: function(data) { 
        console.log("from1 h5peditor-library-list-cache.js: "+JSON.stringify(data));
        llc.setLibraries(data, libraries);
        handler.call(thisArg, data);
        llc.runQue();
      },
      data: {'libraries': libraries},
      dataType: "json"
    };
    H5PEditor.$.ajax(ajaxParams);
    }
    break;
  }
};

/**
 * Call all qued handlers
 */
llc.runQue = function() {
  var l = llc.que.length;
  for (var i = 0; i < l; i++) {
    var handlerObject = llc.que.shift();
    llc.getLibraries(handlerObject.libraries, handlerObject.handler, handlerObject.thisArg);
  }
};

/**
 * We've got new libraries from the server, save them
 * 
 * @param {Array} libraries - Libraries with info from server
 * @param {Array} requestedLibraries - List of what libraries we requested
 */
llc.setLibraries = function(libraries, requestedLibraries) {
  var reqLibraries = requestedLibraries.slice();
  for (var i = 0; i < libraries.length; i++) {
    llc.libraryCache[libraries[i].uberName] = libraries[i];
    if (libraries[i].uberName in llc.librariesComingIn) {
      delete llc.librariesComingIn[libraries[i].uberName];
    }
    var index = reqLibraries.indexOf(libraries[i].uberName);
    if (index > -1) {
      reqLibraries.splice(index, 1);
    }
  }
  for (var i = 0; i < reqLibraries.length; i++) {
    llc.libraryCache[reqLibraries[i]] = null;
    if (reqLibraries[i] in llc.librariesComingIn) {
      delete llc.librariesComingIn[libraries[i]];
    }
  }
};
;
var H5PEditor = H5PEditor || {};

H5PEditor.widgets.select = H5PEditor.Select = (function (E) {
  /**
   * Initialize a new widget.
   *
   * @param {object} parent
   * @param {object} field
   * @param {object} params
   * @param {function} setValue
   * @returns {_L3.C}
   */
  function C(parent, field, params, setValue) {
    this.field = field;
    this.value = params;
    this.setValue = setValue;
  }

  /**
   * Append widget to the DOM.
   *
   * @param {jQuery} $wrapper
   * @returns {undefined}
   */
  C.prototype.appendTo = function ($wrapper) {
    var that = this;

    this.$item = E.$(this.createHtml()).appendTo($wrapper);
    this.$select = this.$item.find('select');
    this.$errors = this.$item.children('.h5p-errors');

    this.$select.change(function () {
      var val = that.validate();
      if (val !== false) {
        that.value = val;
        that.setValue(that.field, val);
      }
    });
  };

  /**
   * Generate HTML for the widget.
   *
   * @returns {String} HTML.
   */
  C.prototype.createHtml = function () {
    var options = E.createOption('-', '-');
    for (var i = 0; i < this.field.options.length; i++) {
      var option = this.field.options[i];
      options += E.createOption(option.value, option.label, option.value === this.value);
    }

    var label = E.createLabel(this.field, '<select>' + options + '</select>');

    return E.createItem(this.field.type, label, this.field.description);
  };


  /**
   * Validate this field.
   *
   * @returns {Boolean}
   */
  C.prototype.validate = function () {
    var value = this.$select.val();
    if (value === '-') {
      value = undefined; // No value selected
    }

    if (this.field.optional !== true && value === undefined) {
      // Not optional and no value selected, print required error
      this.$errors.append(ns.createError(ns.t('core', 'requiredProperty', {':property': 'text field'})));

      return false;
    }

    // All valid. Remove old errors
    var $errors = this.$errors.children();
    if ($errors.length) {
      $errors.remove();
    }

    return value;
  };


  /**
   * Remove widget from DOM.
   *
   * @returns {undefined}
   */
  C.prototype.remove = function () {
    this.$item.remove();
  };

  return C;
})(H5PEditor);
;
var H5PEditor = H5PEditor || {};
var ns = H5PEditor;

/**
 * Adds a dimensions field to the form.
 *
 * TODO: Make it possible to lock width/height ratio.
 *
 * @param {mixed} parent
 * @param {object} field
 * @param {mixed} params
 * @param {function} setValue
 * @returns {ns.Dimensions}
 */
ns.Dimensions = function (parent, field, params, setValue) {
  var that = this;

  this.parent = parent;
  this.field = field;
  this.changes = [];

  // Find image field to get max size from.
  H5PEditor.followField(parent, field.max, function (file) {
    that.setMax(file);
  });

  // Find image field to get default size from.
  H5PEditor.followField(parent, field['default'], function (file, index) {
    // Make sure we don't set size if we have one in the default params.
    if (params.width === undefined) {
      that.setSize(file);
    }
  });

  this.params = params;
  this.setValue = setValue;

  // Remove default field from params to avoid saving it.
  if (this.params.field) {
    this.params.field = undefined;
  }
};

/**
 * Set max dimensions.
 *
 * @param {Object} file
 * @returns {unresolved}
 */
ns.Dimensions.prototype.setMax = function (file) {
  if (file === undefined) {
    return;
  }

  this.max = {
    width: parseInt(file.width),
    height: parseInt(file.height)
  };
};

/**
 * Set current dimensions.
 *
 * @param {string} width
 * @param {string} height
 * @returns {undefined}
 */
ns.Dimensions.prototype.setSize = function (file) {
  if (file === undefined) {
    return;
  }

  this.params = {
    width: parseInt(file.width),
    height: parseInt(file.height)
  };
  this.setValue(this.field, this.params);

  this.$inputs.filter(':eq(0)').val(file.width).next().val(file.height);

  for (var i = 0; i < this.changes.length; i++) {
    this.changes[i](file.width, file.height);
  }
};

/**
 * Append the field to the given wrapper.
 *
 * @param {jQuery} $wrapper
 * @returns {undefined}
 */
ns.Dimensions.prototype.appendTo = function ($wrapper) {
  var that = this;

  this.$item = ns.$(this.createHtml()).appendTo($wrapper);
  this.$inputs = this.$item.find('input');
  this.$errors = this.$item.children('.h5p-errors');

  this.$inputs.change(function () {
    // Validate
    var value = that.validate();

    if (value) {
      // Set param
      that.params = value;
      that.setValue(that.field, value);

      for (var i = 0; i < that.changes.length; i++) {
        that.changes[i](value.width, value.height);
      }
    }
  }).click(function () {
    return false;
  });
};

/**
 * Create HTML for the field.
 */
ns.Dimensions.prototype.createHtml = function () {
  var input = ns.createText(this.params !== undefined ? this.params.width : undefined, 15, 'Width') + ' x ' + ns.createText(this.params !== undefined ? this.params.height : undefined, 15, 'Height');
  var label = ns.createLabel(this.field, input);

  return ns.createItem(this.field.widget, label, this.field.description, this.field.description);
};

/**
 * Validate the current text field.
 */
ns.Dimensions.prototype.validate = function () {
  var that = this;
  var size = {};

  this.$errors.html('');

  this.$inputs.each(function (i) {
    var $input = ns.$(this);
    var value = H5P.trim($input.val());
    var property = i ? 'height' : 'width';

    if ((that.field.optional === undefined || !that.field.optional) && !value.length) {
      that.$errors.append(ns.createError(ns.t('core', 'requiredProperty', {':property': property})));
      return false;
    }
    else if (!value.match(new RegExp('^[0-9]+$'))) {
      that.$errors.append(ns.createError(ns.t('core', 'onlyNumbers', {':property': property})));
      return false;
    }

    value = parseInt(value);
    if (that.max !== undefined && value > that.max[property]) {
      that.$errors.append(ns.createError(ns.t('core', 'exceedsMax', {':property': property, ':max': that.max[property]})));
      return false;
    }

    size[property] = value;
  });

  return ns.checkErrors(this.$errors, this.$inputs, size);
};

/**
 * Remove this item.
 */
ns.Dimensions.prototype.remove = function () {
  this.$item.remove();
};

// Tell the editor what widget we are.
ns.widgets.dimensions = ns.Dimensions;;
var H5PEditor = H5PEditor || {};
var ns = H5PEditor;

/**
 * Creates a coordinates picker for the form.
 *
 * @param {mixed} parent
 * @param {object} field
 * @param {mixed} params
 * @param {function} setValue
 * @returns {ns.Coordinates}
 */
ns.Coordinates = function (parent, field, params, setValue) {
  var that = this;

  this.parent = parent;
  this.field = H5P.cloneObject(field, true); // TODO: Cloning is a quick fix, make sure this field doesn't change semantics!

  // Find image field to get max size from.
  // TODO: Use followField?
  this.findImageField('max', function (field) {
    if (field instanceof ns.File) {
      if (field.params !== undefined) {
        that.setMax(field.params.width, field.params.height);
      }

      field.changes.push(function (file) {
        if (file === undefined) {
          return;
        }
        // TODO: This callback should be removed when this item is removed.
        that.setMax(file.params.width, file.params.height);
      });
    }
    else if (field instanceof ns.Dimensions) {
      if (field.params !== undefined) {
        that.setMax(field.params.width, field.params.height);
      }

      field.changes.push(function (width, height) {
        // TODO: This callback should be removed when this item is removed.
        that.setMax(width, height);
      });
    }
  });

  this.params = params;
  this.setValue = setValue;
};

/**
 * Set max coordinates.
 *
 * @param {string} x
 * @param {string} y
 * @returns {undefined}
 */
ns.Coordinates.prototype.setMax = function (x, y) {
  this.field.max = {
    x: parseInt(x),
    y: parseInt(y)
  };
  if (this.params !== undefined) {
    this.$errors.html('');
    this.validate();
  }
};

/**
 * Find the image field for the given property and then run the callback.
 *
 * @param {string} property
 * @param {function} callback
 * @returns {unresolved}
 */
ns.Coordinates.prototype.findImageField = function (property, callback) {
  var that = this;
  var str = 'string';

  if (typeof this.field[property] !== str) {
    return;
  }

  // Find field when tree is ready.
  this.parent.ready(function () {
    if (typeof that.field[property] !== str) {
      if (that.field[property] !== undefined) {
        callback(that.field[property]);
      }
      return; // We've already found this field before.
    }
    var path = that.field[property];

    that.field[property] = ns.findField(that.field[property], that.parent);
    if (!that.field[property]) {
      throw ns.t('core', 'unknownFieldPath', {':path': path});
    }
    if (that.field[property].field.type !== 'image' && that.field[property].field.widget !== 'dimensions') {
      throw ns.t('core', 'notImageOrDimensionsField', {':path': path});
    }

    callback(that.field[property]);
  });
};

/**
 * Append the field to the wrapper.
 *
 * @param {jQuery} $wrapper
 * @returns {undefined}
 */
ns.Coordinates.prototype.appendTo = function ($wrapper) {
  var that = this;

  this.$item = ns.$(this.createHtml()).appendTo($wrapper);
  this.$inputs = this.$item.find('input');
  this.$errors = this.$item.children('.h5p-errors');

  this.$inputs.change(function () {
    // Validate
    var value = that.validate();

    if (value) {
      // Set param
      that.params = value;
      that.setValue(that.field, value);
    }
  }).click(function () {
    return false;
  }).click(function () {
    return false;
  });
};

/**
 * Create HTML for the coordinates picker.
 */
ns.Coordinates.prototype.createHtml = function () {
  var input = ns.createText(this.params !== undefined ? this.params.x : undefined, 15, 'X') + ' , ' + ns.createText(this.params !== undefined ? this.params.y : undefined, 15, 'Y');
  var label = ns.createLabel(this.field, input);

  return ns.createItem(this.field.widget, label, this.field.description);
};

/**
 * Validate the current values.
 */
ns.Coordinates.prototype.validate = function () {
  var that = this;
  var coordinates = {};

  this.$inputs.each(function (i) {
    var $input = ns.$(this);
    var value = H5P.trim($input.val());
    var property = i ? 'y' : 'x';

    if (that.field.optional && !value.length) {
      return true;
    }

    if ((that.field.optional === undefined || !that.field.optional) && !value.length) {
      that.$errors.append(ns.createError(ns.t('core', 'requiredProperty', {':property': property})));
      return false;
    }

    if (value.length && !value.match(new RegExp('^[0-9]+$'))) {
      that.$errors.append(ns.createError(ns.t('core', 'onlyNumbers', {':property': property})));
      return false;
    }

    value = parseInt(value);
    if (that.field.max !== undefined && value > that.field.max[property]) {
      that.$errors.append(ns.createError(ns.t('core', 'exceedsMax', {':property': property, ':max': that.field.max[property]})));
      return false;
    }

    coordinates[property] = value;
  });

  return ns.checkErrors(this.$errors, this.$inputs, coordinates);
};

/**
 * Remove this item.
 */
ns.Coordinates.prototype.remove = function () {
  this.$item.remove();
};

// Tell the editor what widget we are.
ns.widgets.coordinates = ns.Coordinates;;
var H5PEditor = H5PEditor || {};
var ns = H5PEditor;

/**
 * Create a field without html
 * 
 * @param {mixed} parent
 * @param {object} field
 * @param {mixed} params
 * @param {function} setValue
 */
ns.None = function (parent, field, params, setValue) {
  this.parent = parent;
  this.field = field;
  this.params = params;
  this.setValue = setValue;
};

/**
 * Implementation of appendTo
 * 
 * None doesn't append anything
 * 
 * @param {jQuery} $wrapper
 */
ns.None.prototype.appendTo = function ($wrapper) {};

/**
 * Implementation of validate
 * 
 * None allways validates
 */
ns.None.prototype.validate = function () {
  return true;
};

/**
 * Collect functions to execute once the tree is complete.
 * 
 * @param {function} ready
 */
ns.None.prototype.ready = function (ready) {
  this.parent.ready(ready);
};

/**
 * Remove this item.
 */
ns.None.prototype.remove = function () {
  ns.removeChildren(this.children);
};

// Tell the editor what widget we are.
ns.widgets.none = ns.None;