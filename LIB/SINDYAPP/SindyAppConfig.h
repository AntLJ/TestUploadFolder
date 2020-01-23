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

// SindyAppConfig.h: SindyAppConfig クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYAPPCONFIG_H__26A94150_A439_49A2_B3A3_A556CD0B6231__INCLUDED_)
#define AFX_SINDYAPPCONFIG_H__26A94150_A439_49A2_B3A3_A556CD0B6231__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GDB.h"

class SindyAppConfig  
{
public:
	SindyAppConfig();
	virtual ~SindyAppConfig();

	virtual HRESULT Read(const char *cFileName);
	virtual void GetConfig(const std::string &cKey, std::vector<std::string> *cConfigList);
	virtual void GetConfig(const std::string &cKey, std::string *cConfig);

protected:
	struct AppConfig
	{
		std::string eKeyword;
		std::vector<std::string> eConfigList;

		AppConfig() : eKeyword(""), eConfigList() {}
		AppConfig(const std::string key) : eKeyword(key), eConfigList() {}
		virtual ~AppConfig() {}
	};

	struct lessAppConfig : public std::binary_function<AppConfig,AppConfig,bool>
	{
		bool operator ()(const AppConfig &cConf1, const AppConfig &cConf2) const
		{
			return cConf1.eKeyword < cConf2.eKeyword;
		}
	};

	std::set<AppConfig,lessAppConfig> mConfiguration;
};

#endif // !defined(AFX_SINDYAPPCONFIG_H__26A94150_A439_49A2_B3A3_A556CD0B6231__INCLUDED_)
