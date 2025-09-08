#include<iostream>
#include<fstream>
#include<ctime>
#include<sstream>
#include<string>
#include<cstdlib>
using namespace std;

// Function to get current Date & Time
string getDateTime()
{
  time_t now = time(0);
  tm *lclTime = localtime(&now);
  stringstream ss;
  ss<<lclTime->tm_mday<<'-'<<1+lclTime->tm_mon<<'-'<<1900+lclTime->tm_year<<" "
  <<lclTime->tm_hour<<':'<<1+lclTime->tm_min<<':'<<1+lclTime->tm_sec;
  return ss.str();
}

// Password Encryption Function
string encryptPass(string password){
    for(char &ch : password){
        ch += 3;   // shifts each character value (ASCII) by 3
    }
    return password;
}

// Password Decryption Function
string decryptPass(string password){
    for(char &ch : password){
        ch -= 3; // unshifts each character value (ASCII) by 3
    }
    return password;
}

//  Base Class for User
class User
{
    protected: string username;
               string pass;
    public: 
    User(string uname, string pwd){
        username = uname;
        pass = encryptPass(pwd);
    }
    
    bool checkPass(string paswd){
        return pass == encryptPass(paswd);
    }
};

// Derived class for Diary with user accounts
class Diary : public User
{
  public:
  Diary (string uname, string pwd) : User (uname, pwd){} //initialises the User constructor
  
  // Adding diary entry
  void addEntry(){
    string entry;
    cout<<"Enter your Diary Entry: ";
    cin.ignore();
    getline(cin, entry);
    string timestamp = getDateTime();
    ofstream userFile(username+".txt", ios::app);
    userFile<<"Date and Time : "<<timestamp<<endl;
    userFile<<"Entry : "<<entry<<endl;
    userFile<<"----------------------------"<<endl;
    userFile.close();
    cout<<"Entry added successfully :)"<<endl;
  }

  // Viewing all Diary entries
  void viewAllEntry(){
    string line;
    ifstream userFile(username+".txt");
    if(userFile.is_open()){
        while(getline(userFile, line)){
            cout<<line<<endl;
        }
    }
    else{
        cout<<"No entries found for user "<<username<<endl;
    }
    userFile.close();
  }
 

// Deleting entry by date and time
   void deleteEntryByDateTime()
{
    string datetime;
    cout<<"Enter the exact date(DD-MM-YYYY) and time(HH:MM:SS): ";
    cin.ignore();
    getline(cin, datetime);
    ifstream userFile(username+".txt");
    if(!userFile){
        cout<<"No entries found for user "<<username<<endl;
    }

    bool entryFound=false;
    string line;
      ofstream tempFile("temp.txt");
      while(getline(userFile, line))
      {
       if(line.find(datetime)!=string::npos)
       {
        entryFound = true;
        while(getline(userFile, line) && line!="----------------------------"){}
       }
       else{
        tempFile<<line<<endl;
       }
      }
       userFile.close();
       tempFile.close();
       
    remove((username+".txt").c_str());
    rename("temp.txt", (username+".txt").c_str());
    if(entryFound)
    {
        cout<<"Entry deleted successfully."<<endl;
    }
    else{
           cout<<"No entry found for given date and time"<<endl;
    }
}


//Edit entry by date and time
void editEntrybyDateTime()
{
    string datetime, newEntry;
    cout<<"Enter the exact Date(DD-MM-YYYY) and Time(HH:MM:SS): ";
    cin.ignore();
    getline(cin, datetime);
    ifstream userFile(username+".txt");
    if(!userFile){
        cout<<"No entries found for user "<<username<<endl;
    }
    bool entryFound=false;
    string line;
    ofstream tempFile("temp.txt");
    while(getline(userFile, line))
    {
    if(line.find(datetime) != string::npos)
    {
        entryFound = true;
        cout<<"Enter the new Diary Entry: ";
        cin.ignore();
        getline(cin, newEntry);
        tempFile<<"Date and Time : "<<datetime<<endl;
        tempFile<<"Entry : "<<newEntry<<endl;
        tempFile<<"----------------------------"<<endl;
        while(getline(userFile, line) && line!="----------------------------"){}
    }
    else{
            tempFile<<line<<endl;
        }
    }
       userFile.close();
       tempFile.close();

       remove((username+".txt").c_str());
       rename("temp.txt", (username+".txt").c_str());

       if(!entryFound){
        cout<<"No entry found for such date and time"<<endl;
       }
       else{
        cout<<"Entry edited successfully!"<<endl;
       }
}
};

// Creating new User
Diary *createAccount(){
    string uname, pwd;
    cout<<"Enter username: ";
    cin>>uname;
    cout<<"Enter password: ";
    cin>>pwd;
    ofstream userFile(uname+"_auth.txt");
    userFile<<encryptPass(pwd)<<endl;
    userFile.close();
    cout<<"Account Created Successfully :)";
    return new Diary(uname, pwd);
}


// Login function
Diary *Login(){
    string uname, paswd;
    cout<<"Enter username: ";
    cin>>uname;
    cout<<"Enter password: ";
    cin>>paswd;
    ifstream userFile(uname+"_auth.txt");
    if(!userFile){
        cout<<"User not Found\n";
        return nullptr;
    }
    string storedpass;
    getline(userFile, storedpass);
    userFile.close();
    
    if(encryptPass(paswd)==storedpass){
        Diary *diary = new Diary(uname, paswd);
        cout<<"Logged in Successfully\n";
        return diary;
    }
    else{
        cout<<"Incorrect Password \n";
        return nullptr;
    }
}

int main()
{
   Diary *currentDiary = nullptr;
   int choice;
   cout<<"\n----------------Personal Diary Application----------------\n";
   while(true){
   cout<<"\n--------------------Welcome to the Menu--------------------\n";
   cout<<"1. Create Account\n"
       <<"2. Login\n"
       <<"3. Add New Entry\n"
       <<"4. View All Entries\n"
       <<"5. Delete Entry By Date & Time\n"
       <<"6. Edit Entry By Date & Time\n"
       <<"7. Exit\n";
    cout<<"Enter your choice: ";
    cin>>choice;
    switch (choice)
    {
        case 1: currentDiary = createAccount();
        break;
        case 2: currentDiary = Login();
        break;
        case 3: if(currentDiary) currentDiary->addEntry();
        else cout<<"You must login first!!\n";
        break;
        case 4: if(currentDiary) currentDiary->viewAllEntry();
        else cout<<"You must login first!!\n";
        break;
        case 5: if(currentDiary) currentDiary->deleteEntryByDateTime();
        else cout<<"You must login first!!\n";
        break;
        case 6: if(currentDiary) currentDiary->editEntrybyDateTime();
        else cout<<"You must login first!!\n";
        break;
        case 7: cout<<"Exiting Application...\n";
        exit(0);
        break;
        default:
        cout<<"Invalid Case!! Please try again.\n";
    }
   }
   return 0;
}