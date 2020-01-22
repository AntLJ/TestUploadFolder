#pragma once

#include "stdafx.h"
#include "Utility.h"

/**
 * @brief 住所文字列ソートコンテナ
 */
class AddressSort
{
public:

	/**
	 * @brief コンストラクタ
	 * @param[in] address   住所文字列
	 * @param[in] dist  　　距離
	 * @param[in] isTarget  検索対象かどうか（true:検索対象、false:検索対象ではない）
	 */
	AddressSort(const CString& address, double dist, bool isTarget);

	/**
	 * @brief 住所文字列内の指定要素の値を返す
	 * @retval 住所文字列（指定要素値）
	 */
	const CString getElement(unsigned int index) const
	{
		return ( m_splitAddress.size() > index ) ? m_splitAddress[index] : _T("");
	}

	/**
	 * @brief アクセサ（getter：住所文字列）
	 * @retval 住所文字列
	 */
	const CString getAddress() const
	{
		return m_address;
	}

	/**
	 * @brief アクセサ（getter：距離）
	 * @retval 距離
	 */
	const double getDist() const
	{
		return m_dist;
	}

	/**
	 * @brief アクセサ（getter：検索対象かどうか）
	 * @retval 検索対象有無
	 */
	const bool isTarget() const
	{
		 return m_isTarget;
	}

	/**
	 * @brief アクセサ（getter：住所文字列（要素分割）
	 * @retval 住所文字列（要素分割）
	 */
	const std::vector<CString>& getSplitAddress() const
	{
		return m_splitAddress;
	}

	/**
	 * @brief 指定した住所文字列との要素の一致レベルを取得する
	 * @param[in] other   住所文字列ソートコンテナ
	 * @retval 一致レベル
	 */
	int getMatchLevel(const AddressSort& other);

	/**
	 * @brief 2つの住所文字列コンテナを比較し、大小判定を行う
	 * @param[in] other           住所文字列ソートコンテナ（比較先）
	 * @param[in] isConsiderDist  大小判定の際、距離を条件に使用するか（true:使用する、false:使用しない）
	 * @retval true 比較元のコンテナの方が小さい
	 * @retval false 比較先のコンテナの方が小さい
	 */
	bool comp(const AddressSort& other, bool isConsiderDist) const
	{
		// 大小判定の際、距離を使用する場合
		if( isConsiderDist ) 
		{
			// 距離が同じでなければ、距離が短い方が小さい
			if( getDist() != other.getDist() )
				return getDist() < other.getDist();
		}

		int baseSize = m_splitAddress.size();
		int otherSize = other.getSplitAddress().size();

		// 各要素内の値の大小比較を行う（より小さい要素数でループを回す）
		int maxSize = (baseSize <= otherSize) ? baseSize : otherSize;
		for(int i = 0; i < maxSize; ++i)
		{
			// 数値かどうか
			bool isNumberBase = isNumber( m_splitAddress[i] );
			bool isNumberOther = isNumber( other.getSplitAddress()[i] );

			// 要素が末尾か
			bool isEndBase = ( m_splitAddress.size() == i+1 );
			bool isEndOther = ( other.getSplitAddress().size() == i+1 );

			// 両方とも数値の場合、数値で比較
			if( isNumberBase && isNumberOther )
			{
				int valueBase = _tstol(m_splitAddress[i]);
				int valueOther = _tstol(other.getSplitAddress()[i]);

				// 値が異なる場合は、数値の大小で判定
				if( valueBase != valueOther ) return valueBase < valueOther;

				// 両方とも末尾の要素の場合、ターゲット（isTarget=true）の方が後にくるようにする
				if( isEndBase && isEndOther )
					return ( other.isTarget() && !isTarget() );

				// どちらかの要素が末尾の場合、より要素数が多い方が後にくるようにする
				if( isEndBase || isEndOther )
					return ( isEndBase );

				// 要素値が完全一致する場合は、次の要素で大小比較を行う
				if( valueBase == valueOther )
					continue;
			}

			// どちらかが数値ではない場合、文字列で比較
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
	 * @brief 2つの住所文字列コンテナが同じものか判定する
	 * @param[in] other           住所文字列ソートコンテナ（比較先）
	 * @param[in] isConsiderDist  判定の際、距離を条件に使用するか（true:使用する、false:使用しない）
	 * @retval true 同じ
	 * @retval false 異なる
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

	CString m_address;                     //!< 住所文字列
	std::vector<CString> m_splitAddress;   //!< 住所文字列（要素分割後）
	double m_dist;                         //!< 距離
	bool m_isTarget;                       //!< 検索対象の住所ポイントかどうか
};



