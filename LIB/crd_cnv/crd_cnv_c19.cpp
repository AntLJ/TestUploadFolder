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

#include "crd_cnv.h"
#include "crd_cnv_defines.h"

//------------------------------------------------------------
/*
	�P�X���W�n�̕\����郁���o�֐�
*/
//------------------------------------------------------------
void	crd_cnv::
Make19tbl	(	void	)
{
	c19_origin	origin19[19] = 
				{{{129,30,0.0},	{33,0,0.0}},	// �n�ԍ��P
				{{131,0,0.0},	{33,0,0.0}},	// �n�ԍ��Q
				{{132,10,0.0},	{36,0,0.0}},	// �n�ԍ��R
				{{133,30,0.0},	{33,0,0.0}},	// �n�ԍ��S
				{{134,20,0.0},	{36,0,0.0}},	// �n�ԍ��T
				{{136,0,0.0},	{36,0,0.0}},	// �n�ԍ��U
				{{137,10,0.0},	{36,0,0.0}},	// �n�ԍ��V
				{{138,30,0.0},	{36,0,0.0}},	// �n�ԍ��W
				{{139,50,0.0},	{36,0,0.0}},	// �n�ԍ��X
				{{140,50,0.0},	{40,0,0.0}},	// �n�ԍ��P�O
				{{140,15,0.0},	{44,0,0.0}},	// �n�ԍ��P�P
				{{142,15,0.0},	{44,0,0.0}},	// �n�ԍ��P�Q
				{{144,15,0.0},	{44,0,0.0}},	// �n�ԍ��P�R
				{{142,0,0.0},	{26,0,0.0}},	// �n�ԍ��P�S
				{{127,30,0.0},	{26,0,0.0}},	// �n�ԍ��P�T
				{{124,0,0.0},	{26,0,0.0}},	// �n�ԍ��P�U
				{{131,0,0.0},	{26,0,0.0}},	// �n�ԍ��P�V
				{{136,0,0.0},	{20,0,0.0}},	// �n�ԍ��P�W
				{{154,0,0.0},	{26,0,0.0}}};	// �n�ԍ��P�X

	// ���ʒ��p���W(19�n)�̃��W�A���̕\���쐬����
	int	i;
	for (i = 0; i < 19; i++)
	{
		Rad_Origin[i].lat	= (origin19[i].lat.deg + (origin19[i].lat.min / 60.0)) * M_PI / 180.0;
		Rad_Origin[i].lon	= (origin19[i].lon.deg + (origin19[i].lon.min / 60.0)) * M_PI / 180.0;
	//	printf("Rad_Origin[%d].lat	= %0.60f;\n", i, Rad_Origin[i].lat);
	//	printf("Rad_Origin[%d].lon	= %0.60f;\n", i, Rad_Origin[i].lon);
	}
}	

//------------------------------------------------------------
/*
	�ԓ�����ܓxPhi�܂ł̎q�ߐ��ǒ��𓾂�
	@return �q�ߐ��ǒ�(m)

	�y�Q�l�z
	http://vldb.gsi.go.jp/sokuchi/surveycalc/algorithm/b2s/b2s.htm
*/
//------------------------------------------------------------
inline	double	crd_cnv::
M_Arc_Len_Core	(	double	Ra,		// �n���ȉ~�̂̒����a�i���[�g���j
					double	ea2,	// ��ꗣ�S����2��
					double	Phi	)	// �ܓx
{
	double	tmp1 = 0.0;
	double	A=0.0, B=0.0, C=0.0, D=0.0, E=0.0, F=0.0, G=0.0, H=0.0, I=0.0;
	double	B1=0.0, B2=0.0, B3=0.0, B4=0.0, B5=0.0, B6=0.0, B7=0.0, B8=0.0, B9=0.0;
	double	arc_len=0.0;

	A	= 1.0 + 3.0*ea2/4.0 + 45.0*pow(ea2,2.0)/64.0 + 175.0*pow(ea2,3.0)/256.0 + 11025.0*pow(ea2,4.0)/16384 + 43659.0*pow(ea2,5.0)/65536.0
			+ 693693.0*pow(ea2,6.0)/1048576.0 + 19324305.0*pow(ea2,7.0)/29360128.0 + 4927697775.0*pow(ea2,8.0)/7516192768.0;

	B	= 3.0*ea2/4.0 + 15.0*pow(ea2,2.0)/16.0 + 525.0*pow(ea2,3.0)/512.0 + 2205.0*pow(ea2,4.0)/2048.0 + 72765.0*pow(ea2,5.0)/65536.0
			+ 297297.0*pow(ea2,6.0)/262144.0 + 135270135.0*pow(ea2,7.0)/117440512.0 + 547521975.0*pow(ea2,8.0)/469762048.0;

	C	= 15.0*pow(ea2,2.0)/64.0 + 105.0*pow(ea2,3.0)/256.0 + 2205.0*pow(ea2,4.0)/4096.0 + 10395.0*pow(ea2,5.0)/16384.0
			+ 1486485.0*pow(ea2,6.0)/2097152.0 + 45090045.0*pow(ea2,7.0)/58720256.0 + 766530765.0*pow(ea2,8.0)/939524096.0;

	D	= 35.0*pow(ea2,3.0)/512.0 + 315.0*pow(ea2,4.0)/2048.0 + 31185.0*pow(ea2,5.0)/131072.0 + 165165.0*pow(ea2,6.0)/524288.0
			+ 45090045.0*pow(ea2,7.0)/117440512.0 + 209053845.0*pow(ea2,8.0)/469762048.0;

	E	= 315.0*pow(ea2,4.0)/16384.0 + 3465.0*pow(ea2,5.0)/65536.0 + 99099.0*pow(ea2,6.0)/1048576.0 + 4099095.0*pow(ea2,7.0)/29360128.0
			+ 348423075.0*pow(ea2,8.0)/1879048192.0;

	F	= 693.0*pow(ea2,5.0)/131072.0 + 9009.0*pow(ea2,6.0)/524288.0 + 4099095.0*pow(ea2,7.0)/117440512.0 + 26801775.0*pow(ea2,8.0)/469762048.0;

	G	= 3003.0*pow(ea2,6.0)/2097152.0 + 315315.0*pow(ea2,7.0)/58720256.0 + 11486475.0*pow(ea2,8.0)/939524096.0;

	H	= 45045.0*pow(ea2,7.0)/117440512.0 + 765765.0*pow(ea2,8.0)/469762048.0;

	I	= 765765.0*pow(ea2,8.0)/7516192768.0;

	tmp1	= Ra*(1.0 - ea2);

	B1	= tmp1 * A;
	B2	= tmp1 * (-B/2.0);
	B3	= tmp1 * (C/4.0);
	B4	= tmp1 * (-D/6.0);
	B5	= tmp1 * (E/8.0);
	B6	= tmp1 * (-F/10.0);
	B7	= tmp1 * (G/12.0);
	B8	= tmp1 * (-H/14.0);
	B9	= tmp1 * (I/16.0);

	arc_len	= B1*Phi + B2*sin(2.0*Phi) + B3*sin(4.0*Phi) + B4*sin(6.0*Phi) + B5*sin(8.0*Phi)
				+ B6*sin(10.0*Phi) + B7*sin(12.0*Phi) + B8*sin(14.0*Phi) + B9*sin(16.0*Phi);

	return(arc_len);
}

//------------------------------------------------------------
/*
	�ԓ�����ܓxPhi�܂ł̎q�ߐ��ǒ��𓾂�
	@return �q�ߐ��ǒ�(m)
*/
//------------------------------------------------------------
inline	double	crd_cnv::
M_Arc_Len	(	double	Ra,		// �n���ȉ~�̂̒����a�i���[�g���j
				double	ea2,	// ��ꗣ�S����2��
				double	Phi	)	// �ܓx
{
	double	arc_len;

	arc_len	= Ra * (1.0 - ea2)
			* (C1 * Phi
			- C2 / 2.0 * sin(2.0 * Phi)
			+ C3 / 4.0 * sin(4.0 * Phi)
			- C4 / 6.0 * sin(6 * Phi)
			+ C5 / 8 * sin(8.0 * Phi)
			- C6 / 10.0 * sin(10.0 * Phi));

	return(arc_len);
}

//---------------------------------------------------------------------
/**
* 19���W�n�̖k�C����5���R�[�h�ƌn�̑Ή���������
*/
//---------------------------------------------------------------------
bool	crd_cnv::
Sys_Number_Init(	char*	c_fname	)	///< [in]  �ǂݍ��݃t�@�C����
{
	if( c_fname != NULL ) {
		// �t�@�C����ǂݍ��ޏꍇ�͍ŏ��ɃN���A
		m_mCode_Sys19.clear();

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
			if( r_buf[0] == '#' || r_buf[0] == '\n' ) {
				continue;
			}

			int	a_ilen	= strlen(r_buf);
			if( r_buf[a_ilen-1] == '\n' ) {
				r_buf[a_ilen-1] = '\0';
			}

			char	*delim		= "\t, ";			// �^�u�A�R���}�A�X�y�[�X��؂�
			char	*field[2]	= { NULL, NULL };	// �t�B�[���h�̕�����̃|�C���^

			field[0]	= strtok(r_buf, delim);		// �Z���R�[�h
			field[1]	= strtok(NULL, delim);		// �n�ԍ�

			// �t�B�[���h�̓ǂݎ��`�F�b�N
			int	i;
			for(i = 0; i < 2; i++)
			{
				if(field[i] == NULL) {
					return	( false );
				}
			}

			std::string		a_sCode;	// �R�[�h
			int				a_iSys_Num;	// �n�ԍ�

			a_sCode		= field[0];
			a_iSys_Num	= atoi ( field[1] );

			m_mCode_Sys19.insert(std::pair<std::string, int>( a_sCode, a_iSys_Num ));	// �f�[�^�i�[
			memset(r_buf, '\0', LINEMAX);	// �o�b�t�@�̏�����
		}
		fclose	( rfp );
		return	( true );
	}else {
		// �f�t�H���g�l�ݒ�
		// 11�n 140�x15��0�b0 44�x0��0�b0 ���M�s�@���َs�@�ɒB�s�@�k�l�s�@�_�U�x���Ǔ��̂����L��S�y�ш��c�S�@�w�R�x���Ǔ��@��u�x���Ǔ��@�n���x���Ǔ�  
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01202", 11 ) );	// �k�C��	�n���x��	���َs	�n�R�_�e�V	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01203", 11 ) );	// �k�C��	��u�x��	���M�s	�I�^���V	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01233", 11 ) );	// �k�C��	�_�U�x��	�ɒB�s	�_�e�V	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01236", 11 ) );	// �k�C��	�n���x��	�k�l�s	�z�N�g�V	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01331", 11 ) );	// �k�C��	�n���x��	���O��	�}�c�}�G�`���E	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01332", 11 ) );	// �k�C��	�n���x��	������	�t�N�V�}�`���E	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01333", 11 ) );	// �k�C��	�n���x��	�m����	�V���E�`�`���E	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01334", 11 ) );	// �k�C��	�n���x��	�،Ó���	�L�R�i�C�`���E	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01337", 11 ) );	// �k�C��	�n���x��	���ђ�	�i�i�G�`���E	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01343", 11 ) );	// �k�C��	�n���x��	������	�V�J�x�`���E	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01345", 11 ) );	// �k�C��	�n���x��	�X��	�����}�`	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01346", 11 ) );	// �k�C��	�n���x��	���_��	���N���`���E	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01347", 11 ) );	// �k�C��	�n���x��	��������	�I�V���}���x�`���E	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01361", 11 ) );	// �k�C��	�w�R�x��	�]����	�G�T�V�`���E	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01362", 11 ) );	// �k�C��	�w�R�x��	��m����	�J�~�m�N�j�`���E	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01363", 11 ) );	// �k�C��	�w�R�x��	���򕔒�	�A�b�T�u�`���E	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01364", 11 ) );	// �k�C��	�w�R�x��	������	�I�g�x�`���E	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01367", 11 ) );	// �k�C��	�w�R�x��	���K��	�I�N�V���`���E	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01370", 11 ) );	// �k�C��	�w�R�x��	������	�C�}�J�l�`���E	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01371", 11 ) );	// �k�C��	�w�R�x��	�����Ȓ�	�Z�^�i�`���E	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01391", 11 ) );	// �k�C��	��u�x��	���q��	�V�}�}�L����	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01392", 11 ) );	// �k�C��	��u�x��	���s��	�X�b�c�`���E	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01393", 11 ) );	// �k�C��	��u�x��	��������	�N���}�c�i�C�`���E	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01394", 11 ) );	// �k�C��	��u�x��	���z��	�����R�V�`���E	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01395", 11 ) );	// �k�C��	��u�x��	�j�Z�R��	�j�Z�R�`���E	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01396", 11 ) );	// �k�C��	��u�x��	�^�둺	�}�b�J������	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01397", 11 ) );	// �k�C��	��u�x��	�����s��	���X�c����	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01398", 11 ) );	// �k�C��	��u�x��	��Εʒ�	�L���x�c�`���E	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01399", 11 ) );	// �k�C��	��u�x��	���ɒ�	�L���E�S�N�`���E	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01400", 11 ) );	// �k�C��	��u�x��	��m����	�N�b�`�����`���E	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01401", 11 ) );	// �k�C��	��u�x��	���a��	�L���E���`���E	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01402", 11 ) );	// �k�C��	��u�x��	�����	�C���i�C�`���E	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01403", 11 ) );	// �k�C��	��u�x��	����	�g�}������	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01404", 11 ) );	// �k�C��	��u�x��	�_�b����	�J���G�i�C����	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01405", 11 ) );	// �k�C��	��u�x��	�ϒO��	�V���R�^���`���E	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01406", 11 ) );	// �k�C��	��u�x��	�Õ���	�t���r���`���E	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01407", 11 ) );	// �k�C��	��u�x��	�m�ؒ�	�j�L�`���E	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01408", 11 ) );	// �k�C��	��u�x��	�]�s��	���C�`�`���E	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01409", 11 ) );	// �k�C��	��u�x��	�Ԉ�쑺	�A�J�C�K������	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01571", 11 ) );	// �k�C��	�_�U�x��	�L�Y��	�g���E���`���E	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01575", 11 ) );	// �k�C��	�_�U�x��	�s�˒�	�\�E�x�c�`���E	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01584", 11 ) );	// �k�C��	�_�U�x��	����Β�	�g�E���R�`���E	11
		// 12�n 142�x15��0�b0 44�x0��0�b0 �D�y�s�@����s�@�t���s�@���G�s�@���S�s�@�[���s �〈��s�@�Ϗ��q�s�@�����s�@�m�ʎs�@����s�@���ʎs�@�ԕ��s�@�O�}�s�@���s�@����s�@�]�ʎs ��Ύs�@�̎u���s�@�[��s�@��ʎs�@�x�ǖ�s�@�o�ʎs�@�b��s�@�k�L���s�@�Ύ�s�@�Ύ�x���Ǔ� �ԑ��x���Ǔ��̂�����ʌS�@���x���Ǔ��@�@�J�x���Ǔ��@�����x���Ǔ��@�_�U�x���Ǔ��i�L��S�y�ш��c�S�������B)�@��m�x���Ǔ��@���G�x���Ǔ�  
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01100", 12 ) );	// �k�C��	�Ύ�x��	�D�y�s	�T�b�|���V	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01101", 12 ) );	// �k�C��	�Ύ�x��	�D�y�s_������	�T�b�|���V�`���E�I�E�N	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01102", 12 ) );	// �k�C��	�Ύ�x��	�D�y�s_�k��	�T�b�|���V�L�^�N	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01103", 12 ) );	// �k�C��	�Ύ�x��	�D�y�s_����	�T�b�|���V�q�K�V�N	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01104", 12 ) );	// �k�C��	�Ύ�x��	�D�y�s_���΋�	�T�b�|���V�V���C�V�N	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01105", 12 ) );	// �k�C��	�Ύ�x��	�D�y�s_�L����	�T�b�|���V�g���q���N	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01106", 12 ) );	// �k�C��	�Ύ�x��	�D�y�s_���	�T�b�|���V�~�i�~�N	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01107", 12 ) );	// �k�C��	�Ύ�x��	�D�y�s_����	�T�b�|���V�j�V�N	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01108", 12 ) );	// �k�C��	�Ύ�x��	�D�y�s_���ʋ�	�T�b�|���V�A�c�x�c�N	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01109", 12 ) );	// �k�C��	�Ύ�x��	�D�y�s_����	�T�b�|���V�e�C�l�N	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01110", 12 ) );	// �k�C��	�Ύ�x��	�D�y�s_���c��	�T�b�|���V�L���^�N	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01204", 12 ) );	// �k�C��	���x��	����s	�A�T�q�J���V	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01205", 12 ) );	// �k�C��	�_�U�x��	�����s	���������V	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01209", 12 ) );	// �k�C��	��m�x��	�[���s	���E�o���V	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01210", 12 ) );	// �k�C��	��m�x��	�〈��s	�C���~�U���V	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01212", 12 ) );	// �k�C��	���G�x��	���G�s	�����C�V	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01213", 12 ) );	// �k�C��	�_�U�x��	�Ϗ��q�s	�g�}�R�}�C�V	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01214", 12 ) );	// �k�C��	�@�J�x��	�t���s	���b�J�i�C�V	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01215", 12 ) );	// �k�C��	��m�x��	���S�s	�r�o�C�V	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01216", 12 ) );	// �k�C��	��m�x��	���ʎs	�A�V�x�c�V	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01217", 12 ) );	// �k�C��	�Ύ�x��	�]�ʎs	�G�x�c�V	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01218", 12 ) );	// �k�C��	��m�x��	�ԕ��s	�A�J�r���V	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01219", 12 ) );	// �k�C��	�ԑ��x��	��ʎs	�����x�c�V	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01220", 12 ) );	// �k�C��	���x��	�m�ʎs	�V�x�c�V	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01221", 12 ) );	// �k�C��	���x��	����s	�i�����V	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01222", 12 ) );	// �k�C��	��m�x��	�O�}�s	�~�J�T�V	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01224", 12 ) );	// �k�C��	�Ύ�x��	��Ύs	�`�g�Z�V	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01225", 12 ) );	// �k�C��	��m�x��	���s	�^�L�J���V	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01226", 12 ) );	// �k�C��	��m�x��	����s	�X�i�K���V	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01227", 12 ) );	// �k�C��	��m�x��	�̎u���s	�E�^�V�i�C�V	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01228", 12 ) );	// �k�C��	��m�x��	�[��s	�t�J�K���V	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01229", 12 ) );	// �k�C��	���x��	�x�ǖ�s	�t���m�V	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01230", 12 ) );	// �k�C��	�_�U�x��	�o�ʎs	�m�{���x�c�V	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01231", 12 ) );	// �k�C��	�Ύ�x��	�b��s	�G�j���V	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01234", 12 ) );	// �k�C��	�Ύ�x��	�k�L���s	�L�^�q���V�}�V	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01235", 12 ) );	// �k�C��	�Ύ�x��	�Ύ�s	�C�V�J���V	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01303", 12 ) );	// �k�C��	�Ύ�x��	���ʒ�	�g�E�x�c�`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01304", 12 ) );	// �k�C��	�Ύ�x��	�V�Ñ�	�V���V�m�c����	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01423", 12 ) );	// �k�C��	��m�x��	��y��	�i���|���`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01424", 12 ) );	// �k�C��	��m�x��	�ވ�]��	�i�C�G�`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01425", 12 ) );	// �k�C��	��m�x��	�㍻�쒬	�J�~�X�i�K���`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01427", 12 ) );	// �k�C��	��m�x��	�R�m��	���j�`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01428", 12 ) );	// �k�C��	��m�x��	������	�i�K�k�}�`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01429", 12 ) );	// �k�C��	��m�x��	�I�R��	�N�����}�`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01430", 12 ) );	// �k�C��	��m�x��	���`��	�c�L�K�^�`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01431", 12 ) );	// �k�C��	��m�x��	�Y�P��	�E���E�X�`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01432", 12 ) );	// �k�C��	��m�x��	�V�\�Ð쒬	�V���g�c�J���`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01433", 12 ) );	// �k�C��	��m�x��	���w����	���Z�E�V�`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01434", 12 ) );	// �k�C��	��m�x��	�����ʒ�	�`�b�v�x�c�`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01436", 12 ) );	// �k�C��	��m�x��	�J����	�E�����E�`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01437", 12 ) );	// �k�C��	��m�x��	�k����	�z�N�����E�`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01438", 12 ) );	// �k�C��	��m�x��	���c��	�k�}�^�`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01452", 12 ) );	// �k�C��	���x��	�鐲��	�^�J�X�`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01453", 12 ) );	// �k�C��	���x��	���_�y��	�q�K�V�J�O���`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01454", 12 ) );	// �k�C��	���x��	������	�g�E�}�`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01455", 12 ) );	// �k�C��	���x��	��z��	�s�b�v�`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01456", 12 ) );	// �k�C��	���x��	���ʒ�	�A�C�x�c�`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01457", 12 ) );	// �k�C��	���x��	��쒬	�J�~�J���`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01458", 12 ) );	// �k�C��	���x��	���쒬	�q�K�V�J���`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01459", 12 ) );	// �k�C��	���x��	���l��	�r�G�C�`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01460", 12 ) );	// �k�C��	���x��	��x�ǖ쒬	�J�~�t���m�`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01461", 12 ) );	// �k�C��	���x��	���x�ǖ쒬	�i�J�t���m�`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01462", 12 ) );	// �k�C��	���x��	��x�ǖ쒬	�~�i�~�t���m�`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01463", 12 ) );	// �k�C��	���x��	�芥��	�V���J�b�v����	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01464", 12 ) );	// �k�C��	���x��	�a����	���b�T���`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01465", 12 ) );	// �k�C��	���x��	������	�P���u�`�`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01468", 12 ) );	// �k�C��	���x��	���쒬	�V���J���`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01469", 12 ) );	// �k�C��	���x��	���[��	�r�t�J�`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01470", 12 ) );	// �k�C��	���x��	���Ўq�{��	�I�g�C�l�b�v����	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01471", 12 ) );	// �k�C��	���x��	���쒬	�i�J�K���`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01472", 12 ) );	// �k�C��	���x��	�y������	�z���J�i�C�`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01481", 12 ) );	// �k�C��	���G�x��	���ђ�	�}�V�P�`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01482", 12 ) );	// �k�C��	���G�x��	������	�I�r���`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01483", 12 ) );	// �k�C��	���G�x��	�ϑO��	�g�}�}�G�`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01484", 12 ) );	// �k�C��	���G�x��	�H�y��	�n�{���`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01485", 12 ) );	// �k�C��	���G�x��	���R�ʑ�	�V���T���x�c����	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01486", 12 ) );	// �k�C��	���G�x��	���ʒ�	�G���x�c�`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01487", 12 ) );	// �k�C��	���G�x��	�V����	�e�V�I�`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01511", 12 ) );	// �k�C��	�@�J�x��	������	�T���t�c����	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01512", 12 ) );	// �k�C��	�@�J�x��	�l�ڕʒ�	�n�}�g���x�c�`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01513", 12 ) );	// �k�C��	�@�J�x��	���ڕʒ�	�i�J�g���x�c�`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01514", 12 ) );	// �k�C��	�@�J�x��	�}�K��	�G�T�V�`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01516", 12 ) );	// �k�C��	�@�J�x��	�L�x��	�g���g�~�`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01517", 12 ) );	// �k�C��	�@�J�x��	�當��	���u���`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01518", 12 ) );	// �k�C��	�@�J�x��	���K��	���V���`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01519", 12 ) );	// �k�C��	�@�J�x��	���K�x�m��	���V���t�W�`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01520", 12 ) );	// �k�C��	�@�J�x��	�y����	�z���m�x�`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01555", 12 ) );	// �k�C��	�ԑ��x��	���y��	�G���K���`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01558", 12 ) );	// �k�C��	�ԑ��x��	��N�ʒ�	�J�~���E�x�c�`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01559", 12 ) );	// �k�C��	�ԑ��x��	�N�ʒ�	���E�x�c�`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01560", 12 ) );	// �k�C��	�ԑ��x��	��㒬	�^�L�m�E�G�`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01561", 12 ) );	// �k�C��	�ԑ��x��	������	�I�R�b�y�`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01562", 12 ) );	// �k�C��	�ԑ��x��	��������	�j�V�I�R�b�y����	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01563", 12 ) );	// �k�C��	�ԑ��x��	�Y����	�I�E���`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01578", 12 ) );	// �k�C��	�_�U�x��	���V��	�V���I�C�`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01581", 12 ) );	// �k�C��	�_�U�x��	���^��	�A�c�}�`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01585", 12 ) );	// �k�C��	�_�U�x��	������	�A�r���`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01586", 12 ) );	// �k�C��	�_�U�x��	�ނ��풬	���J���`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01601", 12 ) );	// �k�C��	�����x��	������	�q�_�J�`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01602", 12 ) );	// �k�C��	�����x��	���撬	�r���g���`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01604", 12 ) );	// �k�C��	�����x��	�V����	�j�C�J�b�v�`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01607", 12 ) );	// �k�C��	�����x��	�Y�͒�	�E���J���`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01608", 12 ) );	// �k�C��	�����x��	�l����	�T�}�j�`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01609", 12 ) );	// �k�C��	�����x��	�������	�G�����`���E	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01610", 12 ) );	// �k�C��	�����x��	�V�Ђ�����	�V���q�_�J�`���E	12
		// 13�n 144�x15��0�b0 44�x0��0�b0 �k���s�@�эL�s�@���H�s�@�ԑ��s�@�����s�@�����x���Ǔ��@���H�x���Ǔ��@�ԑ��x���Ǔ��i��ʌS�������B)�@�\���x���Ǔ�  
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01206", 13 ) );	// �k�C��	���H�x��	���H�s	�N�V���V	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01207", 13 ) );	// �k�C��	�\���x��	�эL�s	�I�r�q���V	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01208", 13 ) );	// �k�C��	�ԑ��x��	�k���s	�L�^�~�V	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01211", 13 ) );	// �k�C��	�ԑ��x��	�ԑ��s	�A�o�V���V	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01223", 13 ) );	// �k�C��	�����x��	�����s	�l�����V	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01543", 13 ) );	// �k�C��	�ԑ��x��	���y��	�r�z���`���E	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01544", 13 ) );	// �k�C��	�ԑ��x��	�Õʒ�	�c�x�c�`���E	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01545", 13 ) );	// �k�C��	�ԑ��x��	�Η���	�V�����`���E	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01546", 13 ) );	// �k�C��	�ԑ��x��	������	�L���T�g�`���E	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01547", 13 ) );	// �k�C��	�ԑ��x��	��������	�R�V�~�Y�`���E	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01549", 13 ) );	// �k�C��	�ԑ��x��	�P�q�{��	�N���l�b�v�`���E	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01550", 13 ) );	// �k�C��	�ԑ��x��	�u�˒�	�I�P�g�`���E	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01552", 13 ) );	// �k�C��	�ԑ��x��	���C�Ԓ�	�T���}�`���E	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01564", 13 ) );	// �k�C��	�ԑ��x��	���	�I�I�]���`���E	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01631", 13 ) );	// �k�C��	�\���x��	���X��	�I�g�t�P�`���E	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01632", 13 ) );	// �k�C��	�\���x��	�m�y��	�V�z���`���E	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01633", 13 ) );	// �k�C��	�\���x��	��m�y��	�J�~�V�z���`���E	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01634", 13 ) );	// �k�C��	�\���x��	���ǒ�	�V�J�I�C�`���E	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01635", 13 ) );	// �k�C��	�\���x��	�V����	�V���g�N�`���E	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01636", 13 ) );	// �k�C��	�\���x��	������	�V�~�Y�`���E	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01637", 13 ) );	// �k�C��	�\���x��	�莺��	�������`���E	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01638", 13 ) );	// �k�C��	�\���x��	���D����	�i�J�T�c�i�C����	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01639", 13 ) );	// �k�C��	�\���x��	�X�ʑ�	�T���x�c����	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01641", 13 ) );	// �k�C��	�\���x��	�����	�^�C�L�`���E	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01642", 13 ) );	// �k�C��	�\���x��	�L����	�q���I�`���E	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01643", 13 ) );	// �k�C��	�\���x��	���ʒ�	�}�N�x�c�`���E	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01644", 13 ) );	// �k�C��	�\���x��	�r�c��	�C�P�_�`���E	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01645", 13 ) );	// �k�C��	�\���x��	�L����	�g���R���`���E	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01646", 13 ) );	// �k�C��	�\���x��	�{�ʒ�	�z���x�c�`���E	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01647", 13 ) );	// �k�C��	�\���x��	����	�A�V�����`���E	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01648", 13 ) );	// �k�C��	�\���x��	���ʒ�	���N�x�c�`���E	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01649", 13 ) );	// �k�C��	�\���x��	�Y�y��	�E���z���`���E	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01661", 13 ) );	// �k�C��	���H�x��	���H��	�N�V���`���E	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01662", 13 ) );	// �k�C��	���H�x��	���ݒ�	�A�b�P�V�`���E	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01663", 13 ) );	// �k�C��	���H�x��	�l����	�n�}�i�J�`���E	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01664", 13 ) );	// �k�C��	���H�x��	�W����	�V�x�`���`���E	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01665", 13 ) );	// �k�C��	���H�x��	��q����	�e�V�J�K�`���E	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01667", 13 ) );	// �k�C��	���H�x��	�ߋ���	�c���C����	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01668", 13 ) );	// �k�C��	���H�x��	���f��	�V���k�J�`���E	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01691", 13 ) );	// �k�C��	�����x��	�ʊC��	�x�b�J�C�`���E	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01692", 13 ) );	// �k�C��	�����x��	���W�Ò�	�i�J�V�x�c�`���E	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01693", 13 ) );	// �k�C��	�����x��	�W�Ò�	�V�x�c�`���E	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01694", 13 ) );	// �k�C��	�����x��	���P��	���E�X�`���E	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01695", 13 ) );	// �k�C��	�����x��	�F�O��	�V�R�^������	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01696", 13 ) );	// �k�C��	�����x��	����	�g�}������	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01697", 13 ) );	// �k�C��	�����x��	����ʑ�	�����x�c����	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01698", 13 ) );	// �k�C��	�����x��	���ʑ�	���x�c����	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01699", 13 ) );	// �k�C��	�����x��	�ѓߑ�	�V���i����	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01700", 13 ) );	// �k�C��	�����x��	�A�摺	�V�x�g������	13
	}
	return	( false );
}

