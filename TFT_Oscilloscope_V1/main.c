/* Defines the base address of the memories and peripherals */
#include "inc/hw_memmap.h"

/* Defines the common types and macros */
#include "inc/hw_types.h"

#include "inc/hw_timer.h"

#include "inc/hw_ints.h"

/* Defines and Macros for GPIO API */
#include "driverlib/gpio.h"

/* Prototypes for the system control driver */
#include "driverlib/sysctl.h"

/* ADC headers for using the ADC driver functions. */
#include "driverlib/adc.h"

/* Defines and Macros for the UART. */
#include "driverlib/uart.h"

/* Prototypes for the NVIC Interrupt Controller Driver. */
#include "driverlib/interrupt.h"

/* Prototypes for the UART console functions. */
#include "utils/uartstdio.h"

#include "IQmath/IQmathLib.h"

#include "driverlib/timer.h"

#include "driverlib/ELT240320TP.h"

//#define M 50
//#define GLOBAL_Q 8
#define N 514

int r01=1,r02,r03,r04,r05,r06,r07,r08;

/*
void InitConsole(void)
{
	// Enable GPIO port A which is used for UART0 pins.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    // Make the UART pins be peripheral controlled.
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // Initialize the UART for console I/O.
    UARTStdioInit(0);
}
*/

unsigned long i=0,k,a2=1,a1=0,g_count=1,s2,s3,j=0,max=0,min=127,count1=0,count2=0,count3=0;
long diff,diff1,diff2;
unsigned long x[320],savg=0,avg1,f,fs=0,n1[10],avg,flag=0;
unsigned long max1,amp3,min1;
unsigned long fs1[11]={139000,40000,20000,17000,13000,10700,9000,7550,6650,5950,5400};

float max2,min2;
int maxf,maxd,maxf1,maxf2,minf,mind,minf1,minf2,f1=0,f2=0,f3=0,f4=0;

int main(void)
{

unsigned long temp[2];
/*Set the clocking to directly run from the crystal at 50MHz*/

SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);
SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

/* Set the clock for the GPIO Port A,B,C,D */
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);


//GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE,GPIO_PIN_4);
//GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_4,GPIO_PIN_4);
		GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5);
		GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
		GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4);
		GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0);

		//Switch S2 and S3 as input
		GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_1|GPIO_PIN_0);

	/*Configure GPIO pad with internal pull-up enabled*/
	GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
	GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_1, GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);

    //InitConsole();
    IntMasterEnable();
    SysCtlADCSpeedSet(SYSCTL_ADCSPEED_1MSPS);

    /*Configure ADC Peripheral*/

    /*Configure ADC Sequence*/
    ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
  	ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH3);
   	ADCSequenceStepConfigure(ADC0_BASE, 0, 1, ADC_CTL_CH7| ADC_CTL_IE | ADC_CTL_END);

	/*Enable ADC sequence*/
	ADCSequenceEnable(ADC0_BASE, 0);

	/*Clear ADC Interrupt*/
	ADCIntClear(ADC0_BASE, 0);

        	TSLCDRst();
		    TSLCDInit();
		    setBitBL;
		    TSLCDFillRect(0,TS_SIZE_X-1,0,TS_SIZE_Y-1,TS_COL_BLUE,TS_MODE_NORMAL);
		    TSLCDSetFontColor(TS_COL_WHITE);
		    TSLCDPrintStr(1,0,"             OSCILLOSCOPE  ",TS_MODE_NORMAL);
		    TSLCDPrintStr(3,0,"Vmax:",TS_MODE_NORMAL);
		    TSLCDPrintStr(4,0,"Vmin:",TS_MODE_NORMAL);
		    TSLCDPrintStr(3,36,"RUN",TS_MODE_NORMAL);
		    TSLCDPrintStr(4,16,"FREQ:",TS_MODE_NORMAL);
		    //GND Reference
		    TSLCDFillRect(0,TS_SIZE_X-1,7,7,TS_COL_PURPLE,TS_MODE_NORMAL);
		    for(i=6;i>=3;i--)
		    {
		    TSLCDFillRect(6-i,i,i,i,TS_COL_PURPLE,TS_MODE_NORMAL);
		    }

		    //3.3 V reference
		    TSLCDFillRect(0,TS_SIZE_X-1,136,136,TS_COL_PURPLE,TS_MODE_NORMAL);
		    for(i=137;i<=145;i++)
		       {
		    	TSLCDFillRect(3,3,i,i,TS_COL_PURPLE,TS_MODE_NORMAL);
		       }
		    for(i=0;i<=3;i++)
		       {
		      	TSLCDFillRect(i,6-i,145+i,145+i,TS_COL_PURPLE,TS_MODE_NORMAL);
		       }
		    TSLCDSetFontColor(TS_COL_PURPLE);
		    TSLCDPrintStr1(7,86,"3.27V",TS_MODE_NORMAL);
		    TSLCDSetFontColor(TS_COL_WHITE);

		    ADCProcessorTrigger(ADC0_BASE, 0);
     		while(!ADCIntStatus(ADC0_BASE, 0, false))
				{
	    		}
	   	    ADCIntClear(ADC0_BASE, 0);
		    ADCSequenceDataGet(ADC0_BASE, 0, temp);
		    //a1=temp[0]/8;
		    a2=temp[1]/100;
		    for(i=0;i<=10;i++)
		    {
		    	if(a2==i)
		    		{
		    		a2=i*40;
		    		fs=fs1[i];
		    		}
		    }
		    if(a2==0)
		    	a2=1;

