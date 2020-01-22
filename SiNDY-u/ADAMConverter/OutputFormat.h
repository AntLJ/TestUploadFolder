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

#include "Arguments.h"

namespace ENCODE {
	enum ECode {
		UNKNOWN_CODE = 0,
		SHIFT_JIS = 1,
		UTF_8 = 2
	};
}

namespace SHAPETYPE {
	enum ECode {
		UNKNOWN_TYPE = 0,
		NORMAL = 1,
		WKT = 2
	};
}

namespace POLYGONTYPE {
	enum ECode {
		NORMAL = 0,
		CLOCKWISE = 1,
		ANTICLOCKWISE = 2
	};
}

/**
 * @brief ���n�n
 */
namespace GEOREF {
	enum ECode {
		UNKNOWN_CODE	= 0,
		TOKYO97			= 4301,
		JGD2000			= 4612,
		JGD2011			= 6668,
	};
}

// �o�̓f�[�^�t�H�[�}�b�g���
class COutputFormat
{
public:
	COutputFormat() : m_GeoRef(0), m_ShapeType(SHAPETYPE::UNKNOWN_TYPE), m_PolygonType(POLYGONTYPE::NORMAL){}
	~COutputFormat(){}

public:
	/**
	 * @brief ������
	 * @param args [in] �R�}���h���C������
	 * @retval true  ��������
	 * @retval false �������s
	 */
	bool init(const Arguments& args);

private:
	/**
	 * @brief �����R�[�h�ݒ�
	 * @param encode [in] �����R�[�h�p�̃I�v�V�����l
	 * @retval true  ��������
	 * @retval false �������s
	 */
	bool setEncode(const CString& encode);

	/**
	 * @brief �f���~�^�ݒ�
	 * @param delimiter [in] �f���~�^�p�̃I�v�V�����l
	 * @retval true  ��������
	 * @retval false �������s
	 */
	bool setDelimiter(const CString& delimiter);

	/**
	 * @brief ���s�R�[�h�ݒ�
	 * @param newline [in] ���s�R�[�h�p�̃I�v�V�����l
	 * @retval true  ��������
	 * @retval false �������s
	 */
	bool setNewline(const CString& newline);

	/**
	 * @brief ���n�n�ݒ�
	 * @param georef [in] ���n�n�p�̃I�v�V�����l
	 * @retval true  ��������
	 * @retval false �������s
	 */
	bool setGeoReference(const CString& georef);

	/**
	 * @brief �`��^�C�v�ݒ�
	 * @param shapetype [in] �`��^�C�v�p�̃I�v�V�����l
	 * @retval true  ��������
	 * @retval false �������s
	 */
	bool setShapeType(const CString& shaptype);

	/**
	 * @brief �|���S���̌����ݒ�
	 * @param polygontype [in] �|���S�������p�̃I�v�V�����l
	 * @retval true  ��������
	 * @retval false �������s
	 */
	bool setPolygonType(const CString& polygontype);

public:
	ENCODE::ECode m_Encode;			//!< �����R�[�h
	CString m_Delimiter;			//!< �f���~�^
	CString m_Newline;				//!< ���s�R�[�h
	long m_GeoRef;					//!< ���n�n
	CString m_GeoRefName;			//!< ���n�n��
	SHAPETYPE::ECode m_ShapeType;	//!< �`��^�C�v
	POLYGONTYPE::ECode m_PolygonType; //!< �|���S���̌���
	bool m_Header;					//!< �w�b�_�o�͗L��
};
