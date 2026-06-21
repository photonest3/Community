// Copyright (c) 2012 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "tests/cefclient/browser/scheme_test.h"
#include "include/cef_browser.h"
#include "include/cef_callback.h"
#include "include/cef_frame.h"
#include "include/cef_request.h"
#include "include/cef_resource_handler.h"
#include "include/cef_response.h"
#include "include/cef_scheme.h"
#include "include/wrapper/cef_helpers.h"
#include "tests/cefclient/browser/test_runner.h"
#include "tests/shared/browser/resource_util.h"
#include "tests/cefclient/resources/win/resource.h"
#include "include/capi/cef_parser_capi.h"

#include <algorithm>
#include "../../scheme/scheme/scheme.h"
#include "timestamp.h"
#include <process.h>
#include "IsoHandler.h"
#include "MimeType.h"
#include "Blowfish.h"


using namespace std;

typedef struct _RANGE
{
	unsigned long start = 0;
	unsigned long end = 0;
	unsigned long length = 0;
	unsigned long total = 0;
	unsigned long rem = 0;

} RANGE;


typedef struct _DE_HEADER
{
	unsigned char ot1;
	unsigned char crc;
	unsigned char offset;
	unsigned char type;
	unsigned short ot2;
	unsigned short ot3;
} DE_HEADER, * PDE_HEADER;

#pragma comment( lib, "../../../PhotoNest/tests/cefclient/Release/bin/scheme.lib" ) 


namespace client {
	namespace scheme_test {

		namespace {
			CBlowfish* _pBF = NULL;
			CMimeType* _pMimeType = NULL;

			// Implementation of the schema handler for client:// requests.
			class ClientSchemeHandler : public CefResourceHandler {
			public:
				ClientSchemeHandler() : offset_(0) {
					_offset = 0;
					_Status = 200;
					_hFile = INVALID_HANDLE_VALUE;

				}
				~ClientSchemeHandler() {
					if (_hFile != INVALID_HANDLE_VALUE)
					{
						CloseHandle(_hFile);
						_hFile = INVALID_HANDLE_VALUE;
						//LOG(ERROR) << "_~ClientSchemeHandler" ;
					}
				}

				bool Open(CefRefPtr<CefRequest> request,
					bool& handle_request,
					CefRefPtr<CefCallback> callback) override {
					DCHECK(!CefCurrentlyOn(TID_UI) && !CefCurrentlyOn(TID_IO));

					// The request will be continued or canceled based on the return value.
					handle_request = true;

					bool handled = false;

					_content = "";
					_filePath = L"";
					_mimeType = "";
					_offset = 0;
					_Status = 200;
					_lengthActual = 0;
					_hFile = INVALID_HANDLE_VALUE;

					_de_hdr.type = 0;
					_de_hdr.offset = 0;
					_de_hdr.crc = 0;
					_de_hdr.ot1 = 0;
					_de_hdr.ot2 = 0;

					_szUploadBody = "";
					_szUploadFile = L"";
					_szUploadUrl = "";
					_cmd = request->GetMethod();

					if (_pBF == NULL)
					{
						if (IsoHandler::Instance().album_ == L"public")
						{
							IsoHandler::Instance().enckey_ = "123456";
						}

						if (IsoHandler::Instance().enckey_ != "")
						{
							_pBF = new CBlowfish();
							_pBF->SetPassword((char*)IsoHandler::Instance().enckey_.c_str());
							IsoHandler::Instance().username_ = "ok";
							//LOG(ERROR) << "_pBF:" << IsoHandler::Instance().enckey_;
						}
					}

					if (request->GetResourceType() == RT_XHR)
					{
						_szUploadUrl = request->GetURL();
						if (_cmd == "POST")
						{
							CefRefPtr<CefPostData> data_array = request->GetPostData();
							if (data_array != nullptr && data_array->GetElementCount() > 0)
							{
								CefPostData::ElementVector elements;
								data_array->GetElements(elements);

								size_t num = elements.size();
								for (size_t i = 0; i < num; i++)
								{
									CefRefPtr<CefPostDataElement> data = elements[i];
									cef_postdataelement_type_t t = data->GetType();
									if (t == PDE_TYPE_BYTES)
									{
										size_t length = data->GetBytesCount();
										std::vector<uint8_t> v;
										v.resize(length);
										data->GetBytes(length, &v[0]);

										string trmp = "";
										_szUploadBody += trmp.assign(v.begin(), v.end());

									}
									else if (t == PDE_TYPE_FILE)
									{
										_szUploadFile = data->GetFile().ToWString();
									}
									//else
									//{
									//	//PDE_TYPE_EMPTY

									//}
								}
							}

						}
						_responseHeadersReadyCallback = callback;

						_beginthreadex(NULL, 0, ProcThumbnailThread, this, 0, 0);

						//string::size_type pos0 = _szUploadUrl.find("index.get_pending");
						//if (pos0 == string::npos)
						//{
						//	LOG(ERROR) << "RT_XHR: " << _szUploadUrl;
						//}


						handle_request = false;
					}
					else
					{
						bool b = document_handler(request);
						if (b)
						{
							_szUploadUrl = request->GetURL();
							_responseHeadersReadyCallback = callback;

							_beginthreadex(NULL, 0, ProcThumbnailThread, this, 0, 0);
							handle_request = false;
						}
						else
						{
							if (_content == "" && _filePath == L"")
							{
								return false;
							}
							callback->Continue();
						}
					}

					return true;
				}

