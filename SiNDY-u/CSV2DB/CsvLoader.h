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
#include <WinLib/ADOBase.h>

namespace sindy{
	class CTableContainer;
	class CFieldMap;
}

extern const CString LAT_FIELD;     //!< �o�x�t�B�[���h
extern const CString LON_FIELD;     //!< �ܓx�t�B�[���h

/// CSV�t�B�[���h���Ǘ��p
struct FieldInfo
{
private:
	FieldInfo();
public:
	/// �R���X�g���N�^
	FieldInfo( long csvIdx, long destIdx, const CString& field, bool opt );
	
	/// OBJECTID�t�B�[���h��
	static bool isOidField( const CString& fieldName ){
		return (fieldName.CompareNoCase( sindy::schema::kObjectID ) == 0);
	}
	bool isOidField() const { return isOidField( fieldName ); }
	
	/// LAT�t�B�[���h��
	static bool isLatField( const CString& fieldName ){
		return (fieldName.CompareNoCase( LAT_FIELD ) == 0); 
	}
	
	/// LON�t�B�[���h��
	static bool isLonField( const CString& fieldName ){
		return (fieldName.CompareNoCase( LON_FIELD ) == 0); 
	}

	/// LAN�܂��́ALAT�t�B�[���h��
	static bool isLatLonField( const CString& fieldName )	{
		return ( isLatField(fieldName) || isLonField(fieldName) );
	}
	bool isLatLonField() const { return isLatLonField( fieldName ); }


	bool operator == (const FieldInfo& fi ) const{
		return fi.fieldName == fieldName;
	}

	bool operator < (const FieldInfo& fi ) const{
		return csvIndex < fi.csvIndex;
	}

public:
	long csvIndex;      //!< CSV���C���f�b�N�X
	long destIndex;     //!< �e�[�u�����C���f�b�N�X
	CString fieldName;  //!< �t�B�[���h��
	bool forceOutput;   //!< �����o�̓t���O
	bool isChanged;     //!< �ύX�t���O
};

/// CSV���R�[�h���Ǘ��p
struct RecordInfo
{
public:
	// ���̃N���X�́A��ʂɃR���e�i�ɓ˂����񂾂肻����R�s�[�i�ł͂Ȃ����j�����肳���B
	// �����Ȃ��i�f�t�H���g�j�R���X�g���N�^���������O�ŗp�ӂ���̂ł���΁A�R���p�C�����C�𗘂�����
	// ���[�u�R���X�g���N�^�Ƒ�����Z�q��p�ӂ��Ă���邪�A�ǂ���Ǝv���ăf�X�g���N�^��R�s�[�R���X�g���N�^��
	// ����Ɏ��O�ō���Ă��܂��ƁA�C�𗘂����Ă���Ȃ��Ȃ�A�p�t�H�[�}���X����������������B
	// �Ȃ̂ŁA���̂悤�ȃR���X�g���N�^�����Ȃ���΂Ȃ�Ȃ��ꍇ�́A�K�����[�u�̖ʓ|�����邱�ƁB
	/// �R���X�g���N�^
	RecordInfo();

	/// �I�y���[�^
	CComVariant& operator [](const FieldInfo& fi);

	bool setShape( const CString& fieldName, CComVariant& val );

	typedef std::pair<FieldInfo, CComVariant> fieldValue;       //!< �P�t�B�[���h���̏��ikey:�t�B�[���h��� val:�l�j
	typedef std::list<fieldValue> fieldValues;                  //!< �S�t�B�[���h���̏��

	/// �R���e�i����p
	fieldValues::const_iterator begin() const { return m_fieldValues.cbegin();}
	fieldValues::const_iterator end() const { return m_fieldValues.cend();}
	fieldValues::iterator begin(){ return m_fieldValues.begin();}
	fieldValues::iterator end(){ return m_fieldValues.end();}

