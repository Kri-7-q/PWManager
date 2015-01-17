#include "options.h"
#include <QTextStream>


int main(int argc, char *argv[])
{
    QTextStream out(stdout);
    Options options(argv[1]);
    Account account = options.parseOptions(argc, argv);
    if (options.hasError()) {
        out << options.errorMessage();
    }

    return 1;
}
