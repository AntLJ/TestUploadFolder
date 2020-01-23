#include "stdafx.h"
#include "CopyOverlapFeatureAttr.h"
#include "FeatureObjects.h"
#include "Message.h"

// boost
#include <boost/algorithm/string/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm/find_if.hpp>

#include <sindy/workspace/name.h>	//!< create_workspace
#include <sindy/libschema.h>
#include <winlib/versioninfo.h>

#include <TDC/useful_headers/str_util.h>
#include <TDC/sindylib_core/Workspace.h>
#include <TDC/useful_headers/scope_guard.h>

#include <ArcHelperEx/AheglobalsMisc.h>
#include <ArcHelperEx/GlobalFunctions.h>
#include <ArcHelperEx/AheGlobalsCoord.h>
#include <ArcHelperEx/AheGeometryOp.h>
#include <ArcHelperEx/AheGeometryConvert.h>

#include <fstream>
#include <locale>
#include <codecvt>

static crd_cnv g_cnv;

const int CCopyOverlapFeatureAttr::max_retry_num = 5;          //!< 接続リトライ回数
const double CCopyOverlapFeatureAttr::max_retry_time = 3.0;    //!< 接続リトライ時間(秒数)
const TCHAR* childver_name_format = _T("ETC_COFA_%04d%02d%02d%02d%02d%02d");   //!< 子バージョン名のフォーマット

// 設定ファイルの項目名
namespace settings
{
	const wchar_t kSource[]              = L"source";
	const wchar_t kSourceTable[]         = L"sourcetable";
	const wchar_t kSourceField[]         = L"sourcefield";
	const wchar_t kSourceWhere[]         = L"sourcewhere";

	const wchar_t kTarget[]              = L"target";
	const wchar_t kTargetTable[]         = L"targettable";
	const wchar_t kTargetField[]         = L"targetfield";
	const wchar_t kTargetWhere[]         = L"targetwhere";

	const wchar_t kMesh[]                = L"mesh";
	const wchar_t kMeshTable[]           = L"meshtable";
	const wchar_t kMeshListPath[]        = L"meshlist";

	const wchar_t kExeMode[]             = L"exemode";
	const wchar_t kAttrListPath[]        = L"attrlist";
	const wchar_t kDatumFlag[]           = L"datumflag";
	const wchar_t kFieldFlag[]           = L"fieldflag";

	const wchar_t kOverlay[]             = L"overlay";
	const wchar_t kMagnification[]       = L"magnification";
	const wchar_t kReduction[]           = L"reduction";
	const wchar_t kBuffer[]              = L"buffer";
}

// 実行モード
namespace exemode
{
	const CString skipMode               = _T("skip");
	const CString matchMode              = _T("match");
}

// R-Treeから、与えた箱の範囲に引っかかるID群を取得
std::set<long> CCopyOverlapFeatureAttr::getIntersectsIDs( const BGBox& box, const BGRTree& rTree )
{
	std::set<long> retIDs;
	std::vector<std::pair<BGBox, unsigned int>> results;
	bgi::query( rTree, bgi::intersects( box ), std::back_inserter(results) );

	for( const auto& result : results )
		retIDs.insert( static_cast<long>( result.second ) );

	return retIDs;
}

// R-Treeを生成する
void CCopyOverlapFeatureAttr::createRTree( const std::map<long, GeoClass>& geoMap, BGRTree& rTree )
{
	for(const auto& geo : geoMap){
		long oID = geo.first;
		BGBox box = geo.second.GetBox();
		rTree.insert( std::make_pair( box, static_cast<unsigned int>(oID) ) );
	}
}

// 相対パスを絶対パスに変換
CString CCopyOverlapFeatureAttr::convertRelativePath( const CString& strPath )
{	
	CString strFullPath;
	TCHAR* pszFilePart = nullptr;

	GetFullPathName( strPath, MAX_PATH, strFullPath.GetBuffer( MAX_PATH ), &pszFilePart );
	strFullPath.ReleaseBuffer();

	return strFullPath;
}

// 設定情報の取得
bool CCopyOverlapFeatureAttr::getSettingInfo(const std::vector<std::wstring>& info)
{
	// info[0]がKey、info[1]がValue、Sizeが2以外の場合、未設定と見なす（正常値）
	if( info.size() != 2 )
		return true;

	std::wstring strKey   = info[0];
	std::wstring strValue = info[1];

	std::transform(strKey.begin(), strKey.end(), strKey.begin(), std::towlower);

	// 比較元の接続先
	if( strKey.compare( settings::kSource ) == 0 )
		m_setting.strSource = strValue.c_str();

	// 比較元のテーブルとフィールド名
	else if( strKey.compare( settings::kSourceField ) == 0 )
	{
		// 設定の内容が"."で分割(例えば、BUILDING_STEP.FLOORS)
		std::vector<std::wstring> values;
		boost::split( values, strValue, boost::is_any_of(L".") );

		// values[0]がテーブル名、values[1]がフィールド名、Sizeが2以外の場合、異常値
		if( values.size() != 2 )
			return false;

		m_setting.strSourceTable = values[0].c_str();
		m_setting.strSourceField = values[1].c_str();
	}

	// 比較元を指定するWHERE条件
	else if( strKey.compare( settings::kSourceWhere ) == 0 )
		m_setting.strSourceWhere = strValue.c_str();

	// 比較先の接続先
	else if( strKey.compare( settings::kTarget ) == 0 )
		m_setting.strTarget = strValue.c_str();

	// 比較先のテーブルとフィールド名
	else if( strKey.compare( settings::kTargetField ) == 0 )
	{
		// 設定の内容が"."で分割(例えば、BUILDING_STEP.FLOORS)
		std::vector<std::wstring> values;
		boost::split( values, strValue, boost::is_any_of(L".") );

		// values[0]がテーブル名、values[1]がフィールド名、Sizeが2以外の場合、異常値
		if( values.size() != 2 )
			return false;

		m_setting.strTargetTable = values[0].c_str();
		m_setting.strTargetField = values[1].c_str();
	}

	// 比較先を指定するWHERE条件
	else if( strKey.compare( settings::kTargetWhere ) == 0 )
		m_setting.strTargetWhere = strValue.c_str();

	// 処理単位(メッシュ)の接続先
	else if( strKey.compare( settings::kMesh ) == 0 )
		m_setting.strMesh = strValue.c_str();

	// 処理単位(メッシュ)のテーブル名
	else if( strKey.compare( settings::kMeshTable ) == 0 )
		m_setting.strMeshTable = strValue.c_str();

	// 処理範囲（メッシュリスト）のファイルパス
	else if( strKey.compare( settings::kMeshListPath ) == 0 )
	{
		CString tmpStrValue = strValue.c_str();
		if( PathIsRelative(tmpStrValue) )
			m_setting.strMeshListPath = convertRelativePath( strValue.c_str() );
		else
			m_setting.strMeshListPath = strValue.c_str();
	}

	// 実行モード
	else if( strKey.compare( settings::kExeMode ) == 0 )
		m_setting.strExeMode = strValue.c_str();

	// 属性変換表のファイルパス
	else if( strKey.compare( settings::kAttrListPath ) == 0 )
	{
		CString tmpStrValue = strValue.c_str();
		if( PathIsRelative(tmpStrValue) )
			m_setting.strAttrListPath = convertRelativePath( strValue.c_str() );
		else
			m_setting.strAttrListPath = strValue.c_str();
	}

	// 測地系強制変換フラグ
	else if( strKey.compare( settings::kDatumFlag ) == 0 )
	{
		std::transform(strValue.begin(), strValue.end(), strValue.begin(), std::towlower);

		if( strValue.compare( L"true" ) == 0 )
			m_setting.bDatumFlag = true;
		else if( strValue.compare( L"false" ) == 0 || strValue.empty() )
			m_setting.bDatumFlag = false;
		else
			return false;
	}

	// フィールド型強制変換フラグ
	else if( strKey.compare( settings::kFieldFlag ) == 0 )
	{
		std::transform(strValue.begin(), strValue.end(), strValue.begin(), std::towlower);

		if( strValue.compare( L"true" ) == 0 )
			m_setting.bFieldFlag = true;
		else if( strValue.compare( L"false" ) == 0 || strValue.empty() )
			m_setting.bFieldFlag = false;
		else
			return false;
	}

	// 重畳率の閾値
	else if( strKey.compare( settings::kOverlay ) == 0 )
	{
		// 未設定の場合、checkSettings()でチェック
		if( !strValue.empty() )
			m_setting.dOverlay = std::stod( strValue );
	}

	// 拡大率の閾値
	else if( strKey.compare( settings::kMagnification ) == 0 )
	{
		// 未設定の場合、checkSettings()でチェック
		if( !strValue.empty() )
			m_setting.dMagnification = std::stod( strValue );
	}

	// 縮小率の閾値
	else if( strKey.compare( settings::kReduction ) == 0 )
	{
		// 未設定の場合、checkSettings()でチェック
		if( !strValue.empty() )
			m_setting.dReduction = std::stod( strValue );
	}

	// バッファーの幅（メートル単位）
	else if( strKey.compare( settings::kBuffer ) == 0 )
	{
		// 未設定の場合、checkSettings()でチェック
		if( !strValue.empty() )
			m_setting.dBuffer = std::stod( strValue );
	}

	return true;
}


