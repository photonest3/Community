
///////////////////////////////////////////////////////////////////////
/// @file Htmlexif.cpp
/// @brief EXIF 元数据提取处理实现文件
/// @details 实现 JPEG 图片 EXIF 信息的读取、解析、格式化等处理功能
/// @author PhotoNest Team
/// @date 2024
/// @version 1.0
/// @note 使用 easyexif 库进行 EXIF 数据解析
///////////////////////////////////////////////////////////////////////

﻿#include "StdAfx.h"
#include "../../templ/templ/resource.h"
#include "Htmlexif.h"
#include "unitil.h"
#include "Datasrv.h"
#include "uploadParser.h"


#pragma intrinsic (_InterlockedIncrement)
volatile LONG _gdata = 1;

CHtmlexif::CHtmlexif(void)
{
}

CHtmlexif::~CHtmlexif(void)
{
}

long CHtmlexif::get_exif(wstring szSrc, EXIFInfo &result)
{
	//const char *photoPath = "E:/1.png";
	// Read the JPEG file into a buffer
	FILE *fp = _wfsopen(szSrc.c_str(), L"rb", _SH_DENYNO);
	if (!fp) {
		printf("Can't open file.\n");
		return -1;
	}
	fseek(fp, 0, SEEK_END);
	unsigned long fsize = ftell(fp);
	rewind(fp);
	unsigned char *buf = new unsigned char[fsize];
	if (fread(buf, 1, fsize, fp) != fsize) {
		printf("Can't read file.\n");
		delete[] buf;
		return -2;
	}
	fclose(fp);

	// Parse EXIF
	//easyexif::EXIFInfo result;
	int code = result.parseFrom(buf, fsize);
	delete[] buf;
	if (code) {
		printf("Error parsing EXIF: code %d\n", code);
		return -3;
	}

	return 0;
}

long CHtmlexif::get_exif_value(wstring filename, string type, map<string, string> &attr)
{
	wstring dst = filename;

	if (CDataSrv::Instance()._albumName == "private")
	{
		long nd = _InterlockedIncrement(&_gdata);

		wchar_t uid[MAX_PATH] = { 0 };
		swprintf_s(uid, MAX_PATH, L"%s%d%s", L"/data2/_temp/exif_", nd, L".png");


		wstring szPath = CDataSrv::Instance()._appDataDir;//get_module_path(NULL);
		dst = szPath + uid;

		CUploadParser::Instance().Decryptfile(filename + L".krf", dst);
	}

	EXIFInfo result;
	int ret = get_exif(dst, result);
	if (ret == 0)
	{
		if (type == "")
		{
			attr.insert(map<string, string>::value_type("DateTimeOriginal", result.DateTimeOriginal.c_str()));
			attr.insert(map<string, string>::value_type("Make", result.Make.c_str()));

		}
		else if (type == "1")
		{
			attr.insert(map<string, string>::value_type("rotation", uint2str(result.Orientation)));
			attr.insert(map<string, string>::value_type("width", uint2str(result.ImageWidth)));
			attr.insert(map<string, string>::value_type("height", uint2str(result.ImageHeight)));

		}
		else if (type == "all")
		{
			attr.insert(map<string, string>::value_type("Make", result.Make.c_str()));
			attr.insert(map<string, string>::value_type("Model", result.Model.c_str()));
			attr.insert(map<string, string>::value_type("DateTimeOriginal", result.DateTimeOriginal.c_str()));
			attr.insert(map<string, string>::value_type("DateTimeDigitized", result.DateTimeDigitized.c_str()));
			//attr.insert(map<string, string>::value_type("DateTime", result.DateTime));

			if (result.DateTime != "")
			{
				attr.insert(map<string, string>::value_type("DateTime", result.DateTime.c_str()));
			}

			string Orientation = "";
			switch (result.Orientation)
			{
			case 1:
				Orientation = "upper left of image";
				break;
			case 2:
				Orientation = "lower right of image";
				break;
			case 3:
				Orientation = "lower right of image";
				break;
			case 6:
				Orientation = "upper right of image";
				break;
			case 7:
				Orientation = "upper left of image";
				break;
			case 8:
				Orientation = "lower left of image";
				break;
			default:
				break;
			}
			attr.insert(map<string, string>::value_type("Orientation", Orientation));

			string Flash = "";
			if (result.Flash == 0)
			{
				Flash = "no flash";
			}
			else if (result.Flash == 0)
			{
				Flash = "flash used";
			}
			if (Flash != "")
			{
				attr.insert(map<string, string>::value_type("Flash", Flash));
			}
			attr.insert(map<string, string>::value_type("FocalLength", float2Str(static_cast<float>(result.FocalLength), "%.2fmm")));
			attr.insert(map<string, string>::value_type("FocalLengthIn35mm", uint2str(result.FocalLengthIn35mm) + "mm"));
			attr.insert(map<string, string>::value_type("ExposureTime", float2Str(static_cast<float>(result.ExposureTime), "%.2fs")));

			string ExposureProgram = "";
			switch (result.ExposureProgram)
			{
			case 0:
				ExposureProgram = "Not defined";
				break;
			case 1:
				ExposureProgram = "Manual";
				break;
			case 2:
				ExposureProgram = "Normal program";
				break;
			case 3:
				ExposureProgram = "Aperture priority";
				break;
			case 4:
				ExposureProgram = "Shutter priority";
				break;
			case 5:
				ExposureProgram = "Creative program";
				break;
			case 6:
				ExposureProgram = "Action program";
				break;
			case 7:
				ExposureProgram = "Portrait mode";
				break;
			case 8:
				ExposureProgram = "Landscape mode";
				break;
			default:
				break;
			}

			attr.insert(map<string, string>::value_type("ExposureProgram", ExposureProgram.c_str()));
			attr.insert(map<string, string>::value_type("FNumber", float2Str(static_cast<float>(result.FNumber), "f/%.2f")));
			attr.insert(map<string, string>::value_type("ISOSpeedRatings", uint2str(result.ISOSpeedRatings)));
			//attr.insert(map<string, string>::value_type("LightSource", result.li));

			string MeteringMode = "";
			switch (result.MeteringMode)
			{
			case 1:
				MeteringMode = "average";
				break;
			case 2:
				MeteringMode = "center weighted average";
				break;
			case 3:
				MeteringMode = "spot";
				break;
			case 4:
				MeteringMode = "multi-spot";
				break;
			case 5:
				MeteringMode = "multi-segment";
				break;
			default:
				break;
			}

			attr.insert(map<string, string>::value_type("MeteringMode", MeteringMode));
			//attr.insert(map<string, string>::value_type("ColorSpace", result.));

			//attr.insert(map<string, string>::value_type("Quality", result.q));
			attr.insert(map<string, string>::value_type("GPSLatitude", float2Str(static_cast<float>(result.GeoLocation.Latitude), "%.1f")));
			attr.insert(map<string, string>::value_type("GPSLongitude", float2Str(static_cast<float>(result.GeoLocation.Longitude), "%.1f")));
			attr.insert(map<string, string>::value_type("GPSAltitudeRef", Int2Str((int)result.GeoLocation.Altitude)));
			//attr.insert(map<string, string>::value_type("XResolution", result.x));
			//attr.insert(map<string, string>::value_type("YResolution", result.Make));
			//attr.insert(map<string, string>::value_type("ResolutionUnit", result.Make));

			attr.insert(map<string, string>::value_type("Software", result.Software.c_str()));
		}
	}

	if (CDataSrv::Instance()._albumName == "private")
	{
		my_DeleteFile(dst.c_str());
	}

	return 0;
}
