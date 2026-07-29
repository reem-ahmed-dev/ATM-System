#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;
const string ClientsFileName = "MyFile.txt";
const string UsersFileName = "Users.txt";
void PrintTheMeunScreen();
void Login();
enum ENMainMenuOptions { eQuickWithdraw = 1, eNormalWithdraw = 2, eDeposit = 3, eCheckBalance = 4, eLogout = 5};
enum ENQuickWithDrawOptions { Twenty = 1, Fifty = 2, OneHunderd = 3, TwoHunderd = 4, FourHunderd = 5, SixHunderd = 6
	, EightHunderd=7 , OneThousand=8, Exit=9};

struct sClient
{
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	double AccountBalance;
	bool MarkForDelete = false;
};
sClient CurrentClient;

vector<string> SplitString(string S1, string Delim)
{
	vector<string> vString;
	int pos = 0;
	string sWord;
	while ((pos = S1.find(Delim)) != std::string::npos)
	{
		sWord = S1.substr(0, pos);
		if (sWord != "")
		{
			vString.push_back(sWord);
		}
		S1.erase(0, pos + Delim.length());
	}
	if (S1 != "")
	{
		vString.push_back(S1);
	}
	return vString;
}

sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{
	sClient Client;
	vector<string> vClientData;
	vClientData = SplitString(Line, Seperator);
	Client.AccountNumber = vClientData[0];
	Client.PinCode = vClientData[1];
	Client.Name = vClientData[2];
	Client.Phone = vClientData[3];
	Client.AccountBalance = stod(vClientData[4]);
	return Client;
}

vector <sClient> LoadCleintsDataFromFile(string FileName)
{
	vector <sClient> vClients;
	fstream MyFile;
	MyFile.open(FileName, ios::in);
	if (MyFile.is_open())
	{
		string Line;
		sClient Client;
		while (getline(MyFile, Line))
		{
			Client = ConvertLinetoRecord(Line);
			vClients.push_back(Client);
		}
		MyFile.close();
	}
	return vClients;
}

int AskForTheChoice()
{
	int Number;
	cout << "Choose what do you want to do? [1 to 5]? ";
	cin >> Number;
	return Number;
}

int AskForTheChoiceQuickWithdraw()
{
	int Number;
	cout << "Choose what do you want to do? [1 to 9]? ";
	cin >> Number;
	return Number;
}

string ConvertRecordToLineClient(sClient Client, string Seperator = "#//#")
{
	string stClientRecord = "";
	stClientRecord += Client.AccountNumber + Seperator;
	stClientRecord += Client.PinCode + Seperator;
	stClientRecord += Client.Name + Seperator;
	stClientRecord += Client.Phone + Seperator;
	stClientRecord += to_string(Client.AccountBalance);
	return stClientRecord;
}

void AddDataLineToFile(string FileName, string stDataLine)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out | ios::app);
	if (MyFile.is_open())
	{
		MyFile << stDataLine << endl;
		MyFile.close();
	}
}

bool FindClientByAccountNumberAndPINCode(string Accountnumber, string PINCode, sClient& Client)
{
	vector<sClient> vClient = LoadCleintsDataFromFile(ClientsFileName);
	for (sClient C : vClient)
	{
		if (C.AccountNumber == Accountnumber && C.PinCode == PINCode)
		{
			Client = C;
			return true;
		}
	}
	return false;
}

vector <sClient> SaveCleintsDataToFile(string FileName, vector<sClient> vClients)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);
	string DataLine;
	if (MyFile.is_open())
	{
		for (sClient C : vClients)
		{
			if (C.MarkForDelete == false)
			{
				DataLine = ConvertRecordToLineClient(C);
				MyFile << DataLine << endl;
			}
		}
		MyFile.close();
	}
	return vClients;
}

void ShowLogoutScreen()
{
	system("cls");
	Login();
}

void ReturnToMenuScreen()
{
	cout << "Press any key to back to menu screen ...";
	system("pause>0");
	PrintTheMeunScreen();
}

