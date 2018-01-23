
H5P.DragNBar = (function (EventDispatcher) {

  /**
   * Constructor. Initializes the drag and drop menu bar.
   *
   * @class
   * @param {Array} buttons
   * @param {H5P.jQuery} $container
   * @param {H5P.jQuery} $dialogContainer
   * @param {object} [options] Collection of options
   * @param {boolean} [options.disableEditor=false] Determines if DragNBar should be displayed in view or editor mode
   * @param {H5P.jQuery} [options.$blurHandlers] When clicking these element(s) dnb focus will be lost
   */
  function DragNBar(buttons, $container, $dialogContainer, options) {
    var self = this;
	
    EventDispatcher.call(this);
    
    this.overflowThreshold = 17; // How many buttons to display before we add the more button.
    this.buttons = buttons;
    this.$container = $container;
    this.$dialogContainer = $dialogContainer;
    this.dnd = new H5P.DragNDrop(this, $container);
    this.dnd.snap = 10;
    this.newElement = false;
    var defaultOptions = {
      disableEditor: false
    };
    options = H5P.jQuery.extend(defaultOptions, options);
    this.isEditor = !options.disableEditor;
    this.$blurHandlers = options.$blurHandlers ? options.$blurHandlers : undefined;

    /**
     * Keeps track of created DragNBar elements
     * @type {Array}
     */
    this.elements = [];

    // Create a popup dialog
    this.dialog = new H5P.DragNBarDialog($dialogContainer, $container);

    // Fix for forcing redraw on $container, to avoid "artifcats" on safari
    this.$container.addClass('hardware-accelerated');
	
	//h5pcustomize
	try
	{
	ns.$(".h5peditor-form").find("fieldset").hide();
	ns.$(".h5peditor-form").find("div.common").hide();
	ns.$(".h5peditor-label").each(function(){
		if(ns.$(this).text()== "") //"Interactive Video Editor")
			{
			ns.$(this).hide();
			return;
			}
	});
	}catch(e){}
	
    if (this.isEditor) {
      this.initEditor();
      this.initClickListeners();

      H5P.$window.resize(function () {
        self.resize();
      });
    }
  }

  // Inherit support for events
  DragNBar.prototype = Object.create(EventDispatcher.prototype);
  DragNBar.prototype.constructor = DragNBar;

  return DragNBar;
})(H5P.EventDispatcher);

/**
 * Initializes editor functionality of DragNBar
 */
H5P.DragNBar.prototype.initEditor = function () {

  var that = this;
  this.dnr = new H5P.DragNResize(this.$container);
  this.dnr.snap = 10;

  // Update coordinates when element is resized
  this.dnr.on('moveResizing', function () {
    var offset = that.$element.offset();
    var position = that.$element.position();
    that.updateCoordinates(offset.left, offset.top, position.left, position.top);
  });

  // Set pressed to not lose focus at the end of resize
  this.dnr.on('stoppedResizing', function () {
    that.pressed = true;

    // Delete pressed after dnbelement has been refocused so it will lose focus on single click.
    setTimeout(function () {
      delete that.pressed;
    }, 10);
  });

  /**
   * Show transform panel listener
   */
  this.dnr.on('showTransformPanel', function () {
    if (that.focusedElement) {
      that.focusedElement.contextMenu.trigger('contextMenuTransform', {showTransformPanel: true});
    }
  });

  this.dnd.on('showTransformPanel', function () {
    // Get moving element and show transform panel
    if (that.focusedElement) {
      that.focusedElement.contextMenu.trigger('contextMenuTransform', {showTransformPanel: true});
    }
  });

  this.dnd.startMovingCallback = function (x, y) {
    that.dnd.min = {x: 0, y: 0};
    that.dnd.max = {
      x: that.$container.width() - that.$element.outerWidth(),
      y: that.$container.height() - that.$element.outerHeight()
    };

    if (that.newElement) {
      that.dnd.adjust.x = 10;
      that.dnd.adjust.y = 10;
      that.dnd.min.y -= that.$list.height();
    }

    return true;
  };

  this.dnd.stopMovingCallback = function (event) {
    var pos = {};

    if (that.newElement) {
      that.$container.css('overflow', '');
      if (Math.round(parseFloat(that.$element.css('top'))) < 0) {
        // Try to center element, but avoid overlapping
        pos.x = (that.dnd.max.x / 2);
        pos.y = (that.dnd.max.y / 2);
        that.avoidOverlapping(pos, that.$element);
      }
    }

    if (pos.x === undefined || pos.y === undefined ) {
      pos.x = Math.round(parseFloat(that.$element.css('left')));
      pos.y = Math.round(parseFloat(that.$element.css('top')));
    }

    that.stopMoving(pos.x, pos.y);
    that.newElement = false;

    delete that.dnd.min;
    delete that.dnd.max;
  };
};

/**
 * Tries to position the given element close to the requested coordinates.
 * Element can be skipped to check if spot is available.
 *
 * @param {object} pos
 * @param {number} pos.x
 * @param {number} pos.y
 * @param {(H5P.jQuery|Object)} element object with width&height if ran before insertion.
 */
H5P.DragNBar.prototype.avoidOverlapping = function (pos, $element) {
  // Determine size of element
  var size = $element;
  if (size instanceof H5P.jQuery) {
    size = window.getComputedStyle(size[0]);
    size = {
      width: parseFloat(size.width),
      height: parseFloat(size.height)
    };
  }
  else {
    $element = undefined;
  }

  // Determine how much they can be manuvered
  var containerStyle = window.getComputedStyle(this.$container[0]);
  var manX = parseFloat(containerStyle.width) - size.width;
  var manY = parseFloat(containerStyle.height) - size.height;

  var limit = 16;
  var attempts = 0;

  while (attempts < limit && this.elementOverlaps(pos.x, pos.y, $element)) {
    // Try to place randomly inside container
    if (manX > 0) {
      pos.x = Math.floor(Math.random() * manX);
    }
    if (manY > 0) {
      pos.y = Math.floor(Math.random() * manY);
    }
    attempts++;
  }
};

