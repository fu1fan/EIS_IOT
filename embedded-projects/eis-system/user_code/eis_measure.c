/*
 * eis_measure.c
 *
 *  Created on: 2024年6月29日
 *      Author: Whisp
 */
#include "eis_measure.h"
#include "ad7606.h"
#include "ads1256.h"
#include "dac1220.h"
#include "can_remote.h"
#include <math.h>

#define BATTERY_CH 2
#define e 2.718281828459045

//uint32_t EIS_ALL_FREQ_POINTS[] = {2000, 1611, 1298, 1046, 843, 679, 547, 441, 355, 286, 230, 186, 150, 121, 97, 78, 63, 50, 40, 32, 25, 20, 16, 13, 10, 8, 6, 5, 4, 3, 2};

static float voltage_correction;

void vf_relay(uint8_t value_){
	static uint8_t is_set = 0;
	gpio_write_pin(VF_RELAY, value_);
	if(value_ != is_set){
		osal_delay_millisec(RELAY_DELAY);
		is_set = value_;
	}
}

void ccs_relay(uint8_t value_) {
//	static uint8_t is_set = 0;
	gpio_write_pin(CCS_RELAY, value_);
//	if (value_ != is_set) {
//		osal_delay_millisec(RELAY_DELAY);
//		is_set = value_;
//	}
}

void _end(void){
	ad9959_reset();
	ccs_relay(0);
	vf_relay(0);
	DAC1220VolWrite(0);
}

void voltage_follow(void) {
	vf_relay(1);
	float battery_voltage = ADS1256_ReadPin(ADS1256_GAIN_1, 0) + voltage_correction;
	DAC1220VolWrite(battery_voltage);
	osal_delay_millisec(FRQ_CHANGE_WAIT);
}

eis_status_t voltage_follow_correct(void){
	// 这里没有对修正因子的大小做限制
	// TODO: 加入重试机制
	eis_status_t status;
	status.is_success = 1;
	status.error_code = 0x00;
    ad9959_set(CH1, 1000, 1023);
    osal_delay_millisec(FRQ_CHANGE_WAIT*2);
    ccs_relay(1);

    int8_t binary_status = 0;	//状态机：-1检测是否已通过测试 0初测 1二分开始 2二分查找中 3二分结束
    uint8_t binary_depth = 0;
    float binary_l = 0;
    float binary_r = 0;
    while(1){
    	voltage_follow();
    	ad7606_StartRecord(16000, 64);
    	ad7606_WaitforRecord();
		uint8_t top_count = 0;
		uint8_t buttom_count = 0;
		uint8_t top_flag = 0;
		uint8_t buttom_flag = 0;
		for (uint8_t i = 0; i < 128; i++) {
			//  检查是否有触顶数据
			if (!top_flag && g_tAD.usBuf_1[i] > 30000) {
				top_count++;
			}
			if (!buttom_flag && g_tAD.usBuf_1[i] < -30000) {
				buttom_count++;
			}
			if (top_count >= 5) {
				top_flag = 1;
			}
			if (buttom_count >= 5) {
				buttom_flag = 1;
			}
			if(top_flag && buttom_flag) {
				status.is_success = 0;
				status.error_code = 0x31;
				voltage_correction = 0;
				ad9959_reset();
				return status;
			}
		}
		// 采用二分查找，最小分辨率0.01
		switch(binary_status){
		case -1:
			if (!top_flag && !buttom_flag) {
				binary_status = 3;
				break;
			}
			voltage_correction = 0;
			binary_status = 1;
			break;
		case 0:
			if (top_flag) {
				voltage_correction = 0.5 * VOLTAGE_CORRECTION_RANGE;
				binary_l = 0;
				binary_r = voltage_correction;
			} else if (buttom_flag) {
				voltage_correction = -0.5 * VOLTAGE_CORRECTION_RANGE;
				binary_l = voltage_correction;
				binary_r = 0;
			} else {
				binary_status = 3;
				break;
			}
			binary_status = 1;
			break;
		case 1:
			if ((voltage_correction > 0 && top_flag) || (voltage_correction < 0 && buttom_flag)) {
				status.is_success = 0;
				status.error_code = 0x32;
				binary_status = 3;
				break;
			}
			voltage_correction = (binary_l + binary_r) / 2;
			binary_status = 2;
			break;

		case 2:
            if (!top_flag && !buttom_flag) {
				binary_status = 3;
				break;
			}
            if (top_flag) {
				binary_l = voltage_correction;
				voltage_correction = (binary_l + binary_r) / 2;
			} else if (buttom_flag) {
				binary_r = voltage_correction;
				voltage_correction = (binary_l + binary_r) / 2;
			}
            binary_depth++;
            if (binary_depth >= 8) {
            	status.is_success = 0;
            	status.error_code = 0x32;
            	binary_status = 3;
            }
		}
		if (binary_status==3)
			break;
    }
    ad9959_reset();
    return status;
//	while(retry<VOLTAGE_CORRECTION_RANGE*2){
//		uint8_t top_count = 0;
//		uint8_t buttom_count = 0;
//		uint8_t top_flag = 0;
//		uint8_t buttom_flag = 0;
//		for(uint16_t i=0; i<256; i++){
//			// 检查是否有触顶数据
//			if(!top_flag&&g_tAD.usBuf_1[i] == 0x7FFF){
//				top_count++;
//			}
//			else if(top_count){
//				top_count = 0;
//			}
//			if(!buttom_flag&&g_tAD.usBuf_1[i] == -32768){
//				buttom_count++;
//			}
//			else if(buttom_count){
//				buttom_count = 0;
//			}
//			if(top_count>=5){
//				top_flag = 1;
//			}
//			if(buttom_count>=5){
//				buttom_flag = 1;
//			}
//		}
//		if (top_flag && buttom_flag) {
//			status.is_success = 0;
//			status.error_code = 0x31; //TODO: 错误代码是内阻过大
//			voltage_correction = 0;
//			return status;
//		}
//		if (top_flag) {
//			voltage_correction += 0.01;
//		}
//		else if (buttom_flag) {
//			voltage_correction -= 0.01;
//		}
//		else {
//			break;
//		}
//		retry++;
//		if (retry != MAX_RETRY) {
//			voltage_follow();
//			osal_delay_millisec(10);
//			ad7606_StartRecord(sfreq_, 2048);
//			ad7606_WaitforRecord();
//		}
//	}
//	if (retry == MAX_RETRY) {
//		status.is_success = 0;
//		status.error_code = 0x32;	//偏置异常
//		voltage_correction = 0;
//	}
//	return status;
}


