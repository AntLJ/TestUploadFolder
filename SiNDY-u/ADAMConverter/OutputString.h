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
#include "CheckString.h"
#include "BaseRow.h"

/**
 * @brief DB���R�[�h�f�[�^�𕶎���ɕϊ�����N���X
 */
class COutputString : CBaseRow
{
public:
	COutputString(){}
	COutputString(const crd_cnv& cnv, const COutputFormat& cOutputFormat, bool isZ, bool isM, esriGeometryType geomType){
		m_cOutputFormat = cOutputFormat;
		m_isZ = isZ;
		m_isM = isM;
		m_geomType = geomType;
		m_cnv = cnv;
	}
	~COutputString(){}

public:
	/**
	 * @brief DB���R�[�h�f�[�^�𕶎���ɕϊ�����
	 * @param cConvTableInfo [in] �ϊ��Ώۃe�[�u�����
	 * @param ipRow [in] �ϊ������R�[�h
	 * @param strOutput [out] �ϊ����f�[�^�𕶎���ϊ���������
	 * @retval true ��������
	 * @retval true �������s
	 */
	bool createString(const CConvTableInfo& cConvTableInfo, const _IRowPtr& ipRow, CString& strOutput);
	
	/**
	 * @brief �o�̓t�@�C���̃w�b�_�[�쐬
	 * @param cConvTableInfo [in] �ϊ��Ώۃe�[�u�����
	 * @param isZ [in] �`���Z�l�L��
	 * @param isM [in] �`���M�l�L��
	 * @param strHeader [out] �o�̓f�[�^�̃w�b�_�[
	 * @retval true ��������
	 * @retval true �������s
	 */
	bool createHeader(const CConvTableInfo& cConvTableInfo, bool isZ, bool isM, CString& strHeader);

private:
	/**
	 * @brief �J�����f�[�^�𕶎���ɕϊ�����
	 * @param ipRow [in] �ϊ������R�[�h�i�G���[�o�͗p�j
	 * @param cConvTableInfo [in] �ϊ��Ώۃe�[�u�����i�G���[�o�͗p�j
	 * @param cFieldDefInfo [in] �ϊ��Ώۃe�[�u���̃t�B�[���h�Ή��\
	 * @param vaValue [in] �ϊ����J�����f�[�^
	 * @param strColumnString [out] �ϊ����f�[�^�𕶎���ϊ���������
	 * @retval true ��������
	 * @retval true �������s
	 */
	bool value2String(const _IRowPtr& ipRow, const CConvTableInfo& cConvTableInfo, const CFieldDefInfo& cFieldDefInfo, const CComVariant& vaValue, CString& strColumnString);

	/**
	 * @brief �`��f�[�^�̕�����ϊ�
	 * @param ipRow [in] ���R�[�h�f�[�^�i�G���[�o�͗p�j
	 * @param cConvTableInfo [in] �ϊ��Ώۃe�[�u�����i�G���[�o�͗p�j
	 * @param vaValue [in] �ϊ����`��f�[�^
	 * @param vecShapeString [out] ������ϊ���f�[�^
	 * @retval true ��������
	 * @retval true �������s
	 */
	bool shape2String(const _IRowPtr& ipRow, const CConvTableInfo& cConvTableInfo, const CComVariant& vaValue, std::vector<std::vector<CString>>& vecShapeString);

	/**
	 * @brief �|�C���g�`��f�[�^�̕�����ϊ�
	 * @param ipRow [in] ���R�[�h�f�[�^�i�G���[�o�͗p�j
	 * @param cConvTableInfo [in] �ϊ��Ώۃe�[�u�����i�G���[�o�͗p�j
	 * @param vaValue [in] �`��f�[�^
	 * @param vecShapeString [out] ������ϊ���f�[�^
	 * @retval true ��������
	 * @retval true �������s
	 */
	bool point2String(const _IRowPtr& ipRow, const CConvTableInfo& cConvTableInfo, const CComVariant& vaValue, std::vector<std::vector<CString>>& vecShapeString);

	/**
	 * @brief �|�����C���`��f�[�^�̕�����ϊ�
	 * @param ipRow [in] ���R�[�h�f�[�^�i�G���[�o�͗p�j
	 * @param cConvTableInfo [in] �ϊ��Ώۃe�[�u�����i�G���[�o�͗p�j
	 * @param vaValue [in] �`��f�[�^
	 * @param vecShapeString [out] ������ϊ���f�[�^
	 * @retval true ��������
	 * @retval true �������s
	 */
	bool polyline2String(const _IRowPtr& ipRow, const CConvTableInfo& cConvTableInfo, const CComVariant& vaValue, std::vector<std::vector<CString>>& vecShapeString);

	/**
	 * @brief �|���S���`��f�[�^�̕�����ϊ�
	 * @param ipRow [in] ���R�[�h�f�[�^�i�G���[�o�͗p�j
	 * @param cConvTableInfo [in] �ϊ��Ώۃe�[�u�����i�G���[�o�͗p�j
	 * @param vaValue [in] �`��f�[�^
	 * @param vecShapeString [out] ������ϊ���f�[�^
	 * @retval true ��������
	 * @retval true �������s
	 */
	bool polygon2String(const _IRowPtr& ipRow, const CConvTableInfo& cConvTableInfo, const CComVariant& vaValue, std::vector<std::vector<CString>>& vecShapeString);

