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

/**
 * @file RowBase.h
 * @brief <b>CRowBaseクラス定義ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _ROWBASE_H_
#define _ROWBASE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <atlsimpcoll.h>
#include "WinLib/ComWrapper.h"
#include "FieldMap.h"
#include "TableType.h"
#include "ClassUtil.h"
#include "EditDef.h"
#include "sindylib_base_global.h"
#include "SchemaSupport.h"

#pragma warning(push)
#pragma warning(disable : 4275)
#pragma warning(disable : 4251)
namespace sindy {
class CErrorInfosBase;
class CErrorObjectsBase;
class CLogicRule;
class CCacheAttribute;
class CGeometryBase;
class CTable;
class CRowBase;
class CTableNameString;

typedef boost::shared_ptr<CRowBase> CSPRowBase;

/**
 * @class CRowBase
 * @brief IRowBasePtrのラッパークラス
 *
 * 全てのアイテムクラスはこのクラスが基底となります
 */
class CRowBase : public CComWrapper<_IRow>, public CSchemaSupport
{
public:
	CRowBase();
	virtual ~CRowBase()
	{
		DeleteCache();
	}

	CRowBase( const CRowBase& obj );
	CRowBase( _IRow* lp, sindyTableType::ECode emTableType = sindyTableType::unknown, bool bCreated = false, const CSPFieldMap& spFields = CSPFieldMap(), const CSPTableNameString& spNameString = CSPTableNameString() );

	CRowBase& operator=( const CRowBase& obj );
	CRowBase& operator=( _IRow* lp );

	operator _IRow*() const
	{
		return (_IRow*)p;
	}
	operator IGeometry*() const;

	bool operator ==( const CRowBase& obj ) const
	{
		return IsEqualObject( obj );
	}
	bool operator ==( _IRow* obj ) const
	{
		return IsEqualObject( obj );
	}
	bool operator ==( int null ) const
	{
        if (null != 0) {
            _com_issue_error(E_POINTER);
        }

        return p == NULL;
	}
	bool operator !=( int null ) const
	{
        if (null != 0) {
            _com_issue_error(E_POINTER);
        }

        return p != NULL;
	}
    operator bool() const throw()
    { 
        return p != NULL; 
    }
	bool operator <( const CRowBase& obj );

	_IRow** Release()
	{
		IUnknown* pTemp = p;
		if (pTemp)
		{
			p = NULL;
			pTemp->Release();
		}
		// キャッシュを削除する
		DeleteCache();
		// メンバ初期化
		Init();

		return &p;
	}

	/**
	 * @brief <b>フィーチャ・レコードをセットする</b>\n
	 * 内部でテーブル名を取得し、テーブルタイプを設定します。\n
	 * 第三パラメータは、新規に作成した場合にのみtrueをいれてください。このフラグ
	 * はStore時の履歴付与のために必要です。\n
	 * spFields、spNamesはなるべくセットするようにしてください。
	 * セットしなかった場合、例えばGetTableName()を呼んだときにその場で
	 * キャッシュを作成するためコストがかかります。
	 * @param lp			[in]			フィーチャ
	 * @param emTableType	[in,optional]	テーブルタイプ（デフォルト：sindyTableType::unknown）
	 * @param bCreated		[in,optional]	新規に作成されたフィーチャをセットする場合は必ずtrueを渡すこと（デフォルト：false）
	 * @param spFields		[in,optional]	フィールドマップキャッシュ（デフォルト：NULL）
	 * @param spNames		[in,optional]	各種名前情報キャッシュ（デフォルト：NULL）
	 */
	void SetObject( _IRow* lp, sindyTableType::ECode emTableType = sindyTableType::unknown, bool bCreated = false, const CSPFieldMap& spFields = CSPFieldMap(), const CSPTableNameString& spNames = CSPTableNameString() );

