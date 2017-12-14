/**
******************************************************************************
* @file    STM32LowPower.cpp
* @author  WI6LABS
* @version V1.0.0
* @date    11-December-2017
* @brief   Provides a STM32 Low Power interface with Arduino
*
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*   1. Redistributions of source code must retain the above copyright notice,
*      this list of conditions and the following disclaimer.
*   2. Redistributions in binary form must reproduce the above copyright notice,
*      this list of conditions and the following disclaimer in the documentation
*      and/or other materials provided with the distribution.
*   3. Neither the name of STMicroelectronics nor the names of its contributors
*      may be used to endorse or promote products derived from this software
*      without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************
*/

#include "STM32LowPower.h"

STM32LowPower LowPower;


STM32LowPower::STM32LowPower()
{
  _configured = false;
}

void STM32LowPower::begin(void)
{
  LowPower_init();
  _configured = true;
}

void STM32LowPower::idle(void)
{
  LowPower_sleep();
}

void STM32LowPower::programRtcWakeUp(uint32_t millis)
{
  int epoc;
  uint32_t sec;

  if(millis > 0) {
    if (!rtcLowPower.isConfigured()){
      //Enable RTC
      rtcLowPower.begin(HOUR_24);
    }
    
    // convert millisecond to second
    sec = millis / 1000;
    // Minimum is 1 second
    if (sec == 0){
      sec = 1;
    }

    epoc = rtcLowPower.getEpoch();
    rtcLowPower.setAlarmEpoch( epoc + sec );
  }

}

void STM32LowPower::idle(uint32_t millis)
{
  programRtcWakeUp(millis);
  LowPower_sleep();
}

void STM32LowPower::sleep(void)
{
  LowPower_stop();
}

void STM32LowPower::sleep(uint32_t millis)
{
  programRtcWakeUp(millis);
  LowPower_stop();
}

void STM32LowPower::deepSleep(void)
{
  LowPower_standby();
}

void STM32LowPower::deepSleep(uint32_t millis)
{
  programRtcWakeUp(millis);
  LowPower_standby();
}

void STM32LowPower::shutdown(void)
{
  LowPower_shutdown();
}

void STM32LowPower::shutdown(uint32_t millis)
{
  programRtcWakeUp(millis);
  LowPower_shutdown();
}

void STM32LowPower::attachInterruptWakeup(uint32_t pin, voidFuncPtr callback, uint32_t mode)
{
  // all GPIO for idle (smt32 sleep) and sleep (stm32 stop)
  attachInterrupt(pin, callback, mode);

  // If Gpio is a Wake up pin activate it for deepSleep (standby stm32) and shutdown
  LowPower_EnableWakeUpPin(pin, mode);
}

void STM32LowPower::enableWakeupFrom(HardwareSerial *serial, voidFuncPtr callback)
{
  LowPower_EnableWakeUpUart(serial->getSerialPointer(), callback);
}

void STM32LowPower::enableWakeupFrom(TwoWire *wire, voidFuncPtr callback)
{

}

void STM32LowPower::enableWakeupFrom(STM32RTC *rtc, voidFuncPtr callback)
{
  rtc->attachInterrupt(callback);
}
