// mfthumb.cpp : Defines the exported functions for the DLL application.
//
#include "stdafx.h"

#include <io.h>
#include <shlobj.h>
#include <thumbcache.h>
#include <string>
#include<fstream>

#include "photol.h"
#include "unitil2.h"
#include "unitil3.h"

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/objdetect.hpp>
#include <opencv2/face.hpp>

#include <MediaInfo/MediaInfo.h>
#include <gif_lib.h>
#include <process.h>
#include "resource.h"
#include "Blowfish.h"
#include "unitil2.h"

#include "paint.h"

using namespace std;
using namespace cv;
using namespace cv::face;

//#pragma comment( lib, "ippicvmt.lib" )
//#pragma comment( lib, "ippiw.lib" )
//#pragma comment( lib, "IlmImf.lib" )

#pragma comment( lib, "libtiff.lib" )
#pragma comment( lib, "libpng.lib" )
//#pragma comment( lib, "libjasper.lib" )
#pragma comment( lib, "libjpeg-turbo.lib" )
#pragma comment( lib, "libwebp.lib" )
#pragma comment( lib, "libopenjp2.lib" )
#pragma comment( lib, "zlib.lib" )

#pragma comment( lib, "opencv_core480.lib" )
#pragma comment( lib, "opencv_imgcodecs480.lib" )
#pragma comment( lib, "opencv_imgproc480.lib" )
#pragma comment( lib, "opencv_quality480.lib" )
#pragma comment( lib, "opencv_ml480.lib" )
#pragma comment( lib, "opencv_highgui480.lib" ) 
#pragma comment( lib, "opencv_photo480.lib" ) 

#pragma comment( lib, "opencv_objdetect480.lib" )
#pragma comment( lib, "opencv_face480.lib" )

#pragma comment( lib, "giflib.lib" )
#pragma comment( lib, "ZenLib.lib" ) 
#pragma comment( lib, "MediaInfo-Static.lib" ) 
#pragma comment( lib, "lunasvg.lib" ) 

CBlowfish* pBF = NULL;
CPaint* pPaint = NULL;
Mat _artMat;

wstring _alb = L"\\data2";
wstring _root = L"";
wstring _cascade_file = L"";
wstring _eye1_file = L"";
wstring _eye2_file = L"";
string _model_file = "";

Ptr<LBPHFaceRecognizer> _model = nullptr;
CascadeClassifier cascade_face;
CascadeClassifier classifier_eye1;
CascadeClassifier classifier_eye2;

Ptr<quality::QualityBRISQUE> _brisque = nullptr;

extern HINSTANCE _his;


double BRISQUE(Mat img)
{
	if (img.cols <= 16 || img.rows <= 16)
	{
		return 0;
	}

	if (_brisque == nullptr)
	{
		wstring root = get_module_path0(NULL);

		// path to the trained model
		wstring model_path = root + L"\\model\\brisque_model_live.yml";
		// path to range file
		wstring range_path = root + L"\\model\\brisque_range_live.yml";

		try {
			_brisque = quality::QualityBRISQUE::create(_w2u(model_path), _w2u(range_path));
		}
		catch (...) {
		}
	}
	if (_brisque == nullptr)
	{
		return 0;
	}
	//Scalar result_static = quality::QualityBRISQUE::compute(img, _w2u(model_path), _w2u(range_path));
	Scalar result_static = _brisque->compute(img);

	return 100 - calMEAN(result_static);
}

void add_arr(Mat& bg0)
{
	int channels = bg0.channels();
	if (channels == 4)
	{
		cvtColor(bg0, bg0, CV_BGRA2BGR);
	}

	channels = bg0.channels();
	if (channels == 3)
	{
		if (_artMat.empty())
		{
			//HINSTANCE his = ::GetModuleHandle(L"photol.dll");
			load_bitmap(_his, _artMat, IDB_ART);
		}

		if (bg0.cols > _artMat.cols && bg0.rows > _artMat.rows)
		{
			int	width = _artMat.cols;
			int height = _artMat.rows;
			Mat roi = bg0(Rect((bg0.cols - width) / 2, (bg0.rows - height) / 2, width, height));

			for (int i = 0; i < roi.rows; i++)
			{
				for (int j = 0; j < roi.cols; j++)
				{
					Vec3b& v1 = roi.at<Vec3b>(i, j);
					Vec3b v2 = _artMat.at<Vec3b>(i, j);
					if (v2[0] == 0 && v2[1] == 0 && v2[2] == 255)
					{
					}
					else
					{
						v1 = v2;
					}
				}
			}
		}
	}
}

unsigned char* memstr(unsigned char* full_data, int full_data_len, unsigned char* substr, int sublen)
{
	if (full_data == NULL || full_data_len <= 0 || substr == NULL)
	{
		return NULL;
	}

	if (*substr == '\0')
	{
		return NULL;
	}

	//int sublen = strlen(substr);

	int i;
	unsigned char* cur = full_data;
	int last_possible = full_data_len - sublen + 1;
	for (i = 0; i < last_possible; i++)
	{
		if (*cur == *substr)
		{
			//assert(full_data_len - i >= sublen);
			if (memcmp(cur, substr, sublen) == 0)
			{
				//found
				return cur;
			}
		}
		cur++;
	}

	return NULL;
}

