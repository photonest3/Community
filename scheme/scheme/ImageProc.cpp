#include "StdAfx.h"
#include "resource.h"
#include "ImageProc.h"
#include "unitil.h"
#include "base64-decode.h"

#include <Shobjidl.h>
#include <shlobj.h>
#include <thumbcache.h>
#include "uploadParser.h"
#include "Datasrv.h"
#include "imlog.h"
#include <jpeglib.h>
#include <iostream>
#include <fstream>

#pragma intrinsic (_InterlockedIncrement)
volatile LONG gdata = 1;

class byte4
{
public:
	BYTE r; // 用于存放 red
	BYTE g; // 用于存放 green
	BYTE b; // 用于存放 blue
	BYTE a; // 用于存放 alpha
};

extern "C"
{
#include <transupp.h>
}

#ifdef _DEBUG
#pragma comment( lib, "../../../pictureofu/tests/cefclient/Debug/bin/FreeImaged.lib" ) 
#pragma comment( lib, "../../libs/debug/transupp.lib" ) 
#pragma comment( lib, "../../libs/Debug/turbojpeg-static.lib" ) 

#else
#pragma comment( lib, "../../../pictureofu/tests/cefclient/Release/bin/FreeImage.lib" ) 
#pragma comment( lib, "../../libs/release/transupp.lib" ) 
#pragma comment( lib, "../../libs/Release/turbojpeg-static.lib" ) 
#endif

struct my_error_mgr {
	struct jpeg_error_mgr pub;
	jmp_buf setjmp_buffer;
};
typedef struct my_error_mgr *my_error_ptr;

void my_error_exit(j_common_ptr cinfo)
{
	/* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
	my_error_ptr myerr = (my_error_ptr)cinfo->err;
	/* Always display the message. */
	/* We could postpone this until after returning, if we chose. */
	(*cinfo->err->output_message) (cinfo);
	/* Return control to the setjmp point */
	longjmp(myerr->setjmp_buffer, 1);
}

CImageProc::CImageProc(void)
{
	_seq_id = 0;
	_func = NULL;

	char buf[500];
	memset(buf, 500, 0);
	GetSystemDirectoryA(buf, 500);
	string s = buf;
	s += "\\mfplat.dll";

	struct _stat32 info;
	int result = _stat32(s.c_str(), &info);
	if (result == 0 && info.st_size > 0)
	{
		_mfthumb_dll = ::LoadLibrary(L"mfthumb.dll");
		if (_mfthumb_dll)
		{
			_func = (pfnGetStatus)GetProcAddress(_mfthumb_dll, "save_first_sample_as_jpg");
		}
	}
}

CImageProc::~CImageProc(void)
{
	if (_mfthumb_dll) {
		::FreeLibrary(_mfthumb_dll);
	}
}

BOOL CImageProc::jp_test(const wchar_t *lpszPathName, const wchar_t *lpszImagePathName, int scale_denom, long rotation)
{
	unsigned int outwidth = 0;
	unsigned int outheight = 0;

	wstring dst = lpszImagePathName;
	if (rotation != 1)
	{
		wchar_t drive[_MAX_DRIVE];
		wchar_t direc[_MAX_DIR];
		wchar_t fname[_MAX_FNAME];
		wchar_t ext[_MAX_EXT];
		_wsplitpath_s(lpszImagePathName, drive, direc, fname, ext);

		_seq_id++;
		wchar_t buf[MAX_PATH];
		memset(buf, 0, MAX_PATH * sizeof(wchar_t));
		swprintf_s(buf, MAX_PATH, L"%d", _seq_id);

		ReplaceOne(dst, fname, wstring(fname) + buf);

	}

	unsigned char *img_buffer = read_jpeg_file(lpszPathName, scale_denom, outwidth, outheight);
	if (NULL == img_buffer) {
		return FALSE;
	}

	if (0 != write_jpeg_file(dst.c_str(), img_buffer, 80, outwidth, outheight)) {
		free(img_buffer);
		return FALSE;
	}

	if (rotation != 1)
	{
		xxxx(dst.c_str(), lpszImagePathName, rotation);
		DeleteFile(dst.c_str());
	}

	return TRUE;
}

