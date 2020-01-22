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

namespace meshtype
{

/**
 * @brief	���b�V�����X�g�Ɋi�[����Ă��郁�b�V���R�[�h�̎�ނ�����
 */
enum MeshType
{
	kUnknown, //!< �s��(�������)
	kBasemesh, //!< �񎟃��b�V��(6�P�^)
	kCitymesh, //!< �s�s�n�}���b�V��(8�P�^)
	kInvalid //!< ���b�V�����X�g�ɕs������������
};

/**
* @brief �������b�V���P�ʂɉ����ē���̕������Ԃ�
* @param type[in] �������b�V���P�ʂ̎��ʃR�[�h
*/
inline uh::tstring getStrMeshType( const MeshType& type )
{
	uh::tstring strMeshType;

	switch( type )
	{
	case kBasemesh: strMeshType = _T("BASEMESH"); break;
	case kCitymesh:	strMeshType = _T("Citymesh"); break;
	case kInvalid: strMeshType = _T("�s���ȃ��b�V���P��"); break;
	case kUnknown:
	default:
		assert(false);
		break;
	}

	return strMeshType;
}

} //meshtype
