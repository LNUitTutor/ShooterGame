
// GameDlg.h : ���� ���������
//
#include "MovedObjects.h"
#include "Lists.h"
#include "Airfield.h"

#pragma once


// ���������� ���� CGameDlg
class CGameDlg :public CDialog
{
// ��������
public:
	CGameDlg(CWnd* pParent = NULL);	// ����������� �����������

// ������ ����������� ����
	enum { IDD = IDD_GAME_DIALOG, K_LEFT = 188, K_RIGHT = 190 };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// ��������� DDX/DDV


// ����������
protected:
	HICON m_hIcon;
	CBitmap m_background_image;
	enum { TimerInterval = 15 };
	bool m_stop_timer;		// ���� ������� - ��� ����� �����������
	HDC m_hMemDC;			// �������� �������� ��� ���������� ��� ��'����
	CRect m_client_rectangle;       // �������� ��� ��������� ������� ����

// ��� ������� �� ������� ���: �������, ������ ����, ������ �����, ��������
	TGun gun;			
	TBulletList bullet_list;
	TAircraftList aircraft_list;
	TAirfield* airfield;

	// ��������� ������� ����� ���������
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// �������� ����� ���� ���������/��������� ���
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	// ����� ���� � �����, ������� ��������� ���� ������
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// �������� ���� ������� �� ��������� ���������
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	// ���������� MFC-������ ���, �� ���������� ����� � �������� ����� "�����������"
	BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};