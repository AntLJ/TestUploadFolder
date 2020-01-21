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

// DRMARecord.h: DRMARecord クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRMARECORD_H__7E2A08D7_1F8F_4078_9805_6839D7512A95__INCLUDED_)
#define AFX_DRMARECORD_H__7E2A08D7_1F8F_4078_9805_6839D7512A95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DRMAOriginalValues.h"
#include "DRMAValues.h"

/**
 * @brief DRMA項目変換支援クラス
 *
 * DRMAデータに存在する以下のデータを、より一般的な形式のデータに変換する関数の集合体:
 *	- EBCDIC10進数
 *		- 整数値に変換	
 *	- EBCDIC16進数
 *		- 整数値に変換
 *	- EBCDIC半角カナ文字列
 *		- (Win32)ASCII半角カナ文字列に変換
 *	- JIS全角文字列
 *		- (Win32)SJIS全角文字列に変換
 */
class DRMAItemConverter 
{
public:
// DRMAデータ変換
	static bool setValueTo(const _DRMADec* cOriginalItem, int cSize, int& cItem);
	static bool setValueTo(const _DRMAHex* cOriginalItem, int cSize, int& cItem);
	static bool setValueTo(const _DRMAKanji* cOriginalItem, int cSize, char* cItem);
	static bool setValueTo(const _DRMAKana* cOriginalItem, int cSize, char* cItem);

// 一般データ変換
	static WideChar jisToSjis(const WideChar& cJis);
};

/**
 * @brief DRMA項目変換支援クラス拡張
 *
 * PREPARE_SET_VALUE、及びSET_VALUEマクロを用いて、型変換を行いやすくするテンプレートクラス
 */
template<typename TFormat>
class DRMAItemConverterTemplate : protected DRMAItemConverter
{
public:
	typedef TFormat Format; ///< 当レコードのフォーマット

	/**
	 * @brief 項目追加ループ
	 *
	 * @param cOriginalList [in] 変換元項目リスト
	 * @param cList [in] 追加先項目リスト（std::vector互換クラス）
	 * @param cItemCount [in] 総項目数
	 * @param cLoopCount [in] １レコード辺りの項目数
	 */
	template<typename TOriginalList, typename TList, typename TCount>																 
	static void appendLoop(const TOriginalList& cOriginalList, TList& cList, TCount& cItemCount, int cLoopCount)
	{
		for(int i = 0; i < cLoopCount && (int)cList.size() < cItemCount; i++) {
			cList.push_back(TList::value_type(cOriginalList[i]));
		}
	}
};

/**
 * @brief DRMAレコードクラス
 */
class DRMARecord
{
public:
	int eRecordId; ///< レコードID
};

template<typename TFormat>
class DRMARecordT : public DRMARecord, public DRMAItemConverterTemplate<TFormat>
{
};

/// 値設定簡略化マクロ
#define PREPARE_SET_VALUE(original) const Format& _rOriginal = original
#define SET_VALUE(item) setValueTo(_rOriginal.item, sizeof(_rOriginal.item), item)
#define SET_VALUE_FROM(original, item) setValueTo(original.item, sizeof(original.item), item)
#define SET_VALUE_FROM_TO(original, item) setValueTo(original, sizeof(original), item)

#define APPEND_LOOP(_list, _itemcount, _loopcount) appendLoop(_rOriginal._list, _list, _itemcount, _loopcount)
#define APPEND_LOOP_FROM(_original, _list, _itemcount, _loopcount) appendLoop(_original._list, _list, _itemcount, _loopcount)

#endif // !defined(AFX_DRMARECORD_H__7E2A08D7_1F8F_4078_9805_6839D7512A95__INCLUDED_)
