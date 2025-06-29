#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>

using namespace std;

enum enMainMenuOptions
{
	eShowClientList = 1,
	eAddClient = 2,
	eDeleteClient = 3, 
    eUpdateClient = 4,
    eFindClient = 5,
    eTransactions = 6,
	eExit = 7
};

enum enTransactionsMenuOptions
{
	eDeposit = 1,
	eWithdraw = 2,
	eTotalBalances = 3,
    eMainMenu = 4
};

void ShowMainMenu();

void ShowTransactionsMenuScreen();

string ClientsFileName = "Clients.txt";

short ReadMainMenuOption()
{
	short Num;
	cout << "\nChoose what you want to do ? [ 1 - 7 ] :  ";
	cin >> Num;
	return Num;
}

short ReadTransacionsMenuOption()
{
	short Num;
	cout << "\nChoose what you want to do ? [ 1 - 4 ] :  ";
	cin >> Num;
	return Num;
}

struct stClient
{
	string AccountNumber = "";
	string PinCode = "";
	string FullName = "";
	string PhoneNumber = "";
	double AccountBalance = 0;
	bool MarkForDelete = false;
};

vector<string> SplitString(string S1, string Delim = "#//#")
{
	vector<string> vString;
	short pos = 0;
	string sWord; // define a string variable
	// use find() function to get the position of the delimiters
	while ((pos = S1.find(Delim)) != std::string::npos)
	{
		sWord = S1.substr(0, pos); // store the word
		if (sWord != "")
		{
			vString.push_back(sWord);
		}
		S1.erase(0, pos + Delim.length()); /* erase() until
		positon and move to next word. */
	}
	if (S1 != "")
	{
		vString.push_back(S1); // it adds last word of the string.
	}
	return vString;
}

stClient ConvertLineToRecord(string stLine, string Seperator = "#//#")
{
	stClient sClient;
	vector<string> vClientData = SplitString(stLine, Seperator);

	sClient.AccountNumber = vClientData[0];
	sClient.PinCode = vClientData[1];
	sClient.FullName = vClientData[2];
	sClient.PhoneNumber = vClientData[3];
	sClient.AccountBalance = stod(vClientData[4]);

	return sClient;
}

bool IsClientExistByAccountNumber(string AccountNumber, string ClientsFileName)
{
	vector <stClient> vClients;
	fstream MyFile;

	MyFile.open(ClientsFileName, ios::in); // read mode

	if (MyFile.is_open())
	{
		string Line;
		stClient Client;

		while (getline(MyFile, Line))
		{
			Client = ConvertLineToRecord(Line);
			
			if (Client.AccountNumber == AccountNumber)
			{
				MyFile.close();
				return true;
			}
			vClients.push_back(Client);
		}

		MyFile.close();
	}
	return false;
}

stClient ReadNewClient()
{
	stClient sClient;

	cout << "\nPlease Enter Acount Numebr : ";
	getline(cin >> ws, sClient.AccountNumber);

	while (IsClientExistByAccountNumber(sClient.AccountNumber,ClientsFileName))
	{
		cout << "\nClient with [ " << sClient.AccountNumber << " ] is already exist, Enter another account number ? ";
		getline(cin >> ws, sClient.AccountNumber);
	}

	cout << "\nPlease Enter Your Pin Code : ";
	getline(cin, sClient.PinCode);

	cout << "\nPlease Enter Your Full Name : ";
	getline(cin, sClient.FullName);

	cout << "\nPlease Enter Your Phone Number : ";
	getline(cin, sClient.PhoneNumber);

	cout << "\nPlease Enter Your Account Balance : ";
	cin >> sClient.AccountBalance;

	return sClient;
}


string ConvertRecordToLine(stClient BankClient, string Seperator = "#//#")
{
	string S1 = "";
	S1 += BankClient.AccountNumber + Seperator;
	S1 += BankClient.PinCode + Seperator;
	S1 += BankClient.FullName + Seperator;
	S1 += BankClient.PhoneNumber + Seperator;
	S1 += to_string(BankClient.AccountBalance);

	return S1;
}


