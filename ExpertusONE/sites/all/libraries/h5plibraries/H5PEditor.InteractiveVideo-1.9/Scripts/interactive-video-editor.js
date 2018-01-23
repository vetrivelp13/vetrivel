/*global H5PEditor, H5P*/
H5PEditor.widgets.interactiveVideo = H5PEditor.InteractiveVideo = (function ($) {

  /**
   * Initialize interactive video editor.
   *
   * @class H5PEditor.InteractiveVideo
   * @param {Object} parent
   * @param {Object} field
   * @param {Object} params
   * @param {function} setValue
   */
  function InteractiveVideoEditor(parent, field, params, setValue) {
    var that = this;

    this.showGuidedTour = false;
    this.parent = parent;
    this.field = field;

    this.findField(this.field.video, function (field) {
      if (field.field.type !== 'video') {
        throw t('notVideoField', {':path': that.field.video});
      }

      if (field.params !== undefined) {
        that.setVideo(field.params);
      }

      field.changes.push(function (file) {
        that.setVideo(field.params);
      });
    });

    this.findField(this.field.poster, function (field) {
      if (field.field.type !== 'image') {
        throw t('notImageField', {':path': that.field.poster});
      }

      if (field.params !== undefined) {
        that.setPoster(field.params);
      }

      field.changes.push(function () {
        that.setPoster(field.params);
      });
    });

    this.params = $.extend({
      interactions: [],
      bookmarks: []
    }, params);
    setValue(field, this.params);

    this.children = [];

    this.passReadies = true;
    parent.ready(function () {
      that.passReadies = false;
    });

    H5P.$window.on('resize', function () {
      if (that.IV) {
        that.IV.trigger('resize');
      }
    });

    // Tour de editor
    this.currentTabIndex = 0;

    // When wizard changes step
    parent.on('stepChanged', function (event) {
      that.currentTabIndex = event.data.id;
      if (event.data.name === 'summary') {
        // Start summary guide
        that.startGuidedTour();
      }
    });
  }

  /**
   * Must be changed if the semantics for the elements changes.
   * @private
   * @type {string}
   */
  InteractiveVideoEditor.clipboardKey = 'H5PEditor.InteractiveVideo';
  /**
   * Find a field, then run the callback.
   *
   * @param {function} callback
   */
  InteractiveVideoEditor.prototype.findField = function (path, callback) {
    var that = this;
    // Find field when tree is ready.
    this.parent.ready(function () {
      var field = H5PEditor.findField(path, that.parent);

      if (!field) {
        throw H5PEditor.t('core', 'unknownFieldPath', {':path': path});
      }

      callback(field);
    });
  };

  /**
   * Our tab has been set active. Create a new player if necessary.
   */
  InteractiveVideoEditor.prototype.setActive = function () {
    if (this.IV !== undefined) {
      // A video has been loaded, no need to recreate.
      return;
    }

    // Reset css
    this.$editor.css({
      width: '',
      height: '',
      fontSize: ''
    });

    if (this.video === undefined) {
      this.$editor.html(t('selectVideo')).removeClass('h5p-interactive-video');
      return;
    }

    var that = this;

    // Create new player.
    this.IV = new H5P.InteractiveVideo({
      interactiveVideo: {
        video: {
          files: this.video,
          poster: this.poster
        },
        assets: this.params
      }
    }, H5PEditor.contentId);
    this.IV.editor = this;
    $(window).on('resize', function () {
      if (that.dnb) {
        that.dnb.resize();
      }
    });
    for (var i = 0; i < this.IV.interactions.length; i++) {
      this.processInteraction(this.IV.interactions[i], this.params.interactions[i]);
    }
    this.IV.on('controls', function () {
      // Add DragNBar.
      that.$bar = $('<div class="h5p-interactive-video-dragnbar">' + t('loading') + '</div>').prependTo(that.$editor);
      var interactions = findField('interactions', that.field.fields);
      var action = findField('action', interactions.field.fields);
      //$.post(H5PEditor.ajaxPath + 'libraries', {libraries: action.options}, function (libraries) 
      //{
        var libraries = [{"uberName":"H5P.Nil 1.0","name":"H5P.Nil","majorVersion":"1","minorVersion":"0","tutorialUrl":null,"title":"Nil","runnable":"0","restricted":false},{"uberName":"H5P.Text 1.1","name":"H5P.Text","majorVersion":"1","minorVersion":"1","tutorialUrl":null,"title":"Text","runnable":"0","restricted":false},{"uberName":"H5P.Link 1.1","name":"H5P.Link","majorVersion":"1","minorVersion":"1","tutorialUrl":null,"title":"Link","runnable":"0","restricted":false},{"uberName":"H5P.Image 1.0","name":"H5P.Image","majorVersion":"1","minorVersion":"0","tutorialUrl":null,"title":"Image","runnable":"0","restricted":false},{"uberName":"H5P.SingleChoiceSet 1.3","name":"H5P.SingleChoiceSet","majorVersion":"1","minorVersion":"3","tutorialUrl":null,"title":"Single Choice Set","runnable":"1","restricted":false},{"uberName":"H5P.Blanks 1.4","name":"H5P.Blanks","majorVersion":"1","minorVersion":"4","tutorialUrl":null,"title":"Fill in the Blanks","runnable":"1","restricted":false},{"uberName":"H5P.MarkTheWords 1.5","name":"H5P.MarkTheWords","majorVersion":"1","minorVersion":"5","tutorialUrl":null,"title":"Mark the Words","runnable":"1","restricted":false},{"uberName":"H5P.DragText 1.4","name":"H5P.DragText","majorVersion":"1","minorVersion":"4","tutorialUrl":null,"title":"Drag Text","runnable":"1","restricted":false},{"uberName":"H5P.GoToQuestion 1.0","name":"H5P.GoToQuestion","majorVersion":"1","minorVersion":"0","tutorialUrl":null,"title":"Go To Question","runnable":"0","restricted":false}];
        that.createDragNBar(libraries);
        that.setInteractionTitles();
        that.startGuidedTour();
        that.IV.trigger('dnbEditorReady');
      //});

      // Add "Add bookmark" to bookmarks menu.
      $('<div/>', {
        'class': 'h5p-add-bookmark',
        html: t('addBookmark'),
        role: 'button',
        tabindex: 0,
        on: {
          click: function () {
            that.addBookmark();
          }
        },
        appendTo: that.IV.controls.$bookmarksChooser
      });
    });
    this.IV.on('bookmarkAdded', that.bookmarkAdded, that);
    this.IV.attach(this.$editor);

    // Create a focus handler
    this.$focusHandler = $('<div>', {
      'class': 'h5peditor-iv-focus-handler'
    }).click(function () {
      if (!that.dnb.focusedElement || !that.dnb.focusedElement.$element.is(':focus')) {

        // No focused element, remove overlay
        that.$focusHandler.removeClass('show');
      }
    }).appendTo(this.IV.$videoWrapper);

    this.pToEm = (this.IV.width / this.IV.fontSize) / 100;
  };

  /**
   * Set custom interaction titles when libraries are registered.
   */
  InteractiveVideoEditor.prototype.setInteractionTitles = function () {
    var self = this;

    this.IV.interactions.forEach(function (interaction) {
      // Try to figure out a title for the dialog
      var title = self.findLibraryTitle(interaction.getLibraryName());
      if (!title) {
        // Couldn't find anything, use default
        title = self.IV.l10n.interaction;
      }

      interaction.setTitle(title);
    });

    // Create title element
    this.$interactionTitle = $('<div>', {
      'class': 'h5p-interaction-button-title'
    }).appendTo(this.$editor);

  };

  InteractiveVideoEditor.prototype.showInteractionTitle = function (title, $interaction) {
    if (!this.$interactionTitle) {
      return;
    }

    // Set static margin
    var fontSize = parseInt(this.IV.$videoWrapper.css('font-size'), 10);
    var staticMargin = 0.3 * fontSize;

    var videoOffsetX = $interaction.position().left;
    var videoOffsetY = $interaction.position().top;
    var dnbOffsetY = this.$bar.height();

    this.$interactionTitle.html(window.parent.Drupal.t(title));

    // center title
    var totalOffsetX = videoOffsetX - (this.$interactionTitle.outerWidth(true) / 2) + ($interaction.width() / 2);
    if (totalOffsetX < 0) {
      totalOffsetX = 0;
    } else if(totalOffsetX + this.$interactionTitle.outerWidth(true) > this.IV.$videoWrapper.width()) {
      totalOffsetX = this.IV.$videoWrapper.width() - this.$interactionTitle.outerWidth(true);
    }
    var totalOffsetY = videoOffsetY + dnbOffsetY - this.$interactionTitle.height() - 1;

    this.$interactionTitle.css({
      'left': totalOffsetX,
      'top': totalOffsetY - staticMargin
    }).addClass('show');
  };

  InteractiveVideoEditor.prototype.hideInteractionTitle = function () {
    if (!this.$interactionTitle) {
      return;
    }

    this.$interactionTitle.removeClass('show');
  };

  /**
   * Add bookmark
   */
  InteractiveVideoEditor.prototype.addBookmark = function () {
    var time = this.IV.video.getCurrentTime();

    // Find out where to place the bookmark
    for (var i = 0; i < this.params.bookmarks.length; i++) {
      if (this.params.bookmarks[i].time > time) {
        // Insert before this.
        break;
      }
    }

    var tenth = Math.floor(time * 10) / 10;
    if (this.IV.bookmarksMap[tenth] !== undefined) {
      // Create warning:
      this.displayMessage(t('bookmarkAlreadyExists'));
      return; // Not space for another bookmark.
    }

    // Hide dialog
    if (this.IV.controls.$more.hasClass('h5p-active')) {
      this.IV.controls.$more.click();
    }
    else {
      this.IV.controls.$bookmarks.click();
    }

    // Move other increament other ids.
    this.IV.trigger('bookmarksChanged', {'index': i, 'number': 1});

    this.params.bookmarks.splice(i, 0, {
      time: time,
      label: t('newBookmark')
    });

    var $bookmark = this.IV.addBookmark(i, tenth);
    $bookmark.addClass('h5p-show');
    $bookmark.find('.h5p-bookmark-text').click();
  };

  /**
   * Display a popup containing a message.
   *
   * @param {string} message
   */
  InteractiveVideoEditor.prototype.displayMessage = function (message) {
    var timeout;
    var $warning = $('<div/>', {
      'class': 'h5p-iv-message-popup',
      text: message,
      click: function () {
        clearTimeout(timeout);
        $warning.remove();
      }
    }).appendTo(this.$editor);

    timeout = setTimeout(function(){
      $warning.remove();
    }, 3000);
  };

  /**
   * Gets called whenever a bookmark is added to the UI.
   *
   * @param {H5P.Event} event
   */
  InteractiveVideoEditor.prototype.bookmarkAdded = function (event) {
    var self = this;
    var $bookmark = event.data.bookmark;

    $('<a class="h5p-remove-bookmark" href="#"></a>')
      .appendTo($bookmark.find('.h5p-bookmark-label'))
      .click(function () {
        var id = $bookmark.data('id');
        self.params.bookmarks.splice(id, 1);
        self.IV.trigger('bookmarksChanged', {'index': id, 'number': -1});
        $bookmark.remove();
        return false;
      });

    // Click to edit label.
    $bookmark.find('.h5p-bookmark-text').click(function () {
      if ($bookmark.hasClass('h5p-force-show')) {
        return; // Double click
      }
      $bookmark.addClass('h5p-force-show');
      var $text = $(this);

      // This is a IE-fix. Without this, text is not shown when editing
      $text.css({overflow: 'visible'});

      var $input = $text.html('<input type="text" class="h5p-bookmark-input" style="width:' + ($text.width() - 19) + 'px" maxlength="255" value="' + $text.text() + '"/>')
        .children()
        .blur(function () {
          var newText = $input.val();
          if (H5P.trim(newText) === '') {
            newText = t('newBookmark');
          }
          $text.text(newText);
          $bookmark.removeClass('h5p-force-show').mouseover().mouseout();
          $text.css({overflow: 'hidden'});

          var id = $bookmark.data('id');
          self.params.bookmarks[id].label = newText;
          self.IV.controls.$bookmarksChooser.find('li:eq(' + id + ')').text(newText);
        })
        .keydown(function (event) {
          if (event.which === 13) {
            $input.blur();
          }
        })
        .focus();

      if ($input.val() === t('newBookmark')) {
        // Delete default value when editing
        $input.val('');
      }
    });
  };

  /**
   * Initialize the toolbar for creating interactivties.
   *
   * @param {Array} libraries
   */
  InteractiveVideoEditor.prototype.createDragNBar = function (libraries) {
    var that = this;

    this.libraries = libraries;
    this.dnb = new H5P.DragNBar(this.getButtons(libraries), this.IV.$videoWrapper, this.IV.$container);

    /**
     * @private
     * @param {string} lib uber name
     * @returns {boolean}
     */
    var supported = function (lib) {
      for (var i = 0; i < libraries.length; i++) {
        if (libraries[i].restricted !== true && libraries[i].uberName === lib) {
          return true; // Library is supported and allowed
        }
      }

      return false;
    };

    this.dnb.on('paste', function (event) {
      var pasted = event.data;
      var options = {
        width: pasted.width,
        height: pasted.height,
        pasted: true
      };

      if (pasted.from === InteractiveVideoEditor.clipboardKey) {
        // Pasted content comes from the same version of IV

        if (!pasted.generic) {
          // Non generic part, must be a something not created yet
          that.dnb.focus(that.addInteraction(pasted.specific, options));
        }
        else if (supported(pasted.generic.library)) {
          // Has generic part and the generic libray is supported
          that.dnb.focus(that.addInteraction(pasted.specific, options));
        }
        else {
          alert(H5PEditor.t('H5P.DragNBar', 'unableToPaste'));
        }
      }
      else if (pasted.generic) {
        if (supported(pasted.generic.library)) {
          // Supported library from another content type

          if (pasted.specific.displayAsButton) {
            // Make sure buttons from CP still are buttons.
            options.displayType = 'button';
          }
          options.action = pasted.generic;
          that.dnb.focus(that.addInteraction(pasted.generic.library, options));
        }
        else {
          alert(H5PEditor.t('H5P.DragNBar', 'unableToPaste'));
        }
      }
    });

    that.dnb.dnr.on('stoppedResizing', function (event) {
      // Set size in em
      that.interaction.setSize(event.data.width, event.data.height);

      // Set pos in %
      var containerStyle = window.getComputedStyle(that.dnb.$container[0]);
      that.interaction.setPosition(event.data.left / (parseFloat(containerStyle.width) / 100), event.data.top / (parseFloat(containerStyle.height) / 100));
      that.interaction.setPositionPX(event.data.left / (parseFloat(containerStyle.width) / 100), event.data.top / (parseFloat(containerStyle.height) / 100));
    });

    this.dnb.dnd.startMovingCallback = function () {
      that.dnb.dnd.min = {x: 0, y: 0};
      that.dnb.dnd.max = {
        x: that.dnb.$container.width() - that.dnb.$element.outerWidth(),
        y: that.dnb.$container.height() - that.dnb.$element.outerHeight()
      };

      if (that.dnb.newElement) {
        that.dnb.dnd.adjust.x = 10;
        that.dnb.dnd.adjust.y = 10;
        that.dnb.dnd.min.y -= that.dnb.$list.height();
      }

      return true;
    };

    // Update params when the element is dropped.
    this.dnb.stopMovingCallback = function (x, y) {
      that.interaction.positionLabel(that.IV.$videoWrapper.width());
      that.interaction.setPosition(x, y);
      //h5pcustomize
      that.interaction.setPositionPX(x, y);
	 
      
    };

    this.dnb.dnd.releaseCallback = function () {
      // Edit element when it is dropped.
      if (that.dnb.newElement) {
        that.dnb.dnd.$element.dblclick();
        that.dnb.blurAll();
      }
    };
    that.IV.interactions.forEach(function (interaction) {
      // Add drag functionality if interaction has element
      if (interaction.getElement()) {
        var libraryName = interaction.getLibraryName();
        var options = {
          lock: (libraryName === 'H5P.Image'),
          disableResize: (libraryName === 'H5P.Link') || interaction.isButton()
        };
        that.addInteractionToDnb(interaction, interaction.getElement(), options);
      }
    });

    if (that.IV.scaledFontSize) {
      // Set the container em since the resizing is useless without it
      that.dnb.dnr.setContainerEm(that.IV.scaledFontSize);
    }

    this.dnb.attach(this.$bar);
  };

  /**
   * Create form for interaction.
   *
   * @param {H5P.InteractiveVideoInteraction} interaction
   * @param {Object} parameters
   */
  InteractiveVideoEditor.prototype.createInteractionForm = function (interaction, parameters) {
    var self = this;

    var $semanticFields = $('<div>');

    // Create form
    interaction.$form = $semanticFields;
    var interactions = findField('interactions', this.field.fields);

    // Clone semantics to avoid changing them for all interactions
    var interactionFields = H5PEditor.$.extend(true, [], interactions.field.fields);

    // Hide some fields for some interaction types
    var type = interaction.getLibraryName();
    var xAPIQuestionTypes = [
      'H5P.MultiChoice',
      'H5P.SingleChoiceSet',
      'H5P.Blanks',
      'H5P.DragQuestion',
      'H5P.Summary',
      'H5P.MarkTheWords',
      'H5P.DragText'
    ];
    if (xAPIQuestionTypes.indexOf(type) === -1) {
      hideFields(interactionFields, ['adaptivity']);
    }
    if (type === 'H5P.Nil') {
      hideFields(interactionFields, ['displayType']);
    }

    // Always show link as poster
    if (type === 'H5P.Link' || type === 'H5P.GoToQuestion') {
      var field = findField('displayType', interactionFields);
      // Must set default to false and hide
      field.default = 'poster';
      field.widget = 'none';
      

      // Hide label field
      var labelField = findField('label', interactionFields);
      labelField.widget = 'none';

      if (type === 'H5P.GoToQuestion') {
    	      //84231 - commented below.
        //parameters.pause = true;
      }
      
    }

    // Get indexes of fields that needs unique styling
    if (interaction.indexes === undefined) {
      interaction.indexes = {};
    }
    interaction.indexes.durationIndex = {name: 'duration', index: interactionFields.indexOf(findField('duration', interactionFields))};
    interaction.indexes.pauseIndex = {name: 'pause', index: interactionFields.indexOf(findField('pause', interactionFields))};
    interaction.indexes.labelIndex = {name: 'label', index: interactionFields.indexOf(findField('label', interactionFields))};
    H5PEditor.processSemanticsChunk(interactionFields, parameters, $semanticFields, self);

    self.setLibraryName(interaction.$form, type);
  };

  /**
   * Process interaction.
   *
   * @param {H5P.InteractiveVideoInteraction} interaction
   * @param {Object} parameters
   */
  InteractiveVideoEditor.prototype.processInteraction = function (interaction, parameters) {
    var self = this;
    var type = interaction.getLibraryName();
    this.createInteractionForm(interaction, parameters);

    // Keep track of form elements
    interaction.children = this.children;
    this.children = undefined;

    // Add classes to form elements if they exist
    if (interaction.children[interaction.indexes.durationIndex.index].$item) {
      interaction.children[interaction.indexes.durationIndex.index].$item.addClass('h5peditor-interaction-' + interaction.indexes.durationIndex.name);
    }

    if (interaction.children[interaction.indexes.pauseIndex.index].$item) {
      interaction.children[interaction.indexes.pauseIndex.index].$item.addClass('h5peditor-interaction-' + interaction.indexes.pauseIndex.name);
    }

    if (interaction.children[interaction.indexes.labelIndex.index].$item) {
      interaction.children[interaction.indexes.labelIndex.index].$item.addClass('h5peditor-interaction-' + interaction.indexes.labelIndex.name);

      // Remove label when displayType is poster
      var $displayTypeRadios = $('.h5p-image-radio-button-group input:radio', interaction.$form);
      var $labelWrapper = interaction.children[interaction.indexes.labelIndex.index].$item;
      $displayTypeRadios.change(function () {
        $labelWrapper.toggleClass('hide', !interaction.isButton());
        if (!interaction.isButton() && interaction.children[interaction.indexes.pauseIndex.index].$item) {
          interaction.children[interaction.indexes.pauseIndex.index].$input[0].checked = true;
          interaction.children[interaction.indexes.pauseIndex.index].$input.trigger('change');
        }
      });

      $labelWrapper.toggleClass('hide', !interaction.isButton());
    }

    interaction.on('display', function (event) {
      var $interaction = event.data;
      // Customize rendering of interaction
      self.newInteraction(interaction, $interaction);
    });

    // Find library field instance
    var libraryFieldInstance;
    for (var i = 0; i < interaction.children.length; i++) {
      if (interaction.children[i] instanceof H5PEditor.Library) {
        libraryFieldInstance = interaction.children[i];
      }
    }

    if (libraryFieldInstance) {
      /**
       * Callback for when library changes.
       *
       * @private
       * @param {String} library
       */
      var libraryChange = function () {
        var lib = libraryFieldInstance.currentLibrary.split(' ')[0];
        if (lib !== 'H5P.Image') {
          return;
        }

        /**
         * Callback for when image changes.
         *
         * @private
         * @param {Object} params
         */
        var imageChange = function (newParams) {
          if (newParams !== undefined && newParams.width !== undefined && newParams.height !== undefined) {
            self.setImageSize(parameters, newParams);
          }
        };

        // Add callback to the correct field
        libraryFieldInstance.forEachChild(function (child) {
          if (child.field.name === 'file') {
            child.changes.push(imageChange);
            return true;
          }
        });
      };

      // Add callback
      libraryFieldInstance.changes.push(libraryChange);
      if (libraryFieldInstance.children !== undefined) {
        // Trigger right away
        libraryChange();
      }
    }

    if (parameters.pasted) {
      if (type === 'H5P.Image' && parameters.action.params.file !== undefined) {
        self.setImageSize(parameters, parameters.action.params.file);
      }
      delete parameters.pasted;
    }
  };

  /**
   * Help set size for new images and keep aspect ratio.
   *
   * @param {object} parameters
   * @param {object} newParams
   */
  InteractiveVideoEditor.prototype.setImageSize = function (parameters, newParams) {
    if (newParams === undefined || newParams.width === undefined || newParams.height === undefined) {
      return;
    }
    var self = this;

    // Avoid to small images
    var fontSize = Number(self.IV.$videoWrapper.css('fontSize').replace('px', ''));
    if (newParams.width < fontSize) {
      newParams.width = fontSize;
    }
    if (newParams.height < fontSize) {
      newParams.height = fontSize;
    }

    // Reduce height for tiny images, stretched pixels looks horrible
    var suggestedHeight = newParams.height / fontSize;
    if (suggestedHeight < parameters.height) {
      parameters.height = suggestedHeight;
    }

    // Calculate new width
    parameters.width = (parameters.height * (newParams.width / newParams.height));
  };

  /**
   * Add library name to library form.
   *
   * @param {H5P.jQuery} $form
   *   Interaction view form
   * @param {string} libraryType
   *   Library type, e.g. H5P.Blanks
   */
  InteractiveVideoEditor.prototype.setLibraryName = function ($form, libraryType) {
    var libraryName = libraryType.replace('.', '-').toLowerCase() + '-library';
    var $libraryForm = $form.children('.library');
    $libraryForm.addClass(libraryName);
  };

  /**
   *
   * @param interaction
   */
  InteractiveVideoEditor.prototype.openInteractionDialog = function (interaction,isEdit) {
    var that = this;
    //h5pcustomize remove empty interactions when navigating in the toolbar
    $(".h5p-remove").click();
    
    var currentInteractionIdx = 0;
    for (var i = 0; i < that.IV.interactions.length; i++) {
        if (that.IV.interactions[i] === interaction) {
          currentInteractionIdx = i;
          break;
          }
    }
        
    if (that.lastState !== H5P.Video.PAUSED && that.lastState !== H5P.Video.ENDED) {
      // Pause video
      that.IV.video.pause();
    }

    // Try to figure out a title for the dialog
    var title = interaction.getTitle();
    if (title === that.IV.l10n.interaction) {
      // Try to find something better than thhe default title
      title = that.findLibraryTitle(interaction.getLibraryName());
      if (!title) {
        // Couldn't find anything, use default
        title = that.IV.l10n.interaction;
      }
    }
	
    // Add dialog buttons
    var $doneButton = $('<a href="#" class="h5p-button h5p-done">' + t('done') + '</a>')
      .click(function () {
      	var libraryName = interaction.getLibraryName();
		if(that.params.interactions.length > 0)
			window.parent.$(".h5p-editor-iframe").addClass("interactionsexists"); //to disable delete icon in the video
			//window.parent.$(".edit-edit-btn-prerequest").addClass("interactionsexists"); //to disable delete icon in the video
      	if(libraryName == "H5P.Nil")
      	{
      		 var errormsges = "";
      		 var displayType = "";
			 var interactionsCurrent =   that.params.interactions[currentInteractionIdx];  		
          	var label = ns.$("#labelid").val();
          	var url = ns.$("#urlid").val();
      		var from = ns.$("#durationfrom").val();
			var to = ns.$("#durationto").val();
			errormsges = durationValidateH5P("from",from);
			errormsges += durationValidateH5P("to",to);
			errormsges += validateH5PTextField("Label",label);
			from = parseInt(convertToSeconds(from),10);
			to = parseInt(convertToSeconds(to),10);

			
			if(from > to)
			{
				//errormsges += geth5pexpertusmsg("from"," value should be less than of to value");
				errormsges += geth5pexpertusmsg(window.parent.Drupal.t('LBL649'),window.parent.Drupal.t('MSG904'));
			}


			if(errormsges != "" && errormsges != undefined && errormsges != "undefined"){
				displayh5pexpertusmsg(errormsges);
				return false;
			}
			
			
			if(ns.$("#h5p-image-radio-button-0").is(":checked"))
				displayType = ns.$("#h5p-image-radio-button-0").val();
			if(ns.$("#h5p-image-radio-button-1").is(":checked"))
				displayType = ns.$("#h5p-image-radio-button-1").val();
			
			var pauseVideo = false;
			if(ns.$("#pausevideo:checked").length >0)
				pauseVideo =  true;
			
			interactionsCurrent.duration.from = from;
			interactionsCurrent.duration.to = to;
			interactionsCurrent.displayType = displayType;
			interactionsCurrent.label = label;
			interactionsCurrent.pause = pauseVideo;
			
			saveInteractionCommonAPI(that,interactionsCurrent,interaction);

			
      	}
      	else  if(libraryName == "H5P.Link")
      	{
      		 var errormsges = "";
			 var interactionsCurrent =   that.params.interactions[currentInteractionIdx];  		
      		 var displayType = "";

          	var title = ns.$("#titleid").val();
          	var url = ns.$("#urlid").val();
      		var from = ns.$("#durationfrom").val();
			var to = ns.$("#durationto").val();
			
			errormsges = durationValidateH5P("from",from);
			errormsges += durationValidateH5P("to",to);
			errormsges += urlH5PValidate("Url",url);
			errormsges += validateH5PTextField("Label",title);

			from = parseInt(convertToSeconds(from),10);
			to = parseInt(convertToSeconds(to),10);

			if(from > to)
			{
				errormsges += geth5pexpertusmsg("from"," value should be less than of to value");
			}


			if(errormsges != "" && errormsges != undefined && errormsges != "undefined"){
				displayh5pexpertusmsg(errormsges);
				return false;
			}
			
			
			if(ns.$("#h5p-image-radio-button-0").is(":checked"))
				displayType = ns.$("#h5p-image-radio-button-0").val();
			if(ns.$("#h5p-image-radio-button-1").is(":checked"))
				displayType = ns.$("#h5p-image-radio-button-1").val();
	
			var pauseVideo = false;
			if(ns.$("#pausevideo:checked").length >0)
				pauseVideo =  true;
			
			interactionsCurrent.duration.from = from;
			interactionsCurrent.duration.to = to;
			interactionsCurrent.displayType = "poster";
			//interactionsCurrent.label = label;
			interactionsCurrent.pause = pauseVideo;
			
			
			//interactionsCurrent.action.params.linkWidget.protocol = "";
			
			/*if(url.indexOf("http://") >= 0)
				interactionsCurrent.action.params.linkWidget.protocol = "http://";
			else if(url.indexOf("https://") >= 0)
				interactionsCurrent.action.params.linkWidget.protocol = "https://";
			else
				interactionsCurrent.action.params.linkWidget.protocol = "http://";
			*/
			
			interactionsCurrent.action.params.linkWidget.url = url;
			
			interactionsCurrent.action.params.title = title;
		
			saveInteractionCommonAPI(that,interactionsCurrent,interaction);

			
      	}
      	else if(libraryName == "H5P.Text")
      	{
      		var errormsges = "";
      		var displayType = "";
      		
			 var interactionsCurrent =   that.params.interactions[currentInteractionIdx];  		
          	var label = ns.$("#labelid").val();
          	//var editorData = CKEDITOR.instances.formattextbox.getData();
          	var editorData = tinyMCE.get('edit-short-description-value').getContent();
      		var from = ns.$("#durationfrom").val();
			var to = ns.$("#durationto").val();
			
			
			errormsges = durationValidateH5P("from",from);
			errormsges += durationValidateH5P("to",to);
			errormsges += validateH5PTextField("Text",editorData);

			from = parseInt(convertToSeconds(from),10);
			to = parseInt(convertToSeconds(to),10);
			
			if(from > to)
			{
				errormsges += geth5pexpertusmsg("from"," value should be less than of to value");
			}


			if(errormsges != "" && errormsges != undefined && errormsges != "undefined"){
				displayh5pexpertusmsg(errormsges);
				return false;
			}
			
			
			if(ns.$("#h5p-image-radio-button-0").is(":checked"))
				displayType = ns.$("#h5p-image-radio-button-0").val();
			if(ns.$("#h5p-image-radio-button-1").is(":checked"))
				displayType = ns.$("#h5p-image-radio-button-1").val();
	
			var pauseVideo = false;
			
			if(ns.$("#pausevideo").is(":checked"))
				pauseVideo =  true;
			interactionsCurrent.action.params.text = editorData;
			
			interactionsCurrent.duration.from = from;
			interactionsCurrent.duration.to = to;
			interactionsCurrent.displayType = displayType;
			interactionsCurrent.label = label;
			interactionsCurrent.pause = pauseVideo;
			
			
			saveInteractionCommonAPI(that,interactionsCurrent,interaction);
			

          	
      	}
      	else if(libraryName == "H5P.Table")
      	{
      		var errormsges = "";
      		var displayType = "";
      		
			 var interactionsCurrent =   that.params.interactions[currentInteractionIdx];  		
          	var label = ns.$("#labelid").val();
          	var editorData = CKEDITOR.instances.formattextbox.getData();
      		var from = ns.$("#durationfrom").val();
			var to = ns.$("#durationto").val();
			
			errormsges = durationValidateH5P("from",from);
			errormsges += durationValidateH5P("to",to);
			errormsges += validateH5PTextField("Text",editorData);

			from = parseInt(convertToSeconds(from),10);
			to = parseInt(convertToSeconds(to),10);
			
			if(from > to)
			{
				errormsges += geth5pexpertusmsg("from"," value should be less than of to value");
			}


			if(errormsges != "" && errormsges != undefined && errormsges != "undefined"){
				displayh5pexpertusmsg(errormsges);
				return false;
			}
			
			
			if(ns.$("#h5p-image-radio-button-0").is(":checked"))
				displayType = ns.$("#h5p-image-radio-button-0").val();
			if(ns.$("#h5p-image-radio-button-1").is(":checked"))
				displayType = ns.$("#h5p-image-radio-button-1").val();
	
			var pauseVideo = false;
			
			if(ns.$("#pausevideo").is(":checked"))
				pauseVideo =  true;
			
			interactionsCurrent.duration.from = from;
			interactionsCurrent.duration.to = to;
			interactionsCurrent.displayType = displayType;
			interactionsCurrent.label = label;
			interactionsCurrent.pause = pauseVideo;
			
			interactionsCurrent.action.params.text = editorData;
			saveInteractionCommonAPI(that,interactionsCurrent,interaction);

          	
      	}
      	else  if(libraryName == "H5P.Image")
      	{
      		
      		var errormsges = "";
      		var displayType = "";
      		
			var interactionsCurrent =   that.params.interactions[currentInteractionIdx];
			  	
          	var label = ns.$("#labelid").val();
          	//var editorData = CKEDITOR.instances.formattextbox.getData();
			var img = ns.$("#uploadImg").attr("filename");
			
		
		    if(img != undefined && img != "")
		     {
			  var extension =  img.split('.').pop();
			  if(extension != "jpg" && extension != "JPG" && extension != "png" && extension != "PNG" && extension != "gif" && extension != "GIF"){
				  displayh5pexpertusmsg("Upload jpg, png or gif files only");
			      return "";  
			    }
		     }
			
			
			
			
			var processedRes = ns.$("#uploadImg").attr("processedresponse");
			if(processedRes != null)
				processedRes = JSON.parse(processedRes);
			
		//	var altText = ns.$("#alterid").val();
			var titleText = ns.$("#titleid").val();
			
      		var from = ns.$("#durationfrom").val();
			var to = ns.$("#durationto").val();
			
			errormsges = durationValidateH5P("from",from);
			errormsges += durationValidateH5P("to",to);
			errormsges += validateH5PTextField("Image",img);
			//errormsges += validateH5PTextField("Text",editorData);

			from = parseInt(convertToSeconds(from),10);
			to = parseInt(convertToSeconds(to),10);
			
			if(from > to)
			{
				errormsges += geth5pexpertusmsg("from"," value should be less than of to value");
			}


			if(errormsges != "" && errormsges != undefined && errormsges != "undefined"){
				displayh5pexpertusmsg(errormsges);
				return false;
			}
			
			
			if(ns.$("#h5p-image-radio-button-0").is(":checked"))
				displayType = ns.$("#h5p-image-radio-button-0").val();
			if(ns.$("#h5p-image-radio-button-1").is(":checked"))
				displayType = ns.$("#h5p-image-radio-button-1").val();
	
			var pauseVideo = false;
			
			if(ns.$("#pausevideo").is(":checked"))
				pauseVideo =  true;
			//{"x":31.746031746031747,"y":0,"width":15.996401259559153,"height":10,"duration":{"from":0,"to":10},"action":{"library":"H5P.Image 1.0","params":{"contentName":"Image","file":{"path":"image.jpg^^^images/file-5837fc8a884de.jpg","mime":"image/jpeg","copyright":{"license":"U"},"width":3556,"height":2223},"alt":"L1","title":"L1"},"subContentId":"b0d52cf6-0211-4a4f-80f9-eeb398f31036"},"pause":false,"displayType":"button","label":"L1"}
			
			interactionsCurrent.duration.from = from;
			interactionsCurrent.duration.to = to;
			interactionsCurrent.displayType = displayType;
			interactionsCurrent.label = label;
			//interactionsCurrent.action.params.alt = altText;
			interactionsCurrent.action.params.title = titleText;
			interactionsCurrent.action.params.contentName = "Image";
			var jsono = {"mime":"","height":"","width":""};
			if(processedRes != undefined && processedRes.mime != undefined && processedRes.mime != null)
			{ 
				interactionsCurrent.action.params.file=jsono;
				interactionsCurrent.action.params.file.path = img;
				interactionsCurrent.action.params.file.mime = processedRes.mime;
				interactionsCurrent.action.params.file.height=processedRes.height;
				interactionsCurrent.action.params.file.width=processedRes.width;
			}
			interactionsCurrent.pause = pauseVideo;
			//interactionsCurrent.action.params.text = editorData;
			saveInteractionCommonAPI(that,interactionsCurrent,interaction);	
      	}
      	else if(libraryName == "H5P.MarkTheWords")
      	{
      		var errormsges = "";
      		var displayType = "";
      		
			var interactionsCurrent =   that.params.interactions[currentInteractionIdx];  		
          	var label = ns.$("#labelid").val();
          	var taskDescription = ns.$("#taskDescription").val();
          	var editorData = ns.$("#formattextbox").val();
          	
      		var from = ns.$("#durationfrom").val();
			var to = ns.$("#durationto").val();
			
			errormsges = durationValidateH5P("from",from);
			errormsges += durationValidateH5P("to",to);
			errormsges += validateH5PTextField("Question",taskDescription);
			errormsges += validateH5PTextField("Text",editorData);
			
			checkstar_return = checkstar(editorData);
			if((checkstar_return != "" && checkstar_return != undefined && checkstar_return != "undefined"))
				errormsges+=checkstar_return;
			from = parseInt(convertToSeconds(from),10);
			to = parseInt(convertToSeconds(to),10);
			
			if(from > to)
			{
				errormsges += geth5pexpertusmsg("from"," value should be less than of to value");
			}


			if(errormsges != "" && errormsges != undefined && errormsges != "undefined"){
				displayh5pexpertusmsg(errormsges);
				return false;
			}
			
			
			if(ns.$("#h5p-image-radio-button-0").is(":checked"))
				displayType = ns.$("#h5p-image-radio-button-0").val();
			if(ns.$("#h5p-image-radio-button-1").is(":checked"))
				displayType = ns.$("#h5p-image-radio-button-1").val();
	
			var pauseVideo = false;
			
			if(ns.$("#pausevideo").is(":checked"))
				pauseVideo =  true;
			
			interactionsCurrent.duration.from = from;
			interactionsCurrent.duration.to = to;
			interactionsCurrent.displayType = displayType;
			interactionsCurrent.label = label;
			interactionsCurrent.pause = pauseVideo;
			interactionsCurrent.action.params.taskDescription = taskDescription;
			interactionsCurrent.action.params.textField = editorData;
			//interactionsCurrent.action.params.text = editorData;
			saveInteractionCommonAPI(that,interactionsCurrent,interaction);
          	
      	}
    	else if(libraryName == "H5P.DragText")
      	{
      		var errormsges = "";
      		var displayType = "";
      		
			var interactionsCurrent =   that.params.interactions[currentInteractionIdx];  		
          	var label = ns.$("#labelid").val();
          //	var textField = ns.$("#textField").val();
          	var taskDescription = ns.$("#taskDescription").val();
          //	var editorData = CKEDITOR.instances.formattextbox.getData();
          	var editorData = ns.$("#formattextbox").val();
          	
      		var from = ns.$("#durationfrom").val();
			var to = ns.$("#durationto").val();
			
			errormsges = durationValidateH5P("from",from);
			errormsges += durationValidateH5P("to",to);
			errormsges += validateH5PTextField("Question",taskDescription);
			errormsges += validateH5PTextField("Text",editorData);
			checkstar_return = checkstar(editorData);
			if((checkstar_return != "" && checkstar_return != undefined && checkstar_return != "undefined"))
				errormsges+=checkstar_return;
			
			

			from = parseInt(convertToSeconds(from),10);
			to = parseInt(convertToSeconds(to),10);
			
			if(from > to)
			{
				errormsges += geth5pexpertusmsg("from"," value should be less than of to value");
			}


			if(errormsges != "" && errormsges != undefined && errormsges != "undefined"){
				displayh5pexpertusmsg(errormsges);
				return false;
			}
			
			
			if(ns.$("#h5p-image-radio-button-0").is(":checked"))
				displayType = ns.$("#h5p-image-radio-button-0").val();
			if(ns.$("#h5p-image-radio-button-1").is(":checked"))
				displayType = ns.$("#h5p-image-radio-button-1").val();
	
			var pauseVideo = false;
			
			if(ns.$("#pausevideo").is(":checked"))
				pauseVideo =  true;
			
			interactionsCurrent.duration.from = from;
			interactionsCurrent.duration.to = to;
			interactionsCurrent.displayType = displayType;
			interactionsCurrent.label = label;
			interactionsCurrent.pause = pauseVideo;
			interactionsCurrent.action.params.taskDescription = taskDescription;
			interactionsCurrent.action.params.textField = editorData;
			//interactionsCurrent.action.params.text = editorData;
			saveInteractionCommonAPI(that,interactionsCurrent,interaction);
          	
      	}
      	else if(libraryName == "H5P.SingleChoiceSet")
      	{

	      	var errormsges = "";
      		var displayType = "";
      		var error="";
			var interactionsCurrent =   that.params.interactions[currentInteractionIdx];  		
          	var label = stripTags(ns.$("#singlechoicesetlabel").html());
      		var from = ns.$("#durationfrom").val();
			var to = ns.$("#durationto").val();
			
			errormsges = durationValidateH5P("from",from);
			errormsges += durationValidateH5P("to",to);
	
			from = parseInt(convertToSeconds(from),10);
			to = parseInt(convertToSeconds(to),10);
			
			if(from > to)
			{
				errormsges += geth5pexpertusmsg("from"," value should be less than of to value");
			}


			if(errormsges != "" && errormsges != undefined && errormsges != "undefined"){
				displayh5pexpertusmsg(errormsges);
				return false;
			}
			
			
			if(ns.$("#h5p-image-radio-button-0").is(":checked"))
				displayType = ns.$("#h5p-image-radio-button-0").val();
			if(ns.$("#h5p-image-radio-button-1").is(":checked"))
				displayType = ns.$("#h5p-image-radio-button-1").val();
	
			var pauseVideo = false;
			
			if(ns.$("#pausevideo").is(":checked"))
				pauseVideo =  true;
			
			
			var qCount = ns.$(".questionset").size();
			var qarr = new Array();
			var count = 0; 
			ns.$.each(ns.$(".questionset"),function(i,val){
				var questionset =ns.$(this).attr("qset");
				//var question = ns.$("."+questionset).find(".ckeditor").html();
				
				
				var question = stripTags(ns.$("."+questionset).find(".ckeditor").html());
			//	var question = ns.$("."+questionset).find(".ckeditor").val();
				
				
				if(interactionsCurrent.action.params.choices[count] == undefined || interactionsCurrent.action.params.choices[count].question == undefined)
				{
					var json = {"question":question,"answers":[]};
					interactionsCurrent.action.params.choices[count] = json;
				}
				else
				{	
					interactionsCurrent.action.params.choices[count].question = question;
				}
				var answers  = new Array();
				
		 		ns.$.each(ns.$("."+questionset+" .answercontainerwrapper .ckeditor"),function(i,val){
			 	//	answers[i]=ns.$(this).html();
		 			answers[i]=stripTags(answers[i]=ns.$(this).html());
			 		 if((answers[i] == "" || answers[i] == undefined) && (errormsges == "" || errormsges == undefined || errormsges == "undefined"))
	      				{
							errormsges += geth5pexpertusmsg(window.parent.Drupal.t('LBL387'),window.parent.Drupal.t('ERR261'));  
	      				} 
	                 else if(answers[i] == "" || answers[i] == undefined){
	                        errormsges = geth5pexpertusmsg(window.parent.Drupal.t('LBL3097'),window.parent.Drupal.t('ERR262')); 
	                     }	
			 	
			 		
			 		
		 		});
		 		if(errormsges.indexOf("Question")<0){
		 			error=validateH5PTextField("Question",question);
		 			error+=errormsges;
		 		}
		 		  // errormsges += 
		 		interactionsCurrent.action.params.choices[count].answers = answers;
		 		count++;
		 		
			});
			interactionsCurrent.action.params.behaviour={"timeoutCorrect":50,"timeoutWrong":75,"soundEffectsEnabled":true,"enableRetry":false,"enableSolutionsButton":false};
			interactionsCurrent.duration.from = from;
			interactionsCurrent.duration.to = to;
			interactionsCurrent.displayType = displayType;
			interactionsCurrent.label = label;
			interactionsCurrent.pause = pauseVideo;	
			interactionsCurrent.action.params.choices.length = ns.$(".questionset").size(); // Fix for #0086216
			
			
			
			if(error != "" && error != undefined && error != "undefined"){
				displayh5pexpertusmsg(error);
				return false;
			}
			
			saveInteractionCommonAPI(that,interactionsCurrent,interaction);			
			
			
			
      	}
      	else if(libraryName == "H5P.Blanks")
      	{
      		
      		var errormsges = "";
      		var displayType = "";
      		
			 var interactionsCurrent =   that.params.interactions[currentInteractionIdx];  		
          	var label = ns.$("#labelid").val();
          	var text = ns.$("#questionid").val();
      		var from = ns.$("#durationfrom").val();
			var to = ns.$("#durationto").val();
			/*		
			errormsges = durationValidateH5P("from",from);
			errormsges += durationValidateH5P("to",to);
			errormsges += validateH5PTextField("Question",text);

			from = parseInt(convertToSeconds(from),10);
			to = parseInt(convertToSeconds(to),10);
			
			if(from > to)
			{
				errormsges += geth5pexpertusmsg("from"," value should be less than of to value");
			}


			if(errormsges != "" && errormsges != undefined && errormsges != "undefined"){
				displayh5pexpertusmsg(errormsges);
				return false;
			} */
			
			
			if(ns.$("#h5p-image-radio-button-0").is(":checked"))
				displayType = ns.$("#h5p-image-radio-button-0").val();
			if(ns.$("#h5p-image-radio-button-1").is(":checked"))
				displayType = ns.$("#h5p-image-radio-button-1").val();
	
			var pauseVideo = false;
			
			if(ns.$("#pausevideo").is(":checked"))
				pauseVideo =  true;
			
		/*	interactionsCurrent.duration.from = from;
			interactionsCurrent.duration.to = to;
			interactionsCurrent.displayType = displayType;
			interactionsCurrent.label = label;
			interactionsCurrent.pause = pauseVideo; */
      		
      		var questions = [];
      		//errormsges = validateH5PTextField("Title",text);
      		ns.$(".statements_text").each(function(index,value){
				questions[index]= ns.$(this).val();
				checkstar_return = checkstar(questions[index]);
				if((checkstar_return != "" && checkstar_return != undefined && checkstar_return != "undefined"))
					errormsges+=checkstar_return;
				
				
                             if((questions[index] == "" || questions[index] == undefined ) && (errormsges == "" || errormsges == undefined || errormsges == "undefined"))
      				{
                            	 
						errormsges = geth5pexpertusmsg(window.parent.Drupal.t('LBL325'),window.parent.Drupal.t('ERR261'));
      				} 
                     else if(questions[index] == "" || questions[index] == undefined){
                    	 	errormsges = geth5pexpertusmsg(window.parent.Drupal.t('LBL240'),window.parent.Drupal.t('ERR262'));
                     }
                     else if(questions[index].indexOf("**")>=0)
                    	 {
                    	 	errormsges = geth5pexpertusmsg(window.parent.Drupal.t('LBL613'),'');
                    	 }

				
			});

      		
      		errormsges += durationValidateH5P("from",from);
			errormsges += durationValidateH5P("to",to);
			
			var error = validateH5PTextField("Title",text);
      		error +=errormsges;
      		if(error != "" && error != undefined && error != "undefined"){
				displayh5pexpertusmsg(error);
				return false;
			}
			from = parseInt(convertToSeconds(from),10);
			to = parseInt(convertToSeconds(to),10);
			
			if(from > to)
			{
				errormsges += geth5pexpertusmsg("from"," value should be less than of to value");
			}

      		
      		
      		
      		if(errormsges != "" && errormsges != undefined && errormsges != "undefined"){
				displayh5pexpertusmsg(errormsges);
				return false;
			}
      		
      		interactionsCurrent.duration.from = from;
			interactionsCurrent.duration.to = to;
			interactionsCurrent.displayType = displayType;
			interactionsCurrent.label = label;
			interactionsCurrent.pause = pauseVideo;
      		
      		interactionsCurrent.action.params.text = text;
      		interactionsCurrent.action.params.questions = questions;
			

			saveInteractionCommonAPI(that,interactionsCurrent,interaction);			
      		
      		
      		
      	}else if(libraryName == "H5P.GoToQuestion")
      	{
      	
      		var errormsges = "";
      		var displayType = "";
      		
			 var interactionsCurrent =   that.params.interactions[currentInteractionIdx];  		
      		var from = ns.$("#durationfrom").val();
			var to = ns.$("#durationto").val();
			var text = ns.$("#questionid").val();
			
			errormsges = durationValidateH5P("from",from);
			errormsges += durationValidateH5P("to",to);
			errormsges += validateH5PTextField("Question",text);

			from = parseInt(convertToSeconds(from),10);
			to = parseInt(convertToSeconds(to),10);
			
			
			if(from > to)
			{
				errormsges += geth5pexpertusmsg("from"," value should be less than of to value");
			}


		/*	if(errormsges != "" && errormsges != undefined && errormsges != "undefined"){
				displayh5pexpertusmsg(errormsges);
				return false;
			}*/
			
			
			
			if(ns.$("#h5p-image-radio-button-0").is(":checked"))
				displayType = ns.$("#h5p-image-radio-button-0").val();
			if(ns.$("#h5p-image-radio-button-1").is(":checked"))
				displayType = ns.$("#h5p-image-radio-button-1").val();
	
			var pauseVideo = false;
			
			if(ns.$("#pausevideo").is(":checked"))
				pauseVideo =  true;
			
			interactionsCurrent.duration.from = from;
			interactionsCurrent.duration.to = to;
			interactionsCurrent.displayType = displayType;
			interactionsCurrent.pause = pauseVideo;
      		
      		
      	
      
			
			var choices = [];
      		var errormsges1 = "";
      		ns.$(".choices_container .choice_text").each(function(index,value){
      			var textData = ns.$(this).val();
      		//	if(parseInt(ns.$(this).attr("choicecount"),10) <=  2) Breaks the functionality and does not allow more than two options.
      		//		{
      				
      			if((textData == "" || textData == undefined) && (errormsges1 == "" || errormsges1 == undefined || errormsges1 == "undefined"))
      			  {
						//errormsges += geth5pexpertusmsg("Option"+ns.$(this).attr("choicecount")," is mandatory.");LBL3097
      				errormsges1 = geth5pexpertusmsg(window.parent.Drupal.t('LBL387'),window.parent.Drupal.t('ERR261'));
      			  }
      			else if(textData == "" || textData == undefined){
      				errormsges1 = geth5pexpertusmsg(window.parent.Drupal.t('LBL3097'),window.parent.Drupal.t('ERR262'));
      			}
      				var goTo  = ns.$(this).parent().parent().find(".goto_text").val();
      				
      				if(goTo != "" && goTo != undefined)
      					errormsges1 += durationValidateH5P("goto"+ns.$(this).attr("choicecount"),goTo);
      				else if((errormsges1.indexOf("Go") < 0))
      					errormsges1 += geth5pexpertusmsg(window.parent.Drupal.t('LBL906'),window.parent.Drupal.t('ERR261'));
      					

      		//	}
      			if(errormsges1 == "")
      			{
      				var row = {"text":textData,"goTo":convertToSeconds(goTo),"ifChosenText":""};
					choices[index]= row;
				}
      	//			}
			});
			
			if(errormsges != "" && errormsges != undefined && errormsges != "undefined" &&
					errormsges1 != "" && errormsges1 != undefined && errormsges1 != "undefined"
					){
				errormsges += errormsges1;
				displayh5pexpertusmsg(errormsges);
				return false;
			}
			else if(errormsges != "" && errormsges != undefined && errormsges != "undefined")
			{
				displayh5pexpertusmsg(errormsges);
				return false;
			}
			else if(errormsges1 != "" && errormsges1 != undefined && errormsges1 != "undefined")
			{
				displayh5pexpertusmsg(errormsges1);
				return false;
			}
			

      	//	if(interactionsCurrent.action.params.choices[0] == "" )
				//errormsges += validateH5PTextField("Option 1",interactionsCurrent.action.params.choices[0]);
		//	else if(index == 1)
			//	errormsges += validateH5PTextField("Option 2",textData);
			
      		interactionsCurrent.action.params.text = text;
      		interactionsCurrent.action.params.choices = choices;
			
			
      		//To display the goto container once DONE button is clicked
      		jQObj(".h5p-interaction.h5p-poster.h5p-gotoquestion-interaction.h5p-dragnbar-element").css("display","block");

			saveInteractionCommonAPI(that,interactionsCurrent,interaction);			
      	}
      	else 
      	{
      	   
      	
        	if (H5PEditor.Html) {
          		// Need to do this before form is validated
          	H5PEditor.Html.removeWysiwyg();
        	}
	        //alert(interaction+"===="+JSON.stringify(interaction));
    	    if (that.validDialog(interaction)) {
        	  that.dnb.dialog.close();
          	interaction.focus();
        	}
        	that.IV.addSliderInteractions();
        }
        return false;
      });

    /*var $removeButton = $('<a href="#" class="h5p-button h5p-remove">' + t('remove') + '</a>')
      .click(function () {
        if (H5PEditor.Html) {
          // Need to do this before form is validated
          H5PEditor.Html.removeWysiwyg();
        }
        if (confirm(t('removeInteraction'))) {
          that.removeInteraction(interaction);
          that.dnb.dialog.close();
        }
        that.IV.addSliderInteractions();
        that.dnb.blurAll();
        return false;
      });*/
      
      
    var $removeButton = $('<a href="#" class="h5p-button h5p-remove">' + t('close') + '</a>')
      .click(function () {
        if (H5PEditor.Html) {
          // Need to do this before form is validated
          H5PEditor.Html.removeWysiwyg();
        }
      //  alert(ns.$(".clickonclosedonotremoveinteraction").size()+"==="+isEdit);
          if(isEdit != true)
          {
        	 // alert(ns.$(".clickonclosedonotremoveinteraction").size());
          	if(ns.$(".clickonclosedonotremoveinteraction").size() == 0)
          	{
          	
          		that.removeInteraction(interaction);
			
          		
          	}
          //	alert(7777);
          	that.IV.addSliderInteractions();	
          }
          
          $(".h5p_int_overlay").remove();
          that.dnb.dialog.close();
	      that.dnb.blurAll();
          return false;
      });
      

    var $buttons = $('<div class="h5p-dialog-buttons"></div>')
      .append($doneButton)
      .append($removeButton);

    interaction.setTitle(title);
    var libraryName = interaction.getLibraryName();

    if(libraryName == "H5P.SingleChoiceSet")
    	that.dnb.dialog.open(interaction.$form, title, interaction.getClass() + '-icon', $buttons,that.params.interactions[currentInteractionIdx],isEdit);
    else
    	//that.dnb.dialog.open(interaction.$form, title, interaction.getClass() + '-icon', $buttons,interaction);
    	that.dnb.dialog.open(interaction.$form, title, interaction.getClass() + '-icon', $buttons,that.params.interactions[currentInteractionIdx],isEdit);
	/*var libraryName = interaction.getLibraryName();
    
     if(libraryName == "H5P.SingleChoiceSet")
     {
			ns.$("#singlechoicesetlabel").html(that.params.interactions[0].label);
			ns.$("#singlechoicesetfrom").val(that.params.interactions[0].duration.from);
			ns.$("#singlechoicesetto").val(that.params.interactions[0].duration.to);
			
			if(displayType == "button")
				ns.$("#h5p-image-radio-button-0").attr("checked","checked");
			else
				ns.$("#h5p-image-radio-button-1").attr("checked","checked");
			
			
		
	 }*/
    // Blur context menu when opening dialog
    setTimeout(function () {
      that.dnb.blurAll();
    }, 0);
  };

  /**
   * Add interaction to drag n bar and initialize listeners.
   * @param {H5P.InteractiveVideoInteraction} interaction Interaction
   * @param {H5P.jQuery} $interaction Interaction element
   * @param {Object} [options] Options for new dnb element
   */
  InteractiveVideoEditor.prototype.addInteractionToDnb = function (interaction, $interaction, options) {
    var that = this;
    var newDnbElement = that.dnb.add($interaction, interaction.getClipboardData(), options);
    var createdNewElement = interaction.setDnbElement(newDnbElement);

    // New DragNBarElement was set, register listeners
    if (createdNewElement) {
      newDnbElement.contextMenu.on('contextMenuEdit', function () {
       var isEdit = true;
       	
        that.openInteractionDialog(interaction,isEdit);
        newDnbElement.hideContextMenu();
      });

      newDnbElement.contextMenu.on('contextMenuRemove', function () {
    	   removeInteractionMessage();
      //  if (confirm(t('removeInteraction'))) {
    	//  if (confirm(t('removeInteraction'))) {
    	//	  alert(123232323);
    	   
    	ns.$('.removingH5pInteraction').click(function(){ 
          that.removeInteraction(interaction);
          
			
          that.dnb.dialog.close();
          that.IV.addSliderInteractions();
          that.dnb.blurAll();
		  if(that.params.interactions.length == 0)
		//	window.parent.$(".edit-edit-btn-prerequest").removeClass("interactionsexists"); //to enable delete icon in the video
			  window.parent.$(".h5p-editor-iframe").removeClass("interactionsexists"); //to enable delete icon in the video
			  
        });
        that.IV.addSliderInteractions();
        that.dnb.blurAll(); 
      });

      newDnbElement.contextMenu.on('contextMenuBringToFront', function () {
        // Find interaction index
        var oldZ;
        for (var i = 0; i < that.IV.interactions.length; i++) {
          if (that.IV.interactions[i] === interaction) {
            oldZ = i;
            break;
          }
        }

        // Add to end of params
        that.params.interactions.push(that.params.interactions.splice(oldZ, 1)[0]);

        // Update internally for IV player
        that.IV.interactions.push(that.IV.interactions.splice(oldZ, 1)[0]);

        // Update visuals
        $interaction.appendTo(that.IV.$overlay);
      });
    }
  };

  /**
   * Called when rendering a new interaction.
   *
   * @param {H5P.InteractiveVideoInteraction} interaction
   * @param {H5P.jQuery} $interaction
   */
  InteractiveVideoEditor.prototype.newInteraction = function (interaction, $interaction) {
    var that = this;
    var libraryName = interaction.getLibraryName();
    var options = {
      lock: (libraryName === 'H5P.Image'),
      disableResize: (libraryName === 'H5P.Link') || interaction.isButton()
    };

    if (!interaction.isButton()) {
      // Add overlay
      $('<div/>', {
        'class': 'h5p-interaction-overlay'
      }).appendTo($interaction);
    }

    if (that.dnb !== undefined) {
      // Add resizing, context menu etc.
      that.addInteractionToDnb(interaction, $interaction, options);
    }

    if (!interaction.isButton()) {
      // Pause video on resizing
      $interaction.children('.h5p-dragnresize-handle').mousedown(function (event) {
        that.interaction = interaction;
        that.IV.video.pause();
      });
    }

    // Disable the normal dialog
    interaction.dialogDisabled = true;

    $interaction.mousedown(function (event) {
      // Keep track of last state
      that.IV.lastState = that.IV.currentState;

      that.interaction = interaction;
    }).dblclick(function (event) {
        var isEdit = false;
   //alert(event.target.className);
    	if(event.target.className == "h5p-interaction-label-text" || event.target.className == "h5p-interaction-button" || event.target.className == "h5p-interaction-label" || event.target.className == "h5p-interaction-overlay")
        isEdit = true;
      	that.openInteractionDialog(interaction,isEdit);
    }).focus(function () {
      // On focus, show overlay
      that.$focusHandler.addClass('show');
    });
  };

  /**
   * Validate the current dialog to see if it can be closed.
   *
   * @param {H5P.InteractiveVideoInteraction} interaction
   * @returns {boolean}
   */
  InteractiveVideoEditor.prototype.validDialog = function (interaction) {
    var valid = true;
    
    
  /* var elementKids = interaction.children;
    for (var i = 0; i < elementKids.length; i++) {
      if (elementKids[i].validate() === false) {
        valid = false;
      }
    }
	*/
    if (valid) {
      // Keep form	
      interaction.$form.detach();

      // Remove interaction from display
      interaction.remove(true);

      // Recreate content instance
      interaction.reCreate();

      // Make sure the element is inside the container the next time it's displayed
      interaction.fit = true;

      // Check if we should show again
      interaction.toggle(this.IV.video.getCurrentTime());

      if (this.dnb) {
        this.dnb.blurAll();
      }
    }
	ns.$(".h5p-interaction-button").css("display","block");
    return valid;
  };

  /**
   * Makes sure the given interaction doesn't stick out of the video container.
   *
   * @param {H5P.jQuery} $interaction
   * @param {Object} interactionParams
   */
  InteractiveVideoEditor.prototype.fit = function ($interaction, interactionParams) {
    var self = this;
    
    var videoContainer = H5P.DragNBar.getSizeNPosition(self.IV.$videoWrapper[0]);
    var updated = H5P.DragNBar.fitElementInside($interaction, videoContainer);

    // Set the updated properties
    var style = {};
    if (updated.width !== undefined) {
      interactionParams.width = updated.width / self.IV.scaledFontSize;
      style.width = interactionParams.width + 'em';
    }
    if (updated.left !== undefined) {
      interactionParams.x = updated.left / (videoContainer.width / 100);
      style.left = interactionParams.x + '%';
    }
    if (updated.height !== undefined) {
      interactionParams.height = updated.height / self.IV.scaledFontSize;
      style.height = interactionParams.height + 'em';
    }
    if (updated.top !== undefined) {
      interactionParams.y = updated.top / (videoContainer.height / 100);
      style.top = interactionParams.y + '%';
    }
    // Apply style
    $interaction.css(style);
  };

  /**
   * Revert our customization to the dialog.
   */
  InteractiveVideoEditor.prototype.hideDialog = function () {
    this.IV.hideDialog();
    this.IV.$dialog.children('.h5p-dialog-inner').css({
      height: '',
      width: ''
    });
    this.IV.$dialog.children('.h5p-dialog-hide').show();
    this.IV.$dialog.children('.h5p-dialog-buttons').remove();
  };

  /**
   * Remove interaction from video.
   *
   * @param {number} id
   */
  InteractiveVideoEditor.prototype.removeInteraction = function (interaction) {
  
    for (var i = 0; i < this.IV.interactions.length; i++) {
    	console.log('tt:'+JSON.stringify(this.params));
      if (this.IV.interactions[i] === interaction) {
        this.params.interactions.splice(i, 1);
        this.IV.interactions.splice(i, 1);
        break;
      }
    }
    H5PEditor.removeChildren(interaction.children);
    interaction.remove();
  };

  /**
   * Returns buttons for the DragNBar.
   *
   * @param {Array} libraries
   * @returns {Array}
   */
  InteractiveVideoEditor.prototype.getButtons = function (libraries) {
    var buttons = [];
    for (var i = 0; i < libraries.length; i++) {
      if (libraries[i].restricted === undefined || !libraries[i].restricted) {
        buttons.push(this.getButton(libraries[i]));
      }
    }

    return buttons;
  };

  /**
   * Find the title for the given library.
   *
   * @param {string} libraryName
   * @returns {string}
   */
  InteractiveVideoEditor.prototype.findLibraryTitle = function (libraryName) {
    if (!this.libraries) {
      return;
    }

    for (var i = 0; i < this.libraries.length; i++) {
      if (this.libraries[i].name === libraryName) {
        return this.getLibraryTitle(this.libraries[i]);
      }
    }
  };

  /**
   * Determines a human readable name for the library to use in the editor.
   *
   * @param {string} library
   * @returns {string}
   */
  InteractiveVideoEditor.prototype.getLibraryTitle = function (library) {
    // Determine title
    switch (library.name) {
      case 'H5P.Summary':
        return 'Statements';
      case 'H5P.Nil':
        return 'Label';
      default:
        return library.title;
    }
  };

  /**
   * Returns button data for the given library.
   *
   * @param {string} library
   * @returns {Object}
   */
  InteractiveVideoEditor.prototype.getButton = function (library) {
    var that = this;
    var id = library.name.split('.')[1].toLowerCase();

    return {
      id: id,
     // title: t('insertElement', {':type': that.getLibraryTitle(library).toLowerCase() }),
    //  title: t('insertElement', {':type': that.getLibraryTitle(library) }),
      title: t('insertElement', {':type': window.parent.Drupal.t(that.getLibraryTitle(library))}),
      
      createElement: function () {
        return that.addInteraction(library.uberName);
      }
    };
  };

  /**
   * Add a new interaction to the interactive video.
   *
   * @param {string|object} library Content type or parameters
   * @param {object} [options] Override the default options
   * @returns {H5P.jQuery}
   */
  InteractiveVideoEditor.prototype.addInteraction = function (library, options) {
  	
    this.IV.$overlay.addClass('h5p-visible');
    
    options = options || {};
    var self = this;
    self.IV.video.pause();

    var params;
    if (!(library instanceof String || typeof library === 'string')) {
      params = library;
    }

    var from = Math.floor(self.IV.video.getCurrentTime());
    if (!params) {
      params = {
        x: 47.813153766, // Center button
        y: 46.112273361,
        width: 10,
        height: 10,
        duration: {
          from: from,
          to: from + 10
        }
      };
      if (options.action) {
        params.action = options.action;
        params.displayType = options.displayType ? options.displayType : 'poster';
      }
      else {
        params.action = {
          library: library,
          params: {}
        };
      }
      if (options.width && options.height && !options.displayType) {
        params.width = options.width * this.pToEm;
        params.height = options.height * this.pToEm;
      }
      params.action.subContentId = H5P.createUUID();
      var type = library.split(' ')[0];
      if (type === 'H5P.Nil') {
        params.label = '';
      }
      else if (type === 'H5P.Link') {
        // Links are always posters
        params.displayType = 'poster';
      }
      if (options.pasted) {
        params.pasted = true;
      }
    }
    else {
      // Change starting time, but keep the same length
      params.duration.to = from + (params.duration.to - params.duration.from);
      params.duration.from = from;
    }

    var duration = Math.floor(self.IV.video.getDuration());
    if (params.duration.to > duration) {
      // Keep interaction inside video play time
      params.duration.to = duration;
    }

    // Make sure we don't overlap another visible element
    var size = window.getComputedStyle(this.IV.$videoWrapper[0]);
    var widthToPx = parseFloat(size.width) / 100;
    var heightToPx = parseFloat(size.height) / 100;
    var pos = {
      x: params.x * widthToPx,
      y: params.y * heightToPx
    };
    this.dnb.avoidOverlapping(pos, {
      width: params.width * this.IV.scaledFontSize,
      height: params.height * this.IV.scaledFontSize,
    });
    params.x = pos.x / widthToPx;
    params.y = pos.y / heightToPx;

    self.params.interactions.push(params);
    var i = self.params.interactions.length - 1;
    self.interaction = self.IV.initInteraction(i);
    self.processInteraction(self.interaction, params);

    var $interaction = self.interaction.toggle(from);
    this.IV.addSliderInteractions();
    return $interaction;
  };

  /**
   * Set new video params and remove old player.
   *
   * @param {Object} files
   */
  InteractiveVideoEditor.prototype.setVideo = function (files) {
    this.video = files;

    if (this.IV !== undefined) {
      delete this.IV;
    }
  };

  /**
   * Set new poster and remove old player.
   *
   * @param {Object} poster
   */
  InteractiveVideoEditor.prototype.setPoster = function (poster) {
    this.poster = poster;

    if (this.IV !== undefined) {
      delete this.IV;
    }
  };

  /**
   * Disable guided tour
   *
   * @method disableGuidedTour
   */
  InteractiveVideoEditor.prototype.disableGuidedTour = function () {
    this.showGuidedTour = false;
  };

  /**
   * Start the guided tour if not disabled
   *
   * @method startGuidedTour
   * @param  {Boolean}        force If true, don't care if user already has seen it
   */
  InteractiveVideoEditor.prototype.startGuidedTour = function (force) {
    if (this.showGuidedTour) {
      H5PEditor.InteractiveVideo.GuidedTours.start(this.currentTabIndex, force || false, t);
    }
  };

  /**
   * Append field to wrapper.
   *
   * @param {H5P.jQuery} $wrapper
   */
  InteractiveVideoEditor.prototype.appendTo = function ($wrapper) {
    var self = this;
    // Added to support older versions of core. Needed when using IV in CP.
    var $libwrap = $wrapper.parent().parent();
    if ($libwrap.hasClass('libwrap')) {
      $libwrap.addClass('h5p-interactivevideo-editor');
    }

    this.$item = $(this.createHtml()).appendTo($wrapper);
    this.$editor = this.$item.children('.h5peditor-interactions');
    this.$errors = this.$item.children('.h5p-errors');
    this.$bar = this.$item.children('.h5peditor-dragnbar');

/*    $('<span>', {
      'class': 'h5peditor-guided-tour',
      html: t('tourButtonStart'),
      click: function () {
        self.startGuidedTour(true);
        return false;
      }
    }).appendTo('.h5p-interactivevideo-editor .field.wizard > .h5peditor-label');
    self.startGuidedTour();*/
  };
  /**
   * Create HTML for the field.
   *
   * @returns {string}
   */
  InteractiveVideoEditor.prototype.createHtml = function () {
    return H5PEditor.createItem(this.field.widget, '<div class="h5peditor-interactions">' + t('selectVideo') + '</div>');
  };

  /**
   * Validate the current field.
   *
   * @returns {boolean}
   */
  InteractiveVideoEditor.prototype.validate = function () {
    // We must stops the playpack of any media!
    if (this.IV && this.IV.video) {
      this.IV.video.pause();
    }

    return true; // An interactive video is always valid :-)
  };

  /**
   * Remove this item.
   */
  InteractiveVideoEditor.prototype.remove = function () {
    this.$item.remove();
  };

  /**
   * Collect functions to execute once the tree is complete.
   *
   * @param {function} ready
   */
  InteractiveVideoEditor.prototype.ready = function (ready) {
    if (this.passReadies) {
      this.parent.ready(ready);
    }
    else {
      this.readies.push(ready);
    }
  };

  /**
   * Translate UI texts for this library.
   *
   * @private
   * @param {string} key
   * @param {Object} vars Placeholders
   * @returns {string}
   */
  var t = InteractiveVideoEditor.t = function (key, vars) {
    return H5PEditor.t('H5PEditor.InteractiveVideo', key, vars);
  };

  /**
   * Look for field with the given name in the given collection.
   *
   * @private
   * @param {string} name of field
   * @param {Array} fields collection to look in
   * @returns {Object} field object
   */
  var findField = function (name, fields) {
    for (var i = 0; i < fields.length; i++) {
      if (fields[i].name === name) {
        return fields[i];
      }
    }
  };

  /**
   * Hide the given fields from the given form.
   *
   * @private
   * @param {Array} interactionFields to be form
   * @param {Array} fields to hide
   */
  var hideFields = function (interactionFields, fields) {
    // Find and hide fields in list
    for (var i = 0; i < fields.length; i++) {
      var field = findField(fields[i], interactionFields);
      if (field) {
        field.widget = 'none';
      }
    }
  };

  return InteractiveVideoEditor;
})(H5P.jQuery);

