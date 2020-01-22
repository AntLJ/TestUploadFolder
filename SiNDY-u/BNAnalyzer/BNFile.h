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
	Name,					// 漢字建物名称(旧BuildingName)
	OrgName1,				// 加工前名称1
	OrgName2,				// 加工前名称2
	Kana,					// カナ(旧BuildingName_Yomi)
	Addr,					// 整形後住所(旧Address)
	OrgAddr,				// 成形前住所
	AddrCode,				// 5桁住所
	Addr_Lon,				// 住所経度
	Addr_Lat,				// 住所緯度
	RepType,				// 地番代表点複合コード
	AddrLevel,				// 住所レベル
	PinPoint,				// ピンポイントフラグ
	MultiSameAddr,			// 同一素材住所複数存在フラグ
	MultiSameBldg,			// 同一家形内複数住所存在フラグ
	SameAddr,				// 同一iPC住所複数存在フラグ
	SameAddr_Count,			// 同一iPC住所数
	Addr_Ver,				// 住所解析元XY付与
	SourceType,				// 出典コード(旧Source)
	SourceName,				// 出典元(旧Original_Source
	SourceDate,				// 出典資料入手時期(旧TimeOfSource)
	Source_ID,				// 出典資料行番号(旧Source_No)
	Building1_Div1,			// 建物用途区分①(旧UseOfBld1)
	Building1_Div2,			// 建物用途区分②(旧UseOfBld2)
	Building2_Div1,			// 建物棟用途区分①(旧UseOfBld3)
	Building2_Div2,			// 建物棟用途区分②(旧UseOfBld4)
	Height,					// 建物高さ
	Floors,					// 地上(旧Ground)
	BaseMentFloors,			// 地下(旧Under)
	BuildingArea,			// 建面積(旧Area)
	AllArea,				// 延べ床面積(旧AreaAll)
	Src_StartDate,			// 素材使用開始年月日(旧StartTime)
	Src_UpdateDate,			// 素材更新年月日(旧UpdateTime)
	Update,					// 更新コード
	UpdateDate,				// 更新年月日
	Adoption,				// 採用コード
	PrefCode,				// 都道府県コード
	CityCode,				// 市区町村コード
	OazaCode,				// 大字・通称コード
	ChomeCode,				// 字・丁目コード
	ChibanCode,				// 地番コード
	GouCode,				// 号コード
	PrefName,				// 都道府県名
	CityName,				// 市区町村名
	OazaName,				// 大字・通称名
	ChomeName,				// 字・丁目名
	ChibanName,				// 地番
	GouName,				// 号
	IPCode,					// ＩＰコード
	Precision,				// 精度
	ChibanFlag,				// 代表点・地番止まりフラグ
	AddrLevel2,				// 住所レベル
	Longitude,				// 実数経度
	Latitude,				// 実数緯度
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