int write_file(wstring szDst, vector<uchar>& buf, bool bEnc)
{
	if (bEnc)
	{
		szDst += L".krf";
	}

	ofstream file3(szDst.c_str(), ios::out | ios::binary);
	if (!file3)
	{
		return -1;
	}

	if (bEnc)
	{
		int lengthActual = (int)buf.size();

		unsigned char buf_hdr[100] = { 0 };

		DE_HEADER *de_hdr = (DE_HEADER *)buf_hdr;
		de_hdr->type = 1;

		div_t div_result = div(lengthActual, 8);
		if (div_result.rem != 0)
		{
			de_hdr->offset = 8 - div_result.rem;
		}

		de_hdr->crc = pBF->_crc;

		srand((unsigned int)time(NULL));
		de_hdr->ot1 = rand() * 255;
		de_hdr->ot2 = rand() * 255;
		de_hdr->ot3 = 2024;

		file3.write((char*)buf_hdr, sizeof(DE_HEADER) * sizeof(uchar));

		if (div_result.rem != 0)
		{
			char tempbuf[100] = { 0 };
			memcpy(tempbuf + de_hdr->offset, (char*)&buf[0], div_result.rem);

			pBF->Encrypt((void*)tempbuf, 8);
			file3.write((char*)tempbuf, 8);
		}

		pBF->Encrypt((void*)&buf[div_result.rem], lengthActual - div_result.rem);
		file3.write((char*)&buf[div_result.rem], lengthActual - div_result.rem);
	}
	else
	{
		file3.write((char*)&buf[0], buf.size() * sizeof(uchar));
	}

	file3.close();

	return 0;
}

int bgra2bgr(Mat& src2)
{
	int channels = src2.channels();
	if (channels == 4)
	{
		for (int i = 0; i < src2.rows; i++)
		{
			Vec4b* ptr = src2.ptr<Vec4b>(i);
			for (int j = 0; j < src2.cols; j++)
			{
				Vec4b& v = ptr[j];
				if (v[3] != 255)
				{
					float alphaReserve = (float)v[3] / 255.0;
					v[0] = saturate_cast<uchar>((int)(v[0] * alphaReserve + 255 * (1 - alphaReserve)));
					v[1] = saturate_cast<uchar>((int)(v[1] * alphaReserve + 255 * (1 - alphaReserve)));
					v[2] = saturate_cast<uchar>((int)(v[2] * alphaReserve + 255 * (1 - alphaReserve)));
				}
			}
		}

		cvtColor(src2, src2, CV_BGRA2BGR);
	}
	return 0;
}

int save_mat(int ty, const char* ext, Mat src, const wchar_t* szDst, bool ismp4, bool bEnc)
{
	string ext2 = ext;
	vector<int> ql;
	if (strcmp(ext, ".png") == 0)
	{
		ext2 = ".png";
		ql.push_back(IMWRITE_PNG_COMPRESSION);
		ql.push_back(3);
	}
	else if (strcmp(ext, ".webp") == 0)
	{
		ext2 = ".webp";
		//q.push_back(IMWRITE_WEBP_QUALITY);
		//q.push_back(64);
	}
	else /*if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0)*/
	{
		ext2 = ".jpg";
		ql.push_back(IMWRITE_JPEG_QUALITY);
		ql.push_back(70);
	}

	wchar_t dst[MAX_PATH] = { 0 };
	DWORD dwSize = GetFullPathName(szDst, MAX_PATH, dst, NULL);

	int width = src.cols;
	int height = src.rows;

	if (ty == 2)
	{
		Mat ResImg;
		if (width == 50 && height == 50) {
			ResImg = src;
		}
		else
		{
			resize(src, ResImg, Size(50, 50), 0, 0);
		}
		bgra2bgr(ResImg);

		if (ismp4)
		{
			add_arr(ResImg);
		}

		vector<uchar> buf;
		imencode(ext, ResImg, buf, ql);

		return write_file(szDst, buf, bEnc);
		//ofstream file2(szDst, ios::out | ios::binary);
		//if (!file2)
		//{
		//	return -1;
		//}
		//file2.write((char*)&buf[0], buf.size() * sizeof(uchar));
		//file2.close();
	}
	else
	{
		double scale = get_scale(ty, width, height);

		Size ResImgSiz = Size(0, 0);
		Mat ResImg = Mat(ResImgSiz, src.type());
		if (scale < 2)
		{
			ResImg = src;
		}
		else
		{
			double f = 1.0f / scale;
			resize(src, ResImg, ResImgSiz, f, f);
		}

		vector<uchar> buf;
		imencode(ext2.c_str(), ResImg, buf, ql);

		int ret = write_file(dst, buf, bEnc);
		if (ret != 0)
		{
			return ret;
		}
		//ofstream file2(dst, ios::out | ios::binary);
		//if (!file2)
		//{
		//	return -1;
		//}
		//file2.write((char*)&buf[0], buf.size() * sizeof(uchar));
		//file2.close();

		Mat ResImg2;
		if (width == 50 && height == 50) {
			ResImg2 = ResImg;
		}
		else
		{
			resize(ResImg, ResImg2, Size(50, 50), 0, 0);
		}

		bgra2bgr(ResImg2);
		if (ismp4)
		{
			add_arr(ResImg2);
		}

		vector<uchar> buf3;
		imencode(ext2.c_str(), ResImg2, buf3, ql);

		wstring szDst2 = dst;
		szDst2 += L"_2.png";
		return write_file(szDst2, buf3, bEnc);

		//ofstream file3(szDst2.c_str(), ios::out | ios::binary);
		//if (!file3)
		//{
		//	return -1;
		//}
		//file3.write((char*)&buf3[0], buf3.size() * sizeof(uchar));
		//file3.close();
	}
	return 0;
}

int openmp4file(const wchar_t* sMp4file, uint32_t& width, uint32_t& height, uint32_t& duration, long& mp4rotate)
{
	wstring W, H;
	MediaInfoLib::MediaInfo* _MI = new MediaInfoLib::MediaInfo();
	_MI->Open(sMp4file);

	W = _MI->Get(MediaInfoLib::stream_t::Stream_Video, 0, L"Width");
	H = _MI->Get(MediaInfoLib::stream_t::Stream_Video, 0, L"Height");
	wstring d = _MI->Get(MediaInfoLib::stream_t::Stream_Video, 0, L"Duration");
	wstring r = _MI->Get(MediaInfoLib::stream_t::Stream_Video, 0, L"Rotation");

	width = _wtoi(W.c_str());
	height = _wtoi(H.c_str());
	duration = _wtol(d.c_str()) / 1000;
	mp4rotate = _wtoi(r.c_str());
	//90.000

	if (mp4rotate == 90 || mp4rotate == 270)
	{
		long t = width;
		width = height;
		height = t;
	}

	_MI->Close();
	delete _MI;

	if (width > 0)
	{
		return 0;
	}

	return -1;
}

