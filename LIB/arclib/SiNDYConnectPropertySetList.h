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

#ifndef __SINDY_CONNECT_PROPERTYSET_LIST_H__
#define __SINDY_CONNECT_PROPERTYSET_LIST_H__

#include <vector>
#include "SiNDYConnectPropertySet.h"

class SiNDYConnectPropertySetList : public std::vector<SiNDYConnectPropertySet>
{
public:
	bool setFromRegistry();
	bool writeToRegistry() const;

	template <typename TComboBox>
	void extractToComboBox(TComboBox& rComboBox)
	{
		// ���ݑI����Ԃ̐ڑ��v���p�e�B��ێ��A�����ăR���{�{�b�N�X���N���A
		CString strText;
		rComboBox.GetWindowText(strText);
		rComboBox.ResetContent();

		// �ڑ��v���p�e�B�ꗗ���R���{�{�b�N�X�ɕ���
		for(SiNDYConnectPropertySetList::const_iterator it = begin(); it != end(); ++it) {
			const SiNDYConnectPropertySet& rPropretySet = *it;
			rComboBox.AddString(rPropretySet.getPropertiesString());
		}
		
		// �ێ����ꂽ�ڑ��v���p�e�B�����݂���ꍇ�A�����ڑ��v���p�e�B��I����ԂƂ���悤���݂�
		if(strText.IsEmpty() && rComboBox.GetCount() > 0) {
			rComboBox.SetCurSel(0);
		}
		else {
			rComboBox.SetWindowText(strText);
		}
	}
};

#endif // __SINDY_CONNECT_PROPERTYSET_LIST_H__
