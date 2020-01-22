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

#pragma once

#include "Arguments.h"
#include "FeatureObjects.h"
#include "util.h"
#include "OutputLog.h"
#include "NameChk.h"
#include "FieldMap.h"
#include <sindy/workspace.h>

/**
 * @brief   メインクラス
 */
class CCheckBuildNamePoint
{

public:

	/**
	 * @brief   コンストラクタ
	 * @param   args [in]  引数
	 */
	CCheckBuildNamePoint( const Arguments& args ) :
		m_strBlNameDB(    (args.m_strBlNameDB).c_str() ), 
		m_strBuildingDB(  (args.m_strBuildingDB).c_str() ), 
		m_strAnnoDB(      (args.m_strAnnoDB).c_str() ), 
		m_strMeshDB(      (args.m_strMeshDB).c_str() ), 
		m_strOutput(      (args.m_strOutput).c_str() ), 
		m_strMeshList(    (args.m_strMeshList).c_str() ), 
		m_strNgBuild(     (args.m_strNgBuild).c_str() ), 
		m_strErrWord(     (args.m_strErrWord).c_str() ), 
		m_strWarnWord(    (args.m_strWarnWord).c_str() ), 
		m_strBldcOKWord(  (args.m_strBldcOKWord).c_str() ),
		m_strBldcNGWord(  (args.m_strBldcNGWord ).c_str() ),
		m_chkFixclass(     args.m_chkFixclass ),
		m_strReplaceWord( (args.m_strReplaceWord).c_str() ), 
		m_strJoinAnno(    (args.m_strJoinAnno).c_str() ),
		m_strExclude_mark((args.m_strExclude_mark).c_str() ),
		m_bufferRange(     args.m_bufferRange )
	{};

	/**
	 * @brief   デストラクタ
	 */
	virtual ~CCheckBuildNamePoint(){};

	/**
	 * @brief   実行
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool execute();

private:

	/**
	 * @brief   初期化
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool init();

	/**
	 * @brief   処理に必要なレイヤを開く
	 * @param   blNameWsName   [in]  ビル名称ポイントワークスペース名
	 * @param   buildingWsName [in]  都市建物ワークスペース名
	 * @param   annoWsName     [in]  注記ワークスペース名
	 * @param   meshWsName     [in]  メッシュワークスペース名
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool OpenLayers( const CString& blNameWsName, const CString& buildingWsName, const CString& annoWsName, const CString& meshWsName )
	{
		using namespace sindy::schema;
		IWorkspacePtr ipBlNameWS, ipBuildingWS, ipAnnoWS, ipMeshWS;

		if( ! OpenWorkspace( blNameWsName, ipBlNameWS ) )
			return false;
		if( ! OpenFeatureClass( sj::buildingname_point::kTableName, ipBlNameWS, m_ipBlNameFC, m_blNameFM ) )
			return false;

		if( ! OpenWorkspace( buildingWsName, ipBuildingWS ) )
			return false;
		if( ! OpenFeatureClass( building::kTableName, ipBuildingWS, m_ipBuildingFC, m_buildingFM ) )
			return false;

		if( ! OpenWorkspace( annoWsName, ipAnnoWS ) )
			return false;
		if( ! OpenFeatureClass( city_annotation::kTableName, ipAnnoWS, m_ipCityAnnoFC, m_cityAnnoFM ) )
			return false;

		if( ! OpenWorkspace( meshWsName, ipMeshWS ) )
			return false;
		if( ! OpenFeatureClass( citymesh::kTableName, ipMeshWS, m_ipCityMeshFC, m_cityMeshFM ) )
			return false;

		return true;
	};

	/**
	 * @brief   ワークスペースオープン
	 * @param   name        [in]  ワークスペース名
	 * @param   ipWorkspace [out] ワークスペース
	 * @retval  true  オープン成功
	 * @retval  false オープン失敗
	 */
	bool OpenWorkspace( const CString& name, IWorkspacePtr& ipWorkspace )
	{
		ipWorkspace = sindy::create_workspace( name );
		if( !ipWorkspace ){
			std::cerr << "#ERROR Open Workspace Fail:" << CT2CA( name ) << std::endl;
			return false;
		}
		return true;
	};

