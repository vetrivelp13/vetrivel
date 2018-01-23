/*global H5P*/

var H5PEditor = H5PEditor || {};

/**
 * Create a field for the form.
 *
 * @param {mixed} parent
 * @param {Object} field
 * @param {mixed} params
 * @param {function} setValue
 * @returns {H5PEditor.Text}
 */
H5PEditor.CoursePresentation = function (parent, field, params, setValue) {
	var that = this;
	
	H5P.EventDispatcher.call(this);

	if (params === undefined) {
		params = {
				slides: [{
					elements: [],
					keywords: []
				}]
		};

		setValue(field, params);
	}

	this.parent = parent;
	this.field = field;
	this.params = params;
	// Elements holds a mix of forms and params, not element instances
	this.elements = [];
	this.slideRatio = 1.9753;

	this.doneProcessed = false;
	this.addSlideInProgress = false;
	this.passReadies = true;
	parent.ready(function () {
		that.passReadies = false;

		// Active surface mode
		var activeSurfaceCheckbox = H5PEditor.findField('override/activeSurface', parent);

		//h5pcustomize -- added if condition and try catch
		if (activeSurfaceCheckbox != null && activeSurfaceCheckbox != undefined)
		{
			try
			{
				activeSurfaceCheckbox.on('checked', function () {
					// Make note of current height
					var oldHeight = parseFloat(window.getComputedStyle(that.cp.$current[0]).height);

					// Enable adjustments
					that.cp.$container.addClass('h5p-active-surface');

					// Remove navigation
					that.cp.$progressbar.remove();

					// Find change in %
					var newHeight = parseFloat(window.getComputedStyle(that.cp.$current[0]).height);
					var change = (newHeight - oldHeight) / newHeight;

					// Update elements
					that.updateElementSizes(1 - change);
				});
			}catch(e){}
		}
	});

	// Make sure each slide has keywords array defined.
	// This won't always be the case for old presentations
	this.params.slides.forEach(function (slide) {
		slide.keywords = slide.keywords || [];
	});
};

H5PEditor.CoursePresentation.prototype = Object.create(H5P.EventDispatcher.prototype);
H5PEditor.CoursePresentation.prototype.constructor = H5PEditor.CoursePresentation;

/**
 * Must be changed if the semantics for the elements changes.
 * @type {string}
 */
H5PEditor.CoursePresentation.clipboardKey = 'H5PEditor.CoursePresentation';

/**
 * Will change the size of all elements using the given ratio.
 *
 * @param {number} heightRatio
 */
H5PEditor.CoursePresentation.prototype.updateElementSizes = function (heightRatio) {
	var $slides = this.cp.$slidesWrapper.children();

	// Go through all slides
	for (var i = 0; i < this.params.slides.length; i++) {
		var slide = this.params.slides[i];
		var $slideElements = $slides.eq(i).children();

		for (var j = 0; j < slide.elements.length; j++) {
			var element = slide.elements[j];

			// Update params
			element.height *= heightRatio;
			element.y *= heightRatio;

			// Update visuals if possible
			$slideElements.eq(j).css({
				height: element.height + '%',
				top: element.y + '%'
			});
		}
	}
};

/**
 * Add an element to the current slide and params.
 *
 * @param {string|object} library Content type or parameters
 * @param {object} [options] Override the default options
 * @returns {object}
 */
H5PEditor.CoursePresentation.prototype.addElement = function (library, options) {

	options = options || {};
	var elementParams;
	if (!(library instanceof String || typeof library === 'string')) {
		elementParams = library;
	}

	if (!elementParams) {
		// Create default start parameters
		elementParams = {
				x: 30,
				y: 30,
				width: 40,
				height: 40
		};

		if (library === 'GoToSlide') {
			elementParams.goToSlide = 1;
		}
		else {
			elementParams.action = (options.action ? options.action : {
				library: library,
				params: {}
			});
			elementParams.action.subContentId = H5P.createUUID();

			var libraryName = library.split(' ')[0];
			switch (libraryName) {
			case 'H5P.Audio':
				elementParams.width = 2.577632696;
				elementParams.height = 5.091753604;
				elementParams.action.params.fitToWrapper = true;
				break;

			case 'H5P.DragQuestion':
				elementParams.width = 50;
				elementParams.height = 50;
				break;
			}
		}

		if (options.width && options.height && !options.displayAsButton) {
			// Use specified size
			elementParams.width = options.width;
			elementParams.height = options.height * this.slideRatio;
		}
		if (options.displayAsButton) {
			elementParams.displayAsButton = true;
		}
	}
	if (options.pasted) {
		elementParams.pasted = true;
	}

	var slideIndex = this.cp.$current.index();
	var slideParams = this.params.slides[slideIndex];

	if (slideParams.elements === undefined) {
		// No previous elements
		slideParams.elements = [elementParams];
	}
	else {
		var containerStyle = window.getComputedStyle(this.dnb.$container[0]);
		var containerWidth = parseFloat(containerStyle.width);
		var containerHeight = parseFloat(containerStyle.height);

		// Make sure we don't overlap another element
		var pToPx = containerWidth / 100;
		var pos = {
				x: elementParams.x * pToPx,
				y: (elementParams.y * pToPx) / this.slideRatio
		};
		this.dnb.avoidOverlapping(pos, {
			width: (elementParams.width / 100) * containerWidth,
			height: (elementParams.height / 100) * containerHeight,
		});
		elementParams.x = pos.x / pToPx;
		elementParams.y = (pos.y / pToPx) * this.slideRatio;

		// Add as last element
		slideParams.elements.push(elementParams);
	}
	this.cp.$boxWrapper.add(this.cp.$boxWrapper.find('.h5p-presentation-wrapper:first')).css('overflow', 'visible');
	var instance = this.cp.addElement(elementParams, this.cp.$current, slideIndex);
	return this.cp.attachElement(elementParams, instance, this.cp.$current, slideIndex);
};

/**
 * Append field to wrapper.
 *
 * @param {type} $wrapper
 * @returns {undefined}
 */
H5PEditor.CoursePresentation.prototype.appendTo = function ($wrapper) {
	var that = this;
	var st = new Date().getTime();
	console.log("Toolbar loading st time:"+st);


	this.$item = H5PEditor.$(this.createHtml()).appendTo($wrapper);
	this.$editor = this.$item.children('.editor');
	this.$errors = this.$item.children('.h5p-errors');


	// Create new presentation.
	this.cp = new H5P.CoursePresentation(this.parent.params, H5PEditor.contentId, {cpEditor: this});
	this.cp.attach(this.$editor);
	if (this.cp.$wrapper.is(':visible')) {
		this.cp.trigger('resize');
	}
	var $settingsWrapper = H5PEditor.$('<div>', {
		'class': 'h5p-settings-wrapper hidden',
		appendTo: that.cp.$boxWrapper.children('.h5p-presentation-wrapper')
	});



	// Add drag and drop menu bar.
	that.initializeDNB();
	H5P.jQuery(".h5p-course-presentation").hide();
	window.parent.$(".addedit-form-cancel-container-actions").hide();
	// Find BG selector fields and init slide selector
	var globalBackgroundField = H5PEditor.CoursePresentation.findField('globalBackgroundSelector', this.field.fields);
	var slideFields = H5PEditor.CoursePresentation.findField('slides', this.field.fields);
	this.backgroundSelector = new H5PEditor.CoursePresentation.SlideSelector(that, that.cp.$slidesWrapper, globalBackgroundField, slideFields, that.params)
	.appendTo($settingsWrapper);

	
	
	// Add and bind slide controls.
	/*if($(".h5p-slides-wrapper .h5p-slide").size() > 1)
		{*/
	H5PEditor.$(
			'<div class="h5p-slidecontrols" style="display:none;">' +
			'<a href="#" title="' + H5PEditor.t('H5PEditor.CoursePresentation', 'backgroundSlide') + '" class="h5p-slidecontrols-button h5p-slidecontrols-button-background vtip"></a>' +
			'<a href="#" title="' + window.parent.Drupal.t("LBL3091") +' - '+window.parent.Drupal.t("LBL3093")+ '" class="h5p-slidecontrols-button h5p-slidecontrols-button-sort-left vtip"></a>' +
			'<a href="#" title="' + window.parent.Drupal.t("LBL3091") +' - '+window.parent.Drupal.t("LBL3092")+ '" class="h5p-slidecontrols-button h5p-slidecontrols-button-sort-right vtip"></a>' +
			'<a href="#" title="' + H5PEditor.t('H5PEditor.CoursePresentation', 'removeSlide') + '" class="h5p-slidecontrols-button h5p-slidecontrols-button-delete vtip"></a>' +
			'<a href="#" title="' + H5PEditor.t('H5PEditor.CoursePresentation', 'cloneSlide') + '" class="h5p-clone-slide h5p-slidecontrols-button h5p-slidecontrols-button-clone vtip"></a>' +
			'<a href="#" title="' + H5PEditor.t('H5PEditor.CoursePresentation', 'newSlide') + '" class="h5p-slidecontrols-button h5p-slidecontrols-button-add vtip"></a>' +
			'</div>'
	).appendTo(this.cp.$wrapper)
	.children('a:first')
	.click(function () {
		$(".h5p-remove").click();//remove any interaction window opened
		that.backgroundSelector.toggleOpen();
		H5PEditor.$(this).toggleClass('active');

		//H5PCUSTOMIZE - ONLY SELECT COLOR FILL  - HIDDEN RADIO BOX BY HEIGHT:1PX
		$("input[name='h5p-radio-selector-0']")[1].click();




		return false;
	})
	.next()
	.click(function () {
		$(".h5p-remove").click();//remove any interaction window opened
		that.trigger('sortSlide', -1);
		that.sortSlide(that.cp.$current.prev(), -1); // Left
		return false;
	})
	.next()
	.click(function () {
		$(".h5p-remove").click();//remove any interaction window opened
		that.trigger('sortSlide', 1);
		that.sortSlide(that.cp.$current.next(), 1); // Right
		return false;
	})
	.next()
	.click(function () {
		$(".h5p-remove").click();//remove any interaction window opened
		var removeIndex = that.cp.$current.index();
		var removed = that.removeSlide();

		if (removed !== false) {
			that.trigger('removeSlide', removeIndex);
		}
		return false;
	})
	.next()
	.click(function () {
		$(".h5p-remove").click(); //remove any interaction window opened
		H5P.recordCloned = true;
		that.addSlide(H5P.cloneObject(that.params.slides[that.cp.$current.index()],true));
		H5P.ContinuousText.Engine.run(that);
		return false;
	})
	.next()
	.click(function () {
		H5P.jQuery(".h5p-remove").click(); //remove any interaction window opened
		console.log("that.addSlideInProgress11:"+that.addSlideInProgress);
		if(that.addSlideInProgress == false) // if already processing happening then don't process
		{
			that.addSlide();
		}
		return false;
	});

	if (this.cp.activeSurface) {
		// Enable adjustments
		this.cp.$container.addClass('h5p-active-surface');

		// Remove navigation
		this.cp.$progressbar.remove();
	}

	var et = new Date().getTime();
	console.log("Toolbar loading end time:"+et+" in diff:"+(et-st));

	H5P.$window.on('resize', function () {
		that.cp.trigger('resize');

		// Reset drag and drop adjustments.
		if (that.keywordsDNS !== undefined) {
			delete that.keywordsDNS.dnd.containerOffset;
			delete that.keywordsDNS.marginAdjust;
		}
	});
	vtip();
	
	window.setTimeout(function(){
		H5P.jQuery(".h5p-course-presentation").show();
		window.setTimeout(function(){
			window.parent.$(".addedit-form-cancel-container-actions").show();
		},100); 
	},100); 

};

H5PEditor.CoursePresentation.prototype.addDNBButton = function (library) {
	var that = this;
	var id = library.name.split('.')[1].toLowerCase();

	return {
		id: id,
		title: H5PEditor.t('H5PEditor.CoursePresentation', 'insertElement', {':type': library.title}),//.toLowerCase()
		createElement: function () {
			return that.addElement(library.uberName);
		}
	};
};

H5PEditor.CoursePresentation.prototype.setContainerEm = function (containerEm) {
	this.containerEm = containerEm;

	if (this.dnb !== undefined && this.dnb.dnr !== undefined) {
		this.dnb.dnr.setContainerEm(this.containerEm);
	}
	H5P.jQuery(".h5p-element-outer:not('.h5p-image-outer-element,.h5p-singlechoiceset-outer-element, .h5p-advancedtext-outer-element, .h5p-dragtext-outer-element, .h5p-markthewords-outer-element, .h5p-video-outer-element, .h5p-audio-outer-element, .h5p-blanks-outer-element ')").css({"height":"auto","position":"relative"});
	H5P.jQuery(".other-outer-element").css("height","100%");
	H5P.jQuery(".h5p-link-outer-element").css("height","100%");
	
//	H5P.jQuery(".h5p-element-inner .h5p-go-to-slide").parent().parent().css({"max-height":"40%","min-height":"40%"});

};

/**
 * Initialize the drag and drop menu bar.
 *
 * @returns {undefined}
 */
