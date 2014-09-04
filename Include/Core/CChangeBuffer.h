#ifndef CORE_CCHANGEBUFFER_H
#define CORE_CCHANGEBUFFER_H

#include <memory>
#include <cstddef>
#include <Core/IDataStorage.h>

namespace Core {
    /**
     * A class that remembers all changes made to the file.
     */
    class CChangeBuffer
    {
    public:

        /**
         * @param storage Pointer to CFile instance available for read access
         * or CNullStorage if the goal is to create a new file in the end.
         */
        explicit CChangeBuffer(std::unique_ptr<IDataStorage> storage);

        ~CChangeBuffer();

        // TODO: Insert

        // TODO: Delete

        // TODO: Update

    private:

        // TODO: Block tree.
        // Blocks can be either projections of the data from in m_storage
        // or separatelty allocated data.

        std::unique_ptr<IDataStorage> m_storage;
    };
}

#endif // CORE_CCHANGEBUFFER_H
