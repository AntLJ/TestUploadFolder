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

// DeleteAddrPoint.cpp: DeleteAddrPoint クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DeleteAddrPoint.h"
#include "GlobalFunc.h"
#include <WinLib/VersionInfo.h>

#include <crd_cnv/coord_converter.h>

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

DeleteAddrPoint::DeleteAddrPoint()
{
}

DeleteAddrPoint::~DeleteAddrPoint()
{
}

// 使用法
void DeleteAddrPoint::PrintUsage()
{
	std::cout << "[住所ポイント削除ツール]\n"
			  << "(Usage)DeleteAddrPoint.exe [入力ファイル] [ログファイル]\n"
			  << "環境変数\n"
			  << "DB_MAIN      ... 住所系接続プロパティ\n"
			  << "FC_TARGET    ... 削除対象フィーチャクラス\n" << std::endl;
}

// 初期化
bool DeleteAddrPoint::Init(int cArgc, _TCHAR *cArgv[])
{
	if( cArgc < 3 )
	{
		std::cerr << "#Error 引数が足りません" << std::endl;
		return false;
	}

	// 削除可能レイヤをセットに格納
	fnSetDeleteLayer();

	// 環境変数取得とDB接続
	if( !fnSetInfoFromEnv() )
		return false;

	int		i = 0;

	// 入力ファイル
	CString	aOIDListName;
	aOIDListName = cArgv[++i];

	if( !fnLoadOIDList( aOIDListName ) )
	{
		cerr << "#Error OIDリストの読み込みに失敗:" << CT2CA( aOIDListName ) << endl;
		return false;
	}
	
	// 出力ファイルのオープン
	m_ofs.open( static_cast<CT2CW>(cArgv[++i]) );
	if( !m_ofs.is_open() )
	{
		cerr << "#Error 出力ファイルを開けません:" << CT2CA( cArgv[i] ) << endl;
		return false;
	}
	
	// ヘッダ出力
	fnPrintHeader();

	return true;
}

// ログにヘッダーを出力
void DeleteAddrPoint::fnPrintHeader()
{
	// 固定部分の出力
	m_ofs << "# FREESTYLELOG" << endl;

	CVersion cVer;
	m_ofs << "# " << CT2CA(cVer.GetOriginalFilename()) << " FILEVERSION " << CT2CA(cVer.GetFileVersion()) << " PRODUCTVERSION " << CT2CA(cVer.GetProductVersion()) << endl;

	CString csStartTime = gf::GetCurDateStr();
	m_ofs << "# 開始時間：" << CT2CA(csStartTime) << endl;

	m_ofs << "FLAG\tMESH\tX\tY\tLAYERNAME\t";

	// DBからフィールド情報を取得し、全てヘッダに出力
	IFieldsPtr	ipFields;
	long		iFieldNum = 0;
	ipFields	= m_cTargetPointFC.GetFields();
	ipFields->get_FieldCount( &iFieldNum );

	IFieldPtr	ipField;
	CComBSTR	bsFieldName;
	// SHAPEフィールド以外のフィールド全て出力
	for( int i = 0 ; i < iFieldNum ; i++ )
	{
		ipFields->get_Field( i, &ipField );
		esriFieldType	fType;
		ipField->get_Type( &fType );
		if( fType != esriFieldTypeGeometry )
		{
			ipField->get_Name( &bsFieldName );
			m_ofs << CT2CA(bsFieldName) << "\t";
		}
		// 削除処理の際の出力に使うため、SHAPEフィールドの番号を取得しておく
		else
			m_iGeometryFieldCount = i;
	}

	// 固定部分の出力
	m_ofs << "STATUS\tCOMMENT" << endl;
}

// 実行
bool DeleteAddrPoint::Execute()
{
	// 子バージョン作成
	if( !fnCreateChildVer() )
		return false;
	
	// 編集開始
	if( !fnEditStart() )
		return false;

	// 削除処理
	if( !fnDelete() )
		return false;

	// 編集終了
	if( !fnEditStop() )
		return false;

	return true;
}

