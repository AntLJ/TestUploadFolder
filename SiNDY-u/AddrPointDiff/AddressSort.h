#pragma once

#include "stdafx.h"
#include "Utility.h"

/**
 * @brief �Z��������\�[�g�R���e�i
 */
class AddressSort
{
public:

	/**
	 * @brief �R���X�g���N�^
	 * @param[in] address   �Z��������
	 * @param[in] dist  �@�@����
	 * @param[in] isTarget  �����Ώۂ��ǂ����itrue:�����ΏہAfalse:�����Ώۂł͂Ȃ��j
	 */
	AddressSort(const CString& address, double dist, bool isTarget);

	/**
	 * @brief �Z����������̎w��v�f�̒l��Ԃ�
	 * @retval �Z��������i�w��v�f�l�j
	 */
	const CString getElement(unsigned int index) const
	{
		return ( m_splitAddress.size() > index ) ? m_splitAddress[index] : _T("");
	}

	/**
	 * @brief �A�N�Z�T�igetter�F�Z��������j
	 * @retval �Z��������
	 */
	const CString getAddress() const
	{
		return m_address;
	}

	/**
	 * @brief �A�N�Z�T�igetter�F�����j
	 * @retval ����
	 */
	const double getDist() const
	{
		return m_dist;
	}

	/**
	 * @brief �A�N�Z�T�igetter�F�����Ώۂ��ǂ����j
	 * @retval �����ΏۗL��
	 */
	const bool isTarget() const
	{
		 return m_isTarget;
	}

	/**
	 * @brief �A�N�Z�T�igetter�F�Z��������i�v�f�����j
	 * @retval �Z��������i�v�f�����j
	 */
	const std::vector<CString>& getSplitAddress() const
	{
		return m_splitAddress;
	}

	/**
	 * @brief �w�肵���Z��������Ƃ̗v�f�̈�v���x�����擾����
	 * @param[in] other   �Z��������\�[�g�R���e�i
	 * @retval ��v���x��
	 */
	int getMatchLevel(const AddressSort& other);

	/**
	 * @brief 2�̏Z��������R���e�i���r���A�召������s��
	 * @param[in] other           �Z��������\�[�g�R���e�i�i��r��j
	 * @param[in] isConsiderDist  �召����̍ہA�����������Ɏg�p���邩�itrue:�g�p����Afalse:�g�p���Ȃ��j
	 * @retval true ��r���̃R���e�i�̕���������
	 * @retval false ��r��̃R���e�i�̕���������
	 */
	bool comp(const AddressSort& other, bool isConsiderDist) const
	{
		// �召����̍ہA�������g�p����ꍇ
		if( isConsiderDist ) 
		{
			// �����������łȂ���΁A�������Z������������
			if( getDist() != other.getDist() )
				return getDist() < other.getDist();
		}

		int baseSize = m_splitAddress.size();
		int otherSize = other.getSplitAddress().size();

		// �e�v�f���̒l�̑召��r���s���i��菬�����v�f���Ń��[�v���񂷁j
		int maxSize = (baseSize <= otherSize) ? baseSize : otherSize;
		for(int i = 0; i < maxSize; ++i)
		{
			// ���l���ǂ���
			bool isNumberBase = isNumber( m_splitAddress[i] );
			bool isNumberOther = isNumber( other.getSplitAddress()[i] );

			// �v�f��������
			bool isEndBase = ( m_splitAddress.size() == i+1 );
			bool isEndOther = ( other.getSplitAddress().size() == i+1 );

			// �����Ƃ����l�̏ꍇ�A���l�Ŕ�r
			if( isNumberBase && isNumberOther )
			{
				int valueBase = _tstol(m_splitAddress[i]);
				int valueOther = _tstol(other.getSplitAddress()[i]);

				// �l���قȂ�ꍇ�́A���l�̑召�Ŕ���
				if( valueBase != valueOther ) return valueBase < valueOther;

				// �����Ƃ������̗v�f�̏ꍇ�A�^�[�Q�b�g�iisTarget=true�j�̕�����ɂ���悤�ɂ���
				if( isEndBase && isEndOther )
					return ( other.isTarget() && !isTarget() );

				// �ǂ��炩�̗v�f�������̏ꍇ�A���v�f��������������ɂ���悤�ɂ���
				if( isEndBase || isEndOther )
					return ( isEndBase );

				// �v�f�l�����S��v����ꍇ�́A���̗v�f�ő召��r���s��
				if( valueBase == valueOther )
					continue;
			}

			// �ǂ��炩�����l�ł͂Ȃ��ꍇ�A������Ŕ�r
			if( m_splitAddress[i] != other.getSplitAddress()[i] )
				return m_splitAddress[i] < other.getSplitAddress()[i];

			if( isEndBase && isEndOther )
				return ( other.isTarget() && !isTarget());

			if( isEndBase || isEndOther )
				return ( isEndBase );

			if( m_splitAddress[i] == other.getSplitAddress()[i] )
				continue;
		}	
		return true;
	}
	
	/**
	 * @brief 2�̏Z��������R���e�i���������̂����肷��
	 * @param[in] other           �Z��������\�[�g�R���e�i�i��r��j
	 * @param[in] isConsiderDist  ����̍ہA�����������Ɏg�p���邩�itrue:�g�p����Afalse:�g�p���Ȃ��j
	 * @retval true ����
	 * @retval false �قȂ�
	 */
	bool equalsWithConditions(const AddressSort& other, bool isConsiderDist) const
	{
		return ( isConsiderDist ) ? 
				( ( getAddress() == other.getAddress() ) && ( getDist() == other.getDist() ) ) :
				( getAddress() == other.getAddress() );
	}

	bool operator == (const AddressSort& other) const
	{
		return ( ( getAddress() == other.getAddress() ) && ( getDist() == other.getDist() ) );
	}

private:

	CString m_address;                     //!< �Z��������
	std::vector<CString> m_splitAddress;   //!< �Z��������i�v�f������j
	double m_dist;                         //!< ����
	bool m_isTarget;                       //!< �����Ώۂ̏Z���|�C���g���ǂ���
};



