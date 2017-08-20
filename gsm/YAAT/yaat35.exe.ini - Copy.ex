[UWDL]
download_cmd=AT^SUDL=""


[CostumerButtons]
; Definition von einzelnen Befehlen; \r\n für das Modul nicht vergessen.
; Links vor dem '=' dürfen keine gleichen Bezeichner auftauchen (Windows-ini-File).

RESET=at+cfun=1,1\r\n
PIN=at+cpin=9999\r\n
CHUP=AT+CHUP\r\n
CICRET(swn)=AT^CICRET="swn"\r\n
MUX=at+cmux=0\r\n
AT=AT\r\n
BFB=AT^SQWE=1\r\n
TOGGLE_1s=TOGGLE1000
Inteface?=AT^SQPORT\r\n
IntefaceConfig?=AT^SSRVSET?\r\n
IntefaceSupportedConfig?=AT^SSRVSET=?\r\n
SCFG?=AT^SCFG?\r\n
SIM_2=AT^SCFG="Sim/CS","3"\r\n
SIM_1=AT^SCFG="Sim/CS","0"\r\n
DualOFF=AT^SCFG="Sim/DualMode","0"\r\n
DualON=AT^SCFG="Sim/DualMode","1"\r\n
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
E5=AT^SNFG=1000,43,659\r\n
D#=AT^SNFG=1000,43,622\r\n
C#=AT^SNFG=1000,43,554\r\n
B=AT^SNFG=1000,43,493\r\n
A=AT^SNFG=1000,43,440\r\n
G#=AT^SNFG=1000,43,415\r\n
F#=AT^SNFG=1000,43,370\r\n
E4=AT^SNFG=1000,43,329\r\n
Melody=melody_01
LedConfig=ledconfig
LedShow=ledshow
TCPListenerConfig=TCPListenerConf
SMTPConfig=emailConfig
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

;E5=AT^SNFG=1000,43,659\r\n
;D#=AT^SNFG=1000,43,622\r\n
;C#=AT^SNFG=1000,43,554\r\n
;B=AT^SNFG=1000,43,493\r\n
;A=AT^SNFG=1000,43,440\r\n
;G#=AT^SNFG=1000,43,415\r\n
;F#=AT^SNFG=1000,43,370\r\n
;E4=AT^SNFG=1000,43,329\r\n

[melody_01]
UART_SEND=AT^SNFG=700,43,622\r\n
UART_WAITTIME     = 750
UART_SEND=AT^SNFG=300,43,622\r\n
UART_WAITTIME     = 300
UART_SEND=AT^SNFG=1000,43,659\r\n
UART_WAITTIME     = 1000
UART_SEND=AT^SNFG=500,43,622\r\n
UART_WAITTIME     = 500
UART_SEND=AT^SNFG=500,43,659\r\n
UART_WAITTIME     = 500
UART_SEND=AT^SNFG=500,43,622\r\n
UART_WAITTIME     = 500
C#=AT^SNFG=500,43,554\r\n
UART_WAITTIME     = 500
UART_SEND=AT^SNFG=500,43,493\r\n
UART_WAITTIME     = 500
UART_SEND=AT^SNFG=1000,43,554\r\n
UART_WAITTIME     = 1000
UART_SEND=AT^SNFG=500,43,493\r\n
UART_WAITTIME     = 500
UART_SEND=at^snfo=43,0,0\r\n
UART_SEND=AT^SNFG=1000,43,415\r\n
UART_WAITTIME     = 1000

