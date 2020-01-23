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
 * @file RelAnnotation.h
 * @brief <b>CRelAnnotation�N���X��`�w�b�_�t�@�C��</b>
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * @vertion $Id$
 */

#ifndef _RELANNOTATION_H_
#define _RELANNOTATION_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "exportimport.h"
#include "Annotation.h"
#include "DispLine.h"

namespace sindy {

class CErrorObjects;

namespace annotation_class {
class CAnnotationClass;
} // namespace annotation_class

extern double g_dTotalSetDispLineTime;	//!< SetDispLine�ɂ����鑍����

class DispIterator
{
public:
	DispIterator( sindyTableType::ECode __T, bool __r = false )
	{
		__reverce = __r;
		switch( __T )
		{
			case sindyTableType::city_annotation:	__disp = sindyTableType::city_disp_line; break;
			case sindyTableType::d1_annotation:		__disp = sindyTableType::c_sc1disp_line; break;
			case sindyTableType::base_annotation:	__disp = ( __reverce ) ? sindyTableType::b_sc1disp_line : sindyTableType::b_sc4disp_line; break;
			case sindyTableType::middle_annotation:	__disp = ( __reverce ) ? sindyTableType::m_sc1disp_line : sindyTableType::m_sc4disp_line; break;
			case sindyTableType::top_annotation:	__disp = ( __reverce ) ? sindyTableType::t_sc1disp_line : sindyTableType::t_sc4disp_line; break;
			default: __disp = sindyTableType::unknown; break;
		}
	}
	DispIterator( sindyAnnotationGroup __T, bool __r = false )
	{
		__reverce = __r;
		switch( __T )
		{
			case sindyAnnoGroup_city:	__disp = sindyTableType::city_disp_line; break;
			case sindyAnnoGroup_csc1:	__disp = sindyTableType::c_sc1disp_line; break;
			case sindyAnnoGroup_base:	__disp = ( __reverce ) ? sindyTableType::b_sc1disp_line : sindyTableType::b_sc4disp_line; break;
			case sindyAnnoGroup_middle:	__disp = ( __reverce ) ? sindyTableType::m_sc1disp_line : sindyTableType::m_sc4disp_line; break;
			case sindyAnnoGroup_top:	__disp = ( __reverce ) ? sindyTableType::t_sc1disp_line : sindyTableType::t_sc4disp_line; break;
			default: __disp = sindyTableType::unknown; break;
		}
	}
	const DispIterator& operator++() {
		switch( __disp )
		{
			case sindyTableType::b_sc4disp_line: __disp = ( __reverce ) ? sindyTableType::unknown        : sindyTableType::b_sc3disp_line; break;
			case sindyTableType::b_sc3disp_line: __disp = ( __reverce ) ? sindyTableType::b_sc4disp_line : sindyTableType::b_sc2disp_line; break;
			case sindyTableType::b_sc2disp_line: __disp = ( __reverce ) ? sindyTableType::b_sc3disp_line : sindyTableType::b_sc1disp_line; break;
			case sindyTableType::b_sc1disp_line: __disp = ( __reverce ) ? sindyTableType::b_sc2disp_line : sindyTableType::unknown; break;
			case sindyTableType::m_sc4disp_line: __disp = ( __reverce ) ? sindyTableType::unknown        : sindyTableType::m_sc3disp_line; break;
			case sindyTableType::m_sc3disp_line: __disp = ( __reverce ) ? sindyTableType::m_sc4disp_line : sindyTableType::m_sc2disp_line; break;
			case sindyTableType::m_sc2disp_line: __disp = ( __reverce ) ? sindyTableType::m_sc3disp_line : sindyTableType::m_sc1disp_line; break;
			case sindyTableType::m_sc1disp_line: __disp = ( __reverce ) ? sindyTableType::m_sc2disp_line : sindyTableType::unknown; break;
			case sindyTableType::t_sc4disp_line: __disp = ( __reverce ) ? sindyTableType::unknown        : sindyTableType::t_sc3disp_line; break;
			case sindyTableType::t_sc3disp_line: __disp = ( __reverce ) ? sindyTableType::t_sc4disp_line : sindyTableType::t_sc2disp_line; break;
			case sindyTableType::t_sc2disp_line: __disp = ( __reverce ) ? sindyTableType::t_sc3disp_line : sindyTableType::t_sc1disp_line; break;
			case sindyTableType::t_sc1disp_line: __disp = ( __reverce ) ? sindyTableType::t_sc2disp_line : sindyTableType::unknown; break;
			default: __disp = sindyTableType::unknown; break;
		}
		return *this;
	}
	const DispIterator& operator--() {
		__reverce = ! __reverce;
		operator++();
		__reverce = ! __reverce;
	}
	const sindyTableType::ECode& operator*() const { return __disp; }
	bool operator !=( const DispIterator& __sc ) const { return ( ! ( **this == *__sc)  ); }
	bool operator ==( const DispIterator& __sc ) const { return ( **this == *__sc ); }
private:
	sindyTableType::ECode __disp;
	bool __reverce;
};

/**
 * @class CRelAnnotation
 * @brief <b>���L�Ǘ��N���X</b>\n
 * ���̃N���X�͒��L�^�ʒu���x�[�X�Ƃ����p���N���X�ŁA
 * �֘A����\���ʒu�t�B�[�`�����Ǘ����܂��B
 */
