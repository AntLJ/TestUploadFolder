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

#include "ErrorObject.h"	
#include "..\TextAddressMaster.h"

/**
* @brief	フィールド
*/
struct Fields
{
	CString addrCode1;              //!< 住所コード1
	IGeometryPtr ipGeometry;        //!< ジオメトリ
};


/**
* @brief	フィールドグループキー
*/
struct FieldGroupKey
{
	CString addrCode1Str;              //!< 住所コード1
	CString addrCode2Str;              //!< 住所コード2
	CString cityCodeStr;               //!< 市区町村コード	
	long gaikufugoCode;                //!< 街区符号

	FieldGroupKey():gaikufugoCode(INVALID_VALUE){};
	FieldGroupKey(const CString& addrCode1, const CString& addrCode2, const CString& cityCode, long gaikufugo ):addrCode1Str(addrCode1),addrCode2Str(addrCode2),cityCodeStr(cityCode),gaikufugoCode(gaikufugo)
	{};

	/**
	* @brief	演算子<のオーバロード
	* @param	[in] fieldGroupKey	     フィールドグループキー
	* @retval	true:小さい
	* @retval	false:よりまたは等しい
	*/
	bool operator <(const FieldGroupKey& fieldGroupKey) const
	{
		if( gaikufugoCode < fieldGroupKey.gaikufugoCode )	return true;
		if( gaikufugoCode > fieldGroupKey.gaikufugoCode )	return false;
		if( addrCode1Str.Compare( fieldGroupKey.addrCode1Str ) < 0) return true;
		if( addrCode1Str.Compare( fieldGroupKey.addrCode1Str ) > 0) return false;
		if( addrCode2Str.Compare( fieldGroupKey.addrCode2Str ) < 0) return true;
		if( addrCode2Str.Compare( fieldGroupKey.addrCode2Str ) > 0) return false;
		if( cityCodeStr.Compare( fieldGroupKey.cityCodeStr ) < 0) return true;
		return false;
	}

	/**
	* @brief	演算子=のオーバロード
	* @param	[in] fieldGroupKey	     フィールドグループキー
	* @return	フィールドグループキー
	*/
	FieldGroupKey &operator =(const FieldGroupKey& fieldGroupKey)
	{
		addrCode1Str = fieldGroupKey.addrCode1Str;
		addrCode2Str = fieldGroupKey.addrCode2Str;
		cityCodeStr = fieldGroupKey.cityCodeStr;
		gaikufugoCode = fieldGroupKey.gaikufugoCode;
	};
};

/**
* @brief	フィールドグループ値
*/
struct FieldGroupValue
{
	IGeometryCollectionPtr ipGeometryCollection;   //!< 市区町村コード、住所コード、住所コード2、街区符号の組み合わせが等しい行政界ポリゴンをマージした結果
		
	std::map<CString, CString> addrTypeMap;        //!< 住所整備種別,オブジェクトID
	std::map<long, IGeometryPtr> geoMap;           //!< 形マップ
};

/**
* @brief	フィールドグループキー
*/
struct FieldGroupKey2
{
	long addrTypeCode;               //!< 住所整備種別コード
	CString extName1;                //!< 拡張名称（公称）
	CString extName2;                //!< 拡張名称（通称）
	FieldGroupKey fieldKey;          //!< 他フィールド
	/**
	 * @brief  コンストラクタ
	 * @param	[in] addrType    //!< 住所整備種別コード
	 * @param	[in] extName1Str //!< 拡張名称（公称）
	 * @param	[in] extName2Str //!< 拡張名称（通称）
	 * @param	[in] fieldGroup  //!< 他フィールド
	 */
	FieldGroupKey2(long addrType, const CString & extName1Str, const CString & extName2Str, const FieldGroupKey& fieldGroup):addrTypeCode(addrType),extName1(extName1Str),extName2(extName2Str),fieldKey(fieldGroup)
	{}

	/**
	* @brief	演算子<のオーバロード
	* @param	[in] fieldGroupKey	     フィールドグループキ
	* @retval	true:小さい
	* @retval	false:よりまたは等しい
	*/
	bool operator < (const FieldGroupKey2& fieldGroupKey)const
	{
		if(addrTypeCode < fieldGroupKey.addrTypeCode)return true;
		if(addrTypeCode > fieldGroupKey.addrTypeCode)return false;
		if(extName1.Compare( fieldGroupKey.extName1 ) < 0) return true;
		if(extName1.Compare( fieldGroupKey.extName1 ) > 0) return false;
		if(extName2.Compare( fieldGroupKey.extName2 ) < 0) return true;
		if(extName2.Compare( fieldGroupKey.extName2 ) > 0) return false;
		return fieldKey < fieldGroupKey.fieldKey;
	}
};



/**
 * @class	AddrPolygonCheck
 * @brief	住所系チェック
 */
class AddrPolygonCheck : public LayerCheckBase
{
public:
	/**
	 * @brief  コンストラクタ
	 */
	AddrPolygonCheck(void){}
	
	/**
	 * @brief  デストラクタ
	 */
	virtual ~AddrPolygonCheck(void){}

private:
	/**
	 * @brief	レイヤチェック名取得
	 * @return	レイヤチェック名
	 */
	LPCTSTR getName(){ return addr_polygon::kTableName; }

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
	* @brief	重なるチェック
	* @param	[in] ipGeometry	     ジオメトリ
	* @param	[in] ipFeature	     フィーチャ
	* @param	[in] query	         検索条件
	* @param	[in] info	         情報
	* @param	[in] oID             オブジェクトID
	*/
	void checkOverlap( const IGeometryPtr& ipGeometry, const IFeaturePtr& ipFeature, const CString& query, const CString& info, long oID );

