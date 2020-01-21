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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "crd_cnv.h"
#include "crd_cnv_defines.h"

//---------------------------------------------------------------------
// IPC��Tokyo97�G���A�̃��b�V�����ǂ����𒲂ׂ�
//	@return	Tokyo97�G���A�ȊO == false
//	@return	Tokyo97�G���A == true
//---------------------------------------------------------------------
bool	crd_cnv::
Is_IPC_Tokyo97_Area(	int		c_iMeshCode	)	///< 2�����b�V���R�[�h
{
	std::set<int, std::less<int> >::iterator	a_iIndx;
	a_iIndx	= m_siTokyo97_Area.find ( c_iMeshCode );	// 2�����b�V���R�[�h�����Ċm�F
	if( a_iIndx != m_siTokyo97_Area.end() ) {
		return	( true );
	}
	return	( false );
}

//---------------------------------------------------------------------
// JGD2000�p�����[�^�t�@�C����ǂݍ��݁A�Z�b�g�̍\���ɗ��Ƃ�
//	@return	bool
//---------------------------------------------------------------------
bool	crd_cnv::
JGD2000_RP_Init( char*			c_fname )			///< �ǂݍ��݃t�@�C����
{
	const int	LINEMAX	= 512;
	FILE	*rfp	= NULL;
	char	r_buf[LINEMAX];
	int		max_field	= 0;	// 2000.11.30

	// �t�@�C���I�[�v��
	rfp		= fopen(c_fname, "r");
	if(rfp == NULL)
	{
		fprintf(stderr, "�t�@�C���I�[�v���G���[ <%s> �I�I\n", c_fname);
		return	( false );
	}

	memset(r_buf, '\0', LINEMAX);	// �o�b�t�@�̏�����

	int	a_iCurLine	= 0;
	for(; fgets(r_buf, LINEMAX, rfp) != NULL; a_iCurLine++)
	{
		param_info	r_data;		// �ǂݍ��݃f�[�^

		if( a_iCurLine < 2 ) {	// TKY2JGD.par�̐擪2�s���΂�
			continue;
		}

		if( r_buf[0] == '#' || r_buf[0] == '\n' ) {
			continue;
		}

		int	a_ilen	= strlen(r_buf);
		if( r_buf[a_ilen-1] == '\n' ) {
			r_buf[a_ilen-1] = '\0';
		}

		char	*delim		= "\t, ";		// �^�u�A�R���}�A�X�y�[�X��؂�
		char	*field[3]	= { NULL, NULL,	NULL };	// �t�B�[���h�̕�����̃|�C���^

		field[0]	= strtok(r_buf, delim);	// ���W�n���
		field[1]	= strtok(NULL, delim);		// ���W���P(���b�V���R�[�h)
		field[2]	= strtok(NULL, delim);		// ���W���Q(X)

		// �t�B�[���h�̓ǂݎ��`�F�b�N
		int	i;
		for(i = 0; i < 3; i++)
		{
			if(field[i] == NULL) {
				return	( false );
			}
		}

		r_data.e_imeshcode	= atoi ( field[0] );
		r_data.e_dDlat		= atof ( field[1] );
		r_data.e_dDlon		= atof ( field[2] );

		// �b����x�ɕϊ����Ă���
		double	a_dTmp;
		a_dTmp	= r_data.e_dDlat / 3600.0;
		r_data.e_dDlat	= a_dTmp;

		a_dTmp	= r_data.e_dDlon / 3600.0;
		r_data.e_dDlon	= a_dTmp;

		m_spParam.insert(r_data);	// �f�[�^�i�[

		memset(r_buf, '\0', LINEMAX);	// �o�b�t�@�̏�����
	}
	fclose	( rfp );
	m_bJGD2000_RP_Inited	= true;	// �������ς�
	return	( true );
}

//---------------------------------------------------------------------
// �o�ܓx����A3�����b�V���R�[�h + (x,y) �����߂�
//	@return	����
//---------------------------------------------------------------------
void	crd_cnv::
LLto3rdMesh(	double	lon,			///< �o�x
				double	lat,			///< �ܓx
				int		*meshcode,		///< ���b�V���R�[�h
				int		*x,				///< ���K���w
				int		*y,				///< ���K���x
				int		no_mill	)		///< XY���W��1000000�ɂȂ邱�Ƃ����e���邩�i0=���e,1=���e���Ȃ�=�ׂ̃��b�V����0�Ƃ��ĕԂ��j 
{
	double	wid1		= 1.0,	hei1		= 2.0 / 3.0;
	double	wid2		= 0.0,	hei2		= 0.0;
	double	wid64		= 0.0,	hei64		= 0.0;
	double	lat_amari1	= 0.0,	lon_amari1	= 0.0;
	double	lat_amari2	= 0.0,	lon_amari2	= 0.0;
	double	lat_amari3	= 0.0,	lon_amari3	= 0.0;
	double	x_step_1	= 0.0,	y_step_1	= 0.0;
	double	x_step_2	= 0.0,	y_step_2	= 0.0;
	double	x_step_3	= 0.0,	y_step_3	= 0.0;
	int		up_1st		= 0,	low_1st		= 0;
	int		up_2nd		= 0,	low_2nd		= 0;
	int		up_64		= 0,	low_64		= 0;
	// topmap�p�ɒǉ�
	double	lat_amari0	= 0.0,	lon_amari0	= 0.0;
	int		up_0		= 0,	low_0		= 0;
	double	widtop		= 32.0;
	double	heitop		= 21.0 + 1.0 / 3.0;	// 21*60+20;
	double	x_step_0	= 0.0,	y_step_0	= 0.0;

	double	offset_lon	= 100.0;		// ���b�V�����_�̃I�t�Z�b�g�l�i�o�x�j
	double	offset_lat	= 0.0; 			// ���b�V�����_�̃I�t�Z�b�g�l�i�ܓx�j

	x_step_0	= widtop / D_MILLION;	// for topmap
	y_step_0	= heitop / D_MILLION;	// for topmap

	x_step_1	= wid1 / D_MILLION;
	y_step_1	= hei1 / D_MILLION;

	wid2		= 7.0 * 1.0 / 60.0 + 30.0 * 1.0 / 3600.0; 
	hei2		= 5.0 * 1.0 / 60.0;

	x_step_2	= wid2 / D_MILLION;
	y_step_2	= hei2 / D_MILLION;

	wid64		= wid2 / 10.0;
	hei64		= hei2 / 10.0;

	x_step_3	= wid64 / D_MILLION;
	y_step_3	= hei64 / D_MILLION;
	
	up_1st		= (lat - offset_lat) * 1.5;	// 2006.09.06
	low_1st		= lon - offset_lon;			// 2006.09.06

	lat_amari1	= lat - (up_1st / 1.5) - offset_lat;	// 2006.09.06
	lon_amari1	= lon - (low_1st + offset_lon);

	up_2nd		= lat_amari1 / hei2;
	low_2nd		= lon_amari1 / wid2;
	lat_amari2	= lat_amari1 - up_2nd * hei2;
	lon_amari2	= lon_amari1 - low_2nd * wid2;
	up_64		= lat_amari2 / hei64;
	low_64		= lon_amari2 / wid64;
	lat_amari3	= lat_amari2 - up_64 * hei64;
	lon_amari3	= lon_amari2 - low_64 * wid64;
	*meshcode	= up_1st * MILLION + low_1st * 10000 +
					up_2nd * 1000 + low_2nd * 100 +
					up_64 * 10 + low_64;
	*x			= lon_amari3 / x_step_3 + 0.5;
	*y			= lat_amari3 / y_step_3 + 0.5;

	if( no_mill == 1 ) {
		int	xflag = 0, yflag = 0;
		if( *x == MILLION ) {
			xflag	= 1;
			*x		= 0;
		}
		if( *y == MILLION ) {
			yflag	= 1;
			*y		= 0;
		}
		*meshcode	= Get3rdMeshCode(*meshcode, xflag, yflag );
	}
}

