#ifndef HELPERS_H
#define HELPERS_H

#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

struct GLFWwindow;
class Window;



namespace GLFWHelper
{
	// Register the callback that will be called each time GLFW returns an error code/message (preferred over multiple uses of glfwGetError())
	void SetErrorHandlingGLFWCallback();

	// Register user of current GLFW Window to access contextual data from within GLFWwindow callbacks later on (avoid making global variable)
	void SetGLFWWindowPointerToUserData(GLFWwindow* GLFWWindow, Window* data);

	// Access contextual user data of current GLFW Window from within GLFWwindow callbacks
	Window* GetGLFWWindowPointerToUserData(GLFWwindow* GLFWWindow);
};



// Utility class gathering functions to process and parse directories and files
namespace FileHelper
{
	// Return file buffer content as an std::string
	std::string ReadFile(const std::filesystem::path& path);

	void ListModelPaths(const std::filesystem::path& inDirectory, std::unordered_map<std::string, std::filesystem::path>& outPaths);

	// Get model name from path (e.g. by convention, texture name stored in folders follows: [num]k_[bodyName]_[bodyNameOptionalPrecisions])
	std::string GetModelNameFromPath(const std::filesystem::path& inPath);

	// Retrieve the path a Material .mtl file for which the concerned line is provided as an argument, including a comment we need to remove 
	std::string GetTexturePathFromMtlLine(const std::string& mtlLine);

	std::string GetSolutionAbsolutePath();
	std::string GetProjectAbsolutePath();

	std::string GetErrorStateFlagMessage(const std::ifstream& fileStream);
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

	std::size_t GetCSVLinesCount() const { return csvParsed.size(); }

private:
	std::vector<CSVLine> csvParsed;
};



#endif // HELPERS_H
