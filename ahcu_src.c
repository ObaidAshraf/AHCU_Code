#include <16F877A.h>
#fuses xt, nowdt, nolvp, brownout
#use delay (clock = 4Mhz)
#define GLCD_CS1 PIN_C0
#define GLCD_CS2 PIN_C1
#define GLCD_DI  PIN_C2
#define GLCD_RW  PIN_C3
#define GLCD_E   PIN_C4
#define GLCD_RST PIN_C5
#include <glcd.c>
#define use_portb_kbd TRUE
#include <kbd.c>
#include <stdlib.h>
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7)

unsigned int8 k, n, i, g = 0;
unsigned char temp1[6], temp2[6], gluco1[6], gluco2[6], weight1[6], weight2[6], pulse1[6], pulse2[6];
int8 temp3, temp4, gluco3, gluco4, weight3, weight4, pulse3, pulse4;
unsigned char analyze[] = {"Analyzing."};
unsigned char dots[] = {"........"};
char normal[] = {"Normal"};
char low[] = {"Low"};
char high[] = {"High"};
char preDia[] = {"PreDiabetes"};
char Dia[] = {"Diabetes"};
char under[] = {"Under"};
char over[] = {"Over"};
char meds[] = {"Medicines:"};
void drawTable(void);
int readThermometer(void);
int readGluco(void);
int readWeight(void);
int readPulse(void);
void nextGadget(int8);
void analysis(int8, int8, int8, int8);
void result(char, char, char, char);
void presHead(void);
void presTail(void);


void main()
{
  glcd_init(ON);
  delay_ms(10);
  kbd_init();
  delay_ms(10);
  set_tris_a(0);
  set_tris_e(0);

start:
  output_e(0);
//!output_a(0);
  drawTable();
  delay_ms(50);

  g += 1;
  nextGadget(g);

  temp4 = readThermometer();
  delay_ms(10);

  g += 1;
  nextGadget(g);

  gluco4 = readGluco();
  delay_ms(10);

  g += 1;
  nextGadget(g);

  weight4 = readWeight();
  delay_ms(10);

  g += 1;
  nextGadget(g);

  pulse4 = readPulse();
  delay_ms(10);

  output_a(0);

  glcd_fillscreen(OFF);
  delay_ms(200);

  analysis(temp4, gluco4, weight4, pulse4);
  delay_ms(1500);

  glcd_fillscreen(OFF);
  g = 0;
  goto start;

}

void drawTable(void)
{
  char t1[] = {"Temp:"};
  char t2[] = {"Gluco:"};
  char t3[] = {"Weight:"};
  char t4[] = {"Pulse:"};
  char ut1[] = {"F"};
  char ut2[] = {"mg/dL"};
  char ut3[] = {"Kg"};
  char ut4[] = {"BPM"};

  glcd_rect(65, 2, 127, 12, NO, ON);
  glcd_rect(0, 2, 63, 12, NO, ON);
  glcd_rect(65, 18, 127, 28, NO, ON);
  glcd_rect(0, 18, 63, 28, NO, ON);
  glcd_rect(65, 36, 127, 46, NO, ON);
  glcd_rect(0, 36, 63, 46, NO, ON);
  glcd_rect(65, 52, 127, 62, NO, ON);
  glcd_rect(0, 52, 63, 62, NO, ON);
  glcd_text57(67, 4, t1, 1, ON);
  glcd_text57(67, 20, t2, 1, ON);
  glcd_text57(67, 38, t3, 1, ON);
  glcd_text57(67, 54, t4, 1, ON);
  glcd_text57(55, 4, ut1, 1, ON);
  glcd_text57(32, 20, ut2, 1, ON);
  glcd_text57(49, 38, ut3, 1, ON);
  glcd_text57(45, 54, ut4, 1, ON);
}

int readThermometer(void)
{
  for (i = 0; i < 6; i++)
  {
    temp1[i] = 0;
  }
  n = 0;
  while (1)
  {
again:
    k = kbd_getc();
    if (k == '*') goto again;
    if (k != 0)
    {
      temp1[n] = k;
      if (k == '#')
      {
        output_high(PIN_A5);
        delay_ms(500);
        output_low(PIN_A5);
        goto here;
      }
      glcd_text57(5, 4, temp1, 1, ON);
here:
      k = 0;
      delay_ms(250);
      n++;
    }
    delay_ms(1);
    if (temp1[n - 1] == '#')
    {
      unsigned int8 length, count, i;

      length = strlen(temp1);
      count = length - 1;
      for (i = count; i < count + 1; i--)
      {
        temp2[count - i] = temp1[count - i];
      }
      temp3 = atoi(temp2);
      break;
    }
  }
  return temp3;
}

