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

#ifndef INCLUDED_SVX_FLOAT3D_HXX
#define INCLUDED_SVX_FLOAT3D_HXX

#include <sfx2/ctrlitem.hxx>
#include <vcl/fixed.hxx>
#include <vcl/field.hxx>
#include <sfx2/dockwin.hxx>
#include <vcl/button.hxx>
#include <svtools/stdctrl.hxx>
#include <svx/svxdllapi.h>

#include <svx/f3dchild.hxx>
#include <svx/dlgctl3d.hxx>
#include <svx/dlgctrl.hxx>
#include <svx/svdmodel.hxx>

enum ViewType3D
{
    VIEWTYPE_GEO = 1,
    VIEWTYPE_REPRESENTATION,
    VIEWTYPE_LIGHT,
    VIEWTYPE_TEXTURE,
    VIEWTYPE_MATERIAL
};

class SdrModel;
class FmFormModel;
class VirtualDevice;
class E3dView;
class SdrPageView;
class Svx3DCtrlItem;
class SvxConvertTo3DItem;


struct Svx3DWinImpl;

class SVX_DLLPUBLIC Svx3DWin : public SfxDockingWindow
{
    friend class        Svx3DChildWindow;
    friend class        Svx3DCtrlItem;
    using Window::Update;

private:
    VclPtr<ImageButton>         aBtnGeo;
    VclPtr<ImageButton>         aBtnRepresentation;
    VclPtr<ImageButton>         aBtnLight;
    VclPtr<ImageButton>         aBtnTexture;
    VclPtr<ImageButton>         aBtnMaterial;
    VclPtr<ImageButton>         aBtnUpdate;
    VclPtr<ImageButton>         aBtnAssign;

// geometry
    VclPtr<FixedLine>           aFLGeometrie;
    VclPtr<FixedText>           aFtPercentDiagonal;
    VclPtr<MetricField>         aMtrPercentDiagonal;
    VclPtr<FixedText>           aFtBackscale;
    VclPtr<MetricField>         aMtrBackscale;
    VclPtr<FixedText>           aFtEndAngle;
    VclPtr<MetricField>         aMtrEndAngle;
    VclPtr<FixedText>           aFtDepth;
    VclPtr<MetricField>         aMtrDepth;
    VclPtr<FixedLine>           aFLSegments;

    VclPtr<FixedText>           aFtHorizontal;
    VclPtr<NumericField>        aNumHorizontal;
    VclPtr<FixedText>           aFtVertical;
    VclPtr<NumericField>        aNumVertical;
    VclPtr<FixedLine>           aFLNormals;

    VclPtr<ImageButton>         aBtnNormalsObj;
    VclPtr<ImageButton>         aBtnNormalsFlat;
    VclPtr<ImageButton>         aBtnNormalsSphere;
    VclPtr<ImageButton>         aBtnNormalsInvert;
    VclPtr<ImageButton>         aBtnTwoSidedLighting;

    VclPtr<ImageButton>         aBtnDoubleSided;
    VclPtr<FixedLine>           aFLRepresentation;
// presentation
    VclPtr<FixedText>           aFtShademode;
    VclPtr<ListBox>             aLbShademode;
    VclPtr<FixedLine>           aFLShadow;
    VclPtr<ImageButton>         aBtnShadow3d;
    VclPtr<FixedText>           aFtSlant;
    VclPtr<MetricField>         aMtrSlant;
    VclPtr<FixedText>           aFtDistance;
    VclPtr<MetricField>         aMtrDistance;
    VclPtr<FixedText>           aFtFocalLeng;
    VclPtr<MetricField>         aMtrFocalLength;
    VclPtr<FixedLine>           aFLCamera;
    VclPtr<FixedLine>           aFLLight;

// lighting
    VclPtr<ImageButton>         aBtnLight1;
    VclPtr<ImageButton>         aBtnLight2;
    VclPtr<ImageButton>         aBtnLight3;
    VclPtr<ImageButton>         aBtnLight4;
    VclPtr<ImageButton>         aBtnLight5;
    VclPtr<ImageButton>         aBtnLight6;
    VclPtr<ImageButton>         aBtnLight7;
    VclPtr<ImageButton>         aBtnLight8;
    VclPtr<FixedText>           aFTLightsource;
    VclPtr<ColorLB>             aLbLight1;
    VclPtr<ColorLB>             aLbLight2;
    VclPtr<ColorLB>             aLbLight3;
    VclPtr<ColorLB>             aLbLight4;
    VclPtr<ColorLB>             aLbLight5;
    VclPtr<ColorLB>             aLbLight6;
    VclPtr<ColorLB>             aLbLight7;
    VclPtr<ColorLB>             aLbLight8;

    VclPtr<ImageButton>         aBtnLightColor;

    // #99694# Keyboard shortcuts activate the next control, so the
    // order needed to be changed here
    VclPtr<FixedText>           aFTAmbientlight;    // Text label
    VclPtr<ColorLB>             aLbAmbientlight;    // ListBox
    VclPtr<ImageButton>         aBtnAmbientColor;   // color button

