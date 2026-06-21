
function u3down(path) {
    path = path.substring(path.indexOf("?") + 1);
    var idx = path.indexOf("&");
    if (idx > 0) {
        path = path.substring(0, idx);
    }

    var d = document.getElementById('down');
    d.href = "./action.html?download=action.u3down&" + path;
    d.click();
}

function cleanup() {
    $.get("./view.html?his=clear");
    $("#clean").empty();
    if ($("#sel").hasClass("my-center2")) {
        $("#sfile").html(_htm4);

        $("#sid2").css("text-align", "center");
    }
}

function folder(obj) {
    var pfile = obj.getAttribute("data");
    window.cefQuery({
        request: 'Prompt.q.viewfile:' + pfile
    });
}

function get_title(e) {
    var idx = e.detail.index;
    var title = "";

    if (e.detail.image && e.detail.image.naturalWidth) {
        title = e.detail.originalImage.alt
            + ' (' + e.detail.image.naturalWidth + '&times' + e.detail.image.naturalHeight + ') - '
            + (idx + 1) + '/' + _vr.length;
    }
    else {
        title = e.detail.originalImage.alt
            + ' - '
            + (idx + 1) + '/' + _vr.length;
    }

    window.cefQuery({
        request: 'Prompt.q.title:' + title.replace(/&times/, '×')
    });

    return title;
}

function selfile() {
    document.getElementById('fileinput0').click();
}

function selecthis(path, second_app) {

    if (second_app == 1) {
        path = path.getAttribute("data");
    }
    var reg = new RegExp("#", "g");
    //import
    path = path.replace(reg, ">");

    var idx = path.indexOf("/view.html");
    if (idx > 0) {
        return;
    }
    $("#sel").hide();

    $.ajax({
        type: 'GET',
        url: "./view.html?" + path,
        success: function (info) {
            $('#galley').empty();
            $('#clean').empty();

            var josn = $.parseJSON(info);
            if (_vr) {
                _vr.destroy();
            }
            if (josn.idx == "-1") {
                $("#sel").show();
                return;
            }
            $.colorbox.close();

            $("#sel").hide();
            $("#galley").prepend(josn.htm);
            $("#clean").prepend("<ul>" + josn.his + "</ul>");

            var galley = document.getElementById('galley');
            _vr = new Viewer(galley, {
                url: 'data-original-url',
                navbar: false,
                inline: true,
                transition: false,
                button: false,
                isfolder: true,
                initialViewIndex: josn.idx,
                second: true,
                autorotate: true,
                title: true,
                viewed: function (e) {
                    _idx = e.detail.index;
                    $('.viewer-title').html(get_title(e));
                    show_bar();
                }
            });
        }
    });
}

function openwith(fname) {
    var path = $("#box_" + _idx).attr('data-original-url');

    var idx = path.indexOf("f=");
    if (idx > 0) {
        path = path.substring(idx + 2);
    }

    idx = path.indexOf("&u=");
    if (idx > 0) {
        path = path.substring(0, idx);
    }

    window.cefQuery({
        request: 'Prompt.q.open:' + fname + "|" + path
    });
}

function u3del(idx) {
    var d = document.getElementById('del_box');
    d.href = "./delbox.html?type=0&nb=1&idx=" + _idx;
    d.click();
}

function reload_img(id) {
    _vr.reset();
}

function getSrc() {
    var path = _vr.getSrc();
    var idx = path.indexOf("f=");
    if (idx > 0) {
        path = path.substring(idx + 2);
    }

    idx = path.indexOf("&u=");
    if (idx > 0) {
        path = path.substring(0, idx);
    }
    return path;
}

function addto() {
    $("#sel").hide();

    if (_readonly != "0") {
        window.cefQuery({
            request: 'Prompt.q.jumpto:'
        });
    }
    else {
        $.colorbox({
            transition: "none", width: 566, height: 350, fixed: true, iframe: true, overlayClose: false, opacity: .7,
            title: _tit10,
            href: function () {
                return "./albbox.html?src=" + getSrc();
            },
            onComplete: function () {
                window.cefQuery({
                    request: 'Prompt.q.disabledrag:1'
                });
            },
            onClosed: function () {
                window.cefQuery({
                    request: 'Prompt.q.disabledrag:0'
                });
            }
        });
    }
}

function onpage() {
    if (_zflag == 1) {
        _zflag = 0;
        return false;
    }

    if ($("#sel").hasClass("my-center3")) {
        if ($("#sel").is(":hidden")) {
        }
        else {
            hide_up();
        }
    }
}

function hide_up() {
    $("#sel").removeClass("my-center3");
    $("#sel").addClass("my-center2");
    $("#selectable").removeClass("ul2");
    $("#sfile").html(_htm2);
    $("#sfile").attr("title", _tit2);

    $("#alb").hide();
    $("#sel").hide();
}