string ReadClientAccountNumber()
{
	string AccountNumber;
	cout << "\nPlease Enter Account Number ? ";
	cin >> AccountNumber;
	return AccountNumber;
}


vector <stClient> LoadClientsDataFromFile(string FileName)
{
	vector <stClient> vClients;

	fstream MyFile;
	MyFile.open(FileName, ios::in); // read mode

	if (MyFile.is_open())
	{
		string Line; // empty
		stClient Client;

		while (getline(MyFile, Line)) // get the line & store it in the Line variable.
		{
			Client = ConvertLineToRecord(Line);
			vClients.push_back(Client);
		}

		MyFile.close();
	}

	return vClients;
}

void PrintClientCard(stClient sClient)
{
	cout << "\nThe Following are client details : \n";
	cout << "\n-------------------------------------------\n";
	cout << "\nAcount Numebr   :  " << sClient.AccountNumber;
	cout << "\nPin Code        :  " << sClient.PinCode;
	cout << "\nFull Name       :  " << sClient.FullName;
	cout << "\nPhone Number    :  " << sClient.PhoneNumber;
	cout << "\nAccount Balance :  " << sClient.AccountBalance;
	cout << "\n-------------------------------------------\n";
}


bool FindClientByAccountNumber(string AccountNumber, vector <stClient> vClient, stClient& Client)
{

	for (stClient C : vClient)
	{
		if (C.AccountNumber == AccountNumber)
		{
			Client = C;
			return true;
		}
	}

	return false;
}


vector <stClient> SaveClientsDataToFile(string FileName, vector <stClient> vClients)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out); // overwrite

	string DataLine = "";

	if (MyFile.is_open())
	{

		for (stClient C : vClients)
		{
			if (C.MarkForDelete == false)
			{
				//we only write records that are not marked for delete.
				DataLine = ConvertRecordToLine(C);
				MyFile << DataLine << endl;
			}
		}


		MyFile.close();
	}
	return vClients;
}

void AddClientLineToFile(string FileName, string stDataLine)
{
	fstream MyFile;

	MyFile.open(FileName, ios::out | ios::app);

	if (MyFile.is_open())
	{
		MyFile << stDataLine << endl;
		MyFile.close();
	}
}

void AddNewClient()
{
	stClient Client;
	Client = ReadNewClient();
	AddClientLineToFile(ClientsFileName, ConvertRecordToLine(Client));
}

void AddNewClients()
{
	char AddMore = 'Y';
	do
	{
		//system("cls");
		cout << "\nAdding New Client \n";
		AddNewClient();

		cout << "\nDo you want to add more clients ? Y/N : ";
		cin >> AddMore;

	} while (toupper(AddMore) == 'Y');
}

stClient ChangeClientRecord(string AccountNumber)
{
	stClient sClient;

	sClient.AccountNumber = AccountNumber;

	cout << "\nPlease Enter Your Pin Code : ";
	getline(cin >> ws, sClient.PinCode);

	cout << "\nPlease Enter Your Full Name : ";
	getline(cin, sClient.FullName);

	cout << "\nPlease Enter Your Phone Number : ";
	getline(cin, sClient.PhoneNumber);

	cout << "\nPlease Enter Your Account Balance : ";
	cin >> sClient.AccountBalance;

	return sClient;
}


bool UpdateClientByAccountNumber(string AccountNumber, vector <stClient>& vClients)
{
	stClient Client;
	char answer = 'n';

	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client);

		cout << "\n\nAre you sure you want to update the client card ? y/n : ";
		cin >> answer;

		if (answer == 'y' || answer == 'Y')
		{

			for (stClient& C : vClients)
			{
				if (C.AccountNumber == AccountNumber)
				{
					C = ChangeClientRecord(AccountNumber);
					break;
				}
			}

			SaveClientsDataToFile(ClientsFileName, vClients);

			cout << "\nClient Updated Successfully." << endl;
			return true; // to quit from the fundction.
		}
		else
		{
			cout << "\nEdition cancelled." << endl;
		}
	}
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not found" << endl;
		return false;
	}
}


bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <stClient>& vClient)
{
	for (stClient& C : vClient)
	{
		if (C.AccountNumber == AccountNumber)
		{
			C.MarkForDelete = true;
			return true;
		}
	}

	return false;
}


bool DeleteClientByAccountNumber(string AccountNumber, vector <stClient>& vClients)
{
	stClient Client;
	char answer = 'n';

	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client);

		cout << "\nAre you sure you want to delete the client card ? y/n : ";
		cin >> answer;

		if (answer == 'y' || answer == 'Y')
		{
			MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
			SaveClientsDataToFile(ClientsFileName, vClients);

			// Refresh Clients : 
			vClients = LoadClientsDataFromFile(ClientsFileName);

			cout << "\nclient deleted successfully" << endl;
			return true;
		}
		else
		{
			cout << "\ndeletion cancelled." << endl;
		}
	}
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not found" << endl;
		return false;
	}
}


bool DepositTheAmountByAccountNumber(string AccountNumebr,double Amount, vector <stClient> & vClients)
{
	char AddAmount = 'Y';

	cout << "\nAre you sure you want to perform this transaction : y/n ?  ";
	cin >> AddAmount;


	if (toupper(AddAmount) == 'Y')
	{

		for (stClient& C : vClients)
		{
			if (C.AccountNumber == AccountNumebr)
			{
				C.AccountBalance += Amount;
				SaveClientsDataToFile(ClientsFileName, vClients);
				cout << "\nDone successfully. New balance is : " << C.AccountBalance << endl;
				return true;
			}
		}

		return false;
	}
}


void DisplayDepositScreen()
{
	cout << "\n---------------------------------------\n";
	cout << "\t  Deposit Screen ";
	cout << "\n---------------------------------------\n";

	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();
	stClient sClient;

	while (!FindClientByAccountNumber(AccountNumber, vClients, sClient))
	{
		cout << "\nClient with account number [ " << AccountNumber << " ] is NOT found! " << endl;
		AccountNumber = ReadClientAccountNumber();
	}

	PrintClientCard(sClient);

	double Amount = 0;
	cout << "Please enter deposit amount ? ";
	cin >> Amount;

	DepositTheAmountByAccountNumber(AccountNumber, Amount, vClients);

}

void DisplayWithdrawScreen()
{
	cout << "\n---------------------------------------\n";
	cout << "\t  Withdraw Screen ";
	cout << "\n---------------------------------------\n";

	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();
	stClient sClient;

	while (!FindClientByAccountNumber(AccountNumber, vClients, sClient))
	{
		cout << "Client with account number [ " << AccountNumber << " ] is NOT found! " << endl;
		AccountNumber = ReadClientAccountNumber();
	}

	PrintClientCard(sClient);

	double Amount = 0;
	cout << "Please enter withdraw amount ? ";
	cin >> Amount;

	// validate that the amount does not exceeds the balance.
	while (Amount > sClient.AccountBalance)
	{
		cout << "\nAmount exceeds the balance , you can withdraw up to : " << sClient.AccountBalance << endl;
		cout << "Please enter another amount ? ";
		cin >> Amount;
	}

	DepositTheAmountByAccountNumber(AccountNumber, Amount * -1, vClients);
}


void PrintClientTotalBalancesLine(stClient sClient)
{
	cout << "| " << left << setw(15) << sClient.AccountNumber;
	cout << "| " << left << setw(36) << sClient.FullName;
	cout << "| \t" << left << setw(12) << sClient.AccountBalance;
}

