#include "pch.h"
#include "stat_data.h"
using namespace std;

//����ƽ�����
void StatData::cal_ave_bais()
{	
	//ͳ�����е��ƽ�����
	float sum = 0;
	for (float &item : total_bias)
	{
		sum += item;
	}
	total_ave_bias = sum / total_bias.size();

	//ͳ�Ƴ�����ֵ��ƽ�����
	sum = 0;
	for (float &item : exc_bias)
	{
		sum += item;
	}
	exc_ave_bias = sum / exc_bias.size();
}

//��ʾͳ����Ϣ
void StatData::show_data()
{
	std::cout << "=================ͳ����Ϣ===================" << std::endl;
	std::cout << "total_amount:"	<< total_amount << std::endl;
	std::cout << "exc_amount:" << exc_amount << std::endl;
	std::cout << "total_ave_bias:" << total_ave_bias << std::endl;
	std::cout << "exc_ave_bias:" << exc_ave_bias << std::endl;
	std::cout << "has_converged:" << has_converged << std::endl;
	std::cout << "fitness_score:" << fitness_score << std::endl;
	std::cout << "trans_mat:" << std::endl;
	std::cout << trans_mat << std::endl;
	std::cout << "=================ͳ����Ϣ����=================" << std::endl;
}

//����ͳ����Ϣ
void StatData::save_data(Properties &prop)
{
	std::ofstream file_out(prop.output_file_total_bias);
	//ͳ�����е����Ϣ
	if (file_out)
	{
		file_out << "vertex amount:" << total_amount << std::endl;
		file_out << "exc_vertex amount:" << exc_amount << std::endl;
		file_out << "total_ave_bias:" << total_ave_bias << std::endl;
		file_out << std::setw(10) << std::setfill(' ') << "index_1" << " ";
		file_out << std::setw(10) << std::setfill(' ') << "index_2" << " ";
		file_out << std::setw(10) << std::setfill(' ') << "point_1" << " ";
		file_out << std::setw(30) << std::setfill(' ') << "point_2" << " ";
		file_out << std::setw(30) << std::setfill(' ') << "bias " << " ";
		file_out << std::endl;

		for (int i = 0; i < total_bias.size(); i++)
		{
			file_out << std::setw(10) << std::setfill(' ') << i << " ";
			file_out << std::setw(10) << std::setfill(' ') << nn_index[i] << " ";
			file_out << std::setw(10) << std::setfill(' ') << point_cloud_icp_res_ptr->points[i].x << " ";
			file_out << std::setw(10) << std::setfill(' ') << point_cloud_icp_res_ptr->points[i].y << " ";
			file_out << std::setw(10) << std::setfill(' ') << point_cloud_icp_res_ptr->points[i].z << " ";
			file_out << std::setw(10) << std::setfill(' ') << point_cloud_origin_ptr->points[nn_index[i]].x << " ";
			file_out << std::setw(10) << std::setfill(' ') << point_cloud_origin_ptr->points[nn_index[i]].y << " ";
			file_out << std::setw(10) << std::setfill(' ') << point_cloud_origin_ptr->points[nn_index[i]].z << " ";
			file_out << std::setw(10) << std::setfill(' ') << total_bias[i] << " ";
			file_out << std::endl;
		}
	}
	else
	{	
		std::cerr << prop.output_file_total_bias << "��ʧ��" << std::endl;
		return;
	}

	//ͳ�Ƴ�����ֵ�������
	file_out.clear();
	file_out.close();	//��Ҫ��Ҫ�ȹر��ļ����������´�
	file_out.open(prop.output_file_exc_bias);
	if (file_out)
	{
		file_out << "vertex amount:" << total_amount << std::endl;
		file_out << "exc_vertex amount:" << exc_amount << std::endl;
		file_out << "exc_ave_bias:" << exc_ave_bias << std::endl;
		file_out << std::setw(10) << std::setfill(' ') << "index_1" << " ";
		file_out << std::setw(10) << std::setfill(' ') << "index_2" << " ";
		file_out << std::setw(10) << std::setfill(' ') << "point_1" << " ";
		file_out << std::setw(30) << std::setfill(' ') << "point_2" << " ";
		file_out << std::setw(30) << std::setfill(' ') << "bias " << " ";
		file_out << std::endl;

		for (int i = 0; i < flags.size(); i++)
		{
			if (flags[i])
			{
				file_out << std::setw(10) << std::setfill(' ') << i << " ";
				file_out << std::setw(10) << std::setfill(' ') << nn_index[i] << " ";
				file_out << std::setw(10) << std::setfill(' ') << point_cloud_icp_res_ptr->points[i].x << " ";
				file_out << std::setw(10) << std::setfill(' ') << point_cloud_icp_res_ptr->points[i].y << " ";
				file_out << std::setw(10) << std::setfill(' ') << point_cloud_icp_res_ptr->points[i].z << " ";
				file_out << std::setw(10) << std::setfill(' ') << point_cloud_origin_ptr->points[nn_index[i]].x << " ";
				file_out << std::setw(10) << std::setfill(' ') << point_cloud_origin_ptr->points[nn_index[i]].y << " ";
				file_out << std::setw(10) << std::setfill(' ') << point_cloud_origin_ptr->points[nn_index[i]].z << " ";
				file_out << std::setw(10) << std::setfill(' ') << total_bias[i] << " ";
				file_out << std::endl;
			}
		}
	}
	else
	{
		std::cerr << prop.output_file_exc_bias << "��ʧ��" << std::endl;
		return;
	}

	//ͳ����ת����
	file_out.clear();
	file_out.close();
	file_out.open(prop.output_file_trans_mat);
	if (file_out)
	{
		file_out << "has_converged:" << has_converged << std::endl;
		file_out << "fitness_score:" << fitness_score << std::endl;
		file_out << "trans_mat:" << std::endl;
		file_out << std::setw(10) << std::setfill(' ') << trans_mat(0, 0) << " ";
		file_out << std::setw(10) << std::setfill(' ') << trans_mat(0, 1) << " ";
		file_out << std::setw(10) << std::setfill(' ') << trans_mat(0, 2) << " ";
		file_out << std::setw(10) << std::setfill(' ') << trans_mat(0, 3) << std::endl;

		file_out << std::setw(10) << std::setfill(' ') << trans_mat(1, 0) << " ";
		file_out << std::setw(10) << std::setfill(' ') << trans_mat(1, 1) << " ";
		file_out << std::setw(10) << std::setfill(' ') << trans_mat(1, 2) << " ";
		file_out << std::setw(10) << std::setfill(' ') << trans_mat(1, 3) << std::endl;

		file_out << std::setw(10) << std::setfill(' ') << trans_mat(2, 0) << " ";
		file_out << std::setw(10) << std::setfill(' ') << trans_mat(2, 1) << " ";
		file_out << std::setw(10) << std::setfill(' ') << trans_mat(2, 2) << " ";
		file_out << std::setw(10) << std::setfill(' ') << trans_mat(2, 3) << std::endl;

		file_out << std::setw(10) << std::setfill(' ') << trans_mat(3, 0) << " ";
		file_out << std::setw(10) << std::setfill(' ') << trans_mat(3, 1) << " ";
		file_out << std::setw(10) << std::setfill(' ') << trans_mat(3, 2) << " ";
		file_out << std::setw(10) << std::setfill(' ') << trans_mat(3, 3) << std::endl;

	}
	else
	{
		std::cerr << prop.output_file_trans_mat << "��ʧ��" << std::endl;
		return;
	}

	file_out.close();
}