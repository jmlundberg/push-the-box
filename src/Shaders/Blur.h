#ifndef PushTheBox_Shaders_Blur_h
#define PushTheBox_Shaders_Blur_h

#include <AbstractShaderProgram.h>

#include "PushTheBox.h"

namespace PushTheBox { namespace Shaders {

class Blur: public AbstractShaderProgram {
    public:
        enum class Direction: UnsignedByte {
            Horizontal,
            Vertical
        };

        enum: Int {
            TextureLayer = 16,
        };

        explicit Blur(Direction direction);
};

}}

#endif
