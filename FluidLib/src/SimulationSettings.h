#pragma once

namespace FluidLib {

	struct Settings {
		int fps = 30;
		float spreading = 0.5;
		float diffuse = 0.9;

		float intesity = 0.5;
		float freezeintensity = 0.5;
		bool editfreeze = false;

		bool surfaceedit = false;
		bool movementedit = false;
	
		bool autosavestate = false;
		
		float renderforce = 0;
		int fast = 0;
	};


}