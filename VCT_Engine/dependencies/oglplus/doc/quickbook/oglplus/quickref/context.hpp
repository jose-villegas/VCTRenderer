/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_Context

class Context
 : public __context_Errors
 , public __context_Capabilities
 , public __context_ClipControlState
 , public __context_ViewportState
 , public __context_ViewportOps
 , public __context_BufferSelection
 , public __context_BufferMaskingState
 , public __context_BufferClearingState
 , public __context_BufferClearingOps
 , public __context_RasterizationState
 , public __context_RasterizationOps
 , public __context_DrawingState
 , public __context_DrawingOps
 , public __context_ComputingOps
 , public __context_DepthTest
 , public __context_StencilTest
 , public __context_ScissorTest
 , public __context_LogicOpState
 , public __context_PixelState
 , public __context_PixelOps
 , public __context_BlendingOps
 , public __context_BlendingState
 , public __context_Synchronization
 , public __context_Hints
 , public __context_LimitQueries
 , public __context_NumericQueries
 , public __context_StringQueries
 , public __context_ObjectBinding
 , public __context_ObjectDSA
{ };

//]

