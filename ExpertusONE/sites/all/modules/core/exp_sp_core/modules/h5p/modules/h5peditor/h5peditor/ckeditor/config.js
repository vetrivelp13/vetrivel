/**
 * @license Copyright (c) 2003-2015, CKSource - Frederico Knabben. All rights reserved.
 * For licensing, see LICENSE.md or http://ckeditor.com/license
 */

CKEDITOR.editorConfig = function( config ) {
	// Define changes to default configuration here.
	// For complete reference see:
	// http://docs.ckeditor.com/#!/api/CKEDITOR.config

	// The toolbar groups arrangement, optimized for a single toolbar row.
	config.toolbarGroups = [
		//{ name: 'document',	   groups: [ 'mode', 'document', 'doctools' ] },
		//{ name: 'clipboard',   groups: [ 'clipboard', 'undo' ] },
		//{ name: 'editing',     groups: [ 'find', 'selection', 'spellchecker' ] },
		//{ name: 'forms' },
		{ name: 'basicstyles', groups: [ 'basicstyles', 'cleanup' ] },
		//{ name: 'paragraph',   groups: [ 'list', 'indent', 'blocks', 'align', 'bidi' ] },
		//{ name: 'links' },
		{ name: 'insert' },
		{ name: 'styles' },
		{ name: 'colors' }
	//	{ name: 'tools' },
	//	{ name: 'others' },
	//	{ name: 'about' }
	];
	


	// The default plugins included in the basic setup define some buttons that
	// are not needed in a basic editor. They are removed here.
  	   config.removeButtons = 'Cut,Copy,Paste,Undo,Redo,Anchor,Strike,Subscript,Superscript,JustifyCenter,RemoveFormat,NumberedList,BulletedList,About,BGColor,Maximize,Styles,Format,InsertHorizontalLine';

	// Dialog windows are also simplified.
//      config.removeDialogTabs = 'link:advanced';
	config.removeDialogTabs = 'image:advanced;link:advanced';

	// Se the most common block elements.
	config.format_tags = 'p;h1;h2;h3;pre';

	config.autoGrow_minHeight = 50;
  	config.height = 50;
	config.width = 361;
	config.autoGrow_onStartup = true;
	config.autoGrow_maxHeight = 500;
	//config.colorButton_enableMore = true;
	config.plugins += ',removeRedundantNBSP';
	
  	config.resize_enabled = false;
  	config.contentsCss = "/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_contentauthor/exp_sp_administration_contentauthor.css";
	config.startupFocus = true;
//	config.font_names = 'Arial/Arial, Helvetica';
	
};

