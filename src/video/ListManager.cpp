#include "ListManager.h"
#include "VideoManager.h"
#include "../math/Vector2D.h"

using namespace jvgs::math;

namespace jvgs
{
    namespace video
    {
        ListManager::ListManager() : nextListId(1), currentRecordingList(0), isRecording(false), isDestroying(false)
        {
        }

        ListManager::~ListManager()
        {
            isDestroying = true;
        }

        ListManager* ListManager::getInstance()
        {
            static ListManager instance;
            return &instance;
        }

        ListManager::List ListManager::createLists(int number)
        {
            List baseId = nextListId;
            for (int i = 0; i < number; i++) {
                lists[nextListId] = std::make_shared<DisplayList>();
                nextListId++;
            }
            return baseId;
        }

        void ListManager::beginList(const List &list)
        {
            if (lists.find(list) != lists.end()) {
                currentRecordingList = list;
                isRecording = true;
                // Clear any existing data
                lists[list]->operations.clear();
            }
        }

        void ListManager::endList()
        {
            isRecording = false;
            currentRecordingList = 0;
        }

        void ListManager::callList(const List &list) const
        {
            auto it = lists.find(list);
            if (it != lists.end() && it->second) {
                VideoManager* vm = VideoManager::getInstance();
                SDL_Renderer* renderer = vm->getSDLRenderer();
                
                if (!renderer) return;
                
                // Get current transformation matrix
                const auto& matrix = vm->getCurrentMatrix();
                
                // Get current drawing color (may have been inverted)
                const Color& currentColor = vm->getColor();
                SDL_SetRenderDrawColor(renderer,
                    (Uint8)(currentColor.getRed() * 255),
                    (Uint8)(currentColor.getGreen() * 255),
                    (Uint8)(currentColor.getBlue() * 255),
                    (Uint8)(currentColor.getAlpha() * 255));
                
                // Replay all cached operations with current transformation and color
                for (const auto& op : it->second->operations) {
                    if (op.type == OP_LINE) {
                        const auto& line = op.line;
                        // Apply transformation to cached coordinates
                        auto v1 = matrix * Vector2D(line.x1, line.y1);
                        auto v2 = matrix * Vector2D(line.x2, line.y2);
                        
                        SDL_RenderDrawLineF(renderer, v1.getX(), v1.getY(), v2.getX(), v2.getY());
                    } else if (op.type == OP_TRANSLATE) {
                        // Apply the recorded translation to current matrix
                        vm->translate(op.transform.translation);
                    }
                }
            }
        }

        void ListManager::callLists(const List &base, unsigned char *lists, int number) const
        {
            for (int i = 0; i < number; i++) {
                callList(base + lists[i]);
            }
        }

        void ListManager::deleteLists(const List &list, int number)
        {
            // Don't try to delete from map if we're being destroyed
            if (isDestroying) {
                return;
            }
            
            for (int i = 0; i < number; i++) {
                lists.erase(list + i);
            }
        }

        void ListManager::recordLine(float x1, float y1, float x2, float y2)
        {
            if (isRecording && lists.find(currentRecordingList) != lists.end()) {
                DisplayListOp op;
                op.type = OP_LINE;
                op.line = {x1, y1, x2, y2};
                lists[currentRecordingList]->operations.push_back(op);
            }
        }
        
        void ListManager::recordTranslate(const Vector2D& translation)
        {
            if (isRecording && lists.find(currentRecordingList) != lists.end()) {
                DisplayListOp op;
                op.type = OP_TRANSLATE;
                op.transform = {translation};
                lists[currentRecordingList]->operations.push_back(op);
            }
        }
    }
}