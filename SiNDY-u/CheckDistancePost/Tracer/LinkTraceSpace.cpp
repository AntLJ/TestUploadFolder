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

// LinkTraceSpace.cpp: LinkTraceSpace �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <sindy/schema/road.h>
#include "LinkTraceSpace.h"

using namespace sindy::schema;

namespace sindy
{
typedef std::map<long, LinkBranch::LinkBranchPtrSet> LinkBranchPtrSetMap;

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

LinkTraceSpace::LinkTraceSpace()
{

}

LinkTraceSpace::~LinkTraceSpace()
{

}

//////////////////////////////////////////////////////////////////////
// �N���A
//////////////////////////////////////////////////////////////////////

void LinkTraceSpace::clear()
{
	m_cLinkBranchMap.clear();
}

//////////////////////////////////////////////////////////////////////
// �ݒ�
//////////////////////////////////////////////////////////////////////

/**
 * @param rLinks [in] �o�^���郊���N
 * @param ipEnvelope [in] �����N�ǐՋ�ԋ�`
 */
void LinkTraceSpace::setLinks(const LinksCache& rLinks)
{
	using namespace road_link;

	// �����N��Ԃ��N���A
	clear();

	const LinkMap& rLinkMap = rLinks.m_cLinkMap;

	// �m�[�h���Ƃ̗��o�\�����N���X�g���쐬����
	LinkBranchPtrSetMap cOutableLinkBranchPtrSetMap;
	for ( const auto& itr : rLinkMap)
	{
		const Link& rLink = itr.second;

		// �����N��ԂɃ����N��ǉ�
		LinkBranch& rBranch = m_cLinkBranchMap[itr.first] = LinkBranch(&rLink);

		// ����������ʍs�����N�łȂ���΁ATo�m�[�h�����瓖�Y�����N�ɗ����\
		cOutableLinkBranchPtrSetMap[rLink.m_nToNodeId].insert(&rBranch);

		// �t��������ʍs�����N�łȂ���΁AFrom�m�[�h�����瓖�Y�����N�ɗ����\
		cOutableLinkBranchPtrSetMap[rLink.m_nFromNodeId].insert(&rBranch);
	}

	// �����N���Ƃ̕��򃊃X�g���쐬����
	for(LinkBranchMap::iterator itr = m_cLinkBranchMap.begin(); itr != m_cLinkBranchMap.end(); ++itr) {
		LinkBranch& rBranch = itr->second;
		
		// From�m�[�h�����痬�o���镪���o�^����
		LinkBranchPtrSetMap::const_iterator itr2 = cOutableLinkBranchPtrSetMap.find(rBranch.m_pLink->m_nFromNodeId);
		if(itr2 != cOutableLinkBranchPtrSetMap.end()) {
			rBranch.insertBranch(rBranch.m_pLink->m_nFromNodeId, itr2->second);
		}

		// To�m�[�h�����痬�o���镪���o�^����
		itr2 = cOutableLinkBranchPtrSetMap.find(rBranch.m_pLink->m_nToNodeId);
		if(itr2 != cOutableLinkBranchPtrSetMap.end()) {
			rBranch.insertBranch(rBranch.m_pLink->m_nToNodeId, itr2->second);
		}
	}
}

}
