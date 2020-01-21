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

// DRMALoadProtocol.h: DRMALoadProtocol クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRMALOADPROTOCOL_H__E2D2CEE1_E922_4921_8B75_F226DAC89A6D__INCLUDED_)
#define AFX_DRMALOADPROTOCOL_H__E2D2CEE1_E922_4921_8B75_F226DAC89A6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <iostream>

/**
 * @brief DRMAデータ読み取りプロトコル
 */
class DRMALoadProtocol  
{
public:
	DRMALoadProtocol();

	virtual bool load(const char* cDirectoryName, int cMeshCode, const char* cSuffix = ".mt") throw(std::runtime_error);
	virtual bool load(const char* cFileName) throw(std::runtime_error);
	virtual bool load(std::istream& cStream) throw(std::runtime_error);

	void close();

protected:
	/**
	 * @brief 読み込み完了宣言
	 *
	 * 全レコードを走査する前にload()を終了させたい場合、append()内からこの関数を呼ぶ
	 */
	void setComplete() { mLoadComplete = true; }
	
// 純粋仮想関数
	/**
	 * @brief DRMAの1レコードをハンドリングする
	 *
	 * @param cRecordId [in] レコードID
	 * @param cOriginalRecord [in] レコード
	 * @retval true 登録に成功した、あるいは登録しなかった
	 * @retval false あるいは登録に失敗した
	 */
	virtual bool append(int cRecordId, const char* cOriginalRecord) = 0;

	/**
	 * @brief ハンドリングした全てのレコードを解放する
	 */
	virtual void clear() = 0;

private:
// プロパティ
	bool mLoadComplete;
};

#endif // !defined(AFX_DRMALOADPROTOCOL_H__E2D2CEE1_E922_4921_8B75_F226DAC89A6D__INCLUDED_)
