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

// DRMALoadProtocol.cpp: DRMALoadProtocol �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include <fstream>
#include <sstream>

#include "DRMADevPlant.h"
#include "DRMAOriginalValues.h"
#include "DRMARecord.h"
#include "DRMALoadProtocol.h"

using namespace drma;

struct DRMARecordFormat {
	_DRMADec eRecordId[2];
};

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

DRMALoadProtocol::DRMALoadProtocol() :
mLoadComplete(false)
{
}

//////////////////////////////////////////////////////////////////////
// �J��
//////////////////////////////////////////////////////////////////////

/**
 * @brief �w�肵���f�B���N�g�����ɑ��݂���A�w�肵�����b�V���R�[�h��DRMA�t�@�C�����J��
 *
 * @note �J���̂Ɏ��s�����ꍇ�A�W���G���[�o�͂ɃG���[���o�͂���B
 *
 * @param cDirectoryName [in] �f�B���N�g����
 * @param cMeshCode [in] ���b�V���R�[�h
 * @param cSuffix [in] �g���q
 * @retval true ����
 * @retval false ���s
 */
bool DRMALoadProtocol::load(const char* cDirectoryName, int cMeshCode, const char* cSuffix)
{
	std::stringstream aStream;

	aStream << cDirectoryName;

	// �f�B���N�g����؂蕶�����t���Ă��Ȃ�������t����
	int aLen = strlen(cDirectoryName);
	if(aLen && cDirectoryName[aLen-1] != dev_plant::_pathChr) {
		aStream << dev_plant::_pathChr;
	}

	// ���b�V���R�[�h���
	aStream
		<< int(cMeshCode / 100) //< �P�����b�V���R�[�h
		<< dev_plant::_pathChr //< �p�X��؂蕶��
		<< cMeshCode; //< �Q�����b�V���R�[�h

	if(cSuffix) {
		aStream << cSuffix; //< �g���q
	}

	return load(aStream.str().c_str());
}

/**
 * @brief �w�肵�����̂�DRMA�t�@�C�����J��
 *
 * @note �J���̂Ɏ��s�����ꍇ�A�W���G���[�o�͂ɃG���[���o�͂���B
 *
 * @param cFileName [in] �t�@�C����
 * @retval true ����
 * @retval false ���s
 */
bool DRMALoadProtocol::load(const char* cFileName)
{
	std::ifstream aFileStream(cFileName, std::ios::binary);
	if(! aFileStream) {
		std::cerr << "�t�@�C��\"" << cFileName << "\"���J���Ȃ�" << std::endl;
		return false;
	}

	return load(aFileStream);
}

/**
 * @brief �w�肵���X�g���[������DRMA�f�[�^��ǂݍ���
 *
 * @param cStream [in] DRMA�f�[�^�X�g���[��
 * @retval true ����
 * @retval false ���s
 */
bool DRMALoadProtocol::load(std::istream& cStream)
{
	// �J���Ă��������
	close();

	char aRecord[256];

	for(int i = 1; ! (mLoadComplete || cStream.read(aRecord, 256).eof()); i++) {
		// ���R�[�hID�擾�p�\���̂Ɍ^�ϊ�
		const DRMARecordFormat* pFormat = reinterpret_cast<DRMARecordFormat*>(aRecord);

		// ���R�[�hID�擾
		int aRecordId;
		VALID(DRMAItemConverter::setValueTo(pFormat->eRecordId, sizeof(pFormat->eRecordId), aRecordId));

		// ���R�[�h�n���h�����O
		if(! append(aRecordId, aRecord)) {
			std::cerr << "���R�[�h #" << i << " ID:" << aRecordId << " �̃n���h�����O�Ɏ��s" << std::endl;
			return false;
		}
	}

	setComplete();

	return true;
}

//////////////////////////////////////////////////////////////////////
// ����
//////////////////////////////////////////////////////////////////////

/**
 * @brief �ǂݍ���DRMA�f�[�^���������
 */
void DRMALoadProtocol::close()
{
	mLoadComplete = false;
	clear();
}
