#include "Renderer.h"
using namespace Core;
using namespace System;
using namespace System::Runtime::CompilerServices;
namespace Core {
	public ref class Utils {
	public:
		static void Init(GameFunc^ start, GameFunc^ update, Vector3^ clearColor) {
			init(start, update, *clearColor);
		}
	};
}