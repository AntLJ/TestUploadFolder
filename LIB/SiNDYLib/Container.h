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

#include <boost/shared_ptr.hpp>
#include <deque>
#include "define_enum.h"
#include "TableTypeUtil.h"
#include "ErrorCode.h"
#include "EditDef.h"
#include "sindy/schema.h"

namespace sindy {

class CRow;
typedef boost::shared_ptr<CRow> CSPRow;
class CErrorObjects;
class CLogicRule;
class CGeometry;

/// CContainer::Queryで使用する比較オペレータ
enum sindyCompareOperator {
	sindyCompOpeEqual,            //!< 同値（=）
	sindyCompOpeThanSmall,        //!< 引数より小さい（<）
	sindyCompOpeThanBig,          //!< 引数より大きい（>）
	sindyCompOpeThanSmallOrEqual, //!< 同値か引数より小さい（<=）
	sindyCompOpeThanBigOrEqual,   //!< 同値か引数より大きい（>=）
	sindyCompOpeNotEqual,         //!< 等しくない（!=）
};

class SINDYLIB_API CContainer
{
	friend class FeatureContainer;
	friend class CRoadNetwork;
public:
	CContainer(void);
	virtual ~CContainer(void);

	CContainer& operator =( const CContainer& obj );

	errorcode::sindyErrCode Materialization( ITable* ipTable );

	//@{ コンテナ操作関数
	typedef std::deque<CSPRow> TRows;
	typedef std::map<sindyTableType::ECode,TRows> TRowsContainer;
#ifdef _USE_FIND_INDEX
	typedef std::map<sindyTableType::ECode, std::map<long,long> > TRowsContainerOIDIndex;
#endif //_USE_FIND_INDEX
	typedef TRowsContainer::iterator iterator;
	typedef TRowsContainer::const_iterator const_iterator;
	typedef TRows::iterator rows_iterator;
	typedef TRows::const_iterator const_rows_iterator;
	typedef TRowsContainer::reverse_iterator reverse_iterator;
	typedef TRowsContainer::const_reverse_iterator const_reverse_iterator;
	typedef TRows::reverse_iterator reverse_rows_iterator;
	typedef TRows::const_reverse_iterator const_reverse_rows_iterator;
	// 順方向
	iterator begin(){ return m_mapRowsContainer.begin(); }
	iterator end(){ return m_mapRowsContainer.end(); }
	const_iterator begin() const { return m_mapRowsContainer.begin(); }
	const_iterator end() const { return m_mapRowsContainer.end(); }
	rows_iterator begin(sindyTableType::ECode emType){ return m_mapRowsContainer[emType].begin(); }
	rows_iterator end(sindyTableType::ECode emType){ return m_mapRowsContainer[emType].end(); }
	const_rows_iterator begin(sindyTableType::ECode emType) const {
		return (*const_cast<TRowsContainer*>(&m_mapRowsContainer))[emType].begin();
	}
	const_rows_iterator end(sindyTableType::ECode emType) const {
		return (*const_cast<TRowsContainer*>(&m_mapRowsContainer))[emType].end();
	}
	// 逆方向
	reverse_iterator rbegin(){ return m_mapRowsContainer.rbegin(); }
	reverse_iterator rend(){ return m_mapRowsContainer.rend(); }
	const_reverse_iterator rbegin() const { return m_mapRowsContainer.rbegin(); }
	const_reverse_iterator rend() const { return m_mapRowsContainer.rend(); }
	reverse_rows_iterator rbegin(sindyTableType::ECode emType){ return m_mapRowsContainer[emType].rbegin(); }
	reverse_rows_iterator rend(sindyTableType::ECode emType){ return m_mapRowsContainer[emType].rend(); }
	const_reverse_rows_iterator rbegin(sindyTableType::ECode emType) const {
		return (*const_cast<TRowsContainer*>(&m_mapRowsContainer))[emType].rbegin();
	}
	const_reverse_rows_iterator rend(sindyTableType::ECode emType) const {
		return (*const_cast<TRowsContainer*>(&m_mapRowsContainer))[emType].rend();
	}