	bool isCreate;    //!< �V�K�쐬���郌�R�[�h��
	long oid;         //!< �Ώۂ�OID�i�V�K��-1�j
	bool hasShape;    //!< LAN,LOT�̒l���w�肳��Ă��邩
	WKSPoint latlon;  //!< LAN,LOT

private:
	fieldValues m_fieldValues; //!< 1���R�[�h�̑S�t�B�[���h���
};

class CsvLoader
{
public:
	/**
	* @brief csv�̃��R�[�h��ǂݍ���
	* @note  �G���[���L�����ꍇ�͗�O�𓊂���
	* @param path  [in] csv�t�@�C���̃t���p�X
	* @param optFields [in] �����I�ɏo�͂���t�B�[���h�i","��؂�j
	* @param table [in] �X�V�Ώۂ̃e�[�u�� 
	*/
	void Load( const CString& path, const CString& optFields, const sindy::CTableContainer& table );

	/**
	* @brief OID�Ō�������
	* @note ������Ȃ��ꍇ�͗�O�𓊂���
	* @return �Ώۂ̃��R�[�h���
	*/
	const RecordInfo& find_by_oid( long oid ) const;

	/**
	* @brief �X�V�Ώۂ�OID�����ׂĎ擾����
	* @note �V�K�ǉ��ΏہiOID�F-1�j�͊܂܂Ȃ�
	* @return �X�V�Ώۂ�OBJECTID�Q
	*/
	std::vector< long > GetUpdateIDs() const;

	/**
	* @brief �V�K�ǉ��Ώۂ̃��R�[�h�Q���擾����
	* @return �V�K�ǉ������\��̃��R�[�h����
	*/
	typedef std::vector< RecordInfo > CsvRecords;
	std::vector< CsvRecords > GetNewRecordsList() const;
	
	/**
	* @brief �S�t�B�[���h�����擾����
	* @return CSV�̃��R�[�h���Q
	*/
	const std::vector<FieldInfo>& GetFieldInfos() const { return m_fieldInfos; }
	
	/// CSV���R�[�h�Ǘ��p�i first:�s�ԍ� second:���R�[�h��� �j
	typedef std::pair<long, RecordInfo> CsvRecord;


	/// �R���e�i�Ǘ��p
	std::vector< CsvRecord >::iterator begin() { return m_records.begin(); }
	std::vector< CsvRecord >::iterator end() { return m_records.end(); }
	std::vector< CsvRecord >::const_iterator cbegin() const { return m_records.cbegin(); }
	std::vector< CsvRecord >::const_iterator cend() const { return m_records.cend(); }
	size_t size() const { return m_records.size(); } 
	
	
private:
	/**
	* @brief csv���J���A�S���R�[�h��ǂݍ���
	* @note �G���[���������ꍇ�͗�O�𓊂���
	* @param path [in] csv�t�@�C���̃t���p�X
	*/
	void TryOpenCSV( const CString& path );
	
	/**
	* @brief CSV�t�@�C�����J���A�S�s��ADO�J�[�\���Ɏ擾����
	* @note �G���[���������ꍇ�͗�O�𓊂���
	* @param path [in] csv�t�@�C���̃t���p�X
	* @param destField [in] �X�V�Ώۃe�[�u���̃t�B�[���h�}�b�v
	* @param optFields [in] �����o�͑Ώۂ̃t�B�[���h���i","��؂�j
	*/
	void init( const CString& path, const sindy::CFieldMap& destField, const CString& optFields );
	
	/**
	* @brief ���R�[�h����ǉ�����
	* @param lineNum [in] �s�ԍ�
	* @param record [in] ���R�[�h���
	*/
	void Add( long lineNum, const RecordInfo& record );

private:
	CADOBase m_ado;                          //!< ADO
	CADOCursor m_adoCur;                     //!< �J�[�\��

	std::vector<FieldInfo> m_fieldInfos;     //!< �t�B�[���h���
	std::vector< CsvRecord > m_records;      //!< ���R�[�h���
};

