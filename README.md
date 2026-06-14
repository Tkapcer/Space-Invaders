# Space Invaders - Gra 2D w SFML

## Informacje o projekcie
* **Typ projektu:** Projekt realizowany w ramach studiów.
* **Przedmiot akademicki:** Programowanie Komputerów 4.

## Status projektu
> Projekt został pomyślnie ukończony, zaliczony i **nie jest już aktywnie rozwijany ani utrzymywany**. Kod odzwierciedla stan aplikacji w momencie zakończenia przedmiotu akademickiego.

---

## Opis projektu
Projekt to dwuwymiarowa gra komputerowa zainspirowana kultowym, klasycznym tytułem **"Space Invaders"**. Gracz przejmuje kontrolę nad kosmicznym statkiem bojowym, broniąc dolnej krawędzi ekranu przed nacierającymi falami obcych. Głównym celem rozgrywki jest eliminacja przeciwników, zdobywanie punktów oraz ustanawianie nowych rekordów.

### Kluczowe funkcjonalności:
* **Klasyczna mechanika rozgrywki:** Intuicyjne sterowanie ruchem statku (lewo/prawo) oraz możliwość prowadzenia ognia ciągłego (wystrzeliwanie pocisków).
* **System zarządzania przeciwnikami:** Automatyczne generowanie hord obcych, kontrolowanie ich trajektorii ruchu, prędkości oraz koordynacja kontrataków.
* **Profilowanie gracza i autoryzacja:** Możliwość zalogowania się do systemu i podania unikalnego pseudonimu (nicku). Wyniki są trwale wiązane z profilem użytkownika.
* **Lokalna tabela wyników (Top Scores):** System zapisu i odczytu najlepszych rezultatów osiągniętych przez graczy, motywujący do poprawiania osiągów.

---

## Architektura i Wykorzystane Technologie

Gra została stworzona w paradygmacie **programowania obiektowego** z wykorzystaniem zaawansowanych technik języka C++.

* **Biblioteka graficzna:** **SFML (Simple and Fast Multimedia Library)** – wykorzystana do renderowania grafiki 2D, obsługi zdarzeń systemowych (klawiatura), zarządzania oknem aplikacji oraz wyświetlania czcionek.
* **Wielowątkowość:** Detekcja kolizji pocisków z obcymi oraz statkiem gracza została oddelegowana do osobnych, równolegle pracujących wątków, co optymalizuje wydajność gry.
* **Wyrażenia regularne (Regex):** Implementacja walidacji wprowadzanego pseudonimu użytkownika pod kątem niedozwolonych znaków.
* **Zarządzanie plikami:** Bezpieczny mechanizm zapisu danych chroniący integralność pliku z najlepszymi rekordami przed niepowołaną manipulacją zewnętrzną.

### Struktura Klas (Architektura Kodu):
1. `Obiekt` (Klasa bazowa): Definiuje wspólne właściwości obiektów sceny (pozycja x/y, tekstury, sprity, flagi aktywności).
2. `Gracz`, `Obcy`, `Pocisk` (Klasy pochodne): Reprezentują wyspecjalizowane obiekty gry korzystające z mechanizmów dziedziczenia i polimorfizmu.
3. `MenadzerObcych`: Odpowiada za logiczną strukturę dwuwymiarowego wektora przeciwników, ich płynne przesuwanie oraz sprawdzanie warunków końca poziomu.
4. `Menu`: Klasa zarządzająca stanami interfejsu (Pozycja, Tytuł, Graj, Logowanie, Rekordy, Wyjście).

---

## Specyfikacja zewnętrzna

### Uruchomienie programu
Aby rozpocząć, należy skompilować projekt z podlinkowaną biblioteką SFML i uruchomić główny plik wykonywalny. Na ekranie pojawi się interaktywne menu główne z opcjami: Graj, Logowanie, Rekordy oraz Wyjście.

### Sterowanie w grze:
* `Strzałka w lewo` – Ruch statkiem w lewą stronę ekranu.
* `Strzałka w prawo` – Ruch statkiem w prawą stronę ekranu.
* `Spacja` – Wystrzelenie pocisku.

---

## Zarządzanie pamięcią i Testowanie

Aplikacja przeszła kompleksowe testy jednostkowe, weryfikujące zachowanie metod, interakcje między dynamicznymi obiektami oraz responsywność interfejsu użytkownika pod kątem detekcji kolizji czy stabilności zapisu rekordów.

### Kluczowe usprawnienia i naprawione błędy:
* **Bezpieczeństwo pamięci:** Wylleniono wycieki pamięci poprzez całkowitą rezygnację z surowych wskaźników na rzecz nowoczesnych, inteligentnych wskaźników `std::unique_ptr`.
* **Optymalizacja kontenerów:** Obiekty zniszczone lub trafione są niemal natychmiast fizycznie usuwane z pamięci kontenerów (`std::vector`), zamiast polegać wyłącznie na sprawdzaniu flag logicznych, co drastycznie odciążyło pętlę gry.
* **Detekcja kolizji:** Zoptymalizowano matematyczny algorytm odpowiedzialny za sprawdzanie przecinania się granic obiektów (hitboxy), eliminując błędy niewykrywania trafień.
* **Relatywne renderowanie:** Naprawiono błędy wyświetlania grafiki poprzez wprowadzenie relatywnego systemu pozycjonowania elementów względem wymiarów okna.
