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

#include "TimeTrace.h"

// �O���[�o���ϐ��錾
extern CTimeTrace g_timetrace; //<! ���Ԍv���p

using namespace std;
using namespace sindy;
using namespace sindy::schema;

/**
 * @brief   �`���v�m�F
 * @param   ipGeom1  [in]  ��r�`��1
 * @param   ipGeom2  [in]  ��r�`��2
 * @return  true   �`���v
 *          false  �`��s��v
 */
bool IsSameGeom( const IGeometryPtr& ipGeom1, const IGeometryPtr& ipGeom2 );

CString GetTableName( const ITablePtr& ipTable );

/**
 * @brief AGOPS�e�[�u����
 * @note AGOPS�Ƃ́A�����N������(segment_Attr)�A�X��(Grad)�A����ʍs(Oneway)�A�ʍs�֎~(noPass)�A�ȈՌX��(Simple_Grad)�̈�
 * @note ������������1�����N�ɑΉ�����e�[�u���Q�̂��ƁB
 * @note ���/�ʋ֎Ԏ�ʋK��(Vehicle_reg)���ǉ�����AGOPSV�Ƃ����l�����邪�A�Ӗ��͒ʂ�̂�AGOPS�̂܂܂Ƃ���
 * @param strTableName [in] �e�[�u������
 * @retval true �^����ꂽ�e�[�u������AGOPS�e�[�u���ɊY������
 * @retval false �^����ꂽ�e�[�u������AGOPS�e�[�u���ɊY�����Ȃ�
 */
bool IsAGOPSTable( const CString strTableName );

/**
 * @brief �Ԏ�^�C�v�̃e�[�u����
 * @param strTableName [in] �e�[�u������
 * @retval true �^����ꂽ�e�[�u�����͎Ԏ�^�C�v�̃e�[�u���ɊY������
 * @retval false �^����ꂽ�e�[�u�����͎Ԏ�^�C�v�̃e�[�u���ɊY�����Ȃ�
 */
bool IsVehicleTypeTable( const CString strTableName );

/**
 * @brief �����N��(INF)�̃e�[�u����
 * @param strTableName [in] �e�[�u������
 * @retval true �^����ꂽ�e�[�u�����̓����N����e�[�u��(INF)�ɊY������
 * @retval false �^����ꂽ�e�[�u�����̓����N����e�[�u��(INF)�ɊY�����Ȃ�
 */
bool IsInfTable( const CString strTableName );

/**
 * @brief �����N��(LQ)�̃e�[�u����
 * @param strTableName [in] �e�[�u������
 * @retval true �^����ꂽ�e�[�u�����̓����N��(LQ)�̃e�[�u���ɊY������
 * @retval false �^����ꂽ�e�[�u�����̓����N��(LQ)�̃e�[�u���ɊY�����Ȃ�
 */
bool IsLqTable( const CString strTableName );

CString getNowTime();


// TODO:�ҏW�ۑ����s�������A�G���[�R�[�h�Ń��R���T�C�����邩�̏���
