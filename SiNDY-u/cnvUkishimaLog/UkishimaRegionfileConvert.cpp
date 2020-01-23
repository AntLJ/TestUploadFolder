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

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <direct.h>

#pragma warning (disable : 4172)

using namespace std;

/////////////////////////////////////////////
// @�t�@�C���̑��s���𐔂���
//
// @param fp[in] ���̓t�@�C���|�C���^
// @param pszPath ���̓t�@�C���f�B���N�g���|�C���^
// @return ���s��
/////////////////////////////////////////////
int CountLines(char *pszPath)
{
	//���O�t�@�C���̓���
	FILE *fp = fopen(pszPath, "r");

	char aLineData[200];	//��s�f�[�^
	int iNum = 0;			//���s��

	while(fp){
		//�z��̏�����
		for(int i = 0; i < 200; i++)
			aLineData[i] = NULL;

		//���O�t�@�C�������s�f�[�^�ǂݍ���
		char* apLineData = fgets(aLineData, 200,fp);

		//�f�[�^�������Ă��Ȃ��ꍇ�A�G���[
		if(apLineData == NULL)
		{
			break;
		}

		//�s�����₷
		iNum++;

	}
	
	fclose(fp);

	//���s����Ԃ�
	return iNum;
}

/////////////////////////////////////////////////////////////
//@brief �o�̓t�@�C���̃f�B���N�g��(���̓t�@�C���Ɠ����f�B���N�g���j
//�@     �p�X���쐬����֐�
//
//@param pszPath [in] ���̓t�@�C���|�C���^
//@param apDirectory [in, out] �o�̓t�@�C���|�C���^
/////////////////////////////////////////////////////////////

void MakeDirectory(const char* pszPath, char *apDirectory)
{
	int iDirectoryPoint = 0;	//�Ō��'\\'�̈ʒu
	char chPath[100];			//�o�̓f�B���N�g��

	//�o�̓t�@�C����̃R�s�[
	strcpy(chPath, pszPath);	

	//�z�񏉊���
	for(int i = 0; i<100; i++)
		chPath[i] = NULL;

	//�ŏI'\\'�̈ʒu�̋L�^
	for(int i = 0;i<100;i++)
	{
		if(chPath[i] == '\\')
			iDirectoryPoint = i - 1;
	}

	//�p�X����t�@�C�������폜
	for(int i = iDirectoryPoint ; i<100; i++)
		chPath[i] = NULL;

	//�o�͐�f�B���N�g����Ԃ�
	strcpy(apDirectory, chPath);
}
/////////////////////////////////////////////////////////
//@breif �^�ϊ��֐�
//
//@param iStart_pos[in]			�����ǂݍ��݊J�n�ʒu
//@param iUnderbar_pos[in]	'_'�̏ꏊ
//@param apLineData[in]		��s�f�[�^
//@param oft			�o�̓t�@�C���|�C���^
//
/////////////////////////////////////////////////////////
void ConvertFormat(int iStart_pos, int iUnderbar_pos,const char *apLineData, FILE *ofp)
{
	//��{�f�[�^�ǂݍ���
	for(int i = iStart_pos; i<100; i++)
	{
	//���b�V���R�[�h�̌���'-'��t����
		//SCALE���x����'M'�̏ꍇ
		if(apLineData[iStart_pos + 4] == 'M')
		{
			if(i == iStart_pos + 4)
				fprintf(ofp,"-");
		}
		
		//SCALE B1�̏ꍇ
		if(apLineData[iStart_pos + 6] == 'B')
		{
			if(i == iStart_pos + 6)
				fprintf(ofp,"-");
		}

		//���O�f�[�^��'_'�ʒu�̋L�^�A������'_'����':'��
		if(apLineData[i] == '_')
		{
			iUnderbar_pos = i;
			fprintf(ofp,";");
			continue;
		}

		//X,Y�����̃��j�b�g�\���ɕϊ�
		if(i == iUnderbar_pos + 2)
		{
			fprintf(ofp,",");
			fputc(apLineData[i],ofp);
			fprintf(ofp,"]");
			continue;
		}

		//�󔒂̌�A���������Ă�����󔒂�����
		if(apLineData[i] == ' ' && isdigit(apLineData[i + 1]) == 0)
			continue;

		//NULL������������1�s�f�[�^�ǂݍ��ݏI��
		if(apLineData[i] == NULL)
			break;

		//��L�ȊO�̏ꍇ�A���̕������o��
		fputc(apLineData[i],ofp);
	}
}

