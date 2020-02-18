#include "Entry.h"
#include "Layer.h"
#include "Log.h"
#include "Application.h"
#include "Events/Event.h"

int main(int argc, char* argv[])
{
    Sprocket::Application app;
    return Sprocket::begin(argc, argv, &app);

}