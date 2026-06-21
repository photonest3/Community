#include "MimeType.h"


CMimeType::CMimeType(void)
{
	_pMimeTypes = new MIMETYPES();
}

CMimeType::~CMimeType(void)
{
	if (_pMimeTypes != NULL)
	{
		_pMimeTypes->clear();
		delete _pMimeTypes;
		_pMimeTypes = NULL;
	}
}

void CMimeType::InitializeMime()
{
	_pMimeTypes->insert( MIMETYPES::value_type(L"xml","text/xml"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"323","text/h323"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"acx","application/internet-property-stream"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"ai","application/postscript"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"aif","audio/x-aiff"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"aifc","audio/x-aiff"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"aiff","audio/x-aiff"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"asf","video/x-ms-asf"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"asr","video/x-ms-asf"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"asx","video/x-ms-asf"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"au","audio/basic"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"avi","video/x-msvideo"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"axs","application/olescript"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"bas","text/plain"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"bcpio","application/x-bcpio"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"bin","application/octet-stream"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"bmp","image/bmp"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"c","text/plain"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"cat","application/vnd.ms-pkiseccat"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"cdf","application/x-cdf"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"cer","application/x-x509-ca-cert"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"class","application/octet-stream"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"clp","application/x-msclip"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"cmx","image/x-cmx"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"cod","image/cis-cod"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"cpio","application/x-cpio"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"crd","application/x-mscardfile"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"crl","application/pkix-crl"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"crt","application/x-x509-ca-cert"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"csh","application/x-csh"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"css","text/css"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"dcr","application/x-director"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"der","application/x-x509-ca-cert"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"dir","application/x-director"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"dll","application/x-msdownload"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"dms","application/octet-stream"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"doc","application/msword"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"dot","application/msword"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"dvi","application/x-dvi"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"dxr","application/x-director"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"eps","application/postscript"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"etx","text/x-setext"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"evy","application/envoy"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"exe","application/octet-stream"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"fif","application/fractals"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"flr","x-world/x-vrml"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"gif","image/gif"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"gtar","application/x-gtar"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"gz","application/x-gzip"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"h","text/plain"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"hdf","application/x-hdf"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"hlp","application/winhlp"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"hqx","application/mac-binhex40"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"hta","application/hta"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"htc","text/x-component"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"htm","text/html"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"html","text/html"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"htt","text/webviewhtml"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"ico","image/x-icon"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"ief","image/ief"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"iii","application/x-iphone"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"ins","application/x-internet-signup"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"isp","application/x-internet-signup"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"jfif","image/pipeg"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"jpe","image/jpeg"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"jpeg","image/jpeg"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"jpg","image/jpeg"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"png","image/png"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"js","application/x-javascript"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"latex","application/x-latex"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"lha","application/octet-stream"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"lsf","video/x-la-asf"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"lsx","video/x-la-asf"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"lzh","application/octet-stream"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"m13","application/x-msmediaview"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"m14","application/x-msmediaview"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"m3u","audio/x-mpegurl"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"man","application/x-troff-man"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"mdb","application/x-msaccess"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"me","application/x-troff-me"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"mht","message/rfc822"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"mhtml","message/rfc822"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"mid","audio/mid"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"mny","application/x-msmoney"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"mov","video/quicktime"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"movie","video/x-sgi-movie"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"mp2","video/mpeg"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"mp3","audio/mpeg"));
	//_pMimeTypes->insert( MIMETYPES::value_type(L"mp3","audio/mp3"));
	_pMimeTypes->insert(MIMETYPES::value_type(L"mp4", "video/mp4"));
	_pMimeTypes->insert(MIMETYPES::value_type(L"wmv", "video/x-ms-wmv"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"mpa","video/mpeg"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"mpe","video/mpeg"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"mpeg","video/mpeg"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"mpg","video/mpeg"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"mpp","application/vnd.ms-project"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"mpv2","video/mpeg"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"ms","application/x-troff-ms"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"mvb","application/x-msmediaview"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"nws","message/rfc822"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"oda","application/oda"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"p10","application/pkcs10"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"p12","application/x-pkcs12"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"p7b","application/x-pkcs7-certificates"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"p7c","application/x-pkcs7-mime"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"p7m","application/x-pkcs7-mime"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"p7r","application/x-pkcs7-certreqresp"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"p7s","application/x-pkcs7-signature"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"pbm","image/x-portable-bitmap"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"pdf","application/pdf"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"pfx","application/x-pkcs12"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"pgm","image/x-portable-graymap"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"pko","application/ynd.ms-pkipko"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"pma","application/x-perfmon"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"pmc","application/x-perfmon"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"pml","application/x-perfmon"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"pmr","application/x-perfmon"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"pmw","application/x-perfmon"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"pnm","image/x-portable-anymap"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"pot","application/vnd.ms-powerpoint"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"ppm","image/x-portable-pixmap"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"pps","application/vnd.ms-powerpoint"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"ppt","application/vnd.ms-powerpoint"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"prf","application/pics-rules"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"ps","application/postscript"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"pub","application/x-mspublisher"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"qt","video/quicktime"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"ra","audio/x-pn-realaudio"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"ram","audio/x-pn-realaudio"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"ras","image/x-cmu-raster"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"rgb","image/x-rgb"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"rmi","audio/mid"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"roff","application/x-troff"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"rtf","application/rtf"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"rtx","text/richtext"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"scd","application/x-msschedule"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"sct","text/scriptlet"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"setpay","application/set-payment-initiation"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"setreg","application/set-registration-initiation"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"sh","application/x-sh"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"shar","application/x-shar"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"sit","application/x-stuffit"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"snd","audio/basic"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"spc","application/x-pkcs7-certificates"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"spl","application/futuresplash"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"src","application/x-wais-source"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"sst","application/vnd.ms-pkicertstore"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"stl","application/vnd.ms-pkistl"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"stm","text/html"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"sv4cpio","application/x-sv4cpio"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"sv4crc","application/x-sv4crc"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"t","application/x-troff"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"tar","application/x-tar"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"tcl","application/x-tcl"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"tex","application/x-tex"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"texi","application/x-texinfo"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"texinfo","application/x-texinfo"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"tgz","application/x-compressed"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"tif","image/tiff"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"tiff","image/tiff"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"tr","application/x-troff"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"trm","application/x-msterminal"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"tsv","text/tab-separated-values"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"txt","text/plain"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"uls","text/iuls"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"ustar","application/x-ustar"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"vcf","text/x-vcard"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"vrml","x-world/x-vrml"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"wav","audio/x-wav"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"wcm","application/vnd.ms-works"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"wdb","application/vnd.ms-works"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"wks","application/vnd.ms-works"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"wmf","application/x-msmetafile"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"wps","application/vnd.ms-works"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"wri","application/x-mswrite"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"wrl","x-world/x-vrml"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"wrz","x-world/x-vrml"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"xaf","x-world/x-vrml"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"xbm","image/x-xbitmap"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"xla","application/vnd.ms-excel"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"xlc","application/vnd.ms-excel"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"xlm","application/vnd.ms-excel"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"xls","application/vnd.ms-excel"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"xlt","application/vnd.ms-excel"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"xlw","application/vnd.ms-excel"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"xof","x-world/x-vrml"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"xpm","image/x-xpixmap"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"xwd","image/x-xwindowdump"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"z","application/x-compress"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"zip","application/zip"));

	_pMimeTypes->insert( MIMETYPES::value_type(L"eps","application/postscript"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"arj","application/x-arj"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"lnk","application/x-ms-shortcut"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"cert","application/x-x509-ca-cert"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"cab","application/x-compressed"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"vox","audio/voxware"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"cpp","text/plain"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"def","text/plain"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"java","text/x-java-source"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"shtml","wwwserver/html-ssi"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"asa","wwwserver/isapi"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"asp","wwwserver/isapi"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"cfm","wwwserver/isapi"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"dbm","wwwserver/isapi"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"isa","wwwserver/isapi"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"plx","wwwserver/isapi"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"url","wwwserver/isapi"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"cgi","wwwserver/isapi"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"php","wwwserver/isapi"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"wcgi","wwwserver/isapi"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"csv","text/comma-separated-values"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"swf","application/x-shockwave-flash"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"svg","image/svg+xml"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"woff","application/font-woff"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"woff2","application/x-font-woff"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"eot","application/vnd.ms-fontobject"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"otf","application/octet-stream"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"ttf","application/octet-stream"));
	_pMimeTypes->insert( MIMETYPES::value_type(L"webm", "video/webm"));

}

bool CMimeType::GetMime(wstring ext, string &type)
{
	bool b = false;
	MIMETYPES::iterator it;
	it = _pMimeTypes->find(ext);
	if (it != _pMimeTypes->end())
	{
		type = (*it).second;
		b = true;
	}

	return b;
}