//---------------------------------------------------------------------
// 3�����b�V���R�[�h + (x,y) ����A�o�ܓx�����߂� 2007.09.13
//	@return	����
//---------------------------------------------------------------------
bool	crd_cnv::
Mesh3rdtoLL(	int		meshcode,		///< ���b�V���R�[�h
				int		x,				///< ���K���w
				int		y,				///< ���K���x
				double*	lon,			///< �o�x
				double*	lat	)			///< �ܓx
{
	int		atai[6];
	int		i;
	double	wid1	= 1.0,	hei1	= 2.0 / 3.0;
	double	wid2	= 0.0;
	double	wid100	= 0.0;
	double	hei2	= 0.0;
	double	hei100	= 0.0;
	double	widtop	= 0.0;
	double	heitop	= 0.0;

	char	a_cTmpBuf[18];
	char*	a_cTmpStr[6] = {NULL, NULL, NULL, NULL, NULL, NULL};

	memset	( a_cTmpBuf, '\0', 18 );

	wid2	= 7.0 / 60.0 + 30.0 / 3600.0; 
	hei2	= 5.0 / 60.0;
	wid100	= wid2 / 10.0;
	hei100	= hei2 / 10.0;

	widtop	= 32.0;
	heitop	= 21.0 + 1.0 / 3.0;	// 21*60+20

	// ������
	for(i = 0; i < 6; i++)
	{
		atai[i]		= 0;
		a_cTmpStr[i]= a_cTmpBuf+(i*3);
	}

	int	digit	= GetMeshDigit(meshcode);


	if(digit == 8) { // 1/64���b�V���R�[�h�̎�

		char	a_cpMeshStr[9];
		sprintf	( a_cpMeshStr, "%d", meshcode );

		strncpy ( a_cTmpStr[5], a_cpMeshStr, 2);	// 1�����b�V����2��
		strncpy ( a_cTmpStr[4], a_cpMeshStr+2, 2);	// 1�����b�V����2��
		strncpy ( a_cTmpStr[3], a_cpMeshStr+4, 1);	// 2�����b�V����1��
		strncpy ( a_cTmpStr[2], a_cpMeshStr+5, 1);	// 2�����b�V����1��
		strncpy ( a_cTmpStr[1], a_cpMeshStr+6, 1);	// 8�����b�V����1��
		strncpy ( a_cTmpStr[0], a_cpMeshStr+7, 1);	// 8�����b�V����1��

		for( i = 5; i >= 0; i-- )
		{
			atai[i]	= atoi(a_cTmpStr[i]);
		}

		*lat	= atai[5] / 1.5 + hei2 * atai[3] + hei100 * atai[1]
				+ y * hei100 / D_MILLION;
		*lon	= 100.0 + atai[4] + wid2 * atai[2] + wid100 * atai[0]
				+ x * wid100 / D_MILLION;
	} else {
		return(false);
	}
	return(true);
}

//---------------------------------------------------------------------
//	���_���b�V������(x,y)���ꂽ���̃��b�V���R�[�h�����߂�
//	@return		�����F���b�V���R�[�h \n
//	@return		���s�F -1
//---------------------------------------------------------------------
int		crd_cnv::
Get3rdMeshCode(	int		meshcode,	///< ���b�V���R�[�h
				int		xdist,		///< ���_���b�V������X�����։����b�V������Ă��邩
				int		ydist	)	///< ���_���b�V������Y�����։����b�V������Ă��邩
{

	int		meshunit	= 0;
	int		roop_count	= 0;

	int		mc			= 0;				// �W�����b�V���R�[�h
	int		num1[6]		= {0,0,0,0,0,0};	// �W���΂�΂�ɂ���
	int		dist[2]		= {0,0};
	int		retmesh		= 0;
	int		tmp1		= 0;
	int		tmp2		= 0;

	dist[0]				= xdist;
	dist[1]				= ydist;

	mc		= meshcode;

	meshunit	= 64;
	roop_count	= 4;

	// �����΂�΂�ɂ���(59 41 4 1 7 7 �̂悤�ɂ���)
	int	i;
	for(i = 0; i < roop_count; i++)
	{
		num1[i]	= mc % 10;
		mc		/= 10;
	}
	num1[roop_count]	= mc % 100;
	num1[roop_count+1]	= mc / 100;

	int	j = 0;
	if(meshunit == 64) {
		// j = 0 ... ���E�����ɗ��ꂽ���b�V���R�[�h
		// j = 1 ... �㉺�����ɗ��ꂽ���b�V���R�[�h
		for(j = 0; j < 2; j++) {
			num1[j]	+= dist[j];
			if(num1[j] > 9) {		// 3�����b�V���̏���
				num1[j+2]	+= num1[j] / 10;		// ���̑���グ
				num1[j]		%= 10;
				if(num1[j+2] > 7) {	// 2�����b�V���̏���
					num1[j+4]	+= num1[j+2] / 8;	// ���̑���グ
					num1[j+2]	%= 8;
				}
			} else
			if(num1[j] < 0) {		// 3�����b�V���̏���
				num1[j]		-= 9;
				num1[j+2]	-= (-num1[j] / 10);		// ���̌J����
				num1[j]		=  9 - (-num1[j] % 10);
				if(num1[j+2] < 0) {	// 2�����b�V���̏���
					num1[j+2]	-= 7;
					num1[j+4]	-= (-num1[j+2] / 8);// ���̌J����
					num1[j+2]	= 7 - (-num1[j+2] % 8);
				}
			}
		}

		retmesh	= num1[5] * MILLION	+ num1[4] * 10000 + 
					num1[3] * 1000	+ num1[2] * 100 +
					num1[1] * 10	+ num1[0];
	}
	return(retmesh);
}

