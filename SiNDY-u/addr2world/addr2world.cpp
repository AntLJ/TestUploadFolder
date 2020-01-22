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

// addr2world.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"


static std::set<CString> Filenames;
static CString Srcdir = _T("");
static CString Dstdir = _T("");

static crd_cnv Crdcnv;
static bool To_jgd2000 = false;

struct AddrFile {
	LPCTSTR		m_fileName;
	int			m_columnCount;
	int			m_lonCol;
};

static AddrFile AddrFileInfo[] = {
	{_T("pref.txt"), 5, 3},
	{_T("city.txt"), 6, 4},
	{_T("addr*.txt"), 11, 9},
	{_T("pnt*.txt"), 10, 7},
	{_T("gou*.txt"), 10, 8},
	{NULL, 0, 0}
};

bool usage()
{
	std::cerr << "�Z���f�[�^�𐢊E���n�n�ϊ�����v���O����" << std::endl;
	std::cerr << "Usage : addr2world.exe [Option]" << std::endl;
	std::cerr << "Option : " << std::endl;
	std::cerr << "         -i [���{���n�n�Z���f�[�^�t�H���_�̎w��(�K�{)]" << std::endl;
	std::cerr << "         -o [���E���n�n�Z���f�[�^�t�H���_(�o�͐�)�̎w��(�K�{)]" << std::endl;
	std::cerr << "         -r [�n��p�����[�^�̃p�X�w��(�C�ӁA�w�肳�ꂽ�ꍇ�͒n��p�����[�^�A�w�肵�Ȃ��ꍇ��3�p�����[�^�ł̕ϊ�)]" << std::endl;
	std::cerr << "�����̓t�@�C���́A�c�a�̔�������10�i�����W�̏Z���f�[�^�e�L�X�g�t�@�C���ƁA�X�֔ԍ��f�[�^" << std::endl;
	std::cerr << "���w�肵���t�H���_�ȉ��̊Y���t�@�C�����������ď������܂�" << std::endl;
	return false;
}

bool arg_check(int argc, _TCHAR **argv)
{
	// �������Ȃ����usage���o��
	if (argc <= 1)
		return usage();

	// �I�v�V�������
	while (--argc) {
		_TCHAR *s = *++argv;
		switch (*s) {
		case _T('-'):
			switch (*++s) {
			case _T('i'):
				Srcdir = *++argv; --argc; break;
			case _T('o'):
				Dstdir = *++argv; --argc; break;
			case _T('r'):
				{
					CString fname = *++argv; --argc;
					std::string param = CT2CA(fname);
					std::cerr << param.c_str() << "�ŏ�������...";
					if (! Crdcnv.JGD2000_RP_Init((char *)param.c_str())) {
						std::cerr << "�n��p�����[�^�̏������Ɏ��s" << std::endl;
						return false;
					}
					std::cerr << "����" << std::endl;
					To_jgd2000 = true;
				}
				break;
			default:
				std::cerr << "invalid option." << std::endl;
				return usage();
			}
			break;
		default:
			return usage();
		}
	}

	// �K�{�I�v�V�����̗L���`�F�b�N
	if (Srcdir.GetLength() == 0 || Dstdir.GetLength() == 0) {
		std::cerr << "-i�I�v�V������-o�I�v�V�����͕K�{�ł�" << std::endl;
		return usage();
	}

	return true;
}

/**
 *	@brief	<b>�f�B���N�g���쐬</b>\n
 *	@note
 *	@param	lpDirectory [in] �쐬����f�B���N�g��
 *	@return	bool
 */
bool make_folder(LPCTSTR lpDirectory)
{
	if (! lpDirectory || ! lstrlen(lpDirectory))
		return false;

	CString cDirectory = _T("");

	for (INT i = 0; i < lstrlen(lpDirectory); i++) {
		if (cDirectory.GetLength() && cDirectory.GetAt(cDirectory.GetLength()-1) != _T(':') && lpDirectory[i] == _T('\\') && _taccess(cDirectory, 0) != 0) {
			_tmkdir(cDirectory);
		}
		cDirectory += lpDirectory[i];
	}

	if (cDirectory.GetAt(cDirectory.GetLength()-1) != _T(':') &&  _taccess(cDirectory, 0) != 0) {
		_tmkdir(cDirectory);
	}

	return true;
}