/**
 * Determine if moving the given element to its new position will cause it to
 * cover another element. This can make new or pasted elements difficult to see.
 * Element can be skipped to check if spot is available.
 *
 * @param {number} x
 * @param {number} y
 * @param {H5P.jQuery} [$element]
 * @returns {boolean}
 */
H5P.DragNBar.prototype.elementOverlaps = function (x, y, $element) {
  var self = this;

  // Use snap grid
  x = Math.round(x / 10);
  y = Math.round(y / 10);

  for (var i = 0; i < self.elements.length; i++) {
    var element = self.elements[i];
    if ($element !== undefined && element.$element === $element) {
      continue;
    }

    if (x === Math.round(parseFloat(element.$element.css('left')) / 10) &&
        y === Math.round(parseFloat(element.$element.css('top')) / 10)) {
      return true; // Stop loop
    }
  }

  return false;
};

/**
 * Initialize click listeners
 */
H5P.DragNBar.prototype.initClickListeners = function () {
  var self = this;

  // Key coordinates
  var CTRL = 17;
  var C = 67;
  var V = 86;

  // Keep track of key state
  var ctrlDown = false;

  // Register event listeners
  H5P.$body.keydown(function (event) {
    if (event.which === CTRL) {
      ctrlDown = true;

      if (self.dnd.snap !== undefined) {
        // Disable snapping
        delete self.dnd.snap;
      }
    }
    else if (event.which === C && ctrlDown && self.focusedElement && self.$container.is(':visible')) {
      // Copy element params to clipboard
      var elementSize = window.getComputedStyle(self.focusedElement.$element[0]);
      var width = parseFloat(elementSize.width);
      var height = parseFloat(elementSize.height) / width;
      width = width / (parseFloat(window.getComputedStyle(self.$container[0]).width) / 100);
      height *= width;

      self.focusedElement.toClipboard(width, height);
    }
    else if (event.which === V && ctrlDown && window.localStorage && self.$container.is(':visible')) {
      if (self.preventPaste || self.dialog.isOpen() || document.activeElement.contentEditable === 'true' || document.activeElement.value !== undefined) {
        // Don't allow paste if dialog is open or active element can be modified
        return;
      }

      var clipboardData = localStorage.getItem('h5pClipboard');
      if (clipboardData) {

        // Parse
        try {
          clipboardData = JSON.parse(clipboardData);
        }
        catch (err) {
          console.error('Unable to parse JSON from clipboard.', err);
          return;
        }

        // Update file URLs
        if (clipboardData.contentId !== H5PEditor.contentId) {
          var prefix = clipboardData.contentId ? '../' + clipboardData.contentId : '../../editor';
          H5P.DragNBar.updateFileUrls(clipboardData.specific, prefix);
        }

        if (clipboardData.generic) {
          // Use reference instead of key
          clipboardData.generic = clipboardData.specific[clipboardData.generic];

          // Avoid multiple content with same ID
          delete clipboardData.generic.subContentId;
        }

        self.trigger('paste', clipboardData);
      }
    }
  }).keyup(function (event) {
    if (event.which === CTRL) {
      // Update key state
      ctrlDown = false;

      // Enable snapping
      self.dnd.snap = 10;
    }
  }).click(function () {
    // Remove pressed on click
    delete self.pressed;
  });

  // Set blur handler element if option has been specified
  var $blurHandlers = this.$container;
  if (this.$blurHandlers) {
    $blurHandlers = this.$blurHandlers;
  }

  $blurHandlers.click(function () {
    // Remove coordinates picker if we didn't press an element.
    if (self.pressed !== undefined) {
      delete self.pressed;
    }
    else {
      self.blurAll();
      if (self.focusedElement !== undefined) {
        delete self.focusedElement;
      }
    }
  });
};

/**
 * Update file URLs. Useful when copying between different contents.
 *
 * @param {object} params Reference
 * @param {number} contentId From source
 */
H5P.DragNBar.updateFileUrls = function (params, prefix) {
  for (var prop in params) {
    if (params.hasOwnProperty(prop) && params[prop] instanceof Object) {
      var obj = params[prop];
      if (obj.path !== undefined && obj.mime !== undefined) {
        obj.path = prefix + '/' + obj.path;
      }
      else {
        H5P.DragNBar.updateFileUrls(obj, prefix);
      }
    }
  }
};

/**
 * Attaches the menu bar to the given wrapper.
 *
 * @param {jQuery} $wrapper
 * @returns {undefined}
 */
