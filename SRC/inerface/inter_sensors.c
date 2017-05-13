#include "inter_sensors.h"


SENSOR_INTERFACE sensor_interface;

void init_sensor_interface(void)
{
	pADC_SENSORS temp_adc_sensor;
	pSPI_SENSORS temp_spi_sensors;
	ADCC_Configuration();
	ADCP_Configuration();
	//SPI1_Configureation();
	
	temp_adc_sensor = get_adc_sensor_handle();
	temp_spi_sensors = get_spi_sensor_handle();
	sensor_interface.finger_pressure_value = temp_adc_sensor->adc_value_p;
	sensor_interface.servo_current_value = temp_adc_sensor->adc_value_c;
	sensor_interface.servo_angle_value	= temp_spi_sensors->spi_value_a;
	
	sensor_interface.start_angle_sample = temp_spi_sensors->SPI_start_sample;
	sensor_interface.start_pressure_sample = temp_adc_sensor->ADCP_start_sample;
	sensor_interface.start_current_sample = temp_adc_sensor->ADCC_start_sample;
	
}









SENSOR_INTERFACE* get_sensor_interface_handle(void)
{
	return & sensor_interface;
}