int readGluco(void)
{
  for (i = 0; i < 6; i++)
  {
    gluco1[i] = 0;
  }
  n = 0;
  while (1)
  {
again:
    k = kbd_getc();
    if (k == '*') goto again;
    if (k != 0)
    {
      gluco1[n] = k;
      if (k == '#')
      {
        output_high(PIN_A5);
        delay_ms(500);
        output_low(PIN_A5);
        goto here;
      }
      glcd_text57(5, 20, gluco1, 1, ON);
here:
      k = 0;
      delay_ms(250);
      n++;
    }
    delay_ms(1);
    if (gluco1[n - 1] == '#')
    {
      unsigned int8 length, count, i;
      length = strlen(gluco1);
      count = length - 1;
      for (i = count; i < count + 1; i--)
      {
        gluco2[count - i] = gluco1[count - i];
      }
      gluco3 = atoi(gluco2);
      break;
    }
  }
  return gluco3;
}

int readWeight(void)
{
  for (i = 0; i < 6; i++)
  {
    weight1[i] = 0;
  }
  n = 0;
  while (1)
  {
again:
    k = kbd_getc();
    if (k == '*') goto again;
    if (k != 0)
    {
      weight1[n] = k;
      if (k == '#')
      {
        output_high(PIN_A5);
        delay_ms(500);
        output_low(PIN_A5);
        goto here;
      }
      glcd_text57(5, 38, weight1, 1, ON);;
here:
      k = 0;
      delay_ms(250);
      n++;
    }
    delay_ms(1);
    if (weight1[n - 1] == '#')
    {
      unsigned int8 length, count, i;
      length = strlen(weight1);
      count = length - 1;
      for (i = count; i < count + 1; i--)
      {
        weight2[count - i] = weight1[count - i];
      }
      weight3 = atoi(weight2);
      break;
    }
  }
  return weight3;
}

int readPulse(void)
{
  for (i = 0; i < 6; i++)
  {
    pulse1[i] = 0;
  }
  n = 0;
  while (1)
  {
again:
    k = kbd_getc();
    if (k == '*') goto again;
    if (k != 0)
    {
      pulse1[n] = k;
      if (k == '#')
      {
        output_high(PIN_A4);
        delay_ms(500);
        output_low(PIN_A4);
        delay_ms(500);

        goto here;
      }
      glcd_text57(5, 54, pulse1, 1, ON);;
here:
      k = 0;
      delay_ms(250);
      n++;
    }
    delay_ms(1);
    if (pulse1[n - 1] == '#')
    {
      unsigned int8 length, count, i;
      length = strlen(pulse1);
      count = length - 1;
      for (i = count; i < count + 1; i--)
      {
        pulse2[count - i] = pulse1[count - i];
      }
      pulse3 = atoi(pulse2);
      break;
    }
  }
  return pulse3;
}

void nextGadget(int8 g)
{
  switch (g)
  {
  case 1: //Thermometer
    output_high(PIN_A0);
    delay_ms(1000);
    output_high(PIN_A1);
    break;
  case 2: // Glucometer
    output_a(0x04);
    break;
  case 3: // Weight Machine
    output_a(0x08);
    break;
  case 4: // pulse-oximeter
    output_high(PIN_E0);
    delay_ms(1000);
    output_high(PIN_E1);
    break;
  default:
    output_a(0x00);
    output_e(0x00);
    break;
  }
}


