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

#include <TDC/useful_headers/tstring.h>

class crd_cnv;
extern crd_cnv g_cnv;
class CTimeTrace;
extern CTimeTrace g_trace;

std::list<long> set_intersection( const std::list<long>& in1, const std::list<long>& in2 );
inline std::list<long> set_intersection( const std::list<long>& in1, const std::list<long>& in2 )
{
	std::list<long> out;
	out.assign( max( in1.size(), in2.size() ), LONG_MAX );
	std::set_intersection( in1.begin(), in1.end(), in2.begin(), in2.end(), out.begin() );
	out.remove(LONG_MAX);
	return out;
}

/// �`��R�s�[�擾���[�h
enum ShapeMode {
	enumLatest = 1, //!< �ύX��`��
	enumOriginal = 2, //!< �ύX�O�`��
	enumAll = 3, //!< enumLatest|enumOriginal
	enumFeatureChangedOriginal = 4, //!< IFeatureChanged�C���^�[�t�F�[�X����擾�ł���ύX�O�`��
	enumDiff = 8, //!< �ύX�����iLatest��Original�j
};

/**
 * @brief �n���ꂽ�`��Q��UNION����
 * 
 * �����`�����̌`��ɂ܂Ƃ߂܂��B
 * �ʎ����̌`�󂪑��݂���ꍇ��GeometryBag��
 * ���ĕԂ��܂��B�������̌`��̂ݑ��݂���ꍇ��
 * ���̎����̌`��ŕԂ��܂��iMultiPoint�APolyline�APolygon�j�B
 */
IGeometryPtr AheConstructUnion( _ISetPtr& ipGeometrySet );

// SDE�ɓ���邽�߂̒l�Ƃ������̓������ǂ����̔���p�֐�
bool IsEqualSDEValue( esriFieldType type, const CComVariant& va1, const CComVariant& va2 );

/**
 * @brief target��sep�ŕ������Alist�Ɋi�[����
 * @param target [in] ����Ώە�����
 * @param sep_char [in] �Z�p���[�^������
 * @param values [out] ������̕����񃊃X�g
 */
void get_separated_values( const uh::tstring& target, const uh::tstring& sep_char, std::list<uh::tstring>& values );

/// CRowBase::Store()���̌`���r�̔�����@�i�ʏ��CGeometry::Equals�iIClone::IsEqual�j���g�p����j
extern bool g_bStoreShapeJudge_Native; //!< CRowBase::Store()���̌`���r��IRelationalOperator::Equals���g��
extern bool g_bStoreShapeJudge_Exact;  //!< CRowBase::Store()���̌`���r��CGeometry::EqualsVertex�i�\���_��r�j���g��
extern bool g_bStoreShapeJudge_PointZ_Exact; //!< CRowBase::Store()���̌`���r��Z�l�����|�C���g�̏ꍇ��Z�l�̋��e�l���l�����Ȃ��i�f�t�H���g�Ffalse)
extern bool g_bShapeChanged_UseRelationalOperator; //!< CRowBase::ShapeChanged��IRelationalOperator::Equals���g�p���邩�ǂ����i�f�t�H���g�Ffalse�j