// 削除対象レイヤ列作成
void DeleteAddrPoint::fnSetDeleteLayer()
{
	m_csSetDeleteLayer.insert(_T("gou_point"));
	m_csSetDeleteLayer.insert(_T("cs_addr_point"));
	m_csSetDeleteLayer.insert(_T("klink_point"));
	m_csSetDeleteLayer.insert(_T("base_rep_point"));
	m_csSetDeleteLayer.insert(_T("extp_point"));
	m_csSetDeleteLayer.insert(_T("teltp_point"));
}

// 環境変数から情報取得
bool DeleteAddrPoint::fnSetInfoFromEnv()
{
#ifdef	_DEBUG
//	_tputenv( _T("DB_MAIN=FIX200911/FIX200911/SDE.DEFAULT/5151/topaz2") );
	_tputenv( _T("DB_MAIN=SINDYTEST/SINDYTEST/DeleteAddrPoint/5151/galatea") );
//	_tputenv( _T("DB_MAIN=SINDYTEST/SINDYTEST/SDE.DEFAULT/5151/galatea") );
//	_tputenv( _T("DB_MAIN=FIX201001/FIX201001/SDE.DEFAULT/5151/topaz2") );
//	_tputenv( _T("DB_MAIN=SINUP2009B/SINUP2009B/SDE.EDT_SINUP2009B/5151/garnet2") );
	//_tputenv( _T("FC_TARGET=TelTP_Point") );
	//_tputenv( _T("FC_TARGET=FIX200911.TelTP_Point") );
	//_tputenv( _T("FC_TARGET=GOU_POINT") );
	_tputenv( _T("FC_TARGET=Gou_Point") );
#endif

	// SDE接続
	CString strDBProp;
	if( !strDBProp.GetEnvironmentVariableW( _T("DB_MAIN") ) )
	{
		cerr << "#Error 環境変数 DB_MAIN が設定されていません" << endl;
		return false;
	}
	if( !m_cWorkspace.Connect( strDBProp ) )
	{
		cerr << "#Error SDE接続に失敗 : " << CT2CA( strDBProp ) << endl;
		return false;
	}

	// フィーチャクラス取得
	if( !m_strFCName.GetEnvironmentVariableW( _T("FC_TARGET")) )
	{
		cerr << "#Error 環境変数 FC_TARGET が設定されていません" << endl;
		return false;
	}
	// 削除可能レイヤ以外エラー
	CString	strLayerName;
	int		iPos	= 0;
	iPos			= m_strFCName.Find(_T("."));
	if( iPos > 0 )
		strLayerName	= m_strFCName.Mid( ++iPos );
	else
		strLayerName	= m_strFCName;

	if( m_csSetDeleteLayer.find( strLayerName.MakeLower() ) == m_csSetDeleteLayer.end() )
	{
		cerr << "#Error 指定したレイヤは削除可能レイヤではありません : " << CT2CA( m_strFCName ) << endl;
		return false;
	}

	m_cTargetPointFC.SetObject( m_cWorkspace.OpenTable( m_strFCName ) );
	if( m_cTargetPointFC == NULL )
	{
		cerr << "#Error 削除対象フィーチャクラスの取得に失敗 : " << CT2CA( m_strFCName ) << endl;
		return false;
	}

	return true;
}

