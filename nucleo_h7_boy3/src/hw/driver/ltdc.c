/*
 * ltdc.c
 *
 *  Created on: 2019. 6. 23.
 *      Author: Baram
 */




#include "ltdc.h"
#include "sdram.h"



#define FRAME_BUF_ADDR        0x24000000

#define LCD_WIDTH             ((uint16_t)320)          /* LCD PIXEL WIDTH            */
#define LCD_HEIGHT            ((uint16_t)240)          /* LCD PIXEL HEIGHT           */


#define LCD_HSYNC             ((uint16_t)70)   /* Horizontal synchronization */
#define LCD_HBP               ((uint16_t)0)   /* Horizontal back porch      */
#define LCD_HFP               ((uint16_t)0)   /* Horizontal front porch     */


#define LCD_VSYNC             ((uint16_t)13)   /* Vertical synchronization   */
#define LCD_VBP               ((uint16_t)1)    /* Vertical back porch        */
#define LCD_VFP               ((uint16_t)1)    /* Vertical front porch       */







LTDC_HandleTypeDef hltdc;



bool ltdcInit(void)
{
  bool ret = true;

  RCC_PeriphCLKInitTypeDef periph_clk_init_struct;



/* LTDC Initialization -------------------------------------------------------*/

  /*## LTDC Clock Configuration ###########################################*/
  /* RK043FN48H LCD clock configuration */
  /* LCD clock configuration */
  /* PLL3_VCO Input = HSE_VALUE/PLL3M = 5 Mhz */
  /* PLL3_VCO Output = PLL3_VCO Input * PLL3N = 800 Mhz */
  /* PLLLCDCLK = PLL3_VCO Output/PLL3R = 800/83 = 9.63 Mhz */
  /* LTDC clock frequency = PLLLCDCLK = 9.63 Mhz */
  periph_clk_init_struct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  periph_clk_init_struct.PLL3.PLL3M = 5;
  periph_clk_init_struct.PLL3.PLL3N = 160;
  periph_clk_init_struct.PLL3.PLL3FRACN = 0;
  periph_clk_init_struct.PLL3.PLL3P = 2;
  periph_clk_init_struct.PLL3.PLL3Q = 2;
  periph_clk_init_struct.PLL3.PLL3R = 40;
  HAL_RCCEx_PeriphCLKConfig(&periph_clk_init_struct);

  /* LTDC Initialization -------------------------------------------------------*/
  /* DeInit */
  HAL_LTDC_DeInit(&hltdc);

  /* Polarity configuration */
  /* Initialize the horizontal synchronization polarity as active low */
  hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  /* Initialize the vertical synchronization polarity as active low */
  hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  /* Initialize the data enable polarity as active low */
  hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  /* Initialize the pixel clock polarity as input pixel clock */
  hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  /* Timing configuration */
  /* The LCD AMPIRE 640x480 is selected */
  /* Timing configuration */
  hltdc.Init.HorizontalSync = (LCD_HSYNC - 1);
  hltdc.Init.VerticalSync = (LCD_VSYNC - 1);
  hltdc.Init.AccumulatedHBP = (LCD_HSYNC + LCD_HBP - 1);
  hltdc.Init.AccumulatedVBP = (LCD_VSYNC + LCD_VBP - 1);
  hltdc.Init.AccumulatedActiveH = (LCD_HEIGHT + LCD_VSYNC + LCD_VBP - 1);
  hltdc.Init.AccumulatedActiveW = (LCD_WIDTH + LCD_HSYNC + LCD_HBP - 1);
  hltdc.Init.TotalHeigh = (LCD_HEIGHT + LCD_VSYNC + LCD_VBP + LCD_VFP - 1);
  hltdc.Init.TotalWidth = (LCD_WIDTH + LCD_HSYNC + LCD_HBP + LCD_HFP - 1);


  /* Configure R,G,B component values for LCD background color */
  hltdc.Init.Backcolor.Blue = 0;
  hltdc.Init.Backcolor.Green = 0;
  hltdc.Init.Backcolor.Red = 0;

  hltdc.Instance = LTDC;


  /* Configure the LTDC */
  if(HAL_LTDC_Init(&hltdc) != HAL_OK)
  {
    /* Initialization Error */
    ret = false;
  }


  uint8_t *p_data = (uint8_t *)FRAME_BUF_ADDR;

  for (int i=0; i<LCD_WIDTH*LCD_HEIGHT*3; i++)
  {
    p_data[i] = 0;
  }

  ltdcLayerInit(0, FRAME_BUF_ADDR);
  //ltdcLayerInit(1, sdramGetAddr());

  return ret;
}


