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

#include "Arguments.h"
#include "LayerInfoManager.h"

/**
 * @brief エラーログヘッダリスト
 */
const std::vector<CString> ERRLOG_HEADER_LIST = boost::assign::list_of
		( _T("FLAG") )
		( _T("LAYER") )
		( _T("OBJECTID") )
		( _T("X") )
		( _T("Y") )
		( _T("エラーレベル") )
		( _T("エラーコード") )
		( _T("エラーメッセージ") )
		( _T("情報1") )
		( _T("情報2") )
		( _T("情報3") )
		;

/**
 * @brief Conversion between VARIANT_BOOL and boolean
 */
struct VariantBool
{
	VariantBool() : vb(VARIANT_FALSE) {}
	explicit VariantBool(bool b) : vb(b ? VARIANT_TRUE : VARIANT_FALSE) {}
	explicit VariantBool(VARIANT_BOOL b) : vb(b) {}
	operator VARIANT_BOOL() const { return vb; }
	operator bool() const { return (vb == VARIANT_FALSE) ? false : true; }
	VARIANT_BOOL* operator &() {return &vb;}
	VARIANT_BOOL vb;
};

/**
 * @brief   データ生成クラス（基底クラス）
 */
class BaseDataCreator
{
public:

	/** 
	 * @ brief States of session
	 */
	enum State : char
	{
		S_ERROR_MIXED = -1,	///< Mixed with versioned and non versioned
		S_ERROR = 0,		///< Open failed
		S_VERSIONED,		///< Versioned
		S_NON_VERSIONED,	///< Non versioned
	};

	/**
	 * @brief   コンストラクタ
	 * @param   arg [in]  引数
	 */
	BaseDataCreator(const Arguments& arg);

	/**
	 * @brief   デストラクタ
	 */
	virtual ~BaseDataCreator() {}

	/**
	 * @brief   初期化（共通）
	 */
	bool initBase();

	/**
	 * @brief   初期化（派生クラスで実装）
	 */
	virtual bool init() = 0;

	/**
	 * @brief   データ生成（派生クラスで実装）
	 */
	virtual bool create() = 0;

protected:

	/**
	 * @brief   属性検索
	 * @param   layerManager [in]  レイヤ情報管理
	 * @param   whereClause [in]  検索条件
	 * @return  FeatureCursor
	 */
	IFeatureCursorPtr searchByWhereClause(LayerInfoManager& layerManager, const CString& whereClause);

	/**
	 * @brief   フィールド値設定
	 * @param   featureBuf [in/out]  対象FeatureBuffer
	 * @param   fieldIdx [in]  フィールドインデックス
	 * @param   value [in]  設定値
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool putValue(IFeatureBufferPtr& featureBuf, long fieldIdx, const CString& value);

	/**
	 * @brief   フィールド値設定
	 * @param   featureBuf [in/out]  対象FeatureBuffer
	 * @param   fieldIdx [in]  フィールドインデックス
	 * @param   value [in]  設定値
	 * @param   isNull [in]  値をnullにする場合はtrueを設定（デフォルトはfalse）
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool putValue(IFeatureBufferPtr& featureBuf, long fieldIdx, int value, bool isNull = false);

	/**
	 * @brief   フィールド値設定
	 * @param   featureBuf [in/out]  対象FeatureBuffer
	 * @param   fieldIdx [in]  フィールドインデックス
	 * @param   value [in]  設定値
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool putValue(IFeatureBufferPtr& featureBuf, long fieldIdx, CComVariant& value);

	/**
	 * @brief   フィールド値取得
	 * @param   featureBuf [in/out]  対象Feature
	 * @param   fieldIdx [in]  フィールドインデックス
	 * @param   value [out]  取得値
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool getValue(const IFeaturePtr& feature, long fieldIdx, CComVariant& value);

	/**
	 * @brief   フィールド値取得
	 * @param   row [in/out]  対象Row
	 * @param   fieldIdx [in]  フィールドインデックス
	 * @param   value [out]  取得値
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool getValue(const _IRowPtr& row, long fieldIdx, CComVariant& value);

	/**
	 * @brief   編集開始処理
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool startEdit();

	/**
	 * @brief   編集終了処理
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool stopEdit(bool isSave);

	/**
	 * @brief   編集破棄処理
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool abort();

	/**
	 * @brief テーブルの統計情報更新
	 * @param layerName[in] レイヤ名
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool tableAnalyze(const CString& layerName);

private:
	/**
	 * @brief 編集開始（StartEditing）
	 * @param bRedoUndo[in] RedoUndoを行うか（true：行う、false：行わない）
	 * @return 処理結果(State)
	 */
	State StartEditing(bool bRedoUndo);

	/**
	 * @brief 編集終了（StopEditing）
	 * @param bSave[in] 編集結果を保存する（true：保存する、false：保存しない）
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool StopEditing(bool bSave);

	/**
	 * @brief 編集開始（StartEditOperation）
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool StartEditOperation();

	/**
	 * @brief 編集終了（StopEditOperation）
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool StopEditOperation();

	/**
	 * @brief 編集破棄(AbortEditOperation)
	 * @note バージョン対応に対する編集開始時のみ実施
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool AbortEditOperation();

protected:
	Arguments m_args;                                      //!< 引数
	IWorkspaceEditPtr m_edtWorkspace;                      //!< ワークスペース
	std::map<CString, LayerInfoManager> m_baseLayerInfos;  //!< 参照元レイヤ情報管理クラス群
	std::map<CString, LayerInfoManager> m_edtLayerInfos;   //!< 編集レイヤ情報管理クラス群
	std::map<CString, CComVariant> m_commonAttrMap;        //!< フィーチャ共通属性更新値管理MAP
	bool m_isVersioned;
};

