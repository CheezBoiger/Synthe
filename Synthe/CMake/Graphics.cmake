set ( SYNTHE_GRAPHICS_INC_DIR ${SYNTHE_INCLUDE_DIR}/Graphics )
set ( SYNTHE_D3D12_SRC_DIR ${SYNTHE_SOURCE_DIR}/D3D12 )


set (SYNTHE_INCLUDE_FILES 
    ${SYNTHE_GRAPHICS_INC_DIR}/Fence.hpp
    ${SYNTHE_GRAPHICS_INC_DIR}/GraphicsBuffer.hpp
    ${SYNTHE_GRAPHICS_INC_DIR}/GraphicsCommandList.hpp
    ${SYNTHE_GRAPHICS_INC_DIR}/GraphicsCommandQueue.hpp
    ${SYNTHE_GRAPHICS_INC_DIR}/GraphicsConfiguration.hpp
    ${SYNTHE_GRAPHICS_INC_DIR}/GraphicsDevice.hpp
    ${SYNTHE_GRAPHICS_INC_DIR}/GraphicsResource.hpp
    ${SYNTHE_GRAPHICS_INC_DIR}/GraphicsResourceView.hpp
    ${SYNTHE_GRAPHICS_INC_DIR}/GraphicsStructs.hpp
    ${SYNTHE_GRAPHICS_INC_DIR}/PipelineState.hpp
    ${SYNTHE_GRAPHICS_INC_DIR}/RenderPass.hpp
    ${SYNTHE_GRAPHICS_INC_DIR}/Swapchain.hpp
)


set (SYNTHE_D3D12_FILES
    ${SYNTHE_D3D12_SRC_DIR}/D3D12Buffers.hpp
    ${SYNTHE_D3D12_SRC_DIR}/D3D12CommandLists.hpp
    ${SYNTHE_D3D12_SRC_DIR}/D3D12ComputePipelineState.hpp
    ${SYNTHE_D3D12_SRC_DIR}/D3D12Fence.hpp
    ${SYNTHE_D3D12_SRC_DIR}/D3D12GPUManager.hpp
    ${SYNTHE_D3D12_SRC_DIR}/D3D12GraphicsDevice.hpp
    ${SYNTHE_D3D12_SRC_DIR}/D3D12GraphicsPipelineState.hpp
    ${SYNTHE_D3D12_SRC_DIR}/D3D12Resource.hpp
    ${SYNTHE_D3D12_SRC_DIR}/D3D12ResourceView.hpp
    ${SYNTHE_D3D12_SRC_DIR}/D3D12Swapchain.hpp
    ${SYNTHE_D3D12_SRC_DIR}/D3D12CommandList.cpp
    ${SYNTHE_D3D12_SRC_DIR}/D3D12ComputePipelineState.cpp
    ${SYNTHE_D3D12_SRC_DIR}/D3D12Fence.cpp
    ${SYNTHE_D3D12_SRC_DIR}/D3D12GPUManager.cpp
    ${SYNTHE_D3D12_SRC_DIR}/D3D12GraphicsCommandQueue.cpp
    ${SYNTHE_D3D12_SRC_DIR}/D3D12GraphicsPipelineState.cpp
    ${SYNTHE_D3D12_SRC_DIR}/D3D12GraphicsDevice.cpp
    ${SYNTHE_D3D12_SRC_DIR}/D3D12MemoryManager.cpp
    ${SYNTHE_D3D12_SRC_DIR}/D3D12PhysicalDeviceFeatures.cpp
    ${SYNTHE_D3D12_SRC_DIR}/D3D12Resource.cpp
    ${SYNTHE_D3D12_SRC_DIR}/D3D12ResourceView.cpp
    ${SYNTHE_D3D12_SRC_DIR}/D3D12Swapchain.cpp
)

set ( SYNTHE_GLOB
    ${SYNTHE_GLOB}
    ${SYNTHE_D3D12_FILES}
    ${SYNTHE_INCLUDE_FILES}
)


set ( SYNTHE_LIBS
    ${SYNTHE_LIBS}
    d3d12.lib
    dxgi.lib
)