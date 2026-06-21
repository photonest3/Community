#include "StdAfx.h"
#include "unitil.h"
#include <vector>
#include <string>
#include <atlcomcli.h>
#include "HtmlgetFolder.h"
#include "writedb.h"

//#include <filesystem>
//namespace fs = std::filesystem;
bool compareByName(const WIN32_FIND_DATA& a, const WIN32_FIND_DATA& b)
{
	return wcscmp(a.cFileName, b.cFileName) < 0;
}
//bool compareByName2(const FILE_DTO& a, const FILE_DTO& b)
//{
//	return wcscmp(a.file.c_str(), b.file.c_str()) < 0;
//}

CHtmlgetFolder::CHtmlgetFolder(void)
{
	//string pattern = "(jpg|jpeg|png|webp|webm|gif|bmp|mp3|mp4|ogg)";
	//_re = regex(pattern, regex::icase);
}

CHtmlgetFolder::~CHtmlgetFolder(void)
{
}

long CHtmlgetFolder::GetSubFolders(int ty, wstring strPath, list<wstring >& lstSubs)
{
	//for (auto &itr: filesystem::directory_iterator(strPath))
	//{
	//	auto filepath=itr.path();
	//}

	try
	{
		Scripting::IFileSystem3Ptr fs;
		fs.CreateInstance(__uuidof(Scripting::FileSystemObject));

		Scripting::IFolderPtr folder;

		fs->GetFolder(_bstr_t(strPath.c_str()), &folder);
		if (folder)
		{
			zz(ty, folder, lstSubs);
		}

		fs.Release();
	}
	catch (...)
	{
		//_bstr_t bstrSource(e.Source());
		//_bstr_t bstrDescription(e.Description());
		//std::cout << "Get directory size via FileSystemObject, by masterz" << std::endl;
		//std::cout << "COM error occurred,source:" << (LPCTSTR)bstrSource << std::endl;
		//std::cout << "Description:" << (LPCTSTR)bstrDescription << std::endl;
	}

	return 0;
}

int CHtmlgetFolder::zz(int ty, Scripting::IFolderPtr folder, list<wstring >& lstSubs)
{
	try
	{
		Scripting::IFolderCollectionPtr files;
		folder->get_SubFolders(&files);

		IUnknown* pUnk = NULL;
		HRESULT hr = files->get__NewEnum(&pUnk);
		if (SUCCEEDED(hr))
		{
			CComPtr<IEnumVARIANT> pEnum;
			hr = pUnk->QueryInterface(IID_IEnumVARIANT, (void**)&pEnum);
			if (pEnum)
			{
				pEnum->Reset();
				ULONG fget = 1;
				while (SUCCEEDED(hr) && fget > 0)
				{
					Scripting::IFolderPtr file;
					_variant_t varfile;
					hr = pEnum->Next(1, &varfile, &fget);
					if (SUCCEEDED(hr) && fget > 0)
					{
						file = varfile.pdispVal;
						if (file)
						{
							BSTR bspath;
							file->get_Path(&bspath);

							wstring folderName = (wchar_t*)_bstr_t(bspath);
							Replace(folderName, L"\\", L"/");

							lstSubs.push_back(folderName);
							if (ty == 1)
							{
								zz(ty, file, lstSubs);
							}
						}
					}
					else
					{
						//std::cout << " get file  return " << hr << std::endl;
					}
				}
			}
			files->Release();
		}
	}
	catch (...) //_com_error &e)
	{
		//_bstr_t bstrSource(e.Source());
		//_bstr_t bstrDescription(e.Description());
		//std::cout << "Get directory size via FileSystemObject, by masterz" << std::endl;
		//std::cout << "COM error occurred,source:" << (LPCTSTR)bstrSource << std::endl;
		//std::cout << "Description:" << (LPCTSTR)bstrDescription << std::endl;
	}

	return 0;
}

