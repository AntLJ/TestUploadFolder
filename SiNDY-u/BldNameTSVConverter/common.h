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

#include <TDC/useful_headers/str_util.h>

/**
 * @brief �t�B�[���h�C���f�b�N�X�}�b�v
 */
typedef std::map< CString, long > FieldIndexes;

/**
 * @brief ��؂蕶���i��������j
 */
const CString SEPARATOR_FULL_PIPE = _T("�b");
const CString SEPARATOR_HALF_SLASH = _T("/");

/**
 * @brief   �����r�����̃|�C���g�R���e�i
 */
struct BldNamePointRec
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
	BldNamePointRec() : bldNamePointOID( 0 ), bldgOID( 0 ), contentsCode( 0 ), meshcode( 0 ), x( 0 ), y( 0 ) {}

	/**
	 * @brief �f�X�g���N�^
	 */
	virtual ~BldNamePointRec() {}

	long bldNamePointOID;     //!< �����r�����̃|�C���gOID
	long bldgOID;             //!< �����|���S��OID
	CString name;             //!< ����
	CString nameSep;          //!< ���́i�����j
	CString yomiSep;          //!< �ǂ݁i�����j
	CString bldgClass1;       //!< ������ʃR�[�h1
	CString floors;           //!< �K��
	long contentsCode;        //!< ���LPOI�R���e���c�R�[�h
	CString suitable;         //!< �r�����̑Ó��t���O
	CString addrcode;         //!< �Z���R�[�h
	int meshcode;             //!< 2�����b�V���R�[�h
	int x;                    //!< X���W
	int y;                    //!< Y���W

	/**
	 * @brief   �ǂ݁i�������������j�擾
	 * @return  �ǂ݁i��������������j
	 */
	const CString getRemovedSepStrYomi() { return uh::str_util::replace(yomiSep, SEPARATOR_FULL_PIPE, _T("") ); };

	/**
	 * @brief   ���́i���������u���j�擾
	 * @return  ���́i���������u����j
	 */
	const CString getReplacedSepName() { return uh::str_util::replace(nameSep, SEPARATOR_FULL_PIPE, SEPARATOR_HALF_SLASH ); };

	/**
	 * @brief   �ǂ݁i���������u���j�擾
	 * @return  �ǂ݁i���������u����j
	 */
	const CString getReplacedSepYomi() { return uh::str_util::replace(yomiSep, SEPARATOR_FULL_PIPE, SEPARATOR_HALF_SLASH ); };

	/**
	 * @brief   ���LPOI�f�[�^���ރR�[�h�擾
	 * @note    �R���e���c�R�[�h�̏�ʌ��i������6���ڈȏ�j�𕶎���ŕԂ�
	 * @note    �R���e���c�R�[�h�� 0 �̏ꍇ�͋󕶎����Ԃ�
	 * @return  �f�[�^���ރR�[�h
	 */
	const CString getGrpCode() { return ( 0 != contentsCode ) ? uh::str_util::ToString( static_cast<long>( contentsCode / 100000 ) ) : _T(""); }

	/**
	 * @brief   ���LPOI�X�܃R�[�h�擾
	 * @note    �R���e���c�R�[�h�̉��ʌ��i������5���ڂ܂Łj�𕶎���ŕԂ�
	 * @note    �R���e���c�R�[�h�� 0 �̏ꍇ�͋󕶎����Ԃ�
	 * @return  �X�܃R�[�h
	 */
	const CString getChainCode() { return ( 0 != contentsCode ) ? uh::str_util::ToString( contentsCode % 100000 ) : _T(""); }
};

