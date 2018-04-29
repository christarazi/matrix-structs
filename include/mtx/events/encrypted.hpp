#pragma once

#include <json.hpp>

using json = nlohmann::json;

namespace mtx {
namespace events {
namespace msg {

//! Content of the `m.room.encrypted` event.
//! Endpoint is `PUT /_matrix/client/r0/rooms/{roomId}/send/m.room.encrypted/{txnId}`.
struct Encrypted
{
        //! Used for one-on-one exchanges.
        std::string algorithm = "m.megolm.v1.aes-sha2";
        //! The actual encrypted payload.
        std::string ciphertext;
        //! Sender's device id.
        std::string device_id;
        //! The curve25519 device key.
        std::string sender_key;
        //! Outbound group session id.
        std::string session_id;
};

void
to_json(json &obj, const Encrypted &event);

void
from_json(const json &obj, Encrypted &event);

} // namespace msg
} // namespace events
} // namespace mtx