long CHtmlgetFolder::GetFolderPics(string cat_id, wstring strPath, uint8_t crc, list<FILE_DTO >& lstFile)
{
	strPath.erase(strPath.find_last_not_of(L"/") + 1);

	wchar_t* buf1 = (wchar_t*)malloc(1024 * sizeof(wchar_t));
	if (buf1 != NULL)
	{
		memset(buf1, 0, 1024 * sizeof(wchar_t));
		swprintf_s(buf1, 1024, L"%s/*", strPath.c_str());

		WIN32_FIND_DATA ffd;
		HANDLE hFind = FindFirstFile(buf1, &ffd);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			if (wcscmp(ffd.cFileName, L".") != 0 && wcscmp(ffd.cFileName, L"..") != 0)
			{
				if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) || (ffd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
				{
				}
				else
				{
					wstring fullname = strPath + L"/" + ffd.cFileName;
					bool b = is_picture((wchar_t*)fullname.c_str(), crc);
					if (b)
					{
						FILE_DTO dto;
						dto.cat_id = cat_id;
						dto.filename = fullname;
						dto.file = ffd.cFileName;
						dto.dwSize = ffd.nFileSizeLow;

						lstFile.push_back(dto);
					}
				}
			}

			while (FindNextFile(hFind, &ffd))
			{
				if (wcscmp(ffd.cFileName, L".") != 0 && wcscmp(ffd.cFileName, L"..") != 0)
				{
					if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) || (ffd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
					{
					}
					else
					{
						wstring fullname = strPath + L"/" + ffd.cFileName;
						bool b = is_picture((wchar_t*)fullname.c_str(), crc);
						if (b)
						{
							FILE_DTO dto;
							dto.cat_id = cat_id;
							dto.filename = fullname;
							dto.dwSize = ffd.nFileSizeLow;
							dto.file = ffd.cFileName;

							lstFile.push_back(dto);
						}
					}
				}
			}
			FindClose(hFind);
		}
		free(buf1);
	}

	//lstFile.sort(compareByName2);
	return 0;
}

long CHtmlgetFolder::GetFolderPics(int ty, wstring strPath, list<WIN32_FIND_DATA >& lstffd)
{
	strPath.erase(strPath.find_last_not_of(L"/") + 1);

	wchar_t* buf1 = (wchar_t*)malloc(1024 * sizeof(wchar_t));
	if (buf1 != NULL)
	{
		memset(buf1, 0, 1024 * sizeof(wchar_t));
		swprintf_s(buf1, 1024, L"%s/*", strPath.c_str());

		WIN32_FIND_DATA ffd;
		HANDLE hFind = FindFirstFile(buf1, &ffd);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			if (wcscmp(ffd.cFileName, L".") != 0 && wcscmp(ffd.cFileName, L"..") != 0)
			{
				if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) || (ffd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
				{
				}
				else
				{
					if (ty == 1)
					{
						lstffd.push_back(ffd);
					}
					else
					{
						uint8_t crc = 0;
						bool b = is_picture(ffd.cFileName, crc);
						if (b)
						{
							lstffd.push_back(ffd);
						}
					}
				}
			}

			while (FindNextFile(hFind, &ffd))
			{
				if (wcscmp(ffd.cFileName, L".") != 0 && wcscmp(ffd.cFileName, L"..") != 0)
				{
					if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) || (ffd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
					{
					}
					else
					{
						if (ty == 1)
						{
							lstffd.push_back(ffd);
						}
						else
						{
							uint8_t crc = 0;
							bool b = is_picture(ffd.cFileName, crc);
							if (b)
							{
								lstffd.push_back(ffd);
							}
						}
					}
				}
			}
			FindClose(hFind);
		}
		free(buf1);
	}

	if (ty == 0)
	{
		lstffd.sort(compareByName);
	}

	return 0;
}

bool inline CHtmlgetFolder::is_picture(wchar_t* cFileName, uint8_t crc)
{
	wstring ext = L"";
	wchar_t* p = wcsrchr(cFileName, L'.');
	if (p != NULL)
	{
		ext = p;
	}
	else
	{
		return false;
	}

	bool b = false;
	const wchar_t** filename = _exts;

	while (*filename)
	{
		if (_wcsicmp(*filename, ext.c_str()) == 0)
		{
			b = true;
			break;
		}
		filename++;
	}

	//bool b = regex_match(ext0, _re);
	if (b)
	{
		p = wcsstr(cFileName, L"_coi.png");
		if (p == NULL)
		{
			return true;
		}
	}

	//if (_alb == "private")
	if (crc != 0)
	{
		if (ext == L".krf")
		{
			p = wcsstr(cFileName, L"_coi.png");
			if (p == NULL)
			{
				uint8_t crc0;
				get_file_crc(cFileName, crc0);
				if (crc == crc0)
				{
					return true;
				}
			}
		}
	}

	return false;
}

