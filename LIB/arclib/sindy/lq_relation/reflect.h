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

#ifndef SINDY_LQ_RELATION_SYNCER_H_
#define SINDY_LQ_RELATION_SYNCER_H_

#include <functional>
#include <boost/shared_ptr.hpp>
#include <rns/string.h>
#include <rns/sstream.h>
#include <sqltl/inclause_portal.h>
#include <arctl/field/oid.h>
#include <sindy/lq_relation/edit_watcher.h>
#include <sindy/stamper/modify_stamper.h>

namespace sindy {

//////////////////////////////////////////////////////////////////////
// lq_relation_reflect ��`
//////////////////////////////////////////////////////////////////////

/**
 * @brief lq_relation_edit_watcher �̏�����ɁALqRf�̍X�V���s���֐��N���X�B
 *
 * @attention �X�V���m���ɔ��f������ɂ́A�I�u�W�F�N�g��j������O�Ɏ蓮�� flush() ���Ȃ���΂Ȃ�Ȃ��B
 */
class lq_relation_reflect : public std::unary_function<const lq_relation_edit_watcher::value_type, void>
{
	class impl;
public:
// ��`
	typedef boost::shared_ptr<const modify_stamper> modify_stamper_ref;

// �R���X�g���N�^�ƃf�X�g���N�^
	lq_relation_reflect(
		IFeatureClass* ipRoadLinkClass,
		LPCTSTR lpszLqRfName,
		long nLqRfIndex,
		const modify_stamper_ref& pModStamper,
		ITable* ipLqTable);
	~lq_relation_reflect();

// ����
	void flush() const;

// ���Z
	void operator()(const lq_relation_edit_watcher::value_type& value) const;

private:
// �ϐ�
	boost::shared_ptr<impl> pimpl;
};

//////////////////////////////////////////////////////////////////////
// lq_relation_reflect::impl
//////////////////////////////////////////////////////////////////////

class lq_relation_reflect::impl
{
public:
// �R���X�g���N�^
	impl(
		IFeatureClass* ipRoadLinkClass,
		LPCTSTR lpszLqRfName,
		long nLqRfIndex,
		const modify_stamper_ref& pModStamper,
		ITable* ipLqTable);

// ����
	void flush();

	void flagup(long id);
	void flagdown(long id);
	void flagcheck(long id);

private:
// ��`
	struct data_t
	{
		IFeatureClassPtr m_ipRoadLinkClass; ///< ���H�����N�t�B�[�`���N���X�B
		rns::tstring m_strLqRfName; ///< LqRf�����t�B�[���h���B
		long m_nLqRfIndex; ///< LqRf�����t�B�[���h�C���f�b�N�X�B
		ITablePtr m_ipLqTable; ///< Lq�e�[�u���B
	};

	/// �t���O�̕t���ւ����s���֐��N���X�B
	class flag : public std::unary_function<LPCTSTR, void>
	{
	public:
		flag(const data_t& d, long nValue, const modify_stamper_ref& pModStamper);

		void operator()(LPCTSTR where_clause) const;

	private:
		const data_t& m_Data;
		_variant_t m_vValue;
		const modify_stamper_ref m_pModStamper;
	};