	/**
	 * @brief   フィーチャクラスオープン＆フィールドマップ作成
	 * @param   name           [in]  フィーチャクラス名
	 * @param   ipWorkspace    [in]  ワークスペース
	 * @param   ipFeatureClass [out] フィーチャクラス
	 * @param   fieldMap       [out] フィールドマップ
	 * @retval  true  オープン成功
	 * @retval  false オープン失敗
	 */
	bool OpenFeatureClass(const CString& name, const IWorkspacePtr& ipWorkspace, IFeatureClassPtr& ipFeatureClass, CFieldMap& fieldMap )
	{
		((IFeatureWorkspacePtr)ipWorkspace)->OpenFeatureClass( _bstr_t( name ), &ipFeatureClass );
		if( !ipFeatureClass ){
			std::cerr << "#ERROR Open Feature Class Fail:" << CT2CA( name ) << std::endl;
			return false;
		}
		fieldMap.CreateFieldMap( ipFeatureClass->GetFields() );

		return true;
	};

	/**
	 * @brief   テーブルオープン＆フィールドマップ作成
	 * @param   name           [in]  テーブル名
	 * @param   ipWorkspace    [in]  ワークスペース
	 * @param   ipTable        [out] テーブル
	 * @param   fieldMap       [out] フィールドマップ
	 * @retval  true  オープン成功
	 * @retval  false オープン失敗
	 */
	bool OpenTable(const CString& name, const IWorkspacePtr& ipWorkspace, ITablePtr& ipTable, CFieldMap& fieldMap )
	{
		((IFeatureWorkspacePtr)ipWorkspace)->OpenTable( _bstr_t( name ), &ipTable );
		if( !ipTable ){
			std::cerr << "#ERROR Open Table Fail:" << CT2CA( name ) << std::endl;
			return false;
		}
		fieldMap.CreateFieldMap( ipTable->GetFields() );

		return true;
	};

	/**
	 * @brief   与えたメッシュ隣接9メッシュリストを作成する
	 * @param   mesh     [in]  取得したい中心メッシュ
	 * @param   meshlist [out] 取得したメッシュリストのset
	 */
	void Get9Mesh( long mesh, std::set<long>& meshlist );

	/**
	 * @brief   ログヘッダ出力
	 */
	void OutputLogHeader();

	/**
	 * @brief   メッシュリストから、結合したメッシュポリゴンを取得
	 * @param   mesh           [in]  ipMeshGeoに取りたいメッシュコード
	 * @param   meshlist       [in]  ipUnionMeshGeoに取りたいメッシュリスト
	 * @param   ipMeshGeo      [out] 1メッシュのメッシュジオメトリ
	 * @param   ipUnionMeshGeo [out] 結合したメッシュジオメトリ
	 * @retval  true  取得成功
	 * @retval  false 取得失敗
	 */
	bool GetMeshPolygons( long mesh, const std::set<long>& meshlist, IGeometryPtr& ipMeshGeo, IGeometryPtr& ipUnionMeshGeo );

	/**
	 * @brief   与えられたメッシュ形状内のオブジェクトを取得
	 * @param   ipMeshGeo      [in]  メッシュポリゴンのジオメトリ
	 * @param   fields         [in]  値を取得したいフィールド名のVec
	 * @param   ipFeatureClass [in]  取得したいフィーチャクラス
	 * @param   fieldMap       [in]  フィールドマップ
	 * @param   geoMap         [out] 取得したオブジェクトMap(キー：ObjectID)
	 * @retval  true  取得成功
	 * @retval  false 取得失敗
	 */
	bool GetObjInMesh( const IGeometryPtr& ipMeshGeo, const std::vector<CString>& fields, const IFeatureClassPtr& ipFeatureClass, const CFieldMap& fieldMap, std::map<long, GeoClass>& geoMap );

