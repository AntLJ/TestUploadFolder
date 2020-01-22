#include "StdAfx.h"
#include "App.h"

#include "ListHandle.h"
#include <WinLib/VersionInfo.h>
#include <crd_cnv/coord_converter.h>
#include <sindy/workspace.h>

// ワークスペース取得
inline IWorkspacePtr getWorkspace( LPCTSTR lpcszDBProp )
{
	INamePtr ipName( sindy::create_workspace_name(lpcszDBProp) );
	if( NULL == ipName )
		return NULL;
	IUnknownPtr ipUnknown;
	ipName->Open( &ipUnknown );
	return ipUnknown;
}

CApp::CApp(void) : m_nListMode(-1), m_lpszTargetFCName(NULL)
{
}

CApp::~CApp(void)
{
}

// 使用法
void CApp::printUsage()
{
	std::cerr << "【リストからポイント追加ツール】\n"
			  << "(Usage)AddList2Point.exe [各種オプション] ( > 進捗ログファイル)\n"
			  << "・オプションについて\n"
			  << "  -i <IDリスト>     ... IDリストを指定(フォーマットはヘルプ参照)\n"
			  << "  -m <MeshXYリスト> ... MeshXYリストを指定(フォーマットはヘルプ参照)\n"
			  << "・環境変数について\n"
			  << "  DB_FROM  追加元のDB接続プロパティ(-i オプション指定時必須)\n"
			  << "  FC_FROM  追加元のフィーチャクラス名(オブジェクト名がFC_TOと必ず一緒でないといけない)\n"
			  << "  DB_TO    追加先のDB接続プロパティ\n"
			  << "  FC_TO    追加先のフィーチャクラス名\n" << std::endl;
}

// 初期化
bool CApp::init(int argc, char *argv[])
{
	if( argc < 3 )
		return false;

	for( int i=1;i<argc-1;++i )
	{
		if( strcmp(argv[i], "-i") == 0 ){
			m_nListMode = IDLIST_MODE;
			strncpy( m_szListFile, argv[++i], sizeof(m_szListFile) );
		}
		else if( strcmp(argv[i], "-m") == 0 ){
			m_nListMode = MXYLIST_MODE;
			strncpy( m_szListFile, argv[++i], sizeof(m_szListFile) );
		}
		else{
			std::cerr << "#ERROR 不明なオプション : " << argv[i] << std::endl;
			return false;
		}
	}

	std::cout.precision(12);

	return setDBInfoFromEnv();
}

// 実行
bool CApp::execute()
{
	// 編集開始
	if( !editStart() )
		return false;

	// 編集用ワークスペースで追加先フィーチャクラスを取得する
	((IFeatureWorkspacePtr)m_ipWorkspaceEdit)->OpenFeatureClass( CComBSTR(m_lpszTargetFCName), &m_ipTargetToFC );
	if( ! m_ipTargetToFC ){
		std::cerr << "#ERROR 追加先フィーチャクラスの取得に失敗 : " << m_lpszTargetFCName << std::endl;
		m_ipWorkspaceEdit->AbortEditOperation();
		return false;
	}

	// 追加先フィーチャクラスフィールド一覧(OID除く)作成＋ログヘッダー出力
	CString strLogHeader( _T("#FREESTYLELOG\nLAYER\tOBJECTID\tMSG") );
	IFieldsPtr ipFields;
	m_ipTargetToFC->get_Fields( &ipFields );
	long lFieldCount = 0;
	ipFields->get_FieldCount( &lFieldCount );	// フィールド数取得
	for( long i = 0; i < lFieldCount; ++i )
	{
		IFieldPtr ipField;
		ipFields->get_Field( i, &ipField );
		CComBSTR bstr;
		ipField->get_Name( &bstr );
		CString strField( bstr );
		if( strField != _T("OBJECTID") && strField != _T("SHAPE") )
		{
			m_vecTargetFCFID.push_back( i );
			strLogHeader.AppendFormat( _T("\t%s"), strField );
		}
	}
	CVersion cVer;
	strLogHeader.AppendFormat( _T("\n#%s FILEVERSION %s PRODUCTVERSION %s"), cVer.GetOriginalFilename(), cVer.GetFileVersion(), cVer.GetProductVersion() );
	std::cout << strLogHeader << std::endl;

	// 処理開始
	switch( m_nListMode )
	{
	case IDLIST_MODE:	addFromID();	break;
	case MXYLIST_MODE:	addFromMXY();	break;
	default:
		std::cerr << "#ERROR リストファイルを指定してください！！" << std::endl;
		break;
	}

	// 編集おしまい
	if( !editStop() )
		return false;

	return true;
}

