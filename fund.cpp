#include "fund.h"

Fund::Fund()
{
	balance_ = 0;
}
void Fund::AddHistory(TransactionData add)
{
	history_.push_back(add);
	return;
}
void Fund::DisplayHistory()const
{
	for (int i = 0; i < history_.size(); i++)
	{
		cout << history_[i];
	}
}
void Fund::Deposit(const int value)
{
	balance_ += value;
}
int Fund::GetBalance()const
{
	return balance_;
}
bool Fund::Withdraw(const int withdraw_value)
{
	if (balance_ >= withdraw_value)
	{
		balance_ -= withdraw_value;
		return true;
	}
	return false;
}
bool Fund::IsHistory() const
{
	if (history_.size() == 0)
	{
		return false;
	}
	return true;
}
