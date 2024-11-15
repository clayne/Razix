#pragma once

#ifdef RAZIX_RENDER_API_DIRECTX12

    #include "Razix/Graphics/RHI/API/RZTexture.h"

    #include <d3d12.h>

namespace Razix {
    namespace Graphics {

        /* DirectX 12 implementation of the RZTexture class */
        class DX12Texture final : public RZTexture
        {
        public:
            DX12Texture(const RZTextureDesc& desc RZ_DEBUG_NAME_TAG_E_ARG);
            DX12Texture(const RZTextureDesc& desc, const std::string& filePath RZ_DEBUG_NAME_TAG_E_ARG);
            DX12Texture(ID3D12Resource* backbuffer);
            ~DX12Texture() {}

            void DestroyResource() override;
            void Bind(u32 slot) override;
            void Unbind(u32 slot) override;
            int32_t ReadPixels(u32 x, u32 y) override;
            void* GetAPIHandlePtr() const override;

        private:
            ID3D12Resource* m_ResourceHandle;
        };
    }    // namespace Graphics
}    // namespace Razix

#endif