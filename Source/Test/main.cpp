#include <iostream>
#include <Core/CFile.h>

int main(int argc, char* argv[])
{
    using Core::CFile;
    using Core::ITracker;

    CFile file(TEXT("d:\\test.exe"), CFile::MODE_READ);

    if (file.IsOpen()) {
        std::cout << "Is open!\n";

        std::unique_ptr<char> buf(new char[file.GetSize()]);        
        std::cout << "File size = " << file.GetSize() << std::endl;

        auto tracker = file.Read(0, file.GetSize(), buf.get());
        std::cout << tracker->GetState() << std::endl;

        while (ITracker::STATE_PENDING == tracker->GetState()) {
            std::cout << "Waiting...\n";
            std::cout.flush();
        }

    } else {
        std::cout << "Cannot open!\n";
    }

    return 0;
}
