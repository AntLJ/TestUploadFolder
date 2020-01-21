#include "stdafx.h"
#include "AddrRec.h"

namespace addr
{

bool judgeAddrChar(const CString& code)
{
	return !(code.SpanIncluding(_T("0123456789")) == code);
}

AddrRec::AddrRec(const CString& jusyocode, const IGeometryPtr& geo)
	: m_geo(geo), m_isMatchTarget(true)
{
	// �Z���R�[�h�̊e�K�w�̃R�[�h�ɕ���
	uh::tstring code = jusyocode;
	m_citycode = (code.size() >= 5)  ? code.substr(0, 5).c_str() : _T("");
	m_addrcode = (code.size() >= 11) ? code.substr(5, 6).c_str() : _T("");
	m_chibancode = (code.size() >= 16) ? code.substr(11, 5).c_str() : _T("");
	m_goucode = (code.size() >= 20) ? code.substr(16, 4).c_str() :  _T("");

	// �Z���R�[�h�̌�����ݒ�
	switch( code.size() )
	{
		case 11:
			m_addrDigit = AddrDigit::addr;
			break;
		case 16:
			m_addrDigit = AddrDigit::chiban;
			break;
		case 20:
			m_addrDigit = AddrDigit::gou;
			break;
		default:
			m_addrDigit = AddrDigit::Other;
			break;
	}

	// �n�ԃR�[�h�E���R�[�h���ꂼ��A�����t�����ǂ����𔻒�
	m_isChibanChar = judgeAddrChar(m_chibancode);
	m_isGouChar = judgeAddrChar(m_goucode);
}

} 	// namespace addr