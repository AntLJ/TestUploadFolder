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

#include "FieldMap.h"

using namespace std;
using namespace sindy;

enum ERRTYPE {
	INFO     = 0,
	NOTICE   = 1,
	WARNING  = 2,
	ERR      = 3,
	CRITICAL = 4
};

/**
 * @brief   データ更新情報
 */
enum UPDATETYPE {
	SAME          = 0,   //!< 変更なし
	NEW           = 1,   //!< 新規
	DEL           = 2,   //!< 削除
	SHAPE         = 3,   //!< 形状変更
	ATTR          = 4,   //!< 属性変更
	SHAPE_ATTR    = 5    //!< 形状＋属性変更
};

/**
 * @brief   更新予定データのステータス情報
 * @note    ビットフラグ
 */
enum DATASTATUS {
	NONE          = 0, //!< 更新無し
	UPDATE        = 1, //!< 更新有り
	CONFLICT      = 2, //!< コンフリクト
	REL_LINK      = 4, //!< リンクに引きずられ
	REL_NODE      = 8, //!< ノードに引きずられ
	REL_AGOPS     = 16, //!< リンク内属性などに引きずられ
	REL_INF       = 32, //!< リンク列に引きずられ
	REL_EDGENODE  = 64, //!< メッシュ境界ノードに引きずられ
	REL_ZERONODE  = 128,  //!< FromToノードIDが0のリンク引きずられ
	REL_VEHICLETYPE = 256, //!< 車種に引きずられ 
	OUTMESH       = 512,   //!< 指定メッシュ外
};

/**
 * @brief   取得DBのタイプ
 */
enum DBTYPE {
	BEFORE = 0,   //!< 編集前PGDB
	AFTER  = 1,   //!< 編集後PGDB
	SDE    = 2    //!< SDE
};

/**
 * @brief   テーブルのデータタイプ
 * @note    ビットフラグ
 */
enum DATATYPE {
	FEATURECLASS   = 1,    //!< フィーチャクラス
	TABLE          = 2,    //!< テーブル
	REL_LINKTABLE  = 4,    //!< リンク紐付きテーブル
	INF            = 8,    //!< INFテーブル
	LQ             = 16,   //!< LQテーブル
	REL_OTHERTABLE = 32    //!< その他紐付きテーブル
};


/**
 * @brief   LQデータ
 */
struct LqData
{
	long lInfID;      //!< INFID
	long lMeshCode;   //!< メッシュコード
	long lLinkID;     //!< リンク
	long lLinkDir;    //!< リンク方向
	long lSeq;        //!< シーケンス
	long lLast;       //!< ラストフラグ

	bool operator==( const LqData& cLqData ) const 
	{
		return (
			this->lInfID == cLqData.lInfID &&
			this->lMeshCode == cLqData.lMeshCode &&
			this->lLinkID== cLqData.lLinkID &&
			this->lLinkDir == cLqData.lLinkDir &&
			this->lSeq == cLqData.lSeq &&
			this->lLast == cLqData.lLast
			);
	}
	bool operator<( const LqData& cLqData ) const 
	{
		return 
			(
			this->lInfID != cLqData.lInfID ? this->lInfID < cLqData.lInfID : 
			this->lSeq != cLqData.lSeq ? this->lSeq < cLqData.lSeq : 
			this->lMeshCode != cLqData.lMeshCode ? this->lMeshCode < cLqData.lMeshCode :
			this->lLinkID != cLqData.lLinkID ? this->lLinkID < cLqData.lLinkID :
			this->lLinkDir != cLqData.lLinkDir ? this->lLinkDir < cLqData.lLinkDir : this->lLast < cLqData.lLast
			);
	}
};

struct TableType
{
	CString strTableName;
	DATATYPE dataType;

};

/**
 * @brief   テーブルデータ
 */
