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

// CheckGPError.cpp: CheckGPError クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CheckGPError.h"
#include <WinLib/VersionInfo.h>
#include <crd_cnv/coord_converter.h>

#include <ArcHelperEx/GlobalFunctions.h>

#include <AddrLib/Initializer.h>
#include <AddrLib/AddrGlobals.h>
#include <AddrLib/CityAdmin.h>

#include "CSHelper.h"
#include "GPHelper.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CheckGPError::CheckGPError() :
	m_bUpdateMode(false),
	m_bAddMode(false),
	m_bCharAddrMode(false)
{
}

CheckGPError::~CheckGPError()
{
}

// 使用法
void CheckGPError::PrintUsage()
{
	std::cout << "[住所過去トラチェックツール]\n"
			  << "(Usage)CheckGPError.exe [更新モードオプション] [入力ファイル] [出力ファイル]\n"
			  << "・更新モードオプションについて\n"
			  << "  更新モードで実行する際には、第一引数に[-u]を追加してください。\n"
			  << "・環境変数について\n"
			  << "  DB_ADDRESS       ... 住所系接続プロパティ\n"
			  << "  DB_ADDRESS_REF   ... 更新時参照用住所系接続プロパティ(任意)\n"
			  << "  FC_GOU_POINT     ... 号ポイントフィーチャクラス\n"
			  << "  FC_CITY_ADMIN    ... 都市地図行政界クラス\n"
			  << "  FC_CS_ADDR_POINT ... CSポイントフィーチャクラス\n"
			  << "  JDB_CONNECT      ... 住所マスタ接続文字列\n"
			  << "  TBL_CODECONV     ... 文字付き住所用コード変換テーブル(任意)" << std::endl;
}

// 初期化
bool CheckGPError::Init( int cArgc, _TCHAR* cArgv[] )
{
	if( cArgc < 3 )
	{
		cerr << "引数が足りません" << endl;
		return false;
	}

	int i = 1;

	// 更新モード設定されているかチェック
	if( _tcscmp(cArgv[i], _T("-u")) == 0 )
	{
		m_bUpdateMode	= true;		
		++i;
	}

	// 入力ファイルのオープン
	if( !m_cListHandle.loadData(cArgv[i]) )
	{
		cerr << "入力ファイルを開けません: " << CT2CA(cArgv[i]) << endl;
		return false;
	}

	// 出力ファイルのオープン・ヘッダ出力
	m_ofs.open( static_cast<CT2CW>(cArgv[++i]) );
	if( !m_ofs.is_open() )
	{
		cerr << "出力ファイルを開けません:" << CT2CA(cArgv[i]) << endl;
		return false;
	}

	// 環境変数から各種設定
	if( !fnSetInfoFromEnv() )
		return false;

	// ログヘッダー出力
	CVersion cVer;
	CString strMsg;
	strMsg.Format( _T("#FREESTYLELOG\n")
				   _T("#%s FILEVERSION %s PRODUCTVERSION %s\n")
				   _T("FLAG\tクレームNo.\t住所コード\t住所コード(新)\tMESH\tX\tY\t作業目的\t拡張号番号フラグ\t情報ソース\tメッセージ")
				   _T("\t1:MESH\t1:X\t1:Y\t2:LAYER\t2:OBJECTID\tCOMMENT"),
				   cVer.GetOriginalFilename(), cVer.GetFileVersion(), cVer.GetProductVersion() );
	m_ofs << CT2CA(strMsg) << std::endl;
			
	return true;
}

