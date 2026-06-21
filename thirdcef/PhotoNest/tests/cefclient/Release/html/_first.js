
function myactivate(activate) {
    if (_readonly != "0") {
        return;
    }
    if (activate == "1") {
        if (storage.idx == "0") {
            var sub = Math.ceil((new Date().getTime() - storage.curr) / 1000);
            if (sub > 6) {
                setTimeout("need_refresh()", 100);
            }
        }
    }

    storage.idx = activate;
    if (activate == "0") {
        storage.curr = new Date().getTime();
    }
}

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
    $("#cancelUpload").click();
}

function notify_uploading(val) {
    storage.b = val;
    window.cefQuery({
        request: 'Prompt.q.uploading:' + val
    });
}

function selfol(root_id) {
    if (_readonly != "0") {
        $("#err0").addClass("err2");
        $("#err0").html(_err_readonly);
        $('#err0').delay(100).show();
        $('#err0').delay(2000).fadeOut();
        return true;
    }

    if (_processing == 1) {
        return;
    }

    if ($("#tips3").is(":hidden") || root_id == '0') {
        var response = _sample;
        $.get("./ws.php?method=galleries.addfolder&firstrun=1&folder=" + encodeURIComponent(response), function (data, state) {
            var info = $.parseJSON(data);
            if (info.stat == "ok") {
                location.replace("./admin.html?page=admin.firstrun&reload=1&changed=1");
            }
        });
    }
    else {
        window.cefQuery({
            request: 'Prompt.q.dir:'
        });
    }
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

    $.get("./ws.php?method=galleries.addfolder&firstrun=1&folder=" + encodeURIComponent(response), function (data, state) {
        var info = $.parseJSON(data);
        if (info.stat == "ok") {
            location.replace("./admin.html?page=admin.firstrun&reload=1&changed=1");
        }
    });
}

function profile(obj) {
    var v = $(obj).val();
    $.get("./ws.php?method=galleries.language&language=" + v, function (data, state) {
        var info = $.parseJSON(data);
        if (info.stat == "ok") {
            location.reload();
        }
    });
}

function openfol(obj) {
    window.cefQuery({
        request: 'Prompt.q.folder:' + $(obj).attr('root')
    });
}

