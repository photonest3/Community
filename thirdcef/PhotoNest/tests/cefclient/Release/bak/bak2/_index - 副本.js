function lazyLoadImages() {
    const images = document.querySelectorAll('img[data-src0]');
    const config = {
        // rootMargin: '0px 0px 50px 0px',
        threshold: 0
    };

    const observer = new IntersectionObserver(function (entries, observer) {
        entries.forEach(function (entry) {
            //if (entry.isIntersecting) 
            {
                const img = entry.target;
                img.src = img.getAttribute('data-src0');
                observer.unobserve(img);
            }
        });
    }, config);

    images.forEach(function (img) {
        observer.observe(img);
    });
}

function single_onload(obj) {
    var h = parseInt($(obj).css('height'));
    var div = $(obj).parents(".single_photo").find(".stackdiv0")[0];
    $(div).css("top", h + 20 + "px");

    var w = parseInt($(obj).css("width"));
    $(obj).css("margin-left", (258 - w) / 2 + 4 + 'px');
    $(obj).show();
}

function stack_onload(obj) {
    var par = $(obj).parent();
    var h1 = parseInt(par.find('img.photo1').css('height'));
    var h2 = parseInt(par.find('img.photo2').css('height'));
    var h3 = parseInt(par.find('img.photo3').css('height'));
    if (h1 > 0 && h2 > 0 && h3 > 0) {
        var h = Math.max(h1, h2, h3);
        $(obj).parent().find(".stackdiv").css("top", h + 20 + "px");
    }

    var w1 = parseInt(par.find('img.photo1').css("width"));
    var w2 = parseInt(par.find('img.photo2').css("width"));
    var w3 = parseInt(par.find('img.photo3').css("width"));
    if (w1 > 0 && w2 > 0 && w3 > 0) {
        var w = Math.max(w1, w2, w3);
        par.find('img.photo1').css("left", (258 - w) / 2 + 8 + 'px');
        par.find('img.photo2').css("left", (258 - w) / 2 + 4 + 'px');
        par.find('img.photo3').css("left", (258 - w) / 2 + 'px');

        par.find('img.photo1').show();
        par.find('img.photo2').show();
        par.find('img.photo3').show();
    }
}

function show_tip(id) {
    var obj = $("#p" + id);
    var t = obj.attr('tip');
    if (!t) {
        obj.attr('tip', '1');
        $("#p" + id).tipTip({
            'delay': 1,
            'fadeIn': 200,
            'fadeOut': 200,
            'maxWidth': '300px',
            'attribute': "mtitle",
            'keepAlive': true,
            'activation': 'click'
        })
    }
    var o2 = $("a[data-id='" + id + "']");
    if (o2.width() + 80 >= o2.parent().width()) {
        o2.parent().removeClass('patit');
    }
}

function mycancel() {
    $("#searchBox").hide();
}

function stopv() {
    var ifm = document.getElementsByClassName("cboxIframe");
    if (ifm.length > 0) {
        var src = ifm[0].src;
        if (src.indexOf("/picture.html") >= 0 || src.indexOf("/index.html?act=index.zoom") >= 0) {
            ifm[0].contentWindow.stopv();
        }
        else {
            document.webkitExitFullscreen();
        }
    }
    else {
        document.webkitExitFullscreen();
    }
}

function on_return() {
    var w = $("input[name=search_allwords]").val();
    if (w == "") {
        return;
    }
    if (window.event.keyCode == 13) {
        submitclick();
    }
}

function submitclick() {
    ccSave();
    var body = $("#catSearch").serialize();
    $.post("./index.html?search=q&filter=" + _opt._filters, body, function (data, state) {
        var info = $.parseJSON(data);
        if (info.stat == "ok") {
            location.href = "./index.html?search=q";
        }
    });

}

function myclear() {
    _opt._mode = "AND";
    _opt._searchin = "search_in_name,search_in_comment,search_in_file";
    _opt._subcats = "";
    _opt._filetype = "";
    _opt._osmtype = "";
    _opt._dimension_type = "widths";
    _opt._datetype = "date_creation";
    _opt._tag_mode = "AND";

    set_checkbox();
    storage.searchopt = JSON.stringify(_opt);

    $("input[name=search_allwords]").val("");
    $('.slider-choice').click();

    if (control1) {
        control1.clear();
    }
    if (control2) {
        control2.clear();
    }
    if (control3) {
        control3.clear();
    }
}

function advfilter() {
    $(".filter-manager-controller-container").toggle();
    if ($(".filter-manager-controller-container").is(":hidden")) {
        val = "0";
        $("#arrd1").html('<i2 class="icon-down-open"></i2>');
    }
    else {
        $("#arrd1").html('<i class="icon-down-open"></i>');
    }
}

function filter_controller() {

    $('#field_words').hide();
    $('#field_album').hide();
    $('#field_type').hide();
    $('#field_date').hide();
    $('#field_osm').hide();
    $('#field_dimension').hide();
    $('#field_author').hide();
    $('#field_tag').hide();

    if (_opt._filters.indexOf('filter_search_use') >= 0) {
        $("input[name='filter_search_use']").prop('checked', true);
        $('#field_words').show();
    }
    if (_opt._filters.indexOf('filter_category_use') >= 0) {
        $("input[name='filter_category_use']").prop('checked', true);
        $('#field_album').show();
    }
    if (_opt._filters.indexOf('filter_filetypes_use') >= 0) {
        $("input[name='filter_filetypes_use']").prop('checked', true);
        $('#field_type').show();
    }
    if (_opt._filters.indexOf('filter_date_use') >= 0) {
        $("input[name='filter_date_use']").prop('checked', true);
        $('#field_date').show();
    }
    if (_opt._filters.indexOf('filter_dimension_use') >= 0) {
        $("input[name='filter_dimension_use']").prop('checked', true);
        $('#field_dimension').show();
    }
    if (_opt._filters.indexOf('filter_authors_use') >= 0) {
        $("input[name='filter_authors_use']").prop('checked', true);
        $('#field_author').show();
    }
    if (_opt._filters.indexOf('filter_tags_use') >= 0) {
        $("input[name='filter_tags_use']").prop('checked', true);
        $('#field_tag').show();
    }
    if (_opt._filters.indexOf('filter_osm_use') >= 0) {
        $("input[name='filter_osm_use']").prop('checked', true);
        $('#field_osm').show();
    }
}

