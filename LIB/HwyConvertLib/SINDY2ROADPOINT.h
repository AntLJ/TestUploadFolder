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

// SINDY2ROADPOINT.h: SINDY2ROADPOINT �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDY2ROADPOINT_H__42268805_C56C_44B7_8181_34D96A3EC8B1__INCLUDED_)
#define AFX_SINDY2ROADPOINT_H__42268805_C56C_44B7_8181_34D96A3EC8B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SINDY2INHWY.h"
#include "Customer.h"
#include "FacilNameChangeTable.h"

// roadpoint.txt�̂P���R�[�h�̏����i�[����\����
struct RoadPoint
{
	bool				eHwyModeFlag;				/// �n�C�E�F�C���[�h�t���O

	long				ePrefMesh;					/// ��\�|�C���g2�����b�V���R�[�h
	long				ePrefX;						/// ��\�|�C���g���K��X���W
	long				ePrefY;						/// ��\�|�C���g���K��Y���W
	long				eAccessPointID;				/// �擾��Access_Point��ObjectID�i���b�V�����V�[�P���X�ԍ��A�{�݃|�C���gID�j
	long				eMesh;						/// �{�݃|�C���g2�����b�V���R�[�h
	long				eX;							/// �{�݃|�C���g���K��X���W
	long				eY;							/// �{�݃|�C���g���K��Y���W
	long				eAdmin;						/// �s�撬���R�[�h
	long				eFacilInfoPointAdmin;		/// �{�ݑ�\�|�C���g�̎s�撬���R�[�h
	long				eFacilInfoPointID;			/// ������{�݂�Facil_Info_Point��ObjectID�i�{��ID�j
	long				eSeq;						/// �H�����V�[�P���X�ԍ�
	std::string			eName;						/// ���H�{�ݖ���
	std::string			eYomi;						/// �{�ݖ��̓ǂ�
	long				eRoadCode;					/// �H���R�[�h
	long				eFacilClassAndInOutCode;	/// �{�ݎ�ʁ{�o�����R�[�h
	long				eFacilClass;				/// �{�ݎ�ʃR�[�h
	long				eInOut;						/// �o�����R�[�h
	long				ePntPos;					/// �|�C���g�ʒu�R�[�h
	std::string			ePntName;					/// �|�C���g�ʒu����
	std::string			ePntYomi;					/// �|�C���g�ʒu�ǂ�
	std::vector<long>	eSapaInfo;					/// SA/PA���i25���j
	long				eOriginal;					/// �Ǝ����̃t���O
	long				eDummyFacil;				/// �_�~�[�{�ݏ��t���O

	// �ȉ���SiRiUS�p
	long				eSearchClass;				/// �����p�{�ݎ�ʃR�[�h
	long				ePrefMeshForSirius;			/// ��\�|�C���g2�����b�V���R�[�h
	long				ePrefXForSirius;			/// ��\�|�C���g���K��X���W
	long				ePrefYForSirius;			/// ��\�|�C���g���K��Y���W
	long				eDirection;					/// �㉺����ʃR�[�h [Bug6428]�Œǉ�

	RoadPoint();
	virtual ~RoadPoint() {}
};

// roadpoint.txt�̃��R�[�h���A�H���R�[�h�{�H�����V�[�P���X�ԍ�����ɕ��ׂ邽�߂̃t�@���N�^
struct lessRoadPoint : public std::binary_function<RoadPoint,RoadPoint,bool>
{
	bool operator ()(const RoadPoint &rp1, const RoadPoint &rp2) const
	{
		// �H�����{�݈ʒu�R�[�h�Ƃ͕ʂɊi�[�����H���R�[�h�ƘH�����V�[�P���X�ԍ�����Ƀ\�[�g����悤�ɏC��
		return (rp1.eRoadCode != rp2.eRoadCode) ? rp1.eRoadCode < rp2.eRoadCode : rp1.eSeq < rp2.eSeq;
	}
};

typedef std::multiset<RoadPoint,lessRoadPoint> RoadPointArray;


class SINDY2ROADPOINT : public SINDY2INHWY  
{
public:
	SINDY2ROADPOINT(SINDY2INHWYINFO *cHwyInfo);
	virtual ~SINDY2ROADPOINT();

	virtual HRESULT Convert(void);
	virtual HRESULT Write			(std::string cFileName, bool cHwyModeFlag, long cFormat, long cJctDataType);
	HRESULT makeJctBranchData(void);

