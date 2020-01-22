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

#pragma once

#include "ConvertTable.h"

/**
 * @brief ���R�[�h�`�F�b�N�p
 */
class CBaseRow
{
public:
	CBaseRow(){}
	virtual ~CBaseRow(){}
	
protected:
	/**
	 * @brief �G���[�o�͗pID�擾�֐�
	 * @param cConvTableInfo [in] �ϊ��e�[�u�����
	 * @param ipRow [in] ���R�[�h
	 * @return ID������
	 */
	CString getID(const CConvTableInfo& cConvTableInfo, const _IRowPtr& ipRow);
};

