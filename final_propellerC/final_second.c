/*
  Blank Simple Project.c
  http://learn.parallax.com/propeller-c-tutorials 
*/
#include "simpletools.h"                      // Include simple tools
#include "servo.h"
#include "ping.h"
#include "fdserial.h"

#define NOT_CHANGE 0
#define ACCELERATE 5
#define TURN_RIGHT 3
#define TURN_LEFT  1
#define RECEIVE    4
#define SLOW_DOWN  2
#define STOP       6



volatile int ch = 0;
volatile int XeeChange=0;
volatile fdserial *xbee;
int cmDist=0;
/*int run_path=0;
int run_run=0;
int path[4000];*/
int *cog_ptr[8];
 

int xbee_direction(); //PING+XBEE to measure durection
void XBEE();
void speed(int mode);          //robot's speed
void day_and_night();  //turn on the light un the night

int main()                                    // Main function
{
  servo_set(14, 1500);
  servo_set(15, 1500);
  xbee = fdserial_open(0, 1, 0, 9600);
  cog_ptr[1] = cog_run(day_and_night,32);
  /*for(int i=0 ; i<4000 ; i++){
    path[i]=-1;
  }    
  
  //cog_ptr[2] = cog_run(&XBEE,32);                     //???????????
  pause(6000);*/
  int mode = 0;
  while(1)
  {
    mode = xbee_direction();
    print("%c", (char) ch);
    speed(mode);

  }  
}


int xbee_direction(){     //PING+XBEE to measure durection
  
  cog_ptr[2] = cog_run(XBEE,32);
  while(1){
    cmDist = ping_cm(17);
    //printf("ch=%d\n",ch-48);          //
    //printf("Xee=%d\n",XeeChange-48);
    pause(500);
    //if(path[run_run]!=-1){
      if(cmDist>=20 && (ch-48) == 0){
        return NOT_CHANGE;
      }
      else if(cmDist>=20 && (ch-48) == SLOW_DOWN){
        return SLOW_DOWN;
      }
      else if(cmDist>=20 && (ch-48) == ACCELERATE){
        return ACCELERATE;
      }
      else if(cmDist<20){
        return STOP;
      }
      else if(cmDist>=20 && (ch-48) == TURN_RIGHT){
       return TURN_RIGHT;
     }
     else if(cmDist>=20 && (ch-48) == TURN_LEFT){
       return TURN_LEFT;
     }
     else{
       return STOP;
     }
   //}
   /*else if(path[run_run]==-1){
     return STOP;
   }
   run_run=run_run+1;*/
                                             
  }            
}

void XBEE(){              //the first car send message to the second car
  putchar(CLS); // Clear terminal screen
	fdserial_rxFlush(xbee);
	//pause(100);
 	
	while (1) {
	  ch = fdserial_rxChar(xbee);
		fdserial_rxFlush(xbee);
		//print("%c", (char) ch);
    /*path[run_path]=ch;
    run_path=run_path+1;*/
		fdserial_txChar(xbee, ch);
		fdserial_txFlush(xbee);
	}    
}
  

void speed(int mode){            //mode: accerlate not_change slow_down
 int limit_speed=30;            //get data from openCV ***
    switch(mode){
      case NOT_CHANGE :
        servo_speed(14,limit_speed-10);
        servo_speed(15,-(limit_speed-10));
        //XeeChange = NOT_CHANGE;
        //printf("NOT_CHANGE\n");
        pause(500);
        break;
      case ACCELERATE :
        servo_speed(14,limit_speed+10);
        servo_speed(15,-(limit_speed+10));
        //XeeChange = ACCELERATE;
        //printf("ACCELERATE\n");
        pause(500);
        break;
      case SLOW_DOWN :
        servo_speed(14,limit_speed-20);
        servo_speed(15,-(limit_speed-20));
        //XeeChange = SLOW_DOWN;
        //printf("SLOW_DOWN\n");
        pause(500);
        break;
      case TURN_RIGHT :
        servo_speed(14,-(limit_speed-20));
        servo_speed(15,limit_speed-20);
        pause(250);
        servo_speed(14,(limit_speed-5));
        servo_speed(15,(limit_speed-5));
        //XeeChange = TURN_RIGHT;
        //printf("TURN_RIGHT\n");
        pause(200); //?
        break;
      case TURN_LEFT :
        servo_speed(14,-(limit_speed-20));
        servo_speed(15,limit_speed-20);
        pause(250);
        servo_speed(14,-(limit_speed-5));
        servo_speed(15,-(limit_speed-5));
        //XeeChange = TURN_LEFT;
        //printf("TURN_LEFT\n");
        pause(200);
        break;  //?
      case STOP :
        servo_speed(14,0);
        servo_speed(15,0);
        //XeeChange = STOP;
        //printf("STOP\n");
        pause(500); //?
        break;
      default:
        servo_speed(14,limit_speed-10);
        servo_speed(15,-(limit_speed-10));
        //XeeChange = STOP;
        //printf("DEFAULT\n");
        pause(500);
        break;
    }  
}    

void day_and_night(){
  while(1){
    high(5);
    pause(1);
    int t = rc_time(5,1);
    if(t<1000){
      dac_ctr(26, 0, 0);      //0V    
      dac_ctr(27, 1, 0);
      pause(500);
    }
    else if(t>=1000 && t<1500){
      dac_ctr(26, 0, 77);     //1V
      dac_ctr(27, 1, 77);
      pause(500);
    }
    else if(t>=1500 && t<2000){
      dac_ctr(26, 0, 155);    //2V   
      dac_ctr(27, 1, 155);
      pause(500);            
    }
    else if(t>=2000 && t<3000){
      dac_ctr(26, 0, 230);    //3V   
      dac_ctr(27, 1, 230);
      pause(500);      
    }
    else if(t>=3000){
      dac_ctr(26, 0, 255);    //3.3V   
      dac_ctr(27, 1, 255);
      pause(500);      
    }
    else{
      dac_ctr(26, 0, 0);   
      dac_ctr(27, 1, 0);
      pause(500);
    }
  }
}            