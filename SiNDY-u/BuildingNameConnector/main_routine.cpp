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
#include "main_routine.h"
#include <conio.h>

#include <atlstr.h>
#include "..\..\LIB\WinLib\VersionInfo.h"
#include <sindy/workspace.h>
#include <boost/program_options.hpp>

#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>

namespace {
	typedef boost::char_separator<char> char_separator;
	typedef boost::tokenizer<char_separator> tokenizer;
}

//--------------------------------------------------------------------------------
/**
* @brief エラーメッセージ
*/
//--------------------------------------------------------------------------------
void	PrnMsg	( FILE* c_cpFp, FILE* c_cpFp2, const char* c_cpFormat, ... ) {
							
	va_list	a_vArgList = NULL;

	va_start(a_vArgList, c_cpFormat );	// 初期化

	vfprintf_s ( c_cpFp, c_cpFormat, a_vArgList );
	fflush	( c_cpFp );
	if( c_cpFp2 != NULL && c_cpFp != c_cpFp2 ) {
		vfprintf_s ( c_cpFp2, c_cpFormat, a_vArgList );
		fflush	( c_cpFp2 );					
	}
}

// DBへの接続オープン
bool	main_routine::
DB_Open ( void )
{
	// 建物用ワークスペース
	m_ipWSpace_Building = sindy::create_workspace(CString(m_sDB_Building.c_str()));
	if(!m_ipWSpace_Building ){
		PrnMsg( m_fErr_Fp, stderr, "ERROR:DBがオープンできない : <%s>\n", m_sDB_Building.c_str() );
		return false;
	}
	// メッシュ用ワークスペース
	m_ipWSpace_Mesh = sindy::create_workspace(CString(m_sDB_Mesh.c_str()));
	if(!m_ipWSpace_Mesh ){
		PrnMsg( m_fErr_Fp, stderr, "ERROR:DBがオープンできない : <%s>\n", m_sDB_Mesh.c_str() );
		return false;
	}
	// POI用ワークスペース
	m_ipWSpace_POI = sindy::create_workspace(CString(m_sDB_POI.c_str()));
	if(!m_ipWSpace_POI ){
		PrnMsg( m_fErr_Fp, stderr, "ERROR:DBがオープンできない : <%s>\n", m_sDB_POI.c_str() );
		return false;
	}
	// 不動産情報用ワークスペース
	m_ipWSpace_RealEstate = sindy::create_workspace(CString(m_sDB_RealEstate.c_str()));
	if(!m_ipWSpace_RealEstate ){
		PrnMsg( m_fErr_Fp, stderr, "ERROR:DBがオープンできない : <%s>\n", m_sDB_RealEstate.c_str() );
		return false;
	}

	// 建物用フィーチャクラスのオープン
	if( FAILED (IFeatureWorkspacePtr(m_ipWSpace_Building)->OpenFeatureClass ( _bstr_t ( m_sFC_Building.c_str() ), &m_ipFC_Building ) ) ) {
		PrnMsg ( m_fErr_Fp, stderr, "ERROR:フィーチャクラスがオープンできませんでした : <%s>\n", m_sFC_Building.c_str() ); 
		return false;
	}
	// メッシュ用フィーチャクラスのオープン
	if( FAILED (IFeatureWorkspacePtr(m_ipWSpace_Mesh)->OpenFeatureClass ( _bstr_t ( m_sFC_Mesh.c_str() ), &m_ipFC_Mesh ) ) ) {
		PrnMsg ( m_fErr_Fp, stderr, "ERROR:フィーチャクラスがオープンできませんでした : <%s>\n", m_sFC_Mesh.c_str() ); 
		return false;
	}
	// POI用フィーチャクラスのオープン
	if( FAILED (IFeatureWorkspacePtr(m_ipWSpace_POI)->OpenFeatureClass ( _bstr_t ( m_sFC_POI.c_str() ), &m_ipFC_POI ) ) ) {
		PrnMsg ( m_fErr_Fp, stderr, "ERROR:フィーチャクラスがオープンできませんでした : <%s>\n", m_sFC_POI.c_str() ); 
		return false;
	}
	// 不動産情報用フィーチャクラスのオープン
	if( FAILED (IFeatureWorkspacePtr(m_ipWSpace_RealEstate)->OpenFeatureClass ( _bstr_t ( m_sFC_RealEstate.c_str() ), &m_ipFC_RealEstate ) ) ) {
		PrnMsg ( m_fErr_Fp, stderr, "ERROR:フィーチャクラスがオープンできませんでした : <%s>\n", m_sFC_RealEstate.c_str() ); 
		return false;
	}
	return true;
}

//	ログファイルをオープンする
bool	main_routine::
Open_Log ( std::string& csFName,
           FILE**       cFp )
{
	if( m_sLog_File == "" ) {
		return	true;
	}else {
		*cFp = fopen ( csFName.c_str(), "w" );
		if( *cFp == NULL ) {
			return false;
		}
	}
	return true;
}

