#include "ch.h"

// ccdb
#include <CCDB/Calibration.h>
#include <CCDB/Model/Assignment.h>
#include <CCDB/CalibrationGenerator.h>
using namespace ccdb;

bool chPlugin::loadConstants(int runno, string variation)
{

	string connection = "mysql://clas12reader@clasdb.jlab.org/clas12";
	unique_ptr<Calibration> calib(CalibrationGenerator::CreateCalibration(connection));

	int icomponent;
	vector<vector<double> > data;

	string database = "/calibration/ft/ftcal/noise:" + to_string(runno) + ":"  + variation;
	gLogMessage("FT-CAL: Getting noise from " + database);
	data.clear(); calib->GetCalib(data, database);
	for(unsigned row = 0; row < data.size(); row++) {
		icomponent   = data[row][2];
		pedestal[icomponent] = data[row][3];         pedestal[icomponent] = 101.;   // When DB will be filled, I should remove this
		pedestal_rms[icomponent] = data[row][4];     pedestal_rms[icomponent] = 2.; // When DB will be filled, I should remove this
		noise[icomponent] = data[row][5];
		noise_rms[icomponent] = data[row][6];
		threshold[icomponent] = data[row][7];
	}


	database = "/calibration/ft/ftcal/charge_to_energy:" + to_string(runno) + ":"  + variation;
	gLogMessage("FT-CAL: Getting charge_to_energy from " + database);
	data.clear(); calib->GetCalib(data, database);
	for(unsigned row = 0; row < data.size(); row++) {
		icomponent   = data[row][2];
		mips_charge[icomponent] = data[row][3];
		mips_energy[icomponent] = data[row][4];
		fadc_to_charge[icomponent] = data[row][5];
		preamp_gain[icomponent] = data[row][6];
		apd_gain[icomponent] = data[row][7];
	}

	database = "/calibration/ft/ftcal/time_offsets:" + to_string(runno) + ":"  + variation;
	gLogMessage("FT-CAL: Getting time_offsets from " + database);
	data.clear(); calib->GetCalib(data, database);
	for(unsigned row = 0; row < data.size(); row++) {
		icomponent   = data[row][2];
		time_offset[icomponent] = data[row][3];
		time_rms[icomponent] = data[row][4];
	}

	return true;
}
