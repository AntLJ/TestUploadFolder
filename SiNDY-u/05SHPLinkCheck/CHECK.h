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
#include<vector>
#include"Def.h"

/*
typedef struct LinkInfo{
	long ObjectID,FromNodeID,ToNodeID;		//�I�u�W�F�N�gID�A�n�_�m�[�hID�A�I�_�m�[�hID 
	WKSPoint FirstPoint,EndPoint;           //�n�_���W�A�I�_���W
}LINK;
*/
class CCHECK
{
public:
	CCHECK(void);
	~CCHECK(void);

	/**
	 * @brief �����N���퐫�`�F�b�N
	 * @param rNodeSet		[in]	�m�[�h���
	 * @param rLinkSort		[in]	�����N���
	 * @param rSecondMesh	[in]	���b�V���R�[�h
	 * @param bWalkOpt		[in]	-w�I�v�V�����L��
	 * @retval true					��������
	 * @retval false				�������s
	 */
	bool CheckLinkSHP(std::map<std::pair<long, long>, WKSPoint> & rNodeSet, std::vector<LINK> & rLinkSort,long rSecondMesh, bool bWalkOpt);
};
