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

/*! \file FieldDef.h
	\brief CFieldDef クラスのインターフェース
	\author INCREMENTP CORP. R&D技術センター第４開発グループ 古川貴宏
	\date 2002-03-05
*/

#if !defined(AFX_FIELDDEF_H__6B343E74_CE5C_4B23_B417_E5BC6DBA1D53__INCLUDED_)
#define AFX_FIELDDEF_H__6B343E74_CE5C_4B23_B417_E5BC6DBA1D53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CArcHelper;


struct CODEVALUE_DOMAIN {
public:
	long idx;					//!< コード値をコンボボックスへ格納したときのインデックス
	CComVariant code;			//!< コード値
	CString code_discription;	//!< コード値の説明(コンボボックスへの標示用)

	CODEVALUE_DOMAIN() : idx(-1) {}
	void setIdx(long index) { idx = index; }

	bool operator<(const CODEVALUE_DOMAIN& domain) const {
		return idx < domain.idx;
	}
	bool operator==(const CODEVALUE_DOMAIN& domain) const {
		return idx == domain.idx;
	}
};

namespace virtual_field{
	extern const _TCHAR kShapeZ[];
}

/**
 * \brief  固定値ドメイン情報
 */
typedef std::set<CODEVALUE_DOMAIN> DOMAINMAP;

/**
 * \brief レンジドメイン格納用構造体
 */
struct RANGEDOMAIN {
	DOUBLE dMax;		//!< レンジドメインの最大値
	DOUBLE dMin;		//!< レンジドメインの最小値
	BOOL   bExist;		//!< 存在判定フラグ
	BOOL   bInited;		//!< 初期化判定フラグ
};

/*! \class CFieldDef
 	\brief IField のラッパークラス

	このクラスは IField と一対一の関係になります。
	ただし、使用しやすいようにいくつかのメンバ変数を追加してあります。
	また、例えば固定値ドメインなどデータベースに負荷がかかると思われるものについては一度
	読み込んだら破棄されるまでクラス内で情報を保持します。

	\b Example:

	CFieldDef fieldDef;

	CFieldDefオブジェクトを生成します。
	
	fieldDef.SetFieldDef( NODE_TABLE_NAME, _T("KANJI_NAME") );	
	
	テーブル名とフィールド名がそれぞれm_lpszTableNameとm_lpszFieldNameに設定されます。

	fieldDef.SetRowBuffer( 3, ipField, ipRowBuffer );		
	
	フィールドのインデックス番号、IFieldインターフェースオブジェクト、IFeature（又はIRow）インターフェースオブジェクトを渡します。
	\note 3番目の引数はIRowBufferインターフェースオブジェクトになっていますが、
	\note IFeatureもIRowもIRowBufferインターフェースを持っているのでそのまま渡しても大丈夫です。
*/
class CFieldDef
{
public:
	CFieldDef();
	virtual ~CFieldDef();

public:
	void SetArcHelper( IApplication* ipApp ){m_cArcHelper.Init( (IApplicationPtr)ipApp );};
	void SetFieldDef( LPCTSTR lpszTableName, LPCTSTR lpszFieldName );
	CFieldDef* GetFieldDef( LPCTSTR lpszTableName, LPCTSTR lpszFieldName );
	void SetRowBuffer( LONG lFieldIndex, IFieldPtr ipField, IRowBufferPtr ipRowBuffer );
	BOOL Store();
	void Reset();
	void Clear();
	DOMAINMAP* GetDomain();					//!< 固定値ドメインを返す
	RANGEDOMAIN* GetRangeDomain();			//!< レンジドメインを返す
	CComVariant GetCodeDomainValue(long idx);	//!< コンボボックスの値を返す
	VARTYPE GetCodedDomainType();			//!< 文字列型のコード値ドメインの型を返す
	BOOL IsEditable();						//!< そのフィールドが編集可能かどうか
	BOOL CheckValue( const CComVariant& vaValue, HWND hWnd );
	BOOL SetNewValue( CComVariant vaNewValue, HWND hWnd );
	void dump();
	CString value2string( BOOL bIsInitValue = FALSE );
public:
	CArcHelper		m_cArcHelper;			//!< CArcHelper クラスのポインタ
	CString			m_strFieldName;		//!< 対応するフィールド名
	CString			m_strFieldAliasName;	//!< 対応するフィールドのエイリアス名
	CString			m_strTableName;		//!< 対応するテーブル名（関連テーブルの場合のみ）
	BOOL			m_bIsUnset;				//!< 値がセットされたかどうかのフラグ（フィールドが存在するかどうか）
	BOOL			m_bIsNullable;
	BOOL			m_bIsInited;
	BOOL			m_bChanged;
	CComVariant		m_vaValue;				//!< 現在の値（変更される場合はこの値も一緒に変更される）
	CComVariant		m_vaInitValue;			//!< 初期状態の値（変更判定及び値を元に戻す場合に使用）
	LONG			m_lFieldIndex;			//!< フィールドのインデックス番号（CRowDef::Delate()、CRowDef::Create() で使用するのでパブリック）
	IFieldPtr		m_ipField;				//!< CAttrDlg::CreateLinkTimeRegRow() でセットする必要があるのでパブリック
private:
	BOOL			m_bIsNotDomain;			//!< ドメインがあるかどうかのフラグ（調査済み且つない場合はTRUE、それ以外はFALSE）
	BOOL			m_bIsUnsetOld;			//!< m_bIsUnset の保存用（新規フィールド追加の場合は m_bIsUnset != m_bIsUnsetOld）
	INT				m_nRelType;				//!< = 2 の時は Node_Queue を使用するので Store 時に特別処理必要
	DOMAINMAP		m_mapDomain;			//!< コード値ドメイン情報
	RANGEDOMAIN		m_strctRangeDomain;		//!< レンジドメイン
	IRowBufferPtr	m_ipRowBuffer;			//!< 対応する IRowBuffer
};

#endif // !defined(AFX_FIELDDEF_H__6B343E74_CE5C_4B23_B417_E5BC6DBA1D53__INCLUDED_)
