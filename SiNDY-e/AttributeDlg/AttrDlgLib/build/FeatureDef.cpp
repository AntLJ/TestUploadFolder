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

/*! \file FeatureDef.cpp
	\brief CFeatureDef クラスのインプリメンテーション
	\author INCREMENTP CORP. R&D技術センター第４開発グループ 古川貴宏
	\date 2002-03-05
*/
#include "stdafx.h"
#include "../FeatureDef.h"

#include "ArcHelper.h"
#include "MapHelper.h"

using namespace sindy::schema;

namespace
{
	/**
	 * @brief Z値付きのポイントかどうかを判定する
	 * @param [in] ipFeatureClass フィーチャクラス
	 * @retval true フィーチャクラスはZ値付きのポイント
	 * @retval false フィーチャクラスはZ値付きのポイントではない
	 */
	bool IsPointWithZ(IFeatureClassPtr ipFeatureClass)
	{
		// フィーチャクラスが有効かつZ値持ちでなければfalse返す
		if(!(ipFeatureClass && AheHasZ(ipFeatureClass)))
			return false;

		// フィーチャクラスのジオメトリタイプを取得する。POINT型かどうかを判断基準とする
		esriGeometryType type = esriGeometryNull;
		ipFeatureClass->get_ShapeType(&type);
		
		return type == esriGeometryPoint;
	}
}

/**
 * コンストラクタ
 */
CFeatureDef::CFeatureDef()
{
	Clear();
}

/**
 * デストラクタ
 */
CFeatureDef::~CFeatureDef()
{
	Clear();
}
/**
 * メンバ変数を全て初期状態に戻す
 */
void CFeatureDef::Clear()
{
	CRowDef::Clear();
	m_ipFeature = NULL;
	m_bError = FALSE;
	m_listRelationalTable.clear();
	m_bWarnDelete = FALSE;
	m_sindyeEditableType = sindyeEditableNoDefine;
}
/**
 * メンバ変数をフィールド値の変更がかかる前に戻す
 */
void CFeatureDef::Reset()
{
	CRowDef::Reset();
	/// 外部テーブルもリセット
	std::list<CRelationalTableDef>::iterator it = m_listRelationalTable.begin();
	while( it != m_listRelationalTable.end() ) {
		it->Reset();

		/// リセットの結果外部テーブルのレコード数が0になったら外部テーブル自体も削除
		if( it->size() == 0 )
			it = m_listRelationalTable.erase(it);
		else
			it++;
	}
}
/**
 * フィールド値の変更をデータベースに書き込み、履歴情報を更新する
 *
 * @param bDoOperation		[in]	StartEditOperation を自分でするか、呼び出し側のツールでするかどうか
 *
 */