				void GetResponseHeaders(CefRefPtr<CefResponse> response,
					int64& response_length,
					CefString& redirectUrl) override {
					CEF_REQUIRE_IO_THREAD();

					//DCHECK(!data_.empty());

					//response->SetMimeType(mime_type_);
					//response->SetStatus(200);

					//// Set the resulting response length
					//response_length = data_.length();

					_offset = 0;

					if (_content != "")
					{
						_lengthActual = _content.size();
						response_length = _lengthActual;
					}
					else
					{
						if (_ranges.empty())
						{
							_Status = 200;
							response_length = _lengthActual;
						}
						else if (_ranges.size() >= 1)
						{
							RANGE r = _ranges.front();

							char buf[1024] = { 0 };
							sprintf_s(buf, "bytes %ld-%ld/%ld", r.start, r.end, r.total);

							pair<std::string, std::string>p1("Content-Range", buf);
							_header.insert(p1);

							_mimeType = "application/octet-stream";

							response_length = r.length;
							_Status = 206;
						}
						else {
						}
					}

					if (_mimeType != "")
					{
						response->SetMimeType(_mimeType);
					}

					char buf[100] = { 0 };
					sprintf_s(buf, 100, "%u", (unsigned int)response_length);
					pair<std::string, std::string> Connection("Content-Length", buf);

					_header.insert(Connection);
					response->SetHeaderMap(_header);

					string txt = "Ok";
					if (_Status == 206)
					{
						txt = "Partial Content";
					}
					else if (_Status == 304)
					{
						txt = "Not Modified";
					}
					else if (_Status == 404)
					{
						txt = "Not Found";
					}
					else if (_Status == 501)
					{
						txt = "Error: Invalid file extension";
					}
					else if (_Status == 302)
					{
						_lengthActual = 0;
						response_length = _lengthActual;
						txt = "Location";
					}
					response->SetStatus(_Status);
					response->SetStatusText(txt);


				}

				bool Skip(int64 bytes_to_skip,
					int64& bytes_skipped,
					CefRefPtr<CefResourceSkipCallback> callback) override {

					if (_ranges.size() >= 1)
					{
						RANGE r = _ranges.front();

						bytes_skipped = r.length;
					}
					//LOG(ERROR) << "Skip";
					return true;
				}

				void Cancel() override { CEF_REQUIRE_IO_THREAD(); }

				bool Read(void* data_out,
					int bytes_to_read,
					int& bytes_read,
					CefRefPtr<CefResourceReadCallback> callback) override {
					DCHECK(!CefCurrentlyOn(TID_UI) && !CefCurrentlyOn(TID_IO));


					/*
					bool has_data = false;
					bytes_read = 0;

					if (offset_ < data_.length()) {
					  // Copy the next block of data into the buffer.
					  int transfer_size =
						  std::min(bytes_to_read, static_cast<int>(data_.length() - offset_));
					  memcpy(data_out, data_.c_str() + offset_, transfer_size);
					  offset_ += transfer_size;

					  bytes_read = transfer_size;
					  has_data = true;
					}

					return has_data;
					*/

					bool has_data = false;
					bytes_read = 0;

					if (_content != "")
					{
						if (_offset < _lengthActual)
						{
							int transfer_size = min(bytes_to_read, static_cast<int>(_lengthActual - _offset));

							memcpy(data_out, _content.c_str() + _offset, transfer_size);
							_offset += transfer_size;

							bytes_read = transfer_size;
							has_data = true;
						}
					}
					else
					{
						if (_hFile != INVALID_HANDLE_VALUE)
						{
							DWORD dwNumBytesRead = 0;

							//if (_ranges.size() > 1)
							//{
							//	list<RANGE> ::iterator it;
							//	for (it = _ranges.begin(); it != _ranges.end(); it++)
							//	{
							//	}
							//}

							if (_de_hdr.type == 0)
							{
								int transfer_size = bytes_to_read;
								if (_ranges.size() >= 1)
								{
									RANGE r = _ranges.front();
									//LOG(ERROR) << "start:" << r.start << "-" << r.end;

									transfer_size = min(bytes_to_read, static_cast<int>(r.length - _offset));
								}

								char* pBuf = (char*)malloc(transfer_size + 1);
								if (pBuf != NULL)
								{
									memset(pBuf, 0, transfer_size + 1);

									BOOL bResult = ReadFile(_hFile, pBuf, transfer_size, &dwNumBytesRead, 0);
									if (bResult)
									{
										if (dwNumBytesRead > 0)
										{
											_offset += dwNumBytesRead;
											bytes_read = dwNumBytesRead;
											memcpy(data_out, pBuf, dwNumBytesRead);
											has_data = true;
										}
									}
									free(pBuf);
								}
							}
							else
							{
								int rem = 0;
								int rem_size = 0;
								DWORD transfer_size = bytes_to_read + 8;
								if (_ranges.size() >= 1)
								{
									RANGE r = _ranges.front();
									//LOG(ERROR) << "start:" << r.start << "-" << r.end;

									rem = r.rem;
									rem_size = static_cast<int>(r.length - _offset);
									transfer_size = bytes_to_read + 16;
								}

								char* pBuf = (char*)malloc(transfer_size + 1);
								if (pBuf != NULL)
								{
									memset(pBuf, 0, transfer_size + 1);

									BOOL bResult = ReadFile(_hFile, pBuf, transfer_size, &dwNumBytesRead, 0);
									if (bResult)
									{
										if (dwNumBytesRead > 0)
										{
											int off = 0;
											if (_ranges.size() >= 1)
											{
												if (rem_size > bytes_to_read)
												{
													_offset += bytes_to_read;
													bytes_read = bytes_to_read;

													if (dwNumBytesRead >= bytes_to_read + 16)
													{
														off = 16;
													}
													else if (dwNumBytesRead >= bytes_to_read + 8)
													{
														off = 8;
													}
												}
												else
												{
													_offset += rem_size;
													bytes_read = rem_size;
												}
											}
											else
											{
												if (dwNumBytesRead >= bytes_to_read + 8)
												{
													_offset += bytes_to_read;
													bytes_read = bytes_to_read;

													off = 8;
												}
												else
												{
													_offset += dwNumBytesRead - _de_hdr.offset;
													bytes_read = dwNumBytesRead - _de_hdr.offset;
												}
											}

											if (off != 0)
											{
												LARGE_INTEGER li;
												li.QuadPart = -off;
												SetFilePointer(_hFile, li.LowPart, &li.HighPart, FILE_CURRENT);
											}

											_pBF->Decrypt((void*)(pBuf), dwNumBytesRead);
											memcpy(data_out, pBuf + _de_hdr.offset + rem, bytes_read);

											has_data = true;

										}
									}
									free(pBuf);
								}
							}

							if (_ranges.empty())
							{
								//if (_de_hdr.type != 0)
								//{
								//	//LOG(ERROR) << "_offset:" << _offset << ",_lengthActual:" << _lengthActual;
								//}

								if (_offset >= _lengthActual)
								{
									CloseHandle(_hFile);
									_hFile = INVALID_HANDLE_VALUE;
								}
							}
							else if (_ranges.size() == 1)
							{
								RANGE r = _ranges.front();
								if (_offset >= r.length)
								{
									CloseHandle(_hFile);
									_hFile = INVALID_HANDLE_VALUE;
								}
							}
						}
					}

					return has_data;
				}

