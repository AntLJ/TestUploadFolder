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


namespace adam_v2
{
/**
 * �t�B�[���h�v�f
 * �X�L�[�}����
*/
class SchemaFiled
{
public:
	/**
	 * �R���X�g���N�^
	 *
	 * @param[in]	cursor	ICursor�|�C���^
	 * @param[in]	schema	�I�u�W�F�N�g�N���X��
	 * @deprecated	ITablePtr�ł��g���Ă�������
	 * 
	 */
	SchemaFiled(const _ICursorPtr& cursor, const uh::tstring& schema) 
	{
		if(cursor)
		{
			cursor->FindField(CComBSTR(schema.c_str()), &index);
		}
		else
		{
			index = 0;
		}
		name = schema;
	}
	/**
	 * �R���X�g���N�^
	 *
	 * @param[in]	ipTable	ITable�|�C���^
	 * @param[in]	schema	�I�u�W�F�N�g�N���X��
	 * 
	 */
	SchemaFiled(const ITablePtr& ipTable, const uh::tstring& schema) 
	{
		if(ipTable)
		{
			ipTable->FindField(CComBSTR(schema.c_str()), &index);
		}
		else
		{
			index = 0;
		}
		name = schema;
	}
	/**
	 * �f�X�g���N�^
	 */
	~SchemaFiled(void){}

public:
	long index;			///< �C���f�b�N�X�ԍ�
	uh::tstring name;	///< �I�u�W�F�N�g�N���X��
};

};