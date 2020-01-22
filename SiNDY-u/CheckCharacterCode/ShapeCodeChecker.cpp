#include "stdafx.h"
#include "ShapeCodeChecker.h"

bool ShapeCodeChecker::CheckCharacterCode(bool isImmediateExit)
{
	CString strMsg;
	bool isCheckOK = true;

	// ファイルリストに書かれた対象ファイルでループ
	for(const auto& itlTargetFile : m_vecTargetFileList){
		CDBFHandle dbfHandle;

		// DBFオープン
		if(!dbfHandle.open(CT2A(itlTargetFile), "rb")){
			strMsg.Format(_T("%s\tNG：ファイルオープン失敗"), itlTargetFile);
			CLog::GetInstance().PrintLog(true, false, true, true, strMsg);
			isCheckOK = false;
			continue;
		}

		// 文字列型フィールドを抽出
		std::map<int, std::string> mapFieldName;

		for(int i=0; i<dbfHandle.get_FieldCount(); ++i){
			char* fldName = nullptr;
			int iWidth = 0, iDecimals = 0;

			DBFFieldType iDbfFieldType;
			iDbfFieldType = dbfHandle.get_FieldInfo(i, &fldName, &iWidth, &iDecimals);
			if(iDbfFieldType == FTString){
				std::string strFieldName = fldName;
				mapFieldName[i] = strFieldName;
			}
		}

		// 文字列型フィールドがなければ次のファイルを読み込む
		if(mapFieldName.empty()){
			strMsg.Format(_T("%s\t文字列フィールドなし"), itlTargetFile);
			CLog::GetInstance().PrintLog(true, false, true, false, strMsg);
			continue;
		}

		bool isParmittedFile = true;

		// レコードでループ
		for(int i=0; i<dbfHandle.get_RecordCount(); ++i){

			// 文字列型フィールドでループ
			for(const auto& itlFieldID : mapFieldName){

				// 文字列データを取得
				const char *strTemp;
				dbfHandle.read_Attribute(i, itlFieldID.first, &strTemp);
				bool isParmittedString = true;
				std::string chkStr = strTemp;

				// 文字列が空なら次のフィールドにスキップ
				if(chkStr.empty()){
					continue;
				}

				// 取得した文字列をIsParmittedに投げる
				if(!IsParmitted(chkStr)){
					isParmittedString = false;
					isParmittedFile = false;
					isCheckOK = false;
				}

				// 不正文字列をエラーログに出力
				if(!isParmittedString){
					CString strFileName = itlTargetFile.Mid(itlTargetFile.ReverseFind(_T('\\'))+1);

					CString strFieldName = itlFieldID.second.c_str();
					strMsg.Format(_T("%s\t%d\t%s\tNG"), strFileName, i, strFieldName);
					CLog::GetInstance().PrintLog(false, false, false, true, strMsg);
				}

				// 即時終了モードの場合中断して次のファイルに移る
				if(isImmediateExit && !isParmittedFile){
					break;
				}
			}

			if(isImmediateExit && !isParmittedFile){
				break;
			}
		}

		// ファイル内に不正文字コードがあったか
		if(isParmittedFile){
			strMsg.Format(_T("%s\tOK"), itlTargetFile);
			CLog::GetInstance().PrintLog(true, false, true, false, strMsg);
		} else {
			strMsg.Format(_T("%s\tNG"), itlTargetFile);
			CLog::GetInstance().PrintLog(true, false, true, false, strMsg);
		}
	}

	return isCheckOK;
}