H5PEditor.CoursePresentation.prototype.initializeDNB = function () {
	var that = this;

	this.$bar = H5PEditor.$('<div class="h5p-dragnbar">' + H5PEditor.t('H5PEditor.CoursePresentation', 'loading') + '</div>').insertBefore(this.cp.$boxWrapper);
	var slides = H5PEditor.CoursePresentation.findField('slides', this.field.fields);
	var elementFields = H5PEditor.CoursePresentation.findField('elements', slides.field.fields).field.fields;
	var action = H5PEditor.CoursePresentation.findField('action', elementFields);
	var st = new Date().getTime();
	console.log("initializeDNB:"+st);
	//H5PEditor.LibraryListCache.getLibraries(action.options, function (libraries) {

	var libraries = [{"uberName":"H5P.AdvancedText 1.1","name":"H5P.AdvancedText","majorVersion":"1","minorVersion":"1","tutorialUrl":null,"title":window.parent.Drupal.t("Advanced Text"),"runnable":"0","restricted":false},{"uberName":"H5P.Link 1.1","name":"H5P.Link","majorVersion":"1","minorVersion":"1","tutorialUrl":null,"title":window.parent.Drupal.t("Link"),"runnable":"0","restricted":false},{"uberName":"H5P.Image 1.0","name":"H5P.Image","majorVersion":"1","minorVersion":"0","tutorialUrl":null,"title":window.parent.Drupal.t("LBL3044"),"runnable":"0","restricted":false},{"uberName":"H5P.Video 1.2","name":"H5P.Video","majorVersion":"1","minorVersion":"2","tutorialUrl":null,"title":window.parent.Drupal.t("Video"),"runnable":"0","restricted":false},{"uberName":"H5P.Audio 1.2","name":"H5P.Audio","majorVersion":"1","minorVersion":"2","tutorialUrl":null,"title":window.parent.Drupal.t("Audio"),"runnable":"1","restricted":false},{"uberName":"H5P.Blanks 1.4","name":"H5P.Blanks","majorVersion":"1","minorVersion":"4","tutorialUrl":null,"title":window.parent.Drupal.t("Fill in the Blanks"),"runnable":"1","restricted":false},{"uberName":"H5P.SingleChoiceSet 1.3","name":"H5P.SingleChoiceSet","majorVersion":"1","minorVersion":"3","tutorialUrl":null,"title":window.parent.Drupal.t("Single Choice Set"),"runnable":"1","restricted":false},{"uberName":"H5P.DragText 1.4","name":"H5P.DragText","majorVersion":"1","minorVersion":"4","tutorialUrl":null,"title":window.parent.Drupal.t("Drag Text"),"runnable":"1","restricted":false},{"uberName":"H5P.MarkTheWords 1.5","name":"H5P.MarkTheWords","majorVersion":"1","minorVersion":"5","tutorialUrl":null,"title":window.parent.Drupal.t("Mark the Words"),"runnable":"1","restricted":false}];
	console.log(JSON.stringify(libraries));
	that.libraries = libraries;
	var buttons = [];
	for (var i = 0; i < libraries.length; i++) {
		if (libraries[i].restricted !== true) {
			buttons.push(that.addDNBButton(libraries[i]));
		}
	}

	// Add go to slide button
	var goToSlide = H5PEditor.CoursePresentation.findField('goToSlide', elementFields);
	if (goToSlide) {
		buttons.splice(5, 0, {
			id: 'gotoslide',
			title: H5PEditor.t('H5PEditor.CoursePresentation', 'insertElement', {':type': window.parent.Drupal.t(goToSlide.label)}),
			createElement: function () {
				return that.addElement('GoToSlide');
			}
		});
	}

	var et = new Date().getTime();
	console.log("initializeDNB end:"+et+" diff:"+(et-st));

	that.dnb = new H5P.DragNBar(buttons, that.cp.$current, that.$editor, {$blurHandlers: that.cp.$boxWrapper});
	that.dnb.dnr.snap = 10;
	that.dnb.dnr.setContainerEm(that.containerEm);

	// Register all attached elements with dnb
	that.elements.forEach(function (slide, slideIndex) {
		slide.forEach(function (element, elementIndex) {
			var elementParams = that.params.slides[slideIndex].elements[elementIndex];
			var options = {};
			if (elementParams.displayAsButton) {
				options.disableResize = true;
			}

			var type = (elementParams.action ? elementParams.action.library.split(' ')[0] : null);
			if (type === 'H5P.Image' || (type === 'H5P.Chart' && elementParams.action.params.graphMode === 'pieChart')) {
				options.lock = true;
			}

			// Register option for locking dimensions if image
			that.addToDragNBar(element, elementParams, options);
		});
	});

	var reflowLoop;
	var reflowInterval = 250;
	var reflow = function () {
		H5P.ContinuousText.Engine.run(that);
		reflowLoop = setTimeout(reflow, reflowInterval);
	};

	// Resizing listener
	that.dnb.dnr.on('startResizing', function (eventData) {
		var elementParams = that.params.slides[that.cp.$current.index()].elements[that.dnb.$element.index()];

		// Check for continuous text
		if (elementParams.action && elementParams.action.library.split(' ')[0] === 'H5P.ContinuousText') {
			reflowLoop = setTimeout(reflow, reflowInterval);
		}
	});

	// Resizing has stopped
	that.dnb.dnr.on('stoppedResizing', function (eventData) {
		var elementParams = that.params.slides[that.cp.$current.index()].elements[that.dnb.$element.index()];

		// Store new element position

		elementParams.width = that.dnb.$element.width() / (that.cp.$current.innerWidth() / 100);
		elementParams.height = that.dnb.$element.height() / (that.cp.$current.innerHeight() / 100);

		//h5pcustomize
		if(elementParams.action.library == "H5P.Image 1.0")
		{

			try
			{
				elementParams.action.params.file.heightPX = that.dnb.$element.height();
				elementParams.action.params.file.widthPX = that.dnb.$element.width();
			}catch(e){}
		}
		else
		{
//			alert("height_fresh:"+that.dnb.$element.height()+ "width_fresh:"+that.dnb.$element.width());
//			alert(that.dnb.$element.html());
			elementParams.action.params.heightPX = that.dnb.$element.height();

			elementParams.action.params.widthPX = that.dnb.$element.width();

		}
//		alert(that.dnb.$element.html());
		//   if(element.action.library == "H5P.MarkTheWords 1.5")
		{
//			elementParams.action.params.heightPX = parseFloat($element.css("height"));
//			elementParams.action.params.widthPX =parseFloat($element.css("width"));
			var total_height = parseInt(H5P.jQuery(".h5p-markthewords-outer-element").last().height());
			var title_height = parseInt(H5P.jQuery(".h5p-markthewords-outer-element").last().find(".h5p-mark-the-words").find(".h5p-question-introduction").height());
			var body_height = parseInt(H5P.jQuery(".h5p-markthewords-outer-element").last().find(".h5p-mark-the-words").find(".h5p-question-content").height());
			var button_height = parseInt(H5P.jQuery(".h5p-markthewords-outer-element").last().find(".h5p-mark-the-words").find(".h5p-question-buttons").height());
			var excess_difference = total_height - (title_height + body_height + button_height);
			console.log("total_heightaaabb:"+total_height+ "title_height:"+title_height+ "body_height:"+body_height+"button_height:"+button_height+"excess_difference"+excess_difference);
			//alert("excess_difference"+excess_difference);
			elementParams.action.params.excess_difference = excess_difference;
		}

		elementParams.y = ((parseFloat(that.dnb.$element.css('top')) / that.cp.$current.innerHeight()) * 100);
		elementParams.x = ((parseFloat(that.dnb.$element.css('left')) / that.cp.$current.innerWidth()) * 100);

		// Stop reflow loop and run one last reflow
		if (elementParams.action && elementParams.action.library.split(' ')[0] === 'H5P.ContinuousText') {
			clearTimeout(reflowLoop);
			H5P.ContinuousText.Engine.run(that);
		}

		// Trigger element resize
		var elementInstance = that.cp.elementInstances[that.cp.$current.index()][that.dnb.$element.index()];
		H5P.trigger(elementInstance, 'resize');
	});

	// Update params when the element is dropped.
	that.dnb.stopMovingCallback = function (x, y) {
		var params = that.params.slides[that.cp.$current.index()].elements[that.dnb.$element.index()];
		params.x = x;
		params.y = y;
	};

	// Update params when the element is moved instead, to prevent timing issues.
	that.dnb.dnd.moveCallback = function (x, y) {
		var params = that.params.slides[that.cp.$current.index()].elements[that.dnb.$element.index()];
		params.x = x;
		params.y = y;

		that.dnb.updateCoordinates();
	};

	// Edit element when it is dropped.
	that.dnb.dnd.releaseCallback = function () {
		var params = that.params.slides[that.cp.$current.index()].elements[that.dnb.$element.index()];
		var element = that.elements[that.cp.$current.index()][that.dnb.$element.index()];

		if (that.dnb.newElement) {
			that.cp.$boxWrapper.add(that.cp.$boxWrapper.find('.h5p-presentation-wrapper:first')).css('overflow', '');

			if (params.action !== undefined && H5P.libraryFromString(params.action.library).machineName === 'H5P.ContinuousText') {
				H5P.ContinuousText.Engine.run(that);
				if (!that.params.ct) {
					// No CT text but there could be elements
					var CTs = that.getCTs(false, true);
					if (CTs.length === 1) {
						// First element, open form
						that.showElementForm(element, that.dnb.$element, params,false);
					}
				}
			}
			else {
				that.showElementForm(element, that.dnb.$element, params,false);
			}
		}
	};

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

	that.dnb.on('paste', function (event) {
		var pasted = event.data;
		var options = {
				width: pasted.width,
				height: pasted.height,
				pasted: true
		};

		if (pasted.from === H5PEditor.CoursePresentation.clipboardKey) {
			// Pasted content comes from the same version of CP

			if (!pasted.generic) {
				// Non generic part, must be content like gotoslide or similar
				that.dnb.focus(that.addElement(pasted.specific, options));
			}
			else if (supported(pasted.generic.library)) {
				// Has generic part and the generic libray is supported
				that.dnb.focus(that.addElement(pasted.specific, options));
			}
			else {
				alert(H5PEditor.t('H5P.DragNBar', 'unableToPaste'));
			}
		}
		else if (pasted.generic) {
			if (supported(pasted.generic.library)) {
				// Supported library from another content type)

				if (pasted.specific.displayType === 'button') {
					// Make sure buttons from IV  still are buttons.
					options.displayAsButton = true;
				}
				options.action = pasted.generic;
				that.dnb.focus(that.addElement(pasted.generic.library, options));
			}
			else {
				alert(H5PEditor.t('H5P.DragNBar', 'unableToPaste'));
			}
		}
	});

	that.dnb.attach(that.$bar);

	// Bind keyword interactions.
	that.initKeywordInteractions();

	// Trigger event
	that.trigger('librariesReady');

	// });
};

/**
 * Create HTML for the field.
 */
H5PEditor.CoursePresentation.prototype.createHtml = function () {
	return H5PEditor.createItem(this.field.widget, '<div class="editor"></div>');
};

/**
 * Validate the current field.
 */
H5PEditor.CoursePresentation.prototype.validate = function () {
	// Validate all form elements
	var valid = true;
	var firstCT = true;
	for (var i = 0; i < this.elements.length; i++) {
		if (!this.elements[i]) {
			continue;
		}
		for (var j = 0; j < this.elements[i].length; j++) {
			// We must make sure form values are stored if the dialog was never closed
			var elementParams = this.params.slides[i].elements[j];
			var isCT = (elementParams.action !== undefined && elementParams.action.library.split(' ')[0] === 'H5P.ContinuousText');
			if (isCT && !firstCT) {
				continue; // Only need to process the first CT
			}

			// Validate element form
			for (var k = 0; k < this.elements[i][j].children.length; k++) {
				if (this.elements[i][j].children[k].validate() === false && valid) {
					valid = false;
				}
			}

			if (isCT) {
				if (!this.params.ct) {
					// Store complete text in CT param
					this.params.ct = elementParams.action.params.text;
				}
				firstCT = false;
			}
		}
	}
	valid &= this.backgroundSelector.validate();

	//h5pcustomize 
	// Distribute CT text across elements
	//H5P.ContinuousText.Engine.run(this);
	return valid;
};

/**
 * Remove this item.
 */
H5PEditor.CoursePresentation.prototype.remove = function () {
	this.$item.remove();
};

/**
 * Initialize keyword interactions.
 *
 * @returns {undefined} Nothing
 */
H5PEditor.CoursePresentation.prototype.initKeywordInteractions = function () {
	var that = this;
	// Add our own menu to the drag and drop menu bar.
	that.$keywordsDNB = H5PEditor.$(
			'<ul class="h5p-dragnbar-ul h5p-dragnbar-left">' +
			'<li class="h5p-dragnbar-li">' +
			'<div title="' + H5PEditor.t('H5PEditor.CoursePresentation', 'keywordsMenu') + '" class="h5p-dragnbar-a h5p-dragnbar-keywords" role="button" tabindex="1"></div>' +
			'<div class="h5p-keywords-dropdown">' +
			'<label class="h5p-keywords-enable"><input type="checkbox"/> Keywords list</label>' +
			'<label class="h5p-keywords-always"><input type="checkbox"/> Always show</label>' +
			'<label class="h5p-keywords-hide"><input type="checkbox"/> Auto hide</label>' +
			'<label class="h5p-keywords-opacity">Opacity <input type="text"/> %</label>' +
			'</div>' +
			'</li>' +
	'</ul>').prependTo(this.$bar);

	// We use this awesome library to make things easier.
	this.keywordsDNS = new H5P.DragNSort(this.cp.$keywords);

	this.keywordsDNS.startMovingCallback = function (event) {
		return that.keywordStartMoving(event);
	};

	this.keywordsDNS.moveCallback = function (x, y) {
		that.keywordMove(x, y);
	};

	this.keywordsDNS.swapCallback = function (direction) {
		that.swapKeywords(direction);
	};

	// Keyword events
	var keywordClick = function (event) {
		// Convert keywords into text areas when clicking.
		if (!that.keywordsDNS.moving && that.editKeyword(H5PEditor.$(this)) !== false) {
			event.stopPropagation();
		}
	};
	var keywordMousedown = function (event) {
		that.keywordsDNS.press(H5PEditor.$(this).parent(), event.pageX, event.pageY);
		return false;
	};
	var newKeyword = function ($li, newKeywordString, classes, x, y) {
		if (that.$keywordsTip !== undefined) {
			that.$keywordsTip.remove();
			delete that.$keywordsTip;
		}

		var $ol = $li.children('ol');
		if (!$ol.length) {
			$ol = H5PEditor.$('<ol class="h5p-keywords-ol"></ol>').prependTo($li);
		}
		var $element = H5PEditor.$('<li class="h5p-keywords-li h5p-new-keyword h5p-empty-keyword ' + classes + '"><span>' + newKeywordString + '</span></li>').appendTo($ol);
		var $label = $element.children('span').click(keywordClick).mousedown(keywordMousedown);

		that.keywordsDNS.press($element, x, y);

		// Edit once element is dropped.
		var edit = function () {
			H5P.$body.off('mouseup', edit).off('mouseleave', edit);

			// Use timeout to edit on next tick. (when moving and sorting has finished)
			setTimeout(function () {
				that.keywordsDNS.moving = false;
				$label.trigger('click');
			}, 0);
		};
		H5P.$body.on('mouseup', edit).on('mouseleave', edit);

		return false;
	};

	// Make existing keywords editable
	this.cp.$keywords.find('span').click(keywordClick).mousedown(keywordMousedown);

	this.$newKeyword = H5PEditor.$('<li class="h5p-keywords-li h5p-add-keyword" role="button" tabindex="1">Add keyword</li>').mousedown(function (event) {
		if (event.button !== 0) {
			return; // We only handle left click
		}

		// Create new keyword.
		var newKeywordString = H5PEditor.t('H5PEditor.CoursePresentation', 'newKeyword');

		// Add to params
		that.params.slides[that.cp.$current.index()].keywords.push({main: newKeywordString});

		return newKeyword(that.cp.$keywords.children('.h5p-current'), newKeywordString, 'h5p-main-keyword', event.pageX, event.pageY);
	}).appendTo(this.cp.$currentKeyword);

	// Make keywords drop down menu come alive
	var $dropdown = this.$bar.find('.h5p-keywords-dropdown');
	var preventClose = false;
	var closeDropdown = function () {
		if (preventClose) {
			preventClose = false;
		}
		else {
			$dropdown.removeClass('h5p-open');
			that.cp.$container.off('click', closeDropdown);
		}
	};

	// Open dropdown when clicking the dropdown button
	this.$bar.find('.h5p-dragnbar-keywords').click(function () {
		if (!$dropdown.hasClass('h5p-open')) {
			that.cp.$container.on('click', closeDropdown);
			$dropdown.addClass('h5p-open');
			preventClose = true;
		}
	});

	// Prevent closing when clicking on the dropdown dialog it self
	$dropdown.click(function () {
		preventClose = true;
	});

	// Enable keywords list
	var $enableKeywords = this.$bar.find('.h5p-keywords-enable input').change(function () {
		that.params.keywordListEnabled = $enableKeywords.is(':checked');
		if (that.params.keywordListEnabled) {
			if (that.params.keywordListAlwaysShow) {
				that.cp.$keywordsWrapper.show().add(that.cp.$keywordsButton).addClass('h5p-open');
				that.cp.$keywordsButton.hide();
			}
			else {
				that.cp.$keywordsWrapper.add(that.cp.$keywordsButton).show();
			}
		}
		else {
			that.cp.$keywordsWrapper.add(that.cp.$keywordsButton).hide();
		}
	});

	// Always show keywords list
	var $alwaysKeywords = this.$bar.find('.h5p-keywords-always input').change(function () {
		that.params.keywordListAlwaysShow = $alwaysKeywords.is(':checked');
		if (!that.params.keywordListEnabled) {
			that.cp.hideKeywords();
			that.cp.$keywordsButton.hide();
			return;
		} else if (!that.params.keywordListAlwaysShow) {
			that.cp.$keywordsButton.show();
		}
		if (that.params.keywordListAlwaysShow) {
			that.cp.$keywordsButton.hide();
			that.cp.showKeywords();
		}
		else if (that.params.keywordListEnabled) {
			that.cp.$keywordsButton.show();
			that.cp.showKeywords();
		}
	});

	// Auto hide keywords list
	var $hideKeywords = this.$bar.find('.h5p-keywords-hide input').change(function () {
		that.params.keywordListAutoHide = $hideKeywords.is(':checked');
	});

	// Opacity for keywords list
	var $opacityKeywords = this.$bar.find('.h5p-keywords-opacity input').change(function () {
		var opacity = parseInt($opacityKeywords.val());
		if (isNaN(opacity)) {
			opacity = 90;
		}
		if (opacity > 100) {
			opacity = 100;
		}
		if (opacity < 0) {
			opacity = 0;
		}
		that.params.keywordListOpacity = opacity;
		that.cp.setKeywordsOpacity(opacity);
	});

	/**
	 * Help set default values if undefined.
	 *
	 * @private
	 * @param {String} option
	 * @param {*} defaultValue
	 */
	var checkDefault = function (option, defaultValue) {
		if (that.params[option] === undefined) {
			that.params[option] = defaultValue;
		}
	};

	// Set defaults if undefined
	checkDefault('keywordListEnabled', true);
	checkDefault('keywordListAlwaysShow', false);
	checkDefault('keywordListAutoHide', false);
	checkDefault('keywordListOpacity', 90);

	// Update HTML
	$enableKeywords.attr('checked', that.params.keywordListEnabled);
	$alwaysKeywords.attr('checked', that.params.keywordListAlwaysShow);
	$hideKeywords.attr('checked', that.params.keywordListAutoHide);
	$opacityKeywords.val(that.params.keywordListOpacity);
};

