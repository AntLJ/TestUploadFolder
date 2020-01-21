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

// CrdConvert.cpp : CCrdConvert �̎���

#include "stdafx.h"
#include "CrdConvert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ���b�V�� + (X,Y)����o�ܓx�֕ϊ�
STDMETHODIMP CCrdConvert::MeshtoLL(LONG mesh, LONG x, LONG y, DOUBLE* lon, DOUBLE* lat)
{
	if ( ! lon || ! lat )
		return E_POINTER;

	m_cnv.MeshtoLL( mesh, x, y, lon, lat );

	return S_OK;
}

// �o�ܓx����A���b�V���R�[�h + (x,y) �����߂�
STDMETHODIMP CCrdConvert::LLtoMesh(DOUBLE lon, DOUBLE lat, LONG level, LONG* mesh, LONG* x, LONG* y)
{
	if ( ! mesh || ! x || ! y )
		return E_POINTER;

	m_cnv.LLtoMesh( lon, lat, level, (int*)mesh, (int*)x, (int*)y );

	return S_OK;
}

// ���_���b�V������(x,y)���b�V�����ꂽ���̃��b�V���R�[�h�����߂�
STDMETHODIMP CCrdConvert::GetMeshCode(LONG code, LONG xdist, LONG ydist, LONG* mesh)
{
	if ( ! mesh )
		return E_POINTER;

	(*mesh) = m_cnv.GetMeshCode( code, xdist, ydist );

	return S_OK;
}

// �Q�n�_�̃��b�V��+(x,y)����A�Q�_�Ԃ̋��������߂�i�P��: m) 
STDMETHODIMP CCrdConvert::GetMeshDist(LONG mesh1, LONG x1, LONG y1, LONG mesh2, LONG x2, LONG y2, DOUBLE* dist)
{
	if ( ! dist )
		return E_POINTER;

	(*dist) = m_cnv.GetMeshDist( mesh1, x1, y1, mesh2, x2, y2 );

	return S_OK;
}

// Tokyo->JGD2000�ւ̍��W�ϊ�
STDMETHODIMP CCrdConvert::TOKYOtoJGD2000(DOUBLE before_lon, DOUBLE before_lat, DOUBLE before_he, DOUBLE* after_lon, DOUBLE* after_lat, DOUBLE* after_he)
{
	if ( ! after_lon || ! after_lat || ! after_he )
		return E_POINTER;

	m_cnv.TOKYOtoJGD2000(before_lon, before_lat, before_he, after_lon, after_lat, after_he);

	return S_OK;
}

// JGD2000->Tokyo�ւ̍��W�ϊ�
STDMETHODIMP CCrdConvert::JGD2000toTOKYO(DOUBLE before_lon, DOUBLE before_lat, DOUBLE before_he, DOUBLE* after_lon, DOUBLE* after_lat, DOUBLE* after_he)
{
	if ( ! after_lon || ! after_lat || ! after_he )
		return E_POINTER;

	m_cnv.JGD2000toTOKYO(before_lon, before_lat, before_he, after_lon, after_lat, after_he);

	return S_OK;
}

// �p�����[�^�t�@�C���ǂݍ���
STDMETHODIMP CCrdConvert::JGD2000_RP_Init(BSTR bstrFile, VARIANT_BOOL* bRet)
{
	if ( ! bRet || ! bstrFile)
		return E_POINTER;

	(*bRet) = m_cnv.JGD2000_RP_Init(CT2CA(CString(bstrFile))) ? VARIANT_TRUE : VARIANT_FALSE;

	return S_OK;
}

// Tokyo->JGD2000�ւ̍��W�ϊ�
STDMETHODIMP CCrdConvert::TOKYOtoJGD2000_RP(DOUBLE before_lon, DOUBLE before_lat, DOUBLE* after_lon, DOUBLE* after_lat, VARIANT_BOOL biPCFlag, VARIANT_BOOL* bRet)
{
	if ( ! bRet || ! after_lon || ! after_lat )
		return E_POINTER;

	(*bRet) = VARIANT_TRUE;
	if ( 0 == m_cnv.TOKYOtoJGD2000_RP(before_lon, before_lat, after_lon, after_lat, (biPCFlag == VARIANT_TRUE)) )
		(*bRet) = VARIANT_FALSE;

	return S_OK;
}

// JGD2000->Tokyo�ւ̍��W�ϊ�
STDMETHODIMP CCrdConvert::JGD2000toTOKYO_RP(DOUBLE before_lon, DOUBLE before_lat, DOUBLE* after_lon, DOUBLE* after_lat, VARIANT_BOOL biPCFlag, VARIANT_BOOL* bRet)
{
	if ( ! bRet || ! after_lon || ! after_lat )
		return E_POINTER;

	(*bRet) = VARIANT_TRUE;
	if ( 0 == m_cnv.JGD2000toTOKYO_RP(before_lon, before_lat, after_lon, after_lat, (biPCFlag == VARIANT_TRUE)) )
		(*bRet) = VARIANT_FALSE;

	return S_OK;
}

// 19���W�n->�o�ܓx�ւ̍��W�ϊ�
STDMETHODIMP CCrdConvert::XY19toLL(INT axis, DOUBLE before_x, DOUBLE before_y, DOUBLE* after_lon, DOUBLE* after_lat)
{
	if ( ! after_lon || ! after_lat )
		return E_POINTER;

	m_cnv.XY19toLL(axis, before_x, before_y, after_lon, after_lat);

	return S_OK;
}

// �o�ܓx->19���W�n�ւ̍��W�ϊ�
STDMETHODIMP CCrdConvert::LLtoXY19(INT axis, DOUBLE before_lon, DOUBLE before_lat, DOUBLE* after_x, DOUBLE* after_y)
{
	if ( ! after_x || ! after_y )
		return E_POINTER;

	m_cnv.LLtoXY19(axis, before_lon, before_lat, after_x, after_y);

	return S_OK;
}
