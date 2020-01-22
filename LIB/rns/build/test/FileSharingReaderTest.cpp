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

#include "stdafx.h"
#include <io.h>
#include <fcntl.h>
#include <process.h>
#include <list>
#include <iostream>
#include <boost/scoped_ptr.hpp>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#include <rns/file_sharing_reader.h>
#include "FileReaderTest.h"
#include "Misc.h"

#ifndef DOXYGEN
namespace {
#endif // DOXYGEN

/**
 * @brief �w�肵�������ʂ�Ƀt�@�C�����J���邱�Ƃ����؂���B
 *
 * @param lpszPath [in] �t�@�C���p�X�B
 * @param nFlag [in] _open() �̑�2�����B
 * @retval 0 �t�@�C�����J�����Ƃ��ł����B
 * @retval !0 errno
 */
int check_open(const char* lpszPath, int nFlag)
{
	int fh = ::_open(lpszPath, nFlag);

	if(fh != -1) {
		::_close(fh);
		return 0;
	}

	return errno;
}

/**
 * @brief @ref file_sharing_info_file�̐����������؂���B
 *
 * @param lpszShrPath [in] @ref file_sharing_info_file�p�X�B
 * @param nExpectPos [in] �R��ׂ��t�@�C���|�W�V�����B
 * @param nExpectRefCount [in] �R��ׂ��Q�Ɛ��B
 * @param lpszExpectPath [in] �R��ׂ��p�X������B
 */
void check_shr(const char* lpszShrPath, long nExpectPos, const int nExpectRefCount, const char* lpszExpectPath)
{
	std::ifstream ifs(CA2WEX<MAX_PATH>(lpszShrPath), std::ios::binary | std::ios::in);
	CPPUNIT_ASSERT(ifs.is_open());

	unsigned char nMajor;
	unsigned char nMinor;
	unsigned __int64 nActualPos;
	int nActualRefCount;
	char szActualPath[_MAX_PATH] = { 0 };
	ifs.read(reinterpret_cast<char*>(&nMajor), sizeof(nMajor));
	ifs.read(reinterpret_cast<char*>(&nMinor), sizeof(nMinor));
	ifs.read(reinterpret_cast<char*>(&nActualPos), sizeof(nActualPos));
	ifs.read(reinterpret_cast<char*>(&nActualRefCount), sizeof(int));
	ifs.read(szActualPath, ::strlen(lpszExpectPath));
	CPPUNIT_ASSERT(ifs);

	CPPUNIT_ASSERT_EQUAL(nExpectPos, static_cast<long>(nActualPos));
	CPPUNIT_ASSERT_EQUAL(nExpectRefCount, nActualRefCount);
	CPPUNIT_ASSERT_EQUAL(std::string(lpszExpectPath), std::string(szActualPath));
}

typedef std::list<int> MeshCodeList;

/**
 * @brief read_loop() �̈���
 */
struct ReadLoopArgs
{
	const char* m_lpszFilePath; ///< ���p�t�@�C���p�X�B
	MeshCodeList* m_pMeshCodeList; ///< ���b�V���R�[�h�ꗗ�B
};

/**
 * @brief test_thread() ��1�X���b�h�B
 *
 * @param vpArgs [in] �����B
 * @return 0
 */
unsigned int __stdcall read_loop(void* vpArgs)
{
	const ReadLoopArgs* pArgs = static_cast<ReadLoopArgs*>(vpArgs);

	rns::file_sharing_reader aFileSharingReader(pArgs->m_lpszFilePath);

	std::copy(
		rns::file_read_iterator<int>(aFileSharingReader),
		rns::file_read_iterator<int>(),
		std::back_inserter(*pArgs->m_pMeshCodeList)
	);

	return 0;
}

/**
 * @brief �}���`�X���b�h�ɂ��r�������B
 *
 * @param lpszFilePath [in] ���p�t�@�C���p�X�B
 */
void test_thread(const char* lpszFilePath)
{
	MeshCodeList aExpectList;

	{
		std::ifstream ifs(CA2WEX<MAX_PATH>(lpszFilePath).m_psz);

		// �S���b�V���ǂݍ��ށB
		std::copy(
			std::istream_iterator<int>(ifs),
			std::istream_iterator<int>(),
			std::back_inserter(aExpectList)
		);
	}

	static const int s_nThreadCount = 6;

	// �X���b�h�𗧂��グ��B
	MeshCodeList aMeshCodeList[s_nThreadCount];
	ReadLoopArgs aArgs[s_nThreadCount];

	HANDLE hThreads[s_nThreadCount];
	for(int i = 0; i < s_nThreadCount; i++) {
		unsigned thrdaddr;
		aArgs[i].m_lpszFilePath = lpszFilePath;
		aArgs[i].m_pMeshCodeList = &aMeshCodeList[i];
		hThreads[i] = reinterpret_cast<HANDLE>(::_beginthreadex(NULL, 0, read_loop, &aArgs[i], 0, &thrdaddr));
		CPPUNIT_ASSERT(hThreads[i]);
	}

	unsigned int nActualCount = 0;

	// �S�ẴX���b�h�̏I����҂B
	for(int i = 0; i < s_nThreadCount; i++) {
		::WaitForSingleObject(hThreads[i], INFINITE);
		::CloseHandle(hThreads[i]);
		nActualCount += aMeshCodeList[i].size();
//		std::cout << aMeshCodeList[i].size() << std::endl;
	}

	// �d�����������Ȃ��A���b�V���R�[�h�����o�������Ƃ��m�F����B
	CPPUNIT_ASSERT_EQUAL(aExpectList.size(), nActualCount);

	// ���������ԂŃ��b�V�����R�[�h�����o�������Ƃ��m�F����B
	for(MeshCodeList::const_iterator it = aExpectList.begin(); it != aExpectList.end(); ++it) {
		const long nMeshCode = *it;

		bool bHas[s_nThreadCount];
		
		for(int i = 0; i < s_nThreadCount; i++) {
			bHas[i] = ! aMeshCodeList[i].empty() && aMeshCodeList[i].front() == nMeshCode;
			if(bHas[i]) {
				aMeshCodeList[i].pop_front();
			}
		}

		CPPUNIT_ASSERT_EQUAL(1, std::count(bHas, bHas + s_nThreadCount, true));
	}

	for(int i = 0; i < s_nThreadCount; i++) {
		CPPUNIT_ASSERT_EQUAL(0U, aMeshCodeList[i].size());
	}
}

#ifndef DOXYGEN
} // anonymous namespace
#endif // DOXYGEN

