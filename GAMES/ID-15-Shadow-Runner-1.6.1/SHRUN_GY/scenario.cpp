#include <avr/pgmspace.h>
#include <Gamby.h>

#include "stage.h"

extern char stage;
extern char scenario;
extern byte showitems;

void scenario1()
{
  switch (stage)
    {
      case 0:
        showitems = showitems | B00000001;
        break;
  } 
  stage +=6;
  if (stage >96)
  {
    stage = 0;
    scenario = 0;
  }
}

void scenario2()
{
  switch (stage)
    {
      case 0:
        showitems = showitems | B00000100;
        break;
  } 
  stage +=6;
  if (stage >96)
  {
    stage = 0;
    scenario = 0;
  }
}


void scenario3()
{
  switch (stage)
    {
      case 0:
        showitems = showitems | B00001000;
        break;
  } 
  stage +=6;
  if (stage >96)
  {
    stage = 0;
    scenario = 0;
  }
}


void scenario4()
{
  switch (stage)
    {
      case 0:
        showitems = showitems | B00000001;
        break;
      case 54:
        showitems = showitems | B00000010;
        break;
  } 
  stage +=6;
  if (stage >96)
  {
    stage = 0;
    scenario = 0;
  }
}

void scenario5()
{
  switch (stage)
    {
      case 0:
        showitems = showitems | B00000100;
        break;
      case 72:
        showitems = showitems | B00010000;
        break;
  } 
  stage +=6;
  if (stage >96)
  {
    stage = 0;
    scenario = 0;
  }
}

void scenario6()
{
  switch (stage)
    {
      case 0:
        showitems = showitems | B00001000;
        break;
      case 18:
        break;
      case 36:
        break;
      case 54:
        break;
      case 72:
        showitems = showitems | B00000100;
        break;
      case 96:
        break;
  } 
  stage +=6;
  if (stage >96)
  {
    stage = 0;
    scenario = 0;
  }
}

void scenario7()
{
  switch (stage)
    {
      case 0:
        showitems = showitems | B00000001;
        break;
      case 36:
        showitems = showitems | B00000100;
        break;
      case 72:
        showitems = showitems | B00000010;
        break;
  } 
  stage +=6;
  if (stage >96)
  {
    stage = 0;
    scenario = 0;
  }
}


void scenario8()
{
  switch (stage)
    {
      case 0:
      showitems = showitems | B00000100;
      showitems = showitems | B00001000;
        break;
      case 18:
        break;
      case 36:
        break;
      case 54:
      showitems = showitems | B00000001;
        break;
      case 72:
        break;
      case 96:
      showitems = showitems | B00000010;
        break;
  } 
  stage +=6;
  if (stage >96)
  {
    stage = 0;
    scenario = 0;
  }
}


void scenario9()
{
  switch (stage)
    {
      case 0:
      showitems = showitems | B00000001;
        break;
      case 18:
        break;
      case 36:
        break;
      case 54:
      showitems = showitems | B00000100;
      showitems = showitems | B00001000;
        break;
      case 72:
        break;
      case 96:
      showitems = showitems | B00000010;
        break;
  } 
  stage +=6;
  if (stage >96)
  {
    stage = 0;
    scenario = 0;
  }
}



void scenario1EL()
{
  switch (stage)
    {
      case 36:
        showitems = showitems | B00100000;
        break;
  } 
  stage +=6;
  if (stage >96)
  {
    stage = 0;
    scenario = 0;
  }
}

void scenario2EL()
{
  switch (stage)
    {
      case 36:
        showitems = showitems | B00100000;
        break;
  } 
  stage +=6;
  if (stage >96)
  {
    stage = 0;
    scenario = 0;
  }
}

void scenario3EL()
{
  switch (stage)
    {
      case 18:
        showitems = showitems | B00000100;
        break;
      case 72:
        showitems = showitems | B00100000;
        break;
      case 96:
        showitems = showitems | B00000100;
        break;
  } 
  stage +=6;
  if (stage >96)
  {
    stage = 0;
    scenario = 0;
  }
}

void scenario4EL()
{
  switch (stage)
    {
      case 0:
        showitems = showitems | B00000100;
        break;
      case 36:
        showitems = showitems | B00100000;
        break;
      case 96:
        showitems = showitems | B00010000;
        break;
  } 
  stage +=6;
  if (stage >96)
  {
    stage = 0;
    scenario = 0;
  }
}