/**
 * Keyword start moving handler.
 *
 * @param {object} event
 * @returns {Boolean} Indicates if we're ready to start moving.
 */
H5PEditor.CoursePresentation.prototype.keywordStartMoving = function (event) {
	// Make sure we're moving the keywords that belongs to this slide.
	this.keywordsDNS.$parent = this.keywordsDNS.$element.parent().parent();
	if (!this.keywordsDNS.$parent.hasClass('h5p-current')) {
		// Element is a sub keyword.
		if (!this.keywordsDNS.$parent.parent().parent().hasClass('h5p-current')) {
			return false;
		}
	}
	else {
		delete this.keywordsDNS.$parent; // Remove since we're not a sub keyword.
	}

	if (this.keywordsDNS.$element.hasClass('h5p-new-keyword')) {
		this.keywordsDNS.$element.removeClass('h5p-new-keyword');
	}

	this.keywordsDNS.dnd.scrollTop = this.cp.$keywords.scrollTop() - parseInt(this.cp.$keywords.css('marginTop'));
	return true;
};

/**
 * Keyword move handler.
 *
 * @param {int} x
 * @param {int} y
 * @returns {undefined}
 */
H5PEditor.CoursePresentation.prototype.keywordMove = function (x, y) {
	// Check if sub keyword should change parent.
	if (this.keywordsDNS.$parent === undefined) {
		return;
	}

	var fontSize = parseInt(this.cp.$wrapper.css('fontSize'));

	// Jump up
	var $prev = this.keywordsDNS.$parent.prev();
	if ($prev.length && y < $prev.offset().top + ($prev.height() + this.keywordsDNS.marginAdjust + parseInt($prev.css('paddingBottom')) - (fontSize/2))) {
		return this.jumpKeyword($prev, 1);
	}

	// Jump down
	var $next = this.keywordsDNS.$parent.next();
	if ($next.length && y + this.keywordsDNS.$element.height() > $next.offset().top + fontSize) {
		return this.jumpKeyword($next, -1);
	}
};

/**
 * Update params after swapping keywords.
 *
 * @param {type} direction
 * @returns {undefined}
 */
H5PEditor.CoursePresentation.prototype.swapKeywords = function (direction) {
	var keywords = this.params.slides[this.cp.$current.index()].keywords;
	if (this.keywordsDNS.$parent !== undefined) {
		// We're swapping sub keywords.
		keywords = keywords[this.keywordsDNS.$parent.index()].subs;
	}

	var index = this.keywordsDNS.$element.index() - 1;
	var oldIndex = index + direction;
	var oldItem = keywords[oldIndex];
	keywords[oldIndex] = keywords[index];
	keywords[index] = oldItem;
};

/**
 * Move a sub keyword to another parent.
 *
 * @param {jQuery} $target The new parent.
 * @param {int} direction Indicates the direction we're jumping in.
 * @returns {undefined}
 */
H5PEditor.CoursePresentation.prototype.jumpKeyword = function ($target, direction) {
	var $ol = $target.children('ol');
	if (!$ol.length) {
		$ol = H5PEditor.$('<ol class="h5p-keywords-ol"></ol>').appendTo($target);
	}

	// Remove from params
	var keywords = this.slide.params.slides[this.cp.$current.index()].keywords;
	var subs = keywords[this.keywordsDNS.$parent.index()];
	var item = subs.subs.splice(this.keywordsDNS.$element.index() - 1, 1)[0];
	if (!subs.subs.length) {
		delete subs.subs;
	}

	// Update UI
	if (direction === -1) {
		this.keywordsDNS.$element.add(this.keywordsDNS.$placeholder).prependTo($ol);
	}
	else {
		this.keywordsDNS.$element.add(this.keywordsDNS.$placeholder).appendTo($ol);
	}

	// Add to params
	subs = keywords[$target.index()];
	if (subs.subs === undefined) {
		subs.subs = [item];
	}
	else {
		subs.subs.splice(this.keywordsDNS.$element.index() - 1, 0, item);
	}

	// Remove ol if empty.
	$ol = this.keywordsDNS.$parent.children('ol');
	if (!$ol.children('li').length) {
		$ol.remove();
	}
	this.keywordsDNS.$parent = $target;
};

/**
 * Adds slide after current slide.
 *
 * @param {object} slideParams
 * @returns {undefined} Nothing
 */
H5PEditor.CoursePresentation.prototype.addSlide = function (slideParams) {
	var that = this;
	this.addSlideInProgress = true;
	if (slideParams === undefined) {
		// Set new slide params
		slideParams = {
				elements: []
		};
		if (this.cp.$keywords !== undefined) {
			slideParams.keywords = [];
		}
	}

	var index = this.cp.$current.index() + 1;
	if (index >= this.params.slides.length) {
		this.params.slides.push(slideParams);
	}
	else {
		this.params.slides.splice(index, 0, slideParams);
	}

	this.elements.splice(index, 0, []);
	this.cp.elementInstances.splice(index, 0, []);
	this.cp.elementsAttached.splice(index, 0, []);

	// Add slide with elements
	var $slide = H5P.jQuery(H5P.CoursePresentation.createSlide(slideParams)).insertAfter(this.cp.$current);
	that.trigger('addedSlide', that.cp.$current.index() + 1);


	this.cp.addElements(slideParams, $slide, $slide.index());

	// Add keywords
	if (slideParams.keywords !== undefined) {
		H5PEditor.$(this.cp.keywordsHtml(slideParams.keywords)).insertAfter(this.cp.$currentKeyword).click(function (event) {
			that.cp.keywordClick(H5PEditor.$(this));
			event.preventDefault();
		}).find('span').click(function (event) {
			// Convert keywords into text areas when clicking.
			if (!that.keywordsDNS.moving && that.editKeyword(H5PEditor.$(this)) !== false) {
				event.stopPropagation();
			}
		}).mousedown(function (event) {
			that.keywordsDNS.press(H5PEditor.$(this).parent(), event.pageX, event.pageY);
			return false;
		});
	}

	this.updateNavigationLine(index);
	console.log("that.addSlideInProgress in 111 true:"+that.addSlideInProgress);
	// Switch to the new slide.
	this.cp.nextSlide();

	
};

H5PEditor.CoursePresentation.prototype.updateNavigationLine = function (index) {
	var that = this;
	// Update slides with solutions.
	var hasSolutionArray = [];
	this.cp.slides.forEach(function (instanceArray, slideNumber) {
		var isTaskWithSolution = false;

		if (that.cp.elementInstances[slideNumber] !== undefined && that.cp.elementInstances[slideNumber].length) {
			that.cp.elementInstances[slideNumber].forEach(function (elementInstance) {
				if (that.cp.checkForSolutions(elementInstance)) {
					isTaskWithSolution = true;
				}
			});
		}

		if (isTaskWithSolution) {
			hasSolutionArray.push([[isTaskWithSolution]]);
		} else {
			hasSolutionArray.push([]);
		}
	});

	// Update progressbar and footer
	this.cp.navigationLine.initProgressbar(hasSolutionArray);
	this.cp.navigationLine.updateProgressBar(index);
	this.cp.navigationLine.updateFooter(index);
};

/**
 * Remove the current slide
 *
 * @returns {Boolean} Indicates success
 */
H5PEditor.CoursePresentation.prototype.removeSlide = function () {
	var index = this.cp.$current.index();
	var $remove = this.cp.$current.add(this.cp.$currentKeyword);
	var cpObj = this;
	removeInteractionMessage(cpObj,'slide');
	H5P.jQuery('.removingH5pInteraction').click(function(){
		// Remove elements from slide
		removeSlideWrapper(cpObj,index,$remove);

	});
};

/**
 * Sort current slide in the given direction.
 *
 * @param {H5PEditor.$} $element The next/prev slide.
 * @param {int} direction 1 for next, -1 for prev.
 * @returns {Boolean} Indicates success.
 */
H5PEditor.CoursePresentation.prototype.sortSlide = function ($element, direction) {
	if (!$element.length) {
		return false;
	}

	var index = this.cp.$current.index();

	var keywordsEnabled = this.cp.$currentKeyword !== undefined;

	// Move slides and keywords.
	if (direction === -1) {
		this.cp.$current.insertBefore($element.removeClass('h5p-previous'));
		if (keywordsEnabled) {
			this.cp.$currentKeyword.insertBefore(this.cp.$currentKeyword.prev());
		}
	}
	else {
		this.cp.$current.insertAfter($element.addClass('h5p-previous'));
		if (keywordsEnabled) {
			this.cp.$currentKeyword.insertAfter(this.cp.$currentKeyword.next());
		}
	}

	if (keywordsEnabled) {
		this.cp.scrollToKeywords();
	}

	// Jump to sorted slide number
	var newIndex = index + direction;
	this.cp.jumpToSlide(newIndex);

	// Need to inform exportable text area about the change:
	H5P.ExportableTextArea.CPInterface.changeSlideIndex(direction > 0 ? index : index-1, direction > 0 ? index+1 : index);

	// Update params.
	this.params.slides.splice(newIndex, 0, this.params.slides.splice(index, 1)[0]);
	this.elements.splice(newIndex, 0, this.elements.splice(index, 1)[0]);
	this.cp.elementInstances.splice(newIndex, 0, this.cp.elementInstances.splice(index, 1)[0]);
	this.cp.elementsAttached.splice(newIndex, 0, this.cp.elementsAttached.splice(index, 1)[0]);

	this.updateNavigationLine(newIndex);

	H5P.ContinuousText.Engine.run(this);

	return true;
};

/**
 * Edit keyword.
 *
 * @param {H5PEditor.$} $span Keyword wrapper.
 * @returns {unresolved} Nothing
 */
H5PEditor.CoursePresentation.prototype.editKeyword = function ($span) {
	var that = this;

	var $li = $span.parent();
	var $ancestor = $li.parent().parent();
	var main = $ancestor.hasClass('h5p-current');

	if (!main && !$ancestor.parent().parent().hasClass('h5p-current')) {
		return false;
	}

	var slideIndex = that.cp.$current.index();
	var $delete = H5PEditor.$('<a href="#" class="h5p-delete-keyword" title="' + H5PEditor.t('H5PEditor.CoursePresentation', 'deleteKeyword') + '"></a>');
	var $textarea = H5PEditor.$('<textarea>' + ($li.hasClass('h5p-empty-keyword') ? '' : $span.text()) + '</textarea>').insertBefore($span.hide()).keydown(function (event) {
		if (event.keyCode === 13) {
			$textarea.blur();
			return false;
		}
	}).keyup(function () {
		$textarea.css('height', 1).css('height', $textarea[0].scrollHeight - 8);
	}).blur(function () {
		var keyword = $textarea.val();

		if (H5P.trim(keyword) === '') {
			$li.addClass('h5p-empty-keyword');
			keyword = H5PEditor.t('H5PEditor.CoursePresentation', 'newKeyword');
		}
		else {
			$li.removeClass('h5p-empty-keyword');
		}

		// Update visuals
		$span.text(keyword).show();
		$textarea.add($delete).remove();

		// Update params
		if (main) {
			that.params.slides[slideIndex].keywords[$li.index()].main = keyword;
		}
		else {
			that.params.slides[slideIndex].keywords[$li.parent().parent().index()].subs[$li.index()] = keyword;
		}
	}).focus();

	$textarea.keyup();

	$delete.insertBefore($textarea).mousedown(function () {
		// Remove keyword
		if (main) {
			that.params.slides[slideIndex].keywords.splice($li.index(), 1);
			$li.add($textarea).remove();
		}
		else {
			// Sub keywords
			var pi = $li.parent().parent().index();
			var $ol = $li.parent();
			if ($ol.children().length === 1) {
				delete that.params.slides[slideIndex].keywords[pi].subs;
				$ol.remove();
			}
			else {
				that.params.slides[slideIndex].keywords[pi].subs.splice($li.index(), 1);
				$li.add($textarea).remove();
			}
		}
	});
};

