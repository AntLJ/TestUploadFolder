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

#ifndef FILE_READER_TEST_H_
#define FILE_READER_TEST_H_

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#include <rns/file_sharing_reader.h>
#include "Misc.h"

/**
 * @ingroup unit_test
 * @brief rns::file_reader �̒���@ref unit_test�N���X�B
 */
class FileReaderTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(FileReaderTest);
	CPPUNIT_TEST(testFileReadIterator);
	CPPUNIT_TEST_SUITE_END_ABSTRACT();

public:
/// @name �e�X�g�P�[�X
//@{
	void testFileReadIterator()
	{
		std::vector<int> aExpectList, aActualList;

		// �\�z�����񓚂�p�ӁB
		{
			std::ifstream ifs(CA2WEX<MAX_PATH>(kSampleFilePath).m_psz);

			// �S���b�V���ǂݍ��ށB
			std::copy(
				std::istream_iterator<int>(ifs),
				std::istream_iterator<int>(),
				std::back_inserter(aExpectList)
			);
		}

		// ���ۂ̉񓚂�p�ӁB
		{
			file_reader_ref pFileReader(create_file_reader(kSampleFilePath));
			CPPUNIT_ASSERT(pFileReader.get());

			// �S���b�V���ǂݍ��ށB
			std::copy(
				rns::file_read_iterator<int>(*pFileReader),
				rns::file_read_iterator<int>(),
				std::back_inserter(aActualList)
			);
		}

		// �񓚂��r�B
		CPPUNIT_ASSERT(aExpectList == aActualList);
	}
//@}

protected:
	typedef std::auto_ptr<rns::file_reader> file_reader_ref;

// �������z�֐�
	/**
	 * @brief ���̍H�v���Ȃ��t�@�C����ǂݍ��� file_reader ���쐬����B
	 *
	 * @param lpszFilePath [in] �t�@�C���p�X�B
	 * @return file_reader �B
	 */
	virtual file_reader_ref create_file_reader(const char* lpszFilePath) const = 0;
};

#endif // FILE_READER_TEST_H_