H5P.DragNBar.prototype.attach = function ($wrapper) {
  $wrapper.html('');
  $wrapper.addClass('h5peditor-dragnbar');

  var $list = H5P.jQuery('<ul class="h5p-dragnbar-ul pre-icons"></ul>').appendTo($wrapper);
  this.$list = $list;
  
  for (var i = 0; i < this.buttons.length; i++) {
    var button = this.buttons[i];
    if (i === this.overflowThreshold) {
      $list = H5P.jQuery('<li class="h5p-dragnbar-li"><a href="#" title="' + 'More elements' + '" class="h5p-dragnbar-a h5p-dragnbar-more-button"></a><ul class="h5p-dragnbar-li-ul"></ul></li>')
        .appendTo($list)
        .click(function () {
          return false;
        })
        .hover(function () {
          $list.stop().slideToggle(300);
        }, function () {
          $list.stop().slideToggle(300);
        })
        .children(':first')
        .next();
    }

    this.addButton(button, $list);
    
  	 vtip();

    
  }
     
    
  // H5P.jQuery(".h5p-dragnbar-ul li:last").css("border-right","1px SOLID #ddd");
	  //h5pcustomize
	try
    {
        var browser = checkBrowser();
        var isAtLeastIE11 = !!(navigator.userAgent.match(/Trident/) && !navigator.userAgent.match(/MSIE/))? true : false;
        
        if(isAtLeastIE11==1){
        	loadCssFilesH5p('/sites/all/themes/core/expertusoneV2/expertusone-internals/css/ie11-h5p.css');
        }
         if(isAtLeastIE11==1 && window.parent.Drupal.settings.user.language !== undefined && H5P.jQuery.inArray(window.parent.Drupal.settings.user.language, ["ru"]) != -1)
         {
	     	loadCssFilesH5p('/sites/all/themes/core/expertusoneV2/expertusone-internals/lang-css/ru/ie11_h5p_ru.css');
         }
         if(isAtLeastIE11==1 && window.parent.Drupal.settings.user.language !== undefined && H5P.jQuery.inArray(window.parent.Drupal.settings.user.language, ["fr"]) != -1) {
        	 loadCssFilesH5p('/sites/all/themes/core/expertusoneV2/expertusone-internals/lang-css/fr/ie11_h5p_fr.css');
         }
	      if(window.parent.Drupal.settings.user.language !== undefined && H5P.jQuery.inArray(window.parent.Drupal.settings.user.language, ["ru"]) != -1) {		
	    	  loadCssFilesH5p('/sites/all/themes/core/expertusoneV2/expertusone-internals/lang-css/ru/lang_h5p_ru.css');
	      }
	      else if(window.parent.Drupal.settings.user.language !== undefined && H5P.jQuery.inArray(window.parent.Drupal.settings.user.language, ["it"]) != -1){
	    	  loadCssFilesH5p('/sites/all/themes/core/expertusoneV2/expertusone-internals/lang-css/it/lang_h5p_it.css');
	      }
	      else if(window.parent.Drupal.settings.user.language !== undefined && H5P.jQuery.inArray(window.parent.Drupal.settings.user.language, ["de"]) != -1) {
	    	  loadCssFilesH5p('/sites/all/themes/core/expertusoneV2/expertusone-internals/lang-css/de/lang_h5p_de.css');
	    	  }
    	  else if(window.parent.Drupal.settings.user.language !== undefined && H5P.jQuery.inArray(window.parent.Drupal.settings.user.language, ["es"]) != -1){
    		  loadCssFilesH5p('sites/all/themes/core/expertusoneV2/expertusone-internals/lang-css/es/lang_h5p_es.css');  
	    	  }
    	  else if(window.parent.Drupal.settings.user.language !== undefined && H5P.jQuery.inArray(window.parent.Drupal.settings.user.language, ["ko"]) != -1){
    		  loadCssFilesH5p('sites/all/themes/core/expertusoneV2/expertusone-internals/lang-css/ko/lang_h5p_ko.css');  
	    	  }
	      else if(window.parent.Drupal.settings.user.language !== undefined && H5P.jQuery.inArray(window.parent.Drupal.settings.user.language, ["zh-hans"]) != -1){
	    	  loadCssFilesH5p('/sites/all/themes/core/expertusoneV2/expertusone-internals/lang-css/zh-hans/lang_h5p_zh-hans.css');
	      }
	      else if(window.parent.Drupal.settings.user.language !== undefined && H5P.jQuery.inArray(window.parent.Drupal.settings.user.language, ["fr"]) != -1){
	    	  loadCssFilesH5p('/sites/all/themes/core/expertusoneV2/expertusone-internals/lang-css/fr/lang_h5p_fr.css');
	      }
	      else if(window.parent.Drupal.settings.user.language !== undefined && H5P.jQuery.inArray(window.parent.Drupal.settings.user.language, ["ja"]) != -1){
	    	  loadCssFilesH5p('/sites/all/themes/core/expertusoneV2/expertusone-internals/lang-css/ja/lang_h5p_ja.css');
	      }
	      else if(window.parent.Drupal.settings.user.language !== undefined && H5P.jQuery.inArray(window.parent.Drupal.settings.user.language, ["pt-pt"]) != -1){
	    	  loadCssFilesH5p('/sites/all/themes/core/expertusoneV2/expertusone-internals/lang-css/pt-pt/lang_h5p_pt-pt.css');
	      }

		window.parent.parent.$(".edit-edit-btn-prerequest").addClass("h5peditorloaded");
		window.parent.parent.displayEditorInEditMode();
		
    	if(!window.parent.parent.$(".edit-edit-btn-details").hasClass("selected"))
    	{
    	
    		window.parent.parent.$("#iframe_editor_wrapper_container").css("height", "425px");
    		window.parent.parent.$("#iframe_editor_wrapper_container").css("margin-top", "-9px");
    		
    		window.parent.parent.document.getElementById("iframe_editor_wrapper").style.display="block";
    		window.setTimeout(function(){
    		window.parent.parent.document.getElementById("iframe_editor_wrapper").style.visibility="visible";
    		},0);

     		window.parent.EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.destroyLoader("exp-sp-administration-contentauthor-addedit-form");
    
    		window.parent.parent.document.getElementById("iframe_editor_wrapper_container").style.display="block";
    
    		window.parent.parent.EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.destroyLoader("contentauthor-addedit-form-container");
     		window.parent.parent.document.getElementById("iframe_editor").style.display="block";
    		window.parent.parent.document.getElementById("h5pe1loading").style.display="none";
    		
    	}
    	
    		
    	
    }catch(e){
    }
    
    
	  
	//H5P.jQuery("#h5peditor-library").show();
	

  	window.parent.$("#h5pe1loading").hide();

   
  	window.setTimeout(function(){
  		H5P.jQuery(".h5p-footer-toggle-keywords").insertBefore(".h5p-dragnbar-left");
	window.parent.$("#edit-actions").show();
	window.parent.$("#edit-submit").show();
    window.parent.$("#edit-delete").show();

	H5P.jQuery(".h5p-footer-toggle-keywords").hide();
	
	
	
	
	H5P.jQuery(".h5p-slidecontrols").appendTo(".h5p-dragnbar");
	H5P.jQuery(".h5p-slidecontrols").show();
	if(H5P.jQuery(".h5p-course-presentation").size() == 0) //for presentation it will be shown in cp-editor.js
		window.parent.$(".addedit-form-cancel-container-actions").show();
		  		
  		//H5P.jQuery(".h5p-footer-toggle-keywords").show();
  		loadScriptsForTinyMCE("/misc/jquery.js");
  		
  		loadScriptsForTinyMCE("/misc/jquery.once.js");
  		loadScriptsForTinyMCE("/misc/drupal.js");
		
		Drupal.wysiwyg = Drupal.wysiwyg || { 'instances': {} };
		Drupal.wysiwyg.editor = Drupal.wysiwyg.editor || { 'init': {}, 'attach': {}, 'detach': {}, 'instance': {} };
		Drupal.wysiwyg.plugins = Drupal.wysiwyg.plugins || {};
		Drupal.settings.wysiwyg = {"configs":{"tinymce":{ "mode" : "specific_textareas","id": "edit-short-description-value","language":"en","popup_css":"/sites/all/libraries/tinymce/jscripts/tiny_mce/themes/advanced/skins/default/dialog_theme2.css","theme":"advanced","skin":"default","theme_advanced_buttons1":"bold,italic,underline,fontselect,fontsizeselect,forecolor,backcolor","formatfiltered_html":{"button_tile_map":true,"document_base_url":"/","mode":"none","plugins":"","theme":"advanced","width":"100%","strict_loading_mode":true,"convert_urls":false,"entities":"160,nbsp,173,shy,8194,ensp,8195,emsp,8201,thinsp,8204,zwnj,8205,zwj,8206,lrm,8207,rlm","theme_advanced_resize_horizontal":false,"theme_advanced_resizing_use_cookie":false,"theme_advanced_statusbar_location":"bottom","theme_advanced_resizing":1,"theme_advanced_toolbar_location":"top","theme_advanced_toolbar_align":"left","theme_advanced_buttons1":"bold,italic,underline,fontselect,fontsizeselect,forecolor,backcolor","theme_advanced_buttons2":"","theme_advanced_buttons3":""},"formatfull_html":{"button_tile_map":true,"document_base_url":"/","mode":"none","plugins":"","theme":"advanced","width":"100%","strict_loading_mode":true,"convert_urls":false,"entities":"160,nbsp,173,shy,8194,ensp,8195,emsp,8201,thinsp,8204,zwnj,8205,zwj,8206,lrm,8207,rlm","apply_source_formatting":"0","convert_fonts_to_spans":"1","language":"en","paste_auto_cleanup_on_paste":"0","preformatted":"0","remove_linebreaks":"1","verify_html":true,"content_css":"/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_learning/exp_sp_administration_data_grid_v2.css,/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_view_v2.css,/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_v2.css,/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_catalog_access/exp_sp_administration_catalog_access_v2.css,/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_learning/exp_sp_administration_learning_v2.css,/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_learning/exp_sp_administration_program/exp_sp_administration_program_v2.css,/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_learning/exp_sp_administration_program/font-awesome.min.css,/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_learning/exp_sp_administration_program/exp_sp_administration_program_roster_v2.css,/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_survey/exp_sp_administration_surveyquestions/exp_sp_administration_surveyquestions_v2.css,/sites/all/modules/core/exp_sp_core/js/exp_sp_jquery/exp_multiselect/css/jquery.expertus.multiselect.css,/sites/all/themes/core/AdministrationTheme/calender_style.css,/sites/all/modules/core/exp_sp_core/js/videojs/video-js.min.css,/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_manage/exp_sp_administration_content/exp_sp_administration_content_v2.css","extended_valid_elements":"","theme_advanced_resize_horizontal":false,"theme_advanced_resizing_use_cookie":false,"theme_advanced_statusbar_location":"bottom","theme_advanced_resizing":"1","theme_advanced_toolbar_location":"top","theme_advanced_toolbar_align":"left","theme_advanced_blockformats":"p,address,pre,h2,h3,h4,h5,h6,div","theme_advanced_buttons1":"bold,italic,underline,fontselect,fontsizeselect,forecolor,backcolor","theme_advanced_buttons2":"","theme_advanced_buttons3":""},"formatplain_text":{"button_tile_map":true,"document_base_url":"/","mode":"none","plugins":"","theme":"advanced","width":"100%","strict_loading_mode":true,"convert_urls":false,"entities":"160,nbsp,173,shy,8194,ensp,8195,emsp,8201,thinsp,8204,zwnj,8205,zwj,8206,lrm,8207,rlm","theme_advanced_resize_horizontal":false,"theme_advanced_resizing_use_cookie":false,"theme_advanced_statusbar_location":"bottom","theme_advanced_resizing":1,"theme_advanced_toolbar_location":"top","theme_advanced_toolbar_align":"left","theme_advanced_buttons1":"bold,italic,underline,fontselect,fontsizeselect,forecolor,backcolor","theme_advanced_buttons2":"","theme_advanced_buttons3":""}}},"plugins":[],"disable":"Disable rich-text","enable":"Enable rich-text","triggers":{"edit-short-description-format--2":{"field":"edit-short-description-value","formatfiltered_html":{"editor":"tinymce","status":1,"toggle":1,"resizable":1,"format":"formatfiltered_html","trigger":"edit-short-description-format--2","field":"edit-short-description-value"},"formatfull_html":{"editor":"tinymce","status":1,"toggle":0,"resizable":1,"format":"formatfull_html","trigger":"edit-short-description-format--2","field":"edit-short-description-value"},"formatplain_text":{"editor":"tinymce","status":1,"toggle":1,"resizable":1,"format":"formatplain_text","trigger":"edit-short-description-format--2","field":"edit-short-description-value"}}}};
		window.tinyMCEPreInit = {"base":"\/sites\/all\/libraries\/tinymce\/jscripts\/tiny_mce","suffix":"","query":""};

  		loadScriptsForTinyMCE("/sites/all/libraries/tinymce/jscripts/tiny_mce/tiny_mce.js");
  		loadScriptsForTinyMCE("/sites/all/modules/core/exp_sp_core/modules/wysiwyg/editors/js/tinymce-3.js");
  		loadScriptsForTinyMCE("/sites/all/modules/core/exp_sp_core/modules/wysiwyg/editors/js/none.js");
  		loadScriptsForTinyMCE("/modules/filter/filter.js");
  		loadScriptsForTinyMCE("/sites/all/modules/core/exp_sp_core/modules/wysiwyg/wysiwyg.js");
  		loadScriptsForTinyMCE("/sites/all/libraries/tinymce/jscripts/tiny_mce/langs/en.js");
  		loadScriptsForTinyMCE("/sites/all/libraries/tinymce/jscripts/tiny_mce/themes/advanced/h5peditor_template_src.js");
  		
  		//loadScriptsForTinyMCE("/h5pmerge/lazyloadpresentationscripts.js");
  		console.log("lazyloadpresentationscripts....new....");
  		
		console.log("after tinymce:"+new Date().getTime());
	
	
  		
  	},100);
  	
  
  
  

  	
};