//---------------------------------------------------------------------
//	�o�C���j�A��ԂŒl�����߂Ă���
//	@return		�v�Z�l
//---------------------------------------------------------------------
double	crd_cnv::
Bilinear(	double	c_dLU,		///< ����̒l
			double	c_dRU,		///< �E��̒l
			double	c_dLD,		///< �����̒l
			double	c_dRD,		///< �E���̒l
			double	c_dPtX,		///< ��`����X�����ʒu(0.0<=X<=1.0)
			double	c_dPtY )	///< ��`����Y�����ʒu(0.0<=Y<=1.0)
{
	double	a_dValue = 0.0;

	// 2009.02.04 ��c���j���u���E���n�n�ƍ��W�ϊ��v�̉�������Ɏ���
	a_dValue	= (1.0 - c_dPtX) * (1.0 - c_dPtY) * c_dLD +
					c_dPtY * (1.0 - c_dPtX) * c_dLU +
					c_dPtX * (1.0 - c_dPtY) * c_dRD + c_dPtX * c_dPtY * c_dRU;					

	return	( a_dValue );
}

//---------------------------------------------------------------------
// �p�����[�^�e�[�u�����獶���A����A�E��A�E���̃p�����[�^���擾����
//	@return	���s == 0
//	@return	�p�����[�^�͈͓̔��ŕϊ� == 1
//	@return	�p�����[�^�͈̔͊O�ŕϊ�(���x���͈͓����Â�) == 2
//---------------------------------------------------------------------
bool		crd_cnv::
GetParam(	int			c_iMeshCode,	///< ���b�V���R�[�h[IN]
			param_info*	c_pLD_Prm,		///< �����̃p�����[�^[OUT]
			param_info*	c_pLU_Prm,		///< ����̃p�����[�^[OUT]
			param_info*	c_pRU_Prm,		///< �E��̃p�����[�^[OUT]
			param_info*	c_pRD_Prm,		///< �E���̃p�����[�^[OUT]
			bool		c_bIPC_Flag	)	///< IPC�ŗL�ϊ������邩�ǂ���[IN] true=IPC�ŗL�ϊ� false=���Ԉ�ʑΉ��ϊ�
{
	// 3�����b�V������A����A�E���A�E���3�����b�V�����Z�o����
	int	a_iLU3rdMesh = 0, a_iRD3rdMesh = 0, a_iRU3rdMesh = 0;

	a_iLU3rdMesh	= Get3rdMeshCode ( c_iMeshCode, 0, 1 );
	a_iRD3rdMesh	= Get3rdMeshCode ( c_iMeshCode, 1, 0 );
	a_iRU3rdMesh	= Get3rdMeshCode ( c_iMeshCode, 1, 1 );

	// �p�����[�^�e�[�u������A�o�ܓx�̍����l���擾���Ă���
	int j = 0;
	int	a_iGetFailFlag	= 0;
	for( j = 0; j < 4; j++ )
	{
		param_info		a_pKey;	// �T�[�`�p�̃L�[
		switch ( j )
		{
		case	0:	a_pKey.e_imeshcode = c_iMeshCode; break;
		case	1:	a_pKey.e_imeshcode = a_iLU3rdMesh; break;
		case	2:	a_pKey.e_imeshcode = a_iRD3rdMesh; break;
		case	3:	a_pKey.e_imeshcode = a_iRU3rdMesh; break;
		}

		std::set<param_info, std::less<param_info> >::iterator	a_iIndx;
		a_iIndx	= m_spParam.find ( a_pKey );
		if( a_iIndx == m_spParam.end() ) {
			a_iGetFailFlag	= 1;
			continue;
		}

		//----------------------------------------------
		// �����Ƃ��߂��|�C���g������Ă���
		//----------------------------------------------

		//------------------------------------------------------------------------------------
		param_info		a_pFoundParam	= *a_iIndx;	// �������ꂽ�n��p�����[�^
		double			a_dBefore_Lon = 0.0, a_dBefore_Lat = 0.0; // ���{���n�n�̌o�ܓx
		double			a_dJGD2000_Lon = 0.0, a_dJGD2000_Lat = 0.0, a_dJGD2000_He = 0.0;	// JGD2000�̌o�ܓx�A�ȉ~�̍�(m)
		// 2�����b�V���R�[�h�ɕϊ����A�p�����[�^�̃G���A�𔻕� 2009.10.20 �ǉ�
		if( Is_IPC_Tokyo97_Area( a_pKey.e_imeshcode / 100 ) && c_bIPC_Flag == true ) {
			// �l���̌o�ܓx�����߂�
			switch ( j )
			{
			case	0:	Mesh3rdtoLL(a_pKey.e_imeshcode,       0,       0, &a_dBefore_Lon, &a_dBefore_Lat); break;
			case	1:	Mesh3rdtoLL(a_pKey.e_imeshcode,       0, MILLION, &a_dBefore_Lon, &a_dBefore_Lat); break;
			case	2:	Mesh3rdtoLL(a_pKey.e_imeshcode, MILLION,       0, &a_dBefore_Lon, &a_dBefore_Lat); break;
			case	3:	Mesh3rdtoLL(a_pKey.e_imeshcode, MILLION, MILLION, &a_dBefore_Lon, &a_dBefore_Lat); break;
			}
			// ���߂��o�ܓx��3�p�����[�^�Ő��E���n�n�ɕϊ�����
			TOKYOtoJGD2000	( a_dBefore_Lon, a_dBefore_Lat, 0.0, &a_dJGD2000_Lon, &a_dJGD2000_Lat, &a_dJGD2000_He );
			// �n��p�����[�^�̒l����A�Y���|�C���g��n��p�����[�^�ŕϊ������o�ܓx�ƁA
			// 3�p�����[�^�ŕϊ������o�ܓx�̍����l����������
			a_pFoundParam.e_dDlon	= a_dJGD2000_Lon - a_dBefore_Lon;
			a_pFoundParam.e_dDlat	= a_dJGD2000_Lat - a_dBefore_Lat;
		}
		//------------------------------------------------------------------------------------

		switch ( j )
		{
		case	0:	*c_pLD_Prm = a_pFoundParam; break;
		case	1:	*c_pLU_Prm = a_pFoundParam; break;
		case	2:	*c_pRD_Prm = a_pFoundParam; break;
		case	3:	*c_pRU_Prm = a_pFoundParam; break;
		}
	}
	if ( a_iGetFailFlag == 1 ) {
		return	( false );
	}else {
		return	( true );
	}
}

//---------------------------------------------------------------------
/** 
* @brief �p�����[�^�͈̔͂��`�F�b�N����
* @return	�͈͓� == true
* @return	�͈͊O == false
*/
//---------------------------------------------------------------------
bool		crd_cnv::
ChkParamRange(	long	c_lLD3rdMesh,			///< [in] ������3�����b�V���R�[�h
				long	c_lRU3rdMesh,			///< [in] �E���3�����b�V���R�[�h
				long	c_lTgt3rdMesh	)		///< [in] �^�[�Q�b�g��3�����b�V���R�[�h
{
	double	a_dLD_Lon = 0.0, a_dLD_Lat = 0.0;
	double	a_dRU_Lon = 0.0, a_dRU_Lat = 0.0;
	double	a_dTgt_Lon= 0.0, a_dTgt_Lat= 0.0;
	// ��ԋ߂��p�����[�^�̂���_
	Mesh3rdtoLL(c_lLD3rdMesh, 0, 0, &a_dLD_Lon, &a_dLD_Lat);
	Mesh3rdtoLL(c_lRU3rdMesh, 0, 0, &a_dRU_Lon, &a_dRU_Lat);
	Mesh3rdtoLL(c_lTgt3rdMesh, 0, 0, &a_dTgt_Lon, &a_dTgt_Lat);
	if( a_dTgt_Lon < a_dLD_Lon ||	// ���ɂ͂ݏo��
		a_dTgt_Lon > a_dRU_Lon ||	// �E�ɂ͂ݏo��
		a_dTgt_Lat < a_dLD_Lat ||	// ���ɂ͂ݏo��
		a_dTgt_Lat > a_dRU_Lat )	// ��ɂ͂ݏo��
	{
		return	( false );	// �͈͊O
	}
	return	( true );	// �͈͓�
}

