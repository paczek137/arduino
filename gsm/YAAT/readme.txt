More information concerning this tool you get from Frank Siegmund, 8257

Na gut :-)

Dies ist ein etwas erweitertes Terminalprogramm, mit dem man z.B. die
Statusleitungen 'nach drau�en' setzen kann wie man will. Au�erdem kann
man beliebug viele MDI-Fenster f�r verschiedene Serielle Ports aufmachen.
Man kann alles ankommene mitloggen oder auch zur�ck-senden (loop, auch
mit etwas verz�gerung). man kann adpcm-Frames, wie sie in DECT genutzt
werden, online dekodieren. ein MD32-Server kann en/decodiert werden. Und
es sieht sch�ner aus als HyperTerminal. Neuerdings kann man auch Befehle 
und rudiment�re Skripte ausf�hren. Ein Beispiel ist in der *.ini.ex-Datei gegeben,
die mu� immer so hei�en und im selben Verzeichnis wie das Programm liegen.

17.07.2009
----------
f�r AE (H.C. Schemmel): Pcap Schreiben: Erweiterung des WM eigenen Formats nachgezogen

15.07.2009
----------
-Terminalfenster
 Korrektur Verhalten bei UART-Error-Detection: unter _debug gibt es jetzt RestartOnError. 
 Damit wird ein DTR 1-0-1 Impuls (500ms low) generiert, wenn der Haken im _debug gesetzt
 ist und der Fehler EV_ERR kam. Vorher wurde der Restart immer ausgef�hrt. Jetzt
 kommt auch noch ein sch�ner Text im Terminal.


16.10.2008
----------
-Xon/Xoff korrigiert, jetzt wird wieder 0x11/0x13 verwendet
-Version ist jetzt 2.4.0.0
-siemens --> wireless modules
-sehr viele �nderungen der letzten 2 Jahre



20.12.2006
----------
-internes Handling bzgl. eines Lesethreads in der Uart-Komponente ge�ndert. Jetzt
 geht es auch mit den VSCOM-USB-Adaptern
-RX-Speed-Anzeige verbessert


21.06.2006
----------
-diverse kleinere �nderungen, die mir grad nicht mehr gel�ufig sind
-gsm03.38 character converter kann jetzt '_' richtig

02.02.2005
----------
-460800 Baud hinzugef�gt
-gsm03.38 erweitert/korrigiert
-serieller Tracer: Steuerleitungen jetzt richtige Anzeige 
  (mit der Kabelpeitschenbeschaltung ala Frontline!!!)


03.05.2004
----------
-Programm jetzt auf C++ Builder 6 Personal gehoben
- EditierZeile jetzt persistent in der *.ini-Datei  



31.03.2004
----------
-CopyPaste geht jetzt
-Editierzeile (EditLine) hinzugef�gt
-gsm 03.38 CharSet Decoder hinzugef�gt (more settings)



18.08.03
--------
-Scrollverhalten der Terminalfenster verbessert
-�ffnen der UART jetzt so, da� es auch nach einem SWUP noch gut geht


7.5.03
------
-diverse interne �nderungen, Fehlerbehebungen
-Elise Download: man mu� jetzt in die *.ini.ex-Datei einen Eintrag f�r den Download
 haben; ein Beispiel ist

 [UWDL]
 download_cmd="AT^SUDL=0000"

 Dies dient dazu, den PIN nicht hardkodieren zu m�ssen

4.10.01
-------
  -Offline Decoder f�r GSM Multiplexer 07.10 (TC/MC3x) integriert:
    Dieser Decoder erwartet z.Z. als EIngabeformat eine einzelne Datei, in der
    die Pakete von RX und TX gemeinsam gespeichert, jedoch nicht ineinander
    geschachtelt sind. D.h., die Bytes von jedem Paket sind vollst�ndig nacheinander
    in dieser Datei abgelegt.



