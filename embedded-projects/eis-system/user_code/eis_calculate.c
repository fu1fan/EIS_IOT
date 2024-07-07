/*
 * eis_calculate.c
 *
 *  Created on: 2024年6月29日
 *      Author: Whisp
 */
#include "eis_calculate.h"
#include "soft_fft.h"
#include "ad7606.h"
#include <math.h>

struct eis_data_t eis_result;

void eis_result_clear(){
	struct eis_node_t *p = eis_result.head;
	while (p != NULL) {
		struct eis_node_t *q = p->next;
		free(p);
		p = q;
	}
	eis_result.size = 0;
}

void eis_calculate_init() {
	create_sin_tab(SIN_TAB);
	eis_result.head = NULL;
	eis_result.tail = NULL;
	eis_result.size = 0;
}

void eis_add_point(uint32_t sample_rate) {
	// TODO: 错误频率纠正
	// 刷新数据集
	for (int i = 0; i < FFT_N; i++) {
		Refresh_Data(CompxIO, i, (double)g_tAD.usBuf_0[i]);
	}
	FFT(CompxIO);
	Get_Result(CompxIO, CompxRE, sample_rate);	// CompxRE real为mag, img为freq

	double max_mag_c = 0;
	int max_index_c = 0;
	for (int i = 1; i < FFT_N / 2; i++) {
		if (CompxRE[i].real > max_mag_c) {
			max_mag_c = CompxRE[i].real;
			max_index_c = i;
		}
	}
	float phase_c = atan2(CompxIO[max_index_c].imag, CompxIO[max_index_c].real);

	for (int i = 0; i < FFT_N; i++) {
		Refresh_Data(CompxIO, i, (double)g_tAD.usBuf_1[i]);
	}
	FFT(CompxIO);
	Get_Result(CompxIO, CompxRE, sample_rate);	// CompxRE real为mag, img为freq

	double max_mag_v = 0;
	int max_index_v = 0;
	for (int i = 1; i < FFT_N / 2; i++) {
		if (CompxRE[i].real > max_mag_v) {
			max_mag_v = CompxRE[i].real;
			max_index_v = i;
		}
	}
	float phase_v = atan2(CompxIO[max_index_v].imag, CompxIO[max_index_v].real);

	// 计算阻抗
	float z_abs = max_mag_v / max_mag_c / FACTOR * 1000;
	float z_phase = phase_v - phase_c;
	float z_real = z_abs * cos(z_phase);
	float z_imag = z_abs * sin(z_phase);

	// 添加数据
	struct eis_node_t *new_node = (struct eis_node_t *)malloc(sizeof(struct eis_node_t));
	new_node->real = z_real;
	new_node->imag = z_imag;
	new_node->freq = CompxRE[max_index_v].imag;
	new_node->next = NULL;
	if (eis_result.size == 0) {
		eis_result.head = new_node;
		eis_result.tail = new_node;
	} else {
		eis_result.tail->next = new_node;
		eis_result.tail = new_node;
	}
	eis_result.size++;
}

void eis_remove_tail() {
	if (eis_result.size == 0) {
		return;
	}
	if (eis_result.size == 1) {
		free(eis_result.head);
		eis_result.head = NULL;
		eis_result.tail = NULL;
		eis_result.size--;
		return;
	}
	struct eis_node_t *p = eis_result.head;
	while (p->next != eis_result.tail) {
		p = p->next;
	}
	free(eis_result.tail);
	eis_result.tail = p;
	p->next = NULL;
	eis_result.size--;
}

void eis_single_point_process(void){
	if (eis_result.size == 0) {
		struct eis_node_t *new_node = (struct eis_node_t *)malloc(sizeof(struct eis_node_t));
		new_node->real = 0;
		new_node->imag = 0;
		new_node->freq = 0;
		new_node->next = NULL;
		eis_result.head = new_node;
		eis_result.tail = new_node;
		eis_result.size = 1;
		return;
	}
	// 3sigma排除异常数据
	// 先计算实部和虚部的均值
	struct eis_node_t *p = eis_result.head;
	float real_sum = 0;
	float imag_sum = 0;
	while (p != NULL) {
		real_sum += p->real;
		imag_sum += p->imag;
		p = p->next;
	}
	float real_mean = real_sum / eis_result.size;
	float imag_mean = imag_sum / eis_result.size;
	// 计算实部和虚部的方差
	p = eis_result.head;
	float real_var = 0;
	float imag_var = 0;
	while (p != NULL) {
		real_var += pow(p->real - real_mean, 2);
		imag_var += pow(p->imag - imag_mean, 2);
		p = p->next;
	}
	real_var /= eis_result.size;
	imag_var /= eis_result.size;
	// 计算有效数据的均值
	p = eis_result.head;
	float real_sum_valid = 0;
	float imag_sum_valid = 0;
	int valid_size = 0;
	while (p != NULL) {
		if (pow(p->real - real_mean, 2) <= 3 * real_var
				&& pow(p->imag - imag_mean, 2) <= 3 * imag_var) {
			real_sum_valid += p->real;
			imag_sum_valid += p->imag;
			valid_size++;
		}
		p = p->next;
	}
	float real_mean_valid = real_sum_valid / valid_size;
	float imag_mean_valid = imag_sum_valid / valid_size;
	float freq = eis_result.tail->freq;
	eis_result_clear();
	// 添加唯一正确数据
	struct eis_node_t *new_node = (struct eis_node_t *)malloc(sizeof(struct eis_node_t));
	new_node->real = real_mean_valid;
	new_node->imag = imag_mean_valid;
	new_node->freq = freq;
	new_node->next = NULL;
	eis_result.head = new_node;
	eis_result.tail = new_node;
	eis_result.size = 1;
}
