#include <gtest/gtest.h>

#include <fstream>

#include <json.hpp>
#include <variant.hpp>

#include <mtx.hpp>

using json = nlohmann::json;

namespace ns = mtx::responses;
namespace me = mtx::events;

TEST(Responses, State)
{
        json data = R"({
	  "events": [
	    { 
	      "unsigned": {
	        "age": 242352,
	        "transaction_id": "txnid"
	      },
	      "content": {
 	        "aliases": [
	          "#somewhere:localhost",
	          "#another:localhost"
	        ]
	      },
	      "event_id": "$WLGTSEFSEF:localhost",
	      "origin_server_ts": 1431961217939,
              "room_id": "!Cuyf34gef24t:localhost",
	      "sender": "@example:localhost",
	      "state_key": "localhost",
	      "type": "m.room.aliases"
	    },
	    { 
	      "unsigned": {
	        "age": 242352,
	        "transaction_id": "txnid"
	      },
	      "content": {
 	        "name": "Random name"
	      },
	      "event_id": "$WLGTSEFSEF2:localhost",
	      "origin_server_ts": 1431961217939,
              "room_id": "!Cuyf34gef24t:localhost",
	      "sender": "@example2:localhost",
	      "state_key": "localhost",
	      "type": "m.room.name"
	    }
	  ]
	})"_json;

        ns::State state = data;

        EXPECT_EQ(state.events.size(), 2);

        auto aliases = mpark::get<me::StateEvent<me::state::Aliases>>(state.events[0]);
        EXPECT_EQ(aliases.event_id, "$WLGTSEFSEF:localhost");
        EXPECT_EQ(aliases.type, me::EventType::RoomAliases);
        EXPECT_EQ(aliases.sender, "@example:localhost");
        EXPECT_EQ(aliases.content.aliases.size(), 2);
        EXPECT_EQ(aliases.content.aliases[0], "#somewhere:localhost");

        auto name = mpark::get<me::StateEvent<me::state::Name>>(state.events[1]);
        EXPECT_EQ(name.event_id, "$WLGTSEFSEF2:localhost");
        EXPECT_EQ(name.type, me::EventType::RoomName);
        EXPECT_EQ(name.sender, "@example2:localhost");
        EXPECT_EQ(name.content.name, "Random name");
}

TEST(Responses, Timeline) {}
TEST(Responses, JoinedRoom) {}
TEST(Responses, LeftRoom) {}

TEST(Responses, Sync)
{
        std::ifstream file("./fixtures/responses/sync.json");

        json data;
        file >> data;

        ns::Sync sync = data;

        EXPECT_EQ(sync.next_batch,
                  "s333358558_324502987_444424_65663508_21685260_193623_2377336_2940807_454");
        EXPECT_EQ(sync.rooms.join.size(), 5);

        auto nheko = sync.rooms.join["!BPvgRcBVHzyFSlYkrg:matrix.org"];
        EXPECT_GT(nheko.state.events.size(), 0);
        EXPECT_GT(nheko.timeline.events.size(), 0);
        EXPECT_EQ(nheko.timeline.limited, true);
        EXPECT_EQ(nheko.timeline.prev_batch,
                  "t10853-333025362_324502987_444424_65663508_21685260_193623_2377336_2940807_454");

        EXPECT_EQ(sync.rooms.leave.size(), 1);
        EXPECT_EQ(sync.rooms.invite.size(), 0);
}

TEST(Responses, Rooms) {}

TEST(Responses, Profile)
{
        json response = R"({
	  "avatar_url": "mxc://matrix.org/SDGdghriugerRg",
	  "displayname": "Alice Margatroid"
        })"_json;

        json null_response = R"({
	  "avatar_url": "mxc://matrix.org/SDGdghriugerRg",
	  "displayname": null
        })"_json;

        json missing_response = R"({
	  "displayname": "Alice Margatroid"
        })"_json;

        json error_response = R"({
	  "displayname": 42
        })"_json;

        ns::Profile profile = response;
        EXPECT_EQ(profile.avatar_url, "mxc://matrix.org/SDGdghriugerRg");
        EXPECT_EQ(profile.display_name, "Alice Margatroid");

        ns::Profile null_profile = null_response;
        EXPECT_EQ(null_profile.avatar_url, "mxc://matrix.org/SDGdghriugerRg");
        EXPECT_EQ(null_profile.display_name, "");

        ns::Profile missing_profile = missing_response;
        EXPECT_EQ(missing_profile.avatar_url, "");
        EXPECT_EQ(missing_profile.display_name, "Alice Margatroid");

        ASSERT_THROW(ns::Profile error_profile = error_response, std::exception);
}

