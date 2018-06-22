#ifndef VG_CAMERA_OP_H
#define VG_CAMERA_OP_H

#include "graphics/scene/camera.hpp"

namespace vg
{
    template <SpaceType SPACE_TYPE>
    class CameraOP : public Camera<SPACE_TYPE>
    {
    public:
        using BoundsType = typename ProjectorOP<SPACE_TYPE>::BoundsType;
        CameraOP();
        void updateProj(BoundsType viewBounds);
        BoundsType getViewBounds() const;
    protected:
    private:
        
    };
} //namespace kgs

#endif // !VG_CAMERA_OP_H
