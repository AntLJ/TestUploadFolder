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

// Link.h: Link �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <set>

namespace sindy
{
class Link  
{
public:

// �\�z/����
	Link();
	Link(const IFeaturePtr& ipFeature);
	virtual ~Link();

// �ݒ�
	void setFeature(const IFeaturePtr& ipFeature);

	/**
	 * �t�B�[�`�����瑮�����擾����
	 *
	 * @param ipRow				[in]	�t�B�[�`���i���R�[�h�j
	 * @param lpcszFieldName	[in]	�擾����t�B�[���h��
	 *
	 * @retval �擾�ł����l
	 */
	CComVariant GetAttribute( const _IRowPtr& ipRow, LPCTSTR lpcszFieldName );
	CComVariant GetAttribute( const _IRowPtr& ipRow, long lIndex );
	/*
	 * @brief �t�B�[���h���Ńt�B�[���h�C���f�b�N�X���擾����
	 * @param ipFields [in] IFields
	 * @param lpcszFieldName [in] �t�B�[���h��
	 * �t�B�[���h���ƌ���������̑啶������������v���Ȃ��ꍇ���l��
	 * ���A���[�v���񂵂Ĉ���t�B�[���h�����m�F���܂��B
	 * �]���āAIFields::FindField�͎g�p���܂���B
	 * @retval -1 �t�B�[���h����������Ȃ��A���͈�����NULL
	 * @retval -1�ȊO �t�B�[���h�C���f�b�N�X
	 */
	long FindField( const IFieldsPtr& ipFields, LPCTSTR lpcszFieldName );

// �I�y���[�^
	bool operator < (const Link& rLink) const { return m_nObjectId < rLink.m_nObjectId; }

// �v���p�e�B
	long m_nObjectId; ///< �I�u�W�F�N�gID
	long m_nFromNodeId; ///< From�m�[�hID
	long m_nToNodeId; ///< To�m�[�hID

	long m_nRoadClass; ///< ���H���
	long m_nNaviClass; ///< �o�H���
	long m_nRoadWidth; ///< ���H�����敪
	long m_nLinkClass; ///< �����N���
	long m_nRoadNo; ///< �H���ԍ�
	long m_nRoadCode; ///< �H���R�[�h
	
	IGeometryPtr m_ipGeometry; ///< �����N�`��i�n�C���C�g�Ɏg�p�j
};

}
