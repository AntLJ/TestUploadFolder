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

#ifndef ARCHELPEREX_AHEBUGEVASION_H__
#define ARCHELPEREX_AHEBUGEVASION_H__

/**
 * @brief IWkb::ExportToWkb�̉���֐�
 * 
 * �����I�ɂ��������Ȃ��Ă���`���ExportToWkb���ĂԂ�
 * �����邽�߁A�\������O���g�p���ĉ�����s���Ă��܂��B
 * IWkb::ExportToWkb�͒��ڎg�p�����ɂ��̊֐���
 * �ʂ��Ďg�p���邱�Ƃ�try...catch�����邱�ƂȂ����S��
 * �ĂԂ��Ƃ��ł��܂��B
 */
HRESULT IWkb_ExportToWkb( IWkb* ipWkb, long* pBytes, BYTE* pBuf );

/**
 * @brief IDataset::get_Workspace�̉���֐�
 *
 * �\�[�X���s���ȃI�u�W�F�N�g�i�ʏ́F�Ԃт�����j�ɑ΂���get_Workspace���ĂԂ�
 * �����邽�߁A�\������O���g�p���ĉ�����Ă��܂��B
 * IDataset::get_Workspace�𒼐ڎg�p�����A���̊֐���
 * �ʂ��Ďg�p���邱�Ƃ�try...catch�����邱�ƂȂ����S��
 * �ĂԂ��Ƃ��ł��܂��B
 */
HRESULT IDataset_getWorkspace( IDataset* ipDataset, IWorkspace** ppWork );

/**
 * @brief Arc10.1�ł�ITopologicalOperator::Intersect�̃o�O���������֐�
 *
 * @note �����E�����̃��C���̍������ƁAITopologicalOperator::Intersect��E_FAIL��
 *       �Ԃ��Ă��܂����߁AS_OK�łȂ����Ɍ`�󂪓������ǂ����𒲂ׂāA�����Ȃ�Base�Ɠ���
 *       �`���Ԃ����Ƃɂ���B�����A�����łȂ��ꍇ�ɂ͂Ƃ肠����NULL��Ԃ��B
 *       �g������NULL���n���h�����O���邱�ƁB�����A�����łȂ��P�[�X���łĂ����ꍇ�ɂ�
 *       ������Intersect�`����쐬���邱�Ƃ����Ȃ���΂Ȃ�Ȃ��B
 *       SiNDY-Q�ł̂ݎg�p
 *
 * @param ipBase  [in] ��r���`��
 * @param ipOther [in] Intersect�����`��
 *
 * @return IGeometryPtr  ipBase��ipOther�̌`��̏d�Ȃ蕔��
 */
IGeometryPtr ITopologicalOperator_Intersect( IGeometryPtr ipBase, IGeometryPtr ipOther );

/**
 * @brief Arc10.1�ł�ITopologicalOperator::Difference�̃o�O���������֐�
 *
 * @note �����E�����̃��C���̍������ƁAITopologicalOperator::Difference��E_FAIL��
 *       �Ԃ��Ă��܂����߁AS_OK�łȂ����Ɍ`�󂪓������ǂ����𒲂ׂāA�����Ȃ��`���
 *       �Ԃ����Ƃɂ���B�����A�����łȂ��ꍇ�ɂ͂Ƃ肠����NULL��Ԃ��B
 *       �g������NULL���n���h�����O���邱�ƁB�����A�����łȂ��P�[�X���łĂ����ꍇ�ɂ�
 *       �����ƍ����`����쐬���邱�Ƃ����Ȃ���΂Ȃ�Ȃ��B
 *       SiNDY-Q�ł̂ݎg�p
 *
 * @param ipBase  [in] ��r���`��
 * @param ipOther [in] ���������`��
 *
 * @return IGeometryPtr  ipBase��ipOther�̌`��̍�
 */
IGeometryPtr ITopologicalOperator_Difference( IGeometryPtr ipBase, IGeometryPtr ipOther );

#endif // ifndef ARCHELPEREX_AHEBUGEVASION_H__
