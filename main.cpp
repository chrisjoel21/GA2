#include <iostream>
#include "ArgumentManager.h"
#include <stack>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <queue>
using namespace std;

struct node {
    string id;
    string decodedID;
    bool guilt_status;
    int leading_zeros;
    node* next;
    node* prev;
    long long int data;
};

class linkedlist{ // MAKE SURE YOU DO FUNCTIONS RECURSIVELY
private:
    node* head;
    node* tail;
    int size;

public:
    linkedlist() {head = tail = nullptr; size = 0;}
    ~linkedlist();
    void setHead(node* cu){head = cu;}
    node* getHead() {return head;}
    node* getTail() {return tail;}
    int getSize(){return size;};
    bool isEmpty() {return head == nullptr;};
    void insertAtHead(string, string, bool, long long int);
    node* recurSelectionSort(node* head);
    node* sort_ll(node* head);
};


node* linkedlist::sort_ll(node* head) {
    if(head == NULL)
        return head;  //should be base case
    else
    {
        long long int max = 00000;
        node* tmp = head;
        node* to_move = head;

        while(tmp != NULL)  //find max value and store it into to_move
        {
            if(tmp->data < max) {
                max = tmp->data;
                to_move = tmp;
            }

            tmp = tmp->next;
        }

        tmp = head;

        if(to_move == head)  //if it's already in head position
            return sort_ll(head->next);

        while(tmp->next != to_move)  //find it
            tmp = tmp->next;

        tmp->next = tmp->next->next;  //move it to head
        to_move->next = head;
        head = to_move;

        return sort_ll(head->next);  //call function again
    }
}

node* linkedlist::recurSelectionSort(node* head) {
    setHead(head);
    if(head->next == nullptr) {
        return head;
    }
    node* min = head;
    node* beforeMin = nullptr;
    node* ptr = head;

    while(ptr->next != nullptr){
        if (min->data > ptr->next->data){
            min = ptr->next;
            beforeMin = ptr;
        }
        ptr = ptr->next;
    }



    if(min != head){
        node* head_ref = beforeMin;
        beforeMin->next = head;

        node* temp = min->next;
        min->next = head->next;
        head->next = temp;
    }

    head->next = recurSelectionSort(head->next);
    return head;
}
linkedlist::~linkedlist() {
    node* cu = head;
    while (!isEmpty()) {
        head = head->next;
        delete cu;
        cu = head;
    }
}

void linkedlist::insertAtHead(string code, string decode, bool status, long long int data_){
    if(getHead() == nullptr){
        node *newNode = new node;
        newNode->id = code;
        newNode->decodedID = decode;
        newNode->guilt_status = status;
        newNode->data = data_;
        newNode->prev = nullptr;
        setHead(newNode);
        size++;
        return;
    }
    else{
        node *temp = new node;
        temp->id = code;
        temp->decodedID = decode;
        temp->guilt_status = false;
        temp->data = data_;
        temp->next = head;
        head->prev = temp;
        setHead(temp);
        size++;
    }
}
int find_leading_zero(string str){
    int count = 0;

    while(str.find('0') == 0){
        count++;
        str = str.substr(1, str.size() - 1);
    }
    return count;
}

string decode(string line){
    stack<char> st;
    for (int i = 0; i < line.length(); i++){
        if (line[i] != ')'){
            st.push(line[i]); //add to stack to easily access the top or parts added last mainly needed for ()
        }
        else{
            queue<char> q;
            while (st.top() != '('){ //add to queue then back to the stack to reverse it easily
                q.push(st.top());
                st.pop();
            }
            st.pop(); // pop '('
            while (!q.empty()){
                st.push(q.front());
                q.pop();
            }
        }
    }
    string dec = "";//string to return
    while (!st.empty()){
        dec= st.top()+ dec;
        st.pop();
    }
    return dec;
}


