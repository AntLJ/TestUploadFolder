#include "stdafx.h"
#include "AdamBorderChecker.h"


bool sindy::adam2::format::CSimpleBorderLinkChecker::sub(const ImportFileList & kInfo, const std::vector<Feature>& kData)
{
	return true;
}

bool sindy::adam2::format::CSimpleBorderPointChecker::sub(const ImportFileList & kInfo, const std::vector<Feature>& kData)
{
	for (const auto& itr : kData)
	{
		long objectid = GetLong<CBorderNodeChecker::OBJECTID>(itr);
		long class_c = GetLong<CBorderNodeChecker::POINT_CLASS_C>(itr);
		long lastnode_f = GetLong<CBorderNodeChecker::LAST_NODE_F>(itr);

		if (lastnode_f && class_c != 1) {
			sCALLBACK->callback(_T("[ERROR][%s.%d]構成点ポイントに終端フラグが付与されています."), kInfo.alias_name, objectid);
		}
	}

	return true;
}
