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
    string Username;
    List *commit;
    List *file;
    int height;
    RepositoryNode* left;
    RepositoryNode* right;

    RepositoryNode(){
        left = nullptr;
        right = nullptr;
        commit = nullptr;
        file = nullptr;
    }
};


struct User {
    string username;
    string password;
    RepositoryNode* repositories;
    User* followers[50];
    User* following[50];
  
    
    
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



class SocialGraph {
private:
    static const int MAX_USERS = 100;
    bool adjMatrix[MAX_USERS][MAX_USERS]; 

    int userCount = 0; 


    int findUserIndex(const string& username) {
        for (int i = 0; i < userCount; ++i) {
            if (users[i].username == username) {
                return i;
            }
        }
        return -1; 
    }

public:
SocialGraph() {
        for (int i = 0; i < MAX_USERS; ++i) {
            for (int j = 0; j < MAX_USERS; ++j) {
                adjMatrix[i][j] = false;
            }
        }
    }
    User users[MAX_USERS]; 

   
    void addUser(const string& username) {
        if (userCount < MAX_USERS) {
          //  users[userCount++] = User(username);
        } else {
            cout << "Cannot add more users. Maximum limit reached." << endl;
        }
    }

    
    void followUser(const string& follower, const string& followee) {
        int followerIndex = findUserIndex(follower);
        int followeeIndex = findUserIndex(followee);

        if (followerIndex != -1 && followeeIndex != -1) {
            adjMatrix[followerIndex][followeeIndex] = true;
        } else {
            cout << "User does not exist." << endl;
        }
    }

   
    void printFollowers(const string& username) {
        int userIndex = findUserIndex(username);

        if (userIndex != -1) {
            cout << "Followers of " << username << ":" << endl;
            for (int i = 0; i < userCount; ++i) {
                if (adjMatrix[i][userIndex]) {
                    cout << users[i].username << endl;
                }
            }
        } else {
            cout << "User does not exist." << endl;
        }
    }
};


class RepositoryTree: public HashTable
{
private:
    RepositoryNode *root;

public:
    RepositoryTree() : root(nullptr)
    {
        loadDataFromFile("repositories.csv");
    }

    void printtree()
    {
        printTree(root);
    }
    void storefileData(const string& filename, RepositoryNode* node) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening file." << endl;
        sleep(2);
        return;
    }

    if (node == nullptr) {
        return;
    }
    file << node->Username << ","<< node->repositoryName << "," << node->commit->data << "," << node->file->data << endl;
    file.close();
}
   
    void storeDataToFile(const string &filename) {
        ofstream file("repositories.csv", ios::app);
        if (!file) {
            cout << "Error: Unable to open file." << endl;
            return;
        }
        file << filename << endl;
        file.close();
        cout << "Data stored to file successfully." << endl;
    }
    
    void createRepository(const string &repositoryName, const string &username, const string &commitData, const string &file)
    {
        root = insert(root, repositoryName, username, commitData, file);
        storeDataToFile(repositoryName);
    }

    void loadDataFromFile(const string& path) {
    ifstream file(path);

    if (!file.is_open()) {
        cerr << "Error: Unable to open main file " << path << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        ifstream data(line);
        if (!data.is_open()) {
            cerr << "Error: Unable to open file " << line << endl;
            continue;
        }

        string row;
        while (getline(data, row)) {
            stringstream rowStream(row);
            string username, repositoryname, commit, file;

            getline(rowStream, username, ',');
            getline(rowStream, repositoryname, ',');
            getline(rowStream, commit, ',');
            getline(rowStream, file, ',');
            root = insert(root, repositoryname+"_repositories.csv", username, commit, file);

           

        }

        data.close();
        cout << endl;
    }

    file.close();
}

    void deleterepository(const string &repositoryName, const string &username)
    {
        root = deleteRepository(root, repositoryName, username);
    }

    bool searchRepository(const string &repositoryName)
    {
        return printRepository(search(root, repositoryName + "_repositories.csv"));
    }

