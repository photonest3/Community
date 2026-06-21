
import { kv } from '@vercel/kv';
 
async function init() {
	alert("1");
  try {
    await kv.set("idx", "1");
    await kv.set("key-0", "aaaa");
    await kv.set("key-1", "bbbbbbbb");
    await kv.set("key-2", "ccccc");
    await kv.set("key-3", "ddddddd");
    await kv.set("key-4", "eeeee");
    await kv.set("key-5", "ffffffff");
    await kv.set("key-6", "gggggggggg");
    await kv.set("key-7", "hhhhhhhh");
    await kv.set("key-8", "iiiiiiiii");
    await kv.set("key-9", "jjjjjjj");
  } catch (error) {
	  alert(error);
  }
}

async function getlic(email) {
  try {
    var nb0 = await kv.get("idx");
    console.log(nb0);
    var k = "key-" + nb0;
	
	var nb = parseInt(nb0);
    if (nb >= 10) {
        var nb3 = nb - (nb / 10) * 10;
        k = "key-" + nb3;
    }
    var lic = await kv.get(k);

    k = "mail-" + nb0;
    await kv.set(k, lic + ":" + email);

    await kv.set("idx", nb + 1);

    return lic;	
	
	
  } catch (error) {
  }
}

 /*
async function exampleCommands() {
  try {
    await kv.hset('userSession', { userId: 123, email: 'ex@example.com' });
  } catch (error) {
    // Handle errors
  }
}
*/
function transl() {
    document.getElementById("nav_0").innerHTML = opt.nav_0;
    document.getElementById("nav_1").innerHTML = opt.nav_1;
    document.getElementById("nav_2").innerHTML = opt.nav_2;

    document.getElementById("banner_1").innerHTML = opt.banner_1;
    document.getElementById("banner_2").innerHTML = opt.banner_2;

    document.getElementById("special_1").innerHTML = opt.special_1;
    document.getElementById("detail_1").innerHTML = opt.detail_1;
    document.getElementById("special_2").innerHTML = opt.special_2;
    document.getElementById("detail_2").innerHTML = opt.detail_2;
    document.getElementById("special_3").innerHTML = opt.special_3;
    document.getElementById("detail_3").innerHTML = opt.detail_3;
    document.getElementById("special_4").innerHTML = opt.special_4;
    document.getElementById("detail_4").innerHTML = opt.detail_4;

    document.getElementById("download_0").innerHTML = opt.download_0;
    document.getElementById("download_1").innerHTML = opt.download_1;
    document.getElementById("download_s1").innerHTML = opt.download_s1;
    document.getElementById("download_2").innerHTML = opt.download_2;
    document.getElementById("download_s2").innerHTML = opt.download_s2;

    document.getElementById("like").innerHTML = opt.like;
    document.getElementById("like_1").innerHTML = opt.like_1;
    document.getElementById("like_2").innerHTML = opt.like_2;
    document.getElementById("like_3").innerHTML = opt.like_3;

    document.getElementById("yearly").innerHTML = opt.yearly;
    document.getElementById("buy").innerHTML = opt.buy;
    document.getElementById("buy_1").innerHTML = opt.buy_1;
    document.getElementById("buy_2").innerHTML = opt.buy_2;
    document.getElementById("buy_3").innerHTML = opt.buy_3;

    document.getElementById("git").innerHTML = opt.git;

};

var _init = 0;
function _change(lang) {
    var c = document.getElementById("i18n");
    if (c) {
        c.remove();
    }

    var o = document.createElement("script");
    o.src = "assets/js/nest-" + lang + ".js";
    o.id = "i18n";
    o.onload = function () {
        transl();

        var nav = document.querySelector('.dropdown-menu');
        nav.style.display = 'none';

        nav = document.querySelector('.navbar-collapse');
        nav.style.display = 'none';

        if (_init == 0) {
            _init = 1;
            new WOW().init();
            tns({
                container: '.testimonial-active',
                autoplay: true,
                autoplayTimeout: 5000, autoplayButtonOutput: false, mouseDrag: true, gutter: 0, nav: false,
                navPosition: "bottom", controls: true,
                controlsText: ['<i class="lni lni-chevron-left"></i>', '<i class="lni lni-chevron-right"></i>',], items: 1,
            });
        }
    };
    document.body.appendChild(o);
}

function shownav() {
    var nav = document.querySelector('.navbar-collapse');
    if (nav.style.display == 'block') {
        nav.style.display = 'none';
    }
    else {
        nav.style.display = 'block';
    }

    nav = document.querySelector('.dropdown-menu');
    nav.style.display = 'none';
}

function showmwnu() {
    var nav = document.querySelector('.dropdown-menu');
    if (nav.style.display == 'block') {
        nav.style.display = 'none';
    }
    else {
        nav.style.display = 'block';
    }
}

window.onload = function () {
    var lang = "cn";
    if (location.href.indexOf('cn') >= 0) {
        lang = "cn";
    }
    _change(lang);

    document.onmouseup = function (e) {
        if (!e.target.classList.contains("dropdown-toggle")) {
            var nav = document.querySelector('.dropdown-menu');
            nav.style.display = 'none';
        }
    };
	
	init();
};
