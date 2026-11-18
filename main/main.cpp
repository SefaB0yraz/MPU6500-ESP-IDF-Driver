#include <cstddef>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include "driver/i2c_master.h"
#include "driver/i2c_types.h"
#include "esp_err.h"
#include "hal/i2c_types.h"
#include "soc/clk_tree_defs.h"
#include "soc/gpio_num.h"

extern "C" void app_main(void);

uint8_t pwr_mgmt_reg=0x6B;
uint8_t accel_confi_reg=0x1C;
uint8_t accel_confi_data=0;
uint8_t gyro_config_reg=0x1B;
uint8_t gyro_config_data=0;
uint8_t data=0;
uint8_t X_accel_reg=0x3B;
uint8_t accel_data[6]={0};
uint8_t X_gyro_reg=0x43;
uint8_t gyro_data[6]={};

//WAKING MPU UP
uint8_t pwr_mgmt_data[2]={0x6B,0x01};

void app_main(void)
{
	i2c_master_bus_config_t i2c_config={};
	
	i2c_config.i2c_port= I2C_NUM_0;
	i2c_config.sda_io_num=GPIO_NUM_21;
	i2c_config.scl_io_num= GPIO_NUM_22;
	i2c_config.clk_source= I2C_CLK_SRC_DEFAULT;
    i2c_config.flags.enable_internal_pullup=true;
    
    i2c_master_bus_handle_t bus_handle;
    
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_config,&bus_handle));
    
    i2c_device_config_t dev_config={};
    dev_config.dev_addr_length=I2C_ADDR_BIT_LEN_7;
    dev_config.scl_speed_hz=100000;
    dev_config.device_address=0x68;
    
    i2c_master_dev_handle_t dev_handle;
    
    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle,&dev_config, &dev_handle));
    //FOR WAKING MPU UP 
    ESP_ERROR_CHECK(i2c_master_transmit(dev_handle,pwr_mgmt_data,2,-1));
    //FOR PWR_MGMT_1
    ESP_ERROR_CHECK(i2c_master_transmit_receive(dev_handle, &pwr_mgmt_reg,1,&data,1,-1));
    //FOR READING ACCEL_CONFIG BITS
    ESP_ERROR_CHECK(i2c_master_transmit_receive(dev_handle,&accel_confi_reg,1,&accel_confi_data,1,-1));
	printf("ACCEL_CONFIG: 0x%02X\n", accel_confi_data);
	ESP_ERROR_CHECK(i2c_master_transmit_receive(dev_handle, &gyro_config_reg,1, &gyro_config_data,1,-1));
    printf("GYRO CONFIG: 0x%02X\n", gyro_config_data);
    
    while (true) {
		//FOR READING XYZ RAW ACCEL
		ESP_ERROR_CHECK(i2c_master_transmit_receive(dev_handle, &X_accel_reg,1,accel_data,6,-1));
		int16_t X_accel_raw = (accel_data[0] << 8) | accel_data[1];
		int16_t Y_accel_raw=(accel_data[2]<<8) | accel_data[3];
        int16_t Z_accel_raw=(accel_data[4]<<8) | accel_data[5];
        
        //G ACCEL FOR XYZ AXIS
        float X_accel_g=(float)X_accel_raw/16384.0f;
        float Y_accel_g=(float)Y_accel_raw/16384.0f;
        float Z_accel_g=(float)Z_accel_raw/16384.0f;
        
        //FOR READING XYZ RAW GYRO
        ESP_ERROR_CHECK(i2c_master_transmit_receive(dev_handle, &X_gyro_reg,1,gyro_data, 6,-1));
        int16_t X_gyro_raw=(gyro_data[0]<<8) | gyro_data[1];
        int16_t Y_gyro_raw=(gyro_data[2]<<8) | gyro_data[3];
        int16_t Z_gyro_raw=(gyro_data[4]<<8) | gyro_data[5];
        
        // /s GYRO FOR XYZ AXIS
        float X_gyro_dps=(float)X_gyro_raw/131.0f;
        float Y_gyro_dps=(float)Y_gyro_raw/131.0f;
        float Z_gyro_dps=(float)Z_gyro_raw/131.0f;
        
        
        printf(" ACCEL X: %.3f g |, Y:%.3f g |, Z:%.3f g\n",X_accel_g,Y_accel_g,Z_accel_g);
        printf(" GYRO X: %.3f s|, Y:%.3f s |, Z:%.3f s\n",X_gyro_dps,Y_gyro_dps,Z_gyro_dps);
        
        
        sleep(1);
    }
}
