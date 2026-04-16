// Copyright (C) 2011-2014 by Maximilian Hönig
// This file is part of "DunGen - the Dungeongenerator".
// For conditions of distribution and use, see licence.txt provided together with DunGen.

#ifndef SHADER_H
#define SHADER_H

namespace DunGen
{
	// note: light position is camera position in these shaders
	static const char * VertexShaderCaveSingleColor =
	"uniform vec3 mLightPos;"										// position of the camera
	"varying vec3 normal,lightDir;"									// normal and direction to the camera

	"void main(void)"
	"{"	
		"gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;"	// transform position
		"normal = gl_Normal;"										// pass normal
		"lightDir = mLightPos - gl_Vertex.xyz;"						// compute direction to light
	"}";

	static const char * FragmentShaderCaveSingleColor =
	"uniform vec3 mHalfColor;"										// half of the color value
	"varying vec3 normal,lightDir;"									// normal and direction to the camera

	"void main (void)"
	"{"
		"float NdotL = dot(normalize(normal),normalize(lightDir));"	// compute N * L
		"gl_FragColor.xyz = (1.0-NdotL)*mHalfColor;"				// compute final color: interpolation over 180 degrees instead of usual 90 degrees
		"gl_FragColor.a = 1.0;"
	"}";

	static const char * VertexShaderCaveMultiColor =
	"uniform vec3 mLightPos;"
	"varying vec3 normal,lightDir;"									// normal and direction to the camera
	"varying vec3 position;"										// position of the 3D sampling point

	"void main(void)"
	"{"	
		"gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;"	// transform position
		"normal = gl_Normal;"										// pass normal
		"position = gl_Vertex.xyz;"									// pass position
		"lightDir = mLightPos - gl_Vertex.xyz;"						// compute direction to light
	"}";

	static const char * FragmentShaderCaveMultiColor =
	"varying vec3 normal,lightDir;"									// normal and direction to the camera
	"varying vec3 position;"										// position of the 3D sampling point

	"void main (void)"
	"{"
		"float NdotL = dot(normalize(normal),normalize(lightDir));"	// compute N * L
		"gl_FragColor.rgb = (1.0-NdotL)*0.002*position;"			// compute final color: interpolation over 180 degrees instead of usual 90 degrees, using position as RGB base
		"gl_FragColor.a = 1.0;"
	"}";

	static const char * ShaderCaveSingleColor_HLSL =
	"float4x4 mWorldViewProj;"										// World * View * Projection transformation
	"float3 mLightPos;"												// Light position
	"float3 mHalfColor;"											// half of the color value

	"struct VertexShaderInput"
	"{"
		"float4 Position : POSITION0;"
		"float3 Normal : NORMAL0;"
	"};"

	"struct VertexShaderOutput"
	"{"
		"float4 PositionProj : POSITION0;"
		"float3 Normal : TEXCOORD0;"
		"float3 LightDirection: TEXCOORD1;"
	"};"

	"VertexShaderOutput vertexMain(VertexShaderInput input)"
	"{"
		"VertexShaderOutput output;	"
		"output.PositionProj = mul(input.Position, mWorldViewProj);"// transform position to clip space
		"output.Normal = input.Normal;"								// pass normal
		"output.LightDirection = mLightPos - input.Position.xyz;"	// compute light direction
		"return output;"
	"}"

	"float4 pixelMain(VertexShaderOutput input) : COLOR0"
	"{"
		"float NdotL = dot(normalize(input.Normal),normalize(input.LightDirection));"	// compute N * L
		"float3 color = ((1.0-NdotL)*mHalfColor);"										// compute the final color
		"return float4(color,1.0);"
	"}";


	static const char * ShaderCaveMultiColor_HLSL =
	"float4x4 mWorldViewProj;"										// World * View * Projection transformation
	"float3 mLightPos;"												// Light position

	"struct VertexShaderInput"
	"{"
		"float4 Position : POSITION0;"
		"float3 Normal : NORMAL0;"
	"};"

	"struct VertexShaderOutput"
	"{"
		"float4 PositionProj : POSITION0;"
		"float3 Normal : TEXCOORD0;"
		"float3 LightDirection: TEXCOORD1;"
		"float3 Position: TEXCOORD2;"
	"};"

	"VertexShaderOutput vertexMain(VertexShaderInput input)"
	"{"
		"VertexShaderOutput output;	"
		"output.PositionProj = mul(input.Position, mWorldViewProj);"// transform position to clip space
		"output.Position = input.Position.xyz;"						// pass position
		"output.Normal = input.Normal;"								// pass normal
		"output.LightDirection = mLightPos - input.Position.xyz;"	// compute light direction
		"return output;"
	"}"

	"float4 pixelMain(VertexShaderOutput input) : COLOR0"
	"{"
		"float NdotL = dot(normalize(input.Normal),normalize(input.LightDirection));"	// compute N * L
		"float3 color = ((1.0-NdotL)*0.002*input.Position);"							// compute the final color
		"return float4(color,1.0);"
	"}";

} // END NAMESPACE DunGen

#endif