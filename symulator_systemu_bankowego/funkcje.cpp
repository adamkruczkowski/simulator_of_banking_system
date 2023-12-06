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

//! Funkcja wczyta_cofanie() zapisuje do pliku tekstowego o nazwie uzytkownika komunikat o cofnieciu transakcji
/*!
\param st okresla obecna date
\param numk konwertuje numer konta na zmienna typu string, aby mozliwe bylo znalezienie pliku danego uzytkownika
*/

void wczytaj_cofanie(int nr, int wyb, int kw, vector<Dane>& logi, vector<Transakcje>& trans)
{
    SYSTEMTIME st;
    GetSystemTime(&st);
    string numk ="uzytkownicy/" + to_string(logi[nr].numer) + ".txt";
    fstream plikpw(numk, ios::out | ios::app);
    plikpw << "Cofnales transakcje nr " << wyb << " o kwocie " << kw << "zl, w dniu " << st.wDay << "." << st.wMonth << "." << st.wYear << " o godzinie " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << endl;
    plikpw.close();
}

//!Funkcja cofanie() cofa wybrana przez uzytkownika transakcje
/*!
\param wyb wybor uzytkownika, ktora transakcje chce cofnac
*/

void cofanie(int num, vector<Dane>& logi, vector<Transakcje>& trans)
{
    system("cls");
    int wyb;
    for (int i = 0; i < trans.size(); i++)
    {
        if (num == trans[i].os1)
        {
            cout << "Numer transakcji: " << i << ". Przelew na numer konta: " << trans[i].os2 << ", kwota: " << trans[i].kwota << endl;
        }
        else
            continue;
    }
    cout << "Wybierz numer transakcji ktora chcesz cofnac: ";
    cin >> wyb;
    while (!cin)
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Nieprawidlowy wybor. Sproboj ponownie: " << endl;
        cin >> wyb;
    }
    logi[num].stan += trans[wyb].kwota;
    logi[trans[wyb].os2].stan -= trans[wyb].kwota;
    trans.erase(trans.begin() + wyb);
    cout << endl;
    wczytaj_cofanie(num,wyb,trans[wyb].kwota,logi,trans);
    cout << "Transakcja cofnieta pomyslnie. Nacisnij enter, aby powrocic do konta.";
    getchar(); getchar();
}

//! Funkcja zaladuj_transakcje laduje do wektora wszystkie transakcje zawarte w pliku tekstowym
/*!
\param linia pobiera cala linie z pliku tekstowego
\param s zmienna typu transakcje, dzieki ktorej mozliwy jest odczyt danych z pliku tekstowego i zapisanie ich do odpowiedniego wektora
*/

void zaladuj_transakcje(vector<Transakcje>& transakcje)
{
    ifstream baza_do_odczytania("transakcje.txt");
    string linia;
    while (getline(baza_do_odczytania, linia))
    {
        Transakcje s;
        stringstream line_stream(linia);
        line_stream >> s.os1 >> s.os2 >> s.kwota;
        transakcje.push_back(s);
    }
    baza_do_odczytania.close();
}

//! Funkcja wykrywanie_podejrzanych_transakcji() typu bool przeszukuje wektor Transakcje w celu odnalezienia cyklu podejrzanych transakcji, zwraca wartosc true, gdy taka transakcja zostala odnaleziona


bool wykrywanie_podejrzanych_transakcji(vector<Transakcje>& trans, int num)
{
    for (int i = 0; i < trans.size(); i++)
    {
        if (trans[i].os1 == num)
        {
            for (int j = i + 1; j < trans.size(); j++)
            {
                if (trans[j].os1 == trans[i].os2)
                {
                    if (trans[i].kwota == trans[j].kwota || trans[i].kwota > trans[j].kwota)
                    {
                        for (int k = j + 1; k < trans.size(); k++)
                        {
                            if (trans[k].os1 == trans[j].os2)
                            {
                                if (trans[j].kwota == trans[k].kwota || trans[j].kwota > trans[k].kwota)
                                {
                                    if (trans[k].os2 == num)
                                    {
                                        return true;
                                    }
                                    else
                                        continue;
                                }
                            }
                            else
                                continue;
                        }
                    }
                    else
                        continue;
                }
                else
                    continue;
            }
        }
        else
            continue;
    }
    return false;
}

