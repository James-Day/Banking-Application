#ifndef TRANSACTION_H_
#define TRANSACTION_H_
#include "transaction_data.h"
#include "fund.h"
#include "account.h"
#include "bstree.h"
#include <iostream>
#include <string>
using namespace std;
const int kNumCommands = 6; 

class Transaction
{
public:
	Transaction(string commands[kNumCommands]);
	bool OpenAccount(const int id, const string first_name, const string last_name, BSTree& accounts); //uses BST insert														
	bool Deposit(const int id, const int fund, const int deposit_value, const BSTree& accounts); //uses BST retrieve													
	bool Withdraw(const int id, const int fund, const int withdraw_value, const BSTree& accounts); //uses BST retrieve													
	bool Transfer(const int from_id, const int from_fund, const int to_id,
		const int to_fund, const int transfer_value, const BSTree& accounts); //uses BST retrieve	
	bool DisplayAllHistories(const int id, const BSTree& accounts);
	bool DisplayFundHistory(const int id, const int fund, const BSTree& accounts);
	bool Execute(BSTree& accounts);
private:
	string commands_[kNumCommands];

	void TransferHelper(Account* from_acc, Account* link_acc, const int withdraw_value, const int from_fund, const int to_fund, const int link_fund);
	void WithdrawHelper(Account* acc, const int fund, const int linked_fund, const int withdraw_value);
	string FundName(const int fund_num);
};
#endif

