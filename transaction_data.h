#ifndef TRANSACTION_DATA_H_
#define TRANSACTION_DATA_H_
#include <iostream>
using namespace std;
class TransactionData
{
	friend ostream& operator<< (ostream& stream, const TransactionData& rhs);
public:
	TransactionData();
	TransactionData(const char type, const int id, const int fund, const int to_id, const int to_fund, const int value, bool failed);
private:
	char transaction_type_;
	int account_id_;
	int fund_number_;
	int to_account_id_;
	int to_fund_number_;
	int value_;
	bool failed_;
};
#endif
