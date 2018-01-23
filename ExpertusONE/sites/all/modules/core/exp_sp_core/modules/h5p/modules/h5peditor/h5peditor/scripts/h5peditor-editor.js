/**
 * @namespace
 */
var H5PEditor = (H5PEditor || {});
var ns = H5PEditor;

/**
 * Construct the editor.
 *
 * @class H5PEditor.Editor
 * @param {string} library
 * @param {Object} defaultParams
 */
ns.Editor = function (library, defaultParams, replace) {
  var self = this;
   
  var width = "100%";
 
  if(window.parent.$(".page-administration-contentauthor-presentation").size() > 0)
  	width= "86%";
	
  // Create iframe and replace the given element with it
  var iframe = ns.$('<iframe/>', {
    css: {
      display: 'block',
      width: width,
      height: '3em',
      border: 'none',
      zIndex: 101,
      top: 0,
      left: 0
    },
    'class': 'h5p-editor-iframe',
    frameBorder: '0'
  }).replaceAll(replace).load(function () {
    var $ = this.contentWindow.H5P.jQuery;
    var LibrarySelector = this.contentWindow.H5PEditor.LibrarySelector;
    this.contentWindow.H5P.$body = $(this.contentDocument.body);
    var $container = $('body > .h5p-editor');

  // Load libraries list
	//h5pcustomize - List of libraries is meta data. Not requried to make ajax call to retrieve this. It saves around 1 to 2s of loading time. So commented out ajax below
	
	  var data = [{"name":"H5P.Accordion","title":"Accordion","majorVersion":"1","minorVersion":"0","restricted":false,"tutorialUrl":null},{"name":"H5P.AppearIn","title":"appear.in for Chat and Talk","majorVersion":"1","minorVersion":"0","restricted":false,"tutorialUrl":null},{"name":"H5P.Audio","title":"Audio","majorVersion":"1","minorVersion":"2","restricted":false,"tutorialUrl":null},{"name":"H5P.Boardgame","title":"Board Game","majorVersion":"1","minorVersion":"6","restricted":false,"tutorialUrl":null},{"name":"H5P.Chart","title":"Chart","majorVersion":"1","minorVersion":"0","restricted":false,"tutorialUrl":null},{"name":"H5P.Collage","title":"Collage","majorVersion":"0","minorVersion":"1","restricted":false,"tutorialUrl":null},{"name":"H5P.CoursePresentation","title":"Course Presentation","majorVersion":"1","minorVersion":"9","restricted":false,"tutorialUrl":null},{"name":"H5P.Dialogcards","title":"Dialog Cards","majorVersion":"1","minorVersion":"2","restricted":false,"tutorialUrl":null},{"name":"H5P.DocumentationTool","title":"Documentation Tool","majorVersion":"1","minorVersion":"2","restricted":false,"tutorialUrl":null},{"name":"H5P.DragQuestion","title":"Drag and Drop","majorVersion":"1","minorVersion":"5","restricted":false,"tutorialUrl":null},{"name":"H5P.DragText","title":"Drag Text","majorVersion":"1","minorVersion":"4","restricted":false,"tutorialUrl":null},{"name":"H5P.Blanks","title":"Fill in the Blanks","majorVersion":"1","minorVersion":"4","restricted":false,"tutorialUrl":null},{"name":"H5P.ImageHotspotQuestion","title":"Find the Hotspot","majorVersion":"1","minorVersion":"1","restricted":false,"tutorialUrl":null},{"name":"H5P.Flashcards","title":"Flashcards","majorVersion":"1","minorVersion":"2","restricted":false,"tutorialUrl":null},{"name":"H5P.GreetingCard","title":"Greeting Card","majorVersion":"1","minorVersion":"0","restricted":false,"tutorialUrl":null},{"name":"H5P.GuessTheAnswer","title":"Guess the Answer","majorVersion":"1","minorVersion":"1","restricted":false,"tutorialUrl":null},{"name":"H5P.IFrameEmbed","title":"Iframe Embedder","majorVersion":"1","minorVersion":"0","restricted":false,"tutorialUrl":null},{"name":"H5P.ImageHotspots","title":"Image Hotspots","majorVersion":"1","minorVersion":"3","restricted":false,"tutorialUrl":null},{"name":"H5P.InteractiveVideo","title":"Interactive Video","majorVersion":"1","minorVersion":"9","restricted":false,"tutorialUrl":null},{"name":"H5P.MarkTheWords","title":"Mark the Words","majorVersion":"1","minorVersion":"5","restricted":false,"tutorialUrl":null},{"name":"H5P.MemoryGame","title":"Memory Game","majorVersion":"1","minorVersion":"1","restricted":false,"tutorialUrl":null},{"name":"H5P.MultiChoice","title":"Multiple Choice","majorVersion":"1","minorVersion":"6","restricted":false,"tutorialUrl":null},{"name":"H5P.QuestionSet","title":"Question Set","majorVersion":"1","minorVersion":"6","restricted":false,"tutorialUrl":null,"isOld":true},{"name":"H5P.QuestionSet","title":"Question Set","majorVersion":"1","minorVersion":"7","restricted":false,"tutorialUrl":null},{"name":"H5P.SingleChoiceSet","title":"Single Choice Set","majorVersion":"1","minorVersion":"3","restricted":false,"tutorialUrl":null},{"name":"H5P.Summary","title":"Summary","majorVersion":"1","minorVersion":"4","restricted":false,"tutorialUrl":null},{"name":"H5P.Timeline","title":"Timeline","majorVersion":"1","minorVersion":"0","restricted":false,"tutorialUrl":null},{"name":"H5P.TwitterUserFeed","title":"Twitter User Feed","majorVersion":"1","minorVersion":"0","restricted":false,"tutorialUrl":null}]
	  self.selector = new LibrarySelector(data, library, defaultParams);
      self.selector.appendTo($container.html(''));
      if (library) {
        self.selector.$selector.change();
      }
       
	
   /*
    $.ajax({
      dataType: 'json',
      url:ns.getAjaxUrl('libraries'),// "/?q=h5peditor/197/libraries",//h5peditor/d11a85411b14e/185/libraries",//ns.getAjaxUrl('libraries')
    }).fail(function () {
      $container.html('Error, unable to load libraries.');
    }).done(function (data) {
	  console.log("h5peditor-editor.js dd:"+JSON.stringify(data));
      self.selector = new LibrarySelector(data, library, defaultParams);
      self.selector.appendTo($container.html(''));
      if (library) {
        self.selector.$selector.change();
      }
    });
    */
	
    // Start resizing the iframe
    if (iframe.contentWindow.MutationObserver !== undefined) {
      // If supported look for changes to DOM elements. This saves resources.
      var running;
      var limitedResize = function (mutations) {
        if (!running) {
          running = setTimeout(function () {
            resize();
            running = null;
          }, 40); // 25 fps cap
        }
      };

      new iframe.contentWindow.MutationObserver(limitedResize).observe(iframe.contentWindow.document.body, {
        childList: true,
        attributes: true,
        characterData: true,
        subtree: true,
        attributeOldValue: false,
        characterDataOldValue: false
      });
      H5P.$window.resize(limitedResize);
    }
    else {
      // Use an interval for resizing the iframe
      (function resizeInterval() {
        resize();
        setTimeout(resizeInterval, 40); // No more than 25 times per second
      })();
    }
  }).get(0);
  iframe.contentDocument.open();
  
  iframe.contentDocument.write(
    '<!doctype html><html>' +
    '<head>' +
      ns.wrap('<link rel="stylesheet" href="', ns.assets.css, '">') +
      ns.wrap('<script src="', ns.assets.js, '"></script>') +
    '</head><body>' +
      '<div class="h5p-editor">' + "" + '</div>' + //ns.t('core', 'loading', {':type': 'libraries'})
    '</body></html>');
  iframe.contentDocument.close();
  iframe.contentDocument.documentElement.style.overflow = 'hidden';

  /**
   * Checks if iframe needs resizing, and then resize it.
   *
   * @private
   */
  var resize = function () {
    if (iframe.clientHeight === (iframe.contentDocument != null && iframe.contentDocument.body != null && iframe.contentDocument.body.scrollHeight) &&
        iframe.contentDocument.body.scrollHeight === iframe.contentWindow.document.body.clientHeight) {
      return; // Do not resize unless page and scrolling differs
    }

    // Retain parent size to avoid jumping/scrolling
    //h5pcustomize
    try
    {
    var parentHeight = iframe.parentElement.style.height;
    iframe.parentElement.style.height = iframe.parentElement.clientHeight + 'px';

    // Reset iframe height, in case content has shrinked.
    iframe.style.height = iframe.contentWindow.document.body.clientHeight + 'px';

    // Resize iframe so all content is visible. Use scrollHeight to make sure we get everything
    iframe.style.height = iframe.contentDocument.body.scrollHeight + 'px';

    // Free parent
    iframe.parentElement.style.height = parentHeight;
    }catch(e){}
  };
};