//------------------------------------------------------------
/*
	�o�ܓx�����ԋ߂��Ƃ����19���W�n�̌n�𓾂�
	�i�Ȃ񂩖��ʂȂ��Ƃ����Ă�����ۂ��j
	@return		�n�ԍ�
*/
//------------------------------------------------------------
int	crd_cnv::
Sys_Number	(	double		lon,	// �o�x
				double		lat)	// �ܓx
{
	double	d_lat,	d_lon;
	double	dist			= 9999999.0;	// ���ݒ��ׂĂ��鋗��
	double	dist_n1			= 9999999.0;	// �ł��߂���������
	double	dist_n2			= 9999999.0;	// �Q�Ԗڂɋ߂���������
	int		sys_n1			= 0;
	int		sys_n2			= 0;
	int		i;

//	Make19tbl();	// 19���W�n�̃e�[�u�����쐬
		
	// degree��radian�ɕϊ�
	lat	= lat * M_PI / 180.0;
	lon	= lon * M_PI / 180.0;

	for(i = 0; i < 19; i++)
	{
		d_lat	= Rad_Origin[i].lat - lat;
		d_lon	= Rad_Origin[i].lon - lon;
		dist	= sqrt(pow(d_lat, 2.0) + pow(d_lon, 2.0));
		if(dist < dist_n1)
		{
			sys_n1		= i + 1;
			dist_n1		= dist;
		}
		else
		if(dist < dist_n2)
		{
			sys_n2		= i + 1;
			dist_n2		= dist;
		}
	}
	return(sys_n1);
}