void CFeatureDef::Store( BOOL bDoOperation )
{
	// フィーチャのフィールドの保存
	BOOL bStored = CRowDef::Store( bDoOperation );

	// 外部テーブルへの変更を適用
	// TODO: ここの処理はほんとうに必要かどうか含め要検討
	//       おそらく相手にしたいのは規制のテーブルだけだと思われるが、m_listRelationalTable には
	//       ArcMap上でテーブル結合した際のテーブルも格納されている。なので、ラベリングのために他のテーブルを
	//       結合してたりするとそれらも相手にしてしまうことになり、非常に危うい気がする。
	//       ちなみに、この中でのStore()は、変更あった場合のみのput_Value()であり、実際には保存はされない。
	//       また、通常、外部テーブルは変更されていることはないはずなので、put_Value()もされていないはず。
	for( std::list<CRelationalTableDef>::iterator it = m_listRelationalTable.begin(); it != m_listRelationalTable.end(); it++ ) 
	{
		it->Store();

		// レコードが0個の時にリレーションフラグがつきっぱなしならフラグをOFFにする
		if( it->IsNoRows() )
		{
			CString strRFName;
			// テーブル名からリレーションフラグ名を取得
			if( it->GetTableName().CompareNoCase(TIME_ONEWAY_TABLE_NAME) == 0 )
				strRFName = _T("ONEWAY_RF");
			else if( it->GetTableName().CompareNoCase(TIME_NOPASSAGE_TABLE_NAME) == 0 )
				strRFName = _T("NOPASSAGE_RF");
			else if( it->GetTableName().CompareNoCase(DUPLILINK_TABLE_NAME) == 0 )
				strRFName = _T("DUPLINK_RF");

			// ↑のリレーションしか相手にしない
			// 他のテーブルが結合されている場合があるが、その時は無視（bug 10209）
			if( strRFName.IsEmpty() )
				continue;

			// 取得したリレーションフラグをチェック
			CFieldDef* pFieldDef = GetFieldDef( (LPCTSTR)m_strTableName, (LPCTSTR)strRFName );
			ATLASSERT( pFieldDef != NULL ); // 絶対あるはず

			if( pFieldDef->m_vaValue.lVal > 0 )
			{
				pFieldDef->m_vaValue.lVal = 0;
				// まだ Store() されていなければそのまま、でなければここで Store()
				if( !pFieldDef->m_bChanged )
				{
					pFieldDef->m_bChanged = TRUE;
					pFieldDef->Store();
				}

//{ ADD 02.10.28 by hkakeno for TEST
				//	時間規制レコードがなくなった場合、一通種別を「一通なし」にする
				if( it->GetTableName().CompareNoCase(TIME_ONEWAY_TABLE_NAME) == 0 )
				{
					CFieldDef* pFieldDef2 = GetFieldDef( (LPCTSTR)m_strTableName, (LPCTSTR)_T("ONEWAY_C") );
					pFieldDef2->m_vaValue.lVal = 0;
					if( !pFieldDef2->m_bChanged )
					{
						pFieldDef2->m_bChanged = TRUE;
						pFieldDef2->Store();
					}
				}
//} ADD 02.10.28 by hkakeno for TEST

				// フィーチャの属性が変更されたので
				bStored = TRUE;
			}
		}
	}

	// 履歴を書き込む
	if( bStored )
	{
		// StartEditOperation をしない場合には編集履歴は呼び出し元がつけ、Store() も呼び出し元がする
		if( bDoOperation )
		{
			if( !AheSetModifyData( m_ipFeature, sindyUpdateTypeProperty ) )
				ATLASSERT( FALSE );	// iPC Feature 以外を編集するとこうなる？
			
			m_ipFeature->Store();

			BOOL		bTest;
			IFieldsPtr	ipFields;
			m_ipFeature->get_Fields(&ipFields);
			CComVariant	vaField;
			long		lTemp;
			ipFields->FindField( CComBSTR(_T("ONEWAY_RF")), &lTemp );
			m_ipFeature->get_Value( lTemp, &vaField );
			bTest = vaField.bVal;
			ipFields->FindField( CComBSTR(_T("ONEWAY_C")), &lTemp );
			m_ipFeature->get_Value( lTemp, &vaField );
			bTest = vaField.lVal;
		}
	}
}
/**
 * CArcHelper クラスをセットする
 *
 * @param pHelper	[in]	CArcHelper クラスのポインタ
 */
void CFeatureDef::SetArcHelper( IApplication* ipApp )
{
	ATLASSERT( ipApp != NULL );

	m_cArcHelper.Init( (IApplicationPtr)ipApp );
}
/**
 * フィーチャをセットする
 *
 * @param ipFeature	[in]	元となる _IRow インターフェースポインタ
 *
 * 全てのフィールドを分解し、その際 *_RF というフィールドが存在したら外部テーブルとみなして
 * 外部テーブルも同様に定義します。
 *
 * \note 現在対応している *_RF フィールドは TIMEREG_RF、DRMA_RF、DUPLINK_RF のみです。
 * \note 上記以外の *_RF フィールドを対応させるにはコード中に埋め込む必要があります。
 */