namespace common_proc
{
	/**
	 * @brief ���[�N�X�y�[�X�̎擾
	 * @param strConnect [in] ���[�N�X�y�[�X���iDB���j
	 * @param ipWorkspace [out] ���[�N�X�y�[�X
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool getWorkspace( const CString& strConnect, IWorkspacePtr& ipWorkspace );

	/**
	 * @brief �t�B�[�`���N���X�̎擾
	 * @param strName [in] �e�[�u����
	 * @param ipWorkspace [in] ���[�N�X�y�[�X
	 * @param ipFeatureClass [out] �t�B�[�`���N���X
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool getFeatureClass( const CString& strName, const IWorkspacePtr& ipWorkspace, IFeatureClassPtr& ipFeatureClass );

	/**
	 * @brief �e�[�u���̎擾
	 * @param strName [in] �e�[�u����
	 * @param ipWorkspace [in] ���[�N�X�y�[�X
	 * @param ipTable [out] �e�[�u��
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool getTable( const CString& strName, const IWorkspacePtr& ipWorkspace, ITablePtr& ipTable );

	/**
	 * @breif �t�B�[���h�C���f�b�N�X�}�b�v�̎擾
	 * @param ipTable [in] �Ώۃe�[�u��
	 * @param mapIndexes [out] �t�B�[���h�C���f�b�N�X�}�b�v
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool getFieldIndexes( const ITablePtr& ipTable, FieldIndexes& mapIndexes );

	/**
	 * @brief �t�B�[���h�̎擾
	 * @param ipTable [in] �Ώۃe�[�u��
	 * @param strFieldName [in] �Ώۃt�B�[���h��
	 * @param ipField [out] �t�B�[���h
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool getField( const ITablePtr& ipTable, const CString& strFieldName, IFieldPtr& ipField );

	/**
	 * @brief �t�B�[���h�l�̎擾
	 * @param ipRow [in] �Ώۃ��R�[�h
	 * @param strFieldName [in] �Ώۃt�B�[���h��
	 * @param mapIndexes [in] �t�B�[���h�C���f�b�N�X�}�b�v
	 * @param varValue [out] �擾�����l
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool getValue( const _IRowPtr& ipRow, const CString& strFieldName, FieldIndexes& mapIndexes, CComVariant& varValue );

	/**
	 * @breif �R�[�h�l�h���C���Œ�`����Ă���R�[�h�l�̃Z�b�g���擾
	 * @note �����ł̃R�[�h�l�h���C���͐����^�݂̂�ΏۂƂ���
	 * @param ipField [in] �Ώۃt�B�[���h
	 * @param mapValues [out] �R�[�h�l�h���C���Œ�`����Ă���R�[�h�l�Ɛ����̃}�b�v
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool getCodedValues( const IFieldPtr& ipField, std::map< long, CString >& mapValues );

	/**
	 * @breif �R�[�h�l�h���C���Œ�`����Ă���R�[�h�l�̃Z�b�g���擾
	 * @note �����ł̃R�[�h�l�h���C���͐����^�݂̂�ΏۂƂ���
	 * @param ipTable [in] �Ώۃe�[�u��
	 * @param strFieldName [in] �Ώۃt�B�[���h��
	 * @param mapValues [out] �R�[�h�l�h���C���Œ�`����Ă���R�[�h�l�Ɛ����̃}�b�v
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool getCodedValues( const ITablePtr& ipTable, const CString& strFieldName, std::map< long, CString >& mapValues );

	/**
	 * @brief Where��ɂ�錟��
	 * @param ipFeatureClass [in] �����Ώۃt�B�[�`���N���X
	 * @param ipFeatureCursor [out] �t�B�[�`���J�[�\���i�������ʁj
	 * @param lCount [out] �����t�B�[�`����
	 * @param strWhere [in] Where��
	 * @param strPostfix [in] �ڔ���i�uORDER BY ��v�Ȃǁj
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool searchByWhereClause( const IFeatureClassPtr& ipFeatureClass, IFeatureCursorPtr& ipFeatureCursor, long& lCount, const CString& strWhere = _T(""), const CString& strPostfix = _T("") );

	/**
	 * @brief �`��ɂ���Ԍ���
	 * @param ipFeatureClass [in] �����Ώۃt�B�[�`���N���X
	 * @param ipFeatureCursor [out] �t�B�[�`���J�[�\���i�������ʁj
	 * @param lCount [out] �����t�B�[�`����
	 * @param ipGeometry [in] �������`��
	 * @param spRel [in] �����Ώۂƌ������`��̋�ԓI�֌W
	 * @param strPostfix [in] �ڔ���i�uORDER BY ��v�Ȃǁj
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool searchByGeometry( const IFeatureClassPtr& ipFeatureClass, IFeatureCursorPtr& ipFeatureCursor, long& lCount, const IGeometryPtr& ipGeometry, esriSpatialRelEnum spRel, const CString& strPostfix = _T("") );

	/**
	 * @brief   �����R�[�h�ϊ��iEUC��SJIS�j
	 * @param   str [in] �ϊ��Ώە�����(EUC)
	 * @return  �ϊ��ςݕ�����(SJIS)
	 */
	std::string convEUC2SJIS( const std::string& str );

	/**
	 * @brief   �����R�[�h�ϊ��iSJIS��EUC�j
	 * @param   str [in] �ϊ��Ώە�����(SJIS)
	 * @return  �ϊ��ςݕ�����(EUC)
	 */
	std::string convSJIS2EUC( const std::string& str );

} // namespace common_proc
