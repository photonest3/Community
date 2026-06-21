function u3down() {
    if (_total == 0) {
        return;
    }

    $.colorbox({
        transition: "none", width: 650, height: 400, fixed: true, iframe: true, overlayClose: false,
        title: _exportTitle,
        href: "./index.html?act=index.down_box&down=1&nb=1&ids=" + _picid,
        onComplete: function () {
            $('#cboxClose', window.parent.document).hide();
        },
        onClosed: function () {
            $('#cboxClose', window.parent.document).show();
        }
    });
}

function cefQuerySuccess(response) {
    var ifm = document.getElementsByClassName("cboxIframe");
    if (ifm.length > 0) {
        ifm[0].contentWindow.cefQuerySuccess(response);
    }
}

function show_paint() {
    window.cefQuery({
        request: 'Prompt.q.paint2:' + _picid
    });
}

function jshow_mask(ty) {
    var ifm = document.getElementsByClassName("cboxIframe");
    if (ifm.length > 0) {
        return;
    }
    var htm = '<center><a class="paintx" href="javascript:;" onclick="$.colorbox.close();return false;">X</a></center>';
    var opy = 0.9;
    if (ty == 1) {
        opy = 0.9;
        htm = $('.errors0').html() + '<center style="margin-top: 30px;"> <a class="paintx" href="javascript:;" onclick="$.colorbox.close();return false;">X</a></center>';
    }

    setTimeout(function () {
        $.colorbox({
            transition: "none", width: "400", height: "200", fixed: true, overlayClose: false, escKey: false, opacity: opy,
            html: htm,
            onComplete: function () {
                $('#colorbox').css('background', 'transparent');
                $('#colorbox').css('box-shadow', 'unset');
                $('#cboxContent').css('background-color', 'transparent');
                $('#cboxLoadedContent').css('background-color', 'transparent');
                $('#cboxClose').hide();
                parent._vr_mask = 1;
            },
            onClosed: function () {
                var box = document.getElementById('colorbox');
                box.style.removeProperty('background');
                box.style.removeProperty('box-shadow');

                $('#cboxContent').css('background-color', '#000');
                $('#cboxLoadedContent').css('background-color', '#f1f1f1');
                $('#cboxClose', window.parent.document).show();
                $('#cboxClose').show();
                parent._vr_mask = 0;
            }
        });
    }, 0);
}

function jshow_close() {
    $.colorbox.close();
}

function reload_img(id) {
    $.get("./picture.html?act=picture.get_src&idx=" + _idx, function (data, state) {
        var info = $.parseJSON(data);
        if (info.stat == "ok") {
            var obj = $("#box_" + info.result.id);
            $(obj).attr('rotate', '0');

            var d = new Date();
            var n = d.getTime();

            var src0 = info.result.src;
            $(obj).attr('data-original-url', src0 + "?d=" + n);

            var src1 = $(obj).attr('data-src');
            var pos = src1.indexOf("?d=");
            if (pos > 0) {
                src1 = src1.substring(0, pos);
            }

            $(obj).attr('data-src', src1 + "?d=" + n);

            _vr.viewed = false;
            _vr.view(_idx);
        }
    });
}

function gotourl(obj) {
    var url = $(obj).attr("data");
    if (url == 'index.html') {
        parent.window.location.href = url;
    }
    else if (parent.window.location.href.indexOf(url) >= 0) {
        parent.$.colorbox.close();
    }
    else {
        parent.window.location.href = url;
    }
}

function viewerjs_play(id) {
    if (_total == 0) {
        return;
    }

    var d = document.getElementById('vdo');
    d.href = "./index.html?act=index.box&id=" + id;
    d.click();
}

function vr_del() {
    if (_total == 1) {
        _vr.destroy();
        _total = 0;
        $('#imageToolBar').hide();
    }
    else {
        if (_vr.viewed == false) {
            _idx = _vr.index;
        }

        _total -= 1;
        var index = _idx;
        if (_updated == 0) {
            if (_nbPage > 0) {
                index = _idx - (_nbPage - 1) * PERPAGE_TOTAL;
            }
        }
        _vr.update(index);
    }
    parent._vr_reload = 1;
}

function show_loc(obj) {
    var fpath = $(obj).attr('title');
    window.cefQuery({
        request: 'Prompt.q.viewfile:' + fpath
    });
}

