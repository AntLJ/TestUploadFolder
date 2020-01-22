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

// CheckJusyoDist.h: CheckJusyoDist �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHECKJUSYODIST_H__BF9CACAC_4A4E_4A68_9937_C1F4A4D98FEB__INCLUDED_)
#define AFX_CHECKJUSYODIST_H__BF9CACAC_4A4E_4A68_9937_C1F4A4D98FEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define FD_ADDR_CODE		"AddrCode"	//!< �G���[�Z���|�C���gPGDB��̑�����

#define PROGRAM_ID				"H05xx"

#define ECT_MAX_RECORD			7

#define ERR_CANT_GET_VALUE		1
#define ERR_ILLEGAL_VALUE		2
#define ERR_QUERY_FAILED		3
#define ERR_CANT_INSERT			4
#define ERR_CANT_GET_SHAPE		5
#define ERR_NULL_GAIKU_FUGO		10
#define ERR_GP_DISTRIBUTION		100

#define	ON_FiveCode		0x01	// �s�撬���P��(5��)���s���[�h
#define	ON_ElevenCode	0x04	// 11���P�ʎ��s���[�h
#define ON_Addrcode8    0x08    // �Z��8���{�n�ԒP�ʎ��s���[�h

static ErrCodeRec gECTable[] = {
	{ ERR_CANT_GET_VALUE,	kError,		"�t�B�[���h����l���擾�ł��Ȃ�" },
	{ ERR_ILLEGAL_VALUE,	kError,		"�l���s��" },
	{ ERR_QUERY_FAILED,		kError,		"�N�G���̎��s�Ɏ��s" },
	{ ERR_CANT_INSERT,		kError,		"�e�[�u���ɑ}���ł��܂���" },
	{ ERR_CANT_GET_SHAPE,	kError,		"�`����擾�ł��܂���" },
	{ ERR_NULL_GAIKU_FUGO,	kWarning,	"�Z���\�������ςȂ̂ɊX�敄����NULL" },
	{ ERR_GP_DISTRIBUTION,	kError,		"���|�C���g�̕��z�ɂ΂��������" },
};

/// �s�撬�����X�g�쐬�^�C�v
enum	ECityListType {
	kCLNone			= 0,	// ���w��
	kCLAll			= 1,	// �S�s�撬���w��
	kCLList			= 2,	// ���X�g�t�@�C���ł̎s�撬���w��
	kCLSingle		= 3,	// �P��s�撬���w��
	kCLRange		= 4,	// �R�[�h�͈͎w��
	kAdr8List       = 5		// �Z��8���R�[�h���X�g�w��
};

struct AddrTmpRec
{
	string	eAddrCode;
	long	ePointID;
	double	eLongitude;
	double	eLatitude;
	long	eAddrClass;
	long	eGouType;
	string	eCharPntName;	//!< [Bug6516]�����n�Ԗ���

	AddrTmpRec() {
		ePointID	= 0;
		eLongitude	= 0;
		eLatitude	= 0;
		eAddrClass	= 0;
		eGouType	= 0;
	}

	void operator = (const AddrTmpRec &cATRec) {
		eAddrCode	= cATRec.eAddrCode;
		ePointID	= cATRec.ePointID;
		eLongitude	= cATRec.eLongitude;
		eLatitude	= cATRec.eLatitude;
		eAddrClass	= cATRec.eAddrClass;
		eGouType	= cATRec.eGouType;
		eCharPntName = cATRec.eCharPntName;
	}

	bool operator < (const AddrTmpRec &cATRec) const {
		if(eAddrCode == cATRec.eAddrCode) {
			if(eGouType == cATRec.eGouType) {
				return ePointID < cATRec.ePointID;
			}
			return eGouType < cATRec.eGouType;
		}
		return eAddrCode < cATRec.eAddrCode;
	}

};

typedef set<AddrTmpRec, less<AddrTmpRec> >	AddrTmpSet;

struct PointRec
{

	long	ePointID;
	double	eLongitude;
	double	eLatitude;
	long	eGouType;

	PointRec() {
		ePointID	= 0;
		eLongitude	= 0;
		eLatitude	= 0;
		eGouType	= 0;
	}

	void operator = (const PointRec &cPtRec) {
		ePointID	= cPtRec.ePointID;
		eLongitude	= cPtRec.eLongitude;
		eLatitude	= cPtRec.eLatitude;
		eGouType	= cPtRec.eGouType;
	}

	bool operator < (const PointRec &cPtRec) const {
		if(eGouType == cPtRec.eGouType) {
			return ePointID < cPtRec.ePointID;
		}
		return eGouType < cPtRec.eGouType;
	}

};