/**
 * Add button.
 *
 * @param {type} button
 * @param {Function} button.createElement Function for creating element
 * @param {type} $list
 * @returns {undefined}
 */
H5P.DragNBar.prototype.addButton = function (button, $list) {
  var that = this;
    
    H5P.jQuery('<li class="h5p-dragnbar-li" title="' + window.parent.Drupal.t(button.title) + '"><a href="#"  class=" h5p-dragnbar-a  h5p-dragnbar-' + button.id + '-button"></a></li>')
   //H5P.jQuery('<li class="h5p-dragnbar-li"><span class="vtip" title="' + button.title + '"><a href="#"  class="h5p-dragnbar-a  h5p-dragnbar-' + button.id + '-button"></a></span></li>')
    .addClass("vtip")
    .appendTo($list)
    .children()
    .click(function () {
    //H5P.jQuery(".h5p-dragnbar-li").find(".vtip").remove;
     // alert(22222222);
      return false;
    }).mousedown(function (event) {
      if (event.which !== 1) {
        return;
      }
    
      that.newElement = true;
      that.pressed = true;
      var createdElement = button.createElement();
      that.$element = createdElement;
      that.$container.css('overflow', 'visible');
      that.dnd.press(that.$element, event.pageX, event.pageY);
      that.focus(that.$element);
    });
  
};

