#ifndef HELPERS_H
#define HELPERS_H

#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

struct GLFWwindow;
class Window;



class GLFWHelper
{
public:
	// Register the callback that will be called each time GLFW returns an error code/message (preferred over multiple uses of glfwGetError())
	static void SetErrorHandlingGLFWCallback();

	// Register user of current GLFW Window to access contextual data from within static GLFWwindow callbacks later on (avoid making global variable)
	static void SetGLFWWindowPointerToUserData(GLFWwindow* GLFWWindow, Window* data);

	// Access contextual user data of current GLFW Window from within static GLFWwindow callbacks
	static Window* GetGLFWWindowPointerToUserData(GLFWwindow* GLFWWindow);
};



// Utility class gathering functions to process and parse directories and files
class FileHelper
{
public:
	// Return file buffer content as an std::string
	static std::string ReadFile(const std::filesystem::path& path);

	static void ListModelPaths(const std::filesystem::path& inDirectory, std::unordered_map<std::string, std::filesystem::path>& outPaths);

	// Get model name from path (e.g. by convention, texture name stored in folders follows: [num]k_[bodyName]_[bodyNameOptionalPrecisions])
	static std::string GetModelNameFromPath(const std::filesystem::path& inPath);

	// Retrieve the path a Material .mtl file for which the concerned line is provided as an argument, including a comment we need to remove 
	static std::string GetTexturePathFromMtlLine(const std::string& mtlLine);

	static std::string GetSolutionAbsolutePath();
	static std::string GetProjectAbsolutePath();

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

	std::size_t GetCSVLinesCount() const { return csvParsed.size(); }

private:
	std::vector<CSVLine> csvParsed;
};



#endif // HELPERS_H