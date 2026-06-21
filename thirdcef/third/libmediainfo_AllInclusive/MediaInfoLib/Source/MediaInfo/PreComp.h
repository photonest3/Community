/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */
 

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// Helpers for compilers (precompilation)
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef MediaInfo_PreCompH
#define MediaInfo_PreCompH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#if defined(_MSC_VER) || defined(__BORLANDC__)
 //#include "MediaInfo/Setup.h"
 //#include "MediaInfo/File__Analyze.h"
#endif //_MSC_VER
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

#define MEDIAINFO_MINIMIZESIZE
//#define MEDIAINFO_MINIMAL_NO


#define MEDIAINFO_MINIMAL_YES

// Readers

#define MEDIAINFO_DIRECTORY_NO
//#define MEDIAINFO_FILE_NO
#define MEDIAINFO_LIBCURL_NO

#define MEDIAINFO_FILE_YES

//---------------------------------------------------------------------------
// Export
#define MEDIAINFO_EXPORT_NO
/*
#define MEDIAINFO_TEXT_NO
#define MEDIAINFO_HTML_NO
#define MEDIAINFO_XML_NO
#define MEDIAINFO_JSON_NO
#define MEDIAINFO_CSV_NO
#define MEDIAINFO_CUSTOM_NO
#define MEDIAINFO_EBUCORE_NO
#define MEDIAINFO_FIMS_NO
#define MEDIAINFO_MPEG7_NO
#define MEDIAINFO_PBCORE_NO
#define MEDIAINFO_REVTMD_NO
#define MEDIAINFO_NISO_NO
*/
#define MEDIAINFO_XML_YES

//

// Multiple
#define MEDIAINFO_MULTI_NO
/*
#define MEDIAINFO_DCP_NO
#define MEDIAINFO_MXF_NO
#define MEDIAINFO_DVDV_NO
#define MEDIAINFO_OGG_NO
#define MEDIAINFO_RIFF_NO
#define MEDIAINFO_WM_NO
#define MEDIAINFO_MK_NO
#define MEDIAINFO_CDXA_NO
#define MEDIAINFO_FLV_NO
#define MEDIAINFO_GXF_NO
#define MEDIAINFO_HDSF4M_NO
#define MEDIAINFO_BDAV_NO
#define MEDIAINFO_BDMV_NO
#define MEDIAINFO_HLS_NO
#define MEDIAINFO_ISM_NO
#define MEDIAINFO_IVF_NO
#define MEDIAINFO_IBI_NO
#define MEDIAINFO_LXF_NO
#define MEDIAINFO_MIXML_NO
#define MEDIAINFO_MPEG4_NO
#define MEDIAINFO_MPEGPS_NO
#define MEDIAINFO_MPEGTS_NO
#define MEDIAINFO_MPEGTS_PCR_NO
#define MEDIAINFO_MPEGTS_PESTIMESTAMP_NO
#define MEDIAINFO_MPEGTS_DUPLICATE_NO
#define MEDIAINFO_NUT_NO
#define MEDIAINFO_P2_NO
#define MEDIAINFO_PMP_NO
#define MEDIAINFO_PTX_NO
#define MEDIAINFO_RM_NO
#define MEDIAINFO_SEQUENCEINFO_NO
#define MEDIAINFO_SKM_NO
#define MEDIAINFO_SWF_NO
#define MEDIAINFO_TSP_NO
#define MEDIAINFO_UMF_NO
#define MEDIAINFO_VBI_NO
#define MEDIAINFO_WTV_NO
#define MEDIAINFO_XDCAM_NO
#define MEDIAINFO_DPG_NO


//------------
#define MEDIAINFO_REFERENCES_NO
#define MEDIAINFO_AAF_NO
#define MEDIAINFO_ANCILLARY_NO
#define MEDIAINFO_DASHMPD_NO
#define MEDIAINFO_DVDIF_NO
#define MEDIAINFO_DVDIF_ANALYZE_NO
#define MEDIAINFO_DXW_NO
*/


