# PIC16F877A Akilli Fan / Motor Kontrol Odevi

Bu depo, Mikroislemciler ve Mikrodenetleyiciler dersi kapsaminda verilen
"PIC16F877A ile Akilli Fan / Motor Kontrol Sistemi" odevi icin hazirlanmis
baslangic sablonudur.

Odevde, PIC16F877A mikrodenetleyicisi kullanilarak ADC, PWM, LCD ve
DC motor kontrolu birlikte uygulanacaktir. Sistem SimulIDE ortaminda
simule edilecek ve teknik bir rapor ile belgelenmis olacaktir.

## Klasor Yapisi

- firmware/
  Mikrodenetleyici yazilimi (MPLAB X + XC8 kaynak kodlari)

- simulasyon/
  SimulIDE proje dosyasi ve devre simulasyonu

- rapor/
  Odev raporu (PDF)

## Odev Ozeti

- Analog giris: AN0 (RA0)
- PWM cikis: CCP1 (RC2)
- Motor hizi, ADC degerine bagli olarak kontrol edilecektir.
- 16x2 LCD uzerinde ADC, voltaj ve PWM bilgileri gosterilecektir.

## Teslim

Son teslim tarihi: 25 Ocak Pazar, 23:59

Odev, tum klasorler tek bir dizin altinda toplanarak ziplenmeli ve
e-posta ile gonderilmelidir. Dosya ve klasor adlarinda Turkce karakter
kullanilmamalidir.

## Notlar

Bu depo bir sablon olup, ogrenciler odeve bu yapi uzerinden baslamak
zorundadir. Klasor yapisi degistirilmemelidir.
