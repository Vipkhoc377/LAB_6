#include<iostream>
#include<fstream>
#include<string>
#include<ctime>

using namespace std;

struct words {
	string spell;
	string meaning;
};

struct node { // Khoi tao node
	words word;
	node* left;
	node* right;
};

typedef node* BST;
// AVL TREE
int treeLevel(node* tree) {
	if (tree == NULL) {
		return -1;
	}
	return 1 + max(treeLevel(tree->left), treeLevel(tree->right));
}

bool checkAvl(node* tree) {
	if (tree == NULL) {
		return true;
	}
	if (abs(treeLevel(tree->left) - treeLevel(tree->right)) > 1) {
		return false;
	}
	return checkAvl(tree->left) && checkAvl(tree->right);
}

node* turnRight(node* a) {
	node* b = a->left;
	node* d = b->right;
	a->left = d;
	b->right = a;
	return b;
}

node* turnLeft(node* a) {
	node* b = a->right;
	node* c = b->left;
	a->right = c;
	b->left = a;
	return b;
}

node* updateTreeAvl(node* tree) {
	if (abs(treeLevel(tree->left) - treeLevel(tree->right)) > 1) {
		if (treeLevel(tree->left) > treeLevel(tree->right)) {
			node* p = tree->left;
			if (treeLevel(p->left) >= treeLevel(p->right)) {
				tree = turnRight(tree);
			}
			else {
				tree->left = turnLeft(tree->left);
				tree = turnRight(tree);
			}
		}
		else {
			node* p = tree->right;
			if (treeLevel(p->right) >= treeLevel(p->left)) {
				tree = turnLeft(tree);
			}
			else {
				tree->right = turnRight(tree->right);
				tree = turnLeft(tree);

			}
		}
	}
	if (tree->left != NULL) tree->left = updateTreeAvl(tree->left);
	if (tree->right != NULL) tree->right = updateTreeAvl(tree->right);
	return tree;
}
// END AVL TREE

//CHECK IF EMPTY
bool isLeafNode(node* l) {
	return (l->left == NULL && l->right == NULL);
}
int countLeafNode(node* tree) {
	if (tree == NULL) return 0;
	if (isLeafNode(tree)) return 1;
	return countLeafNode(tree->left) + countLeafNode(tree->right);
}
// END CHECKING

void menu() {
	cout << "1. Search." << endl;
	cout << "2. Add a word." << endl;
	cout << "3. Edit word's meaning." << endl;
	cout << "4. Delete a word." << endl;
	cout << "5. Save." << endl;
	cout << "6. Exit." << endl;
	cout << "Enter a number (1-6): ";
}

node* createNode(words word) // init = x
{
	node* p = new node;
	p->word = word;
	p->left = NULL;
	p->right = NULL;
	return p;
}

void insert(BST& tree, node* N) {
	if (tree == NULL) {
		tree = N;
	}
	else {
		if (N->word.spell == tree->word.spell) {
			return;
		}
		if (N->word.spell < tree->word.spell) {
			insert(tree->left, N);
		}
		else {
			insert(tree->right, N);
		}
	}
}

void addNewWord(BST& tree, node* N) {
	if (tree == NULL) {
		tree = N;
	}
	else {
		if (N->word.spell < tree->word.spell) {
			addNewWord(tree->left, N);
		}
		else {
			addNewWord(tree->right, N);
		}
	}
}

node* editMeaning(BST& tree, string spell, string meaning) {
	node* p = tree;
	if (p->word.spell == spell) {
		p->word.spell = spell;
		p->word.meaning = meaning;
		return p;
	}
	else {
		if (spell < p->word.spell) {
			return editMeaning(p->left, spell, meaning);
		}
		else {
			return editMeaning(p->right, spell, meaning);
		}
	}
}

node* search(BST& tree, string s) {
	node* p = tree;
	if (!tree) {
		return NULL;
	}
	else {
		if (s == p->word.spell) {
			return p;
		}
		if (s < p->word.spell) {
			return search(p->left, s);
		}
		else {
			return search(p->right, s);
		}
	}
}

node* minNode(node* N)
{
	node* p = N;

	while (p && p->left != NULL) {
		p = p->left;
	}
	return p;
}

node* deleteWord(BST& tree, string s) {
	if (tree == NULL) {
		return tree;
	}

	if (s < tree->word.spell) {
		tree->left = deleteWord(tree->left, s);
	} else if (s > tree->word.spell) {
		tree->right = deleteWord(tree->right, s);
	}
	else {
		if (tree->left == NULL && tree->right == NULL) {
			delete tree;
			return NULL;
		}
		else if (tree->left == NULL) {
			node* p = tree->right;
			delete tree;
			return p;
		}
		else if (tree->right == NULL) {
			node* p = tree->left;
			delete tree;
			return p;
		}
		node* p = minNode(tree->right);
		tree->word = p->word;
		tree->right = deleteWord(p, tree->word.spell);
	}

	return tree;
}

void printTree(BST tree, ofstream &ofs) {
	if (tree != NULL) {
		printTree(tree->left, ofs);
		ofs << tree->word.spell << " " << tree->word.meaning << endl;
		printTree(tree->right, ofs);
	}
	else {
		return;
	}
}