while(1)
{
           //Switch S2 to Stop and Switch S3 to resume

           s2 = GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0);
           if(s2==0)
             {
        	   a1=1;
        	   TSLCDFillRect(288,319,172,192,TS_COL_BLUE,TS_MODE_NORMAL);
               TSLCDPrintStr(3,36,"STOP",TS_MODE_NORMAL);
             }
           while(a1==1)
           {
 	         s3 = GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_1);
 	         if(s3==0)
 	           {
 	        	 a1=0;
 	        	TSLCDFillRect(288,319,172,192,TS_COL_BLUE,TS_MODE_NORMAL);
 	        	TSLCDPrintStr(3,36,"RUN",TS_MODE_NORMAL);
 	           }
     	   }


	       max=0;
	       min=128;
	       flag=0;
	       j=0;
	       count1=0;
	       count2=0;
	       count3=0;

	       ADCProcessorTrigger(ADC0_BASE, 0);
    	   while(!ADCIntStatus(ADC0_BASE, 0, false))
				{
				}
	       ADCIntClear(ADC0_BASE, 0);
	       ADCSequenceDataGet(ADC0_BASE, 0, temp);
	       //a1=temp[0]/8;
	       a2=temp[1]/100;

	       for(i=0;i<=10;i++)
	          {
	          if(a2==i)
	       		{
	        	  fs=fs1[i];
	        	  a2=i*40;
	            }
	       	  }
	        if(a2==0)
	       	  a2=1;


	       for(i=0;i<=319*a2;i++)
           {
	    	 if(i%a2==0)
	    	 {
	    	 ADCProcessorTrigger(ADC0_BASE, 0);
     			while(!ADCIntStatus(ADC0_BASE, 0, false))
    			{
        		}
             ADCIntClear(ADC0_BASE, 0);
             ADCSequenceDataGet(ADC0_BASE, 0, temp);
             x[i/a2]=temp[0]/8;
             if(x[i/a2]>max)
                max=x[i/a2];
              if(x[i/a2]<min)
             	min=x[i/a2];
	    	}
	        //a2=temp[1]/2;
	       }

           max1 = (max*330)/128;
           maxd=(max*3.3)/128;
           maxf=max1-(maxd*100);
           maxf1=maxf/10;
           maxf2=maxf%10;



