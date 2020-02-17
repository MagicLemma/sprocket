#include <iostream>
#include <memory>

#include <Sprocket.h>
/*
class ExampleLayer : public Sprocket::Layer
{
public:
	ExampleLayer() : Layer("Example") {};

	void onEvent(Sprocket::Event& event) override
	{
		//Sprocket::Event& e2 = event;
		//std::string str = event.toString();
		//SPKT_LOG_INFO("Got event {}", event.toString());
	}
};
*/
class Workshop : public Sprocket::Application
{
};

int main(int argc, char* argv[])
{
    Workshop w;
    return Sprocket::begin(argc, argv, &w);
}