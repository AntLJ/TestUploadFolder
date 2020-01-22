#include "stdafx.h"
#include "AdamRoadObjectChecker.h"

bool sindy::adam2::format::CSimpleSignalPointChecker::sub(const ImportFileList & kInfo, const std::vector<Feature>& kData)
{
	for (const auto& itr : kData)
	{
		long objectid = GetLong<CSignalPointChecker::OBJECTID>(itr);
		long lamp_num = GetLong<CSignalPointChecker::LAMP_COUNT>(itr);
		long arrow_f = GetLong<CSignalPointChecker::ARROW_F>(itr);

		if (arrow_f && lamp_num <= 3){
			sCALLBACK->callback(_T("[WARNING][%s.%d]���t�M���̃����v�������Ȃ���������܂���.[LAMP=%d]"), kInfo.alias_name, objectid, lamp_num);
		}
	}

	return true;
}

bool sindy::adam2::format::CSimpleRoadsignPointChecker::sub(const ImportFileList & kInfo, const std::vector<Feature>& kData)
{
	return true;
}
