#include "Drive.h"
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")


Drive::Drive()
{
	m_pszDrive = NULL;
	m_pszVolName = NULL;
	m_pszDisplayName = NULL;
	m_nIconIndex = NULL;
	m_nCount = 0;
}

Drive::~Drive()
{
	for (int i = 0; i < m_nCount; ++i)
	{
		delete[] m_pszDrive[i];
		delete[] m_pszVolName[i];
		delete[] m_pszDisplayName[i];
	}

	delete[] m_pszDrive;
	delete[] m_pszVolName;
	delete[] m_pszDisplayName;
	delete[] m_nIconIndex;
	m_nCount = 0;
}

TCHAR* Drive::GetDriveName(const int &i)
{
	return m_pszDrive[i];
}

TCHAR* Drive::GetDisplayName(const int &i)
{
	return m_pszDisplayName[i];
}

int Drive::GetCount()
{
	return m_nCount;
}

int Drive::GetIconIndex(const int &i)
{
	return m_nIconIndex[i];
}

void Drive::GetSystemDrives()
{
	TCHAR buffer[BUFFER_LEN];  
	int i;

	GetLogicalDriveStrings(BUFFER_LEN, buffer);

	//count number disk
	for(i = 0; !((buffer[i] == 0) && (buffer[i + 1] == 0)); ++i)
		if (buffer[i] == 0)  
			++m_nCount;
	++m_nCount;

	//product memory
	m_pszDrive = new TCHAR*[m_nCount];
	m_pszVolName = new TCHAR*[m_nCount];
	m_pszDisplayName = new TCHAR*[m_nCount];
	m_nIconIndex = new int[m_nCount];

	for (i = 0; i < m_nCount; ++i)
	{
		m_pszDrive[i] = new TCHAR[4];
		m_pszVolName[i] = new TCHAR[30];
		m_pszDisplayName[i] = new TCHAR[35];
	}

	int j, k;
	i = 0;

	
	for (j = 0; j< m_nCount; ++j)
	{
		k = 0;
		while(buffer[i] != 0)
			m_pszDrive[j][k++] = buffer[i++];
		m_pszDrive[j][k] = 0; 
		++i;
	}

	int nType;

	for (i = 0; i < m_nCount; ++i)
	{
		nType = GetDriveType(m_pszDrive[i]);
		StrCpy(buffer, _T(""));

		//is disk or usb
		if ( (nType == DRIVE_FIXED) || ((i > 1) && (nType == DRIVE_REMOVABLE)) )
		{
			if (nType == DRIVE_FIXED)
				m_nIconIndex[i] = IDI_HDD;
			else
				m_nIconIndex[i] = IDI_USB;

			GetVolumeInformation(m_pszDrive[i], buffer, BUFFER_LEN, NULL, NULL, NULL, NULL, 0);
			StrCpy(m_pszVolName[i], buffer);
		}
		else
			if (nType == DRIVE_CDROM)
			{
				GetVolumeInformation(m_pszDrive[i], buffer, BUFFER_LEN, NULL, NULL, NULL, NULL, 0);
				if (wcslen(buffer) == 0)
					StrCpy(m_pszVolName[i], _T("CD Rom"));
				else
					StrCpy(m_pszVolName[i], buffer);
				m_nIconIndex[i] = IDI_CD; 
			}
			else 
				if ( ((i == 0) || (i == 1)) && (nType == DRIVE_REMOVABLE))
				{
					StrCpy(m_pszVolName[i], _T("3½ Floppy"));
					m_nIconIndex[i] = IDI_FLOPPY;
				}

				if (wcslen(m_pszVolName[i]) == 0)
					StrCpy(m_pszDisplayName[i], _T("Local Disk"));
				else
					StrCpy(m_pszDisplayName[i], m_pszVolName[i]);

				StrCat(m_pszDisplayName[i], _T(" ("));
				StrNCat(m_pszDisplayName[i], m_pszDrive[i], 3);
				StrCat(m_pszDisplayName[i], _T(")"));
	}
}


