#include "stdafx.h"
#include "AdamCompartChecker.h"


bool sindy::adam2::format::CSimpleCompartLinkChecker::sub(const ImportFileList & kInfo, const std::vector<Feature>& kData)
{
	return true;
}

bool sindy::adam2::format::CSimpleCompartPointChecker::sub(const ImportFileList & kInfo, const std::vector<Feature>& kData)
{
	for (const auto& itr : kData)
	{
		long objectid = GetLong<CCompartNodeChecker::OBJECTID>(itr);
		long class_c = GetLong<CCompartNodeChecker::POINT_CLASS_C>(itr);
		long lastnode_f = GetLong<CCompartNodeChecker::LAST_NODE_F>(itr);

		double width = GetDouble<CCompartNodeChecker::WIDTH>(itr);

		if (lastnode_f && class_c != 1)
		{
			sCALLBACK->callback(_T("[ERROR][%s.%d]構成点ポイントに終端フラグが付与されています."), kInfo.alias_name, objectid);
		}
		if (!(0.0 <= width && width < 10.0))
		{
			sCALLBACK->callback(_T("[WARNING][%s.%d]幅員値が正しくない可能性があります.[Width=%5.3lf]"), kInfo.alias_name, objectid, width);
		}
	}

	return true;
}