	bool ring2vector(const _IRowPtr& ipRow, const CConvTableInfo& cConvTableInfo, const IRingPtr& ipRing, std::vector<std::vector<CString>>& vecRing);

	/**
	 * @brief �|�C���g�f�[�^�̕�����ϊ��i�ܓx�o�x�{ZM�l�j
	 * @param ipRow [in] ���R�[�h�f�[�^�i�G���[�o�͗p�j
	 * @param cConvTableInfo [in] �ϊ��Ώۃe�[�u�����i�G���[�o�͗p�j
	 * @param ipPoint [in] �`��f�[�^
	 * @param vecCoordStr [out] ������ϊ���f�[�^
	 * @retval true ��������
	 * @retval true �������s
	 */
	bool point2CoordString(const _IRowPtr& ipRow, const CConvTableInfo& cConvTableInfo, const IPointPtr& ipPoint, std::vector<CString>& vecCoordStr);

	/**
	 * @brief �ʏ�t�H�[�}�b�g�̃|�C���g�f�[�^��WKT������ϊ��i�ܓx�o�x�{ZM�l�j
	 * @param ipRow [in] ���R�[�h�f�[�^�i�G���[�o�͗p�j
	 * @param cConvTableInfo [in] �ϊ��Ώۃe�[�u�����i�G���[�o�͗p�j
	 * @param vecCoordStr [in] �`��f�[�^
	 * @param strWKTPoint [out] WKT������ϊ���f�[�^
	 * @retval true ��������
	 * @retval true �������s
	 */
	bool normal2WKTString(const _IRowPtr& ipRow, const CConvTableInfo& cConvTableInfo, const std::vector<CString>& vecCoordStr, CString& strWKTPoint);

	/**
	 * @brief WKT�̌`���`
	 * @param strWKTGeomType [in] WKT������ϊ���f�[�^
	 * @retval true ��������
	 * @retval true �������s
	 */
	CString setGeomTypeWKT(const CString& strWKTGeomType);

	/**
	 * @brief �ۂߍ���
	 * @param mapRoundValue [in] �ۂߍ��ݏ���
	 * @param strValue [in,out] �Ώۃf�[�^
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool roundValue(const std::map<CString,CString>& mapRoundValue, CString& strValue);

	/**
	 * @brief NULL�l�ϊ�
	 * @param ipRow [in] ���R�[�h�f�[�^�i�G���[�o�͗p�j
	 * @param cConvTableInfo [in] �ϊ��Ώۃe�[�u�����i�G���[�o�͗p�j
	 * @param cFieldDef [in] �t�B�[���h�Ή��\�̏��
	 * @param strValue [in] �Ώۃf�[�^
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool null2String(const _IRowPtr& ipRow, const CConvTableInfo& cConvTableInfo, const CFieldDefInfo& cFieldDef, CString& strValue);

	/**
	 * @brief �����^�f�[�^��������ϊ��֐�
	 * @param cConvTableInfo [in] �ϊ��Ώۃe�[�u�����i�G���[�o�͗p�j
	 * @param cFieldDefInfo [in] �t�B�[���h�Ή��\�̏��
	 * @param lValue [in] �ϊ��Ώۃf�[�^
	 * @param ipRow [in] ���R�[�h�f�[�^�i�G���[�o�͗p�j
	 * @param strColumnString [out] �o�͕�����
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool numVal2String(const CConvTableInfo& cConvTableInfo, const CFieldDefInfo& cFieldDefInfo, long lValue, const _IRowPtr& ipRow, CString& strColumnString);
	
	/**
	 * @brief ���������_�^�f�[�^��������ϊ��֐�
	 * @param cConvTableInfo [in] �ϊ��Ώۃe�[�u�����i�G���[�o�͗p�j
	 * @param cFieldDefInfo [in] �t�B�[���h�Ή��\�̏��
	 * @param dValue [in] �ϊ��Ώۃf�[�^
	 * @param ipRow [in] ���R�[�h�f�[�^�i�G���[�o�͗p�j
	 * @param strColumnString [out] �o�͕�����
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool doubleVal2String(const CConvTableInfo& cConvTableInfo, const CFieldDefInfo& cFieldDefInfo, double dValue, const _IRowPtr& ipRow, CString& strColumnString);

	/**
	 * @brief ������^�f�[�^��������ϊ��֐�
	 * @param cConvTableInfo [in] �ϊ��Ώۃe�[�u�����i�G���[�o�͗p�j
	 * @param cFieldDefInfo [in] �t�B�[���h�Ή��\�̏��
	 * @param strValue [in] �ϊ��Ώۃf�[�^
	 * @param ipRow [in] ���R�[�h�f�[�^�i�G���[�o�͗p�j
	 * @param strColumnString [out] �o�͕�����
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool str2String(const CConvTableInfo& cConvTableInfo, const CFieldDefInfo& cFieldDefInfo, CStringW& strValue, const _IRowPtr& ipRow, CString& strColumnString);

private:
	COutputFormat m_cOutputFormat;	//!< �o�̓t�H�[�}�b�g
	bool m_isZ;						//!< Z�l�g�p�t���O
	bool m_isM;						//!< M�l�g�p�t���O
	esriGeometryType m_geomType;	//!< �`��^�C�v
	crd_cnv m_cnv;
};

