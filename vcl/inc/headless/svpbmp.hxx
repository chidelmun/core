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

#ifndef INCLUDED_VCL_INC_HEADLESS_SVPBMP_HXX
#define INCLUDED_VCL_INC_HEADLESS_SVPBMP_HXX

#include "sal/config.h"
#include "tools/solar.h"

#include "basebmp/bitmapdevice.hxx"

#include <salbmp.hxx>

#define SVP_DEFAULT_BITMAP_FORMAT basebmp::FORMAT_THIRTYTWO_BIT_TC_MASK_BGRX

class VCL_DLLPUBLIC SvpSalBitmap : public SalBitmap
{
    basebmp::BitmapDeviceSharedPtr     m_aBitmap;
public:
    SvpSalBitmap() {}
    virtual ~SvpSalBitmap();

    const basebmp::BitmapDeviceSharedPtr& getBitmap() const { return m_aBitmap; }
    void setBitmap( const basebmp::BitmapDeviceSharedPtr& rSrc ) { m_aBitmap = rSrc; }

    // SalBitmap
    virtual bool            Create( const Size& rSize,
                                    sal_uInt16 nBitCount,
                                    const BitmapPalette& rPal ) SAL_OVERRIDE;
    virtual bool            Create( const SalBitmap& rSalBmp ) SAL_OVERRIDE;
    virtual bool            Create( const SalBitmap& rSalBmp,
                                    SalGraphics* pGraphics ) SAL_OVERRIDE;
    virtual bool            Create( const SalBitmap& rSalBmp,
                                    sal_uInt16 nNewBitCount ) SAL_OVERRIDE;
    virtual bool            Create( const ::com::sun::star::uno::Reference< ::com::sun::star::rendering::XBitmapCanvas >& rBitmapCanvas,
                                    Size& rSize,
                                    bool bMask = false ) SAL_OVERRIDE;
    virtual void            Destroy() SAL_OVERRIDE;
    virtual Size            GetSize() const SAL_OVERRIDE;
    virtual sal_uInt16      GetBitCount() const SAL_OVERRIDE;

    virtual BitmapBuffer*   AcquireBuffer( BitmapAccessMode nMode ) SAL_OVERRIDE;
    virtual void            ReleaseBuffer( BitmapBuffer* pBuffer, BitmapAccessMode nMode ) SAL_OVERRIDE;
    virtual bool            GetSystemData( BitmapSystemData& rData ) SAL_OVERRIDE;

    virtual bool            Crop( const Rectangle& rRectPixel ) SAL_OVERRIDE;
    virtual bool            Erase( const Color& rFillColor ) SAL_OVERRIDE;
    virtual bool            Scale( const double& rScaleX, const double& rScaleY, sal_uInt32 nScaleFlag ) SAL_OVERRIDE;
    virtual bool            Replace( const Color& rSearchColor, const Color& rReplaceColor, sal_uLong nTol ) SAL_OVERRIDE;

    static sal_uInt32 getBitCountFromScanlineFormat( basebmp::Format nFormat );
};

#endif // INCLUDED_VCL_INC_HEADLESS_SVPBMP_HXX

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
