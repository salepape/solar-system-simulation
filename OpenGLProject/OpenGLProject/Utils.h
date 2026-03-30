#ifndef UTILS_H
#define UTILS_H

#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

struct GLFWwindow;
class Window;



class GLFWUtils
{
public:
	// Register user data field to access it later from within static GLFWwindow callbacks (to avoid making variable global)
	static void SetGLFWCallbackData(GLFWwindow* GLFWWindow, Window* data);

	// Access user data field from within static GLFWwindow callbacks
	static Window* GetGLFWCallbackData(GLFWwindow* GLFWWindow);
};



// Utility class gathering functions to process and parse directories and files
class FileUtils
{
public:
	// Return file buffer content as a string
	static std::string ReadFile(const std::string& path);

	static void ListPaths(const std::filesystem::path& inMap, std::unordered_map<std::string, std::filesystem::path>& outPaths);

	// Get model name from path (e.g. by convention, texture name stored in folders follows: [num]k_[bodyName]_[bodyNameOptionalPrecisions])
	static std::string GetNameFromPath(const std::filesystem::path& inPath);

private:
	static std::string GetErrorStateFlagMessage(const std::ifstream& fileStream);
};



// Store a list of strings corresponding to each line of a CSV file provided as an argument
class ResourceCSVParser
{
	using CSVLine = std::vector<std::string>;

public:
	ResourceCSVParser(const std::filesystem::path& inDataPath);

	void ReadCSV(const std::filesystem::path& inDataPath);

	const std::vector<CSVLine>& GetParsedCSV() const { return csvParsed; }
	const CSVLine& GetParsedCSVLine(const std::string& name);

private:
	std::vector<CSVLine> csvParsed;
};



#endif // UTILS_H