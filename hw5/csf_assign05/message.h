#ifndef MESSAGE_H
#define MESSAGE_H

#include <vector>
#include <string>
struct Message {
  // An encoded message may have at most this many characters,
  // including the trailing newline ('\n'). Note that this does
  // *not* include a NUL terminator (if one is needed to
  // temporarily store the encoded message.)
  static const unsigned MAX_LEN = 255;

  std::string tag;
  std::string data;

  Message() { }

  Message(const std::string &tag, const std::string &data)
    : tag(tag), data(data) { }

  // TODO: you could add helper functions

  const std::string returnTag() const {
    return this->tag;
  }
  const std::string returnData() const {
    return this->data;
  }

  const char* messageToBuffer() const {
    const std::string tag = this->returnTag();
    const std::string data = this->returnData();
    const std::string total_message = tag + ":" + data + "\n";
    const char* message_c_string = total_message.c_str();
    return message_c_string;
  }

  Message bufferToMessage(char *buf) {

    std::string str(buf);
    size_t colonSeperator = str.find(':');

    const std::string tag = str.substr(0 , colonSeperator);
    const std::string data = str.substr(colonSeperator+1, str.length()-1);

    Message mes =  Message(tag, data);
    return mes;
  }

};

// standard message tags (note that you don't need to worry about
// "senduser" or "empty" messages)
#define TAG_ERR       "err"       // protocol error
#define TAG_OK        "ok"        // success response
#define TAG_SLOGIN    "slogin"    // register as specific user for sending
#define TAG_RLOGIN    "rlogin"    // register as specific user for receiving
#define TAG_JOIN      "join"      // join a chat room
#define TAG_LEAVE     "leave"     // leave a chat room
#define TAG_SENDALL   "sendall"   // send message to all users in chat room
#define TAG_SENDUSER  "senduser"  // send message to specific user in chat room
#define TAG_QUIT      "quit"      // quit
#define TAG_DELIVERY  "delivery"  // message delivered by server to receiving client
#define TAG_EMPTY     "empty"     // sent by server to receiving client to indicate no msgs available

#endif // MESSAGE_H