BOOL CImageProc::testSaveThumbnail(const wchar_t *lpszPathName, const wchar_t *lpszImagePathName, long rotation, float maxWidth, float maxHeight, int &ftype)
{
	BOOL bResult = FALSE;
	FIBITMAP *dib = NULL;
	FIBITMAP *finalPic = NULL;
	FIBITMAP *rescalePic = NULL;

	BOOL b = TRUE;
	long width = 0;
	long height = 0;

	try {
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		fif = FreeImage_GetFileTypeU(lpszPathName);
		if (fif == FIF_UNKNOWN)
			fif = FreeImage_GetFIFFromFilenameU(lpszPathName);

		ftype = fif;

		if ((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif))
		{
			int flags = FIF_LOAD_NOPIXELS;

			// load the dib
			dib = FreeImage_LoadU(fif, lpszPathName, 0);
			if (!dib)
			{
				return FALSE;
			}

			float width = FreeImage_GetWidth(dib);
			float height = FreeImage_GetHeight(dib);

			//			float maxWidth = 792;
			//			float maxHeight = 594;

			float outW = 0;
			float outH = 0;

			if (width < maxWidth && height < maxHeight) {
				outW = width;
				outH = height;
			}
			else
			{
				if (maxWidth / maxHeight <= width / height)
				{
					outW = maxWidth;
					outH = maxWidth* (height / width);
				}
				else
				{
					outW = maxHeight  * (width / height);
					outH = maxHeight;
				}
			}

			if (fif == FIF_GIF)
			{
				b = FreeImage_SaveU(FIF_GIF, dib, lpszImagePathName, JPEG_QUALITYSUPERB);
				FreeImage_Unload(dib);
				dib = NULL;
				return TRUE;

			}

			int bpp1 = FreeImage_GetBPP(dib);

			if (width < maxWidth && height < maxHeight && bpp1 == 32 && rotation == 1) {
				FreeImage_Unload(dib);
				dib = NULL;
				return FALSE;
			}

			rescalePic = FreeImage_Rescale(dib, outW, outH, FILTER_BOX);//rescale size: 165x130
			if (!rescalePic)
			{
				FreeImage_Unload(dib);
				dib = NULL;
				return FALSE;
			}

			if (bpp1 == 32)
			{
				RGBQUAD BGColor;
				//FreeImage_GetBackgroundColor(dib, &BGColor);
				BGColor.rgbRed = 0;
				BGColor.rgbGreen = 0;
				BGColor.rgbBlue = 0;

				finalPic = FreeImage_Allocate(outW, outH, 24);
				for (int i = outH - 1; i >= 0; i--)
				{
					for (int j = 0; j < outW; j++)
					{
						RGBQUAD src_color;
						FreeImage_GetPixelColor(rescalePic, j, i, &src_color);
						float a = ((float)src_color.rgbReserved) / 255.0;

						RGBQUAD dst_color;

						dst_color.rgbRed = ((1 - a) * BGColor.rgbRed) + (a * src_color.rgbRed);
						dst_color.rgbGreen = ((1 - a) * BGColor.rgbGreen) + (a * src_color.rgbGreen);
						dst_color.rgbBlue = ((1 - a) * BGColor.rgbBlue) + (a * src_color.rgbBlue);
						FreeImage_SetPixelColor(finalPic, j, i, &dst_color);
					}
				}
			}
			else
			{
				finalPic = FreeImage_ConvertTo24Bits(rescalePic); //this is important,if no this,the rescaled gif cannot flash
			}

			FreeImage_Unload(dib);
			FreeImage_Unload(rescalePic);

			if (!finalPic)
			{
				return FALSE;
			}


			if (rotation == 1)
			{
				b = FreeImage_SaveU(FIF_JPEG, finalPic, lpszImagePathName, JPEG_QUALITYSUPERB);
				FreeImage_Unload(finalPic);
			}
			else
			{
				float angle = 0;
				if (rotation == 6)
				{
					angle = -90;
				}
				else if (rotation == 8)
				{
					angle = 90;
				}
				else if (rotation == 3)
				{
					angle = 180;
				}

				FIBITMAP *finalPic2 = FreeImage_Rotate(finalPic, angle);// , 0, 0, outW / 2, outH / 2, TRUE);
				FreeImage_Unload(finalPic);
				if (!finalPic2)
				{
					return FALSE;
				}

				// char val = 1;
				//bool b1 = FreeImage_SetMetadataKeyValue(FIMD_EXIF_MAIN, finalPic2, "Orientation", &val);

				b = FreeImage_SaveU(FIF_JPEG, finalPic2, lpszImagePathName, JPEG_QUALITYSUPERB);// , JPEG_DEFAULT);
				FreeImage_Unload(finalPic2);

			}
		}
	}
	catch (int) {
	}

	return b;
}


