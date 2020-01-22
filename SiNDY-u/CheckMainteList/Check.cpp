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

#include "StdAfx.h"
#include "Check.h"
#ifdef USE_BOOST_REGEX
#include <boost/regex.hpp>
#else
#include <regex>
#endif
#include "Defines.h"
#define foreach BOOST_FOREACH
namespace{
	const long BUF_SIZE = 256;
	typedef boost::char_separator<char> char_separator;
	typedef boost::tokenizer<char_separator> tokenizer;
	LPCTSTR GS_WEEK[] = {
		_T("��"),
		_T("��"),
		_T("��"),
		_T("��"),
		_T("��"),
		_T("��"),
		_T("�y"),
		_T("�j"),
		NULL
	};
	LPCTSTR NOT_ORIG[] = {
		MAINTE_FIELD_NOT_CHECK1, 
		MAINTE_FIELD_NOT_CHECK2,
		MAINTE_FIELD_NOT_CHECK3,
		MAINTE_FIELD_NOT_CHECK4,
		MAINTE_FIELD_NOT_CHECK5,
		MAINTE_FIELD_NOT_CHECK6,
		NULL
	};

	LPCTSTR NOT_NULL[] = {
		MAINTE_FIELD_VARID_DATE,	//�X�V���L�����t
		MAINTE_FIELD_PREFNAME,		//�s���{������
		MAINTE_FIELD_CITY_NAME,		//�s�撬������
		MAINTE_FIELD_LONG256,		//1/256�o�x
		MAINTE_FIELD_LAT256,		//1/256�ܓx
		MAINTE_FIELD_LONG_DEG,		//�x���b�o�x
		MAINTE_FIELD_LAT_DEG,		//�x���b�ܓx
		MAINTE_FIELD_LONG,			//�o�x
		MAINTE_FIELD_LAT,			//�ܓx
		MAINTE_FIELD_CONTENTS_NAME,	//�R���e���c����
		MAINTE_FIELD_NAME,			//����
		MAINTE_FIELD_YOMI,			//�ǂ�
		MAINTE_FIELD_PRECISION,		//���x�R�[�h
		MAINTE_FIELD_COMPANY,		//���W�l�񋟌��R�[�h
		MAINTE_FIELD_LANDMARK,		//�����h�}�[�N
//		MAINTE_FIELD_TEL,			//�d�b�ԍ�
		MAINTE_FIELD_LIQUOR_FLAG,	//����̔�
		MAINTE_FIELD_CIGAR_FLAG,	//��޺�̔�
		MAINTE_FIELD_TOOLROAD_FLAG,	//�L�����H�t���O
		NULL
	};
}

CCheck::CCheck(void)
: m_OldDir(_T(""))
, m_NewDir(_T(""))
, m_OldFile()
, m_NewFile()
, m_LineNoIndex(-1)
, m_SeqIndex(0)
, m_FuncSeqIndex(0)
, m_NameIndexOrg(-1)
, m_NameIndexNew(-1)
, m_pFile(NULL)
, m_LineNo(_T(""))
, m_Seq(_T(""))
, m_Name(_T(""))
, m_HaveAddr(false)
, m_ParkMode(false)
, m_Mode(0)
{
}

CCheck::~CCheck(void)
{
	m_Head.clear();
	m_OrgData.clear();
	m_NewData.clear();
	m_IDSet.clear();
	if(m_pFile && m_pFile != stderr){
		fclose(m_pFile);
	}
}

bool CCheck::init(LPCTSTR cOldDir, LPCTSTR cNewDir, LPCTSTR cLogFile, int cMode, const CString &cMMaster)
{
	if(cLogFile){
		if((m_pFile = _tfopen(cLogFile, _T("wt"))) == NULL){
			_ftprintf(stderr, _T("ERROR\t���O�t�@�C�����J���܂���B�W���G���[�o�͂��g�p���܂��B\t%s\n"), cLogFile);
			m_pFile = stderr;
		}
	}else{
		m_pFile = stderr;
	}
	if(_taccess(cOldDir, 0x04) == 0){
		m_OldDir = cOldDir;
	}else{
		_ftprintf(m_pFile, _T("ERROR\t�ҏW�O�f�B���N�g���̐ݒ肪����������܂���B\t%s\n"), cOldDir);
		return false;
	}
	if(_taccess(cNewDir, 0x04) == 0){
		m_NewDir = cNewDir;
	}else{
		_ftprintf(m_pFile, _T("ERROR\t�ҏW��f�B���N�g���̐ݒ肪����������܂���B\t%s\n"), cNewDir);
		return false;
	}
	if(cMMaster != _T("")){
		if(!fnLoadMikakuMaster(cMMaster)){
			return false;
		}
	}
	m_CTypeMap[MAINTE_FIELD_NAME] = (int)CheckerType::Name;	//����
	m_CTypeMap[MAINTE_FIELD_YOMI] = (int)CheckerType::Yomi;	//�ǂ�
	m_CTypeMap[MAINTE_FIELD_VARID_DATE] = (int)CheckerType::Date;	//�X�V���L�����t
	m_CTypeMap[MAINTE_FIELD_COMPANY] = (int)CheckerType::Ipcode;	//���W�l�񋟌�
	m_CTypeMap[MAINTE_FIELD_LONG256] = (int)CheckerType::Lon256;	//1/256�o�x
	m_CTypeMap[MAINTE_FIELD_LAT256] = (int)CheckerType::Lat256;	//1/256�ܓx
	m_CTypeMap[MAINTE_FIELD_LONG_DEG] = (int)CheckerType::DmsE;	//�x���b�o�x
	m_CTypeMap[MAINTE_FIELD_LAT_DEG] = (int)CheckerType::DmsN;	//�x���b�ܓx
	m_CTypeMap[MAINTE_FIELD_PRECISION] = (int)CheckerType::Acccode;	//���x
	m_CTypeMap[MAINTE_FIELD_CLOSED_CODE] = (int)CheckerType::Closecode;	//�����R�R�[�h
	m_CTypeMap[MAINTE_FIELD_ADDR] = (int)CheckerType::Address;	//�Z��
	m_CTypeMap[MAINTE_FIELD_ADMIN_MEMO] = (int)CheckerType::MntMemoAdmin;	//�Ǘ��҃���
	m_CTypeMap[MAINTE_FIELD_WORK_MEMO] = (int)CheckerType::MntMemoWork;	//��Ǝ҃���
	m_CTypeMap[MAINTE_FIELD_BG_INFO1] = (int)CheckerType::BgInfo1;	//�Q�l����1
	m_CTypeMap[MAINTE_FIELD_BG_INFO2] = (int)CheckerType::BgInfo2;	//�Q�l����2
	m_CTypeMap[MAINTE_FIELD_URL] = (int)CheckerType::Url;	//URL
	m_CTypeMap[MAINTE_FIELD_USE_ID] = (int)CheckerType::IpUseId;	//���񋟌����pID
	m_CTypeMap[MAINTE_FIELD_TEL] = (int)CheckerType::Tel;	//�d�b�ԍ�
	m_CTypeMap[MAINTE_FIELD_CLOSED_MEMO] = (int)CheckerType::CloseMemo;	//�����R����
//	m_CTypeMap[MAINTE_FIELD_XML] = CheckerType::MntInfo;	//������Ə��iXML�j.
	//	NameYomiPair
	m_Mode = cMode;
	return true;
}

