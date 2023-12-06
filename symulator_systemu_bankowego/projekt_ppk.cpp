#include <iostream>
#include <cstdlib>
#include <ctime>
#include <Windows.h>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <windows.h>
#include <winbase.h>
#include "funkcje.hpp"

using namespace std;

//! Funkcja glowna programu tworzy wektor z zmienna typu Dane i nastepny z zmienna typu Transakcje, nastepnie wywoluje funkcje, ktore laduja z plikow tekstowych dane do wczesniej utworzonych wektorow
/*!
\param transakcje wektor typu Transakcje
\param logi wektor typu Dane
\param i parametr typu string służy do wywołania w konsoli.
*/

int main(int argc, char* argv[])
{

    string input, output;

    for (int i = 1; i < argc - 1; i++) {
        string argument = argv[i];
        if (argv[i + 1] != " ")
        {

            if (argument == "-i") {
                string argument2 = argv[i + 1];
                input = argument2;
                i++;
            }
            if (argument == "-o") {
                string argument2 = argv[i + 1];
                output = argument2;
                i++;
            }

        }
    }

    vector<Transakcje> transakcje;
    vector<Dane> logi;
    zaladuj_baze(logi);
    zaladuj_transakcje(transakcje);
    menu(logi,transakcje);
}

