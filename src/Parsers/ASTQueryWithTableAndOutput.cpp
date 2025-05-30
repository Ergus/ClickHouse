#include <Parsers/ASTIdentifier.h>
#include <Parsers/ASTQueryWithTableAndOutput.h>
#include <Parsers/IAST_erase.h>


namespace DB
{

String ASTQueryWithTableAndOutput::getDatabase() const
{
    String name;
    tryGetIdentifierNameInto(database, name);
    return name;
}

String ASTQueryWithTableAndOutput::getTable() const
{
    String name;
    tryGetIdentifierNameInto(table, name);
    return name;
}

void ASTQueryWithTableAndOutput::setDatabase(const String & name)
{
    if (database)
    {
        std::erase(children, database);
        database.reset();
    }

    if (!name.empty())
    {
        database = std::make_shared<ASTIdentifier>(name);
        children.push_back(database);
    }
}

void ASTQueryWithTableAndOutput::setTable(const String & name)
{
    if (table)
    {
        std::erase(children, table);
        table.reset();
    }

    if (!name.empty())
    {
        table = std::make_shared<ASTIdentifier>(name);
        children.push_back(table);
    }
}

void ASTQueryWithTableAndOutput::cloneTableOptions(ASTQueryWithTableAndOutput & cloned) const
{
    if (database)
    {
        cloned.database = database->clone();
        cloned.children.push_back(cloned.database);
    }
    if (table)
    {
        cloned.table = table->clone();
        cloned.children.push_back(cloned.table);
    }
}

}