inline bool IsWindowsVersionOrGreater(WORD wMajorVersion, WORD wMinorVersion, WORD wServicePackMajor)
{

	typedef LONG(__stdcall* fnRtlGetVersion)(PRTL_OSVERSIONINFOW lpVersionInformation);

	RTL_OSVERSIONINFOEXW verInfo = { 0 };
	verInfo.dwOSVersionInfoSize = sizeof(verInfo);

	static auto RtlGetVersion = (fnRtlGetVersion)GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "RtlGetVersion");

	if (RtlGetVersion != 0 && RtlGetVersion((PRTL_OSVERSIONINFOW)&verInfo) == 0)
	{
		if (verInfo.dwMajorVersion > wMajorVersion)
			return true;
		else if (verInfo.dwMajorVersion < wMajorVersion)
			return false;

		if (verInfo.dwMinorVersion > wMinorVersion)
			return true;
		else if (verInfo.dwMinorVersion < wMinorVersion)
			return false;

		if (verInfo.wServicePackMajor >= wServicePackMajor)
			return true;
	}

	return false;
}

inline bool IsWindows8OrGreater()
{
	return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WIN8), LOBYTE(_WIN32_WINNT_WIN8), 0);
}

inline void NewSize(Mat src2)
{
	Size si = src2.size();
	//3840 2160
	//	7680 4320
	//	6000 3000
	//	6000 4000
	//	4096 2160
	//	6250 3515
	//	5000 2800
	//	------ - 1920 2048 2000

	int val = max(si.width, si.height);
	if (val > 5000)
	{
		float f = 1.0f;
		div_t div_result = div(val, 7);
		if (div_result.rem == 0 && div_result.quot > 2000 && div_result.quot < 4000)
		{
			f = 7.0f;
		}
		div_result = div(val, 6);
		if (div_result.rem == 0 && div_result.quot > 2000 && div_result.quot < 4000)
		{
			f = 6.0f;
		}
		div_result = div(val, 5);
		if (div_result.rem == 0 && div_result.quot > 2000 && div_result.quot < 4000)
		{
			f = 5.0f;
		}
		div_result = div(val, 4);
		if (div_result.rem == 0 && div_result.quot > 2000 && div_result.quot < 4000)
		{
			f = 4.0f;
		}
		div_result = div(val, 3);
		if (div_result.rem == 0 && div_result.quot > 2000 && div_result.quot < 4000)
		{
			f = 3.0f;
		}
		div_result = div(val, 2);
		if (div_result.rem == 0 && div_result.quot > 2000 && div_result.quot < 4000)
		{
			f = 2.0f;
		}

		if (f < 2.0f)
		{
			if (val > 10000)
			{
				f = 4.0f;
			}
			else
			{
				f = 2.0f;
			}
		}
		resize(src2, src2, Size((int)(si.width / f), (int)(si.height / f)), 0, 0);
	}
}

HRESULT proc_bimap(int ty, HBITMAP hbmp, wstring dst, long& mp4rotate)
{
	if (hbmp)
	{
		BITMAP bm = { 0 };
		int ret = GetObject(hbmp, sizeof(BITMAP), (LPSTR)&bm);
		if (ret == 0 || ret > sizeof(BITMAP))
		{
			return S_FALSE;
		}

		int nChannels = bm.bmBitsPixel == 1 ? 1 : bm.bmBitsPixel / 8;
		int depth = bm.bmBitsPixel == 1 ? IPL_DEPTH_1U : IPL_DEPTH_8U;

		Mat src;
		src.create(cvSize(bm.bmWidth, bm.bmHeight), CV_MAKETYPE(CV_8U, nChannels));
		GetBitmapBits(hbmp, bm.bmHeight * bm.bmWidth * nChannels, src.data);

		if (mp4rotate == 90 || mp4rotate == 270)
		{
			if (!IsWindows8OrGreater())
			{
				Mat temp;
				transpose(src, temp);
				flip(temp, src, 1);
			}
		}

		if (src.data == NULL)
		{
			return 2;
		}

		ret = save_mat(ty, ".png", src, dst.c_str(), true, (ty == 0));
		if (ret == 0)
		{
			return 0;
		}
	}
	return S_FALSE;
}

HRESULT GetThumbnailEx(int ty, uint32_t cx, wstring szFile, wstring dst, long& mp4rotate)
{
	HRESULT hr;

	IShellItem* psi;
	hr = SHCreateItemFromParsingName(szFile.c_str(), NULL, IID_PPV_ARGS(&psi));
	if (SUCCEEDED(hr))
	{
		IThumbnailProvider* pThumbProvider;
		hr = psi->BindToHandler(NULL, BHID_ThumbnailHandler, IID_PPV_ARGS(&pThumbProvider));
		if (SUCCEEDED(hr))
		{
			HBITMAP hThumbnail = NULL;
			WTS_ALPHATYPE wtsAlpha = WTSAT_UNKNOWN;
			hr = pThumbProvider->GetThumbnail(cx, &hThumbnail, &wtsAlpha);
			if (SUCCEEDED(hr))
			{
				hr = proc_bimap(ty, hThumbnail, dst, mp4rotate);
				DeleteObject(hThumbnail);
			}

			pThumbProvider->Release();
		}
		psi->Release();
	}

	return hr;
}

