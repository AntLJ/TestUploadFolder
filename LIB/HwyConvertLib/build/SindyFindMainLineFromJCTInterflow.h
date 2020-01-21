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

#include <set>
#include "SindyHighwayTrace.h"

/**
 * @brief [Bug7307] �V�ێRJCT�̃P�[�X��fclpoint.txt�̏㉺����ʃR�[�h�����肷�邽�߂̒ǐՃN���X
 * @note	�E�^����ꂽ�ǐՑΏۘH���R�[�h���������N�܂��͘H���R�[�h0�̃����N������ʍs�̋t�����ɒH��A
 *			�@�n�_�A�N�Z�X�|�C���g�Ɠ����H���R�[�h�����{�������N��T��
 *			�E�ړI�̃����N�ɓ��B�������ۂ��́Atrace()��cTrace�ł͂Ȃ��Am_FoundLinks�ɂP�ȏナ���N���i�[���ꂽ���ǂ����Ŕ��肷��
 *			�E�����̃����N����������悤�ɂ��邽�߁Aexit()�ł͏I�������Atrace()�Ώۃ����N���P���Ȃ��Ȃ�܂ŒǐՂ𑱂���
 */
class SindyFindMainLineFromJCTInterflow : public SindyHighwayTrace
{
public:
	SindyFindMainLineFromJCTInterflow( long cOriginAccessPointRoadCode, long cTraceTargetRoadCode );
	~SindyFindMainLineFromJCTInterflow(void);

	virtual HRESULT trace( SindyRoadInfo *cRoadInfo, IFeaturePtr ipLink, IFeaturePtr ipNode, bool *cTrace );
	virtual HRESULT exit( SindyRoadInfo *cRoadInfo, IFeaturePtr ipNode, bool *cExit );
	
	/**
	 * @brief �������������N�Q�̎擾
	 * @return	�ǐՂ̌��ʁA�������������N�Q
	 */
	const GDBOBJLIST& getFoundLinks(void) { return m_FoundLinks; }

private:
	SindyFindMainLineFromJCTInterflow(void);	// �f�t�H���g�R���X�g���N�^���֎~

	long m_OriginAccessPointRoadCode;	// �n�_�A�N�Z�X�|�C���g�̘H���R�[�h
	long m_TraceTargetRoadCode;			// �ǐՑΏۂƂ��郊���N�̘H���R�[�h

	GDBOBJLIST	m_FoundLinks;			// �������������N�Q

	typedef long OID;
	typedef std::set<OID> OIDs;
	OIDs m_TraceEndNodes;	// �ǐՏI�[�m�[�h�Q

};
