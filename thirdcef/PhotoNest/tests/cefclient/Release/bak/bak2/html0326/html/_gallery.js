
function close_uploading(cmd) {
    if (cmd == "close") {
        $('#aru').html(_aru2);
    }
    else {
        $('#aru').html(_aru1);
    }
    $('#aru2').show();

    $('#info').attr('cmd', cmd);
    $('#info').delay(100).show();
    $('#info').delay(2000).fadeOut();
}

function admin_ok() {
    storage.b = '0';

    var cmd = $('#info').attr('cmd');
    if (!cmd) {
        return;
    }

    if (cmd == "close") {
        window.cefQuery({
            request: 'Prompt.q.close:0'
        });
        return;
    }

    $.getJSON('./ws.php?method=upload.brisque', function (data) {
        if (cmd == "back") {
            history.back();
        }
        else if (cmd == "forward") {
            history.forward();
        }
        else if (cmd == "reload") {
            location.reload();
        }
        else {
            window.cefQuery({
                request: 'Prompt.q.uploading:0'
            });
            window.location = cmd;
        }
    });
}

function fupload_err(obj) {
    window.cefQuery({
        request: 'Prompt.q.fupload_err:' + $(obj).attr("fname")
    });
}

function notify_uploading(val) {
    storage.b = val;
    window.cefQuery({
        request: 'Prompt.q.uploading:' + val
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

function selfol() {
    if (is_readonly()) {
        return;
    }
    if (_processing == 1) {
        return;
    }

    window.cefQuery({
        request: 'Prompt.q.dir:'
    });
}

function show_err(txt) {
    $('#aru').html(txt);
    $('#aru2').hide();
    $('#info').delay(100).show();
    $('#info').delay(2000).fadeOut();
}

function cefQuerySuccess(response) {
    var obj = $("#aru");
    obj.html("");
    var t0 = response;
    t0 = response.toLowerCase();
    if (t0.length < 4) {
        show_err(_label1);
        return;
    }

    if (_appdir.toLowerCase().indexOf(t0) >= 0) {
        show_err(_label2);
        return;
    }

    var t1, i;
    for (i = 0; i < _windir.length; i++) {
        t1 = _windir[i].toLowerCase();
        if (t0.indexOf(t1) >= 0) {
            show_err(_label2);
            return;
        }
    }

    for (i = 0; i < _currdir.length; i++) {
        t1 = _currdir[i].toLowerCase();
        if (t0 == t1) {
            show_err(_label2);
            return;
        }
    }

    var tmp = t0 + "/";
    for (i = 0; i < _synroot.length; i++) {
        t1 = _synroot[i].toLowerCase() + "/";
        if (tmp.indexOf(t1) >= 0) {
            show_err(_label3);
            return;
        }
    }

    $.get("./ws.php?method=galleries.addfolder&folder=" + encodeURIComponent(response), function (data, state) {
        var info = $.parseJSON(data);
        if (info.stat == "ok") {

            window.cefQuery({
                request: 'Prompt.q.syn_root:'
            });

            location.reload();
        }
    });
}

function openfol(obj) {
    window.cefQuery({
        request: 'Prompt.q.folder:' + $(obj).attr('root')
    });
}

function process_end(html) {
    $('.progress-extended').hide();
    $('#progressbarall').hide();

    if (html != "") {
        $("#info2").append('<ul style="margin-top: -10px;"><li>' + html + '</li></ul>');
        $("#info2").show();
    }
    $(".afterUploadActions").show();
    $(".selectFiles").hide();
    $(".more").hide();
    _processing = 0;
}

function complete(cancel) {
    var catids = uploadedCats.join(",");
    $('.icon-plus').css({ 'pointer-events': 'auto', 'opacity': '1' });
    $('.icon-cancel').css({ 'pointer-events': 'auto', 'opacity': '1' });

    var nb = 0;
    for (var i = 0; i < uploadedPhotos.length; i++) {
        if (uploadedPhotos[i] != 0) {
            nb++;
        }
    }

    $.getJSON('./ws.php?method=upload.complete&id=' + catids + '&cancel=' + cancel + "&nb=" + uploadedPhotos.length, function (data) {
        process_end("");
        if (uploadedPhotos.length > 0) {
            $("#info2").append('<ul style="margin-top: -10px;"><li>' + sprintf(_photosUploaded_label, nb) + '</li></ul>');
            var cat_id = "";
            for (var i = 0; i < data.result.length; i++) {
                var info = data.result[i];

                var ur = '&quot;<a href="./index.html?category=' + info.cat_id + '">' + info.label + '</a>&quot;';
                var title = _albumSummary_label.replace(/alburl/, ur);
                var html = sprintf(title, parseInt(info.nb_photos));

                $("#info2 ul").append('<li>' + html + '</li>');
                if (cat_id == "") {
                    cat_id = info.cat_id;
                }
            }

            $("#info2").show();
            $(".batchLink").attr("href", "./admin.html?page=admin.batch&from=1&batch=" + uploadedPhotos.join(","));
            $(".batchLink").html(sprintf(_batch_Label, nb));

            if (cat_id != "") {
                $("#visit").attr("href", "./index.html?category=" + cat_id);
            }

            //    $("a.preview-box").colorbox({
            //        transition: "none", width: "85%", height: "85%", fixed: true, maxWidth: 1140, opacity: .7
            //    });
        }
        else {
            $("#visit").attr("href", "./index.html");
        }
    });
}

function playbox(url) {
    $.colorbox({
        href: url, transition: "none", width: "95%", height: "95%", fixed: true, opacity: .7, //maxWidth: 1140,
        onComplete: function () {
        },
        onClosed: function () {
        }

    });
}

function progressall(file) {
    var progress = Math.floor(_sent * 100 / _len);

    $('.progress-extended').html(_sent + ' / ' + _len);
    $('.fileupload-progress')
        .find('.progress')
        .attr('aria-valuenow', progress)
        .children()
        .first()
        .css('width', progress + '%');

    if (file) {
        var folders = $("#folder").find('.progress');
        for (var i = 0; i < folders.length; i++) {
            var folder = folders[i];
            var txt = $(folder).attr('dir0');
            var pos = file.lastIndexOf("/");
            var path = file.substr(0, pos);
            if (path == txt) {
                var sizes = $(folder).attr('sizes');
                var idx = $(folder).attr('idx');
                var all = parseInt(sizes);
                var nb = parseInt(idx);
                if (all > 0) {
                    nb++;
                    $(folder).attr('idx', nb);
                    var progress2 = Math.floor(nb * 100 / all);
                    $(folder)
                        .attr('aria-valuenow', progress2)
                        .children()
                        .first()
                        .html(nb)
                        .css('width', progress2 + '%');

                    if (nb == all) {
                        $(folder).removeClass("progress-striped");
                    }
                }
                break;
            }
        }
    }

    if (_sent == _len) {
        notify_uploading("0");
        complete("0");
    }
}

function post_data(info) {
    _len = info.files.length;
    var interval = setInterval(function () {
        if (_all_cancel > 0) {
            clearInterval(interval);
            return;
        }

        if (_idx >= _len) {
            clearInterval(interval);
        }
        else if (_sending < 5) {
            for (var i = 0; i < 5; i++) {
                setTimeout(function () {
                    if (_idx < _len) {
                        _sending++;
                        var cat_id = info.files[_idx].cat_id;
                        var file = info.files[_idx].filename;
                        var postdata = "file=" + file;
                        $.post("./upload.html?category=" + cat_id, postdata, function (data, state) {
                            var r = $.parseJSON(data);
                            var res = r.result;
                            var fullname = '';

                            if (r.stat == "ok") {
                                fullname = res.fullname;
                                uploadedPhotos.push(parseInt(res.image_id));
                            }
                            else //if (r.stat == "small") 
                            {
                                fullname = r.file;
                                uploadedPhotos.push(0);

                                var html_ = '<div class="thumbdiv"><div class="fuploaderr" onclick="fupload_err(this);return false;" fname="' + r.file + '">' + r.err2 + '<br/>';
                                html_ += '<span style="color:#ff0000">' + r.message;
                                html_ += '</span></div></div>';

                                jQuery("#uploadedPhotos").prepend(html_);
                                jQuery("#uploadedPhotos").parent("fieldset").show();
                            }

                            if (uploadedCats.indexOf(cat_id) < 0) {
                                uploadedCats.push(cat_id);
                            }

                            _sent++;
                            _sending--;
                            progressall(fullname);
                        });
                        _idx++;
                    }
                }, 0);
                if (_sending >= _len) {
                    break;
                }
            }
        }
    }, 100);
}

function handle() {
    var folders = $("#folder").find('.uploaded');
    var title = "";
    var nb1 = 0;
    var nb2 = folders.length;
    for (var i = 0; i < folders.length; i++) {
        var folder = folders[i];
        nb1 += parseInt($(folder).text());
    }

    if (nb1 > 0) {
        title = _penSyn2;
        if (_album == "public") {
            title = _penSyn0;
        }
    }

    if (nb2 > 1) {
        if (title != "") {
            title += ", ";
        }
        title += _penSyn1;
    }

    title = title.replace(/nb1/, nb1);
    title = title.replace(/nb2/, nb2);

    var title0 = "";
    if (_count == 0) {
        if (_all_ok == 0) {
            if (_pending != 0) {
                title0 = _penTitle2;
                if (_album == "public") {
                    title0 = _penTitle;
                }

                title0 = title0.replace(/nb1/, _pending);
                if (title != "") {
                    title += ", ";
                }
                title += title0;
                $("#startUpload").removeAttr("disabled");
            }
        }
    }
    $('.progress-extended').html(title);
    $("#btnadd").removeClass("mydisabled");
    $('#btnadd').css({ 'pointer-events': 'auto' });

}

function pre_data() {
    $("a.openf").colorbox({
        transition: "none", width: 720, height: 450, fixed: true, iframe: true, opacity: .7,
        onClosed: function () {
            if (_cat_name_change == 1) {
                var catid = $.colorbox.element().attr('data-id');
                $.get('./ws.php?method=galleries.modify&id=' + catid, function (data) {
                    var info = $.parseJSON(data);
                    if (info.stat == "ok") {
                        $.colorbox.element().text(info.text);
                    }
                });
            }
        }
    });

    var folders = $("#folder").find('.state');
    _count = folders.length;
    _pre_len = _count;

    var pre_val = setInterval(function () {
        if (_all_cancel > 0) {
            clearInterval(pre_val);
            return;
        }

        if (_pre_idx >= _pre_len) {
            clearInterval(pre_val);
            $("#btnadd").removeClass("mydisabled");
            $('#btnadd').css({ 'pointer-events': 'auto' });
        }
        else if (_pre_sending < 30) {
            for (var i = 0; i < 30; i++) {
                _pre_sending++;
                var folder = folders[_pre_idx];
                var id = $(folder).attr('catid');

                $.getJSON('./ws.php?method=galleries.pending&id=' + id, function (data) {
                    if (data.stat == "ok") {
                        var info = data.result;
                        $('#trid' + info.id).find('.uploaded').text(info.uploaded);
                        $('#trid' + info.id).find('.pending').text(info.pending);
                        $('#trid' + info.id).find('.state').attr('state', info.state);
                        $('#trid' + info.id).find('.progress').attr('sizes', info.sizes);

                        if (info.alb != "") {
                            $('#trid' + info.id).find('.tbody0').html(_notfound);
                        }

                        _pending += parseInt(info.pending);
                        if (info.state != "ok") {
                            _all_ok = 0;
                        }
                        _pre_sending--;
                    }
                    _count--;
                    setTimeout(function () { handle(); }, 100);
                });
                _pre_idx++;
                if (_pre_idx >= _pre_len) {
                    break;
                }

                if (_pre_sending >= _pre_len) {
                    break;
                }
            }
        }
    }, 10);
}

function restore() {
    if (is_readonly()) {
        return;
    }
    $.get('./ws.php?method=galleries.restore', function (data) {
        var info = $.parseJSON(data);
        if (info.stat == "ok") {
            window.cefQuery({
                request: 'Prompt.q.syn_root:'
            });

            location.reload();
        }
    });
}

function proc_upload() {
    var ids = "";
    var folders = $("#folder").find('.state');
    for (var i = 0; i < folders.length; i++) {
        var folder = folders[i];
        var id = $(folder).attr('catid');
        var state = $(folder).attr('state');
        if (state != "ok") {
            ids += id + ",";
        }
    }

    if (ids == "") {
        notify_uploading("0");
        complete("0");
    }
    else {
        $('.progress-extended').show();
        $('#progressbarall').show();

        $("#startUpload").attr("disabled", "true");
        $.post("./ws.php?method=galleries.categories", "ids=" + ids, function (data, state) {
            var info = $.parseJSON(data);
            if (info.stat == "ok") {
                if (info.files.length > 0) {
                    $("#uploadingActions").show();
                    $("#btnadd").hide();
                    $(".showInfo").hide();
                    post_data(info);
                }
                else {
                    notify_uploading("0");
                    complete("0");
                }
            }
            else {
                notify_uploading("0");
                complete("0");
            }
        });
    }
}

function show_paint(id) {
    window.cefQuery({
        request: 'Prompt.q.paint2:' + id
    });
}

function close_paint() {
    window.cefQuery({
        request: 'Prompt.q.closep:'
    });
    $.colorbox.close();
}

function jshow_mask() {
    setTimeout(function () {
        $.colorbox({
            transition: "none", width: "200", height: "200", fixed: true, overlayClose: false, escKey: false, //iframe: true,
            html: '<a class="paintx" href="javascript:;" onclick="close_paint();return false;">X</a>',
            onComplete: function () {
                $('#cboxContent').css('background-color', 'transparent');
                $('#cboxLoadedContent').css('background-color', 'transparent');
                $('#cboxClose').hide();
            },
            onClosed: function () {
                document.getElementById('cboxContent').style.removeProperty('background-color');
                $('#cboxClose').show();
            }
        });
    }, 0);
}

function jshow_close() {
    $.colorbox.close();
}

function reload_img(id) {
    var obj = $('#pic_' + id);
    var d = new Date();
    var n = d.getTime();
    var src0 = $(obj).attr('data-original') + "?d=" + n;
    $(obj).attr({ src: src0 });
}

function remove_folder(rootid) {
    if (is_readonly()) {
        return;
    }
    $.colorbox({
        transition: "none", width: 650, height: 400, fixed: true, iframe: true, overlayClose: false, opacity: 0.6, closeButton: false,
        // title: _tit10,
        href: "./admin.html?page=admin.remove_enc_box&rootid=" + rootid,
        onComplete: function () {
            $('#cboxOverlay').css({ 'background': 'slategray' });
            $('#cboxWrapper').css({ 'border-top': 'solid 7px #e74c3c' });
        },
        onClosed: function () {
            document.getElementById('cboxOverlay').style.removeProperty('background');
            document.getElementById('cboxWrapper').style.removeProperty('border-top');
        }

    });
}

function coo(obj, id) {
    var txt = $(obj).html();
    if (txt.indexOf("i2") >= 0) {
        $(obj).html('<i class="icon-down-open"></i>');
    }
    else {
        $(obj).html('<i2 class="icon-left-open"></i2>');
    }
    $('.zz' + id).toggleClass('hide_tr');
}

function myupload() {
    if (is_readonly()) {
        return;
    }

    $("#info2").hide();
    $('#info').hide();

    $("#startUpload").attr("disabled", "true");

    _processing = 1;

    $('.icon-plus').css({ 'pointer-events': 'none', 'opacity': '.7' });
    $('.icon-cancel').css({ 'pointer-events': 'none', 'opacity': '.7' });

    if (_all_ok == 1) {
        $.post("./ws.php?method=galleries.clear", "rename=1", function (data, state) {
            var info = $.parseJSON(data);
            if (info.stat == "ok") {
                window.cefQuery({
                    request: 'Prompt.q.syn_root:'
                });
                proc_upload();
                _processing = 0;
                $("#info2").html(_ok);
                $("#info2").show();
                $('.icon-plus').css({ 'pointer-events': 'auto', 'opacity': '1' });
                $('.icon-cancel').css({ 'pointer-events': 'auto', 'opacity': '1' });
            }
        });
        return false;
    }

    notify_uploading("1");

    $('.cancel').hide();
    $('#progressbarall').show();

    var ids = "";
    var datas = "";
    var folders = $("#folder").find('.state');

    for (var i = 0; i < folders.length; i++) {
        var folder = folders[i];
        var id = $(folder).attr('catid');
        var path = $(folder).attr('path');
        path = encodeURIComponent(path);

        if (id.indexOf('i') >= 0) {
            ids += id + ",";
            datas += path + ",";
        }
    }

    $.post("./ws.php?method=galleries.clear", "ids=" + ids + "&files=" + datas, function (data, state) {
        var info = $.parseJSON(data);
        if (info.stat == "ok") {

            window.cefQuery({
                request: 'Prompt.q.syn_root:'
            });

            for (var i = 0; i < info.cats.length; i++) {
                var cat = info.cats[i];
                $('#trid' + cat.id).find('.state').attr('catid', cat.newid);
                $('#trid' + cat.id).attr('id', 'trid' + cat.newid);
            }
            proc_upload();
        }
    });
}

$(document).ready(function () {
    if (_album == "public") {
        $(".icon-download").hide();
        $(".normal_tab").show();
    }
    else {
        $("#startUpload").text(_encrypt);
        $("#td_upload").text(_encrypted);
        $("#td_tobe").text(_toenc);
        $("#titleh2 span").html(_title2);

    }
    $('#btnadd').css({ 'pointer-events': 'none' });

    $.get('./ws.php?method=galleries.init', function (data) {
        if (data != " ") {
            $("#folder").append(data);
            if (_album == "public") {
                $(".icon-download").hide();
            }

            setTimeout(function () {
                pre_data();
            }, 100);
        }
    });

    $("a.showInfo").click(function () {
        $(".more").toggle();
        document.getElementById('content-wrapper').scrollTop = 2000;
        return false;
    });

    $("#cancelUpload").click(function () {
        notify_uploading("0");

        _all_cancel = 1;
        var cancelval = setInterval(function () {
            if (_sending == 0) {
                clearInterval(cancelval);
                complete("1");
                $("#uploadingActions").hide();
            }
        }, 100);
        return false;
    });

    $("#startUpload").click(function () {
        myupload();
    });

    if (storage.idx == undefined) {
        storage.idx = "1";
    }
});
