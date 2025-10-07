#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>

using namespace std;

struct stUser
{
	string UserName;
	string Password;
	int Permisions;
	bool MarkForDelete = false;
};

enum enMainMenuOptions
{
	eShowClientList = 1,
	eAddClient = 2,
	eDeleteClient = 3, 
    eUpdateClient = 4,
    eFindClient = 5,
    eTransactions = 6,
    eManageUsers = 7,
	eLogout = 8
};

enum enTransactionsMenuOptions
{
	eDeposit = 1,
	eWithdraw = 2,
	eTotalBalances = 3,
    eMainMenu = 4
};

enum enManageUsersOptions
{
	eListUsers = 1,
	eAddNewUser = 2,
	eDeleteUser = 3,
	eUpdateUser = 4,
	eFindUser = 5,
	MainMenu = 6
};

enum enMainMenuPermisions
{
	eAll = -1,
	pListClients = 1,
	pAddNewClient = 2,
	pDeleteClient = 4,
	pUpdateClient = 8,
	pFindClient = 16,
	pTransactions = 32,
	pManageUsers = 64,
};

void ShowMainMenu();

void ShowTransactionsMenuScreen();

void ShowManageUsersMenu();

bool CheckAccessPermission(enMainMenuPermisions Permission);

void Login();

string ClientsFileName = "Clients.txt";
string UsersFileName = "Users.txt";

stUser CurrentUser;

struct stClient
{
	string AccountNumber = "";
	string PinCode = "";
	string FullName = "";
	string PhoneNumber = "";
	double AccountBalance = 0;
	bool MarkForDelete = false;
};

void ShowAccessDeniedMessage()
{
	cout << "\n-----------------------------------\n";
	cout << "\n Access Denied!,\n You dont have permissions to do this, \n Please Contact Your Admin.\n";
	cout << "\n-----------------------------------\n";
}

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