// 削除処理用子バージョンを作成
bool DeleteAddrPoint::fnCreateChildVer()
{
	// バージョン用の時間文字列取得
	CString		csVarTime = gf::GetCurDateStr( FORVER );

	// 編集用子バージョン作成(DeleteAddrPoint_yyyymmddhhmmss)
	CVersion	cVer;
	CString		csChildVer = cVer.GetInternalName();
	csChildVer	= csChildVer + "_" + csVarTime;

	// バージョン非対応レイヤはだめ
	IFeatureClassPtr	ipTargetFC( m_cTargetPointFC.OpenTable( m_strFCName ));
	IDatasetPtr			ipDataset( ipTargetFC );
	INamePtr			ipName;
	ipDataset->get_FullName( &ipName );			// IName取得
	IWorkspacePtr		ipWorkspace;
	ipDataset->get_Workspace( &ipWorkspace );	// IWorkspace取得

	// バージョン対応の判別は、IFeatureWorkspaceManage::IsRegisteredAsVersioned メソッドを使用する
	VARIANT_BOOL		vBool = VARIANT_FALSE;
	IFeatureWorkspaceManagePtr	ipFeatureWorkspaceManage( ipWorkspace );
	ipFeatureWorkspaceManage->IsRegisteredAsVersioned( ipName, &vBool );
	if( vBool == VARIANT_FALSE )
	{
		cerr << "#Error 指定レイヤがバージョン非対応のため終了" << endl;
		return false;
	}

	// DEFAULTバージョンはだめ
	IVersionPtr ipCurVersion( (IWorkspace*)m_cWorkspace );
	CComBSTR bstr;
	ipCurVersion->get_VersionName( &bstr );
	CString strTmp(bstr);
	if( strTmp.Find(_T("DEFAULT")) != -1 )
	{
		cerr << "#Error デフォルトバージョンのため終了" << endl;
		return false;
	}

	// 既にそのバージョンが存在するかチェック
	IVersionedWorkspacePtr ipVerWorkspace( (IWorkspace*)m_cWorkspace );
	IVersionPtr ipChildVersion;
	ipVerWorkspace->FindVersion( CComBSTR(csChildVer), &ipChildVersion );
	if( NULL == ipChildVersion )
	{
		ipCurVersion->CreateVersion( CComBSTR(csChildVer), &ipChildVersion );
		if( NULL == ipChildVersion )
		{
			cerr << "#Error 編集用バージョンの作成に失敗 : " << CT2CA(csChildVer) << endl;
			return false;
		}

		// 子バージョンにコメント追加
		CString strComment;
		strComment.Format( _T("DeleteAddrPoint による編集Ver %s"), gf::GetCurAccount() );
		ipChildVersion->put_Description( CComBSTR(strComment) );			
	}
	cout << "編集用バージョン : " << CT2CA(csChildVer) << " の作成が完了" << endl;

	// workspaceを作成した子バージョンに変更
	m_cWorkspace = (IWorkspacePtr)ipChildVersion;

	return true;
}

// OIDリストを開き、vectorに格納
bool DeleteAddrPoint::fnLoadOIDList( CString aOIDListName )
{
	ifstream	fin;
	fin.open( aOIDListName );
	if( !fin )
	{
		cerr << "#Error ファイルをオープンできません" << endl;
		return false;
	}

	int		i = 0;
	while(1)
	{
		char	acBuf[1024];
		i++;

		fin.getline( acBuf, 1024 );

		// 終端ならばループを抜ける
		if( fin.eof() )
			break;

		if( !fin )
		{
			cerr << "【 " << i << " 行目】読み込み失敗" << endl;
			fin.close();
			return false;
		}
		string	asBuf	= acBuf;
		if(asBuf.find_first_not_of("0123456789") != string::npos)
		{
			cerr << "【 " << i << " 行目】不正なID( " << asBuf << " )" << endl;
			fin.close();
			return false;
		}
		long	aOID = atol( asBuf.c_str() );
		m_lOIDList.push_back( aOID );
	}
	fin.close();

	return true;
}

// 編集開始
bool DeleteAddrPoint::fnEditStart()
{
	HRESULT	aResult;
	aResult = m_cWorkspace.StartEditing( VARIANT_FALSE );
	if(FAILED(aResult))
	{
		cerr << "StartEditing 失敗" << endl;
		return false;
	}
	aResult = m_cWorkspace.StartEditOperation();
	if(FAILED(aResult))
	{
		cerr << "StartEditOperation 失敗" << endl;
		m_cWorkspace.StopEditing(VARIANT_FALSE);
		return false;
	}

	// 編集用ワークスペースから削除対象フィーチャクラス取り直しをする
	m_cTargetPointFC.SetObject( m_cWorkspace.OpenTable(m_cTargetPointFC.GetNameString()->GetTableName()) );
	if( NULL == m_cTargetPointFC )
	{
		cerr << "#Error 削除対象ポイントフィーチャクラスの取得に失敗" << CT2CA(m_cTargetPointFC.GetNameString()->GetTableName()) << std::endl;
		return false;
	}

	return true;
}