/**
 * Helper function for traversing a tree of nodes recursively. Invoking callback
 * for each nodes
 *
 * @method traverseChildren
 * @param  {Array}         children
 * @param  {Function}       callback
 */
function traverseChildren(children, callback) {
	if (children !== undefined && children.length !== undefined) {
		for (var i = 0; i < children.length; i++) {
			var child = children[i];
			callback(child);
			traverseChildren(child.children, callback);
		}
	}
}

/**
 * Generate element form.
 *
 * @param {Object} elementParams
 * @param {String} type
 * @returns {Object}
 */
H5PEditor.CoursePresentation.prototype.generateForm = function (elementParams, type) {
	var self = this;

	if (type === 'H5P.ContinuousText' && self.ct) {
		// Continuous Text shares a single form across all elements
		return {
			'$form': self.ct.element.$form,
			children: self.ct.element.children
		};
	}

	// Get semantics for the elements field
	var slides = H5PEditor.CoursePresentation.findField('slides', this.field.fields);
	var elementFields = H5PEditor.$.extend(true, [], H5PEditor.CoursePresentation.findField('elements', slides.field.fields).field.fields);

	// Manipulate semantics into only using a given set of fields
	if (type === 'goToSlide') {
		// Hide all others
		self.showFields(elementFields, ['title', 'goToSlide', 'invisible']);
	}
	else {
		var hideFields = ['title', 'goToSlide', 'invisible'];

		if (type === 'H5P.ContinuousText' || type === 'H5P.Audio') {
			// Continuous Text or Go To Slide cannot be displayed as a button
			hideFields.push('displayAsButton');
		}

		// Only display goToSlide field for goToSlide elements
		self.hideFields(elementFields, hideFields);
	}




	var popupTitle = H5PEditor.t('H5PEditor.CoursePresentation', 'popupTitle', {':type': type.split('.')[1]});
	var element = {
			'$form': H5P.jQuery('<div/>')
	};

	// Find title for form (used by popup dialog)
	self.findElementTitle(type, function (title) {
		element.$form.attr('title', H5PEditor.t('H5PEditor.CoursePresentation', 'popupTitle', {':type': title}));
	});

	// Render element fields
	H5PEditor.processSemanticsChunk(elementFields, elementParams, element.$form, self);
	element.children = self.children;

	// If IV editor - do not show guided tour
	if (H5PEditor.InteractiveVideo) {
		H5PEditor.InteractiveVideo.disableGuidedTour = true;
	}

	// Hide library selector
	element.$form.children('.library:first').children('label, select').hide().end().children('.libwrap').css('margin-top', '0');

	// Set correct aspect ratio on new images.
	// TODO: Do not use/rely on magic numbers!
	var library = element.children[4];
	if (!(library instanceof H5PEditor.None)) {
		var libraryChange = function () {
			if (library.children[0].field.type === 'image') {
				library.children[0].changes.push(function (params) {
					self.setImageSize(element, elementParams, params);
				});
			}
		};
		if (library.children === undefined) {
			library.changes.push(libraryChange);
		}
		else {
			libraryChange();
		}
	}

	return element;
};

/**
 * Help set size for new images and keep aspect ratio.
 *
 * @param {object} element
 * @param {object} elementParams
 * @param {object} fileParams
 */
H5PEditor.CoursePresentation.prototype.setImageSize = function (element, elementParams, fileParams) {
	if (fileParams === undefined || fileParams.width === undefined || fileParams.height === undefined) {
		return;
	}

	// Avoid to small images
	var minSize = parseInt(element.$wrapper.css('font-size')) +
	element.$wrapper.outerHeight() -
	element.$wrapper.innerHeight();

	// Use minSize
	if (fileParams.width < minSize) {
		fileParams.width = minSize;
	}
	if (fileParams.height < minSize) {
		fileParams.height = minSize;
	}

	// Reduce height for tiny images, stretched pixels looks horrible
	var suggestedHeight = fileParams.height / (this.cp.$current.innerHeight() / 100);
	if (suggestedHeight < elementParams.height) {
		elementParams.height = suggestedHeight;
	}

	// Calculate new width
	elementParams.width = (elementParams.height * (fileParams.width / fileParams.height)) / this.slideRatio;
};

/**
 * Hide all fields in the given list. All others are shown.
 *
 * @param {Object[]} elementFields
 * @param {String[]} fields
 */
H5PEditor.CoursePresentation.prototype.hideFields = function (elementFields, fields) {
	// Find and hide fields in list
	for (var i = 0; i < fields.length; i++) {
		var field = H5PEditor.CoursePresentation.findField(fields[i], elementFields);
		if (field) {
			field.widget = 'none';
		}
	}
};

/**
 * Show all fields in the given list. All others are hidden.
 *
 * @param {Object[]} elementFields
 * @param {String[]} fields
 */
H5PEditor.CoursePresentation.prototype.showFields = function (elementFields, fields) {
	// Find and hide all fields not in list
	for (var i = 0; i < elementFields.length; i++) {
		var field = elementFields[i];
		var found = false;

		for (var j = 0; j < fields.length; j++) {
			if (field.name === fields[j]) {
				found = true;
				break;
			}
		}

		if (!found) {
			field.widget = 'none';
		}
	}
};

/**
 * Find the title for the given element type.
 *
 * @param {String} type Element type
 * @param {Function} next Called when we've found the title
 */
H5PEditor.CoursePresentation.prototype.findElementTitle = function (type, next) {
	var self = this;

	if (type === 'goToSlide') {
		// Find field label
		var slides = H5PEditor.CoursePresentation.findField('slides', this.field.fields);
		var elements = H5PEditor.CoursePresentation.findField('elements', slides.field.fields);
		var field = H5PEditor.CoursePresentation.findField(type, elements.field.fields);
		next(field.label);
	}

	else if (type.substring(0,4) === 'H5P.') {
		self.findLibraryTitle(type, next);
	}
	else {
		// Generic
		next(H5PEditor.t('H5PEditor.CoursePresentation', 'element'));
	}
};

/**
 * Find the title for the given library.
 *
 * @param {String} type Library name
 * @param {Function} next Called when we've found the title
 */
H5PEditor.CoursePresentation.prototype.findLibraryTitle = function (library, next) {
	var self = this;

	/** @private */
	var find = function () {
		for (var i = 0; i < self.libraries.length; i++) {
			if (self.libraries[i].name === library) {
				next(self.libraries[i].title);
				return;
			}
		}
	};

	if (self.libraries === undefined) {
		// Must wait until library titles are loaded
		self.once('librariesReady', find);
	}
	else {
		find();
	}
};

/**
 * Callback used by CP when a new element is added.
 *
 * @param {Object} elementParams
 * @param {jQuery} $wrapper
 * @param {Number} slideIndex
 * @param {Object} elementInstance
 * @returns {undefined}
 */
H5PEditor.CoursePresentation.prototype.processElement = function (elementParams, $wrapper, slideIndex, elementInstance) {
	var that = this;

	// Detect type
	var type;
	if (elementParams.action !== undefined) {
		type = elementParams.action.library.split(' ')[0];
	}
	else {
		type = 'goToSlide';
	}

	// Find element identifier
	var elementIndex = $wrapper.index();

	// Generate element form
	if (this.elements[slideIndex] === undefined) {
		this.elements[slideIndex] = [];
	}
	if (this.elements[slideIndex][elementIndex] === undefined) {
		this.elements[slideIndex][elementIndex] = this.generateForm(elementParams, type);
	}

	// Get element
	var element = this.elements[slideIndex][elementIndex];

	seen = []; 

	var replacer = function(key, value) {
		if (value != null && typeof value == "object") {
			if (seen.indexOf(value) >= 0) {
				return;
			}
			seen.push(value);
		}
		return value;
	};




	//console.log("element"+JSON.stringify(element, replacer));
	element.$wrapper = $wrapper;

	H5P.jQuery('<div/>', {
		'class': 'h5p-element-overlay'
	}).appendTo($wrapper);

	if (that.dnb) {
		var options = {};
		if (elementParams.displayAsButton) {
			options.disableResize = true;
		}

		if (type === 'H5P.Image' || (type === 'H5P.Chart' && elementParams.action.params.graphMode === 'pieChart')) {
			options.lock = true;
		}

		that.addToDragNBar(element, elementParams, options);
	}

	// Open form dialog when double clicking element
	$wrapper.dblclick(function () {
		that.showElementForm(element, $wrapper, elementParams,true);
	});

	if (type === 'H5P.ContinuousText' && that.ct === undefined) {
		// Keep track of first CT element!
		that.ct = {
				element: element,
				params: elementParams
		};
	}

	if (elementParams.pasted) {
		if (type === 'H5P.Image') {
			that.setImageSize(element, elementParams, elementParams.action.params.file);
		}
		else if (type === 'H5P.ContinuousText') {
			H5P.ContinuousText.Engine.run(this);
		}
		delete elementParams.pasted;
	}

	if (elementInstance.onAdd) {
		// Some sort of callback event thing
		elementInstance.onAdd(elementParams, slideIndex);
	}
};

/**
 * Make sure element can be moved and stop moving while resizing.
 *
 * @param {Object} element
 * @param {Object} elementParams
 * @param {Object} options
 * @returns {H5P.DragNBarElement}
 */
H5PEditor.CoursePresentation.prototype.addToDragNBar = function(element, elementParams, options) {

	//h5pcustomize - do not attach contextmenu when creating new interactions
	if(elementParams.isEdit == undefined)
		return "";
	var self = this;
	var clipboardData = H5P.DragNBar.clipboardify(H5PEditor.CoursePresentation.clipboardKey, elementParams, 'action');

	var dnbElement = self.dnb.add(element.$wrapper, clipboardData, options);
	dnbElement.contextMenu.on('contextMenuEdit', function () {
		self.showElementForm(element, element.$wrapper, elementParams,true);
	});
	/*
  dnbElement.contextMenu.on('contextMenuRemove', function () {
    if (!confirm(H5PEditor.t('H5PEditor.CoursePresentation', 'confirmRemoveElement'))) {
      return;
    }
    if (H5PEditor.Html) {
      H5PEditor.Html.removeWysiwyg();
    }
    self.removeElement(element, element.$wrapper, (elementParams.action !== undefined && H5P.libraryFromString(elementParams.action.library).machineName === 'H5P.ContinuousText'));
    dnbElement.blur();
  }); */

	dnbElement.contextMenu.on('contextMenuRemove', function () {

		removeInteractionMessage(self,'pre');
		H5P.jQuery('.removingH5pInteraction').click(function(){
			if (H5PEditor.Html) {
				H5PEditor.Html.removeWysiwyg();
			}
			self.removeElement(element, element.$wrapper, (elementParams.action !== undefined && H5P.libraryFromString(elementParams.action.library).machineName === 'H5P.ContinuousText'));
			self.dnb.dialog.close();
			$(".ui-dialog").remove();

		});
		dnbElement.blur();
	});  



	dnbElement.contextMenu.on('contextMenuBringToFront', function () {
		// Old index
		var oldZ = element.$wrapper.index();

		// Current slide index
		var slideIndex = self.cp.$current.index();

		// Update visuals
		element.$wrapper.appendTo(self.cp.$current);

		// Find slide params
		var slide = self.params.slides[slideIndex].elements;

		// Remove from old pos
		slide.splice(oldZ, 1);

		// Add to top
		slide.push(elementParams);

		// Re-order elements in the same fashion
		self.elements[slideIndex].splice(oldZ, 1);
		self.elements[slideIndex].push(element);
	});

	return dnbElement;
};

/**
 * Removes element from slide.
 *
 * @param {Object} element
 * @param {jQuery} $wrapper
 * @param {Boolean} isContinuousText
 * @returns {undefined}
 */
H5PEditor.CoursePresentation.prototype.removeElement = function (element, $wrapper, isContinuousText) {
	var slideIndex = this.cp.$current.index();
	var elementIndex = $wrapper.index();

	var elementInstance = this.cp.elementInstances[slideIndex][elementIndex];
	var removeForm = (element.children.length ? true : false);

	if (isContinuousText) {
		var CTs = this.getCTs(false, true);
		if (CTs.length === 2) {
			// Prevent removing form while there are still some CT elements left
			removeForm = false;

			if (element === CTs[0].element && CTs.length === 2) {
				CTs[1].params.action.params = CTs[0].params.action.params;
			}
		}
		else {
			delete this.params.ct;
			delete this.ct;
		}
	}

	if (removeForm) {
		H5PEditor.removeChildren(element.children);
	}

	// Completely remove element from CP
	if (elementInstance.onDelete) {
		elementInstance.onDelete(this.params, slideIndex, elementIndex);
	}
	this.elements[slideIndex].splice(elementIndex, 1);
	this.cp.elementInstances[slideIndex].splice(elementIndex, 1);
	this.params.slides[slideIndex].elements.splice(elementIndex, 1);

	$wrapper.remove();

	if (isContinuousText) {
		H5P.ContinuousText.Engine.run(this);
	}
};

/**
 * Displays the given form in a popup.
 *
 * @param {jQuery} $form
 * @param {jQuery} $wrapper
 * @param {object} element Params
 * @returns {undefined}
 */