bool AddDepositByAccontNumber(vector <sClient>& vClients)
{
	sClient Client;
	double DepositAmount = 0;
	char Answer;
	cout << "Please enter Positive amount ?";
	cin >> DepositAmount;

	cout << "\n\nAre you sure you want do this transaction ? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
	{
		CurrentClient.AccountBalance += DepositAmount;
		for (sClient C : vClients)
		{
			if (C.AccountNumber == CurrentClient.AccountNumber)
			{
				C.AccountBalance = CurrentClient.AccountBalance;
				break;
			}
		}
		SaveCleintsDataToFile(ClientsFileName, vClients);
		cout << "\n\nDone Successfully ";
		cout << ". Client Balance :" << CurrentClient.AccountBalance << endl;
		return true;
	}
	return false;
}

void ShowDepositScreen()
{
	cout << "===============================================\n";
	cout << "\tDeposit screen : \n";
	cout << "===============================================\n";

	vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
	AddDepositByAccontNumber(vClients);
}

bool AddWithDrawByAccontNumber(vector <sClient>& vClients)
{
	int WithDrawAmount = 0;
	char Answer;

	cout << "Please enter an amount multiple of 5s ?";
	cin >> WithDrawAmount;

	while (WithDrawAmount%5!=0)
	{
		cout << "Please enter an amount multiple of 5s ?";
		cin >> WithDrawAmount;
	}

	if (CurrentClient.AccountBalance < WithDrawAmount)
	{
		cout << "Amount Exceeds your balance , make another choice ";
		ReturnToMenuScreen();
		return false;
	}

	cout << "\n\nAre you sure you want do this transaction ? y/n? ";
	cin >> Answer;

	if (Answer == 'y' || Answer == 'Y')
	{
		CurrentClient.AccountBalance -=WithDrawAmount;
		for (sClient C : vClients)
		{
			if (C.AccountNumber == CurrentClient.AccountNumber)
			{
				C.AccountBalance = CurrentClient.AccountBalance;
				break;
			}
		}
		SaveCleintsDataToFile(ClientsFileName, vClients);
		cout << "\n\nDone Successfully ";
		cout << ". Client Balance :" << CurrentClient.AccountBalance << endl;
		return true;
	}
	return false;
}

void ShowCheckBalanceScreen()
{
	vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
	cout << "===============================================\n";
	cout << "\tCheck Balance screen : \n";
	cout << "===============================================\n";
	cout<<"Your Balance is "<<CurrentClient.AccountBalance << endl;
}

bool CheckTheBalance(int Amount)
{
	return CurrentClient.AccountBalance >= Amount;
}

void ShowWithdrawScreenWithNumber(int Number, sClient &CurrentClient)
{
	vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
	char Answer = 'n';
	if (CheckTheBalance(Number))
	{
		cout << "\nAre you sure you want to perform this transaction? y/n";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y')
		{
			CurrentClient.AccountBalance -= Number;
			for (sClient C : vClients)
			{
				if (C.AccountNumber == CurrentClient.AccountNumber)
				{
					C.AccountBalance = CurrentClient.AccountBalance;
					break;
				}
			}
			SaveCleintsDataToFile(ClientsFileName, vClients);
			cout << "\n\nDone Succesfully .New Balance is : " << (CurrentClient.AccountBalance) << endl;
		}
	}
	else
	{
		cout << "Amount Exceeds your balance , make another choice " << endl;
	}

}
void SwitchCasesWithDraw(ENQuickWithDrawOptions WitrhDrawOptions)
{
	switch (WitrhDrawOptions)
	{
	case ENQuickWithDrawOptions::Twenty:
		ShowWithdrawScreenWithNumber(20,CurrentClient);
		ReturnToMenuScreen();
		break;
	case ENQuickWithDrawOptions::Fifty:
		system("cls");
		ShowWithdrawScreenWithNumber(50, CurrentClient);
		ReturnToMenuScreen();
		break;
	case ENQuickWithDrawOptions::OneHunderd:
		system("cls");
		ShowWithdrawScreenWithNumber(100, CurrentClient);
		ReturnToMenuScreen();
		break;
	case ENQuickWithDrawOptions::TwoHunderd:
		system("cls");
		ShowWithdrawScreenWithNumber(200, CurrentClient);
		ReturnToMenuScreen();
		break;
	case ENQuickWithDrawOptions::FourHunderd:
		system("cls");
		ShowWithdrawScreenWithNumber(400, CurrentClient);
		ReturnToMenuScreen();
		break;
	case ENQuickWithDrawOptions::SixHunderd:
		system("cls");
		ShowWithdrawScreenWithNumber(600, CurrentClient);
		ReturnToMenuScreen();
		break;
	case ENQuickWithDrawOptions::EightHunderd:
		system("cls");
		ShowWithdrawScreenWithNumber(800, CurrentClient);
		ReturnToMenuScreen();
		break;
	case ENQuickWithDrawOptions::OneThousand:
		system("cls");
		ShowWithdrawScreenWithNumber(1000, CurrentClient);
		ReturnToMenuScreen();
		break;
	case ENQuickWithDrawOptions::Exit:
		system("cls");
		ReturnToMenuScreen();
		break;
	}
}