/**
 * @ingroup unit_test
 * @brief rns::file_sharing_reader ��@ref unit_test�N���X�i���[�J���Łj�B
 */
class FileSharingReaderTest : public FileReaderTest
{
	CPPUNIT_TEST_SUB_SUITE(FileSharingReaderTest, FileReaderTest);
	CPPUNIT_TEST(testExceptions);
	CPPUNIT_TEST(testExceptions);
	CPPUNIT_TEST(testConstruct);
	CPPUNIT_TEST(testConstruct2);
	CPPUNIT_TEST(testOpen);
	CPPUNIT_TEST(testOpen2);
	CPPUNIT_TEST(testAbsolutePath);
	CPPUNIT_TEST(testUseCount);
	CPPUNIT_TEST(testShare);
	CPPUNIT_TEST(testThread);
	CPPUNIT_TEST_SUITE_END();

public:
	virtual void setUp()
	{
		::remove(kSampleFileShrPath);
		::remove(kSampleFileShrPath2);
		CPPUNIT_ASSERT_EQUAL(-1, ::_access(kSampleFileShrPath, 0));
		CPPUNIT_ASSERT_EQUAL(-1, ::_access(kSampleFileShrPath2, 0));
	}

	virtual void tearDown()
	{
		::remove(kSampleFileShrPath);
		::remove(kSampleFileShrPath2);
	}

/// @name �e�X�g�P�[�X
//@{
	/**
	 * @brief �e���O�̌����B
	 */
	void testExceptions()
	{
		// �t�@�C�����p���t�@�C���p�X���f�B���N�g�����w���Ă����B
		CPPUNIT_ASSERT_THROW(
			rns::file_sharing_reader(kSampleFilePath, kDataFolderPath),
			rns::file_sharing_reader::cant_create_sharing_info_file
		);

		// �t�@�C�����p���t�@�C�������݂��Ȃ����߁A���p�t�@�C�������߂��Ȃ��B
		CPPUNIT_ASSERT_THROW(
			rns::file_sharing_reader(0, kSampleFileShrPath),
			rns::file_sharing_reader::not_existing_sharing_info_file
		);

		// �_�~�[�̋��p���t�@�C�����쐬����B
		rns::file_sharing_reader(kSampleFile2Path, kSampleFileShrPath);

		// ���p��񂪍��v���Ȃ��B
		CPPUNIT_ASSERT_THROW(
			rns::file_sharing_reader(kSampleFilePath, 0),
			rns::file_sharing_reader::not_match_sharing_info
		);

		// ���p��񂪎w���t�@�C�������݂��Ȃ��B
		CPPUNIT_ASSERT_THROW(
			rns::file_sharing_reader(0, kSampleFileShrPath),
			rns::file_sharing_reader::not_existing_sharing_file
		);
	}