/**
 * Change container.
 *
 * @param {jQuery} $container
 * @returns {undefined}
 */
H5P.DragNBar.prototype.setContainer = function ($container) {
  this.$container = $container;
  this.dnd.$container = $container;
};

/**
 * Handler for when the dragging stops. Makes sure the element is inside its container.
 *
 * @param {Number} left
 * @param {Number} top
 * @returns {undefined}
 */
H5P.DragNBar.prototype.stopMoving = function (left, top) {
  // Calculate percentage
  top = top / (this.$container.height() / 100);
  left = left / (this.$container.width() / 100);
  this.$element.css({top: top + '%', left: left + '%'});

  // Give others the result
  if (this.stopMovingCallback !== undefined) {
    this.stopMovingCallback(left, top);
  }
};

/**
 * Makes it possible to focus and move the element around.
 * Must be inside $container.
 *
 * @param {H5P.jQuery} $element
 * @param {Object} [options]
 * @param {H5P.DragNBarElement} [options.dnbElement] Register new element with dnbelement
 * @param {boolean} [options.disableResize] Resize disabled
 * @param {boolean} [options.lock] Lock ratio during resize
 * @param {string} [clipboardData]
 * @returns {H5P.DragNBarElement} Reference to added dnbelement
 */
H5P.DragNBar.prototype.add = function ($element, clipboardData, options) {
  var self = this;
  options = options || {};
  if (this.isEditor && !options.disableResize) {
    this.dnr.add($element, options);
  }
  var newElement = null;

  // Check if element already exist
  if (options.dnbElement) {
    // Set element as added element
    options.dnbElement.setElement($element);
    newElement = options.dnbElement;
  }
  else {
    options.element = $element;
    newElement = new H5P.DragNBarElement(this, clipboardData, options);
    this.elements.push(newElement);
  }

  $element.addClass('h5p-dragnbar-element');

  if ($element.attr('tabindex') === undefined) {
    // Make it possible to tab between elements.
    $element.attr('tabindex', 1);
  }

  if (this.isEditor) {
    $element.mousedown(function (event) {
      if (event.which !== 1) {
        return;
      }

      self.pressed = true;
      self.focus($element);
      if (self.dnr.active !== true) { // Moving can be stopped if the mousedown is doing something else
        self.dnd.press($element, event.pageX, event.pageY);
      }
    });
  }

  $element.focus(function () {
    self.focus($element);
  });

  return newElement;
};

