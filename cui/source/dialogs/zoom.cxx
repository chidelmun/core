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

#include <svl/itemset.hxx>
#include <svl/itempool.hxx>
#include <sfx2/objsh.hxx>
#include <vcl/layout.hxx>
#include <vcl/msgbox.hxx>

#include <cuires.hrc>

#include "zoom.hxx"
#include <sfx2/zoomitem.hxx>
#include <svx/viewlayoutitem.hxx>
#include <dialmgr.hxx>
#include <svx/zoom_def.hxx>

namespace
{

const sal_uInt16 SPECIAL_FACTOR = 0xFFFF;

} // anonymous namespace

sal_uInt16 SvxZoomDialog::GetFactor() const
{
    if (m_p100Btn->IsChecked())
        return 100;

    if (m_pUserBtn->IsChecked())
        return static_cast<sal_uInt16>(m_pUserEdit->GetValue());
    else
        return SPECIAL_FACTOR;
}

void SvxZoomDialog::SetFactor(sal_uInt16 nNewFactor, sal_uInt16 nButtonId)
{
    m_pUserEdit->Disable();

    if (!nButtonId)
    {
        if ( nNewFactor == 100 )
        {
            m_p100Btn->Check();
            m_p100Btn->GrabFocus();
        }
        else
        {
            m_pUserBtn->Check();
            m_pUserEdit->Enable();
            m_pUserEdit->SetValue(static_cast<long>(nNewFactor));
            m_pUserEdit->GrabFocus();
        }
    }
    else
    {
        m_pUserEdit->SetValue(static_cast<long>(nNewFactor));
        switch(nButtonId)
        {
            case ZOOMBTN_OPTIMAL:
            {
                m_pOptimalBtn->Check();
                m_pOptimalBtn->GrabFocus();
                break;
            }
            case ZOOMBTN_PAGEWIDTH:
            {
                m_pPageWidthBtn->Check();
                m_pPageWidthBtn->GrabFocus();
                break;
            }
            case ZOOMBTN_WHOLEPAGE:
            {
                m_pWholePageBtn->Check();
                m_pWholePageBtn->GrabFocus();
                break;
            }
        }
    }
}

void SvxZoomDialog::HideButton(sal_uInt16 nButtonId)
{
    switch (nButtonId)
    {
        case ZOOMBTN_OPTIMAL:
            m_pOptimalBtn->Hide();
            break;

        case ZOOMBTN_PAGEWIDTH:
            m_pPageWidthBtn->Hide();
            break;

        case ZOOMBTN_WHOLEPAGE:
            m_pWholePageBtn->Hide();
            break;

        default:
            OSL_FAIL("Wrong button number!" );
    }
}

void SvxZoomDialog::SetLimits(sal_uInt16 nMin, sal_uInt16 nMax)
{
    DBG_ASSERT(nMin < nMax, "invalid limits");
    m_pUserEdit->SetMin(nMin);
    m_pUserEdit->SetFirst(nMin);
    m_pUserEdit->SetMax(nMax);
    m_pUserEdit->SetLast(nMax);
}

const SfxItemSet* SvxZoomDialog::GetOutputItemSet() const
{
    return mpOutSet.get();
}

SvxZoomDialog::SvxZoomDialog( vcl::Window* pParent, const SfxItemSet& rCoreSet )
    : SfxModalDialog(pParent, "ZoomDialog", "cui/ui/zoomdialog.ui")
    , mrSet(rCoreSet)
    , mpOutSet()
    , mbModified(false)

