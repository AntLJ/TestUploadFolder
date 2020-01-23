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
 * @file Row.h
 * @brief <b>CRowクラス定義ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _ROW_H_
#define _ROW_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../LIB/WinLib/ComWrapper.h"
#include "FieldMap.h"
#include "TableType.h"
#include "ClassUtil.h"
#include "NameString.h"
#include "Container.h"
#include "ListString.h"

#pragma warning(push)
#pragma warning(disable : 4275)
#pragma warning(disable : 4251)
namespace sindy {
class CErrorInfos;
class CErrorObject;
class CLogicRule;
class CCacheAttribute;
class CGeometry;
class CTable;

// 時間計測用
extern SINDYLIB_API double g_dTotalCacheTime;	//!< CRow::CreateCacheにかかる時間の総計

class CRow;
typedef boost::shared_ptr<CRow> CSPRow;

/**
 * @class CRow
 * @brief IRowPtrのラッパークラス
 *
 * 全てのアイテムクラスはこのクラスが基底となります
 */
class SINDYLIB_API CRow : public CComWrapper<_IRow>, public CContainer
{
public:
	CRow();
	virtual ~CRow()
	{
		DeleteCache();
	}

	CRow( const CRow& obj );
	CRow( _IRow* lp, sindyTableType::ECode emTableType = sindyTableType::unknown, bool bCreated = false, const CSPFieldMap& spFields = CSPFieldMap(), const CSPTableNameString& spNameString = CSPTableNameString() );

	CRow& operator=( const CRow& obj );
	CRow& operator=( _IRow* lp );

	operator _IRow*() const
	{
		return (_IRow*)p;
	}

	bool operator ==( const CRow& obj ) const
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
	bool operator <( const CRow& obj );

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
	 * @brief <b>ダミーCRowを実体化させる</b>
	 * CTable::CreateDummyRowで作成したダミーに対して実体をセットします。
	 * オブジェクトの作成タイミングをぎりぎりまで遅延させたい場合に有効です。
	 * CRow::pをセットするだけですのでキャッシュ等はクリアされません。
	 * テーブルタイプも確認しませんのでタイプの異なるものをセットしないでください。
	 */
	void Materialization( _IRow* lp );

	/**
	 * @brief <b>ダミーCRowを実体化させる</b>
	 * CTable::CreateDummyRowで作成したダミーに対して実体をセットします。
	 * オブジェクトの作成タイミングをぎりぎりまで遅延させたい場合に有効です。
	 * CRow::pをセットするだけですのでキャッシュ等はクリアされません。
	 * テーブルタイプも確認しませんのでタイプの異なるものをセットしないでください。
	 */
	errorcode::sindyErrCode Materialization( ITable* ipTable );

	/**
	 * @brief <b>ダミーCRowを実体化させる</b>
	 * CTable::CreateDummyRowで作成したダミーに対して実体をセットします。
	 * オブジェクトの作成タイミングをぎりぎりまで遅延させたい場合に有効です。
	 * CRow::pをセットするだけですのでキャッシュ等はクリアされません。
	 * テーブルタイプも確認しませんのでタイプの異なるものをセットしないでください。
	 */
	errorcode::sindyErrCode Materialization( ITable* ipTable, ITable* ipSubTable );

	/**
	 * @brief <b>フィーチャのコピーを作成する</b>\n
	 * 同じテーブル上に同じ属性を持つオブジェクトを作成します。\n
	 * キャッシュはコピーされません。\n
	 * フィールド対応表、テーブル名キャッシュはコピーされます（shared_ptr）。
	 * @warning 編集開始されていなければエラーログが出ます。
	 * @warning IRowがデタッチされている場合はエラーログが出ます。
	 * @retval CRow
	 */
	CRow Clone() const;