// 実行
bool CheckGPError::Execute()
{
	if( m_bUpdateMode )
	{
		cout << "実行モード : 更新" << endl;
		if( !fnEditStart() )
			return false;
	}
	else
		cout << "実行モード : チェック" << endl;

	// ヘルパークラス初期化
	CCSHelper	m_cCSHelper( m_cCSPointFC, m_cCityAdminFC );
	CGPHelper	m_cGPHelper( m_cGouPointFC, m_cGouPointFC_Ref, m_cCityAdminFC, m_cCityAdmin_Ref );

	CLList& listRec = m_cListHandle.getRecodList();
	CLList::const_iterator itrE = listRec.end();
	for( CLList::iterator itr = listRec.begin(); itr != itrE; ++itr )
	{
		// メッシュコード・フィールドが空なら付与モードへ
		if( itr->m_cMXY.m_nMesh == 0 )
		{
			m_bAddMode = true;
			cout << CT2CA(itr->m_strAddrCode) << ": 座標付与中..." << endl;
		}
		else
		{
			m_bAddMode = false;
			cout << CT2CA(itr->m_strAddrCode) << ": チェック中..." << endl;
		}

		// 文字付き住所コード考慮でない場合、文字付き住所は処理対象から外す
		if( !m_bCharAddrMode && itr->IsCharAddrCode() )
		{
			m_ofs << "#文字付き住所考慮モードでない為、文字付き住所レコードは処理をスキップします : " << CT2CA(itr->m_strAddrCode) << std::endl;
			continue;
		}

		// 最新11桁住所コードに置換
		itr->m_strAddrCodeNew = m_cAcHelper.GetNewestCode( itr->m_strAddrCode.Left(11) ) + itr->m_strAddrCode.Mid(11, 9);

		// CS_ADDR_POINT から検索(文字付き住所の場合は、飛ばす)
		EResultStat emStatus = !itr->IsCharAddrCode()? m_cCSHelper.findPoint(*itr, m_bAddMode) : kO_NOT_FOUND;
		if( kO_NOT_FOUND != emStatus )
		{
			// 更新モードなら、移動処理実施
			if( kW_CS_MOVED == emStatus && m_bUpdateMode )
			{
				emStatus = m_cCSHelper.movePoint(*itr);
			}
		}
		else
		{
			// CS_ADDR_POINT がなければ GOU_POINT から検索
			emStatus = m_cGPHelper.findPoint( *itr, m_bAddMode );
			if( kO_NOT_FOUND != emStatus )
			{
				// 更新モードなら、移動処理実施
				if( (kW_GP_MOVED == emStatus || kW_GP_NOT_PRIOR == emStatus) && m_bUpdateMode )
				{
					emStatus = m_cGPHelper.movePoint( *itr, kW_GP_MOVED == emStatus );
				}
			}
			else
			{
				// 更新モードなら、追加処理実施
				if( m_bUpdateMode )
				{
					// 先にどういった形でポイントを追加するかをチェック
					emStatus = checkInsertPoint( *itr );
					switch( emStatus )
					{
					case kW_CS_INSERT:
						if( itr->IsCharAddrCode() )
							emStatus = KW_CS_INSERT_CANNOT_CHAR;
						else
							emStatus = m_cCSHelper.insertPoint(*itr);
						break;
					case kW_GP_INSERT:
						emStatus = m_cGPHelper.insertPoint( *itr );
						break;
					default:
						break;
					}
				}
			}
		}
		outputLog( emStatus, *itr );	// 出力
	}

	if( m_bUpdateMode )
	{
		// 編集終了
		if( !fnEditStop() )
			return false;
	}

	return true;
}

//////////////////////////////////
//	内部関数
//////////////////////////////////