//------------------------------------------------------------
/*
	�o�ܓx�Ǝs�撬���R�[�h(5��)����19���W�n�̌n�𓾂�
	@return     �n�ԍ�(�s�撬���R�[�h��o�ܓx�ɊԈႢ������ꍇ��0��ԋp)
*/
//------------------------------------------------------------
int	crd_cnv::
Sys_Number	(	double	lon,	// [in]  �o�x
				double	lat,	// [in]  �ܓx
				char*	code5 )	// [in]  �s�撬���R�[�h(5��)
{
	// �e�[�u�����󂾂����ꍇ�A��x�f�t�H���g�̏�����������B
	// �i���ɊO���ŏ��������{���Ă���ꍇ�A�����͒ʂ�Ȃ��j
	if( m_mCode_Sys19.size() == 0 ) {
		Sys_Number_Init();
	}

	if( code5 == NULL ) {
		return ( 0 );
	}else
	if( strlen(code5) < 5 ) {
		return ( 0 ); 
	}
	// 1�n 129�x30��0�b0 33�x0��0�b0 ���茧 ���������̂����k���k��32�x����k��27�x�������o128�x18���������o130�x�����E���Ƃ�������i�����Q���͓��o130�x13���܂ł��܂ށB)�ɂ��邷�ׂĂ̓��A�����A�ʋy�ъ��  
	if( strncmp( code5, "42", 2 ) == 0 ) {	// ���茧
		return	( 1 );
	}else
	if( strncmp( code5, "46", 2 ) == 0 ){	// ��������
		// �k���k��32�x ����k��27�x �������o128�x18�� �������o130�x
		if(	27.0 <= lat && lat <= 32.0 && 
			128.30 <= lon && lon <= 130.0 ) {
			return	( 1 );
		}else {
			// 2�n 131�x 0��0�b0 33�x0��0�b0 �������@���ꌧ�@�F�{���@�啪���@�{�茧�@���������i��I�n�ɋK�肷����������B)  
			return	( 2 );
		}
	}else
	// 2�n 131�x 0��0�b0 33�x0��0�b0 �������@���ꌧ�@�F�{���@�啪���@�{�茧�@���������i��I�n�ɋK�肷����������B)  
	if( strncmp( code5, "40", 2 ) == 0 ||	// ������
		strncmp( code5, "41", 2 ) == 0 ||	// ���ꌧ
		strncmp( code5, "43", 2 ) == 0 ||	// �F�{��
		strncmp( code5, "44", 2 ) == 0 ||	// �啪��
		strncmp( code5, "45", 2 ) == 0 ) {	// �{�茧
		return	( 2 );
	}else
	// 3�n 132�x10��0�b0 36�x0��0�b0 �R�����@�������@�L����  
	if( strncmp( code5, "32", 2 ) == 0 ||	// ������
		strncmp( code5, "34", 2 ) == 0 ||	// �L����
		strncmp( code5, "35", 2 ) == 0 ) {	// �R����
		return	( 3 );
	}else
	// 4�n 133�x30��0�b0 33�x0��0�b0 ���쌧�@���Q���@�������@���m��  
	if( strncmp( code5, "36", 2 ) == 0 ||	// ������
		strncmp( code5, "37", 2 ) == 0 ||	// ���쌧
		strncmp( code5, "38", 2 ) == 0 ||	// ���Q��
		strncmp( code5, "39", 2 ) == 0 ) {	// ���m��
		return	( 4 );
	}else
	// 5�n 134�x20��0�b0 36�x0��0�b0 ���Ɍ��@���挧�@���R��  
	if( strncmp( code5, "28", 2 ) == 0 ||	// ���Ɍ�
		strncmp( code5, "31", 2 ) == 0 ||	// ���挧
		strncmp( code5, "33", 2 ) == 0 ) {	// ���R��
		return	( 5 );
	}else
	// 6�n 136�x 0��0�b0 36�x0��0�b0 ���s�{�@���{�@���䌧�@���ꌧ�@�O�d���@�ޗǌ� �a�̎R�� 
	if( strncmp( code5, "18", 2 ) == 0 ||	// ���䌧
		strncmp( code5, "24", 2 ) == 0 ||	// �O�d��
		strncmp( code5, "25", 2 ) == 0 ||	// ���ꌧ
		strncmp( code5, "26", 2 ) == 0 ||	// ���s�{
		strncmp( code5, "27", 2 ) == 0 ||	// ���{
		strncmp( code5, "29", 2 ) == 0 ||	// �ޗǌ�
		strncmp( code5, "30", 2 ) == 0 ) {	// �a�̎R��
		return	( 6 );
	}else
	// 7�n 137�x10��0�b0 36�x0��0�b0 �ΐ쌧�@�x�R���@�򕌌��@���m��  
	if( strncmp( code5, "16", 2 ) == 0 ||	// �x�R��	7
		strncmp( code5, "17", 2 ) == 0 ||	// �ΐ쌧	7
		strncmp( code5, "21", 2 ) == 0 ||	// �򕌌�	7
		strncmp( code5, "23", 2 ) == 0 ) {	// ���m��	7
		return	( 7 );
	}else
	// 8�n 138�x30��0�b0 36�x0��0�b0 �V�����@���쌧�@�R�����@�É���  
	if( strncmp( code5, "15", 2 ) == 0 ||	// �V����	8
		strncmp( code5, "19", 2 ) == 0 ||	// �R����	8
		strncmp( code5, "20", 2 ) == 0 ||	// ���쌧	8
		strncmp( code5, "22", 2 ) == 0 ) {	// �É���	8
		return	( 8 );
	}else
	// 9�n 139�x50��0�b0 36�x0��0�b0 �����s�i14�n�A18�n�y��19�n�ɋK�肷����������B)�@�������@�Ȗ،��@��錧�@��ʌ� ��t�� �Q�n���@�_�ސ쌧  
	if( strncmp( code5, "13", 2 ) == 0 ) {	// �����s
		if(	lat <= 28.0 ) {		//  �k��28�x�����
			// 14�n 142�x 0��0�b0 26�x0��0�b0 �����s�̂����k��28�x�����ł���A�����o140�x30�����瓌�ł��蓌�o143�x���琼�ł�����  
			if( 140.50 <= lon && lon <= 143.0 ) {	// ���o140�x30�����瓌�ł��蓌�o143�x���琼
				return	( 14 );
			}else
			// 18�n 136�x 0��0�b0 20�x0��0�b0 �����s�̂����k��28�x�����ł���A�����o140�x30�����琼�ł�����
			if( lon < 140.50 ) {	// ���o140�x30�����琼
				return	( 18 );
			}else
			// 19�n 154�x 0��0�b0 26�x0��0�b0 �����s�̂����k��28�x�����ł���A�����o143�x���瓌�ł�����
			if( lon > 143.0 ) {		// ���o143�x���瓌
				return	( 19 );
			}
		}
		return	( 9 );
	}else
	// 9�n 139�x50��0�b0 36�x0��0�b0 �����s�i14�n�A18�n�y��19�n�ɋK�肷����������B)�@�������@�Ȗ،��@��錧�@��ʌ� ��t�� �Q�n���@�_�ސ쌧  
	if( strncmp( code5, "07", 2 ) == 0 ||	// ������	9
		strncmp( code5, "08", 2 ) == 0 ||	// ��錧	9
		strncmp( code5, "09", 2 ) == 0 ||	// �Ȗ،�	9
		strncmp( code5, "10", 2 ) == 0 ||	// �Q�n��	9
		strncmp( code5, "11", 2 ) == 0 ||	// ��ʌ�	9
		strncmp( code5, "12", 2 ) == 0 ||	// ��t��	9
		strncmp( code5, "14", 2 ) == 0 ) {	// �_�ސ쌧	9
		return	( 9 );
	}else
	// 10�n 140�x50��0�b0 40�x0��0�b0 �X���@�H�c���@�R�`���@��茧�@�{�錧  
	if( strncmp( code5, "02", 2 ) == 0 ||	// �X��	10
		strncmp( code5, "03", 2 ) == 0 ||	// ��茧	10
		strncmp( code5, "04", 2 ) == 0 ||	// �{�錧	10
		strncmp( code5, "05", 2 ) == 0 ||	// �H�c��	10
		strncmp( code5, "06", 2 ) == 0 ) {	// �R�`��	10
		return	( 10 );
	}else
	// 11�n 140�x15��0�b0 44�x0��0�b0 ���M�s�@���َs�@�ɒB�s�@�k�l�s�@�_�U�x���Ǔ��̂����L��S�y�ш��c�S�@�w�R�x���Ǔ��@��u�x���Ǔ��@�n���x���Ǔ�  
	// 12�n 142�x15��0�b0 44�x0��0�b0 �D�y�s�@����s�@�t���s�@���G�s�@���S�s�@�[���s �〈��s�@�Ϗ��q�s�@�����s�@�m�ʎs�@����s�@���ʎs�@�ԕ��s�@�O�}�s�@���s�@����s�@�]�ʎs ��Ύs�@�̎u���s�@�[��s�@��ʎs�@�x�ǖ�s�@�o�ʎs�@�b��s�@�k�L���s�@�Ύ�s�@�Ύ�x���Ǔ� �ԑ��x���Ǔ��̂�����ʌS�@���x���Ǔ��@�@�J�x���Ǔ��@�����x���Ǔ��@�_�U�x���Ǔ��i�L��S�y�ш��c�S�������B)�@��m�x���Ǔ��@���G�x���Ǔ�  
	// 13�n 144�x15��0�b0 44�x0��0�b0 �k���s�@�эL�s�@���H�s�@�ԑ��s�@�����s�@�����x���Ǔ��@���H�x���Ǔ��@�ԑ��x���Ǔ��i��ʌS�������B)�@�\���x���Ǔ�  
	if(	strncmp( code5, "01", 2 ) == 0 ) {
		std::map<std::string, int>::iterator	a_iIndx;
		a_iIndx	= m_mCode_Sys19.find ( code5 );
		if( a_iIndx != m_mCode_Sys19.end() ) {
			return	( (*a_iIndx).second );
		}else {
			return	( -1 );
		}
	}else
	if( strncmp( code5, "47", 2 ) == 0 ) {		// ���ꌧ
		// 15�n 127�x30��0�b0 26�x0��0�b0 ���ꌧ�̂������o126�x���瓌�ł���A�����o130�x���琼�ł�����
		if( 126.0 <= lon && lon <= 130.0 ) {	// ���o126�x���瓌�ł���A�����o130�x���琼
			return	( 15 );
		}else
		// 16�n 124�x 0��0�b0 26�x0��0�b0 ���ꌧ�̂������o126�x���琼�ł�����
		if( lon < 126.0 ) {
			return	( 16 );
		}else
		// 17�n 131�x 0��0�b0 26�x0��0�b0 ���ꌧ�̂������o130�x���瓌�ł�����
		if( lon > 130.0 ) {
			return	( 17 );
		}
	}
	// �G���[
	return	( -1 );
}

