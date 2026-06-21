
///////////////////////////////////////////////////////////////////////
/// @file pngProc.cpp
/// @brief PNG 图像处理实现文件
/// @details 实现 PNG 图片的位深度转换、像素格式处理等功能
/// @author PhotoNest Team
/// @date 2024
/// @version 1.0
/// @note 本文件负责 PNG 格式图片的低层处理
///////////////////////////////////////////////////////////////////////

#include "StdAfx.h"

#include <stdio.h>
#include "pngProc.h"

CPngProc::CPngProc(void) {

}
CPngProc::~CPngProc(void) {

}

/**
* get diffrent bpp occupy data lenth
* @param:[in]      int bpp, bpp type (8, 16, 24, 32)
* @return: data lenth
*/
int CPngProc::FreeImage_GetBppByteNum(int bpp)
{
	switch (bpp)
	{
	case BPP_TYPE_8:
		return BPP_8_BYTE_NUM;
	case BPP_TYPE_16:
		return BPP_16_BYTE_NUM;
	case BPP_TYPE_24:
		return BPP_24_BYTE_NUM;
	case BPP_TYPE_32:
	default:
		break;
	}


	return BPP_32_BYTE_NUM;
}


/**
* copy between two 8 bpp image bit
* @param:[in]      const BYTE* in, in image bits handle
* @param:[in/out]  BYTE* out, out image bits handle
* @return: void
*/
void CPngProc::FreeImage_Copy8Bit(const BYTE* in, BYTE* out)
{
}


/**
* copy between two 16 bpp image bit
* @param:[in]      const BYTE* in, in image bits handle
* @param:[in/out]  BYTE* out, out image bits handle
* @return: void
*/
void CPngProc::FreeImage_Copy16Bit(const BYTE* in, BYTE* out)
{
}


/**
* copy between two 24 bpp image bit
* @param:[in]      const BYTE* in, in image bits handle
* @param:[in/out]  BYTE* out, out image bits handle
* @return: void
*/
void CPngProc::FreeImage_Copy24Bit(const BYTE* in, BYTE* out)
{
	out[FI_RGBA_RED] = in[FI_RGBA_RED];
	out[FI_RGBA_GREEN] = in[FI_RGBA_GREEN];
	out[FI_RGBA_BLUE] = in[FI_RGBA_BLUE];
}


/**
* copy between two 32 bpp image bit
* @param:[in]      const BYTE* in, in image bits handle
* @param:[in/out]  BYTE* out, out image bits handle
* @return: void
*/
void CPngProc::FreeImage_Copy32Bit(const BYTE* in, BYTE* out)
{
	out[FI_RGBA_RED] = in[FI_RGBA_RED];
	out[FI_RGBA_GREEN] = in[FI_RGBA_GREEN];
	out[FI_RGBA_BLUE] = in[FI_RGBA_BLUE];
	out[FI_RGBA_ALPHA] = in[FI_RGBA_ALPHA];
}


/**
* copy source image to target image
* @param:[in]      const BYTE* src, source image bits handle
* @param:[in]      int srcBpp, source image bit offset
* @param:[in/out]  BYTE* dst, target image bits handle
* @param:[in]      int dstBpp, target image bit offset
* @param:[in]      int paste, paste action or zoom action flag
* @return: void
*/
void CPngProc::FreeImage_CopyBit(const BYTE* src, int srcBpp, BYTE* dst, int dstBpp, int paste)
{
	int bpp = srcBpp > dstBpp ? srcBpp : dstBpp;
	switch (bpp)
	{
	case BPP_TYPE_8:
		if (srcBpp == dstBpp)
		{
			FreeImage_Copy8Bit(src, dst);
		}
		else if (srcBpp == BPP_TYPE_8)
		{
			// out change
		}
		else
		{
			// 
			if (srcBpp == BPP_TYPE_32)
			{
			}
			else if (srcBpp == BPP_TYPE_24)
			{
			}
			else // inBpp == BPP_TYPE_16
			{
			}
		}
		break;
	case BPP_TYPE_16:
		if (srcBpp == dstBpp)
		{
			FreeImage_Copy16Bit(src, dst);
		}
		else if (srcBpp == BPP_TYPE_16)
		{
			// out change
		}
		else
		{
			// 
			if (srcBpp == BPP_TYPE_32)
			{
			}
			else // inBpp == BPP_TYPE_24
			{
			}
		}
		break;
	case BPP_TYPE_24:
		FreeImage_Copy24Bit(src, dst);
		break;
	case BPP_TYPE_32:
		if (paste == 0)
		{
			FreeImage_Copy32Bit(src, dst);
		}
		else
		{
			if (src[FI_RGBA_ALPHA] > 100)
			{
				FreeImage_Copy24Bit(src, dst);
			}
		}
		break;
	default:
		break;
	}
}