// 環境変数取得
bool CheckGPError::fnSetInfoFromEnv()
{
#ifdef _DEBUG
	_tputenv( _T("JDB_CONNECT=y1205/y1205@delltcp") );
	_tputenv( _T("DB_ADDRESS=PRODENG1/PRODENG1/PRODENG1.CheckGPError_test/5151/coral2") );	
	_tputenv( _T("DB_ADDRESS_REF=PRODENG1/PRODENG1/PRODENG1.CheckGPError_20120801214147/5151/coral2") );
	_tputenv( _T("FC_GOU_POINT=Gou_Point") );
	_tputenv( _T("FC_CITY_ADMIN=City_Admin") );
	_tputenv( _T("FC_CS_ADDR_POINT=CS_Addr_Point") );
	_tputenv( _T("TBL_CODECONV=Test\\CodeConvTable.mdb") );
#endif

	// 住所DB接続
	CString strDBProp;
	if( !strDBProp.GetEnvironmentVariable(_T("JDB_CONNECT")) )
	{
		cerr << "環境変数 JDB_CONNECT が設定されていません" << endl;
		return false;
	}
	if( !m_cAcHelper.Connect(strDBProp) )
	{
		cerr << "住所DBへの接続に失敗 : " << CT2CA(strDBProp) << endl;
		return false;
	}

	// SDE接続
	if( !strDBProp.GetEnvironmentVariable(_T("DB_ADDRESS")) )
	{
		cerr << "環境変数 DB_ADDRESS が設定されていません" << endl;
		return false;
	}
	if( !m_cWorkspace.Connect(strDBProp) )
	{
		cerr << "SDE接続に失敗 : " << CT2CA(strDBProp) << endl;
		return false;
	}

	// 更新モードのときは更新元データDBを参照するかどうか確認する (bug9300)
	if( m_bUpdateMode )
	{
		if( !strDBProp.GetEnvironmentVariable(_T("DB_ADDRESS_REF")) )
		{
			cerr << "環境変数 DB_ADDRESS_REF が設定されていません" << endl;
			return false;
		}
		else
		{
			if( !m_cWorkspace_Ref.Connect(strDBProp) )
			{
				cerr << "SDE接続に失敗 : " << CT2CA(strDBProp) << endl;
				return false;
			}
			m_ofs << "#更新時にDB：" << CT2CA(strDBProp) << "を参照します" << std::endl;
		}
	}

	// フィーチャクラス取得
	CString strFCName;
	if( !strFCName.GetEnvironmentVariable(_T("FC_GOU_POINT")) )
	{
		cerr << "環境変数 FC_GOU_POINT が設定されていません" << endl;
		return false;
	}
	m_cGouPointFC.SetObject( m_cWorkspace.OpenTable(strFCName) );
	if( !m_cGouPointFC )
	{
		cerr << "号ポイントフィーチャクラスの取得に失敗 : " << CT2CA(strFCName) << std::endl;
		return false;
	}
	if( m_bUpdateMode )
	{
		m_cGouPointFC_Ref.SetObject( m_cWorkspace_Ref.OpenTable(strFCName) );
		if( !m_cGouPointFC_Ref )
		{
			cerr << "号ポイントフィーチャクラスの取得に失敗 : " << CT2CA(strFCName) << std::endl;
		}
	}

	if( !strFCName.GetEnvironmentVariable(_T("FC_CITY_ADMIN")) )
	{
		cerr << "環境変数 FC_CITY_ADMIN が設定されていません" << endl;
		return false;
	}
	m_cCityAdminFC.SetObject( m_cWorkspace.OpenTable(strFCName) );
	if( !m_cCityAdminFC )
	{
		cerr << "都市地図行政界フィーチャクラスの取得に失敗 : " << CT2CA(strFCName) << std::endl;
		return false;
	}	
	if( m_bUpdateMode )
	{
		m_cCityAdmin_Ref.SetObject( m_cWorkspace_Ref.OpenTable(strFCName) );
		if( !m_cCityAdmin_Ref )
		{
			cerr << "都市地図行政界フィーチャクラスの取得に失敗 : " << CT2CA(strFCName) << std::endl;
		}
	}

	if( !strFCName.GetEnvironmentVariable(_T("FC_CS_ADDR_POINT")) )
	{
		cerr << "環境変数 FC_CS_ADDR_POINT が設定されていません" << endl;
		return false;
	}
	m_cCSPointFC.SetObject( m_cWorkspace.OpenTable(strFCName) );
	if( !m_cCSPointFC )
	{
		cerr << "CSポイントフィーチャクラスの取得に失敗 : " << CT2CA(strFCName) << std::endl;
		return false;
	}

	// 文字付き住所用コード変換テーブル読み込み(環境変数指定しれば、文字付き住所も対応するモード)
	CString strCodeConvTbl;
	if( strCodeConvTbl.GetEnvironmentVariable(_T("TBL_CODECONV")) )
	{
		if( !addr::Init_CodeConverter(strCodeConvTbl) )
		{
			cerr << "文字付き住所用コード変換テーブルの初期化に失敗 : " << CT2CA(strCodeConvTbl) << std::endl;
			return false;
		}
		m_bCharAddrMode = true;
		m_ofs << "#文字付き住所を考慮してチェックします" << std::endl;
	}
	else
	{
		cerr << "環境変数 TBL_CODECONV が未設定の為、文字付き住所は考慮しません" << std::endl;
		m_ofs << "#環境変数 TBL_CODECONV が未設定の為、文字付き住所は考慮しません" << std::endl;
	}

	return true;
}