void get_out_path(LPCTSTR source, CString &destination, CString &destination_folder)
{
	CString srcpath = source;
	CString subpath = srcpath.Right(srcpath.GetLength() - Srcdir.GetLength());
	CString dstpath = Dstdir;
	if (dstpath[dstpath.GetLength()-1] == _T('\\')) {
		dstpath = dstpath.Left(dstpath.GetLength()-1);
	}
	destination = dstpath + subpath;

	TCHAR title[BUFSIZ*sizeof(TCHAR)+sizeof(TCHAR)];
	GetFileTitle(destination, title, BUFSIZ*sizeof(TCHAR));

	destination_folder = destination.Left(destination.GetLength() - lstrlen(title));
	if (destination_folder[destination_folder.GetLength()-1] == _T('\\')) {
		destination_folder = destination_folder.Left(destination_folder.GetLength()-1);
	}
}

/**
 *	@brief	���E���n�n�ϊ�
 *	@param	filepattern		�t�@�C�����܂��̓p�^�[���̎w��[in]
 *	@param	tokencheck		1���R�[�h�̃J�������̎w��[in]
 *	@param	loncol			�o�x�̃J�����ԍ�(���̎��Ɉܓx���K������O��)[in]
 *	@return	bool			�����Ftrue�A���s�Ffalse
 */
int to_world(LPCTSTR filePattern, int tokencheck, int loncol, int *filenum)
{
	FileFinder finder;
	if (! finder.Search(Srcdir, filePattern, true)) {
		std::cerr << CT2CA(filePattern) << "�̌����Ɏ��s" << std::endl;
		return -1;
	}
	int count = finder.GetCount(), recnum = 0;
	*filenum = count;
	for (int i = 0; i < count; i++) {
		// �o�͐�̃p�X�̐���
		CString dstpath = _T(""), dstdir = _T("");
		get_out_path(finder[i], dstpath, dstdir);
		make_folder(dstdir);

		// �ǂݎ��
		TSVFileReader reader;
		if (reader.read(finder[i]) < 0) {
			std::cerr << CT2CA(finder[i]) << "�̓ǂݎ��Ɏ��s" << std::endl;
			return -1;
		}

		std::cerr << CT2CA(finder[i]) << "��������...";

		// �o�̓t�@�C���̃I�[�v��
		std::ofstream file(dstpath);
		if (! file.is_open()) {
			std::cerr << "�o�̓t�@�C���̃I�[�v���Ɏ��s" << std::endl;
			return -1;
		}

		// �o��
		for (unsigned int n = 0; n < reader.size(); n++) {

			// ���R�[�h�̃g�[�N�����̃`�F�b�N
			if (reader[n].size() != tokencheck) {
				std::cerr << CT2CA(finder[i]) << "�̃t�H�[�}�b�g���s��" << std::endl;
				return -1;
			}

			for (unsigned int m = 0; m < reader[n].size(); m++) {
				if (m != 0) {
					file << "\t";
				}
				if (m != loncol) {
					std::string tmp = CT2CA(reader[n][m]);
					file << tmp.c_str();
				} else {
					double lon = _tcstod(reader[n][m], 0);
					double lat = _tcstod(reader[n][m+1], 0);
					double lon2, lat2, alt2;
					if (To_jgd2000) {
						Crdcnv.TOKYOtoJGD2000_RP(lon, lat, &lon2, &lat2, true);
					} else {
						Crdcnv.TOKYOtoJGD2000(lon, lat, 0.0, &lon2, &lat2, &alt2);
					}
					file.setf(std::ios_base::fixed, std::ios_base::floatfield);
					file.precision(11);
					file << lon2 << "\t" << lat2;
					file.clear();
					m++; // �ܓx���̃J��������ǉ�
				}
			}
			file << "\n";
			recnum++;
		}

		std::cerr << "����I��" << std::endl;
	}
	return recnum;
}