int main() {
	BST tree = NULL;

	int total = 0;
	int choice;
	string sTemp;
	srand(time(NULL));

	double time;
	clock_t start, end;

	//LOAD
	start = clock();
	ifstream ifs;
	ifs.open("dictionary.txt");
	while (!ifs.eof()) {
		getline(ifs, sTemp);
		if (sTemp.size() > 3) {
			total++;
		}
	}
	ifs.clear();
	ifs.seekg(0L, ios::beg);

	words* word = new words[total];

	total = 0;
	while (!ifs.eof()) {
		getline(ifs, word[total].spell, ' ');
		if (word[total].spell.size() < 2) {
			total--;
		}
		else {
			getline(ifs, word[total].meaning, '\n');
			total++;
		}
	}
	for (int i = 0; i < total; i++) { // Erase '\n' in every word
		if (word[i].spell[0] == '\n')
		{
			word[i].spell.erase(0, 1);
		}
	}

	/*for (int i = 0; i < total; i++) {
		cout << word[i].spell << word[i].meaning << endl;
	}*/

	for (int i = 0; i < total; ++i) //Swap to avoid stackoverflow
	{
		int temp = rand() % total + 0;
		swap(word[i], word[temp]);
	}

	for (int i = 0; i < total; i++) { // Insert
		insert(tree, createNode(word[i]));
	}

	while (!checkAvl(tree)) {
		tree = updateTreeAvl(tree);
	}
	ifs.close();
	delete[] word;

	if (countLeafNode != 0) {
		cout << "Tree is not empty" << endl;
	}
	else {
		cout << "TREE EMPTY!";
		return 0;
	}

	end = clock();
	time = end - start;
	cout << "Time: " << time << "ms" << endl;

	menu();
	cin >> choice;
	cin.ignore();
	switch (choice) {
		case 1: { // SEARCH
			string wordFinder;
			cout << "Enter a word you wanna search: ";
			getline(cin, wordFinder);
			start = clock();
			if (wordFinder[0] >= 'a' && wordFinder[0] <= 'z') {
				wordFinder[0] -= 32;
			}
			node* result = NULL;
			result = search(tree, wordFinder);
			if (result == NULL) {
				cout << "Invalid word!";
			}
			else {
				cout << result->word.spell + ": " + result->word.meaning;
			}
	
			end = clock();
			time = end - start;
			cout << "Time: " << time << "ms" << endl;
			break;
		}
		case 2: {// ADD A WORD
			words word;
			cout << "Enter a word you want to add: ";
			getline(cin, word.spell);
			if (word.spell[0] >= 'a' && word.spell[0] <= 'z') {
				word.spell[0] -= 32;
			}
			cout << "Enter its meaning: ";
			getline(cin, word.meaning);
			start = clock();
			addNewWord(tree, createNode(word));
			cout << "Added!" << endl;

			end = clock();
			time = end - start;
			cout << "Time: " << time << "ms" << endl;
			while (!checkAvl(tree)) {
				tree = updateTreeAvl(tree);
			}
			break;
		}
		case 3: { // EDIT WORD'S MEANING
			string sameWord;
			string newMeaning;
			cout << "Enter a valid word you wanna edit: ";
			getline(cin, sameWord);
			start = clock();
			if (sameWord[0] >= 'a' && sameWord[0] <= 'z') {
				sameWord[0] -= 32;
			}
			node* result;
			result = search(tree, sameWord);
			if (result == NULL) {
			cout << "Invalid word.\n";
			end = clock();
			time = end - start;
			cout << "Time: " << time << "ms" << endl;
			}
			else
			{
				end = clock();
				time = end - start;
				cout << result->word.spell << ": " << result->word.meaning << endl;
				cout << "New meaning: ";
				getline(cin, newMeaning);
				start = clock();
				result = editMeaning(tree, sameWord, newMeaning);
				cout << result->word.spell << ": " << result->word.meaning << endl;
				end = clock();
				time = time + (end - start);
				cout << "Time: " << time << "ms" << endl;
			}
			// END EDITING
			break;
		}
		case 4: { // DELETE A WORD
			string delWord;
			cout << "Enter a word you want to delete: ";
			getline(cin, delWord);

			start = clock();
			if (delWord[0] >= 'a' && delWord[0] <= 'z') {
				delWord[0] -= 32;
			}
			node* result;
			result = deleteWord(tree, delWord);
			if (result == NULL) {
				cout << "Invalid word" << endl;

				end = clock();
				time = end - start;
				cout << "Time: " << time << "ms" << endl;
			} else {
				cout << "DELETE!" << endl;

				end = clock();
				time = end - start;
				cout << "Time: " << time << "ms" << endl;
			}
			break;
		}
		
		case 5: { // SAVE
			start = clock();
			ofstream ofs;
			ofs.open("save.txt");
			printTree(tree, ofs);
			end = clock();
			time = end - start;
			cout << "Time: " << time << "ms" << endl;
			ofs.close();
			break;
		}
		
		case 6: {
			return 0;
			break;
		}

		default: {
			cout << "Invalid option";
			break;
		}
	}

	return 0;
}
// hello ban kiet
// hello ban huy
