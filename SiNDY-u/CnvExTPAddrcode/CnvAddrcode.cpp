/*
 * Copyright (C) INCREMENT P CORP. All Rights Reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY INCREMENT P CORP., WITHOUT WARRANTY OF
 * ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT.
 *
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDER BE LIABLE FOR ANY
 * CLAIM, DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING
 * OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.
*/

#include "StdAfx.h"
#include "CnvAddrcode.h"
#include <sindy/workspace.h>

/**
 * @brief	住所DB接続
 * @param	strDBProp	[in]	接続プロパティ
 * @param	cDatabase	[out]	データソース
 * @return	bool
 */
inline bool getDataSource( const CString& strDBProp, ODatabase& cDatabase )
{
	CString str(strDBProp);

	int nPos = 0;
	CString strUser		= str.Tokenize( "/", nPos );
	CString strPass		= str.Tokenize( "@", nPos );
	CString strDBName	= str.Tokenize( "@", nPos );

	return OSUCCESS == cDatabase.Open( strDBName, strUser, strPass );
}


CCnvAddrcode::CCnvAddrcode(void) : 
	m_ipWorkspace(NULL),
	m_ipEditWorkspace(NULL),
	m_bEditMode(false),
	m_lAddrcode_FID(0),
	m_lAddrName_FID(0),
	m_lPointStatC_FID(0),
	m_lInfoSrcC_FID(0),
	m_lProgModifyDate_FID(0),
	m_lModifyProgName_FID(0)
{
}

CCnvAddrcode::~CCnvAddrcode(void)
{
}

// 使用法
void CCnvAddrcode::printUsage()
{
	std::cerr << "【拡張電話番号検索ポイント住所コード置換ツール】\n"
			  << "(Usage)CnvExTPAddrcode.exe [オプション] ( > ログファイル)\n"
			  << "・必須オプション\n"
			  << "  -c  [ref/update] ... 動作モードの指定(ref : 更新箇所抽出  update : 更新モード)\n"
			  << "・追加オプション\n"
			  << "  -l  [仮コード対応表]  ... 指定時には、住所マスタで対応できないものに対して、リストを元に可能なものは置換します。\n"
			  << "・環境変数について\n"
			  << "  DB_ADDRESS       住所系SDE接続プロパティ\n"
			  << "  FC_EXTP_POINT    拡張電話番号検索ポイントフィーチャクラス\n"
			  << "  JDB_CONNECT_NEW  (新)住所マスタ\n"
			  << "  JDB_CONNECT_OLD  (旧)住所マスタ" << std::endl;
}

// 初期化
bool CCnvAddrcode::init(int argc, char *argv[])
{
	// 引数の数チェック
	if( argc < 3 )
	{
		std::cerr << "#ERROR オプションの数が正しくない" << std::endl;
		return false;
	}

	for( int i=1;i<argc-1;++i )
	{
		if( strcmp(argv[i], "-c") == 0 )
		{
			++i;
			if( strcmp(argv[i], "ref") == 0 )			m_bEditMode = false;
			else if( strcmp(argv[i], "update") == 0 )	m_bEditMode = true;

		}
		else if( strcmp(argv[i], "-l") == 0 )
		{
			// 仮コード対応表読み込み
			if( !m_cTmpTable.load(argv[++i]) )
			{
				std::cerr << "#ERROR 仮コード対応表の読み込みに失敗 : " << argv[i] << std::endl;
				return false;
			}
		}
		else
		{
			std::cerr << "#ERROR 不正なオプションです : " << argv[i] << std::endl;
			return false;
		}
	}

	return setInfoFromEnv();	// 環境変数から、必要情報の取得
}

// 実行
bool CCnvAddrcode::execute()
{
	if( m_bEditMode )
	{
		// 編集モードなら
		std::cerr << "実行モード : 編集" << std::endl;
		if( !editStart() )
			return false;

		IFeatureWorkspacePtr(m_ipEditWorkspace)->OpenTable( CComBSTR(_T(m_strExTPFName)), &m_ipExTPTable );
		if( NULL == m_ipExTPTable )
		{
			std::cerr << "#ERROR 編集ワークスペースから、拡張電話番号検索ポイントテーブルの取得に失敗 : " << m_strExTPFName << std::endl;
			return false;
		}
	}
	else
		std::cerr << "実行モード : 参照(編集は行いません)" << std::endl;

	printHeader();

	// レコード取得
	LONG lAllCount = 0;
	m_ipExTPTable->RowCount( NULL, &lAllCount );
	_ICursorPtr ipCursor;
	if( FAILED(m_ipExTPTable->Search(NULL, VARIANT_FALSE, &ipCursor)) )
	{
		std::cerr << "#ERROR レコードの取得に失敗" << std::endl;
		m_ipEditWorkspace->AbortEditOperation();
		return false;
	}
	
	_IRowPtr ipRow;
	LONG lCount = 1;
	while( ipCursor->NextRow(&ipRow) == S_OK )
	{
		std::cerr << lCount++ << " / " << lAllCount << " 件完了\r";

		editExTPPoint( ipRow );	// 編集
	}
	
	std::cerr << std::endl;

	if( m_bEditMode && !editStop() )
	{
		return false;
	}
	else
		std::cerr << "完了" << std::endl;

	return true;
}

