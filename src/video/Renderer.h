#ifndef JVGS_VIDEO_RENDERER_H
#define JVGS_VIDEO_RENDERER_H

#include "../math/Vector2D.h"
#include <vector>

namespace jvgs
{
    namespace video
    {
        /** Types of primives that can be rendered.
         */
        enum RenderType
        {
            RENDERTYPE_LINES = 1,
            RENDERTYPE_LINE_STRIP = 2,
            RENDERTYPE_LINE_LOOP = 3,
            RENDERTYPE_QUADS = 4,
            RENDERTYPE_POLYGON = 5,
            RENDERTYPE_CONCAVE_POLYGON = 6
        };

        /** A class to draw primitives fast.
         */
        class Renderer
        {
            protected:
                /** If we are between a begin() and an end() call.
                 */
                bool busy;

                /** Type currently rendering.
                 */
                RenderType rendering;

            public:
                /** Constructor.
                 */
                Renderer();

                /** Destructor.
                 */
                virtual ~Renderer();

                /** Start rendering a certain type of primitive.
                 *  @param type Type of the primitive to be rendered.
                 */
                virtual void begin(RenderType type);

                /** End the primitive currently being rendered.
                 */
                virtual void end();

                /** Check if we are between a begin() and an end() call.
                 */
                virtual bool isBusy() const;

                /** Add a vector to the primitive.
                 *  @param vector Vector to add to the primitive.
                 */
                virtual void vector(const math::Vector2D &vector) = 0;
        };
    };
};

#endif
