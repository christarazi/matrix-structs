#include <string>

#include "mtx/events/encrypted.hpp"

namespace mtx {
namespace events {
namespace msg {

void
from_json(const json &obj, Encrypted &content)
{
        content.algorithm  = obj.at("algorithm").get<std::string>();
        content.ciphertext = obj.at("ciphertext").get<std::string>();
        content.device_id  = obj.at("device_id").get<std::string>();
        content.sender_key = obj.at("sender_key").get<std::string>();
        content.session_id = obj.at("session_id").get<std::string>();
}

void
to_json(json &obj, const Encrypted &content)
{
        obj["algorithm"]  = "m.megolm.v1.aes-sha2";
        obj["ciphertext"] = content.ciphertext;
        obj["device_id"]  = content.device_id;
        obj["sender_key"] = content.sender_key;
        obj["session_id"] = content.session_id;
}

} // namespace msg
} // namespace events
} // namespace mtx
