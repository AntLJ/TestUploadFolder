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
#include "OutputFormat.h"
#include "ConvertTable.h"
#include "FieldDef.h"
#include "BaseRow.h"

/**
 * @brief ������ϊ���̃f�[�^�`�F�b�N
 */
class CCheckString : CBaseRow
{
public:
	CCheckString(){}
	~CCheckString(){}

public:
	/**
	 * @brief �o�͕����̃`�F�b�N�֐�
	 * @param ipRow [in] ���R�[�h�f�[�^�i�G���[�o�͗p�j
	 * @param cConvTableInfo [in] �ϊ��Ώۃe�[�u�����i�G���[�o�͗p�j
	 * @param cFieldDef [in] �t�B�[���h�Ή��\�̏��
	 * @param strValue [in] �Ώۃf�[�^
	 * @retval true �`�F�b�N�G���[�Ȃ�
	 * @retval false �`�F�b�N�G���[����
	 */
	bool checkOutputString(const _IRowPtr& ipRow, const CConvTableInfo& cConvTableInfo, const CFieldDefInfo& cFieldDef, const CString& strValue);

private:
	
	/**
	 * @brief NULL�`�F�b�N
	 * @param ipRow [in] ���R�[�h�f�[�^�i�G���[�o�͗p�j
	 * @param cConvTableInfo [in] �ϊ��Ώۃe�[�u�����i�G���[�o�͗p�j
	 * @param cFieldDef [in] �t�B�[���h�Ή��\�̏��
	 * @param canNull [in] NULL����
	 * @param strValue [in] �Ώۃf�[�^
	 * @retval true NULL�l�łȂ�orNULL����NULL�l
	 * @retval false NULL���Ȃ̂�NULL�l
	 */
	bool checkNull(const _IRowPtr& ipRow, const CConvTableInfo& cConvTableInfo, const CFieldDefInfo& cFieldDef, bool canNull, const CString& strValue);

	/**
	 * @brief �񕝃`�F�b�N
	 * @param ipRow [in] ���R�[�h�f�[�^�i�G���[�o�͗p�j
	 * @param cConvTableInfo [in] �ϊ��Ώۃe�[�u�����i�G���[�o�͗p�j
	 * @param cFieldDef [in] �t�B�[���h�Ή��\�̏��
	 * @param strValue [in] �Ώۃf�[�^
	 * @retval true ��OK
	 * @retval false ��NG
	 */
	bool checkLength(const _IRowPtr& ipRow, const CConvTableInfo& cConvTableInfo, const CFieldDefInfo& cFieldDef, const CString& strValue);

	/**
	 * @brief �����`�F�b�N
	 * @param setCheckValue [in] �`�F�b�N����
	 * @param strValue [in] �Ώۃf�[�^
	 * @param bValid [in] true=�L���l�`�F�b�N, false=�G���[�l�`�F�b�N
	 * @retval true �����Ɋ܂܂��
	 * @retval false �����Ɋ܂܂�Ȃ�
	 */
	bool checkValue(const std::set<CString>& setCheckValue, const CString& strValue, bool bValid);

	/**
	 * @brief �͈͏����`�F�b�N
	 * @param cFieldDef [in] �t�B�[���h�Ή��\�̏��
	 * @param setRange [in] �`�F�b�N����
	 * @param strValue [in] �Ώۃf�[�^
	 * @param bValid [in] true=�L���l�`�F�b�N, false=�G���[�l�`�F�b�N
	 * @retval true �����Ɋ܂܂��
	 * @retval false �����Ɋ܂܂�Ȃ�
	 */
	bool checkRangeValue(const CFieldDefInfo& cFieldDef, const std::set<CRange>& setRange, const CString& strValue, bool bValid);

};

