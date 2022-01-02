#ifndef ACCOUNT_H_
#define ACCOUNT_H_
#include <iostream>
#include "fund.h"

const int kNumFunds = 8; 
class Account {
	friend ostream& operator<< (ostream& stream, const Account& rhs);
public:
	Account(const int id, const std::string first_name, std::string last_name);
	Fund* GetFunds();
	void AddHistory(TransactionData history);
	std::string GetFirstName()const;
	std::string GetLastName()const;
	bool operator<(const Account& rhs) const;
	bool operator>(const Account& rhs) const;
	bool operator<=(const Account& rhs)const;
	bool operator>=(const Account& rhs)const;
	bool operator==(const Account& rhs) const;
	bool operator!=(const Account& rhs) const;
	int GetId() const;
private:

	int id_;
	std::string first_name_;
	std::string last_name_;
	Fund funds_[kNumFunds];
	TransactionData account_history_;

};
#endif