long CHtmlgetFolder::get_file_crc(wchar_t* src, uint8_t& crc)
{
	crc = 0;
	HANDLE hFile = CreateFile(src, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return 1;
	}

	DWORD nTemp = sizeof(DE_HEADER);
	unsigned char* tempbuf = (unsigned char*)malloc(nTemp + 1);
	if (tempbuf != NULL)
	{
		memset(tempbuf, 0, nTemp + 1);
		DWORD npos = 0, dwRead = 0, dwWritten = 0;

		BOOL b = ReadFile(hFile, tempbuf, nTemp, &dwRead, 0);
		if (b && dwRead >= nTemp)
		{
			DE_HEADER* de_hdr = (DE_HEADER*)tempbuf;

			crc = de_hdr->crc;
		}
		free(tempbuf);
	}

	CloseHandle(hFile);

	return 0;
}

long CHtmlgetFolder::GetFolderSize(list<ROOT_DTO> lstRoot, string album, uint64_t& fsize)
{
	try
	{
		fsize = 0;
		Scripting::IFileSystem3Ptr fs;
		fs.CreateInstance(__uuidof(Scripting::FileSystemObject));

		wstring dDir = (album == "private") ? L"/data2" : L"/data1";
		wstring strPath = get_appdata_path(album);//CDataSrv::Instance()._appDataDir;//get_module_path(NULL);

		wstring pub = strPath + dDir;
		Scripting::IFolderPtr folder;
		fs->GetFolder(_bstr_t(pub.c_str()), &folder);

		if (folder)
		{
			_variant_t vsize2;
			folder->get_Size(&vsize2);
			fsize += variantSize(vsize2);
		}

		if (!lstRoot.empty())
		{
			list<wstring> lstFile;
			list<ROOT_DTO>::iterator it;
			for (it = lstRoot.begin(); it != lstRoot.end(); it++)
			{
				//lstFile.push_back(_u2w(it->root_dir));
				uint32_t fsize0 = 0;
				zz0(_u2w(it->root_dir0), lstFile, fsize0);
				fsize += fsize0;
			}

			list<wstring>::iterator I;
			for (I = lstFile.begin(); I != lstFile.end(); I++)
			{
				_variant_t vsize2;
				fs->GetFolder(_bstr_t(I->c_str()), &folder);
				if (folder)
				{
					folder->get_Size(&vsize2);
					fsize += variantSize(vsize2);
				}
			}
		}

		fs.Release();
	}
	catch (...)
	{
		//_bstr_t bstrSource(e.Source());
		//_bstr_t bstrDescription(e.Description());
		//std::cout << "Get directory size via FileSystemObject, by masterz" << std::endl;
		//std::cout << "COM error occurred,source:" << (LPCTSTR)bstrSource << std::endl;
		//std::cout << "Description:" << (LPCTSTR)bstrDescription << std::endl;
	}

	return 0;
}

long CHtmlgetFolder::zz0(wstring strPath, list<wstring>& lstFile, uint32_t& fsize)
{
	fsize = 0;
	strPath.erase(strPath.find_last_not_of(L"/") + 1);

	wchar_t* buf1 = (wchar_t*)malloc(1024 * sizeof(wchar_t));
	if (buf1 != NULL)
	{
		memset(buf1, 0, 1024 * sizeof(wchar_t));
		swprintf_s(buf1, 1024, L"%s/*", strPath.c_str());

		WIN32_FIND_DATA ffd;
		HANDLE hFind = FindFirstFile(buf1, &ffd);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			if (wcscmp(ffd.cFileName, L".") != 0 && wcscmp(ffd.cFileName, L"..") != 0)
			{
				if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) || (ffd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
				{
					wstring fullname = strPath + L"/" + ffd.cFileName;
					lstFile.push_back(fullname);
				}
				else
				{
					fsize += ffd.nFileSizeLow / 1024;
				}
			}

			while (FindNextFile(hFind, &ffd))
			{
				if (wcscmp(ffd.cFileName, L".") != 0 && wcscmp(ffd.cFileName, L"..") != 0)
				{
					if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) || (ffd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
					{
						wstring fullname = strPath + L"/" + ffd.cFileName;
						lstFile.push_back(fullname);
					}
					else
					{
						fsize += ffd.nFileSizeLow / 1024;
					}
				}
			}
			FindClose(hFind);
		}
		free(buf1);
	}

	return 0;
}

