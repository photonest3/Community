var last_clicked = 0,
    last_clickedstatus = true;
jQuery.fn.enableShiftClick = function () {
    var inputs = [],
        count = 0;
    this.find('input[type=checkbox]').each(function () {
        var pos = count;
        inputs[count++] = this;
        $(this).bind("shclick", function (dummy, event) {
            if (event.shiftKey) {
                var first = last_clicked;
                var last = pos;
                if (first > last) {
                    first = pos;
                    last = last_clicked;
                }

                for (var i = first; i <= last; i++) {
                    input = $(inputs[i]);
                    $(input).prop('checked', last_clickedstatus).trigger("change");
                    if (last_clickedstatus) {
                        $(input).closest("li").addClass("thumbSelected");
                    }
                    else {
                        $(input).closest("li").removeClass("thumbSelected");
                    }
                }
            }
            else {
                last_clicked = pos;
                last_clickedstatus = this.checked;
            }
            return true;
        });
        $(this).click(function (event) { $(this).triggerHandler("shclick", event); });
    });
};

function filter_enable(filter) {
    $("#" + filter).show();
    $("input[type=checkbox][name=" + filter + "_use]").prop("checked", true);
    $("#addFilter").children("option[value=" + filter + "]").attr("disabled", "disabled");
}

function filter_disable(filter) {
    $("#" + filter).hide();
    $("input[name=" + filter + "_use]").prop("checked", false);

    $("input[data-wid=" + filter + "_use]").prop("checked", false);
}

function gourl(url) {
    location = url;
}

function addalb() {
    if (_readonly != "0") {
        $("#err").html(_err_readonly);
        $('#err').delay(100).show();
        $('#err').delay(2000).fadeOut();
        return;
    }
    $('[data-add-album]').addAlbum();
}

function field_show() {
    $('#filter_prefilter').hide();
    $('#filter_search').hide();
    $('#filter_category').hide();
    $('#filter_tags').hide();
    $('#filter_data').hide();
    $('#filter_dimension').hide();
    $('#filter_date').hide();

    if (_opt._filters.indexOf('filter_prefilter_use') >= 0) {
        $("input[data-wid='filter_prefilter_use']").prop('checked', true);
        $("input[name='filter_prefilter_use']").prop('checked', true);
        $('#filter_prefilter').show();
    }

    if (_opt._filters.indexOf('filter_search_use') >= 0) {
        $("input[data-wid='filter_search_use']").prop('checked', true);
        $("input[name='filter_search_use']").prop('checked', true);
        $('#filter_search').show();
    }
    if (_opt._filters.indexOf('filter_category_use') >= 0) {
        $("input[data-wid='filter_category_use']").prop('checked', true);
        $("input[name='filter_category_use']").prop('checked', true);
        $('#filter_category').show();
    }

    if (_opt._filters.indexOf('filter_date_use') >= 0) {
        $("input[data-wid='filter_date_use']").prop('checked', true);
        $("input[name='filter_date_use']").prop('checked', true);
        $('#filter_date').show();
    }
    if (_opt._filters.indexOf('filter_dimension_use') >= 0) {
        $("input[data-wid='filter_dimension_use']").prop('checked', true);
        $("input[name='filter_dimension_use']").prop('checked', true);
        $('#filter_dimension').show();
    }
    if (_opt._filters.indexOf('filter_authors_use') >= 0) {
        $("input[data-wid='filter_authors_use']").prop('checked', true);
        $("input[name='filter_authors_use']").prop('checked', true);
        $('#field_author').show();
    }
    if (_opt._filters.indexOf('filter_tags_use') >= 0) {
        $("input[data-wid='filter_tags_use']").prop('checked', true);
        $("input[name='filter_tags_use']").prop('checked', true);
        $('#filter_tags').show();
    }

    if (_opt._subcats == "on") {
        $("input[name='subcats']").prop('checked', true);
    }

    $("input[name=date_type][value=" + _opt._datetype + "]").prop('checked', true);
    $("input[name=tag_mode][value=" + _opt._tag_mode + "]").prop('checked', true);
    $("input[name=dimension_type][value=" + _opt._dimension_type + "]").prop('checked', true);
}