long CImageProc::get_Orientation_value(wstring filename)
{
	int Orientation = 1;
	char* tagName = "Orientation";

	int flags = FIF_LOAD_NOPIXELS;
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileTypeU(filename.c_str());

	FIBITMAP *dib = FreeImage_LoadU(fif, filename.c_str(), flags);
	if (dib)
	{
		FITAG *tagUserComment = NULL;
		FreeImage_GetMetadata(FIMD_EXIF_MAIN, dib, tagName, &tagUserComment);
		if (tagUserComment != NULL)
		{
			char*_tagValue = (char*)FreeImage_GetTagValue(tagUserComment);
			Orientation = _tagValue[0];
		}

		FreeImage_Unload(dib);
		dib = NULL;
	}
	return Orientation;
}

long CImageProc::get_exif_value(wstring filename, string type, map<string, string> &attr)
{
	wstring dst = filename;

	if (CDataSrv::Instance()._albumName == "private")
	{
		long nd = _InterlockedIncrement(&gdata);

		wchar_t uid[MAX_PATH];
		memset(uid, 0, MAX_PATH * sizeof(wchar_t));
		swprintf_s(uid, MAX_PATH, L"%s%d%s", L"\\data\\private\\temp\\a", nd, L".jpg");


		wstring szPath = get_module_path(NULL);
		dst = szPath + uid;// L"\\data\\private\\temp\\a.jpg";

		CUploadParser::Instance().Decryptfile(filename + L".krf", dst);
	}


	int flags = FIF_LOAD_NOPIXELS;
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileTypeU(dst.c_str());

	FIBITMAP *dib = FreeImage_LoadU(fif, dst.c_str(), flags);
	if (dib)
	{
		if (type == "")
		{
			{
				char* tagName = "DateTimeOriginal";
				FITAG *tagUserComment = NULL;
				FreeImage_GetMetadata(FIMD_EXIF_EXIF, dib, tagName, &tagUserComment);
				if (tagUserComment != NULL)
				{
					//FREE_IMAGE_MDTYPE tagType = FreeImage_GetTagType(tag);
					char*tagValue = (char*)FreeImage_GetTagValue(tagUserComment);

					if (tagValue != NULL)
					{
						attr.insert(map<string, string>::value_type(tagName, tagValue));
					}
				}
			}
			{
				char* tagName = "Make";
				FITAG *tagUserComment = NULL;
				FreeImage_GetMetadata(FIMD_EXIF_MAIN, dib, tagName, &tagUserComment);
				if (tagUserComment != NULL)
				{
					char*tagValue = (char*)FreeImage_GetTagValue(tagUserComment);

					if (tagValue != NULL)
					{
						attr.insert(map<string, string>::value_type(tagName, tagValue));
					}
				}
			}
		}
		else if (type == "1")
		{
			int rotation = 1;
			int width = 0;
			int height = 0;

			char* tagName = "Orientation";
			FITAG *tagUserComment = NULL;
			FreeImage_GetMetadata(FIMD_EXIF_MAIN, dib, tagName, &tagUserComment);
			if (tagUserComment != NULL)
			{
				char*_tagValue = (char*)FreeImage_GetTagValue(tagUserComment);
				rotation = _tagValue[0];
				attr.insert(map<string, string>::value_type("rotation", Int2Str(rotation)));
			}
			if (rotation == 6 || rotation == 8)
			{
				height = FreeImage_GetWidth(dib);
				width = FreeImage_GetHeight(dib);
			}
			else
			{
				width = FreeImage_GetWidth(dib);
				height = FreeImage_GetHeight(dib);
			}
			if (width > 0)
			{
				attr.insert(map<string, string>::value_type("width", Int2Str(width)));
				attr.insert(map<string, string>::value_type("height", Int2Str(height)));
			}
		}
		else if (type == "all")
		{
			FITAG *tag = NULL;
			FIMETADATA *mdhandle = NULL;
			mdhandle = FreeImage_FindFirstMetadata(FIMD_EXIF_MAIN, dib, &tag);
			if (mdhandle)
			{
				do {
					char * name = (char*)FreeImage_GetTagKey(tag);

					FREE_IMAGE_MDTYPE tagType = FreeImage_GetTagType(tag);
					// 7 = FIDT_UNDEFINED, An 8-bit byte that may contain anything, depending on the definition of the field.
					char* tagValue = (char*)FreeImage_TagToString(FIMD_EXIF_MAIN, tag);

					if (tagValue != NULL)
					{
						attr.insert(map<string, string>::value_type(name, tagValue));
					}
				} while (FreeImage_FindNextMetadata(mdhandle, &tag));
			}

			mdhandle = FreeImage_FindFirstMetadata(FIMD_EXIF_EXIF, dib, &tag);
			if (mdhandle)
			{
				do {
					char * name = (char*)FreeImage_GetTagKey(tag);

					FREE_IMAGE_MDTYPE tagType = FreeImage_GetTagType(tag);
					// 7 = FIDT_UNDEFINED, An 8-bit byte that may contain anything, depending on the definition of the field.
					char* tagValue = (char*)FreeImage_TagToString(FIMD_EXIF_EXIF, tag);

					if (tagValue != NULL)
					{
						attr.insert(map<string, string>::value_type(name, tagValue));
					}
				} while (FreeImage_FindNextMetadata(mdhandle, &tag));
			}



		}

		FreeImage_Unload(dib);
		dib = NULL;
	}

	if (CDataSrv::Instance()._albumName == "private")
	{
		DeleteFile(dst.c_str());
	}

	return 0;
}

