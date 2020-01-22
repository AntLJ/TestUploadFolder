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

#include <string>

#include "../Json/json.h"

/**
 * @brief Configure information class
 *
 */
class Config
{
public:
	/**
	 * @brief Convert from UTF-8 to UCS2
	 * 
	 * @param[in] str UTF-8 string
	 * @return UCS2 string
	 */
	static std::wstring s2w(const std::string& str);

	/**
	 * @brief Convert from ucs2 to UTF-8
	 * 
	 * @param[in] str UCS2 string
	 * @return  UTF-8 string
	 */
	static std::string w2s(const std::wstring& str);

	/**
	 * @brief load configures from file
	 *
	 * @param[in] name File name
	 * @return succeed or not
	 */
	bool Load(const std::wstring& name);

protected:
	/**
	 * @brief Get a value from object via key name
	 *
	 * @param[in] pObject JSON object
	 * @param[in] strName key name
	 * @return JSON Value
	 */
	const json::JsonValuePtr GetValue(const json::JsonObjectPtr pObject, const std::wstring& strName);

	/**
	 * @brief Get a value from array via index number
	 *
	 * @param[in] pArray JSON array
	 * @param[in] n Index number
	 * @return JSON Value
	 */
	const json::JsonValuePtr GetValue(const json::JsonArrayPtr pArray, size_t n);

	/**
	 * @brief Get string from JSON value
	 *
	 * @param[in] pValue JSON value object
	 * @param[out] str string value
	 * @return succeed or not
	 */
	bool GetString(const json::JsonValuePtr pValue, std::wstring& str);

	/**
	 * @brief Get integer from JSON value
	 *
	 * @param[in] pValue JSON value object
	 * @param[out] i Integer value
	 * @return succeed or not
	 */
	bool GetInt(const json::JsonValuePtr pValue, int& i);

	/**
	 * @brief Get double from JSON value
	 *
	 * @param[in] pValue JSON value object
	 * @param[out] d Double value
	 * @return succeed or not
	 */
	bool GetDouble(const json::JsonValuePtr pValue, double& d);

	/**
	 * @brief Get long long from JSON value
	 *
	 * @param[in] pValue JSON value object
	 * @param[out] ll Long long value
	 * @return succeed or not
	 */
	bool GetLongLong(const json::JsonValuePtr pValue, long long& ll);

	/**
	 * @brief Get bool from JSON value
	 *
	 * @param[in] pValue JSON value object
	 * @param[out] b Boolean value
	 * @return succeed or not
	 */
	bool GetBool(const json::JsonValuePtr pValue, bool& b);

	/**
	 * @brief Get string from JSON value
	 *
	 * @param[in] pValue JSON value object
	 * @param[in] strDefault Default value, when the specified name is not exist in configure file.
	 * @return String
	 */
	std::wstring Get(const json::JsonValuePtr pValue, const std::wstring& strDefault);

	/**
	 * @brief Get integer from JSON value
	 *
	 * @param[in] pValue JSON value object
	 * @param[in] nDefault Default value, when the specified name is not exist in configure file.
	 * @return Ingeter
	 */
	int Get(const json::JsonValuePtr pValue, int nDefault);

	/**
	 * @brief Get double from JSON value
	 *
	 * @param[in] pValue JSON value object
	 * @param[in] dDefault Default value, when the specified name is not exist in configure file.
	 * @return Double
	 */
	double Get(const json::JsonValuePtr pValue, double dDefault);

	/**
	 * @brief Get long long from JSON value
	 *
	 * @param[in] pValue JSON value object
	 * @param[in] llDefault Default value, when the specified name is not exist in configure file.
	 * @return long long
	 */
	long long Get(const json::JsonValuePtr pValue, long long llDefault);

	/**
	 * @brief Get boolean from JSON value
	 *
	 * @param[in] pValue JSON value object
	 * @param[in] bDefault Default value, when the specified name is not exist in configure file.
	 * @return boolean
	 */
	bool Get(const json::JsonValuePtr pValue, bool bDefault);

protected:
	/**
	 * @brief Load event
	 * @details process all needed configure values from the JSON object.
	 *
	 * @param[in] pObject object
	 * return succeed or not
	 */
	virtual bool OnLoad(json::JsonObjectPtr pObject) = 0;
};

/**
 * @brief This is a sample of using Class Configure, It is used for AutoSnap tool.
 */
class AutoSnapConfig : public Config
{
public:
	/**
	 * @brief Mode type
	 */
	enum Type : int
	{
		T_NORMAL = 1,	///< Normal Mode: One database, one snap log file and one log file
		T_MULTI_FGDB,	///< Multi PGDB Files Mode: multiple databases, one snap log file, one log file
		T_MULTI_LOG,	///< Multi Snap Log Files Mode: multiple databases, multiple snap log files, multiple log files
	} type; ///< Mode
	double radius; ///< Snapping range from endpoint
	std::wstring log; ///< Log file path
	std::wstring mesh_list; ///< Mesh code filename
	std::wstring snap; ///< Snap records filename
	std::wstring snap_layer; ///< Snap setting file
	std::wstring lot; ///< Data
	std::wstring mesh; ///< Mesh data
	std::wstring mesh_name; ///< Mesh table name
	std::unordered_map<std::wstring, double> mini_list; ///< Short length of features
	std::wstring snap_format;	///< Snap log filename format , Type3 only
	std::wstring log_format;	///< Log filename format, Type3 only
private:
	virtual bool OnLoad(json::JsonObjectPtr pObject);
};
