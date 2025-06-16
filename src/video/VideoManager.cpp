#include "VideoManager.h"
#include "Renderer.h"
#include "ListManager.h"

#include <SDL2/SDL.h>
#include <stack>

using namespace jvgs::math;
using namespace std;

namespace jvgs
{
    namespace video
    {
        VideoManager::VideoManager()
        {
            SDL_InitSubSystem(SDL_INIT_VIDEO);
            window = nullptr;
            renderer = nullptr;
        }

        VideoManager::~VideoManager()
        {
            if (renderer) {
                SDL_DestroyRenderer(renderer);
                renderer = nullptr;
            }
            if (window) {
                SDL_DestroyWindow(window);
                window = nullptr;
            }
            SDL_QuitSubSystem(SDL_INIT_VIDEO);
        }

        VideoManager *VideoManager::getInstance()
        {
            static VideoManager instance;
            return &instance;
        }

        void VideoManager::setVideoMode(string title)
        {
            setVideoMode(Vector2D(800, 600), title);
        }

        void VideoManager::setVideoMode(const Vector2D &newSize, string title)
        {
            size = newSize;
            
            if (window) {
                SDL_DestroyWindow(window);
            }
            if (renderer) {
                SDL_DestroyRenderer(renderer);
            }
            
            window = SDL_CreateWindow(title.c_str(),
                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                (int)size.getX(), (int)size.getY(),
                SDL_WINDOW_SHOWN);
                
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
                
            setVideoDefaults();
        }

        void VideoManager::setVideoMode(const Vector2D &newSize)
        {
            setVideoMode(newSize, "JVGS");
        }

        void VideoManager::setVideoDefaults()
        {
            if (renderer) {
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            }
            identity();
            setColor(Color(0.0f, 0.0f, 0.0f, 1.0f));        // Black lines
            setClearColor(Color(1.0f, 1.0f, 1.0f, 1.0f));   // White background
        }

        const Vector2D &VideoManager::getSize() const
        {
            return size;
        }

        void VideoManager::clear() const
        {
            if (renderer) {
                SDL_SetRenderDrawColor(renderer, 
                    (Uint8)(clearColor.getRed() * 255),
                    (Uint8)(clearColor.getGreen() * 255),
                    (Uint8)(clearColor.getBlue() * 255),
                    (Uint8)(clearColor.getAlpha() * 255));
                SDL_RenderClear(renderer);
            }
        }

        void VideoManager::flip() const
        {
            if (renderer) {
                SDL_RenderPresent(renderer);
            }
        }

        void VideoManager::identity()
        {
            currentMatrix = AffineTransformationMatrix();
        }

        void VideoManager::push()
        {
            matrixStack.push(currentMatrix);
        }

        void VideoManager::pop()
        {
            if (!matrixStack.empty()) {
                currentMatrix = matrixStack.top();
                matrixStack.pop();
            }
        }

        void VideoManager::translate(const Vector2D &vector)
        {
            // Record translation if we're recording a display list
            ListManager* lm = ListManager::getInstance();
            if (lm->getIsRecording()) {
                lm->recordTranslate(vector);
            }
            
            currentMatrix.translate(vector);
        }

        void VideoManager::scale(const Vector2D &scale)
        {
            currentMatrix.scale(scale);
        }

        void VideoManager::rotate(const float &degrees)
        {
            currentMatrix.rotate(degrees);
        }

        void VideoManager::transform(const AffineTransformationMatrix &matrix)
        {
            currentMatrix *= matrix;
        }

        void VideoManager::setColor(const Color &newColor)
        {
            color = newColor;
        }

        const Color &VideoManager::getColor() const
        {
            return color;
        }

        void VideoManager::setClearColor(const Color &newClearColor)
        {
            clearColor = newClearColor;
        }

        const Color &VideoManager::getClearColor() const
        {
            return clearColor;
        }

        void VideoManager::invert()
        {
            Color tmp = color;
            setColor(clearColor);
            setClearColor(tmp);
        }
        
        // SDL2-specific methods that other parts need
        SDL_Renderer* VideoManager::getSDLRenderer() const
        {
            return renderer;
        }
        
        const AffineTransformationMatrix& VideoManager::getCurrentMatrix() const
        {
            return currentMatrix;
        }
    }
}