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

#ifndef CRD_CNV_DEFINES_H_
#define CRD_CNV_DEFINES_H_

// �萔��`
#define		RA			6377397.15500		///< �n���ȉ~��(�x�b�Z��)�̒����a�i���[�g���j
#define		RB			6356078.96300		///< �n���ȉ~��(�x�b�Z��)�̒Z���a�i���[�g���j
#define		EA2			0.006674372231315	///< ��ꗣ�S����2��
#define		EB2			0.006719218798677	///< ��񗣐S����2��	
#define		M0			0.9999				///< �����o���ł̏k�ڌW��
/** C1�`C6�͎q�ߐ��ǒ������߂�ۂɎg����p�����[�^ */
#define		C1			1.005037306048555	///< 
#define		C2			0.005047849240300
#define		C3			0.000010563786831
#define		C4			0.000000020633322
#define		C5			0.000000000038853
#define		C6			0.000000000000070
// �K�E�X���W�n�p
#define		G_A			1.0050373060874
#define		G_B			0.002523924637
#define		G_C			0.0000026409446
#define		G_D			0.000000003438362
// WGS84�ϊ��p
#define		DX			582.0
#define		DY			105.0
#define		DZ			414.0
#define		ALFA		-5.04206228354e-6
#define		BETA		-1.6968478839e-6
#define		GAMMA		1.4932261378e-5
#define		DELTA		8.3e-6
// WGS84�ȉ~��
#define		WGS84_RA	6378137.00000		///< �n���ȉ~��(WGS84)�̒����a�i���[�g���j2000.10.11
#define		WGS84_RB	6356752.31424		///< �n���ȉ~��(WGS84)�̒Z���a�i���[�g���j2000.10.11
#define		WGS84_EA2	0.006694379990132
#define		WGS84_EB2	0.006739496742267
// Tokyo->JGD2000�ϊ��Ŏg�p
#define		GRS80_RA	6378137.0			///< GRS80�ȉ~�� 2008.01.22
#define		GRS80_INV_F		298.257222101	///< GRS80�ȉ~�̝̂G�����̋t�� 2008.01.23
#define		BESSEL_INV_F	299.152813		///< �x�b�Z���ȉ~�̝̂G�����̋t�� 2008.01.23
#define		GRS80_EA2		pow((sqrt(2.0 * GRS80_INV_F - 1.0) / GRS80_INV_F), 2.0)	///< ��ꗣ�S����2��
#define		GRS80_EB2		pow((sqrt(2.0 * GRS80_INV_F - 1.0) / (GRS80_INV_F - 1.0)), 2.0)	///< ��񗣐S����2��
// ����
#define		D_MILLION	1000000.0
#define		MILLION		1000000

#endif // CRD_CNV_DEFINES_H_
