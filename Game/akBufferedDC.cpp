#include "stdafx.h"

#include "akBufferedDC.h"


//
//  CakBufferedDC
//
CakBufferedDC::CakBufferedDC(CWnd *pParent)
    : m_pParent(pParent)
{
    ASSERT(pParent);

    //  Prepare window to painting
    m_pTarget = m_pParent->BeginPaint(&m_PaintStruct);
    m_pParent->GetClientRect(&m_RcClient);
    m_pParent->GetWindowRect(&m_RcWindow);

    //  Prepare buffer
    m_MemoryDC.CreateCompatibleDC(m_pTarget);
    m_MemoryBmp.CreateBitmap(m_RcClient.Width(), m_RcClient.Height(), 
        m_MemoryDC.GetDeviceCaps(PLANES), m_MemoryDC.GetDeviceCaps(BITSPIXEL), 0);
    m_pOldMemoryBmp = m_MemoryDC.SelectObject(&m_MemoryBmp);

    //  Attach self to the memory buffer
    Attach(m_MemoryDC);
}

CakBufferedDC::~CakBufferedDC()
{
    //  Copy buffer to window's DC
    m_pTarget->BitBlt(m_PaintStruct.rcPaint.left, m_PaintStruct.rcPaint.top, 
        m_PaintStruct.rcPaint.right - m_PaintStruct.rcPaint.left, 
        m_PaintStruct.rcPaint.bottom - m_PaintStruct.rcPaint.top, 
        &m_MemoryDC, m_PaintStruct.rcPaint.left, m_PaintStruct.rcPaint.top, SRCCOPY);

    //  Release buffer
    m_MemoryDC.SelectObject(m_pOldMemoryBmp);

    //  End painting
    m_pParent->EndPaint(&m_PaintStruct);

    //  Detach self from the buffer
    Detach();
}

BOOL CakBufferedDC::Attach(HDC hDC)
{
    return CDC::Attach(hDC);
}

HDC CakBufferedDC::Detach()
{
    return CDC::Detach();
}