// 必須項目（文字列）のチェック
bool CCopyOverlapFeatureAttr::checkStringSetting(const std::wstring& strKey, const CString& strValue)
{
	CString message;
	if( strValue.IsEmpty() )
	{
		message = uh::str_util::format( message::getMsg(message::iRequiredSetting), strKey.c_str() );
		writeRunLog(message, false);
		return false;
	}

	return true;
}

// 必須項目（数字型）のチェック
bool CCopyOverlapFeatureAttr::checkNumericSetting(const std::wstring& strKey, double value)
{
	CString message;

	// 初期値のままだと、未設定と見なす
	if( value == -1.0 )
	{
		message = uh::str_util::format( message::getMsg(message::iRequiredSetting), strKey.c_str() );
		writeRunLog(message, false);
		return false;
	}

	// TODO: 閾値の定数化は要検討

	// 重畳率の設定値が0～100%以外の場合、エラー
	if( strKey.compare( settings::kOverlay ) == 0 )
	{
		if( value < 0 || value > 100 )
		{
			message = uh::str_util::format( message::getMsg(message::iAbnormalOverlay), value );
			writeRunLog(message, false);
			return false;
		}
	}

	// 拡大率の設定値が100%未満の場合、エラー
	else if( strKey.compare( settings::kMagnification ) == 0 )
	{
		if( value < 100 )
		{
			message = uh::str_util::format( message::getMsg(message::iAbnormalMagnification), value );
			writeRunLog(message, false);
			return false;
		}
	}

	// 縮小率の設定値が0～100%以外の場合、エラー
	else if( strKey.compare( settings::kMagnification ) == 0 )
	{
		if( value < 0 || value > 100 )
		{
			message = uh::str_util::format( message::getMsg( message::iAbnormalReduction ), value );
			writeRunLog( message, false );
			return false;
		}
	}

	// バッファーの設定値が0未満の場合、エラー
	else if( strKey.compare( settings::kBuffer ) == 0 )
	{
		if( value < 0 )
		{
			message = uh::str_util::format( message::getMsg(message::iAbnormalBuffer), value );
			writeRunLog(message, false);
			return false;
		}
	}

	return true;
}

// ファイルのチェック
bool CCopyOverlapFeatureAttr::checkFileSetting(const std::wifstream& ifs, const CString& strFilePath)
{
	CString message;
	if( ifs.fail() )
	{
		message = uh::str_util::format( message::getMsg(message::iCannotOpen), strFilePath );
		writeRunLog(message, false);
		return false;
	}

	if( ifs.eof() )
	{
		message = uh::str_util::format( message::getMsg(message::iEmptyFile), strFilePath );
		writeRunLog(message, false);
		return false;
	}

	return true;
}

// 設定内容のチェック
bool CCopyOverlapFeatureAttr::checkSettings()
{
	bool result = true;

	// 必須項目（文字列型）のチェック
	result &= checkStringSetting( settings::kSource,         m_setting.strSource );
	result &= checkStringSetting( settings::kSourceTable,    m_setting.strSourceTable );
	result &= checkStringSetting( settings::kSourceField,    m_setting.strSourceField );

	result &= checkStringSetting( settings::kTarget,         m_setting.strTarget );
	result &= checkStringSetting( settings::kTargetTable,    m_setting.strTargetTable );
	result &= checkStringSetting( settings::kTargetField,    m_setting.strTargetField );

	result &= checkStringSetting( settings::kMesh,           m_setting.strMesh );
	result &= checkStringSetting( settings::kMeshTable,      m_setting.strMeshTable );
	result &= checkStringSetting( settings::kMeshListPath,   m_setting.strMeshListPath );

	result &= checkStringSetting( settings::kExeMode,        m_setting.strExeMode );

	// 必須項目（数字型）のチェック
	result &= checkNumericSetting( settings::kOverlay,       m_setting.dOverlay );
	result &= checkNumericSetting( settings::kMagnification, m_setting.dMagnification );
	result &= checkNumericSetting( settings::kReduction,     m_setting.dReduction );
	result &= checkNumericSetting( settings::kBuffer,        m_setting.dBuffer );

	return result;
}

// 設定ファイルの読み込み
bool CCopyOverlapFeatureAttr::readSettingFile(const CString& strFilePath)
{
	std::wifstream  ifs( strFilePath );
	if( !checkFileSetting(ifs, strFilePath) )
		return false;

	// UTF-8で読み込む
	std::locale utf8_locale(std::locale(), new std::codecvt_utf8<wchar_t>);
	ifs.imbue( utf8_locale );

	std::wstring strLine;
	while( std::getline( ifs, strLine ) )
	{
		// 空行、コメント行除外
		if( strLine.empty() ) continue;
		if(	strLine.front() == L'#' ) continue;

		// 設定の内容が"=:"で分割(Where句の"="と区別するため、例えば、sourcewhere=:FLOORS = 0)
		std::vector<std::wstring> info;
		boost::split_regex( info, strLine, boost::wregex(L"=:") );
		if( !getSettingInfo(info) ) return false;
	}

	// 設定内容のチェック
	if( !checkSettings() )
		return false;

	return true;
}

// メッシュリストの読み込み
bool CCopyOverlapFeatureAttr::readMeshList(const CString& strFilePath)
{
	std::wifstream ifs( strFilePath );
	if( !checkFileSetting(ifs, strFilePath) )
		return false;

	// UTF-8で読み込む
	std::locale utf8_locale(std::locale(), new std::codecvt_utf8<wchar_t>);
	ifs.imbue( utf8_locale );

	std::wstring strLine;
	while( std::getline( ifs, strLine ) )
	{
		// 空行、コメント行除外
		if( strLine.empty() ) continue;
		if(	strLine.front() == L'#' ) continue;

		m_setMeshList.insert( std::stoul(strLine) );
	}

	return true;
}

// 属性変換表の読み込み
bool CCopyOverlapFeatureAttr::readAttributeList(const CString& strFilePath)
{
	std::wifstream ifs( strFilePath );
	if( !checkFileSetting(ifs, strFilePath) )
		return false;

	// UTF-8で読み込む
	std::locale utf8_locale(std::locale(), new std::codecvt_utf8<wchar_t>);
	ifs.imbue( utf8_locale );

	std::wstring strLine;
	while( std::getline( ifs, strLine ) )
	{
		// 空行、コメント行除外
		if( strLine.empty() ) continue;
		if(	strLine.front() == L'#' ) continue;

		// 比較元と比較先の属性値がタブで分割
		std::vector<std::wstring> info;
		boost::split( info, strLine, boost::is_any_of(L"\t") );

		// info[0]が比較元、info[1]が比較先、Sizeが2以外の場合、異常値
		if( info.size() != 2 )
			return false;

		m_mapAttrList.insert( std::pair<CString, CString>(info[0].c_str(), info[1].c_str()) );
	}

	return true;
}