	/**
	 * @brief <b>ダミーCRowBaseを実体化させる</b>
	 * CTable::CreateDummyRowBaseで作成したダミーに対して実体をセットします。
	 * オブジェクトの作成タイミングをぎりぎりまで遅延させたい場合に有効です。
	 * CRowBase::pをセットするだけですのでキャッシュ等はクリアされません。
	 * テーブルタイプも確認しませんのでタイプの異なるものをセットしないでください。
	 */
	void Materialization( _IRow* lp );

	/**
	 * @brief <b>フィーチャのコピーを作成する</b>\n
	 * 同じテーブル上に同じ属性を持つオブジェクトを作成します。\n
	 * キャッシュはコピーされません。\n
	 * フィールド対応表、テーブル名キャッシュはコピーされます（shared_ptr）。
	 * @warning 編集開始されていなければエラーログが出ます。
	 * @warning IRowBaseがデタッチされている場合はエラーログが出ます。
	 * @retval CSPRowBase
	 */
	CSPRowBase Clone() const;

	/**
	 * @brief <b>フィーチャの変更を元に戻す</b>\n
	 * 作成フラグがついていたものはCTable::CreateDummyRowBase()相当の処理を
	 * 行います。\n
	 * 削除フラグはfalseになります。
	 * @param bRecursive [in] CContainerの中身も戻したい場合はtrue(デフォルト：false)
	 */
	virtual void Reset();

	/**
	 * @brief <b>デフォルト値をセットする</b>\n
	 * @warning pが存在するか、又はFieldMapがセットされている必要があります。
	 */
	void SetDefaultValue();

	/**
	 * @brief <b>新規作成時の自動付与属性を付与する<\b>\n
	 * @note ログインしている時のみに限定されます
	 */
	void SetCreateAutoValue();

	/**
	 * @brief <b>削除する</b>\n
	 * 削除フラグを立てます。
	 * 一度立てた削除フラグを元に戻すにはUnDeleteを使用します。
	 * 反映させるにはStore()する必要があります。
	 * @param bRecursive [in,optional] CContainerにも適用する場合はtrue（デフォルト：false）
	 */
	virtual void Delete(){ m_bDeleted = true; }
	virtual void UnDelete(){ m_bDeleted = false; }

	//@{ @name IRowBaseインターフェース
	IFieldsPtr GetFields() const;
	bool HasOID() const;
    long GetOID() const;
	virtual errorcode::sindyErrCode Store( CErrorObjectsBase& cErrs );
	ITablePtr GetTable() const;
	errorcode::sindyErrCode GetValue( long lIndex, VARIANT& vaValue ) const;
	virtual errorcode::sindyErrCode SetValue( long lIndex, const VARIANT& vaValue );
	/**
	 * @brief <b>CComVariantを返すGetValue</b>\n
	 * @param lIndex	[in]	フィールドインデックス番号
	 * @return CComVariant
	 */
	CComVariant GetValue( long lIndex ) const;

	//@}

	/**
	 * @brief <b>ワークスペースを取得する</b>\n
	 * @return IWorkspacePtr
	 */
	IWorkspacePtr GetWorkspace() const;

	//@{ @name CSchemaSupportオーバーライド
	/**
	 * @brief <b>フィールド対応表の参照を返す</b>\n
	 * m_spFieldsの実体の参照を返します。\n
	 * m_spNameStringの参照を返します。\n
	 * 上記メンバ変数は、CreateCache関数で自動的に作成されますが、
	 * キャッシュが存在しない場合はこの関数が呼ばれた時点で新規に作成されます。
	 * @warning この関数で実体が自動生成されると、その分リソースが消費されます。
	 * 同じ種類のオブジェクトが複数存在する場合はできるだけCreateCacheでshared_ptr
	 * を渡して共有できるように心がけてください。
	 * @return boost::shared_ptr<CFieldMap>
	 */
	virtual CSPFieldMap GetFieldMap() const;