string ReadClientAccountNumber()
{
	string AccountNumber;
	cout << "\nPlease Enter Account Number ? ";
	cin >> AccountNumber;
	return AccountNumber;
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

void GoBackToTheMainMenu()
{
	cout << "\nPress any key to go back to main menu ...";
	system("pause>0");
	ShowMainMenu();
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
	if (!CheckAccessPermission(enMainMenuPermisions::pAddNewClient))
	{
		ShowAccessDeniedMessage();
		return;
	}

	cout << "\n---------------------------------------\n";
	cout << "\t Add New Clients Screen ";
	cout << "\n---------------------------------------\n";

	AddNewClients();
}

void ShowDeleteClientScreen()
{
	if (!CheckAccessPermission(enMainMenuPermisions::pDeleteClient))
	{
		ShowAccessDeniedMessage();
		return;
	}

	cout << "\n---------------------------------------\n";
	cout << "\t Delete Client Screen ";
	cout << "\n---------------------------------------\n";

	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();
	DeleteClientByAccountNumber(AccountNumber, vClients);
}

void ShowUpdateClientScreen()
{
	if (!CheckAccessPermission(enMainMenuPermisions::pUpdateClient))
	{
		ShowAccessDeniedMessage();
		return;
	}

	cout << "\n---------------------------------------\n";
	cout << "\t Update Client Screen ";
	cout << "\n---------------------------------------\n";

	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();
	UpdateClientByAccountNumber(AccountNumber, vClients);
}

void ShowFindClientScreen()
{
	if (!CheckAccessPermission(enMainMenuPermisions::pFindClient))
	{
		ShowAccessDeniedMessage();
		return;
	}

	cout << "\n---------------------------------------\n";
	cout << "\t Find Client Screen ";
	cout << "\n---------------------------------------\n";

	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();
	stClient sClient;

	if (FindClientByAccountNumber(AccountNumber, vClients, sClient))
		PrintClientCard(sClient);
	else
		cout << "Client with account number [ " << AccountNumber << " ] is NOT found! " << endl;
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

short ReadTransacionsMenuOption()
{
	short Num;
	cout << "\nChoose what you want to do ? [ 1 - 4 ] :  ";
	cin >> Num;
	return Num;
}

void ShowTransactionsMenuScreen()
{
	if (!CheckAccessPermission(enMainMenuPermisions::pTransactions))
	{
		ShowAccessDeniedMessage();
		GoBackToTheMainMenu();
		return;
	}

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
	if (!CheckAccessPermission(enMainMenuPermisions::pListClients))
	{
		ShowAccessDeniedMessage();
		return;
	}

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

// user part ====================================================================================

string ConvertUserRecordToLine(stUser BankUser, string Seperator = "#//#")
{
	string stUserRecord = "";
	stUserRecord += BankUser.UserName + Seperator;
	stUserRecord += BankUser.Password + Seperator;
	stUserRecord += to_string(BankUser.Permisions);

	return stUserRecord;
}

void ShowLoginScreen()
{
	system("cls");
	cout << "\n-------------------------------------\n";
	cout << "\t Login Screen";
	cout << "\n-------------------------------------\n\n";
}

string ReadUserName()
{
	string Username;
	cout << "\nPlease Enter Account Number ? ";
	cin >> Username;
	return Username;
}

int ReadPermissionsToSet()
{
	int Permissions = 0;
	char Answer = 'n';

	cout << "\nDo you want to give full access: y/n ? ";
	cin >> Answer;

	if (Answer == 'y' || Answer == 'Y')
	{
		return -1;
	}

	cout << "\nDo you want to give access to : \n";
	
	cout << "\nShow Client List : y/n? ";
	cin >> Answer;

	if (Answer == 'y' || Answer == 'Y')
	{
		Permissions += enMainMenuPermisions::pListClients;
	}

	cout << "\nAdd New Client: y/n? ";
	cin >> Answer;

	if (Answer == 'y' || Answer == 'Y')
	{
		Permissions += enMainMenuPermisions::pAddNewClient;
	}

	cout << "\nDelete Client: y/n? ";
	cin >> Answer;

	if (Answer == 'y' || Answer == 'Y')
	{
		Permissions += enMainMenuPermisions::pDeleteClient;
	}

	cout << "\nUpdate Client: y/n? ";
	cin >> Answer;

	if (Answer == 'y' || Answer == 'Y')
	{
		Permissions += enMainMenuPermisions::pUpdateClient;
	}

	cout << "\nFind Client: y/n? ";
	cin >> Answer;

	if (Answer == 'y' || Answer == 'Y')
	{
		Permissions += enMainMenuPermisions::pFindClient;
	}

	cout << "\nShow Transactions: y/n? ";
	cin >> Answer;

	if (Answer == 'y' || Answer == 'Y')
	{
		Permissions += enMainMenuPermisions::pTransactions;
	}

	cout << "\nManage Users : y/n? ";
	cin >> Answer;

	if (Answer == 'y' || Answer == 'Y')
	{
		Permissions += enMainMenuPermisions::pManageUsers;
	}

	return Permissions;
}

void PrintUserCard(stUser User)
{
	cout << "\nThe Following are client details : \n";
	cout << "\n-------------------------------------------\n";
	cout << "\nUser Name   :  " << User.UserName;
	cout << "\nPassword        :  " << User.Password;
	cout << "\nPermision      :  " << User.Permisions;
	cout << "\n-------------------------------------------\n";
}

stUser ConvertUsersLineToRecord(string Line, string Seperator = "#//#")
{
	stUser vUser;
	vector<string> vUserData;
	
	vUserData = SplitString(Line, Seperator);

	vUser.UserName = vUserData[0];
	vUser.Password = vUserData[1];
	vUser.Permisions = stoi(vUserData[2]);

	return vUser;
}

bool IsUserExistByUserName(string UserName, string UsersFileName)
{
	vector <stUser> vUsers;
	fstream MyFile;

	MyFile.open(UsersFileName, ios::in); // read mode
	
	if (MyFile.is_open())
	{
		string Line;
		stUser User;

		while (getline(MyFile, Line))
		{
			User = ConvertUsersLineToRecord(Line);

			if (User.UserName == UserName)
			{
				MyFile.close();
				return true;
			}
			vUsers.push_back(User);
		}

		MyFile.close();
	}
	return false;
}

stUser ReadNewUser()
{
	stUser User;

	cout << "\nPlease Enter UserName : ";
	getline(cin >> ws, User.UserName);

	while (IsUserExistByUserName(User.UserName, UsersFileName))
	{
		cout << "\nUser with [ " << User.UserName << " ] is already exist, Enter another username ? ";
		getline(cin >> ws, User.UserName);
	}

	cout << "\nPlease Enter Your Password: ";
	getline(cin, User.Password);

	User.Permisions = ReadPermissionsToSet();

	return User;
}

bool MarkUserForDeleteByUserName(string username, vector <stUser>& vUser)
{
	for (stUser& U : vUser)
	{
		if (U.UserName == username)
		{
			U.MarkForDelete = true;
			return true;
		}
	}

	return false;
}

void AddUserLineToFile(string FileName, string stDataLine)
{
	fstream MyFile;

	MyFile.open(FileName, ios::out | ios::app);

	if (MyFile.is_open())
	{
		MyFile << stDataLine << endl;
		MyFile.close();
	}
}

void AddNewUser()
{
	stUser User;
	User = ReadNewUser();
	AddUserLineToFile(UsersFileName, ConvertUserRecordToLine(User));
}

void AddNewUsers()
{
	char AddMore = 'Y';

	do
	{
		//system("cls");
		cout << "\nAdding New User \n";
		AddNewUser();

		cout << "\nUser Added Successfully, do you want to add more users ? Y/N : ";
		cin >> AddMore;

	} while (toupper(AddMore) == 'Y');

}

void ShowAddNewUserScreen()
{
	system("cls");
	cout << "\n---------------------------------------------\n";
	cout << "\tAdd New User Screen" << endl;
	cout << "\n---------------------------------------------\n";

	AddNewUsers();
}

vector <stUser> LoadUsersDataFromFile(string FileName)
{
	vector <stUser> vUsers;

	fstream MyFile;
	MyFile.open(FileName, ios::in); // read mode

	if (MyFile.is_open())
	{
		string Line;
		stUser User;

		while (getline(MyFile, Line))
		{
			User = ConvertUsersLineToRecord(Line);
			vUsers.push_back(User);
		}

		MyFile.close();
	}

	return vUsers;
}

bool FindUserByUserName(string Username, vector <stUser> vUsers, stUser& User)
{
	for (stUser U : vUsers)
	{
		if (U.UserName == Username)
		{
			User = U;
			return true;
		}
	}

	return false;
}

bool FindUserByUserNameAndPassword(string Username, string Password, stUser& User)
{
	vector <stUser> vUsers = LoadUsersDataFromFile(UsersFileName);

	for (stUser U : vUsers)
	{
		if (U.UserName == Username && U.Password == Password)
		{
			User = U;
			return true;
		}
	}

	return false;
}

void GoBackToTheUsersMenu()
{
	cout << "\nPress any key to go back to main menu ...";
	system("pause>0");
	ShowManageUsersMenu();
}

short ReadManageUsersMenuOption()
{
	short choice;
	cout << "\nChoose what you want to do ? [ 1 - 6 ] :  ";
	cin >> choice;
	return choice;
}

void PrintUserRecordLine(stUser stUser)
{
	cout << "| " << left << setw(15) << stUser.UserName;
	cout << "| " << left << setw(10) << stUser.Password;
	cout << "| " << left << setw(36) << stUser.Permisions;
}

vector <stUser> SaveUsersDataToFile(string FileName, vector <stUser> vUsers)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out); // overwrite

	string DataLine = "";

	if (MyFile.is_open())
	{

		for (stUser U : vUsers)
		{
			if (U.MarkForDelete == false)
			{
				//we only write records that are not marked for delete.
				DataLine = ConvertUserRecordToLine(U);
				MyFile << DataLine << endl;
			}
		}


		MyFile.close();
	}
	return vUsers;
}

bool DeleteUserByUserName(string username, vector <stUser>& vUsers)
{
	stUser User;
	char answer = 'n';

	if (FindUserByUserName(username, vUsers, User))
	{
		PrintUserCard(User);

		cout << "\nAre you sure you want to delete the user card ? y/n : ";
		cin >> answer;

		if (answer == 'y' || answer == 'Y')
		{
			MarkUserForDeleteByUserName(username, vUsers);
			SaveUsersDataToFile(UsersFileName, vUsers);

			// Refresh Users : 
			vUsers = LoadUsersDataFromFile(UsersFileName);

			cout << "\nUser deleted successfully" << endl;
			return true;
		}
		else
		{
			cout << "\ndeletion cancelled." << endl;
		}
	}
	else
	{
		cout << "\nUser with username (" << username << ") is Not found" << endl;
		return false;
	}
}

stUser ChangeUserRecord(string username)
{
	stUser sUser;

	sUser.UserName = username;

	cout << "\nPlease Enter Your New Password : ";
	getline(cin >> ws, sUser.Password);

	return sUser;
}

bool UpdateUserByUserName(string username, vector <stUser>& vUsers)
{
	stUser User;
	char answer = 'n';

	if (FindUserByUserName(username, vUsers, User))
	{
		PrintUserCard(User);

		cout << "\n\nAre you sure you want to update the user card ? y/n : ";
		cin >> answer;

		if (answer == 'y' || answer == 'Y')
		{

			for (stUser& U : vUsers)
			{
				if (U.UserName == username)
				{
					U = ChangeUserRecord(username);
					break;
				}
			}

			SaveUsersDataToFile(UsersFileName, vUsers);

			cout << "\nUser Updated Successfully." << endl;
			return true; // to quit from the fundction.
		}
		else
		{
			cout << "\nEdition cancelled." << endl;
		}
	}
	else
	{
		cout << "\nUser with username (" << username << ") is Not found" << endl;
		return false;
	}
}

void ShowUpdateUserScreen()
{
	cout << "\n---------------------------------------\n";
	cout << "\t\t Update User Screen " << endl;
	cout << "\n---------------------------------------\n";

	string UserName = ReadUserName();
	vector <stUser> vUsersData = LoadUsersDataFromFile(UsersFileName);
	UpdateUserByUserName(UserName, vUsersData);
}

void ShowDeleteUserScreen()
{
	cout << "\n---------------------------------------\n";
	cout << "\t\t Delete User Screen " << endl;
	cout << "\n---------------------------------------\n";

	string UserName = ReadUserName();
	vector <stUser> vUsersData = LoadUsersDataFromFile(UsersFileName);
	DeleteUserByUserName(UserName, vUsersData);
}

void ShowFindUserScreen()
{
	cout << "\n---------------------------------------\n";
	cout << "\t\t Find User Screen " << endl;
	cout << "\n---------------------------------------\n";

	stUser User;
	string UserName = ReadUserName();
	vector <stUser> vUsersData = LoadUsersDataFromFile(UsersFileName);
	FindUserByUserName(UserName, vUsersData, User);
	PrintUserCard(User);
}

void ShowListUsersScreen()
{
	vector <stUser> vUsersData = LoadUsersDataFromFile(UsersFileName);

	cout << "\n\t\t\t\t\t Users List (" << vUsersData.size() << ") User(s).\n";

	cout << "\n---------------------------------------------------------";
	cout << "-------------------------------------------- \n\n";
	cout << "| " << left << setw(15) << "User Name ";
	cout << "| " << left << setw(10) << "Password ";
	cout << "| " << left << setw(36) << "Permision";
	cout << "\n\n---------------------------------------------------------";
	cout << "-------------------------------------------- \n\n";

	if (vUsersData.size() == 0)
	{
		cout << "\t\t\tNo User Avilable In The System!" << endl;
	}
	else
	{
		for (stUser& User : vUsersData) // for each structur in the vector.
		{
			PrintUserRecordLine(User);
			cout << endl;
		}
	}

	cout << "\n---------------------------------------------------------";
	cout << "-------------------------------------------- \n";
}

void PerformManageUsersOption(enManageUsersOptions ManageUsersOptions)
{
	switch (ManageUsersOptions)
	{
	case eListUsers:
		system("cls");
		ShowListUsersScreen();
		GoBackToTheUsersMenu();
		break;

	case eAddNewUser:
		system("cls");
		ShowAddNewUserScreen();
		GoBackToTheUsersMenu();
		break;

	case eDeleteUser:
		system("cls");
		ShowDeleteUserScreen();
		GoBackToTheUsersMenu();
		break;

	case eUpdateUser:
		system("cls");
		ShowUpdateUserScreen();
		GoBackToTheUsersMenu();
		break;

	case eFindUser:
		system("cls");
		ShowFindUserScreen();
		GoBackToTheUsersMenu();
		break;

	case MainMenu:
		system("cls");
		ShowMainMenu();
		break;

	default:
		cout << "invalid option" << endl;
		break;
	}
}

void ShowManageUsersMenu()
{
	if (!CheckAccessPermission(enMainMenuPermisions::pManageUsers))
	{
		ShowAccessDeniedMessage();
		GoBackToTheMainMenu();
		return;
	}

	system("cls");
	cout << "======================================================\n" << endl;
	cout << "\t\tManage Users Menu Screen\n" << endl;
	cout << "======================================================\n" << endl;
	cout << "\t\t [1] List Users." << endl;
	cout << "\t\t [2] Add New User." << endl;
	cout << "\t\t [3] Delete User." << endl;
	cout << "\t\t [4] Update User." << endl;
	cout << "\t\t [5] Find User." << endl;
	cout << "\t\t [6] Main Menu." << endl;
	cout << "\n======================================================\n" << endl;

	PerformManageUsersOption((enManageUsersOptions)ReadManageUsersMenuOption());
}

bool CheckAccessPermission(enMainMenuPermisions Permission)
{
	if (CurrentUser.Permisions == enMainMenuPermisions::eAll)
		return true;

	if ((Permission & CurrentUser.Permisions) == Permission)
		return true;
	else
		return false;
}

bool LoadUserInfo(string Username, string Password)
{
	if (FindUserByUserNameAndPassword(Username, Password, CurrentUser))
		return true;
	else
		return false;
}

void PerformMainMenuOption(enMainMenuOptions MainMenuOptions)
{

	switch (MainMenuOptions)
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

	case enMainMenuOptions::eManageUsers: // Manage users function to do.
		system("cls");
		ShowManageUsersMenu();
		break;

	case enMainMenuOptions::eLogout:
		system("cls");
		//ShowMainMenu();
		Login();
		break;

	default:
		cout << "invalid option" << endl;
		break;
	}
}

short ReadMainMenuOption()
{
	short Choise;
	cout << "\nChoose what you want to do ? [ 1 - 8 ] :  ";
	cin >> Choise;
	return Choise;
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
	cout << "\t\t [5] Find A Clinet." << endl;
	cout << "\t\t [6] Transactions." << endl;
	cout << "\t\t [7] Manage Users." << endl;
	cout << "\t\t [8] Logout." << endl;
	cout << "\n======================================================\n" << endl;

	PerformMainMenuOption((enMainMenuOptions)ReadMainMenuOption());
}

void Login()
{
	bool LoginFaild = false;

	string Username, Password;

	do
	{
		ShowLoginScreen();

		if (LoginFaild)
		{
			cout << "\nInvalid Username/Password!" << endl;
		}

		cout << "Enter Username : ";
		cin >> Username;
		
		cout << "Enter Password : ";
		cin >> Password;

		LoginFaild = !LoadUserInfo(Username, Password);

	} while (LoginFaild);

	ShowMainMenu();
}


int main()
{
	Login();

	//system("pause>0");

	return 0;
}
