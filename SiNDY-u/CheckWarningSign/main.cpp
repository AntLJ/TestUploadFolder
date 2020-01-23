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
		// �����擾 ���O�̐ݒ�
		TransParam transParam;
		if (! transParam.parse(argc, argv) ||! initLog(transParam)) {
			retval = 1;
		} else {
			// �I�v�V�����̕\��
			procInfoWrite(transParam);
			// �������s
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
		std::cerr << "�\�����ʃG���[���������܂���" << std::endl;
		retval = 2;
	}
	if (retval == 0)
		LogSys::getInstance().writeProcLog(true, false, _T("����I��\n"));
	else
		LogSys::getInstance().writeProcLog(true, false, _T("�ُ�I��\n"));
	return retval;
}
