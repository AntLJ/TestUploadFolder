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

void check_mesh(long nMeshCode);

/*
 * @brief �S�Ẵ��b�V�����`�F�b�N����B
 *
 * @param lpszMeshListFilePath [in] ���b�V�����X�g�t�@�C���p�X�B
 * @param lpszSharingInfoPath [in,optional] ���p���t�@�C���p�X�B�ȗ������ꍇ�͋��p���Ȃ��B
 */
void check_all(const char* lpszMeshListFilePath, const char* lpszSharingInfoPath = 0)
{
	using namespace rns;

	// ���ۂ̃R�[�f�B���O�ł� boost::scoped_ptr ���g���B
	file_reader* pFileReader = 0;

	try {
		if(lpszSharingInfoPath) {
			// ���p���t�@�C�����w�肳�ꂽ�Ƃ��͋��p�ǂݍ��݁B
			pFileReader = new file_sharing_reader(lpszMeshListFilePath, lpszSharingInfoPath);
		}
		else {
			// ���p���t�@�C�����w�肳��Ȃ������Ƃ��͒ʏ�ǂݍ��݁B
			pFileReader = new simple_file_reader(lpszMeshListFilePath);
		}

		// �t�@�C�����烁�b�V���R�[�h�����o���ă`�F�b�N�B
		for(long nMeshCode; *pFileReader->lock() >> nMeshCode; ) {
			check_mesh(nMeshCode);
		}

		delete pFileReader;
	}
	catch(...) {
		delete pFileReader;
		throw;
	}
}