function ondescblur(obj) {
    if (_readonly != "0") {
        return false;
    }

    var body = "&title=" + encodeURIComponent($(obj).val());
    $.get("./picture.html?act=picture.update_comment&idx=" + _idx + body, function (data, state) {
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

function add_to_favorites() {
    if (is_readonly()) {
        return;
    }

    if (_total == 0) {
        return;
    }
    $.get("./picture.html?act=picture.add_to_favorites&idx=" + _idx, function (data, state) {
        $("#cmdfavorite").html(data);

        if (parent.location.href.indexOf('favorites') >= 0) {
            parent._vr_reload = 1;
        }
    });
}

function remove_favorites() {
    if (is_readonly()) {
        return;
    }

    if (_total == 0) {
        return;
    }
    $.get("./picture.html?act=picture.remove_favorites&idx=" + _idx, function (data, state) {
        $("#cmdfavorite").html(data);
        if (parent.location.href.indexOf('favorites') >= 0) {
            parent._vr_reload = 1;
        }
    });
}

function add_to_caddie() {
    if (is_readonly()) {
        return;
    }

    if (_total == 0) {
        return;
    }

    $.get("./picture.html?act=picture.add_to_caddie&idx=" + _idx, function (data, state) {
        $("#cmdCaddie").html(data);
    });
}

function set_as_representative() {
    if (is_readonly()) {
        return;
    }

    if (_total == 0) {
        return;
    }

    $.get("./picture.html?act=picture.set_as_representative&category=" + _category + "&idx=" + _idx, function (data, state) {
        $("#cmdSetRepresentative").html(data);
    });
}

function show_metadata() {
    if (_total == 0) {
        return;
    }
    $.get("./picture.html?act=picture.metadata&idx=" + _idx, function (data, state) {
        var info = $.parseJSON(data);
        if (info.stat == "ok") {
            $('.viewer-title').html(info.result.detail);

            var txt = _idx + 1;
            txt += ' / ' + _total;

            $("#photos").text(txt);
        }
    });

    var offwidth = (window.innerWidth - 1140);
    if ($('#tabcontent').css("display") != 'none') {
        $('#media').html('<span class="stm-icon pn-icon-eye"></span>');
    }
    else {
        offwidth = (280 * 2 > offwidth) ? 280 * 2 : offwidth;
        $('#media').html('<span class="stm-icon pn-icon-eye-off"></span>');
    }
    $('#tabcontent').toggle();

    _vr.resize(offwidth);

    _vr.viewed = false;
    _vr.view(_idx);
}

function get_src() {
    var _href = window.location.href;
    var pos = _href.indexOf("&idx=");
    if (pos > 0) {
        _href = _href.substring(0, pos);
    }
    return _href;
}

function slide() {
    $('#cboxContent').css('margin-top', '0');
    $('body').css('overflow-y', 'hidden');
    $.colorbox({
        transition: "none", width: "100%", height: "100%", fixed: true, iframe: true, //maxWidth: 1140,
        href: function () {
            var str = get_src().replace(/picture.html/, "slide.html");
            if (_slide != "") {
                str += "&slideshow=1";
            }
            return str + "&idx=" + _idx;
        },
        onComplete: function () {
            _sl = 1;
            $('#cboxClose', window.parent.document).hide();
            $('#cboxLoadedContent').css('padding', '0');
            //$('#cboxClose').addClass('colse-map');
            $('#cboxClose').css('top', '0');
            $('#cboxClose').show();
        },
        onClosed: function () {
            _sl = 0;
            $('#cboxClose', window.parent.document).show();
            $('#cboxContent').css('margin-top', '32px');
            $('body').css('overflow-y', 'auto');
            //  $('#cboxClose').removeClass('colse-map');
            $('#cboxClose').css('top', '-28px');

        }
    });
}

function u3del(idx) {
    if (is_readonly()) {
        return;
    }

    _idx = idx;
    var d = document.getElementById('del_box');
    d.href = "./delbox.html?type=2&nb=1&ids=" + _picid;
    d.click();
}

function selimg(picid) {
    _currpicid = picid;

    if (_reload_selectize == 1) {
        add_tag_opt(1);
        _reload_selectize = 0;
    }
    else {
        set_tag_value();
    }

    $.get("./picture.html?act=picture.get_all_exif&picid=" + picid, function (data, state) {
        var info = $.parseJSON(data);
        if (info.stat == "ok") {
            $("#striped tbody").html(info.exif);
        }
    });
}

function tag_selectize() {
    var $area = $('[data-selectize=tags]').selectize({
        valueField: 'id',
        labelField: 'name',
        sortField: 'name',
        searchField: ['name'],
        plugins: ['remove_button'],
        create: (_readonly == "0") ? true : false,
        render: {
            'option_create': function (data, escape) {
                return '<div class="create">' + _lang + ' <strong>' + data.input + '</strong>&hellip;</div>';
            }
        },
        onOptionAdd(value, data) {
            post_tags("0", value);
        },
        onChange(value) {
        },
        onItemAdd(value, $item) {
        },
        onItemRemove(value) {
        },
        onDropdownClose($dropdown) {
            var v = _control.getValue();
            post_tags("1", v);
        }
    });
    _control = $area[0].selectize;
    add_tag_opt(0);
}

function add_tag_opt(type) {
    _control.clearOptions(true);
    $.getJSON('./ws.php?format=json&method=tags.getAdminList', function (data) {
        _reset_tags = 1;
        for (var i = 0; i < data.result.tags.length; i++) {
            var tags = data.result.tags[i];
            _control.addOption({
                id: '~~' + tags.id + '~~',
                name: tags.name,
                fullname: tags.name
            });
        }
        _reset_tags = 0;
        if (type == 1) {
            set_tag_value();
        }
    });
}

function set_tag_value() {
    $.get("./index.html?act=index.get_tags&picid=" + _currpicid, function (data, state) {
        var info = $.parseJSON(data);
        if (info.stat == "ok") {
            //_control.setValue(['~~2~~']);
            _control.setValue(eval(info.tags));
        }
        else {
            _control.clear(true);
        }
    });
}

function post_tags(nb, value) {
    if (_reset_tags == 0) {
        var body = "&photos=" + _currpicid;
        body += "&add_tags=" + encodeURIComponent(value);
        var url = "./ws.php?method=batch.add_tags";

        if (nb == "1") {
            url = "./ws.php?method=batch.update_tags";
        }

        $.ajax({
            type: "POST",
            dataType: "text",
            url: url,
            data: body,
            success: function (result) {
                if (nb == 0) {
                    _reload_selectize = 1;
                }

                parent.change_tags("");

                var exif = "";
                if ($('#tabcontent').css("display") != 'none') {
                    exif = "title";
                }

                $.get("./picture.html?act=picture.get_data&exif=" + exif + "&category=" + _category + "&picid=" + _picid + "&osm=" + _osm, function (data, state) {
                    var info = $.parseJSON(data);
                    if (info.stat == "ok") {
                        var tit = $('#tit0').html() + " / " + info.result.detail;
                        $('.viewer-title').html(tit);
                    }
                });
            }
        });
    }
}

function speed(ty) {
    _sec += ty;

    if (_sec > 9) {
        _sec = 9;
    }
    if (_sec < 1) {
        _sec = 1;
    }

    $('#speedv').html(_sec + 's');
    _vr.set_interval(_sec * 1000);

}


function show_details() {

    if ($('#btnexif').css('opacity') != 1) {
        return;
    }


    if ($('#tbexif').is(':hidden')) {
        $('#tbexif').show();
        $('#btnexif').html('<i2 class="icon-down-open"></i2>');
        storage.exif = '1';
    }
    else {
        $('#tbexif').hide();
        $('#btnexif').html('<i class="icon-down-open"></i>');
        storage.exif = '0';
    }
}

$(document).ready(function () {
    if (_readonly != "0") {
        $('[data-selectize=tags]').removeAttr('data-create');
    }

    tag_selectize();

    if (storage.exif == undefined) {
        storage.exif = '0';
    }


    var offwidth = (window.innerWidth - 1140);
    if (_metadata == "metadata") {
        $('#tabcontent').show();
        offwidth = (280 * 2 > offwidth) ? 280 * 2 : offwidth;
        $('#media').html('<span class="stm-icon pn-icon-eye-off"></span>');
    }
    else {
        $('#tabcontent').hide();
        $('#media').html('<span class="stm-icon pn-icon-eye"></span>');
    }

    $(document).on("click", ".viewer-canvas", function () {
        _control.blur();
    });


    $('.table-striped').mouseenter(function (e) {
        $('.table-striped').css('overflow-y', 'auto');
    });


    $('.table-striped').mouseleave(function (e) {
        $('.table-striped').css('overflow-y', 'hidden');
    });


    $(document).mousemove(function (e) {
        if (_vr && _vr.played) {
            if (e.clientY < 100) {
                $(".viewer-clock-btn").css('display', 'inline-block');
            }
            else {
                $(".viewer-clock-btn").css('display', 'none');
            }
        }

        _in++;
        var off = $(document).height() - e.clientY;
        if (_vrsize == 0 || _vrsize == 3) {
            if (off < 100 /*&& off > 50*/) {
                $('.viewer-toolbar').stop().fadeIn();
                $('.viewer-navbar').stop().fadeIn();
                $('.viewer-title').stop().fadeIn();
            }
            else {
                $('.viewer-toolbar').stop().fadeOut();
                $('.viewer-title').stop().fadeOut();
                $('.viewer-navbar').stop().fadeOut();
            }
        }
        else if (_vrsize == 1) {
            if (off < 100) {
                $('.viewer-toolbar').fadeIn();
            }
            else {
                $('.viewer-toolbar').fadeOut();
            }
            $('.viewer-title').hide();
            $('.viewer-navbar').hide();
        }
        else {
            if (off < 100) {
                $('.viewer-toolbar').fadeIn();
                $('.viewer-title').fadeIn();
            }
            else {
                $('.viewer-toolbar').fadeOut();
                $('.viewer-title').fadeOut();
            }
            $('.viewer-navbar').hide();
        }

        if (e.clientY < 120) {
            $('.actionButtons').fadeIn();
        }
        else {
            //if (_in > 30)
            {
                $('.actionButtons').fadeOut();
            }
        }

        if (e.clientX < 200) {
            $('.previous').fadeIn();
        }
        else {
            $('.previous').fadeOut();
        }

        off = $(document).width() - e.clientX;

        if (off < 200) {
            $('.next').fadeIn();
        }
        else {
            $('.next').fadeOut();
        }
    });

    $(document).mouseleave(function (e) {
        $('.viewer-title').fadeOut(3500);
        $('.viewer-toolbar').fadeOut(3500);
        $('.viewer-navbar').fadeOut(3500);
    });

    if (_total > 300) {
        $("a.coi-box").css("pointer-events", "none");
    }

    var theImage = document.getElementById('theImage0');
    if (_nbPage > 0) {
        _init -= (_nbPage - 1) * PERPAGE_TOTAL;
    }

    _vr = new Viewer(theImage, {
        url: 'data-original-url',
        navbar: true,
        initialViewIndex: _init,
        nbPage: _nbPage * 100,
        inline: true,
        fullsize: _vrsize,
        transition: false,
        button: false,
        autorotate: false,
        initOffwidth: offwidth,
        title: true,
        rotated: function (e) {
            parent.rotate_img_help(_currpicid, e.detail.rotate);
        },
        viewed: function (e) {

            if (_ov == "1") {
                setTimeout(function () { vv(); }, 0);
            }

            _idx = e.detail.index;
            if (_updated == 0) {
                if (_nbPage > 0) {
                    _idx += (_nbPage - 1) * PERPAGE_TOTAL;
                }
            }

            if (_total > 300 && _delay == 0) {
                $("#albumCreationLoading").show();
            }

            var exif = "";
            if ($('#tabcontent').css("display") != 'none') {
                exif = "details";
            }

            $.get("./picture.html?act=picture.get_data&type=all&exif=" + exif + "&category=" + _category + "&idx=" + _idx + "&osm=" + _osm, function (data, state) {
                var info = $.parseJSON(data);
                if (info.stat == "ok") {
                    _picid = info.result.id;
                    //$('#filename').html(" / " + info.result.name);
                    var tit = $('#tit0').html() + " / " + info.result.detail;
                    if (_total > 300 && _delay == 0) {
                        //		tit += _loading;
                    }

                    var ext = info.result.ext;
                    if (ext == "mp3" || ext == "mp4" || ext == "gif" || ext == "webm" || ext == "ogg") {
                        $("#cmdedit").css("visibility", "hidden");
                    }
                    else {
                        $("#cmdedit").css("visibility", "visible");
                    }

                    $('.viewer-title').html(tit);
                    var txt = _idx + 1;
                    txt += ' / ' + _total;

                    $("#photos").text(txt);
                    $("#cmdfavorite").html(info.result.favorite);
                    $("#cmdCaddie").html(info.result.caddie);
                    $("#cmdSetRepresentative").html(info.result.representative);

                    var obj = $("#box_" + _picid);
                    var r = $(obj).attr('rotate');
                    if (r != info.result.rotate) {
                        $(obj).attr('rotate', info.result.rotate);
                        var ro = parseInt(info.result.rotate) - parseInt(r);
                        _vr.rotate(ro);
                    }

                    if (exif == "details") {
                        $("#striped tbody").html(info.result.exif);
                    }

                    _currpicid = _picid;

                    if (_reload_selectize == 1) {
                        add_tag_opt(1);
                        _reload_selectize = 0;
                    }
                    else {
                        if (info.result.tags != "") {
                            //_control.setValue(['~~2~~']);
                            _control.setValue(eval(info.result.tags));
                        }
                        else {
                            _control.clear(true);
                        }
                    }
                    //if (info.result.coi != "") {
                    //    $("#fcoi").show();
                    //}
                    //else {
                    //    $("#fcoi").hide();
                    //}

                    var di = $('.viewer-inpaint').css('display');
                    if (di == "none") {
                        $('#fcoi').hide();
                    }
                    else {
                        $('#fcoi').show();
                    }

                }


                if (storage.exif == '0') {
                    $('#tbexif').hide();
                    $('#btnexif').html('<i class="icon-down-open"></i>');
                }
                else {
                    $('#tbexif').show();
                    $('#btnexif').html('<i2 class="icon-down-open"></i2>');
                }

            });

            if (_delay == 0) {
                if (_total > 300) {
                    setTimeout(function () {
                        $.get("./picture.html?act=picture.get_page&page0=picture", function (data, state) {
                            $("#theImage0 ul").html(data);
                            _vr.update(99999);
                            _delay = 1;
                            _updated = 1;
                            $("a.coi-box").css("pointer-events", "auto");
                            $("#albumCreationLoading").hide();
                        });
                    }, 0);
                }
            }




        }
    });

    $("a.play_video_box").colorbox({
        transition: "none", width: "95%", height: "95%", fixed: true, //maxWidth: 1140,
        onComplete: function () {
            _ot = 1;
            $('#cboxClose', window.parent.document).hide();
        },
        onClosed: function () {
            _ot = 0;
            $('#cboxClose', window.parent.document).show();
        }

    });
    $("a.coi-box").colorbox({
        transition: "none", width: "970", height: "680", fixed: true, iframe: true, maxWidth: 1140,
        href: function () {
            if (_vr.viewed == false) {
                _idx = _vr.index;
            }
            return "./admin.html?page=admin.photo&category=" + _category + "&idx=" + _idx + "&osm=" + _osm;
        },
        onComplete: function () {
            _ot = 1;
            $('#cboxClose', window.parent.document).hide();
        },
        onClosed: function () {
            _ot = 0;
            $('#cboxClose', window.parent.document).show();
            if (_cbox_change == 1) {
                _cbox_change = 0;

                var exif = "";
                if ($('#tabcontent').css("display") != 'none') {
                    exif = "title";
                }

                $.get("./picture.html?act=picture.get_data&exif=" + exif + "&category=" + _category + "&picid=" + _picid + "&osm=" + _osm, function (data, state) {
                    var info = $.parseJSON(data);
                    if (info.stat == "ok") {
                        //$('#filename').html(" / " + info.result.name);
                        var tit = $('#tit0').html() + " / " + info.result.detail;
                        $('.viewer-title').html(tit);

                        var txt = _idx + 1;
                        txt += ' / ' + _total;
                        $("#photos").text(txt);
                        $(".description").text(info.result.comment);
                    }
                    else if (info.stat == "del") {
                        vr_del();
                    }
                });
                add_tag_opt(1);
            }
            else if (_cbox_change == 2) {
                _cbox_change = 0;
                vr_del();
            }
            else if (_cbox_change == 3) {
                _cbox_change = 0;
                parent._vr_reload = 1;
            }
        }
    });

    $("a.del_box").colorbox({
        width: 437, height: 270, fixed: true, iframe: true, overlayClose: false,
        title: _tit20,
        onComplete: function () {
            $('#cboxClose', window.parent.document).hide();
        },
        onClosed: function () {
            $('#cboxClose', window.parent.document).show();
        }
    });
});