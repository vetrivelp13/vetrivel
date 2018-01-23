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
