#include "stdafx.h"
#include "Lists.h"

// список - власник своїх елементів, тому деструктор знищує весь вміст
TList::~TList()
{
	Node* victim = m_head;
	while (victim != nullptr)
	{
		m_head = victim->next;
		delete victim->obj;
		delete victim;
		victim = m_head;
	}
}

void TList::Put(TMovedObject* obj)
{
	m_head->next = new Node(obj, m_head->next);
}

// для коректного вилучення ланки потрібен вказівник на попередню ланку
void TList::Remove(Node* pre)
{
	Node* victim = pre->next;
	pre->next = victim->next;
	delete victim->obj;
	delete victim;
}

// звичайний перебір, заголовна ланка не враховується
void TList::ShowAll(CDC& dc)
{
	Node* current = m_head->next;
	while (current != nullptr)
	{
		current->obj->Show(dc);
		current = current->next;
	}
}

void TList::MoveAll()
{
	Node* current = m_head->next;
	while (current != nullptr)
	{
		current->obj->Move();
		current = current->next;
	}
}

// перебір з вилученням
void TList::Garbage()
{
	Node* current = m_head;
	while (current->next != nullptr)
		if (current->next->obj->IsOut()) this->Remove(current);
		else current = current->next;
}

//-------------------

// Щоб з'ясувати, чи влучила bullet у котрийсь з літаків, треба перебрати всі,
//  кожному передати кулю для перевірки. Влучання припиняє перебір, змінює
//  стан літака і сигналізує про кінець існування кулі
// Приведення типу застосовано, щоб змусити літак виконати специфічні для нього дії
bool TAircraftList::LookForShotedBy(TBullet& bullet)
{
	Node* current = m_head->next;
	while (current != nullptr)
		if (static_cast<TAircraft*>(current->obj)->HitBy(bullet))
		{
			static_cast<TAircraft*>(current->obj)->Crash();
			return true;
		}
		else current = current->next;
	return false;
}

// Перебір з вилученням: якщо котрась куля влучила, то її треба вилучити
void TBulletList::LookForHitIn(TAircraftList& list)
{
	Node* current = m_head;
	while (current->next != nullptr)
		if (list.LookForShotedBy(*static_cast<TBullet*>(current->next->obj)) )
			this->Remove(current);
		else current = current->next;
}