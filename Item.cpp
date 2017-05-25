#include "include\Item.h"

Item::Item(string title, string author, string source)
{
	_title = title;
	_author = author;
	_source = source;
}


Item::~Item()
{
}

string Item::getAuthor() const { return _author; }
string Item::getTitle() const { return _title; }
string Item::getSource() const{ return _source; }

bool Item ::operator==(Item _item) const
{
	return (_title == _item.getTitle());
}

bool Item::operator<(const Item _item) const
{
	return _title < _item.getTitle();
}

ostream& operator<<(ostream& os, Item& item)
{
    return os<<item.getTitle()<<'\t'<<item.getAuthor()<<'\t'<<item.getSource();
}
