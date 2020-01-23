#include "StdAfx.h"
#include "IMEStatus.h"
#pragma comment(lib, "imm32.lib")

// IMEの入力モードを制御する
void CIMEStatus::setIMEConversionStatus(CONVERSIONMODE mode){
	// 変更なしの場合はすぐにreturn
	if(mode == NOCHANGE)
		return;

	if(m_himc){
		DWORD dwConversion = 0;
		DWORD dwSentence = 0;
		::ImmGetConversionStatus(m_himc, &dwConversion, &dwSentence);
		switch(mode){
		case HIRAGANA: // 全角ひらがな
			::ImmSetOpenStatus(m_himc, TRUE);
			dwConversion |= IME_CMODE_NATIVE | IME_CMODE_FULLSHAPE;
			dwConversion &= ~IME_CMODE_KATAKANA;
			break;
		case KATAKANA_FULL: // 全角カタカナ
			::ImmSetOpenStatus(m_himc, TRUE);
			dwConversion |= IME_CMODE_NATIVE | IME_CMODE_FULLSHAPE |IME_CMODE_KATAKANA;
			break;
		case KATAKANA_HALF: // 半角カタカナ
			::ImmSetOpenStatus(m_himc, TRUE);
			dwConversion |= IME_CMODE_NATIVE | IME_CMODE_KATAKANA;
			dwConversion &= ~IME_CMODE_FULLSHAPE;
			break;
		case ALPHA_FULL: // 全角英数
			::ImmSetOpenStatus(m_himc, TRUE);
			dwConversion |= IME_CMODE_FULLSHAPE;
			dwConversion &= ~(IME_CMODE_NATIVE |IME_CMODE_KATAKANA);
			break;
		case ALPHA_HALF: // 半角英数
			::ImmSetOpenStatus(m_himc, FALSE);
			dwConversion &= ~(IME_CMODE_NATIVE | IME_CMODE_FULLSHAPE |IME_CMODE_KATAKANA);
			break;
		default:
			break;
		}
		::ImmSetConversionStatus(m_himc, dwConversion, dwSentence);
	}
}
