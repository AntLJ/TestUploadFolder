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

// Diff.h: Diff �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIFF_H__7CF4D5FF_FEA5_4B86_BB97_099D418DD03F__INCLUDED_)
#define AFX_DIFF_H__7CF4D5FF_FEA5_4B86_BB97_099D418DD03F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef enum MyDataTypes{
	TYPE_CITY,
	TYPE_BASE,
	TYPE_CITY_ANNO,
	TYPE_CITY_DISP,
	TYPE_BASE_ANNO,
	TYPE_BASE_DISP,
	TYPE_MIDDLE_ANNO,
	TYPE_MIDDLE_DISP,
	TYPE_ROAD,
	TYPE_BUILD,
};

enum MyErrorLevel {
	ERRTYPE_INFO,
	ERRTYPE_WARNING,
	ERRTYPE_ERROR
};

struct INDEX_PAIR {
	LONG m_OldIndex;
	LONG m_NewIndex;
};
typedef std::list<INDEX_PAIR> INDEX_LIST;

struct LAYER_TABLE {
	LPCTSTR m_LayerName;
	MyDataTypes m_DataType;
	IFeatureClassPtr m_ipNewFeatureClass;
	IFeatureClassPtr m_ipOldFeatureClass;
	INDEX_LIST m_AttrIndex;
	LAYER_TABLE(LPCTSTR cLayerName, MyDataTypes cDataType){
		m_LayerName = cLayerName;
		m_DataType = cDataType;
		m_ipNewFeatureClass = NULL;
		m_ipOldFeatureClass = NULL;
		m_AttrIndex.clear();
	}
};

struct FEATURE_SET {
	LONG m_ObjID;
	IFeaturePtr m_ipFeature;
	FEATURE_SET(){
	}
	FEATURE_SET(LONG cObjID){
		m_ObjID = cObjID;
	}
	bool operator<(const FEATURE_SET &Cmp) const{
		return m_ObjID < Cmp.m_ObjID;
	}
	bool operator<(const LONG &cObjID) const{
		return m_ObjID < cObjID;
	}
	bool operator==(const LONG &cObjID) const{
		return m_ObjID == cObjID;
	}
};

struct CLAIM_INFO {
	LONG m_ObjID;
	CString m_Msg;
};

typedef std::set<FEATURE_SET> FEATURE_TABLE;
typedef std::set<LONG> ID_SET;

class Diff  
{
public:
	bool readExtFile(LPCTSTR cListFile);
	bool execCheckDiff();

	/**
	 * @brief ������
	 * @param pFile	[in]	���O�o�̓t�@�C��
	 * @param ipMapWorkspace	[in]	���n�}�T�[�o
	 * @param ipOldMapWorkspace	[in]	���n�}�T�[�o
	 * @param ipAnoWorkspace	[in]	�����L�T�[�o
	 * @param ipOldAnoWorkspace	[in]	�����L�T�[�o
	 * @param ipBasWorkspace	[in]	�����k�T�[�o
	 * @param ipOldBasWorkspace	[in]	�����k�T�[�o
	 * @param ipRodWorkspace	[in]	�����H�T�[�o
	 * @param ipOldRodWorkspace	[in]	�����H�T�[�o
	 * @param ipBldWorkspace	[in]	�������T�[�o
	 * @param ipOldBldWorkspace	[in]	�������T�[�o
	 * @param ipClmWorkspace	[in]	�N���[���|���S���̐ڑ���
	 * @param setCheckFeatures	[in]	��r�Ώۃt�B�[�`��
	 * @param bOutputAll        [in]    �Ώ۔͈͓��̃I�u�W�F�N�g�̍�������S�ďo�� Bug 10390]�����̂������S�ẴI�u�W�F�N�g�̍�����񂪏o�͂����I�v�V�������~����
	 * @note AK�Č��Ή���A�������͔�r�ΏۃI�u�W�F�N�g�݂̂����s��Ȃ��悤�ɂ���
	 * @retval true	����
	 * @retval false	���s
	 */
	bool init(FILE *pFile, const IWorkspacePtr& ipMapWorkspace, const IWorkspacePtr& ipOldMapWorkspace,
		const IWorkspacePtr& ipAnoWorkspace, const IWorkspacePtr& ipOldAnoWorkspace,const IWorkspacePtr& ipBasWorkspace,
		const IWorkspacePtr& ipOldBasWorkspace, const IWorkspacePtr& ipRodWorkspace, const IWorkspacePtr& ipOldRodWorkspace,
		const IWorkspacePtr& ipBldWorkspace, const IWorkspacePtr& ipOldBldWorkspace, const IWorkspacePtr& ipClmWorkspace, const std::set<CString>& setCheckFeatures, bool bOutputAll);
	Diff();
	virtual ~Diff();
protected:

