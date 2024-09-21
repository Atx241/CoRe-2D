#include "Renderer.h"
using namespace Core;
using namespace System;
using namespace System::Runtime::CompilerServices;
namespace Core {

	public ref class Utils sealed abstract {
	public:
		static void Init(GameFunc^ start, GameFunc^ update, Vector3^ clearColor) {
			prevTime = DateTime::Now;
			init(start, update, updateDt, *clearColor);
		}
		static void Init(GameFunc^ start, GameFunc^ update) {
			prevTime = DateTime::Now;
			init(start, update, updateDt);
		}
		static double DeltaTime;
	private:
		static DateTime prevTime;
		static void updateDt() {
			DeltaTime = (DateTime::Now - prevTime).TotalSeconds;
			prevTime = DateTime::Now;
		}
	};
}