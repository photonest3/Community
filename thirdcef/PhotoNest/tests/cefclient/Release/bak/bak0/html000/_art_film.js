
function exportStream(e) {
    if (_chunks.length) {
        _blob = new Blob(_chunks, { type: "video/webm" });
    }
}

function saveChunks(e) {
    e.data.size && _chunks.push(e.data);
}

function initAudioStream(evt) {
    if (!_aStream) {
        var audioCtx = new AudioContext();
        var dest = audioCtx.createMediaStreamDestination();
        _aStream = dest.stream;

        var sourceNode = audioCtx.createMediaElementSource(this);
        sourceNode.connect(dest);
    }

    if (!_analyser) {
        _analyser = audioCtx.createAnalyser();
        sourceNode.connect(_analyser);
        _analyser.fftSize = 2048;
        var bufferLength = _analyser.frequencyBinCount;
        _dataArray = new Uint8Array(bufferLength);
        _analyser.getByteTimeDomainData(_dataArray);
        sourceNode.connect(audioCtx.destination);
    }
}

function startRec() {
    startVideo();
    $('#tit0').text(_tit0);
    $('.box').show();

    $('#ele').hide();
    $('#ply').hide();

    var canvas = document.querySelector("canvas");
    canvas.style.width = _theW;
    canvas.style.height = _theH;

    _cStream = canvas.captureStream();


    if (_musci_src && _musci_src != "") {
        var interval = setInterval(function () {
            if (_aStream) {
                clearInterval(interval);
                _cStream.addTrack(_aStream.getAudioTracks()[0]);
                var options = { mimeType: "video/webm" };
                _recorder = new MediaRecorder(_cStream, options);

                _recorder.start();
                _recorder.ondataavailable = saveChunks;
                _recorder.onstop = exportStream;
            }

        }, 10);
    }
    else {
        //var options = { mimeType: "video/webm; codecs=vp9" };
        var options = { mimeType: "video/webm" };
        _recorder = new MediaRecorder(_cStream, options);

        _recorder.start();
        _recorder.ondataavailable = saveChunks;
        _recorder.onstop = exportStream;
    }
}

function onend() {
    $('.jmn-video-pause').hide();
    $('.jmn-video-play').show();

    if (_musci_src && _musci_src != "") {
        if (_bgvdo) {
            _bgvdo.pause();
        }
    }

    if (_recorder && _recorder.state != 'inactive') {
        $('#ply').css('display', 'inline-block');
        $('#ele').css('display', 'inline-block');
        $('#tit0').text(_tit1);

        _recorder.stop();
    }
}

function playbox() {
    if (_blob) {
        $.colorbox({
            transition: "none", width: "85%", height: "85%", fixed: true, opacity: 1,
            html: '<div class="vdobox"><video id="vdo" disablepictureinpicture="true" controls="controls" autoplay="autoplay" style="vertical-align: middle;max-width: 100%;background-color:black;height: 95%; "><source src="' + URL.createObjectURL(_blob) + '" type="video/webm"> </video></div>',
            onComplete: function () {
				$('body').css('overflow-y', 'hidden');
				$('#cboxLoadedContent').css('background','#000');
				$('#cboxOverlay').css('background-color', '#000');
				$('#cboxWrapper').css('background-color', '#000');
				$('#cboxClose').removeClass('stm-icon-cancel2').addClass('stm-icon-cancel3');
                parent.$("#cboxClose").hide();
            },
            onClosed: function () {
				document.body.style.removeProperty('overflow-y');			
				document.getElementById('cboxOverlay').style.removeProperty('background-color');
				document.getElementById('cboxWrapper').style.removeProperty('background-color');
				$('#cboxClose').removeClass('stm-icon-cancel3').addClass('stm-icon-cancel2');
                parent.$("#cboxClose").show();
            }
        });
    }
}

function downloadmp4() {
    if (_blob) {
        var al = document.createElement("a");
        al.href = URL.createObjectURL(_blob);
        document.body.appendChild(al);
        al.download = (_title1 == "") ? "ii" : _title1;
        al.style.display = 'none';
        al.click();
        document.body.removeChild(al);
    }
}

var startVideo = function () {
    if (diaporama.currentTime >= diaporama.duration) {
        diaporama.currentTime = 0;
    }

    $('.jmn-video-play').hide();
    $('.jmn-video-pause').show();

    if (_musci_src && _musci_src != "") {
        _bgvdo = document.getElementById('bgvdo');
        _bgvdo.crossOrigin = 'anonymous';
        _bgvdo.oncanplay = initAudioStream;
        _bgvdo.src = _musci_src;

        _bgvdo.play();
    }

    diaporama.play();
};

var stopRec = function () {
    $('.jmn-video-pause').hide();
    $('.jmn-video-play').show();

    diaporama.pause();
    if (_musci_src && _musci_src != "") {
        if (_bgvdo) {
            _bgvdo.pause();
        }
    }
};

var sel_music = function () {
    document.getElementById('file').click();
};

