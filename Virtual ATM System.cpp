#include<iostream>
#include<fstream>
using namespace std;
struct createaccount{
    string name;
    int account_number;
    int pin;
    int balance;
};

struct transaction{
    string type[10];        
    int amount[10];          
    string transfer_name[10];
    int balance[10];         
};


void saveUsersToFile(createaccount user[],int size){
    ofstream file("users.txt");
    for(int i =0;i< size;i++){
        file<<user[i].name<<" "<<user[i].account_number<<" "<< user[i].pin<<" "<<user[i].balance<<endl;
  }
    file.close();
}

void loadUsersFromFile(createaccount user[],int &size){
    ifstream file("users.txt");
    if(!file){ 
        cout<<"No user data found. Please create users in users.txt."<<endl;
        return;
    }
    size=0;
    while(file>>user[size].name>>user[size].account_number>>user[size].pin>>user[size].balance){
    size++;
    }
    file.close();
}

void withdrawAmount(createaccount user[],int index,int user_count,int &a,transaction &receipt) {
    double withdraw;
    cout << "\n\tAmount You Want to Withdraw: ";
    cin >> withdraw;
    if(withdraw> 0 && user[index].balance>= withdraw) {
        user[index].balance-= withdraw;
        saveUsersToFile(user,user_count);
        receipt.type[a] = "Withdraw";
        receipt.amount[a] = withdraw;
        receipt.transfer_name[a] = user[index].name;
        receipt.balance[a] = user[index].balance;
        a++;
        cout<<"\n\t Your Current Balance is: "<<user[index].balance<<endl;
    } else{
        cout<<"You Don't Have Enough Money to Withdraw or Invalid Amount."<<endl;
    }
}

void transferAmount(createaccount user[], int sender_index,int user_count,int &a,transaction &receipt){
    int receiver_account, receiver_index= -1;
    cout<<"\n\tEnter the Account Number to Transfer To: ";
    cin>>receiver_account;
    for(int i= 0; i< user_count;i++) {
        if(user[i].account_number == receiver_account){
            receiver_index = i;
            break;
        }
    }
    if(receiver_index == -1) {
        cout << "\n\tInvalid Account Number. Transfer Failed." << endl;
    } else if(receiver_index == sender_index){
        cout << "You cannot transfer amount to yourself." << endl;
    } else{
        double transfer_amount;
        cout << "\n\tEnter the Amount to Transfer: ";
        cin >> transfer_amount;
        if (user[sender_index].balance >= transfer_amount && transfer_amount > 0) {
            user[sender_index].balance -= transfer_amount;
            user[receiver_index].balance += transfer_amount;
            saveUsersToFile(user, user_count);
            
            receipt.type[a] = "Transfer";
            receipt.amount[a] = transfer_amount;
            receipt.transfer_name[a] = user[receiver_index].name;
            receipt.balance[a] = user[sender_index].balance;
            a++;
            cout << "\n\tTransfer Successful! Your Current Balance is: " << user[sender_index].balance << endl;
        } else {
            cout << "\n\tYou Don't Have Enough Balance to Transfer or Invalid Amount." << endl;
        }
    }
}

void displayMiniStatement(transaction receipt, int a){
    if(a ==0){
        cout<< "\n\tNo Transaction Display\n";
    }else{
        cout<< "\n\tMini Statement";
        cout<< "\n\t-------------------";
        for (int i = 0; i < a; i++) {
            cout << "\n\tType : " << receipt.type[i];
            cout << "\n\tAmount : " << receipt.amount[i];
            if (receipt.type[i] == "Withdraw") {
                cout << "\n\tAccount Holder Name : " << receipt.transfer_name[i];
            } else {
                cout << "\n\tTransferred to : " << receipt.transfer_name[i];
            }
            cout<< "\n\tCurrent Balance: "<< receipt.balance[i]<<"\n\t-----------------";
        }
    }
}

int displayMenu(){
    int option;
    cout<<"\n\t*************Menu*************";
    cout<<"\n\t*                            *";
    cout<<"\n\t*       1. Check Balance     *";
    cout<<"\n\t*       2. Withdraw          *";
    cout<<"\n\t*       3. Transfer Amount   *";
    cout<<"\n\t*       4. Mini Statement    *";
    cout<<"\n\t*       5. Exit              *";
    cout<<"\n\t*                            *";
    cout<<"\n\t******************************";
    cout<<"\n\t Please Enter an Option: ";
    cin>>option;
    return option;
}

int main() {
    createaccount user[10];
    int user_count=0;
    loadUsersFromFile(user, user_count);
    transaction receipt;
    int a= 0;
    
    int option,pin,count=0,account,error=1,b=0;
    cout<<"Welcome to Virtual ATM system."<<endl;
    do{  
        cout<<"Enter Your Account Number: ";
        cin>>account;
        bool account_found=false;
        for(int i = 0;i <user_count;i++){
            if(account== user[i].account_number){
            b=i;
             account_found = true;
            cout<<"Enter your PIN: ";
              cin>>pin;
                
                if(pin!=user[i].pin){
                    cout<<"Wrong PIN. Please try again.\n";
                    count++;
                    if(count ==2){
                        cout<< "         ***Alert***         \n Last Try. One More Wrong PIN and your ATM Card Will Be Blocked \n ";
                    }
                    if(count > 2){
                        cout<<"\nYou Have Exceeded the limit of wrong PINs \n Now your ATM Card is Blocked \n";
                        return 1;
                    }
                } else{
                    cout<<"Welcome "<<user[i].name<<endl;
                    error= 0; 
                }
            }
        }
        if (!account_found){
            cout<<"Invalid account. Please try again."<<endl;
        }
    } while (error == 1);
    
    cout<<"Your Account Login Successfully!"<<endl;
    
    do {
        option =displayMenu();
        switch (option){
            case 1:
                cout<<"\n\t Your Balance is: "<<user[b].balance<<endl;
                break;
            case 2:
                withdrawAmount(user, b,user_count,a,receipt);
                break;
            case 3: 
               transferAmount(user,b, user_count,a,receipt);
                break;
            case 4:
                displayMiniStatement(receipt, a);
                break;
            case 5:
            	cout<<"Thanks for using our services."<<endl;
            	break;
            default:
                cout<<"\n\tInvalid Option. Please Try Again."<<endl;
        }
    } while(option!= 5);  
    return 0;
}
