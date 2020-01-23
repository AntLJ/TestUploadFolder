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

// コマンドライン引数用（構造体でも良かった。。）
class CInputInfo
{
public:
	CInputInfo() : bTestMode(false), lPurpose(-1) {}
	~CInputInfo(){}

public:
	CString strBeforeDir;     //!< 編集前PGDBのディレクトリ
	CString strAfterDir;      //!< 編集後PGDBのディレクトリ
	CString strSDEConnect;    //!< インポート先DBへの接続文字列
	CString strOperator;      //!< インポート時の編集者名
	long lPurpose;            //!< インポート時の作業目的
	std::set<long> setMesh;   //!< インポート対象メッシュリスト
	CString strLogDir;        //!< ログ出力先ディレクトリ
	bool bTestMode;           //!< テストモードかどうか
	CString strExceptList;    //!< インポート除外フィールドリストファイル名(bug 10363)
};

class CDefaultInfo
{
public:
	CDefaultInfo(){}
	~CDefaultInfo(){}

public:
	void init();

public:

};
