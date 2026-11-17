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
uint8_t data=0;
uint8_t X_accel_data[2]={0};
uint8_t X_reg=0x3B;
uint8_t Y_accel_data[2]={};
uint8_t Y_reg=0x3D;

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
    
    
    
    while (true) {
		//FOR READING X ACCEL
		ESP_ERROR_CHECK(i2c_master_transmit_receive(dev_handle, &X_reg,1,X_accel_data,2,-1));
		int16_t X_accel = (X_accel_data[0] << 8) | X_accel_data[1];
		printf("X Accel Raw: %d\n", X_accel);
        ESP_ERROR_CHECK(i2c_master_transmit_receive(dev_handle, &X_reg,1, X_accel_data,2,-1));
        int16_t Y_accel=(X_accel_data[0]<<8) | X_accel_data[1];
        printf("Y Accel Raw: %d",Y_accel);
        
        sleep(1);
    }
}
