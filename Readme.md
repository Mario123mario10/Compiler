Rozwiązanie: Lekser.sln (Visual Studio 2019)
Składa się z 2 projektów:

1) Lekser - Prosty test użytkownika leksera
2) Lekser_test - Testowanie

Aby wybrać projekt, należy kliknąć na niego PPM i wybrać "Ustaw jako projekt startowy".
W ustawieniach obu projektów należy ustawić standard C++ 17 (domyślny w VS 2019 to zazwyczaj C++ 14, przez co bez tej zmiany kompilator będzie zgłaszał błędy). Przy testowaniu testów jednostkowych należy w ustawieniach dla projektu lekser wybrac typ konfiguracji biblioteka statyczna (.lib) zamiast domyślnego aplikacja (.exe). Aby uruchomić LekserBasicTest należy z powrotem zmienić typ konfiguracji na aplikację (.exe).

Testowanie za pomocą testów jednostkowych korzysta z Google Test.