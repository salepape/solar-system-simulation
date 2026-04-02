#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H



// Singleton class allowing a global access point for a unique InputHandler instance (without being tight to GLFW)
class InputHandler
{
public:
	// Unique Singleton instance defined in source file
	static InputHandler& GetInstance();

	static bool IsKeyPressed(const int keyCode);
	static bool IsKeyReleased(const int keyCode);

	static bool IsMouseButtonPressed(const int buttonCode);

private:
	static InputHandler* instance;
};


#endif // INPUT_HANDLER_H