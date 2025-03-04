#include <iostream>
#include <string>
#include <fstream>
using namespace std;

// Node class for the Binary Search Tree
class TreeNode {
public:
    int roll;
    string name;
    string dept;
    int marks;
    TreeNode* left;
    TreeNode* right;

    TreeNode(int r, const string& n, const string& d, int m)
        : roll(r), name(n), dept(d), marks(m), left(nullptr), right(nullptr) {}
};

// Node class for the Linked List
class ListNode {
public:
    int roll;
    string name;
    string dept;
    int marks;
    ListNode* next;

    ListNode(int r, const string& n, const string& d, int m)
        : roll(r), name(n), dept(d), marks(m), next(nullptr) {}
};

// Tree class for managing student records
class StudentTree {
private:
    TreeNode* root;
    ListNode* head; // Head of the linked list

    // Add to Linked List
    void addToLinkedList(int roll, const string& name, const string& dept, int marks) {
        ListNode* newNode = new ListNode(roll, name, dept, marks);
        if (!head) {
            head = newNode;
        } else {
            ListNode* temp = head;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    // Delete from Linked List
    void deleteFromLinkedList(int roll) {
        if (!head) return;
        if (head->roll == roll) {
            ListNode* temp = head;
            head = head->next;
            delete temp;
            return;
        }
        ListNode* prev = head;
        ListNode* curr = head->next;
        while (curr && curr->roll != roll) {
            prev = curr;
            curr = curr->next;
        }
        if (curr) {
            prev->next = curr->next;
            delete curr;
        }
    }

    // Inorder Traversal for BST
    void inorder(TreeNode* node) {
        if (node != nullptr) {
            inorder(node->left);
            cout << node->roll << "\t\t" << node->name << "\t" << node->dept << "\t" << node->marks << endl;
            inorder(node->right);
        }
    }

    // Search in BST
    TreeNode* search(TreeNode* node, int roll) {
        if (node == nullptr || node->roll == roll)
            return node;
        return roll < node->roll ? search(node->left, roll) : search(node->right, roll);
    }

    // Delete Node from BST
    TreeNode* deleteNode(TreeNode*& node, int roll) {
        if (node == nullptr) return nullptr;
        if (roll < node->roll) {
            node->left = deleteNode(node->left, roll);
        } else if (roll > node->roll) {
            node->right = deleteNode(node->right, roll);
        } else {
            if (node->left == nullptr) {
                TreeNode* temp = node->right;
                delete node;
                return temp;
            } else if (node->right == nullptr) {
                TreeNode* temp = node->left;
                delete node;
                return temp;
            }
            TreeNode* minNode = node->right;
            while (minNode->left != nullptr) {
                minNode = minNode->left;
            }
            node->roll = minNode->roll;
            node->name = minNode->name;
            node->dept = minNode->dept;
            node->marks = minNode->marks;
            node->right = deleteNode(node->right, minNode->roll);
        }
        return node;
    }

    // Count the total number of nodes
    int countNodes(TreeNode* node) {
        if (node == nullptr) return 0;
        return 1 + countNodes(node->left) + countNodes(node->right);
    }

    // Calculate the total marks
    int totalMarks(TreeNode* node) {
        if (node == nullptr) return 0;
        return node->marks + totalMarks(node->left) + totalMarks(node->right);
    }

    void showLinkedList() {
        ListNode* temp = head;
        while (temp) {
            cout << temp->roll << "\t\t" << temp->name << "\t" << temp->dept << "\t" << temp->marks << endl;
            temp = temp->next;
        }
    }

public:
    StudentTree() : root(nullptr), head(nullptr) {}

    void insertRecord(int roll, const string& name, const string& dept, int marks) {
        insert(root, roll, name, dept, marks);
        addToLinkedList(roll, name, dept, marks);
    }

    void insert(TreeNode*& node, int roll, const string& name, const string& dept, int marks) {
        if (node == nullptr) {
            node = new TreeNode(roll, name, dept, marks);
        } else if (roll < node->roll) {
            insert(node->left, roll, name, dept, marks);
        } else if (roll > node->roll) {
            insert(node->right, roll, name, dept, marks);
        }
    }

    void deleteRecord(int roll) {
        root = deleteNode(root, roll);
        deleteFromLinkedList(roll);
    }

    void searchRecord(int roll) {
        TreeNode* found = search(root, roll);
        if (found) {
            cout << "Roll Number: " << found->roll << endl;
            cout << "Name: " << found->name << endl;
            cout << "Department: " << found->dept << endl;
            cout << "Marks: " << found->marks << endl;
        } else {
            cout << "No such record available\n";
        }
    }

    void showRecords() {
        if (root == nullptr) {
            cout << "No student records found.\n";
            return;
        }
        cout << "Displaying All Records (BST - Sorted by Roll Number):\n";
        cout << "Roll Number\t\tName\t\tDepartment\tMarks\n";
        cout << "------------------------------------------------------\n";
        inorder(root);
    }

    void showAllRecordsLinkedList() {
        if (head == nullptr) {
            cout << "No student records in the linked list.\n";
            return;
        }
        cout << "Displaying All Records (Linked List Format):\n";
        cout << "Roll Number\t\tName\t\tDepartment\tMarks\n";
        cout << "------------------------------------------------------\n";
        showLinkedList();
    }

    // Update Record feature
    void updateRecord(int roll, const string& name, const string& dept, int marks) {
        TreeNode* found = search(root, roll);
        if (found) {
            found->name = name;
            found->dept = dept;
            found->marks = marks;

            // Update linked list as well
            deleteFromLinkedList(roll);
            addToLinkedList(roll, name, dept, marks);

            cout << "Record updated successfully\n";
        } else {
            cout << "No such record found\n";
        }
    }

    // Count total records
    void countRecords() {
       int total = countNodes(root);
       cout << "Total number of students: " << total << endl;
   }

   // Calculate average marks
   void averageMarks() {
       int total_students = countNodes(root);
       int total_marks = totalMarks(root);
       if(total_students == 0){
           cout<<"No students available.\n";
       }else{
           double average=(double)total_marks/total_students;
           cout<<"Average Marks: "<<average<<endl;
       }
   }

   // Export records to a file
   void exportRecords(const string& filename) {
       ofstream outFile(filename);
       if (!outFile) {
           cout<<"Error opening file for writing.\n";
           return;
       }

       // Export BST records
       outFile<<"Exporting Records from BST:\n";
       outFile<<"Roll Number\tName\tDepartment\tMarks\n";
       outFile<<"------------------------------------------------------\n";
       exportInOrder(root,outFile);

       // Export Linked List records
       outFile<<"\nExporting Records from Linked List:\n";
       outFile<<"Roll Number\tName\tDepartment\tMarks\n";
       outFile<<"------------------------------------------------------\n";
       exportLinkedList(outFile);

       outFile.close();
       cout<<"Records exported successfully to "<<filename<<endl;
   }

   // Helper function to export BST records in order
   void exportInOrder(TreeNode* node , ofstream &outFile){
       if(node!=nullptr){
           exportInOrder(node -> left,outFile);
           outFile<<node -> roll<<"\t\t"<<node -> name<<"\t"<<node -> dept<<"\t"<<node -> marks<<endl;
           exportInOrder(node -> right,outFile);
       }
   }

   // Helper function to export Linked List records
   void exportLinkedList(ofstream &outFile){
       ListNode *temp=head;
       while(temp){
           outFile<<temp -> roll<<"\t\t"<<temp -> name<<"\t"<<temp -> dept<<"\t"<<temp -> marks<<endl;
           temp=temp -> next;
       }
   }
};

// Simple login function
bool login() {
   const string USERNAME="admin";
   const string PASSWORD="password"; // Change this for production use
   string username,password;

   cout<<"Enter username: "; cin>>username;
   cout<<"Enter password: "; cin>>password;

   return username==USERNAME && password==PASSWORD;
}

// Driver code
int main() {
   if (!login()) {
      cout<<"Invalid username or password. Exiting.\n";
      return 0;
   }

   StudentTree studentTree;
   string name,dept;
   int roll ,marks;

   while(true){
      cout<<"\n\t\tWelcome to Student Record Management System\n\n"
          "\tPress\n\t1 to create a new record\n\t2 to delete a student record\n"
          "\t3 to search a student record\n\t4 to view all students' records\n"
          "\t5 to view all records in linked list format\n\t6 to update a student record\n"
          "\t7 to count total students\n\t8 to view average marks\n"
          "\t9 to export records to file\n\t10 to exit\n";

      int choice;
      cin>>choice;

      switch(choice){
         case 1:
             cout<<"Enter Roll Number: "; cin>>roll;
             cout<<"Enter Name of Student: "; cin.ignore(); getline(cin,name);
             cout<<"Enter Department of Student: "; getline(cin,dept);
             cout<<"Enter Marks of Student: "; cin>>marks;
             studentTree.insertRecord(roll,name,dept ,marks);
             cout<<"Record created successfully.\n";
             break;

         case 2:
             cout<<"Enter Roll Number to delete: "; cin>>roll;
             studentTree.deleteRecord(roll);
             cout<<"Record deleted successfully if it existed.\n";
             break;

         case 3:
             cout<<"Enter Roll Number to search: "; cin>>roll;
             studentTree.searchRecord(roll);
             break;

         case 4:
             studentTree.showRecords();
             break;

         case 5:
             studentTree.showAllRecordsLinkedList();
             break;

         case 6:
             cout<<"Enter Roll Number of student to update: "; cin>>roll;
             cout<<"Enter New Name of Student: "; cin.ignore(); getline(cin,name);
             cout<<"Enter New Department of Student: "; getline(cin,dept);
             cout<<"Enter New Marks of Student: "; cin>>marks;
             studentTree.updateRecord(roll,name,dept ,marks);
             break;

         case 7:
              studentTree.countRecords();
              break;

         case 8:
              studentTree.averageMarks();
              break;

         case 9:
              {
                  cout<<"Enter filename to export records: ";
                  string filename;
                  cin>>filename;
                  studentTree.exportRecords(filename);
                  break;
              }

         case 10:
              cout<<"Exiting the program. Goodbye!\n";
              return 0;

         default:
              cout<<"Invalid choice. Please try again.\n";
              break;
      }
   }
   return 0;
}
