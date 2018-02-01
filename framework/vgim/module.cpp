#include "vgim/module.hpp"

namespace vgim {
	void static moduleCreate()
	{
		if (isInited == VG_TRUE) return;
		//Indicate module was initialized.
		isInited = VG_IM_TRUE;
	}

	void moduleDestory()
	{

		isInited = VG_IM_FALSE;
	}

    void renderImGUI()
    {
        auto drawData = ImGui::GetDrawData();
		
    }
} //vgim