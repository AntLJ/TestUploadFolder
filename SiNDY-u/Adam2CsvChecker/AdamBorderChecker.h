#pragma once
#include "LogicalChecker.h"

namespace sindy {
namespace adam2 {
namespace format {

/**
* @brief border_link.csv �_���`�F�b�J�[
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
* @brief border_point.csv �_���`�F�b�J�[
*/
class CSimpleBorderPointChecker : public CSimpleLogicalChecker
{
public:
	CSimpleBorderPointChecker() {}
	virtual ~CSimpleBorderPointChecker() {}

private:
	virtual bool sub(const ImportFileList& kInfo, const std::vector<Feature>& kData) override;
};

} // format
} // adam2
} // sindy