				bool document_handler(CefRefPtr<CefRequest> request)
				{
					string szUrl = request->GetURL();

					if (_cmd == "GET")
					{
						string strUrl = "";
						string strArgs = "";
						get_request_url(szUrl.c_str(), strUrl, strArgs);
						URLDecode(strUrl);
						URLDecode(strArgs);

						string path = pathParser(strUrl);
						if (path != "")
						{
							string temp = strUrl;
							string::size_type pos = string::npos;

							wstring szPath = IsoHandler::Instance().get_appdata_path();
							Replace(szPath, L"\\", L"/");

							if (path == "local_thumb")
							{
								return true;
							}
							else if (path == "templ")
							{
								_filePath = IsoHandler::Instance().get_module_path2();
								_filePath += CefString(temp);
							}
							else if (path == "public")
							{
								_filePath = szPath + L"/data1";
								_filePath += CefString(temp);

								pos = szUrl.find("/_data/");
								if (pos == string::npos)
								{
									pos = 9;
									_filePath += L".krf";
								}
								else
								{
									pos = string::npos;
								}
								//LOG(ERROR) << L"GET:" << szUrl.c_str();
								//LOG(ERROR) << L"temp:" << _filePath.c_str();

							}
							else if (path == "private")
							{
								_filePath = szPath + L"/data2";
								_filePath += CefString(temp);
								_filePath += L".krf";

								pos = 9;

								//LOG(ERROR) << L"GET:" << szUrl.c_str();
								//LOG(ERROR) << L"temp:" << _filePath.c_str();
							}
							else if (path == "galleries")
							{
								string::size_type pos0 = temp.find("/galleries");
								if (pos0 != string::npos)
								{
									transform(temp.begin(), temp.end(), temp.begin(), ::tolower);


									//LOG(ERROR) << L"GET:" << szUrl.c_str();
									map<string, string> roots = IsoHandler::Instance()._roots_map;
									map<string, string> ::iterator I;
									for (I = roots.begin(); I != roots.end(); I++)
									{
										//LOG(ERROR) << L"I->first:" << I->first.c_str();
										pos0 = temp.find(I->first);
										if (pos0 != string::npos)
										{
											temp = temp.substr(pos0 + I->first.size() - 1);
											_filePath = CefString(I->second);
											_filePath += CefString(temp);
											break;
										}
									}
									//LOG(ERROR) << L"temp:" << _filePath.c_str();
								}

								if (IsoHandler::Instance().album_ == L"private")
								{
									pos = 9;
									_filePath += L".krf";
								}
								else
								{
									//string mk = "";
									//pos0 = strArgs.find("mk=");
									//if (pos0 != string::npos)
									//{
									//	mk = strArgs.substr(pos0 + 3);
									//}

									//if (mk != "")
									//{
									//	wstring fpath = _filePath;
									//	proc_syn(mk, fpath);
									//}
								}


								//LOG(ERROR) << L"GET:" << szUrl.c_str();
								//LOG(ERROR) << L"temp:" << _filePath.c_str();

							}
							else if (path == "local_file")
							{
								//LOG(ERROR) << L"GET:" << szUrl.c_str();
								string f = strArgs;
								string::size_type pos0 = strArgs.find("f=");
								if (pos0 != string::npos)
								{
									f = strArgs.substr(pos0 + 2);
								}

								pos0 = f.find("&");
								if (pos0 != string::npos)
								{
									f = f.substr(0, pos0);
								}

								_filePath = CefString(f);
								wchar_t szbuf[MAX_PATH] = { 0 };
								DWORD dwSize = GetFullPathName(_filePath.c_str(), MAX_PATH, szbuf, NULL);

								_filePath = szbuf;
								//LOG(ERROR) << "local_file:" << _filePath;
							}
							else if (path == "background")
							{
								wstring p = get_module_path();
								wstring::size_type pos = p.find(L"\\windowsapps\\photonest");
								if (pos != wstring::npos)
								{
									p = IsoHandler::Instance().get_extension_dir();
								}

								_filePath = p;

								_filePath += CefString(temp);
							}
							else if (path == "_face")
							{
								_filePath = szPath;
								_filePath += CefString(temp);
							}
							else if (path == "_face2")
							{
								_filePath = szPath;
								_filePath += CefString(temp);
								_filePath += L".krf";

								pos = 9;
							}

							_hFile = CreateFile(_filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
								FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
								NULL);
							if (_hFile != INVALID_HANDLE_VALUE)
							{
								// Retrive file size
								_lengthActual = GetFileSize(_hFile, NULL);

								if (pos != string::npos)
								{
									unsigned char* tempbuf = (unsigned char*)malloc(100);
									memset(tempbuf, 0, 100);

									DWORD dwRead = 0;
									ReadFile(_hFile, tempbuf, 8, &dwRead, 0);

									DE_HEADER* de_hdr = (DE_HEADER*)tempbuf;
									int offset = 8 + de_hdr->offset;
									_lengthActual -= offset;

									_de_hdr.type = de_hdr->type;
									_de_hdr.offset = de_hdr->offset;
									_de_hdr.crc = de_hdr->crc;

									free(tempbuf);
								}

								multimap<CefString, CefString > header;
								request->GetHeaderMap(header);

								multimap<CefString, CefString > ::iterator it;

								string strRange = "";
								it = header.find(L"Range");
								if (it != header.end())
								{
									strRange = it->second;
									get_range(strRange, _ranges);
									if (!_ranges.empty())
									{
										RANGE r = _ranges.front();
										LARGE_INTEGER li;
										li.QuadPart = r.start;
										if (pos != string::npos)
										{
											ldiv_t div1 = ldiv(r.start, 8);
											li.QuadPart = div1.quot * 8;
										}

										SetFilePointer(_hFile, li.LowPart, &li.HighPart, FILE_CURRENT);
									}
								}

								_mimeType = StuffFileType(_filePath);

								if (_mimeType == "")
								{
									_mimeType = "application/x-javascript";
								}
							}
							else
							{
								_Status = 404;
								_content = "Not Found";
								_mimeType = "text/plain; charset=utf-8";
							}
						}
						else
						{
							//LOG(ERROR) << L"GET:" << szUrl.c_str();
							char* szResponse = 0;
							char* szMimeType = 0;

							HRESULT hr = mygetfunc(szUrl.c_str(), &szResponse, &szMimeType);
							if (hr == S_OK)
							{
								if (strcmp(szMimeType, "Location") == 0)
								{
									pair<std::string, std::string> loc("Location", szResponse);
									_header.insert(loc);
									_content = " ";
									_mimeType = "text/html";
									_Status = 302;

								}
								else if (strncmp(szMimeType, "filename:", 9) == 0)
								{
									//./local_file?C:\Users\liewen\Pictures\IMG_20161224_104905.jpg
									if (strncmp(szResponse, "./local_file?", 13) == 0)
									{
										std::string temp = szResponse + 13;

										string::size_type pos0 = temp.find("f=");
										if (pos0 != string::npos)
										{
											temp = temp.substr(pos0 + 2);
										}

										pos0 = temp.find("&");
										if (pos0 != string::npos)
										{
											temp = temp.substr(0, pos0);
										}
										URLDecode(temp);

										_filePath = CefString(temp);

										wchar_t szbuf[MAX_PATH] = { 0 };
										DWORD dwSize = GetFullPathName(_filePath.c_str(), MAX_PATH, szbuf, NULL);

										_filePath = szbuf;




									}
									//else if (strncmp(szResponse, "./zip?", 6) == 0)
									//{
									//	wstring szPath = get_module_path();
									//	_filePath = szPath + L"\\data\\";
									//	_filePath += CefString(szResponse + 6);
									//}
									else
									{
										_filePath = CefString(szResponse);
									}

									string fname = szMimeType + 9;
									if (fname == "")
									{
										wstring filePath = _filePath;
										wstring::size_type npos = filePath.rfind(L".krf");
										if (npos != wstring::npos)
										{
											if (npos == filePath.length() - 4)
											{
												filePath = filePath.substr(0, npos);
											}
										}

										wchar_t drive[_MAX_DRIVE];
										wchar_t direc[_MAX_DIR];
										wchar_t fnm[_MAX_FNAME];
										wchar_t ext[_MAX_EXT];
										_wsplitpath_s(filePath.c_str(), drive, direc, fnm, ext);

										CefString t;
										t.FromWString(ext);
										fname = "output" + t.ToString();
									}


									std::string val = "attachment; filename*=UTF-8''";
									if (strncmp(szResponse, "./local_file?", 13) == 0)
									{
										val += fname;
									}
									else
									{
										CefString tmp(fname);
										cef_string_userfree_t _retval = cef_uriencode(tmp.GetStruct(), false);

										CefString _retvalStr;
										_retvalStr.AttachToUserFree(_retval);

										val += _retvalStr;
									}

									pair<std::string, std::string>p1("Content-Disposition", val);
									_header.insert(p1);

									_mimeType = StuffFileType(_filePath);

									string::size_type pos = string::npos;

									if (IsoHandler::Instance().album_ == L"private")
									{
										pos = 9;
										_filePath += L".krf";
									}

									_hFile = CreateFile(_filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
										FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
										NULL);

									if (_hFile != INVALID_HANDLE_VALUE)
									{
										_lengthActual = GetFileSize(_hFile, NULL);

										if (pos != string::npos)
										{
											unsigned char* tempbuf = (unsigned char*)malloc(100);
											memset(tempbuf, 0, 100);

											DWORD dwRead = 0;
											ReadFile(_hFile, tempbuf, 8, &dwRead, 0);

											DE_HEADER* de_hdr = (DE_HEADER*)tempbuf;
											int offset = 8 + de_hdr->offset;
											_lengthActual -= offset;

											_de_hdr.type = de_hdr->type;
											_de_hdr.offset = de_hdr->offset;
											_de_hdr.crc = de_hdr->crc;

											free(tempbuf);
										}
									}
								}
								else
								{
									_content = szResponse;
									_mimeType = szMimeType;
								}
								LocalFree(szResponse);
								LocalFree(szMimeType);
							}
							else
							{
								//cancel
								_content = "";
								_Status = 200;

								//
								_Status = 404;
								_content = "Not Found";
								_mimeType = "text/plain; charset=utf-8";


							}
						}
					}
					else if (_cmd == "POST")
					{
						string szBody = "";
						wstring szFile = L"";

						CefRefPtr<CefPostData> data_array = request->GetPostData();
						if (data_array != nullptr && data_array->GetElementCount() > 0)
						{
							CefPostData::ElementVector elements;
							data_array->GetElements(elements);

							size_t num = elements.size();
							for (size_t i = 0; i < num; i++)
							{
								CefRefPtr<CefPostDataElement> data = elements[i];
								cef_postdataelement_type_t t = data->GetType();

								if (t == PDE_TYPE_BYTES)
								{
									size_t length = data->GetBytesCount();
									std::vector<uint8_t> v;
									v.resize(length);
									data->GetBytes(length, &v[0]);

									string trmp = "";
									szBody += trmp.assign(v.begin(), v.end());

								}
								else if (t == PDE_TYPE_FILE)
								{
									szFile = data->GetFile().ToWString();
								}
								else
								{
									//PDE_TYPE_EMPTY
								}
							}
						}

						//LOG(ERROR) << L"POST:" << szUrl.c_str();

						char* szResponse = 0;
						char* szMimeType = 0;
						HRESULT hr = mypostfunc(szUrl.c_str(), szBody.c_str(), szFile.c_str(), &szResponse, &szMimeType);
						if (hr == S_OK)
						{
							if (strcmp(szMimeType, "error") == 0)
							{
								_content = szResponse;
								_mimeType = "text/plain; charset=utf-8";
								_Status = 501;
							}
							else
							{
								_content = szResponse;
								_mimeType = szMimeType;
							}
							LocalFree(szResponse);
							LocalFree(szMimeType);
						}

					}

					return false;
				}