void analysis(int8 t, int8 g, int8 w, int8 p)
{
  char t1[] = {"Temp:"};
  char t2[] = {"Sugar:"};
  char t3[] = {"Weight:"};
  char t4[] = {"Pulse"};

  char tempRes[], glucoRes[], weightRes[], pulseRes[];
  int8 temp, gluco, weight, pulse;
  temp = t;
  gluco = g;
  weight = w;
  pulse = p;

  glcd_text57(68, 25, analyze, 1, ON);
  glcd_text57(0, 25, dots, 1, ON);
  delay_ms(100);
  glcd_fillscreen(OFF);
  glcd_text57(68, 25, analyze, 1, ON);
  glcd_text57(0, 25, dots, 1, ON);
  delay_ms(100);
  glcd_fillscreen(OFF);
  glcd_text57(68, 25, analyze, 1, ON);
  glcd_text57(0, 25, dots, 1, ON);
  delay_ms(100);
  glcd_fillscreen(OFF);
  glcd_text57(68, 25, analyze, 1, ON);
  glcd_text57(0, 25, dots, 1, ON);
  delay_ms(100);
  glcd_fillscreen(OFF);

  glcd_rect(65, 2, 127, 12, NO, ON);
  glcd_rect(0, 2, 63, 12, NO, ON);
  glcd_rect(65, 18, 127, 28, NO, ON);
  glcd_rect(0, 18, 63, 28, NO, ON);
  glcd_rect(65, 36, 127, 46, NO, ON);
  glcd_rect(0, 36, 63, 46, NO, ON);
  glcd_rect(65, 52, 127, 62, NO, ON);
  glcd_rect(0, 52, 63, 62, NO, ON);
  glcd_text57(67, 4, t1, 1, ON);
  glcd_text57(67, 20, t2, 1, ON);
  glcd_text57(67, 38, t3, 1, ON);
  glcd_text57(67, 54, t4, 1, ON);

  if (temp >= 1 && temp <= 99)
  {
    glcd_text57(5, 4, normal, 1, ON);
    tempRes = normal;
  }
  else if (temp > 99)
  {
    glcd_text57(5, 4, high, 1, ON);
    tempRes = high;
  }

  if (gluco >= 70 && gluco <= 100)
  {
    glcd_text57(5, 20, normal, 1, ON);
    glucoRes = normal;
  }
  else if (gluco >= 101 && gluco <= 125)
  {
    glcd_text57(5, 20, preDia, 1, ON);
    glucoRes = preDia;
  }
  else if (gluco > 125)
  {
    glcd_text57(5, 20, Dia, 1, ON);
    glucoRes = Dia;
  }

  if (weight >= 1 && weight < 40)
  {
    glcd_text57(5, 38, under, 1, ON);
    weightRes = under;
  }
  else if (weight >= 40 && weight <= 80)
  {
    glcd_text57(5, 38, normal, 1, ON);
    weightRes = normal;
  }
  else if (weight > 80)
  {
    glcd_text57(5, 38, over, 1, ON);
    weightRes = over;
  }

  if (pulse >= 60 && pulse <= 95 )
  {
    glcd_text57(5, 54, normal, 1, ON);
    pulseRes = normal;
  }
  if (pulse >= 1 && pulse < 60)
  {
    glcd_text57(5, 54, low, 1, ON);
    pulseRes = low;
  }
  else if (pulse > 95)
  {
    glcd_text57(5, 54, high, 1, ON);
    pulseRes = high;
  }

  delay_ms(2500);
  result(tempRes, glucoRes, weightRes, pulseRes);
  output_high(PIN_E2);
  delay_ms(500);
}