HRESULT GetCacheThumbnail(int ty, uint32_t width, uint32_t height, const wchar_t* szFile, wstring dst, long& mp4rotate)
{
	IShellItem* item = nullptr;
	HRESULT hr = SHCreateItemFromParsingName(szFile, nullptr, IID_PPV_ARGS(&item));
	if (hr == S_OK)
	{
		IThumbnailCache* cache = nullptr;
		hr = CoCreateInstance(
			CLSID_LocalThumbnailCache,
			nullptr,
			CLSCTX_INPROC,
			IID_PPV_ARGS(&cache));
		if (hr == S_OK)
		{
			ISharedBitmap* shared_bitmap;
			hr = cache->GetThumbnail(
				item,
				width * height,
				WTS_EXTRACT,
				&shared_bitmap,
				nullptr,
				nullptr);

			if (hr == S_OK)
			{
				// Retrieve thumbnail HBITMAP
				HBITMAP hThumbnail = NULL;
				hr = shared_bitmap->GetSharedBitmap(&hThumbnail);
				if (hr == S_OK)
				{
					hr = proc_bimap(ty, hThumbnail, dst, mp4rotate);
					DeleteObject(hThumbnail);
				}

				shared_bitmap->Release();
			}

			cache->Release();
		}
		item->Release();
	}
	return hr;
}

PHOTOL_API int set_key(const char* enckey)
{
	if (pBF != NULL)
	{
		delete pBF;
		pBF = NULL;
	}

	if (pBF == NULL)
	{
		pBF = new CBlowfish();
		pBF->SetPassword((char*)enckey);
	}

	return 0;
}

PHOTOL_API int resize_img(const char* ext, const wchar_t* szSrc, const wchar_t* szDst, uint32_t& width, uint32_t& height, uint32_t& level)
{
	wchar_t src[MAX_PATH] = { 0 };
	DWORD dwSize = GetFullPathName(szSrc, MAX_PATH, src, NULL);

	ifstream file(src, ios::in | ios::binary | ios::ate);
	if (!file.is_open())
	{
		return -1;
	}

	streampos size = file.tellg();
	file.seekg(0, ios::beg);
	string buffer(size, 0);
	file.read(&buffer[0], size);
	file.close();
	if ((int)size == 0)
	{
		return 2;
	}

	vector<uchar> vec_data(&buffer[0], &buffer[0] + size);
	Mat src2 = imdecode(vec_data, IMREAD_UNCHANGED);
	int u = src2.depth();
	if (src2.data != NULL && u != 0)
	{
		Mat temp = imdecode(vec_data, IMREAD_ANYCOLOR);
		temp.convertTo(src2, CV_8U);
	}

	if (src2.data == NULL)
	{
		return 2;
	}

	int ty = 2;
	if (width == 0 && height == 0)
	{
		ty = 0;
	}

	width = src2.cols;
	height = src2.rows;

	if (ty == 2 && (width < 16 || height < 16))
	{
		//		return 1;
	}
	int ret = save_mat(ty, ext, src2, szDst, false, (level == 0));
	//if (level == 1)
	//{
	//	NewSize(src2);
	//	level = (uint32_t)(BRISQUE(src2) * 100);

	//}

	return ret;
}

PHOTOL_API int img_brisque(const wchar_t* szSrc, const char* enckey, uint32_t& level)
{
	wchar_t src[MAX_PATH] = { 0 };
	DWORD dwSize = GetFullPathName(szSrc, MAX_PATH, src, NULL);

	ifstream file(src, ios::in | ios::binary | ios::ate);
	if (!file.is_open())
	{
		return -1;
	}

	streampos size = file.tellg();
	file.seekg(0, ios::beg);
	string buffer(size, 0);
	file.read(&buffer[0], size);
	file.close();
	if ((int)size == 0)
	{
		return 2;
	}
	vector<uchar> vec_data;

	if (strlen(enckey) == 0)
	{
		vec_data = vector<uchar>(&buffer[0], &buffer[0] + size);
	}
	else
	{
		DWORD lengthActual = (DWORD)size;

		DE_HEADER* de_hdr = (DE_HEADER*)buffer.c_str();
		int offset = 8 + de_hdr->offset;

		pBF->Decrypt((void*)(buffer.c_str() + 8), lengthActual - 8);

		vec_data = vector<uchar>(&buffer[offset], &buffer[offset] + lengthActual - offset);
	}

	//vector<uchar> vec_data(&buffer[0], &buffer[0] + size);
	Mat src2 = imdecode(vec_data, IMREAD_UNCHANGED);
	int u = src2.depth();
	if (src2.data != NULL && u != 0)
	{
		Mat temp = imdecode(vec_data, IMREAD_ANYCOLOR);
		temp.convertTo(src2, CV_8U);
	}

	if (src2.data == NULL)
	{
		return 2;
	}

	NewSize(src2);
	level = (uint32_t)(BRISQUE(src2) * 100);

	return 0;
}

PHOTOL_API int img_size(const wchar_t* szSrc, uint32_t& width, uint32_t& height)
{
	wchar_t src[MAX_PATH] = { 0 };
	DWORD dwSize = GetFullPathName(szSrc, MAX_PATH, src, NULL);

	ifstream file(src, ios::in | ios::binary | ios::ate);
	if (!file.is_open())
	{
		return -1;
	}

	streampos size = file.tellg();
	file.seekg(0, ios::beg);
	string buffer(size, 0);
	file.read(&buffer[0], size);
	file.close();
	if ((int)size == 0)
	{
		return 2;
	}

	vector<uchar> vec_data(&buffer[0], &buffer[0] + size);
	Mat src2 = imdecode(vec_data, IMREAD_UNCHANGED);
	int u = src2.depth();
	if (src2.data != NULL && u != 0)
	{
		Mat temp = imdecode(vec_data, IMREAD_ANYCOLOR);
		temp.convertTo(src2, CV_8U);
	}

	if (src2.data == NULL)
	{
		return 2;
	}

	width = src2.cols;
	height = src2.rows;

	return 0;
}