/**
 * 内部関数
 */

// 環境変数から必要情報の設定
bool CCnvAddrcode::setInfoFromEnv()
{
#ifdef _DEBUG
	_tputenv( _T("DB_ADDRESS=techmap/techmap/SDE.ExTP_Work/5151/spinel2") );
	_tputenv( _T("FC_EXTP_POINT=ExTP_POINT") );
	_tputenv( _T("JDB_CONNECT_NEW=k0703/k0703@delltcp") );
	_tputenv( _T("JDB_CONNECT_OLD=y0611e/y0611e@delltcp") );
#endif

	// 環境変数取得
	CString strDBProp, strJDBPropNew, strJDBPropOld;
	if( !strDBProp.GetEnvironmentVariable( _T("DB_ADDRESS") ) || 
		!strJDBPropNew.GetEnvironmentVariable( _T("JDB_CONNECT_NEW") ) ||
		!strJDBPropOld.GetEnvironmentVariable( _T("JDB_CONNECT_OLD") ) ||
		!m_strExTPFName.GetEnvironmentVariable( _T("FC_EXTP_POINT") ) )
	{
		std::cerr << "#ERROR 環境変数が取得できないものがありました。確認してください。" << std::endl;
		return false;
	}

	// 住所DB接続
	if( !getDataSource(strJDBPropNew, m_cJDBNew) )
	{
		std::cerr << "#ERROR (新)住所DBへの接続に失敗 : " << strJDBPropNew << std::endl;
		return false;
	}
	std::cerr << "#(新)住所DBへ接続 : " << strJDBPropNew << std::endl;
	if( !getDataSource(strJDBPropOld, m_cJDBOld) )
	{
		std::cerr << "#ERROR (旧)住所DBへの接続に失敗 : " << strJDBPropOld << std::endl;
		return false;
	}
	std::cerr << "#(旧)住所DBへ接続 : " << strJDBPropOld << std::endl;

	// 住所コード置換用テーブル作成
	std::cerr << "住所コード置換用テーブル作成中・・・";
	if( !makeAddrcodeMap() || !makeTmpcodeMap() )
	{
		std::cerr << "#ERROR 住所コード置換用テーブルの作成に失敗" << std::endl;
		return false;
	}
	std::cerr << "作成完了\n" << std::endl;

	// SDE接続
	INamePtr ipName( sindy::create_workspace_name(strDBProp) );
	if( ipName )
	{
		IUnknownPtr ipUnknown;
		ipName->Open( &ipUnknown );
		m_ipWorkspace = ipUnknown;
	}
	if( NULL == m_ipWorkspace )
	{
		std::cerr << "#Error SDE接続に失敗 : " << strDBProp << std::endl;
		return false;
	}
	std::cerr << "#SDE接続 : " << strDBProp << std::endl;

	// 拡張電話番号検索ポイントテーブル取得
	IFeatureWorkspacePtr(m_ipWorkspace)->OpenTable( CComBSTR(m_strExTPFName), &m_ipExTPTable );
	if( NULL == m_ipExTPTable )
	{
		std::cerr << "#ERROR 拡張電話番号検索ポイントテーブルの取得に失敗 : " << m_strExTPFName << std::endl;
		return false;
	}

	// そのまま住所コードフィールドインデックスも取得
	m_ipExTPTable->FindField( CComBSTR(_T("ADDRCODE")), &m_lAddrcode_FID );
	m_ipExTPTable->FindField( CComBSTR(_T("ADDRNAME")), &m_lAddrName_FID );
	m_ipExTPTable->FindField( CComBSTR(_T("INFOSRC_C")), &m_lInfoSrcC_FID );
	m_ipExTPTable->FindField( CComBSTR(_T("POINTSTAT_C")), &m_lPointStatC_FID );
	m_ipExTPTable->FindField( CComBSTR(_T("PROGMODIFYDATE")), &m_lProgModifyDate_FID );
	m_ipExTPTable->FindField( CComBSTR(_T("MODIFYPROGNAME")), &m_lModifyProgName_FID );

	return true;
}

