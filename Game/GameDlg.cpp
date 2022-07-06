
// GameDlg.cpp : файл реализации
//

#include "stdafx.h"
#include "Game.h"
#include "GameDlg.h"
#include "akBufferedDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Данные диалогового окна
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// диалоговое окно CGameDlg




CGameDlg::CGameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGameDlg::IDD, pParent), m_stop_timer(true)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGameDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// обработчики сообщений CGameDlg

// *** Метод доповнює роботу конструктора
BOOL CGameDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Задает значок для этого диалогового окна. Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// *** Тут виконуємо всю додаткову ініціалізацію: налаштовуємо клас TVisibleObject,
	// створюємо контекст, запускаємо таймер, створюємо гармату і аеродром.
	// Списки снарядів і літаків створив конструктор
	
	GetClientRect(&m_client_rectangle);
	TVisibleObject::InitScreenSize(m_client_rectangle.Width(), m_client_rectangle.Height());
	TVisibleObject::InitBitmaps(::AfxGetInstanceHandle());
	m_hMemDC = ::CreateCompatibleDC(NULL);
	m_background_image.LoadBitmap(IDB_BACKGROUND);

	gun = TGun(m_hMemDC);
	airfield = new TAirfield(aircraft_list, m_hMemDC);
	SetTimer(ID_TIMER, TimerInterval, NULL);

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CGameDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок. Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.


// *** Метод відображення вікна на екрані
void CGameDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else	// *** Тут зображаємо всі видимі об'єкти
	{
		// спеціальний контекст, що підтримує подвійну буферизацію зображень
		CakBufferedDC dc(this);
		// зафарбовуємо фон
		CDC memoryDC;
		memoryDC.CreateCompatibleDC(&dc);
		memoryDC.SelectObject(&m_background_image);
		dc.BitBlt(0, 0, m_client_rectangle.Width(), m_client_rectangle.Height(), &memoryDC, 0, 0, SRCCOPY);
		// зображаємо рухомі об'єкти
		gun.Show(dc);
		bullet_list.ShowAll(dc);
		aircraft_list.ShowAll(dc);
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CGameDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// *** Повідомлення від таймера потрібні багатьом:
void CGameDlg::OnTimer(UINT_PTR nIDEvent)
{
	airfield->Spent(TimerInterval);		// аеродром
	aircraft_list.MoveAll();			// літаки летять
	aircraft_list.Garbage();			//   і втікають
	bullet_list.MoveAll();				// кулі летять
	bullet_list.Garbage();				//   і самоліквідуються 
	bullet_list.LookForHitIn(aircraft_list);	// або влучають
	// після зміни координат об'єктів вікно треба перемалювати!!!
	Invalidate();

	CDialog::OnTimer(nIDEvent);
}

// *** клацання мишею змінює стан гри: пауза - рух
void CGameDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_stop_timer) KillTimer(ID_TIMER);
	else SetTimer(ID_TIMER, TimerInterval, NULL);
	m_stop_timer = !m_stop_timer;

	CDialog::OnLButtonDown(nFlags, point);
}

// *** Розпізнаємо, яка клавіша натиснута
void CGameDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_SPACE: bullet_list.Put(this->gun.Fire()); break;
	case K_LEFT: gun.MoveLeft(); break;  	// [ < | , ]
	case K_RIGHT: gun.MoveRight(); break;	// [ > | . ]
	case 'Q': ::PostQuitMessage(0);		// завершити виконання
	}

	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}
// Попередній метод не бачить клавіш зі стрілками, тому допомагаємо
BOOL CGameDlg::PreTranslateMessage(MSG* pMsg) 
{
    if(pMsg->message==WM_KEYDOWN)
    {
		if (pMsg->wParam == VK_LEFT) pMsg->wParam = K_LEFT;
		else if (pMsg->wParam == VK_RIGHT) pMsg->wParam = K_RIGHT;
    }	
    return CDialog::PreTranslateMessage(pMsg);
}
// Потрібна модифікація, щоб позбутися мерехтіння екрана
// Витирати екран буде сам метод OnPaint
BOOL CGameDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
