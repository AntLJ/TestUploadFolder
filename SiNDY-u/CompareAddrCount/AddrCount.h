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

#include <AddrLib/AddrCodeList.h>

using namespace addr;

/**
 * @enum ETarget
 * @brief カウント対象指定方法
 */
enum ETarget
{
	kTgtAll,	///< 全国指定
	kTgtRange,	///< 範囲指定
	kTgtList,	///< リスト指定
	kTgtSingle,	///< 単体指定
};

typedef set<CString> AddrSet;

/**
 * @class AddrCount
 * @brief 住所データ件数カウントクラス
 */
class AddrCount : public map<CString, int>
{

public:

	AddrCount(void) : mCharF(false), mTarget(kTgtAll) {}
	~AddrCount(void) {}

	/**
	 * @brief	初期化
	 * @param	cCharF			[in]	文字付き住所モードかどうか
	 * @param	cTarget			[in]	カウント対象指定方法
	 * @param	cTgtOpt			[in]	カウント対象指定オプション文字列
	 * @param	cGouPoint		[in]	号ポイント・フィーチャクラス
	 * @param	cCityAdmin		[in]	都市地図行政界・フィーチャクラス
	 * @param	cCSAddrPoint	[in]	CSポイント・フィーチャクラス
	 * @return	成功したか否か
	 */
	bool Init(bool cCharF, ETarget cTarget, const CString& cTgtOpt, IFeatureClassPtr cGouPoint, IFeatureClassPtr cCityAdmin, IFeatureClassPtr cCSAddrPoint);

	/**
	 * @brief	カウントの実行
	 * @return	成功したか否か
	 */
	bool Count();

private:

	bool				mCharF;			///< カウントの対象を文字付き住所とするかどうか
	ETarget				mTarget;		///< カウント対象指定方法
	CString				mTgtOpt;		///< 範囲指定文字列（範囲 or リストファイル or 5桁単体）

	CAddrCodeList		mList;			///< カウント対象住所コードリスト

	IFeatureClassPtr	mGouPoint;		///< 号ポイント・フィーチャクラス
	IFeatureClassPtr	mCityAdmin;		///< 都市地図行政界・フィーチャクラス
//	IFeatureClassPtr	mCSAddrPoint;	///< CSポイント・フィーチャクラス
	CFeatureClass		mCSAddrPoint;	///< CSポイント・フィーチャクラス

	CSPFieldMap			mFMapCSPoint;	///< CSポイント・フィールド対応表
	CSPTableNameString	mNameCSPoint;	///< CSポイント・テーブル名

	/**
	 * @brief	カウント対象住所コードリストの作成
	 * @return	成功したか否か
	 */
	bool fnCreateAddrList();

	/**
	 * @brief	号番号文字列を「-」でトークンに分解
	 * @param	cGouNo		[in]		号番号
	 * @param	cTokens		[in,out]	トークンリスト
	 */
	void fnTokenizeGouNo(const CString& cGouNo, vector<CString>& cTokens);
};
