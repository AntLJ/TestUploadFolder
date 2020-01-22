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
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#include <rns/file_sharing_reader.h>
#include "FileReaderTest.h"
#include "Misc.h"

namespace {
} // anonymous namespace

/**
 * @ingroup unit_test
 * @brief rns::simple_file_reader ÇÃ@ref unit_testÉNÉâÉXÅB
 */
class SimpleFileReaderTest : public FileReaderTest 
{
	CPPUNIT_TEST_SUB_SUITE(SimpleFileReaderTest, FileReaderTest);
	CPPUNIT_TEST_SUITE_END();

protected:
	virtual file_reader_ref create_file_reader(const char* lpszFilePath) const
	{
		return file_reader_ref(new rns::simple_file_reader(lpszFilePath));
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(SimpleFileReaderTest);
