//----------------------------------------------------------
// Tetris.c   Schüler @TBZ Jahr 2015 ... 2021
// Testet Grafik und Peripherie aus der Library TouchSys.lib
//----------------------------------------------------------
#include <stm32f10x.h>				// Mikrocontrollertyp
//#include "TouchSys.h"						// Grafik,Peripherie ohne P0P1   
#include "TouchP0P1.h"
#include "String.h"
#include "stdio.h"
#define Timer if(time--==0)time=reloader, timer=1; else timer=0
#define Up 		PD_12
#define Down 	PD_14
#define Right PD_11
#define Left 	PD_13
#define Push 	PD_15
#define Menue 0
#define Start 1
#define Optionen 2
#define Ranking 3

int timer=0, time=50, farbe=0, reloader=50;
char Status=6, dR=0, dU=0, dL=0, dD=0, dP=0, teil=1, teilD=0, Drehen=1, Linie=0, Loeschen=0, Loesch=0; 
int x=0, y=0, x1=0, y1=0, x2=0, y2=0, x3=0, y3=0, x4=0, y4=0, ysave=0;
int Speicher[21][13] = {0}, Wert=0;
char buf[10]={0}, Loeschges=10, einmal=0, Level=1, lvl[10], i=0, Tausch=0, Out=0, teilS=0, Spiel=0;
char AuswahlMenu=0, MenuFarbe=0, Settings=1, Tetris=0; 
int MenuColor=0x076A, light=160, entRG=0, entGB=0, OwnColor=0, Points=0, point=1, punkte=0;
char RGB=0, R=0, RG=0, GB=0, B=0, baba=0, rank[6], pause=0;
long entR=0;

int SX[]= {15, 25, 35, 45};
int SY[]= {40, 50, 60, 70};
int Y[] = {50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200, 210, 220, 230, 240};
int X[] = {60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170};

