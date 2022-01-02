#include "bank.h"
#include <crtdbg.h>
using namespace std;
int main(int argc, char* argv[]) {
	{
		Bank Chase(argv[1], "Chase"); // calls ReadTransactions
		Chase.ExecuteTransactions();
		cout << Chase << endl;
	}
	_CrtDumpMemoryLeaks();
}