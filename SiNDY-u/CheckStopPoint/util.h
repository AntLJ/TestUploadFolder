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


namespace util {

	/**
	 * @brief ������񋓂����t�@�C����ǂݍ���
	 * @note �T�^�I�ɂ̓��b�V�����X�g�t�@�C���̓ǂݍ��݂Ɏg��
	 * @param fileName [in] ���X�g�t�@�C����
	 * @param integerList [out] �o�̓��X�g��set
	 * @retval true �ǂݍ��ݐ���
	 * @retval false �ǂݍ��ݎ��s
	 */
	inline bool loadMeshList(const CString& fileName, std::set<long>& list)
	{
		std::ifstream ifs(fileName);
			
		// �X�g���[�����擾�ł��Ȃ����false
		if( !ifs ){
			return false;
		}
		std::copy(
			std::istream_iterator<long>(ifs),
			std::istream_iterator<long>(),
			inserter(list, list.end())
		);
		if(ifs.fail() && !ifs.eof()){
			return false;
		}
		return true;
	};
}