	/**
	 * @brief <b>テーブル名などを管理するメンバの参照を返す</b>\n
	 * m_spNameStringの実体の参照を返します。\n
	 * 上記メンバ変数は、CreateCache関数で自動的に作成されますが、
	 * キャッシュが存在しない場合はこの関数が呼ばれた時点で新規に作成されます。
	 * @warning この関数で実体が自動生成されると、その分リソースが消費されます。
	 * 同じ種類のオブジェクトが複数存在する場合はできるだけCreateCacheでshared_ptr
	 * を渡して共有できるように心がけてください。
	 * @return boost::shared_ptr<CTableNameString>
	 */
	virtual CSPTableNameString GetNameString() const;
	//@}

	bool IsEqualObject(_IRow* ipRowBase ) const;
	bool IsEqualObject( const CRowBase& cRowBase ) const;

	/**
	 * @brief <b>フィールド名でフィールド値を取得する</b>\n
	 * フィールド対応表が無ければ作成してから取得します。
	 * @param lpcszFieldName	[in]	フィールド名
	 * @param vaValue			[out]	フィールド値
	 *
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode GetValue( LPCTSTR lpcszFieldName, VARIANT& vaValue ) const { return GetValue( FindField(lpcszFieldName), vaValue ); }

	/**
	 * @brief <b>CComVariantを返すGetValue</b>\n
	 * @param lpcszFieldName	[in]	フィールド名
	 * @return CComVariant
	 */
	CComVariant GetValue( LPCTSTR lpcszFieldName ) const { return GetValue( FindField(lpcszFieldName) ); }

	/**
	 * @brief <b>フィールド名でフィールド値をセットする</b>\n
	 * フィールド対応表が無ければ作成してからセットします。
	 * @param lpcszFieldName	[in]	フィールド名
	 * @param vaValue			[out]	フィールド値
	 *
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetValue( LPCTSTR lpcszFieldName, const VARIANT& vaValue ){ return SetValue( FindField(lpcszFieldName), vaValue ); }

	/**
	 * @brief <b>属性に変更があったかどうかをチェックする</b>\n
	 * キャッシュしている場合に限り有効です。キャッシュしていなければ、常に
	 * falseが返ります。
	 * 仮想関数なのは、リンク列でCLQに変更があった時にCInfoを更新しなければいけないから
	 * @param emUpdateType	[out,optional]	変更タイプが入る（デフォルト：NULL）
	 * @retval true 変更あり
	 * @retval false 変更なし（キャッシュしていない場合は常にfalse）
	 */
	virtual bool Changed( schema::ipc_table::update_type::ECode* emUpdateType = NULL ) const;

	/**
	 * @brief <b>属性に変更があったかどうかをチェックする</b>\n
	 * キャッシュしている場合に限り有効です。キャッシュしていなければ、常に
	 * falseが返ります。
	 * @param lpcszFieldName	[in]	フィールド名
	 * @retval true 変更あり
	 * @retval false 変更なし（キャッシュしていない場合は常にfalse）
	 */
	bool Changed( LPCTSTR lpcszFieldName ) const;

	/// 形状に変更があったかどうかチェックする
	bool ShapeChanged() const;

	/**
	 * @brief <b>整合性をチェックする</b>\n
	 * この関数では基本的なチェックしか行いません。
	 * 必要なチェックは、継承先のクラスで実装してください。
	 * @param cRule			[in]	整合性チェック用ルール
	 * @param cErrs			[out]	エラー情報格納用コンテナ
	 * @return sindyErrCode
	 */
	virtual errorcode::sindyErrCode CheckLogic( const CLogicRule& cRule, CErrorObjectsBase& cErrs ) const;
	void CheckLogic( const CLogicRule& cRule, CErrorInfosBase& infos ) const;

