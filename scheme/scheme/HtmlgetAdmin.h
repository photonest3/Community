#pragma once

#include "stmdef.h"
#include "Singleton.h"
#include "ImBase.h"
#include <atomic>
#include <queue>

class CHtmlgetAdmin : public CImBase, public Singleton<CHtmlgetAdmin>
{
	friend class Singleton<CHtmlgetAdmin>;
protected:
	CHtmlgetAdmin(void);
	~CHtmlgetAdmin(void);

public:
	long get_admin_htm(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	long post_admin_htm(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	long get_ws_php(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	long post_ws_php(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	long delete_image_files(string type, list<string> _lstFiles, list<string> _lstCoi);

	long get_folder_sizes(int ty, uint64_t& fsize);
	long remove_root(string first, list<CATEGORY_DTO> lst, string root_id, string root_dir);
	long unins();

	long ParseLangXML();
	long addbyViewer(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	long get_restore(list<string>& fol);

private:
	string get_field_item(string  szBody, string name, string id);
	long get_fullname(string full_id, list<CATEGORY_DTO>* ptrlstCategory, string& fullname);

	long proc_login(const REQUEST_DTO request_dto);
	long proc_registr(const REQUEST_DTO request_dto);
	long get_tagids(string tags, string& val);
	long get_viewer_u3_degree(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);

	static UINT __stdcall sb_brisque(LPVOID pParam);
	long brisque(int idx);

	void galleries_addfolder(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	void categories_refresh(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	void galleries_init(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	void galleries_pending(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	void galleries_pendingok(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	void galleries_clear(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	void galleries_categories(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	void upload_complete(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	void galleries_remove(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	void galleries_restore(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);

	bool folder_ischange();

	void init_proc(IDX_DTO idxdto);
	static UINT __stdcall sb_init(LPVOID pParam);
	atomic_int _nb_init0_thread[100] = { 0 };
	queue<FOLDER_STATE_DTO> _queState[100];

	static UINT __stdcall sb_upload(LPVOID pParam);
	void upload();
	atomic_int _nb_gg2_thread;
	atomic_int _progress_len3;
	atomic_int _progress_step3;
	atomic_int _progress_cancel;

	atomic_int _nb_upload_ok = 0;
	atomic_int _nb_upload_err30 = 0;
	atomic_int _nb_upload_err40 = 0;
	queue<FILE_DTO> _queUpload;

public:
	atomic_int _progress_step;
	atomic_int _progress_len;
	uint8_t _crc;

	void update_catnb(map<string, uint32_t> mapCat);
	void galleries_brisque(int idx);
	void viewer_nb();


private:
	static UINT __stdcall sb_nb(LPVOID pParam);
public:
	void nb_proc();
	atomic_int _lstCatNbOk;
private:
	static UINT __stdcall proc_nb_(LPVOID pParam);
	void proc_nb_ok();
	list<FILE_DTO> _lstDbPending;
	list<FILE_DTO> _lstFsPending;
	queue<CAT_NB_DTO> _queCatNb2;

	list<FOLDER_STATE_DTO > _lstState;

	atomic_int _nb_thread;
	string _del_ids = "";
	MAP_MD5 _lstmd5;
	list<CAT_NB_DTO> _lstCatNb;
	list<FOLDER_STATE_DTO> _lstInitDto;

	CRITICAL_SECTION _nbcs;
	CRITICAL_SECTION _cs0;

	map<string, string> _cats_map;
	map<wstring, uint32_t> _mapSizes;

	string _trs = "";
	int _cpu = 5;
	atomic_int _brisquing = 0;

	static UINT __stdcall sb_decrypt(LPVOID pParam);
	void decrypt();

	queue<wstring> _queDecrypt;
	queue<wstring> _queThumb;
	list<wstring> _lstDir;
	void processok(string root_id);

public:
	atomic_int _brisque_end = 1;

private:
	static UINT __stdcall sb_predict(LPVOID pParam);
	void predict_proc();
	atomic_int _predict = 0;
	atomic_int _nbTrain = 0;
	string _faceIds = "";
	list<string> _lstImageId;

	void get_group_face(wstring strPath, map<string, list<string>>& mapFiles);

};
