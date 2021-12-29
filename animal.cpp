/*
	animal.cpp

	author: L. Henke and C. Painter-Wakefield
	date: 04 November 2019

	Animal/20 questions program for CSCI 262, starter code.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <queue>
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;

class node {
public:
    string data;
    node* left;
    node* right;
};

void play_game(node*);
node* read_game_tree();
void write_game_tree(node*);
void delete_game_tree(node*);
node* read_preorder(ifstream& fin);

// TODO: Add helper method templates here, such as read_preorder().

/**
 * Handles showing the main menu/basic UI
 */
int main() {
    node* root = read_game_tree();
    if (root == NULL) return -1;

    while (true) {
        string tmp = "";
        int choice = 0;

        cout << "Welcome to 20 questions!" << endl;
        cout << "  1) Play the game" << endl;
        cout << "  2) Save the game file" << endl;
        cout << "  3) Quit" << endl;
        cout << "Please make your selection: ";
        getline(cin, tmp);
        choice = atoi(tmp.c_str());

        switch (choice) {
            case 1:
                play_game(root);
                break;
            case 2:
                write_game_tree(root);
                break;
            case 3:
                break;
            default:
                cout << "Sorry, I don't understand." << endl << endl;
        }
        if (choice == 3) break;
    }

    delete_game_tree(root);
    return 0;
}

node* read_preorder(ifstream& fin) {
    node* n = new node;
    // does this work?
    // I think the node should be dynamically allocated, but it is a pointer...
    // this may break something
    string s = "";

    getline(fin, s);

    string first = "";
    istringstream sin(s);
    sin >> first;

    string q = "";

    bool f = true;

    while(!sin.eof()) {
        string temp = "";
        sin >> temp;
        if (f) {q += temp; f = false;}
        else {q += (" " + temp);}
    }

    n->data = q;

    if (first == "#A") {

        n->left = nullptr;
        n->right = nullptr;

        return n;

    }

    n->left = read_preorder(fin);
    n->right = read_preorder(fin);
    return n; // <-- experiment w random shit more often my dude.
}

/**
 * Sets up the recursive call to the read_preorder
 * @return root of the tree
 */
node* read_game_tree() {
    // TODO: Implement me. See implementation hints! Reading is more complex than it may seem.
    ifstream fin("animal_game_tree.txt");

    node* root = read_preorder(fin);

    fin.close();

    return root;
}


/**
 * Plays the game
 * @param root Root of the game tree
 */
string to_lower(string s) {

    string result;

    for (char c: s) {

        c = tolower(c);

        string str(1, c);

        result += str;

    }

    return result;

}

void play_game(node* root) {

    // hold cur and prev nodes and string gamestate
    
    node* cur_node = root;

    node* prev_node = nullptr;

    vector<string> game_state = {};

    string ans = "";

    // go through binary tree, update cur and prev nodes

    while(cur_node->left!=nullptr) {

        cout << cur_node->data << " (y/n): " << endl;

        string ans = "";

        getline(cin, ans);

        ans = to_lower(ans);

        if (ans == "yes" || ans == "y") {

            game_state.push_back(cur_node->data + " YES");

            prev_node = cur_node;

            cur_node = cur_node->left;

        } else if (ans == "no" || ans == "n") {

            game_state.push_back(cur_node->data + " NO");

            prev_node = cur_node;

            cur_node = cur_node->right;

        }

    }

    cout << cur_node->data << " (y/n): ";

    getline(cin, ans);

    ans = to_lower(ans);

    if (ans == "yes" || ans == "y") { // this is a final answer question

        cout << "Yay! I won ig" << endl;

    } else if (ans == "no" || ans == "n") {

        string expand = "";

        game_state.push_back(cur_node->data + " NO");

        cout << "Dang. Got it wrong ig." << endl;

        cout << "Would you like to expand the game tree (y/n)? ";

        getline(cin, expand);

        expand = to_lower(expand);

        if (expand == "yes" || expand == "y") {

            string new_animal = "";

            string new_q = "";

            cout << "I asked the following:" << endl;

            for (string s: game_state) {

                cout << s << endl;

            }

            cout << "Enter a new animal in the form of a question," << endl;
            cout << "e.g., 'Is it a whale?':";

            getline(cin, new_animal);

            cout << "Now enter a question for which the answer is 'yes' for your new animal, and which does not contradict your previous answers:";

            getline(cin, new_q);

            string temp = prev_node->data;

            prev_node->data = new_q;

            node* nl = new node;

            node* nr = new node;

            nl->data = new_animal;

            nr->data = temp;

            nl->left = nullptr; nl->right = nullptr;
            nr->left = nullptr; nr->right = nullptr;

        }

    }

}

/**
 * Writes the game tree, sets up a recursive call to write_preorder();
 * @param root The root of the tree
 */
void write_preorder(ofstream& out, node* root) {

    string data = root->data;

    out << data << endl;

    if (root->left == nullptr) {

        return;

    }

    write_preorder(out, root->left);

    write_preorder(out, root->right);

}

void write_game_tree(node* root) {

    ofstream out("animal_game_tree.txt");

    write_preorder(out, root);

    out.close();

}

/**
 * Deletes the game tree
 * @param root Root of the game tree
 */
void delete_game_tree(node* root) {
    // Optional. Do a post-order deletion of the game tree.
    // This isn't strictly needed as the program exits after this is called,
    // which frees up all the memory anyway.
}