#include "transaction.h"

Transaction::Transaction(std::string commands[kNumCommands])
{
	for (int i = 0; i < kNumCommands; i++)
	{
		commands_[i] = commands[i];
	}
}
bool Transaction::OpenAccount(const int id, const string first_name, const string last_name, BSTree& accounts)
{
	Account* acc = new Account(id, first_name, last_name);
	bool inserted = accounts.Insert(acc);
	if (inserted)
	{
		TransactionData history('O', id, -1, -1, -1, -1, !inserted);
		delete acc;
		acc = nullptr;
		accounts.Retrieve(id, acc); // find the copy made in the tree
		acc->AddHistory(history);
		return inserted;
	}
	else
	{
		cerr << "ERROR: Acount " << id << " is already open. Transaction refused." << endl;
		delete acc;
		return false;
	}

}
bool Transaction::Deposit(const int id, const int fund, const int deposit_value, const BSTree& accounts)
{
	Account* acc = nullptr;
	accounts.Retrieve(id, acc);
	if (acc == nullptr)
	{
		cerr << "ERROR: Account " << id << " not found. Deposit refused." << endl;
		return false;
	}
	Fund* currFund = &acc->GetFunds()[fund];
	currFund->Deposit(deposit_value);
	TransactionData history('D', id, fund, -1, -1, deposit_value, false);
	currFund->AddHistory(history);
	return true;
}
bool Transaction::Withdraw(const int id, const int fund, const int withdraw_value, const BSTree& accounts)
{
	Account* acc = nullptr;
	accounts.Retrieve(id, acc);
	if (acc == nullptr)
	{
		cerr << "ERROR: Account " << id << " not found. Withdraw refused." << endl;
		return false;
	}
	Fund* currFund = &acc->GetFunds()[fund];

	if (fund == 0 || fund == 1)
	{ //Linked accounts 
		int money_market_balance = acc->GetFunds()[0].GetBalance();
		int prime_money_market_balance = acc->GetFunds()[1].GetBalance();
		if (withdraw_value > (money_market_balance + prime_money_market_balance))
		{	//Transaction can not be complete
			TransactionData new_history('W', id, fund, -1, -1, withdraw_value, true);
			currFund->AddHistory(new_history);
			cerr << "ERROR: Not enough funds to withdraw " << withdraw_value << " from " << acc->GetFirstName() << " " << acc->GetLastName() << " " << FundName(fund) << endl;
			return false;
		}
		if (fund == 0)
		{
			if (money_market_balance < withdraw_value)
			{ // take money from both accounts
				WithdrawHelper(acc, 0, 1, withdraw_value);
				return true;
			}
			else
			{
				currFund->Withdraw(withdraw_value);
				TransactionData new_history('W', id, fund, -1, -1, withdraw_value, false);
				currFund->AddHistory(new_history);
				return true;
			}
		}
		else if (fund == 1)
		{
			if (prime_money_market_balance < withdraw_value)
			{// take money from both accounts
				WithdrawHelper(acc, 1, 0, withdraw_value);
				return true;
			}
			else
			{
				currFund->Withdraw(withdraw_value);
				TransactionData new_history('W', id, fund, -1, -1, withdraw_value, false);
				currFund->AddHistory(new_history);
				return true;
			}
		}
	}
	else if (fund == 2 || fund == 3)
	{ //Linked accounts 
		int long_term_bond = acc->GetFunds()[2].GetBalance();
		int short_term_bond = acc->GetFunds()[3].GetBalance();
		if (withdraw_value > (long_term_bond + short_term_bond))
		{//Transaction can not be complete
			TransactionData new_history('W', id, fund, -1, -1, withdraw_value, true);
			currFund->AddHistory(new_history);
			cerr << "ERROR: Not enough funds to withdraw " << withdraw_value << " from " << acc->GetFirstName() << " " << acc->GetLastName() << " " << FundName(fund) << endl;
			return false;
		}
		if (fund == 2)
		{
			if (long_term_bond < withdraw_value)
			{// take money from both accounts
				WithdrawHelper(acc, 2, 3, withdraw_value);
				return true;
			}
			else
			{
				currFund->Withdraw(withdraw_value);
				TransactionData new_history('W', id, fund, -1, -1, withdraw_value, false);
				currFund->AddHistory(new_history);
				return true;
			}
		}
		if (fund == 3)
		{
			if (short_term_bond < withdraw_value)
			{// take money from both accounts
				WithdrawHelper(acc, 3, 2, withdraw_value);
				return true;
			}
			else
			{
				currFund->Withdraw(withdraw_value);
				TransactionData new_history('W', id, fund, -1, -1, withdraw_value, false);
				currFund->AddHistory(new_history);
				return true;
			}
		}
	}
	else if (fund > 3)
	{
		if (currFund->GetBalance() >= withdraw_value)
		{
			currFund->Withdraw(withdraw_value);
			TransactionData new_history('W', id, fund, -1, -1, withdraw_value, false);
			currFund->AddHistory(new_history);
			return true;
		}
	}
	TransactionData new_history('W', id, fund, -1, -1, withdraw_value, true);
	currFund->AddHistory(new_history);
	cerr << "ERROR: Not enough funds to withdraw " << withdraw_value << " from " << acc->GetFirstName() << " " << acc->GetLastName() << " " << FundName(fund) << endl;
	return false;
}
bool Transaction::Transfer(const int from_id, const int from_fund, const int to_id,
	const int to_fund, const int transfer_value, const BSTree& accounts)
{
	Account* from_acc = nullptr;
	accounts.Retrieve(from_id, from_acc);
	Fund* fund_one = &from_acc->GetFunds()[from_fund];
	Account* to_acc = nullptr;
	accounts.Retrieve(to_id, to_acc);
	Fund* fund_two = &to_acc->GetFunds()[to_fund];
	if (from_acc == nullptr)
	{ //from account not found
		cerr << "ERROR: Account " << from_id << " not found. Transferal refused." << endl;
		if (to_acc != nullptr)
		{
			TransactionData failure('T', from_id, from_fund, to_id, to_fund, transfer_value, true);
			fund_two->AddHistory(failure);
		}
		return false;
	}
	if (to_acc == nullptr)
	{//to account not found
		cerr << "ERROR: Account " << to_id << " not found. Transferal refused." << endl;
		TransactionData failure('T', from_id, from_fund, to_id, to_fund, transfer_value, true);
		fund_one->AddHistory(failure);
		return false;
	}

	//if accounts and funds are the same don't transfer anything
	if (from_acc == to_acc && from_fund == to_fund)
	{
		cerr << "ERROR: cannot transfer funds from the same account" << endl;
		TransactionData failure('T', from_id, from_fund, to_id, to_fund, transfer_value, true);
		fund_one->AddHistory(failure); //only need to add to one account because accounts are the same
		return false;
	}

	if (from_fund == 0)
	{
		if (from_acc == to_acc && to_fund == 1)
		{ //transfering to linked account
			if (fund_one->Withdraw(transfer_value))
			{
				fund_two->Deposit(transfer_value);
				TransactionData transfer_data('T', from_id, from_fund, to_id, to_fund, transfer_value, false);
				fund_one->AddHistory(transfer_data);
				fund_two->AddHistory(transfer_data);
				return true;
			}
			else
			{
				TransactionData transfer_data('T', from_id, from_fund, to_id, to_fund, transfer_value, true);
				fund_one->AddHistory(transfer_data);
				fund_two->AddHistory(transfer_data);
				cerr << "ERROR: Not enough funds to transfer " << transfer_value << " from " << from_acc->GetFirstName() << " " << from_acc->GetLastName() << " " << FundName(from_fund) << endl;
				return false;
			}
		}
		else
		{ //normal transfer // might need to use frunds from fund 1
			Fund* linked_fund = &from_acc->GetFunds()[1];
			if (fund_one->GetBalance() + linked_fund->GetBalance() >= transfer_value)
			{
				TransferHelper(from_acc, to_acc, transfer_value, from_fund, to_fund, 1);
				return true;
			}
			else
			{
				TransactionData transfer_data('T', from_id, from_fund, to_id, to_fund, transfer_value, true);
				fund_one->AddHistory(transfer_data);
				fund_two->AddHistory(transfer_data);
				cerr << "ERROR: Not enough funds to transfer " << transfer_value << " from " << from_acc->GetFirstName() << " " << from_acc->GetLastName() << " " << FundName(from_fund) << endl;
				return false;
			}

		}
	}
	if (from_fund == 1)
	{
		if (from_acc == to_acc && to_fund == 0)
		{ //transfering to linked account
			if (fund_one->Withdraw(transfer_value))
			{
				fund_two->Deposit(transfer_value);
				TransactionData transfer_data('T', from_id, from_fund, to_id, to_fund, transfer_value, false);
				fund_one->AddHistory(transfer_data);
				fund_two->AddHistory(transfer_data);
				return true;
			}
			else
			{
				TransactionData transfer_data('T', from_id, from_fund, to_id, to_fund, transfer_value, true);
				fund_one->AddHistory(transfer_data);
				fund_two->AddHistory(transfer_data);
				cerr << "ERROR: Not enough funds to transfer " << transfer_value << " from " << from_acc->GetFirstName() << " " << from_acc->GetLastName() << " " << FundName(from_fund) << endl;
				return false;
			}
		}
		else
		{//normal transfer using linked
			Fund* linked_fund = &from_acc->GetFunds()[0];
			if (fund_one->GetBalance() + linked_fund->GetBalance() >= transfer_value)
			{
				TransferHelper(from_acc, to_acc, transfer_value, from_fund, to_fund, 0);
				return true;
			}
			else
			{
				TransactionData transfer_data('T', from_id, from_fund, to_id, to_fund, transfer_value, true);
				fund_one->AddHistory(transfer_data);
				fund_two->AddHistory(transfer_data);
				cerr << "ERROR: Not enough funds to transfer " << transfer_value << " from " << from_acc->GetFirstName() << " " << from_acc->GetLastName() << " " << FundName(from_fund) << endl;
				return false;
			}
		}

	}
	if (from_fund == 2)
	{
		if (from_acc == to_acc && to_fund == 3)
		{ //transfering to linked account
			if (fund_one->Withdraw(transfer_value))
			{
				fund_two->Deposit(transfer_value);
				TransactionData transfer_data('T', from_id, from_fund, to_id, to_fund, transfer_value, false);
				fund_one->AddHistory(transfer_data);
				fund_two->AddHistory(transfer_data);
				return true;
			}
			else
			{
				TransactionData transfer_data('T', from_id, from_fund, to_id, to_fund, transfer_value, true);
				fund_one->AddHistory(transfer_data);
				fund_two->AddHistory(transfer_data);
				cerr << "ERROR: Not enough funds to transfer " << transfer_value << " from " << from_acc->GetFirstName() << " " << from_acc->GetLastName() << " " << FundName(from_fund) << endl;
				return false;
			}
		}
		else
		{//normal transfer using linked
			Fund* linked_fund = &from_acc->GetFunds()[3];
			if (fund_one->GetBalance() + linked_fund->GetBalance() >= transfer_value)
			{
				TransferHelper(from_acc, to_acc, transfer_value, from_fund, to_fund, 3);
				return true;
			}
			else
			{
				TransactionData transfer_data('T', from_id, from_fund, to_id, to_fund, transfer_value, true);
				fund_one->AddHistory(transfer_data);
				fund_two->AddHistory(transfer_data);
				cerr << "ERROR: Not enough funds to transfer " << transfer_value << " from " << from_acc->GetFirstName() << " " << from_acc->GetLastName() << " " << FundName(from_fund) << endl;
				return false;
			}
		}
	}
	if (from_fund == 3)
	{
		if (from_acc == to_acc && to_fund == 2)
		{ //transfering to linked account
			if (fund_one->Withdraw(transfer_value))
			{
				fund_two->Deposit(transfer_value);
				TransactionData transfer_data('T', from_id, from_fund, to_id, to_fund, transfer_value, false);
				fund_one->AddHistory(transfer_data);
				fund_two->AddHistory(transfer_data);
				return true;
			}
			else
			{
				TransactionData transfer_data('T', from_id, from_fund, to_id, to_fund, transfer_value, true);
				fund_one->AddHistory(transfer_data);
				fund_two->AddHistory(transfer_data);
				cerr << "ERROR: Not enough funds to transfer " << transfer_value << " from " << from_acc->GetFirstName() << " " << from_acc->GetLastName() << " " << FundName(from_fund) << endl;
				return false;
			}
		}
		else
		{//normal transfer using linked
			Fund* linked_fund = &from_acc->GetFunds()[2];
			if (fund_one->GetBalance() + linked_fund->GetBalance() >= transfer_value)
			{
				TransferHelper(from_acc, to_acc, transfer_value, from_fund, to_fund, 2);
				return true;
			}
			else
			{
				TransactionData transfer_data('T', from_id, from_fund, to_id, to_fund, transfer_value, true);
				fund_one->AddHistory(transfer_data);
				fund_two->AddHistory(transfer_data);
				cerr << "ERROR: Not enough funds to transfer " << transfer_value << " from " << from_acc->GetFirstName() << " " << from_acc->GetLastName() << " " << FundName(from_fund) << endl;
				return false;
			}
		}
	}

	//from fund is not a linked fund
	if (fund_one->GetBalance() <= transfer_value)
	{
		TransactionData failure('T', from_id, from_fund, to_id, to_fund, transfer_value, true);
		fund_one->AddHistory(failure);
		fund_two->AddHistory(failure);
		cerr << "ERROR: Not enough funds to transfer " << transfer_value << " from " << from_acc->GetFirstName() << " " << from_acc->GetLastName() << " " << FundName(from_fund) << endl;
		return false;
	}
	else if (fund_one->Withdraw(transfer_value))
	{
		fund_two->Deposit(transfer_value);
		TransactionData transfer_data('T', from_id, from_fund, to_id, to_fund, transfer_value, false);
		fund_one->AddHistory(transfer_data);
		fund_two->AddHistory(transfer_data);
		return true;
	}
	return false;
}

