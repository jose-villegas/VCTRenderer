/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_ClientContext

class ClientContext
 : public __client_CapabilityState
 , public __client_ViewportState
 , public __client_ClipControlState
 , public __client_BufferMaskingState
 , public __client_BufferClearingOps
 , public __client_BufferClearingState
 , public __client_RasterizationState
 , public __client_RasterizationOps
 , public __client_DrawingState
 , public __client_DrawingOps
 , public __client_ComputingOps
 , public __client_DepthTestState
 , public __client_StencilTestState
 , public __client_ScissorTestState
 , public __client_LogicOpState
 , public __client_PixelState
 , public __client_PixelOps
 , public __client_BlendingOps
 , public __client_BlendingState
 , public __client_Synchronization
 , public __client_HintState
 , public __client_LimitQueries
 , public __client_NumericQueries
 , public __client_StringQueries
 , public __client_CurrentObjects
{ };

//]

