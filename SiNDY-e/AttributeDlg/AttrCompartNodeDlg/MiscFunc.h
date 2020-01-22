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
#include <list>
#include <vector>

#include <ArcHelperEx/ActiveView.h>
#include <ArcHelperEx/AheGlobals.h>

namespace miscfunc
{
	/**
	 * @brief �e�[�u���̃t�B�[���h�ɐݒ肳��Ă���R�[�h�l�h���C����map���擾����
	 * @note �����^�̂ݑΉ��B�ǂ����ł��������ǃR�[�h�l���L�[�Ƃ����ق����d���̉\�����y���ł���̂ŁA�����
	 * @param [in] ipTable �Ώۃe�[�u��
	 * @param [in] fieldName �Ώۃt�B�[���h
	 * @param [out] nameValueMap �R�[�h�l����h���C�������Ăяo��map
	 * @retval true �h���C��map�擾����
	 * @retval false �h���C��map�擾���s
	 */
	bool GetCodedValueDomainMap(ITablePtr ipTable, const CString& fieldName, std::map<long, CString>& valueDomainMap);

	/**
	* @brief �t�B�[���h�^�C�v���擾����
	* @param[in]  fields       �t�B�[���h�Q
	* @param[in]  field_name   �t�B�[���h�^�C�v���擾�������t�B�[���h�̖���
	* @param[out] field_type   �t�B�[���h�^�C�v
	* @retval true  �擾����
	* @retval false �擾���s
	*/
	bool GetFieldType(const IFieldsPtr& fields, const CString& field_name, esriFieldType& field_type);
}