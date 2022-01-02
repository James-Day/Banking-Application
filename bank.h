#ifndef BANK_H_
#define BANK_H_
#include "bstree.h"
#include "transaction.h"
#include <fstream>
#include <sstream>
#include <queue>
class Bank
{
public:
	Bank();
	Bank(std::string file, std::string name);
	~Bank();
	void ReadTransactions(std::string file);
	void ExecuteTransactions();
	friend ostream& operator<< (ostream& stream, const Bank& rhs);
	BSTree* GetAccounts();
private:
	BSTree accounts_;
	std::string name_;
	std::queue<Transaction> queue_;
};
#endif