void ShowQickWithdrawScreen()
{
	cout << "===================================================\n";
	cout << "\t\tQuick WithDraw screen : \n";
	cout << "===================================================\n";
	cout << "\t[1] 20  \t[2] 50\n";
	cout << "\t[3] 100 \t[4] 200\n";
	cout << "\t[5] 400 \t[6] 600\n";
	cout << "\t[7] 800 \t[8] 1000\n";
	cout << "\t[9] Exit \n";
	cout << "===================================================\n";
	cout << "Your Balance is " << CurrentClient.AccountBalance << endl;
	SwitchCasesWithDraw((ENQuickWithDrawOptions)AskForTheChoiceQuickWithdraw());
}

void ShowNormalWithdrawScreen()
{
	cout << "===================================================\n";
	cout << "\t\tNormal WithDraw screen : \n";
	cout << "===================================================\n";
	
	vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
	AddWithDrawByAccontNumber(vClients);
}

void SwitchCases(ENMainMenuOptions ChoicesList)
{
	switch (ChoicesList)
	{
	case ENMainMenuOptions::eQuickWithdraw:
		system("cls");
		ShowQickWithdrawScreen();
		ReturnToMenuScreen();
		break;
	case ENMainMenuOptions::eNormalWithdraw:
		system("cls");
		ShowNormalWithdrawScreen();
		ReturnToMenuScreen();
		break;
	case ENMainMenuOptions::eDeposit:
		system("cls");
		ShowDepositScreen();
		ReturnToMenuScreen();
		break;
	case ENMainMenuOptions::eCheckBalance:
		system("cls");
		ShowCheckBalanceScreen();
		ReturnToMenuScreen();
		break;
	case ENMainMenuOptions::eLogout:
		system("cls");
		ShowLogoutScreen();
		break;
	}
}

void PrintTheMeunScreen()
{
	system("cls");
	cout << "========================================================\n";
	cout << "\t\tATM Main Menu Screen\n";
	cout << "========================================================\n";
	cout << "\t[1] Quick Withdraw \n";
	cout << "\t[2] Normal Withdraw \n";
	cout << "\t[3] Deposit \n";
	cout << "\t[4] Check Balance \n";
	cout << "\t[5] Logout \n";
	cout << "========================================================\n";
	SwitchCases((ENMainMenuOptions)AskForTheChoice());
}

bool LoadUserInfo(string AccountNumber, string PINCode)
{
	if (FindClientByAccountNumberAndPINCode(AccountNumber, PINCode,CurrentClient))
		return true;
	else
		return false;
}

void Login()
{
	bool LoginFaild = false;
	string AccountNumber, PINCode;
	do
	{
		system("cls");
		cout << "\n---------------------------------\n";
		cout << "\tLogin Screen";
		cout << "\n---------------------------------\n";
		if (LoginFaild)
		{
			cout << "Invlaid Account Number/PIN Code!\n";
		}
		cout << "Enter Account Number? ";
		cin >> AccountNumber;
		cout << "Enter PIN code? ";
		cin >> PINCode;
		LoginFaild = !LoadUserInfo(AccountNumber, PINCode);
	} while (LoginFaild);
	PrintTheMeunScreen();
}

int main()
{
	Login();
}

