# Updates of app


## Aktualizacja 3

### Dodanie zakładek do głównego panelu
- Dodano zakładki do głównego panelu: logowanie oraz tworzenie użytkownika.
- Po zalogowaniu użytkownika przenosi się do głównego panelu, gdzie znajdują się zakładki: 
    - Expenses
    - Incomes
    - Savings Goals
    - User Settings
    - Settings

### Formularze Expenses i Incomes
- W zakładce "Expenses" oraz "Incomes" znajduje się prosty formularz, który jeszcze nie przesyła zapytania do bazy danych, ale zawiera pola:
    - Amount
    - Currency
    - Category
    - Description
    - Przycisk "Add"

### Dynamiczne dodawanie kategorii
- W kategorii dynamicznie dodają się kategorie z bazy danych.

---

## Aktualizacja 2

- **Baza danych**
    - Stworzenie w klasie DatabaseManager konstruktora łączącego się z bazą danych projektu

- **User Panel**
    - Inicjowanie panelu uzytkownika
    - tworzenie dynamicznigo okienka z logowaniem oraz wyborem użytkowników
    - wyswietlanie panelu w glównym oknie projektu
    - wyswietlanie dynamicznie uzytkownikow z bazy danych wraz z ich nazwa oraz przyciskiem login
    - Przycisk logowania dziala i loguje w pewnym sensie uzytkownika
    - poprawne łączenie z baza danych

---

## Aktualizacja 1

- **Plik run**
  - Prosty skrypt wejściowy, który przenosi do katalogu projektu, buduje go i uruchamia, jeśli wszystko przebiega pomyślnie. Sprawdza, czy qmake jest zainstalowany, a jeśli nie, to próbuje go zainstalować, jednakże tylko na systemach typu Fedora.

- **Plik Updates**
  - Ten plik służy do opisywania dotychczasowych postępów w projekcie.
  - Dodano pierwszą aktualizację, czyli tę.

- **Plik readme**
  - Zawiera opis projektu, instrukcje instalacji, funkcje, które zostaną zaimplementowane, użyte technologie oraz wymagania.

- **Stworzenie projektu**
  - Stworzono prosty projekt, który składa się z pustego okna, automatycznie tworzonego wraz z utworzeniem podstawowego projektu w Qt Creator.

- **Plik gitignore**
  - Zaktualizowano plik gitignore, dodając pliki zbędne dla projektu.
---