#define MEDIAINFO_OGG_YES
#define MEDIAINFO_RIFF_YES
#define MEDIAINFO_MK_YES
#define MEDIAINFO_IVF_YES
#define MEDIAINFO_MPEG4_YES

#define MEDIAINFO_MPEGPS_YES
#define MEDIAINFO_MPEGTS_YES
#define MEDIAINFO_MPEGTS_PCR_YES
#define MEDIAINFO_MPEGTS_PESTIMESTAMP_YES
#define MEDIAINFO_MPEGTS_DUPLICATE_YES



// Video
#define MEDIAINFO_VIDEO_NO


/*
#define MEDIAINFO_MPEGV_NO
#define MEDIAINFO_FFV1_NO
#define MEDIAINFO_MPEG4V_NO
//-------------
#define MEDIAINFO_AIC_NO
#define MEDIAINFO_AFDBARDATA_NO
#define MEDIAINFO_AV1_NO
#define MEDIAINFO_AVC_NO
#define MEDIAINFO_AVSV_NO
#define MEDIAINFO_CANOPUS_NO
#define MEDIAINFO_DIRAC_NO
#define MEDIAINFO_FLIC_NO
#define MEDIAINFO_FRAPS_NO
#define MEDIAINFO_H263_NO
#define MEDIAINFO_HEVC_NO
#define MEDIAINFO_HUFFYUV_NO
#define MEDIAINFO_LAGARITH_NO
#define MEDIAINFO_PRORES_NO
#define MEDIAINFO_VC1_NO
#define MEDIAINFO_VC3_NO
#define MEDIAINFO_TIMECODE_NO
#define MEDIAINFO_THEORA_NO
#define MEDIAINFO_VP8_NO
#define MEDIAINFO_Y4M_NO*/

#define MEDIAINFO_MPEGV_YES
#define MEDIAINFO_FFV1_YES
#define MEDIAINFO_MPEG4V_YES

#define MEDIAINFO_AVC_YES
#define MEDIAINFO_VP8_YES





//---------------------------------------------------------------------------
// Audio
#define MEDIAINFO_AUDIO_NO
/*
#define MEDIAINFO_DTS_NO
#define MEDIAINFO_AAC_NO
#define MEDIAINFO_APE_NO
#define MEDIAINFO_FLAC_NO
#define MEDIAINFO_WVPK_NO

#define MEDIAINFO_AC3_NO
#define MEDIAINFO_DOLBYE_NO
#define MEDIAINFO_SMPTEST0302_NO
#define MEDIAINFO_SMPTEST0331_NO
#define MEDIAINFO_SMPTEST0337_NO

//---------------
#define MEDIAINFO_ADPCM_NO
#define MEDIAINFO_ALS_NO
#define MEDIAINFO_LATM_NO
#define MEDIAINFO_AMR_NO
#define MEDIAINFO_AMV_NO
#define MEDIAINFO_AU_NO
#define MEDIAINFO_CAF_NO
#define MEDIAINFO_CELT_NO
#define MEDIAINFO_DSF_NO
#define MEDIAINFO_DSDIFF_NO
#define MEDIAINFO_IT_NO
#define MEDIAINFO_LA_NO
#define MEDIAINFO_MIDI_NO
#define MEDIAINFO_MOD_NO
#define MEDIAINFO_MPC_NO
#define MEDIAINFO_MPCSV8_NO
#define MEDIAINFO_MPEGA_NO
#define MEDIAINFO_OPENMG_NO
#define MEDIAINFO_OPUS_NO
#define MEDIAINFO_PCM_NO
#define MEDIAINFO_PCMM2TS_NO
#define MEDIAINFO_PCMVOB_NO
#define MEDIAINFO_PS2A_NO
#define MEDIAINFO_RKAU_NO
#define MEDIAINFO_S3M_NO
#define MEDIAINFO_SPEEX_NO
#define MEDIAINFO_TAK_NO
#define MEDIAINFO_TTA_NO
#define MEDIAINFO_TWINVQ_NO
#define MEDIAINFO_VORBIS_NO
#define MEDIAINFO_XM_NO
*/
#define MEDIAINFO_MPEGA_YES
#define MEDIAINFO_VORBIS_YES


