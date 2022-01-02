#ifndef FUND_H_
#define FUND_H_
#include <vector>
#include <iostream>
#include "transaction_data.h"
using namespace std;
class Fund
{
public:
	Fund();
	void AddHistory(TransactionData add);
	void DisplayHistory() const;
	void Deposit(const int value);
	int	GetBalance() const;
	bool Withdraw(const int withdraw_value);
	bool IsHistory()const;
private:
	int balance_;
	vector<TransactionData> history_;
};
#endif
