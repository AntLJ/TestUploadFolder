#include "stdafx.h"
#include "ConvertZipCodeData.h"
#include "ConvRelZipAddr.h"
#include "ConvZipPol.h"
#include "ConvCentroid.h"
#include "ConvRep.h"
#include "Log.h"
#include "Common.h"

bool CConvertZipCodeData::init(const Arguments& args)
{
	// データ出力先設定
	m_strOutDir = args.m_out_dir.c_str();
	m_strOutDir = m_strOutDir.TrimRight(_T("\\"));

	// 郵便番号ポリゴンのワークスペース取得
	IWorkspacePtr ipWork = sindy::create_workspace(args.m_zip_db.c_str());
	if(!ipWork){
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgDBConnect, CString(args.m_zip_db.c_str()));
		return false;
	}

	// 郵便番号ポリゴンのテーブル名設定
	CString strZipPol;
	if(args.m_zip_schema.empty())
		strZipPol = sindy::schema::zipcode_area::kTableName;
	else
		strZipPol = CString(args.m_zip_schema.c_str()) + _T(".") + sindy::schema::zipcode_area::kTableName;

	// 郵便番号ポリゴンのフィーチャクラス取得
	if(S_OK != IFeatureWorkspacePtr(ipWork)->OpenFeatureClass(CComBSTR(strZipPol), &m_ipZipAreaClass)){
		CLog::GetInstance().PrintLog2(false, true, false, true, err_type::fatal, err_code::NgOpenTable, strZipPol, _T(""));
		return false;
	}

	// 11桁住所データ格納先設定
	m_strAddr11Dir = args.m_addr11_dir.c_str();

	// 住所紐付けテーブル（20桁コード）のテーブル名設定
	CString strRelAddr;
	if(args.m_zip_schema.empty())
		strRelAddr = sindy::schema::rel_addr::kTableName20;
	else
		strRelAddr = CString(args.m_zip_schema.c_str()) + _T(".") + sindy::schema::rel_addr::kTableName20;

	// 住所紐付けテーブル（20桁コード）のテーブル取得
	if(S_OK != IFeatureWorkspacePtr(ipWork)->OpenTable(CComBSTR(strRelAddr), &m_ipRelAddr)){
		CLog::GetInstance().PrintLog2(false, true, false, true, err_type::fatal, err_code::NgOpenTable, strRelAddr, _T(""));
		return false;
	}

	// 市区町村代表点接続モードの住所DB接続先
	m_strAddrDB = args.m_addr_db.c_str();
	if(!m_strAddrDB.IsEmpty()){
		std::vector<CString> vecAddr = uh::str_util::split(m_strAddrDB, _T("@"));
		if(2 != vecAddr.size()){
			CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgAddrDBConnectString, m_strAddrDB);
			return false;
		}
		// MSのODBCを使用
		CString strConnect;
		strConnect.Format(_T("Driver={Microsoft ODBC for Oracle};Server=%s;UID=%s;PWD=%s;"), vecAddr[1], vecAddr[0], vecAddr[0]);
		if(!m_cADOBase.connect_by_string(strConnect)){
			CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgDBConnect, strConnect);
			return false;
		}
	}

	return true;
}

bool CConvertZipCodeData::run()
{
	// 住所紐付けテーブル変換
	CLog::GetInstance().PrintLog(true, false, true, false, _T("■住所紐付けテーブル変換"), true);
	CConvRelZipAddr cConvRelZipAddr(m_strOutDir, m_ipRelAddr, m_strAddr11Dir);
	if(!cConvRelZipAddr.init())
		return false;
	if(!cConvRelZipAddr.convert())
		return false;

	// 市区町村代表ポイント変換
	CLog::GetInstance().PrintLog(true, false, true, false, _T("■市区町村代表ポイント変換"), true);
	CConvRep cConvRep(m_strOutDir, m_ipRelAddr, m_strAddrDB, m_cADOBase);
	if(!cConvRep.init())
		return false;
	if(!cConvRep.convert())
		return false;

	// メモリを食うので不要データ解放
	cConvRelZipAddr.clearData();
	cConvRep.clearData();

	// 郵便番号ポリゴン変換
	CLog::GetInstance().PrintLog(true, false, true, false, _T("■郵便番号ポリゴン変換"), true);
	CConvZipPol cConvZipPol(m_strOutDir, m_ipZipAreaClass);
	if(!cConvZipPol.init())
		return false;
	if(!cConvZipPol.convert())
		return false;

	// 重心ポイント変換
	CLog::GetInstance().PrintLog(true, false, true, false, _T("■重心ポイント変換"), true);
	CConvCentroid cConvCentroid(m_strOutDir);
	if(!cConvCentroid.init())
		return false;
	cConvCentroid.setMergedZipGeom(cConvZipPol.getMergedZipGeom()); //!< マージ後の郵便番号ポリゴンを設定
	cConvZipPol.clearData();
	if(!cConvCentroid.convert())
		return false;

	return true;
}