// 使用法表示
void		main_routine::
print_usage	(char **argv)
{
	PrnMsg(	stdout, NULL,
		"=== 【建物名称・建物ポリゴン紐付けツール】(v%s) ===\n"
				"<usage>\n"
				"%s <オプション>... \n"
/*				"\t      '--db1' : ★削除処理対象DB接続1(ex. --db1 SINDY2012B/SINDY2012B/SDE.SNP_delete_130220/5151/ruby2 )\n"
				"\t      '--fc1' : ★削除処理対象フィーチャクラス1(ex. --fc1 SINDY2012B.Building )\n"
				"\t      '--db2' : 　削除処理対象DB接続(ex. --db1 SINDY2012B/SINDY2012B/SDE.SNP_delete_130220/5151/ruby2 )\n"
				"\t      '--fc2' : 　削除処理対象フィーチャクラス(ex. --fc1 SINDY2012B.Building_Step )\n"
				"\t      '--rel' : 　フィーチャクラス１に対する２の関係指定(ex. --rel contains).\n"
				"\t                     <指定可能な関係>\n"
				"\t                        contains   fc1のデータにfc2が内包（--rel未指定時のデフォルト）\n"
				"\t                        overlaps   fc1のデータにfc2が重複（fc1とfc2が同次元）\n"
				"\t                        within     fc1のデータにfc2が内包\n"
				"\t                        touches    fc1のデータにfc2が接触\n"
				"\t                        equals     fc1のデータとfc2が同じ（Arcだと微妙な誤差を許容した判定になるはず）\n"
				"\t                        crosses    fc1のデータにfc2が交差（fc1とfc2が別次元）\n"
				"\t      '--oid' : ★オブジェクトIDリストを指定(ex. --oid .\\test\\objectid.txt)\n"
				"\t      '--l    : 　ログファイル名を指定(ex. -l .\\test\\erace.log)\n"
				"\t      '--del' : 削除モード指定（未指定時はチェックモード）\n"*/
				"\t      ※★印がついているものは必須オプション。\n"
				"\t      \n", m_sVersion, argv[0] );
}


using boost::program_options::options_description;
using boost::program_options::value;
using boost::program_options::variables_map;
using boost::program_options::store;
using boost::program_options::parse_command_line;
using boost::program_options::notify;

// 実行情報表示
void main_routine::
PrintExecInfo( void ) {
	PrnMsg( m_fErr_Fp, stderr, "#SINDYSTDLOG\n");
	PrnMsg( m_fErr_Fp, stderr, "#ツールバージョン               : <%s>\n", m_sVersion );
	PrnMsg( m_fErr_Fp, stderr, "#建物の接続DB                   : <%s>\n", m_sDB_Building.c_str() );
	PrnMsg( m_fErr_Fp, stderr, "#建物のフィーチャクラス名       : <%s>\n", m_sFC_Building.c_str() );
	PrnMsg( m_fErr_Fp, stderr, "#メッシュの接続DB               : <%s>\n", m_sDB_Mesh.c_str() );
	PrnMsg( m_fErr_Fp, stderr, "#メッシュのフィーチャクラス名   : <%s>\n", m_sFC_Mesh.c_str() );
	PrnMsg( m_fErr_Fp, stderr, "#POI・注記の接続DB              : <%s>\n", m_sDB_POI.c_str() );
	PrnMsg( m_fErr_Fp, stderr, "#POI・注記のフィーチャクラス名  : <%s>\n", m_sFC_POI.c_str() );
	PrnMsg( m_fErr_Fp, stderr, "#不動産・防火対象物情報の接続DB : <%s>\n", m_sDB_RealEstate.c_str() );
	PrnMsg( m_fErr_Fp, stderr, "#不動産・防火対象物情報のフィーチャクラス名 : <%s>\n", m_sFC_RealEstate.c_str() );
	PrnMsg( m_fErr_Fp, stderr, "#メッシュリスト                 : <%s>\n", m_sMeshList.c_str() );
	PrnMsg( m_fErr_Fp, stderr, "#POI・注記データ優先度設定リスト: <%s>\n", m_sPriorityList.c_str() );
	PrnMsg( m_fErr_Fp, stderr, "#出力Shapeファイル名            : <%s>\n", m_sShapeFName.c_str() );
	PrnMsg( m_fErr_Fp, stderr, "#エラーログファイル             : <%s>\n", m_sLog_File.c_str() );
	PrnMsg( m_fErr_Fp, stderr, "#実行ログファイル               : <%s>\n", m_sExeLog_File.c_str() );

}