function complete(cancel) {
    $.getJSON('./ws.php?method=upload.complete&id=' + _uploadedCats + '&cancel=' + cancel, function (data) {
        location.replace("./b.html?nb=" + data.nb + "&err30=" + data.err30 + "&err40=" + data.err40);
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

function handle() {
    if (_pending != 0) {
        $("#startUpload").removeAttr("disabled");

        var title = _penTitle2;
        if (_album == "public") {
        }
        title = _penTitle;
        title = title.replace(/nb1/, _pending);
        $('.progress-extended').html(title);
    }
    $("#btnadd").removeClass("mydisabled");
    $('#btnadd').css({ 'pointer-events': 'auto' });

    if (_reload) {
        $('#info').delay(100).fadeOut();
        _reload = false;
    }
}

function pre_data() {
    _pendingok = 0;

    var folders = $("#folder").find('.root_tr');
    for (var i = 0; i < folders.length; i++) {
        var roottr = $(folders[i]);
        var rootid = roottr.attr('rootid');
        var catid = roottr.attr('catid');

        $.getJSON('./ws.php?method=galleries.pending&first=1&rootid=' + rootid + '&catid=' + catid + "&idx0=" + i, function (data) {
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

function ispending_ok() {
    var folders = $("#folder").find('.root_tr');
    if (_pendingok == folders.length) {
        $.getJSON('./ws.php?method=galleries.pendingok', function (data) {
            handle();
        });
    }
    else {
        setTimeout(function () {
            ispending_ok();
        }, 100);
    }
}

function progress3() {
    $.post("./ws.php?method=ws.progress3", "0", function (data, state) {
        var info = $.parseJSON(data);
        if (info.stat == "ok") {
            var len = parseInt(info.len);
            _sent = parseInt(info.step);
            progressall();
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
        $("#startUpload").attr("disabled", "true");
        $('#refresha').css({ 'pointer-events': 'none' });

        $.post("./ws.php?method=galleries.categories", "ids=" + _uploadedCats, function (data, state) {
            setTimeout("progress3()", 100);
        });
    }
}

function remove_folder(rootid) {
    $.post("./ws.php?method=galleries.remove&first=1", "rootid=" + rootid, function (data, state) {
        var info = $.parseJSON(data);
        if (info.stat == "ok") {
            location.href= "./index.html?page=admin.firstrun&reg=1";
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

function clear_dir() {
    _sent = 0;
    _pending = 0;
    _processing = 0;
    _pendingok = 0;
    _tobeclear = 0;
    _uploadedCats = "";
    _jsonPending = [];
    _jsonTr = [];

    $("#startUpload").attr("disabled", "true");
    $("#btnadd").addClass("mydisabled");
    $('.progress-extended span').html("");

    $('#folder tr').each(function () {
        if (!$(this).hasClass('tabtitle')) {
            $(this).remove();
        }
    });

    $("#loadgif").show();

    init();
}

function init() {
    $("#btnadd").addClass("mydisabled");
    $('#btnadd').css({ 'pointer-events': 'none' });

    $.get('./ws.php?method=galleries.init0', function (data) {
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

            var folders = $("#folder").find('.root_tr');
            if (folders.length > 0 || _reg == "1") {
                $("#tips3").show();
                $('#btnadd').css({ 'font-size': '13px' });
                $('#btnadd').attr("title", _tit9);
                $('#btnadd').html(_addtitle1);
			}
            else {
                $("#tips3").hide();
                $('#btnadd').css({ 'font-size': '18px' });
                $('#btnadd').attr("title", "");
                $('#btnadd').html(_addtitle2);
                 
			}
			if (_reg == "1" && folders.length == 0 && _sample.indexOf("photonest_app")>0) {
				$("#btnsample").show();
			}

            setTimeout(function () {
                pre_data();
            }, 0);
        }
    });
}

function refresh_dir() {
    $("#btnadd").addClass("mydisabled");
    $('#btnadd').css({ 'pointer-events': 'none' });
    $('#info').delay(100).fadeOut();
    $.getJSON('./ws.php?method=categories.refresh&first=1', function (data) {
        clear_dir();
    });
}

function need_refresh() {
    if (_addFlag == 1) {
        _addFlag = 0;
        return;
    }
    if ($("#stripeddiv").is(":hidden")) {
        return;
    }

    if ($("#progressbarall").is(":hidden")) {
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
    if (storage.idx == undefined) {
        storage.idx = "1";
    }

    if (_error != "0") {
        $(".jquery-ui").css("overflow-y", "hidden");
        $(".RenameTagPopInContainer").hide();
        $("#wrapper").hide();
        $("#err0").show();
        return;
    }

    if (_album == "public") {
        window.cefQuery({
            request: 'Prompt.q.enckey:'
        });
    }
    else {
        $("#startUpload").text(_encrypt);
    }

    init();

    $("#cancelUpload").click(function () {
        _cancel = true;
        $("#cancelUpload").attr("disabled", "true");
        $("#cancel-progress").show(); $('#progressbarall').hide();
        $.post("./ws.php?method=ws.cancel3", "0", function (data, state) { });
        return false;
    });

    $("#startUpload").click(function () {
        $('#info2').hide();
        $('.icon-plus').css({ 'pointer-events': 'none', 'opacity': '.7' });
        $('.icon-cancel').css({ 'pointer-events': 'none', 'opacity': '.7' });

        $("#prof_language").attr("disabled", "true");
        $("#startUpload").attr("disabled", "true");
        _processing = 1;

        notify_uploading("1");

        $('#progressbarall').show();
        $("#uploadingActions").show();
        $("#cancelUpload").removeAttr("disabled");

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
    });

});