//------------------------------------------------------------
/*
	���b�V���R�[�h����Y������P�X���W�n�̐��Ɣԍ��𓾂� \n
	�Y������n���Ȃ��ꍇ�A���A�ԍ��Ƃ���0��Ԃ�
	@return �\���� sys_num �ւ̃|�C���^
*/
//------------------------------------------------------------
sys_num		*crd_cnv::
Sys_Number	(	int		meshcode	)		// ���b�V���R�[�h
{
	static	sys_num		sys;

	switch(meshcode){
		case	392725:		//15	�ߔe�s
			sys.total	= 1;
			sys.sys[0]	= 15;
			return(&sys);
		case	473024:		//2		�������s
		case	473034:		//2		�������s
		case	473163:		//2		�{��s
		case	473173:		//2		�{��s
			sys.total	= 1;
			sys.sys[0]	= 2;
			return(&sys);
		case	492906:		//1		����s
		case	492907:		//1		����s
		case	492916:		//1		����s
		case	492917:		//1		����s
		case	492955:		//1		�����ێs
		case	492965:		//1		�����ێs
			sys.total	= 1;
			sys.sys[0]	= 1;
			return(&sys);
		case	493015:		//2		�F�{�s
		case	493062:		//2		����s
		case	493072:		//2		����s
		case	493074:		//2		�v���Ďs
		case	493164:		//2		����s	�啪�s
		case	493165:		//2		�啪�s
		case	493173:		//2		�ʕ{�s
		case	493174:		//2		�ʕ{�s	�啪�s
		case	503022:		//2		�߉ϐ쒬
		case	503023:		//2		�t���s	����s	�߉ϐ쒬	�u�ƒ�	�{�b��
			sys.total	= 1;
			sys.sys[0]	= 2;
			return(&sys);
		case	503032:		//		�Y���Ȃ�
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
		case	503033:		//2		�u�ƒ�	�{�b��	������
		case	503065:		//2		������
			sys.total	= 1;
			sys.sys[0]	= 2;
			return(&sys);
		case	503066:		//		�Y���Ȃ�
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
		case	503067:		//3		���֎s
		case	503077:		//3		���֎s
		case	503171:		//3		�F���s
		case	503172:		//3		�F���s
			sys.total	= 1;
			sys.sys[0]	= 3;
			return(&sys);
		case	503255:		//4		���R�s
		case	503256:		//4		���R�s
		case	503265:		//4		���R�s
		case	503266:		//4		���R�s
		case	503324:		//4		���m�s
		case	503372:		//4		�V���l�s
			sys.total	= 1;
			sys.sys[0]	= 4;
			return(&sys);
		case	513104:		//3		�R���s	�h�{�s
		case	513106:		//3		���R�s
		case	513113:		//3		�F���s	�R���s	�h�{�s
		case	513123:		//3		�R���s
			sys.total	= 1;
			sys.sys[0]	= 3;
			return(&sys);
		case	513207:		//4		�����s
			sys.total	= 1;
			sys.sys[0]	= 4;
			return(&sys);
		case	513211:		//3		�⍑�s
		case	513221:		//3		�⍑�s	�a�ؒ�	��|�s
		case	513224:		//3		���s
		case	513234:		//3		���s	�⒬
		case	513242:		//3		�����s�s
		case	513243:		//3		�{����	�⒬
		case	513244:		//3		�{����	�C�c��	�⒬
			sys.total	= 1;
			sys.sys[0]	= 3;
			return(&sys);
		case	513253:		//		�Y���Ȃ�
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
		case	513254:		//3		���L���s	�{����
			sys.total	= 1;
			sys.sys[0]	= 3;
			return(&sys);
		case	513300:		//4		�����s
			sys.total	= 1;
			sys.sys[0]	= 4;
			return(&sys);
		case	513341:		//3		�����s	���R�s
		case	513352:		//3		���R�s
		case	513353:		//3		�����s	���R�s
			sys.total	= 1;
			sys.sys[0]	= 3;
			return(&sys);
		case	513365:		//5		�q�~�s
		case	513366:		//5		���R�s	�q�~�s
		case	513375:		//5		�q�~�s
		case	513376:		//5		���R�s	�q�~�s
		case	513377:		//5		���R�s
			sys.total	= 1;
			sys.sys[0]	= 5;
			return(&sys);
		case	513404:		//4		�����s
		case	513430:		//4		�����s
		case	513440:		//4		�����s
			sys.total	= 1;
			sys.sys[0]	= 4;
			return(&sys);
		case	513477:		//5		���Ύs
			sys.total	= 1;
			sys.sys[0]	= 5;
			return(&sys);
		case	513521:		//6		�a�̎R�s
		case	513541:		//6		���s	���s	�c�K��				
		case	513542:		//6		���s	���s	�c�K��	�F�撬	�L�ˎs	�򍲖�s	
		case	513551:		//6		���s	���s	�c�K��	�L�ˎs	�򍲖�s		
		case	513552:		//6		������	�c�K��	�ݘa�c�s	�L�ˎs	�򍲖�s		
		case	513553:		//6		������	��s	�ݘa�c�s	�L�ˎs	�a��s		
		case	513554:		//6		��s	�x�c�юs	�͓�����s	�a��s	��㋷�R�s		
		case	513562:		//6		��s	�ݘa�c�s	���Îs	�L�ˎs	���Ύs	������	
		case	513563:		//6		��s	���Îs	�a��s	���Ύs	������		
		case	513564:		//6		��s �����s �Q����s �����s �����s �H�g��s ���䎛�s ��㋷�R�s ������
		case	513565:		//6		�x�c�юs �����s �H�g��s ���䎛�s ��a���c�s ���Ŏs �V���� �c���� ��q�� ������ �L�˒� �͍���
			sys.total	= 1;
			sys.sys[0]	= 6;
			return(&sys);
		case	513570:		//5		���Ύs						
			sys.total	= 1;
			sys.sys[0]	= 5;
			return(&sys);
		case	513571:		//6		������						
			sys.total	= 1;
			sys.sys[0]	= 6;
			return(&sys);
		case	513573:		//5	6	���s(5)	��s(6)					
			sys.total	= 2;
			sys.sys[0]	= 5;
			sys.sys[1]	= 6;
			return(&sys);
		case	513574:		//6		��s	�����s	�����s	�����s	���䎛�s	�����s	
		case	513575:		//6		�����s	�����s	�����s	�ޗǎs	��a�S�R�s	����s	������	���g��	������	�͍���
			sys.total	= 1;
			sys.sys[0]	= 6;
			return(&sys);
		case	523307:		//5		���R�s						
		case	523406:		//5		�P�H�s	���Ύs	���Ð�s	�����s	�d����
		case	523407:		//5		���Ύs	���Ð�s	�����	�d����
		case	523415:		//5		�P�H�s	�����s		
		case	523416:		//5		�����s	�P�H�s	���Ð�s	
		case	523417:		//5		���Ð�s	�����		
		case	523425:		//5		�P�H�s			
			sys.total	= 1;
			sys.sys[0]	= 5;
			return(&sys);
		case	523501:		//		�Y���Ȃ�
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
		case	523502:		//5		���s	���{�s	�����s	
		case	523503:		//5		���s	���{�s	���Ύs	
			sys.total	= 1;
			sys.sys[0]	= 5;
			return(&sys);
		case	523504:		//		�Y���Ȃ�
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
		case	523505:		//5		�P�H�s			
		case	523506:		//5		�P�H�s			
		case	523512:		//5		���{�s	�����s	�ɒO�s	��ˎs
		case	523513:		//5		���s	���Ύs	���{�s	�ɒO�s	��ˎs	�쐼�s
			sys.total	= 1;
			sys.sys[0]	= 5;
			return(&sys);
		case	523514:		//6		�L���s	���c�s	���Ύs	����s	�����s	��؎s	�Q����s	���ʎs	�ےÎs
		case	523515:		//6		���Ύs	�����s	�Q����s	���s	����s	���Ŏs
		case	523523:		//6		�r�c�s	��؎s	���ʎs	��ˎs	�쐼�s
		case	523524:		//6		���Ύs	���ʎs	���Ύs
		case	523525:		//6		���Ύs	�����s	���{��	�������s	�����s	��R�蒬	�v��R��
		case	523526:		//6		�F���s	��z�s	�����s	�v��R��	��Îs
		case	523535:		//6		�����s	�������s	���Ύs	���{��
		case	523536:		//6		�F���s	��Îs
		case	523537:		//6		�F���s	��Îs	�I����
			sys.total	= 1;
			sys.sys[0]	= 6;
			return(&sys);
		case	523545:		//		�Y���Ȃ�
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
		case	523546:		//6		��Îs	
		case	523547:		//6		��Îs	��R�s	�I����	��F��
		case	523603:		//6		�Îs	
		case	523604:		//6		�Îs	
		case	523634:		//6		�l���s�s	�鎭�s
		case	523635:		//6		�l���s�s	�K���s	������
		case	523640:		//6		��R�s	�I����	��F��
		case	523645:		//6		�l���s�s	������
			sys.total	= 1;
			sys.sys[0]	= 6;
			return(&sys);
		case	523656:		//7		����	�厡��					
			sys.total	= 1;
			sys.sys[0]	= 7;
			return(&sys);
		case	523657:		//		�Y���Ȃ�
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
		case	523666:		//7		��{�s ���s �����f���� �t���� ���t�� �t���� ���F�� �V�쒬 ���� ���a�� �r�ڎ��� �厡��
		case	523667:		//7		�t����s �����f���� �L�R�� �t���� ���t��		
			sys.total	= 1;
			sys.sys[0]	= 7;
			return(&sys);
		case	523671:		//6		�F���s						
		case	523672:		//6		�F���s						
			sys.total	= 1;
			sys.sys[0]	= 6;
			return(&sys);
		case	523676:		//7		����s	�����s	���s	��q�s	�t����	���t��	�ؑ]�쒬	�H���s
		case	523677:		//7		��{�s	�t����s	���q�s	��q�s	�L�R��	�t����	���t��
		case	523702:		//7		�L���s						
		case	523703:		//7		�L���s						
			sys.total	= 1;
			sys.sys[0]	= 7;
			return(&sys);
		case	523705:		//8		�l���s						
		case	523706:		//8		�l���s						
			sys.total	= 1;
			sys.sys[0]	= 8;
			return(&sys);
		case	523712:		//7		�L���s						
		case	523713:		//7		�L���s						
		case	523731:		//7		����s	�L�c�s	����s				
		case	523741:		//7		����s	�L�c�s					
		case	523750:		//7		�L�c�s	�L���s	�O�D��
		case	523751:		//7		�L�c�s	�O�D��
		case	523760:		//7		���ˎs	�t����s	�L�c�s	�������s
			sys.total	= 1;
			sys.sys[0]	= 7;
			return(&sys);
		case	523832:		//8		�É��s
		case	523833:		//8		�É��s	�����s
		case	523837:		//8		���Îs	�C�P����
		case	523843:		//8		�É��s	�����s
		case	523846:		//8		���Îs
		case	523847:		//8		���Îs	�O���s	�ɓ�������	�C�P����	������
		case	523855:		//8		���Îs	�x�m�s
		case	523856:		//8		���Îs	�x�m�s
		case	523857:		//8		���Îs	�O���s	������
		case	523930:		//8		�ɓ��s	�C�P����
		case	523940:		//8		�M�C�s	�ɓ��s	�C�P����
		case	523950:		//8		�M�C�s	�O���s
			sys.total	= 1;
			sys.sys[0]	= 8;
			return(&sys);
		case	523954:		//9		�O�Y�s
		case	523955:		//9		���{��s	�O�Y�s	�x�Îs
			sys.total	= 1;
			sys.sys[0]	= 9;
			return(&sys);
		case	523960:		//9	8	���c���s(9)	������(9)	���͌���(9)	�O���s(8)
			sys.total	= 2;
			sys.sys[0]	= 8;
			sys.sys[0]	= 9;
			return(&sys);
		case	523961:		//9		���c���s	��{��	���͌���
		case	523964:		//9		���{��s	�O�Y�s	�t�R��
		case	523965:		//9		���{��s	�O�Y�s	�x�Îs
		case	523966:		//9		�x�Îs	���{��s
		case	523971:		//9		���ˎs	���c���s	�쑫���s	��{��	���䒬	�J����
		case	523972:		//9		���ˎs	������s	��钬	��{��	���䒬
		case	523973:		//9		���ˎs	���q�s	����s	������s	���q�s	�t�R��
		case	523974:		//9		���{��s	���q�s	����s	���q�s	�t�R��
		case	523975:		//9		���{��s	���q�s	�t�R��	�x�Îs
		case	523976:		//9		�N�Îs	�x�Îs	���{��s
			sys.total	= 1;
			sys.sys[0]	= 9;
			return(&sys);
		case	533310:		//3		���]�s
			sys.total	= 1;
			sys.sys[0]	= 3;
			return(&sys);
		case	533411:		//5		����s
		case	533421:		//5		����s
			sys.total	= 1;
			sys.sys[0]	= 5;
			return(&sys);
		case	533605:		//7		�򕌎s	�H���s	�}����	���Ò�	���쒬
		case	533606:		//7		�򕌎s	�H���s	��쒬	�}����	���Ò�	��{�s	�ؑ]�쒬
		case	533616:		//7		�򕌎s		
			sys.total	= 1;
			sys.sys[0]	= 7;
			return(&sys);
		case	533834:		//8		�b�{�s		
		case	533844:		//8		�b�{�s		
			sys.total	= 1;
			sys.sys[0]	= 8;
			return(&sys);
		case	533902:		//9		���ˎs	������s	�`��s	���؎s	�ɐ����s	�C�V���s	���쒬	���䒬
		case	533903:		//9		���ˎs	���q�s	����s	������s	���؎s	�C�V���s	�����s	���쒬
		case	533904:		//9		���q�s	����s	
			sys.total	= 1;
			sys.sys[0]	= 9;
			return(&sys);
		case	533905:		//		�Y���Ȃ�
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
		case	533912:		//9		���؎s	�ɐ����s	�C�V���s	���쒬
		case	533913:		//9		����s	���͌��s	���؎s	��a�s	�C�V���s	���Ԏs	�����s
			sys.total	= 1;
			sys.sys[0]	= 9;
			return(&sys);
		case	533914:		//		�Y���Ȃ�
		case	533915:		//		�Y���Ȃ�
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
		case	533916:		//9		�؍X�Îs	�N�Îs	
		case	533922:		//9		���͌��s	���؎s	���쒬
		case	533923:		//9		���͌��s	���؎s	��a�s	���Ԏs
			sys.total	= 1;
			sys.sys[0]	= 9;
			return(&sys);
		case	533924:		//		�Y���Ȃ�
		case	533925:		//		�Y���Ȃ�
		case	533926:		//		�Y���Ȃ�
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
		case	533932:		//9		���͌��s
		case	533933:		//9		���͌��s
			sys.total	= 1;
			sys.sys[0]	= 9;
			return(&sys);
		case	533934:		//		�Y���Ȃ�
		case	533935:		//		�Y���Ȃ�
		case	533936:		//		�Y���Ȃ�
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
		case	533937:		//9		�s��s	�D���s	�K�u��s	�Y���s
			sys.total	= 1;
			sys.sys[0]	= 9;
			return(&sys);
		case	533942:		//		�Y���Ȃ�
		case	533943:		//		�Y���Ȃ�
		case	533944:		//		�Y���Ȃ�
		case	533945:		//		�Y���Ȃ�
		case	533946:		//		�Y���Ȃ�
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
		case	533947:		//9		�s��s	�D���s	���ˎs	�����J�s	�Y���s
		case	533952:		//9		�є\�s	���Ԏs
		case	533953:		//9		����s	���R�s	���Ԏs	�O�F��
		case	533954:		//9		�Y�a�s	����s	�˓c�s	�����s	�u�؎s	�a���s	�V���s	�x�m���s	�O�F��
		case	533955:		//9		����s	�Y�a�s	�n�s	�˓c�s	�����J�s	�����s	�a���s
		case	533956:		//9		����s	�����s	�����J�s	�����s	�O���s
		case	533957:		//9		�O���s	�s��s	�D���s	���ˎs	���s	���R�s	�����J�s	���쒬
		case	533962:		//9		�є\�s	���R�s	���Ԏs	��ˎs	�����s
		case	533963:		//9		��z�s	����s	�є\�s	���R�s	���Ԏs	�߃P���s	�����s	��䒬	�O�F��
		case	533964:		//9		��z�s	�Y�a�s	��{�s	�^��s	�u�؎s	�x�m���s	�㕟���s	��䒬	�O�F��
		case	533965:		//9		����s	�Y�a�s	��{�s	��Ύs	�^��s	�z�J�s	�����J�s
		case	533966:		//9		����s	��Ύs	�����s	�z�J�s	�����s	�O���s	�g��s	������	��c�s
		case	533967:		//9		�O���s	�g��s	���ˎs	��c�s	���s	���R�s	�䑷�q�s	���쒬	���s	��J��
		case	533973:		//9		��z�s	�����R�s	��ˎs	�߃P���s	�����s
		case	533974:		//9		��z�s	��{�s	����s	����s	�k�{�s	�ɓޒ�	�쓇��
		case	533975:		//9		��{�s	��Ύs	�t�����s	����s	�z�J�s	�@�c�s	�ɓޒ�	�{�㒬	������	���˒�
		case	533976:		//9		��Ύs	�t�����s	�z�J�s	�g��s	���˒�	������	���a��	��c�s
		case	533977:		//9		��c�s	���s	���C���s	���s	�J�a����	��J��
		case	534010:		//9		�s���s	�����Y�s			
		case	534020:		//9		�s���s	�����Y�s	
		case	534021:		//9		�s���s		
		case	534022:		//9		�����s	�s���s	���X�s	��Ԕ�����
		case	534030:		//9		�D���s	�K�u��s	
		case	534031:		//9		���q�s	�l�X���s	
		case	534040:		//9		�D���s	�K�u��s	�����s	�����J�s
		case	534041:		//9		���q�s	�����s	�l�X���s	���X�䒬	��ב�
		case	534050:		//9		�D���s	���ˎs	���s	�����s	�����J�s	�󐼎s	���쒬	���䒬
		case	534060:		//9		���s	�䑷�q�s	�󐼎s	���쒬	���s	��J��	���㒬
			sys.total	= 1;
			sys.sys[0]	= 9;
			return(&sys);
		case	543601:		//6		����s		
			sys.total	= 1;
			sys.sys[0]	= 6;
			return(&sys);
		case	543665:		//7		����s		
			sys.total	= 1;
			sys.sys[0]	= 7;
			return(&sys);
		case	543727:		//8		���{�s		
		case	543737:		//8		���{�s		
		case	543800:		//8		���J�s	�z�K�s	���z�K��
		case	543801:		//8		�z�K�s	���z�K��	
			sys.total	= 1;
			sys.sys[0]	= 8;
			return(&sys);
		case	543837:		//9		����s		
			sys.total	= 1;
			sys.sys[0]	= 9;
			return(&sys);
		case	543845:		//8		�y���		
		case	543871:		//8		����s		
			sys.total	= 1;
			sys.sys[0]	= 8;
			return(&sys);
		case	543924:		//9		�ٗюs	�q��	���a��	�W�y��	�s�c�s	���{�s	�H���s
		case	543930:		//9		�O���s	����s	�����s	�V��	�ʑ���	�㗢��
		case	543931:		//9		�O���s	�ɐ���s	�V��	����	�ʑ���	�{���s	�㗢��
		case	543932:		//9		�ȏ���	���c�s	����	������	�V�c��	���˖{��
		case	543933:		//9		�����s	���c�s	�ٗюs	���	�W�y��
		case	543934:		//9		�ٗюs	�q��	�W�y��	�����s	����s	������	��M��	�c����
		case	543936:		//9		����s	���R�s	��ؒ�
		case	543940:		//9		�O���s	����s	
		case	543942:		//9		�ː��s	���c�s	�Ԗx��	�V�c��	���˖{��	�}����
		case	543943:		//9		�ː��s	���c�s	�����s	�c����
			sys.total	= 1;
			sys.sys[0]	= 9;
			return(&sys);
		case	543966:		//8	9	�F�s�{�s(8)	�����s(9)	�p����(9)
		case	543967:		//8	9	�F�s�{�s(8)	�^���s(9)	
			sys.total	= 2;
			sys.sys[0]	= 8;
			sys.sys[1]	= 9;
			return(&sys);
		case	544001:		//9		�y�Y�s	���v�s	���Ύs	������	�s�蒬
		case	544011:		//9		�y�Y�s	�Ή��s	���Ύs	���c��	�V����
		case	544043:		//9		���ˎs	�Ђ����Ȃ��s	��钬	������
		case	544044:		//9		���ˎs	�Ђ����Ȃ��s
		case	544065:		//9		�����s
		case	544075:		//9		�����s
			sys.total	= 1;
			sys.sys[0]	= 9;
			return(&sys);
		case	553701:		//7		�x�R�s
			sys.total	= 1;
			sys.sys[0]	= 7;
			return(&sys);
		case	554046:		//9		���킫�s
		case	554047:		//9		���킫�s
			sys.total	= 1;
			sys.sys[0]	= 9;
			return(&sys);
		case	563816:		//8		�����s
		case	563817:		//8		�����s
			sys.total	= 1;
			sys.sys[0]	= 8;
			return(&sys);
		case	563917:		//9		��Îᏼ�s
		case	563927:		//9		��Îᏼ�s
			sys.total	= 1;
			sys.sys[0]	= 9;
			return(&sys);
		case	563960:		//8		�V���s	�T�c��	���钬
		case	563970:		//8		�V���s
			sys.total	= 1;
			sys.sys[0]	= 8;
			return(&sys);
		case	564002:		//9		�S�R�s
		case	564003:		//9		�S�R�s
		case	564043:		//9		�����s
		case	564053:		//9		�����s
			sys.total	= 1;
			sys.sys[0]	= 9;
			return(&sys);
		case	574022:		//10	�R�`�s	��R�s
			sys.total	= 1;
			sys.sys[0]	= 10;
			return(&sys);
		case	574026:		//		�Y���Ȃ�
		case	574027:		//		�Y���Ȃ�
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
		case	574032:		//10	�R�`�s	�V���s
			sys.total	= 1;
			sys.sys[0]	= 10;
			return(&sys);
		case	574036:		//		�Y���Ȃ�
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
		case	574037:		//10	�����s	�����s	���{��
		case	574042:		//10	�R�`�s	�V���s
		case	574043:		//10	�R�`�s	�V���s
		case	594040:		//10	�H�c�s
		case	594041:		//10	�H�c�s
		case	594140:		//10	�����s
		case	594141:		//10	�����s
		case	604073:		//10	�O�O�s
		case	604163:		//10	���ˎs
		case	604164:		//10	���ˎs
		case	614015:		//10	�X�s
		case	614016:		//10	�X�s
			sys.total	= 1;
			sys.sys[0]	= 10;
			return(&sys);
		case	624045:		//11	���َs	��钬
		case	624055:		//11	6	���َs	��钬	��쒬(6)
		case	624056:		//11	���َs
		case	644067:		//11	���M�s
			sys.total	= 1;
			sys.sys[0]	= 11;
			return(&sys);
		case	644132:		//		�Y���Ȃ�
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
		case	644133:		//12	�k�L���s
			sys.total	= 1;
			sys.sys[0]	= 12;
			return(&sys);
		case	644142:		//		�Y���Ȃ�
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
		case	644143:		//12	�]�ʎs	�k�L���s
		case	644144:		//12	�]�ʎs	�k�L���s
			sys.total	= 1;
			sys.sys[0]	= 12;
			return(&sys);
		case	644151:		//11	���M�s
		case	644152:		//11	12	���M�s(11)	�Ύ�s(12)
			sys.total	= 1;
			sys.sys[0]	= 11;
			sys.sys[1]	= 12;
			return(&sys);
		case	644153:		//12	�]�ʎs
		case	644154:		//12	�]�ʎs
			sys.total	= 1;
			sys.sys[0]	= 12;
			return(&sys);
		case	644160:		//11	���M�s
			sys.total	= 1;
			sys.sys[0]	= 11;
			return(&sys);
		case	654252:		//12	����s
		case	654253:		//12	����s
			sys.total	= 1;
			sys.sys[0]	= 12;
			return(&sys);
		default:
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
	}
}

