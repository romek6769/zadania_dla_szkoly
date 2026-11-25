#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

class Wydarzenie {
private:
    string nazwa;
    string data;
    int liczbaMiejsc;
    int liczbaZarezerwowanych;

public:
    Wydarzenie(string naz, string dat, int miejsc)
        : nazwa(naz), data(dat), liczbaMiejsc(miejsc), liczbaZarezerwowanych(0) {
    }

    Wydarzenie(string naz, string dat, int miejsc, int zarezerwowane)
        : nazwa(naz), data(dat), liczbaMiejsc(miejsc), liczbaZarezerwowanych(zarezerwowane) {
    }

    bool zarezerwujMiejsca(int liczba) {
        if (liczba <= 0) {
            cout << "Liczba miejsc do rezerwacji musi byc wieksza od zera.\n";
            return false;
        }
        if (zwrocWolneMiejsca() < liczba) {
            cout << "Brak wystarczajacej liczby miejsc.\n";
            return false;
        }
        liczbaZarezerwowanych += liczba;
        cout << "Sukces! Zarezerwowano " << liczba << " miejsc na wydarzenie: " << nazwa << ".\n";
        return true;
    }

    bool zwolnijMiejsca(int liczba) {
        if (liczba <= 0) {
            cout << "Liczba miejsc do anulowania musi byc wieksza od zera.\n";
            return false;
        }
        if (liczba > liczbaZarezerwowanych) {
            cout << "Nie mozna anulowac wiecej miejsc niz zarezerwowano.\n";
            return false;
        }
        liczbaZarezerwowanych -= liczba;
        cout << "Sukces! Anulowano rezerwacje " << liczba << " miejsc na wydarzenie: " << nazwa << ".\n";
        return true;
    }

    int zwrocWolneMiejsca() const {
        return liczbaMiejsc - liczbaZarezerwowanych;
    }

    string zwrocInformacje() const {
        return "Wydarzenie: " + nazwa +
            " | Data: " + data +
            " | Wszystkich miejsc: " + to_string(liczbaMiejsc) +
            " | Zarezerwowane: " + to_string(liczbaZarezerwowanych) +
            " | Wolne: " + to_string(zwrocWolneMiejsca());
    }

    string getNazwa() const { return nazwa; }
    string getData() const { return data; }
    int getLiczbaMiejsc() const { return liczbaMiejsc; }
    int getLiczbaZarezerwowanych() const { return liczbaZarezerwowanych; }
};

class SystemRezerwacji {
private:
    vector<Wydarzenie> wydarzenia;

public:
    void dodajWydarzenie(const Wydarzenie& w) {
        wydarzenia.push_back(w);
        cout << "Dodano nowe wydarzenie pomyslnie!\n";
    }

    void wyswietlWydarzenia() const {
        if (wydarzenia.empty()) {
            cout << "Brak wydarzen w systemie.\n";
            return;
        }
        cout << "\n--- LISTA WYDARZEN ---\n";
        for (size_t i = 0; i < wydarzenia.size(); ++i) {
            cout << i + 1 << ". " << wydarzenia[i].zwrocInformacje() << "\n";
        }
        cout << "----------------------\n";
    }

    Wydarzenie* znajdzWydarzenie(const string& nazwa) {
        for (auto& w : wydarzenia) {
            if (w.getNazwa() == nazwa) {
                return &w;
            }
        }
        return nullptr;
    }

    void zapiszDoPliku(const string& nazwaPliku) const {
        ofstream plik(nazwaPliku);
        if (!plik.is_open()) {
            cout << "Blad: Nie mozna otworzyc pliku do zapisu!\n";
            return;
        }
        for (const auto& w : wydarzenia) {
            plik << w.getNazwa() << ";"
                << w.getData() << ";"
                << w.getLiczbaMiejsc() << ";"
                << w.getLiczbaZarezerwowanych() << "\n";
        }
        plik.close();
        cout << "Dane zostaly pomyslnie zapisane do pliku: " << nazwaPliku << "\n";
    }

