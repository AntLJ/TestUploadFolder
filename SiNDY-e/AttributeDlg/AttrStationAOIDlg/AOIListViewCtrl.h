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

#include <windows.h>
#include <atlbase.h>
#include <atlcom.h>
#include <atlwin.h>
#define _ATL_TMP_NO_CSTRING     // ATLとWTLでCStringがかぶるため
#include <atlapp.h>             // WTLを使用
#include <atlmisc.h>            // CPoint CSize CRect
#include <atlctrls.h>
#include <atlstr.h>             // CString
#include <atlcrack.h>           // メッセージ系

#include <boost/utility.hpp>
#include <list>
#include <map>

#include "CommonData.h"

/**
* @brief 駅AOIポリゴンの処理に特化したListViewCtrl
* @note  リストビューの表示だけでデータを管理するとフィーチャ切り替えの際に不都合が出たため、
* @note  一度表示したデータは内部的に保持し、再表示できるようにした
* @note  汎用的に作ろうとして挫折
* @note  構想段階では、リストビュー上のデータとlist<list<CString>>を等価に扱えるようにしようとしていた
* @note  ([][]のように参照できるので、データの受け渡しはvectorでした方が良かったかもしれない)
*/
class CAOIListViewCtrl final: boost::noncopyable
{
public:
	/**
	* @brief 各要素にカラム名を格納するデータ型
	*/
	typedef std::list<CString> COLUMN_NAMES;

	/**
	* @brief 初期化処理
	* @warning 処理の開始時に 一度だけ必ず呼び出すこと
	* @param[in] hwnd    ウィンドウハンドル
	* @param[in] id      このインスタンスで管理するリストビューのID
	* @param[in] columns 各列のカラム名を格納したリスト
	* @param[in] option  リストビューに設定するオプション
	*/
	void Init( HWND hwnd, UINT id, const COLUMN_NAMES& columns, DWORD option );

	/**
	* @brief リストビューの表示をクリアする
	* @note 内部に保持しているデータは消去されない
	*/
	void Clear();

	/**
	* @brief リストビュー上の表示を内部で保持しているデータに合わせる
	* @warning 編集対象となるフィーチャーが切り替わったら必ず呼び出すこと
	* @param[in] lOID  編集対象フィーチャーのOBJECTID
	* @retval true  成功
	* @retval false 内部的に保持しているデータが空であったため、表示できなかった
	*/
	bool Review( long lOID );

	/**
	* @brief リストビューの表示をクリアし、内部で保持しているデータも全て消す
	*/
	void DeleteAll();

	/**
	* @brief リストビューの表示をクリアし、内部で保持されているデータを消す
	* @param[in] lOID データを消すオブジェクトID
	*/
	void Delete( long lOID );

	/**
	* @brief 指定行のチェックボックスがチェックされているか
	* @param[in] lineCount チェックされているか判定する行番号
	* @retval true  チェックされている
	* @retval false チェックされていない
	*/
	bool IsChecked( int lineCount ) const;

	/**
	* @brief リストビューで表示されている行の数を取得
	* @return 行数
	*/
	int GetLineCount() const;

	/**
	* @brief リストビューで表示されているデータがあるか判定
	* @retval true  表示されているデータはない
	* @retval false 表示されているデータがある
	*/
	bool IsEmpty() const;

	/*
	* @brief リストビュー上の指定カラムに指定の値が存在するか否か
	* @param[in]   value      指定の値
	* @param[in]   columnName 値の存在を確認するカラム名
	* @retval true  値が存在する
	* @retval false 値は存在しない
	*/
	bool IsExist( const CString& value, const CString& columnName ) const;

	/**
	* @brief リストビューに一行分のデータを追加する
	* @note  追加されたデータを内部的保持する
	* @param[in] lList 追加するデータのリスト（listのsize()がカラム数）
	*/
	void AddItem( const AOI_LINE& lList );

	/*
	* @brief 指定カラムのリストビュー上でのインデックス番号を取得
	* @param[in]   columnName インデックス番号を取得するカラム名
	* @retval 0以上 指定カラムのリストビュー上でのインデックス番号
	* @retval -1    指定カラム名を持つカラムが存在しない
	*/
	int GetColumnIndex( const CString& columnName ) const;

	/**
	* @brief 指定行の指定カラムの値を取得する
	* @param[in] lineIndex  値を取得する行のインデックス番号
	* @param[in] columnName 指定カラム名
	* @return    strValue   指定行の指定カラムの値
	*/
	CString GetValue( unsigned int lineIndex , const CString& columnName) const;

	/**
	* @brief 各行の指定カラムの値をリスト化したものを返す
	* @param[in] columnName  指定カラム名
	* @param[in] isCheckOnly チェックボックスがチェックされているものだけを返すか否か
	*/
	AOI_COLUMN MakeTargetColumnList(const CString& columnName, bool isCheckOnly = false) const;

	/**
	* @brief 指定行のチェックを変更する
	* @param[in] lineIndex 指定行
	* @param[in] checkOn   TRUEならチェックを入れる、FALSEならチェックを入れない
	*/
	void ChangeChecked( unsigned int lineIndex, BOOL checkOn );

	/**
	* @brief 指定カラムの値がlist内に存在するなら、その行にチェックを入れる
	* @param[in] columnName 指定カラム名
	* @param[in] lList      チェックを入れる値のリスト
	*/
	void SetCheckedOn( const CString& columnName, const AOI_COLUMN& lList );

	/**
	* @brief 指定カラムの値がlist内に存在する行を削除する
	* @note  内部データも削除する
	* @param[in] columnName 指定カラム名
	* @param[in] lList      削除するデータのリスト
	* @retval true  削除が最低一回、行われた
	* @retval false 一度も削除が行われなかった
	*/
	bool DeleteItem( const CString& columnName, const AOI_COLUMN& lList );

	/*
	* @brief リストビュー上の指定カラムにlist内の値が存在するか否か
	* @param[in]   values     存在を判定する値を格納したリスト
	* @param[in]   columnName 値の存在を確認するカラム名
	* @retval true  リスト中のいずれかの値が存在する
	* @retval false リスト中のいずれの値も存在しない
	*/
	bool IsExist( const AOI_COLUMN& values, const CString& columnName ) const;

	/*
	* @brief リストビュー上の指定カラムにlist内の値が存在しない行を取得
	* @param[in]   values          このリスト内に存在する値をもつ行は取得しない
	* @param[in]   eraseNonChecked チェックボックスがチェックされていないものを除くならtrueを渡す
	* @param[in]   columnName      値の存在を確認するカラム名
	* @return retList  リスト中のいずれの値も存在しない行のリスト
	*/
	AOI_MATRIX GetNonExistValue( const AOI_COLUMN& values, bool eraseNonChecked , const CString& columnName ) const;

	/// OBJECTIDに関連付くリストデータを取得
	std::map< long, AOI_MATRIX >& getRelationData() { return m_OID2Values; }

private:
	/**
	* @brief リストビューに一行分のデータを追加する
	* @note  追加されたデータを内部的に保持することはない
	* @param[in] lList 追加するデータのリスト（listのsize()がカラム数）
	*/
	void addData( const AOI_LINE& lList );

	CListViewCtrl m_lvCtrl; //!< このインスタンスで管理するリストビュー
	long m_OID;    //!< m_ViewIDに対応付くOBJECTID
	std::map< long, AOI_MATRIX > m_OID2Values; //!< IDに対して保持されている値を格納
};
