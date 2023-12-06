#ifndef FUNCKJE_HPP
#define FUNCKJE_HPP

using namespace std;

struct Dane
{
    int numer = 0;
    string name;
    string secondname;
    string haslo;
    double stan = 0;
    double debet = 0.01;
};

struct Transakcje
{
    int os1, os2;
    float kwota;
};

void wczytaj_cofanie(int nr, int wyb, int kw, vector<Dane>& logi, vector<Transakcje>& trans);
void cofanie(int num, vector<Dane>& logi, vector<Transakcje>& trans);
void zaladuj_transakcje(vector<Transakcje>& transakcje);
bool wykrywanie_podejrzanych_transakcji(vector<Transakcje>& trans, int num);
void pokaz_debet(int nr, vector<Dane>& logi15, vector<Transakcje>& transakcje);
void ustaw_debet(int nr, vector<Dane>& logi10, vector<Transakcje>& transakcje);
void wczytajwyplatezprzelewu(int nr, double kwprzelewu, int numkonta, vector<Dane>& logi8);
void wczytajwplatezprzelewu(int nr, double kwprzelewu, int numkonta, vector<Dane>& logi9);
void wczytajwplate(int nr, double kw, vector<Dane>& logi6);
void wczytajwyplate(int nr, double kw, vector<Dane>& logi14);
void wyplac(int number, vector<Dane>& logi13, vector<Transakcje>& transakcje);
bool szukanienrkonta(vector<Dane>& elements, int szukane);
void przelew(int numb, vector<Dane>& logi7, vector<Transakcje>& transakcje);
void pokaz_stan(int number, vector<Dane>& logi12, vector<Transakcje>& transakcje);
void wplac(int num, vector<Dane>& logi5, vector<Transakcje>& transakcje);
void pokazhistorie(int nr, vector<Dane>& logi11, vector<Transakcje>& transakcje);
void konto(int nr, vector<Dane>& logi4, vector<Transakcje>& transakcje);
bool isInVectorNr(vector<Dane>& log, int x);
bool isInVectorPass(vector<Dane>& log, string x, int y);
void zaladuj_baze(vector<Dane>& logix);
void utworz(vector<Dane>& logi3, vector<Transakcje>& transakcje);
void wpisz_do_bazy(vector<Dane>& logi21);
void wpisz_do_transakcji(vector<Transakcje>& trans);
void zaloguj(vector <Dane>& logi2, vector<Transakcje>& transakcje);
bool spr_dla_admina();
void menu(vector<Dane>& logi1, vector<Transakcje>& transakcje);

#endif
