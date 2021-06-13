// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// Request GLSL 3.3
#version 330

// Tex coord and color input from vertex shader
in vec2 fragTexCoord;
in vec3 fragColor;

// This corresponds to the output color to the color buffer
out vec4 outColor;

// This is used for the texture sampling
uniform sampler2D uTexture;

void main()
{
	// Sample color from texture
	outColor = (0.5*vec4(fragColor, 1.0)) +
               (0.5*texture(uTexture, fragTexCoord));
}