var file_change = function () {
    var file = document.getElementById('file');
    _musci_src = URL.createObjectURL(file.files[0]);
    if (_readonly == "0") {
        update_art(_artid, _title1, _title2, "1");
    }
    file.value = '';
    stopRec();
    startVideo();
};

function to_film() {
    location.reload();
}

function to_wall() {
    location.replace("./index.html?act=index.wall&ids=" + _ids.join(",") + "&artid=" + _artid);
}
function to_zoom() {
    location.replace("./index.html?act=index.zoom&ids=" + _ids.join(",") + "&artid=" + _artid);
}

function get_w(idx) {
    if (idx >= _widths.length || idx < 0) {
        idx = 0;
    }

    var aspectRatio = _widths[idx] / _heights[idx];
    var w = aspectRatio * _theH;
    w = w < 10 ? 500 : w;
    w = w > window.innerWidth ? 500 : w;
    return w;
}

function down() {
    _speed -= 1;
    if (_speed == 0) {
        _speed = 0;
        diaporama.playbackRate = 0.5;
        $('#speed').text('0.5 ×');
    }
    else if (_speed < 0) {
        _speed = 0;
    }
    else {
        diaporama.playbackRate = _speed;
        $('#speed').text(_speed + ' ×');
    }
}

function up() {
    _speed += 1;
    if (_speed > 4) {
        _speed = 4;
        return;
    }
    diaporama.playbackRate = _speed;
    $('#speed').text(_speed + ' ×');
}

function get_title(idx) {
    var id = _ids[idx];
    $.get("./picture.html?act=picture.get_data&zoom=2&picid=" + id, function (data, state) {
        var info = $.parseJSON(data);
        if (info.stat == "ok") {
            if (_title1 != "") {
                $('.viewer-title').text(_title1 + " / " + info.result.detail);
            }
            else {
                $('.viewer-title').text(info.result.detail);
            }
            if (info.result.width && info.result.height) {
                if (_widths[idx] != info.result.width || _heights[idx] != info.result.height) {
                    _widths[idx] = info.result.width;
                    _heights[idx] = info.result.height;
                    myresize(idx);
                }
            }
        }
    });
}

function myresize(idx) {
    var w = get_w(idx);
    var h = _theH;
    diaporama.width = w;
    diaporama.height = h;
    diaporama.resolution = Math.min(window.devicePixelRatio || 1, Math.ceil((_threshold / (w * h))));

    $('#diaporama canvas').css("margin-top", (window.innerHeight - h) / 2 - 82);
}

function formatDuring(mss) {
    var days = parseInt(mss / (1000 * 60 * 60 * 24));
    var hours = parseInt((mss % (1000 * 60 * 60 * 24)) / (1000 * 60 * 60));
    var minutes = parseInt((mss % (1000 * 60 * 60)) / (1000 * 60));
    var seconds = parseInt((mss % (1000 * 60)) / 1000);
    hours = hours == 0 ? '' : hours + ' ';
    minutes = minutes < 10 ? '0' + minutes : minutes;
    seconds = seconds < 10 ? '0' + seconds : seconds;
    return hours + minutes + ":" + seconds;
}

function to_mycreate() {
    parent.window.location = "./index.html?art_list=1";
}

function save_art() {
    if (_readonly != "0") {
        $("#err").html(_err_readonly);
        $('#err').delay(100).show();
        $('#err').delay(2000).fadeOut();
        return;
    }
    $.colorbox({
        transition: "none", width: 457, height: 280, fixed: true, iframe: true, overlayClose: false, opacity: .7,
        title: _tit20,
        href: function () {
            return "./film_box.html?ids=" + _ids.join(",");
        }
    });
}

function update_art(artid, title1, title2, type) {
    var fileObj = document.getElementById("file").files[0];
    var formFile = new FormData();
    formFile.append("file", fileObj);

    var url = "./ws.php?method=ws.update_art&type=film&photos=" + _ids.join(",") + "&artid=" + artid;
    url += "&title1=" + encodeURIComponent(title1);
    url += "&title2=" + encodeURIComponent(title2);

    $.ajax({
        type: "POST",
        url: url,
        data: formFile,
        cache: false,
        second: true,
        contentType: false,
        processData: false,
        success: function (info) {
            if (info == "ok") {
                if (type == "0") {
                    location.replace(location.href + "&reload=1");
                }
                else {
                    $.colorbox.close();
                    $('#infos').delay(100).show();
                    $('#infos').delay(2000).fadeOut();
                }
            }
            else {
                $.colorbox.close();
                $('#infos').delay(100).show();
                $('#infos').delay(2000).fadeOut();
            }
        }
    });
}

