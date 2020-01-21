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

// SINDY2HWYSAME.h: SINDY2HWYSAME クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDY2HWYSAME_H__5B9246F6_99EB_491C_84A2_D6B1A62B08E0__INCLUDED_)
#define AFX_SINDY2HWYSAME_H__5B9246F6_99EB_491C_84A2_D6B1A62B08E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SINDY2INHWY.h"
#include "Customer.h"
#include "FacilNameChangeTable.h"

class SINDY2HWYSAME : public SINDY2INHWY  
{
public:
	enum EFormat
	{
		hwysame_old			= 1,	/// 旧フォーマット(5.50以前)、スマートICを出力しない
		hwysame_new			= 2		/// 通常のフォーマット(5.61以降)、スマートICを出力する
	};

	SINDY2HWYSAME(SINDY2INHWYINFO *cHwyInfo);
	virtual ~SINDY2HWYSAME();

	virtual HRESULT Convert();
	virtual HRESULT Write(std::string cFileName, EFormat cFormat);

	/**
	 * @brief 施設名称変換（仕向け先指定）
	 * @note	施設名称変換が不要な仕向け先が指定された場合は何もしない。
	 * @param	cCustomer	[in]	仕向け先
	 * @param	cpFacilNameChangeTable	[in]	施設名称変換テーブル
	 */
	void changeFacilName(Customer cCustomer, FacilNameChangeTable* cpFacilNameChangeTable);

	/**
	 * @brief 施設名称変換
	 * @param	cpFacilNameChangeTable	[in]	施設名称変換テーブル
	 */
	void changeFacilName( FacilNameChangeTable* cpFacilNameChangeTable );

	HRESULT createHwySameListWithoutSmartIC(void);

public:
	struct HwySame
	{
		long				eRoadCode;
		long				eSequence;
		long				eDirection;
		std::string			eName;
		std::vector<long>	eFacil;

		enum direction_c
		{
			fore			= 0,	// 順方向(下り)
			back			= 1		// 逆方向(上り)
		};

		HwySame() : eRoadCode(0), eSequence(0), eDirection(0), eName(""), eFacil() {}
		virtual ~HwySame() {}
	};

	struct lessHwySame : public std::binary_function<HwySame,HwySame,bool>
	{
		bool operator ()(const HwySame &cHwySame1, const HwySame &cHwySame2) const
		{
			return (cHwySame1.eRoadCode != cHwySame2.eRoadCode) ? cHwySame1.eRoadCode < cHwySame2.eRoadCode : (cHwySame1.eSequence != cHwySame2.eSequence) ? cHwySame1.eSequence < cHwySame2.eSequence : cHwySame1.eDirection < cHwySame2.eDirection;
		}
	};

protected:
	typedef std::vector<HwySame> Data;
	Data	mHwySameList;
	Data	mHwySameListWithoutSmartIC;		// スマートICを除いたデータ

public:
	typedef Data::const_iterator Iter;

	/**
	 * @brief データ列の先頭を指すconst_iteratorを返す
	 * @param[in]	withoutSmartIC	trueなら「スマートICを除いたデータ」、falseなら通常のデータを扱う
	 */
	Iter begin( bool withoutSmartIC = false ) const { return withoutSmartIC ? mHwySameListWithoutSmartIC.begin() : mHwySameList.begin(); }

	/**
	 * @brief データ列の末尾を指すconst_iteratorを返す
	 * @param[in]	withoutSmartIC	trueなら「スマートICを除いたデータ」、falseなら通常のデータを扱う
	 */
	Iter end( bool withoutSmartIC = false ) const { return withoutSmartIC ? mHwySameListWithoutSmartIC.end() : mHwySameList.end(); }

	/**
	 * @brief データ列のサイズを返す
	 * @param[in]	withoutSmartIC	trueなら「スマートICを除いたデータ」、falseなら通常のデータを扱う
	 */
	std::size_t size( bool withoutSmartIC = false ) const { return withoutSmartIC ? mHwySameListWithoutSmartIC.size() : mHwySameList.size(); }
};

#endif // !defined(AFX_SINDY2HWYSAME_H__5B9246F6_99EB_491C_84A2_D6B1A62B08E0__INCLUDED_)
