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

/** @file
 * CDoc �N���X�̃C���v�������e�[�V����
 *
 * @author	shimano
 * @date	2003/04/09
 *
<<<<<<< Doc.h
 * $Log: Doc.h,v $
 * Revision 1.5  2006/10/02 11:46:17  shimano
 * Arc9.1�Ή��iF.2.0.0.9�j
 *
 * Revision 1.4  2005/08/02 02:12:53  shimano
 * FILEVERSION 1,0,8,1 PRODUCTVERSION 1,0,8,8 �Ή��B
 *
 * Revision 1.3  2003/11/17 04:18:38  shimano
 * Ver.1.6�i�A�N�Z�X���H���X�g�o�̓t�H�[�}�b�g�C���j�Ή�
 *
 * Revision 1.2  2003/11/15 01:35:57  shimano
 * �A�N�Z�X���H�����m�F�t�@�C���o�͑Ή��iDebug���[�h�j
 *
=======
 * $Log: Doc.h,v $
 * Revision 1.5  2006/10/02 11:46:17  shimano
 * Arc9.1�Ή��iF.2.0.0.9�j
 *
 * Revision 1.4  2005/08/02 02:12:53  shimano
 * FILEVERSION 1,0,8,1 PRODUCTVERSION 1,0,8,8 �Ή��B
 *
 * Revision 1.3  2003/11/17 04:18:38  shimano
 * Ver.1.6�i�A�N�Z�X���H���X�g�o�̓t�H�[�}�b�g�C���j�Ή�
 *
 * Revision 1.2  2003/11/15 01:35:57  shimano
 * �A�N�Z�X���H�����m�F�t�@�C���o�͑Ή��iDebug���[�h�j
 *
 * Revision 1.1.1.2  2003/04/24 09:46:44  shimano
 * �V�K�쐬
 *
>>>>>>> 1.1.1.2
 * Revision 1.0  2003/04/09 15:13:43  shimano
 * beta8�Ή��B
 *
>>>>>>> 1.1.1.3
 * Revision 1.0  2003/05/04 14:31:41  shimano
 * beta9�Ή��B
 */

#if !defined(AFX_DOC_H__0E3F8A46_D0F3_4183_98D3_BFDA6E880BCA__INCLUDED_)
#define AFX_DOC_H__0E3F8A46_D0F3_4183_98D3_BFDA6E880BCA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include	"FacilPoint.h"
#include	"NwObject.h"
#include	"road.h"

typedef	vector<CFacilPoint>		FacilArray;
typedef	vector<CNode>			NodeArray;
typedef	set<long>			    LinkArray;

class CAccessRoad
{
public:
	CAccessRoad() :m_Meshcode(0),m_FromId(0),m_ToId(0) {}
	CAccessRoad(int cMesh,long cFromID,long cToID){ m_Meshcode=cMesh;
													m_FromId=cFromID;
													m_ToId=cToID;}
	int			getMeshcode(void){	return( m_Meshcode );		}
	long		getFromId(void){	return( m_FromId );			}
	long		getToId(void){		return( m_ToId );			}
	
	// ��r���Z�q
	bool operator == (const CAccessRoad& cRoad) const{       
		bool bEqual =( m_Meshcode == cRoad.m_Meshcode
					&& m_FromId  == cRoad.m_FromId
					&& m_ToId  == cRoad.m_ToId );
		return bEqual;
	}
	bool operator < (const CAccessRoad& cRoad) const{
		bool bStatus;
		if(m_Meshcode != cRoad.m_Meshcode){
			bStatus = (m_Meshcode < cRoad.m_Meshcode);
		}
		else if( m_FromId != cRoad.m_FromId ){
			bStatus = (m_FromId < cRoad.m_FromId);
		}
		else{
			bStatus = (m_ToId < cRoad.m_ToId);
		}
		return bStatus;
	}
	bool operator > (const CAccessRoad& cRoad) const{
		bool bStatus;
		if(m_Meshcode != cRoad.m_Meshcode){
			bStatus = (m_Meshcode > cRoad.m_Meshcode);
		}
		else if( m_FromId != cRoad.m_FromId ){
			bStatus = (m_FromId > cRoad.m_FromId);
		}
		else{
			bStatus = (m_ToId > cRoad.m_ToId);
		}
		return bStatus;
	}
// �����o�ϐ�
protected:
	int		m_Meshcode;	//���b�V���R�[�h
	long	m_FromId;	//�����N�n�_�m�[�hOBJECTID
	long	m_ToId;		//�����N�I�_�m�[�hOBJECTID
};