H5PEditor.CoursePresentation.prototype.showElementForm = function (element, $wrapper, elementParams,isEdit) {
	var that = this;

	// Determine element type
	var machineName;
	if (elementParams.action !== undefined) {
		machineName = H5P.libraryFromString(elementParams.action.library).machineName;
	}
	if(machineName == "undefined" || machineName == undefined)
	{
		if(elementParams.goToSlide != undefined)
			machineName = "H5P.GoToSlide";
	}

	//console.log(JSON.stringify(elementParams));
	// Special case for Continuous Text
	var isContinuousText = (machineName === 'H5P.ContinuousText');
	if (isContinuousText && that.ct) {
		// Get CT text from storage
		that.ct.element.$form.find('.text .ckeditor').first().html(that.params.ct);
		that.ct.params.action.params.text = that.params.ct;
	}

	// Disable guided tour for IV
	if (machineName === 'H5P.InteractiveVideo') {
		traverseChildren(element.children, function (elementInstance) {
			if (elementInstance instanceof H5PEditor.InteractiveVideo) {
				elementInstance.disableGuidedTour();

				// Recreate IV form, workaround for Youtube API not firing
				// onStateChange when IV is reopened.
				element = that.generateForm(elementParams, 'H5P.InteractiveVideo');
			}
		});
	}

	var compClassName = machineName.substring(4,machineName.length)+'-CourPres';
	// close existing opened popup if any.It will happen when click on other icon when one is already opened.
	$(".h5p-remove").click();
	$(".ui-dialog").remove();
	that.doneClicked(false); //reset to false so that done click will be enabled
	// Display dialog with form
	var nearby = "";
	if(compClassName == "MarkTheWords-CourPres")
		nearby = "h5p-dragnbar-markthewords-button";
	else if(compClassName == "DragText-CourPres")
		nearby = "h5p-dragnbar-dragtext-button";
	else if(compClassName == "SingleChoiceSet-CourPres")
		nearby = "h5p-dragnbar-singlechoiceset-button";
	else if(compClassName == "Blanks-CourPres")
		nearby = "h5p-dragnbar-blanks-button";
	else if(compClassName == "GoToSlide-CourPres")
		nearby = "h5p-dragnbar-gotoslide-button";
	else if(compClassName == "Audio-CourPres")
		nearby = "h5p-dragnbar-audio-button";
	else if(compClassName == "Video-CourPres")
		nearby = "h5p-dragnbar-video-button";
	else if(compClassName == "Image-CourPres")
		nearby = "h5p-dragnbar-image-button";
	else if(compClassName == "Link-CourPres")
		nearby = "h5p-dragnbar-link-button";
	else if(compClassName == "AdvancedText-CourPres")
		nearby = "h5p-dragnbar-advancedtext-button";




	element.$form.dialog({
		modal: true, 
		draggable: false,
		resizable: false,
		//width: '80%',
		maxHeight: H5P.jQuery('.h5p-coursepresentation-editor').innerHeight(),
		//position: {my: 'top', at: 'top', of: '.h5p-coursepresentation-editor'},
		position:{my: "center top+25", of: '.'+nearby},
		dialogClass: "h5p-dialog-no-close "+compClassName,
		appendTo: '.h5p-course-presentation',
		buttons: [
		          {
		        	  text: H5PEditor.t('H5PEditor.CoursePresentation', 'close'),
		        	  class: 'h5p-remove',
		        	  click: function () {
		        		  //Display the slide in screen after click on done or close
		        		  H5P.jQuery(H5P.jQuery(".h5p-current > div")[(jQObj(".h5p-current > div").size()-1)]).show();


		        		  $(".defaultSkin").not(':last').remove();

		        		  ns.$(".h5p_int_overlay").remove();
		        		  if (H5PEditor.Html) {
		        			  H5PEditor.Html.removeWysiwyg();
		        		  }
		        		  element.$form.dialog('close');
		        		  element.$form.dialog('destroy').remove();
		        		  if(isEdit != true){
		        			  if(H5P.jQuery(".clickonclosedonotremoveinteraction").size() == 0)
		        				  that.removeElement(element, $wrapper, isContinuousText);
		        		  }
		        		  //adding next line - Abhishek
		        		  that.dnb.dialog.close();
		        		  that.dnb.blurAll();

		        		  that.dnb.preventPaste = false;
		        		  return false;
		        	  }
		          },
		          {
		        	  text: H5PEditor.t('H5PEditor.CoursePresentation', 'done'),
		        	  class: 'h5p-done',
		        	  click: function () {
		        		  if(that.doneProcessed == true)
		        			  return false;
		        		   that.doneClicked(true);
		        		  
		        		   
		        		  //  alert(JSON.stringify(elementParams))  ; 
		        		  // Validate children
		        		  var valid = true;
		        		  for (var i = 0; i < element.children.length; i++) {

		        			  if (element.children[i].validate() === false) {
		        				  valid = false;
		        			  }
		        		  }
		        		  if(machineName == "H5P.GoToSlide")
		        		  {
		        		
		        			 
		        			  
		        			  
		        			  var errormsges = ""; 
		        			  var slideTitle = H5P.jQuery("#gotoslide_title").val();
		        			  var slide_value = parseInt(H5P.jQuery("#gotoslide_value").val(),10);
		        			  var maxSlides = parseInt(H5P.jQuery(".h5p-footer-slide-count-max").html(),10);
		        			  var current_slide = parseInt(H5P.jQuery(".h5p-footer-slide-count-current").html(),10);
		        			  
		        			  errormsges += validateH5PTextField("Title",slideTitle);
		        			
		        			  if(isNaN(slide_value) || slide_value < 1 || slide_value > maxSlides)
		        			  {
		        				  errormsges += window.parent.Drupal.t("Goto Slide")+" "+window.parent.Drupal.t("LBL3094");
		        			  }
		        			  else if(H5P.jQuery("#gotoslide_value").val() != slide_value) //Adding for special characters
		        			  {
		        				 errormsges += window.parent.Drupal.t("LBL3096");
		        			  }
		        			 
		        			  if(errormsges != "" && errormsges != undefined && errormsges != "undefined"){
		        				  displayh5pexpertusmsg(errormsges);
		        				  that.doneClicked(false);
		        				  return false;
		        			  }
		        			  
		        			  
		        			  elementParams.goToSlide = slide_value;
		        			  elementParams.title = slideTitle;
		        			  displayh5pexpertusmsg(window.parent.Drupal.t("LBL3021")+" "+window.parent.Drupal.t("LBL3098"));
		        		  }
		        		  else if(machineName == "H5P.AdvancedText")
		        		  {
		        			  var errormsges = "";
		        			  //	var text = H5P.jQuery("#textid").val();

		        			  //var editorData = CKEDITOR.instances.formattextbox_presentation.getData();
		        			  var editorData = tinyMCE.get('edit-short-description-value').getContent();
		        			  errormsges += validateH5PTextField("Text",editorData);
		        			  if(errormsges != "" && errormsges != undefined && errormsges != "undefined"){
		        				  displayh5pexpertusmsg(errormsges);
		        				  
		        				  that.doneClicked(false);
		        				  return false;
		        			  }
		        			  elementParams.action.params.text = editorData;

		        		  }
		        		  else if(machineName == "H5P.ContinuousText")
		        		  {
		        			  var errormsges = ""; 
		        			  var text = H5P.jQuery("#formattextbox").val();
		        			  var comment = H5P.jQuery("#labelid").val();
		        			  errormsges += validateH5PTextField("Text",text);
		        			  if(errormsges != "" && errormsges != undefined && errormsges != "undefined"){
		        				  displayh5pexpertusmsg(errormsges);
		        				 
		        					  that.doneClicked(false);
		        				  return false;
		        			  }
		        			  // alert(text);
		        			  // alert(comment);

		        			  ct = text;
		        			  elementParams.solution = comment;

		        		  }
		        		  else if(machineName == "H5P.Link")
		        		  {
		        			  var errormsges = "";
		        			  var title = H5P.jQuery("#titleid").val();
		        			  var urlid =  H5P.jQuery("#urlid").val();

		        			  errormsges += urlH5PValidate("URL",urlid);
		        			  errormsges += validateH5PTextField("Label",title);
		        			  if(errormsges != "" && errormsges != undefined && errormsges != "undefined"){
		        				  displayh5pexpertusmsg(errormsges);
		        				
		        					  that.doneClicked(false);
		        				  return false;
		        			  }

		        			  elementParams.action.params.linkWidget.protocol="";
		        			  elementParams.action.params.linkWidget.url=urlid;
		        			  elementParams.action.params.title=title;
		        			  //	H5P.jQuery("#show_expertus_message").text("clickonclosedonotremoveinteraction");
		        			  /*	displayh5pexpertusmsg("Interaction is saved.");
				H5P.jQuery("#show_expertus_message").addClass("clickonclosedonotremoveinteraction"); */
		        			  //that.dnb.dialog.close();
		        			  //	element.focus();
		        			  //	return ;


		        			  //	elementParams.action.params.linkWidget.protocol="";
		        			  //	elementParams.action.params.linkWidget.url=urlid;
		        			  //	elementParams.action.params.title=title;
		        			  //	alert("after clicking done");
		        			  //	saveInteractionCommonAPI_CP(that,elementParams,element);	
		        			  // displayh5pexpertusmsg("Interaction is saved.");
		        			  //  H5P.jQuery("#show_expertus_message").text("clickonclosedonotremoveinteraction");
		        		  }
		        		  else if(machineName == "H5P.Image")
		        		  {
		        			  var errormsges = "";
		        			  var title = H5P.jQuery("#labelid").val();
		        			  var hover = H5P.jQuery("#titleid").val();
		        			  var resizeImg = false;

		        			  if(H5P.jQuery("#resizeImg").is(":checked"))
		        				  resizeImg =  true;
		        			  elementParams.action.params.resize = resizeImg;

		        			  elementParams.duration = {"from":"","to":""};
		        			  elementParams.action.params.title = title;
		        			  elementParams.action.params.alt = hover;

		        			  var img = H5P.jQuery("#uploadImg").attr("filename");
		        			  if(img != undefined && img != "")
		        			  {
		        				  var extension =  img.split('.').pop();
		        				  if(extension != "jpg" && extension != "JPG" && extension != "png" && extension != "PNG" && extension != "jpeg" && extension != "JPEG" && extension != "gif" && extension != "GIF"){
		        					  displayh5pexpertusmsg("Upload jpg, png or gif files only");
		        					  
		        						  that.doneClicked(false);
		        					  //  errormsges += "Upload jpg, png or gif files only";
		        					  return "";  
		        				  }
		        			  }


		        			  var processedRes = H5P.jQuery("#uploadImg").attr("processedresponse");
		        			  if(processedRes != null)
		        				  processedRes = JSON.parse(processedRes);

		        			  errormsges = validateH5PTextField("Image",img);
		        			  if(errormsges != "" && errormsges != undefined && errormsges != "undefined"){
		        				  //	alert(errormsges);
		        				  displayh5pexpertusmsg(errormsges);
		        				 
		        					  that.doneClicked(false);
		        				  return false;
		        			  }
		        			  if(processedRes != undefined && processedRes.mime != undefined && processedRes.mime != null)
		        			  {
		        				  /*var height = H5P.jQuery(".h5p-element").height();
				var width =  H5P.jQuery(".h5p-element").width();
				elementParams.action.params.file.heightPX = height;
				elementParams.action.params.file.widthPX = width; */
		        				  elementParams.action.params.file.path = img;
		        				  elementParams.action.params.file.mime = processedRes.mime;
		        				  elementParams.action.params.file.height=processedRes.height;
		        				  elementParams.action.params.file.width=processedRes.width;
		        			  }



		        		  }
		        		  else if(machineName == "H5P.Audio")
		        		  {
		        			  var errormsges = "";
		        			  // 	var title = H5P.jQuery("#labelid").val();
		        			  //	var hover = H5P.jQuery("#titleid").val();
		        			  elementParams.duration = {"from":"","to":""};
		        			  //  	elementParams.action.params.title = title;
		        			  //  	elementParams.action.params.alt = hover;
		        			  var pauseAudio = false;

		        			  if(H5P.jQuery("#pauseAudio").is(":checked"))
		        				  pauseAudio =  true;

		        			  var autoplay = false;

		        			  if(H5P.jQuery("#autoPlay").is(":checked"))
		        				  autoplay =  true;




		        			  var img = H5P.jQuery("#uploadImg").attr("filename");
		        			  var processedRes = H5P.jQuery("#uploadImg").attr("processedresponse");
		        			  if(processedRes != null)
		        				  processedRes = JSON.parse(processedRes);

		        			  errormsges += validateH5PTextField("Audio",img);
		        			  if(errormsges != "" && errormsges != undefined && errormsges != "undefined"){
		        				  displayh5pexpertusmsg(errormsges);
		        				
		        					  that.doneClicked(false);
		        				  return false;
		        			  }
		        			  elementParams.action.params.files[0].path = img;
		        			  if(processedRes != undefined)
		        			  {
		        				  elementParams.action.params.files[0].mime = processedRes.mime;
		        			  }
		        			  elementParams.action.params.controls = pauseAudio;
		        			  elementParams.action.params.autoplay = autoplay;
		        		  }else if(machineName == "H5P.Video")
		        		  {
		        			  var errormsges = "";
		        			  var pauseVideo = false;

		        			  if(H5P.jQuery("#pauseVid").is(":checked"))
		        				  pauseVideo =  true;

		        			  var autoplay = false;

		        			  if(H5P.jQuery("#autoPlayVid").is(":checked"))
		        				  autoplay =  true; 
		        			  var img = H5P.jQuery("#uploadImg").attr("filename");
		        			  var processedRes = H5P.jQuery("#uploadImg").attr("processedresponse");
		        			  if(processedRes != null)
		        				  processedRes = JSON.parse(processedRes);

		        			  errormsges += validateH5PTextField("Video",img);
		        			  if(errormsges != "" && errormsges != undefined && errormsges != "undefined"){
		        				  displayh5pexpertusmsg(errormsges);
		        				
		        					  that.doneClicked(false);
		        				  return false;
		        			  }
		        			  elementParams.action.params.sources[0].path = img;
		        			  if(processedRes != undefined)
		        			  {
		        				  elementParams.action.params.sources[0].mime = processedRes.mime;
		        			  }
		        			  elementParams.action.params.visuals.controls = pauseVideo;
		        			  elementParams.action.params.playback.autoplay = autoplay;
		        		  }
		        		  else if(machineName == "H5P.Summary")
		        		  {
		        			  var errormsges = "";
		        			  var introtext = H5P.jQuery("#taskDescription").val();
		        			  var summaries = H5P.jQuery("#formattextbox").val();

		        			  errormsges += validateH5PTextField("Introduction Text",introtext);
		        			  errormsges += validateH5PTextField("Summary",summaries);
		        			  if(errormsges != "" && errormsges != undefined && errormsges != "undefined"){
		        				  displayh5pexpertusmsg(errormsges);
		        				
		        					  that.doneClicked(false);
		        				  return false;
		        			  }
		        			  elementParams.action.params.intro = introtext;
		        			  elementParams.action.params.summaries[0].summary = [summaries];
		        		  }
		        		  else if(machineName == "H5P.Blanks")
		        		  {

		        			  var errormsges = "";
		        			  var label = ns.$("#labelid").val();
		        			  var text = H5P.jQuery("#questionid").val();
		        			  var questions = [];

		        			  H5P.jQuery(".statements_text").each(function(index,value){
		        				  questions[index]= ns.$(this).val();
		        				  
		        				  checkstar_return = checkstar(questions[index]);		        				  
			        				if((checkstar_return != "" && checkstar_return != undefined && checkstar_return != "undefined"))
			        					errormsges+=checkstar_return;
		        				 
		        				  if((questions[index] == "" || questions[index] == undefined) && (errormsges == "" || errormsges == undefined || errormsges == "undefined"))
		        				  {
		        					  errormsges = geth5pexpertusmsg(window.parent.Drupal.t("LBL325"),window.parent.Drupal.t("ERR261"));
		        				  } 
		        				  else if(questions[index] == "" || questions[index] == undefined){
		        					  errormsges = geth5pexpertusmsg(window.parent.Drupal.t("LBL240"),window.parent.Drupal.t("ERR262"));
		        				  } else if(questions[index].indexOf("**")>=0)
		                     	 {
		                      	 	errormsges = geth5pexpertusmsg(window.parent.Drupal.t('LBL613'),'');
		                      	 }
		        			  });
		        			  
		        			  
		        			
		        			
		        			  var error = validateH5PTextField("Title",text);
		        			  error +=errormsges;
		        			  if(error != "" && error != undefined && error != "undefined"){
		        				  displayh5pexpertusmsg(error);
		        				 
		        					  that.doneClicked(false);
		        				  return false;
		        			  }
		        			  elementParams.label = label;
		        			  elementParams.action.params.text = text;
		        			  elementParams.action.params.questions = questions;

		        		  }
		        		  else if(machineName == "H5P.DragText")
		        		  {
		        			  var errormsges = "";
		        			  var label = ns.$("#labelid").val();
		        			  var taskDescription = H5P.jQuery("#taskDescription").val();
		        			  var editorData = H5P.jQuery("#formattextbox").val();
		        			 
		        			  errormsges += validateH5PTextField("Question",taskDescription);
		        			  errormsges += validateH5PTextField("Text",editorData);
		        			 
		        			  checkstar_return = checkstar(editorData);
		        		
		        				if((checkstar_return != "" && checkstar_return != undefined && checkstar_return != "undefined"))
		        					errormsges+=checkstar_return;
		        			 
		        			  if(errormsges != "" && errormsges != undefined && errormsges != "undefined"){
		        				  displayh5pexpertusmsg(errormsges);
		        				 
		        					  that.doneClicked(false);
		        				  return false;
		        			  }
		        			  elementParams.label = label;
		        			  elementParams.action.params.taskDescription = taskDescription;
		        			  elementParams.action.params.textField = editorData;
		        		  }
		        		  else if(machineName == "H5P.MarkTheWords")
		        		  {
		        			  var errormsges = "";
		        			  var label = ns.$("#labelid").val();
		        			  var taskDescription = H5P.jQuery("#taskDescription").val();
		        			  var editorData = H5P.jQuery("#formattextbox").val();
		        			  

		        			  errormsges += validateH5PTextField("Question",taskDescription);
		        			  errormsges += validateH5PTextField("Text",editorData);
		        			  checkstar_return = checkstar(editorData);
		        			  if((checkstar_return != "" && checkstar_return != undefined && checkstar_return != "undefined"))
		        					errormsges+=checkstar_return;
		        			
		        			  if(errormsges != "" && errormsges != undefined && errormsges != "undefined"){
		        				  displayh5pexpertusmsg(errormsges);
		        				  
		        					  that.doneClicked(false);
		        				  return false;
		        			  }
		        			  elementParams.label = label;
		        			  elementParams.action.params.taskDescription = taskDescription;
		        			  elementParams.action.params.textField = editorData;
		        		  }
		        		  else if(machineName == "H5P.SingleChoiceSet")
		        		  {

		        			  // elementParams = {"x":32.51028806584362,"y":5.835543766578249,"width":40,"height":40,"action":{"library":"H5P.SingleChoiceSet 1.3","params":{"choices":[{"question":"Q1","answers":["A1","A2"]}]},"subContentId":"6b706024-416b-445e-a171-8d52ff2262e2"},"alwaysDisplayComments":false,"backgroundOpacity":0,"displayAsButton":false,"invisible":false};
		        			  var errormsges = "";	
		        			  var error="";
		        			  var qCount = H5P.jQuery(".questionset").size();
		        			  var qarr = new Array();
		        			  var count = 0; 
		        			  H5P.jQuery.each(H5P.jQuery(".questionset"),function(i,val){
		        				  var questionset =H5P.jQuery(this).attr("qset");
		        				  var question = stripTags(H5P.jQuery("."+questionset).find(".ckeditor").html());
		        				  //H5P.jQuery("."+questionset).find(".ckeditor").html(question);


		        				  //	if(question !=  "")
		        				  //	{	
		        				  if(elementParams.action.params.choices[count] == undefined || elementParams.action.params.choices[count].question == undefined)
		        				  {
		        					  var json = {"question":question,"answers":[]};
		        					  elementParams.action.params.choices[count] = json;
		        				  }
		        				  else 
		        				  {	
		        					  elementParams.action.params.choices[count].question = question;
		        				  }

		        				  var answers  = new Array();
		        				  H5P.jQuery.each(H5P.jQuery("."+questionset+" .answercontainerwrapper .ckeditor"),function(i,val){
		        					  answers[i]=stripTags(H5P.jQuery(this).html());

		        					  H5P.jQuery(this).html(answers[i]);
		        					  if((answers[i] == "" || answers[i] == undefined) && (errormsges == "" || errormsges == undefined || errormsges == "undefined"))
		        					  {
		        						  errormsges += geth5pexpertusmsg(window.parent.Drupal.t("LBL387"),window.parent.Drupal.t("ERR261"));
		        					  } 
		        					  else if(answers[i] == "" || answers[i] == undefined){ 
		        						  errormsges = geth5pexpertusmsg(window.parent.Drupal.t("LBL3097"),window.parent.Drupal.t("ERR262"));
		        					  }	

		        				  });
		        				  if(errormsges.indexOf("Question")<0){
		        					  error=validateH5PTextField("Question",question);
		        					  error+=errormsges;
		        				  }

		        				  elementParams.action.params.choices[count].answers = answers;
		        				  //	}
		        				  count++;

		        			  });


		        			  //element.$form.dialog('close');
		        			  //element.$form.dialog('destroy').remove();

		        			  if(error != "" && error != undefined && error != "undefined"){
		        				  displayh5pexpertusmsg(error);
		        				  
		        					  that.doneClicked(false);
		        				  return false;
		        			  }


		        		  }
		        		  //Display the slide in screen after click on done or close h5pcustomize
		        		  //H5P.jQuery(H5P.jQuery(".h5p-current > div")[(jQObj(".h5p-current > div").size()-1)]).show();

		        		  if (isContinuousText) {
		        			  // Store complete CT on slide 0
		        			  that.params.ct = that.ct.params.action.params.text;

		        			  // Split up text and place into CT elements
		        			  H5P.ContinuousText.Engine.run(that);

		        			  setTimeout(function () {
		        				  // Put focus back on ct element
		        				  that.dnb.focus($wrapper);
		        			  }, 1);
		        		  }
		        		  else {

		        			  that.redrawElement($wrapper, element, elementParams);
		        		  }

		        		  if (H5PEditor.Html) {
		        			  H5PEditor.Html.removeWysiwyg();
		        		  }
		        		  that.dnb.preventPaste = false;
		        		  displayh5pexpertusmsg(window.parent.Drupal.t("LBL3021")+" "+window.parent.Drupal.t("LBL3098"));//"Interaction is saved.");
		        		  H5P.jQuery("#show_expertus_message").addClass("clickonclosedonotremoveinteraction");
		        		  
		        	  }
		          }
		          ]
	});
	jQObj = "";
	try
	{
		if(ns == undefined || ns == null)
			jQObj = $;
		else
			jQObj = ns.$;
	}catch(e){
		jQObj = $;
	}
	elementParams.isEdit = isEdit;
	H5P.jQuery(".ui-dialog-content").html("");
	if(machineName == "H5P.GoToSlide")
	{
		elementParams.duration = {"from":"","to":""};
		if(elementParams.isEdit == false)
		{
			elementParams.title = "";
		}
		launchInteractionComponentCP("GotoSlide",elementParams);
	}
	else if(machineName == "H5P.Summary")
	{
		elementParams.duration = {"from":"","to":""};

		if(elementParams.action.params.summaries == undefined || elementParams.action.params.intro == undefined)
			elementParams.action.params = {"intro":"","solvedLabel":"Progress:","scoreLabel":"Wrong answers:","resultLabel":"Your result","response":{"scorePerfect":{"title":"PERFECT!","message":"You got everything correct on your first try!"},"scoreOver70":{"title":"Great!","message":"You got most of the statements correct on your first try."},"scoreOver40":{"title":"Ok","message":"You got some of the statements correct on your first try."},"scoreOver0":{"title":"A few mistakes.","message":"Have another try!"}},"summary":"You got @score of @total statements (@percent %) correct on your first try.","summaries":[{"summary":[""]}]};
		launchInteractionComponentCP("Summary",elementParams);

	}
	else if(machineName == "H5P.AdvancedText")
	{
		elementParams.duration = {"from":"","to":""};
		if(elementParams.action.params.text == undefined)
			elementParams.action.params = {"text":"","contenttype":"presentation"};
		else
			elementParams.action.params.contenttype = "presentation";
		launchInteractionComponentCP("AdvancedText",elementParams);

	}else if(machineName == "H5P.ContinuousText")
	{
		//elementParams.duration = {"from":"","to":""};
		if(elementParams.ct == undefined){
			//	alert(2222222222222222);
			//elementParams = {"ct":"","elements":[{"solution":""}]};
			elementParams = {"ct":"","solution":""};
		}
		launchInteractionComponentCP("ContinuousText",elementParams);

	}else if(machineName == "H5P.Link")
	{
		elementParams.duration = {"from":"","to":""};
		if(elementParams.action.params.linkWidget == undefined || elementParams.action.params.title == undefined)
			elementParams.action.params = {"linkWidget":{"protocol":"","url":""},"title":"","contenttype":"presentation"};
		launchInteractionComponentCP("Link",elementParams);
	}else if(machineName == 'H5P.Image')
	{
		elementParams.duration = {"from":"","to":""};
		if(elementParams.action.params.file == undefined)
			elementParams.action.params = {"contentName":"","file":{"path":"","mime":"","width":"","height":""},"alt":"","title":"","contenttype":"presentation"};
		launchInteractionComponentCP("ImagePre",elementParams);
	}else if(machineName == "H5P.Audio")
	{
		elementParams.duration = {"from":"","to":""};
		if(elementParams.action.params.files == undefined)
			elementParams.action.params = {"fitToWrapper":true,"playerMode":"minimalistic","controls":"","autoplay":"","contentName":"","files":[{"path":"","mime":""}]};

		launchInteractionComponentCP("Audio",elementParams);
	}else if(machineName == "H5P.Video")
	{
		elementParams.duration = {"from":"","to":""};
		if(elementParams.action.params.sources == undefined)
			elementParams.action.params = {"visuals":{"fit":true,"controls":""},"playback":{"autoplay":"","loop":false},"sources":[{"path":"","mime":""}]};
		launchInteractionComponentCP("Video",elementParams);
	}else if(machineName == "H5P.Blanks")
	{
		elementParams.duration = {"from":"","to":""};
		if(elementParams.action.params.text == undefined || elementParams.action.params.questions == undefined)
			elementParams.action.params = {"text":"","questions":"","contenttype":"presentation"};

		launchInteractionComponentCP("FillInTheBlanks",elementParams);
		if(elementParams.action.params.questions != undefined && elementParams.action.params.questions != "")
		{
			for(var i = 0;i < elementParams.action.params.questions.length; i++)
			{
				addStatement(elementParams);
			}
		}
		else{

			addStatement(elementParams);
		}
		vtip();

	}
	else if(machineName == "H5P.DragText")
	{
		elementParams.duration = {"from":"","to":""};
		if(elementParams.action.params.taskDescription == undefined)
			elementParams.action.params = {"taskDescription":"","textField":"","contenttype":"presentation"};
		launchInteractionComponentCP("Mark_the_Words",elementParams);
	}
	else if(machineName == "H5P.MarkTheWords")
	{
		elementParams.duration = {"from":"","to":""};
		if(elementParams.action.params.taskDescription == undefined)
			elementParams.action.params = {"taskDescription":"","textField":"","contenttype":"presentation"};
		launchInteractionComponentCP("Mark_the_Words",elementParams);
	}
	else if(machineName == 'H5P.SingleChoiceSet')
	{

		elementParams.duration = {"from":"","to":""};
		if(elementParams.action.params.choices == undefined){
			elementParams.action.params = {"choices":[{"question":"","answers":[]}],"contenttype":"presentation"};
		}
		else{
			elementParams.action.params.contenttype = "presentation"; 
		}
		// alert(elementParams);
		// console.log(elementParams);
		launchInteractionComponentCP("SingleChoiceSet",elementParams);
		// var html = constructSingleChoiceSet(elementParams);
		// jQObj(".ui-dialog-content").html(html);
	}

	if(isEdit == true){
		//h5pcustomize      
		if(jQObj(".h5p_int_overlay").size() == 0)
			jQObj("<div class='h5p_int_overlay'></div>").insertBefore(jQObj(".h5p-dialog-no-close"));


		jQObj(".h5p-dialog-no-close").css({
			'position': 'fixed'});
		/*'top': parseInt((containerHeight / 2) - (dialogHeight/ 2), 10),
					'left': parseInt((containerWidth / 2) - (dialogWidth / 2), 10),
					//'width':"450px"
					});*/
		jQObj(".h5p-dialog-wrapper").addClass("level-two");
		jQObj(".h5p-dialog-wrapper").css("top","0px");
	}
	else
	{
		jQObj(".h5p-dialog-wrapper").removeClass("level-two");
		jQObj(".h5p-dialog-wrapper").css("top","35px");
		jQObj(".h5p-dialog-wrapper").css("position","fixed");
		jQObj(".h5p-dialog").css("position","");
		jQObj(".h5p_int_overlay").remove();
	}



	if (that.dnb !== undefined) {
		that.dnb.preventPaste = true;
		setTimeout(function () {
			that.dnb.blurAll();
		}, 0);
	}

	var library = element.children[4];
	var focusFirstField = function () {
		// Find the first ckeditor or texteditor field that is not hidden.
		// h5p-editor dialog is copyright dialog
		// h5p-dialog-box is IVs video choose dialog
		H5P.jQuery('.ckeditor, .h5peditor-text', library.$myField)
		.not('.h5p-editor-dialog .ckeditor, ' +
				'.h5p-editor-dialog .h5peditor-text, ' +
				'.h5p-dialog-box .ckeditor, ' +
				'.h5p-dialog-box .h5peditor-text', library.$myField)
				.eq(0)
				.focus();
	};
	if (library instanceof ns.Library && library.currentLibrary === undefined) {
		library.change(focusFirstField);
	}
	else {
		focusFirstField();
	}
	H5P.jQuery("."+compClassName).attr("style","");
	if(isEdit != true){
		H5P.jQuery("."+compClassName).prepend("<div id='vtip'></div>");
	}
	alignCPCommonAttributes(compClassName);
	//Adding for Inner control of Interactions in presentation 
	H5P.jQuery(".h5p-dialog-no-close").find(".ui-dialog-content").attr("style","");
	H5P.jQuery(".h5p-dialog-no-close").find(".ui-dialog-content").addClass("pre-inner-dialog");
};