$("#applyFilter").on("click", function () {
    ccSave();
});

function ccSave() {
    var obj = $('input[name=tag_mode]').filter(':checked')[0];
    _opt._tag_mode = $(obj).val();
    _opt._subcats = $("input[name=subcats]").prop('checked') ? "on" : "";

    var filters = "";
    var objs = $('.filter-manager-controller').filter(':checked');
    for (var i = 0; i < objs.length; i++) {
        var o = $(objs[i]);
        filters += o.attr('data-wid') + ',';
    }
    _opt._filters = filters;
    storage.filteropt = JSON.stringify(_opt);
}

function advfilter() {
    $(".filter-manager-controller-container").toggle();
    var val = "1";
    if ($(".filter-manager-controller-container").is(":hidden")) {
        val = "0";
        $("#arrd1").html('<i2 class="icon-down-open"></i2>');
    }
    else {
        $("#arrd1").html('<i class="icon-down-open"></i>');
    }
}

function show_paint(id) {
    window.cefQuery({
        request: 'Prompt.q.paint2:' + id
    });
}

function show_loc(obj) {
    var fpath = $(obj).attr('fol');
    if (_album == "private") {
        fpath += ".krf";
    }
    window.cefQuery({
        request: 'Prompt.q.viewfile:' + fpath

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
                $('#cboxClose').show();
                _vr_mask = 0;
            }
        });
    }, 0);
}

function jshow_close() {
    $.colorbox.close();
}

function reload_img(id) {
    var obj = $('#' + id);
    var d = new Date();
    var n = d.getTime();

    var src0 = $(obj).attr('data-original') + "_coi.png?d=" + n;
    $(obj).attr({ src: src0 });
}

function mygoback() {
    if (_vr_mask == 1) {
        return;
    }

    history.back();
}

function mygoforward() {
    if (_vr_mask == 1) {
        return;
    }
    history.forward();
}

function myreload() {
    if (_vr_mask == 1) {
        return;
    }
    location.reload();
}

function get_element_ids() {
    var element_ids = "";
    if ($("input[name=setSelected]").is(':checked')) {
        element_ids = _element_ids; // $('#element_ids').val();
    }
    else {
        var objs = $(".thumbnails input[type=checkbox]").filter(':checked');
        for (var i = 0; i < objs.length; i++) {
            var o = objs[i];
            element_ids += o.value + ",";
        }

        if (element_ids != "") {
            element_ids = element_ids.substr(0, element_ids.length - 1);
        }
    }
    return element_ids;
}

function checkPermitAction() {
    if ($('[name="selectAction"]').val() == 'duplicates') {
    }

    var nbSelected = 0;
    if ($("input[name=setSelected]").is(':checked')) {
        nbSelected = nb_thumbs_set;
    }
    else {
        nbSelected = $(".thumbnails input[type=checkbox]").filter(':checked').length;
    }

    if (nbSelected == 0) {
        $("#permitAction").hide();
        $("#forbidAction").show();
    }
    else {
        $("#permitAction").show();
        $("#forbidAction").hide();
    }

    $("#applyOnDetails").text(
        sprintf(
            applyOnDetails_pattern,
            nbSelected
        )
    );

    if (nbSelected == 0) {
        $("#selectedMessage").text(
            sprintf(
                selectedMessage_none,
                nb_thumbs_set
            )
        );
    }
    else if (nbSelected == nb_thumbs_set) {
        $("#selectedMessage").text(
            sprintf(
                selectedMessage_all,
                nb_thumbs_set
            )
        );
    }
    else {
        $("#selectedMessage").text(
            sprintf(
                selectedMessage_pattern,
                nbSelected,
                nb_thumbs_set
            )
        );
    }
}