typedef list<PointRec>	PointList;

struct AddrRec
{
	string		eAddrCode;
	PointList	ePoints;
	string		eCharPntName;	//!< [Bug6516]�����n�Ԗ���

	AddrRec() {}

	AddrRec& operator = (const AddrRec &cAddrRec) {
		eAddrCode	= cAddrRec.eAddrCode;
		ePoints		= cAddrRec.ePoints;
		eCharPntName = cAddrRec.eCharPntName;
		return *this;
	}

	bool operator == (const AddrRec& addrRec)
	{
		return eAddrCode == addrRec.eAddrCode && eCharPntName == addrRec.eCharPntName;
	}

	bool operator < (const AddrRec &cAddrRec) const {
		return eAddrCode < cAddrRec.eAddrCode;
	}

};

typedef list<AddrRec>	AddrList;

struct GroupRec
{

	PointRec	eRepPoint;
	PointList	ePoints;

	GroupRec() {
	}

	GroupRec& operator = (const GroupRec &cGRec) {
		ePoints		= cGRec.ePoints;
		return *this;
	}

};

typedef list<GroupRec>	GroupList;

struct AC11Rec
{

	string	eAC11;
	set<long, less<long> >	eOIDSet;

	AC11Rec() {
	}

	AC11Rec(const AC11Rec &cAC11Rec) {
		eAC11		= cAC11Rec.eAC11;
		eOIDSet		= cAC11Rec.eOIDSet;
	}

	~AC11Rec() {
	}

	bool operator < (const AC11Rec &cAC11Rec) const {
		return eAC11 < cAC11Rec.eAC11;
	}

};

typedef set<AC11Rec, less<AC11Rec> >	AC11Set;

struct AC8Rec
{
	string	eAC8;
	AC11Set	eAC11Set;

	AC8Rec() {
	}

	AC8Rec(const AC8Rec &cAC8Rec) {
		eAC8		= cAC8Rec.eAC8;
		eAC11Set	= cAC8Rec.eAC11Set;
	}

	~AC8Rec() {
	}

	bool operator < (const AC8Rec &cAC8Rec) const {
		return eAC8 < cAC8Rec.eAC8;
	}

};

typedef set<AC8Rec, less<AC8Rec> >		AC8Set;

class ACSet : public AC8Set
{

public:

	void	Add(const string &cAC11, const long cOID )
	{
		AC8Rec	aAC8Rec;
		aAC8Rec.eAC8	= cAC11.substr(0, 8);
		AC8Set::iterator aAC8Itr = find(aAC8Rec);
		if( aAC8Itr != end() )
		{	// ����W������������
			aAC8Rec = *aAC8Itr;	// �R�s�[
			erase(aAC8Itr);		// ��U�폜
			AC11Rec	aAC11Rec;
			aAC11Rec.eAC11	= cAC11;
			AC11Set::iterator aAC11Itr = aAC8Rec.eAC11Set.find(aAC11Rec);
			if( aAC11Itr != aAC8Rec.eAC11Set.end() )	// ����11������������
			{
				aAC11Rec = *aAC11Itr;				// �R�s�[
				aAC8Rec.eAC11Set.erase(aAC11Itr);	// ��U�폜
				aAC11Rec.eOIDSet.insert(cOID);		// �o�^�iOID�j
				aAC8Rec.eAC11Set.insert(aAC11Rec);	// �ēo�^�i11���j
				insert(aAC8Rec);					// �ēo�^�i�W���j
			}
			else {										// ����11����������Ȃ�
				aAC11Rec.eOIDSet.insert(cOID);		// �o�^�iOID�j
				aAC8Rec.eAC11Set.insert(aAC11Rec);	// �o�^�i11���j
				insert(aAC8Rec);					// �ēo�^�i�W���j
			}
		} else {				// ����W����������Ȃ�
			AC11Rec	aAC11Rec;
			aAC11Rec.eAC11	= cAC11;
			aAC11Rec.eOIDSet.insert(cOID);			// �o�^�iOID�j
			aAC8Rec.eAC11Set.insert(aAC11Rec);		// �o�^�i11���j
			insert(aAC8Rec);						// �o�^�i�W���j
		}
	}

};

struct AddrPolyRec
{

	long				eOID;
	IPolygonPtr			ipPolygon;

	AddrPolyRec& operator = (const AddrPolyRec &cAPRec) {
		eOID		= cAPRec.eOID;
		ipPolygon	= cAPRec.ipPolygon;
		return *this;
	}

};

