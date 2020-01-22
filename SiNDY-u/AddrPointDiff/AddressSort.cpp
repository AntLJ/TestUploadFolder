#include "stdafx.h"
#include "AddressSort.h"
#include "Utility.h"

AddressSort::AddressSort(
	const CString& address,
	double dist,
	bool isTarget)
	: m_address(address), m_dist(dist), m_isTarget(isTarget)
{
	// 全角ハイフンで分割
	m_splitAddress = uh::str_util::split(address, SEPARATOR_FULL_HYPHEN, false);
}

int AddressSort::getMatchLevel(const AddressSort& other)
{
	int matchLevel = 0;
	int baseSize = m_splitAddress.size();
	int otherSize = other.getSplitAddress().size();

	// セパレータで分割した要素同士の一致レベル（階層）を確認
	int maxSize = (baseSize <= otherSize) ? baseSize : otherSize;
	for(int i = 0; i < maxSize; ++i)
	{
		if( m_splitAddress[i] != other.getSplitAddress()[i] )
			break;

		++matchLevel;
	}	

	return matchLevel;
}

