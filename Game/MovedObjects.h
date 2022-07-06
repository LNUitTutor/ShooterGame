/*        �������� �������            TVisibleObject
               ��'����            TGun         TMovedObject
                                            TBullet      TAircraft

����������� ������� ���� TVisibleObject 
- ���� ��������� ��� ��������� ����� ��'����:
 ����������, ������, �������� ����������, ������� ���������� �������,
 � �����, ����� � ��������� �� �������� ��������, ������ ��� ��������� �������
- �����, �� �� ��'���� ����� ���� ���������.
- ������� �������� ����� ������ ��� �������� ��������� ����������

���� TGun:TVisibleObject ������� �������, ������� �������� ����������
 ������-�������� �� ��������� ������� - ���������� ���

�������� ����������� ���� TMovedObject:TVisibleObject �����
- ����� ��'���� �������� ��������� (����� ����������)
- ��������� ������� �� ��� ����

���� TBullet:TMovedObject ������� ������ �
- ���������� ����� ����������: �����������
- ���������� ����� �� ��� ����: �� ������ ����
- �������� ��������� �������� �����: ��������� ��� ����������

���� TAircraft:TMovedObject ������� ���� �
- ���������� ����� ����������: �������������
- ���������� ����� �� ��� ����: �� ����� ����
- �������� ��������� �������� �����: �������� �������� �������
       �� ������� �� �������� �����
*/

#pragma once

class TVisibleObject abstract
{
	// "��������" ��������� ��� ��������� ���������� � ��������� �������
	enum {ROP_DstCopy = 0x00AA0029};

protected:
	int m_x, m_y;			// ���������� ����� ��������� ���� ����������
	int m_width, m_height;	// ������ ����������
	int m_step;				// �������� ���������� (���� �� ������� ����)
	HBITMAP m_view, m_mask;	// ������ ���������� (������ � ���������� �����)
	HDC m_hMemoryDC;		// �������� ��� ����������

	static int ScreenWidth;  // ������ ���������� ������ -
	static int ScreenHeight; // ���� ����������

	// ������� ���������� ��'���� ������� ����������� � ������� ��������
	// � ��������������� ��� ��������� ����������
	static HBITMAP FastView;
	static HBITMAP FastMask;
	static HBITMAP MidView;
	static HBITMAP MidMask;
	static HBITMAP SlowView;
	static HBITMAP SlowMask;
	static HBITMAP CrashView;
	static HBITMAP CrashMask;
	static HBITMAP BulletView;
	static HBITMAP BulletMask;
	static HBITMAP GunView;
	static HBITMAP GunMask;
public:
	// ��������� ����������
	static void InitBitmaps(HINSTANCE);
	// �����'����� ������ ������
	static void InitScreenSize(int width, int height);

	TVisibleObject(): m_x(0),m_y(0), m_width(0),m_height(0), m_step(0), m_view(0),m_mask(0), m_hMemoryDC(0) {}
	TVisibleObject(int x, int y, int w, int h, int s, HBITMAP b, HBITMAP m, HDC dc)
		: m_x(x), m_y(y), m_width(w), m_height(h), m_step(s), m_view(b), m_mask(m), m_hMemoryDC(dc) {}
	virtual ~TVisibleObject() {}

	// �� ����� ��'���� ����� ���� ��������� �� �������� ��������
	void Show(CDC&) const;
};

//----------------------

class TMovedObject : public TVisibleObject
{
public:
	TMovedObject() :TVisibleObject() {}
	TMovedObject(int x, int y, int w, int h, int s, HBITMAP b, HBITMAP m, HDC dc)
		: TVisibleObject(x, y, w, h, s, b, m, dc) {}
	virtual void Move() abstract;
	virtual bool IsOut() const abstract;
};

class TBullet : public TMovedObject
{
public:
	TBullet() : TMovedObject() {}
	// ������� ��������� ������������ ����� ������ ��������������:
	// ���� ������ �� ������� ����������/�����
	TBullet(int x, int y, HDC dc)
		: TMovedObject(x, y, 2, 4, 6, BulletView, BulletMask, dc) {}
	virtual void Move() { m_y -= m_step; }
	virtual bool IsOut() const { return m_y < 5; }
	int GetX() const { return m_x; }
	int GetY() const { return m_y; }
};

class TAircraft : public TMovedObject
{
private:
	// ����� �����: ������� ���� �� ����� ������� ������ � �������� ������
	int m_killed;
public:
	TAircraft() : TMovedObject(), m_killed(0) {}
	TAircraft(int y, int s, HDC dc);	// ������� ��������� ����� ������ ��������������
	virtual void Move();
	virtual bool IsOut() const;
	// ����� ���� ����� �����
	void Crash();
	// �������� �� �������� ���
	bool HitBy(const TBullet&) const;
};

//----------------------

class TGun : public TVisibleObject
{
public:
	TGun() : TVisibleObject() {}
	// ����� �� ��������� ����� ������ ��������������
	TGun(HDC dc) : TVisibleObject(ScreenWidth / 2 - 10, ScreenHeight - 40, 20, 30, 5, GunView, GunMask, dc) {}
	// ������ ������� ����� ��'��� - ������
	TBullet* Fire() const;
	// ���������� ������� �������� ����
	void MoveLeft() { m_x = (m_x >= m_step + 5) ? m_x - m_step : 5; }
	void MoveRight() 
	{
		int limit = ScreenWidth - m_width - 5;
		m_x = (m_x <= limit - m_step) ? m_x + m_step : limit;
	}
};