	/**
	 * @brief rns::file_sharing_reader::file_sharing_reader() �̌����B
	 */
	void testConstruct()
	{
		// �R���X�g���N�^�ŊJ���B
		boost::scoped_ptr<rns::file_sharing_reader> pFileSharingReader(new rns::file_sharing_reader(kSampleFilePath));

		// ���p�t�@�C�����J�����B
		CPPUNIT_ASSERT(pFileSharingReader->is_open());

		// �t�@�C�����p���t�@�C�������݂��A�ǂݏ����\�B
		CPPUNIT_ASSERT_EQUAL(0, check_open(kSampleFileShrPath, _O_RDWR));

		// �t�@�C�����p���������B
		check_shr(kSampleFileShrPath, 0, 1, kSampleFileRelPath);

		pFileSharingReader.reset();

		// �t�@�C�����p���������B
		check_shr(kSampleFileShrPath, 0, 0, kSampleFileRelPath);
	}

	/**
	 * @brief @ref file_sharing_info_file���w��t�� rns::file_sharing_reader::file_sharing_reader() �̌����B
	 */
	void testConstruct2()
	{
		// �R���X�g���N�^�ŊJ���B
		boost::scoped_ptr<rns::file_sharing_reader>
			pFileSharingReader(new rns::file_sharing_reader(kSampleFilePath, kSampleFileShrPath2));

		// ���p�t�@�C�����J�����B
		CPPUNIT_ASSERT(pFileSharingReader->is_open());

		// �t�@�C�����p���t�@�C�������݂��A�ǂݏ����\�B
		CPPUNIT_ASSERT_EQUAL(0, check_open(kSampleFileShrPath2, _O_RDWR));

		// �t�@�C�����p���������B
		check_shr(kSampleFileShrPath2, 0, 1, kSampleFileRelPath);

		// �t�@�C���̋��p���I���B
		pFileSharingReader.reset();

		// �t�@�C�����p���������B
		check_shr(kSampleFileShrPath2, 0, 0, kSampleFileRelPath);
	}

	/**
	 * @brief rns::file_sharing_reader::open() �̌����B
	 */
	void testOpen()
	{
		boost::scoped_ptr<rns::file_sharing_reader> pFileSharingReader(new rns::file_sharing_reader);
		pFileSharingReader->open(kSampleFilePath);

		// ���p�t�@�C�����J�����B
		CPPUNIT_ASSERT(pFileSharingReader->is_open());

		// �t�@�C�����p���t�@�C�������݂��A�ǂݏ����\�B
		CPPUNIT_ASSERT_EQUAL(0, check_open(kSampleFileShrPath, _O_RDWR));

		// �t�@�C�����p���������B
		check_shr(kSampleFileShrPath, 0, 1, kSampleFileRelPath);

		// �t�@�C���̋��p���I���B
		pFileSharingReader.reset();

		// �t�@�C�����p���������B
		check_shr(kSampleFileShrPath, 0, 0, kSampleFileRelPath);
	}

	/**
	 * @brief @ref file_sharing_info_file���w��t�� rns::file_sharing_reader::open() �̌����B
	 */
	void testOpen2()
	{
		boost::scoped_ptr<rns::file_sharing_reader> pFileSharingReader(new rns::file_sharing_reader);
		pFileSharingReader->open(kSampleFilePath, kSampleFileShrPath2);

		// ���p�t�@�C�����J�����B
		CPPUNIT_ASSERT(pFileSharingReader->is_open());

		// �t�@�C�����p���t�@�C�������݂��A�ǂݏ����\�B
		CPPUNIT_ASSERT_EQUAL(0, check_open(kSampleFileShrPath2, _O_RDWR));

		// �t�@�C�����p���������B
		check_shr(kSampleFileShrPath2, 0, 1, kSampleFileRelPath);

		// �t�@�C���̋��p���I���B
		pFileSharingReader.reset();

		// �t�@�C�����p���������B
		check_shr(kSampleFileShrPath2, 0, 0, kSampleFileRelPath);
	}