	/**
	 *  @brief フィールド単位のチェックをする
	 * 
	 * @warning 編集可能フィールドしか入ってきません
	 * 
	 * @param cRule			[in]	整合性チェック用ルール
	 * @param cErrs			[out]	エラー情報格納用コンテナ
	 * @param lIndex        [in]    フィールドインデックス
	 * @param cFieldAttr    [in]    フィールド情報
	 * @param vaValue       [in]    現在の属性値
	 * @param vaOrg         [in]    もともとの属性値
	 * @param bChanged      [in]    属性に変更があるかどうか
	 * @param bIsSDE        [in]    SDEかどうか（＝SJISかUNICODEかどうか）
	 */
	virtual void CheckLogic_Field( const CLogicRule& cRule, CErrorInfosBase& cErrs, long lIndex, const CFieldMap::_FIELD& cFieldAttr, const CComVariant& vaValue, const CComVariant& vaOrg, bool bChanged, bool bIsSDE ) const;

	/**
	 * @brief <b>コンフリクト回避のための既編集チェックを行う</b>\n
	 * @param cErrs		[out]	エラー情報格納用コンテナ
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckModify( CErrorObjectsBase& cErrs ) const;

	/**
	 * @brief <b>編集可能かどうかをチェックする</b>\n
	 * 編集ルールに基づいて編集可能かどうかをチェックします。\n
	 * 編集済みチェックは行いません。
	 * @param emType          [in]          編集タイプ
	 * @param cErrInfos       [out]         エラー格納用コンテナ
	 * @param check_modify    [in,optional] ModifyCheckをするかどうか（デフォルト：true）
	 * @param err2warn_modify [in,optional] ModifyCheckを警告扱いにするかどうか（デフォルト：false）
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckEditable( sindyeEditType emType, CErrorInfosBase& cErrInfos, bool check_modify = true, bool err2warn_modify = false ) const;

	long GetValue( long index, long lDefaultValue ) const;
	double GetValue( long index, const double& dDefaultValue ) const;
	bool GetValue( long index, bool bDefaultValue ) const;
	CString GetValue( long index, LPCTSTR lpcszDefaultValue ) const;

	long GetValue( LPCTSTR lpcszFieldName, long lDefaultValue ) const { return GetValue( FindField(lpcszFieldName), lDefaultValue ); }
	double GetValue( LPCTSTR lpcszFieldName, const double& dDefaultValue ) const { return GetValue( FindField(lpcszFieldName), dDefaultValue ); }
	bool GetValue( LPCTSTR lpcszFieldName, bool bDefaultValue ) const { return GetValue( FindField(lpcszFieldName), bDefaultValue ); }
	CString GetValue( LPCTSTR lpcszFieldName, LPCTSTR lpcszDefaultValue ) const { return GetValue( FindField(lpcszFieldName), lpcszDefaultValue ); }

	/**
	 * @brief インデックス番号でlong値をセットする
	 * @param index				[in]			フィールドインデックス番号
	 * @param lNewValue			[in]			セットする値
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetValue( long index, long lNewValue );
	errorcode::sindyErrCode SetValue( long index, int iNewValue ){ return SetValue( index, (long)iNewValue ); }
	errorcode::sindyErrCode SetValue( long index, const double& dNewValue );
	errorcode::sindyErrCode SetValue( long index, bool bNewValue );
	errorcode::sindyErrCode SetValue( long index, LPCTSTR lpcszNewValue );

	errorcode::sindyErrCode SetValue( LPCTSTR lpcszFieldName, long lNewValue ) { return SetValue( FindField(lpcszFieldName), lNewValue ); }
	errorcode::sindyErrCode SetValue( LPCTSTR lpcszFieldName, const double& dNewValue ) { return SetValue( FindField(lpcszFieldName), dNewValue ); }
	errorcode::sindyErrCode SetValue( LPCTSTR lpcszFieldName, bool bNewValue ) { return SetValue( FindField(lpcszFieldName), bNewValue ); }
	errorcode::sindyErrCode SetValue( LPCTSTR lpcszFieldName, LPCTSTR lpcszNewValue ) { return SetValue( FindField(lpcszFieldName), lpcszNewValue ); }

	/**
	 * @brief 値がNULLかどうか確認する
	 * 
	 * @param index [in] フィールドインデックス番号
	 * @retval true 値はNULL
	 * @retval false 値はNULLではない
	 */
	bool IsNull( long index ) const;
	bool IsNull( LPCTSTR lpcszFieldName ) const { return IsNull( FindField(lpcszFieldName) ); }
	/**
	 * @brief 指定されたフィールドがデフォルト値かどうかをチェックする
	 *
	 * @param lpcszFieldName [in] フィールド名
	 * @retval true デフォルト値である
	 * @retval false デフォルト値ではない（取得エラーの場合も含む）
	 */
	bool IsDefaultValue( LPCTSTR lpcszFieldName ) const;