/**
 * Remove given element in the UI.
 *
 * @param {H5P.DragNBarElement} dnbElement
 */
H5P.DragNBar.prototype.removeElement = function (dnbElement) {
  dnbElement.removeElement();
};

/**
 * Select the given element in the UI.
 *
 * @param {jQuery} $element
 * @returns {undefined}
 */
H5P.DragNBar.prototype.focus = function ($element) {
  var self = this;

  // Blur last focused
  if (this.focusedElement && this.focusedElement.$element !== $element) {
    this.focusedElement.blur();
    this.focusedElement.hideContextMenu();
  }

  // Keep track of the element we have in focus
  self.$element = $element;

  // Show and update coordinates picker
  this.focusedElement = this.getDragNBarElement($element);

  if (this.focusedElement) {
    this.focusedElement.showContextMenu();
    this.focusedElement.focus();
    self.updateCoordinates();
  }

  // Wait for potential recreation of element
  setTimeout(function () {
    self.updateCoordinates();
    if (self.focusedElement && self.focusedElement.contextMenu && self.focusedElement.contextMenu.canResize) {
      self.focusedElement.contextMenu.updateDimensions();
    }
  }, 0);
};

/**
 * Get dnbElement from $element
 * @param {jQuery} $element
 * @returns {H5P.DragNBarElement} dnbElement with matching $element
 */
H5P.DragNBar.prototype.getDragNBarElement = function ($element) {
  var foundElement;
  // Find object with matching element
  this.elements.forEach(function (element) {
    if (element.getElement().is($element)) {
      foundElement = element;
    }
  });
  return foundElement;
};

/**
 * Deselect all elements in the UI.
 *
 * @returns {undefined}
 */
H5P.DragNBar.prototype.blurAll = function () {
  this.elements.forEach(function (element) {
    element.blur();
  });
  delete this.focusedElement;
};

/**
 * Resize DnB, make sure context menu is positioned correctly.
 */
H5P.DragNBar.prototype.resize = function () {
  var self = this;
  this.updateCoordinates();

	//alert(self.$element);
  if (self.focusedElement) {
  	//h5pcustomize - commented below resizable code due to left undefined error. will look into it later.
    //self.focusedElement.resizeContextMenu(self.$element.offset().left - self.$element.parent().offset().left);
  }
};

/**
 * Update the coordinates of context menu.
 *
 * @param {Number} [left]
 * @param {Number} [top]
 * @param {Number} [x]
 * @param {Number} [y]
 * @returns {undefined}
 */
H5P.DragNBar.prototype.updateCoordinates = function (left, top, x, y) {
  if (!this.focusedElement) {
    return;
  }

  var containerPosition = this.$container.position();

  if (left && top && x && y) {
    left = x + containerPosition.left;
    top = y + containerPosition.top;
    this.focusedElement.updateCoordinates(left, top, x, y);
  }
  else {
    var position = this.$element.position();
    this.focusedElement.updateCoordinates(position.left + containerPosition.left, position.top + containerPosition.top, position.left, position.top);
  }
};

/**
 * Creates element data to store in the clipboard.
 *
 * @param {string} from Source of the element
 * @param {object} params Element options
 * @param {string} [generic] Which part of the parameters can be used by other libraries
 * @returns {string} JSON
 */
H5P.DragNBar.clipboardify = function (from, params, generic) {
  var clipboardData = {
    from: from,
    specific: params
  };

  if (H5PEditor.contentId) {
    clipboardData.contentId = H5PEditor.contentId;
  }

  // Add the generic part
  if (params[generic]) {
    clipboardData.generic = generic;
  }

  return clipboardData;
};

/**
 * @typedef SizeNPosition
 * @type Object
 * @property {number} width Width of the Element
 * @property {number} height Height of the Element
 * @property {number} left The X Coordinate
 * @property {number} top The Y Coordinate
 */

/**
 * Calculates position and size for the given element (in pixels)
 *
 * @throws Error if invalid type
 * @param {Element} element
 * @param {string} [type=inner] Possible values "inner" and "outer"
 * @returns {SizeNPosition}
 */
H5P.DragNBar.getSizeNPosition = function (element, type) {
  type = type || 'inner';
  var style;
  switch (type) {
    case 'inner':
      style = window.getComputedStyle(element);
      break;
    case 'outer':
      style = element.getBoundingClientRect();
      break;
    default:
      throw 'Unknown type';
  }

  return {
    width: parseFloat(style.width),
    height: parseFloat(style.height),
    left: parseFloat(style.left),
    top: parseFloat(style.top)
  };
};

/**
 * Make sure the given element is inside the container.
 *
 * @param {H5P.jQuery} $element
 * @param {SizeNPosition} containerSize
 * @returns {SizeNPosition} Only the properties which require change
 */
