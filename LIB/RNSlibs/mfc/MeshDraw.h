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

#if !defined(AFX_MESHDRAW_H__8DA5B93D_75D2_485F_84CE_5FA362AD9B0D__INCLUDED_)
#define AFX_MESHDRAW_H__8DA5B93D_75D2_485F_84CE_5FA362AD9B0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MeshDraw.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CMeshDraw ウィンドウ

class CMeshDraw : public CStatic
{
// コンストラクション
public:
	CMeshDraw();

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMeshDraw)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CMeshDraw();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CMeshDraw)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
// 定義
	typedef int MeshCode;
	typedef std::set<MeshCode> MeshCodeSet;

// 設定
	void setMeshCodeSet(const MeshCodeSet& rMeshCodeSet) { m_cMeshCodeSet = rMeshCodeSet; }

	void setSelect(const MeshCodeSet& rMeshCode) { m_cSelectedMeshCodeSet = rMeshCode; }
	void appendSelect(const MeshCode& rMeshCode) { m_cSelectedMeshCodeSet.insert(rMeshCode); }
	void appendSelectBy(std::istream& rStream);

	void clearSelect() { m_cSelectedMeshCodeSet.clear(); }

// 取得
	const MeshCodeSet& meshCodeSet() const { return m_cMeshCodeSet; }
	const MeshCodeSet& selectedMeshCodeSet() const { return m_cSelectedMeshCodeSet; }

// 移動
	void zoomIn();
	void zoomOut();
	void zoom(double dScale);

protected:
// ２次メッシュ矩形取得
	RECT GetMeshRect(int nMeshCode);

// 静的プロパティ
	static COLORREF m_sForeColor; ///< 前景色
	static COLORREF m_sBkColor; ///< 背景色
	static COLORREF m_sMeshBrushColor; ///< メッシュ背景色
	static COLORREF m_sSelectColor; ///< 選択色

	static CPen m_s2ndMeshPen; ///< ２次メッシュペン
	static CPen m_sSelectedMeshPen; ///< 選択メッシュペン

	static CBrush m_sMeshBrush; ///< メッシュ塗りつぶしブラシ
	static CBrush m_sSelectedMeshBrush; ///< 選択メッシュ塗りつぶしブラシ

private:
// プロパティ
	MeshCodeSet m_cMeshCodeSet; ///< 全メッシュコードセット
	MeshCodeSet m_cSelectedMeshCodeSet; ///< 選択されたメッシュコードセット

	double m_dMeshWidth; ///< メッシュ幅
	double m_dMeshHeight; ///< メッシュ高さ
	CPoint m_pntOffset; ///< 中心位置

	BOOL m_bDragging; ///< ドラッグ中？
	CPoint m_pntPrevious; ///< ドラッグ中の直前のマウス位置
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MESHDRAW_H__8DA5B93D_75D2_485F_84CE_5FA362AD9B0D__INCLUDED_)