				string pathParser(string& url)
				{
					string temp = "";

					temp = "/templ/";
					if (_strnicmp(url.c_str(), temp.c_str(), temp.length()) == 0)
					{
						return "templ";
					}

					temp = "/background/";
					if (_strnicmp(url.c_str(), temp.c_str(), temp.length()) == 0)
					{
						return "background";
					}

					temp = "/data1/_train/_face/";
					if (_strnicmp(url.c_str(), temp.c_str(), temp.length()) == 0)
					{
						return "_face";
					}

					temp = "/data2/_train/_face/";
					if (_strnicmp(url.c_str(), temp.c_str(), temp.length()) == 0)
					{
						return "_face2";
					}

					temp = "/public/";
					if (_strnicmp(url.c_str(), temp.c_str(), temp.length()) == 0)
					{
						url = url.substr(temp.length() - 1);
						return "public";
					}

					temp = "/private/";
					if (_strnicmp(url.c_str(), temp.c_str(), temp.length()) == 0)
					{
						url = url.substr(temp.length() - 1);
						return "private";
					}

					temp = "/galleries_";
					if (_strnicmp(url.c_str(), temp.c_str(), temp.length()) == 0)
					{
						return "galleries";
					}

					temp = "/local_file";
					if (_strnicmp(url.c_str(), temp.c_str(), temp.length()) == 0)
					{
						return "local_file";
					}

					temp = "/local_thumb";
					if (_strnicmp(url.c_str(), temp.c_str(), temp.length()) == 0)
					{
						return "local_thumb";
					}


					return "";
				}


