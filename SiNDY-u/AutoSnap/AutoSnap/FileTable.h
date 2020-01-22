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

#include <fstream>
#include <vector>
#include <string>

/**
 * @brief			Process table like text file
 * @details			Read or write text file with separator
 *					All strings will be used by Unicode
 */
class CFileTable
{
public:
	typedef std::vector<std::wstring> Record;
	typedef std::vector<Record> Records;

public:
	virtual ~CFileTable(void);
	
	/**
	 * @brief		Open a table like style file
	 *
	 * @param[in]	strName Filename
	 * @param[in]	isRead Read or write
	 * @return		Succeed or not
	 */
	bool Open(const std::wstring& strName, bool isRead = true);

	/**
	 * @brief		Close file
	 */
	void Close();

	/**
	 * @brief		Read all valid records from file
	 * @details		Suggest for small file
	 *
	 * @param[out]	rds Records
	 * @return		Count of records
	 */
	size_t Read(Records& rds);

	/**
	 * @brief		Read a valid record from file
	 *
	 * @param[out]	rd Record
	 * @return		Succeed or not
	 */
	bool Read(Record& rd);

	/**
	 * @brief		Write a record
	 *
	 * @param[in]	rd The record which will be write into the file
	 * @return		Succeed or not
	 */
	bool Write(const Record& rd);

	/**
	 * @brief		Write records
	 *
	 * @param[in]	rds The records which will be written into the file
	 * @return		Succeed or not
	 */
	bool Write(const Records& rds);

	/**
	 * @brief		Write string
	 *
	 * @param[in]	str String
	 * @return		Succeed or not
	 */
	bool Write(const std::wstring& str);

	/**
	 * @brief		Write string
	 *
	 * @param[in]	str String
	 * @return		Succeed or not
	 */
	bool Write(const wchar_t* str);

protected:
	/**
	 * @brief		Read a valid record
	 *
	 * @param[out]	rd Record
	 * @return		Succeed or not
	 */
	bool ReadRecord(Record& rd);

	/**
	 * @brief		Read a line from file
	 *
	 * @param[out]	str String
	 * @return		Succeed or not
	 */
	bool ReadLine(std::wstring& str);

private:
	std::wfstream m_stream; ///< File stream
};