void CFeatureDef::SetFeature( _IRowPtr ipFeature )
{
	IMapPtr ipMap( m_cArcHelper.GetMap() );
	IFieldsPtr ipFields;
	ITablePtr ipTable;

	CString strTableName, strFieldName;
	LONG lFieldCount = 0;

	if( !ipFeature )
		return;	//!< フィーチャではない

	USES_CONVERSION;

	/// テーブル名を取得
	strTableName = AheGetFeatureClassName( ipFeature );

	ipFeature->get_Fields( &ipFields );
	if( !ipFields )
		return;
	ipFields->get_FieldCount( &lFieldCount );

	/// フィールドに分解して登録
	for( LONG i = 0; i < lFieldCount; i++ ) {
		CComBSTR bstrFieldName;
		IFieldPtr ipField;
		ipFields->get_Field( i, &ipField );
		ipField->get_Name( &bstrFieldName );
		strFieldName = OLE2T( bstrFieldName );

		/// フィールドの登録
		CFieldDef fieldDef;
		fieldDef.SetArcHelper( m_cArcHelper.GetApp() );
		fieldDef.SetFieldDef( strTableName, strFieldName );
		fieldDef.SetRowBuffer( i, ipField, ipFeature );
		push_back( fieldDef );

		// 外部テーブル、フィールド名取得
		CString strOuterTableName;
		CString strOuterFieldName;
		if( ! getOuterTableAndFieldName(strTableName, strFieldName, strOuterTableName, strOuterFieldName) )
			continue;

		if( strOuterTableName.Compare(_T("")) == 0 )
		{
			ATLASSERT( strOuterTableName.Compare(_T("")) != 0 );
			ATLTRACE(_T("CFeatureDef::SetFeature() : リレーションフラグが存在していますが、テーブル名が分かりませんでした %s\n"), strFieldName );
			continue;
		} 
			
		/// リレーションフラグの値が0ならテーブルの登録はしない
		CComVariant vaValue;
		ipFeature->get_Value( i, &vaValue );
		if( vaValue.vt == VT_NULL || vaValue == _variant_t(0L) )
			continue;

		// 関連テーブルをセット
		ipTable = AheOpenSameOwnerTable( (ITablePtr)AheGetFeatureClass( ipFeature ), strOuterTableName );
		CRelationalTableDef relTableDef;
		relTableDef.SetArcHelper( m_cArcHelper.GetApp() );

		LONG lOID = -1;
		if( strOuterFieldName == _T("LINK_ID") )
			ipFeature->get_OID( &lOID );
		else
			lOID = vaValue.lVal;

		relTableDef.SetTable( ipTable, strOuterTableName, lOID, strOuterFieldName );
		m_listRelationalTable.push_back( relTableDef );

	}
	// Z値付きのポイントであればZ値の仮想フィールド追加(2017/2/17 ta_suga)
	if(IsPointWithZ(AheGetFeatureClass(ipFeature)))
	{
		/// フィールドの登録
		CFieldDef fieldDef;
		fieldDef.SetArcHelper( m_cArcHelper.GetApp() );
		fieldDef.SetFieldDef( strTableName, virtual_field::kShapeZ ); // フィールド名は「SHAPE.Z」
		fieldDef.m_strFieldAliasName = virtual_field::kShapeZ;        // エイリアス名も「SHAPE.Z」
		fieldDef.SetRowBuffer( -1, nullptr, ipFeature );              // フィールドインデックス、フィールドなしで登録
		push_back( fieldDef );
	}

	// リレーションが張られているならリレーション先も取得（テーブル結合限定）
	CMapHelper cMapHelper( m_cArcHelper );
	CComBSTR bstrLayerName;
	ILayerPtr ipLayer;

	cMapHelper.GetLayerTableName( ipFeature, &bstrLayerName );
	cMapHelper.GetLayerByTableName( OLE2T( bstrLayerName ), &ipLayer );
	IDisplayRelationshipClassPtr ipDispRelClass( ipLayer );

	IRelationshipClassPtr ipRelClass;

	if( ipDispRelClass != NULL ) ipDispRelClass->get_RelationshipClass( &ipRelClass );
	if( ipRelClass != NULL )
	{
		esriRelCardinality relcardinal;

		ipRelClass->get_Cardinality( &relcardinal );
		if( relcardinal == esriRelCardinalityOneToMany )
		{
			CComBSTR bstrForeignKey, bstrPrimaryKey, bstrTableName;
			IObjectClassPtr ipObjectClass;

			ipRelClass->get_OriginForeignKey( &bstrForeignKey );
			ipRelClass->get_OriginPrimaryKey( &bstrPrimaryKey );

			ipRelClass->get_OriginClass( &ipObjectClass );
			ITablePtr ipTable( ipObjectClass );
			IDatasetPtr ipDataset( ipTable );

			ipDataset->get_Name( &bstrTableName );
			CString strTableName( OLE2T( bstrTableName ) );
			strTableName.MakeUpper();
			strTableName = 	strTableName.ReverseFind('.') < 0 ? strTableName : strTableName.Right( strTableName.GetLength() - strTableName.ReverseFind('.') - 1 );

			// テーブル結合していて、かつ、フィールドに分解して結合しようとしてしまっている場合（この関数の最初のほうでやっている処理）
			// には、ニ重に登録されてしまう恐れがあるので、同じテーブルを登録しないようにここで調整
			bool bFind = false;
			for( std::list<CRelationalTableDef>::iterator it = m_listRelationalTable.begin(); it != m_listRelationalTable.end(); it++ ) 
			{
				CString strTest = it->GetTableName();
				if( strTest == strTableName ) 
					bFind = true;
			}
			if( !bFind )
			{
				LONG lForeignKey = 0;
				CComVariant vaValue;

				ipFeature->get_Fields( &ipFields );
				ipFields->FindField( bstrForeignKey, &lForeignKey );
				ipFeature->get_Value( lForeignKey, &vaValue );

				ATLASSERT( vaValue.vt == VT_I4 );	// OBJECTID なので必ず LONG のはず

				CRelationalTableDef relTableDef;

				relTableDef.SetTable( ipTable, strTableName, vaValue.lVal, OLE2T( bstrPrimaryKey ) );
				m_listRelationalTable.push_back( relTableDef );
			}
		}
	}

	m_ipFeature = ipFeature;
	m_strTableName = strTableName;
}
/**
 * セットされているフィーチャを取得する
 *
 * @return m_ipFeature を返す
 */