class CDoc  
{
public:
	CDoc();
	virtual ~CDoc();
	
	// �o�̓I�u�W�F�N�g
	enum EnwLinkColor{
		VIEW_ARC_DISP	= 0x0001,		//���H���
		VIEW_ARC_NAVI	= 0x0002,		//�o�H���
		VIEW_ARC_FERRY	= 0x0004,		//���H��ʃ`�F�b�N
		VIEW_ARC_WIDTH	= 0x0008		//���H����
	};
	void deleteMap(void);
	bool convertFacilPoint(LPCTSTR);

	/**
	 * @brief �{�ݏ����擾�����A�̗�������s
	 * @note �R���e���c���X�g�̎擾����SiNDY-k�̏�񒊏o�܂ł��s���B	[bug 8187]�Ή� 2012/04/17 ta_suga
	 * @retval true �ǂݍ��ݐ���
	 * @retval false �ǂݍ��ݎ��s
	 */
	bool getFacilPoint(void);
	/**
	 * @brief �{�ݏ���XML(Excel�̃X�v���b�h�V�[�g�`��)���o�͂���
	 * @note SiNDY-k�̎{�ݏ�񂩂�A�����[�X�f�[�^�uaccess_list.xls�v�Ɠ�����e�̃f�[�^���o�͂���	[bug 8187]�Ή� 2012/04/17 ta_suga
	 * @param �Ȃ�
	 * @retval true �ǂݍ��ݐ���
	 * @retval false �ǂݍ��ݎ��s
	 */
	bool writeFacilList(void);
	/**
	 * @brief Excel�̍s������
	 * @note ������̗��񕪂���Excel�̍s�𖄂߂Ă���	[bug 8187]�Ή� 2012/04/17 ta_suga
	 * @param rStrAry [in] ���͕�����̗���
	 * @param rColNum [in] ���͍s�̗�
	 * @param pOfstream [in/out] �o�̓X�g���[��
	 * @return	�Ȃ�
	 */
	void writeExcelRow(const CString* rStrAry, const int& rColNum, ofstream* pOfstream) const;
	/**
	 * @brief Excel�̃Z���̒��g������
	 * @note Excel�̒��g��String�`���ŏ���	[bug 8187]�Ή� 2012/04/17 ta_suga
	 * @param strValue [in] �Z���ɓ��͂���l
	 * @param pOfstream [in/out] �o�̓X�g���[��
	 * @return	�Ȃ�
	 */
	void writeExcelCell(LPCTSTR strValue, ofstream* pOfstream) const;
	/**
	 * @brief �R���e���c���X�g��ǂݍ���
	 * @note �����́u-�v��؂�ŁA�O�̒l��10���{�����l�ƌ��̒l�𑫂��āA�R���e���c�R�[�h�Ƃ���
	 * @note �Ⴆ�΁u2220-1�v��������R���e���c�R�[�h�́u222000001�v��Ԃ��B	[bug 8187]�Ή� 2012/04/17 ta_suga
	 * @param rContentsVec [out] �R���e���c�R�[�h��vector
	 * @retval true �ǂݍ��ݐ���
	 * @retval false �ǂݍ��ݎ��s
	 */
	bool readContentsList(std::vector<long>& rContentsVec);
	bool makeFacilPoint(LPCTSTR);
	bool makeCheckArea(LPCTSTR);
	int checkFacilPoint(void);
	bool makeAccessRoadList(void);
	bool showAccessRoad(int,CFacilPoint *);

	void setDBRoadStr(LPCTSTR cStr)			{	m_strRoadDB.Format("%s",cStr);	}		//(_T("SINDY2012@onyx(SDE.DEFAULT)"))
	void setDBPOIStr(LPCTSTR cStr)			{	m_strPOIDB.Format("%s",cStr);	}		//(_T("ANNOPOI2011B@coral2(SDE.DEFAULT)"))
	void setContentsListStr(LPCTSTR cStr)	{	m_strContentsList.Format("%s",cStr);	}	//(_T("NONAME"))
	void setLogFileStr(LPCTSTR cStr)		{	m_strLogFile.Format("%s",cStr);		}	//(_T("access_road_err.txt"))
	void setLogDirStr(LPCTSTR cStr)			{	m_mkFileDir.Format("%s",cStr);		}	//(_T(".\\log"))
	void setDistance(int cVal)				{ if( cVal>0 ) m_CheckDist = cVal;	}
	CString getLogDirStr(void)				{	return( m_mkFileDir );				}
	CString getLogFileStr(void)				{	return( m_strLogFile );				}
	FacilArray	getFacilArray(void)			{	return( m_FacilPoints );			}	//�{�݃|�C���g���
	void setDebugMode(void)					{	m_DebugMode = true;					}	//�f�B�o�O���[�h��ݒ�
	void setFacilListWrite(const bool bFlag){	m_FacilListWrite =	bFlag;			}	//�{�݃��X�g�������݃��[�h�ݒ� [bug 8187]�Ή� 2012/04/17 ta_suga

