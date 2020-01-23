#include "stdafx.h"
#include "Logger.h" // たぶん使い方が良くない
#include "CheckUtil.h"

using namespace addr;
using namespace sindy;
using namespace sindy::schema;

CheckUtil::CheckUtil(addr::AddrRecHelper& addrRecHelper)
{
	m_addrRecHelper = addrRecHelper;
}


CheckUtil::~CheckUtil()
{
}

bool CheckUtil::MakeAddrYomiList(
	const sindy::CTableContainer& relAddrTbl,
	std::map<CString, AddrRecPairs>& relAddrMap)
{
	bool hasError = false;

	for (const auto& relAddrRec : relAddrTbl)
	{
		auto& row = *CAST_ROWC(relAddrRec);
		// 名称(地番、号)取得
		AddrRec tgtAddrRec(row.GetValue(rel_addr::kAddrCode, CString()), nullptr);
		if (!m_addrRecHelper.makeAddrNames(tgtAddrRec))
		{
			ERRLOGGER.Log(_T("#文字付き住所テーブルから名称取得に失敗\t%s\n"), tgtAddrRec.getJusyocode());
			hasError = true;
			continue;
		}

		// 11桁ごとに格納
		auto addrCode11 = tgtAddrRec.getAdrcode11();
		if (0 == relAddrMap.count(addrCode11))
		{
			AddrRecPairs addrRecPairs;
			addrRecPairs.emplace_back(tgtAddrRec, row);
			relAddrMap.emplace(addrCode11, addrRecPairs);
		}
		else
		{
			relAddrMap[addrCode11].emplace_back(tgtAddrRec, row);
		}
	}

	return !hasError;
}

bool CheckUtil::CheckAddressDataExistance(
	const addr::AddrRecPairs & adminAddrRecs,
	const addr::AddrRecs & csAddrRecs,
	sindy::CTableContainer & gouPointTbl,
	const AddrRec & addrRec)
{
	bool exist = false;
	AddrRecs gpAddrRecs;
	switch (addrRec.getAddrDigit())
	{
	case AddrDigit::addr: // 対象が11桁なら行政界とCSポイントから存在判定
		exist = decideAddrDigitStatus(adminAddrRecs, csAddrRecs, addrRec);
		break;
	case AddrDigit::chiban: // 対象が16桁なら行政界、号、CSから存在判定
		getGpAddrRecs(gouPointTbl, adminAddrRecs, gpAddrRecs);
		exist = decideChibanDigitStatus(adminAddrRecs, gpAddrRecs, csAddrRecs, addrRec);
		break;
	case AddrDigit::gou: // 対象が20桁なら号、CSから存在判定
		getGpAddrRecs(gouPointTbl, adminAddrRecs, gpAddrRecs);
		exist = decideGouDigitStatus(gpAddrRecs, csAddrRecs, addrRec);
		break;
	default:
		break;
	}
	return exist;
}

bool CheckUtil::CheckAddressDataExistance(
	const addr::AddrRecPairs & adminAddrRecs,
	const addr::AddrRecs & csAddrRecs,
	sindy::CTableContainer & gouPointTbl,
	const addr::AddrRec & addrRec,
	addr::AddrRecs & existAddrRecs)
{
	bool exist = false;
	AddrRecs gpAddrRecs;
	switch (addrRec.getAddrDigit())
	{
	case AddrDigit::addr: // 対象が11桁なら行政界とCSポイントから存在判定
		exist = decideAddrDigitStatus(adminAddrRecs, csAddrRecs, addrRec, existAddrRecs);
		break;
	case AddrDigit::chiban: // 対象が16桁なら行政界、号、CSから存在判定
		getGpAddrRecs(gouPointTbl, adminAddrRecs, gpAddrRecs);
		exist = decideChibanDigitStatus(adminAddrRecs, gpAddrRecs, csAddrRecs, addrRec, existAddrRecs);
		break;
	case AddrDigit::gou: // 対象が20桁なら号、CSから存在判定
		getGpAddrRecs(gouPointTbl, adminAddrRecs, gpAddrRecs);
		exist = decideGouDigitStatus(gpAddrRecs, csAddrRecs, addrRec, existAddrRecs);
		break;
	default:
		break;
	}
	return exist;
}

void CheckUtil::getGpAddrRecs(
	sindy::CTableContainer & gouPointTbl,
	const AddrRecPairs & caAddrRecPairs,
	AddrRecs & gpAddrRecs)
{
	if (gpAddrRecs.empty())
	{
		m_addrRecHelper.makeAdrRecordsFromGp(gouPointTbl, caAddrRecPairs, gpAddrRecs);
	}
}

bool CheckUtil::decideAddrDigitStatus(
	const AddrRecPairs & caAddrRecPairs,
	const AddrRecs & csAddrRecs,
	const AddrRec & rec)
{
	const auto& foundCa = std::find_if
	(
		std::begin(caAddrRecPairs),
		std::end(caAddrRecPairs),
		[&rec](const AddrRecPair& addrRec)
	{
		return rec.getAdrcode11() == addrRec.first.getAdrcode11() &&
			addrRec.first.isMatchTarget();
	}
	);

	if (foundCa != caAddrRecPairs.end())
	{
		return true;
	}

	return judgeSameAddrRecByCs(csAddrRecs, rec);
}

