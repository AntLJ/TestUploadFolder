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

#include "CharAddrConvertFunc.h"

/**
* @brief	���ۂ̍��|�C���g�A�s���E���ǂݕϊ��e�[�u���ŕϊ��ł��邩�`�F�b�N����N���X
*/
class CCheckConvert
{
public:
	CCheckConvert();
	~CCheckConvert();

public:
	/**
	* @brief	�ݒ菉����
	* @param	iGouPoint[in]				���|�C���g�N���X
	* @param	ipCityAdmin[in]				�s�s�n�}�s���E�N���X
	* @param	lpcszConvertTable[in]		�ϊ��e�[�u����PGDB�p�X
	* @param	lpcszOutput[in]				�o�̓f�B���N�g��
	*/
	void Init( IFeatureClass* ipGouPoint, IFeatureClass* ipCityAdmin, LPCTSTR lpcszConvertTablePath, LPCTSTR lpcszOutput );

	/**
	* @brief	�������s���\�b�h
	* @retval	true	:����
	* @retval	false	:���s
	*/
	bool run();

	/**
	* @brief	NG���X�g�p�X�o��
	* @param	strGouPointText[out]		���|�C���gNG���X�g
	* @param	strCityAdminText[out]		�s�s�n�}�s���ENG���X�g
	*/
	bool get_GouCityAdminNG(CString& strGouPointText, CString& strCityAdminText);

private:
	/**
	* @brief	GetItems()�œ���ꂽ���X�g�������ǂݕϊ����X�g�ɑΉ����Ă��邩�`�F�b�N
	* @param	mapItem[in]					GetItems()�œ���ꂽ���X�g
	* @param	mapConvertedItems[in,out]	���̏����őΉ��������́i�����ǂݕϊ��œ���ꂽ���X�g�j
	*/
	void ConvertItemsFromKanjiYomiTable( const std::map<long,std::pair<CString,CAdapt<IFeaturePtr>>>& mapItems, std::map<long,std::pair<CString,long>>& mapConvertedItems );

	/**
	* @brief	GetItems()�œ���ꂽ���X�g���I�u�W�F�N�g�ǂݕϊ����X�g�ɑΉ����Ă��邩�`�F�b�N
	* @param	mapItem[in,out]				GetItems()�œ���ꂽ���X�g���Ή��������͍̂폜�����
	* @param	mapConvertedItems[in,out]	���̏����őΉ��������́i�I�u�W�F�N�gID�ǂݕϊ��œ���ꂽ���X�g�j
	* @param	LayerType[in]				���C���^�C�v[1:�s�s�n�}�s���E,2:���|�C���g]
	*/
	void ConvertItemsFromObjectYomiTable( std::map<long,std::pair<CString,CAdapt<IFeaturePtr>>>& mapItems, std::map<long,std::pair<CString,long>>& mapConvertedItems, GouOrCityAddrLayer LayerType );

	/**
	* @brief	�ϊ����ʂ̏o��
	* @param	mapConvertedItems[in]		�ϊ����ʂ̃A�C�e���Q
	* @param	lpcszOutputFile[in]			�o�̓t�@�C����
	* @param	strNgFilePath[out]			NG�o�̓t�@�C���p�X
	* @retval	true	NG�݂�
	* @retval	false	NG����
	*/
	bool WriteConvertResult( const std::map<long,std::pair<CString,long>>& mapConvertedItems, LPCTSTR lpcszOutputFile, CString& strNGFilePath );

private:
	CCharAddrConvert m_cCharAddrConvert;		//!< �ϊ��e�[�u���N���X
	IFeatureClassPtr m_ipGouPointClass;			//!< ���|�C���g�N���X
	IFeatureClassPtr m_ipCityAdminClass;		//!< �s���E�N���X
	CString m_strConvertTable;					//!< �ϊ��e�[�u��PGDB�p�X
	CString m_strOutput;						//!< �o�̓f�B���N�g��

	CString m_strNGGouFilePath;					//!< convertGouPoint_NG_*.txt
	CString m_strNGCityAdminFilePath;			//!< convertCityAdmin_NG_*.txt
	bool m_bGouNG;								//!< NG���X�g����Ŗ����ꍇ��True
	bool m_bCityAdminNG;						//!< NG���X�g����Ŗ����ꍇ��True
};
