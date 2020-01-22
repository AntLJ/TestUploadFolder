#include "stdafx.h"
#include "AddrPointRec.h"
#include "Utility.h"
#include "AddressSort.h"


AddrPointRec::AddrPointRec(
	AddrPointType pointType,
	long objectId,
	const CString& addrcode11,
	const CString& addressStr,
	const IGeometryPtr& geometry,
	const CString& geoBuildingId ) :
		addrPointType(pointType), oid(objectId), addrcode(addrcode11), 
		address(addressStr), geo(geometry), isInBuilding(false), geoBldId(geoBuildingId), 
		addrExistAttr(AddrExistAttr::NoExist), isExistOtherPointInSameBld(false)
	{ 
		// 住所文字列を全角化し、データ比較用の変数に格納する
		addressComp = ToFull(addressStr);

		// 中間ファイル住所のADDRESS1～3に格納されるハイフンは「‐」(815D)であるが、
		// 半角ハイフン（-）をToFullで全角化すると「-」→「－」(817C)になってしまい、
		// 単純に比較が出来ないので、ここで「－」(817C）→「‐」(815D）に置き換える
		addressComp.Replace(_T("－"), SEPARATOR_FULL_HYPHEN);
	}

void AddrPointRecSet::setAddrPointRecs(const AddrPointRec& addrPointRec)
{
	m_addrPointRecs[addrPointRec.geoBldId].emplace_back(addrPointRec);
}


void AddrPointRecSet::getAddrPointRecs(AddrPointRecs& addrPointRecs)
{
	// m_addrPointRecsに格納された住所ポイントをAddrPointRecs形式にする
	for(auto& recs : m_addrPointRecs)
	{
		for(auto& rec : recs.second)
		{
			addrPointRecs.emplace_back(rec);
		}
	}
}

void AddrPointRecSet::getAddrPointRecs(AddrPointRecsByCode& addrPointRecs)
{
	// m_addrPointRecsに格納された住所ポイントをAddrPointRecsByCode形式にする
	// といっても、元々メンバ変数ではその形で保持しているので、そのまま
	addrPointRecs = m_addrPointRecs;
}

AddrPointWithSortKey selectNearestAddress(
		std::vector<AddrPointWithSortKey>& recs,
		AddrPointWithSortKey& ipcPointWithSortKey,
		bool isConsiderDist)
{
	std::vector<AddrPointWithSortKey> minDistAddrPoints;
	double minDist = DBL_MAX;

	// 距離を考慮して選定を行う場合、距離が一番短いものを探す
	if( isConsiderDist )
	{
		for(const auto& rec : recs)
		{
			// 保持した距離値より大きい場合はスキップ
			if( minDist < rec.second.getDist() )
				continue;
			
			// 保持した距離未満の場合は、蓄積した住所ポイント群は破棄
			if( minDist > rec.second.getDist() )
				minDistAddrPoints.clear();

			minDist = rec.second.getDist();
			minDistAddrPoints.push_back(rec);
		}			
	}

	// 選定前にコンテナ内にiPC住所ポイントも追加する
	auto targetRecs = ( minDistAddrPoints.empty() ) ? recs : minDistAddrPoints;
	targetRecs.push_back(ipcPointWithSortKey);

	// ソート
	std::sort(
			targetRecs.begin(), 
			targetRecs.end(),
			[isConsiderDist](const AddrPointWithSortKey& left, const AddrPointWithSortKey& right)
				{ return ( left.second.equalsWithConditions( right.second, isConsiderDist) ) ?
							*(left.first) < *(right.first) :
							left.second.comp(right.second, isConsiderDist); }
		);

	// iPC住所ポイントの位置を取得
	auto& fi = std::find_if(
					targetRecs.begin(),
					targetRecs.end(),
					[](const AddrPointWithSortKey& rec) { return rec.second.isTarget(); }
				);

	// iPC住所ポイントが先頭の場合は、1つ後を返す
	if( fi == targetRecs.begin() )
		return *(fi+1);

	// iPC住所ポイントが末尾の場合は、1つ前を返す
	if( fi == targetRecs.end()-1 )
		return *(fi-1);

	// 完全に値が同じ場合は、それを返す
	// （同じ値がリストに存在する場合、iPC住所ポイントは、その中で一番後になるようにしているので、1つ前を返す）
	if( (*(fi-1)) == (*fi) )
		return *(fi-1);

	// 前後の要素の一致レベルを見て、より一致している方を返す
	auto& before = *(fi-1);
	auto& after = *(fi+1);
	int beforeMatchLvl = before.second.getMatchLevel((*fi).second);
	int afterMatchLvl = after.second.getMatchLevel((*fi).second);
	bool isNumBefore = isNumber( before.second.getElement(beforeMatchLvl) );
	bool isNumAfter = isNumber( after.second.getElement(afterMatchLvl) );

	// 要素の一致レベルが同じ場合で、値の一致しない先頭要素が、一方は「数値のみ」で、
	// もう一方は「文字を含む」といった状態の場合、
	// 対象の値が数値のみ/文字列かによって、どちらかによせる
	// 【例】前：「１－４－５」、iPC住所ポイント：「１－５」、後：「１－甲ー３」の場合
	//       2要素目がそれぞれ「４」「甲」であり、条件に合致し、
	//       iPC住所は「５」で数値なので、この場合は、前（「１－４－５」）を返す
	//       （「５」の部分が文字を含む値なら、後（「１－甲－３」）を返すことになる）
	if( (beforeMatchLvl == afterMatchLvl) && (isNumBefore != isNumAfter) )
	{
		if( isNumber( (*fi).second.getElement(beforeMatchLvl) ) )
			return ( isNumBefore ) ? before : after;
		else
			return ( isNumBefore ) ? after : before;

	}

	return ( beforeMatchLvl <= afterMatchLvl ) ? after : before;
}
