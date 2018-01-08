#include <iostream>
#include <vector>
#include <StringTemplate/StringTemplate.hpp>

using namespace std;
using namespace StringTemplate;

int main(int argc, const char**argv)
{
    try
    {
        if (argc != 2)
        {
            throw std::runtime_error
            (
                "You must supply a filename as argument 2."
            );
        }

        Template tpl(argv[1]);

        tpl.set("name", "John Doe");
        tpl.set("value", "$1,000");
        auto in_ca = tpl.block("in_ca");
        //in_ca.disable();
        in_ca.set("taxed_value", "$400");

        tpl.render(std::cout);

        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}