[ledshow]
UART_SEND=at^ssio=0,0\r\n
UART_WAITTIME     = 750
UART_SEND=at^ssio=0,1\r\n
UART_WAITTIME     = 100
UART_SEND=at^ssio=1,0\r\n
UART_WAITTIME     = 750
UART_SEND=at^ssio=1,1\r\n
UART_WAITTIME     = 100
UART_SEND=at^ssio=2,0\r\n
UART_WAITTIME     = 750
UART_SEND=at^ssio=2,1\r\n
UART_WAITTIME     = 100
UART_SEND=at^ssio=3,0\r\n
UART_WAITTIME     = 750
UART_SEND=at^ssio=3,1\r\n
UART_WAITTIME     = 100
UART_SEND=at^ssio=4,0\r\n
UART_WAITTIME     = 750
UART_SEND=at^ssio=4,1\r\n
UART_WAITTIME     = 100
UART_SEND=at^ssio=5,0\r\n
UART_WAITTIME     = 750
UART_SEND=at^ssio=5,1\r\n
UART_WAITTIME     = 100
UART_SEND=at^ssio=6,0\r\n
UART_WAITTIME     = 750
UART_SEND=at^ssio=6,1\r\n
UART_WAITTIME     = 100
UART_SEND=at^ssio=7,0\r\n
UART_WAITTIME     = 750
UART_SEND=at^ssio=7,1\r\n
UART_WAITTIME     = 100
UART_SEND=at^ssio=8,0\r\n
UART_WAITTIME     = 750
UART_SEND=at^ssio=8,1\r\n
UART_WAITTIME     = 100
UART_SEND=at^ssio=9,0\r\n
UART_WAITTIME     = 750
UART_SEND=at^ssio=9,1\r\n
UART_WAITTIME     = 100
UART_GOTO         = 0


[ledconfig]
UART_TIMEOUT    =1
UART_SEND=at^spio=1\r\n
UART_WAITTIME     = 750
UART_SEND=at^scpin=1,0,1,1\r\n
UART_WAITFOR    =OK
UART_SEND=at^scpin=1,1,1,1\r\n
UART_WAITFOR    =OK
UART_SEND=at^scpin=1,2,1,1\r\n
UART_WAITFOR    =OK
UART_SEND=at^scpin=1,4,1,1\r\n
UART_WAITFOR    =OK
UART_SEND=at^scpin=1,5,1,1\r\n
UART_WAITFOR    =OK
UART_SEND=at^scpin=1,6,1,1\r\n
UART_WAITFOR    =OK
UART_SEND=at^scpin=1,7,1,1\r\n
UART_WAITFOR    =OK
UART_SEND=at^scpin=1,8,1,1\r\n
UART_WAITFOR    =OK
UART_SEND=at^scpin=1,9,1,1\r\n
UART_WAITFOR    =OK


[TCPListenerConf]
at^siss=0,"srvType","Socket"
UART_WAITTIME     = 750


[emailConfig]
1UART_SEND       =AT^SISS=9,"srvType","Smtp"\r\n
2UART_TIMEOUT    =2
3UART_WAITFOR    =OK
4UART_SEND       =AT^SISS=9,"conId","1"\r\n
5UART_TIMEOUT    =2
6UART_WAITFOR    =OK
10UART_SEND      =AT^SISS=9,"alphabet",1\r\n
11UART_TIMEOUT   =2
12UART_WAITFOR   =OK
7UART_SEND       =AT^SISS=9,"address","smtp://poczta.interia.pl"\r\n
8UART_TIMEOUT    =2
9UART_WAITFOR    =OK
16UART_SEND      =AT^SISS=9,"user","testowy2.pop3smtp@interia.pl"\r\n
17UART_TIMEOUT   =2
18UART_WAITFOR   =OK
31UART_SEND      =AT^SISS=9,"smFrom","testowy2.pop3smtp@interia.pl"\r\n
32UART_TIMEOUT   =2
33UART_WAITFOR   =OK
34UART_SEND      =AT^SISS=9,"smRcpt","mateusz.rudzki@globallogic.com"\r\n
35UART_TIMEOUT   =2
36UART_WAITFOR   =OK
37UART_SEND      =AT^SISS=9,"smSubj","SMTP test "\r\n
38UART_TIMEOUT   =2
39UART_WAITFOR   =OK
40UART_SEND      =AT^SISS=9,"smAuth","1"\r\n
41UART_TIMEOUT   =2
42UART_WAITFOR   =OK
46UART_SEND      =AT^SISS=9,"passwd","testowy2pop"\r\n
47UART_TIMEOUT   =2
48UART_WAITFOR   =OK
43UART_SEND      =AT^SISS=9,"smHdr"," my test"\r\n
44UART_TIMEOUT   =2
45UART_WAITFOR   =OK



[FTPConfig]
AT^SISS=2,"srvType","Ftp"
AT^SISS=2,conId,"1"