//! Funkcja pokaz_debet() wyswietla obecny debet zalogowanego uzytkownika, ktory pobiera z wektora logi


void pokaz_debet(int nr, vector<Dane>& logi15, vector<Transakcje>& transakcje)
{
    system("cls");
    cout << "Debet twojego konta wynosi: " << logi15[nr].debet << "zl";
    cout << endl;
    cout << endl;
    cout << "Wcisnij enter, aby cofnac";
    getchar(); getchar();
}

//! Funckja ustaw_debet() umozliwia aktualnie zalogowanemu uzytkownikowi ustalic debet konta
/*!
\param pozyczka uzytkownik ustala ta wartosc na wartosc debetu, ktory chce zeby jego konto posiadalo
*/

void ustaw_debet(int nr, vector<Dane>& logi10, vector<Transakcje>& transakcje)
{
    system("cls");
    double pozyczka;
    cout << "Podaj jaki chcesz miec debet w banku: ";
    cin >> pozyczka;
    while (!cin)
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Nieprawidlowy wybor. Sproboj ponownie: " << endl;
        cin >> pozyczka;
    }
    pozyczka *= -1;
    logi10[nr].debet = pozyczka;
    cout << endl;
    cout << "Debet zostal pomyslnie ustawiony na twoje konto, w kazdej chwili mozesz go zmienic. Za chwile nastapi powrot do menu glownego";
    Sleep(2000);
}

//! Funckja wczytajwyplatezprzelewu() wczytuje do pliku tekstowego z historia ilosc pieniedzy, date i numer konto osoby, na ktore wyplynela kwota od zalogowanego uzytkownika 
/*!
\param st okresla obecna date
\param numk konwertuje numer konta na zmienna typu string, aby mozliwe bylo znalezienie pliku danego uzytkownika
*/

void wczytajwyplatezprzelewu(int nr, double kwprzelewu, int numkonta, vector<Dane>& logi8)
{
    SYSTEMTIME st;
    GetSystemTime(&st);
    string numk = "uzytkownicy/" + to_string(logi8[nr].numer) + ".txt";
    fstream plikpw(numk, ios::out | ios::app);
    plikpw << "Z twojego konta wyplynelo " << kwprzelewu << "zl na konto o numerze " << numkonta << " w dniu " << st.wDay << "." << st.wMonth << "." << st.wYear << " o godzinie " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << endl;
    plikpw.close();
}

//! Funckja wczytajwplatezprzelewu() wczytuje do pliku tekstowego ilosc pieniedzy, ktora wplynela na konto uzytkownika od innego uzytkownika wraz z data
/*!
\param st okresla obecna date
\param numk konwertuje numer konta na zmienna typu string, aby mozliwe bylo znalezienie pliku danego uzytkownika
*/

void wczytajwplatezprzelewu(int nr, double kwprzelewu, int numkonta, vector<Dane>& logi9)
{
    SYSTEMTIME st;
    GetSystemTime(&st);
    string numk = "uzytkownicy/" + to_string(logi9[nr].numer) + ".txt";
    fstream plikp(numk, ios::out | ios::app);
    plikp << "Na twoje konto wplynelo " << kwprzelewu << "zl z konta o numerze " << numkonta << " w dniu " << st.wDay << "." << st.wMonth << "." << st.wYear << " o godzinie " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << endl;
    plikp.close();
}

