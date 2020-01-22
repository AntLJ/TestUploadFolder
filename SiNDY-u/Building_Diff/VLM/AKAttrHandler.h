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

#include <sindy/schema.h>

struct AKAttrHandler {
	/**
	* @brief �R���X�g���N�^
	*/
	AKAttrHandler() {
	}
	/**
	* @brief �f�X�g���N�^
	*/
	~AKAttrHandler() {
	}
	/**
	* @brief ���O�����̔���
	* @return �ΏۊO=1, �G���[�Ȏ��=-1, �o�͑Ώ�=0
	*/
	long fnIsExcluded (
	         long cBuildAttr,  ///< [in] ��������
			 long cNoWall,     ///< [in] ���ǎɃt���O
			 long cCorridor    ///< [in] �n��L���t���O
	     );
	/**
	* @brief �����ϊ�
	*/
	void fnAttrConv (
	         long& cBuildAttr  ///< [in,out] ��������
	     );
};

// �ΏۊO=1, �G���[�Ȏ��=-1, �o�͑Ώ�=0
inline long AKAttrHandler::
fnIsExcluded ( long cBuildAttr,  ///< [in] ��������
               long cNoWall,     ///< [in] ���ǎɃt���O
               long cCorridor )  ///< [in] �n��L���t���O
{
	using namespace sindy::schema;

	if( cBuildAttr == building::bld_class::kNone && // ��������=�������E��ƒ�
		cNoWall == 1 ) { // ���ǎɃt���O��1
		return 1;
	}
	switch (cBuildAttr)
	{
	// �ϊ��ΏۊO
	case	14: return -1;   // ���̖��ǎɎ��
	case	21: return -1; // �������K�i(�폜���)
	case	22: return -1; // �������G�X�J���[�^(�폜���)
	case	24: return -1; // �y�f�X�g���A���f�b�L�G�X�J���[�^(�폜���)
	case	building::bld_class::kSiteStairs: return 1;  // �~�n�K�i
	case	building::bld_class::kArcade: return 1;  // �A�[�P�[�h
	case	building::bld_class::kTollGate: return 1;// ������
	}
	return 0;
}

inline void AKAttrHandler::
fnAttrConv ( long& cBuildAttr )  ///< [in] ��������
{
	using namespace sindy::schema;

	switch (cBuildAttr)
	{
	// ��ʂ�ϊ�
	case	building::bld_class::kUniversalTrafficStation: // �V��ʉw�i�n��j�˒n���S�ȊO�w�i�n��j(1)
	case	building::bld_class::kMonorailStation:         // ���m���[���w�i�n��j�˒n���S�ȊO�w�i�n��j(1)
	case	building::bld_class::kCableCarStation:         // �P�[�u���J�[�w�i�n��j�˒n���S�ȊO�w�i�n��j(1)
		cBuildAttr = building::bld_class::kOtherStation;
		break;
	case	building::bld_class::kForce: 
		cBuildAttr = building::bld_class::kGovmentOffice; // ���q���E�ČR�ˊ�����(5)
		break;
	case	building::bld_class::kGasStation:
		cBuildAttr = building::bld_class::kNone; //�K�\�����X�^���h�˖������E��ƒ�(0)
		break;
	}
};
