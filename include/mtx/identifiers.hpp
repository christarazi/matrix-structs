#pragma once

#include <exception>
#include <iostream>

namespace mtx {
namespace identifiers {

//! Base class for all the identifiers.
//
//! Each identifier has the following format `(sigil)``(localpart)`:`(hostname)`.
class ID
{
public:
        //! Returns the unique local part of the identifier.
        std::string localpart() const { return localpart_; }
        //! Returns the name of the originating homeserver.
        std::string hostname() const { return hostname_; }
        //! Returns the whole identifier (localpart + hostname).
        std::string toString() const { return id_; }

protected:
        //! Local part of the identifier.
        std::string localpart_;
        //! The name of the originating homeserver.
        std::string hostname_;
        //! The whole identifier.
        std::string id_;
};

class Event : public ID
{
public:
        template<typename Identifier>
        friend Identifier parse(const std::string &id);

private:
        //! The `sigil` used to represent an Event.
        std::string sigil = "$";
};

class Room : public ID
{
public:
        template<typename Identifier>
        friend Identifier parse(const std::string &id);

private:
        std::string sigil = "!";
};

class User : public ID
{
public:
        template<typename Identifier>
        friend Identifier parse(const std::string &id);

private:
        std::string sigil = "@";
};

//! Parses the given string into a @p Identifier.
//! \param id String to parse.
//! \returns The parsed @p Identifier.
//! \throws std::invalid_argument in case of invalid input.
template<typename Identifier>
Identifier
parse(const std::string &id)
{
        Identifier identifier;

        if (id.empty()) {
                // FIXME: enable logging only in debug mode.
                /* std::cout << "mtx::identifiers - Empty matrix identifier was given" << std::endl;
                 */
                return identifier;
        }

        if (std::string(1, id.at(0)) != identifier.sigil)
                throw std::invalid_argument(
                  std::string(id + ": missing sigil " + identifier.sigil + "\n"));

        const auto parts = id.find_first_of(':');

        // Split into localpart and server.
        if (parts != std::string::npos) {
                identifier.localpart_ = id.substr(1, parts - 1);
                identifier.hostname_  = id.substr(parts + 1);
                identifier.id_        = id;
        } else {
                throw std::invalid_argument(id + ": invalid format\n");
        }

        return identifier;
}

} // namespace identifiers
} // namespace mtx
