/**
* ViewState is the internal management structure to enable multiple views in the GL window.
* Views can be switched with PAGE[UP/DOWN] keys. This will also change the current shader in the console.
* A stack model is implemented, so that a view can access the view below via shader textures.
* The idea is that each view depends on the underlying view to build its frame.
* Therefore when viewing the top most image, the whole stack is rendered from bottom up step by step using offscreen framebuffers.
* Simple use case: Use pushGLView() followed by createGLQuad() to postprocess underlying view in shader of new view.
* Note: Painter-style layering is currently not intended and framebuffer is cleared before new view is rendered.
*/
struct ViewState {
	GLuint vao_ = 0;

	// Holds GL shader handles
	GLuint vertexShader_ = 0;
	GLuint fragmentShader_ = 0;
	GLuint shaderProgram_ = 0;

	// Holds declarations of currently added buffers, samplers etc.
	// Uniform locations use range that doesnt collide with buffers.
	// Max locations must be at least 1024 per GL spec.
	std::string glslUniformString_ = 
"layout(location=42) uniform mat4 PROJ = mat4(1);\n\
layout(location=43) uniform vec2 PX_SIZE;\n\
layout(location=44) uniform float POINT_SIZE;\n\
layout(location=45) uniform float DELTA_T = 0.005;\n\
layout(location=46) uniform vec3 L = vec3(0, 0, 1);\n";

	// Holds fragment shader code, that can be extended via input at runtime
	std::string fragmentShaderSource_ =
"in vec3 p;\n\
out vec4 color;\n\
int i; float f;\n\
void main() {\n\
  color = vec4(p, 1);\n";

	// Holds a copy of the above after input was made, gets written back when successfully compiled
	std::string fragmentShaderSourceTmp_;

	// Holds local subrange of image units, needed to set uniforms before using them in shader
	// Framebuffer images are produced by the previous view
	unsigned int imageCount_ = 0, framebufferImageCount_ = 0;
	int imageOffset_ = -1, framebufferImageOffset_ = -1;

	// Holds number of vertices added through createGLTriangles2D
	GLsizei currentVertexCount_ = 0;

	// Holds current drawing primitive
	GLenum currentPrimitive_ = GL_TRIANGLES;

	// Holds mat4 for vertex transform
	float* projection_ = 0;

	// Holds offscreen framebuffer where this view is rendered when used by higher view
	GLuint framebuffer_ = 0;

	// Holds number of repeated executions of same shader on same geometry into same framebuffer
	// but using framebuffer images from the last pass instead of lower view
	int numPasses_ = 1;

}

// Public interfaces:

/**
*
*/
struct GLVertexHandle {
	unsigned int vbo, vao;
};

/**
*
*/
void createGLContexts(void* outDeviceContext = 0, void* outRenderContext = 0);

/**
*
*/
void createGLQuad();

/**
*
*/
void createGLTriangles2D(size_t bytes, void* outBuffer, void* data = 0);

/**
*
*/
void createGLPoints2D(size_t bytes, GLVertexHandle* outHandle, void* data = 0, int stride = 0);

/**
*
*/
void updateGLVertexData(GLVertexHandle handle, size_t bytes, void* data);

/**
*
*/
void createGLImage(int w, int h, void* outImageHandle = 0, void* data = 0, int channels = 1);

/**
*
*/
void openGLWindowAndREPL();

/**
*
*/
bool processWindowsMessage(unsigned int* mouse = 0, bool* mouseDown = 0, char* pressedKey = 0);

/**
* Acts as ImGui float slider and shader uniform
*/
struct GLShaderParam {
	const char* name = 0;
	float* ptr = 0;
	float minVal = .0f, maxVal = 1.f;
};

/**
*
*/
void runGLShader(GLShaderParam slot1 = GLShaderParam(), GLShaderParam slot2 = GLShaderParam(), GLShaderParam slot3 = GLShaderParam());

/**
*
*/
void swapGLBuffers(double hz);

/**
*
*/
void closeGLWindowAndREPL();

/**
*
*/
void getGLWindowSize(unsigned int* s);

/**
*
*/
void pushGLView(float* proj = 0);

void updateGLLightSource(float x, float y, float z);