function set_checkbox() {

    $(".dateslider").hide();
    $(".dimensionslider").hide();

    $('.searchin').prop('checked', false);
    $('.filetype').prop('checked', false);
    $('.osmtype').prop('checked', false);

    $("input[name=mode][value=" + _opt._mode + "]").prop('checked', true);

    if (_opt._searchin.indexOf('search_in_name') >= 0) {
        $("input[name='search_in_name']").prop('checked', true);
    }
    if (_opt._searchin.indexOf('search_in_comment') >= 0) {
        $("input[name='search_in_comment']").prop('checked', true);
    }
    if (_opt._searchin.indexOf('search_in_file') >= 0) {
        $("input[name='search_in_file']").prop('checked', true);
    }
    if (_opt._searchin.indexOf('search_in_tags') >= 0) {
        $("input[name='search_in_tags']").prop('checked', true);
    }

    $("input[name='subcats']").prop('checked', (_opt._subcats == "on") ? true : false);

    if (_opt._filetype.indexOf('search_images') >= 0) {
        $("input[name='search_images']").prop('checked', true);
    }
    if (_opt._filetype.indexOf('search_videos') >= 0) {
        $("input[name='search_videos']").prop('checked', true);
    }

    if (_opt._osmtype.indexOf('osm0') >= 0) {
        $("input[name='osm0']").prop('checked', true);
    }
    if (_opt._osmtype.indexOf('osm1') >= 0) {
        $("input[name='osm1']").prop('checked', true);
    }

    $("input[name=date_type][value=" + _opt._datetype + "]").prop('checked', true);
    $("input[name=tag_mode][value=" + _opt._tag_mode + "]").prop('checked', true);
    $("input[name=dimension_type][value=" + _opt._dimension_type + "]").prop('checked', true);

    $("div[data-slider=" + _opt._dimension_type + "]").show();
    $("div[data-slider=" + _opt._datetype + "]").show();

}

function ccSave() {
    var obj = $('input[name=mode]').filter(':checked')[0];
    _opt._mode = $(obj).val();

    obj = $('input[name=tag_mode]').filter(':checked')[0];
    _opt._tag_mode = $(obj).val();

    var searchin = "";
    var objs = $('.searchin').filter(':checked');
    for (var i = 0; i < objs.length; i++) {
        var o = $(objs[i]);
        searchin += o.attr('name') + ',';
    }

    _opt._searchin = searchin;
    _opt._subcats = $("input[name=subcats]").prop('checked') ? "on" : "";

    var filetype = "";
    var objs = $('.filetype').filter(':checked');
    for (var i = 0; i < objs.length; i++) {
        var o = $(objs[i]);
        filetype += o.attr('name') + ',';
    }
    _opt._filetype = filetype;

    var osmtype = "";
    var objs = $('.osmtype').filter(':checked');
    for (var i = 0; i < objs.length; i++) {
        var o = $(objs[i]);
        osmtype += o.attr('name') + ',';
    }
    _opt._osmtype = osmtype;

    storage.searchopt = JSON.stringify(_opt);

}

$.fn.fontCheckbox3 = function () {
    this.find('input[type=checkbox]').on('change', function () {
        $(this).prev().removeClass();
        if (!$(this).is(':checked')) {
            $(this).prev().addClass('icon-check-empty');
        }
        else {
            $(this).prev().addClass('icon-check');
        }
    });
};

function get_pending(ty) {
    $.get("./ws.php?method=categories.get_nb&ty=" + ty, function (data, state) {
        var info = $.parseJSON(data);
        if (info.stat == "err") {
            setTimeout("get_pending('0')", 700);
        }
        else if (info.stat == "ok") {
            var nb = parseInt(info.nb);
            if (nb == 0) {
                $("#pendingtitle").hide();
                $("#menubar_manage").attr("title", "");
            }
            else {
                $("#pendingtitle").show();
                $("#menubar_manage").attr("title", _titlefol);
            }
        }
    });
}

function myactivate(activate) {
    if (activate == "1" && storage.idx == "0") {
        var sub = Math.ceil((new Date().getTime() - storage.curr) / 1000);
        if (sub > 6) {
            setTimeout("get_pending('1')", 100);
        }
    }

    storage.idx = activate;
    if (activate == "0") {
        storage.curr = new Date().getTime();
    }
}

function myscroll(h0, maxPage0) {
    if (maxPage0 > 0) {
        $('.wrapper').infinitescroll({
            loading: {
                msgText: "Loading...",
                img: "./templ/themes/default/images/ajax_loader.gif",
                finishedMsg: 'End of content',
                selector: '#loading'
            },
            state: {
                currPage: 0
            },
            dataType: 'html',
            maxPage: maxPage0,
            prefill: true,
            navSelector: "#pages",
            nextSelector: "#next",
            itemSelector: '.item',
            appendCallback: true,
            path: function (index) {
                return './page.html?page_nb=' + index;
            },
            extraScrollPx: 650
        },
            function (items, data, url) {
                var doms = [];
                for (j = 0; j < items.length; j++) {
                    var c = items[j];
                    w = parseInt(c.getAttribute('data-w'));
                    h = parseInt(c.getAttribute('data-h'));
                    norm_w = w * (h0 / h);

                    obj = $(c).find(".thumbimg");
                    doms.push([c, w, h, norm_w, obj]);
                    mydrag0(obj);
                    $(c).fontCheckbox3();
                }

                _msnry.append_items(doms);

            });
    }
}

function mon_click(url) {
    $.get(url + "&act=index.monbar", function (data, state) {
        var info = $.parseJSON(data);
        if (info.stat == "ok") {
            _details_url = "";
            $('.nb_items').text(info.nb_images);
            $("#Month12").replaceWith(info.bar);
            $(".calendarTitle").replaceWith(info.title);

            if (url.indexOf('details') > 0) {
                _details_url = url;
                _details_url = _details_url.replace("&details=1", "");
                url += "&act=index.details";
            }
            else if (url.indexOf('calendar') > 0) {
                url += "&act=index.cal";
            }
            else if (url.indexOf('weekly') > 0) {
                url += "&act=index.weekly";
            }
            else if (url.indexOf('mlist') > 0) {
                url += "&act=index.mlist";
            }
            else if (url.indexOf('ylist') > 0) {
                url += "&act=index.ylist";
            }
            else {
                url += "&act=index.details";
            }
            gourl(url);
        }
    });
}

function gourl(url) {
    $.get(url, function (data, state) {
        $("#thumb-wap").empty();
        var o = $(data);
        o.fontCheckbox3();

        $("#thumb-wap").html(o);
        if (url.indexOf('details') > 0 || url.indexOf('mlist') > 0 || url.indexOf('ylist') > 0 || url.indexOf('start') > 0) {

            var b = $('#thumb-wap').hasClass("wap99");
            if (!b) {
                $('#trash').css("visibility", "visible");
            }

            _msnry = $('.wrapper').flexImages({ rowHeight: _h0 });
            myscroll(_h0, _maxPage0);
            mydrag0($('#thumb-wap img'));
        }
        else {
            $('#thumb-wap').removeClass("wap81");
            $('#thumb-wap').addClass("wap99");
            $('#trash-wap').removeClass("wap19");
            $('#trash-wap').addClass("wap1");
            $('#trash').css("visibility", "hidden");
        }
        change_tags(url);

        if (document.querySelector('.flex-images')) {
            $('.trash2').show();
        }
        else {
            $('.trash2').hide();
        }

        re_height();

    });
}

function gopic(url) {
    if (_lock == 1) {
        return;
    }
    $('#cboxContent').css('margin-top', '0');
    $('body').css('overflow-y', 'hidden');
    _map = 1;
    $.colorbox({
        transition: "none", width: "100%", height: "100%", fixed: true, iframe: true, opacity: .7,
        href: url,
        onComplete: function () {
            $.colorbox.resize({ innerWidth: window.innerWidth, innerHeight: window.innerHeight });
            $('#cboxLoadedContent').css('padding', '0');
            $('#cboxClose').addClass('colse-map');
            $('#cboxClose').show();
        },
        onClosed: function () {
            _map = 0;
            $('#cboxLoadedContent').css('padding', '1px');
            $('#cboxContent').css('margin-top', '32px');
            $('body').css('overflow-y', 'auto');
            $('#cboxClose').removeClass('colse-map');

            if (_vr_reload == 1) {
                location.reload();
            }
        }
    });
}

