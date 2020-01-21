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

#ifndef __SINDY_INCLAUSE_STREAM_H__
#define __SINDY_INCLAUSE_STREAM_H__

#include <functional>
#include <string>
#include <sstream>
#include "SiNDYTraits.h"

namespace sindy {

/**
 * @brief IN���Z�X�g���[��
 *
 * IN���Z���ɗv�f�𗭂ߍ���ł����A��萔�i�f�t�H���g1000�j�ɒB�����牉�Z����K�p����N���X�B
 *
 * - TFunction : std::unary_function<LPCTSTR, void>
 *
 * @note �������z�N���XSynchronizer�ւ̎Q�Ƃ��P�����Ƃ��������X�}�[�g��������Ȃ��B
 * @note Synchronizer�p���N���X���֐����Œ�`���Ďg���邵�B
 */
template <typename TFunction>
class InClauseStream
{
public:
// �\�z/����
	/**
	 * @param cFunction [in] IN���Z����P�������Ƃ���t�@���N�^�B
	 * @param lpszFieldName [in] �����t�B�[���h���B
	 * @param nMaxCount [in] IN���Z���ő�v�f���B
	 */
	InClauseStream(TFunction cFunction, LPCTSTR lpszFieldName, unsigned int nMaxCount = 1000) :
	m_Function(cFunction),
	m_strFieldName(lpszFieldName),
	m_nMaxCount(nMaxCount),
	m_nCount(0)
	{
	}

	InClauseStream(const InClauseStream& rStream) :
	m_Function(rStream.m_Function),
	m_strFieldName(rStream.m_strFieldName),
	m_nMaxCount(rStream.m_nMaxCount),
	m_nCount(0)
	{
	}

	~InClauseStream()
	{
		flush();
	}

// ����
	/**
	 * @brief sync()���AIN���Z������ɂ���
	 *
	 * �v�f����0�̏ꍇ�͉������Ȃ��B
	 */
	void flush()
	{
		// �v�f����0�̏ꍇ�͉������Ȃ�
		if(m_nCount == 0)
			return;

		// IN���Z�������
		m_WhereClauseStream << _T(')');

		// IN���Z����K�p����
		sync(m_WhereClauseStream.str().c_str());

		// IN���Z������ɂ���
		m_WhereClauseStream.str(_T(""));
		m_nCount = 0;
	}

// ���Z�q�̃I�[�o�[���C�h
	/**
	 * @brief IN���Z���ɗv�f��ǉ�����B
	 *
	 * �v�f�����ő�v�f���ɒB������flush()�B
	 *
	 * @param rT [in] �ǉ�����v�f�B
	 */
	template <typename T>
	InClauseStream& operator<<(T const& rT)
	{
		std::locale loc(std::locale("japanese"), "C", std::locale::numeric);
		m_WhereClauseStream.imbue(loc);
		if(m_nCount) {
			m_WhereClauseStream << _T(',');
		}
		else {
			// IN���Z����V���ɍ\�z����
			m_WhereClauseStream << m_strFieldName.c_str() << _T(" IN (");
		}

		// IN���Z���ɗv�f��ǉ�����
		m_WhereClauseStream << rT;

		// �v�f�����ő�v�f���ɒB������flush()
		if(++m_nCount == m_nMaxCount) {
			flush();
		}

		return *this;
	}

