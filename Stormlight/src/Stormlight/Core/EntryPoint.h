#pragma once

#ifdef SL_PLATFORM_WINDOWS

extern Stormlight::Application* Stormlight::CreateApplication();

int main() {

	Stormlight::Log::Init();
	SL_CORE_WARN("Log initialized!");
	int a = 5;
	SL_CRITICAL("Hello! var={0}", a);
	SL_CORE_ERROR("ERROR initialized!");
	auto app = Stormlight::CreateApplication();
	app->Run();
	delete app;

	return 0;
}

#endif

