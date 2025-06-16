#ifndef JVGS_VIDEO_LISTMANAGER_H
#define JVGS_VIDEO_LISTMANAGER_H

namespace jvgs
{
    namespace video
    {
        class ListManager
        {
        public:
            // Stub ListManager - no longer used since we use live rendering
            typedef unsigned int List;
            
            static ListManager* getInstance() {
                static ListManager instance;
                return &instance;
            }
            
            List newList() { return 0; }
            void endList() {}
            void callList(const List &list) const {}
            void callLists(const List &base, unsigned char *lists, int number) const {}
            
            // Additional methods used by Path and Font classes
            void deleteLists(const List &list) {}
            void deleteLists(const List &base, int number) {}
            List createLists() { return 0; }
            List createLists(int number) { return 0; }
            void beginList(const List &list) {}
        };
    }
}

#endif