//---------------------------------------------------------------------
/**  2010.01.19 �ǉ��e�X�g
* @brief �p�����[�^�e�[�u�����狗���I�Ɉ�ԋ߂��p�����[�^���擾����(�b��)
* @return	���s == 0
* @return	�p�����[�^�͈͓̔��ŕϊ� == 1
* @return	�p�����[�^�͈̔͊O�ŕϊ�(���x���͈͓����Â�) == 2
*/
//---------------------------------------------------------------------
bool	crd_cnv::
GetNearestParam(	double		c_dLon,			///< [in]   �o�x
					double		c_dLat,			///< [in]   �ܓx
					param_info*	c_pNearest_Prm,	///< [out]  �ŋߓ_�̃p�����[�^
					bool		c_bIPC_Flag	)	///< [in]	IPC�ŗL�ϊ������邩�ǂ��� true=IPC�ŗL�ϊ� false=���Ԉ�ʑΉ��ϊ�
{
	//---------------------------------------------------------------------------------
	// �o�ܓx��3�����b�V���R�[�h��
	int	a_iMesh3rd = 0, a_iX = 0, a_iY = 0;
	LLto3rdMesh	( c_dLon, c_dLat, &a_iMesh3rd, &a_iX, &a_iY, 1 );

	double	a_dLon = 0.0, a_dLat = 0.0;
	param_info	a_pNearest;
	
	// �n��p�����[�^�O�̃��b�V���̏ꍇ�́A���ɍŋߖT�p�����[�^���擾�ς݂��m�F����
	std::map<int, std::set<param_info, std::less<param_info>>>::const_iterator nearPram = m_outCode2nearCodes.find(a_iMesh3rd);
	if(m_outCode2nearCodes.end() == nearPram){
		// �ŋߖT�p�����[�^���擾���Ă��Ȃ���Όv�Z����
		std::set<param_info, std::less<param_info> >	a_pTmpParamSet;	// �ŋߖT�擾�p�̃p�����[�^���X�g
		std::set<param_info, std::less<param_info> >::iterator	a_iBeginIndx, a_iEndIndx, a_iTmpIndx;
		int i = 0;
		for(i = 1;i < 1600; ) // i��320�ł��悻1�����b�V���͈̔�
		{
			int	a_iLD_MeshCode = 0, a_iRU_MeshCode = 0;
			param_info	a_pLDKey, a_pRUKey;
			a_iLD_MeshCode	= Get3rdMeshCode( a_iMesh3rd, -1*i, -1*i );	// �����̃��b�V���R�[�h���쐬
			a_iRU_MeshCode	= Get3rdMeshCode( a_iMesh3rd, 1*i, 1*i );	// �����̃��b�V���R�[�h���쐬
			a_pLDKey.e_imeshcode	= a_iLD_MeshCode;
			a_pRUKey.e_imeshcode	= a_iRU_MeshCode;
			a_iBeginIndx	= m_spParam.lower_bound(a_pLDKey);
			a_iEndIndx		= m_spParam.upper_bound(a_pRUKey);
			// �͈̓`�F�b�N�����
			for( a_iTmpIndx = a_iBeginIndx; a_iTmpIndx != a_iEndIndx; a_iTmpIndx++ )
			{
				if( ChkParamRange( a_iLD_MeshCode, a_iRU_MeshCode, (*a_iTmpIndx).e_imeshcode ) ) {
					// �p�����[�^���X�g�ɒǉ�
					a_pTmpParamSet.insert ( *a_iTmpIndx );
				}
			}
			// �p�����[�^�����������甲����i�ǂ�������Ō�����͂��j
			if( a_pTmpParamSet.size() > 0 ) {
				break;
			}
			if( i < 10 ) {
				i++;
			}else {
				i+=40;
			}
		}

		//---------------------------------------------------------------------------------
		std::set<param_info, std::less<param_info> >::iterator	a_iIndx, a_iBegin, a_iEnd;
		// 1�����b�V���P�ʂɖ����ꍇ�́A�S��������
		if( a_pTmpParamSet.size() == 0 ) {
			a_iBegin	= m_spParam.begin();
			a_iEnd		= m_spParam.end();
		}else {
			a_iBegin	= a_pTmpParamSet.begin();
			a_iEnd		= a_pTmpParamSet.end();
		}

		// ����ȍ~�̂��߂ɋߖT�p�����[�^���m�ۂ��邽�߂̃R���e�i
		std::vector<std::pair<double,param_info>> tmpNearParam;

		double	a_dDist= -1.0;
		for( a_iIndx = a_iBegin; a_iIndx != a_iEnd; a_iIndx++ )
		{
			param_info	a_pCur	= *a_iIndx;
			// �������v�Z
			// �p�����[�^�̃��b�V���R�[�h���ܓx�A�o�x�ɕϊ����A�������v�Z
			a_dLon = 0.0;	a_dLat = 0.0;
			Mesh3rdtoLL(a_pCur.e_imeshcode, 0, 0, &a_dLon, &a_dLat);
			double	a_dTmpDist = 0.0;
			a_dTmpDist	= sqrt(pow((c_dLon - a_dLon),2.0) + pow((c_dLat - a_dLat),2.0 ));
			if( a_dDist < 0.0 ){
				a_dDist = a_dTmpDist;
				a_pNearest	= a_pCur;
			} else {
				if( a_dTmpDist < a_dDist ) {
					a_dDist	= a_dTmpDist;
					a_pNearest	= a_pCur;
				}
			}
			tmpNearParam.push_back(std::make_pair(a_dTmpDist,a_pCur));
		}
		// ����ȍ~�̂��߂ɁA�p�����[�^�O�̃��b�V���ƍŋߖT�p�����[�^�̑Ή��͊m�ۂ��Ă���
		// �ߖT�p�����[�^�́A�w�胁�b�V���̎���8������Ώ\��
		std::sort(tmpNearParam.begin(),tmpNearParam.end());
		for(long l = 0; l < tmpNearParam.size(); ++l){
			m_outCode2nearCodes[a_iMesh3rd].insert(tmpNearParam[l].second);
			if(8 == l)
				break;
		}
	} else {
		double	a_dDist= -1.0;
		std::set<param_info>::const_iterator a_iIndx = nearPram->second.begin();
		std::set<param_info>::const_iterator a_iEnd = nearPram->second.end();
		for( ; a_iIndx != a_iEnd; ++a_iIndx )
		{
			param_info	a_pCur	= *a_iIndx;
			// �������v�Z
			// �p�����[�^�̃��b�V���R�[�h���ܓx�A�o�x�ɕϊ����A�������v�Z
			a_dLon = 0.0;	a_dLat = 0.0;
			Mesh3rdtoLL(a_pCur.e_imeshcode, 0, 0, &a_dLon, &a_dLat);
			double	a_dTmpDist = 0.0;
			a_dTmpDist	= sqrt(pow((c_dLon - a_dLon),2.0) + pow((c_dLat - a_dLat),2.0 ));
			if( a_dDist < 0.0 ){
				a_dDist = a_dTmpDist;
				a_pNearest	= a_pCur;
			} else {
				if( a_dTmpDist < a_dDist ) {
					a_dDist	= a_dTmpDist;
					a_pNearest	= a_pCur;
				}
			}
		}
	}

	// ��ԋ߂��p�����[�^�̂���_
	Mesh3rdtoLL(a_pNearest.e_imeshcode, 0, 0, &a_dLon, &a_dLat);

	//------------------------------------------------------------------------------------
	double			a_dBefore_Lon = 0.0, a_dBefore_Lat = 0.0; // ���{���n�n�̌o�ܓx
	double			a_dJGD2000_Lon = 0.0, a_dJGD2000_Lat = 0.0, a_dJGD2000_He = 0.0;	// JGD2000�̌o�ܓx�A�ȉ~�̍�(m)
	// 2�����b�V���R�[�h�ɕϊ����A�p�����[�^�̃G���A�𔻕� 2009.10.20 �ǉ�
	if( Is_IPC_Tokyo97_Area( a_pNearest.e_imeshcode / 100 ) && c_bIPC_Flag == true ) {
		// �l���̌o�ܓx�����߂�
		Mesh3rdtoLL(a_pNearest.e_imeshcode,       0,       0, &a_dBefore_Lon, &a_dBefore_Lat);
		// ���߂��o�ܓx��3�p�����[�^�Ő��E���n�n�ɕϊ�����
		TOKYOtoJGD2000	( a_dBefore_Lon, a_dBefore_Lat, 0.0, &a_dJGD2000_Lon, &a_dJGD2000_Lat, &a_dJGD2000_He );
		// �n��p�����[�^�̒l����A�Y���|�C���g��n��p�����[�^�ŕϊ������o�ܓx�ƁA
		// 3�p�����[�^�ŕϊ������o�ܓx�̍����l����������
		a_pNearest.e_dDlon	= a_dJGD2000_Lon - a_dBefore_Lon;
		a_pNearest.e_dDlat	= a_dJGD2000_Lat - a_dBefore_Lat;
	}
	//------------------------------------------------------------------------------------

	// 3�p�����[�^�ŕϊ�
	double	a_dAfter_3prm_Lon = 0.0, a_dAfter_3prm_Lat = 0.0, a_dAfter_He = 0.0;
	TOKYOtoJGD2000	( a_dLon, a_dLat, 0.0, &a_dAfter_3prm_Lon, &a_dAfter_3prm_Lat, &a_dAfter_He );
	// �n��p�����[�^�ŕϊ������l
	double	a_dAfter_Rprm_Lon = 0.0, a_dAfter_Rprm_Lat = 0.0;
	a_dAfter_Rprm_Lon		= a_dLon + a_pNearest.e_dDlon;
	a_dAfter_Rprm_Lat		= a_dLat + a_pNearest.e_dDlat;

	// ���ړ_��3�p�����[�^�ŕϊ�
	double	a_dTmp_3prm_Lon = 0.0, a_dTmp_3prm_Lat = 0.0;
	TOKYOtoJGD2000	( c_dLon, c_dLat, 0.0, &a_dTmp_3prm_Lon, &a_dTmp_3prm_Lat, &a_dAfter_He );

	// �b��p�����[�^���v�Z
	a_pNearest.e_dDlon	= (a_dTmp_3prm_Lon - c_dLon) + (a_dAfter_Rprm_Lon - a_dAfter_3prm_Lon);
	a_pNearest.e_dDlat	= (a_dTmp_3prm_Lat - c_dLat) + (a_dAfter_Rprm_Lat - a_dAfter_3prm_Lat);

	*c_pNearest_Prm	= a_pNearest;
	return	( true );
}