function goadmin(url) {
    storage.a = window.location.href;
    window.location = url;
}

function goadmin2() {
    storage.a = window.location.href;
    window.location = _admin_url;
}

function show_paint(id) {
    if (_lock == 1) {
        return;
    }
    window.cefQuery({
        request: 'Prompt.q.paint1:' + id
    });
}

function show_leave(id) {
    if ($("input[data-id='" + id + "']").is(':checked')) {
    }
    else {
        var o2 = $("a[data-id='" + id + "']");
        o2.parent().addClass('patit');
    }
}

function set_bg(id) {
    $.post("./ws.php?method=ws.bg&id=" + id, "bg", function (data, state) {
        var info = $.parseJSON(data);
        if (info.stat == "ok") {
            var bg = "url('" + info.bg + "') center fixed no-repeat border-box ";
            if (_bg == bg) {
                $.post("./ws.php?method=ws.bg&id=-1", "bg", function (data, state) {
                    var info2 = $.parseJSON(data);
                    if (info2.stat == "ok") {
                        _bg = "";
                        clear_bg2();
                    }
                });
            }
            else {
                clear_bg2();
                set_bg2(bg, 1);
            }
        }
    });
}

function set_bg2(bg, ty) {
    _bg = bg;
    if (_bg == "") {
        return;
    }

    if (_skin == "blueberry") {
        $('#qsearchInput').removeClass('qsearchInput1').addClass('qsearchInput2');
    }

    var rgba = 'background-color: transparent !important';
    if (_skin == "dark") {
        $('.nav').css('border', '0');
    }
    else if (_skin == "blueberry") {
    }
    else if (_skin == "splash") {
    }

    $('#qsearchInput').attr('style', rgba);

    //setTimeout(function () {
    $('#nav-wrapper').css('background', _bg);
    $('#nav-wrapper').css('background-size', 'cover');

    rgba = get_header_rgba(_skin);
    $('#h58').css('background', rgba);
    re_height();
    //}, 0);

    if (_bgtype == 0) {

        setTimeout(function () {
            rgba = get_rgba(_skin);
            $('#content').css('background-color', rgba);
            $('#titrecontent').css('background-color', rgba);
            if (_skin == "glacier") {
                $('#titrecontent').css('background-image', 'unset');
            }
            $('#theCategoryPage').css('background', _bg);
            $('#theCategoryPage').css('background-size', 'cover');
        }, 1);
    }
    else {
        if (_skin == "glacier") {
            rgba = get_header_rgba(_skin);
            $('.nav-wrapper').css('background-color', rgba);
        }
    }
}

function clear_bg2() {
    var box = document.getElementById('theCategoryPage');
    box.style.removeProperty('background');

    box = document.getElementById('content');
    box.style.removeProperty('background-color');

    box = document.querySelector('.nav-wrapper');
    box.style.removeProperty('background-color');
    box.style.removeProperty('background');

    box = document.querySelector('.nav');
    box.style.removeProperty('border');

    box = document.querySelector('#titrecontent');
    box.style.removeProperty('background-color');
    box.style.removeProperty('background-image');

    box = document.getElementById('h58');
    box.style.removeProperty('background');

    //$('.do_search').attr('style', '');
    $('#qsearchInput').attr('style', '');
    if (_skin != "newspaper" && _skin != "avocado" && _skin != "glacier" && _skin != "strawberry_jam") {
        $('#qsearchInput').css('border', '0');
    }
    if (_skin == "blueberry") {
        $('#qsearchInput').removeClass('qsearchInput2').addClass('qsearchInput1');
    }

}

function close_paint() {
    window.cefQuery({
        request: 'Prompt.q.closep:'
    });
    $.colorbox.close();
}

function cefQuerySuccess(response) {
    if (response == "") {
        return;
    }
    var ifm = document.getElementsByClassName("cboxIframe");
    if (ifm.length > 0) {
        ifm[0].contentWindow.cefQuerySuccess(response);
    }
}

function jshow_mask(ty) {
    var ifm = document.getElementsByClassName("cboxIframe");
    if (ifm.length > 0) {
        var src = ifm[0].src;
        if (src.indexOf("/picture.html") >= 0 || src.indexOf("osm=1") >= 0) {
            $('#cboxClose').hide();
            ifm[0].contentWindow.jshow_mask(ty);
        }
        return;
    }

    var htm = '<center><a class="paintx" href="javascript:;" onclick="close_paint();return false;">X</a></center>';
    var opy = 0.9;
    if (ty == 1) {
        opy = 0.9;
        htm = '<div class="perror">' + $('.errors0').html() + '<center style="margin-top: 30px;"> <a class="paintx" href="javascript:;" onclick="$.colorbox.close();return false;">X</a></center></div>';
    }

    setTimeout(function () {
        $.colorbox({
            transition: "none", width: 400, height: 200, fixed: true, overlayClose: false, escKey: false, opacity: opy,
            //iframe: true,
            html: htm,
            onComplete: function () {
                $('#colorbox').css('background', 'transparent');
                $('#colorbox').css('box-shadow', 'unset');
                $('#cboxContent').css('background-color', 'transparent');
                $('#cboxLoadedContent').css('background-color', 'transparent');
                $('#cboxClose').hide();
                _vr_mask = 1;
            },
            onClosed: function () {
                var box = document.getElementById('colorbox');
                box.style.removeProperty('background');
                box.style.removeProperty('box-shadow');

                document.getElementById('cboxContent').style.removeProperty('background-color');
                //$('#cboxContent').css('background-color', '#000');
                //$('#cboxLoadedContent').css('background-color', '#f1f1f1');
                $('#cboxClose').show();
                _vr_mask = 0;
            }
        });
    }, 0);
}

function jshow_close() {
    var ifm = document.getElementsByClassName("cboxIframe");
    if (ifm.length > 0) {
        var src = ifm[0].src;
        if (src.indexOf("/picture.html") >= 0 || src.indexOf("osm=1") >= 0) {
            $('#cboxClose').show();
            ifm[0].contentWindow.jshow_close();
            return;
        }
    }
    $.colorbox.close();
}

function reload_img(id) {
    var ifm = document.getElementsByClassName("cboxIframe");
    if (ifm.length > 0) {
        var src = ifm[0].src;
        if (src.indexOf("/picture.html") >= 0) {
            ifm[0].contentWindow.reload_img(id);
        }
    }
    reload_img_help(id, '1');
}

function rotate_img_help(id, angl) {
    var obj = $('#' + id);
    obj.css('transform', 'rotate(' + angl + 'deg)');
}