bool CheckUtil::decideAddrDigitStatus(
	const addr::AddrRecPairs & caAddrRecPairs,
	const addr::AddrRecs & csAddrRecs,
	const addr::AddrRec & rec,
	addr::AddrRecs & existAddrRecs)
{
	for (const auto& addrRec : caAddrRecPairs)
	{
		if (rec.getAdrcode11() == addrRec.first.getAdrcode11() &&
			addrRec.first.isMatchTarget())
		{
			existAddrRecs.emplace_back(addrRec.first);
		}
	}

	if (!existAddrRecs.empty())
	{
		return true;
	}

	return judgeSameAddrRecByCs(csAddrRecs, rec, existAddrRecs);
}

bool CheckUtil::decideChibanDigitStatus(
	const AddrRecPairs & caAddrRecPairs,
	const AddrRecs & gpAddrRecs,
	const AddrRecs & csAddrRecs,
	const AddrRec & rec)
{
	const auto& foundCa = std::find_if
	(
		std::begin(caAddrRecPairs),
		std::end(caAddrRecPairs),
		[&rec](const AddrRecPair& addrRec)
	{
		return rec.getAdrcode11() == addrRec.first.getAdrcode11() &&
			rec.getChibanName() == addrRec.first.getChibanName() &&
			addrRec.first.isMatchTarget();
	}
	);

	if (foundCa != caAddrRecPairs.end())
	{
		return true;
	}

	const auto& foundGp = std::find_if
	(
		std::begin(gpAddrRecs),
		std::end(gpAddrRecs),
		[&rec](const AddrRec& addrRec)
	{
		return rec.getAdrcode11() == addrRec.getAdrcode11() &&
			rec.getChibanName() == addrRec.getChibanName();
	}
	);

	if (foundGp != gpAddrRecs.end())
	{
		return true;
	}

	return judgeSameAddrRecByCs(csAddrRecs, rec);
}

bool CheckUtil::decideChibanDigitStatus(
	const addr::AddrRecPairs & caAddrRecPairs,
	const addr::AddrRecs & gpAddrRecs,
	const addr::AddrRecs & csAddrRecs,
	const addr::AddrRec & rec,
	addr::AddrRecs & existAddrRecs)
{
	for (const auto& addrRec : caAddrRecPairs)
	{
		if (rec.getAdrcode11() == addrRec.first.getAdrcode11() &&
			rec.getChibanName() == addrRec.first.getChibanName() &&
			addrRec.first.isMatchTarget())
		{
			existAddrRecs.emplace_back(addrRec.first);
		}
	}

	if (!existAddrRecs.empty())
	{
		return true;
	}

	for (const auto& addrRec : gpAddrRecs)
	{
		if (rec.getAdrcode11() == addrRec.getAdrcode11() &&
			rec.getChibanName() == addrRec.getChibanName())
		{
			existAddrRecs.emplace_back(addrRec);
		}
	}

	if (!existAddrRecs.empty())
	{
		return true;
	}

	return judgeSameAddrRecByCs(csAddrRecs, rec, existAddrRecs);
}

bool CheckUtil::decideGouDigitStatus(
	const AddrRecs & gpAddrRecs,
	const AddrRecs & csAddrRecs,
	const AddrRec & rec)
{
	const auto& foundGp = std::find_if
	(
		std::begin(gpAddrRecs),
		std::end(gpAddrRecs),
		[&rec](const AddrRec& addrRec)
	{
		return rec.getAdrcode11() == addrRec.getAdrcode11() &&
			rec.getChibanName() == addrRec.getChibanName() &&
			rec.getGouName() == addrRec.getGouName();
	}
	);

	if (foundGp != gpAddrRecs.end())
	{
		return true;
	}

	return judgeSameAddrRecByCs(csAddrRecs, rec);
}

bool CheckUtil::decideGouDigitStatus(
	const addr::AddrRecs & gpAddrRecs,
	const addr::AddrRecs & csAddrRecs,
	const addr::AddrRec & rec,
	addr::AddrRecs & existAddrRecs)
{
	for (const auto& addrRec : gpAddrRecs)
	{
		if (rec.getAdrcode11() == addrRec.getAdrcode11() &&
			rec.getChibanName() == addrRec.getChibanName() &&
			rec.getGouName() == addrRec.getGouName())
		{
			existAddrRecs.emplace_back(addrRec);
		}
	}

	if (!existAddrRecs.empty())
	{
		return true;
	}

	return judgeSameAddrRecByCs(csAddrRecs, rec, existAddrRecs);
}

bool CheckUtil::judgeSameAddrRecByCs(
	const addr::AddrRecs & addrRecs,
	const addr::AddrRec & rec)
{
	const auto& found = std::find_if
	(
		std::begin(addrRecs),
		std::end(addrRecs),
		[&rec](const AddrRec& addrRec)
	{
		return (0 == addrRec.getJusyocode().Find(rec.getJusyocode()));
	}
	);

	if (found != addrRecs.end())
	{
		return true;
	}
	return false;
}

bool CheckUtil::judgeSameAddrRecByCs(
	const addr::AddrRecs & addrRecs,
	const addr::AddrRec & rec,
	addr::AddrRecs & existAddrRecs)
{
	for (const auto& addrRec : addrRecs)
	{
		if (0 == addrRec.getJusyocode().Find(rec.getJusyocode()))
		{
			existAddrRecs.emplace_back(addrRec);
		}
	}

	return !existAddrRecs.empty();
}