function show_up() {
    if ($("#sel").is(":hidden")) {
        _zflag = 1;
        $("#sel").removeClass("my-center2");
        $("#sel").addClass("my-center3");
        $("#selectable").addClass("ul2");
        $("#sfile").html(_htm3);
        $("#sfile").attr("title", _tit3);
        $("#sid2").css("text-align", "left");
        $("#alb").show();
        $("#sel").show();
    }
    else {
        hide_up();
    }
}

function show_bar() {
    if (_oncebar == 0) {
        _oncebar = 1;
        $('.viewer-title').show();
        $('.viewer-toolbar').show();
        $('.viewer-navbar').show();
        $('.viewer-title').fadeOut(3500);
        $('.viewer-toolbar').fadeOut(3500);
        $('.viewer-navbar').fadeOut(3500);
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

$(document).ready(function () {
    var o = document.getElementById("sel");
    if (o) {
        o.onmouseleave = function (e) {
            e.preventDefault();
            if ($("#sel").hasClass("my-center3")) {
                if ($("#sel").is(":hidden")) {
                }
                else {
                    hide_up();
                }
            }
        };
    }

    if (_hisempty == 1) {
        $("#sfile").html(_htm4);
        $("#sid2").css("text-align", "center");
    }
    else {
        $("#sfile").html(_htm2);
        $("#sid2").css("text-align", "left");
    }

    $(document).mousemove(function (e) {
        _in++;
        var off = $(document).height() - e.clientY;

        if (_vr && _vr.played) {
            if (e.clientY < 100) {
                $(".viewer-clock-btn").css('display', 'inline-block');
            }
            else {
                $(".viewer-clock-btn").css('display', 'none');
            }
        }

        if (off < 100) {
            $('.viewer-title').stop().fadeIn(100);
            $('.viewer-toolbar').stop().fadeIn(100);
            $('.viewer-navbar').stop().fadeIn(100);
        }
        else {
            if (_oy != 0 && _num < 1) {
                $('.viewer-title').stop().fadeIn(100);
                $('.viewer-toolbar').stop().fadeIn(100);
                $('.viewer-navbar').stop().fadeIn(100);
                _num++;
                _in = 0;
            }
            else {
                if (_in > 30 && _oy != 0 && _oy - e.clientY > 2) {
                    $('.viewer-title').stop().fadeOut();
                    $('.viewer-toolbar').stop().fadeOut();
                    $('.viewer-navbar').stop().fadeOut();
                    _in = 0;
                }
            }
        }
        _oy = e.clientY;

        if (_oncebar == 1) {
            if (e.clientX < 120) {
                $('.previous').fadeIn();
            }
            else {
                $('.previous').fadeOut();
            }

            off = $(document).width() - e.clientX;

            if (off < 120) {
                $('.next').fadeIn();
            }
            else {
                $('.next').fadeOut();
            }
        }
    });

    $(document).mouseleave(function (e) {
        $('.viewer-title').fadeOut(3500);
        $('.viewer-toolbar').fadeOut(3500);
        $('.viewer-navbar').fadeOut(3500);
    });

    $("a.del_box").colorbox({
        transition: "none", width: 566, height: 350, fixed: true, iframe: true, overlayClose: false, opacity: .6, closeButton: false,
        //   title: _tit20,
        onComplete: function () {
            $('#cboxOverlay').css({ 'background': 'slategray' });
            //$('#cboxWrapper').css({ 'border-top': 'solid 7px #e74c3c' });
            window.cefQuery({
                request: 'Prompt.q.disabledrag:1'
            });
        },
        onClosed: function () {
            document.getElementById('cboxOverlay').style.removeProperty('background');
            //document.getElementById('cboxWrapper').style.removeProperty('border-top');
            window.cefQuery({
                request: 'Prompt.q.disabledrag:0'
            });
        }
    });

    $.fn.uploader = function (options) {
        return this.each(function (index) {
            options = $.extend({
                dropZone: $(this),
                fileTypeWhiteList: ['jpg', 'png', 'jpeg', 'gif', 'webp', 'bmp', 'mp3', 'ogg', 'mp4', 'webm'],
                ajaxUrl: './ws.php?method=ws.viewer'
            }, options);

            var state = {
                fileBatch: [],
                isUploading: false,
                listIndex: 0
            };

            var dom = {
                uploaderBox: $(this),
                selectButton: $('<input style="height: 0; width: 0;" accept=".jpg,.jpeg,.png,.gif,.webp,.bmp,.mp3,.ogg,.mp4,.webm;" id="fileinput' + index + '" type="file" >')
            };

            dom.uploaderBox.append(dom.selectButton);

            function bindUIEvents() {
                options.dropZone.on('dragover dragleave', function (e) {
                    e.preventDefault();
                    e.dataTransfer.dropEffect = 'copy';
                    e.stopPropagation();
                });
                $.event.props.push('dataTransfer'); // jquery bug hack
                options.dropZone.on('drop', selectFilesHandler);

                dom.selectButton.on('click', function () { this.value = null; });
                dom.selectButton.on('change', selectFilesHandler2);

                $(document).on('dragover dragleave', '.viewer-container', function (e) {
                    e.preventDefault();
                    e.dataTransfer.dropEffect = 'copy';
                    e.stopPropagation();
                });
                $(document).on('drop', '.viewer-container', selectFilesHandler);
            }
            bindUIEvents();

            function addItem(file) {
                var fileName = cleanName(file.name);
                var fileSize = file.size;

                state.fileBatch.length = 0;
                state.listIndex = 0;
                state.isUploading = false;

                if (options.fileTypeWhiteList.indexOf(getExtension(file.name).toLowerCase()) !== -1) {
                    var id = state.listIndex;
                    state.listIndex++; state.fileBatch.push({ file: file, id: id, fileName: fileName, fileSize: fileSize });
                }
                else {
                    if (_vr) {
                        _vr.destroy();
                    }

                    $("#sel").show();
                    $("#sel").removeClass("my-center3");
                    $("#sel").addClass("my-center2");
                }
            }

            function getExtension(path) {
                var basename = path.split(/[\\\/]/).pop();
                var pos = basename.lastIndexOf('.');

                if (basename === '' || pos < 1) {
                    return '';
                }
                return basename.slice(pos + 1);
            }

            function cleanName(name) {
                name = name.replace(/\s+/gi, '-');
                return name.replace(/[^a-zA-Z0-9.\-]/gi, '');
            }

            function uploadSubmitHandler(ty) {

                if (state.fileBatch.length !== 0) {
                    var data = new FormData();
                    for (var i = 0; i < state.fileBatch.length; i++) {
                        data.append('files[]', state.fileBatch[i].file, state.fileBatch[i].fileName);
                    }
                    $.ajax({
                        type: "POST",
                        url: options.ajaxUrl,
                        data: data,
                        cache: false,
                        second: true,
                        contentType: false,
                        processData: false,
                        success: function (info) {
                            var josn = $.parseJSON(info);
                            if (_vr) {
                                _vr.destroy();
                            }

                            if (josn.idx == "-1") {
                                $("#sel").show();
                                return;
                            }

                            $('#galley').empty();
                            $('#clean').empty();
                            $("#sel").hide();

                            $("#galley").prepend(josn.htm);
                            $("#clean").prepend("<ul>" + josn.his + "</ul>");

                            var galley = document.getElementById('galley');
                            _vr = new Viewer(galley, {
                                url: 'data-original-url',
                                navbar: false,
                                inline: true,
                                transition: false,
                                button: false,
                                isfolder: true,
                                initialViewIndex: josn.idx,
                                second: true,
                                autorotate: true,

                                title: true,
                                viewed: function (e) {
                                    _idx = e.detail.index;
                                    $('.viewer-title').html(get_title(e));

                                    if ($("#sel").hasClass("my-center2")) {
                                        $("#sfile").html(_htm2);
                                    }
                                    show_bar();
                                }
                            });

                            if (ty == 2) {
                                setTimeout(function () {
                                    window.cefQuery({
                                        request: 'Prompt.q.active:0'
                                    });
                                }, 0);
                            }
                        }
                    });
                }
            }

            function selectFilesHandler(e) {
                e.preventDefault();
                e.stopPropagation();

                if (!state.isUploading) {
                    var files = e.target.files || e.dataTransfer.files || e.dataTransfer.getData;
                    for (var i = 0; i < files.length; i++) {
                        addItem(files[i]);
                    }
                }

                uploadSubmitHandler(1);
            }

            function selectFilesHandler2(e) {
                e.preventDefault();
                e.stopPropagation();

                if (!state.isUploading) {
                    var files = e.target.files || e.dataTransfer.files || e.dataTransfer.getData;
                    for (var i = 0; i < files.length; i++) {
                        addItem(files[i]);
                    }
                }

                uploadSubmitHandler(2);
            }
        });
    };
    $('.uploader__box').uploader();

    function selectfirst(idx0) {
        if (_vr) {
            _vr.destroy();
        }
        $("#sel").hide();
        var galley = document.getElementById('galley');
        _vr = new Viewer(galley, {
            url: 'data-original-url',
            navbar: false,
            inline: true,
            transition: false,
            button: false,
            isfolder: true,
            initialViewIndex: idx0,
            second: true,
            autorotate: true,

            title: true,
            viewed: function (e) {
                _idx = e.detail.index;
                $('.viewer-title').html(get_title(e));
                show_bar();
            }
        });
    }

    if (_idx0 >= 0) {
        setTimeout(function () { selectfirst(_idx0); }, 100);
    }
    else {
        $('#sel').show();
    }
});

document.ondragover = function (e) {
    e.preventDefault();
    return false;
};
document.ondrop = function (e) {
    e.preventDefault();
    return false;
};

window.cefQuery({
    request: 'Prompt.q.loading:1'
});
