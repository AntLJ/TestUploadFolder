#pragma once
#include "LogicalChecker.h"


namespace sindy {
namespace adam2 {
namespace format {

/**
* @brief compart_link.csv �_���`�F�b�J�[
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
* @brief compart_point.csv �_���`�F�b�J�[
*/
class CSimpleCompartPointChecker : public CSimpleLogicalChecker
{
public:
	CSimpleCompartPointChecker() {}
	virtual ~CSimpleCompartPointChecker() {}

private:
	virtual bool sub(const ImportFileList& kInfo, const std::vector<Feature>& kData) override;
};

} // format
} // adam2
} // sindy
