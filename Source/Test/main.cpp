#include <iostream>
#include <Core/CChangeBuffer.h>
#include <Core/CNullStorage.h>
#include <Core/CFile.h>

int main(int argc, char* argv[])
{
    using namespace Core;

    std::unique_ptr<IDataStorage> s(new CFile(TEXT("D:\\test.txt"), CFile::MODE_READ));

    CChangeBuffer buf(std::move(s));


    buf.Delete(125, 100);

    char cb[4096];
    memset(&cb, 0, sizeof(cb));

    auto size = static_cast<std::size_t>(buf.GetSize());
    auto t = buf.Read(0, cb, size);

    while (t->GetState() == ITracker::STATE_PENDING) {
        std::cout << "pending\n";
    }

    std::cout << cb;
    std::cout.flush();

    return 0;
}