	/**
	 * @brief 変更前の値を取得する
	 */
	CComVariant GetOriginalValue( LPCTSTR lpcszFieldName ) const;
	CComVariant GetOriginalValue( long lFieldIndex ) const;

	/**
	 * @brief フィールド名から相当するコード値ドメインの文字列を取得する
	 * @param lpszFieldName	[in]	対象フィールド名
	 * @return コード値ドメインの文字列を取得（コード値ドメインの文字列を取得できなかった場合は値をそのまま返す）
	 */
	CString GetDomainName( LPCTSTR lpszFiedlName ) const;

	/**
	 * @brief <b>属性・形状などのキャッシュを作成する</b>\n
	 * フィールド対応表、テーブル名キャッシュオブジェクトが無ければ作成してから作成しますが、
	 * その場合ほかのオブジェクトと共有できないためリソースを多く必要とします。
	 * リソースがどうでもよい場合、及びオブジェクトが少ない場合を除いて極力フィールド対応表
	 * とテーブル名キャッシュオブジェクトを渡すようにしてください。\n
	 * 既にキャッシュが存在する場合はキャッシュを作成しません。また、
	 * bDetachをtrueにすると、キャッシュ作成後に
	 * _IRow*を切り離します。
	 * @param bDetach			[in,optional]	処理後_IRow*を切り離す場合はtrue
	 * @param ipFields			[in,optional]	フィールド対応表オブジェクト(shared_ptr)
	 * @param spNameString		[in,optional]	テーブル名等管理オブジェクト(shared_ptr)
	 * @return sindyErrCode;
	 */
	virtual errorcode::sindyErrCode CreateCache( bool bDetach = false, const CSPFieldMap& spFields = CSPFieldMap(), const CSPTableNameString& spNameString = CSPTableNameString() );

	/**
	 * @brief <b>キャッシュを削除する</b>\n
	 * デストラクタから呼ばれます。
	 * 継承先クラスでオーバーライドした場合、
	 * C++の規約により例えvirtualでもデストラクタで
	 * CRowBase::DeleteCache()が必ず呼ばれることに注意してください。
	 */
	virtual void DeleteCache();

	/**
	 * @brief <b>キャッシュされているかどうかをチェックする</b>\n
	 * @retval true		全てキャッシュされている
	 * @retval false	一つでもキャッシュされていないものがある
	 */
	virtual bool IsCached() const;

	/**
	 * @brief 形状を返す
	 * 
	 * @warning この関数は、キャッシュが無ければキャッシュを自動作成します。
	 * @retval CGeometryBase*
	 */
	virtual CGeometryBase* GetShape();
	/**
	 * @brief 形状を返す
	 * 
	 * @warning この関数はconstですが、キャッシュが無ければキャッシュを自動作成します。
	 * @retval CGeometryBase*
	 */
	const CGeometryBase* GetShape() const;

	/**
	 * @brief 形状をセットする
	 * 
	 * INF_*等の形状を持たないオブジェクトでも仮想的な形状を
	 * 持つことができます。
	 * @param Shape [in] セットする形状（内部でコピーを作成します）
	 * @return sindyErrCode
	 */
	virtual errorcode::sindyErrCode SetShape( IGeometry* Shape );