struct TableData 
{
	DATATYPE dataType;                     //!< テーブルのデータタイプ
	CString strTableName;                  //!< テーブル名
	ITablePtr ipTable;                     //!< テーブル
	CFieldMap cFieldMap;                   //!< フィールドマップ
	set<long> exceptFieldIndexSet;         //!< コンフリクトの除外フィールドインデックスのset(bug 10363)

	map<long,CAdapt<_IRowPtr>> mapRow;     //!< データ群
	map<long,long> mapOrgID2PGDBID;        //!< SDEのOIDとPGDBのOIDのマップ
	map<long,long> mapPGDBID2OrgID;        //!< PGDBのOIDとSDEのOIDのマップ

	map<long,bool> mapInMeshData;          //!< メッシュ内データかどうか

	// [リンク内属性、傾斜、一方通行、通行規制、簡易傾斜、一通/通禁車種別規制]用
	map<long,set<long>> mapLink2AGOPS;     //!< リンクに紐付く[リンク内属性、傾斜、一方通行、通行規制、簡易傾斜、一通/通禁車種別規制]のマップ
	map<long,long> mapAGOPS2Link;          //!< [リンク内属性、傾斜、一方通行、通行規制、簡易傾斜、一通/通禁車種別規制]に紐付くリンクのマップ
	
	// [車種タイプ用] (bug 10363)
	map<long,set<long>> mapVehicleReg2Type; //!< 一通/通禁車種別規制に紐付く車種タイプのマップ
	map<long,long> mapType2VehicleReg;      //!< 車種タイプに紐付く一通/通禁車種別規制のマップ

	// リンク列用
	map<long,set<LqData>> mapInfData;      //!< リンク列のデータ群
	map<long,set<long>> mapLink2Inf;       //!< リンクに紐付くリンク列のマップ
	map<long,set<long>> mapInf2Link;       //!< リンク列に紐付くリンクのマップ
};

// ログ出力用
struct UpdateData 
{
	long lOID;
	UPDATETYPE updateType;
	DATASTATUS dataStatus;
};

class CNWData
{
public:
	CNWData(){}
	~CNWData(){}
public:
	/**
	 * @brief   データ初期化
	 * @param   mapTableMap    [in]  テーブルマップ
	 * @param   ipFWorkspace   [in]  ワークスペース
	 * @param   lMeshCode      [in]  メッシュコード
	 * @param   ipMeshGeom     [in]  メッシュ形状
	 * @param   exceptFieldMap [in] インポート除外フィールドmap(bug 10363)
	 * @retval  true   初期化成功
	 * @retval  false  初期化失敗
	 */
	bool Init( const map<CString,DATATYPE>& mapTableMap, const IFeatureWorkspacePtr& ipFWorkspace, long lMeshCode, const IGeometryPtr& ipMeshGeom, const map<CString,set<CString>>& exceptFieldMap );
	/**
	 * @brief   SDE初期化
	 * @param   mapTableMap    [in]  テーブルマップ
	 * @param   ipFWorkspace   [in]  ワークスペース
	 * @param   exceptFieldMap [in] インポート除外フィールドmap(bug 10363)
	 * @retval  true   初期化成功
	 * @retval  false  初期化失敗
	 */
	bool Init_SDE( const map<CString,DATATYPE>& mapTableMap, const IFeatureWorkspacePtr& ipFWorkspace, const map<CString,set<CString>>& exceptFieldMap );
	
	// 空間参照取得用
	ISpatialReferencePtr GetSpatialReference() const { return m_ipSpRef; }

