

function myactivate(activate) {
    var storage = window.localStorage;
    if (activate == "1") {
        if (storage.idx == "0") {
            var sub = Math.ceil((new Date().getTime() - storage.curr) / 1000);
            if (sub > 20) {
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

    $.getJSON('./ws.php?method=upload.complete&cancel=2', function (data) {

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

function notify_uploading(val) {
    var storage = window.localStorage;
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
    //.replace(/\//g, "\\\\");
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
    var catids = uploadedCats.join(",");

    $.getJSON('./ws.php?method=upload.complete&id=' + catids + '&cancel=' + cancel, function (data) {
        location.replace("./b.html?nb=" + data.nb);
        //$(".selectFiles").hide();
    });
}

function fupload_err(obj) {
    window.cefQuery({
        request: 'Prompt.q.fupload_err:' + $(obj).attr("fname")
    });
}

function progressall() {
    if (_sent < _pending) {
        var progress = Math.floor(_sent * 100 / _pending);

        $('.progress-extended').html(_sent + ' / ' + _pending);
        $('.fileupload-progress')
            .find('.progress')
            .attr('aria-valuenow', progress)
            .children()
            .first()
            .css('width', progress + '%');
    }
}

function handle() {
    if (_all_ok == 0) {
        if (_pending != 0) {
            $("#startUpload").removeAttr("disabled");

            var title = _penTitle2;
            if (_album == "public") {
                title = _penTitle;
            }
            title = title.replace(/nb1/, _pending);
            $('.progress-extended').html(title);
        }
    }
}

function pre_data() {
    _pendingok = 0;

    var folders = $("#folder").find('.root_tr');
    for (var i = 0; i < folders.length; i++) {
        var id = $(folders[i]).attr('rootid');
        $.getJSON('./ws.php?method=galleries.pending&first=1&id=' + id, function (data) {
            if (data.stat == "ok") {
                for (var j = 0; j < data.result.length; j++) {
                    var info = data.result[j];
                    $('#trid' + info.id).find('.uploaded').text(info.uploaded);
                    $('#trid' + info.id).find('.pending').text(info.pending);
                    $('#trid' + info.id).find('.state').attr('state', info.state);
                    $('#trid' + info.id).find('.progress').attr('sizes', info.sizes);

                    _pending += parseInt(info.pending);
                    if (info.state != "ok") {
                        _all_ok = 0;
                    }

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
                _sent = _pending;
                $('.progress-extended').html(_sent + ' / ' + _pending);
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
    var ids = "";
    var folders = $("#folder").find('.state');
    for (var i = 0; i < folders.length; i++) {
        var folder = folders[i];
        var id = $(folder).attr('catid');
        var state = $(folder).attr('state');
        if (state != "ok") {
            ids += id + ",";
            if (uploadedCats.indexOf(id) < 0) {
                uploadedCats.push(id);
            }
        }
    }

    if (ids == "") {
        notify_uploading("0");
        complete("0");
    }
    else {
        setTimeout("progress3()", 100);
        $('.progress-extended').show();
        $('#progressbarall').show();

        $("#startUpload").attr("disabled", "true");

        $.post("./ws.php?method=galleries.categories", "ids=" + ids, function (data, state) {
        });
    }
}


function remove_folder(rootid) {
    $.post("./ws.php?method=galleries.remove&first=1", "rootid=" + rootid, function (data, state) {
        var info = $.parseJSON(data);
        if (info.stat == "ok") {
            location.reload();
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

function need_refresh() {

    if ($("#stripeddiv").is(":hidden")) {
        return;
    }
    if ($("#progressbarall").is(":hidden")) {
        $("#needtip").show();
    }



}
function clear_click() {
    location.reload();
}

$(document).ready(function () {
    var storage = window.localStorage;
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
        $("#td_upload").text(_encrypted);
        $("#td_tobe").text(_toenc);
        $("#titleh1 span").html(_title2);
        $("#titleh1 span").html(_title2);
    }

    if (location.href.indexOf("changed=1") > 0) {
        $.get('./ws.php?method=galleries.init0', function (data) {
            if (data != " ") {
                $("#folder").append(data);
                setTimeout(function () {
                    pre_data();
                }, 100);
            }
            else {
                $("#startUpload").attr("disabled", "true");
            }
        });
    }

    $("#cancelUpload").click(function () {
        notify_uploading("0");
        setTimeout(function () {
            complete("1");
            $("#uploadingActions").hide();
        }, 0);
        return false;
    });

    $("#startUpload").click(function () {

        $('#info2').hide();
        $('.icon-plus').css({ 'pointer-events': 'none', 'opacity': '.7' });
        $('.icon-cancel').css({ 'pointer-events': 'none', 'opacity': '.7' });

        $("#startUpload").attr("disabled", "true");
        _processing = 1;

        if (_all_ok == 1) {
            $.post("./ws.php?method=galleries.clear", "rename=1", function (data, state) {
                var info = $.parseJSON(data);
                if (info.stat == "ok") {
                    window.cefQuery({
                        request: 'Prompt.q.syn_root:'
                    });

                    proc_upload();
                    _processing = 0;
                    $("info2").html(_ok);
                    $("info2").show();
                    $('.icon-plus').css({ 'pointer-events': 'auto', 'opacity': '1' });
                    $('.icon-cancel').css({ 'pointer-events': 'auto', 'opacity': '1' });
                }
            });
            notify_uploading("0");
            complete("0");
            return false;
        }

        notify_uploading("1");

        $('#progressbarall').show();
 $("#uploadingActions").show();
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
    });


});