bool ltdcLayerInit(uint16_t LayerIndex, uint32_t Address)
{
  LTDC_LayerCfgTypeDef      pLayerCfg;
  bool ret = true;


  /* Layer1 Configuration ------------------------------------------------------*/

  /* Windowing configuration */
  /* In this case all the active display area is used to display a picture then :
     Horizontal start = horizontal synchronization + Horizontal back porch = 43
     Vertical start   = vertical synchronization + vertical back porch     = 12
     Horizontal stop = Horizontal start + window width -1 = 43 + 480 -1
     Vertical stop   = Vertical start + window height -1  = 12 + 272 -1      */
  pLayerCfg.WindowX0 = 0;
  pLayerCfg.WindowX1 = LCD_WIDTH;
  pLayerCfg.WindowY0 = 0;
  pLayerCfg.WindowY1 = LCD_HEIGHT;

  /* Pixel Format configuration*/
  pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB888;

  /* Start Address configuration : frame buffer is located at FLASH memory */
  pLayerCfg.FBStartAdress = Address;

  /* Alpha constant (255 == totally opaque) */
  pLayerCfg.Alpha = 255;

  /* Default Color configuration (configure A,R,G,B component values) : no background color */
  pLayerCfg.Alpha0 = 0; /* fully transparent */
  pLayerCfg.Backcolor.Blue = 0;
  pLayerCfg.Backcolor.Green = 0;
  pLayerCfg.Backcolor.Red = 0;

  /* Configure blending factors */
  pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
  pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;

  /* Configure the number of lines and number of pixels per line */
  pLayerCfg.ImageWidth  = LCD_WIDTH;
  pLayerCfg.ImageHeight = LCD_HEIGHT;


  /* Configure the Layer*/
  if(HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, LayerIndex) != HAL_OK)
  {
    /* Initialization Error */
    ret = false;
  }

  return ret;
}




uint32_t ltdcWidth(void)
{
  return LCD_WIDTH;
}

uint32_t ltdcHeight(void)
{
  return LCD_HEIGHT;
}

uint32_t ltdcGetBufferAddr(uint8_t index)
{
  return 0x24000000;
}





/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

/**
  * @brief LTDC MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param hltdc: LTDC handle pointer
  * @retval None
  */
void HAL_LTDC_MspInit(LTDC_HandleTypeDef *hltdc)
{
 GPIO_InitTypeDef GPIO_InitStruct;

 /* Enable the LTDC and DMA2D clocks */
 __HAL_RCC_LTDC_CLK_ENABLE();
 __HAL_RCC_DMA2D_CLK_ENABLE();

 /** @brief Toggle Sw reset of LTDC IP */
 __HAL_RCC_LTDC_FORCE_RESET();
 __HAL_RCC_LTDC_RELEASE_RESET();

 /** @brief Toggle Sw reset of DMA2D IP */
 __HAL_RCC_DMA2D_FORCE_RESET();
 __HAL_RCC_DMA2D_RELEASE_RESET();



 __HAL_RCC_GPIOF_CLK_ENABLE();
 __HAL_RCC_GPIOC_CLK_ENABLE();
 __HAL_RCC_GPIOA_CLK_ENABLE();
 __HAL_RCC_GPIOB_CLK_ENABLE();
 __HAL_RCC_GPIOE_CLK_ENABLE();
 __HAL_RCC_GPIOD_CLK_ENABLE();
 __HAL_RCC_GPIOG_CLK_ENABLE();
 /**LTDC GPIO Configuration
 PF10     ------> LTDC_DE
 PC0     ------> LTDC_R5
 PA3     ------> LTDC_B5
 PA4     ------> LTDC_VSYNC
 PA5     ------> LTDC_R4
 PA6     ------> LTDC_G2
 PB0     ------> LTDC_R3
 PB1     ------> LTDC_R6
 PE11     ------> LTDC_G3
 PE12     ------> LTDC_B4
 PE14     ------> LTDC_CLK
 PE15     ------> LTDC_R7
 PB10     ------> LTDC_G4
 PB11     ------> LTDC_G5
 PD10     ------> LTDC_B3
 PG8     ------> LTDC_G7
 PC6     ------> LTDC_HSYNC
 PC7     ------> LTDC_G6
 PB8     ------> LTDC_B6
 PB9     ------> LTDC_B7
 */
 GPIO_InitStruct.Pin = GPIO_PIN_10;
 GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
 GPIO_InitStruct.Pull = GPIO_NOPULL;
 GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
 GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
 HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

 GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_6|GPIO_PIN_7;
 GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
 GPIO_InitStruct.Pull = GPIO_NOPULL;
 GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
 GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
 HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

 GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
 GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
 GPIO_InitStruct.Pull = GPIO_NOPULL;
 GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
 GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
 HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

 GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
 GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
 GPIO_InitStruct.Pull = GPIO_NOPULL;
 GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
 GPIO_InitStruct.Alternate = GPIO_AF9_LTDC;
 HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

 GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_14|GPIO_PIN_15;
 GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
 GPIO_InitStruct.Pull = GPIO_NOPULL;
 GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
 GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
 HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

 GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_8|GPIO_PIN_9;
 GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
 GPIO_InitStruct.Pull = GPIO_NOPULL;
 GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
 GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
 HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

 GPIO_InitStruct.Pin = GPIO_PIN_10;
 GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
 GPIO_InitStruct.Pull = GPIO_NOPULL;
 GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
 GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
 HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

 GPIO_InitStruct.Pin = GPIO_PIN_8;
 GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
 GPIO_InitStruct.Pull = GPIO_NOPULL;
 GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
 GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
 HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
}

/**
  * @brief LTDC MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  * @param hltdc: LTDC handle pointer
  * @retval None
  */
void HAL_LTDC_MspDeInit(LTDC_HandleTypeDef *hltdc)
{

  /*##-1- Reset peripherals ##################################################*/
  /* Enable LTDC reset state */
  __HAL_RCC_LTDC_FORCE_RESET();

  /* Release LTDC from reset state */
  __HAL_RCC_LTDC_RELEASE_RESET();
}