HRESULT CImageProc::save_mp4_thumbnail(UINT cx, wstring szFile, wstring dst)
{
	HRESULT hr;

	//if (SUCCEEDED(hr))
	{
		IShellItem *psi;
		hr = SHCreateItemFromParsingName(szFile.c_str(), NULL, IID_PPV_ARGS(&psi));
		if (SUCCEEDED(hr))
		{
			IThumbnailProvider *pThumbProvider;
			hr = psi->BindToHandler(NULL, BHID_ThumbnailHandler, IID_PPV_ARGS(&pThumbProvider));
			if (SUCCEEDED(hr))
			{
				HBITMAP hThumbnail = NULL;
				WTS_ALPHATYPE wtsAlpha = WTSAT_UNKNOWN;
				hr = pThumbProvider->GetThumbnail(cx, &hThumbnail, &wtsAlpha);
				//if (!SUCCEEDED(hr))
				//{
				//	Sleep(10);
				//	hr = GetShellThumbnailImage(szFile.c_str(), &hThumbnail);
				//}

				if (SUCCEEDED(hr))
				{
					hr = proc_bimap(hThumbnail, dst);
					if (hr != S_OK)
					{
						//Cimlog::Instance().LogToFile("proc_bimap error");
					}

					DeleteObject(hThumbnail);
				}
				else
				{
					//Cimlog::Instance().LogToFile("save_mp4_thumbnail GetThumbnail: " + Int2Str(hr) + _w2u(szFile));
				}

				pThumbProvider->Release();
			}
			else
			{
				//Cimlog::Instance().LogToFile("save_mp4_thumbnail BindToHandler: " + Int2Str(hr));
			}
			psi->Release();
		}
		else
		{
			//Cimlog::Instance().LogToFile("save_mp4_thumbnail SHCreateItemFromParsingName: " + Int2Str(hr));
		}
		//CoUninitialize();
	}
	//else
	//{
	//	Cimlog::Instance().LogToFile("save_mp4_thumbnail CoInitializeEx: " + Int2Str(hr));
	//}

	return hr;
}

