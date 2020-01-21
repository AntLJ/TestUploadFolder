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

//#include <deque>
#include <boost/shared_ptr.hpp>
#include "sindylib_base_global.h"
#include "TableTypeUtil.h"
#include "ErrorCode.h"
#include "EditDef.h"

namespace sindy {
//class CRowBase;
class CContainerBase;
class CErrorObjectsBase;
class CLogicRule;
class CGeometryBase;
class CFieldMap;
class CTableNameString;
//typedef boost::shared_ptr<CRowBase> CSPRowBase;
typedef boost::shared_ptr<CContainerBase> CSPContainerBase;
typedef boost::shared_ptr<CFieldMap> CSPFieldMap;
typedef boost::shared_ptr<CTableNameString> CSPTableNameString;

/// CContainerBase::Queryで使用する比較オペレータ
enum sindyCompareOperator2 {
	sindyCompOpeEqual2,            //!< 同値（=）
	sindyCompOpeThanSmall2,        //!< 引数より小さい（<）
	sindyCompOpeThanBig2,          //!< 引数より大きい（>）
	sindyCompOpeThanSmallOrEqual2, //!< 同値か引数より小さい（<=）
	sindyCompOpeThanBigOrEqual2,   //!< 同値か引数より大きい（>=）
	sindyCompOpeNotEqual2,         //!< 等しくない（!=）
};

/**
 * @class CContainerBase
 * @brief 汎用コンテナクラス
 * 汎用コンテナクラスと書いてありますが、CRowBase系とCFeatureContainer系を
 * 格納することを前提に書かれています。
 * メンバ変数としてshared_ptr<CContainerBase>を保持するため、CRowBase系と
 * CFeatureContainer系のどちらでも同階層に格納することが可能です。
 * ただし、一部の関数は一階層しか対象としないことに注意してください：
 * @li Materialization
 * @li empty
 * @li イテレータを返す関数全て（begin,end,find、erase）
 * @li push_back、push_front
 * @li QueryDoughnutInteriorContainFeatures
 * 
 * eraseに関しては、階層を辿るものも存在します。findに関しては、
 * 階層を辿ってCSPRowBaseを返すfind_rowが存在します。
 */
class CContainerBase
{
//	friend class FeatureContainer;
//	friend class CRoadNetwork;

	/// プレディケート
	class Equal
	{
	public:
		Equal() : m_pContainer(NULL) {}
		Equal( const CSPContainerBase& c ) : m_pContainer(&*c) {}
		bool operator()( const CSPContainerBase& c ) const;
	private:
		CContainerBase* m_pContainer;
	};
/*	class EqualTableType
	{
	public:
		EqualTableType() : m_emType(sindyTableType::unknown) {}
		EqualTableType( sindyTableType::ECode emType ) : m_emType(emType){}
		bool operator()( const CSPContainerBase& c ) const;
	private:
		sindyTableType::ECode m_emType;
	};
	class EqualTableTypeSPRowBase
	{
	public:
		EqualTableTypeSPRowBase() : m_emType(sindyTableType::unknown) {}
		EqualTableTypeSPRowBase( sindyTableType::ECode emType ) : m_emType(emType){}
		bool operator()( const CSPContainerBase& c ) const;
	private:
		sindyTableType::ECode m_emType;
	};
	class EqualTableType_and_OIDSPRowBase
	{
	public:
		EqualTableType_and_OIDSPRowBase() : m_emType(sindyTableType::unknown){}
		EqualTableType_and_OIDSPRowBase( sindyTableType::ECode emType, long lOID ) : m_emType(emType), m_lOID(lOID){}
		bool operator()( const CSPContainerBase& c ) const;
	private:
		sindyTableType::ECode m_emType;
		long m_lOID;
	};
	class EqualTableType_and_ValueSPRowBase
	{
	public:
		EqualTableType_and_ValueSPRowBase() : m_emType(sindyTableType::unknown){}
		EqualTableType_and_ValueSPRowBase(sindyTableType::ECode emType, LPCTSTR lpcszFieldName, const VARIANT& Value) : m_emType(sindyTableType::unknown), m_lpcszFieldName(lpcszFieldName), m_value(&Value){}
		bool operator()( const CSPContainerBase& c ) const;
	private:
		sindyTableType::ECode m_emType;
		LPCTSTR m_lpcszFieldName;
		const VARIANT* m_value;
	};
	class EqualRowBaseSPRowBase
	{
	public:
		EqualRowBaseSPRowBase(){}
		EqualRowBaseSPRowBase( const CRowBase& cRow ) : m_pRow(&cRow){}
		bool operator()( const CSPContainerBase& c ) const;
	private:
		const CRowBase* m_pRow;
	};*/

	typedef std::list<CSPContainerBase> TContainer;
protected:
	TContainer& GetContainer(){ return m_cContainer; }
	const TContainer& GetContainer() const { return m_cContainer; }
private:
#pragma warning(push)
#pragma warning(disable:4251)
	TContainer m_cContainer;
#pragma warning(pop)
public:
	CContainerBase(void);
	virtual ~CContainerBase(void);