	/**
	 * @brief   フィールド名分の値を取得し、オブジェクトコンテナに詰める
	 * @param   ipFeature [in]  取得したいフィーチャ
	 * @param   fields    [in]  取得したいフィールド名のVec
	 * @param   fieldMap  [in]  フィールドマップ
	 * @param   obj       [out] オブジェクトコンテナ
	 * @retval  true  取得成功
	 * @retval  false 取得失敗
	 */
	bool GetFieldValues( const IFeaturePtr& ipFeature, const std::vector<CString>& fields, const CFieldMap& fieldMap, GeoClass& obj );

	/**
	 * @brief   R-Treeを生成する
	 * @param   geoMap         [in]  対象フィーチャクラス
	 * @param   rTree          [out] 結果のR-Tree(冒頭の[r]は参照の意味ではない)
	 */
	void createRTree( const std::map<long, GeoClass>& geoMap, BGRTree& rTree );

	/**
	 * @brief   R-Treeから、与えた箱の範囲に引っかかるID群を取得
	 * @param   box   [in]    取得したい範囲(エンベロープ)
	 * @param   rTree [in]    取得してくる対象のR-Tree
	 * @retval  set<long>     対象範囲のID群
	 */
	std::set<long> getIntersectsIDs( const BGBox& box, const BGRTree& rTree );

	/**
	 * @brief   チェックのメイン関数
	 * @param   ipMeshGeo     [in]  該当メッシュポリゴン
	 * @param   blNameMap     [in]  ビル名称ポイントのオブジェクトマップ
	 * @param   buildingMap   [in]  建物のオブジェクトマップ
	 * @param   cAnnoMap      [in]  都市注記のオブジェクトマップ
	 * @param   blNameRTree   [in]  ビル名称ポイントのRTree
	 * @param   buildingRTree [in]  建物のRTree
	 * @param   cAnnoRTree    [in]  都市注記のRTree
	 */
	void CheckMain( const IGeometryPtr& ipMeshGeo, const std::map<long, GeoClass>& blNameMap, const std::map<long, GeoClass>& buildingMap, const std::map<long, GeoClass>& cAnnoMap, const BGRTree& blNameRTree, const BGRTree& buildingRTree, const BGRTree& cAnnoRTree );

	/**
	 * @brief   ビル名称ポイントの建物IDと位置の整合チェック
	 * @param   blNameOid     [in]  ビル名称ポイントのOid
	 * @param   blNameObj     [in]  ビル名称ポイントのオブジェクトコンテナ
	 * @param   buildID       [in]  ビル名称ポイントの建物ID
	 * @param   buildingMap   [in]  建物のオブジェクトマップ
	 * @param   buildingRTree [in]  建物のRTree
	 * @retval  true  エラーなし
	 * @retval  false エラーあり
	 */
	bool CheckBuildIdPos( long blNameOid, const GeoClass& blNameObj, long buildID, const std::map<long, GeoClass>& buildingMap, const BGRTree& buildingRTree );

	/**
	 * @brief   ビル名称ポイントの名称チェック
	 * @param   blNameOid     [in]  ビル名称ポイントのOid
	 * @param   blNameObj     [in]  ビル名称ポイントのオブジェクトコンテナ
	 */
	void CheckBlNameStr( long blNameOid, const GeoClass& blNameObj );

	/**
	* @brief   建物種別コード別OK/NGワードによる名称チェック
	* @param   blNameObj     [in]  ビル名称ポイントのオブジェクトコンテナ
	* @param   blName        [in]  ビル名称ポイントの名称
	* @param   judgeStr      [out] 判定に用いた文字列格納用
	* @retval  err_def::ECode エラー種別
	*/
	err_def::ECode CheckBlNameByBldcWord( const GeoClass& blNameObj, const std::string& blName, CString& judgeStr );

	/**
	* @brief  建物種別コード別NGワードを名称に持つかどうか
	* @param   blName        [in]  ビル名称ポイントの名称
	* @param   bldClasSet    [in]  建物種別コード1~3格納コンテナ
	* @param   judgeStr      [out] 判定に用いた文字列
	* @retval  true  建物種別コード別NGワードを名称に持つ
	* @retval  false 建物種別コード別NGワードを名称に持たない
	*/
	bool HasBldcNGWord( const std::string& blName, const std::set<long>& bldClassSet, CString& judgeStr );

