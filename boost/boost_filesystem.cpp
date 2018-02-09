#include <boost\filesystem.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <assert.h>

namespace
{
    namespace fs = boost::filesystem;

    // copy_file does not create parent directory if it does not exist
    void f1()
    {
        const std::string from("file");
        std::ofstream f(from, std::fstream::out);
        f << "dummpy";
        f.close();

        fs::path folder("folder");
        fs::path to = folder / "file";
        try
        {
            fs::copy_file(from, to);
        }
        catch (fs::filesystem_error& e)
        {
            std::cout << e.what() << std::endl;
        }

        fs::create_directory(folder);
        fs::copy_file(from, to);
        assert(fs::is_regular_file(to));

        fs::remove(from);
        fs::remove_all(folder);
    }
}

void testFilesystem()
{
    f1();
}