PHOTOL_API int capture_mp4(const wchar_t* szSrc, const wchar_t* szDst, uint32_t& width, uint32_t& height, uint32_t& duration)
{
	int ty = 2;
	if (width == 0 && height == 0)
	{
		ty = 0;
	}

	int rt = 2;
	wchar_t src[MAX_PATH] = { 0 };
	DWORD dwSize = GetFullPathName(szSrc, MAX_PATH, src, NULL);

	long mp4rotate = 0;
	int ret = openmp4file(src, width, height, duration, mp4rotate);
	if (ret != 0)
	{
		width = 576;
		height = 576;
	}

	HRESULT hr = GetThumbnailEx(ty, max(width, height), src, szDst, mp4rotate);
	if (hr != S_OK)
	{
		Sleep(100);
		hr = GetCacheThumbnail(ty, width, height, src, szDst, mp4rotate);
	}
	if (hr == 0)
	{
		rt = 0;
	}
	else
	{
		width = 0;
		height = 0;
		rt = 1;
	}


	if (rt != 0 && ty == 2)
	{
		//url0 = sample + L"\\templ\\themes\\default\\images\\mp4.png";
		Mat mp4;
		//HINSTANCE his = ::GetModuleHandle(L"photol.dll");
		load_bitmap(_his, mp4, IDB_MP4);
		if (!mp4.empty())
		{
			rt = save_mat(ty, ".png", mp4, szDst, false, false);
		}
	}
	return rt;
}

int capture_gif(const wchar_t* szSrc, const wchar_t* szDst, uint32_t& width, uint32_t& height)
{
	int ty = 2;
	if (width == 0 && height == 0)
	{
		ty = 0;
	}

	wchar_t src[MAX_PATH] = { 0 };
	DWORD dwSize = GetFullPathName(szSrc, MAX_PATH, src, NULL);

	int ret = 2;
	int err;

	HANDLE hFile = CreateFile(src, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return ret;
	}

	bool error = false;
	GifFileType* GifFile = NULL;
	int fd = ::_open_osfhandle(reinterpret_cast <::intptr_t> (hFile), _A_RDONLY);
	if (-1 == fd)
	{
		error = true;
	}
	else
	{
		GifFile = DGifOpenFileHandle(fd, &err);
		if (GifFile == NULL)
		{
			error = true;
		}
	}

	if (error)
	{
		//CloseHandle(hFile);
		return ret;
	}

	int trans_color = -1;

	GifRowType* ScreenBuffer = (GifRowType*)malloc(GifFile->SHeight * sizeof(GifRowType));
	if (ScreenBuffer == NULL)
	{
		error = true;
	}
	else
	{
		int len0 = GifFile->SWidth * sizeof(GifPixelType);
		ScreenBuffer[0] = (GifRowType)malloc(len0);
		if (ScreenBuffer[0] == NULL)
		{
			error = true;
		}
		else
		{
			for (int i = 0; i < GifFile->SWidth; i++)
				ScreenBuffer[0][i] = GifFile->SBackGroundColor;

			for (int i = 1; i < GifFile->SHeight; i++)
			{
				ScreenBuffer[i] = (GifRowType)malloc(len0);
				if (ScreenBuffer[i] == NULL)
				{
					error = true;
				}
				else
				{
					memcpy(ScreenBuffer[i], ScreenBuffer[0], len0);
				}
			}
		}
	}

	if (error)
	{
		if (ScreenBuffer != NULL)
		{
			free(ScreenBuffer);
		}

		DGifCloseFile(GifFile, &err);
		return ret;
	}

	int	InterlacedOffset[] = { 0, 4, 2, 1 };
	int	InterlacedJumps[] = { 8, 8, 4, 2 };


	GifRecordType RecordType;
	do
	{
		if (DGifGetRecordType(GifFile, &RecordType) == GIF_ERROR)
		{
			break;
		}

		switch (RecordType)
		{
		case IMAGE_DESC_RECORD_TYPE:
		{
			if (DGifGetImageDesc(GifFile) == GIF_ERROR)
			{
				error = true;
				break;
			}

			int Row = GifFile->Image.Top;
			int Col = GifFile->Image.Left;
			width = GifFile->Image.Width;
			height = GifFile->Image.Height;

			if (GifFile->Image.Left + GifFile->Image.Width > GifFile->SWidth ||
				GifFile->Image.Top + GifFile->Image.Height > GifFile->SHeight)
			{
				error = true;
				break;
			}

			if (GifFile->Image.Interlace)
			{
				for (int i = 0; i < 4; i++)
				{
					for (int j = Row + InterlacedOffset[i]; j < Row + height; j += InterlacedJumps[i])
					{
						if (DGifGetLine(GifFile, &ScreenBuffer[j][Col], width) == GIF_ERROR)
						{
							error = true;
							break;
						}
					}

					if (error)
					{
						break;
					}
				}
			}
			else
			{
				for (uint32_t i = 0; i < height; i++)
				{
					if (DGifGetLine(GifFile, &ScreenBuffer[Row++][Col], width) == GIF_ERROR)
					{
						error = true;
						break;
					}
				}
			}

			ColorMapObject* ColorMap = NULL;
			if (!error)
			{
				ColorMap = (GifFile->Image.ColorMap ? GifFile->Image.ColorMap : GifFile->SColorMap);
				if (NULL == ColorMap)
				{
					error = true;
				}
			}

			if (!error)
			{
				width = GifFile->SWidth;
				height = GifFile->SHeight;

				Mat	img = Mat(Size(GifFile->SWidth, GifFile->SHeight), CV_8UC3);
				GifRowType GifRow;
				GifColorType* ColorMapEntry;
				for (int i = 0; i < GifFile->SHeight; i++)
				{
					GifRow = ScreenBuffer[i];
					for (int j = 0; j < GifFile->SWidth; j++)
					{
						if (trans_color != -1 && trans_color == GifRow[j]) {
							img.at<Vec3b>(i, j) = Vec3b(255, 255, 255);
						}
						else
						{
							ColorMapEntry = &ColorMap->Colors[GifRow[j]];
							img.at<Vec3b>(i, j) = Vec3b(ColorMapEntry->Blue, ColorMapEntry->Green, ColorMapEntry->Red);
						}
					}
				}

				ret = save_mat(ty, ".png", img, szDst, false, (ty == 0));

				error = true;
			}
		}
		break;
		case EXTENSION_RECORD_TYPE:
		{
			int  ExtCode = 0;
			GifByteType *Extension = NULL;

			if (DGifGetExtension(GifFile, &ExtCode, &Extension) == GIF_ERROR)
			{
				error = true;
				break;
			}

			if (ExtCode == GRAPHICS_EXT_FUNC_CODE)
			{
				GraphicsControlBlock gcb;
				if (Extension == NULL)
				{
					error = true;
					break;
				}
				if (DGifExtensionToGCB(Extension[0], Extension + 1, &gcb) == GIF_ERROR)
				{
					error = true;
					break;
				}

				trans_color = gcb.TransparentColor;
			}

			for (;;)
			{
				if (DGifGetExtensionNext(GifFile, &Extension) == GIF_ERROR)
				{
					error = true;
					break;
				}

				if (Extension == NULL)
				{
					break;
				}
			}
		}
		break;
		case TERMINATE_RECORD_TYPE:
			break;
		default:
			break;
		}

		if (error)
		{
			break;
		}

	} while (RecordType != TERMINATE_RECORD_TYPE);

	free(ScreenBuffer);
	DGifCloseFile(GifFile, &err);
	return ret;
}

