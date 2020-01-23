#include "stdafx.h"
#include <arctl/coinitializer.h>
#include <comdef.h>

#include "LogSys.h"
#include "TransParam.h"
#include "Executer.h"

int _tmain(int argc, _TCHAR* argv[])
{
	int retval = 0;
	try {
		arctl::coinitializer coinitializer;
		// 引数取得 ログの設定
		TransParam transParam;
		if (! transParam.parse(argc, argv) ||! initLog(transParam)) {
			retval = 1;
		} else {
			// オプションの表示
			procInfoWrite(transParam);
			// 処理実行
			if (! exec(transParam))
				retval = 1;
		}
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		retval = 2;
	} catch (const _com_error &com_err) {
		std::cerr << CT2CA(com_err.ErrorMessage()) << std::endl;
		retval = 2;
	} catch (...) {
		std::cerr << "予期せぬエラーが発生しました" << std::endl;
		retval = 2;
	}
	if (retval == 0)
		LogSys::getInstance().writeProcLog(true, false, _T("正常終了\n"));
	else
		LogSys::getInstance().writeProcLog(true, false, _T("異常終了\n"));
	return retval;
}