//! Funkcja wczytajwplate() wczytuje od pliku tekstowego ilosc pieniedzy, ktora uzytkownik wplacil na konto wraz z data
/*!
\param st okresla obecna date
\param numk konwertuje numer konta na zmienna typu string, aby mozliwe bylo znalezienie pliku danego uzytkownika
*/

void wczytajwplate(int nr, double kw, vector<Dane>& logi6)
{
    SYSTEMTIME st;
    GetSystemTime(&st);
    string numk = "uzytkownicy/" + to_string(logi6[nr].numer) + ".txt";
    fstream plikpw(numk, ios::out | ios::app);
    plikpw << "Wplaciles na konto: " << kw << "zl" << " w dniu " << st.wDay << "." << st.wMonth << "." << st.wYear << " o godzinie " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << endl;
    plikpw.close();
}

//! Funkcja wczytajwplate() wczytuje od pliku tekstowego ilosc pieniedzy, ktora uzytkownik wyplacil z konta wraz z data
/*!
\param st okresla obecna date
\param numk konwertuje numer konta na zmienna typu string, aby mozliwe bylo znalezienie pliku danego uzytkownika
*/

void wczytajwyplate(int nr, double kw, vector<Dane>& logi14)
{
    SYSTEMTIME st;
    GetSystemTime(&st);
    string numk = "uzytkownicy/" + to_string(logi14[nr].numer) + ".txt";
    fstream plikpw(numk, ios::out | ios::app);
    plikpw << "Wyplaciles z konta: " << kw << "zl" << " w dniu " << st.wDay << "." << st.wMonth << "." << st.wYear << " o godzinie " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << endl;
    plikpw.close();
}

//! Funkcja wyplac() pozwala uzytkownikowi wyplacic pieniadze z konta, lecz sprawdza czy ma ich wystarczajaca liczbe z uwzglednieniem debetu
/*!
\param kw uzytkownik okresla ta wartosc jako ilosc pieniedzy, ktora chce wyplacic
*/

void wyplac(int number, vector<Dane>& logi13, vector<Transakcje>& transakcje)
{
    double kw;
    system("cls");
    cout << "Podaj kwote ktora chcesz wyplacic: ";
    cin >> kw;
    while (!cin)
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Nieprawidlowy wybor. Sproboj ponownie: " << endl;
        cin >> kw;
    }
    if (-1 * logi13[number].debet + logi13[number].stan > kw)
    {
        logi13[number].stan -= kw;
        wczytajwyplate(number, kw, logi13);
        cout << endl;
        cout << "Kwota zostala pomyslnie wyplacona z twojego konta, za chwile nastapi powrot do menu glownego";
        Sleep(2000);
    }
    else
    {
        cout << "Masz za malo pieniedzy na koncie lub przekroczyles limit debetu. wcisnij enter aby powrocic do menu glownego";
        getchar(); getchar();
    }
}

//! Funkcja szukanienrkonta() typu bool przeszukuje wektor z danymi w celu odnalezienia numeru konta, na ktory uzytkownik chce przelac pieniadze, gdy go odnajdzie zwraca wartosc true


bool szukanienrkonta(vector<Dane>& elements, int szukane)
{
    auto it = find_if(elements.begin(), elements.end(), [&](const Dane& element)
        {
            return element.numer == szukane;
        });
    return it != elements.end();
}

//! Funkcja przelew() umozliwia uzytkownikowi przelanie pieniedzy na konto innego uzytkownika z uwzglednieniem czy uzytkownik, ktory chce przelac, ma na koncie wystarczajaca ilosc pieniedzy
/*!
\param nk uzytkownik okresla ta wartosc jako numer konta, na ktory chce przelac pieniadze, gdy program go nie znajdzie wyswietla odpowiedni komunikat
\param lp uzytkownik okresla ta wartosc jako ilosc pieniedzy, ktora chce przelac innemu uzytkownikowi
*/

