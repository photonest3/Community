#pragma once

#include "stmdef.h"
#include "../../FreeImage/Source/FreeImage.h"



#define BPP_TYPE_4  4
#define BPP_TYPE_8  8
#define BPP_TYPE_16 16
#define BPP_TYPE_24 24
#define BPP_TYPE_32 32


#define SCALE_FIX_PRE 100


#define BPP_8_BYTE_NUM  1
#define BPP_16_BYTE_NUM 2
#define BPP_24_BYTE_NUM 3
#define BPP_32_BYTE_NUM 4


// define copy image callback
typedef void(*copyback)(const BYTE* in, BYTE* out, int* offset);
// define bilinear copy image callback
typedef void(*bilinearcopyback)(const BYTE* pa, const BYTE* pb, const BYTE* pc, const BYTE* pd,
	double pla, double plb, double plc, double pld, BYTE* out);


class CPngProc
{
public:
	CPngProc(void);
	~CPngProc(void);

public:
	static int CPngProc::FreeImage_GetBppByteNum(int bpp);

	static void FreeImage_Copy8Bit(const BYTE* in, BYTE* out);
	static void FreeImage_Copy16Bit(const BYTE* in, BYTE* out);
	static void FreeImage_Copy24Bit(const BYTE* in, BYTE* out);
	static void FreeImage_Copy32Bit(const BYTE* in, BYTE* out);
	static void FreeImage_CopyBit(const BYTE* src, int srcBpp, BYTE* dst, int dstBpp, int paste);
	static void FreeImage_AlphaCopyBit(const BYTE** pst, int pstBpp, BYTE** dst, int dstBpp);
	static void FreeImage_ZoomCopy8Bit(const BYTE* in, BYTE* out, int* offset);
	static void FreeImage_ZoomCopy16Bit(const BYTE* in, BYTE* out, int* offset);
	static void FreeImage_ZoomCopy24Bit(const BYTE* in, BYTE* out, int* offset);
	static void FreeImage_ZoomCopy32Bit(const BYTE* in, BYTE* out, int* offset);
	static int FreeImage_ZoomCopyScale(int oldWidth, int oldHeight,
		int width, int height, int* wScale, int* wScaleFix, int* hScale, int* hScaleFix);
	static int FreeImage_ZoomGetIndex(int index,
		int scale, int scaleFix, int* pos, int* posFix, int srcLength, int length);
	static int FreeImage_ZoomCopy(FIBITMAP* in, int width, int height, FIBITMAP* out, copyback copy);
	static void FreeImage_BillinearCopy32Bit(const BYTE* pa, const BYTE* pb, const BYTE* pc, const BYTE* pd,
		const double pla, const double plb, const double plc, const double pld, BYTE* out);
	static int FreeImage_Bilinear_ZoomCopy(FIBITMAP* in, int width, int height, FIBITMAP* out, bilinearcopyback copy);
	FIBITMAP* FreeImage_New8BppImage(FIBITMAP* dib, int width, int height);
	FIBITMAP* FreeImage_New16BppImage(FIBITMAP* dib, int width, int height);
	FIBITMAP* FreeImage_New24BppImage(FIBITMAP* dib, int width, int height);
	FIBITMAP* FreeImage_New32BppImage(FIBITMAP* dib, int width, int height, int bilinear);
	int FreeImage_AlphaPaste(FIBITMAP* dib, FIBITMAP* pst, int pitx, int pity);
	int main0();

};
