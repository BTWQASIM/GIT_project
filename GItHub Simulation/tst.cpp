#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unistd.h>

using namespace std;

struct List
{
    string data ;
    List *next;
    List(){
        next = nullptr;
    }
};

struct RepositoryNode {
    string repositoryName;
    List *commits;
    List *files;
    RepositoryNode* left;
    RepositoryNode* right;

    RepositoryNode(){
        left = nullptr;
        right = nullptr;
        commits = nullptr;
        files = nullptr;
    }
};


struct User {
    string username;
    string password;
    RepositoryNode* repositories;
    User* followers[50];
};

struct Node {
    User user;
    Node* next;
};

class HashTable {
private:
    static const int TABLE_SIZE = 100;
    Node* table[TABLE_SIZE];
public:
    HashTable() {
        for (int i = 0; i < TABLE_SIZE; ++i) {
            table[i] = nullptr;
        }

        loadDataFromFile("loginCredential.csv");
    }
int hash(const string& key) {
        int hashValue = 0;
        int multiplier = 1;
        for (char c : key) {
            hashValue += c * multiplier;
            multiplier++;
        }
        return hashValue % TABLE_SIZE;
    }

    
    bool addUser(const string& username) {
        if (findUser(username)) {
            cout << "User already exists." << endl;
            return false;
        }

        int index = hash(username);

        Node* newNode = new Node();
        newNode->user.username = username;
        newNode->next = nullptr;

        if (table[index] == nullptr) {
            table[index] = newNode;
        } else {
            Node* temp = table[index];
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
        return true;
    }

    bool findUser(const string& username) {
        int index = hash(username);

        Node* temp = table[index];
        while (temp != nullptr) {
            if (temp->user.username == username) {
                return true;
            }
            temp = temp->next;
        }
        return false;
    }

    void storeDataToFile(const string& filename) {
        ofstream file(filename);
        if (!file) {
            cout << "Error: Unable to open file." << endl;
            return;
        }

        for (int i = 0; i < TABLE_SIZE; ++i) {
            Node* temp = table[i];
            while (temp != nullptr) {
                file << temp->user.username << endl;
                temp = temp->next;
            }
        }

        file.close();
        cout << "Data stored to file successfully." << endl;
    }

    void loadDataFromFile(const string& filename) {
        ifstream file(filename);
        if (!file) {
            cout << "Error: Unable to open file." << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string username;
            getline(ss, username, ',');

            addUser(username);
        }

        file.close();
        cout << "Data loaded from file successfully." << endl;
    }
    // access user account by username
    User* accessUser(const string& username) {
        int index = hash(username);

        Node* temp = table[index];
        while (temp != nullptr) {
            if (temp->user.username == username) {
                return &temp->user;
            }
            temp = temp->next;
        }
        return nullptr;
    }
};
// make a class connection which uses graphs to store followers and following of a user


/*class Connection : public HashTable{
    public:
    Connection(){
        HashTable();
    }
    // use graph to store followers and following of a user
    
    void follow(const string& follower, const string& followee) {
        Node *user1 = new Node;
        user1->user = *accessUser(followee);
        Node *user2 = new Node;
        user2->user = *accessUser(follower);
        user1->next = user2;
        user2->next = nullptr;
        int index = hash(follower);
        if(table[index] == nullptr){
            table[index] = user1;
        }else{
            Node* temp = table[index];
            while(temp->next != nullptr){
                temp = temp->next;
            }
            temp->next = user1;
        }

    }


};*/


class RepositoryTree : public HashTable{
private:
    RepositoryNode* root;

public:
    RepositoryTree() :root(nullptr) {}

    
    void createRepository(const string& repositoryName , const string& username) {
        root = insert(root, repositoryName, username);
    }

   
    void deleteRepository(const string& repositoryName) {
        root = remove(root, repositoryName);
    }

 
    bool searchRepository(const string& repositoryName) {
        return search(root, repositoryName);
    }
    
    void loadDataRepo(const string& username){
        string filename = username + "_repositories.csv";
        ifstream file(filename);
        if (!file) {
            cout << "No repositories found for user " << username << endl;
            return;
        }

        string repository;
        while (getline(file, repository)) {
            createRepository(repository, username);
        }
        file.close();
    }
    // a csv file to store names of repositories
    void storeDataToFile(const string &filename) {
        ofstream file("repositories.csv");
        if (!file) {
            cout << "Error: Unable to open file." << endl;
            return;
        }
        file << filename << endl;
        file.close();
        cout << "Data stored to file successfully." << endl;
    }
private:
    
    void storefileData(const string& filename, RepositoryNode* node, const string& username) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening file." << endl;
        sleep(2);
        return;
    }