// Default english translations
H5PEditor.language['H5PEditor.InteractiveVideo'] = {
  libraryStrings: {
    selectVideo: 'You must select a video before adding interactions.',
    notVideoField: '":path" is not a video.',
    notImageField: '":path" is not a image.',
    insertElement: ':type',
    popupTitle: 'Edit :type',
   // done: 'Done',
    done: window.parent.Drupal.t('LBL569'),
    loading: 'Loading...',
    remove: 'Remove',
   // close:'Close',
    close: window.parent.Drupal.t('LBL123'),
    removeInteraction: 'Are you sure you wish to remove this interaction?',
    addBookmark: 'Add bookmark',
    newBookmark: 'New bookmark',
    bookmarkAlreadyExists: 'Bookmark already exists here. Move playhead and add a bookmark at another time.',
    tourButtonStart: 'Tour',
    tourButtonExit: 'Exit',
    tourButtonDone: 'Done',
    tourButtonBack: 'Back',
    tourButtonNext: 'Next',
    tourStepUploadIntroText: '<p>This tour guides you through the most important features of the Interactive Video editor.</p><p>Start this tour at any time by pressing the Tour button in the top right corner.</p><p>Press EXIT to skip this tour or press NEXT to continue.</p>',
    tourStepUploadFileTitle: 'Adding video',
    tourStepUploadFileText: '<p>Start by adding a video file. You can upload a file from your computer or paste a URL to a YouTube video or a supported video file.</p><p>To ensure compatibility across browsers, you can upload multiple file formats of the same video, such as mp4 and webm.</p>',
    tourStepUploadAddInteractionsTitle: 'Adding interactions',
    tourStepUploadAddInteractionsText: '<p>Once you have added a video, you can start adding interactions.</p><p>Press the <em>Add interactions</em> tab to get started.</p>',
    tourStepCanvasToolbarTitle: 'Adding interactions',
    tourStepCanvasToolbarText: 'To add an interaction, drag an element from the toolbar and drop it onto the video.',
    tourStepCanvasEditingTitle: 'Editing interactions',
    tourStepCanvasEditingText: '<p>Once an interaction has been added, you can drag to reposition it.</p><p>To resize an interaction, press on the handles and drag.</p><p>When you select an interaction, a context menu will appear. To edit the content of the interaction, press the Edit button in the context menu. You can remove an interaction by pressing the Remove button on the context menu.</p>',
    tourStepCanvasBookmarksTitle: 'Bookmarks',
    tourStepCanvasBookmarksText: 'You can add Bookmarks from the Bookmarks menu. Press the Bookmark button to open the menu.',
    tourStepCanvasPreviewTitle: 'Preview your interactive video',
    tourStepCanvasPreviewText: 'Press the Play button to preview your interactive video while editing.',
    tourStepCanvasSaveTitle: 'Save and view',
    tourStepCanvasSaveText: "When you're done adding interactions to your video, press Save/Create to view the result.",
    tourStepSummaryText: 'This optional Summary quiz will appear at the end of the video.'
  }
};

 
function durationValidateH5P(fieldname,fieldval)
{
	var errormsgs = "";
	var resultMsg = "";
	console.log("trying to cross check");
	if( fieldval == "")
	{
		errormsgs += geth5pexpertusmsg(fieldname,window.parent.Drupal.t('ERR260'));
		return errormsgs; 
		
	}
	else if(fieldval != null && fieldval != "")
	{
		var valTmp = fieldval.split(":");
		var negativeCheck = "";
		//if(valTmp.length != 2)
		if(valTmp.length != 2 && valTmp.length != 3)
		{
			errormsgs += geth5pexpertusmsg(fieldname,window.parent.Drupal.t('ERR260'));
		}
		else if(isNaN(valTmp[0]) || isNaN(valTmp[1]) )
		{
			errormsgs += geth5pexpertusmsg(fieldname,window.parent.Drupal.t('ERR260'));
		}
		else if(fieldname.indexOf("goto") == 0)
		{
			var totalVideoTime = ns.$(".h5p-total").html();
			var totalVideoTimeInSec= convertToSeconds(totalVideoTime);
			negativeCheck = fieldval.charAt(0);
			if(negativeCheck == "-")
			{
					resultMsg = window.parent.Drupal.t('ERR213');  
					errormsgs += geth5pexpertusmsg(fieldname,resultMsg);
					return errormsgs;
			 }
			
			errormsgs += digitsSizeCheck(valTmp,fieldname);
			     
			fieldval = convertToSeconds(fieldval);
		  if(parseInt(fieldval,10) > parseInt(totalVideoTimeInSec,10)){
			 resultMsg = window.parent.Drupal.t('MSG903', {'@seconds' : totalVideoTimeInSec  });
			 errormsgs += geth5pexpertusmsg(fieldname,resultMsg);
			 return errormsgs; 
		  }
		}
		else 
		{
			var totalVideoTime = ns.$(".h5p-total").html();
			var totalVideoTimeInSec= convertToSeconds(totalVideoTime);			
			negativeCheck = fieldval.charAt(0);
			if(negativeCheck == "-")
			{
			 if(fieldname == "from"){

					resultMsg = window.parent.Drupal.t('ERR213');  
					errormsgs += geth5pexpertusmsg(fieldname,resultMsg);
					return errormsgs;
			 }
			 else if(fieldname == "to") {
				 resultMsg = window.parent.Drupal.t('ERR213');
					errormsgs += geth5pexpertusmsg(fieldname,resultMsg);
					return errormsgs;
			 }
			}
			errormsgs += digitsSizeCheck(valTmp,fieldname);
			 if(errormsgs !="" && errormsgs !="undefined" && errormsgs !=undefined )
				 return errormsgs;
			
			fieldval = convertToSeconds(fieldval);
			console.log("fieldval:"+fieldval);
			console.log("totalVideoTimeInSec:"+totalVideoTimeInSec);
			if(fieldname == "from")
			{
				/*if(fieldval <0)
				{
					resultMsg = window.parent.Drupal.t('ERR213');  //issue is with the minutes conversion check it
					errormsgs += geth5pexpertusmsg(fieldname,resultMsg);
					return errormsgs;
				}
				else*/ 
			   if(parseInt(fieldval,10) > parseInt(totalVideoTimeInSec,10))
				{
					resultMsg = window.parent.Drupal.t('MSG903', {'@seconds' : totalVideoTimeInSec  });
					errormsgs += geth5pexpertusmsg(fieldname,resultMsg);
					return errormsgs;
					
				}
			}
			else if(fieldname == "to")
			{
			  
				/*if(fieldval <0)
				{
					resultMsg = window.parent.Drupal.t('ERR213');
					errormsgs += geth5pexpertusmsg(fieldname,resultMsg);
					return errormsgs;
				}
				else*/ 
			   if(parseInt(fieldval,10) > parseInt(totalVideoTimeInSec,10))
				{
					resultMsg = window.parent.Drupal.t('MSG903', {'@seconds' : totalVideoTimeInSec  });
					errormsgs += geth5pexpertusmsg(fieldname,resultMsg);
					return errormsgs;
					
				}
			}
			return "";	
		}
		
	} 
	return errormsgs;
}


