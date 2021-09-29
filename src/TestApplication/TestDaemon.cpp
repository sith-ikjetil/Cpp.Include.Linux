///////////////////////////////////////////////////////////////////
//: Title        : TestDaemon.cpp
//: Product:     : Cpp.Include.Windows
//: Date         : 2020-05-01
//: Author       : "Kjetil Kristoffer Solberg" <post@ikjetil.no>
//: Version      : 1.0.0.0
//: Descriptions : Implementation of Cpp.Include.Window Test Daemon Application.
//
// #include
//
#include "../include/itsoftware-linux.h"
#include "../include/itsoftware-linux-core.h"

//
// using
//
using ItSoftware::Linux::Core::ItsDaemon;
using ItSoftware::Linux::Core::ItsFile;

//
// Function: main
//
// (i): C/C++ application entry point.
//
int main(int argc, char* argv[]) 
{
    //
    // This makes this app a daemon
    //
    ItsDaemon daemon;

    //
    // Sanity check that daemon is successfull.
    //
    if (!daemon.IsDaemonSuccessfull()) {
        //
        // Return daemon unsuccessfull
        //
        return EXIT_FAILURE;
    }    

    //
    // Implement your daemon logic here. 
    // This is only an example: appending lines to a file.
    //
    while (!ItsDaemon::get_SIGKILL()) {
        ItsFile file;
        if (!file.OpenOrCreate("/home/kjetilso/daemon.txt","rwa",ItsFile::CreateMode("rw","rw","rw")))
        {
            return EXIT_FAILURE;
        }

        char text[] = "Cpp.Include.Linux TestDaemon 12345\n";
        size_t written(0);
        if ( !file.Write((void*)text,strlen(text), &written) )
        {
            return EXIT_FAILURE;
        }

        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    //
    // Return all ok
    //
    return EXIT_SUCCESS;
}