bool CCheck::execCheck(LPCTSTR cFileName)
{
	m_IDSet.clear();
	m_Head.clear();
	m_OrgData.clear();
	m_NewData.clear();
	m_OldFile.close();
	m_NewFile.close();
	m_OldFile.clear();	//�ė��p�ɂ͕K�{�炵��
	m_NewFile.clear();	//�ė��p�ɂ͕K�{�炵��
	_tprintf(_T("Now %s\n"), cFileName);
	if(CString(cFileName).Find(_T("�yPARK�z")) == 0 || CString(cFileName).Find(_T("�yROUTE�z")) == 0){
		m_ParkMode = true;
	}else{
		m_ParkMode = false;
	}
	//�ҏW�O�f�[�^�I�[�v��
	CString aFile;
	aFile.Format(_T("%s\\%s"), static_cast<LPCTSTR>(m_OldDir), cFileName);
	m_OldFile.open(aFile, std::ios::in);
	if( ! m_OldFile.is_open() ){
		_ftprintf(m_pFile, _T("#ERROR\t�t�@�C�����J���܂���B\t%s\n"), static_cast<LPCTSTR>(cFileName));
		return false;
	}
	//�ҏW��f�[�^�I�[�v��
	CString aNewFile;
	aFile.Format(_T("%s\\%s"), static_cast<LPCTSTR>(m_NewDir), cFileName);
	m_NewFile.open(aFile, std::ios::in);
	if( ! m_NewFile.is_open() ){
		m_NewFile.clear();	//�ė��p�ɂ͕K�{�炵��
		CString aSearch;
		CString aTmpFile = cFileName;
		int aIndex = aTmpFile.ReverseFind(_T('.'));
		if(aIndex > 0){
			aSearch.Format(_T("%s\\%s*%s"), static_cast<LPCTSTR>(m_NewDir), aTmpFile.Left(aIndex), aTmpFile.Mid(aIndex));
			WIN32_FIND_DATA fd;
			HANDLE hFind = ::FindFirstFile(aSearch, &fd);
			if(hFind == INVALID_HANDLE_VALUE){//�������s
				_ftprintf(m_pFile, _T("ERROR\t�ҏW��t�@�C�����J���܂���B\t%s\n"), static_cast<LPCTSTR>(aFile));
				return false;
			}else{
				aFile.Format(_T("%s\\%s"), static_cast<LPCTSTR>(m_NewDir), fd.cFileName);
				aNewFile = aFile;
				m_NewFile.open(aFile, std::ios::in);
				if( ! m_NewFile.is_open() ){
					_ftprintf(m_pFile, _T("ERROR\t�ҏW��t�@�C�����J���܂���B\t%s\n"), static_cast<LPCTSTR>(aFile));
					return false;
				}
			}
		}else{
			_ftprintf(m_pFile, _T("ERROR\t�ҏW��t�@�C�����J���܂���B\t%s\n"), static_cast<LPCTSTR>(aFile));
			return false;
		}
	}
	//�w�b�_�̕��є�r
	if(fnHeadComp()){
		//�`�F�b�N�ɕK�v�ȃt�B�[���h�̗L���m�F
		fnExistFieldCheck();
		//�f�[�^�̔�r
		fnDataComp();
		//�]��f�[�^�E�E�E�V�K�̂͂��H
		fnNewDataCheck();
	}
	return true;
}

bool CCheck::fnHeadComp(void)
{
	m_SeqIndex = m_FuncSeqIndex = -1;
	m_FieldMap.clear();
	std::string aLine;
	if( getline( m_OldFile, aLine ) ){
		if(fnAnalyzeHeadrer(aLine, m_Head)){
			if( getline( m_NewFile, aLine ) ){
				HEADER aTgtHead;
				if(fnAnalyzeHeadrer(aLine, aTgtHead)){
					if(m_Head == aTgtHead){
						if(m_SeqIndex >= 0){
							return true;
						}else{
							_ftprintf(m_pFile, _T("ERROR\t�u�V�[�P���X�v���Ȃ����ߔ�r�ł��܂���B\n"));
							return false;
						}
					}
				}
			}else{
				_ftprintf(m_pFile, _T("ERROR\t�ǂݍ��݃G���[\n"));
				return false;
			}
		}
	}
	_ftprintf(m_pFile, _T("ERROR\t�w�b�_���Ⴄ����r�ł��܂���B\n"));
	return false;
}

bool CCheck::fnAnalyzeHeadrer(const std::string cData, HEADER& cHead)
{
	char_separator a_csSep("\t", "", boost::keep_empty_tokens); // �����ݒ�
	tokenizer	a_tTokens(cData, a_csSep);	// ����
	int i = 0;
	foreach( const std::string& aStr, std::make_pair( a_tTokens.begin(), a_tTokens.end()) ){
		CString aTmp(aStr.c_str());
		m_Seq = _T("�w�b�_");
		aTmp = fnCutDoubleQuotation(aTmp, _T("�w�b�_"));
		m_FieldMap[aTmp] = i;
		if(m_Mode == 0){
			if(aTmp == MAINTE_FIELD_SEQUENCE){
				m_SeqIndex = i;
			}else if(aTmp == MAINTE_FIELD_FUNC_SEQ){
				m_FuncSeqIndex = i;
			}else if(aTmp == MAINTE_FIELD_BANK_SEQUENCE){
				m_SeqIndex = i;
			}
		}else if(m_Mode == 1){
			//GS�c�Ǝ���
			if(aTmp == MAINTE_FIELD_GS_SEQUENCE){
				m_SeqIndex = i;
			}
		}else{
			_ftprintf(m_pFile, _T("ERROR\t�ݒ肳��Ă��Ȃ����[�h�ł��B\n"));
		}
		if(aTmp == MAINTE_FIELD_LINE_NO)	m_LineNoIndex = i;
		if(aTmp == MAINTE_FIELD_NAME)		m_NameIndexOrg = i;
		if(aTmp == CString(MAINTE_FIELD_NAME) + CString(MAINTE_FIELD_MODIFY))	m_NameIndexNew = i;
		cHead.push_back(aTmp);
		i++;
	}
	return true;
}

bool CCheck::fnDataComp(void)
{
	std::string aLine;
	while( getline( m_OldFile, aLine ) ){
		fnLoadOldData(aLine);
	}
	while( getline( m_NewFile, aLine ) ){
		fnLoadNewData(aLine);
	}
	return fnCompare();
}

bool CCheck::fnLoadOldData(std::string cData)
{
	char_separator a_csSep("\t", "", boost::keep_empty_tokens); // �����ݒ�
	tokenizer	a_tTokens(cData, a_csSep);	// ����
	RECORD aRec;
	foreach( const std::string& aStr, std::make_pair( a_tTokens.begin(), a_tTokens.end()) ){
		CString aTmp(aStr.c_str());
		aRec.push_back(aTmp);
	}
	if(m_Head.size() != aRec.size()){
		if(m_FuncSeqIndex < 0){
			m_Seq = aRec[m_SeqIndex];
		}else{
			m_Seq = aRec[m_SeqIndex] + _T("_") + aRec[m_FuncSeqIndex];
		}
		if(m_LineNoIndex  >= 0) m_LineNo	= aRec[m_LineNoIndex];
		if(m_NameIndexOrg >= 0) m_Name		= aRec[m_NameIndexOrg];
		fnOutputErr(kError, _T("�ҏW�O�f�[�^�̃t�B�[���h������܂���B"));
		while(m_Head.size() > aRec.size()){
			//�s������NULL�ŕ₤
			aRec.push_back(_T(""));
		}
	}
	m_OrgData.push_back(aRec);
	return true;
}

bool CCheck::fnLoadNewData(std::string cData)
{
	char_separator a_csSep("\t", "", boost::keep_empty_tokens); // �����ݒ�
	tokenizer	a_tTokens(cData, a_csSep);	// ����
	RECORD aRec;
	foreach( const std::string& aStr, std::make_pair( a_tTokens.begin(), a_tTokens.end()) ){
		CString aTmp(aStr.c_str());
		aRec.push_back(aTmp);
	}
	if(m_FuncSeqIndex < 0){
		m_Seq = aRec[m_SeqIndex];
	}else{
		m_Seq = aRec[m_SeqIndex] + _T("_") + aRec[m_FuncSeqIndex];
	}
	if(m_LineNoIndex  >= 0) m_LineNo	= aRec[m_LineNoIndex];
	if(m_NameIndexOrg >= 0) m_Name		= aRec[m_NameIndexOrg];
	if(m_Head.size() != aRec.size()){
		fnOutputErr(kWarning, _T("�ҏW��f�[�^�̃t�B�[���h������܂���B"));
		while(m_Head.size() > aRec.size()){
			//�s������NULL�ŕ₤
			aRec.push_back(_T(""));
		}
	}
	m_NewData.insert(COMP_LIST::value_type(m_Seq, aRec));
	return true;
}

bool CCheck::fnCompare(void)
{
	for(DATA::iterator aOrg = m_OrgData.begin(); aOrg != m_OrgData.end(); aOrg++){
		if(m_FuncSeqIndex < 0){
			m_Seq = (*aOrg)[m_SeqIndex];
		}else{
			m_Seq = (*aOrg)[m_SeqIndex] + _T("_") + (*aOrg)[m_FuncSeqIndex];
		}
		if(m_LineNoIndex  >= 0) m_LineNo	= (*aOrg)[m_LineNoIndex];
		if(m_NameIndexOrg >= 0) m_Name		= (*aOrg)[m_NameIndexOrg];
		COMP_LIST::iterator aNew = m_NewData.find(m_Seq);
		if(aNew != m_NewData.end()){
			if(m_Mode == 0){
				fnCompareRecord(*aOrg, aNew->second);
			}else if(m_Mode == 1){
				fnCompareRecordGS(*aOrg, aNew->second);
			}else{
				_ftprintf(m_pFile, _T("ERROR\t�ݒ肳��Ă��Ȃ��`�F�b�N���[�h�ł��B\t%d\n"), m_Mode);
				return false;
			}
			m_NewData.erase(aNew);
		}else{
			fnOutputErr(kError, _T("�f�[�^���폜����܂����B"));
		}
	}
	return true;
}