// 引数チェック
bool	main_routine::
arg_check ( int    argc,  //!< [IN] 引数個数
            char** argv ) //!< [IN] 引数列
{
	//-----------------------------------------------------------------------
	// ツールバージョン表示
	CVersion	a_cvTool_Var;
	m_sVersion	= a_cvTool_Var.GetFileVersion();
	//-----------------------------------------------------------------------

	options_description oOption("オプション");

	// 引数の書式を定義
	oOption.add_options()
	//	("db1,d", value<string>(&m_sSrc_Path), "削除処理対象DB接続1")
	// 【必須のオプション】
		("blddb", value<std::string>(&m_sDB_Building), "建物の接続DB")
		("bldfc", value<std::string>(&m_sFC_Building), "建物のフィーチャクラス名")
		("mlist", value<std::string>(&m_sMeshList), "メッシュリスト")
		("prlist", value<std::string>(&m_sPriorityList),"POI・注記データ優先度設定リスト")
		("outshp", value<std::string>(&m_sShapeFName),"出力Shapeファイル名")
		("log", value<std::string>(&m_sLog_File),"エラーログファイル")
		("exelog", value<std::string>(&m_sExeLog_File),"実行ログファイル")
	// 【任意のオプション】
		("meshdb", value<std::string>(&m_sDB_Mesh), "メッシュの接続DB")
		("meshfc", value<std::string>(&m_sFC_Mesh), "メッシュのフィーチャクラス名")
		("poidb", value<std::string>(&m_sDB_POI), "POI・注記の接続DB")
		("poifc", value<std::string>(&m_sFC_POI), "POI・注記のフィーチャクラス名")
		("redb", value<std::string>(&m_sDB_RealEstate), "不動産情報の接続DB")
		("refc", value<std::string>(&m_sFC_RealEstate), "不動産情報のフィーチャクラス名")
		("fpdb", value<std::string>(&m_sDB_FireProof), "防火対象物情報の接続DB")
		("fpfc", value<std::string>(&m_sFC_FireProof), "防火対象物情報のフィーチャクラス名");

	variables_map vArgmap;

	try {
		store(parse_command_line(argc, argv, oOption), vArgmap);
		notify(vArgmap);

		// 接続DB1指定が無い場合は終了
		if ( !vArgmap.count("blddb") || !vArgmap.count("bldfc") || !vArgmap.count("mlist") ||
			 !vArgmap.count("prlist") || !vArgmap.count("outshp") || !vArgmap.count("log") || !vArgmap.count("exelog")) {
			PrnMsg ( m_fErr_Fp, stderr, "ERROR:オプション指定を確認してください\n");
			return false;
		}

		// デフォルトでは建物DBの接続を使用するのでその設定を行う
		if( !vArgmap.count("meshdb") ) { m_sDB_Mesh = m_sDB_Building; }
		if( !vArgmap.count("meshfc") ) { m_sFC_Mesh = _T("REFERENCE.CITYMESH"); } // デフォルトは都市地図メッシュ
		if( !vArgmap.count("poidb") )  { m_sDB_POI = m_sDB_Building; }
		if( !vArgmap.count("poifc") )  { m_sFC_POI = m_sFC_Building; }
		if( !vArgmap.count("redb") ) { m_sDB_RealEstate = m_sDB_Building; }
		if( !vArgmap.count("refc") ) { m_sFC_RealEstate = m_sFC_Building; }
		if( !vArgmap.count("fpdb") ) { m_sDB_FireProof = m_sDB_Building; }
		if( !vArgmap.count("fpfc") ) { m_sFC_FireProof = m_sFC_Building; }

		// 実行フラグ処理
	//	if( vArgmap.count("l") ) { m_lExe_Flag |= ON_LOGFILE; }
	}catch (...) {
		PrnMsg ( m_fErr_Fp, stderr, "ERROR:オプションの指定に間違いがあります\n");
		return false;
	}

	// 実行ログファイルのオープン
	if( !Open_Log (m_sExeLog_File, &m_fExe_Fp) ) {
		PrnMsg ( stderr, NULL, "ERROR:実行ログファイルがオープンできない : <%s>\n", m_sExeLog_File.c_str() );
		return	false;
	}

	// エラーログファイルのオープン
	if( !Open_Log (m_sLog_File, &m_fErr_Fp) ) {
		PrnMsg ( stderr, NULL, "ERROR:エラーログファイルがオープンできない : <%s>\n", m_sLog_File.c_str() );
		return	false;
	}
	// Shapeファイルのオープン
	if( !m_mShpFile.Init ( m_sShapeFName, esriGeometryPoint ) ) {
		PrnMsg ( stderr, NULL, "ERROR:Shapeファイルがオープンできない : <%s>\n", m_sShapeFName.c_str() );
		return	false;
	}

	//-----------------------------------------------------------------------------------------------
	// 実行情報を出力
	PrintExecInfo();

	// 開始時間の設定
	m_eExeTime.Set_Start();

	//--------------------------------------------------------------------------
	// メッシュリストの読み込み
	//--------------------------------------------------------------------------
	if( !Read_List ( m_sMeshList, m_sMESH_List ) ) {
		PrnMsg ( m_fErr_Fp, stderr, "ERROR:メッシュリストの読み込みに失敗 : <%s>\n", m_sMeshList.c_str() );
		return false;
	}

	//--------------------------------------------------------------------------
	// 優先度リストの読み込み
	//--------------------------------------------------------------------------
	if( !m_pPriorityMgr.Init ( m_sPriorityList ) ) {
		PrnMsg ( m_fErr_Fp, stderr, "ERROR:優先度リストの読み込みに失敗 : <%s>\n", m_sPriorityList.c_str() );
		return false;
	}

	//--------------------------------------------------------------------------
	// DB接続関連、フィーチャクラスのオープン処理
	//--------------------------------------------------------------------------
	if( !DB_Open () ) {
		return false;
	}
	// DB接続にかかった時間の表示
	PrnMsg ( m_fErr_Fp, stderr, "#DB接続処理時間 = %0.2f(s)でした\n", (double)((m_eExeTime.Get_From_Start()) / 1000.0) );

	return true;
}

// リストの読み込み
bool main_routine::
Read_List ( const std::string& c_sFileName, //!< [in] ファイル名
            std::set<long>&    c_sList )    //!< [in] リスト（ID等に使用）
{
	const int bmax	= 512;
	FILE*     a_pFp;		
	char      a_cBuf[bmax];
	int       a_StrLen = 0;	

	a_pFp = fopen ( c_sFileName.c_str(), "r" );
	if(a_pFp == NULL) {
		return false;
	}

	while( fgets ( a_cBuf, bmax, a_pFp ) != NULL)
	{
		a_StrLen	= strlen ( a_cBuf );
		// 最後の"\n"を取り除く
		if(a_cBuf[a_StrLen - 1] == '\n') {
			a_cBuf[a_StrLen - 1] = '\0';
		}
		// コメント文を飛ばす
		if(a_cBuf[0] == '#' || a_cBuf[0] == '\0') {
			continue;
		}
		// リストに追加
		c_sList.insert ( atol ( a_cBuf ) );
	}
	fclose ( a_pFp );
	return true;
}

