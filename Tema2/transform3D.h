#pragma once

#include "utils/glm_utils.h"
#include <GL/glew.h>


namespace transform3D
{
    
    inline glm::mat4 Translate(float translateX, float translateY, float translateZ)
    {
        
        return glm::mat4(
            1.f, 0.f, 0.f, 0.f,
            0.f, 1.f, 0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            translateX, translateY, translateZ, 1.f
        );
    }

    
    inline glm::mat4 Scale(float scaleX, float scaleY, float scaleZ)
    {
       
        return glm::transpose(glm::mat4(
            scaleX, 0.f, 0.f, 0.f,
            0.f, scaleY, 0.f, 0.f,
            0.f, 0.f, scaleZ, 0.f,
            0.f, 0.f, 0.f, 1.f)
        );

    }

   
    inline glm::mat4 RotateOZ(float radians)
    {
        GLfloat c = cos(radians);
        GLfloat s = sin(radians);
        
        return glm::transpose(glm::mat4(
            c, s, 0.f, 0.f,
            -s, c, 0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            0.f, 0.f, 0.f, 1.f)
        );

    }

    
    inline glm::mat4 RotateOY(float radians)
    {
        GLfloat c = cos(radians);
        GLfloat s = sin(radians);
        
        return glm::transpose(glm::mat4(
            c, 0.f, -s, 0.f,
            0.f, 1.f, 0.f, 0.f,
            s, 0.f, c, 0.f,
            0.f, 0.f, 0.f, 1.f)
        );

    }

    
    inline glm::mat4 RotateOX(float radians)
    {
        GLfloat c = cos(radians);
        GLfloat s = sin(radians);
        
        return glm::transpose(glm::mat4(
            1.f, 0.f, 0.f, 0.f,
            0.f, c, s, 0.f,
            0.f, -s, c, 0.f,
            0.f, 0.f, 0.f, 1.f)
        );

    }
}   