				void get_request_url(const char* szUrl, std::string& strUrl, std::string& strArgs)
				{
					strUrl = "";
					strArgs = "";

					//"/templ/themes/default/js/plugins/tui/svg/icon-d.svg#icon-d-ic-undo
					std::string temp = szUrl;
					string::size_type ndx = 0;

					if (temp.find("/templ/") != string::npos)
					{
						//"/templ/themes/default/js/plugins/tui/svg/icon-d.svg#icon-d-ic-undo
						ndx = temp.find('#');
						if (ndx != string::npos)
						{
							temp = temp.substr(0, ndx);
						}
					}

					ndx = temp.find('?');
					if (ndx != string::npos)
					{
						// yup; save the args....
						strArgs = temp.substr(ndx + 1);
						// strip from file name....
						strUrl = temp.substr(0, ndx);
					}
					else
					{
						strUrl = temp;
					}

					ndx = strUrl.find("//");
					if (ndx != string::npos)
					{
						strUrl = strUrl.substr(ndx + 2);
					}

					ndx = strUrl.find('/');
					if (ndx != string::npos)
					{
						strUrl = strUrl.substr(ndx);
					}

					//CefString tmp(strUrl);

					//cef_string_userfree_t _retval = cef_uridecode(tmp.GetStruct(), 1, UU_SPOOFING_AND_CONTROL_CHARS);

					//// Return type: string
					//CefString _retvalStr;
					//_retvalStr.AttachToUserFree(_retval);

					//strUrl = _retvalStr;


					//CefString tmp2(strArgs);

					//cef_string_userfree_t _retval2 = cef_uridecode(tmp2.GetStruct(), 1, UU_SPOOFING_AND_CONTROL_CHARS);

					//// Return type: string
					//CefString _retvalStr2;
					//_retvalStr2.AttachToUserFree(_retval2);

					//strArgs = _retvalStr2;
				}

