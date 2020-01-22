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

namespace ENCODE {
	enum ECode {
		UNKNOWN_CODE = 0,
		SHIFT_JIS = 1,
		UTF_8 = 2
	};
}

namespace SHAPETYPE {
	enum ECode {
		UNKNOWN_TYPE = 0,
		NORMAL = 1,
		WKT = 2
	};
}

namespace POLYGONTYPE {
	enum ECode {
		NORMAL = 0,
		CLOCKWISE = 1,
		ANTICLOCKWISE = 2
	};
}

/**
 * @brief 測地系
 */
namespace GEOREF {
	enum ECode {
		UNKNOWN_CODE	= 0,
		TOKYO97			= 4301,
		JGD2000			= 4612,
		JGD2011			= 6668,
	};
}

// 出力データフォーマット情報
class COutputFormat
{
public:
	COutputFormat() : m_GeoRef(0), m_ShapeType(SHAPETYPE::UNKNOWN_TYPE), m_PolygonType(POLYGONTYPE::NORMAL){}
	~COutputFormat(){}

public:
	/**
	 * @brief 初期化
	 * @param args [in] コマンドライン引数
	 * @retval true  処理成功
	 * @retval false 処理失敗
	 */
	bool init(const Arguments& args);

private:
	/**
	 * @brief 文字コード設定
	 * @param encode [in] 文字コード用のオプション値
	 * @retval true  処理成功
	 * @retval false 処理失敗
	 */
	bool setEncode(const CString& encode);

	/**
	 * @brief デリミタ設定
	 * @param delimiter [in] デリミタ用のオプション値
	 * @retval true  処理成功
	 * @retval false 処理失敗
	 */
	bool setDelimiter(const CString& delimiter);

	/**
	 * @brief 改行コード設定
	 * @param newline [in] 改行コード用のオプション値
	 * @retval true  処理成功
	 * @retval false 処理失敗
	 */
	bool setNewline(const CString& newline);

	/**
	 * @brief 測地系設定
	 * @param georef [in] 測地系用のオプション値
	 * @retval true  処理成功
	 * @retval false 処理失敗
	 */
	bool setGeoReference(const CString& georef);

	/**
	 * @brief 形状タイプ設定
	 * @param shapetype [in] 形状タイプ用のオプション値
	 * @retval true  処理成功
	 * @retval false 処理失敗
	 */
	bool setShapeType(const CString& shaptype);

	/**
	 * @brief ポリゴンの向き設定
	 * @param polygontype [in] ポリゴン向き用のオプション値
	 * @retval true  処理成功
	 * @retval false 処理失敗
	 */
	bool setPolygonType(const CString& polygontype);

public:
	ENCODE::ECode m_Encode;			//!< 文字コード
	CString m_Delimiter;			//!< デリミタ
	CString m_Newline;				//!< 改行コード
	long m_GeoRef;					//!< 測地系
	CString m_GeoRefName;			//!< 測地系名
	SHAPETYPE::ECode m_ShapeType;	//!< 形状タイプ
	POLYGONTYPE::ECode m_PolygonType; //!< ポリゴンの向き
	bool m_Header;					//!< ヘッダ出力有無
};
