/********************************************************************

Battery Balancing Algorithm - When 3 different voltage batteries are
connected in parallel to serve a constant load, than they discharges
unequally, to resolve this, this program switches batteries ON and OFF
for automatically by three switches to keep three voltages at equal
level.


Date 19 November2016

Written By:
Vikram Pareek

********************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define     NUMTHRDS 3          //Total 3 threads are used for automatic battery discharging based on linear curve
pthread_t   t[NUMTHRDS];        //


/******************Function Declaration******************************/


char GreatestCalculator(float a,float b,float c);

/***********************************************
This function takes three float values a,b and c
and returns a character which means
a - a is greater
b - b is greater
c - c is greater
d - a and b are greater and equal
e - b and c are greater and equal
f - a and c are greater and equal
g - a,b and c are equal

************************************************/

void Switcher(char BatteryToSwitch); // It switches S1, S2 and S3 based on return value of GreatestCalculator

static void * BATTERY1(void * _);   //Threads for automatic battery discharging
static void * BATTERY2(void * _);
static void * BATTContinousl;ERY3(void * _);


/******************Function Declaration******************************/


/**********************Global Variables******************************/


const float BATTERY_CAPACITY=50; //In Watt*hour// Assume all battery have same capacity
float       RLoad; //in watt
int         TotalDischargeTime;

struct BatteryParameter
{
    float          InitialBatteryVoltage;
    volatile float BatteryVoltage;
    volatile float BatteryCurrent;
    float          BatteryResistance;
    volatile int   SwitchState;         //0 means OFF and 1 means ON
    volatile int   DischargeSecond;
};

struct BatteryParameter B1;
struct BatteryParameter B2;
struct BatteryParameter B3;


/**********************Global Variables******************************/


int main()
{
	pthread_create(&t[1], NULL, BATTERY1, NULL);
	pthread_create(&t[0], NULL, BATTERY2, NULL);
	pthread_create(&t[2], NULL, BATTERY3, NULL);


    printf("Enter Battery B1 voltage\n");
    scanf("%f",&B1.InitialBatteryVoltage);
    printf("Enter Battery B2 voltage\n");
    scanf("%f",&B2.InitialBatteryVoltage);
    printf("Enter Battery B3 voltage\n");
    scanf("%f",&B3.InitialBatteryVoltage);
    printf("Enter load amount (in watt)\n");
    scanf("%f",&RLoad);
    printf("Enter R1, R2, R3 values\n");
    //scanf("%f%f%f",&B1.BatteryResistance,B2.BatteryResistance,B3.BatteryResistance);

    printf("Battery energy is assumed to be %f Watt*Hour(Wh)\n",BATTERY_CAPACITY);

    printf("Simulator Started\n");

    B1.BatteryVoltage=B1.InitialBatteryVoltage;
    B2.BatteryVoltage=B2.InitialBatteryVoltage;
    B3.BatteryVoltage=B3.InitialBatteryVoltage;

    B1.DischargeSecond=0;
    B2.DischargeSecond=0;
    B3.DischargeSecond=0;

    B1.SwitchState=0;
    B2.SwitchState=0;
    B3.SwitchState=0;

    TotalDischargeTime=BATTERY_CAPACITY/RLoad; // TotalDischargeTime is time in which all batteries get discharged when connected individually


char SwitchIdentifier;
while(1)
{
    SwitchIdentifier=GreatestCalculator(B1.BatteryVoltage,B2.BatteryVoltage,B3.BatteryVoltage);
    Switcher(SwitchIdentifier);
    printf("Switch1= %d  V1= %f V  I1= %f A            Switch2= %d  V2= %f V  I2= %f A             Switch3= %d  V3= %f V  I3= %f A\n",B1.SwitchState,B1.BatteryVoltage,B1.BatteryCurrent,B2.SwitchState,B2.BatteryVoltage,B2.BatteryCurrent,B3.SwitchState,B3.BatteryVoltage,B3.BatteryCurrent);
    Sleep(2000);

    if((B1.BatteryVoltage<8)||(B2.BatteryVoltage<8)||(B3.BatteryVoltage<8))
    {
        B1.SwitchState=0;
        B2.SwitchState=0;
        B3.SwitchState=0;
        break;
    }


}
    printf("Program Completed\n");
    //switch(i);
    pthread_exit(NULL);
    return 0;
}

