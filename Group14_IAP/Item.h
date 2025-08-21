#pragma once
#include <string>
class Item
{
private:

	std::string ItemName;
	std::string ItemDescription;
	char Type;
	int Value;
	int ResaleValue;
	int SaleValue;
	int Number;

public:
	
	Item();

	Item(std::string ItemName, std::string ItemDescription, char Type, int Value, int ResaleValue, int SaleValue, int number);

	~Item();

	std::string GetItemWord(char ItemType) const;

	void SetItemWord(char ChangeType, std::string Change);

	int GetItemValue(char Type) const;

	void SetItemValue(char Type, int Value);

	char GetType() const;

	void SetType(char Type);

	int GetNumber() const;

	void SetNumber(int Numberinsert);

};

