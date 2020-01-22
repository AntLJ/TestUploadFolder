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

// CheckBaseRepExist.h: CheckBaseRepExist �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHECKBASEREPEXIST_H__B1E7A4F7_5191_438C_857C_AFCCA442E3A1__INCLUDED_)
#define AFX_CHECKBASEREPEXIST_H__B1E7A4F7_5191_438C_857C_AFCCA442E3A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <iostream>
#include <fstream>
#include <string>
#include <oracl.h>
#include <libjdbaccess.h>
#include <libsdeaccess.h>
#include <liboutputerrlog.h>
#include <JDBAccess.h>
#include <SDEAccess.h>
#include <OutputErrLog.h>
#include <sindy/libschema.h>

namespace {
	enum ErrCode {
		ERR_NO_BASE_REP = 1,
		ERR_NO_OAZA_REP,
		ERR_ONLY_PAREA,
		ERR_INVALID_CODE,
		ERR_FROM_GOU_POINT,
		ERR_NO_BASE_REP_ADDR2,
		ERR_NO_OAZA_REP_ADDR2,
		ERR_INVALID_CODE_ADDR2,
		ERR_FROM_GOU_POINT_ADDR2,
		ERR_NOT_MATCH_AREACODEMASTER,
	};

	// OutPutErrorLog.lib���g�����߂̒�`
	const char* PROGRAM_ID = "H04xx";
	const int ECT_MAX_RECORD = 10;
	static ErrCodeRec gECTable[] = {
		{ ERR_NO_BASE_REP,			kError,		"Base_Rep_Point ���Ȃ�" },
		{ ERR_NO_OAZA_REP,			kError,		"�厚��\�_���Ȃ�" },
		{ ERR_ONLY_PAREA,			kError,		"PAREA �̂�" },
		{ ERR_INVALID_CODE,			kError,		"�p�~ or ������11��" },
		{ ERR_FROM_GOU_POINT,		kWarning,	"���|�C���g��萶��" },
		{ ERR_NO_BASE_REP_ADDR2,	kError,		"Base_Rep_Point ���Ȃ� (ADDRCODE2)" },
		{ ERR_NO_OAZA_REP_ADDR2,	kError,		"�厚��\�_���Ȃ� (ADDRCODE2)" },
		{ ERR_INVALID_CODE_ADDR2,	kError,		"�p�~ or ������11�� (ADDRCODE2)" },
		{ ERR_FROM_GOU_POINT_ADDR2,	kWarning,	"���|�C���g��萶�� (ADDRCODE2)" },
		{ ERR_NOT_MATCH_AREACODEMASTER,	kWarning,	"�Z���}�X�^�ɂ�����Base_Rep_Point�ɂȂ�" },
	};

	/// �`�F�b�N���[�h
	enum	ECheckMode {
		kNone		= 0,
		kCityAll	= 1,
		kCityList	= 2,
		kCitySingle	= 3,
	};
} // namespace

/**
 * @brief �s���E���i�[�p
 * @note �s���E��11�����Z���R�[�h�P�A�Q�̔��ʂ����悤��
 */
struct adminfo
{
	enum infotype {
		ADDR1 = 0,	///< ADDRCODE1����
		ADDR2 = 1	///< ADDRCODE2����
	};
	
	std::string	mCode6;		///< 6���R�[�h
	long	mInfoType;	///< �Z���R�[�h�P����Ȃ̂��Q����Ȃ̂�(0=�P����A1=�Q����)

	///< �R���X�g���N�^
	adminfo () {
		mInfoType = ADDR1;
	}
	///< �f�X�g���N�^
	~adminfo () {}

	bool operator<(const adminfo& cDat ) const {
		if( mCode6 != cDat.mCode6 ) {
			if( mCode6 < cDat.mCode6 ) {
				return true;
			}
		}else {
			if( mInfoType != cDat.mInfoType ) {
				if( mInfoType < cDat.mInfoType ) {
					return true;
				}
			}
		}
		return false;
	}
};