    VclPtr<FixedLine>           aFLTexture;

// Textures
    VclPtr<FixedText>           aFtTexKind;
    VclPtr<ImageButton>         aBtnTexLuminance;
    VclPtr<ImageButton>         aBtnTexColor;
    VclPtr<FixedText>           aFtTexMode;
    VclPtr<ImageButton>         aBtnTexReplace;
    VclPtr<ImageButton>         aBtnTexModulate;
    VclPtr<ImageButton>         aBtnTexBlend;
    VclPtr<FixedText>           aFtTexProjectionX;
    VclPtr<ImageButton>         aBtnTexObjectX;
    VclPtr<ImageButton>         aBtnTexParallelX;
    VclPtr<ImageButton>         aBtnTexCircleX;
    VclPtr<FixedText>           aFtTexProjectionY;
    VclPtr<ImageButton>         aBtnTexObjectY;
    VclPtr<ImageButton>         aBtnTexParallelY;
    VclPtr<ImageButton>         aBtnTexCircleY;
    VclPtr<FixedText>           aFtTexFilter;
    VclPtr<ImageButton>         aBtnTexFilter;

// material
// material editor
    VclPtr<FixedLine>           aFLMaterial;
    VclPtr<FixedText>           aFtMatFavorites;
    VclPtr<ListBox>             aLbMatFavorites;
    VclPtr<FixedText>           aFtMatColor;
    VclPtr<ColorLB>             aLbMatColor;
    VclPtr<ImageButton>         aBtnMatColor;
    VclPtr<FixedText>           aFtMatEmission;
    VclPtr<ColorLB>             aLbMatEmission;
    VclPtr<ImageButton>         aBtnEmissionColor;
    VclPtr<FixedLine>           aFLMatSpecular;
    VclPtr<FixedText>           aFtMatSpecular;
    VclPtr<ColorLB>             aLbMatSpecular;
    VclPtr<ImageButton>         aBtnSpecularColor;
    VclPtr<FixedText>           aFtMatSpecularIntensity;
    VclPtr<MetricField>         aMtrMatSpecularIntensity;

    VclPtr<Svx3DPreviewControl> aCtlPreview;
    VclPtr<SvxLightCtl3D>       aCtlLightPreview;

// bottom part
    VclPtr<ImageButton>         aBtnConvertTo3D;
    VclPtr<ImageButton>         aBtnLatheObject;
    VclPtr<ImageButton>         aBtnPerspective;

// the rest ...
    Image               aImgLightOn;
    Image               aImgLightOff;
    bool                bUpdate;
    ViewType3D          eViewType;
    Size                aSize;

    // Model, Page, View etc. for favourites
    FmFormModel*        pModel;
    VclPtr<VirtualDevice> pVDev;
    E3dView*            p3DView;

    SfxBindings*                pBindings;
    Svx3DCtrlItem*              pControllerItem;

    SvxConvertTo3DItem*         pConvertTo3DItem;
    SvxConvertTo3DItem*         pConvertTo3DLatheItem;

    Svx3DWinImpl*       mpImpl;
    SfxMapUnit          ePoolUnit;
    FieldUnit           eFUnit;

    // ItemSet used to remember set 2d attributes
    SfxItemSet*         mpRemember2DAttributes;

    bool                bOnly3DChanged;



    DECL_LINK( ClickViewTypeHdl, void * );
    DECL_LINK( ClickUpdateHdl, void * );
    DECL_LINK( ClickAssignHdl, void * );
    DECL_LINK( ClickHdl, PushButton * );
    DECL_LINK( ClickColorHdl, PushButton * );
    DECL_LINK( SelectHdl, void * );
    DECL_LINK( ModifyHdl, void * );
    void ClickLight(PushButton &rBtn);

    DECL_LINK( ChangeLightCallbackHdl, void * );
    DECL_LINK( ChangeSelectionCallbackHdl, void * );

    SVX_DLLPRIVATE void         Construct();
    SVX_DLLPRIVATE void         Reset();

    SVX_DLLPRIVATE bool         LBSelectColor( ColorLB* pLb, const Color& rColor );
    SVX_DLLPRIVATE sal_uInt16   GetLightSource( const PushButton* pBtn = NULL );
    SVX_DLLPRIVATE ColorLB*     GetLbByButton( const PushButton* pBtn = NULL );

    SVX_DLLPRIVATE bool         GetUILightState( const ImageButton& rBtn ) const;
    SVX_DLLPRIVATE void         SetUILightState( ImageButton& aBtn, bool bState );

protected:
    virtual void    Resize() SAL_OVERRIDE;

public:
            Svx3DWin( SfxBindings* pBindings, SfxChildWindow *pCW,
                        vcl::Window* pParent );
            virtual ~Svx3DWin();
    virtual void dispose() SAL_OVERRIDE;

    void    InitColorLB( const SdrModel* pDoc );
    bool    IsUpdateMode() const { return bUpdate; }

    void    Update( SfxItemSet& rSet );
    void    GetAttr( SfxItemSet& rSet );

    void UpdatePreview(); // nach oben (private)
    void DocumentReload(); // #83951#
};

/*************************************************************************
|*
|* Controller item for 3D Window (Floating/Docking)
|*
\************************************************************************/

class Svx3DCtrlItem : public SfxControllerItem
{
 protected:
    virtual void StateChanged( sal_uInt16 nSId, SfxItemState eState,
                                const SfxPoolItem* pState ) SAL_OVERRIDE;

 public:
    Svx3DCtrlItem( sal_uInt16, SfxBindings* );
};

/*************************************************************************
|*
|* ControllerItem for State of a Slot
|* (SID_CONVERT_TO_3D, SID_CONVERT_TO_3D_LATHE_FAST)
|*
\************************************************************************/

class SvxConvertTo3DItem : public SfxControllerItem
{
    bool                        bState;

protected:
    virtual void StateChanged(sal_uInt16 nSId, SfxItemState eState, const SfxPoolItem* pState) SAL_OVERRIDE;

public:
    SvxConvertTo3DItem(sal_uInt16 nId, SfxBindings* pBindings);
    bool GetState() const { return bState; }
};

#endif // INCLUDED_SVX_FLOAT3D_HXX

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