function reload_img_help(id, iscoi) {
    var obj = $('#' + id);
    var d = new Date();
    var n = d.getTime();

    var src = obj.attr('src');
    var pos = src.indexOf("?d=");
    if (pos > 0) {
        src = src.substring(0, pos);
    }

    if (iscoi == '1') {
        pos = src.indexOf("_coi.png");
        if (pos > 0) {
            src = src + "?d=" + n;
        }
        else {
            src = src + "_coi.png?d=" + n;
        }
    }
    else {
        pos = src.indexOf("_coi.png");
        if (pos > 0) {
            src = src.substring(0, pos);
        }
        src = src + "?d=" + n;
    }
    obj.attr('src', src);
    obj.css('transform', 'rotate(0deg)');
    obj.attr("onload", "AutoResizeImage(" + id + ")");

    $.get("./index.html?act=index.title&id=" + id, function (data, state) {
        var info = $.parseJSON(data);
        if (info.stat == "ok") {
            obj.attr('title', info.q);
        }
    });
}

function AutoResizeImage(id) {
    var obj = $('#' + id);
    obj.removeAttr("onload");

    var w = obj.width();
    var h = obj.height();

    var outW = w;
    var outH = h;

    if (h < _maxHeight) {
        outW = w;
        outH = h;
    }
    else {
        outW = _maxHeight * (w / h);
        outH = _maxHeight;
    }

    var norm_w = outW * (_h0 / outH);

    var o = obj.parents('.item');
    o.attr('data-w', outW);
    o.attr('data-h', outH);
    o.width(norm_w + 2);
}

function viewmap(obj) {
    $('#cboxContent').css('margin-top', '0');
    $('body').css('overflow-y', 'hidden');
    _map = 1;
    $.colorbox({
        transition: "none", width: "100%", height: "100%", fixed: true, iframe: true, opacity: .7,
        href: function () {
            var href = $(obj).attr('data-url');
            return href; // + "&flag=1";
        },
        onComplete: function () {
            $.colorbox.resize({ innerWidth: window.innerWidth, innerHeight: window.innerHeight });
            $('#cboxLoadedContent').css('padding', '0');
            $('#cboxClose').addClass('colse-map');
            // $('#cboxClose').show();
        },
        onClosed: function () {
            _map = 0;
            $('#cboxContent').css('margin-top', '32px');
            $('body').css('overflow-y', 'auto');
            $('#cboxClose').removeClass('colse-map');
        }
    });
}

function art_box(ids) {
    $('#cboxContent').css('margin-top', '0');
    $('body').css('overflow-y', 'hidden');
    _map = 1;
    $.colorbox({
        transition: "none", width: "100%", height: "100%", fixed: true, iframe: true, opacity: .7,
        href: function () {
            if (ids == "") {
                ids = get_photos();
            }
            return "./index.html?act=index.film&ids=" + ids;
        },
        onComplete: function () {
            $.colorbox.resize({ innerWidth: window.innerWidth, innerHeight: window.innerHeight });
            $('#cboxLoadedContent').css('padding', '0');
            $('#cboxClose').addClass('colse-map');
            $('#cboxClose').show();
        },
        onClosed: function () {
            _map = 0;
            $('#cboxContent').css('margin-top', '32px');
            $('body').css('overflow-y', 'auto');
            $('#cboxClose').removeClass('colse-map');
        }
    });
}

function show_filter(catid) {
    var q = $('#qsearchInput').val();
    $.colorbox({
        transition: "none", width: "650", height: "600", fixed: true, iframe: true, maxWidth: 1140, opacity: .7,
        title: _searchTitle,
        href: function () {
            var url = "./index.html?search=1&category=" + catid;
            if (q != '' && q != qsearch_prompt) {
                url += "&search_allwords=" + q;
            }
            return url;
        }
    });
}

function show_star() {
    $('#cmdlevel').toggleClass('pn-icon-star pn-icon-star-empty');
    if (_star == "0") {
        _star = "1";
    }
    else {
        _star = "0";
    }

    $.get("./index.html?act=index.star&star=" + _star, function (data, state) {
        if (_star == "1") {
            var info = $.parseJSON(data);
            if (info.stat == "ok") {
                for (var i = 0; i < info.imgs.length; i++) {
                    var cat = info.imgs[i];
                    if (parseInt(cat.lv) > 80) {
                        var lv = $('#i' + cat.id).attr('data-l');
                        if (lv != cat.lv) {
                            var obj = $('#i' + cat.id).find('img');
                            var tit1 = obj.attr('title');
                            var idx = tit1.indexOf(',');
                            tit1 = tit1.substr(0, idx + 1) + " " + cat.lv;
                            obj.attr('title', tit1);

                            obj = $('#' + cat.id);
                            var tit1 = obj.attr('title');
                            var idx = tit1.indexOf(':');
                            tit1 = tit1.substr(0, idx + 1) + " " + cat.lv + ")";
                            obj.attr('title', tit1);
                        }

                        $('#i' + cat.id).removeClass('nostar');

                    }
                }
            }
        }
        else {
            //$('.divlevel').hide();
            $('.divlevel').addClass('nostar');
        }


    });


}

function is_readonly() {
    if (_readonly != "0") {
        $("#err").html(_err_readonly);
        $('#err').delay(100).show();
        $('#err').delay(2000).fadeOut();
        return true;
    }
    return false;
}

function addto_fav(obj) {
    if (is_readonly()) {
        return;
    }
    var tit = _tit_favorites;

    var ids = get_photos();
    var url = "./picture.html?act=picture.remove_favorites&picid=" + ids;
    if ($(obj).find('.stm-icon').hasClass('stm-icon-favorite-add')) {
        url = "./picture.html?act=picture.add_to_favorites&picid=" + ids;

        tit = _tit_favorites_del;
    }

    $.get(url, function (data, state) {
        $(obj).find('.stm-icon').toggleClass('stm-icon-favorite-add stm-icon-favorite-del');
        $(obj).attr('title', tit);
        if (location.href.indexOf('favorites') >= 0) {
            location.reload();
        }
    });
}

function addcaddie(obj, ty) {
    if (is_readonly()) {
        return;
    }

    var ids = '';
    if (ty == 1) {
        ids = get_photos();
    }

    var tit = _tit_caddie;
    var url = "./index.html?act=index.addcaddie&op=remvoe&ids=" + ids;
    if ($(obj).find('.stm-icon').hasClass('pn-icon-caddie-add')) {
        url = "./index.html?act=index.addcaddie&op=add&ids=" + ids;

        tit = _tit_caddie_del;
    }

    $.get(url, function (data, state) {
        $(obj).find('.stm-icon').toggleClass('pn-icon-caddie-add pn-icon-caddie-del');
        $(obj).attr('title', tit);
        if (location.href.indexOf('caddie') >= 0) {
            location.reload();
        }
    });
}

function item_sel(obj) {
    var txt = $(obj).text();
    var it = $(obj).parent().parent().nextUntil(".ititle");
    var items = it.find('.font-checkbox input[type=checkbox]');
    var j, c, srcid, o;

    if ($(obj).is(':checked')) {
        for (j = 0; j < items.length; j++) {
            c = $(items[j]);
            srcid = c.attr("data-id");
            c.prop('checked', true);
            o = $("input[data-id='" + srcid + "']").parent().find("span");
            o.removeClass('icon-check-empty');
            o.addClass('icon-check');
            var src = $('#' + srcid).attr('src');
            add_trash(srcid, src);
        }
    }
    else {
        for (j = 0; j < items.length; j++) {
            c = $(items[j]);
            srcid = c.attr("data-id");
            c.prop('checked', false);
            o = $("input[data-id='" + srcid + "']").parent().find("span");
            o.removeClass('icon-check');
            o.addClass('icon-check-empty');
            remove_trash(srcid);
            $("a[data-id='" + srcid + "']").parent().addClass('patit');
        }
    }

    items = $("#mydiv").find('img');
    if (items.length > 0) {
        $('#thumb-wap').addClass("wap99");
    }
    else {
        $('#thumb-wap').removeClass("wap99");
    }
    trash_click();
}

