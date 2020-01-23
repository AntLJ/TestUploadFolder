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


/**
 * @brief ���b�V���֘A�̃��[�e�B���e�B�֐��Q
 * @note  ���c�[��(ReleaseDataCreate)�̃\�[�X�𗬗p(�قڃR�s�y)
 */
class CMeshcodeUtility
{
public:
	CMeshcodeUtility(void){}
	~CMeshcodeUtility(void){}

	/**
	 * @brief   �����ܓx�o�x����2�����b�V���R�[�h���擾����(���E����)
	 * @param[in]  latlon    �����ܓx�o�x
	 * @return     2�����b�V���R�[�h�Q
	 */
	static std::vector<int> GetBelong2ndMeshcode(const WKSPoint &latlon);

	/**
	 * @brief   2�����b�V���R�[�h��������ܓx�o�x���擾����
	 * @param[in]  meshcode ���b�V���R�[�h
	 * @return     �����ܓx�o�x
	 */
	static WKSPoint MeshToLL(const int &meshcode);

	/**
	 * @brief   ���ܓx�o�x����2�����b�V���R�[�h���擾����
	 * @param[in]  meshcode ���b�V���R�[�h
	 * @return     �����ܓx�o�x
	 */
	static int Get2ndMeshcode(const WKSPoint &latlon);

};