private:
    int getHeight(RepositoryNode *node)
    {
        if (node == nullptr)
        {
            return 0;
        }
        else
        {
            return node->height;
        }
    }

    int getBalance(RepositoryNode *node)
    {
        if (node == nullptr)
        {
            return 0;
        }
        else
        {
            return getHeight(node->left) - getHeight(node->right);
        }
    }

    RepositoryNode *rotateRight(RepositoryNode *y)
    {
        RepositoryNode *x = y->left;
        RepositoryNode *T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

        return x;
    }

    RepositoryNode *rotateLeft(RepositoryNode *x)
    {
        RepositoryNode *y = x->right;
        RepositoryNode *T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

        return y;
    }

    RepositoryNode *insert(RepositoryNode *node, const string &repositoryName, const string &username, const string &commitData, const string &fileName)
    {
        if (node == nullptr)
        {
            RepositoryNode *newNode = new RepositoryNode();
            newNode->repositoryName = repositoryName;
            newNode->Username = username;
            newNode->commit = new List();
            
            newNode->commit->data = commitData;
         
            newNode->commit->next = nullptr;
            newNode->file = new List();
            newNode->file->data = fileName;
            newNode->file->next = nullptr;
            storefileData(repositoryName, newNode);
            return newNode;
        }

        if (repositoryName < node->repositoryName)
            node->left = insert(node->left, repositoryName, username, commitData, fileName);
        else if (repositoryName > node->repositoryName)
            node->right = insert(node->right, repositoryName, username, commitData, fileName);
        else
            return node;

        node->height = 1 + max(getHeight(node->left), getHeight(node->right));

        int balance = getBalance(node);

        if (balance > 1 && repositoryName < node->left->repositoryName)
            return rotateRight(node);

        if (balance < -1 && repositoryName > node->right->repositoryName)
            return rotateLeft(node);

        if (balance > 1 && repositoryName > node->left->repositoryName)
        {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        if (balance < -1 && repositoryName < node->right->repositoryName)
        {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    RepositoryNode *findMin(RepositoryNode *node)
    {
        RepositoryNode *current = node;
        while (current->left != nullptr)
            current = current->left;
        return current;
    }

    RepositoryNode *deleteRepository(RepositoryNode *node, const string &repositoryName, const string &username)
    {
        if (node == nullptr)
            return node;

        if (repositoryName < node->repositoryName)
            node->left = deleteRepository(node->left, repositoryName, username);
        else if (repositoryName > node->repositoryName)
            node->right = deleteRepository(node->right, repositoryName, username);
        else
        {
            if (username != node->Username)
            {
                cout << "You are not authorized to delete this repository." << endl;
                return node;
            }

            // Delete the repository from the CSV file

            deleteStringInFile("repositories.csv", repositoryName);

            if (node->left == nullptr || node->right == nullptr)
            {
                RepositoryNode *temp;
                if (node->left == nullptr)
                {
                    temp = node->right;
                }
                else
                {
                    temp = node->left;
                }

                if (temp == nullptr)
                {
                    temp = node;
                    node = nullptr;
                }
                else
                    *node = *temp;

                delete temp;
            }
            else
            {
                RepositoryNode *temp = findMin(node->right);
                node->repositoryName = temp->repositoryName;
                node->right = deleteRepository(node->right, temp->repositoryName, username);
            }
        }

        if (node == nullptr)
            return node;

        node->height = 1 + max(getHeight(node->left), getHeight(node->right));

        int balance = getBalance(node);

        if (balance > 1 && getBalance(node->left) >= 0)
            return rotateRight(node);

        if (balance > 1 && getBalance(node->left) < 0)
        {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        if (balance < -1 && getBalance(node->right) <= 0)
            return rotateLeft(node);

        if (balance < -1 && getBalance(node->right) > 0)
        {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    RepositoryNode *search(RepositoryNode *node, const string &repositoryName)
    {
        if (node == nullptr)
            return nullptr;
        else if (repositoryName == node->repositoryName)
            return node;
        else if (repositoryName < node->repositoryName)
            return search(node->left, repositoryName);
        else
            return search(node->right, repositoryName);
    }

    bool printRepository(RepositoryNode *node)
    {
        if (node != nullptr)
        {
            cout << "Repository Name: " << node->repositoryName << endl;
            cout << "Commit: " << node->commit->data << endl;
            cout << "File: " << node->file->data << endl;
            return true;
        }
        return false;
    }
    void printList(List *head)
    {
        List *current = head;
        while (current != nullptr)
        {
            cout << current->data << " ";
            current = current->next;
        }
        cout << endl;
    }
    void printTree(RepositoryNode *node)
    {
        if (node != nullptr)
        {
            printTree(node->left);
            cout << "Repository Name: " << node->repositoryName << endl;
            cout << "Username: " << node->Username << endl;
            cout << "Commits: ";
            printList(node->commit);
            cout << "Files: ";
            printList(node->file);
            cout << endl;
            printTree(node->right);
        }
    }
    bool deleteStringInFile(const string &filename, const string &searchString)
    {
        ifstream inFile(filename);
        if (!inFile.is_open())
        {

            return false;
        }

        string line;
        string content;

        while (getline(inFile, line))
        {
            size_t pos = line.find(searchString);
            if (pos != string::npos)
            {

                line.erase(pos, searchString.length());
            }
            content += line + '\n';
        }

        inFile.close();

        ofstream outFile(filename);
        if (!outFile.is_open())
        {
            cout << "Error: Repository not deleted."<< endl;
            return false;
        }
        outFile << content;
        outFile.close();

        cout <<"Repository deleted successfully."<< endl;
        return true;
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
    string username,commit,files;
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
                cout<<"Enter the commit data: ";
                cin>>commit;
                cout<<"Enter the file name: ";
                cin>>files;
               repo.createRepository(username + "_repositories.csv", user->username, commit, files);
            }
            
            
            break;
        case 6:
            cout << "Enter the repository name: ";
            cin >> username;
            repo.deleterepository(username, user->username);
            cout << "Deleting repository..." << endl;
            break;
        case 7:
            cout << "Enter the repository name: ";
            username="";
            cin >> username;
            
            if(repo.searchRepository(username+ "_repositories.csv")){
                cout << "Repository found." << endl;
                repo.printtree();

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