typedef list<AddrPolyRec>	AddrPoly;
typedef list<AddrPoly>		AddrGroup;

struct AdminAttr
{
	string	eAddrCode;		//!< �Z���R�[�h
	long	eGaikuFugo;		//!< �X�敄��
	long	eAddrClassC;	//!< �Z���������
	string	eAddrCode2;		//!< �Z���R�[�h2
	string	eExtGaiku;		//!< �g���X�敄��[Bug6516]

	AdminAttr() {
		eGaikuFugo	= 0;
		eAddrClassC	= 0;
	}

	AdminAttr& operator = (const AdminAttr &cAdmAttr) {
		eAddrCode	= cAdmAttr.eAddrCode;
		eGaikuFugo	= cAdmAttr.eGaikuFugo;
		eAddrClassC	= cAdmAttr.eAddrClassC;
		eAddrCode2	= cAdmAttr.eAddrCode2;
		eExtGaiku	= cAdmAttr.eExtGaiku;
		return *this;
	}

};

class CheckJusyoDist : public SDEAccess  
{
public:

	CheckJusyoDist();
	virtual ~CheckJusyoDist();

	/**
	 * @brief	�g�p�@
	 */
	static void Usage();

	bool Execute(int cArgc, char **cArgv);

private:

	char	*mDBCon_Address;       //!< �Z���nSDE�T�[�o�ڑ���
	char	*mPGDB_Out;            //!< �G���[�Z���|�C���gPGDB

	char	*mFC_Target;           //!< �G���[�Z���|�C���gPGDB��̃��C����
	char	*mFC_CityAdmin;        //!< �s�s�n�}�s���E�|���S���̃t�B�[�`����
	char	*mFC_GouPoint;         //!< ���|�C���g�̃t�B�[�`����
	char	*mFC_CSAddrPoint;      //!< �N���[���|�C���g�̃t�B�[�`����
	char	*mFC_KlinkPoint;       //!< �ƌ`�����N�|�C���g�̃t�B�[�`����

	IFeatureClassPtr	ipFC_CityAdmin;      //!< CITY_ADMIN�̃t�B�[�`���N���X
	IFeatureClassPtr	ipFC_GouPoint;       //!< GOU_POINT�̃t�B�[�`���N���X
	IFeatureClassPtr	ipFC_CSAddrPoint;    //!< CS_ADDR_POINT�̃t�B�[�`���N���X
	IFeatureClassPtr	ipFC_KlinkPoint;     //!< KLINK_POINT�̃t�B�[�`���N���X

	IFeatureClassPtr	ipFC_TmpPoints;      //!< �G���[�Z���|�C���gPGDB�̃t�B�[�`���N���X

	long	mFID_CityCode;       //!< CITY_ADMIN��CITYCODE�t�B�[���h�C���f�b�N�X
	long	mFID_AddrCode;       //!< CITY_ADMIN��ADDRCODE�t�B�[���h�C���f�b�N�X
	long	mFID_GaikuFugo;      //!< CITY_ADMIN��GAIKUFUGO�t�B�[���h�C���f�b�N�X 
	long	mFID_ExtGaikuFugo;   //!< CITY_ADMIN��EXTGAIKUFUGO�t�B�[���h�C���f�b�N�X 
	long	mFID_AreaClassC;     //!< CITY_ADMIN��AREACLASS_C�t�B�[���h�C���f�b�N�X 
	long	mFID_AddrClassC;     //!< CITY_ADMIN��ADDRCLASS_C�t�B�[���h�C���f�b�N�X
	long	mFID_AddrCode2;      //!< CITY_ADMIN��ADDRCODE2�t�B�[���h�C���f�b�N�X

	long	mFID_GouNo;          //!< GOU_POINT��GOU_NO�t�B�[���h�C���f�b�N�X
	long	mFID_GouType;        //!< GOU_POINT��GOU_TYPE�t�B�[���h�C���f�b�N�X
	long	mFID_PriorityF;      //!< GOU_POINT��PRIORITY_F�t�B�[���h�C���f�b�N�X
	long	mFID_ExpGouNoF;      //!< GOU_POINT��EXPGOUNO_F�t�B�[���h�C���f�b�N�X

	long	mFID_InfoSrcC;       //!< KLINK_POINT��INFOSRC_C�t�B�[���h�C���f�b�N�X
	long	mFID_InputSeidoC;    //!< KLINK_POINT��INPUTSEIDO_C�t�B�[���h�C���f�b�N�X
	long	mFID_AddrCode_KP;    //!< KLINK_POINT��ADDRCODE_ORG�t�B�[���h�C���f�b�N�X

