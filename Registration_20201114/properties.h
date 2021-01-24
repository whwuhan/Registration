#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <boost/algorithm/string.hpp>   //split
/**
 * ��������ṹ��
*/
typedef struct Properties
{  
	//����

	//���в�����Ӧ��map
	std::unordered_map<std::string, std::string> prop_in;

	double dis_threshold;                       //������ֵ
	int iter;									//icp��������
	std::string input_format;                   //�����ʽ
	std::string input_file_scan;                //ɨ�����
	std::string input_file_origin;              //ģ�͵���
	std::string output_format;                  //�����ʽ
	std::string output_file_icp;                //icp��׼���
	std::string output_file_outlier;            //��Ⱥ����
	std::string output_file_icp_color;			//����ɫ��icp������Ⱥ�����׼���
	std::string output_file_total_bias;			//���е�����������·��
	std::string output_file_exc_bias;			//������ֵ�������·��
	std::string output_file_trans_mat;			//�任����
	float output_color_grade_1;			//��ɫ�ּ�
	float output_color_grade_2;
	float output_color_grade_3;
	bool outlier_remove;						//�Ƿ���Ҫ����Ⱥ����
	double outlier_radius;						//��Ⱥ����뾶
	int outlier_min_neighbors;					//��Ⱥ������С�ھӸ���

	//����������Ϣ
	int read_data(const std::string property_file = "registration.pcproperties");

	//��ʾ������Ϣ
	void show_properties();

} PROPERTIES;
