#include <iostream>
#include <string>
#include <map>
#include <vector>
using namespace std;

enum AccessLevels {
    OWNER,
    EDITOR,
    READER
};

class Document {
private:
    static int totalDocs;
    int id;
    string name;
    string content;
    map<int, AccessLevels> userIdtoAccess;

public:
    Document(string name, int userId) {
        totalDocs++;
        this->name = name;
        this->id = totalDocs; 
        this->content = "";
        userIdtoAccess[userId] = OWNER;
    }

    int getId() {
        return id;
    }

    string getName() {
        return name;
    }

    string getContent(int userId) {
        AccessLevels userAccess = userIdtoAccess[userId];
        AccessLevels readableAccess[] = {OWNER, READER, EDITOR};
        for (AccessLevels access : readableAccess) {
            if (userAccess == access) {
                return content;
            }
        }
        return "You are not authorised to access this document";
    }

    void setContent(string content, int userId) {
        AccessLevels userAccess = userIdtoAccess[userId];
        if (userAccess != OWNER && userAccess != EDITOR) {
            cout << "Sorry! you are not allowed to access this." << endl;
            return;
        }
        this->content = content;
    }

    map<int, AccessLevels> getAllUsers() {
        return userIdtoAccess;
    }

    AccessLevels getUserIdAccess(int userId) {
        return userIdtoAccess[userId];
    }

    void setUserIdAccess(int userId, AccessLevels access) {
        userIdtoAccess[userId] = access;
    }
};

int Document::totalDocs = 0;

class User {
private:
    static int totalUser;
    int id;
    string name;
    vector<Document> documents;

public:
    User(string name) {
        this->name = name;
        this->id = ++totalUser;
    }

    int getId() {
        return id;
    }

    void createDocument(string docName) {
        Document doc(docName, this->id);
        documents.push_back(doc);
    }

    void grantEditAccess(int userId, int docId) {
        for (auto &doc : documents) {
            if (doc.getId() == docId) {
                doc.setUserIdAccess(userId, EDITOR);
                cout << "Editor Access granted to user: " << userId << endl;
                return;
            }
        }
        cout << "The doc you are searching is not available" << endl;
    }

    void grantReadAccess(int userId, int docId) {
        for (auto &doc : documents) {
            if (doc.getId() == docId) {
                doc.setUserIdAccess(userId, READER);
                cout << "Reader Access granted to user: " << userId << endl;
                return;
            }
        }
        cout << "The doc you are searching is not available" << endl;
    }

    vector<Document> getAllDocuments() {
        return documents;
    }
};

int User::totalUser = 0;

int main() {
    
    User user1("Shyam");
    User user2("Ram");

    user1.createDocument("myfirstDoc");
    user1.createDocument("mysecondDoc");

    vector<Document> documents = user1.getAllDocuments();
    Document doc1 = documents[0];
    Document doc2 = documents[1];
    doc1.setContent("My first line in the document", user1.getId());

    doc2.setContent("My first line in the second document", user2.getId());

    cout << doc1.getContent(user1.getId()) << endl;
    cout << doc2.getContent(user2.getId()) << endl;

    cout << "Adding access for the user2" << endl;
    user1.grantReadAccess(user2.getId(), doc1.getId());

    cout << doc1.getContent(user2.getId()) << endl;

    return 0;
}