HRESULT CImageProc::proc_bimap(HBITMAP hbmp, wstring dst)
{
	FIBITMAP *dib = NULL;
	if (hbmp)
	{
		BITMAP bm = { 0 };
		int ret = GetObject(hbmp, sizeof(BITMAP), (LPSTR)&bm);
		if (ret == 0 || ret > sizeof(BITMAP))
		{
			return S_FALSE;
		}
		dib = FreeImage_AllocateT(FIT_BITMAP, bm.bmWidth, bm.bmHeight, bm.bmBitsPixel);
		if (dib == nullptr)
		{
			return S_FALSE;
		}

		int nColors = FreeImage_GetColorsUsed(dib);
		HDC dc = GetDC(NULL);
		int Success = GetDIBits(dc, hbmp, 0, FreeImage_GetHeight(dib),
			FreeImage_GetBits(dib), FreeImage_GetInfo(dib), DIB_RGB_COLORS);
		ReleaseDC(NULL, dc);
		FreeImage_GetInfoHeader(dib)->biClrUsed = nColors;
		FreeImage_GetInfoHeader(dib)->biClrImportant = nColors;


		FIBITMAP *finalPic = FreeImage_ConvertTo24Bits(dib); //this is important,if no this,the rescaled gif cannot flash
		if (!finalPic)
		{
			FreeImage_Unload(dib);
			dib = NULL;
			return S_FALSE;
		}

		BOOL b = FreeImage_SaveU(FIF_JPEG, finalPic, dst.c_str(), JPEG_QUALITYSUPERB);
		FreeImage_Unload(finalPic);
		finalPic = NULL;

		return b ? S_OK : S_FALSE;
	}
	return S_FALSE;
}

unsigned char * CImageProc::read_jpeg_file(const wchar_t *filename, int scale_denom, unsigned int &outwidth, unsigned int &outheight)
{
	struct jpeg_decompress_struct cinfo;
	struct my_error_mgr jerr;

	FILE *infile = NULL;
	//JSAMPARRAY buffer;
	int row_stride;

	_tfopen_s(&infile, filename, L"rb");
	if (infile == NULL)
	{
		return NULL;
	}

	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;

	if (setjmp(jerr.setjmp_buffer))
	{
		jpeg_destroy_decompress(&cinfo);
		fclose(infile);
		return NULL;
	}

	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, infile);
	jpeg_read_header(&cinfo, TRUE);


	cinfo.scale_num = 1;
	cinfo.scale_denom = scale_denom;

	jpeg_start_decompress(&cinfo);
	row_stride = cinfo.output_height * cinfo.output_width * cinfo.output_components;

	outwidth = cinfo.output_width;
	outheight = cinfo.output_height;

	unsigned char *data = (unsigned char *)malloc(sizeof(unsigned char) * row_stride);

	JSAMPROW row_pointer[1];
	while (cinfo.output_scanline < cinfo.output_height) {
		row_pointer[0] = &data[(cinfo.output_height - cinfo.output_scanline - 1)*cinfo.output_width*cinfo.output_components];
		jpeg_read_scanlines(&cinfo, row_pointer, 1);
	}

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	fclose(infile);
	return data;
}

int CImageProc::write_jpeg_file(const wchar_t * filename, unsigned char* image_buffer, int quality, unsigned int image_width, unsigned int image_height)
{
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	FILE * outfile = 0;
	JSAMPROW row_pointer[1];
	int row_stride;
	cinfo.err = jpeg_std_error(&jerr);

	jpeg_create_compress(&cinfo);

	_tfopen_s(&outfile, filename, L"wb");
	if (outfile == NULL)
	{
		return 1;
	}

	jpeg_stdio_dest(&cinfo, outfile);

	cinfo.image_width = image_width;
	cinfo.image_height = image_height;
	cinfo.input_components = 3;
	cinfo.in_color_space = JCS_RGB;
	jpeg_set_defaults(&cinfo);

	jpeg_set_quality(&cinfo, quality, TRUE);

	jpeg_start_compress(&cinfo, TRUE);

	row_stride = image_width * 3;
	while (cinfo.next_scanline < cinfo.image_height) {
		row_pointer[0] = &image_buffer[(cinfo.image_height - cinfo.next_scanline - 1) * row_stride];
		jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}


	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);

	fclose(outfile);

	if (NULL != image_buffer) {
		free(image_buffer);
		image_buffer = NULL;
	}

	return 0;
}

