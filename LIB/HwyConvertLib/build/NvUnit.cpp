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

/*
**	NvUnit
**
**		navi98 unitcode class
**
**		Programmed by H.Miyahara
*/

#include "stdafx.h"
#include "NvUnit.h"
#include "ZDigit.h"
#include "ZFile.h"


/*
**	static variables
*/

int	NvUnit::mOrigin	= 392000;


/*
**	��r
*/

int NvUnit::compare(const NvUnit& cUnitCode) const
{
	if(eLevel < cUnitCode.eLevel) return -1;
	if(eLevel > cUnitCode.eLevel) return  1;

	if(eMesh < cUnitCode.eMesh) return -1;
	if(eMesh > cUnitCode.eMesh) return  1;

	if(eX < cUnitCode.eX) return -1;
	if(eX > cUnitCode.eX) return  1;

	if(eY < cUnitCode.eY) return -1;
	if(eY > cUnitCode.eY) return  1;

	return 0;
}


/*
**	���b�V���̈ړ�
*/

NvUnit NvUnit::getMoved(
	int		cDeltaX,		// �w�����̈ړ���
	int		cDeltaY)		// �x�����̈ړ���
	const
{
	if((0==cDeltaX) && (0==cDeltaY)) return *this;

	NvUnit	aMovedUnit;

	switch(eLevel)
	{
		case 0:{
			assert((eX>=0) && (eX<16));
			assert((eY>=0) && (eY<16));
			int	aX = eX + 16 * (8*((eMesh/100)%100) + (eMesh%10)) + cDeltaX;
			int	aY = eY + 16 * (8*(eMesh/10000) + ((eMesh/10)%10)) + cDeltaY;
			aMovedUnit.eX = aX % 16;
			aMovedUnit.eY = aY % 16;
			aX /= 16;
			aY /= 16;
			aMovedUnit.eMesh = 10*(10*(100*(aY/8) + (aX/8)) + (aY%8)) + (aX%8);
			break;}

		case 1:{
			assert((eX>=0) && (eX<4));
			assert((eY>=0) && (eY<4));
			int	aX = eX + 4 * (8*((eMesh/100)%100) + (eMesh%10)) + cDeltaX;
			int	aY = eY + 4 * (8*(eMesh/10000) + ((eMesh/10)%10)) + cDeltaY;
			aMovedUnit.eX = aX % 4;
			aMovedUnit.eY = aY % 4;
			aX /= 4;
			aY /= 4;
			aMovedUnit.eMesh = 10*(10*(100*(aY/8) + (aX/8)) + (aY%8)) + (aX%8);
			break;}

		case 2:{
			assert((eX==0) && (eY==0));
			int	aX = 8*((eMesh/100)%100) + (eMesh%10) + cDeltaX;
			int	aY = 8*(eMesh/10000) + ((eMesh/10)%10) + cDeltaY;
			aMovedUnit.eMesh = 10*(10*(100*(aY/8) + (aX/8)) + (aY%8)) + (aX%8);
			aMovedUnit.eX    = 0;
			aMovedUnit.eY    = 0;
			break;}
		
		case 3:{
			assert((eX>=0) && (eX<4));
			assert((eY>=0) && (eY<4));
			int	aX0 = ((mOrigin/100) % 100) % 2;
			int	aY0 = ((mOrigin/100) / 100) % 2;
			int	aX  = eX + 2*(eMesh%100-aX0) + cDeltaX;
			int	aY  = eY + 2*(eMesh/100-aY0) + cDeltaY;
			aMovedUnit.eMesh = 100*(2*(aY/4)+aY0) + (2*(aX/4)+aX0);
			aMovedUnit.eX    = aX % 4;
			aMovedUnit.eY    = aY % 4;
			break;}

		case 4:{
			assert((eX==0) && (eY==0));
			int	aX0 = ((mOrigin/100) % 100) % 2;
			int	aY0 = ((mOrigin/100) / 100) % 2;
			int	aX  = ((eMesh%100)-aX0)/2 + cDeltaX;
			int	aY  = ((eMesh/100)-aY0)/2 + cDeltaY;
			aMovedUnit.eMesh = 100*(2*aY+aY0) + (2*aX+aX0);
			aMovedUnit.eX    = 0;
			aMovedUnit.eY    = 0;
			break;}

		case 5:{
			aMovedUnit.eMesh = mOrigin / 100;
			aMovedUnit.eX    = eX + cDeltaX;
			aMovedUnit.eY    = eY + cDeltaY;
			if((aMovedUnit.eX<0) || (aMovedUnit.eX>=4) || (aMovedUnit.eY<0) || (aMovedUnit.eY>=4))
			{
				aMovedUnit.eMesh = 0;
				aMovedUnit.eX    = 0;
				aMovedUnit.eY    = 0;
			}
			break;}

		case 6:{
			aMovedUnit.eMesh = mOrigin / 100;
			aMovedUnit.eX    = eX + cDeltaX;
			aMovedUnit.eY    = eY + cDeltaY;
			if((0!=aMovedUnit.eX) || (0!=aMovedUnit.eY))
			{
				aMovedUnit.eMesh = 0;
				aMovedUnit.eX    = 0;
				aMovedUnit.eY    = 0;
			}
			break;}

		default:
			assert(0);
	}
	aMovedUnit.eLevel = eLevel;

	return aMovedUnit;
}


