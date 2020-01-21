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

/**
 * @file SiNDYHashSearcher.h
 * @brief ����Search�N���X�̒�`
 * @note ���̃w�b�_�ɒ�`����Ă���N���X�̎g�p�ɂ̓��C�u�������K�v�ł��B
 * @author �n�}DB���암���H����O���[�v �e�n�����Y
 * $Id$
 */
#ifndef _SINDY_HASH_SEARCHER_H_
#define _SINDY_HASH_SEARCHER_H_

#include <string>
#include "SiNDYDatasetName.h"
#include "SiNDYObjectIdRange.h"
#include "SiNDYRowSetSearcher.h"

namespace sindy {

#pragma warning(push)
#pragma warning(disable : 4251)
/**
 * @brief �I�u�W�F�N�gID���A������I�u�W�F�N�g���10�����i�f�t�H���g�j����Search����N���X
 *
 * �`��܂݂̃I�u�W�F�N�g����x�ɑ�ʂ�Search()����ƁA�T�[�o�ɑ傫�ȕ��ׂ��|����B
 */
class SINDYLIB_API HashSearcher
{
public:
	HashSearcher(
		IFeatureWorkspace* ipFeatureWorkspace,
		BSTR bstrTableName,
		IQueryFilter* ipQueryFilter = 0,
		int nStep = 100000);

// �ݒ�/�擾
	long first() const { return m_ObjectIdRange.first; }
	long last() const { return m_ObjectIdRange.second; }
	long current() const { return m_nCurrent; }

protected:
// ����
	IQueryFilter* nextQueryFilter();

private:
// �ϐ�
	IQueryFilterPtr m_ipQueryFilter; ///< �N�G���t�B���^
#ifdef _UNICODE
	std::wstring m_strWhereClause; ///< Where��iwchar_t�j
#else
	std::string m_strWhereClause; ///< Where��ichar�j
#endif

	ObjectIdRange m_ObjectIdRange; ///< OBJECTID�͈�
	int m_nStep; ///< ��x�Ɏ��o���I�u�W�F�N�g��

	long m_nCurrent; ///< ����Search()�̊J�n�I�u�W�F�N�gID
};
#pragma warning(pop)

template <typename TRow>
class RowHashSearcher_ : public RowSetSearcher_<TRow>, public HashSearcher
{
public:
	RowHashSearcher_(
		TableInterface* ipTable,
		VARIANT_BOOL vbRecycling,
		IQueryFilter* ipQueryFilter = 0,
		int nStep = 100000
	) :
	RowSetSearcher_<TRow>(ipTable),
	HashSearcher(FeatureWorkspacePtr(ipTable), CDatasetNameBSTR(ipTable), ipQueryFilter, nStep),
	m_vbRecycling(vbRecycling)
	{
	}

	RowHashSearcher_(
		IFeatureWorkspace* ipFeatureWorkspace,
		BSTR bstrTableName,
		VARIANT_BOOL vbRecycling,
		IQueryFilter* ipQueryFilter = 0,
		int nStep = 100000
	) :
	RowSetSearcher_<TRow>(getData<row_traits::TableInterfacePtr>(ipFeatureWorkspace, bstrTableName)),
	HashSearcher(ipFeatureWorkspace, bstrTableName, ipQueryFilter, nStep),
	m_vbRecycling(vbRecycling)
	{
	}

// �������z�֐��̉���
	virtual HRESULT __stdcall SearchNext(CursorInterface** ppCursor)
	{
		IQueryFilter* ipQueryFilter = nextQueryFilter();
		return ipQueryFilter ? table()->Search(ipQueryFilter, m_vbRecycling, ppCursor) : S_FALSE;
	}

private:
	VARIANT_BOOL m_vbRecycling;
};

typedef RowHashSearcher_<_IRow> RowHashSearcher; ///< IRow�d�l��_RowHashSearcher�ɖ��O�t��
typedef RowHashSearcher_<IFeature> FeatureHashSearcher; ///< IFeature�d�l��_RowHashSearcher�ɖ��O�t��

// traits

template <>
struct pointer_traits<RowHashSearcher*> : public _row_set_searcher_traits<RowSetSearcher> {};

template <>
struct pointer_traits<FeatureHashSearcher*> : public _row_set_searcher_traits<FeatureSetSearcher> {};

/**
 * @addtogroup for_each
 * @brief _RowHashSearcher�n��p��for_each()�B
 *
 * @note TFunction���Q�Ɖ�����ɂ�boost::call_taits.hpp���K�v�B
 */
/*@{*/

// for_each

#include "detail/def_reference_to.h"

template <typename TFunction, typename TRow>
inline TFunction for_each_row(RowHashSearcher_<TRow> cSearcher, TFunction f)
{
	for_each_element<_IRow*, _REFERENCE_TO(TFunction)>(&cSearcher, f);
	return f;
}

#include "detail/undef_reference_to.h"

/*@}*/

} // namespace sindy

#endif // _SINDY_HASH_SEARCHER_H_