////////////////////////////////////////
//	内部関数
////////////////////////////////////////

// 環境変数からＤＢ関係の情報取得
bool CApp::setDBInfoFromEnv()
{
#ifdef _DEBUG
	_tputenv( _T("DB_FROM=FIX201101/FIX201101/SDE.DEFAULT/5151/topaz2") );
	_tputenv( _T("FC_FROM=Gou_Point") );
	_tputenv( _T("DB_TO=SiNDYTEST/SiNDYTEST/SiNDYTEST.AddList2Point/5151/coral2") );
	_tputenv( _T("FC_TO=Gou_Point") );
#endif

	LPTSTR lpszDBProp = NULL;

	// IDリストモードなら、追加元に接続しフィーチャクラスも取得
	if( IDLIST_MODE == m_nListMode )
	{
		if( !(lpszDBProp = _tgetenv(_T("DB_FROM"))) ){
			std::cerr << "#ERROR 環境変数 DB_FROM の取得に失敗" << std::endl;
			return false;
		}
		LPTSTR lpszTargetFCName = NULL;
		if( !(lpszTargetFCName = _tgetenv(_T("FC_FROM"))) ){
			std::cerr << "#ERROR 環境変数 FC_FROM の取得に失敗" << std::endl;
			return false;
		}

		m_ipFromWorkspace = getWorkspace(lpszDBProp);
		if( ! m_ipFromWorkspace ){
			std::cerr << "#ERROR 追加元のSDE接続に失敗 : " << lpszDBProp << std::endl;
			return false;
		}
		std::cerr << "追加元DB接続 : " << lpszDBProp << std::endl;
		((IFeatureWorkspacePtr)m_ipFromWorkspace)->OpenFeatureClass( _bstr_t(lpszTargetFCName), &m_ipTargetFromFC );
		if( ! m_ipTargetFromFC ){
			std::cerr << "#ERROR 追加元フィーチャクラスの取得に失敗 : " << lpszTargetFCName << std::endl;
			return false;
		}
	}

	lpszDBProp = NULL;
	if( !(lpszDBProp = _tgetenv(_T("DB_TO"))) ){
		std::cerr << "#ERROR 環境変数 DB_TO の取得に失敗" << std::endl;
		return false;
	}
	if( !(m_lpszTargetFCName = _tgetenv(_T("FC_TO"))) ){
		std::cerr << "#ERROR 環境変数 FC_TO の取得に失敗" << std::endl;
		return false;
	}

	m_ipToWorkspace = getWorkspace(lpszDBProp);
	if( ! m_ipToWorkspace ){
		std::cerr << "#ERROR 追加先のSDE接続に失敗 : " << lpszDBProp << std::endl;
		return false;
	}
	std::cerr << "追加先DB接続 : " << lpszDBProp << std::endl;

	// 追加先フィーチャクラスは、編集ワークスペースで取得する

	return true;
}

// 編集開始
bool CApp::editStart()
{
	// 先に編集用子バージョン作成
	TCHAR szChildVerName[100] = _T("");
	SYSTEMTIME st;
	::GetLocalTime( &st );
	sprintf( szChildVerName, "AddList2Point_%04d%02d%02d%02d%02d%02d", 
			 st.wYear, st.wMonth, st.wDay,
			 st.wHour, st.wMinute, st.wSecond );
	IVersionedWorkspacePtr ipVerWorkspace( m_ipToWorkspace );
	if( NULL == ipVerWorkspace ){
		std::cerr << "#ERROR バージョン対応でありません" << std::endl;
		return false;
	}

	// DEFAULTバージョンはだめ！
	IVersionPtr ipCurVersion( m_ipToWorkspace );
	CComBSTR bstr;
	ipCurVersion->get_VersionName(&bstr);

	USES_CONVERSION;
	std::string strVer = OLE2A( bstr );
	if( strVer.find("DEFAULT") != -1 ){
		std::cerr << "#ERROR DEFAULTバージョンです。終了します : " << strVer << std::endl;
		return false;
	}

	// 既にそのバージョン名が存在するかチェック
	IVersionPtr ipChildVersion;
	ipVerWorkspace->FindVersion( CComBSTR(szChildVerName), &ipChildVersion );
	if( NULL == ipChildVersion ){
		ipCurVersion->CreateVersion( CComBSTR(szChildVerName), &ipChildVersion );
		if( NULL == ipChildVersion ){
			std::cerr << "#ERROR 子バージョンの作成に失敗 : " << szChildVerName << std::endl;
			return false;
		}
	}
	std::cerr << "子バージョン : " << szChildVerName << " の作成が完了\n" << std::endl;

	m_ipWorkspaceEdit = ipChildVersion;
	if( SUCCEEDED(m_ipWorkspaceEdit->StartEditing(VARIANT_FALSE)) && SUCCEEDED(m_ipWorkspaceEdit->StartEditOperation()) ){
		std::cerr << "編集を開始します" << std::endl;
		return true;
	}
	else{
		std::cerr << "#ERROR 編集を開始できませんでした" << std::endl;
		return false;
	}
}