bool Transaction::DisplayAllHistories(const int id, const BSTree& accounts)
{
	Account* curr = nullptr;
	accounts.Retrieve(id, curr);
	if (curr == nullptr)
	{
		cerr << "ERROR: Account " << id << " not found. History cannot be shown." << endl;
		return false;
	}
	cout << "Transaction History for " << curr->GetFirstName() << " " << curr->GetLastName() << " by fund." << endl;
	for (int i = 0; i < kNumFunds; i++) {
		if (curr->GetFunds()[i].IsHistory())
		{
			switch (i)
			{
			case(0):
				cout << "Money Market: $";
				break;
			case(1):
				cout << "Prime Money Market: $";
				break;
			case(2):
				cout << "Long - Term Bond: $";
				break;
			case(3):
				cout << "Short - Term Bond: $";
				break;
			case(4):
				cout << "500 Index Fund: $";
				break;
			case(5):
				cout << "Capital Value Fund: $";
				break;
			case(6):
				cout << "Growth Equity Fund: $";
				break;
			case(7):
				cout << "Growth Index Fund: $";
				break;
			default:
				cerr << "INVALID FUND TYPE" << endl;
			}

			cout << curr->GetFunds()[i].GetBalance() << endl;
			curr->GetFunds()[i].DisplayHistory();
		}
	}
	cout << endl;
	return true;
}
bool Transaction::DisplayFundHistory(const int id, const int fund, const BSTree& accounts)
{
	Account* curr = nullptr;
	accounts.Retrieve(id, curr);
	if (curr == nullptr)
	{
		cerr << "ERROR: Account " << id << " not found. History cannot be shown." << endl;
		return false;
	}
	cout << "Transaction History for " << curr->GetFirstName() << " " << curr->GetLastName() << " ";
	switch (fund)
	{
	case(0):
		cout << "Money Market : $";
		break;
	case(1):
		cout << "Prime Money Market : $";
		break;
	case(2):
		cout << "Long - Term Bond : $";
		break;
	case(3):
		cout << "Short - Term Bond : $";
		break;
	case(4):
		cout << "500 Index Fund : $";
		break;
	case(5):
		cout << "Capital Value Fund : $";
		break;
	case(6):
		cout << "Growth Equity Fund : $";
		break;
	case(7):
		cout << "Growth Index Fund : $";
		break;
	default:
		cerr << "INVALID FUND TYPE" << endl;
	}
	if (fund < 0 || fund > kNumFunds) {
		//Invalid Fund Type
		return false;
	}
	cout << curr->GetFunds()[fund].GetBalance() << endl;
	curr->GetFunds()[fund].DisplayHistory();
	cout << endl;
	return true;
}
bool Transaction::Execute(BSTree& accounts)
{
	char transaction_type = commands_[0][0]; //first character of the first string
	int account_id = stoi(commands_[1]);

	switch (transaction_type)
	{
	case 'O':
		OpenAccount(account_id, commands_[3], commands_[2], accounts);
		break;
	case 'D':
		Deposit(account_id, stoi(commands_[2]), stoi(commands_[3]), accounts);
		break;
	case 'W':
		Withdraw(account_id, stoi(commands_[2]), stoi(commands_[3]), accounts);
		break;
	case 'T':
		Transfer(account_id, stoi(commands_[2]), stoi(commands_[3]), stoi(commands_[4]), stoi(commands_[5]), accounts);
		break;
	case 'A':
		DisplayAllHistories(account_id, accounts);
		break;
	case 'F':
		DisplayFundHistory(account_id, stoi(commands_[2]), accounts);
		break;
	default:
		cerr << "INVALID TRANSACTION TYPE";
		return false;
	}

	return true;
}