class SINDYLIB_API CRelAnnotation : public CAnnotation
{
	ROWCLASS_CONSTRUCTOR( CRelAnnotation, unknown )

	//@{ @name �����o������
	/**
	 * @brief <b>�\���ʒu���Z�b�g����</b>\n
	 * �\���ʒu���Z�b�g���܂��B�Ή�����^�ʒu�����ɃZ�b�g����Ă��Ȃ��Ă��Z�b�g�ł��܂����A
	 * �Ăяo�����ŃZ�b�g���������Ή�����^�ʒu�ł��邱�Ƃ�ۏ؂���K�v������܂��B
	 * @param emTableType			[in]			�e�[�u���^�C�v
	 * @param ipAnnotationFeature	[in]			�\���ʒu�t�B�[�`��
	 * @param bDetatch				[in,optional]	true�ɂ���ƃL���b�V�����쐬���A�t�B�[�`�����f�^�b�`����i�f�t�H���g�Ffalse�j
	 * @param ipFields				[in,optional]	�t�B�[���h�Ή��\�I�u�W�F�N�g(shared_ptr)
	 * @param spNameString			[in,optional]	�e�[�u�������Ǘ��I�u�W�F�N�g(shared_ptr)
	 * @retval FALSE �������Ɏ��s
	 */
	errorcode::sindyErrCode SetDispLine( sindyTableType::ECode emTableType, IFeature* ipDispLineFeature, bool bDetatch = false, const CSPFieldMap& spFields = CSPFieldMap(), const CSPTableNameString& spNameString = CSPTableNameString() );
	//@}
	//@{ @name �`�F�b�N���[�`��
	/**
	 * @brief <b>���L������Ɛ^�ʒu�̈ʒu�֌W���`�F�b�N����</b>\n
	 * �^�ʒu�ɑ΂��ĕ�����̈ʒu���A�㉺���E�E�c�����������̂���������Ă���
	 * �p�^�[���ł��邩�ǂ������`�F�b�N���܂��B
	 * [rule_check_1_1_4_3E] ���L������ƋL���̈ʒu�֌W�̃`�F�b�N\n
	 * [rule_check_1_1_4_4E] ���L������ƋL���̈ʒu�֌W�̃`�F�b�N�i�R�x�j\n
	 * [rule_check_1_1_4_5E] ���L������ƋL���̈ʒu�֌W�̃`�F�b�N�i���j\n
	 * [rule_check_1_1_4_9E] ���L������ƋL���̈ʒu�֌W�̃`�F�b�N\n
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckPosition() const;

	/**
	 * @brief <b>�^�ʒu�L���ƕ�����̊Ԋu���`�F�b�N����</b>\n
	 */
	errorcode::sindyErrCode CheckAnnotationGap() const;

	/**
	 * @brief <b>�\���^�C�v���`�F�b�N����</b>\n
	 * ���k�A�~�h���A�g�b�v��S1����S4���ׂĂ��`�F�b�N���܂��B
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckDispType() const;
	//@}

	/**
	 * @biref <b>�L���b�V�����쐬����</b>\n
	 * @param bDetatch			[in,optional]	true�ɂ���ƃL���b�V���쐬��ɐ؂藣���i�f�t�H���g�Ffalse�j
	 * @param ipFields			[in,optional]	�t�B�[���h�Ή��\�I�u�W�F�N�g(shared_ptr)
	 * @param spNameString		[in,optional]	�e�[�u�������Ǘ��I�u�W�F�N�g(shared_ptr)
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CreateCache( bool bDetach = false, const CSPFieldMap& spFields = CSPFieldMap(), const CSPTableNameString& spNameString = CSPTableNameString() );

	/**
	 * @brief <b>�\���ʒu�t�B�[�`���̐����擾����</b>\n
	 * �e�[�u���^�C�v�ɂ͒��L�\���ʒu�t�B�[�`���̃^�C�v��n���Ă��������B
	 * @param emTableType	[in]	�e�[�u���^�C�v
	 * @retval -1 �G���[
	 * @retval 0�ȏ� �\���ʒu�t�B�[�`���̐�
	 */
	long GetDispLineCount( sindyTableType::ECode emTableType ) const;

	/**
	 * @brief <b>�\���ʒu�t�B�[�`�����擾����</b>\n
	 * @param lIndex		[in]	�C���f�b�N�X�ԍ�
	 * @param emTableType	[in]	�\���ʒu�t�B�[�`���e�[�u���^�C�v
	 * return �\���ʒu�t�B�[�`��
	 */
	const CDispLine* GetDispLine( long lIndex, sindyTableType::ECode emTableType ) const;