// 実行ログファイルにオプション引数を出力する
void CCopyOverlapFeatureAttr::writeSettings()
{
	CString message;
	message = uh::str_util::format( message::getMsg(message::iWriteSettingInfo) );
	writeRunLog( message, false, true );

	// Source
	message = uh::str_util::format( message::getMsg(message::iWriteSource), m_setting.strSource );
	writeRunLog( message, false, true );
	message = uh::str_util::format( message::getMsg(message::iWriteSourceTable), m_setting.strSourceTable );
	writeRunLog( message, false, true );
	message = uh::str_util::format( message::getMsg(message::iWriteSourceField), m_setting.strSourceField );
	writeRunLog( message, false, true );
	message = uh::str_util::format( message::getMsg(message::iWriteSourceWhere), m_setting.strSourceWhere );
	writeRunLog( message, false, true );

	// Target
	message = uh::str_util::format( message::getMsg(message::iWriteTarget), m_setting.strTarget );
	writeRunLog( message, false, true );
	message = uh::str_util::format( message::getMsg(message::iWriteTargetTable), m_setting.strTargetTable );
	writeRunLog( message, false, true );
	message = uh::str_util::format( message::getMsg(message::iWriteTargetField), m_setting.strTargetField );
	writeRunLog( message, false, true );
	message = uh::str_util::format( message::getMsg(message::iWriteTargetWhere), m_setting.strTargetWhere );
	writeRunLog( message, false, true );

	// Mesh
	message = uh::str_util::format( message::getMsg(message::iWriteMesh), m_setting.strMesh );
	writeRunLog( message, false, true );
	message = uh::str_util::format( message::getMsg(message::iWriteMeshTable), m_setting.strMeshTable );
	writeRunLog( message, false, true );
	message = uh::str_util::format( message::getMsg(message::iWriteMeshListPath), m_setting.strMeshListPath );
	writeRunLog( message, false, true );

	// 他の情報
	message = uh::str_util::format( message::getMsg(message::iWriteExeMode), m_setting.strExeMode );
	writeRunLog( message, false, true );
	message = uh::str_util::format( message::getMsg(message::iWriteAttrListPath), m_setting.strAttrListPath );
	writeRunLog( message, false, true );
	message = uh::str_util::format( message::getMsg(message::iWriteDatumFlag), m_setting.bDatumFlag );
	writeRunLog( message, false, true );
	message = uh::str_util::format( message::getMsg(message::iWriteFieldFlag), m_setting.bFieldFlag );
	writeRunLog( message, false, true );
	message = uh::str_util::format( message::getMsg(message::iWriteOverlay), m_setting.dOverlay );
	writeRunLog( message, false, true );
	message = uh::str_util::format( message::getMsg(message::iWriteMagnification), m_setting.dMagnification );
	writeRunLog( message, false, true );
	message = uh::str_util::format( message::getMsg( message::iWriteReducation ), m_setting.dReduction );
	writeRunLog( message, false, true );
	message = uh::str_util::format( message::getMsg(message::iWriteBuffer), m_setting.dBuffer );
	writeRunLog( message, false, true );
}

bool CCopyOverlapFeatureAttr::createChildVer( const IWorkspacePtr& ipWorkspace )
{
	// 子バージョン名作成
	SYSTEMTIME systemTime;
	::GetLocalTime( &systemTime );
	CString strChildVer;
	strChildVer.Format( childver_name_format, systemTime.wYear, systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond );

	// 子バージョン作成
	IVersionPtr ipCurVersion( ipWorkspace );
	IVersionPtr ipChildVersion;
	ipCurVersion->CreateVersion( _bstr_t( strChildVer ), &ipChildVersion );

	CString message;
	if( !ipChildVersion )
	{
		message.Format( _T( "%s : %s" ), message::getMsg( message::iErrorCreateVersion ), strChildVer );
		writeRunLog(message, false);
		return false;
	}

	// 作成した子バージョンにコメント追加
	ipChildVersion->put_Description( _bstr_t( message::getMsg(message::iChildVerComment) ) );

	m_ipWorkspaceEdit = ( IWorkspaceEditPtr )ipChildVersion;
	if( !m_ipWorkspaceEdit )
	{
		message = uh::str_util::format( message::getMsg(message::iErrorEditWorkspace) );
		writeRunLog(message, false);
		return false;
	}
	else
	{
		message.Format( _T( "%s : %s" ), message::getMsg( message::iSuccessCreateVersion ), strChildVer );
		writeRunLog(message, false);
	}
	return true;
}

// ワークスペースオープン
bool CCopyOverlapFeatureAttr::openWorkspace( const CString& name, IWorkspacePtr& ipWorkspace )
{
	// DBに接続(接続リトライあり3秒5回)
	ipWorkspace = sindy::create_workspace( name, true, max_retry_num, max_retry_time );
	if( !ipWorkspace )
		return false;

	return true;
}

// DBのオープン処理
bool CCopyOverlapFeatureAttr::openDB()
{
	CString message;

	// 比較元のワークスペースオープン
	if( !openWorkspace(m_setting.strSource, m_ipSourceWorkspace) )
	{
		message = uh::str_util::format( message::getMsg(message::iNotConnectDB), m_setting.strSource );
		writeRunLog(message, false);
		return false;
	}

	// 比較先のワークスペースオープン
	if( !openWorkspace(m_setting.strTarget, m_ipTargetWorkspace) )
	{
		message = uh::str_util::format( message::getMsg(message::iNotConnectDB), m_setting.strTarget );
		writeRunLog(message, false);
		return false;
	}

	// 比較先がSDEの場合は子バージョンを作成する
	sindy::CWorkspace cWork(m_ipTargetWorkspace);
	if ( cWork.GetNameString()->IsSDE() )
	{
		if( !createChildVer(m_ipTargetWorkspace) )
			return false;
	}
	else
		m_ipWorkspaceEdit = m_ipTargetWorkspace;

	// メッシュのワークスペースオープン
	if( !openWorkspace(m_setting.strMesh, m_ipMeshWorkspace) )
	{
		message = uh::str_util::format( message::getMsg(message::iNotConnectDB), m_setting.strMesh );
		writeRunLog(message, false);
		return false;
	}
	return true;
}

// フィーチャクラスを取得
bool CCopyOverlapFeatureAttr::openFeatureClass(const CString& name, const IWorkspacePtr& ipWorkspace, IFeatureClassPtr& ipFeatureClass)
{
	IFeatureWorkspacePtr ipFeatureWork(ipWorkspace);

	if(FAILED(ipFeatureWork->OpenFeatureClass(CComBSTR(name), &ipFeatureClass)))
		return false;

	return true;
}

// 入力フィーチャクラスを取得
bool CCopyOverlapFeatureAttr::openFC()
{
	CString message;

	// 比較元のフィーチャクラスを取得
	if( !openFeatureClass( m_setting.strSourceTable, m_ipSourceWorkspace, m_ipSourceFC ) )
	{
		message = uh::str_util::format( message::getMsg(message::iNotOpenFC), m_setting.strSourceTable );
		writeRunLog(message, false);
		return false;
	}

	// 比較先のフィーチャクラスを取得（編集用ワークスペースを使用）
	if( !openFeatureClass( m_setting.strTargetTable, m_ipWorkspaceEdit, m_ipTargetFC ) )
	{
		message = uh::str_util::format( message::getMsg(message::iNotOpenFC), m_setting.strTargetTable );
		writeRunLog(message, false);
		return false;
	}

	// メッシュのフィーチャクラスを取得
	if( !openFeatureClass( m_setting.strMeshTable, m_ipMeshWorkspace, m_ipMeshFC ) )
	{
		message = uh::str_util::format( message::getMsg(message::iNotOpenFC), m_setting.strMeshTable );
		writeRunLog(message, false);
		return false;
	}
	return true;
}

