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

// EtcLinkList.h: EtcLinkList �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ETCLINKLIST_H__7BFD1718_1AC5_4FBA_9544_339E15D40A61__INCLUDED_)
#define AFX_ETCLINKLIST_H__7BFD1718_1AC5_4FBA_9544_339E15D40A61__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>

class EtcLinkList  
{
public:
	enum EType
	{
		type_ramp	= 1,	// ETC��p�����v�����N
		type_access	= 2		// ETC�ڑ���ʓ������N
	};

	struct EtcLink
	{
		typedef	std::vector<long>	NodeList;

		EType		eType;				// ���ރR�[�h
		long		eMeshCode;			// ���b�V���R�[�h
		long		eFromNodeID;		// �n�_�m�[�hID
		NodeList	eNodeList;			// ���_�m�[�hID��
		long		eToNodeID;			// �I�_�m�[�hID

		EtcLink(void) : eMeshCode(0), eFromNodeID(0), eToNodeID(0) {}
	};

	typedef std::vector<EtcLink> Data;

	EtcLinkList();
	virtual ~EtcLinkList();

	void	append(EtcLink cEtcLink) { mData.push_back(cEtcLink); }
	void	write(std::ostream& crStream);
	void	write(const char* cFilePath, bool cUseEUC);

private:
	Data	mData;

};

#endif
