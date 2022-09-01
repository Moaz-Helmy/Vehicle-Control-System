/*
 * main.c
 *
 *  Created on: Aug 19, 2022
 *      Author: Moaz Mohamed Helmy
 *      Description: Vehicle Control System implementation
 */
#include <stdio.h>
/*Bonus Requirement*/
#define WITH_ENGINE_TEMP_CONTROLLER 1

/****************************Functions Prototypes*************************************/
/*Function to display choices of the main menu an take input from the user*/
void menu1(char*);
/*function to display options of the second menu after turning on the engine and to take input from the user*/
void sensors(char*);
/*function that set value to traffic light sensor and adjust speed based on the inputed data*/
void trafficLight();
/*function that prints the state of different devices in the vehicle like AC, ETC, and engine, and also displays sensors readings*/
void printStates();
/*function that assign a value to room temp based on data read from the user, and make decisions related to AC status based on inputd data*/
void roomTemp();

#if WITH_ENGINE_TEMP_CONTROLLER
/*function that assigns a value to engine temperature and adjust ETC status based on these values*/
void engineTemp();
#endif
/*function that adjust AC status and room and engine temperatures based on vehicle speed*/
void validateSpeed();
/*function to set initial values to system parameters*/
void sysInit();
/*function that act like a processor of the system; to control the flow of the system code*/
int controlUnit(char);
/*structure that contains sensor readings variables*/
typedef struct {
	char trafficLight;
	float roomTemp;

#if WITH_ENGINE_TEMP_CONTROLLER
	float engineTemp;
#endif

	int vehicleSpeed;
}sensorsSet;
sensorsSet senReadings;
/*enum to specify the status of system devices like AC, ETc, and engine*/
typedef enum{
	OFF,ON
}state;

#if WITH_ENGINE_TEMP_CONTROLLER
state ETC;
#endif

state AC;
state engineStatus;

/*program starts here*/
int main(void)
{
	setvbuf(stdout,NULL,_IONBF,0);
	setvbuf(stderr,NULL,_IONBF,0);
	char choice1,choice2;
	state bool;
	sysInit();
	while(1)
	{
		bool=ON;
		menu1(&choice1);

		if(choice1=='c'||choice1=='C')
		{
			return 0;
		}
		while(bool)
		{
			sensors(&choice2);
			bool=controlUnit(choice2);
		}
	}

}

void menu1(char * choice1)
{
	int invalidInput=0;
	state bool=ON;
	while(bool)
	{
		if(!invalidInput)
		{
			puts("a. Turn on the vehicle engine.");
			puts("b. Turn off the vehicle engine.");
			puts("c. Quit the system.");
		}
		scanf(" %c",choice1);
		switch (*choice1)
		{
		case 'a':
		case 'A':
			puts("Turn on the vehicle engine.");
			engineStatus=ON;
			bool=OFF;
			break;

		case 'b':
		case 'B':
			puts("Turn off the vehicle engine.");
			engineStatus=OFF;
			break;
		case 'c':
		case 'C':
			puts("Quit the system.");
			return ;

		default:
			puts("Invalid Input");
			invalidInput=1;
			break;
		}
	}
	return ;
}

void sensors(char *choice2)
{
	int invalidInput=0;
	state bool=ON;
	while(bool)
	{
		if(!invalidInput)
		{
			puts("\na. Turn off the vehicle engine.");
			puts("b. Set the traffic light color.");
			puts("c. Set the room temperature (Temperature Sensor).");

#if WITH_ENGINE_TEMP_CONTROLLER
			puts("d. Set the engine temperature (Engine Temperature Sensor).");
#endif

		}
		scanf(" %c",choice2);
		switch (*choice2)
		{
		case 'a':
		case 'A':
			puts("Turn off the vehicle engine.");
			engineStatus=OFF;
			bool=OFF;
			break;

		case 'b':
		case 'B':
			puts("Set the traffic light color.");
			bool=OFF;
			break;
		case 'c':
		case 'C':
			puts("Set the room temperature (Temperature Sensor).");
			bool=OFF;
			break;

#if WITH_ENGINE_TEMP_CONTROLLER
		case 'd':
		case 'D':
			puts("Set the engine temperature (Engine Temperature Sensor).");
			bool=OFF;
			break;
#endif

		default:
			puts("Invalid Input");
			invalidInput=1;
			break;
		}
	}
	return ;
}