/**
 *
 */
H5PEditor.CoursePresentation.prototype.redrawElement = function($wrapper, element, elementParams) {
	var elementIndex = $wrapper.index();
	var slideIndex = this.cp.$current.index();
	var elementsParams = this.params.slides[slideIndex].elements;
	var elements = this.elements[slideIndex];
	var elementInstances = this.cp.elementInstances[slideIndex];

	if (elementParams.action && elementParams.action.library.split(' ')[0] === 'H5P.Chart' &&
			elementParams.action.params.graphMode === 'pieChart') {
		elementParams.width = elementParams.height / this.slideRatio;
	}

	// Remove instance of lib:
	elementInstances.splice(elementIndex, 1);

	// Update params
	elementsParams.splice(elementIndex, 1);
	elementsParams.push(elementParams);

	// Update elements
	elements.splice(elementIndex, 1);
	elements.push(element);

	// Update visuals
	$wrapper.remove();

	var instance = this.cp.addElement(elementParams, this.cp.$current, slideIndex);
	var $element = this.cp.attachElement(elementParams, instance, this.cp.$current, slideIndex);

	// Make sure we're inside the container
	this.fitElement($element, elementParams);

	// Resize element.
	instance = elementInstances[elementInstances.length - 1];
	if ((instance.preventResize === undefined || instance.preventResize === false) && instance.$ !== undefined && !elementParams.displayAsButton) {
		H5P.trigger(instance, 'resize');
	}

	var that = this;
	setTimeout(function () {
		// Put focus back on element
		that.dnb.focus($element);
	}, 1);
};

