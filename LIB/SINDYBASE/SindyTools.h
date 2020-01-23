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
 *	SindyTools.h
 *	@author	Fusatoshi Abe
 *	@date	2003/10/17		新規作成
 */

#ifndef	___SINDY_TOOLS_H___
#define	___SINDY_TOOLS_H___

#include "GDB.h"

extern HRESULT SindyGetInfID(ITablePtr ipTable, long cMeshCode, bool cStart, std::set<long> *cList);

/**
 * @brief 与えられたID（A,B,C…）列と列名に対して、「列名=A OR 列名=B OR 列名=C OR …」というWhere句を作成して返す
 * @param[in]	cList			ID列
 * @param[in]	cColumn			列名
 * @param[out]	cWhereClause	作成したWhere句文字列
 * @return	結果（常にS_OK）
 */
extern HRESULT SindyGetWhereClause(const std::set<long> &cList, const std::string &cColumn, std::string *cWhereClause);

/**
 * @brief 与えられたID（A,B,C…）列と列名に対して、「列名 IN (A,B,C,…)」というWhere句を作成して返す
 * @param[in]	cList			ID列
 * @param[in]	cColumn			列名
 * @param[out]	cWhereClause	作成したWhere句文字列
 * @return	結果（IN句の中に列挙されるIDの数が1000を超える場合は失敗する）
 */
extern HRESULT SindyGetWhereClause2(const std::set<long> &cList, const std::string &cColumn, std::string *cWhereClause);

/**
 * @brief 与えられたID（A,B,C…）列と列名に対して、「列名 IN (A,B,C,…)」というWhere句群を作成し、与えられたコンテナに追加する
 * @note	ID列の数が1000を超える場合は、1000個につき１つのWhere句を作成
 *			[Bug7843]対応時に作成
 * @param[in]	cList			ID列
 * @param[in]	cColumn			列名
 * @param[in,out]	crWhereClauses	作成したWhere句文字列群の追加先
 */
extern void SindyGetWhereClauses( const std::set<long> &cList, const std::string &cColumn, std::vector<std::string>& crWhereClauses );

#endif	//___SINDY_TOOLS_H___
