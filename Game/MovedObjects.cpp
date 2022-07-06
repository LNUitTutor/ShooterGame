#include "stdafx.h"
#include "Resource.h"
#include "MovedObjects.h"

void TVisibleObject::Show(CDC& dc) const
{
// для відображення графіки використано АРІ-функцію
	SelectObject(m_hMemoryDC, m_view);
	::MaskBlt(dc.m_hDC,m_x,m_y,m_width,m_height,m_hMemoryDC,0,0,m_mask,0,0,MAKEROP4(ROP_DstCopy,SRCCOPY));
}

//------------------------------------

TAircraft::TAircraft(int y, int s, HDC dc)
		: TMovedObject(1, y, 0, 0, s, 0, 0, dc), m_killed(0)
{
// літаки бувають трьох видів
	if (s < 3)		// повільні
	{
		m_width = 30; m_height = 16;
		m_view = SlowView; m_mask = SlowMask;
	}
	else if (s < 6)	// звичайні
	{
		m_width = 31; m_height = 18;
		m_view = MidView; m_mask = MidMask;
	}
	else			// швидкі
	{
		m_width = 29; m_height = 14;
		m_view = FastView; m_mask = FastMask;
	}
}

void TAircraft::Move()
{
	m_x += m_step;
	if (m_killed)		// підбитий літак падає
	{
		m_y += m_killed;
		m_killed *= 2;
	}
}
void TAircraft::Crash()
{
	m_view = CrashView;
	m_mask = CrashMask;
	m_killed = 1;
}
bool TAircraft::IsOut() const
{
	return m_x > ScreenWidth - m_width || m_killed > 32;
}
// перевірка влучання - порівняння координат об'єктів на співпадіння
bool TAircraft::HitBy(const TBullet& b) const
{
	return b.GetX() > m_x && b.GetX() + 1 < m_x + m_width && b.GetY() > m_y && b.GetY() < m_y + m_height;
}

//-------------------------------------

TBullet* TGun::Fire() const
{
	return new TBullet(m_x + 9, m_y - 4, m_hMemoryDC);
}

//-------------------------------------

HBITMAP TVisibleObject::FastView = 0;
HBITMAP TVisibleObject::FastMask = 0;
HBITMAP TVisibleObject::MidView = 0;
HBITMAP TVisibleObject::MidMask = 0;
HBITMAP TVisibleObject::SlowView = 0;
HBITMAP TVisibleObject::SlowMask = 0;
HBITMAP TVisibleObject::CrashView = 0;
HBITMAP TVisibleObject::CrashMask = 0;
HBITMAP TVisibleObject::BulletView = 0;
HBITMAP TVisibleObject::BulletMask = 0;
HBITMAP TVisibleObject::GunView = 0;
HBITMAP TVisibleObject::GunMask = 0;

int TVisibleObject::ScreenWidth = 100;
int TVisibleObject::ScreenHeight = 100;

// ініціалізація змінних класу - завантаження необхідних зображень з ресурсів аплікації
void TVisibleObject::InitBitmaps(HINSTANCE h)
{
	FastView = ::LoadBitmap(h,MAKEINTRESOURCE(IDB_FASTVIEW));
	FastMask = ::LoadBitmap(h,MAKEINTRESOURCE(IDB_FASTMASK));
	MidView = ::LoadBitmap(h,MAKEINTRESOURCE(IDB_MIDVIEW));
	MidMask = ::LoadBitmap(h,MAKEINTRESOURCE(IDB_MIDMASK));
	SlowView = ::LoadBitmap(h,MAKEINTRESOURCE(IDB_SLOWVIEW));
	SlowMask = ::LoadBitmap(h,MAKEINTRESOURCE(IDB_SLOWMASK));
	CrashView = ::LoadBitmap(h,MAKEINTRESOURCE(IDB_CRASHVIEW));
	CrashMask = ::LoadBitmap(h,MAKEINTRESOURCE(IDB_CRASHMASK));
	BulletView = ::LoadBitmap(h,MAKEINTRESOURCE(IDB_BULLETVIEW));
	BulletMask = ::LoadBitmap(h,MAKEINTRESOURCE(IDB_BULLETMASK));
	GunView = ::LoadBitmap(h,MAKEINTRESOURCE(IDB_GUNVIEW));
	GunMask = ::LoadBitmap(h,MAKEINTRESOURCE(IDB_GUNMASK));
}

void TVisibleObject::InitScreenSize(int width, int height)
{
	ScreenWidth = width;
	ScreenHeight = height;
}
