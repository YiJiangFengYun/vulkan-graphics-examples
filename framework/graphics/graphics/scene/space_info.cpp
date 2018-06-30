#include "graphics/scene/space_info.hpp"

namespace vg
{
    Space::Space(SpaceType spaceType
        , Bool32 rightHand
        )
        : spaceType(spaceType)
        , rightHand(rightHand)
    {

    }

    Matrix4x4 Space::getVulkanProjMatrix(Matrix4x4 projMatrix) const
    {
        if (rightHand == VG_FALSE)
        {
            projMatrix[1][1] *= -1;
        }
        else
        {
            vg::Vector3 eulerAngles = vg::Vector3(glm::radians(-90.0f), 0.0f, 0.0f);
            vg::Quaternion changeToGLCoord = vg::Quaternion(eulerAngles);
            projMatrix = projMatrix * glm::toMat4(changeToGLCoord);
        }
        return projMatrix;
    }
}