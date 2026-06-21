/* ----------------------------------
jQuery Timelinr 0.9.4

tested with jQuery v1.6+
©2011 CSSLab.cl
free for any use, of course... :D
instructions: http://www.csslab.cl/2011/08/18/jquery-timelinr/
---------------------------------- */

jQuery.fn.timelinr = function(options) {
	// default plugin settings
	settings = jQuery.extend({
		orientation: 'horizontal',		// value: horizontal | vertical, default to horizontal
		containerDiv: '#timeline',		// value: any HTML tag or #id, default to #timeline
		datesDiv: '#dates',			// value: any HTML tag or #id, default to #dates
		datesSelectedClass: 'selected',			// value: any class, default to selected
		datesSpeed: 500,				// value: integer between 100 and 1000 (recommended), default to 500 (normal)
		issuesSelectedClass: 'selected',			// value: any class, default to selected
		issuesSpeed: 200,				// value: integer between 100 and 1000 (recommended), default to 200 (fast)
		issuesTransparency: 0.2,				// value: integer between 0 and 1 (recommended), default to 0.2
		issuesTransparencySpeed: 500,				// value: integer between 100 and 1000 (recommended), default to 500 (normal)
		arrowKeys: 'false',			// value: true | false, default to false
		startAt: 1,					// value: integer, default to 1 (first)
		autoPlay: 'false',			// value: true | false, default to false
		autoPlayDirection: 'forward',			// value: forward | backward, default to forward
		autoPlayPause: 2000				// value: integer (1000 = 1 seg), default to 2000 (2segs)

	}, options);

	$(function () {
		// setting variables... many of them
		var howManyDates = $(settings.datesDiv + ' li').length;
		var currentDate = $(settings.datesDiv).find('a.' + settings.datesSelectedClass);
		var widthContainer = $(settings.containerDiv).width();
		var heightContainer = $(settings.containerDiv).height();

		var widthDates = $(settings.datesDiv).width();
		var heightDates = $(settings.datesDiv).height();
		var widthDate = $(settings.datesDiv + ' li').width();
		var heightDate = $(settings.datesDiv + ' li').height();

		// set positions!
		if (settings.orientation == 'horizontal') {

			$(settings.datesDiv).width(widthDate * howManyDates).css('marginLeft', widthContainer / 2 - widthDate / 2);
			var defaultPositionDates = parseInt($(settings.datesDiv).css('marginLeft').substring(0, $(settings.datesDiv).css('marginLeft').indexOf('px')));
		} else if (settings.orientation == 'vertical') {

			$(settings.datesDiv).height(heightDate * howManyDates).css('marginTop', heightContainer / 2 - heightDate / 2);
			var defaultPositionDates = parseInt($(settings.datesDiv).css('marginTop').substring(0, $(settings.datesDiv).css('marginTop').indexOf('px')));
		}

		$(settings.datesDiv + ' a').click(function (event) {
			event.preventDefault();
			// first vars
			var whichIssue = $(this).text();
			var currentIndex = $(this).parent().prevAll().length;


			// now moving the dates
			$(settings.datesDiv + ' a').removeClass(settings.datesSelectedClass);
			$(this).addClass(settings.datesSelectedClass);
			if (settings.orientation == 'horizontal') {
				$(settings.datesDiv).animate({'marginLeft': defaultPositionDates - (widthDate * currentIndex)}, {
					queue: false,
					duration: settings.datesSpeed
				});
			} else if (settings.orientation == 'vertical') {
				$(settings.datesDiv).animate({'marginTop': defaultPositionDates - (heightDate * currentIndex)}, {
					queue: false,
					duration: settings.datesSpeed
				});
			}
		});

		currentDate.trigger('click');

	});
};
