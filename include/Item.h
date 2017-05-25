#pragma once
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Item
{
private:
	string filename;
	vector<string> context;
public:
	Item(string name);
	~Item();
	string getName() const;
	string getContext() const;
	bool operator==(Item _item) const;
	bool operator<(const Item _item) const;
};

ostream& operator<<(ostream& os, Item& item);
