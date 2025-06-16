#ifndef JVGS_VIDEO_LISTMANAGER_H
#define JVGS_VIDEO_LISTMANAGER_H

#include <vector>
#include <map>
#include <memory>
#include <SDL2/SDL.h>
#include "Color.h"
#include "../math/Vector2D.h"

namespace jvgs
{
    namespace video
    {
        struct CachedLine {
            float x1, y1, x2, y2;
            Color color;
        };
        
        struct CachedTransform {
            jvgs::math::Vector2D translation;
        };
        
        enum DisplayListOpType {
            OP_LINE,
            OP_TRANSLATE
        };
        
        struct DisplayListOp {
            DisplayListOpType type;
            CachedLine line;           // Only used when type == OP_LINE
            CachedTransform transform; // Only used when type == OP_TRANSLATE
            
            DisplayListOp() : type(OP_LINE) {}
        };
        
        struct DisplayList {
            std::vector<DisplayListOp> operations;
        };

        class ListManager
        {
        private:
            std::map<unsigned int, std::shared_ptr<DisplayList>> lists;
            unsigned int nextListId;
            unsigned int currentRecordingList;
            bool isRecording;
            
        public:
            typedef unsigned int List;
            
            ListManager();
            ~ListManager();
            
            static ListManager* getInstance();
            
            List createLists(int number = 1);
            void beginList(const List &list);
            void endList();
            void callList(const List &list) const;
            void callLists(const List &base, unsigned char *lists, int number) const;
            void deleteLists(const List &list, int number = 1);
            
            // Recording interface for SketchyRenderer and VideoManager
            void recordLine(float x1, float y1, float x2, float y2, const Color& color);
            void recordTranslate(const jvgs::math::Vector2D& translation);
            bool getIsRecording() const { return isRecording; }
        };
    }
}

#endif