// clang-format off
#include "rzxpch.h"
// clang-format on
#include "RZGraphNode.h"

namespace Razix {
    namespace Gfx {
        namespace FrameGraph {

            RZGraphNode::RZGraphNode(const std::string_view name, u32 id)
                : m_Name(name), m_ID(id)
            {
            }
        }    // namespace FrameGraph
    }    // namespace Gfx
}    // namespace Razix