char GreatestCalculator(float a,float b,float c)
{
    if(a==b)
    {
        if(b==c)
        {
            return 'g';
        }else if(b>c)
        {
            return 'd';
        }else
        {
            return 'c';
        }

    }else if(a==c)
    {
        if(a>b)
        {
            return 'f';
        }else
        {
            return 'b';
        }
    }else if(b==c)
    {
        if(b>a)
        {
            return 'e';
        }else
        {
            return 'a';
        }
    }else if(a>b)
    {
        if(a>c)
        {
            return 'a';
        }else
        {
            return 'c';
        }
    }else
    {
        if(b>c)
        {
            return 'b';

        }else
        {
            return 'c';
        }
    }
}

void Switcher(char BatteryToSwitch)
{
        if(BatteryToSwitch=='a') //means voltage of B1 is highest
        {
         B1.SwitchState=1;
         B2.SwitchState=0;
         B3.SwitchState=0;

        }else if(BatteryToSwitch=='b') //means voltage of B2 is highest
        {
         B1.SwitchState=0;
         B2.SwitchState=1;
         B3.SwitchState=0;


        }else if(BatteryToSwitch=='c')  //means voltage of B3 is highest
        {
         B1.SwitchState=0;
         B2.SwitchState=0;
         B3.SwitchState=1;

        }
        else if(BatteryToSwitch=='d')   //means B1 and B2 are at equal and higher voltage
        {
         B1.SwitchState=1;
         B2.SwitchState=1;
         B3.SwitchState=0;

        }
        else if(BatteryToSwitch=='e')   //means B2 and B3 are at equal and higher voltage
        {
         B1.SwitchState=0;
         B2.SwitchState=1;
         B3.SwitchState=1;

        }else if(BatteryToSwitch=='f')   //means B3 and B1 are at equal and higher voltage
        {
         B1.SwitchState=1;
         B2.SwitchState=0;
         B3.SwitchState=1;

        }else if(BatteryToSwitch=='g')   //means B1 and B2 and B3 are at equal voltage
        {
         B1.SwitchState=1;
         B2.SwitchState=1;
         B3.SwitchState=1;
        }
}



/****************************************************************
This thread will run parallel to process of main function and
when Battery B1 gets connected in circuit this thread start
changing battery parameters automatically based on slope of
discharging curve.

 Present Battery voltage = (slope)*(Discharging time)+ Initial battery voltage

 Present Current sourced by battery = (Load in watt) / (Present Battery voltage)

*****************************************************************/
static void * BATTERY1(void * _)       .
{

    while(1)
    {
        if((B1.SwitchState==1)&&(B2.SwitchState==0)&&(B3.SwitchState==0))
        {
            Sleep(1000);
            B1.DischargeSecond=B1.DischargeSecond+1;
            B1.BatteryVoltage=B1.InitialBatteryVoltage-(0.3*B1.DischargeSecond);
            B1.BatteryCurrent=RLoad/B1.BatteryVoltage;
            //printf("%d              %f\n",B1.DischargeSecond,B1.BatteryVoltage);
        }else if((B1.SwitchState==1)&&(B2.SwitchState==1)&&(B3.SwitchState==0))
        {
            Sleep(1000);
            B1.DischargeSecond=B1.DischargeSecond+1;
            B1.BatteryVoltage=B1.InitialBatteryVoltage-(0.15*B1.DischargeSecond);
            B1.BatteryCurrent=(RLoad/B1.BatteryVoltage)/2;

        }else if((B1.SwitchState==1)&&(B2.SwitchState==0)&&(B3.SwitchState==1))
        {
            Sleep(1000);
            B1.DischargeSecond=B1.DischargeSecond+1;
            B1.BatteryVoltage=B1.InitialBatteryVoltage-(0.15*B1.DischargeSecond);
            B1.BatteryCurrent=(RLoad/B1.BatteryVoltage)/2;

        }else if((B1.SwitchState==1)&&(B2.SwitchState==1)&&(B3.SwitchState==1))
        {
            Sleep(1000);
            B1.DischargeSecond=B1.DischargeSecond+1;
            B1.BatteryVoltage=B1.InitialBatteryVoltage-(0.1*B1.DischargeSecond);
            B1.BatteryCurrent=(RLoad/B1.BatteryVoltage)/3;

        }

	}
	return 0;
}

