#include "account.h"

Account::Account(const int id, const std::string first_name, std::string last_name)
{
	id_ = id;
	first_name_ = first_name;
	last_name_ = last_name;
}

Fund* Account::GetFunds()
{
	return funds_;
}
void Account::AddHistory(TransactionData history)
{
	account_history_ = history;
}
int Account::GetId() const
{
	return id_;
}
std::string Account::GetFirstName()const
{
	return first_name_;
}
std::string Account::GetLastName()const
{
	return last_name_;
}

bool Account::operator<(const Account& rhs) const
{
	if (id_ < rhs.id_)
	{
		return true;
	}
	return false;
}
bool Account::operator<=(const Account& rhs) const
{
	if (*this < rhs)
	{
		return true;
	}
	else if (*this == rhs)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool Account::operator>(const Account& rhs) const
{
	if (id_ > rhs.id_)
	{
		return true;
	}
	return false;
}
bool Account::operator>=(const Account& rhs) const
{
	if (*this > rhs)
	{
		return true;
	}
	else if (*this == rhs)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool Account::operator==(const Account& rhs) const
{
	if (id_ == rhs.id_)
	{
		return true;
	}
	return false;
}
bool Account::operator!=(const Account& rhs) const
{
	return !(*this == rhs);
}

ostream& operator<<(ostream& stream, const Account& rhs)
{
	stream << rhs.first_name_ << " " << rhs.last_name_ << "Account ID: " << rhs.id_ << endl;
	stream << "Money Market : $" << rhs.funds_[0].GetBalance() << endl;
	stream << "Prime Money Market : $" << rhs.funds_[1].GetBalance() << endl;
	stream << "Long - Term Bond : $" << rhs.funds_[2].GetBalance() << endl;
	stream << "Short - Term Bond : $" << rhs.funds_[3].GetBalance() << endl;
	stream << "500 Index Fund $" << rhs.funds_[4].GetBalance() << endl;
	stream << "Capital Value Fund : $" << rhs.funds_[5].GetBalance() << endl;
	stream << "Growth Equity Fund : $" << rhs.funds_[6].GetBalance() << endl;
	stream << "Growth Index Fund : $" << rhs.funds_[7].GetBalance() << endl;
	stream << endl;
	return stream;
}