uint64_t CHtmlgetFolder::variantSize(_variant_t vsize2)
{
	uint64_t fsize = 0;
	switch (vsize2.vt)
	{
	case VT_I2:
		fsize = vsize2.iVal / 1024;
		break;
	case VT_I4:
		fsize = vsize2.lVal / 1024;
		break;
	case VT_R4:
		fsize = (uint64_t)(vsize2.fltVal / 1024.0f);
		break;
	case VT_R8:
		fsize = (uint64_t)(vsize2.dblVal / 1024.0f);
		break;
	default:
		break;
	}
	return fsize;
}

long CHtmlgetFolder::GetSubFolderNb(wstring dDir, map<wstring, uint32_t>& mapSubs)
{
	try
	{
		Scripting::IFileSystem3Ptr fs;
		fs.CreateInstance(__uuidof(Scripting::FileSystemObject));

		Scripting::IFolderPtr folder;
		fs->GetFolder(_bstr_t(dDir.c_str()), &folder);

		if (folder)
		{
			zz5(folder, mapSubs);
		}
		fs.Release();
	}
	catch (...) //_com_error &e)
	{
		//_bstr_t bstrSource(e.Source());
		//_bstr_t bstrDescription(e.Description());
		//std::cout << "Get directory size via FileSystemObject, by masterz" << std::endl;
		//std::cout << "COM error occurred,source:" << (LPCTSTR)bstrSource << std::endl;
		//std::cout << "Description:" << (LPCTSTR)bstrDescription << std::endl;
	}

	return 0;
}

int CHtmlgetFolder::zz5(Scripting::IFolderPtr folder, map<wstring, uint32_t>& mapSubs)
{
	try
	{
		BSTR bspath;
		folder->get_Path(&bspath);

		wstring folderName = (wchar_t*)_bstr_t(bspath);
		Replace(folderName, L"\\", L"/");
		//my_tolower(folderName);

		Scripting::IFileCollectionPtr filer;
		folder->get_Files(&filer);

		long nb = 0;
		filer->get_Count(&nb);
		mapSubs.insert(map<wstring, uint32_t>::value_type(folderName, nb));

		Scripting::IFolderCollectionPtr subfolders;
		folder->get_SubFolders(&subfolders);

		IUnknown* pUnk = NULL;
		HRESULT hr = subfolders->get__NewEnum(&pUnk);
		if (SUCCEEDED(hr))
		{
			CComPtr<IEnumVARIANT> pEnum;
			hr = pUnk->QueryInterface(IID_IEnumVARIANT, (void**)&pEnum);
			if (pEnum)
			{
				pEnum->Reset();
				ULONG fget = 1;
				while (SUCCEEDED(hr) && fget > 0)
				{
					Scripting::IFolderPtr folder2;
					_variant_t varfile;
					hr = pEnum->Next(1, &varfile, &fget);
					if (SUCCEEDED(hr) && fget > 0)
					{
						folder2 = varfile.pdispVal;
						if (folder2)
						{
							zz5(folder2, mapSubs);
						}
					}
				}
			}
			subfolders->Release();
		}
	}
	catch (...) //_com_error &e)
	{
		//_bstr_t bstrSource(e.Source());
		//_bstr_t bstrDescription(e.Description());
		//std::cout << "Get directory size via FileSystemObject, by masterz" << std::endl;
		//std::cout << "COM error occurred,source:" << (LPCTSTR)bstrSource << std::endl;
		//std::cout << "Description:" << (LPCTSTR)bstrDescription << std::endl;
	}

	return 0;
}