{
    get(m_pOptimalBtn, "optimal");
    get(m_pWholePageBtn, "fitwandh");
    get(m_pPageWidthBtn, "fitw");
    get(m_p100Btn, "100pc");
    get(m_pUserBtn, "variable");
    get(m_pUserEdit, "zoomsb");
    get(m_pViewFrame, "viewframe");
    get(m_pAutomaticBtn, "automatic");
    get(m_pSingleBtn, "singlepage");
    get(m_pColumnsBtn, "columns");
    get(m_pColumnsEdit, "columnssb");
    get(m_pBookModeChk, "bookmode");
    get(m_pOKBtn, "ok");
    Link aLink = LINK(this, SvxZoomDialog, UserHdl);
    m_p100Btn->SetClickHdl(aLink);
    m_pOptimalBtn->SetClickHdl(aLink);
    m_pPageWidthBtn->SetClickHdl(aLink);
    m_pWholePageBtn->SetClickHdl(aLink);
    m_pUserBtn->SetClickHdl(aLink);

    Link aViewLayoutLink = LINK(this, SvxZoomDialog, ViewLayoutUserHdl);
    m_pAutomaticBtn->SetClickHdl(aViewLayoutLink);
    m_pSingleBtn->SetClickHdl(aViewLayoutLink);
    m_pColumnsBtn->SetClickHdl(aViewLayoutLink);

    Link aViewLayoutSpinLink = LINK(this, SvxZoomDialog, ViewLayoutSpinHdl);
    m_pColumnsEdit->SetModifyHdl(aViewLayoutSpinLink);

    Link aViewLayoutCheckLink = LINK(this, SvxZoomDialog, ViewLayoutCheckHdl);
    m_pBookModeChk->SetClickHdl(aViewLayoutCheckLink);

    m_pOKBtn->SetClickHdl(LINK(this, SvxZoomDialog, OKHdl));
    m_pUserEdit->SetModifyHdl(LINK(this, SvxZoomDialog, SpinHdl));

    // default values
    sal_uInt16 nValue = 100;
    sal_uInt16 nMin = 10;
    sal_uInt16 nMax = 1000;

    // maybe get the old value first
    const SfxUInt16Item* pOldUserItem = 0;
    SfxObjectShell* pShell = SfxObjectShell::Current();

    if (pShell)
        pOldUserItem = static_cast<const SfxUInt16Item*>(pShell->GetItem(SID_ATTR_ZOOM_USER));

    if (pOldUserItem)
        nValue = pOldUserItem->GetValue();

    // initialize UserEdit
    if (nMin > nValue)
        nMin = nValue;
    if (nMax < nValue)
        nMax = nValue;

    SetLimits(nMin, nMax);
    m_pUserEdit->SetValue(nValue);

    m_pUserEdit->SetAccessibleName(m_pUserBtn->GetText());
    m_pColumnsEdit->SetAccessibleName(m_pColumnsBtn->GetText());
    m_pColumnsEdit->SetAccessibleRelationMemberOf(m_pColumnsBtn);
    m_pBookModeChk->SetAccessibleRelationMemberOf(m_pColumnsBtn);

    const SfxPoolItem& rItem = mrSet.Get(mrSet.GetPool()->GetWhich(SID_ATTR_ZOOM));

    if (rItem.ISA(SvxZoomItem))
    {
        const SvxZoomItem& rZoomItem = static_cast<const SvxZoomItem&>(rItem);
        const sal_uInt16 nZoom = rZoomItem.GetValue();
        const SvxZoomType eType = rZoomItem.GetType();
        const SvxZoomEnableFlags nValSet = rZoomItem.GetValueSet();
        sal_uInt16 nButtonId = 0;

        switch (eType)
        {
            case SvxZoomType::OPTIMAL:
                nButtonId = ZOOMBTN_OPTIMAL;
                break;
            case SvxZoomType::PAGEWIDTH:
                nButtonId = ZOOMBTN_PAGEWIDTH;
                break;
            case SvxZoomType::WHOLEPAGE:
                nButtonId = ZOOMBTN_WHOLEPAGE;
                break;
            case SvxZoomType::PERCENT:
                break;
            case SvxZoomType::PAGEWIDTH_NOBORDER:
                break;
        }

        if (!(SvxZoomEnableFlags::N100 & nValSet))
            m_p100Btn->Disable();
        if (!(SvxZoomEnableFlags::OPTIMAL & nValSet))
            m_pOptimalBtn->Disable();
        if (!(SvxZoomEnableFlags::PAGEWIDTH & nValSet))
            m_pPageWidthBtn->Disable();
        if (!(SvxZoomEnableFlags::WHOLEPAGE & nValSet))
            m_pWholePageBtn->Disable();

        SetFactor(nZoom, nButtonId);
    }
    else
    {
        const sal_uInt16 nZoom = static_cast<const SfxUInt16Item&>(rItem).GetValue();
        SetFactor(nZoom);
    }

    const SfxPoolItem* pPoolViewLayoutItem = NULL;
    if (SfxItemState::SET == mrSet.GetItemState(SID_ATTR_VIEWLAYOUT, false, &pPoolViewLayoutItem))
    {
        const SvxViewLayoutItem* pViewLayoutItem = static_cast<const SvxViewLayoutItem*>(pPoolViewLayoutItem);
        const sal_uInt16 nColumns = pViewLayoutItem->GetValue();
        const bool bBookMode  = pViewLayoutItem->IsBookMode();

        if (0 == nColumns)
        {
            m_pAutomaticBtn->Check();
            m_pColumnsEdit->SetValue(2);
            m_pColumnsEdit->Disable();
            m_pBookModeChk->Disable();
        }
        else if (1 == nColumns)
        {
            m_pSingleBtn->Check();
            m_pColumnsEdit->SetValue(2);
            m_pColumnsEdit->Disable();
            m_pBookModeChk->Disable();
        }
        else
        {
            m_pColumnsBtn->Check();
            if (!bBookMode)
            {
                m_pColumnsEdit->SetValue(nColumns);
                if (nColumns % 2 != 0)
                    m_pBookModeChk->Disable();
            }
            else
            {
                m_pColumnsEdit->SetValue(nColumns);
                m_pBookModeChk->Check();
            }
        }
    }
    else
    {
        // hide view layout related controls:
        m_pViewFrame->Disable();
    }
}

