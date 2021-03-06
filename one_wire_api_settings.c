/*
   Licensed under the EUPL V.1.1, Lizenziert unter EUPL V.1.1 
*/
/*
 * one_wire_api_settings.c
 *
 *  Created on: May 3, 2010
 *      Author: zumbruch
 */


#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "api_define.h"
#include "api_global.h"

#include "api_debug.h"#include "mem-check.h"
#include "one_wire_temperature.h"
#include "one_wire_adc.h"
#include "one_wire_api_settings.h"

static const char filename[] 		PROGMEM = __FILE__;

/* max length defined by MAX_LENGTH_PARAMETER */
static const char owiApiCommandKeyword00[] PROGMEM = "common_temp_convert";
static const char owiApiCommandKeyword01[] PROGMEM = "common_adc_convert";

//const showCommand_t showCommands[] PROGMEM =
//{
//      { (int8_t(*)(struct uartStruct)) showFreeMemNow, owiApiCommandKeyword00 },
//      { (int8_t(*)(struct uartStruct)) showUnusedMemNow, owiApiCommandKeyword01 },
//      { (int8_t(*)(struct uartStruct)) showUnusedMemStart, owiApiCommandKeyword02 }
//};

const char* const owiApiCommandKeywords[] PROGMEM = {
        owiApiCommandKeyword00,
        owiApiCommandKeyword01};


int8_t owiApi(struct uartStruct *ptr_uartStruct)
{
    uint8_t index;
    //int8_t (*func)(struct uartStruct);
    //struct showCommand_t

     printDebug_p(debugLevelEventDebug, debugSystemOWIApiSettings, __LINE__, filename, PSTR("begin"));

    switch(ptr_uartStruct->number_of_arguments)
    {
    case 0:
        for (index = 0; index < owiApiCommandKeyNumber_MAXIMUM_NUMBER; index++)
        {
            printDebug_p(debugLevelEventDebug, debugSystemOWIApiSettings, __LINE__, filename, PSTR("all begin %i"), index);
            ptr_uartStruct->number_of_arguments = 1;

            clearString(setParameter[1], MAX_LENGTH_PARAMETER);
            snprintf_P(setParameter[1],MAX_LENGTH_PARAMETER -1, (PGM_P) (pgm_read_word( &(owiApiCommandKeywords[index]))));

            printDebug_p(debugLevelEventDebug, debugSystemOWIApiSettings, __LINE__, filename, PSTR("recursive call of %s with parameter \"%s\" (%p)"), __func__, &setParameter[1][0], &setParameter[1][0]);

            owiApi(ptr_uartStruct);

            printDebug_p(debugLevelEventDebug, debugSystemOWIApiSettings, __LINE__, filename, PSTR("all end %i"), index);

            ptr_uartStruct->number_of_arguments = 0;
        }
        break;
    default: /* index > 0 */
        index = apiFindCommandKeywordIndex(setParameter[1], owiApiCommandKeywords, owiApiCommandKeyNumber_MAXIMUM_NUMBER);

        switch (index)
        {
           case owiApiCommandKeyNumber_COMMON_ADC_CONVERSION:
           case owiApiCommandKeyNumber_COMMON_TEMPERATURE_CONVERSION:
              owiApiFlag(ptr_uartStruct, index);
              break;
           default:
              CommunicationError_p(ERRA, dynamicMessage_ErrorIndex, FALSE, PSTR("owiApi:invalid argument"));
              return 1;
              break;
        }
        break;
    }

     printDebug_p(debugLevelEventDebug, debugSystemOWIApiSettings, __LINE__, filename, PSTR("end"));

    return 0;
}

int8_t owiApiFlag(struct uartStruct * ptr_uartStruct, uint8_t index)
{
   uint8_t flag = 0;
   if (1 < ptr_uartStruct->number_of_arguments)  /*set value*/
   {
      flag = ( 0 != strtoul(setParameter[2], &ptr_setParameter[2], 16));
   }

   switch (index)
   {
      case owiApiCommandKeyNumber_COMMON_ADC_CONVERSION:
    	  if (1 < ptr_uartStruct->number_of_arguments)  /*set value*/
    	  {
    		  owiUseCommonAdcConversion_flag = flag;
    	  }
    	  flag = owiUseCommonAdcConversion_flag;
         break;
      case owiApiCommandKeyNumber_COMMON_TEMPERATURE_CONVERSION:
    	  if (1 < ptr_uartStruct->number_of_arguments)  /*set value*/
    	  {
    		  owiUseCommonTemperatureConversion_flag = flag;
    	  }
         flag = owiUseCommonTemperatureConversion_flag;
         break;
      default:
         CommunicationError_p(ERRA, dynamicMessage_ErrorIndex, FALSE, PSTR("owiApiFlag:invalid argument"));
         return 1;
         break;
   }

   createExtendedSubCommandReceiveResponseHeader(ptr_uartStruct, commandKeyNumber_OWSA, index, owiApiCommandKeywords);
   snprintf_P(uart_message_string,BUFFER_SIZE -1, PSTR("%s%i"), uart_message_string, flag);
   strncat_P(uart_message_string,((flag)?PSTR(" (TRUE)"):PSTR(" (FALSE)")),BUFFER_SIZE -1);
   UART0_Send_Message_String_p(NULL,0);

   return 0;
}
