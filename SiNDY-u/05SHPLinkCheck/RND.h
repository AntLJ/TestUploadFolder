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
#include<map>


class CRND
{
public:
	CRND(IFeatureClassPtr ipFeatureClass);  
	~CRND(void);

	/**
	 * @brief �m�[�h����map�Ɋi�[
	 * @param rSortSet		[in/out]	�L�[��OBJECTID�EDIVID�A�v�f�����W�Ƃ���map�R���e�i
	 * @param rNodeFileName	[in]		�m�[�h�t�@�C����
	 * @param bWalkOpt		[in]		-w�I�v�V�����L��
	 * @retval true						��������
	 * @retval false					�������s
	 */
	bool Point2Map(std::map<std::pair<long, long>, WKSPoint>& rSortSet, std::wstring rNodeFileName, bool bWalkOpt);

private:
	IFeatureClassPtr m_ipFeatureClass;			//̨�����׽
};