// 各フィールドのIndexを取得
bool CCopyOverlapFeatureAttr::getFieldsIndex()
{
	CString message;
	using namespace sindy::schema;

	// フィールドMeshCodeのIndex
	m_nMeshCodeIndex = -1;
	if( FAILED( m_ipMeshFC->FindField(CComBSTR(category::mesh_code_table::kMeshCode), &m_nMeshCodeIndex) ) )
	{
		message = uh::str_util::format( message::getMsg(message::iNotGetIndex), category::mesh_code_table::kMeshCode );
		writeRunLog(message, false);
		return false;
	}

	// 比較元フィールドのIndex
	m_nSourceFieldIndex = -1;
	if( FAILED( m_ipSourceFC->FindField(CComBSTR(m_setting.strSourceField), &m_nSourceFieldIndex) ) )
	{
		message = uh::str_util::format( message::getMsg(message::iNotGetIndex), m_setting.strSourceField );
		writeRunLog(message, false);
		return false;
	}

	// 比較先フィールドのIndex
	m_nTargetFieldIndex = -1;
	if( FAILED( m_ipTargetFC->FindField(CComBSTR(m_setting.strTargetField), &m_nTargetFieldIndex) ) )
	{
		message = uh::str_util::format( message::getMsg(message::iNotGetIndex), m_setting.strTargetField );
		writeRunLog(message, false);
		return false;
	}

	// Shapeのフィールド名が10文字以下でなければならないという制限があるので、
	// 下記3フィールド（UpdateType、ModifyProgName、ProgModifyDate）が存在しない可能性がある

	// 最終更新内容のIndex
	m_nUpdateTypeIndex = -1;
	if( FAILED( m_ipTargetFC->FindField(CComBSTR(global::common_table::kUpdateType), &m_nUpdateTypeIndex) ) )
	{
		message = uh::str_util::format( message::getMsg(message::iNotGetIndex), global::common_table::kUpdateType );
		writeRunLog(message, false);
	}

	// 最終更新プログラム名のIndex
	m_nModifyProgNameIndex = -1;
	if( FAILED( m_ipTargetFC->FindField(CComBSTR(global::common_table::kModifyProgName), &m_nModifyProgNameIndex) ) )
	{
		message = uh::str_util::format( message::getMsg(message::iNotGetIndex), global::common_table::kModifyProgName );
		writeRunLog(message, false);
	}

	// 最終プログラム更新日時のIndex
	m_nProgModifyDateIndex = -1;
	if( FAILED( m_ipTargetFC->FindField(CComBSTR(global::common_table::kProgModifyDate), &m_nProgModifyDateIndex) ) )
	{
		message = uh::str_util::format( message::getMsg(message::iNotGetIndex), global::common_table::kProgModifyDate );
		writeRunLog(message, false);
	}

	return true;
}

// 初期化
bool CCopyOverlapFeatureAttr::init(const Arguments& args)
{
	CString message;

	// 設定ファイルの読み込み
	if( !readSettingFile(args.m_setting.c_str()) )
	{
		message = uh::str_util::format( message::getMsg(message::iReadFailed), args.m_setting.c_str() );
		writeRunLog(message, false);
		return false;
	}

	// メッシュリストの読み込み
	if( !readMeshList(m_setting.strMeshListPath) )
	{
		message = uh::str_util::format( message::getMsg(message::iReadFailed), m_setting.strMeshListPath );
		writeRunLog(message, false);
		return false;
	}

	// 属性変換表の読み込み（必須ではない）
	if( !m_setting.strAttrListPath.IsEmpty() )
	{
		if( !readAttributeList(m_setting.strAttrListPath) )
		{
			message = uh::str_util::format( message::getMsg(message::iReadFailed), m_setting.strAttrListPath );
		writeRunLog(message, false);
			return false;
		}
	}

	// ログファイルを開く(CloseはLogWriterのデストラクタで呼び出すのでこの関数内では呼ばない)
	if( !openLogFiles(args.m_runlog.c_str(), args.m_errlog.c_str(), args) )
		return false;

	// 設定情報の出力
	writeSettings();

	return true;
}

// メッシュリストより、メッシュとバッファーのポリゴンを取得
bool CCopyOverlapFeatureAttr::getMeshPolygons(std::map<unsigned long, std::pair<IGeometryPtr, IGeometryPtr>>& mapMeshGeo)
{
	CString message;
	using namespace sindy::schema;

	CString strSubFields;
	strSubFields.Format( _T("%s,%s"), category::mesh_code_table::kMeshCode, feature::kShape );

	IFeatureCursorPtr ipFeatureCursor;
	if( FAILED(m_ipMeshFC->Search(AheInitQueryFilter( nullptr, strSubFields, nullptr ), VARIANT_FALSE, &ipFeatureCursor)) )
	{
		message = uh::str_util::format( message::getMsg(message::iErrorMeshSearch), m_setting.strAttrListPath );
		writeRunLog(message, false);
		return false;
	}

	IFeaturePtr ipFeature;
	while ( S_OK == (ipFeatureCursor->NextFeature(&ipFeature)) && ipFeature )
	{
		CComVariant vaMeshCode = 0;
		IGeometryPtr ipMeshGeom;
		ipFeature->get_Value(m_nMeshCodeIndex, &vaMeshCode);
		ipFeature->get_Shape(&ipMeshGeom);

		// TERTIARYMESHのメッシュコードが10桁で、lValで取得できないため、dblValで取得する
		unsigned long ulMeshCode = 0;
		if( 0 == m_setting.strMeshTable.CompareNoCase( global::citymesh::kTableName ) )
			ulMeshCode = vaMeshCode.dblVal;
		else
			ulMeshCode = vaMeshCode.lVal;

		if( m_setMeshList.find(ulMeshCode) != m_setMeshList.end() )
		{
			// メッシュ毎にメートルから緯度経度の距離に変換
			double bufferSize = 0.0;
			IUnitConverterPtr ipUnitConv( CLSID_UnitConverter );
			ipUnitConv->ConvertUnits( m_setting.dBuffer, esriMeters, esriDecimalDegrees, &bufferSize );

			// 指定距離のBufferを作成
			IGeometryPtr ipBuffer;
			if( FAILED( ((ITopologicalOperatorPtr)ipMeshGeom)->Buffer( bufferSize, &ipBuffer ) ))
				continue;;

			mapMeshGeo[ulMeshCode] = std::make_pair( ipMeshGeom, ipBuffer );
			m_setMeshList.erase(ulMeshCode);
		}
	}

	// 存在しないメッシュコードが指定されたかのチェック
	if( m_setMeshList.size() > 0 )
	{
		for( const auto& mesh : m_setMeshList )
		{
			message = uh::str_util::format( message::getMsg(message::iNotExistMeshCode), mesh );
			writeRunLog(message, false);
		}
	}

	return true;
}

// 比較元のジオメトリを取得
bool CCopyOverlapFeatureAttr::getSourceObjectInMesh( const IGeometryPtr& ipMeshGeo, std::map<long, GeoClass>& mapSource)
{
	CString message;
	using namespace sindy::schema;

	IFeatureCursorPtr ipCursor;
	if( FAILED( m_ipSourceFC->Search( AheInitQueryFilter( AheInitSpatialFilter( NULL, ipMeshGeo ), NULL, m_setting.strSourceWhere ), VARIANT_FALSE, &ipCursor ) ) || !ipCursor )
	{
		message = uh::str_util::format( message::getMsg(message::iErrorSourceSearch) );
		writeRunLog(message, false);
		return false;
	}


	IFeaturePtr ipFeature;
	while( S_OK == ipCursor->NextFeature( &ipFeature ) && ipFeature )
	{
		GeoClass SourceGeoClass;
		SourceGeoClass.oid         = ipFeature->GetOID();
		SourceGeoClass.geometry    = ipFeature->GetShapeCopy();
		SourceGeoClass.attr.first  = m_setting.strSourceField;
		SourceGeoClass.attr.second = uh::str_util::ToString( ipFeature->GetValue( m_nSourceFieldIndex ) );

		// 測地系強制変換フラグがTrue、且つ測地系が一致していない場合、比較先の測地系を真として更新する
		if( m_setting.bDatumFlag && !isSameSpRef() )
			SourceGeoClass.geometry->putref_SpatialReference(m_ipTargetSpRef);

		mapSource.insert( std::make_pair(SourceGeoClass.oid, SourceGeoClass) );
	}

	return true;
}