H5P.DragNBar.fitElementInside = function ($element, containerSize) {
  var elementSize = H5P.DragNBar.getSizeNPosition($element[0], 'outer');
  var elementPosition = H5P.DragNBar.getSizeNPosition($element[0], 'inner');
  var style = {};

  if (elementPosition.left < 0) {
    // Element sticks out of the left side
    style.left = elementPosition.left = 0;
  }

  if (elementSize.width + elementPosition.left > containerSize.width) {
    // Element sticks out of the right side
    style.left = containerSize.width - elementSize.width;
    if (style.left < 0) {
      // Element is wider than the container
      style.left = 0;
      style.width = containerSize.width;
    }
  }

  if (elementPosition.top < 0) {
    // Element sticks out of the top side
    style.top = elementPosition.top = 0;
  }

  if (elementSize.height + elementPosition.top > containerSize.height) {
    // Element sticks out of the bottom side
    style.top = containerSize.height - elementSize.height;
    if (style.top < 0) {
      // Element is higher than the container
      style.top = 0;
      style.height = containerSize.height;
    }
  }

  return style;
};

if (window.H5PEditor) {
  // Add translations
  H5PEditor.language['H5P.DragNBar'] = {
    libraryStrings: {
      transformLabel: window.parent.Drupal.t('LBL3206'),
      editLabel: window.parent.Drupal.t('LBL063'),
      removeLabel: window.parent.Drupal.t('LBL082'),
      bringToFrontLabel: window.parent.Drupal.t('LBL3205'),
      unableToPaste: 'Cannot paste this object. Unfortunately, the object you are trying to paste is not supported by this content type or version.',
      sizeLabel: window.parent.Drupal.t('LBL1288'),
      positionLabel: window.parent.Drupal.t('Position')
    }
  };
}
function displayQuestionSet(quesandans,cnt)
{
	var html = "";
	var count = 0;
	if(ns.$(".questionset").size() == 0)
		count = 1;
	else
		count =ns.$(".questionset").size()+1;
	
	if(cnt != null && cnt != "" && cnt != undefined)
		count = cnt;
	if(quesandans == null || quesandans.question == undefined || quesandans.question == "" || quesandans.question == null)
	{
		quesandans = {'question':'','answers':''};
		//quesandans.question = "";
	}
	html += "<div class='questionset questionset"+count+"' qset='questionset"+count+"'>"; 
	
	html += '<div class="field text h5peditor-interaction-label questioncontainer" >';
	html += '      <label class="h5peditor-label"><span class="h5peditor-label h5peditor-required">'+window.parent.Drupal.t('LBL325') +count+':</span></label>';
	html += '      <div class="ckeditor striptags" tabindex="0" contenteditable="true">'+quesandans.question+'</div>';
	
	
	if(count==1)
	{
	html +='<a href="#" class="close_icon_ques_ans" onclick="removeEntity(this);"  style="display:none" >x</a>';
	}
	else {
	html +='<a href="#" class="close_icon_ques_ans quesBlock" onclick="removeEntity(this);">x</a>';	
	}
	
	
	html += '      <div class="h5p-errors"></div>';
	html += '      <div class="h5peditor-field-description question" ></div>';
   	html += '</div>'; 

	html += '<div class="answercontainerwrapper">';
	if(quesandans.answers != "" && quesandans.answers != null)
		for(var i = 0; i <quesandans.answers.length;i++)
		{ 
			if(i == 0)
				html += addAnswerSet({'count':1,'answer':'('+window.parent.Drupal.t('LBL714')+')','questionCount':'','defaultvalue':quesandans.answers[i]});
			
			else if(i == 1)
			    html += addAnswerSet({'count':2,'answer':'(Second Option)','questionCount':'','defaultvalue':quesandans.answers[i]});

			else{
			//	html += addAnswerSet({'count':1,'answer':'','questionCount':'','defaultvalue':quesandans.answers[i]});
				html += addAnswerSet({'count':i+1,'answer':'','questionCount':'','defaultvalue':quesandans.answers[i]});
			}
		}
	else
	{
		html += addAnswerSet({'count':1,'answer':'('+window.parent.Drupal.t('LBL714')+')','questionCount':'','defaultvalue':''});
		html += addAnswerSet({'count':2,'answer':'(Second Option)','questionCount':'','defaultvalue':''});
	
	}
  	html += '</div>';
  	
    html += '<div class="addedit-form-cancel-container-actions">';
//	html += '<div class="admin-save-pub-unpub-button-container btn-answer"><div class="admin-save-button-left-bg"></div><input class="addedit-edit-contentauthor-basic-save edit-catalog-course-save-publish addedit-form-expertusone-throbber form-submit ajax-processed" onclick="addAnswerSetWrapper(this,'+count+');" data-wrapperid="contentauthor-addedit-form" tabindex="4" type="button" id="addanswer_btn" name="addanswer" value="Add Answer"><span id="pub-unpub-action-btn" onclick="displayPubActionList()" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn">&nbsp;</span></div>';

//	html += '<div class="admin-save-pub-unpub-button-container btn-answer" ><div class="admin-save-button-left-bg"></div><input class="addedit-edit-contentauthor-basic-save edit-catalog-course-save-publish addedit-form-expertusone-throbber form-submit ajax-processed" onclick="addAnswerSetWrapper(this,'+count+');" data-wrapperid="contentauthor-addedit-form" tabindex="4" type="button" id="addquestion_btn" name="addanswer" value="+ Add Answer"><span id="pub-unpub-action-btn" onclick="CheckFunc(this)" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn">&nbsp;';
	html += '<div class="admin-save-pub-unpub-button-container btn-answer" ><div class="admin-save-button-left-bg"></div><input class="addedit-edit-contentauthor-basic-save edit-catalog-course-save-publish addedit-form-expertusone-throbber form-submit ajax-processed" onclick="addAnswerSetWrapper(this,'+count+');" data-wrapperid="contentauthor-addedit-form" tabindex="4" type="button" id="addquestion_btn" name="addanswer" value="'+window.parent.Drupal.t('LBL3033')+'"><span id="pub-unpub-action-btn" onclick="CheckFunc(this)" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn">&nbsp;';

    html += '<div class="admin-save-pub-unpub-button-container btn-question" style="display:none"><div class="e1-tip"></div><div class="admin-save-button-left-bg"></div><input class="addedit-edit-contentauthor-basic-save edit-catalog-course-save-publish addedit-form-expertusone-throbber form-submit ajax-processed addQuesButton" onclick="addQuestionSetWrapper(this);" data-wrapperid="contentauthor-addedit-form" tabindex="4" type="button" id="addquestion_btn" name="addanswer" value="'+window.parent.Drupal.t('Add Question Set')+'"><span id="pub-unpub-action-btn" onclick="displayPubActionList()" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn">&nbsp;</span></div>';
	html += '</span></div>';
  //  html+=displaySingleChoiceSetAction();
	
	
	html += '</div>';

	html += "</div>"; //question set end

	
	 return html;
   	
}
function addAnswerSetWrapper(domobj,count)
{
	var quesandansset = ns.$(domobj).parent().parent().parent().parent();
	//var qset = ns.$(domobj).parent().parent().parent().parent();
	var qset = ns.$(domobj).parent().parent().parent();
	
	count = ns.$(domobj).parent().parent().parent().attr("qset").substr(11);
	
	//params = {'count':qset.find(".answer").size()+1,'answer':''};
	//$(this).closest("div").find('.selected').length;
	//var cnt = ns.$(".option-set").length; answercontainerwrapper
	
	
	/*var cnt = ns.$(".questionset"+count).find(".answercontainerwrapper").find(".answercontainer").find(".option-set").length;
	params = {'count':cnt+1,'answer':''};  //find the option set for current parent tag only
*/
	//params = {'count':qset.find(".answer").size()+1,'answer':''};
	
	params = {'count':qset.find(".answer").length+1,'answer':''};
	
	//var html = addAnswerSet(params);
	var html = addAnswerSet(params,count);
	
	//ns.$(".questionset"+count).find(".answercontainer").last().parent().append(html);
	qset.find(".answercontainer").last().parent().append(html);
}

