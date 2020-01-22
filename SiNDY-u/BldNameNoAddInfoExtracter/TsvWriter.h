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

#include <fstream>

struct AddrInfo;
namespace sindy{
	class CRowContainer;
	typedef boost::shared_ptr<CRowContainer> CSPRowContainer;
}

/// ����
namespace cols{
	enum index{
		SourceType,   //!< BUILDINGNAME_SRC_POINT.SOURCETYPE_C
		SourceName,   //!< BUILDINGNAME_SRC_POINT.SOURCENAME
		SourceId,     //!< BUILDINGNAME_SRC_POINT.SOURCE_ID
		SrcOid,       //!< BUILDINGNAME_POINT.SRC_OID
		Oid,          //!< BUILDINGNAME_POINT.OBJECTID
		Name,         //!< BUILDINGNAME_POINT.NAME
		Yomi,         //!< BUILDINGNAME_POINT.YOMI
		AddrCode,     //!< �Z���R�[�h
		Pref,         //!< �s���{��
		PrefYomi,     //!< �s���{���i���~�j
		City,         //!< �s�撬��
		CityYomi,     //!< �s�撬���i���~�j
		Oaza,         //!< �厚
		OazaYomi,     //!< �厚�i���~�j
		Aza,          //!< ��
		AzaYomi,      //!< ���i���~�j

		num          //!< ��
	};
};


class TsvWriter
{
public:
	TsvWriter(void){}
	virtual ~TsvWriter(void){}

	/**
	* @brief ������
	* @param path [in] �o�͐�t�@�C���̃t���p�X
	*/
	void init( const CString& path );
	
	/**
	* @brief �o�͑Ώۃ��R�[�h��ǉ�����
	* @note �����Ńo�b�t�@���Aflush()����܂Ŏ��ۂ̏������݂͍s��Ȃ�
	* @param bldName [in] �����r������
	* @param srcP [in] �f�ރ|�C���g
	* @param addrInfo [in] �Z���֘A���
	* @param yomi [in] ���~
	*/
	void addRecord( const sindy::CRowContainer& bldName,
		            const sindy::CSPRowContainer& srcP,
					const AddrInfo& addrInfo,
					const CString& yomi );
	
	/**
	* @brief �t�@�C����������
	* @note addRecord()�Œ��߂��񂾃��R�[�h�����ۂɃt�@�C���ɏo�͂���
	*/
	void flush();

private:
	
	std::ofstream m_ofs;		                      //!< �o�̓t�@�C���X�g���[��
	typedef std::vector<CString> record;              //!< 1�s���̏o�̓f�[�^
	typedef std::pair<CString, record> orderedRecord; //!< �Z���R�[�h���ɂ��邽�߂̃��R�[�h���
	std::set<orderedRecord> m_records;                //!< �o�͍s�o�b�t�@�p
};