static void * BATTERY2(void *_)
{


    while(1)
    {
        if((B2.SwitchState==1)&&(B1.SwitchState==0)&&(B3.SwitchState==0))
        {
            Sleep(1000);
            B2.DischargeSecond=B2.DischargeSecond+1;
            B2.BatteryVoltage=B2.InitialBatteryVoltage-(0.2*B2.DischargeSecond);
            B2.BatteryCurrent=RLoad/B2.BatteryVoltage;

        }else if((B2.SwitchState==1)&&(B1.SwitchState==1)&&(B3.SwitchState==0))
        {
            Sleep(1000);
            B2.DischargeSecond=B2.DischargeSecond+1;
            B2.BatteryVoltage=B2.InitialBatteryVoltage-(0.1*B2.DischargeSecond);
            B2.BatteryCurrent=(RLoad/B2.BatteryVoltage)/2;

        }else if((B2.SwitchState==1)&&(B1.SwitchState==0)&&(B3.SwitchState==1))
        {
            Sleep(1000);
            B2.DischargeSecond=B2.DischargeSecond+1;
            B2.BatteryVoltage=B2.InitialBatteryVoltage-(0.1*B2.DischargeSecond);
            B2.BatteryCurrent=(RLoad/B2.BatteryVoltage)/2;

        }else if((B2.SwitchState==1)&&(B1.SwitchState==1)&&(B3.SwitchState==1))
        {
            Sleep(1000);
            B2.DischargeSecond=B2.DischargeSecond+1;
            B2.BatteryVoltage=B2.InitialBatteryVoltage-(0.05*B2.DischargeSecond);
            B2.BatteryCurrent=(RLoad/B2.BatteryVoltage)/3;

        }

	}
	return 0;
}

static void * BATTERY3(void * _)
{

    while(1)
    {
        if((B3.SwitchState==1)&&(B2.SwitchState==0)&&(B1.SwitchState==0))
        {
            Sleep(1000);
            B3.DischargeSecond=B3.DischargeSecond+1;
            B3.BatteryVoltage=B3.InitialBatteryVoltage-(0.1*B3.DischargeSecond);
            B3.BatteryCurrent=RLoad/B3.BatteryVoltage;

        }else if((B3.SwitchState==1)&&(B2.SwitchState==1)&&(B1.SwitchState==0))
        {
            Sleep(1000);
            B3.DischargeSecond=B3.DischargeSecond+1;
            B3.BatteryVoltage=B3.InitialBatteryVoltage-(0.05*B3.DischargeSecond);
            B3.BatteryCurrent=(RLoad/B3.BatteryVoltage)/2;

        }else if((B3.SwitchState==1)&&(B2.SwitchState==0)&&(B1.SwitchState==1))
        {
            Sleep(1000);
            B3.DischargeSecond=B3.DischargeSecond+1;
            B3.BatteryVoltage=B3.InitialBatteryVoltage-(0.05*B3.DischargeSecond);
            B3.BatteryCurrent=(RLoad/B3.BatteryVoltage)/2;

        }else if((B3.SwitchState==1)&&(B2.SwitchState==1)&&(B1.SwitchState==1))
        {
            Sleep(1000);
            B3.DischargeSecond=B3.DischargeSecond+1;
            B3.BatteryVoltage=B3.InitialBatteryVoltage-(0.025*B3.DischargeSecond);
            B3.BatteryCurrent=(RLoad/B3.BatteryVoltage)/3;

        }

	}
	return 0;
}




