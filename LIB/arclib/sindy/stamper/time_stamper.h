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
 * @file time_stamper.h
 * @brief ��������N���X�̒�`
 */
#ifndef SINDY_TIME_STAMPER_H_
#define SINDY_TIME_STAMPER_H_

#include <functional>
#include <atl2/exception.h>
#include <arctl/field.h>

namespace sindy {

/**
 * @brief ��������N���X
 *
 * �n���ꂽ<b>IRowBuffer</b>�Ɍ��݂̓��������ޒP���֐��N���X�B<br>
 * ���������t�B�[���h�����ÓI�ɒ�܂��Ă���ꍇ��@ref time_stamper_t���g���B
 *
 * @test TimeStamperTest - @ref unit_test 
 */
class time_stamper : public std::unary_function<IRowBuffer*, bool>
{
public:
	/**
	 * @brief ���������t�B�[���h�C���f�b�N�X��ݒ肷��B
	 * @param nIndex [in] �X�V���������t�B�[���h�C���f�b�N�X�B
	 */
	time_stamper(long nIndex) :
	m_nIndex(nIndex)
	{
	}

	/**
	 * @brief ���݂̓��������󂷂�B
	 *
	 * @param ipRowBuffer [in] ����ΏہB
	 * @retval true ����ɐ��������B
	 * @exception _com_ptr_t <b>IRowBuffer::put_Value()</b>���G���[��Ԃ����B
	 */
	bool operator()(IRowBuffer* ipRowBuffer) const
	{
		// VT_DATE�^��VARIANT��p�ӂ���B
		_variant_t va;
		va.vt = VT_DATE;

		// ���݂̓�����VARIANT�Ɋi�[����B
		SYSTEMTIME st;
		::GetLocalTime(&st);
		::SystemTimeToVariantTime(&st, &va.dblVal);

		// IRowBuffer�ɒl���i�[����B
		atl2::valid(ipRowBuffer->put_Value(m_nIndex, va));

		return true;
	}

private:
// �ϐ�
	const long m_nIndex; ///< ���������t�B�[���h�C���f�b�N�X�B
};

/**
 * @brief ���������t�B�[���h����`���E��������N���X
 *
 * @ref time_stamper�����̓��������t�B�[���h���ɓ��ꉻ����e���v���[�g�N���X�B
 *
 * @par �e���v���[�g����:
 * - @a TFieldName : ���������t�B�[���h���B
 *
 * @test TimeStamperTest - @ref unit_test 
 */
template <LPCTSTR TFieldName>
class time_stamper_t : public time_stamper
{
public:
// ��`
	typedef arctl::field::index_t<TFieldName> index_type; ///< �����t�B�[���h�C���f�b�N�X�^�B

//@{ @name �\�z/����
	time_stamper_t(index_type const& rIndex) : time_stamper(rIndex) {}
//@}
};

} // namespace sindy

#endif // SINDY_TIME_STAMPER_H_
