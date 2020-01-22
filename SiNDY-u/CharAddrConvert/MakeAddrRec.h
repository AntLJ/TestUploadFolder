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

#include "CodeConvTable.h"
#include <oo4o/include/oracl.h>
#pragma comment(lib, "../../LIB/oo4o/lib/ORACLM32.LIB")

/**
 * @enum	EADDR_STATUS
 * @brief	�Z���X�e�[�^�X
 */
enum EADDR_STATUS
{
	kNormal,			//!< �ʏ�Z��
	kExtGaiku,			//!< �g���X�敄������
	kMojiGou,			//!< ���ԍ��ɕ�������
	kMojiGou_ExtGaiku,	//!< ���ԍ��ɕ�������(�g���X�敄����)
	kSepOver,			//!< ���ԍ��̋�؂肪����
	kSepOver_ExtGaiku,	//!< ���ԍ��̋�؂肪����(�g���X�敄����)
	kNoOver,			//!< �n�ԁE���ŕ\��������Ȃ��ԍ�
	kNoOver_ExtGaiku,	//!< �n�ԁE���ŕ\��������Ȃ��ԍ�(�g���X�敄����)
};

/**
 * @class	CMakeAddrRec
 * @brief	�Z�����R�[�h�쐬�N���X
 */
class CMakeAddrRec
{
public:
	CMakeAddrRec(void) : m_lBGClass_FIdx(-1), m_lCreateYear_FIdx(-1){}
	~CMakeAddrRec(void){}

	/**
	 * @brief ������
	 * @param ipCityAdminFC    [in] �s�s�n�}�s���E�t�B�[�`���N���X
	 * @param ipCitySiteFC     [in] �s�s�n�}�w�i�t�B�[�`���N���X
	 * @param ipBuildingFC     [in] �s�s�n�}�����t�B�[�`���N���X
	 * @param ipBuildingRoofFC [in] �s�s�n�}����t�B�[�`���N���X
	 * @param ipCityMeshFC     [in] �s�s�n�}���b�V���t�B�[�`���N���X
	 * @param lpcszCodeConvDB  [in] �R�[�h�ϊ�DB�ڑ���
	 * @param lpcszJusyoDB     [in] �Z���}�X�^�ڑ���
	 * @return bool
	 */
	bool Init( const IFeatureClassPtr& ipCityAdminFC, const IFeatureClassPtr& ipCitySiteFC, const IFeatureClassPtr& ipBuildingFC, const IFeatureClassPtr& ipBuildingRoofFC, const IFeatureClassPtr& ipCityMeshFC, LPCTSTR lpcszCodeConvDB, LPCTSTR lpcszJusyoDB );

	/**
	 * @brief	�s���E����n�ԑ�\�_����
	 * @param	cAdmin		[in]		�s���E���
	 * @param	rContainer	[in, out]	Pnt���R�[�h�i�[�R���e�i
	 */
	void MakePntRecordByAdmin( const CAdminInfo& cAdmin, PNTMAP& rContainer );

	/**
	 * @brief	�����R�[�h����n�ԑ�\�_����
	 * @param	rGouRec		[in]		�����R�[�h�i�[
	 * @param	rContainer	[in, out]	Gou���R�[�h�i�[�R���e�i
	 */
	void MakePntRecordByGou( const GOUMAP& rGouRec, PNTMAP& rContainer );

	/**
	 * @brief	�����R�[�h�쐬
	 * @param	cAdmin		[in]		�s���E���
	 * @param	cGou		[in]		���|�C���g���
	 * @param	rContainer	[in,out]	�����R�[�h�i�[�R���e�i
	 */
	void MakeGouRecordByDB( const CAdminInfo& cAdmin, const CGouInfo& cGou, GOUMAP& rContainer );

	/**
	 * @brief	�����R�[�h�ꗗ�����\�Z���Ƀt�B���^�����O
	 * @param	rGouMap		[in]		�����R�[�h�ꗗ
	 * @param	rContainer	[in, out]	��\�����R�[�h�ꗗ
	 */
	void SelectRepGouRecord( const GOUMAP& rGouMap, GOUMAP& rContainer );

	/**
	 * @brief	�L����11���H
	 * @param	lpcszAddrCode	[in]	11���Z���R�[�h
	 * @return	bool
	 */
	bool IsValidAddrCode( LPCTSTR lpcszAddrCode )
	{
		return m_ZipMap.find(lpcszAddrCode) != m_ZipMap.end();
	}

private:

	/**
	 * @brief	�����t���Z���H
 	 * @param	cAdmin	[in]	�s���E���
	 * @param	cGou	[in]	���|�C���g���
	 * @return	�X�e�[�^�X
	 */
	EADDR_STATUS IsCharAddr( const CAdminInfo& cAdmin, const CGouInfo& cGou );

	/**
	 * @brief	�v���b�gNG�Ȕw�i�H
	 * @param	ipPoint	[in]	�|�C���g
	 * @return	bool
	 */
	bool IsNGSite( IPoint* ipPoint );
		
	/**
	 * @brief	����|���S���̖ʐώ擾
	 * @param	ipBldGeometry	[in]	�ƌ`�W�I���g��
	 * @return	�ʐ�(����|���S�����Ȃ���΁A0)
	 */
	double GetRoofArea( const IGeometryPtr& ipBldGeometry );

	/**
	 * @brief	���|�C���g�����݂���ƌ`�̖ʐώ擾
	 * @param	ipPoint	[in]	�|�C���g
	 * @return	�ʐ�(�ƌ`���Ƀ|�C���g���Ȃ���΁A0)
	 */
	double GetContainsBldArea( const IPointPtr& ipPoint );

	/**
	 * @brief	�X�֔ԍ��擾
	 * @param	lpcszAddrCode	[in]	11���Z���R�[�h
	 * @return	�X�֔ԍ�(�Ȃ���Εs��[0000000])
	 */
	CString GetZipCode( LPCTSTR lpcszAddrCode )
	{
		std::map<CString, CString>::iterator itr = m_ZipMap.find( lpcszAddrCode );
		return itr != m_ZipMap.end()? itr->second : _T("0000000");
	}

	/**
	 * @brief	�����R�[�h����n�ԑ�\�_�ƂȂ肤�鍆�|�C���g�I��
	 * @param	listGou		[in]		����16��GOU���X�g
	 * @param	listTpg		[in]		����16��TPG���X�g
	 * @param	rGouRec		[in,out]	�n�ԑ�\�_�Ώۂ�GOu���R�[�h�i�[�p
	 * @param	bOazaIgnore	[in]		�厚�������|�C���g�𖳎�����H
	 * @return	��\�_�E�n�Ԏ~�܂�t���O
	 */
	long SelectPntFromGouList( const GOULIST& listGou, const GOULIST& listTpg, CGouRec& rGouRec, bool bOazaIgnore = true );

	/**
	 * @brief	�����R�[�h�̈ʒu�֌W����n�ԑ�\�_�ƂȂ肤�鍆�|�C���g�I��
	 * @note	�n�ԑ�\�_���e�f�[�^�ʒu�����Ɍ���
	 * @param	listChiban		[in]	����16���n�Ԏ~�܂�
	 * @param	listNotChiban	[in]	����16���n�Ԏ~�܂�łȂ����X�g
	 * @param	rGouRec	[in,out]	�n�ԑ�\�_�Ώۂ�GOu���R�[�h�i�[�p
	 */
	void SelectPntFromGouDist( const GOULIST& listChiban, const GOULIST& listNotChiban, CGouRec& rGouRec );

private:

	CCodeConvTable   m_cConvTable;       //!< �R�[�h�Ή��\
	CCharAddrConvert m_cYomiTable;       //!< ���~�t�^�N���X
	
	IFeatureClassPtr m_ipCityAdminFC;    //!< �s�s�n�}�s���E�t�B�[�`���N���X
	IFeatureClassPtr m_ipCitySiteFC;     //!< �s�s�n�}�w�i�t�B�[�`���N���X
	IFeatureClassPtr m_ipBuildingFC;     //!< �s�s�n�}�����t�B�[�`���N���X
	IFeatureClassPtr m_ipBuildingRoofFC; //!< �s�s�n�}��������t�B�[�`���N���X
	IFeatureClassPtr m_ipCityMeshFC;     //!< �s�s�n�}���b�V���t�B�[�`���N���X

	long             m_lBGClass_FIdx;    //!< �s�s�w�i��ʃt�B�[���h�C���f�b�N�X
	long             m_lCreateYear_FIdx; //!< ���b�V���쐬�N�x�t�B�[���h�C���f�b�N�X

	std::map<CString, CString> m_ZipMap; //!< �X�֔ԍ����X�g(�L�[:11���R�[�h �l:�X�֔ԍ�) ... 11�����Z���}�X�^�ɑ��݂��邩�ɂ����p����
};