/** ------------------------------------------------------------------------------
	メッシュ矩形をGeoDBから取得する \
	※GeoDBへの接続が確立されていることが前提
--------------------------------------------------------------------------------*/
IGeometryPtr	main_routine::
GetMeshRect ( int c_iMeshCode ) // メッシュコード
{
/*
	_bstr_t		a_bsWhere_Str;
	char		a_tmpstr[30];	//　ここはなんとかしたい気がする
*/
	int			a_iDigit = 0;
	a_iDigit	= m_cCrd.GetMeshDigit( c_iMeshCode );

	if( a_iDigit != 2 && a_iDigit != 4 && a_iDigit != 6 && a_iDigit != 8 ) {
		return	( NULL );
	}
	CString csWhereStr; // 検索句

	csWhereStr.Format ("MESHCODE = %.2d", c_iMeshCode );

	// 検索用フィルタ設定
//	sprintf(a_tmpstr, "MESHCODE = %.2d", c_iMeshCode);		// メッシュコードを文字列に
//	a_bsWhere_Str	= a_tmpstr;		// stringをBSTRに変換
	
	IQueryFilterPtr		a_Sp_Filter	( CLSID_QueryFilter );	// 空間検索フィルタ
	a_Sp_Filter->put_WhereClause	( _bstr_t(csWhereStr) );// Where句の設定
	
	// サーチしてくる
	IFeatureCursorPtr	a_ipFCursor;
	if( FAILED (m_ipFC_Mesh->Search ( a_Sp_Filter, VARIANT_FALSE, &a_ipFCursor )) ) {
		PrnMsg (m_fErr_Fp, NULL, "%d\tデータの属性検索に失敗\n", c_iMeshCode);
		return NULL;
	}else {
		IFeaturePtr				a_ipFeature;
		while ( a_ipFCursor->NextFeature ( &a_ipFeature ) == S_OK )
		{
			IGeometryPtr		a_ipClip_Shp;

			if( FAILED(a_ipFeature->get_ShapeCopy ( &a_ipClip_Shp )) ) {	// 形状取得
				PrnMsg( m_fErr_Fp, NULL, "%d\tシェイプのコピーに失敗\n", c_iMeshCode);
				return NULL;
			}
			VARIANT_BOOL	a_bResult;	
			a_ipClip_Shp->get_IsEmpty( &a_bResult );
			if(a_bResult == VARIANT_TRUE) {	// クリップ結果がNULLならメッシュ外
				PrnMsg( m_fErr_Fp, NULL, "%d\tメッシュ矩形が空\n", c_iMeshCode);
				return NULL;
			}
			return	( a_ipClip_Shp );
		}
	}
	return	NULL;
}

/** ------------------------------------------------------------------------------
	フィーチャを出力用のフィーチャにする
	@return	データ数（失敗＝-1）
--------------------------------------------------------------------------------*/
int main_routine::
MakeDataContainer ( long                     cMeshCode,       //!< [in] 現メッシュコード
                    const IFeatureCursorPtr& c_ipFCursor,     //!< [IN] カーソル
                    mIFeatureContainer&      c_mWork_Data,    //!< [IN] 出力用データ
                    bool                     bLeftDownPtMode )//!< [in] 一番XYの小さいポイントが当該メッシュに含まれる場合のみ格納する場合
{
	int	a_iCount	= 0;

	long lMeshLevel = 0;
	if( 1000 <= cMeshCode && cMeshCode < 10000 ) {
		lMeshLevel = 1;
	}else if( 100000 <= cMeshCode && cMeshCode < 1000000 ) {
		lMeshLevel = 2;
	}else if( 10000000 <= cMeshCode && cMeshCode < 100000000 ) {
		lMeshLevel = 64;
	}else {
		return -1; // エラー扱い
	}

	// データvectorコンテナに落とす
	if( c_ipFCursor != NULL ) {

		IFeaturePtr		a_ipCur_Feat;
		while ( c_ipFCursor->NextFeature ( &a_ipCur_Feat ) == S_OK )
		{
			long	a_lOID = 0;
			if( FAILED ( a_ipCur_Feat->get_OID ( &a_lOID ) ) ) {return false;}

			IGeometryPtr	a_ipWorkGeo;	//!< 作業用
			if( FAILED ( a_ipCur_Feat->get_ShapeCopy (&a_ipWorkGeo) )) {return false;}

			std::set<XYPt> sPts;

			if( bLeftDownPtMode == true ) {
				IPointCollectionPtr ipPts(a_ipWorkGeo);
				long lPtCount = 0;
				if( FAILED ( ipPts->get_PointCount(&lPtCount) ) ) {return false;}
				WKSPoint* wTmpPts = new WKSPoint[lPtCount];
				if( FAILED ( ipPts->QueryWKSPoints(0, lPtCount, wTmpPts) ) ) {
					delete wTmpPts;
					return false;
				}
				for( int i = 0; i < lPtCount; ++i )
				{
					XYPt xTmp;
					xTmp.mX = wTmpPts[i].X;
					xTmp.mY = wTmpPts[i].Y;
					sPts.insert ( xTmp );
				}
				delete wTmpPts;

				// 一番XYが小さい点がメッシュ内かの判定
				int iTmpMeshCode = 0, tX = 0, tY = 0;
				m_cCrd.LLtoMesh ( sPts.begin()->mX, sPts.begin()->mY, lMeshLevel, &iTmpMeshCode, &tX, &tY, 1 ); // 1000000不許可
				if( cMeshCode != iTmpMeshCode ) {
#ifdef _DEBUG
					PrnMsg( m_fErr_Fp, NULL, "0\t%s\t%d\t\t\tINFO\t\tこのメッシュでは対象外\t⇒%d\n",
						m_sFC_Building.c_str(), a_lOID, iTmpMeshCode );
#endif
					continue; // 当該メッシュでは飛ばす
				}
			}

			c_mWork_Data.insert ( mIFeatureContainer::value_type ( a_lOID, a_ipWorkGeo) );
			a_iCount++;
		}
	}else {
		a_iCount = -1;
	}
	return	a_iCount;
}