int main(int argc, char* argv[]) {

    ArgumentManager am(argc, argv);
    ifstream input(am.get("input"));
    ofstream out(am.get("output"));

    //ifstream input("input13.txt");
    //ofstream out("output1.txt");

    linkedlist list;
    string line = "";
    vector<string> codes;
    vector<string> decodes;
    int guilty = 0;
    int innocent = 0;
    int biggest_number_size = 0;
    while(getline(input, line)){
        line.erase(remove(line.begin(), line.end(), '\n'), line.end());
        line.erase(remove(line.begin(), line.end(), '\r'), line.end());
        line.erase(remove(line.begin(), line.end(), ' '), line.end());

        if(line.find('(') == -1 || line.find(')') == -1){
            continue;
        }
        if(line == "Bar1" || line == "Bar2"){
            continue;
        }
        bool valid = true;
        string checker = line;
        while(checker.find('(') != -1){
            if((checker.find('(') != -1 && checker.find(')') == -1) || (checker.find('(') == -1 && checker.find(')') != -1)){
                valid = false;
                break;
            }
            else{
                int pos1 = checker.find('(');
                int pos2 = checker.find(')');
                checker.erase(pos2, 1);
                checker.erase(pos1, 1);

            }
        }
        if(!valid){
            continue;
        }
        codes.push_back(line);
        decodes.push_back(decode(line));

        string decodedLine = decode(line);
        //string data = decodedLine.substr(find_leading_zero(decode(line)), decode(line).size());
        long long int data_ = stoll(decode(line), nullptr, 10);
        list.insertAtHead(line, decode(line), false, data_);

    }




    innocent = list.getSize();
    for(int i = 0; i < list.getSize(); i++){
        bool breakOut = false;
        node* cu = list.getHead();
        for(int j = 0; j < i; j++){
            cu = cu->next;
        }
        node* temp = cu->next;
        while(temp != nullptr){
            if(temp->decodedID == cu->decodedID){
                cu->guilt_status = true;
                temp->guilt_status = true;
                breakOut = true;
                innocent -= 2;
                guilty += 2;
                break;
            }
            else{
                temp = temp->next;
            }
        }
        cu = list.getHead();

        for(int j = 0; j < i; j++){
            cu = cu->next;
        }
        temp = cu->prev;
        while(temp != nullptr){
            if(breakOut){
                break;
            }
            if(temp->decodedID == cu->decodedID){
                cu->guilt_status = true;
                temp->guilt_status = true;
                breakOut = true;
                innocent -= 2;
                guilty += 2;
                break;
            }
            else{
                temp = temp->prev;
            }
        }
        continue;
    }

    node *trav = list.getHead();


    vector<long long int> vec1;//innnocent
    vector<long long int> vec2;//guilty


    int z = 0;
    int x = 0;

    while(trav != nullptr){
        if(trav->guilt_status){
            vec2.push_back(stoll("1" + trav->decodedID));
            trav = trav->next;
        }
        else{
            string a ="1"  + trav->decodedID;
            long long int b = stoll(a);
            vec1.push_back((b));
            trav = trav->next;
        }
    }


    sort(vec2.begin(), vec2.end());
    sort(vec1.begin(), vec1.end());
    vector<string> newVec1;
    vector<string> newVec2;

    for(int i = 0; i < vec2.size(); i++){
        newVec2.push_back(to_string(vec2[i]).erase(0,1));
    }
    for(int i = 0; i < vec1.size(); i++){
        newVec1.push_back(to_string(vec1[i]).erase(0,1));
    }
    bool printedStat = false;
    while(newVec2.size() != 0){
        if(!printedStat){
            printedStat = true;
            out << "Guilty:" << endl;
        }
        string a = newVec2[0];
        for(int i = 0; i < newVec2.size(); i++){
            if(newVec2[i] == a){
                newVec2.erase(newVec2.begin() + i);
            }
        }
        out << a << endl;
        newVec2.erase(newVec2.begin());
    }
    printedStat = false;
    while(newVec1.size() != 0){
        if(!printedStat){
            printedStat = true;
            out << "Innocent:" << endl;
        }
        string a = newVec1[0];
        out << a << endl;
        newVec1.erase(newVec1.begin());
    }

    //cout << "compiled" << endl;

    return 0;

}