void przelew(int numb, vector<Dane>& logi7, vector<Transakcje>& transakcje)
{
    int nk;
    double lp;
    cout << "Podaj numer konta osoby, ktorej chcesz przelac pieniadze: ";
    cin >> nk;
    while (!cin)
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Nieprawidlowy wybor. Sproboj ponownie: " << endl;
        cin >> nk;
    }
    cout << endl;
    if (szukanienrkonta(logi7, nk))
    {
        cout << "Podaj kwote ktora chcesz przelac: ";
        cin >> lp;
        while (!cin)
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Nieprawidlowy wybor. Sproboj ponownie: " << endl;
            cin >> lp;
        }
        if (-1 * logi7[numb].debet + logi7[numb].stan > lp)
        {
            logi7[numb].stan -= lp;
            logi7[nk].stan += lp;

            transakcje.resize(transakcje.size() + 1);

            transakcje[transakcje.size() - 1].os1 = numb;
            transakcje[transakcje.size() - 1].os2 = nk;
            transakcje[transakcje.size() - 1].kwota = lp;


            wczytajwyplatezprzelewu(numb, lp, nk, logi7);
            wczytajwplatezprzelewu(nk, lp, numb, logi7);
            cout << endl;
            cout << "Przelew wykonany pomyslnie. Wcisnij eneter aby kontynuowac.";
            getchar(); getchar();

        }
        else
        {
            cout << "Masz za malo pieniedzy, aby wykonac przelew. Za chwile nastapi powrot do menu glownego.";
            Sleep(2000);
        }
    }
    else
    {
        cout << "Taki numer konta nie istnieje, sprobuj ponownie" << endl;
        getchar(); getchar();
    }
}

//! Funkcja pokaz_stan() pokazuje stan konta obecnie zalogowanego uzytkownika, poprzez odnalezienie jego numeru konta w wektorze Dane


void pokaz_stan(int number, vector<Dane>& logi12, vector<Transakcje>& transakcje)
{
    system("cls");
    cout << "Stan twojego konta wynosi: " << logi12[number].stan << "zl";
    cout << endl;
    cout << endl;
    cout << "Wcisnij enter, aby cofnac";
    getchar(); getchar();
}

//! Funkcja wplac() pozwala obecnie zalogowanemu uzytkownikowi wplacic pieniadze na konto
/*!
\param kw uzytkownik okresla ta wartosc jako ilosc pieniedzy, ktora chce wplacic na konto
*/

void wplac(int num, vector<Dane>& logi5, vector<Transakcje>& transakcje)
{
    double kw;
    system("cls");
    cout << "Podaj kwote ktora wplacasz: ";
    cin >> kw;
    while (!cin)
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Nieprawidlowy wybor. Sproboj ponownie: " << endl;
        cin >> kw;
    }

    logi5[num].stan += kw;
    wczytajwplate(num, kw, logi5);
    cout << endl;
    cout << "Kwota zostala pomyslnie wplacona na twoje konto, za chwile nastapi powrot do menu glownego";
    Sleep(2000);
    konto(num, logi5, transakcje);
}

//! Funkcja pokazhistorie() znajduje plik tekstowy na podstawie numeru konta obecnie zalogowanego uzykownika i wyswietla jego zawartosc
/*!
\param numk konwertuje numer konta na zmienna typu string, aby mozliwe bylo znalezienie pliku danego uzytkownika
*/

void pokazhistorie(int nr, vector<Dane>& logi11, vector<Transakcje>& transakcje)
{
    system("cls");
    string numk ="uzytkownicy/" + to_string(logi11[nr].numer) + ".txt", line;
    ifstream file(numk);
    if (!file.is_open())
    {
        cout << "W twojej historii nic sie nie znajduje! Wcisnij enter, aby powrocic do menu glownego." << endl;
        getchar(); getchar();
        konto(nr, logi11, transakcje);
    }
    else
    {
        while (getline(file, line))
        {
            cout << line << endl;
        }
        file.close();
        cout << endl;
        cout << "Nacisnij enter aby powrocic do menu glownego.";
        getchar(); getchar();
    }
}

