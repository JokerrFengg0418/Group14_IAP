#include "Item.h"
#include <iostream>

Item::Item() {

	ItemName = "NIL";
	ItemDescription = "This Item should not exist";
	Value = 0;
	ResaleValue = 0;
	SaleValue = 0;
	Type = NULL;
}





Item::Item(std::string Item, std::string ItemDes, char type, int value, int resaleValue, int saleValue) {

	ItemName = Item;
	ItemDescription = ItemDes;
	Value = value;
	ResaleValue = resaleValue;
	SaleValue = saleValue;
	Type = type;

}




Item::~Item() {

	std::cout << ItemName << "was destroyed! \n";

}




std::string Item::GetItemWord(char WordType) const {

	switch (WordType) {

	case 'N':

		return ItemName;

	case 'D':

		return ItemDescription;

	}
}




void Item::SetItemWord(char ChangeType, std::string Change) {

	switch (ChangeType) {

	case 'N':
		ItemName = Change;
	case 'D':
		ItemDescription = Change;

	}
}

int Item::GetItemValue(char Type) const {

	switch (Type) {
	case 'V':
		return Value;
	case 'R':
		return ResaleValue;
	case 'S':
		return SaleValue;
	}

}

void Item::SetItemValue(char Type, int IncomingValue) {

	switch (Type) {
	case 'V':
		Value = IncomingValue;
		break;
	case 'R':
		ResaleValue = IncomingValue;
		break;
	case 'S':
		SaleValue = IncomingValue;
		break;
	}
}

char Item::GetType() const{

	return Type;

}

void Item::SetType(char NewType) {

	Type = NewType;

}




