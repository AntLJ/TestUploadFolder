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

#include "StdAfx.h"
#include "CheckUpperReachability.h"
#include "RegionPyramidCreator.h"

RegionPyramidCreator::RegionPyramidCreator(RegionManagerRef pRegMng, RegionPropertyManagerPyramidRef pRegPropMngPyramid) :
m_pRegMng(pRegMng),
m_pRPMP(pRegPropMngPyramid)
{
	_ASSERT(m_pRPMP->size() > 0);
}

/**
 * @brief �w����W���܂ރ��[�W�����K�w���쐬����B
 *
 * @note
 * ���[�W�������́A�R�~�b�g�`���[�W���������������v-100MB�𒴂���܂ŃL���b�V�������B<br>
 * ��������A��ԍŌ�Ɏg�p���ꂽ���[�W������񂩂�폜����B
 * 
 * @param ipPoint [in] ���W�B
 * @retval !0 ���[�W�����K�w�B
 * @retval 0 ���[�W�����K�w���쐬�ł��Ȃ������B
 */
std::auto_ptr<RegionPyramid> RegionPyramidCreator::create(IPoint* ipPoint)
{
	const size_t n = m_pRPMP->size();
	std::auto_ptr<RegionPyramid> pRegPyramid(new RegionPyramid(n));

	for(size_t i = 0; i < n; i++) {
		while(1) {
			try {
				const RegionPropertyRef pRegProp((*m_pRPMP)[i].getRegionProperty(ipPoint));
				if(! pRegProp)
					return std::auto_ptr<RegionPyramid>();

				(*pRegPyramid)[i] = *m_pRegMng->getRegion(pRegProp);

				break;
			}
			catch(const std::bad_alloc&) {
				if(! m_pRegMng->releaseLRURegion())
					throw;
			}
		}
	}

	// �u�g�p���z�������� > �S������������-100MB || �v���Z�X�g�p�\�������c�� <= 100MB�v�ł���Ȃ�΁A�ŋߎg���Ă��Ȃ����[�W�������������B
	while(1) {
		MEMORYSTATUSEX msex = { sizeof(MEMORYSTATUSEX) };
		::GlobalMemoryStatusEx(&msex);
		DWORDLONG ullSwapLimitMB = (msex.ullTotalPhys - (msex.ullTotalPageFile - msex.ullAvailPageFile)) / (1024 * 1024);
		DWORDLONG ullAvailVirtualMB = msex.ullAvailVirtual / (1024 * 1024);

#if(0) // �������g�p�󋵂̊Ď�
		std::cerr
			<< "["
			<< "SL" << ullSwapLimit << "MB "
			<< "AV" << ullAvailVirtualMB << "MB"
			<< "]" << std::endl;
#endif

		if(ullSwapLimitMB > 100 && ullAvailVirtualMB > 100)
			break;

		if(! m_pRegMng->releaseLRURegion())
			break;
	}

	return pRegPyramid;
}