/*
**	�X�P�[���̃A�b�v�_�E��
*/

// �A�b�v
NvUnit NvUnit::getUp(void) const
{
	NvUnit	aNewUnit;

	switch(eLevel)
	{
		case 0:	
		case 1:
			aNewUnit.eMesh = eMesh;
			aNewUnit.eX    = eX / 4;
			aNewUnit.eY    = eY / 4;
			break;

		case 2:{
			int	aX0 = ((mOrigin/100) % 100) % 2;
			int	aY0 = ((mOrigin/100) / 100) % 2;
			int	aX1 = (eMesh/100) % 100;
			int	aY1 = eMesh / 10000;
			int	aX2 = eMesh % 10;
			int	aY2 = (eMesh/10) % 10;
			int	aX  = 2*((aX1-aX0)/2) + aX0;
			int	aY  = 2*((aY1-aY0)/2) + aY0;
			aNewUnit.eMesh = 100*aY + aX;
			aX = 8*(aX1-aX) + aX2;
			aY = 8*(aY1-aY) + aY2;
			aNewUnit.eX = aX / 4;
			aNewUnit.eY = aY / 4;
			break;}

		case 3:
			aNewUnit.eMesh = eMesh;
			aNewUnit.eX    = 0;
			aNewUnit.eY    = 0;
			break;

		case 4:{
			int	aX0 = ((mOrigin/100) % 100);
			int	aY0 = ((mOrigin/100) / 100);
			int	aX1 = eMesh % 100;
			int	aY1 = eMesh / 100;
			aNewUnit.eMesh = mOrigin / 100;
			aNewUnit.eX    = (aX1-aX0) / 8;
			aNewUnit.eY    = (aY1-aY0) / 8;
			break;}

		case 5:{
			aNewUnit.eMesh = mOrigin / 100;
			aNewUnit.eX    = 0;
			aNewUnit.eY    = 0;
			break;}
		
		default:
			assert(0);
			break;
	}
	aNewUnit.eLevel = eLevel + 1;

	return aNewUnit;
}

// �_�E��
NvUnit NvUnit::getDown(void) const
{
	NvUnit	aNewUnit;

	switch(eLevel)
	{
		case 1:
			aNewUnit.eMesh = eMesh;
			aNewUnit.eX    = 4 * eX;
			aNewUnit.eY    = 4 * eY;
			break;

		case 2:
			aNewUnit.eMesh = eMesh;
			aNewUnit.eX    = eX;
			aNewUnit.eY    = eY;
			break;
		
		case 3:{
			int	aX = 8*(eMesh%100) + 4*eX;
			int	aY = 8*(eMesh/100) + 4*eY;
			aNewUnit.eMesh = 10*(10*(100*(aY/8) + (aX/8)) + (aY%8)) + (aX%8);
			aNewUnit.eX    = 0;
			aNewUnit.eY    = 0;
			break;}
		
		case 4:{
			aNewUnit.eMesh = eMesh;
			aNewUnit.eX    = eX;
			aNewUnit.eY    = eY;
			break;}
		
		case 5:{
			int	aX = (mOrigin/100)%100 + 8*eX;
			int	aY = (mOrigin/100)/100 + 8*eY;
			aNewUnit.eMesh = 100*aY + aX;
			aNewUnit.eX = 0;
			aNewUnit.eY = 0;
			break;}
		
		case 6:{
			aNewUnit.eMesh = mOrigin / 100;
			aNewUnit.eX    = 0;
			aNewUnit.eY    = 0;
			break;}

		default:
			break;
	}
	aNewUnit.eLevel = eLevel - 1;

	return aNewUnit;
}

// ���x���̕ύX
NvUnit NvUnit::getLevel(int cLevel) const
{
	NvUnit	aNewUnit = *this;
	if(eLevel > cLevel)
	{
		for(int i=0 ; i<eLevel-cLevel ; ++i) aNewUnit.down();
	}
	else if(eLevel < cLevel)
	{
		for(int i=0 ; i<cLevel-eLevel ; ++i) aNewUnit.up();
	}

	return aNewUnit;
}


