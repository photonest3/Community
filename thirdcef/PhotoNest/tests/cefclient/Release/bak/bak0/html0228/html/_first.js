
function get_pending(ty) {
    $.get("./index.html?act=index.get_pending&ty=" + ty, function (data, state) {
        var info = $.parseJSON(data);
        if (info.stat == "err") {
            setTimeout("get_pending('0')", 700);
        }
        else if (info.stat == "ok") {
            var nb = parseInt(info.nb);
            if (_pending != nb) {
                location.reload();
            }
            else {
                if (nb <= 0) {
                    return;
                }

                if (_album == "public" || _acc != "0") {
                    $("#startUpload").removeAttr("disabled");
                    $("#startUpload").css("background-color", "#ff7700");
                }
            }
        }
    });
}

function myactivate(activate) {
    var storage = window.localStorage;
    if (activate == "1") {
        if (storage.idx == "0") {
            if ($("#progressbarall").is(":hidden") && !$(".selectFiles").is(":hidden")) {
                $("#startUpload").attr("disabled", "true");
                $("#startUpload").css("background-color", "#999");
                setTimeout("get_pending('1')", 100);
            }
        }
    }

    storage.idx = activate;
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
    notify_uploading("0");
    var storage = window.localStorage;
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
    _root_id = root_id;

    window.cefQuery({
        request: 'Prompt.q.dir:'
    });
}

function show_err(txt) {
    if (_root_id != "") {
        $('#lbl1').html(txt);
        $('#info1').delay(100).show();
        $('#info1').delay(2000).fadeOut();
    }
    else {
        $('#aru').html(txt);
        $('#aru2').hide();
        $('#info').delay(100).show();
        $('#info').delay(2000).fadeOut();
    }
}

function cefQuerySuccess(response) {
    var obj = $("#lbl1");
    if (location.href.indexOf("changed=1") > 0) {
        obj = $("#aru");
    }
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

    if (_root_id == "") {
        var tmp = t0 + "/";
        for (i = 0; i < _synroot.length; i++) {
            t1 = _synroot[i].toLowerCase() + "/";
            if (tmp.indexOf(t1) >= 0) {
                show_err(_label3);
                return;
            }
        }
    }

    $.get("./ws.php?method=galleries.addfolder&firstrun=1&root_id=" + _root_id + "&folder=" + encodeURIComponent(response), function (data, state) {
        var info = $.parseJSON(data);
        if (info.stat == "ok") {
            if (_root_id == "1") {
                location.href = "./admin.html?page=admin.firstrun&reload=1&changed=1";
            }
            else {
                location.reload();
            }
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
    //var image_ids = uploadedPhotos.join(",");
    var nb = 0;
    for (var i = 0; i < uploadedPhotos.length; i++) {
        if (uploadedPhotos[i] != 0) {
            nb++;
        }
    }

    $.getJSON('./ws.php?method=upload.complete&id=' + catids + '&cancel=' + cancel + "&nb=" + uploadedPhotos.length, function (data) {
        location.replace("./b.html?nb=" + nb);
        //$(".selectFiles").hide();
    });
}

function fupload_err(obj) {
    window.cefQuery({
        request: 'Prompt.q.fupload_err:' + $(obj).attr("fname")
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
                            var html_ = '';
                            var fullname = '';

                            if (r.stat == "ok") {
                                fullname = res.fullname;
                                uploadedPhotos.push(parseInt(res.image_id));
                            }
                            else {
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
    if (_count == 0) {
        if (_all_ok == 0) {
            if (_pending != 0) {
                if (_album == "public" || _acc != "0") {
                    $("#startUpload").removeAttr("disabled");
                    $("#startUpload").css("background-color", "#ff7700");
                }

                var title = _penTitle2;
                if (_album == "public") {
                    title = _penTitle;
                }
                title = title.replace(/nb1/, _pending);
                $('.progress-extended').html(title);
            }
        }
    }
}

function pre_data() {
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

                        _pending += parseInt(info.pending);
                        if (info.state != "ok") {
                            _all_ok = 0;
                        }
                        _pre_sending--;
                    }
                    _count--;
                    handle();
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
        $("#startUpload").css("background-color", "#999");
        $.post("./ws.php?method=galleries.categories", "ids=" + ids, function (data, state) {
            var info = $.parseJSON(data);
            if (info.stat == "ok") {
                if (info.files.length > 0) {
                    $("#uploadingActions").show();
                    $("#btnadd").hide();
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

$(document).ready(function () {
    var storage = window.localStorage;
    if (storage.idx == undefined) {
        storage.idx = "1";
    }

    if (_error != "0") {
        $(".jquery-ui").css("overflow-y", "hidden");
        $(".addAlbumEmptyCenter0").hide();
        $("#content-wrapper").hide();
        $("#err0").show();
        return;
    }

    if (_album == "public") {
        $("#wel_title").html(_wel_title1);
        window.cefQuery({
            request: 'Prompt.q.enckey:'
        });
    }
    else {
        $("#wel_title").html(_wel_title2);
        $("#myfd").text(_myfd);
        $("#startUpload").text(_encrypt);
        $("#td_upload").text(_encrypted);
        $("#td_tobe").text(_toenc);
    }
    $(".wel").show();

    if (_album == "public" || _acc != "0") {
        $("#startUpload").css("background-color", "#ff7700");
        $("#createAccount").hide();
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
                $("#startUpload").css("background-color", "#999");
            }
        });
    }

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
        if (_album == "private" && _acc == "0") {
            return;
        }

        $('#info2').hide();
        $('.icon-plus').css({ 'pointer-events': 'none', 'opacity': '.7' });
        $('.icon-cancel').css({ 'pointer-events': 'none', 'opacity': '.7' });

        $("#startUpload").attr("disabled", "true");
        $("#startUpload").css("background-color", "#999");
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
    });

    $("#createAccount").click(function () {
        $.colorbox({
            transition: "none", width: "647", height: "400", fixed: true, maxWidth: 1140, maxHeight: 495, overlayClose: false, escKey: false, iframe: true,
            href: "./register.html", title: _acctitle,
            onClosed: function () {
                $.get('./ws.php?method=galleries.pwd', function (data) {
                    if (data == "1") {
                        _acc = "2";
                        $("#createAccount").hide();

                        if (_all_ok == 0) {
                            $("#startUpload").removeAttr("disabled");
                            $("#startUpload").css("background-color", "#ff7700");
                        }
                    }
                });
            }
        });
    });

});