/**
 * Find out which library is used/selected.
 *
 * @alias H5PEditor.Editor#getLibrary
 * @returns {string} Library name
 */
ns.Editor.prototype.getLibrary = function () {
  if (this.selector !== undefined) {
    return this.selector.$selector.val();
  }
};

/**
 * Get parameters needed to start library.
 *
 * @alias H5PEditor.Editor#getParams
 * @returns {Object} Library parameters
 */
ns.Editor.prototype.getParams = function () {

   for(var key in this.selector)
   	console.log("key:"+key+" ==== "+this.selector[key]);
  if (this.selector !== undefined) {
    return this.selector.getParams();
  }
};

/**
 * Editor translations index by library name or "core".
 *
 * @member {Object} H5PEditor.language
 */
ns.language = {};

/**
 * Translate text strings.
 *
 * @method H5PEditor.t
 * @param {string} library The library name(machineName), or "core".
 * @param {string} key Translation string identifier.
 * @param {Object} [vars] Placeholders and values to replace in the text.
 * @returns {string} Translated string, or a text if string translation is missing.
 */
ns.t = function (library, key, vars) {
  if (ns.language[library] === undefined) {
    return 'Missing translations for library ' + library;
  }

  var translation;
  if (library === 'core') {
    if (ns.language[library][key] === undefined) {
      return 'Missing translation for ' + key;
    }
    translation = ns.language[library][key];
  }
  else {
    if (ns.language[library].libraryStrings === undefined || ns.language[library].libraryStrings[key] === undefined) {
      return ns.t('core', 'missingTranslation', {':key': key});
    }
    translation = ns.language[library].libraryStrings[key];
  }

  // Replace placeholder with variables.
  for (var placeholder in vars) {
    if (!vars[placeholder]) {
      continue;
    }
    translation = translation.replace(placeholder, vars[placeholder]);
  }

  return translation;
};

/**
 * Wraps multiple content between a prefix and a suffix.
 *
 * @method H5PEditor.wrap
 * @param {string} prefix Inserted before the content.
 * @param {Array} content List of content to be wrapped.
 * @param {string} suffix Inserted after the content.
 * @returns {string} All content put together with prefix and suffix.
 */
ns.wrap = function (prefix, content, suffix) {
  var result = '';
  for (var i = 0; i < content.length; i++) {
    result += prefix + content[i] + suffix;
  }
  return result;
};


