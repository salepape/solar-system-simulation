#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H



// Singleton class to access input system without being tight to GLFW
class InputHandler
{
public:
	static InputHandler& GetInstance();

	// @todo - Implement our own key codes
	static bool IsKeyPressed(const int keyCode);
	static bool IsKeyReleased(const int keyCode);

	static bool IsMouseButtonPressed(const int buttonCode);

private:
	static InputHandler* instance;
};


#endif // INPUT_HANDLER_H