// 重畳率を取得
double CCopyOverlapFeatureAttr::getOverlay( const IGeometryPtr& ipSourceGeo, const IGeometryPtr& ipTargetGeo)
{
	// 重畳率は、比較元と比較先のフィーチャ重複部分が、比較先フィーチャ形状の何%を占めるかを表した数字。
	// 0%～100%の値区分となる。
	double overlay = -1.0;

	esriGeometryType sourceType = esriGeometryNull;
	ipSourceGeo->get_GeometryType( &sourceType );
	esriGeometryType targetType = esriGeometryNull;
	ipTargetGeo->get_GeometryType( &targetType );

	if( (sourceType != esriGeometryPoint && sourceType != esriGeometryPolyline && sourceType != esriGeometryPolygon)
		|| (targetType != esriGeometryPoint && targetType != esriGeometryPolyline && targetType != esriGeometryPolygon) )
	{
		// 想定外のジオメトリ
		CString message = uh::str_util::format( message::getMsg(message::iErrUnexpectedGeometryType) );
		writeRunLog(message, false);
		overlay = -1.0;
		return overlay;
	}

	// 比較元     比較先     重畳率						
	// ポリゴン   ポリゴン   重複部分の面積 / ポリゴン（比較先）の面積 * 100 [%]						
	// ポリゴン   ポリライン 重複部分の長さ / ポリラインの全長 * 100 [%]						
	// ポリゴン   ポイント   ポリゴン内にポイントがあれば（接する場合も含む）100%、それ以外は0%						
	// ポリライン ポリゴン   重複部分の長さ / ポリラインの全長 * 100 [%]						
	// ポリライン ポリライン 重複部分の長さ / ポリライン（比較先）の全長 * 100 [%]						
	// ポリライン ポイント   ポリライン上にポイントがあれば100%、それ以外は0%						
	// ポイント   ポリゴン   ポリゴン内にポイントがあれば（接する場合も含む）100%、それ以外は0%						
	// ポイント   ポリライン ポリライン上にポイントがあれば100%、それ以外は0%						
	// ポイント   ポイント   ポイントとポイントが一致していれば100%、それ以外は0%						

	IGeometryPtr ipIntersectGeo;

	// 比較先がポイントの場合、比較元とWithinで判定する
	switch (targetType)
	{
	case esriGeometryPoint:
		{
			// ポイントがポリゴンの辺上にあった場合、又はポイントがポリラインの端点にあった場合、AheIsWithin2()がFalseを返すため、
			// AheIsDisjoint判定で重畳率を判定する
			if( AheIsDisjoint(ipTargetGeo, ipSourceGeo) )
				overlay = 0.0;
			else
				overlay = 100.0;
			break;
		}
	case esriGeometryPolyline:
		{
			if( sourceType == esriGeometryPoint )
			{
				// ポイントがポリラインの端点にあった場合、AheIsWithin2()がFalseを返すため、
				// AheIsDisjoint判定で重畳率を判定する
				if( AheIsDisjoint(ipTargetGeo, ipSourceGeo) )
					overlay = 0.0;
				else
					overlay = 100.0;
			}
			else
			{
				ipIntersectGeo = AheIntersect(ipSourceGeo, ipTargetGeo, esriGeometry1Dimension);
				if( !ipIntersectGeo )
				{
					overlay = 0.0;
					break;
				}
				ipIntersectGeo->putref_SpatialReference(m_ipTargetSpRef);

				// 一致なら100%にする
				// Shapeなど座標精度の無い場合には、長さや面積計算で座標精度のある場合に比べて微小な差が生まれる
				// ただ、その時でもジオメトリ自体は一致するため、100%とみなす
				if( AheIsEqual( ipTargetGeo, ipIntersectGeo ) )
				{
					overlay = 100.0;
					break;
				}

				double intersectLength = 0.0, baseLength = 0.0;
				IPolylinePtr(ipIntersectGeo)->get_Length(&intersectLength);
				IPolylinePtr(ipTargetGeo)->get_Length(&baseLength);

				overlay = (intersectLength / baseLength) * 100;
			}
			break;
		}
	case esriGeometryPolygon:
		{
			// 比較先がポリゴンの場合、交差している部分の次元は比較元と同じ
			if( sourceType == esriGeometryPoint )
			{
				// ポイントがポリゴンの辺上にあった場合、AheIsWithin2()がFalseを返すため、
				// AheIsDisjoint判定で重畳率を判定する
				if( AheIsDisjoint(ipSourceGeo, ipTargetGeo) )
					overlay = 0.0;
				else
					overlay = 100.0;
			}
			else if( sourceType == esriGeometryPolyline )
			{
				ipIntersectGeo = AheIntersect(ipSourceGeo, ipTargetGeo, esriGeometry1Dimension);
				if( !ipIntersectGeo )
				{
					overlay = 0.0;
					break;
				}
				ipIntersectGeo->putref_SpatialReference(m_ipTargetSpRef);

				// 一致なら100%にする
				if( AheIsEqual( ipTargetGeo, ipIntersectGeo ) )
				{
					overlay = 100.0;
					break;
				}

				double intersectLength = 0.0, baseLength = 0.0;
				IPolylinePtr(ipIntersectGeo)->get_Length(&intersectLength);
				IPolylinePtr(ipSourceGeo)->get_Length(&baseLength);

				overlay = (intersectLength / baseLength) * 100;
			}
			else
			{
				ipIntersectGeo = AheIntersect(ipSourceGeo, ipTargetGeo, esriGeometry2Dimension);
				if( !ipIntersectGeo )
				{
					overlay = 0.0;
					break;
				}
				ipIntersectGeo->putref_SpatialReference(m_ipTargetSpRef);

				// 一致なら100%にする
				if( AheIsEqual( ipTargetGeo, ipIntersectGeo ) )
				{
					overlay = 100.0;
					break;
				}

				double intersectArea = 0.0, baseArea = 0.0;
				IAreaPtr(ipIntersectGeo)->get_Area(&intersectArea);
				IAreaPtr(ipTargetGeo)->get_Area(&baseArea);

				overlay = (intersectArea / baseArea) * 100;
			}
			break;
		}
	default:
		{
			overlay = -1.0;
			break;
		}
	}

	return overlay;
}

// 閾値以上の比較元との重畳率を取得
std::multimap<double, GeoClass> CCopyOverlapFeatureAttr::getAllOverlay(const IGeometryPtr& ipTargetGeo, const std::set<long>& sourceIDs, const std::map<long, GeoClass>& mapSource, ErrItem& errItem)
{
	// 重畳率の同じ比較元が複数存在する可能性があるため、std::multimapを使用する
	std::multimap<double, GeoClass> mapSourceOverlay;

	for( const auto& findSourceID : sourceIDs )
	{
		auto findSourceItr = mapSource.find( findSourceID );
		if( findSourceItr == mapSource.end() )
			continue;

		double overlay = getOverlay( findSourceItr->second.geometry, ipTargetGeo );

		// 閾値以上は処理対象とする
		// 理論上は100%を超えることはあり得ない
		if( m_setting.dOverlay <= overlay && overlay <= 100 )
			mapSourceOverlay.insert( std::make_pair(overlay, findSourceItr->second) );
		else
		{
			errItem.nSrcOID = findSourceID;
			errItem.strNote.Format( _T("重畳率:%.15lf, 閾値:%.15lf～100"), overlay, m_setting.dOverlay );

			setDetailErrorInfo( err_code::errFailureOverlay, errItem );
			writeErrLog( errItem, false );
			resetDetailErrorInfo( errItem );
		}
	}
	return mapSourceOverlay;
}

// 比較元との拡縮率を見て満たすものを取得
std::set<long> CCopyOverlapFeatureAttr::getAllOkScale(const IGeometryPtr& ipTargetGeo, const std::set<long>& sourceIDs, const std::map<long, GeoClass>& mapSource, ErrItem& errItem)
{
	std::set<long> setSourceIDs;

	for( const auto& findSourceID : sourceIDs )
	{
		auto findSourceItr = mapSource.find( findSourceID );
		if( findSourceItr == mapSource.end() )
			continue;

		// 比較元      比較先      拡縮率	
		// ポリゴン    ポリゴン    比較元の面積 / 比較先の面積 * 100 [%]
		// ポリライン  ポリライン  比較元の全長 / 比較先の全長 * 100 [%]

		IGeometryPtr ipSourceGeo = findSourceItr->second.geometry;

		esriGeometryType sourceType = esriGeometryNull;
		ipSourceGeo->get_GeometryType( &sourceType );
		esriGeometryType targetType = esriGeometryNull;
		ipTargetGeo->get_GeometryType( &targetType );

		// 比較元と比較先が両方ともポリライン、或いは両方ともポリゴンの場合のみ、拡縮率を計算する必要がある
		double scale = 0.0;
		if( sourceType == esriGeometryPolyline && targetType == esriGeometryPolyline )
		{
			// 一致なら100%にする
			if( AheIsEqual( ipTargetGeo, ipSourceGeo ) )
			{
				scale = 100.0;
			}
			else
			{
				// 比較先が分母のため、1.0と初期化する
				double sourceLength = 0.0, targetLength = 1.0;
				IPolylinePtr( ipSourceGeo )->get_Length( &sourceLength );
				IPolylinePtr( ipTargetGeo )->get_Length( &targetLength );

				scale = ( sourceLength / targetLength ) * 100;
			}
		}
		else if( sourceType == esriGeometryPolygon && targetType == esriGeometryPolygon )
		{
			// 一致なら100%にする
			if( AheIsEqual( ipTargetGeo, ipSourceGeo ) )
			{
				scale = 100.0;
			}
			else
			{
				// 比較先が分母のため、1.0と初期化する
				double sourceArea = 0.0, targetArea = 1.0;
				IAreaPtr( ipSourceGeo )->get_Area( &sourceArea );
				IAreaPtr( ipTargetGeo )->get_Area( &targetArea );

				scale = ( sourceArea / targetArea ) * 100;
			}
		}
		// 上記以外の場合、全比較元が処理対象となるため、拡縮率を100%と設定する
		else
			scale = 100.0;

		// 縮小率以上、拡大率以下の値区分となる
		if( m_setting.dReduction <= scale && scale <= m_setting.dMagnification )
			setSourceIDs.insert(findSourceID);
		else
		{
			errItem.nSrcOID = findSourceID;
			errItem.strNote.Format( _T("拡縮率:%.15lf, 閾値:%.15lf～%.15lf"), scale, m_setting.dReduction, m_setting.dMagnification);

			setDetailErrorInfo( err_code::errFailureScale, errItem );
			writeErrLog( errItem, false );
			resetDetailErrorInfo( errItem );
		}

	}
	return setSourceIDs;
}

