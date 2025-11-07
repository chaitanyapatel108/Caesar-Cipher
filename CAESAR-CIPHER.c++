#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
using namespace std;

void display_logo() {
    cout << R"(
,adPPYba, ,adPPYYba,  ,adPPYba, ,adPPYba, ,adPPYYba, 8b,dPPYba,  
a8"     "" ""     `Y8 a8P_____88 I8[    "" ""     `Y8 88P'   "Y8  
8b         ,adPPPPP88 8PP"""""""  `"Y8ba,  ,adPPPPP88 88          
"8a,   ,aa 88,    ,88 "8b,   ,aa aa    ]8I 88,    ,88 88          
 `"Ybbd8"' `"8bbdP"Y8  `"Ybbd8"' `"YbbdP"' `"8bbdP"Y8 88          
            88             88                                   
           ""             88                                   
                          88                                   
 ,adPPYba, 88 8b,dPPYba,  88,dPPYba,   ,adPPYba, 8b,dPPYba,  
a8"     "" 88 88P'    "8a 88P'    "8a a8P_____88 88P'   "Y8  
8b         88 88       d8 88       88 8PP""""""" 88          
"8a,   ,aa 88 88b,   ,a8" 88       88 "8b,   ,aa 88          
 `"Ybbd8"' 88 88`YbbdP"'  88       88  `"Ybbd8"' 88          
              88                                            
              88                                            
)";
}

vector<char> alphabet = {
    'a','b','c','d','e','f','g','h','i','j','k','l','m',
    'n','o','p','q','r','s','t','u','v','w','x','y','z'
};

vector<char> digits = {'0','1','2','3','4','5','6','7','8','9'};
vector<char> symbols = {'!','@','#','$','%','^','&','*','(',')'};

string caesar(string original_text, string encode_or_decode) {
    int shift_amount = (encode_or_decode == "decode") ? -7 : 7;
    string output_text = "";

    for (char letter : original_text) {
        if (find(alphabet.begin(), alphabet.end(), letter) != alphabet.end()) {
            int pos = find(alphabet.begin(), alphabet.end(), letter) - alphabet.begin();
            int shifted = (pos + shift_amount + 26) % 26;
            output_text += alphabet[shifted];
        } else if (find(digits.begin(), digits.end(), letter) != digits.end()) {
            int pos = find(digits.begin(), digits.end(), letter) - digits.begin();
            output_text += (encode_or_decode == "encode") ? symbols[pos] : digits[pos];
        } else {
            output_text += letter;
        }
    }

    return output_text;
}

void save_message(const string& message, const string& password) {
    ofstream file("messages.txt", ios::app);
    if (file.is_open()) {
        file << message << "|" << password << endl;  // Save as "encrypted_message|password"
        file.close();
    } else {
        cout << "Error: Unable to open file for writing." << endl;
    }
}

vector<pair<string, string>> load_messages() {
    vector<pair<string, string>> messages;
    ifstream file("messages.txt");
    string line;
    while (getline(file, line)) {
        size_t delimiter = line.find('|');
        if (delimiter != string::npos) {
            string msg = line.substr(0, delimiter);
            string pass = line.substr(delimiter + 1);
            messages.push_back({msg, pass});
        }
    }
    return messages;
}

void clear_messages() {
    ofstream file("messages.txt", ios::trunc);
    if (file.is_open()) {
        file.close();
        cout << "All messages cleared from file." << endl;
    } else {
        cout << "Error: Unable to clear file." << endl;
    }
}

void delete_message(int index) {
    vector<pair<string, string>> messages = load_messages();
    if (index < 1 || index > messages.size()) {
        cout << "Invalid message number." << endl;
        return;
    }

    messages.erase(messages.begin() + index - 1);
    ofstream file("messages.txt", ios::trunc);
    for (const auto& msg : messages) {
        file << msg.first << "|" << msg.second << endl;
    }
    file.close();
    cout << "Message " << index << " deleted." << endl;
}