// ポイント情報のコンテナを作成する
int main_routine::
MakePointContainer ( const IFeatureCursorPtr& cipFCursor,    //!< [in] フィーチャカーソル
                     CString&                 cNameFidldName,//!< [in] 名称フィールド名指定
                     mPointInfo_XY&           cmWork_Data,   //!< [out] 作業用データ
                     long                     clDataType )   //!< [in] データタイプ(PriorityManagerで定義のもの)
{
	int	a_iCount	= 0;

	IFieldsPtr ipFields;
	if( FAILED ( cipFCursor->get_Fields ( &ipFields ) ) ) {
		return -1;
	}
	long lFidldIndex = -1;
	// 名称のフィールドを見つけてくる
	if( FAILED ( ipFields->FindField ( _bstr_t(cNameFidldName), &lFidldIndex ) ) ) {
		return -1;
	}

	// POIモードの場合は、コンテンツコードを取得するのでフィールド取得。
	long lContentsCodeIndex = -1;
	// （それ以外は固定でコンテンツコードは-1に設定）
	if( clDataType == PriorityManager::TYPE_POI ) {
		if( FAILED ( ipFields->FindField ( _bstr_t("CONTENTS_CODE"), &lContentsCodeIndex ) ) ) {
			return -1;
		}
	}

	// POIモード以外の場合は、データタイプの判別フィールドを取得する
	long lSourceTypeIndex = -1;
	if( clDataType == -1 ) {
		if( FAILED ( ipFields->FindField ( _bstr_t("SOURCETYPE_C"), &lSourceTypeIndex ) ) ) {
			return -1;
		}
	}

	// データvectorコンテナに落とす
	if( cipFCursor != NULL ) {

		IFeaturePtr		a_ipCur_Feat;
		while ( cipFCursor->NextFeature ( &a_ipCur_Feat ) == S_OK )
		{
			PointInfo_XY pCurInfo; // ポイント情報
			long	a_lOID = 0;
			if( FAILED (a_ipCur_Feat->get_OID ( &a_lOID ) ) ) {return false;}

			CComVariant cName;
			if( FAILED (a_ipCur_Feat->get_Value(lFidldIndex, &cName ) ) ) {return false;}

			IGeometryPtr	a_ipWorkGeo;	// 作業用
			if( FAILED (a_ipCur_Feat->get_ShapeCopy (&a_ipWorkGeo) ) ) {return false;}

			// POIの場合はコンテンツコードを取得する
			long lContents_Code = -1;
			if( clDataType == PriorityManager::TYPE_POI ) {
				CComVariant cContents_Code;
				if( FAILED (a_ipCur_Feat->get_Value ( lContentsCodeIndex, &cContents_Code ) ) ) {return false;}
				lContents_Code = cContents_Code.lVal;
			}
			// POI以外の場合は、ソースタイプを取得する
			long lSourceType = PriorityManager::TYPE_POI;
			if( clDataType == -1 ) {
				CComVariant cSourceType;
				if( FAILED (a_ipCur_Feat->get_Value ( lSourceTypeIndex, &cSourceType ) ) ) {return false;}
				switch ( cSourceType.lVal )
				{
				case	1: lSourceType = PriorityManager::TYPE_REALESTATE; break; // 不動産情報
				case	2: lSourceType = PriorityManager::TYPE_FIREPROOF; break;  // 防火対象物情報
				default: 
#ifdef _DEBUG
					PrnMsg( m_fErr_Fp, stderr, "未対応のタイプ\t%d\t%d\n", a_lOID, cSourceType.lVal );
#endif
					continue;
				}
			}
			std::string sAttrName; // 種別名称
			long lPriority = m_pPriorityMgr.GetPriority ( lSourceType, lContents_Code, sAttrName );
			if( lPriority == -1 ) {
#ifdef _DEBUG
				PrnMsg( m_fErr_Fp, stderr, "優先度が取得できない\t%d\t%d\n", clDataType, lContents_Code );
#endif
				continue;
			}
			// ソート用コンテナに格納
			pCurInfo.mlDataType = lSourceType; // データタイプ				
			pCurInfo.mlContentsCode = lContents_Code;
			pCurInfo.mlPriority = lPriority;  // 優先度
			pCurInfo.mlOID      = a_lOID;     // ID
			pCurInfo.mcsName    = cName.bstrVal;// 名称
			pCurInfo.mipGeom    = a_ipWorkGeo;// ジオメトリ

			// POIで、文字列が「ヌル」の場合はスキップする 2014/03/07 要望
			if(  clDataType == PriorityManager::TYPE_POI && pCurInfo.mcsName == _T("ヌル") ) {
				continue;
			}


			// XYを追加
			if( FAILED ( IPointPtr(a_ipWorkGeo)->get_X(&pCurInfo.mX)) ){return false;}
			if( FAILED ( IPointPtr(a_ipWorkGeo)->get_Y(&pCurInfo.mY)) ){return false;}

			cmWork_Data.insert ( pCurInfo );
		//	cmWork_Data.insert ( mIFeatureContainer::value_type ( a_lOID, a_ipCur_Feat) );
			++a_iCount;
		}
	}else {
		a_iCount = -1;
	}
	return	a_iCount;
}

/**
* @brief XYソートしたポイント列から、ポリゴンに含まれるポイントの、優先度ソートのコンテナを作成
* @return	成功=true,失敗=false
*/
bool   main_routine::
GetInnerPoint ( IGeometryPtr&   cipPolygon,   //!< [in] ポリゴンジオメトリ
                mPointInfo_XY&  cmPoint_Data, //!< [in] ポイント情報コンテナ(XYソート)
                mPointInfo&     cmOutput )    //!< [out] ポイント情報コンテナ(優先度ソート)
{
	// ポリゴンの外接矩形を取得
	IEnvelopePtr ipEnvelope;
	if( FAILED ( cipPolygon->get_Envelope ( &ipEnvelope ) ) ) { return false; }

	// 最小・最大位置情報を取得
	XYPt xyMin, xyMax;
	if( FAILED (ipEnvelope->get_XMin ( &xyMin.mX )) ) { return false; }
	if( FAILED (ipEnvelope->get_YMin ( &xyMin.mY )) ) { return false; }
	if( FAILED (ipEnvelope->get_XMax ( &xyMax.mX )) ) { return false; }
	if( FAILED (ipEnvelope->get_YMax ( &xyMax.mY )) ) { return false; }

	// 検索キー作成(Y方向は、ポイントの最小から最大までを検索対象としたいので０～９０で）
	PointInfo_XY pMinKey, pMaxKey;
	pMinKey.mX = xyMin.mX;
	pMinKey.mY = 0;
	pMaxKey.mX = xyMax.mX;
	pMaxKey.mY = 90;

	// 検索
	mPointInfo_XY::const_iterator iIterXY_Begin, iIterXY_End; // XYソートの最初と最後
	iIterXY_Begin = cmPoint_Data.lower_bound ( pMinKey );
	iIterXY_End   = cmPoint_Data.upper_bound ( pMaxKey );

	IRelationalOperatorPtr ipRel = cipPolygon;

	// Y方向で範囲を絞ったコンテナを作成する
	mPointInfo_XY::const_iterator iIterXY = iIterXY_Begin;
	for( iIterXY = iIterXY_Begin; iIterXY != cmPoint_Data.end(); ) // コンテナ自体の終了判定もしないとだめ
	{
		// X方向は既にフィルタリングしているのでY方向だけでOK
		if( iIterXY->mY >= xyMin.mY && iIterXY->mY <= xyMin.mY ) {
			++iIterXY;
			continue;
		}

		PointInfo pAdd = *iIterXY;

		VARIANT_BOOL vRet1, vRet2;
		if( FAILED ( ipRel->Disjoint( pAdd.mipGeom, &vRet1 ) ) ) {
			return false;
		}

		if( vRet1 == VARIANT_FALSE ) {
			if( FAILED ( ipRel->Contains ( pAdd.mipGeom, &vRet2 ) ) ) {
				return false;
			}
			if( vRet2 == VARIANT_TRUE ) { // 建物が注記を含む場合
				cmOutput.insert ( pAdd );
			}
		}
		
		if( iIterXY == iIterXY_End ) { // Endまで処理したら終了
			break;
		}
		++iIterXY;
	}
	return true;
}

