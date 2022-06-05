
#include "cgp/cgp.hpp" // Give access to the complete CGP library
#include <iostream>  

// Custom scene of this code
#include "scene.hpp"


// *************************** //
// Global Variables
// *************************** //

// A helper tool to factorize common default functions (Window initialization, code that starts frame, etc)
cgp::helper_common_scene helper_common;


// The custom structure of the current scene defined in "scene.hpp"
scene_structure scene;




// *************************** //
// Start of the program
// *************************** //

GLFWwindow* standard_window_initialization(int width=0, int height=0);

int main(int, char* argv[])
{
	std::cout << "Run " << argv[0] << std::endl;


	// ************************ //
	//     INITIALISATION
	// ************************ //
	
	// Standard Initialization with dimension in pixels
	GLFWwindow* window = standard_window_initialization(); 
	
	// Custom scene initialization
	std::cout << "Initialize data of the scene ..." << std::endl;
	scene.initialize();                                              
	std::cout << "Initialization success" << std::endl;


	// ************************ //
	//     Animation Loop
	// ************************ //
	std::cout<<"Start animation loop ..."<<std::endl;
	bool camera = true;
	cgp::timer_basic timerBase;
	while (!glfwWindowShouldClose(window))
	{
		// Reset the screen for a new frame
		helper_common.frame_begin(scene.environment.background_color, window, scene.inputs.window, scene.inputs.mouse.on_gui);
		scene.environment.projection.update_aspect_ratio(scene.inputs.window.aspect_ratio());
		
		// Display the ImGUI interface (button, sliders, etc)
		scene.display_gui();

		// Call the display of the scene
		timerBase.update();

		if (timerBase.t < 17.0 && camera)
		{
			scene.update_cameraPresentation();
			
		}
		else if (!scene.inputs.keyboard.shift)
		{
			//std::cout << "shift" << std::endl;
			scene.update_cameraManuelle();
		}
		else
		{
			scene.update_cameraArriereManuelle();
		}
		scene.display();
		
		// End of ImGui display and handle GLFW events
		helper_common.frame_end(window);
	}
	
	// Cleanup
	cgp::imgui_cleanup();
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}



// This function is called everytime the window is resized
void window_size_callback(GLFWwindow* , int width, int height)
{
	scene.inputs.window = { width, height };
}

// This function is called everytime the mouse is moved
void mouse_move_callback(GLFWwindow* /*window*/, double xpos, double ypos)
{
	scene.inputs.mouse_position_update( { xpos, ypos } );

	// Apply camera rotation only if shift is not pressed
	if (!scene.inputs.keyboard.shift)
	camera_standard_behavior_rotation(scene.environment.camera, scene.inputs);

}

// This function is called everytime a mouse button is clicked/released
void mouse_click_callback(GLFWwindow* /*window*/, int button, int action, int /*mods*/)
{
	scene.inputs.mouse.click.update_from_glfw_click(button, action);
}

// This function is called everytime a keyboard touch is pressed/released
void keyboard_callback(GLFWwindow* /*window*/, int key, int , int action, int /*mods*/)
{
	scene.inputs.keyboard.update_from_glfw_key(key, action);
}

// Standard initialization procedure
GLFWwindow* standard_window_initialization(int width_target, int height_target)
{
	// Create the window using GLFW
	GLFWwindow* window = cgp::create_window(width_target, height_target);

	// Update storage for window size for the scene
	int width = 0, height = 0;
	glfwGetWindowSize(window, &width, &height);
	scene.inputs.window = { width, height };
	

	// Display debug information on command line
	std::cout << cgp::opengl_info_display() << std::endl;

	// Initialize ImGUI
	cgp::imgui_init(window);

	// Set the callback functions for the inputs
	glfwSetKeyCallback(window, keyboard_callback);            // Event called when a keyboard touch is pressed/released
	glfwSetMouseButtonCallback(window, mouse_click_callback); // Event called when a button of the mouse is clicked/released
	glfwSetCursorPosCallback(window, mouse_move_callback);    // Event called when the mouse is moved
	glfwSetWindowSizeCallback(window, window_size_callback);  // Event called when the window is rescaled        

	// Load default shader and initialize default frame
	helper_common.initialize();

	return window;
}

