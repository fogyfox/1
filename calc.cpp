#include <iostream>
#include <string>
#include <cctype>
#include <cstdlib>

using namespace std;

struct NodeS {
    string key;
    NodeS* next;
};

struct stack {
    NodeS* head;
    int size;
};

void init(stack& s) {
    s.head = nullptr;
    s.size = 0;
}

void push(stack& s, string val) {
    NodeS* newNode = new NodeS;
    newNode->key = val;
    newNode->next = s.head;
    s.head = newNode;
    s.size++;
}

string pop(stack& s) {
    if (s.head == nullptr) return "";
    NodeS* temp = s.head;
    string val = temp->key;
    s.head = s.head->next;
    delete temp;
    s.size--;
    return val;
}

string top(stack& s) {
    if (s.head == nullptr) return "";
    return s.head->key;
}

bool isEmpty(stack& s) {
    return s.size == 0;
}

void clear(stack& s) {
    while (!isEmpty(s)) {
        pop(s);
    }
}

int priority(string op) {
    if (op == "+" || op == "-") return 1;
    if (op == "*" || op == "/") return 2;
    return 0;
}

void performOp(stack& nums, stack& ops) {
    if (isEmpty(nums)) throw "Error";
    long long b = stoll(pop(nums));
    
    if (isEmpty(nums)) throw "Error";
    long long a = stoll(pop(nums));

    string op = pop(ops);

    long long res = 0;
    if (op == "+") res = a + b;
    else if (op == "-") res = a - b;
    else if (op == "*") res = a * b;
    else if (op == "/") {
        if (b == 0) throw "Error";
        res = a / b;
    }
    
    push(nums, to_string(res));
}

int main() {
    string expr;
    getline(cin, expr);

    stack nums;
    init(nums);
    
    stack ops;
    init(ops);

    bool expectingOperand = true;

    try {
        for (int i = 0; i < expr.length(); i++) {
            if (expr[i] == ' ') continue;

            if (expr[i] == '(') {
                push(ops, "(");
                expectingOperand = true;
            }
            else if (isdigit(expr[i])) {
                string numStr;
                while (i < expr.length() && isdigit(expr[i])) {
                    numStr += expr[i];
                    i++;
                }
                i--;
                push(nums, numStr);
                expectingOperand = false;
            }
            else if (expr[i] == ')') {
                while (!isEmpty(ops) && top(ops) != "(") {
                    performOp(nums, ops);
                }
                if (isEmpty(ops)) throw "Error";
                pop(ops);
                expectingOperand = false;
            }
            else {
                string currentOp(1, expr[i]);

                if (currentOp == "-" && expectingOperand) {
                    push(nums, "0");
                    push(ops, "-");
                } 
                else {
                    while (!isEmpty(ops) && priority(top(ops)) >= priority(currentOp)) {
                        performOp(nums, ops);
                    }
                    push(ops, currentOp);
                }
                expectingOperand = true;
            }
        }

        while (!isEmpty(ops)) {
            if (top(ops) == "(") throw "Error";
            performOp(nums, ops);
        }

        if (nums.size != 1) throw "Error";

        cout << pop(nums) << endl;

    } catch (...) {
        cout << "Error" << endl;
    }

    clear(nums);
    clear(ops);

    return 0;
}
