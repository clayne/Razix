// clang-format off
#include "rzxpch.h"
// clang-format on
#include "RZTransientResources.h"

template<typename T, typename... Rest>
static inline void hashCombine(std::size_t &seed, const T &v,
    const Rest &...rest)
{
    // https://stackoverflow.com/questions/2590677/how-do-i-combine-hash-values-in-c0x
    seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    (hashCombine(seed, rest), ...);
}

namespace std {

    template<>
    struct hash<Razix::Graphics::FrameGraph::RZFrameGraphTexture::Desc>
    {
        std::size_t operator()(const Razix::Graphics::FrameGraph::RZFrameGraphTexture::Desc &desc) const noexcept
        {
            std::size_t h{0};
            hashCombine(h, desc.extent.x, desc.extent.y, desc.type, desc.name);
            return h;
        }
    };
    template<>
    struct hash<Razix::Graphics::FrameGraph::RZFrameGraphSemaphore::Desc>
    {
        std::size_t operator()(const Razix::Graphics::FrameGraph::RZFrameGraphSemaphore::Desc &desc) const noexcept
        {
            std::size_t h{0};
            hashCombine(h, desc.name);
            return h;
        }
    };

}    // namespace std

namespace Razix {
    namespace Graphics {
        namespace FrameGraph {

            void RZTransientResources::destroyResources()
            {
                for (auto &texture: m_Textures) {
                    if (*texture.get())
                        (*texture.get())->Release(true);
                }

                for (auto &semaphore: m_Semaphores)
                    (*semaphore.get())->Destroy();
            }

            Razix::Graphics::RZTexture *RZTransientResources::acquireTexture(const RZFrameGraphTexture::Desc &desc)
            {
                const auto hash = std::hash<RZFrameGraphTexture::Desc>()(desc);
                auto      &pool = m_TexturePools[hash];
                if (pool.empty()) {
                    Graphics::RZTexture *texture = nullptr;

                    uint32_t w = static_cast<uint32_t>(desc.extent.x);
                    uint32_t h = static_cast<uint32_t>(desc.extent.y);

                    switch (desc.type) {
                        case TextureType::Texture_2D:
                            texture = Graphics::RZTexture2D::Create(RZ_DEBUG_NAME_TAG_STR_F_ARG(desc.name) desc.name, w, h, nullptr, desc.format);
                        case TextureType::Texture_3D:
                            break;
                        case TextureType::Texture_CubeMap:
                            break;
                        case TextureType::Texture_Depth:
                            texture = Graphics::RZDepthTexture::Create(w, h);
                            break;
                        case TextureType::Texture_RenderTarget:
                            texture = Graphics::RZRenderTexture::Create(RZ_DEBUG_NAME_TAG_STR_F_ARG(desc.name) w, h, desc.format);
                            break;
                        case TextureType::Texture_SwapchainImage:
                            //m_Texture = RZRenderContext::getSwapchain()->GetCurrentImage()
                            break;
                    }

                    m_Textures.push_back(std::make_unique<RZTexture *>(std::move(texture)));
                    auto *ptr = m_Textures.back().get();
                    RAZIX_CORE_INFO("[Transient Resources] Created Texture resource : {0}", fmt::ptr(ptr));
                    return *ptr;
                } else {
                    auto *texture = pool.back().resource;
                    pool.pop_back();
                    return texture;
                }
            }

            void RZTransientResources::releaseTexture(const RZFrameGraphTexture::Desc &desc, RZTexture *texture)
            {
                const auto h = std::hash<RZFrameGraphTexture::Desc>{}(desc);
                m_TexturePools[h].push_back({texture, 0.0f});
            }

            Razix::Graphics::RZSemaphore *RZTransientResources::acquireSemaphore(const RZFrameGraphSemaphore::Desc &desc)
            {
                const auto h    = std::hash<RZFrameGraphSemaphore::Desc>{}(desc);
                auto      &pool = m_SemaphorePools[h];
                if (pool.empty()) {
                    auto buffer = RZSemaphore::Create(RZ_DEBUG_NAME_TAG_STR_S_ARG(desc.name));
                    m_Semaphores.push_back(std::make_unique<RZSemaphore *>(std::move(buffer)));
                    auto *ptr = m_Semaphores.back().get();
                    RAZIX_CORE_INFO("[Transient Resources] Created Semaphore : {0}", fmt::ptr(ptr));
                    return *ptr;
                } else {
                    auto *buffer = pool.back().resource;
                    pool.pop_back();
                    return buffer;
                }
            }

            void RZTransientResources::releaseSemaphore(const RZFrameGraphSemaphore::Desc &desc, RZSemaphore *semaphore)
            {
                const auto h = std::hash<RZFrameGraphSemaphore::Desc>{}(desc);
                m_SemaphorePools[h].push_back({std::move(semaphore), 0.0f});
            }
        }    // namespace FrameGraph
    }        // namespace Graphics
}    // namespace Razix