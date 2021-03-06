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
	Name,					// ¿¨¼Ì(BuildingName)
	OrgName1,				// ÁHO¼Ì1
	OrgName2,				// ÁHO¼Ì2
	Kana,					// Ji(BuildingName_Yomi)
	Addr,					// ®`ãZ(Address)
	OrgAddr,				// ¬`OZ
	AddrCode,				// 5Z
	Addr_Lon,				// Zox
	Addr_Lat,				// ZÜx
	RepType,				// nÔã\_¡R[h
	AddrLevel,				// Zx
	PinPoint,				// s|CgtO
	MultiSameAddr,			// ¯êfÞZ¡¶ÝtO
	MultiSameBldg,			// ¯êÆ`à¡Z¶ÝtO
	SameAddr,				// ¯êiPCZ¡¶ÝtO
	SameAddr_Count,			// ¯êiPCZ
	Addr_Ver,				// ZðÍ³XYt^
	SourceType,				// oTR[h(Source)
	SourceName,				// oT³(Original_Source
	SourceDate,				// oT¿üèú(TimeOfSource)
	Source_ID,				// oT¿sÔ(Source_No)
	Building1_Div1,			// ¨præª@(UseOfBld1)
	Building1_Div2,			// ¨præªA(UseOfBld2)
	Building2_Div1,			// ¨præª@(UseOfBld3)
	Building2_Div2,			// ¨præªA(UseOfBld4)
	Height,					// ¨³
	Floors,					// nã(Ground)
	BaseMentFloors,			// nº(Under)
	BuildingArea,			// ÊÏ(Area)
	AllArea,				// ×°ÊÏ(AreaAll)
	Src_StartDate,			// fÞgpJnNú(StartTime)
	Src_UpdateDate,			// fÞXVNú(UpdateTime)
	Update,					// XVR[h
	UpdateDate,				// XVNú
	Adoption,				// ÌpR[h
	PrefCode,				// s¹{§R[h
	CityCode,				// sæ¬ºR[h
	OazaCode,				// åEÊÌR[h
	ChomeCode,				// EÚR[h
	ChibanCode,				// nÔR[h
	GouCode,				// R[h
	PrefName,				// s¹{§¼
	CityName,				// sæ¬º¼
	OazaName,				// åEÊÌ¼
	ChomeName,				// EÚ¼
	ChibanName,				// nÔ
	GouName,				// 
	IPCode,					// hoR[h
	Precision,				// ¸x
	ChibanFlag,				// ã\_EnÔ~ÜètO
	AddrLevel2,				// Zx
	Longitude,				// Àox
	Latitude,				// ÀÜx
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

