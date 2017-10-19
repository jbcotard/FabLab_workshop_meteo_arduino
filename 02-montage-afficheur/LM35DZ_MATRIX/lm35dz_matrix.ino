#include <MD_MAX72xx.h>
#define     MAX_DEVICES   4

#define     CLK_PIN     13
#define     DATA_PIN    11
#define     CS_PIN      10

MD_MAX72XX mx = MD_MAX72XX(DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);


// Text parameters
#define  CHAR_SPACING  1 // pixels between characters


int valeur_brute;
int total = 0;
int nbmesures = 15; // nb secondes entre chaque affichage


void setup()
{
  mx.begin();
  //https://playground.arduino.cc/Main/LM35HigherResolution
  
  // Améliore la précision de la mesure en réduisant la plage de mesure
  analogReference(INTERNAL); // 2.5v sur leonardo

  mx.control(MD_MAX72XX::INTENSITY, 1);

  // premiÃ¨re lecture - un peu bidon
  
  valeur_brute = analogRead(A0); 
  total = valeur_brute * nbmesures;
  
}

void loop()
{
  //float temperature_celcius = valeur_brute * (5.5 / 1023.0 * 100.0); // Sans analogReference(INTERNAL); 
  float temperature_celcius = valeur_brute * (1.1 / 1023.0 * 100.0);
  //float temperature_celcius = (total/nbmesures)/4.096;
  //float temperature_celcius = (total / nbmesures) / 4.185; // TÂ° Ludovic
  String mystring =  "  " + String(temperature_celcius) ;

  mystring.remove(mystring.length() - 1);

  char charBuf[50];
  mystring.toCharArray(charBuf, 50);
  printText(0, MAX_DEVICES - 1, charBuf);

  // Symbole "Â°"
  mx.setPoint(0,3,true);
  mx.setPoint(2,3,true);
  mx.setPoint(1,4,true);
  mx.setPoint(1,2,true);
 
  total = 0;
  for (int i=0;i<nbmesures; i++){
    mx.setPoint(0, 0, true);
    delay(100);
    valeur_brute = analogRead(A0); 
    total = total + valeur_brute;
    mx.setPoint(0, 0, false);
    delay(900);
  }



}



void printText(uint8_t modStart, uint8_t modEnd, char *pMsg)
// Print the text string to the LED matrix modules specified.
// Message area is padded with blank columns after printing.
{
  uint8_t   state = 0;
  uint8_t   curLen;
  uint16_t  showLen;
  uint8_t   cBuf[8];
  int16_t   col = ((modEnd + 1) * COL_SIZE) - 1;

  mx.control(modStart, modEnd, MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);

  do     // finite state machine to print the characters in the space available
  {
    switch (state)
    {
      case 0: // Load the next character from the font table
        // if we reached end of message, reset the message pointer
        if (*pMsg == '\0')
        {
          showLen = col - (modEnd * COL_SIZE);  // padding characters
          state = 2;
          break;
        }

        // retrieve the next character form the font file
        showLen = mx.getChar(*pMsg++, sizeof(cBuf) / sizeof(cBuf[0]), cBuf);
        curLen = 0;
        state++;
      // !! deliberately fall through to next state to start displaying

      case 1: // display the next part of the character
        mx.setColumn(col--, cBuf[curLen++]);

        // done with font character, now display the space between chars
        if (curLen == showLen)
        {
          showLen = CHAR_SPACING;
          state = 2;
        }
        break;

      case 2: // initialize state for displaying empty columns
        curLen = 0;
        state++;
      // fall through

      case 3: // display inter-character spacing or end of message padding (blank columns)
        mx.setColumn(col--, 0);
        curLen++;
        if (curLen == showLen)
          state = 0;
        break;

      default:
        col = -1;   // this definitely ends the do loop
    }
  } while (col >= (modStart * COL_SIZE));

  mx.control(modStart, modEnd, MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
}