/**
* copy paset image to base image
* @param:[in]      const BYTE** pst, paste image bits handle
* @param:[in]      int pstBpp, paste image bit offset
* @param:[in/out]  BYTE** dst, base image bits handle
* @param:[in]      int dstBpp, base image bit offset
* @return: void
*/
void CPngProc::FreeImage_AlphaCopyBit(const BYTE** pst, int pstBpp, BYTE** dst, int dstBpp)
{
	FreeImage_CopyBit(*pst, pstBpp, *dst, dstBpp, 1);
	*dst += FreeImage_GetBppByteNum(dstBpp);// base image bits increase self
	*pst += FreeImage_GetBppByteNum(pstBpp);// paste image bits increase self
}


/**
* copy 8 bpp bit to zoom in or out image from base image
* @param:[in]      const BYTE* in, base image bits handle
* @param:[in/out]  BYTE* out, new image bits handle
* @param:[out]     int* offset, image bit offset (8bpp => 1)
* @return: void
*/
void CPngProc::FreeImage_ZoomCopy8Bit(const BYTE* in, BYTE* out, int* offset)
{
	FreeImage_CopyBit(in, BPP_TYPE_8, out, BPP_TYPE_8, 0);
	*offset = BPP_8_BYTE_NUM;
}


/**
* copy 16 bpp bit to zoom in or out image from base image
* @param:[in]      const BYTE* in, base image bits handle
* @param:[in/out]  BYTE* out, new image bits handle
* @param:[out]     int* offset, image bit offset (16bpp => 2)
* @return: void
*/
void CPngProc::FreeImage_ZoomCopy16Bit(const BYTE* in, BYTE* out, int* offset)
{
	FreeImage_CopyBit(in, BPP_TYPE_16, out, BPP_TYPE_16, 0);
	*offset = BPP_16_BYTE_NUM;
}


/**
* copy 24 bpp bit to zoom in or out image from base image
* @param:[in]      const BYTE* in, base image bits handle
* @param:[in/out]  BYTE* out, new image bits handle
* @param:[out]     int* offset, image bit offset (24bpp => 3)
* @return: void
*/
void CPngProc::FreeImage_ZoomCopy24Bit(const BYTE* in, BYTE* out, int* offset)
{
	FreeImage_CopyBit(in, BPP_TYPE_24, out, BPP_TYPE_24, 0);
	*offset = BPP_24_BYTE_NUM;
}


/**
* copy 32 bpp bit to zoom in or out image from base image
* @param:[in]      const BYTE* in, base image bits handle
* @param:[in/out]  BYTE* out, new image bits handle
* @param:[out]     int* offset, image bit offset (32bpp => 4)
* @return: void
*/
void CPngProc::FreeImage_ZoomCopy32Bit(const BYTE* in, BYTE* out, int* offset)
{
	FreeImage_CopyBit(in, BPP_TYPE_32, out, BPP_TYPE_32, 0);
	*offset = BPP_32_BYTE_NUM;
}




