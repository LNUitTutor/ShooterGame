#pragma once


//
//  CakBufferedDC
//
class CakBufferedDC : public CDC
{

private:
    CakBufferedDC() { }                                     //  Prohibited
    CakBufferedDC(const CakBufferedDC &src) { }             //  Prohibited
    CakBufferedDC& operator=(const CakBufferedDC &src) { }  //  Prohibited

protected:
    BOOL Attach(HDC hDC);                                   //  Shouldn't be called directly
    HDC Detach();                                           //  Shouldn't be called directly

private:
    CWnd *m_pParent;                                        //  Pointer to parent window
    CDC *m_pTarget;                                         //  Pointer to target DC
    PAINTSTRUCT m_PaintStruct;                              //  Paint struct for BeginPaint()/EndPaint() pair
    CRect m_RcClient, m_RcWindow;                           //  Client and window rectangles

    CDC m_MemoryDC;                                         //  Buffer DC
    CBitmap m_MemoryBmp, *m_pOldMemoryBmp;                  //  Buffer bitmaps

public:
    CakBufferedDC(CWnd *pParent);
    ~CakBufferedDC();

public:
    inline CRect ClientRect() const { return m_RcClient; }
    inline CRect WindowRect() const { return m_RcWindow; }
    inline CRect UpdateRect() const { return m_PaintStruct.rcPaint; }

    operator HDC() const { return m_MemoryDC.m_hDC; }       //  DC handle for API functions

};