eis_status_t eis_measure_init(void) {
	eis_status_t status;
	status.is_success = 1;
	status.error_code = 0x00;

	voltage_correction = 0;

	gpio_write_pin(VF_RELAY, 0);
	gpio_write_pin(CCS_RELAY, 0);

	DAC1220_Init();
	DAC1220VolWrite(0);

	bsp_InitAD7606();
	ad7606_SingleRecord();

	uint8_t i;
	for(i = 0; i < 8; i++){
		if(g_tAD_single_buffer[i] != 0.0f){
			break;
		}
	}
	if(i==8){
		status.is_success = 0;
		status.error_code = 0x12;
		return status;
	}

	ad9959_reset();

	InitADS1256();
	if(ADS1256_ReadChipID() != 3){
		status.is_success = 0;
		status.error_code = 0x13;
		return status;
	}

	eis_calculate_init();

	return status;
}

eis_status_t eis_battery_select(uint8_t battery_) {
	eis_status_t status;
	status.is_success = 1;
	status.error_code = 0x00;

	vf_relay(0);

	eb_set(battery_ / 4, battery_ % 4);

	float battery_voltage = 5.0*g_tAD_single_buffer[BATTERY_CH]/32768;
	if (battery_voltage < 2 || battery_voltage > 5) {
		status.is_success = 0;
		status.error_code = 0x21;
		return status;
	}

	return status;
}

eis_status_t eis_measure() {
	eis_status_t status;
	status.is_success = 1;
	status.error_code = 0x00;

	eis_result_clear();

	ad9959_reset();
	ccs_relay(1);

	uint8_t failure_cnt = 0;

	status = voltage_follow_correct();
	if (!status.is_success) {
		_end();
		return status;
	}

	for (uint8_t i = 5; i < 38; i+=2) {
		uint32_t freq = pow(e, 0.2303*i)+0.5;

		status = voltage_follow_correct();
		if (!status.is_success) {
			_end();
			return status;
		}

		ad9959_set(CH1, freq, 1023);
		osal_delay_millisec(FRQ_CHANGE_WAIT);
		uint32_t sfreq = freq * 512;
		while(sfreq>30000){
			sfreq /= 2;
		}

		uint8_t retry = 0;

		while(retry<MAX_RETRY){
			ad7606_StartRecord(sfreq, 2048);
			ad7606_WaitforRecord();

			eis_add_point(sfreq);
			if(eis_result.tail->freq == freq &&(i>=5||eis_result.tail->imag<0)){
				break;
			}
			retry++;
			if(retry!=MAX_RETRY){
				eis_remove_tail();
			}
		}
		if (retry == MAX_RETRY) {
			status.is_success = 0;
			status.error_code = 0x33;
			eis_result.tail->real = 0;
			eis_result.tail->imag = 0;
			failure_cnt++;
		}

		// 替代
#ifdef UART_DEBUG
		printf("%d\t%f\t%f\r\n", (int)eis_result.tail->freq, eis_result.tail->real, eis_result.tail->imag);
#endif
		voltage_follow();
	}

	ccs_relay(0);
	vf_relay(0);
	ad9959_reset();
	DAC1220VolWrite(0);

	if (failure_cnt>=3){
		// status.is_success = 0;
		status.error_code = 0x34;
	}

	return status;
}