/**
* get the new image with base image scale, with width scale and height scale
* @param:[in]  int oldWidth, base image width
* @param:[in]  int oldHeight, base image height
* @param:[in]  int width, new image width
* @param:[in]  int height, new image height
* @param:[out] int* wScale, the new image with base image width scale
* @param:[out] int* wScaleFix, the new image with base image width scale fixed
* @param:[out] int* hScale, the new image with base image height scale
* @param:[out] int* hScaleFix, the new image with base image height scale fixed
* @return:  0 => success or -1 => fail
*/
int CPngProc::FreeImage_ZoomCopyScale(int oldWidth, int oldHeight,
	int width, int height, int* wScale, int* wScaleFix, int* hScale, int* hScaleFix)
{
	if (width == 0 || height == 0)
	{
		return -1;
	}


	int max = 0;
	int min = 0;


	max = oldWidth > width ? oldWidth : width;
	min = oldWidth > width ? width : oldWidth;
	double w = (double)max / (double)min;


	max = oldHeight > height ? oldHeight : height;
	min = oldHeight > height ? height : oldHeight;
	double h = (double)max / (double)min;


	int wInt = (int)w;
	int hInt = (int)h;


	int wFix = (w * SCALE_FIX_PRE) - (wInt * SCALE_FIX_PRE);
	int hFix = (h * SCALE_FIX_PRE) - (hInt * SCALE_FIX_PRE);


	int i = 0;
	for (i = 0; (wFix != 0) && (i < SCALE_FIX_PRE); i++)
	{
		if (i*wFix >(SCALE_FIX_PRE - (wFix / 2)))
		{
			break;
		}
	}
	*wScaleFix = i;
	*wScale = wInt;


	for (i = 0; (hFix != 0) && (i < SCALE_FIX_PRE); i++)
	{
		if (i*hFix >(SCALE_FIX_PRE - (hFix / 2)))
		{
			break;
		}
	}
	*hScaleFix = i;
	*hScale = hInt;


	return 0;
}


/**
* get the base image bits location when zoom in or out
* @param:[in]  int index, now the base image bits location when zoom in or out
* @param:[in]  int scale, zoom in or out scale
* @param:[in]  int scaleFix, fix zoom in or out scale
* @param:[out] int* pos, new image location fixed
* @param:[out] int* posFix, pos fixed
* @param:[in]  int srcLength, source image width or height length
* @param:[in]  int length, target image width or height length
* @return: the base image bits location when zoom in or out
*/
int CPngProc::FreeImage_ZoomGetIndex(int index,
	int scale, int scaleFix, int* pos, int* posFix, int srcLength, int length)
{
	int lpos = *pos, fpos = *posFix;

	if (srcLength - length < 0) // zoom out
	{
		index = lpos / scale;
		if (fpos != index && scaleFix != 0 && (lpos % scaleFix) == 0)
		{
			fpos = index;
			index--;
			lpos--;
		}
	}
	else if (srcLength - length > 0) // zoom in
	{
		index = lpos * scale;
		if (index != 0 && scaleFix != 0 && (lpos % scaleFix) == 0)
		{
			fpos++;
		}
		index += fpos;
	}
	else // equal
	{
		index = lpos / scale;
	}


	*pos = lpos;
	*posFix = fpos;


	if (index > srcLength)
	{
		index = srcLength;
	}


	return index;
}


/**
* zoom in or out image from base image
* @param:[in]  FIBITMAP* in, base image handle
* @param:[in]  int width, new image width
* @param:[in]  int height, new image height
* @param:[out] FIBITMAP* out, new image handle
* @param:[in]  copyback copy, callback function, copy base image bits to new empty image
* @return: 0 => success or -1 => fail
*/
int CPngProc::FreeImage_ZoomCopy(FIBITMAP* in, int width, int height, FIBITMAP* out, copyback copy)
{
	int offset = 0;
	int xPos = 0, xPosFix = 0, yPos = 0, yPosFix = 0;
	BYTE* line = NULL, *tmp = NULL;
	BYTE* bits = FreeImage_GetBits(out);
	if (!bits)
	{
		return -1;
	}


	int x = 0, y = 0, wIndex = 0, hIndex = 0, wScale = 0, hScale = 0, wScaleFix = 0, hScaleFix = 0;
	int inWidth = FreeImage_GetWidth(in);
	int inHeight = FreeImage_GetHeight(in);


	FreeImage_ZoomCopyScale(inWidth, inHeight, width, height,
		&wScale, &wScaleFix, &hScale, &hScaleFix);// calculate zoom scale
	for (y = 0, yPos = y, yPosFix = 0; y<height; y++, yPos++)
	{
		hIndex = FreeImage_ZoomGetIndex(hIndex, hScale, hScaleFix,
			&yPos, &yPosFix, inHeight, height);// get the base image bits y direction location
		line = FreeImage_GetScanLine(in, hIndex);// get in image line bits
		for (x = 0, wIndex = x - 1, xPos = x, xPosFix = 0; x<width; x++, xPos++)
		{
			wIndex = FreeImage_ZoomGetIndex(wIndex, wScale, wScaleFix,
				&xPos, &xPosFix, inWidth, width);// get the base image bits x direction location
			tmp = line + (wIndex * offset);// in image bits location fixed
			bits += offset;// out image bits location fixed
			copy(tmp, bits, &offset);// copy in image bit to out image bit by zoom in or out
		}
	}

	return 0;
}


