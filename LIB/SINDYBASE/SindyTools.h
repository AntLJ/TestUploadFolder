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
 *	@date	2003/10/17		�V�K�쐬
 */

#ifndef	___SINDY_TOOLS_H___
#define	___SINDY_TOOLS_H___

#include "GDB.h"

extern HRESULT SindyGetInfID(ITablePtr ipTable, long cMeshCode, bool cStart, std::set<long> *cList);

/**
 * @brief �^����ꂽID�iA,B,C�c�j��Ɨ񖼂ɑ΂��āA�u��=A OR ��=B OR ��=C OR �c�v�Ƃ���Where����쐬���ĕԂ�
 * @param[in]	cList			ID��
 * @param[in]	cColumn			��
 * @param[out]	cWhereClause	�쐬����Where�啶����
 * @return	���ʁi���S_OK�j
 */
extern HRESULT SindyGetWhereClause(const std::set<long> &cList, const std::string &cColumn, std::string *cWhereClause);

/**
 * @brief �^����ꂽID�iA,B,C�c�j��Ɨ񖼂ɑ΂��āA�u�� IN (A,B,C,�c)�v�Ƃ���Where����쐬���ĕԂ�
 * @param[in]	cList			ID��
 * @param[in]	cColumn			��
 * @param[out]	cWhereClause	�쐬����Where�啶����
 * @return	���ʁiIN��̒��ɗ񋓂����ID�̐���1000�𒴂���ꍇ�͎��s����j
 */
extern HRESULT SindyGetWhereClause2(const std::set<long> &cList, const std::string &cColumn, std::string *cWhereClause);

/**
 * @brief �^����ꂽID�iA,B,C�c�j��Ɨ񖼂ɑ΂��āA�u�� IN (A,B,C,�c)�v�Ƃ���Where��Q���쐬���A�^����ꂽ�R���e�i�ɒǉ�����
 * @note	ID��̐���1000�𒴂���ꍇ�́A1000�ɂ��P��Where����쐬
 *			[Bug7843]�Ή����ɍ쐬
 * @param[in]	cList			ID��
 * @param[in]	cColumn			��
 * @param[in,out]	crWhereClauses	�쐬����Where�啶����Q�̒ǉ���
 */
extern void SindyGetWhereClauses( const std::set<long> &cList, const std::string &cColumn, std::vector<std::string>& crWhereClauses );

#endif	//___SINDY_TOOLS_H___