	bool			Init(void);
	void			Close(void);
	void			CheckStart(void);
	void			CheckStop(void);
	CComVariant		GetValue( const IFeaturePtr, const LPCTSTR);
	CComVariant		GetValue( const _IRowPtr, const LPCTSTR);
	void			GetValueStringWithDomain(const IFeaturePtr, LPCTSTR, CString* coString);
	bool			variantToDomainName(ICodedValueDomain*, const _variant_t&, BSTR*);
	long			SelectByShape(IGeometryPtr, IFeatureClassPtr, IFeatureCursor**, esriSpatialRelEnum);

	/**
	 * @brief WHERE��Ńt�B�[�`���Q������Ă���
	 * @note POI�|�C���g���擾����Ƃ��Ɏg�p����B�߂�l��vector��OBJECTID�Ń\�[�g�����	[bug 8187]�Ή� 2012/04/17 ta_suga
	 * @param r_ipFeatureClass [in] �Ώۂ̃t�B�[�`���N���X
	 * @param strWhereClause [in] WHERE��
	 * @param rFeatureVec [out] �t�B�[�`����vector
	 * @return �����Ɉ�v�����t�B�[�`���̐�
	 */
	long SelectByWhereClause(IFeatureClassPtr& r_ipFeatureClass, LPCTSTR strWhereClause, std::vector< CAdapt<IFeaturePtr> >& rFeatureVec ) const;
	/**
	 * @brief �t�B�[�`������WKSPoint���擾����
	 * @note ��A�̏�����1�s�Ŏ��s�������������ߍ쐬	[bug 8187]�Ή� 2012/04/17 ta_suga
	 * @param r_ipFeature [in] �Ώۂ̃t�B�[�`��
	 * @param rWKSPoint [out] �߂�l�Ƃ��Ă�WKSPoint
	 * @retval true �`��擾����
	 * @retval false �`��擾���s
	 */
	bool GetFeaturePoint(const IFeaturePtr& r_ipFeature, WKSPoint& rWKSPoint) const;
	/**
	 * @brief WKSPoint��1/256�b�ܓx�o�x�̃y�A�ɕϊ�
	 * @note �߂�l�̃y�A��first���o�x�Asecond���ܓx�B�@[bug 8187]�Ή� 2012/04/17 ta_suga
	 * @param pWKSPoint [out] �߂�l�Ƃ��Ă�WKSPoint
	 * @return 1/256�b�ܓx�o�x�̃y�A
	 */
	std::pair<int, int> getLL256(const WKSPoint& rWKSPoint) const;

	/**
	 * @brief �t�B�[�`���N���X�̃I�[�v��
	 * @note �I�[�v���Ɏ��s�����Ƃ��͕W���G���[�o�͂ɃG���[���o�͂���B�@[bug 8187]�Ή� 2012/04/17 ta_suga
	 * @param r_ipFeatureWorkspace [in] �t�B�[�`�����[�N�X�y�[�X
	 * @param strFeatureClassName [in] �t�B�[�`���N���X��
	 * @param ppFeatureClass [out] �t�B�[�`���N���X
	 * @retval true �ǂݍ��ݐ���
	 * @retval false �ǂݍ��ݎ��s
	 */
	bool OpenFeatureClass(const IFeatureWorkspacePtr& r_ipFeatureWorkspace, LPCTSTR strFeatureClassName, IFeatureClass** ppFeatureClass);

	/**
	 * @brief �e�[�u���̃I�[�v��
	 * @note �I�[�v���Ɏ��s�����Ƃ��͕W���G���[�o�͂ɃG���[���o�͂���B�@[bug 8187]�Ή� 2012/04/17 ta_suga
	 * @param r_ipFeatureWorkspace [in] �t�B�[�`�����[�N�X�y�[�X
	 * @param strTableName [in] �e�[�u����
	 * @param ppTable [out] �e�[�u��
	 * @retval true �ǂݍ��ݐ���
	 * @retval false �ǂݍ��ݎ��s
	 */
	bool OpenTable(const IFeatureWorkspacePtr& r_ipFeatureWorkspace, LPCTSTR strTableName, ITable** ppTable);