//---------------------------------------------------------------------
// �o�ܓx�̑��n�n���ݕϊ�
//	@return	�p�����[�^�͈͓̔��ŕϊ� == 1
//	@return	�p�����[�^�͈̔͊O�ŕϊ�(���x���͈͓����Â�) == 2
//---------------------------------------------------------------------
int		crd_cnv::
LLConv(	double	c_dBefore_Lon,	///< �ϊ��O�o�x
		double	c_dBefore_Lat,	///< �ϊ��O�ܓx
		double*	c_dAfter_Lon,	///< �ϊ���o�x
		double*	c_dAfter_Lat,	///< �ϊ����ܓx
		int		c_iMode,		///< �ϊ����[�h�i0=Tokyo->JGD2000�ւ̕ϊ� 1=JGD2000->Tokyo�ւ̕ϊ��j
		bool	c_bIPC_Flag	)	///< IPC�ŗL�ϊ������邩�ǂ��� true=IPC�ŗL�ϊ� false=���Ԉ�ʑΉ��ϊ�
{

	int	a_iMeshCode = 0, a_iX = 0, a_iY = 0;
	double	a_dGTmpLon = 0.0, a_dGTmpLat = 0.0;

	// �o�ܓx����3�����b�V���R�[�h�ɕϊ�����
	if( c_iMode == 0 ) {
		LLto3rdMesh( c_dBefore_Lon, c_dBefore_Lat, &a_iMeshCode, &a_iX, &a_iY, 1 );
	}else {
		int	a_iTmpMeshCode = 0, a_iTmpX = 0, a_iTmpY = 0;
		double	a_dTmpLon = 0.0, a_dTmpLat = 0.0;
		a_dTmpLon	= c_dBefore_Lon + (12.0/3600.0);
		a_dTmpLat	= c_dBefore_Lat - (12.0/3600.0);
		// ���̋����{���n�n�ɕϊ�
		LLto3rdMesh( a_dTmpLon, a_dTmpLat, &a_iTmpMeshCode, &a_iTmpX, &a_iTmpY, 1 );
		
		// tmp�̒l����e���|�����̍����l�����߂�
		param_info		a_pTmpLD, a_pTmpLU, a_pTmpRU, a_pTmpRD;	// �p�����[�^����擾�����l
		if( GetParam( a_iTmpMeshCode, &a_pTmpLD, &a_pTmpLU, &a_pTmpRU, &a_pTmpRD, c_bIPC_Flag ) ) {

			// tmp�̒l����o�C���j�A��ԂŒl�����߂�
			// �擾�����l����o�C���j�A��ԂŊY���n�_�̒l�����߂�
			double	a_dTmpPtX = 0.0, a_dTmpPtY = 0.0;
			double	a_dDTmpLon = 0.0, a_dDTmpLat = 0.0;

			a_dTmpPtX	= a_iTmpX / D_MILLION;
			a_dTmpPtY	= a_iTmpY / D_MILLION;

			// ���̋����{���n�n���瓾��ꂽ�p�����[�^���g�p���A��萳�m�ȋ����{���n�n�̈ʒu�����o���A���̈ʒu����
			// �p�����[�^������o��
			a_dDTmpLon	= Bilinear ( a_pTmpLU.e_dDlon, a_pTmpRU.e_dDlon, a_pTmpLD.e_dDlon, a_pTmpRD.e_dDlon, a_dTmpPtX, a_dTmpPtY );
			a_dDTmpLat	= Bilinear ( a_pTmpLU.e_dDlat, a_pTmpRU.e_dDlat, a_pTmpLD.e_dDlat, a_pTmpRD.e_dDlat, a_dTmpPtX, a_dTmpPtY );

			LLto3rdMesh( c_dBefore_Lon - a_dDTmpLon, c_dBefore_Lat - a_dDTmpLat, &a_iMeshCode, &a_iX, &a_iY, 1 );

		}else {
			// ��ԋ߂��Ƃ���̃p�����[�^���擾����
			param_info		a_pNearest;
			GetNearestParam( a_dTmpLon, a_dTmpLat, &a_pNearest, c_bIPC_Flag );
			a_dGTmpLon	= a_pNearest.e_dDlon;
			a_dGTmpLat	= a_pNearest.e_dDlat;
		//	return	( false );
		}
	}
	// 2009.01.28 IPC�ŗL�̕ϊ��Ή����s�����ǂ����̔�����s��
	// Tokyo97�Ή��������Ƃ���ɂ��Ă͋����I��3�p�����[�^�v�Z�ŕϊ�����悤�ɂ��Ă���B
//	bool	a_bExec_IPC_Only	= false;
//	if( c_bIPC_Flag == true ) {
//		a_bExec_IPC_Only	= Is_IPC_Tokyo97_Area( a_iMeshCode / 100 );	// 2�����b�V���R�[�h�����Ċm�F
//	}

	// �p�����[�^�e�[�u������A�o�ܓx�̍����l���擾���Ă���
	param_info		a_pLD, a_pLU, a_pRU, a_pRD;	// �p�����[�^����擾�����l

	double	a_dDLon = 0.0, a_dDLat = 0.0;
	if( GetParam( a_iMeshCode, &a_pLD, &a_pLU, &a_pRU, &a_pRD, c_bIPC_Flag ) ) {

		// �擾�����l����o�C���j�A��ԂŊY���n�_�̒l�����߂�
		double	a_dPtX = 0.0, a_dPtY = 0.0;

		a_dPtX	= a_iX / D_MILLION;
		a_dPtY	= a_iY / D_MILLION;

		a_dDLon	= Bilinear ( a_pLU.e_dDlon, a_pRU.e_dDlon, a_pLD.e_dDlon, a_pRD.e_dDlon, a_dPtX, a_dPtY );
		a_dDLat	= Bilinear ( a_pLU.e_dDlat, a_pRU.e_dDlat, a_pLD.e_dDlat, a_pRD.e_dDlat, a_dPtX, a_dPtY );
	}else {
		param_info		a_pNearest;
		GetNearestParam( c_dBefore_Lon - a_dGTmpLon, c_dBefore_Lat - a_dGTmpLat, &a_pNearest, c_bIPC_Flag );
		a_dDLon	= a_pNearest.e_dDlon;
		a_dDLat	= a_pNearest.e_dDlat;
	}
	// �����l�ƌ��̌o�ܓx����JGD2000�ł̌o�ܓx�����߂�
	if( c_iMode == 0 ) {
		*c_dAfter_Lon	= c_dBefore_Lon + a_dDLon;
		*c_dAfter_Lat	= c_dBefore_Lat + a_dDLat;
	}else {
		*c_dAfter_Lon	= c_dBefore_Lon - a_dDLon;
		*c_dAfter_Lat	= c_dBefore_Lat - a_dDLat;
	}
	return	( CONVED_REGIONPRM );
}
//-----------------------------------------------------------------------------

