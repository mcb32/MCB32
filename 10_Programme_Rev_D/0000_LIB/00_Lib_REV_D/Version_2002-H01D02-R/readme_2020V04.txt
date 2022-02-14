Version: 11.01.2020: TimerInitHr NEU, Kor. bei 	TimerInit	Malacarne, dMCB32_LibVersion = 2002-H01D02-R  // Auflösung 10us ab 20us
Version: 19.9.2017:  Bugfix Clearscreen, 			Malacarne, dMCB32_LibVersion = 1738-H01D02-R
Version: 14.11.2014: Neue Lib veröffentlicht,			Malacarne, dMCB32_LibVersion = 1450-H01D02-R

**** 2002-H01D02-R *****************************************************************************************
*   void TimerInitHr(char Nr, int Time10us, char IRQPrio);
*   Nr:	2..5			Einer von 4 Timern oder Counter  
*   Time10us: 	2..65535	Zeit in 10us, muss >=2 sein!   t = 20us .. 650ms in 10us Incr

// Version kann mit Zugriff auf dMCB32_LibVersion  gelesen werden:
#define dMCB32_LibVersion "2002-H01D02-R"  // TimerInitHr()  HR für Higheresolution

// TimerInit wurde korrigiert so dass Werte von 1*100us auf 2*100us gesetzt werden. Zudem wir das
   ARR Register nun richtig geladen (Time100us-1 anstelle alt Time100us): 
   Beispiel: Time100us = 4 wird als Wert 3 ins ARR Register geladen. 
             Der Zähler zählt aber 0,1,2,3= 4 Werte  = 4*100us

****************************************************************************** 2002-H01D02-R ***************

