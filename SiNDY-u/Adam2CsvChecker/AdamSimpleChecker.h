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


/**
 * 単一フィーチャクラスの論理チェッカー
 */

#pragma once
#include "LogicalChecker.h"

namespace sindy  {
namespace adam2  {
namespace format {

/**
 * @brief lane_link.csv 論理チェッカー
 */
class CSimpleLaneLinkChecker : public CSimpleLogicalChecker
{
public:
	virtual ~CSimpleLaneLinkChecker() {}
	CSimpleLaneLinkChecker() {}

private:
	virtual bool sub(const ImportFileList& kInfo, const std::vector<Feature>& kData) override;
};


/**
 * @brief lane_point.csv 論理チェッカー
 */
class CSimpleLanePointChecker : public CSimpleLogicalChecker
{
public:
	virtual ~CSimpleLanePointChecker() {}
	CSimpleLanePointChecker() {}

private:
	virtual bool sub(const ImportFileList& kInfo, const std::vector<Feature>& kData) override;
};


/**
* @brief compart_link.csv 論理チェッカー
*/
class CSimpleCompartLinkChecker : public CSimpleLogicalChecker
{
public:
	CSimpleCompartLinkChecker() {}
	virtual ~CSimpleCompartLinkChecker() {}

private:
	virtual bool sub(const ImportFileList& kInfo, const std::vector<Feature>& kData) override;
};


/**
* @brief compart_point.csv 論理チェッカー
*/
class CSimpleCompartPointChecker : public CSimpleLogicalChecker
{
public:
	CSimpleCompartPointChecker() {}
	virtual ~CSimpleCompartPointChecker() {}

private:
	virtual bool sub(const ImportFileList& kInfo, const std::vector<Feature>& kData) override;
};


/**
* @brief border_link.csv 論理チェッカー
*/
class CSimpleBorderLinkChecker : public CSimpleLogicalChecker
{
public:
	CSimpleBorderLinkChecker() {}
	virtual ~CSimpleBorderLinkChecker() {}

private:
	virtual bool sub(const ImportFileList& kInfo, const std::vector<Feature>& kData) override;
};


/**
* @brief border_point.csv 論理チェッカー
*/
class CSimpleBorderPointChecker : public CSimpleLogicalChecker
{
public:
	CSimpleBorderPointChecker() {}
	virtual ~CSimpleBorderPointChecker() {}

private:
	virtual bool sub(const ImportFileList& kInfo, const std::vector<Feature>& kData) override;
};


/**
* @brief signal_point.csv 論理チェッカー
*/
class CSimpleSignalPointChecker : public CSimpleLogicalChecker
{
public:
	CSimpleSignalPointChecker() {}
	virtual ~CSimpleSignalPointChecker() {}

private:
	virtual bool sub(const ImportFileList& kInfo, const std::vector<Feature>& kData) override;
};


/**
* @brief roadsign_point.csv 論理チェッカー
*/
class CSimpleRoadsignPointChecker : public CSimpleLogicalChecker
{
public:
	CSimpleRoadsignPointChecker() {}
	virtual ~CSimpleRoadsignPointChecker() {}

private:
	virtual bool sub(const ImportFileList& kInfo, const std::vector<Feature>& kData) override;
};


} // format
} // adam2
} // sindy