//! Funkcja konto() wyswietla uzytkownikowi po zalogowaniu wszystkie mozliwe opcje do wykonania dzieki funkcji switch, dziala w petli dzieki funkcji while
/*!
\param wy okresla wybor uzytkownika i wywoluje odpowiednia funkcje
*/

void konto(int nr, vector<Dane>& logi4, vector<Transakcje>& transakcje)
{
    char wy = '-';

    while (wy != '9')
    {
        system("cls");
        cout << "Witaj, " << logi4[nr].name << " wybierz operacje, ktora chcesz wykonac: " << endl;
        cout << endl;
        cout << "1. Wplac pieniadze" << endl;
        cout << "2. Wykonaj przelew" << endl;
        cout << "3. Ustaw debet" << endl;
        cout << "4. Zobacz historie" << endl;
        cout << "5. Zobacz stan konta" << endl;
        cout << "6. Wyplac pieniadze" << endl;
        cout << "7. Sprawdz swoj debet" << endl;
        cout << "8. Cofnij transakcje" << endl;
        cout << "9. Wyloguj sie" << endl;
        cout << endl;
        cout << endl;
        cout << "Wybierz opcje: ";
        cin >> wy;
            cin.ignore(10000, '\n');
        switch (wy)
        {
        case '1':
            wplac(nr, logi4, transakcje);
            break;
        case '2':
            przelew(nr, logi4, transakcje);
            break;
        case '3':
            ustaw_debet(nr, logi4, transakcje);
            break;
        case '4':
            pokazhistorie(nr, logi4, transakcje);
            break;
        case '5':
            pokaz_stan(nr, logi4, transakcje);
            break;
        case '6':
            wyplac(nr, logi4, transakcje);
            break;
        case '7':
            pokaz_debet(nr, logi4, transakcje);
            break;
        case '8':
            cofanie(nr, logi4, transakcje);
            break;
        default:
            cout << "Nieprawidlowy wybor";
            break;
        }
    }
}

//! Funkcja isInVectorNr() typu bool przeszukuje wektor z danymi w celu odnalezienia numeru konta, na ktory uzytkownik chce sie zalogowac, gdy go odnajdzie zwraca wartosc true


bool isInVectorNr(vector<Dane>& log, int x)
{
    for (int i = 0; i < log.size(); i++)
    {
        if (log[i].numer == x)
        {
            return true;
        }
    }
    return false;
}

//! Funkcja isInVectorPass() typu bool przeszukuje wektor z danymi w celu odnalezienia hasla dla numeru konta, na ktory uzytkownik chce sie zalogowac, gdy go odnajdzie zwraca wartosc true


bool isInVectorPass(vector<Dane>& log, string x, int y)
{
    if (log[y].haslo == x)
    {
        return true;
    }
    return false;
}

//! Funkcja zaladuj_baze() wywolywana jest na poczatku programu i wczytuje do wektora dane z pliku tekstowego w celu mozliwosci ponownego uzytkowania programu po jego zmknieciu
/*!
\param linia pobiera linie z pliku tekstowego, w ktorej znajduja sie dane
*/

void zaladuj_baze(vector<Dane>& logix)
{
    ifstream baza_do_odczytania("baza.txt");
    string linia;
    while (getline(baza_do_odczytania, linia))
    {
        Dane s;
        stringstream line_stream(linia);
        line_stream >> s.numer >> s.name >> s.secondname >> s.haslo >> s.stan >> s.debet;
        logix.push_back(s);
    }
    baza_do_odczytania.close();
}