/**
 * Applies the updated position and size properties to the given element.
 *
 * All properties are converted to percentage.
 *
 * @param {H5P.jQuery} $element
 * @param {Object} elementParams
 */
H5PEditor.CoursePresentation.prototype.fitElement = function ($element, elementParams) {
	var self = this;
	var currentSlide = H5P.DragNBar.getSizeNPosition(self.cp.$current[0]);
	var updated = H5P.DragNBar.fitElementInside($element, currentSlide);

	var pW = (currentSlide.width / 100);
	var pH = (currentSlide.height / 100);
	// Set the updated properties
	var style = {};

	if (updated.width !== undefined) {
		elementParams.width = updated.width / pW;
		style.width = elementParams.width + '%';
	}
	if (updated.left !== undefined) {
		elementParams.x = updated.left / pW;
		style.left = elementParams.x + '%';
	}
	if (updated.height !== undefined) {
		elementParams.height = updated.height / pH;
		style.height = elementParams.height + '%';
	}
	if (updated.top !== undefined) {
		elementParams.y = updated.top / pH;
		style.top = elementParams.y + '%';
	}
	//alert(JSON.stringify(style));
//	alert(updated.height);
//	alert(updated.width);

	// Apply style
	$element.css(style);


	//h5pcustomize
//	if(elementParams.action.library == "H5P.Image 1.0")
//	{

//	try
//	{
//	elementParams.action.params.file.heightPX = updated.height;
//	elementParams.action.params.file.widthPX = updated.width;
//	}catch(e){}
//	}
//	else
//	{
////	alert("height_updated:"+updated.height+ "width_updated:"+updated.width);
//	elementParams.action.params.heightPX = updated.height;

//	elementParams.action.params.widthPX = updated.width;

//	}
//	alert($element.HTML());
	if(elementParams.action.library == "H5P.Image 1.0")
	{

		try
		{

			elementParams.action.params.file.heightPX = parseFloat($element.css("height"));
			elementParams.action.params.file.widthPX = parseFloat($element.css("width"));
//			alert( elementParams.action.params.file.heightPX);
//			alert(elementParams.action.params.file.widthPX);
		}catch(e){}
	}

	else // if(element.action.library == "H5P.MarkTheWords 1.5")
	{
		elementParams.action.params.heightPX = parseFloat($element.css("height"));
		elementParams.action.params.widthPX =parseFloat($element.css("width"));
		/*  var total_height = parseInt(H5P.jQuery(".h5p-markthewords-outer-element").last().height());
		var title_height = parseInt(H5P.jQuery(".h5p-markthewords-outer-element").last().find(".h5p-mark-the-words").find(".h5p-question-introduction").height());
		var body_height = parseInt(H5P.jQuery(".h5p-markthewords-outer-element").last().find(".h5p-mark-the-words").find(".h5p-question-content").height());
		var button_height = parseInt(H5P.jQuery(".h5p-markthewords-outer-element").last().find(".h5p-mark-the-words").find(".h5p-question-buttons").height());
		var excess_difference = total_height - (title_height + body_height + button_height);
		//alert("excess_difference11ee"+excess_difference);
		 elementParams.action.params.excess_difference = excess_difference;
	  	console.log("total_height11ee:"+total_height+ "title_height:"+title_height+ "body_height:"+body_height+"button_height:"+button_height+"excess_difference"+excess_difference);
		 */
	}


};

/**
 * Find ContinuousText elements.
 *
 * @param {Boolean} [firstOnly] Return first element only
 * @param {Boolean} [maxTwo] Return after two elements have been found
 * @returns {{Object[]|Object}}
 */
H5PEditor.CoursePresentation.prototype.getCTs = function (firstOnly, maxTwo) {
	var self = this;

	var CTs = [];

	for (var i = 0; i < self.elements.length; i++) {
		var slideElements = self.elements[i];
		if (!self.params.slides[i] || !self.params.slides[i].elements) {
			continue;
		}

		for (var j = 0; slideElements !== undefined && j < slideElements.length; j++) {
			var element = slideElements[j];
			var params = self.params.slides[i].elements[j];
			if (params.action !== undefined && params.action.library.split(' ')[0] === 'H5P.ContinuousText') {
				CTs.push({
					element: element,
					params: params
				});

				if (firstOnly) {
					return CTs[0];
				}
				if (maxTwo && CTs.length === 2) {
					return CTs;
				}
			}
		}
	}

	return firstOnly ? null : CTs;
};

/**
 * Collect functions to execute once the tree is complete.
 *
 * @param {function} ready
 * @returns {undefined}
 */
H5PEditor.CoursePresentation.prototype.ready = function (ready) {
	if (this.passReadies) {
		this.parent.ready(ready);
	}
	else {
		this.readies.push(ready);
	}
};

/**
 * Look for field with the given name in the given collection.
 *
 * @param {String} name of field
 * @param {Array} fields collection to look in
 * @returns {Object} field object
 */
H5PEditor.CoursePresentation.findField = function (name, fields) {
	for (var i = 0; i < fields.length; i++) {
		if (fields[i].name === name) {
			return fields[i];
		}
	}
};

H5PEditor.CoursePresentation.prototype.doneClicked = function (st) {
	this.doneProcessed = st;
};

function constructSingleChoiceSet(interaction)
{
	if(interaction.label == undefined)
		interaction.label = "";
	var html = "";
	html += '<div class="single-choice-container">'; 	
	html += "<div class='questionandanswers'>"; 

	if(interaction.action.params.choices != null)
	{
		for( var i = 0 ; i < interaction.action.params.choices.length;i++)
		{
			html += displayQuestionSet(interaction.action.params.choices[i],i+1);
		}
	}
	else
	{
		html += displayQuestionSet();
	}


	html += "</div>"; //questionandanswer



	html += '</div>';
	//  	html += '</div>'; 

	// vtip(); 
	return html;
}
/*
function displayQuestionSet(quesandans,cnt)
{
	//alert(cnt);
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
	html += '      <label class="h5peditor-label"><span class="h5peditor-label h5peditor-required">Question '+count+':</span></label>';
	html += '      <div class="ckeditor" tabindex="0" contenteditable="true" style="width:390px;margin-right:0px;">'+quesandans.question+'</div>';


	if(count==1)
	{
	html +='<a href="#" class="close_icon_ques_ans" onclick="removeEntity(this);"  style="display:none" >x</a>';
	}
	else {
	html +='<a href="#" class="close_icon_ques_ans" onclick="removeEntity(this);">x</a>';	
	}


	html += '      <div class="h5p-errors"></div>';
	html += '      <div class="h5peditor-field-description question" ></div>';
   	html += '</div>'; 

	html += '<div class="answercontainerwrapper">';
	if(quesandans.answers != "" && quesandans.answers != null)
		for(var i = 0; i <quesandans.answers.length;i++)
		{ 
		//	if(i == 0)
		//		html += addAnswerSet({'count':1,'answer':'(Correct)','questionCount':'','defaultvalue':quesandans.answers[i]});

		//	else if(i == 1)
		//	    html += addAnswerSet({'count':1,'answer':'(Second Option)','questionCount':'','defaultvalue':quesandans.answers[i]});

		//	else
				html += addAnswerSet({'count':1,'answer':'','questionCount':'','defaultvalue':quesandans.answers[i]});
		}
	else
	{
		html += addAnswerSet({'count':1,'answer':'(Correct)','questionCount':'','defaultvalue':''});
		html += addAnswerSet({'count':2,'answer':'(Second Option)','questionCount':'','defaultvalue':''});

	}
  	html += '</div>';

    html += '<div class="addedit-form-cancel-container-actions">';
//	html += '<div class="admin-save-pub-unpub-button-container btn-answer"><div class="admin-save-button-left-bg"></div><input class="addedit-edit-contentauthor-basic-save edit-catalog-course-save-publish addedit-form-expertusone-throbber form-submit ajax-processed" onclick="addAnswerSetWrapper(this,'+count+');" data-wrapperid="contentauthor-addedit-form" tabindex="4" type="button" id="addanswer_btn" name="addanswer" value="Add Answer"><span id="pub-unpub-action-btn" onclick="displayPubActionList()" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn">&nbsp;</span></div>';

	html += '<div class="admin-save-pub-unpub-button-container btn-answer" ><div class="admin-save-button-left-bg"></div><input class="addedit-edit-contentauthor-basic-save edit-catalog-course-save-publish addedit-form-expertusone-throbber form-submit ajax-processed" onclick="addAnswerSetWrapper(this,'+count+');" data-wrapperid="contentauthor-addedit-form" tabindex="4" type="button" id="addquestion_btn" name="addanswer" value="Add Answer"><span id="pub-unpub-action-btn" onclick="CheckFunc(this)" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn">&nbsp;';
	html += '<div class="admin-save-pub-unpub-button-container btn-question" style="display:none"><div class="e1-tip"></div><div class="admin-save-button-left-bg"></div><input class="addedit-edit-contentauthor-basic-save edit-catalog-course-save-publish addedit-form-expertusone-throbber form-submit ajax-processed" onclick="addQuestionSetWrapper(this);" data-wrapperid="contentauthor-addedit-form" tabindex="4" type="button" id="addquestion_btn" name="addanswer" value="Add Question Set"><span id="pub-unpub-action-btn" onclick="displayPubActionList()" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn">&nbsp;</span></div>';
	html += '</span></div>';
  //  html+=displaySingleChoiceSetAction();


	html += '</div>';

	html += "</div>"; //question set end


   	return html;

}
function addAnswerSetWrapper(domobj,count)
{

	var quesandansset = ns.$(domobj).parent().parent().parent().parent();
	var qset = ns.$(domobj).parent().parent().parent().parent();
	//var qset = ns.$(domobj).parent().parent().parent();
	//alert(qset.find(".answer").size());
	params = {'count':qset.find(".answer").size()+1,'answer':''};
	var html = addAnswerSet(params);

	ns.$(".questionset"+count).find(".answercontainer").last().parent().append(html);
	//qset.find(".answercontainer").last().parent().append(html);
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


function addAnswerSet(params)
{
    //alert(params.count);
	if(params == "")
	{
		params = {'count':'','answer':'','defaultvalue':''};
	}
	if(params.defaultvalue == undefined || params.defaultvalue == "")
	  params.defaultvalue = "";
	var html ="";
	html += '<div class="field text h5peditor-interaction-label answercontainer">';

	if(params.answer=='(Correct)')
	{
	html += '      <div class="ans-set">';
	html += '      <label class="ans-label">Answers:</label>';
	html += '      <div class="option-set"><label class="h5peditor-label">Option 1:</label>';// + " " + params.count+'</label>';
	html += '      <div class="ckeditor" tabindex="0" contenteditable="true">'+params.defaultvalue +'</div><span class="ans-correct">'+params.answer+'</span></div>';
	html += '      </div>';
	}

	else if(params.answer=='(Second Option)')
	{
	html += '      <div class="ans-set">';
	html += '      <label class="ans-label">&nbsp;</label>';			
	html += '      <div class="option-set"><label class="h5peditor-label">Option 2:</label>';// + " " + params.count+'</label>';
	html += '      <div class="ckeditor" tabindex="0" contenteditable="true">'+params.defaultvalue+'</div><a href="#" class="close_icon_ques_ans" onclick="removeEntity(this);" style="display:none">x</a></div>';
	html += '      </div>';	
	}


	else{
	html += '      <div class="ans-set">';
	html += '      <label class="ans-label">&nbsp;</label>';			
 //	html += '      <div class="option-set"><label class="h5peditor-label">Option:</label>';// + " " + params.count+'</label>';
	html += '      <div class="option-set"><label class="h5peditor-label">Option '+ params.count+':</label>';
	html += '      <div class="ckeditor" tabindex="0" contenteditable="true">'+params.defaultvalue+'</div><a href="#" class="close_icon_ques_ans" onclick="removeEntity(this);">x</a></div>';
	html += '      </div>';	
	}
	html += '      <div class="h5p-errors"></div>';
	html += '      <div class="h5peditor-field-description answer" > </div>';
   	html += '      </div>';
   	return html;
}
 */
function removeEntity(domobj)
{
	//ns.$(domobj).parent().parent().remove();

	if(ns.$(domobj).hasClass("ansBlock"))
	{
		ns.$(domobj).parent().parent().parent().remove();	

		//reshuffleOptionCount();
		var opt = ns.$(".questionset"+ns.$(domobj).attr("quesNumber")).find(".answercontainerwrapper").find(".answercontainer:nth-child(3)").find(".ans-set .option-set .h5peditor-label").html();

		if(opt == window.parent.Drupal.t('LBL387')+" 4:")
		{
			ns.$(".questionset"+ns.$(domobj).attr("quesNumber")).find(".answercontainerwrapper").find(".answercontainer:nth-child(3)").find(".ans-set .option-set .h5peditor-label").html(window.parent.Drupal.t('LBL387')+" 3:");
		}

	}
	else if(ns.$(domobj).hasClass("quesBlock"))
	{
		ns.$(domobj).parent().parent().remove();
		reshuffleQuestionCount();
	}



}