typedef std::set<std::string>	AddrList;
typedef std::set<adminfo> AdminList;	// [Bug9463]�Œǉ�

struct AORec
{

	std::string	eAddrCode;
	long	eOID;

	AORec() {
		eOID	= 0;
	}

	AORec(const AORec &cAORec) {
		eAddrCode	= cAORec.eAddrCode;
		eOID		= cAORec.eOID;
	}

	AORec(const std::string &cAddrCode) {
		eAddrCode	= cAddrCode;
		eOID		= 0;
	}

	AORec& operator = (const AORec &cAORec) {
		eAddrCode	= cAORec.eAddrCode;
		eOID		= cAORec.eOID;
		return *this;
	}

	bool operator < (const AORec &cAORec) const {
		return eAddrCode < cAORec.eAddrCode;
	}

};

typedef std::set<AORec, std::less<AORec> > AOList;

class CheckBaseRepExist : public SDEAccess  
{

private:

	int		m_Argc; //!< ���s���I�v�V������
	char	**m_Argv; //!< ���s���I�v�V����

	std::string		m_fnLogFile; //!< ���O�t�@�C����
	std::string		m_fnStatList; //!< �X�e�[�^�X�t�@�C����
	OutputErrLog	m_errLog; //!< ���O�Ǘ��N���X

	FILE		*m_fpStatList; //!< �X�e�[�^�X�t�@�C���p�t�@�C���\����

	ECheckMode	m_checkMode; //!< �`�F�b�N���[�h(ca�Acs�Acl�I�v�V����)
	std::string		m_cityCode; //!< 5���R�[�h(cs�I�v�V�����w�莞�Ɏg�p)
	std::string		m_fnCityList; //!< 5���R�[�h���X�g�t�@�C����(cl�I�v�V�����w�莞�Ɏg�p)
	AddrList	m_cityList; //!< �����Ώ�5���R�[�h���X�g

	std::string		m_dbAddress; //!< �Z���nSDE�T�[�o�ڑ���

	std::string		m_fcGouPoint; //!< GOU_POINT�t�B�[�`���N���X��
	std::string		m_fcCityAdmin; //!< CITY_ADMIN�t�B�[�`���N���X��
	std::string		m_fcBaseRep; //!< BASE_REP_POINT�t�B�[�`���N���X��
	std::string		m_fcCSAddrPoint; //!< CS_ADDR_POINT�t�B�[�`���N���X��

	IFeatureClassPtr	m_ipFcGouPoint; //!< GOU_POINT�t�B�[�`���N���X
	IFeatureClassPtr	m_ipFcCityAdmin; //!< CITY_ADMIN�t�B�[�`���N���X
	IFeatureClassPtr	m_ipFcBaseRep; //!< BASE_REP_POINT�t�B�[�`���N���X
	IFeatureClassPtr	m_ipFcCSAddrPoint; //!< CS_ADDR_POINT�t�B�[�`���N���X

	long	m_fIdGPGouNumber; //!< GOU_POINT��GOU_NO�t�B�[���h�C���f�b�N�X
	long	m_fIdCACityCode; //!< CITY_ADMIN��CITYCODE�t�B�[���h�C���f�b�N�X
	long	m_fIdCAAddrCode; //!< CITY_ADMIN��ADDRCODE�t�B�[���h�C���f�b�N�X
	long	m_fIdCSAddrCode; //!< CS_ADDR_POINT��ADDRCODE�t�B�[���h�C���f�b�N�X
	long	m_fIdBRAddrCode; //!< BASE_REP_POINT��ADDRCODE�t�B�[���h�C���f�b�N�X

	JDBAccess	m_jdbAccess; //!< JDBAccess�N���X

	/*
	 * @brief ���ϐ��̉�́E�擾
	 * @retval true ����
	 * @retval false ���s
	 */
	bool	fnGetEnvVar();

	/*
	 * @brief ���s���I�v�V�����̉�́E�擾
	 * @retval true ����
	 * @retval false ���s
	 */
	bool	fnAnalyzeArg();

