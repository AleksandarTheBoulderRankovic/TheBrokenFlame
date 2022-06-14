#include "Stormlight.h"
#include "Stormlight/Core/EntryPoint.h"
#include "EditorLayer.h"

namespace Stormlight {

	class StormlightEditor : public Application {
	public:
		StormlightEditor() : Application("Stormlight Editor"){
			PushLayer(new EditorLayer());
		}
		~StormlightEditor() {

		}

	};

	Application* CreateApplication() {
		return new StormlightEditor();
	}
}