function sel_down() {
    var items = $("#mydiv").find('img');
    var nb = items.length;
    var ids = get_photos();

    $.colorbox({
        transition: "none", width: 650, height: 400, fixed: true, iframe: true, overlayClose: false, opacity: .7,
        title: _exportTitle,
        href: "./index.html?act=index.down_box&nb=" + nb + "&ids=" + ids
    });
}

function tags_box() {
    if (is_readonly()) {
        return;
    }

    var items = $("#mydiv").find('img');
    var nb = items.length;
    var ids = get_photos();
    $.colorbox({
        transition: "none", width: 650, height: 400, fixed: true, iframe: true, overlayClose: false, opacity: .7,
        title: _tagsTitle,
        href: "./index.html?act=index.tags_box&nb=" + nb + "&ids=" + ids,
        onClosed: function () { }
    });
}

function empty9(ty) {
    if (is_readonly()) {
        return;
    }

    $.colorbox({
        transition: "none", width: 650, height: 400, fixed: true, iframe: true, overlayClose: false, opacity: .6, closeButton: false,
        href: function () {
            return "./delbox.html?type=" + ty;
        },
        onComplete: function () {
            // $('#cboxWrapper').css({ 'border-top': 'solid 7px #e74c3c' });
            $('#cboxOverlay').css({ 'background': 'slategray' });
            $('#cboxWrapper').css({ 'background-color': 'transparent' });
        },
        onClosed: function () {
            document.getElementById('cboxOverlay').style.removeProperty('background');
            document.getElementById('cboxWrapper').style.removeProperty('background-color');
        }
    });
}

function delabox() {
    if (is_readonly()) {
        return;
    }

    $.colorbox({
        transition: "none", width: 650, height: 400, fixed: true, iframe: true, overlayClose: false, opacity: .6, closeButton: false,
        //title: _tit20,
        href: function () {
            var nb = 0;
            var items = $("#mydiv").find('img');
            nb = items.size();
            var ids = get_photos();
            return "./delbox.html?type=1&nb=" + nb + "&ids=" + ids;
        },
        onComplete: function () {
            // $('#cboxWrapper').css({ 'border-top': 'solid 7px #e74c3c' });
            $('#cboxOverlay').css({ 'background': 'slategray' });
            $('#cboxWrapper').css({ 'background-color': 'transparent' });
        },
        onClosed: function () {
            document.getElementById('cboxOverlay').style.removeProperty('background');
            document.getElementById('cboxWrapper').style.removeProperty('background-color');
        }
    });
}

function playbox(url) {
    $.colorbox({
        href: url, transition: "none", width: "85%", height: "85%", fixed: true, opacity: 1, //maxWidth: 1140,
        onComplete: function () {
            $('body').css('overflow-y', 'hidden');
            $('#cboxLoadedContent').css('background', '#000');
            $('#cboxOverlay').css('background-color', '#000');
            $('#cboxWrapper').css('background-color', '#000');
            $('#cboxClose').removeClass('stm-icon-cancel2').addClass('stm-icon-cancel3');

        },
        onClosed: function () {
            document.body.style.removeProperty('overflow-y');
            document.getElementById('cboxOverlay').style.removeProperty('background-color');
            document.getElementById('cboxWrapper').style.removeProperty('background-color');
            $('#cboxClose').removeClass('stm-icon-cancel3').addClass('stm-icon-cancel2');
        }
    });
}

function mbprofile() {
    var width0 = 437;
    var height0 = 270;
    if (_album == "private") {
        width0 = 650;
        height0 = 400;
    }

    $.colorbox({
        href: "./index.html?act=index.profile2",
        transition: "none", width: width0, height: height0, iframe: true, fixed: true, maxWidth: 1140, opacity: .7,
        title: (_album == "private") ? _profiletitle : _profiletitle2
    });
}

function mbskins() {
    $.colorbox({
        href: "./index.html?act=index.skins",
        transition: "none", width: "95%", height: "95%", iframe: true, fixed: true, maxWidth: 1140, maxHeight: 700, opacity: .7,
        title: _skintitle
    });
}

function change_tags(url) {
    $.post("./ws.php?method=ws.menubartags", "src=" + encodeURIComponent(url), function (data, state) {
        var info = $.parseJSON(data);
        if (info.stat == "ok") {
            $('#menubar_tags').remove();
            if (info.htm != "") {
                $('#menubar_categories').after(info.htm);
            }
        }
    });
}

function get_photos() {
    var ids = "";
    var items = $("#mydiv").find('img');
    for (var i = 0; i < items.length; i++) {
        var item = items[i];
        var id = $(item).attr("data-id");
        ids += id + ",";
    }
    if (ids != "") {
        ids = ids.substr(0, ids.length - 1);
    }
    return ids;
}

function is_ifr() {
    var ifm = document.getElementsByClassName("cboxIframe");
    if (ifm.length > 0) {
        $.colorbox.close();
        return true;
    }
    return false;
}

function mygoback() {
    if (_pic == 1) {
        var ifm = document.getElementsByClassName("cboxIframe");
        if (ifm.length > 0) {
            ifm[0].contentWindow.mygoback();
            return;
        }
    }
    if (_vr_mask == 1) {
        return;
    }

    if (is_ifr()) {
        return;
    }
    if (_details_url != "") {
        mon_click(_details_url);
        _details_url = "";
        return;
    }
    history.back();
}

function mygoforward() {
    if (is_ifr() || _vr_mask == 1) {
        return;
    }
    history.forward();
}

function myreload() {
    if (_pic == 1) {
        var ifm = document.getElementsByClassName("cboxIframe");
        if (ifm.length > 0) {
            ifm[0].contentWindow.myreload();
            return;
        }
    }
    if (_vr_mask == 1) {
        return;
    }

    var ifm = document.getElementsByClassName("cboxIframe");
    if (ifm.length > 0) {
        var src = ifm[0].src;
        //if (src.indexOf("&net=1") >= 0) {
        //    $.colorbox.close();
        //    return true;
        //}
        if (src.indexOf("/picture.html") >= 0 || src.indexOf("&osm=1") >= 0 ||
            src.indexOf("index.wall") >= 0 || src.indexOf("index.film") >= 0 || src.indexOf("index.zoom") >= 0) {
            var pos = src.indexOf("&d=");
            if (pos > 0) {
                src = src.substring(0, pos);
            }
            var d = new Date();
            var n = d.getTime();
            ifm[0].src = src + "&d=" + n;
            return;
        }
        else //if (src.indexOf("index.profile2") >= 0) 
        {
            $.colorbox.close();
            return true;
        }
    }
    location.reload();
}

function trash_cleanup() {
    $('#thumb-wap').removeClass("wap81");
    $('#thumb-wap').addClass("wap99");
    $('#trash-wap').removeClass("wap19");
    $('#trash-wap').addClass("wap1");
    $('#trash').css("visibility", "hidden");

    //$(".all_none").text(_selectAll);
    $('.trash2').show();

    if (_resized == 1) {
        _resized = 0;
        _msnry = $('.wrapper').flexImages({ rowHeight: _h0 });
    }
}

