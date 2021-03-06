#line 1 "default_alph_ocean.fx"
//------------------------------------------------------------------------------
//  ps2.0/default_alph_ocean.fx
//
//  Ocean shader, ported from the nVidia FXComposer sample of the same name.
//
//  (C) 2004 RadonLabs GmbH
//------------------------------------------------------------------------------
#include "shaders:../lib/lib.fx"
#include "shaders:../lib/fog.fx"
#include "shaders:../lib/libzombie.fx"

shared float4x4 Model;
shared float4x4 ModelViewProjection;
shared float4x4 ModelView;
shared float4x4 InvView;
shared float    Time;

shared float4 LightDiffuse;         // light diffuse color

//if (bump)
float BumpScale;
float4 TexGenS;                 // texture scale

float Intensity0;               // hdr multiplier
//float4 MatDiffuse;              // deep water color
//float4 MatSpecular;             // shallow water color
//float4 MatAmbient;              // reflection color
//float Intensity1;               // reflection amount
//float Intensity2;               // water color amount

float Amplitude;                // wave amplitude
float Frequency;                // wave frequency
float4 Velocity;                // wave speed

float FresnelBias;
float FresnelPower;

int CullMode = 2; // CW
float4 fogDistances;
float4 fogColor = { 1.0f, 1.0f, 1.0f, 1.0f };
 
texture BumpMap0;
texture CubeMap0;

//------------------------------------------------------------------------------
struct VsInput
{
    float4 position : POSITION;
    //if (uv0):
    float2 uv0      : TEXCOORD0;
    float3 color    : COLOR0;
};

struct VsOutput
{
    float4 position     : POSITION;
    //if (uv0)
    float2 uv0          : TEXCOORD0;

    //if (bump)
    float3 row0         : TEXCOORD1; // first row of the 3x3 transform from tangent to cube space
    float3 row1         : TEXCOORD2; // second row of the 3x3 transform from tangent to cube space
    float3 row2         : TEXCOORD3; // third row of the 3x3 transform from tangent to cube space

    float2 bumpCoord0   : TEXCOORD4;
    float2 bumpCoord1   : TEXCOORD5;
    float2 bumpCoord2   : TEXCOORD6;
    
    //if (envmap)
    float3 eyeVector    : TEXCOORD7;

    float  fog          : FOG;
};

//------------------------------------------------------------------------------
sampler NormalMapSampler = sampler_state
{
    Texture = <BumpMap0>;
	MagFilter = Linear;
	MinFilter = Linear;
	MipFilter = Linear;
    AddressU = Wrap;
    AddressV = Wrap;
    MipMapLodBias = -0.75;
};

sampler EnvMapSampler = sampler_state
{
    Texture = <CubeMap0>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
    AddressU = Clamp;
    AddressV = Clamp;
};

//------------------------------------------------------------------------------
//  vertex shader
//------------------------------------------------------------------------------
VsOutput vsMain(const VsInput vsIn, uniform bool fog)
{
    VsOutput vsOut;

    float4 position = vsIn.position;
/*
    //if (wave)
    wave[0].freq = Frequency;
    wave[0].amp  = Amplitude;
    wave[1].freq = Frequency * 2.0;
    wave[1].amp  = Amplitude * 0.5;

    // sum wave
    //if (envmap)
    float ddx = 0.0;
    float ddy = 0.0;
    //endif (envmap)

    for (int i = 0; i < NumWaves; i++)
    {
	    position.y += evaluateWave(wave[i], position.xz, Time) * vsIn.color.y;
	    //if (envmap)
	    float deriv = evaluateWaveDeriv(wave[i], position.xz, Time);
	    ddx += deriv * wave[i].dir.x;
	    ddy += deriv * wave[i].dir.y;
	    //endif (envmap)
	}

    //if (envmap)
    // compute tangent basis
    float3 B = float3(1, ddx, 0);
    float3 T = float3(0, ddy, 1);
    float3 N = float3(-ddx, 1, -ddy);
*/
    float3 B;
    float3 T;
    float3 N;

    // transform wave
    position = transformWave(position, vsIn.color, Frequency, Amplitude, Time, N, T, B);
    
    // compute the 3x3 tranform from tangent space to object space
    // first rows are the tangent and binormal scaled by the bump scale
    float3x3 objToTangentSpace;
    objToTangentSpace[0] = BumpScale * normalize(T);
    objToTangentSpace[1] = BumpScale * normalize(B);
    objToTangentSpace[2] = normalize(N);

    vsOut.row0.xyz = mul(objToTangentSpace, Model[0].xyz);
    vsOut.row1.xyz = mul(objToTangentSpace, Model[1].xyz);
    vsOut.row2.xyz = mul(objToTangentSpace, Model[2].xyz);
    //endif (envmap)

	// pass texture coordinates for fetching the normal map
	//if (uv0):
	vsOut.uv0.xy = vsIn.uv0 * TexGenS.xy;
	
    //if (bump)
	float modTime = fmod(Time, 100.0f);
	vsOut.bumpCoord0.xy = vsIn.uv0 * TexGenS.xy + modTime * Velocity.xy;
	vsOut.bumpCoord1.xy = vsIn.uv0 * TexGenS.xy + modTime * Velocity.xy * 4.0f;
	vsOut.bumpCoord2.xy = vsIn.uv0 * TexGenS.xy + modTime * Velocity.xy * 8.0f;
    //endif (bump)

	// compute output vertex position
	vsOut.position = mul(position, ModelViewProjection);

    //if (envmap)
    // compute the eye vector (going from shaded point to eye) in cube space
    float4 worldPos = mul(position, Model);
    vsOut.eyeVector = InvView[3] - worldPos; // view inv. transpose contains eye position in world space in last row
    //endif (envmap)

    if (fog)
    {
        vsOut.fog = linearFog(position, fogDistances, ModelView);
    }
    else
    {
        vsOut.fog = 0.0f;
    }
    return vsOut;
}