// �������� z19_add.cpp
//1999.12.27 �����܂�
//------------------------------------------------------------
/*
	���b�V���R�[�h����Y������P�X���W�n�̐��Ɣԍ��𓾂� \n
	�Y������n���Ȃ��ꍇ�A���A�ԍ��Ƃ���0��Ԃ�
*/
//------------------------------------------------------------
void	crd_cnv::
Sys_Number	(	int		meshcode,		// ���b�V���R�[�h
				sys_num	*sys)			// �n�ԍ�
{
	*sys	= *Sys_Number(meshcode);	// 2009.05.25 �C��
}

//------------------------------------------------------------
/*
	�o�ܓx����P�X���W�n�̂w�x�֕ϊ�(�ȉ~�̏����w��)
*/
//------------------------------------------------------------
void	crd_cnv::
LLtoXY19_Core	(	double		Ra,			// �n���ȉ~�̂̒����a�i���[�g���j
					double		ea2,		// ��ꗣ�S����2��
					double		eb2,		// ��񗣐S����2��
					int			axis,		// �Y��������W�n�̔ԍ�
					double		lon,		// �o�x
					double		lat,		// �ܓx
					int			mode,		// �ϊ����[�h(0=���{���n�n,1=���E���n�n)
					double		*x,			// �P�X���W�n�̂w
					double		*y)			// �P�X���W�n�̂x
{

	double			d_lon,	Tau;
	double			Eta2,	Eta4;
	double			N,		Phi;
	double			DPhi,	t;
	double			tx,		ty;

//	Make19tbl();			// 19���W�n�̃e�[�u�����쐬

	d_lon	= lon * M_PI / 180.0 - Rad_Origin[axis - 1].lon;
	lat		= lat * M_PI / 180.0;

	Tau		= tan(lat);
	Eta2	= eb2 * pow(cos(lat), 2.0);
	Eta4	= pow(Eta2, 2.0);
	N		= Ra / sqrt(1.0 - ea2 * pow(sin(lat), 2.0));
	Phi		= Rad_Origin[axis - 1].lat;
	DPhi	= lat - Rad_Origin[axis - 1].lat;
	t		= tan(lat);

	double	arclen	= 0.0, arclen_org = 0.0;

	if( mode == 0 ) {
		arclen		= M_Arc_Len(Ra, ea2, lat);
		arclen_org	= M_Arc_Len(Ra, ea2, Rad_Origin[axis - 1].lat);
	}else {
		arclen		= M_Arc_Len_Core(Ra, ea2, lat);
		arclen_org	= M_Arc_Len_Core(Ra, ea2, Rad_Origin[axis - 1].lat);
	}

	tx		= arclen - arclen_org
  			+ N * pow(d_lon, 2.0) * sin(lat) * cos(lat) / 2.0
			* (1.0 + pow(d_lon, 2.0) * pow(cos(lat), 2.0)
			* ((5.0 - pow(t, 2.0) + 9.0 * Eta2 + 4.0 * Eta4) / 12.0
			+ pow(d_lon, 2.0) * pow(cos(lat), 2.0)
			* (61.0 - 58.0 * pow(t, 2.0) + pow(t, 4.0) + 270.0 * Eta2
			- 330.0 * pow(t, 2.0) * Eta2) / 30.0));

	ty		= N * d_lon * cos(lat)
			* (1.0 + pow(d_lon, 2.0) * pow(cos(lat), 2.0)
			* ((1.0 - pow(t, 2.0) + Eta2)
			+ pow(d_lon, 2.0) * pow(cos(lat), 2.0)
			* (5.0 - 18.0 * pow(t, 2.0) + pow(t, 4.0)
			+ 14.0 * Eta2 -58.0 * pow(t, 2.0) * Eta2) / 20.0) / 6.0);

	*x	= tx * M0;
	*y	= ty * M0;
}

