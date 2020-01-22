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
#include "..\layercheckbase.h"
#include "..\TextAddressMaster.h"

/**
* @brief	市区町村コードのポリゴン
*/
struct CityPolygonValue 
{
	long oID;            //!< オブジェクトID
	IGeometryPtr geo;    //!< ジオメトリ
	/**
	 * @brief  コンストラクタ
	 */
	CityPolygonValue():oID(INVALID_VALUE){}

	/**
	 * @brief  コンストラクタ
	 * @param	[in] objectID  //!< オブジェクトID
	 * @param	[in] cityGeo   //!< ジオメトリ
	 */
	CityPolygonValue(long objectID, const IGeometryPtr& cityGeo):oID(objectID), geo(cityGeo){}
};
/**

 * @class	CityPolygonCheck
 * @brief	住所系チェック
 */
class CityPolygonCheck :	public LayerCheckBase
{
public:
	/**
	 * @brief  コンストラクタ
	 */
	CityPolygonCheck(void){}

	 /**
	 * @brief  デストラクタ
	 */
	virtual ~CityPolygonCheck(void){}

	/**
	 * @brief	レイヤチェック名取得
	 * @return	レイヤチェック名
	 */
	LPCTSTR getName(){ return city_polygon::kTableName; }

	/**
	 * @brief	初期化
	 * @param	[in] settingFile		設定ファイル
	 * @param	[in] tableFinder		テーブル管理
	 * @param	[in] errorLog           エラーファイル管理
	 * @param	[in] textAddressMaster  住所マスタデータパース
	 * @retval	true:成功
	 * @retval	false:失敗
	 */
	bool initialize( const SettingFile& settingFile, TableFinder& tableFinder, ErrLogger& errorLog, const CString& textAddressMaster );

	/**
	 * @brief	チェック
	 * @param	[in] ulMeshCode		    メッシュコード
	 * @param	[in] ipMeshGeometry	    メッシュジオメトリ
	 * @param	[in] ulMeshID	    　　メッシュオブジェクトID
	 */
	void check( unsigned long ulMeshCode, const IGeometryPtr& ipMeshGeometry, long meshID );

	/**
	 * @brief	同じ市区町村コードのチェック
	 * @param	[in] oID		        オブジェクトID
	 * @param	[in] geo        	    ジオメトリ
	 * @param	[in] cityCodeStr        市区町村コード
	 * @param	[out]sameCityPolygonMap	市区町村コードとジオメトリのコンテナ
	 */
	void checkSameCityPolygon( long oID, const IGeometryPtr& geo, const CString& cityCodeStr, map<CString, map<long, IGeometryPtr>>& sameCityPolygonMap );

	/**
	 * @brief	マスターとチェック
	 * @param	[in] oID		        オブジェクトID
	 * @param	[in] cityCodeStr        市区町村コード
	 */
	void checkWithMasterInfo( long oID, const CString& cityCodeStr );

private:
	IFeatureClassPtr   m_ipCityPolygon;     //!< 都市ポリゴンフィーチャクラス
	FIELDSMAP          m_cityPolygonFieldsMap;  //!< 都市ポリゴンフィーチャクラスのフィールドマップ

	TextAddressMaster  m_textAddressMaster; //!< テキスト住所マスタ

	RelationCheck      m_relationCheck;     //!< 共通の関係のチエック
};