// フィールド値を指定値に変換
CComVariant CCopyOverlapFeatureAttr::convertFieldValue(const CString& strValue)
{
	CComVariant rtnValue(strValue);

	esriFieldType targetFieldType = m_ipTargetField->GetType();

	switch (targetFieldType)
	{
	case esriFieldTypeSmallInteger:
		rtnValue.ChangeType(VT_I4);
		break;
	case esriFieldTypeInteger:
		rtnValue.ChangeType(VT_I8);
		break;
	case esriFieldTypeSingle:
		rtnValue.ChangeType(VT_R4);
		break;
	case esriFieldTypeDouble:
		rtnValue.ChangeType(VT_R8);
		break;
	case esriFieldTypeString:
		rtnValue.ChangeType(VT_BSTR);
		break;
	default:
		{
			// 想定外のフィールド型
			CString message = uh::str_util::format( message::getMsg(message::iErrUnexpectedFieldType) );
			writeRunLog(message, false);
		}
		break;
	}

	return rtnValue;
}

// 出力用の座標値を取得
bool CCopyOverlapFeatureAttr::getShapeCoordinate(const IGeometryPtr& shape, ErrItem& errItem)
{
	switch(shape->GetGeometryType())
	{
	case esriGeometryType::esriGeometryPoint:
		{
			IPointPtr point = shape;
			if( !point ) return false;

			errItem.dLon = point->GetX();
			errItem.dLat = point->GetY();
		}
		break;
	case esriGeometryType::esriGeometryPolyline:
		{
			IPointCollectionPtr pointColl( shape );
			if( !pointColl ) return false;

			long pointNum = pointColl->GetPointCount();
			if( !pointNum )	return false;

			IPointPtr point = pointColl->GetPoint(pointNum/2);
			if ( !point ) return false;

			errItem.dLon = point->GetX();
			errItem.dLat = point->GetY();
		}
		break;
	case esriGeometryType::esriGeometryPolygon:
		{
			IAreaPtr ipArea( shape );
			if ( !ipArea ) return false;

			IPointPtr point;
			ipArea->get_Centroid( &point );
			if ( !point ) return false;

			errItem.dLon = point->GetX();
			errItem.dLat = point->GetY();
		}
		break;
	default:
		return false;
	}
	return true;
}

// エラー出力用の共通情報を取得
ErrItem CCopyOverlapFeatureAttr::getCommonErrorInfo(unsigned long meshCode, const GeoClass& target)
{
	ErrItem errItem;
	errItem.strLayer  = m_setting.strTargetTable;
	errItem.nOID      = target.oid;
	errItem.nMeshCode = meshCode;

	// 出力用の座標情報を取得
	getShapeCoordinate( target.geometry, errItem );

	return errItem;
}

// エラー出力用の詳細情報を設定
void CCopyOverlapFeatureAttr::setDetailErrorInfo(err_code::ECode errCode, ErrItem& errItem)
{
	switch (errCode)
	{
	case err_code::errUpdateFailed:
	case err_code::errPutValueFailed:
		errItem.strErrLevel = getErrLevelMsg( err_level::levelError );
		break;
	case err_code::errOverBuffer:
	case err_code::errSrcNotExist:
	case err_code::errOverlaySrcNotExist:
	case err_code::errScaleSrcNotExist:
	case err_code::errBothSrcNotExist:
	case err_code::errMulitSrcSkipMode:
	case err_code::errMulitSrcAttrNoMatch:
	case err_code::errAttrNotInAttrList:
	case err_code::errNotSameField:
	case err_code::errFailureOverlay:
	case err_code::errFailureScale:
		errItem.strErrLevel = getErrLevelMsg( err_level::levelWarning );
		break;
	case err_code::errSuccessUpdate:
		errItem.strErrLevel = getErrLevelMsg( err_level::levelInfo );
		break;
	default:
		errItem.strErrLevel = getErrLevelMsg( err_level::levelInfo );
		break;
	}
	errItem.nErrCode = errCode;
	errItem.strErrMsg = getErrCodeMsg(errCode);
	return;
}

// エラー出力用の詳細情報をリセット
void CCopyOverlapFeatureAttr::resetDetailErrorInfo(ErrItem& errItem)
{
	errItem.strErrLevel   = _T("");
	errItem.nErrCode      = 0;
	errItem.strErrMsg     = _T("");
	errItem.nSrcOID       = -1;
	errItem.strNote       = _T("");
}

// 対象フィールドを更新
bool CCopyOverlapFeatureAttr::putValues(const CString& strValue, IFeaturePtr& ipFeature, ErrItem& errItem)
{
	using namespace sindy::schema;

	// 対象フィールドを更新する
	if( FAILED( ipFeature->put_Value( m_nTargetFieldIndex, convertFieldValue(strValue) ) ) )
	{
		errItem.strNote = m_setting.strTargetField;
		setDetailErrorInfo( err_code::errPutValueFailed, errItem );
		writeErrLog( errItem, false );
		resetDetailErrorInfo( errItem );
		return false;
	}

	// 下記最終更新系の３フィールドに対し、Shape、FGDBデータには存在しない可能性があるため、存在しているフィールドだけ更新する
	if( -1 != m_nUpdateTypeIndex )
	{
		if( FAILED( ipFeature->put_Value( m_nUpdateTypeIndex, CComVariant( global::common_table::gl_chg_code::kAttrModify ) ) ) )
		{
			errItem.strNote = global::common_table::kUpdateType;
			setDetailErrorInfo( err_code::errPutValueFailed, errItem );
			writeErrLog( errItem, false );
			resetDetailErrorInfo( errItem );
			return false;
		}
	}

	if( -1 != m_nModifyProgNameIndex )
	{
		if( FAILED( ipFeature->put_Value( m_nModifyProgNameIndex, CComVariant( CVersion().GetInternalName() ) ) ) )
		{
			errItem.strNote = global::common_table::kModifyProgName;
			setDetailErrorInfo( err_code::errPutValueFailed, errItem );
			writeErrLog( errItem, false );
			resetDetailErrorInfo( errItem );
			return false;
		}
	}

	if( -1 != m_nProgModifyDateIndex )
	{
		if( FAILED( ipFeature->put_Value( m_nProgModifyDateIndex, CComVariant( getNowTime().c_str() ) ) ) )
		{
			errItem.strNote = global::common_table::kProgModifyDate;
			setDetailErrorInfo( err_code::errPutValueFailed, errItem );
			writeErrLog( errItem, false );
			resetDetailErrorInfo( errItem );
			return false;
		}
	}
	return true;
}