int CImageProc::xxxx(const wchar_t *lpszPathName, const wchar_t *lpszImagePathName, long rotation)
{
	JCOPY_OPTION copyoption; /* -copy switch */
	jpeg_transform_info transformoption; /* image transformation options */

	struct jpeg_decompress_struct srcinfo;
	struct jpeg_compress_struct dstinfo;
	struct jpeg_error_mgr jsrcerr, jdsterr;
#ifdef PROGRESS_REPORT
	struct cdjpeg_progress_mgr progress;
#endif
	jvirt_barray_ptr *src_coef_arrays;
	jvirt_barray_ptr *dst_coef_arrays;

	/* We assume all-in-memory processing and can therefore use only a
	* single file pointer for sequential input and output operation.
	*/
	FILE *fp;
	JOCTET *icc_profile = NULL;
	long icc_len = 0;
	/* Initialize the JPEG decompression object with default error handling. */
	srcinfo.err = jpeg_std_error(&jsrcerr);
	jpeg_create_decompress(&srcinfo);
	/* Initialize the JPEG compression object with default error handling. */
	dstinfo.err = jpeg_std_error(&jdsterr);
	jpeg_create_compress(&dstinfo);

	/* Scan command line to find file names.
	* It is convenient to use just one switch-parsing routine, but the switch
	* values read here are mostly ignored; we will rescan the switches after
	* opening the input file.  Also note that most of the switches affect the
	* destination JPEG object, so we parse into that and then copy over what
	* needs to affects the source too.
	*/

	if (rotation == 6)
	{
		transformoption.transform = JXFORM_ROT_90;
	}
	else if (rotation == 8)
	{
		transformoption.transform = JXFORM_ROT_270;
	}
	else
	{
		transformoption.transform = JXFORM_ROT_180;

	}

	copyoption = JCOPYOPT_DEFAULT;
	transformoption.perfect = FALSE;
	transformoption.trim = TRUE;
	transformoption.force_grayscale = FALSE;
	transformoption.crop = FALSE;
	transformoption.slow_hflip = FALSE;
	dstinfo.err->trace_level = 0;

	//file_index = parse_switches(&dstinfo, argc, argv, 0, FALSE);
	jsrcerr.trace_level = jdsterr.trace_level;
	srcinfo.mem->max_memory_to_use = dstinfo.mem->max_memory_to_use;

	_tfopen_s(&fp, lpszPathName, L"rb");


	/* Specify data source for decompression */
	jpeg_stdio_src(&srcinfo, fp);

	/* Enable saving of extra markers that we want to copy */
	jcopy_markers_setup(&srcinfo, copyoption);

	/* Read file header */
	(void)jpeg_read_header(&srcinfo, TRUE);

	/* Any space needed by a transform option must be requested before
	* jpeg_read_coefficients so that memory allocation will be done right.
	*/
#if TRANSFORMS_SUPPORTED
	/* Fail right away if -perfect is given and transformation is not perfect.
	*/
	if (!jtransform_request_workspace(&srcinfo, &transformoption)) {
		//fprintf(stderr, "%s: transformation is not perfect\n", progname);
		exit(EXIT_FAILURE);
	}
#endif

	/* Read source file as DCT coefficients */
	src_coef_arrays = jpeg_read_coefficients(&srcinfo);

	/* Initialize destination compression parameters from source values */
	jpeg_copy_critical_parameters(&srcinfo, &dstinfo);

	/* Adjust destination parameters if required by transform options;
	* also find out which set of coefficient arrays will hold the output.
	*/
#if TRANSFORMS_SUPPORTED
	dst_coef_arrays = jtransform_adjust_parameters(&srcinfo, &dstinfo,
		src_coef_arrays,
		&transformoption);
#else
	dst_coef_arrays = src_coef_arrays;
#endif

	/* Close input file, if we opened it.
	* Note: we assume that jpeg_read_coefficients consumed all input
	* until JPEG_REACHED_EOI, and that jpeg_finish_decompress will
	* only consume more while (!cinfo->inputctl->eoi_reached).
	* We cannot call jpeg_finish_decompress here since we still need the
	* virtual arrays allocated from the source object for processing.
	*/
	if (fp != stdin)
		fclose(fp);

	_tfopen_s(&fp, lpszImagePathName, L"wb");

	/* Adjust default compression parameters by re-parsing the options */
	////////////file_index = parse_switches(&dstinfo, argc, argv, 0, TRUE);

	/* Specify data destination for compression */
	jpeg_stdio_dest(&dstinfo, fp);

	/* Start compressor (note no image data is actually written here) */
	jpeg_write_coefficients(&dstinfo, dst_coef_arrays);

	/* Copy to the output file any extra markers that we want to preserve */
	jcopy_markers_execute(&srcinfo, &dstinfo, copyoption);

	if (icc_profile != NULL)
		jpeg_write_icc_profile(&dstinfo, icc_profile, (unsigned int)icc_len);

	/* Execute image transformation, if any */
#if TRANSFORMS_SUPPORTED
	jtransform_execute_transformation(&srcinfo, &dstinfo, src_coef_arrays,
		&transformoption);
#endif

	/* Finish compression and release memory */
	jpeg_finish_compress(&dstinfo);
	jpeg_destroy_compress(&dstinfo);
	(void)jpeg_finish_decompress(&srcinfo);
	jpeg_destroy_decompress(&srcinfo);

	/* Close output file, if we opened it */
	if (fp != stdout)
		fclose(fp);

#ifdef PROGRESS_REPORT
	end_progress_monitor((j_common_ptr)&dstinfo);
#endif

	if (icc_profile != NULL)
		free(icc_profile);

	/* All done. */
	//exit(jsrcerr.num_warnings + jdsterr.num_warnings ?
	//	EXIT_WARNING : EXIT_SUCCESS);
	return 0;                     /* suppress no-return-value warnings */
}

