#include "StdAfx.h"
#include "IMEStatus.h"
#pragma comment(lib, "imm32.lib")

// IME�̓��̓��[�h�𐧌䂷��
void CIMEStatus::setIMEConversionStatus(CONVERSIONMODE mode){
	// �ύX�Ȃ��̏ꍇ�͂�����return
	if(mode == NOCHANGE)
		return;

	if(m_himc){
		DWORD dwConversion = 0;
		DWORD dwSentence = 0;
		::ImmGetConversionStatus(m_himc, &dwConversion, &dwSentence);
		switch(mode){
		case HIRAGANA: // �S�p�Ђ炪��
			::ImmSetOpenStatus(m_himc, TRUE);
			dwConversion |= IME_CMODE_NATIVE | IME_CMODE_FULLSHAPE;
			dwConversion &= ~IME_CMODE_KATAKANA;
			break;
		case KATAKANA_FULL: // �S�p�J�^�J�i
			::ImmSetOpenStatus(m_himc, TRUE);
			dwConversion |= IME_CMODE_NATIVE | IME_CMODE_FULLSHAPE |IME_CMODE_KATAKANA;
			break;
		case KATAKANA_HALF: // ���p�J�^�J�i
			::ImmSetOpenStatus(m_himc, TRUE);
			dwConversion |= IME_CMODE_NATIVE | IME_CMODE_KATAKANA;
			dwConversion &= ~IME_CMODE_FULLSHAPE;
			break;
		case ALPHA_FULL: // �S�p�p��
			::ImmSetOpenStatus(m_himc, TRUE);
			dwConversion |= IME_CMODE_FULLSHAPE;
			dwConversion &= ~(IME_CMODE_NATIVE |IME_CMODE_KATAKANA);
			break;
		case ALPHA_HALF: // ���p�p��
			::ImmSetOpenStatus(m_himc, FALSE);
			dwConversion &= ~(IME_CMODE_NATIVE | IME_CMODE_FULLSHAPE |IME_CMODE_KATAKANA);
			break;
		default:
			break;
		}
		::ImmSetConversionStatus(m_himc, dwConversion, dwSentence);
	}
}