/*
**	�C���f�b�N�X�̎��o�����ݒ�
*/

int NvUnit::getIndex(void) const
{
	return eX + eY*getDivideCount();
}

NvUnit& NvUnit::setIndex(int cIndex)
{
	int	aDivide = getDivideCount();
	assert((cIndex>=0) && (cIndex<aDivide*aDivide));

	eX = cIndex % aDivide;
	eY = cIndex / aDivide;

	return *this;
}


/*
**	�f�B���N�g���E�t�@�C�����̐���
*/

// ���ԃt�@�C���̃f�B���N�g������
char* NvUnit::getDirectory(
	char*	cBuffer) const
{
	assert(0 != cBuffer);

	switch(eLevel)
	{
		case 0:
			sprintf(cBuffer, "detail/%d", eMesh);
			break;
		case 1:
		case 2:
			sprintf(cBuffer, "%d", eMesh/100);
			break;
		case 3:
		case 4:
			strcpy(cBuffer, "middle");
			break;
		case 5:
		case 6:
			strcpy(cBuffer, "top");
			break;
		default:
			assert(0);
			break;
	}

	return cBuffer;
}

// ���ԃt�@�C���̃t�@�C������
char* NvUnit::getFilebody(
	char*	cBuffer) const
{
	assert(cBuffer != 0);
	assert((eLevel>=0) && (eLevel<=6));

	sprintf(cBuffer, "%dL%d_%x%x", eMesh, eLevel, eX, eY);

	return cBuffer;
}

// �ǂݍ��݃f�B���N�g�����̍쐬
void NvUnit::createReadDirectory(
	const char*		cDirectory,		// �f�[�^�i�[�f�B���N�g��
	char*			cDirname)		// �f�B���N�g�������o���o�b�t�@
	const
{
	char	aTempBuf[32];
	sprintf(cDirname, "%s/%s", cDirectory, getDirectory(aTempBuf));
}

// �ǂݍ��݃t�@�C�����̍쐬
void NvUnit::createReadFilename(
	const char*		cDirectory,		// �f�[�^�i�[�f�B���N�g��
	char*			cFilename)		// �t�@�C�������o���o�b�t�@
	const
{
	char	aTempBuf[32];
	sprintf(cFilename, "%s/%s", cDirectory, getDirectory(aTempBuf));
	sprintf(cFilename, "%s/%s", cFilename,  getFilebody(aTempBuf));
}

// �������݃f�B���N�g�����̍쐬
bool NvUnit::createWriteDirectory(
	const char*		cDirectory,		// �f�[�^�i�[�f�B���N�g��
	char*			cDirname)		// �f�B���N�g�������o���o�b�t�@
	const
{
	char	aTempBuf[32];
	sprintf(cDirname, "%s/%s", cDirectory, getDirectory(aTempBuf));
	if(! ZFile::CreateDir(cDirname)) return false;

	return true;
}

// �������݃t�@�C�����̍쐬
bool NvUnit::createWriteFilename(
	const char*		cDirectory,		// �f�[�^�i�[�f�B���N�g��
	char*			cFilename)		// �t�@�C�������o���o�b�t�@
	const
{
	char	aTempBuf[32];
	sprintf(cFilename, "%s/%s", cDirectory, getDirectory(aTempBuf));

	bool	aRes = ZFile::CreateDir(cFilename);
	sprintf(cFilename, "%s/%s", cFilename, getFilebody(aTempBuf));

	return aRes;
}

// ���ԃt�@�C���̃f�B���N�g������
char* NvUnit::getPackDir(
	char*	cBuffer) const
{
	assert(0 != cBuffer);

	switch(eLevel)
	{
		case 0:
		case 1:
		case 2:
			sprintf(cBuffer, "%d", eMesh/100);
			break;
		case 3:
		case 4:
			strcpy(cBuffer, "middle");
			break;
		case 5:
		case 6:
			strcpy(cBuffer, "top");
			break;
		default:
			assert(0);
			break;
	}

	return cBuffer;
}

// ���ԃt�@�C���̃t�@�C������
char* NvUnit::getPackFile(
	char*	cBuffer) const
{
	assert(cBuffer != 0);
	assert((eLevel>=0) && (eLevel<=6));

	sprintf(cBuffer, "%dL%d", eMesh, eLevel);

	return cBuffer;
}

