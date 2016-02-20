// Xmlvalidation.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#import <msxml6.dll>
#include <string>
using namespace std;
// Macro that calls a COM method returning HRESULT value.
#define CHK_HR(stmt)        do { hr=(stmt); if (FAILED(hr)) goto CleanUp; } while(0)

void dump_com_error(_com_error &e)
{
    _bstr_t bstrSource(e.Source());
    _bstr_t bstrDescription(e.Description());

    printf("Error\n");
    printf("\a\tCode = %08lx\n", e.Error());
    printf("\a\tCode meaning = %s", e.ErrorMessage());
    printf("\a\tSource = %s\n", (LPCSTR) bstrSource);
    printf("\a\tDescription = %s\n", (LPCSTR) bstrDescription);
}

_bstr_t validateFile(_bstr_t bstrFile)
{
    // Initialize objects and variables.
    MSXML2::IXMLDOMDocument2Ptr pXMLDoc;
    MSXML2::IXMLDOMParseErrorPtr pError;
    _bstr_t bstrResult = L"";
    HRESULT hr = S_OK;

    // Create a DOMDocument and set its properties.
    CHK_HR(pXMLDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60), NULL, CLSCTX_INPROC_SERVER));

    pXMLDoc->async = VARIANT_FALSE;
    pXMLDoc->validateOnParse = VARIANT_TRUE;
    pXMLDoc->resolveExternals = VARIANT_TRUE;

    // Load and validate the specified file into the DOM.
    // And return validation results in message to the user.
    if(pXMLDoc->load(bstrFile) != VARIANT_TRUE)
    {
        pError = pXMLDoc->parseError;

        bstrResult = _bstr_t(L"Validation failed on ") + bstrFile +
            _bstr_t(L"\n=====================") +
            _bstr_t(L"\nReason: ") + _bstr_t(pError->Getreason()) +
            _bstr_t(L"\nSource: ") + _bstr_t(pError->GetsrcText()) +
            _bstr_t(L"\nLine: ") + _bstr_t(pError->Getline()) +
            _bstr_t(L"\n");
    }
    else
    {
        bstrResult = _bstr_t(L"Validation succeeded for ") + bstrFile +
        _bstr_t(L"\n======================\n") +
        _bstr_t(pXMLDoc->xml) + _bstr_t(L"\n");
    }

CleanUp:
    return bstrResult;
}

int _tmain(int argc, _TCHAR* argv[])
{
    HRESULT hr = CoInitialize(NULL);
    if(SUCCEEDED(hr))
    {
        try
        {
            _bstr_t bstrOutput = validateFile(L"nn-valid.xml");
            bstrOutput += validateFile(L"nn-notValid.xml");
            MessageBoxW(NULL, bstrOutput, L"noNamespace",MB_OK);
        }
        catch(_com_error &e)
        {
            dump_com_error(e);
        }
        CoUninitialize();
    }

    return 0;

}
