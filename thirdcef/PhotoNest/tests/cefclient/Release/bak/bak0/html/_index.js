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
    $.get("./index.html?act=index.get_pending&ty=" + ty, function (data, state) {
        var info = $.parseJSON(data);
        if (info.stat == "err") {
            setTimeout("get_pending('0')", 700);
        }
        else if (info.stat == "ok") {
            var nb = parseInt(info.nb);
            if (nb <= 0) {
                $("#pendingtitle").hide();
                _admin_url = "./admin.html";
            }
            else {
                $("#pendingtitle").text(info.nb);
                $("#pendingtitle").attr("title", _title);
                $("#pendingtitle").show();
                _admin_url = "./admin.html?page=admin.galleries";
            }
        }
    });
}

function myactivate(activate) {
    if (activate == "1" && storage.idx == "0") {
        var sub = Math.ceil((new Date().getTime() - storage.curr) / 1000);
        if (sub > 60) {
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
                //show_level(1);

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

function header58() {
    $('.header').css('height', '58px');
    $('#foot').show();
    $('#nav-wrapper').addClass('topbar-sticky-pos');
    $('#zz0').addClass('topbar-sticky-pos');
    $('#zz0').addClass('actscorll');
    $('.categoryActions').addClass('actions');

    $('.nav').css('height', '58px');

    if (_bg != "") {
        $('#nav-wrapper').css('background', _bg);
        $('#nav-wrapper').css('background-size', 'cover');
        var rgba = get_header_rgba(_skin);
        $('#h58').css('background', rgba);
    }


    $('#banner').css('position', 'relative');
    $('#banner').css('top', '0px');
}
function header128() {
    $('.header').css('height', '138px');
    $('#foot').hide();
    $('#nav-wrapper').removeClass('topbar-sticky-pos');
    $('#zz0').removeClass('topbar-sticky-pos');
    $('#zz0').removeClass('actscorll');
    $('.categoryActions').removeClass('actions');

    $('.nav').css('height', '138px');

    var box = document.getElementById('nav-wrapper');
    box.style.removeProperty('background');
    box.style.removeProperty('background-size');


    box = document.querySelector('#h58');
    box.style.removeProperty('background');




    $('#banner').css('position', 'absolute');
    $('#banner').css('top', '40px');
}

function scroll0() {
    var h = 138 - 80;
    var b = $('#nav-wrapper').hasClass('topbar-sticky-pos');

    var sc = $(window).scrollTop();
    if (sc > h) {
        if (!b) {
            header58();
        }
    }
    else {
        if (b) {
            header128();
        }
    }
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

        //scroll0();
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
            var bg = info.bg + " center fixed no-repeat border-box ";
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
                set_bg2(bg);
            }
        }
    });
}

function set_bg2(bg) {
    _bg = bg;
    if (_bg == "") {
        return;
    }

    var rgba = 'background-color: transparent !important';
    if (_skin == "dark") {
        $('.nav').css('border', '0');
        $('.do_search').attr('style', 'color:#bbb !important');
    }
    else if (_skin == "splash") {
        rgba = 'background-color: rgb(255,255,255,0.3) !important';
    }
    $('#qsearchInput').attr('style', rgba);

    if (_bgtype == 1 && _skin == "glacier") {
        $('#nav-wrapper').css('background', _bg);
        $('#nav-wrapper').css('background-size', 'cover');
        rgba = get_header_rgba(_skin);
        $('.nav-wrapper').css('background-color', rgba);
    }
    else {

        //rgba = get_header_rgba(_skin);
        // $('#nav-wrapper').css('background-color', rgba);

        // if (_skin == "glacier") {
        //    $('#nav-wrapper').css('background-image', 'unset');
        // }

        if (_bgtype != 1) {

            $('#theCategoryPage').css('background', _bg);
            $('#theCategoryPage').css('background-size', 'cover');

            rgba = get_rgba(_skin);
            $('#content').css('background-color', rgba);
            $('#zz0').css('background-color', 'transparent');
			            $('#titrecontent').css('background-color', rgba);

			
        }
        else {
            var box = document.getElementById('zz0');
            box.style.removeProperty('background-color');
        }
    }

    $('#nav-wrapper').css('background', _bg);
    $('#nav-wrapper').css('background-size', 'cover');

    var rgba = get_header_rgba(_skin);
    $('#h58').css('background', rgba);

    re_height();
}

function clear_bg2() {
    var box = document.getElementById('theCategoryPage');
    box.style.removeProperty('background');

    box = document.getElementById('header');
    box.style.removeProperty('background-color');
    box.style.removeProperty('background-image');

    box = document.getElementById('content');
    box.style.removeProperty('background-color');

    box = document.querySelector('.nav-wrapper');
    box.style.removeProperty('background-color');
    box.style.removeProperty('background');

    box = document.querySelector('.nav');
    box.style.removeProperty('border');
    $('.do_search').attr('style', '');
    $('#qsearchInput').attr('style', '');
}

function close_paint() {
    window.cefQuery({
        request: 'Prompt.q.closep:'
    });
    $.colorbox.close();
}

function cefQuerySuccess(response) {
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
        htm = $('.errors0').html() + '<center style="margin-top: 30px;"> <a class="paintx" href="javascript:;" onclick="$.colorbox.close();return false;">X</a></center>';
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
    //document.styleSheets[0].rules[0].style.background='#555';
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

function show_star(catid) {

    $('#cmdlevel').toggleClass('pn-icon-star pn-icon-star-empty');
    _star = "0";
    if ($('.divlevel').is(":hidden")) {
        $('.divlevel').removeClass('nostar');
        _star = "1";
    }
    else {
        $('.divlevel').addClass('nostar');
    }

    $.get("./index.html?act=index.star&star=" + _star, function (data, state) {
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
        title: (_album == "private") ? _profiletitle : ''
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
        rgba = 'rgba(20,20,20,' + _skinOpacity + ')';
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
        if (_maxPage0 > 0) {
            $('#content').height('auto');
        }
        else {
            var h = $('#oooo').height() + $('#thumb-wap').height();
            var hw = $(window).height() - 300;
            if (h < hw) {
                $('#content').height('calc(100% - 70px)');
            }
            else {
                $('#content').height('auto');
            }
        }
    }, 10);

}

function show_level(ty) {

    if (_star == "1") {
        $('.divlevel').removeClass('nostar');
    }
    else {
        $('.divlevel').addClass('nostar');
    }
}

$(document).ready(function () {
    window.cefQuery({
        request: 'Prompt.q.loading:1'
    });

    if (_bg != "") {
        _bg = _bg + " center fixed no-repeat border-box ";

        set_bg2(_bg);
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
        $('.errors0').delay(5000).fadeOut();
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
        //show_level(1);
    }

    var timeline = document.getElementById("timeline");
    if (timeline) {
        $().timelinr();
    }

    var sbFunc = function (link, box) {
        $(link).click(function () {
            var elt = $(box);
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
                setTimeout(function () {
                    $("input[name=search_allwords]").focus();
                }, 50);

            }

            var left = Math.min($(this).position().left, $(window).width() - elt.outerWidth(true) - 5);
            var top = $(this).position().top + $(this).outerHeight(true);
            elt.css("left", left);
            elt.css("top", top);

            $(box).toggle();
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
        //scroll0();
    });

    $.get("./index.html?act=index.menubar_categories", function (data, state) {
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


});
