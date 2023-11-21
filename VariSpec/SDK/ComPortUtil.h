
#ifndef COMPORT_UTILS_INCLUDED
#define COMPORT_UTILS_INCLUDED

#include "stdafx.h"

int GetComPortList(CString nameList[], DWORD maxNames);
bool FillComComboBox(CComboBox *pcBox, LPCSTR targetString);
#endif