//------------------------------------------------------------
/*
	�o�ܓx����P�X���W�n�̂w�x�֕ϊ�
	@return	�\���� xy_coord �ւ̃|�C���^
*/
//------------------------------------------------------------
xy_coord*	crd_cnv::
LLtoXY19	(	int		axis,		// �Y��������W�n�̔ԍ�
				double	lon,		// �o�x
				double	lat)		// �ܓx
{
	static xy_coord	coord;

	LLtoXY19 ( axis, lon, lat, &coord.x, &coord.y );

	return(&coord);
}

//------------------------------------------------------------
/*
	�o�ܓx����P�X���W�n�̂w�x�֕ϊ�
	@return	�\���� coord19 �ւ̃|�C���^
*/
//------------------------------------------------------------
coord19*	crd_cnv::
LLtoXY19	(	double	lon,		// �o�x
				double	lat)		// �ܓx
{
	static	coord19		sys_xy;	
	int					sys		= 0;

	sys			= Sys_Number(lon, lat);		
	sys_xy.sys	= sys;

	LLtoXY19 ( sys, lon, lat, &sys_xy.x, &sys_xy.y );

	return(&sys_xy);
}

//------------------------------------------------------------
/*
	�o�ܓx����P�X���W�n�̂w�x�֕ϊ�
*/
//------------------------------------------------------------
void	crd_cnv::
LLtoXY19	(	int			axis,		// �Y��������W�n�̔ԍ�
				double		lon,		// �o�x
				double		lat,		// �ܓx
				xy_coord	*coord)		// 19���W�n�̂w�x
{
	LLtoXY19 ( axis, lon, lat, &coord->x, &coord->y );
}

