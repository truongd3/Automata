#include <iostream>
#include <string>
#include <stack>
using namespace std;

string state = "q0", epsilon = "\u03B5", inputOP = "+-*/", inputN = "0123456789", reason;
char current, a = 'a', b = 'b', dot = '.', openPA = '(', closePA = ')';
int idx; // Index of input string s

void printIntro() {
    cout << "Project 2 for CS 341\n";
    cout << "Section number: 006\n";
    cout << "Semester: Spring 2024\n";
    cout << "Written by: Truong Dang, tdd4@njit.edu\n";
    cout << "Instructor: Arashdeep Kaur, ak3257@njit.edu\n\n";
}

void printTransition(string presentState, char currInput, stack<string> stack, string pop, string push, string nextState) {
    cout << "Present State: " << presentState << endl;
    cout << "Current input symbol under R-head: " << currInput << endl;
    string stackTop = epsilon; // Initialize stackTop in case stack is empty
    if (!stack.empty()) stackTop = stack.top();
    cout << "Stack Top: " << stackTop << endl;
    cout << "Symbol popped from Stack: " << pop << endl;
    cout << "Symbol pushed onto Stack: " << push << endl;
    cout << "Next State: " << nextState << endl;
    cout << "----------\n";
}

void printResult(string s, bool Accept, string reason) {
    if (Accept) cout << "String w=\"" << s << "\" is ACCEPTABLE." << endl;
    else cout << "String w=\"" << s << "\" is NOT ACCEPTABLE by the given PDA because " << reason << endl;
}

int main() {
    printIntro();

    int n; // Number of input strings
    cout << "Enter the number of input strings to be processed: ";
    cin >> n;
    cout << "****************************************************\n\n";
    if (n == 0) return 0;

    string s;
    for (int i = 1; i <= n; i++) {
        cout << "Enter string " << i << " of " << n << ": ";
        cin >> s;
        cout << "--------------------\n";
        stack<string> stack;
        stack.push("Z0"); // Initial step, push Z0 to the stack bottom
        state = "q1"; // Transition from q0 to q1
        reason = "-"; // Reset reason for NOT ACCEPTABLE
        for (idx = 0; idx < s.size(); idx++) { // Recognize by inputs
            current = s[idx]; // Current character
            if (current == a) {
                if (state == "q1") {
                    printTransition(state, current, stack, epsilon, "a", "q2");
                    state = "q2";
                    stack.push("a");
                } else if (state == "q2") {
                    printTransition(state, current, stack, epsilon, "a", "q3");
                    state = "q3";
                    stack.push("a");
                } else if (state == "q6" || state == "q7") {
                    printTransition(state, current, stack, "a", epsilon, "q8");
                    state = "q8";
                    if (!stack.empty() && stack.top() != "a") {
                        reason = "symbol a at index " + to_string(idx) + " needed to be popped off does not match the stack top.";
                        break; // Not match stack top
                    }
                    else stack.pop();
                } else if (state == "q8") {
                    printTransition(state, current, stack, "a", epsilon, "q9");
                    state = "q9";
                    if (!stack.empty() && stack.top() != "a") {
                        reason = "symbol a at index " + to_string(idx) + " needed to be popped off does not match the stack top.";
                        break; // Not match stack top
                    }
                    else stack.pop();
                } else { // Trap case
                    reason = "input a is read at state " + state + ", index " + to_string(idx) + " which is not accepted.";
                    break;
                }
            } else if (current == b) {
                if (state == "q2") {
                    printTransition(state, current, stack, epsilon, "b", "q2");
                    stack.push("b");
                } else if (state == "q8") {
                    printTransition(state, current, stack, "b", epsilon, "q8");
                    if (!stack.empty() && stack.top() != "b") {
                        reason = "symbol b at index " + to_string(idx) + " needed to be popped off does not match the stack top.";
                        break; // Not match stack top
                    }
                    else stack.pop();
                } else { // Trap case
                    reason = "input b is read at state " + state + ", index " + to_string(idx) + " which is not accepted.";
                    break;
                }
            } else if (find(inputN.begin(), inputN.end(), current) != inputN.end()) { // 0 | 1 | 2 | ... | 9
                if (state == "q3" || state == "q4") {
                    printTransition(state, current, stack, epsilon, epsilon, "q4");
                    state = "q4";
                } else if (state == "q5" || state == "q6") {
                    printTransition(state, current, stack, epsilon, epsilon, "q6");
                    state = "q6";
                } else { // Trap case
                    reason = "digit " + string(1, current) + " is read at state " + state + ", index " + to_string(idx) + " which is not accepted.";
                    break;
                }
            } else if (find(inputOP.begin(), inputOP.end(), current) != inputOP.end()) { // + - * /
                if (state == "q6" || state == "q7") {
                    printTransition(state, current, stack, epsilon, epsilon, "q3");
                    state = "q3";
                } else { // Trap case
                    reason = "operator " + string(1, current) + " is read at state " + state + ", index " + to_string(idx) + " which is not accepted.";
                    break;
                }
            } else if (current == dot) {
                if (state == "q3") {
                    printTransition(state, current, stack, epsilon, epsilon, "q5");
                    state = "q5";
                } else if (state == "q4") {
                    printTransition(state, current, stack, epsilon, epsilon, "q6");
                    state = "q6";
                } else { // Trap case
                    reason = "input . is read at state " + state + ", index " + to_string(idx) + " which is not accepted.";
                    break;
                }
            } else if (current == openPA) { // read (
                if (state == "q3") {
                    printTransition(state, current, stack, epsilon, "(", "q3");
                    stack.push("(");
                } else { // Trap case
                    reason = "input ( is read at state " + state + ", index " + to_string(idx) + " which is not accepted.";
                    break;
                }
            } else if (current == closePA) { // read )
                if (state == "q6" || state == "q7") {
                    printTransition(state, current, stack, "(", epsilon, "q7");
                    state = "q7";
                    if (!stack.empty() && stack.top() != "(") {
                        reason = "symbol ( needed to be popped off correspoding to index " + to_string(idx) + " does not match the stack top.";
                        break; // Not match stack top
                    }
                    else stack.pop();
                } else { // Trap case
                    reason = "input ) is read at state " + state + ", index " + to_string(idx) + " which is not accepted.";
                    break;
                }
            }
        }
        if (reason.size() > 1) printResult(s, false, reason);
        else if (state != "q9") printResult(s, false, "it does not reach final state q10.");
        else if (stack.size() > 1) printResult(s, false, "stack is not empty.");
        else if (idx < s.size()-1) printResult(s, false, "input string is not parsed completely.");
        else printResult(s, true, reason);
        cout << "****************************************************\n";
    }
    return 0;
}