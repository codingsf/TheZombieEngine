#line 1 "clipsampler.fx"
//------------------------------------------------------------------------------
//  clipsampler.h
//  Declare the standard clip sampler for all shaders.
//  (C) 2004 Conjurer Services, S.A.
//------------------------------------------------------------------------------
sampler ClipSampler = sampler_state
{
    Texture = <clipMap>;
    AddressU  = Wrap;
    AddressV  = Wrap;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
    mipMapLodBias = <mipMapLodBias>;
};