function trash_click() {
    var b = $('#thumb-wap').hasClass("wap99");
    if (b) {
        trash_show();
    }

    if (_resized == 1) {
        _resized = 0;
        _msnry = $('.wrapper').flexImages({ rowHeight: _h0 });
    }
}

function trash_show() {
    $('#thumb-wap').removeClass("wap99");
    $('#thumb-wap').addClass("wap81");
    $('#trash-wap').removeClass("wap1");
    $('#trash-wap').addClass("wap19");
    $('#trash').css("visibility", "visible");

    $('.trash2').hide();
}

function add_trash(srcid, src) {
    var b = false;
    var items = $("#mydiv").find('img');
    for (var i = 0; i < items.length; i++) {
        var item = items[i];
        var id = $(item).attr("data-id");
        if (id == srcid) {
            b = true;
            break;
        }
    }

    if (!b) {
        var htm = '<li><img class="selitem ui-draggable imgbg" data-id="' + srcid + '" title="' + _title3 + '" src="' + src + '" /></li>';
        if (src.indexOf('/images/mp4.png') > 0 || src.indexOf('/images/mp3.png') > 0) {
            htm = '<li><img class="selitem ui-draggable" data-id="' + srcid + '" src="' + src + '" /></li>';
        }
        var r = $(htm);
        var ff = r.find('.selitem');

        $("input[data-id='" + srcid + "']").prop('checked', true);
        var o = $("input[data-id='" + srcid + "']").parent().find("span");
        o.removeClass('icon-check-empty');
        o.addClass('icon-check');

        $("#mydiv").append(r);
        $('#p' + srcid).parent().removeClass('patipshow');

        var o2 = $("a[data-id='" + srcid + "']");
        if (o2.width() + 80 >= o2.parent().width()) {
            o2.parent().removeClass('patit');
        }
    }
    set_sel_title();
}

function remove_trash(srcid) {
    var items = $("#mydiv").find('img');
    for (var i = 0; i < items.length; i++) {
        var item = items[i];
        var id = $(item).attr("data-id");
        if (id == srcid) {
            $("input[data-id='" + srcid + "']").prop('checked', false);
            var o = $("input[data-id='" + srcid + "']").parent().find("span");
            o.removeClass('icon-check');
            o.addClass('icon-check-empty');

            $(item).parent().remove();
            $('#p' + srcid).parent().addClass('patipshow');
            break;
        }
    }
    set_sel_title();
}

function setText(obj) {
    var srcid = $(obj).attr("data-id");
    if ($(obj).is(':checked')) {
        var src = $('#' + srcid).attr('src');
        add_trash(srcid, src);
    }
    else {
        remove_trash(srcid);
    }

    var items = $("#mydiv").find('img');
    if (items.length > 0) {
        $('#thumb-wap').addClass("wap99");
    }
    else {
        $('#thumb-wap').removeClass("wap99");
    }

    trash_click();
}

function set_sel_title() {
    var items = $("#mydiv").find('img');
    var nb = items.size();
    if (nb == 0) {
        $("#seltitle").text(_title0);
        $('.selbtn .sel').css({ 'pointer-events': 'none', 'opacity': '.7' });
    }
    else if (nb == 1) {
        $("#seltitle").text(_title1);
        $('.selbtn .sel').css({ 'pointer-events': 'auto', 'opacity': '1' });
    }
    else {
        var t = _title2;
        t = t.replace("%d", nb);
        $("#seltitle").text(t);
        $('.selbtn .sel').css({ 'pointer-events': 'auto', 'opacity': '1' });
    }
}

function mydrag0(ele) {
    ele.draggable({
        helper: 'clone',
        opacity: 0.7,
        zIndex: 5,
        containment: 'document',
        start: function () {
            trash_show();
        }
    });
}

function selimg(obj) {
    var srcid = $(obj).attr("data-id");
    var chk = $("input[data-id='" + srcid + "']");
    var o = $("input[data-id='" + srcid + "']").parent().find("span");
    if (!chk.is(':checked')) {
        $("input[data-id='" + srcid + "']").prop('checked', true);
        o.removeClass('icon-check-empty');
        o.addClass('icon-check');
        var src = $('#' + srcid).attr('src');
        add_trash(srcid, src);
    }
    else {
        remove_trash(srcid);
    }

    var items = $("#mydiv").find('img');
    if (items.length > 0) {
        $('#thumb-wap').addClass("wap99");
    }
    else {
        $('#thumb-wap').removeClass("wap99");
    }
    trash_click();
}

function logout() {
    $.getJSON('./index.html?act=index.logout', function (data) {
        location.replace("./a.html");
    });
}

window.onunload = function () {
    $('body').css('overflow-y', 'auto');
};

function get_header_rgba(skin) {
    var rgba = 'rgba(255,255,255,' + _skinOpacity + ')';
    if (skin == "avocado") {
        //#65A603
        rgba = 'rgba(101,166,3,' + _skinOpacity + ')';
    }
    else if (skin == "blueberry") {
        //#16262e
        rgba = 'rgba(22,38,46,' + _skinOpacity + ')';
    }
    else if (skin == "cafe_latte") {
        //#C0B283
        rgba = 'rgba(192,178,131,' + _skinOpacity + ')';
    }
    else if (skin == "dark") {
        //#141414
        rgba = 'rgba(63,63,63,' + _skinOpacity + ')';
    }
    else if (skin == "glacier") {
        //#1E3259
        rgba = 'rgba(30,50,89,' + _skinOpacity + ')';
    }
    else if (skin == "neon_orange") {
        //#ff7700
        rgba = 'rgba(255,119,0,' + _skinOpacity + ')';
    }
    else if (skin == "neon_pink") {
        //#FF2F63
        rgba = 'rgba(255,47,99,' + _skinOpacity + ')';
    }
    else if (skin == "newspaper") {
        //#2D2D2D
        rgba = 'rgba(45,45,45,' + _skinOpacity + ')';
    }
    else if (skin == "quartz") {
        //#1E1F23
        rgba = 'rgba(30,31,35,' + _skinOpacity + ')';
    }
    else if (skin == "splash") {
        //#f2f2f2
        rgba = 'rgba(242,242,242,' + _skinOpacity + ')';
    }
    else if (skin == "strawberry_jam") {
        //#BF0D08
        rgba = 'rgba(143,13,8,' + _skinOpacity + ')';
    }
    else if (skin == "swimming_pool") {
        //#7ca2bc
        rgba = 'rgba(124,162,188,' + _skinOpacity + ')';
    }
    return rgba;
}

