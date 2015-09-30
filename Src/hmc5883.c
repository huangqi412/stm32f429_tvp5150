#include "hmc5883.h"

void HMC_Init()
{
	return;
}


void HMC_WriteByte(uint8_t addr, uint8_t data)
{
	uint8_t i2cbuf[2];
	i2cbuf[0] = addr;
	i2cbuf[1] = data;
	I2C1_WriteBuffer(HMC_WRITE_ADDRESS, i2cbuf, 2);
}

uint8_t HMC_ReadByte(uint8_t addr)
{
	uint8_t i2cbuf[1];
  I2C1_WriteBuffer(HMC_READ_ADDRESS, &addr, 1);
	I2C1_ReadBuffer(HMC_READ_ADDRESS, addr, i2cbuf, 1);
	return i2cbuf[0];
}


void HMC_Get_Configuration(HMC_CF_StructTypeDef *res)
{
	res->VAL1 = HMC_ReadByte(HMC_Addr_ConfA);
	res->RATE = (HMC_DR_TypeDef) ((res->VAL1 >> 2) & 7);
	res->MODE = (HMC_MM_TypeDef)  (res->VAL1       & 3);
	res->VAL2 = HMC_ReadByte(HMC_Addr_ConfA);
	res->GAIN = (HMC_GS_TypeDef) ((res->VAL2 >> 5) & 7);
}

void HMC_Set_Configurarion(HMC_DR_TypeDef rate, HMC_MM_TypeDef mode, HMC_GS_TypeDef gain)
{
	uint8_t tmp = 0;
	tmp = (rate & 7) * 4 + (mode & 3);
	HMC_WriteByte(HMC_Addr_ConfA, tmp);
	tmp = 0;
	tmp = (gain & 7) * 0x20;
	HMC_WriteByte(HMC_Addr_ConfB, tmp);
}

HMC_MR_TypeDef HMC_Get_Mode()
{
	uint8_t res;
	res = HMC_ReadByte(HMC_Addr_Mode);
	return (HMC_MR_TypeDef) (res & 3);
}

void HMC_Set_MODE(HMC_MR_TypeDef mode)
{
	uint8_t tmp = 0;
	tmp = mode & 3;
	HMC_WriteByte(HMC_Addr_Mode, tmp);
}

int16_t HMC_GetX()
{
	int16_t res = 0;
	res = HMC_ReadByte(HMC_Addr_X_MSB);
	res = res << 8;
	res = res + HMC_ReadByte(HMC_Addr_X_LSB);
	return res;
}

int16_t HMC_GetY()
{
	int16_t res = 0;
	res = HMC_ReadByte(HMC_Addr_Y_MSB);
	res = res << 8;
	res = res + HMC_ReadByte(HMC_Addr_Y_LSB);
	return res;
}

int16_t HMC_GetZ()
{
	int16_t res = 0;
	res = HMC_ReadByte(HMC_Addr_Z_MSB);
	res = res << 8;
	res = res + HMC_ReadByte(HMC_Addr_Z_LSB);
	return res;
}

void HMC_GetXYZ(HMC_XYZ_StructTypeDef *res)
{
	res->X = HMC_GetX();
	res->Y = HMC_GetY();
	res->Z = HMC_GetZ();
}

void HMC_GetStatus(HMC_SR_StructTypeDef *res)
{
	res->VAL  = HMC_ReadByte(HMC_Addr_Status);
	res->REN  = (bool) ((res->VAL >> 2) & 1);
	res->LOCK = (bool) ((res->VAL >> 1) & 1);
	res->RDY  = (bool)  (res->VAL       & 1);
}

void HMC_GetId(HMC_ID_StructTypeDef *res)
{
	res->VALA  = HMC_ReadByte(HMC_Addr_IdentA);
	res->VALB  = HMC_ReadByte(HMC_Addr_IdentB);
	res->VALC  = HMC_ReadByte(HMC_Addr_IdentC);
}

bool HMC_TestConnection()
{
	HMC_ID_StructTypeDef res;
	HMC_GetId(&res);
	if (res.VALA == 'H' && res.VALB == '4' && res.VALB == '3' ) return true;
	return false;
}


