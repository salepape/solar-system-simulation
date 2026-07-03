#ifndef APPLICATION_CONTROLS_H
#define APPLICATION_CONTROLS_H



// Detect and process all keyboard and mouse user input relative to the Application 
// (cannot be callback-based, as glfwSetKeyCallback() already used in Perspective Camera Controller)
namespace ApplicationControls
{
	// Keyboard key release action will not be registered if press action has happened at a delta time equal/lower to this value
	constexpr float KEY_RELEASE_SENSITIVITY = 1.0f;

	// Detect input where keyboard key press only triggers a single state
	void ProcessUserInput();
};



#endif // APPLICATION_CONTROLS_H
