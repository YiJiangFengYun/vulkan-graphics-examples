#include "vgim/module.hpp"

namespace vgim {
	void static moduleCreate()
	{
		if (isInited == VG_TRUE) return;
		//Indicate module was initialized.
		isInited = ICIMGUI_TRUE;
	}

	void moduleDestory()
	{

		isInited = ICIMGUI_FALSE;
	}

    void renderImGUI()
    {
        auto drawData = ImGui::GetDrawData();
		
    }
} //vgim