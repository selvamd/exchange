#ifndef QFIXAPP_H
#define QFIXAPP_H

#include "quickfix/Application.h"
#include "quickfix/MessageCracker.h"
#include "quickfix/Values.h"
#include "quickfix/Utility.h"
#include "quickfix/Mutex.h"

#include "quickfix/fix40/NewOrderSingle.h"
#include "quickfix/fix41/NewOrderSingle.h"
#include "quickfix/fix42/NewOrderSingle.h"
#include "quickfix/fix43/NewOrderSingle.h"
#include "quickfix/fix44/NewOrderSingle.h"
#include "quickfix/fix50/NewOrderSingle.h"

class qfixapp : public FIX::Application, public FIX::MessageCracker
{
public:
  qfixapp() : m_orderID(0), m_execID(0) {}

  // Application overloads
  void onCreate( const FIX::SessionID& );
  void onLogon( const FIX::SessionID& sessionID );
  void onLogout( const FIX::SessionID& sessionID );
  void toAdmin( FIX::Message&, const FIX::SessionID& );
  void toApp( FIX::Message&, const FIX::SessionID& )
    EXCEPT( FIX::DoNotSend );
  void fromAdmin( const FIX::Message&, const FIX::SessionID& )
    EXCEPT( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::RejectLogon );
  void fromApp( const FIX::Message& message, const FIX::SessionID& sessionID )
    EXCEPT( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType );

  // MessageCracker overloads
  void onMessage( const FIX40::NewOrderSingle&, const FIX::SessionID& );
  void onMessage( const FIX41::NewOrderSingle&, const FIX::SessionID& );
  void onMessage( const FIX42::NewOrderSingle&, const FIX::SessionID& );
  void onMessage( const FIX43::NewOrderSingle&, const FIX::SessionID& );
  void onMessage( const FIX44::NewOrderSingle&, const FIX::SessionID& );
  void onMessage( const FIX50::NewOrderSingle&, const FIX::SessionID& );

  std::string genOrderID() {
    return std::to_string(++m_orderID);
  }
  std::string genExecID() {
    return std::to_string(++m_execID);
  }
private:
  int m_orderID, m_execID;
};

#endif