//
//unsigned long CImageProc::GetUniqueID()
//{
//	static unsigned long seq_id = 0;
//	unsigned long seq = seq_id++;
//	//seq = (seq << 8);
//	//seq = (seq >> 8);
//	//unsigned long long tag = ((unsigned long long)server_id << 56);
//	//seq = (seq | tag);
//	return seq;
//}
//HRESULT CImageProc::GetShellThumbnailImage(LPCWSTR pszPath, HBITMAP* pThumbnail)
//{
//	HRESULT hr;
//
//	*pThumbnail = NULL;
//
//	LPITEMIDLIST pidlItems = NULL, pidlURL = NULL, pidlWorkDir = NULL;
//	WCHAR szBasePath[MAX_PATH], szFileName[MAX_PATH];
//	WCHAR* p;
//	wcscpy_s(szBasePath, pszPath);
//	p = wcsrchr(szBasePath, L'\\');
//	if (p) *(p + 1) = L'\0';
//	wcscpy_s(szFileName, pszPath + (p - szBasePath) + 1);
//
//	while (TRUE)
//	{
//		CComPtr<IShellFolder> psfDesktop;
//		hr = SHGetDesktopFolder(&psfDesktop);
//		if (FAILED(hr)) break;
//
//		CComPtr<IShellFolder> psfWorkDir;
//		hr = psfDesktop->ParseDisplayName(NULL, NULL, szBasePath, NULL, &pidlWorkDir, NULL);
//		if (FAILED(hr)) break;
//		hr = psfDesktop->BindToObject(pidlWorkDir, NULL, IID_IShellFolder, (LPVOID*)&psfWorkDir);
//		if (FAILED(hr)) break;
//
//		hr = psfWorkDir->ParseDisplayName(NULL, NULL, szFileName, NULL, &pidlURL, NULL);
//		if (FAILED(hr)) break;
//
//		// query IExtractImage 
//		CComPtr<IExtractImage> peiURL;
//		hr = psfWorkDir->GetUIObjectOf(NULL, 1, (LPCITEMIDLIST*)&pidlURL, IID_IExtractImage, NULL, (LPVOID*)&peiURL);
//		if (FAILED(hr)) break;
//
//		// define thumbnail properties 
//		SIZE size = { 576, 432 };
//		DWORD dwPriority = 0, dwFlags = IEIFLAG_ASPECT;
//		WCHAR pszImagePath[MAX_PATH];
//		hr = peiURL->GetLocation(pszImagePath, MAX_PATH, &dwPriority, &size, 16, &dwFlags);
//		if (FAILED(hr)) break;
//
//		// generate thumbnail 
//		hr = peiURL->Extract(pThumbnail);
//		if (FAILED(hr)) break;
//
//		break;
//	}
//
//	// free allocated structures
//	if (pidlWorkDir) CoTaskMemFree(pidlWorkDir);
//	if (pidlURL) CoTaskMemFree(pidlURL);
//	return hr;
//}

