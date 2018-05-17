Celem projektu była implementacja funkcjonalności rozpoznawania kodów QR w obrazie rejestrowanym przez podłączone kamery (max. 4 z wykorzystaniem huba dla CSI, domyślnie RPi pozwala na podłączenie 1 kamery).

Rozpoznawane są również spersonalizowane QR, zablokowane kody paskowe (możliwość odblokowania).

Realizacja c++.
Projekt wykorzystuje:
- QtFramework - interfejs graficzny
- OpenCV3 - przetwarzanie obrazu
- Zbar - dekodowanie
- Raspicam ( https://sourceforge.net/projects/raspicam/ ) - wydajna obsługa kamer
- WiringPi - obsługa GPIO dla multipleksacji danych z kamer

Po testach wydajności zrezygnowano z większej liczby wątków oraz dzielenia obrazu.
