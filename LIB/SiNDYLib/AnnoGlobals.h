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
 * @file AnnoGlobals.h
 * @brief ���L�n�O���֐�
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * 
 * �����ɋL�q����Ă���֐���Export�ΏۊO�ł�
 *
 * @version $Id$
 */
#ifndef _ANNOGLOBALS_H_
#define _ANNOGLOBALS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AnnotationCommunity.h"
#include "RuleCommunity.h"
#include "ErrorCode.h"
#include "TableType.h"

namespace sindy {
/**
 * @brief <b>���L��ʂ��璍�L��ʕ\�������擾����</b>
 * ���L��ʕ\������annotation_code_label�l�[���X�y�[�X����
 * ��`����Ă��܂��B
 * @param emCode	[in]	���L��ʃR�[�h
 * @return ���L��ʕ\����
 */
LPCTSTR SINDYLIB_API AheAnnoCode2CodeName( schema::annotation::annotation_code::ECode emCode );

/**
 * @brief <b>�e�[�u���^�C�v����X�P�[���R�[�h�ɕϊ�����</b>
 * @param emCode [in] �e�[�u���^�C�v�i���L�n�̂݁j
 * @retval sindyAnnotationScale �X�P�[���R�[�h
 * @retval -1 �e�[�u���^�C�v���s��
 */
sindyAnnotationScale SINDYLIB_API AheTableType2AnnotationScale( sindyTableType::ECode emCode );

/**
 * @brief <b>�e�[�u���^�C�v���璍�L�O���[�v�ɕϊ�����</b>
 * @param emCode [in] �e�[�u���^�C�v�i���L�n�̂݁j
 * @retval sindyAnnotationGroup ���L�O���[�v
 * @retval -1 �e�[�u���^�C�v���s��
 */
sindyAnnotationGroup SINDYLIB_API AheTableType2AnnotationGroup( sindyTableType::ECode emCode );

/**
 * @brief <b>�\���ʒu�e�[�u���^�C�v����^�ʒu�e�[�u���^�C�v���擾����</b>
 * @param emDispTypeTableType [in] �\���ʒu���͐^�ʒu�e�[�u���^�C�v
 * @return �^�ʒu�e�[�u���^�C�v
 */
sindyTableType::ECode SINDYLIB_API AheTableType2AnnoPointTableType( sindyTableType::ECode emTableType );

/**
 * @brief <b>�\���ʒu�e�[�u���^�C�v����X�P�[���^�C�v������ɕϊ�����</b>\n
 * @note ���̊֐���CString��Ԃ����߁ADLL�ł͔���J�ł�
 * @param emDispTableType		[in]			�\���ʒu�e�[�u���^�C�v
 * @return �X�P�[���^�C�v������i"S1"�Ƃ�"S2"�Ƃ��B�s�s�n�}�̏ꍇ�ɂ͋󕶎��ɂȂ�܂��j
 */
CString AheTableType2ScaleTypeString( sindyTableType::ECode emDispTableType );

/**
 * @brief <b>�O���[�v���l�����ă��b�V���R�[�h����`�ɕϊ�����</b>\n
 * @warning �O���[�v�ƃ��b�V���R�[�h�̌����̐��������Ȃ��ꍇ�A
 * ���b�V���R�[�h�̌�����D�悵�܂��i�Ƃ������A�O���[�v�w��̓~�h�������Ή����Ă��܂���j
 * @param lMeshCode [in] ���b�V���R�[�h
 * @param emGroup   [in] ���L�O���[�v
 * @param ipSpRef   [in] ��ԎQ��
 * @return ���b�V����`
 */
IEnvelopePtr SINDYLIB_API AheMeshCode2Envelope( long lMeshCode, sindyAnnotationGroup emGroup, ISpatialReference* ipSpRef );

/**
 * @brief <b>�n���ꂽ�e�[�u���^�C�v���^�ʒu�e�[�u���^�C�v���ǂ�������������</b>
 * @param emDispTypeTableType [in] �e�[�u���^�C�v
 * @retval true �^�ʒu�e�[�u���^�C�v�ł���
 * @retval false �^�ʒu�e�[�u���^�C�v�ł͂Ȃ�
 */
bool SINDYLIB_API AheIsAnnoPointTableType( sindyTableType::ECode emTableType );

/**
 * @brief <b>�n���ꂽ�e�[�u���^�C�v���\���ʒu�e�[�u���^�C�v���ǂ�������������</b>
 * @param emDispTypeTableType [in] �e�[�u���^�C�v
 * @retval true �\���ʒu�e�[�u���^�C�v�ł���
 * @retval false �\���ʒu�e�[�u���^�C�v�ł͂Ȃ�
 */
bool SINDYLIB_API AheIsAnnoLineTableType( sindyTableType::ECode emTableType );

/**
 * @brief <b>���L��������`�F�b�N�E�C������</b>\n
 *
 * bFix��TRUE�̏ꍇ�A����ɏC�������ƕԒl�͏��sindyErr_NoErr�ł��B\n
 *
 * ���̊֐��ł́A�ȉ��̃`�F�b�N���s���܂�\n
 *
 * [rule_check_1_2_1_1E]  ���L������̐������]���i�������A����3���ǂ����̃`�F�b�N�͂��Ȃ��j\n
 * [rule_check_1_2_1_2E]  �s�v\n
 * [rule_check_1_2_1_3E]  �s�v\n
 * [rule_check_1_2_1_4E]  �H�i�R�x�W���l�̐����L��������3�ȉ��H��Ȃ킯�Ȃ��j\n
 * [rule_check_1_2_1_5E]  �C���^�[�`�F���W�̏ꍇ�͕�����Ōオ�u�h�b�v�A�������񐔂�4�ȏ�\n
 * [rule_check_1_2_1_6E]  �s�s�n�}���L�ȊO�̏ꍇ�A�W�����N�V�����͕�����Ōオ�u�i�b�v�A�������񐔂�4�ȏ�\n
 * [rule_check_1_2_1_7E]  �t�F���[�^�[�~�i���̏ꍇ�͕�����Ōオ�u�e�s�v�A�������񐔂�4�ȏ�\n
 * [rule_check_1_2_1_8E]  ��L3�Əd������̂ŕs�v\n
 * [rule_check_1_2_1_9E]  �S�p0�͐擪�ɗ��Ă͂����Ȃ�\n
 * [rule_check_1_2_1_10E] �s�v\n
 * [rule_check_1_2_1_11E] �s�s�n�}���L�̏ꍇ�A�W�����N�V�����͕�����Ōオ�u�i�b�s�v�łȂ���΂Ȃ�Ȃ�\n
 * [rule_check_1_2_1_12E] ��v�n�����ԍ��̏ꍇ�͊���1�`151�܂Łi�\���^�C�v�̃`�F�b�N�͑��ł��j\n
 * [rule_check_1_2_1_13E] �����ԍ��̏ꍇ��1�`58�A101�`108�A112�`507\n
 * [rule_check_1_2_1_14E] �S�p�����L���͑S�p�Ђ炪�ȁE�S�p�J�^�J�i�̌����i�n�C�t���̃`�F�b�N�͂ǂ�������悢���킩��Ȃ��c�j\n
 * [rule_check_1_2_1_15E] �R�x�W���l�̏ꍇ�͏����_���ʂ܂ł͑��݂���K�v������
 *
 * @bug �C������@�\�͂܂���������Ă��܂���
 *
 * @param lpcszString	[in]			���L������
 * @param lStringLen	[in]			���L�������i�t�B�[���h�l�j
 * @param emAnnoCode	[in]			���L���
 * @param bIsCity		[in,optional]	�s�s�n�}���L���ǂ����iDEFAULT:true�j
 * @param bIsString3	[in,optional]	���L������3���ǂ����iDEFAULT:false�j
 * @param bFix			[in,optional]	�C������ꍇ��TRUE�iDEFAULT:false�j
 * @param pstrFixString	[out,optional]	�C�����ꂽ�����񂪓���iDEFAULT:NULL�j
 *
 * @return sindyErrCode
 */
errorcode::sindyErrCode SINDYLIB_API AheCheckOrFixAnnotationString( LPCTSTR lpcszString, long lStringLen, schema::annotation::annotation_code::ECode emAnnoCode, bool bIsCity = true, bool bIsString3 = false, bool bFix = false, CString* pstrFixString = NULL );

/**
 * @brief <b>�\���ʒu���C���^�C�v����1pt������̋������Z�o����</b>\n
 * @param emTableType	[in]	���L�\���ʒu���C���e�[�u���^�C�v
 * @param bCalcX		[in]	X���W�l���擾����Ȃ�true�AY���W�l���擾����Ȃ�false
 * @return 1pt������̋����i�P�ʁF�ܓx�o�x�j
 */
double SINDYLIB_API AheCalcDispLineType21PTLen( sindyTableType::ECode emTableType, bool bCalcX );

/**
 * @brief <b>�\���ʒu���C���^�C�v����1px������̋������Z�o����</b>\n
 * @param emTableType	[in]	���L�\���ʒu���C���e�[�u���^�C�v
 * @param bCalcX		[in]	X���W�l���擾����Ȃ�true�AY���W�l���擾����Ȃ�false
 * @return 1px������̋����i�P�ʁF�ܓx�o�x�j
 */
double SINDYLIB_API AheCalcDispLineType21PXLen( sindyTableType::ECode emTableType, bool bCalcX );

/**
 * @brief <b>�X�̕������`����S�̂̃o�E���f�B���O�{�b�N�X���v�Z����</b>\n
 * pStringsBounds�Ɋ܂܂��X�̋�`�͂��ꂼ��5�_�Ō`�������K�v������܂��B
 * �Ⴆ�΁AlStringCount��5�̏ꍇ��5x5��25�_�Ō`������܂��B
 * �܂��A���ꂼ��͕����|���S���łȂ���΂Ȃ�܂���B
 * @param pStringsBounds		[in]	�X�̕������`
 * @param lStringCount			[in]	������
 * @param emLineType			[in]	���C���^�C�v
 * @param lSecondLineStartIndex	[in]	��s���L�̊J�n�C���f�b�N�X�ԍ�
 * @param ipSpRef				[in]	��ԎQ��
 * @return IGeometryPtr
 */
IGeometryPtr SINDYLIB_API AheCalcStringBounds( const WKSPoint* pStringsBounds, long lStringCount, sindyAnnotationLineType emLineType, long lSecondLineStartIndex, ISpatialReference* ipSpRef );

/**
 * @brief <b>�\���^�C�v�ƃX�P�[���\�����[���̘_�������`�F�b�N����</b>\n
 * �\���^�C�v�ƕ\�����[�����A�_���I�ɐ������g�ݍ��킹���ǂ������`�F�b�N���܂��B
 * @param emDispType			[in]	�\���^�C�v
 * @param emScaleDispRule		[in]	�X�P�[���\�����[��
 * @retval true �_���I�ɐ������g�ݍ��킹
 * @retval false �_���I�ɊԈ�����g�ݍ��킹
 */
bool SINDYLIB_API AheCheckDispTypeCodeAndScaleDispRule( schema::annotation::disp_type::ECode emDispType, schema::annotation_class::scaledisp_rule::ECode emScaleDispRule );

/**
 * @brief <b>�\���^�C�v�Ə�ʂ̕\���^�C�v�̘_�������`�F�b�N����</b>\n
 * ���݂̕\���^�C�v�Ƃ���̏�ʂɓ�����\���^�C�v�̑g�ݍ��킹���`�F�b�N���܂��B
 * ���̃`�F�b�N�͓s�s�n�}���C���ł͕s�v�ł��B
 * @see AheCheckDispTypeNakanuke
 * @param emDispType			[in]	�`�F�b�N�Ώە\���^�C�v
 * @param emUpperDispType		[in]	��ʃX�P�[���̕\���^�C�v�iemDispType==S3�Ȃ�emUpperDispType==S4�ƂȂ�j
 * @retval true �_���I�ɐ������g�ݍ��킹
 * @retval false �_���I�ɊԈ�����g�ݍ��킹
 */
bool SINDYLIB_API AheCheckDispTypeAndUpperDispType( schema::annotation::disp_type::ECode emDispType, schema::annotation::disp_type::ECode emUpperDispType );

/**
 * @brief <b>�e�X�P�[���̕\���^�C�v�̘_�������`�F�b�N����</b>\n
 * ���Ɍ����u�������v�̃`�F�b�N�ł��B
 * ���̃`�F�b�N�́A�������̎�ʁi@see CAnnotationClass::GetExistScale�j�y�ѓs�s�n�}���C���ł͕s�v�ł��B
 * @note pRetScale�͕K��4�ȏ��bool�z��Ŏw�肵�Ă��������B
 * @see AheCheckDispTypeCodeAndDispRule
 * @param emDispTypeS1			[in]			S1�\���^�C�v
 * @param emDispTypeS2			[in]			S2�\���^�C�v
 * @param emDispTypeS3			[in]			S3�\���^�C�v
 * @param emDispTypeS4			[in]			S4�\���^�C�v
 * @param pRetScale				[out,optional]	S1�`S4�̂ǂꂪ�G���[���������t���O�B�Ⴆ�΁AS1���G���[�Ȃ�pRetScale[0]==false�ƂȂ�i�f�t�H���g�FNULL�j
 * @retval true ���������Ă��Ȃ�
 * @retval false ���������Ă���
 */
bool SINDYLIB_API AheCheckDispType( schema::annotation::disp_type::ECode emDispTypeS1, schema::annotation::disp_type::ECode emDispTypeS2,
								   schema::annotation::disp_type::ECode emDispTypeS3, schema::annotation::disp_type::ECode  emDispTypeS4, bool* pRetScale = NULL );

#ifdef SINDYLIB_CALC_TIME
extern SINDYLIB_API double g_dAheCalcStringBounds_OneLineTime;		//!< ��s���L�̃o�E���f�B���O�{�b�N�X�̍쐬�ɂ����鑍����
extern SINDYLIB_API double g_dAheCalcStringBounds_OneLine1Time;		//!< ��s���L�̃o�E���f�B���O�{�b�N�X�̍쐬�ɂ����鑍���ԁi�O���j
extern SINDYLIB_API double g_dAheCalcStringBounds_OneLine2Time;		//!< ��s���L�̃o�E���f�B���O�{�b�N�X�̍쐬�ɂ����鑍���ԁi�㔼�j
extern SINDYLIB_API double g_dAheCalcStringBounds_DoubleLineTime;	//!< ��s���L�̃o�E���f�B���O�{�b�N�X�̍쐬�ɂ����鑍����
extern SINDYLIB_API double g_dAheCalcStringBounds_LinerLineTime;	//!< ���󒍋L�̃o�E���f�B���O�{�b�N�X�̍쐬�ɂ����鑍����

void SINDYLIB_API AheCalcStringBoundsTraceTime();
void SINDYLIB_API AheCalcStringBoundsClearTime();
#endif // ifdef SINDYLIB_CALC_TIME

} // namespace sindy

#endif // _ANNOGLOBALS_H_