	/**
	* @brief   建物種別コード別OKワードによる名称チェック
	* @param   blName        [in]  ビル名称ポイントの名称
	* @param   bldClasSet    [in]  建物種別コード1~3格納コンテナ
	* @param   judgeStr      [out] 判定に用いた文字列
	* @retval  err_def::ECode エラー種別
	*/
	err_def::ECode CheckBldNameByBldcOKWord( const std::string& blName, const std::set<long>& bldClassSet, CString& judgeStr );

	/**
	* @brief   建物種別コードチェック
	           チェック対象の建物種別コード群に、OKとする建物種別コードが含まれているかどうか
	* @param   chkBldcSet   [in]  チェック対象の建物種別コード群
	* @param   okBldcVec    [in]  OKとする建物種別コード群
	* @retval  err_def::ECode エラー種別
	*/
	err_def::ECode CheckOKBldc( const std::set<long>& chkBldcSet, const std::vector<long>& okBldcVec );

	/**
	 * @brief   ビル名称ポイント付加価値情報チェック
	 * @param   blNameOid     [in]  ビル名称ポイントのOid
	 * @param   blNameObj     [in]  ビル名称ポイントのオブジェクトコンテナ
	 */
	void CheckBlNameAddInfo( long blNameOid, const GeoClass& blNameObj );

	/**
	 * @brief   ビル名称ポイントの所属家形の属性チェック
	 * @param   blNameOid     [in]  ビル名称ポイントのOid
	 * @param   blNameObj     [in]  ビル名称ポイントのオブジェクトコンテナ
	 * @param   buildID       [in]  ビル名称ポイントの建物ID
	 * @param   buildObj      [in]  建物のオブジェクトコンテナ
	 */
	void CheckBuildClass( long blNameOid, const GeoClass& blNameObj, long buildID, const GeoClass& buildObj );

	/**
	 * @brief   同一建物内のビル名称ポイント個数チェック
	 * @param   blNameOid     [in]  ビル名称ポイントのOid
	 * @param   blNameObj     [in]  ビル名称ポイントのオブジェクトコンテナ
	 * @param   buildObj      [in]  建物のオブジェクトコンテナ
	 * @param   blNameMap     [in]  ビル名称ポイントのオブジェクトマップ
	 * @param   blNameRTree   [in]  ビル名称ポイントのRTree
	 */
	void CheckDupBlName( long blNameOid, const GeoClass& blNameObj, const GeoClass& buildObj, const std::map<long, GeoClass>& blNameMap, const BGRTree& blNameRTree );

	/**
	 * @brief   バッファ範囲のビル名称ポイント名称チェック
	 * @param   blNameOid     [in]  ビル名称ポイントのOid
	 * @param   blNameObj     [in]  ビル名称ポイントのオブジェクトコンテナ
	 * @param   ipBufferGeom  [in]  起点のビル名称ポイントから指定距離バッファを広げたジオメトリ
	 * @param   blNameMap     [in]  ビル名称ポイントのオブジェクトマップ
	 * @param   buildingMap   [in]  建物のオブジェクトマップ
	 * @param   blNameRTree   [in]  ビル名称ポイントのRTree
	 * @param   buildingRTree [in]  建物のRTree
	 */
	void CheckBufferRangeBlName( long blNameOid, const GeoClass& blNameObj, const IGeometryPtr& ipBufferGeom, const std::map<long, GeoClass>& blNameMap, const std::map<long, GeoClass>& buildingMap, const BGRTree& blNameRTree, const BGRTree& buildingRTree );

	/**
	 * @brief   名称比較チェック
	 * @param   blNameOid     [in]  ビル名称ポイントのOid
	 * @param   blName        [in]  ビル名称ポイントの名称
	 * @param   blNameObj     [in]  ビル名称ポイントのオブジェクトコンテナ
	 * @param   buildObj      [in]  建物のオブジェクトコンテナ
	 * @param   blNameMap     [in]  ビル名称ポイントのオブジェクトマップ
	 * @param   blNameRTree   [in]  ビル名称ポイントのRTree
	 */
	void CheckBufferRangeName( long blNameOid, const CString& blName, const GeoClass& blNameObj, const GeoClass& buildObj, const std::map<long, GeoClass>& blNameMap, const BGRTree& blNameRTree );