// 編集開始
bool CheckGPError::fnEditStart()
{
	// まずは、編集用子バージョン作成(CheckGPError_yyyymmddhhmmss)
	SYSTEMTIME st;
	::GetLocalTime( &st );
	CString strChildVer;
	strChildVer.Format( _T("%s_%04d%02d%02d%02d%02d%02d"), PROGRAM_NAME, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );

	IVersionedWorkspacePtr ipVerWorkspace( (IWorkspace*)m_cWorkspace );
	if( !ipVerWorkspace )
	{
		cerr << "#Error バージョン対応でありません" << endl;
		return false;
	}

	// DEFAULTバージョンはだめ
	IVersionPtr ipCurVersion( (IWorkspace*)m_cWorkspace );
	CComBSTR bstr;
	ipCurVersion->get_VersionName( &bstr );
	CString strTmp(bstr);
	if( strTmp.Find(_T("DEFAULT")) != -1 )
	{
		cerr << "#Error デフォルトバージョンですので終了します : " << CT2CA(strTmp) << endl;
		return false;
	}

	// 既にそのバージョンが存在するかチェック
	IVersionPtr ipChildVersion;
	ipVerWorkspace->FindVersion( CComBSTR(strChildVer), &ipChildVersion );
	if( !ipChildVersion )
	{
		ipCurVersion->CreateVersion( CComBSTR(strChildVer), &ipChildVersion );
		if( !ipChildVersion )
		{
			cerr << "#Error 編集用バージョンの作成に失敗 : " << CT2CA(strChildVer) << endl;
			return false;
		}

		// 子バージョンにコメント追加
		CString strComment;
		strComment.Format( _T("CheckGPErrorによる編集Ver %s"), gf::GetCurAccount() );
		ipChildVersion->put_Description( CComBSTR(strComment) );			
	}

	cout << "編集用バージョン : " << CT2CA(strChildVer) << " の作成が完了" << endl;

	m_cWorkspace = (IWorkspacePtr)ipChildVersion;

	// 編集開始
	if( sindyErr_NoErr != m_cWorkspace.StartEditing(false) || sindyErr_NoErr != m_cWorkspace.StartEditOperation() )
	{
		cerr << "#Error 編集開始に失敗" << endl;
		return false;
	}

	// 編集用ワークスペースからCS/GPフィーチャクラス取り直しをする
	m_cGouPointFC.SetObject( m_cWorkspace.OpenTable(m_cGouPointFC.GetNameString()->GetTableName()) );
	if( !m_cGouPointFC )
	{
		cerr << "#Error 号ポイントフィーチャクラスの取得に失敗" << CT2CA(m_cGouPointFC.GetNameString()->GetTableName()) << std::endl;
		return false;
	}
	m_cCSPointFC.SetObject( m_cWorkspace.OpenTable(m_cCSPointFC.GetNameString()->GetTableName()) );
	if( !m_cCSPointFC )
	{
		cerr << "#Error CSポイントフィーチャクラスの取得に失敗 : " << CT2CA(m_cCSPointFC.GetNameString()->GetTableName()) << std::endl;
		return false;
	}

	return true;
}

// 編集終了
bool CheckGPError::fnEditStop()
{
	if( sindyErr_NoErr != m_cWorkspace.StartEditOperation() || sindyErr_NoErr != m_cWorkspace.StopEditing(true) )
	{
		m_cWorkspace.AbortEditOperation();
		cerr << "#Error 編集終了に失敗" << endl;
		return false;
	}
	else
		return true;
}