_IRowPtr CFeatureDef::GetFeature()
{
	return m_ipFeature;
}
/**
 * 関連テーブル定義のリストを取得する
 *
 * @return m_listRelationalTable のポインタを返す
 */
std::list<CRelationalTableDef>* CFeatureDef::GetRelationalTableDefList()
{
	return &m_listRelationalTable;
}
CRelationalTableDef* CFeatureDef::GetRelationalTableDef( LPCTSTR lpcszTableName )
{
	std::list<CRelationalTableDef>::iterator it;
	for( it = m_listRelationalTable.begin(); it != m_listRelationalTable.end(); it++ )
	{
		CString strTest = it->GetTableName();
		if( lstrcmpi( (LPCTSTR)it->GetTableName(), lpcszTableName ) == 0 ) 
			return &(*it);
	}

	return NULL;
}
/**
 * テーブル名とフィールド名から既に登録済みの CFieldDef オブジェクトのポインタを返す
 *
 * @param lpszTableName	[in]	テーブル名
 * @param lpszFieldName	[in]	フィールド名
 *
 * @return 存在すれば CFieldDef オブジェクトポインタ、存在しなければ NULL を返す
 *
 * \note 外部テーブルには対応しません（複数のレコードが存在する場合があるため）
 */
CFieldDef* CFeatureDef::GetFieldDef( LPCTSTR lpszTableName, LPCTSTR lpszFieldName )
{
	if( m_strTableName.CompareNoCase( lpszTableName ) == 0 )
		return CRowDef::GetFieldDef( lpszFieldName );
	else
		return NULL;
}
/**
 * フィーチャのフィールドと、リレーションしている外部テーブルのレコードで変更がないかどうか調べる
 *
 * @return 変更がある場合は TRUE、なければ FALSE
 */
