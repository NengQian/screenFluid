#pragma once

#include "math_types.hpp"  //maybe even math_types is not needs
#include "sim_record.hpp"
#include "Particle.hpp"

#include <iostream>
#include <fstream>
#include <string>

#include <cereal/types/vector.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/xml.hpp>

class SimData
{
public:

	SimData();
	SimData(std::string fp);

	/*------------for cereals to read data---*/
	void input_sim_record_xml(std::string fp);
	void input_sim_record_bin(std::string fp);

	SimulationRecord sim_rec;
	int sim_count;
	int total_frame_num;
	int particles_num;
	Real particle_radius;
	Real real_time_step;
	Real total_time;

	std::string file_name;
};
