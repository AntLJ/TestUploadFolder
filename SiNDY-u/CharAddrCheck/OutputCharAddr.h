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

#include <oracl.h>

#include "CharAddrConvertFunc.h"

class COutputCharAddr
{
public:
	COutputCharAddr();
	~COutputCharAddr();

public:
	/**
	* @brief	�ݒ菉����
	* @param	iGouPoint[in]				���|�C���g�N���X
	* @param	ipCityAdmin[in]				�s�s�n�}�s���E�N���X
	* @param	eOraDB[in]					Oracle�ڑ���
	* @param	ipWorkPorposeTable[in]		��ƖړI�e�[�u��
	* @param	lpcszOutput[in]				�o�̓f�B���N�g��
	*/
	void Init( IFeatureClass* ipGouPoint, IFeatureClass* ipCityAdmin, ODatabase& eOraDB, LPCTSTR lpcszOutput, ITable* ipWorkPorposeTable );

	/**
	* @brief	�������s���\�b�h
	* @retval	true	:����
	* @retval	false	:���s
	*/
	bool run( LPCTSTR lpcszConvertTablePath, LPCTSTR lpcszGouPoint, LPCTSTR lpcszCityAdmin );

private:
	/**
	* @brief	�����t���Z���̕�����𕪊����ă��X�g��
	* @param	ipCityAdmin[in]				�s�s�n�}�s���E�N���X
	* @param	mapList[in]					�����t���Z���̃t�B�[�`�����X�g
	* @param	lType[in]					�����^�C�v�i0:���|�C���g���X�g,1:�s�s�n�}�s���E���X�g�j
	* @return	�����t���Z���̕�����𕪊������P��ƍs���E�R�[�h�̃��X�g
	*/
	std::map<CString,std::map<CString,long>> getDataFromList( IFeatureClass* ipCityAdmin, const std::map<long,std::pair<CString,CAdapt<IFeaturePtr>>>& mapList, long lType );

	/**
	* @brief	���X�g�𓝍�����
	* @param	mapGouCharList[in,out]		���|�C���g���X�g�i������ɓ��������j
	* @param	mapCityAdminChar[in]		�s�s�n�}�s���E���X�g
	*/
	void CombineCharList( std::map<CString,std::map<CString,long>>& mapGouCharList, const std::map<CString,std::map<CString,long>>& mapCityAdminChar );

	/**
	* @brief	�����t���Z�����X�g�o��
	* @note		�P�ɕ����t���Z���̃��X�g��OID�ƕ�������o�͂��邾��
	* @param	mapItems[in]				�����t���Z�����X�g
	* @param	lpcszOutputFile[in]			�o�̓t�@�C�����i�g���q���܂܂Ȃ����Ɓj
	* @param	lpcszTableName[in]			�e�[�u����
	* @param	ipFeatureClass[in]			�e�[�u��
	*/
	void WriteCharAddrList( const std::map<long,std::pair<CString,CAdapt<IFeaturePtr>>>& mapItems, LPCTSTR lpcszOutputFile, LPCTSTR lpcszTableName, IFeatureClass* ipFeatureClass );

	/**
	* @brief	�����t���Z���𕪊�����������Q���Z�����ɏo��
	* @param	mapCharAddr[in]				�����t���Z���𕪊�����������Q���X�g
	* @param	lpcszOutputFile[in]			�o�̓t�@�C�����i�g���q���܂܂Ȃ����Ɓj
	*/
	void WriteCharAddrList2( const std::map<CString,std::map<CString,long>>& mapCharAddr, LPCTSTR lpcszOutputFile );

private:
	CCharAddrConvert m_cCharAddrConvert;		//!< �ϊ��e�[�u���N���X
	IFeatureClassPtr m_ipGouPointClass;			//!< ���|�C���g�N���X
	IFeatureClassPtr m_ipCityAdminClass;		//!< �s���E�N���X
	CString m_strOutput;						//!< �o�̓f�B���N�g��
	ODatabase m_eOraDB;							//!< �ڑ�DB
	ITablePtr m_ipWorkPorposeTable;				//!< ��ƖړI�e�[�u��
};
