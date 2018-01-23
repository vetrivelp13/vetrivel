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
         
          //h5pcustomization : no need of h5p language library.
          libraryData.language = null;
          
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