//------------------------------------------------------------------------------
//  pixel shader
//------------------------------------------------------------------------------
float4 psMain(VsOutput psIn) : COLOR
{
    float4 dstColor = float4(0.0f, 0.0f, 0.0f, 1.0f);

    //if (bump and envmap)
    // sum normal maps
    half4 t0 = tex2D(NormalMapSampler, psIn.bumpCoord0.xy) * 2.0 - 1.0;
    half4 t1 = tex2D(NormalMapSampler, psIn.bumpCoord1.xy) * 2.0 - 1.0;
    half4 t2 = tex2D(NormalMapSampler, psIn.bumpCoord2.xy) * 2.0 - 1.0;
    half3 N = t0.xyz + t1.xyz + t2.xyz;
    
    half3x3 m; // tangent to world matrix
    m[0] = psIn.row0;
    m[1] = psIn.row1;
    m[2] = psIn.row2;
    half3 Nw = mul(m, N.xyz);
    Nw = normalize(Nw);

    // reflection
    float3 E = normalize(psIn.eyeVector);
    half3 R = reflect(-E, Nw);
    half4 reflection = texCUBE(EnvMapSampler, R);

    // hdr effect (multiplier in alpha channel)
    reflection.rgb *= (1.0 + reflection.a * Intensity0);

	// fresnel - could use 1D tex lookup for this
    half facing = 1.0 - max(dot(E, Nw), 0);
    half fresnel = FresnelBias + (1.0 - FresnelBias) * pow(facing, FresnelPower);

//    half4 waterColor = lerp(MatDiffuse, MatSpecular, facing);
//    float4 color = waterColor * Intensity2 + reflection * MatAmbient * Intensity1 * LightDiffuse * 0.5 * fresnel;
    dstColor = reflection;
    
    //if (fresnel)
    dstColor *= fresnel;
    dstColor.a = fresnel * 0.5 + 0.5;

    return dstColor;
}

//------------------------------------------------------------------------------
//  The technique.
//------------------------------------------------------------------------------
technique tDefault
{
    pass p0
    {
        AlphaTestEnable     = False;
        DepthBias           = 0.0f;
        CullMode            = CW;
        SrcBlend            = SrcAlpha;
        DestBlend           = InvSrcAlpha;
        FogEnable           = False;

        VertexShader        = compile vs_2_0 vsMain(false);
        PixelShader         = compile ps_2_0 psMain();
    }
}

technique tLinearFog
{
    pass p0
    {
        AlphaTestEnable     = False;
        DepthBias           = 0.0f;
        CullMode            = CW;
        SrcBlend            = SrcAlpha;
        DestBlend           = InvSrcAlpha;
        FogEnable           = True;
        FogColor            = <fogColor>;
        FogVertexMode       = Linear;
        FogTableMode        = None;

        VertexShader        = compile vs_2_0 vsMain(true);
        PixelShader         = compile ps_2_0 psMain();
    }
}
