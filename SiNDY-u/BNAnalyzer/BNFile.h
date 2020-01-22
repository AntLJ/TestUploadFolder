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

#pragma once
#include <WinLib/CStringTokenizer.h>

typedef std::vector<CString> CStringArray;

#define	LINESIZE		5000
#define	SKIPLINE		1

namespace BN {
enum BNCOL {
	ObjectID = 0,			// OBJECTID
	Name,					// ������������(��BuildingName)
	OrgName1,				// ���H�O����1
	OrgName2,				// ���H�O����2
	Kana,					// �J�i(��BuildingName_Yomi)
	Addr,					// ���`��Z��(��Address)
	OrgAddr,				// ���`�O�Z��
	AddrCode,				// 5���Z��
	Addr_Lon,				// �Z���o�x
	Addr_Lat,				// �Z���ܓx
	RepType,				// �n�ԑ�\�_�����R�[�h
	AddrLevel,				// �Z�����x��
	PinPoint,				// �s���|�C���g�t���O
	MultiSameAddr,			// ����f�ޏZ���������݃t���O
	MultiSameBldg,			// ����ƌ`�������Z�����݃t���O
	SameAddr,				// ����iPC�Z���������݃t���O
	SameAddr_Count,			// ����iPC�Z����
	Addr_Ver,				// �Z����͌�XY�t�^
	SourceType,				// �o�T�R�[�h(��Source)
	SourceName,				// �o�T��(��Original_Source
	SourceDate,				// �o�T�������莞��(��TimeOfSource)
	Source_ID,				// �o�T�����s�ԍ�(��Source_No)
	Building1_Div1,			// �����p�r�敪�@(��UseOfBld1)
	Building1_Div2,			// �����p�r�敪�A(��UseOfBld2)
	Building2_Div1,			// �������p�r�敪�@(��UseOfBld3)
	Building2_Div2,			// �������p�r�敪�A(��UseOfBld4)
	Height,					// ��������
	Floors,					// �n��(��Ground)
	BaseMentFloors,			// �n��(��Under)
	BuildingArea,			// ���ʐ�(��Area)
	AllArea,				// ���׏��ʐ�(��AreaAll)
	Src_StartDate,			// �f�ގg�p�J�n�N����(��StartTime)
	Src_UpdateDate,			// �f�ލX�V�N����(��UpdateTime)
	Update,					// �X�V�R�[�h
	UpdateDate,				// �X�V�N����
	Adoption,				// �̗p�R�[�h
	PrefCode,				// �s���{���R�[�h
	CityCode,				// �s�撬���R�[�h
	OazaCode,				// �厚�E�ʏ̃R�[�h
	ChomeCode,				// ���E���ڃR�[�h
	ChibanCode,				// �n�ԃR�[�h
	GouCode,				// ���R�[�h
	PrefName,				// �s���{����
	CityName,				// �s�撬����
	OazaName,				// �厚�E�ʏ̖�
	ChomeName,				// ���E���ږ�
	ChibanName,				// �n��
	GouName,				// ��
	IPCode,					// �h�o�R�[�h
	Precision,				// ���x
	ChibanFlag,				// ��\�_�E�n�Ԏ~�܂�t���O
	AddrLevel2,				// �Z�����x��
	Longitude,				// �����o�x
	Latitude,				// �����ܓx
};

inline CString read_address_code(LPCTSTR str)
{
	CStringTokenizer tk(CString(str), _T(","), TOKEN_RET_EMPTY_ALL);
	int n = tk.CountTokens();
	CStringArray a;
	for (int i = 0; i < n; i++) a.push_back(tk.GetNextToken());
	CString ret;
	ret.Format(_T("%s%s%s%s%s%s"), a[PrefCode], a[CityCode], a[OazaCode], a[ChomeCode], a[ChibanCode], a[GouCode]);
	return ret;
}

}

class BNFile
{
	CString filename;
	std::ifstream fs;
	int line;
	std::map<CString, int> adrnum;

public:
	BNFile(void) : line(0), filename(_T("")) {}
	BNFile(LPCTSTR fname) : line(0), filename(fname?fname:_T("")) { open(fname); }
	virtual ~BNFile(void) {}
	void open(LPCTSTR fname) {
		close();
		filename = fname ? fname : _T("");
		if (fname == NULL) return;
		fs.open(fname);
		line = 0;
		analyze();
	}
	bool is_open() { return fs.is_open(); }
	void close() { if (fs.is_open()) fs.close(); line = 0; }
	int read(CStringArray &data) {
		std::ifstream::pos_type pt = fs.tellg();
		data.clear();
		if (! is_open()) return 0;
		char c[LINESIZE];
		if (! fs.getline(c, sizeof(c)))
			return 0;
		CStringTokenizer tkzr(CString(CA2CT(c)), _T(","), TOKEN_RET_EMPTY_ALL);
		int num = tkzr.CountTokens();
		for (int i = 0; i < num; i++) data.push_back(tkzr.GetNextToken());
		line++;
		return line;
	}
	int get_same_code_count(LPCTSTR code) {
		std::map<CString, int>::iterator i = adrnum.find(code);
		if (i == adrnum.end()) return 0;
		return i->second;
	}
private:
	void analyze() {
		if (! fs.is_open()) return;
		std::ifstream::pos_type pt = fs.tellg();
		fs.seekg(0, std::ios_base::beg);
		adrnum.clear();
		char c[LINESIZE];
		int count = 0;
		while (fs.getline(c, sizeof(c))) {
			count++;
			if (count <= SKIPLINE) continue;
			CString code = BN::read_address_code(CString(CA2CT(c)));
			std::map<CString, int>::iterator i = adrnum.find(code);
			if (i != adrnum.end()) {
				i->second++;
			} else {
				adrnum.insert(std::pair<CString, int>(code, 1));
			}
		}
		fs.close();
		fs.open(filename);
	}
};