	typedef sqltl::inclause_portal<flag> flag_portal;

// �ϐ�
	data_t m_Data;
	flag_portal m_FlagUp;
	flag_portal m_FlagDown;
};

inline lq_relation_reflect::impl::impl(
	IFeatureClass* ipRoadLinkClass,
	LPCTSTR lpszLqRfName,
	long nLqRfIndex,
	const modify_stamper_ref& pModStamper,
	ITable* ipLqTable) :
m_FlagUp(flag(m_Data, 1, pModStamper), arctl::field::oid_index_t::bname(ipRoadLinkClass), 998),
m_FlagDown(flag(m_Data, 0, pModStamper), arctl::field::oid_index_t::bname(ipRoadLinkClass), 998)
{
	m_Data.m_ipRoadLinkClass = ipRoadLinkClass;
	m_Data.m_strLqRfName = lpszLqRfName;
	m_Data.m_nLqRfIndex = nLqRfIndex;
	m_Data.m_ipLqTable = ipLqTable;
};

/**
 * @brief �֌W�t���O�𗧂Ă�B
 *
 * @param id [in] �����NID�B
 */
inline void lq_relation_reflect::impl::flagup(long id)
{
	m_FlagUp(id);
}

/**
 * @brief �֌W�t���O���~�낷�B
 *
 * @param id [in] �����NID�B
 */
inline void lq_relation_reflect::impl::flagdown(long id)
{
	m_FlagDown(id);
}

/**
 * @brief �֌W�t���O���X�V����B
 *
 * @param id [in] �����NID�B
 */
inline void lq_relation_reflect::impl::flagcheck(long id)
{
	if(m_Data.m_ipLqTable == 0)
		throw std::runtime_error("Lq�e�[�u�������݂��Ȃ����� indeterminate �������ł��Ȃ��B");

	// �����t�B���^�p�ӁB
	IQueryFilterPtr ipQueryFilter(CLSID_QueryFilter);

	// where��ݒ�B
	rns::tostringstream oss;
	std::locale loc(std::locale("japanese"), "C", std::locale::numeric);
	oss.imbue(loc);
	oss << sindy::schema::link_queue::kLinkID << _T("=") << id;
	ipQueryFilter->put_WhereClause(CComBSTR(oss.str().c_str()));

	// �֌W�t���O�X�V�B
	long nCount;
	atl2::valid(m_Data.m_ipLqTable->RowCount(ipQueryFilter, &nCount));
	if(nCount > 0) {
		// Lq���P�ł����݂��Ă�����t���O�𗧂Ă�B
		flagup(id);
	}
	else {
		// Lq���P�����݂��Ă��Ȃ�������t���O���~�낷�B
		flagdown(id);
	}
}

/**
 * @brief �֌W�t���O�̍X�V�̃f�[�^�x�[�X�ւ̔��f�����S�Ȃ��̂Ƃ���B
 */
inline void lq_relation_reflect::impl::flush()
{
	m_FlagUp.flush();
	m_FlagDown.flush();
}

//////////////////////////////////////////////////////////////////////
// lq_relation_reflect::impl::flag
//////////////////////////////////////////////////////////////////////

inline lq_relation_reflect::impl::flag::flag(const data_t& data, long nValue, const modify_stamper_ref& pModStamper) :
m_Data(data),
m_vValue(nValue),
m_pModStamper(pModStamper)
{
}

/**
 * @brief �t���O�̕t���ւ����s���B
 *
 * @param where_clause [in] �t���O�̕t���ւ��ΏۂƂȂ�I�u�W�F�N�g�����߂�where��B
 */
inline void lq_relation_reflect::impl::flag::operator()(LPCTSTR where_clause) const
{
	// �����t�B���^�p�ӁB
	IQueryFilterPtr ipQueryFilter(CLSID_QueryFilter);

	// �t�B�[���h����B
	ipQueryFilter->put_SubFields(CComBSTR(m_Data.m_strLqRfName.c_str()));

	// where��ݒ�B
	rns::tostringstream oss;
	std::locale loc(std::locale("japanese"), "C", std::locale::numeric);
	oss.imbue(loc);
	oss << where_clause << _T(" AND ") << m_Data.m_strLqRfName << _T("<>") << V_I4(&m_vValue);
	ipQueryFilter->put_WhereClause(CComBSTR(oss.str().c_str()));

	// �X�V�J�[�\���擾�B
	IFeatureCursorPtr ipFeatureCursor;
	if(m_Data.m_ipRoadLinkClass->Search(ipQueryFilter, VARIANT_FALSE, &ipFeatureCursor) != S_OK)
		return;

	// �X�V�B
	IFeaturePtr ipFeature;
	while(ipFeatureCursor->NextFeature(&ipFeature) == S_OK) {
		// �l������ƈႤ�ꍇ�̂ݍX�V����B
		_variant_t vOldValue;
		atl2::valid(ipFeature->get_Value(m_Data.m_nLqRfIndex, &vOldValue));
		if(vOldValue != m_vValue) {
			atl2::valid(ipFeature->put_Value(m_Data.m_nLqRfIndex, _variant_t(m_vValue)));

			// �X�V������t�^����B
			if(m_pModStamper) {
				(*m_pModStamper)(ipFeature, sindy::schema::ipc_table::update_type::kAttrUpdated);
			}

			ipFeature->Store();
		}
	}
}

//////////////////////////////////////////////////////////////////////
// lq_relation_reflect ����
//////////////////////////////////////////////////////////////////////

/**
 * @param ipRoadLinkClass [in] ���H�����N�t�B�[�`���N���X�B
 * @param lpszLqRfName [in] LqRf�����t�B�[���h���B
 * @param nLqRfIndex [in] LqRf�����t�B�[���h�C���f�b�N�X�B
 * @param pModStamper [in] �X�V���t�^�I�u�W�F�N�g�B
 * @param ipLqTable [in] Lq�e�[�u���Bindeterminate�̉������ɕK�v�ƂȂ�B
 */
inline lq_relation_reflect::lq_relation_reflect(
	IFeatureClass* ipRoadLinkClass,
	LPCTSTR lpszLqRfName,
	long nLqRfIndex,
	const modify_stamper_ref& pModStamper,
	ITable* ipLqTable) :
pimpl(new impl(ipRoadLinkClass, lpszLqRfName, nLqRfIndex, pModStamper, ipLqTable))
{
}

inline lq_relation_reflect::~lq_relation_reflect()
{
}

/**
 * @brief �֌W�t���O�̍X�V�̃f�[�^�x�[�X�ւ̔��f�����S�Ȃ��̂Ƃ���B
 */
inline void lq_relation_reflect::flush() const
{
	pimpl->flush();
}

/**
 * @brief �֌W�t���O�̍X�V�����݂�B
 *
 * @param val [in] �����NID�ƃt���O�̑΁B
 */
inline void lq_relation_reflect::operator()(const lq_relation_edit_watcher::value_type& val) const
{
	if(val.second) {
		// true
		pimpl->flagup(val.first);
	}
	else if(! val.second) {
		// false
		pimpl->flagdown(val.first);
	}
	else {
		// indeterminate
		pimpl->flagcheck(val.first);
	}
}

} // namespace sindy

#endif // SINDY_LQ_RELATION_SYNCER_H_