//=======================================================================
//	���L6�̃����o�֐��ABLHtoXYZ(), XYZtoBLH(), XYZtoXYZ()	�́A
//  [Bug 5755] �̗v�]�ɂ��ǉ� 2008.01.21
//	���y�⑫�����z
//	����6�̑��n�n�ϊ��֐��́A��c ���v�������A�u���E���n�n�ƍ��W�ϊ��v
//  �ŉ������Ă����@�����ɍ쐬����Ă��܂��B
//=======================================================================

//---------------------------------------------------------------------
// �n�����W�l����O�����������W�l�ւ̕ϊ� 2007.05.09 �ǉ�
// (B,L,He)->(X,Y,Z)
//	@return	����
//---------------------------------------------------------------------
void	crd_cnv::
BLHtoXYZ(	double	c_dA,			///< ��]�ȉ~�̒����a(m)
			double	c_dInvF,		///< �G����f�̋t��1/f
			double	c_dLon,			///< �o�x
			double	c_dLat,			///< �ܓx
			double	c_dHe,			///< �ȉ~�̍�(m)
			double*	c_dX,			///< ���_�����X�������̋���(m)[OUT]
			double*	c_dY,			///< ���_�����Y�������̋���(m)[OUT]
			double*	c_dZ	)		///< ���_�����Z�������̋���(m)[OUT]
{
//	const double	a_dA	= 6377397.155;				// �x�b�Z���ȉ~�̒����a(m)
	const double	a_dF	= 1.0/c_dInvF;				// �G����
	double			a_dE2	= a_dF*(2.0 - a_dF);		// ���S���̂Q��
	double			a_dN	= 0.0;						// �K�ѐ��ȗ����a(m)

	double			a_dRad_Lon = 0.0, a_dRad_Lat = 0.0;	// ���W�A���ɒ������o�x�A�ܓx

	// �o�ܓx�����W�A���ɒ���
	a_dRad_Lon	= c_dLon * M_PI / 180.0;
	a_dRad_Lat	= c_dLat * M_PI / 180.0;

	a_dN	= c_dA / sqrt(1.0 - (a_dE2)*pow(sin(a_dRad_Lat), 2.0));

	*c_dX	= (a_dN + c_dHe) * cos(a_dRad_Lat) * cos(a_dRad_Lon);
	*c_dY	= (a_dN + c_dHe) * cos(a_dRad_Lat) * sin(a_dRad_Lon);
	*c_dZ	= (a_dN * (1.0 - a_dE2) + c_dHe) * sin(a_dRad_Lat);
}

