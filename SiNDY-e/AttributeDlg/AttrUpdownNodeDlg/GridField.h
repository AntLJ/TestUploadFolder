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
class GridField
{
public:
	/**
	*	@�t�B�[���h���ƃO���b�h��̑Ή�
	*/
	struct FieldGridCol
	{
		CString m_field_name;	//!< �t�B�[���h��
		int m_col;				//!< �O���b�h��
		

		FieldGridCol(const CString& field, int col)
			: m_field_name(field), m_col(col) {}

		// �O���b�h�񏇂ɕ��ׂ�
		bool operator<(const FieldGridCol& rec) const {
			return m_col < rec.m_col;
		}
	};

public:
	typedef std::set<FieldGridCol>::const_iterator CIter;

public:
	GridField() {};
	~GridField() {};

	CIter begin() const { return m_Data.begin(); }
	CIter end() const { return m_Data.end(); }
	size_t size() const { return m_Data.size(); }

	/**
	 *	@brief �t�B�[���h���ƃO���b�h��̑Ή��̒ǉ�
	 *  @param[in] field_name �t�B�[���h��
	 *  @param[in] col        �O���b�h��ԍ�
	 */
	void add(const CString& field_name, int col) { m_Data.emplace(field_name, col); }

	/**
	 *	@brief �w�肵���t�B�[���h���ɑΉ������O���b�h��ԍ���Ԃ�
	 *  @param[in] field_name �t�B�[���h��
	 *  @return ��ԍ�(������Ȃ������ꍇ��-1��Ԃ�
	 */
	long getCol(const CString& field_name) const;

	/**
	 *	@brief �w�肵���O���b�h��ԍ��ɑΉ������t�B�[���h����Ԃ�
	 *  @param[in] col  �O���b�h��ԍ�
	 *  @return  �t�B�[���h��(������Ȃ������ꍇ�͋󕶎���Ԃ�)
	 */
	const CString& getFieldName(int col) const;

private:
	std::set<FieldGridCol> m_Data;	//!< �t�B�[���h���ƃO���b�h�̑Ή����X�g
};

