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

// EtcLinkList.cpp: EtcLinkList �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EtcLinkList.h"
#include <fstream>
#include <string>

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

EtcLinkList::EtcLinkList()
{

}

EtcLinkList::~EtcLinkList()
{

}


/**
 *	@brief	��������
 *	@note	�w�肳�ꂽ�X�g���[���Ƀf�[�^���o�͂���
 **/
void EtcLinkList::write( std::ostream& crStream )
{
	for (Data::const_iterator aIt = mData.begin(); aIt != mData.end(); ++aIt)
	{
		// ���ރR�[�h
		crStream << aIt->eType;

		// ���b�V���R�[�h
		crStream << "\t";
		crStream << aIt->eMeshCode;

		// �n�_�m�[�hID
		crStream << "\t";
		crStream << aIt->eFromNodeID;

		// ���_�m�[�hID��
		for (EtcLink::NodeList::const_iterator aNodeIt = aIt->eNodeList.begin(); aNodeIt != aIt->eNodeList.end(); ++aNodeIt)
		{
			crStream << "\t";
			crStream << *aNodeIt;
		}

		// �I�_�m�[�hID
		crStream << "\t";
		crStream << aIt->eToNodeID;

		crStream << std::endl;
	}
}


/**
 *	@brief	��������
 *	@note	�t�@�C�����ƕ����R�[�h�i���s�R�[�h�̂ݍl���j���w�肵�ď�������
 **/
void EtcLinkList::write(const char* cFilePath, bool cUseEUC)
{
	std::string aFunc = "EtcLinkList::write()";

	// EUC�̏ꍇ�̓o�C�i�����[�h�ŊJ��
	std::ios::openmode aOpenMode;
	if (cUseEUC) { aOpenMode = std::ios::out | std::ios::binary; }
	else		 { aOpenMode = std::ios::out; }

	std::ofstream aFout( cFilePath, aOpenMode );
	if (!aFout) { throw std::runtime_error( "R1F:" + aFunc + " : �X�}�[�gIC�p�����N���X�g�t�@�C���I�[�v���G���[" ); }
	
	write( aFout );

	aFout.close();
	
}
