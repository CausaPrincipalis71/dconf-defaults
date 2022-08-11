#include <QCoreApplication>
#include <QLocale>
#include <QTranslator>
#include <QCommandLineParser>
#include <QTextStream>

enum CommandLineParseResults {CommandLineOk = 0, CommandLineError, CommandLineHelpFound};

CommandLineParseResults setParserSettings(QCommandLineParser * parser, const QStringList &arguments, QString &helpText)
{
    QString programName = "dconf-defaults";
    QString availableArguments = QCommandLineParser::tr("Should be used with: "); // Moved to a separate QString for easier translation

    helpText += QCommandLineParser::tr("A program with the functionality of setting default dconf settings and blocking their changes") + "\n\n";
    helpText += QCommandLineParser::tr("USAGE:") + "\n";
    helpText += "\t" + programName + " " + QCommandLineParser::tr("COMMANDS [ARGUMENTS]") + "\n";

    QString commandName, commandDescription, commandArguments;

    // ***DATABASE OPTIONS***
    helpText += "\n" + QCommandLineParser::tr("Databases command:") + "\n";

    commandName = "list-databases";
    commandDescription = QCommandLineParser::tr("List of databases contained in /etc/dconf/profile");
    parser->addPositionalArgument(commandName, commandDescription);
    helpText += "\t" + commandName + "\t\t" + commandDescription + "\n";

    commandName = "init-db";
    commandDescription = QCommandLineParser::tr("Create new database");
    commandArguments = QCommandLineParser::tr("[Name of database]");
    parser->addPositionalArgument(commandName, commandDescription, commandArguments);
    helpText += "\t" + commandName + "\t\t\t" + commandDescription + "\n\t\t\t\t\t" + availableArguments + commandArguments + "\n";

    commandName = "remove-db";
    commandDescription = QCommandLineParser::tr("Delete existing database");
    commandArguments = QCommandLineParser::tr("[Name of database]");
    parser->addPositionalArgument(commandName, commandDescription, commandArguments);
    helpText += "\t" + commandName + "\t\t" + commandDescription + "\n\t\t\t\t\t" + availableArguments + commandArguments + "\n";

    commandName = "rename-db";
    commandDescription = QCommandLineParser::tr("Rename existing database");
    commandArguments = QCommandLineParser::tr("[Old name of database] [New name of database]");
    parser->addPositionalArgument(commandName, commandDescription, commandArguments);
    helpText += "\t" + commandName + "\t\t" + commandDescription + "\n\t\t\t\t\t" + availableArguments + commandArguments + "\n";

    // ***OTHER OPTIONS***
    helpText += "\n" + QCommandLineParser::tr("Other commands:") + "\n";

    commandName = "help";
    commandDescription = QCommandLineParser::tr("Show this message");
    parser->addPositionalArgument(commandName, commandDescription);
    helpText += "\t" + commandName + "\t\t" + commandDescription + "\n";

    parser->parse(arguments);

    if(parser->positionalArguments().contains("help"))
        return CommandLineHelpFound;
    else
        return CommandLineOk;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("dconf-defaults");

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "dconf-defaults_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    QTextStream out(stdout);

    QString helpText;

    QCommandLineParser *parser = new QCommandLineParser();

    auto parserResult = setParserSettings(parser, a.arguments(), helpText);

    switch (parserResult)
    {
        case CommandLineOk:
        {
            return a.exec();
            break;
        }
        case CommandLineHelpFound:
        {
            out << helpText << Qt::endl;
            return 0;
            break;
        }
        case CommandLineError:
        {
            out << "Error found" << Qt::endl;
            return 1;
            break;
        }
    }
}