	// フィーチャワークスペース取得用
	IFeatureWorkspacePtr GetFWorkspace() const { return m_ipFWorkspace; }
	// ワークスペース取得
	IWorkspacePtr GetWorkspace() const { return IWorkspacePtr(m_ipFWorkspace); }
	// メッシュ形状取得
	IGeometryPtr GetMeshGeometry() const { return m_ipMeshGeom; }
	// メッシュコード取得
	long GetMeshCode() const { return m_lMeshCode; }
	// テーブル取得
	ITablePtr GetTable( const CString& strTableName ) const;
	// フィールドマップ取得
	CFieldMap GetFieldMap( const CString& strTableName ) const;
	// 指定OIDのレコード取得
	_IRowPtr GetRow( const CString& strTableName, long lOID ) const;
	// PGDBの指定OrgOIDのレコード取得
	long GetOrgOIDFromPGDBOID( const CString& strTableName, long lPGDBOID ) const;
	// PGDBの指定PGDBOIDのレコード取得
	long GetPGDBOIDFromOrgOID( const CString& strTableName, long lOrgOID ) const;

private:
	/**
	 * @brief   テーブルのデータを取得
	 * @param   strTableName   [in]  テーブルのテーブル名
	 * @param   dataType       [in]  テーブルのデータタイプ
	 * @return  true  成功
	 *          false 失敗
	 */
	bool setTable( const CString& strTableName, const DATATYPE dataType );

	/**
	 * @brief   フィールドインデックスを取得
	 * @param   strTableName   [in]  テーブル名
	 * @return  true  成功
	 *          false 失敗
	 */
	bool setFieldMap( const CString& strTableName, const map<CString,set<CString>>& exceptFieldMap );

	/**
	 * @brief   テーブルのデータを取得
	 * @param   strTableName   [in]  テーブルのテーブル名
	 * @param   ipQFilter      [in]  クエリフィルタ
	 * @param   bInMesh        [in]  指定メッシュ内のデータかどうか
	 * @return  true  成功
	 *          false 失敗
	 */
	bool setTableData( const CString& strTableName, const IQueryFilterPtr& ipQFilter, bool bInMesh );

	/**
	 * @brief   メッシュ形状に紐付くフィーチャクラスのデータを取得
	 * @param   strTableName   [in]  フィーチャクラスのテーブル名
	 * @param   ipGeom         [in]  メッシュ形状
	 * @return  true  成功
	 *          false 失敗
	 */
	bool setFeatureClassData( const CString& strTableName, const IGeometryPtr& ipGeom );
	
	/**
	 * @brief   リンクとノードの関連性をセット
	 * @return  true  成功
	 *          false 失敗
	 */
	bool setLinkNodeRelation();

	/**
	 * @brief   リンクと[リンク内属性、傾斜、一方通行、通行規制、簡易傾斜、一通/通禁車種別規制]との関係をセット
	 * @param   strTableName   [in]  [リンク内属性、傾斜、一方通行、通行規制、簡易傾斜、一通/通禁車種別規制]のテーブル名
	 * @return  true  成功
	 *          false 失敗
	 */
	bool setLinkAGOPSRelation( const CString& strTableName );

	/**
	 * @brief   一通/通禁車種別規制と車種タイプの関係をセット
	 * @param   strTableName   [in]  車種タイプのテーブル名
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool setVehicleRegTypeRelation( const CString& strTableName );

	/**
	 * @brief   リンク列をセット
	 * @param   strTableName   [in]  INFかLQのテーブル名
	 * @return  true  成功
	 *          false 失敗
	 */
	bool setInfLq( const CString& strTableName, long lMeshCode );

	/**
	 * @brief   メッシュ境界ノードをセット
	 * @return  true  成功
	 *          false 失敗
	 */
	bool setMeshNode();

public:
	long m_lMeshCode;                       //!< メッシュコード
	IGeometryPtr m_ipMeshGeom;              //!< メッシュ形状
	IFeatureWorkspacePtr m_ipFWorkspace;    //!< フィーチャワークスペース

	ISpatialReferencePtr m_ipSpRef;         //!< 空間参照

	map<CString,TableData> m_mapTableData;  //!< テーブル毎のデータ
	map<long,pair<long,long>> m_mapLink2FromToNode; //!< リンクに紐付くFromToノードのマップ
	map<long,set<long>> m_mapNode2Link;     //!< ノードに紐付くリンク群のマップ
	set<long> m_setMeshNode;                //!< メッシュ境界ノード
};
