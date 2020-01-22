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

// ArrivedInfo.cpp: ArrivedInfo �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ArrivedInfo.h"

namespace sindy
{
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

ArrivedInfo::ArrivedInfo()
{

}

ArrivedInfo::~ArrivedInfo()
{

}

//////////////////////////////////////////////////////////////////////
// ����
//////////////////////////////////////////////////////////////////////

void ArrivedInfo::clear()
{
	m_cRoute.clear();
	m_cUturnRouteMap.clear();
}

//////////////////////////////////////////////////////////////////////
// �ݒ�
//////////////////////////////////////////////////////////////////////

/**
 * @brief ���[�g��ݒ肷��
 *
 * @param rRoute [in] �ݒ肷�郋�[�g
 * @return �ݒ肵�����[�g�B�w�肵�����[�g�����m�̃��[�g�������I�ł͂Ȃ����ߐݒ肵�Ȃ������ꍇNULL�B
 */
Route* ArrivedInfo::setRoute(const Route &rRoute)
{
	ATLASSERT(rRoute.isValid());

	// ���[�g���ݒ肳��Ă��Ȃ��ꍇ�A�������[�g�Ƃ��Đݒ肷��
	if(isNotArrived()) {
		if(rRoute.isNoRegulations()) {
			// �E���܋֎~���ݒ肳��Ă��Ȃ��ꍇ�A�������������[�g��
			return &(m_cRoute = rRoute);
		}
		else {
			// �E���܋֎~���ݒ肳��Ă���ꍇ�A�����t�������[�g��
			Route* pResult = 0;

			if(rRoute.m_pLeftLink) {
				pResult = &(m_cUturnRouteMap[rRoute.m_pLeftLink] = rRoute);
			}

			return pResult;
		}
	}

	// �K���֌W�Ȃ����[�g���R�X�g���������[�g�̓p�X
	if(m_cRoute.isValid() && m_cRoute.m_dCost <= rRoute.m_dCost)
		return 0;

	if(rRoute.isNoRegulations()) {
		// �ݒ肷�郋�[�g�ɋK�����ݒ肳��Ă��Ȃ������ꍇ�̏���
		return &(m_cRoute = rRoute);
	}
	else {
		Route* pResult = nullptr;

		return pResult;
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////
// �擾
//////////////////////////////////////////////////////////////////////

/**
 * @brief �o�^����Ă��郋�[�g�̐����擾����
 *
 * @return ���[�g�̐�
 */
int ArrivedInfo::countRoute() const
{
	return (m_cRoute.isEmpty() ? 0 : 1) + m_cUturnRouteMap.size() ;
}

/**
 * @brief �K���ɗ���ł�����̂��܂߁A�����Ƃ��D�ꂽ���[�g
 *
 * @return �ŒZ���[�g�B���B�������[�g�����݂��Ȃ��ꍇ��NULL�B
 */
Route* ArrivedInfo::bestRouteOfAll()
{
	Route* pResult = 0;
	
	if(m_cRoute.isValid()) {
		pResult = &m_cRoute;
	}

	Route* pRouteOfReg = bestRouteOfReg();

	if(! pResult || (pRouteOfReg && pRouteOfReg->m_dCost < pResult->m_dCost)) {
		pResult = pRouteOfReg;
	}

	return pResult;
}

const Route* ArrivedInfo::bestRouteOfAll() const
{
	const Route* pResult = 0;
	
	if(m_cRoute.isValid()) {
		pResult = &m_cRoute;
	}

	const Route* pRouteOfReg = bestRouteOfReg();

	if(! pResult || (pRouteOfReg && pRouteOfReg->m_dCost < pResult->m_dCost)) {
		pResult = pRouteOfReg;
	}

	return pResult;
}

/**
 * @brief �K���ɗ���ł�����̂̂Ȃ��ŁA�����Ƃ��D�ꂽ���[�g
 *
 * @return �K������݂̍ŒZ���[�g�B���݂��Ȃ��ꍇ��NULL�B
 */
Route* ArrivedInfo::bestRouteOfReg()
{
	Route* pResult = 0;

	for(LinkKeyRouteMap::iterator itr = m_cUturnRouteMap.begin(); itr != m_cUturnRouteMap.end(); ++itr) {
		Route& rRoute = itr->second;
		
		if(rRoute.isValid() && (! pResult || rRoute.m_dCost < pResult->m_dCost)) {
			pResult = &rRoute;
		}
	}

	return pResult;
}

const Route* ArrivedInfo::bestRouteOfReg() const
{
	const Route* pResult = 0;

	for(LinkKeyRouteMap::const_iterator itr = m_cUturnRouteMap.begin(); itr != m_cUturnRouteMap.end(); ++itr) {
		const Route& rRoute = itr->second;
		
		if(rRoute.isValid() && (! pResult || rRoute.m_dCost < pResult->m_dCost)) {
			pResult = &rRoute;
		}
	}

	return pResult;
}

/**
 * @brief �u�o���t���O�v�������Ă��Ȃ����̂̒��ŁA�ł��Z�����[�g
 *
 * @return �ŒZ���[�g�B���݂��Ȃ��ꍇ��NULL�B
 */
Route* ArrivedInfo::bestRouteOfYetDepartured()
{
	Route*	pResult = 0;
	
	if(m_cRoute.isValid() && ! m_cRoute.m_bDepartured) {
		pResult = &m_cRoute;
	}

	for(LinkKeyRouteMap::iterator itr = m_cUturnRouteMap.begin(); itr != m_cUturnRouteMap.end(); ++itr) {
		Route& rRoute = itr->second;

		if(rRoute.isValid() && ! rRoute.m_bDepartured && ! pResult || (pResult && rRoute.m_dCost < pResult->m_dCost)) {
			pResult = &rRoute;
		}
	}

	return pResult;
}

//////////////////////////////////////////////////////////////////////
// ����
//////////////////////////////////////////////////////////////////////

/**
 * @brief �܂���x���K�ꂽ���Ƃ��Ȃ���
 *
 * @retval true ����
 * @retval false �K�ꂽ���Ƃ��L��
 */
bool ArrivedInfo::isNotArrived() const
{
	return m_cRoute.isEmpty() && m_cUturnRouteMap.empty();
}

/**
 * @brief �K���ɗ���ł�����̂��܂߁A�ǂ̃��[�g�����R�X�g�̏��Ȃ����[�g��
 *
 * @retval true �Z��
 * @retval false �����A���邢�͓�������
 */
bool ArrivedInfo::isBetterThanAll(const Route& rRoute) const
{
	const Route* pBestRoute = bestRouteOfAll();

	return ! pBestRoute || rRoute.m_dCost < pBestRoute->m_dCost ;
}

}
