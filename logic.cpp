#include <iostream>
#include <string>
#include <cctype>

using namespace std;

struct NodeS {
    char key;
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

void push(stack& s, char val) {
    NodeS* newNode = new NodeS;
    newNode->key = val;
    newNode->next = s.head;
    s.head = newNode;
    s.size++;
}

char pop(stack& s) {
    if (s.head == nullptr) return 0;
    NodeS* temp = s.head;
    char val = temp->key;
    s.head = s.head->next;
    delete temp;
    s.size--;
    return val;
}

char top(stack& s) {
    if (s.head == nullptr) return 0;
    return s.head->key;
}

bool isEmpty(stack& s) {
    return s.size == 0;
}

void clear(stack& s) {
    while (!isEmpty(s)) pop(s);
}

int priority(char op) {
    if (op == '!') return 3;
    if (op == '&') return 2;
    if (op == '|' || op == '^') return 1;
    return 0;
}

void performOp(stack& vals, stack& ops) {
    char op = pop(ops);

    if (op == '!') {
        char val = pop(vals);
        push(vals, (val == '1' ? '0' : '1'));
    }
    else {
        int b = pop(vals) - '0';
        int a = pop(vals) - '0';

        int res = 0;
        if (op == '&') res = a & b;
        else if (op == '|') res = a | b;
        else if (op == '^') res = a ^ b;

        push(vals, res + '0');
    }
}

int main() {
    string expr;
    getline(cin, expr);

    stack vals;
    init(vals);
    
    stack ops;
    init(ops);

    for (int i = 0; i < expr.length(); i++) {
        char c = expr[i];
        if (c == ' ') continue;

        if (c == '(') {
            push(ops, '(');
        }
        else if (c == '0' || c == '1') {
            push(vals, c);
        }
        else if (c == ')') {
            while (!isEmpty(ops) && top(ops) != '(') {
                performOp(vals, ops);
            }
            pop(ops);
        }
        else {
            while (!isEmpty(ops) && top(ops) != '(') {
                if (c == '!') break; 

                if (priority(top(ops)) >= priority(c)) {
                    performOp(vals, ops);
                } else {
                    break;
                }
            }
            push(ops, c);
        }
    }

    while (!isEmpty(ops)) {
        performOp(vals, ops);
    }

    if (!isEmpty(vals)) {
        cout << pop(vals) << endl;
    } else {
        cout << "Error" << endl; 
    }

    clear(vals);
    clear(ops);

    return 0;
}
