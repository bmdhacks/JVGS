#include "SketchyRenderer.h"
#include "VideoManager.h"
#include "../math/MathManager.h"
#include <cstdlib>

using namespace jvgs::math;

namespace jvgs
{
    namespace video
    {
        SketchyRenderer::SketchyRenderer()
        {
            noiseX = new Noise(-2.0f, 2.0f, 5);
            noiseY = new Noise(-2.0f, 2.0f, 5);
        }

        SketchyRenderer::~SketchyRenderer()
        {
            delete noiseX;
            delete noiseY;
        }

        void SketchyRenderer::begin(RenderType type)
        {
            Renderer::begin(type);
            vectorList.clear();
            noiseX->seed();
            noiseY->seed();
        }

        void SketchyRenderer::end()
        {
            Renderer::end();

            VideoManager* vm = VideoManager::getInstance();
            SDL_Renderer* renderer = vm->getSDLRenderer();
            
            if (!renderer) {
                return;
            }
            if (vectorList.empty()) {
                return;
            }
            


            Color color = vm->getColor();
            SDL_SetRenderDrawColor(renderer,
                (Uint8)(color.getRed() * 255),
                (Uint8)(color.getGreen() * 255),
                (Uint8)(color.getBlue() * 255),
                (Uint8)(color.getAlpha() * 255));

            // Apply current transformation matrix to all vectors
            const AffineTransformationMatrix& matrix = vm->getCurrentMatrix();

            int repeat = MathManager::getInstance()->randInt(1, 10);
            int start = 0, end = (int) vectorList.size();
            while(repeat > 0) {
                
                for(int i = start; i < end - 1; i++) {
                    Vector2D v1 = vectorList[i];
                    Vector2D v2 = vectorList[i + 1];
                    
                    // Transform vectors using current matrix
                    v1 = matrix * v1;
                    v2 = matrix * v2;
                    
                    float x1 = v1.getX() + noiseX->nextValue();
                    float y1 = v1.getY() + noiseY->nextValue();
                    float x2 = v2.getX() + noiseX->nextValue();
                    float y2 = v2.getY() + noiseY->nextValue();
                    
                    SDL_RenderDrawLineF(renderer, x1, y1, x2, y2);
                }

                start = MathManager::getInstance()->randInt(0,
                        (int) vectorList.size());
                end = MathManager::getInstance()->randInt(0,
                        (int) vectorList.size());

                if(start > end) {
                    int tmp = start;
                    start = end;
                    end = tmp;
                }

                repeat--;
            }
        }

        void SketchyRenderer::vector(const Vector2D &vector)
        {
            vectorList.push_back(vector);
        }
    }
}