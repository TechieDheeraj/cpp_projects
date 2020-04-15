#include<iostream>
#include<fstream>
#include<map>

using namespace std;

#define MIN_BALANCE 1000

class InsufficientFunds : public exception {};

class Account {

   private:
      long accountNo;
      string firstName;
      string lastName;
      float balance;
      static long nextAccountNo;

   public:
      Account() {}
      Account(string fname, string lname, float bal);
      long getAccountNo() { return accountNo; }
      string getFirstName() { return firstName; }
      string getLastName() { return lastName; }
      float getBalance() { return balance; }

      void deposit(float amount);
      void withdraw(float amount);
      static void setLastAccountNo(long lastActNo); // When Application restarts It should know last account number
      static long getLastAccountNo();

      friend ifstream &operator>>(ifstream &ifstrm, Account &act);
      friend ofstream &operator<<(ofstream &ofstrm, Account &act);
      friend ostream &operator<<(ostream &ostrm, Account &act);
};

long Account::nextAccountNo = 0;

class Bank {
   
   private:
      map<long, Account> accounts;

   public:

      Bank();
      Account openAccount(string fname, string lname, float bal);
      Account balanceEnquiry(long accountNo);
      Account deposit(long accountNo, float amount);
      Account withdraw(long accounttNo, float amount);
      void closeAccount(long accountNo);    
      void showAllAccounts();
      ~Bank();
};

int main() {

   Bank b;
   Account acc;

   int choice;
   string fname, lname;
   long accountNumber;
   float balance;
   float amount;

   cout << "*************** Banking System ***************" << endl;

   do {

      cout << "\n\tSelect one option below" << endl;
      cout << "\n\t1 Open an Account" << endl;
      cout << "\n\t2 Balance enquiry" << endl;
      cout << "\n\t3 Deposit" << endl;
      cout << "\n\t4 Withdrawl" << endl;
      cout << "\n\t5 Close an Account" << endl;
      cout << "\n\t6 Show all Accountst" << endl;
      cout << "\n\t7 Quit" << endl;

      cout << "Enter your choice: " ;
      cin >> choice;
      
      switch(choice) {
         case 1 : 
            cout << "\n\t1 Enter First Name : ";
            cin >> fname;
            cout << "\n\t1 Enter Last Name : ";
            cin >> lname;
            cout << "\n\t1 Enter Initial balance : ";
            cin >> balance;

            acc = b.openAccount(fname, lname, balance);
            cout << "\n Account Created Successfully" << endl;
            cout << acc;
               
            break;

         case 2:
            cout << "Enter Account Number: "; 
            cin >> accountNumber;

            acc = b.balanceEnquiry(accountNumber);
            cout << "Your Account Details: " << endl;
            cout << acc;

            break;

         case 3:
            cout << "Enter Account Number: ";
            cin >> accountNumber;

            cout << "Enter deposit amount: ";
            cin >> amount;

            acc = b.deposit(accountNumber, amount);
            cout << acc;

            break;

         case 4:
            cout << "Enter Account Number: ";
            cin >> accountNumber;

            cout << "Enter amount to Withdraw: ";
            cin >> amount;

            acc = b.withdraw(accountNumber, amount);
            cout << acc;

            break;

         case 5:
            cout << "Enter Account Number: ";
            cin >> accountNumber;

            b.closeAccount(accountNumber);
            cout << "Account " << accountNumber << "is closed" << endl;

            break;

         case 6:
            b.showAllAccounts();
            break;

         case 7: break;

         default:
            cout << "Enter valid choice" << endl;
            break;
      }

   }
   while(choice != 7);

   return 0;
}

Account::Account(string fname, string lname, float bal) {
   nextAccountNo++;
   accountNo = nextAccountNo;
   firstName = fname;
   lastName = lname;
   balance = bal;
}

void Account::deposit(float amount) {
   balance+=amount;
}

void Account::withdraw(float amount) {
   if (balance-amount < MIN_BALANCE)
      throw InsufficientFunds();

   balance-=amount;
}

void Account::setLastAccountNo(long lastActNo) {
   nextAccountNo = lastActNo;
}

long Account::getLastAccountNo() {
   return nextAccountNo;
}

ifstream &operator>>(ifstream &ifstrm, Account &act) {
   ifstrm>>act.accountNo;
   ifstrm>>act.firstName;
   ifstrm>>act.lastName;
   ifstrm>>act.balance;

   return ifstrm;
}

ofstream &operator<<(ofstream &ofstrm, Account &act) {
   ofstrm << act.accountNo << endl;
   ofstrm << act.firstName << endl;
   ofstrm << act.lastName << endl;
   ofstrm << act.balance << endl;

   return ofstrm;
}

ostream &operator<<(ostream &ostrm, Account &act) {
   ostrm << "First Name :" << act.firstName << endl;
   ostrm << "Last Name :" << act.lastName << endl;
   ostrm << "Account Number :" << act.accountNo << endl;
   ostrm << "Balance :" << act.balance << endl;

   return ostrm;
}

Account Bank::openAccount(string fname, string lname, float bal) {

   ofstream ofstrm;

   Account account(fname, lname, bal);
   accounts.insert(pair<long, Account>(account.getAccountNo(), account));

   ofstrm.open("Bank.data", ios::trunc);

   map<long, Account>::iterator itr;

   for(itr = accounts.begin(); itr != accounts.end(); ++itr) {

      ofstrm << itr->second;
   }

   ofstrm.close();

   return account;
}

Account Bank::balanceEnquiry(long accountNo) {
   
   auto itr = accounts.find(accountNo);

   return itr->second;
}

Account Bank::deposit(long accountNo, float amount) {

   auto itr = accounts.find(accountNo);

   itr->second.deposit(amount);
   return itr->second;
}

Account Bank::withdraw(long accountNo, float amount) {

   auto itr = accounts.find(accountNo);

   try {
      itr->second.withdraw(amount);
   }
   catch(InsufficientFunds e) {
      cout << "Insufficient funds .." << endl;
   }

   return itr->second;
}

void Bank::closeAccount(long accountNo) {
   
   auto itr = accounts.find(accountNo);

   cout << "Account deleted " << itr->second << endl;

   accounts.erase(accountNo);
}   

void Bank::showAllAccounts() {
   
   for(auto itr = accounts.begin(); itr != accounts.end(); ++itr) {  
      cout << "Account : " << itr->first << endl << itr->second << endl;
   }
}

Bank::Bank() {
   
   ifstream infile;
   Account account;

   infile.open("Bank.data");
   if(!infile) {
      cout << "File not Opening" << endl;
      return;
   }

   while(!infile.eof()) {

      infile >> account;
      accounts.insert(pair<long, Account>(account.getAccountNo(), account));
   }

   Account::setLastAccountNo(account.getAccountNo());
   infile.close();
}

Bank::~Bank() {
   
   ofstream outfile;

   outfile.open("Bank.data", ios::trunc);

   for (auto itr = accounts.begin(); itr != accounts.end(); ++itr) {
      outfile << itr->second << endl;
   }

   outfile.close();
}