	/**
	 * @brief 形状のコピーを取得する
	 *
	 * @note この関数は、変更前・変更後を含む全ての形状のコピーを取得することができます。
	 * 複数状態を指定した場合はUnionされた形状となります。
	 * @note IGeometry::get_ShapeCopy相当の機能を実現するには、引数に何も指定せずに使用してください。
	 * 
	 * @param emMode       [in,optional] コピーを取得する際のモード（ShapeModeを参照 デフォルト：emLatest）
	 * @return IGeometryPtr
	 */
	virtual IGeometryPtr GetShapeCopy( ShapeMode emMode = enumLatest ) const;

	/**
	 * @brief 現在格納されているフィーチャの存在矩形を返す
	 *
	 * @note この関数は、例えば編集後の再描画をする場合に変更前・変更後の形状を含んだ矩形が欲しい場合などに使用します。
	 * @warning Store()してしまうとキャッシュは全て破棄されますので、変更前の形状が取得できなくなります。
	 * 変更前の形状を取得する必要がある場合はStore()前にこの関数を呼ぶ必要があります。
	 *
	 * @param emMode       [in,optional] コピーを取得する際のモード（ShapeModeを参照 デフォルト：emLatest）
	 * @return IEnvelopePtr
	 */
	IEnvelopePtr GetEnvelope( ShapeMode emMode = enumLatest ) const;

	/**
	 * @brief <b>履歴をセットする</b>\n
	 * 履歴フィールドが存在しなくてもエラーを出しません。\n
	 * 現調反映用の作業目的の場合は現調フラグを付与します。\n
	 *
	 * @warn CRowBase::Storeを呼ぶとこの関数も一緒に呼ばれます。従って、CRowBase::Storeを
	 * 使用する場合はこの関数は呼ぶ必要はありません。
	 * 内部でSetAutoValueも呼びます。
	 *
	 * @note 内部でemCodeをUPDATE_Cに変換していますが、emCodeは属性更新且つ形状更新という状態を持たないため、
	 * その場合には形状更新となります。ただ、UPDATE_C更新時には属性更新且つ形状更新の状態にする
	 * 必要が有るため、bAttrUpdatedはUPDATE_Cを更新する際にemCodeが形状変更だった場合にのみ有効に働きます。
	 *
	 * @param emCode [in] 更新コード
	 * @param bAttrUpdated [in,optional] 属性更新されたかどうか（デフォルト：false）
	 *
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetHistory( schema::ipc_table::update_type::ECode emCode, bool bAttrUpdated = false );

	/**
	 * @breif 自動付与属性を付与する
	 *
	 * SiNDYルールにログインしていない場合は何もせずにsindyErr_NoErrを返します。
	 * サポートしている型は文字列及び整数型のみです。
	 *
	 * @retval sindyErr_NoErr 正常終了
	 * @retval sindyErr_AlgorithmFailed サポートされていない型に対して付与しようとした
	 */
	virtual errorcode::sindyErrCode SetAutoValue();

	/**
	 * @brief <b>コピーする</b>\n
	 * destにキャッシュがある場合は、削除されます。
	 * キャッシュがある場合は、キャッシュも一緒にコピーされます。
	 * @param dest              [in]　ペースト対象
	 * @param bIsCopyCache      [in] キャッシュをコピーするかどうか
	 * @param bIsCopyRowBase    [in] オブジェクトをコピーするかどうか
	 * @param bIsCopyFlag       [in] 作成削除フラグをコピーするかどうか
	 * @param bIsCopyHistory    [in] 編集履歴をコピーするかどうか
	 * @param bIsCopySchema     [in] スキーマ関係をコピーするかどうか
	 * @return sindyErrCode
	 */
	virtual errorcode::sindyErrCode CopyTo( CRowBase& dest, bool bIsCopyCache=true, bool bIsCopyRowBase=true, bool bIsCopyFlag=true, bool bIsCopyHistory = true, bool bIsCopySchema=true ) const;

