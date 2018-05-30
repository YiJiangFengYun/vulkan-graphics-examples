#ifndef VG_VISUAL_OBJECT_3_H
#define VG_VISUAL_OBJECT_3_H

#include "graphics/scene/space_info.hpp"
#include "graphics/scene/visual_object.hpp"

namespace vg
{
    class VisualObject3 : public VisualObject<SpaceType::SPACE_3>
    {
    public:
        VisualObject3();

        template <class VisualizerType>
        VisualObject3()
            : VisualObject<SpaceType::SPACE_3><VisualizerType>()
        {

        }

        virtual Matrix4x4 _getModelMatrix() const override;
    };

} //namespace kgs

#endif // !VG_VISUAL_OBJECT_3_H