				long sublong(const string& value, size_t beginIndex, size_t endIndex)
				{
					string substring = value.substr(beginIndex, endIndex);

					return (substring.length() > 0) ? std::stol(substring) : -1;

				}

				long get_range(string strRange, list<RANGE>& ranges)
				{
					if (strRange.length() > 6)
					{
						strRange = strRange.substr(6, strRange.length() - 6);

						vector<string> v1 = tokenize(strRange, ",", true, "");
						size_t len = v1.size();
						for (size_t i = 0; i < len; i++)
						{
							string part = v1[i];
							long start = sublong(part, 0, part.find("-", 0));
							long end = sublong(part, part.find("-", 0) + 1, part.length());

							if (start == 0 && end == 0) {
								end = 1;
							}
							else if (start == -1 && end == 1) {
								start = _lengthActual - 2;
								end = _lengthActual - 1;
							}
							else if (start == -1) {
								start = _lengthActual - end - 1;
								if (_lengthActual - start < DEFAULT_BUFFER_SIZE) {
									end = _lengthActual - 1;
								}
								else
									end = start + DEFAULT_BUFFER_SIZE;
							}
							else if (end == -1 || end > _lengthActual - 1) {
								if (_lengthActual - start < DEFAULT_BUFFER_SIZE) {
									end = _lengthActual - 1;
								}
								else
									end = start + DEFAULT_BUFFER_SIZE;
							}

							if (start <= end) {
								RANGE range;
								range.start = start;
								range.end = end;
								range.length = end - start + 1;
								range.total = _lengthActual;
								ldiv_t div1 = ldiv(start, 8);
								range.rem = div1.rem;
								ranges.push_back(range);
							}

						}
					}
					return 0;
				}

				string StuffFileType(wstring filePath)
				{
					BOOL b = FALSE;
					wstring::size_type npos = filePath.rfind(L".krf");
					if (npos != wstring::npos)
					{
						if (npos == filePath.length() - 4)
						{
							filePath = filePath.substr(0, npos);
						}
					}

					wchar_t drive[_MAX_DRIVE];
					wchar_t direc[_MAX_DIR];
					wchar_t fname[_MAX_FNAME];
					wchar_t ext[_MAX_EXT];
					_wsplitpath_s(filePath.c_str(), drive, direc, fname, ext);

					wstring temp = ext;
					transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
					temp.erase(0, temp.find_first_not_of(L"."));

					//	cef_string_t t0;
					//	t0.str = ext;
					//	t0.length = wcslen(ext);
					////	CefString(t0) = L"ext";

					string strType = "";// ::cef_get_mime_type(&t0);
					if (_pMimeType != NULL && _pMimeType->GetMime(temp, strType))
					{
						b = true;
					}

					if (temp == L"mp4" || temp == L"webm")
					{
						b = true;
					}

					if (!b)
					{
						unsigned char* bufType = (unsigned char*)malloc(1024);
						if (bufType != NULL)
						{
							memset(bufType, 0, 1024);

							// find it in the registry....
							HKEY hKey = NULL;
							if (RegOpenKeyEx(HKEY_CLASSES_ROOT, temp.c_str(),
								0, KEY_READ, &hKey) == ERROR_SUCCESS)
							{
								DWORD dwSize = 0;
								// see how long the data is....
								if (RegQueryValueExA(hKey, "Content Type", NULL, NULL,
									NULL, &dwSize) == ERROR_SUCCESS)
								{
									LONG lRet = RegQueryValueExA(hKey, "Content Type", NULL, NULL,
										bufType, &dwSize);
									if (lRet == ERROR_SUCCESS)
									{
										strType = (char*)bufType;
										b = true;
									}
								}
								RegCloseKey(hKey);
							}

							free(bufType);
						}
					}

					return strType;
				}