	/**
	 * @brief <b>フィーチャの変更を元に戻す</b>\n
	 * 作成フラグがついていたものはCTable::CreateDummyRow()相当の処理を
	 * 行います。\n
	 * 削除フラグはfalseになります。
	 * @param bRecursive [in] CContainerの中身も戻したい場合はtrue(デフォルト：false)
	 */
	virtual void Reset( bool bRecursive = false );

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
	 * 反映させるにはStore()する必要があります。
	 * @param bRecursive [in,optional] CContainerにも適用する場合はtrue（デフォルト：false）
	 */
	virtual void Delete( bool bRecursive = false );

	//@{ @name IRowインターフェース
	IFieldsPtr GetFields() const;
	bool HasOID() const;
    long GetOID() const;
	virtual errorcode::sindyErrCode Store( CErrorObjects& cErrs );
	ITablePtr GetTable() const;
	errorcode::sindyErrCode GetValue( long lIndex, VARIANT& vaValue ) const;
	virtual errorcode::sindyErrCode SetValue( long lIndex, const VARIANT& vaValue );
#if defined(SINDYLIB_EXPORTS) || defined(USE_SINDYLIB_DLL)
	/**
	 * @brief <b>CComVariantを返すGetValue</b>\n
	 * この関数はスタティックリンク版のみの提供です
	 * @param lIndex	[in]	フィールドインデックス番号
	 * @return CComVariant
	 */
	CComVariant GetValue( long lIndex ) const
	{
		CComVariant vaValue;
		GetValue( lIndex, vaValue );
		return vaValue;
	}
#endif // defined(SINDYLIB_EXPORTS) || defined(USE_SINDYLIB_DLL)

	//@}

	/**
	 * @brief <b>ワークスペースを取得する</b>\n
	 * @return IWorkspacePtr
	 */
	IWorkspacePtr GetWorkspace() const;

	/**
	 * @brief <b>テーブルタイプを取得する</b>\n
	 * @return sindyTableType::ECode
	 */
	sindyTableType::ECode GetTableType() const;

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
	CSPFieldMap GetFieldMap() const;

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
	CSPTableNameString GetNameString() const;

	/**
	 * @brief <b>テーブル名を返す</b>\n
	 * @see CTableNameString::GetTableName()
	 * @retval LPCTSTR
	 */
	LPCTSTR GetTableName() const;

	bool IsEqualObject(_IRow* ipRow ) const;
	bool IsEqualObject( const CRow& cRow ) const;

	/**
	 * @brief <b>フィールド名でフィールド値を取得する</b>\n
	 * フィールド対応表が無ければ作成してから取得します。
	 * @param lpcszFieldName	[in]	フィールド名
	 * @param vaValue			[out]	フィールド値
	 *
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode GetValueByFieldName( LPCTSTR lpcszFieldName, VARIANT& vaValue ) const;

#if defined(SINDYLIB_EXPORTS) || defined(USE_SINDYLIB_DLL)
	/**
	 * @brief <b>CComVariantを返すGetValueByFieldName</b>\n
	 * この関数はスタティックリンク版のみの提供です
	 * @param lpcszFieldName	[in]	フィールド名
	 * @return CComVariant
	 */
	CComVariant getValueByFieldName( LPCTSTR lpcszFieldName ) const
	{
		CComVariant vaValue;
		GetValueByFieldName( lpcszFieldName, vaValue );
		return vaValue;
	}
#endif // defined(SINDYLIB_EXPORTS) || defined(USE_SINDYLIB_DLL)

	/**
	 * @brief <b>フィールド名でフィールド値をセットする</b>\n
	 * フィールド対応表が無ければ作成してからセットします。
	 * @param lpcszFieldName	[in]	フィールド名
	 * @param vaValue			[out]	フィールド値
	 *
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetValueByFieldName( LPCTSTR lpcszFieldName, const VARIANT& vaValue );

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

	/**
	 * @brief <b>整合性をチェックする</b>\n
	 * この関数では基本的なチェックしか行いません。
	 * 必要なチェックは、継承先のクラスで実装してください。
	 * @param cRule			[in]	整合性チェック用ルール
	 * @param cErrs			[out]	エラー情報格納用コンテナ
	 * @return sindyErrCode
	 */
	virtual errorcode::sindyErrCode CheckLogic( const CLogicRule& cRule, CErrorObjects& cErrs ) const;

