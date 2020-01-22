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

#include <FieldMap.h>
#include "CommonData.h"

/**
* @brief ����e�[�u���Ƃ���e�[�u�����֘A�t���A�K�v�ȏ����擾���邽�߂̃N���X
* @warning  �����e�[�u�����񌩂�悤�ȏ󋵂ɑΉ��ł��Ȃ�
* @warning  :ex. ����STATION_POI��GUIDEID��STATION_GUIDE_POINT��OBJECTID��STATION_GUIDE_POINT��OID�����S�Ă�STATION_POINT��OID
* @warning  :���̏ꍇ�A�C���X�^���X���쐬���A���ꂼ���STATION_POI_��STATION_GUIDE_POINT�ASTATION_GUIDE_POINT��STATION_POINT���֘A�t����
* @warning  ������A���������ƁA�̂悤�ȓǂݍ��݂��ł��Ȃ��B���̂悤�Ȃ��̂��~�����ꍇ�͈ꂩ���蒼�����ƁB
*/
class CTableManagerBase
{
	/**
	* @brief �ǂݎ�����f�[�^�𑽕��؏�Ɋi�[���邽�߂̃N���X
	* @note  ������AHeader�ƂȂ�node��m_attrList�͏�ɋ�ł���͂�
	*/
	class CSearchList
	{
	public:
		std::list<class CSearchList> m_childList; //!< �q�ƂȂ�m�[�h
		AOI_LINE m_attrList;            //!< ���̃m�[�h��������
		/// �R���X�g���N�^
		CSearchList(){}
		/// �f�X�g���N�^
		virtual ~CSearchList(){};
		/// �R�s�[�R���X�g���N�^
		CSearchList(const CSearchList& c)
		{
			m_childList = c.m_childList;
			m_attrList = c.m_attrList;
		}
		/**
		 * @brief ������Z�q
		 * @param[in] c �����CSearchList
		 * @return CSearchList
		 */
		CSearchList operator=(const CSearchList& c)
		{
			CSearchList temp;
			temp.m_childList = c.m_childList;
			temp.m_attrList = c.m_attrList;
			return temp;
		}
	};

public:

	/**
	* @brief ���̃N���X�ŊJ�����e�[�u���́A�����ŃZ�b�g�����e�[�u���̃I�[�i�[�Ɠ����ɂȂ�
	* @warning �K���ŏ��Ɉ�x�Ăяo��
	*/
	void setTable(ITablePtr& ipTale)
	{
		m_ipTable = ipTale;
	}

	/**
	* @brief �j�e�[�u���Ԃł̘A���ɕK�v�ȏ���ݒ肷��
	* @warning Main->Rel1->Rel2->Rel3->Main�̂悤�ɁA�������[�v������Ă��܂���̂Œ���
	* @warning Main->Rel1, Main->Rel2 �̂悤�ɁA����e�[�u�������ȏ�̃e�[�u���Ɋ֘A���������邱�Ƃ͂ł��Ȃ�
	* @param[in] strMain    ��̂ƂȂ�e�[�u����
	* @param[in] strSub     ��̂Ɋ֘A�Â��e�[�u����
	* @param[in] strMainKey ��̂ŃL�[�ƂȂ�t�B�[���h��
	* @param[in] strSubKey  ��̂���֘A�����������ۂɃL�[�ƂȂ�t�B�[���h��
	*/
	void Join(const CString& strMain, const CString& strSub, const CString& strMainKey, const CString& strSubKey);

	/**
	* @brief �l���擾
	* @param[in] key       �t�B���^�ɗ^����l(�t�B���^�ɗ^���錟��������"OBJECTID={����}")
	* @param[in] fieldName �t�B���^�őΏۂƂ���t�B�[���h��(�t�B���^�ɗ^���錟��������"{����}=1")
	* @param[in] strTable  �����̊J�n�_�Ƃ���e�[�u����
	* return retList �擾�����f�[�^�̃��X�g
	*/
	AOI_MATRIX GetValue( const CString& key, const CString& filedName, const CString& strTable );

	/**
	* @brief DB����擾����l��ݒ�
	* @param[in] dataList  �擾����l�̃��X�g(�e�v�f��[�e�[�u����,�l���擾����t�B�[���h��]�ƂȂ��Ă���)
	*/
	void SetReturnData( const std::list<std::pair<CString,CString>>& dataList );

private:

	/**
	* @brief �l���擾
	* @param[in] ipCursor strTable�������������ʂ�ێ������J�[�\��
	* @param[in] strTable �����̑ΏۂƂȂ�e�[�u����
	* return retList �擾�����f�[�^�̃��X�g
	*/
	AOI_MATRIX getValue( _ICursorPtr& ipCursor, const CString& strTable );

	/**
	* @brief DB����l��ǂݏo���A��������ɕێ����鏈��(�ċA)
	* @param[in]     ipCursor strTable�������������ʂ�ێ������J�[�\��
	* @param[in]     strTable �����̑ΏۂƂȂ�e�[�u����
	* @param[in,out] parent   �ǂݍ��񂾃f�[�^���i�[���鑽����
	*/
	void loadValue( _ICursorPtr& ipCursor, const CString& strTable, CSearchList& parent );

	/**
	*  @brief _IRowPtr����l���擾����
	*  @note  CSearchList�Ɏ��ۂɒl�����Ă���̂͂���
	*  @param[in]     ipRow    �Ώۂ�_IRowPtr
	*  @param[in]     strTable ipRow�̑Ώۂł���e�[�u����
	*  @param[in,out] addList  �擾�����l���i�[���郊�X�g
	*/
	void getRowValue( _IRowPtr& ipRow, const CString& strTable, AOI_LINE& addList );

	/**
	*  @brief strTable����������ۂ̃T�u�t�B�[���h�𐶐�
	*  @param[in,out] strSubField  �T�u�t�B�[���h��������i�[���镶����
	*  @param[in]     strTable     �T�u�t�B�[���h���g�p����e�[�u����
	*  @param[in,out] keyField     strTable����������ۂ̃L�[�ƂȂ�t�B�[���h��
	*/
	void setStrSubFields( CString& strSubField , const CString& strTable, const CString& keyField = CString() );

	/**
	*  @brief SearchList��AOI_MATRIX�̌`���ɕϊ�(�ċA)
	*  @param[in]     ops          �ϊ����錳�ƂȂ�
	*  @param[in,out] lList        �ϊ���̌��ʂ��i�[���郊�X�g
	*/
	void searchList2ListList( CSearchList& ops, AOI_MATRIX& lList ) const ;

	/**
	* @brief �t�B�[���h�}�b�v���쐬
	* @param[in] �t�B�[���h�}�b�v���쐬����e�[�u���̖���
	* @param[in,out] �t�B�[���h�}�b�v�̍쐬��
	* @retval true  ����
	* @retval false ���s
	*/
	bool createFieldMap( const CString& strTable, sindy::CFieldMap& fieldMap );

private:
	/**
	* @brief ����e�[�u���ɑ΂��āA���Ɍ�������e�[�u���ƁA���̃e�[�u������������̂ɕK�v�ȃt�B�[���h�����i�[
	* @note  [����e�[�u����,[���Ɍ�������e�[�u����,���Ɍ�������e�[�u������������̂ɕK�v�ȁA����e�[�u���̃t�B�[���h��]]
	*/
	typedef std::map<CString,std::pair<CString,CString>> JOIN_MAP;


	JOIN_MAP m_joinMap;                               //!< ����e�[�u���ɑ΂��Ď��̌����e�[�u���ƂȂ���̂ƁA�����ɕK�v�ȃt�B�[���h�����i�[

	std::map<CString,sindy::CFieldMap> m_FieldMaps;   //!< �e�e�[�u���̃t�B�[���h�}�b�v
	std::map<CString,FIELDNAMES> m_subFields;         //!< �e�[�u�����Ɠǂݍ��ރt�B�[���h�̑Ή�

	std::map<CString,CString> m_tableSearchedKey;     //!< ��������鑤�ɂȂ����Ƃ��ɁA�L�[�ƂȂ�t�B�[���h���i�[

	/**
	* @brief �ŏI�I�Ɏ擾����f�[�^�̑Ή����i�[
	* @note  �e�v�f�ɂ�[�e�[�u����,�t�B�[���h��]�������Ă���z��
	*/
	std::list<std::pair<CString,CString>> m_dataList;

	ITablePtr m_ipTable; //!< ����Ɠ������[�U�[�̃e�[�u�����J�����ƂɂȂ�
protected:

};
