#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <tuple>
#include <sstream>
#include <iomanip>
#include <cstdlib> 


using std::string;
using std::cout;
using std::cin;
using std::endl;
using namespace std;
#include <thread>



void animation(int version){
    if(version==1){
        cout << "" << endl;
        cout << "" << endl;
        cout << "       /\\_/\\        [1] See Entries" << std::endl;
        cout << "      ( o.o )       [2] Create New Entry" << std::endl;
        cout << "       > ^ <  /     [3] Storage Information (filepath,size)" << std::endl;
        cout << "      /       |     [4] Raw content" << std::endl;
        cout << "     (  |  |  )     [5] Quit(cmd+w)" << std::endl;
        cout << "    /   |  |   \\ " << std::endl;
        cout << "   (____|_____)     command ---->  ";
        cout <<"";
    }

    if(version==2){
    cout << "" << endl;
    cout << "" << endl;
    cout << "     ( (\n";
    cout << "      ) )\n";
    cout << "   ........\n";
    cout << "   |      |]\n";
    cout << "   \\      /  \n";
    cout << "    `----'    Please Enter the Password ---> ";
    }

    if(version==3){
    cout << "" << endl;
    cout << "               .--.                                   \n";
    cout << "            .-(    ).             .--.                   \n";
    cout << "           (___.__)__)         .-(    ).                          \n";
    cout << "           *  *  *  *         (___.__)__)                          \n";
    cout << "           *  *  *  *         *  *  *  *                          \n";
    cout << "\n";
}
}

void clear_console(){
    system("clear");
}

void set_color(const std::string& color_code) {
    cout << "\033[" << color_code << "m";
}

void reset_color() {
    cout << "\033[0m";
}

void quit(){
    clear_console();
    cout << "Goodbye!" << endl;
    exit(0);
}

void invalid_command(){
    cout << "\nInvalid Command, returning to main menu " << endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    clear_console();
}


void clear_file(const std::string& filename) {
    std::ofstream outFile(filename, std::ios::trunc);
    if (!outFile.is_open()) {
        cout << "Unable to open file: " << filename << endl;
    }
    outFile.close();
}


void handle_input(){
    reset_color();
    cout << " \n[1] see main menu " <<endl << "command ---> ";
    string cmd;
    cin >> cmd;
    if(cmd == "1"){
        clear_console();
    }
    else{
        invalid_command();
    }
}


std::string to_hex(const std::string& input) {
    std::ostringstream oss;
    for (unsigned char c : input) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c);
    }
    return oss.str();
}


std::string from_hex(const std::string& input) {
    std::string output;
    for (size_t i = 0; i < input.length(); i += 2) {
        std::string byte = input.substr(i, 2);
        char chr = static_cast<char>(std::stoi(byte, nullptr, 16));
        output.push_back(chr);
    }
    return output;
}

std::string encrypt(std::string pwd, std::string key) {
    std::string result_pwd = pwd;
    size_t key_length = key.length();
    for (int i = 0; i < pwd.length(); i++) {
        result_pwd[i] = pwd[i] ^ key[i % key_length];
    }
    return to_hex(result_pwd);
}

//for encrypting/decrypting using xor;one function is enough for both operations ---> xor is a symmetric encryption.
//But in this case we need two functions due of the hexadecimal conversion.


std::string decrypt(std::string encrypted_pwd, std::string key) {
    std::string hex_decoded = from_hex(encrypted_pwd);
    std::string result_pwd = hex_decoded; //for size reasons
    size_t key_length = key.length();
    for (int i = 0; i < hex_decoded.length(); i++) {
        result_pwd[i] = hex_decoded[i] ^ key[i % key_length];
    }
    return result_pwd;
}

void check_file(string filename){
    ofstream outFile(filename, ios::app);
    if (!outFile.is_open()) {
        cout << "Unable to open file: " << filename << endl;
        return;
    }
}


void save_to_file(std::string filename, std::string entry, std::string username, std::string pwd) {
    ofstream outFile(filename, ios::app);
    check_file(filename);
    outFile << entry << endl << username << endl << pwd << endl;
    outFile.close();
}


vector<string> read_file(string filename) {
    vector<string> read_file_per_column;
    ifstream read_file(filename);
    string column;
    check_file(filename);
    while (getline(read_file, column)) {
        read_file_per_column.push_back(column);
    }
    return read_file_per_column;
}


