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
* @file Feedback.h
*
* @brief CFeedback クラスのインターフェイス定義
*/

#if !defined(AFX_FEEDBACK_H__A62BEE45_C308_4A59_978D_0637FD1CF6F0__INCLUDED_)
#define AFX_FEEDBACK_H__A62BEE45_C308_4A59_978D_0637FD1CF6F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
* @brief 円形フィードバック管理クラス
*
* @note 選択エリア表示などのため、円図形のフィードバックを管理する
*
*/
class CFeedback  
{
public:
	CFeedback();
	virtual ~CFeedback();

	/**
	* @brief フィードバックの描画
	*
	* @note パラメータの点を中心として、選択領域を描画。
	* @note StartDrawSearchArea後は、マウス移動ごとにこのメソッドを呼び出して利用する。
	*
	* @param piPoint [in] フィードバックを表示させるべき点
	*/
	BOOL DrawSearchArea( IPointPtr piPoint);

	/**
	* @brief フィードバックの再描画
	*
	* @param hDC		[in,optional]	デバイスコンテキスト
	*
	* @note 最後の状態を元に再描画を実行する。
	*/
	void ResetSearchArea( OLE_HANDLE hDC = NULL );

//	void StartDrawSearchArea( IPoint* ipPoint, IEnvelope* ipEnv, IScreenDisplay* ipScreen);

	/**
	* @brief フィードバックを開始
	*
	* @note unToleranceは画面上のPixel値
	*
	* @param piScreen [in] フィードバックを表示させるスクリーンインターフェイス
	* @param piPoint [in] フィードバックを開始する点
	* @param unTolerance [in] 選択領域として表示させる円の半径
	*/
	BOOL StartDrawSearchArea( IScreenDisplayPtr piScreen, IPointPtr piPoint, UINT unTolerance);

	/**
	* @brief フィードバックを停止
	*/
	void StopDrawSearchArea();

	/**
	 * @brief 検索エリアを取得する
	 *
	 * この関数は、ArcObjectsのバグを回避するために与えられた閾値＋10ピクセルで
	 * バッファを作成して返します。現在はほぼ？バグが修正されていますので余計な
	 * バッファリングが不要な場合は引数が2つのものを使用してください。
	 * @param ipPoint		[in]			エリア中心ポイント
	 * @param dSearchRadius	[out]			エリア半径
	 * @param ipSpRef		[in,optional]	ISpatialReferencePtr
	 *
	 * @return IGeometryPtr
	 */
	IGeometryPtr GetSearchArea( IPointPtr ipPoint, DOUBLE* dSearchRadius, ISpatialReferencePtr ipSpRef = NULL ) const;
	IGeometryPtr GetSearchArea( IPointPtr ipPoint, ISpatialReferencePtr ipSpRef ) const;

	void SetScreenDisplay( IScreenDisplay* ipDisp, int* pTolerance = NULL ) const {
		m_ipScreenDisplay = ipDisp;
		// StartDrawSearchAreaでm_unToleranceが初期化されずに来るケースが存在するので
		// ここでも初期化できるように（場当たり的だなぁ…）
		if( pTolerance )
			m_nTolerance = *pTolerance;
	}
	IScreenDisplayPtr GetScreenDisplay() const { return m_ipScreenDisplay; }
private:
	IMovePointFeedbackPtr m_piFeedback; //!< フィードバックインターフェイス
	mutable IScreenDisplayPtr m_ipScreenDisplay;
	mutable int m_nTolerance;
};

#endif // !defined(AFX_FEEDBACK_H__A62BEE45_C308_4A59_978D_0637FD1CF6F0__INCLUDED_)