// 編集終了
bool CApp::editStop()
{
	// 更新の後処理(編集を保存するなら VARIANT_TRUE にすること)
	if( SUCCEEDED(m_ipWorkspaceEdit->StopEditOperation()) && SUCCEEDED(m_ipWorkspaceEdit->StopEditing(VARIANT_TRUE)) ){
		std::cerr << "編集保存が完了" << std::endl;
		return true;
	}
	else{
		std::cerr << "#ERROR 編集の保存に失敗" << std::endl;
		m_ipWorkspaceEdit->AbortEditOperation();
		return false;
	}
}

// ＩＤリストから追加
void CApp::addFromID()
{
	std::vector<CIDPoint>	m_vecList;
	CListHandle cListHandle;
	if( !cListHandle.loadIDList(m_szListFile, m_vecList) ){
		std::cerr << "#ERROR IDリストの読み込みに失敗 : " << m_szListFile << std::endl;
		return;
	}

	// 挿入用フィーチャカーソル
	IFeatureCursorPtr ipInsFeatureCursor;
	if (FAILED(m_ipTargetToFC->Insert(VARIANT_TRUE, &ipInsFeatureCursor))) {
		std::cerr << "#ERROR インサートに失敗 : " << m_szListFile << std::endl;
		return;
	}

	int nAllCount = (signed)m_vecList.size();
	int nCount = 1;
	for( std::vector<CIDPoint>::const_iterator itr=m_vecList.begin();itr!=m_vecList.end();++itr )
	{
		std::cerr << nCount++ << " / " << nAllCount << " 件完了\r";

		// 追加元のフィーチャを取得
		IFeaturePtr ipFeature;
		m_ipTargetFromFC->GetFeature( itr->m_lOID, &ipFeature );
		if( ! ipFeature ){
			std::cout << "#ERROR 追加元からフィーチャが取得できない " << itr->m_lOID << std::endl;
			continue;
		}

		// 追加元フィーチャの属性をセット
		std::string strError;
		IFeatureBufferPtr ipFeatureBuffer;
		m_ipTargetToFC->CreateFeatureBuffer(&ipFeatureBuffer);
		if (!setAttribute(ipFeature, ipFeatureBuffer, strError)) {
			std::cout << "#ERROR " << strError << " " << itr->m_lOID << std::endl;
			continue;
		}

		// リストの指示に従って、属性値を変更
		if( !setAttribute(itr->m_mapVal, ipFeatureBuffer, strError) ){
			std::cout << "#ERROR " << strError << " " << itr->m_lOID << std::endl;
			continue;
		}

		// 共通属性値セット
		if( !setCommonAttribute(ipFeatureBuffer, strError) ){
			std::cout << "#ERROR " << strError << " " << itr->m_lOID << std::endl;
			continue;
		}

		// 追加
		CComVariant vaOID;
		if( SUCCEEDED(ipInsFeatureCursor->InsertFeature(ipFeatureBuffer, &vaOID)) )
		{
			PrintLog( vaOID.lVal, ipFeatureBuffer );
			ipInsFeatureCursor->Flush();
		}
		else
			std::cout << "#ERROR 追加に失敗" << std::endl;
	}
}