//         TSLCDFillRect(90,150,192,208,TS_COL_BLUE,TS_MODE_NORMAL);
           TSLCDFillRect(40,80,172,192,TS_COL_BLUE,TS_MODE_NORMAL);
           TSLCDPrintNumber(3,5,maxd,TS_MODE_NORMAL);
           TSLCDPrintStr(3,6,".",TS_MODE_NORMAL);
           TSLCDPrintNumber(3,7,maxf1,TS_MODE_NORMAL);
           TSLCDPrintNumber(3,8,maxf2,TS_MODE_NORMAL);
           TSLCDPrintStr(3,9,"V",TS_MODE_NORMAL);

           min1= (min*330)/128;
           mind=(min*3.3)/128;
           minf=min1-(mind*100);
           minf1=minf/10;
           minf2=minf%10;

           for(i=0;i<=9;i++)
              {
              n1[i]=0;
              }
           avg1=(min+max)/2;

           TSLCDFillRect(40,80,151,171,TS_COL_BLUE,TS_MODE_NORMAL);
           TSLCDPrintNumber(4,5,mind,TS_MODE_NORMAL);
           TSLCDPrintStr(4,6,".",TS_MODE_NORMAL);
           TSLCDPrintNumber(4,7,minf1,TS_MODE_NORMAL);
           TSLCDPrintNumber(4,8,minf2,TS_MODE_NORMAL);
           TSLCDPrintStr(4,9,"V",TS_MODE_NORMAL);

           for(i=0;i<=318;i++)
           {
           diff= x[i+1]-x[i];

           if((x[i]<avg1)&&(x[i+1]>=avg1)&&(!flag))
             {
              n1[j]=i;
              j++;
             }
           if(j==9)
             {
         	  flag=1;
             //j=0;
             }


           TSLCDFillRect(i,i,8,135,TS_COL_BLUE,TS_MODE_NORMAL);
           TSLCDFillRect(i+1,i+1,8,135,TS_COL_BLUE,TS_MODE_NORMAL);
              if(diff>=0)
              {
			    TSLCDFillRect(i,i,x[i]+8,x[i]+diff+8,TS_COL_WHITE,TS_MODE_NORMAL);
			    TSLCDFillRect(i+1,i+1,x[i+1]+8,x[i+1]+8,TS_COL_WHITE,TS_MODE_NORMAL);
              }

              else if(diff<0)
              {

               TSLCDFillRect(i,i,x[i]+diff+8,x[i]+8,TS_COL_WHITE,TS_MODE_NORMAL);
               TSLCDFillRect(i+1,i+1,x[i+1]+8,x[i+1]+8,TS_COL_WHITE,TS_MODE_NORMAL);
              }

           }



           for(i=0;i<=9;i++)
           {
             if(n1[i+1]!=0)
           {
          	 count2+=(n1[i+1]-n1[i]);
          	 count3++;
           }
           }

           //UARTprintf("%04d\n",count3);
           avg=count2/(count3);

           //f=fs*avg;
           savg+=avg;

          k++;
          if(k==10)
          {
          	f=fs*10/savg;
          	//f+=60;
          	k=0;
              //f=fs*savg/(savg+n1[1]);
            //  UARTprintf("avg::");
             // UARTprintf("%04d\n",f);
              //UARTprintf("freq::");
              //UARTprintf("%04d\n",f);
              savg=0;
              TSLCDFillRect(168,260,151,180,TS_COL_BLUE,TS_MODE_NORMAL);
              f1=f%10;
              f=f/10;
              f2=f%10;
              f=f/10;
              f3=f%10;
              f=f/10;
              f4=f%10;

              if(f4==0)
              {
              TSLCDPrintNumber(4,21,f3,TS_MODE_NORMAL);
              TSLCDPrintNumber(4,22,f2,TS_MODE_NORMAL);
              TSLCDPrintNumber(4,23,f1,TS_MODE_NORMAL);
              TSLCDPrintStr(4,24," Hz",TS_MODE_NORMAL);
              }

              else if(f4!=0)
              {

            	  TSLCDPrintNumber(4,21,f4,TS_MODE_NORMAL);
            	  TSLCDPrintNumber(4,23,f3,TS_MODE_NORMAL);
            	  TSLCDPrintNumber(4,24,f2,TS_MODE_NORMAL);
            	  TSLCDPrintNumber(4,25,f1,TS_MODE_NORMAL);
            	  TSLCDPrintStr(4,26," kHz",TS_MODE_NORMAL);
            	  TSLCDPrintStr(4,22,".",TS_MODE_NORMAL);
              }

              //TSLCDPrintNumber(3,8,maxf2,TS_MODE_NORMAL);
              //TSLCDPrintStr(3,9,"V",TS_MODE_NORMAL);


          }

            //a1=a2;

}

}

