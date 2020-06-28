// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia
#pragma once

#include "Common/Types.hpp"


namespace Synthe {


class DescriptorSet;
class Resource;


enum CommandListLevel
{
    CommandListLevel_PRIMARY,
    CommandListLevel_SECONDARY
};


enum CommandListType
{
    CommandListType_GRAPHICS,
    CommandListType_COMPUTE,
    CommandListType_COPY
};


enum CommandListFlag
{
    //! Command List default flag.
    CommandListFlag_NONE = 0,
    //! Command List to be used only once.
    CommandListFlag_ONE_TIME_SUBMIT = (1 << 0),
    //! Commandlist to be recorded very few times.
    CommandListFlag_RARELY_UPDATE = (1 << 1)
};


typedef U32 CommandListFlags;


//! Command list structure for graphics processing unit.
class GraphicsCommandList {
public:

    virtual ~GraphicsCommandList() { }

    //! Begin the command list recording. This must be called before any draw, copy, compute, or state
    //! command is called.
    //!
    virtual void Begin() { }

    //! End the command list recording. This must be called after every Begin() function.
    //! 
    virtual void End() { }

    //! Draws call that records an indexed draw (ordered vertex buffer.)
    //!
    virtual void DrawIndexedInstanced() { }

    //! Draw call that records a draw command.
    virtual void DrawInstanced() { }

    //! Compute command that records a dispatch.
    virtual void Dispatch() { }

    //! Set the graphics pipeline state, along with the layout info.
    virtual void SetGraphicsPipelineState() { }

    //! Set the compute pipeline state, along with the layout info.
    virtual void SetComputePipelineState() { }

    //! Set viewports for the corresponding pass.
    virtual void SetViewports() { }

    //! Set the scissors for the corresponding pass.
    virtual void SetScissors() { }

    //! Set the render target views that will be used.
    virtual void SetRenderTargets(U32 NumRTVs) { }

    //! Set the descriptor sets that correspond to the data resources that will be used by the render pass
    //! state.
    //!
    //! \param NumSets Number of descriptor sets to be bound.
    //! \param PDescriptorSets An array of descriptor sets that will be used by bound shaders.
    virtual void BindDescriptorSets(U32 NumSets, DescriptorSet* const* PDescriptorSets) { }

    //! Bind vertex buffers for drawing.
    virtual void BindVertexBuffers(U32 NumBuffers, Resource* const* PBuffers, U32* Offsets) { }

    //! Bind index buffers for drawing.
    virtual void BindIndexBuffer(const Resource* PBuffer, U32 Offset) { }

    //! Copy a Resource to another Resource.
    virtual void CopyResource(Resource* PDest, Resource* PSrc) { }
};
} // Synth