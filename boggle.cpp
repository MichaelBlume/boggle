#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class wordnode {
	public:
		wordnode (istream &, string &, int, bool &, int &);	//constucts a wordnode object
		~wordnode ();	//deletes a wordnode object
		bool findwords (int, int, int &, char[10][10], int &);
		bool wordful;	//is myword an english word?
		wordnode *nexts[27];	// Pointers to more wordnodes!  Each corresponding to a different letter
		string myword;	//the string of letters which bring us to this node
		bool reachable;
		void print();
};

wordnode::wordnode (istream &in, string &word, int level, bool &ndone, int &nlang)
{
	for (int n=0;n<27;n++)
		nexts[n]=NULL;
	string newword;	//a newword variable is only necessary because of a peculiarity in the language, which you'll see later
	if (level == word.size())	//if we've reached the point in the tree correspnonding to the end of the word
	{
		if (level)	//if the word's at least one letter
		{
			myword = word;
			wordful = 1;
//			if (nlang%10000 == 0)
//				cout << myword << endl;
		}
		in >> word;	//now we have to get another word to see if it should be attached to this node.  If it shouldn't be, it needs to be sent back to the calling fuction, which is why word is passed as a reference.
		ndone = !(in.eof());
		nlang++;
	}
	else
	{
		myword = word.substr(0,level);
		newword = word;
		nexts[word[level]-96] = new wordnode (in, newword, level+1, ndone, nlang);	//newword is necessary because of this command.  The compiler executes the right-hand side of it first, changing the value of newword, which we want.  However, if we just passed it word, it would use the new value of word on the *left*, changing which next gets modified.  This was completely screwing things up until I noticed it
		word = newword;
	}
	newword = word;
	while ((word.substr(0,level) == myword)&&(ndone))	//while we're still processing words that start from this node
	{
		nexts[word[level]-96] = new wordnode (in, newword, level+1, ndone, nlang);
		word = newword;
	}
	reachable=0;
}

bool wordnode::findwords (int j, int k, int &nwords, char boggle [10][10], int &width)
{
	wordnode *jim;
//	cout << "successfully entered findwords" << endl;
	if (wordful) //if we've already hit a word
	{
		reachable=1;
//		cout << myword << " is a word!" << endl;
	}
	char newboggle [10][10];	//like I said, we have to manually copy the grid
	for (int a=0;a<width;a++)
		for (int b=0;b<width;b++)
			newboggle[a][b]=boggle[a][b];
	newboggle[j][k]=0;	//and remove the letter we just used
	for (int l = -1; l<2; l++)	//step through adjoining squares
		for (int m=-1;m<2;m++)
			if (((j+l)>-1)&&((j+l)<width)&&((k+m)>-1)&&((k+m)<width)&&(newboggle[j+l][k+m]!=0))
				if (jim=nexts[boggle[j+l][k+m]-96])
					if (jim->findwords (j+l, k+m, nwords, newboggle, width))	//yay recursion! =)
					reachable=1;
	return reachable;
}

void wordnode::print ()
{
	if (reachable)
	{
		if (wordful)
			cout << myword << endl;
		for (int n=0;n<27;n++)
			if (nexts[n])
				nexts[n]->print();
	}
}

wordnode::~wordnode ()
{
//	cout << "successfully entered destructor!" << endl;
	for (int n=0; n<27; n++)
		if (nexts[n]!=NULL)
			delete nexts[n];	//free memory
} 

int main ()
{
	cout << "Welcome to Boggle!" << endl << "by Mike Blume" << endl << endl << "Please wait, while I initialize the program by reading words into memory.  When you've finished playing Boggle, you can see the words I'm using by typing" << endl << "vim /web2new" << endl << "at the command prompt." << endl;
	ifstream in ("/web2new");	//open dictionary file for reading
	string empty = "\0";	//I can't just pass the constructor an empty string, because then it couldn't change it.  I have to make a variable.
	wordnode *bob;
	wordnode *jim;
	char a;
	bool joe = true;	//same issue
	int nlang = 0;
	bob = new wordnode (in, empty, 0, joe, nlang);
	int width = 0;
	cout << "Initialization complete.  First off, go ahead and tell me how wide your Boggle grid is.  Real boggle grids are either four or five letters wide, but this program will happily handle anythting up to 10." << endl;
	cin >> width;
	cout << "Please enter " << width*width << " letters.  Please only enter lowercase letters from a-z, and hit enter when you're done.  If you'd like to hit enter in between rows (ie, to make a grid), knock yourself out, but this isn't necessary." << endl;
	char boggle[10][10];
	for (int j = 0;j<width;j++)
		for (int k = 0; k<width;k++)
		{
			a=0;
			while ((a<'a')||(a>'z'))
				cin >> a;
			boggle[j][k] = a;	//get the boggle grid
		}
	int nwords=0;
//	char words[20000][100];
	cout << "successfully received grid!" << endl;
	for (int j = 0;j<width;j++)
		for (int k = 0; k<width;k++)
			if (jim=bob->nexts[boggle[j][k]-96])
				jim->findwords (j, k, nwords, boggle, width);	//search for words
	bob->reachable=1;
	cout << "Search Complete.  Here are your words." << endl << endl << endl;
	bob->print();
	cout << endl << endl << "Thank you for playing Boggle.  Have a nice day =)" << endl << endl << "If you'd like to see the source for this program type" << endl << "vim /boggle.cpp" << endl << "at the command line." << endl;
	delete bob;
	return 0;
}

/*int testmain ()
{
	ifstream in ("/web2new");
	string empty = "\0";
	wordnode *bob;	//50
	bool joe = true;
	int nlang = 0;
	bob = new wordnode (in, empty, 0, joe, nlang);
	wordnode *root = bob;
	cout << "Yes!  Survived initialization" << endl;
	empty = "\0";
	cout << "length of empty string is" << empty.size() << endl;
	char test = 'a';
	while (test != 'z')
	{
		while (bob)
		{
			for (test = 'a'; test <= 'z'; test++)
				if (bob->nexts[test-96])
					cout << test << endl;
			cin >> test;
			bob = bob->nexts[test-96];
		}
		bob = root;
	}
	delete bob;
	return 0;
}*/

//back when I used it, this function was the main function and allowed me to explore the dictionary tree

