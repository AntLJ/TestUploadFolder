/* 
 * 地物フィーチャのチェッカー定義
 * - signal_point
 * - roadsign_point
 */

#pragma once
#include "LogicalChecker.h"

namespace sindy {
namespace adam2 {
namespace format {

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
