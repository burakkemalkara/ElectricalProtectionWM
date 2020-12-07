/* tester.c ANSI/ISO C99
   Simulator of MCU applied to control input voltage applied to
   inverter welding machines. Uses semi random data to simulate
   four inputs bits into control MCU and analyzes for taking
   control decisions taking into account logic hysteresis for
   cutting or enabling PWM in the inverter welding machine.
   For Burak, August 9th 2018
 */
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
/* Define cutting values. By standard +/- 10% */
#define hhv 253  /* +10% */
#define hlv 240	 /* Arbitrary hysteresis */
#define llv 207	 /* -10% */
#define lhv 220	 /* Arbitrary hysteresis */
/* Define simulation range */
#define sh 270
#define sl 170
/* Define status bits */
#define ok 1
#define ol 0  /* Out of sh and sv */
char statusbit (int ivalue, char cst [4]);
char status (char cst [4]);
int main(void)
{
	int i, ic, ir, cc = 0;	   	   	/* Auxiliary varialbles */
	char cr, ci, cf = 0, cs[4];
	srand (time(NULL));  	    	/* Initialize seed for fresh simulation */
	printf ("volts\t1 2 3 4  s Output\n");
	for (i = 0; i < 70; i++)
	{
		ir = rand() / 100; 	   	    /* Getting more sampling */
		cr = statusbit(ir, cs);		/* Getting samples of status bits */
		if (cr)
		{
			printf ("> %d\t", ir);
			for (ci = 0; ci < 4; ci++)
				printf ("%c ", cs[ci]);
			cr = status (cs);
			printf (" %c ", cr);    /* Real decision is made here */
			if (cr == 'S')
			{
				printf ("Stop\n");
				cf = 1;
			}
			else if (cr == 'H' && cf == 1)
				printf ("Stop Hyst.\n");
			else if (cr == 'H' && cf == 0)
				printf ("Working Hyst.\n");
			else
			{
				cf = 0;
				printf ("Working\n");
			}
		}
		cc++;
	}
	printf ("\nReal sampling: %d \n", cc);
	return 0;
}
char statusbit (int ivalue, char cst [4]) /* Returns an array with four bit values hh, hl, lh, ll */
{
	char cc;
	for (cc = 0; cc < 4; cc++)      /* Initialize */
		cst[cc] = 'x';
	if (ivalue < sl || ivalue > sh) /* Only check in quoted range */
		return ol;
	else
	{
		if (ivalue > hhv)
			cst[0] = '1';
		else
			cst[0] = '0';
		if (ivalue > hlv)
			cst[1] = '1';
		else
			cst[1] = '0';
		if (ivalue < lhv)
			cst[2] = '1';
		else
			cst[2] = '0';
		if (ivalue < llv)
			cst[3] = '1';
		else
			cst[3] = '0';
	}
	return ok;
}
char status (char cst [4])					  /* Sends status checking array of simulated MCU bits */
{
	if (cst[0] == '1' || cst[3] == '1')	      /* Stop conditions */
		return 'S';
	else if (cst[1] == '1' || cst[2] == '1')  /* Out to hysteresis limits */
		return 'H';
	else if (cst[1] == '0' && cst[2] == '0')  /* Recover from stop conditions */
		return 'W';
	else
		return '-';							  /* Failsafe for maybe debug */
}

