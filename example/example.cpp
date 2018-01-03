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

Template getTemplate()
{
    Template tpl;
    auto tplPerson = tpl.addBlock("person");
    tplPerson.addText("First name: ");
    tplPerson.addSymbol("firstName");
    tplPerson.addText("\n");
    tplPerson.addText("Last name: ");
    tplPerson.addSymbol("lastName");
    tplPerson.addText("\n");
    auto tplDrive = tplPerson.addBlock("isAllowedToDrive");
    tplDrive.addText("Drive away.\n");

    auto tplNoDrive = tplPerson.addBlock("isNotAllowedToDrive");
    tplNoDrive.addText("Ride a bike.\n");

    tplPerson.addText("\n");
    return tpl;
}

int main(int argc, const char**argv)
{
    auto people = getPeople();
    auto tpl = getTemplate();

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

