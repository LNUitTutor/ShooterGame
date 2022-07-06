/*     �������� ������                        TList
               ��'����            TAircraftList   TBulletList

������� ���� TList ���������� ������'����� ������ ������� ��'���� ��
������ ��� ��������� "��������" ��������������� (���������, ���������, ������).
����� ������ ����� �������� ��� Node, ��� �������� - �������� �� ������� ����.

ϳ������ ���������� ��������������� �������� �����:
������ ����� TAircraftList �쳺 ���������, �� ������� ���� � �������� � �����.
������ ���� TBulletList �쳺 ���������� ��� �������� ������� ����� ��� ��������.
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
	// ������'����� ������ � ���������� ������
	TList() : m_head(new Node()) {}
	virtual ~TList();
	// ���� ��� ��'���� �� ������� ������ (������ ���� ��������� �����)
	void Put(TMovedObject*);
	// ������ �����, �� ��� ����� pre->next
	void Remove(Node* pre);
	// ���������� �� �������� �� ��� ��������
	void ShowAll(CDC&);
	// ������ �������� ������� ��������
	void MoveAll();
	// ������ � ������ � ��'����, �� ������� �� ��� ����
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