#include "rexp.h"

void main(void)
{
	uchar *a = re_comp((uchar *)"[$%#>]");	
	int b = re_exec((uchar *)"#");
	
	a = re_comp((uchar*)"^[a-zA-Z_]+\\.[a-zA-Z_]+[0-9][0-9][0-9][0-9]/[0-9][0-9]/[0-9][0-9] [0-9][0-9]:[0-9][0-9]:[0-9][0-9]$");
	b = re_exec((uchar *)" sindychk.furukawa2004/08/08 22:22:22");

	a = re_comp((uchar*)".+\\\\[1-9][0-9][0-9][0-9]\\\\[1-9][0-9][0-9][0-9][0-9][0-9]\\.[mM][dD][bB]$");
	b = re_exec((uchar *)"C:\\Program Files\\Home\\furu2\\PGDB\\5339\\533935.mdb");
}