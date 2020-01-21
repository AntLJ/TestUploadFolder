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

#include "stdafx.h"
#include "AddrSchema.h"

namespace address {

namespace schema {

// �Z���}�X�^
namespace areacode_master {

	extern const _TCHAR kTableName[]		= _T("AREACODE_MASTER");	//!< �e�[�u����

	extern const _TCHAR kKenCode[]			= _T("KEN_CODE");			//!< �s���{���R�[�h
	extern const _TCHAR kShiCode[]			= _T("SHI_CODE");			//!< �s�撬���R�[�h
	extern const _TCHAR kOazaCode[]			= _T("OAZA_CODE");			//!< �厚�E�ʏ̃R�[�h 
	extern const _TCHAR kAzaCode[]			= _T("AZA_CODE");			//!< ���E���ڃR�[�h
	extern const _TCHAR kNewKenCode[]		= _T("NEW_KEN_CODE");		//!< �V�s���{���R�[�h
	extern const _TCHAR kNewShiCode[]		= _T("NEW_SHI_CODE");		//!< �V�s��S�����R�[�h
	extern const _TCHAR kNewOazaCode[]		= _T("NEW_OAZA_CODE");		//!< �V�厚�E�ʏ̃R�[�h
	extern const _TCHAR kNewAzaCode[]		= _T("NEW_AZA_CODE");		//!< �V���E���ڃR�[�h
	extern const _TCHAR kZipCode[]			= _T("ZIP_CODE");			//!< �X�֔ԍ�
	extern const _TCHAR kZipCount[]			= _T("ZIP_COUNT");			//!< �X�֔ԍ���
	extern const _TCHAR kSeireishiFlag[]	= _T("SEIREISHI_FLAG");		//!< ���ߎw��s�s�t���O
	extern const _TCHAR kKenKana[]			= _T("KEN_KANA");			//!< �s���{�����J�i�ǂ�
	extern const _TCHAR kShiKana[]			= _T("SHI_KANA");			//!< �s��S�������J�i�ǂ�
	extern const _TCHAR kOazaKana[]			= _T("OAZA_KANA");			//!< �厚�E�ʏ̖��J�i�ǂ�
	extern const _TCHAR kAzaKana[]			= _T("AZA_KANA");			//!< �����E���ڃJ�i�ǂ�
	extern const _TCHAR kKenKanji[]			= _T("KEN_KANJI");			//!< �s���{����
	extern const _TCHAR kShiKanji[]			= _T("SHI_KANJI");			//!< �s��S������
	extern const _TCHAR kOazaKanji[]		= _T("OAZA_KANJI");			//!< �厚�E�ʏ̖�
	extern const _TCHAR kAzaKanji[]			= _T("AZA_KANJI");			//!< �����E����
	extern const _TCHAR kKenKanjiType[]		= _T("KEN_KANJI_TYPE");		//!< �m�O�����n�s���{����
	extern const _TCHAR kShiKanjiType[]		= _T("SHI_KANJI_TYPE");		//!< �m�O�����n�s��S������
	extern const _TCHAR kOazaKanjiType[]	= _T("OAZA_KANJI_TYPE");	//!< �m�O�����n�厚�E�ʏ̖�
	extern const _TCHAR kAzaKanjiType[]		= _T("AZA_KANJI_TYPE");		//!< �m�O�����n�����E����
	extern const _TCHAR kShikoDate[]		= _T("SHIKO_DATE");			//!< �{�s�N��
	extern const _TCHAR kHaishiDate[]		= _T("HAISHI_DATE");		//!< �p�~�N��
	extern const _TCHAR kTyoumeiDate[]		= _T("TYOUMEI_DATE");		//!< �����ύX�N��
	extern const _TCHAR kZipCodeDate[]		= _T("ZIP_CODE_DATE");		//!< �X�֔ԍ��ύX�N��
	extern const _TCHAR kJukyoDate[]		= _T("JUKYO_DATE");			//!< �Z���\�����{�N��
	extern const _TCHAR kJukyoFlag[]		= _T("JUKYO_FLAG");			//!< �Z���\�����{�t���O
	extern const _TCHAR kRecordFlag[]		= _T("RECORD_FLAG");		//!< ���R�[�h�敪
	extern const _TCHAR kTsushoFlag[]		= _T("TSUSHO_FLAG");		//!< �ʏ̃t���O
	extern const _TCHAR kTorinaFlag[]		= _T("TORINA_FLAG");		//!< �ʂ薼����
	extern const _TCHAR kOazaFlag[]			= _T("OAZA_FLAG");			//!< �厚�E���t���O
	extern const _TCHAR kAzaFlag[]			= _T("AZA_FLAG");			//!< ���E�����t���O
	extern const _TCHAR kOldZipCode[]		= _T("OLD_ZIP_CODE");		//!< ��5���X�֔ԍ�
	extern const _TCHAR kShuseiCode[]		= _T("SHUSEI_CODE");		//!< �C���R�[�h
} // areacode_master

// �S�p�ǂ݉����e�[�u��
namespace areacode_kana {

	extern const _TCHAR kTableName[]	= _T("AREACODE_KANA");		//!< �e�[�u����

	extern const _TCHAR kKenCode[]		= _T("KEN_CODE");			//!< �s���{���R�[�h
	extern const _TCHAR kShiCode[]		= _T("SHI_CODE");			//!< �s�撬���R�[�h
	extern const _TCHAR kOazaCode[]		= _T("OAZA_CODE");			//!< �厚�E�ʏ̃R�[�h 
	extern const _TCHAR kAzaCode[]		= _T("AZA_CODE");			//!< ���E���ڃR�[�h
	extern const _TCHAR kKenKanaD[]		= _T("KEN_KANA_D");			//!< �s���{�����S�p�J�i�ǂ�
	extern const _TCHAR kShiKanaD[]		= _T("SHI_KANA_D");			//!< �s��S�������S�p�J�i�ǂ�
	extern const _TCHAR kOazaKanaD[]	= _T("OAZA_KANA_D");		//!< �厚�E�ʏ̖��S�p�J�i�ǂ�
	extern const _TCHAR kAzaKanaD[]		= _T("AZA_KANA_D");			//!< �����E���ڑS�p�J�i�ǂ�
}// areacode_kana

}// schema

}// address