// 11桁住所コードがキーで、最新11桁住所コードが値のmapコンテナを作成
bool CCnvAddrcode::makeAddrcodeMap()
{
	CString strSQL( "select distinct ken_code || shi_code || oaza_code || aza_code, "
					"getnewestcode(ken_code || shi_code || oaza_code || aza_code) from areacode_master" );

	ODynaset cDynaset( m_cJDBNew, strSQL );
	if( cDynaset.GetRecordCount() == 0 )
	{
		std::cerr << "#ERROR SQLの実行に失敗 : " << strSQL << std::endl;
		return false;
	}

	OValue cVal1, cVal2;
	do
	{
		cDynaset.GetFieldValue( 0, &cVal1 );
		cDynaset.GetFieldValue( 1, &cVal2 );

		m_mapAddrcode.insert( std::pair<CString, CString>(CString(cVal1), CString(cVal2)) );	// mapに格納

		cDynaset.MoveNext();

	}while( !cDynaset.IsEOF() );

	return true;
}

// 仮コードがキーで、旧コードが値のmapコンテナ作成
bool CCnvAddrcode::makeTmpcodeMap()
{
	CString strSQL( "select distinct ken_code || shi_code || oaza_code || aza_code, "
					"getprevcode(ken_code || shi_code || oaza_code || aza_code) from areacode_master "
					"where shusei_code='+'" );

	ODynaset cDynaset( m_cJDBOld, strSQL );
	if( cDynaset.GetRecordCount() == 0 )
	{
		std::cerr << "#ERROR SQLの実行に失敗 : " << strSQL << std::endl;
		return false;
	}

	OValue cVal1, cVal2;
	do
	{
		cDynaset.GetFieldValue( 0, &cVal1 );
		cDynaset.GetFieldValue( 1, &cVal2 );

		m_mapTmpcode.insert( std::pair<CString, CString>(CString(cVal1), CString(cVal2)) );	// mapに格納

		cDynaset.MoveNext();

	}while( !cDynaset.IsEOF() );
	
	return true;
}

// 編集用子バージョン作成
bool CCnvAddrcode::makeChildVersion()
{
	// 子バージョン名は要検討(とりあえず時間名)
	SYSTEMTIME st;
	::GetLocalTime( &st );
	CString strChildVersion;
	strChildVersion.Format( _T("CnvExTPAddrcode_%04d%02d%02d_%02d%02d%02d"), 
							st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );
	IVersionedWorkspacePtr ipVerWorkspace( m_ipWorkspace );
	if( NULL == ipVerWorkspace )
	{
		std::cerr << "#ERROR バージョン対応でありません" << std::endl;
		return false;
	}

	// DEFAULTバージョンはだめ！
	IVersionPtr ipCurVersion( m_ipWorkspace );
	CComBSTR bstr;
	ipCurVersion->get_VersionName(&bstr);

	CString strTmp( bstr );
	if( strTmp.Find(_T("DEFAULT")) != -1 )
	{
		std::cerr << "#ERROR DEFAULTバージョンです。終了します : " << strTmp << std::endl;
		return false;
	}

	// 既にそのバージョン名が存在するかチェック
	IVersionPtr ipChildVersion;
	ipVerWorkspace->FindVersion( CComBSTR(_T(strChildVersion)), &ipChildVersion );
	if( NULL == ipChildVersion )
	{
		ipCurVersion->CreateVersion( CComBSTR(_T(strChildVersion)), &ipChildVersion );
		if( NULL == ipChildVersion )
		{
			std::cerr << "#ERROR 子バージョンの作成に失敗しました : " << strChildVersion << std::endl;
			return false;
		}
	}

	std::cerr << "子バージョン : " << strChildVersion << " の作成が完了\n" << std::endl;

	m_ipEditWorkspace = ipChildVersion;

	return true;
}

// 編集開始
bool CCnvAddrcode::editStart()
{
	// 編集用子バージョン作成
	if( !makeChildVersion() )
		return false;

	if( SUCCEEDED(m_ipEditWorkspace->StartEditing(VARIANT_FALSE)) && SUCCEEDED(m_ipEditWorkspace->StartEditOperation()) )
	{
		std::cerr << "編集を開始します" << std::endl;
		return true;
	}
	else
	{
		std::cerr << "#ERROR 編集を開始できませんでした" << std::endl;
		return false;
	}
}