	/**
	 * @brief <b>コンフリクト回避のための既編集チェックを行う</b>\n
	 * @param cErrs		[out]	エラー情報格納用コンテナ
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckModify( CErrorObjects& cErrs ) const;

	/**
	 * @brief <b>編集可能かどうかをチェックする</b>\n
	 * 編集ルールに基づいて編集可能かどうかをチェックします。\n
	 * 編集済みチェックは行いません。
	 * @param emType		[in]	編集タイプ
	 * @param cErrInfos		[out]	エラー格納用コンテナ
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckEditable( sindyeEditType emType, CErrorInfos& cErrInfos ) const;

	/**
	 * @brief <b>インデックス番号でbool値を取得する</b>\n
	 * フィールドタイプがlongでも、使用方法がboolの場合はこれを使用します。
	 * @param index				[in]			フィールドインデックス番号
	 * @param bDefaultValue		[in]			NULL、またはエラーの場合に返す値
	 * @return bool
	 */
	bool GetBoolValue( long index, bool bDefaultValue ) const;

	/**
	 * @brief インデックス番号でbool値をセットする\n
	 * フィールドタイプがlongでも、使用方法がboolの場合はこれを使用します。
	 * @param index				[in]			フィールドインデックス番号
	 * @param bNewValue			[in]			セットする値
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetBoolValue( long index, bool bNewValue );

	/**
	 * @brief <b>インデックス番号でlong値を取得する</b>\n
	 * @param index				[in]			フィールドインデックス番号
	 * @param lDefaultValue		[in]			NULL、又はエラーの場合に返す値
	 * @return long
	 */
	long GetLongValue( long index, long lDefaultValue ) const;

	/**
	 * @brief インデックス番号でlong値をセットする
	 * @param index				[in]			フィールドインデックス番号
	 * @param lNewValue			[in]			セットする値
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetLongValue( long index, long lNewValue );

	/**
	 * @brief インデックス番号でdouble値を取得する
	 * @param index				[in]			フィールドインデックス番号
	 * @param dDefaultValue		[in]			NULL、またはエラーの場合に返す値
	 * @return double
	 */
	double GetDoubleValue( long index, const double& dDefaultValue ) const;

	/**
	 * @brief インデックス番号でdouble値をセットする
	 * @param index				[in]			フィールドインデックス番号
	 * @param dNewValue			[in]			セットする値
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetDoubleValue( long index, const double& dNewValue );
	/**
	 * @brief インデックス番号でString値を取得する\n
	 * 値がNULL、又は取得に失敗した場合はNULLが返ります。
	 * @param index				[in]			フィールドインデックス番号
	 * @param nLen				[in]			lpszValueの収納可能文字列数
	 * @param lpszValue			[out]			取得文字列を格納するバッファ
	 * @retval NULL エラーの場合
	 * @retval 空文字列 値がNULLの場合
	 */
	LPCTSTR GetStringValue( long index, int nLen, LPTSTR lpszValue ) const;

	/**
	 * @brief インデックス番号で文字列値をセットする
	 * @param index				[in]			フィールドインデックス番号
	 * @param lpcszNewValue		[in]			セットする値
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetStringValue( long index, LPCTSTR lpcszNewValue );

	/**
	 * @brief <b>フィールド名でbool値を取得する</b>\n
	 * フィールドタイプがlongでも、使用方法がboolの場合はこれを使用します。
	 * @param lpcszFieldName	[in]			フィールド名
	 * @param bDefaultValue		[in]			NULL、またはエラーの場合に返す値
	 * @return bool
	 */
	bool GetBoolValueByFieldName( LPCTSTR lpcszFieldName, bool bDefaultValue ) const;