function convertMinutes(time)
{
	var minutes = Math.floor(time / 60);
	var seconds = (time - minutes * 60);
	return minutes+":"+seconds;
	return "";//minutes.substr(-2) + ":" + seconds.substr(-2);
}

function convertToSeconds(input) {
	var parts = input.split(':'),
	minutes = +parts[0],
	seconds = +parts[1];
	return (minutes * 60 + seconds).toFixed(0);
}


function CheckFunc(obj)
{
	//alert($);
	ns.$(obj).find('.btn-question').show();
//	$('.questionandanswers .addedit-form-cancel-container-actions .admin-save-pub-unpub-button-container.btn-question #addquestion_btn').show();

}












//Tell the editor what widget we are.
H5PEditor.widgets.coursepresentation = H5PEditor.CoursePresentation;

//Add translations
H5PEditor.language["H5PEditor.CoursePresentation"] = {
		"libraryStrings": {
			"confirmDeleteSlide": "Are you sure you wish to delete this slide?",
			"sortSlide": "Sort slide - :dir",
			"backgroundSlide": window.parent.Drupal.t("Set slide background"),
			"removeSlide": window.parent.Drupal.t("Delete current slide"),
			"cloneSlide": window.parent.Drupal.t("Clone current slide"),
			"newSlide": window.parent.Drupal.t("Add new slide"),
			"insertElement": ":type",
			"newKeyword": "New keyword",
			"deleteKeyword": "Remove this keyword",
			"removeElement": "Remove this element",
			"confirmRemoveElement": "Are you sure you wish to remove this element?",
			"cancel": window.parent.Drupal.t("LBL109"),//Cancel
			"done": window.parent.Drupal.t("LBL569"), //"Done", 
			"remove": window.parent.Drupal.t("LBL082"), //"Remove", 
			"keywordsTip": "Drag in keywords using the two buttons above.",
			"popupTitle": "Edit :type",
			"loading": "&nbsp;",
			"keywordsMenu": "Keywords menu",
			"element": "Element",
			"resetToDefault": window.parent.Drupal.t("Reset to default"),
			"resetToTemplate": window.parent.Drupal.t("Reset to template"),
			"slideBackground": window.parent.Drupal.t("Slide background"),
			"setImageBackground": "Image background",
			"setColorFillBackground": window.parent.Drupal.t("Color fill background"),
			"activeSurfaceWarning": "Are you sure you want to activate Active Surface Mode? This action cannot be undone.",
			"template": window.parent.Drupal.t("LBL437"), //"Template",
			"templateDescription": window.parent.Drupal.t("MSG917") +  "\":currentSlide\"" + window.parent.Drupal.t("LBL563") + ".",
			"currentSlide": window.parent.Drupal.t("This slide"),
			"currentSlideDescription": window.parent.Drupal.t("MSG918") +  "\":template\"" + window.parent.Drupal.t("LBL563") + ".",
			"close":window.parent.Drupal.t("LBL123") //"Close"
		}
};

/*function alignCPCommonAttributes(compName) {
  var arr =H5P.jQuery("."+compName).find(".ui-dialog-content").children();
    if(compName == "AdvancedText-CourPres" || compName == "Link-CourPres" || compName == "Image-CourPres") {
      if(H5P.jQuery(".bgrow").size() == 0) {
    	H5P.jQuery(arr[1]).after("<div class='field bgrow advatext'></div>");
		H5P.jQuery(arr[3]).appendTo(".advatext");
		H5P.jQuery(arr[2]).appendTo(".advatext");
		H5P.jQuery(arr[4]).appendTo(".advatext");
      }
    }
}*/


function alignCPCommonAttributes(compName) {
	var arr = H5P.jQuery("."+compName).find(".ui-dialog-content").children();
	var pres = compName;
	switch(pres) {
	case "AdvancedText-CourPres": 
		setClassFroCPCommonAttributes(compName,"advatext");
		break;
	case "ContinuousText-CourPres": 
		setClassFroCPCommonAttributes(compName,"conttext");
		break;	  
	case "Link-CourPres":
		setClassFroCPCommonAttributes(compName,"linkpop");
		break;
	case "Image-CourPres":
		setClassFroCPCommonAttributes(compName,"imagpop");
		break;
	case "Video-CourPres":
		setClassFroCPCommonAttributes(compName,"videpop");
		break;	  
	}
}

function setClassFroCPCommonAttributes(compName,clsName) {
	//console.log("compName:"+compName);
	var arr = H5P.jQuery("."+compName).find(".ui-dialog-content").children();
	if(compName == "ContinuousText-CourPres") {
		if(H5P.jQuery(".bgrow").size() == 0) {
			H5P.jQuery(arr[1]).after("<div class='field bgrow "+clsName+"'></div>");
			H5P.jQuery(arr[3]).appendTo("."+clsName);
			H5P.jQuery(arr[2]).appendTo("."+clsName);
		}
	}
	// else if(){

	//}
	else {
		if(H5P.jQuery("."+compName+" .bgrow").size() == 0) {
			H5P.jQuery(arr[1]).after("<div class='field bgrow "+clsName+"'></div>");
			H5P.jQuery(arr[3]).appendTo("."+clsName);
			H5P.jQuery(arr[2]).appendTo("."+clsName);
			H5P.jQuery(arr[4]).appendTo("."+clsName);
		}
	}
}




function launchInteractionComponentCP(compName,params)
{

	GLOBALPARAMS_H5P = params;

	GLOBALPARAMS_H5P.duration.from = "";//convertMinutes(GLOBALPARAMS_H5P.duration.from);
	GLOBALPARAMS_H5P.duration.to = "";//convertMinutes(GLOBALPARAMS_H5P.duration.to.toString());

	var html = "";
	if(compName == "SingleChoiceSet"){
		//alert("single choice");
		html ="<div class='single-choice-container'>";
	}
	else
		html ="<div>"; 
	//html += new EJS({url: '/sites/all/modules/core/exp_sp_core/modules/h5p/templates/'+compName+'.ejs?ts='+new Date().getTime()}).render(params);
	html += new EJS({url: '/sites/all/modules/core/exp_sp_core/modules/h5p/templates/'+compName+'.ejs'}).render(params);
	html += "</div>";

	H5P.jQuery(".ui-dialog-content").html(html);
	//stripe out html tags
	$("body").on("blur",".striptags",function(e){
		$(this).html(stripTags($(this).html()));
	});

	//hide empty parent interaction when it is popup is open
	if(params.isEdit != true)
		jQObj(jQObj(".h5p-current > div")[(jQObj(".h5p-current > div").size()-1)]).hide();
}



function validateH5PTextField(fieldName,fieldVal)
{	
	if(fieldName == "Text" || fieldName == "Title" || fieldName == "Label" || fieldName == "Question" || fieldName == "Option 1" || fieldName == "Option 2" || fieldName == "Image" || fieldName == "Video" || fieldName == "Audio" || fieldName == "Introduction Text" || fieldName == "Summary")
	{	
		if(fieldName == "Question")
			fieldName = "LBL325";
		else if(fieldName == "Title")
			fieldName = "LBL083";
		else if(fieldName.indexOf("Option")>=0)
			fieldName = window.parent.Drupal.t("LBL387")+ " "+fieldName.split(" ")[1];
		else
			fieldName = window.parent.Drupal.t(fieldName);
		if(fieldVal == "" || fieldVal == undefined)
			return geth5pexpertusmsg(window.parent.Drupal.t(fieldName),window.parent.Drupal.t("ERR101"));
		else
			return "";
	}
}

function geth5pexpertusmsg(fieldname,msg)
{
	return fieldname+" "+msg+" <br/>";
}

function displayh5pexpertusmsg(msg)
{
	ns.$('.e1_h5p_error').html("");
	ns.$('.e1_h5p_error').show();
	var data = '<div id="show_expertus_message"><div><div id="message-container" style="visibility: visible;"><div class="messages error">'+msg+'<div class="msg-close-btn" onclick=\"ns.$(\'.e1_h5p_error\').hide();\"></div></div></div><img  style=\"display:none;\"  src=\"sites/all/themes/core/expertusoneV2/expertusone-internals/images/close.png\" height=\"0\" width=\"0\"></div></div>';
	ns.$(".e1_h5p_error").html(data);
}

function autoplayEvent(obj)
{
	if(H5P.jQuery(obj).is(":checked")){
		H5P.jQuery(obj).parent().removeClass("uncheckAutoplay");
		H5P.jQuery(obj).parent().addClass("checkAutoplay");
	}
	else{
		H5P.jQuery(obj).parent().addClass("uncheckAutoplay");
		H5P.jQuery(obj).parent().removeClass("checkAutoplay");

	}
} 
function urlH5PValidate(fieldName,fieldVal)
{
	//alert("hello333");
	var regexx = new RegExp("^(http|https|ftp)\://([a-zA-Z0-9\.\-]+(\:[a-zA-Z0-9\.&amp;%\$\-]+)*@)*((25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[1-9])\.(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[1-9]|0)\.(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[1-9]|0)\.(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[0-9])|([a-zA-Z0-9\-]+\.)*[a-zA-Z0-9\-]+\.(com|edu|gov|int|mil|net|org|biz|arpa|info|name|pro|aero|coop|museum|[a-zA-Z]{2}))(\:[0-9]+)*(/($|[a-zA-Z0-9\.\,\?\'\\\+&amp;%\$#\=~_\-]+))*$");
	// 	var regexx = /[-a-zA-Z0-9@:%_\+.~#?&//=]{2,256}\.[a-z]{2,4}\b(\/[-a-zA-Z0-9@:%_\+.~#?&//=]*)?/gi;
	var urlregex = new RegExp(regexx);
	if(fieldVal == "" || fieldVal == null || fieldVal == undefined)
		return geth5pexpertusmsg(window.parent.Drupal.t("LBL611"),window.parent.Drupal.t("ERR101"));
	else if (urlregex.test(fieldVal)) {
		return "";
	}
	else 
		return geth5pexpertusmsg(window.parent.Drupal.t("LBL611"),window.parent.Drupal.t("ERR259"));


}
function closeH5PIcon(obj)
{
	ns.$(".h5p-remove").click();
	ns.$(".h5p-dialog-wrapper").removeClass("level-two");
	ns.$(".h5p-dialog-wrapper").css("top","35px");
	ns.$(".h5p_int_overlay").remove();

}
function addStatement($params)
{ 

	var stCount = ns.$(".statements_container .stmt_numcount").size();
	if(stCount == undefined)
		stCount = 1;
	else
		stCount = parseInt(stCount,10)+1;
	GLOBALPARAMS_H5P.stmtcount = stCount;	
	var html = new EJS({url: '/sites/all/modules/core/exp_sp_core/modules/h5p/templates/FillInTheBlanksStatement.ejs'}).render(GLOBALPARAMS_H5P);
	ns.$(".statements_container").append(html);   
}

function deleteStatement(obj)
{
	ns.$(obj).parent().parent().remove();
	reshuffleStatementCount();
}

function reshuffleStatementCount()
{
	var count1 = 1;
	ns.$(".statements_container .stmt_numcount").each(function(){
		ns.$(this).html(count1++);
	});
}


function reshuffleOptionCount(cnt,from)
{
	
		ns.$(".questionset"+cnt).find(".answercontainerwrapper .answercontainer .ans-set .option-set .ansBlock").each(function(){
			
			ns.$(this).attr("quesnumber",cnt);
			//alert("here : " +ns.$(this).attr("quesnumber"));
			
		});
		
	
}

function reshuffleQuestionCount()
{
	var count1 = 1;
	ns.$(".questionandanswers .questionset .questioncontainer .h5peditor-label .h5peditor-label").each(function(){
		var count2 = count1;
		ns.$(this).html(window.parent.Drupal.t('LBL325')+count1++);
		
		//alert("the classes are :: " +ns.$(this).parent().parent().parent().attr("class"));
		ns.$(this).parent().parent().parent().attr('class', 'questionset');
		ns.$(this).parent().parent().parent().attr('qset', "questionset"+count2);
		ns.$(this).parent().parent().parent().addClass("questionset"+count2);
		reshuffleOptionCount(count2,'fromQues');
	});
	
	
	
}




function saveInteractionCommonAPI_CP(that,interactionsCurrent,interaction)
{
	if (that.validDialog(interaction)) 
	{
		displayh5pexpertusmsg(window.parent.Drupal.t("LBL3021")+window.parent.Drupal.t("LBL3098"));
		ns.$("#show_expertus_message").addClass("clickonclosedonotremoveinteraction");
		//that.dnb.dialog.close();
		interaction.focus();
	}
	that.IV.addSliderInteractions();

}
H5PEditor.CoursePresentation.prototype.validDialog = function (element) {
	var valid = true;


	/* var elementKids = interaction.children;
	    for (var i = 0; i < elementKids.length; i++) {
	      if (elementKids[i].validate() === false) {
	        valid = false;
	      }
	    }

	 */
	/*	 element.$form.dialog('close');
     element.$form.dialog('destroy').remove();
     that.dnb.preventPaste = false; */

	if (valid) {
		// Keep form	
		//   interaction.$form.detach();
		element.$form.detach();    

		// Remove interaction from display
		// interaction.remove(true);
		element.$form.dialog('destroy').remove(true);

		// Recreate content instance
		//   interaction.reCreate();
		element.$form.reCreate();

		// Make sure the element is inside the container the next time it's displayed
		//  interaction.fit = true;

		// Check if we should show again
		//    interaction.toggle(this.IV.video.getCurrentTime());

		if (this.dnb) {
			this.dnb.blurAll();
		}
	}

	ns.$(".h5p-interaction-button").css("display","block");
	return valid;
};

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


function removeSlideWrapper(cpObj,index,$remove)
{
	var slideKids = cpObj.elements[index];

	if (slideKids !== undefined) {
		for (var i = 0; i < slideKids.length; i++) {
			cpObj.removeElement(slideKids[i], slideKids[i].$wrapper, cpObj.cp.elementInstances[index][i].libraryInfo && cpObj.cp.elementInstances[index][i].libraryInfo.machineName === 'H5P.ContinuousText');
		}
	}
	cpObj.elements.splice(index, 1);

	// Change slide
	var move = cpObj.cp.previousSlide() ? -1 : (cpObj.cp.nextSlide(true) ? 0 : undefined);
	if (move === undefined) {
		return false; // No next or previous slide
	}

	// ExportableTextArea needs to know about the deletion:
	//h5pcustomize not used exportabletextarea so it is commented
	//H5P.ExportableTextArea.CPInterface.onDeleteSlide(index);

	// Remove visuals.
	$remove.remove();

	// Update presentation params.
	cpObj.params.slides.splice(index, 1);

	// Update the list of element instances
	cpObj.cp.elementInstances.splice(index, 1);
	cpObj.cp.elementsAttached.splice(index, 1);

	cpObj.updateNavigationLine(index + move);

	H5P.ContinuousText.Engine.run(cpObj);
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