//------------------------------------------------------------
/*
	�o�ܓx����P�X���W�n�̂w�x�֕ϊ�		1998.7.14
*/
//------------------------------------------------------------
void	crd_cnv::
LLtoXY19	(	double		lon,			// �o�x
				double		lat,			// �ܓx
				coord19		*sys_xy)		// 19���W�n�̌n�Ƃw�x��\��
{
	int			sys		= 0;

	sys			= Sys_Number(lon, lat);		
	sys_xy->sys = sys;
	LLtoXY19 ( sys, lon, lat, &sys_xy->x, &sys_xy->y );
}

//------------------------------------------------------------
/*
	�o�ܓx����P�X���W�n�̂w�x�֕ϊ�		2000.1.12
*/
//------------------------------------------------------------
void	crd_cnv::
LLtoXY19	(	int			axis,		// �Y��������W�n�̔ԍ�
				double		lon,		// �o�x
				double		lat,		// �ܓx
				double		*x,			// �P�X���W�n�̂w
				double		*y)			// �P�X���W�n�̂x
{
	LLtoXY19_Core ( RA, EA2, EB2, axis, lon, lat, 0, x, y );
}

//------------------------------------------------------------
/*
	�o�ܓx����P�X���W�n�̂w�x�֕ϊ��i���E���n�n�Łj
	@return	�\���� xy_coord �ւ̃|�C���^
*/
//------------------------------------------------------------
void	crd_cnv::
LLtoXY19_JGD2000	(	int		axis,		// �Y��������W�n�̔ԍ�
						double	lon,		// �o�x
						double	lat,		// �ܓx
						double	*x,			// �P�X���W�n�̂w
						double	*y)			// �P�X���W�n�̂x
{
	LLtoXY19_Core ( GRS80_RA, GRS80_EA2, GRS80_EB2, axis, lon, lat, 1, x, y );
}

//------------------------------------------------------------
/*
	�P�X���W�n�̂w�x����o�ܓx�֕ϊ�(�ȉ~�̏����w��)
*/
//------------------------------------------------------------
void 	crd_cnv::
XY19toLL_Core	(	double		Ra,		// �n���ȉ~�̂̒����a�i���[�g���j
					double		ea2,	// ��ꗣ�S����2��
					double		eb2,	// ��񗣐S����2��
					double		f,		// �ȉ~�̝̂G����
					int			axis,	// �Y��������W�n�̔ԍ�
					double		x,		// �k�����̍��W�l
					double		y,		// �������̍��W�l
					int			mode,	// �ϊ����[�h(0=���{���n�n,1=���E���n�n)
					double		*lon,	// �o�x
					double		*lat)	// �ܓx
{
	double	Rad_Phi,	Rad_Ramda0,	Rad_Ramda;
	double	Rad_Phi1,	Rad_Phi0;
	double	Rad_Phi_n,	Rad_Phi_c,	Gunma2;
	double	t1,	a1,	p;

//	Make19tbl();	// 19���W�n�̃e�[�u�����쐬

	// ���_�o�ܓx�̎��o��
	Rad_Phi0		= Rad_Origin[axis - 1].lat;

	Rad_Ramda0		= Rad_Origin[axis - 1].lon;

	Rad_Phi_c		= Rad_Phi0;

	double	arc_len1 = 0.0, arc_len2 = 0.0;

	// �����ܓx�����߂�
	while(1)
	{
		if( mode == 0 ) {
			arc_len1	= M_Arc_Len(Ra, ea2, Rad_Phi_c);
			arc_len2	= M_Arc_Len(Ra, ea2, Rad_Phi0);
		}else {
			arc_len1	= M_Arc_Len_Core(Ra, ea2, Rad_Phi_c);
			arc_len2	= M_Arc_Len_Core(Ra, ea2, Rad_Phi0);
		}
		Rad_Phi_n	= Rad_Phi_c
					- ((arc_len1 - arc_len2 - (x / M0)) / Ra * (1.0 - ea2))
					* pow((1.0 - ea2 * pow(sin(Rad_Phi_c), 2.0)), 1.5);

		if(fabs(Rad_Phi_n - Rad_Phi_c) < 1.0e-12)
		{
			break;
		}
		Rad_Phi_c	= Rad_Phi_n;
	}
	Rad_Phi1	= Rad_Phi_n;

	t1		= tan(Rad_Phi1);
	Gunma2	= eb2 * pow(cos(Rad_Phi1), 2.0);
	a1		= Ra / sqrt(1.0 - ea2 * pow(sin(Rad_Phi1), 2.0));
	p		= y / M0;

	double p2, p4, t2, t4, a2, a4;
	p2		= pow(p,	2.0);
	p4		= pow(p,	4.0);
	t2		= pow(t1,	2.0);
	t4		= pow(t1,	4.0);
	a2		= pow(a1,	2.0);
	a4		= pow(a1,	4.0);

	Rad_Phi		= Rad_Phi1
				- (p2 * (1.0 + Gunma2) * t1) / (2.0 * a2)
				* (1.0 - p2 / (12.0 * a2)
				* (5.0 + 3.0 * t2 + Gunma2 - 9.0 * t2 * Gunma2)
				+ p4 / (360.0 * a4) 
				* (61.0 + 90.0 * t2 + 45.0 * t4));

	Rad_Ramda	= Rad_Ramda0
				+ p / (a1 * cos(Rad_Phi1))
				* (1.0 - p2 / (6.0 * a2)
				* (1.0 + 2.0 * t2 + Gunma2)
				+ p4 / (120.0 * a4)
				* (5.0 + 28.0 * t2 + 24.0 * t4));

	*lon	= Rad_Ramda	/ (M_PI/180.0);
	*lat	= Rad_Phi	/ (M_PI/180.0);
}

//------------------------------------------------------------
/*
	�P�X���W�n�̂w�x����o�ܓx�֕ϊ�
	@return	�\���� d_lonlat �ւ̃|�C���^
*/
//------------------------------------------------------------
d_lonlat*	crd_cnv::
XY19toLL	(	int		axis,		// �Y��������W�n�̔ԍ�
				double	x,			// �k�����̍��W�l
				double	y)			// �������̍��W�l
{
	static	d_lonlat	lonlat;

	XY19toLL ( axis, x, y, &lonlat.lon, &lonlat.lat );

	return(&lonlat);
}

//------------------------------------------------------------
/*
	�P�X���W�n�̂w�x����o�ܓx�֕ϊ�
*/
//------------------------------------------------------------
void 	crd_cnv::
XY19toLL	(	int			axis,		// �Y��������W�n�̔ԍ�
				double		x,			// �k�����̍��W�l
				double		y,			// �������̍��W�l
				d_lonlat	*lonlat)	// 10�i�ŕ\�����o�ܓx(ex:130.23432342....)
{
	XY19toLL ( axis, x, y, &lonlat->lon, &lonlat->lat );
}

//------------------------------------------------------------
/*
	�P�X���W�n�̂w�x����o�ܓx�֕ϊ�
*/
//------------------------------------------------------------
void 	crd_cnv::
XY19toLL	(	int			axis,	// �Y��������W�n�̔ԍ�
				double		x,		// �k�����̍��W�l
				double		y,		// �������̍��W�l
				double		*lon,	// �o�x
				double		*lat)	// �ܓx
{
	double	f = 1.0 / BESSEL_INV_F;	// �ȉ~�̝̂G����

	XY19toLL_Core (	RA, EA2, EB2, f, axis, x, y, 0, lon, lat );
}

//------------------------------------------------------------
/*
	�P�X���W�n�̂w�x����o�ܓx�֕ϊ��i���E���n�n�Łj
	@return	�\���� d_lonlat �ւ̃|�C���^
*/
//------------------------------------------------------------
void	crd_cnv::
XY19toLL_JGD2000	(	int		axis,		// �Y��������W�n�̔ԍ�
						double	x,			// �k�����̍��W�l
						double	y,			// �������̍��W�l
						double	*lon,		// �o�x
						double	*lat	)	// �ܓx
{
	double	f = 1.0 / GRS80_INV_F;	// �ȉ~�̝̂G����

	XY19toLL_Core (	GRS80_RA, GRS80_EA2, GRS80_EB2, f, axis, x, y, 1, lon, lat );
}

//------------------------------------------------------------
/*
	�P�X���W�n��(X,Y)����A���b�V���R�[�h + (x,y) �����߂�
	@retval		0	����
	@retval		-1	���s
*/
//------------------------------------------------------------
int		crd_cnv::
XY19toMesh	(	int		sys,		// �P�X���W�n�̌n�ԍ�
				double	x_19,		// �P�X���W�n�̂w�i�ܓx�����j
				double	y_19,		// �P�X���W�n�̂x�i�o�x�����j
				int		level,		// ���b�V���̃��x�� (1,2,64)
				int		*meshcode,	// ���b�V���R�[�h
				int		*x,			// ���K���w
				int		*y)			// ���K���x
{
	double	lat	= 0.0,	lon	= 0.0;

	XY19toLL(sys, x_19, y_19, &lon, &lat);
	if(LLtoMesh(lon, lat, level, meshcode, x, y) == 0)
	{
		return(0);
	}
	else
	{
		return(-1);
	}
}

//------------------------------------------------------------
/*
	�P�X���W�n��(X,Y)����A���b�V���R�[�h + (x,y) �����߂�
*/
//------------------------------------------------------------
void	crd_cnv::
MeshtoXY19	(	int		sys,		// �P�X���W�n�̌n�ԍ�
				int		meshcode,	// ���b�V���R�[�h
				int		x,			// ���K���w
				int		y,			// ���K���x
				double	*x_19,		// �P�X���W�n�̂w�i�ܓx�����j
				double	*y_19)		// �P�X���W�n�̂x�i�o�x�����j
{
	double	lon	= 0.0,	lat	= 0.0;

	MeshtoLL(meshcode, x, y, &lon, &lat);
	LLtoXY19(sys, lon, lat, x_19, y_19); 
}
