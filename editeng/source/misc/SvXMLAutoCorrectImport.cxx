/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of the LibreOffice project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This file incorporates work covered by the following license notice:
 *
 *   Licensed to the Apache Software Foundation (ASF) under one or more
 *   contributor license agreements. See the NOTICE file distributed
 *   with this work for additional information regarding copyright
 *   ownership. The ASF licenses this file to you under the Apache
 *   License, Version 2.0 (the "License"); you may not use this file
 *   except in compliance with the License. You may obtain a copy of
 *   the License at http://www.apache.org/licenses/LICENSE-2.0 .
 */

#include <SvXMLAutoCorrectImport.hxx>
#include <SvXMLAutoCorrectTokenHandler.hxx>

using namespace ::css;
using namespace ::css::xml::sax;

SvXMLAutoCorrectImport::SvXMLAutoCorrectImport(
    const uno::Reference< uno::XComponentContext > xContext,
    SvxAutocorrWordList *pNewAutocorr_List,
    SvxAutoCorrect &rNewAutoCorrect,
    const com::sun::star::uno::Reference < com::sun::star::embed::XStorage >& rNewStorage)
:   SvXMLImport( xContext, "" ),
    pAutocorr_List (pNewAutocorr_List),
    rAutoCorrect ( rNewAutoCorrect ),
    xStorage ( rNewStorage )
{
}

SvXMLAutoCorrectImport::~SvXMLAutoCorrectImport ( void ) throw ()
{
}

SvXMLImportContext *SvXMLAutoCorrectImport::CreateFastContext( sal_Int32 Element,
        const uno::Reference< xml::sax::XFastAttributeList > & xAttrList )
{
    if( Element == SvXMLAutoCorrectToken::BLOCKLIST )
        return new SvXMLWordListContext( *this, Element, xAttrList );
    else
        return SvXMLImport::CreateFastContext( Element, xAttrList );
}

SvXMLWordListContext::SvXMLWordListContext(
   SvXMLAutoCorrectImport& rImport,
   sal_Int32 /*Element*/,
   const com::sun::star::uno::Reference<
   com::sun::star::xml::sax::XFastAttributeList > & /*xAttrList*/ ) :
   SvXMLImportContext ( rImport ),
   rLocalRef(rImport)
{
    rLocalRef.rAutoCorrect.refreshBlockList( rLocalRef.xStorage );
}

com::sun::star::uno::Reference<XFastContextHandler> SAL_CALL SvXMLWordListContext::createFastChildContext(
    sal_Int32 Element, const uno::Reference< xml::sax::XFastAttributeList > & xAttrList )
throw (css::uno::RuntimeException, css::xml::sax::SAXException, std::exception)
{
    if ( Element == SvXMLAutoCorrectToken::BLOCK )
        return new SvXMLWordContext (rLocalRef, Element, xAttrList);
    else
        return new SvXMLImportContext( rLocalRef );
}

SvXMLWordListContext::~SvXMLWordListContext ( void )
{
}

SvXMLWordContext::SvXMLWordContext(
   SvXMLAutoCorrectImport& rImport,
   sal_Int32 /*Element*/,
   const com::sun::star::uno::Reference<
   com::sun::star::xml::sax::XFastAttributeList > & xAttrList ) :
   SvXMLImportContext ( rImport ),
   rLocalRef(rImport)
{
    OUString sWrong, sRight;
    if ( xAttrList.is() && xAttrList->hasAttribute( SvXMLAutoCorrectToken::ABBREVIATED_NAME ) )
        sWrong = xAttrList->getValue( SvXMLAutoCorrectToken::ABBREVIATED_NAME );

    if ( xAttrList.is() && xAttrList->hasAttribute( SvXMLAutoCorrectToken::NAME ) )
        sRight = xAttrList->getValue( SvXMLAutoCorrectToken::NAME );

   if ( sWrong.isEmpty() || sRight.isEmpty())
        return;

    bool bOnlyTxt = sRight != sWrong;
    if( !bOnlyTxt )
    {
        const OUString sLongSave( sRight );
        if( !rLocalRef.rAutoCorrect.GetLongText( sWrong, sRight ) &&
            !sLongSave.isEmpty() )
        {
            sRight = sLongSave;
            bOnlyTxt = true;
        }
    }
    rLocalRef.pAutocorr_List->LoadEntry( sWrong, sRight, bOnlyTxt );
}

SvXMLWordContext::~SvXMLWordContext ( void )
{
}

SvXMLExceptionListImport::SvXMLExceptionListImport(
    const uno::Reference< uno::XComponentContext > xContext,
    SvStringsISortDtor & rNewList )
:   SvXMLImport( xContext, "" ),
    rList (rNewList)
{
}

SvXMLExceptionListImport::~SvXMLExceptionListImport ( void ) throw ()
{
}

SvXMLImportContext *SvXMLExceptionListImport::CreateFastContext(sal_Int32 Element,
    const uno::Reference< xml::sax::XFastAttributeList > & xAttrList )
{
    if( Element == SvXMLAutoCorrectToken::BLOCKLIST )
        return new SvXMLExceptionListContext( *this, Element, xAttrList );
    else
        return SvXMLImport::CreateFastContext( Element, xAttrList );
}

SvXMLExceptionListContext::SvXMLExceptionListContext(
   SvXMLExceptionListImport& rImport,
   sal_Int32 /*Element*/,
   const com::sun::star::uno::Reference<
   com::sun::star::xml::sax::XFastAttributeList > & /* xAttrList */ ) :
   SvXMLImportContext ( rImport ),
   rLocalRef(rImport)
{
}

com::sun::star::uno::Reference<xml::sax::XFastContextHandler> SAL_CALL SvXMLExceptionListContext::createFastChildContext(
    sal_Int32 Element, const uno::Reference< xml::sax::XFastAttributeList > & xAttrList )
    throw (css::uno::RuntimeException, css::xml::sax::SAXException, std::exception)
{
    if ( Element == SvXMLAutoCorrectToken::BLOCK )
        return new SvXMLExceptionContext (rLocalRef, Element, xAttrList);
    else
        return new SvXMLImportContext( rLocalRef );
}

SvXMLExceptionListContext::~SvXMLExceptionListContext ( void )
{
}

SvXMLExceptionContext::SvXMLExceptionContext(
   SvXMLExceptionListImport& rImport,
   sal_Int32 /*Element*/,
   const com::sun::star::uno::Reference<
   com::sun::star::xml::sax::XFastAttributeList > & xAttrList ) :
   SvXMLImportContext ( rImport ),
   rLocalRef(rImport)
{
    OUString sWord;
    if( xAttrList.is() && xAttrList->hasAttribute( SvXMLAutoCorrectToken::ABBREVIATED_NAME ) )
        sWord = xAttrList->getValue( SvXMLAutoCorrectToken::ABBREVIATED_NAME );

    if (sWord.isEmpty())
        return;

    rLocalRef.rList.insert( sWord );
}

SvXMLExceptionContext::~SvXMLExceptionContext ( void )
{
}

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