function get_rgba(skin) {
    var rgba = 'rgba(255,255,255,' + _skinOpacity + ')';
    if (skin == "avocado") {
        //#e7ffe7
        rgba = 'rgba(231,255,231,' + _skinOpacity + ')';
    }
    else if (skin == "blueberry") {
        //#2e4756
        rgba = 'rgba(46,71,86,' + _skinOpacity + ')';
    }
    else if (skin == "cafe_latte") {
        //#f4f4f4
        rgba = 'rgba(244,244,244,' + _skinOpacity + ')';
    }
    else if (skin == "dark") {
        //#141414
        rgba = 'rgba(20,20,20,' + _skinOpacity + ')';
    }
    else if (skin == "glacier") {
        //#C1C7D9
        rgba = 'rgba(193,199,217,' + _skinOpacity + ')';
    }
    else if (skin == "neon_orange") {
        //#fff
    }
    else if (skin == "neon_pink") {
        //#fff
    }
    else if (skin == "newspaper") {
        //#fff
    }
    else if (skin == "quartz") {
        //#2F3032
        rgba = 'rgba(47,48,50,' + _skinOpacity + ')';
    }
    else if (skin == "splash") {
        //#fff
    }
    else if (skin == "strawberry_jam") {
        //#fff
    }
    else if (skin == "swimming_pool") {
        //#f1f5f8
        rgba = 'rgba(241,245,248,' + _skinOpacity + ')';
    }
    return rgba;
}

function re_height() {
    setTimeout(function () {
        var off = 150;
        var timeline = document.getElementById("timeline");
        if (timeline) {
            off = 270;
        }
        var hw = $(window).height();
        var h0 = $('#oooo').height() + off;
        $('#thumb-wap').css('min-height', (hw - h0) + 'px');
        // $('#content').css('height', 'auto');
    }, 0);
}