	/*
	 * @brief 5���R�[�h���X�g�擾
	 * @note -ca �Z���}�X�^����S������5���R�[�h���擾���A���X�g�Ɋi�[
	 *       -cs �I�v�V�����Ŏw�肳�ꂽ5���R�[�h�����X�g�Ɋi�[
	 *       -cl �I�v�V�����Ŏw�肳�ꂽ�t�@�C������5���R�[�h���擾���A���X�g�Ɋi�[
	 * @retval true ����
	 * @retval false ���s
	 */
	bool	fnCreateCityList();

public:

	CheckBaseRepExist();

	virtual ~CheckBaseRepExist();

	/**
	 * @brief ���s�̏�����
	 * @retval true ����������
	 * @retval false ���������s
	 */
	bool	Init( void );

	/**
	 * @brief �厚�^���R�[�h���X�g�̍쐬
	 * @param crCityCode [in]		�s�撬���R�[�h	 
	 * @param crAddrList [out]		�厚�^���R�[�h���X�g
	 */
	void	MakeOazaAzaList( const std::string& crCityCode, AdminList& crAddrList );

	/**
	 * @brief Base_Rep_Point �̃��X�g�쐬
	 * @param crCityCode [in]		�s�撬���R�[�h	 
	 * @param crAOListBR [out]		Base_Rep_Point�̃��X�g	 
	 */
	void	MakeBaseRepPointList( const std::string& crCityCode, AOList& crAOListBR );

	/**
	 * @brief 11���s���E�̓����Ɋ܂܂�鍆�̃J�E���g���擾
	 * @param crCityCode [in]		�s�撬���R�[�h	 
	 * @param crAdmInfo [in]		�s���E�̏��
	 * @param crOazaRepFlag [out]	�厚��\�_�t���O
	 * @param crOID_CityAdmin [out]	�s���E��OID
	 * @param crGPTotal [out]		���̃J�E���g
	 */
	void 	GetContainedGouCount( const std::string& crCityCode, 
									const adminfo& crAdmInfo,
									bool& crOazaRepFlag,
									long& crOID_CityAdmin,
									long& crGPTotal );

	/**
	 * @brief ���|�C���g�̃`�F�b�N
	 * @param crCityCode [in]		�s�撬���R�[�h
	 * @param crAdmInfo [in]		�s���E�̏��
	 * @param crOazaRepFlag [in]	�厚��\�_�t���O
	 * @param crOID_CityAdmin [in]	�s���E��OID
	 * @param crGPTotal [in]		���̃J�E���g
	 */
	void 	CheckGoupoint( const std::string& crCityCode, 
							const adminfo& crAdmInfo,
							bool crOazaRepFlag,
							long crOID_CityAdmin,
							long crGPTotal );

	/**
	 * @brief Base_Rep_Point �Ɋւ���`�F�b�N
	 * @param crAOListBR [in]		Base_Rep_Point �̃��X�g
	 */
	void	CheckBaseRepPoint( const AOList& crAOListBR );

	/**
	 * @brief CS_Addr_Point �Ɋւ���`�F�b�N
	 * @param crAOListBR [in]		Base_Rep_Point �̃��X�g
	 * @param crCityCode [in]		�s�撬���R�[�h
	 */
	void 	CheckCSAddrPoint( const AOList& crAOListBR, const std::string& crCityCode );

	/**
	 * @brief AreacodeMaster(�Z���}�X�^)�Ɋւ���`�F�b�N
	 * @param cityCode [in] �s�撬���R�[�h
	 * @param baseRepAddrCodes [in] Base_Rep_Point��11���R�[�h���X�g
	 */
	void CheckAreacodeMaster(const std::string& cityCode, const AOList& baseRepAddrCodes);

	/**
	 * @brief ���C�������֐�
	 * @param cArgc [in] ���s���I�v�V������
	 * @param cArgv [in] ���s���I�v�V����������
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool	Execute(int cArgc, char **cArgv);

};

#endif // !defined(AFX_CHECKBASEREPEXIST_H__B1E7A4F7_5191_438C_857C_AFCCA442E3A1__INCLUDED_)
