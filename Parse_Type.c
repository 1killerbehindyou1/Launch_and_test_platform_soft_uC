/*
 * CFile1.c
 *
 * Created: 1/11/2020 8:35:59 AM
 *  Author: plesn
 */ 
# include "Parse_Type.h"

	
	char* IntToChar(int t)										//konwersja typu int na typ char
	{
		
		for(int i=0;i<=2;i++)
		{
			floatString[i]= 48 + t/pow(10,2-i);
			t -= (floatString[i] - 48)*pow(10,2-i);
		}
		floatString[3]= 0;
		return floatString;
	}
	
	char* DecToBin(int x)								//konwersja typu int na posta? binarn?
	{
		
		int i, k;
		char bin[8];

		for (i = 7; i >= 0; i--)
		{
			k = x >> i;

			if (k & 1)
			bin[7-i] = '1';
			else
			bin[7-i] = '0';
		}
		bin[8] = '\0';
		return bin;
		
	}
	
	char* FloatToChar(float f)									//konwersja typu float na typ char
	{
		
		int intPART =(int) 100*f;						//rzutowanie na typ int

		int e= 0,w=100;
		while((intPART/w )>1)							//ustalenie ilosci cyfr w zmiennej pomn o 1
		w *=10;

		

		while(intPART >0)
		{
			if(w==10)
			{

				floatString[e]= '.';
				e++;
			}
			floatString[e]=(int)intPART/w + 48;
			intPART -=w*(floatString[e]-48);

			e++;
			w /=10;

		}
		floatString[e]= 0;
		
		return floatString;
	}
	
	float CharToFloat(char * RX_buf)							//konwersja   typu char na typ float
	{
		float a=0;
		int o=0;

		while(RX_buf[o+1] !='.' )						//okreslenie ilosci miejsc ca?kowiotych
		{
			o++;
			
		}

		for(int i=1;i <=o+5;i++)						//0 elem tabl to #
		{


			o--;
			if(RX_buf[i] =='.' )
			i++;

			a += (RX_buf[i]-48)*pow(10,o) ;
		}

		return a;
	}