				static UINT __stdcall ProcThumbnailThread(LPVOID pParam)
				{
					ClientSchemeHandler* pThis = (ClientSchemeHandler*)pParam;
					pThis->AddRef();

					HRESULT hr = S_OK;
					char* szResponse = 0;
					char* szMimeType = 0;
					if (pThis->_cmd == "GET")
					{
						hr = mygetfunc(pThis->_szUploadUrl.c_str(), &szResponse, &szMimeType);
					}
					else if (pThis->_cmd == "POST")
					{
						hr = mypostfunc(pThis->_szUploadUrl.c_str(), pThis->_szUploadBody.c_str(), pThis->_szUploadFile.c_str(), &szResponse, &szMimeType);
					}

					if (hr == S_OK)
					{

						pThis->_mimeType = szMimeType;
						if (pThis->_mimeType == "0")
						{
							pThis->_content = "";
							pThis->_filePath = CefString(szResponse);
							//LOG(ERROR) << pThis->_filePath;
							pThis->_hFile = CreateFile(pThis->_filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
								FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
						}
						else
						{
							pThis->_content = szResponse;
						}

						LocalFree(szResponse);
						LocalFree(szMimeType);

						pThis->_responseHeadersReadyCallback->Continue();

						//important: when debug it is error, but direct run is ok
					//if (!IsBadReadPtr(pThis->_responseHeadersReadyCallback, 100))
					}
					pThis->Release();
					return 0;
				}


			private:
				std::string data_;
				std::string mime_type_;
				size_t offset_;

				const int DEFAULT_BUFFER_SIZE = 204800 - 1;
				//const int DEFAULT_BUFFER_SIZE = 327680 - 1;

				std::wstring _filePath;
				std::list<RANGE> _ranges;
				HANDLE _hFile;
				int64 _lengthActual;
				multimap<CefString, CefString > _header;

				DE_HEADER _de_hdr;
				size_t _offset;
				std::string _content;
				std::string _mimeType;
				int _Status;

				CefRefPtr<CefCallback> _responseHeadersReadyCallback;
				string _szUploadBody;
				wstring _szUploadFile;
				string _szUploadUrl;
				CefString _cmd;



				IMPLEMENT_REFCOUNTING(ClientSchemeHandler);
				DISALLOW_COPY_AND_ASSIGN(ClientSchemeHandler);
			};

			// Implementation of the factory for for creating schema handlers.
			class ClientSchemeHandlerFactory : public CefSchemeHandlerFactory {
			public:
				ClientSchemeHandlerFactory() {}

				// Return a new scheme handler instance to handle the request.
				CefRefPtr<CefResourceHandler> Create(CefRefPtr<CefBrowser> browser,
					CefRefPtr<CefFrame> frame,
					const CefString& scheme_name,
					CefRefPtr<CefRequest> request) override {
					CEF_REQUIRE_IO_THREAD();
					return new ClientSchemeHandler();
				}

				IMPLEMENT_REFCOUNTING(ClientSchemeHandlerFactory);
				DISALLOW_COPY_AND_ASSIGN(ClientSchemeHandlerFactory);
			};

		}  // namespace

		void RegisterSchemeHandlers() {
			//CefRegisterSchemeHandlerFactory("client", "tests",
			//                                new ClientSchemeHandlerFactory());

			if (_pMimeType == NULL)
			{
				_pMimeType = new CMimeType();
				_pMimeType->InitializeMime();
			}

			CefRegisterSchemeHandlerFactory("http", "nest1.pc",
				new ClientSchemeHandlerFactory());
			CefRegisterSchemeHandlerFactory("http", "nest2.pc",
				new ClientSchemeHandlerFactory());


		}
		void UnRegisterSchemeHandlers() {
			if (_pMimeType == NULL)
			{
				delete _pMimeType;
				_pMimeType = NULL;
			}
			if (_pBF == NULL)
			{
				delete _pBF;
				_pBF = NULL;
			}
		}

	}  // namespace scheme_test
}  // namespace client