$(document).ready(function () {
    window.cefQuery({
        request: 'Prompt.q.loading:1'
    });

    if (_bg != "") {
        var img = new Image();
        img.src = _bg;
        img.onload = function () {
            _bg = "url('" + _bg + "') center fixed no-repeat border-box ";
            set_bg2(_bg, 1);

        };
    }
    else {
        if (_skin != "newspaper" && _skin != "avocado" && _skin != "glacier" && _skin != "strawberry_jam") {
            $('#qsearchInput').css('border', '0');
        }
    }

    if (!$("#pendingtitle").is(":hidden")) {
        $("#menubar_manage").attr("title", _titlefol);
    }

    if (_last == "1") {
        setTimeout(function () {
            $.get("./index.html?act=last.get_nb", function (data, state) {
                var info = $.parseJSON(data);
                if (info.stat == "ok") {
                    var nb = parseInt(info.nb);
                    if (nb == 0) {
                        $("#pendingtitle").hide();
                        $("#menubar_manage").attr("title", "");
                    }
                    else {
                        $("#pendingtitle").show();
                        $("#menubar_manage").attr("title", _titlefol);
                    }
                }
            });
        }, 700);
    }

    $(window).resize(function () {
        _resized = 1;
        if (_map == 1) {
            $.colorbox.resize({ innerWidth: window.innerWidth, innerHeight: window.innerHeight });
        }
        re_height();
    });

    storage.b = '0';

    window.cefQuery({
        request: 'Prompt.q.uploading:0'
    });

    if (_day > 0) {
        $('.errors0').delay(100000).fadeOut();
    }

    if (_ismsnry == "1") {
        _msnry = $('.wrapper').flexImages({ rowHeight: _h0 });
        myscroll(_h0, _maxPage0);
    }

    $('ul.slim').superfish();
    $('ul.slim').show();
    $('.selbtn .sel').css({ 'pointer-events': 'none', 'opacity': '.7' });

    if (_art == "") {
        var b = $('#thumb-wap').hasClass("wap99");
        if (!b) {
            $('#trash').css("visibility", "visible");
        }
    }

    var timeline = document.getElementById("timeline");
    if (timeline) {
        $().timelinr();
    }

    var sbFunc = function (link, box) {
        $(link).click(function () {
            var elt = $(box);

            var w0 = elt.outerWidth(true);
            var h0 = 0;
            var off = 0;
            var vw = $('#titrecontent').width();

            var id = elt.attr("id");
            if (id == "calendarViewSwitchBox") {
                $("#sortOrderBox").hide();
                $("#derivativeSwitchBox").hide();
                $("#searchBox").hide();
            }
            else if (id == "sortOrderBox") {
                $("#calendarViewSwitchBox").hide();
                $("#derivativeSwitchBox").hide();
                $("#searchBox").hide();
            }
            else if (id == "derivativeSwitchBox") {
                $("#calendarViewSwitchBox").hide();
                $("#sortOrderBox").hide();
                $("#searchBox").hide();
            }
            else if (id == "searchBox") {
                $("#calendarViewSwitchBox").hide();
                $("#derivativeSwitchBox").hide();
                $("#sortOrderBox").hide();

                w0 = Math.max(elt.outerWidth(true), 550);
                h0 = 70;
                if (vw > 1140) {
                    off = (vw - 1140) / 2;
                }
            }
            var left = Math.min($(this).position().left + off, vw - w0);
            var top = $(this).position().top + $(this).outerHeight(true) + h0;

            elt.css("left", left);
            elt.css("top", top);

            $(box).toggle();

            if (id == "searchBox") {
				if($('#searchBox').html()<100){
					$.get("./index.html?act=index.searchbox", function (data, state) {

						$('#searchBox').html(data);
						var $area1 = $('#authors').selectize({
							plugins: ['remove_button'] 
						});
						if ($area1) {
							control1 = $area1[0].selectize;
						}

						var $area2 = $('#tags').selectize({
							plugins: ['remove_button'] 
						});
						if ($area2) {
							control2 = $area2[0].selectize;
						}

						var $area3 = $('#categories').selectize({
							plugins: ['remove_button']
						});
						if ($area3) {
							control3 = $area3[0].selectize;
						}
							
						$(".filter-manager-controller-container").hide();
						if ($(".filter-manager-controller-container").is(":hidden")) {
							$("#arrd1").html('<i2 class="icon-down-open"></i2>');
						}

						$("input[name=dimension_type]").change(function () {
							$(".dimensionslider").hide();
							var v = $(this).prop("value");
							$("div[data-slider=" + v + "]").show();
							_opt._dimension_type = v;
						});

						$("input[name=date_type]").change(function () {
							$(".dateslider").hide();
							var v = $(this).prop("value");
							$("div[data-slider=" + v + "]").show();
							_opt._datetype = v;
						});

						$('.filter-manager-controller').change(function () {
							var filters = "";
							var objs = $('.filter-manager-controller').filter(':checked');
							for (var i = 0; i < objs.length; i++) {
								var o = $(objs[i]);
								filters += o.attr('name') + ',';
							}
							_opt._filters = filters;

							filter_controller();
						});

						$("#search_submit").on("click", function () {
							submitclick();
						});
										
						$.getJSON("./index.html?act=index.slider", function (data) {
							if (data.stat == "ok") {
								$("#s_date").html(data.result.slider_postdate + data.result.slider_createdate);
								$("#s_dimension").html(data.result.slider_dimension);

								var sliders = data.result.slider_josn;
								$('[data-slider=date_creation]').dblSlider(sliders.createdates);
								$('[data-slider=date_available]').dblSlider(sliders.postdates);

								$('[data-slider=widths]').dblSlider(sliders.widths);
								$('[data-slider=heights]').dblSlider(sliders.heights);
								$('[data-slider=ratios]').dblSlider(sliders.ratios);
								$('[data-slider=filesizes]').dblSlider(sliders.filesizes);

							}
							filter_controller();
							set_checkbox();

							setTimeout(function () {
								$("input[name=search_allwords]").focus();
							}, 50);
						});
					});
				}
				else{
					setTimeout(function () {
						$("input[name=search_allwords]").focus();
					}, 50);
				}
            }

            return false;
        });
		
        $(box).on("mouseleave", function () {
            var elt = $(box);
            var id = elt.attr("id");
            if (id == "searchBox") { } else {
                $(this).hide();
            }
        });

        $(box).on("click", function () {
            var elt = $(box);
            var id = elt.attr("id");
            if (id == "searchBox") { } else {
                $(this).hide();
            }
        });
    };
	
    if (window.SwitchBox) {
        for (var i = 0; i < SwitchBox.length; i += 2)
            sbFunc(SwitchBox[i], SwitchBox[i + 1]);
    }
    window.SwitchBox = {
        push: sbFunc
    };

    (window.SwitchBox = window.SwitchBox || []).push("#searchLink", "#searchBox");
    (window.SwitchBox = window.SwitchBox || []).push("#calendarViewSwitchLink", "#calendarViewSwitchBox");
    (window.SwitchBox = window.SwitchBox || []).push("#sortOrderLink", "#sortOrderBox");
    (window.SwitchBox = window.SwitchBox || []).push("#derivativeSwitchLink", "#derivativeSwitchBox");

    $(window).scroll(function () {
        if ($("#searchBox").is(":hidden")) {
            $('.switchBox').hide();
        }
    });

    $.get("./index.html?act=index.menubar_categories&category=" + _cat_id, function (data, state) {
        $("#menubar_c").html(data);
        $('#menubar_c').superfish();
    });

    $(".image_stack").mouseenter(function () {
        var $parent = $(this);
        $parent.find('img.photo1').addClass('rotate1');
        $parent.find('img.photo2').addClass('rotate2');
        $parent.find('img.photo3').addClass('rotate3');

        var w1 = parseInt($parent.find('img.photo1').css("width"));
        var w2 = parseInt($parent.find('img.photo2').css("width"));
        var w3 = parseInt($parent.find('img.photo3').css("width"));
        if (w1 > 0 && w2 > 0 && w3 > 0) {
            var w = Math.max(w1, w2, w3);
            $parent.find('img.photo1').css("left", (258 - w) / 2 + 50 + 'px');
            $parent.find('img.photo3').css("left", (258 - w) / 2 - 50 + 'px');
        }
    })
        .mouseleave(function () {
            var $parent = $(this);
            $parent.find('img.photo1').removeClass('rotate1');
            $parent.find('img.photo2').removeClass('rotate2');
            $parent.find('img.photo3').removeClass('rotate3');

            var w1 = parseInt($parent.find('img.photo1').css("width"));
            var w2 = parseInt($parent.find('img.photo2').css("width"));
            var w3 = parseInt($parent.find('img.photo3').css("width"));
            if (w1 > 0 && w2 > 0 && w3 > 0) {
                var w = Math.max(w1, w2, w3);
                $parent.find('img.photo1').css("left", (258 - w) / 2 + 8 + 'px');
                $parent.find('img.photo3').css("left", (258 - w) / 2 + 'px');
            }
        });

    if (location.href.indexOf('favorites') >= 0) {
        $('.bfav').find('.stm-icon').toggleClass('stm-icon-favorite-add stm-icon-favorite-del');
        $('.bfav').attr('title', _tit_favorites_del);
    }
    if (location.href.indexOf('caddie') >= 0) {
        $('.bcaddie').find('.stm-icon').toggleClass('pn-icon-caddie-add pn-icon-caddie-del');
        $('.bcaddie').attr('title', _tit_caddie_del);
    }

    $("#mydiv").mouseup(function () {
        if (_sort_start == 1) {
            _sort_start = 0;
            if (_obj.offset().left + _obj.width() < $('#trash').offset().left
                || _obj.offset().left > $('#trash').offset().left + $('#trash').width()
                || _obj.offset().top + _obj.height() < $('#trash').offset().top) {

                var srcid = _obj.find('img').attr("data-id");
                $("input[data-id='" + srcid + "']").prop('checked', false);
                var o = $("input[data-id='" + srcid + "']").parent().find("span");
                o.removeClass('icon-check');
                o.addClass('icon-check-empty');
                _obj.remove();
                set_sel_title();
                $('#p' + srcid).parent().addClass('patipshow');
                $("a[data-id='" + srcid + "']").parent().addClass('patit');
            }
        }
    });

    $('.font-checkbox').fontCheckbox3();
    mydrag0($('#thumb-wap img'));

    $('#mydiv').sortable({
        start: function (event, ui) {
            _sort_start = 1;
            _obj = $(ui.item[0]);
        }
    });

    $('#trash').droppable({
        drop: function (e, source) {
            var srcid = $(source.helper[0]).attr("data-id");
            if (srcid) {
                var src = $(source.helper[0]).attr("src");
                add_trash(srcid, src);
            }
        }
    });

    if (storage.idx == undefined) {
        storage.idx = "1";
    }

    if (qsearch_prompt != _qval) {
        setTimeout(function () {
            document.getElementById("qsearchInput").value = _qval;
        }, 0);
    }

    if (_ov == "1") {
        $('.thumbimg, .stackphotos').watermark({
            text: _txt,
            textWidth: 500,
            opacity: 0.5,
            gravity: 'c',
            textSize: 36
        });

        setTimeout(function () {
            jshow_mask(1);
        }, 10000);
    }

    $(document).on("mouseup", function (e) {
        var con = $(e.target);
        if (!con.is('#searchBox *') && !con.is('#searchLink *')) {
            $("#searchBox").hide();
        }
        if (!con.is('#sortOrderBox *') && !con.is('#sortOrderLink *')) {
            $("#sortOrderBox").hide();
        }
        if (!con.is('#derivativeSwitchBox *') && !con.is('#derivativeSwitchLink *')) {
            $("#derivativeSwitchBox").hide();
        }
        if (!con.is('#calendarViewSwitchBox *') && !con.is('#calendarViewSwitchLink *')) {
            $("#calendarViewSwitchBox").hide();
        }

        if (!con.is('.filter-manager-controller-container *') && !con.is('#choosef *')) {
            $(".filter-manager-controller-container").hide();
        }
    });

    if (document.querySelector('#timeline')) {
        $('#header').css('height', '270px');
        $('#titrecontent').css('height', '200px');
    }
    else {
        $('#header').css('height', '150px');
        $('#titrecontent').css('height', '80px');
    }

    lazyLoadImages();

    if (document.querySelector('.flex-images')) {
        $('.trash2').show();
    }
    else {
        $('.trash2').hide();
    }

    if (_filters == "") {
        _filters = "filter_search_use,filter_category_use,filter_filetypes_use";
    }

    _opt = {
        "_filters": _filters,
        "_dimension_type": "widths",
        "_mode": "AND",
        "_searchin": "search_in_name,search_in_comment,search_in_file",
        "_subcats": "",
        "_filetype": "",
        "_osmtype": "",
        "_datetype": "date_creation",
        "_tag_mode": "AND"
    }

    if (!storage.searchopt) {
        storage.searchopt = JSON.stringify(_opt);
    }
    else {
        _opt = JSON.parse(storage.searchopt);
    }


})