void	Teil(char teilD, char Drehen);
//----------------------------------------------------------
int main(void) 								// Hauptprogramm Grafiktest
{ 
	InitTouchScreen();
	
	printAt(1, "  Tetris  DEMO    ");
	rectan(10,10,230,35,2,0x076A,0); 		//OBERE LEISTE
	
	while(1)															// Endlosschlaufe
	{
		switch(Spiel)
		{
			case Menue: printAt(5, "           Start!");
									printAt(7, "         Menu-Color");
									printAt(9, "          Ranking");
									switch (AuswahlMenu)
									{
										case 0: line(70,129,152,129,2,BLACK);
														line(85,97,135,97,2,MenuColor);
														if(Down==0 && dD==0) dD=1, AuswahlMenu=1;
														if(Push==0 && dP==0) rectan(60,50,180,250,0,BLACK,1), Spiel=Start, Status=6;
										break;
										case 1: line(70,129,152,129,2,MenuColor);
														line(75,97,145,97,2,BLACK);
														line(78,161,136,161,2,BLACK);
														if(Down==0 && dD==0) dD=1, AuswahlMenu=2;
														if(Up==0 && dU==0) dU=1, AuswahlMenu=0;
														if(Push==0 && dP==0) rectan(60,50,180,250,0,BLACK,1), Spiel=Optionen;
										break;
										case 2: line(70,129,152,129,2,BLACK);
														line(78,161,136,161,2,MenuColor);
														if(Up==0 && dU==0) dU=1, AuswahlMenu=1;
														if(Push==0 && dP==0) rectan(60,50,180,250,0,BLACK,1), Spiel=Ranking;
										break;
									}
									if(Down==1)dD=0;
									if(Up==1)dU=0;
									if(Push==1)dP=0;
			break;
			case Start: switch(Level)
			{
				case 1: if (lvl[1]==0){ lvl[1]=1; i=1; point=1; reloader=26; printAt(7, "LVL: 1");} break;
				case 2: if (lvl[2]==0){ lvl[2]=1; i=2; point=2; reloader=22; printAt(7, "LVL: 2");} break;
				case 3: if (lvl[3]==0){ lvl[3]=1; i=3; point=3; reloader=18; printAt(7, "LVL: 3");} break;
				case 4: if (lvl[4]==0){ lvl[4]=1; i=4; point=4; reloader=12; printAt(7, "LVL: 4");} break;
				case 5: if (lvl[5]==0){ lvl[5]=1; i=5; point=5; reloader=5; printAt(7, "LVL: 5");} break;
			}
				switch(Status)
				{
			
			case 1: switch(teil)     //erstes Teil auswählen und auf anfangsposition
							{
								case 1: farbe=LIGHT_CYAN; 
												x1=5;			y1=0;
												x2=x1+2; 	x3=x1-1; 	x4=x1+1;
												y2=y1; 		y3=y1; 		y4=y1;
												teilD=1;
												
								break;
								case 2: farbe=BRIGHT_BLUE; 
												x1=5;			y1=1;
												x2=x1+1;	x3=x1-1;	x4=x1-1;
												y2=y1; 		y3=y1-1; 	y4=y1;
												teilD=2;
								break;
								case 3: farbe=0xFE20; 	
												x1=5;			y1=1;
												x2=x1+1; 	x3=x1+1;	x4=x1-1;	
												y2=y1; 		y3=y1-1;	y4=y1;
												teilD=3;
								break; 	
								case 4: farbe=BRIGHT_RED;	
												x1=5;			y1=0;
												x2=x1+1;	x3=x1-1;	x4=x1;	
												y2=y1+1; 	y3=y1; 		y4=y1+1;
												teilD=4;
								break;
								case 5: farbe=BRIGHT_GREEN; 
												x1=5;			y1=0;
												x2=x1+1;	x3=x1-1;	x4=x1;	
												y2=y1;	 	y3=y1+1; 	y4=y1+1;
												teilD=5;
								break;
								case 6: farbe=LIGHT_MAGENTA; 
												x1=5;			y1=1;
												x2=x1+1;	x3=x1;		x4=x1-1;	
												y2=y1;		y3=y1-1;	y4=y1;
												teilD=6;
								break;
								case 7: farbe=BRIGHT_YELLOW; 
												x1=5;			y1=0;
												x2=x1+1; 	x3=x1; 		x4=x1+1;	
												y2=y1;		y3=y1+1;	y4=y1+1;
												teilD=7;
								break;
							}
							Status=2;
			break;
			case 2: 	rectan(X[x1],Y[y1],X[x1]+9,Y[y1]+9,0,farbe,1); //Teil immer eins nach unten, links + rechts, PUSH->Teil Save
								rectan(X[x2],Y[y2],X[x2]+9,Y[y2]+9,0,farbe,1);
								rectan(X[x3],Y[y3],X[x3]+9,Y[y3]+9,0,farbe,1);
								rectan(X[x4],Y[y4],X[x4]+9,Y[y4]+9,0,farbe,1);
								if(timer==1)
								{
									if(y4>=18) Status=4;
									rectan(X[x1],Y[y1],X[x1]+9,Y[y1]+9,0,BLACK,1);
									rectan(X[x2],Y[y2],X[x2]+9,Y[y2]+9,0,BLACK,1);
									rectan(X[x3],Y[y3],X[x3]+9,Y[y3]+9,0,BLACK,1);
									rectan(X[x4],Y[y4],X[x4]+9,Y[y4]+9,0,BLACK,1);
									y1=y1+1; y2=y2+1; y3=y3+1; y4=y4+1;
									if(Speicher[y1+1][x1]!=0||Speicher[y2+1][x2]!=0||Speicher[y3+1][x3]!=0||Speicher[y4+1][x4]!=0) Status=4;
									Teil(teilD, Drehen);
								}
								Timer;
							if(Right==0 && dR==0)
							{
								dR=1;
								if(x2<11 && (Speicher[y3][x3-1]==0&&Speicher[y1][x1-1]==0&&Speicher[y4][x4-1]==0&&Speicher[y2][x2-1]==0))
								{
									rectan(X[x1],Y[y1],X[x1]+9,Y[y1]+9,0,BLACK,1);
									rectan(X[x2],Y[y2],X[x2]+9,Y[y2]+9,0,BLACK,1);
									rectan(X[x3],Y[y3],X[x3]+9,Y[y3]+9,0,BLACK,1);
									rectan(X[x4],Y[y4],X[x4]+9,Y[y4]+9,0,BLACK,1);
									x1=x1+1;  x2=x2+1;  x3=x3+1;  x4=x4+1;
									if(Speicher[y1+1][x1]!=0||Speicher[y2+1][x2]!=0||Speicher[y3+1][x3]!=0||Speicher[y4+1][x4]!=0) Status=4;
								}
							}
							if(Left==0 && dL==0)
							{ 
								dL=1;
								if(x1>0 && x4>0 && x3>0 && (Speicher[y3][x3-1]==0&&Speicher[y1][x1-1]==0&&Speicher[y4][x4-1]==0&&Speicher[y2][x2-1]==0))
								{
									rectan(X[x1],Y[y1],X[x1]+9,Y[y1]+9,0,BLACK,1);
									rectan(X[x2],Y[y2],X[x2]+9,Y[y2]+9,0,BLACK,1);
									rectan(X[x3],Y[y3],X[x3]+9,Y[y3]+9,0,BLACK,1);
									rectan(X[x4],Y[y4],X[x4]+9,Y[y4]+9,0,BLACK,1);
									x1=x1-1;	x2=x2-1;	x3=x3-1;  x4=x4-1;
									if(Speicher[y1+1][x1]!=0||Speicher[y2+1][x2]!=0||Speicher[y3+1][x3]!=0||Speicher[y4+1][x4]==1) Status=4;
								}
							}
							if(Up==0&&dU==0)
							{
								dU=1, Drehen++; 
								if(Drehen==5) Drehen=1;
								Teil(teilD, Drehen);
							}
							if(Push==0 && dP==0 && Tausch==0)
							{
								dP=1; Tausch=1;
								Status=5;
							}
							if(Down==0) reloader=1; else lvl[i]=0;
							if(Right==1)dR=0;
							if(Left==1)dL=0;	
							if(Up==1) dU=0;
							if(Push==1) dP=0;
			break;
			case 3:	Wert=sprintf(buf, "  %u  ", Loeschges); //Statistik, Gewonnen, Verloren, PUSH->Neustart
							printAt(10, buf);
							if(Loesch==0)punkte=0;
							if(Loesch==1)
							{
								Points=Points+15+punkte;
								punkte++;
							}
							if(Loesch==2)
							{
								Points=Points+40+punkte;
								punkte++;
							}
							if(Loesch==3)
							{
								Points=Points+75+punkte;
								punkte++;
							}
							if(Loesch==4)
							{
								Tetris++;
								Points=Points+100;
								if(Tetris>0) Points=Points+100;
								printAt(6, "Tetris!");
							}else Tetris=0;
							for(y=0;y<20;y++)
							{
								for(x=0;x<12;x++)
								{
									if(Speicher[y][x]!=1) baba++;
								}
							}
							if(baba==0) Points=Points+500;
							baba=0;
							Loesch=0;
							teil++;
							if(teil==8) teil=1;
							Wert=sprintf(buf,"  %u", Points);
							printAt(13, buf);
							if(y4==2)
							{
								if(einmal==0)
								{
									einmal=1;
									printAt(6, "           GAME OVER");
									rectan(58,36,182,251,2,MenuColor,0);
									for(y=0; y<21; y++)
									{
										for(x=0; x<13; x++) Speicher[y][x]=0;
									}
									if(Points>rank[1])
									{
										rank[5]=rank[4];
										rank[4]=rank[3];
										rank[3]=rank[2];
										rank[2]=rank[1];
										rank[1]=Points;
									}else if(Points>rank[2])
									{
										rank[5]=rank[4];
										rank[4]=rank[3];
										rank[3]=rank[2];
										rank[2]=Points;
									}else if(Points>rank[3])
									{
										rank[5]=rank[4];
										rank[4]=rank[3];
										rank[3]=Points;
									}else if(Points>rank[4])
									{
										rank[5]=rank[4];
										rank[4]=Points;
									}else if(Points>rank[4])
									{
										rank[5]=Points;
									}
								}
							}else if(Level==6)
							{
								if(einmal==0)
								{
									einmal=1;
									printAt(6, "           GEWONNEN!");
									rectan(58,36,182,251,2,MenuColor,0);
									for(y=0; y<21; y++)
									{
										for(x=0; x<13; x++) Speicher[y][x]=0;
									}
								}
							}else Status=1, Drehen=1;

							if(Push==0)
							{
								einmal=0;
								for(i=0;i<5;i++)
								{
									lvl[i]=0;
								}
								Out=0;
								Level=1;
								Loeschges=10;
								Loesch=0;
								Loeschen=0;
								Status=1;
								rectan(0,37,240,320,1,BLACK,1);
								Spiel=Menue;
							}
			break;
			case 4: rectan(X[x1],Y[y1],X[x1]+9,Y[y1]+9,0,farbe,1); //Wenn das Teil unten ist
							rectan(X[x2],Y[y2],X[x2]+9,Y[y2]+9,0,farbe,1);
							rectan(X[x3],Y[y3],X[x3]+9,Y[y3]+9,0,farbe,1);
							rectan(X[x4],Y[y4],X[x4]+9,Y[y4]+9,0,farbe,1);
							Speicher[y1][x1]=farbe; 
							Speicher[y2][x2]=farbe; 
							Speicher[y3][x3]=farbe;
							Speicher[y4][x4]=farbe;
							Points=Points+point;
							Tausch=0;
							for(y=0; y<21; y++)
							{
								for(x=0; x<12; x++)
								{
									if(Speicher[y][x]!=0) Linie++;
								}
								if(Linie==12)
								{
									rectan(60,Y[y],180,Y[y]+9,0,BLACK,1);
									for(x=0; x<12; x++) Speicher[y][x]=0;
									Loeschen=Loeschen+10; Loeschges--; Loesch++;
									if(Loeschges==0)
									{
										Loeschges=10;
										Level++;
										if (Level==6) Status=3;
									}
								}
								Linie=0;
								if(Loeschen>0)
								{
									ysave=y;
									for(y=20; y>0; y--)
									{
										for(x=12; x>=0; x--)
										{
											if(Speicher[y][x]!=0 && Speicher[y+1][x]==0 && y+1!=20)
											{
												rectan(X[x],Y[y],X[x]+9,Y[y]+9,0,BLACK,1);
												rectan(X[x],Y[y+1],X[x]+9,Y[y+1]+9,0,Speicher[y][x],1);
												Speicher[y+1][x]=Speicher[y][x];
												Speicher[y][x]=0;
											}
										}
									}
									y=ysave;
									Loeschen=0;
								}
							}
							Status=3;
			break;
			case 5: rectan(X[x1],Y[y1],X[x1]+9,Y[y1]+9,0,BLACK,1); //Savemodus
							rectan(X[x2],Y[y2],X[x2]+9,Y[y2]+9,0,BLACK,1);
							rectan(X[x3],Y[y3],X[x3]+9,Y[y3]+9,0,BLACK,1);
							rectan(X[x4],Y[y4],X[x4]+9,Y[y4]+9,0,BLACK,1);
							Speicher[y1][x1]=0; 
							Speicher[y2][x2]=0; 
							Speicher[y3][x3]=0;
							Speicher[y4][x4]=0;
							if(Out==0)
							{
								Out=8;
								teil++;
								if(teil==8) teil=1;
								Status=1;
							}else
							{
								Out=teilS;
								rectan(SX[0],SY[0],SX[3]+9,SY[3]+9,0,BLACK,1);
							}
							switch(teilD)
							{
								case 1: farbe=LIGHT_CYAN; 
												x1=1;			y1=1;
												x2=x1+2; 	x3=x1-1; 	x4=x1+1;
												y2=y1; 		y3=y1; 		y4=y1;
												teilS=1;
								break;
								case 2: farbe=BRIGHT_BLUE; 
												x1=1;			y1=2;
												x2=x1+1;	x3=x1-1;	x4=x1-1;
												y2=y1; 		y3=y1-1; 	y4=y1;
												teilS=2;
								break;
								case 3: farbe=LIGHT_RED; 	
												x1=1;			y1=2;
												x2=x1+1; 	x3=x1+1;	x4=x1-1;	
												y2=y1; 		y3=y1-1;	y4=y1;
												teilS=3;
								break;
								case 4: farbe=BRIGHT_RED;	
												x1=1;			y1=1;
												x2=x1+1;	x3=x1-1;	x4=x1;	
												y2=y1+1; 	y3=y1; 		y4=y1+1;
												teilS=4;
								break;
								case 5: farbe=BRIGHT_GREEN; 
												x1=1;			y1=1;
												x2=x1+1;	x3=x1-1;	x4=x1;	
												y2=y1;	 	y3=y1+1; 	y4=y1+1;
												teilS=5;
								break;
								case 6: farbe=LIGHT_MAGENTA; 
												x1=1;			y1=2;
												x2=x1+1;	x3=x1;		x4=x1-1;	
												y2=y1;		y3=y1-1;	y4=y1;
												teilS=6;
								break;
								case 7: farbe=BRIGHT_YELLOW; 
												x1=1;			y1=1;
												x2=x1+1; 	x3=x1; 		x4=x1+1;	
												y2=y1;		y3=y1+1;	y4=y1+1;
												teilS=7;
								break;
							}
							rectan(SX[x1],SY[y1],SX[x1]+9,SY[y1]+9,0,farbe,1);
							rectan(SX[x2],SY[y2],SX[x2]+9,SY[y2]+9,0,farbe,1);
							rectan(SX[x3],SY[y3],SX[x3]+9,SY[y3]+9,0,farbe,1);
							rectan(SX[x4],SY[y4],SX[x4]+9,SY[y4]+9,0,farbe,1);
							teilD=Out;
							switch(teilD)
							{
								case 1: farbe=LIGHT_CYAN; 
												x1=5;			y1=0;
												x2=x1+2; 	x3=x1-1; 	x4=x1+1;
												y2=y1; 		y3=y1; 		y4=y1;
												teilD=1;
												
								break;
								case 2: farbe=BRIGHT_BLUE; 
												x1=5;			y1=1;
												x2=x1+1;	x3=x1-1;	x4=x1-1;
												y2=y1; 		y3=y1-1; 	y4=y1;
												teilD=2;
								break;
								case 3: farbe=LIGHT_RED; 	
												x1=5;			y1=1;
												x2=x1+1; 	x3=x1+1;	x4=x1-1;	
												y2=y1; 		y3=y1-1;	y4=y1;
												teilD=3;
								break; 	
								case 4: farbe=BRIGHT_RED;	
												x1=5;			y1=0;
												x2=x1+1;	x3=x1-1;	x4=x1;	
												y2=y1+1; 	y3=y1; 		y4=y1+1;
												teilD=4;
								break;
								case 5: farbe=BRIGHT_GREEN; 
												x1=5;			y1=0;
												x2=x1+1;	x3=x1-1;	x4=x1;	
												y2=y1;	 	y3=y1+1; 	y4=y1+1;
												teilD=5;
								break;
								case 6: farbe=LIGHT_MAGENTA; 
												x1=5;			y1=1;
												x2=x1+1;	x3=x1;		x4=x1-1;	
												y2=y1;		y3=y1-1;	y4=y1;
												teilD=6;
								break;
								case 7: farbe=BRIGHT_YELLOW; 
												x1=5;			y1=0;
												x2=x1+1; 	x3=x1; 		x4=x1+1;	
												y2=y1;		y3=y1+1;	y4=y1+1;
												teilD=7;
								break;
							}
							if(Out==8) Status=1;
							else Status=2;
				break;
				case 6: printAt(7, "LVL: 1");
								printAt(9, "Lines:");
								printAt(10, "  10");
								printAt(12, "Points:");
								printAt(13, "  0");
								Out=0;
								Level=1;
								Loeschges=10;
								Loesch=0;
								Loeschen=0;
								Status=1;
								Points=0;
								rectan(10,36,59,84,2,MenuColor,0);			//SPEICHERLEISTE
								rectan(58,36,182,251,2,MenuColor,0);		//SPIELLEISTE
								rectan(181,36,230,251,2,MenuColor,0);	//STANDLEISTE
								Status=1;
				break;
				case 7: printAt(15, "PAUSE"); 
								printAt(17, "        return to Menu");
								if(Push==0 && dP==0)
								{
									rectan(0,37,240,320,1,BLACK,1);
									Status=1; Spiel=Menue;
								}
								if(getTSCtouched())
								{
									printAt(15, "       ");
									 printAt(17, "                         ");
									Status=pause;
								}
				break;
				}
				/*if(getTSCtouched()&& Status!=3)
				{
					pause=Status;
					Status=7;
				}*/
			break;
			case Optionen: 	switch(Settings)
											{
												case 1: printAt(4, "  Color:");
																printAt(8, "  Own Color  ");
																rectan(116,100,210,108,1,BLACK,1);
																line(14,81,61,81,2,MenuColor);
																switch(MenuFarbe)
																{
																		case 0: printAt(4, "  Color:    Green   ");
																						switch(light)
																						{
																							case 120: MenuColor=LIGHT_GREEN; 	break; //0x87F0
																							case 140: MenuColor=BRIGHT_GREEN;	break; //0x07E0
																							case 160: MenuColor=0x076A;				break; 
																							case 180: MenuColor=0x0DC0; 			break;
																							case 200: MenuColor=DARK_GREEN; 	break; //0x1C03
																						}
																		break;
																		case 1: printAt(4, "  Color:    Blue        ");
																						switch(light)
																						{
																							case 120: MenuColor=0x07DF; 			break; 
																							case 140: MenuColor=0x0E5F;				break;
																							case 160: MenuColor=0x0C1F;				break; 
																							case 180: MenuColor=0x009F; 			break;
																							case 200: MenuColor=0x0096; 			break; 
																						} 
																		break;
																		case 2: printAt(4, "  Color:    Yellow        ");
																						switch(light)
																				{
																					case 120: MenuColor=0xFFF8; 			break; 
																					case 140: MenuColor=YELLOW;				break;
																					case 160: MenuColor=0xFFE4;				break; 
																					case 180: MenuColor=0xFFC0; 			break;
																					case 200: MenuColor=0xFF60; 			break; //<--eher orange
																				}
																		break;
																		case 3: printAt(4, "  Color:    Orange     ");
																						switch(light)
																				{
																					case 120: MenuColor=0xFEE1; 			break; 
																					case 140: MenuColor=0xFE60;				break;
																					case 160: MenuColor=0xFDA0;				break; 
																					case 180: MenuColor=0xFD00; 			break;
																					case 200: MenuColor=0xFC40; 			break; 
																				}
																		break;
																		case 4: printAt(4, "  Color:    Red     ");
																						switch(light)
																				{
																					case 120: MenuColor=0xFC0D; 		break; 
																					case 140: MenuColor=BRIGHT_RED;		break;
																					case 160: MenuColor=BRIGHT_RED;		break; 
																					case 180: MenuColor=BRIGHT_RED; 	break;
																					case 200: MenuColor=RED; 			break; 
																				}
																		break;
																		case 5: printAt(4, "  Color:    Magenta     ");
																						switch(light)
																				{
																					case 120: MenuColor=0xFD1F; 		break; 
																					case 140: MenuColor=LIGHT_MAGENTA;		break;
																					case 160: MenuColor=BRIGHT_MAGENTA;		break; 
																					case 180: MenuColor=0xF93F; 	break;
																					case 200: MenuColor=MAGENTA; 			break; 
																				}
																		break;
																		case 6: printAt(4, "  Color:    White     ");
																						MenuColor=WHITE;
																		break;
																		case 7: printAt(4, "  Color:    None     ");
																						MenuColor=BLACK;
																		break;
																	}
																rectan(10,10,230,35,2,MenuColor,0);
																if(Right==0 && dR==0) dR=1, MenuFarbe++;
																if(MenuFarbe==0 && Left==0 && dL==0) dL=1, MenuFarbe=7;
																if(Left==0 && dL==0) dL=1, MenuFarbe--;
																if(MenuFarbe>7) MenuFarbe=0;
																if(Down==0 && dD==0 && MenuColor!=WHITE && MenuColor!=BLACK) dD=1, Settings=2;
																if(MenuColor==WHITE || MenuColor==BLACK) printAt(6,"              ");
																else printAt(6, "  Brightness: ");
																if(Push==0 && dP==0)dP=1, rectan(10,38,240,200,1,BLACK,1), Spiel=Menue;
																if(Left==1)dL=0;
																if(Right==1)dR=0;
																if(Down==1) dD=0;
																if(Push==1)dP=0;
											break;
											case 2: line(14,81,102,81,2,BLACK);
															line(15,145,88,145,2,BLACK);
															line(116,103,210,103,2,MenuColor);
															line(light,100,light,108,2,MenuColor);
															if(Right==0 && light<190)line(light,100,light,108,2,BLACK), light+=20;
															if(Left==0 && light>121)line(light,100,light,108,2,BLACK), light-=20;
															rectan(10,10,230,35,2,MenuColor,0);
															if(Push==0 && dP==0)dP=1, rectan(10,38,240,200,1,BLACK,1), Settings=1, Spiel=Menue;
															if(Up==0 && dU==0) dU=1, Settings=1;
															if(Down==0 && dD==0) dD=1, MenuColor=0x0000, Settings=3;
															if(Up==1) dU=0;
															if(Down==1) dD=0;
															if(Push==1)dP=0;
															switch (MenuFarbe)
															{
																case 0: switch(light)
																				{
																					case 120: MenuColor=LIGHT_GREEN; 	break; //0x87F0
																					case 140: MenuColor=BRIGHT_GREEN;	break; //0x07E0
																					case 160: MenuColor=0x076A;				break; 
																					case 180: MenuColor=0x0DC0; 			break;
																					case 200: MenuColor=DARK_GREEN; 	break; //0x1C03
																				}
																break;
																case 1: switch(light)
																				{
																					case 120: MenuColor=0x07DF; 			break; 
																					case 140: MenuColor=0x0E5F;				break;
																					case 160: MenuColor=0x0C1F;				break; 
																					case 180: MenuColor=0x009F; 			break;
																					case 200: MenuColor=0x0096; 			break; 
																				}
																break;
																case 2: switch(light)
																				{
																					case 120: MenuColor=0xFFF8; 			break; 
																					case 140: MenuColor=YELLOW;				break;
																					case 160: MenuColor=0xFFE4;				break; 
																					case 180: MenuColor=0xFFC0; 			break;
																					case 200: MenuColor=0xFF60; 			break; 
																				}
																break;
																case 3: switch(light)
																				{
																					case 120: MenuColor=0xFEE1; 			break; 
																					case 140: MenuColor=0xFE60;				break;
																					case 160: MenuColor=0xFDA0;				break; 
																					case 180: MenuColor=0xFD00; 			break;
																					case 200: MenuColor=0xFC40; 			break; 	
																				}
																break;
																case 4: switch(light)
																				{
																					case 120: MenuColor=0xFC0D; 		break; 
																					case 140: MenuColor=BRIGHT_RED;		break;
																					case 160: MenuColor=BRIGHT_RED;		break; 
																					case 180: MenuColor=BRIGHT_RED; 	break;
																					case 200: MenuColor=RED; 			break; 
																				}
																break;
																case 5: switch(light)
																				{
																					case 120: MenuColor=0xFD1F; 		break; 
																					case 140: MenuColor=LIGHT_MAGENTA;		break;
																					case 160: MenuColor=BRIGHT_MAGENTA;		break; 
																					case 180: MenuColor=0xF93F; 	break;
																					case 200: MenuColor=MAGENTA; 			break; 
																				}
																break;
															}
											break;
											case 3: rectan(116,100,210,108,1,BLACK,1);
															printAt(8, "  Own Color  ");
															line(15,145,88,145,2,MenuColor);
															if(Up==0 && dU==0) dU=1, Settings=2;
															if(Up==1) dU=0;
															if(Push==0 && dP==0)
															{
																dP=1, rectan(10,38,240,200,1,BLACK,1); 
																R=MenuColor>>12;
																RG=MenuColor>>8 & 0x0F;
																GB=MenuColor>>4 & 0x00F;
																B=MenuColor & 0x000F;
																Settings=4;
															}
															if(Push==1)dP=0;
											break;
											case 4: printAt(4, "          OWN COLOR");
															printAt(15, "            Back");
															printAt(17, "             OK");
															x1=5;			y1=5;
															x2=x1+1;	x3=x1-1;	x4=x1;	
															y2=y1;	 	y3=y1+1; 	y4=y1+1;
															rectan(X[x1],Y[y1],X[x1]+9,Y[y1]+9,0,OwnColor,1); 
															rectan(X[x2],Y[y2],X[x2]+9,Y[y2]+9,0,OwnColor,1);
															rectan(X[x3],Y[y3],X[x3]+9,Y[y3]+9,0,OwnColor,1);
															rectan(X[x4],Y[y4],X[x4]+9,Y[y4]+9,0,OwnColor,1);
															switch(RGB)
															{
																case 0: if(Right==0)R++;
																				if(Left==0)R--;
																				if(R>15) R=0;
																				setTextcolor(LIGHT_CYAN);
																				Wert=sprintf(buf,"            R : %u  ", R);
																				printAt(9, buf);
																				setTextcolor(WHITE);
																				Wert=sprintf(buf,"            RG: %u  ", RG); printAt(10, buf);
																				Wert=sprintf(buf,"            GB: %u  ", GB); printAt(11, buf); 
																				Wert=sprintf(buf,"            B : %u  ", B); printAt(12, buf);
																				if(Down==0 && dD==0)dD=1, RGB++;
																break;
																case 1: if(Right==0)RG++;
																				if(Left==0)RG--;
																				if(RG>15) RG=0;
																				setTextcolor(LIGHT_CYAN);
																				Wert=sprintf(buf,"            RG: %u  ", RG);
																				printAt(10, buf);
																				setTextcolor(WHITE);
																				Wert=sprintf(buf,"            R : %u  ", R); printAt(9, buf);
																				Wert=sprintf(buf,"            GB: %u  ", GB); printAt(11, buf); 
																				Wert=sprintf(buf,"            B : %u  ", B); printAt(12, buf);
																				if(Down==0 && dD==0)dD=1, RGB++;
																				if(Up==0 && dP==0)dP=1, RGB--;
																break;
																case 2: if(Right==0)GB++;
																				if(Left==0)GB--;
																				if(GB>15) GB=0;	
																				setTextcolor(LIGHT_CYAN);
																				Wert=sprintf(buf,"            GB: %u  ", GB);
																				printAt(11, buf);
																				setTextcolor(WHITE);
																				Wert=sprintf(buf,"            R : %u  ", R); printAt(9, buf);
																				Wert=sprintf(buf,"            RG: %u  ", RG); printAt(10, buf); 
																				Wert=sprintf(buf,"            B : %u  ", B); printAt(12, buf);
																				if(Down==0 && dD==0)dD=1, RGB++;
																				if(Up==0 && dP==0)dP=1, RGB--;
																break;
																case 3: line(93,258,128,258,2,BLACK);
																				if(Right==0)B++;
																				if(Left==0)B--;
																				if(B>15) B=0;	
																				setTextcolor(LIGHT_CYAN);
																				Wert=sprintf(buf,"            B : %u  ", B);
																				printAt(12, buf);
																				setTextcolor(WHITE);
																				Wert=sprintf(buf,"            R : %u  ", R); printAt(9, buf);
																				Wert=sprintf(buf,"            GB: %u  ", GB); printAt(11, buf); 
																				Wert=sprintf(buf,"            RG: %u  ", RG); printAt(10, buf);
																				if(Down==0 && dD==0)dD=1, RGB++;
																				if(Up==0 && dP==0)dP=1, RGB--;
																break;
																case 4: Wert=sprintf(buf,"            B : %u  ", B);
																				printAt(12, buf);
																				line(100,290,120,290,2,BLACK);
																				line(93,258,128,258,2,MenuColor);
																				if(Push==0 && dP==0) 
																				{
																					dP=1;
																					rectan(80, 50, 310, 280, 1,BLACK, 1);
																					RGB=0; Settings=1;
																				}
																				if(Down==0 && dD==0)dD=1, RGB++;
																				if(Up==0 && dP==0)dP=1, RGB--;
																break;
																case 5: line(93,258,128,258,2,BLACK);
																				line(101,290,120,290,2,MenuColor);
																				if(Push==0 && dP==0) 
																				{
																					dP=1;
																					MenuColor=OwnColor;
																					rectan(80, 50, 155, 310, 1,BLACK, 1);
																					rectan(10,10,230,35,2,MenuColor,0);
																					RGB=0; Settings=1;  Spiel=Menue;
																				}
																				if(Up==0 && dP==0)dP=1, RGB--;
																break;
															}
															entR=R<<12;
															entRG=RG<<8;
															entGB=GB<<4;
															OwnColor=B | entGB | entRG  | entR;
															if(Left==1)	dL=0;
															if(Right==1)dR=0;
															if(Down==1) dD=0;
															if(Up==1)		dU=0;
															if(Push==1)	dP=0;
											break;	
										}
			break;
			case Ranking: Wert=sprintf(buf, "  1.  %u    ", rank[1]); printAt(6, buf);
										Wert=sprintf(buf, "  2.  %u    ", rank[2]); printAt(8, buf);
										Wert=sprintf(buf, "  3.  %u    ", rank[3]); printAt(10, buf);
										Wert=sprintf(buf, "  4.  %u    ", rank[4]); printAt(12, buf);
										Wert=sprintf(buf, "  5.  %u    ", rank[5]); printAt(14, buf);
										
										if(Push==0)
										{
											rectan(0,37,240,320,1,BLACK,1);
											Spiel=Menue;
										}
			break;
		}
	}
} 

