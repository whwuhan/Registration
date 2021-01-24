#include "pch.h"
#include "properties.h"
using namespace std;
//��ȡ����
int Properties::read_data(const std::string property_file)
{
	//  �򿪲����ļ�
	std::ifstream file_in(property_file);
	if (!file_in.is_open())
	{
		std::cerr << "�����ļ���ʧ��" << std::endl;
		return 0;
	}
	//  ��ȡ�����ļ����뵽unordered_map��
	std::unordered_map<std::string, std::string> prop_in_unor_map; // ���������map
	std::string line;
	std::vector<std::string> line_split;
	while (getline(file_in, line))
	{
		//std::cerr << line << std::endl;
		boost::split(line_split, line, boost::is_any_of(" ="), boost::token_compress_on);
		prop_in_unor_map[line_split[0]] = line_split[1];
	}
	file_in.close();

	//����properties��
	dis_threshold = std::atof(prop_in_unor_map["dis_threshold"].c_str());
	iter = std::atoi(prop_in_unor_map["iter"].c_str());
	input_file_scan = prop_in_unor_map["input_file_scan"];
	input_file_origin = prop_in_unor_map["input_file_origin"];
	input_format = prop_in_unor_map["input_format"];
	output_format = prop_in_unor_map["output_format"];
	output_file_icp = prop_in_unor_map["output_file_icp"];
	output_file_outlier = prop_in_unor_map["output_file_outlier"];
	output_file_icp_color = prop_in_unor_map["output_file_icp_color"];
	output_file_total_bias = prop_in_unor_map["output_file_total_bias"];
	output_file_exc_bias = prop_in_unor_map["output_file_exc_bias"];
	output_file_trans_mat = prop_in_unor_map["output_file_trans_mat"];
	output_color_grade_1 = std::atof(prop_in_unor_map["output_color_grade_1"].c_str());
	output_color_grade_2 = std::atof(prop_in_unor_map["output_color_grade_2"].c_str());
	output_color_grade_3 = std::atof(prop_in_unor_map["output_color_grade_3"].c_str());
	if (prop_in_unor_map["outlier_remove"] == "true")
	{
		outlier_remove = true;
	}
	else
	{
		outlier_remove = false;
	}
	outlier_radius = std::atof(prop_in_unor_map["outlier_radius"].c_str());
	outlier_min_neighbors = std::atoi(prop_in_unor_map["outlier_min_neighbors"].c_str());

	prop_in = prop_in_unor_map;
	return 1;
}

// ��ʾ����
void Properties::show_properties()
{
	std::cout << "���õĲ������� : " << std::endl;
	for (auto iter = prop_in.cbegin(); iter != prop_in.cend(); iter++)
	{
		cout << iter->first << " : " << iter->second << endl;
	}
}


