#include <iostream>
#include <Core/CChangeBuffer.h>
#include <Core/CNullStorage.h>
#include <Core/CFile.h>

int main(int argc, char* argv[])
{
    using namespace Core;

    CFile* file = new CFile(TEXT("D:\\test.txt.txt"), CFile::MODE_READ);

    std::unique_ptr<IDataStorage> s(file);

    CChangeBuffer buf(std::move(s));

    buf.Insert(3, "Hello", 5);
    buf.Insert(130, "!!!UMBRA!!!", 11);
    buf.Insert(3, "---", 3);

    char cb[1024];
    memset(&cb, 0, sizeof(cb));

    auto t = buf.Read(0, cb, static_cast<std::size_t>(buf.GetSize()));

    while (t->GetState() == ITracker::STATE_PENDING) {
        std::cout << "pending\n";
    }

    std::cout << cb;

    return 0;
}