void result(char t, char g, char w, char p)
{
  char tRes[], gRes[], wRes[], pRes[];
  char med1[] = {"Panadol"};
  char med2[] = {"Glucophage"};
  char med3[] = {"Gatryl"};
  char med4[] = {"Disprene"};
  char med5[] = {"Ascard"};
  char med6[] = {"Crestor"};

  tRes = t;
  gRes = g;
  wRes = w;
  pRes = p;
  delay_ms(1000);
  glcd_fillscreen(OFF);
  delay_ms(500);

  glcd_text57(65, 2, meds, 1, ON);
  glcd_rect(65, 12, 127, 25, NO, ON);
  glcd_rect(0, 12, 63, 25, NO, ON);
  glcd_rect(65, 30, 127, 43, NO, ON);
  glcd_rect(0, 30, 63, 43, NO, ON);
  glcd_rect(65, 48, 127, 60, NO, ON);
  glcd_rect(0, 48, 63, 60, NO, ON);

  if (gRes == normal && tRes == high && wRes == under)
  {
    glcd_text57(67, 15, med1, 1, ON);
    delay_ms(3000);
    presHead();
    printf("Panadol             |    1 + 0 + 1");
    for (i = 0; i < 12; i++)
    {
      putc(10);
      putc(13);
    }
    presTail();
  }
  else if (gRes == normal && tRes == normal && wRes == normal && pRes == normal)
  {
    glcd_text57(67, 15, med1, 1, ON);
    delay_ms(3000);
    presHead();
    printf("ALL SYMPTOMS ARE NORMAL");
    putc(10);
    putc(13);
    printf("NO NEED FOR ANY MEDICATIONS.");
    for (i = 0; i < 12; i++)
    {
      putc(10);
      putc(13);
    }
    presTail();
  }

  else if (gRes == normal && tRes == normal && wRes == over)
  {
    glcd_text57(67, 15, med1, 1, ON);
    delay_ms(3000);
    presHead();
    printf("Panadol             |   2 + 0 + 2");
    for (i = 0; i < 12; i++)
    {
      putc(10);
      putc(13);
    }
    presTail();
  }
  else if ((gRes == Dia || gRes == preDia) && tRes == normal && wRes == normal)
  {
    glcd_text57(67, 15, med2, 1, ON);
    glcd_text57(67, 33, med3, 1, ON);
    delay_ms(3000);
    presHead();
    printf("Glucophage (500mg)  |    1 + 0 + 1");
    putc(10);
    putc(13);
    putc(10);
    putc(13);
    printf("Gatral (1mg)        |    1 + 0 + 1");
    for (i = 0; i < 9; i++)
    {
      putc(10);
      putc(13);
    }
    presTail();
  }
  else if ((gRes == Dia || gRes == preDia) && tRes == high && wRes == normal && pRes == high)
  {
    glcd_text57(67, 15, med1, 1, ON);
    glcd_text57(67, 33, med2, 1, ON);
    glcd_text57(67, 52, med3, 1, ON);
    glcd_text57(3, 15, med4, 1, ON);
    glcd_text57(3, 33, med5, 1, ON);
    delay_ms(3000);
    presHead();
    printf("Panadol             |    1 + 0 + 1");
    putc(10);
    putc(13);
    putc(10);
    putc(13);
    printf("Glucophage (500mg)  |    1 + 0 + 1");
    putc(10);
    putc(13);
    putc(10);
    putc(13);
    printf("Gatral (1mg)        |    1 + 0 + 1");
    putc(10);
    putc(13);
    putc(10);
    putc(13);
    printf("Disprene CV         |    1 + 0 + 1");
    putc(10);
    putc(13);
    putc(10);
    putc(13);
    printf("Ascard (75mg)       |    1 + 0 + 1");
    putc(10);
    putc(13);
    putc(10);
    putc(13);
    presTail();
//!}
//!if(gRes==Dia && tRes==high && wRes==over && pRes==high)
//!{
//!glcd_text57(67,15,med1,1,ON);
//!glcd_text57(67,33,med2,1,ON);
//!glcd_text57(67,52,med3,1,ON);
//!glcd_text57(3,15,med4,1,ON);
//!glcd_text57(3,33,med5,1,ON);
//!delay_ms(3000);
//!presHead(tRes, gRes, wRes, pRes);
//!printf("Panadol             |    1 + 0 + 1");
//!putc(10);
//!putc(13);
//!putc(10);
//!putc(13);
//!printf("Glucophage (500mg)  |    1 + 0 + 1");
//!putc(10);
//!putc(13);
//!putc(10);
//!putc(13);
//!printf("Gatral (1mg)        |    1 + 0 + 1");
//!putc(10);
//!putc(13);
//!putc(10);
//!putc(13);
//!printf("Disprene CV         |    1 + 0 + 1");
//!putc(10);
//!putc(13);
//!putc(10);
//!putc(13);
//!printf("Ascard (75mg)       |    1 + 0 + 1");
//!putc(10);
//!putc(13);
//!putc(10);
//!putc(13);
//!presTail();
//!}
    delay_ms(1500);
  }
}


void presHead(void)
{
//initialize printer
  putc(27);
  putc(64);
//selecting justification, Center justified
  putc(27);
  putc(97);
  putc(1);
//selecting character font A
  putc(27);
  putc(77);
  putc(0);
//underline ON (2-dot)
  putc(27);
  putc(45);
  putc(2);
  printf("SSUET, Electronics Department");
  putc(10);
  putc(13);
  putc(10);
  putc(13);
  printf("AUTOMATIC HEALTH AND CARE UNIT");
  putc(10);
  putc(13);
  putc(10);
  putc(13);
//underline ON (1-dot)
  putc(27);
  putc(45);
  putc(1);


  printf("PRESCRIPTION");
  putc(13);
  putc(10);
  putc(13);
//underline OFF
  putc(27);
  putc(45);
  putc(0);
//selecting justification, Left justified
  putc(27);
  putc(97);
  putc(0);
  printf("Medicines           |           Dosage");
  putc(10);
  putc(13);
//left justification
  putc(27);
  putc(97);
  putc(0);
  putc(10);
  putc(13);
  putc(10);
  putc(13);
}

void presTail(void)
{
//underline ON (2-dot)
  putc(27);
  putc(45);
  putc(2);
  printf("VISIT A DOCTOR AS EARLIER AS POSSIBLE");
  putc(10);
  putc(13);
  putc(10);
  putc(13);
  printf("Thankyou for using AHCU ...");
  putc(10);
  putc(13);
  putc(10);
  putc(13);
  for (i = 0; i < 35; i++)
  {
    putc(10);
    putc(13);
  }
//selecting cut mode
  putc(29);
  putc(86);
  putc(66);
  putc(0);
//Generating pulse
  putc(27);
  putc(112);
  putc(60);
  putc(50);
  putc(50);
}