	/**
	 * @brief �{�ݖ��̕ϊ��i�d������w��j
	 * @note	�{�ݖ��̕ϊ����s�v�Ȏd�����悪�w�肳�ꂽ�ꍇ�͉������Ȃ��B
	 * @param	cCustomer	[in]	�d������
	 * @param	cpFacilNameChangeTable	[in]	�{�ݖ��̕ϊ��e�[�u��
	 */
	void changeFacilName( Customer cCustomer, FacilNameChangeTable* cpFacilNameChangeTable );

	/**
	 * @brief �{�ݖ��̕ϊ�
	 * @param	cpFacilNameChangeTable	[in]	�{�ݖ��̕ϊ��e�[�u��
	 */
	void changeFacilName( FacilNameChangeTable* cpFacilNameChangeTable );

private:
	virtual HRESULT Write			(RoadPointArray* cpData, std::string cFileName, bool cHwyModeFlag, long cFormat);
	virtual HRESULT WriteForSirius	(RoadPointArray* cpData, std::string cFileName, long cFormat);
	virtual HRESULT WriteAll		(std::string cFileName);
	void changeFacilName(FacilNameChangeTable* cpFacilNameChangeTable, RoadPointArray& crData, std::string cFileName);

public:
	static HRESULT GetHwyModeFlag				(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, bool *cTarget);
	static HRESULT GetPrefPoint					(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, long *cMesh, long *cX, long *cY);
	static HRESULT GetPrefPointForSirius		(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, long *cMesh, long *cX, long *cY);
	static HRESULT GetMeshSequence				(IFeaturePtr ipAccessPoint, long *cMeshSeq);
	static HRESULT GetBaseMeshCoord				(IFeaturePtr ipAccessPoint, long *cMesh, long *cX, long *cY);
	static HRESULT GetBaseAdmin					(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, long *cAdminCode);
	static HRESULT GetFacilInfoPointAdmin		(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, long *cAdminCode);
	static HRESULT GetFacilInfoPointID			(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, long *cID);
	static HRESULT GetSequence					(IFeaturePtr ipAccessPoint, long *cSequence);
	static HRESULT GetSearchClass				(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, long *cSearchClass);
	static HRESULT GetRoadCode					(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, long *cRoadCode);
	static HRESULT GetFacilClassAndInOutCode	(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, long *cFacilClass);
	static HRESULT GetPntPos					(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, long *cPntPos, std::set<long> *cWasteID = NULL);
	static HRESULT GetName						(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, std::string *cName, std::string *cYomi);
	static HRESULT GetPntName					(IFeaturePtr ipAccessPoint, long cPntPos, std::string *cName, std::string *cYomi);
	static HRESULT GetSapaInfo					(IFeaturePtr ipAccessPoint, std::vector<long> *cSapaInfo);
	static HRESULT GetOriginalFlag				(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, long *cOriginal);
	static HRESULT GetDummyFacilFlag			(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, long *cDummyFacilFlag);

private:
	RoadPointArray mRoadPointArray;			// jct_interflow�̃f�[�^�i���roadpoint.txt�p�j
	RoadPointArray mRoadPointBranchArray;	// jct_branch�̃f�[�^�i���roadpoint.bunnki.txt�p�j

public:
	typedef RoadPointArray::const_iterator Iter;

	/**
	 * @brief �f�[�^��̐擪���w��const_iterator��Ԃ�
	 * @param[in]	isBranch	true�Ȃ�jct_branch�̃f�[�^�Afalse�Ȃ�ʏ�ijct_interflow�j�̃f�[�^������
	 */
	Iter begin( bool isBranch = false ) const { return isBranch ? mRoadPointBranchArray.begin() : mRoadPointArray.begin(); }

	/**
	 * @brief �f�[�^��̖������w��const_iterator��Ԃ�
	 * @param[in]	isBranch	true�Ȃ�jct_branch�̃f�[�^�Afalse�Ȃ�ʏ�ijct_interflow�j�̃f�[�^������
	 */
	Iter end( bool isBranch = false ) const { return isBranch ? mRoadPointBranchArray.end() : mRoadPointArray.end(); }

	/**
	 * @brief �f�[�^��̃T�C�Y��Ԃ�
	 * @param[in]	isBranch	true�Ȃ�jct_branch�̃f�[�^�Afalse�Ȃ�ʏ�ijct_interflow�j�̃f�[�^������
	 */
	std::size_t size( bool isBranch = false ) const { return isBranch ? mRoadPointBranchArray.size() : mRoadPointArray.size(); }
};

#endif // !defined(AFX_SINDY2ROADPOINT_H__42268805_C56C_44B7_8181_34D96A3EC8B1__INCLUDED_)
