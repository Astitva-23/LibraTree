#include <iostream>
#include <fstream>
#include <vector>
#include <string>

class Book {
public:
    Book(int id, const std::string &title, const std::string &author)
        : id(id), title(title), author(author) {}

    int getId() const {
        return id;
    }

    std::string getTitle() const {
        return title;
    }

    std::string getAuthor() const {
        return author;
    }

    void save(std::ofstream &out) const {
        out << id << "\n" << title << "\n" << author << "\n";
    }

    static Book load(std::ifstream &in) {
        int id;
        std::string title, author;
        in >> id;
        in.ignore(); 
        std::getline(in, title);
        std::getline(in, author);
        return Book(id, title, author);
    }

private:
    int id;
    std::string title;
    std::string author;
};

// Library Class Definition
class Library {
public:
    Library() : root(nullptr) {
        loadFromFile();
    }

    ~Library() {
        saveToFile();
        destroy(root);
    }

    void addBook(const Book &book) {
        addBook(root, book);
    }

    void deleteBook(int id) {
        deleteBook(root, id);
    }

    void searchBook(int id) const {
        searchBook(root, id);
    }

    void inOrder() const {
        inOrder(root);
    }

private:
    struct Node {
        Book book;
        Node *left;
        Node *right;
        Node(const Book &book) : book(book), left(nullptr), right(nullptr) {}
    };
    Node *root;

    void addBook(Node *&node, const Book &book) {
        if (node == nullptr) {
            node = new Node(book);
        } else if (book.getId() < node->book.getId()) {
            addBook(node->left, book);
        } else {
            addBook(node->right, book);
        }
    }

    void deleteBook(Node *&node, int id) {
        if (node == nullptr) return;
        if (id < node->book.getId()) {
            deleteBook(node->left, id);
        } else if (id > node->book.getId()) {
            deleteBook(node->right, id);
        } else {
            if (node->left == nullptr && node->right == nullptr) {
                delete node;
                node = nullptr;
            } else if (node->left == nullptr) {
                Node *temp = node;
                node = node->right;
                delete temp;
            } else if (node->right == nullptr) {
                Node *temp = node;
                node = node->left;
                delete temp;
            } else {
                Node *temp = findMin(node->right);
                node->book = temp->book;
                deleteBook(node->right, temp->book.getId());
            }
        }
    }

    Node* findMin(Node *node) const {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    void searchBook(Node *node, int id) const {
        if (node == nullptr) {
            std::cout << "Book not found!" << std::endl;
            return;
        }
        if (id < node->book.getId()) {
            searchBook(node->left, id);
        } else if (id > node->book.getId()) {
            searchBook(node->right, id);
        } else {
            std::cout << "Book found: " << node->book.getTitle() << " by " << node->book.getAuthor() << std::endl;
        }
    }

    void inOrder(Node *node) const {
        if (node != nullptr) {
            inOrder(node->left);
            std::cout << node->book.getId() << ": " << node->book.getTitle() << " by " << node->book.getAuthor() << std::endl;
            inOrder(node->right);
        }
    }

    void destroy(Node *node) {
        if (node != nullptr) {
            destroy(node->left);
            destroy(node->right);
            delete node;
        }
    }

    void saveToFile() const {
        std::ofstream outFile("library.txt");
        if (outFile.is_open()) {
            saveToFile(root, outFile);
            outFile.close();
        } else {
            std::cerr << "Unable to open file for saving!" << std::endl;
        }
    }

    void saveToFile(Node *node, std::ofstream &outFile) const {
        if (node != nullptr) {
            node->book.save(outFile);
            saveToFile(node->left, outFile);
            saveToFile(node->right, outFile);
        }
    }

    void loadFromFile() {
        std::ifstream inFile("library.txt");
        if (inFile.is_open()) {
            while (inFile.peek() != EOF) {
                Book book = Book::load(inFile);
                addBook(book);
            }
            inFile.close();
        } else {
            std::cerr << "Unable to open file for loading!" << std::endl;
        }
    }
};

void addBook(Library &library) {
    int id;
    std::string title, author;
    
    std::cout << "Enter book ID: ";
    std::cin >> id;
    std::cout << "Enter book title: ";
    std::cin.ignore();
    std::getline(std::cin, title);
    std::cout << "Enter book author: ";
    std::getline(std::cin, author);
    
    Book book(id, title, author);
    library.addBook(book);
    std::cout << "Book added successfully!" << std::endl;
}

void deleteBook(Library &library) {
    int id;
    std::cout << "Enter book ID to delete: ";
    std::cin >> id;
    library.deleteBook(id);
    std::cout << "Book deleted successfully!" << std::endl;
}

void searchBook(Library &library) {
    int id;
    std::cout << "Enter book ID to search: ";
    std::cin >> id;
    library.searchBook(id);
}

void viewBooks(Library &library) {
    library.inOrder();
}

int main() {
    Library library;
    int choice;
    
    do {
        std::cout << "Library Management System" << std::endl;
        std::cout << "1. Add Book" << std::endl;
        std::cout << "2. Delete Book" << std::endl;
        std::cout << "3. Search Book" << std::endl;
        std::cout << "4. View Books" << std::endl;
        std::cout << "5. Save & Exit" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                addBook(library);
                break;
            case 2:
                deleteBook(library);
                break;
            case 3:
                searchBook(library);
                break;
            case 4:
                viewBooks(library);
                break;
            case 5:
                std::cout << "Exiting..." << std::endl;
                break;
            default:
                std::cout << "Invalid choice! Please try again." << std::endl;
        }
    } while (choice != 5);
    
    return 0;
}