int main() {
    display_logo();
    bool should_continue = true;

    while (should_continue) {
        string direction;
        cout << "\nType 'encode' to encrypt, 'decode' to decrypt, or 'clear' to manage saved messages:\n";
        cin >> direction;

        if (direction == "encode" || direction == "decode") {
            string mode;
            cout << "Do you want to process a 'single' message or 'bulk'?\n";
            cin >> mode;

            if (direction == "encode") {
                cin.ignore();
                if (mode == "single") {
                    string text, password;
                    cout << "Enter your message:\n";
                    getline(cin, text);
                    transform(text.begin(), text.end(), text.begin(), ::tolower);

                    cout << "Set a password for this message:\n";
                    getline(cin, password);

                    string encrypted = caesar(text, "encode");
                    cout << "Encoded & Saved Successfully.\n";
                    save_message(encrypted, password);

                } else if (mode == "bulk") {
                    int count;
                    cout << "How many messages do you want to encode?\n";
                    cin >> count;
                    cin.ignore();
                    for (int i = 0; i < count; ++i) {
                        string text, password;
                        cout << "Enter message " << i + 1 << ":\n";
                        getline(cin, text);
                        transform(text.begin(), text.end(), text.begin(), ::tolower);

                        cout << "Set a password for this message:\n";
                        getline(cin, password);

                        string encrypted = caesar(text, "encode");
                        cout << "Encoded: " << encrypted << endl;
                        save_message(encrypted, password);
                    }
                    cout << "All messages saved to file." << endl;
                }

            } else if (direction == "decode") {
                vector<pair<string, string>> messages = load_messages();
                if (messages.empty()) {
                    cout << "No messages found to decrypt." << endl;
                } else {
                    cout << "\nSaved encrypted messages:\n";
                    for (size_t i = 0; i < messages.size(); ++i) {
                        cout << i + 1 << ": " << messages[i].first << endl;
                    }

                    if (mode == "single") {
                        int choice;
                        cout << "Enter the number of the message to decrypt:\n";
                        cin >> choice;
                        cin.ignore();

                        if (choice >= 1 && choice <= messages.size()) {
                            string password;
                            cout << "Enter the password for this message:\n";
                            getline(cin, password);

                            if (password == messages[choice - 1].second) {
                                string decrypted = caesar(messages[choice - 1].first, "decode");
                                cout << "Decoded Message: " << decrypted << endl;
                            } else {
                                cout << "Incorrect password! Access denied.\n";
                            }
                        } else {
                            cout << "Message " << choice << " does not exist." << endl;
                        }

                    } else if (mode == "bulk") {
                        int start, end;
                        cout << "Enter the starting message number:\n";
                        cin >> start;
                        cout << "Enter the ending message number:\n";
                        cin >> end;
                        cin.ignore();

                        for (int i = start; i <= end; ++i) {
                            if (i >= 1 && i <= messages.size()) {
                                string password;
                                cout << "Enter password for message " << i << ":\n";
                                getline(cin, password);

                                if (password == messages[i - 1].second) {
                                    string decrypted = caesar(messages[i - 1].first, "decode");
                                    cout << "Message " << i << " → Decoded: " << decrypted << endl;
                                } else {
                                    cout << "Message " << i << " → Incorrect password! Access denied.\n";
                                }
                            } else {
                                cout << "Message " << i << " does not exist." << endl;
                            }
                        }
                    }
                }
            }

        } else if (direction == "clear") {
            string option;
            cout << "Type 'all' to clear all messages or 'one' to delete a specific message:\n";
            cin >> option;

            if (option == "all") {
                clear_messages();
            } else if (option == "one") {
                vector<pair<string, string>> messages = load_messages();
                if (messages.empty()) {
                    cout << "No messages to delete." << endl;
                } else {
                    cout << "Saved encrypted messages:\n";
                    for (size_t i = 0; i < messages.size(); ++i) {
                        cout << i + 1 << ": " << messages[i].first << endl;
                    }
                    int index;
                    cout << "Enter the number of the message to delete:\n";
                    cin >> index;
                    delete_message(index);
                }
            } else {
                cout << "Invalid option." << endl;
            }

        } else {
            cout << "Invalid option. Please type 'encode', 'decode', or 'clear'." << endl;
        }

        string restart;
        cout << "\nType 'yes' to go again. Otherwise, type 'no'.\n";
        cin >> restart;
        if (restart == "no") {
            should_continue = false;
            cout << "Goodbye!\n";
        }
    }

    return 0;
}