function urlH5PValidate(fieldName,fieldVal)
{
   	//Below one is hanging the browser: - Ticket #73323 so
   	//var regexx = new RegExp("^(http|https|ftp)\://([a-zA-Z0-9\.\-]+(\:[a-zA-Z0-9\.&amp;%\$\-]+)*@)*((25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[1-9])\.(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[1-9]|0)\.(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[1-9]|0)\.(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[0-9])|([a-zA-Z0-9\-]+\.)*[a-zA-Z0-9\-]+\.(com|edu|gov|int|mil|net|org|biz|arpa|info|name|pro|aero|coop|museum|[a-zA-Z]{2}))(\:[0-9]+)*(/($|[a-zA-Z0-9\.\,\?\'\\\+&amp;%\$#\=~_\-]+))*$");
	// introduced new version of regex
	var regexx = new RegExp("^(http[s]?:\\/\\/(www\\.)?|ftp:\\/\\/(www\\.)?|www\\.){1}([0-9A-Za-z-\\.@:%_\+~#=]+)+((\\.[a-zA-Z]{2,3})+)(/(.)*)?(\\?(.)*)?");
	    
	if(fieldVal == "" || fieldVal == null || fieldVal == undefined)
		return geth5pexpertusmsg(fieldName,window.parent.Drupal.t('ERR101'));  //if (link.search(/^http[s]?\:\/\//) == -1) {
	else if(fieldVal.search(/^http[s]?\:\/\//) == -1)
		return geth5pexpertusmsg(fieldName,window.parent.Drupal.t('ERR259'));
	else if (regexx.test(fieldVal)) {
        return "";
    }
    else 
    	return geth5pexpertusmsg(fieldName,window.parent.Drupal.t('ERR259'));
	

	
		
}


function validateH5PTextField(fieldName,fieldVal)
{	
	if(fieldName == "Text" || fieldName == "Title" || fieldName == "Label" || fieldName == "Question" || fieldName == "Option 1" || fieldName == "Option 2"  || fieldName == "Image")
	{
		if(fieldName == "Question")
			fieldName = "LBL325";
		if(fieldName == "Title")
			fieldName = "LBL083";
		
		if(fieldVal == "" || fieldVal == undefined)
			return geth5pexpertusmsg(window.parent.Drupal.t(fieldName),window.parent.Drupal.t('ERR101'));
		else
			return "";
	}
}
function geth5pexpertusmsg(fieldname,msg)
{
	if(fieldname.indexOf("goto") !== -1)
	{
		var indx = fieldname.substr(4);
	  return window.parent.Drupal.t('LBL906')+indx+" "+msg+" <br/>";
	}
	else if(fieldname == "from" || fieldname == "to")
		fieldname = fieldname.charAt(0).toUpperCase() +  fieldname.slice(1);

	return fieldname+" "+msg+" <br/>";
}
function displayh5pexpertusmsg(msg)
{
	ns.$('.e1_h5p_error').html("");
	ns.$('.e1_h5p_error').show();
	var data = '<div id="show_expertus_message"><div><div id="message-container" style="visibility: visible;"><div class="messages error">'+msg+'<div class="msg-close-btn" onclick=\"ns.$(\'.e1_h5p_error\').hide();\"></div></div></div><img  style=\"display:none;\"  src=\"sites/all/themes/core/expertusoneV2/expertusone-internals/images/close.png\" height=\"0\" width=\"0\"></div></div>';
	ns.$(".e1_h5p_error").html(data);
}



function DeleteInteraction(){
	
	ns.$('#delete-object-dialog').hide();
	ns.$('.h5p_int_overlay').remove();
	//ns.$('.h5p-big').hide();
	//ns.$('.h5p-big').hide();
	//ns.$('.e1-tip').hide();
	
}
function NotDeleteInteraction(){
	
	ns.$('#delete-object-dialog').hide();
	ns.$('.h5p_int_overlay').remove();
	//ns.$('#delete-object-dialog').dnb.dialog.close();
	
}

function saveInteractionCommonAPI(that,interactionsCurrent,interaction)
{
	if (that.validDialog(interaction)) 
	{
		displayh5pexpertusmsg(window.parent.Drupal.t("LBL3021")+" "+window.parent.Drupal.t("LBL3098"));//"Interaction is saved.");
		ns.$("#show_expertus_message").addClass("clickonclosedonotremoveinteraction");
		//that.dnb.dialog.close();
		interaction.focus();
	}
	that.IV.addSliderInteractions();
}
function getEncodedH5PText(text)
{
	return text;
	//return encodeURIComponent(text);
}
function getDecodedH5PText(text)
{
	return text;
	//return decodeURIComponent(text);
}

function removeFromParentWindowChild()
{
		H5P.jQuery(".h5p-remove").click();
}

function digitsSizeCheck(valTmp,fieldname){
 var hours = "", minutes = "",seconds="", errormsgs1 ="";	
 if(valTmp.length == 3)
  {
	hours = valTmp[0];
    minutes = valTmp[1];
    seconds = valTmp[2];
  }
 else if(valTmp.length == 2)
 {
	minutes = valTmp[0];
    seconds = valTmp[1];
 }
  if(hours.toString().length > 2 || minutes.toString().length > 2 || seconds.toString().length > 2 || minutes > 59 || seconds > 59)
  	 errormsgs1 = geth5pexpertusmsg(fieldname,window.parent.Drupal.t('ERR260'));
  	
  return errormsgs1;
 }

function checkstar(string){
	//var string = questions[index];
  	 var firststar=" ";
  	 var secondstar=" ";
  	 var spacecount=0;
  	 var wordcharcount=0;
  	for(var i =0; i < string.length; i++){
  	   if(string[i]=="*" && firststar ==" "){
  	   	firststar=true;
  	   	continue;
  	   }else if(string[i]=="*"){
  	   	secondstar=true;	       	   	
  	   }
  	   
  	   if(firststar==true && secondstar==true){
  	    if(spacecount==wordcharcount){
  	   	return geth5pexpertusmsg(window.parent.Drupal.t('LBL613'),'');
  	   	break;
  	   	}
  	   	secondstar=" ";
  	   	firststar=" ";
  	   	spacecount=0;
  	wordcharcount=0;
  	   }
  	   if(firststar==true){
  	   	if(string[i]==" "){
  	   	spacecount++;
  	   	}
  	   	wordcharcount++;	  
  	   }
  	
  	}

}