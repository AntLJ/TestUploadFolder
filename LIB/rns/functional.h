/*
 * Copyright (C) INCREMENT P CORP. All Rights Reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY INCREMENT P CORP., WITHOUT WARRANTY OF
 * ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT.
 *
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDER BE LIABLE FOR ANY
 * CLAIM, DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING
 * OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.
*/

#ifndef RNS_FUNCTIONAL_H_
#define RNS_FUNCTIONAL_H_

#include <functional>
#include <boost/type_traits.hpp>
#include <boost/mpl/if.hpp>

namespace rns {

template <class T>
struct identity : public std::unary_function<T, T>
{
	const T& operator()(const T& x) const { return x; }
};

/**
 * @brief std::map �Ɋi�[���ꂽ�v�f�̃|�C���^���L�[�l�w��Ŏ��o���֐��N���X�B
 *
 * &map[key] �Ƃ͈قȂ�A�i�[����Ă��Ȃ��L�[�l���w�肵���ꍇ��NULL��Ԃ��B
 *
 * @note
 * &map[key] ����������֐��I�u�W�F�N�g�́A Boost.Lambda �� &var(map)[_1] �ƋL�q�ł��邽�߁A�֐��N���X������قǂ̂��̂ł͂Ȃ��B
 */
template <typename MapType>
class mapped_value_referer
{
	typedef
		typename boost::mpl::if_<
			boost::is_const<MapType>,
			typename MapType::const_iterator,
			typename MapType::iterator
		>::type iterator_type;
public:
	typedef
		typename boost::mpl::if_<
			boost::is_const<MapType>,
			const typename MapType::mapped_type*,
			typename MapType::mapped_type*
		>::type result_type;

	typedef
		typename boost::mpl::if_<
			boost::is_const<MapType>,
			const typename MapType::key_type,
			typename MapType::key_type
		>::type argument_type;

	mapped_value_referer(MapType& rMap) :
	m_rMap(rMap)
	{
	}

	result_type operator()(const argument_type& rKey) const
	{
		const iterator_type it(m_rMap.find(rKey));
		return it != m_rMap.end() ? &it->second : 0;
	}

private:
	MapType& m_rMap;
};

template <typename MapType>
mapped_value_referer<MapType> make_mapped_value_referer(MapType& rMap)
{
	return mapped_value_referer<MapType>(rMap);
}

} // namespace rns

#endif // RNS_FUNCTIONAL_H_