//---------------------------------------------------------------------------
// Text
#define MEDIAINFO_TEXT_NO
/*
#define MEDIAINFO_ARIBSTDB24B37_NO
#define MEDIAINFO_CDP_NO
#define MEDIAINFO_CMML_NO
#define MEDIAINFO_DVBSUBTITLE_NO
#define MEDIAINFO_DTVCCTRANSPORT_NO
#define MEDIAINFO_EIA608_NO
#define MEDIAINFO_EIA708_NO
#define MEDIAINFO_N19_NO
#define MEDIAINFO_KATE_NO
#define MEDIAINFO_PDF_NO
#define MEDIAINFO_PGS_NO
#define MEDIAINFO_SCC_NO
#define MEDIAINFO_SCTE20_NO
#define MEDIAINFO_SDP_NO
#define MEDIAINFO_SUBRIP_NO
#define MEDIAINFO_TELETEXT_NO
#define MEDIAINFO_TIMEDTEXT_NO
#define MEDIAINFO_TTML_NO
#define MEDIAINFO_OTHERTEXT_NO
*/

//---------------------------------------------------------------------------
// Image
#define MEDIAINFO_IMAGE_NO
/*
#define MEDIAINFO_ARRIRAW_NO
#define MEDIAINFO_BMP_NO
#define MEDIAINFO_BPG_NO
#define MEDIAINFO_DDS_NO
#define MEDIAINFO_DPX_NO
#define MEDIAINFO_EXR_NO
#define MEDIAINFO_GIF_NO
#define MEDIAINFO_ICO_NO
#define MEDIAINFO_JPEG_NO
#define MEDIAINFO_PCX_NO
#define MEDIAINFO_PNG_NO
#define MEDIAINFO_PSD_NO
#define MEDIAINFO_RLE_NO
#define MEDIAINFO_TIFF_NO
#define MEDIAINFO_TGA_NO
*/

//---------------------------------------------------------------------------
// Archive
#define MEDIAINFO_ARCHIVE_NO
/*
#define MEDIAINFO_7Z_NO
#define MEDIAINFO_ACE_NO
#define MEDIAINFO_BZIP2_NO
#define MEDIAINFO_ELF_NO
#define MEDIAINFO_GZIP_NO
#define MEDIAINFO_ISO9660_NO
#define MEDIAINFO_MZ_NO
#define MEDIAINFO_RAR_NO
#define MEDIAINFO_TAR_NO
#define MEDIAINFO_ZIP_NO
*/
#define MEDIAINFO_ADVANCED2_NO

//---------------------------------------------------------------------------
// Tag
#define MEDIAINFO_TAG_NO
/*
#define MEDIAINFO_APETAG_NO
#define MEDIAINFO_ID3_NO
#define MEDIAINFO_ID3V2_NO
#define MEDIAINFO_LYRICS3_NO
#define MEDIAINFO_LYRICS3V2_NO
#define MEDIAINFO_PROPERTYLIST_NO
#define MEDIAINFO_VORBISCOM_NO
*/
#define MEDIAINFO_APETAG_YES
#define MEDIAINFO_ID3_YES
#define MEDIAINFO_ID3V2_YES

//---------------------------------------------------------------------------
// Other
#define MEDIAINFO_DUMMY_NO
#define MEDIAINFO_OTHER_NO
//#define MEDIAINFO_UNKNOWN_NO
#define MEDIAINFO_JNI_NO





#endif