/**
* copy between two 32 bpp image bit
* @param:[in]      const BYTE* pa, pointer a
* @param:[in]      const BYTE* pb, pointer b
* @param:[in]      const BYTE* pc, pointer c
* @param:[in]      const BYTE* pd, pointer d
* @param:[in]      const double pla, polynomial value a
* @param:[in]      const double plb, polynomial value b
* @param:[in]      const double plc, polynomial value c
* @param:[in]      const double pld, polynomial value d
* @param:[in/out]  BYTE* out, out image bits handle
* @return: void
*/
void CPngProc::FreeImage_BillinearCopy32Bit(const BYTE* pa, const BYTE* pb, const BYTE* pc, const BYTE* pd,
	const double pla, const double plb, const double plc, const double pld, BYTE* out)
{

	out[FI_RGBA_RED] = pa[FI_RGBA_RED] * pla + pb[FI_RGBA_RED] * plb
		+ pc[FI_RGBA_RED] * plc + pd[FI_RGBA_RED] * pld;
	out[FI_RGBA_GREEN] = pa[FI_RGBA_GREEN] * pla + pb[FI_RGBA_GREEN] * plb
		+ pc[FI_RGBA_GREEN] * plc + pd[FI_RGBA_GREEN] * pld;
	out[FI_RGBA_BLUE] = pa[FI_RGBA_BLUE] * pla + pb[FI_RGBA_BLUE] * plb
		+ pc[FI_RGBA_BLUE] * plc + pd[FI_RGBA_BLUE] * pld;
	out[FI_RGBA_ALPHA] = pa[FI_RGBA_ALPHA] * pla + pb[FI_RGBA_ALPHA] * plb
		+ pc[FI_RGBA_ALPHA] * plc + pd[FI_RGBA_ALPHA] * pld;
}