void Teil(char TeilD, char Drehen) //Unterprogramm teile
{
	if(y4<20)
	{
		rectan(X[x1],Y[y1],X[x1]+9,Y[y1]+9,0,BLACK,1);
		rectan(X[x2],Y[y2],X[x2]+9,Y[y2]+9,0,BLACK,1);
		rectan(X[x3],Y[y3],X[x3]+9,Y[y3]+9,0,BLACK,1);
		rectan(X[x4],Y[y4],X[x4]+9,Y[y4]+9,0,BLACK,1);
	}
	switch(teilD)
	{
		case 1:	switch(Drehen) //Helblau I 
						{
							case 1:	if(x1+2>12) x1=9;
											if(x1==0) x1=1;
											x2=x1+2; 	x3=x1-1; 	x4=x1+1;
											y2=y1; 		y3=y1; 		y4=y1;
							break;
							case 2: if(y1-1<0) y1=1;
											x2=x1; 		x3=x1; 		x4=x1;
											y2=y1+1;	y3=y1-1;	y4=y1+2; 
							break;
							case 3: if(x1+2>12) x1=9;
											if(x1==0) x1=1;
											x2=x1+2; 	x3=x1-1; 	x4=x1+1;
											y2=y1; 		y3=y1; 		y4=y1;
							break;
							case 4: if(y1-1<0) y1=1;
											x2=x1; 		x3=x1; 		x4=x1;
											y2=y1+1;	y3=y1-1;	y4=y1+2; 
							break;
							}
		break;
		case 2: switch(Drehen) //Dunkelblau L
						{
							case 1: if(x1+1>11) x1=10;
											if(x1==0) x1=1;
											if(y1-1<0) y1=1;
											x2=x1+1;	x3=x1-1;	x4=x1-1;
											y2=y1; 		y3=y1-1; 	y4=y1; 
							break;
							case 2:	if(x1+1>11) x1=10;
											if(y1-1<0) y1=1;
											x2=x1+1;	x3=x1;		x4=x1;
											y2=y1-1; 	y3=y1-1; 	y4=y1+1;
											
							break;
							case 3: if(x1+1>170) x1=9;
											if(x1==0) x1=1;
											x2=x1+1;	x3=x1-1;	x4=x1+1;
											y2=y1; 		y3=y1; 		y4=y1+1;
							break;
							case 4: if(x1==0) x1=1;
											if(y1-1<0) y1=1;
											x2=x1;		x3=x1-1;	x4=x1;
											y2=y1-1; 	y3=y1+1; 	y4=y1+1;
							break;
						}
		break;
		case 3:	switch(Drehen) //Pink L
						{
							case 1: if(x1+1>11) x1=10;
											else if(x1==0) x1=1;
											x2=x1+1; 	x3=x1+1;	x4=x1-1;	
											y2=y1; 		y3=y1-1;	y4=y1;
							break;
							case 2: if(y1-1<0) y1=1;
											if(x1+1>11) x1=10;
											x2=x1+1; 	x3=x1;		x4=x1;	
											y2=y1+1; 	y3=y1-1;	y4=y1+1;
							break;
							case 3:	if(x1+1>11) x1=10;
											else if(x1==0) x1=1;
											x2=x1+1; 	x3=x1-1;	x4=x1-1;	
											y2=y1; 		y3=y1;		y4=y1+1;
							break;
							case 4: if(y1-1<0) y1=1;
											if(x1==0) x1=1;
											x2=x1; 		x3=x1-1;	x4=x1;	
											y2=y1-1; 	y3=y1-1;	y4=y1+1;
							break;
						}
		break;
		case 4:	switch(Drehen) //Rot Z
						{
							case 1: if(x1+1>11) x1=10;
											if(x1==0) x1=1;
											x2=x1+1;	x3=x1-1;	x4=x1;	
											y2=y1+1; 	y3=y1; 		y4=y1+1;
							break;
							case 2: if(x1+1>11) x1=10;
											if(y1-1<0) y1=1;
											x2=x1+1;	x3=x1+1;	x4=x1;	
											y2=y1;	 	y3=y1-1; 	y4=y1+1;
							break;
							case 3: if(x1+1>11) x1=10;
											if(x1==0) x1=1;
											x2=x1+1;	x3=x1-1;	x4=x1;	
											y2=y1+1; 	y3=y1; 		y4=y1+1;
							break;
							case 4: if(x1+1>11) x1=10;
											if(y1-1<0) y1=1;
											x2=x1+1;	x3=x1+1;	x4=x1;	
											y2=y1;	 	y3=y1-1; 	y4=y1+1;
							break;
						}
		break;
		case 5: 
						switch(Drehen) //Grün Z
						{
							case 1: if(x1+1>11) x1=10;
											if(x1==0) x1=1;
											x2=x1+1;	x3=x1-1;	x4=x1;	
											y2=y1;	 	y3=y1+1; 	y4=y1+1;
							break;
							case 2: if(x1+1>11) x1=10;
											if(y1-1<0) y1=1;
											x2=x1+1;	x3=x1;		x4=x1+1;	
											y2=y1;	 	y3=y1-1; 	y4=y1+1;
							break;
							case 3: if(x1+1>11) x1=10;
											if(x1==0) x1=1;
											x2=x1+1;	x3=x1-1;	x4=x1;	
											y2=y1;	 	y3=y1+1; 	y4=y1+1;
							break;
							case 4: if(x1+1>11) x1=10;
											if(y1-1<0) y1=1;
											x2=x1+1;	x3=x1;		x4=x1+1;	
											y2=y1;	 	y3=y1-1; 	y4=y1+1;
							break;
						}
		break;
		case 6: switch(Drehen) //Viollet 
						{
							case 1: if(x1+1>11) x1=10;
											if(x1==0) x1=1;
											if(y1-1<0) y1=1;
											x2=x1+1;	x3=x1;		x4=x1-1;	
											y2=y1;		y3=y1-1;	y4=y1;
							break;
							case 2: if(x1+1>11) x1=10;
											if(y1-1<0) y1=1;
											x2=x1+1;	x3=x1;		x4=x1;	
											y2=y1;		y3=y1-1;	y4=y1+1;
							break;
							case 3: if(x1+1>11) x1=10;
											if(x1==0) x1=1;
											x2=x1+1;	x3=x1-1;	x4=x1;	
											y2=y1;		y3=y1;		y4=y1+1;
							break;
							case 4: if(x1==0) x1=1;
											if(y1-1<0) y1=1;
											x2=x1;		x3=x1-1;	x4=x1;	
											y2=y1-1;	y3=y1;		y4=y1+1;
							break;
						}
		break;
		case 7: switch(Drehen) //Gelb
						{
							case 1: x2=x1+1; 	x3=x1; 		x4=x1+1;	
											y2=y1;		y3=y1+1;	y4=y1+1;break;
							case 2: x2=x1+1; 	x3=x1; 		x4=x1+1;	
											y2=y1;		y3=y1+1;	y4=y1+1;break;
							case 3: x2=x1+1; 	x3=x1; 		x4=x1+1;	
											y2=y1;		y3=y1+1;	y4=y1+1;break;
							case 4: x2=x1+1; 	x3=x1; 		x4=x1+1;	
											y2=y1;		y3=y1+1;	y4=y1+1;break;
						}
		break;
	}
}