//! Funkcja utworz() umozliwia nowemu uzytkownikowi utworzenie konta w banku, ktore nastepnie umieszcza w wektorze struktur, wraz z innymi kontami
/*!
\param wiek uzytkownik musi podac wiek w celu zweryfikowania pelnoletnosci
\param imie uzytkownik podaje imie, ktore trafia do bazy
\param nazwisko uzytkownik podaje nazwisko, ktore trafia do bazy
\param haslo uzytkownik podaje haslo, ktorym bedzie sie logowal
\param haslo2 uzytkownik potwierdza haslo, ktorym bedzie sie logowal, jesli sie nie zgadzaja musi przejsc operacje tworzenia konta od nowa
*/

void utworz(vector<Dane>& logi3, vector<Transakcje>& transakcje)
{

    int wiek;
    string imie, nazwisko, haslo, haslo2;
    fstream baza;
    system("cls");
    cout << endl;
    cout << "Podaj wiek: ";
    cin >> wiek;
    while (!cin)
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Nieprawidlowy wybor. Sproboj ponownie: " << endl;
        cin >> wiek;
    }
    cout << endl;
    if (wiek < 18)
    {
        cout << "Mozliwosc rezrewacji tylko dla pelnoletnich, poczekaj az dorosniesz! Za chwile nastapi powrot do menu glownego";
        Sleep(5000);
    }
    else
    {
        cout << "Podaj imie: ";
        cin >> imie;
        cout << endl;
        cout << "Podaj nazwisko: ";
        cin >> nazwisko;
        cout << endl;
        cout << "Podaj haslo: ";
        cin >> haslo;
        cout << endl;
        cout << "Potwierdz haslo: ";
        cin >> haslo2;

        if (haslo2 != haslo)
        {
            system("cls");
            cout << endl;
            cout << "nieprawidlowo potwierdzone haslo, sprobuj ponownie" << endl;
            Sleep(5000);
        }
        else
        {
            logi3.resize(logi3.size() + 1);

            logi3[logi3.size() - 1].numer = logi3.size() - 1;
            logi3[logi3.size() - 1].name = imie;
            logi3[logi3.size() - 1].secondname = nazwisko;
            logi3[logi3.size() - 1].haslo = haslo;
            logi3[logi3.size() - 1].stan = 50;
            logi3[logi3.size() - 1].debet = -1;
            cout << endl;
            cout << "Witamy w naszym banku. Twoj numer konta to " << logi3.size() - 1 << " Za chwile nastapi przekierowanie do menu glownego, gdzie bedziesz mogl sie zalogowac.";
            Sleep(3000);
        }
    }
}

//! Funkcja wpisz_do_bazy() jest wywolywana na koniec programu w celu zapisania do pliku wszystkich operacji uzytkownikow podczas dzialania programu


void wpisz_do_bazy(vector<Dane>& logi21)
{
    ofstream plik("baza.txt");
    plik.clear();
    if (plik.is_open())
    {
        for (const auto& s : logi21)
        {
            plik << s.numer << " " << s.name << " " << s.secondname << " " << s.haslo << " " << s.stan << " " << s.debet << endl;
        }
    }
}

//! Funkcja wpisz_do_transakcji() jest wywolywana na koniec programu w celu zapisania do pliku wszystkich transakcji uzytkownikow podczas dzialania programu


void wpisz_do_transakcji(vector<Transakcje>& trans)
{
    ofstream plik("transakcje.txt");
    plik.clear();
    if (plik.is_open())
    {
        for (const auto& s : trans)
        {
            plik << s.os1 << " " << s.os2 << " " << s.kwota << endl;
        }
    }
    plik.close();
}

//! Funkcja zaloguj() umozliwia zarejestrowanym uzytkownikom zalogowanie sie poprzez sprawdzanie czy w bazie danych znajduja sie odpowiednie hasla i numery konta
/*!
\param nr uzytkownik podaje numer konta, na ktory chce sie zalogowac
\param pass uzytkownik podaje haslo, ktore jest przypisane do danego numeru konta
*/

