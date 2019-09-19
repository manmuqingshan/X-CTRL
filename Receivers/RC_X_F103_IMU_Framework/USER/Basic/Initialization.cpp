#include "FileGroup.h"
#include "ComPrivate.h"

void Init_IWDG(uint8_t prescaler, uint16_t reload)
{
    /*Tout=((4*2^prer)*rlr)/32 (ms)*/
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //ʹ�ܶ�IWDG->PR IWDG->RLR��д
    IWDG_SetPrescaler(prescaler); //����IWDG��Ƶϵ��
    IWDG_SetReload(reload);   //����IWDGװ��ֵ
    IWDG_ReloadCounter(); //reload
    IWDG_Enable();       //ʹ�ܿ��Ź�
}

void Init_BaseClocks(bool internal)
{
    if(internal)//@48MHz
    {
        RCC_HSICmd(ENABLE);
        RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
        while(RCC_GetSYSCLKSource());
        RCC_PLLCmd(DISABLE);
        RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_12);
        RCC_PLLCmd(ENABLE);
        while(!RCC_GetFlagStatus(RCC_FLAG_PLLRDY));
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
        while(RCC_GetSYSCLKSource() != 0x08);
        RCC_HCLKConfig(RCC_SYSCLK_Div1);
        RCC_PCLK1Config(RCC_SYSCLK_Div1);
        RCC_PCLK2Config(RCC_SYSCLK_Div1);
    }
    else//@128MHz
    {
        RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
        while(RCC_GetSYSCLKSource());
        RCC_HCLKConfig(RCC_SYSCLK_Div1);
        RCC_PCLK1Config(RCC_HCLK_Div2);
        RCC_PCLK2Config(RCC_HCLK_Div1);
        RCC_PLLCmd(DISABLE);
        RCC_HSEConfig(RCC_HSE_ON);
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_8);
        RCC_PLLCmd(ENABLE);
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) ;
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
        while(RCC_GetSYSCLKSource()!=0x08);
    }
}

void Init_Value()
{
    StorageDataReg(NRF_Cfg);
    EEPROM_Handle(EEPROM_Chs::ReadData);
}