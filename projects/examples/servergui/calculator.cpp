#include "calculator.h"

#include <QTime>

Calculator::Calculator(Server *server)
  : ServerObserver(server),
    m_server(server)
{
}


Calculator::~Calculator()
{
}


Message
Calculator::createReply(const Message& message)
{
  //  std::cout << "Calculator::createReply() - connectionID: " << connectionId << std::endl;

//  QTime timer;
//  timer.start();

//  try
//  {
//    if ( message.getData() )
//    {
//      m_server->send("received empty message!", connectionId);
//      return;
//    }

//    if (dataStrings[0] == "add")
//    {
//      double sum = atof(dataStrings[1].c_str()) + atof(dataStrings[2].c_str());
//      double something = std::pow(sum, 4) * cos(sum) + sin(sum);
//      for (std::size_t i = 0; i < 10; ++i)
//      {
//        something += std::pow(something, 4) * cos(something) + sin(something);
//      }
//      m_server->send(std::to_string(sum), connectionId);
//    }
//    else
//    {
//      m_server->send("Hello!", connectionId);
//    }
//  }
//  catch (std::exception& e)
//  {
//    std::cerr << "Calculator::createReply() - write error: " << e.what() << std::endl;
//  }
//  std::cout << "Calculator::createReply() - connectionID: " << connectionId << " time to compute and write: " << timer.elapsed()<< std::endl;
}