    if (node == nullptr) {
        return;
    }
    file << username << ","<< node->repositoryName << "," << node->commits->data << "," << node->files->data << endl;
    file.close();
}

    RepositoryNode* insert(RepositoryNode* node, const string& repositoryName, const string& username) {
        if (node == nullptr) {
            RepositoryNode* newNode = new RepositoryNode();
            newNode->repositoryName = repositoryName;
            string data;
            cout<<"Enter the data for the commit: ";
            cin>>data;
            newNode->commits = new List();
            newNode->commits->data = data;
            cout<<"Enter the data for the files: ";
            cin>>data;
            newNode->files = new List();
            newNode->files->data = data;
            
            storefileData(repositoryName + "_repositories.csv", newNode, username);
            storeDataToFile(repositoryName + "_repositories.csv");

            return newNode;
        }

        if (repositoryName < node->repositoryName) {
            node->left = insert(node->left, repositoryName, username);
        } else if (repositoryName > node->repositoryName) {
            node->right = insert(node->right, repositoryName, username);
        }

        return node;
    }

   
    RepositoryNode* remove(RepositoryNode* node, const string& repositoryName) {
        if (node == nullptr) {
            return nullptr;
        }

        if (repositoryName < node->repositoryName) {
            node->left = remove(node->left, repositoryName);
        } else if (repositoryName > node->repositoryName) {
            node->right = remove(node->right, repositoryName);
        } else {
            if (node->left == nullptr && node->right == nullptr) {
                delete node;
                return nullptr;
            } else if (node->left == nullptr) {
                RepositoryNode* temp = node->right;
                delete node;
                return temp;
            } else if (node->right == nullptr) {
                RepositoryNode* temp = node->left;
                delete node;
                return temp;
            } else {
                RepositoryNode* minNode = findMin(node->right);
                node->repositoryName = minNode->repositoryName;
                node->right = remove(node->right, minNode->repositoryName);
            }
        }

        return node;
    }
    RepositoryNode* findMin(RepositoryNode* node) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    bool search(RepositoryNode* node, const string& repositoryName) {

        if (node == nullptr) {
            return false;
        }

        if (repositoryName == node->repositoryName) {
            return true;
        } else if (repositoryName < node->repositoryName) {
            return search(node->left, repositoryName);
        } else {
            return search(node->right, repositoryName);
        }
    }
};
class UserAccount : public RepositoryTree{
    public:
    UserAccount(){
        HashTable();
    }

    bool registerUser(string username, string password){
        if(addUser(username)){
            ofstream file("loginCredential.csv", ios::app);
            file << username <<","<< password << endl;
            file.close();
            cout << "User registered successfully." << endl;
            return true;
        }
        else{
            return false;
    }
    }

    bool loginUser(string username, string password){
        ifstream file("loginCredential.csv");
        if(!file){
            cout << "Error: Unable to open file." << endl;
            return false;
        }
        string line;
        while(getline(file, line)){
            stringstream ss(line);
            string user, pass;
            getline(ss, user, ',');
            getline(ss, pass, ',');
            if(user == username && pass == password){
                cout << "Login successful." << endl;
                User* user = accessUser(username);
                UserMenu(user);
                return true;
                
            }
        }
        cout << "Login failed." << endl;
        return false;
    }
    void logoutUser(User* user){
        delete user;
    }
    bool fileExists(const std::string& filename) {
    ifstream file(filename);
    return file.good(); 
}
    void UserMenu(User* user)
{ 
    system("clear");
    RepositoryTree repo;

    while(1){
    cout << "Welcome " << user->username << endl;
    cout << "1. View Profile" << endl;
    cout << "2. Edit Profile" << endl;
    cout << "3. View Followers" << endl;
    cout << "4. View Following" << endl;
    cout<< "5. create repository"<<endl;
    cout<< "6. delete repository"<<endl;
    cout<< "7. search repository"<<endl;
    cout << "8. Logout" << endl;
    cout << "Enter your choice: ";
    int choice;
    cin >> choice;
    string username;
    switch (choice) {
        case 1:
            cout << "Username: " << user->username << endl;
            cout << "Followers: " << user->followers << endl;
            break;
        case 2:
            cout << "Enter new username: ";
            cin >> username;
            user->username = username;
            break;
        case 3:
            cout << "Followers: " << user->followers << endl;
            break;
        case 4:
            cout << "Following: " << user->followers << endl;
            break;
       

        case 5:
            cout << "Enter the repository name: ";
            cin >> username;

            if(fileExists(username + "_repositories.csv")){
                cout << "Repository already exists." << endl;
                sleep(5);
            }
            else{
               repo.createRepository(username, user->username);
            }
            
            
            break;
        case 6:
            cout << "Enter the repository name: ";
            cin >> username;
            repo.deleteRepository(username);
            break;
        case 7:
            cout << "Enter the repository name: ";
            cin >> username;
            
            if(repo.searchRepository(username)){
                cout << "Repository found." << endl;

            }
            else{
                cout << "Repository not found." << endl;
            }
            sleep(5);
            break;
        case 8:
            
            logoutUser(user);
            cout << "Logging out..." << endl;
            break;
        default:
            cout << "Invalid choice." << endl;
    }
    system("clear");
    }

}


};




int main() {
    UserAccount userAccount;
    string username, password;
    system("clear");
    int choice;
    while (1)
    {
        cout << "1. Register" << endl;
        cout << "2. Login" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        switch(choice){
            case 1:
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;
                if(userAccount.registerUser(username, password)){
                    cout << "Enter username: ";
                    cin >> username;
                    cout << "Enter password: ";
                    cin >> password;
                   if(userAccount.loginUser(username, password)){
                    User* user = userAccount.accessUser(username);
                    userAccount.loadDataRepo(username);
                    userAccount.UserMenu(user);
                }
                }
                else{
                    
                }
                

                break;
            case 2:
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;
                if(userAccount.loginUser(username, password)){
                    User* user = userAccount.accessUser(username);
                    userAccount.UserMenu(user);
                }
                break;
            default:
                cout << "Invalid choice." << endl;
        }
    }
    return 0;
}