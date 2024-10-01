#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Book {
public:
    string id;
    string title;
    vector<string> authors;
    vector<string> publishers;
    vector<bool> copies; // true if the copy is available, false if borrowed

    Book(string id, string title, vector<string> authors, vector<string> publishers, int copyCount) 
        : id(id), title(title), authors(authors), publishers(publishers) {
        copies.resize(copyCount, true); // Initialize all copies as available
    }
};

class Library {
private:
    map<string, Book> books;
    map<string, string> users; // User ID and Name
    int totalRacks;

public:
    Library(int totalRacks) : totalRacks(totalRacks) {
        cout << "Created library with " << totalRacks << " racks" << endl;
    }

    void addBook(string id, string title, vector<string> authors, vector<string> publishers, int copyCount) {
        if (books.size() >= totalRacks) {
            cout << "No more space to add new books." << endl;
            return;
        }
        books[id] = Book(id, title, authors, publishers, copyCount);
        cout << "Added Book: " << title << " with " << copyCount << " copies." << endl;
    }

    void borrowBook(string bookId, string userId) {
        if (books.find(bookId) == books.end()) {
            cout << "Invalid Book ID" << endl;
            return;
        }

        Book &book = books[bookId];
        for (size_t i = 0; i < book.copies.size(); i++) {
            if (book.copies[i]) { // Check for available copy
                book.copies[i] = false; // Mark as borrowed
                cout << "User " << userId << " borrowed Book: " << book.title << " (Copy " << i + 1 << ")" << endl;
                return;
            }
        }
        cout << "Book: " << book.title << " is not available for borrowing." << endl;
    }

    void returnBook(string bookId, int copyNumber) {
        if (books.find(bookId) == books.end()) {
            cout << "Invalid Book ID" << endl;
            return;
        }

        Book &book = books[bookId];
        if (copyNumber < 1 || copyNumber > book.copies.size()) {
            cout << "Invalid copy number." << endl;
            return;
        }

        book.copies[copyNumber - 1] = true; // Mark as available
        cout << "Returned Book: " << book.title << " (Copy " << copyNumber << ")" << endl;
    }

    void search(string attribute, string value) {
        for (const auto& [bookId, book] : books) {
            if ((attribute == "book_id" && book.id == value) || 
                (attribute == "author" && find(book.authors.begin(), book.authors.end(), value) != book.authors.end()) ||
                (attribute == "publisher" && find(book.publishers.begin(), book.publishers.end(), value) != book.publishers.end())) {
                cout << "Found Book: " << book.title << " (ID: " << book.id << ")" << endl;
                for (size_t i = 0; i < book.copies.size(); i++) {
                    cout << "Copy " << i + 1 << (book.copies[i] ? " - Available" : " - Borrowed") << endl;
                }
            }
        }
    }
};

int main() {
    Library library(10);

    // Adding books
    library.addBook("1", "book1", {"author1", "author2"}, {"publisher1"}, 3);
    library.addBook("2", "book2", {"author2", "author3"}, {"publisher2"}, 5);
    library.addBook("3", "book3", {"author2"}, {"publisher2"}, 3);

    // Searching for books
    library.search("book_id", "1");
    library.search("book_id", "3");
    library.search("author", "author2");
    library.search("publisher", "publisher2");

    // Borrowing books
    library.borrowBook("1", "user1");
    library.borrowBook("1", "user2"); // Borrow another copy
    library.borrowBook("2", "user1");

    // Searching after borrowing
    library.search("book_id", "1");
    library.search("book_id", "2");

    // Returning a book
    library.returnBook("1", 1); // Return the first copy of book 1
    library.returnBook("2", 1); // Return the first copy of book 2

    // Searching after returning
    library.search("book_id", "1");
    library.search("book_id", "2");

    return 0;
}
