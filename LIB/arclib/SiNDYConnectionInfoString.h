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

#ifndef __SINDY_CONNECTION_INFO_STRING_H__
#define __SINDY_CONNECTION_INFO_STRING_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <iostream>
#include "SiNDYConnectionInfoStringI.h"

namespace sindy {

//////////////////////////////////////////////////////////////////////
// ��`
//////////////////////////////////////////////////////////////////////

/**
 * @brief SiNDY�ڑ���񕶎��񐶐��N���X
 *
 * ���[�N�X�y�[�X�̐ڑ�����A�ȉ��̕\�L�@�Ŏ����B
 * - �u�q���[�J��DB�ւ̃t���p�X�r�v ... IWorkspace�����[�J��DB�̏ꍇ�B
 * - �u�qUSER�r@�qSERVET�r(�qVERSION�r�j�v ... IWorkspace�����uDB�̏ꍇ�B
 */ 
class ConnectionInfoString : public CString, public ConnectionInfoStringI
{
public:
	ConnectionInfoString(IWorkspace* ipWorkspace) { setConnectionInfoString(ipWorkspace); }
	ConnectionInfoString(IPropertySet* ipPropertySet) { setConnectionInfoString(ipPropertySet); }
	ConnectionInfoString(LPCTSTR lpszConnectionInfo) : CString(lpszConnectionInfo) {}

protected:
// �������z�֐��̉���
	virtual void setString(LPCTSTR lpszConnectionInfo) { CString::operator=(lpszConnectionInfo); }
	virtual LPCTSTR getString() const { return operator LPCTSTR(); }
};

inline std::ostream& operator<<(std::ostream& os, const ConnectionInfoString& rString)
{
	std::locale loc(std::locale("japanese"), "C", std::locale::numeric);
	os.imbue(loc);
	return os << static_cast<LPCTSTR>(rString);
}

} // namespace sindy

#endif // __SINDY_CONNECTION_INFO_STRING_H__
