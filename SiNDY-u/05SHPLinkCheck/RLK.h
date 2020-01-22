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
#include<vector>
#include"Def.h"

class CRLK
{
public:
	CRLK(IFeatureClassPtr ipFeatureClass);
	~CRLK(void);

	/**
	 * @brief �����N����vector�Ɋi�[
	 * @param rLinkSet			[in/out]	vector�R���e�i
	 * @param rLinkeFileName	[in]		�m�[�h�t�@�C����
	 * @param bWalkOpt			[in]		-w�I�v�V�����L��
	 * @retval true							��������
	 * @retval false						�������s
	 */
	bool GetInfo(std::vector<LINK> & rLinkSet, std::wstring rLinkeFileName, bool bWalkOpt);

	
private:
    IFeatureClassPtr m_ipFeatureClass;			//̨�����׽
	

};


