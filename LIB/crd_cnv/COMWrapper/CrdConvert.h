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

// CrdConvert.h : CCrdConvert �̐錾

#pragma once
#include "resource.h"       // ���C�� �V���{��

#include "crdcnvLib_i.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "DCOM �̊��S�T�|�[�g���܂�ł��Ȃ� Windows Mobile �v���b�g�t�H�[���̂悤�� Windows CE �v���b�g�t�H�[���ł́A�P��X���b�h COM �I�u�W�F�N�g�͐������T�|�[�g����Ă��܂���BATL ���P��X���b�h COM �I�u�W�F�N�g�̍쐬���T�|�[�g���邱�ƁA����т��̒P��X���b�h COM �I�u�W�F�N�g�̎����̎g�p�������邱�Ƃ���������ɂ́A_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ���`���Ă��������B���g�p�� rgs �t�@�C���̃X���b�h ���f���� 'Free' �ɐݒ肳��Ă���ADCOM Windows CE �ȊO�̃v���b�g�t�H�[���ŃT�|�[�g�����B��̃X���b�h ���f���Ɛݒ肳��Ă��܂����B"
#endif

/**
* @class crd_cnv��VB.NET�Ŏg�p���邽�߂�COM���b�p
*/
class ATL_NO_VTABLE CCrdConvert :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCrdConvert, &CLSID_CrdConvert>,
	public ICoordConverter3
{
public:
	CCrdConvert()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_CRDCONVERT)