function addQuestionSetWrapper(domobj)
{
	window.setTimeout(function(){ns.$(".btn-question").css("display","none");},200);
	var quesandansset = ns.$(domobj).parent().parent().parent().parent().parent().parent();
	var qset = ns.$(domobj).parent().parent().parent().parent();
	
	params = {'count':qset.find(".answer").size()+1,'answer':''};
	var html = displayQuestionSet(params);
	quesandansset.append(html);
	
}


function addAnswerSet(params,countVal)
{
 if(params.count < 5)
	 {
	if(params == "")
	{
		params = {'count':'','answer':'','defaultvalue':''};
	}
	if(params.defaultvalue == undefined || params.defaultvalue == "")
	  params.defaultvalue = "";
	var html ="";
	html += '<div class="field text h5peditor-interaction-label answercontainer">';
	
	if(params.answer=='('+window.parent.Drupal.t('LBL714')+')')
	{
//  html += '      <div class="ans-set">';
	html += '      <label class="ans-label"><span class="h5peditor-label h5peditor-required">'+window.parent.Drupal.t('LBL814')+':</span></label>';
	html += '      <div class="ans-set">';
	html += '      <div class="option-set"><label class="h5peditor-label">'+window.parent.Drupal.t('LBL387')+' 1:</label>';// + " " + params.count+'</label>';
	html += '      <div class="ckeditor striptags" tabindex="0" contenteditable="true">'+params.defaultvalue +'</div><span class="ans-correct">'+params.answer+'</span></div>';
	html += '      </div>';
	}
	
	else if(params.answer=='(Second Option)')
	{
//	html += '      <div class="ans-set">';
//	html += '      <label class="ans-label">&nbsp;</label>';			
	html += '      <div class="ans-set">';
	html += '      <div class="option-set"><label class="h5peditor-label">'+window.parent.Drupal.t('LBL387')+' 2:</label>';// + " " + params.count+'</label>';
	html += '      <div class="ckeditor striptags" tabindex="0" contenteditable="true">'+params.defaultvalue+'</div><a href="#" class="close_icon_ques_ans" onclick="removeEntity(this);" style="display:none">x</a></div>';
	html += '      </div>';	
	}
	
//	else{
	else if(params.count == 3 || params.count == 4){
//	html += '      <div class="ans-set">';
//	html += '      <label class="ans-label">&nbsp;</label>';			
	html += '      <div class="ans-set">';
//  html += '      <div class="option-set"><label class="h5peditor-label">Option:</label>';// + " " + params.count+'</label>';
	html += '      <div class="option-set"><label class="h5peditor-label">'+window.parent.Drupal.t('LBL387') +' ' + params.count+':</label>';
	html += '      <div class="ckeditor" tabindex="0" contenteditable="true">'+params.defaultvalue+'</div><a href="#" class="close_icon_ques_ans ansBlock" quesNumber="'+countVal+'" onclick="removeEntity(this);">x</a></div>';
	html += '      </div>';	
	}
	html += '      <div class="h5p-errors"></div>';
	html += '      <div class="h5peditor-field-description answer" > </div>';
   	html += '      </div>';
   	return html;
}

}

function stripTags(html)
{	
  
   var tmp = document.createElement("DIV");
   tmp.innerHTML = html;
   return tmp.textContent || tmp.innerText || "";
}

function pauseEvent(obj)
{
	if(ns.$(obj).is(":checked")){
		ns.$(obj).parent().removeClass("uncheckPause");
		ns.$(obj).parent().addClass("checkPause");
	}
	else{
		ns.$(obj).parent().addClass("uncheckPause");
		ns.$(obj).parent().removeClass("checkPause");

	}
} 