/**
* @brief データ出力
* @note  
* @return	成功=true,失敗=false
*/
bool   main_routine::
PrintOut( mIFeatureContainer& cmBuilding_Data, //!< [in] 建物データコンテナ
          mPointInfo_XY&      cmPoint_Data )   //!< [out] ポイント情報コンテナ
{
	// 建物のループ
	mIFeatureContainer::const_iterator iBuidingIter;
	for( iBuidingIter = cmBuilding_Data.begin(); iBuidingIter != cmBuilding_Data.end(); ++iBuidingIter )
	{
		IGeometryPtr ipCurBuilding = (*iBuidingIter).second;

		// 建物内での優先度決定用コンテナ
		mPointInfo mInnerPoints;

		// ポリゴンに内包される点を取得
		if( !GetInnerPoint ( ipCurBuilding, cmPoint_Data, mInnerPoints ) ) {
			return false;
		}

		// 含まれる点が無ければ次の家形へ！！
		if( mInnerPoints.size() == 0 ) continue;

		// ここまでに優先度順にならんだデータが作成されるので、出力する
		mPointInfo::const_iterator iOutputIter;
		long lPtCount = 0;
		for( iOutputIter = mInnerPoints.begin(); iOutputIter != mInnerPoints.end(); ++iOutputIter, ++lPtCount )
		{
			if( lPtCount == 0 ) { // 最優先のポイントだけ出力する
				// 形状の出力
				m_mShpFile.OuttoShapeFile ( IGeometryPtr((*iOutputIter).mipGeom), (*iOutputIter).mlOID, (*iBuidingIter).first, CString((*iOutputIter).mcsName) );
			}

			IPointPtr ipPt = (*iOutputIter).mipGeom;
			double dX = 0, dY = 0;
			if( FAILED ( ipPt->get_X ( &dX )) ) {return false;}
			if( FAILED ( ipPt->get_Y ( &dY )) ) {return false;}

			string sType;
			switch( (*iOutputIter).mlDataType )
			{
			case	PriorityManager::TYPE_POI:       sType = "POI"; break;
			case	PriorityManager::TYPE_REALESTATE:sType = "不動産"; break;
			case	PriorityManager::TYPE_FIREPROOF: sType = "防火施設"; break;
			default: return false;
			}

			PrnMsg( m_fExe_Fp, NULL, "0\t%s\t%d\t%0.10f\t%0.10f\tINFO\t\t%s\t<%s>\t%d\t%d\t%s\n",
				m_sFC_Building.c_str(), (*iBuidingIter).first, dX, dY, lPtCount==0?"家形内のポイント（代表）":"家形内のポイント",
				sType.c_str(), (*iOutputIter).mlContentsCode, (*iOutputIter).mlOID, (*iOutputIter).mcsName );
		}
	}
	return true;
}

// 建物形状から外接矩形を作成する
IGeometryPtr   main_routine::
MakeBuildingsEnvelope ( const mIFeatureContainer& cmWork_Data ) //!< [in] 作業用データ
{
	ISpatialReferencePtr ipSpRef; // 空間参照を取得しておく

	double dXMin = 0, dYMin = 0, dXMax = 0, dYMax = 0;

	long lCount = 0;
	mIFeatureContainer::const_iterator iIter;
	for( iIter = cmWork_Data.begin(), lCount = 0; iIter != cmWork_Data.end(); ++iIter )
	{
		IGeometryPtr ipCurGeom = (*iIter).second;

		// 左下、右上を取り出す
		IEnvelopePtr ipCurEnv;
		if( FAILED (ipCurGeom->get_Envelope ( &ipCurEnv ) ) ) {
			return NULL;
		}
		double dXMinTmp = 0, dYMinTmp = 0, dXMaxTmp = 0, dYMaxTmp = 0;
		if( FAILED (ipCurEnv->get_XMin ( &dXMinTmp ) ) ) {return NULL;}
		if( FAILED (ipCurEnv->get_YMin ( &dYMinTmp ) ) ) {return NULL;}
		if( FAILED (ipCurEnv->get_XMax ( &dXMaxTmp ) ) ) {return NULL;}
		if( FAILED (ipCurEnv->get_YMax ( &dYMaxTmp ) ) ) {return NULL;}
		if( lCount == 0 ) {
			dXMin = dXMinTmp;
			dYMin = dYMinTmp;
			dXMax = dXMaxTmp;
			dYMax = dYMaxTmp;
			if( FAILED(ipCurGeom->get_SpatialReference ( &ipSpRef )) ) { return NULL;}
		}else {
			// Minから格納
			if( dXMin > dXMinTmp ) { dXMin = dXMinTmp; }
			if( dYMin > dYMinTmp ) { dYMin = dYMinTmp; }
			// 次にMax
			if( dXMax < dXMaxTmp ) { dXMax = dXMaxTmp; }
			if( dYMax < dYMaxTmp ) { dYMax = dYMaxTmp; }
		}
		++lCount; // データがある場合だけカウントする
	}

	// ポリゴン作成して返却


	IPolygonPtr          ipRectPoly(CLSID_Polygon); // 矩形ポリゴン
	IPointCollectionPtr	 pRecPolyPts(ipRectPoly);

	for(int i = 0; i < 5; ++i )
	{
		IPointPtr	ipPt(CLSID_Point);
		switch (i)
		{
		case	0:	ipPt->put_X ( dXMin ); ipPt->put_Y ( dYMin ); break;	// 左下
		case	1:	ipPt->put_X ( dXMin ); ipPt->put_Y ( dYMax ); break;	// 左上
		case	2:	ipPt->put_X ( dXMax ); ipPt->put_Y ( dYMax ); break;	// 右上
		case	3:	ipPt->put_X ( dXMax ); ipPt->put_Y ( dYMin ); break;	// 右下
		case	4:	ipPt->put_X ( dXMin ); ipPt->put_Y ( dYMin ); break;	// 左下
		}
		pRecPolyPts->AddPoint(ipPt);
	}
	
	// 空間参照の付与とスナップ
	if( FAILED ( ipRectPoly->putref_SpatialReference ( ipSpRef ) ) ) { return NULL; }
	if( FAILED ( ipRectPoly->SnapToSpatialReference ( ) ) ) { return NULL; }

	return (IGeometryPtr(pRecPolyPts));
}