	/**
	 * @brief IN���Z���ɗv�f��ǉ�����B
	 */
	template <typename T>
	void operator()(T const& rT)
	{
		std::locale loc(std::locale("japanese"), "C", std::locale::numeric);
		operator.imbue(loc);
		operator<<(rT);
	}

private:
	/**
	 * @brief IN���Z����K�p����
	 *
	 * @param lpszWhereClause [in] IN���Z���B
	 */
	void sync(LPCTSTR lpszWhereClause)
	{
		m_Function(lpszWhereClause);
	}

// �ϐ�
	TFunction m_Function; ///< IN���Z���K�p�t�@���N�^
	unsigned int m_nMaxCount; ///< IN���Z���ő�v�f��
	unsigned int m_nCount; ///< IN���Z�����ݗv�f��
#ifdef _UNICODE
	std::wstring m_strFieldName; ///< �����t�B�[���h��
	std::wostringstream m_WhereClauseStream; ///< IN���Z��
#else
	std::string m_strFieldName; ///< �����t�B�[���h��
	std::ostringstream m_WhereClauseStream; ///< IN���Z��
#endif // _UNICODE
};

/**
 * @brief Where�喈�ɃT�[�`����N���X�B
 *
 * - TFunction : TRow�������Ƃ���P���֐��I�u�W�F�N�g�B���邢�͂��̎Q�ƁB
 * - TRow : TFunction�̈����^�iIRow��IFeature�j�B
 *
 * @note TFunction�́A�֐��I�u�W�F�N�g�ւ̎Q�Ƃł��A�֐��I�u�W�F�N�g���g�ł��ǂ��B
 * @note ��҂̏ꍇ�A��񂶂�boost::call_traits���C���N���[�h���Ă����Ȃ��ƁAfor_each�̓x�ɃR�s�[�R���X�g���N�^�������B
 */
template <typename TFunction, typename TRow>
class SearchByWhereClause
{
public:
	/**
	 * @brief �T�[�`���邽�߂̏���o�^����B
	 *
	 * @param cFunction [in] �T�[�`���ʂ̏������s���P���֐��I�u�W�F�N�g�B�R���X�g���N�^�ɓn�����_�ł͖��������ł��\��Ȃ��B
	 * @param ipTable [in,ref] �T�[�`�Ώۃe�[�u���B
	 * @param vbRecycling [in] IRow�̍ė��p���s�����H
	 * @param ipQueryFilter [in,optional] �T�[�`�ɗp����N�G���t�B���^�B�������AWhere��͖��������B
	 */
	SearchByWhereClause(
		TFunction cFunction,
		traits<TRow>::TableInterface* ipTable,
		VARIANT_BOOL vbRecycling,
		IQueryFilter* ipQueryFilter = 0) :
	m_Function(cFunction),
	m_ipTable(ipTable),
	m_vbRecycling(vbRecycling),
	m_ipQueryFilter(ipQueryFilter)
	{
		if(m_ipTable) {
			m_ipTable->AddRef();
		}
		if(m_ipQueryFilter == 0) {
			m_ipQueryFilter.CreateInstance(CLSID_QueryFilter);
		}
	}

	SearchByWhereClause(const SearchByWhereClause& rClause) :
	m_Function(rClause.m_Function),
	m_ipTable(rClause.m_ipTable),
	m_vbRecycling(rClause.m_vbRecycling),
	m_ipQueryFilter(rClause.m_ipQueryFilter)
	{
		if(m_ipTable) {
			m_ipTable->AddRef();
		}
	}

	~SearchByWhereClause()
	{
		if(m_ipTable) {
			m_ipTable->Release();
		}
	}

	void operator()(LPCTSTR lpszWhereClause)
	{
		operator()(CComBSTR(lpszWhereClause));
	}

	void operator()(BSTR bstrWhereClause)
	{
		// �N�G���t�B���^�ɐV����Where���o�^
		m_ipQueryFilter->put_WhereClause(bstrWhereClause);

		// �T�[�`
		traits<TRow>::CursorInterfacePtr ipCursor;
		m_ipTable->Search(m_ipQueryFilter, m_vbRecycling, &ipCursor);

		// ����
		m_Function(static_cast<traits<TRow>::CursorInterface*>(ipCursor));
	}

protected:
	IQueryFilterPtr m_ipQueryFilter; ///< �T�[�`�ɗp����N�G���t�B���^

private:
	TFunction m_Function; ///< �P���֐��I�u�W�F�N�g
	traits<TRow>::TableInterfacePtr m_ipTable; ///< �Ώۃe�[�u��
	VARIANT_BOOL m_vbRecycling;
};

//////////////////////////////////////////////////////////////////////
// �w���p�֐�
//////////////////////////////////////////////////////////////////////

/**
 * @brief IN���Z�X�g���[���\�z�w���p�֐��B
 *
 * @param cFunction [in] IN���Z����P�������Ƃ���t�@���N�^�B
 * @param lpszFieldName [in] �����t�B�[���h���B
 * @param nMaxCount [in] IN���Z���ő�v�f���B
 */
template <typename TFunction>
inline InClauseStream<TFunction> make_InClauseStream(TFunction cFunction, LPCTSTR lpszFieldName, unsigned int nMaxCount = 1000)
{
	return InClauseStream<TFunction>(cFunction, lpszFieldName, nMaxCount);
}

} // namespace sindy

#endif // __SINDY_VALUESET_WHERECLAUSE_STREAM_H__
