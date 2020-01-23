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

#include "StdAfx.h"
#include "AddrCount.h"
#include <AddrLib/GouPoints.h>
#include <AddrLib/CSAddrPoint.h>

bool AddrCount::Init(bool cCharF, ETarget cTarget, const CString& cTgtOpt, IFeatureClassPtr cGouPoint, IFeatureClassPtr cCityAdmin, IFeatureClassPtr cCSAddrPoint)
{
	mCharF			= cCharF;
	mTarget			= cTarget;
	mTgtOpt			= cTgtOpt;
	mGouPoint		= cGouPoint;
	mCityAdmin		= cCityAdmin;
	mCSAddrPoint	= CFeatureClass(cCSAddrPoint);

	mFMapCSPoint	= CSPFieldMap(mCSAddrPoint.GetFieldMap());
	mNameCSPoint	= CSPTableNameString(mCSAddrPoint.GetNameString());

	if(!fnCreateAddrList()) return false;

	return true;
}

bool AddrCount::Count()
{
	// 5���R�[�h���X�g�̃��[�v
	for(CAddrCodeList::const_iterator itrList = mList.begin(); itrList != mList.end(); ++itrList)
	{
		cerr << CT2CA(*itrList) << " : �J�E���g��" << endl;

		CGouPoints aGouPts(mCityAdmin);
		aGouPts.SelectByAddrCode(*itrList);

		AddrSet aAddrSet;

		// �s���E�̃��[�v
		for(CCityAdmin::const_rows_iterator itrAdmin = aGouPts.begin(aGouPts.GetTableType()); itrAdmin != aGouPts.end(aGouPts.GetTableType()); ++itrAdmin)
		{
			const CSPCityAdmin& rAdmin = boost::static_pointer_cast<CCityAdmin>(*itrAdmin);
			CString aAddrCode2 = rAdmin->GetOazaAzaCode2();

			// �����t���Z�����[�h�łȂ��A�Z���\�� or �n�Ԑ����ς݁^�X�敄����NULL�̏ꍇ�̓X�L�b�v
			if(!mCharF && rAdmin->GetGaikuFugo() == 0 &&
				(rAdmin->GetAddrClass() == city_admin::addr_class::kChibanMaintenance ||
				rAdmin->GetAddrClass() == city_admin::addr_class::kResidenceIndicationMaintenance))
			{
				cerr << "#Notice �Z���\�� or �n�Ԑ����ς݁^�X�敄����NULL : " << CT2A(rAdmin->GetOazaAzaCode()) << endl;
				continue;
			}

			// ���|�C���g�̃��[�v
			for(CGouPoint::const_rows_iterator itrGou = rAdmin->begin(sindyTableType::gou_point); itrGou != rAdmin->end(sindyTableType::gou_point); ++itrGou)
			{
				const CSPGouPoint pGou = boost::static_pointer_cast<CGouPoint>(*itrGou);

				if(!mCharF) {	// �����t���Z�����[�h�łȂ�

					CString aAddrCode(pGou->GetAddrCode());		// 20���R�[�h

					if(aAddrCode.IsEmpty()) {
//						cerr << "#Notice 20���Z���R�[�h���󕶎��� : " << pGou->GetOID() << " : " << CT2A(pGou->GetGouNo()) << endl;
						continue;
					}

					// 20���R�[�h����������set�ɓ���
					aAddrSet.insert(aAddrCode);

					// �s���E�� ADDRCODE2 ���ݒ肳��Ă���ꍇ
					if(aAddrCode2 != _T("000000")) {
						aAddrCode = aAddrCode.Left(5) + aAddrCode2 + aAddrCode.Mid(11);
						aAddrSet.insert(aAddrCode);
					}

				} else {		// �����t���Z�����[�h�̏ꍇ

					CString aAddrCode(rAdmin->GetAddrCode());	// �s���E�̏Z���R�[�h(11 or 16��)
					CString	aExGaiku(rAdmin->GetExGaikuFugo());	// �g���X�敄��

					CString	aGouNo(pGou->GetGouNo());			// ���ԍ�
					vector<CString> aTokens;
					fnTokenizeGouNo(aGouNo, aTokens);			// ���ԍ����g�[�N���ɕ���

					if(!aExGaiku.IsEmpty()) {	// �g���X�敄������
						// �g�����ԍ��t���O�������Ă�����̂�20���R�[�h���ł�����̈ȊO���J�E���g�ΏۂƂ���
						if(!(pGou->IsExGouNo() && aGouNo.Left(1) != _T("#") && 0 < aTokens.size() && aTokens.size() <= 2 &&
							aTokens[0].GetLength() <= 5 && (aTokens.size() < 2 || aTokens[1].GetLength() <= 4))) {
							aAddrCode = aAddrCode + "|" + aExGaiku + "|" + aGouNo;
							aAddrSet.insert(aAddrCode);
						}
					} else {
						// �g�����ԍ��t���O�������Ă���ꍇ��16���R�[�h��11���ɍ��
						if(pGou->IsExGouNo() && aAddrCode.GetLength() == 16) aAddrCode = aAddrCode.Left(11);
						if(aGouNo.Left(1) == _T("#")) {	// �擪�Ɂu#�v
							aAddrCode = aAddrCode + "|" + aGouNo;
							aAddrSet.insert(aAddrCode);
						}
						else if(aGouNo != _T("*")) {
							if(aTokens.size() > 0) {
								if(aAddrCode.GetLength() == 16) {	// �X�敄���E
									if(aTokens.size() > 1 || aTokens[0].GetLength() > 4) {
										// �n�C�t�����P�ȏ� or �擪�̐������T���ȏ�
										aAddrCode = aAddrCode + "|" + aGouNo;
										aAddrSet.insert(aAddrCode);
									}
								} else {	// �X�敄���E�łȂ�
									if(aTokens.size() > 2 || aTokens[0].GetLength() > 5 || (aTokens.size() > 1 && aTokens[1].GetLength() > 4)) {
										// �n�C�t�����Q�ȏ� or �擪�̐������U���ȏ� or �Q�ڂ̐������T���ȏ�
										aAddrCode = aAddrCode + "|" + aGouNo;
										aAddrSet.insert(aAddrCode);
									}
								}
							}
						}
					}
				}
			}
		}

		// CS�|�C���g�̃J�E���g�i�����t���Z�����[�h�̍ۂ̓J�E���g���Ȃ��j
		if(!mCharF) {
			IQueryFilterPtr ipFilter(CLSID_QueryFilter);
			CString aWhere;
			aWhere.Format(_T("%s LIKE '%s%%'"), cs_addr_point::kAddrCode, *itrList);
			ipFilter->put_WhereClause(CComBSTR(aWhere));
			_ICursorPtr ipCursor = mCSAddrPoint.Search(ipFilter, VARIANT_FALSE);
			if(ipCursor) {
				_IRowPtr ipRow;
				while(ipCursor->NextRow(&ipRow) == S_OK) {
					CCSAddrPoint aCSPt(ipRow, sindyTableType::cs_addr_point, false, mFMapCSPoint, mNameCSPoint);
					CString aAddrCode = aCSPt.GetAddrCode();
					aAddrSet.insert(aAddrCode);
				}
			}
		}

		// ���j�[�N�����̃J�E���g
		for(AddrSet::const_iterator itrSet = aAddrSet.begin(); itrSet != aAddrSet.end(); ++itrSet)
		{
#ifdef _DEBUG
			if(mCharF) cerr << "#Notice �����t���Z�� : " << CT2A(*itrSet) << endl;
#endif
			(*this)[(*itrSet).Left(11)]++;
		}
	}

	return true;
}