bool CCheck::fnCompareRecord(const RECORD& cOrgData, const RECORD& cNewData)
{
/*	int aSize;
	if(cOrgData.size() > cNewData.size()){
		aSize = cNewData.size();
	}else{
		aSize = cOrgData.size();
	}*/
	/*���j�C���l��茳�l����ɗ��邱�Ƃ��O��̍��ɂȂ��Ă���*/
	bool aChgCity = false;
	bool aChgXY = false;		//���W�ύX�̗L��
	bool aHavePrecision = false;	//�u���x�v�t�B�[���h�̗L��
	bool aHaveCompany = false;		//�u���W�l�񋟌��v�t�B�[���h�̗L��
	bool aChgPrecision = false;	//�u���x�v�t�B�[���h�ύX�̗L��
	bool aChgCompany = false;	//�u���W�l�񋟌��v�t�B�[���h�ύX�̗L��
	bool aQCCode = false;		//�i�ؕۏ؃f�[�^�i���W�l�񋟌�=62�j
	CString aPrecision = _T(""), aCompany = _T("");
	CString aAdd(_T(""));

	//��sATM�p�ɒǉ�
	bool aCheckBank = false;
	DELETE_INFO aDelInfo[2];
	long aOrgForm = -1;
	long aNewForm = -1;
	CString aParking = _T("");
	CString aBisTime = _T("");
	CString aBisHoliday = _T("");
	//��sATM�p�ɒǉ��@�����܂�

	int i = 0;
	for(HEADER::iterator aHead = m_Head.begin(); /*i < aSize && */aHead != m_Head.end(); aHead++, i++){
		if(aHead->Find(MAINTE_FIELD_MODIFY) < 0){
			if(!fnIsNotOrig(*aHead)){
				//���l
				if(cOrgData[i] != cNewData[i] && cOrgData[i] != fnCompRegulate(*aHead, cNewData[i])){
					aAdd.Format(_T("%s\t%s\t%s"), static_cast<LPCTSTR>(*aHead), static_cast<LPCTSTR>(cOrgData[i]), static_cast<LPCTSTR>(cNewData[i]));
					fnOutputErr(kError, _T("���l���C������Ă��܂��B"));
				}
				if(*aHead == MAINTE_FIELD_INFOFILE_ID){
					fnInfoFileCheck(cNewData[i]);
				}
				if(cOrgData[i] != _T("")){
					fnFieldCheck(*aHead, cOrgData[i], cOrgData);
				}
				//�����`�F�b�N�̏���
				if((*aHead) == MAINTE_FIELD_PRECISION){
					aPrecision = cOrgData[i];
				}else if((*aHead) == MAINTE_FIELD_COMPANY){
					aCompany = cOrgData[i];
					if(aCompany == _T("62")){
						aQCCode = true;
					}
				}else if((*aHead) == MAINTE_FIELD_FORM){
					//�X�܌`�Ԏ��
					aOrgForm = _ttol(cOrgData[i]);
					aCheckBank = true;
				}
				//�����`�F�b�N�̏����@�����܂�
			}
		}else{
			//�C���l
			if(cNewData[i] != _T("")){
				fnFieldCheck(*aHead, cNewData[i], cNewData);
				//�����`�F�b�N�̏���
				if((*aHead).Find(MAINTE_FIELD_PREFNAME) == 0 || (*aHead).Find(MAINTE_FIELD_CITY_NAME) == 0){
					aChgCity = true;
				}else if((*aHead).Find(MAINTE_FIELD_LONG) >= 0 || (*aHead).Find(MAINTE_FIELD_LAT) >= 0){
					//�u�ܓx�v���́u�o�x�v���܂܂�Ă���΂������Ƃ�
					//���l�������Ă��邱�Ƃ����邩��
					if(cOrgData[i] != cNewData[i])
						aChgXY = true;
				}
				//�����`�F�b�N�̏����@�����܂�
			}
			//�����`�F�b�N�̏���
			if((*aHead).Find(MAINTE_FIELD_PRECISION) == 0){
				aHavePrecision = true;
				if(cNewData[i] != _T("")){
					aPrecision = cNewData[i];
					aChgPrecision = true;
				}
			}else if((*aHead).Find(MAINTE_FIELD_COMPANY) == 0){
				aHaveCompany = true;
				if(cNewData[i] != _T("")){
					aCompany = cNewData[i];
					aChgCompany = true;
					if(aQCCode && cNewData[i] != _T("62")){
						aAdd.Format(_T("%s\t%s\t%s"), static_cast<LPCTSTR>(*aHead), static_cast<LPCTSTR>(cOrgData[i]), static_cast<LPCTSTR>(cNewData[i]));
						fnOutputErr(kError, _T("�i�؃f�[�^�ɑ΂�����W�l�񋟌��R�[�h�C���ł��B"), aAdd);
					}
				}
			}else if((*aHead).Find(MAINTE_FIELD_FORM) == 0){
				//�X�܌`�Ԏ��
				aNewForm = _ttol(cNewData[i]);
			}else if((*aHead).Find(MAINTE_FIELD_WINDOW) == 0){
				//����_
				fnSetDeleteInfo(aDelInfo[0], (*aHead), cNewData[i]);
			}else if((*aHead).Find(MAINTE_FIELD_ATM) == 0){
				//ATM_
				fnSetDeleteInfo(aDelInfo[1], (*aHead), cNewData[i]);
			}else if((*aHead).Find(MAINTE_FIELD_BANK_PARKING) == 0){
				//���ԏ�
				aParking = cNewData[i];
			}else if((*aHead).Find(MAINTE_FIELD_BANK_BIS_TIME) == 0){
				//�c�Ǝ���
				aBisTime = cNewData[i];
			}else if((*aHead).Find(MAINTE_FIELD_BANK_BIS_HOLIDAY) == 0){
				//�x�Ɠ�
				aBisHoliday = cNewData[i];
			}
			//�����`�F�b�N�̏����@�����܂�
		}
	}

	//�e�퐮���`�F�b�N
	if(aChgXY)
	{
		if(aHavePrecision && !aChgPrecision){
			fnOutputErr(kError, _T("���W�l�C���l�ɓ��͂�����܂����A��q�̃t�B�[���h�ɓ��͂�����܂���B"), MAINTE_FIELD_PRECISION);
		}
		if(aHaveCompany && !aChgCompany){
			fnOutputErr(kError, _T("���W�l�C���l�ɓ��͂�����܂����A��q�̃t�B�[���h�ɓ��͂�����܂���B"), MAINTE_FIELD_COMPANY);
		}
		if(aQCCode){
			fnOutputErr(kError, _T("�i�؃f�[�^�ɑ΂�����W�C���ł��B"));
		}
	}else{
		if(aChgCity){
			fnOutputErr(kError, _T("�s���{��/�s�撬�����C������Ă��܂����A���W�l�ɏC���l������܂���B"));
		}
	}
	if(aHavePrecision && aHaveCompany){
		if((aCompany == _T("62") && aPrecision != _T("UK"))
			|| (aCompany == _T("00") && aPrecision == _T("UK"))
			|| (aCompany == _T("04") && aPrecision == _T("UK"))){
			aAdd.Format(_T("%s\t%s"), static_cast<LPCTSTR>(aCompany), static_cast<LPCTSTR>(aPrecision));
			fnOutputErr(kError, _T("���x�Ƃ̑g�ݍ��킹���s���ł��B"), aAdd);
		}
	}
	//��sATM����
	if(aCheckBank){
		if(aNewForm == 1){
			//�����̂�
			if(aOrgForm == 3 || aOrgForm == 2){
				//ATM�폜
				if(aDelInfo[1].m_DeleteCode != 1){
					aAdd.Format(_T("%d->%d\t%d"), aOrgForm, aNewForm, aDelInfo[1].m_DeleteCode);
					fnOutputErr(kError, _T("�X�܌`�Ԏ�ʂƍ폜�t���O�̑g�ݍ��킹���s���ł��B"), aAdd);
				}
				if(aDelInfo[1].m_ClosedCode == _T("")){
					aAdd.Format(_T("%d->%d\t%s"), aOrgForm, aNewForm, static_cast<LPCTSTR>(aDelInfo[1].m_ClosedCode));
					fnOutputErr(kError, _T("�X�܌`�Ԏ�ʂƕ����R�R�[�h�̑g�ݍ��킹���s���ł��B"), aAdd);
				}
				if(aDelInfo[1].m_ClosedMemo == _T("")){
					aAdd.Format(_T("%d->%d\t%s"), aOrgForm, aNewForm, static_cast<LPCTSTR>(aDelInfo[1].m_ClosedMemo));
					fnOutputErr(kError, _T("�X�܌`�Ԏ�ʂƕ����t�E���R�̑g�ݍ��킹���s���ł��B"), aAdd);
				}
			}
			if(aParking != _T("����") && aParking != _T("�Ȃ�") && aParking != _T("��")){
				aAdd.Format(_T("%d\t%s"), aNewForm, static_cast<LPCTSTR>(aParking));
				fnOutputErr(kError, _T("�X�܌`�Ԏ�ʂƒ��ԏ�̑g�ݍ��킹���s���ł��B"), aAdd);
			}
			if(aBisTime != _T("") && aBisTime != _T("��")){
				aAdd.Format(_T("%d\t%s"), aNewForm, static_cast<LPCTSTR>(aBisTime));
				fnOutputErr(kError, _T("�X�܌`�Ԏ�ʂƉc�Ǝ��Ԃ̑g�ݍ��킹���s���ł��B"), aAdd);
			}
			if(aBisHoliday != _T("") && aBisHoliday != _T("��")){
				aAdd.Format(_T("%d\t%s"), aNewForm, static_cast<LPCTSTR>(aBisHoliday));
				fnOutputErr(kError, _T("�X�܌`�Ԏ�ʂƋx�Ɠ��̑g�ݍ��킹���s���ł��B"), aAdd);
			}
		}else if(aNewForm == 2){
			//ATM�̂�
			if(aOrgForm == 3 || aOrgForm == 1){
				//�����폜
				if(aDelInfo[0].m_DeleteCode != 1){
					aAdd.Format(_T("%d->%d\t%d"), aOrgForm, aNewForm, aDelInfo[0].m_DeleteCode);
					fnOutputErr(kError, _T("�X�܌`�Ԏ�ʂƍ폜�t���O�̑g�ݍ��킹���s���ł��B"), aAdd);
				}
				if(aDelInfo[0].m_ClosedCode == _T("")){
					aAdd.Format(_T("%d->%d\t%s"), aOrgForm, aNewForm, static_cast<LPCTSTR>(aDelInfo[0].m_ClosedCode));
					fnOutputErr(kError, _T("�X�܌`�Ԏ�ʂƕ����R�R�[�h�̑g�ݍ��킹���s���ł��B"), aAdd);
				}
				if(aDelInfo[0].m_ClosedMemo == _T("")){
					aAdd.Format(_T("%d->%d\t%s"), aOrgForm, aNewForm, static_cast<LPCTSTR>(aDelInfo[0].m_ClosedMemo));
					fnOutputErr(kError, _T("�X�܌`�Ԏ�ʂƕ����t�E���R�̑g�ݍ��킹���s���ł��B"), aAdd);
				}
			}
			if(aParking != _T("") && aParking != _T("��")){
				aAdd.Format(_T("%d\t%s"), aNewForm, static_cast<LPCTSTR>(aParking));
				fnOutputErr(kError, _T("�X�܌`�Ԏ�ʂƒ��ԏ�̑g�ݍ��킹���s���ł��B"), aAdd);
			}
			if(aBisTime == _T("")){
				aAdd.Format(_T("%d\t%s"), aNewForm, static_cast<LPCTSTR>(aBisTime));
				fnOutputErr(kError, _T("�X�܌`�Ԏ�ʂƉc�Ǝ��Ԃ̑g�ݍ��킹���s���ł��B"), aAdd);
			}
			if(aBisHoliday == _T("")){
				aAdd.Format(_T("%d\t%s"), aNewForm, static_cast<LPCTSTR>(aBisHoliday));
				fnOutputErr(kError, _T("�X�܌`�Ԏ�ʂƋx�Ɠ��̑g�ݍ��킹���s���ł��B"), aAdd);
			}
		}else if(aNewForm == 3){
			//��������
			if(aParking == _T("")){
				aAdd.Format(_T("%d\t%s"), aNewForm, static_cast<LPCTSTR>(aParking));
				fnOutputErr(kError, _T("�X�܌`�Ԏ�ʂƒ��ԏ�̑g�ݍ��킹���s���ł��B"), aAdd);
			}
			if(aBisTime == _T("")){
				aAdd.Format(_T("%d\t%s"), aNewForm, static_cast<LPCTSTR>(aBisTime));
				fnOutputErr(kError, _T("�X�܌`�Ԏ�ʂƉc�Ǝ��Ԃ̑g�ݍ��킹���s���ł��B"), aAdd);
			}
			if(aBisHoliday == _T("")){
				aAdd.Format(_T("%d\t%s"), aNewForm, static_cast<LPCTSTR>(aBisHoliday));
				fnOutputErr(kError, _T("�X�܌`�Ԏ�ʂƋx�Ɠ��̑g�ݍ��킹���s���ł��B"), aAdd);
			}
		}
	}
	return true;
}

