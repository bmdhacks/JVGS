#include "SketchyRenderer.h"
#include "VideoManager.h"
#include "ListManager.h"
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
            ListManager* lm = ListManager::getInstance();
            
            if (vectorList.empty()) {
                return;
            }

            Color color = vm->getColor();
            
            int repeat = MathManager::getInstance()->randInt(1, 10);
            int start = 0, end = (int) vectorList.size();
            while(repeat > 0) {
                
                for(int i = start; i < end - 1; i++) {
                    Vector2D v1 = vectorList[i];
                    Vector2D v2 = vectorList[i + 1];
                    
                    float x1 = v1.getX() + noiseX->nextValue();
                    float y1 = v1.getY() + noiseY->nextValue();
                    float x2 = v2.getX() + noiseX->nextValue();
                    float y2 = v2.getY() + noiseY->nextValue();
                    
                    // Record line if we're recording a display list, otherwise draw directly
                    if (lm->getIsRecording()) {
                        // Store raw coordinates - transformation and color applied during callList
                        lm->recordLine(x1, y1, x2, y2);
                    } else {
                        // Apply current transformation matrix for direct rendering
                        const AffineTransformationMatrix& matrix = vm->getCurrentMatrix();
                        Vector2D tv1 = matrix * Vector2D(x1, y1);
                        Vector2D tv2 = matrix * Vector2D(x2, y2);
                        
                        SDL_Renderer* renderer = vm->getSDLRenderer();
                        if (renderer) {
                            // Choose blend mode based on whether we're drawing light or dark lines
                            SDL_BlendMode oldBlendMode;
                            SDL_GetRenderDrawBlendMode(renderer, &oldBlendMode);
                            
                            float brightness = color.getRed() + color.getGreen() + color.getBlue();
                            
                            if (brightness < 1.5f) {
                                // Dark lines (black) - use multiplicative to darken white background
                                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_MOD);
                                SDL_SetRenderDrawColor(renderer,
                                    (Uint8)(255 * 0.7f),  // Light gray that darkens when multiplied
                                    (Uint8)(255 * 0.7f),
                                    (Uint8)(255 * 0.7f),
                                    (Uint8)(color.getAlpha() * 255));
                            } else {
                                // Light lines (white) - use additive to brighten black background
                                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);
                                SDL_SetRenderDrawColor(renderer,
                                    (Uint8)(color.getRed() * 255 * 0.3f),
                                    (Uint8)(color.getGreen() * 255 * 0.3f),
                                    (Uint8)(color.getBlue() * 255 * 0.3f),
                                    (Uint8)(color.getAlpha() * 255));
                            }
                            
                            // Draw multiple lines with slight offsets for antialiasing effect
                            float offsets[] = {0.0f, 0.3f, -0.3f, 0.6f, -0.6f};
                            for (float offset : offsets) {
                                SDL_RenderDrawLineF(renderer, 
                                    tv1.getX() + offset, tv1.getY() + offset, 
                                    tv2.getX() + offset, tv2.getY() + offset);
                            }
                            
                            // Restore original blend mode
                            SDL_SetRenderDrawBlendMode(renderer, oldBlendMode);
                        }
                    }
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