	/**
	 * @brief ��΃p�X�̌����B
	 */
	void testAbsolutePath()
	{
		char szPath[_MAX_PATH] = { 0 };
		::_fullpath(szPath, kSampleFilePath, _MAX_PATH);
		
		rns::file_sharing_reader aFileSharingReader(szPath);

		// ���p�t�@�C�����J�����B
		CPPUNIT_ASSERT(aFileSharingReader.is_open());

		// �t�@�C�����p���t�@�C�������݂��A�ǂݏ����\�B
		CPPUNIT_ASSERT_EQUAL(0, check_open(kSampleFileShrPath, _O_RDWR));
	}

	/**
	 * @brief rns::basic_file_sharing_reader::use_count() �� rns::basic_file_sharing_reader::unique() �̌����B
	 */
	void testUseCount()
	{
		boost::scoped_ptr<rns::file_sharing_reader> pFileSharingReader1(new rns::file_sharing_reader(kSampleFilePath));

		// ���p����1�B
		CPPUNIT_ASSERT(pFileSharingReader1->unique());
		CPPUNIT_ASSERT_EQUAL(1, pFileSharingReader1->use_count());

		// ���b�N���ł���Ȃ����삷��B
		rns::file_reader::stream_ref pStream(pFileSharingReader1->lock());
		CPPUNIT_ASSERT(pFileSharingReader1->unique());
		CPPUNIT_ASSERT_EQUAL(1, pFileSharingReader1->use_count());
		pStream.reset();

		boost::scoped_ptr<rns::file_sharing_reader> pFileSharingReader2(new rns::file_sharing_reader(kSampleFilePath));

		// ���p����2�B
		CPPUNIT_ASSERT(! pFileSharingReader1->unique());
		CPPUNIT_ASSERT(! pFileSharingReader2->unique());
		CPPUNIT_ASSERT_EQUAL(2, pFileSharingReader1->use_count());
		CPPUNIT_ASSERT_EQUAL(2, pFileSharingReader2->use_count());

		pFileSharingReader1.reset();

		// ���p����1�B
		CPPUNIT_ASSERT(pFileSharingReader2->unique());
		CPPUNIT_ASSERT_EQUAL(1, pFileSharingReader2->use_count());
	}

	/**
	 * @brief �X�g���[���|�C���^���������p������邱�Ƃ̌����B
	 */
	void testShare()
	{
		static const int s_nHeader = 100;

		// �\�z�����񓚂�p�ӁB
		std::vector<int> aExpectList1, aExpectList2;

		{
			std::ifstream ifs(CA2WEX<MAX_PATH>(kSampleFilePath).m_psz);

			// �擪�����b�V����ǂݍ��ށB
			for(int i = 0; i < s_nHeader; i++) {
				long nMeshCode;
				ifs >> nMeshCode;
				aExpectList1.push_back(nMeshCode);
			}

			// �c��̃��b�V����ǂݍ��ށB
			std::copy(
				std::istream_iterator<int>(ifs),
				std::istream_iterator<int>(),
				std::back_inserter(aExpectList2)
			);
		}

		// �t�@�C���̋��p���J�n�B
		rns::file_sharing_reader aFileSharingReader(kSampleFilePath);

		std::vector<int> aActualList1, aActualList2;

		// ��1���ǂݍ��݁B
		{
			rns::file_reader::stream_ref pis(aFileSharingReader.lock());

			// ���b�N���̓t�@�C�����p���t�@�C���̓ǂݏ������s�\�B
			CPPUNIT_ASSERT_EQUAL(EACCES, check_open(kSampleFileShrPath, _O_RDWR));

			// �擪�����b�V����ǂݍ��ށB
			for(int i = 0; i < s_nHeader; i++) {
				long nMeshCode;
				*pis >> nMeshCode;
				aActualList1.push_back(nMeshCode);
			}
		}

		// ���b�N�͉�������Ă���B
		CPPUNIT_ASSERT_EQUAL(0, check_open(kSampleFileShrPath, _O_RDWR));

		// ��2���ǂݍ��݁B
		{
			rns::file_reader::stream_ref pis(aFileSharingReader.lock());

			// ���b�N���̓t�@�C�����p���t�@�C���̓ǂݏ������s�\�B
			CPPUNIT_ASSERT_EQUAL(EACCES, check_open(kSampleFileShrPath, _O_RDWR));

			// �c��̃��b�V����ǂݍ��ށB
			std::copy(
				std::istream_iterator<int>(*pis),
				std::istream_iterator<int>(),
				std::back_inserter(aActualList2)
			);
		}

		// ���b�N�͉�������Ă���B
		CPPUNIT_ASSERT_EQUAL(0, check_open(kSampleFileShrPath, _O_RDWR));

		// �񓚂��r�B
		CPPUNIT_ASSERT(aExpectList1 == aActualList1);
		CPPUNIT_ASSERT(aExpectList2 == aActualList2);
	}

