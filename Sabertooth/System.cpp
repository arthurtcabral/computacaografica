#include "System.h"

static bool keys[1024];

System::System()
{
}


System::~System()
{
}

int System::GLFWInit()
{

	left = 0;
	right = 0;
	up = 0;

	glfwInit();
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE );
	glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
	glfwWindowHint( GLFW_SAMPLES, 4 );

	window = glfwCreateWindow( WIDTH, HEIGHT, "Sabertooth", nullptr, nullptr );

	glfwSetKeyCallback(window, KeyCallback);

	glfwGetFramebufferSize( window, &screenWidth, &screenHeight );

	if ( window == nullptr ) {
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent( window );

	glewExperimental = GL_TRUE;

	if ( glewInit() != GLEW_OK ) {
		std::cout << "Failed no init GLEW." << std::endl;
		return EXIT_FAILURE;
	}

	glViewport( 0, 0, screenWidth, screenHeight );

	return EXIT_SUCCESS;

}

int System::OpenGLSetup()
{

	glEnable( GL_BLEND );	// Enables blending ( glBlendFunc )
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	glEnable( GL_DEPTH_TEST );

	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );
	glFrontFace( GL_CW );

	return EXIT_SUCCESS;
}

void System::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

int System::SystemSetup()
{

	coreShader = Shader( "Shaders/Core/core.vert", "Shaders/Core/core.frag" );
	coreShader.Use();

	return EXIT_SUCCESS;
}

void System::Run()
{
	coreShader.Use();
	coreShader.LoadTexture( "bin/Images/woodTexture.jpg", "texture1", "woodTexture" );

	GLfloat vertices[] =
	{
		// Positions         // Textures

		 0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // Top Right
		 0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // Bottom Right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // Bottom Left
		
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // Bottom Left
		-0.5f,  0.5f, 0.0f,   0.0f, 1.0f, // Top Left
		 0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // Top Right
	};

	GLuint VBO, VAO;
	glGenVertexArrays( 1, &VAO );
	glGenBuffers( 1, &VBO );

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray( VAO );

	glBindBuffer( GL_ARRAY_BUFFER, VBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

	// Position attribute
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof( GLfloat ), ( GLvoid * ) 0 );
	glEnableVertexAttribArray( 0 );

	// Texture attribute
	glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof( GLfloat ), ( GLvoid * ) ( 3 * sizeof( GLfloat ) ) );
	glEnableVertexAttribArray( 1 );

	glBindVertexArray( 0 ); // Unbind VAO
	
	while ( !glfwWindowShouldClose( window ) ) {

		glfwPollEvents();

#pragma region Input Handling

		if ( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS ) {
			glfwSetWindowShouldClose( window, GLFW_TRUE );
		}

#pragma endregion

		DoMovement();

		glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		coreShader.Use();

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(left, right, up));

		GLint modelLoc = glGetUniformLocation(coreShader.program, "model");
		glUseProgram(coreShader.program);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		coreShader.UseTexture( "woodTexture" );
		
		glBindVertexArray( VAO );
		glDrawArrays( GL_TRIANGLES, 0, 6 );
		glBindVertexArray( 0 );

		glfwSwapBuffers( window );
	}


}

void System::DoMovement() 
{
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		up += 0.001f;
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		up -= 0.001f;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		left += 0.001f;
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		right += 0.001f;
	}

	if (keys[GLFW_KEY_ESCAPE])
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void System::Finish()
{
	coreShader.Delete();

	glfwTerminate();
}
