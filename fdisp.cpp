using namespace std;
//comment for change for week 09 github
#include <iostream>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
	string input;
	while(getline(cin, input))
	{
		if(!strcmp(*(argv+1), "-d"))
		{
			if(input.find("directories") != -1)
			{
				cout << input << " \n";
			}
		}
		else if(!strcmp(*(argv+1), "-f"))
		{
			if(input.find("files") != -1)
			{
				cout << input << "\n";
			}
		}
		else if(!strcmp(*(argv+1), "-b"))
		{
			if(input.find("bytes") != -1)
			{
				cout << input << "\n";
			}
		}
		else
		{
			cout << "Invalid option \n";
		}
	}
}
