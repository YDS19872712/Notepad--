#ifndef CORE_CFILE_H
#define CORE_CFILE_H

#include <windows.h>
#include <Core/ITracker.h>
#include <Core/IDataStorage.h>

namespace Core {
    /**
     * File class that implements IDataStorage interface.
     * @author Dmytro Yurchenko.
     */
    class CFile : public IDataStorage
    {
    public:
        /**
         * File mode flags.
         * May be combined with bitwise or operator.
         */
        enum Mode
        {
            MODE_READ  = 1,
            MODE_WRITE = 2
        };

        /**
         * A constructor which constructs an object by path.
         * @param path A path to the file.
         * @param mode Opening mode.
         */
        CFile(PCTSTR path, unsigned int mode);

        /**
         * Copy constructor.
         */
        CFile(const CFile& file);

        /**
         * Move constructor.
         */
        CFile(CFile&& file);

        /**
         * A destructor that destructs (Captain Obvious).
         */
        virtual ~CFile();

        /**
         * Copy operator.
         */
        CFile& operator =(const CFile& file);

        /**
         * Move operator.
         */
        CFile& operator =(CFile&& file);

        /**
         * Checks if file is open.
         */
        bool IsOpen() const;

        virtual unsigned long long GetSize() const;

        virtual std::unique_ptr<ITracker> Read(
            unsigned long long offset,
            void* buffer,
            std::size_t size) const;

        virtual std::unique_ptr<ITracker> Write(
            unsigned long long offset,
            const void* buffer,
            std::size_t size);

    private:

        void Close();

        HANDLE m_handle;
    };

    inline bool CFile::IsOpen() const
    {
        return INVALID_HANDLE_VALUE != m_handle;
    }

    inline void CFile::Close()
    {
        if (INVALID_HANDLE_VALUE != m_handle) {
            ::CloseHandle(m_handle);
        }
    }
}

#endif // CORE_CFILE_H