$(document).ready(function () {

    if (_reload == "1") {
        $('#infos').delay(100).show();
        $('#infos').delay(2000).fadeOut();
    }

    var _data = {
        timeline: [
            {
                "slide2d": {
                    "background": "#89B",
                    "size": [800, 600],
                    "draws": [
                        { "font": "bold 80px sans-serif", "fillStyle": "#fff", "textBaseline": "middle", "textAlign": "center" },
                        ["fillText", _title1, 400, 250],
                        { "font": "normal 80px sans-serif", "fillStyle": "#fff", "textBaseline": "middle", "textAlign": "center" },
                        ["fillText", _title2, 400, 350]
                    ]
                },
                "duration": 1000,
                "transitionNext": {
                    "duration": 3000
                }
            }
        ].concat(_pics.map(function (src) {
            return {
                image: src,
                duration: 2000,
                kenburns: {
                    from: [0.6, [0.5, 0.5]],
                    to: [1, [0.5, 0.5]]
                    //	,        easing: [ 0, 0.1, 0.9, 1 ]
                    //                                from: [0.5, [0.4, 0.4]],
                    //                        to: [1, [0.6, 0.6]]
                },
                transitionNext: {
                    duration: 2000,
                    name: "circleopen"//burn"//morph"//circleopen//fade
                }
            };
        })),
        transitions: [
            {
                "id": "35e8c18577278e",
                "name": "circleopen",
                "owner": "gre",
                "uniforms": { "smoothness": 0.8, "opening": true },
                "glsl": "\n#ifdef GL_ES\nprecision highp float;\n#endif\nuniform sampler2D from;uniform sampler2D to;uniform float progress;uniform vec2 resolution;uniform float smoothness;uniform bool opening;const vec2 center=vec2(0.5,0.5);const float SQRT_2=1.414213562373;void main(){vec2 p=gl_FragCoord.xy/resolution.xy;float x=opening?progress:1.-progress;float m=smoothstep(-smoothness,0.0,SQRT_2*distance(center,p)-x*(1.+smoothness));gl_FragColor=mix(texture2D(from,p),texture2D(to,p),opening?1.-m:m);}"
            }
        ]
    };

    _theH = parseInt(Math.max.apply(null, _heights));
    _theH = _theH < 500 ? 500 : _theH;
    _theH = _theH > window.innerHeight - 200 ? window.innerHeight - 200 : _theH;
    _theW = get_w(_init);

    $('#tit0').text('');
    if (_nb > 0) {
        $('#filename').text(_title1);
        get_title(0);
        var diaporama = Diaporama(
            document.querySelector("#diaporama"),
            {
                data: _data,
                width: _theW,
                height: _theH,
                autoplay: false,
                loop: false,
                resolution: 1,
                renderingMode: "canvas"
            }
        );
        window.diaporama = diaporama;

        var pro = new Progress('.progress', {
            val: 0,
            size: 4,
            precision: 2,
            drag: true,
            direction: 'horizontal',
            tip: false,
            getVal: function (el) {
                stopRec();
                var currTime = el.val * diaporama.duration / 100;
                $('.jmn-video-start').text(formatDuring(currTime));
                diaporama.currentTime = currTime;
            }
        });

        $('#toolbar').show();
        $('#actbar').show();
        $('.jmn-video-pause').hide();
        $('.jmn-video-play').show();
        $('.jmn-video-start').text(formatDuring(0));

        diaporama.on('progress', function (timeBuffered, loaded, total) {
            $('#diaporama').show();
            $('.jmn-video-end').text(formatDuring(diaporama.duration));
            $('.box').show();
            $('#diaporama canvas').css("margin-top", (window.innerHeight - _theH) / 2 - 82);
        });

        diaporama.on('slide', function (transObj) {
            var i = diaporama.slide;
            if (_total != 0) {
                _idx = Math.ceil(diaporama.currentTime / 4000);
                if (_idx > _total || _idx < 0) {
                    _idx = 0;
                    return;
                }
                var cur = _idx;

                if (cur == 0) {
                    cur = 1;
                }
                $('#photos').text(cur + " / " + _total);
                get_title(i);
            }

            myresize(i);
        });
    }

    diaporama.on('render', function (currTime, renderStatus) {
        setTimeout(function () {
            var cur = currTime * 100 / diaporama.duration;
            pro.updateVal(cur);
            var seconds = parseInt(currTime % 60);
            if (_seconds != seconds) {
                _seconds = seconds;
                $('.jmn-video-start').text(formatDuring(currTime));
            }
        });
    });

    diaporama.on('play', function (currTime, renderStatus) {
    });

    diaporama.on('pause', function (currTime, renderStatus) {
        onend();
    });

    diaporama.on('ended', function (currTime, renderStatus) {
        onend();
    });


    $('.jmn-video-play').click(function () {
        var txt = $('#tit0').text();
        if (txt == "" || txt == _tit1) {
            $("#rec").attr("title", _rec0);
            startRec();
        }

    });

    $('.jmn-video-pause').click(function () {
        stopRec();
    });

    $('.jmn-video-prev').click(function () {
        down();
    });

    $('.jmn-video-next').click(function () {
        up();
    });

    document.body.addEventListener("keydown", function (e) {
        switch (e.which) {
            case 38: // Up
                up();
                break;
            case 40: // Down
                down();
                break;
            case 37: // Left
                diaporama.prev();
                break;
            case 39: // Right
                diaporama.next();
                break;
            case 32: // Space
                diaporama.paused = !diaporama.paused;
                break;
        }
    });

});