	/**
	 * @brief   名称比較チェック
	 * @param   blNameOid     [in]  ビル名称ポイントのOid
	 * @param   buildID       [in]  ビル名称ポイントの建物ID
	 * @param   blNameObj     [in]  ビル名称ポイントのオブジェクトコンテナ
	 * @param   buildObj      [in]  建物のオブジェクトコンテナ
	 * @param   ipBufferGeom  [in]  起点のビル名称ポイントから指定距離バッファを広げたジオメトリ
	 * @param   cAnnoMap      [in]  都市注記のオブジェクトマップ
	 * @param   buildingMap   [in]  建物のオブジェクトマップ
	 * @param   cAnnoRTree    [in]  都市注記のRTree
	 * @param   buildingRTree [in]  建物のRTree
	 */
	void CheckBufferRangeAnno( long blNameOid, long buildID, const GeoClass& blNameObj, const GeoClass& buildObj, const IGeometryPtr& ipBufferGeom, const std::map<long, GeoClass>& cAnnoMap, const std::map<long, GeoClass>& buildingMap, const BGRTree& cAnnoRTree, const BGRTree& buildingRTree );

	/**
	 * @brief   建物ビル名と注記名称が一致するか
	 * @note    どちらか包含の場合も一致とみなす
	 * @param   blName        [in]  ビル名称ポイントの名称
	 * @param   annoObj       [in]  都市注記のオブジェクトコンテナ
	 * @param   bSame         [out] 完全一致したか
	 * @retval  true  一致
	 * @retval  false 不一致
	 */
	bool CompAnnoStr( const CString& blName, const GeoClass& annoObj, bool& bSame );

	/**
	 * @brief   自身の建物内の注記個数と完全一致かどうかから、エラー種別を取得
	 * @param   sameBuildAnnoCnt [in]  自身の建物内の注記個数
	 * @param   bSameName        [in]  完全一致かどうか
	 * @retval  err_def::ECode エラー種別
	 */
	err_def::ECode GetExistAnnoErrDef( long sameBuildAnnoCnt, bool bSameName ){
		using namespace err_def;
		if( sameBuildAnnoCnt == 0 )
			return bSameName ? wSameAnnoOwnNo : wLikeAnnoOwnNo;
		else if( sameBuildAnnoCnt == 1 )
			return bSameName ? wSameAnnoOther : wLikeAnnoOther;
		else
			return bSameName ? wSameAnnoOthers : wLikeAnnoOthers;
	};

	/**
	 * @brief   自身の建物内の注記個数から、エラー種別を取得
	 * @param   sameBuildAnnoCnt [in]  自身の建物内の注記個数
	 * @retval  err_def::ECode エラー種別
	 */
	err_def::ECode GetNoAnnoErrDef( long sameBuildAnnoCnt ){
		using namespace err_def;
		if( sameBuildAnnoCnt == 0 )
			return Ok; // OK：自身の家形に注記なし＆周辺に一致無し
		else if( sameBuildAnnoCnt == 1 )
			return wNotLikeAnnoOne;
		else
			return wNotLikeAnnoDup;
	};

	/**
	 * @brief   指定範囲分のバッファを取得する
	 * @note    バッファの範囲はm_bufferRangeの値(単位:m)
	 * @param   ipGeometry [in] 対象ジオメトリ
	 * @retval  IGeometryPtr バッファ形状
	 */
	IGeometryPtr getBufferGeometry( const IGeometryPtr& ipGeometry );

	/**
	 * @brief   中抜きポリゴンを埋める(Exteriorのみ取得)
	 * @param   ipGeom [in]
	 * @retval  IGeometryPtr 埋めた形状
	 */
	IGeometryPtr getExterior( const IGeometryPtr& ipGeom );

	/**
	 * @brief   二つの文字列が一致、もしくはどちらか包含か？
	 * @note    どちらか包含の場合も一致とみなす
	 * @param   str1  [in]  比較文字列
	 * @param   str2  [in]  比較文字列
	 * @param   bSame [out] 一致かどうか
	 * @retval  true  一致
	 * @retval  false 不一致
	 */
	bool IsLikeName( const CString& str1, const CString& str2, bool& bSame );