bool CCheck::fnFieldCheck(LPCTSTR cField, CString cData, const RECORD& cNewData)
{
	CString aTmpField = cField;
	aTmpField.Replace(MAINTE_FIELD_MODIFY, _T(""));
	CString aData = fnRegulate(aTmpField, cData);
	CString aAdd(_T(""));

	CHECK_TYPE_MAP::iterator aCType =m_CTypeMap.find(aTmpField);
	if(aCType != m_CTypeMap.end()){
		IChecker<CheckContainer^>^ iChecker = CheckerFactory::GetSingleUnitChecker((CheckerType)aCType->second);  // �����񌟍��N���X
		if( iChecker )
		{
			CheckContainer^ cc;
			bool aDoYomiName = false;
			switch( aCType->second ){
				case CheckerType::Address:
				{
					if(m_HaveAddr){
						// �s���{���s�撬�����擾
						CString strPrefName;
						if(cNewData[m_FieldMap[CString(MAINTE_FIELD_PREFNAME)+CString(MAINTE_FIELD_MODIFY)]] != _T("")){
							strPrefName = cNewData[m_FieldMap[CString(MAINTE_FIELD_PREFNAME)+CString(MAINTE_FIELD_MODIFY)]];
						}else{
							strPrefName = cNewData[m_FieldMap[MAINTE_FIELD_PREFNAME]];
						}
						CString strCityName;
						if(cNewData[m_FieldMap[CString(MAINTE_FIELD_CITY_NAME)+CString(MAINTE_FIELD_MODIFY)]] != _T("")){
							strCityName = cNewData[m_FieldMap[CString(MAINTE_FIELD_CITY_NAME)+CString(MAINTE_FIELD_MODIFY)]];
						}else{
							strCityName = cNewData[m_FieldMap[MAINTE_FIELD_CITY_NAME]];
						}
						// [CHECK] �s���{���R�[�h�Ǝs�撬���R�[�h�̓Z�b�g�ł���K�v������
						if( strPrefName.IsEmpty() || strCityName.IsEmpty() ){
//							cErrs.push_back( CSPSkErrorInfo(  new CSkErrorInfo( _T(""), _T(""), _T("������"), sindyErr_CheckReturnErr, sindyErrLevel_ERR, _T("%s"), _T("�s���{���R�[�h�Ǝs�撬���R�[�h�̓Z�b�g�ł���K�v������܂�") ) ) );
							aAdd.Format(_T("%s\t%s"), cField, static_cast<LPCTSTR>(cData));
							fnOutputErr(kError, _T("�s���{�����͎s�撬����NULL"), aAdd);
						}else {
							cc = gcnew AddressCheckContainer();
							safe_cast<AddressCheckContainer^>(cc)->Address = gcnew System::String(aData);
							safe_cast<AddressCheckContainer^>(cc)->RefPrefname = gcnew System::String(strPrefName);
							safe_cast<AddressCheckContainer^>(cc)->RefCityname = gcnew System::String(strCityName);
						}
					}
					break;
				}
/*				case CheckerType::NameYomiPair:
				{
					cc = gcnew NameYomiCheckContainer();
					safe_cast<NameYomiCheckContainer^>(cc)->Name = strCLIValue;
					// �ǂݕ����͒��ڎ擾
					CString strYomi( GetValue( poi_point::kYomi, _T("") ) );
					strYomi.Remove(_T('�b'));
					safe_cast<NameYomiCheckContainer^>(cc)->Yomi = gcnew System::String(strYomi);
					break;
				}*/
				case CheckerType::Name:	//����
				case CheckerType::Yomi:	//�ǂ�
					aDoYomiName = true;
					//�X�̃`�F�b�N�����邩��Abreak���Ȃ��B
				default:
					cc = gcnew StringCheckContainer();
					safe_cast<StringCheckContainer^>(cc)->Value = gcnew System::String(aData);
					break;
			}
			if( cc )
			{
				ErrorInfoList^ errList = iChecker->Check(cc); // �`�F�b�N
				if( 0 < errList->Count )
				{
					for each( ErrorInfo^ info in errList )
					{
						pin_ptr<const wchar_t> strType = PtrToStringChars( info->ErrorType.ToString() );
						pin_ptr<const wchar_t> strErrID = PtrToStringChars( info->ErrorId );
						pin_ptr<const wchar_t> strErrMsg = PtrToStringChars( info->Message );
//						pin_ptr<const wchar_t> strCheckType = PtrToStringChars( aCType->second.ToString() );
						bool bIsWarning = ( 0 <= CString(strType).Find(_T("WAR")) );
						aAdd.Format(_T("%s\t%s"), cField, static_cast<LPCTSTR>(cData));
						fnOutputErr(bIsWarning ? kWarning : kError, CString(strErrMsg), aAdd);
//						cErrs.push_back( CSPSkErrorInfo(  new CSkErrorInfo( CString(strType), CString(strErrID), CString(strCheckType), bIsWarning ? sindyErr_CheckReturnWarning : sindyErr_CheckReturnErr, bIsWarning ? sindyErrLevel_WARN : sindyErrLevel_ERR, _T("%s�i�����F%s�j"), CString(strErrMsg), cFieldAttr.alias ) ) );
					}
				}
			}
			if(aDoYomiName){
				iChecker = CheckerFactory::GetSingleUnitChecker(CheckerType::NameYomiPair);	//�K�v�H
				if( iChecker ){
					cc = gcnew NameYomiCheckContainer();
					CString strName;
					if(cNewData[m_FieldMap[CString(MAINTE_FIELD_NAME)+CString(MAINTE_FIELD_MODIFY)]] != _T("")){
						strName = cNewData[m_FieldMap[CString(MAINTE_FIELD_NAME)+CString(MAINTE_FIELD_MODIFY)]];
					}else{
						strName = cNewData[m_FieldMap[MAINTE_FIELD_NAME]];
					}
					safe_cast<NameYomiCheckContainer^>(cc)->Name = gcnew System::String(strName);
					CString strYomi;
					if(cNewData[m_FieldMap[CString(MAINTE_FIELD_YOMI)+CString(MAINTE_FIELD_MODIFY)]] != _T("")){
						strYomi = cNewData[m_FieldMap[CString(MAINTE_FIELD_YOMI)+CString(MAINTE_FIELD_MODIFY)]];
					}else{
						strYomi = cNewData[m_FieldMap[MAINTE_FIELD_YOMI]];
					}
					safe_cast<NameYomiCheckContainer^>(cc)->Yomi = gcnew System::String(strYomi);
					if( cc )
					{
						ErrorInfoList^ errList = iChecker->Check(cc); // �`�F�b�N
						if( 0 < errList->Count )
						{
							for each( ErrorInfo^ info in errList )
							{
								pin_ptr<const wchar_t> strType = PtrToStringChars( info->ErrorType.ToString() );
								pin_ptr<const wchar_t> strErrID = PtrToStringChars( info->ErrorId );
								pin_ptr<const wchar_t> strErrMsg = PtrToStringChars( info->Message );
//								pin_ptr<const wchar_t> strCheckType = PtrToStringChars( aCType->second.ToString() );
								bool bIsWarning = ( 0 <= CString(strType).Find(_T("WAR")) );
								aAdd.Format(_T("%s\t%s"), cField, static_cast<LPCTSTR>(cData));
								fnOutputErr(bIsWarning ? kWarning : kError, CString(strErrMsg), aAdd);
		//						cErrs.push_back( CSPSkErrorInfo(  new CSkErrorInfo( CString(strType), CString(strErrID), CString(strCheckType), bIsWarning ? sindyErr_CheckReturnWarning : sindyErr_CheckReturnErr, bIsWarning ? sindyErrLevel_WARN : sindyErrLevel_ERR, _T("%s�i�����F%s�j"), CString(strErrMsg), cFieldAttr.alias ) ) );
							}
						}
					}
				}
			}
		}

	}else{
		if(aTmpField == MAINTE_FIELD_DELETE_CODE || aTmpField == MAINTE_FIELD_ROOT_DELCODE){
			if(aData != _T("") && aData != _T("1") && aData != _T("2") && aData != _T("�~")){
				aAdd.Format(_T("%s\t%s"), cField, static_cast<LPCTSTR>(cData));
				fnOutputErr(kError, _T("�폜�R�[�h���s���ł��B"), aAdd);
			}
		}else if(aTmpField == MAINTE_FIELD_PIC && m_Mikaku.size() > 0){
			if(aData != _T("��") && aData != _T("�~")){
				aData = fnZen2Han(aData);
				aData = fnCutMultiSpace(aData);
				int aStart = 0;
				int aIndex = aData.Find(_T("�"));	//���p�ɕϊ�����邩��
				CString aCmp;
				while(aIndex > 0){
					aCmp = aData.Mid(aStart, aIndex - aStart);
					if(m_Mikaku.find(aCmp) == m_Mikaku.end()){
						aAdd.Format(_T("%s\t%s"), cField, static_cast<LPCTSTR>(aCmp));
						fnOutputErr(kError, _T("���X�g�ɂȂ����o���f�[�^�ł��B"), aAdd);
					}
					aStart = aIndex+1;
					aIndex = aData.Find(_T("�"), aStart);	//���p�ɕϊ�����邩��
				}
				aCmp = aData.Mid(aStart);
				if(m_Mikaku.find(aCmp) == m_Mikaku.end()){
					aAdd.Format(_T("%s\t%s"), cField, static_cast<LPCTSTR>(aCmp));
					fnOutputErr(kError, _T("���X�g�ɂȂ����o���f�[�^�ł��B"), aAdd);
				}
			}
		}else if(aTmpField.Find(MAINTE_FIELD_LONG) >= 0 || aTmpField.Find(MAINTE_FIELD_LAT) >= 0 ){
			// [Bug8763]���W�����`�F�b�N�ǉ�
			if( !fnCoordCheck(aTmpField, aData) ) {
				aAdd.Format(_T("%s\t%s"), cField, static_cast<LPCTSTR>(cData));
				fnOutputErr(kError, _T("���W�t�H�[�}�b�g���s���ł��B"), aAdd);
			}
		}
	}
	if(aData.Find(_T("\"")) >= 0){
		aAdd.Format(_T("%s\t%s"), cField, static_cast<LPCTSTR>(cData));
		fnOutputErr(kError, _T("�u\"�v���܂܂�Ă��܂��B"), aAdd);
	}
	return true;
}

