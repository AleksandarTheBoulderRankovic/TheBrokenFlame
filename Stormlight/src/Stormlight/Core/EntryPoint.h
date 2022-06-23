#pragma once

#ifdef SL_PLATFORM_WINDOWS

extern Stormlight::Application* Stormlight::CreateApplication();

int main() {

	Stormlight::Log::Init();
	auto app = Stormlight::CreateApplication();
	app->Run();
	delete app;

	return 0;
}

#endif

