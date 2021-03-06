// Hanamikoji.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream> //cout, cin
#include <algorithm> //random_shuffle
#include <string> //to_string
#include <ctime> //time
#include <stdlib.h> //cls
#include <iomanip> //left, right in cout
#include <windows.h> //colors

using namespace std;

//Változók//
int kaki;
int playerhand [3] [11]; // játékos keze [játékos, lap]
int playerhandcounter[3]; //játékosnál hány lap van
int deck[21] = { 1, 1, 2, 2, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7, 7 }; //pakli
int deckcounter; // pakli számláló
bool secretavailable[3] = { true,true,true };
bool tradeoffavailable[3] = { true,true,true };
bool giftavailable[3] = { true,true,true };
bool competitionavailable[3] = { true,true,true };
int geishagifts[3][8];
int geishalove[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
int secretcard[3] = { 0, 0, 0 };
int score[3] = { 0, 0, 0 };
int value[8] = { 0, 2, 2, 2, 3, 3, 4, 5 };
int geishanumber[3] = { 0, 0, 0 };
bool gameover=false;
int winner = 0;
HANDLE  hConsole;

//Függvények//
void card(int code); //kártya neve a számából
void secret(int player); // titok akció
void tradeoff(int player); // kompromisszum akció
void gift(int player); // ajándék akció
void competition(int player); // versengés akció
void deal(); // keverés és osztás
void draw(int player); // húzás
void turn(int player); //játékos köre
int other(int player); //másik játékos
void table(); //kiírja az állást
void endgame();
void displayhand(int player);

int main() {
	setlocale(LC_ALL, "Hun");
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	srand(time(NULL)); //randomizálás
	cout << "Hello és üdvözlet a Hanamikoji játékban! \n\n";
	while (true) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 8; j++) geishagifts[i][j] = 0;
		}
		for (int i = 0; i < 3; i++) {
			secretavailable[i] = true;
			tradeoffavailable[i] = true;
			giftavailable[i] = true;
			competitionavailable[i] = true;
		}
		deal(); // keverés és osztás
		for (int i = 0; i < 4; i++) {
			turn(1);
			turn(2);
		}
		endgame();
	}
}
void draw(int player) {
	sort(rbegin(playerhand[player]), rend(playerhand[player]));
	playerhand[player][playerhandcounter[player]] = deck[deckcounter];
	playerhandcounter[player]++;
	deckcounter++;
	sort(rbegin(playerhand[player]), rend(playerhand[player]));
}
void deal() {
	random_shuffle(begin(deck), end(deck));
	deckcounter = 0;
	playerhandcounter[1] = 0;
	playerhandcounter[2] = 0;
	for (int i = 0; i < 6; i++) {
		playerhand[1][i] = deck[deckcounter];
		playerhandcounter[1]++;
		deckcounter = deckcounter + 1;
		playerhand[2][i] = deck[deckcounter];
		playerhandcounter[2]++;
		deckcounter = deckcounter + 1;
	}
	sort(rbegin(playerhand[1]), rend(playerhand[1]));
	sort(rbegin(playerhand[2]), rend(playerhand[2]));

}
void turn(int player) {
	system("cls");
	draw(player);
	table();
	cout << to_string(player) << ". játékos jön, a lapjai: \n\n";
	displayhand(player);
	cout << "\nMit szeretnél lépni? Írd be a megfelelő számot!\n";
	if (secretavailable[player]) cout << "1. Titok\n";
	if (tradeoffavailable[player]) cout << "2. Kompromisszum\n";
	if (giftavailable[player]) cout << "3. Ajándék\n";
	if (competitionavailable[player]) cout << "4. Versenyzés\n";
	bool actiondone = false;
	int action = 0;
	do {
		cin >> action;
		switch (action) {
		case 1: if (secretavailable[player]) {actiondone = true; secret(player);} break;
		case 2: if (tradeoffavailable[player]) { actiondone = true; tradeoff(player); } break;
		case 3: if (giftavailable[player]) { actiondone = true; gift(player); } break;
		case 4: if (competitionavailable[player]) { actiondone = true; competition(player); } break;
		}
	} while (!actiondone);

}
void card(int code) {
	switch (code) {
	case 1: SetConsoleTextAttribute(hConsole, 5*16); cout << "furulya 2"; SetConsoleTextAttribute(hConsole, 15); break;
	case 2: SetConsoleTextAttribute(hConsole, 14*16); cout << "legyező 2"; SetConsoleTextAttribute(hConsole, 15); break;
	case 3: SetConsoleTextAttribute(hConsole, 11*16); cout << "tekercs 2"; SetConsoleTextAttribute(hConsole, 15); break;
	case 4: SetConsoleTextAttribute(hConsole, 12*16); cout << "ernyő 3"; SetConsoleTextAttribute(hConsole, 15); break;
	case 5: SetConsoleTextAttribute(hConsole, 6*16); cout << "dibár 3"; SetConsoleTextAttribute(hConsole, 15); break;
	case 6: SetConsoleTextAttribute(hConsole, 2*16); cout << "tea 4"; SetConsoleTextAttribute(hConsole, 15); break;
	case 7: SetConsoleTextAttribute(hConsole, 13*16); cout << "virág 5"; SetConsoleTextAttribute(hConsole, 15); break;
	default: cout << "baj van";
	}
}
void secret(int player) {
	int input=0;
	secretavailable[player] = false;
	cout << "Melyik kártyát szeretnéd félretenni?\n";
	do {
		cin >> input;
		if (input > playerhandcounter[player]) cout << "Nincs ennyi lap a kezedben!\n";
	} while (input > playerhandcounter[player]);
	secretcard[player] = playerhand[player][input - 1];
	playerhand[player][input - 1] = 0;
	playerhandcounter[player]--;
}
void tradeoff(int player) {
	int input1 = 0;
	int input2 = 0;
	tradeoffavailable[player] = false;
	cout << "Melyik kártyákat szeretnéd eldobni?\n";
	do {
		cin >> input1;
		cin >> input2;
		if (input1 > playerhandcounter[player] || input2 > playerhandcounter[player] || input1==input2 || input1 < 1 || input2 < 1)
			cout << "Hibás input, ismételd meg mindkét kártya számát!\n";
	} while (input1 > playerhandcounter[player] || input2 > playerhandcounter[player] || input1==input2 || input1 < 1 || input2 < 1);
	playerhand[player][input1 - 1] = 0;
	playerhand[player][input2 - 1] = 0;
	playerhandcounter[player] = playerhandcounter[player] - 2;
}
void gift(int player) {
	giftavailable[player] = false;
	int input1 = 0;
	int input2 = 0;
	int input3 = 0;
	int takencard = 0;
	cout << "Melyik kártyákat szeretnéd felajánlani?\n";
	do {
		cin >> input1;
		cin >> input2;
		cin >> input3;
		if (input1 > playerhandcounter[player] || input2 > playerhandcounter[player] || input3 > playerhandcounter[player] ||
			input1==input2 || input2==input3 || input1 == input3 || input1 < 1 || input2 < 1 || input3 < 1)
			cout << "Hibás input, ismételd meg mindhárom kártya számát!\n";
	} while (input1 > playerhandcounter[player] || input2 > playerhandcounter[player] || input3 > playerhandcounter[player] ||
		input1 == input2 || input2 == input3 || input1 == input3 || input1 < 1 || input2 < 1 || input3 < 1);
	system("cls");
	table();
	cout << to_string(player) << ". játékos ajándékot játszott!\n" << to_string(other(player)) << ". játékos lapjai:\n\n";
	displayhand(other(player));
	cout << "\n\nA felajánlott lapok:\n";
	cout << "1. ";
	card(playerhand[player][input1 - 1]);
	cout << endl;
	cout << "2. ";
	card(playerhand[player][input2 - 1]);
	cout << endl;
	cout << "3. ";
	card(playerhand[player][input3 - 1]);
	cout << endl;
	cout << "Melyik kártyát választod?\n";
	do {
		cin >> takencard;
		if (takencard > 3 || takencard < 1) cout << "Hibás input!";
	} while (takencard > 3 || takencard < 1);
	if (takencard == 1) {
		geishagifts[other(player)][playerhand[player][input1 - 1]]++; //másiké a kiválasztott első
	}
	else {
		geishagifts[player][playerhand[player][input1 - 1]]++; // különben játékosé az első
	}
	if (takencard == 2) {
		geishagifts[other(player)][playerhand[player][input2 - 1]]++; 
	}
	else {
		geishagifts[player][playerhand[player][input2 - 1]]++;
	}
	if (takencard == 3) {
		geishagifts[other(player)][playerhand[player][input3 - 1]]++; 
	}
	else {
		geishagifts[player][playerhand[player][input3 - 1]]++;
	}
	playerhand[player][input1 - 1] = 0;
	playerhand[player][input2 - 1] = 0;
	playerhand[player][input3 - 1] = 0;
	playerhandcounter[player] = playerhandcounter[player] - 3;
}
void competition(int player) {
	competitionavailable[player] = false;
	int input1 = 0;
	int input2 = 0;
	int input3 = 0;
	int input4 = 0;
	int takenpair = 0;
	cout << "Melyik kártyák legyenek az első párban?\n";
	do {
		cin >> input1;
		cin >> input2;
		if (input1 > playerhandcounter[player] || input2 > playerhandcounter[player] || input1 == input2 || input1 < 1 || input2 < 1)
			cout << "Hibás input! Melyik kártyák legyenek az első párban?\n";
	} while (input1 > playerhandcounter[player] || input2 > playerhandcounter[player] || input1 == input2 || input1 < 1 || input2 < 1);
	cout << "Melyik kártyák legyenek a második párban?\n";
	do {
		cin >> input3;
		cin >> input4;
		if (input3 > playerhandcounter[player] || input4 > playerhandcounter[player] || input3 == input4 || input3 < 1 || input4 < 1 ||
			input3==input1 || input3 == input2 || input4 == input1 || input4 == input2)
			cout << "Hibás input! Melyik kártyák legyenek a második párban?\n";
	} while (input3 > playerhandcounter[player] || input4 > playerhandcounter[player] || input3 == input4 || input3 < 1 || input4 < 1 ||
		input3 == input1 || input3 == input2 || input4 == input1 || input4 == input2);
	system("cls");
	table();
	cout << "Az ellenfél versenyt játszott!\n";
	cout << "1. ";
	card(playerhand[player][input1 - 1]);
	cout << " & ";
	card(playerhand[player][input2 - 1]);
	cout << endl;
	cout << "2. ";
	card(playerhand[player][input3 - 1]);
	cout << " & ";
	card(playerhand[player][input4 - 1]);
	cout << endl;
	cout << "Melyik párt választod?\n";
	cin >> takenpair;
	if (takenpair == 1) {
		geishagifts[other(player)][playerhand[player][input1 - 1]]++; //másik játékos megkapja az első pár egyikét
		geishagifts[other(player)][playerhand[player][input2 - 1]]++; //és a másikat
		geishagifts[player][playerhand[player][input3 - 1]]++;
		geishagifts[player][playerhand[player][input4 - 1]]++;
	}
	if (takenpair == 2) {
		geishagifts[player][playerhand[player][input1 - 1]]++; //játékos megkapja az első pár egyikét
		geishagifts[player][playerhand[player][input2 - 1]]++; //és a másikat
		geishagifts[other(player)][playerhand[player][input3 - 1]]++;
		geishagifts[other(player)][playerhand[player][input4 - 1]]++;
	}
	playerhand[player][input1 - 1] = 0;
	playerhand[player][input2 - 1] = 0;
	playerhand[player][input3 - 1] = 0;
	playerhand[player][input4 - 1] = 0;
	playerhandcounter[player] = playerhandcounter[player] - 4;
}
int other (int player) {
	int out = 0;
	if (player == 1) out = 2;
	if (player == 2) out = 1;
	return out;
}
void table () {
	size_t W1 = 13;
	size_t W = 5;
	cout << setw(W1) << left << "Gésa" << setw(W) << "1." << setw(W) << "2." << setw(W) << "1." << setw(W) << "-" << setw(W) << "2." << endl;

	SetConsoleTextAttribute(hConsole, 5 * 16);
	if (geishalove[1] == 0) cout << setw(W1) << left << "furulya 2" << setw(W) << to_string(geishagifts[1][1]) << setw(W) <<
		to_string(geishagifts[2][1]) << setw(W) << " " << setw(W) << "X" << setw(W) << " " << endl;
	if (geishalove[1] == 1) cout << setw(W1) << left << "furulya 2" << setw(W) << to_string(geishagifts[1][1]) << setw(W) <<
		to_string(geishagifts[2][1]) << setw(W) << "X" << setw(W) << " " << setw(W) << " " << endl;
	if (geishalove[1] == 2) cout << setw(W1) << left << "furulya 2" << setw(W) << to_string(geishagifts[1][1]) << setw(W) <<
		to_string(geishagifts[2][1]) << setw(W) << " " << setw(W) << " " << setw(W) << "X" << endl;

	SetConsoleTextAttribute(hConsole, 14 * 16);
	if (geishalove[2] == 0) cout << setw(W1) << left << "legyező 2" << setw(W) << to_string(geishagifts[1][2]) << setw(W) <<
		to_string(geishagifts[2][2]) << setw(W) << " " << setw(W) << "X" << setw(W) << " " << endl;
	if (geishalove[2] == 1) cout << setw(W1) << left << "legyező 2" << setw(W) << to_string(geishagifts[1][2]) << setw(W) <<
		to_string(geishagifts[2][2]) << setw(W) << "X" << setw(W) << " " << setw(W) << " " << endl;
	if (geishalove[2] == 2) cout << setw(W1) << left << "legyező 2" << setw(W) << to_string(geishagifts[1][2]) << setw(W) <<
		to_string(geishagifts[2][2]) << setw(W) << " " << setw(W) << " " << setw(W) << "X" << endl;

	SetConsoleTextAttribute(hConsole, 11 * 16);
	if (geishalove[3] == 0) cout << setw(W1) << left << "tekercs 2" << setw(W) << to_string(geishagifts[1][3]) << setw(W) <<
		to_string(geishagifts[2][3]) << setw(W) << " " << setw(W) << "X" << setw(W) << " " << endl;
	if (geishalove[3] == 1) cout << setw(W1) << left << "tekercs 2" << setw(W) << to_string(geishagifts[1][3]) << setw(W) <<
		to_string(geishagifts[2][3]) << setw(W) << "X" << setw(W) << " " << setw(W) << " " << endl;
	if (geishalove[3] == 2) cout << setw(W1) << left << "tekercs 2" << setw(W) << to_string(geishagifts[1][3]) << setw(W) <<
		to_string(geishagifts[2][3]) << setw(W) << " " << setw(W) << " " << setw(W) << "X" << endl;

	SetConsoleTextAttribute(hConsole, 12 * 16);
	if (geishalove[4] == 0) cout << setw(W1) << left << "ernyő 3" << setw(W) << to_string(geishagifts[1][4]) << setw(W) <<
		to_string(geishagifts[2][4]) << setw(W) << " " << setw(W) << "X" << setw(W) << " " << endl;
	if (geishalove[4] == 1) cout << setw(W1) << left << "ernyő 3" << setw(W) << to_string(geishagifts[1][4]) << setw(W) <<
		to_string(geishagifts[2][4]) << setw(W) << "X" << setw(W) << " " << setw(W) << " " << endl;
	if (geishalove[4] == 2) cout << setw(W1) << left << "ernyő 3" << setw(W) << to_string(geishagifts[1][4]) << setw(W) <<
		to_string(geishagifts[2][4]) << setw(W) << " " << setw(W) << " " << setw(W) << "X" << endl;

	SetConsoleTextAttribute(hConsole, 6 * 16);
	if (geishalove[5] == 0) cout << setw(W1) << left << "dibár 3" << setw(W) << to_string(geishagifts[1][5]) << setw(W) <<
		to_string(geishagifts[2][5]) << setw(W) << " " << setw(W) << "X" << setw(W) << " " << endl;
	if (geishalove[5] == 1) cout << setw(W1) << left << "dibár 3" << setw(W) << to_string(geishagifts[1][5]) << setw(W) <<
		to_string(geishagifts[2][5]) << setw(W) << "X" << setw(W) << " " << setw(W) << " " << endl;
	if (geishalove[5] == 2) cout << setw(W1) << left << "dibár 3" << setw(W) << to_string(geishagifts[1][5]) << setw(W) <<
		to_string(geishagifts[2][5]) << setw(W) << " " << setw(W) << " " << setw(W) << "X" << endl;

	SetConsoleTextAttribute(hConsole, 2 * 16);
	if (geishalove[6] == 0) cout << setw(W1) << left << "tea 4" << setw(W) << to_string(geishagifts[1][6]) << setw(W) <<
		to_string(geishagifts[2][6]) << setw(W) << " " << setw(W) << "X" << setw(W) << " " << endl;
	if (geishalove[6] == 1) cout << setw(W1) << left << "tea 4" << setw(W) << to_string(geishagifts[1][6]) << setw(W) <<
		to_string(geishagifts[2][6]) << setw(W) << "X" << setw(W) << " " << setw(W) << " " << endl;
	if (geishalove[6] == 2) cout << setw(W1) << left << "tea 4" << setw(W) << to_string(geishagifts[1][6]) << setw(W) <<
		to_string(geishagifts[2][6]) << setw(W) << " " << setw(W) << " " << setw(W) << "X" << endl;

	SetConsoleTextAttribute(hConsole, 13 * 16);
	if (geishalove[7] == 0) cout << setw(W1) << left << "virág 5" << setw(W) << to_string(geishagifts[1][7]) << setw(W) <<
		to_string(geishagifts[2][7]) << setw(W) << " " << setw(W) << "X" << setw(W) << " " << endl;
	if (geishalove[7] == 1) cout << setw(W1) << left << "virág 5" << setw(W) << to_string(geishagifts[1][7]) << setw(W) <<
		to_string(geishagifts[2][7]) << setw(W) << "X" << setw(W) << " " << setw(W) << " " << endl;
	if (geishalove[7] == 2) cout << setw(W1) << left << "virág 5" << setw(W) << to_string(geishagifts[1][7]) << setw(W) <<
		to_string(geishagifts[2][7]) << setw(W) << " " << setw(W) << " " << setw(W) << "X" << endl;

	SetConsoleTextAttribute(hConsole, 15);
	cout << endl;
		/*<< setw(W1) << left << "legyező 2" << setw(W) << to_string(geishagifts[1][2]) << setw(W) << to_string(geishagifts[2][2]) << endl
		<< setw(W1) << left << "tekercs 2" << setw(W) << to_string(geishagifts[1][3]) << setw(W) << to_string(geishagifts[2][3]) << endl
		<< setw(W1) << left << "ernyő 3" << setw(W) << to_string(geishagifts[1][4]) << setw(W) << to_string(geishagifts[2][4]) << endl
		<< setw(W1) << left << "dibár 3" << setw(W) << to_string(geishagifts[1][5]) << setw(W) << to_string(geishagifts[2][5]) << endl
		<< setw(W1) << left << "tea 4" << setw(W) << to_string(geishagifts[1][6]) << setw(W) << to_string(geishagifts[2][6]) << endl
		<< setw(W1) << left << "virág 5" << setw(W) << to_string(geishagifts[1][7]) << setw(W) << to_string(geishagifts[2][7]) << endl << endl;*/
}
void endgame() {
	system("cls");
	geishagifts[1][secretcard[1]]++;
	geishagifts[2][secretcard[2]]++;
	for (int i = 1; i < 8; i++) {
		if (geishagifts[1][i] > geishagifts[2][i]) geishalove[i] = 1;
		if (geishagifts[1][i] < geishagifts[2][i]) geishalove[i] = 2;
	}
	table();
	score[1] = 0;
	score[2] = 0;
	geishanumber[1] = 0;
	geishanumber[2] = 0;
	for (int i = 1; i < 8; i++) {
		if (geishalove[i] == 1) {
			score[1] = score[1] + value[i];
			geishanumber[1]++;
		}
		if (geishalove[i] == 2) {
			score[2] = score[2] + value[i];
			geishanumber[2]++;
		}
	}
	cout << "Az első játékos " << to_string(score[1]) << " pontot gyűjtött " << to_string(geishanumber[1]) << " gésa szívének elnyerésével." << endl;
	cout << "A második játékos " << to_string(score[2]) << " pontot gyűjtött " << to_string(geishanumber[2]) << " gésa szívének elnyerésével." << endl;
	if (geishanumber[1] > 3) {
		gameover = true;
		winner = 1;
	}
	if (geishanumber[2] > 3) {
		gameover = true;
		winner = 2;
	}
	if (score[1] > 10) {
		gameover = true;
		winner = 1;
	}
	if (score[2] > 10) {
		gameover = true;
		winner = 2;
	}
	if (gameover) {
		cout << "A játéknak vége, " << to_string(winner) << ". játékos nyert! Gratulálunk!\nÚj játék kezdéséhez nyomj entert!";
		gameover = false;
		for (int i = 0; i < 8; i++) geishalove[i] = 0;
	}
	else {
		cout << "A játék folytatásához nyomj entert!";
	}
	cin >> kaki;
}
void displayhand(int player) {
	int cardnumber = 0;
	for (int i = 0; i < 11; i++) {
		if (playerhand[player][i] != 0) {
			cardnumber++;
			cout << to_string(cardnumber) << ". ";
			card(playerhand[player][i]);
			cout << endl;
		}
	}
}