	CContainerBase& operator =( const CContainerBase& obj );

	typedef TContainer::iterator iterator;
	typedef TContainer::const_iterator const_iterator;
	typedef TContainer::reverse_iterator reverse_iterator;
	typedef TContainer::const_reverse_iterator const_reverse_iterator;

	//@{ @name 順方向イテレータ
	iterator begin(){ return GetContainer().begin(); }
	iterator end(){ return GetContainer().end(); }
	const_iterator begin() const { return GetContainer().begin(); }
	const_iterator end() const { return GetContainer().end(); }
	//@}
	//@{ @name 逆方向イテレータ
	reverse_iterator rbegin(){ return GetContainer().rbegin(); }
	reverse_iterator rend(){ return GetContainer().rend(); }
	const_reverse_iterator rbegin() const { return GetContainer().rbegin(); }
	const_reverse_iterator rend() const { return GetContainer().rend(); }
	//@}
	//@{ コンテナ操作関数
	/**
	 * @brief コンテナへ追加する
	 * @param cContainer [in] 追加対象オブジェクト
	 * @param bSecureUnique [in,optional] 重複させない場合はtrue（デフォルト：true）
	 *
	 * CFeatureContainer系を追加する場合、第二引数がtrueならこの場合もCFeatureContainer系
	 * を探索します。この場合の比較対象はCTableNameStringであり、抱えているCRowBase系オブジェクト
	 * には左右されないことに注意してください。
	 */
	virtual void push_back( const CSPContainerBase& cContainer, bool bSecureUnique = true );
	virtual void push_front( const CSPContainerBase& cSPContainer, bool bSecureUnique = true );

	virtual iterator insert( iterator it, const CSPContainerBase& cContainer, bool bSecureUnique = true );

	virtual void clear();
	bool empty() const { return GetContainer().empty(); }
	size_t size() const { return GetContainer().size(); }
	iterator find( const CSPContainerBase& cContainer ){ return std::find_if( begin(), end(), Equal(cContainer) ); }
	const_iterator find( const CSPContainerBase& cContainer ) const { return std::find_if( begin(), end(), Equal(cContainer) ); }
//	iterator find( const CRowBase& cRowBase ){ return std::find_if( begin(), end(), EqualRowBaseSPRowBase(cRowBase) ); }
//	const_iterator find( const CRowBase& cRowBase )const { return std::find_if( begin(), end(), EqualRowBaseSPRowBase(cRowBase) ); }
//	iterator find( _IRow* ipRowBase );
//	const_iterator find( _IRow* ipRowBase ) const;
//	iterator find( sindyTableType::ECode emType, long lOID ){ return std::find_if( begin(), end(), EqualTableType_and_OIDSPRowBase(emType,lOID) ); }
//	const_iterator find( sindyTableType::ECode emType, long lOID ) const { return std::find_if( begin(), end(), EqualTableType_and_OIDSPRowBase(emType,lOID) ); }
//	iterator find( sindyTableType::ECode emType, LPCTSTR lpcszFieldName, const VARIANT& vaValue ){ return std::find_if( begin(), end(), EqualTableType_and_ValueSPRowBase(emType,lpcszFieldName,vaValue) ); }
//	const_iterator find( sindyTableType::ECode emType, LPCTSTR lpcszFieldName, const VARIANT& vaValue ) const { return std::find_if( begin(), end(), EqualTableType_and_ValueSPRowBase(emType,lpcszFieldName,vaValue) ); }

	/**
	 * @brief <b>コンテナから除外する</b>\n
	 * @param cRowBase		[in]	除外対象
	 */
//	void erase( const CSPRowBase& cRowBase );
	virtual iterator erase( iterator it );
	virtual iterator erase( iterator first, iterator last );

	/// ソート
	virtual void sort(){ GetContainer().sort(); }
	template<class _Pr3> void sort(_Pr3 _Pred){ GetContainer().sort(_Pred); }

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
//	CContainer Query( sindyCompareOperator emOperator, sindyTableType::ECode emTableType ) const;
//	CContainer Query( sindyCompareOperator emOperator, const CGeometry& cGeom, sindyeSpatialRel emSpRel ) const;
//	CContainer Query( sindyCompareOperator emOperator, long lFieldIndex, const VARIANT& vaValue ) const;
//	CContainer Query( sindyCompareOperator emOperator, LPCTSTR lpcszFieldName, const VARIANT& vaValue ) const;
	CSPContainerBase Query( sindyCompareOperator2 emOperator, LPCTSTR lpcszTableName, bool bSecureUnique = true ) const;
//	void Query( sindyCompareOperator emOperator, sindyTableType::ECode emTableType, CContainer& cContainer ) const;
//	void Query( sindyCompareOperator emOperator, const CGeometry& cGeom, sindyeSpatialRel emSpRel, CContainer& cContainer ) const;
//	void Query( sindyCompareOperator emOperator, long lFieldIndex, const VARIANT& vaValue, CContainer& cContainer ) const;
//	void Query( sindyCompareOperator emOperator, LPCTSTR lpcszFieldName, const VARIANT& vaValue, CContainer& cContainer ) const;
	virtual CContainerBase& Query( sindyCompareOperator2 emOperator, LPCTSTR lpcszTableName, CContainerBase& cContainer, bool bSecureUnique = true ) const;
	//@}