////////////////////////////////////////////////////////
//@breif R�t�@�C���쐬�֐�
//
//@param apDirectory [in] �o�̓t�@�C���f�B���N�g���|�C���^
//@param apLineData	[in]	 ��s�f�[�^�|�C���^
//@param oft		 �o�̓t�@�C���|�C���^
//@param apNewDirectory [in,out] �ϊ���̏o�̓t�@�C���f�B���N�g��
///////////////////////////////////////////////////////
void ChangeFiles(char* apDirectory, char* apLineData, FILE *ofp, char* apNewDirectory)
{
	char cPath[100];	//�V�����o�̓t�@�C���p�X

	//�z�񏉊���
	for(int i = 0;i < 100;i++)
	{
		cPath[i] = NULL;
	}

	//SCALE M3��������SCALE B1�̏ꍇ
	if((apLineData[6] == 'M' && apLineData[7] == '3')||apLineData[6] == 'B')
	{
		//����܂ł̏o�̓t�@�C�������
		fclose(ofp);

		//�o�͐�f�B���N�g���̃R�s�[
		strcpy(cPath, apDirectory);

		//SCALE M3�̏ꍇ�AR2�t�@�C���̍쐬
		if(apLineData[6] == 'M' && apLineData[7] == '3')
			strcat(cPath, "\\R2.rgn");

		//SCALE B1�̏ꍇ�AR1�t�@�C���̍쐬
		if(apLineData[6] == 'B')
			strcat(cPath, "\\R1.rgn");
	}
	//�V�����o�̓t�@�C������R�s�[
	strcpy(apNewDirectory, cPath);
}

///////////////////////////////////////////////
//@breif �^�`�F�b�N�֐�
//
//@param iStart_pos	�����ǂݍ��݊J�n�ʒu
//@param apLineData	��s�f�[�^
///////////////////////////////////////////////
void FileCheck(int iStart_pos, char* apLineData)
{
	//SCALE��B�N���X�̏ꍇ�A���b�V���R�[�h��6����荞��
	if(apLineData[iStart_pos + 6] == 'B')
		iStart_pos = iStart_pos + 2;

	//��{���O�f�[�^���`�F�b�N����
	for(int i = iStart_pos; i < iStart_pos + 9; i++)
	{
		//����������ꏊ��������
		if(i != iStart_pos + 4&& i != iStart_pos + 6)
		{
			//�����ȊO�������Ă�����v���O�����G���[
			if(isdigit(apLineData[i]) == 0)
			{
				cerr << "file error!" << endl;
				exit(0);
			}
		}
	}

	//SCALE���x����'M'�ȊO�������Ă������A��������'_'�̏ꏊ�ɂ���ȊO�������Ă�����
	if(apLineData[iStart_pos+4] != 'M'||apLineData[iStart_pos+6] != '_')
	{
		//SCALE���x����'B'�ȊO�������Ă�����G���[
		if(apLineData[iStart_pos+4] != 'B')
		{
			cerr << "file error!" << endl;
			exit(0);
		}
	}
}