//---------------------------------------------------------------------
// �O�����������W�l����n�����W�l�ւ̕ϊ� 2007.05.09 �ǉ�
// (X,Y,Z)->(B,L,He)
//	@return	����
//---------------------------------------------------------------------
void	crd_cnv::
XYZtoBLH(	double	c_dA,			///< ��]�ȉ~�̒����a(m)
			double	c_dInvF,		///< �G����f�̋t��1/f
			double	c_dX,			///< ���_�����X�������̋���(m)
			double	c_dY,			///< ���_�����Y�������̋���(m)
			double	c_dZ,			///< ���_�����Z�������̋���(m)
			double*	c_dLon,			///< �o�x[OUT]
			double*	c_dLat,			///< �ܓx[OUT]
			double*	c_dHe	)		///< �ȉ~�̍�(m)[OUT]
{
//	const double	a_dA	= 6378137.0;				// GRS80�ȉ~�̒����a(m)
	const double	a_dF	= 1.0/c_dInvF;				// �G����
	double			a_dE2	= a_dF*(2.0 - a_dF);		// ���S���̂Q��
	double			a_dN	= 0.0;						// �K�ѐ��ȗ����a(m)

	double			a_dRad_Lon = 0.0, a_dRad_Lat = 0.0;	// ���W�A���ɒ������o�x�A�ܓx

	double			a_dOut_Lon = 0.0, a_dOut_Lat = 0.0, a_dOut_He = 0.0;

	// ��Ɨp�ϐ�
	double			a_dp = 0.0, a_dr = 0.0, a_du = 0.0;

	a_dp			= sqrt ( c_dX*c_dX + c_dY*c_dY );
	a_dr			= sqrt ( a_dp*a_dp + c_dZ*c_dZ );

	a_du			= atan ( c_dZ * ((1.0 - a_dF) + (a_dE2 * c_dA)/a_dr) / a_dp );

	a_dOut_Lat		= atan ( (c_dZ * (1.0 - a_dF) + (a_dE2 * c_dA) * pow(sin(a_du), 3.0)) /
								((1.0 - a_dF) * (a_dp - a_dE2 * c_dA * pow(cos(a_du), 3.0)) )) * 180.0 / M_PI;

	// 2011.2.10 atan()���ƁA-PI/2<=atan(x)<=PI/2 �܂ł͈̔͂Ȃ̂�atan2()�A-PI<=atan2(y,x)<=PI 
	// ���g�p����悤�ɏC���B
	a_dOut_Lon		= atan2 ( c_dY, c_dX ) * 180.0 / M_PI;

	// �ܓx�����W�A���ɕϊ����Ă���
	a_dRad_Lat		= a_dOut_Lat * M_PI / 180.0;

	a_dOut_He		= a_dp * cos(a_dRad_Lat) + c_dZ * sin(a_dRad_Lat) - c_dA * sqrt( (1.0 - a_dE2 * pow(sin(a_dRad_Lat), 2.0)) );

	*c_dLon			= a_dOut_Lon;
	*c_dLat			= a_dOut_Lat;
	*c_dHe			= a_dOut_He;
}

//---------------------------------------------------------------------
// �O�����������W�Ԃł̍��W�ϊ� 2007.05.09 �ǉ�
//	@return	����
//---------------------------------------------------------------------
void	crd_cnv::
XYZtoXYZ(	param_7	c_pParam7,		///< �ϊ��Ɏg�p����7�p�����[�^
			double	c_dXi,			///< ���_�����X�������̋���(m)(����)
			double	c_dYi,			///< ���_�����Y�������̋���(m)(����)
			double	c_dZi,			///< ���_�����Z�������̋���(m)(����)
			double*	c_dXo,			///< ���_�����X�������̋���(m)(�o��)[OUT]
			double*	c_dYo,			///< ���_�����Y�������̋���(m)(�o��)[OUT]
			double*	c_dZo	)		///< ���_�����Z�������̋���(m)(�o��)[OUT]
{
	*c_dXo	= c_dXi + (c_pParam7.T1 / 100.0) + (c_pParam7.D * 1e-8) * c_dXi
				- (c_pParam7.R3*0.001*M_PI*c_dYi/(180.0*3600.0)) + (c_pParam7.R2*0.001*M_PI*c_dZi/(180.0*3600.0));

	*c_dYo	= c_dYi + (c_pParam7.T2 / 100.0) + (c_pParam7.R3*0.001*M_PI*c_dXi/(180.0*3600.0))
				+ (c_pParam7.D * 1e-8) * c_dYi - (c_pParam7.R1*0.001*M_PI*c_dZi/(180.0*3600.0));

	*c_dZo	= c_dZi + (c_pParam7.T3 / 100.0) - (c_pParam7.R2*0.001*M_PI*c_dXi/(180.0*3600.0))
				+ (c_pParam7.R1*0.001*M_PI*c_dYi/(180.0*3600.0)) + (c_pParam7.D * 1e-8) * c_dZi;
}