	/**
	 * @brief フィールド名でbool値をセットする\n
	 * フィールドタイプがlongでも、使用方法がboolの場合はこれを使用します。
	 * @param lpcszFieldName	[in]			フィールド名
	 * @param bNewValue			[in]			セットする値
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetBoolValueByFieldName( LPCTSTR lpcszFieldName, bool bNewValue );

	/**
	 * @brief フィールド名でlong値を取得する
	 * @param lpcszFieldName	[in]			フィールド名
	 * @param lDefaultValue		[in]			NULL、またはエラーの場合に返す値
	 * @return long
	 */
	long GetLongValueByFieldName( LPCTSTR lpcszFieldName, long lDefaultValue ) const;

	/**
	 * @brief フィールド名でlong値をセットする
	 * @param lpcszFieldName	[in]			フィールド名
	 * @param lNewValue			[in]			セットする値
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetLongValueByFieldName( LPCTSTR lpcszFieldName, long lNewValue );

	/**
	 * @brief フィールド名でdouble値を取得する
	 * @param lpcszFieldName	[in]			フィールド名
	 * @param dDefaultValue		[in]			NULL、またはエラーの場合に返す値
	 * @return double
	 */
	double GetDoubleValueByFieldName( LPCTSTR lpcszFieldName, const double& dDefaultValue ) const;

	/**
	 * @brief フィールド名でdouble値をセットする
	 * @param lpcszFieldName	[in]			フィールド名
	 * @param dNewValue			[in]			セットする値
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetDoubleValueByFieldName( LPCTSTR lpcszFieldName, const double& dNewValue );
	/**
	 * @brief フィールド名でString値を取得する\n
	 * 値がNULL、又は取得に失敗した場合はNULLが返ります。\n
	 * @param lpcszFieldName	[in]			フィールド名
	 * @param nLen				[in]			lpszValueの収納可能文字列数
	 * @param lpszValue			[out]			取得文字列を格納するバッファ
	 * @retval NULL エラーの場合又は値がNULLの場合
	 * @retval NULL以外 フィールド値
	 */
	LPCTSTR GetStringValueByFieldName( LPCTSTR lpcszFieldName, int nLen, LPTSTR lpszValue ) const;

	/**
	 * @brief <b>フィールド名でString値のサイズを取得する</b>\n
	 * 格納されている文字数を取得したい場合は、GetStringValueNumByFieldNameを使用してください。\n
	 * 終端のNULLはカウントに含めません。
	 * @param lpcszFieldName	[in]			フィールド名
	 * @retval -1 エラー
	 * @retval -1以外 サイズ（バッファ長）
	 */
	int GetStringValueLengthByFieldName( LPCTSTR lpcszFieldName ) const;

	/**
	 * @brief <b>フィールド名でString値の文字数を取得する</b>\n
	 * バッファ長を取得したい場合は、GetStringValueLengthByFieldNameを使用してください。\n
	 * @param lpcszFieldName	[in]			フィールド名
	 * @retval -1 エラー
	 * @retval -1以外 文字数
	 */
	int GetStringValueNumByFieldName( LPCTSTR lpcszFieldName ) const;