	long	mFID_AddrCode_CS;    //!< CS_ADDR_POINT��ADDRCODE�t�B�[���h�C���f�b�N�X

	long	mFID_AddrCode_TP;    //!< �G���[�Z���|�C���gPGDB��ADDRCODE�t�B�[���h�C���f�b�N�X

	bool	mIgnoreTPG;          //!< TPG���O�I�v�V�����i�i-t�j�w�蔻��t���O

	char	*mFN_ChkLog;         //!< �G���[���O�t�@�C����
	OutputErrLog	mChkLog;     //!< ���O�Ǘ��N���X

	char	*mFN_CityList;       //!< �s�撬�����X�g�t�@�C�����i-cl�I�v�V�����w�莞�j
	char	*mCityCode;          //!< �s�撬���R�[�h�i-cs�I�v�V�����w�莞�j
	char    *mAddr8List;         //!< �Z��8���R�[�h���X�g���i-adr8�I�v�V�����w�莞�j

	long	mRangeMin;           //!< �s�撬���R�[�h�ŏ��l�i-cr�I�v�V�����w�莞�j
	long	mRangeMax;           //!< �s�撬���R�[�h�ő�l�i-cr�I�v�V�����w�莞�j

	double	mDistErr;            //!< �O���[�v�����i-dn�I�v�V�����l�j
	double	mDistNbr;            //!< �G���[�Ƃ���O���[�v�ԋ����i-de�I�v�V�����l�j

	int		m_iExeMode;			 //!< ���s���[�h�i5���A11���A8���{�n�ԁj

	CityList	mCityList;       //!< �����Ώۂ̏Z���R�[�h���X�g

	ECityListType	mCLType;     //!< �s�撬�����X�g�쐬�^�C�v

	AddrTmpSet	mAddrTmpSet;     //!< �Z���|�C���g�i�[�R���e�i
	AddrList	mAddrList;       //!< ����Z���R�[�h�ŃO���[�v�������Z���|�C���g�̃��X�g

	ACSet		mACSet;          //!< �Z��8��-11���̑Ή����X�g

	crd_cnv		mCConv;          //!< ���W�l�ϊ����C�u����

	JDBAccess	mJDBAccess;      //!< �Z��DB�A�N�Z�X�N���X

	ISpatialCacheManagerPtr	ipSpCacheMgr;       //!< ISpatialCacheManager 

	std::set<int> m_exeModeList; //!< ���s���[�h���X�g

	/**
	 * @brief	���ϐ����擾����
	 * @retval true  �擾����
	 * @retval false �擾���s
	 */
	bool	fnGetEnvVar();

	/**
	 * @brief	�I�v�V������́{����
	 * @param[in] argc  �R�}���h���C���Ŏw�肵�������̐�
	 * @param[in] argv  �R�}���h���C���Ŏw�肵�������̕�����
	 * @retval true  ����OK
	 * @retval false ����NG
	 */
	bool	fnAnalyzeArg( int argc, char** argv );

	/**
	 * @brief	�G���[�Z���|�C���gPGDB�𐶐�����
	 * @retval true  ��������
	 * @retval false �������s
	 */
	bool	fnCreateTmpFC();

	/**
	 * @brief	�����Ώۂ̏Z���R�[�h���X�g�𐶐�����
	 * @retval true  ��������
	 * @retval false �������s
	 */
	bool	fnMakeCityList();

	/**
	 * @brief	�s���E���X�g����
	 * @param	cCityCode	[in]	�s�s�R�[�h
	 * @retval true  ��������
	 * @retval false �������s
	 */
	bool	fnMakeACList(const char *cCityCode);

	/**
	 * @brief	�G���[���O�E�t�@�C���̃I�[�v���{�w�b�_���̏�����
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool	fnOpenChkLog();

	/**
	 * @brief	�w��s���E�̍��|�C���g20����
	 * @param	ipFeature	[in]	�s���E
	 */
	bool fnAdminProc(const IFeaturePtr& ipFeature );

	/**
	 * @brief	�w��s���E���̍��|�C���g20����
	 * @param	ipFeature	[in]	���|�C���g
	 * @param	cAdmAttr	[in]	�s���E
	 * @param	isAddAdr2Point	[in]	ADDRCODE2�̃R�[�h�l�������|�C���g�̒ǉ��L���itrue�F�ǉ�����Afalse�F�ǉ����Ȃ��j
	 */
	void fnGetGPInfo(const IFeaturePtr& ipFeature, const AdminAttr &cAdmAttr, bool isAddAdr2Point);