// ----------------------------------------------------------------------------------------
//
// D(x, y) = S(j, k)*(1-t)*(1-u) + S(j, k+1)*t*(1-u) + S(j+1,k)*(1-t)*u + S(j+1,k+1)*t*u
//
// a = (1-t)*(1-u)
// b = t*(1-u)
// c = (1-t)*u
// d = t*u
//
// |--.(j,k+1)---|-------------.(j+1,k+1)-
// |             |             |          
// |             .(j+t,k+u)    |          
// |             |             |          
// |--.(j,k)-------------------.(j+1,k)---
//
// j = srcX
// k = srcY
// t = srcX.xx 
// u = srcY.yy
//
//------------------------------------------------------------------------------------------
/**
* Bilinear interpolation Zoom
* @param:[in]  FIBITMAP* in, base image handle
* @param:[in]  int width, new image width
* @param:[in]  int height, new image height
* @param:[out] FIBITMAP* out, new image handle
* @param:[in]  bilinearcopyback copy, callback function, copy base image bits to new empty image
* @return: 0 => success or -1 => fail
*/
int CPngProc::FreeImage_Bilinear_ZoomCopy(FIBITMAP* in, int width, int height, FIBITMAP* out, bilinearcopyback copy)
{
	int row = 0, col = 0;
	BYTE* linef = NULL, *linel = NULL;
	BYTE* pa = NULL, *pb = NULL, *pc = NULL, *pd = NULL;
	BYTE* bits = FreeImage_GetBits(out);
	if (!bits)
	{
		return -1;
	}


	int inWidth = FreeImage_GetWidth(in);
	int inHeight = FreeImage_GetHeight(in);
	int bpp = FreeImage_GetBPP(in);
	int offset = FreeImage_GetBppByteNum(bpp);


	float wScale = (float)inWidth / (float)width;
	float hScale = (float)inHeight / (float)height;


	for (row = 0; row < height; ++row)
	{
		// j and t value
		double inRow = ((float)row)*hScale;
		int k = (int)inRow;
		double u = inRow - k;


		// j and j+1 image
		linef = FreeImage_GetScanLine(in, k);// get in image line bits (k)
		if (k + 1 < inHeight)
		{
			linel = FreeImage_GetScanLine(in, (k + 1));// get in image next line bits (k+1)
		}
		else
		{
			linel = linef;
		}


		// new image line
		for (col = 0; col < width; ++col)
		{
			// k and u value
			double inCol = ((float)col)*wScale;
			int j = (int)inCol;
			double t = inCol - j;

			// polynomial value
			double pla = (1.0 - t) * (1.0 - u);
			double plb = t * (1.0 - u);
			double plc = (1.0 - t) * u;
			double pld = t * u;


			// 4 piont
			pa = linef + j*offset; // S(j, k)
			pb = linel + j*offset; // S(j, k+1)
			pc = (j + 1 < inWidth) ? linef + (j + 1)*offset : pa; // S(j+1, k)
			pd = (j + 1 < inWidth) ? linel + (j + 1)*offset : pb; // S(j+1, k+1)


			copy(pa, pb, pc, pd, pla, plb, plc, pld, bits);
			bits += offset;
		}
	}


	return 0;
}


/**
* create 8 bpp image from base image, can zoom in or zoom out
* @param:[in] FIBITMAP* dib, base image handle
* @param:[in] int width, new image width
* @param:[in] int height, new image height
* @return: new image handle
*/
FIBITMAP* CPngProc::FreeImage_New8BppImage(FIBITMAP* dib, int width, int height)
{
	FIBITMAP* out = FreeImage_Allocate(width, height, FreeImage_GetBPP(dib), 0, 0, 0);
	if (!out)
	{
		return NULL;
	}


	if (FreeImage_ZoomCopy(dib, width, height, out, FreeImage_ZoomCopy8Bit) < 0)
	{
		return NULL;
	}


	return out;
}


/**
* create 16 bpp image from base image, can zoom in or zoom out
* @param:[in] FIBITMAP* dib, base image handle
* @param:[in] int width, new image width
* @param:[in] int height, new image height
* @return: new image handle
*/
FIBITMAP* CPngProc::FreeImage_New16BppImage(FIBITMAP* dib, int width, int height)
{
	FIBITMAP* out = FreeImage_Allocate(width, height, FreeImage_GetBPP(dib), 0, 0, 0);
	if (!out)
	{
		return NULL;
	}


	if (FreeImage_ZoomCopy(dib, width, height, out, FreeImage_ZoomCopy16Bit) < 0)
	{
		return NULL;
	}


	return out;
}


/**
* create 24 bpp image from base image, can zoom in or zoom out
* @param:[in] FIBITMAP* dib, base image handle
* @param:[in] int width, new image width
* @param:[in] int height, new image height
* @return: new image handle
*/
FIBITMAP* CPngProc::FreeImage_New24BppImage(FIBITMAP* dib, int width, int height)
{
	FIBITMAP* out = FreeImage_Allocate(width, height, FreeImage_GetBPP(dib), 0, 0, 0);
	if (!out)
	{
		return NULL;
	}


	if (FreeImage_ZoomCopy(dib, width, height, out, FreeImage_ZoomCopy24Bit) < 0)
	{
		return NULL;
	}


	return out;
}