void zaloguj(vector <Dane>& logi2, vector<Transakcje>& transakcje)
{
    system("cls");
    int nr{};
    string pass = "";
    cout << "Podaj numer konta: ";
    cin >> nr;
    while (!cin)
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Nieprawidlowy wybor. Sproboj ponownie: " << endl;
        cin >> nr;
    }
    cout << endl;
    if (szukanienrkonta(logi2, nr))
    {
        if (isInVectorNr(logi2, nr))
        {
            cout << "Podaj haslo: ";
            cin >> pass;
            cout << endl;
            if (isInVectorPass(logi2, pass, nr))
            {
                cout << "Za chwile nastapi przekierowanie na twoje konto";
                Sleep(3000);
                konto(nr, logi2, transakcje);

            }
            else {
                cout << "Nieprawid³owe haslo, sprobuj ponownie za 3 sekundy";
                Sleep(3000);
                system("cls");
                zaloguj(logi2, transakcje);
            }
        }
        else {
            cout << "Nieprawid³owy numer konta, sprobuj ponownie za 3 sekundy";
            Sleep(3000);
            system("cls");
            zaloguj(logi2, transakcje);
        }
    }
    else
    {
        cout << "Taki numer konta nie istnieje, sprobuj ponownie. Wcisnij enter aby kontynuowac." << endl;
        getchar(); getchar();
    }

}

//! Funkcja spr_dla_admina() typu bool zwraca true, gdy administrator poda odpowiednie haslo, w celu sprawdzenia podejrzanych transakcji dla wybranego konta
/*!
\param haslo administrator podaje je w celu zweryfikowania tego, czy jest administratorem
*/

bool spr_dla_admina()
{
    system("cls");
    string haslo;
    cout << "Wpisz haslo: ";
    cin >> haslo;
    if (haslo == "admin")
        return true;
    else
        return false;
}

//! Funkcja menu() wyswietla menu glowne po uruchomieniu programu, gdzie uzytkownik moze dokonac wyboru, co chce dalej zrobic
/*!
\param wyb zmienna okreslajaca wybor uzytkownika
*/

void menu(vector<Dane>& logi1, vector<Transakcje>& transakcje)
{
    char wyb = '!';
    while (wyb != '4')
    {
        system("cls");
        wyb = getchar();
        cout << "============== WITAJ W BANKU ==============" << endl;
        cout << endl;
        cout << "1. Zaloguj sie" << endl;
        cout << "2. Utworz konto" << endl;
        cout << "3. Sprawdz podejrzane transakcje" << endl;
        cout << "4. Zamknij aplikacje banku";
        cout << endl;
        cout << endl;
        cout << "Wybierz opcje: ";
        cin >> wyb;
        cin.ignore(10000, '\n');
        switch (wyb)
        {
        case '1':
            zaloguj(logi1, transakcje);
            break;
        case '2':
            utworz(logi1, transakcje);
            break;
        case '3':
            if (spr_dla_admina())
            {
                system("cls");
                int num;
                cout << "Wpisz numer konta dla ktorego chcesz sprawdzic podejrzane transakcje: ";
                cin >> num;
                while (!cin)
                {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Nieprawidlowy wybor. Sproboj ponownie: " << endl;
                    cin >> num;
                }
                if (wykrywanie_podejrzanych_transakcji(transakcje, num))
                {
                    cout << "Wykryto podejrzana transakcje dla konta: " << num << endl;
                    getchar(); getchar();
                }
                else
                {
                    cout << "Nie wykryto podejrzanej transakcji dla konta " << num << endl;
                    getchar(); getchar();
                }
            }
            else
            {
                cout << "Nie masz uprawnien do tej opcji.";
                getchar(); getchar();
            }
            break;
        case '4':
            wpisz_do_bazy(logi1);
            wpisz_do_transakcji(transakcje);
            break;
        default:
            cout << "Nieprawidlowy wybor";
            break;
        }
    }
}