TEST(Responses, Versions)
{
        json data = R"({
	  "versions" : [
	    "r0.0.1",
	    "r0.2.0",
	    "r0.3.0"
	  ]
        })"_json;

        ns::Versions versions = data;
        EXPECT_EQ(versions.versions.size(), 3);
        EXPECT_EQ(versions.versions[0], "r0.0.1");
        EXPECT_EQ(versions.versions[1], "r0.2.0");
        EXPECT_EQ(versions.versions[2], "r0.3.0");

        json error_data = R"({
	  "versions" : [
	    "r.0.0.1"
	  ]
        })"_json;

        ASSERT_THROW(ns::Versions versions = error_data, std::invalid_argument);
}

TEST(Responses, Login)
{
        json data = R"({
          "user_id": "@cheeky_monkey:matrix.org",
          "access_token": "abc123", 
	  "home_server": "matrix.org",
          "device_id": "GHTYAJCE"
        })"_json;

        ns::Login login = data;
        EXPECT_EQ(login.user_id.toString(), "@cheeky_monkey:matrix.org");
        EXPECT_EQ(login.access_token, "abc123");
        EXPECT_EQ(login.home_server, "matrix.org");
        EXPECT_EQ(login.device_id, "GHTYAJCE");

        json data2 = R"({
          "user_id": "@cheeky_monkey:matrix.org",
          "access_token": "abc123", 
	  "home_server": "matrix.org"
        })"_json;

        ns::Login login2 = data2;
        EXPECT_EQ(login2.user_id.toString(), "@cheeky_monkey:matrix.org");
        EXPECT_EQ(login2.access_token, "abc123");
        EXPECT_EQ(login2.home_server, "matrix.org");
        EXPECT_EQ(login2.device_id, "");
}

TEST(Responses, Messages)
{
        json data = R"({
	"start": "t47429-4392820_219380_26003_2265",
	"end": "t47409-4357353_219380_26003_2265",
	"chunk": [{
	  "origin_server_ts": 1444812213737,
	  "sender": "@alice:example.com",
	  "event_id": "$1444812213350496Caaaa:example.com",
	  "content": {
	    "body": "hello world",
	    "msgtype": "m.text"
	  },
	  "room_id": "!Xq3620DUiqCaoxq:example.com",
	  "type": "m.room.message",
	  "age": 1042
	}, {
	  "origin_server_ts": 1444812194656,
	  "sender": "@bob:example.com",
	  "event_id": "$1444812213350496Cbbbb:example.com",
	  "content": {
	    "body": "the world is big",
	    "msgtype": "m.text"
	  },
	  "room_id": "!Xq3620DUiqCaoxq:example.com",
	  "type": "m.room.message",
	  "age": 20123
	}, {
	  "origin_server_ts": 1444812163990,
	  "sender": "@bob:example.com",
	  "event_id": "$1444812213350496Ccccc:example.com",
	  "content": {
	    "name": "New room name"
	  },
	  "prev_content": {
	    "name": "Old room name"
	  },
	  "state_key": "",
	  "room_id": "!Xq3620DUiqCaoxq:example.com",
	  "type": "m.room.name",
	  "age": 50789
	 }
	]})"_json;

        ns::Messages messages = data;
        EXPECT_EQ(messages.start, "t47429-4392820_219380_26003_2265");
        EXPECT_EQ(messages.end, "t47409-4357353_219380_26003_2265");
        EXPECT_EQ(messages.chunk.size(), 3);

        using mtx::events::RoomEvent;
        using mtx::events::StateEvent;
        using mtx::events::msg::Text;
        using mtx::events::state::Name;

        auto first_event = mpark::get<RoomEvent<Text>>(messages.chunk[0]);
        EXPECT_EQ(first_event.content.body, "hello world");
        EXPECT_EQ(first_event.content.msgtype, "m.text");
        EXPECT_EQ(first_event.type, mtx::events::EventType::RoomMessage);
        EXPECT_EQ(first_event.event_id, "$1444812213350496Caaaa:example.com");

        auto second_event = mpark::get<RoomEvent<Text>>(messages.chunk[1]);
        EXPECT_EQ(second_event.content.body, "the world is big");
        EXPECT_EQ(second_event.content.msgtype, "m.text");
        EXPECT_EQ(second_event.type, mtx::events::EventType::RoomMessage);
        EXPECT_EQ(second_event.event_id, "$1444812213350496Cbbbb:example.com");

        auto third_event = mpark::get<StateEvent<Name>>(messages.chunk[2]);
        EXPECT_EQ(third_event.content.name, "New room name");
        EXPECT_EQ(third_event.prev_content.name, "Old room name");
        EXPECT_EQ(third_event.type, mtx::events::EventType::RoomName);
        EXPECT_EQ(third_event.event_id, "$1444812213350496Ccccc:example.com");
        EXPECT_EQ(third_event.sender, "@bob:example.com");
}