bool CCheck::fnInfoFileCheck(CString cData)
{
	//�E�f�B���N�g�����̌�����2�`3���ł��邱��
	//�E�t�@�C�����̌����̓f�B���N�g�����̌����Ɠ���ł��邱��
	//�E�t�@�C�����͓���t�H���_���Ń��j�[�N�ł��邱�Ɓi�R���e���c���ł͂Ȃ����Ƃɒ��Ӂj
	//�E���l�Ɓuf�v�݂̂ō\������Ă��邱�Ɓif�͈�����j
/*
	�E�f�B���N�g�����̌�����2�`3���ł��邱��
	�E�t�@�C�����i�g���q�Ȃ��j�̌����́A�u7 - �f�B���N�g�����̌����v�ł��邱��
	�E�V�K�t�@�C�����i�g���q�Ȃ��j�̌����́A�u7 - �f�B���N�g�����̌����v�ƈ�v���Ă��邱��
	�E�u7 - �f�B���N�g�����̌����v�ɁA�t�@�C�����i�g���q�Ȃ��j�̌����������Ȃ���0�l�߂���Ă��邱��
	�E�f�B���N�g�����ƃt�@�C�����i�g���q�Ȃ��j�̊ԂɁuf�v�����݂��邱��
	�E����POI�ɕR�t�����t�@�C��ID��1�ł��邱��
*/
	if(cData != _T("")){
		if(cData.GetLength() == 8){
			std::string aData = CT2CA(cData).m_psz;
#ifdef USE_BOOST_REGEX
			if(!boost::regex_match(aData, boost::regex("[0-9]{2}f[0-9]{5}")) && !boost::regex_match(aData, boost::regex("[0-9]{3}f[0-9]{4}"))){
#else
			if(!std::tr1::regex_match(aData, std::tr1::regex("[0-9]{2}f[0-9]{5}")) && !std::tr1::regex_match(aData, std::tr1::regex("[0-9]{3}f[0-9]{4}"))){
#endif
				fnOutputErr(kError, _T("���t�@�C��ID�̃t�H�[�}�b�g���s���ł��B"), cData);
			}
			std::pair<ID_SET::iterator, bool> aRet = m_IDSet.insert(cData);
			if(!aRet.second){
				fnOutputErr(kError, _T("���t�@�C��ID�����j�[�N�ł͂���܂���B"), cData);
			}
		}else{
			fnOutputErr(kError, _T("���t�@�C��ID�̌���8���ł͂���܂���B"), cData);
		}
	}
	return true;
}

CString CCheck::fnRegulate(const CString& cField, const CString& cData)
{

	CString aResult = fnCutDoubleQuotation(cData, cField);
	aResult = fnCutSpace(aResult);
	if(cField == MAINTE_FIELD_NAME || cField == MAINTE_FIELD_YOMI || cField == MAINTE_FIELD_ADDR)
	{
		// [Bug8762]���K���O�ɖ��́E�����ڈȉ��Z���ɂ��Ĕ��p��d���p���`�F�b�N�����{(�ǂ݂�����Ė��Ȃ��̂Ŏ��{)
		if(aResult.Find(_T("\"")) >= 0){
			CString aAdd;
			aAdd.Format(_T("%s\t%s"), cField, static_cast<LPCTSTR>(cData));
			fnOutputErr(kError, _T("�u\"�v���܂܂�Ă��܂��B"), aAdd);
		}

		//���́E�ǂ�
		aResult = fnHan2Zen(aResult);
		aResult = fnCutMultiSpace(aResult);
		if(cField == MAINTE_FIELD_YOMI){
			aResult = fnConvYomiSP(aResult);
		}
	}else if(cField == MAINTE_FIELD_TEL || cField == MAINTE_FIELD_LONG256 || cField == MAINTE_FIELD_LAT256 || cField == MAINTE_FIELD_LONG_DEG || cField == MAINTE_FIELD_LAT_DEG
		|| cField == MAINTE_FIELD_ROOT_LONG256 || cField == MAINTE_FIELD_ROOT_LAT256){
		//�d�b�ԍ��E1/256�o�x�E1/256�ܓx�E�x���b�o�x�E�x���b�ܓx, ���[�g�T���p�|�C���g.1/256�o�x, ���[�g�T���p�|�C���g.1/256�ܓx
		aResult = fnZen2Han(aResult);
	}else if(cField == MAINTE_FIELD_CLOSED_MEMO || cField == MAINTE_FIELD_WORK_MEMO || cField == MAINTE_FIELD_ADMIN_MEMO || cField == MAINTE_FIELD_BG_INFO1 || cField == MAINTE_FIELD_BG_INFO2 || cField == MAINTE_FIELD_URL){
		//�����R�����E��Ǝ҃����E�Ǘ��҃����E�Q�l����1�E�Q�l����2�EURL
		aResult = fnCutMultiSpace(aResult);
	}else if(cField.Find(MAINTE_FIELD_INTRO) == 0){
		//�Љ.xx
//		aResult = fnZen2Han(aResult);
//		aResult = fnCutMultiSpace(aResult);
	}else if(cField.Find(MAINTE_FIELD_XML) == 0){
		//������Ə��iXML�j.xx
	}
	return aResult;
}

CString CCheck::fnCutSpace(CString cData)
{
//	cData = _T(" �@  �@�@ �e�X�g  �@�@�@ ");
//	cData = _T(" �@  �@�@ ��  �@�@�@ ");
	std::wstring aResult = cData;
#ifdef USE_BOOST_REGEX
	//�擪�̋󔒍폜
	aResult = boost::regex_replace(aResult, boost::wregex(L"^[ �@]+"), L"");
	//�����̂̋󔒍폜
	aResult = boost::regex_replace(aResult, boost::wregex(L"[ �@]+$"), L"");
#else
	//�擪�̋󔒍폜
	aResult = std::tr1::regex_replace(aResult, std::tr1::wregex(L"^[ �@]+"), std::wstring(L""));
	//�����̂̋󔒍폜
	aResult = std::tr1::regex_replace(aResult, std::tr1::wregex(L"[ �@]+$"), std::wstring(L""));
#endif
	return CString(aResult.c_str());
}

CString CCheck::fnZen2Han(CString cData)
{
	CString aResult = _T("");
		aResult = Ipc::PoiMaintenance::Utility::StringConvert::ToNarrow( gcnew System::String(cData) );
	return aResult;
}

CString CCheck::fnHan2Zen(CString cData)
{
	CString aResult = _T("");
	aResult = Ipc::PoiMaintenance::Utility::StringConvert::ToWide( gcnew System::String(cData) );
	return aResult;
}

CString CCheck::fnCutMultiSpace(CString cData)
{
//	cData = _T("�@�@�e    �X�@�@�@�g");
	std::wstring aResult = cData;
#ifdef USE_BOOST_REGEX
	//2�ȏ�A������̋󔒂�1��(���p)
	aResult = boost::regex_replace(aResult, boost::wregex(L"  +"), L" ");
	//2�ȏ�A������̋󔒂�1��(�S�p)
	aResult = boost::regex_replace(aResult, boost::wregex(L"�@�@+"), L"�@");
#else
	//2�ȏ�A������̋󔒂�1��(���p)
	aResult = std::tr1::regex_replace(aResult, std::tr1::wregex(L"  +"), std::wstring(L" "));
	//2�ȏ�A������̋󔒂�1��(�S�p)
	aResult = std::tr1::regex_replace(aResult, std::tr1::wregex(L"�@�@+"), std::wstring(L"�@"));
#endif
	return CString(aResult.c_str());
}

CString CCheck::fnCutDoubleQuotation(CString cData, LPCTSTR cField)
{
	std::string aResult = CT2CA(cData).m_psz;
#ifdef USE_BOOST_REGEX
	if(boost::regex_match(aResult, boost::regex("^\".*\"$"))){	//���[�Ɂu"�v����������
		//�擪�́u"�v�폜
		aResult = boost::regex_replace(aResult, boost::regex("^\""), "");
		//�����́u"�v�폜
		aResult = boost::regex_replace(aResult, boost::regex("\"$"), "");
		//�u""�v���u"�v��
//		aResult = boost::regex_replace(aResult, boost::regex("\"\""), "\"");
		_ftprintf(m_pFile, _T("ERROR,���[�Ɂu\"�v������܂��B,%s,%s,%s\n"), static_cast<LPCTSTR>(m_Seq), cField, static_cast<LPCTSTR>(cData));
	}
#else
	if(std::tr1::regex_match(aResult, std::tr1::regex("^\".*\"$"))){	//���[�Ɂu"�v����������
		//�擪�́u"�v�폜
		aResult = std::tr1::regex_replace(aResult, std::tr1::regex("^\""), std::string(""));
		//�����́u"�v�폜
		aResult = std::tr1::regex_replace(aResult, std::tr1::regex("\"$"), std::string(""));
		//�u""�v���u"�v��
//		aResult = boost::regex_replace(aResult, boost::regex("\"\""), "\"");
//		_ftprintf(m_pFile, _T("ERROR,���[�Ɂu\"�v������܂��B,%s,%s,%s\n"), static_cast<LPCTSTR>(m_Seq), cField, static_cast<LPCTSTR>(cData));
	}
#endif
	return CString(aResult.c_str());
}

bool CCheck::fnNewDataCheck(void)
{
	CString aAdd(_T(""));

	for(COMP_LIST::iterator aNew = m_NewData.begin(); aNew != m_NewData.end(); aNew++)
	{
		// �V�[�P���X�Z�b�g
		m_Seq = aNew->first;

		int i = 0;
		RECORD aRec = aNew->second;
		if(m_LineNoIndex  >= 0) m_LineNo	= aRec[m_LineNoIndex];
		if(m_NameIndexNew >= 0) m_Name		= aRec[m_NameIndexNew];
		for(HEADER::iterator aHead = m_Head.begin(); aHead != m_Head.end(); aHead++, i++){
			if(aHead->Find(MAINTE_FIELD_MODIFY) < 0){
				if(!fnIsNotOrig(*aHead)){
					//���l
					if(aRec[i] != _T("")){
						if(!m_ParkMode){
							aAdd.Format(_T("%s\t%s"), static_cast<LPCTSTR>(*aHead), static_cast<LPCTSTR>(aRec[i]));
							fnOutputErr(kError, _T("�V�K�Ȃ̂Ɍ��l�����͂���Ă��܂��B"), aAdd);
						}else{
							//�@�\���V�[�P���X�A���[�g�T���p�|�C���g.*�A������Ə��iXML�j.*�́A���ԏ�ł�NULL���d�l
							if(*aHead == MAINTE_FIELD_FUNC_SEQ || aHead->Find(MAINTE_FIELD_ROOT_POINT) == 0|| aHead->Find(MAINTE_FIELD_XML) == 0 || *aHead == MAINTE_FIELD_ROOT_PRI || *aHead == MAINTE_FIELD_ROOT_PRI_ATTR || *aHead == MAINTE_FIELD_ROOT_ATTR){
								aAdd.Format(_T("%s\t%s"), static_cast<LPCTSTR>(*aHead), static_cast<LPCTSTR>(aRec[i]));
								fnOutputErr(kError, _T("�V�K�Ȃ̂Ɍ��l�����͂���Ă��܂��B"), aAdd);
							}
						}
					}else{
						if(m_ParkMode){
							//Not NULL�`�F�b�N
							fnNotNullCheck(*aHead);
						}
					}
					if(*aHead == MAINTE_FIELD_INFOFILE_ID){
						fnInfoFileCheck(aRec[i]);
					}
				}
			}else{
				//�C���l
				if(aRec[i] != _T("")){
					fnFieldCheck(*aHead, aRec[i], aRec);
				}else{
					if(!m_ParkMode){
						//Not NULL�`�F�b�N
						fnNotNullCheck(*aHead);
					}
				}
			}
		}
	}
	return true;
}

CString CCheck::fnConvYomiSP(CString cData)
{
#if 0
	/*
	�y�ǂ݁z�n�C�t���Ȃǁ������L���̕ϊ�
�@�@�u�\�v(�S�p�_�b�V��)
�@�@�u�]�v(�S�p�n�C�t��)
�@�@�u�|�v(�S�p�}�C�i�X)
�@�@�u���v�i�r���j
	*/
	LPCTSTR  aHyphen[] = {
		_T("�\"),
		_T("�]"),
		_T("�|"),
		_T("��"),
		NULL
	};
	for(int i = 0; aHyphen[i] != NULL; i++){
		cData.Replace(aHyphen[i], _T("�["));
	}

	/*
	�y�ǂ݁z����L���̍폜
	�D�@(�S�p�s���I�h)
�@�@�E�@(�S�p���_)
�@�@�u�@(�S�p�����J�b�R(�n))
�@�@�v�@(�S�p�����J�b�R(�I))
�@�@�i�@(�S�p���J�b�R(�n))
�@�@�j�@(�S�p���J�b�R(�I))
�@�@�^�@(�S�p�ΐ�)
�@�@�C�@(�S�p�R���})
�@�@�A�@(�S�p�Ǔ_)
�@�@�B�@(�S�p��_)
�@�@�w�@(��d�����J�b�R(�n))
�@�@�x�@(��d�����J�b�R(�I))
�@�@�m�@(�S�p��J�b�R(�n))
�@�@�n�@(�S�p��J�b�R(�I))
�@�@�o�@(�S�p���J�b�R(�n))
�@�@�p�@(�S�p���J�b�R(�I))
�@�@�y�@(���݂��J�b�R(�n))
�@�@�z�@(���݂��J�b�R(�I))
�@�@�k�@(���������J�b�R(�n))
�@�@�l�@(���������J�b�R(�I))
�@�@�q�@(�R�J�b�R(�n))
�@�@�r�@(�R�J�b�R(�I))
�@�@�s�@(��d�R�J�b�R(�n))
�@�@�t�@(��d�R�J�b�R(�I))
�@�@���@(��菬����)
�@�@���@(���傫��)
�@�@��@(�����ɏ�����)
�@�@��@(�����ɑ傫��)
	*/
	LPCTSTR  aDelete[] = {
		_T("�D"),//�@(�S�p�s���I�h)
		_T("�E"),//�@(�S�p���_)
		_T("�u"),//�@(�S�p�����J�b�R(�n))
		_T("�v"),//�@(�S�p�����J�b�R(�I))
		_T("�i"),//�@(�S�p���J�b�R(�n))
		_T("�j"),//�@(�S�p���J�b�R(�I))
		_T("�^"),//�@(�S�p�ΐ�)
		_T("�C"),//�@(�S�p�R���})
		_T("�A"),//�@(�S�p�Ǔ_)
		_T("�B"),//�@(�S�p��_)
		_T("�w"),//�@(��d�����J�b�R(�n))
		_T("�x"),//�@(��d�����J�b�R(�I))
		_T("�m"),//�@(�S�p��J�b�R(�n))
		_T("�n"),//�@(�S�p��J�b�R(�I))
		_T("�o"),//�@(�S�p���J�b�R(�n))
		_T("�p"),//�@(�S�p���J�b�R(�I))
		_T("�y"),//�@(���݂��J�b�R(�n))
		_T("�z"),//�@(���݂��J�b�R(�I))
		_T("�k"),//�@(���������J�b�R(�n))
		_T("�l"),//�@(���������J�b�R(�I))
		_T("�q"),//�@(�R�J�b�R(�n))
		_T("�r"),//�@(�R�J�b�R(�I))
		_T("�s"),//�@(��d�R�J�b�R(�n))
		_T("�t"),//�@(��d�R�J�b�R(�I))
		_T("��"),//�@(��菬����)
		_T("��"),//�@(���傫��)
		_T("��"),//�@(�����ɏ�����)
		_T("��"),//�@(�����ɑ傫��)
		NULL
	};
	for(int i = 0; aDelete[i] != NULL; i++){
		cData.Replace(aDelete[i], _T(""));
	}
#else
//	cData = _T("�D��\�]�|��");
	cData = Ipc::PoiMaintenance::Utility::StringConvert::ToMacron( gcnew System::String(cData) );
	cData = Ipc::PoiMaintenance::Utility::StringConvert::DeleteYomiSymbols( gcnew System::String(cData) );
#endif
	return cData;
}

bool CCheck::fnNotNullCheck(CString cField)
{
	cField.Replace(MAINTE_FIELD_MODIFY, _T(""));
	for(int i = 0; NOT_NULL[i] != NULL; i++){
		if(cField == NOT_NULL[i]){
			fnOutputErr(kError, _T("�V�K�ɕK�v�Ȓl�����͂���Ă��܂���B"), cField);
			return false;
		}
	}
	return true;
}

bool CCheck::fnExistFieldCheck(void)
{
	if(m_FieldMap.find(MAINTE_FIELD_PREFNAME) != m_FieldMap.end() 
		&& m_FieldMap.find(CString(MAINTE_FIELD_PREFNAME)+CString(MAINTE_FIELD_MODIFY)) != m_FieldMap.end()
		&& m_FieldMap.find(MAINTE_FIELD_CITY_NAME) != m_FieldMap.end() 
		&& m_FieldMap.find(CString(MAINTE_FIELD_CITY_NAME)+CString(MAINTE_FIELD_MODIFY)) != m_FieldMap.end()){
		m_HaveAddr = true;
	}else{
		m_HaveAddr = false;
	}
	return true;
}

using namespace Ipc::PoiMaintenance::Sirius::Utility::SiriusSvTime::Controller;
using namespace System::Collections::Generic; 
using namespace Ipc::PoiMaintenance::Sirius::Utility::SiriusSvTime::Items;
using namespace Ipc::PoiMaintenance::Sirius::Utility::SiriusSvTime::Items::MultiSvElements;
using namespace Ipc::PoiMaintenance::Sirius::Utility::SiriusSvTime;

bool CCheck::fnCompareRecordGS(const RECORD & cOrgData, const RECORD & cNewData)
{
	bool aResult = true;
	int i = 0;
	List<GsRecord^> ^gsWeekList = gcnew List<GsRecord^>;
	List<GsRecord^> ^gsPeriodList = gcnew List<GsRecord^>;
	CString aAdd(_T(""));
	for(HEADER::iterator aHead = m_Head.begin(); aHead != m_Head.end(); aHead++, i++){
		if(fnIsWeekData(*aHead)){
			//���`�y�{�j
			if(cNewData[i] == _T("")){
				fnOutputErr(kError, _T("�t�B�[���h��NULL�ł��B"), *aHead);
				aResult = false;
			}else{
				GsWeekRecord ^aWeekRec = gcnew GsWeekRecord;
				System::String^ aCap = gcnew System::String(*aHead);
				aWeekRec->Caption = aCap;
				System::String^ aVal = gcnew System::String(cNewData[i]);
				aWeekRec->Value = aVal;
				gsWeekList->Add(aWeekRec);
			}
		}else if(aHead->Find(MAINTE_FIELD_PERIOD) == 0){
			//�w�����x
			if(cNewData[i] == _T("")){
				fnOutputErr(kError, _T("�t�B�[���h��NULL�ł��B"), *aHead);
				aResult = false;
			}else{
				GsPeriodRecord ^aPeriodRec = gcnew GsPeriodRecord;
				System::String^ aCap = gcnew System::String(*aHead);
				aPeriodRec->Caption = aCap;
				System::String^ aVal = gcnew System::String(cNewData[i]);
				aPeriodRec->Value = aVal;
				gsPeriodList->Add(aPeriodRec);
			}
		}else{
			if(!fnIsNotOrig(*aHead)){
				if(cOrgData[i] != cNewData[i]){
					aAdd.Format(_T("%s\t%s\t%s"), static_cast<LPCTSTR>(*aHead), static_cast<LPCTSTR>(cOrgData[i]), static_cast<LPCTSTR>(cNewData[i]));
					fnOutputErr(kError, _T("���l���C������Ă��܂��B"), aAdd);
				}
				if(cOrgData[i] != _T("")){
					fnFieldCheck(*aHead, cOrgData[i], cOrgData);
				}
			}
		}
	}
	if(aResult){
		GsStringAnalyzer^ analyzer = gcnew GsStringAnalyzer;
		MultiSvAnalyzeResult^ result = analyzer->Execute( gsWeekList, gsPeriodList );
		if( result->ResultStatus == SvResult::Error )
		{
	//		SvErr2stSiRiUSErr( result->ErrorList, listErrs );
			fnOutputErr(kError, _T("�c�Ǝ��Ԃ̉�͂Ɏ��s���܂����B"));
			return false;
		}
		// �����c�Ǝ��Ԃɕϊ�
		MultiSvDataAnalyzer^ analyzer2 = MultiSvDataAnalyzer::CreateInstance();
		analyzer2->Execute( result );
		if( result->ResultStatus == SvResult::Error )
		{
	//		SvErr2stSiRiUSErr( result->ErrorList, listErrs );
			fnOutputErr(kError, _T("�����c�Ǝ��Ԃ̕ϊ��Ɏ��s���܂����B"));
			return false;
		}
	}
	return aResult;
}

bool CCheck::fnIsWeekData(const CString& cData)
{
	for(int i= 0; GS_WEEK[i] != NULL; i++){
		if(cData == GS_WEEK[i]){
			return true;
		}
	}
	return false;
}

bool CCheck::fnIsNotOrig(const CString& cData)
{
	for(int i= 0; NOT_ORIG[i] != NULL; i++){
		if(cData == NOT_ORIG[i]){
			return true;
		}
	}
	return false;
}

bool CCheck::fnLoadMikakuMaster(LPCTSTR cMMaster)
{
	// �t�@�C���ǂݍ���
	std::ifstream ifs( cMMaster );
	if( ! ifs.is_open() ){
		_ftprintf(m_pFile, _T("#ERROR\t�t�@�C�����J���܂���B\t%s\n"), cMMaster);
		return false;
	}

	std::string line;
	if( getline( ifs, line ) ){	//�w�b�_�ǂݔ�΂�
		while( getline( ifs, line ) ){
			RECORD aRecord;
			char_separator a_csSep("\t", "", boost::keep_empty_tokens); // �����ݒ�
			tokenizer	a_tTokens(line, a_csSep);	// ����
			foreach( const std::string& str, std::make_pair( a_tTokens.begin(), a_tTokens.end()) ){
				aRecord.push_back(CString(str.c_str()));
			}
			m_Mikaku.insert(aRecord[1]);
		}
	}
	return true;
}

bool CCheck::fnSetDeleteInfo(DELETE_INFO& cDelInfo, CString cField, const CString cData)
{
	cField.Replace(MAINTE_FIELD_WINDOW, _T(""));
	cField.Replace(MAINTE_FIELD_ATM, _T(""));
	if(cField.Find(MAINTE_FIELD_BANK_DELETE_FLAG) == 0){
		//�폜�t���O
		cDelInfo.m_DeleteCode = _ttol(cData);
	}else if(cField.Find(MAINTE_FIELD_BABK_CLOSED_CODE) == 0){
		//�����R�R�[�h
		cDelInfo.m_ClosedCode = cData;
	}else if(cField.Find(MAINTE_FIELD_BANK_CLOSED_MEMO) == 0){
		//�����t�E���R
		cDelInfo.m_ClosedMemo = cData;
	}
	return true;
}

CString CCheck::fnCompRegulate(const CString& cField, CString cData)
{
	if(cData == _T("")){
		return cData;
	}
	CString aData = _T("");
	if(cField.Find(_T("���t")) > 0){
		std::string aDate = CT2CA(cData).m_psz;
		char_separator a_csSep("/", "", boost::keep_empty_tokens); // �����ݒ�
		tokenizer	a_tTokens(aDate, a_csSep);	// ����
		RECORD aRec;
		std::vector<CString> aTmpList;
		foreach( const std::string& aStr, std::make_pair( a_tTokens.begin(), a_tTokens.end()) ){
			CString aTmp(aStr.c_str());
			aTmpList.push_back(aTmp);
		}
		int aYYYY= _ttol(aTmpList[0]);
		int aMM= _ttol(aTmpList[1]);
		int aDD= _ttol(aTmpList[2]);
		aData.Format(_T("%4d/%02d/%02d"), aYYYY, aMM, aDD);
	}else{
		aData = fnRegulate(cField, cData);
	}
	return aData;
}

// ���W�����`�F�b�N
bool CCheck::fnCoordCheck( const CString& strField, const CString& strData )
{
	// ���X�g�̍��W�t�H�[�}�b�g��1/256���x���b���͔��ʂł��Ȃ��̂ŁA
	// �吨���߂� 1/256 �`�F�b�N�����{���A�G���[������Γx���b�����{
	// ����NG�Ȃ�G���[�Ƃ���
	// (1/256�Ȃ̂ɓx���b������P�[�X�̓G���[�ƂȂ�Ȃ����A�^�p�゠�肦�Ȃ��Ƃ������ƂŖ��Ȃ�)
	std::string strTmpData( CT2CA(strData).m_psz );

	// �o�x
	bool b256Error = false;
	if( strField.Find(MAINTE_FIELD_LONG) >= 0 )
	{
		// �܂�1/256�`�F�b�N
		if( std::tr1::regex_match(strTmpData, std::tr1::regex("[0-9]{9}")) )
			b256Error = _ttol(strData) < 112435200 || _ttol(strData) > 143358965;
		else
			b256Error = true;

		// 1/256��NG�Ȃ�A�x���b�`�F�b�N
		if( b256Error )
		{
			// �b�ŏ����_�ȉ�����o�[�W�����ƂȂ��o�[�W�����ǂ���ɂ��Ή�
			return std::tr1::regex_match(strTmpData, std::tr1::regex("E1[2-5]{1}[0-9]{1}[.][0-5]{0,1}[0-9]{1}[.][0-5]{0,1}[0-9]{1}[.][0-9]{1,2}"))
				|| std::tr1::regex_match(strTmpData, std::tr1::regex("E1[2-5]{1}[0-9]{1}[.][0-5]{0,1}[0-9]{1}[.][0-5]{0,1}[0-9]{1}"));
		}
		else
			return true;

	}
	// �ܓx
	else
	{
		// �܂�1/256�`�F�b�N
		if( std::tr1::regex_match(strTmpData, std::tr1::regex("[0-9]{8}")) )
			b256Error = _ttol(strData) < 17510400 || _ttol(strData) > 48434165;
		else
			b256Error = true;

		// 1/256��NG�Ȃ�A�x���b�`�F�b�N
		if( b256Error )
		{
			// �b�ŏ����_�ȉ�����o�[�W�����ƂȂ��o�[�W�����ǂ���ɂ��Ή�
			return std::tr1::regex_match(strTmpData, std::tr1::regex("N[1-5]{1}[0-9]{1}[.][0-5]{0,1}[0-9]{1}[.][0-5]{0,1}[0-9]{1}[.][0-9]{1,2}"))
				   || std::tr1::regex_match(strTmpData, std::tr1::regex("N[1-5]{1}[0-9]{1}[.][0-5]{0,1}[0-9]{1}[.][0-5]{0,1}[0-9]{1}"));
		}
		else
			return true;
	}
}

// �G���[���O�̏o��
void CCheck::fnOutputErr(EErrLv cErrLv, const CString &cMsg, const CString &cAdd)
{
	CString aErrLv(_T("ERROR"));
	if(cErrLv == kWarning) aErrLv = _T("WARNING");

	_ftprintf(m_pFile, _T("%s\t%s\t%s\t%s\t%s\t%s\n"), static_cast<LPCTSTR>(aErrLv), static_cast<LPCTSTR>(cMsg),
		static_cast<LPCTSTR>(m_LineNo), static_cast<LPCTSTR>(m_Seq), static_cast<LPCTSTR>(m_Name), static_cast<LPCTSTR>(cAdd));
}
