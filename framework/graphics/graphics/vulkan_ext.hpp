#ifndef VG_VULKAN_EXT_HPP
#define VG_VULKAN_EXT_HPP

#include <cstdint>
#include <foundation/foundation.hpp>

namespace vg
{
    struct PhysicalDeviceFeaturePriorities
    {
      PhysicalDeviceFeaturePriorities( uint32_t robustBufferAccess_ = 0, uint32_t fullDrawIndexUint32_ = 0, uint32_t imageCubeArray_ = 0, uint32_t independentBlend_ = 0, uint32_t geometryShader_ = 0, uint32_t tessellationShader_ = 0, uint32_t sampleRateShading_ = 0, uint32_t dualSrcBlend_ = 0, uint32_t logicOp_ = 0, uint32_t multiDrawIndirect_ = 0, uint32_t drawIndirectFirstInstance_ = 0, uint32_t depthClamp_ = 0, uint32_t depthBiasClamp_ = 0, uint32_t fillModeNonSolid_ = 0, uint32_t depthBounds_ = 0, uint32_t wideLines_ = 0, uint32_t largePoints_ = 0, uint32_t alphaToOne_ = 0, uint32_t multiViewport_ = 0, uint32_t samplerAnisotropy_ = 0, uint32_t textureCompressionETC2_ = 0, uint32_t textureCompressionASTC_LDR_ = 0, uint32_t textureCompressionBC_ = 0, uint32_t occlusionQueryPrecise_ = 0, uint32_t pipelineStatisticsQuery_ = 0, uint32_t vertexPipelineStoresAndAtomics_ = 0, uint32_t fragmentStoresAndAtomics_ = 0, uint32_t shaderTessellationAndGeometryPointSize_ = 0, uint32_t shaderImageGatherExtended_ = 0, uint32_t shaderStorageImageExtendedFormats_ = 0, uint32_t shaderStorageImageMultisample_ = 0, uint32_t shaderStorageImageReadWithoutFormat_ = 0, uint32_t shaderStorageImageWriteWithoutFormat_ = 0, uint32_t shaderUniformBufferArrayDynamicIndexing_ = 0, uint32_t shaderSampledImageArrayDynamicIndexing_ = 0, uint32_t shaderStorageBufferArrayDynamicIndexing_ = 0, uint32_t shaderStorageImageArrayDynamicIndexing_ = 0, uint32_t shaderClipDistance_ = 0, uint32_t shaderCullDistance_ = 0, uint32_t shaderFloat64_ = 0, uint32_t shaderInt64_ = 0, uint32_t shaderInt16_ = 0, uint32_t shaderResourceResidency_ = 0, uint32_t shaderResourceMinLod_ = 0, uint32_t sparseBinding_ = 0, uint32_t sparseResidencyBuffer_ = 0, uint32_t sparseResidencyImage2D_ = 0, uint32_t sparseResidencyImage3D_ = 0, uint32_t sparseResidency2Samples_ = 0, uint32_t sparseResidency4Samples_ = 0, uint32_t sparseResidency8Samples_ = 0, uint32_t sparseResidency16Samples_ = 0, uint32_t sparseResidencyAliased_ = 0, uint32_t variableMultisampleRate_ = 0, uint32_t inheritedQueries_ = 0 )
        : robustBufferAccess( robustBufferAccess_ )
        , fullDrawIndexUint32( fullDrawIndexUint32_ )
        , imageCubeArray( imageCubeArray_ )
        , independentBlend( independentBlend_ )
        , geometryShader( geometryShader_ )
        , tessellationShader( tessellationShader_ )
        , sampleRateShading( sampleRateShading_ )
        , dualSrcBlend( dualSrcBlend_ )
        , logicOp( logicOp_ )
        , multiDrawIndirect( multiDrawIndirect_ )
        , drawIndirectFirstInstance( drawIndirectFirstInstance_ )
        , depthClamp( depthClamp_ )
        , depthBiasClamp( depthBiasClamp_ )
        , fillModeNonSolid( fillModeNonSolid_ )
        , depthBounds( depthBounds_ )
        , wideLines( wideLines_ )
        , largePoints( largePoints_ )
        , alphaToOne( alphaToOne_ )
        , multiViewport( multiViewport_ )
        , samplerAnisotropy( samplerAnisotropy_ )
        , textureCompressionETC2( textureCompressionETC2_ )
        , textureCompressionASTC_LDR( textureCompressionASTC_LDR_ )
        , textureCompressionBC( textureCompressionBC_ )
        , occlusionQueryPrecise( occlusionQueryPrecise_ )
        , pipelineStatisticsQuery( pipelineStatisticsQuery_ )
        , vertexPipelineStoresAndAtomics( vertexPipelineStoresAndAtomics_ )
        , fragmentStoresAndAtomics( fragmentStoresAndAtomics_ )
        , shaderTessellationAndGeometryPointSize( shaderTessellationAndGeometryPointSize_ )
        , shaderImageGatherExtended( shaderImageGatherExtended_ )
        , shaderStorageImageExtendedFormats( shaderStorageImageExtendedFormats_ )
        , shaderStorageImageMultisample( shaderStorageImageMultisample_ )
        , shaderStorageImageReadWithoutFormat( shaderStorageImageReadWithoutFormat_ )
        , shaderStorageImageWriteWithoutFormat( shaderStorageImageWriteWithoutFormat_ )
        , shaderUniformBufferArrayDynamicIndexing( shaderUniformBufferArrayDynamicIndexing_ )
        , shaderSampledImageArrayDynamicIndexing( shaderSampledImageArrayDynamicIndexing_ )
        , shaderStorageBufferArrayDynamicIndexing( shaderStorageBufferArrayDynamicIndexing_ )
        , shaderStorageImageArrayDynamicIndexing( shaderStorageImageArrayDynamicIndexing_ )
        , shaderClipDistance( shaderClipDistance_ )
        , shaderCullDistance( shaderCullDistance_ )
        , shaderFloat64( shaderFloat64_ )
        , shaderInt64( shaderInt64_ )
        , shaderInt16( shaderInt16_ )
        , shaderResourceResidency( shaderResourceResidency_ )
        , shaderResourceMinLod( shaderResourceMinLod_ )
        , sparseBinding( sparseBinding_ )
        , sparseResidencyBuffer( sparseResidencyBuffer_ )
        , sparseResidencyImage2D( sparseResidencyImage2D_ )
        , sparseResidencyImage3D( sparseResidencyImage3D_ )
        , sparseResidency2Samples( sparseResidency2Samples_ )
        , sparseResidency4Samples( sparseResidency4Samples_ )
        , sparseResidency8Samples( sparseResidency8Samples_ )
        , sparseResidency16Samples( sparseResidency16Samples_ )
        , sparseResidencyAliased( sparseResidencyAliased_ )
        , variableMultisampleRate( variableMultisampleRate_ )
        , inheritedQueries( inheritedQueries_ )
      {
      }
  
