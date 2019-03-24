#pragma once
#include "sim_loader.hpp"

#include <cassert>
#include <math.h>       /* cbrt */
#include <chrono>
#include <thread>
#include <string>

using namespace Eigen;
/*
SimData::SimData()
{
	std::string fp;
	std::getline(std::cin, fp);
	sim_count = 0;
	input_sim_record_bin(fp);
	SimulationState simu_state = sim_rec.states[0];
	particles_num = simu_state.particles.size();
	particle_radius = sim_rec.unit_particle_length/2.0;
	total_frame_num = sim_rec.states.size();
	real_time_step = sim_rec.timestep;
	total_time = real_time_step*total_frame_num;

	file_name = fp;
}
*/

SimData::SimData(std::string fp)
{
	sim_count = 0;
	input_sim_record_bin(fp);
	SimulationState simu_state = sim_rec.states[0];
	particles_num = simu_state.particles.size();
	particle_radius = sim_rec.unit_particle_length/2.0;
	total_frame_num = sim_rec.states.size();
	real_time_step = sim_rec.timestep;
	total_time = real_time_step*total_frame_num;

	file_name = fp;
}

void SimData::input_sim_record_xml(std::string fp)
{
	std::ifstream file(fp);
	cereal::XMLInputArchive input(file); // stream to cout
	input(sim_rec);  //not good... maybe directly ar the vector
}

void SimData::input_sim_record_bin(std::string fp)
{
	std::ifstream file(fp);
	cereal::BinaryInputArchive input(file); // stream to cout
	input(sim_rec);  //not good... maybe directly ar the vector
}

