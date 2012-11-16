#pragma config(Sensor, S1,     IR,                  sensorI2CCustom)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*
 * IR_Read_02_Modulated.c
 * This file is part of EMC Squirreled IR Test
 *
 * Copyright (C) 2012 - EMC Squirrled
 *
 * EMC Squirreled IR Test is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * EMC Squirreled IR Test is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EMC Squirreled IR Test; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301  USA
 */



#include <HTIRS2-driver.h>

task main()
{
  int irSensor[5];
  int irDirection;
  int i;
  int tone;
  while(true)
  {
    HTIRS2readAllACStrength(IR, irSensor[0], irSensor[1], irSensor[2], irSensor[3], irSensor[4]);
    irDirection = HTIRS2readDCDir(IR);
    nxtDisplayCenteredTextLine(1, "Direction: %2d", irDirection);
    nxtDisplayCenteredTextLine(2, "Sensor 1: %3d", irSensor[0]);
    nxtDisplayCenteredTextLine(3, "Sensor 2: %3d", irSensor[1]);
    nxtDisplayCenteredTextLine(4, "Sensor 3: %3d", irSensor[2]);
    nxtDisplayCenteredTextLine(5, "Sensor 4: %3d", irSensor[3]);
    nxtDisplayCenteredTextLine(6, "Sensor 5: %3d", irSensor[4]);

    tone = 0;
    for(i = 0; i < 5; i++)
    {
      tone += irSensor[i];
    }
    tone *= 10;

    PlayImmediateTone(tone, 1);

    if(irSensor[1] != 0 && irSensor[2] != 0)
    {
      if(abs(irSensor[1] - irSensor[2]) < 2) PlayImmediateTone(5000, 1);
    }
  }
}