void DisplayAllClientsBalances()
{
	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);

	system("cls");

	cout << "\n\t\t\t\t\t Balances List (" << vClients.size() << ") Client(s).\n";

	cout << "\n---------------------------------------------------------";
	cout << "-------------------------------------------- \n\n";
	cout << "| " << left << setw(15) << "Acount Numebr ";
	cout << "| " << left << setw(36) << "CLient Name ";
	cout << "| " << left << setw(12) << "Account Balance ";
	cout << "\n\n---------------------------------------------------------";
	cout << "-------------------------------------------- \n\n";

	double TotalBalances = 0;

	if (vClients.size() == 0)
	{
		cout << "\t\t\tNo Clients Avilable In The System!" << endl;
	}
	else
	{
		for (stClient& Client : vClients) // for each structur in the vector.
		{
			PrintClientTotalBalancesLine(Client);
			TotalBalances += Client.AccountBalance;
			cout << endl;
		}
	}

	cout << "\n---------------------------------------------------------";
	cout << "-------------------------------------------- \n";

	cout << "\t\t\t\tTotal Balances : " << TotalBalances << endl;
}

void DisplayTotalBalancesScreen()
{
	DisplayAllClientsBalances();
}

void GoBackToTransactionsMenu()
{
	cout << "\nPress any key to go back to transaction menu ...";
	system("pause>0");
	ShowTransactionsMenuScreen();
}

void PerformTransactionsMenuOption(enTransactionsMenuOptions TransactionsMenuOptions)
{
	switch (TransactionsMenuOptions)
	{
	case eDeposit:
		system("cls");
		DisplayDepositScreen();
		GoBackToTransactionsMenu();
		break;

	case eWithdraw:
		system("cls");
		DisplayWithdrawScreen();
		GoBackToTransactionsMenu();
		break;

	case eTotalBalances:
		system("cls");
		DisplayTotalBalancesScreen();
		GoBackToTransactionsMenu();
		break;

	case eMainMenu:
		system("cls");
		ShowMainMenu();
		break;

	default:
		cout << "invalid option" << endl;
		break;
	}
}

void PrintClientRecordLine(stClient sClient)
{
	cout << "| " << left << setw(15) << sClient.AccountNumber;
	cout << "| " << left << setw(10) << sClient.PinCode;
	cout << "| " << left << setw(36) << sClient.FullName;
	cout << "| " << left << setw(14) << sClient.PhoneNumber;
	cout << "| \t" << left << setw(12) << sClient.AccountBalance;
}

void ShowAddNewClientsScreen()
{
	cout << "\n---------------------------------------\n";
	cout << "\t\t Add New Clients Screen " << endl;
	cout << "\n---------------------------------------\n";

	AddNewClients();
}

void ShowDeleteClientScreen()
{
	cout << "\n---------------------------------------\n";
	cout << "\t\t Delete Client Screen " << endl;
	cout << "\n---------------------------------------\n";

	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();
	DeleteClientByAccountNumber(AccountNumber, vClients);
}

void ShowUpdateClientScreen()
{
	cout << "\n---------------------------------------\n";
	cout << "\t\t Update Client Screen " << endl;
	cout << "\n---------------------------------------\n";

	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();
	UpdateClientByAccountNumber(AccountNumber, vClients);
}

void ShowFindClientScreen()
{
	cout << "\n---------------------------------------\n";
	cout << "\t\t Update Client Screen " << endl;
	cout << "\n---------------------------------------\n";

	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();
	stClient sClient;

	if (FindClientByAccountNumber(AccountNumber, vClients, sClient))
		PrintClientCard(sClient);
	else
		cout << "Client with account number [ " << AccountNumber << " ] is NOT found! " << endl;
}