      PhysicalDeviceFeaturePriorities( PhysicalDeviceFeaturePriorities const & rhs )
      {
        memcpy( this, &rhs, sizeof( PhysicalDeviceFeaturePriorities ) );
      }
  
      PhysicalDeviceFeaturePriorities& setRobustBufferAccess( uint32_t robustBufferAccess_ )
      {
        robustBufferAccess = robustBufferAccess_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setFullDrawIndexUint32( uint32_t fullDrawIndexUint32_ )
      {
        fullDrawIndexUint32 = fullDrawIndexUint32_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setImageCubeArray( uint32_t imageCubeArray_ )
      {
        imageCubeArray = imageCubeArray_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setIndependentBlend( uint32_t independentBlend_ )
      {
        independentBlend = independentBlend_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setGeometryShader( uint32_t geometryShader_ )
      {
        geometryShader = geometryShader_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setTessellationShader( uint32_t tessellationShader_ )
      {
        tessellationShader = tessellationShader_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setSampleRateShading( uint32_t sampleRateShading_ )
      {
        sampleRateShading = sampleRateShading_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setDualSrcBlend( uint32_t dualSrcBlend_ )
      {
        dualSrcBlend = dualSrcBlend_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setLogicOp( uint32_t logicOp_ )
      {
        logicOp = logicOp_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setMultiDrawIndirect( uint32_t multiDrawIndirect_ )
      {
        multiDrawIndirect = multiDrawIndirect_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setDrawIndirectFirstInstance( uint32_t drawIndirectFirstInstance_ )
      {
        drawIndirectFirstInstance = drawIndirectFirstInstance_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setDepthClamp( uint32_t depthClamp_ )
      {
        depthClamp = depthClamp_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setDepthBiasClamp( uint32_t depthBiasClamp_ )
      {
        depthBiasClamp = depthBiasClamp_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setFillModeNonSolid( uint32_t fillModeNonSolid_ )
      {
        fillModeNonSolid = fillModeNonSolid_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setDepthBounds( uint32_t depthBounds_ )
      {
        depthBounds = depthBounds_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setWideLines( uint32_t wideLines_ )
      {
        wideLines = wideLines_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setLargePoints( uint32_t largePoints_ )
      {
        largePoints = largePoints_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setAlphaToOne( uint32_t alphaToOne_ )
      {
        alphaToOne = alphaToOne_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setMultiViewport( uint32_t multiViewport_ )
      {
        multiViewport = multiViewport_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setSamplerAnisotropy( uint32_t samplerAnisotropy_ )
      {
        samplerAnisotropy = samplerAnisotropy_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setTextureCompressionETC2( uint32_t textureCompressionETC2_ )
      {
        textureCompressionETC2 = textureCompressionETC2_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setTextureCompressionASTC_LDR( uint32_t textureCompressionASTC_LDR_ )
      {
        textureCompressionASTC_LDR = textureCompressionASTC_LDR_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setTextureCompressionBC( uint32_t textureCompressionBC_ )
      {
        textureCompressionBC = textureCompressionBC_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setOcclusionQueryPrecise( uint32_t occlusionQueryPrecise_ )
      {
        occlusionQueryPrecise = occlusionQueryPrecise_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setPipelineStatisticsQuery( uint32_t pipelineStatisticsQuery_ )
      {
        pipelineStatisticsQuery = pipelineStatisticsQuery_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setVertexPipelineStoresAndAtomics( uint32_t vertexPipelineStoresAndAtomics_ )
      {
        vertexPipelineStoresAndAtomics = vertexPipelineStoresAndAtomics_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setFragmentStoresAndAtomics( uint32_t fragmentStoresAndAtomics_ )
      {
        fragmentStoresAndAtomics = fragmentStoresAndAtomics_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setShaderTessellationAndGeometryPointSize( uint32_t shaderTessellationAndGeometryPointSize_ )
      {
        shaderTessellationAndGeometryPointSize = shaderTessellationAndGeometryPointSize_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setShaderImageGatherExtended( uint32_t shaderImageGatherExtended_ )
      {
        shaderImageGatherExtended = shaderImageGatherExtended_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setShaderStorageImageExtendedFormats( uint32_t shaderStorageImageExtendedFormats_ )
      {
        shaderStorageImageExtendedFormats = shaderStorageImageExtendedFormats_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setShaderStorageImageMultisample( uint32_t shaderStorageImageMultisample_ )
      {
        shaderStorageImageMultisample = shaderStorageImageMultisample_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setShaderStorageImageReadWithoutFormat( uint32_t shaderStorageImageReadWithoutFormat_ )
      {
        shaderStorageImageReadWithoutFormat = shaderStorageImageReadWithoutFormat_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setShaderStorageImageWriteWithoutFormat( uint32_t shaderStorageImageWriteWithoutFormat_ )
      {
        shaderStorageImageWriteWithoutFormat = shaderStorageImageWriteWithoutFormat_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setShaderUniformBufferArrayDynamicIndexing( uint32_t shaderUniformBufferArrayDynamicIndexing_ )
      {
        shaderUniformBufferArrayDynamicIndexing = shaderUniformBufferArrayDynamicIndexing_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setShaderSampledImageArrayDynamicIndexing( uint32_t shaderSampledImageArrayDynamicIndexing_ )
      {
        shaderSampledImageArrayDynamicIndexing = shaderSampledImageArrayDynamicIndexing_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setShaderStorageBufferArrayDynamicIndexing( uint32_t shaderStorageBufferArrayDynamicIndexing_ )
      {
        shaderStorageBufferArrayDynamicIndexing = shaderStorageBufferArrayDynamicIndexing_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setShaderStorageImageArrayDynamicIndexing( uint32_t shaderStorageImageArrayDynamicIndexing_ )
      {
        shaderStorageImageArrayDynamicIndexing = shaderStorageImageArrayDynamicIndexing_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setShaderClipDistance( uint32_t shaderClipDistance_ )
      {
        shaderClipDistance = shaderClipDistance_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setShaderCullDistance( uint32_t shaderCullDistance_ )
      {
        shaderCullDistance = shaderCullDistance_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setShaderFloat64( uint32_t shaderFloat64_ )
      {
        shaderFloat64 = shaderFloat64_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setShaderInt64( uint32_t shaderInt64_ )
      {
        shaderInt64 = shaderInt64_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setShaderInt16( uint32_t shaderInt16_ )
      {
        shaderInt16 = shaderInt16_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setShaderResourceResidency( uint32_t shaderResourceResidency_ )
      {
        shaderResourceResidency = shaderResourceResidency_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setShaderResourceMinLod( uint32_t shaderResourceMinLod_ )
      {
        shaderResourceMinLod = shaderResourceMinLod_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setSparseBinding( uint32_t sparseBinding_ )
      {
        sparseBinding = sparseBinding_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setSparseResidencyBuffer( uint32_t sparseResidencyBuffer_ )
      {
        sparseResidencyBuffer = sparseResidencyBuffer_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setSparseResidencyImage2D( uint32_t sparseResidencyImage2D_ )
      {
        sparseResidencyImage2D = sparseResidencyImage2D_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setSparseResidencyImage3D( uint32_t sparseResidencyImage3D_ )
      {
        sparseResidencyImage3D = sparseResidencyImage3D_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setSparseResidency2Samples( uint32_t sparseResidency2Samples_ )
      {
        sparseResidency2Samples = sparseResidency2Samples_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setSparseResidency4Samples( uint32_t sparseResidency4Samples_ )
      {
        sparseResidency4Samples = sparseResidency4Samples_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setSparseResidency8Samples( uint32_t sparseResidency8Samples_ )
      {
        sparseResidency8Samples = sparseResidency8Samples_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setSparseResidency16Samples( uint32_t sparseResidency16Samples_ )
      {
        sparseResidency16Samples = sparseResidency16Samples_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setSparseResidencyAliased( uint32_t sparseResidencyAliased_ )
      {
        sparseResidencyAliased = sparseResidencyAliased_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setVariableMultisampleRate( uint32_t variableMultisampleRate_ )
      {
        variableMultisampleRate = variableMultisampleRate_;
        return *this;
      }
  
      PhysicalDeviceFeaturePriorities& setInheritedQueries( uint32_t inheritedQueries_ )
      {
        inheritedQueries = inheritedQueries_;
        return *this;
      }
  
      bool operator==( PhysicalDeviceFeaturePriorities const& rhs ) const
      {
        return ( robustBufferAccess == rhs.robustBufferAccess )
            && ( fullDrawIndexUint32 == rhs.fullDrawIndexUint32 )
            && ( imageCubeArray == rhs.imageCubeArray )
            && ( independentBlend == rhs.independentBlend )
            && ( geometryShader == rhs.geometryShader )
            && ( tessellationShader == rhs.tessellationShader )
            && ( sampleRateShading == rhs.sampleRateShading )
            && ( dualSrcBlend == rhs.dualSrcBlend )
            && ( logicOp == rhs.logicOp )
            && ( multiDrawIndirect == rhs.multiDrawIndirect )
            && ( drawIndirectFirstInstance == rhs.drawIndirectFirstInstance )
            && ( depthClamp == rhs.depthClamp )
            && ( depthBiasClamp == rhs.depthBiasClamp )
            && ( fillModeNonSolid == rhs.fillModeNonSolid )
            && ( depthBounds == rhs.depthBounds )
            && ( wideLines == rhs.wideLines )
            && ( largePoints == rhs.largePoints )
            && ( alphaToOne == rhs.alphaToOne )
            && ( multiViewport == rhs.multiViewport )
            && ( samplerAnisotropy == rhs.samplerAnisotropy )
            && ( textureCompressionETC2 == rhs.textureCompressionETC2 )
            && ( textureCompressionASTC_LDR == rhs.textureCompressionASTC_LDR )
            && ( textureCompressionBC == rhs.textureCompressionBC )
            && ( occlusionQueryPrecise == rhs.occlusionQueryPrecise )
            && ( pipelineStatisticsQuery == rhs.pipelineStatisticsQuery )
            && ( vertexPipelineStoresAndAtomics == rhs.vertexPipelineStoresAndAtomics )
            && ( fragmentStoresAndAtomics == rhs.fragmentStoresAndAtomics )
            && ( shaderTessellationAndGeometryPointSize == rhs.shaderTessellationAndGeometryPointSize )
            && ( shaderImageGatherExtended == rhs.shaderImageGatherExtended )
            && ( shaderStorageImageExtendedFormats == rhs.shaderStorageImageExtendedFormats )
            && ( shaderStorageImageMultisample == rhs.shaderStorageImageMultisample )
            && ( shaderStorageImageReadWithoutFormat == rhs.shaderStorageImageReadWithoutFormat )
            && ( shaderStorageImageWriteWithoutFormat == rhs.shaderStorageImageWriteWithoutFormat )
            && ( shaderUniformBufferArrayDynamicIndexing == rhs.shaderUniformBufferArrayDynamicIndexing )
            && ( shaderSampledImageArrayDynamicIndexing == rhs.shaderSampledImageArrayDynamicIndexing )
            && ( shaderStorageBufferArrayDynamicIndexing == rhs.shaderStorageBufferArrayDynamicIndexing )
            && ( shaderStorageImageArrayDynamicIndexing == rhs.shaderStorageImageArrayDynamicIndexing )
            && ( shaderClipDistance == rhs.shaderClipDistance )
            && ( shaderCullDistance == rhs.shaderCullDistance )
            && ( shaderFloat64 == rhs.shaderFloat64 )
            && ( shaderInt64 == rhs.shaderInt64 )
            && ( shaderInt16 == rhs.shaderInt16 )
            && ( shaderResourceResidency == rhs.shaderResourceResidency )
            && ( shaderResourceMinLod == rhs.shaderResourceMinLod )
            && ( sparseBinding == rhs.sparseBinding )
            && ( sparseResidencyBuffer == rhs.sparseResidencyBuffer )
            && ( sparseResidencyImage2D == rhs.sparseResidencyImage2D )
            && ( sparseResidencyImage3D == rhs.sparseResidencyImage3D )
            && ( sparseResidency2Samples == rhs.sparseResidency2Samples )
            && ( sparseResidency4Samples == rhs.sparseResidency4Samples )
            && ( sparseResidency8Samples == rhs.sparseResidency8Samples )
            && ( sparseResidency16Samples == rhs.sparseResidency16Samples )
            && ( sparseResidencyAliased == rhs.sparseResidencyAliased )
            && ( variableMultisampleRate == rhs.variableMultisampleRate )
            && ( inheritedQueries == rhs.inheritedQueries );
      }
  
      bool operator!=( PhysicalDeviceFeaturePriorities const& rhs ) const
      {
        return !operator==( rhs );
      }
  
      uint32_t robustBufferAccess;
      uint32_t fullDrawIndexUint32;
      uint32_t imageCubeArray;
      uint32_t independentBlend;
      uint32_t geometryShader;
      uint32_t tessellationShader;
      uint32_t sampleRateShading;
      uint32_t dualSrcBlend;
      uint32_t logicOp;
      uint32_t multiDrawIndirect;
      uint32_t drawIndirectFirstInstance;
      uint32_t depthClamp;
      uint32_t depthBiasClamp;
      uint32_t fillModeNonSolid;
      uint32_t depthBounds;
      uint32_t wideLines;
      uint32_t largePoints;
      uint32_t alphaToOne;
      uint32_t multiViewport;
      uint32_t samplerAnisotropy;
      uint32_t textureCompressionETC2;
      uint32_t textureCompressionASTC_LDR;
      uint32_t textureCompressionBC;
      uint32_t occlusionQueryPrecise;
      uint32_t pipelineStatisticsQuery;
      uint32_t vertexPipelineStoresAndAtomics;
      uint32_t fragmentStoresAndAtomics;
      uint32_t shaderTessellationAndGeometryPointSize;
      uint32_t shaderImageGatherExtended;
      uint32_t shaderStorageImageExtendedFormats;
      uint32_t shaderStorageImageMultisample;
      uint32_t shaderStorageImageReadWithoutFormat;
      uint32_t shaderStorageImageWriteWithoutFormat;
      uint32_t shaderUniformBufferArrayDynamicIndexing;
      uint32_t shaderSampledImageArrayDynamicIndexing;
      uint32_t shaderStorageBufferArrayDynamicIndexing;
      uint32_t shaderStorageImageArrayDynamicIndexing;
      uint32_t shaderClipDistance;
      uint32_t shaderCullDistance;
      uint32_t shaderFloat64;
      uint32_t shaderInt64;
      uint32_t shaderInt16;
      uint32_t shaderResourceResidency;
      uint32_t shaderResourceMinLod;
      uint32_t sparseBinding;
      uint32_t sparseResidencyBuffer;
      uint32_t sparseResidencyImage2D;
      uint32_t sparseResidencyImage3D;
      uint32_t sparseResidency2Samples;
      uint32_t sparseResidency4Samples;
      uint32_t sparseResidency8Samples;
      uint32_t sparseResidency16Samples;
      uint32_t sparseResidencyAliased;
      uint32_t variableMultisampleRate;
      uint32_t inheritedQueries;
    };
    static_assert( sizeof( PhysicalDeviceFeaturePriorities ) == sizeof( VkPhysicalDeviceFeatures ), 
        "struct PhysicalDeviceFeaturePriorities and struct VkPhysicalDeviceFeatures have different size!" );
} //vg

#endif