//---------------------------------------------------------------------
// �o�ܓx�̑��n�n���ݕϊ�
//	@return	����
//---------------------------------------------------------------------
void	crd_cnv::
BLHtoBLH(	double	c_dBefore_A,	///< �ϊ��O���W�n�̏�������ȉ~�̒����a(m)
			double	c_dBefore_InvF,	///< �ϊ��O���W�n�̏�������ȉ~�̝̂G����
			double	c_dAfter_A,		///< �ϊ��O���W�n�̏�������ȉ~�̒����a(m)
			double	c_dAfter_InvF,	///< �ϊ��O���W�n�̏�������ȉ~�̝̂G����
			param_7	c_pBtoAParam7,	///< �ϊ��Ɏg�p����7�p�����[�^
			double	c_dBefore_Lon,	///< �ϊ��O�o�x
			double	c_dBefore_Lat,	///< �ϊ��O�ܓx
			double	c_dBefore_He,	///< �ϊ��O�ȉ~�̍�(m)
			double*	c_dAfter_Lon,	///< �ϊ���o�x[OUT]
			double*	c_dAfter_Lat,	///< �ϊ���ܓx[OUT]
			double*	c_dAfter_He )	///< �ϊ���ȉ~�̍�(m)[OUT]
{

	// �e���|������X,Y,Z(m)���W�l
	double	a_dXi = 0.0, a_dYi = 0.0, a_dZi = 0.0;
	double	a_dXo = 0.0, a_dYo = 0.0, a_dZo = 0.0;

	// �ϊ���̒l
	double	a_dTmp_Lon = 0.0, a_dTmp_Lat = 0.0;
	double	a_dHo = 0.0;

	int	a_iRetMode	= 2;

	BLHtoXYZ(c_dBefore_A, c_dBefore_InvF, c_dBefore_Lon, c_dBefore_Lat, c_dBefore_He, &a_dXi, &a_dYi, &a_dZi );

	XYZtoXYZ(c_pBtoAParam7, a_dXi, a_dYi, a_dZi, &a_dXo, &a_dYo, &a_dZo);

	XYZtoBLH(c_dAfter_A, c_dAfter_InvF, a_dXo, a_dYo, a_dZo, &a_dTmp_Lon, &a_dTmp_Lat, &a_dHo);

	*c_dAfter_Lon	= a_dTmp_Lon;	// 2011.2.10 �C��
	*c_dAfter_Lat	= a_dTmp_Lat;
	*c_dAfter_He	= a_dHo;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------
// Tokyo->JGD2000�ւ̍��W�ϊ�
//	@return ����
//---------------------------------------------------------------------
void	crd_cnv::
TOKYOtoJGD2000	(	double	c_dBefore_Lon,	///< �ϊ��O�o�x
					double	c_dBefore_Lat,	///< �ϊ��O�ܓx
					double	c_dBefore_He,	///< �ϊ��O�ȉ~�̍�(m)
					double*	c_dAfter_Lon,	///< �ϊ���o�x
					double*	c_dAfter_Lat,	///< �ϊ����ܓx
					double*	c_dAfter_He )	///< �ϊ���ȉ~�̍�(m)[OUT]
{
	double	a_dBefore_A = 0.0, a_dAfter_A = 0.0;
	double	a_dBefore_InvF = 0.0, a_dAfter_InvF = 0.0;
	param_7	a_pParam;

	// Tokyo->JGD2000 �ϊ��p�p�����[�^�ݒ�
	//=============================================================
	a_dBefore_A		= RA;				// �x�b�Z���ȉ~��
	a_dBefore_InvF	= BESSEL_INV_F;		// �x�b�Z���ȉ~�̝̂G�����̋t��

	a_dAfter_A		= GRS80_RA;			// GRS80�ȉ~��
	a_dAfter_InvF	= GRS80_INV_F;		// GRS80�ȉ~�̝̂G�����̋t��

	a_pParam.T1	= -14641.40;
	a_pParam.T2	= 50733.70;
	a_pParam.T3	= 68050.70;
	a_pParam.D	= 0.00;
	a_pParam.R1	= 0.00;
	a_pParam.R2	= 0.00;
	a_pParam.R3	= 0.00;
	//=============================================================

	BLHtoBLH( a_dBefore_A, a_dBefore_InvF, a_dAfter_A, a_dAfter_InvF, 
			a_pParam, c_dBefore_Lon, c_dBefore_Lat, c_dBefore_He, c_dAfter_Lon, c_dAfter_Lat, c_dAfter_He);
}

//---------------------------------------------------------------------
// JGD2000->Tokyo�ւ̍��W�ϊ�
//	@return ����
//---------------------------------------------------------------------
void	crd_cnv::
JGD2000toTOKYO	(	double	c_dBefore_Lon,	///< �ϊ��O�o�x
					double	c_dBefore_Lat,	///< �ϊ��O�ܓx
					double	c_dBefore_He,	///< �ϊ��O�ȉ~�̍�(m)
					double*	c_dAfter_Lon,	///< �ϊ���o�x
					double*	c_dAfter_Lat,	///< �ϊ����ܓx
					double*	c_dAfter_He )	///< �ϊ���ȉ~�̍�(m)[OUT]
{
	double	a_dBefore_A = 0.0, a_dAfter_A = 0.0;
	double	a_dBefore_InvF = 0.0, a_dAfter_InvF = 0.0;
	param_7	a_pParam;

	// JGD2000->Tokyo �ϊ��p�p�����[�^�ݒ�
	//=============================================================
	a_dBefore_A		= GRS80_RA;			// GRS80�ȉ~��
	a_dBefore_InvF	= GRS80_INV_F;		// GRS80�ȉ~�̝̂G�����̋t��

	a_dAfter_A		= RA;				// �x�b�Z���ȉ~��
	a_dAfter_InvF	= BESSEL_INV_F;		// �x�b�Z���ȉ~�̝̂G�����̋t��

	a_pParam.T1	= 14641.40;
	a_pParam.T2	= -50733.70;
	a_pParam.T3	= -68050.70;
	a_pParam.D	= 0.00;
	a_pParam.R1	= 0.00;
	a_pParam.R2	= 0.00;
	a_pParam.R3	= 0.00;
	//=============================================================

	BLHtoBLH( a_dBefore_A, a_dBefore_InvF, a_dAfter_A, a_dAfter_InvF, 
			a_pParam, c_dBefore_Lon, c_dBefore_Lat, c_dBefore_He, c_dAfter_Lon, c_dAfter_Lat, c_dAfter_He);
}

//---------------------------------------------------------------------
// �n�斈�p�����[�^���g�p����Tokyo->JGD2000�ւ̍��W�ϊ�
//	@return 0  = �ϊ��G���[�i���������Ŏg�p�����ꍇ���j
//  @return	1  = �n�斈�̃p�����[�^�ŕϊ����ꂽ�ꍇ3�p�����[�^�ŕϊ����ꂽ�ꍇ
//  @return 2  = 3�p�����[�^�ŕϊ����ꂽ�ꍇ
//---------------------------------------------------------------------
int	crd_cnv::
TOKYOtoJGD2000_RP	(	double	c_dBefore_Lon,		///< �ϊ��O�o�x[IN]
						double	c_dBefore_Lat,		///< �ϊ��O�ܓx[IN]
						double*	c_dAfter_Lon,		///< �ϊ���o�x[OUT]
						double*	c_dAfter_Lat,		///< �ϊ����ܓx[OUT]
						bool	c_bIPC_Flag	)		///< IPC�ŗL�ϊ������邩�ǂ���true=IPC�ŗL�ϊ� false=���Ԉ�ʑΉ��ϊ�[IN]
{
	if( m_bJGD2000_RP_Inited == false )	return	(JGD2000_RP_UNINITED);
	int	a_iRet	= 0;
	a_iRet	= LLConv( c_dBefore_Lon, c_dBefore_Lat, c_dAfter_Lon, c_dAfter_Lat, 0, c_bIPC_Flag );
	return	( a_iRet );
}

//---------------------------------------------------------------------
// �n�斈�p�����[�^���g�p����JGD2000->Tokyo�ւ̍��W�ϊ�
//	@return 0  = �ϊ��G���[�i���������Ŏg�p�����ꍇ���j
//  @return	1  = �n�斈�̃p�����[�^�ŕϊ����ꂽ�ꍇ3�p�����[�^�ŕϊ����ꂽ�ꍇ
//  @return 2  = 3�p�����[�^�ŕϊ����ꂽ�ꍇ
//---------------------------------------------------------------------
int	crd_cnv::
JGD2000toTOKYO_RP	(	double	c_dBefore_Lon,		///< �ϊ��O�o�x[IN]
						double	c_dBefore_Lat,		///< �ϊ��O�ܓx[IN]
						double*	c_dAfter_Lon,		///< �ϊ���o�x[OUT]
						double*	c_dAfter_Lat,		///< �ϊ����ܓx[OUT]
						bool	c_bIPC_Flag	)		///< IPC�ŗL�ϊ������邩�ǂ���true=IPC�ŗL�ϊ� false=���Ԉ�ʑΉ��ϊ�[IN]
{
	if( m_bJGD2000_RP_Inited == false )	return	(JGD2000_RP_UNINITED);
	int	a_iRet	= 0;
	a_iRet	= LLConv( c_dBefore_Lon, c_dBefore_Lat, c_dAfter_Lon, c_dAfter_Lat, 1, c_bIPC_Flag );
	return	( a_iRet );
}
