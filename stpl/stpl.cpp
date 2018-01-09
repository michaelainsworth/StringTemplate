#include <iostream>
#include <StringTemplate/StringTemplate.hpp>
using namespace std;
using namespace StringTemplate;

#define STPL_VERSION "0.1"

int help();

int main(int argc, const char**argv)
{
    try
    {
        bool gotFilename = false;
        String filename;

        int i = 1;
        while (i < argc)
        {
            String arg(argv[i]);

            if ("-h" == arg || "--help" == arg)
            {
                return help();
            }
            else if ("-f" == arg || "--filename" == arg)
            {
                if (i >= argc - 1)
                {
                    throw runtime_error
                    (
                        "Missing --filename argument."
                    );
                }

                filename = argv[++i];
                gotFilename = true;
            }
            else
            {
                throw runtime_error
                (
                    "Unknown argument. Use --help for usage."
                );
            }

            ++i;
        }

        if (!gotFilename)
        {
            throw runtime_error
            (
                "Incorrect usage. Use --help for usage."
            );
        }

        Template tpl(filename);
        tpl.render(cout, false);

        return EXIT_SUCCESS;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}

int help()
{
    cout << "stpl version " << STPL_VERSION << ", ";
    cout << "library version " << STRINGTEMPLATE_VERSION << "\n";

    cout << "Usage: \n";
    cout << "    -h,--help\n";
    cout << "        Print this help message.\n";
    cout << "    -f,--filename [filename]\n";
    cout << "        Process filename.\n";

    return EXIT_SUCCESS;
}