long CHtmlgetFolder::DelFolderFiles(wstring strPath)
{
	strPath.erase(strPath.find_last_not_of(L"/") + 1);

	wchar_t* buf1 = (wchar_t*)malloc(1024 * sizeof(wchar_t));
	if (buf1 != NULL)
	{
		memset(buf1, 0, 1024 * sizeof(wchar_t));
		swprintf_s(buf1, 1024, L"%s/*", strPath.c_str());

		WIN32_FIND_DATA ffd;
		HANDLE hFind = FindFirstFile(buf1, &ffd);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			if (wcscmp(ffd.cFileName, L".") != 0 && wcscmp(ffd.cFileName, L"..") != 0)
			{
				if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) || (ffd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
				{
				}
				else
				{
					wstring fullname = strPath + L"/" + ffd.cFileName;
					DeleteFile(fullname.c_str());
				}
			}

			while (FindNextFile(hFind, &ffd))
			{
				if (wcscmp(ffd.cFileName, L".") != 0 && wcscmp(ffd.cFileName, L"..") != 0)
				{
					if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) || (ffd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
					{
					}
					else
					{
						wstring fullname = strPath + L"/" + ffd.cFileName;
						DeleteFile(fullname.c_str());
					}
				}
			}
			FindClose(hFind);
		}
		free(buf1);
	}

	return 0;
}

long CHtmlgetFolder::GetFolderPics(wstring strPath, queue<wstring>& lstFile)
{
	strPath.erase(strPath.find_last_not_of(L"/") + 1);

	wchar_t* buf1 = (wchar_t*)malloc(1024 * sizeof(wchar_t));
	if (buf1 != NULL)
	{
		memset(buf1, 0, 1024 * sizeof(wchar_t));
		swprintf_s(buf1, 1024, L"%s/*", strPath.c_str());

		WIN32_FIND_DATA ffd;
		HANDLE hFind = FindFirstFile(buf1, &ffd);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			if (wcscmp(ffd.cFileName, L".") != 0 && wcscmp(ffd.cFileName, L"..") != 0)
			{
				if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) || (ffd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
				{
				}
				else
				{
					wstring ext = L"";
					wchar_t* p = wcsrchr(ffd.cFileName, L'.');
					if (p != NULL)
					{
						ext = p;
					}

					if (ext == L".krf")
					{
						wstring fullname = strPath + L"/" + ffd.cFileName;
						lstFile.push(fullname);
					}
				}
			}

			while (FindNextFile(hFind, &ffd))
			{
				if (wcscmp(ffd.cFileName, L".") != 0 && wcscmp(ffd.cFileName, L"..") != 0)
				{
					if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) || (ffd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
					{
					}
					else
					{
						wstring ext = L"";
						wchar_t* p = wcsrchr(ffd.cFileName, L'.');
						if (p != NULL)
						{
							ext = p;
						}

						if (ext == L".krf")
						{
							wstring fullname = strPath + L"/" + ffd.cFileName;
							lstFile.push(fullname);
						}
					}
				}
			}
			FindClose(hFind);
		}
		free(buf1);
	}

	return 0;
}

void CHtmlgetFolder::sc(wstring path, wstring appDataDir, wstring subfolder)
{
	list<WIN32_FIND_DATAW > vecffd;
	GetFolderPics(1, path + subfolder, vecffd);

	list<WIN32_FIND_DATA>::iterator it0;
	for (it0 = vecffd.begin(); it0 != vecffd.end(); it0++)
	{
		wstring src = path + subfolder + L"\\" + it0->cFileName;

		wstring dst = appDataDir + subfolder + L"\\" + it0->cFileName;
		CopyFile(src.c_str(), dst.c_str(), TRUE);
	}
}