	/**
	* @brief	同じ属性の形を集める
	* @param	[in]  oID             オブジェクトID
	* @param	[in]  ipAddressGeometry   ジオメトリ
	* @param	[in]  fieldGroup      フィールドグループ
	* @param	[in]  addrTypeCode　　住所整備種別コード	
	* @param	[in]  extName1Str     拡張名称（公称）
	* @param	[in]  extName2Str     拡張名称（通称）
	* @param	[in]  touchGeoMap     コンテナ
	*/
	void collectSameAttrGeometry( long oID, const IGeometryPtr& ipAddressGeometry, const FieldGroupKey& fieldGroup, long addrTypeCode, const CString& extName1Str, const CString& extName2Str, map<FieldGroupKey2, map<long,IGeometryPtr>>& touchGeoMap );

	/**
	* @brief	マスタ情報を通じてのチエック
	* @param	[in]  oID                 オブジェクトID
	* @param	[in]  ipAddressGeometry   ジオメトリ
	* @param	[in]  cityCodeStr         市区町村コード
	* @param	[in]  addrCodeStr         住所コード(公称)
	* @param	[in]  extName1Str         拡張名称（公称）
	*/
	void checkWithMasterInfo(long oID, const IGeometryPtr& ipAddressGeometry, const CString& cityCodeStr, const CString& addrCodeStr, const CString& extName1Str);

	/**
	* @brief	地名代表点のチエック
	* @param	[in]  oID                 オブジェクトID
	* @param	[in]  ipAddressGeometry   ジオメトリ
	* @param	[in]  cityCodeStr         市区町村コード
	* @param	[in]  addrCodeStr         住所コード(公称)
	*/
	void checkRepresentPoint(long oID, const IGeometryPtr& ipAddressGeometry, const CString& cityCodeStr, const CString& addrCodeStr );

	/**
	* @brief	飛び地のチエック
	* @param	[in] ipSpatialReference   空間参照
	* @param	[in] differentAddrCodeMap コンテナ
	*/
	void checkEnclave(const ISpatialReferencePtr& ipSpatialReference,  map<FieldGroupKey, FieldGroupValue>& differentAddrCodeMap);

	/**
	* @brief	フィールドが等しい住所整備種別の異なるのチエック
	* @param	[in]  oID                オブジェクトID
	* @param	[in]  ipAddressGeometry  ジオメトリ
	* @param	[in]  ipSpatialReference 空間参照
	* @param	[in]  addrTypeStr        住所整備種別コード	
	* @param	[in]  addrCodeStr        住所コード(公称)
	* @param	[in]  addrCode2Str       住所コード(通称)
	* @param	[in]  cityCodeStr        市区町村コード
	* @param	[in]  gaikufugo          街区符号
	* @param	[out] differentAddrCodeMap        コンテナ
	*/
	void checkSameAttrAddrTypeC(long oID, IGeometryPtr& ipAddressGeometry, const ISpatialReferencePtr& ipSpatialReference, const CString& addrTypeStr, const CString& addrCodeStr, const CString& addrCode2Str, const CString& cityCodeStr, long gaikufugoCode, map<FieldGroupKey, FieldGroupValue>& differentAddrCodeMap);

	/**
	* @brief	フィールドのチエック
	* @param	[in]  oID                オブジェクトID
	* @param	[in]  ipAddressGeometry  ジオメトリ
	* @param	[in]  addrType           住所整備種別コード	
	* @param	[in]  cityCode           市区町村コード
	* @param	[in]  addrCode1          住所コード(公称)
	* @param	[in]  addrCode2          住所コード(通称)
	* @param	[in]  gaikufugo          街区符号
	* @param	[in]  extName1           拡張名称（公称）
	* @param	[in]  extName2           拡張名称（通称）
	* @param	[in]  extYomi2           拡張読み（通称）
	*/
	void checkField(long oID, const IGeometryPtr& ipAddressGeometry, const CComVariant& addrType, const CComVariant& cityCode, const CComVariant& addrCode1, const CComVariant& addrCode2, const CComVariant& gaikufugo, const CComVariant& extName1, const CComVariant& extName2, const CComVariant& extYomi2);

	/**
	* @brief	禁則文字のチエック
	* @param	[in]  oID                オブジェクトID
	* @param	[in]  ipAddressGeometry  ジオメトリ
	* @param	[in]  extName1           拡張名称（公称）
	*/
	void checkProhibitChar(long oID, const IGeometryPtr& ipAddressGeometry, const CString& extName1Str);

	/**
	* @brief	フィールドが等しい行政界が隣接しているのチエック
	* @param	[in]  touchGeoMap        コンテナ
	*/
	void checkSameAttrShape(const map<FieldGroupKey2, map<long,IGeometryPtr>>& touchGeoMap);

	/**
	* @brief	空間検索
	* @param	[out] ipCursor           検索カーソル
	* @param	[in]  ipGeometry         メッシュジオメトリ	
	* @retval	true:成功
	* @retval	false:失敗
	*/
	bool select( IFeatureCursorPtr& ipCursor, const IGeometryPtr& ipGeometry );


	IFeatureClassPtr   m_ipAddressPolygon;        //!< 住所ポリゴンフィーチャクラス
	IFeatureClassPtr   m_ipCityPolygon;           //!< 都市ポリゴンフィーチャクラス
	IFeatureClassPtr   m_ipPlaseNameRepPoint;     //!< 地名代表点ポイントフィーチャクラス
	FIELDSMAP          m_addressFieldsMap;        //!< 住所ポリゴンフィーチャクラスのフィールドマップ

	TextAddressMaster  m_textAddressMaster;       //!< テキスト住所マスタ
	CString            m_prohibitStr;             //!< 禁則文字列

	RelationCheck      m_relationCheck;           //!< 共通の関係チエック
};