	CString m_LayerName;
	bool m_ClaimNG;
	bool fnFeatureCompareDisp(IFeaturePtr ipFeature, LONG cIndex, LPCTSTR cAnnotationLayer);
	LONG fnFindTableIndex(LPCTSTR cLayerName);
	bool fnFeatureCompareAnno(IFeaturePtr ipFeature, LONG cIndex, LPCTSTR cDispLayer);
	bool fnCompareRoad(FEATURE_TABLE &cOldTable, FEATURE_TABLE &cNewTable, IGeometryPtr ipGeom, LONG cIndex);
	bool fnFeatureCompareRoad(IFeaturePtr ipOldFeature, IFeaturePtr ipNewFeature, IGeometryPtr ipGeom, LONG cIndex);
	LONG m_OldRoadIndex;
	LONG m_NewRoadIndex;
	void fnMsgOut(MyErrorLevel cErrLevel, LPCTSTR cMsg);
	bool fnGetClaimInfo(IFeaturePtr ipFeature);
	CLAIM_INFO m_ClaimInfo;
	LONG m_ClaimNoIndex;
	bool fnGeometryCompare(IGeometryPtr ipOldGeom, IGeometryPtr ipNewGeom, esriGeometryType cType);
	bool fnFeatureCompareNormal(IFeaturePtr ipOldFeature, IFeaturePtr ipNewFeature, IGeometryPtr ipGeom, LONG cIndex);
	bool fnCompareNormal(FEATURE_TABLE &cOldTable, FEATURE_TABLE &cNewTable, IGeometryPtr ipGeom, LONG cIndex);
	bool fnCheckDiffNormal(IFeaturePtr ipFeature, LONG cIndex);
	bool fnCheckDiffAll(IFeaturePtr ipFeature);
	FILE *m_pFile;
	IFeatureClassPtr m_ipClaim;
	ID_SET m_ExpID;
	static LAYER_TABLE m_LayerTable[];
	double m_BuffRadius;
	bool m_IsPoint;
	bool m_bOutputAll;	// ���������ׂďo�͂��邩�ǂ��� [Bug 10390]�����̂������S�ẴI�u�W�F�N�g�̍�����񂪏o�͂����I�v�V�������~����
	std::set<CString> m_CheckFeatures; //!< ��r�Ώۃt�B�[�`����(�啶��)
	/**
	 * @brief ���ϐ����擾����
	 * @note �uFC_CLAIMPOLY_MAP�v,�uFD_CLAIM_NO�v,�uRADIUS�v�͊��ϐ��̐ݒ�łȂ���Ύ擾�ł��Ȃ��̂Œ���
	 *       �ݒ�t�@�C������ǂݍ��߂�悤�ɂ����ق���������������Ȃ�
	 */
	void fnGetEnv(void);
private:
	double m_DiffX;
	double m_DiffY;
};

#endif // !defined(AFX_DIFF_H__7CF4D5FF_FEA5_4B86_BB97_099D418DD03F__INCLUDED_)
