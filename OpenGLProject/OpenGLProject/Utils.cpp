#include "Utils.h"

#include <algorithm>
#include <cassert>
#include <cctype>  // std::toupper()
#include <cstddef> // std::size_t
#include <cstdint>
#include <glfw3.h>
#include <ios>
#include <iostream>
#include <sstream>

#include "Window.h"



void GLFWUtils::SetGLFWCallbackData(GLFWwindow* GLFWWindow, Window* data)
{
	glfwSetWindowUserPointer(GLFWWindow, static_cast<void*>(data));
}

Window* GLFWUtils::GetGLFWCallbackData(GLFWwindow* GLFWWindow)
{
	return static_cast<Window*>(glfwGetWindowUserPointer(GLFWWindow));
}



void FileUtils::ListPaths(const std::filesystem::path& inMap, std::unordered_map<std::string, std::filesystem::path>& outPaths)
{
	for (const auto& directory : std::filesystem::recursive_directory_iterator(inMap))
	{
		const std::filesystem::path& directoryPath = directory.path();
		const std::string& modelName = GetNameFromPath(directoryPath);
		if (modelName == "")
		{
			continue;
		}

		outPaths[modelName] = directoryPath;
	}
}

std::string FileUtils::GetNameFromPath(const std::filesystem::path& inPath)
{
	const std::string& fileName = inPath.filename().string();

	std::size_t firstTrimSymbol = fileName.find_first_of("_");
	if (firstTrimSymbol == std::string::npos)
	{
		firstTrimSymbol = fileName.find_first_of(".");
		if (firstTrimSymbol == std::string::npos)
		{
			return "";
		}
	}
	const std::string& fileNameWithSizeTrimmed = (firstTrimSymbol == fileName.find_first_of("_")) ?
		fileName.substr(firstTrimSymbol + 1, fileName.length() - firstTrimSymbol - 1) :
		fileName.substr(0, firstTrimSymbol);

	std::size_t lastTrimSymbol = fileNameWithSizeTrimmed.find_first_of("_");
	if (lastTrimSymbol == std::string::npos)
	{
		lastTrimSymbol = fileNameWithSizeTrimmed.find_first_of(".");
	}
	std::string modelName = fileNameWithSizeTrimmed.substr(0, lastTrimSymbol);
	modelName[0] = std::toupper(modelName[0]);

	return modelName;
}

std::string FileUtils::ReadFile(const std::string& path)
{
	std::ifstream fileStream(path, std::ios::in | std::ios::binary);
	if (fileStream.fail())
	{
		std::cout << "ERROR::UTILS - " << "File " << FileUtils::GetNameFromPath(path) << " has not been successfully read.\nReason: " << GetErrorStateFlagMessage(fileStream) << "\n" << std::endl;
		assert(false);
	}

	// Read file buffer content as a stream
	std::stringstream fileStringStream;
	fileStringStream << fileStream.rdbuf();

	fileStream.close();

	return fileStringStream.str();
}

std::string FileUtils::GetErrorStateFlagMessage(const std::ifstream& fileStream)
{
	std::string result;

	std::ios_base::iostate errorStateFlag = fileStream.rdstate();
	switch (errorStateFlag)
	{
	case std::ios::eofbit:
	{
		result = "End-of-File reached on input operation.";
		break;
	}
	case std::ios::failbit:
	{
		result = "Logical error on input/output operation.";
		break;
	}
	case std::ios::badbit:
	{
		result = "Read/writing error on input/output operation.";
		break;
	}
	default:
	{
		result = "No specific error state flag returned by std::fstream().";
		break;
	}
	}

	return result;
}



ResourceCSVParser::ResourceCSVParser(const std::filesystem::path& inDataPath)
{
	ReadCSV(inDataPath);
}

void ResourceCSVParser::ReadCSV(const std::filesystem::path& inDataPath)
{
	const std::string& csvFile = FileUtils::ReadFile(inDataPath.string());
	std::string csvLine;
	std::stringstream csvLineStream(csvFile);

	// Overwrite the first line (i.e. the legend) by reading it before looping
	std::getline(csvLineStream, csvLine, '\n');

	// Read lines from the CSV file until its end (i.e. carriage return) is reached
	while (std::getline(csvLineStream, csvLine, '\n'))
	{
		const std::size_t carriageReturn = csvLine.find('\r');
		if (carriageReturn != std::string::npos)
		{
			csvLine.erase(carriageReturn);
		}

		CSVLine csvLineCells;

		// Read cells from the CSV line until its end (i.e. a comma) is reached
		std::stringstream csvCellStream(csvLine);
		std::string csvCell;
		while (std::getline(csvCellStream, csvCell, ','))
		{
			csvLineCells.emplace_back(csvCell);
		}

		csvParsed.push_back(csvLineCells);
	}
}

const ResourceCSVParser::CSVLine& ResourceCSVParser::GetParsedCSVLine(const std::string& sceneEntityName)
{
	// Scene Entity name is stored on the first CSV cell
	const auto& csvLineIt = std::find_if(csvParsed.begin(), csvParsed.end(), [&sceneEntityName](const CSVLine& csvLine)
	{
		return csvLine[0] == sceneEntityName;
	});

	if (csvLineIt == csvParsed.end())
	{
		std::cout << "ERROR::UTILS - CSV line corresponding to " << sceneEntityName << " has not been found!" << std::endl;
	}

	return *csvLineIt;
}