// MXYリストから追加
void CApp::addFromMXY()
{
	std::vector<CMXYPoint>	m_vecList;
	CListHandle cListHandle;
	if( !cListHandle.loadMXYList(m_szListFile, m_vecList) ){
		std::cerr << "#ERROR MXYリストの読み込みに失敗 : " << m_szListFile << std::endl;
		return;
	}

	// 挿入用フィーチャカーソル
	IFeatureCursorPtr ipInsFeatureCursor;
	m_ipTargetToFC->Insert( VARIANT_TRUE, &ipInsFeatureCursor );

	crd_cnv cCrdCnv;
	int nAllCount = (signed)m_vecList.size();
	int nCount = 1;
	for( std::vector<CMXYPoint>::const_iterator itr=m_vecList.begin();itr!=m_vecList.end();++itr )
	{
		std::cerr << nCount++ << " / " << nAllCount << " 件完了\r";

		d_lonlat cLonLat;
		int nResult = cCrdCnv.MeshtoLL( (*itr).m_nMeshCode, (*itr).m_nMeshX, (*itr).m_nMeshY, 
										&(cLonLat.lon), &(cLonLat.lat) );
		if( -1 == nResult ){
			std::cout << "#ERROR MESHXYから10進緯度経度に変換失敗 " 
					  << (*itr).m_nMeshCode << "," << (*itr).m_nMeshX << "," << (*itr).m_nMeshY << std::endl;
			continue;
		}
		IPointPtr ipPoint( CLSID_Point );
		ipPoint->PutCoords( cLonLat.lon, cLonLat.lat );

		IFeatureBufferPtr ipFeatureBuffer;
		m_ipTargetToFC->CreateFeatureBuffer( &ipFeatureBuffer );

		// デフォルト値を格納する
		LONG lFieldCount = 0;
		IFieldsPtr ipFields;
		ipFeatureBuffer->get_Fields( &ipFields );
		ipFields->get_FieldCount( &lFieldCount );
		for( LONG i = 0; i < lFieldCount; i++ )
		{
			IFieldPtr ipField;
			VARIANT_BOOL vaIsEditable = VARIANT_FALSE;

			ipFields->get_Field( i, &ipField );
			ipField->get_Editable( &vaIsEditable );
			// 編集可能フィールドのみ
			if( VARIANT_TRUE == vaIsEditable )
			{
				CComVariant vaDefaultValue;
				ipField->get_DefaultValue( &vaDefaultValue );
				ipFeatureBuffer->put_Value( i, vaDefaultValue );
			}
		}
		ipFeatureBuffer->putref_Shape( IGeometryPtr(ipPoint) );

		// リストの指示に従って、属性値を変更
		std::string strError;
		if( !setAttribute((*itr).m_mapVal, ipFeatureBuffer, strError) ){
			std::cout << "#ERROR " << strError << " "
				      << (*itr).m_nMeshCode << "," << (*itr).m_nMeshX << "," << (*itr).m_nMeshY << std::endl;
			continue;
		}

		// 共通属性値セット
		if( !setCommonAttribute(ipFeatureBuffer, strError) ){
			std::cout << "#ERROR " << strError << " " 
				      << (*itr).m_nMeshCode << "," << (*itr).m_nMeshX << "," << (*itr).m_nMeshY << std::endl;
			continue;
		}

		// 追加
		CComVariant vaOID;
		if( SUCCEEDED(ipInsFeatureCursor->InsertFeature(ipFeatureBuffer, &vaOID)) )
		{
			PrintLog( vaOID.lVal, ipFeatureBuffer );
			ipInsFeatureCursor->Flush();
		}
		else
			std::cout << "#ERROR 追加に失敗" << std::endl;
	}
}

bool CApp::setAttribute(IFeaturePtr ipSrcFeature, IFeatureBufferPtr ipFeatureBuffer, std::string& strError)
{
	IFieldsPtr ipFields;
	ipFeatureBuffer->get_Fields( &ipFields );

	long count(0);
	ipFields->get_FieldCount(&count);
	for (long i = 0; i < count; ++i)
	{
		IFieldPtr ipField;
		ipFields->get_Field(i, &ipField);
		esriFieldType type;
		ipField->get_Type(&type);

		if (type == esriFieldTypeOID) {
			continue;
		}
		else if (type == esriFieldTypeGeometry) {
			IGeometryPtr geo;
			ipSrcFeature->get_ShapeCopy(&geo);
			ipFeatureBuffer->putref_Shape(geo);
			continue;
		}

		CComVariant val;
		ipSrcFeature->get_Value(i, &val);

		if (FAILED(ipFeatureBuffer->put_Value(i, val)))
		{
			CComBSTR bstr;
			ipField->get_AliasName(&bstr);
			strError = bstr + "のセットに失敗";
			return false;
		}
	}

	return true;
}

