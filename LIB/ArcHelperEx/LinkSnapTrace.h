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

#ifndef LINK_SNAP_TRACE_H_
#define LINK_SNAP_TRACE_H_

#include <boost/scoped_ptr.hpp>

/**
 * @brief スナップしたリンクを繋げていくクラス。
 */
class LinkSnapTrace
{
	class impl;
public:
// コンストラクタとデストラクタ
	LinkSnapTrace(IFeatureClass* ipLinkClass);
	~LinkSnapTrace();

// 設定
	void setTolerance(double dTolerance, double dCacheRatio);
	void setAhead(bool bAhead);
	/**
	 * @brief 検索条件設定
	 * @bug   [Bug 10374]トレース可能なラインデータを、平行移動後（[MANAGEMENT_ID]is not Null)のオブジェクトに限定する
	 * @param lpcszWhere [in] 検索条件
	*/
	void setWhere(LPCTSTR lpcszWhere);

// 取得
	IPolylinePtr getTrace() const;
	IArrayPtr getLinkArray() const;
	long getLastMarkingIndex() const;

// 処理
	bool start(IPoint* ipPoint);
	bool trace(IPoint* ipPoint);
	void mark();
	void demark();
	void undo();
	void stop();
	bool back_public();
private:
// 実装
	boost::scoped_ptr<impl> pimpl;
};

#endif // LINK_SNAP_TRACE_H_