PHOTOL_API int mp3_cover(int ty, const wchar_t* szSrc, const wchar_t* szDst, long& duration, bool& cover)
{
	wchar_t src[MAX_PATH] = { 0 };
	DWORD dwSize = GetFullPathName(szSrc, MAX_PATH, src, NULL);

	int rt = 1;
	cover = false;

	MediaInfoLib::MediaInfo* _MI = new MediaInfoLib::MediaInfo();

	_MI->Open(src);
	wstring d = _MI->Get(MediaInfoLib::stream_t::Stream_Audio, 0, L"Duration");
	duration = _wtol(d.c_str()) / 1000;

	wstring cover0 = _MI->Get(MediaInfoLib::stream_t::Stream_General, 0, L"Cover");
	_MI->Close();
	delete _MI;

	if (duration == 0)
	{
		rt = -1;
	}
	else
	{
		if (cover0 != L"")
		{
			char* pic1 = NULL;
			int filesize = 0;

			HANDLE hFile = CreateFile(src, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
			if (hFile != INVALID_HANDLE_VALUE)
			{
				int nTemp = 1000 * 1024;
				unsigned char* tempbuf = (unsigned char*)malloc(nTemp + 1);
				if (tempbuf != NULL)
				{
					DWORD dwRead = 0;
					memset(tempbuf, 0, nTemp + 1);

					BOOL b = ReadFile(hFile, tempbuf, nTemp, &dwRead, 0);
					if (b)
					{
						unsigned char buf2[10] = { 0 };
						memcpy(buf2, "APIC", 4);
						unsigned char* p = memstr(tempbuf, dwRead, buf2, 4);
						if (p != NULL)
						{
							unsigned char* code = (unsigned char*)p + 4;
							filesize = code[0] * 0x1000000
								+ code[1] * 0x10000
								+ code[2] * 0x100
								+ code[3] * 0x1;

							if (filesize + p + 10 - tempbuf < nTemp)
							{
								unsigned char sub[3] = { 0xff,0xd8,0 };
								//unsigned char *pt = sub;
								unsigned char* p1 = memstr(p + 10, filesize, sub, 2);
								if (p1 != NULL)
								{
									filesize -= (int)(p1 - p - 10);
									pic1 = (char*)malloc(filesize);
									memset(pic1, 0, filesize);
									memcpy(pic1, p1, filesize);
								}
							}
						}
					}

					free(tempbuf);
				}

				CloseHandle(hFile);
			}

			if (pic1 != NULL)
			{
				vector<uchar> vec_data(pic1, pic1 + filesize);
				Mat src2 = imdecode(vec_data, 1);

				if (src2.data == NULL)
				{
					free(pic1);
					rt = 2;
				}
				else
				{
					rt = save_mat(ty, ".png", src2, szDst, true, (ty == 0));
					free(pic1);

					cover = true;
				}
			}
		}
	}

	if (rt != 0 && ty == 2)
	{
		//url0 = sample + L"\\templ\\themes\\default\\images\\mp4.png";
		Mat mp3;
		//HINSTANCE his = ::GetModuleHandle(L"photol.dll");
		load_bitmap(_his, mp3, IDB_MP3);
		if (!mp3.empty())
		{
			rt = save_mat(ty, ".png", mp3, szDst, false, false);
		}
	}
	return rt;

}

PHOTOL_API int paint_show(int type, uint64_t hwnd, int pixelsy, int w, int h, const wchar_t* fname, const wchar_t* langue, const wchar_t* user, const wchar_t* enckey, int ov)
{
	if (pPaint == NULL)
	{
		pPaint = new CPaint();
		pPaint->init2(type, pixelsy, langue, user, ov);
	}

	pPaint->show((HWND)hwnd, w, h, fname, enckey);

	return 0;
}

PHOTOL_API int capture_ot(int type, const wchar_t* szDst)
{
	int ret = 0;
	UINT id = IDB_MP3;
	if (type == 1)
	{
		id = IDB_MP4;
	}
	else if (type == 2)
	{
		id = IDB_JPG;
	}
	Mat mp3;
	//HINSTANCE his = ::GetModuleHandle(L"photol.dll");
	load_bitmap(_his, mp3, id);
	if (!mp3.empty())
	{
		ret = save_mat(2, ".png", mp3, szDst, false, false);
	}
	return ret;
}

int saveface(const char* enckey, Mat ori, wstring szDst2)
{
	vector<uchar> buf0;
	imencode(".jpg", ori, buf0);
	if (strlen(enckey) != 0)
	{
		szDst2 += L".krf";
	}

	ofstream file2(szDst2.c_str(), ios::out | ios::binary);
	if (file2)
	{
		if (strlen(enckey) != 0)
		{
			DWORD lengthActual = (DWORD)buf0.size();

			int nTemp = lengthActual + 32;
			unsigned char* tempbuf = (unsigned char*)malloc(nTemp + 1);
			if (tempbuf != NULL)
			{
				memset(tempbuf, 0, nTemp + 1);

				DE_HEADER* de_hdr = (DE_HEADER*)tempbuf;
				de_hdr->type = 1;
				div_t div_result = div(lengthActual, 8);
				if (div_result.rem != 0)
				{
					de_hdr->offset = 8 - div_result.rem;
				}
				de_hdr->crc = pBF->_crc;

				srand((unsigned int)time(NULL));
				de_hdr->ot1 = rand() * 255;
				de_hdr->ot2 = rand() * 255;
				de_hdr->ot3 = 2024;

				memcpy(tempbuf + sizeof(DE_HEADER) + de_hdr->offset, (char*)&buf0[0], buf0.size());

				pBF->Encrypt(tempbuf + sizeof(DE_HEADER), lengthActual * sizeof(uchar) + de_hdr->offset);

				file2.write((char*)tempbuf, buf0.size() * sizeof(uchar) + sizeof(DE_HEADER) + de_hdr->offset);
				file2.close();

				free(tempbuf);
			}
		}
		else
		{
			file2.write((char*)&buf0[0], buf0.size() * sizeof(uchar));
			file2.close();
		}
		return 0;
	}

	return 1;
}

int getMatAndLabels(int ty, const char* enckey, const char* filelist_txt, vector<TRAIN_DTO>& lstTrain)
{
	vector<string> ids;
	vector<wstring> files;
	vector<string> v1 = tokenize(filelist_txt, "\r\n", true, "");

	int len = (int)v1.size();
	for (int i = 0; i < len; i++)
	{
		vector<string> v2 = tokenize(v1[i], "|", true, "");
		if ((int)v2.size() == 2)
		{
			ids.push_back(v2[0]);
			files.push_back(_u2w(v2[1]));
		}
	}

	for (int j = 0; j < files.size(); j++)
	{
		wchar_t src[MAX_PATH] = { 0 };
		DWORD dwSize = GetFullPathName(files[j].c_str(), MAX_PATH, src, NULL);

		ifstream file(src, ios::in | ios::binary | ios::ate);
		if (!file.is_open())
		{
			return -1;
		}

		streampos size = file.tellg();
		file.seekg(0, ios::beg);
		string buffer(size, 0);
		file.read(&buffer[0], size);
		file.close();
		if ((int)size == 0)
		{
			return 2;
		}
		vector<uchar> vec_data;

		if (strlen(enckey) == 0)
		{
			vec_data = vector<uchar>(&buffer[0], &buffer[0] + size);
		}
		else
		{
			DWORD lengthActual = (DWORD)size;

			DE_HEADER* de_hdr = (DE_HEADER*)buffer.c_str();
			int offset = 8 + de_hdr->offset;

			pBF->Decrypt((void*)(buffer.c_str() + 8), lengthActual - 8);

			vec_data = vector<uchar>(&buffer[offset], &buffer[offset] + lengthActual - offset);
		}

		Mat frame = imdecode(vec_data, IMREAD_COLOR);
		if (!frame.empty())
		{
			if (ty == 1)
			{
				NewSize(frame);

				Mat gray;
				cvtColor(frame, gray, CV_BGR2GRAY);

				Size minSize(100, 100);
				Size maxSize(800, 800);
				Size eyeSize(30, 30);

				double scaleFactor = 1.1;
				int minNeighbors = 5;

				int label = atoi(ids[j].c_str());
				vector<Rect> faces;
				vector<Rect> eyes1;
				vector<Rect> eyes2;
				cascade_face.detectMultiScale(gray, faces, scaleFactor, 5, CASCADE_DO_CANNY_PRUNING, minSize, maxSize);

				vector<TRAIN_DTO> lstDto;

				int len0 = (int)faces.size();
				if (len0 < 9)
				{
					for (int i = 0; i < len0; i++)
					{
						Mat dst2;
						Rect rect = faces[i];
						resize(gray(rect), dst2, Size(128, 128), 0, 0, INTER_LINEAR);
						//cv::Mat mlap;
						////cv::Laplacian(dst2, mlap, CV_16U);
						//cv::Sobel(dst2, mlap, CV_16U, 1, 1);
						//double meanvalue = cv::mean(mlap)[0];
						//if (meanvalue > 2)
						{
							rect.height = rect.height * 2 / 3;
							//equalizeHist(dst2, dst2);

							classifier_eye1.detectMultiScale(dst2, eyes1, scaleFactor, minNeighbors, CASCADE_DO_CANNY_PRUNING, eyeSize);
							if (eyes1.empty())
							{
								classifier_eye2.detectMultiScale(dst2, eyes2, scaleFactor, minNeighbors, CASCADE_DO_CANNY_PRUNING, eyeSize);
							}

							if (!eyes1.empty() || !eyes2.empty())
							{
								rect = faces[i];
								TRAIN_DTO dto;
								Mat dst;
								resize(frame(rect), dst, Size(128, 128), 0, 0, INTER_LINEAR);
								dto.ori = dst;

								dto.gray = dst2;
								dto.image_id = ids[j];
								dto.label = Int2Str(label * 10 + i);
								dto.al = "1";
								lstDto.push_back(dto);
							}
						}
					}
				}

				len0 = (int)lstDto.size();
				vector<_TRAIN_DTO>::iterator it;

				if (len0 == 1)
				{
					for (it = lstDto.begin(); it != lstDto.end(); it++)
					{
						lstTrain.push_back(*it);
					}
				}
				else
				{
					for (it = lstDto.begin(); it != lstDto.end(); it++)
					{
						string name = format("%s.jpg", it->label.c_str());
						wstring szDst2 = _root + _alb + L"\\_train\\_temp\\" + _u2w(name);

						int r = saveface(enckey, it->ori, szDst2);
					}
				}
			}
			else
			{
				Mat gray;
				cvtColor(frame, gray, CV_BGR2GRAY);

				TRAIN_DTO dto;
				dto.ori = frame.clone();
				dto.gray = gray.clone();
				dto.label = ids[j];
				dto.image_id = dto.label.substr(0, dto.label.length() - 1);
				dto.al = Int2Str((int)files.size());

				lstTrain.push_back(dto);
			}
		}
	}

	return 0;
}

PHOTOL_API int predict(int ty, const char* enckey, const char* trainlist_txt, char** mlabels)
{
	string ret = "";
	if (_root == L"")
	{
		if (strlen(enckey) == 0)
		{
			_alb = L"\\data1";
			_root = get_appdata_path("public");
		}
		else
		{
			_root = get_appdata_path("private");
		}
		_model_file = _w2u(_root + _alb + L"\\_train\\_model\\face_model.yml");

		wstring path = get_module_path0(NULL);

		_cascade_file = path + L"\\model\\haarcascade_frontalface_alt2.yml";
		_eye1_file = path + L"\\model\\haarcascade_lefteye_2splits.yml";
		_eye2_file = path + L"\\model\\haarcascade_righteye_2splits.yml";

		cascade_face = CascadeClassifier(_w2u(_cascade_file));
		classifier_eye1 = CascadeClassifier(_w2u(_eye1_file));
		classifier_eye2 = CascadeClassifier(_w2u(_eye2_file));
	}

	if (cascade_face.empty() || classifier_eye1.empty() || classifier_eye2.empty())
	{
		return 1;
	}

	vector<TRAIN_DTO> lstTrain;
	getMatAndLabels(ty, enckey, trainlist_txt, lstTrain);
	if (lstTrain.empty())
	{
		return 0;
	}

	int start = 0;
	bool b = false;
	if (_access(_model_file.c_str(), _A_NORMAL) == -1)
	{
		_model = LBPHFaceRecognizer::create();

		TRAIN_DTO dto = lstTrain[0];
		string name = format("%s.jpg", dto.label.c_str());
		wstring szDst2 = _root + _alb + L"\\_train\\_face\\" + _u2w(name);

		int r = saveface(enckey, dto.ori, szDst2);
		if (r == 0)
		{
			vector<Mat> mats2;
			vector<int> labels2;
			mats2.push_back(dto.gray);
			labels2.push_back(atoi(dto.label.c_str()));

			b = true;
			_model->train(mats2, labels2);

			ret += dto.image_id + "|" + dto.image_id + "|" + name + "|" + dto.al + "|997\r\n";
		}

		start = 1;
	}
	else
	{
		if (_model == nullptr)
		{
			try {
				_model = Algorithm::load<LBPHFaceRecognizer>(_model_file);
			}
			catch (...) {
			}
		}

		if (_model == nullptr || _model->empty())
		{
			return 1;
		}
	}
	vector<Mat> mats0;
	vector<int> labels0;

	double dence = 76;
	if (ty == 1)
	{
		dence = 76;
	}

	for (int i = start; i < lstTrain.size(); i++)
	{
		string image_id = lstTrain[i].image_id;

		int label = -1;
		double confidence = 0;
		_model->predict(lstTrain[i].gray, label, confidence);

		if (confidence < dence)
		{
			string name = format("%d.jpg", label);
			ret += image_id + "|" + Int2Str(label) + "|" + name + "|" + lstTrain[i].al + "|" + Int2Str(confidence) + "\r\n";
		}
		else
		{
			bool b0 = true;
			if (ty == 0 && lstTrain.size() > 4)
			{
				b0 = false;
			}
			if (b0)
			{
				string label0 = lstTrain[i].label;
				label = atoi(label0.c_str());

				string name = format("%s.jpg", label0.c_str());
				wstring szDst2 = _root + _alb + L"\\_train\\_face\\" + _u2w(name);

				int r = saveface(enckey, lstTrain[i].ori, szDst2);
				if (r == 0)
				{
					if (ty == 1)
					{
						vector<Mat> mats2;
						vector<int> labels2;
						mats2.push_back(lstTrain[i].gray);
						labels2.push_back(label);
						_model->update(mats2, labels2);
					}
					else
					{
						mats0.push_back(lstTrain[i].gray);
						labels0.push_back(label);
					}
					b = true;

					ret += image_id + "|" + image_id + "|" + name + "|" + lstTrain[i].al + "|997\r\n";
				}
			}
		}
	}

	if (!mats0.empty())
	{
		_model->update(mats0, labels0);
	}

	if (b)
	{
		_model->save(_model_file);
	}

	int len2 = (int)ret.size() + 1;
	*mlabels = (char*)LocalAlloc(LMEM_ZEROINIT, len2);
	strcpy_s(*mlabels, len2, ret.c_str());

	return 0;
}
