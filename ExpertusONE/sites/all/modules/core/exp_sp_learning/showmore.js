(function ($) {
	$.widget("ui.showmore", {
		options: {
			// showAlways: false,
			showMore: '#jqgrid-show-more',
			grid: null,
			gridWrapper: null,
		},
		_init: function () {
			try {
				// console.log('init here');
				this.refreshShowMore();
				this.attachShowMoreBehavior();
				} catch (e) {
				// console.log(e, e.stack);
			}
		},
		refreshShowMore: function() {
			try {
				// console.log('options = ', options);
				// console.log('this options = ', this.options);
				var _grid = $(this.options.grid);
				var _showMore = $(this.options.showMore);
				// var _showMoreWrapper = $(this.options.showMore).parent();
				var maxRec = _grid.getGridParam("rowNum");
				var actRec = _grid.getGridParam("reccount");
				// console.log('maxRec = ', maxRec, 'actRec = ', actRec, _grid.getGridParam("rowNum"));
				if (maxRec > actRec) {
					_showMore.hide();
				} else {
					_showMore.show();
				}
				_grid.find('tr.ui-widget-content.ui-widget-content-last-row').removeClass('ui-widget-content-last-row');
				_grid.find('tr.ui-widget-content').filter(":last").addClass('ui-widget-content-last-row');
			} catch(e) {
				// console.log(e, e.stack);
			}
		},
		attachShowMoreBehavior: function() {
			try {
				// console.log('attachShowMoreBehavior');
				var sm = this;
				var _showMore = $(this.options.showMore);
				if(_showMore.hasClass('show-more-attached') == false) {
					_showMore.bind('click', function () {
						sm.showMoreData();
					})
					/* .bind('click', function () {
						console.log('test log here');
					}) */
					.addClass('show-more-attached');

				}
			} catch(e) {
				// console.log(e, e.stack);
			}
		},
		showMoreData: function() {
			try {
				// console.log('showMoreData called');
				var obj = this;
				var _grid = $(this.options.grid);
				var _gridWrapper = $(this.options.gridWrapper);
				//return if there is already an ajax request in progress leave doing nothing
				//or return if there are no data to show more
				// console.log('showMoreData', this, this.options, _grid, _gridWrapper);
				// console.log('isAjax', grid.data('isAjax'), 'reccount', grid.getGridParam("reccount"), 'totalrecords', grid.data('totalrecords'));
				// if(grid.data('isAjax') || (grid.getGridParam("reccount") >= _grid.data('totalrecords')) || obj.showAlways) {
				if(_grid.data('isAjax')) {
					return;
				}
				var showMore = $(this.options.showMore);
				var obj = this;
				// options.obj = obj;
				var option = {
					beforeSend: function() {
						// showMore.hide();
						obj.createLoader(_gridWrapper.attr('id'));
						_gridWrapper.find('.loadercontent').css({
							'display': 'table'
						})
						.find('div:not(.loaderimg)').css({'display': 'table-cell', 'vertical-align': 'bottom'})
						.find('td').attr('valign', 'bottom');
						_grid.data('isAjax', true);
					},
					onComplete: function() {
						try {
							_grid.data('isAjax', false);
							obj.destroyLoader(_gridWrapper.attr('id'));
						} catch(e) {
							// console.log(e, e.stack);
						}
					},
					onSuccess: function() {
						// _grid.data('isAjax', false);
					},
					onError: function() {
						// _grid.data('isAjax', false);
						// obj.destroyLoader(_gridWrapper.attr('id'));
					},
					noData: function() {
						// _grid.data('isAjax', false);
						// obj.destroyLoader(_gridWrapper.attr('id'));
						showMore.hide();
						obj.refreshShowMoreDataOnScroll(false);
					},
					beforeAppend: function () {
						// console.log('beforeAppend', _grid.find('tr.ui-widget-content').filter(":last"));
						_grid.find('tr.ui-widget-content').filter(":last").removeClass('ui-widget-content-last-row');
					},
					afterAppend: function (data, count) {
						// console.log(_grid.getGridParam("rowNum"), count);
						if(_grid.getGridParam("rowNum") <= count) {
							showMore.show();
						} else {
							showMore.hide();
							obj.refreshShowMoreDataOnScroll(false);
						}
						// console.log(_grid.find('tr.ui-widget-content').filter(":last"));
						_grid.find('tr.ui-widget-content').filter(":last").addClass('ui-widget-content-last-row');
						// obj.destroyLoader(_gridWrapper.attr('id'));
					},

				};
				//append data to existing result set
				_grid.jqGrid('appendDataToGrid', option);
				obj.refreshShowMoreDataOnScroll(true);
				obj.attachMouseLeaveHandler();
			} catch(e) {
				 // window.console.log(e, e.stack);
			}
		},
		refreshShowMoreDataOnScroll: function(attach) {
			try {
				var obj = this;
				// console.log('refreshShowMoreDataOnScroll ', attach);
				if(attach) {
					if($(window).hasClass('scroll-showmore-attached') == false) {
						$(window).bind('scroll.showmore mousewheel.showmore', function(e) {
							if (typeof lastScrollTop == 'undefined' || lastScrollTop == null) {
								lastScrollTop = $(this).scrollTop();
							} else {
								var st = $(this).scrollTop();
								if (st > lastScrollTop) {
									// downscroll code
									// console.log('scroll Down');
									obj.showMoreDataOnScroll();
								}
								/* else {
									// upscroll code
									// console.log('scroll Up');
								} */
								lastScrollTop = st;
							}
						});
						$(window).addClass('scroll-showmore-attached');
					}
				} else {
					$(window).unbind('scroll.showmore')
					.unbind('mousewheel.showmore')
					.removeClass('scroll-showmore-attached');
					lastScrollTop = null;
				}
			} catch(e) {
				// window.console.log(e, e.stack);
			}
		},
		attachMouseLeaveHandler: function() {
			try {
				var obj = this;
				var _grid = $(this.options.grid);
				var _gridWrapper = $(this.options.gridWrapper);
				if(!_grid.hasClass('mouseleave-showmore-attached')) {
					$(_grid).bind('mouseleave.showmore', function(event) {
						// console.log('target-',$(event.target));
						// console.log('relatedTarget-',$(event.relatedTarget));
						// console.log('relatedTarget-',$(event.target));
						// window.console.log('mouseleave called');
						var comingfrom = event.toElement || event.relatedTarget;
						var isVtip = ($(comingfrom).attr('id') == 'vtip' || $(comingfrom).attr('id') == 'vtipArrow');
						if(!isVtip && $(comingfrom).parents('#'+_gridWrapper.attr('id')).length == 0) {
							obj.refreshShowMoreDataOnScroll(false);
							// $(_grid).unbind('mouseleave.showmore');
						}
					})
					.addClass('mouseleave-showmore-attached');
				}
			} catch(e) {
				// console.log(e, e.stack);
			}
		},
		showMoreDataOnScroll: function(event) {
			try {
				// var obj = event.data.obj;
				// console.log('showMoreDataOnScroll called');
				var obj = this;
				var grid = $(this.options.grid);
				if (grid.is(':hover')) {
				// console.log('showMoreDataOnScroll called hover true');
					// if(grid.getGridParam("reccount") < grid.data('totalrecords')) {
						obj.showMoreData();
						// console.log('showMoreDataOnScroll showMoreData called');
					// }
				}
			} catch(e) {
				// window.console.log(e, e.stack);
			}
		},
	});
	$.extend($.ui.showmore.prototype,EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget,{defaults:{start:true,catStart:true}});
})(jQuery);