tuple<vector<string>, vector<string>, vector<string>> sort_vector(vector<string> file_per_column_decrypted) {
    vector<string> entries;
    vector<string> usernames;
    vector<string> passwords;
    for (int y = 0; y < file_per_column_decrypted.size(); y++) {
        if (y % 3 == 0) {
            entries.push_back(file_per_column_decrypted[y]);
        }
        if (y % 3 == 1) {
            usernames.push_back(file_per_column_decrypted[y]);
        }
        if (y % 3 == 2) {
            passwords.push_back(file_per_column_decrypted[y]);
        }
    }
    return std::make_tuple(entries, usernames, passwords);
}


void show_read_file_as_entries(string key,string filename) {
    clear_console();
    vector<string> file_per_column_encrypted = read_file(filename);
    vector<string> file_per_column_decrypted;
    vector<string> entries, usernames, passwords;
    int entry_number;
    string cmd;
    for (string x : file_per_column_encrypted) {
        file_per_column_decrypted.push_back(decrypt(x, key));
    }
    tie(entries, usernames, passwords) = sort_vector(file_per_column_decrypted);
    for (int x = 0; x < entries.size(); x++) {
        cout << "Entries: " ;
        set_color("33");
        cout<< "["<<x<<"] ";
        reset_color();
        cout<<entries[x] << endl;
    }
    cout << ""<<endl;
    cout << ""<<endl;
    cout << "Please Enter the Entry Number to see the Details: ";
    cin >> entry_number;
    if (entry_number < entries.size()) {
        clear_console();
        animation(3);
        set_color("33");
        cout << "Username: " ;
        set_color("31");
        cout<< usernames[entry_number] << endl;
        set_color("33");
        cout << "Password: " ;
        set_color("31");
        cout<< passwords[entry_number] << endl;
        reset_color();
        cout << "\n[1] see main menu " <<endl << "[2] delete this entry\n" << endl;
        cout << "command ---> ";
        cin >> cmd;
        if(cmd == "1"){
            clear_console();
            //function closes automatically
        }
        if(cmd == "2"){
            cout << "Deleting Entry Number ["<<entry_number<<"]"<< "..."<< endl;
            entries.erase(entries.begin() + entry_number);
            usernames.erase(usernames.begin() + entry_number);
            passwords.erase(passwords.begin() + entry_number);
            clear_file(filename);
            for(int x = 0; x < entries.size(); x++){
                save_to_file(filename, encrypt(entries[x], key), encrypt(usernames[x], key), encrypt(passwords[x], key));
        }   
        clear_console();        
    } 
    if(cmd!="1" && cmd!="2"){ 
        invalid_command();
    }
}
}


string save_interface(string key,string filename) {
    string entry;
    string username;
    string pwd;
    clear_console();
    cout << "Please Enter the Entry Name: ";
    cin >> entry;
    cout << "Please enter the Username: ";
    cin >> username;
    cout << "Please Enter the Password: ";
    cin >> pwd;
    cout << "Data to save: Entry: " << entry << " Username: " << username << " Password: " << pwd << endl;
    save_to_file(filename, encrypt(entry, key), encrypt(username, key), encrypt(pwd, key));
    clear_console();
}


void show_file_info(string filename) {
    clear_console();
    cout << "Path :  ";
    set_color("31");
    cout << filename << endl;
    reset_color();
    cout << "Size :  ";
    set_color("31");
    cout << filename.size() << " Bytes" << endl;
    handle_input();
}


void show_raw_content_encrypted(string filename){
    clear_console();
    vector<string> file_per_column_encrypted = read_file(filename);
    set_color("33");
    cout << "Raw Encrypted Content (hex/base16) : " << endl;  
    set_color("31");
    for (string x : file_per_column_encrypted) {
        cout << x << endl;
    }
    handle_input();
}


int main() {
    std::string pwd;
    std::string key;
    std::string filename = "/Users/fynn/Desktop/Data/Projects/xor_c++/beispiel.txt";
    std::string cmd;
    bool run = true;
    clear_console();
    animation(2);
    cin >> key;
    clear_console();
    while (run) {
        animation(1);
        cin >> cmd;
        if (cmd == "2") {
            save_interface(key,filename);
        }
        if (cmd == "1") {
            show_read_file_as_entries(key,filename);
        }
        if (cmd == "5") {
            quit();
        }
        if (cmd == "3") {
            show_file_info(filename);
        }
        if(cmd=="4"){
            show_raw_content_encrypted(filename);
        }
        if(cmd!="1" && cmd!="2" && cmd!="3" && cmd!="4" && cmd!="5"){
            invalid_command();
        }
    }
    return 0;
}
