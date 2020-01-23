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
 * @brief 指定座標を含むリージョン階層を作成する。
 *
 * @note
 * リージョン情報は、コミットチャージが物理メモリ合計-100MBを超えるまでキャッシュされる。<br>
 * 超えたら、一番最後に使用されたリージョン情報から削除する。
 * 
 * @param ipPoint [in] 座標。
 * @retval !0 リージョン階層。
 * @retval 0 リージョン階層を作成できなかった。
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

	// 「使用仮想メモリ量 > 全物理メモリ量-100MB || プロセス使用可能メモリ残量 <= 100MB」であるならば、最近使っていないリージョンを解放する。
	while(1) {
		MEMORYSTATUSEX msex = { sizeof(MEMORYSTATUSEX) };
		::GlobalMemoryStatusEx(&msex);
		DWORDLONG ullSwapLimitMB = (msex.ullTotalPhys - (msex.ullTotalPageFile - msex.ullAvailPageFile)) / (1024 * 1024);
		DWORDLONG ullAvailVirtualMB = msex.ullAvailVirtual / (1024 * 1024);

#if(0) // メモリ使用状況の監視
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
