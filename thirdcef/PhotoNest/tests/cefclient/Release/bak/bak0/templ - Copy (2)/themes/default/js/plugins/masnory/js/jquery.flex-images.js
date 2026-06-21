/*
    jQuery flexImages v1.0.2
    Copyright (c) 2014 Simon Steinberger / Pixabay
    GitHub: https://github.com/Pixabay/jQuery-flexImages
    License: http://www.sucaijiayuan.com
*/

(function ($) {
    $.fn.flexImages = function (options) {
        var o = $.extend({ container: '.item', object: '.thumbimg', rowHeight: 180, maxRows: 0, truncate: 0 }, options);

        return this.each(function () {
            var grid = $(this), containers = $(grid).find(o.container), items = [], t = new Date().getTime();
            if (containers.length == 0) {
                return;
            }
            var s = window.getComputedStyle ? getComputedStyle(containers[0], null) : containers[0].currentStyle;
            o.margin = (parseInt(s.marginLeft) || 0) + (parseInt(s.marginRight) || 0) + (parseInt(s.borderLeftWidth) || 0) + (parseInt(s.borderRightWidth) || 0);
            for (j = 0; j < containers.length; j++) {
                var c = containers[j],
                    w = parseInt(c.getAttribute('data-w')),
                    h = parseInt(c.getAttribute('data-h')),
                    norm_w = w * (o.rowHeight / h), // normalized width
                    obj = $(c).find(o.object);
                items.push([c, w, h, norm_w, obj]);
            }

            _items = items;

            makeGrid(grid, items, o);
            $(window).off('resize.flexImages' + grid.data('flex-t'));
            $(window).on('resize.flexImages' + t, function () { makeGrid(grid, items, o); });
            grid.data('flex-t', t)
        });
    }

    function makeGrid(grid, items, o, noresize) {
        var x, new_w, ratio = 1, rows = 1, max_w = grid.width() - 30, row = [], row_width = 0, row_h = o.rowHeight;
        if (!max_w) max_w = grid.width() - 30; // IE < 8 bug

        // define inside makeGrid to access variables in scope
        function _helper(lastRow) {
            if (o.maxRows && rows > o.maxRows || o.truncate && lastRow && rows > 1) {
                //                row[x][0].style.display = 'none';
            }
            else {
                if (row[x][4]) {
                    if (row[x][1] < new_w && row[x][2] < row_h) {
                        var obj = $(row[x][0]).find(".thumbdiv");
                        if (obj) {
                            obj.css('width', new_w - 3 + 'px');
                            obj.css('height', row_h + 'px');
                        }
                    }
                    var vdo = row[x][4].hasClass("vdo");
                    if (!vdo && row_h > row[x][2]) {
                        //row[x][4].css('height', row[x][2] + 'px');
                        row[x][4].css('height', row_h + 'px');
                    }
                    else {
                        row[x][4].css('height', row_h + 'px');
                    }
                }
                row[x][0].style.width = new_w + 2 + 'px';
                row[x][0].style.height = row_h + 50 + 'px';
                row[x][0].style.display = 'block';
            }
        }

        var b = false;
        for (i = 0; i < items.length; i++) {
            if ($(items[i][0]).hasClass("ititle")) {
                row_width = 0;
                b = true;
                if (row.length > 0) {
                    //var margins_in_row = row.length * o.margin;
                    //ratio = (max_w - margins_in_row) / (row_width - margins_in_row);
                    ratio = 1;
                    row_h = Math.ceil(o.rowHeight * ratio), exact_w = 0;
                    for (x = 0; x < row.length; x++) {
                        //new_w = Math.floor(row[x][3] * ratio);
                        new_w = Math.floor(row[x][1] * (row_h / row[x][2]));
                        _helper();
                    }
                }
                rows = 1;
                row = [], row_width = 0;
            }
            else {
                row.push(items[i]);
                row_width += items[i][3] + o.margin;

                if (row_width >= max_w /*&& row.length > 1*/) {
                    b = false;
                    var margins_in_row = row.length * o.margin;
                    ratio = (max_w - margins_in_row) / (row_width - margins_in_row), row_h = Math.ceil(o.rowHeight * ratio), exact_w = 0;

                    for (x = 0; x < row.length; x++) {
                        //  new_w = Math.floor(row[x][3] * ratio);
                        new_w = Math.floor(row[x][1] * (row_h / row[x][2]));
                        exact_w += new_w + o.margin;
                        if (exact_w > max_w) new_w -= exact_w - max_w + 1;
                        _helper();
                    }
                    // reset for next row
                    row = [], row_width = 0;
                    rows++;
                }
            }
        }

        _lastobj = row;
        _grid = grid;
        _option = o;

        if (b) {
            ratio = 1;
            row_h = Math.ceil(o.rowHeight * ratio);
        }

        // layout last row - match height of last row to previous row
        for (x = 0; x < row.length; x++) {
            //new_w = Math.floor(row[x][3] * ratio);
            new_w = Math.floor(row[x][1] * (row_h / row[x][2]));
            _helper();
        }

        // scroll bars added or removed during rendering new layout?
        //   if (!noresize && max_w != grid.width()) makeGrid(grid, items, o, true);
    }

    var _lastobj = [];
    var _option;
    var _grid;
    var _items;

    function _append(items) {
        var temp = [];
        for (i = 0; i < items.length; i++) {
            _lastobj.push(items[i]);
            _items.push(items[i]);
        }
        makeGrid(_grid, _lastobj, _option);
    };

    $.fn.extend({
        append_items: function (items) {
            _append(items);
        }
    });

    $.fn.extend({
        refresh_items: function () {
            makeGrid(_grid, _items, _option);
        }
    });

    return this;
}(jQuery));