// �ǂݍ��݃f�B���N�g�����̍쐬
void NvUnit::createReadPackDir(
	const char*		cDirectory,		// �f�[�^�i�[�f�B���N�g��
	char*			cDirname)		// �f�B���N�g�������o���o�b�t�@
	const
{
	char	aTempBuf[32];
	sprintf(cDirname, "%s/%s", cDirectory, getPackDir(aTempBuf));
}

// �ǂݍ��݃t�@�C�����̍쐬
void NvUnit::createReadPackFile(
	const char*		cDirectory,		// �f�[�^�i�[�f�B���N�g��
	char*			cFilename)		// �t�@�C�������o���o�b�t�@
	const
{
	char	aTempBuf[32];
	sprintf(cFilename, "%s/%s", cDirectory, getPackDir(aTempBuf));
	sprintf(cFilename, "%s/%s", cFilename,  getPackFile(aTempBuf));
}

// �������݃f�B���N�g�����̍쐬
bool NvUnit::createWritePackDir(
	const char*		cDirectory,		// �f�[�^�i�[�f�B���N�g��
	char*			cDirname)		// �f�B���N�g�������o���o�b�t�@
	const
{
	char	aTempBuf[32];
	sprintf(cDirname, "%s/%s", cDirectory, getPackDir(aTempBuf));
	if(! ZFile::CreateDir(cDirname)) return false;

	return true;
}

// �������݃t�@�C�����̍쐬
bool NvUnit::createWritePackFile(
	const char*		cDirectory,		// �f�[�^�i�[�f�B���N�g��
	char*			cFilename)		// �t�@�C�������o���o�b�t�@
	const
{
	char	aTempBuf[32];
	sprintf(cFilename, "%s/%s", cDirectory, getPackDir(aTempBuf));

	bool	aRes = ZFile::CreateDir(cFilename);
	sprintf(cFilename, "%s/%s", cFilename, getPackFile(aTempBuf));

	return aRes;
}


/*
**	�w�t�H�[�}�b�g�ɂ����郁�b�V���ԍ������o��
*/

int NvUnit::getMesh(void) const
{
	int	aMesh;
	switch(eLevel)
	{
		case 0:
			aMesh = 256*ZDigit::DECtoBCD(eMesh) | 16*eY | eX;
			break;
		case 1:
			aMesh = 256*ZDigit::DECtoBCD(eMesh) | 16*(0xc|eY) | (0xc|eX);
			break;
		case 2:
			aMesh = 256*ZDigit::DECtoBCD(eMesh) | 0xf2;
			break;
		case 3:{
			int	aX = eMesh%100 + ((eX<2) ? 0 : 1);
			int	aY = eMesh/100 + ((eY<2) ? 0 : 1);
			aMesh = 256*ZDigit::DECtoBCD(100*(100*aY+aX)) | 0xf300 | 16*(eY%2) | (eX%2);
			break;}
		case 4:
			aMesh = 256*ZDigit::DECtoBCD(100*eMesh) | 0xf4ff;
			break;
		case 5:{
			int	aX = eMesh%100 + 8*eX;
			int	aY = eMesh/100 + 8*eY;
			aMesh = 256*256*ZDigit::DECtoBCD(100*aY+aX) | 0xf5ff;
			break;}
		case 6:{
			aMesh = (ZDigit::DECtoBCD(eMesh)<<16) | 0xf600;
			break;}
		default:
			aMesh = 0;
			break;
	}
	assert(0 != aMesh);

	return aMesh;
}


/*
**	�j�h�v�h�t�H�[�}�b�g�ɂ����郁�b�V���R�[�h�����o��
*/

KiwiMesh NvUnit::getKiwiMesh(void) const
{
	ZPoint	aPos;
	switch(eLevel)
	{
		case 0:
			aPos = ZMap::GetMeshCornerPos(eMesh);
			aPos.eX += 28125 * eX;	// 450 * 1000 / 16
			aPos.eY += 18750 * eY;	// 300 * 1000 / 16
			break;
		case 1:
			aPos = ZMap::GetMeshCornerPos(eMesh);
			aPos.eX += 112500 * eX;	// 450 * 1000 / 4
			aPos.eY += 75000 * eY;	// 300 * 1000 / 4
			break;
		case 2:
			aPos = ZMap::GetMeshCorner(eMesh);
			break;
		case 3:
			aPos = ZMap::GetMeshCornerPos(100*eMesh);
			aPos.eX += 4*450000 * eX;
			aPos.eY += 4*300000 * eY;
			break;
		case 4:
			aPos = ZMap::GetMeshCornerPos(100*eMesh);
			break;
		case 5:
			aPos.eX = 0;
			aPos.eY = 0;
			break;
		case 6:
			aPos.eX = 0;
			aPos.eY = 0;
			break;
		default:
			assert(0);
			break;
	}

	return KiwiMesh(aPos.eX, aPos.eY);
}