	/**
	 * @brief   置換リストに存在する文字を置換して返却
	 * @param   cStr [in]  文字列
	 * @retval  string 置換後の文字列
	 */
	std::string ReplaceStr( const CString& cStr );

	/**
	* @brief   除外リストの記号を削除した状態の文字列リストを返却
	* @param   cStr [in]  文字列リスト
	* @retval  CString 削除後の文字列リスト
	*/
	std::vector<CString> ExcludeListedMark(const std::vector<CString>& cStr);

private:

	CString          m_strBlNameDB;          //!< ビル名称DB接続先
	CString          m_strBuildingDB;        //!< 都市建物DB接続先
	CString          m_strAnnoDB;            //!< 注記DB接続先
	CString          m_strMeshDB;            //!< メッシュDB接続先
	CString          m_strOutput;            //!< アウトプット
	CString          m_strMeshList;          //!< メッシュリスト
	CString          m_strNgBuild;           //!< NG建物種別リスト
	CString          m_strErrWord;           //!< NG文字列リスト(エラー)
	CString          m_strWarnWord;          //!< NG文字列リスト(警告)
	CString          m_strBldcOKWord;        //!< 建物種別コード別OKワードリスト
	CString          m_strBldcNGWord;        //!< 建物種別コード別NGワードリスト
	CString          m_strReplaceWord;       //!< 置換文字列リスト
	CString          m_strJoinAnno;          //!< 文字列比較用置換リスト
	CString          m_strExclude_mark;      //!< 分割数チェック時除外記号リスト
	double           m_bufferRange;          //!< 探索バッファ(m)

	CString          m_strBlNameWorkspace;   //!< ビル名称ワークスペース名
	CString          m_strBuildingWorkspace; //!< 建物ワークスペース名
	CString          m_strAnnoWorkspace;     //!< 注記ワークスペース名
	CString          m_strMeshWorkspace;     //!< メッシュワークスペース名

	IFeatureClassPtr m_ipBlNameFC;           //!< ビル名称フィーチャクラス
	IFeatureClassPtr m_ipBuildingFC;         //!< 都市建物フィーチャクラス
	IFeatureClassPtr m_ipCityAnnoFC;         //!< 都市注記テーブル
	IFeatureClassPtr m_ipCityMeshFC;         //!< 都市メッシュフィーチャクラス

	CFieldMap        m_blNameFM;             //!< ビル名称フィールドマップ
	CFieldMap        m_buildingFM;           //!< 都市建物フィールドマップ
	CFieldMap        m_cityAnnoFM;           //!< 都市注記フィールドマップ
	CFieldMap        m_cityMeshFM;           //!< 都市メッシュフィールドマップ

	COutPut          m_log;                  //!< ログ管理クラス
	cNameChk         m_cNameChk;             //!< 名称チェッククラス
	
	std::set<long>   m_meshList;             //!< メッシュリスト
	std::set<long>   m_ngBuildClass;         //!< NG建物種別コードリスト

	std::vector<std::string> m_errWordVec;     //!< NGワードリスト（エラー）
	std::vector<std::string> m_warnWordVec;    //!< NGワードリスト（警告）
	std::set<std::string> m_ExcludeMarkVec; //!< 割数チェック時除外記号リスト

	util::strKey_map         m_bldcOKWordMap;  //!< 建物種別コード別OKワードリスト
	util::vec_map            m_bldcNGWordMap;  //!< 建物種別コード別NGワードリスト
	bool                     m_chkFixclass;    //!< 建物種別コード別ワードチェックを種別確定コードが未確認（0）のデータに対してのみ行うかどうか

	util::pair_vec           m_replaceWordVec; //!< 置換文字リスト
	util::vec_map            m_joinAnnoMap;    //!< 注記比較用接続文字リスト

	IProjectedCoordinateSystemPtr m_ipPrjCoordinateSystem; //!< 19座標系の投影座標系(メッシュごとに置き換える)
};

