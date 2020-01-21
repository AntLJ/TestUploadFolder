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

#ifndef ADDRESS_SCHEMA_H_
#define ADDRESS_SCHEMA_H_

#include "stdafx.h"

namespace address {

namespace schema {

// �Z���}�X�^
namespace areacode_master {

	extern const _TCHAR kTableName[];		//!< �e�[�u����

	extern const _TCHAR kKenCode[];			//!< �s���{���R�[�h
	extern const _TCHAR kShiCode[];			//!< �s�撬���R�[�h
	extern const _TCHAR kOazaCode[];		//!< �厚�E�ʏ̃R�[�h
	extern const _TCHAR kAzaCode[];			//!< ���E���ڃR�[�h
	extern const _TCHAR kNewKenCode[];		//!< �V�s���{���R�[�h
	extern const _TCHAR kNewShiCode[];		//!< �V�s��S�����R�[�h
	extern const _TCHAR kNewOazaCode[];		//!< �V�厚�E�ʏ̃R�[�h
	extern const _TCHAR kNewAzaCode[];		//!< �V���E���ڃR�[�h
	extern const _TCHAR kZipCode[];			//!< �X�֔ԍ�
	extern const _TCHAR kZipCount[];		//!< �X�֔ԍ���
	extern const _TCHAR kSeireishiFlag[];	//!< ���ߎw��s�s�t���O
	extern const _TCHAR kKenKana[];			//!< �s���{�����J�i�ǂ�
	extern const _TCHAR kShiKana[];			//!< �s��S�������J�i�ǂ�
	extern const _TCHAR kOazaKana[];		//!< �厚�E�ʏ̖��J�i�ǂ�
	extern const _TCHAR kAzaKana[];			//!< �����E���ڃJ�i�ǂ�
	extern const _TCHAR kKenKanji[];		//!< �s���{����
	extern const _TCHAR kShiKanji[];		//!< �s��S������
	extern const _TCHAR kOazaKanji[];		//!< �厚�E�ʏ̖�
	extern const _TCHAR kAzaKanji[];		//!< �����E����
	extern const _TCHAR kKenKanjiType[];	//!< �m�O�����n�s���{����
	extern const _TCHAR kShiKanjiType[];	//!< �m�O�����n�s��S������
	extern const _TCHAR kOazaKanjiType[];	//!< �m�O�����n�厚�E�ʏ̖�
	extern const _TCHAR kAzaKanjiType[];	//!< �m�O�����n�����E����
	extern const _TCHAR kShikoDate[];		//!< �{�s�N��
	extern const _TCHAR kHaishiDate[];		//!< �p�~�N��
	extern const _TCHAR kTyoumeiDate[];		//!< �����ύX�N��
	extern const _TCHAR kZipCodeDate[];		//!< �X�֔ԍ��ύX�N��
	extern const _TCHAR kJukyoDate[];		//!< �Z���\�����{�N��
	extern const _TCHAR kJukyoFlag[];		//!< �Z���\�����{�t���O
	extern const _TCHAR kRecordFlag[];		//!< ���R�[�h�敪
	extern const _TCHAR kTsushoFlag[];		//!< �ʏ̃t���O
	extern const _TCHAR kTorinaFlag[];		//!< �ʂ薼����
	extern const _TCHAR kOazaFlag[];		//!< �厚�E���t���O
	extern const _TCHAR kAzaFlag[];			//!< ���E�����t���O
	extern const _TCHAR kOldZipCode[];		//!< ��5���X�֔ԍ�
	extern const _TCHAR kShuseiCode[];		//!< �C���R�[�h


	// �O�����
	namespace kanji_type {
		enum ECode {
			kNone	= 0,	//!< �����������Ă��Ȃ�
			kJis	= 1,	//!< JIS�̑�P�E�Q�����݂̂ō\��
			kOther	= 2,	//!< JIS�ȊO�̕������܂܂��
		};
	}

	// �厚�E���E�����t���O
	namespace oaza_aza_flag {
		enum ECode {
			kNone	= 0,		//!< �������Ȃ�
			kOaza	= 1,		//!< ��������/�J�i���̂̐擪���u�厚�v/�u����ށv
			kAza	= 2,		//!< ��������/�J�i���̂̐擪���u���v/�u��ށv
			kKoaza	= 3,		//!< ��������/�J�i���̂̐擪���u�����v/�u���ށv
		};
	}

} // areacode_master

// �S�p�ǂ݉����e�[�u��
namespace areacode_kana {

	extern const _TCHAR kTableName[];		//!< �e�[�u����

	extern const _TCHAR kKenCode[];			//!< �s���{���R�[�h
	extern const _TCHAR kShiCode[];			//!< �s�撬���R�[�h
	extern const _TCHAR kOazaCode[];		//!< �厚�E�ʏ̃R�[�h 
	extern const _TCHAR kAzaCode[];			//!< ���E���ڃR�[�h
	extern const _TCHAR kKenKanaD[];		//!< �s���{�����S�p�J�i�ǂ�
	extern const _TCHAR kShiKanaD[];		//!< �s��S�������S�p�J�i�ǂ�
	extern const _TCHAR kOazaKanaD[];		//!< �厚�E�ʏ̖��S�p�J�i�ǂ�
	extern const _TCHAR kAzaKanaD[];		//!< �����E���ڑS�p�J�i�ǂ�
} // areacode_kana

} // schema

} // address

#endif // ADDRESS_SCHEMA_H_