bool AddrCount::fnCreateAddrList()
{
	mList.SetObject(mCityAdmin);
	switch(mTarget) {
		case kTgtAll:
			if(!mList.MakeCodeListAll(5)) {
				cerr << "#Error �Z���R�[�h���X�g�̐����Ɏ��s�i�S���w��j" << endl;
				return false;
			}
			break;
		case kTgtRange:
			if(!mList.MakeCodeListFromRange(mTgtOpt.Left(5), mTgtOpt.Right(5))) {
				cerr << "#Error �Z���R�[�h���X�g�̐����Ɏ��s�i�͈͎w��j : " << CT2A(mTgtOpt) << endl;
				return false;
			}
			break;
		case kTgtList:
			if(!mList.AddFromFile(mTgtOpt)) {
				cerr << "#Error �Z���R�[�h���X�g�t�@�C���̓ǂݍ��݂Ɏ��s : " << CT2A(mTgtOpt) << endl;
				return false;
			}
			break;
		case kTgtSingle:
			mList.Add(mTgtOpt);
			break;
		default:
			// �����ɗ��邱�Ƃ͂܂��Ȃ�
			cerr << "#Error �J�E���g�Ώێw�肪�s��" << endl;
			return false;
	}

	return true;
}

void AddrCount::fnTokenizeGouNo(const CString& cGouNo, vector<CString>& cTokens)
{
	int aPos(0);
	CString aToken = cGouNo.Tokenize(_T("-"), aPos);
	while(!aToken.IsEmpty()) {
		cTokens.push_back(aToken);
		aToken = cGouNo.Tokenize(_T("-"), aPos);
	}
}