// 属性値セット
bool CApp::setAttribute( const std::map<CString, CString>& mapVal, IFeatureBuffer* ipFeatureBuffer, std::string& strError )
{
	IFieldsPtr ipFields;
	ipFeatureBuffer->get_Fields( &ipFields );
	USES_CONVERSION;
	for( std::map<CString, CString>::const_iterator itr=mapVal.begin();itr!=mapVal.end();++itr )
	{
		LONG lFID = 0;
		ipFields->FindField( CComBSTR(_T(itr->first)), &lFID );

		if( !itr->second.IsEmpty() ){
			
			// 先にデータ型が何か調べる
			IFieldPtr ipField;
			ipFields->get_Field( lFID, &ipField );
			LONG varType;
			ipField->get_VarType( &varType );

			CComVariant vaValue;
			switch( varType )
			{
			case VT_BSTR:
				vaValue.vt = VT_BSTR;
				vaValue.bstrVal = T2BSTR(itr->second);
				break;
			case VT_I4:
				vaValue.vt = VT_I4;
				vaValue.lVal = atol(itr->second);
				break;
			case VT_DATE:
				vaValue.vt = VT_BSTR;
				vaValue.bstrVal = T2BSTR(itr->second);
				vaValue.ChangeType(VT_DATE);
				break;
			default:
				continue;
			}

			if( FAILED(ipFeatureBuffer->put_Value(lFID, vaValue)) ){
				strError = itr->first + "のセットに失敗";
				return false;
			}

		}
	}

	return true;
}

// 共通属性セット
bool CApp::setCommonAttribute( IFeatureBuffer* ipFeatureBuffer, std::string& strError )
{
	long lFID = 0;

	// 最終更新者(値が空なら、[sindy]にする)
	m_ipTargetToFC->FindField( CComBSTR(_T("OPERATOR")), &lFID );
	CComVariant vaValue;
	ipFeatureBuffer->get_Value( lFID, &vaValue );
	if( vaValue.vt == VT_EMPTY || vaValue.vt == VT_NULL
		|| (vaValue.vt == VT_BSTR && CString(vaValue.bstrVal) == _T(" ")) ){

		if( FAILED(ipFeatureBuffer->put_Value(lFID, CComVariant(OPERATOR))) ){
			strError = "OPERATORのセットに失敗";
			return false;
		}
	}

	// 最終更新内容(新規作成にする)
	m_ipTargetToFC->FindField( CComBSTR(_T("UPDATETYPE_C")), &lFID );
	if( FAILED(ipFeatureBuffer->put_Value(lFID, CComVariant(1))) ){
		strError = "UPDATETYPE_Cのセットに失敗";
		return false;
	}

	// 最終更新プログラム名
	m_ipTargetToFC->FindField( CComBSTR(_T("MODIFYPROGNAME")), &lFID );
	if( FAILED(ipFeatureBuffer->put_Value(lFID, CComVariant(PROGNAME))) ){
		strError = "MODIFYPROGNAMEのセットに失敗";
		return false;
	}

	// 最終プログラム更新日時
	TCHAR szTime[100] = {0};
	SYSTEMTIME st;
	::GetLocalTime( &st );
	_stprintf( szTime, _T("%04d/%02d/%02d %02d:%02d:%02d"), 
				st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );
	m_ipTargetToFC->FindField( CComBSTR(_T("PROGMODIFYDATE")), &lFID );
	if( FAILED(ipFeatureBuffer->put_Value(lFID, CComVariant(szTime))) ){
		strError = "PROGMODIFYDATEのセットに失敗";
		return false;
	}

	return true;
}

// ログ出力
void CApp::PrintLog( long lOID, IFeaturePtr ipFeature )
{
	std::cout << m_lpszTargetFCName << "\t" << lOID << "\t追加成功";

	// Bug8605 全属性出力
	for( std::vector<long>::const_iterator it = m_vecTargetFCFID.begin(); it != m_vecTargetFCFID.end(); ++it )
	{
		CComVariant vaValue;
		ipFeature->get_Value( *it, &vaValue );
		std::cout << "\t" << vaValue;
	}
	std::cout << std::endl;
}