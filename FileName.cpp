#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <time.h<
#include <sys\locking.h>
#include <string.h>
#include <ctype.h>
using namespace std;
//������������ ����� �����
#define MAX_WORD_LENGTH 21
//���-�� �������
int Tries = 10;
//���-�� ��������� ����
int CountWords = 0;

// �������� 31

//�������� �����
bool LoadWord(FILE* file, char* word)
{
	int i = 0;
	char s[MAX_WORD_LENGTH] = { 0 };
	//���-�� ���� � �����
	static int count = -1;
	if (count == -1)
	{
		//������� ���������� ����
		while (!feof(file))
		{
			fgets(s, MAX_WORD_LENGTH, file);
			count++;
		}
		//���� ���?
		if (count == 0)
			return false;
		//������� ��������� ��������� � ������ �����
		fseek(file, 0, 0);
	}
	//��������� �����
	int n = rand() % count;
	//����� �����
	while (i <= n)
	{
		fgets(s, MAX_WORD_LENGTH, file);
		i++;
	}
	//���������� ����� �����
	int wordlen = strlen(s);
	//����������� ����� ����� 2 �����
	if (wordlen <= 1)
		return false;

	// �������� 32

	//������� Enter (� DOS'� 2 ����� 13 10)
	if (s[wordlen - 1] == 10)
		s[wordlen - 2] = 0;
	else if (s[wordlen - 1] == 13)
		s[wordlen - 1] = 0;
	//�������� �����
	strcpy(word, s);
	//�������� ���������� �����
	int hFile = _fileno(file);
	//��������� ������ �����
	int size = _filelength(hFile);
	//���������� �����
	fseek(file, 0, 0);
	_locking(hFile, _LK_NBLCK, size);
	return true;
}
//����
void Game(char* word)
{
	//������� � ������� �����
	strupr(word);
	int len = strlen(word);
	//������-�����
	char* copy = new char[len + 1];
	memset(copy, '*', len);
	copy[len] = 0;
	//������� + �������
	char letters[52];
	int i, j = 0;
	for (i = 0; i < 26; i++)
	{
		letters[j++] = i + 'A';

		// �������� 33

		letters[j++] = ' ';
	}
	//���������� ����
	letters[51] = 0;
	//�����
	char letter;
	char* pos;
	bool replace = false;
	do {
		//������� ������
		system("cls");
		cout << copy << endl << endl;
		cout << letters << endl << endl;
		cout << "Count of tries: " << Tries << endl
			<< endl;
		cout << "Input any letter:\t";
		cin >> letter;
		//�������� ������
		Beep(500, 200);
		//if(letter >= 'A' && letter <= 'Z'
		//|| letter >= 'a' && letter <= 'z')
		//�����?
		if (!isalpha(letter))
		{
			cout << "It's not a letter" << endl;
			//�������� �� 1 �������
			Sleep(1000);
			continue;
		}
		//������� ����� � �������
		letter = toupper(letter);

		// �������� 34

		//����� ����� � ��������
		pos = strchr(letters, letter);
		//����� ����� ��� ����
		if (pos == 0)
		{
			cout << "This letter have been already
				pressed" << endl;
				Sleep(1000);
			continue;
		}
		else
		{
			//������� ����� �� ��������
			pos[0] = ' ';
		}
		//����� ����� � �����
		for (i = 0; i < len; i++)
		{
			if (word[i] == letter)
			{
				copy[i] = letter;
				replace = true;
			}
		}
		if (replace == false)
			Tries--;
		else
			replace = false;
		//������� ������
		if (strcmp(word, copy) == 0)
		{
			system("cls");
			cout << copy << endl << endl;
			cout << letters << endl << endl;

			// �������� 35

			cout << "Count of tries: " << Tries <<
				endl << endl;
			cout << "Congratulation !!!" << endl;
			CountWords++;
			break;
		}
	} while (Tries != 0);
	delete[] copy;
}
void main()
{
	//��������� ���� �� ������ � �������� ������
	FILE* f = fopen("words.txt", "rb");
	//���� ���� �� ��������
	if (f == 0)
	{
		//������
		perror("Open");
		return;
	}
	srand(time(0));
	char Word[20];
	//�������� ��������� �����
	if (!LoadWord(f, Word))
	{
		//���� ��������
		cout << "Error !!!" << endl;
		fclose(f);
		return;
	}
	char answer;
	//������, ���� �� �������
	do
	{

		// �������� 36

		Game(Word);
		//���� ������� �� ��������, �� �����
		if (Tries == 0)
		{
			cout << "Count of words: " << CountWords << endl;
			cout << "Bye-bye" << endl;
			break;
		}
		//���� ��������
		cout << "Continue ??? (Y/N)\t";
		cin >> answer;
		//��� ������?
		if (answer == 'Y' || answer == 'y')
			if (!LoadWord(f, Word))
			{
				cout << "Error !!!" << endl;
				fclose(f);
				return;
			}
	} while (answer == 'Y' || answer == 'y');
	//�������� ����������
	int hFile = _fileno(f);
	//������������� �����
	int size = _filelength(hFile);
	fseek(f, 0, 0);
	_locking(hFile, _LK_UNLCK, size);
	fclose(f);
}
