#include "stdafx.h"
#include "AddressSort.h"
#include "Utility.h"

AddressSort::AddressSort(
	const CString& address,
	double dist,
	bool isTarget)
	: m_address(address), m_dist(dist), m_isTarget(isTarget)
{
	// �S�p�n�C�t���ŕ���
	m_splitAddress = uh::str_util::split(address, SEPARATOR_FULL_HYPHEN, false);
}

int AddressSort::getMatchLevel(const AddressSort& other)
{
	int matchLevel = 0;
	int baseSize = m_splitAddress.size();
	int otherSize = other.getSplitAddress().size();

	// �Z�p���[�^�ŕ��������v�f���m�̈�v���x���i�K�w�j���m�F
	int maxSize = (baseSize <= otherSize) ? baseSize : otherSize;
	for(int i = 0; i < maxSize; ++i)
	{
		if( m_splitAddress[i] != other.getSplitAddress()[i] )
			break;

		++matchLevel;
	}	

	return matchLevel;
}