void CHtmlgetFolder::CopyAppData(string alb, int msix, int& bAttr)
{
	wstring szPath = get_module_path(NULL);
	wstring appDataDir = get_appdata_path2(alb, 1);


	DWORD dwAttr = 0;
	wstring db = L"data1";
	if (alb == "private")
	{
		db = L"data2";
		wstring dst2 = appDataDir + L"\\" + db + L"\\_db\\photo2.db";
		dwAttr = GetFileAttributesW(dst2.c_str());
	}
	else {
		wstring dst2 = appDataDir + L"\\" + db + L"\\_db\\photo1.db";
		dwAttr = GetFileAttributesW(dst2.c_str());
	}

	if (dwAttr == -1)
	{
		bAttr = 1;
		wstring s5 = appDataDir + L"\\" + db;
		CreateDirectory(s5.c_str(), NULL);

		s5 = appDataDir + L"\\" + db + L"\\_db";
		CreateDirectory(s5.c_str(), NULL);


		wstring src = szPath + L"/db/photo.db";
		wstring dst = appDataDir + L"/" + db + L"/_db/photo1.db";
		if (alb == "private")
		{
			dst = appDataDir + L"/" + db + L"/_db/photo2.db";
		}
		CopyFile(src.c_str(), dst.c_str(), TRUE);

		if (msix == 1)
		{
			//appDataDir = get_appdata_path2(alb, 0);
			s5 = appDataDir + L"\\sample";
			CreateDirectory(s5.c_str(), NULL);

			s5 = appDataDir + L"\\sample\\lena";
			CreateDirectory(s5.c_str(), NULL);

			s5 = appDataDir + L"\\sample\\sea";
			CreateDirectory(s5.c_str(), NULL);


			//s5 = appDataDir + L"\\background";
			//CreateDirectory(s5.c_str(), NULL);



			//s5 = appDataDir + L"\\extension";
			//CreateDirectory(s5.c_str(), NULL);

			//s5 = appDataDir + L"\\extension\\sky";
			//CreateDirectory(s5.c_str(), NULL);

			//s5 = appDataDir + L"\\extension\\sticker";
			//CreateDirectory(s5.c_str(), NULL);


			sc(szPath, appDataDir, L"\\sample\\lena");
			sc(szPath, appDataDir, L"\\sample\\sea");
			sc(szPath, appDataDir, L"\\sample");
			//sc(szPath, appDataDir, L"\\background");
			//sc(szPath, appDataDir, L"\\extension\\sky");
			//sc(szPath, appDataDir, L"\\extension\\sticker");

			/*
			std::wstring lnk = L"";

			wchar_t* szDocument;
			if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Desktop, 0, NULL, &szDocument)))
			{
				lnk = szDocument;
				lnk += L"\\PhotoNest.lnk";

				wstring icon = szPath + L"/Images/g001.ico";
				//CreateLink(L"photonest.exe", lnk.c_str(), L"PhotoNest", L"", icon.c_str());

				CoTaskMemFree(szDocument);
			}
			*/
		}
	}
}


HRESULT CHtmlgetFolder::CreateLink(LPCWSTR lpszPathObj, LPCWSTR lpszPathLink, LPCWSTR lpszDesc, LPCWSTR lpszArgs, LPCWSTR lpszIcon)
{
	HRESULT hres;
	IShellLink* psl;

	// Get a pointer to the IShellLink interface. It is assumed that CoInitialize
	// has already been called.
	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl);
	//DWORD errorCode = ::GetLastError();

	if (SUCCEEDED(hres))
	{
		IPersistFile* ppf;

		// Set the path to the shortcut target and add the description. 
		psl->SetPath(lpszPathObj);
		if (lpszDesc)
		{
			psl->SetDescription(lpszDesc);
		}
		if (lpszIcon)
		{
			psl->SetIconLocation(lpszIcon, 0);
		}
		if (lpszArgs)
		{
			psl->SetArguments(lpszArgs);
		}

		// Query IShellLink for the IPersistFile interface, used for saving the 
		// shortcut in persistent storage. 
		hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf);

		if (SUCCEEDED(hres))
		{
			// Save the link by calling IPersistFile::Save. 
			hres = ppf->Save(lpszPathLink, TRUE);
			ppf->Release();
		}
		psl->Release();
	}
	return hres;
}

wstring CHtmlgetFolder::get_appdata_path2(string alb, int ty)
{
	std::wstring dst = L"";
	wchar_t* szDocument;
	HRESULT hr = 0;
	if (ty == 1) {
		hr = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &szDocument);
	}
	else {
		hr = SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &szDocument);
	}

	if (SUCCEEDED(hr))
	{
		dst = szDocument;

		wstring p = get_module_path(NULL);
		wstring::size_type pos = p.find(L"/windowsapps/photonest");
		if (pos != wstring::npos)
		{
			dst += L"\\PhotoNest_App";
		}
		else
		{
			dst += L"\\PhotoNest";
		}
		CreateDirectory(dst.c_str(), NULL);

		if (pos != wstring::npos)
		{
			dst += L"\\Nest1";
		}
		else {
			if (alb == "private")
			{
				dst += L"\\Nest2";
			}
			else
			{
				dst += L"\\Nest1";
			}
		}
		CreateDirectory(dst.c_str(), NULL);

		CoTaskMemFree(szDocument);
	}

	wstring sz = dst;

	return sz;
}