// 編集終了
bool CCnvAddrcode::editStop()
{
	// 更新の後処理(編集を保存するなら VARIANT_TRUE にすること)
	if( SUCCEEDED(m_ipEditWorkspace->StopEditOperation()) && SUCCEEDED(m_ipEditWorkspace->StopEditing(VARIANT_TRUE)) )
	{
		std::cerr << "編集保存が完了しました" << std::endl;
		return true;
	}
	else
	{
		std::cerr << "#ERROR 編集の保存に失敗しました" << std::endl;
		return false;
	}
}

// 拡張電話番号検索ポイント編集
void CCnvAddrcode::editExTPPoint( _IRow *ipRow )
{
	// 住所コードを取得する
	LONG lOID;
	ipRow->get_OID( &lOID );
	CComVariant vaVal;
	ipRow->get_Value( m_lAddrcode_FID, &vaVal );

	CString strAddrcode( vaVal.bstrVal );

	// 仮コードならば、先に旧住所コードに戻してから変更対象かチェック
	bool bIsTmp = false;	// 仮コードか否か
	int nResult = 0;		// 結果が1のときに更新
	CString strOldAddrcode;
	CString strNewAddrcode;
	if( isTmpCode(strAddrcode.Left(11), strOldAddrcode) )
	{
		bIsTmp = true;
		nResult = getNewAddrcode( strOldAddrcode, strNewAddrcode );

		// 仮コードが、まだ仮コードのままなら更新なし(手動更新しないといけない可能性あるのでログは出力)
		if( strAddrcode.Left(11) == strNewAddrcode )
		{
			printLog( ipRow, C_NOCHANGE_KARICODE, strAddrcode );
			return;
		}
	}
	else
	{
		// 仮コードでないとき
		nResult = getNewAddrcode( strAddrcode.Left(11), strNewAddrcode );
		
		// 新コードと旧コードが等しいかチェックし、等しければ更新なし
		if( nResult != 2 )
			nResult = ( (strAddrcode.Left(11) != strNewAddrcode)? 1 : 0 );
	}


	if( 1 != nResult )
	{
		int nState = checkAddrCodeState( strAddrcode.Left(11) );
		if( HAISHI_CODE == nState || NG_CODE == nState )
		{
			if( m_cTmpTable.isSetting() )
			{
				// 仮コード対応表でも置換試みるならチェックしてみる
				if( m_cTmpTable.getTaiouCode(strAddrcode.Left(11), strNewAddrcode) )
					nResult = 1;	/// 更新できる
			}

			if( 1 != nResult )
				printLog( ipRow, (HAISHI_CODE == nState? E_NOUPDATE_HAISHI_ADDRCODE : E_NOUPDATE_NG_ADDRCODE), "", strAddrcode );
		}
	}


	// 住所コード変更ありなら、コード置換＋共通属性更新
	if( 1 ==  nResult )
	{
		bool bEditError = false;

		// 参照モードなら、ログ出力しておしまい
		if( !m_bEditMode )
		{
			if( checkAddrCodeState(strNewAddrcode) != HAISHI_CODE )
				printLog( ipRow, O_UPDATE_DATA, strAddrcode, strNewAddrcode + strAddrcode.Mid(11) );
			else
				printLog( ipRow, O_UPDATE_DATA_HAISHI_ADDRCODE, strAddrcode, strNewAddrcode + strAddrcode.Mid(11) );
			return;
		}

		// 最終更新プログラム名
		if( FAILED(ipRow->put_Value(m_lModifyProgName_FID, CComVariant(PROGNAME))) ){
			printLog( ipRow, E_UPDATE_PROGNAME );
			bEditError = true;
		}
			
		// 最終プログラム更新日時
		SYSTEMTIME st;
		::GetLocalTime( &st );
		CString strTime;
		strTime.Format( _T("%04d/%02d/%02d %02d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );
		if( FAILED(ipRow->put_Value(m_lProgModifyDate_FID, CComVariant(strTime))) )
		{
			printLog( ipRow, E_UPDATE_MODIFYDATE );
			bEditError = true;
		}

		// 住所コード置換
		strNewAddrcode += strAddrcode.Mid(11);
		if( strNewAddrcode.GetLength() == 20 )
		{
			if( FAILED(ipRow->put_Value(m_lAddrcode_FID, CComVariant(strNewAddrcode))) )
			{
				printLog( ipRow, E_UPDATE_ADDRCODE, "", strNewAddrcode );
				bEditError = true;
			}
		}
		else
		{
			printLog( ipRow, E_UPDATE_ADDRCODE, "", strNewAddrcode );
			bEditError = true;
		}

		if( !bEditError )
		{
			if( SUCCEEDED(ipRow->Store()) )
			{
				int nErrorCode = 0;
				if( !bIsTmp )
					nErrorCode = (checkAddrCodeState(strNewAddrcode.Left(11)) == SIKOU_CODE)?  O_UPDATE : O_UPDATE_HAISHI_ADDRCODE;
				else
					nErrorCode = O_UPDATE_TMP;

				if( nErrorCode != 0 )
					printLog( ipRow, nErrorCode, strAddrcode, strNewAddrcode );
			}
			else
			{
				printLog( ipRow, E_STORE );
				m_ipEditWorkspace->AbortEditOperation();
			}
		}
		else
			m_ipEditWorkspace->AbortEditOperation();
	}
	
}

// 11桁住所コードの現在の状態取得
int	CCnvAddrcode::checkAddrCodeState( const CString& strCode )
{
	// その住所コードが存在するかチェック
	CString strSQL;
	strSQL.Format( "select haishi_date from areacode_master where ken_code='%s' and shi_code='%s' and oaza_code='%s' and aza_code='%s'",
					strCode.Left(2), strCode.Mid(2, 3), strCode.Mid(5, 3), strCode.Mid(8, 3) );

	ODynaset cDynaset( m_cJDBNew, strSQL );
	if( cDynaset.GetRecordCount() < 1 )
		return NG_CODE;	// コードなければ無効コード
	
	OValue cVal;
	cDynaset.GetFieldValue( 0, &cVal );
	CString strVal(cVal);
	return (strVal == "000000")? SIKOU_CODE : HAISHI_CODE;
}

// ログ出力
void CCnvAddrcode::printLog( _IRow* ipRow, int nErrorCode, const CString& strOldCode /* = "" */, const CString& strNewCode /* = "" */ )
{
	using namespace std;

	LONG lOID;
	ipRow->get_OID( &lOID );

	cout << "0\t" << m_strExTPFName << "\t" << lOID << "\t\t\t";

	switch( nErrorCode )
	{
	case O_UPDATE:		cout << "OK\t1\t更新完了\t" << strOldCode << "\t" << strNewCode;			break;
	case O_UPDATE_TMP:	cout << "OK\t2\t更新完了(仮コード)\t" << strOldCode << "\t" << strNewCode;	break;
	case E_UPDATE_PROGNAME:		cout << "ERROR\t3\t最終更新プログラム名の更新に失敗\t\t";	break;
	case E_UPDATE_MODIFYDATE:	cout << "ERROR\t4\t最終プログラム更新日時の更新に失敗\t\t";	break;
	case E_UPDATE_ADDRCODE:		cout << "ERROR\t5\t住所コード更新に失敗\t\t" << strNewCode;	break;
	case E_STORE:				cout << "ERROR\t6\tStoreに失敗\t\t";						break;
	case O_UPDATE_DATA:			cout << "OK\t7\t更新対象データ\t" << strOldCode << "\t" << strNewCode;	break;
	case C_NOCHANGE_KARICODE:	cout << "CONFIRM\t8\t新住所として作成された仮コードかもしれません。確認してください。\t" << strOldCode << "\t"; break;
	case O_UPDATE_HAISHI_ADDRCODE:	cout << "OK\t9\t更新完了(但し、廃止コード)\t" << strOldCode << "\t" << strNewCode;	break;
	case O_UPDATE_DATA_HAISHI_ADDRCODE:	cout << "OK\t10\t更新対象データ(但し、廃止コード)\t" << strOldCode << "\t" << strNewCode;	break;
	case E_NOUPDATE_HAISHI_ADDRCODE:	cout << "ERROR\t11\t更新なし(但し、廃止コード)\t\t" << strNewCode;	break;
	case E_NOUPDATE_NG_ADDRCODE:	cout << "ERROR\t12\t更新なし(但し、無効コード)\t\t" << strNewCode;	break;
	default:
		cout << "FATAL ERROR\t\tツール作成者に問合せしてください\t\t";
		break;
	}
	cout << "\t";

	// 住所名・初期座標提供元コード・調査結果コード出力
	CComVariant vaValue;
	ipRow->get_Value( m_lAddrName_FID, &vaValue );
	if( vaValue.vt == VT_BSTR )
		cout << COLE2T(vaValue.bstrVal);
	cout << "\t";

	ipRow->get_Value( m_lInfoSrcC_FID, &vaValue );
	cout << vaValue.lVal << "\t";

	ipRow->get_Value( m_lPointStatC_FID, &vaValue );
	cout << vaValue.lVal << endl;
}
