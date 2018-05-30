#ifndef VG_CAMERA_OP_3_H
#define VG_CAMERA_OP_3_H

#include "graphics/scene/camera_op.hpp"

namespace vg
{
    class CameraOP3 : public CameraOP<SpaceType::SPACE_3>
    {
    public:
        CameraOP3();
    private:
        virtual void _applyProj() override;
    };
} //namespace kgs

#endif // !VG_CAMERA_OP_3_H
