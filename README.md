# System Rezerwacji Biletów

Konsolowa aplikacja napisana w języku C++ umożliwiająca zarządzanie wydarzeniami oraz rezerwację i anulowanie biletów. Projekt wykorzystuje paradygmat programowania obiektowego (OOP) oraz zapewnia trwałość danych poprzez automatyczny zapis i odczyt stanu z pliku tekstowego.

## Funkcjonalności

* **Przeglądanie wydarzeń** – wyświetlanie listy dostępnych wydarzeń wraz z datą, pulą miejsc, liczbą zarezerwowanych oraz wolnych biletów.
* **Dodawanie wydarzeń** – tworzenie nowych wydarzeń z określoną nazwą, datą i limitem miejsc.
* **Rezerwacja biletów** – rezerwowanie wybranej liczby miejsc na konkretne wydarzenie z walidacją dostępności.
* **Anulowanie rezerwacji** – zwalnianie wcześniej zarezerwowanych miejsc.
* **Trwałość danych** – automatyczne wczytywanie danych przy starcie programu oraz zapis zmian do pliku `rezerwacje.txt`.

## Struktura projektu

```text
system-rezerwacji-biletow/
├── main.cpp           # Główny plik źródłowy zawierający klasy oraz logikę menu
├── rezerwacje.txt     # Plik bazy danych przechowujący zapisane wydarzenia (generowany automatycznie)
└── README.md          # Dokumentacja projektu
```

## Instrukcja kompilacji i uruchomienia

### Wymagania
* Kompilator wspierający standard **C++11** lub nowszy (np. `g++`, `clang++`, MSVC).

### Kompilacja w terminalu (Linux / macOS / Windows MinGW)
1. Sklonuj repozytorium lub pobierz plik `main.cpp`:
   ```bash
   git clone [https://github.com/twoj-login/system-rezerwacji-biletow.git](https://github.com/twoj-login/system-rezerwacji-biletow.git)
   cd system-rezerwacji-biletow
   ```
2. Skompiluj kod źródłowy:
   ```bash
   g++ -std=c++17 -Wall main.cpp -o system_rezerwacji
   ```
3. Uruchom program:
   * **Linux / macOS:**
     ```bash
     ./system_rezerwacji
     ```
   * **Windows (CMD / PowerShell):**
     ```cmd
     system_rezerwacji.exe
     ```

## Opis klas i metod

### 1. Klasa `Wydarzenie`
Reprezentuje pojedyncze wydarzenie oraz zarządza pulą jego biletów.

**Pola prywatne:**
* `string nazwa` – nazwa wydarzenia.
* `string data` – data wydarzenia (np. w formacie `RRRR-MM-DD`).
* `int liczbaMiejsc` – całkowita pojemność wydarzenia.
* `int liczbaZarezerwowanych` – liczba aktualnie zajętych miejsc.

**Konstruktory i metody:**

| Sygnatura metody | Parametry | Zwracany typ | Opis |
| :--- | :--- | :--- | :--- |
| `Wydarzenie(string naz, string dat, int miejsc)` | `naz` – nazwa<br>`dat` – data<br>`miejsc` – pula miejsc | – | Tworzy nowe wydarzenie z zerową liczbą rezerwacji. |
| `Wydarzenie(string naz, string dat, int miejsc, int zarezerwowane)` | Jak wyżej + `zarezerwowane` | – | Konstruktor używany podczas wczytywania danych z pliku. |
| `bool zarezerwujMiejsca(int liczba)` | `liczba` – liczba biletów | `bool` | Rezerwuje podaną liczbę miejsc, jeśli `liczba > 0` oraz jest wystarczająca liczba wolnych miejsc. Zwraca `true` w przypadku powodzenia. |
| `bool zwolnijMiejsca(int liczba)` | `liczba` – liczba biletów | `bool` | Anuluje rezerwację podanej liczby miejsc, o ile nie przekracza ona liczby miejsc zarezerwowanych. |
| `int zwrocWolneMiejsca() const` | Brak | `int` | Oblicza i zwraca różnicę między całkowitą liczbą miejsc a liczbą miejsc zarezerwowanych. |
| `string zwrocInformacje() const` | Brak | `string` | Zwraca sformatowany ciąg znaków ze szczegółami wydarzenia. |
| `getNazwa()`, `getData()`, `getLiczbaMiejsc()`, `getLiczbaZarezerwowanych()` | Brak | `string` / `int` | Gettery zwracające wartości poszczególnych pól prywatnych. |

---