	/**
	 * @brief <b>�\���ʒu�t�B�[�`�����擾����</b>\n
	 * @param lIndex		[in]	�C���f�b�N�X�ԍ�
	 * @param emTableType	[in]	�\���ʒu�t�B�[�`���e�[�u���^�C�v
	 * @return �\���ʒu�t�B�[�`��
	 */
	CDispLine* GetDispLine( long lIndex, sindyTableType::ECode emTableType );

	/**
	 * @brief <b>���L������Ɛ^�ʒu�̈ʒu�֌W���擾����</b>\n
	 */
//	sindyAnnotationPosition CalcPosition() const;

	/**
	 * @brief <b>�X�P�[���ɂ��������L��������擾����</b>\n
	 * @param emDisp	[in]	�\���ʒu���C���^�C�v
	 * @param nLen		[in]	lpszName�̎��[�\������
	 * @param lpszName	[out]	�̈���m�ۂ���Ă���o�b�t�@
	 * @return �G���[���͋󕶎���̏ꍇ��NULL
	 */
	LPCTSTR GetNameString( long lIndex, sindyTableType::ECode emTableType, int nLen, LPTSTR lpszName ) const;
	/**
	 * @brief <b>�X�P�[���ɂ��������L���������擾����</b>\n
	 * @param emDisp	[in]	�\���ʒu���C���^�C�v
	 * @retval -1 �G���[
	 * @retval -1�ȊO ���L������
	 */
	long GetStringNum( long lIndex, sindyTableType::ECode emTableType ) const;

	/**
	* @brief <b>�R�s�[���L�����݂��邩�ǂ���</b>\n
	* �R�s�[���L�����݂��邩�ǂ����m�F���܂��B
	* @warning �R�x�̏ꍇ�W���l�̓R�s�[�Ƃ��Ĉ����܂��B
	* @param emDispTableType [in] �m�F�Ώۃe�[�u���^�C�v
	* @retval true �R�s�[���L�����݂���
	* @retval false �R�s�[���L�����݂��Ȃ�
	*/
	bool HasCopyDispLine( sindyTableType::ECode emDispTableType ) const;

	//@{ @name �ҏW�n
	/**
	 * @brief <b>�w��I�u�W�F�N�g���폜����</b>\n
	 * �w�肳�ꂽ�^�ʒu�E�\���ʒu���폜���܂��B
	 * �w�肳�ꂽ�I�u�W�F�N�g���^�ʒu�̏ꍇ�A�֘A����\���ʒu��
	 * ���ׂč폜�ΏۂƂȂ�܂��B
	 * �w�肳�ꂽ�I�u�W�F�N�g���\���ʒu�̏ꍇ�A�󋵂ɉ����Đ^�ʒu
	 * �̑������ω�����ꍇ������܂��ie.x. �L���{������ -> �L���̂݁j�B
	 * �폜����ۂɊ֘A�����Ɛ��������Ȃ��ꍇ�͑S�폜�ƂȂ�܂��B
	 * @param ipRow [in] �폜�Ώہi�\���ʒu or �^�ʒu�j
	 * @note �폜�t���O�𗧂Ă邾���Ȃ̂ŁA���ۂɍ폜����ɂ�Store��
	 * �ĂԕK�v������܂��B
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Delete( _IRow* ipRow, CErrorObjects& cErrs );
	//@}

	//@{ @name ���L�X�P�[���C�e���[�^
	typedef DispIterator scale_iterator;
	scale_iterator scale_begin() const { scale_iterator sc = DispIterator(GetTableType()); return sc; }
	scale_iterator scale_end() const { scale_iterator sc = DispIterator(sindyTableType::unknown); return sc; }
	//@}
	/**
	 * @brief <b>���L�^�ʒu�ɍ������\���ʒu���C������Ԃ�</b>\n
	 * �s�s�n�}�̏ꍇ��1�A����ȊO��4�ɂȂ�܂��B
	 * @retval 1 �s�s�n�}�̏ꍇ
	 * @retval 4 ����ȊO�̏ꍇ
	 * @retval -1 �e�[�u���^�C�v������
	 */
	int GetDispLayerCount() const;

	/**
	 * @brief <b>�\���ʒu���C���̃e�[�u���^�C�v��Ԃ�</b>\n
	 * �\���ʒu���C�����������邩��GetDispLayerCount�Œ��ׂ邱�Ƃ��ł��܂��B
	 * @retval S4or�s�s�n�}�\���ʒu���C�� �C���f�b�N�X�ԍ�0
	 * @retval S3 �C���f�b�N�X�ԍ�1
	 * @retval S2 �C���f�b�N�X�ԍ�2
	 * @retval S1 �C���f�b�N�X�ԍ�3
	 * @retval -1 �C���f�b�N�X�ԍ����͈͊O�A���̓e�[�u���^�C�v������
	 */
	sindyTableType::ECode GetDispLayer( int index ) const;
};

} // sindy

#endif // _RELANNOTATION_H_
