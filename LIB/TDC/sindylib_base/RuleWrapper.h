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

#include "ComWrapper.h"
#include <boost/shared_ptr.hpp>

namespace sindy {

//class CAutoValue;
class CRowBase;

typedef std::list<std::pair<CString,CString>> CAutoValue; //!< 自動付与属性
class CRuleWrapper
{
public:
	CRuleWrapper(void){}
	virtual ~CRuleWrapper(void){}

	/**
	* @brief 抱えているCOMをクリアする
	*/
	void Clear()
	{
		m_ipRule = NULL;
		m_ipModify = NULL;
	}

	/**
	* @brief ルールオブジェクトにログイン情報がセットされているかどうか
	* @retval true ログイン済み
	* @retval false ログインしていない
	*/
	bool IsLogined() const;

	/**
	* @brief 編集可能かどうかを確認する
	* @param cRow [in] row（作成以外はpが存在する必要があります）
	* @param emEditType [in] 編集タイプ
	* @param strMsg [out] エラーメッセージ
	* @param check_modify [in,optional] ModifyCheckを実行するかどうか（デフォルト：false）
	* @param err2warn_modify [in,optional] ModifyCheckを警告扱いにするかどうか（デフォルト：false）
	* @return 編集可不可タイプ
	*/
	sindyeEditableType IsEditable( const CRowBase& cRow, sindyeEditType emEditType, CString& strMsg, bool check_modify = true, bool err2warn_modify = false  ) const;

	/**
	* @brief ロックエリアかどうか確認する
	* @param strUser [in] テーブルオーナー名
	* @param strTable [in] テーブル名
	* @param ipGeom [in] 確認対象形状
	* @retval true ロックエリア
	* @retval false 非ロックエリア
	*/
	bool IsLockArea( const CString& strOwner, const CString& strTable, IGeometryPtr& ipGeom ) const;

	/**
	* @brief 編集可能エリアかどうか確認する
	* @param strTable [in] テーブル名
	* @param ipGeom [in] 確認対象形状
	* @retval true 編集可能エリア
	* @retval false 編集不可能エリア
	*/
	bool IsEditableArea( const CString& strTable, IGeometryPtr& ipGeom ) const;

	/**
	* @brief 他に編集しているユーザがいないか確認する
	* @param cRow [in] row（pが存在する必要があります）
	* @param strMsg [out] エラーメッセージ
	* @retval true 他に編集しているユーザがいる
	* @retval false 誰も編集していない
	*/
	bool IsModified( const CRowBase& cRow, CString& strMsg ) const;

	/**
	* @brief 作業目的IDを取得する
	* @retval -1 ログインしていない
	* @retval -1以外 作業目的ID
	*/
	long GetWorkPurposeID() const;

	/**
	* @brief ログイン名を取得する
	* @retval ログイン名
	*/
	CString GetOperatorName() const;
	
	/**
	* @brief 新規作成時の自動付与属性を取得する
	* @note 作成可能レイヤに設定された自動作成属性の値を取得します
	* @param strTable [in] 対象のテーブル名
	* @return 自動付与属性ペアリスト（first:カラム名 second:値）
	*/
	const CAutoValue& GetCreatedAutoValue( const CString& strTable ) const;

	/**
	* @brief 自動付与属性を取得する
	* @note 「作業目的及びMXDファイルの編集」に設定された自動属性付与の値を
	* @return 自動付与属性ペアリスト（first:カラム名 second:値）
	*/
	const CAutoValue& GetAutoValue() const;

	/**
	* @brief ルールオブジェクトを取得する
	*/
	ISiNDYRule4Ptr GetRule() const;

	/**
	* @brief モディファイチェックオブジェクトを取得する
	*/
	ISiNDYModifyCheckPtr GetModify() const;
protected:
	mutable ISiNDYRule4Ptr m_ipRule; //!< ルールオブジェクト
	mutable ISiNDYModifyCheckPtr m_ipModify; //!< モディファイチェックオブジェクト
	mutable std::map<CString,CAutoValue> m_mapAutoValue; //!< 自動付与属性（作成時のみ）
	mutable CAutoValue m_cAutoValue; //!< 自動付与属性（編集全般用）
};

typedef boost::shared_ptr<CRuleWrapper> CSPRuleWrapper;

};
