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
 * @file
 * @note AheGlobals.h/cpp����crd_cnv�̃����N���K�v�ɂȂ�֐��Q�𕪗��������́B
 *       �o�܂͉��L���r���[���Q�Ƃ̎��B
 *       ArcHelperEx�̃��t�@�N�^�����O�icrd_cnv�֌W�j http://orias.mr.ipc.pioneer.co.jp/reviewboard/r/2424/
 *       �A���A�r���h���ʂ������_�ł�߂��̂ŁA����crd_cnv��K�v�Ƃ��Ȃ����̂��܂܂�Ă���\������B
 *       ���̏ꍇ��AheGlobals.h/cpp��AheGlobalsMisc.h/cpp�Ɉړ�����Ƃ��������m��Ȃ��B
 */
#pragma once

//////////////////////////////////////////////////////////////////////
// URI ��͗p�֐��E���W�ϊ��Ȃ�
//////////////////////////////////////////////////////////////////////
/// URI ��͊֐��p�\����
struct LonLat{
	double Longitude;
	double Latitude;
	double Scale;
};

struct MeshXY{
	unsigned int MeshCode;
	int X;
	int Y;
	double Scale;
};

struct AdrsScale{
	CComBSTR Adrs;
	double Scale;
};

/**
* @brief ���b�V���R�[�h���W�C���N�������g
*
* @note ���b�V���R�[�h��XY�����ɃC���N�������g����ۂ̂W�i�C���N�������g
*/
#define INC_MESHXY(x)	( 7 == ( x % 10)) ? ( x + 3) : ( x + 1)
/**
* @brief ���b�V���R�[�h���W�f�N�������g
*
* @note ���b�V���R�[�h��XY�����Ƀf�N�������g����ۂ̂W�i�f�N�������g
*/
#define DEC_MESHXY(x)	( 0 == ( x % 10)) ? ( x - 3) : ( x - 1)

/**
 * @brief ���b�V���R�[�h����A���b�V��XY�R�[�h�֕ϊ�
 *
 * @param nX				[out]			X
 * @param nY				[out]			Y
 * @param nMeshCode			[in]			���b�V���R�[�h
 */
void AheMeshcode2MeshXY( int& nX, int& nY, unsigned int nMeshCode);

/**
 * @brief	���b�V��XY�R�[�h����A���b�V���R�[�h�֕ϊ�
 *
 * @note	�w�肳�ꂽ�|�C���g�ƒ��������`���쐬���܂��B
 *			�t�B�[�`���̑I���Ȃǂɑ����g�p����Ă��܂��B
 *
 * @param nX				[in]  X
 * @param nY				[in]  Y
 *
 * @return ���b�V���R�[�h
 */
unsigned int AheMeshXY2Meshcode( int nX, int nY);
/**
 * @brief �x���b�̈ܓx�A�o�x���� Real Degree �����̈ܓx�A�o�x���擾����
 *
 * @param iDeg			[in]			�x
 * @param iMin			[in]			��
 * @param dSec			[in]			�b
 *
 * @return Real Degree �ł̐��l
 */
DOUBLE AheDegMinSec2Realdegree( INT iDeg, INT iMin, DOUBLE dSec );

/**
 * @brief ���͂��ꂽ�����񂩂�x���b�`���̈ܓx�A�o�x���擾����
 *
 * @param str			[in]			�ΏۂƂȂ镶����
 * @param iDeg			[out]			�擾�����x
 * @param iMin			[out]			�擾������
 * @param dSec			[out]			�擾�����b
 *
 * @retval TRUE �K��������W���擾�ł���
 * @retval FALSE �K��������W���擾�ł��Ȃ�����
 */
BOOL AheStr2DegMinSec( CString& , INT& , INT& , DOUBLE& );

/**
 * URI ( MapFanV �p ) mf://mapfan/map/pos? �Ŏn�܂�A�h���X�� LonLat �\���̂ɕϊ����ĕԂ�
 *
 * @param CString		[in]  URI ������
 * @param LonLat		[out] LonLat �^�̌���
 *
 * @retval TURE �����URI
 * @retval FALSE ��͂ł��Ȃ�URI
 */
BOOL AheURIMFV2LonLat(CString, LonLat&);

BOOL AheURIMFV2MeshXY(CString, MeshXY&);

BOOL AheChkLonLat(CString);

/**
 * URI ( MapFan Web �p ) http://kokomail.mapfan.com/receive.cgi? �Ŏn�܂�A�h���X�� LonLat �\���̂ɕϊ����ĕԂ�
 *
 * @param CString		[in]  URI ������
 * @param LonLat		[out] LonLat �^�̌���
 *
 * @retval TURE �����URI
 * @retval FALSE ��͂ł��Ȃ�URI
 */
BOOL AheURIMFW2LonLat(CString, LonLat&);

BOOL AheURIMFW2MeshXY(CString, MeshXY&);

/**
 * URI ( MapFan Web �p ) http://mapserv.mapfan.com/cgi-bin/map/mapserv.cgi? �Ŏn�܂�A�h���X�� LonLat �\���̂ɕϊ����ĕԂ�
 *
 * @param CString		[in]  URI ������
 * @param LonLat		[out] LonLat �^�̌���
 *
 * @retval TURE �����URI
 * @retval FALSE ��͂ł��Ȃ�URI
 */
BOOL AheURIMFP2LonLat(CString, LonLat&);

/**
 * URI ( SiNDY ���b�V�� XY�p ) sindy://sindy/map/pos? �Ŏn�܂�A�h���X�� LonLat �\���̂ɕϊ����ĕԂ�
 *
 * @param CString		[in]  URI ������
 * @param LonLat		[out] LonLat �^�̌���
 *
 * @retval TURE �����URI
 * @retval FALSE ��͂ł��Ȃ�URI
 */
BOOL AheURISiNDYMesh2LonLat(CString, LonLat&);

/**
 * URI ( SiNDY ���b�V�� XY�p ) sindy://sindy/map/pos? �Ŏn�܂�A�h���X�� MeshXY �\���̂ɕϊ����ĕԂ�
 *
 * @param CString		[in]  URI ������
 * @param LonLat		[out] MeshXY �^�̌���
 *
 * @retval TURE �����URI
 * @retval FALSE ��͂ł��Ȃ�URI
 */
BOOL AheURISiNDYMesh2MeshXY(CString, MeshXY&);

/**
 * URI ( SiNDY �Z���p ) sindy://sindy/map/pos? �Ŏn�܂�A�h���X�� LonLat �\���̂ɕϊ����ĕԂ�
 *
 * @param CString		[in]  URI ������
 * @param LonLat		[out] LonLat �^�̌���
 *
 * @retval TURE �����URI
 * @retval FALSE ��͂ł��Ȃ�URI
 */
BOOL AheURISiNDYAdrs2LonLat(CString, LonLat&);

BOOL AheURISiNDYAdrs2Adrs(CString, AdrsScale&);

/**
 * @brief �n���Q�Ƃ��擾����
 *
 * @param ipUnk	[in]	AheGetFeatureClass �Ńt�B�[�`���N���X���擾�ł���I�u�W�F�N�g
 *
 * return ISpatialReferencePtr
 */
ISpatialReferencePtr AheGetSpatialReference( IUnknownPtr ipUnk );