	/**
	 * @brief フィールド名で文字列値をセットする
	 * @param lpcszFieldName	[in]			フィールド名
	 * @param lpcszNewValue		[in]			セットする値
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetStringValueByFieldName( LPCTSTR lpcszFieldName, LPCTSTR lpcszNewValue );

	/**
	 * @brief <b>属性・形状などのキャッシュを作成する</b>\n
	 * フィールド対応表、テーブル名キャッシュオブジェクトが無ければ作成してから作成しますが、
	 * その場合ほかのオブジェクトと共有できないためリソースを多く必要とします。
	 * リソースがどうでもよい場合、及びオブジェクトが少ない場合を除いて極力フィールド対応表
	 * とテーブル名キャッシュオブジェクトを渡すようにしてください。\n
	 * 既にキャッシュが存在する場合はキャッシュを作成しません。また、
	 * bDetatchをtrueにすると、キャッシュ作成後に
	 * _IRow*を切り離します。
	 * @param bDetach			[in,optional]	処理後_IRow*を切り離す場合はtrue
	 * @param ipFields			[in,optional]	フィールド対応表オブジェクト(shared_ptr)
	 * @param spNameString		[in,optional]	テーブル名等管理オブジェクト(shared_ptr)
	 * @return sindyErrCode;
	 */
	virtual errorcode::sindyErrCode CreateCache( bool bDetatch = false, const CSPFieldMap& spFields = CSPFieldMap(), const CSPTableNameString& spNameString = CSPTableNameString() );

	/**
	 * @brief <b>キャッシュを削除する</b>\n
	 * デストラクタから呼ばれます。
	 * 継承先クラスでオーバーライドした場合、
	 * C++の規約により例えvirtualでもデストラクタで
	 * CRow::DeleteCache()が必ず呼ばれることに注意してください。
	 */
	virtual void DeleteCache( bool bDeleteContainerCache = false );

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
	 * @retval CGeometry*
	 */
	virtual CGeometry* GetShape();
	/**
	 * @brief 形状を返す
	 * 
	 * @warning この関数はconstですが、キャッシュが無ければキャッシュを自動作成します。
	 * @retval CGeometry*
	 */
	const CGeometry* GetShape() const;

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
	 * @brief <b>現在格納されているフィーチャの形状のコピーを返す</b>\n
	 * @param bLatest               	[in,optional] 最新の形状が欲しい場合はtrue（デフォルト：true）
	 * @param bOriginal             	[in,optional] 変更前の形状が欲しい場合はtrue（デフォルト：false）
	 * @param bFeatureChangedOriginal	[in,optional] IFeatureChanges::get_OriginalShapeの形状が欲しい場合にはtrue（デフォルト：false）
	 * @param bDiff						[in,optional] 複数状態を指定した時に、Unionした形状ではなく差分形状にするかどうか
	 * @param lParam					[in,optional] 任意引数（CRow::GetShapeCopyでは使用しません デフォルト：-1）\n継承先で使用します。
	 * @note この関数は、変更前・変更後を含む全ての形状のコピーを取得することができます。
	 * 複数状態を指定した場合はUnionされた形状となります。
	 * @note IGeometry::get_ShapeCopy相当の機能を実現するには、引数に何も指定せずに使用してください。
	 */
	virtual IGeometryPtr GetShapeCopy( bool bLatest = true, bool bOriginal = false, bool bFeatureChangedOriginal = false, bool bDiff = false, long lParam = -1 ) const;

	/**
	 * @brief <b>履歴をセットする</b>\n
	 * 履歴フィールドが存在しなくてもエラーを出しません。\n
	 * 現調反映用の作業目的の場合は現調フラグを付与します。\n
	 *
	 * @warn CRow::Storeを呼ぶとこの関数も一緒に呼ばれます。従って、CRow::Storeを
	 * 使用する場合はこの関数は呼ぶ必要はありません。
	 *
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetHistory( schema::ipc_table::update_type::ECode emCode );

	/**
	 * @brief <b>コピーする</b>\n
	 * destにキャッシュがある場合は、削除されます。
	 * キャッシュがある場合は、キャッシュも一緒にコピーされます。
	 * @param dest				[in]	ペースト対象
	 * @param bIsCopyCache		[in]	キャッシュをコピーするかどうか
	 * @param bIsCopyRow		[in]	オブジェクトをコピーするかどうか
	 * @param bIsCopyFlag		[in]	作成削除フラグをコピーするかどうか
	 * @param bIsCopyContainer	[in]	コンテナをコピーするかどうか
	 * @return sindyErrCode
	 */
	virtual errorcode::sindyErrCode CopyTo( CRow& dest, bool bIsCopyCache=true, bool bIsCopyRow=true, bool bIsCopyFlag=true, bool bIsCopyContainer=true ) const;

