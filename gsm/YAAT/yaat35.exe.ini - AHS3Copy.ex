[UWDL]
download_cmd=AT^SUDL=""


[CostumerButtons]
; Definition von einzelnen Befehlen; \r\n für das Modul nicht vergessen.
; Links vor dem '=' dürfen keine gleichen Bezeichner auftauchen (Windows-ini-File).

PIN=at+cpin=9999\r\n
MUX=at+cmux=0\r\n
AT=AT\r\n
BFB=AT^SQWE=1\r\n
TOGGLE_1s=TOGGLE1000
Inteface?=AT^SQPORT\r\n
IntefaceConfig?=AT^SDPORT?\r\n
IntefaceSupportedConfig?=AT^SDPORT=?\r\n
SCFG?=AT^SCFG?\r\n
SIM_2=AT^SCFG="Sim/CS","3"\r\n
SIM_1=AT^SCFG="Sim/CS","0"\r\n
DualOFF=AT^SCFG="Sim/DualMode","0"\r\n
DualON=AT^SCFG="Sim/DualMode","1"\r\n
cfun1,1=at+cfun=1,1\r\n
ata=ata\r\n
ath=ath\r\n
;chup=at+chup\r\n
ati=ati\r\n
at^siekret=at^siekret=1\r\n
;COPS_0=at+cops=0\r\n
PIN?=at+cpin?\r\n
;URCmdm=at^scfg="URC/DstIfc","mdm"\r\n
;URCapp=at^scfg="URC/DstIfc","app"\r\n
IMSI=AT+CIMI\r\n
;SDPORT?=at^sdport?\r\n
;at^moni=at^moni\r\n
;PIN2=at+cpin2=0000\r\n
;at+cimi=at+cimi\r\n
;at+cops_?=at+cops=?\r\n
at+cops?=at+cops?\r\n
CREG_1=at+creg=1\r\n
;at+creg?=at+creg?\r\n
CMEE_2=at+cmee=2\r\n
;at+clcc=at+clcc\r\n
;SMSO=AT^SMSO\r\n
;IMEI=AT+CGSN\r\n
Bootloader=at^sos=bootloader/info\r\n
at^cicret=at^cicret=swn\r\n
SetSINDsignal=AT^SIND="signal",1\r\n
AT^SIND="service",1
AT^SIND="sounder",1
AT^SIND="voiceprompt",1
AT^SIND="message",1
AT^SIND="call",1
AT^SIND="roam",1
AT^SIND="smsfull",1
AT^SIND="rssi4",1
AT^SIND="audio",1
AT^SIND="simstatus",1
AT^SIND="simdata",1
AT^SIND="eons",1
AT^SIND="nitz",1
AT^SIND="psinfo",1
AT^SIND="vcall",1
AT^SIND="simlocal",1
AT^SIND="lsta",1
AT^SIND="ratmode",1
AT^SIND="pacsp",1
AT^SIND="steerroam",1
AT^SIND="ecallda",1
AT^SIND="ecallco",1
AT^SIND="ecaller",1
AT^SIND="pagingcoor",1
AT^SIND="imsi",1
AT^SIND="iccid",1
AT^SIND="ceer",1
AT^SIND="simread",1


; Definition eines Befehlssatzes. Man kann mehrer Befehle ausführen und auf Antworten
; warten. Links stehen die Bezeichner für den Befehlssatz, rechts steht die Sektion, unter
; der der Satz abgelegt wird.
; Links vor dem '=' dürfen keine gleichen Bezeichner auftauchen (Windows-ini-File).


D1GPRS=D1_GPRS
E+GPRS=E+_GPRS
AUTOBTEST=AUTOB

; Definition von Befehlssätzen. Syntax ist eigentlich selbsterklärend. Etwas irritierend
; sind nur die aufeinanderfolgenden Zahlen, die am Anfang jeder Zeile auftauchen. 
; Sie sorgen eigentlich nur dafür, daß ein Bezeichner nicht zweimal auftaucht, ich hatte keine
; Lust, mir da was spezielles zu programmieren ... Ansonsten:
;
;  UART_SEND = gesendeter Text wie oben beschrieben, \r\n beim Modul nicht vergessen
;
;  UART_SENDHEX = 0x00 0x01 255 etc. etc. Man kann mit Leerzeichen getrennt sehr viele Zeichen senden
;
;  UART_TIMEOUT = xx setzt das Timeout für den WAITFOR-Befehl in s  !!! Sekunden !!!
;
;  UART_WAITTIME = xxx ms !!! Milllisekunden !!! 
;
;  UART_WAITFOR = yyy wartet auf eine Reaktion yyy , z.B. für AT+CPIN 
;                 Parameter UART_ABORT_ON xxx bricht beim Empfang von xxx sofort ab
;                 ansonsten wir die Zeit, die mit UART_TIMEOUT gesetzt wird, gewartet
;
;  UART_BAUDRATE = xxxxx stellt die Baudrate DES TERMINALS um, nicht des Muduls, dass
                         muss mit dem at+ipr-Befehl davor gemacht werden
;
;  UART_COMPORT  = COMx stellt COMx vom Terminal ein
;
;  UART_TOGGLE_DTR = 1|0 setzt DTR auf den booleschen Wert, der übergeben wird
;
;  UART_TOGGLE_RTS = 1|0 setzt RTS auf den booleschen Wert, der übergeben wird
;
;  UART_GOTO = Zeilennummer, wobei die Zählung mit 0 beginnt
;
;  UART_UPLOAD = FileName
;

; Und hier ein paar Beispiele:



[D1_GPRS]
1UART_SEND       =AT+CPIN=9999\r\n
2UART_TIMEOUT    =20
3UART_WAITFOR    =OK       UART_ABORT_ON ERROR
4UART_SEND       =AT+cgdcont=1,"IP","internet.t-d1.de"\r\n
5UART_WAITFOR    =OK
6UART_SEND       =AT+cgqreq=1,3,4,3,0,0\r\n
7UART_WAITFOR    =OK

[E+_GPRS]
1UART_SEND       =AT+CPIN=9999\r\n
2UART_TIMEOUT    =20
3UART_WAITFOR    =OK
4UART_SEND       =AT+CGDCONT=1,"IP","internet.eplus.de"\r\n
5UART_WAITFOR    =OK
6UART_SEND       =AT+cgqreq=1,2,4,3,9,31\r\n
7UART_WAITFOR    =OK


[AUTOB]
1UART_SEND       =AT+IPR=0\r\n
2UART_TIMEOUT    =2
3UART_WAITFOR    =OK
4UART_WAITTIME   =100
5UART_SEND       =ATI\r\n
6UART_WAITFOR    =OK




[TOGGLE1000]
UART_TOGGLE_RTS   = 0
UART_WAITTIME     = 1000
UART_TOGGLE_RTS   = 1
UART_WAITTIME     = 1000
UART_GOTO         = 0












