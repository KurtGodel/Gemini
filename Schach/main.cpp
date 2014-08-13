
//
// Disclamer:
// ----------
//
// This code will work only if you selected window, graphics and audio.
//
// Note that the "Run Script" build phase will copy the required frameworks
// or dylibs to your application bundle so you can execute it on any OS X
// computer.
//
// Your resource files (images, sounds, fonts, ...) are also copied to your
// application bundle. To get the path to these resource, use the helper
// method resourcePath() from ResourcePath.hpp
//

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <mutex>
#include <thread>
#include "ResourcePath.hpp"
#include "Controller.h"
#include "AI.h"
#include "HashTable.h"

void* startAI(void* input)
{
    AI ai((RunAIInput*)input);
    while(true)
    {
        ai.run();
    }
}

int numberOfEvals = 0;


int main(int, char const**)
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Gemini", sf::Style::Close);
    
    srand(std::chrono::system_clock::now().time_since_epoch()/std::chrono::milliseconds(1)%10000);
    
    window.setFramerateLimit(40);
    
    unsigned int maxNumberOfThreads = std::thread::hardware_concurrency();
    maxNumberOfThreads = 1;
    
    // Set the Icon
    sf::Image icon;
    if (!icon.loadFromFile(resourcePath() + "icon.png")) {
        return EXIT_FAILURE;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    
    Board board;
    board.setRandomArr();
    
    HashTable transpositionTable;
    
    RunAIInput input;
    input.shouldAiBeRunning = false;
    input.transpositionTable = &transpositionTable;
    SearchAICommunicator searchCommunicator[4];
    
    if(maxNumberOfThreads != 1)
    {
        while(input.lock.try_lock()){}
        for(int i=0; i<maxNumberOfThreads; i++)
        {
            searchCommunicator[i].transpositionTable = &transpositionTable;
            input.communicator.push_back(&searchCommunicator[i]);
        }
        input.lock.unlock();
    }
    
    Controller controller = Controller(&input);

    pthread_t aiThread;
    pthread_create(&aiThread, NULL, startAI, (void *)(&input));
    
    bool commandLeft = false;
    bool commandRight = false;
    
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if(event.type == sf::Event::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::Escape)
                {
                    // Escape pressed : exit
                    while(input.lock.try_lock()){}
                    input.shouldAiBeRunning = false;
                    input.lock.unlock();
                    struct timespec tim, tim2;
                    tim.tv_sec = 0;
                    tim.tv_nsec = 10;      // milliseconds
                    tim.tv_nsec *= 1000000;
                    nanosleep(&tim , &tim2);

                    window.close();
                }
                else if(event.key.code == sf::Keyboard::LSystem)
                {
                    commandLeft = true;
                }
                else if(event.key.code == sf::Keyboard::RSystem)
                {
                    commandRight = true;
                }
                else if((commandLeft || commandRight) && (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Q))
                {
                    window.close();
                }
                else if(event.key.code == sf::Keyboard::W)
                {
                    if(commandLeft || commandRight)
                    {
                        window.close();
                    }
                }
                else
                {
                    controller.keyDown(event.key.code);
                }
            }
            else if(event.type == sf::Event::KeyReleased)
            {
                if(event.key.code == sf::Keyboard::LSystem)
                {
                    commandLeft = false;
                }
                else if(event.key.code == sf::Keyboard::RSystem)
                {
                    commandRight = false;
                }
                else
                {
                    controller.keyUp(event.key.code);
                }
            }
            else if(event.type == sf::Event::MouseMoved)
            {
                controller.mouseMove(event.mouseMove);
            }
            else if(event.type == sf::Event::MouseButtonPressed)
            {
                controller.mouseDown(event.mouseButton);
            }
            else if(event.type == sf::Event::MouseButtonReleased)
            {
                controller.mouseUp(event.mouseButton);
            }
            else if(event.type == sf::Event::Resized)
            {
                controller.resized(event.size);
            }
            else if(event.type == sf::Event::TextEntered)
            {
                controller.textEntered(event.text.unicode);
            }
        }

        // Clear screen
        window.clear();
        
        controller.frame(&window);
        
        // Update the window
        window.display();
    }

    return EXIT_SUCCESS;
}