	/**
	 * @brief <b>中身を交換する</b>\n
	 * @param dest				[in]	ペースト対象
	 * @param src				[in]	コピー対象
	 * @param bIsCopyCache		[in]	キャッシュをコピーするかどうか
	 * @param bIsCopyRowBase		[in]	オブジェクトをコピーするかどうか
	 * @param bIsCopyFlag		[in]	作成削除フラグをコピーするかどうか
	 * @param bIsCopyContainer	[in]	コンテナをコピーするかどうか
	 * @return sindyErrCode
	 */
	void swap(CRowBase& src, bool bIsCopyCache=true, bool bIsCopyRowBase=true, bool bIsCopyFlag=true/*, bool bIsCopyContainer=true*/ );

	/**
	 * @brief <b>与えられた形状の範囲で平行移動処理を行う</b>\n
	 * 編集ルールを考慮して移動処理を行います。\n
	 * 編集済みチェックは行いません。
	 * @param cGeom			[in]	クエリ形状
	 * @param pairMove		[in]	移動差分
	 * @param emMode		[in]	移動モード
	 * @param cErrInfos		[out]	エラー情報格納コンテナ
	 * @return sindyErrCode
	 */
	virtual errorcode::sindyErrCode Move( const CGeometryBase& cGeom, const std::pair<double,double>& pairMove, sindyeMoveMode emMode, CErrorObjectsBase& cErrObjects );

	/**
	 * @brief ある2つのレコードの属性値が同じかどうか
	 *
	 * @param src			[in]			比較するレコード（_IRowPtr or IFeaturePtr）
	 * @param cExclusions   [in,optional]   除外対象カラム名
	 *
	 * @note SiNDY-e 的に編集可能なフィールドのみチェックします
	 * 
	 * @retval true  同じ属性値を持つ
	 * @retval false 異なる属性値を持つ
	 */
	bool IsSameAttribute(const CRowBase& src, const CSimpleArray<CString>& cExclusions) const;

	/**
	 * @brief 作成フラグがたっているか
	 *
	 * @retval true  作成フラグがたっている
	 * @retval false 作成フラグがたっていない
	 */
	bool IsCreated() const { return m_bCreated; }
	/**
	 * @brief 削除フラグがたっているか
	 *
	 * @retval true  削除フラグがたっている
	 * @retval false 削除フラグがたっていない
	 */
	bool IsDeleted() const{ return m_bDeleted; }

	void Trace( bool bAttribute = true, bool bGeometry = true ) const;

	const VARIANT* GetCacheValue( long index ) const;
	VARIANT* GetCacheValue( long index );
	const VARIANT* GetValuePointer( long index, CComVariant& va ) const;
	VARIANT* GetValuePointer( long index, CComVariant& va );
protected:
	/**
	 * @brief <b>メンバ変数を初期化する</b>\n
	 * コンストラクタから呼ばれます。
	 * 継承先クラスでオーバーライドした場合、
	 * C++の規約により例えvirtualでもコンストラクタで
	 * CRowBase::Init()が必ず呼ばれることに注意してください。
	 */
	virtual void Init();
protected:
	CCacheAttribute*				m_pCache;		//!< 属性キャッシュ
	CGeometryBase*					m_pGeom;		//!< 形状キャッシュ
	mutable long					m_lOID;			//!< OBJECTIDキャッシュ
	bool							m_bCreated;		//!< 作成フラグ
	bool							m_bDeleted;		//!< 削除フラグ
};

bool operator==( int null, const CRowBase& obj );
bool operator!=( int null, const CRowBase& obj );
bool operator==( _IRow* lp, const CRowBase& obj );
inline bool operator==( int null, const CRowBase& obj )
{
	return obj.operator ==( null );
}
inline bool operator!=( int null, const CRowBase& obj )
{
	return obj.operator !=( null );
}
inline bool operator==( _IRow* lp, const CRowBase& obj )
{
	return obj.operator ==( lp );
}

#define CAST_ROWB boost::static_pointer_cast<CRowBase>

} // namepsace sindy
#pragma warning(pop)

#endif // _ROWBASE_H_
