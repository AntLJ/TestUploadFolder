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


// 以下の ifdef ブロックは DLL から簡単にエクスポートさせるマクロを作成する標準的な方法です。 
// この DLL 内のすべてのファイルはコマンドラインで定義された EXPORTDLG_EXPORTS シンボル
// でコンパイルされます。このシンボルはこの DLL が使用するどのプロジェクト上でも未定義でなけ
// ればなりません。この方法ではソースファイルにこのファイルを含むすべてのプロジェクトが DLL 
// からインポートされたものとして EXPORTDLG_API 関数を参照し、そのためこの DLL はこのマク 
// ロで定義されたシンボルをエクスポートされたものとして参照します。
/*#ifdef EXPORTDLG_EXPORTS
#define EXPORTDLG_API __declspec(dllexport)
#else
#define EXPORTDLG_API __declspec(dllimport)
#endif
*/

#ifndef __EXPORTDLG_H__
#define __EXPORTDLG_H__

class CArcHelper;
class CLQRowDef;
class CFeatureDef;

class CExportDlg
{
public:
	virtual HWND Create(HWND hWndParent, LPARAM dwInitParam = NULL) = 0;
	virtual HWND GetDlg() = 0;
	virtual void Delete() = 0;
	virtual void SetArcHelper( IApplication* ipApp ) = 0;
	virtual void SetAliasOrField( BOOL bAliasOrField ) = 0;
	virtual void SetFeatureDefList( std::list<CLQRowDef>* pFeatureDefList ) = 0;
	virtual void SetFeatureDefList( std::list<CFeatureDef>* pFeatureDefList ) = 0;
	virtual BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable ) = 0;
	virtual void ClearFeatureDefs() = 0;
	virtual BOOL ErrorCheck() = 0;
	virtual LONG GetTabNum() = 0;
	virtual void SetTabNum(LONG lTabNum) = 0;
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName) = 0;
	virtual LRESULT SendMessage( UINT message, WPARAM wParam, LPARAM lParam ) = 0;
};
#endif  
