/*
 * An example of file signature verification using WinTrust API
 * Derived from the sample vertrust.cpp in the Platform SDK
 *
 * Copyright (c) 2009 Mounir IDRASSI <mounir.idrassi@idrix.fr>. All rights reserved.
 *
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 * or FITNESS FOR A PARTICULAR PURPOSE.
 * 
 */

#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#define _WIN32_WINNT 0x0500
#define WINVER       0x0500

#include <windows.h>
#include <Softpub.h>
#include <Wincrypt.h>
#include <tchar.h>
#include <stdlib.h>

#pragma comment(lib, "Crypt32.lib")
#pragma comment(lib, "Wintrust.lib")

// values returned by the program to indicate the status of the verification
#define VERIFICATION_STATUS_OK					0
#define VERIFICATION_STATUS_INVALID_SIGNATURE	1
#define VERIFICATION_STATUS_NO_SIGNATURE		2
#define VERIFICATION_STATUS_FILE_ACCESS_ERROR	3
#define VERIFICATION_STATUS_NO_PROVIDER			4
#define VERIFICATION_STATUS_UNKNOWN_ERROR		5

LPTSTR GetCertificateDescription(PCCERT_CONTEXT pCertCtx)
{
   DWORD dwStrType;
   DWORD dwCount;
   LPTSTR szSubjectRDN = NULL;

   dwStrType = CERT_X500_NAME_STR;
   dwCount = CertGetNameString(pCertCtx,
      CERT_NAME_RDN_TYPE,
      0,
      &dwStrType,
      NULL,
      0);
   if (dwCount)
   {
      szSubjectRDN = (LPTSTR) LocalAlloc(0, dwCount * sizeof(TCHAR));
      CertGetNameString(pCertCtx,
         CERT_NAME_RDN_TYPE,
         0,
         &dwStrType,
         szSubjectRDN,
         dwCount);
   }

   return szSubjectRDN;
}


int _tmain(int argc, _TCHAR* argv[])
{
   GUID guidAction = WINTRUST_ACTION_GENERIC_VERIFY_V2;
   WINTRUST_FILE_INFO sWintrustFileInfo;
   WINTRUST_DATA      sWintrustData;
   HRESULT            hr;
   int				  status = VERIFICATION_STATUS_UNKNOWN_ERROR;

   if (argc != 2)
   {
      _tprintf(_T("Usage: VerifyExeSignature file_name\n"));
      return -1;
   }

   memset((void*)&sWintrustFileInfo, 0x00, sizeof(WINTRUST_FILE_INFO));
   memset((void*)&sWintrustData, 0x00, sizeof(WINTRUST_DATA));

   sWintrustFileInfo.cbStruct = sizeof(WINTRUST_FILE_INFO);
   sWintrustFileInfo.pcwszFilePath = argv[1];
   sWintrustFileInfo.hFile = NULL;

   sWintrustData.cbStruct            = sizeof(WINTRUST_DATA);
   sWintrustData.dwUIChoice          = WTD_UI_NONE;
   sWintrustData.fdwRevocationChecks = WTD_REVOKE_NONE;
   sWintrustData.dwUnionChoice       = WTD_CHOICE_FILE;
   sWintrustData.pFile               = &sWintrustFileInfo;
   sWintrustData.dwStateAction       = WTD_STATEACTION_VERIFY;

   hr = WinVerifyTrust((HWND)INVALID_HANDLE_VALUE, &guidAction, &sWintrustData);

   if (TRUST_E_NOSIGNATURE == hr)
   {
      _tprintf(_T("No signature found on the file.\n"));
      status = VERIFICATION_STATUS_NO_SIGNATURE;
   }
   else if (CRYPT_E_FILE_ERROR == hr)
   {
	   _tprintf(_T("An error occurred while reading the file. Check that the file exists and that it's not locked by another process.\n"));
      status = VERIFICATION_STATUS_FILE_ACCESS_ERROR;
   }
   else if (TRUST_E_BAD_DIGEST == hr)
   {
      _tprintf(_T("The signature of the file is invalid\n"));
      status = VERIFICATION_STATUS_INVALID_SIGNATURE;
   }
   else if (TRUST_E_PROVIDER_UNKNOWN == hr)
   {
      _tprintf(_T("No trust provider on this machine can verify this type of files.\n"));
      status = VERIFICATION_STATUS_NO_PROVIDER;
   }
   else if (S_OK != hr)
   {
      _tprintf(_T("WinVerifyTrust failed with error 0x%.8X\n"), hr);
      status = VERIFICATION_STATUS_UNKNOWN_ERROR;
   }
   else
   {
      _tprintf(_T("File signature is OK.\n"));

      status = VERIFICATION_STATUS_OK;

      // retreive the signer certificate and display its information
      CRYPT_PROVIDER_DATA const *psProvData     = NULL;
      CRYPT_PROVIDER_SGNR       *psProvSigner   = NULL;
      CRYPT_PROVIDER_CERT       *psProvCert     = NULL;
      FILETIME                   localFt;
      SYSTEMTIME                 sysTime;

      psProvData = WTHelperProvDataFromStateData(sWintrustData.hWVTStateData);
      if (psProvData)
      {
         psProvSigner = WTHelperGetProvSignerFromChain((PCRYPT_PROVIDER_DATA)psProvData, 0 , FALSE, 0);
         if (psProvSigner)
         {
            FileTimeToLocalFileTime(&psProvSigner->sftVerifyAsOf, &localFt);
            FileTimeToSystemTime(&localFt, &sysTime);

            _tprintf(_T("Signature Date = %.2d/%.2d/%.4d at %.2d:%2.d:%.2d\n"), sysTime.wDay, sysTime.wMonth,sysTime.wYear, sysTime.wHour,sysTime.wMinute,sysTime.wSecond);

            psProvCert = WTHelperGetProvCertFromChain(psProvSigner, 0);
            if (psProvCert)
            {
               LPTSTR szCertDesc = GetCertificateDescription(psProvCert->pCert);
               if (szCertDesc)
               {
                  _tprintf(_T("File Signer = %s\n"), szCertDesc);
                  LocalFree(szCertDesc);
               }
            }

            if (psProvSigner->csCounterSigners)
            {
               _tprintf(_T("\n"));
               // Timestamp information
               FileTimeToLocalFileTime(&psProvSigner->pasCounterSigners[0].sftVerifyAsOf, &localFt);
               FileTimeToSystemTime(&localFt, &sysTime);

               _tprintf(_T("Timestamp Date = %.2d/%.2d/%.4d at %.2d:%2.d:%.2d\n"), sysTime.wDay, sysTime.wMonth,sysTime.wYear, sysTime.wHour,sysTime.wMinute,sysTime.wSecond);               
               psProvCert = WTHelperGetProvCertFromChain(&psProvSigner->pasCounterSigners[0], 0);
               if (psProvCert)
               {
                  LPTSTR szCertDesc = GetCertificateDescription(psProvCert->pCert);
                  if (szCertDesc)
                  {
                     _tprintf(_T("Timestamp Signer = %s\n"), szCertDesc);
                     LocalFree(szCertDesc);
                  }
               }
            }
         }
      }
   }
   
   sWintrustData.dwUIChoice = WTD_UI_NONE;
   sWintrustData.dwStateAction = WTD_STATEACTION_CLOSE;
   WinVerifyTrust((HWND)INVALID_HANDLE_VALUE, &guidAction, &sWintrustData);

   return status;
}