int controlUnit(char choice2)
{
	switch(choice2)
	{
	case'a':
	case 'A':
		return OFF;
	case'b':
	case'B':
		trafficLight();
		validateSpeed();
		printStates();
		break;
	case'c':
	case'C':
		roomTemp();
		printStates();
		break;

#if WITH_ENGINE_TEMP_CONTROLLER
	case'd':
	case 'D':
		engineTemp();
		printStates();
		break;
#endif

	default:
		break;
	}
	return 1;
}

void sysInit()
{
	senReadings.trafficLight='r';
	senReadings.vehicleSpeed=0;
	senReadings.roomTemp=25;

#if WITH_ENGINE_TEMP_CONTROLLER
	senReadings.engineTemp=100;
	ETC=OFF;
#endif

	AC=OFF;
}

void printStates()
{
	(engineStatus==ON)?printf("\nEngine is ON\n"):printf("Engine is OFF\n");
	(AC==ON)?printf("AC is ON\n"):printf("AC is OFF\n");
	printf("Vehicle Speed = %d km/h\n",senReadings.vehicleSpeed);
	printf("Room Temperature = %d C\n",(int)senReadings.roomTemp);
#if WITH_ENGINE_TEMP_CONTROLLER
	(ETC==ON)?printf("Engine Temperature Controller is ON\n"):printf("Engine Temperature Controller is OFF\n");
	printf("Engine Temperature = %d C\n\n",(int)senReadings.engineTemp);
#endif
}

void trafficLight()
{
	printf("\nEnter traffic light data:");
	state bool=ON;
	while(bool)
	{
		scanf(" %c",&senReadings.trafficLight);
		switch (senReadings.trafficLight)
		{
		case'g':
		case'G':
			senReadings.vehicleSpeed=100;
			bool=OFF;
			break;
		case'o':
		case'O':
			senReadings.vehicleSpeed=30;
			bool=OFF;
			break;
		case'r':
		case'R':
			senReadings.vehicleSpeed=0;
			bool=OFF;
			break;
		default:
			puts("Invalid Input");
			break;
		}
	}

}

void validateSpeed()
{
	if(senReadings.vehicleSpeed==30)
	{
		AC=ON;
		senReadings.roomTemp=senReadings.roomTemp*(5/4)+1;
#if WITH_ENGINE_TEMP_CONTROLLER
		ETC=ON;
		senReadings.engineTemp=senReadings.engineTemp*(5/4)+1;
#endif
	}
	else
	{

	}
}

void roomTemp()
{
	printf("\nEnter room temperature data:");
	scanf(" %f",&senReadings.roomTemp);
	if(senReadings.roomTemp<10)
	{
		AC=ON;
		senReadings.roomTemp=20;
	}
	else if(senReadings.roomTemp>30)
	{
		AC=ON;
		senReadings.roomTemp=20;
	}
	else
	{
		AC=OFF;
	}
}

#if WITH_ENGINE_TEMP_CONTROLLER
void engineTemp()
{
	printf("\nEnter engine temperature data:");
	scanf(" %f",&senReadings.engineTemp);
	if(senReadings.engineTemp<100)
	{
		ETC=ON;
		senReadings.engineTemp=125;
	}
	else if(senReadings.engineTemp>150)
	{
		ETC=ON;
		senReadings.engineTemp=125;
	}
	else
	{
		ETC=OFF;
	}
}
#endif
