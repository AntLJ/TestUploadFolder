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
#include "BaseCnv.h"
#include <functional>

// ������ʃR�[�h
namespace split_func {
	enum ECode {
		kNormal = 1,
		kParen = 2,
		kHwy = 3,
	};
}

/**
* @brief �������̊i�[�N���X
*/
class SplitElement
{
public:
	SplitElement(const CStringW str, const split_class::ECode& cls, bool isParen)
		:m_str(str), m_class(cls), m_isParen(isParen) {}
	~SplitElement() {}

public:
	CStringW m_str;
	split_class::ECode  m_class;
	bool m_isParen;
};

/**
* @brief ���̕����x�[�X�N���X
*/
class SplitBasicTokenRuleCnv :
	public BaseCnv
{
public:
	SplitBasicTokenRuleCnv(const std::map<CString, std::vector<CString>>& dictionary, const IFeatureClassPtr& ipRoadLink, const ITablePtr& ipRoadNameList, const ITablePtr& ipRoadCodeList, const ITablePtr& ipSubNameTable)
		:BaseCnv(ipRoadLink, ipRoadNameList, ipSubNameTable), m_ipRoadCodeList(ipRoadCodeList), m_dictionary(dictionary){}
	~SplitBasicTokenRuleCnv() {}

protected:
	bool setRoadLinkFieldIndex() override;
	bool setSubTblFieldIndex() override;
	bool loadSubTable() override;
	virtual bool cnvSplitName(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, const RoadNameRecord& roadNameRecord, std::set<SpltNameRecord>& spltRdNameRecs) override;
	
	/**
	 * @brief �����`�F�b�N
	 * @note �ϊ��Ώە����񂪎����ɓo�^����Ă���ꍇ�́A�����ɏ]����������B
	 * @param roadNameRecord [in] �ϊ��Ώۖ��̃f�[�^
	 * @param spltRdNameRecs [out] ���H���̕����f�[�^���R�[�h�ϊ�����
	 * @return true �����ɓo�^���ꂽ�f�[�^�L��
	 * @return false �����ɓo�^���ꂽ�f�[�^����
	 *
	 */
	bool chkDictionary(const RoadNameRecord& roadNameRecord, std::set<SpltNameRecord>& spltRdNameRecs);

	/**
	 * @brief �ϊ��Ώۓ��H�����N�̘H���R�[�h�擾
	 * @note ���H�����N�Ɋ֘A�t���H���R�[�h����擾����B
	 * @param linkOID [in] �ϊ��Ώۃ����NOBJECTID
	 * @param ipLink [in] �ϊ��Ώۃ����N
	 * @param lineCls [out] �H���R�[�h
	 * @retval true ����
	 * @retval fasle ���s
	 */
	bool getLineCls(const long& linkOID, const IFeaturePtr& ipLink, long& lineCls);
	
	/**
	 * @brief Basic Tokenization Rule�Ɋ�Â����H���̕����ϊ�
	 * @note Basic Tokenization Rule�ɂ��ẮA�J�������Q��
	 * @param tgtID [in] ���������̃f�[�^�������R�[�h��OBJECTID
	 * @param tgtTblName [in] ���������̃f�[�^�����e�[�u����
	 * @param nameStr [in] �����Ώۖ���
	 * @param yomiStr [in] �����Ώۓǂ�
	 * @param mainNameSpltElem [out] ���̕�������
	 * @param mainYomiSpltElem [out] �ǂݕ�������
	 */
	void basicTokenRuleCnv(const long& tgtID, 
		const CString& tgtTblName,
		const long& lineCls,
		const CString& nameStr, 
		const CString& yomiStr, 
		std::list<SplitElement>& mainNameSpltElem, 
		std::list<SplitElement>& mainYomiSpltElem);
	/*
	* @brief ���H�����N�Ɋ֘A����H���R�[�h���R�[�h�擾
	* @param linkOID [in] �ϊ��Ώۃ����NOBJECTID
	* @param ipLink [in] �ϊ��Ώۃ����N
	* @param codeOID [out] �֘A����H���R�[�hOBJECTID
	* @param ipCode [out] �֘A����H���R�[�h���R�[�h
	*/
	bool getLinkRelRoadCode(const long& linkOID, const IFeaturePtr& ipLink, long& codeOID, _IRowPtr& ipCode);

private:
	split_class::ECode getSplitClass(const CString& splitStr);

	/**
	 * @brief ���̓ǂݕ����S�̊֐�
	 * @param funcCode [in] �֐��R�[�h(�g�p���镶���񕪊��֐������߂�R�[�h)
	 * @param tgtID [in] ���������̃f�[�^�������R�[�h��OBJECTID
	 * @param tgtTblName [in] ���������̃f�[�^�����e�[�u����
	 * @param spltNameWord [in] ���̗p�����������
	 * @param spltYomiWord [in] �ǂݗp�����������
	 * @param mainNameSpltElem [in,out] ���̕�������(�����Ώە�������{�R���e�i�Ɋi�[����Ă���)
	 * @param mainYomiSpltElem [in,out] �ǂݕ�������(�����Ώە�������{�R���e�i�Ɋi�[����Ă���)
	 */
	void splitNameYomi(const split_func::ECode& funcCode,
		const long& tgtID,
		const CString& tgtTblName,
		const CStringW& spltNameWord,
		const CStringW& spltYomiWord,
		std::list<SplitElement>& mainNameSpltElem,
		std::list<SplitElement>& mainYomiSpltElem);

	void splitNameYomiByRoadNo(const long& tgtID,
		const CString& tgtTblName,
		const CStringW& spltBaseNameWord,
		const CStringW& spltBaseYomiWord,
		std::list<SplitElement>& mainNameSpltElem,
		std::list<SplitElement>& mainYomiSpltElem);
	
	/*
	 * @brief �����񕪊��֐�
	 * @note funcCode�ɍ��킹�āA�e�핶���񕪊��֐����Ăяo�������̊֐�
	 * @param funcCode [in] �֐��R�[�h(�g�p���镶���񕪊��֐������߂�R�[�h)
	 * @param tgtID [in] ���������̃f�[�^�������R�[�h��OBJECTID
	 * @param tgtTblName [in] ���������̃f�[�^�����e�[�u����
	 * @param spltWord [in] �����������
	 * @param spltCls [in] ��������(�����������ɕt�^����鑮��)
	 * @param tgtElem [in] �����Ώە�����(����������������)
	 * @param spltElem [out] ��������
	 */
	void splitStr(const split_func::ECode& funcCode, 
		const long& tgtID,
		const CString& tgtTblName,
		const CStringW& spltWord,
		const SplitElement& tgtElem,
		std::vector<SplitElement>& spltElem);
	
	/*
	 * @brief �m�[�}�������֐�
	 * @param spltWord [in] �����������
	 * @param spltCls [in] ��������(�����������ɕt�^����鑮��)
	 * @param tgtElem [in] �����Ώە�����(����������������)
	 * @param spltElem [out] ��������
	 */
	void splitStrNormal(const CStringW& spltWord, const SplitElement& tgtElem, std::vector<SplitElement>& spltElem);
	
	/*
	 * @brief ()�����񕪊��֐�
	 * @param tgtID [in] ���������̃f�[�^�������R�[�h��OBJECTID
	 * @param tgtTblName [in] ���������̃f�[�^�����e�[�u����
	 * @param tgtElem [in] �����Ώە�����(����������������)
	 * @param spltElem [out] ��������
	 */
	void splitStrByParen(const long& tgtID, const CString& tgtTblName, const SplitElement& tgtElem, std::vector<SplitElement>& spltElem);
	
	/*
	 * @brief ���������񕪊��֐�
	 * @param spltWord [in] �����������
	 * @param tgtElem [in] �����Ώە�����(����������������)
	 * @param spltElem [out] ��������
	 */
	void splitStrByHwy(const CStringW& spltWord, const SplitElement& tgtElem, std::vector<SplitElement>& spltElem);
	
	/*
	 * @brief ���HNo�����񕪊��֐�
	 * @param spltBaseWord [in] �����������
	 * @param tgtElem [in] �����Ώە�����(����������������)
	 * @param spltNo [out] ���ۂɎg�p�������������Q
	 * @param spltElem [out] ��������
	 */
	void splitStrByRoadNo(const CStringW& spltBaseWord, const SplitElement& tgtElem, std::vector<CString>& spltNo, std::vector<SplitElement>& spltElem);

	/*
	 * @brief �������ʂ̃}�[�W�֐�
	 * @param spltElem [in] ���镶����̕�������
	 * @param itr [in] �S�̂̕������ʂ̒��̂��镶����(�����O)���w���C�e���[�^
	 * @param mainSplitElem[in,out] �S�̂̕�������
	 */
	void mergeElementsToMain(const std::vector<SplitElement>& spltElem, std::list<SplitElement>::iterator& itr, std::list<SplitElement>& mainSpltElem);
	
protected:
	ITablePtr m_ipRoadCodeList;						//!< ���H�H�����X�g�e�[�u��
	
	long m_roadLinkRoadCodeIndex;					//!< ���H�����NROAD_CODE�t�B�[���h�C���f�b�N�X
	long m_roadCodeListRoadCodeIndex;				//!< �H���R�[�h���X�gROAD_CODE�t�B�[���h�C���f�b�N�X
	long m_roadCodeListLineClsIndex;				//!< �H���R�[�h���X�gLINECLASS_C�t�B�[���h�C���f�b�N�X
	
	std::map<long, _IRowPtr> m_tgtRdCodeListRecs;	//!< �ϊ��ΏۘH�����X�g���R�[�h �L�[�F�H���R�[�h

	const std::map<CString, std::vector<CString>> m_dictionary; //!< ����ϊ��p����
};