    void wczytajZPliku(const string& nazwaPliku) {
        ifstream plik(nazwaPliku);
        if (!plik.is_open()) {
            cout << "Informacja: Brak pliku z danymi lub nie mozna go otworzyc.\n";
            return;
        }
        wydarzenia.clear();
        string linia;
        while (getline(plik, linia)) {
            stringstream ss(linia);
            string nazwa, data, sMiejsc, sZarezerwowane;

            if (getline(ss, nazwa, ';') &&
                getline(ss, data, ';') &&
                getline(ss, sMiejsc, ';') &&
                getline(ss, sZarezerwowane, ';')) {
                int miejsc = stoi(sMiejsc);
                int zarezerwowane = stoi(sZarezerwowane);
                wydarzenia.emplace_back(nazwa, data, miejsc, zarezerwowane);
            }
        }
        plik.close();
        cout << "Wczytano wydarzenia z pliku: " << nazwaPliku << "\n";
    }
};

void wyczyscBufor() {
    cin.clear();
    cin.ignore(10000, '\n');
}

int main() {
    SystemRezerwacji systemRezerwacji;
    string plikDanych = "rezerwacje.txt";

    systemRezerwacji.wczytajZPliku(plikDanych);

    int wybor;
    do {
        cout << "\n=== SYSTEM REZERWACJI BILETOW ===\n";
        cout << "1. Wyswietl liste wydarzen\n";
        cout << "2. Dodaj nowe wydarzenie\n";
        cout << "3. Zarezerwuj bilety\n";
        cout << "4. Anuluj rezerwacje\n";
        cout << "5. Zapisz dane do pliku\n";
        cout << "6. Wyjdz\n";
        cout << "Wybierz opcje (1-6): ";

        if (!(cin >> wybor)) {
            cout << "Niepoprawny wybor. Sprobuj ponownie.\n";
            wyczyscBufor();
            continue;
        }
        wyczyscBufor();

        switch (wybor) {
        case 1: {
            systemRezerwacji.wyswietlWydarzenia();
            break;
        }
        case 2: {
            string nazwa, data;
            int miejsca;
            cout << "Podaj nazwe wydarzenia: ";
            getline(cin, nazwa);
            cout << "Podaj date (np. 2026-06-15): ";
            getline(cin, data);
            cout << "Podaj calkowita liczbe miejsc: ";
            while (!(cin >> miejsca) || miejsca <= 0) {
                cout << "Podaj poprawna liczbe miejsc (>0): ";
                wyczyscBufor();
            }
            wyczyscBufor();

            systemRezerwacji.dodajWydarzenie(Wydarzenie(nazwa, data, miejsca));
            systemRezerwacji.zapiszDoPliku(plikDanych);
            break;
        }
        case 3: {
            string nazwa;
            int liczba;
            systemRezerwacji.wyswietlWydarzenia();
            cout << "Podaj nazwe wydarzenia, na ktore chcesz zarezerwowac bilety: ";
            getline(cin, nazwa);

            Wydarzenie* w = systemRezerwacji.znajdzWydarzenie(nazwa);
            if (w != nullptr) {
                cout << "Podaj liczbe miejsc do rezerwacji: ";
                while (!(cin >> liczba) || liczba <= 0) {
                    cout << "Podaj poprawna liczbe miejsc (>0): ";
                    wyczyscBufor();
                }
                wyczyscBufor();
                w->zarezerwujMiejsca(liczba);
                systemRezerwacji.zapiszDoPliku(plikDanych);
            }
            else {
                cout << "Nie znaleziono wydarzenia o takiej nazwie.\n";
            }
            break;
        }
        case 4: {
            string nazwa;
            int liczba;
            systemRezerwacji.wyswietlWydarzenia();
            cout << "Podaj nazwe wydarzenia, w ktorym chcesz anulowac rezerwacje: ";
            getline(cin, nazwa);

            Wydarzenie* w = systemRezerwacji.znajdzWydarzenie(nazwa);
            if (w != nullptr) {
                cout << "Podaj liczbe miejsc do anulowania: ";
                while (!(cin >> liczba) || liczba <= 0) {
                    cout << "Podaj poprawna liczbe miejsc (>0): ";
                    wyczyscBufor();
                }
                wyczyscBufor();
                w->zwolnijMiejsca(liczba);
                systemRezerwacji.zapiszDoPliku(plikDanych);
            }
            else {
                cout << "Nie znaleziono wydarzenia o takiej nazwie.\n";
            }
            break;
        }
        case 5: {
            systemRezerwacji.zapiszDoPliku(plikDanych);
            break;
        }
        case 6: {
            cout << "Zamykanie programu. Do widzenia!\n";
            break;
        }
        default: {
            cout << "Nieznana opcja. Wybierz liczbe od 1 do 6.\n";
            break;
        }
        }
    } while (wybor != 6);

    return 0;
}