	/**
	 * @brief <b>中身を交換する</b>\n
	 * @param dest				[in]	ペースト対象
	 * @param src				[in]	コピー対象
	 * @param bIsCopyCache		[in]	キャッシュをコピーするかどうか
	 * @param bIsCopyRow		[in]	オブジェクトをコピーするかどうか
	 * @param bIsCopyFlag		[in]	作成削除フラグをコピーするかどうか
	 * @param bIsCopyContainer	[in]	コンテナをコピーするかどうか
	 * @return sindyErrCode
	 */
	void swap(CRow& src, bool bIsCopyCache=true, bool bIsCopyRow=true, bool bIsCopyFlag=true, bool bIsCopyContainer=true );

	/**
	 * @brief IRow*を新しいものに挿げ替える
	 * @param pTargetTable [in,optional] 指定しなければ同じテーブルにクローンを作成（デフォルト：NULL）
	 * @param bRecursive [in,optional] 階層的に実行するのであればtrue（デフォルト：false）
	 * @warning ipTargetTableは同じ構造のテーブルである必要があります（関数自体は正常終了します）。
	 * @note Detach済み且つipTargetTable=NULLの場合はエラーとなります。
	 */
	errorcode::sindyErrCode ReplaceNewRow( const CTable* pTargetTable = NULL, bool bRecursive = false );

	/**
	 * @brief ある2つのレコードの属性値が同じかどうか
	 *
	 * @param src			[in]			比較するレコード（_IRowPtr or IFeaturePtr）
	 *
	 * @note SiNDY-e 的に編集可能なフィールドのみチェックします
	 * 
	 * @retval true  同じ属性値を持つ
	 * @retval false 異なる属性値を持つ
	 */
	bool IsSameAttribute(const CRow& src, const CListString& cExclusions=CListString()) const;

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
protected:
	/**
	 * @brief <b>メンバ変数を初期化する</b>\n
	 * コンストラクタから呼ばれます。
	 * 継承先クラスでオーバーライドした場合、
	 * C++の規約により例えvirtualでもコンストラクタで
	 * CRow::Init()が必ず呼ばれることに注意してください。
	 */
	virtual void Init();

	/** 
	* @brief キャッシュを反映させる
	*
	* CRow::Store()からの分離です。
	* キャッシュをpに反映させます。
	* pが存在しない場合は何もしません。
	* 反映後もキャッシュは削除しません。
	*
	* @param emUpdate [out] 更新タイプ
	* @param cErrs [out] エラーオブジェクトコンテナ
	* @param pErr [out] エラーオブジェクト
	*/
	void UpdateCache( schema::ipc_table::update_type::ECode& emUpdate, CErrorObjects& cErrs, CErrorObject* pErr );
protected:
	mutable sindyTableType::ECode	m_emTableType;	//!< 格納されているフィーチャのテーブル識別子
	mutable CSPTableNameString		m_spNameString;	//!< テーブル名キャッシュ用（shared ptr）
	mutable CSPFieldMap				m_spFields;		//!< フィールド対応表（shared ptr）

	CCacheAttribute*				m_pCache;		//!< 属性キャッシュ
	CGeometry*						m_pGeom;		//!< 形状キャッシュ
	mutable long					m_lOID;			//!< OBJECTIDキャッシュ
	bool							m_bCreated;		//!< 作成フラグ
	bool							m_bDeleted;		//!< 削除フラグ
};

bool operator==( int null, const CRow& obj );
bool operator!=( int null, const CRow& obj );
inline bool operator==( int null, const CRow& obj )
{
	return obj.operator ==( null );
}
inline bool operator!=( int null, const CRow& obj )
{
	return obj.operator !=( null );
}

} // namepsace sindy
#pragma warning(pop)

#endif // _ROW_H_
