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

#ifndef SINDY_GEODATABASE_H_
#define SINDY_GEODATABASE_H_

#include <rns/sstream.h>
#include <atl2/exception.h>

namespace sindy {

/**
 * @brief ���[�U���Ƃ��āureference�v���w�肵�Ȃ��ƊJ���Ȃ���������Ȃ��t�B�[�`���N���X���J���B
 *
 * @param ipFeatureWorkspace [in] �t�B�[�`�����[�N�X�y�[�X�B
 * @param lpszName [in] �t�B�[�`���N���X���B
 * @return �t�B�[�`���N���X�B�J���Ȃ������ꍇ��null�B
 *
 * @note
 * ���̊֐��͗�O���S�ł���B
 */
inline IFeatureClassPtr open_ref_featureclass(IFeatureWorkspace* ipFeatureWorkspace, LPCTSTR lpszName) throw()
{
	_ASSERT(ipFeatureWorkspace);
	_ASSERT(lpszName);

	IFeatureClassPtr ipFeatureClass;
	if(ipFeatureWorkspace->OpenFeatureClass(CComBSTR(lpszName), &ipFeatureClass) != S_OK || ipFeatureClass == 0) {
		LPCTSTR p = ::_tcschr(lpszName, _T('.'));

		rns::tostringstream oss;
		std::locale loc(std::locale("japanese"), "C", std::locale::numeric);
		oss.imbue(loc);
		oss << _T("reference.") << (p ? p + 1 : lpszName);
		if(ipFeatureWorkspace->OpenFeatureClass(CComBSTR(oss.str().c_str()), &ipFeatureClass) != S_OK)
			return 0;
	}

	return ipFeatureClass;
}

} // namespace sindy

#endif // SINDY_GEODATABASE_H_
