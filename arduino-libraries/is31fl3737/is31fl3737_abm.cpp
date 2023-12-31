#ifdef __cplusplus
extern "C" {
#endif

#include "is31fl3737_abm.h"

void
IS31FL3737_SetLEDMode (IS31FL3737 *device, uint8_t cs, uint8_t sw, IS31FL3737_LED_MODE mode)
{
  uint8_t offset;
  
  // Check SW boundaries.
  if (sw < IS31FL3737_SW)
  {
    // Check CS boundaries.
    if (cs < IS31FL3737_CS)
    {
      // Set mode of individual LED.
      // Calculate LED offset.
      offset = sw * IS31FL3737_CS + cs;
      // Write LED mode to device register.
      IS31FL3737_WritePagedReg (device, IS31FL3737_LEDABM + offset, mode);
    }
    else
    {
      // Set mode of full row selected by SW.
      for (cs = 0; cs < IS31FL3737_CS; cs++)
      {
        // Calculate LED offset.
        offset = sw * IS31FL3737_CS + cs;
        // Write LED mode to device register.
        IS31FL3737_WritePagedReg (device, IS31FL3737_LEDABM + offset, mode);
      }
    }
  }
  else
  {
    // Check CS boundaries.
    if (cs < IS31FL3737_CS)
    {
      // Set mode of full column selected by CS.
      for (sw = 0; sw < IS31FL3737_SW; sw++)
      {
          // Calculate LED offset.
          offset = sw * IS31FL3737_CS + cs;
          // Write LED mode to device register.
          IS31FL3737_WritePagedReg (device, IS31FL3737_LEDABM + offset, mode);
      }
    }
    else
    {
      // Set mode of all LEDs.
      for (sw = 0; sw < IS31FL3737_SW; sw++)
      {
        for (cs = 0; cs < IS31FL3737_CS; cs++)
        {
          // Calculate LED offset.
          offset = sw * IS31FL3737_CS + cs;
          // Write LED mode to device register.
          IS31FL3737_WritePagedReg (device, IS31FL3737_LEDABM + offset, mode);
        }
      }
    }
  }
}

void
IS31FL3737_ConfigABM (IS31FL3737 *device, IS31FL3737_ABM_NUM n, IS31FL3737_ABM *config)
{
  // Set fade in and fade out time.
  IS31FL3737_WritePagedReg (device, n, config->T1 | config->T2);
  // Set hold and off time.
  IS31FL3737_WritePagedReg (device, n + 1, config->T3 | config->T4);
  // Set loop begin/end time and high part of loop times.
  IS31FL3737_WritePagedReg (device, n + 2, config->Tend | config->Tbegin | ((config->Times >> 8) & 0x0F));
  // Set low part of loop times.
  IS31FL3737_WritePagedReg (device, n + 3, config->Times & 0xFF);
}

void
IS31FL3737_StartABM (IS31FL3737 *device)
{
  // Clear B_EN bit in configuration register.
  IS31FL3737_WritePagedReg (device, IS31FL3737_CR, IS31FL3737_CR_SSD);
  // Set B_EN bit in configuration register.
  IS31FL3737_WritePagedReg (device, IS31FL3737_CR, IS31FL3737_CR_BEN | IS31FL3737_CR_SSD);
  // Write 0x00 to Time Update Register to update ABM settings.
  IS31FL3737_WritePagedReg (device, IS31FL3737_TUR, 0x00);
}

#ifdef __cplusplus
}
#endif