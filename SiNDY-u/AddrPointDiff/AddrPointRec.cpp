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
		// �Z���������S�p�����A�f�[�^��r�p�̕ϐ��Ɋi�[����
		addressComp = ToFull(addressStr);

		// ���ԃt�@�C���Z����ADDRESS1�`3�Ɋi�[�����n�C�t���́u�]�v(815D)�ł��邪�A
		// ���p�n�C�t���i-�j��ToFull�őS�p������Ɓu-�v���u�|�v(817C)�ɂȂ��Ă��܂��A
		// �P���ɔ�r���o���Ȃ��̂ŁA�����Łu�|�v(817C�j���u�]�v(815D�j�ɒu��������
		addressComp.Replace(_T("�|"), SEPARATOR_FULL_HYPHEN);
	}

void AddrPointRecSet::setAddrPointRecs(const AddrPointRec& addrPointRec)
{
	m_addrPointRecs[addrPointRec.geoBldId].emplace_back(addrPointRec);
}


void AddrPointRecSet::getAddrPointRecs(AddrPointRecs& addrPointRecs)
{
	// m_addrPointRecs�Ɋi�[���ꂽ�Z���|�C���g��AddrPointRecs�`���ɂ���
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
	// m_addrPointRecs�Ɋi�[���ꂽ�Z���|�C���g��AddrPointRecsByCode�`���ɂ���
	// �Ƃ����Ă��A���X�����o�ϐ��ł͂��̌`�ŕێ����Ă���̂ŁA���̂܂�
	addrPointRecs = m_addrPointRecs;
}

AddrPointWithSortKey selectNearestAddress(
		std::vector<AddrPointWithSortKey>& recs,
		AddrPointWithSortKey& ipcPointWithSortKey,
		bool isConsiderDist)
{
	std::vector<AddrPointWithSortKey> minDistAddrPoints;
	double minDist = DBL_MAX;

	// �������l�����đI����s���ꍇ�A��������ԒZ�����̂�T��
	if( isConsiderDist )
	{
		for(const auto& rec : recs)
		{
			// �ێ����������l���傫���ꍇ�̓X�L�b�v
			if( minDist < rec.second.getDist() )
				continue;
			
			// �ێ��������������̏ꍇ�́A�~�ς����Z���|�C���g�Q�͔j��
			if( minDist > rec.second.getDist() )
				minDistAddrPoints.clear();

			minDist = rec.second.getDist();
			minDistAddrPoints.push_back(rec);
		}			
	}

	// �I��O�ɃR���e�i����iPC�Z���|�C���g���ǉ�����
	auto targetRecs = ( minDistAddrPoints.empty() ) ? recs : minDistAddrPoints;
	targetRecs.push_back(ipcPointWithSortKey);

	// �\�[�g
	std::sort(
			targetRecs.begin(), 
			targetRecs.end(),
			[isConsiderDist](const AddrPointWithSortKey& left, const AddrPointWithSortKey& right)
				{ return ( left.second.equalsWithConditions( right.second, isConsiderDist) ) ?
							*(left.first) < *(right.first) :
							left.second.comp(right.second, isConsiderDist); }
		);

	// iPC�Z���|�C���g�̈ʒu���擾
	auto& fi = std::find_if(
					targetRecs.begin(),
					targetRecs.end(),
					[](const AddrPointWithSortKey& rec) { return rec.second.isTarget(); }
				);

	// iPC�Z���|�C���g���擪�̏ꍇ�́A1���Ԃ�
	if( fi == targetRecs.begin() )
		return *(fi+1);

	// iPC�Z���|�C���g�������̏ꍇ�́A1�O��Ԃ�
	if( fi == targetRecs.end()-1 )
		return *(fi-1);

	// ���S�ɒl�������ꍇ�́A�����Ԃ�
	// �i�����l�����X�g�ɑ��݂���ꍇ�AiPC�Z���|�C���g�́A���̒��ň�Ԍ�ɂȂ�悤�ɂ��Ă���̂ŁA1�O��Ԃ��j
	if( (*(fi-1)) == (*fi) )
		return *(fi-1);

	// �O��̗v�f�̈�v���x�������āA����v���Ă������Ԃ�
	auto& before = *(fi-1);
	auto& after = *(fi+1);
	int beforeMatchLvl = before.second.getMatchLevel((*fi).second);
	int afterMatchLvl = after.second.getMatchLevel((*fi).second);
	bool isNumBefore = isNumber( before.second.getElement(beforeMatchLvl) );
	bool isNumAfter = isNumber( after.second.getElement(afterMatchLvl) );

	// �v�f�̈�v���x���������ꍇ�ŁA�l�̈�v���Ȃ��擪�v�f���A����́u���l�̂݁v�ŁA
	// ��������́u�������܂ށv�Ƃ�������Ԃ̏ꍇ�A
	// �Ώۂ̒l�����l�̂�/�����񂩂ɂ���āA�ǂ��炩�ɂ悹��
	// �y��z�O�F�u�P�|�S�|�T�v�AiPC�Z���|�C���g�F�u�P�|�T�v�A��F�u�P�|�b�[�R�v�̏ꍇ
	//       2�v�f�ڂ����ꂼ��u�S�v�u�b�v�ł���A�����ɍ��v���A
	//       iPC�Z���́u�T�v�Ő��l�Ȃ̂ŁA���̏ꍇ�́A�O�i�u�P�|�S�|�T�v�j��Ԃ�
	//       �i�u�T�v�̕������������܂ޒl�Ȃ�A��i�u�P�|�b�|�R�v�j��Ԃ����ƂɂȂ�j
	if( (beforeMatchLvl == afterMatchLvl) && (isNumBefore != isNumAfter) )
	{
		if( isNumber( (*fi).second.getElement(beforeMatchLvl) ) )
			return ( isNumBefore ) ? before : after;
		else
			return ( isNumBefore ) ? after : before;

	}

	return ( beforeMatchLvl <= afterMatchLvl ) ? after : before;
}
