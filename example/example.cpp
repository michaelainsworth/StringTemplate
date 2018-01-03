#include <iostream>
#include <vector>
#include <StringTemplate/StringTemplate.hpp>

using namespace std;
using namespace StringTemplate;

struct Person
{
    Person(const String& firstName_, const String& lastName_, bool allowedToDrive_)
        : firstName(firstName_), lastName(lastName_), allowedToDrive(allowedToDrive_)
    {}

    String firstName, lastName;
    bool allowedToDrive;
};

typedef std::vector<Person> PersonVector;

PersonVector getPeople()
{
    PersonVector people;
    people.push_back(Person("John", "Doe", true));
    people.push_back(Person("Jane", "Doe", false));
    people.push_back(Person("Michael", "Angelo", false));
    people.push_back(Person("Leonardo", "Davinci", true));
    return people;
}

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

        auto people = getPeople();
        Template tpl(argv[1]);

        auto tplPerson = tpl.block("person").repeat(people.size());
        for (auto person : people)
        {
            tplPerson.set("firstName", person.firstName);
            tplPerson.set("lastName", person.lastName);

            auto tplDrive = tplPerson.block("isAllowedToDrive");
            auto tplNoDrive = tplPerson.block("isNotAllowedToDrive");
            
            if (person.allowedToDrive)
            {
                tplDrive.enable();
                tplNoDrive.disable();
            }
            else
            {
                tplDrive.disable();
                tplNoDrive.enable();
            }

            tplPerson = tplPerson.next();
        }

        tpl.render(std::cout);

        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}

