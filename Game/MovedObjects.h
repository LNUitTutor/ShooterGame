/*        Ієрархія рухомих            TVisibleObject
               об'єктів            TGun         TMovedObject
                                            TBullet      TAircraft

Абстрактний базовий клас TVisibleObject 
- задає структуру для зберігання даних об'єкта:
 координати, розміри, швидкість переміщення, бітмапку зовнішнього вигляду,
 а також, маску і посилання на контекст пристрою, потрібні для виведення графіки
- вказує, що всі об'єкти вміють себе зображати.
- статичні елементи класу потрібні для швидкого створення екземплярів

Клас TGun:TVisibleObject зображає гармату, моделює дискретні переміщення
 ліворуч-праворуч та можливість пострілу - породження кулі

Проміжний абстрактний клас TMovedObject:TVisibleObject описує
- вміння об'єктів рухатись самостійно (міняти координати)
- можливість вильоту за межі вікна

Клас TBullet:TMovedObject зображає снаряд і
- конкретизує спосіб переміщення: вертикально
- конкретизує вихід за межі вікна: за верхню межу
- розширяє можливості базового класу: повідомляє свої координати

Клас TAircraft:TMovedObject зображає літак і
- конкретизує спосіб переміщення: горизонтально
- конкретизує вихід за межі вікна: за праву межу
- розширяє можливості базового класу: перевірка влучання снаряда
       та перехід до підбитого стану
*/

#pragma once

class TVisibleObject abstract
{
	// "секретна" константа для виведення зображення з прозорими цятками
	enum {ROP_DstCopy = 0x00AA0029};

protected:
	int m_x, m_y;			// координати лівого верхнього кута зображення
	int m_width, m_height;	// розміри зображення
	int m_step;				// швидкість переміщення (крок за одиницю часу)
	HBITMAP m_view, m_mask;	// власне зображення (вигляд і монохромна маска)
	HDC m_hMemoryDC;		// контекст для зображення

	static int ScreenWidth;  // розміри графічного екрана -
	static int ScreenHeight; // вікна застосунку

	// графічні зображення об'єктів потрібно завантажити з ресурсів аплікації
	// і використовувати для створення екземплярів
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
	// завантажує зображення
	static void InitBitmaps(HINSTANCE);
	// запам'ятовує розміри екрана
	static void InitScreenSize(int width, int height);

	TVisibleObject(): m_x(0),m_y(0), m_width(0),m_height(0), m_step(0), m_view(0),m_mask(0), m_hMemoryDC(0) {}
	TVisibleObject(int x, int y, int w, int h, int s, HBITMAP b, HBITMAP m, HDC dc)
		: m_x(x), m_y(y), m_width(w), m_height(h), m_step(s), m_view(b), m_mask(m), m_hMemoryDC(dc) {}
	virtual ~TVisibleObject() {}

	// усі видимі об'єкти вміють себе зображати на контексті пристрою
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
	// частину параметрів конструктора можна одразу конкретизувати:
	// відомі розміри та бітмапки зображення/маски
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
	// змінна стану: підбитий літак має інший зовнішній вигляд і рухається інакше
	int m_killed;
public:
	TAircraft() : TMovedObject(), m_killed(0) {}
	TAircraft(int y, int s, HDC dc);	// частину параметрів можна одразу конкретизувати
	virtual void Move();
	virtual bool IsOut() const;
	// метод зміни стану літака
	void Crash();
	// перевірка на влучання кулі
	bool HitBy(const TBullet&) const;
};

//----------------------

class TGun : public TVisibleObject
{
public:
	TGun() : TVisibleObject() {}
	// майже всі параметри можна одразу конкретизувати
	TGun(HDC dc) : TVisibleObject(ScreenWidth / 2 - 10, ScreenHeight - 40, 20, 30, 5, GunView, GunMask, dc) {}
	// постріл створює новий об'єкт - снаряд
	TBullet* Fire() const;
	// переміщення обмежені розмірами вікна
	void MoveLeft() { m_x = (m_x >= m_step + 5) ? m_x - m_step : 5; }
	void MoveRight() 
	{
		int limit = ScreenWidth - m_width - 5;
		m_x = (m_x <= limit - m_step) ? m_x + m_step : limit;
	}
};
