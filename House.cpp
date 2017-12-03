#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "House.h"
#include "Montage.h"


int createDirectoryIfNotExists(const std::string& dirPath)
{
	std::string cmd = "mkdir -p " + dirPath;
	int ret = system(cmd.c_str());
	if (ret == -1)
	{
		return -1;
	}
	return 0;
}


void House::montage(const std::string& algoName, std::vector<std::string>& videoErrors)
{
	std::vector<std::string> tiles;
	for (int row = 0; row < rows; ++row)
	{
		for (int col = 0; col < cols; ++col)
		{
			if ((row == robot.row) && (col == robot.col))
				tiles.push_back("R");
			else if (matrix[row][col] == ' ')
				tiles.push_back("0");
			else
				tiles.push_back(std::string() + matrix[row][col]);
		}
	}
	int indexAlgo = static_cast<int>(algoName.find(".so"));
	string algorithmName = algoName.substr(0, indexAlgo);
	int indexHouse = static_cast<int>(houseFileName.find(".house"));
	string houseName = houseFileName.substr(0, indexHouse);

	imagesDirPath = "simulations/" + algorithmName + "_" + houseName;
	if (createDirectoryIfNotExists(imagesDirPath)) {
		std::string error_msg = "Error: In the simulation " + algorithmName + ", " + houseName + ": folder creation " + imagesDirPath + " failed";
		videoErrors.push_back(error_msg);
		folderError = true;
		return;
	}
	std::string counterStr = std::to_string(picCounter++);
	std::string composedImage = imagesDirPath + "/image" + std::string(5 - counterStr.length(), '0') + counterStr + ".jpg";
	if (Montage::compose(tiles, cols, rows, composedImage)) {
		imageErrors++;
		return;
	}
	// image was created succesfully -> should create a video at the end
	createVideo = true;
}