#include "Auxiliary.h"
#include "Score.h"
#include "Sensor.h"
#include "Encoder.h"

class Simulator {
	int numOfHouses = 0; // based on number of files in folder (before threads start)
	atomic<int> numOfWorkingHouses{ 0 }; // based on proper formatted and readable files (while threads work)
	int numOfAlgorithms = 0;
	int numOfThreads = 1;
	bool score_loaded = false;
	void* score_hndl = NULL;
	int (*score_function)(const map<string, int>& score_params);
	atomic<int> nextHouse{ 0 };
	vector<string> houseFileNames; // full path + ending (.house)
	unique_ptr<bool[]> isValidHouses;
	unique_ptr<string[]> houseErrors;
	unique_ptr<string[]> walkingIntoWallsErrors;
	vector<string> videoErrors;
	bool algorithmIntoWall = false;
	// map algorithm (name) to vector of scores (score[i] for houses[i])
	// no intereference between different threads because each one accesses a different index in the score array
	map<string, unique_ptr<int[]>> scores;
	int isErrorInScoreCalc = false;
	AlgorithmRegistrar& registrar = AlgorithmRegistrar::getInstance();
	map<string, int> config = {};
	bool isFlagVideoUp = false;
	vector<string> flags{ defaultConfigPath, defaultScorePath, defaultHousePath, defaultAlgorithmPath, defaultThreads };
public:
	void handleArguments(int argc, const char* argv[]);
	int handleAll();
	int handleConfiguration();
	int handleScore();
	int handleAlgorithms();
	int handleHouses();
	void handleThreads();
	int handleVideo();
	void startSimulation();
	void threadSimulation();
	void runThreadOnHouse(int houseIndex);
	void printScores();
	void printErrors();
	void end();
};