	/**
	 * @brief <b>フィーチャの変更を元に戻す</b>\n
	 */
	virtual void Reset();

	/**
	 * @brief <b>削除する</b>\n
	 * 削除フラグを立てます。
	 * 反映させるにはStore()する必要があります。
	 * @param lpcszTableName [in,optional] 特定のテーブル名を持つもののみ削除したい場合に指定します（デフォルト：NULL)
	 * @param bRecursive [in,optional] 内部のCContainerBaseにも適用する場合はtrue（デフォルト：false）
	 */
	virtual void Delete( /*LPCTSTR lpcszTableName = NULL*/ );
	virtual void UnDelete();

	//@{ @name 編集系関数
	// 与えられた形状の範囲で平行移動処理を行う
	virtual errorcode::sindyErrCode Move( const CGeometryBase& cGeom, const std::pair<double,double>& pairMove, sindyeMoveMode emMove, CErrorObjectsBase& cErrs );
	/**
	 * @brief <b>編集内容を保存する</b>\n
	 * この関数では整合性のチェックを行いません。\n
	 * 必ずCheck関数を呼んだ後にこの関数を呼んでください。
	 */
	virtual errorcode::sindyErrCode Store( CErrorObjectsBase& cErrs );
	virtual errorcode::sindyErrCode CheckLogic( const CLogicRule& cRule, CErrorObjectsBase& cErrObjects ) const;
	virtual errorcode::sindyErrCode CheckModify( CErrorObjectsBase& cErrObjects ) const;

	/**
	 * @brief 属性に変更があったかどうかをチェックする
	 * 
	 * 一つでも変更があればそれの結果を返します。
	 * @param emUpdateType	[out,optional]	変更タイプが入る（デフォルト：NULL）
	 * @retval true 変更あり
	 * @retval false 変更なし（キャッシュしていない場合は常にfalse）
	 */
	virtual bool Changed( schema::ipc_table::update_type::ECode* emUpdateType = NULL ) const;
	//@}
	virtual bool IsCached() const;
	virtual IEnvelopePtr GetEnvelope( ShapeMode emMode = enumLatest, bool bChangedOnly = false, long lParam = -1 ) const;

	/**
	 * @brief 形状のコピーを取得する
	 *
	 * 形状のコピーを様々なモードで行います。
	 * 取得した形状群は全て結合され一つの形状として返します。
	 * 次元の異なる形状が存在する場合は、CoClassはGeometryBagと
	 * なることに注意してください。そうではない場合はMultiPoint、
	 * Polyline、Polygonのどれかを返します。
	 *	 
	 * 形状の取得はGetShapeCopy( _ISetPtr&, ShapeMode, bool, long )を使用し、最後に
	 * ITopologicalOperator::ConstructUnionを使用して形状を結合します。
	 * 
	 * @param emMode       [in,optional] コピーを取得する際のモード（ShapeModeを参照 デフォルト：emLatest）
	 * @param bChangedOnly [in,optional] コピー取得を、変更があったフィーチャのみに限定するかどうかのフラグ（デフォルト：false）
	 * @param lParam       [in,optional] 各コンテナで使用するオプション（通常は使用しません デフォルト：-1）
	 * @return IGeometryPtr
	 */
	IGeometryPtr GetShapeCopy( ShapeMode emMode = enumLatest, bool bChangedOnly = false, long lParam = -1 ) const;

	/**
	 * @brief 形状のコピーを取得する
	 *
	 * 形状のコピーを様々なモードで行います。
	 * コピー形状はipColへ格納されます。
	 * 
	 * この関数はGetShapeCopy( ShapeMode, bool, long )で呼ばれます。
	 * 
	 * @param ipCol        [out] _ISetPtr（インスタンスが作成されている必要があります）
	 * @param emMode       [in,optional] コピーを取得する際のモード（ShapeModeを参照 デフォルト：emLatest）
	 * @param bChangedOnly [in,optional] コピー取得を、変更があったフィーチャのみに限定するかどうかのフラグ（デフォルト：false）
	 * @param lParam       [in,optional] 各コンテナで使用するオプション（通常は使用しません デフォルト：-1）
	 */
	virtual void GetShapeCopy( _ISetPtr& ipCol, ShapeMode emMode = enumLatest, bool bChangedOnly = false, long lParam = -1 ) const;

	virtual errorcode::sindyErrCode CreateCache( bool bDetach = false, const CSPFieldMap& spFields = CSPFieldMap(), const CSPTableNameString& spNameString = CSPTableNameString() );
	/**
	 * @brief <b>キャッシュを削除する</b>
	 * @param bRecursive [in,optional] CRowBaseが抱えているコンテナのキャッシュも削除する場合はtrue（デフォルト：false）
	 */
	void DeleteCache();

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
};

} // sindy