void eis_single_init(){

	ccs_relay(1);
	vf_relay(1);
}

eis_status_t eis_single_measure(uint32_t freq_, uint8_t accuracy_){
	eis_status_t status;
	status.is_success = 1;
	status.error_code = 0x00;

	eis_result_clear();
	uint8_t retry = 0;
	while(retry<MAX_RETRY&&eis_result.size<accuracy_){
		status = voltage_follow_correct();
		if (!status.is_success) {
			_end();
			return status;
		}
		ad9959_set(CH1, freq_, 1023);
		osal_delay_millisec(FRQ_CHANGE_WAIT);

		uint32_t sfreq = freq_ * 512;
		while (sfreq > 30000) {
			sfreq /= 2;
		}

		ad7606_StartRecord(sfreq, 2048);
		ad7606_WaitforRecord();

		eis_add_point(sfreq);
		if(eis_result.tail->freq == freq_){
			continue;
		}
		retry++;
		eis_remove_tail();
	}

	eis_single_point_process();

	if (eis_result.tail->real == 0 && eis_result.tail->imag == 0) {
		status.is_success = 0;
		status.error_code = 0x35;
	}
#ifdef UART_DEBUG
	printf("%d\t%f\t%f\r\n", (int)eis_result.tail->freq, eis_result.tail->real, eis_result.tail->imag);
	printf("mag=%f\r\n", sqrt(eis_result.tail->real*eis_result.tail->real+eis_result.tail->imag*eis_result.tail->imag));
#endif
	return status;
}

void eis_single_end() {
	_end();
}

eis_status_t eis_ohmage_measure(float *ohmage, uint32_t *freq){
	eis_status_t status;
	status.is_success = 1;
	status.error_code = 0x00;

	eis_single_init();
	// 二分查找,找到虚部绝对值<0.05的点
	// 范围500Hz-7000Hz
	// 先对两个点进行测量,500Hz虚部应当小于零,7000Hz虚部应当大于零,否则返回错误
	status = eis_single_measure(500, 5);
	if (!status.is_success) {
		eis_single_end();
		return status;
	}
	if (eis_result.tail->imag > 0) {
		status.is_success = 0;
		status.error_code = 0x36;
		return status;
	}
	status = eis_single_measure(7000, 5);
	if (!status.is_success) {
		eis_single_end();
		return status;
	}
	if (eis_result.tail->imag < 0) {
		status.is_success = 0;
		status.error_code = 0x37;
		eis_single_end();
		return status;
	}
	// 二分查找
	uint32_t l = 500;
	uint32_t r = 7000;
	uint32_t mid = 0;
	uint8_t depth = 0;
	while (depth < 8) {
		mid = (l + r) / 2;
		status = eis_single_measure(mid, 2);
		if (!status.is_success) {
			eis_single_end();
			return status;
		}
		if (eis_result.tail->imag < 0) {
			// 虚部小于0于零,说明频率不够高
			l = mid;
		} else {
			r = mid;
		}
		if (fabs(eis_result.tail->imag) < 0.1) {
			*ohmage = sqrt(
					eis_result.tail->real * eis_result.tail->real
				+	eis_result.tail->imag * eis_result.tail->imag);
			*freq = mid;
			break;
		}
		depth++;
	}
	if (depth == 8) {
		*ohmage = sqrt(
				eis_result.tail->real * eis_result.tail->real
			+	eis_result.tail->imag * eis_result.tail->imag);
		*freq = mid;
		status.is_success = 0;
		status.error_code = 0x38;
	}
	eis_single_end();
	return status;
}

eis_status_t eis_get_voltage(float *voltage) {
	eis_status_t status;
	status.is_success = 1;
	status.error_code = 0x00;

	vf_relay(1);

	*voltage = ADS1256_ReadPin(ADS1256_GAIN_1, 0);
	if (*voltage < 2 || *voltage > 4.8) {
		status.is_success = 0;
		status.error_code = 0x21;
		return status;
	}

	return status;
}

