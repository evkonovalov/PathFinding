#include <iostream>

#include "tinyxml.h"
#include "tinystr.h"

using namespace std;

int main()
{

    cout << "Hello World!" << endl;
    TiXmlDocument doc( "example1.xml" );
    bool k = doc.LoadFile();
    if(k)
        doc.Print();
    else
        cout << "kek" << endl;
    return 0;
}

