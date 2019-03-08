/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <ARMCM0.h>

#define LedToggle 0xf0f0f0f0

#define X0             (*(volatile signed long int*)0x40000000)
#define Y0             (*(volatile signed long int*)0x40000004)
#define Z0             (*(volatile signed long int*)0x40000008)
#define CONTROL_START0 (*(volatile signed long int*)0x4000000c)
#define X_RESULT0      (*(volatile signed long int*)0x40000010)
#define Y_RESULT0      (*(volatile signed long int*)0x40000014)
#define Z_RESULT0      (*(volatile signed long int*)0x40000018)
#define CONTROL_DONE0  (*(volatile signed long int*)0x4000001c)

#define X1             (*(volatile signed long int*)0x40000100)
#define Y1             (*(volatile signed long int*)0x40000104)
#define Z1             (*(volatile signed long int*)0x40000108)
#define CONTROL_START1 (*(volatile signed long int*)0x4000010c)
#define X_RESULT1      (*(volatile signed long int*)0x40000110)
#define Y_RESULT1      (*(volatile signed long int*)0x40000114)
#define Z_RESULT1      (*(volatile signed long int*)0x40000118)
#define CONTROL_DONE1  (*(volatile signed long int*)0x4000011c)

#define X2             (*(volatile signed long int*)0x40000200)
#define Y2             (*(volatile signed long int*)0x40000204)
#define Z2             (*(volatile signed long int*)0x40000208)
#define CONTROL_START2 (*(volatile signed long int*)0x4000020c)
#define X_RESULT2      (*(volatile signed long int*)0x40000210)
#define Y_RESULT2      (*(volatile signed long int*)0x40000214)
#define Z_RESULT2      (*(volatile signed long int*)0x40000218)
#define CONTROL_DONE2  (*(volatile signed long int*)0x4000021c)

#define X3             (*(volatile signed long int*)0x40000300)
#define Y3             (*(volatile signed long int*)0x40000304)
#define Z3             (*(volatile signed long int*)0x40000308)
#define CONTROL_START3 (*(volatile signed long int*)0x4000030c)
#define X_RESULT3      (*(volatile signed long int*)0x40000310)
#define Y_RESULT3      (*(volatile signed long int*)0x40000314)
#define Z_RESULT3      (*(volatile signed long int*)0x40000318)
#define CONTROL_DONE3  (*(volatile signed long int*)0x4000031c)

/*
 * address    description                comment
 * 0x0   32   Prescale Register          Global Prescale register
 * 0x8   32   Timer Enable Register      Enable Timers
 * 0xc   32   Interrupt Enable Register  Enable interrupt p. timer
 * 0x10  64   'mtime' Register           'mtime'
 * 0x18  64   'mtimecmp' Register        'mtimecmp'
 */

#define TIMER_GLO_PRESCALE (*(volatile unsigned long int*) 0x40000400)
#define TIMER_IPENDING     (*(volatile unsigned long int*) 0x40000408)
#define TIMER_IRQ_EN       (*(volatile unsigned long int*) 0x4000040C)
#define TIMER_MTIME        (*(volatile unsigned long long*)0x40000410)
#define TIMER_MTIMECMP0    (*(volatile unsigned long long*)0x40000418)


volatile unsigned int counter; // dummy
volatile unsigned int ii;      // loop iterator
volatile unsigned int trap;    // memory access pattern receiver
volatile unsigned int period;  // time interval for memory access


void Interrupt0_Handler(void)
{
  // Calculate sine and cosine of pi/3
  CONTROL_START0 = 0;
  X0 = 1073741824; // 1 in Q1.30 notation
  Y0 = 0;
  Z0 = 715827883; // angle pi/3, since 2pi = 2^32
  CONTROL_START0 = 1;

  // Calculate sine and cosine of pi/4
  CONTROL_START1 = 0;
  X1 = 1073741824; // 1 in Q1.30 notation
  Y1 = 0;
  Z1 = 536870912; // angle pi/4, since 2pi = 2^32
  CONTROL_START1 = 1;

  // Calculate sine and cosine of pi/6
  CONTROL_START2 = 0;
  X2 = 1073741824; // 1 in Q1.30 notation
  Y2 = 0;
  Z2 = 357913941; // angle pi/6, since 2pi = 2^32
  CONTROL_START2 = 1;

  // Calculate sine and cosine of pi
  CONTROL_START3 = 0;
  X3 = 1073741824; // 1 in Q1.30 notation
  Y3 = 0;
  Z3 = 4294967295; // angle pi, since 2pi = 2^32
  CONTROL_START3 = 1;

  trap=LedToggle; // memory access pattern (toggle LED)

  // Reset timer
  TIMER_MTIME = 0U;

}


int main(void)
{
  period=5000000U;      // period for FPGA implementation; blinking twice a
                        // second for a 10MHz osc in CM0_DS
  /* period=300U;          // period for simulations in Xilinx Vivado tool */

  // Setting Timer interrupt
  TIMER_MTIMECMP0 = period;
  TIMER_MTIME = 0U;
  TIMER_IRQ_EN = 1U;
  TIMER_GLO_PRESCALE = 0U;

  NVIC_SetPriority (0, 1);            // Set Timer priority
  NVIC_EnableIRQ (0);                 // Enable Timer Interrupt

  /* SCB->SCR |= 0x2;  // Enable S1eep-on-exit feature */

  while (1)
    {

    }
}