	void push_back( const CSPRow& cRow, bool bSecureUnique = true );
	void push_front( const CSPRow& cRow, bool bSecureUnique = true );
	virtual void clear();
	bool empty() const { return m_mapRowsContainer.empty(); }
	bool empty(sindyTableType::ECode emType) const {
		const_iterator it = m_mapRowsContainer.find(emType);
		if( it != end() ) 
			return it->second.empty();
		return true;
	}
	size_t size() const { return m_mapRowsContainer.size(); }
	size_t size(sindyTableType::ECode emType) const {
		const_iterator it = m_mapRowsContainer.find(emType);
		if( it != end() )
			return it->second.size();
		return (size_t)0;
	}
	CSPRow find( _IRow* ipRow ) const;
	CSPRow find( const CRow& cRow ) const;
	iterator find( sindyTableType::ECode emType ){ return m_mapRowsContainer.find(emType); }
	const_iterator find( sindyTableType::ECode emType ) const { return m_mapRowsContainer.find(emType); }
	rows_iterator find( sindyTableType::ECode emType, long lOID );
	const_rows_iterator find( sindyTableType::ECode emType, long lOID ) const;
	rows_iterator find( sindyTableType::ECode emType, LPCTSTR lpcszFieldName, const VARIANT& vaValue );
	const_rows_iterator find( sindyTableType::ECode emType, LPCTSTR lpcszFieldName, const VARIANT& vaValue ) const;

	/**
	 * @brief <b>コンテナから除外する</b>\n
	 * @param cRow		[in]	除外対象
	 */
	void erase( const CSPRow& cRow );
	rows_iterator erase( sindyTableType::ECode emType, rows_iterator position );
	rows_iterator erase( sindyTableType::ECode emType, rows_iterator first, rows_iterator last );

	CSPRow operator[]( size_t pos );
	CSPRow operator[]( size_t pos ) const;
	//@}

	/**
	 * @brief <b>コンテナ内オブジェクトのうち、中抜き部分でcGeomを内包するものを抜き出す</b>\n
	 * @param emTableType   [in] 包含対象テーブルタイプ
	 * @param cGeom         [in] チェック形状
	 * @param cContains	[in] チェック形状を包含しているオブジェクト群
	 * @note 指定したテーブルタイプのコンテナ内オブジェクトのうち、中抜き部分でcGeomを内包するものを抜き出す
	 */
	bool QueryDoughnutInteriorContainFeatures( sindyTableType::ECode emTableType, const CGeometry& cGeom, CContainer& cContains ) const;

	/**
	 * @brief コンテナ内オブジェクトを特定の条件で抽出する
	 * 
	 * 返されるCContainerはただのコンテナであり、格納されるCRowオブジェクトはshared_ptrに
	 * 包まれている為、コピーのコストはそれ程発生しません。
	 * それぞれの関数は単発の条件しか設定できませんが、返されたCContainerに対して更にQuery
	 * を呼ぶことができるので、複数の条件を一行で指定することが可能です。
	 * 
	 * voidを返す版はコピーコストを最小限にとどめたい場合や、追加でコンテナに入れたい場合に使用します。
	 * また、内部的に階層を辿るときにもこちらを使用します。
	 *
	 * 例：cContainerから、都市地図注記のOBJECTID=100を抽出する\n
	 * CConatiner cResult( cContainer.Query( sindyeTableType::city_annotation ).Query( sindyCompOpeEqual, _T("OBJECTID"), CComVariant(100L) ) );
	 *
	 * @param emTableType		[in]    テーブルタイプ
	 * @param ipFilterGeom		[in]	フィルタジオメトリ（NULLの場合は全て取得）
	 * @param emSpRel			[in]	関連フィルタ（esriSpatialIntersectsしか対応していません）
	 * @param emOperator		[in]	比較演算子
	 * @param lpcszFieldName	[in]	フィールド名
	 * @param vaValue			[in]	比較値
	 * @param lpcszTableName	[in]	テーブル名
	 * @retval 抽出されたオブジェクト群
	 */
	CContainer Query( sindyCompareOperator emOperator, sindyTableType::ECode emTableType ) const;
	CContainer Query( sindyCompareOperator emOperator, const CGeometry& cGeom, sindyeSpatialRel emSpRel ) const;
	CContainer Query( sindyCompareOperator emOperator, long lFieldIndex, const VARIANT& vaValue ) const;
	CContainer Query( sindyCompareOperator emOperator, LPCTSTR lpcszFieldName, const VARIANT& vaValue ) const;
	CContainer Query( sindyCompareOperator emOperator, LPCTSTR lpcszTableName ) const;
	void Query( sindyCompareOperator emOperator, sindyTableType::ECode emTableType, CContainer& cContainer ) const;
	void Query( sindyCompareOperator emOperator, const CGeometry& cGeom, sindyeSpatialRel emSpRel, CContainer& cContainer ) const;
	void Query( sindyCompareOperator emOperator, long lFieldIndex, const VARIANT& vaValue, CContainer& cContainer ) const;
	void Query( sindyCompareOperator emOperator, LPCTSTR lpcszFieldName, const VARIANT& vaValue, CContainer& cContainer ) const;
	void Query( sindyCompareOperator emOperator, LPCTSTR lpcszTableName, CContainer& cContainer ) const;