///////////////////////////////////////////
//@breif ��Ɗ֐�
//
//@param iNum[in]		���s��
//@param apDirectory[in]	�o�̓t�@�C���f�B���N�g��
//@param fp[in]			���̓t�@�C���|�C���^
//@param ofp[in]		�o�̓t�@�C���|�C���^
//@param apNewDirectory[in,out]		�V�����o�̓t�@�C���f�B���N�g��
//////////////////////////////////////////
void MainWorks(int iNum, char* apDirectory, FILE *fp, FILE *ofp, char* apNewDirectory)
{
	char aUpper[10];					//UPPER������z��
	char aLineData[400];				//���O�t�@�C���̈�s���f�[�^

	//��s���f�[�^���Ƃ肾���A�^�ϊ�
	for(int i = 0; i < iNum-1; i++)
	{
		int iUnderbar_pos = -5;		//��s�f�[�^�ɓ����Ă���'_'�̈ʒu

		//������
		for(int j = 0; j < 200; j++)
			aLineData[j] = 0;

		//��s�f�[�^�����Ȃ��ꍇ�̓G���[��Ԃ�
		if( fgets( aLineData, 400, fp ) == NULL)
		{
			cout << "fgets error" << endl;
			break;
		}

		//1�C2�s�ڂ̓X�L�b�v
		if(i < 2)
		{
			continue;
		}

		else
		{
			int iStart_pos = 0;		//���b�V���R�[�h�ǂݍ��݊J�n�ʒu

			//�ǂݍ��ݍŏI�s�Ȃ�A���̍s�ǂݍ���Ŋ֐��I��
			if(i == iNum - 1)
			{
				fputs(aLineData,ofp);
				break;
			}

		//���b�V���R�[�h�̈ʒu���L�����A�R�[�h�̓���'['��t����
			//�ŏ��������̏ꍇ

			if(isdigit(aLineData[0]) != 0)
			{
				fprintf(ofp,"[");
				iStart_pos = 0;
			}

			//��s�f�[�^�̍ŏ���'S'�̏ꍇ
			if(aLineData[0] == 'S')
			{	
				//�V�������[�W�����t�@�C���̍쐬
				ChangeFiles(apDirectory, aLineData, ofp, apNewDirectory);
				
				//SCALE M1�̏ꍇ�A�������X�L�b�v
				if(aLineData[6] == 'M' && aLineData[7] == '1')
					continue;
				
				//�쐬�����V�������[�W�����t�@�C���ɏo�͐��ύX
				FILE *ofp2 = fopen(apNewDirectory,"w");
				ofp = ofp2;
				continue;
			}
				
			//�^�u�̏ꍇ
			if(aLineData[0] == '\t')
			{
				fputc(aLineData[0],ofp);
				fprintf(ofp, "[");
				iStart_pos = 1;
			}

			//UPPER���������ꍇ

			if(aLineData[0] == 'U' && aLineData[5] == ' ')
			{
				//UPPER�̕������o��
				for(int j = 0;j < 6;j++)
				{
					fputc(aLineData[j],ofp);
					aUpper[j] = aLineData[j];
				}

				//������U��UPPER�ȊO�̏ꍇ
				if(strncmp(aUpper,"UPPER", 5) != 0)
				{
					//�G���[���o�͂��A�v���O�����I��
					cerr << "file miss!" << endl;
					exit(0);
				}

				fprintf(ofp,"[");
				iStart_pos = 6;
			}

			//�^�ϊ�����
			ConvertFormat(iStart_pos, iUnderbar_pos, aLineData, ofp);
			
			//�^�`�F�b�N
			FileCheck(iStart_pos, aLineData);

		}
	}
}


//////////////////////////////////////////
//@breif  ���C���̏���(�R�[�h�ϊ��j
//
//@param argc		����
//@param argv		����
/////////////////////////////////////////
int main(int argc, char* argv[])
{
	int iNum = 0;			//���O�t�@�C���̑��s��
	char aDirectory[100];       //���[�W�����t�@�C���̏o�͐�
	char aNewDirectory[100];    //�V�������[�W�����t�@�C���̏o�͐�
	char aFile[100];			//�o�̓t�@�C���̃p�X

	//�z��̏�����
	for(int i = 0; i < 100; i++)
	{
		aFile[i] = NULL;
		aDirectory[i] = NULL;
		aNewDirectory[i] = NULL;
	}

	//���O�t�@�C���̑��s���̎擾
	iNum = CountLines(argv[1]);

	//���O�t�@�C�����J��
	FILE *fp = fopen(argv[1], "r");

	//���O�t�@�C���̒��g��������Ȃ��ꍇ
	if(fp == NULL)
	{
		return 0;
	}

	//���[�W�����t�@�C���̏o�̓f�B���N�g���쐬
	MakeDirectory(argv[1], aDirectory);

	//�ŏ��̃��[�W�����t�@�C���쐬
	strcpy(aFile, aDirectory);
	strcat(aFile, "\\R3.rgn");

	//�쐬�������[�W�����t�@�C�����o�͐��
	FILE *ofp = fopen(aFile, "w");

	//���O�t�@�C�����烊�[�W�����t�@�C���֕ϊ�
	MainWorks(iNum, aDirectory, fp, ofp, aNewDirectory);

	fclose(fp);
	fclose(ofp);
	return 0;
}