BOOL CFeatureDef::Changed()
{
	// フィーチャフィールド
	if( CRowDef::Changed() )
		return TRUE;

	// 外部テーブルフィールド
	std::list<CRelationalTableDef>* pRelationalTableDef = GetRelationalTableDefList();
	for( std::list<CRelationalTableDef>::iterator it = pRelationalTableDef->begin(); it != pRelationalTableDef->end(); it++ ) 
	{
		if( it->getChanged() )
			return TRUE;

		for( std::list<CRowDef>::iterator itRow = it->begin(); itRow != it->end(); itRow++ )
		{
			// 外部テーブルのレコードに新規作成フラグ又は削除フラグが付いていたら変更ありとする
			if( itRow->GetDeleteFlag() || itRow->GetCreateFlag() ) {
				ATLTRACE(_T("CFeatureDef::Changed()：外部テーブルのレコードに新規作成フラグ又は削除フラグが付いています\n"));

				return TRUE;
			}
			if( itRow->Changed() )
				return TRUE;
		}
	}
	return FALSE;
}


// 外部関連テーブル名、フィールド名を取得する
bool CFeatureDef::getOuterTableAndFieldName(const CString& strTableName, const CString& strFieldName, CString& strOuterTableName, CString& strOuterFieldName)
{
	using namespace sindy::schema;

	strOuterTableName = _T("");
	strOuterFieldName = _T("");

	// DUPLILINKフィールド
	if( strFieldName.CompareNoCase( road_link::kDupliLinkRF ) == 0)
	{
			strOuterTableName = _T("DUPLI_LINK");
			strOuterFieldName = _T("LINK_ID");
			return true;
	}

	// 道路リンク
	if( strTableName.CompareNoCase( road_link::kTableName ) == 0)
	{
		if( strFieldName.CompareNoCase( road_link::kOnewayRF) == 0 ){
			strOuterTableName = time_oneway::kTableName;
			strOuterFieldName = time_oneway::kLinkID;
		}
		else if( strFieldName.CompareNoCase( road_link::kNoPassageRF) == 0 ){
			strOuterTableName = time_nopassage::kTableName;
			strOuterFieldName = time_nopassage::kLinkID;
		}
		return true;
	}
	// 歩行者リンク
	if( strTableName.CompareNoCase( walk_link::kTableName ) == 0)
	{
		if( strFieldName.CompareNoCase( walk_link::kOnewayRF) == 0 ){
			strOuterTableName = _T("WALK_ONEWAY");
			strOuterFieldName = _T("LINK_ID");
		}
		else if( strFieldName.CompareNoCase( walk_link::kNoPassageRF) == 0 ){
			strOuterTableName = _T("WALK_NOPASSAGE");
			strOuterFieldName = _T("LINK_ID");
		}
		return true;
	}
	// 都市地図注記表示位置テーブル
	if( strTableName.CompareNoCase( city_disp_line::kTableName) == 0 )
	{
		if( strFieldName.CompareNoCase( city_disp_line::kAnnoID) == 0 ){
			strOuterTableName = city_annotation::kTableName;
			strOuterFieldName = city_annotation::kObjectID;
		}
		return true;
	}
	// 中縮注記表示位置テーブル
	if( strTableName.CompareNoCase( b_sc1disp_line::kTableName) == 0 ||
	    strTableName.CompareNoCase( b_sc2disp_line::kTableName) == 0 ||
	    strTableName.CompareNoCase( b_sc3disp_line::kTableName) == 0 ||
	    strTableName.CompareNoCase( b_sc4disp_line::kTableName) == 0 ) 
	{
		if( strFieldName.CompareNoCase( b_sc1disp_line::kAnnoID) == 0 ){
			strOuterTableName = base_annotation::kTableName;
			strOuterFieldName = base_annotation::kObjectID;
		}
		return true;
	}
	// ミドル注記表示位置テーブル
	if( strTableName.CompareNoCase( m_sc1disp_line::kTableName) == 0 ||
	    strTableName.CompareNoCase( m_sc2disp_line::kTableName) == 0 ||
	    strTableName.CompareNoCase( m_sc3disp_line::kTableName) == 0 ||
	    strTableName.CompareNoCase( m_sc4disp_line::kTableName) == 0 ) 
	{
		if( strFieldName.CompareNoCase( m_sc1disp_line::kAnnoID) == 0 ){
			strOuterTableName = middle_annotation::kTableName;
			strOuterFieldName = middle_annotation::kObjectID;
		}
		return true;
	}
	// トップ注記表示位置テーブル
	if( strTableName.CompareNoCase( t_sc1disp_line::kTableName) == 0 ||
	    strTableName.CompareNoCase( t_sc2disp_line::kTableName) == 0 ||
	    strTableName.CompareNoCase( t_sc3disp_line::kTableName) == 0 ||
	    strTableName.CompareNoCase( t_sc4disp_line::kTableName) == 0 ) 
	{
		if( strFieldName.CompareNoCase( t_sc1disp_line::kAnnoID) == 0 ){
			strOuterTableName = top_annotation::kTableName;
			strOuterFieldName = top_annotation::kObjectID;
		}
		return true;
	}

	// 鉄道リンクテーブル
	if( strTableName.CompareNoCase( sj::railway_link::kTableName) == 0 )
	{
		if( strFieldName.CompareNoCase( object::kObjectID) == 0 ){
			strOuterTableName = sj::rel_railway_link::kTableName;
			strOuterFieldName = sj::rel_railway_link::kRwLinkID;
		}
		return true;
	}
	// 鉄道ノードテーブル
	if( strTableName.CompareNoCase( sj::railway_node::kTableName) == 0 )
	{
		if( strFieldName.CompareNoCase( object::kObjectID) == 0 ){
			strOuterTableName = sj::rel_railway_node::kTableName;
			strOuterFieldName = sj::rel_railway_node::kRwNodeID;
		}
		return true;
	}

	// 駅ポリゴンテーブル
	if( strTableName.CompareNoCase( sj::station_detail::kTableName ) == 0   ||
	    strTableName.CompareNoCase( sj::midzoom_platform::kTableName ) == 0 ||
	    strTableName.CompareNoCase( sj::highzoom_station::kTableName ) == 0 ||
	    strTableName.CompareNoCase( sj::midzoom_station::kTableName ) == 0  ||
	    strTableName.CompareNoCase( sj::lowzoom_platform::kTableName ) == 0   )
	{
		if( strFieldName.CompareNoCase( object::kObjectID) == 0 ){
			strOuterTableName = sj::rel_station_site::kTableName;
			strOuterFieldName = sj::rel_station_site::kLayerOID;
		}
		return true;
	}

	// [ADAM] 横断歩道ライン
	if( strTableName.CompareNoCase( adam_v2::crosswalk_line::kTableName ) == 0 )
	{
		if( strFieldName.CompareNoCase( adam_v2_adam_ipc_feature::kGlobalID) == 0 ){
			strOuterTableName = adam_v2::rel_lane_node_crosswalk_line::kTableName;
			strOuterFieldName = adam_v2::rel_lane_node_crosswalk_line::kCrosswalkLineGID;
		}
		return true;
	}

	// [ADAM] 道路標識ポイント
	if (strTableName.CompareNoCase(adam_v2::roadsign_point::kTableName) == 0)
	{
		if (strFieldName.CompareNoCase(adam_v2_adam_ipc_feature::kGlobalID) == 0) {
			strOuterTableName = adam_v2::rel_lane_node_roadsign_point::kTableName;
			strOuterFieldName = adam_v2::rel_lane_node_roadsign_point::kRoadsignPointGID;
		}
		return true;
	}

	// [ADAM] 車線リンク
	if (strTableName.CompareNoCase(adam_v2::lane_link::kTableName) == 0)
	{
		if (strFieldName.CompareNoCase(adam_v2_adam_ipc_feature::kGlobalID) == 0) {
			strOuterTableName = adam_v2::nq_lane_node::kTableName;
			strOuterFieldName = adam_v2::nq_lane_node::kLaneNodeGID;
		}
		return true;
	}

	// [ADAM] 車線ノード列
	if (strTableName.CompareNoCase(adam_v2::lane_node::kTableName) == 0)
	{
		if (strFieldName.CompareNoCase(adam_v2_adam_ipc_feature::kGlobalID) == 0) {
			strOuterTableName = adam_v2::nq_lane_node::kTableName;
			strOuterFieldName = adam_v2::nq_lane_node::kLaneLinkGID;
		}
		return true;
	}

	// [ADAM] 上下線リンク
	if (strTableName.CompareNoCase(adam_v2::updown_link::kTableName) == 0)
	{
		if (strFieldName.CompareNoCase(adam_v2_adam_ipc_feature::kGlobalID) == 0) {
			strOuterTableName = adam_v2::nq_updown_node::kTableName;
			strOuterFieldName = adam_v2::nq_updown_node::kUpdownNodeGID;
		}
		return true;
	}

	// [ADAM] 上下線ノード列
	if (strTableName.CompareNoCase(adam_v2::updown_node::kTableName) == 0)
	{
		if (strFieldName.CompareNoCase(adam_v2_adam_ipc_feature::kGlobalID) == 0) {
			strOuterTableName = adam_v2::nq_updown_node::kTableName;
			strOuterFieldName = adam_v2::nq_updown_node::kUpdownLinkGID;
		}
		return true;
	}

	// [ADAM] 区画線リンク
	if (strTableName.CompareNoCase(adam_v2::compart_link::kTableName) == 0)
	{
		if (strFieldName.CompareNoCase(adam_v2_adam_ipc_feature::kGlobalID) == 0) {
			strOuterTableName = adam_v2::nq_compart_node::kTableName;
			strOuterFieldName = adam_v2::nq_compart_node::kCompartNodeGID;
		}
		return true;
	}

	// [ADAM] 区画線ノード列
	if (strTableName.CompareNoCase(adam_v2::compart_node::kTableName) == 0)
	{
		if (strFieldName.CompareNoCase(adam_v2_adam_ipc_feature::kGlobalID) == 0) {
			strOuterTableName = adam_v2::nq_compart_node::kTableName;
			strOuterFieldName = adam_v2::nq_compart_node::kCompartLinkGID;
		}
		return true;
	}

	// [ADAM] 道路標示エリア
	if (strTableName.CompareNoCase(adam_v2::roadmark_area::kTableName) == 0)
	{
		if (strFieldName.CompareNoCase(adam_v2_adam_ipc_feature::kGlobalID) == 0) {
			strOuterTableName = adam_v2::rel_roadmark_area_lane_link::kTableName;
			strOuterFieldName = adam_v2::rel_roadmark_area_lane_link::kRoadMarkAreaGID;
		}
		return true;
	}

	// [ADAM] 道路境界線リンク
	if (strTableName.CompareNoCase(adam_v2::border_link::kTableName) == 0)
	{
		if (strFieldName.CompareNoCase(adam_v2_adam_ipc_feature::kGlobalID) == 0) {
			strOuterTableName = adam_v2::nq_border_node::kTableName;
			strOuterFieldName = adam_v2::nq_border_node::kBorderLinkGID;
		}
		return true;
	}

	return false;
}
