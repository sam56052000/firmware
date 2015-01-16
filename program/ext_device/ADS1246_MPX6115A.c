// ADS1246_MPX6115A.c
#include "ADS1246_MPX6115A.h"

#include "stm32f4xx_conf.h"
#include "spi.h"

int32_t mpx6115A_tare_value;

void ads1246_delay(volatile uint32_t count)
{

	while (count--) {

	}
}
void ads1246_reset(void){

ADS1246_RESET_HIGH();
ads1246_delay(100000);
ADS1246_RESET_LOW();
ads1246_delay(100000);
ADS1246_RESET_HIGH();
ads1246_delay(100000);
ads1246_delay(100000);
}

uint8_t ads1246_read1byte(uint8_t addr){

	uint8_t output=0;

	SPI_xfer(ADS1246_MPX6115A_SPI,0x20|addr);
	SPI_xfer(ADS1246_MPX6115A_SPI,0x00);
	output = SPI_xfer(ADS1246_MPX6115A_SPI,0xFF);
	return output;

}

void ads1246_write1byte(uint8_t addr, uint8_t data){

	SPI_xfer(ADS1246_MPX6115A_SPI,0x40|addr);
	SPI_xfer(ADS1246_MPX6115A_SPI,0x00);
	SPI_xfer(ADS1246_MPX6115A_SPI,data);

}

int32_t ads1246_readADCconversion(void){

	uint8_t rxdat1=0,rxdat2=0,rxdat3=0;


	rxdat1 = SPI_xfer(ADS1246_MPX6115A_SPI,0xFF);
	rxdat2 = SPI_xfer(ADS1246_MPX6115A_SPI,0xFF);
	rxdat3 = SPI_xfer(ADS1246_MPX6115A_SPI,0xFF);

	return  (uint32_t)rxdat1<<16 | (uint32_t)rxdat2<<8 |  (uint32_t)rxdat3;
}

void ads1246_initialize(void){

	ads1246_reset();
	ADS1246_CS_HIGH(); // Deselect the chip to reset SPI
	ads1246_delay(100000);
	ADS1246_CS_LOW(); // Select the chip to initiate transmission
	ADS1246_START_HIGH(); // Set start high to initiate communication
	ads1246_write1byte(0x03,0b00001000); // Write sampling configuration at 0x03 register 
	ads1246_delay(1000);

	/* There should be one spi_xfer ( 0x14 ) here (read_continous command). But not sure if it needs or not */

	while(ADS1246_DRDY_PIN_STATE()){
	}

	mpx6115A_tare_value = ads1246_readADCconversion();

}

float MPX6115_get_raw_altitude(int32_t adc_data){


	return (float)((mpx6115A_tare_value)-adc_data)*ALT_PER_LSB ;

}


void MPX6115_update_tare_value(void){

	while(ADS1246_DRDY_PIN_STATE()){
	}

	mpx6115A_tare_value = ads1246_readADCconversion();


}


// void ads1246_CAN_UpdateADC(imu_unscaled_data_t *imu_raw_data){

// 	CanRxMsg RxMessage;
//  	uint8_t barometer_raw_data[3];

//  		if( CAN2_CheckMessageStatusFlag(CAN_MESSAGE_BAROMETER) == 1){

//     			RxMessage =  CAN2_PassRXMessage();
// 				CAN2_ClearMessageStatusFlag(CAN_MESSAGE_MAGNETOMETER);

// 				hmc5983_buffer[0] = RxMessage.Data[0];
// 				hmc5983_buffer[1] = RxMessage.Data[1];
// 				hmc5983_buffer[2] = RxMessage.Data[2];
// 				hmc5983_buffer[3] = RxMessage.Data[3];
// 				hmc5983_buffer[4] = RxMessage.Data[4];
// 				hmc5983_buffer[5] = RxMessage.Data[5];


// 				imu_raw_data->mag[0] = -(int16_t)(((uint16_t)hmc5983_buffer[0] << 8) | (uint16_t)hmc5983_buffer[1]);
// 				imu_raw_data->mag[2] = -(int16_t)(((uint16_t)hmc5983_buffer[2] << 8) | (uint16_t)hmc5983_buffer[3]);
// 				imu_raw_data->mag[1] =  (int16_t)(((uint16_t)hmc5983_buffer[4] << 8) | (uint16_t)hmc5983_buffer[5]);


//  		}



// }