SvxZoomDialog::~SvxZoomDialog()
{}

IMPL_LINK(SvxZoomDialog, UserHdl, RadioButton *, pButton)
{
    mbModified = true;

    if (pButton == m_pUserBtn)
    {
        m_pUserEdit->Enable();
        m_pUserEdit->GrabFocus();
    }
    else
    {
        m_pUserEdit->Disable();
    }
    return 0;
}

IMPL_LINK_NOARG(SvxZoomDialog, SpinHdl)
{
    if (!m_pUserBtn->IsChecked())
        return 0;

    mbModified = true;
    return 0;
}

IMPL_LINK(SvxZoomDialog, ViewLayoutUserHdl, RadioButton*, pButton)
{
    mbModified = true;

    if (pButton == m_pAutomaticBtn)
    {
        m_pColumnsEdit->Disable();
        m_pBookModeChk->Disable();
    }
    else if (pButton == m_pSingleBtn)
    {
        m_pColumnsEdit->Disable();
        m_pBookModeChk->Disable();
    }
    else if (pButton == m_pColumnsBtn)
    {
        m_pColumnsEdit->Enable();
        m_pColumnsEdit->GrabFocus();
        if (m_pColumnsEdit->GetValue() % 2 == 0)
            m_pBookModeChk->Enable();
    }
    else
    {
        OSL_FAIL("Wrong Button");
        return 0;
    }

    return 0;
}

IMPL_LINK(SvxZoomDialog, ViewLayoutSpinHdl, NumericField*, pEdit)
{
    if (pEdit == m_pColumnsEdit && !m_pColumnsBtn->IsChecked())
        return 0;

    if (m_pColumnsEdit->GetValue() % 2 == 0)
    {
        m_pBookModeChk->Enable();
    }
    else
    {
        m_pBookModeChk->Check(false);
        m_pBookModeChk->Disable();
    }

    mbModified = true;

    return 0;
}

IMPL_LINK(SvxZoomDialog, ViewLayoutCheckHdl, CheckBox*, pCheckBox)
{
    if (pCheckBox == m_pBookModeChk && !m_pColumnsBtn->IsChecked())
        return 0;

    mbModified = true;

    return 0;
}

IMPL_LINK(SvxZoomDialog, OKHdl, Button*, pButton)
{
    if (mbModified || m_pOKBtn != pButton)
    {
        SvxZoomItem aZoomItem(SvxZoomType::PERCENT, 0, mrSet.GetPool()->GetWhich(SID_ATTR_ZOOM));
        SvxViewLayoutItem aViewLayoutItem(0, false, mrSet.GetPool()->GetWhich(SID_ATTR_VIEWLAYOUT));

        if (m_pOKBtn == pButton)
        {
            sal_uInt16 nFactor = GetFactor();

            if (SPECIAL_FACTOR == nFactor)
            {
                if (m_pOptimalBtn->IsChecked())
                    aZoomItem.SetType(SvxZoomType::OPTIMAL);
                else if (m_pPageWidthBtn->IsChecked())
                    aZoomItem.SetType(SvxZoomType::PAGEWIDTH);
                else if (m_pWholePageBtn->IsChecked())
                    aZoomItem.SetType(SvxZoomType::WHOLEPAGE);
            }
            else
            {
                aZoomItem.SetValue(nFactor);
            }

            if (m_pAutomaticBtn->IsChecked())
            {
                aViewLayoutItem.SetValue(0);
                aViewLayoutItem.SetBookMode(false);
            }
            if (m_pSingleBtn->IsChecked())
            {
                aViewLayoutItem.SetValue(1);
                aViewLayoutItem.SetBookMode(false);
            }
            else if (m_pColumnsBtn->IsChecked())
            {
                aViewLayoutItem.SetValue(static_cast<sal_uInt16>(m_pColumnsEdit->GetValue()));
                aViewLayoutItem.SetBookMode(m_pBookModeChk->IsChecked());
            }
        }
        else
        {
            OSL_FAIL("Wrong Button");
            return 0;
        }
        mpOutSet.reset(new SfxItemSet(mrSet));
        mpOutSet->Put(aZoomItem);

        // don't set attribute in case the whole viewlayout stuff is disabled:
        if (m_pViewFrame->IsEnabled())
            mpOutSet->Put(aViewLayoutItem);

        // memorize value from the UserEdit beyond the dialog
        SfxObjectShell* pShell = SfxObjectShell::Current();

        if (pShell)
        {
            sal_uInt16 nZoomValue = static_cast<sal_uInt16>(m_pUserEdit->GetValue());
            pShell->PutItem(SfxUInt16Item(SID_ATTR_ZOOM_USER, nZoomValue));
        }
        EndDialog( RET_OK );
    }
    else
    {
        EndDialog( RET_CANCEL );
    }
    return 0;
}

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