	bool			isNaviLink(const int,const IFeaturePtr);
	bool			isKendoClassLink(const IFeaturePtr);
	bool			isNaviRoadWidth(const int,const int);
	bool			isLinkQueRoute(IFeaturePtr);
	bool			isCrossSuikei(CFacilPoint *);
	bool			isTraceOverLink(IFeaturePtr, LinkArray &);
	int				CheckAccess(const IFeaturePtr piFeature, const IGeometryPtr piGeometry, LinkArray& rLink);
	int				LinkTrace(const IFeaturePtr,const IGeometryPtr,LinkArray &);
	bool			CheckConnectLink(const IPointPtr piFacilPoint, const IGeometryPtr piNearestGeometry, long cTracedLinkOID);

	//2005�N�x�Ȃ�G���[���肵�Ȃ�	Add 2005/8/2
	bool is_2005_L3Road(IFeaturePtr ipLinkFeature){
		using namespace sindy::schema;

		if( ipLinkFeature!= NULL ){
			CComVariant vaRoad_C = GetValue(ipLinkFeature,road_link::kRoadClass);
			if( vaRoad_C.intVal==road_link::road_class::kNarrowL3 ){
				CComVariant vaRoad_C = GetValue(ipLinkFeature,road_link::kAlleyMainte);
				if( vaRoad_C.intVal==road_link::alley_mainte::k2005 ){
					return true;
				}
			}
		}
		return false;
	}
	//�`�F�b�N�p���C���̍쐬
	bool CreateCheckLineShape(CFacilPoint *cFacil);

	//���肵�Ȃ�(��������)�����N���ǂ���  bug 7208 
	bool isIgnoreLink(_IRowPtr ipRow);

	//�ŋߖT�|�C���g�Ƃ̋������擾
	double GetNearestDist(const IGeometryPtr piTargetGeom, const IPointPtr piSorcePoint, IPointPtr& rNearestPoint);

	//�ʍs�֎~�����N���ǂ��� bug 7208
	bool isNopassLink(_IRowPtr ipRow);

	//����ʍs�����N���ǂ��� bug 7208
	bool isOnewayLink(int cOneway);

	//�ǐՂ̖��ɓ��B���������N������ʍs���ǂ����𔻒肵�A����ʍs�̏ꍇ��
	//����ʍs�ł͂Ȃ���{���H�ɓ��B����܂ŒǐՂ���
	int IsTraceSuccessLink(IFeaturePtr ipFeature, IGeometryPtr ipGeom, LinkArray& rLink);

// �����o�ϐ�
protected:
	CSHPHdl				m_SHPHdl;				//Shape SHP�n���h��
	CDBFHdl				m_DBFHdl;				//Shape DBF�n���h��

	IWorkspacePtr		m_ipWorkspace;
	IFeatureClassPtr	m_ipLinkFeatureClass;
	IFeatureClassPtr	m_ipNodeFeatureClass;
	IFeatureClassPtr	m_ipAdmFeatureClass;
	IFeatureClassPtr	m_ipBaseSiteFeatureClass;
	IFeatureClassPtr	m_ipBaseLineFeatureClass;
	ITablePtr			m_ipLQ_Route;
	ITablePtr			m_ipINF_Route;
	FILE				*m_Fp;					//�A�N�Z�X���H�`�F�b�N���ʃt�@�C���ւ̃|�C���^
	CString				m_strRoadDB;			//���H�̐ڑ�DB
	CString				m_strPOIDB;				//POI�̐ڑ�DB
	CString				m_strContentsList;		//�R���e���c���X�g�t�@�C��
	CString				m_strLogFile;			//�A�N�Z�X���H�G���[���O�t�@�C����
	CString				m_mkFileDir;			//�o�̓t�@�C���t�H���_��
	FacilArray			m_FacilPoints;			//�{�ݏ��|�C���g
	int					m_CheckDist;			//�A�N�Z�X���܂ł̋���
	bool				m_DebugMode;			//�f�B�o�O���[�h�i�A�N�Z�X���H�����t�@�C���o�́j
	bool				m_FacilListWrite;		//�{�ݏ�񃊃X�g(access_list.xml)���o�͂���t���O
};

#endif // !defined(AFX_DOC_H__0E3F8A46_D0F3_4183_98D3_BFDA6E880BCA__INCLUDED_)
