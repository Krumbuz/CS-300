#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Structure to represent a course
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

// Node of a binary search tree
struct Node {
    Course course;
    Node* left;
    Node* right;
    Node(Course c) : course(c), left(nullptr), right(nullptr) {}
};

// BST root
Node* root = nullptr;

// Helper to insert course into BST
Node* insert(Node* node, Course course) {
    if (node == nullptr) return new Node(course);

    if (course.courseNumber < node->course.courseNumber) {
        node->left = insert(node->left, course);
    } else {
        node->right = insert(node->right, course);
    }
    return node;
}

// In-order print to show sorted course list
void printInOrder(Node* node) {
    if (node == nullptr) return;

    printInOrder(node->left);
    cout << node->course.courseNumber << ", " << node->course.courseTitle << endl;
    printInOrder(node->right);
}

// Find a course by course number
Course* search(Node* node, const string& courseNumber) {
    if (node == nullptr) return nullptr;

    if (courseNumber == node->course.courseNumber) return &node->course;

    if (courseNumber < node->course.courseNumber) {
        return search(node->left, courseNumber);
    } else {
        return search(node->right, courseNumber);
    }
}

// Load course data from file into BST
void loadCourses(const string& fileName) {
    ifstream file(fileName);
    if (!file.is_open()) {
        cout << "Error: Unable to open file " << fileName << endl;
        return;
    }

    string line;
    vector<vector<string>> rawLines;
    vector<string> allCourseNumbers;

    // First pass - store raw lines and collect all course numbers
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string token;
        vector<string> tokens;

        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() < 2) {
            cout << "Invalid line format: " << line << endl;
            continue;
        }

        rawLines.push_back(tokens);
        allCourseNumbers.push_back(tokens[0]);
    }
    file.close();

    // Validation pass
    for (auto& tokens : rawLines) {
        for (size_t i = 2; i < tokens.size(); ++i) {
            if (find(allCourseNumbers.begin(), allCourseNumbers.end(), tokens[i]) == allCourseNumbers.end()) {
                cout << "Error: Prerequisite " << tokens[i] << " not found in course list." << endl;
                return;
            }
        }
    }

    // Insert into BST
    for (auto& tokens : rawLines) {
        Course course;
        course.courseNumber = tokens[0];
        course.courseTitle = tokens[1];
        for (size_t i = 2; i < tokens.size(); ++i) {
            course.prerequisites.push_back(tokens[i]);
        }
        root = insert(root, course);
    }

    cout << "Courses successfully loaded!" << endl;
}

// Display course info and prerequisites
void displayCourseInfo(const string& courseNumber) {
    Course* course = search(root, courseNumber);
    if (course == nullptr) {
        cout << "Course not found." << endl;
        return;
    }

    cout << course->courseNumber << ", " << course->courseTitle << endl;

    if (!course->prerequisites.empty()) {
        cout << "Prerequisites: ";
        for (size_t i = 0; i < course->prerequisites.size(); ++i) {
            cout << course->prerequisites[i];
            if (i < course->prerequisites.size() - 1) cout << ", ";
        }
        cout << endl;
    } else {
        cout << "Prerequisites: None" << endl;
    }
}

// Menu interface
void displayMenu() {
    int choice = 0;
    string filename;
    while (choice != 9) {
        cout << "\n===== Course Planner Menu =====" << endl;
        cout << "1. Load Data Structure" << endl;
        cout << "2. Print Course List" << endl;
        cout << "3. Print Course" << endl;
        cout << "9. Exit\n" << endl;
        cout << "What would you like to do? ";

        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                cout << "Enter filename to load: ";
                getline(cin, filename);
                loadCourses(filename);
                break;
            case 2:
                printInOrder(root);
                break;
            case 3: {
                string courseNumber;
                cout << "Enter course number: ";
                getline(cin, courseNumber);
                transform(courseNumber.begin(), courseNumber.end(), courseNumber.begin(), ::toupper);
                displayCourseInfo(courseNumber);
                break;
            }
            case 9:
                cout << "Thank you for using the Course Planner!" << endl;
                break;
            default:
                cout << "Invalid option. Please try again." << endl;
        }
    }
}

int main() {
    displayMenu();
    return 0;
}
