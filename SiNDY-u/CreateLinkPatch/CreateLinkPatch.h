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
 *	@file CreateLinkPatch.h
 *	@brief �A�v���P�[�V�����N���X ��`��
 *	@author	F.Adachi
 *	@date	2005/08/02		�V�K�쐬
 *	$ID$
 */

#ifndef ___CREATELINKPATCH___
#define ___CREATELINKPATCH___

#include "DBHandleBase.h"
#include "CmdLineArg.h"
#include "AppropriateAttribute.h"
#include "ConfirmedLink.h"
#include "RoadLinkCollection.h"
#include "HighwayNodeCollection.h"
#include "LinkPatch.h"
#include "EtcLinkList.h"


/// <b>�A�v���P�[�V�����N���X</b>
class CreateLinkPatch : public DBHandleBase
{
public:
	CreateLinkPatch(void);
	~CreateLinkPatch();

	void	init( int cArgc, char** cArgv );	///< ������
	void	process(void);						///< �ϊ�����

protected:
	void	read(void);
	void	readRoadLink(void);
	void	write(void) const;

	void	checkMainLink(void) const;
	void	checkSubLink(void) const;
	void	searchSubLink( long cObjectID, long cRoadCode ) const;
	void	trackSubLink( long cObjectID, IPointPtr cpPrevPoint, long cRoadCode ) const;
	void	getNodePair( long cObjectID, IPoint** cpFromNodePoint, IPoint** cpToNodePoint ) const;
	bool	isConnected( IPoint* cpPoint, long cObjectID ) const;
	void	dispLogHeader( std::ostream& crStream ) const;
	void	dispRoadLink( std::ostream& crStream, long cObjectID, long cRoadCode, bool cFlag, const char* cpComment ) const;
	std::string	getINRoadClass( long cSindyRoadClass ) const;

private:
	std::auto_ptr<CmdLineArg>				m_pCmdLineArg;				//< �R�}���h���C�������N���X�ւ̊֘A
	std::auto_ptr<AppropriateAttribute>		m_pAppropriateAttribute;	//< �H���R�[�h�w��t�@�C���N���X�ւ̊֘A
	std::auto_ptr<ConfirmedLink>			m_pConfirmedLink;			//< �m�F�ς݃����N�W���N���X�ւ̊֘A
	std::auto_ptr<RoadLinkCollection>		m_pRoadLinkCollection;		//< ���H�����N�W���N���X�ւ̊֘A
	std::auto_ptr<HighwayNodeCollection>	m_pHighwayNodeCollection;	//< �n�C�E�F�C�m�[�h�W���N���X�ւ̊֘A
	std::auto_ptr<LinkPatch>				m_pLinkPatch;				//< �����N�p�b�`�N���X�ւ̊֘A	
};

#endif