// 削除処理
bool DeleteAddrPoint::fnDelete()
{
	// OIDリストのループ
	list<long>::iterator	itrOID;
	for( itrOID = m_lOIDList.begin() ; itrOID != m_lOIDList.end() ; itrOID++ )
	{
		CString strWhere;
		strWhere.Format( _T("OBJECTID = %d"), *itrOID );

		IQueryFilterPtr ipQuery( AheInitQueryFilter(NULL, NULL, strWhere) );

		long lTarCount						= m_cTargetPointFC.FeatureCount( ipQuery );
		IFeatureCursorPtr ipFeatureCursor	= m_cTargetPointFC.Search( ipQuery, VARIANT_FALSE );

		if( lTarCount > 0 && ipFeatureCursor )
		{
			cout << "【 " << (*itrOID) << " 】対応データ確認";

			IFeaturePtr		ipFeature;
			while( ipFeatureCursor->NextFeature( &ipFeature ) == S_OK )
			{
				IGeometryPtr	ipGeo;
				ipFeature->get_ShapeCopy( &ipGeo );
				IPointPtr		ipPoint( ipGeo );
				gf::ConvPoint2MXY( ipPoint, &m_cMXY );
				m_ofs << "0\t" << m_cMXY.m_nMesh << "\t" << m_cMXY.m_nX << "\t" << m_cMXY.m_nY << "\t" << CT2CA(m_strFCName) << "\t";

				// DBから全ての情報を取得し、出力
				IFieldsPtr	ipFields;
				long		iFieldNum = 0;
				ipFields	= m_cTargetPointFC.GetFields();
				ipFields->get_FieldCount( &iFieldNum );

				IFieldPtr	ipField;
				CComBSTR	bsValue;
				CComVariant	vaValue;
				SYSTEMTIME st;

				// SHAPEフィールド以外のフィールド全て出力
				for( int i = 0 ; i < iFieldNum  ; i++ )
				{
					if( i == m_iGeometryFieldCount )
						continue;

					ipFeature->get_Value( i, &vaValue );
					switch( vaValue.vt )
					{
					case VT_I2:
						m_ofs << vaValue.iVal << "\t";
						break;
					case VT_I4:
						m_ofs << vaValue.lVal << "\t";
						break;
					case VT_BSTR:
						m_ofs << CT2CA(vaValue.bstrVal) << "\t";
						break;
					case VT_DATE:
						VariantTimeToSystemTime(vaValue.date, &st);
						m_ofs << st.wYear << "/" << st.wMonth << "/" << st.wDay << " " << st.wHour << ":" << st.wMinute <<  ":"  << st.wSecond << "\t";
						break;
					case VT_EMPTY:
					case VT_NULL:
					default:
						m_ofs << "\t";
						break;
					}
				}

				if( ipFeature->Delete() == S_OK )
				{
					cout << "...削除成功" << endl;			
					m_ofs << "削除成功";
				} 
				else 
				{
					cout << "...削除失敗" << endl;	
					m_ofs << "削除失敗";
				}

				m_ofs << endl;
			}
		}
	}
	return true;
}

// 編集終了
bool DeleteAddrPoint::fnEditStop()
{	
	if(FAILED( m_cWorkspace.StopEditOperation() ))
	{
		cerr << "StopEditOperation 失敗" << endl;
		return false;
	}
	if(FAILED( m_cWorkspace.StopEditing(true) ))
	{
		cerr << "StopEditing 失敗" << endl;
		return false;
	}

	// 終了時間出力
	CString csEndTime = gf::GetCurDateStr();
	m_ofs << "# 終了時間：" << CT2CA(csEndTime) << endl;

	return true;
}
