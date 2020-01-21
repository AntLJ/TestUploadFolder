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
 * @file prog_modify_stamper.h
 * @brief �v���O�����ŏI�X�V��������N���X��`�w�b�_
 *
 * sindy::prog_modify_stamper���`����w�b�_�B
 */
#ifndef SINDY_PROG_MODIFY_STAMPER_H_
#define SINDY_PROG_MODIFY_STAMPER_H_

#include <boost/shared_ptr.hpp>
#include <arctl/field/index.h>
#include <arclib/sindy/stamper/modify_stamper.h>
#include <arclib/sindy/stamper/modify_date_stamper.h>
#include <sindy/schema/common.h>

namespace sindy {

//////////////////////////////////////////////////////////////////////
// prog_modify_stamper ��`
//////////////////////////////////////////////////////////////////////

/**
 * @brief �v���O�����ɂ��X�V���ׂ��ꂽ���Ƃ��������������󂷂�N���X
 *
 * @par ���󂳂�鑮��:
 * - �ŏI�X�V�v���O������
 * - �ŏI�v���O�����X�V����
 * - �X�V�X�V���e
 *
 * @test ProgModifyStamper - @ref unit_test 
 */
class prog_modify_stamper : public modify_stamper
{
public:
// ��`
	ARCTL_FIELD_INDEX_CLASS(index_type, (schema::ipc_table::kModifyProgName)(schema::ipc_table::kProgModifyDate)(schema::ipc_table::kUpdateType));

///@name �R���X�g���N�^�ƃf�X�g���N�^
//@{
	prog_modify_stamper();
	prog_modify_stamper(IFields* ipFields);
//@}

protected:
///@name �������z�֐��̃I�[�o�[���C�h
//@{
	virtual bool stamp(IRowBuffer* ipRowBuffer, schema::ipc_table::update_type::ECode eUpdateType) const;
//@}

private:
// �ϐ�
	boost::shared_ptr<index_type> m_pIndex; ///< �����t�B�[���h�C���f�b�N�X�L���b�V���B
};

//////////////////////////////////////////////////////////////////////
// prog_modify_stamper ����
//////////////////////////////////////////////////////////////////////

/**
 * @brief �l�X�ȃe�[�u���ɗ��p�\�ȃI�u�W�F�N�g���\�z����B
 */
inline prog_modify_stamper::prog_modify_stamper()
{
}

/**
 * @brief �P�̃e�[�u���ɍœK�����ꂽ�I�u�W�F�N�g���\�z����B
 *
 * �����t�B�[���h�C���f�b�N�X���L���b�V�����邱�Ƃ� IRowBuffer::FindField() �̃R�X�g���Ȃ��B<br>
 * ���̃e�[�u���ɏ�������I�u�W�F�N�g��n���Ă͂Ȃ�Ȃ��B
 */
inline prog_modify_stamper::prog_modify_stamper(IFields* ipFields) :
m_pIndex(new index_type(ipFields))
{
}

inline bool prog_modify_stamper::stamp(IRowBuffer* ipRowBuffer, schema::ipc_table::update_type::ECode eUpdateType) const
{
	using namespace arctl::field;
	using namespace schema::ipc_table;

	TCHAR szBuffer[_MAX_PATH];

	// �����t�B�[���h�C���f�b�N�X�����߂�B
	boost::shared_ptr<index_type>
		pIndex(m_pIndex ? m_pIndex : boost::shared_ptr<index_type>(new index_type(ipRowBuffer)));

	// �ŏI�X�V�v���O��������ݒ肷��B
	::GetModuleFileName(NULL, szBuffer, _MAX_PATH);
	LPCTSTR p = ::_tcsrchr(szBuffer, _T('\\'));
	atl2::valid(ipRowBuffer->put_Value(index_t<kModifyProgName>(*pIndex), _variant_t(p ? p + 1 : szBuffer)));

	// �ŏI�v���O�����X�V������ݒ肷��B
	if(! prog_modify_date_stamper(*pIndex)(ipRowBuffer))
		return false;

	// �ŏI�X�V���e��ݒ肷��B
	atl2::valid(ipRowBuffer->put_Value(index_t<kUpdateType>(*pIndex), _variant_t(static_cast<long>(eUpdateType))));

	return true;
}

} // namespace sindy

#endif // SINDY_PROG_MODIFY_STAMPER_H_