int CImageProc::yuv420sp_to_jpg(const wchar_t *filename, int width, int height, unsigned char *pYUVBuffer)
{
	FILE *fJpg;
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	JSAMPROW row_pointer[1];
	int row_stride;
	int i = 0, j = 0;
	unsigned char* yuvbuf = (unsigned char*)malloc(width * 3);
	unsigned char *pY, *pU, *pV;
	int ulen;

	ulen = width * height / 4;

	if (pYUVBuffer == NULL) {
		//debugE("pBGRBuffer is NULL!\n");
		return -1;
	}

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	_wfopen_s(&fJpg, filename, L"wb");
	if (fJpg == NULL) {
		//debugE("Cannot open file %s, %s\n", filename, strerror(errno));
		jpeg_destroy_compress(&cinfo);
		return -1;
	}

	jpeg_stdio_dest(&cinfo, fJpg);
	cinfo.image_width = width;
	cinfo.image_height = height;
	cinfo.input_components = 3;
	cinfo.in_color_space = JCS_YCbCr;
	cinfo.dct_method = JDCT_ISLOW;


	jpeg_set_defaults(&cinfo);


	jpeg_set_quality(&cinfo, 99, TRUE);

	jpeg_start_compress(&cinfo, TRUE);
	row_stride = cinfo.image_width * 3; /* JSAMPLEs per row in image_buffer */

	pY = pYUVBuffer;
	pU = pYUVBuffer + width*height;
	pV = pYUVBuffer + width*height + ulen;

	j = 0;
	//j = 1;????????????????????????????????????
	while (cinfo.next_scanline < cinfo.image_height) {
		/* jpeg_write_scanlines expects an array of pointers to scanlines.
		* Here the array is only one element long, but you could pass
		* more than one scanline at a time if that's more convenient.
		*/

		/*Test yuv buffer serial is : yyyy...uu..vv*/
		if (j % 2 == 1 && j > 1) {
			pU = pYUVBuffer + width*height + width / 2 * (j / 2);
			pV = pYUVBuffer + width*height * 5 / 4 + width / 2 * (j / 2);
		}
		for (i = 0; i < width; i += 2) {
			yuvbuf[i * 3] = *pY++;
			yuvbuf[i * 3 + 1] = *pU;
			yuvbuf[i * 3 + 2] = *pV;

			yuvbuf[i * 3 + 3] = *pY++;
			yuvbuf[i * 3 + 4] = *pU++;
			yuvbuf[i * 3 + 5] = *pV++;
		}

		row_pointer[0] = yuvbuf;
		(void)jpeg_write_scanlines(&cinfo, row_pointer, 1);
		j++;
	}

	jpeg_finish_compress(&cinfo);

	jpeg_destroy_compress(&cinfo);
	fclose(fJpg);

	free(yuvbuf);

	return 0;
}

int CImageProc::proc_mp4_thumbnail(wstring szFile, wstring dst, long rotate)
{
	if (!_func)
	{
		return 1;
	}

	wstring tempFile = dst + L"0.jpg";
	int width = 0;
	int height = 0;

	unsigned char *pYUVBuffer = NULL;
	int ret = _func(szFile.c_str(), tempFile.c_str(), width, height, &pYUVBuffer);
	if (ret == 0)
	{
		yuv420sp_to_jpg(tempFile.c_str(), width, height, pYUVBuffer);
		LocalFree(pYUVBuffer);

		int w = width;
		if (height > width)
		{
			w = height;
		}

		int scale_denom = 8;

		div_t div_result = div(w, 1000);
		if (div_result.quot < 2)
		{
			scale_denom = 1;
		}
		else if (div_result.quot <= 3)
		{
			scale_denom = 2;
		}
		else if (div_result.quot <= 6)
		{
			scale_denom = 4;
		}

		int deg = 1;
		if (rotate == 90)
		{
			deg = 6;
		}
		else if (rotate == 270)
		{
			deg = 8;
		}
		else if (rotate == 180)
		{
			deg = 3;

		}
		BOOL b = jp_test(tempFile.c_str(), dst.c_str(), scale_denom, deg);
		if (!b)
		{
			ret = 1;
		}

	}
	DeleteFile(tempFile.c_str());

	return ret;
}