/**
* create 32 bpp image from base image, can zoom in or zoom out
* @param:[in] FIBITMAP* dib, base image handle
* @param:[in] int width, new image width
* @param:[in] int height, new image height
* @return: new image handle
*/
FIBITMAP* CPngProc::FreeImage_New32BppImage(FIBITMAP* dib, int width, int height, int bilinear)
{
	FIBITMAP* out = FreeImage_Allocate(width, height, FreeImage_GetBPP(dib), 0, 0, 0);
	if (!out)
	{
		return NULL;
	}


	if (bilinear == 0)
	{
		if (FreeImage_ZoomCopy(dib, width, height, out, FreeImage_ZoomCopy32Bit) < 0)
		{
			return NULL;
		}
	}
	else
	{
		if (FreeImage_Bilinear_ZoomCopy(dib, width, height, out, FreeImage_BillinearCopy32Bit) < 0)
		{
			return NULL;
		}
	}


	return out;
}


/**
* paste image to base image, has alpha effect, FreeImage_Paste can't do that
* @param:[in/out] FIBITMAP* dib, base image handle
* @param:[in]     FIBITMAP* pst, paste image width
* @param:[in]     int pitx, pst image on base image x location
* @param:[in]     int pity, pst image on base image y location
* @return: 0 => success or -1 => fail
*/
int CPngProc::FreeImage_AlphaPaste(FIBITMAP* dib, FIBITMAP* pst, int pitx, int pity)
{
	// FreeImage_GetWidth or FreeImage_GetHeight has div bpp
	int dibWidth = FreeImage_GetWidth(dib);
	int dibHeight = FreeImage_GetHeight(dib);
	int dibBpp = FreeImage_GetBPP(dib);
	int offset = FreeImage_GetBppByteNum(dibBpp);

	int pstWidth = FreeImage_GetWidth(pst);
	int pstHeight = FreeImage_GetHeight(pst);
	int pstBpp = FreeImage_GetBPP(pst);

	int x = 0, y = 0;
	BYTE *dibBits = NULL;
	const BYTE *pstBits = NULL;

	for (y = 0; y < dibHeight; y++)
	{
		// y direction, offset of source bitmap and begin get source or paste bitmap
		if (y > pity && (y - pity) < pstHeight)
		{
			dibBits = FreeImage_GetScanLine(dib, y);
			if (!dibBits)
			{
				return -1;
			}
			pstBits = FreeImage_GetScanLine(pst, (y - pity));
			if (!pstBits)
			{
				return -1;
			}
		}
		// x direction, offset of source bitmap and replace by paste bitmap
		for (x = 0; dibBits && pstBits && (x < dibWidth); x++)
		{
			if (x == pitx)
			{
				dibBits += x * offset;
			}
			if (x >= pitx && (x - pitx) < pstWidth)
			{
				FreeImage_AlphaCopyBit(&pstBits, pstBpp, &dibBits, dibBpp);
			}
		}

		dibBits = NULL;
		pstBits = NULL;
	}


	return 0;
}


// unit test
int CPngProc::main0()
{
	FIBITMAP *dib = NULL, *new0 = NULL, *dst = NULL;
	dib = FreeImage_Load(FIF_PNG, "f:\\00\1.png", PNG_DEFAULT);
	if (!dib)
	{
		goto out;
	}


	new0 = FreeImage_New32BppImage(dib, 48, 48, 1);
	if (!new0)
	{
		goto out;
	}


	FreeImage_Save(FIF_JPEG, new0, "f:\\00\1.jpg", 0);


	//dst = FreeImage_Load(FIF_BMP, "f:\\00\1.png", BMP_DEFAULT);
	//if (!dst)
	//{
	//	goto out;
	//}

	//if (FreeImage_AlphaPaste(dst, new0, 101, 50) < 0)
	//{
	//	goto out;
	//}

	//FreeImage_Save(FIF_PNG, dst, "./res/paste.png", 0);
out:
	if (dib)
	{
		FreeImage_Unload(dib);
		dib = NULL;
	}
	if (new0)
	{
		FreeImage_Unload(new0);
		new0 = NULL;
	}
	if (dst)
	{
		FreeImage_Unload(dst);
		dst = NULL;
	}
	return 0;
}