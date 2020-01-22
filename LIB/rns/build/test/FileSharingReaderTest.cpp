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
 * @brief 指定した条件通りにファイルを開けることを検証する。
 *
 * @param lpszPath [in] ファイルパス。
 * @param nFlag [in] _open() の第2引数。
 * @retval 0 ファイルを開くことができた。
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
 * @brief @ref file_sharing_info_fileの正当性を検証する。
 *
 * @param lpszShrPath [in] @ref file_sharing_info_fileパス。
 * @param nExpectPos [in] 然るべきファイルポジション。
 * @param nExpectRefCount [in] 然るべき参照数。
 * @param lpszExpectPath [in] 然るべきパス文字列。
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
 * @brief read_loop() の引数
 */
struct ReadLoopArgs
{
	const char* m_lpszFilePath; ///< 共用ファイルパス。
	MeshCodeList* m_pMeshCodeList; ///< メッシュコード一覧。
};

/**
 * @brief test_thread() の1スレッド。
 *
 * @param vpArgs [in] 引数。
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
 * @brief マルチスレッドによる排他検査。
 *
 * @param lpszFilePath [in] 共用ファイルパス。
 */
void test_thread(const char* lpszFilePath)
{
	MeshCodeList aExpectList;

	{
		std::ifstream ifs(CA2WEX<MAX_PATH>(lpszFilePath).m_psz);

		// 全メッシュ読み込む。
		std::copy(
			std::istream_iterator<int>(ifs),
			std::istream_iterator<int>(),
			std::back_inserter(aExpectList)
		);
	}

	static const int s_nThreadCount = 6;

	// スレッドを立ち上げる。
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

	// 全てのスレッドの終了を待つ。
	for(int i = 0; i < s_nThreadCount; i++) {
		::WaitForSingleObject(hThreads[i], INFINITE);
		::CloseHandle(hThreads[i]);
		nActualCount += aMeshCodeList[i].size();
//		std::cout << aMeshCodeList[i].size() << std::endl;
	}

	// 重複も抜けもなく、メッシュコードが取り出せたことを確認する。
	CPPUNIT_ASSERT_EQUAL(aExpectList.size(), nActualCount);

	// 正しい順番でメッシュをコードを取り出せたことを確認する。
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
 * @brief rns::file_sharing_reader の@ref unit_testクラス（ローカル版）。
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

/// @name テストケース
//@{
	/**
	 * @brief 各種例外の検査。
	 */
	void testExceptions()
	{
		// ファイル共用情報ファイルパスがディレクトリを指していた。
		CPPUNIT_ASSERT_THROW(
			rns::file_sharing_reader(kSampleFilePath, kDataFolderPath),
			rns::file_sharing_reader::cant_create_sharing_info_file
		);

		// ファイル共用情報ファイルが存在しないため、共用ファイルを求められない。
		CPPUNIT_ASSERT_THROW(
			rns::file_sharing_reader(0, kSampleFileShrPath),
			rns::file_sharing_reader::not_existing_sharing_info_file
		);

		// ダミーの共用情報ファイルを作成する。
		rns::file_sharing_reader(kSampleFile2Path, kSampleFileShrPath);

		// 共用情報が合致しない。
		CPPUNIT_ASSERT_THROW(
			rns::file_sharing_reader(kSampleFilePath, 0),
			rns::file_sharing_reader::not_match_sharing_info
		);

		// 共用情報が指すファイルが存在しない。
		CPPUNIT_ASSERT_THROW(
			rns::file_sharing_reader(0, kSampleFileShrPath),
			rns::file_sharing_reader::not_existing_sharing_file
		);
	}

	/**
	 * @brief rns::file_sharing_reader::file_sharing_reader() の検査。
	 */
	void testConstruct()
	{
		// コンストラクタで開く。
		boost::scoped_ptr<rns::file_sharing_reader> pFileSharingReader(new rns::file_sharing_reader(kSampleFilePath));

		// 共用ファイルを開けた。
		CPPUNIT_ASSERT(pFileSharingReader->is_open());

		// ファイル共用情報ファイルが存在し、読み書き可能。
		CPPUNIT_ASSERT_EQUAL(0, check_open(kSampleFileShrPath, _O_RDWR));

		// ファイル共用情報を検査。
		check_shr(kSampleFileShrPath, 0, 1, kSampleFileRelPath);

		pFileSharingReader.reset();

		// ファイル共用情報を検査。
		check_shr(kSampleFileShrPath, 0, 0, kSampleFileRelPath);
	}

	/**
	 * @brief @ref file_sharing_info_file名指定付き rns::file_sharing_reader::file_sharing_reader() の検査。
	 */
	void testConstruct2()
	{
		// コンストラクタで開く。
		boost::scoped_ptr<rns::file_sharing_reader>
			pFileSharingReader(new rns::file_sharing_reader(kSampleFilePath, kSampleFileShrPath2));

		// 共用ファイルを開けた。
		CPPUNIT_ASSERT(pFileSharingReader->is_open());

		// ファイル共用情報ファイルが存在し、読み書き可能。
		CPPUNIT_ASSERT_EQUAL(0, check_open(kSampleFileShrPath2, _O_RDWR));

		// ファイル共用情報を検査。
		check_shr(kSampleFileShrPath2, 0, 1, kSampleFileRelPath);

		// ファイルの共用を終了。
		pFileSharingReader.reset();

		// ファイル共用情報を検査。
		check_shr(kSampleFileShrPath2, 0, 0, kSampleFileRelPath);
	}

	/**
	 * @brief rns::file_sharing_reader::open() の検査。
	 */
	void testOpen()
	{
		boost::scoped_ptr<rns::file_sharing_reader> pFileSharingReader(new rns::file_sharing_reader);
		pFileSharingReader->open(kSampleFilePath);

		// 共用ファイルを開けた。
		CPPUNIT_ASSERT(pFileSharingReader->is_open());

		// ファイル共用情報ファイルが存在し、読み書き可能。
		CPPUNIT_ASSERT_EQUAL(0, check_open(kSampleFileShrPath, _O_RDWR));

		// ファイル共用情報を検査。
		check_shr(kSampleFileShrPath, 0, 1, kSampleFileRelPath);

		// ファイルの共用を終了。
		pFileSharingReader.reset();

		// ファイル共用情報を検査。
		check_shr(kSampleFileShrPath, 0, 0, kSampleFileRelPath);
	}

	/**
	 * @brief @ref file_sharing_info_file名指定付き rns::file_sharing_reader::open() の検査。
	 */
	void testOpen2()
	{
		boost::scoped_ptr<rns::file_sharing_reader> pFileSharingReader(new rns::file_sharing_reader);
		pFileSharingReader->open(kSampleFilePath, kSampleFileShrPath2);

		// 共用ファイルを開けた。
		CPPUNIT_ASSERT(pFileSharingReader->is_open());

		// ファイル共用情報ファイルが存在し、読み書き可能。
		CPPUNIT_ASSERT_EQUAL(0, check_open(kSampleFileShrPath2, _O_RDWR));

		// ファイル共用情報を検査。
		check_shr(kSampleFileShrPath2, 0, 1, kSampleFileRelPath);

		// ファイルの共用を終了。
		pFileSharingReader.reset();

		// ファイル共用情報を検査。
		check_shr(kSampleFileShrPath2, 0, 0, kSampleFileRelPath);
	}

	/**
	 * @brief 絶対パスの検査。
	 */
	void testAbsolutePath()
	{
		char szPath[_MAX_PATH] = { 0 };
		::_fullpath(szPath, kSampleFilePath, _MAX_PATH);
		
		rns::file_sharing_reader aFileSharingReader(szPath);

		// 共用ファイルを開けた。
		CPPUNIT_ASSERT(aFileSharingReader.is_open());

		// ファイル共用情報ファイルが存在し、読み書き可能。
		CPPUNIT_ASSERT_EQUAL(0, check_open(kSampleFileShrPath, _O_RDWR));
	}

	/**
	 * @brief rns::basic_file_sharing_reader::use_count() と rns::basic_file_sharing_reader::unique() の検査。
	 */
	void testUseCount()
	{
		boost::scoped_ptr<rns::file_sharing_reader> pFileSharingReader1(new rns::file_sharing_reader(kSampleFilePath));

		// 共用数は1。
		CPPUNIT_ASSERT(pFileSharingReader1->unique());
		CPPUNIT_ASSERT_EQUAL(1, pFileSharingReader1->use_count());

		// ロック中でも難なく動作する。
		rns::file_reader::stream_ref pStream(pFileSharingReader1->lock());
		CPPUNIT_ASSERT(pFileSharingReader1->unique());
		CPPUNIT_ASSERT_EQUAL(1, pFileSharingReader1->use_count());
		pStream.reset();

		boost::scoped_ptr<rns::file_sharing_reader> pFileSharingReader2(new rns::file_sharing_reader(kSampleFilePath));

		// 共用数は2。
		CPPUNIT_ASSERT(! pFileSharingReader1->unique());
		CPPUNIT_ASSERT(! pFileSharingReader2->unique());
		CPPUNIT_ASSERT_EQUAL(2, pFileSharingReader1->use_count());
		CPPUNIT_ASSERT_EQUAL(2, pFileSharingReader2->use_count());

		pFileSharingReader1.reset();

		// 共用数は1。
		CPPUNIT_ASSERT(pFileSharingReader2->unique());
		CPPUNIT_ASSERT_EQUAL(1, pFileSharingReader2->use_count());
	}

	/**
	 * @brief ストリームポインタが正しく継承されることの検査。
	 */
	void testShare()
	{
		static const int s_nHeader = 100;

		// 予想される回答を用意。
		std::vector<int> aExpectList1, aExpectList2;

		{
			std::ifstream ifs(CA2WEX<MAX_PATH>(kSampleFilePath).m_psz);

			// 先頭数メッシュを読み込む。
			for(int i = 0; i < s_nHeader; i++) {
				long nMeshCode;
				ifs >> nMeshCode;
				aExpectList1.push_back(nMeshCode);
			}

			// 残りのメッシュを読み込む。
			std::copy(
				std::istream_iterator<int>(ifs),
				std::istream_iterator<int>(),
				std::back_inserter(aExpectList2)
			);
		}

		// ファイルの共用を開始。
		rns::file_sharing_reader aFileSharingReader(kSampleFilePath);

		std::vector<int> aActualList1, aActualList2;

		// 第1次読み込み。
		{
			rns::file_reader::stream_ref pis(aFileSharingReader.lock());

			// ロック中はファイル共用情報ファイルの読み書きが不可能。
			CPPUNIT_ASSERT_EQUAL(EACCES, check_open(kSampleFileShrPath, _O_RDWR));

			// 先頭数メッシュを読み込む。
			for(int i = 0; i < s_nHeader; i++) {
				long nMeshCode;
				*pis >> nMeshCode;
				aActualList1.push_back(nMeshCode);
			}
		}

		// ロックは解除されている。
		CPPUNIT_ASSERT_EQUAL(0, check_open(kSampleFileShrPath, _O_RDWR));

		// 第2次読み込み。
		{
			rns::file_reader::stream_ref pis(aFileSharingReader.lock());

			// ロック中はファイル共用情報ファイルの読み書きが不可能。
			CPPUNIT_ASSERT_EQUAL(EACCES, check_open(kSampleFileShrPath, _O_RDWR));

			// 残りのメッシュを読み込む。
			std::copy(
				std::istream_iterator<int>(*pis),
				std::istream_iterator<int>(),
				std::back_inserter(aActualList2)
			);
		}

		// ロックは解除されている。
		CPPUNIT_ASSERT_EQUAL(0, check_open(kSampleFileShrPath, _O_RDWR));

		// 回答を比較。
		CPPUNIT_ASSERT(aExpectList1 == aActualList1);
		CPPUNIT_ASSERT(aExpectList2 == aActualList2);
	}

	/**
	 * @brief マルチスレッドによる排他検査。
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
 * @brief rns::file_sharing_reader の@ref unit_testクラス（ネットワーク版）。
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

/// @name テストケース
//@{
	/**
	 * @brief rns::file_sharing_reader::file_sharing_reader() の検査。
	 */
	void testConstruct()
	{
		// コンストラクタで開く。
		boost::scoped_ptr<rns::file_sharing_reader> pFileSharingReader(new rns::file_sharing_reader(kGlobalSampleFilePath));

		// 共用ファイルを開けた。
		CPPUNIT_ASSERT(pFileSharingReader->is_open());

		// ファイル共用情報ファイルが存在し、読み書き可能。
		CPPUNIT_ASSERT_EQUAL(0, check_open(kGlobalSampleFileShrPath, _O_RDWR));

		// ファイル共用情報を検査。
		check_shr(kGlobalSampleFileShrPath, 0, 1, kSampleFileRelPath);

		pFileSharingReader.reset();

		// ファイル共用情報を検査。
		check_shr(kGlobalSampleFileShrPath, 0, 0, kSampleFileRelPath);
	}

	/**
	 * @brief 相対パスで示せない場合、絶対パスで示されることの検査。
	 */
	void testAbsolutePath()
	{
		// コンストラクタで開く。
		boost::scoped_ptr<rns::file_sharing_reader>
			pFileSharingReader(new rns::file_sharing_reader(kSampleFilePath, kGlobalSampleFileShrPath));

		// 共用ファイルを開けた。
		CPPUNIT_ASSERT(pFileSharingReader->is_open());

		// ファイル共用情報ファイルが存在し、読み書き可能。
		CPPUNIT_ASSERT_EQUAL(0, check_open(kGlobalSampleFileShrPath, _O_RDWR));

		// フルパスを求める。
		char szSmapleFileAbsPath[_MAX_PATH];
		::_fullpath(szSmapleFileAbsPath, kSampleFilePath, _MAX_PATH);

		// ファイル共用情報を検査。
		check_shr(kGlobalSampleFileShrPath, 0, 1, szSmapleFileAbsPath);

		pFileSharingReader.reset();

		// ファイル共用情報を検査。
		check_shr(kGlobalSampleFileShrPath, 0, 0, szSmapleFileAbsPath);
	}

	/**
	 * @brief マルチスレッドによる排他検査。
	 */
	void testThread()
	{
		test_thread(kGlobalSampleFilePath);
	}
//@}
};

CPPUNIT_TEST_SUITE_REGISTRATION(FileSharingReaderTest);
CPPUNIT_TEST_SUITE_REGISTRATION(FileSharingReaderNetworkTest);
