/*     Ієрархія списків                        TList
               об'єктів            TAircraftList   TBulletList

Базовий клас TList інкапсулює однозв'язний список рухомих об'єктів та
реалізує всю необхідну "спискову" функціональність (додавання, вилучення, перебір).
Ланку списку описує внутрішній тип Node, тип елемента - вказівник на базовий клас.

Підкласи розширюють функціональність базового класу:
Список літаків TAircraftList вміє перевіряти, чи влучила куля в котрийсь з літаків.
Список куль TBulletList вміє передавати свої елементи спискові літаків для перевірки.
*/

#include "MovedObjects.h"

#pragma once

class TList
{
protected:
	struct Node
	{
		TMovedObject* obj;
		Node* next;
		Node(TMovedObject* ob = 0, Node* n = 0) : obj(ob), next(n) {}
	};
	Node* m_head;
public:
	// однозв'язний список з заголовною ланкою
	TList() : m_head(new Node()) {}
	virtual ~TList();
	// додає нові об'єкти на початок списку (одразу після заголовної ланки)
	void Put(TMovedObject*);
	// вилучає ланку, на яку вказує pre->next
	void Remove(Node* pre);
	// промальовує на контексті всі свої елементи
	void ShowAll(CDC&);
	// велить рухатися кожному елементу
	void MoveAll();
	// вилучає зі списку ті об'єкти, які вилетіли за межі вікна
	void Garbage();
};

class TAircraftList : public TList
{
public:
	TAircraftList() : TList() {}
	bool LookForShotedBy(TBullet&);
};

class TBulletList : public TList
{
public:
	TBulletList() : TList() {}
	void LookForHitIn(TAircraftList&);
};