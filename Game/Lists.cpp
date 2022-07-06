#include "stdafx.h"
#include "Lists.h"

// ������ - ������� ���� ��������, ���� ���������� ����� ���� ����
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

// ��� ���������� ��������� ����� ������� �������� �� ��������� �����
void TList::Remove(Node* pre)
{
	Node* victim = pre->next;
	pre->next = victim->next;
	delete victim->obj;
	delete victim;
}

// ��������� ������, ��������� ����� �� �����������
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

// ������ � ����������
void TList::Garbage()
{
	Node* current = m_head;
	while (current->next != nullptr)
		if (current->next->obj->IsOut()) this->Remove(current);
		else current = current->next;
}

//-------------------

// ��� �'�������, �� ������� bullet � �������� � �����, ����� ��������� ��,
//  ������� �������� ���� ��� ��������. �������� �������� ������, �����
//  ���� ����� � �������� ��� ����� ��������� ���
// ���������� ���� �����������, ��� ������� ���� �������� ��������� ��� ����� 䳿
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

// ������ � ����������: ���� ������� ���� �������, �� �� ����� ��������
void TBulletList::LookForHitIn(TAircraftList& list)
{
	Node* current = m_head;
	while (current->next != nullptr)
		if (list.LookForShotedBy(*static_cast<TBullet*>(current->next->obj)) )
			this->Remove(current);
		else current = current->next;
}