#pragma once
#include <iostream>

class Inventory
{
public:
	Inventory();
	void addtoinv(int a);
	void display();
private:
	int Items;
	int numItems;
};