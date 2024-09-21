#pragma once
#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "ShaderLoader.h"
#include <vector>
#include <msclr/marshal_cppstd.h>
#include <functional>
using namespace System::Collections::Generic;
namespace Core {
	public ref struct Vector2 {
	public:
		Vector2(double x, double y)
		{
			this->x = x;
			this->y = y;
		}
		Vector2::Vector2(Vector2% copier) {
			x = copier.x;
			y = copier.y;
		}
		Vector2 operator + (Vector2 other) {
			return Vector2(this->x + other.x, this->y + other.y);
		}
		Vector2 operator - (Vector2 other) {
			return Vector2(this->x - other.x, this->y - other.y);
		}
		Vector2 operator * (Vector2 other) {
			return Vector2(this->x * other.x, this->y * other.y);
		}
		Vector2 operator / (Vector2 other) {
			return Vector2(this->x / other.x, this->y / other.y);
		}
		double x;
		double y;
	};
	public ref struct Vector3 {
	public:
		Vector3(double x, double y, double z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}
		Vector3::Vector3(Vector3% copier) {
			x = copier.x;
			y = copier.y;
			z = copier.z;
		}
		Vector3 operator + (Vector3 other) {
			return Vector3(this->x + other.x, this->y + other.y, this->z + other.z);
		}
		Vector3 operator - (Vector3 other) {
			return Vector3(this->x - other.x, this->y - other.y, this->z - other.z);
		}
		Vector3 operator * (Vector3 other) {
			return Vector3(this->x * other.x, this->y * other.y, this->z * other.z);
		}
		Vector3 operator / (Vector3 other) {
			return Vector3(this->x / other.x, this->y / other.y, this->z / other.z);
		}
		double x;
		double y;
		double z;
	};
	public ref struct Texture {
	public:
		Texture(unsigned int dataBuffer, int width, int height)
		{
			this->dataBuffer = dataBuffer;
			this->width = width;
			this->height = height;
		}
		Texture::Texture(Texture% copier) {
			dataBuffer = copier.dataBuffer;
			width = copier.width;
			height = copier.height;
		}
		unsigned int dataBuffer;
		int width;
		int height;
		static Texture^ CreateTexture(array<unsigned char>^ data, int width, int height, bool linear) {
			pin_ptr<unsigned char> dt = &data[0];
			return createTexture(dt, width, height, GL_RGBA, linear);
		}
		static Texture^ CreateTexture(array<unsigned char>^ data, int width, int height) {
			pin_ptr<unsigned char> dt = &data[0];
			return createTexture(dt, width, height, GL_RGBA, true);
		}
	private:
		static Texture^ createTexture(unsigned char* data, int width, int height, int format, bool linear) {
			//Creating buffer variable
			unsigned int tbo;
			//Generating and binding buffer
			glGenTextures(1, &tbo);
			glBindTexture(GL_TEXTURE_2D, tbo);
			//Applying texture parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, linear ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, linear ? GL_LINEAR : GL_NEAREST);
			//Assigning data and generating mipmaps
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			//Unbinding buffer
			glBindTexture(GL_TEXTURE_2D, 0);
			//Returning buffer
			return gcnew Texture(tbo, width, height);
		}
	};
	public ref struct Entity {
	public:
		Entity(Texture^ texture, Vector2^ position, float rotation, Vector2^ scale, float depth)
		{
			this->position = position;
			this->rotation = rotation;
			this->scale = scale;
			this->texture = texture;
			this->depth = depth;
		}
		Entity(Entity^ parent, Texture^ texture, Vector2^ position, float rotation, Vector2^ scale, float depth)
		{
			this->parent = parent;
			parent->children->Add(this);
			this->position = position;
			this->rotation = rotation;
			this->scale = scale;
			this->texture = texture;
			this->depth = depth;
		}
		Vector2^ position = gcnew Vector2(0, 0);
		float rotation = 0;
		Vector2^ scale = gcnew Vector2(0, 0);
		unsigned int depth = 0;
		Texture^ texture = gcnew Texture(0, 1, 1);
		Entity^ parent;
		List<Entity^>^ children = gcnew List<Entity^>();
		static SortedDictionary<int,Entity^>^ renderEntities = gcnew SortedDictionary<int,Entity^>();
		static Entity^ CreateRenderEntity(Entity^ e) {
			return createRenderEntity(e);
		}
		Vector2^ GetGlobalPosition() {
			if (parent == nullptr) {
				return position;
			}
			else {
				return gcnew Vector2(*parent->GetGlobalPosition() + Vector2(position->x * parent->GetGlobalScale()->x * cos(-parent->GetGlobalRotation() / (180 / 3.141592)) + position->y * parent->GetGlobalScale()->y * sin(-parent->GetGlobalRotation() / (180 / 3.141592)), position->x * parent->GetGlobalScale()->x * sin(-parent->GetGlobalRotation() / (180 / 3.141592)) + position->y * parent->GetGlobalScale()->y * cos(-parent->GetGlobalRotation() / (180 / 3.141592))));
			}
		}
		float GetGlobalRotation() {
			if (parent == nullptr) {
				return rotation;
			}
			else {
				return parent->GetGlobalRotation() + rotation;
			}
		}
		Vector2^ GetGlobalScale() {
			if (parent == nullptr) {
				return scale;
			}
			else {
				return gcnew Vector2(*parent->GetGlobalScale() * *scale);
			}
		}
	private:
		static Entity^ createRenderEntity(Entity^ entity) {
			renderEntities->Add(entity->depth, entity);
			return entity;
		}
	};
	public ref struct AnchoredEntity : public Entity {
		AnchoredEntity(Vector2^ anchor, Texture^ texture, Vector2^ position, float rotation, Vector2^ scale, float depth, bool scaleWithScreen) : Entity(texture,position,rotation,scale,depth)
		{
			this->anchor = anchor;
			this->scaleWithScreen = scaleWithScreen;
		}
		AnchoredEntity(Entity^ parent, Vector2^ anchor, Texture^ texture, Vector2^ position, float rotation, Vector2^ scale, float depth, bool scaleWithScreen) : Entity(parent, texture, position, rotation, scale, depth)
		{
			this->anchor = anchor;
			this->scaleWithScreen = scaleWithScreen;
		}
	public:
		Vector2^ anchor = gcnew Vector2(0,0);
		bool scaleWithScreen = false;
		static AnchoredEntity^ CreateRenderEntity(AnchoredEntity^ e) {
			return createRenderEntity(e);
		}
		static SortedDictionary<int, AnchoredEntity^>^ anchoredRenderEntities = gcnew SortedDictionary<int, AnchoredEntity^>();
	private:
		static AnchoredEntity^ createRenderEntity(AnchoredEntity^ entity) {
			anchoredRenderEntities->Add(entity->depth, entity);
			return entity;
		}
	};
	delegate void KeyPress(int key, int action);
	public ref class Input abstract sealed{
	public:
		static event KeyPress^ onKeyPress;
		static event KeyPress^ onClick;
		static Vector2^ mousePosition;
		static void keyPress(int key, int action) {
			Input::onKeyPress(key, action);
		}
		static void click(int button, int action) {
			Input::onClick(button, action);
		}
	};
	public ref class Camera abstract sealed {
	public:
		static Vector2^ cameraPosition;
		static float cameraZoom;
	};
	void keyPress(GLFWwindow* window, int key, int scancode, int action, int mods) {
		Input::keyPress(key, action);
	}
	void click(GLFWwindow* window, int button, int action, int mods) {
		Input::click(button, action);
	}
	void mouseMove(GLFWwindow* window, double xpos, double ypos)
	{
		Input::mousePosition = gcnew Vector2(xpos, ypos);
	}
	unsigned int program = 0;
	namespace internalShaderLocs {
		int posLocation;
		int rotLocation;
		int scaleLocation;
		int aspectRatioLocation;
		int cameraZoomLocation;
		int cameraPositionLocation;
		int anchorLocation;
		int anchoredEntityLocation;
		int scaleWithScreenLocation;
	}
	public delegate void GameFunc();
	private ref class internalFuncs abstract sealed {
	public:
		static GameFunc^ updateFunc;
	};
	std::function<void()> internalUpdate;
	namespace internalMisc {
		GLFWwindow* window;
	}
	void draw() {
		glClear(GL_COLOR_BUFFER_BIT);
		glUniform1f(internalShaderLocs::cameraZoomLocation, Camera::cameraZoom);
		glUniform2f(internalShaderLocs::cameraPositionLocation, Camera::cameraPosition->x, Camera::cameraPosition->y);
		internalUpdate();
		internalFuncs::updateFunc();
		glUniform1i(internalShaderLocs::anchoredEntityLocation, 0);
		glUniform1i(internalShaderLocs::scaleWithScreenLocation, 0);
		glUniform2f(internalShaderLocs::anchorLocation, 0, 0);
		for each (auto _e in Entity::renderEntities) {
			auto e = _e.Value;
			auto p = _e.Value->parent;
			glUniform2f(internalShaderLocs::posLocation, e->GetGlobalPosition()->x, e->GetGlobalPosition()->y);
			glUniform1f(internalShaderLocs::rotLocation, e->GetGlobalRotation());
			glUniform2f(internalShaderLocs::scaleLocation, e->GetGlobalScale()->x, e->GetGlobalScale()->y);
			glBindTexture(GL_TEXTURE_2D, e->texture->dataBuffer);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		glUniform1i(internalShaderLocs::anchoredEntityLocation, 1);
		for each (auto _e in AnchoredEntity::anchoredRenderEntities) {
			auto e = _e.Value;
			glUniform2f(internalShaderLocs::posLocation, e->GetGlobalPosition()->x, e->GetGlobalPosition()->y);
			glUniform1f(internalShaderLocs::rotLocation, e->GetGlobalRotation());
			glUniform2f(internalShaderLocs::scaleLocation, e->GetGlobalScale()->x, e->GetGlobalScale()->y);
			glUniform2f(internalShaderLocs::anchorLocation, e->anchor->x, e->anchor->y);
			glUniform1i(internalShaderLocs::scaleWithScreenLocation, e->scaleWithScreen);
			glBindTexture(GL_TEXTURE_2D, e->texture->dataBuffer);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		glfwSwapBuffers(internalMisc::window);
	}
	void resizeWindow(GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
		int aspectRatioLocation = glGetUniformLocation(program, "aspectRatio");
		glUniform1f(aspectRatioLocation, (float)width / (float)height);
		draw();
	}
	void init(GameFunc^ startFunc, GameFunc^ iupdateFunc, std::function<void()> iinternalUpdate, Vector3 clearColor = Vector3(0.2, 0.25, 1)) {
		//GLFW
		//Initialize GLFW
		if (!glfwInit()) {
			std::cout << "Failed to initialize GLFW!";
		}
		//Create and show window
		int width = 1000, height = 1000;
		GLFWwindow* window = glfwCreateWindow(width, height, "Game", NULL, NULL);
		internalMisc::window = window;
		glfwShowWindow(window);
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, resizeWindow);
		glfwSetKeyCallback(window, keyPress);
		glfwSetMouseButtonCallback(window, click);
		glfwSetCursorPosCallback(window, mouseMove);
		gladLoadGL();
		//Create viewport
		glViewport(0, 0, width, height);
		//BUFFERING
		//Creating vertices for a quad
		float vertices[] = {
			//Triangle 1
			0.5f,0.5f,  1,1,
			0.5f,-0.5f, 1,0,
			-0.5f,0.5f, 0,1,
			//Triangle 2
			-0.5f,-0.5f,0,0,
			0.5f,-0.5f, 1,0,
			-0.5f,0.5f, 0,1
		};
		//Creating buffer variables
		unsigned int vbo, vao;
		//Generating and binding buffers
		glGenBuffers(1, &vbo);
		glGenVertexArrays(1, &vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindVertexArray(vao);
		//Adding data to the buffers
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);
		//CAMERA
		//Setting default values
		Camera::cameraZoom = 1;
		Camera::cameraPosition = gcnew Vector2(0, 0);
		//SHADERS
		//Creating program
		//Loading shaders
		LoadShader("StandardVertex.vert", "StandardFrag.frag", program);
		glUseProgram(program);
		//Uniforms
		float rot = 0;
		float xPos = 1;
		internalShaderLocs::posLocation = glGetUniformLocation(program, "position");
		internalShaderLocs::rotLocation = glGetUniformLocation(program, "rotation");
		internalShaderLocs::scaleLocation = glGetUniformLocation(program, "scale");
		internalShaderLocs::aspectRatioLocation = glGetUniformLocation(program, "aspectRatio");
		internalShaderLocs::cameraZoomLocation = glGetUniformLocation(program, "cameraZoom");
		internalShaderLocs::cameraPositionLocation = glGetUniformLocation(program, "cameraPosition");
		internalShaderLocs::anchorLocation = glGetUniformLocation(program, "anchor");
		internalShaderLocs::anchoredEntityLocation = glGetUniformLocation(program, "anchoredEntity");
		internalShaderLocs::scaleWithScreenLocation = glGetUniformLocation(program, "scaleWithScreen");
		glUniform1f(internalShaderLocs::aspectRatioLocation, width / height);
		glUniform1f(internalShaderLocs::cameraZoomLocation, Camera::cameraZoom);
		glUniform2f(internalShaderLocs::cameraPositionLocation, Camera::cameraPosition->x, Camera::cameraPosition->y);
		//STARTUP
		unsigned char defaultTextureData[] = { 0xFF,0xFF,0xFF };
		glClearColor(clearColor.x, clearColor.y, clearColor.z, 1);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		startFunc();
		//UPDATE
		internalFuncs::updateFunc = iupdateFunc;
		internalUpdate = iinternalUpdate;
		while (!glfwWindowShouldClose(window)) {
			draw();
			glfwPollEvents();
		}
		glfwTerminate();
	}
}