// 比較先のフィールドを更新
bool CCopyOverlapFeatureAttr::updateTargetField(const std::pair<unsigned long, std::pair<IGeometryPtr, IGeometryPtr>>& meshGeoRec, const std::map<long, GeoClass>& mapSource, const BGRTree& sourceRTree)
{
	CString message;
	using namespace sindy::schema;

	// メッシュ内の比較先を検索する
	IFeatureCursorPtr ipUpdateCursor;
	if( FAILED( m_ipTargetFC->Update( AheInitQueryFilter( AheInitSpatialFilter( NULL, meshGeoRec.second.first ), NULL, m_setting.strTargetWhere ), VARIANT_FALSE, &ipUpdateCursor ) ) || !ipUpdateCursor )
	{
		message = uh::str_util::format( message::getMsg(message::iErrorTargetSearch) );
		writeRunLog(message, false);
		return false;
	}

	long nCount = 0; // 比較先の件数（出力用）
	IFeaturePtr ipFeature;
	while( S_OK == ipUpdateCursor->NextFeature( &ipFeature ) && ipFeature )
	{
		++nCount;

		GeoClass targetGeoClass;
		targetGeoClass.oid         = ipFeature->GetOID();

		// 処理済みの場合、スキップ（メッシュを跨ぐジオメトリに対し、重複処理を避けるために）
		if( m_setProcessedOID.find(targetGeoClass.oid) != m_setProcessedOID.end() )
			continue;

		targetGeoClass.geometry    = ipFeature->GetShapeCopy();
		targetGeoClass.attr.first  = m_setting.strTargetField;
		targetGeoClass.attr.second = uh::str_util::ToString( ipFeature->GetValue( m_nTargetFieldIndex ) );

		ErrItem errItem = getCommonErrorInfo( meshGeoRec.first, targetGeoClass );

		// 比較先のジオメトリが比較元の検索範囲（バッファーの範囲）をはみ出した場合、ログを出力し、スキップする
		IRelationalOperatorPtr ipRel( targetGeoClass.geometry.m_T );
		VARIANT_BOOL isWithin = VARIANT_FALSE;
		ipRel->Within(meshGeoRec.second.second, &isWithin );
		if( isWithin != VARIANT_TRUE)
		{
			setDetailErrorInfo( err_code::errOverBuffer, errItem );
			writeErrLog( errItem, false );
			resetDetailErrorInfo( errItem );
			continue;
		}

		// 引っかかる比較元のID群を取得
		//（比較元のポリラインが比較先のポリゴンの辺の上にあった場合、getIntersectsIDsが0を返すため、漏れないようにGetBoxを0.1m拡大する）
		std::set<long> sourceIDs = getIntersectsIDs( targetGeoClass.GetBox(0.1), sourceRTree );

		// 重なっている比較元が存在しない
		if( sourceIDs.empty() )
		{
			setDetailErrorInfo( err_code::errSrcNotExist, errItem );
			writeErrLog( errItem, false );
			resetDetailErrorInfo( errItem );
			continue;
		}

		// 比較元との重畳率を取得（重畳率の同じ比較元が複数存在する可能性があるため、std::multimapを使用する）
		std::multimap<double, GeoClass> mapSourceOverlay;  // Key:重畳率; Value:比較元のジオメトリとフィールド値等を格納するクラス
		mapSourceOverlay = getAllOverlay( targetGeoClass.geometry, sourceIDs, mapSource, errItem );

		// 重畳率の閾値を満たす比較元が存在しない
		if( mapSourceOverlay.empty() )
		{
			setDetailErrorInfo( err_code::errOverlaySrcNotExist, errItem );
			writeErrLog( errItem, false );
			resetDetailErrorInfo( errItem );
			continue;
		}

		// 拡縮率を満たす比較元のOIDを取得
		std::set<long> setSourceScale;
		setSourceScale = getAllOkScale( targetGeoClass.geometry, sourceIDs, mapSource, errItem );

		// 拡大率と縮小率の閾値を満たす比較元が存在しない
		if( setSourceScale.empty() )
		{
			setDetailErrorInfo( err_code::errScaleSrcNotExist, errItem );
			writeErrLog( errItem, false );
			resetDetailErrorInfo( errItem );
			continue;
		}

		// 重畳率と拡大率を両方とも満たす比較元を取得
		std::multimap<double, GeoClass>::iterator sourceItr;
		for( sourceItr = mapSourceOverlay.begin(); sourceItr != mapSourceOverlay.end(); )
		{
			if( setSourceScale.find(sourceItr->second.oid) == setSourceScale.end() )
				sourceItr = mapSourceOverlay.erase( sourceItr );
			else
				++sourceItr;
		}

		// 重畳率と拡大率を両方とも満たす比較元が存在しない
		if( mapSourceOverlay.empty() )
		{
			setDetailErrorInfo( err_code::errBothSrcNotExist, errItem );
			writeErrLog( errItem, false );
			resetDetailErrorInfo( errItem );
			continue;
		}

		// 重畳率と拡大率を満たす比較元に対し、最も重畳率が高いフィーチャで更新する
		// （mapSourceOverlayが重畳率の昇順でソートされるため、最後尾の重畳率が最も高い）
		std::multimap<double, GeoClass>::reverse_iterator rItr;
		rItr = mapSourceOverlay.rbegin();

		long maxOverlayOID = rItr->second.oid;
		double maxOverlay = rItr->first;
		CString maxAttrValue = rItr->second.attr.second;

		bool bUpdate = true; // 更新必要フラグ

		// 二番目から比較する
		++rItr;
		for( ; rItr != mapSourceOverlay.rend(); ++rItr )
		{
			if( maxOverlay > rItr->first )
				break;

			// 最も重畳率が高いフィーチャが2つ以上、且つ実行モードがスキップモードの場合、更新しない
			if( maxOverlay == rItr->first && 0 == m_setting.strExeMode.CompareNoCase(exemode::skipMode) )
			{
				errItem.nSrcOID = rItr->second.oid;
				errItem.strNote.Format( _T("フィーチャー1のOID:%d, 重畳率:%f, フィーチャー2のOID:%d, 重畳率:%f"),
					maxOverlayOID, maxOverlay, rItr->second.oid, rItr->first );

				setDetailErrorInfo( err_code::errMulitSrcSkipMode, errItem );
				writeErrLog( errItem, false );
				resetDetailErrorInfo( errItem );
				bUpdate = false;
				break;
			}

			// 最も重畳率が高いフィーチャが2つ以上、且つ属性値が一致していない場合、更新しない
			if( maxOverlay == rItr->first && maxAttrValue != rItr->second.attr.second )
			{
				errItem.nSrcOID = rItr->second.oid;
				errItem.strNote.Format( _T("フィーチャー1のOID:%d, 属性値:%s, フィーチャー2のOID:%d, 属性値:%s"),
					maxOverlayOID, maxAttrValue, rItr->second.oid, rItr->second.attr.second );

				setDetailErrorInfo( err_code::errMulitSrcAttrNoMatch, errItem );
				writeErrLog( errItem, false );
				resetDetailErrorInfo( errItem );
				bUpdate = false;
				break;
			}
		}

		// 更新する必要がない場合、スキップ
		if( !bUpdate )
			continue;

		// 属性変換表が指定された場合、属性変換表により更新する(key:比較元の属性値、value:比較先の属性値)
		if( !m_mapAttrList.empty() )
		{
			std::map<CString, CString>::iterator attrItr;
			attrItr = m_mapAttrList.find( maxAttrValue );

			// 変換表に値がない場合は更新スキップ
			if( attrItr == m_mapAttrList.end() )
			{
				setDetailErrorInfo( err_code::errAttrNotInAttrList, errItem );
				writeErrLog( errItem, false );
				resetDetailErrorInfo( errItem );
				continue;
			}
			else
				maxAttrValue = attrItr->second;
		}

		// 対象フィールドを更新
		if( !updateField( maxOverlayOID, maxAttrValue, ipFeature, errItem ) )
			continue;

		if( FAILED(ipUpdateCursor->UpdateFeature(ipFeature)) )
		{
			// 更新失敗
			setDetailErrorInfo( err_code::errUpdateFailed, errItem );
			writeErrLog( errItem, false );
			resetDetailErrorInfo( errItem );
		}
		else
		{
			// 正常更新
			errItem.nSrcOID = maxOverlayOID;
			setDetailErrorInfo( err_code::errSuccessUpdate, errItem );
			writeErrLog( errItem, false );
			resetDetailErrorInfo( errItem );
		}

		// 重複処理を避けるために、処理済みの比較先を記録する
		m_setProcessedOID.insert(targetGeoClass.oid);
	}

	// 比較先が存在しない
	if( 0 == nCount )
	{
		message = uh::str_util::format( message::getMsg(message::iNotExistTarget) );
		writeRunLog(message, false);
	}
	return true;
}