//	指定フィーチャに対してesriSpatialRelEnumで指定した関係を持つデータを
//	とってくる
IFeatureCursorPtr   main_routine::
Search_Feature ( const IFeatureClassPtr& c_ipFeCls,     //!< [in] フィーチャクラス
                 const IGeometryPtr&     c_ipGeo,       //!< [in] 検索用ジオメトリ
                 esriSpatialRelEnum      c_ipSpatialRel,//!< [in] 関係指定
                 std::string             c_sWhere,      //!< [in] Where句指定
                 long&                   c_iCount )     //!< [out] 検索数
{
	ISpatialFilterPtr	a_ipFilter(CLSID_SpatialFilter);
	CComBSTR			a_ccSpCol;

	if (FAILED(c_ipFeCls->get_ShapeFieldName ( &a_ccSpCol )) || // シェイプフィールド名取得
	    FAILED(a_ipFilter->putref_Geometry ( c_ipGeo )) ||      // サーチ用のジオメトリ
	    FAILED(a_ipFilter->put_GeometryField ( a_ccSpCol )) ||  // ジオメトリフィールド
	    FAILED(a_ipFilter->put_SpatialRel ( c_ipSpatialRel )) ||// SpatialRel()
	    FAILED(a_ipFilter->put_SearchOrder ( esriSearchOrderSpatial )) ) { // サーチオーダー
		PrnMsg( m_fErr_Fp, stderr, "ERROR:Filterの設定に失敗[Search_Feature]\n");
		return	( NULL );
	}

	if( c_sWhere.size() > 1 ) {
		if ( FAILED ( a_ipFilter->put_WhereClause ( _bstr_t ( c_sWhere.c_str() ) ) ) )	{
			PrnMsg( m_fErr_Fp, stderr, "ERROR:検索用WHERE句の設定ができない[Search_Feature]" );
			return	( NULL );
		}
	}
	// データカウントをやる
	long		a_lData_Count	= 0;
	if ( FAILED ( c_ipFeCls->FeatureCount ( (IQueryFilterPtr)a_ipFilter, &a_lData_Count ) ) ) {
		PrnMsg( m_fErr_Fp, stderr, "ERROR:検索データ数が取得できない[Search_Feature]\n");
		return	( NULL );
	}
	c_iCount	= a_lData_Count;

	IFeatureCursorPtr	a_ipFeCsr;	//	返却用フィーチャカーソル
	if (FAILED( c_ipFeCls->Search((IQueryFilterPtr)a_ipFilter, VARIANT_FALSE, &a_ipFeCsr)) ) {
		PrnMsg( m_fErr_Fp, stderr, "ERROR:データのサーチができない[Search_Feature]\n");
		return	( NULL );
	}
	return	( a_ipFeCsr );
}

/**
 * @brief 代表点を取得
 * @return 
 * @return	成功	true
 * @return	成功	false
 */
bool main_routine::
GetPoint ( const IGeometryPtr& c_ipGeom, //!< [in] ジオメトリ
		   double&             c_dX,     //!< [out] 経度
		   double&             c_dY )    //!< [out] 経度
{
	// 初期化
	c_dX = 0.0;
	c_dY = 0.0;

	// ジオメトリタイプ
	esriGeometryType a_eGeomType;
	if( FAILED ( c_ipGeom->get_GeometryType ( &a_eGeomType ) ) ) {
		return false;
	}

	double a_dX = 0.0, a_dY = 0.0;

	switch ( a_eGeomType )
	{
	case esriGeometryPoint:
		{
			IPointPtr a_ipPoint ( c_ipGeom );
			a_ipPoint->get_X ( &a_dX );
			a_ipPoint->get_Y ( &a_dY );
		}
		break;
	case esriGeometryPolyline:
		{
			IPolylinePtr a_ipPolyline( c_ipGeom );
			double a_dLen = 0.0;
			a_ipPolyline->get_Length( &a_dLen );
			IPointPtr a_ipPt(CLSID_Point);
			a_ipPolyline->QueryPoint( esriNoExtension, a_dLen/2.0, VARIANT_FALSE, a_ipPt );
			a_ipPt->get_X ( &a_dX );
			a_ipPt->get_Y ( &a_dY );
		}
		break;
	case esriGeometryPolygon:
		{
			IAreaPtr a_ipArea ( (IPolygonPtr)c_ipGeom );
			IPointPtr a_ipPt;
			a_ipArea->get_LabelPoint ( &a_ipPt );
			a_ipPt->get_X ( &a_dX );
			a_ipPt->get_Y ( &a_dY );
		}
		break;
	}
	c_dX = a_dX;
	c_dY = a_dY;

	return true;
}