int to_world_zip(int *filenum)
{
		FileFinder finder;
	if (! finder.Search(Srcdir, _T("zip.txt"), true)) {
		std::cerr << "zip.txt�̌����Ɏ��s" << std::endl;
		return -1;
	}
	int count = finder.GetCount(), recnum = 0;
	*filenum = count;
	for (int i = 0; i < count; i++) {
		// �o�͐�̃p�X�̐���
		CString dstpath = _T(""), dstdir = _T("");
		get_out_path(finder[i], dstpath, dstdir);
		make_folder(dstdir);

		// �ǂݎ��
		TSVFileReader reader;
		if (reader.read(finder[i]) < 0) {
			std::cerr << CT2CA(finder[i]) << "�̓ǂݎ��Ɏ��s" << std::endl;
			return -1;
		}

		// �o�̓t�@�C���̃I�[�v��
		std::ofstream file(dstpath);
		if (! file.is_open()) {
			std::cerr << "�o�̓t�@�C���̃I�[�v���Ɏ��s" << std::endl;
			return -1;
		}

		std::cerr << CT2CA(finder[i]) << "��������...";

		// �o��
		for (unsigned int n = 0; n < reader.size(); n++) {

			// ���R�[�h�̃g�[�N�����̃`�F�b�N
			if (reader[n].size() != 13) {
				std::cerr << CT2CA(finder[i]) << "�̃t�H�[�}�b�g���s��" << std::endl;
				return -1;
			}

			int mesh = _ttoi(reader[n][0]); // ���b�V���R�[�h
			int x = _ttoi(reader[n][1]); // X���W
			int y = _ttoi(reader[n][2]); // Y���W
			// �ܓx�o�x�ɕϊ�
			double lon, lat;
			Crdcnv.MeshtoLL(mesh, x, y, &lon, &lat);
			double lon2, lat2, alt2;
			if (To_jgd2000) {
				Crdcnv.TOKYOtoJGD2000_RP(lon, lat, &lon2, &lat2, true);
			} else {
				Crdcnv.TOKYOtoJGD2000(lon, lat, 0.0, &lon2, &lat2, &alt2);
			}
			file.setf(std::ios_base::fixed, std::ios_base::floatfield);
			file.precision(11);
			file << lon2 << "\t" << lat2;
			file.clear();

			for (unsigned int m = 3; m < reader[n].size(); m++) {
				file << "\t";
				std::string tmp = CT2CA(reader[n][m]);
				file << tmp.c_str();
			}

			file << "\n";
			recnum++;
		}

		std::cerr << "����I��" << std::endl;
	}
	return recnum;
}

bool run()
{
	// ���̏o��
	std::cerr << "----------���͏��----------" << std::endl;
	std::cerr << "�@���̓t�H���_�F" << CT2CA(Srcdir) << std::endl;
	std::cerr << "�@�o�̓t�H���_�F" << CT2CA(Dstdir) << std::endl;
	std::cerr << "�@���W�ϊ������F";
	if (To_jgd2000) {
		std::cerr << "JGD2000" << std::endl;
	} else {
		std::cerr << "3�p�����[�^" << std::endl;
	}
	std::cerr << "----------------------------" << std::endl;

	// �o�̓t�H���_�̍쐬
	make_folder(Dstdir);

	CString src = Srcdir; src.MakeLower();
	CString dst = Srcdir.Right(Srcdir.GetLength() - src.GetLength());

	int recnum[10] = {0, }, filenum[10] = {0, }, n = 0;

	// �Z���n
	for (int i = 0; AddrFileInfo[i].m_fileName != NULL; i++, n++) {
		if ((recnum[n] = to_world(AddrFileInfo[i].m_fileName, AddrFileInfo[i].m_columnCount, AddrFileInfo[i].m_lonCol, &filenum[n])) < 0)
			return false;
	}

	// �X�֔ԍ�
	if ((recnum[n] = to_world_zip(&filenum[n])) < 0)
		return false;
	n++;

	// ���ʂ̏o��
	std::cerr << "----------�o�͌���----------" << std::endl;
	std::cerr << "�@���������t�@�C�����F" << std::endl;
	for (int i = 0; i < (n-1); i++) {
		std::cerr << "�@�@" << CT2CA(AddrFileInfo[i].m_fileName) << "�F" << filenum[i] << std::endl;
	}
	std::cerr << "�@�@zip.txt�F" << filenum[n-1] << std::endl;
	std::cerr << "�@�����������R�[�h���F" << std::endl;
	for (int i = 0; i < (n-1); i++) {
		std::cerr << "�@�@" << CT2CA(AddrFileInfo[i].m_fileName) << "�F" << recnum[i] << std::endl;
	}
	std::cerr << "�@�@zip.txt�F" << recnum[n-1] << std::endl;
	std::cerr << "----------------------------" << std::endl;

	return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
	clock_t start_time = clock();

	int retval = 0;
	try {
		if (! arg_check(argc, argv)) {
			retval = -1;
		} else if (! run()) {
			retval = 1;
		} else {
			retval = 0;
		}
	} catch (...) {
		retval = 2;
	}
	switch (retval) {
	case 0: std::cerr << "����I�����܂���" << std::endl; break;
	case 1: std::cerr << "�ُ�I�����܂���" << std::endl; break;
	case 2: std::cerr << "��O���������܂���" << std::endl; break;
	default: break;
	}
	clock_t end_time = clock();
	double proc_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
	std::cerr << "���s���ԁF" << proc_time << "�b" << std::endl;
	return retval;
}

