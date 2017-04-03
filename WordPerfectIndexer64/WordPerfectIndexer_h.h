

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0620 */
/* at Mon Jan 18 22:14:07 2038
 */
/* Compiler settings for ..\WordPerfectIndexer\WordPerfectIndexer.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.01.0620 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __WordPerfectIndexer_h_h__
#define __WordPerfectIndexer_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IWordPerfectFilter_FWD_DEFINED__
#define __IWordPerfectFilter_FWD_DEFINED__
typedef interface IWordPerfectFilter IWordPerfectFilter;

#endif 	/* __IWordPerfectFilter_FWD_DEFINED__ */


#ifndef __WordPerfectFilter_FWD_DEFINED__
#define __WordPerfectFilter_FWD_DEFINED__

#ifdef __cplusplus
typedef class WordPerfectFilter WordPerfectFilter;
#else
typedef struct WordPerfectFilter WordPerfectFilter;
#endif /* __cplusplus */

#endif 	/* __WordPerfectFilter_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "shobjidl.h"
#include "thumbcache.h"
#include "filter.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IWordPerfectFilter_INTERFACE_DEFINED__
#define __IWordPerfectFilter_INTERFACE_DEFINED__

/* interface IWordPerfectFilter */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IWordPerfectFilter;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("2E19B65E-2A08-4CF2-95CD-F84F4AEB6EB4")
    IWordPerfectFilter : public IUnknown
    {
    public:
    };
    
    
#else 	/* C style interface */

    typedef struct IWordPerfectFilterVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWordPerfectFilter * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWordPerfectFilter * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWordPerfectFilter * This);
        
        END_INTERFACE
    } IWordPerfectFilterVtbl;

    interface IWordPerfectFilter
    {
        CONST_VTBL struct IWordPerfectFilterVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWordPerfectFilter_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWordPerfectFilter_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWordPerfectFilter_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWordPerfectFilter_INTERFACE_DEFINED__ */



#ifndef __WordPerfectIndexerLib_LIBRARY_DEFINED__
#define __WordPerfectIndexerLib_LIBRARY_DEFINED__

/* library WordPerfectIndexerLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_WordPerfectIndexerLib;

EXTERN_C const CLSID CLSID_WordPerfectFilter;

#ifdef __cplusplus

class DECLSPEC_UUID("F4EF307D-BCFC-4DC5-821D-31B77E0A5011")
WordPerfectFilter;
#endif
#endif /* __WordPerfectIndexerLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