function selectPageThumbnails() {
    $(".thumbnails label").each(function () {
        var checkbox = $(this).children("input[type=checkbox]");

        $(checkbox).prop('checked', true).trigger("change");
        $(this).closest("li").addClass("thumbSelected");
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

$(document).ready(function () {
    var tagsCache = new TagsCache();
    tagsCache.selectize($('[data-selectize=tags]'), {
        lang: {
            "Add": _addTitle
        }
    });
    tagsCache.selectize($('[data-selectize=tags0]'), {
        filter: function (categories, options) {
            var filtered = $.grep(categories, function (cat) {
                return cat.nb_images != "0";
            });
            return filtered;
        }
    });
    var Cache = new CategoriesCache('./ws.php?format=json&method=categories.getAdminList&type=2');
    Cache.selectize($('[data-selectize=categories1]'), {});

    var categoriesCache = new CategoriesCache('./ws.php?format=json&method=categories.getAdminList&type=1');
    categoriesCache.selectize($('[data-selectize=categories]'), {});

    //$('#addFilter').selectize();
    //$('#selectAction').selectize();

    $('#search_help').tipTip({
        'delay': 0,
        'fadeIn': 200,
        'fadeOut': 200
    });

    setTimeout(function () {
        $.get("./admin.html?page=admin.batch_picture_data", function (data, state) {

            var info = JSON.parse(data);
            $('#picture_data').html(info.htm);
            _element_ids = info.ids;

            jQuery('.font-checkbox').fontCheckbox();

            $("[id^=action_]").hide();

            $("select[name=selectAction]").change(function () {
                $("[id^=action_]").hide();

                var action = $(this).prop("value");
                if (action == 'move') {
                    action = 'associate';
                }

                if (action == 'duplicates') {
                }

                $("#action_" + action).show();

                if ($(this).val() != -1) {
                    $("#applyActionBlock").show();
                }
                else {
                    $("#applyActionBlock").hide();
                }
            });

            $("#action_add_tags select[name=add_tags]").change(function () {
                $("#action_add_tags span.errors").hide();
            });

            $("#action_del_tags select[name=del_tags]").change(function () {
                $("#action_del_tags span.errors").hide();
            });

            $(".wrap1 label").click(function (event) {
                $("input[name=setSelected]").prop('checked', false);

                var li = $(this).closest("li");
                var checkbox = $(this).children("input[type=checkbox]");

                checkbox.triggerHandler("shclick", event);

                if ($(checkbox).is(':checked')) {
                    $(li).addClass("thumbSelected");
                }
                else {
                    $(li).removeClass('thumbSelected');
                }

                checkPermitAction();
            });

            $("#selectAll").click(function () {
                $("input[name=setSelected]").prop('checked', false);
                selectPageThumbnails();
                checkPermitAction();
                return false;
            });

            $("#selectNone").click(function () {
                $("input[name=setSelected]").prop('checked', false);

                $(".thumbnails label").each(function () {
                    var checkbox = $(this).children("input[type=checkbox]");

                    if ($(checkbox).is(':checked')) {
                        $(checkbox).prop('checked', false).trigger("change");
                    }

                    $(this).closest("li").removeClass("thumbSelected");
                });
                checkPermitAction();
                return false;
            });

            $("#selectInvert").click(function () {
                $("input[name=setSelected]").prop('checked', false);

                $(".thumbnails label").each(function () {
                    var checkbox = $(this).children("input[type=checkbox]");

                    $(checkbox).prop('checked', !$(checkbox).is(':checked')).trigger("change");

                    if ($(checkbox).is(':checked')) {
                        $(this).closest("li").addClass("thumbSelected");
                    }
                    else {
                        $(this).closest("li").removeClass('thumbSelected');
                    }
                });
                checkPermitAction();
                return false;
            });

            $("#selectSet").click(function () {
                selectPageThumbnails();
                $("input[name=setSelected]").prop('checked', true);
                checkPermitAction();
                return false;
            });

            checkPermitAction();

            $("select[name=filter_prefilter]").change(function () {
                $("#empty_caddie").toggle($(this).val() == "caddie" && _nbCaddie > 0);
                $("#delete_orphans").toggle($(this).val() == "no_album" && _nbOrphan > 0);
                $("#duplicates").toggle($(this).val() == "duplicates" && _nbDuplicates > 0);

                //$("#applyFilter").click();
            });

            $('ul.thumbnails').enableShiftClick();
            $(".removeFilter").addClass("icon-cancel-circled");

            $(".removeFilter").click(function () {
                var filter = $(this).parent('li').attr("id");
                filter_disable(filter);
                return false;
            });

            $("#addFilter").change(function () {
                var filter = $(this).prop("value");
                filter_enable(filter);
                $(this).prop("value", -1);
            });

            $("#removeFilters").click(function () {
                $("#filterList li").each(function () {
                    var filter = $(this).attr("id");
                    filter_disable(filter);
                });
                return false;
            });

            $("input[name='search_allwords']").on("input", function () {
            });

            $("input[name=remove_author]").click(function () {
                if ($(this).is(':checked')) {
                    $("input[name=author]").hide();
                }
                else {
                    $("input[name=author]").show();
                }
            });

            $("input[name=remove_title]").click(function () {
                if ($(this).is(':checked')) {
                    $("input[name=title]").hide();
                }
                else {
                    $("input[name=title]").show();
                }
            });

            $('#applyAction').click(function (e) {
                if (_readonly != "0") {
                    $("#err").html(_err_readonly);
                    $('#err').delay(100).show();
                    $('#err').delay(2000).fadeOut();
                    return false;
                }

                var action = $('[name="selectAction"]').val();

                if (action == 'add_tags') {
                    if ($("#action_add_tags select[name=add_tags]").val() == null) {
                        $("#action_add_tags").attr("style", "display:block;");
                        $(".selectize-control").attr("style", "display:inline-block;width:400px;");
                        $("#action_add_tags span.errors").attr("style", "vertical-align:top;height:20px;min-height: 20px;margin-top: 0px;");
                        $("#action_add_tags span.errors").show();
                        return false;
                    }
                    e.stopPropagation();
                }
                if (action == 'del_tags') {
                    if ($("#action_del_tags select[name=del_tags]").val() == null) {

                        $("#action_del_tags").attr("style", "display:block;");
                        $(".selectize-control").attr("style", "display:inline-block;width:400px;");
                        $("#action_del_tags span.errors").attr("style", "vertical-align:top;height:20px;min-height: 20px;margin-top: 0px;");
                        $("#action_del_tags span.errors").show();
                        return false;
                    }
                    e.stopPropagation();
                }

                var element_ids = get_element_ids();
                if (!element_ids) {
                    return false;
                }

                if (action == 'delete') {
                    $.colorbox({
                        transition: "none", width: 650, height: 400, fixed: true, iframe: true, overlayClose: false, opacity: .6, closeButton: false,

                        //title: _tit20,
                        href: function () {
                            var nb = element_ids.split(",").length;
                            return "./delbox.html?type=9&nb=" + nb + "&ids=" + element_ids;
                        },
                        onComplete: function () {
                            $('#cboxOverlay').css({ 'background': 'slategray' });
                            $('#cboxWrapper').css({ 'border-top': 'solid 7px #e74c3c' });

                        },
                        onClosed: function () {
                            document.getElementById('cboxOverlay').style.removeProperty('background');
                            document.getElementById('cboxWrapper').style.removeProperty('border-top');
                        }
                    });
                    return false;
                }

                var body = "";
                var chk = "";
                var r;
                if (action == 'associate' || action == 'move') {
                    body += "&associate=" + encodeURIComponent($('[name="associate"]').val());
                }
                else if (action == 'duplicates') {
                    body += "&duplicates=" + encodeURIComponent($('[name="duplicates"]').val());
                }
                else if (action == 'add_tags') {
                    body += "&add_tags=" + encodeURIComponent($('[name="add_tags"]').val());
                }
                else if (action == 'del_tags') {
                    body += "&del_tags=" + encodeURIComponent($('[name="del_tags"]').val());
                }
                else if (action == 'author') {
                    body += "&author=" + encodeURIComponent($("input[name=author]").val());
                    r = $("input[name=remove_author]").is(':checked');
                    if (r) {
                        chk = "on";
                    }
                    body += "&remove_author=" + chk;
                }
                else if (action == 'title') {
                    body += "&title=" + encodeURIComponent($("input[name=title]").val());
                    r = $("input[name=remove_title]").is(':checked');
                    if (r) {
                        chk = "on";
                    }
                    body += "&remove_title=" + chk;
                }

                $.post("./ws.php?method=batch." + action, "photos=" + element_ids + body, function (data, state) {
                    if (data == "ok") {
                        location.reload();
                    }
                });

                return false;
            });

            $('#empty_caddie').click(function (e) {
                $("#deteleLoading0").show();
                $.get("./ws.php?method=empty.caddie", function (data, state) {
                    $("#deteleLoading0").hide();
                    var info = $.parseJSON(data);
                    if (info.stat == "ok") {
                        location.reload();
                    }
                });
                return false;
            });

            $('#duplicates').click(function (e) {
                $("#deteleLoading0").show();

                $.post("./ws.php?method=batch.duplicates", "photos=", function (data, state) {
                    $("#deteleLoading0").hide();
                    var info = $.parseJSON(data);
                    if (info.stat == "ok") {
                        location.reload();
                    }
                });

                return false;
            });


            $('#delete_orphans').click(function (e) {
                $("#deteleLoading0").show();
                $.get("./ws.php?method=delete.orphans", function (data, state) {
                    $("#deteleLoading0").hide();
                    var info = $.parseJSON(data);
                    if (info.stat == "ok") {
                        location.reload();
                    }
                });
                return false;
            });

            $("#action_delete input[name=confirm_deletion]").change(function () {
                $("#action_delete span.errors").hide();
            });

            $('.thumbnails .thumb0').tipTip({
                'delay': 0,
                'fadeIn': 200,
                'fadeOut': 200,
                'keepAlive': true,
                'attribute': 'mtitle'
            });
        });
    }, 100);

});

$(document).ready(function () {
    if (_ov == "1") {
        $('#content-wrapper').watermark({
            texts: [_expried],
            textColor: "#eee"
        });
    }
    jQuery('.font-checkbox').fontCheckbox();

    if (_readonly != "0") {
        $('[data-selectize=tags]').removeAttr('data-create');
    }

    $("select[name=selectAction]").selectize();
    $("#prefilter").selectize();

    $(document).on("mouseup", function (e) {
        var con = $(e.target);
        if (!con.is('.filter-manager-controller-container *') && !con.is('#choosef *')) {
            $(".filter-manager-controller-container").hide();
        }
    });
})

$(document).ready(function () {
    if (!storage.filteropt) {
        storage.filteropt = JSON.stringify(_opt);
    }
    else {
        _opt = JSON.parse(storage.filteropt);
        if (_opt._filters == "") {
            _opt._filters = "filter_prefilter_use";
        }
    }
    field_show();

    $.getJSON('./ws.php?method=batch.slider', function (data) {
        $("#filterLoading").hide();
        if (data.stat == "ok") {
            $("#s_dimension").html(data.result.slider_dimension);
            $("#s_date").html(data.result.slider_postdate + data.result.slider_createdate);
            var sliders = data.result.slider_josn;

            $('[data-slider=date_creation]').dblSlider(sliders.createdates);
            $('[data-slider=date_available]').dblSlider(sliders.postdates);

            $('[data-slider=widths]').dblSlider(sliders.widths);
            $('[data-slider=heights]').dblSlider(sliders.heights);
            $('[data-slider=ratios]').dblSlider(sliders.ratios);
            $('[data-slider=filesizes]').dblSlider(sliders.filesizes);

            $("div[data-slider=" + _opt._dimension_type + "]").show();
            $("div[data-slider=" + _opt._datetype + "]").show();
        }
    });

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
            filters += o.attr('data-wid') + ',';
        }
        _opt._filters = filters;
        field_show();
    });
    $("#content-wrapper").scroll(function () {
        $("#tiptip_holder").hide();
    });
})