	/**
	 * @brief �}���`�X���b�h�ɂ��r�������B
	 */
	void testThread()
	{
		test_thread(kSampleFilePath);
	}
//@}

protected:
	virtual file_reader_ref create_file_reader(const char* lpszFilePath) const
	{
		::remove(kSampleFileShrPath);
		CPPUNIT_ASSERT_EQUAL(-1, ::_access(kSampleFileShrPath, 0));

		return file_reader_ref(new rns::file_sharing_reader(lpszFilePath));
	}
};

/**
 * @ingroup unit_test
 * @brief rns::file_sharing_reader ��@ref unit_test�N���X�i�l�b�g���[�N�Łj�B
 */
class FileSharingReaderNetworkTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(FileSharingReaderNetworkTest);
	CPPUNIT_TEST(testConstruct);
	CPPUNIT_TEST(testAbsolutePath);
	CPPUNIT_TEST(testThread);
	CPPUNIT_TEST_SUITE_END();
public:
	virtual void setUp()
	{
		USES_CONVERSION;

		::remove(kGlobalSampleFileShrPath);
		CPPUNIT_ASSERT(::CopyFile(A2CT(kSampleFilePath), A2CT(kGlobalSampleFilePath), FALSE));
		CPPUNIT_ASSERT_EQUAL(-1, ::_access(kGlobalSampleFileShrPath, 0));
	}

	virtual void tearDown()
	{
		::remove(kGlobalSampleFilePath);
		::remove(kGlobalSampleFileShrPath);
	}

/// @name �e�X�g�P�[�X
//@{
	/**
	 * @brief rns::file_sharing_reader::file_sharing_reader() �̌����B
	 */
	void testConstruct()
	{
		// �R���X�g���N�^�ŊJ���B
		boost::scoped_ptr<rns::file_sharing_reader> pFileSharingReader(new rns::file_sharing_reader(kGlobalSampleFilePath));

		// ���p�t�@�C�����J�����B
		CPPUNIT_ASSERT(pFileSharingReader->is_open());

		// �t�@�C�����p���t�@�C�������݂��A�ǂݏ����\�B
		CPPUNIT_ASSERT_EQUAL(0, check_open(kGlobalSampleFileShrPath, _O_RDWR));

		// �t�@�C�����p���������B
		check_shr(kGlobalSampleFileShrPath, 0, 1, kSampleFileRelPath);

		pFileSharingReader.reset();

		// �t�@�C�����p���������B
		check_shr(kGlobalSampleFileShrPath, 0, 0, kSampleFileRelPath);
	}

	/**
	 * @brief ���΃p�X�Ŏ����Ȃ��ꍇ�A��΃p�X�Ŏ�����邱�Ƃ̌����B
	 */
	void testAbsolutePath()
	{
		// �R���X�g���N�^�ŊJ���B
		boost::scoped_ptr<rns::file_sharing_reader>
			pFileSharingReader(new rns::file_sharing_reader(kSampleFilePath, kGlobalSampleFileShrPath));

		// ���p�t�@�C�����J�����B
		CPPUNIT_ASSERT(pFileSharingReader->is_open());

		// �t�@�C�����p���t�@�C�������݂��A�ǂݏ����\�B
		CPPUNIT_ASSERT_EQUAL(0, check_open(kGlobalSampleFileShrPath, _O_RDWR));

		// �t���p�X�����߂�B
		char szSmapleFileAbsPath[_MAX_PATH];
		::_fullpath(szSmapleFileAbsPath, kSampleFilePath, _MAX_PATH);

		// �t�@�C�����p���������B
		check_shr(kGlobalSampleFileShrPath, 0, 1, szSmapleFileAbsPath);

		pFileSharingReader.reset();

		// �t�@�C�����p���������B
		check_shr(kGlobalSampleFileShrPath, 0, 0, szSmapleFileAbsPath);
	}

	/**
	 * @brief �}���`�X���b�h�ɂ��r�������B
	 */
	void testThread()
	{
		test_thread(kGlobalSampleFilePath);
	}
//@}
};

CPPUNIT_TEST_SUITE_REGISTRATION(FileSharingReaderTest);
CPPUNIT_TEST_SUITE_REGISTRATION(FileSharingReaderNetworkTest);
