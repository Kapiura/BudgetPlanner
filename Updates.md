# Updates of app

---

## Aktualizacja 5

- **Okno logowania**
    - Dodano funkcjonalność wyświetlania okienka z cytatami dotyczącymi pieniędzy po zalogowaniu
    - Cy­taty są pobierane z bazy danych, gdzie każdemu dniowi jest przypisany inny cytat
    - Zaimplementowano wyświetlanie okienka z cytatami po każdym logowaniu użytkownika

- **Tabela oszczędności**
    - Dodano funkcjonalność zmiany licznika obecnego salda celu oszczędności po dodaniu rekordów
    - Po osiągnięciu celu oszczędzania wyświetla się okienko z gratulacjami, a następnie usuwane są wszystkie rekordy związane z tym oszczędzaniem oraz sam cel
    - Do celu oszczędzania dodano progress bar, który procentowo pokazuje ilość zebranych pieniędzy na dany cel
    - Dodano przycisk usuwania celu, który usuwa również rekordy w tabeli oszczędności należące do tego celu

- **Podsumowanie**
    - Dodano wyświetlanie celów na zakładce "Podsumowanie", gdzie każdy cel jest wyświetlany jako blok zawierający tytuł, obecną i docelową ilość pieniędzy, opis, progress bar oraz przycisk usuń
    - Przycisk "Usuń" przy celu usuwa cel oraz rekordy w tabeli oszczędności z nim związane

- **Ogólne ulepszenia**
    - Zaimplementowano wiele udogodnień w kodzie, takich jak zastąpienie wielu metod jedną, dynamiczne aktualizowanie danych w czasie rzeczywistym oraz usuwanie istniejących widżetów dzięki nowej metodzie usuwania

Powyższe zmiany przyczyniły się do usprawnienia kodu oraz zmniejszenia jego objętości, co pozwoliło na bardziej elastyczną i dynamiczną pracę z aplikacją.

---

## Aktualizacja 4 

- **Baza danych**
    - Dodano obsługę tabeli rekordów savings
    - Zaimplementowano funkcjonalność dodawania rekordów do tabeli savings
    - Dodano obsługę tabeli goals
    - Zaimplementowano funkcjonalność dodawania rekordów do tabeli goals

- **Panel użytkownika**
    - Dodano wyświetlanie tabelki z rekordami savings
    - Zaimplementowano dynamiczne dodawanie opcji do comboboxa w savings
    - Dodano możliwość tworzenia nowych użytkowników
    - Zaimplementowano funkcję wylogowywania oraz wyboru innego użytkownika z listy
    - Dodano miejsce na podsumowanie wydatków
    - Dodano miejsce na ustawienia
    - Dodano tekst powitalny użytkownika z jego nazwą użytkownika

---

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
