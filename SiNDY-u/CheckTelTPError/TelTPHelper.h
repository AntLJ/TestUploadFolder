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

#include <SiNDYLib/FeatureClass.h>

/**
 * @class	CTelTPHelper
 * @brief	TelTPポイントヘルパー検索クラス
 * @note	TelTPポイントに対する色々な処理を行う
 */
class CTelTPHelper
{
public:
	CTelTPHelper( CFeatureClass& cTelTPPointFC ) : 
		m_cTelTPPointFC(cTelTPPointFC)
	{}
	~CTelTPHelper(void){};

	/**
	 * @brief	TelTPポイント検索
	 * @param	cRec		[in,out]	リストレコード
	 * @param	bAddMode	[in]		付与モード？
	 * @return	EResultStat(チェック結果ステータス)
	 */
	EResultStat findPoint( CLRec& cRec, bool bAddMode );

	/**
	 * @brief	TelTPポイントの情報取得
	 * @param	cRec	[in, out]	リストレコード
	 * @param	ipFeature	[in]	TelTPのipFeaturePtr
	 * @return	bool
	 */
	bool getInfo( CLRec& cRec, CRow cTelTPPoint );

	/**
	 * @brief	指定種別のドメイン取得(現状未使用、使用するためにはCheckTelTPError::outputLogにCTelTPHelperを追加する。)
	 * @param	lpszDomainName	[in]	取得したいドメインのフィールド名
	 * @param	lDomainCode		[in]	取得したいドメインのコード値
	 * @return	CString					指定種別のコード値に対応するドメイン名
	 */
	const std::string CTelTPHelper::getDomain(LPCTSTR lpszDomainName, long lDomainCode) const;

private:

	CFeatureClass	m_cTelTPPointFC;		//!< TelTPポイントフィーチャクラス
	IFeaturePtr		m_ipFeature;			//!< TelTPポイントのフィーチャ
};