// 変換実行部
bool main_routine::
exec	( void )
{
	// 開始時間の設定
	m_eExeTime.Set_Start();

	bool bIsOK = true;

	long a_lCount = 0;
	std::set<long>::const_iterator a_iIter;
	for( a_iIter = m_sMESH_List.begin(); a_iIter != m_sMESH_List.end(); ++a_iIter, ++a_lCount )
	{
		m_eExeTime.Set_Lap_Start();

		long lCurMesh = *a_iIter;

		PrnMsg( stdout, NULL, "#メッシュ <%d> 処理中..", lCurMesh );

		// メッシュ矩形の取り出し
		IGeometryPtr	ipMeshRect;
		ipMeshRect = GetMeshRect( lCurMesh );
		if( ipMeshRect == NULL ) {	// メッシュ矩形が無い場合
			PrnMsg( m_fErr_Fp, NULL, "%d\tのメッシュ矩形が見つかりませんでした\n", lCurMesh );
			bIsOK = false;
			continue;
		}
		// 空間参照を取得する
		if( m_ipMeshSpRef == NULL ) {
			if( FAILED (ipMeshRect->get_SpatialReference ( &m_ipMeshSpRef )) ) {
				PrnMsg( m_fErr_Fp, stderr, "%d\t空間参照の取得に失敗\n", lCurMesh );
				bIsOK = false;
				continue;
			}
		}

		// メッシュ矩形で建物形状を取得する
		IFeatureCursorPtr ipBuildingCursor;
		long lBuildingsCount = 0;
		ipBuildingCursor = Search_Feature ( m_ipFC_Building, ipMeshRect, esriSpatialRelIntersects, "", lBuildingsCount );
		if( ipBuildingCursor == NULL ) {
			PrnMsg( m_fErr_Fp, stderr, "%d\t空間検索に失敗(建物)\n", lCurMesh );
			bIsOK = false;
			continue;
		}
		// 建物をコンテナに格納
		mIFeatureContainer mWorkData;
		long lBldCount = MakeDataContainer ( lCurMesh, ipBuildingCursor, mWorkData, true );
		if( lBldCount < 0 ) {
			PrnMsg( m_fErr_Fp, stderr, "%d\tデータコンテナ作成に失敗\n", lCurMesh );
			bIsOK = false;
			continue;
		}

		// 建物形状から外接矩形を作成する
		IGeometryPtr ipBuildingsEnvelope = MakeBuildingsEnvelope ( mWorkData );
		if( ipBuildingsEnvelope == NULL ) {
			PrnMsg( m_fErr_Fp, stderr, "%d\t建物外接矩形作成に失敗\n", lCurMesh );
			bIsOK = false;
			continue;
		}

		// 外接矩形でポイントを検索する
		// POI取得
		IFeatureCursorPtr ipPOICursor;
		long lPOICount = 0;
		ipPOICursor = Search_Feature ( m_ipFC_POI, ipBuildingsEnvelope, esriSpatialRelIntersects, "", lPOICount );
		if( ipPOICursor == NULL ) {
			PrnMsg( m_fErr_Fp, stderr, "%d\t空間検索に失敗(POI)\n", lCurMesh );
			bIsOK = false;
			continue;
		}
		IFeatureCursorPtr ipRealEstateCursor;
		long lRealEstateCount = 0;
		ipRealEstateCursor = Search_Feature ( m_ipFC_RealEstate, ipBuildingsEnvelope, esriSpatialRelIntersects, "", lRealEstateCount );
		if( ipRealEstateCursor == NULL ) {
			PrnMsg( m_fErr_Fp, stderr, "%d\t空間検索に失敗(POI)\n", lCurMesh );
			bIsOK = false;
			continue;
		}

		// 建物とポイントの内包関係を調べる
		// 建物をコンテナに格納
		mPointInfo_XY mWorkPointsData;
		// POIのデータを取得
		long lWorkPOICount = MakePointContainer ( ipPOICursor, CString(_T("NAME")), mWorkPointsData, PriorityManager::TYPE_POI );
		if( lWorkPOICount < 0 ) {
			PrnMsg( m_fErr_Fp, stderr, "%d\tデータコンテナ作成に失敗(POI)\n", lCurMesh );
			bIsOK = false;
			continue;
		}
		// 不動産情報を取得
		long lWorkRealEstateCount = MakePointContainer ( ipRealEstateCursor, CString(_T("NAME")), mWorkPointsData  );
		if( lWorkRealEstateCount < 0 ) {
			PrnMsg( m_fErr_Fp, stderr, "%d\tデータコンテナ作成に失敗(不動産情報・防火対象物情報)\n", lCurMesh );
			bIsOK = false;
			continue;
		}
		// 内包関係を調べ、情報を出力する
		if( !PrintOut ( mWorkData, mWorkPointsData ) ) {
			PrnMsg( m_fErr_Fp, stderr, "%d\t情報出力に失敗\n", lCurMesh );
			bIsOK = false;
			continue;

		}
		DWORD          a_dMake_Finish	= 0;
		a_dMake_Finish = timeGetTime();

		PrnMsg(stdout, NULL, "処理時間 = %0.2f(s)\n", m_eExeTime.Get_Lap() / 1000.0 );

		// 作業中断用
		if(_kbhit()){
			int aKey = _getch();
			if(aKey == 'Q'){
				PrnMsg ( stdout, NULL, "#更新処理を中断します %d まで完了\n", lCurMesh );
				break;
			}
		}
	}
	PrnMsg( stdout, NULL, "\n" );			

	//　実行部終了
	// 終了時間の取得
	PrnMsg	( m_fErr_Fp, stderr, "#データ変換総時間 = %0.2f(s)でした\n",
						(double)((m_eExeTime.Get_From_Start()) / 1000.0) );
	return	( true );
}