void ShowTransactionsMenuScreen()
{
	system("cls");
	cout << "\n======================================================\n" << endl;
	cout << "\t\t Transactions Menu Screen " << endl;
	cout << "\n======================================================\n" << endl;
	cout << "\t\t [1] Deposit. " << endl;
	cout << "\t\t [2] Withdraw. " << endl;
	cout << "\t\t [3] Total Balances. " << endl;
	cout << "\t\t [4] Main Menu. " << endl;
	cout << "\n======================================================\n" << endl;
	PerformTransactionsMenuOption((enTransactionsMenuOptions)ReadTransacionsMenuOption());
}

void ShowAllClientsScreen()
{
	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);

	cout << "\n\t\t\t\t\t Client List (" << vClients.size() << ") Client(s).\n";

	cout << "\n---------------------------------------------------------";
	cout << "-------------------------------------------- \n\n";
	cout << "| " << left << setw(15) << "Acount Numebr ";
	cout << "| " << left << setw(10) << "Pin Code ";
	cout << "| " << left << setw(36) << "CLient Name ";
	cout << "| " << left << setw(14) << "Phone Number ";
	cout << "| " << left << setw(12) << "Account Balance ";
	cout << "\n\n---------------------------------------------------------";
	cout << "-------------------------------------------- \n\n";

	if (vClients.size() == 0)
	{
		cout << "\t\t\tNo Clients Avilable In The System!" << endl;
	}
	else
	{
		for (stClient& Client : vClients) // for each structur in the vector.
		{
			PrintClientRecordLine(Client);
			cout << endl;
		}
	}
		
	cout << "\n---------------------------------------------------------";
	cout << "-------------------------------------------- \n";
}

void ShowEndScreen()
{
	cout << "\n---------------------------------------\n\n";
	cout << "\t Program Ends :-) " << endl;
	cout << "\n---------------------------------------\n";
}

void GoBackToTheMainMenu()
{
	cout << "\nPress any key to go back to main menu ...";
	system("pause>0");
	ShowMainMenu();
}



void PerformMainMenuOption(enMainMenuOptions enMainMenuOptions)
{
	
	switch (enMainMenuOptions)
	{
	case enMainMenuOptions::eShowClientList:
		system("cls");
		ShowAllClientsScreen();
		GoBackToTheMainMenu();
		break;

	case enMainMenuOptions::eAddClient:
		system("cls");
		ShowAddNewClientsScreen();
		GoBackToTheMainMenu();
		break;

	case enMainMenuOptions::eDeleteClient:
		system("cls");
		ShowDeleteClientScreen();
		GoBackToTheMainMenu();
		break;

	case enMainMenuOptions::eUpdateClient:
		system("cls");
		ShowUpdateClientScreen();
		GoBackToTheMainMenu();
		break;

	case enMainMenuOptions::eFindClient:
		system("cls");
		ShowFindClientScreen();
		GoBackToTheMainMenu();
		break;

	case enMainMenuOptions::eTransactions:
		system("cls");
		ShowTransactionsMenuScreen();
		break;

	case enMainMenuOptions::eExit:
		system("cls");
		ShowEndScreen();
		break;

	default:
		cout << "invalid option" << endl;
		break;
	}
}

void ShowMainMenu()
{
	system("cls");
    cout << "\n======================================================\n" << endl;
    cout << "\t\t Main Menu Screen " << endl;
    cout << "\n======================================================\n" << endl;
    cout << "\t\t [1] Show Client List. " << endl;
    cout << "\t\t [2] Add A New Client. " << endl;
    cout << "\t\t [3] Delete A Client. " << endl;
    cout << "\t\t [4] Update A Client Info. " << endl;
    cout << "\t\t [5] Find A Clinet " << endl;
    cout << "\t\t [6] Transactions " << endl;
    cout << "\t\t [7] Exit. " << endl;
    cout << "\n======================================================\n" << endl;
	PerformMainMenuOption((enMainMenuOptions)ReadMainMenuOption());
}


int main()
{
	ShowMainMenu();
	system("pause>0");

	return 0;
}
