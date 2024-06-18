#include <iostream>
#include <string>
using namespace std;

void printIntro() {
    cout << "Project 1 for CS 341\n";
    cout << "Section number: 006\n";
    cout << "Semester: Spring 2024\n";
    cout << "Written by: Truong Dang, tdd4@njit.edu\n";
    cout << "Instructor: Arashdeep Kaur, ak3257@njit.edu\n\n";
}

string state = "q0", psi = "abcdefghijklmnopqrstuvwxyz";
char pi = '.', phi = '@', *current;

void printState() { // Print normal states
    cout << "Character " << *current << " is in state " << state << endl;
}

void printTrapStates(int idx, string s) { // Print trap states from current idx
    for (; idx < s.size(); idx++) cout << "Character " << s[idx] << " is in trap state q8" << endl;
}

int main() {
    printIntro();

    int n; // Number of input strings
    cout << "Enter the number of input strings to be processed: ";
    cin >> n;
    cout << "****************************************************\n\n";
    if (n == 0) return 0;

    string s; // Input string
    for (int i = 1; i <= n; i++) {
        cout << "Enter string " << i << " of " << n << ": ";
        cin >> s;
        state = "q0";
        cout << "Input string: " << s << " has current state: " << state << endl;
        
        for (int idx = 0; idx < s.size(); idx++) {
            current = &s[idx]; // Current character
            if (idx == 0) { // Beginning (should be q0)
                if (find(psi.begin(), psi.end(), *current) == psi.end()) { // Not a,b,c,..,z
                    state = "q8";
                    printTrapStates(0, s);
                    break;
                }
                else if (find(psi.begin(), psi.end(), *current) != psi.end()) {
                    state = "q1"; // From q0 move to q1
                    printState();
                    continue;
                }
            }
            if (find(psi.begin(), psi.end(), *current) != psi.end()) { // a,b,c,..,z
                if (state == "q0" || state == "q1") state = "q1"; // S1S2*
                else if (state == "q2" || state == "q3") state = "q3"; // the next S1S2*
                else if (state == "q4") { // From q4 (dot sign)
                    if (*current == 'g') state = "q5";
                    else if (*current != 'g') state = "q3";
                }
                else if (state == "q5") { // From q5 (letter g)
                    if (*current == 'o') state = "q6";
                    else if (*current == 'r') state = "q7"; // To final state
                    else state = "q3";
                }
                else if (state == "q6") { // From q6 (letter o)
                    if (*current == 'v') state = "q7"; // To final state
                    else if (*current != 'v') state = "q3";
                }
                else if (state == "q7") state = "q3"; // From q7 received more characters instead of finish
                else { // Trap state
                    state = "q8";
                    printTrapStates(idx, s);
                    break;
                }
                printState();
            }
            else if (*current == phi) { // @
                if (state == "q1") state = "q2";
                else { // Two @ is not accepted
                    state = "q8";
                    printTrapStates(idx, s);
                    break;
                }
                printState();
            }
            else if (*current == pi) { // .
                if (state == "q1") state = "q0"; // Before @
                else if (state == "q3" || state == "q5" || state == "q6" || state == "q7") state = "q4"; // After @
                else { // Successive . is not accepted
                    state = "q8";
                    printTrapStates(idx, s);
                    break;
                }
                printState();
            }
        }
        if (state == "q7") cout << "ACCEPTED\n**********************\n";
        else cout << "REJECTED\n**********************\n";
    }
    return 0;
}