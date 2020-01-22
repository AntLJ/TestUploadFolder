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

#pragma once

namespace gf { // �O���[�o���֐�

	/**
	* @brief �w��t�B�[�`���̎��Ӂ`(m)�͈̔͂Ŏw��t�B�[�`���N���X�̃t�B�[�`������������
	* @warning �������ʂɂ͎w��͈͊O�̂��̂��܂܂��̂Œ���
	* @param ipFeature				[in]			�����͈͂̒��S�ɂȂ�t�B�[�`��
	* @param ipFeatClass			[in]			�����Ώۂ̃t�B�[�`���N���X
	* @param dRange					[in]			�����͈͂̔��a�i�P�ʁF[m]�j
	* @param strWhereClause			[in]			���������i�f�t�H���g�FNULL�j
	* @return IFeatureCursorPtr
	*/
	IFeatureCursorPtr AroundSearch( IFeature* ipFeature, IFeatureClass* ipFeatClass, double dRange, LPCTSTR strWhereClause=NULL );

	/**
	* @brief �t�B�[�`����_�ł�����
	*
	* @note    �|�C���g�t�B�[�`���̂ݑΉ�
	* @warning sleepTime�~blinkCount�t���[���̎��ԁA���͂��󂯕t���Ȃ��Ȃ�
	*
	* @param ipScDisp            [in]   �X�N���[���f�B�X�v���C
	* @param lOID                [in]   �Ώۃt�B�[�`���̃I�u�W�F�N�gID
	* @param ipFeatClass         [in]   �Ώۃt�B�[�`���̃t�B�[�`���N���X
	* @param sleepTime           [in]   �_�Ō�̓��͂��󂯕t���Ȃ����� (Sleep�֐��ɓn�����)
	* @param blinkCount          [in]   �_�ł������
	*/
	void BlinkFeature( IScreenDisplay* ipScDisp, LONG lOID, IFeatureClass* ipFeatClass, DWORD sleepTime=100, unsigned int blinkCount=4 );

	/**
	 * @brief CComVariant��CString�^�̕�����ɕϊ�����
	 * @param variant [in] CString�ɕϊ�����CComVariant
	 * @param strNull [in] variant��������^�łȂ��ꍇ�ɕ\�����镶����
	 * @retval null     CComVariant��vt��VT_NULL������
	 * @retval null�ȊO CComVariant�̕ێ����Ă���������
	 */
	CString VariantToCString( const CComVariant& variant, LPCTSTR strNull );

} // namespace gf