	/**
	 * @brief	[Bug6516]20�������֐�
	 * @param	cAdmAttr	[in]	�s���E
	 * @param	strGouNo	[in]	���ԍ�
	 * @param	bExGouF		[in]	�g�����ԍ��t���O
	 * @param	rRec		[in]	�Z���R�[�h�E�n�Ԗ���(�����n�Ԏ�)�i�[�p�R���e�i
	 */
	void CreateAddrCode( const AdminAttr& cAdmAttr, const string& strGouNo, bool bExGouF, AddrTmpRec& rRec );

	/**
	 * @brief �w�肵���ƌ`�����N�|�C���g��������擾����
	 * @param[in] ipF_KlinkPoint  �ƌ`�����N�|�C���g�̃t�B�[�`��
	 * @param[in] cAdmAttr  �s�s�n�}�s���E�|���S�����
	 * @retval true  �擾����
	 * @retval false �擾���s
	 */
	bool fnGetKPInfo(const IFeaturePtr& ipF_KlinkPoint, const AdminAttr& cAdmAttr);

	/**
	 * @brief	CS�|�C���g���擾
	 * @param	ipFeature	[in]	CS�|�C���g
	 */
	void fnCSAddrProc(const IFeaturePtr& ipFeature );

	/**
	 * @brief �Z���|�C���g�Q���O���[�v������i�Z��16���R�[�h�P�ʁA���s���[�h�F-e,-f�p�j
	 */
	void fnConvDataSet16();

	/**
	 * @brief �Z���|�C���g�Q���O���[�v������i�Z��8���R�[�h�{�n�ԒP�ʁA���s���[�h�F-adr8�p�j
	 */
	void fnConvDataSet8Chiban();

	/**
	 * @brief �O���[�v�������Z���|�C���g�Q���Ƃɋ����`�F�b�N���s�Ȃ�
	 */
	void fnCheckDataSet16();

	/**
	 * @brief	8���R�[�h�G���A�ŃL���b�V���쐬
	 * @param	cAddrCode	[in]	8���Z���R�[�h
	 */
	bool fnCreateCache8(const std::string &cAddrCode);

	/**
	 * @brief �w�肵��CITY_ADMIN�̃t�B�[�`���̏����擾����
	 * @param[in] ipCityAdmin  �擾�Ώۂ�CITY_ADMIN�̃t�B�[�`��
	 * @param[out] adminAttr  CITY_ADMIN�̏��i�R���e�i�j
	 * @param[out] ipCityAdminGeo  CITY_ADMIN�̃W�I���g��
	 */
	void getCityAdmin(const IFeaturePtr& ipCityAdmin, AdminAttr& adminAttr, IGeometryPtr& ipCityAdminGeo);

	/**
	 * @brief ��������𖞂����Z���|�C���g�����O
	 * @param[in] isExcludeKakeiPoint  �ƌ`�����N�|�C���g�����O�ΏۂƂ��邩�itrue�F�ΏۂƂ���Afalse�F�ΏۂƂ��Ȃ��j
	 */
	void excludingPoints(bool isExcludeKakeiPoint);

	/**
	 * @brief �Z��16���R�[�h�P�ʂɏZ���|�C���g�Q���O���[�v������
	 */
	void makeAddrcodeGroupsByAdr16();

	/**
	 * @brief �Z��8���R�[�h�{�n�ԃR�[�h�i�����t���Z���̏ꍇ�͏Z��8���R�[�h�{�n�Ԗ��́j�P�ʂɏZ���|�C���g�Q���O���[�v������
	 */
	void makeAddrcodeGroupsByAdr8Chiban();

	/**
	 * @brief �w�肵���s�s�n�}�s���E�|���S�����ɑ��݂��鍆�|�C���g���擾����
	 * @param[in] ipFeature  �s�s�n�}�s���E�|���S���̃t�B�[�`��
	 * @retval true  �擾����
	 * @retval false �擾���s
	 */
	bool getGouPoints(const IFeaturePtr& ipFeature );

	/**
	 * @brief �w�肵���Z���R�[�h�ɕR�t��CS�|�C���g���擾����
	 * @param[in] addrcode  �Z���R�[�h
	 * @retval true  �擾����
	 * @retval false �擾���s
	 */
	bool getCSAddrPoints(const CString& addrcode);

};

#endif // !defined(AFX_CHECKJUSYODIST_H__BF9CACAC_4A4E_4A68_9937_C1F4A4D98FEB__INCLUDED_)