// ポイント追加位置のチェック
EResultStat CheckGPError::checkInsertPoint( CLRec& cRec )
{
	// メッシュＸＹ→経緯度変換
	IPointPtr ipPoint = gf::CreateMXYPoint( cRec.m_cMXY );
	if( ipPoint )
	{
		// 座標位置の行政界を検索
		ISpatialFilterPtr ipFilter( AheInitSpatialFilter(NULL, ipPoint) );
		long lCount = m_cCityAdminFC.FeatureCount(ipFilter);
		if( lCount == 1 )
		{
			IFeatureCursorPtr ipFeatureCursor( m_cCityAdminFC.Search(AheInitSpatialFilter(NULL, ipPoint), VARIANT_FALSE) );
			if( ipFeatureCursor )
			{
				IFeaturePtr	ipFeature;
				ipFeatureCursor->NextFeature( &ipFeature );

				CCityAdmin cAdmin( ipFeature, sindyTableType::city_admin, false, m_cCityAdminFC.GetFieldMap(), m_cCityAdminFC.GetNameString() );

				const CString& strAdminCode = cAdmin.GetAddrCode().Left(11);

				// 住所コード比較(行政界が通称ならそれも考慮[Bug7257,Bug8166])
				if( cRec.m_strAddrCodeNew.Left(11) == strAdminCode.Left(11) 
					|| (cAdmin.GetOazaAzaCode2() != _T("000000") && cRec.m_strAddrCodeNew.Left(11) == strAdminCode.Left(5) + cAdmin.GetOazaAzaCode2()) )
				{
					return kW_GP_INSERT;	// 行政界と整合取れているので、GOUで追加
				}
				else
					return kW_CS_INSERT;	// 行政界と整合取れているので、CSで追加
			}
		}

		return kE_INSERT_ADMIN_FAIL;
	}
	else
		return kE_INSERT_FAIL;
}

// ログ出力
void CheckGPError::outputLog( EResultStat emStatus, CLRec &cRec )
{
	if( cRec.m_strAddrCode == cRec.m_strAddrCodeNew )
		cRec.m_strAddrCodeNew = "";

	CString strLog;
	strLog.Format( _T("0\t%s\t%s\t%s\t"), cRec.m_strClaimNo, cRec.m_strAddrCode, cRec.m_strAddrCodeNew );

	if( cRec.m_cMXY.m_nMesh == 0 )
		strLog.Append( _T("\t\t\t") );
	else
		strLog.AppendFormat( _T("%d\t%d\t%d\t"), cRec.m_cMXY.m_nMesh, cRec.m_cMXY.m_nX, cRec.m_cMXY.m_nY );

	strLog.AppendFormat( _T("%d\t%d\t%s\t%s\t"), cRec.m_lPurpose, cRec.m_lExFlag, cRec.m_strSource, GetResultMsg(emStatus) );

	if( cRec.m_cMXY_Ref.m_nMesh == 0 )
		strLog.Append( _T("\t\t\t") );
	else
		strLog.AppendFormat( _T("%d\t%d\t%d\t"), cRec.m_cMXY_Ref.m_nMesh, cRec.m_cMXY_Ref.m_nX, cRec.m_cMXY_Ref.m_nY );

	switch( emStatus )
	{
	case kW_CS_MOVED:
	case kW_CS_INSERT:
		if( m_bUpdateMode )
			strLog.AppendFormat( _T("%s\t%d\t"), m_cCSPointFC.GetNameString()->GetOwnerTableName(), cRec.m_lEditPointID );
		else
			strLog.Append( _T("\t\t") );
		break;
	case kW_GP_MOVED:
	case kW_GP_MOVED_NOT_PRIOR:
	case kW_GP_INSERT:
	case kW_GP_INSERT_EX:
		if( m_bUpdateMode )
			strLog.AppendFormat( _T("%s\t%d\t"), m_cGouPointFC.GetNameString()->GetOwnerTableName(), cRec.m_lEditPointID );
		else
			strLog.Append( _T("\t\t") );
		break;
	default:
		strLog.Append( _T("\t\t") );
		break;
	}

	m_ofs << CT2CA(strLog) << std::endl;
}
