/*
 * This file is part of rad1o
 *
 */


#include <unistd.h>

#include <rad1olib/setup.h>
#include <r0ketlib/display.h>
#include <r0ketlib/keyin.h>
#include <r0ketlib/menu.h>
#include <r0ketlib/config.h>

#include <rad1olib/pins.h>
#include <rad1olib/systick.h>
#include <rad1olib/battery.h>

#include <r0ketlib/fs_util.h>

#include "main.gen"
#include "l0ungel1cht.h"


#define EVERY(x,y) if((ctr+y)%(x/SYSTICKSPEED)==0)

void tick_batteryRAD1O_LED(void){
    static int ctr;
    ctr++;

    EVERY(50,0){
        if(GLOBAL(chargeled)){
            //char iodir= (GPIO_GPIO1DIR & (1 << (11) ))?1:0;
            if(batteryCharging()) {
                ON(RAD1O_LED4);
            } else {
                OFF(RAD1O_LED4);
            }
        };

        if(batteryGetVoltage()<3600){
            if( (ctr/(50/SYSTICKSPEED))%10 == 1 ) {
                ON(RAD1O_LED4);
            } else {
                OFF(RAD1O_LED4);
            }
        };
    };

    return;
}

void sys_tick_handler(void){
	incTimer();
	generated_tick();
};

void init_nick(void){
	readTextFile("nick.cfg",GLOBAL(nickname),MAXNICK);
	readTextFile("font.cfg",GLOBAL(nickfont),FLEN);
	readTextFile("l0nick.cfg",GLOBAL(nickl0),FLEN);
}

int main(void) {
	cpuClockInit(); /* CPU Clock is now 104 MHz */
	systickInit();

//	cpu_clock_set(204);

	SETUPgout(EN_VDD);
	SETUPgout(EN_1V8);
	SETUPgout(MIXER_EN);
	SETUPgout(MIC_AMP_DIS);

	SETUPgout(RAD1O_LED1);
	SETUPgout(RAD1O_LED2);
	SETUPgout(RAD1O_LED3);
	SETUPgout(RAD1O_LED4);
	SETUPpin(UART0_TXD);
	SETUPpin(UART0_RXD);

	inputInit();
	lcdInit();
	fsInit();
	lcdFill(0xff);
	readConfig();
	batteryInit();
  init_nick();

	generated_init();

	while(1){
      l0ungel1cht();
	};
	return 0;
}