// 測地系のチェック
bool CCopyOverlapFeatureAttr::checkDatum()
{
	IGeoDatasetPtr(m_ipSourceFC)->get_SpatialReference(&m_ipSourceSpRef);
	IGeoDatasetPtr(m_ipTargetFC)->get_SpatialReference(&m_ipTargetSpRef);

	// 測地系強制変換フラグがFalse、且つ比較元と比較先の測地系が一致していない場合、処理を中止する
	if( !m_setting.bDatumFlag && !isSameSpRef() )
	{
		CString message = uh::str_util::format( message::getMsg( message::iNotSameSpRef ) );
		writeRunLog( message, false );
		return false;
	}
	return true;
}

// フィールド型が一致しているかをチェック
bool CCopyOverlapFeatureAttr::isSameField()
{
	if( !m_ipSourceField || !m_ipTargetField )
		return false;

	// タイプ、幅、桁数をチェック
	esriFieldType sourceFieldType = m_ipSourceField->GetType();
	esriFieldType targetFieldType = m_ipTargetField->GetType();

	if( sourceFieldType != targetFieldType )
		return false;

	if( m_ipSourceField->GetLength() != m_ipTargetField->GetLength() )
		return false;

	if( m_ipSourceField->GetPrecision() != m_ipTargetField->GetPrecision() )
		return false;

	if( m_ipSourceField->GetScale() != m_ipTargetField->GetScale() )
		return false;

	return true;
}

// 測地系が一致しているかをチェック
bool CCopyOverlapFeatureAttr::isSameSpRef()
{
	if( !m_ipSourceSpRef || !m_ipTargetSpRef )
		return false;

	long lSrcFcCode = m_ipSourceSpRef->GetFactoryCode();
	long lTrgFcCode = m_ipTargetSpRef->GetFactoryCode();

	return ( lSrcFcCode == lTrgFcCode );
}

// フィールドのタイプが処理対象であるかをチェック
bool CCopyOverlapFeatureAttr::isProcessType( const esriFieldType& fieldType )
{
	// 整数型、小数型、文字列のみ処理対象である
	switch (fieldType)
	{
	case esriFieldTypeSmallInteger:
	case esriFieldTypeInteger:
	case esriFieldTypeSingle:
	case esriFieldTypeDouble:
	case esriFieldTypeString:
		return true;
	default:
		return false;
	}
}

// フィールドのタイプが処理対象であるかをチェック
bool CCopyOverlapFeatureAttr::isProcessType()
{
	if( !m_ipSourceField || !m_ipTargetField )
		return false;

	esriFieldType sourceFieldType = m_ipSourceField->GetType();
	esriFieldType targetFieldType = m_ipTargetField->GetType();

	if( !isProcessType(sourceFieldType) || !isProcessType(targetFieldType) )
		return false;

	return true;
}

// フィールド型のチェック
bool CCopyOverlapFeatureAttr::checkFieldType()
{
	IFieldsPtr ipSourceFields = m_ipSourceFC->GetFields();
	IFieldsPtr ipTargetFields = m_ipTargetFC->GetFields();

	ipSourceFields->get_Field( m_nSourceFieldIndex, &m_ipSourceField );
	ipTargetFields->get_Field( m_nTargetFieldIndex, &m_ipTargetField );

	// 比較元と比較先のフィールド型が処理対象外の場合、処理を中止する
	if( !isProcessType() )
	{
		CString message = uh::str_util::format( message::getMsg(message::iErrUnexpectedFieldType) );
		writeRunLog(message, false);
		return false;
	}

	// フィールド型強制変換フラグがFalse、且つ比較元と比較先のフィールド型が一致していない場合、処理を中止する
	if( !m_setting.bFieldFlag )
	{
		if( !isSameField() )
		{
			CString message = uh::str_util::format( message::getMsg(message::iNotSameField) );
			writeRunLog(message, false);
			return false;
		}
	}

	return true;
}

// 対象フィールドを更新
bool CCopyOverlapFeatureAttr::updateField( long maxOverlayOID, const CString& maxAttrValue, IFeaturePtr& ipFeature, ErrItem& errItem )
{
	// 比較先と比較元のフィールドの型が一致していない場合、警告を出力する
	if( !isSameField() )
	{
		errItem.nSrcOID = maxOverlayOID;
		setDetailErrorInfo( err_code::errNotSameField, errItem );
		writeErrLog( errItem, false );
		resetDetailErrorInfo( errItem );
	}

	// 比較先と比較元の測地系が一致していない場合、警告を出力する
	if( !isSameSpRef() )
	{
		errItem.nSrcOID = maxOverlayOID;
		setDetailErrorInfo( err_code::errNotSameSpRef, errItem );
		writeErrLog( errItem, false );
		resetDetailErrorInfo( errItem );
	}

	// 対象フィールドを更新する
	if( !putValues(maxAttrValue, ipFeature, errItem) )
		return false;

	return true;
}

// 実行
bool CCopyOverlapFeatureAttr::execute(const Arguments& args)
{
	// 初期化
	if( !init(args) )
		return false;

	try
	{
		// DBのオープン
		if( !openDB() )
			return false;

		// フィーチャクラスのオープン
		if( !openFC() )
			return false;

		// 各フィールドのIndexを取得
		if( !getFieldsIndex() )
			return false;

		// 測地系強制変換フラグのチェック
		if( !checkDatum() )
			return false;

		// フィールド型強制変換フラグのチェック
		if( !checkFieldType() )
			return false;

		// 処理範囲のメッシュを取得
		std::map<unsigned long, std::pair<IGeometryPtr, IGeometryPtr>> mapMeshGeo;  // Key:メッシュコード; Value:メッシュ形状,バッファー形状
		if( !getMeshPolygons(mapMeshGeo) )
			return false;

		CString message;
		if( mapMeshGeo.empty() )
		{
			message = uh::str_util::format( message::getMsg(message::iEmptyMeshGeo) );
			writeRunLog(message, false);
			return false;
		}

		// ここのスコープを抜けたら強制的にabort
		// (このabortは、編集開始されているときだけ有効になるような実装になっている)
		uh::scope_guard scope( std::bind( DatabaseUtility::abortEdit, m_ipWorkspaceEdit, message ) );
		if( !DatabaseUtility::startEdit(m_ipWorkspaceEdit, message) )
		{
			writeRunLog(message, false);
			return false;
		}
		else
		{
			message = uh::str_util::format( message::getMsg(message::iSuccessStartEditing) );
			writeRunLog(message, false);
		}

		// メッシュ単位で処理する
		for( const auto& meshGeoRec : mapMeshGeo )
		{
			// 処理開始のログを出力
			message = uh::str_util::format( message::getMsg(message::iStartMesh), meshGeoRec.first );
			writeRunLog(message);

			// バッファー範囲内の比較元ジオメトリを取得（メッシュを跨ぐジオメトリが存在する可能性を考慮するため）
			std::map<long, GeoClass> mapSource;  // Key:比較元OID; Value:比較元のジオメトリとフィールド値等を格納するクラス
			if( !getSourceObjectInMesh( meshGeoRec.second.second, mapSource) )
				continue;

			// 比較元が存在しない
			if( mapSource.empty() )
			{
				message = uh::str_util::format( message::getMsg(message::iNotExistSource) );
				writeRunLog(message, false);
				continue;
			}

			// R-Treeを作成する
			BGRTree sourceRTree;
			createRTree( mapSource, sourceRTree );

			// 更新する
			if( !updateTargetField( meshGeoRec, mapSource, sourceRTree ) )
				continue;

		}

		// 編集終了
		if( !DatabaseUtility::stopEdit(m_ipWorkspaceEdit, message) )
		{
			writeRunLog(message, false);
			return false;
		}
		else
		{
			message = uh::str_util::format( message::getMsg(message::iSuccessStopEditing) );
			writeRunLog(message, false);
		}
	}
	catch(const std::exception& e)
	{
		m_setMeshList.clear();
		m_mapAttrList.clear();
		m_setProcessedOID.clear();
		return false;
	}

	return true;
}