### 2. Klasa `SystemRezerwacji`
Zarządza kolekcją wszystkich wydarzeń oraz obsługuje operacje wejścia/wyjścia na plikach.

**Pola prywatne:**
* `vector<Wydarzenie> wydarzenia` – kontener przechowujący obiekty klasy `Wydarzenie`.

**Metody publiczne:**

| Sygnatura metody | Parametry | Zwracany typ | Opis |
| :--- | :--- | :--- | :--- |
| `void dodajWydarzenie(const Wydarzenie& w)` | `w` – obiekt wydarzenia | `void` | Dodaje przekazane wydarzenie do wektora `wydarzenia`. |
| `void wyswietlWydarzenia() const` | Brak | `void` | Wypisuje ponumerowaną listę wszystkich wydarzeń w konsoli lub komunikat o ich braku. |
| `Wydarzenie* znajdzWydarzenie(const string& nazwa)` | `nazwa` – szukana nazwa | `Wydarzenie*` | Wyszukuje wydarzenie po dokładnej nazwie. Zwraca wskaźnik na obiekt lub `nullptr`, jeśli nie znaleziono. |
| `void zapiszDoPliku(const string& nazwaPliku) const` | `nazwaPliku` – ścieżka pliku | `void` | Zapisuje wszystkie wydarzenia do pliku w formacie CSV rozdzielonym średnikami (`;`). |
| `void wczytajZPliku(const string& nazwaPliku)` | `nazwaPliku` – ścieżka pliku | `void` | Odczytuje dane z pliku tekstowego, parsuje linie i odtwarza listę wydarzeń w pamięci. |

---

### 3. Funkcje pomocnicze
* `void wyczyscBufor()` – czyści flagi błędów strumienia `cin` oraz usuwa pozostałe znaki z bufora wejściowego (zapobiega błędom przy mieszaniu `cin >>` oraz `getline()`).

## Przykładowe dane i użycie programu

### Przykładowa zawartość pliku `rezerwacje.txt`
Dane przechowywane są w formacie `nazwa;data;liczbaMiejsc;liczbaZarezerwowanych`:
```text
Koncert Rockowy;2026-06-15;200;45
Spektakl Teatralny - Hamlet;2026-07-02;80;80
Festiwal Kina Niezaleznego;2026-08-10;150;12
```

### Przykładowy przebieg działania w konsoli
```text
Informacja: Brak pliku z danymi lub nie mozna go otworzyc.

=== SYSTEM REZERWACJI BILETOW ===
1. Wyswietl liste wydarzen
2. Dodaj nowe wydarzenie
3. Zarezerwuj bilety
4. Anuluj rezerwacje
5. Zapisz dane do pliku
6. Wyjdz
Wybierz opcje (1-6): 2
Podaj nazwe wydarzenia: Koncert Jazzowy
Podaj date (np. 2026-06-15): 2026-06-20
Podaj calkowita liczbe miejsc: 100
Dodano nowe wydarzenie pomyslnie!
Dane zostaly pomyslnie zapisane do pliku: rezerwacje.txt

=== SYSTEM REZERWACJI BILETOW ===
1. Wyswietl liste wydarzen
2. Dodaj nowe wydarzenie
3. Zarezerwuj bilety
4. Anuluj rezerwacje
5. Zapisz dane do pliku
6. Wyjdz
Wybierz opcje (1-6): 3

--- LISTA WYDARZEN ---
1. Wydarzenie: Koncert Jazzowy | Data: 2026-06-20 | Wszystkich miejsc: 100 | Zarezerwowane: 0 | Wolne: 100
----------------------
Podaj nazwe wydarzenia, na ktore chcesz zarezerwowac bilety: Koncert Jazzowy
Podaj liczbe miejsc do rezerwacji: 4
Sukces! Zarezerwowano 4 miejsc na wydarzenie: Koncert Jazzowy.
Dane zostaly pomyslnie zapisane do pliku: rezerwacje.txt

=== SYSTEM REZERWACJI BILETOW ===
1. Wyswietl liste wydarzen
2. Dodaj nowe wydarzenie
3. Zarezerwuj bilety
4. Anuluj rezerwacje
5. Zapisz dane do pliku
6. Wyjdz
Wybierz opcje (1-6): 1

--- LISTA WYDARZEN ---
1. Wydarzenie: Koncert Jazzowy | Data: 2026-06-20 | Wszystkich miejsc: 100 | Zarezerwowane: 4 | Wolne: 96
----------------------
```



## Autor

* **Autor:** [Roman Shchepet]
* **Data wykonania:** 25.11.2026
