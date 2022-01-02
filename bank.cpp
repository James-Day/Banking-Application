#include "bank.h"

Bank::Bank()
{
	name_ = "";
}
Bank::Bank(std::string file, std::string name)
{
	name_ = name;
	ReadTransactions(file);
}
Bank::~Bank()
{
	accounts_.Empty();
}
void Bank::ReadTransactions(std::string file)
{
	ifstream myfile;
	string input = "";
	myfile.open(file);
	if (!myfile.is_open())
	{
		cerr << "FILE NOT FOUND";
		return;
	}
	while (std::getline(myfile, input))
	{
		if (input != "")
		{						//skip white lines
			istringstream input_stream(input);
			string inputs[kNumCommands] = { "" };
			for (int i = 0; input_stream >> inputs[i]; i++) {
			}
				queue_.push(Transaction(inputs));
		}
	}
	myfile.close();
}
void Bank::ExecuteTransactions()
{
	while (!queue_.empty()) {
		Transaction cur = queue_.front();
		queue_.pop();
		cur.Execute(accounts_);
	}
}
BSTree* Bank::GetAccounts()
{
	return &accounts_;
}

ostream& operator<<(ostream& stream, const Bank& rhs)
{
	if (!rhs.accounts_.isEmpty())
	{
		stream << "FINAL BALANCES" << endl;
	}
	rhs.accounts_.Display();
	return stream;
}
