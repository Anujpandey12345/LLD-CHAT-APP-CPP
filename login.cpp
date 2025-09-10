#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;

class temp {
    string UserName, Email, Password;
    string searchName, searchEmail, searchPass;
    fstream file;
    public:
    void Login();
    void SignUp();
    void Froget();
}obj;



int main() {
    char choice;
    cout <<"\n 1 - Login : ";
    cout <<"\n 2 - Sign up : ";
    cout <<"\n 3 - Forgate Password : ";
    cout <<"\n 4 - Exit : ";
    cout <<"\n Enter Your Choice :: ";
    cin >> choice;

    switch(choice) {
        case '1':
            cin.ignore();
            obj.Login();
        break;
        case '2':
            cin.ignore();
            obj.SignUp();
        break;
        case '3':
            cin.ignore();
            obj.Froget();
        break;
        case '4':
            return 0;
        break;
        default:
            cout << "Invalid Selection.....";
    }
}

void temp :: SignUp() {
    cout << "\n Enter Your UserName :: ";
    getline(cin, UserName);
    cout << "Enter Your Email :: ";
    getline(cin, Email);
    cout << "Enter Your Password :: ";
    getline(cin, Password);

    file.open("loginData.txt", ios :: out | ios :: app);
    file<<UserName<<"*"<<Email<<"*"<<Password<<endl;
    file.close();
}

void temp :: Login() {
    string searchName, searchPass;
    cout << "----------LOGIN------------"<<endl;
    cout<<"Enter your User Name :: " << endl;
    getline(cin, searchName);
    cout << "Enter Your Password :: " << endl;
    getline(cin, searchPass);


    file.open("loginData.txt", ios :: in);
    getline(file, UserName, '*');
    getline(file, Email, '*');
    getline(file, Password, '\n');
    while(!file.eof()) {
        if(UserName == searchName) {
            if(Password == searchPass) {
                cout << "\n Account Login Successfully !";
                cout << "\n User Name is :: " << UserName << endl;
                cout << "\n User Email is :: " << Email << endl;
            } else {
                cout << "Password is Incorrect !!" << endl;
            }
        }
    getline(file, UserName, '*');
    getline(file, Email, '*');
    getline(file, Password, '\n');
    }
    file.close();
}


void temp :: Froget() {
    cout << "\n Enter Your User Name :: ";
    getline(cin, searchName);
    cout << "\n Enter your Email Address :: ";
    getline(cin, searchEmail);

    file.open("loginData.txt", ios :: in);
    getline(file, UserName, '*');
    getline(file, Email, '*');
    getline(file, Password, '\n');

    while(!file.eof()) {
        if(UserName == searchName){
            if(Email == searchEmail) {
                cout << "\n Account Found .. " << endl;
                cout << "Your Password is " << Password << endl;

            } else {
                cout << "Record Not Found !! \n";
            }
        } else {
            cout << "\n Record Not Found !! \n";
        }
    getline(file, UserName, '*');
    getline(file, Email, '*');
    getline(file, Password, '\n');
    }
    file.close();
}