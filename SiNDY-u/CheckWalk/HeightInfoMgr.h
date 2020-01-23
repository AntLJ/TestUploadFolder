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

namespace
{
	/**
	 * @brief 高さ情報
	 */
	enum Height : int
	{
		higher = 2,  //<! 地面よりさらに高い 
		high   = 1,  //<! 地面より高い
		ground = 0,  //<! 地面と同じ
		low	= -1, //<! 地面より低い
		loweer = -2, //<! 地面よりさらに低い
	};
}

/**
 * @brief  高さ情報を管理するクラス
 */
class HeightInfoMgr
{
public:
	/**
	 * @brief  コンストラクタ
	 */
	HeightInfoMgr(void){}

	/**
	 * @brief  コンストラクタ
	 */
	virtual ~HeightInfoMgr(void){}

	/**
	 * @brief   高さ情報を読み込む
	 * @param  path  [in]  高さ情報定義ファイルのパス
	 * @return true:成功
	 */
	bool read (const uh::tstring& path );

	/**
	 * @brief  歩行者リンクの高さ情報を返す
	 * @param  attr  [in]  歩行者リンクの属性
	 * @return　歩行者リンクの高さ情報
	 */
	int getWalkHeight( long attr ) const;

	/**
	 * @brief  道路リンクの高さ情報を返す
	 * @param  attr  [in]  道路リンクの属性
	 * @return 道路リンクの高さ情報
	 */
	int getRoadHeight( long attr ) const;

private:
	/// 歩行者リンクの低い属性群
	std::set<long> m_walkLowAttr;
	/// 歩行者リンクの高い属性群
	std::set<long> m_walkHighAttr;
	/// 道路リンクの低い属性群
	std::set<long> m_roadLowAttr;
	/// 道路リンクの高い属性群
	std::set<long> m_roadHighAttr;
};