/*
**	�R���X�g���N�^
*/

KiwiMesh::KiwiMesh(
	int		cLon,
	int		cLat)
{
	eLon.eDeg = cLon / 3600000;
	eLon.eMin = (cLon%3600000) / 60000;
	eLon.eSec = (cLon%3600000) % 60000;

	eLat.eDeg = cLat / 3600000;
	eLat.eMin = (cLat%3600000) / 60000;
	eLat.eSec = (cLat%3600000) % 60000;
}


/*
**	�Q�̃��j�b�g�̍��������o��
*/

ZPoint NvUnit::getDiff(
	const NvUnit&	cUnitcode)
	const
{
	assert(eLevel == cUnitcode.eLevel);

	return ZPoint(getUniqX()-cUnitcode.getUniqX(), getUniqY()-cUnitcode.getUniqY());
}

int NvUnit::getDiffX(
	const NvUnit&	cUnitcode)
	const
{
	assert(eLevel == cUnitcode.eLevel);

	return getUniqX() - cUnitcode.getUniqX();
}

int NvUnit::getDiffY(
	const NvUnit&	cUnitcode)
	const
{
	assert(eLevel == cUnitcode.eLevel);

	return getUniqY() - cUnitcode.getUniqY();
}


/*
**	���j�b�g�́u�l�v�����o��
*/

ZPoint NvUnit::getUniqValue(void) const
{
	return ZPoint(getUniqX(), getUniqY());
}

int NvUnit::getUniqX(void) const
{
	int	aValue;
	switch(eLevel)
	{
		case 0:
			aValue = 8*((eMesh/100)%100) + (eMesh%10);
			aValue = 16*aValue + eX;
			break;
		case 1:
			aValue = 8*((eMesh/100)%100) + (eMesh%10);
			aValue = 4*aValue + eX;
			break;
		case 2:
			aValue = 8*((eMesh/100)%100) + (eMesh%10);
			break;
		case 3:
			aValue = (eMesh%100) / 2;
			aValue = 4*aValue + eX;
			break;
		case 4:
			aValue = (eMesh%100) / 2;
			break;
		case 5:
			aValue = eX;
			break;
		case 6:
			aValue = 0;
			break;
		default:
			aValue = 0;
			break;
	}

	return aValue;
}

int NvUnit::getUniqY(void) const
{
	int	aValue;
	switch(eLevel)
	{
		case 0:
			aValue = 8*(eMesh/10000) + (eMesh/10)%10;
			aValue = 16*aValue + eY;
			break;
		case 1:
			aValue = 8*(eMesh/10000) + (eMesh/10)%10;
			aValue = 4*aValue + eY;
			break;
		case 2:
			aValue = 8*(eMesh/10000) + (eMesh/10)%10;
			break;
		case 3:
			aValue = (eMesh/100) / 2;
			aValue = 4*aValue + eY;
			break;
		case 4:
			aValue = (eMesh/100) / 2;
			break;
		case 5:
			aValue = eY;
			break;
		case 6:
			aValue = 0;
			break;
		default:
			aValue = 0;
			break;
	}

	return aValue;
}


/*
**	���[�e�B���e�B���\�b�h�Q
*/

// ���x���ɂ�镪���������o��
int NvUnit::GetDivideCount(
	int		cLevel)				// ���x��
{
	switch(cLevel)
	{
		case 0:	return 16;
		case 1:	return 4;
		case 2:	return 1;
		case 3:	return 4;
		case 4:	return 1;
		case 5:	return 4;
		case 6:	return 1;
	}

	return 0;
}

// ���x���ɂ�郆�j�b�g�T�C�Y�����o��
int NvUnit::GetUnitSize(
	int		cLevel)				// ���x��
{
	switch(cLevel)
	{
		case 0:	return zin::unitsize / 16;
		case 1:	return zin::unitsize / 4;
		case 2:	return zin::unitsize;
		case 3:	return zin::unitsize * 4;
		case 4:	return zin::unitsize * 16;
		case 5:	return zin::unitsize * 64;
		case 6:	return zin::unitsize * 256;
	}

	return 0;
}


/*
**	for debug
*/

ostream& operator << (ostream& os, const NvUnit& cUnit)
{
	os << '[' << cUnit.eMesh;
	os << '-' << cUnit.eLevel;
	os << ';' << cUnit.eX;
	os << ',' << cUnit.eY << ']';

	return os;
}

