
// GameDlg.h : файл заголовка
//
#include "MovedObjects.h"
#include "Lists.h"
#include "Airfield.h"

#pragma once


// диалоговое окно CGameDlg
class CGameDlg :public CDialog
{
// Создание
public:
	CGameDlg(CWnd* pParent = NULL);	// стандартный конструктор

// Данные диалогового окна
	enum { IDD = IDD_GAME_DIALOG, K_LEFT = 188, K_RIGHT = 190 };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;
	CBitmap m_background_image;
	enum { TimerInterval = 15 };
	bool m_stop_timer;		// стан таймера - гру можна призупинити
	HDC m_hMemDC;			// контекст пристрою для зображення всіх об'єктів
	CRect m_client_rectangle;       // придатна для малювання частина вікна

// Тут описано всі складові гри: гармату, список куль, список літаків, аеродром
	TGun gun;			
	TBulletList bullet_list;
	TAircraftList aircraft_list;
	TAirfield* airfield;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// клацання мишею може затримати/відпустити гру
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	// рухом куль і літаків, роботою аеродрому керує таймер
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// гарматою керує гравець за допомогою клавіатури
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	// особливості MFC-діалогу такі, що натискання клавіш зі стрілками треба "виловлювати"
	BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};