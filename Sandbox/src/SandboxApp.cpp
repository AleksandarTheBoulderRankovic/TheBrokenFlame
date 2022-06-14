#include "Stormlight.h"
#include "Stormlight/Core/EntryPoint.h"
#include "ExampleLayer.h"
#include "Sandbox2D.h"

class Sandbox : public Stormlight::Application {
public:
	Sandbox() : Application("Sandbox") {
		PushLayer(new Sandbox2D());
	}
	~Sandbox() {
	
	}

};

Stormlight::Application* Stormlight::CreateApplication() {
	return new Sandbox();
}