#include "transaction_data.h"

TransactionData::TransactionData(const char type, const int id, const int fund, const int to_id, const int to_fund, const int value, bool failed)
{
	transaction_type_ = type;
	account_id_ = id;
	fund_number_ = fund;
	to_account_id_ = to_id;
	to_fund_number_ = to_fund;
	value_ = value;
	failed_ = failed;
}
TransactionData::TransactionData()
{
	transaction_type_ = '~';
	account_id_ = -1;
	fund_number_ = -1;
	to_account_id_ = -1;
	to_fund_number_ = -1;
	value_ = -1;
	failed_ = true;
}
ostream& operator<<(ostream& stream, const TransactionData& rhs)
{
	//Check all values to see if they are default values before outputing them
	if (rhs.transaction_type_ != '~')
	{
		cout << " " << rhs.transaction_type_ << " ";
	}
	if (rhs.account_id_ != -1)
	{
		cout << rhs.account_id_ << " ";
	}
	if (rhs.fund_number_ != -1)
	{
		cout << rhs.fund_number_ << " ";
	}
	if (rhs.to_account_id_ != -1)
	{
		cout << rhs.to_account_id_ << " ";
	}
	if (rhs.to_fund_number_ != -1)
	{
		cout << rhs.to_fund_number_ << " ";
	}
	if (rhs.value_ != -1)
	{
		cout << rhs.value_;
	}
	if (rhs.failed_)
	{
		cout << " (Failed)";
	}
	cout << endl;
	return stream;
}
