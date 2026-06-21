
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
    _cmd = cmd;
    $("#cancelUpload").click();
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
    _addFlag = 1;
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
    _addFlag = 0;
    if (response == "") {
        return;
    }
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
    $("#btnadd").addClass("mydisabled");
    $('#btnadd').css({ 'pointer-events': 'none' });

    $.get("./ws.php?method=galleries.addfolder&folder=" + encodeURIComponent(response), function (data, state) {
        var info = $.parseJSON(data);
        if (info.stat == "ok") {

            window.cefQuery({
                request: 'Prompt.q.syn_root:'
            });
            clear_dir();
        }
    });
}

function openfol(obj) {
    window.cefQuery({
        request: 'Prompt.q.folder:' + $(obj).attr('root')
    });
}

function process_end(html) {
    if (_cmd != "") {
        if (_cmd == "back") {
            history.back();
        }
        else if (_cmd == "forward") {
            history.forward();
        }
        else if (_cmd == "reload") {
            location.reload();
        }
        else {
            window.cefQuery({
                request: 'Prompt.q.uploading:0'
            });
            window.location = _cmd;
        }
        return;
    }

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
    $('.icon-plus').css({ 'pointer-events': 'auto', 'opacity': '1' });
    $('.icon-cancel').css({ 'pointer-events': 'auto', 'opacity': '1' });

    $.getJSON('./ws.php?method=upload.complete&id=' + _uploadedCats + '&cancel=' + cancel, function (data) {
        var nb = parseInt(data.nb) + parseInt(data.err40) + parseInt(data.err30);
        process_end("");
        if (_cmd != "") {
            return;
        }
        if (nb > 0) {
            var t2 = "";
            if (data.err40 != "0") {
                t2 += data.err40 + _msg40;
            }
            if (data.err30 != "0") {
                if (t2 != "") {
                    t2 += ", ";
                }
                t2 += data.err30 + _msg30;
            }
            if (t2 != "") {
                t2 = '<li style="color:#f22;">' + t2 + '</li>';
            }
            $("#info2").append('<ul style="margin-top: -10px;"><li>' + sprintf(_photosUploaded_label, parseInt(data.nb)) + '</li>' + t2 + '</ul>');
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
            $(".batchLink").attr("href", "./admin.html?page=admin.batch&from=1&filter_prefilter=last_import");
            $(".batchLink").html(sprintf(_batch_Label, parseInt(data.nb)));

            if (cat_id != "") {
                $("#visit").attr("href", "./index.html?category=" + cat_id);
            }
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

function progressall() {
    if (_sent < _pending) {
        var progress = Math.floor(_sent * 100 / _pending);

        $('.progress-extended').html(_sent + ' / ' + _pending);
        $('.fileupload-progress')
            .find('.progress')
            .children()
            .first()
            .css('width', progress + '%');
    }
}

function handle(changed) {
    var title = "";
    var nb1 = 0;
    var nb2 = _jsonPending.length;
    for (var i = 0; i < _jsonPending.length; i++) {
        var folder = _jsonPending[i];
        nb1 += parseInt(folder.uploaded);
    }

    if (nb1 > 0) {
        title = _penSyn2;
        if (_album == "public") {
        }
        title = _penSyn0;
    }

    if (nb2 > 1) {
        if (title != "") {
            title += ", ";
        }
        title += _penSyn1;
    }

    title = title.replace(/nb1/, nb1);
    title = title.replace(/nb2/, nb2);

    var b = false;
    var title0 = "";

    if (_pending != 0) {
        title0 = _penTitle2;
        if (_album == "public") {
        }
        title0 = _penTitle;

        title0 = title0.replace(/nb1/, _pending);
        if (title != "") {
            title += ", ";
        }
        title += title0;
        b = true;
        $("#startUpload").removeAttr("disabled");
    }

    if (!b && changed == "-1") {
        var t = _deleted.replace(/nb3/, _tobeclear);
        title += t;
        $('#btnclear').show();
    }
    $('.progress-extended span').html(title);
    $("#btnadd").removeClass("mydisabled");
    $('#btnadd').css({ 'pointer-events': 'auto' });
    $('#refresha').css({ 'pointer-events': 'auto' });

    if (_reload) {
        $('#info').delay(100).fadeOut();
        _reload = false;
    }
}

function ispending_ok() {
    var folders = $("#folder").find('.root_tr');
    if (_pendingok == folders.length) {
        $.getJSON('./ws.php?method=galleries.pendingok', function (data) {
            _tobeclear = data.deletes;
            handle(data.stat);
        });
    }
    else {
        setTimeout(function () {
            ispending_ok();
        }, 100);
    }
}

function pre_data() {
    _pendingok = 0;
    var folders = $("#folder").find('.root_tr');
    for (var i = 0; i < folders.length; i++) {
        var roottr = $(folders[i]);
        var rootid = roottr.attr('rootid');
        var catid = roottr.attr('catid');
        $.getJSON('./ws.php?method=galleries.pending&rootid=' + rootid + '&catid=' + catid + "&idx0=" + i, function (data) {
            if (data.stat == "ok") {
                for (var j = 0; j < data.result.length; j++) {
                    var info = data.result[j];
                    if (data.catid == info.id) {
                        $('#trid' + data.catid).find('img').hide();
                        $('#trid' + data.catid).find('.preview').show();
                        $('#trid' + data.catid).find('.mytd22 a').show();

                        $('#trid' + data.catid).find('.uploaded').text(info.uploaded);
                        $('#trid' + data.catid).find('.pending').text(info.pending);
                        if (info.alb != "") {
                            $('#trid' + data.catid).find('.tbody0').html(_notfound);
                        }
                    }
                    _jsonPending.push(info);
                    _pending += parseInt(info.pending);
                }
            }
            _pendingok++;
        });
    }

    setTimeout(function () {
        ispending_ok();
    }, 100);
}

function restore() {
    if (is_readonly()) {
        return;
    }

    if (storage.b == '1') {
        return;
    }

    $.get('./ws.php?method=galleries.restore', function (data) {
        var info = $.parseJSON(data);
        if (info.stat == "ok") {
            window.cefQuery({
                request: 'Prompt.q.syn_root:'
            });

            clear_dir();
        }
    });
}

function progress3() {
    $.post("./ws.php?method=ws.progress3", "0", function (data, state) {
        var info = $.parseJSON(data);
        if (info.stat == "ok") {
            var len = parseInt(info.len);
            _sent = parseInt(info.step);
            if (len > 0 && !_cancel) {
                progressall();
            }
            if (info.real == "ok") {
                if (len > 0 && !_cancel) {
                    _sent = _pending;
                    $('.progress-extended').html(_sent + ' / ' + _pending);
                }
                notify_uploading("0");
                complete("0");
            }
            else {
                setTimeout("progress3()", 1000);
            }
        }
    });
}

function proc_upload() {
    for (var i = 0; i < _jsonPending.length; i++) {
        var folder = _jsonPending[i];
        var id = folder.id;
        var state = folder.state;
        if (state != "0") {
            _uploadedCats += id + ",";
        }
    }

    if (_uploadedCats == "") {
        notify_uploading("0");
        complete("0");
    }
    else {

        $('.progress-extended').show();
        $('#progressbarall').show();

        $("#uploadingActions").show();
        $("#cancelUpload").removeAttr("disabled");

        $("#btnadd").hide();
        $(".showInfo").hide();
        $('#refresha').css({ 'pointer-events': 'none' });

        $("#startUpload").attr("disabled", "true");
        $.post("./ws.php?method=galleries.categories", "ids=" + _uploadedCats, function (data, state) {
            setTimeout("progress3()", 100);
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
    _addFlag = 1;
    $.colorbox({
        transition: "none", width: 650, height: 400, fixed: true, iframe: true, overlayClose: false, opacity: 0.6, closeButton: false,
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

function coo(obj, id, rootid) {
    var roottr;
    var folders = $("#folder").find('.root_tr');

    if (_pendingok != folders.length) {
        return;
    }

    for (var i = 0; i < folders.length; i++) {
        roottr = $(folders[i]);
        if (roottr.attr('rootid') == rootid) {
            break;
        }
    }

    if (roottr.attr("iscoo") != "1") {
        var tr = "";
        for (var i = 0; i < _jsonTr.length; i++) {
            var cat = _jsonTr[i];
            if (cat.id == rootid) {
                tr += cat.tr;
            }
        }
        roottr.after(tr);

        for (var j = 0; j < _jsonPending.length; j++) {
            var info = _jsonPending[j];
            if (info.root_id == rootid) {
                $('#trid' + info.id).find('.uploaded').text(info.uploaded);
                $('#trid' + info.id).find('.pending').text(info.pending);

                if (info.alb != "") {
                    $('#trid' + info.id).find('.tbody0').html(_notfound);
                }
            }
        }
        $('#folder').find('img').hide();
        $('#folder').find('.preview').show();
        $('#folder').find('.mytd22 a').show();
        roottr.attr("iscoo", "1");
    }

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
    notify_uploading("1");

    $('.cancel').hide();
    $('#progressbarall').show();

    var ids = "";
    var datas = "";
    for (var i = 0; i < _jsonPending.length; i++) {
        var folder = _jsonPending[i];
        var id = folder.id;
        var path = folder.path;
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
                for (var j = 0; j < _jsonPending.length; j++) {
                    if (_jsonPending[j].id == cat.id) {
                        _jsonPending[j].id = cat.newid;
                        break;
                    }
                }
            }
            proc_upload();
        }
    });
}

function clearfol() {
    if (is_readonly()) {
        return;
    }

    $.post("./ws.php?method=galleries.categories&clr=1", "0", function (data, state) {
        refresh_dir();
    });
}

function cbxedit(url) {
    $.colorbox({
        transition: "none", width: 800, height: 520, fixed: true, iframe: true, opacity: .7,
        href: url,
        onClosed: function () {
            if (_change_catid != "") {
                $("a[data-id='" + _change_catid + "']").text(_change_name);
            }
        }
    });
}

function init() {
    $('#btnadd').css({ 'pointer-events': 'none' });
    $('#refresha').css({ 'pointer-events': 'none' });

    $.get('./ws.php?method=galleries.init', function (data) {
        var info = $.parseJSON(data);
        if (info.stat == "ok") {
            $("#loadgif").hide();

            if (info.synroot == "") {
                _synroot = [];
            }
            else {
                _synroot = info.synroot.split("|");
            }

            for (var i = 0; i < info.cats.length; i++) {
                var cat = info.cats[i];
                if (cat.id == "") {
                    $("#folder").append(cat.tr);
                } else {
                    _jsonTr.push(cat);
                }
            }

            if (_album == "public") {
                $(".icon-download").hide();
            }

            setTimeout(function () {
                pre_data();
            }, 0);
        }
    });
}

function clear_dir() {
    _sent = 0;
    _pending = 0;
    _processing = 0;
    _pendingok = 0;
    _tobeclear = 0;
    _uploadedCats = "";
    _jsonPending = [];
    _jsonTr = [];
    _cancel = false;

    $("#startUpload").attr("disabled", "true");
    $("#btnadd").addClass("mydisabled");
    $("#btnclear").hide();
    $('.progress-extended span').html("");

    $('#folder tr').each(function () {
        if (!$(this).hasClass('tabtitle')) {
            $(this).remove();
        }
    });

    $("#loadgif").show();

    init();
}

function refresh_dir() {
    $("#btnadd").addClass("mydisabled");
    $('#btnadd').css({ 'pointer-events': 'none' });

    $('#info').delay(100).fadeOut();
    $.getJSON('./ws.php?method=categories.refresh', function (data) {
        clear_dir();
    });
}

function need_refresh() {
    if (_addFlag == 1) {
        return;
    }

    if ($("#progressbarall").is(":hidden") && $(".afterUploadActions").is(":hidden")) {
        $.post("./ws.php?method=galleries.change", "0", function (data, state) {
            var info = $.parseJSON(data);
            if (info.stat == "ok") {
                _reload = true;
                $('#aru').html($("#needtip").html());
                $('#aru2').hide();
                $('#info').delay(100).show();
            }
        });
    }
}

$(document).ready(function () {
    if (_album == "public") {
        $(".icon-download").hide();
        $(".normal_tab").show();
    }
    else {
        $("#startUpload").text(_encrypt);
    }

    init();

    $("a.showInfo").click(function () {
        $(".more").toggle();
        document.getElementById('content-wrapper').scrollTop = 2000;
        return false;
    });

    $("#cancelUpload").click(function () {
        _cancel = true;
        $("#cancelUpload").attr("disabled", "true");
        $("#cancel-progress").show(); $('#progressbarall').hide();
        $.post("./ws.php?method=ws.cancel3", "0", function (data, state) { });
        return false;
    });

    $("#startUpload").click(function () {
        myupload();
    });

    if (storage.idx == undefined) {
        storage.idx = "1";
    }
});
