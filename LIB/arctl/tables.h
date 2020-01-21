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

#ifndef ARCTL_TABLES_H_
#define ARCTL_TABLES_H_

#include <boost/preprocessor.hpp>
#include <atl2/named_interface_pointers.h>

namespace arctl {

/**
 * @brief ���O�t�� ITable �|�C���^�N���X
 */
template <LPCTSTR TableName>
class table_t : public atl2::named_interface_pointer<TableName, ITablePtr> {};

/**
 * @brief ���O�t�� IFeatureClass �|�C���^�N���X
 */
template <LPCTSTR TableName>
class feature_class_t : public atl2::named_interface_pointer<TableName, IFeatureClassPtr> {};

/**
 * @brief table_t �p���N���X���d�p���N���X
 * 
 * @par ����:
 * - @a TableList - table_t �p���N���X�V�[�P���X
 *
 * @code
 * tables<
 *     boost::mpl::list<
 *         feature_class_t<road_link::kTableName>,
 *         feature_class_t<road_node::kTableName>,
 *         table_t<no_passage::kTableName>
 *     >
 * >
 * @endcode
 */
template <typename TableList>
class tables : public atl2::named_interface_pointers<TableList>
{
};

} // namespace arctl

/**
 * @brief tables ��`�ȗ����}�N��
 *
 * @param c
 * ��`����� tables �̖��O�B
 *
 * @param seq
 * �p������ table_t �p���N���X��<a href="http://boost.cppll.jp/HEAD/libs/preprocessor/doc/data/sequences.html">�V�[�P���X</a>�B<br>
 * table_t �p���N���X�̎w����@�͈ȉ���3��ށB
 * -# table_t �p���N���X�𒼐ڎw�肷��
 *   - <b>(table_t �p���N���X)</b>
 * -# table_t �N���X�̃e�[�u�����̂��w�肷��
 *   - <b>T(�e�[�u������)</b>
 * - feature_class_t �N���X�̃t�B�[�`���N���X���̂��w�肷��
 *   - <b>F(�t�B�[�`���N���X����)</b>
 * 
 * @code
 * ARCTL_TABLES_CLASS(
 *     Tables,
 *     (F(road_link::kTableName))
 *     (F(road_node::kTableName))
 *     (T(no_passage::kTableName))
 * );
 * @endcode
 *
 * @test TablesTest.cpp
 */
#define ARCTL_TABLES_CLASS(c, seq) \
	typedef arctl::tables<boost::mpl::list<BOOST_PP_SEQ_FOR_EACH_I(ARCTL_TABLES_CLASS_LIST, _, seq) > > c

#define ARCTL_TABLES_CLASS_EXPAND(elem) BOOST_PP_CAT(ARCTL_TABLES_CLASS_EXPAND_, elem)
#define ARCTL_TABLES_CLASS_EXPAND_(elem) elem
#define ARCTL_TABLES_CLASS_EXPAND_F(elem) arctl::feature_class_t<elem>
#define ARCTL_TABLES_CLASS_EXPAND_T(elem) arctl::table_t<elem>

#define ARCTL_TABLES_CLASS_LIST(r, data, i, elem) BOOST_PP_COMMA_IF(i) ARCTL_TABLES_CLASS_EXPAND(elem)

#endif // ARCTL_TABLES_H_