	/**
	 * @brief <b>フィーチャの変更を元に戻す</b>\n
	 */
	void Reset( bool bRecursive = false );

	/**
	 * @brief <b>削除する</b>\n
	 * 削除フラグを立てます。
	 * 反映させるにはStore()する必要があります。
	 * @param lpcszTableName [in,optional] 特定のテーブル名を持つもののみ削除したい場合に指定します（デフォルト：NULL)
	 * @param bRecursive [in,optional] 内部のCContainerにも適用する場合はtrue（デフォルト：false）
	 */
	void Delete( LPCTSTR lpcszTableName = NULL, bool bRecursive = false );

	//@{ @name 編集系関数
	errorcode::sindyErrCode Move( const CGeometry& cGeom, const std::pair<double,double>& pairMove, sindyeMoveMode emMove, CErrorObjects& cErrObjects );
	/**
	 * @brief <b>編集内容を保存する</b>\n
	 * この関数では整合性のチェックを行いません。\n
	 * 必ずCheck関数を呼んだ後にこの関数を呼んでください。
	 */
	virtual errorcode::sindyErrCode Store( CErrorObjects& cErrObjects );
	virtual errorcode::sindyErrCode CheckLogic( const CLogicRule& cRule, CErrorObjects& cErrObjects ) const;
	virtual errorcode::sindyErrCode CheckModify( CErrorObjects& cErrObjects ) const;
	//@}
	virtual bool IsCached() const;
	virtual IEnvelopePtr GetEnvelope( bool bOriginal = true, bool bChanged = true, bool bFeatureChangedOriginal = true ) const;
	virtual IGeometryPtr GetShape(  bool bLatest = true, bool bOriginal = false, bool bFeatureChangedOriginal = false, long lParam = -1 ) const;
	virtual errorcode::sindyErrCode CreateCache( bool bDetatch = false );
	/**
	 * @brief <b>キャッシュを削除する</b>
	 * @param bRecursive [in,optional] CRowが抱えているコンテナのキャッシュも削除する場合はtrue（デフォルト：false）
	 */
	void DeleteCache( bool bRecursive = false );

	/**
	 * @brief キャッシュ以外のパラメータを更新する
	 * @param bForce [in,optional] 強制的に作り直す場合はtrue（デフォルト：false）
	 * 
	 * 属性・形状に付随する各種パラメータを更新します。
	 * デフォルトでは何も動作しません。注記の記号・文字列
	 * 矩形や、リンク列形状などを想定しています。
	 */
	virtual errorcode::sindyErrCode UpdateParameter( bool bForce = false );
	virtual void DeleteParameter();

	virtual void Trace( bool bAttribute = true, bool bGeometry = true ) const;

	/**
	 * @brief <b>変更があったオブジェクトの変更領域を取得</b>\n
	 */
	IGeometryPtr GetEditArea() const;

	/// find用のOIDインデックスを再構築する
	void UpdateIndex();
private:
#pragma warning(push)
#pragma warning(disable:4251)
	TRowsContainer m_mapRowsContainer;
#ifdef _USE_FIND_INDEX
	TRowsContainerOIDIndex m_mapRowsContainerOIDIndex;
#endif // _USE_FIND_INDEX
#pragma warning(pop)
};

} // sindy