BEGIN_COM_MAP(CCrdConvert)
	COM_INTERFACE_ENTRY(ICoordConverter)
	COM_INTERFACE_ENTRY(ICoordConverter2)
	COM_INTERFACE_ENTRY(ICoordConverter3)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	/**
	* @brief ���b�V�� + (X,Y)����o�ܓx�֕ϊ�
	* @param mesh [in] ���b�V���R�[�h
	* @param x [in] X���W
	* @param y [in] Y���W
	* @param lon [out] �ܓx
	* @param lat [out] �o�x
	*/
	STDMETHOD(MeshtoLL)(LONG mesh, LONG x, LONG y, DOUBLE* lon, DOUBLE* lat);

	/**
	* @brief �o�ܓx����A���b�V���R�[�h + (x,y) �����߂�
	* @param lon [in] �ܓx
	* @param lat [in] �o�x
	* @param level [in] �ϊ����x��
	* @param mesh [out] ���b�V���R�[�h
	* @param x [out] X���W
	* @param y [out] Y���W
	*/
	STDMETHOD(LLtoMesh)(DOUBLE lon, DOUBLE lat, LONG level, LONG* mesh, LONG* x, LONG* y);

	/**
	* @brief ���_���b�V������(x,y)���b�V�����ꂽ���̃��b�V���R�[�h�����߂�
	* @param code [in] ���b�V���R�[�h
	* @param xdist [in] X�l
	* @param ydist [in] Y�l
	* @param mesh [out] ���b�V���R�[�h
	*/
	STDMETHOD(GetMeshCode)(LONG code, LONG xdist, LONG ydist, LONG* mesh);

	/**
	* @brief �Q�n�_�̃��b�V��+(x,y)����A�Q�_�Ԃ̋��������߂�i�P��: m) 
	* @param mesh1 [in] ���b�V���R�[�h
	* @param x1 [in] X���W
	* @param y1 [in] Y���W
	* @param mesh2 [in] ���b�V���R�[�h
	* @param x2 [in] X���W
	* @param y2 [in] Y���W
	* @param dist [out] �Q�_�Ԃ̋���
	*/
	STDMETHOD(GetMeshDist)(LONG mesh1, LONG x1, LONG y1, LONG mesh2, LONG x2, LONG y2, DOUBLE* dist);

	/**
	* @brief Tokyo->JGD2000�ւ̍��W�ϊ�
	* @param before_lon [in] �ϊ��O�o�x
	* @param before_lat [in] �ϊ��O�ܓx
	* @param before_he [in] �ȉ~�̍�(m)
	* @param after_lon [out] �ϊ���o�x
	* @param after_lat [out] �ϊ���ܓx
	* @param after_he [out] �ȉ~�̍�(m)
	*/
	STDMETHOD(TOKYOtoJGD2000)(DOUBLE before_lon, DOUBLE before_lat, DOUBLE before_he, DOUBLE* after_lon, DOUBLE* after_lat, DOUBLE* after_he);

	/**
	* @brief JGD2000->Tokyo�ւ̍��W�ϊ�
	* @param before_lon [in] �ϊ��O�o�x
	* @param before_lat [in] �ϊ��O�ܓx
	* @param before_he [in] �ȉ~�̍�(m)
	* @param after_lon [out] �ϊ���o�x
	* @param after_lat [out] �ϊ���ܓx
	* @param after_he [out] �ȉ~�̍�(m)
	*/
	STDMETHOD(JGD2000toTOKYO)(DOUBLE before_lon, DOUBLE before_lat, DOUBLE before_he, DOUBLE* after_lon, DOUBLE* after_lat, DOUBLE* after_he);

	/**
	* @brief �p�����[�^�t�@�C���ǂݍ���
	* @param bstrFile [in] �p�����[�^�t�@�C���p�X
	* @param bRet [out] �ǂݍ��݂������������ǂ���
	*/
	STDMETHOD(JGD2000_RP_Init)(BSTR bstrFile, VARIANT_BOOL* bRet);

	/**
	* @brief Tokyo->JGD2000�ւ̍��W�ϊ�
	* @param before_lon [in] �ϊ��O�o�x
	* @param before_lat [in] �ϊ��O�ܓx
	* @param after_lon [out] �ϊ���o�x
	* @param after_lat [out] �ϊ���ܓx
	* @param biPCFlag [in] IPC�ŗL�ϊ������邩�ǂ���
	* @param bRet [out] �p�����[�^���ǂݍ��܂ꂽ���ǂ���
	*/
	STDMETHOD(TOKYOtoJGD2000_RP)(DOUBLE before_lon, DOUBLE before_lat, DOUBLE* after_lon, DOUBLE* after_lat, VARIANT_BOOL biPCFlag, VARIANT_BOOL* bRet);

	/**
	* @brief JGD2000->Tokyo�ւ̍��W�ϊ�
	* @param before_lon [in] �ϊ��O�o�x
	* @param before_lat [in] �ϊ��O�ܓx
	* @param after_lon [out] �ϊ���o�x
	* @param after_lat [out] �ϊ���ܓx
	* @param biPCFlag [in] IPC�ŗL�ϊ������邩�ǂ���
	* @param bRet [out] �p�����[�^���ǂݍ��܂ꂽ���ǂ���
	*/
	STDMETHOD(JGD2000toTOKYO_RP)(DOUBLE before_lon, DOUBLE before_lat, DOUBLE* after_lon, DOUBLE* after_lat, VARIANT_BOOL biPCFlag, VARIANT_BOOL* bRet);

		/**
	* @brief 19���W�n->�o�ܓx�ւ̍��W�ϊ�
	* @param axis [in] �Y��������W�n�̔ԍ�
	* @param before_x [in] �ϊ��O��19���W�n��X(�ܓx����)
	* @param before_y [in] �ϊ��O��19���W�n��Y(�o�x����)
	* @param after_lon [out] �ϊ���o�x
	* @param after_lat [out] �ϊ���ܓx
	*/
	STDMETHOD(XY19toLL)(INT axis, DOUBLE before_x, DOUBLE before_y, DOUBLE* after_lon, DOUBLE* after_lat);

	/**
	* @brief �o�ܓx->19���W�n�ւ̍��W�ϊ�
	* @param axis [in] �Y��������W�n�̔ԍ�
	* @param before_lon [in] �ϊ��O�o�x
	* @param before_lat [in] �ϊ��O�ܓx
	* @param after_x [out] �ϊ����19���W�n��X(�ܓx����)
	* @param after_y [out] �ϊ����19���W�n��Y(�o�x����)
	*/
	STDMETHOD(LLtoXY19)(INT axis, DOUBLE before_lon, DOUBLE before_lat, DOUBLE* after_x, DOUBLE* after_y);

private:
	crd_cnv m_cnv;           ///!< crd_cnv

};

OBJECT_ENTRY_AUTO(__uuidof(CrdConvert), CCrdConvert)