void Transaction::TransferHelper(Account* from_acc, Account* to_acc, const int withdraw_value, const int from_fund, const int to_fund, const int link_fund)
{
	Fund* linked_fund = &from_acc->GetFunds()[link_fund];
	Fund* fund_one = &from_acc->GetFunds()[from_fund];

	if (fund_one->GetBalance() >= withdraw_value)
	{
		fund_one->Withdraw(withdraw_value);
		to_acc->GetFunds()[to_fund].Deposit(withdraw_value);
		TransactionData new_history('T', from_acc->GetId(), from_fund, to_acc->GetId(), to_fund, fund_one->GetBalance(), false);
		fund_one->AddHistory(new_history);
		to_acc->GetFunds()[to_fund].AddHistory(new_history);
		return;
	}
	int remaining_withdraw = withdraw_value - fund_one->GetBalance();
	TransactionData new_history('T', from_acc->GetId(), from_fund, to_acc->GetId(), to_fund, fund_one->GetBalance(), false);
	fund_one->Withdraw(fund_one->GetBalance()); // empty fund completely
	fund_one->AddHistory(new_history);
	linked_fund->Withdraw(remaining_withdraw);
	TransactionData second_fund_history('T', from_acc->GetId(), from_fund, to_acc->GetId(), to_fund, remaining_withdraw, false);
	linked_fund->AddHistory(second_fund_history);
	to_acc->GetFunds()[to_fund].Deposit(withdraw_value);
	TransactionData to_account_history('T', from_acc->GetId(), from_fund, to_acc->GetId(), to_fund, withdraw_value, false);
	to_acc->GetFunds()[to_fund].AddHistory(to_account_history);
}
void Transaction::WithdrawHelper(Account* acc, const int fund, const int linked_fund, const int withdraw_value)
{
	Fund* curr_fund = &acc->GetFunds()[fund];
	int remaining_withdraw = withdraw_value - curr_fund->GetBalance();
	TransactionData new_history('W', acc->GetId(), fund, -1, -1, curr_fund->GetBalance(), false);
	curr_fund->Withdraw(curr_fund->GetBalance()); // empty fund completely
	curr_fund->AddHistory(new_history);
	curr_fund = &acc->GetFunds()[linked_fund];
	curr_fund->Withdraw(remaining_withdraw);
	TransactionData second_fund_history('W', acc->GetId(), fund, -1, -1, remaining_withdraw, false);
	curr_fund->AddHistory(second_fund_history);
}
string Transaction::FundName(const int fund_num)
{
	switch (fund_num)
	{
	case(0):
		return "Money Market";
		break;
	case(1):
		return "Prime Money Market";
		break;
	case(2):
		return "Long - Term Bond";
		break;
	case(3):
		return "Short - Term Bond";
		break;
	case(4):
		return "500 Index Fund";
		break;
	case(5):
		return "Capital Value Fund";
		break;
	case(6):
		return "Growth Equity Fund";
		break;
	case(7):
		return "Growth Index Fund";
		break;
	default:
		cerr << "INVALID FUND TYPE" << endl;
	}
	return "";
}