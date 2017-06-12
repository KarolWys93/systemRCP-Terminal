# systemRCP-Terminal
Projekt terminala RFID systemu rejestracji czasu pracy. Projekt wykonywany w ramach kursu zastosowań informatyki w gospodarce.
Program dla mikrokontrolera został napisany w języku C. W projekcie wykorzystano również narzędzie CubeMX.

Budowa terminala oparta jest na trzech głównych modułach:
* Moduł z mikrokontrolerem STM32F103C8Tx
* Moduł czytnika RFID oparty na układzie MF RC522
* Moduł sieciowy WiFi ESP8266

## Uruchomienie i konfiguracja
Układ powinien działać od razu po podłączeniu do zasilania, lecz pierwsze uruchomienie wymaga dokonania konfiguracji. Konfiguracja urządzenia dokonywana jest z użyciem UART z parametrami połączenia Baud:11520, brak pinów parzystości, 8 bitów danych, 1 bit stopu. (RX na pinie  PB11 oraz TX na pinie PB10). Po restarcie urządzenia na terminalu wyświetla się przez chwilę znak zachęty `>`. W tym czasie należy wprowadzić jedną z komend konfiguracyjnych:
* `config -a` - komenda ta przełącza układ w tryb komend AT dla modułu WiFi
* `config -w' - komenda ta służy do konfiguracji połączenia z AP WiFi. Najpierw urządzenie wyświetli aktualną nazwę sieci z którą jest połączone i zapyta, czy użytkownik chce zmienić konfigurację. Jeśli tak, w następnym kroku należy podać nazwę sieci WiFi, a następnie hasło do tej sieci. Urządzenie powinno się połączyć z AP i przejść do normalnego trybu pracy.
* `config -s` - przy pomocy tej komendy można skonfigurować adres serwera, z którym terminal powinien się łączyć po zeskanowaniu karty RFID. Adres powinien być w formacie adres:port. W adresie nie należy dodawać `http:\\`! Po prawidłowym podaniu adresu urządzenie powinno przejść do normalnego trybu pracy. Uwaga! Urządzenie w obecnej wersji oprogramowania wymaga konfiguracji adresu serwera po każdym restarcie. Jeżeli nie dokonamy konfiguracji, wykorzystane zostaną ustawienia domyślne!

Każda komenda konfiguracyjna wymaga podania hasła. Domyślne hasło to `12345678`.

## Działanie terminala i statusy LED
Aby zarejestrować swoje wejście lub wyjście należy przy pomocy odpowiedniego przycisku wybrać czy chcemy wejść czy wyjść oraz przyłożyć kartę RFID do czytnika. Po chwili urządzenie poinformuje nas pomomocy diody statusu, czy akcja została poprawnie zapisana.

Urządzenie komunikuje się z użytkownikiem przy pomocy trzech diod: diody zajętości, trybu pracy i statusu.
1) Dioda zajętości - Podczas przetwarzania żądania świeci kolorem czerwonym. Przy bezczynności jest zgaszona.
2) Dioda trybu pracy określa, czy następny odczyt karty RFID ma zostać zapisany jako wejście czy wyjście
* kolor zielony oznacza tryb wejścia
* kolor czerwony oznacza tryb wyjścia
3) Dioda statusu określa aktualny stan urządzenia i wynik żądania. Dioda wyświetla status po przyłożeniu karty RFID
* `Kolor zielony` - karta zaakceptowana, przyznano dostęp
* `Kolor czerwony` - brak karty w systemie lub karta nie przypisana do użytkownika
* `Dwa mrugnięcia kolorem niebieskim` - brak połączenia z AP WiFi
* `Dwa mrugnięcia kolorem żółtym` - błąd odpowiedzi serwera, zbyt długi czas na odpowiedź serwera
* `Dwa mrugnięcia na przemian kolorem czerwonym i niebieskim` - serwer jest nieosiągalny, błędny adres serwera, problem z serwerem DNS

## Pozostałe części systemu
* [Aplikacja serwerowa](https://github.com/kodkarolina/ZIG)
* [Desktopowa aplikacja kliencka](https://github.com/bebasak/Projekt-ZiG)
