/*
 * jQuery Autocomplete plugin 1.1
 *
 * Copyright (c) 2009 J�rn Zaefferer
 *
 * Dual licensed under the MIT and GPL licenses:
 *   http://www.opensource.org/licenses/mit-license.php
 *   http://www.gnu.org/licenses/gpl.html
 *
 * Revision: $Id: jquery.autocomplete.js,v 1.1 2011-01-03 13:10:27 srprabhu Exp $
 */

;(function($) {
	
$.fn.extend({
	autocomplete: function(urlOrData, options) {
		var isUrl = typeof urlOrData == "string" || typeof urlOrData== "function";;
		options = $.extend({}, $.Autocompleter.defaults, {
			url: isUrl ? urlOrData : null,
			data: isUrl ? null : urlOrData,
			delay: isUrl ? $.Autocompleter.defaults.delay : 10,
			max: options && !options.scroll ? 10 : 150
		}, options);
				
		// if highlight is set to false, replace it with a do-nothing function
		options.highlight = options.highlight || function(value) { return value; };
		
		// if the formatMatch option is not specified, then use formatItem for backwards compatibility
		options.formatMatch = options.formatMatch || options.formatItem;
		
		return this.each(function() {
			new $.Autocompleter(this, options);
		});
	},
	result: function(handler) {
		return this.bind("result", handler);
	},
	search: function(handler) {
		return this.trigger("search", [handler]);
	},
	flushCache: function() {
		return this.trigger("flushCache");
	},
	setOptions: function(options){
		return this.trigger("setOptions", [options]);
	},
	unautocomplete: function() {
		return this.trigger("unautocomplete");
	}
});

$.Autocompleter = function(input, options) {

	var KEY = {
		UP: 38,
		DOWN: 40,
		DEL: 46,
		TAB: 9,
		RETURN: 13,
		ESC: 27,
		COMMA: 188,
		PAGEUP: 33,
		PAGEDOWN: 34,
		BACKSPACE: 8
	};

	// Create $ object for input element
	var $input = $(input).attr("autocomplete", "off").addClass(options.inputClass);

	var timeout;
	var previousValue = "";
	var cache = $.Autocompleter.Cache(options);
	var hasFocus = 0;
	var lastKeyPressCode;
	var config = {
		mouseDownOnSelect: false
	};
	var select = $.Autocompleter.Select(options, input, selectCurrent, config);
	
	var blockSubmit;
	
	// prevent form submit in opera when selecting with return key
	$.browser.opera && $(input.form).bind("submit.autocomplete", function() {
		if (blockSubmit) {
			blockSubmit = false;
			return false;
		}
	});
	
	// only opera doesn't trigger keydown multiple times while pressed, others don't work with keypress at all
	$input.bind(($.browser.opera ? "keypress" : "keydown") + ".autocomplete", function(event) {
		// a keypress means the input has focus
		// avoids issue where input had focus before the autocomplete was applied
		hasFocus = 1;
		// track last key pressed
		lastKeyPressCode = event.keyCode;
		switch(event.keyCode) {
		
			case KEY.UP:
				event.preventDefault();
				if ( select.visible() ) {
					select.prev();
				} else {
					onChange(0, true);
				}
				break;
				
			case KEY.DOWN:
				event.preventDefault();
				if ( select.visible() ) {
					select.next();
				} else {
					onChange(0, true);
				}
				break;
				
			case KEY.PAGEUP:
				event.preventDefault();
				if ( select.visible() ) {
					select.pageUp();
				} else {
					onChange(0, true);
				}
				break;
				
			case KEY.PAGEDOWN:
				event.preventDefault();
				if ( select.visible() ) {
					select.pageDown();
				} else {
					onChange(0, true);
				}
				break;
			
			// matches also semicolon
			case options.multiple && $.trim(options.multipleSeparator) == "," && KEY.COMMA:
			case KEY.TAB:
			case KEY.RETURN:
				if( selectCurrent() ) {
					// stop default to prevent a form submit, Opera needs special handling
					event.preventDefault();
					blockSubmit = true;
					return false;
				}
				break;
				
			case KEY.ESC:
				select.hide();
				break;
				
			default:
				clearTimeout(timeout);
				timeout = setTimeout(onChange, options.delay);
				break;
		}
	}).focus(function(){
		// track whether the field has focus, we shouldn't process any
		// results if the field no longer has focus
		hasFocus++;
	}).blur(function() {
		hasFocus = 0;
		if (!config.mouseDownOnSelect) {
			hideResults();
		}
	}).click(function() {
		// show select when clicking in a focused field
		if ( hasFocus++ > 1 && !select.visible() ) {
			onChange(0, true);
		}
	}).bind("search", function() {
		// TODO why not just specifying both arguments?
		var fn = (arguments.length > 1) ? arguments[1] : null;

		function findValueCallback(z, data) {
			var result;
			if( data && data.length ) {
				for (var i=0; i < data.length; i++) {
					if( data[i].result.toLowerCase() == z.toLowerCase() ) {
						result = data[i];
						break;
					}
				}
			}
			if( typeof fn == "function" ) fn(result);
			else $input.trigger("result", result && [result.data, result.value]);
		}
		$.each(trimWords($input.val()), function(i, value) {
			request(value, findValueCallback, findValueCallback);
		});
	}).bind("flushCache", function() {
		cache.flush();
	}).bind("setOptions", function() {
		$.extend(options, arguments[1]);
		// if we've updated the data, repopulate
		if ( "data" in arguments[1] )
			cache.populate();
	}).bind("unautocomplete", function() {
		select.unbind();
		$input.unbind();
		$(input.form).unbind(".autocomplete");
	});
	
	
	function selectCurrent() {
		var selected = select.selected();
		if( !selected )
			return false;
		
		var v = selected.result;
		
		/* Coustomized by Vincent for support id & name : starts */
		var sd_encode = selected.data;
		var sd = rawurldecode(sd_encode);
		var sd1 = String(sd).split(" - <b>");
		sd = sd1[0];
		if(options.extraParams.id){
			var nd = String(sd).split(",");
      if (!options.hasOwnProperty('formatResult')) { // If formatResult() is defined, use v (selected.result) already set at the top, which is
                                                     // output of formatResult(). This check is added on 13 Sep 2012 by Sunil
  			if (nd.length > 1) {
  				v='';
  				for(var ki=1;ki<nd.length;ki++){
  					v+=v==''?nd[ki]:','+nd[ki];
  					//console.log('v+:'+v);
  				}
  			}else{
  				v=nd[0];
  			}
      }
  	  //console.log('Updating ' + options.extraParams.id + ' from value = ' + $('#'+options.extraParams.id).val() + ' to value = ' + nd[0]);
      
  		// While we select location get the id paint location capacity value  
  		if(options.capacityParams && options.extraParams.capacityId) {
  			var cp = String(nd[0]).split("#");
  			$('#'+options.extraParams.id).val(cp[0]);
  			$(options.extraParams.capacityId).val(cp[1]);
  		}else{
  			if(options.multiple == true){
  				var nval = $('#'+options.extraParams.id).val();
  				nval = (nval=='')?nd[0]:nval+"|"+nd[0];
  				$('#'+options.extraParams.id).val(nval);
  			}else{
  				$('#'+options.extraParams.id).val(nd[0]);
  			}
  		   
  		}
  	} else if (!options.hasOwnProperty('formatResult')) { // If formatResult() is defined, use v  (selected.result) already set at the top, which is
                                                         // output of formatResult(), otherwise this code. This check is added on 13 Sep 2012 by Sunil
  		v=String(sd);
  	}
    //console.log('selectCurrent() : v = ' + v);
		
		/*if(options.extraParams.callback){
			eval(options.extraParams.callback+'("'+String(sd)+'","'+options.extraParams.id+'")');
		}*/
		
		/* Coustomized by Vincent for support id & name : ends */
		
		previousValue = v;
		
		if ( options.multiple ) {
			var words = trimWords($input.val());
			if ( words.length > 1 ) {
				var seperator = options.multipleSeparator.length;
				var cursorAt = $(input).selection().start;
				var wordAt, progress = 0;
				$.each(words, function(i, word) {
					progress += word.length;
					if (cursorAt <= progress) {
						wordAt = i;
						return false;
					}
					progress += seperator;
				});
				words[wordAt] = v;
				// TODO this should set the cursor to the right position, but it gets overriden somewhere
				//$.Autocompleter.Selection(input, progress + seperator, progress + seperator);
				v = words.join( options.multipleSeparator );
			}
			v += options.multipleSeparator;
		}
		//console.log('v value'+v);
		
		var maxChars = $input.attr('maxlength');
		if(maxChars != -1 && maxChars != '' && maxChars !== undefined) {
			$input.val(v.substring(0, maxChars));
		}
		else {
			$input.val(v);
		}
		
		if(options.extraParams.callback){
			eval(options.extraParams.callback+'("'+String(sd)+'","'+options.extraParams.id+'")');
			
		}
		hideResultsNow();
		$input.trigger("result", [selected.data, selected.value]);
		
		return true;
	}
	
	function split(sep){
		alert(sep);
	}
	
	function onChange(crap, skipPrevCheck) {
		if( lastKeyPressCode == KEY.DEL ) {
			select.hide();
			return;
		}
		
		var currentValue = $input.val();
		
		if ( !skipPrevCheck && currentValue == previousValue )
			return;
		
		previousValue = currentValue;
		
		currentValue = lastWord(currentValue);
		if ( currentValue.length >= options.minChars) {
			$input.addClass(options.loadingClass);
			if (!options.matchCase)
				currentValue = currentValue.toLowerCase();
			request(currentValue, receiveData, hideResultsNow);
		} else {
			stopLoading();
			select.hide();
		}
	};
	
	function trimWords(value) {
		if (!value)
			return [""];
		if (!options.multiple)
			return [$.trim(value)];
		return $.map(value.split(options.multipleSeparator), function(word) {
			return $.trim(value).length ? $.trim(word) : null;
		});
	}
	
	function lastWord(value) {
		if ( !options.multiple )
			return value;
		var words = trimWords(value);
		if (words.length == 1) 
			return words[0];
		var cursorAt = $(input).selection().start;
		if (cursorAt == value.length) {
			words = trimWords(value)
		} else {
			words = trimWords(value.replace(value.substring(cursorAt), ""));
		}
		return words[words.length - 1];
	}
	
	// fills in the input box w/the first match (assumed to be the best match)
	// z: the term entered
	// sValue: the first matching result
	function autoFill(z, sValue){
		/* autoFill has been suppressed by Vincent on 22-Jun-2010 */
		
		// autofill in the complete box w/the first match as long as the user hasn't entered in more data
		// if the last user key pressed was backspace, don't autofill
		//if( options.autoFill && (lastWord($input.val()).toLowerCase() == z.toLowerCase()) && lastKeyPressCode != KEY.BACKSPACE ) {
			// fill in the value (keep the case the user has typed)
			//$input.val($input.val() + sValue.substring(lastWord(previousValue).length));
			// select the portion of the value not typed by the user (so the next character will erase)
			//$(input).selection(previousValue.length, previousValue.length + sValue.length);
		//}
	};

	function hideResults() {
		clearTimeout(timeout);
		timeout = setTimeout(hideResultsNow, 200);
	};

	function hideResultsNow() {
		var wasVisible = select.visible();
		select.hide();
		clearTimeout(timeout);
		stopLoading();
		if (options.mustMatch) {
			// call search and run callback
			$input.search(
				function (result){
					// if no value found, clear the input box
					if( !result ) {
						if (options.multiple) {
							var words = trimWords($input.val()).slice(0, -1);
							
							$input.val( words.join(options.multipleSeparator) + (words.length ? options.multipleSeparator : "") );
						}
						else {
							$input.val( "" );
							$input.trigger("result", null);
						}
					}
				}
			);
		}
	};

	function receiveData(z, data) {
		if ( data && data.length && hasFocus ) {
			stopLoading();
			select.display(data, z);
			autoFill(z, data[0].value);
			select.show();
			hide_findTraining(); /* IE 6 issue */
						
		} else {
			hideResultsNow();
		}
	};

	function request(term, success, failure) {
		if (!options.matchCase)
			term = term.toLowerCase();
		var data = cache.load(term);
		// recieve the cached data
		if (data && data.length) {
			success(term, data);
		// if an AJAX url has been supplied, try loading the data now
		} else if( (typeof options.url == "string") && (options.url.length > 0) ){
			
			var extraParams = {
				timestamp: +new Date()
			};
			$.each(options.extraParams, function(key, param) {
				extraParams[key] = typeof param == "function" ? param() : param;
			});
			autoCompleteTimeOut = parseInt(options.autoCompleteTimeOut);
			if(autoCompleteTimeOut>0){
				$.ajax({
					// try to leverage ajaxQueue plugin to abort previous requests
					mode: "abort",
					// limit abortion to this input
					port: "autocomplete" + input.name,
					dataType: options.dataType,
					url: options.url,
					timeout:autoCompleteTimeOut,
					data: $.extend({
						z: lastWord(term),
						limit: options.max
					}, extraParams),
					success: function(data) {
						var parsed = options.parse && options.parse(data) || parse(data);
						cache.add(term, parsed);
						success(term, parsed);
					},error: function(xhr, status, message) {
				        if(status == "timeout") {
				        	stopLoading();
				        	var error = new Array();
							error[0] = options.errMsg;
				        	$('#show_expertus_message').html(expertus_error_message(error,'error'));
							$('#show_expertus_message').show();
				       }
				   }
				});
			}else{
				$.ajax({
					// try to leverage ajaxQueue plugin to abort previous requests
					mode: "abort",
					// limit abortion to this input
					port: "autocomplete" + input.name,
					dataType: options.dataType,
					url: options.url,
					data: $.extend({
						z: lastWord(term),
						limit: options.max
					}, extraParams),
					success: function(data) {
						var parsed = options.parse && options.parse(data) || parse(data);
						cache.add(term, parsed);
						success(term, parsed);
					}
				});
			}
		} 
		else if(typeof options.url == "function")
		{
			options.url(lastWord(term),functionAdder,success);
			//temp=parseFunction(temp);
			//cache.add(term, temp);
			//success(term, temp);
		}
		else {
			// if we have a failure, we need to empty the list -- this prevents the the [TAB] key from selecting the last successful match
			select.emptyList();
			failure(term);
		}
	};
	
	function functionAdder(success,term,temp)
	{
		temp=parseFunction(temp);
		cache.add(term, temp);
		success(term, temp);
	}
	
	function parseFunction(data) {
	var parsed = [];
	var rows = data.split("}");
	for (var i=0; i < rows.length; i++) {
		var row = $.trim(rows[i]);
		if (row) {
			row = row.split("|");
			parsed[parsed.length] = {
				data: row,
				value: row[0],
				result: options.formatResult && options.formatResult(row, row[0]) || row[0]
			};
		}
	}
	return parsed;
	};
	
	function parse(data) {
		var parsed = [];
		var rows = data.split("\n");
		for (var i=0; i < rows.length; i++) {
			var row = $.trim(rows[i]);
			if (row) {
				row = row.split(Drupal.settings.custom.EXP_AC_SEPARATOR);
				parsed[parsed.length] = {
					data: row,
					value: row[0],
					result: options.formatResult && options.formatResult(row, row[0]) || row[0]
				};
			}
		}
		return parsed;
	};

	function stopLoading() {
		$input.removeClass(options.loadingClass);
	};

};

$.Autocompleter.defaults = {
	inputClass: "ac_input",
	resultsClass: "ac_results",
	loadingClass: "ac_loading",
	minChars: 1,
	delay: 400,
	matchCase: false,
	matchSubset: true,
	matchContains: false,
	cacheLength: 10,
	max: 100,
	mustMatch: false,
	extraParams: {},
	selectFirst: false,
	formatItem: function(row) { return row[0]; },
	formatMatch: null,
	autoFill: false,
	width: 0,
	multiple: false,
	multipleSeparator: ", ",
	highlight: function(value, term) {
		return value.replace(new RegExp("(?![^&;]+;)(?!<[^<>]*)(" + term.replace(/([\^\$\(\)\[\]\{\}\*\.\+\?\|\\])/gi, "\\$1") + ")(?![^<>]*>)(?![^&;]+;)", "gi"), "<strong>$1</strong>");
	},
    scroll: true,
    scrollHeight: 200,
    autoCompleteTimeOut:0,
    errMsg:''
    /*scrollHeight: 180*/
};

$.Autocompleter.Cache = function(options) {

	var data = {};
	var length = 0;
	
	function matchSubset(s, sub) {
		if (!options.matchCase) 
			s = s.toLowerCase();
		var i = s.indexOf(sub);
		if (options.matchContains == "word"){
			i = s.toLowerCase().search("\\b" + sub.toLowerCase());
		}
		if (i == -1) return false;
		return i >= 0 || options.matchContains;
	};
	
	function add(z, value) {
		if (length > options.cacheLength){
			flush();
		}
		if (!data[z]){ 
			length++;
		}
		data[z] = value;
	}
	
	function populate(){
		if( !options.data ) return false;
		// track the matches
		var stMatchSets = {},
			nullData = 0;

		// no url was specified, we need to adjust the cache length to make sure it fits the local data store
		if( !options.url ) options.cacheLength = 1;
		
		// track all options for minChars = 0
		stMatchSets[""] = [];
		
		// loop through the array and create a lookup structure
		for ( var i = 0, ol = options.data.length; i < ol; i++ ) {
			var rawValue = options.data[i];
			// if rawValue is a string, make an array otherwise just reference the array
			rawValue = (typeof rawValue == "string") ? [rawValue] : rawValue;

			var value = options.formatMatch(rawValue, i+1, options.data.length);
			if ( value === false )
				continue;
				
			var firstChar = value.charAt(0).toLowerCase();
			// if no lookup array for this character exists, look it up now
			if( !stMatchSets[firstChar] ) 
				stMatchSets[firstChar] = [];

			// if the match is a string
			var row = {
				value: value,
				data: rawValue,
				result: options.formatResult && options.formatResult(rawValue) || value
			};
			// push the current match into the set list
			stMatchSets[firstChar].push(row);

			// keep track of minChars zero items
			if ( nullData++ < options.max ) {
				stMatchSets[""].push(row);
			}
		};

		// add the data items to the cache
		$.each(stMatchSets, function(i, value) {
			// increase the cache size
			options.cacheLength++;
			// add to the cache
			add(i, value);
		});
	}
	
	// populate any existing data
	setTimeout(populate, 25);
	
	function flush(){
		data = {};
		length = 0;
	}
	
	return {
		flush: flush,
		add: add,
		populate: populate,
		load: function(z) {
			if (!options.cacheLength || !length)
				return null;
			/* 
			 * if dealing w/local data and matchContains than we must make sure
			 * to loop through all the data collections looking for matches
			 */
			if( !options.url && options.matchContains ){
				// track all matches
				var csub = [];
				// loop through all the data grids for matches
				for( var k in data ){
					// don't search through the stMatchSets[""] (minChars: 0) cache
					// this prevents duplicates
					if( k.length > 0 ){
						var c = data[k];
						$.each(c, function(i, x) {
							// if we've got a match, add it to the array
							if (matchSubset(x.value, z)) {
								csub.push(x);
							}
						});
					}
				}
				
				return csub;
			} else 
			// if the exact item exists, use it
			if (data[z]){
				return data[z];
			} else
			if (options.matchSubset) {
				for (var i = z.length - 1; i >= options.minChars; i--) {
					var c = data[z.substr(0, i)];
					if (c) {
						var csub = [];
						$.each(c, function(i, x) {
							if (matchSubset(x.value, z)) {
								csub[csub.length] = x;								
							}
						});
						return csub;
					}
				}
			}
			return null;
		}
	};
};

$.Autocompleter.Select = function (options, input, select, config) {
	var CLASSES = {
		ACTIVE: "ac_over"
	};
	
	var listItems,
		active = -1,
		data,
		term = "",
		needsInit = true,
		element,
		list;
	
	// Create results
	function init() {
		if (!needsInit)
			return; 
		
		//Added by ganeshbabuv for Location UI filter #0073054 Search auto-suggestion UI needs to be improvised for Location filter
		var no_of_ac_results_cls = $('.ac_results').length;   

		element = $("<div/>")
		.hide()
		.addClass(options.resultsClass)
		.attr("id", 'ac_id_'+no_of_ac_results_cls) 
		.css("position", "absolute")						 
		.appendTo(document.body);  
		
		list = $("<ul/>").appendTo(element).mouseover( function(event)	{
			if(target(event).nodeName && target(event).nodeName.toUpperCase() == 'LI') {
	            active = $("li", list).removeClass(CLASSES.ACTIVE).index(target(event));
			    $(target(event)).addClass(CLASSES.ACTIVE);            
	        }
		}).click(function(event) {
			$(target(event)).addClass(CLASSES.ACTIVE);
			select();
			// TODO provide option to avoid setting focus again after selection? useful for cleanup-on-focus
			input.focus();
			return false;
		}).mousedown(function() {
			config.mouseDownOnSelect = true;
		}).mouseup(function() {
			config.mouseDownOnSelect = false;
		});
		
		if( options.width > 0 )
			element.css("width", options.width);
			
		needsInit = false;
	} 
	
	function target(event) {
		var element = event.target;
		while(element && element.tagName != "LI")
			element = element.parentNode;
		// more fun with IE, sometimes event.target is empty, just ignore it then
		if(!element)
			return [];
		return element;
	}

	function moveSelect(step) {
		listItems.slice(active, active + 1).removeClass(CLASSES.ACTIVE);
		movePosition(step);
        var activeItem = listItems.slice(active, active + 1).addClass(CLASSES.ACTIVE);
        if(options.scroll) {
            var offset = 0;
            listItems.slice(0, active).each(function() {
				offset += this.offsetHeight;
			});
            if((offset + activeItem[0].offsetHeight - list.scrollTop()) > list[0].clientHeight) {
                list.scrollTop(offset + activeItem[0].offsetHeight - list.innerHeight());
            } else if(offset < list.scrollTop()) {
                list.scrollTop(offset);
            }
        }
	};
	
	function movePosition(step) {
		active += step;
		if (active < 0) {
			active = listItems.size() - 1;
		} else if (active >= listItems.size()) {
			active = 0;
		}
	}
	
	function limitNumberOfItems(available) {
		return options.max && options.max < available
			? options.max
			: available;
	}
	
	function fillList() {
		list.empty();
		var max = limitNumberOfItems(data.length);
		for (var i=0; i < max; i++) {
			if (!data[i])				
				continue;			
			var formatted = rawurldecode(options.formatItem(data[i].data, i+1, max, data[i].value, term));
			//var formatted = options.formatItem(data[i].data, i+1, max, data[i].value, term);
			if ( formatted === false )
				continue;
			var li = $("<li/>").html( options.highlight(formatted, term) ).addClass(i%2 == 0 ? "ac_even" : "ac_odd").appendTo(list)[0];			
			$.data(li, "ac_data", data[i]);
		}
		listItems = list.find("li");
		if ( options.selectFirst ) {
			listItems.slice(0, 1).addClass(CLASSES.ACTIVE);
			active = 0;
		}
		// apply bgiframe if available
		if ( $.fn.bgiframe )
			list.bgiframe();
	}
	
	return {
		display: function(d, z) {
			init();
			data = d;			
			term = z;
			
			fillList();
		},
		next: function() {
			moveSelect(1);
		},
		prev: function() {
			moveSelect(-1);
		},
		pageUp: function() {
			if (active != 0 && active - 8 < 0) {
				moveSelect( -active );
			} else {
				moveSelect(-8);
			}
		},
		pageDown: function() {
			if (active != listItems.size() - 1 && active + 8 > listItems.size()) {
				moveSelect( listItems.size() - 1 - active );
			} else {
				moveSelect(8);
			}
		},
		hide: function() {
			element && element.hide();
			listItems && listItems.removeClass(CLASSES.ACTIVE);
			active = -1;
			show_findTraining();
		},
		visible : function() {
			return element && element.is(":visible");
		},
		current: function() {
			return this.visible() && (listItems.filter("." + CLASSES.ACTIVE)[0] || options.selectFirst && listItems[0]);
		},
		show: function() {
			var offset = $(input).offset(); 
			
			//Added by ganeshbabuv March 15th 2017 for Location UI filter #0073054 Search auto-suggestion UI needs to be improvised for Location filter
			var loc_filter_ac='0'; 
			var top_search_cls='ac_jscroll_results_left_panel_li_div';
			
			
			//Search Filter for Admin Calender - Added/Changed by ganesh on May 9th 2017 for #73716
			var elem_admin_cal_txt_id=$(input).attr('id');  
			if(elem_admin_cal_txt_id=='narrow-search-calendar-filter'){
		      options.width='150px';
			  offset.left=offset.left-14;
			  loc_filter_ac='1';
			}
			
			
			//Location Filter UI for Learner Catalog		   
		   if($(input).hasClass('loc_catalogpage_ac')){ 
			    options.width='140px'; 
			    offset.left=offset.left-12;
			    loc_filter_ac='1'; 
		    }
			
			//Location Filter UI for autocomplete of admin class catalog
			var elem_admin_class_loc_filter_id=$(input).attr('id'); 
			
			if(elem_admin_class_loc_filter_id=='classLocation-addltext-filter'){
		      options.width='178px';
			  offset.left=offset.left-12;
			  loc_filter_ac='1';
			}
			 
			//Location Filter UI for autocomplete of admin manage location
			var elem_admin_manage_loc_filter_id=$(input).attr('id'); 
			var elem_admin_manage_loc_filter_name=$(input).attr('name');  
			
			if(elem_admin_manage_loc_filter_id=='rescitystate-addltext-filter' && elem_admin_manage_loc_filter_name=='LocationFilter'){
			    options.width='179px';
			    offset.left=offset.left-12;
			    loc_filter_ac='1';			
			} 
			
			//Location Filter UI for Manage Location Top Search
			if($(input).hasClass('manage_location_top_txt_filter')){  
			     options.width='232px';
			     offset.left=offset.left-15;
			     loc_filter_ac='1';
			     //ac_top_ac_search='1';
			     top_search_cls='ac_jscroll_results_top_panel_li_div'; 
			} 
			
			//Location Filter UI for Admin Catalog Class Edit and Add the class location
			if($(input).hasClass('addedit-edit-new_location')){
			    options.width='163px'; 
			    offset.left=offset.left;
			    loc_filter_ac='1';	 
			}   
			
			//Location Filter UI for the my learning/my programs
			var elem_user_mylearing_loc_id=$(input).attr('id');  
			if(elem_user_mylearing_loc_id=='myenrollment-location' || elem_user_mylearing_loc_id=='myprograms-location'){
			  options.width='155px';   
			  offset.left=offset.left+1;
			  loc_filter_ac='1';
			  top_search_cls='ac_jscroll_results_mylearning_panel_li_div';
			} 
			
			//Location Filter UI for the my classes
			if(elem_user_mylearing_loc_id=='myclasses-location'){
			  options.width='168px';  
			  offset.left=offset.left+1;
			  loc_filter_ac='1';
			  top_search_cls='ac_jscroll_results_mylearning_panel_li_div';
			}  
			
			element.css({
				width: typeof options.width == "string" || options.width > 0 ? options.width : $(input).width(),
				top: offset.top + input.offsetHeight,
				left: offset.left
			}).show();
			
			 
			hide_findTraining(); 
			
			if(loc_filter_ac=='1'){ // Initiate JScrollpane for Location Filter 
				
				 options.scroll=false;  
				  
				 var elem_id='#'+$(element).attr('id'); 
				 
				 $(elem_id).addClass('ac_jscroll_results');
				 
				 $(elem_id + ' ul li').each(function( index ) {
				    var li_txt=$(this).html();
				    var li_txt_format='<div class=\''+top_search_cls+'\'>'+li_txt+'</div>';
				    $(this).html(li_txt_format);
				});   
			   
				var ac_res_height=$(elem_id + " ul").height();  
				 
				if(ac_res_height > options.scrollHeight){
					Js_scrollbarsHeight=options.scrollHeight;
				}else{
					Js_scrollbarsHeight=ac_res_height; 
				}   
				
				$(elem_id).css('height',Js_scrollbarsHeight);
				$(elem_id).css('width', options.width);  
				
				$(elem_id).jScrollPane({
					autoReinitialise: true,
					verticalGutter:0,
					horizontalGutter:0				 	 					 
				});  
				 	 
		    }
			
			
            if(options.scroll) {
                list.scrollTop(0);
                list.css({
					maxHeight: options.scrollHeight
					/*overflow: 'hidden'*/
				});
				
                if($.browser.msie && typeof document.body.style.maxHeight === "undefined") {
					var listHeight = 0;
					listItems.each(function() {
						listHeight += this.offsetHeight;
					});
					var scrollbarsVisible = listHeight > options.scrollHeight;
                    list.css('height', scrollbarsVisible ? options.scrollHeight : listHeight );
					if (!scrollbarsVisible) {
						// IE doesn't recalculate width when scrollbar disappears
						listItems.width( list.width() - parseInt(listItems.css("padding-left")) - parseInt(listItems.css("padding-right")) );
					}
                }
                
            }
		},
		selected: function() {
			var selected = listItems && listItems.filter("." + CLASSES.ACTIVE).removeClass(CLASSES.ACTIVE);
			return selected && selected.length && $.data(selected[0], "ac_data");
		},
		emptyList: function (){
			list && list.empty();
		},
		unbind: function() {
			element && element.remove();
			show_findTraining();
		}
	};
};

$.fn.selection = function(start, end) {
	if (start !== undefined) {
		return this.each(function() {
			if( this.createTextRange ){
				var selRange = this.createTextRange();
				if (end === undefined || start == end) {
					selRange.move("character", start);
					selRange.select();
				} else {
					selRange.collapse(true);
					selRange.moveStart("character", start);
					selRange.moveEnd("character", end);
					selRange.select();
				}
			} else if( this.setSelectionRange ){
				this.setSelectionRange(start, end);
			} else if( this.selectionStart ){
				this.selectionStart = start;
				this.selectionEnd = end;
			}
		});
	}
	var field = this[0];
	if ( field.createTextRange ) {
		var range = document.selection.createRange(),
			orig = field.value,
			teststring = "<->",
			textLength = range.text.length;
		range.text = teststring;
		var caretAt = field.value.indexOf(teststring);
		field.value = orig;
		this.selection(caretAt, caretAt + textLength);
		return {
			start: caretAt,
			end: caretAt + textLength
		}
	} else if( field.selectionStart !== undefined ){
		return {
			start: field.selectionStart,
			end: field.selectionEnd
		}
	}
};

/*
 * A JavaScript function equivalent of PHP’s rawurldecode
 */
function rawurldecode(str) {
	  return decodeURIComponent((str + '')
	    .replace(/%(?![\da-f]{2})/gi, function() {
	      // PHP tolerates poorly formed escape sequences
	      return '%25';
	    }));
	}
})(jQuery);